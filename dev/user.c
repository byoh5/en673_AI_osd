//*************************************************************************************************
// Non-volatile memory control(User data load/save)
//*************************************************************************************************

#include "dev.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h

#include "board.h"
#include "factory.h"
#include "user.h"

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

volatile tSystem gtSystem;		// System information
#ifndef DEF_BOOT
volatile tUser gtUser;			// User information
volatile BYTE *gtISP;			// ISP information
#if defined(__ONVIF__)
volatile BYTE *gtONVIF;			// ONVIF information
volatile UINT gtONVIF_size;		// ONVIF information size
#endif // #if defined(__ONVIF__)
#ifdef __WIFI__
volatile BYTE *gtWIFI;
volatile UINT gtWIFI_size;
#endif
#endif

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// User ID
#ifdef DEF_CPU1
UINT getDateID(void)
{
	const char *sBuildDate = "FW Date : "__DATE__" "__TIME__;
	const UINT sBuildDateLen = strlen(sBuildDate);
	gptMsgShare.CPU1_DATEID  = (*(sBuildDate + sBuildDateLen-1) - '0');
	gptMsgShare.CPU1_DATEID += (*(sBuildDate + sBuildDateLen-2) - '0')<<4;
	gptMsgShare.CPU1_DATEID += (*(sBuildDate + sBuildDateLen-4) - '0')<<8;
	gptMsgShare.CPU1_DATEID += (*(sBuildDate + sBuildDateLen-5) - '0')<<12;
	return gptMsgShare.CPU1_DATEID;
}
#endif

// User data save (1 : Ok, 0 : Ng)
#ifndef DEF_CPU0	// DEF_BOOT, DEF_CPU1 only
BYTE UserSave(sfls_fixed_area index)
{
	BYTE bRes = DEF_FAIL;
//	printf("UserSave(%d)...", index);

#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif

	while(CS_SFLS);

	switch(index)
	{
		case sfls_SYSTEM:	// FFF000
			FlsErase(SFLS_SYSTEM_BASE);
			bRes = FlsWrite(SFLS_SYSTEM_BASE, (BYTE*)&gtSystem, sizeof(tSystem)); 	// 1 : Success, 0 : Fail
			break;
#ifndef DEF_BOOT
		case sfls_USER:		// FFE000
			FlsErase(SFLS_USER_BASE);
			bRes = FlsWrite(SFLS_USER_BASE, (BYTE*)&gtUser, sizeof(tUser)); 		// 1 : Success, 0 : Fail
			break;
#if defined(__ISP__)
		case sfls_ISP_SHD: // 3-FFD000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*0));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*0), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_MENU: // 4-FFC000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*1));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*1), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_MENU_BAK: // 5-FFB000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*2));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*2), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_USER: // 6-FFA000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*3));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*3), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_DATA: // 7-FF9000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*4));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*4), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK0: // 8-FF8000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*5));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*5), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK1: // 9-FF7000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*6));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*6), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK2: // 10-FF6000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*7));
			bRes = FlsWrite((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*7), (BYTE*)gptMsgISPTbl.ISPTblAddr, 512); // 1 : Success, 0 : Fail
			CS_ISPCMD = 0;
			break;
#endif // #if defined(__ISP__)
#if defined(__ONVIF__)
		case sfls_ONVIF:	// FF5000, FF4000, FF3000, FF2000
			FlsErase(SFLS_ONVIF_BASE+(4096*0));
			FlsErase(SFLS_ONVIF_BASE+(4096*1));
			FlsErase(SFLS_ONVIF_BASE+(4096*2));
			FlsErase(SFLS_ONVIF_BASE+(4096*3));
			bRes = FlsWrite(SFLS_ONVIF_BASE, (BYTE *)gtONVIF, gtONVIF_size); 	// 1 : Success, 0 : Fail
			break;
#endif // #if defined(__ONVIF__)
#if defined(__WIFI__)
		case sfls_WIFI:		// FF1000
			FlsErase(SFLS_WIFI_BASE);
			bRes = FlsWrite(SFLS_WIFI_BASE, (BYTE *)gtWIFI, gtWIFI_size); 	// 1 : Success, 0 : Fail
			break;
#endif // #if defined(__WIFI__)
#endif // #ifndef DEF_BOOT
		default:
			printf("Error Idx(%d)...", index);
			break;
	}

	CS_SFLS = 0;

#ifdef DEF_CPU1
	portEXIT_CRITICAL();
#else
	CRITICAL_END;
#endif

//	printf("End\r\n");

	return bRes;
}

