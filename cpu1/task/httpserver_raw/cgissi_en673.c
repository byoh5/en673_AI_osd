#include <stdio.h>
#include <stdlib.h>																// for atoi
#include <malloc.h>																// for mallinfo

/* ------------------------ EN673 includes -------------------------------- */
#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "factory.h"
#include "develop.h"
#include "user.h"
#include "msg.h"
#include "init.h"																// for LoadDefUserInfo
#ifdef __ISP__
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#endif

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"

/* ------------------------ FAT includes ---------------------------------- */
#include "ff.h"
#include "file.h"

/* ------------------------ Project includes ------------------------------ */
#include "httpd.h"
#include "cgifuncs.h"
#include "cgissi.h"

/* ------------------------ Util includes --------------------------------- */
#include "time.h"
#include "wifi.h"
#include "sntpc.h"

#include "timer_task.h"		// enum { sd_OFF, sd_INIT ... };
#include "muxer_avienc.h"	// enum { ..., eRecSnapshot };
#include "fwupdate.h"

#include "ipchange.h"
#include "videochange.h"

#ifdef __FAT_LOAD__
#ifdef __DEVICE_SD__
#include "sdcard.h"
#endif
#include "ftp_server.h"
#endif

/* ------------------------------------------------------------------------ */
#if LWIP_HTTPD_SSI

extern char sensor_name[];

