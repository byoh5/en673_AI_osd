//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"

#if defined(__ETH__)

#include "user.h"
#include "msg.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "netif/etharp.h"
#include "lwip/netifapi.h"
#include "lwip/igmp.h"
#include "lwip/dns.h"

#include "networkif.h"
#include "ethernetif.h"
#include "ipchange.h"

#define EIFNAME0 'e'
#define EIFNAME1 'n'

ISRD BYTE etherbuf_Tx[1600];
static err_t low_level_ethif_output(struct netif *netif, struct pbuf *p)
{
	if (wltx_lock()) {
		if (p->next == NULL) {
			UINT nTotLen = p->tot_len < 64 ? 64 : p->tot_len;
			DmaEthTxCpy((BYTE *)p->payload, nTotLen);
		} else {
			struct pbuf *q = NULL;
			UINT nTotLen = 0;

			if (p->tot_len > 1600) {
				printf("Error Ether tx size over 1600bytes!\r\n");
				return ERR_MEM;
			}

			for (q = p; q != NULL; q = q->next) {
				DmaMemCpy_ip((BYTE *)etherbuf_Tx+nTotLen, (BYTE *)q->payload, q->len);
				nTotLen += q->len;	
			}

			if (nTotLen != p->tot_len) {
				printf("%s: nTotLen(%d) != p->tot_len(%d)\r\n", __func__, nTotLen, p->tot_len);
			}

			if (nTotLen < 64) {
				nTotLen = 64;
			}

			DmaEthTxCpy_async((BYTE *)etherbuf_Tx, nTotLen);
		}
		wltx_unlock();
		return ERR_OK;
	}
    return ERR_IF;
	UNUSED(netif);
}

void IsrEthRx(void *dummy)
{
	if(netif_state[enlETHERNET].xrx_sem) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(netif_state[enlETHERNET].xrx_sem, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1; // Task switch required ?
		}
	} else {
		printf("%s interface not ready.\r\n", netif_state[enlETHERNET].ifname);
	}

	UNUSED(dummy);
}

static void network_ethif_link_cb(struct netif *_netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_link_up(_netif)) {
		if (gtUser.bUseDhcp == DHCP_USE_ON) {
			dhcp_start(_netif);
		} else {
			ip_addr_t ipaddr, netmask, gw, dns0, dns1;
			ip4_addr_set_u32(&ipaddr, gtUser.nIpAddr);
			ip4_addr_set_u32(&netmask, gtUser.nNetMask);
			ip4_addr_set_u32(&gw, gtUser.nGateway);
			ip4_addr_set_u32(&dns0, gtUser.nDnsSvr0);
			ip4_addr_set_u32(&dns1, gtUser.nDnsSvr1);

			netif_set_addr(_netif, &ipaddr, &netmask, &gw);
			dns_setserver(0, &dns0);							// Set DNS server 0 addr
			dns_setserver(1, &dns1);							//		"		  1  "

			while(MsgLock() == DEF_FAIL)																vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_IP, gtUser.nIpAddr) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_SM, gtUser.nNetMask) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_GW, gtUser.nGateway) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS0, gtUser.nDnsSvr0) == DEF_FAIL)	vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS1, gtUser.nDnsSvr1) == DEF_FAIL)	vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_LINK, DEF_ON) == DEF_FAIL) vTaskDelay(1);
			MsgUnlock();
		}
		printf("%s link up, bound to %IP%s\r\n", _state->ifname, _netif->ip_addr.addr, (_netif->flags & NETIF_FLAG_DHCP) ? (_netif->dhcp->state ? ", DHCP negotiating..." : "") : "");
	} else {
		while(MsgLock() == DEF_FAIL)																vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_LINK, DEF_OFF) == DEF_FAIL)				vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_IP, 0) == DEF_FAIL)		vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_SM, 0) == DEF_FAIL)		vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_GW, 0) == DEF_FAIL)		vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS0, 0) == DEF_FAIL)	vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS1, 0) == DEF_FAIL)	vTaskDelay(1);
		MsgUnlock();
		if (gtUser.bUseDhcp == DHCP_USE_ON) {
			dhcp_release(_netif);
			dhcp_stop(_netif);
		} else {
			netif_set_addr(_netif, NULL, NULL, NULL);
		}
		dns_setserver(0, NULL);
		dns_setserver(1, NULL);
		printf("%s link down\r\n", _state->ifname);
	}

	network_default_netif();
}

