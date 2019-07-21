/* ------------------------ EXR1K includes -------------------------------- */
#include <stdlib.h>		// for atoi

/* ------------------------ EN673 includes -------------------------------- */
#include "dev.h"
#include "dev_inline.h"
#include "user.h"
#include "msg.h"
#include "init.h"

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/ip_addr.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "lwip/dns.h"
#include "lwip/dhcp.h"

/* ------------------------ FAT includes ---------------------------------- */
#include "ff.h"
#include "file.h"

/* ------------------------ Project includes ------------------------------ */
#include "httpd.h"
#include "cgifuncs.h"
#include "cgissi.h"

/* ------------------------ Util includes --------------------------------- */
#include "wifi.h"
#include "wlif_tools.h"
#include "wifi_task.h"
#include "ipchange.h"

/* ------------------------------------------------------------------------ */
#ifdef __WIFI__

#if LWIP_HTTPD_SSI
extern xSemaphoreHandle xwlif_evt_sem;

static void SSI_wifi_ap(char *pcInsert, int iInsertLen)
{
	const char *strformat = CRLF \
					"var ap_switch = %d;"CRLF \
					"var ap_ssid = '%s';"CRLF \
					"var ap_fix_ssid = %d;"CRLF \
					"var ap_channel = %d;"CRLF \
					"var ap_Maxstacount = %d;"CRLF \
					"var ap_authentication = %d;"CRLF \
					"var ap_networkkey = '%s';"CRLF \
					"var ap_ip = '%IP';"CRLF \
					"var ap_sn = '%IP';"CRLF \
					"var ap_dhcpsw = %d;"CRLF\
					"var ap_startaddress = '%IP';"CRLF\
					"var ap_endaddress = '%IP';"CRLF\
					"var ap_leasetime = '%d';"CRLF;

	gptMsgWlifCmd.resp = 0; // ask cpu1 to reply for cpu0
	cpu2cop_wlif_execmd("apget");
	while (gptMsgWlifCmd.resp != 1) vTaskDelay(1);
	wlif_ewlap_cfg *pwlif_getap = (wlif_ewlap_cfg *)gptMsgWlifCmd.cmd;
	if (!pwlif_getap->flags) {
		dprintf("get fail AP info\r\n");
	} else {
		strcpy(gtWificfg.APcfg.strSSID, pwlif_getap->ssid);
		strcpy(gtWificfg.APcfg.strKey, pwlif_getap->passphrase);
		gtWificfg.APcfg.un8Channel = pwlif_getap->channel;
		gtWificfg.APcfg.un8MaxSTA = pwlif_getap->max_sta_count;
		switch (pwlif_getap->enc_proto) {
			case 0:
				gtWificfg.APcfg.un32Authentication = wifi_OPEN;
				break;
			case 1:
				gtWificfg.APcfg.un32Authentication = wifi_WPA2_PSK;
				break;
			default:
				dprintf("AP enc_proto error? (%d)\r\n", pwlif_getap->enc_proto);
				break;
		}
	}

	snprintf(pcInsert, iInsertLen, strformat, 
			gtWificfg.APcfg.un8Switch,
			gtWificfg.APcfg.strSSID,
			WIFI_AP_SSID_MAC,
			gtWificfg.APcfg.un8Channel,
			gtWificfg.APcfg.un8MaxSTA,
			pwlif_getap->enc_proto, /* 0: open, 1: wpa2_psk */
			strlen(gtWificfg.APcfg.strKey) != 0 ? gtWificfg.APcfg.strKey: "",
			gtWificfg.APcfg.iptLocalip.addr,
			gtWificfg.APcfg.iptNetmask.addr,
			gtWificfg.APcfg.un8DHCPServer,
			gtWificfg.APcfg.iptStartAddress.addr,
			gtWificfg.APcfg.iptEndAddress.addr,
			gtWificfg.APcfg.leaseTime);
}

