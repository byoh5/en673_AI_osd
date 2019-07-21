//*************************************************************************************************
// IpChange task
//*************************************************************************************************
#include "dev.h"
#include "user.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "msg.h"

#if defined(__ETH__) || defined(__WIFI__)
#include "lwip/netifapi.h"
#include "lwip/dns.h"
#include "networkif.h"

#if defined(__ETH__)
#include "ethernetif.h"
#endif
#if defined(__WIFI__)
#include "wlif_tools.h"
#include "wifiif.h"
#include "wifi.h"
#endif

#include "ipchange.h"

static UINT unIPAddressChangeFlag[enlEnd] = {0};

#define ENX_IPC_DGB_COMMAND		DEF_OFF
#define ENX_IPC_DGB_PROCESSING	DEF_OFF
#define ENX_IPC_DGB_RESPONSE	DEF_OFF
#define ENX_IPC_DGB_DEBUG		DEF_OFF
#define ENX_IPC_DGB_ERROR		DEF_ON

#define tprintf(debug, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);

//*************************************************************************************************
// IpChange
//-------------------------------------------------------------------------------------------------
//
extern struct netif *netif_list;
void ChangeDefDeviceId(void)
{
	// strDeviceId의 앞내용이 DEVICE_ID와 같다면 변경한 적이 없음.
	int i, k = 0, id_len = strlen(DEVICE_ID);
	for (i = 0; i < id_len; i++) {
		if (DEVICE_ID[i] != gtUser.strDeviceId[i]) {
			k = 1;
			break;
		}
	}
	// k가 0이면 변경한 적이 없는 것이므로 UPNP를 위해 DeviceId를 변경한다.
	if (k == 0) {
		printf("ChangDefDeviceId : %s => ", gtUser.strDeviceId);
		struct netif *netif = netif_list;
		if (netif != NULL) {
			sprintf((char *)gtUser.strDeviceId, "%s%02X%02X%02X", (char *)DEVICE_ID, netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
		} else {
			sprintf((char *)gtUser.strDeviceId, "%sFFFFFF", (char *)DEVICE_ID);
		}
		printf("%s\r\n", gtUser.strDeviceId);
	}
}

#if defined(__ETH__)
static void enx_ipchange_ethernet(struct netif *_netif, UINT changeflag)
{
	if (changeflag != 0) {
		if (gtUser.bUseDhcp == DHCP_USE_ON) {
			printf("DHCP on state...\r\n");
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_start(_netif);
			} else {
				tprintf(ENX_IPC_DGB_ERROR, "Error! DHCP is in use.\r\n");
			}
		} else {
			network_check_netif();

			vTaskDelay(IpChangeDelay);	// 웹브라우저 변경 중....

			printf("DHCP off state...\r\n");
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_release(_netif);
				netifapi_dhcp_stop(_netif);
				changeflag = UIN_NW_INT_IP | UIN_NW_INT_SN | UIN_NW_INT_GW | UIN_NW_INT_DNS0 | UIN_NW_INT_DNS1;
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS0)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtUser.nDnsSvr0);
				dns_setserver(0, &dns);
//				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS0, ip4_addr_get_u32(&dns)) == DEF_FAIL)	vTaskDelay(1);
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS1)) {
				ip_addr_t dns;
				ip4_addr_set_u32(&dns, gtUser.nDnsSvr1);
				dns_setserver(1, &dns);
//				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS1, ip4_addr_get_u32(&dns)) == DEF_FAIL)	vTaskDelay(1);
			}

			if (bitcheck(changeflag, UIN_NW_INT_IP) || bitcheck(changeflag, UIN_NW_INT_SN) || bitcheck(changeflag, UIN_NW_INT_GW)) {
				ip_addr_t ipaddr, netmask, gw;
				ip4_addr_set_u32(&ipaddr, gtUser.nIpAddr);
				ip4_addr_set_u32(&netmask, gtUser.nNetMask);
				ip4_addr_set_u32(&gw, gtUser.nGateway);
				netifapi_netif_set_addr(_netif, &ipaddr, &netmask, &gw);

				while(MsgLock() == DEF_FAIL)																		vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_SM, ip4_addr_get_u32(&netmask)) == DEF_FAIL)	vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_GW, ip4_addr_get_u32(&gw)) == DEF_FAIL) 		vTaskDelay(1);
				while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_IP, ip4_addr_get_u32(&ipaddr)) == DEF_FAIL) 	vTaskDelay(1);
				MsgUnlock();
			}

			netifapi_dhcp_inform(_netif);
		}
	}
}
#endif