static void SSI_entry_state(char *pcInsert, int iInsertLen)
{
	char strXmlData[2048] = {0};

	snprintf(strXmlData, iInsertLen, "%s<SYSTEM>%s", CRLF, CRLF);

	// DEVINFO
	snprintf(strXmlData, iInsertLen, "%s<DEVINFO>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<VERSION>%u</VERSION>%s", strXmlData, gtSystem.nVersion, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</DEVINFO>%s", strXmlData, CRLF);

	// USER
	snprintf(strXmlData, iInsertLen, "%s<USER>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<ID>%s</ID>%s", strXmlData, gtUser.strUserId, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<PASSWORD>%s</PASSWORD>%s", strXmlData, gtUser.strUserPw, CRLF);
#if (ENX_FTPD_use==1)
	snprintf(strXmlData, iInsertLen, "%s<FTPSERVER>%u</FTPSERVER>%s", strXmlData, gtUser.bFtpServer, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<FTPSERVER>"XML_NA"</FTPSERVER>"CRLF, strXmlData);
#endif

#ifdef __DEVICE_SD__
	snprintf(strXmlData, iInsertLen, "%s<SDAVISAVE>%u</SDAVISAVE>%s", strXmlData, gtUser.bSdAviSave, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<SDAVISAVE>"XML_NA"</SDAVISAVE>%s", strXmlData, CRLF);
#endif

#ifdef __AUDIO__
	snprintf(strXmlData, iInsertLen, "%s<AUDIO>%u</AUDIO>%s", strXmlData, gtUser.bAudio, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<AUDIO>"XML_NA"</AUDIO>%s", strXmlData, CRLF);
#endif

	snprintf(strXmlData, iInsertLen, "%s</USER>%s", strXmlData, CRLF);

	// DATE
	snprintf(strXmlData, iInsertLen, "%s<DATE>%s", strXmlData, CRLF);
#if (ENX_SNTPC_use==1)
	snprintf(strXmlData, iInsertLen, "%s<SNTPSVR>%s</SNTPSVR>%s", strXmlData, gtUser.strSntpSvr, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s<SYSTIME>%u</SYSTIME>%s", strXmlData, gptMsgShare.TIME, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<UPTIME>%lu</UPTIME>%s", strXmlData, gptMsgShare.UPTIME, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<TIMEZONE>%d</TIMEZONE>%s", strXmlData, gtUser.nTimeZone&0xFF, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<TIMEZONEM>%d</TIMEZONEM>%s", strXmlData, (gtUser.nTimeZone>>8)&&0xFF, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</DATE>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s</SYSTEM>%s", strXmlData, CRLF);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __WIFI__
	snprintf(strXmlData, iInsertLen, "%s<WIFI>%s", strXmlData, CRLF);

	// WiFi-AP gtWificfg.APcfg
	snprintf(strXmlData, iInsertLen, "%s<AP_MODE>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtWificfg.APcfg.iptLocalip.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtWificfg.APcfg.iptNetmask.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtWificfg.APcfg.un8DHCPServer, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</AP_MODE>%s", strXmlData, CRLF);

	// WiFi-STA gtWificfg.STAcfg
	snprintf(strXmlData, iInsertLen, "%s<STA_MODE>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtWificfg.STAcfg.iptLocalip.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtWificfg.STAcfg.iptNetmask.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<GATEWAY>%IP</GATEWAY>%s", strXmlData, gtWificfg.STAcfg.iptGateway.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS0>%IP</DNS0>%s", strXmlData, gtWificfg.STAcfg.iptDNS0.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS1>%IP</DNS1>%s", strXmlData, gtWificfg.STAcfg.iptDNS1.addr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtWificfg.STAcfg.un8DHCPSwitch, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</STA_MODE>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s</WIFI>%s", strXmlData, CRLF);
#endif

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	snprintf(strXmlData, iInsertLen, "%s<NETWORK>%s", strXmlData, CRLF);

	// TCP
	// gtUser.nIpAddr, gtUser.nNetMask, gtUser.nGateway, gtUser.nDnsSvr0, gtUser.nDnsSvr1, gtUser.bUseDhcp
	snprintf(strXmlData, iInsertLen, "%s<TCP>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IP>%IP</IP>%s", strXmlData, gtUser.nIpAddr, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NETMASK>%IP</NETMASK>%s", strXmlData, gtUser.nNetMask, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<GATEWAY>%IP</GATEWAY>%s", strXmlData, gtUser.nGateway, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS0>%IP</DNS0>%s", strXmlData, gtUser.nDnsSvr0, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DNS1>%IP</DNS1>%s", strXmlData, gtUser.nDnsSvr1, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DHCPUSE>%u</DHCPUSE>%s", strXmlData, gtUser.bUseDhcp, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</TCP>%s", strXmlData, CRLF);

	// UPNP
	// gtUser.strDeviceId, gtUser.strDeviceName
	snprintf(strXmlData, iInsertLen, "%s<UPNP>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DEVICEID>%s</DEVICEID>%s", strXmlData, gtUser.strDeviceId, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<DEVICENAME>%s</DEVICENAME>%s", strXmlData, gtUser.strDeviceName, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</UPNP>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s</NETWORK>%s", strXmlData, CRLF);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	snprintf(strXmlData, iInsertLen, "%s<VIDEO>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s<SENSOR>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<NAME>%s</NAME>%s", strXmlData, sensor_name, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<SIZE_W>%u</SIZE_W>%s", strXmlData, SENSOR_SIZE_W, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<SIZE_H>%u</SIZE_H>%s", strXmlData, SENSOR_SIZE_H, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<MAXFPS>%d</MAXFPS>%s", strXmlData, gptMsgShare.VIDEO_FPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</SENSOR>%s", strXmlData, CRLF);

	snprintf(strXmlData, iInsertLen, "%s<RTSPPORT>%u</RTSPPORT>%s", strXmlData, gtUser.nRtspPort, CRLF);

	// VIDEO-H264_1
	// gtUser.uviH264.eResolution, gtUser.uviH264.byQuality, gtUser.uviH264.woIDRFrame, gtUser.uviH264.sfFPS
	snprintf(strXmlData, iInsertLen, "%s<H264_1>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtUser.nIpAddr, gtUser.nRtspPort, H264URL, CRLF);
#ifdef __WIFI__
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtWificfg.APcfg.iptLocalip.addr, gtUser.nRtspPort, H264URL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtWificfg.STAcfg.iptLocalip.addr, gtUser.nRtspPort, H264URL, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviH264[0].eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviH264[0].nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IDRFRAME>%u</IDRFRAME>%s", strXmlData, gtUser.uviH264[0].nIDRFrame, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviH264[0].sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviH264[0].eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviH264[0].nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</H264_1>%s", strXmlData, CRLF);

	// VIDEO-H264_2
	// gtUser.uviH264.eResolution, gtUser.uviH264.byQuality, gtUser.uviH264.woIDRFrame, gtUser.uviH264.sfFPS
#ifdef __H264_2CH__
	snprintf(strXmlData, iInsertLen, "%s<H264_2>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtUser.nIpAddr, gtUser.nRtspPort, HSUBURL, CRLF);
#ifdef __WIFI__
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtWificfg.APcfg.iptLocalip.addr, gtUser.nRtspPort, HSUBURL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtWificfg.STAcfg.iptLocalip.addr, gtUser.nRtspPort, HSUBURL, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviH264[1].eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviH264[1].nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<IDRFRAME>%u</IDRFRAME>%s", strXmlData, gtUser.uviH264[1].nIDRFrame, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviH264[1].sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviH264[1].eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviH264[1].nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</H264_2>%s", strXmlData, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<H264_2>%s</H264_2>%s", strXmlData, XML_DISABLE, CRLF);
#endif

	// VIDEO-JEPG
	// gtUser.uviJpeg.eResolution, gtUser.uviJpeg.byQuality, gtUser.uviJpeg.sfFPS
#ifdef __JPEG__
	snprintf(strXmlData, iInsertLen, "%s<JPEG>%s", strXmlData, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL>rtsp://%IP:%u/%s</URL>%s", strXmlData, gtUser.nIpAddr, gtUser.nRtspPort, JPEGURL, CRLF);
#ifdef __WIFI__
	snprintf(strXmlData, iInsertLen, "%s<URL1>rtsp://%IP:%u/%s</URL1>%s", strXmlData, gtWificfg.APcfg.iptLocalip.addr, gtUser.nRtspPort, JPEGURL, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<URL2>rtsp://%IP:%u/%s</URL2>%s", strXmlData, gtWificfg.STAcfg.iptLocalip.addr, gtUser.nRtspPort, JPEGURL, CRLF);
#endif
	snprintf(strXmlData, iInsertLen, "%s<RESOLUTION>%u</RESOLUTION>%s", strXmlData, gtUser.uviJpeg.eResolution, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<QUALITY>%u</QUALITY>%s", strXmlData, gtUser.uviJpeg.nQuality, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<FPS>%u</FPS>%s", strXmlData, gtUser.uviJpeg.sfFPS, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRMODE>%u</BRMODE>%s", strXmlData, gtUser.uviJpeg.eBRMode, CRLF);
	snprintf(strXmlData, iInsertLen, "%s<BRATE>%u</BRATE>%s", strXmlData, gtUser.uviJpeg.nBitRate, CRLF);
	snprintf(strXmlData, iInsertLen, "%s</JPEG>%s", strXmlData, CRLF);
#else
	snprintf(strXmlData, iInsertLen, "%s<JPEG>%s</JPEG>%s", strXmlData, XML_DISABLE, CRLF);
#endif

	snprintf(strXmlData, iInsertLen, "%s</VIDEO>%s", strXmlData, CRLF);
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	strcpy(pcInsert, strXmlData);
//	snprintf(pcInsert, iInsertLen, strXmlData);
}

static void SSI_entry_func(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tisp=%u;\n\tmod_lens=%u;\n\tmod_iris=%u;\n\tsdcd=%u;\n\tavc=%u;\n\tjpeg=%u;\n\teth=%u;\n\twifi=%u;\n\t",
#ifdef __ISP__
	1, model_Lens, model_Iris,
#else
	0, 0, 0,
#endif
#ifdef __DEVICE_SD__
	1,
#else
	0,
#endif
#ifdef __H264__
	1,
#else
	0,
#endif
#ifdef __JPEG__
	1,
#else
	0,
#endif
#if defined(__ETH__)
	1,
#else
	0,
#endif
#if defined(__WIFI__)
	1);
#else
	0);
#endif
}

static void SSI_entry_tcp(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tmac0=0x%x;\n\tmac1=0x%x;\n\tmac2=0x%x;\n\tmac3=0x%x;\n\tmac4=0x%x;\n\tmac5=0x%x;\n\tip='%IP';\n\tnm='%IP';\n\tgw='%IP';\n\tdn1='%IP';\n\tdn2='%IP';\n\tdhcp=%d;\n",
			gtSystem.nMacAddress[0], gtSystem.nMacAddress[1], gtSystem.nMacAddress[2], gtSystem.nMacAddress[3], gtSystem.nMacAddress[4], gtSystem.nMacAddress[5],
			gtUser.nIpAddr, gtUser.nNetMask, gtUser.nGateway, gtUser.nDnsSvr0, gtUser.nDnsSvr1, gtUser.bUseDhcp);
}

static void SSI_entry_upnp(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tuid='%s';\n\tuname='%s';\n",
			gtUser.strDeviceId, gtUser.strDeviceName);
}

static void SSI_entry_user(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tuid='%s';\n\tpw='%s';\n\tftpserver=%d;\n\tsdsave=%d;\n\taudiostm=%d;\n",
			gtUser.strUserId, gtUser.strUserPw,
#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
			gtUser.bFtpServer,
#else
			-1,
#endif
#ifdef __DEVICE_SD__
			gtUser.bSdAviSave,
#else
			-1,
#endif
#else
			-1, -1,
#endif
#ifdef __AUDIO__
			gtUser.bAudio);
#else
			-1);
#endif
}

static void SSI_entry_date(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tsntpsw=%d;\n"
#if (ENX_SNTPC_use==1)
		"\tsntpsvr='%s';\n"
		"\trepetper=%u;\n"
		"\trettime=%u;\n"
		"\tretcount=%u;\n"
#endif
		"\tfirsttime=%u;\n"
		"\tsystime=%u;\n"
		"\tsummertime=%d;\n"
		"\ttimezone=%d;\n",
#if (ENX_SNTPC_use==1)
			gtUser.bUseSntp, gtUser.strSntpSvr, gtUser.nSntpPeriodic, gtUser.nSntpRetrySec, gtUser.nSntpRetryMaxcnt,
#else
			-1,
#endif
			gptMsgShare.UPTIME, gptMsgShare.TIME + gptMsgShare.TIMEZONE, gtUser.nTimeSummer, gtUser.nTimeZone);	// SNTP서버명, 기기작동시간, 기기의시간값(sntp동기값/jiffies), TimeZone
}

static void SSI_entry_vinfo(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\tSensorName='%s';\n\tSensorWSize=%d;\n\tSensorHSize=%d;\n\tSensorMAXfps=%d;\n\t" \
		"MAXTOTAL=%d;\n\tH1RMax=%d;\n\tH2RMax=%d;\n\tJRMax=%d;\n\tHWDS0=%d;\n\tHWDS1=%d;\n\t" \
		"HqualityMAX=%d;\n\tHqualityMIN=%d;\n\tHidrframeMAX=%d;\n\tHidrframeMIN=%d;\n\tJqualityMAX=%d;\n\tJqualityMIN=%d;\n\t",
			sensor_name, SENSOR_SIZE_W, SENSOR_SIZE_H, gptMsgShare.VIDEO_FPS,
			e_res320x240, IMG_H264_RES, IMG_H264_2CH_RES, IMG_JPEG_RES, HW_DS0, HW_DS1,
#ifdef __H264__
			H264_Quantize_MAX, H264_Quantize_MIN, H264_Idrframe_MAX, H264_Idrframe_MIN,
#else
			-1, -1, -1, -1,
#endif
#ifdef __JPEG__
			JPEG_Quantize_MAX, JPEG_Quantize_MIN);
#else
			-1, -1);
#endif
}

static void SSI_entry_video(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n\trtspport=%d;\n\t" \
		"Jresolution=%d;\n\tJquality=%d;\n\tJfps=%d;\n\tJbrmode=%d;\n\tJbrate=%d;\n\t" \
		"Hprofile=%d;\n\tHresolution=%d;\n\tHquality=%d;\n\tHidrframe=%d;\n\tHfps=%d;\n\tHbrmode=%d;\n\tHbrate=%d;\n\t" \
		"H2profile=%d;\n\tH2resolution=%d;\n\tH2quality=%d;\n\tH2idrframe=%d;\n\tH2fps=%d;\n\tH2brmode=%d;\n\tH2brate=%d;\n",
			gtUser.nRtspPort,
#ifdef __JPEG__
			gtUser.uviJpeg.eResolution, gtUser.uviJpeg.nQuality, gtUser.uviJpeg.sfFPS, gtUser.uviJpeg.eBRMode, gtUser.uviJpeg.nBitRate,
#else
			-1, -1, -1, -1, -1,
#endif
#ifdef __H264__
			gtUser.uviH264[0].eProfileMode, gtUser.uviH264[0].eResolution, gtUser.uviH264[0].nQuality, gtUser.uviH264[0].nIDRFrame, gtUser.uviH264[0].sfFPS, gtUser.uviH264[0].eBRMode, gtUser.uviH264[0].nBitRate,
#ifdef __H264_2CH__
			gtUser.uviH264[1].eProfileMode, gtUser.uviH264[1].eResolution, gtUser.uviH264[1].nQuality, gtUser.uviH264[1].nIDRFrame, gtUser.uviH264[1].sfFPS, gtUser.uviH264[1].eBRMode, gtUser.uviH264[1].nBitRate);
#else
			-1, -1, -1, -1, -1, -1, -1);
#endif
#else
			-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1);
