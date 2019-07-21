#ifdef __WIFI__

#include "dev.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/igmp.h"
#include "lwip/tcpip.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"

#include "FreeRTOS.h"
#include "msg.h"
#include "semphr.h"
#include "user.h"

#include "networkif.h"
#include "wifiif.h"

#include "wlif_tools.h"
#include "wifi_cfg.h"
#include "wifi.h"
#include "dhcpc.h"
#include "dhcpd.h"

#include "init.h"

#ifndef __ETH__
#include "ipchange.h"
#endif

void WiFiAP_set(void)
{
	char cmd[80];

	snprintf(cmd, 80, "apset %s %IP", WIFI_UAP_SET_CMD_NETMASK, gtWificfg.APcfg.iptNetmask.addr);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %IP", WIFI_UAP_SET_CMD_IP, gtWificfg.APcfg.iptLocalip.addr);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %s", WIFI_UAP_SET_CMD_SSID, gtWificfg.APcfg.strSSID);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %s", WIFI_UAP_SET_CMD_KEY, gtWificfg.APcfg.strKey);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_CHANNEL, gtWificfg.APcfg.un8Channel);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_MAX_SCB, gtWificfg.APcfg.un8MaxSTA);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_SEC, gtWificfg.APcfg.un32Authentication);
	cpu2cop_wlif_execmd(cmd);

	snprintf(cmd, 80, "apset %s", WIFI_UAP_SET_CMD_SET);
	cpu2cop_wlif_execmd(cmd);
}

void WiFiAP_start(void)
{
	cpu2cop_wlif_execmd("apstart");
}

