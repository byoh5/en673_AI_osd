//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"

#include "user.h"
#include "msg.h"
#include "phy.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/tcpip.h"
#include "lwip/netif.h"
#include "lwip/netifapi.h"
#include "lwip/dhcp.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"
#include "netif/etharp.h"

#include "networkif.h"

#include "init.h"

#ifdef __ETH__
#include "ipchange.h"
#include "ethernetif.h"
#endif
#ifdef __WIFI__
#include "wifi.h"
#include "wlif_tools.h"
#endif

#if defined(__ETH__) || defined(__WIFI__)

#if defined(__ETH__)
struct netif gtMyNetIf;	// ETH
#endif
#if defined(__WIFI__)
struct netif gtwlif_sta;// Wi-Fi STA mode
struct netif gtwlif_uap;// Wi-Fi UAP mode

xSemaphoreHandle xwlif_evt_sem = NULL;
#endif

xSemaphoreHandle xtxwl_sem = NULL;
xSemaphoreHandle xrxwl_sem = NULL;

IGMP_DATA igmp_list[IGMP_LIST_LENS];

netif_state_t netif_state[enlEnd] = { 
#if defined(__ETH__)
{	.ifname = "Ethernet",
	._netif = &gtMyNetIf,
	.xrx_sem = NULL,
	.iftype = enlETHERNET,	},
#endif
#if defined(__WIFI__)
{	.ifname = "Wi-Fi(STA)",
	._netif = &gtwlif_sta,
	.xrx_sem = NULL,
	.iftype = enlWIFISTA,	},
{	.ifname = "Wi-Fi(UAP)",
	._netif = &gtwlif_uap,
	.xrx_sem = NULL,
	.iftype = enlWIFIUAP,	},
#endif
};

err_t low_level_init(struct netif *netif, char name0, char name1)
{
	static int oneshot = 0;

	netif->name[0] = name0;
	netif->name[1] = name1;
	netif->hwaddr_len = NETIF_MAX_HWADDR_LEN;	// Set MAC hardware address length
	netif->mtu = 1500;							// Maximum transfer unit
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_IGMP;	// Don't set NETIF_FLAG_ETHARP if this device is not an ethernet one
#if defined(__ETH__)
	if (netif == &gtMyNetIf) {
		netif->flags |= NETIF_FLAG_UP;
	}
#endif

	if (!oneshot) {
		wltx_init_lock();
		wlrx_init_lock();
		oneshot = 1;
	}

	return ERR_OK;
}

