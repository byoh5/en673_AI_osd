//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"

#if defined(__WIFI__)

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
#include "wifiif.h"
#include "wifi.h"
#include "wlif_tools.h"

#include "dhcpd.h"

#define WIFNAME0 'w'
#define WIFNAME1 'l'

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and
 *        type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
static err_t low_level_wlif_output(struct netif *netif, struct pbuf *p)
{  
	BYTE *ptx;
	struct pbuf *q;
	UINT nTotLen = 0;
	netif_state_t *priv = (netif_state_t *)netif->state;
	if(wltx_lock())
	{
		while(CS_WIFI_TX)
			vTaskDelay(1);

//		portENTER_CRITICAL();

		if(gptMsgEthCpu1to0.num < MSG_ETH_NUM)
		{
			gptMsgEthCpu1to0.type[gptMsgEthCpu1to0.head] = WLIF_DATA;
			gptMsgEthCpu1to0.netif[gptMsgEthCpu1to0.head] = priv->iftype;
			gptMsgEthCpu1to0.length[gptMsgEthCpu1to0.head] = p->tot_len;
			ptx = (BYTE *)gptMsgEthCpu1to0.pkt[gptMsgEthCpu1to0.head];

			if(p->tot_len > 1520)
				printf("%s buffer overflow(%d)\r\n", __func__, p->tot_len);

//			if(p->tot_len != p->len)
//				printf("%s tot_len(%d) len(%d)\r\n", __func__, p->tot_len, p->len);

			for(q=p; q!=NULL; q=q->next)
			{	// Chained "pbuf"
				if(q->len)
				{
					if(q->len > 1520)
						printf("%s buffer overflow(%d)\r\n", __func__, q->len);

					DmaMemCpy_ip(ptx+nTotLen, q->payload, (UINT)q->len);
					//DmaMemCpy_isr(ptx+nTotLen, q->payload, (UINT)q->len);
				}
				nTotLen += q->len;
			}

			num_loop(gptMsgEthCpu1to0.head, MSG_ETH_NUM);
			gptMsgEthCpu1to0.num++;
		}
		else
		{
			printf("%s(%d) : over num(%d)(%d/%d)\r\n", __func__, __LINE__, gptMsgEthCpu1to0.num,
				gptMsgEthCpu1to0.head, gptMsgEthCpu1to0.tail);
		}

//		portEXIT_CRITICAL();

		CS_WIFI_TX = 0;

		wltx_unlock();
		return ERR_OK;
	}
	return ERR_IF;
}

// sys0 irq from cop when cop receive packet from eth or wifi Here
void IsrWifiEvt(void *dummy)
{
	if (gptMsgWlifEvt.num) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		gptMsgWlifEvt.num--;
		gptMsgWlifEvt.cnt++;
		xSemaphoreGiveFromISR(xwlif_evt_sem, &xHigherPriorityTaskWoken);	
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;		// Task switch required ?
		}
	}

	UNUSED(dummy);
}

void IsrWifiRx(void *dummy)
{
	NETIFLIST iftype = gptMsgNetworkRX1.info[gptMsgNetworkRX1.head].iftype;
	if(netif_state[iftype].xrx_sem) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(netif_state[iftype].xrx_sem, &xHigherPriorityTaskWoken);
		if (xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;		// Task switch required ?
		}
	} else {
		printf("%s interface not ready.\r\n", netif_state[iftype].ifname);
	}
	UNUSED(dummy);
}

void network_wlifsta_link_cb(struct netif *_netif)
{
	char cmd[80];
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_link_up(_netif)) {
		if (gtWificfg.STAcfg.un8DHCPSwitch == WIFI_ON) {
			dhcp_start(_netif);
		} else {
			netif_set_addr(_netif, &gtWificfg.STAcfg.iptLocalip, &gtWificfg.STAcfg.iptNetmask, &gtWificfg.STAcfg.iptGateway);
			dns_setserver(0, &gtWificfg.STAcfg.iptDNS0); // Set DNS server 0 addr
			dns_setserver(1, &gtWificfg.STAcfg.iptDNS1); //		"		  1  "

			snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, _netif->ip_addr.addr);
			cpu2cop_wlif_execmd(cmd);
			snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, _netif->netmask.addr);
			cpu2cop_wlif_execmd(cmd);
			snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, _netif->gw.addr);
			cpu2cop_wlif_execmd(cmd);
			snprintf(cmd, 80, "staset %s", WIFI_STA_SET_CMD_NETUP);
			cpu2cop_wlif_execmd(cmd);
		}
		printf("%s link up, bound to %IP%s\r\n", _state->ifname, _netif->ip_addr.addr, (_netif->flags & NETIF_FLAG_DHCP) ? (_netif->dhcp->state ? ", DHCP negotiating..." : "") : "");
	} else {
		snprintf(cmd, 80, "staset %s", WIFI_STA_SET_CMD_NETDOWN);
		cpu2cop_wlif_execmd(cmd);
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, 0);
		cpu2cop_wlif_execmd(cmd);
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, 0);
		cpu2cop_wlif_execmd(cmd);
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, 0);
		cpu2cop_wlif_execmd(cmd);
		if (gtwlif_sta.flags & NETIF_FLAG_DHCP) {
			dhcp_stop(_netif);
			dhcp_release(_netif);
		} else {
			netif_set_addr(_netif, NULL, NULL, NULL);
		}
		dns_setserver(0, NULL);
		dns_setserver(1, NULL);
		printf("%s link down\r\n", _state->ifname);
	}

	network_default_netif();
}

