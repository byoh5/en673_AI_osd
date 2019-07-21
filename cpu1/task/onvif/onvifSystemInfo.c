#include "dev.h"
#include "dev_inline.h"

#include "onvifSystemInfo.h"
//#include "lwip/netif.h"
#include "onvifProfile.h"
#include "msg.h"
#include "user.h"
//#include "ff.h"

#include "videochange.h"

#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#ifdef __DEVICE_SD__
#include "muxer_avienc.h"					// for muxer_avienc_all_stop
#endif

SysInfo gSysInfo;

char *onvif_svc_namespace[] = {
	DEVICE_SERVICE_URL,
	MEDIA_SERVICE_URL,	
	EVENT_SERVICE_URL,
	IMAGING_SERVICE_URL,
	PANTILT_SERVICE_URL,
	DEVICEIO_SERVICE_URL,
	ANALYTICS_SERVICE_URL
};

char *onvif_svc_address[] = {
	DEVICE_SERVICE_ADDRESS,
	MEDIA_SERVICE_ADDRESS,
	EVENT_SERVICE_ADDRESS,	
	IMAGING_SERVICE_ADDRESS,
	PTZ_SERVICE_ADDRESS,
	DEVIO_SERVICE_ADDRESS,
	ANALY_SERVICE_ADDRESS
};

int onvif_svc_version_major[] = {
	DEVICE_SERVICE_VERSION_MAJOR, 
	MEDIA_SERVICE_VERSION_MAJOR, 
	EVENT_SERVICE_VERSION_MAJOR,
	IMAGING_SERVICE_VERSION_MAJOR,
	PANTILT_SERVICE_VERSION_MAJOR,
	DEVICEIO_SERVICE_VERSION_MAJOR,
	ANALYTICS_SERVICE_VERSION_MAJOR
};

int onvif_svc_version_minor[] = {
	DEVICE_SERVICE_VERSION_MINOR, 
	MEDIA_SERVICE_VERSION_MINOR, 
	EVENT_SERVICE_VERSION_MINOR, 
	IMAGING_SERVICE_VERSION_MINOR,
	PANTILT_SERVICE_VERSION_MINOR,
	DEVICEIO_SERVICE_VERSION_MINOR,
	ANALYTICS_SERVICE_VERSION_MINOR
};


SystemProcessCmd_t SystemProcessCmd; 
SystemProcessCmd_t SystemProcessCmd_UDP; 

/*SYSTEM INFORMATIONS*/
SysInfo *GetSysInfo(void)
{
	return &gSysInfo;
}

int GetProfileSize(void)
{
	return gSysInfo.nProfileSize;
}

void SetProfileSize(int size)
{
	gSysInfo.nProfileSize = size;
}

void DataSettingVEC(VEC_t *VEC, int Vencoder, int nIndex, volatile uservideoinfo *Encinfo)
{
	snprintf(VEC->VEname,  SMALL_INFO_LENGTH, "VideoEncoder%d", nIndex);
	snprintf(VEC->VEtoken, SMALL_INFO_LENGTH, "VideoEncoderToken%d", nIndex);
	printf("%s(%d) : %d-%d-%s-%s\r\n", __func__, __LINE__, nIndex, Vencoder, VEC->VEname, VEC->VEtoken);

	VEC->VEusercount = 1;
	VEC->Vencoder = Vencoder;

	Resolution eResolution = Encinfo->eResolution;
	if (listResolution[eResolution].rIndex == eResolution) {
		VEC->Rwidth = listResolution[eResolution].nWidth;
		VEC->Rheight = listResolution[eResolution].nHeight;
	} else {
		printf("%s(%d) : Unknown Size Index(%d)\r\n", __func__, __LINE__, eResolution);
#if (model_1M)
		VEC->Rwidth = listResolution[e_res1280x720].nWidth;
		VEC->Rheight = listResolution[e_res1280x720].nHeight;
#else
		VEC->Rwidth = listResolution[e_res1920x1080].nWidth;
		VEC->Rheight = listResolution[e_res1920x1080].nHeight;
#endif
	}

	VEC->VEquality = (float)Encinfo->nQuality;
	VEC->frameratelimit = (int)Encinfo->sfFPS;
	VEC->encodinginterval = 1;
	VEC->bitratelimit = Encinfo->nBitRate;
	if (VEC->Vencoder == tt__VideoEncoding__H264) {
		VEC->H264govlength = Encinfo->nIDRFrame;
		VEC->H264Profile = Encinfo->eProfileMode;
	} else {
		VEC->H264govlength = 0;
		VEC->H264Profile = 0;
	}
	VEC->Mpeggovlength = 0;
	VEC->Mpegprofile = 0;
	VEC->sessiontimeout = 72000;
	VEC->Multicast_Conf.port = 0;
	VEC->Multicast_Conf.ttl = 0;
	VEC->Multicast_Conf.autostart = 0;
	VEC->Multicast_Conf.nIPType = tt__IPType__IPv4;
}

