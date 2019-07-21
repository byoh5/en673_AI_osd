//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include <stdlib.h>		// for atoi
#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sys_arch.h"

#include "lwip/api.h"
#include "lwip/ip_addr.h"
#include "lwip/netifapi.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
#include "lwip/inet.h"

#include "shell.h"
#include "shell_cmd_net.h"

#include "msg.h"
#include "user.h"
#ifdef __ETH__
#include "phy.h"
#endif

#ifdef __WIFI__
#include "wlif_tools.h"
#include "wifi.h"
#endif

#include "init.h"
#include "timer_task.h"
#if (ENX_SNTPC_use==1)
#include "sntpc.h"
#endif
#if (ENX_FTPD_use==1)
#include "ftp_server.h"
#endif
#include "dhcpc.h"
#include "ipchange.h"

#ifdef __ETH__
const char *sMacCmd[]       = {"MacAddress Set / Get",          (char*)0};
#endif
const char *sIfconfigCmd[]  = {"Network Setting View",          (char*)0};
const char *sDhcpCmd[]      = {"DHCP On/Off",                   (char*)0};
#if (ENX_SNTPC_use==1)
const char *sSntpCmd[]      = {"SNTP On/Off",                   (char*)0};
#endif
const char *sNetstatCmd[]   = {"show TCP/IP network state",     (char*)0};
#if (ENX_FTPD_use==1)
const char *sFtpdCmd[]      = {"FTP Server On/Off",             (char*)0};
#endif
#ifdef __WIFI__
const char *sWiFiStatus[]   = {"wifistatus",                    (char*)0};
const char *sWiFiSW[]       = {"wifi On/Off",                   (char*)0};
#endif
#ifdef __ETH__
const char *sEthCmd[]       = {"show ethernet status",          (char*)0};
#endif
const char *sNetTx[]        = {"Network TX",                    (char*)0};
const char *sRtspState[]    = {"RTSP connect state",            (char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
#include "lwip/dns.h"
extern struct netif *netif_list;
INT16S cmd_ifconfig(INT32S argc, char *argv[])
{
#ifdef __ETH__
	if (argc >= 2) {
		const char *strHelp = "%s [-?] [-ip] [-sn] [-gw]\r\n"\
			"-ip 000.000.000.000 : Set IPAddress\r\n"\
			"-sn 000.000.000.000 : Set SubnetMask\r\n"\
			"-gw 000.000.000.000 : Set Gateway\r\n"\
			"ex) OK : %s -ip x.x.x.x\r\n"\
			"    OK : %s -ip x.x.x.x -sn x.x.x.x\r\n"\
			"    OK : %s -ip x.x.x.x -sn x.x.x.x -gw x.x.x.x\r\n";
		if (strcmp(argv[1], "-?") == 0) {
			printf(strHelp, argv[0], argv[0], argv[0], argv[0]);
		} else if(argc >= 3 && (argc & 1)) {	// 3개이상이고 명령이 홀수인 경우에만 ok
			UINT i, var = 0, unIpFlag = 0;
			for (i = 1; i < (UINT)argc; i += 2) {
				var = ipaddr_addr(argv[i+1]);
				if (strcmp(argv[i], "-ip") == 0) {
					unIpFlag |= UIN_NW_INT_IP;
					gtUser.nIpAddr = var;
					printf("Set IPAddress: %IP\r\n", gtUser.nIpAddr);
				} else if(strcmp(argv[i], "-sn") == 0) {
					unIpFlag |= UIN_NW_INT_SN;
					gtUser.nNetMask = var;
					printf("Set SubnetMask: %IP\r\n", gtUser.nNetMask);
				} else if(strcmp(argv[i], "-gw") == 0) {
					unIpFlag |= UIN_NW_INT_GW;
					gtUser.nGateway = var;
					printf("Set Gateway: %IP\r\n", gtUser.nGateway);
				}
			}
			if (unIpFlag != 0) {
				enx_ipchange_proc(enlETHERNET, unIpFlag);
			}
		} else {
			printf(strHelp, argv[0], argv[0], argv[0], argv[0]);
		}
	} else
#endif
	{
		struct netif *_netif;
		printf("CPU%d ifconfig\r\n", CpuIdGet());
		ip_addr_t dns0 = dns_getserver(0);
		ip_addr_t dns1 = dns_getserver(1);
		printf("DNS	addr0:%IP addr1:%IP\r\n", ip4_addr_get_u32(&dns0), ip4_addr_get_u32(&dns1));
		for (_netif = netif_list; _netif != NULL; _netif = _netif->next) {
			printf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\r\n", _netif->name[0], _netif->name[1], _netif->num,
				_netif->hwaddr[0], _netif->hwaddr[1], _netif->hwaddr[2], _netif->hwaddr[3], _netif->hwaddr[4], _netif->hwaddr[5]);
			printf("	inet addr:%IP Mask:%IP GW:%IP\r\n", ip4_addr_get_u32(&_netif->ip_addr), ip4_addr_get_u32(&_netif->netmask), ip4_addr_get_u32(&_netif->gw));
			printf("	%s%s%s%s%s%s%sMTU:%d %s\r\n",
				_netif->flags & NETIF_FLAG_UP ? "UP " : "",
				_netif->flags & NETIF_FLAG_BROADCAST ? "BROADCAST " : "",
				_netif->flags & NETIF_FLAG_POINTTOPOINT ? "PPP " : "",
				_netif->flags & NETIF_FLAG_DHCP ? "DHCP " : "",
				_netif->flags & NETIF_FLAG_LINK_UP ? "LINK-UP " : "",
				_netif->flags & NETIF_FLAG_ETHARP ? "ETHARP " : "",
				_netif->flags & NETIF_FLAG_IGMP ? "IGMP " : "",
				_netif->mtu,
				_netif == netif_default ? "DEFIF" : "");
		}

		vTaskDelay(10);

		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_IFCONFIG, 0) == DEF_FAIL)
			vTaskDelay(1); // Time update request to Cop

		vTaskDelay(50);
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_dhcp(INT32S argc, char *argv[])
{
#if defined(__ETH__) ||defined(__WIFI__)
	if (argc != 2 && argc != 4) {
		printf("Error : ex) dhcp on                           // DHCP client on\r\n");
		printf("        ex) dhcp off                          // DHCP client off\r\n");
		printf("        ex) dhcp go [interface_idx] [key_idx] // DHCP user control\r\n");
		return 0;
	}

	if (strcmp("go", argv[1]) == 0) {
		enx_ipchange_proc(atoi(argv[2]), atoi(argv[3]));
	} else if (strcmp("on", argv[1]) == 0) {
#if defined(__ETH__)
		if (gtUser.bUseDhcp != DHCP_USE_ON) {
			gtUser.bUseDhcp = DHCP_USE_ON;
			printf("Ethernet DHCP ON\r\n");
			enx_ipchange_proc(enlETHERNET, UIN_NW_INT_DHCP);
		} else {
			printf("Error : Already Ethernet DHCP ON\r\n");
		}
#endif
#if defined(__WIFI__)
		if (gtWificfg.STAcfg.un8Connect == WIFI_ON) {
			if (gtWificfg.STAcfg.un8DHCPSwitch != WIFI_ON) {
				gtWificfg.STAcfg.un8DHCPSwitch = WIFI_ON;
				printf("Wi-Fi(STA) DHCP ON\r\n");
				enx_ipchange_proc(enlWIFISTA, UIN_NW_INT_DHCP);
			} else {
				printf("Error : Already Wi-Fi(STA) DHCP ON\r\n");
			}
		} else {
			printf("Error : Wi-Fi(STA) is not connected(1).\r\n");
		}
#endif
	} else if (strcmp("off", argv[1]) == 0) {
#if defined(__ETH__)
		if (gtUser.bUseDhcp != DHCP_USE_OFF) {
			gtUser.bUseDhcp = DHCP_USE_OFF;
			printf("Ethernet DHCP OFF\r\n");
			enx_ipchange_proc(enlETHERNET, UIN_NW_INT_DHCP);
		} else {
			printf("Error : Already Ethernet DHCP OFF\r\n");
		}
#endif
#if defined(__WIFI__)
		if (gtWificfg.STAcfg.un8Connect == WIFI_ON) {
			if (gtWificfg.STAcfg.un8DHCPSwitch != WIFI_OFF) {
				gtWificfg.STAcfg.un8DHCPSwitch = WIFI_OFF;
				printf("Wi-Fi(STA) DHCP OFF\r\n");
				enx_ipchange_proc(enlWIFISTA, UIN_NW_INT_DHCP);
			} else {
				printf("Error : Already Wi-Fi(STA) DHCP OFF\r\n");
			}
		} else {
			printf("Error : Wi-Fi(STA) is not connected(2).\r\n");
		}
#endif
	} else {
		printf("Error : option 'on' or 'off'\r\n");
	}
#else
	printf("The network function is disabled.\r\n");
	UNUSED(argc);
	UNUSED(argv);
#endif
	return 0;
}

#if (ENX_SNTPC_use==1)
INT16S cmd_sntp(INT32S argc, char *argv[])
{
#if defined(__ETH__) ||defined(__WIFI__)
	if (argc < 2 || argc > 3) {
		printf("Error : ex) sntp on      // SNTP client repeat on\r\n");
		printf("        ex) sntp re 3600 // SNTP client repeat time set(3600sec)\r\n");
		printf("        ex) sntp off     // SNTP client repeat off\r\n");
		return 0;
	}

	if (strcmp("on", argv[1]) == 0) {
		if (gtUser.bUseSntp != DEF_ON) {
			gtUser.bUseSntp = DEF_ON;
			sntpc_start();
			printf("SNTP ON\r\n");
		} else {
			printf("Error : Already SNTP ON\r\n");
		}
	} else if (strcmp("off", argv[1]) == 0) {
		if (gtUser.bUseSntp != DEF_OFF) {
			gtUser.bUseSntp = DEF_OFF;
			sntpc_stop();
			printf("SNTP OFF\r\n");
		} else {
			printf("Error : Already SNTP OFF\r\n");
		}
	} else if(argc == 3 && (strcmp("re", argv[1]) == 0)) {
		gtUser.nSntpPeriodic = atoi(argv[2]);
		if(gtUser.nSntpPeriodic == 0) { // 0 입력시 기본값으로
			gtUser.nSntpPeriodic = SNTP_PERIODIC;
		}
		printf("SNTP Preiodic time %d\r\n", gtUser.nSntpPeriodic);
	} else {
		printf("Error : option 'on' or 'off'\r\n");
	}
#else
	printf("The network function is disabled.\r\n");
	UNUSED(argc);
	UNUSED(argv);
#endif
	return 0;
}
#endif

INT16S cmd_netstat(INT32S argc, char *argv[])
{
	struct tcp_pcb ** const sh_tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
													&tcp_active_pcbs, &tcp_tw_pcbs};
	int i;
	char strBuffer[2][24];
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;
	printf("-------------------------CPU%d netstat------------------------------\r\n", CpuIdGet());
	printf("Protocol  Local                  Remote                 State\r\n");
	printf("-------------------------------------------------------------------\r\n");
	for (i = 0; i < 4; i++) {
		for (tpcb = *sh_tcp_pcb_lists[i]; tpcb != NULL; tpcb = tpcb->next) {
			snprintf(strBuffer[0], 24, "%IP:%d", tpcb->local_ip.addr, tpcb->local_port);
			if (i == 0) { // LISTEN
				snprintf(strBuffer[1], 24, "%IP:*", tpcb->remote_ip.addr);
			} else { // BOUND, ACTIVE, TW
				snprintf(strBuffer[1], 24, "%IP:%d", tpcb->remote_ip.addr, tpcb->remote_port);
			}
			printf("TCP       %-21s  %-21s  %s\r\n", strBuffer[0], strBuffer[1], tcp_debug_state_str(tpcb->state));
		}
	}
	for (upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) {
		snprintf(strBuffer[0], 24, "%IP:%d", upcb->local_ip.addr, upcb->local_port);
		snprintf(strBuffer[1], 24, "%IP:%d", upcb->remote_ip.addr, upcb->remote_port);
		printf("UDP       %-21s  %-21s\r\n", strBuffer[0], strBuffer[1]);
	}
	printf("-------------------------------------------------------------------\r\n");

	vTaskDelay(10);

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_NETSTAT, 0) == DEF_FAIL)
		vTaskDelay(1); // Time update request to Cop

	vTaskDelay(50);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#if (ENX_FTPD_use==1)