#endif
}

static void SSI_entry_play(char *pcInsert, int iInsertLen)
{
#ifdef __WIFI__
	snprintf(pcInsert, iInsertLen, "\n\tip='%IP';\n\tip_ap='%IP';\n\tip_sta='%IP';\n\trtpport=%d;\n\tsystime=%d;\n\tID='%s';\n\tPW='%s';\n", gtUser.nIpAddr, gtWificfg.APcfg.iptLocalip.addr, gtWificfg.STAcfg.iptLocalip.addr, gtUser.nRtspPort, gptMsgShare.TIME, gtUser.strUserId, gtUser.strUserPw);	// IP Address, Server Time
#else
	snprintf(pcInsert, iInsertLen, "\n\tip='%IP';\n\trtpport=%d;\n\tsystime=%d;\n\tID='%s';\n\tPW='%s';\n", gtUser.nIpAddr, gtUser.nRtspPort, gptMsgShare.TIME, gtUser.strUserId, gtUser.strUserPw);	// IP Address, Server Time
#endif
}

static void SSI_entry_info_m(char *pcInsert, int iInsertLen)
{
	char *strOutput = "\n"
		"var m_arena=%u;\n"
		"var m_ordblks=%u;\n"
		"var m_uordblks=%u;\n"
		"var m_fordblks=%u;\n"
		"var m_keepcost=%u;\n";

// Memory infomation
	struct mallinfo minfo = mallinfo();

// Output
	snprintf(pcInsert, iInsertLen, strOutput,
		(unsigned int) minfo.arena,
		(unsigned int) minfo.ordblks,
		(unsigned int) minfo.uordblks,
		(unsigned int) minfo.fordblks,
		(unsigned int) minfo.keepcost);
}