void DataSetting(SysInfo *SysInfo)
{
	dvprintf("%s	\r\n",__func__);

	int i;

	if (SysInfo == NULL) {
		dvprintf(" failed\r\n");
		return;
	}
	memset(SysInfo->VSC, 0x00, sizeof(VSC_t) * MAX_USER);
	memset(SysInfo->VEC, 0x00, sizeof(VEC_t) * MAX_USER);

	char *encoder		= ONVIF_DEVICE_SCOPE_TYPE "video_encoder";
	char *transmitter	= ONVIF_DEVICE_SCOPE_TYPE "NetworkVideoTransmitter";
	char *streaming 	= ONVIF_DEVICE_SCOPE_PROFILE "Streaming";
	char *hardware		= ONVIF_DEVICE_SCOPE_HARDWARE "EN673";
	char *name			= ONVIF_DEVICE_SCOPE_NAME "EN673_ENTRY";
	char *location		= ONVIF_DEVICE_SCOPE_LOCATION "korea";

	////////////////////////////////////////////////////////////////////
	// Video Source Configuration
	////////////////////////////////////////////////////////////////////

	SysInfo->SizeDefaultVSC = 1;
	strcpy(SysInfo->VSC[0].Vname, 			"VideoSourceConfig1");	
	strcpy(SysInfo->VSC[0].Vtoken,			"VideoToken1"); 
	strcpy(SysInfo->VSC[0].Vsourcetoken,	"VideoSourceToken1");

	SysInfo->VSC[0].Vcount = 0;
#ifdef __H264__
	SysInfo->VSC[0].Vcount++;
#ifdef __H264_2CH__
	SysInfo->VSC[0].Vcount++;
#endif
#endif
#ifdef __JPEG__
	SysInfo->VSC[0].Vcount++;
#endif
	if (SysInfo->VSC[0].Vcount == 0) {
		printf("%s(%d) : No Video Encoder!!\r\n", __func__, __LINE__);
	}

	SysInfo->VSC[0].Framerate = oget_framerateMax();//oget_framerate();//30.0000000;
	SysInfo->VSC[0].windowx		= 0; 
	SysInfo->VSC[0].windowy		= 0;
#if (model_1M)
	SysInfo->VSC[0].windowwidth	= listResolution[e_res1280x720].nWidth;
	SysInfo->VSC[0].windowheight= listResolution[e_res1280x720].nHeight;
#else
	SysInfo->VSC[0].windowwidth	= listResolution[e_res1920x1080].nWidth;
	SysInfo->VSC[0].windowheight= listResolution[e_res1920x1080].nHeight;
#endif

	////////////////////////////////////////////////////////////////////
	// Audio Source Configuration
	////////////////////////////////////////////////////////////////////

#ifdef __AUDIO__
	SysInfo->SizeDefaultASC = 1;
	strcpy(SysInfo->ASC[0].Aname, 			"AudioSourceConfig1");	
	strcpy(SysInfo->ASC[0].Atoken,			"AudioToken1"); 
	strcpy(SysInfo->ASC[0].Asourcetoken,	"AudioSourceToken1");

	SysInfo->ASC[0].Acount = 0;
#ifdef __H264__
	SysInfo->ASC[0].Acount++;
#ifdef __H264_2CH__
	SysInfo->ASC[0].Acount++;
#endif
#endif
#ifdef __JPEG__
	SysInfo->ASC[0].Acount++;
#endif

	if (SysInfo->ASC[0].Acount == 0) {
		printf("%s(%d) : No Audio Encoder!!\r\n", __func__, __LINE__);
	}
#endif

	////////////////////////////////////////////////////////////////////
	// Imaging Settings
	////////////////////////////////////////////////////////////////////

	switch (MP(MpBackLight)) {
		case MN_BL_OFF:
		case MN_BL_BLC:
		case MN_BL_WDR:
			SysInfo->VSC[0].gImagingSettings.BacklightCompensation.Mode = tt__BacklightCompensationMode__OFF;
			break;
		case MN_BL_HLC:
			SysInfo->VSC[0].gImagingSettings.BacklightCompensation.Mode = tt__BacklightCompensationMode__ON;
			break;
	}
	SysInfo->VSC[0].gImagingSettings.BacklightCompensation.Level	= MP(MpHlMaskThrs);	
	SysInfo->VSC[0].gImagingSettings.Brightness					= MP(MpBrightness);
	SysInfo->VSC[0].gImagingSettings.ColorSaturation				= MP(MpSaturation);
	SysInfo->VSC[0].gImagingSettings.Contrast						= MP(MpAce);
	SysInfo->VSC[0].gImagingSettings.Exposure.Mode 				= tt__ExposureMode__AUTO;
	SysInfo->VSC[0].gImagingSettings.Exposure.Priority 			= tt__ExposurePriority__FrameRate;
	SysInfo->VSC[0].gImagingSettings.Exposure.Window.bottom		= 0.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.Window.top			= 0.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.Window.right 		= 0.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.Window.left			= 0.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.MinExposureTime		= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.MaxExposureTime		= 100.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.MinGain				= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.MaxGain				= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.MinIris				= 1.0; 
	SysInfo->VSC[0].gImagingSettings.Exposure.MaxIris				= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.ExposureTime 		= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.Gain 				= 1.0;
	SysInfo->VSC[0].gImagingSettings.Exposure.Iris 				= 1.0;
	SysInfo->VSC[0].gImagingSettings.Focus.AutoFocusMode			= tt__AutoFocusMode__AUTO;
	SysInfo->VSC[0].gImagingSettings.Focus.DefaultSpeed			= 1;
	SysInfo->VSC[0].gImagingSettings.Focus.NearLimit				= 1;
	SysInfo->VSC[0].gImagingSettings.Focus.FarLimit				= 1;
	SysInfo->VSC[0].gImagingSettings.Focus.__size					= 0; 
	SysInfo->VSC[0].gImagingSettings.Focus.__any					= NULL;
	SysInfo->VSC[0].gImagingSettings.Focus.__anyAttribute			= NULL;
	switch (MP(MpTdn)) {
		case MN_TDN_AUTO:
			SysInfo->VSC[0].gImagingSettings.IrCutFilter = tt__IrCutFilterMode__AUTO;
			break;
		case MN_TDN_DAY:
		case MN_TDN_EXTERN:
			SysInfo->VSC[0].gImagingSettings.IrCutFilter = tt__IrCutFilterMode__ON;
			break;
		case MN_TDN_NIGHT:
			SysInfo->VSC[0].gImagingSettings.IrCutFilter = tt__IrCutFilterMode__OFF;
			break;
	}
	SysInfo->VSC[0].gImagingSettings.Sharpness 					= MP(MpSharpness);
	switch (MP(FrameMode)) {
#if model_2M30p
		case MN_FRAMEMODE_NORMAL_25_30:
		case MN_FRAMEMODE_DNR_25_30:
			SysInfo->VSC[0].gImagingSettings.WideDynamicRange.Mode = tt__WideDynamicMode__OFF;
			break;
		case MN_FRAMEMODE_WDR_12_15:
			SysInfo->VSC[0].gImagingSettings.WideDynamicRange.Mode = tt__WideDynamicMode__ON;
			break;
#else
		case MN_FRAMEMODE_NORMAL_25_30:
		case MN_FRAMEMODE_NORMAL_50_60:
		case MN_FRAMEMODE_DNR_25_30:
		case MN_FRAMEMODE_DNR_50_60:
			SysInfo->VSC[0].gImagingSettings.WideDynamicRange.Mode = tt__WideDynamicMode__OFF;
			break;
		case MN_FRAMEMODE_WDR_12_15:
		case MN_FRAMEMODE_WDR_25_30:
			SysInfo->VSC[0].gImagingSettings.WideDynamicRange.Mode = tt__WideDynamicMode__ON;
			break;
#endif
	}
	SysInfo->VSC[0].gImagingSettings.WideDynamicRange.Level		= MP(MpFrameRateLv);
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.Mode 			= MP(MpAwb);
	switch (MP(MpAwb)) {
		case MN_AWB_AUTO:
		case MN_AWB_AUTOEXT:
		case MN_AWB_PRESET:
			SysInfo->VSC[0].gImagingSettings.WhiteBalance.Mode = tt__WhiteBalanceMode__AUTO;
			break;
		case MN_AWB_MNL:
			SysInfo->VSC[0].gImagingSettings.WhiteBalance.Mode = tt__WhiteBalanceMode__MANUAL;
			break;
	}
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.CrGain			= MP(MpBgain);
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.CbGain			= MP(MpRgain);
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.__size			= 0;
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.__any			= NULL;
	SysInfo->VSC[0].gImagingSettings.WhiteBalance.__anyAttribute	= NULL;
	SysInfo->VSC[0].gImagingSettings.Extension.__size				= 0;
	SysInfo->VSC[0].gImagingSettings.Extension.__any				= NULL;
	SysInfo->VSC[0].gImagingSettings.__anyAttribute				= NULL;

	////////////////////////////////////////////////////////////////////
	// Video Output Configuration
	////////////////////////////////////////////////////////////////////
#if 0
	strcpy(SysInfo->VOC.name,		"testvoc");
	SysInfo->VOC.usecount			= 1;
	strcpy(SysInfo->VOC.token, 		"testvoctoken");
#endif
	////////////////////////////////////////////////////////////////////
	// Video Analytics Configuration
	////////////////////////////////////////////////////////////////////
#if 0
	strcpy(SysInfo->VAC.name,		"testvac");
	SysInfo->VAC.usecount			= 1;
	strcpy(SysInfo->VAC.token, 		"testvactoken");
#endif
	////////////////////////////////////////////////////////////////////
	// Metadata Configuration
	////////////////////////////////////////////////////////////////////
#if 0
	strcpy(SysInfo->MDC.MDname, 	"H264MetaData");
	strcpy(SysInfo->MDC.MDtoken,	"metaDataConfToken");	

	SysInfo->MDC.MDusecount 		= 2;
	SysInfo->MDC.sessiontimeout 	= 72000;
	SysInfo->MDC.VMC.port			= 0;
	SysInfo->MDC.VMC.ttl			= 0;
	SysInfo->MDC.VMC.autostart		= 0;
	SysInfo->MDC.VMC.nIPType		= 0;
#endif
	////////////////////////////////////////////////////////////////////
	// Video Encoder Configuration
	////////////////////////////////////////////////////////////////////

	SysInfo->SizeDefaultVEC = SysInfo->VSC[0].Vcount;

	int idxVEC = 0;
#ifdef __H264__
	DataSettingVEC(&(SysInfo->VEC[idxVEC]), tt__VideoEncoding__H264, idxVEC + 1, &(gtUser.uviH264[0]));
	idxVEC++;

#ifdef __H264_2CH__
	DataSettingVEC(&(SysInfo->VEC[idxVEC]), tt__VideoEncoding__H264, idxVEC + 1, &(gtUser.uviH264[1]));
	idxVEC++;
#endif
#endif

#ifdef __JPEG__
	DataSettingVEC(&(SysInfo->VEC[idxVEC]), tt__VideoEncoding__JPEG, idxVEC + 1, &(gtUser.uviJpeg));
	idxVEC++;
#endif

#ifdef __AUDIO__
	////////////////////////////////////////////////////////////////////
	// Audio Encoder Configuration
	////////////////////////////////////////////////////////////////////
	SysInfo->SizeDefaultAEC = 1;
	strcpy(SysInfo->AEC[0].AEname, "AudioEncoder1");
	strcpy(SysInfo->AEC[0].AEtoken, "AudioEncoderToken1");
	SysInfo->AEC[0].Abitratelimit = G711_KBitrate_MAX;
	SysInfo->AEC[0].Aencoder = tt__AudioEncoding__G711; // tt__AudioEncoding__G711 = 0, tt__AudioEncoding__G726 = 1, tt__AudioEncoding__AAC = 2
	SysInfo->AEC[0].AEusercount = 0;
#ifdef __H264__
	SysInfo->AEC[0].AEusercount++;
#ifdef __H264_2CH__
	SysInfo->AEC[0].AEusercount++;
#endif
#endif
#ifdef __JPEG__
	SysInfo->AEC[0].AEusercount++;
#endif
	SysInfo->AEC[0].Asamplerate = G711_KHzSampleRate_MAX;
	SysInfo->AEC[0].Multicast_Conf.port = 0;
	SysInfo->AEC[0].Multicast_Conf.ttl = 0;
	SysInfo->AEC[0].Multicast_Conf.autostart = 0;
	SysInfo->AEC[0].Multicast_Conf.nIPType = tt__IPType__IPv4; // tt__IPType__IPv4 = 0, tt__IPType__IPv6 = 1
	SysInfo->AEC[0].sessiontimeout = 30000;

	////////////////////////////////////////////////////////////////////
	// Audio Output Configuration
	////////////////////////////////////////////////////////////////////
#if (AUDIO_RX==1)
	strcpy(SysInfo->AOC.AOname, "AudioOutput1");
	strcpy(SysInfo->AOC.AOtoken, "AOCToken1");
	strcpy(SysInfo->AOC.AOutputToken, "AOToken1");
	SysInfo->AOC.AOusecount = 0;
#ifdef __H264__
	SysInfo->AOC.AOusecount++;
#ifdef __H264_2CH__
	SysInfo->AOC.AOusecount++;
#endif
#endif
#ifdef __JPEG__
	SysInfo->AOC.AOusecount++;
#endif
	SysInfo->AOC.AOutputLevel = 5;
#endif
	////////////////////////////////////////////////////////////////////
	// Audio Decoder Configuration
	////////////////////////////////////////////////////////////////////
#if (AUDIO_RX==1)
	strcpy(SysInfo->ADC.ADname, "AudioDecoder1");
	strcpy(SysInfo->ADC.ADtoken, "ADCToken1");
	SysInfo->ADC.ADusecount = 0;
#ifdef __H264__
	SysInfo->ADC.ADusecount++;
#ifdef __H264_2CH__
	SysInfo->ADC.ADusecount++;
#endif
#endif
#ifdef __JPEG__
	SysInfo->ADC.ADusecount++;
#endif
#endif
	////////////////////////////////////////////////////////////////////
#endif

	dvprintf("%s	4\r\n",__func__);

	/*LAN CONFIG*/
	strcpy(SysInfo->lan_config.chipConfig,			"chipConfig");
	strcpy(SysInfo->lan_config.hostname,			"hostName");
	strcpy(SysInfo->lan_config.modelname,			"modelName");
	strcpy(SysInfo->lan_config.nClickSnapFilename,	"snapFileName");
	SysInfo->lan_config.nVideocodecres	 			= 0;	
	SysInfo->lan_config.nVideocombo					= 0;

	strcpy(SysInfo->lan_config.net.domainnameT, "en673");
	SysInfo->lan_config.net.discovery_mode 			= 0; //discoverable : 0 , non discoverable : 1
	SysInfo->lan_config.net.dns.s_addr 				= gtUser.nDnsSvr0;
	SysInfo->lan_config.net.gateway.s_addr 			= gtUser.nGateway;

#if (ENX_SNTPC_use==1)	
	strcpy(SysInfo->lan_config.net.ntp_server, (char *)gtUser.strSntpSvr);
#else
	strcpy(SysInfo->lan_config.net.ntp_server, "");
#endif
	SysInfo->lan_config.net._http_port = HTTP_PORT;
	SysInfo->lan_config.net._http_enable = 1;
	SysInfo->lan_config.net._rtsp_port = RTSP_PORT;
	SysInfo->lan_config.net._rtsp_enable = 1; 

	SysInfo->nScope = 0;
	memset(SysInfo->Scope, 0x00, sizeof(onvif_scopes_t)*MAX_SCOPE);
	SysInfo->Scope[0].type = 0;
	strcpy(SysInfo->Scope[0].str, encoder);	
	SysInfo->Scope[1].type = 0;
	strcpy(SysInfo->Scope[1].str, transmitter);	
	SysInfo->Scope[2].type = 0;
	strcpy(SysInfo->Scope[2].str, streaming);	
	SysInfo->Scope[3].type = 0;
	strcpy(SysInfo->Scope[3].str, hardware);	
	SysInfo->Scope[4].type = 1;
	strcpy(SysInfo->Scope[4].str, name);	
	SysInfo->Scope[5].type = 1;
	strcpy(SysInfo->Scope[5].str, location);	
	SysInfo->nScope = 6;

	SysInfo->nUserNumber = 1;
	for(i=0;i<MAX_USER;i++)
	{
		memset(SysInfo->strGuestId[i], 0x00, sizeof(SysInfo->strGuestId[i]));
		memset(SysInfo->strGuestPw[i], 0x00, sizeof(SysInfo->strGuestPw[i]));
		SysInfo->bUserLevel[i] = 0;
	}
	strcpy(SysInfo->strGuestId[0], GUEST1_ID);
	strcpy(SysInfo->strGuestPw[0], GUEST1_PW);

	////////////////////////////////////////////////////////////////////
	// PTZ Configuration
	////////////////////////////////////////////////////////////////////
	memset(&SysInfo->PTZC, 0x00, sizeof(PTZC_t));
	strcpy(SysInfo->PTZC.PTZname, 	"PTZConfigure");
	strcpy(SysInfo->PTZC.PTZtoken,	"PTZtoken001");	
	
	SysInfo->PTZC.PTZusecount = 1;
#ifdef __H264_2CH__
	SysInfo->PTZC.PTZusecount++;
#endif
#ifdef __JPEG__
	SysInfo->PTZC.PTZusecount++;
#endif	

	strcpy(SysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");	
	strcpy(SysInfo->PTZC.DefaultAbsoluteZoomPositionSpace,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");	
	strcpy(SysInfo->PTZC.DefaultRelativePanTiltTranslationSpace,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");	
	strcpy(SysInfo->PTZC.DefaultRelativeZoomTranslationSpace,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");	
	strcpy(SysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");	
	strcpy(SysInfo->PTZC.DefaultContinuousZoomVelocitySpace,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");	

	SysInfo->PTZC.DefaultPantilt_x = 0;	
	SysInfo->PTZC.DefaultPantilt_y = 0;
	strcpy(SysInfo->PTZC.DefaultPantilt_space,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");	

	SysInfo->PTZC.DefaultZoom_x = 0;	
	strcpy(SysInfo->PTZC.DefaultZoom_space,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");	
	
	strcpy(SysInfo->PTZC.PanTiltLimit_URI,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");	
	SysInfo->PTZC.PanTiltLimit_XRange_Min = -1;	
	SysInfo->PTZC.PanTiltLimit_XRange_Max = 1;	
	SysInfo->PTZC.PanTiltLimit_YRange_Min = -1;	
	SysInfo->PTZC.PanTiltLimit_YRange_Max = 1;	
	
	strcpy(SysInfo->PTZC.ZoomLimit_XRange_URI,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");	
	SysInfo->PTZC.ZoomLimit_XRange_Min = -1;
	SysInfo->PTZC.ZoomLimit_XRange_Max = 1;

	SysInfo->PTZC.DefaultPTZTimeout = 30000;
	SysInfo->PTZC.MinPTZTimeout = 3000;
	SysInfo->PTZC.MaxPTZTimeout = 30000;
	
	SysInfo->PTZC.CurrentPantilt_x = 0;
	SysInfo->PTZC.CurrentPantilt_y = 0;	
	strcpy(SysInfo->PTZC.CurrentPantilt_space,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");	

	SysInfo->PTZC.CurrentZoom_x = 0;	
	strcpy(SysInfo->PTZC.CurrentZoom_space,	"http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
	
	SysInfo->PTZC.CurrentTransPantilt_x = 0;
	SysInfo->PTZC.CurrentTransPantilt_y = 0;	
	strcpy(SysInfo->PTZC.CurrentTransPantilt_space,"http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");

	SysInfo->PTZC.CurrentTransZoom_x = 0;	
	strcpy(SysInfo->PTZC.CurrentTransZoom_space,"http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
	
	SysInfo->PTZC.CurrentSpeedPantilt_x = 0;
	SysInfo->PTZC.CurrentSpeedPantilt_y = 0;	
	strcpy(SysInfo->PTZC.CurrentSpeedPantilt_space,	"http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");	

	SysInfo->PTZC.CurrentSpeedZoom_x = 0;	
	strcpy(SysInfo->PTZC.CurrentSpeedZoom_space, "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");	
	
	SysInfo->PTZC.CurrentPanTiltStatus = tt__MoveStatus__IDLE;	//{tt__MoveStatus__IDLE = 0, tt__MoveStatus__MOVING = 1, tt__MoveStatus__UNKNOWN = 2};
	SysInfo->PTZC.CurrentZoomStatus = tt__MoveStatus__IDLE;	//{tt__MoveStatus__IDLE = 0, tt__MoveStatus__MOVING = 1, tt__MoveStatus__UNKNOWN = 2};

	// set default preset
	SysInfo->PTZC.SizePreset = 1;
	strcpy(SysInfo->PTZC.Preset[0].Name,"DEFAULT_PRESET");
	strcpy(SysInfo->PTZC.Preset[0].Token,"DEFAULT_TOKEN");
	strcpy(SysInfo->PTZC.Preset[0].Pantilt_space, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	SysInfo->PTZC.Preset[0].Pantilt_x = 0;
	SysInfo->PTZC.Preset[0].Pantilt_y = 0;
	strcpy(SysInfo->PTZC.Preset[0].Zoom_space, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
	SysInfo->PTZC.Preset[0].Zoom_x = 0;

	// set default node
	SysInfo->PTZC.SizeNode = 1;
	
	strcpy(SysInfo->PTZC.Node[0].Token, "NodeToken");
	strcpy(SysInfo->PTZC.Node[0].Name, "Node01");

	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/SphericalPositionSpace
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationSpaceFov
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocitySpaceDegrees
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace
	//http://www.onvif.org/ver10/tptz/PanTiltSpaces/SpeedSpaceDegrees
	
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionSpaceMillimeter
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationSpaceMillimeter
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/NormalizedDigitalPosition
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace
	//http://www.onvif.org/ver10/tptz/ZoomSpaces/SpeedSpaceMillimeter
	
	SysInfo->PTZC.Node[0].__sizeAbsolutePanTiltPositionSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].AbsolutePanTiltPositionSpace_URI,  "http://www.onvif.org/ver10/tptz/PanTiltSpaces/PositionGenericSpace");
	SysInfo->PTZC.Node[0].AbsolutePanTiltPositionSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].AbsolutePanTiltPositionSpace_XRange.Min = 0;
	SysInfo->PTZC.Node[0].AbsolutePanTiltPositionSpace_YRange.Max = 1;
	SysInfo->PTZC.Node[0].AbsolutePanTiltPositionSpace_YRange.Min = 0;

	SysInfo->PTZC.Node[0].__sizeAbsoluteZoomPositionSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].AbsoluteZoomPositionSpace_URI, "http://www.onvif.org/ver10/tptz/ZoomSpaces/PositionGenericSpace");
	SysInfo->PTZC.Node[0].AbsoluteZoomPositionSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].AbsoluteZoomPositionSpace_XRange.Min = 0;
	
	SysInfo->PTZC.Node[0].__sizeRelativePanTiltTranslationSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].RelativePanTiltTranslationSpace_URI,  "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
	SysInfo->PTZC.Node[0].RelativePanTiltTranslationSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].RelativePanTiltTranslationSpace_XRange.Min = 0;
	SysInfo->PTZC.Node[0].RelativePanTiltTranslationSpace_YRange.Max = 1;
	SysInfo->PTZC.Node[0].RelativePanTiltTranslationSpace_YRange.Min = 0;
	
	SysInfo->PTZC.Node[0].__sizeRelativeZoomTranslationSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].RelativeZoomTranslationSpace_URI,  "http://www.onvif.org/ver10/tptz/ZoomSpaces/TranslationGenericSpace");
	SysInfo->PTZC.Node[0].RelativeZoomTranslationSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].RelativeZoomTranslationSpace_XRange.Min = -1;
	
	SysInfo->PTZC.Node[0].__sizeContinuousPanTiltVelocitySpace = 1;
	strcpy(SysInfo->PTZC.Node[0].ContinuousPanTiltVelocitySpace_URI,  "http://www.onvif.org/ver10/tptz/PanTiltSpaces/VelocityGenericSpace");
	SysInfo->PTZC.Node[0].ContinuousPanTiltVelocitySpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].ContinuousPanTiltVelocitySpace_XRange.Min = -1;
	SysInfo->PTZC.Node[0].ContinuousPanTiltVelocitySpace_YRange.Max = 1;
	SysInfo->PTZC.Node[0].ContinuousPanTiltVelocitySpace_YRange.Min = -1;
	
	SysInfo->PTZC.Node[0].__sizeContinuousZoomVelocitySpace = 1;
	strcpy(SysInfo->PTZC.Node[0].ContinuousZoomVelocitySpace_URI,  "http://www.onvif.org/ver10/tptz/ZoomSpaces/VelocityGenericSpace");
	SysInfo->PTZC.Node[0].ContinuousZoomVelocitySpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].ContinuousZoomVelocitySpace_XRange.Min = -1;
	
	SysInfo->PTZC.Node[0].__sizePanTiltSpeedSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].PanTiltSpeedSpace_URI,  "http://www.onvif.org/ver10/tptz/PanTiltSpaces/GenericSpeedSpace");
	SysInfo->PTZC.Node[0].PanTiltSpeedSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].PanTiltSpeedSpace_XRange.Min = 0;
	
	SysInfo->PTZC.Node[0].__sizeZoomSpeedSpace = 1;
	strcpy(SysInfo->PTZC.Node[0].ZoomSpeedSpace_URI,  "http://www.onvif.org/ver10/tptz/ZoomSpaces/ZoomGenericSpeedSpace");
	SysInfo->PTZC.Node[0].ZoomSpeedSpace_XRange.Max = 1;
	SysInfo->PTZC.Node[0].ZoomSpeedSpace_XRange.Min = 0;
	
	SysInfo->PTZC.Node[0].MaximumNumberOfPresets = PTZ_PRESET_MAX_SIZE;	
	SysInfo->PTZC.Node[0].HomeSupported = xsd__boolean__false_;
	SysInfo->PTZC.Node[0].__sizeAuxiliaryCommands = 1;
	strcpy(SysInfo->PTZC.Node[0].AuxiliaryCommands, "DEFAULT_AUX");
	SysInfo->PTZC.Node[0].FixedHomePosition = xsd__boolean__false_;

	dvprintf("%s	6\r\n",__func__);