INT16S UsrFtpdCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("Error : ex) ftpd on   // FTP Server on\r\n");
		printf("        ex) ftpd off  // FTP Server off\r\n");
		return 0;
	}

	if (strcmp("on", argv[1]) == 0) {
		if (gtUser.bFtpServer == FTPD_USE_OFF) {
			printf("FTP ON\r\n");
			FtpdStart();
		} else {
			printf("Error : Already FTP Server ON\r\n");
		}
	} else if (strcmp("off", argv[1]) == 0) {
		if (gtUser.bFtpServer == FTPD_USE_ON) {
			printf("FTP OFF\r\n");
			FtpdStop();
		} else {
			printf("Error : Already FTP Server OFF\r\n");
		}
	} else if (strcmp("stat", argv[1]) == 0) {
		int i;
		for (i = 0; i < FTPDATA_info_MAX; i++) {
			printf("idx(%d) state(%d) port(%d) cmd(%d)\r\n", i, ftpd_mb.ftp_data[i].state, ftpd_mb.ftp_data[i].port, ftpd_mb.ftp_data[i].cmd);
		}
	} else {
		printf("Error : option 'on' or 'off'\r\n");
	}
	return 0;
}
#endif

#ifdef __WIFI__
#include "wifi.h"
extern void WifiCFG_Default_AP(tWifiAPcfg *ap);
extern void WifiCFG_Default_STA(tWifiSTAcfg *sta);
INT16S cmd_wifistatus(INT32S argc, char *argv[])
{
	char *statelist[] = {"WLIF_UAP_START", "WLIF_STA_START", "WLIF_UAP_ACTIVE", 
		"WLIF_STA_ACTIVE", "WLIF_STA_CONNECT_WAIT", "WLIF_CONNECTED", "WLIF_DIS_CONNECTED", 
		"WLIF_SCAN"};
	char *authlist[] = {"OPEN", "WPA_PSK", "WPA2_PSK", "WPAnWPA2_PSK"};

	if(strcmp(argv[1], "show") == 0)
	{
		printf("WIFI AP mode info=========================\r\n");
		printf("un32State : %s\r\n", gtWificfg.APcfg.un32State == WLIF_NONE ? "WLIF_NONE" : 
			gtWificfg.APcfg.un32State == WLIF_STA_WPA_AUTH_FAIL ? "WLIF_STA_WPA_AUTH_FAIL" : 
			gtWificfg.APcfg.un32State == WLIF_STA_NO_AP ? "WLIF_STA_NO_AP" : 
			gtWificfg.APcfg.un32State == WLIF_INIT_FAIL ? "WLIF_INIT_FAIL" : statelist[gtWificfg.APcfg.un32State]);
		printf("un8Switch : %s\r\n", gtWificfg.APcfg.un8Switch == WIFI_ON ? "ON" : "OFF");
		printf("strSSID : %s\r\n", gtWificfg.APcfg.strSSID);
		printf("un8Channel : %d\r\n", gtWificfg.APcfg.un8Channel);
		printf("un32Authentication : %s\r\n", authlist[gtWificfg.APcfg.un32Authentication]);
		printf("strKey : %s\r\n", gtWificfg.APcfg.strKey);
		printf("un8MaxSTA : %d\r\n", gtWificfg.APcfg.un8MaxSTA);
		printf("iptLocalip : %IP\r\n", gtWificfg.APcfg.iptLocalip.addr);
		printf("iptNetmask : %IP\r\n", gtWificfg.APcfg.iptNetmask.addr);
		printf("un8DHCPServer : %s\n\r\n", gtWificfg.APcfg.un8DHCPServer == WIFI_ON ? "ON" : "OFF");

		printf("WIFI STA mode info========================\r\n");
		printf("un32State : %s\r\n", gtWificfg.STAcfg.un32State == WLIF_NONE ? "WLIF_NONE" : 
			gtWificfg.STAcfg.un32State == WLIF_STA_WPA_AUTH_FAIL ? "WLIF_STA_WPA_AUTH_FAIL" : 
			gtWificfg.STAcfg.un32State == WLIF_STA_NO_AP ? "WLIF_STA_NO_AP" : 
			gtWificfg.STAcfg.un32State == WLIF_INIT_FAIL ? "WLIF_INIT_FAIL" : statelist[gtWificfg.STAcfg.un32State]);
		printf("un8Switch : %s\r\n", gtWificfg.STAcfg.un8Switch == WIFI_ON ? "ON" : "OFF");
		printf("un8Connect : %s\r\n", gtWificfg.STAcfg.un8Connect == WIFI_ON ? "ON" : "OFF");
		printf("strSSID : %s\r\n", gtWificfg.STAcfg.strSSID);
		printf("strKey : %s\r\n", gtWificfg.STAcfg.strKey);
		printf("un32Authentication : %s\r\n", authlist[gtWificfg.STAcfg.un32Authentication]);
		printf("un8DHCPSwitch : %s\r\n", gtWificfg.STAcfg.un8DHCPSwitch == WIFI_ON ? "ON" : "OFF");
		printf("iptLocalip : %IP\r\n", gtWificfg.STAcfg.iptLocalip.addr);
		printf("iptNetmask : %IP\r\n", gtWificfg.STAcfg.iptNetmask.addr);
		printf("iptGateway : %IP\r\n", gtWificfg.STAcfg.iptGateway.addr);
		printf("iptDNS0 : %IP\r\n", gtWificfg.STAcfg.iptDNS0.addr);
		printf("iptDNS1 : %IP\r\n", gtWificfg.STAcfg.iptDNS1.addr);
	}
	else if (strcmp(argv[1], "init") == 0) {
		WifiCFG_Default_AP(&gtWificfg.APcfg);
		WifiCFG_Default_STA(&gtWificfg.STAcfg);
		if (strcmp(argv[2], "save") == 0) {
			WifiCFG_Save();
		}
	}
	else if(strcmp(argv[1], "apshow") == 0)
	{
		printf("AP extended config show\r\n");
		vTaskDelay(1);
		cpu2cop_wlif_execmd("apshow");
		vTaskDelay(100);
	}
	else if(strcmp(argv[1], "apexset") == 0)
	{
		if (argc == 4) {
			char cmdbuffer[128] = {0};
			sprintf(cmdbuffer, "apexset %s %s", argv[2], argv[3]);
			printf("AP extended config set(%s)\r\n", cmdbuffer);
			vTaskDelay(1);
			cpu2cop_wlif_execmd(cmdbuffer);
			vTaskDelay(100);
		} else {
			Shell_Unknown();
		}
	}
	else if(strcmp(argv[1], "load") == 0)
	{
		WifiCFG_Load();
		gtWificfg.APcfg.un32State = WLIF_NONE;
		gtWificfg.STAcfg.un32State = WLIF_NONE;
		if(strcmp(argv[2], "save") == 0)
		{
			WifiCFG_Save();
		}
	}
	else if(strcmp(argv[1], "save") == 0)
	{
		WifiCFG_Save();
	}
	else
	{
		if(gtWificfg.STAcfg.un8Switch == WIFI_ON)
			cpu2cop_wlif_execmd("wifistatus");
		else
			printf("STA mode : OFF\r\n");
	}
	return 0;

	UNUSED(argc);
}

