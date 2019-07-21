/* ------------------------ OR32 includes -------------------------------- */
#include <stdlib.h>
#include <string.h>

#include "dev.h"

#include "lwip/igmp.h"

#include "inter_shell.h"
#include "msg.h"
#include "wl_tools.h"
#include "enx_os.h"
#include "ewl.h"
#include "wlif_tools.h"
#include "enx_wifi_helper.h"
#include "enx_lwip_helper.h"

#ifdef __WIFI__

int STA_ID = -1;

/*
    extern local functions 
*/
int connect_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int scan_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int scanned_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int disconnect_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int sta_start_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int sta_set_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int wifi_sta_info_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int wifi_shell_sta_on(struct enxsh_ctx *sh, int argc, const char *argv[]);
int wifi_shell_sta_off(struct enxsh_ctx *sh, int argc, const char *argv[]);

int wifi_status_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int ap_set_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int ap_get_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int ap_show_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int ap_exset_f(struct enxsh_ctx *sh, int argc, const char *argv[]);
int ap_start_f(struct enxsh_ctx *sh, int argc, const char *argv[]);

static struct enxsh_ctx *enx_shell;

static struct enxsh_cmd cmd_connect            = { "connect",     connect_f,          "connect to AP",          NULL, NULL};
static struct enxsh_cmd cmd_sta_set            = { "staset",      sta_set_f,          "set wifi sta",           NULL, NULL};
static struct enxsh_cmd cmd_scan               = { "scan",        scan_f,             "scan APs",               NULL, NULL};
static struct enxsh_cmd cmd_scanned            = { "scanned",     scanned_f,          "old scan APs info",      NULL, NULL};
static struct enxsh_cmd cmd_disconnect         = { "disconnect",  disconnect_f,       "disconnect current APs", NULL, NULL};
static struct enxsh_cmd cmd_sta_start          = { "stastart",    sta_start_f,        "wifi sta start",         NULL, NULL};
static struct enxsh_cmd cmd_wifi_sta_info      = { "wifistainfo", wifi_sta_info_f,    "get wifi-sta info",      NULL, NULL};
static struct enxsh_cmd cmd_wifi_shell_sta_on  = { "staon",       wifi_shell_sta_on,  "sta on",                 NULL, NULL};
static struct enxsh_cmd cmd_wifi_shell_sta_off = { "staoff",      wifi_shell_sta_off, "sta off",                NULL, NULL};
static struct enxsh_cmd cmd_ap_set             = { "apset",       ap_set_f,           "set wifi ap",            NULL, NULL};
static struct enxsh_cmd cmd_ap_get             = { "apget",       ap_get_f,           "get wifi ap",            NULL, NULL};
static struct enxsh_cmd cmd_ap_show            = { "apshow",      ap_show_f,          "show wifi ap config",    NULL, NULL};
static struct enxsh_cmd cmd_ap_set_extended    = { "apexset",     ap_exset_f,         "set wifi ap extended config", NULL, NULL};
static struct enxsh_cmd cmd_ap_start           = { "apstart",     ap_start_f,         "start wifi ap",          NULL, NULL};
static struct enxsh_cmd cmd_wifi_stauts        = { "wifistatus",  wifi_status_f,      "get wifi status",        NULL, NULL};