void network_wlifsta_status_cb(struct netif *_netif)
{
	char cmd[80];
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_up(_netif)) {
		printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		if (netif_is_link_up(_netif)) {
			printf("(%s, %IP)\r\n", (_netif->flags & NETIF_FLAG_DHCP) ? "DHCP" : "static", _netif->ip_addr.addr);
			if (gtWificfg.STAcfg.un8DHCPSwitch == WIFI_ON) {
				gtWificfg.STAcfg.iptLocalip = _netif->ip_addr;
				gtWificfg.STAcfg.iptNetmask = _netif->netmask;
				gtWificfg.STAcfg.iptGateway = _netif->gw;
				gtWificfg.STAcfg.iptDNS0 = dns_getserver(0);
				gtWificfg.STAcfg.iptDNS1 = dns_getserver(1);

				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, _netif->netmask.addr);
				cpu2cop_wlif_execmd(cmd);
				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, _netif->gw.addr);
				cpu2cop_wlif_execmd(cmd);
				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, _netif->ip_addr.addr);
				cpu2cop_wlif_execmd(cmd);
				snprintf(cmd, 80, "staset %s", WIFI_STA_SET_CMD_NETUP);
				cpu2cop_wlif_execmd(cmd);
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

void network_wlifuap_link_cb(struct netif* _netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_link_up(_netif)) {
		printf("%s link up, bound to %IP\r\n", _state->ifname, _netif->ip_addr.addr);
	} else {
		printf("%s link down\r\n", _state->ifname);
	}

	network_default_netif();
}

void network_wlifuap_status_cb(struct netif *_netif)
{
	netif_state_t *_state = (netif_state_t *)_netif->state;
	if (netif_is_up(_netif)) {
		printf("%s status up(link %s)", _state->ifname, netif_is_link_up(_netif) ? "up" : "down");
		if (netif_is_link_up(_netif)) {
			printf("(%IP)\r\n", _netif->ip_addr.addr);
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
err_t network_wlif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

	netif->output = etharp_output;
	netif->linkoutput = low_level_wlif_output;
#if LWIP_IGMP
	netif->igmp_mac_filter = igmp_mac_filter_set_mlanif;
#endif

	if (netif == &gtwlif_sta) {
		netif_set_link_callback(&gtwlif_sta, network_wlifsta_link_cb);
		netif_set_status_callback(&gtwlif_sta, network_wlifsta_status_cb);
	} else if (netif == &gtwlif_uap) {
		netif_set_link_callback(&gtwlif_uap, network_wlifuap_link_cb);
		netif_set_status_callback(&gtwlif_uap, network_wlifuap_status_cb);
	}

	return low_level_init(netif, WIFNAME0, WIFNAME1);
}

void network_wlif_sta_start(void)
{
	static int funcfirst = 0;
	if (funcfirst == 0) {
		funcfirst++;

		memset(&gtwlif_sta, 0, sizeof(gtwlif_sta));
		netif_add(&gtwlif_sta, NULL, NULL, NULL, &netif_state[enlWIFISTA], network_wlif_init, tcpip_input);	// Net add

		netif_state[enlWIFISTA].xrx_sem = xSemaphoreCreateCounting(MSG_NETRX_NUM, 0);

		sys_thread_new("wif_sRx", if_input, &gtwlif_sta, _64KB_STACK_SIZE, WIFI_STA_TASK_PRIO);
	}
}

void network_wlif_uap_start(void)
{
	static int funcfirst = 0;
	if (funcfirst == 0) {
		funcfirst++;

		memset(&gtwlif_uap, 0, sizeof(gtwlif_uap));
		netif_add(&gtwlif_uap, NULL, NULL, NULL, &netif_state[enlWIFIUAP], network_wlif_init, tcpip_input); // Net add

		netif_state[enlWIFIUAP].xrx_sem = xSemaphoreCreateCounting(MSG_NETRX_NUM, 0);

		sys_thread_new("wif_aRx", if_input, &gtwlif_uap, _64KB_STACK_SIZE, WIFI_AP_TASK_PRIO);
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Wi-Fi Mac Filter
err_t igmp_mac_filter_set_mlanif(struct netif *netif, ip_addr_t *group, u8_t action)
{
	char cmd[512] = {0};
	if (group->addr == 0xE0000001) { // default 
		return ERR_OK;
	}

	if (action == IGMP_ADD_MAC_FILTER) {
		if (netif == &gtwlif_uap) {
			sprintf(cmd, "apset %s %IP", WIFI_SET_CMD_IGMPJOIN, group->addr);
		} else {
			sprintf(cmd, "staset %s %IP", WIFI_SET_CMD_IGMPJOIN, group->addr);
		}
	} else {
		if (netif == &gtwlif_uap) {
			sprintf(cmd, "apset %s %IP", WIFI_SET_CMD_IGMPLEAVE, group->addr);
		} else {
			sprintf(cmd, "staset %s %IP", WIFI_SET_CMD_IGMPLEAVE, group->addr);
		}
	}
	printf("%s(%d) %s\r\n", __func__, __LINE__, cmd);
	cpu2cop_wlif_execmd(cmd);
	return ERR_OK;
	UNUSED(netif);
}
#endif