//-------------------------------------------------------------------------------------------------
// User data load (1 : Ok, 0 : Ng)
BYTE UserLoad(sfls_fixed_area index)
{
	BYTE bRes = DEF_FAIL;
//	printf("UserLoad(%d)...", index);

#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif

	while(CS_SFLS);

	switch(index)
	{
		case sfls_SYSTEM:
			bRes = FlsRead((BYTE*)&gtSystem, SFLS_SYSTEM_BASE, sizeof(tSystem));	// 1 : Success, 0 : Fail
			break;
#ifndef DEF_BOOT
		case sfls_USER:
			bRes = FlsRead((BYTE*)&gtUser, SFLS_USER_BASE, sizeof(tUser));			// 1 : Success, 0 : Fail
			break;
#if defined(__ISP__) // => AppLoadPar()
		case sfls_ISP_SHD:
			break;
		case sfls_ISP_MENU:
			break;
		case sfls_ISP_MENU_BAK:
			break;
		case sfls_ISP_USER:
			break;
		case sfls_ISP_DATA:
			break;
		case sfls_ISP_BLK0:
			break;
		case sfls_ISP_BLK1:
			break;
		case sfls_ISP_BLK2:
			break;
#endif // #if defined(__ISP__)
#if defined(__ONVIF__)
		case sfls_ONVIF:
			bRes = FlsRead((BYTE*)gtONVIF, SFLS_ONVIF_BASE, gtONVIF_size);			// 1 : Success, 0 : Fail
			break;
#endif // #if defined(__ONVIF__)
#if defined(__WIFI__)
		case sfls_WIFI:
			bRes = FlsRead((BYTE*)gtWIFI, SFLS_WIFI_BASE, gtWIFI_size);				// 1 : Success, 0 : Fail
			break;
#endif // #if defined(__WIFI__)
#endif // #ifndef DEF_BOOT
		default:
			printf("Error Idx(%d)...", index);
			break;
	}

	CS_SFLS = 0;

#ifdef DEF_CPU1
	portEXIT_CRITICAL();
#else
	CRITICAL_END;
#endif

//	printf("End\r\n");

	return bRes;
}

BYTE UserErase(sfls_fixed_area index)
{
	BYTE bRes = DEF_FAIL;
//	printf("UserErase(%d)...", index);

#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif

	while(CS_SFLS);

	switch(index)
	{
		case sfls_SYSTEM:	// FFF000
			FlsErase(SFLS_SYSTEM_BASE);
			break;
#ifndef DEF_BOOT
		case sfls_USER:		// FFE000
			FlsErase(SFLS_USER_BASE);
			break;
#if defined(__ISP__)
		case sfls_ISP_SHD: // 3-FFD000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*0));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_MENU: // 4-FFC000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*1));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_MENU_BAK: // 5-FFB000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*2));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_USER: // 6-FFA000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*3));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_DATA: // 7-FF9000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*4));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK0: // 8-FF8000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*5));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK1: // 9-FF7000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*6));
			CS_ISPCMD = 0;
			break;
		case sfls_ISP_BLK2: // 10-FF6000
			FlsErase((SFLS_ISP_BASE+SFLS_ISP_UNIT_SIZE*7));
			CS_ISPCMD = 0;
			break;
#endif // if defined(__ISP__)
#if defined(__ONVIF__)
		case sfls_ONVIF:	// FF5000, FF4000, FF3000, FF2000
			FlsErase(SFLS_ONVIF_BASE+(4096*0));
			FlsErase(SFLS_ONVIF_BASE+(4096*1));
			FlsErase(SFLS_ONVIF_BASE+(4096*2));
			FlsErase(SFLS_ONVIF_BASE+(4096*3));
			break;
#endif // if defined(__ONVIF__)
#if defined(__WIFI__)
		case sfls_WIFI:	// FF1000
			FlsErase(SFLS_WIFI_BASE);
			break;
#endif // #if defined(__WIFI__)
#endif // ifndef DEF_BOOT
		default:
			printf("Error Idx(%d)...", index);
			break;
	}

	CS_SFLS = 0;

#ifdef DEF_CPU1
	portEXIT_CRITICAL();
#else
	CRITICAL_END;
#endif

//	printf("End\r\n");

	return bRes;
}
#endif