/*
	//initial profile image configue 
	SysInfo->imaging_conf = (imaging_conf_t *)mem_malloc(sizeof(imaging_conf_t) * SysInfo->nProfileSize);	
	for(i =0; i< SysInfo->nProfileSize; i++)
	{
		SysInfo->imaging_conf[i].CrGain 					= SysInfo->gImagingSettings.WhiteBalance.CrGain;
		SysInfo->imaging_conf[i].CbGain		 				= SysInfo->gImagingSettings.WhiteBalance.CbGain;
		SysInfo->imaging_conf[i].Brightness 				= SysInfo->gImagingSettings.Brightness;
		SysInfo->imaging_conf[i].Saturation 				= SysInfo->gImagingSettings.ColorSaturation;
		SysInfo->imaging_conf[i].Contrast 					= SysInfo->gImagingSettings.Contrast;
		SysInfo->imaging_conf[i].Sharpness 					= SysInfo->gImagingSettings.Sharpness;
		SysInfo->imaging_conf[i].WideDynamicMode 			= SysInfo->gImagingSettings.WideDynamicRange.Mode;
		SysInfo->imaging_conf[i].WideDynamicLevel			= SysInfo->gImagingSettings.WideDynamicRange.Level;
		SysInfo->imaging_conf[i].IrCutFilterMode 			= SysInfo->gImagingSettings.IrCutFilter;
		SysInfo->imaging_conf[i].BacklightCompensationMode 	= SysInfo->gImagingSettings.BacklightCompensation.Mode;
		SysInfo->imaging_conf[i].BackLightLevel 			= SysInfo->gImagingSettings.BacklightCompensation.Level;
		SysInfo->imaging_conf[i].WhiteBalanceMode 			= SysInfo->gImagingSettings.WhiteBalance.Mode;//1;
	}
*/

	dvprintf("%s	e\r\n",__func__);

	return;
}