static void network_ethif_status_cb(struct netif *_netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_up(_netif)) {
		printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		if (netif_is_link_up(_netif)) {
			printf("(%s, %IP)\r\n", (_netif->flags & NETIF_FLAG_DHCP) ? "DHCP" : "static", _netif->ip_addr.addr);
			if (gtUser.bUseDhcp == DHCP_USE_ON) {
				gtUser.nIpAddr = ip4_addr_get_u32(&_netif->ip_addr);
				gtUser.nNetMask = ip4_addr_get_u32(&_netif->netmask);
				gtUser.nGateway = ip4_addr_get_u32(&_netif->gw);
				ip_addr_t dns0 = dns_getserver(0);
				ip_addr_t dns1 = dns_getserver(1);
				gtUser.nDnsSvr0 = ip4_addr_get_u32(&dns0);
				gtUser.nDnsSvr1 = ip4_addr_get_u32(&dns1);

				while(MsgLock() == DEF_FAIL)																vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_IP, gtUser.nIpAddr) == DEF_FAIL)		vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_SM, gtUser.nNetMask) == DEF_FAIL)		vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_GW, gtUser.nGateway) == DEF_FAIL)		vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS0, gtUser.nDnsSvr0) == DEF_FAIL)	vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS1, gtUser.nDnsSvr1) == DEF_FAIL)	vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_LINK, DEF_ON) == DEF_FAIL)				vTaskDelay(1);
				MsgUnlock();
			} else {
				dhcp_inform(_netif);
			}
		} else {
			printf("\r\n");
		}
	} else {
		printf("%s status down(link %s)\r\n", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
	}
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
static err_t network_ethif_init(struct netif *netif)
{
	int i;
	for (i = 0; i < IGMP_LIST_LENS; i++) {
		igmp_list[i].a = 0;
	}

	LWIP_ASSERT("netif != NULL", (netif != NULL));

	netif->output = etharp_output;
	netif->linkoutput = low_level_ethif_output;
#if LWIP_IGMP    
	netif->igmp_mac_filter = igmp_mac_filter_set;
#endif

	netif_set_status_callback(netif, network_ethif_status_cb);
	netif_set_link_callback(netif, network_ethif_link_cb);

	/* Set MAC hardware address */
	netif->hwaddr[0] = gtSystem.nMacAddress[0];
	netif->hwaddr[1] = gtSystem.nMacAddress[1];
	netif->hwaddr[2] = gtSystem.nMacAddress[2];
	netif->hwaddr[3] = gtSystem.nMacAddress[3];
	netif->hwaddr[4] = gtSystem.nMacAddress[4];
	netif->hwaddr[5] = gtSystem.nMacAddress[5];

	printf("MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n", netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

	return low_level_init(netif, EIFNAME0, EIFNAME1);
}

void network_ethif_start(void)
{
	memset(&gtMyNetIf, 0, sizeof(gtMyNetIf));
	netif_add(&gtMyNetIf, NULL, NULL, NULL, &netif_state[enlETHERNET], network_ethif_init, tcpip_input);

	netif_state[enlETHERNET].xrx_sem = xSemaphoreCreateCounting(MSG_NETRX_NUM, 0);

	ChangeDefDeviceId();

	sys_thread_new("eif_Rx", if_input, &gtMyNetIf, ETHRX_STACK_SIZE, ETHRX_TASK_PRIO);
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Mac Filter
err_t igmp_mac_filter_set(struct netif *netif, ip_addr_t *group, u8_t action)
{
	//Make MAC Addr from IP Multicast Addr
	BYTE macAddr[6] = {0x01, 0x00, 0x5E};
	macAddr[3] = (BYTE)(0x7f & (group->addr>>16));
	macAddr[4] = (BYTE)(0xff & (group->addr>>8));
	macAddr[5] = (BYTE)(0xff & (group->addr));

	int i, check = 0;
	if (action == IGMP_ADD_MAC_FILTER) {
		EthMulFiltEnable();
#if (ETH_MAC_PAUSE)
		Eth8021DFiltInsert(eIe8021d_IeeeMacSpecificControlProtocolsGroup0);
#endif

		for (i = 0; i < IGMP_LIST_LENS; i++) {
			if (igmp_list[i].use == 0) {
				igmp_list[i].use = 1;
				igmp_list[i].igmpadr = group->addr & 0x00ffffff;
				check = 1;
				break;
			}
		}

		if(check == 1) {
			printf("IGMP : Add %IP to the mac filter.\r\n", group->addr);
			EthMulFiltInsert(igmp_list[i].igmpadr);
		} else {
			printf("IGMP : Fail - igmp_list is full. Unprocessed IP(%IP)\r\n", group->addr);
		}
	} else {
		for (i = 0; i < IGMP_LIST_LENS; i++) {
			if (igmp_list[i].use == 1) {
				if (igmp_list[i].igmpadr == (group->addr & 0x00ffffff)) {
					igmp_list[i].igmpadr = 0;
					igmp_list[i].use = 0;
					check = 1;
					break;
				}
			}
		}

		if (check == 1) {
			printf("IGMP : Delete %IP to the mac filter.\r\n", group->addr);
			EthMulFiltClear();
			for (i = 0; i < IGMP_LIST_LENS; i++) {
				if (igmp_list[i].use == 1) {
					EthMulFiltInsert(igmp_list[i].igmpadr);
					check = 2;
				}
			}
#if (ETH_MAC_PAUSE)
			Eth8021DFiltInsert(eIe8021d_IeeeMacSpecificControlProtocolsGroup0);
#endif
			if (check == 1) {
				printf("IGMP : Do not use multicast filter.\r\n");
				EthMulFiltDisable();
			}
		} else {
			printf("IGMP : Fail - Address is not present. Unprocessed IP(%IP)\r\n", group->addr);
		}
	}

	return ERR_OK;

	UNUSED(netif);
}

void igmp_mac_filter_list(void)
{
	int i;
	printf("IGMP mac filter list\r\n");
	for (i = 0; i < IGMP_LIST_LENS; i++) {
		if (igmp_list[i].use == 1) {
			printf("%02d : 0x%08X\r\n", i, igmp_list[i].igmpadr);
		}
	}
#if (ETH_MAC_PAUSE)
	printf("IEEE 802.1D\r\n");
	printf("- Mac Specific Control Protocols Group\r\n");
#endif
}
#endif