static void SSI_entry_info_t(char *pcInsert, int iInsertLen)
{
	char *strOutput = "\n"
		"var t_count=%u;\n"
		"var t_string='  Task     State  Priority  Stack     Number   RunTimeCounter  Percentage\\n--------------------------------------------------------------------------------\\n%s';\n";

// FreeRTOS Task Info
	unsigned portBASE_TYPE i, num_of_task = uxTaskGetNumberOfTasks();
	int nPos = 0;
	char *pbuf = pvPortMalloc((num_of_task+1)*80);
	EnxTaskInfo *etiList = (EnxTaskInfo *)pvPortCalloc(sizeof(EnxTaskInfo), num_of_task);
	EYENIX_vTaskGetRunTimeCntNStats(etiList, num_of_task);
	for (i = 0; i < num_of_task; i++) {
		nPos += sprintf(pbuf + nPos, " %-8s    %c       %u      %5u   0x%08X     %10u  %9.1f%\\n", etiList[i].pcTaskName, etiList[i].cStatus, etiList[i].uxPriority,
			etiList[i].usStackRemaining, etiList[i].pxNextTCB, etiList[i].ulRunTimeCounter, etiList[i].ulStatsAsPermil/10.0);
	}
	nPos += sprintf(pbuf + nPos, "-State(S:Suspended D:Deleted B:Blocked R:Ready) TotalRunTimeCounter(%10u)-\\n", xTaskGetTickCount());
	vPortFree(etiList);

// Output
	snprintf(pcInsert, iInsertLen, strOutput,
		num_of_task,
		pbuf);

	vPortFree(pbuf);
}

static void SSI_entry_info_r(char *pcInsert, int iInsertLen)
{
	char *strOutput = "\n"
		"var stm_cnt=[%u,%u];\n";

// RTSP stream user
	int i;
	struct tm tmout;
	char strTemp[RTSP_info_MAX][256] = {{0}};
	int nTempSize[RTSP_info_MAX] = {0};
	char strStmUser[256] = "var stm_user=[", strBuff[16] = {0};
	char *strPos = strStmUser;
	for (i = 0; i < RTSP_info_MAX; i++) {
		sprintf(strBuff, "stm_user%d,", i);
		strPos = strcat(strPos, strBuff);
		if (gptMsgShare.rtsp_conn_info[i].connect_time != 0xFFFFFFFF) {
			enx_get_tmtime(gptMsgShare.rtsp_conn_info[i].connect_time, &tmout, DEF_YES);
			nTempSize[i] = snprintf(strTemp[i], 256, "var stm_user%d=['%IP:%d','%IP:%d','%s','%04d-%02d-%02d %02d:%02d:%02d','%s'];\n", i,
				gptMsgShare.rtsp_conn_info[i].server_addr, gptMsgShare.rtsp_conn_info[i].server_port,
				gptMsgShare.rtsp_conn_info[i].client_addr, gptMsgShare.rtsp_conn_info[i].client_port,
				gptMsgShare.rtsp_conn_info[i].rtp_type == 0 ? "UDP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 1 ? "TCP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 2 ? "HTTP" : "Unknown",
				tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec,
				gptMsgShare.rtsp_conn_info[i].video_type == 0 ? "H.264 1st" : gptMsgShare.rtsp_conn_info[i].video_type == 1 ? "H.264 2nd" : gptMsgShare.rtsp_conn_info[i].video_type == 9 ? "Motion JPEG" : "Unknown");
		} else {
			nTempSize[i] = snprintf(strTemp[i], 256, "var stm_user%d=['','','','',''];\n", i);
		}
	}
	strPos = strcat(strPos, "];\n");

// Output
	snprintf(pcInsert, iInsertLen, strOutput,
		RTSP_info_MAX, gptMsgShare.rtsp_conn_cnt);

	strPos = pcInsert;
	for (i = 0; i < RTSP_info_MAX; i++) {
		strPos = strcat(strPos, strTemp[i]);
	}
	strcat(strPos, strStmUser);
}

static void SSI_entry_wifi(char *pcInsert, int iInsertLen)
{
#ifdef __WIFI__
	snprintf(pcInsert, iInsertLen, "\n\twifi=1;\n\t");
#else
	snprintf(pcInsert, iInsertLen, "\n\twifi=0;\n\t");
#endif
}

//static int update_ing = -1;
static void SSI_entry_uping(char *pcInsert, int iInsertLen)
{
	int update_ing = fwupdate_process();
	printf("[%d%]", update_ing);
	snprintf(pcInsert, iInsertLen, "\n\tnowver='%x.%x.%x';\n\tuping=%d;\n\t", ((gtSystem.nVersion &0xf00)>>8), ((gtSystem.nVersion &0x0f0)>>4), ((gtSystem.nVersion &0x00f)>>0), update_ing);
}

static void SSI_entry_sdformat(char *pcInsert, int iInsertLen)
{
	int update_ing = SDcardGetFormatState();
	printf("[%d]", update_ing);
	snprintf(pcInsert, iInsertLen, "\n\tsdfmt=%d;\n", update_ing);
}