void WiFiSTA_connect(void)
{
	char commandline[128] = {0};
	switch (gtWificfg.STAcfg.un32Authentication) {
		case wifi_OPEN:
			sprintf(commandline, "connect open %s", gtWificfg.STAcfg.strSSID);
			break;
		case wifi_WPA_PSK:		// wpa2 only
		case wifi_WPAnWPA2_PSK: // wpa2 only
			gtWificfg.STAcfg.un32Authentication = wifi_WPA2_PSK;
		case wifi_WPA2_PSK:
			sprintf(commandline, "connect wpa %s %s", gtWificfg.STAcfg.strSSID, gtWificfg.STAcfg.strKey);
			break;
		default:
			printf("%s connection authentication error(%d).\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, gtWificfg.STAcfg.un32Authentication);
			return;
	}
	printf("%s connection command is %s.\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, commandline);
	cpu2cop_wlif_execmd(commandline);
}

#if 0
void WiFiSTA_set(void)
{
	char cmd[80];

	if(gtWificfg.STAcfg.un8DHCPSwitch == WIFI_OFF)
	{
		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_NETMASK, gtWificfg.STAcfg.iptNetmask.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_GATEWAY, gtWificfg.STAcfg.iptGateway.addr);
		cpu2cop_wlif_execmd(cmd);

		snprintf(cmd, 80, "staset %s %IP", WIFI_STA_SET_CMD_IP, gtWificfg.STAcfg.iptLocalip.addr);
		cpu2cop_wlif_execmd(cmd);
	}
}
#endif

void WiFiSTA_start(void)
{
	cpu2cop_wlif_execmd("stastart");
}

void get_cpu0status_cb(void *ctx)
{
	cpu2cop_wlif_execmd("wifistatus");
	UNUSED(ctx);
}

//gtWificfg.STAcfg.un32State
const char *WLIF_STATE_MSG[17] = {
	"WLIF UAP START",			// 0
	"WLIF_STA_START",			// 1
	"WLIF_UAP_ACTIVE",			// 2
    "WLIF_STA_ACTIVE",			// 3 
    "WLIF_STA_CONNECT_WAIT",	// 4
	"WLIF_CONNECTED",			// 5
	"WLIF_DIS_CONNECTED",		// 6
	"WLIF_SCAN",				// 7
	"WLIF_UAP_BSS_ON",			// 8
	"WLIF_UAP_BSS_OFF",			// 9
	"WLIF_UAP_BSS_ERROR",		// 10
	"WLIF_UAP_DISCONN",			// 11
	"WLIF_INIT_FAIL",			// 12
	"WLIF_STA_NO_AP",			// 13
	"WLIF_STA_WPA_AUTH_FAIL",	// 14
	"WLIF_NONE",				// 15
	"WLIF_ERRMSG"				// 16
};

const char *getWlifStateMsg(UINT state)
{
	switch(state)
	{
		case WLIF_UAP_START:			return WLIF_STATE_MSG[0];
		case WLIF_STA_START:			return WLIF_STATE_MSG[1];
		case WLIF_UAP_ACTIVE:			return WLIF_STATE_MSG[2];
		case WLIF_STA_ACTIVE:			return WLIF_STATE_MSG[3];
		case WLIF_STA_CONNECT_WAIT:		return WLIF_STATE_MSG[4];
		case WLIF_CONNECTED:			return WLIF_STATE_MSG[5];
		case WLIF_DIS_CONNECTED:		return WLIF_STATE_MSG[6];
		case WLIF_SCAN:					return WLIF_STATE_MSG[7];
		case WLIF_UAP_BSS_ON:			return WLIF_STATE_MSG[8];
		case WLIF_UAP_BSS_OFF:			return WLIF_STATE_MSG[9];
		case WLIF_UAP_BSS_ERROR:		return WLIF_STATE_MSG[10];
		case WLIF_UAP_DISCONN:			return WLIF_STATE_MSG[11];
		case WLIF_INIT_FAIL:			return WLIF_STATE_MSG[12];
		case WLIF_STA_NO_AP:			return WLIF_STATE_MSG[13];
		case WLIF_STA_WPA_AUTH_FAIL:	return WLIF_STATE_MSG[14];
		case WLIF_NONE:					return WLIF_STATE_MSG[15];
		default:						return WLIF_STATE_MSG[16];
	}
}

void wifiTask(void *pvParameters)
{
	wlif_priv_t wlif_priv;

	WifiCFG_Load();
	gtWificfg.APcfg.un32State = WLIF_NONE;
	gtWificfg.STAcfg.un32State = WLIF_NONE;

	vTaskDelay(200);
	printf("Wi-Fi control starts.\r\n");

	while (1) {
		xSemaphoreTake(xwlif_evt_sem, portMAX_DELAY);
		wlif_get_event(&wlif_priv);
		printf("Wi-Fi event #%d.\r\n", wlif_priv.event);
		switch (wlif_priv.event) {
			case WLIF_UAP_START:
#if WIFI_AP_SSID_MAC
#define DEF_SSID_LEN (sizeof(WIFI_AP_SSID)-1)	// define string length => sizeof, (-1) => delete NULL value
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+0] = hex2ascii((wlif_priv.mac_addr[3]>>4)&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+1] = hex2ascii((wlif_priv.mac_addr[3]	 )&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+2] = hex2ascii((wlif_priv.mac_addr[4]>>4)&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+3] = hex2ascii((wlif_priv.mac_addr[4]	 )&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+4] = hex2ascii((wlif_priv.mac_addr[5]>>4)&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+5] = hex2ascii((wlif_priv.mac_addr[5]	 )&0x0f);
				gtWificfg.APcfg.strSSID[DEF_SSID_LEN+6] = '\0';
#endif

				WiFiAP_set();
				network_wlif_uap_start();
				netifapi_netif_set_addr(&gtwlif_uap, &gtWificfg.APcfg.iptLocalip, &gtWificfg.APcfg.iptNetmask, &gtWificfg.APcfg.iptLocalip);
				memcpy(gtwlif_uap.hwaddr, wlif_priv.mac_addr, ETHARP_HWADDR_LEN);
#ifndef __ETH__
				ChangeDefDeviceId();
#endif

wlif_uap_go:
				printf("%s event #%d, %d-%d, %d-%d-%d\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, gtWificfg.APcfg.un32State, gtWificfg.APcfg.un8Switch, gtWificfg.STAcfg.un8Switch, gtWificfg.STAcfg.un8Connect, gtWificfg.STAcfg.un32State);
				if (gtWificfg.STAcfg.un8Switch == WIFI_OFF || gtWificfg.STAcfg.un8Connect == WIFI_OFF || gtWificfg.STAcfg.un32State == WLIF_CONNECTED) {
					if (gtWificfg.APcfg.un32State == WLIF_NONE) {
						if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
							WiFiAP_start();
							gtWificfg.APcfg.un32State = WLIF_UAP_START;
						} else {
							printf("%s event #%d, switch state off(%s)\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
						}
					} else {
						printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
					}
				}
				break;

			case WLIF_UAP_ACTIVE:
				printf("%s event #%d, %d-%d\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, gtWificfg.APcfg.un32State, gtWificfg.APcfg.un8Switch);
				if (gtWificfg.APcfg.un32State == WLIF_UAP_START) {
					if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
						netif_set_up(&gtwlif_uap);
						netif_set_link_up(&gtwlif_uap);
						if (gtWificfg.APcfg.un8DHCPServer == WIFI_ON) {
							dhcpd_start(&gtwlif_uap);
						}
						printf("%s starts, SSID is %s.\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, gtWificfg.APcfg.strSSID);
						gtWificfg.APcfg.un32State = WLIF_UAP_ACTIVE;
					} else {
						printf("%s event #%d, switch state off(%s)\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
					}
				} else {
					printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
				}
				break;

			case WLIF_UAP_BSS_ON:
				printf("%s event #%d, state - %s\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
				if (!netif_is_up(&gtwlif_uap)) {	
					netif_set_up(&gtwlif_uap);
				}
				break;

			case WLIF_UAP_BSS_OFF:
				printf("%s event #%d, state - %s\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
				if (netif_is_up(&gtwlif_uap)) {	
					netif_set_down(&gtwlif_uap);
				}
				break;

			case WLIF_UAP_BSS_ERROR:
				printf("%s event #%d, state - %s\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
				if (netif_is_up(&gtwlif_uap)) {
					netif_set_down(&gtwlif_uap);
				}
				printf("%s event #%d, Reboot?\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event);
				break;

			case WLIF_UAP_DISCONN:
				printf("%s event #%d, state - %s\r\n", ((netif_state_t *)gtwlif_uap.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.APcfg.un32State));
				break;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

			case WLIF_STA_START:
//				WiFiSTA_set();
				network_wlif_sta_start();
				memcpy(gtwlif_sta.hwaddr, wlif_priv.mac_addr, ETHARP_HWADDR_LEN);

				printf("%s event #%d, %d-%d\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, gtWificfg.STAcfg.un32State, gtWificfg.STAcfg.un8Switch);
				if (gtWificfg.STAcfg.un32State == WLIF_NONE) {
					if (gtWificfg.STAcfg.un8Switch == WIFI_ON) {
						WiFiSTA_start();
						gtWificfg.STAcfg.un32State = WLIF_STA_START;
					} else {
						printf("%s event #%d, switch state off(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
					}
				} else {
					printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
				}
				break;

			case WLIF_STA_ACTIVE:
				printf("%s event #%d, %d-%d-%d\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, gtWificfg.STAcfg.un32State, gtWificfg.STAcfg.un8Switch, gtWificfg.STAcfg.un8Connect);
				if (gtWificfg.STAcfg.un32State == WLIF_STA_START) {
					if (gtWificfg.STAcfg.un8Switch == WIFI_ON) {
						if (gtWificfg.STAcfg.un8Connect == WIFI_ON) {
							WiFiSTA_connect();
							gtWificfg.STAcfg.un32State = WLIF_STA_ACTIVE;
						}
					} else {
						printf("%s event #%d, switch state off(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
					}
				} else {
					printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
				}
				break;

			case WLIF_CONNECTED:
				printf("%s event #%d, %d-%d-%d\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, gtWificfg.STAcfg.un32State, gtWificfg.STAcfg.un8Switch, gtWificfg.STAcfg.un8Connect);
				// 일반사항 | 끊겼을때 | AP의 ch변경으로 끊겼다 바로 붙었을 때
				if (gtWificfg.STAcfg.un32State == WLIF_STA_CONNECT_WAIT || gtWificfg.STAcfg.un32State == WLIF_DIS_CONNECTED || gtWificfg.STAcfg.un32State == WLIF_STA_ACTIVE) { 
					if (gtWificfg.STAcfg.un8Switch == WIFI_ON) {
						if (gtWificfg.STAcfg.un8Connect == WIFI_ON) {
							netif_set_up(&gtwlif_sta);
							netif_set_link_up(&gtwlif_sta);
							gtWificfg.STAcfg.un32State = WLIF_CONNECTED;
						}						
					}
				} else {
					printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
				}

				if (gtWificfg.APcfg.un32State == WLIF_NONE) {
					if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
						goto wlif_uap_go;
					}
				}
				break;

			case WLIF_DIS_CONNECTED:
				printf("%s event #%d, %d-%d-%d\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, gtWificfg.STAcfg.un32State, gtWificfg.STAcfg.un8Switch, gtWificfg.STAcfg.un8Connect);
				if (gtWificfg.STAcfg.un32State == WLIF_STA_CONNECT_WAIT || gtWificfg.STAcfg.un32State == WLIF_CONNECTED) {
					gtWificfg.STAcfg.un8Connect = WIFI_OFF;
					netif_set_link_down(&gtwlif_sta);
					gtWificfg.STAcfg.un32State = WLIF_STA_ACTIVE;
				} else if (gtWificfg.STAcfg.un32State == WLIF_STA_ACTIVE) {
					// 인증 실패 case?
					printf("%s event #%d, Authentication failed?\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event);
				} else {
					printf("%s event #%d, case error(%s)\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event, getWlifStateMsg(gtWificfg.STAcfg.un32State));
				}
				break;

			case WLIF_SCAN:
				printf("%s event #%d, AP scan\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event);
				break;

			case WLIF_INIT_FAIL:
				printf("%s event #%d, WLIF_INIT_FAIL\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event);
				gtWificfg.STAcfg.un32State = WLIF_NONE;
				gtWificfg.STAcfg.un8Connect = WIFI_OFF;
				gtWificfg.STAcfg.un8Switch = WIFI_ON;
				gtWificfg.APcfg.un32State = WLIF_NONE;
				gtWificfg.APcfg.un8Switch = WIFI_ON;
				break;

			case WLIF_STA_NO_AP:
				printf("%s event #%d, WLIF_STA_NO_AP\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event);
				gtWificfg.STAcfg.un32State = WLIF_STA_START; // 초기상태로...
				gtWificfg.STAcfg.un8Connect = WIFI_OFF;

				if (gtWificfg.APcfg.un32State == WLIF_NONE) {
					if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
						goto wlif_uap_go;
					}
				}
				break;

			case WLIF_STA_WPA_AUTH_FAIL:
				printf("%s event #%d, WLIF_STA_WPA_AUTH_FAIL\r\n", ((netif_state_t *)gtwlif_sta.state)->ifname, wlif_priv.event);
				gtWificfg.STAcfg.un32State = WLIF_STA_START; // 초기상태로...
				gtWificfg.STAcfg.un8Connect = WIFI_OFF;

				if (gtWificfg.APcfg.un32State == WLIF_NONE) {
					if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
						goto wlif_uap_go;
					}
				}
				break;

			default:
				break;
		}
	}
	UNUSED(pvParameters);
}
#endif