INT16S cmd_wifionoff(INT32S argc, char *argv[])
{
	char buff[80] = {0};
	if(strcmp(argv[1], "?") == 0)
	{
		printf("AP mode Command\r\n");
		printf("apon : WiFi ap-mode Start\r\n");
		printf("apoff : WiFi ap-mode Stop, State Save and Reboot\r\n");
		printf("apset : Wi-Fi AP mode State Set\r\n");
		printf("apbsson : Wi-Fi AP mode BSS ON\r\n");
		printf("apbssoff : Wi-Fi AP mode BSS OFF\r\n");
		printf("apreset : Wi-Fi AP mode Reset\r\n");
		printf("\r\n");
		printf("STA mode Command\r\n");
		printf("staon : Wi-Fi sta-mode Start\r\n");
		printf("staoff : Wi-Fi sta-mode Disconnect\r\n");
		printf("conn [open/wpa] [SSID] [KEY] : Wi-Fi STA mode Connect\r\n");
		printf("scan : Wi-Fi AP scan\r\n");
		printf("\r\n");
	}
	else if (strcmp(argv[1], "staon") == 0) {
		if (gtWificfg.STAcfg.un32State == WLIF_NONE) {
			gtWificfg.STAcfg.un32State = WLIF_STA_START;
			gtWificfg.STAcfg.un8Switch = WIFI_ON;
			cpu2cop_wlif_execmd("stastart");
		} else {
			printf("already staon\r\n");
		}
	} else if (strcmp(argv[1], "staoff") == 0) {
		cpu2cop_wlif_execmd("disconnect");
	} else if (strcmp(argv[1], "apon") == 0) {
		if (gtWificfg.APcfg.un32State == WLIF_NONE) {
			gtWificfg.APcfg.un8Switch = WIFI_ON;
			xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_START, NULL, enlWIFIUAP);
		} else {
			printf("already apon\r\n");
		}
	} else if (strcmp(argv[1], "apoff") == 0) {
		if(gtWificfg.APcfg.un32State != WLIF_NONE)
		{
			if(gtWificfg.APcfg.un8Switch == WIFI_ON)
			{
				gtWificfg.APcfg.un8Switch = WIFI_OFF;
				WifiCFG_Save();
				printf("reboot~!\r\n");
//				while(1);
//				WdtSystemReset();
			}
			else
			{
				printf("already apoff\r\n");
			}
		}
		else
		{
			printf("already apoff\r\n");
		}
	}
	else if(strcmp(argv[1], "apset") == 0)
	{
		sprintf(buff, "apset %s", WIFI_UAP_SET_CMD_SET);
		cpu2cop_wlif_execmd(buff);
	}
	else if(strcmp(argv[1], "apget") == 0)
	{
		wlif_ewlap_cfg *pwlif_getap = (wlif_ewlap_cfg *)gptMsgWlifCmd.cmd;
		gptMsgWlifCmd.resp = 0; // ask cpu1 to reply for cpu0
		cpu2cop_wlif_execmd("apget");
		while(gptMsgWlifCmd.resp != 1) vTaskDelay(1);
		pwlif_getap = (wlif_ewlap_cfg *)gptMsgWlifCmd.cmd;
		if(!pwlif_getap->flags)
		{
			printf("get fail AP info\r\n");
		}
		else
		{
			printf("SSID : [%s]\r\n", pwlif_getap->ssid);
			printf("KEY : [%s]\r\n", pwlif_getap->passphrase);
			printf("Channel : [%d]\r\n", pwlif_getap->channel);
			printf("MaxSTA : [%d]\r\n", pwlif_getap->max_sta_count);
			printf("Authentication : [%d]\r\n", pwlif_getap->enc_proto);
		}
	}
	else if(strcmp(argv[1], "apbsson") == 0)
	{
		sprintf(buff, "apset %s", WIFI_UAP_SET_CMD_BSSON);
		cpu2cop_wlif_execmd(buff);
	}
	else if(strcmp(argv[1], "apbssoff") == 0)
	{
		sprintf(buff, "apset %s", WIFI_UAP_SET_CMD_BSSOFF);
		cpu2cop_wlif_execmd(buff);
	}
	else if(strcmp(argv[1], "apreset") == 0)
	{
		sprintf(buff, "apset %s", WIFI_UAP_SET_CMD_BSSRESET);
		cpu2cop_wlif_execmd(buff);
	}
	else if(strcmp(argv[1], "conn") == 0)
	{
		if(strcmp(argv[2], "open") == 0 || strcmp(argv[2], "wpa") == 0)
		{
			if(argc == 4)
				sprintf(buff, "connect %s %s", argv[2], argv[3]);
			else if(argc == 5)
				sprintf(buff, "connect %s %s %s", argv[2], argv[3], argv[4]);
			gtWificfg.STAcfg.un32State = WLIF_STA_ACTIVE;
			gtWificfg.STAcfg.un8Connect = WIFI_ON;
			cpu2cop_wlif_execmd(buff);
		}
		else
		{
			printf("input authentication error(open/wpa) => input(%s)\r\n", argv[2]);
		}
	}
	else if(strcmp(argv[1], "scan") == 0)
	{
		// STA mode에서 주변 AP검색 시, EN673 AP를 OFF
		if (gtWificfg.APcfg.un8Switch == WIFI_ON) {
			if (gtWificfg.STAcfg.un8Connect == WIFI_OFF) {
				cpu2cop_wlif_execmd("apset "WIFI_UAP_SET_CMD_BSSOFF);
			}
		}
		timer_sched_timeout_cb(100, TIMEOUT_ONESHOT, cpu2cop_wlif_scan, NULL);
	}