int App_EN673SSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch(iIndex)
	{
		case SSI_ENTR_state:
			SSI_entry_state(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_func:
			SSI_entry_func(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_tcp:
			SSI_entry_tcp(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_upnp:
			SSI_entry_upnp(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_vinfo:
			SSI_entry_vinfo(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_sdformat:
			SSI_entry_sdformat(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_user:
			SSI_entry_user(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_date:
			SSI_entry_date(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_video:
			SSI_entry_video(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_play:
			SSI_entry_play(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_info_m:
			SSI_entry_info_m(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_info_t:
			SSI_entry_info_t(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_info_r:
			SSI_entry_info_r(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_wifi:
			SSI_entry_wifi(pcInsert, iInsertLen);
			break;
		case SSI_ENTR_uping:
			SSI_entry_uping(pcInsert, iInsertLen);
			break;

		default:
			return DEF_FAIL;
	}
	return DEF_OK;
}
#endif

#if LWIP_HTTPD_CGI

static const char strUrl_reboot_cmd[] = "/system_reboot_cmd.html";
static const char strUrl_reboot_dly[] = "/system_reboot_ing.html";
static const char strUrl_submit[] = "/submit.html";
static const char strUrl_405[] = "/405.html";
static const char strUrl_uping[] = "/system_uping.html";
static const char strUrl_netchange1[] = "/network_submit1.html";
static const char strUrl_netchange2[] = "/network_submit2.html";
static const char strUrl_snap[] = "/still.jpg";
static const char strUrl_sdnotready[] = "/sdcard_notready.html";
static const char strUrl_sdnotsave[] = "/sdcard_notsave.html";
static const char strUrl_sdformat[] = "/sdcard_formating.html";

extern UINT uiUserinfoUpdate;

static uint32 un32GetValue = 0;
static uint32 un32TempValue;

typedef struct {
	char *strRate;
	UINT unIndex;
	StreamBitRate sbrRate;
} CBRrate;

CBRrate cbrValue[] = {
	{"hC20m", e_hc20m, e_rate20mbps},
	{"hC19m", e_hc19m, e_rate19mbps},
	{"hC18m", e_hc18m, e_rate18mbps},
	{"hC17m", e_hc17m, e_rate17mbps},
	{"hC16m", e_hc16m, e_rate16mbps},
	{"hC15m", e_hc15m, e_rate15mbps},
	{"hC14m", e_hc14m, e_rate14mbps},
	{"hC13m", e_hc13m, e_rate13mbps},
	{"hC12m", e_hc12m, e_rate12mbps},
	{"hC11m", e_hc11m, e_rate11mbps},
	{"hC10m", e_hc10m, e_rate10mbps},
	{"hC9m", e_hc9m, e_rate9mbps},
	{"hC8m", e_hc8m, e_rate8mbps},
	{"hC7m", e_hc7m, e_rate7mbps},
	{"hC6m", e_hc6m, e_rate6mbps},
	{"hC5m", e_hc5m, e_rate5mbps},
	{"hC4m", e_hc4m, e_rate4mbps},
	{"hC3m", e_hc3m, e_rate3mbps},
	{"hC2m", e_hc2m, e_rate2mbps},
	{"hC1m", e_hc1m, e_rate1mbps},
	{"hC512k", e_hc512k, e_rate512kbps},
	{"hC256k", e_hc256k, e_rate256kbps},
	{"hC128k", e_hc128k, e_rate128kbps},

	{"jC50m", e_jc50m, e_rate50mbps},
	{"jC45m", e_jc45m, e_rate45mbps},
	{"jC40m", e_jc40m, e_rate40mbps},
	{"jC35m", e_jc35m, e_rate35mbps},
	{"jC30m", e_jc30m, e_rate30mbps},
	{"jC25m", e_jc25m, e_rate25mbps},
	{"jC20m", e_jc20m, e_rate20mbps},
	{"jC19m", e_jc19m, e_rate19mbps},
	{"jC18m", e_jc18m, e_rate18mbps},
	{"jC17m", e_jc17m, e_rate17mbps},
	{"jC16m", e_jc16m, e_rate16mbps},
	{"jC15m", e_jc15m, e_rate15mbps},
	{"jC14m", e_jc14m, e_rate14mbps},
	{"jC13m", e_jc13m, e_rate13mbps},
	{"jC12m", e_jc12m, e_rate12mbps},
	{"jC11m", e_jc11m, e_rate11mbps},
	{"jC10m", e_jc10m, e_rate10mbps},
	{"jC9m", e_jc9m, e_rate9mbps},
	{"jC8m", e_jc8m, e_rate8mbps},
	{"jC7m", e_jc7m, e_rate7mbps},
	{"jC6m", e_jc6m, e_rate6mbps},
	{"jC5m", e_jc5m, e_rate5mbps},
	{"jC4m", e_jc4m, e_rate4mbps},
	{"jC3m", e_jc3m, e_rate3mbps},
	{"jC2m", e_jc2m, e_rate2mbps},
	{"jC1m", e_jc1m, e_rate1mbps}
};

#ifdef __ETH__
static UINT CGI_entry_IPCHANG(UINT *ipaddr, char *pcValue, UINT UIN_MSGTYPE)
{
	UINT bRes = DEF_FAIL;
	un32TempValue = ipaddr_addr(pcValue);
	if(un32TempValue == IPADDR_NONE)
	{
		dprintf("ERROR %s\r\n", __func__);
	}
	else
	{
		if(*ipaddr != un32TempValue)
		{
			*ipaddr = un32TempValue;
			un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
			bRes = DEF_OK;
		}
	}
	return bRes;
}
#endif

static UINT CGI_entry_INTEGER_MAX_MIN(UINT *unInterger, char *pcValue, UINT UIN_MSGTYPE, UINT nMax, UINT nMin)
{
	UINT bRes = DEF_FAIL;
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
		bRes = DEF_OK;
	}
	return bRes;
}

#ifdef __ETH__
static UINT CGI_entry_HEXINTEGER8_MAX_MIN(BYTE *unInterger, char *pcValue, UINT UIN_MSGTYPE, BYTE nMax, BYTE nMin)
{
	UINT bRes = DEF_FAIL;
	BYTE un8TempValue = strtol(pcValue, NULL, 16);
	if(*unInterger != un8TempValue)
	{
		if(nMax < nMin)
		{
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if(un8TempValue < nMin)		un8TempValue = nMin;
		else if(un8TempValue > nMax)	un8TempValue = nMax;
		*unInterger = un8TempValue;
		un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
		bRes = DEF_OK;
	}
	return bRes;
}
#endif

static UINT CGI_entry_SWITCH(UINT *bSwitch, char *pcValue, UINT UIN_MSGTYPE)
{
	return CGI_entry_INTEGER_MAX_MIN(bSwitch, pcValue, UIN_MSGTYPE, 1, 0);
}

static UINT CGI_entry_STRING(char *strString, char *pcValue, UINT UIN_MSGTYPE)
{
	UINT bRes = DEF_FAIL;
	if(strcmp(strString, pcValue) != 0)
	{
		strcpy(strString, pcValue);
		un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
		bRes = DEF_OK;
	}
	return bRes;
}

static UINT CGI_entry_BRRATE(UINT *unInterger, char *pcValue, UINT UIN_MSGTYPE)
{
	UINT i, bRes = DEF_FAIL;
	for(i=0;i<(sizeof(cbrValue)/sizeof(cbrValue[0]));i++)
	{
		if(strcmp(pcValue, cbrValue[i].strRate) == 0)
		{
			if(*unInterger != cbrValue[i].unIndex)
			{
				*unInterger = cbrValue[i].unIndex;
				un32GetValue = bitchang(un32GetValue, UIN_MSGTYPE, 1);
				bRes = DEF_OK;
				break;
			}
		}
	}
	return bRes;
}

const char *App_EN673CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i;
	UINT nYear = 0, nMonth = 0, nDay = 0, nHour = 0, nMin = 0, nSec = 0;	// Datetime
	un32GetValue = 0;
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);

#ifdef __ETH__
	UINT unEthernetIpChangeFlag = 0;
#endif
	unVideoChangeFlag = 0;

	for(i=0;i<iNumParams;i++)
	{
		un32TempValue = 0;
		if(strcmp(pcParam[i], "btOK") == 0)	continue;
		dprintf("%s : %s \r\n", pcParam[i], pcValue[i]);
		///////////////////////////////////////////////////////////////
		// TCP ////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#ifdef __ETH__
		if(strcmp(pcParam[i], "n_t_ip") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtUser.nIpAddr), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_IP;
		}
		else if(strcmp(pcParam[i], "n_t_sn") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtUser.nNetMask), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_SN;
		}
		else if(strcmp(pcParam[i], "n_t_gw") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtUser.nGateway), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_GW;
		}
		else if(strcmp(pcParam[i], "n_t_dns0") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtUser.nDnsSvr0), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DNS0;
		}
		else if(strcmp(pcParam[i], "n_t_dns1") == 0)
		{
			if(CGI_entry_IPCHANG((UINT *)&(gtUser.nDnsSvr1), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DNS1;
		}
		else if(strcmp(pcParam[i], "n_t_dhcp") == 0)
		{
			if(CGI_entry_SWITCH((UINT *)&(gtUser.bUseDhcp), pcValue[i], UIN_OK) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_DHCP;
		}
		///////////////////////////////////////////////////////////////
		// MAC ////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "n_t_mac0") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[0]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac1") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[1]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac2") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[2]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac3") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[3]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac4") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[4]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else if(strcmp(pcParam[i], "n_t_mac5") == 0)
		{
			if(CGI_entry_HEXINTEGER8_MAX_MIN((BYTE *)&(gtSystem.nMacAddress[5]), pcValue[i], UIN_OK, 0xFF, 0) == DEF_OK)
				unEthernetIpChangeFlag |= UIN_NW_INT_MAC;
		}
		else
#endif
		///////////////////////////////////////////////////////////////
		// UPNP ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if(strcmp(pcParam[i], "n_u_id") == 0)			CGI_entry_STRING((char *)gtUser.strDeviceId, pcValue[i], UIN_OK);
		else if(strcmp(pcParam[i], "n_u_name") == 0)	CGI_entry_STRING((char *)gtUser.strDeviceName, pcValue[i], UIN_OK);
		///////////////////////////////////////////////////////////////
		// RTSP PORT //////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "n_r_port") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nRtspPort), pcValue[i], UIN_OK, 0xFFFF, 0) == DEF_OK)
				unVideoChangeFlag |= UIN_STREAM_PORT;
		}
		///////////////////////////////////////////////////////////////
		// USER ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////	
		else if(strcmp(pcParam[i], "s_u_id") == 0)
		{
			CGI_entry_STRING((char *)gtUser.strUserId, pcValue[i], UIN_OK);
		}
		else if(strcmp(pcParam[i], "s_u_pw") == 0)
		{
			CGI_entry_STRING((char *)gtUser.strUserPw, pcValue[i], UIN_OK);
		}
#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
		else if(strcmp(pcParam[i], "s_u_ftp") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtUser.bFtpServer), pcValue[i], UIN_UR_FTP);
		}
#endif
#endif
#ifdef __AUDIO__
		else if(strcmp(pcParam[i], "s_u_aud") == 0)
		{
			if(CGI_entry_SWITCH((UINT *)&(gtUser.bAudio), pcValue[i], UIN_OK) == DEF_OK)
				unVideoChangeFlag |= UIN_STREAM_AUDIO;
		}
#endif
		///////////////////////////////////////////////////////////////
		// SNTP ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#if (ENX_SNTPC_use==1)
		else if(strcmp(pcParam[i], "s_s_svw") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtUser.bUseSntp), pcValue[i], UIN_UR_SNTP_SW);
		}
		else if(strcmp(pcParam[i], "s_s_svr") == 0)
		{
			CGI_entry_STRING((char *)gtUser.strSntpSvr, pcValue[i], UIN_UR_SNTP_SW);
		}
		else if(strcmp(pcParam[i], "s_s_svp") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nSntpPeriodic), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 1);
		}
		else if(strcmp(pcParam[i], "s_s_svs") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nSntpRetrySec), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 1);
		}
		else if(strcmp(pcParam[i], "s_s_svm") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nSntpRetryMaxcnt), pcValue[i], UIN_UR_SNTP_SW, 0xFFFFFFFF, 0);
		}