//-------------------------------------------------------------------------------------------------
#ifndef DEF_BOOT	// DEF_CPU0, DEF_CPU1 only
const ResolutionInfo listResolution[ResolutionInfoLength] = {
	{"1920x1080", e_res1920x1080, 1920, 1080},
	{"1280x720", e_res1280x720, 1280, 720},
	{"800x600", e_res800x600, 800, 600},
	{"704x480", e_res704x480, 704, 480},
	{"704x400", e_res704x400, 704, 400},
	{"640x480", e_res640x480, 640, 480},
	{"640x360", e_res640x360, 640, 360},
	{"320x240", e_res320x240, 320, 240},
};
#if 0
const VideoBitrateInfo listVideoBitrate[VideoBitRateInfoLength] = {
	{"50Mbps", e_br50m, e_rate50mbps},
	{"45Mbps", e_br45m, e_rate45mbps},
	{"40Mbps", e_br40m, e_rate40mbps},
	{"35Mbps", e_br35m, e_rate35mbps},
	{"30Mbps", e_br30m, e_rate30mbps},
	{"25Mbps", e_br25m, e_rate25mbps},
	{"20Mbps", e_br20m, e_rate20mbps},
	{"19Mbps", e_br19m, e_rate19mbps},
	{"18Mbps", e_br18m, e_rate18mbps},
	{"17Mbps", e_br17m, e_rate17mbps},
	{"16Mbps", e_br16m, e_rate16mbps},
	{"15Mbps", e_br15m, e_rate15mbps},
	{"14Mbps", e_br14m, e_rate14mbps},
	{"13Mbps", e_br13m, e_rate13mbps},
	{"12Mbps", e_br12m, e_rate12mbps},
	{"11Mbps", e_br11m, e_rate11mbps},
	{"10Mbps", e_br10m, e_rate10mbps},
	{"9Mbps", e_br9m, e_rate9mbps},
	{"8Mbps", e_br8m, e_rate8mbps},
	{"7Mbps", e_br7m, e_rate7mbps},
	{"6Mbps", e_br6m, e_rate6mbps},
	{"5Mbps", e_br5m, e_rate5mbps},
	{"4Mbps", e_br4m, e_rate4mbps},
	{"3Mbps", e_br3m, e_rate3mbps},
	{"2Mbps", e_br2m, e_rate2mbps},
	{"1Mbps", e_br1m, e_rate1mbps},
	{"512kbps", e_br512k, e_rate512kbps},
	{"256kbps", e_br256k, e_rate256kbps},
	{"128kbps", e_br128k, e_rate128kbps},
};
#endif

const VideoFPSInfo listVideoFPSInfo60[e_s60End] = {{60, e_s60f60}, {30, e_s60f30}, {20, e_s60f20}, {15, e_s60f15}, {12, e_s60f12}, {10, e_s60f10}, {6, e_s60f6}, {5, e_s60f5}, {4, e_s60f4}, {3, e_s60f3}, {2, e_s60f2}, {1, e_s60f1}};
const VideoFPSInfo listVideoFPSInfo50[e_s50End] = {{50, e_s50f50}, {25, e_s50f25}, {10, e_s50f10}, {5, e_s50f5}, {2, e_s50f2}, {1, e_s50f1}};
const VideoFPSInfo listVideoFPSInfo30[e_s30End] = {{30, e_s30f30}, {15, e_s30f15}, {10, e_s30f10}, {6, e_s30f6}, {5, e_s30f5}, {3, e_s30f3}, {2, e_s30f2}, {1, e_s30f1}};
const VideoFPSInfo listVideoFPSInfo25[e_s25End] = {{25, e_s25f25}, {5, e_s25f5}, {1, e_s25f1}};

UINT ssfps2fps(UINT max_fps, SensorFps fps)
{
	UINT __idx2fps(const VideoFPSInfo *list, UINT len, SensorFps fps)
	{
		UINT i;
		UINT outfps = 0xffffffff;
		for (i = 0; i < len; i++) {
			if (list[i].sfFPS == fps) {
				outfps = list[i].nFPS;
				break;
			}
		}
		return outfps;
	}

	UINT outfps = 0xffffffff;
	switch (max_fps) {
		case 60:
			outfps = __idx2fps(listVideoFPSInfo60, e_s60End, fps);
			break;
		case 50:
			outfps = __idx2fps(listVideoFPSInfo50, e_s50End, fps);
			break;
		case 30:
			outfps = __idx2fps(listVideoFPSInfo30, e_s30End, fps);
			break;
		case 25:
			outfps = __idx2fps(listVideoFPSInfo25, e_s25End, fps);
			break;
	}
	return outfps;
}

SensorFps fps2ssfps(UINT max_fps, UINT fps)
{
	SensorFps __fps2idx(const VideoFPSInfo *list, UINT len, UINT fps)
	{
		UINT i;
		SensorFps outfps = 0xffffffff;
		for (i = 0; i < len; i++) {
			if (list[i].nFPS == fps) {
				outfps = list[i].sfFPS;
				break;
			}
		}
		return outfps;
	}

	SensorFps outfps = 0xffffffff;
	switch (max_fps) {
		case 60:
			outfps = __fps2idx(listVideoFPSInfo60, e_s60End, fps);
			break;
		case 50:
			outfps = __fps2idx(listVideoFPSInfo50, e_s50End, fps);
			break;
		case 30:
			outfps = __fps2idx(listVideoFPSInfo30, e_s30End, fps);
			break;
		case 25:
			outfps = __fps2idx(listVideoFPSInfo25, e_s25End, fps);
			break;
	}
	return outfps;
}
#endif