int ap_get_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	gptMsgWlifCmd.resp = 2; //signal waiting to cpu0
	wlif_ewlap_cfg *pwlif_apcfg = (wlif_ewlap_cfg *)gptMsgWlifCmd.cmd;
    struct ewlap_cfg ap_cfg;
    if(!ewlap_get_apcfg(&ap_cfg))
    {
    	pwlif_apcfg->flags = 1;	// ok
		strcpy(pwlif_apcfg->ssid, ap_cfg.ssid);
		strcpy(pwlif_apcfg->passphrase, ap_cfg.passphrase);
		pwlif_apcfg->channel = ap_cfg.channel;
		pwlif_apcfg->enc_proto = ap_cfg.enc_proto;
		pwlif_apcfg->max_sta_count = ap_cfg.max_sta_count;
    }
	else
	{
		pwlif_apcfg->flags = 0;	// fail
	}
	gptMsgWlifCmd.resp = 1;
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int wifi_sta_info_results_xfer2cpu0(wl_scans_t *results)
{
	int i = 0;
	if(results == NULL)
	{
		printf("%s(%d) : result NULL\r\n", __func__, __LINE__);
	}
	else
	{
	    struct wpa_ssid *ssid = ewl_get_network_lists();
		for(i=0;ssid!=NULL;ssid=ssid->next,i++)
		{
			if(ssid->ssid && ssid->ssid_len>0)
			{
				memcpy(results[i].ssid, ssid->ssid, ssid->ssid_len);
				results[i].ssid[ssid->ssid_len] = 0;

				memcpy(results[i].bssid, ssid->bssid, 6);

				// temp : passphrase => flage (wifi-key)
				if(ssid->passphrase)
				{
					strcpy(results[i].flags, ssid->passphrase);
				}
			}
			else
			{
				printf("%s(%d) : ssid->ssid lenth 0\r\n", __func__, __LINE__);
			}
		}
	}
	return i;
}

int wifi_sta_info_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	gptMsgWlifCmd.resp = 2; //signal waiting to cpu0
	wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
	pwlif_scan->num = wifi_sta_info_results_xfer2cpu0(pwlif_scan->wl_scans);
	gptMsgWlifCmd.resp = 1;
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int wifi_status_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	enx_os_msleep(100);
	wl_print_list_networks();
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int sta_start_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	enx_wifi_sta_start();
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

static void disconnect_cb(int issue)
{
    switch(issue)
    {
        case NO_AP:
            ewl_remove_network(STA_ID);
			xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_NO_AP, NULL, enlWIFISTA);
            break;

        case WPA_AUTH_FAIL:
            ewl_remove_network(STA_ID);
			xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_WPA_AUTH_FAIL, NULL, enlWIFISTA);
            break;

        default:
            break;
    }
}

/*
    argv
    0       1    2     3
    connect wpa  ssid psk
            open
            wep
*/

int connect_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
    STA_ID = ewl_add_netowrk();
    if(argc >= 3 && argc <= 4)
    {
        int err = ewl_set_network(STA_ID, "ssid", (char *)argv[2]);
		if(err == -1)
			dprintf("%s(%d) : error!!!!!!!!!!!!\r\n", __func__, __LINE__);
        if(!strncmp(argv[1], "wpa", 3))
        {
			err = ewl_set_network(STA_ID, "psk", (char *)argv[3]);
			gtWificfg.STAcfg.un32Authentication = wifi_WPA2_PSK;
        }
		else
		{
			err = ewl_set_network(STA_ID, "key_mgmt", "NONE");
			gtWificfg.STAcfg.un32Authentication = wifi_OPEN;
		}
		ewl_set_disconnect_cb(disconnect_cb);
		printf("STA enable(%d)\r\n", STA_ID);
        err = ewl_enable_network(STA_ID);
		if(err != 0)
			dprintf("%s(%d) : error!!!!!!!!!!!!\r\n", __func__, __LINE__);
    }
    return 0;
	UNUSED(sh);
}

