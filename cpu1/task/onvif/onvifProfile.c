#include "dev.h"
#include "dev_inline.h"

#include "msg.h"

#include "onvifProfile.h"
#include "stdio.h"
#include "stdlib.h"
#include "onvifSystemInfo.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

static void SetDefaultProfile(SysInfo *SysInfo, int idxVEC, char *name)
{
	ENTER();

	Oprofile_t *pPro = &(SysInfo->Oprofile[idxVEC]);
	memset(pPro, 0x00, sizeof(Oprofile_t));

	pPro->fix = 1;
	strcpy(pPro->profilename, name);
	sprintf(pPro->profiletoken, "Profile%d", idxVEC + 1);

	// Video source configuration
	pPro->useVSC = 1;
	strcpy(pPro->VSCtoken, SysInfo->VSC[0].Vtoken);	

	// Video encoder configuration
	pPro->useVEC = 1;
	strcpy(pPro->VECtoken, SysInfo->VEC[idxVEC].VEtoken);

#if 0
	// Video output configuration
	pPro->useVOC = 1;
	strcpy(pPro->VOCtoken, SysInfo->VOC.token);

	// Video analytics configuration
	pPro->useVAC = 1;
	strcpy(pPro->VACtoken, SysInfo->VAC.token);
#endif

#ifdef __AUDIO__
	// Audio source configuration
	pPro->useASC = 1;		
	strcpy(pPro->ASCtoken, SysInfo->ASC[0].Atoken);

	// Audio encoder configuration
	pPro->useAEC = 1;
	strcpy(pPro->AECtoken, SysInfo->AEC[0].AEtoken);

#if (AUDIO_RX==1)
	// Audio output configuration
	pPro->useAOC = 1;	
	strcpy(pPro->AOCtoken, SysInfo->AOC.AOtoken);

	// Audio decoder configuration
	pPro->useADC = 1;	
	strcpy(pPro->ADCtoken, SysInfo->ADC.ADtoken);
#endif
#endif

#if 0
	// Metadata configuration
	pPro->useMDC = 1;
	strcpy(pPro->MDCtoken, SysInfo->MDC.MDtoken);
#endif

	// PTZ configuration
	pPro->usePTZC = 1;
	strcpy(pPro->PTZCtoken, SysInfo->PTZC.PTZtoken);

	LEAVE();
}

void DefualtProfile(SysInfo *SysInfo)
{
	ENTER();

	if (SysInfo == NULL) {
		LEAVE();
		return;
	}

	int idx = 0;
#ifdef __H264__
	SetDefaultProfile(SysInfo, idx, "mainStream");
	idx++;
#ifdef __H264_2CH__
	SetDefaultProfile(SysInfo, idx, "subStream");
	idx++;
#endif
#endif
#ifdef __JPEG__
	SetDefaultProfile(SysInfo, idx, "jpegStream");
	idx++;
#endif

	LEAVE();
}

void init_onvif_profiles(void)
{
	ENTER();

	memset(&gSysInfo, 0x00, sizeof(SysInfo));

	gSysInfo.nMagicNum = MAGICNUM_ONVIF;
	gSysInfo.nDateID = gptMsgShare.CPU1_DATEID;

#ifdef __H264_2CH__
	
#ifndef JPEG_STREAM_UNUSE
	gSysInfo.nProfileSize = 3;
#else
	gSysInfo.nProfileSize = 2;
#endif

#else	// #ifdef __H264_2CH__

#ifndef JPEG_STREAM_UNUSE
	gSysInfo.nProfileSize = 2;
#else
	gSysInfo.nProfileSize = 1;
#endif

#endif	// #ifdef __H264_2CH__

	DataSetting(&gSysInfo);
	DefualtProfile(&gSysInfo);

	LEAVE();
}
