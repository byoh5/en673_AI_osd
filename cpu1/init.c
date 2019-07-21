//*************************************************************************************************
//
//*************************************************************************************************
#include "dev.h"
#include "dev_inline.h"	// to use fast code
#ifdef __ISP__
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#endif

#include <stdlib.h>

#include "init.h"

#include "msg.h"
#include "phy.h"
#include "user.h"
#include "file.h"
#include "net_ascii.h"

#include "ff.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/memp.h"
#include "lwip/stats.h"

#include "networkif.h"
#include "ethernetif.h"

#include "timer_task.h"
#include "shell.h"
#include "upnpd.h"
#include "httpd.h"
#include "ipchange.h"
#include "videochange.h"
#include "isp_task.h"
#include "term_server.h"

#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
#include "ftp_server.h"
#endif
#ifdef __DEVICE_SD__
#include "sdcard.h"
#include "muxer_avienc.h"		// muxer_init_buffering()
#endif
#endif

#ifdef __ETH__
#include "iputil.h"
#endif

#ifdef __WIFI__
#include "wifi_task.h"
#endif

#ifdef __RTC_LOAD__
#include <time.h>
#include "rtc.h"
#ifdef __RTC_S35390A__
#include "s35390a.h"
#endif
#ifdef __RTC_PCF8563__
#include "pcf8563.h"
#endif
#endif

#ifdef __AUDIO__
#ifdef __AUD_TLV320AIC3100__
#include "tlv320aic3100.h"
#endif
#ifdef __AUD_ES8374__
#include "es8374.h"
#endif
#endif

#ifdef __ONVIF__
#include "onvifSystemInfo.h"

void discovery_task(void *pvParameters);
void hello_discovery(void *pvParameters);
void HttpdOnvifTask(void *pvParameters);
void notify_thr(void *arg);
#endif

#if ENX_NETWORK_Debug
#include "network_debugging.h"
#endif


char sensor_name[32] = "EN673_TEST";

//*************************************************************************************************
// H/W init
//-------------------------------------------------------------------------------------------------
//
#if (IMAGE_INPUT_MODE==2)
#define ISP_IRQ_VLOCKI			0			// VLOCKI, Sensor 입력 Sync 기준
#define ISP_IRQ_VLOCKW			1			// VLOCKW, Sensor 입력 Sync 기준 (WDR 기준 Sync)
#define ISP_IRQ_VLOCKO			2			// VLOCKO, ISP 출력 Sync 기준(Low Shutter 제외하고는 VLOKCI와 VLOCKO가 동일 함.)
#define ISP_IRQ_JPGEND			3
#define ISP_IRQ_USER0			4			// User Interrupt 0 (VLOCKI 기준)
#define ISP_IRQ_USER1			5			// User Interrupt 1 (VLOCKO 기준)
#define ISP_IRQ_VLOCKW2			6			// VLOCKW&O, AND Sync

ISRT void IsrISP(void *dummy)
{
	UINT IsrStatus = ISP_rISP_RIRQ1;

	if(IsrStatus&(0x1<<ISP_IRQ_VLOCKI)) { ISP_CLI_VLOCKI1 = 1; }
	else if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW)) { ISP_CLI_VLOCKW1 = 1; }
	else if(IsrStatus&(0x1<<ISP_IRQ_VLOCKO)) { ISP_CLI_VLOCKO1 = 1; }
	else if(IsrStatus&(0x1<<ISP_IRQ_JPGEND)) { ISP_CLI_JPG = 1; }
	else if(IsrStatus&(0x1<<ISP_IRQ_USER0)) { ISP_IP_CLIUIRQ0 = 1; CPU_SHARED0 |= eIP0_DGT_INPUT; CPU_IRQ0 = 1;}
	else if(IsrStatus&(0x1<<ISP_IRQ_USER1)) { ISP_IP_CLIUIRQ1 = 1; }
	else if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW2)) { ISP_CLI_VLOCKW2_1 = 1; }

	UNUSED(dummy);
}
#endif