float oget_framerate(void)
{
	switch(gptMsgShare.VIDEO_FPS)
	{
		case 60:
			switch(gtUser.uviH264[0].sfFPS)
			{
				case e_s60f60:			return 60.0;
				case e_s60f30:			return 30.0;
				case e_s60f20:			return 20.0;
				case e_s60f15:			return 15.0;
				case e_s60f12:			return 12.0;
				case e_s60f10:			return 10.0;
				case e_s60f6:			return 6.0;
				case e_s60f5:			return 5.0;
				case e_s60f4:			return 4.0;
				case e_s60f3:			return 3.0;
				case e_s60f2:			return 2.0;
				case e_s60f1:			return 1.0;
				default:				return 60.0;
			}
			break;
		case 50:
			switch(gtUser.uviH264[0].sfFPS)
			{
				case e_s50f50:	return 50.0;
				case e_s50f25:	return 25.0;
				case e_s50f10:	return 10.0;
				case e_s50f5:	return 5.0;
				case e_s50f2:	return 2.0;
				case e_s50f1:	return 1.0;
				default:		return 50.0;
			}
			break;
		case 30:
			switch(gtUser.uviH264[0].sfFPS)
			{
				case e_s30f30:	return 30.0;
				case e_s30f15:	return 15.0;
				case e_s30f10:	return 10.0;
				case e_s30f6:	return 6.0;
				case e_s30f5:	return 5.0;
				case e_s30f3:	return 3.0;
				case e_s30f2:	return 2.0;
				case e_s30f1:	return 1.0;
				default:		return 30.0;
			}
			break;
		case 25:
			switch(gtUser.uviH264[0].sfFPS)
			{
				case e_s25f25:	return 25.0;
				case e_s25f5:	return 5.0;
				case e_s25f1:	return 1.0;
				default:		return 25.0;
			}
			break;
	}
	return (float)gptMsgShare.VIDEO_FPS;
}