int disconnect_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	struct netif* stanetif = enx_wifi_get_sta_netif();
	gtWificfg.STAcfg.iptLocalip.addr = (u32_t)IP_ADDR_ANY;
	gtWificfg.STAcfg.iptGateway.addr = (u32_t)IP_ADDR_ANY;
	gtWificfg.STAcfg.iptNetmask.addr = (u32_t)IP_ADDR_ANY;
	netif_set_ipaddr(stanetif, IP_ADDR_ANY);
	netif_set_gw(stanetif, IP_ADDR_ANY);
	netif_set_netmask(stanetif, IP_ADDR_ANY);
    ewl_remove_network(STA_ID);
    return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int sta_set_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	if(!strncmp(argv[1], WIFI_STA_SET_CMD_IP, strlen(WIFI_STA_SET_CMD_IP)))
	{
		struct netif* stanetif = enx_wifi_get_sta_netif();
		ipaddr_aton(argv[2], &gtWificfg.STAcfg.iptLocalip);
		netif_set_ipaddr(stanetif, &(gtWificfg.STAcfg.iptLocalip));
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_NETMASK, strlen(WIFI_STA_SET_CMD_NETMASK)))
	{
		struct netif* stanetif = enx_wifi_get_sta_netif();
		ipaddr_aton(argv[2], &gtWificfg.STAcfg.iptNetmask); 
		netif_set_netmask(stanetif, &(gtWificfg.STAcfg.iptNetmask));
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_GATEWAY, strlen(WIFI_STA_SET_CMD_GATEWAY)))
	{
		struct netif* stanetif = enx_wifi_get_sta_netif();
		ipaddr_aton(argv[2], &gtWificfg.STAcfg.iptGateway);
		netif_set_gw(stanetif, &(gtWificfg.STAcfg.iptGateway));
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_NETUP, strlen(WIFI_STA_SET_CMD_NETUP)))
	{
		wifi_sta_setNet(1);
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_NETDOWN, strlen(WIFI_STA_SET_CMD_NETDOWN)))
	{
		wifi_sta_setNet(0);
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_LINKUP, strlen(WIFI_STA_SET_CMD_LINKUP)))
	{
		wifi_sta_setLinkNet(1);
	}
	else if(!strncmp(argv[1], WIFI_STA_SET_CMD_LINKDOWN, strlen(WIFI_STA_SET_CMD_LINKDOWN)))
	{
		wifi_sta_setLinkNet(0);
	}
#ifdef LWIP_IGMP
	else if(!strncmp(argv[1], WIFI_SET_CMD_IGMPJOIN, strlen(WIFI_SET_CMD_IGMPJOIN)))
	{
		ip_addr_t groupaddr;
	    groupaddr.addr = ipaddr_addr(argv[2]);
		err_t err = igmp_joingroup(IP_ADDR_ANY, &groupaddr);
		if(err != ERR_OK)
			printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
	}
	else if(!strncmp(argv[1], WIFI_SET_CMD_IGMPLEAVE, strlen(WIFI_SET_CMD_IGMPLEAVE)))
	{
		ip_addr_t groupaddr;
	    groupaddr.addr = ipaddr_addr(argv[2]);
		err_t err = igmp_leavegroup(IP_ADDR_ANY, &groupaddr);
		if(err != ERR_OK)
			printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
	}
#endif
	else
	{
		printf("%s(%d) : Unknown set command(%s)\r\n", __func__, __LINE__, argv[1]);
	}
	return 0;
	UNUSED(sh);
	UNUSED(argc);
}	