#if 1
	else if(strcmp(argv[1], "reconn") == 0)
	{	// STA mode(re-connect)
		gtWificfg.STAcfg.un32State = WLIF_STA_ACTIVE;
		gtWificfg.STAcfg.un8Connect = WIFI_ON;
		printf("re-connect SSID(%s) KEY(%s)\r\n", gtWificfg.STAcfg.strSSID, gtWificfg.STAcfg.strKey);
		xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_CONNECT_WAIT, NULL, enlWIFISTA);
		vTaskDelay(100);
	}
#endif
	else
	{
		printf("%s %s command not support.\r\n", argv[0], argv[1]);
	}

	return 0;
}
#endif

#ifdef __ETH__
INT16S cmd_mac(INT32S argc, char *argv[])
{
	if (argc == 1) {
		printf("NetIF  MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n", gtMyNetIf.hwaddr[0], gtMyNetIf.hwaddr[1], gtMyNetIf.hwaddr[2], gtMyNetIf.hwaddr[3], gtMyNetIf.hwaddr[4], gtMyNetIf.hwaddr[5]);
		printf("gtSys  MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n", gtSystem.nMacAddress[0], gtSystem.nMacAddress[1], gtSystem.nMacAddress[2], gtSystem.nMacAddress[3], gtSystem.nMacAddress[4], gtSystem.nMacAddress[5]);
		printf("MShare MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n", gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);
	} else {
		const char *strHelp =	"get ex : mac                       : MacAddress Get\r\n"\
								"set ex : mac set ab cd ef 12 34 56 : MacAddress Set & Reboot\r\n";
		if (strcmp(argv[1], "set") == 0) {
			if (argc == 8) {
				gtSystem.nMacAddress[0] = strtol(argv[2], NULL, 16);
				gtSystem.nMacAddress[1] = strtol(argv[3], NULL, 16);
				gtSystem.nMacAddress[2] = strtol(argv[4], NULL, 16);
				gtSystem.nMacAddress[3] = strtol(argv[5], NULL, 16);
				gtSystem.nMacAddress[4] = strtol(argv[6], NULL, 16);
				gtSystem.nMacAddress[5] = strtol(argv[7], NULL, 16);
				UserSave(sfls_SYSTEM);

				printf("MacAddres Change / Reboot yourself.\r\n");
				while(1);
			} else {
				Shell_Unknown();
			}
		} else if(strcmp(argv[1], "?") == 0 || strcmp(argv[1], "help") == 0) {
			printf(strHelp);
		} else {
			printf("mac Cmd Input Error\r\n");
			printf(strHelp);
		}
	}
	return 0;
}