#endif
		///////////////////////////////////////////////////////////////
		// DATETIME ///////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "s_s_uy") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nYear), pcValue[i], UIN_UR_DATETIME, 2100, 1970);
		}
		else if(strcmp(pcParam[i], "s_s_um") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nMonth), pcValue[i], UIN_UR_DATETIME, 12, 1);
		}
		else if(strcmp(pcParam[i], "s_s_ud") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nDay), pcValue[i], UIN_UR_DATETIME, 31, 1);
		}
		else if(strcmp(pcParam[i], "s_s_uh") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nHour), pcValue[i], UIN_UR_DATETIME, 23, 0);
		}
		else if(strcmp(pcParam[i], "s_s_ui") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nMin), pcValue[i], UIN_UR_DATETIME, 59, 0);
		}
		else if(strcmp(pcParam[i], "s_s_us") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(nSec), pcValue[i], UIN_UR_DATETIME, 59, 0);
		}
		else if(strcmp(pcParam[i], "s_s_st") == 0)
		{
			CGI_entry_SWITCH((UINT *)&(gtUser.nTimeSummer), pcValue[i], UIN_UR_TIMEZONE);
		}
		else if(strcmp(pcParam[i], "s_s_tz") == 0)
		{
			CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.nTimeZone), pcValue[i], UIN_UR_TIMEZONE, e_tzP1400, e_tzM1200);
		}
		///////////////////////////////////////////////////////////////
		// VIDEO //////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "v_v_jr") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.eResolution), pcValue[i], UIN_OK, e_res320x240, IMG_JPEG_RES) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_jq") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.nQuality), pcValue[i], UIN_OK, JPEG_Quantize_MAX, JPEG_Quantize_MIN) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_jf") == 0)
		{
			UINT bRes = DEF_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviJpeg.sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
			if(bRes == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_jbm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviJpeg.eBRMode), pcValue[i], UIN_OK, e_brmCBR, e_brmQBR) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_jbr") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviJpeg.nBitRate), pcValue[i], UIN_OK) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_JPEG_BRATE;
		}
		else if(strcmp(pcParam[i], "v_v_hp") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].eProfileMode), pcValue[i], UIN_OK, e_pmMainCB, e_pmBaseline) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_PROFILE;
		}
		else if(strcmp(pcParam[i], "v_v_hr") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].eResolution), pcValue[i], UIN_OK, e_res320x240, IMG_H264_RES) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_hq") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].nQuality), pcValue[i], UIN_OK, H264_Quantize_MAX, H264_Quantize_MIN) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_hi") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].nIDRFrame), pcValue[i], UIN_OK, H264_Idrframe_MAX, H264_Idrframe_MIN) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_IDRFRAME;
		}
		else if(strcmp(pcParam[i], "v_v_hf") == 0)
		{
			UINT bRes = DEF_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes = CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[0].sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
#if (model_2M)
			if(MP(FrameMode) == 3 || MP(FrameMode) == 5)	// 50/60 fps(WDR이 아닌 상태)
			{
				if(gtUser.uviH264[0].eResolution == e_res1920x1080)
				{	// 0ch이 FHD인 상태에서0ch이 max fps만큼H.264 인코딩을 할 수 없다.
					if(gtUser.uviH264[0].sfFPS == 0)
						gtUser.uviH264[0].sfFPS = 1;
				}
			}
#endif
			if(bRes == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_hbm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviH264[0].eBRMode), pcValue[i], UIN_OK, e_brmCVBR, e_brmQBR) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_hbr") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviH264[0].nBitRate), pcValue[i], UIN_OK) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_1CH_BRATE;
		}