static void SSI_wifi_sta(char *pcInsert, int iInsertLen)
{
	const char *strformat = CRLF \
					"var sta_switch = %d;"CRLF \
					"var sta_etcssid = '%s';"CRLF \
					"var sta_ssidcon = %d;"CRLF \
					"var sta_au = %d;"CRLF \
					"var sta_pw = '%s';"CRLF \
					"var dhcp = %d;"CRLF \
					"var ip = '%IP';"CRLF \
					"var nm = '%IP';"CRLF \
					"var gw = '%IP';"CRLF \
					"var dn1 = '%IP';"CRLF \
					"var dn2 = '%IP';"CRLF \
					"var sta_state = %d;"CRLF;

	if (gtWificfg.STAcfg.un8Switch == WIFI_ON) {
		gptMsgWlifCmd.resp = 0; // ask cpu1 to reply for cpu0
		cpu2cop_wlif_execmd("wifistainfo");
		while (gptMsgWlifCmd.resp != 1) {
			vTaskDelay(1);
		}
		wlif_scan_t *pwlif_conn_sta = (wlif_scan_t *)gptMsgWlifCmd.cmd;
		if (!pwlif_conn_sta->num) {
			dprintf("not Conn AP\r\n");
		} else {
			wl_scans_t *pscan = &pwlif_conn_sta->wl_scans[0];
			strcpy(gtWificfg.STAcfg.strSSID, pscan->ssid);
			strcpy(gtWificfg.STAcfg.strKey, pscan->flags);
		}
	}

	snprintf(pcInsert, iInsertLen, strformat, 
			gtWificfg.STAcfg.un8Switch,
			gtWificfg.STAcfg.strSSID,
			gtWificfg.STAcfg.un8Connect,
			gtWificfg.STAcfg.un32Authentication,
			strlen(gtWificfg.STAcfg.strKey) != 0 ? gtWificfg.STAcfg.strKey : "",
			gtWificfg.STAcfg.un8DHCPSwitch,
			gtWificfg.STAcfg.iptLocalip.addr,
			gtWificfg.STAcfg.iptNetmask.addr,
			gtWificfg.STAcfg.iptGateway.addr,
			gtWificfg.STAcfg.iptDNS0.addr,
			gtWificfg.STAcfg.iptDNS1.addr,
			gtWificfg.STAcfg.un32State);
}

static void SSI_wifi_scan_cb(void *ctx)
{
	int *one = (int *)ctx;
	*one = 0;
	// STA mode에서 주변 AP검색 시, EN673 AP를 OFF
	if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
		if (gtWificfg.STAcfg.un8Connect == WIFI_OFF) {
			cpu2cop_wlif_execmd("apset "WIFI_UAP_SET_CMD_BSSOFF);
		}
	}
	timer_sched_timeout_cb(100, TIMEOUT_ONESHOT, cpu2cop_wlif_scan, ctx);
}

static void SSI_wifi_scan(char *pcInsert, int iInsertLen)
{
	static int one = 0;
	if (one == 0) {
		one = 1;
		timer_sched_timeout_cb(100, TIMEOUT_ONESHOT, SSI_wifi_scan_cb, &one);
	}

	UNUSED(pcInsert);
	UNUSED(iInsertLen);
}

int App_WiFi_SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch(iIndex)
	{
		case SSI_WIFI_ap:
			SSI_wifi_ap(pcInsert, iInsertLen);
			break;
		case SSI_WIFI_sta:
			SSI_wifi_sta(pcInsert, iInsertLen);
			break;
		case SSI_WIFI_scan:
			SSI_wifi_scan(pcInsert, iInsertLen);
			break;
		default:
			return DEF_FAIL;
	}
	return DEF_OK;
}
#endif

#if LWIP_HTTPD_CGI

static uint32 un32GetValue = 0;
static uint32 un32TempValue;

static int CGI_wifi_IPCHANG(ip_addr_t *ipaddr, char *pcValue, UINT UIN_MSGTYPE)
{
	un32TempValue = ipaddr_addr(pcValue);
	if(un32TempValue == IPADDR_NONE)
	{
		dprintf("ERROR %s\r\n", __func__);
	}
	else
	{
		if(ipaddr->addr != un32TempValue)
		{
			ipaddr->addr = un32TempValue;
			un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
			return 1;
		}
	}
	return 0;
}