#if 0
INT16S cmd_ethctl(INT32S argc, char *argv[])
{
	static UINT my_PHY_ADDR = PHY_ADDR;

	if(argc == 1)
	{
		printf("REG\r\n");
		printf("ETH_COL_TIME  : 0x%02X(%d)\r\n", ETH_COL_TIME, ETH_COL_TIME);
		printf("ETH_SLOT_TIME : 0x%02X(%d)\r\n", ETH_SLOT_TIME, ETH_SLOT_TIME);
		printf("ETH_IFG_TIME  : 0x%02X(%d)\r\n", ETH_IFG_TIME, ETH_IFG_TIME);
		printf("ETH_LOOP_BACK : 0x%02X(%d)\r\n", ETH_LOOP_BACK, ETH_LOOP_BACK);
		printf("ETH_RTRY_CONT : 0x%02X(%d)\r\n", ETH_RTRY_CONT, ETH_RTRY_CONT);
		printf("ETH_PAUS_EN   : 0x%02X(%d)\r\n", ETH_PAUS_EN, ETH_PAUS_EN);
		printf("ETH_EI_TX     : 0x%02X(%d)\r\n", ETH_EI_TX, ETH_EI_TX);
		printf("ETH_EI_RX     : 0x%02X(%d)\r\n", ETH_EI_RX, ETH_EI_RX);
		printf("ETH_PRO_MOD   : 0x%02X(%d)\r\n", ETH_PRO_MOD, ETH_PRO_MOD);
		printf("ETH_HALF_DUP  : 0x%02X(%d)\r\n", ETH_HALF_DUP, ETH_HALF_DUP);
		printf("ETH_HASH_TBL_H: 0x%08X(%d)\r\n", ETH_HASH_TBL_H, ETH_HASH_TBL_H);
		printf("ETH_HASH_TBL_L: 0x%08X(%d)\r\n", ETH_HASH_TBL_L, ETH_HASH_TBL_L);
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1], "get") == 0)
		{
			BYTE bAddr = atoi(argv[2]);
			WORD wData = PhyGet(my_PHY_ADDR, bAddr);
			printf("Get : Phy Addr(0x%X) Data(%04X)\r\n", bAddr, wData);
		}
		else if(strcmp(argv[1], "set") == 0)
		{
			BYTE bAddr = atoi(argv[2]);
			WORD wData = atoi(argv[3]);
			PhySet(my_PHY_ADDR, bAddr, wData);
			WORD wRdata = PhyGet(my_PHY_ADDR, bAddr);
			if(wData == wRdata)
			{
				printf("Set : Phy Addr(0x%X) Data(%04X)\r\n", bAddr, wData);
			}
			else
			{
				printf("Set Fail : Phy Addr(0x%X) Data(%04X) RData(%04X)\r\n", bAddr, wData, wRdata);
			}
		}
		else if(strcmp(argv[1], "physet") == 0)
		{
			my_PHY_ADDR = atoi(argv[2]);
			printf("Set : Phy Addr(0x%X)\r\n", my_PHY_ADDR);
		}
		else
		{
			printf("%s(%d) : not use\r\n", __func__, __LINE__);
		}
	}
	else if(argc == 4)
	{
		if(strcmp(argv[1], "set") == 0)
		{
			BYTE bAddr = atoi(argv[2]);
			WORD wData = atoi(argv[3]);
			PhySet(my_PHY_ADDR, bAddr, wData);
			WORD wRdata = PhyGet(my_PHY_ADDR, bAddr);
			if(wData == wRdata)
			{
				printf("Set : Phy Addr(0x%X) Data(%04X)\r\n", bAddr, wData);
			}
			else
			{
				printf("Set Fail : Phy Addr(0x%X) Data(%04X) RData(%04X)\r\n", bAddr, wData, wRdata);
			}
		}
		else if(strcmp(argv[1], "es") == 0)
		{
			if(strcmp(argv[2], "hh") == 0)
			{
				ETH_HASH_TBL_H = atoi(argv[3]);
				printf("Set : ETH_HASH_TBL_H: 0x%08X(%d)\r\n", ETH_HASH_TBL_H, ETH_HASH_TBL_H);
			}
			else if(strcmp(argv[2], "hl") == 0)
			{
				ETH_HASH_TBL_L = atoi(argv[3]);
				printf("Set : ETH_HASH_TBL_L: 0x%08X(%d)\r\n", ETH_HASH_TBL_L, ETH_HASH_TBL_L);
			}
			else
			{
				printf("%s(%d) : not use\r\n", __func__, __LINE__);
			}
		}
		else
		{
			printf("%s(%d) : not use\r\n", __func__, __LINE__);
		}
	}
	else if(argc == 2)
	{
		if(strcmp(argv[1], "phyget") == 0)
		{
			printf("Get : Phy Addr(0x%X)\r\n", my_PHY_ADDR);
		}
		else
		{
			printf("%s(%d) : not use\r\n", __func__, __LINE__);
		}
	}
	else
	{
		printf("%s(%d) : not use\r\n", __func__, __LINE__);
	}

	return 0;
}
#endif
#endif