int ap_set_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	if (!strncmp(argv[1], WIFI_UAP_SET_CMD_SSID, strlen(WIFI_UAP_SET_CMD_SSID))) {
		strcpy((char *)gtWificfg.APcfg.strSSID, argv[2]);
		ewlap_init_apcfg((char *)gtWificfg.APcfg.strSSID);	// ssid set => apcfg init
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_KEY, strlen(WIFI_UAP_SET_CMD_KEY))) {	
		strcpy((char *)gtWificfg.APcfg.strKey, argv[2]);
		ewlap_apcfg(WIFI_UAP_SET_CMD_KEY, (char *)gtWificfg.APcfg.strKey);
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_CHANNEL, strlen(WIFI_UAP_SET_CMD_CHANNEL))) {
		gtWificfg.APcfg.un8Channel = atoi(argv[2]);
		ewlap_apcfg(WIFI_UAP_SET_CMD_CHANNEL, (char *)argv[2]);
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_MAX_SCB, strlen(WIFI_UAP_SET_CMD_MAX_SCB))) {
		gtWificfg.APcfg.un8MaxSTA = atoi(argv[2]);
		ewlap_apcfg(WIFI_UAP_SET_CMD_MAX_SCB, (char *)argv[2]);
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_SEC, strlen(WIFI_UAP_SET_CMD_SEC))) {
		gtWificfg.APcfg.un32Authentication = atoi(argv[2]);
		switch (gtWificfg.APcfg.un32Authentication) {
			case wifi_OPEN:
				ewlap_apcfg(WIFI_UAP_SET_CMD_SEC, "open");
				break;
			case wifi_WPAnWPA2_PSK:
			case wifi_WPA_PSK:
				gtWificfg.APcfg.un32Authentication = wifi_WPA2_PSK;
			case wifi_WPA2_PSK:
				ewlap_apcfg(WIFI_UAP_SET_CMD_SEC, "wpa2-psk");
				break;
			default:
				dprintf("%s apset %s error(ver:%d)\r\n", __func__, WIFI_UAP_SET_CMD_SEC, gtWificfg.APcfg.un32Authentication);
				break;
		}
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_SET, strlen(WIFI_UAP_SET_CMD_SET))) {
		int o_res = ewlap_set_apcfg();
		if (o_res) {
			printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_SET, o_res);
		}
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_BSSON, strlen(WIFI_UAP_SET_CMD_BSSON))) {
		int i, o_res;
		for (i = 0; i < 3; i++) {
			o_res = ewlap_bss_start();
			if(o_res) {
				printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSON, o_res);
			} else {
				break;
			}
		}
		if (i == 3) { // ap bss start fail
			o_res = ewlap_bss_reset();
			if (o_res) {
				printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSRESET, o_res);
				netif_set_down(enx_wifi_get_ap_netif());
				xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ERROR, NULL, enlWIFIUAP);
			} else {
				o_res = ewlap_bss_start();
				if (o_res) {
					printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSON, o_res);
					netif_set_down(enx_wifi_get_ap_netif());
					xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ERROR, NULL, enlWIFIUAP);
				} else {
					printf("%s(%d) : AP on ok\r\n", __func__, __LINE__);
					netif_set_up(enx_wifi_get_ap_netif());
					xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ON, NULL, enlWIFIUAP);
				}
			}
		} else {
			printf("%s(%d) : AP on ok\r\n", __func__, __LINE__);
			netif_set_up(enx_wifi_get_ap_netif());
			xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ON, NULL, enlWIFIUAP);
		}
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_BSSOFF, strlen(WIFI_UAP_SET_CMD_BSSOFF)))  {
		int i, o_res;
		netif_set_down(enx_wifi_get_ap_netif());
		for (i = 0; i < 3; i++) {
			o_res = ewlap_bss_stop();
			if (o_res) {
				printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSOFF, o_res);
			} else {
				break;
			}
		}
		if (i == 3) { // ap bss stop fail
			o_res = ewlap_bss_reset();
			if (o_res) {
				printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSRESET, o_res);
				xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ERROR, NULL, enlWIFIUAP);
			} else {
				o_res = ewlap_bss_stop();
				if (o_res) {
					printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSOFF, o_res);
					xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_ERROR, NULL, enlWIFIUAP);
				} else {
					printf("%s(%d) : AP off ok\r\n", __func__, __LINE__);
					xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_OFF, NULL, enlWIFIUAP);
				}
			}
		} else {
			printf("%s(%d) : AP off ok\r\n", __func__, __LINE__);
			xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_BSS_OFF, NULL, enlWIFIUAP);
		}
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_BSSRESET, strlen(WIFI_UAP_SET_CMD_BSSRESET))) {
		int o_res = ewlap_bss_reset();
		if (o_res) {
			printf("%s(%d) : AP %s(%d)\r\n", __func__, __LINE__, WIFI_UAP_SET_CMD_BSSRESET, o_res);
		}
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_IP, strlen(WIFI_UAP_SET_CMD_IP))) {
		struct netif* apnetif = enx_wifi_get_ap_netif();
		ipaddr_aton(argv[2], &gtWificfg.APcfg.iptLocalip);
		netif_set_gw(apnetif, &(gtWificfg.APcfg.iptLocalip));
		netif_set_ipaddr(apnetif, &(gtWificfg.APcfg.iptLocalip));
	} else if (!strncmp(argv[1], WIFI_UAP_SET_CMD_NETMASK, strlen(WIFI_UAP_SET_CMD_NETMASK))) {
		struct netif* apnetif = enx_wifi_get_ap_netif();
		ipaddr_aton(argv[2], &gtWificfg.APcfg.iptNetmask);
		netif_set_netmask(apnetif, &(gtWificfg.APcfg.iptNetmask));
	}