static int CGI_wifi_INT32_MAX_MIN(UINT *unInterger, char *pcValue, UINT UIN_MSGTYPE, UINT nMax, UINT nMin)
{
	un32TempValue = atoi(pcValue);
	if(*unInterger != un32TempValue)
	{
		if(nMax < nMin)
		{
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if(un32TempValue < nMin)		un32TempValue = nMin;
		else if(un32TempValue > nMax)	un32TempValue = nMax;	
		*unInterger = un32TempValue;
		un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
		return 1;
	}
	return 0;
}

#if 0
static int CGI_wifi_INT8_MAX_MIN(BYTE *unInterger, char *pcValue, UINT UIN_MSGTYPE, UINT nMax, UINT nMin)
{
	un32TempValue = atoi(pcValue);
	if(*unInterger != un32TempValue)
	{
		if(nMax < nMin)
		{
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if(un32TempValue < nMin)		un32TempValue = nMin;
		else if(un32TempValue > nMax)	un32TempValue = nMax;	
		*unInterger = un32TempValue;
		un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
		return 1;
	}
	return 0;
}
#endif

static int CGI_wifi_SWITCH(UINT *bSwitch, char *pcValue, UINT UIN_MSGTYPE)
{
	return CGI_wifi_INT32_MAX_MIN(bSwitch, pcValue, UIN_MSGTYPE, 1, 0);
}

static int CGI_wifi_STRING(char *strString, char *pcValue, UINT UIN_MSGTYPE)
{
	if(strcmp(strString, pcValue) != 0)
	{
		strcpy(strString, pcValue);
		un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
		return 1;
	}
	return 0;
}

const char *App_WiFiAP_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	char cmd[80];
	int i;
	un32GetValue = 0;
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);
	for(i=0;i<iNumParams;i++)
	{
		dprintf("%s : %s \r\n", pcParam[i], pcValue[i]);
		///////////////////////////////////////////////////////////////
		// TEST ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if(strcmp(pcParam[i], "n_w_a_sw") == 0)
		{
			CGI_wifi_SWITCH(&(gtWificfg.APcfg.un8Switch), pcValue[i], WIFI_CGI_AP_SW);	// WIFI_CGI_AP_SW
		}
		else if(strcmp(pcParam[i], "n_w_a_id") == 0)
		{
			// AP가 OFF->ON(실제 ON되기 전 이므로) or OFF->OFF 인 경우에만 즉시 적용
			if((bitcheck(un32GetValue, WIFI_CGI_AP_SW)    && gtWificfg.APcfg.un8Switch == WIFI_ON ) ||
			   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_OFF))
			{
				if(CGI_wifi_STRING(gtWificfg.APcfg.strSSID, pcValue[i], WIFI_CGI_AP_OK))	// WIFI_CGI_AP_SSID
				{
					snprintf(cmd, 80, "apset %s %s", WIFI_UAP_SET_CMD_SSID, gtWificfg.APcfg.strSSID);	
					cpu2cop_wlif_execmd(cmd);
				}
			}
			else
			{	// 아닌 경우 EN673 리붓 후 적용
				CGI_wifi_STRING(gtWificfg.APcfg.strSSID, pcValue[i], WIFI_CGI_AP_SSID);
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_ch") == 0)
		{
#if 1
			CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un8Channel), pcValue[i], WIFI_CGI_AP_CH, 13, 1);
#else
			// AP가 OFF->ON(실제 ON되기 전 이므로) or OFF->OFF 인 경우에만 즉시 적용
			if((bitcheck(un32GetValue, WIFI_CGI_AP_SW)	  && gtWificfg.APcfg.un8Switch == WIFI_ON ) ||
			   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_OFF))
			{
				if(CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un8Channel), pcValue[i], WIFI_CGI_AP_OK, 13, 1))	// WIFI_CGI_AP_CH
				{
					snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_CHANNEL, gtWificfg.APcfg.un8Channel);
					cpu2cop_wlif_execmd(cmd);
				}
			}
			else
			{	// 아닌 경우 EN673 리붓 후 적용
				CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un8Channel), pcValue[i], WIFI_CGI_AP_CH, 13, 1);
			}