INT16S cmd_nettx_test(INT32S argc, char *argv[])
{
	static UINT send_size = 100;
	static struct udp_pcb *test_pcb = NULL;

	if(argc == 3 && strcmp(argv[1], "setsize") == 0)
	{
		UINT new_size = atoi(argv[2]);
		if(new_size > 1400)
		{
			printf("input new send size %d -> max 1400\r\n", new_size);
			new_size = 1400;
		}
		printf("Old send size(%d)\nNew send size(%d)\r\n", send_size, new_size);
		send_size = new_size;
	}
	if(argc == 2 && strcmp(argv[1], "getsize") == 0)
	{
		printf("Now send size(%d)\r\n", send_size);
	}
	if(argc == 1)
	{
		u16_t test_port = 9999;

		if(test_pcb == NULL)
		{
			test_pcb = udp_new();
			if(test_pcb == NULL)
			{
				printf("udp pcb create fail!\r\n");
				return 0;
			}

//			ip_set_option(test_pcb, SOF_BROADCAST);

			udp_bind(test_pcb, IP_ADDR_ANY, test_port);
			
			ipaddr_aton("239.255.255.250", &(test_pcb->remote_ip));
			test_pcb->remote_port = test_port;

		}

		struct pbuf *test_pbuf = pbuf_alloc(PBUF_TRANSPORT, send_size, PBUF_RAM);
		if(test_pbuf != NULL)
		{
			err_t ert = ERR_IF;
			UINT i;
			BYTE *raw_data = test_pbuf->payload;
			for(i=0;i<send_size;i++)
				raw_data[i] = i % 256;

#ifdef __ETH__
			ert = udp_sendto_if(test_pcb, test_pbuf, &test_pcb->remote_ip, test_pcb->remote_port, &gtMyNetIf);
#endif
#ifdef __WIFI__
			ert = udp_sendto_if(test_pcb, test_pbuf, &test_pcb->remote_ip, test_pcb->remote_port, &gtwlif_uap);
#endif
			vTaskDelay(5);
			if(ert == ERR_OK)
				printf("IP:%08X PORT:%d SIZE:%d Send OK!\r\n", test_pcb->remote_ip.addr, test_pcb->remote_port, send_size);
			else
				printf("IP:%08X PORT:%d SIZE:%d Send Fail(%d)!\r\n", test_pcb->remote_ip.addr, test_pcb->remote_port, send_size, ert);

			pbuf_free(test_pbuf);
		}
		else
		{
			printf("pbuf alloc fail\r\n");
		}
	}

	return 0;
}