#ifdef LWIP_IGMP
	else if (!strncmp(argv[1], WIFI_SET_CMD_IGMPJOIN, strlen(WIFI_SET_CMD_IGMPJOIN))) {
		ip_addr_t groupaddr;
		groupaddr.addr = ipaddr_addr(argv[2]);
		err_t err = igmp_joingroup(IP_ADDR_ANY, &groupaddr);
		if(err != ERR_OK)
			printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
	} else if (!strncmp(argv[1], WIFI_SET_CMD_IGMPLEAVE, strlen(WIFI_SET_CMD_IGMPLEAVE))) {
		ip_addr_t groupaddr;
		groupaddr.addr = ipaddr_addr(argv[2]);
		err_t err = igmp_leavegroup(IP_ADDR_ANY, &groupaddr);
		if(err != ERR_OK)
			printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
	}
#endif
	else {
		printf("%s(%d) : Unknown set command(%s)\r\n", __func__, __LINE__, argv[1]);
	}
	return 0;
	UNUSED(sh);
	UNUSED(argc);
}

int ap_exset_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	if (argc == 3) {
		printf("%s: AP extended command set(%s %s)\r\n", __func__, argv[1], argv[2]);
		if (ewlap_apcfg((char *)argv[1], (char *)argv[2]) == 0) {
			printf("AP extended command OK\r\n");
		} else {
			printf("AP extended command Fail\r\n");
		}
	} else {
		printf("AP extended command Fail(argc:%d)\r\n", argc);
	}
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int ap_show_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	wl_print_apcfg();
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int ap_start_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	enx_wifi_ap_start();	
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

/////////////////////////////////////////////////////////////////////////////////////////
// STA Mode Scan ========================================================================
/////////////////////////////////////////////////////////////////////////////////////////
void scan_results_xfer2cpu0(int num_scan, struct wpa_scan_result *results_scan, wl_scans_t *results)
{
	int i=0;
	struct wpa_scan_result *scans;
	char *flags;

	for (i = 0; i < num_scan; i++) {
		scans = &results_scan[i];

		memcpy(results[i].ssid, scans->ssid, scans->ssid_len);
		results[i].ssid[scans->ssid_len] = 0;

		memcpy(results[i].bssid, scans->bssid, 6);
		results[i].freq   = scans->freq;
		results[i].level  = scans->level;
		flags             = results[i].flags;
		flags[0] = 0;
		if (scans->wpa_ie_len) {
			wl_sprintf_ie_txt(flags,
			"WPA",
			scans->wpa_ie,
			scans->wpa_ie_len);
			flags += strlen(flags);
		}

		if (scans->rsn_ie_len) {
			wl_sprintf_ie_txt(flags,
			"WPA2",
			scans->rsn_ie,
			scans->rsn_ie_len);
			flags += strlen(flags);
		}

		if (!scans->wpa_ie_len && !scans->rsn_ie_len &&
			scans->caps & IEEE80211_CAP_PRIVACY) {
			sprintf(flags,"[WEP]");
			flags += strlen(flags);	
		}

		if (scans->caps & IEEE80211_CAP_IBSS) {
			sprintf(flags,"[IBSS]");
		}
	}
	return;
}

/**
    scan callback function.
    when scan have been completed, This fn is called. 
*/
static void scan_complete_cb(void)
{
	printf("Wi-Fi Scan Complete Start\r\n");
	struct wpa_scan_result *results;
	wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
	pwlif_scan->num = ewl_get_scan_results(&results);
	if(pwlif_scan->num)
	{
		scan_results_xfer2cpu0(pwlif_scan->num, results, pwlif_scan->wl_scans);
   	}
	printf("Wi-Fi Scan Complete End - cnt(%d)\r\n", pwlif_scan->num);
	ewl_sta_get_freq(listNetif[enlWIFISTA]);
	free(results);
	gptMsgWlifCmd.resp = 1;
	return;
}