void InitHw(void)
{
#ifdef WDT_CPU1
	WdtCpuSetTime(WDT_DEFAULT_TIME);	// CPU1 WDT enable
#endif

	IntInit();				// Interrupt

// CPU0 -> CPU1 IRQ ADD
	IntAdd(IRQ_CPU, (void*)IsrSys1,	0);
	IrqEnable(IRQ_CPU);
	CPU_IRQ1_EN = 1;		// cpu0->cpu1

#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))
	switch (model_Sens) {
		case SENS_MN34210:      strcpy(sensor_name, "MN34210");     break;
		case SENS_IMX225:       strcpy(sensor_name, "IMX225");      break;
		case SENS_MN34229:      strcpy(sensor_name, "MN34229");     break;
		case SENS_MN34420:      strcpy(sensor_name, "MN34420");     break;
		case SENS_IMX291:       strcpy(sensor_name, "IMX291");      break;
		case SENS_IMX327:       strcpy(sensor_name, "IMX327");      break;
		case SENS_IMX307:       strcpy(sensor_name, "IMX307");      break;
		case SENS_AR0237CSH:    strcpy(sensor_name, "AR0237CSH");   break;
		case SENS_MN34227:      strcpy(sensor_name, "MN34227");     break;
		case SENS_OV2718:       strcpy(sensor_name, "OV2718");      break;
//		case SENS_IMX122:       strcpy(sensor_name, "IMX122");      break;
		case SENS_IMX323:       strcpy(sensor_name, "IMX323");      break;
//		case SENS_AR0237CSP:    strcpy(sensor_name, "AR0237CSP");   break;
		default:                                                    break;
	}
#else
	strcpy(sensor_name, "DigitalInput");
#endif

#if (IMAGE_INPUT_MODE==2)

	IntAdd(IRQ_ISP, (void*)IsrISP, 0);
	IrqEnable(IRQ_ISP);

	ISP_DSENS_ON = 1;
	ISP_IP_UIRQ0_CNT_SEL = 2;	// BT656 Input Path로 설정

	ISP_IPUIRQ0_PD = 0;
	ISP_IP_UIRQ0_CK_SEL = 5;	// BT656 Input Clock 으로 User Interrupt Module 설정.
	ISP_IPUIRQ0_PD = 1;

	ISP_IP_UIRQ_HPOS0 = 0;		// User Interrupt H Pos 설정
	ISP_IP_UIRQ_VPOS0 = 0;		// User Interrupt V Pos 설정
	ISP_IP_UIRQ0_EN = 1;		// User Interrupt Enable
#endif

//	IntAdd(IRQ_AUD, (void*)AudIsr, 0);
//	IrqEnable(IRQ_AUD);

#ifdef __FAT_LOAD__
#ifdef __DEVICE_SD__
	muxer_init_buffering();
	printf("Video recording init done\r\n");
#endif
#ifdef __FLS_FAT_LOAD__
	FlsFatInit();							// Fls FAT Init
#endif
#ifdef __MEM_FAT_LOAD__
	MemFatInit();
#endif
#endif

#ifdef __RTC_LOAD__
	RtcInit();
#endif

// Set TimeZone
	setTimeZone(DEF_TIMEZONE);

	printf("H/W init done\r\n");
}

#ifdef __RTC_LOAD__
void RtcInit(void)
{
	rtc_clean();
#ifdef __RTC_S35390A__
	if(s35390a_init() == DEF_FAIL)							// RTC Init
	{	// init fail => rtc func pointer clean
		rtc_clean();
	}
#endif
#ifdef __RTC_PCF8563__
	if(pcf8563_init() == DEF_FAIL)							// RTC Init
	{	// init fail => rtc func pointer clean
		rtc_clean();
	}
#endif
}
#endif

#ifdef __FLS_FAT_LOAD__
void FlsFatInit(void)
{
	FRESULT res = drive_in(FLS);
	switch(res)
	{
		case FR_OK:
		case FR_NO_FILESYSTEM:
			drive_init(FLS);
			break;
		default:
			break;
	}
}
#endif

#ifdef __MEM_FAT_LOAD__
void MemFatInit(void)
{
	FRESULT res = drive_in(MEM);
	switch(res)
	{
		case FR_OK:
		case FR_NO_FILESYSTEM:
			drive_init(MEM);
			break;
		default:
			break;
	}
}
#endif