/**
 * @param netif the lwip network interface structure for this ethernetif, wlif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *q = NULL, *p = NULL;
	BYTE *pkt;
	WORD wLen, nTotLen = 0;
	volatile pkt_info *pkinfo;
	
	if (wlrx_lock()) {
		if (cNRQueue_isempty(&gptMsgNetworkRX1) == DEF_FAIL) {
			portENTER_CRITICAL();

			pkinfo = &(gptMsgNetworkRX1.info[gptMsgNetworkRX1.head]);

			wLen = pkinfo->lenth;
			pkt = (BYTE *)pkinfo->data;

			if ((gptMsgDebug.dltFLAG & eDltCPU1ETHPROC) && (pkinfo->type == ETH_DATA)) {
				printf("b");
			} else if ((gptMsgDebug.dltFLAG & eDltCPU1UAPPROC) && (pkinfo->type == WLIF_DATA)) {
				printf("d");
			}

			p = pbuf_alloc(PBUF_RAW, wLen, PBUF_POOL);
			if (p == NULL) {
				printf("pbuf_alloc(ID:%d) fail H(%04d) T(%04d) Size(%04d)\r\n", CPU_ID, gptMsgNetworkRX1.head, gptMsgNetworkRX1.tail, wLen);
			} else {
				for (q = p; q != NULL; q = q->next) {
					if(q->len) { // Copy to "pbuf"
						DmaMemCpy_isr(q->payload, pkt + nTotLen, (UINT)q->len);
						invalidate_dcache_range((uint)(q->payload), (uint)(q->payload + q->len));
					}
					nTotLen += q->len;
				}
			}
			num_loop(gptMsgNetworkRX1.head, MSG_NETRX_NUM);

			portEXIT_CRITICAL();
		}
#if (ETH_EXCESSIVE_PACKETS_CK==1)
		if(cNRQueue_isempty(&gptMsgNetworkRX1) == DEF_OK) {
			if (ETH_RX_EN == 0) {
				ETH_RX_EN = 1;
			}
		}
#endif
		wlrx_unlock();
	}

	return p;
	UNUSED(netif);
}

/**
 * This function will be called by wlif_poll() when a packet has been received
 * from the mac. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void if_input(void *ctx)
{
	UINT inval_cnt = 0, old_inval_time = gptMsgShare.TIME;
	err_t ert;
	struct netif *netif = (struct netif *)ctx;
	struct eth_hdr *ethhdr;
	struct pbuf *p;
	netif_state_t *priv;

	priv = (netif_state_t *)netif->state;

	for (;;) {
		if (xSemaphoreTake(priv->xrx_sem, portMAX_DELAY) == pdTRUE) {
			while ((p = low_level_input(netif))) {
				// points to packet payload, which starts with an Ethernet header
				ethhdr = (struct eth_hdr *)p->payload;
				switch (htons(ethhdr->type)) {
					// IP or ARP packet ?
					case ETHTYPE_IP:
					case ETHTYPE_ARP:
#if (ETH_EXCESSIVE_PACKETS_CK==1)
						inval_cnt = 0;
#endif
						// full packet send to tcpip_thread to process
						if ((ert = netif->input(p, netif)) != ERR_OK) {
							LWIP_DEBUGF(NETIF_DEBUG, ("if_input: IP input error\r\n"));
							if (gptMsgDebug.dltFLAG & eDltCPU1ETHRX_err) {
								printf("    %04d:%s: IP input error(%d)\r\n", __LINE__, __func__, ert);
								hexDump("IP input error", p->payload, 16*4);								
							}
							pbuf_free(p);
							p = NULL;
						}
						break;
					case 0x0842: // Wake-on-LAN[9]
					case 0x22F3: // IETF TRILL Protocol
					case 0x22EA: // Stream Reservation Protocol
					case 0x6003: // DECnet Phase IV
					case 0x8035: // Reverse Address Resolution Protocol
					case 0x809B: // AppleTalk (Ethertalk)
					case 0x80F3: // AppleTalk Address Resolution Protocol (AARP)
					case 0x8100: // VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq with NNI compatibility[10]
					case 0x8137: // IPX
					case 0x8204: // QNX Qnet
					case 0x86DD: // Internet Protocol Version 6 (IPv6)
					case 0x8808: // Ethernet flow control
					case 0x8809: // Ethernet Slow Protocols[11]
					case 0x8819: // CobraNet
					case 0x8847: // MPLS unicast
					case 0x8848: // MPLS multicast
					case 0x8863: // PPPoE Discovery Stage
					case 0x8864: // PPPoE Session Stage
					case 0x886D: // Intel Advanced Networking Services [12]
					case 0x8870: // Jumbo Frames (Obsoleted draft-ietf-isis-ext-eth-01)
					case 0x887B: // HomePlug 1.0 MME
					case 0x888E: // EAP over LAN (IEEE 802.1X)
					case 0x8892: // PROFINET Protocol
					case 0x8899: // Realtek Remote Control Protocol
					case 0x889A: // HyperSCSI (SCSI over Ethernet)
					case 0x88A2: // ATA over Ethernet
					case 0x88A4: // EtherCAT Protocol
					case 0x88A8: // Provider Bridging (IEEE 802.1ad) & Shortest Path Bridging IEEE 802.1aq[10]
					case 0x88AB: // Ethernet Powerlink[citation needed]
					case 0x88B8: // GOOSE (Generic Object Oriented Substation event)
					case 0x88B9: // GSE (Generic Substation Events) Management Services
					case 0x88BA: // SV (Sampled Value Transmission)
					case 0x88CC: // Link Layer Discovery Protocol (LLDP)
					case 0x88CD: // SERCOS III
					case 0x88DC: // WSMP, WAVE Short Message Protocol
					case 0x88E1: // HomePlug AV MME[citation needed]
					case 0x88E3: // Media Redundancy Protocol (IEC62439-2)
					case 0x88E5: // MAC security (IEEE 802.1AE)
					case 0x88E7: // Provider Backbone Bridges (PBB) (IEEE 802.1ah)
					case 0x88F7: // Precision Time Protocol (PTP) over Ethernet (IEEE 1588)
					case 0x88FB: // Parallel Redundancy Protocol (PRP)
					case 0x8902: // IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
					case 0x8906: // Fibre Channel over Ethernet (FCoE)
					case 0x8914: // FCoE Initialization Protocol
					case 0x8915: // RDMA over Converged Ethernet (RoCE)
					case 0x891D: // TTEthernet Protocol Control Frame (TTE)
					case 0x892F: // High-availability Seamless Redundancy (HSR)
					case 0x9000: // Ethernet Configuration Testing Protocol[13]
					case 0x9100: // VLAN-tagged (IEEE 802.1Q) frame with double tagging
						LWIP_DEBUGF(NETIF_DEBUG, ("if_input: inval type\r\n"));
						pbuf_free(p);
						p = NULL;
						break;
					default:
#if (ETH_EXCESSIVE_PACKETS_CK==1)
						inval_cnt++;
						if (gptMsgDebug.dltFLAG & eDltCPU1ETHRX_err) {
							printf("    %04d:%s: Unknown packet(type:0x%04X) / Count(%d/sec)\r\n", __LINE__, __func__, htons(ethhdr->type), inval_cnt);
						}
						if (inval_cnt > 25) {
							// 1sec내에 Unknown packet이 25회 이상 입력되면 재부팅
							printf("	%04d:%s: ethernet packet error. reboot!\r\n", __LINE__, __func__);
							WdtSysReboot();
						}
						if (old_inval_time != gptMsgShare.TIME) {
							old_inval_time = gptMsgShare.TIME;
							inval_cnt = 0;
						}
#else
						UNUSED(inval_cnt);
						UNUSED(old_inval_time);
#endif
//						hexDump("IP input error", p->payload, p->len);
						LWIP_DEBUGF(NETIF_DEBUG, ("if_input: inval type\r\n"));
						pbuf_free(p);
						p = NULL;
						break;
				}
			} // end of while((p = low_level_input(netif)))
		}
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Check Network Interface
void network_check_netif(void)
{
	do {
		struct netif *nif = netif_list;
		for (nif = netif_list; nif != NULL; nif = nif->next) {
			if (nif && netif_is_up(nif) && netif_is_link_up(nif)) {
				return;
			}
		}
		vTaskDelay(10);
	} while(1);
}

void network_default_netif(void)
{
	const char *strDefnetif = "%s is set as default interface.\r\n";
#if defined(__ETH__)
	if (netif_is_up(&gtMyNetIf) && netif_is_link_up(&gtMyNetIf)) {
		if (netif_default != &gtMyNetIf) {
			netif_set_default(&gtMyNetIf);
			printf(strDefnetif, ((netif_state_t *)gtMyNetIf.state)->ifname);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_NET_DEFIF, enlETHERNET) == DEF_FAIL) 	vTaskDelay(1);
			return;
		}
	}
#endif
#ifdef __WIFI__
	if (netif_is_up(&gtwlif_sta) && netif_is_link_up(&gtwlif_sta)) {
		if (netif_default != &gtwlif_sta) {
			netif_set_default(&gtwlif_sta);
			printf(strDefnetif, ((netif_state_t *)gtwlif_sta.state)->ifname);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_NET_DEFIF, enlWIFISTA) == DEF_FAIL)	vTaskDelay(1);
			return;
		}
	}

	if (netif_is_up(&gtwlif_uap) && netif_is_link_up(&gtwlif_uap)) {
		if (netif_default != &gtwlif_uap) {
			netif_set_default(&gtwlif_uap);
			printf(strDefnetif, ((netif_state_t *)gtwlif_uap.state)->ifname);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_NET_DEFIF, enlWIFIUAP) == DEF_FAIL)	vTaskDelay(1);
			return;
		}
	}
#endif

	printf("%s: Set NULL netif!\r\n", __func__);
	netif_set_default(NULL);
}

static void tcpip_init_done_func(void *ctx)
{
#if defined(__ETH__)
	network_ethif_start();
#endif

	UNUSED(ctx);
}

void startLwip(void *arg)
{
#if defined(__ETH__)
	MdioInit();
	const char *strInit = "Phy/Eth Init";
	if (PhyInit(PHY_ADDR) == DEF_FAIL) { // PHY	
		printf("%s fail\r\n", strInit);
	} else {
		EthInit(); // Ethernet MAC
		printf("%s done\r\n", strInit);
	}
#endif

	tcpip_init(tcpip_init_done_func, NULL);

	vTaskDelete(NULL);					
	UNUSED(arg);
}

err_t IGMP_Join(char *strIP)
{
	ip_addr_t groupaddr;
    err_t err;

    groupaddr.addr = ipaddr_addr(strIP);
	printf("%s => 0x%08X\r\n", strIP, groupaddr.addr);
	err = igmp_joingroup(IP_ADDR_ANY, (ip_addr_t *)&groupaddr);
    if (err != ERR_OK) {
        printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
    }
	return err;	
}

err_t IGMP_Leave(char *strIP)
{
	ip_addr_t groupaddr;
    err_t err;

    groupaddr.addr = ipaddr_addr(strIP);
	printf("%s => 0x%08X\r\n", strIP, groupaddr.addr);
	err = igmp_leavegroup(IP_ADDR_ANY, (ip_addr_t *)&groupaddr);
    if (err != ERR_OK) {
        printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
    }
	return err;	
}
#endif