INT16S cmd_rtsp_state(INT32S argc, char *argv[])
{
	if (argc == 1) {
		int i;
		struct tm tmout;
		printf("Connect user : %d\r\n", gptMsgShare.rtsp_conn_cnt);
		for (i = 0; i < RTSP_info_MAX; i++) {
			if (gptMsgShare.rtsp_conn_info[i].connect_time != 0xFFFFFFFF) {
				enx_get_tmtime(gptMsgShare.rtsp_conn_info[i].connect_time, &tmout, DEF_YES);
				printf("User%d Local(%IP:%u) Remote(%IP:%u) Type(%d/%s)\r\n", i, 
					gptMsgShare.rtsp_conn_info[i].server_addr, gptMsgShare.rtsp_conn_info[i].server_port,
					gptMsgShare.rtsp_conn_info[i].client_addr, gptMsgShare.rtsp_conn_info[i].client_port,
					gptMsgShare.rtsp_conn_info[i].rtp_type, gptMsgShare.rtsp_conn_info[i].rtp_type == 0 ? "UDP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 1 ? "TCP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 2 ? "HTTP" : "Unknown");
				printf("      Connect Time(%04d-%02d-%02d %02d:%02d:%02d) Video(%d/%s)\r\n", 
					tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec,
					gptMsgShare.rtsp_conn_info[i].video_type, gptMsgShare.rtsp_conn_info[i].video_type == 0 ? "H.264 1st" : gptMsgShare.rtsp_conn_info[i].video_type == 1 ? "H.264 2nd" : gptMsgShare.rtsp_conn_info[i].video_type == 9 ? "Motion JPEG" : "Unknown");
			}
		}
	} else if (argc == 3 && strcmp(argv[1], "kick") == 0) {
		int nUserIdx = atoi(argv[2]);
		if (nUserIdx < RTSP_info_MAX) {
			printf("kick user(%d)\r\n", nUserIdx);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_STREAM_KICK, nUserIdx) == DEF_FAIL)
				vTaskDelay(1); // Time update request to Cop
			vTaskDelay(50);
		} else {
			printf("input user idx(%d) error\r\n", nUserIdx);
		}
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