//*************************************************************************************************
// User init
//-------------------------------------------------------------------------------------------------
//
void LoadDefUserInfo(void)
{
	gtUser.nMagicNum = MAGICNUM_USER;
	gtUser.nDateID = gptMsgShare.CPU1_DATEID;

	gtUser.nSystemTime = 0;
#if (ENX_SNTPC_use==1)
	gtUser.bUseSntp = SNTP_START;
	strcpy((char *)gtUser.strSntpSvr, SNTP_SRVR);
	gtUser.nSntpPeriodic = SNTP_PERIODIC;
	gtUser.nSntpRetrySec = SNTP_RETRY_SEC;
	gtUser.nSntpRetryMaxcnt = SNTP_RETRY_CNT;
#endif
	gtUser.nTimeSummer = DEF_OFF;
	gtUser.nTimeZone = DEF_TIMEZONE;
	gtUser.onvif_nTimeZone = 107;

	strcpy((char *)gtUser.strDeviceId, DEVICE_ID);
	strcpy((char *)gtUser.strDeviceName, CAMERA_NAME);

	gtUser.bUseDhcp = NET_DHCP; 					// Dhcp on/off
	if (gtUser.bUseDhcp == DHCP_USE_ON) {
		gtUser.nIpAddr	= 0;
		gtUser.nNetMask = 0;
		gtUser.nGateway = 0;
		gtUser.nDnsSvr0 = 0;
		gtUser.nDnsSvr1 = 0;
	} else {
		gtUser.nIpAddr	= ipaddr_addr(IP_ADDRESS);	// IP addr
		gtUser.nNetMask = ipaddr_addr(NET_MASK);	// Net mask addr
		gtUser.nGateway = ipaddr_addr(GW_ADDRESS);	// Gateway addr
		gtUser.nDnsSvr0 = ipaddr_addr(DNS_SRVR0);
		gtUser.nDnsSvr1 = ipaddr_addr(DNS_SRVR1);
	}

	strcpy((char *)gtUser.strUserId, USER_ID);
	strcpy((char *)gtUser.strUserPw, USER_PW);

#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
	gtUser.bFtpServer = FAT_FTPSERVER;
#endif
#ifdef __DEVICE_SD__
	gtUser.bSdAviSave = FAT_SDSAVE;
#endif
#endif
#ifdef __AUDIO__
	gtUser.bAudio = AUDIO_OUT;
#endif
	gtUser.nHttpPort = HTTP_PORT;

	gtUser.nRtspPort = 554; //RTSP_DEF_PORT;		// rtsp_control.h에 선언

	gtUser.uviH264[0].eProfileMode = IMG_H264_PROFILE;
	gtUser.uviH264[0].eResolution = IMG_H264_RES;
	gtUser.uviH264[0].nQuality = IMG_H264_QULITY;
	gtUser.uviH264[0].nIDRFrame = IMG_H264_IDR; // H.264에서만 사용
	gtUser.uviH264[0].sfFPS = IMG_H264_FPS;
	gtUser.uviH264[0].eBRMode = IMG_BR_H;
	gtUser.uviH264[0].nBitRate = IMG_BR_HR;

#ifdef __H264_2CH__
	gtUser.uviH264[1].eProfileMode = IMG_H264_2CH_PROFILE;
	gtUser.uviH264[1].eResolution = IMG_H264_2CH_RES;
	gtUser.uviH264[1].nQuality = IMG_H264_2CH_QULITY;
	gtUser.uviH264[1].nIDRFrame = IMG_H264_2CH_IDR; // H.264에서만 사용
	gtUser.uviH264[1].sfFPS = IMG_H264_2CH_FPS;
	gtUser.uviH264[1].eBRMode = IMG_BR_H2;
	gtUser.uviH264[1].nBitRate = IMG_BR_H2R;
#endif

#ifdef __JPEG__
	gtUser.uviJpeg.eResolution = IMG_JPEG_RES;
	gtUser.uviJpeg.nQuality = IMG_JPEG_QULITY;
	gtUser.uviJpeg.sfFPS = IMG_JPEG_FPS;
	gtUser.uviJpeg.eBRMode = IMG_BR_J;
	gtUser.uviJpeg.nBitRate = IMG_BR_JR;
#endif
}