float oget_framerateMax(void)
{
#if 1
	return gptMsgShare.VIDEO_FPS;
#else
	switch(gptMsgShare.VIDEO_FPS)
	{
		case 60:	return 60.000000;
		case 50:	return 50.000000;
		case 30:	return 30.000000;
		case 25:	return 25.000000;
		case 20:	return 20.000000;
		case 15:	return 15.000000;
		case 12:	return 12.000000;
		case 10:	return 10.000000;
		case 6:		return 6.000000;
		case 5:		return 5.000000;
		case 4:		return 4.000000;
		case 3:		return 3.000000;
		case 2:		return 2.000000;
		case 1:		return 1.000000;
	}
#endif
}

int oget_framerate2(int framelimit)
{
	switch(gptMsgShare.VIDEO_FPS)
	{
		case 60:
			switch(framelimit)
			{
				case e_s60f60:	return 60;
				case e_s60f30:	return 30;
				case e_s60f20:	return 20;
				case e_s60f15:	return 15;
				case e_s60f12:	return 12;
				case e_s60f10:	return 10;
				case e_s60f6:	return 6;
				case e_s60f5:	return 5;
				case e_s60f4:	return 4;
				case e_s60f3:	return 3;
				case e_s60f2:	return 2;
				case e_s60f1:	return 1;
			}
			break;
		case 50:
			switch(framelimit)
			{
				case e_s50f50:	return 50;
				case e_s50f25:	return 25;
				case e_s50f10:	return 10;
				case e_s50f5:	return 5;
				case e_s50f2:	return 2;
				case e_s30f1:	return 1;
			}
			break;
		case 30:
			switch(framelimit)
			{
				case e_s30f30:	return 30;
				case e_s30f15:	return 15;
				case e_s30f10:	return 10;
				case e_s30f6:	return 6;
				case e_s30f5:	return 5;
				case e_s30f3:	return 3;
				case e_s30f2:	return 2;
				case e_s30f1:	return 1;
			}
			break;
		case 25:
			switch(framelimit)
			{
				case e_s25f25:	return 25;
				case e_s25f5:	return 5;
				case e_s25f1:	return 1;
			}
			break;
	}
	return gptMsgShare.VIDEO_FPS;
}

int oset_framerate(int frameReate)
{
	switch (gptMsgShare.VIDEO_FPS) {
		case 60:
			if(60 <= frameReate)		return e_s60f60;
			else if(30 <= frameReate)	return e_s60f30;
			else if(20 <= frameReate)	return e_s60f20;
			else if(15 <= frameReate)	return e_s60f15;
			else if(12 <= frameReate)	return e_s60f12;
			else if(10 <= frameReate)	return e_s60f10;
			else if(6 <= frameReate)	return e_s60f6;
			else if(5 == frameReate)	return e_s60f5;
			else if(4 == frameReate)	return e_s60f4;
			else if(3 == frameReate)	return e_s60f3;
			else if(2 == frameReate)	return e_s60f2;
			else if(1 == frameReate)	return e_s60f1;
			break;
		case 50:
			if(50 <= frameReate)		return e_s50f50;
			else if(25 <= frameReate)	return e_s50f25;
			else if(10 <= frameReate)	return e_s50f10;
			else if(5 <= frameReate)	return e_s50f5;
			else if(2 <= frameReate)	return e_s50f2;
			else if(1 == frameReate)	return e_s50f1;
			break;
		case 30:
			if(30 <= frameReate)		return e_s30f30;
			else if(15 <= frameReate)	return e_s30f15;
			else if(10 <= frameReate)	return e_s30f10;
			else if(6 <= frameReate)	return e_s30f6;
			else if(5 == frameReate)	return e_s30f5;
			else if(3 <= frameReate)	return e_s30f3;
			else if(2 == frameReate)	return e_s30f2;
			else if(1 == frameReate)	return e_s30f1;
			break;
		case 25:
			if(25 <= frameReate)		return e_s25f25;
			else if(5 <= frameReate)	return e_s25f5;
			else if(1 <= frameReate)	return e_s25f1;
			break;
	}
	printf("%s(%d) : frameReate(%d) Error!\r\n", __func__, __LINE__, frameReate);
	return 0;
}

int oget_bitratelimitH264(int bitrate)
{
	switch (bitrate) {
		case e_hc20m:	return e_rate20mbps;
		case e_hc19m:	return e_rate19mbps;
		case e_hc18m:	return e_rate18mbps;
		case e_hc17m:	return e_rate17mbps;
		case e_hc16m:	return e_rate16mbps;
		case e_hc15m:	return e_rate15mbps;
		case e_hc14m:	return e_rate14mbps;
		case e_hc13m:	return e_rate13mbps;
		case e_hc12m:	return e_rate12mbps;
		case e_hc11m:	return e_rate11mbps;
		case e_hc10m:	return e_rate10mbps;
		case e_hc9m:	return e_rate9mbps;
		case e_hc8m:	return e_rate8mbps;
		case e_hc7m:	return e_rate7mbps;
		case e_hc6m:	return e_rate6mbps;
		case e_hc5m:	return e_rate5mbps;
		case e_hc4m:	return e_rate4mbps;
		case e_hc3m:	return e_rate3mbps;
		case e_hc2m:	return e_rate2mbps;
		case e_hc1m:	return e_rate1mbps;
		case e_hc512k:	return e_rate512kbps;
		case e_hc256k:	return e_rate256kbps;
		case e_hc128k:	return e_rate128kbps;
		default:
			printf("%s(%d) : bitrate(%d) Error!\r\n", __func__, __LINE__, bitrate);
			break;
	}
	return e_rate10mbps;
}