#endif
		}
		else if(strcmp(pcParam[i], "n_w_a_mc") == 0)
		{
			// AP가 OFF->ON(실제 ON되기 전 이므로) or OFF->OFF 인 경우에만 즉시 적용
			if((bitcheck(un32GetValue, WIFI_CGI_AP_SW)	  && gtWificfg.APcfg.un8Switch == WIFI_ON ) ||
			   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_OFF))
			{
				if(CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un8MaxSTA), pcValue[i], WIFI_CGI_AP_OK, 10, 1))	// WIFI_CGI_AP_MAXSTA
				{
					snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_MAX_SCB, gtWificfg.APcfg.un8MaxSTA);
					cpu2cop_wlif_execmd(cmd);
				}
			}
			else
			{	// 아닌 경우 EN673 리붓 후 적용
				CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un8MaxSTA), pcValue[i], WIFI_CGI_AP_MAXSTA, 10, 1);
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_au") == 0)
		{
			// AP가 OFF->ON(실제 ON되기 전 이므로) or OFF->OFF 인 경우에만 즉시 적용
			if((bitcheck(un32GetValue, WIFI_CGI_AP_SW)	  && gtWificfg.APcfg.un8Switch == WIFI_ON ) ||
			   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_OFF))
			{
				if(CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un32Authentication), pcValue[i], WIFI_CGI_AP_OK, 3, 0))	// WIFI_CGI_AP_AUTH
				{
					snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_SEC, gtWificfg.APcfg.un32Authentication);
					cpu2cop_wlif_execmd(cmd);
				}
			}
			else
			{	// 아닌 경우 EN673 리붓 후 적용
				CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.un32Authentication), pcValue[i], WIFI_CGI_AP_AUTH, 3, 0);
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_pw") == 0)
		{
			// AP가 OFF->ON(실제 ON되기 전 이므로) or OFF->OFF 인 경우에만 즉시 적용
			if((bitcheck(un32GetValue, WIFI_CGI_AP_SW)	  && gtWificfg.APcfg.un8Switch == WIFI_ON ) ||
			   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_OFF))
			{
				if(CGI_wifi_STRING(gtWificfg.APcfg.strKey, pcValue[i], WIFI_CGI_AP_KEY))	// WIFI_CGI_AP_KEY
				{
					snprintf(cmd, 80, "apset %s %s", WIFI_UAP_SET_CMD_KEY, gtWificfg.APcfg.strKey);
					cpu2cop_wlif_execmd(cmd);
				}
			}
			else
			{	// 아닌 경우 EN673 리붓 후 적용
				CGI_wifi_STRING(gtWificfg.APcfg.strKey, pcValue[i], WIFI_CGI_AP_KEY);
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_ip") == 0)
		{
			if(CGI_wifi_IPCHANG(&(gtWificfg.APcfg.iptLocalip), pcValue[i], WIFI_CGI_AP_OK))	// WIFI_CGI_AP_IP
			{
				snprintf(cmd, 80, "apset %s %IP", WIFI_UAP_SET_CMD_IP, gtWificfg.APcfg.iptLocalip.addr);
				cpu2cop_wlif_execmd(cmd);
				netif_set_ipaddr(&gtwlif_uap, &(gtWificfg.APcfg.iptLocalip));
				netif_set_gw(&gtwlif_uap, &(gtWificfg.APcfg.iptLocalip));
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_sn") == 0)
		{
			if(CGI_wifi_IPCHANG(&(gtWificfg.APcfg.iptNetmask), pcValue[i], WIFI_CGI_AP_OK))	// WIFI_CGI_AP_SN
			{
				snprintf(cmd, 80, "apset %s %IP", WIFI_UAP_SET_CMD_NETMASK, gtWificfg.APcfg.iptNetmask.addr);
				cpu2cop_wlif_execmd(cmd);
				netif_set_netmask(&gtwlif_uap, &(gtWificfg.APcfg.iptNetmask));
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_dw") == 0)
		{
			CGI_wifi_SWITCH(&(gtWificfg.APcfg.un8DHCPServer), pcValue[i], WIFI_CGI_AP_OK);	// WIFI_CGI_AP_DHCP
		}
		else if(strcmp(pcParam[i], "n_w_a_sa") == 0)
		{
			if(CGI_wifi_IPCHANG(&(gtWificfg.APcfg.iptStartAddress), pcValue[i], WIFI_CGI_AP_OK))	// WIFI_CGI_AP_DHCP_STARTADD
			{
//				printf("defualt StartAddress ip %s\r\n", ipaddr_ntoa(&gtWificfg.APcfg.iptStartAddress));
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_ea") == 0)
		{
			if(CGI_wifi_IPCHANG(&(gtWificfg.APcfg.iptEndAddress), pcValue[i], WIFI_CGI_AP_OK))	// WIFI_CGI_AP_DHCP_ENDADD
			{
//				printf("defualt EndAddress ip %s\r\n", ipaddr_ntoa(&gtWificfg.APcfg.iptEndAddress));
			}
		}
		else if(strcmp(pcParam[i], "n_w_a_lt") == 0)
		{
			if(CGI_wifi_INT32_MAX_MIN(&(gtWificfg.APcfg.leaseTime), pcValue[i], WIFI_CGI_AP_OK, 864000, 0))	// WIFI_CGI_AP_DHCP_LEASETIME
			{
//				printf("n_w_a_lt leaseTime %d\r\n", gtWificfg.APcfg.leaseTime);	
			}
		}
		else
		{
//			dprintf("Error Msg : %s(%s)\r\n", pcParam[i], pcValue[i]);
		}
	}
	dprintf("==============================================================\r\n");

	if(un32GetValue)
	{
		WifiCFG_Save();

		dprintf("%s(%d) : un32GetValue(%d)\r\n", __func__, __LINE__, un32GetValue);
		if(un32GetValue == WIFI_CGI_AP_CH)	// 채널만 변경된 상태라면...
		{
			snprintf(cmd, 80, "apset %s %d", WIFI_UAP_SET_CMD_CHANNEL, gtWificfg.APcfg.un8Channel);
			cpu2cop_wlif_execmd(cmd);

			snprintf(cmd, 80, "apset %s", WIFI_UAP_SET_CMD_SET);
			cpu2cop_wlif_execmd(cmd);
		} else if((bitcheck(un32GetValue, WIFI_CGI_AP_SW) && gtWificfg.APcfg.un8Switch == WIFI_OFF ) ||
		   (bitcheck(un32GetValue, WIFI_CGI_AP_SW)==0 && gtWificfg.APcfg.un8Switch == WIFI_ON))
		{	// ON -> OFF or ON -> ON 인 상태에서
			// ssid, ch, maxsta, auth, key가 변했다면 리부팅 한다.
			dprintf("%s(%d) : ON->OFF or ON->ON\r\n", __func__, __LINE__);
			if( bitcheck(un32GetValue, WIFI_CGI_AP_SW) ||
				bitcheck(un32GetValue, WIFI_CGI_AP_SSID) ||
				bitcheck(un32GetValue, WIFI_CGI_AP_CH) ||
				bitcheck(un32GetValue, WIFI_CGI_AP_MAXSTA) ||
				bitcheck(un32GetValue, WIFI_CGI_AP_AUTH) ||
				bitcheck(un32GetValue, WIFI_CGI_AP_KEY))
			{
				dprintf("%s(%d) : System Reboot\r\n", __func__, __LINE__);
				WdtSysReboot();
			}
		}

		if(bitcheck(un32GetValue, WIFI_CGI_AP_SW))
		{
			if(gtWificfg.APcfg.un8Switch == WIFI_ON)
			{
				if(gtWificfg.APcfg.un32State == WLIF_NONE)
				{
					dprintf("call WiFiAP_start()\r\n");
					WiFiAP_set();
					WiFiAP_start();
					gtWificfg.APcfg.un32State = WLIF_UAP_START;
					goto cgi_ap_end;
				}
			}
		}
	}

cgi_ap_end:
	un32GetValue = 0;
	return "/submit.html";

	UNUSED(iIndex);
}

extern void AppReboot_process(void *ctx);
const char *App_WiFiSTA_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i, reboot = 0;
	un32GetValue = 0;
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);

	UINT unWiFiSTAIpChangeFlag = 0;

#if 0
	printf("%s(%d) : start scan\r\n", __func__, __LINE__);
	wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlif->cmd;
	gptMsgWlif->resp = 0; // ask cpu1 to reply for cpu0
	cpu2cop_wlif_execmd("scanned");
	while(gptMsgWlif->resp != 1) vTaskDelay(1);
	pwlif_scan = (wlif_scan_t *)gptMsgWlif->cmd;
	wl_scans_t *pscan = NULL;
#endif
	for(i=0;i<iNumParams;i++)
	{
		dprintf("%s : [%s]\r\n", pcParam[i], pcValue[i]);
		///////////////////////////////////////////////////////////////
		// TEST ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if(strcmp(pcParam[i], "n_w_s_sw") == 0)
		{
			if(CGI_wifi_SWITCH(&(gtWificfg.STAcfg.un8Switch), pcValue[i], WIFI_CGI_STA_SW))
			{

			}
		}
		else if(strcmp(pcParam[i], "n_w_s_li") == 0)
		{
			if(CGI_wifi_STRING(gtWificfg.STAcfg.strSSID, pcValue[i], WIFI_CGI_STA_SSID))
			{
				reboot = bitchang(reboot, 0x2, 1);	// AP접속에 관한 설정이 변경(1번 bit)
			}
		}
		else if(strcmp(pcParam[i], "n_w_s_au") == 0)
		{
			if(CGI_wifi_INT32_MAX_MIN(&(gtWificfg.STAcfg.un32Authentication), pcValue[i], WIFI_CGI_STA_AU, 3, 0))
			{
				reboot = bitchang(reboot, 0x2, 1);	// AP접속에 관한 설정이 변경(1번 bit)
			}
		}
		else if(strcmp(pcParam[i], "n_w_s_pw") == 0)
		{
			if(CGI_wifi_STRING(gtWificfg.STAcfg.strKey, pcValue[i], WIFI_CGI_STA_KEY))
			{
				reboot = bitchang(reboot, 0x2, 1);	// AP접속에 관한 설정이 변경(1번 bit)
			}
		}
		else if(strcmp(pcParam[i], "n_w_s_cn") == 0)
		{
			if(CGI_wifi_SWITCH(&(gtWificfg.STAcfg.un8Connect), pcValue[i], WIFI_CGI_STA_CONN))
			{
				reboot = bitchang(reboot, 0x1, 1);	// AP접속에 관한 설정이 변경(0번 bit)
			}
			else
			{
				if(gtWificfg.STAcfg.un8Connect == WIFI_ON)
				{
					reboot = bitchang(reboot, 0x4, 1);	// AP접속에 관한 설정이 변경(2번 bit)
				}
			}
		}
		else if(strcmp(pcParam[i], "n_w_dhcp") == 0)
		{
			if (CGI_wifi_SWITCH(&(gtWificfg.STAcfg.un8DHCPSwitch), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_DHCP;
		}
		else if(strcmp(pcParam[i], "n_w_ip") == 0)
		{
			if (CGI_wifi_IPCHANG(&(gtWificfg.STAcfg.iptLocalip), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_IP;
		}
		else if(strcmp(pcParam[i], "n_w_sn") == 0)
		{
			if (CGI_wifi_IPCHANG(&(gtWificfg.STAcfg.iptNetmask), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_SN;
		}
		else if(strcmp(pcParam[i], "n_w_gw") == 0)
		{
			if (CGI_wifi_IPCHANG(&(gtWificfg.STAcfg.iptGateway), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_GW;
		}
		else if(strcmp(pcParam[i], "n_w_dns0") == 0)
		{
			if (CGI_wifi_IPCHANG(&(gtWificfg.STAcfg.iptDNS0), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_DNS0;
		}
		else if(strcmp(pcParam[i], "n_w_dns1") == 0)
		{
			if (CGI_wifi_IPCHANG(&(gtWificfg.STAcfg.iptDNS1), pcValue[i], WIFI_CGI_STA_NETIF) == DEF_OK)
				unWiFiSTAIpChangeFlag |= UIN_NW_INT_DNS1;
		}
		else
		{
			dprintf("Error Msg : %s(%s)\r\n", pcParam[i], pcValue[i]);
		}
	}
	dprintf("==============================================================\r\n");

	// Wi-Fi info FLS save
	if(un32GetValue != 0)
	{
		WifiCFG_Save();
	}

	int rebootflag = 0;
#if 1
	if(gtWificfg.APcfg.un32State != WLIF_NONE)
	{
		if(gtWificfg.APcfg.un8Switch != WIFI_OFF)
		{
			if(un32GetValue & ~WIFI_CGI_STA_OK)	// 변경정보가 단순정보가 아닌 경우에만 재부팅 체크
			{
				printf("reboot info 0bit (0x%X)\r\n", bitcheck(reboot, 0x1));
				printf("reboot info 1bit (0x%X)\r\n", bitcheck(reboot, 0x2));
				printf("reboot info 2bit (0x%X)\r\n", bitcheck(reboot, 0x4));
				// 접속할 STA정보가 변경되었다면 시스템 재부팅
				if(bitcheck(reboot, 0x1))
				{	// Disconnect -> Connect
					dprintf("%s(%d) : System Reboot\r\n", __func__, __LINE__);
					timer_sched_timeout_cb(500, TIMEOUT_ONESHOT, AppReboot_process, NULL);
					rebootflag = 1;
				}
				else if(bitcheck(reboot, 0x2) && bitcheck(reboot, 0x4))
				{	// Conn상태(0x4)에서 SSID등의 값(0x2) 변경
					dprintf("%s(%d) : System Reboot\r\n", __func__, __LINE__);
					timer_sched_timeout_cb(500, TIMEOUT_ONESHOT, AppReboot_process, NULL);
					rebootflag = 1;
				}
			}
		}
	}
#else
	if (un32GetValue & WIFI_CGI_STA_CONN) {
		if (gtWificfg.STAcfg.un8Connect == WIFI_ON) {
			flprintf("Wi-Fi STA Active\r\n");
			xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_ACTIVE, NULL, enlWIFISTA);
		} else {
			
		}
	}
#endif
	if(gtWificfg.STAcfg.un32State == WLIF_NONE)
	{
		if(gtWificfg.STAcfg.un8Switch == WIFI_ON)
		{
			xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_START, NULL, enlWIFISTA);
		}
	}
	else
	{
		if(gtWificfg.STAcfg.un8Switch == WIFI_ON)
		{
			if(gtWificfg.STAcfg.un8Connect == WIFI_ON)
			{
				if(un32GetValue & WIFI_CGI_STA_OK)	// 변경정보가 단순정보가 아닌 경우에만 재부팅 체크
				{	// 연결되어 있는 상태에서 값의 변경이 존재하면, 연결을 끊고(disconnect), CONNECT_WAIT상태로 넘어가서, 다시 진행하도록 한다.
					if(gtWificfg.STAcfg.un32State == WLIF_CONNECTED)
						cpu2cop_wlif_execmd("disconnect"); //we first, disconnect w
					else
						gtWificfg.STAcfg.un32State = WLIF_STA_ACTIVE;
					while((gtWificfg.STAcfg.un32State != WLIF_STA_ACTIVE))
					{
						vTaskDelay(10);
					}

					flprintf("WLIF_STA_CONNECT_WAIT\r\n");
					xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_CONNECT_WAIT, NULL, enlWIFISTA);
				}
				else if (unWiFiSTAIpChangeFlag) {
					enx_ipchange_proc(enlWIFISTA, unWiFiSTAIpChangeFlag);
				}
			}
			else
			{
				printf("%s(%d) : cmd : disconnect\r\n", __func__, __LINE__);
				cpu2cop_wlif_execmd("disconnect");
			}
		}
		else
		{
			printf("%s(%d) : cmd : disconnect\r\n", __func__, __LINE__);
			cpu2cop_wlif_execmd("disconnect");
		}
	}

	un32GetValue = 0;
	if (rebootflag == 1) {
		return "/wifi/wifi_submit.html";
	} else {
		return "/submit.html";
	}

	UNUSED(iIndex);
}
#endif

#endif