void InitUsr(void)
{
	printf("=============================\r\n");
	printf("Load user info loading\r\n");

	getDateID();

	memset((void *)&gtSystem, 0, sizeof(gtSystem));
	memset((void *)&gtUser, 0, sizeof(gtUser));
	UserLoad(sfls_SYSTEM);
	UserLoad(sfls_USER);

#if USER_DEFAULT
	// none
#else
	if ((gtSystem.nMagicNum != MAGICNUM_SYSTEM) || (gtSystem.nVersion != EN673_VERSION) || (gtSystem.nDateID != gptMsgShare.CPU1_DATEID))
#endif
	{
		printf("%s, Default Config Load(System)\r\n", gtSystem.nDateID != gptMsgShare.CPU1_DATEID ? "New build" : "No system info");

		gtSystem.nMagicNum = MAGICNUM_SYSTEM;					// Magic number (Used if flash is blank or not)
		gtSystem.nVersion = EN673_VERSION;
		gtSystem.nDateID = gptMsgShare.CPU1_DATEID;

		UserSave(sfls_SYSTEM);
	}

#if USER_DEFAULT
	// none
#else
	if ((gtUser.nMagicNum != MAGICNUM_USER) || (gtUser.nDateID != gptMsgShare.CPU1_DATEID))
#endif
	{
		printf("%s, Default Config Load(User)\r\n", gtUser.nDateID != gptMsgShare.CPU1_DATEID ? "New build" : "No user info");

#if (USER_DEFAULT==0)	// IP 유지할 경우
		UINT nIpAddr = 0, nNetMask = 0, nGateway = 0, nDnsSvr0 = 0, nDnsSvr1 = 0, bUseDhcp = 0;
		UINT nIPLoad = 0;

		if (gtUser.nMagicNum != 0xffffffff && gtUser.nMagicNum != 0x00000000 &&
			gtUser.nDateID != 0xffffffff && gtUser.nDateID != 0x00000000) {
			nIpAddr = gtUser.nIpAddr;
			nNetMask = gtUser.nNetMask;
			nGateway = gtUser.nGateway;
			nDnsSvr0 = gtUser.nDnsSvr0;
			nDnsSvr1 = gtUser.nDnsSvr1;
			bUseDhcp = gtUser.bUseDhcp;
			nIPLoad = 1;
		}
#endif

		LoadDefUserInfo();

#if (USER_DEFAULT==0)
		if (nIPLoad == 1) {
			gtUser.nIpAddr = nIpAddr;
			gtUser.nNetMask = nNetMask;
			gtUser.nGateway = nGateway;
			gtUser.nDnsSvr0 = nDnsSvr0;
			gtUser.nDnsSvr1 = nDnsSvr1;
			gtUser.bUseDhcp = bUseDhcp;
		}
#endif

		UserSave(sfls_USER);
	}

#ifdef __ONVIF__
	OnvifCFG_Load();
	if(gSysInfo.nMagicNum != MAGICNUM_ONVIF || gSysInfo.nDateID != gptMsgShare.CPU1_DATEID)
	{
		printf("%s, Default Config Load(ONVIF)\r\n", gSysInfo.nDateID != gptMsgShare.CPU1_DATEID ? "New build" : "No onvif info");
		init_onvif_profiles();
		OnvifCFG_Save();
	}
#endif

#ifdef __ETH__
	gptMsgShare.ETH_MAC[0] = gtSystem.nMacAddress[0];
	gptMsgShare.ETH_MAC[1] = gtSystem.nMacAddress[1];
	gptMsgShare.ETH_MAC[2] = gtSystem.nMacAddress[2];
	gptMsgShare.ETH_MAC[3] = gtSystem.nMacAddress[3];
	gptMsgShare.ETH_MAC[4] = gtSystem.nMacAddress[4];
	gptMsgShare.ETH_MAC[5] = gtSystem.nMacAddress[5];

//	printf("%s(%d) : %02x:%02x:%02x:%02x:%02x:%02x\r\n", __func__, __LINE__,
//		gptMsgShare.ETH_MAC[0],gptMsgShare.ETH_MAC[1],gptMsgShare.ETH_MAC[2],
//		gptMsgShare.ETH_MAC[3],gptMsgShare.ETH_MAC[4],gptMsgShare.ETH_MAC[5]);

#if ETH_MAC_TEST	// Debug Code - Check system MAC address
	if(gptMsgShare.ETH_MAC[0] & 1)
	{
		while(1)
		{
			printf("%s(%d) : Wrong MAC Address - Group Address!\r\n", __func__, __LINE__);
			printf("%s(%d) :                     New Mac Address Input!\r\n", __func__, __LINE__);
#ifdef WDT_SYS
			WdtSysCountReset();
#endif
#ifdef WDT_CPU1
			WdtCpuCountReset(); // CPU1 WDT reset
#endif
			WaitXms(2000);
		}
	}
	else if(gptMsgShare.ETH_MAC[0] == 0 && gptMsgShare.ETH_MAC[1] == 0 && gptMsgShare.ETH_MAC[2] == 0 && 
		gptMsgShare.ETH_MAC[3] == 0 && gptMsgShare.ETH_MAC[4] == 0 && gptMsgShare.ETH_MAC[5] == 0)
	{
		while(1)
		{
			printf("%s(%d) : Wrong MAC Address - Zero Address!\r\n", __func__, __LINE__);
			printf("%s(%d) :                     New Mac Address Input!\r\n", __func__, __LINE__);
#ifdef WDT_SYS
			WdtSysCountReset();
#endif
#ifdef WDT_CPU1
			WdtCpuCountReset(); // CPU1 WDT reset
#endif
			WaitXms(2000);
		}
	}
	else if(gptMsgShare.ETH_MAC[3] == 0xff && gptMsgShare.ETH_MAC[4] == 0xff && gptMsgShare.ETH_MAC[5] == 0xff)
	{
		while(1)
		{
			printf("%s(%d) : Wrong MAC Address - Broadcast Address!\r\n", __func__, __LINE__);
			printf("%s(%d) :                     New Mac Address Input!\r\n", __func__, __LINE__);
#ifdef WDT_SYS
			WdtSysCountReset();
#endif
#ifdef WDT_CPU1
			WdtCpuCountReset(); // CPU1 WDT reset
#endif
			WaitXms(2000);
		}
	}
#endif

	// for Mac Set(cpu0)
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_NET_MAC, 1) == DEF_FAIL)						WaitXus(1);
#endif