#ifdef __H264_2CH__
		else if(strcmp(pcParam[i], "v_v_h2p") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].eProfileMode), pcValue[i], UIN_OK, e_pmMainCB, e_pmBaseline) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_PROFILE;
		}
		else if(strcmp(pcParam[i], "v_v_h2r") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].eResolution), pcValue[i], UIN_OK, e_res320x240, IMG_H264_2CH_RES) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_SIZE;
		}
		else if(strcmp(pcParam[i], "v_v_h2q") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].nQuality), pcValue[i], UIN_OK, H264_Quantize_MAX, H264_Quantize_MIN) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_QUALITY;
		}
		else if(strcmp(pcParam[i], "v_v_h2i") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].nIDRFrame), pcValue[i], UIN_OK, H264_Idrframe_MAX, H264_Idrframe_MIN) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_IDRFRAME;
		}
		else if(strcmp(pcParam[i], "v_v_h2f") == 0)
		{
			UINT bRes = DEF_FAIL;
			switch(gptMsgShare.VIDEO_FPS)
			{
				case 60:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s60f1, 0);
					break;
				case 50:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s50f1, 0);
					break;
				case 30:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s30f1, 0);
					break;
				case 25:
					bRes =CGI_entry_INTEGER_MAX_MIN((UINT *)&(gtUser.uviH264[1].sfFPS), pcValue[i], UIN_OK, e_s25f1, 0);
					break;
			}
#if (model_2M)
			if(MP(FrameMode) == 3 || MP(FrameMode) == 5)	// 50/60 fps(WDR이 아닌 상태)
			{
				if(gtUser.uviH264[0].eResolution == e_res1920x1080)
				{	// 0ch이 FHD인 상태에서1ch이 max fps만큼H.264 인코딩을 할 수 없다.
					if(gtUser.uviH264[1].sfFPS == 0)
						gtUser.uviH264[1].sfFPS = 1;
				}
			}
#endif
			if(bRes == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_FPS;
		}
		else if(strcmp(pcParam[i], "v_v_h2bm") == 0)
		{
			if(CGI_entry_INTEGER_MAX_MIN((UINT*)&(gtUser.uviH264[1].eBRMode), pcValue[i], UIN_OK, e_brmCVBR, e_brmQBR) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_BRMODE;
		}
		else if(strcmp(pcParam[i], "v_v_h2br") == 0)
		{
			if(CGI_entry_BRRATE((UINT *)&(gtUser.uviH264[1].nBitRate), pcValue[i], UIN_OK) == DEF_OK)
				unVideoChangeFlag |= UIN_VIDEO_H264_2CH_BRATE;
		}
#endif
		else if (strncmp(pcParam[i], "s_i_kik", 7) == 0) {
			int nUserIdx = -1;
			if (sscanf(pcParam[i], "s_i_kik%d", &nUserIdx) == 1) {
				while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_STREAM_KICK, nUserIdx) == DEF_FAIL)
					vTaskDelay(1); // Time update request to Cop
			} else {
				printf("Kick idx error(%d)\r\n", nUserIdx);
			}
		} else {
			printf("Error Msg : %s(%s)\r\n", pcParam[i], pcValue[i]);
		}
	}

	printf("==============================================================\r\n");

	uiUserinfoUpdate = un32GetValue;
#ifdef __ETH__
	UINT un32Check = unEthernetIpChangeFlag;
	if (unEthernetIpChangeFlag != 0) {
		enx_ipchange_proc(enlETHERNET, unEthernetIpChangeFlag);
	}
#endif

	if (unVideoChangeFlag != 0) {
		sys_thread_new("vchange", VideoChangeTask, NULL, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);
	}

#if defined(__ETH__) ||defined(__WIFI__)
#if (ENX_SNTPC_use==1)
	if (bitcheck(uiUserinfoUpdate, UIN_UR_SNTP_SW)) {
		uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_UR_SNTP_SW, 0);
		if (gtUser.bUseSntp == DEF_ON) {
			printf("SNTP ON\r\n");
			sntpc_start();
		} else {
			printf("SNTP OFF\r\n");
			sntpc_stop();
		}
	}
#endif
#endif
	if (bitcheck(uiUserinfoUpdate, UIN_UR_TIMEZONE)) {
		uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_UR_TIMEZONE, 0);
		setTimeZone(gtUser.nTimeZone);
	}
	if (bitcheck(uiUserinfoUpdate, UIN_UR_DATETIME)) {
		uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_UR_DATETIME, 0);
		set_devicetime(TimeZone_LOC, nYear, nMonth, nDay, nHour, nMin, nSec);
	}
#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
	if (bitcheck(uiUserinfoUpdate, UIN_UR_FTP)) {
		uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_UR_FTP, 0);
		if (gtUser.bFtpServer == FTPD_USE_ON) {
			printf("FTP ON\r\n");
			FtpdStart();
		} else {
			printf("FTP OFF\r\n");
			FtpdStop();
		}
	}
#endif
#endif

	UserSave(sfls_USER);

#ifdef __ETH__
	if(bitcheck(un32Check, UIN_NW_INT_IP))
		return strUrl_netchange1;
	else if(bitcheck(un32Check, UIN_NW_INT_DHCP) && gtUser.bUseDhcp == DHCP_USE_ON)
		return strUrl_netchange2;
	else if (bitcheck(un32Check, UIN_NW_INT_MAC)) {
		UserSave(sfls_SYSTEM);
		return strUrl_reboot_cmd;
	} else