int oget_bitratelimitJPEG(int bitrate)
{
	switch (bitrate) {	 
		case e_jc50m:	return e_rate50mbps;
		case e_jc45m:	return e_rate45mbps;
		case e_jc40m:	return e_rate40mbps;
		case e_jc35m:	return e_rate35mbps;
		case e_jc30m:	return e_rate30mbps;
		case e_jc25m:	return e_rate25mbps;
		case e_jc20m:	return e_rate20mbps;
		case e_jc19m:	return e_rate19mbps;
		case e_jc18m:	return e_rate18mbps;
		case e_jc17m:	return e_rate17mbps;
		case e_jc16m:	return e_rate16mbps;
		case e_jc15m:	return e_rate15mbps;
		case e_jc14m:	return e_rate14mbps;
		case e_jc13m:	return e_rate13mbps;
		case e_jc12m:	return e_rate12mbps;
		case e_jc11m:	return e_rate11mbps;
		case e_jc10m:	return e_rate10mbps;
		case e_jc9m:	return e_rate9mbps;
		case e_jc8m:	return e_rate8mbps;
		case e_jc7m:	return e_rate7mbps;
		case e_jc6m:	return e_rate6mbps;
		case e_jc5m:	return e_rate5mbps;
		case e_jc4m:	return e_rate4mbps;
		case e_jc3m:	return e_rate3mbps;
		case e_jc2m:	return e_rate2mbps;
		case e_jc1m:	return e_rate1mbps;
		default:
			printf("%s(%d) : bitrate(%d) Error!\r\n", __func__, __LINE__, bitrate);
			break;
	}
	return e_rate20mbps;
}

int oset_bitratelimitH264(int limit)
{
	UINT bitrate = e_hc10m;
	if((19*1024)<limit)			bitrate = e_hc20m;
	else if((18*1024)<limit)	bitrate = e_hc19m;
	else if((17*1024)<limit)	bitrate = e_hc18m;
	else if((16*1024)<limit)	bitrate = e_hc17m;
	else if((15*1024)<limit)	bitrate = e_hc16m;
	else if((14*1024)<limit)	bitrate = e_hc15m;
	else if((13*1024)<limit)	bitrate = e_hc14m;
	else if((12*1024)<limit)	bitrate = e_hc13m;
	else if((11*1024)<limit)	bitrate = e_hc12m;
	else if((10*1024)<limit)	bitrate = e_hc11m;
	else if((9*1024)<limit)		bitrate = e_hc10m;
	else if((8*1024)<limit)		bitrate = e_hc9m;
	else if((7*1024)<limit)		bitrate = e_hc8m;
	else if((6*1024)<limit)		bitrate = e_hc7m;
	else if((5*1024)<limit)		bitrate = e_hc6m;
	else if((4*1024)<limit)		bitrate = e_hc5m;
	else if((3*1024)<limit)		bitrate = e_hc4m;
	else if((2*1024)<limit)		bitrate = e_hc3m;
	else if((1*1024)<limit)		bitrate = e_hc2m;
	else if((512)<limit)		bitrate = e_hc1m;
	else if((256)<limit)		bitrate = e_hc512k;
	else if((128)<limit)		bitrate = e_hc256k;
	else if((0)<limit)			bitrate = e_hc128k;
	else						printf("%s(%d) : limit(%d) Error!\r\n", __func__, __LINE__, limit);
	return bitrate;
}

int oset_bitratelimitJPEG(int limit)
{
	UINT bitrate = e_jc20m;
	if((45*1024)<limit)			bitrate = e_jc50m;
	else if((40*1024)<limit)	bitrate = e_jc45m;
	else if((35*1024)<limit)	bitrate = e_jc40m;
	else if((30*1024)<limit)	bitrate = e_jc35m;
	else if((25*1024)<limit)	bitrate = e_jc30m;
	else if((20*1024)<limit)	bitrate = e_jc25m;
	else if((19*1024)<limit)	bitrate = e_jc20m;
	else if((18*1024)<limit)	bitrate = e_jc19m;
	else if((17*1024)<limit)	bitrate = e_jc18m;
	else if((16*1024)<limit)	bitrate = e_jc17m;
	else if((15*1024)<limit)	bitrate = e_jc16m;
	else if((14*1024)<limit)	bitrate = e_jc15m;
	else if((13*1024)<limit)	bitrate = e_jc14m;
	else if((12*1024)<limit)	bitrate = e_jc13m;
	else if((11*1024)<limit)	bitrate = e_jc12m;
	else if((10*1024)<limit)	bitrate = e_jc11m;
	else if((9*1024)<limit)		bitrate = e_jc10m;
	else if((8*1024)<limit)		bitrate = e_jc9m;
	else if((7*1024)<limit)		bitrate = e_jc8m;
	else if((6*1024)<limit)		bitrate = e_jc7m;
	else if((5*1024)<limit)		bitrate = e_jc6m;
	else if((4*1024)<limit)		bitrate = e_jc5m;
	else if((3*1024)<limit)		bitrate = e_jc4m;
	else if((2*1024)<limit)		bitrate = e_jc3m;
	else if((1*1024)<limit)		bitrate = e_jc2m;
	else if((512)<limit)		bitrate = e_jc1m;
	else						printf("%s(%d) : limit(%d) Error!\r\n", __func__, __LINE__, limit);
	return bitrate;
}

u32_t net_get_ifaddr(char *ifname)
{
	struct netif *pnetif = netif_find(ifname);
	if (pnetif != NULL) {
		return pnetif->ip_addr.addr;
	} else {
		return 0;
	}
}

void net_get_hwaddr(char *ifname, char *tp)
{
	struct netif *pnetif = netif_find(ifname);
	if (pnetif != NULL) {
		memcpy(tp, pnetif->hwaddr, NETIF_MAX_HWADDR_LEN);
	}
}

int run_ntpclient(char *value)
{
	return 0;

	UNUSED(value);
}

int sys_set_date(int _Year, int _Month, int _Day)
{
	return 0;

	UNUSED(_Year);
	UNUSED(_Month);
	UNUSED(_Day);
}

int sys_set_time(int _Hour, int _Minute, int _Second)
{
	return 0;

	UNUSED(_Hour);
	UNUSED(_Minute);
	UNUSED(_Second);
}

char *oget_timezone(char *timeZone)
{
	return "GMT+09";

	UNUSED(timeZone);
}

int MatchProfileVSCToken(char * ProfileVtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileVtoken, GetSysInfo()->VSC[idx].Vtoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileVtoken);
	return -1;
}

int MatchProfileVECToken(char * ProfileVtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileVtoken, GetSysInfo()->VEC[idx].VEtoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileVtoken);
	return -1;
}

int MatchProfileASCToken(char * ProfileAtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileAtoken, GetSysInfo()->ASC[idx].Atoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileAtoken);
	return -1;
}

int MatchProfileAECToken(char * ProfileAtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileAtoken, GetSysInfo()->AEC[idx].AEtoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileAtoken);
	return -1;
}

#if 0
int MatchProfileAOCToken(char * ProfileAtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileAtoken, GetSysInfo()->AOC[idx].AOtoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileAtoken);
	return -1;
}
#endif

#if 0
int MatchProfileADCToken(char * ProfileAtoken)
{
	int idx;
	for (idx = 0; idx < MAX_PROFILE; idx++)
		if (strcmp(ProfileAtoken, GetSysInfo()->ADC[idx].ADtoken) == 0)
			return idx;
	printf("    %04d:%s: Error token [%s]!\r\n", __LINE__, __func__, ProfileAtoken);
	return -1;
}
#endif