#if USER_DEFAULT
#if defined(__ETH__) || defined(__WIFI__)
	unVideoChangeFlag = UIN_STREAM_USER_ID | UIN_STREAM_USER_PW;	// rtsp id/pw send cpu0
#endif
#else
	unVideoChangeFlag = 0xFFFFFFFF;									// all video setting send cpu0
#endif

	sys_thread_new("vchange", VideoChangeTask, NULL, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);

	setTimeZone(gtUser.nTimeZone);
}

//*************************************************************************************************
// S/W init
//-------------------------------------------------------------------------------------------------
#ifdef __RTC_LOAD__
//-------------------------------------------------------------------------------------------------
// we read time of RTC that apply to gptMsgCop2Cpu.TIME.
static void TickTimerInit(void *ctx)
{
	int _rtc_set_time = 0;
	struct tm tmout;

	if (rtc_get_time(&tmout) == DEF_OK) {
		gptMsgShare.TIME = mktime(&tmout);
		gptMsgShare.TIMEZONE = nTimeZoneSec[getTimeZone()][2] + (gtUser.nTimeSummer * 3600);
		gtUser.nSystemTime = gptMsgShare.TIME;

#ifdef ENX_DEBUG
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
		printf("RTC(GMT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
		printf("RTC(LOC) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
#endif

		// RTC로부터 시간을 가져왔으나, 2018년 보다 과거인 경우 시간, RTC 재설정
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
		if (tmout.tm_year+1900 < 2018) {
			_rtc_set_time = 1;
		}
	} else {
		// RTC로부터 시간을 못가져 오면, 2018년, 1월로 설정
		printf("RTC get time fail\r\n");
		_rtc_set_time = 1;
	}

	if (_rtc_set_time) {
		// 기준시간 : GMT 2018-01-01-00-00-00
		int nYear = 2018, nMonth = 1, nDay = 1, nHour = 0, nMin = 0, nSec = 0;
		if (set_devicetime(TimeZone_GMT, nYear, nMonth, nDay, nHour, nMin, nSec) == DEF_FAIL) {
			printf("rtc time set fail\r\n");
		}
		gtUser.nSystemTime = gptMsgShare.TIME;
	}

	vTaskDelete(NULL);
	UNUSED(ctx);
}
#endif

#ifdef __AUDIO__
void AudInit(void *ctx)
{
#ifdef __AUD_TLV320AIC3100__
	GpioSetLo(GPIO_AUD_RST);
	vTaskDelay(10);
	GpioSetHi(GPIO_AUD_RST);
	vTaskDelay(10);

	tlv320aic3100_init();
	tlv320aic3100_agc(2);
	tlv320aic3100_dac_init();
#endif

#ifdef __AUD_ES8374__
	es8374_init();
#endif

	Aud_Init(1);
	gptMsgShare.AUD_TX_MODE = 0;	// 0: TX off or Outputs test music(AUDIO_ADD_MUSIC==1).
	gptMsgShare.AUD_TX_GO = 1;

	vTaskDelete(NULL);

	UNUSED(ctx);
}
#endif

#if defined(__ETH__) || defined(__WIFI__)
void startNetProtocol(void *arg)
{
	// Wait for network activation.
	network_check_netif();

	srand(gptMsgShare.TIME);

#if (ENX_TERMD_use==1)
	sys_thread_new("Term0R", TermStart, &term[TERM_CPU0], _32KB_STACK_SIZE, JABBU_TASK_PRIO);
	sys_thread_new("Term1R", TermStart, &term[TERM_CPU1], _32KB_STACK_SIZE, JABBU_TASK_PRIO);
#endif

#if (NET_LWIPERF==2)
	lwiperf_init();
#endif

//	FTP Server : port 21
#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
	if(gtUser.bFtpServer == FTPD_USE_ON)
		FtpdStart();
#endif
#endif

//	UPNP Daemon : port 1900, 55555
//	sys_thread_new("Upnpd", upnpd_init, NULL, _16KB_STACK_SIZE, LOW_TASK_PRIO);

#ifdef __ONVIF__
	sys_thread_new("wsddhello",hello_discovery, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
	sys_thread_new("HttpdOnvif", HttpdOnvifTask, NULL, _16KB_STACK_SIZE, HTTPD_TASK_PRIO);
	sys_thread_new("wsdd",discovery_task, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
	sys_thread_new("notify",notify_thr, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
#else
	// HTTP Server Daemon(CGI, SSI) : port 80
	httpd_init();
#endif

#ifdef __ETH__
#if (ENX_IPUTIL_use==1)
	IpUtil_Init();
#endif
#endif

	vTaskDelete(NULL);
	UNUSED(arg);
}
#endif

void InitSw(void)
{
#ifdef __RTC_LOAD__
	sys_thread_new("loadRTC", TickTimerInit, NULL, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);
#endif

#ifdef __AUDIO__
	sys_thread_new("loadAud", AudInit, NULL, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);
#endif

// Task Create Start -------------------------------------------------------------------------------
	timer_init(NULL);
	sys_thread_new("timer", timerTask, NULL, _32KB_STACK_SIZE, TIMER_TASK_PRIO);

	// Debug Shell
	sys_thread_new("Shell", ShellTask, NULL, SHELL_STACK_SIZE, SHELL_TASK_PRIO);

#ifdef __DEVICE_SD__
	sys_thread_new("checkSD", CheckSDstate, NULL, _32KB_STACK_SIZE, JABBU_TASK_PRIO);
	sys_thread_new("avienc", muxer_avienc_task, NULL, MAKEAVI_STACK_SIZE, MAKEAVI_TASK_PRIO);
#endif

#if defined(__ETH__) || defined(__WIFI__)
	sys_thread_new("startLwip", startLwip, NULL, _32KB_STACK_SIZE, ETHRX_TASK_PRIO);

#ifdef __WIFI__
	sys_thread_new("Wifi", wifiTask, NULL, _64KB_STACK_SIZE, WIFI_TASK_PRIO);

	wlif_init_event();
	xwlif_evt_sem = xSemaphoreCreateCounting(10, 0);
#endif

	sys_thread_new("netProt", startNetProtocol, NULL, _16KB_STACK_SIZE, JABBU_TASK_PRIO);
#endif
// Task Create End --------------------------------------------------------------------------------
}