#endif
		return strUrl_submit;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppUpdate_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
	fwinfo->ddr_addr = (BYTE *)FW_UPDATE_BASE;	// 이미 DDR에 펌웨어 파일이 올라와 있다. 올라온 주소를 저장한다.
	fwinfo->type = eFWT_DDR;			// 이미 DDR에 펌웨어 파일이 올라와 있다.
	fwinfo->is_areachange = DEF_YES;	// fw write 중 전원차단 등을 예방하기 위해 area을 변경 후 write한다.
	fwinfo->is_bootwrite = DEF_NO;		// boot.bin은 write하지 않는다.
	fwinfo->is_autoreboot = DEF_NO;		// CGI처리에 의해 재부팅 할것이므로 자동재부팅 하지 않는다.
	fwinfo->is_malloc = DEF_YES;		// 이 구조체는 malloc 했으므로 free를 위해 선언되었다.
	fwinfo->is_encodingstop = DEF_NO;	// httpd_post_begin에서 encoding stop 명령을 내린다.
	sys_thread_new("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
	dprintf("==============================================================\r\n");
	return strUrl_uping;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppUpdate2_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
	fwinfo->ddr_addr = (BYTE *)FW_UPDATE_BASE;	// 이미 DDR에 펌웨어 파일이 올라와 있다. 올라온 주소를 저장한다.
	fwinfo->type = eFWT_DDR;			// 이미 DDR에 펌웨어 파일이 올라와 있다.
	fwinfo->is_areachange = DEF_YES;	// fw write 중 전원차단 등을 예방하기 위해 area을 변경 후 write한다.
	fwinfo->is_bootwrite = DEF_NO;		// boot.bin은 write하지 않는다.
	fwinfo->is_autoreboot = DEF_YES;	// ONVIF에 의해 요청이 들어온 것 이므로 자동 재부팅한다.
	fwinfo->is_malloc = DEF_YES;		// 이 구조체는 malloc 했으므로 free를 위해 선언되었다.
	fwinfo->is_encodingstop = DEF_NO;	// httpd_post_begin에서 encoding stop 명령을 내린다.
	sys_thread_new("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
	dprintf("==============================================================\r\n");
	return strUrl_uping;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}


void AppReboot_process(void *ctx)
{
	WdtSysReboot();

	UNUSED(ctx);
}

const char *AppInit_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i;

	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);

	for(i=0;i<iNumParams;i++)
	{
		dprintf("%s : %s \r\n", pcParam[i], pcValue[i]);
	}

	LoadDefUserInfo();	// cpu0/init.c
	UserSave(sfls_USER);// init info save
	dprintf("==============================================================\n");

	timer_sched_timeout_cb(500, TIMEOUT_ONESHOT, AppReboot_process, NULL);
	return strUrl_submit;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *AppReboot_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);
	timer_sched_timeout_cb(1000, TIMEOUT_ONESHOT, AppReboot_process, NULL);
	dprintf("==============================================================\r\n");
	return strUrl_reboot_dly;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

#ifdef __DEVICE_SD__
extern const char *ghttpuri; // httpd.c
extern int gWebls_page; // fsfilelist.c
const char *App_EN673sdfilelistCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i;
	gWebls_page = 1;
	for (i = 0; i < iNumParams; i++) {
		if (strcmp(pcParam[i], "page") == 0) {
			gWebls_page = atoi(pcValue[i]);
		}
	}
	return ghttpuri;
	UNUSED(iIndex);
}

#if JPEG_SNAPSHOT
static const char *AppJpegSave(void)
{
#if JPEG_SDSAVE
	UINT idle_ck = 0;
	switch (getSDState()) {
		case sd_OFF:	// sd카드x <- sd카드 없음을 알린다.
			printf("%s(%d) : sd_OFF state\r\n", __func__, __LINE__);
			return strUrl_sdnotready;
		case sd_INIT:	// sd카드o, 초기화 중 <- 대기..?
			printf("%s(%d) : sd_INIT state\r\n", __func__, __LINE__);
			while (getSDState() == sd_INIT) {
				idle_ck++;
				vTaskDelay(1);
				if(idle_ck > 200)	// idle max 2sec...
					break;
			}
			if(getSDState() != sd_IDLE)
				break;
		case sd_IDLE:	// sd카드o, IDLE <- 즉시 처리 가능
			printf("%s(%d) : sd_IDLE state\r\n", __func__, __LINE__);
			timer_sched_timeout_cb(10, TIMEOUT_ONESHOT, muxer_jpegstill_process, NULL);
			return strUrl_submit;
		case sd_SAVE:	// sd카드o, 녹화 중 <- jpeg save 작업을 avienc로 넘긴다.
			printf("%s(%d) : sd_SAVE state\r\n", __func__, __LINE__);
			if (muxer_avienc_state(eRecSnapshot) == 0) {
				muxer_avienc_go(eRecSnapshot);
			}
			return strUrl_submit;
		case sd_ERR:	// sd카드o, 오류 <- sd카드 오류 상태임을 알린다.
			printf("%s(%d) : sd_ERR state\r\n", __func__, __LINE__);
			break;
	}
#else
	printf("%s(%d) : Jpeg snapshot save not supported.\r\n", __func__, __LINE__);
#endif
	return strUrl_405;
}
#endif

const char *AppSDSetup_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	const char *resPath = NULL;
	int i;
	int flag_sd_save = 0;

	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);

	for (i = 0; i < iNumParams; i++) {
		if(strcmp(pcParam[i], "btOK") == 0) continue;
		dprintf("%s : %s \r\n", pcParam[i], pcValue[i]);

		if (strncmp(pcParam[i], "s_u_sd", 6) == 0) {
			CGI_entry_SWITCH((UINT *)&(gtUser.bSdAviSave), pcValue[i], UIN_OK);
			flag_sd_save = 1;
		}
#if JPEG_SNAPSHOT
		else if (strncmp(pcParam[i], "savejpeg", 8) == 0) {
			resPath = AppJpegSave();
		}
#endif
		else if (strncmp(pcParam[i], "saveevt", 7) == 0) {
			if (getSDState() != sd_OFF) {
				if (gtUser.bSdAviSave == SDSAVE_USE_ON) {
					if (muxer_avienc_state(eRecEvent) == 0) 	{
						muxer_avienc_go(eRecEvent);
					}
					resPath = strUrl_submit;
				} else {
					printf("Error : State => SD Save OFF\r\n");
					resPath = strUrl_sdnotsave;
				}
			} else {
				resPath = strUrl_sdnotready;
			}
		} else if (strncmp(pcParam[i], "sdformat", 8) == 0) {
			if (getSDState() != sd_OFF) {
				SDcardSetFormat();
				resPath = strUrl_sdformat;
			} else {
				resPath = strUrl_sdnotready;
			}
		} else {
			printf("Error Msg : %s(%s)\r\n", pcParam[i], pcValue[i]);
		}
	}

	printf("==============================================================\r\n");

	if (flag_sd_save == 1) {
		UserSave(sfls_USER);
	}

	if (resPath == NULL) {
		return strUrl_submit;
	} else {
		return resPath;
	}

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}
#endif

#if JPEG_SNAPSHOT
const char *AppJpegStill_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
#if JPEG_WEBVIEW
	return strUrl_snap;
#else
	printf("%s(%d) : Jpeg snapshot view not supported.\r\n", __func__, __LINE__);
	return strUrl_405;
#endif

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}
#endif
#endif