#if defined(__WIFI__)
static void enx_ipchange_wifista(struct netif *_netif, UINT changeflag)
{
	if (changeflag != 0) {
		printf("DHCP on state...\r\n");
		if (gtWificfg.STAcfg.un8DHCPSwitch == WIFI_ON) {
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_start(_netif);
			} else {
				tprintf(ENX_IPC_DGB_ERROR, "Error! DHCP is in use.\r\n");
			}
		} else {
			network_check_netif();

			vTaskDelay(IpChangeDelay);	// 웹브라우저 변경 중....

			printf("DHCP off state... \r\n");
			if (bitcheck(changeflag, UIN_NW_INT_DHCP)) {
				netifapi_dhcp_release(_netif);
				netifapi_dhcp_stop(_netif);
				changeflag = UIN_NW_INT_IP | UIN_NW_INT_SN | UIN_NW_INT_GW | UIN_NW_INT_DNS0 | UIN_NW_INT_DNS1;
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS0)) {
				dns_setserver(0, &gtWificfg.STAcfg.iptDNS0);
			}

			if (bitcheck(changeflag, UIN_NW_INT_DNS1)) {
				dns_setserver(1, &gtWificfg.STAcfg.iptDNS0);
			}

			if (bitcheck(changeflag, UIN_NW_INT_IP) || bitcheck(changeflag, UIN_NW_INT_SN) || bitcheck(changeflag, UIN_NW_INT_GW)) {
				netifapi_netif_set_addr(_netif, &gtWificfg.STAcfg.iptLocalip, &gtWificfg.STAcfg.iptNetmask, &gtWificfg.STAcfg.iptGateway);

				char cmd[80];
				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, _netif->netmask.addr);
				cpu2cop_wlif_execmd(cmd);
				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, _netif->gw.addr);
				cpu2cop_wlif_execmd(cmd);
				snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, _netif->ip_addr.addr);
				cpu2cop_wlif_execmd(cmd);
			}

			netifapi_dhcp_inform(_netif);
		}
	}
}
#endif

static void enx_ipchange_task(void *ctx)
{
	NETIFLIST iftype = (NETIFLIST)ctx;
	if (unIPAddressChangeFlag[iftype] != 0) {
		switch (iftype) {
#if defined(__ETH__)
			case enlETHERNET: // OK
				enx_ipchange_ethernet(netif_state[iftype]._netif, unIPAddressChangeFlag[iftype]);
				break;
#endif
#if defined(__WIFI__)
			case enlWIFISTA: // OK
				enx_ipchange_wifista(netif_state[iftype]._netif, unIPAddressChangeFlag[iftype]);
				break;
			case enlWIFIUAP: // ...Test
#endif
			default:
				printf("Wrong type(%d) entered.(key:0x%08X)\r\n", iftype, unIPAddressChangeFlag[iftype]);
				break;
		}

		unIPAddressChangeFlag[iftype] = 0;
	}
	vTaskDelete(NULL);
}

void enx_ipchange_proc(NETIFLIST iftype, UINT _key)
{
	unIPAddressChangeFlag[iftype] = _key;
	switch (iftype) {
#if defined(__ETH__)
		case enlETHERNET:
#endif
#if defined(__WIFI__)
		case enlWIFISTA:
		case enlWIFIUAP:
#endif
			sys_thread_new("ipc", enx_ipchange_task, (void *)iftype, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);
			break;
		default:
			printf("Wrong type(%d) entered.(key:0x%08X)\r\n", iftype, _key);
			break;
	}
}
#endif