int ControlSystemData(int stData, void *pData, int szData)
{
	dvprintf("ControlSystemData\r\n");
	
	switch(stData)
	{		
		/*SET DATA*/
		case SFIELD_SET_DEFAULT:
			break;
		case SFIELD_SET_SNTP_SERVER:
			break;
		case SFIELD_SET_MOTIONENABLE:
			break;
		case SFIELD_SET_TIMEZONE:
			break;
		case SFIELD_SET_DAYLIGHT:
			break;
		case SFIELD_SET_HOSTNAME:
			break;
		case SFIELD_SET_DNS:
			break;
		case SFIELD_SET_IP:
			{			
				if(pData == NULL){
					printf(" pData == NULL \r\n");
					return 0;	
				}
				gtUser.nIpAddr = *((UINT*)pData);

				//uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_NW_INT_IP, 1);
				gtUser.bUseDhcp = DHCP_USE_OFF;
				UserSave(sfls_USER);
			}
			break;
		case SFIELD_SET_DHCPC_ENABLE:
			{
//				SysInfo *SysInfo = GetSysInfo();
				int *value = (int *)pData;
//				SysInfo->lan_config.net.dhcp_enable = *value;
				if(*value == 1)//SysInfo->lan_config.net.dhcp_enable
					gtUser.bUseDhcp = DHCP_USE_ON;
				else	
					gtUser.bUseDhcp = DHCP_USE_OFF;
				printf("dhcp DHCP_USE_ON %d gtUser.bUseDhcp %d\r\n", DHCP_USE_ON, gtUser.bUseDhcp);
				UserSave(sfls_USER);
			}
			break;
		case SFIELD_SET_GATEWAY:
			break;
		case SFIELD_SET_RELAYS_CONF:
			break;
		case SFIELD_SET_RELAY_LOGICALSTATE:
			break;
		case SFIELD_SET_BRIGHTNESS:
			break;
		case SFIELD_SET_SATURATION:
			break;
		case SFIELD_SET_CONTRAST:
			break;
		case SFIELD_SET_SHARPNESS:
			break;
		case SFIELD_SET_IMAGING:
			break;
		case SFIELD_SET_VIDEOOUTPUT_CONF:
			break;
		case SFIELD_SET_VIDEOSOURCE_CONF:
			break;
		case SFIELD_SET_VIDEO_MODE:
			break;
		case SFIELD_SET_VIDEOCODECCOMBO:
			break;
		case SFIELD_SET_VIDEOCODECRES:
			{
#ifndef __H264_2CH__				
				dvprintf("SFIELD_SET_VIDEOCODECRES\r\n");
				unsigned char *codec_res = (unsigned char *)pData;
				gtUser.uviH264[0].eResolution = *codec_res;
				printf("res %d\r\n", gtUser.uviH264[0].eResolution);
				unVideoChangeFlag = bitchang(unVideoChangeFlag, UIN_VIDEO_H264_1CH_SIZE, 1);
#endif				
			}
			break;
		case SFIELD_SET_VIDEOENCODER_CONF:
			{
				dvprintf("SFIELD_SET_VIDEOENCODER_CONF\r\n");
//				video_encoder_conf* pEcondVd = (video_encoder_conf *)pData;
//
//				int position = pEcondVd->position;
//				char *VEname_t = pEcondVd->video_encoder_conf_in_t.VEname_t;
//				char *VEtoken_t = pEcondVd->video_encoder_conf_in_t.VEtoken_t;
//				int VEusercount_t = pEcondVd->video_encoder_conf_in_t.VEusercount_t;
//				float VEquality_t = pEcondVd->video_encoder_conf_in_t.VEquality_t;
//				enum tt__VideoEncoding Vencoder_t = pEcondVd->video_encoder_conf_in_t.Vencoder_t;
//				int Rwidth_t = pEcondVd->video_encoder_conf_in_t.Rwidth_t;
//				int Rheight_t = pEcondVd->video_encoder_conf_in_t.Rheight_t;
//				int frameratelimit_t = pEcondVd->video_encoder_conf_in_t.frameratelimit_t;
//				int encodinginterval_t = pEcondVd->video_encoder_conf_in_t.encodinginterval_t;
//				int bitratelimit_t = pEcondVd->video_encoder_conf_in_t.bitratelimit_t;
//				int Mpeggovlength_t = pEcondVd->video_encoder_conf_in_t.Mpeggovlength_t;
//				enum tt__Mpeg4Profile Mpegprofile_t = pEcondVd->video_encoder_conf_in_t.Mpegprofile_t;
//				int H264govlength_t = pEcondVd->video_encoder_conf_in_t.H264govlength_t;
//				enum tt__H264Profile H264profile_t = pEcondVd->video_encoder_conf_in_t.H264profile_t;
//				LONG64 sessiontimeout_t = pEcondVd->video_encoder_conf_in_t.sessiontimeout_t;
//				Multicast_Conf Multicast_Conf_t = pEcondVd->video_encoder_conf_in_t.Multicast_Conf_t;

				SysInfo *SysInfo = GetSysInfo();
				if(SysInfo == NULL)
				{
					dvprintf(" failed \r\n");
					return 0;
				}
/*				
				strcpy(SysInfo->Oprofile[position].VEC.VEname,  VEname_t);		
				strcpy(SysInfo->Oprofile[position].VEC.VEtoken, VEtoken_t);
				
				SysInfo->Oprofile[position].VEC.VEusercount	= VEusercount_t;
				SysInfo->Oprofile[position].VEC.Vencoder		= Vencoder_t; //MJPEG = 0, MPEG4 = 1, H264 = 2;

				SysInfo->Oprofile[position].VEC.Rwidth			= Rwidth_t;
				SysInfo->Oprofile[position].VEC.Rheight		= Rheight_t; 
				SysInfo->Oprofile[position].VEC.VEquality 		= 30;//VEquality_t; //30;
				SysInfo->Oprofile[position].VEC.frameratelimit	= frameratelimit_t;//30;
				
				
				SysInfo->Oprofile[position].VEC.encodinginterval	= encodinginterval_t;//30;
				SysInfo->Oprofile[position].VEC.bitratelimit		= bitratelimit_t;   
				SysInfo->Oprofile[position].VEC.H264govlength 		= H264govlength_t; 
				SysInfo->Oprofile[position].VEC.H264Profile		= H264profile_t; //baseline=0, Main = 1, High = 3	
				SysInfo->Oprofile[position].VEC.Mpeggovlength 		= Mpeggovlength_t;
				SysInfo->Oprofile[position].VEC.Mpegprofile		= Mpegprofile_t;
				
				SysInfo->Oprofile[position].VEC.Multicast_Conf.port		= Multicast_Conf_t.port_t;	 
				SysInfo->Oprofile[position].VEC.Multicast_Conf.ttl			= Multicast_Conf_t.ttl_t;	  
				SysInfo->Oprofile[position].VEC.Multicast_Conf.autostart	= Multicast_Conf_t.autostart_t;
				SysInfo->Oprofile[position].VEC.Multicast_Conf.nIPType		= Multicast_Conf_t.nIPType_t; //IPV4 = 0, IPV6=1  
				//SysInfo->Oprofile[position].VEC.sessiontimeout				= sessiontimeout_t;	

				gtUser.uviH264[0].nIDRFrame = encodinginterval_t;
				gtUser.uviH264[0].nQuality = VEquality_t;
				oset_framerate(frameratelimit_t);
				oset_bitratelimit(bitratelimit_t); 
				printf("encodinginterval_t %d \r\n", encodinginterval_t);
				printf("VEquality_t %f \r\n", VEquality_t);
				printf("frameratelimit_t %d \r\n", frameratelimit_t);
				printf("bitratelimit_t %d \r\n", bitratelimit_t);
				printf("Framerate %d\r\n",gtUser.uviH264[0].sfFPS);
				printf("nBitRate %d\r\n",gtUser.uviH264[0].nBitRate);
*/
				unVideoChangeFlag = bitchang(unVideoChangeFlag, UIN_VIDEO_H264_1CH_FPS, 1);
				//unVideoChangeFlag = bitchang(unVideoChangeFlag, UIN_VIDEO_H264_1CH_QUALITY, 1);
				unVideoChangeFlag = bitchang(unVideoChangeFlag, UIN_VIDEO_H264_1CH_IDRFRAME, 1);
				unVideoChangeFlag = bitchang(unVideoChangeFlag, UIN_VIDEO_H264_1CH_BRATE, 1);
			}
			break;
		case SFIELD_SET_MPEG41_BITRATE:
			break;
		case SFIELD_SET_AUDIO_ENCODE:
			break;
		case SFIELD_SET_AUDIO_SOURCE:
			break;
		case SFIELD_SET_AUDIOENCODER_CONF:
			break;
		case SFIELD_SET_AUDIOENABLE:
			break;
		case SFIELD_SET_AUDIOMODE:
			break;
		case SFIELD_SET_AUDIO_BITRATE:
			break;
		case SFIELD_SET_AUDIO_SAMPLERATE:
			break;
		case SFIELD_SET_VIDEOANALYTICS_CONF:
			break;
		case SFIELD_SET_METADATA:
			break;
			
		/*GET DATA*/	
		case SFIELD_GET_SNTP_FQDN:
			break;

		/*ADD DATA*/
		case SFIELD_ADD_VIDEOSOURCE_CONF:
			{
/*
				video_source_conf *video_source_t = (video_source_conf *)pData;
				
				SysInfo *SysInfo = GetSysInfo();
				if(SysInfo == NULL)
				{
					dvprintf(" failed \r\n");
					return 0;
				}
				int idx = video_source_t->position;
				SysInfo->Oprofile[idx].VSC.Vcount = video_source_t->video_source_conf_in_t.Vcount_t;
				strcpy(SysInfo->Oprofile[idx].VSC.Vsourcetoken, video_source_t->video_source_conf_in_t.Vtoken_t);
*/				
			}
			break;
		case SFIELD_ADD_VIDEOENCODER_CONF:
			{
/*				
				video_encoder_conf *video_encoder_t = (video_encoder_conf *)pData;
				SysInfo *SysInfo = GetSysInfo();
				if(SysInfo == NULL)
				{
					dvprintf(" failed \r\n");
					return 0;
				}
				int idx = video_encoder_t->position;

				SysInfo->Oprofile[idx].VEC.VEusercount = video_encoder_t->video_encoder_conf_in_t.VEusercount_t;
				strcpy(SysInfo->Oprofile[idx].VEC.VEtoken, video_encoder_t->video_encoder_conf_in_t.VEtoken_t);
*/				
			}
			break;
		case SFIELD_ADD_AUDIOENCODER_CONF:
			break;
		case SFIELD_ADD_METADATA:
			break;
		case SFIELD_ADD_PROFILE:
			break;
		case SFIELD_ADD_SCOPE:
			break;
		case SFIELD_ADD_USER:
			break;

		/*DEL DATA*/
		case SFIELD_DEL_VIDEOENCODER_CONF:
			break;
		case SFIELD_DEL_AUDIO_ENCODER:
			break;
		case SFIELD_DEL_AUDIO_SOURCE:
			break;
		case SFIELD_DEL_SCOPE:
			break;
		case SFIELD_DEL_USER:
			break;
		case SFIELD_DEL_PROFILE:
			break;

		/*MODE*/
		case SFIELD_DIS_MODE:
			break;
	}

	return 0;

	if(unVideoChangeFlag)
	{
		UserSave(sfls_USER);
	}
	
	vTaskDelay(1);
	printf("OnvifCFG SAVE\r\n");
	OnvifCFG_Save();
	printf("OnvifCFG SAVE END!!!!\r\n");
	vTaskDelay(1);
	
	//vTaskDelay(100);
	return 0;

	UNUSED(szData);
}