int scanned_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	gptMsgWlifCmd.resp = 2; //signal waiting to cpu0
	printf("Wi-Fi Scanned Start\r\n");
	struct wpa_scan_result *results;
	wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
	pwlif_scan->num = ewl_get_scan_results(&results);
	if(pwlif_scan->num)
	{
		scan_results_xfer2cpu0(pwlif_scan->num, results, pwlif_scan->wl_scans);
	}
	printf("Wi-Fi Scanned End - cnt(%d)\r\n", pwlif_scan->num);
	free(results);
	gptMsgWlifCmd.resp = 1;
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int scan_f(struct enxsh_ctx *sh, int argc, const char *argv[])
{
    gptMsgWlifCmd.resp = 2; //signal waiting to cpu0(processing:scan_complete_cb)
    ewl_scan(scan_complete_cb);
    return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

/////////////////////////////////////////////////////////////////////////////////////////
// CPU1 SHELL cmd =======================================================================
/////////////////////////////////////////////////////////////////////////////////////////
int wifi_shell_sta_on(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	dprintf("%s Start : SSID(%s)\r\n", __func__, gtWificfg.STAcfg.strSSID);
	STA_ID = ewl_add_netowrk();
	ewl_set_network(STA_ID, "ssid", (char *)gtWificfg.STAcfg.strSSID);
	switch(gtWificfg.STAcfg.un32Authentication)
	{
		case wifi_WPA2_PSK:
			ewl_set_network(STA_ID, "psk", (char *)gtWificfg.STAcfg.strKey);
			break;
		case wifi_OPEN:
			ewl_set_network(STA_ID, "key_mgmt", "NONE");
			break;
		default:
			dprintf("%s Error(%d)\r\n", __func__, __LINE__);
			break;
	}
	ewl_enable_network(STA_ID);
	dprintf("SSID(%s), PW(%s)\r\n", gtWificfg.STAcfg.strSSID, gtWificfg.STAcfg.strKey);
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

int wifi_shell_sta_off(struct enxsh_ctx *sh, int argc, const char *argv[])
{
	dprintf("%s Start\r\n", __func__);
	switch(ewl_remove_network(STA_ID))
	{
		case -1:
			dprintf("%s Fail\r\n", __func__);
			break;
		case 0:
			dprintf("%s OK\r\n", __func__);
			break;
	}
	return 0;
	UNUSED(sh);
	UNUSED(argc);
	UNUSED(argv);
}

void inter_cpu_init(void)
{
	if((enx_shell = enxsh_create(NULL)) == NULL) 
	{
		printf("could not create shell\r\n");
		return;
	}

	enxsh_addcmd(enx_shell, &cmd_connect, NULL);
	enxsh_addcmd(enx_shell, &cmd_scan, NULL);
	enxsh_addcmd(enx_shell, &cmd_wifi_sta_info, NULL);	// wifi-CGI
	enxsh_addcmd(enx_shell, &cmd_scanned, NULL);		// wifi-CGI
	enxsh_addcmd(enx_shell, &cmd_disconnect, NULL);
	enxsh_addcmd(enx_shell, &cmd_sta_start, NULL);
	enxsh_addcmd(enx_shell, &cmd_sta_set, NULL);
	enxsh_addcmd(enx_shell, &cmd_wifi_shell_sta_on, NULL);
	enxsh_addcmd(enx_shell, &cmd_wifi_shell_sta_off, NULL);

	enxsh_addcmd(enx_shell, &cmd_ap_get, NULL);			// wifi-CGI
	enxsh_addcmd(enx_shell, &cmd_ap_set, NULL);
	enxsh_addcmd(enx_shell, &cmd_ap_show, NULL);
	enxsh_addcmd(enx_shell, &cmd_ap_set_extended, NULL);
	enxsh_addcmd(enx_shell, &cmd_ap_start, NULL);
	enxsh_addcmd(enx_shell, &cmd_wifi_stauts, NULL);
}

/*
    process sh command
*/
void inter_cpu_execmd(void)
{
	if(cop2cpu_wlif_getcmd(enx_shell->buf, enx_shell->buflen)) 
	{
		enxsh_schedule_cmd(enx_shell);
	}
}
#endif