void systemProcessFunction(void)
{
	if(SystemProcessCmd.IS_CALL == FALSE)
		return;	
	printf("systemProcessFunction\r\n");
	
	switch(SystemProcessCmd.CMD)
	{		
		/*SYSTEM REBOOT*/
		case SYSTEM_PROCESS_CMD_REBOOT_CALL:	
		{
#ifdef USE_DISCOVERY_INTERFACE		
			bye_discovery();
#endif
			//save sdcard			
#ifdef __DEVICE_SD__
			muxer_avienc_all_stop();	// øµªÛ ≥Ï»≠¡ﬂ¿Ã∂Û∏È ∏ÿ√·¥Ÿ.
			vTaskDelay(100);
#endif
			// system reset
			printf("system SYSTEM_PROCESS_CMD_REBOOT_CALL !!\r\n");
			vTaskDelay(200);
			//printf("system SYSTEM_PROCESS_CMD_REBOOT_CALL after !!\r\n");
			WdtSysReboot();
		}
			break;
		default:
			break;
	}

	SystemProcessCmd.IS_CALL = FALSE;
	SystemProcessCmd.CMD = SYSTEM_PROCESS_CMD_DEFUALT;	
	
}

#if 0
int OnvifCFG_dircheck(void)
{
	char strGetcwd[8] = {0};
	sprintf(strGetcwd, "%d:/CFG", FLS);
	if(f_stat(strGetcwd, NULL) != FR_OK)
	{
		printf("%s dir Fail. Make dir.\r\n", strGetcwd);
		if(f_mkdir(strGetcwd) != FR_OK)
		{
			printf("%s Make dir Fail.\r\n", strGetcwd);
			return 1;
		}
	}
	return 0;
}
#endif

int OnvifCFG_Save(void)
{
#if 1
	gtONVIF = (BYTE *)&gSysInfo;
	gtONVIF_size = sizeof(gSysInfo);

	dprintf("Onvif Config Save Start\r\n");
	gSysInfo.nMagicNum = MAGICNUM_ONVIF;
	gSysInfo.nDateID = gptMsgShare.CPU1_DATEID;
	UserSave(sfls_ONVIF);
	dprintf("Onvif Config Save End\r\n");
	return 0;
#else
	FIL fp;
	char strGetcwd[32] = {0};
	int u32Length;
	FRESULT fres;

	if(OnvifCFG_dircheck() == 1)
	{
		printf("Onvif Config Save Fail\r\n");
		return 1;
	}

	sprintf(strGetcwd, "%d:/CFG/onvifcfg.ini", FLS);
	printf("Onvif Config Save : %s\r\n", strGetcwd);

	if(f_open(&fp, strGetcwd, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
	{
		printf("Onvif Config Save Start\r\n");
		fres = f_write(&fp, &gSysInfo, sizeof(gSysInfo), &u32Length);
		if(fres != FR_OK)
		{
			printf("Onvif Config Save Fail.\r\n");
		}
		f_close(&fp);

		printf("Onvif Config Save End\r\n");
	}
	return 0;
#endif
}

int OnvifCFG_Load(void)
{
#if 1
	gtONVIF = (BYTE *)&gSysInfo;
	gtONVIF_size = sizeof(gSysInfo);

	dprintf("Onvif Config Load Start\r\n");
	UserLoad(sfls_ONVIF);
	dprintf("Onvif Config Load End\r\n");
	return 0;
#else
	FIL fp;
	char strGetcwd[16] = {0};
	int u32Length;
	FRESULT fres;
	int u32Flag = 0;

	// default value load
	if(OnvifCFG_dircheck() == 1)
	{
		printf("Onvif Config Load Fail. Default Config Load\r\n");		
		init_onvif_profiles();
		u32Flag = 1;
	}
	else
	{
		sprintf(strGetcwd, "%d:/CFG/onvifcfg.ini", FLS);

		if(f_open(&fp, strGetcwd, FA_READ) == FR_OK)
		{
			fres = f_read(&fp, &gSysInfo, sizeof(gSysInfo), &u32Length);
			if(fres != FR_OK)
			{
				printf("Onvif Config Load Fail. Default Onvif Config Load\r\n");				
				init_onvif_profiles();
				u32Flag = 1;
			}
			f_close(&fp);
		}
		else
		{
			printf("Onvif Config Load Fail. Default Config Load\r\n");
			init_onvif_profiles();
			u32Flag = 1;
		}
	}

	dprintf("Onvif Config Load End\r\n");

	if(u32Flag)
	{
		OnvifCFG_Save();
	}
	return 0;
#endif
}
