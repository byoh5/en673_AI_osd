#include "dev.h"
#include "dev_inline.h"

#include "FreeRTOS.h" 
#include <stdio.h>

#include "onvifSystemInfo.h"
#include "onvifUtil.h"

#include "msg.h"
#include "user.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h

#include "ipchange.h"
#include "videochange.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()
#define LEAVE()
#endif

void viewProfile(Oprofile_t *pPro)
{
	flprintf("S========================\r\n");
	flprintf("profile name(%s) token(%s)\r\n", pPro->profilename, pPro->profiletoken);
	flprintf("VSC(%s) use(%d)\r\n", pPro->VSCtoken, pPro->useVSC);
	flprintf("VEC(%s) use(%d)\r\n", pPro->VECtoken, pPro->useVEC);
	flprintf("VOC(%s) use(%d)\r\n", pPro->VOCtoken, pPro->useVOC);
	flprintf("VAC(%s) use(%d)\r\n", pPro->VACtoken, pPro->useVAC);
	flprintf("ASC(%s) use(%d)\r\n", pPro->ASCtoken, pPro->useASC);
	flprintf("AEC(%s) use(%d)\r\n", pPro->AECtoken, pPro->useAEC);
	flprintf("AOC(%s) use(%d)\r\n", pPro->AOCtoken, pPro->useAOC);
	flprintf("ADC(%s) use(%d)\r\n", pPro->ADCtoken, pPro->useADC);
	flprintf("MDC(%s) use(%d)\r\n", pPro->MDCtoken, pPro->useMDC);
	flprintf("E========================\r\n");
}

void viewSysInfo(SysInfo *oSysInfo)
{
	int i;
	flprintf("S========================\r\n");
	flprintf("SizeDefaultVSC: %d\r\n", oSysInfo->SizeDefaultVSC);
	for (i = 0; i < MAX_USER; i++) {
		flprintf("VSC N(%s) T(%s) S(%s) Cnt(%d)\r\n", oSysInfo->VSC[i].Vname, oSysInfo->VSC[i].Vtoken, oSysInfo->VSC[i].Vsourcetoken, oSysInfo->VSC[i].Vcount);
	}

	flprintf("SizeDefaultVEC: %d\r\n", oSysInfo->SizeDefaultVEC);
	for (i = 0; i < MAX_USER; i++) {
		flprintf("VEC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->VEC[i].VEname, oSysInfo->VEC[i].VEtoken, oSysInfo->VEC[i].VEusercount);
	}

	flprintf("VOC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->VOC.name, oSysInfo->VOC.token, oSysInfo->VOC.usecount);

	flprintf("VAC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->VAC.name, oSysInfo->VAC.token, oSysInfo->VAC.usecount);

	flprintf("SizeDefaultASC: %d\r\n", oSysInfo->SizeDefaultASC);
	for (i = 0; i < MAX_USER; i++) {
		flprintf("ASC N(%s) T(%s) S(%s) Cnt(%d)\r\n", oSysInfo->ASC[i].Aname, oSysInfo->ASC[i].Atoken, oSysInfo->ASC[i].Asourcetoken, oSysInfo->ASC[i].Acount);
	}

	flprintf("SizeDefaultAEC: %d\r\n", oSysInfo->SizeDefaultAEC);
	for (i = 0; i < MAX_USER; i++) {
		flprintf("AEC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->AEC[i].AEname, oSysInfo->AEC[i].AEtoken, oSysInfo->AEC[i].AEusercount);
	}

	flprintf("AOC N(%s) T(%s) S(%s) Cnt(%d)\r\n", oSysInfo->AOC.AOname, oSysInfo->AOC.AOtoken, oSysInfo->AOC.AOutputToken, oSysInfo->AOC.AOusecount);

	flprintf("ADC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->ADC.ADname, oSysInfo->ADC.ADtoken, oSysInfo->ADC.ADusecount);

	flprintf("MDC N(%s) T(%s) Cnt(%d)\r\n", oSysInfo->MDC.MDname, oSysInfo->MDC.MDtoken, oSysInfo->MDC.MDusecount);

	flprintf("E========================\r\n");
}

#define ONVIF_Quality_MIN 1
#define ONVIF_Quality_MAX 10

int ONVIF_Quality2EncQ(float quality, int enc_type)
{
	int _qua = (int)quality;
	switch (enc_type) {
		case tt__VideoEncoding__JPEG:
			switch (_qua) {
				case 1:		return JPEG_Quantize_MIN;	// 0~4
				case 2:		return 5;					// 5~11
				case 3:		return 12;					// 12~17
				case 4:		return 18;					// 18~23
				case 5:		return 24;					// 24~29
				case 6:		return 30;					// 30~44
				case 7:		return 45;					// 45~59
				case 8:		return 60;					// 60~74
				case 9:		return 75;					// 75~98
				case 10:	return JPEG_Quantize_MAX;	// 99
			}
			break;

		case tt__VideoEncoding__H264:
			switch (_qua) {
				case 1:		return H264_Quantize_MIN;	// 10~13
				case 2:		return 14;					// 14~17
				case 3:		return 18;					// 18~21
				case 4:		return 22;					// 22~25
				case 5:		return 26;					// 26~29
				case 6:		return 30;					// 30~35
				case 7:		return 36;					// 36~40
				case 8:		return 41;					// 41~45
				case 9:		return 46;					// 46~50
				case 10:	return H264_Quantize_MAX;	// 51
			}
			break;

		default:
			flprintf("Error enc_type(%d)\r\n", enc_type);
			break;
	}
	return 0;
}

float ONVIF_EncQ2Quality(int quality, int enc_type)
{
	switch (enc_type) {
		case tt__VideoEncoding__JPEG:
			if (quality < 5)						return 1.0;
			else if (quality < 12)					return 2.0;
			else if (quality < 18)					return 3.0;
			else if (quality < 24)					return 4.0;
			else if (quality < 30)					return 5.0;
			else if (quality < 45)					return 6.0;
			else if (quality < 60)					return 7.0;
			else if (quality < 75)					return 8.0;
			else if (quality < JPEG_Quantize_MAX)	return 9.0;
			else									return 10.0;
			break;

		case tt__VideoEncoding__H264:
			if (quality < 14)						return 1.0;
			else if (quality < 18)					return 2.0;
			else if (quality < 22)					return 3.0;
			else if (quality < 26)					return 4.0;
			else if (quality < 30)					return 5.0;
			else if (quality < 36)					return 6.0;
			else if (quality < 41)					return 7.0;
			else if (quality < 46)					return 8.0;
			else if (quality < H264_Quantize_MAX)	return 9.0;
			else									return 10.0;
			break;

		default:
			flprintf("Error enc_type(%d)\r\n", enc_type);
			break;
	}
	return 0;
}

void userVSCupdate(VSC_t *pVSC)
{
	pVSC->Framerate = oget_framerateMax();
	pVSC->windowx = 0;
	pVSC->windowy = 0;
#if (model_1M)
	pVSC->windowwidth = listResolution[e_res1280x720].nWidth;
	pVSC->windowheight = listResolution[e_res1280x720].nHeight;
#else
	pVSC->windowwidth = listResolution[e_res1920x1080].nWidth;
	pVSC->windowheight = listResolution[e_res1920x1080].nHeight;
#endif

#if 0
	pVSC->gImagingSettings.BacklightCompensation;
	pVSC->gImagingSettings.Brightness;
	pVSC->gImagingSettings.ColorSaturation;
	pVSC->gImagingSettings.Contrast;
	pVSC->gImagingSettings.Exposure;
	pVSC->gImagingSettings.Focus;
	pVSC->gImagingSettings.IrCutFilter;
	pVSC->gImagingSettings.Sharpness;
	pVSC->gImagingSettings.WideDynamicRange;
	pVSC->gImagingSettings.WhiteBalance;
	pVSC->gImagingSettings.Extension;
#endif
}

void userVECupdate(VEC_t *pVEC, int idxVEC)
{
	// VEC를 최신정보로 갱신
	volatile uservideoinfo *Encinfo = NULL;
	pVEC->Mpeggovlength = 0;
	pVEC->Mpegprofile = 0;
	switch (pVEC->Vencoder) {
		case tt__VideoEncoding__JPEG:
			Encinfo = &(gtUser.uviJpeg);
			pVEC->H264govlength = 0;
			pVEC->H264Profile = 0;
			break;

		case tt__VideoEncoding__H264:
			if (idxVEC == 0) {
				Encinfo = &(gtUser.uviH264[0]);
			} else if (idxVEC == 1) {
				Encinfo = &(gtUser.uviH264[1]);
			}
			pVEC->H264govlength = Encinfo->nIDRFrame;
			pVEC->H264Profile = Encinfo->eProfileMode;
			break;

		case tt__VideoEncoding__MPEG4:
		default:
			printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
			break;
	}

	if (Encinfo != NULL) {
		Resolution eResolution = Encinfo->eResolution;
		if (listResolution[eResolution].rIndex == eResolution) {
			pVEC->Rwidth = listResolution[eResolution].nWidth;
			pVEC->Rheight = listResolution[eResolution].nHeight;
		} else {
			printf("%s(%d) : Unknown Size Index(%d)\r\n", __func__, __LINE__, eResolution);
#if (model_1M)
			pVEC->Rwidth = listResolution[e_res1280x720].nWidth;
			pVEC->Rheight = listResolution[e_res1280x720].nHeight;
#else
			pVEC->Rwidth = listResolution[e_res1920x1080].nWidth;
			pVEC->Rheight = listResolution[e_res1920x1080].nHeight;
#endif
		}

		pVEC->VEquality = ONVIF_EncQ2Quality(Encinfo->nQuality, pVEC->Vencoder);
		pVEC->frameratelimit = Encinfo->sfFPS;
		pVEC->encodinginterval = 1;
		pVEC->bitratelimit = Encinfo->nBitRate;
	}
}

/** Auto-test server operation __trt__GetServiceCapabilities */
int __trt__GetServiceCapabilities(struct soap *soap, 
struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, 
struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	ENTER();

	trt__GetServiceCapabilitiesResponse->Capabilities = (struct trt__Capabilities *)soap_malloc(soap, sizeof(struct trt__Capabilities));
	memset(trt__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct trt__Capabilities));

	trt__GetServiceCapabilitiesResponse->Capabilities->ProfileCapabilities = (struct trt__ProfileCapabilities *)soap_malloc(soap, sizeof(struct trt__ProfileCapabilities));
	memset(trt__GetServiceCapabilitiesResponse->Capabilities->ProfileCapabilities, 0x00, sizeof(struct trt__ProfileCapabilities));

	trt__GetServiceCapabilitiesResponse->Capabilities->ProfileCapabilities->MaximumNumberOfProfiles = (int *)soap_malloc(soap, sizeof(int));
	trt__GetServiceCapabilitiesResponse->Capabilities->ProfileCapabilities->MaximumNumberOfProfiles[0] = MAX_PROFILE;

	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities = (struct trt__StreamingCapabilities *)soap_malloc(soap, sizeof(struct trt__StreamingCapabilities));
	memset(trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities, 0x00, sizeof(struct trt__StreamingCapabilities));

	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTPMulticast = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTPMulticast[0] = xsd__boolean__false_;
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTP_USCORETCP[0] = xsd__boolean__true_;
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->RTP_USCORERTSP_USCORETCP[0] = xsd__boolean__true_;
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->NonAggregateControl = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->NonAggregateControl[0] = xsd__boolean__false_;
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->NoRTSPStreaming = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->StreamingCapabilities->NoRTSPStreaming[0] = xsd__boolean__false_;

	trt__GetServiceCapabilitiesResponse->Capabilities->SnapshotUri = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->SnapshotUri[0] = xsd__boolean__true_;
	trt__GetServiceCapabilitiesResponse->Capabilities->Rotation = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->Rotation[0] = xsd__boolean__false_;
	trt__GetServiceCapabilitiesResponse->Capabilities->VideoSourceMode = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->VideoSourceMode[0] = xsd__boolean__true_;
	trt__GetServiceCapabilitiesResponse->Capabilities->OSD = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__GetServiceCapabilitiesResponse->Capabilities->OSD[0] = xsd__boolean__false_;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetServiceCapabilities);
}


/** 
 * @brief Get Video Source 
 * @This operation lists all available video sources for the device.
 *   
 * @param Request	: _trt__GetVideoSources structure contains an empty message.     
 * @param Response	: _trt__GetVideoSourcesResponse structure contains a list of structures describing all available video sources of the device.
 * @return               : On success - SOAP_OK.
 On failure - No specific fault codes.
 */
int __trt__GetVideoSources(struct soap *soap, 
struct _trt__GetVideoSources *trt__GetVideoSources, 
struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();

	int _WideDynamicMode=0;
	int _IrCutFilterMode=0;
	int _WhiteBalanceMode=0;
	float _WhiteBalanceCr=0.0;
	float _WhiteBalanceCb=0.0;
	float _Brightness=0.0;
	float _Saturation=0.0; 
	float _Contrast=0.0; 
	float _Sharpness=0.0; 
	float _WideDynamicRange=0.0;
	int _Backlight=0; 
	float _BacklightCompensationLevel=0.0; 
	int _WhiteBalance=0;
	int n = 0;

	flprintf("nProfileSize(%d) SizeDefaultVSC(%d)\r\n", oSysInfo->nProfileSize, oSysInfo->SizeDefaultVSC);
	trt__GetVideoSourcesResponse->__sizeVideoSources = oSysInfo->SizeDefaultVSC;

	trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource *)soap_malloc(soap, sizeof(struct tt__VideoSource) * trt__GetVideoSourcesResponse->__sizeVideoSources);
	memset(trt__GetVideoSourcesResponse->VideoSources, 0x00, sizeof(struct tt__VideoSource) * trt__GetVideoSourcesResponse->__sizeVideoSources);

	for (n = 0; n < trt__GetVideoSourcesResponse->__sizeVideoSources; n++) {
		struct tt__VideoSource *pVS = &(trt__GetVideoSourcesResponse->VideoSources[n]);
		VSC_t *pVSC = &(oSysInfo->VSC[n]);
		userVSCupdate(pVSC);
		ImagingSettings_t *pVSCIS = &(oSysInfo->VSC[n].gImagingSettings);

		_Brightness = (float)(pVSC->gImagingSettings.Brightness);
		_Saturation = (float)(pVSC->gImagingSettings.ColorSaturation);
		_Contrast = (float)(pVSC->gImagingSettings.Contrast);
		_Sharpness = (float)(pVSC->gImagingSettings.Sharpness);
		_WideDynamicMode = pVSC->gImagingSettings.WideDynamicRange.Mode;
		_WideDynamicRange = (float)pVSC->gImagingSettings.WideDynamicRange.Level;
		_Backlight = pVSC->gImagingSettings.BacklightCompensation.Mode;
		_BacklightCompensationLevel = (float)pVSC->gImagingSettings.BacklightCompensation.Level;
		_IrCutFilterMode = pVSC->gImagingSettings.IrCutFilter;
		_WhiteBalanceMode = pVSC->gImagingSettings.WhiteBalance.Mode;
		_WhiteBalanceCr = (float)pVSC->gImagingSettings.WhiteBalance.CrGain;
		_WhiteBalanceCb = (float)pVSC->gImagingSettings.WhiteBalance.CbGain;

		pVS->token = soap_strdup(soap, pVSC->Vsourcetoken);
		pVS->Framerate = pVSC->Framerate;

		pVS->Resolution = (struct tt__VideoResolution*)soap_malloc(soap, sizeof(struct tt__VideoResolution));
		pVS->Resolution->Width = pVSC->windowwidth;
		pVS->Resolution->Height = pVSC->windowheight;

#if 1
		pVS->Imaging =(struct tt__ImagingSettings*)soap_malloc(soap, sizeof(struct tt__ImagingSettings));
		memset(pVS->Imaging, 0x00, sizeof(struct tt__ImagingSettings));

		pVS->Imaging->BacklightCompensation = (struct tt__BacklightCompensation*)soap_malloc(soap, sizeof(struct tt__BacklightCompensation));
		memset(pVS->Imaging->BacklightCompensation, 0x00, sizeof(struct tt__BacklightCompensation));
		pVS->Imaging->BacklightCompensation->Mode = _Backlight; //Streaming_onvif->BacklightCompensationMode;  //{onv__BacklightCompensationMode__OFF = 0, onv__BacklightCompensationMode__ON = 1}
		pVS->Imaging->BacklightCompensation->Level = 0; //_BacklightCompensationLevel;//float

		pVS->Imaging->Brightness = (float *)soap_malloc(soap,sizeof(float));
		pVS->Imaging->Brightness[0] = _Brightness;

		pVS->Imaging->ColorSaturation = (float *)soap_malloc(soap,sizeof(float));
		pVS->Imaging->ColorSaturation[0] = _Saturation;

		pVS->Imaging->Contrast = (float *)soap_malloc(soap,sizeof(float));
		pVS->Imaging->Contrast[0] = _Contrast;

		pVS->Imaging->Exposure = (struct tt__Exposure *)soap_malloc(soap, sizeof(struct tt__Exposure));
		memset(pVS->Imaging->Exposure, 0x00, sizeof(struct tt__Exposure));
		pVS->Imaging->Exposure->Mode = 0; /* required element of type tt:ExposureMode */
		pVS->Imaging->Exposure->Priority = 0; /* required element of type tt:ExposurePriority */
		pVS->Imaging->Exposure->Window = (struct tt__Rectangle *)soap_malloc(soap, sizeof(struct tt__Rectangle)); /* required element of type tt:Rectangle */
		memset(pVS->Imaging->Exposure->Window, 0x00, sizeof(struct tt__Rectangle));
		pVS->Imaging->Exposure->Window->bottom = (float *)soap_malloc(soap, sizeof(float)); /* optional attribute of type xsd:float */
		pVS->Imaging->Exposure->Window->bottom[0] = 0;
		pVS->Imaging->Exposure->Window->top = (float *)soap_malloc(soap, sizeof(float)); /* optional attribute of type xsd:float */
		pVS->Imaging->Exposure->Window->top[0] = 0;
		pVS->Imaging->Exposure->Window->right = (float *)soap_malloc(soap, sizeof(float)); /* optional attribute of type xsd:float */
		pVS->Imaging->Exposure->Window->right[0] = 0;
		pVS->Imaging->Exposure->Window->left = (float *)soap_malloc(soap, sizeof(float)); /* optional attribute of type xsd:float */
		pVS->Imaging->Exposure->Window->left[0] = 0;
		pVS->Imaging->Exposure->MinExposureTime = 0;
		pVS->Imaging->Exposure->MaxExposureTime = 0;
		pVS->Imaging->Exposure->MinGain = 0;
		pVS->Imaging->Exposure->MaxGain = 0;
		pVS->Imaging->Exposure->MinIris = 0;
		pVS->Imaging->Exposure->MaxIris = 0;
		pVS->Imaging->Exposure->ExposureTime = 0;
		pVS->Imaging->Exposure->Gain = 0;
		pVS->Imaging->Exposure->Iris = 0;

		pVS->Imaging->Focus = (struct tt__FocusConfiguration *)soap_malloc(soap, sizeof(struct tt__FocusConfiguration));
		memset(pVS->Imaging->Focus, 0x00, sizeof(struct tt__FocusConfiguration));
		pVS->Imaging->Focus->AutoFocusMode = 0;
		pVS->Imaging->Focus->DefaultSpeed = 0;
		pVS->Imaging->Focus->NearLimit = 0;
		pVS->Imaging->Focus->FarLimit = 0;
		pVS->Imaging->Focus->__size = 0;
		pVS->Imaging->Focus->__any = NULL;
		pVS->Imaging->Focus->__anyAttribute = NULL;

		pVS->Imaging->IrCutFilter = (enum tt__IrCutFilterMode *)soap_malloc(soap, sizeof(enum tt__IrCutFilterMode));
		pVS->Imaging->IrCutFilter[0] = _IrCutFilterMode; // dummy //{onv__IrCutFilterMode__ON = 0, onv__IrCutFilterMode__OFF = 1, onv__IrCutFilterMode__AUTO = 2}

		pVS->Imaging->Sharpness = (float *)soap_malloc(soap, sizeof(float));
		pVS->Imaging->Sharpness[0] = (float)_Sharpness;

		pVS->Imaging->WideDynamicRange = (struct tt__WideDynamicRange*)soap_malloc(soap, sizeof(struct tt__WideDynamicRange));
		memset(pVS->Imaging->WideDynamicRange, 0x00, sizeof(struct tt__WideDynamicRange));
		pVS->Imaging->WideDynamicRange->Mode = _WideDynamicMode; //{onv__WideDynamicMode__OFF = 0, onv__WideDynamicMode__ON = 1}
		pVS->Imaging->WideDynamicRange->Level = _WideDynamicRange; // dummy float

		pVS->Imaging->WhiteBalance = (struct tt__WhiteBalance*)soap_malloc(soap, sizeof(struct tt__WhiteBalance));
		memset(pVS->Imaging->WhiteBalance, 0x00, sizeof(struct tt__WhiteBalance));
		pVS->Imaging->WhiteBalance->Mode = _WhiteBalance; //{onv__WhiteBalanceMode__AUTO = 0, onv__WhiteBalanceMode__MANUAL = 1}
		pVS->Imaging->WhiteBalance->CrGain = 0; //_WhiteBalanceCr; // dummy
		pVS->Imaging->WhiteBalance->CbGain = 0; //_WhiteBalanceCb; // dummy
		pVS->Imaging->WhiteBalance->__size = 0;
		pVS->Imaging->WhiteBalance->__any = NULL;
		pVS->Imaging->WhiteBalance->__anyAttribute = NULL;

		pVS->Imaging->Extension = NULL;
		pVS->Imaging->__anyAttribute = NULL;
#else
		pVS->Imaging = NULL;
#endif

		pVS->Extension = (struct tt__VideoSourceExtension *)soap_malloc(soap, sizeof(struct tt__VideoSourceExtension));
		memset(pVS->Extension, 0x00, sizeof(struct tt__VideoSourceExtension));
		pVS->Extension->__size = 0; /* sequence of elements <-any> */
		pVS->Extension->__any = NULL;

#if 1
		pVS->Extension->Imaging = (struct tt__ImagingSettings20 *)soap_malloc(soap, sizeof(struct tt__ImagingSettings20));

		pVS->Extension->Imaging->BacklightCompensation = (struct tt__BacklightCompensation20 *)soap_malloc(soap, sizeof(struct tt__BacklightCompensation20));
		switch (MP(MpBackLight)) {
			case MN_BL_OFF:
			case MN_BL_BLC:
			case MN_BL_WDR:
				pVS->Extension->Imaging->BacklightCompensation->Mode = tt__BacklightCompensationMode__OFF;
				break;
			case MN_BL_HLC:
				pVS->Extension->Imaging->BacklightCompensation->Mode = tt__BacklightCompensationMode__ON;
				break;
		}
		pVSCIS->BacklightCompensation.Mode = pVS->Extension->Imaging->BacklightCompensation->Mode;
		pVS->Extension->Imaging->BacklightCompensation->Level = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->BacklightCompensation->Level[0] = pVSCIS->BacklightCompensation.Level = MP(MpHlMaskThrs);

		pVS->Extension->Imaging->Brightness = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Brightness[0] = pVSCIS->Brightness = MP(MpBrightness);

		pVS->Extension->Imaging->ColorSaturation = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->ColorSaturation[0] = pVSCIS->ColorSaturation = MP(MpSaturation);

		pVS->Extension->Imaging->Contrast = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Contrast[0] = pVSCIS->Contrast = MP(MpAce);

		pVS->Extension->Imaging->Exposure = (struct tt__Exposure20 *)soap_malloc(soap, sizeof(struct tt__Exposure20));
		pVS->Extension->Imaging->Exposure->Mode = pVSCIS->Exposure.Mode; // tt__ExposureMode__AUTO, tt__ExposureMode__MANUAL
		pVS->Extension->Imaging->Exposure->Priority = (enum tt__ExposurePriority *)soap_malloc(soap, sizeof(enum tt__ExposurePriority));
		pVS->Extension->Imaging->Exposure->Priority[0] = pVSCIS->Exposure.Priority; // tt__ExposurePriority__LowNoise, tt__ExposurePriority__FrameRate
		pVS->Extension->Imaging->Exposure->Window = (struct tt__Rectangle *)soap_malloc(soap, sizeof(struct tt__Rectangle));
		pVS->Extension->Imaging->Exposure->Window->bottom = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Window->bottom[0] = pVSCIS->Exposure.Window.bottom;
		pVS->Extension->Imaging->Exposure->Window->top = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Window->top[0] = pVSCIS->Exposure.Window.top;
		pVS->Extension->Imaging->Exposure->Window->right = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Window->right[0] = pVSCIS->Exposure.Window.right;
		pVS->Extension->Imaging->Exposure->Window->left = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Window->left[0] = pVSCIS->Exposure.Window.left;
		pVS->Extension->Imaging->Exposure->MinExposureTime = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MinExposureTime[0] = pVSCIS->Exposure.MinExposureTime;
		pVS->Extension->Imaging->Exposure->MaxExposureTime = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MaxExposureTime[0] = pVSCIS->Exposure.MaxExposureTime;
		pVS->Extension->Imaging->Exposure->MinGain = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MinGain[0] = pVSCIS->Exposure.MinGain;
		pVS->Extension->Imaging->Exposure->MaxGain = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MaxGain[0] = pVSCIS->Exposure.MaxGain;
		pVS->Extension->Imaging->Exposure->MinIris = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MinIris[0] = pVSCIS->Exposure.MinIris;
		pVS->Extension->Imaging->Exposure->MaxIris = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->MaxIris[0] = pVSCIS->Exposure.MaxIris;
		pVS->Extension->Imaging->Exposure->ExposureTime = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->ExposureTime[0] = pVSCIS->Exposure.ExposureTime;
		pVS->Extension->Imaging->Exposure->Gain = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Gain[0] = pVSCIS->Exposure.Gain;
		pVS->Extension->Imaging->Exposure->Iris = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Exposure->Iris[0] = pVSCIS->Exposure.Iris;

		pVS->Extension->Imaging->Focus = (struct tt__FocusConfiguration20 *)soap_malloc(soap, sizeof(struct tt__FocusConfiguration20));
		pVS->Extension->Imaging->Focus->AutoFocusMode = pVSCIS->Focus.AutoFocusMode; // tt__AutoFocusMode__AUTO, tt__AutoFocusMode__MANUAL
		pVS->Extension->Imaging->Focus->DefaultSpeed = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Focus->DefaultSpeed[0] = pVSCIS->Focus.DefaultSpeed;
		pVS->Extension->Imaging->Focus->NearLimit = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Focus->NearLimit[0] = pVSCIS->Focus.NearLimit;
		pVS->Extension->Imaging->Focus->FarLimit = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Focus->FarLimit[0] = pVSCIS->Focus.FarLimit;
		pVS->Extension->Imaging->Focus->Extension = NULL;
		pVS->Extension->Imaging->Focus->__anyAttribute = NULL;

		pVS->Extension->Imaging->IrCutFilter = (enum tt__IrCutFilterMode *)soap_malloc(soap, sizeof(enum tt__IrCutFilterMode));
		switch (MP(MpTdn)) {
			case MN_TDN_AUTO:
				pVS->Extension->Imaging->IrCutFilter[0] = tt__IrCutFilterMode__AUTO;
				break;
			case MN_TDN_DAY:
			case MN_TDN_EXTERN:
				pVS->Extension->Imaging->IrCutFilter[0] = tt__IrCutFilterMode__ON;
				break;
			case MN_TDN_NIGHT:
				pVS->Extension->Imaging->IrCutFilter[0] = tt__IrCutFilterMode__OFF;
				break;
		}
		pVSCIS->IrCutFilter = pVS->Extension->Imaging->IrCutFilter[0];

		pVS->Extension->Imaging->Sharpness = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->Sharpness[0] = pVSCIS->Sharpness = MP(MpSharpness);

		pVS->Extension->Imaging->WideDynamicRange = (struct tt__WideDynamicRange20 *)soap_malloc(soap, sizeof(struct tt__WideDynamicRange20));
#if model_2M30p
		switch (MP(FrameMode)) {
			case MN_FRAMEMODE_NORMAL_25_30:
			case MN_FRAMEMODE_DNR_25_30:
				pVS->Extension->Imaging->WideDynamicRange->Mode = tt__WideDynamicMode__OFF;
				break;
			case MN_FRAMEMODE_WDR_12_15:
				pVS->Extension->Imaging->WideDynamicRange->Mode = tt__WideDynamicMode__ON;
				break;
		}
#else
		switch (MP(FrameMode)) {
			case MN_FRAMEMODE_NORMAL_25_30:
			case MN_FRAMEMODE_NORMAL_50_60:
			case MN_FRAMEMODE_DNR_25_30:
			case MN_FRAMEMODE_DNR_50_60:
				pVS->Extension->Imaging->WideDynamicRange->Mode = tt__WideDynamicMode__OFF;
				break;
			case MN_FRAMEMODE_WDR_12_15:
			case MN_FRAMEMODE_WDR_25_30:
				pVS->Extension->Imaging->WideDynamicRange->Mode = tt__WideDynamicMode__ON;
				break;
		}
#endif
		pVSCIS->WideDynamicRange.Mode = pVS->Extension->Imaging->WideDynamicRange->Mode;
		pVS->Extension->Imaging->WideDynamicRange->Level = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->WideDynamicRange->Level[0] = pVSCIS->WideDynamicRange.Level = MP(MpFrameRateLv);

		pVS->Extension->Imaging->WhiteBalance = (struct tt__WhiteBalance20 *)soap_malloc(soap, sizeof(struct tt__WhiteBalance20));
		switch (MP(MpAwb)) {
			case MN_AWB_AUTO:
			case MN_AWB_AUTOEXT:
			case MN_AWB_PRESET:
				pVS->Extension->Imaging->WhiteBalance->Mode = tt__WhiteBalanceMode__AUTO;
				break;
			case MN_AWB_MNL:
				pVS->Extension->Imaging->WhiteBalance->Mode = tt__WhiteBalanceMode__MANUAL;
				break;
		}
		pVSCIS->WhiteBalance.Mode = pVS->Extension->Imaging->WhiteBalance->Mode;
		pVS->Extension->Imaging->WhiteBalance->CrGain = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->WhiteBalance->CrGain[0] = pVSCIS->WhiteBalance.CrGain = MP(MpRgain);
		pVS->Extension->Imaging->WhiteBalance->CbGain = (float *)soap_malloc(soap, sizeof(float));
		pVS->Extension->Imaging->WhiteBalance->CbGain[0] = pVSCIS->WhiteBalance.CbGain = MP(MpBgain);
		pVS->Extension->Imaging->WhiteBalance->Extension = NULL;
		pVS->Extension->Imaging->WhiteBalance->__anyAttribute = NULL;

		pVS->Extension->Imaging->Extension = NULL;
		pVS->Extension->Imaging->__anyAttribute = NULL;
#else
		pVS->Extension->Imaging = NULL;
#endif

		pVS->Extension->Extension = NULL;	/* optional element of type tt:VideoSourceExtension2 */

		pVS->__anyAttribute = NULL;
	}	

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetVideoSources);
}

/** Auto-test server operation __trt__GetAudioSources */
int __trt__GetAudioSources(struct soap *soap, 
struct _trt__GetAudioSources *trt__GetAudioSources, 
struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int idxASC;

	trt__GetAudioSourcesResponse->__sizeAudioSources = oSysInfo->SizeDefaultASC;

	trt__GetAudioSourcesResponse->AudioSources = (struct tt__AudioSource *)soap_malloc(soap, sizeof(struct tt__AudioSource) * trt__GetAudioSourcesResponse->__sizeAudioSources);
	memset(trt__GetAudioSourcesResponse->AudioSources, 0x00, sizeof(struct tt__AudioSource) * trt__GetAudioSourcesResponse->__sizeAudioSources);

	for (idxASC = 0; idxASC < trt__GetAudioSourcesResponse->__sizeAudioSources; idxASC++) {
		struct tt__AudioSource *pAS = &(trt__GetAudioSourcesResponse->AudioSources[idxASC]);
		ASC_t *pASC = &(oSysInfo->ASC[idxASC]);

		pAS->token = soap_strdup(soap, pASC->Asourcetoken);
		pAS->Channels = 1;
		pAS->__size = 0;
		pAS->__any = NULL;
		pAS->__anyAttribute = NULL;
	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioSourcesResponse);
#endif
	UNUSED(trt__GetAudioSources);
}


/** Auto-test server operation __trt__GetAudioOutputs */
int __trt__GetAudioOutputs(struct soap *soap, 
struct _trt__GetAudioOutputs *trt__GetAudioOutputs, 
struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();

	trt__GetAudioOutputsResponse->__sizeAudioOutputs = 1;

	trt__GetAudioOutputsResponse->AudioOutputs = (struct tt__AudioOutput *)soap_malloc(soap, sizeof(struct tt__AudioOutput));
	memset(trt__GetAudioOutputsResponse->AudioOutputs, 0x00, sizeof(struct tt__AudioOutput));
	struct tt__AudioOutput *pAO = trt__GetAudioOutputsResponse->AudioOutputs;
	AOC_t *pAOC = &(oSysInfo->AOC);

	pAO->token = soap_strdup(soap, pAOC->AOutputToken);
	pAO->__size = 0;
	pAO->__any = NULL;
	pAO->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioOutputsResponse);
#endif
	UNUSED(trt__GetAudioOutputs);
}

/** 
 * @brief Create Profile
 * @This operation creates a new empty media profile.
 *   
 * @param Request	:_trt__CreateProfile structure contains  the friendly Name of the Profile to create as well as an optional Token 
 parameter, specifying the unique identifier of the new media profile.
 @param Response	:_trt__CreateProfileResponse structure contains an empty Profile structure with no configuration entities.

 * @return               : On success - SOAP_OK.
 : On failure - A profile with the token ProfileToken already exists. or
 The maximum number of supported profiles has been reached.

 */

int __trt__CreateProfile(struct soap *soap, 
struct _trt__CreateProfile *trt__CreateProfile, 
struct _trt__CreateProfileResponse *trt__CreateProfileResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int num;

	/* check the limit on number of profiles */
	num = oSysInfo->nProfileSize;		// total number of profiles existing in the memory
	if ((num + 1) > MAX_PROFILE) {
		onvifInterfaceFault(soap, ovfSC_MaxNVTProfiles, "");
		LEAVE();
		return SOAP_FAULT;
	}

	oSysInfo->nProfileSize++;

	flprintf("nProfileSize(%d) num(%d)\r\n", oSysInfo->nProfileSize, num);
	flprintf("New Profile Name(%s) Token(%s)\r\n", trt__CreateProfile->Name, trt__CreateProfile->Token);

	/* save profile */
	oSysInfo->Oprofile[num].fix = 0; 
	oSysInfo->Oprofile[num].useVSC = 0;
	oSysInfo->Oprofile[num].useVEC = 0;
	oSysInfo->Oprofile[num].useVOC = 0;
	oSysInfo->Oprofile[num].useVAC = 0;
	oSysInfo->Oprofile[num].useASC = 0;
	oSysInfo->Oprofile[num].useAEC = 0;
	oSysInfo->Oprofile[num].useAOC = 0;
	oSysInfo->Oprofile[num].useADC = 0;
	oSysInfo->Oprofile[num].useMDC = 0;

	if (trt__CreateProfile->Name)
		strcpy(oSysInfo->Oprofile[num].profilename, trt__CreateProfile->Name);
	if (trt__CreateProfile->Token)
		strcpy(oSysInfo->Oprofile[num].profiletoken, trt__CreateProfile->Token);
	else
		strcpy(oSysInfo->Oprofile[num].profiletoken, trt__CreateProfile->Name);

//	snprintf(oSysInfo->Oprofile[num].VSCtoken, SMALL_INFO_LENGTH, "VideoToken1");
//	snprintf(oSysInfo->Oprofile[num].VECtoken, SMALL_INFO_LENGTH, "VideoEncoderToken1");
//	strcpy(oSysInfo->Oprofile[num].VOCtoken, "testvoctoken");
//	strcpy(oSysInfo->Oprofile[num].MDCtoken, "metaDataConfToken");
//	oSysInfo->VSC[0].Vcount++;
//	oSysInfo->VEC[0].VEusercount++;
//	flprintf("ADD %s/%s VideoSource(%d) VideoEncoder(%d)\r\n", trt__CreateProfile->Name, trt__CreateProfile->Token, oSysInfo->VSC[0].Vcount, oSysInfo->VEC[0].VEusercount);
//
//	Oprofile_t profile_acount;
//	int ret = ControlSystemData(SFIELD_ADD_PROFILE, (void *)&profile_acount, sizeof(Oprofile_t));      
//	if (ret == -1) {
//		onvif_fault(soap, "","ter:InvalidArgVal", "ter:AddProfileFailed","","");
//		onvifInterfaceFault(soap, , "");
//		LEAVE();
//		return SOAP_FAULT;
//	}

	trt__CreateProfileResponse->Profile = (struct tt__Profile *)soap_malloc(soap, sizeof(struct tt__Profile));
	memset(trt__CreateProfileResponse->Profile, 0x00, sizeof(struct tt__Profile));

	trt__CreateProfileResponse->Profile->Name = soap_strdup(soap, oSysInfo->Oprofile[num].profilename);
	trt__CreateProfileResponse->Profile->token = soap_strdup(soap, oSysInfo->Oprofile[num].profiletoken);
	trt__CreateProfileResponse->Profile->fixed = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	trt__CreateProfileResponse->Profile->fixed[0] = xsd__boolean__false_; 
	trt__CreateProfileResponse->Profile->VideoSourceConfiguration = NULL;
	trt__CreateProfileResponse->Profile->AudioSourceConfiguration = NULL;
	trt__CreateProfileResponse->Profile->VideoEncoderConfiguration = NULL;
	trt__CreateProfileResponse->Profile->AudioEncoderConfiguration = NULL;
	trt__CreateProfileResponse->Profile->VideoAnalyticsConfiguration = NULL;
	trt__CreateProfileResponse->Profile->PTZConfiguration = NULL;
	trt__CreateProfileResponse->Profile->MetadataConfiguration = NULL;
	trt__CreateProfileResponse->Profile->Extension = NULL;
	trt__CreateProfileResponse->Profile->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

/**
 * @brief Get Profile 
 * @This operation fetches the whole profile configuration. 
 *   
 * @param Request	:_trt__GetProfile structure contains the token to the requested profile.       
 * @param Response	:_trt__GetProfileResponse structure contains the Profile indicated by the Token parameter. 
 * @return               : On success -  SOAP_OK.
 : On failure - The requested profile token ProfileToken does not exist.
 */

int __trt__GetProfile(struct soap *soap, 
struct _trt__GetProfile *trt__GetProfile, 
struct _trt__GetProfileResponse *trt__GetProfileResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	char _IPAddr[SMALL_INFO_LENGTH];
	int i = 0;
	int index =0;
	int Ptoken_status = NOT_EXIST;
	int idxVSC, idxVEC;
	VSC_t *pVSC;
	VEC_t *pVEC;
#ifdef __AUDIO__
	int idxASC, idxAEC;
	ASC_t *pASC;
	AEC_t *pAEC;
#if (AUDIO_RX==1)
	AOC_t *pAOC;
	ADC_t *pADC;
#endif
#endif

	// Search ProfileToken
	if (trt__GetProfile) {
		flprintf("Get %s\r\n", trt__GetProfile->ProfileToken == NULL ? "NULL!!!!!" : trt__GetProfile->ProfileToken);
		if ((trt__GetProfile->ProfileToken) == NULL) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}
		if (strcmp(trt__GetProfile->ProfileToken, "") == 0) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}

		/* Check if ProfileToken Exist or Not */
		for (i = 0; i < oSysInfo->nProfileSize; i++) {
			if (strcmp(trt__GetProfile->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_status = EXIST;
				index = i;
				break;
			}
		}
	}

	if (Ptoken_status == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "<env:Text>can not find this profile</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}

	idxVSC = MatchProfileVSCToken(oSysInfo->Oprofile[index].VSCtoken);
	idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[index].VECtoken);
#ifdef __AUDIO__
	idxASC = MatchProfileASCToken(oSysInfo->Oprofile[index].ASCtoken);
	idxAEC = MatchProfileAECToken(oSysInfo->Oprofile[index].AECtoken);
#endif

	if (idxVSC < 0 || idxVEC < 0
#ifdef __AUDIO__
		|| idxASC < 0 || idxAEC < 0
#endif
	) {
		printf("%s(%d) : Unknown index idxVSC(%d) idxVEC(%d)\r\n", __func__, __LINE__, idxVSC, idxVEC);
#ifdef __AUDIO__
		printf("%s(%d) : Unknown index idxASC(%d) idxAEC(%d)\r\n", __func__, __LINE__, idxASC, idxAEC);
#endif
		while(1);
	}

	pVSC = &(oSysInfo->VSC[idxVSC]);
	userVSCupdate(pVSC);

	pVEC = &(oSysInfo->VEC[idxVEC]);
	userVECupdate(pVEC, idxVEC);

#ifdef __AUDIO__
	pASC = &(oSysInfo->ASC[idxASC]);
	pAEC = &(oSysInfo->AEC[idxAEC]);
#if (AUDIO_RX==1)
	pAOC = &(oSysInfo->AOC);
	pADC = &(oSysInfo->ADC);
#endif
#endif

////////////////////////////////////////////////////////////////////////////////

	trt__GetProfileResponse->Profile = (struct tt__Profile *)soap_malloc(soap, sizeof(struct tt__Profile));
	memset(trt__GetProfileResponse->Profile, 0x00, sizeof(struct tt__Profile));
	struct tt__Profile *pProfile = trt__GetProfileResponse->Profile;

	pProfile->Name = soap_strdup(soap, oSysInfo->Oprofile[index].profilename);	
	pProfile->token = soap_strdup(soap, oSysInfo->Oprofile[index].profiletoken);	
	pProfile->fixed = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean)); 
	pProfile->fixed[0] = oSysInfo->Oprofile[index].fix;

	/* VideoSourceConfiguration */
	pProfile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	memset(pProfile->VideoSourceConfiguration, 0x00, sizeof(struct tt__VideoSourceConfiguration));
	pProfile->VideoSourceConfiguration->Name = soap_strdup(soap, pVSC->Vname);
	pProfile->VideoSourceConfiguration->UseCount = pVSC->Vcount;
	pProfile->VideoSourceConfiguration->token = soap_strdup(soap, pVSC->Vtoken);
	pProfile->VideoSourceConfiguration->SourceToken = soap_strdup(soap, pVSC->Vsourcetoken);

	pProfile->VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_malloc(soap, sizeof(struct tt__IntRectangle));
	memset(pProfile->VideoSourceConfiguration->Bounds, 0x00, sizeof(struct tt__IntRectangle));
	pProfile->VideoSourceConfiguration->Bounds->y = pVSC->windowy;
	pProfile->VideoSourceConfiguration->Bounds->x = pVSC->windowx;
	pProfile->VideoSourceConfiguration->Bounds->width = pVSC->windowwidth;
	pProfile->VideoSourceConfiguration->Bounds->height = pVSC->windowheight;
	pProfile->VideoSourceConfiguration->__size = 1;
	pProfile->VideoSourceConfiguration->__any = NULL;
	pProfile->VideoSourceConfiguration->Extension = NULL;
	pProfile->VideoSourceConfiguration->__anyAttribute = NULL;

	/* AudioSourceConfiguration */
#ifdef __AUDIO__
	pProfile->AudioSourceConfiguration = (struct tt__AudioSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
	memset(pProfile->AudioSourceConfiguration, 0x00, sizeof(struct tt__AudioSourceConfiguration));
	pProfile->AudioSourceConfiguration->Name = soap_strdup(soap, pASC->Aname);
	pProfile->AudioSourceConfiguration->UseCount = pASC->Acount;
	pProfile->AudioSourceConfiguration->token = soap_strdup(soap, pASC->Atoken);
	pProfile->AudioSourceConfiguration->SourceToken = soap_strdup(soap, pASC->Asourcetoken);
	pProfile->AudioSourceConfiguration->__size = 0;
	pProfile->AudioSourceConfiguration->__any = NULL;
	pProfile->AudioSourceConfiguration->__anyAttribute = NULL;
#else
	pProfile->AudioSourceConfiguration = NULL;
#endif

	/*VideoEncoderConfiguration */
	pProfile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
	memset(pProfile->VideoEncoderConfiguration, 0x00, sizeof(struct tt__VideoEncoderConfiguration));

	pProfile->VideoEncoderConfiguration->Name = soap_strdup(soap, pVEC->VEname);
	pProfile->VideoEncoderConfiguration->UseCount = pVEC->VEusercount;
	pProfile->VideoEncoderConfiguration->token = soap_strdup(soap, pVEC->VEtoken);
	pProfile->VideoEncoderConfiguration->Encoding = pVEC->Vencoder;

	pProfile->VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	memset(pProfile->VideoEncoderConfiguration->Resolution, 0x00, sizeof(struct tt__VideoResolution));
	pProfile->VideoEncoderConfiguration->Resolution->Width = pVEC->Rwidth;
	pProfile->VideoEncoderConfiguration->Resolution->Height = pVEC->Rheight;

	pProfile->VideoEncoderConfiguration->Quality = pVEC->VEquality;
	
	pProfile->VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	memset(pProfile->VideoEncoderConfiguration->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
	pProfile->VideoEncoderConfiguration->RateControl->FrameRateLimit =  oget_framerate2(pVEC->frameratelimit);   
	pProfile->VideoEncoderConfiguration->RateControl->EncodingInterval =  pVEC->encodinginterval;

	pProfile->VideoEncoderConfiguration->MPEG4 = NULL;
	pProfile->VideoEncoderConfiguration->H264 = NULL;

	switch (pProfile->VideoEncoderConfiguration->Encoding) {
		case tt__VideoEncoding__JPEG:
			pProfile->VideoEncoderConfiguration->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);
			break;

		case tt__VideoEncoding__H264:
			pProfile->VideoEncoderConfiguration->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
			pProfile->VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
			memset(pProfile->VideoEncoderConfiguration->H264, 0x00, sizeof(struct tt__H264Configuration));
			pProfile->VideoEncoderConfiguration->H264->GovLength = pVEC->H264govlength;
			pProfile->VideoEncoderConfiguration->H264->H264Profile = pVEC->H264Profile;
			break;

		case tt__VideoEncoding__MPEG4:
		default:
			pProfile->VideoEncoderConfiguration->RateControl->BitrateLimit = 0;
			printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
			break;
	}

#if 1
	pProfile->VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(pProfile->VideoEncoderConfiguration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	pProfile->VideoEncoderConfiguration->Multicast->Port =  pVEC->Multicast_Conf.port;
	pProfile->VideoEncoderConfiguration->Multicast->TTL =  pVEC->Multicast_Conf.ttl;
	pProfile->VideoEncoderConfiguration->Multicast->AutoStart =  pVEC->Multicast_Conf.autostart;
	
	pProfile->VideoEncoderConfiguration->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(pProfile->VideoEncoderConfiguration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	pProfile->VideoEncoderConfiguration->Multicast->Address->Type =  pVEC->Multicast_Conf.nIPType;

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	pProfile->VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	memset(pProfile->VideoEncoderConfiguration->Multicast->Address->IPv4Address, 0x00, sizeof(char *));
	pProfile->VideoEncoderConfiguration->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);

	pProfile->VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
	pProfile->VideoEncoderConfiguration->Multicast->__size = 0;
	pProfile->VideoEncoderConfiguration->Multicast->__any = NULL;
	pProfile->VideoEncoderConfiguration->Multicast->__anyAttribute = NULL;
#else
	pProfile->VideoEncoderConfiguration->Multicast = NULL;
#endif

	pProfile->VideoEncoderConfiguration->SessionTimeout = pVEC->sessiontimeout;
	pProfile->VideoEncoderConfiguration->__size = 0;
	pProfile->VideoEncoderConfiguration->__any = NULL;
	pProfile->VideoEncoderConfiguration->__anyAttribute = NULL;

	/* AudioEncoderConfiguration */
#ifdef __AUDIO__
	pProfile->AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration)); 
	memset(pProfile->AudioEncoderConfiguration, 0x00, sizeof(struct tt__AudioEncoderConfiguration));

	pProfile->AudioEncoderConfiguration->Name = soap_strdup(soap, pAEC->AEname);
	pProfile->AudioEncoderConfiguration->UseCount = pAEC->AEusercount;
	pProfile->AudioEncoderConfiguration->token = soap_strdup(soap, pAEC->AEtoken);
	pProfile->AudioEncoderConfiguration->Encoding = pAEC->Aencoder;
	pProfile->AudioEncoderConfiguration->Bitrate = pAEC->Abitratelimit;
	pProfile->AudioEncoderConfiguration->SampleRate = pAEC->Asamplerate;

#if 1
	pProfile->AudioEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(pProfile->AudioEncoderConfiguration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	pProfile->AudioEncoderConfiguration->Multicast->Port = pAEC->Multicast_Conf.port;
	pProfile->AudioEncoderConfiguration->Multicast->TTL = pAEC->Multicast_Conf.ttl;
	pProfile->AudioEncoderConfiguration->Multicast->AutoStart =  pAEC->Multicast_Conf.autostart;

	pProfile->AudioEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(pProfile->AudioEncoderConfiguration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	pProfile->AudioEncoderConfiguration->Multicast->Address->Type = pAEC->Multicast_Conf.nIPType;

	pProfile->AudioEncoderConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	memset(pProfile->AudioEncoderConfiguration->Multicast->Address->IPv4Address, 0x00, sizeof(char *));
	pProfile->AudioEncoderConfiguration->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);

	pProfile->AudioEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
	pProfile->AudioEncoderConfiguration->Multicast->__size = 0;
	pProfile->AudioEncoderConfiguration->Multicast->__any = NULL;
	pProfile->AudioEncoderConfiguration->Multicast->__anyAttribute = NULL;
#else
	pProfile->AudioEncoderConfiguration->Multicast = NULL;
#endif

	pProfile->AudioEncoderConfiguration->SessionTimeout = pAEC->sessiontimeout;
	pProfile->AudioEncoderConfiguration->__size = 0;
	pProfile->AudioEncoderConfiguration->__any = NULL;
	pProfile->AudioEncoderConfiguration->__anyAttribute = NULL;
#else
	pProfile->AudioEncoderConfiguration = NULL;
#endif

	/* VideoAnalyticsConfiguration */
#if 0
	pProfile->VideoAnalyticsConfiguration = (struct tt__VideoAnalyticsConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoAnalyticsConfiguration));
	memset(pProfile->VideoAnalyticsConfiguration, 0x00, sizeof(struct tt__VideoAnalyticsConfiguration));
	pProfile->VideoAnalyticsConfiguration->Name = soap_strdup(soap, "video_analytics_config");
	//pProfile->VideoAnalyticsConfiguration->Name = (char*) soap_malloc(soap, sizeof(char)*30);
	//strcpy(pProfile->VideoAnalyticsConfiguration->Name, "video_analytics_config");
	
	pProfile->VideoAnalyticsConfiguration->UseCount = 1;
	pProfile->VideoAnalyticsConfiguration->token = soap_strdup(soap, "video_analytics_config");
	//pProfile->VideoAnalyticsConfiguration->token = (char*) soap_malloc(soap, sizeof(char)*30);
	//strcpy(pProfile->VideoAnalyticsConfiguration->token, "video_analytics_config");

	
	pProfile->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration = (struct tt__AnalyticsEngineConfiguration *)soap_malloc(soap, sizeof(struct tt__AnalyticsEngineConfiguration));
	memset(pProfile->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration, 0x00, sizeof(struct tt__AnalyticsEngineConfiguration));
	pProfile->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->__sizeAnalyticsModule = 0;
	pProfile->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->AnalyticsModule = NULL;
	pProfile->VideoAnalyticsConfiguration->AnalyticsEngineConfiguration->Extension = NULL;
	pProfile->VideoAnalyticsConfiguration->RuleEngineConfiguration = (struct tt__RuleEngineConfiguration *)soap_malloc(soap, sizeof(struct tt__RuleEngineConfiguration));
	memset(pProfile->VideoAnalyticsConfiguration->RuleEngineConfiguration, 0x00, sizeof(struct tt__RuleEngineConfiguration));
	pProfile->VideoAnalyticsConfiguration->RuleEngineConfiguration->__sizeRule = 0;
	pProfile->VideoAnalyticsConfiguration->RuleEngineConfiguration->Rule = NULL;
	pProfile->VideoAnalyticsConfiguration->RuleEngineConfiguration->Extension = NULL;
#else
	pProfile->VideoAnalyticsConfiguration = NULL;
#endif

	/* PTZConfiguration */
#if 0
	PTZC_t *pPTZC = &(oSysInfo->PTZC);

	pProfile->PTZConfiguration = (struct tt__PTZConfiguration *)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
	memset(pProfile->PTZConfiguration, 0x00, sizeof(struct tt__PTZConfiguration *));

	struct tt__PTZConfiguration *PTZConf = pProfile->PTZConfiguration;
	PTZConf->UseCount = oSysInfo->PTZC.PTZusecount;	
	PTZConf->NodeToken = soap_strdup(soap, oSysInfo->PTZC.Node[0].Token);
	PTZConf->Name = soap_strdup(soap, oSysInfo->PTZC.PTZname); 		
	PTZConf->token = soap_strdup(soap, oSysInfo->PTZC.PTZtoken);
	
	PTZConf->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace);
	PTZConf->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsoluteZoomPositionSpace);
	PTZConf->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativePanTiltTranslationSpace);
	PTZConf->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativeZoomTranslationSpace);
	PTZConf->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace);
	PTZConf->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousZoomVelocitySpace);

	PTZConf->DefaultPTZSpeed = (struct tt__PTZSpeed *)soap_malloc(soap, sizeof(struct tt__PTZSpeed));
	memset(PTZConf->DefaultPTZSpeed, 0x00, sizeof(struct tt__PTZSpeed));

	PTZConf->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));
	memset(PTZConf->DefaultPTZSpeed->PanTilt, 0x00, sizeof(struct tt__Vector2D));
	PTZConf->DefaultPTZSpeed->PanTilt->x = oSysInfo->PTZC.DefaultPantilt_x;
	PTZConf->DefaultPTZSpeed->PanTilt->y = oSysInfo->PTZC.DefaultPantilt_y;
	PTZConf->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap,oSysInfo->PTZC.DefaultPantilt_space);
	PTZConf->DefaultPTZSpeed->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));
	memset(PTZConf->DefaultPTZSpeed->Zoom, 0x00, sizeof(struct tt__Vector1D));
	PTZConf->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.DefaultZoom_space);
	PTZConf->DefaultPTZSpeed->Zoom->x = oSysInfo->PTZC.DefaultZoom_x;

	PTZConf->DefaultPTZTimeout = (LONG64 *)soap_malloc(soap, sizeof(LONG64));
	memset(PTZConf->DefaultPTZTimeout, 0x00, sizeof(LONG64));
	*PTZConf->DefaultPTZTimeout = oSysInfo->PTZC.DefaultPTZTimeout;
	
	PTZConf->PanTiltLimits = (struct tt__PanTiltLimits *)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));
	memset(PTZConf->PanTiltLimits, 0x00, sizeof(struct tt__PanTiltLimits));
	PTZConf->PanTiltLimits->Range = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));
	memset(PTZConf->PanTiltLimits->Range, 0x00, sizeof(struct tt__Space2DDescription));
	flprintf("PanTiltLimits (%s)\r\n", oSysInfo->PTZC.PanTiltLimit_URI);
	PTZConf->PanTiltLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.PanTiltLimit_URI);
	PTZConf->PanTiltLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(PTZConf->PanTiltLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	PTZConf->PanTiltLimits->Range->XRange->Max = oSysInfo->PTZC.PanTiltLimit_XRange_Max;
	PTZConf->PanTiltLimits->Range->XRange->Min = oSysInfo->PTZC.PanTiltLimit_XRange_Min;
	PTZConf->PanTiltLimits->Range->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(PTZConf->PanTiltLimits->Range->YRange, 0x00, sizeof(struct tt__FloatRange));
	PTZConf->PanTiltLimits->Range->YRange->Max = oSysInfo->PTZC.PanTiltLimit_YRange_Max;
	PTZConf->PanTiltLimits->Range->YRange->Min = oSysInfo->PTZC.PanTiltLimit_YRange_Min;
	
	PTZConf->ZoomLimits = (struct tt__ZoomLimits *)soap_malloc(soap, sizeof(struct tt__ZoomLimits));
	memset(PTZConf->ZoomLimits, 0x00, sizeof(struct tt__ZoomLimits));
	PTZConf->ZoomLimits->Range = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));
	memset(PTZConf->ZoomLimits->Range, 0x00, sizeof(struct tt__Space1DDescription));
	PTZConf->ZoomLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.ZoomLimit_XRange_URI);
	
	PTZConf->ZoomLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(PTZConf->ZoomLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	PTZConf->ZoomLimits->Range->XRange->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	PTZConf->ZoomLimits->Range->XRange->Min= oSysInfo->PTZC.ZoomLimit_XRange_Min;
	PTZConf->Extension = NULL;
	PTZConf->__anyAttribute = NULL;
#else
#if 1
	PTZC_t *pPTZC = &(oSysInfo->PTZC);

	pProfile->PTZConfiguration = (struct tt__PTZConfiguration *)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
	memset(pProfile->PTZConfiguration, 0x00, sizeof(struct tt__PTZConfiguration *));
	dvprintf("pPTZC %d\r\n",__LINE__);
	
	pProfile->PTZConfiguration->UseCount = pPTZC->PTZusecount;	
	pProfile->PTZConfiguration->Name = soap_strdup(soap, pPTZC->PTZname); 
	pProfile->PTZConfiguration->token = soap_strdup(soap, pPTZC->PTZtoken);
	pProfile->PTZConfiguration->NodeToken = soap_strdup(soap, pPTZC->Node[0].Token);
	pProfile->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, pPTZC->DefaultAbsolutePantTiltPositionSpace);
	pProfile->PTZConfiguration->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, pPTZC->DefaultAbsoluteZoomPositionSpace);
	pProfile->PTZConfiguration->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, pPTZC->DefaultRelativePanTiltTranslationSpace);
	pProfile->PTZConfiguration->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, pPTZC->DefaultRelativeZoomTranslationSpace);
	pProfile->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, pPTZC->DefaultContinuousPanTiltVelocitySpace);
	pProfile->PTZConfiguration->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, pPTZC->DefaultContinuousZoomVelocitySpace);
	dvprintf("pPTZC %d\r\n",__LINE__);
	
	pProfile->PTZConfiguration->DefaultPTZSpeed = (struct tt__PTZSpeed *)soap_malloc(soap, sizeof(struct tt__PTZSpeed));
	memset(pProfile->PTZConfiguration->DefaultPTZSpeed, 0x00, sizeof(struct tt__PTZSpeed *));
	dvprintf("pPTZC %d\r\n",__LINE__);
	pProfile->PTZConfiguration->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));
	memset(pProfile->PTZConfiguration->DefaultPTZSpeed->PanTilt, 0x00, sizeof(struct tt__Vector2D *));
	pProfile->PTZConfiguration->DefaultPTZSpeed->PanTilt->x = pPTZC->DefaultPantilt_x;
	pProfile->PTZConfiguration->DefaultPTZSpeed->PanTilt->y = pPTZC->DefaultPantilt_y;
	pProfile->PTZConfiguration->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap, pPTZC->DefaultPantilt_space);
	dvprintf("pPTZC %d\r\n",__LINE__);
	pProfile->PTZConfiguration->DefaultPTZSpeed->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));
	memset(pProfile->PTZConfiguration->DefaultPTZSpeed->Zoom, 0x00, sizeof(struct tt__Vector1D *));
	pProfile->PTZConfiguration->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, pPTZC->DefaultZoom_space);
	pProfile->PTZConfiguration->DefaultPTZSpeed->Zoom->x = pPTZC->DefaultZoom_x;
	dvprintf("pPTZC %d\r\n",__LINE__);
	
	pProfile->PTZConfiguration->DefaultPTZTimeout = (LONG64 *)soap_malloc(soap, sizeof(LONG64));
	memset(pProfile->PTZConfiguration->DefaultPTZTimeout, 0x00, sizeof(LONG64));
	*pProfile->PTZConfiguration->DefaultPTZTimeout = pPTZC->DefaultPTZTimeout;
	
	pProfile->PTZConfiguration->PanTiltLimits = (struct tt__PanTiltLimits *)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));
	memset(pProfile->PTZConfiguration->PanTiltLimits, 0x00, sizeof(struct tt__PanTiltLimits));
	pProfile->PTZConfiguration->PanTiltLimits->Range = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));
	memset(pProfile->PTZConfiguration->PanTiltLimits->Range, 0x00, sizeof(struct tt__Space2DDescription));
	pProfile->PTZConfiguration->PanTiltLimits->Range->URI = soap_strdup(soap, pPTZC->PanTiltLimit_URI);
	pProfile->PTZConfiguration->PanTiltLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(pProfile->PTZConfiguration->PanTiltLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	pProfile->PTZConfiguration->PanTiltLimits->Range->XRange->Max = pPTZC->PanTiltLimit_XRange_Max;
	pProfile->PTZConfiguration->PanTiltLimits->Range->XRange->Min = pPTZC->PanTiltLimit_XRange_Min;
	pProfile->PTZConfiguration->PanTiltLimits->Range->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(pProfile->PTZConfiguration->PanTiltLimits->Range->YRange, 0x00, sizeof(struct tt__FloatRange));
	pProfile->PTZConfiguration->PanTiltLimits->Range->YRange->Max = pPTZC->PanTiltLimit_XRange_Max;
	pProfile->PTZConfiguration->PanTiltLimits->Range->YRange->Min = pPTZC->PanTiltLimit_XRange_Min;
	dvprintf("pPTZC %d\r\n",__LINE__);	
	
	pProfile->PTZConfiguration->ZoomLimits = (struct tt__ZoomLimits *)soap_malloc(soap, sizeof(struct tt__ZoomLimits));
	memset(pProfile->PTZConfiguration->ZoomLimits, 0x00, sizeof(struct tt__ZoomLimits));
	pProfile->PTZConfiguration->ZoomLimits->Range = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));
	memset(pProfile->PTZConfiguration->ZoomLimits->Range, 0x00, sizeof(struct tt__Space1DDescription));
	pProfile->PTZConfiguration->ZoomLimits->Range->URI = soap_strdup(soap, pPTZC->ZoomLimit_XRange_URI);
	dvprintf("pPTZC %d\r\n",__LINE__);
	pProfile->PTZConfiguration->ZoomLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(pProfile->PTZConfiguration->ZoomLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	pProfile->PTZConfiguration->ZoomLimits->Range->XRange->Max = pPTZC->ZoomLimit_XRange_Max;
	pProfile->PTZConfiguration->ZoomLimits->Range->XRange->Min= pPTZC->ZoomLimit_XRange_Min;
	dvprintf("pPTZC %d\r\n",__LINE__);
	pProfile->PTZConfiguration->Extension = NULL;
	pProfile->PTZConfiguration->__anyAttribute = NULL;
	dvprintf("pPTZC %d\r\n",__LINE__);
#else
	pProfile->PTZConfiguration = NULL;
#endif
#endif




	/* MetadataConfiguration */
#if 0
	pProfile->MetadataConfiguration = (struct tt__MetadataConfiguration *)soap_malloc(soap, sizeof(struct tt__MetadataConfiguration));
	memset(pProfile->MetadataConfiguration, 0x00, sizeof(struct tt__MetadataConfiguration));

	pProfile->MetadataConfiguration->Name = soap_strdup(soap, GetSysInfo()->Oprofile[index].MDC.MDname);
	//pProfile->MetadataConfiguration->Name = (char*) soap_malloc(soap, sizeof(char)*30);
	//strcpy(pProfile->MetadataConfiguration->Name, GetSysInfo()->Oprofile[index].MDC.MDname);
	
	pProfile->MetadataConfiguration->token = soap_strdup(soap, GetSysInfo()->Oprofile[index].MDC.MDtoken);
	//pProfile->MetadataConfiguration->token = (char*) soap_malloc(soap, sizeof(char)*30);
	//strcpy(pProfile->MetadataConfiguration->token, GetSysInfo()->Oprofile[index].MDC.MDtoken);
	pProfile->MetadataConfiguration->UseCount =  GetSysInfo()->Oprofile[index].MDC.MDusecount;//1;
	pProfile->MetadataConfiguration->Analytics =  GetSysInfo()->Oprofile[index].MDC.MDanalytics;//0;
	pProfile->MetadataConfiguration->SessionTimeout = GetSysInfo()->Oprofile[index].MDC.sessiontimeout; 
	pProfile->MetadataConfiguration->PTZStatus = NULL;
	pProfile->MetadataConfiguration->Events = NULL;
	pProfile->MetadataConfiguration->AnalyticsEngineConfiguration = NULL;
	pProfile->MetadataConfiguration->Extension = NULL;
	pProfile->MetadataConfiguration->__size = 0;
	pProfile->MetadataConfiguration->__any = NULL;
	pProfile->MetadataConfiguration->__anyAttribute = NULL;

	pProfile->MetadataConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(pProfile->MetadataConfiguration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	pProfile->MetadataConfiguration->Multicast->Port =  GetSysInfo()->Oprofile[index].MDC.VMC.port;
	pProfile->MetadataConfiguration->Multicast->TTL =  GetSysInfo()->Oprofile[index].MDC.VMC.ttl;
	pProfile->MetadataConfiguration->Multicast->AutoStart =  GetSysInfo()->Oprofile[index].MDC.VMC.autostart;//false
	
	pProfile->MetadataConfiguration->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(pProfile->MetadataConfiguration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	pProfile->MetadataConfiguration->Multicast->Address->Type =  GetSysInfo()->Oprofile[index].MDC.VMC.nIPType;
	
	pProfile->MetadataConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));	
	
	pProfile->MetadataConfiguration->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
	//pProfile->MetadataConfiguration->Multicast->Address->IPv4Address[0] = (char*) soap_malloc(soap, sizeof(char)*50);
	//strcpy(pProfile->MetadataConfiguration->Multicast->Address->IPv4Address[0], _IPAddr);
	
	pProfile->MetadataConfiguration->Multicast->Address->IPv6Address = NULL;

	pProfile->MetadataConfiguration->Multicast->__size = 0;
	pProfile->MetadataConfiguration->Multicast->__any = NULL;
	pProfile->MetadataConfiguration->Multicast->__anyAttribute = NULL;
#else
	pProfile->MetadataConfiguration = NULL;
#endif

#ifdef __AUDIO__
#if (AUDIO_RX==1)
	pProfile->Extension = (struct tt__ProfileExtension *)soap_malloc(soap, sizeof(struct tt__ProfileExtension));
	memset(pProfile->Extension, 0, sizeof(struct tt__ProfileExtension));	

	/* AudioOutputConfiguration */
	pProfile->Extension->AudioOutputConfiguration = (struct tt__AudioOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfiguration));
	memset(pProfile->Extension->AudioOutputConfiguration, 0, sizeof(struct tt__AudioOutputConfiguration));
	pProfile->Extension->AudioOutputConfiguration->Name = soap_strdup(soap, pAOC->AOname);
	pProfile->Extension->AudioOutputConfiguration->UseCount = pAOC->AOusecount;
	pProfile->Extension->AudioOutputConfiguration->token = soap_strdup(soap, pAOC->AOtoken);
	pProfile->Extension->AudioOutputConfiguration->OutputToken = soap_strdup(soap, pAOC->AOutputToken);
	pProfile->Extension->AudioOutputConfiguration->SendPrimacy = NULL;
	pProfile->Extension->AudioOutputConfiguration->OutputLevel = pAOC->AOutputLevel;	
	pProfile->Extension->AudioOutputConfiguration->__size = 0; 
	pProfile->Extension->AudioOutputConfiguration->__any = NULL;
	pProfile->Extension->AudioOutputConfiguration->__anyAttribute = NULL;	

	/* AudioDecoderConfiguration */
	pProfile->Extension->AudioDecoderConfiguration = (struct tt__AudioDecoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioDecoderConfiguration));
	memset(pProfile->Extension->AudioDecoderConfiguration, 0, sizeof(struct tt__AudioDecoderConfiguration));
	pProfile->Extension->AudioDecoderConfiguration->Name = soap_strdup(soap, pADC->ADname);
	pProfile->Extension->AudioDecoderConfiguration->UseCount = pADC->ADusecount;
	pProfile->Extension->AudioDecoderConfiguration->token = soap_strdup(soap, pADC->ADtoken);
	pProfile->Extension->AudioDecoderConfiguration->__size = 0; 
	pProfile->Extension->AudioDecoderConfiguration->__any = NULL;
	pProfile->Extension->AudioDecoderConfiguration->__anyAttribute = NULL;

	pProfile->Extension->__size = 0;
	pProfile->Extension->__any = NULL;
	pProfile->Extension->Extension = NULL;
	pProfile->Extension->__anyAttribute = NULL;
#else
	pProfile->Extension = NULL;
#endif
#else
	pProfile->Extension = NULL;
#endif

	pProfile->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}


/** 
 * @brief Get Profiles
 * @Any endpoint can ask for the existing media profiles of an NVT using the GetProfiles command.
 *   
 * @param Request	:_trt__GetProfile structure contains an empty message.      
 * @param Response	:_trt__GetProfileResponse structure contains contains a configuration of all available profiles.
 * @return               : On success - SOAP_OK.
 On failure - No command specific faults!
*/

int __trt__GetProfiles(struct soap *soap, 
                       struct _trt__GetProfiles *trt__GetProfiles, 
                       struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;
	char _IPAddr[SMALL_INFO_LENGTH];

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	trt__GetProfilesResponse->__sizeProfiles = oSysInfo->nProfileSize;

	trt__GetProfilesResponse->Profiles = (struct tt__Profile *)soap_malloc(soap, sizeof(struct tt__Profile) * trt__GetProfilesResponse->__sizeProfiles);
	memset(trt__GetProfilesResponse->Profiles, 0x00, sizeof(struct tt__Profile) * trt__GetProfilesResponse->__sizeProfiles);

	for (i = 0; i < trt__GetProfilesResponse->__sizeProfiles; i++) {
		struct tt__Profile *pProfile = &(trt__GetProfilesResponse->Profiles[i]);
		pProfile->Name = soap_strdup(soap, oSysInfo->Oprofile[i].profilename);
		pProfile->token = soap_strdup(soap, oSysInfo->Oprofile[i].profiletoken);
		pProfile->fixed = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pProfile->fixed[0] = oSysInfo->Oprofile[i].fix;
		pProfile->__anyAttribute = NULL;

		/* Give VideoSourceConfiguration allocate space */
		pProfile->VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoSourceConfiguration));
		memset(pProfile->VideoSourceConfiguration, 0x00, sizeof(struct tt__VideoSourceConfiguration));

		struct tt__VideoSourceConfiguration *pVideoSocCfg = pProfile->VideoSourceConfiguration;
		int idxVSC = MatchProfileVSCToken(oSysInfo->Oprofile[i].VSCtoken);
		VSC_t *pVSC = &(oSysInfo->VSC[idxVSC]);
		userVSCupdate(pVSC);

		pVideoSocCfg->Name = soap_strdup(soap, pVSC->Vname);
		pVideoSocCfg->UseCount = pVSC->Vcount;
		pVideoSocCfg->token = soap_strdup(soap, pVSC->Vtoken);
		pVideoSocCfg->SourceToken = soap_strdup(soap, pVSC->Vsourcetoken);

		pVideoSocCfg->Bounds = (struct tt__IntRectangle *)soap_malloc(soap,sizeof(struct tt__IntRectangle));
		memset(pVideoSocCfg->Bounds, 0x00, sizeof(struct tt__IntRectangle));
		pVideoSocCfg->Bounds->x = pVSC->windowx;
		pVideoSocCfg->Bounds->y = pVSC->windowy;
		pVideoSocCfg->Bounds->height = pVSC->windowheight;
		pVideoSocCfg->Bounds->width = pVSC->windowwidth;

		pVideoSocCfg->__size = 0;
		pVideoSocCfg->__any = NULL;
		pVideoSocCfg->Extension = NULL;
		pVideoSocCfg->__anyAttribute = NULL;

		/* AudioSourceConfiguration */
#ifdef __AUDIO__
		int idxASC = MatchProfileASCToken(oSysInfo->Oprofile[i].ASCtoken);
		ASC_t *pASC = &(oSysInfo->ASC[idxASC]);

		pProfile->AudioSourceConfiguration = (struct tt__AudioSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
		memset(pProfile->AudioSourceConfiguration, 0x00, sizeof(struct tt__AudioSourceConfiguration));
		pProfile->AudioSourceConfiguration->Name = soap_strdup(soap, pASC->Aname);
		pProfile->AudioSourceConfiguration->UseCount = pASC->Acount;
		pProfile->AudioSourceConfiguration->token = soap_strdup(soap, pASC->Atoken);
		pProfile->AudioSourceConfiguration->SourceToken = soap_strdup(soap, pASC->Asourcetoken);
		pProfile->AudioSourceConfiguration->__size = 0;
		pProfile->AudioSourceConfiguration->__any = NULL;
		pProfile->AudioSourceConfiguration->__anyAttribute = NULL;
#else
		pProfile->AudioSourceConfiguration = NULL;
#endif

		/* VideoEncoderConfiguration */
		pProfile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc (soap,sizeof(struct tt__VideoEncoderConfiguration));	
		memset(pProfile->VideoEncoderConfiguration, 0x00, sizeof(struct tt__VideoEncoderConfiguration));

		struct tt__VideoEncoderConfiguration *pVideoEncCfg = pProfile->VideoEncoderConfiguration;
		int idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[i].VECtoken);
		VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);
		userVECupdate(pVEC, idxVEC);

		pVideoEncCfg->Name = soap_strdup(soap, pVEC->VEname);
		pVideoEncCfg->UseCount = pVEC->VEusercount;
		pVideoEncCfg->token = soap_strdup(soap, pVEC->VEtoken);
		pVideoEncCfg->Encoding = pVEC->Vencoder;

		pVideoEncCfg->Resolution = (struct tt__VideoResolution*)soap_malloc(soap,sizeof(struct tt__VideoResolution));
		memset(pVideoEncCfg->Resolution, 0x00, sizeof(struct tt__VideoResolution));
		pVideoEncCfg->Resolution->Height = pVEC->Rheight;
		pVideoEncCfg->Resolution->Width = pVEC->Rwidth;

		pVideoEncCfg->Quality = pVEC->VEquality;

		pVideoEncCfg->RateControl = (struct tt__VideoRateControl*)soap_malloc(soap,sizeof(struct tt__VideoRateControl));
		memset(pVideoEncCfg->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
		pVideoEncCfg->RateControl->FrameRateLimit = oget_framerate2(pVEC->frameratelimit);
		pVideoEncCfg->RateControl->EncodingInterval = pVEC->encodinginterval;

		pVideoEncCfg->MPEG4 = NULL;
		pVideoEncCfg->H264 = NULL;

		switch (pVideoEncCfg->Encoding) {
			case tt__VideoEncoding__JPEG:
				pVideoEncCfg->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);
				break;

			case tt__VideoEncoding__H264:
				pVideoEncCfg->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
				pVideoEncCfg->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
				memset(pVideoEncCfg->H264, 0x00, sizeof(struct tt__H264Configuration));
				pVideoEncCfg->H264->GovLength = pVEC->H264govlength;
				pVideoEncCfg->H264->H264Profile = pVEC->H264Profile;//Baseline = 0, Main = 1, High = 3
				break;

			case tt__VideoEncoding__MPEG4:
			default:
				pVideoEncCfg->RateControl->BitrateLimit = 0;
				printf("%s(%d) : Encoding Type Error!(%d)\r\n", __func__, __LINE__, pVideoEncCfg->Encoding);
				break;
		}

#if 1
		pVideoEncCfg->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pVideoEncCfg->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
		pVideoEncCfg->Multicast->Port = pVEC->Multicast_Conf.port;
		pVideoEncCfg->Multicast->TTL = pVEC->Multicast_Conf.ttl;
		pVideoEncCfg->Multicast->AutoStart = pVEC->Multicast_Conf.autostart;

		pVideoEncCfg->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));		
		memset(pVideoEncCfg->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
		pVideoEncCfg->Multicast->Address->Type = pVEC->Multicast_Conf.nIPType;//IPv4 = 0, IPv6 = 1
		pVideoEncCfg->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pVideoEncCfg->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
		pVideoEncCfg->Multicast->Address->IPv6Address = NULL;
#else
		pVideoEncCfg->Multicast = NULL;
#endif

		pVideoEncCfg->SessionTimeout = pVEC->sessiontimeout;
		pVideoEncCfg->__size = 0;
		pVideoEncCfg->__any = NULL;
		pVideoEncCfg->__anyAttribute = NULL;

		/* AudioEncoderConfiguration */
#ifdef __AUDIO__
		int idxAEC = MatchProfileAECToken(oSysInfo->Oprofile[i].AECtoken);
		AEC_t *pAEC = &(oSysInfo->AEC[idxAEC]);

//		flprintf("i(%d) AECt(%s) idx(%d) AEname(%s)\r\n", 
//			i, oSysInfo->Oprofile[i].AECtoken, idxAEC, pAEC->AEname);

		pProfile->AudioEncoderConfiguration = (struct tt__AudioEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
		memset(pProfile->AudioEncoderConfiguration, 0x00, sizeof(struct tt__AudioEncoderConfiguration));

		pProfile->AudioEncoderConfiguration->Name = soap_strdup(soap, pAEC->AEname);
		pProfile->AudioEncoderConfiguration->UseCount = pAEC->AEusercount;
		pProfile->AudioEncoderConfiguration->token = soap_strdup(soap, pAEC->AEtoken);
		pProfile->AudioEncoderConfiguration->Encoding = pAEC->Aencoder;
		pProfile->AudioEncoderConfiguration->Bitrate = pAEC->Abitratelimit;
		pProfile->AudioEncoderConfiguration->SampleRate = pAEC->Asamplerate;

#if 1
		pProfile->AudioEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pProfile->AudioEncoderConfiguration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
		pProfile->AudioEncoderConfiguration->Multicast->Port = pAEC->Multicast_Conf.port;
		pProfile->AudioEncoderConfiguration->Multicast->TTL = pAEC->Multicast_Conf.ttl;
		pProfile->AudioEncoderConfiguration->Multicast->AutoStart = pAEC->Multicast_Conf.autostart;
		
		pProfile->AudioEncoderConfiguration->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));		
		memset(pProfile->AudioEncoderConfiguration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
		pProfile->AudioEncoderConfiguration->Multicast->Address->Type = pAEC->Multicast_Conf.nIPType;//IPv4 = 0, IPv6 = 1
		pProfile->AudioEncoderConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pProfile->AudioEncoderConfiguration->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
		pProfile->AudioEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
#else
		pProfile->AudioEncoderConfiguration->Multicast = NULL;
#endif

		pProfile->AudioEncoderConfiguration->SessionTimeout = pAEC->sessiontimeout;
		pProfile->AudioEncoderConfiguration->__size = 0;
		pProfile->AudioEncoderConfiguration->__any = NULL;
		pProfile->AudioEncoderConfiguration->__anyAttribute = NULL;
#else
		pProfile->AudioEncoderConfiguration = NULL;
#endif				

		/* VideoAnalyticsConfiguration */
		pProfile->VideoAnalyticsConfiguration  = NULL;

#if 1
		pProfile->PTZConfiguration = (struct tt__PTZConfiguration *)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));
		memset(pProfile->PTZConfiguration, 0x00, sizeof(struct tt__PTZConfiguration));
		
		struct tt__PTZConfiguration *PTZConf = pProfile->PTZConfiguration;
		PTZConf->UseCount = oSysInfo->PTZC.PTZusecount;	
		PTZConf->NodeToken = soap_strdup(soap, oSysInfo->PTZC.Node[0].Token);
		PTZConf->Name = soap_strdup(soap, oSysInfo->PTZC.PTZname); 		
		PTZConf->token = soap_strdup(soap, oSysInfo->PTZC.PTZtoken);
		
		PTZConf->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace);
		PTZConf->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsoluteZoomPositionSpace);
		PTZConf->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativePanTiltTranslationSpace);
		PTZConf->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativeZoomTranslationSpace);
		PTZConf->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace);
		PTZConf->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousZoomVelocitySpace);

		PTZConf->DefaultPTZSpeed = (struct tt__PTZSpeed *)soap_malloc(soap, sizeof(struct tt__PTZSpeed));
		memset(PTZConf->DefaultPTZSpeed, 0x00, sizeof(struct tt__PTZSpeed));

		PTZConf->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));
		memset(PTZConf->DefaultPTZSpeed->PanTilt, 0x00, sizeof(struct tt__Vector2D));
		PTZConf->DefaultPTZSpeed->PanTilt->x = oSysInfo->PTZC.DefaultPantilt_x;
		PTZConf->DefaultPTZSpeed->PanTilt->y = oSysInfo->PTZC.DefaultPantilt_y;
		PTZConf->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap,oSysInfo->PTZC.DefaultPantilt_space);
		PTZConf->DefaultPTZSpeed->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));
		memset(PTZConf->DefaultPTZSpeed->Zoom, 0x00, sizeof(struct tt__Vector1D));
		PTZConf->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.DefaultZoom_space);
		PTZConf->DefaultPTZSpeed->Zoom->x = oSysInfo->PTZC.DefaultZoom_x;

		PTZConf->DefaultPTZTimeout = (LONG64 *)soap_malloc(soap, sizeof(LONG64));
		memset(PTZConf->DefaultPTZTimeout, 0x00, sizeof(LONG64));
		*PTZConf->DefaultPTZTimeout = oSysInfo->PTZC.DefaultPTZTimeout;
		
		PTZConf->PanTiltLimits = (struct tt__PanTiltLimits *)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));
		memset(PTZConf->PanTiltLimits, 0x00, sizeof(struct tt__PanTiltLimits));
		PTZConf->PanTiltLimits->Range = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));
		memset(PTZConf->PanTiltLimits->Range, 0x00, sizeof(struct tt__Space2DDescription));
		PTZConf->PanTiltLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.PanTiltLimit_URI);
		PTZConf->PanTiltLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
		memset(PTZConf->PanTiltLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
		PTZConf->PanTiltLimits->Range->XRange->Max = oSysInfo->PTZC.PanTiltLimit_XRange_Max;
		PTZConf->PanTiltLimits->Range->XRange->Min = oSysInfo->PTZC.PanTiltLimit_XRange_Min;
		PTZConf->PanTiltLimits->Range->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
		memset(PTZConf->PanTiltLimits->Range->YRange, 0x00, sizeof(struct tt__FloatRange));
		PTZConf->PanTiltLimits->Range->YRange->Max = oSysInfo->PTZC.PanTiltLimit_YRange_Max;
		PTZConf->PanTiltLimits->Range->YRange->Min = oSysInfo->PTZC.PanTiltLimit_YRange_Min;
		
		PTZConf->ZoomLimits = (struct tt__ZoomLimits *)soap_malloc(soap, sizeof(struct tt__ZoomLimits));
		memset(PTZConf->ZoomLimits, 0x00, sizeof(struct tt__ZoomLimits));
		PTZConf->ZoomLimits->Range = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));
		memset(PTZConf->ZoomLimits->Range, 0x00, sizeof(struct tt__Space1DDescription));
		PTZConf->ZoomLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.ZoomLimit_XRange_URI);
		
		PTZConf->ZoomLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
		memset(PTZConf->ZoomLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
		PTZConf->ZoomLimits->Range->XRange->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
		PTZConf->ZoomLimits->Range->XRange->Min= oSysInfo->PTZC.ZoomLimit_XRange_Min;
		PTZConf->Extension = NULL;
		PTZConf->__anyAttribute = NULL;
#else
		pProfile->PTZConfiguration = NULL;
#endif

#if 0		
		if(GetSysInfo()->Oprofile[i].MDC.MDusecount)
		{
			pProfile->MetadataConfiguration = (struct tt__MetadataConfiguration *)soap_malloc(soap, sizeof(struct tt__MetadataConfiguration));																																										   
			memset(pProfile->MetadataConfiguration,0,sizeof(struct tt__MetadataConfiguration));
			
			pProfile->MetadataConfiguration->Name = soap_strdup(soap,GetSysInfo()->Oprofile[i].MDC.MDname);
			
			pProfile->MetadataConfiguration->token = soap_strdup(soap,GetSysInfo()->Oprofile[i].MDC.MDtoken);
																																																				
			pProfile->MetadataConfiguration->UseCount = GetSysInfo()->Oprofile[i].MDC.MDusecount;//1;						   
			pProfile->MetadataConfiguration->Analytics = GetSysInfo()->Oprofile[i].MDC.MDanalytics;//0; 																																																					 
			pProfile->MetadataConfiguration->SessionTimeout = GetSysInfo()->Oprofile[i].MDC.sessiontimeout;//72000;																																																		 
			pProfile->MetadataConfiguration->PTZStatus = NULL;																																																														   
			pProfile->MetadataConfiguration->Events = NULL;				
			pProfile->MetadataConfiguration->AnalyticsEngineConfiguration = NULL; 	  
			pProfile->MetadataConfiguration->Extension = NULL;	   
			pProfile->MetadataConfiguration->__size = 0;
			pProfile->MetadataConfiguration->__any = NULL;	   
			pProfile->MetadataConfiguration->__anyAttribute= NULL;	 
			
			pProfile->MetadataConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration)); 
			memset(pProfile->MetadataConfiguration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration)); 
			pProfile->MetadataConfiguration->Multicast->Port = GetSysInfo()->Oprofile[i].MDC.VMC.port;
			pProfile->MetadataConfiguration->Multicast->TTL = GetSysInfo()->Oprofile[i].MDC.VMC.ttl;
			pProfile->MetadataConfiguration->Multicast->AutoStart = GetSysInfo()->Oprofile[i].MDC.VMC.autostart;//false 	 
			
			pProfile->MetadataConfiguration->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
			memset(pProfile->MetadataConfiguration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
			pProfile->MetadataConfiguration->Multicast->Address->Type = GetSysInfo()->Oprofile[i].MDC.VMC.nIPType;	
			
			pProfile->MetadataConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));  
			
			pProfile->MetadataConfiguration->Multicast->Address->IPv4Address[0] = soap_strdup(soap,_IPAddr);
			
			pProfile->MetadataConfiguration->Multicast->Address->IPv6Address = NULL;	
			
			pProfile->MetadataConfiguration->Multicast->__size = 0;
			pProfile->MetadataConfiguration->Multicast->__any = NULL; 	  
			pProfile->MetadataConfiguration->Multicast->__anyAttribute= NULL; 	
  		}
#else
		pProfile->MetadataConfiguration = NULL;
#endif

#ifdef __AUDIO__
#if (AUDIO_RX==1)
		pProfile->Extension = (struct tt__ProfileExtension *)soap_malloc(soap, sizeof(struct tt__ProfileExtension));
		memset(pProfile->Extension, 0, sizeof(struct tt__ProfileExtension)); 		

		AOC_t *pAOC = &(oSysInfo->AOC);
		ADC_t *pADC = &(oSysInfo->ADC);

		pProfile->Extension->AudioOutputConfiguration = (struct tt__AudioOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfiguration));
		memset(pProfile->Extension->AudioOutputConfiguration, 0, sizeof(struct tt__AudioOutputConfiguration));
		pProfile->Extension->AudioOutputConfiguration->Name = soap_strdup(soap, pAOC->AOname);
		pProfile->Extension->AudioOutputConfiguration->UseCount = pAOC->AOusecount;
		pProfile->Extension->AudioOutputConfiguration->token = soap_strdup(soap, pAOC->AOtoken);
		pProfile->Extension->AudioOutputConfiguration->OutputToken = soap_strdup(soap, pAOC->AOutputToken);
		pProfile->Extension->AudioOutputConfiguration->SendPrimacy = NULL;
		pProfile->Extension->AudioOutputConfiguration->OutputLevel = pAOC->AOutputLevel;	
		pProfile->Extension->AudioOutputConfiguration->__size = 0; 
		pProfile->Extension->AudioOutputConfiguration->__any = NULL;
		pProfile->Extension->AudioOutputConfiguration->__anyAttribute = NULL;	

		pProfile->Extension->AudioDecoderConfiguration = (struct tt__AudioDecoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioDecoderConfiguration));
		memset(pProfile->Extension->AudioDecoderConfiguration, 0, sizeof(struct tt__AudioDecoderConfiguration));
		pProfile->Extension->AudioDecoderConfiguration->Name = soap_strdup(soap, pADC->ADname);
		pProfile->Extension->AudioDecoderConfiguration->UseCount = pADC->ADusecount;
		pProfile->Extension->AudioDecoderConfiguration->token = soap_strdup(soap, pADC->ADtoken);
		pProfile->Extension->AudioDecoderConfiguration->__size = 0;	
		pProfile->Extension->AudioDecoderConfiguration->__any = NULL;
		pProfile->Extension->AudioDecoderConfiguration->__anyAttribute = NULL;

		pProfile->Extension->__size = 0;
		pProfile->Extension->__any = NULL;
		pProfile->Extension->Extension = NULL;
		pProfile->Extension->__anyAttribute = NULL;
#else
		pProfile->Extension = NULL;
#endif
#else
		pProfile->Extension = NULL;
#endif
	}	

	LEAVE();

	return SOAP_OK;

	UNUSED(trt__GetProfiles);
}


/** Auto-test server operation __trt__AddVideoEncoderConfiguration */
int __trt__AddVideoEncoderConfiguration(struct soap *soap, 
struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration, 
struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVEC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddVideoEncoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->VECtoken != NULL) && (strcmp(pPro->VECtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->VECtoken);
		idxVEC = MatchProfileVECToken(pPro->VECtoken);
		if (idxVEC != -1) {
			oSysInfo->VEC[idxVEC].VEusercount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->VECtoken, idxVEC);
		}
		pPro->useVEC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddVideoEncoderConfiguration->ConfigurationToken);
	strcpy(pPro->VECtoken, trt__AddVideoEncoderConfiguration->ConfigurationToken);
	idxVEC = MatchProfileVECToken(pPro->VECtoken);
	if (idxVEC != -1) {
		oSysInfo->VEC[idxVEC].VEusercount++;
	} else {
		flprintf("Error, %s/%d\r\n", pPro->VECtoken, idxVEC);
	}
	pPro->useVEC++;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__AddVideoEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__AddVideoSourceConfiguration */
int __trt__AddVideoSourceConfiguration(struct soap *soap, 
struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration, 
struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVSC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddVideoSourceConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->VSCtoken != NULL) && (strcmp(pPro->VSCtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->VSCtoken);
		idxVSC = MatchProfileVSCToken(pPro->VSCtoken);
		if (idxVSC != -1) {
			oSysInfo->VSC[idxVSC].Vcount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->VSCtoken, idxVSC);
		}
		pPro->useVSC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddVideoSourceConfiguration->ConfigurationToken);
	strcpy(pPro->VSCtoken, trt__AddVideoSourceConfiguration->ConfigurationToken);
	idxVSC = MatchProfileVSCToken(pPro->VSCtoken);
	if (idxVSC != -1) {
		oSysInfo->VSC[idxVSC].Vcount++;
	} else {
		flprintf("Error, %s/%d\r\n", pPro->VSCtoken, idxVSC);
	}
	pPro->useVSC++;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__AddVideoSourceConfigurationResponse);
}


/** Auto-test server operation __trt__AddAudioEncoderConfiguration */
int __trt__AddAudioEncoderConfiguration(struct soap *soap, 
struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration, 
struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxAEC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddAudioEncoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->AECtoken != NULL) && (strcmp(pPro->AECtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->AECtoken);
		idxAEC = MatchProfileAECToken(pPro->AECtoken);
		if (idxAEC != -1) {
			oSysInfo->AEC[idxAEC].AEusercount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->AECtoken, idxAEC);
		}
		pPro->useAEC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddAudioEncoderConfiguration->ConfigurationToken);
	strcpy(pPro->AECtoken, trt__AddAudioEncoderConfiguration->ConfigurationToken);
	idxAEC = MatchProfileAECToken(pPro->AECtoken);
	if (idxAEC != -1) {
		oSysInfo->AEC[idxAEC].AEusercount++;
	} else {
		flprintf("Error, %s/%d\r\n", pPro->AECtoken, idxAEC);
	}
	pPro->useAEC++;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__AddAudioEncoderConfiguration);
#endif
	UNUSED(trt__AddAudioEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__AddAudioSourceConfiguration */
int __trt__AddAudioSourceConfiguration(struct soap *soap, 
struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration, 
struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxASC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddAudioSourceConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->ASCtoken != NULL) && (strcmp(pPro->ASCtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->ASCtoken);
		idxASC = MatchProfileASCToken(pPro->ASCtoken);
		if (idxASC != -1) {
			oSysInfo->ASC[idxASC].Acount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->ASCtoken, idxASC);
		}
		pPro->useASC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddAudioSourceConfiguration->ConfigurationToken);
	strcpy(pPro->ASCtoken, trt__AddAudioSourceConfiguration->ConfigurationToken);
	idxASC = MatchProfileASCToken(pPro->ASCtoken);
	if (idxASC != -1) {
		oSysInfo->ASC[idxASC].Acount++;
	} else {
		flprintf("Error, %s/%d\r\n", pPro->ASCtoken, idxASC);
	}
	pPro->useASC++;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__AddAudioSourceConfiguration);
#endif
	UNUSED(trt__AddAudioSourceConfigurationResponse);
}


/** Auto-test server operation __trt__AddPTZConfiguration */
int __trt__AddPTZConfiguration(struct soap *soap, 
struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration, 
struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse)
{
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;
	
	ENTER();

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddPTZConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(oSysInfo->Oprofile[idxPROF].PTZCtoken, trt__AddPTZConfiguration->ProfileToken);
	oSysInfo->Oprofile[idxPROF].usePTZC = 1;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__AddPTZConfigurationResponse);
}


/** Auto-test server operation __trt__AddVideoAnalyticsConfiguration */
int __trt__AddVideoAnalyticsConfiguration(struct soap *soap, 
struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration, 
struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__AddVideoAnalyticsConfiguration);
	UNUSED(trt__AddVideoAnalyticsConfigurationResponse);
}


/** Auto-test server operation __trt__AddMetadataConfiguration */
int __trt__AddMetadataConfiguration(struct soap *soap, 
struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration, 
struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse)
{
	ENTER();

//	trt__AddMetadataConfiguration->ProfileToken;
//	trt__AddMetadataConfiguration->ConfigurationToken;
	
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__AddMetadataConfiguration);
	UNUSED(trt__AddMetadataConfigurationResponse);
}


/** Auto-test server operation __trt__AddAudioOutputConfiguration */
int __trt__AddAudioOutputConfiguration(struct soap *soap, 
struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration, 
struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddAudioOutputConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);
	AOC_t *pAOC = &(oSysInfo->AOC);

	if ((pPro->AOCtoken != NULL) && (strcmp(pPro->AOCtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->AOCtoken);
		pAOC->AOusecount--;
		pPro->useAOC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddAudioOutputConfiguration->ConfigurationToken);
	strcpy(pPro->AOCtoken, trt__AddAudioOutputConfiguration->ConfigurationToken);
	pAOC->AOusecount++;
	pPro->useAOC++;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__AddAudioOutputConfiguration);
#endif
	UNUSED(trt__AddAudioOutputConfigurationResponse);
}


/** Auto-test server operation __trt__AddAudioDecoderConfiguration */
int __trt__AddAudioDecoderConfiguration(struct soap *soap, 
struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration, 
struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__AddAudioDecoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);
	ADC_t *pADC = &(oSysInfo->ADC);
	
	if ((pPro->ADCtoken != NULL) && (strcmp(pPro->ADCtoken, "") != 0)) {
		// 입력된게 존재하면, 대체를 위해 기존에 할당한 token을 감소시켜준다.
		flprintf("token(%s) replaced!\r\n", pPro->ADCtoken);
		pADC->ADusecount--;
		pPro->useADC--;
	}

	// 추가한다.
	flprintf("token(%s) new!\r\n", trt__AddAudioDecoderConfiguration->ConfigurationToken);
	strcpy(pPro->ADCtoken, trt__AddAudioDecoderConfiguration->ConfigurationToken);
	pADC->ADusecount++;
	pPro->useADC++;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__AddAudioDecoderConfiguration);
#endif
	UNUSED(trt__AddAudioDecoderConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveVideoEncoderConfiguration */
int __trt__RemoveVideoEncoderConfiguration(struct soap *soap, 
struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration, 
struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVEC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveVideoEncoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->VECtoken != NULL) && (strcmp(pPro->VECtoken, "") != 0)) {
		idxVEC = MatchProfileVECToken(pPro->VECtoken);
		if (idxVEC != -1) {
			oSysInfo->VEC[idxVEC].VEusercount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->VECtoken, idxVEC);
		}
		pPro->useVEC--;
		strcpy(pPro->VECtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveVideoEncoderConfiguration->ProfileToken);
	}

	LEAVE();		
	return SOAP_OK;

	UNUSED(trt__RemoveVideoEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveVideoSourceConfiguration */
int __trt__RemoveVideoSourceConfiguration(struct soap *soap, 
struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration, 
struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVSC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveVideoSourceConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->VSCtoken != NULL) && (strcmp(pPro->VSCtoken, "") != 0)) {
		idxVSC = MatchProfileVSCToken(pPro->VSCtoken);
		if (idxVSC != -1) {
			oSysInfo->VSC[idxVSC].Vcount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->VSCtoken, idxVSC);
		}
		pPro->useVSC--;
		strcpy(pPro->VSCtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveVideoSourceConfiguration->ProfileToken);
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__RemoveVideoSourceConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveAudioEncoderConfiguration */
int __trt__RemoveAudioEncoderConfiguration(struct soap *soap, 
struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration, 
struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxAEC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveAudioEncoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->AECtoken != NULL) && (strcmp(pPro->AECtoken, "") != 0)) {
		idxAEC = MatchProfileAECToken(pPro->AECtoken);
		if (idxAEC != -1) {
			oSysInfo->AEC[idxAEC].AEusercount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->AECtoken, idxAEC);
		}
		pPro->useAEC--;
		strcpy(pPro->AECtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveAudioEncoderConfiguration->ProfileToken);
	}

	LEAVE();		
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__RemoveAudioEncoderConfiguration);
#endif
	UNUSED(trt__RemoveAudioEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveAudioSourceConfiguration */
int __trt__RemoveAudioSourceConfiguration(struct soap *soap, 
struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration, 
struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxASC;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveAudioSourceConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);

	if ((pPro->ASCtoken != NULL) && (strcmp(pPro->ASCtoken, "") != 0)) {
		idxASC = MatchProfileASCToken(pPro->ASCtoken);
		if (idxASC != -1) {
			oSysInfo->ASC[idxASC].Acount--;
		} else {
			flprintf("Error, %s/%d\r\n", pPro->ASCtoken, idxASC);
		}
		pPro->useASC--;
		strcpy(pPro->ASCtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveAudioSourceConfiguration->ProfileToken);
	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__RemoveAudioSourceConfiguration);
#endif
	UNUSED(trt__RemoveAudioSourceConfigurationResponse);
}


/** Auto-test server operation __trt__RemovePTZConfiguration */
int __trt__RemovePTZConfiguration(struct soap *soap, 
struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration, 
struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse)
{
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;
	
	ENTER();

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemovePTZConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(oSysInfo->Oprofile[idxPROF].PTZCtoken, "DEFAULT");
	oSysInfo->Oprofile[idxPROF].usePTZC = 0;

	LEAVE();
	return SOAP_OK;
	UNUSED(trt__RemovePTZConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveVideoAnalyticsConfiguration */
int __trt__RemoveVideoAnalyticsConfiguration(struct soap *soap, 
struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration, 
struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__RemoveVideoAnalyticsConfiguration);
	UNUSED(trt__RemoveVideoAnalyticsConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveMetadataConfiguration */
int __trt__RemoveMetadataConfiguration(struct soap *soap,
struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration, 
struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__RemoveMetadataConfiguration);
	UNUSED(trt__RemoveMetadataConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveAudioOutputConfiguration */
int __trt__RemoveAudioOutputConfiguration(struct soap *soap, 
struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration, 
struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveAudioOutputConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);
	AOC_t *pAOC = &(oSysInfo->AOC);

	if ((pPro->AOCtoken != NULL) && (strcmp(pPro->AOCtoken, "") != 0)) {
		pAOC->AOusecount--;
		pPro->useAOC--;
		strcpy(pPro->AOCtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveAudioOutputConfiguration->ProfileToken);

	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__RemoveAudioOutputConfiguration);
#endif
	UNUSED(trt__RemoveAudioOutputConfigurationResponse);
}


/** Auto-test server operation __trt__RemoveAudioDecoderConfiguration */
int __trt__RemoveAudioDecoderConfiguration(struct soap *soap, 
struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration, 
struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__RemoveAudioDecoderConfiguration->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pPro = &(oSysInfo->Oprofile[idxPROF]);
	ADC_t *pADC = &(oSysInfo->ADC);

	if ((pPro->ADCtoken != NULL) && (strcmp(pPro->ADCtoken, "") != 0)) {
		pADC->ADusecount--;
		pPro->useADC--;
		strcpy(pPro->ADCtoken, "");
	} else {
		flprintf("Error, %s\r\n", trt__RemoveAudioDecoderConfiguration->ProfileToken);

	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__RemoveAudioDecoderConfiguration);
#endif
	UNUSED(trt__RemoveAudioDecoderConfigurationResponse);
}


/** Auto-test server operation __trt__DeleteProfile */
int __trt__DeleteProfile(struct soap *soap,
struct _trt__DeleteProfile *trt__DeleteProfile,
struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse)
{
	ENTER();
	
	SysInfo *oSysInfo = GetSysInfo();	
	int Ptoken_exist = NOT_EXIST;
	int idxToken, idxPROF;

	for (idxPROF = 0; idxPROF < MAX_PROFILE; idxPROF++) {	
		if(strcmp(trt__DeleteProfile->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	Oprofile_t *pOpro = &(oSysInfo->Oprofile[idxPROF]);

	pOpro->fix = 0;
	strcpy(oSysInfo->Oprofile[idxPROF].profilename, "");
	strcpy(oSysInfo->Oprofile[idxPROF].profiletoken, "");

	if (pOpro->useVSC) {
		flprintf("Del VSCtoken(%s) use(%d) Check\r\n", pOpro->VSCtoken, pOpro->useVSC);
		idxToken = MatchProfileVSCToken(pOpro->VSCtoken);
		oSysInfo->VSC[idxToken].Vcount--;
		pOpro->useVSC = 0;
		strcpy(pOpro->VSCtoken, "");
	}

	if (pOpro->useVEC) {
		flprintf("Del VECtoken(%s) use(%d) Check\r\n", pOpro->VECtoken, pOpro->useVEC);
		idxToken = MatchProfileVECToken(pOpro->VECtoken);
		oSysInfo->VEC[idxToken].VEusercount--;
		pOpro->useVEC = 0;
		strcpy(pOpro->VECtoken, "");
	}

	if (pOpro->useVOC) {
		flprintf("Del VOCtoken(%s) use(%d) Check\r\n", pOpro->VOCtoken, pOpro->useVOC);
		oSysInfo->VOC.usecount--;
		pOpro->useVOC = 0;
		strcpy(pOpro->VOCtoken, "");
	}

	if (pOpro->useVAC) {
		flprintf("Del VACtoken(%s) use(%d) Check\r\n", pOpro->VACtoken, pOpro->useVAC);
		oSysInfo->VAC.usecount--;
		pOpro->useVAC = 0;
		strcpy(pOpro->VACtoken, "");
	}

	if (pOpro->useASC) {
		flprintf("Del ASCtoken(%s) use(%d) Check\r\n", pOpro->ASCtoken, pOpro->useASC);
		idxToken = MatchProfileASCToken(pOpro->ASCtoken);
		oSysInfo->ASC[idxToken].Acount--;
		pOpro->useASC = 0;
		strcpy(pOpro->ASCtoken, "");
	}

	if (pOpro->useAEC) {
		flprintf("Del AECtoken(%s) use(%d) Check\r\n", pOpro->AECtoken, pOpro->useAEC);
		idxToken = MatchProfileAECToken(pOpro->AECtoken);
		oSysInfo->AEC[idxToken].AEusercount--;
		pOpro->useAEC = 0;
		strcpy(pOpro->AECtoken, "");
	}

	if (pOpro->useAOC) {
		flprintf("Del AOCtoken(%s) use(%d) Check\r\n", pOpro->AOCtoken, pOpro->useAOC);
		oSysInfo->AOC.AOusecount--;
		pOpro->useAOC = 0;
		strcpy(pOpro->AOCtoken, "");
	}

	if (pOpro->useADC) {
		flprintf("Del ADCtoken(%s) use(%d) Check\r\n", pOpro->ADCtoken, pOpro->useADC);
		oSysInfo->ADC.ADusecount--;
		pOpro->useADC = 0;
		strcpy(pOpro->ADCtoken, "");
	}

	viewSysInfo(oSysInfo);
	viewProfile(&oSysInfo->Oprofile[idxPROF]);

	flprintf("DEL %s VideoSource(%d) VideoEncoder(%d)\r\n", trt__DeleteProfile->ProfileToken, oSysInfo->VSC[0].Vcount, oSysInfo->VEC[0].VEusercount);
	oSysInfo->nProfileSize--;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__DeleteProfileResponse);
}


/**
 * @brief Get Source Source Configurations
 * @This operation lists the video source configurations of a VideoSource.  
 *   
 * @param Request	: _trt__GetVideoSourceConfigurations structure contains the token of the video input.
 * @param Response	: _trt__GetVideoSourceConfigurationsResponse structure contains the requested VideoSourceConfiguration with the matching token.
 * @return               : On success - SOAP_OK.
 : On failure No command specific faults!

 */
int __trt__GetVideoSourceConfigurations(struct soap *soap, 
struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations, 
struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;

	trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = oSysInfo->SizeDefaultVSC;

	trt__GetVideoSourceConfigurationsResponse->Configurations = (struct tt__VideoSourceConfiguration*)soap_malloc(soap,trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations * sizeof(struct tt__VideoSourceConfiguration));
	memset(trt__GetVideoSourceConfigurationsResponse->Configurations, 0x00, trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations * sizeof(struct tt__VideoSourceConfiguration));

	for (i = 0; i < trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations; i++) {
		struct tt__VideoSourceConfiguration *pVSConf = &(trt__GetVideoSourceConfigurationsResponse->Configurations[i]);
		VSC_t *pVSC = &(oSysInfo->VSC[i]);
		userVSCupdate(pVSC);

		pVSConf->Name = soap_strdup(soap, pVSC->Vname);		
		pVSConf->UseCount = pVSC->Vcount; 
		pVSConf->token = soap_strdup(soap, pVSC->Vtoken);
		pVSConf->SourceToken = soap_strdup(soap, pVSC->Vsourcetoken);
		pVSConf->Bounds = (struct tt__IntRectangle *)soap_malloc(soap, sizeof(struct tt__IntRectangle));
		pVSConf->Bounds->x = pVSC->windowx;
		pVSConf->Bounds->y = pVSC->windowy;
		pVSConf->Bounds->width = pVSC->windowwidth;
		pVSConf->Bounds->height = pVSC->windowheight;
		pVSConf->__size = 0;
		pVSConf->__any = NULL;
		pVSConf->Extension = NULL;
		pVSConf->__anyAttribute = NULL;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetVideoSourceConfigurations);
}


/** 
 * @brief Get Video Encoder Configuration
 * @This operation lists all existing video encoder configurations of an NVT.
 *   
 * @param Request	: _trt__GetVideoEncoderConfigurations structure contains an empty message.
 * @param Response	:_trt__GetVideoEncoderConfigurationsResponse structure contains a list of all existing video encoder configurations in the NVT.
 * @return               : On success - SOAP_OK.
 : On failure - The requested configuration indicated with ConfigurationToken does not exist.

 */
int __trt__GetVideoEncoderConfigurations(struct soap *soap, 
struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations, 
struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse)
{	
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;
	char _IPAddr[SMALL_INFO_LENGTH];

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));
	
	trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations = oSysInfo->SizeDefaultVEC;

	trt__GetVideoEncoderConfigurationsResponse->Configurations = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, (trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations * sizeof(struct tt__VideoEncoderConfiguration)));
	memset(trt__GetVideoEncoderConfigurationsResponse->Configurations, 0x00, (trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations * sizeof(struct tt__VideoEncoderConfiguration)));

	for (i = 0; i < trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations; i++) {
		struct tt__VideoEncoderConfiguration *pVEConf = &(trt__GetVideoEncoderConfigurationsResponse->Configurations[i]);
		VEC_t *pVEC = &(oSysInfo->VEC[i]);
		userVECupdate(pVEC, i);

		pVEConf->Name = soap_strdup(soap, pVEC->VEname);
		pVEConf->UseCount = pVEC->VEusercount;
		pVEConf->token = soap_strdup(soap, pVEC->VEtoken);
		pVEConf->Encoding = pVEC->Vencoder;

		pVEConf->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
		memset(pVEConf->Resolution, 0x00, sizeof(struct tt__VideoResolution));
		pVEConf->Resolution->Width = pVEC->Rwidth;
		pVEConf->Resolution->Height = pVEC->Rheight;

		pVEConf->Quality = pVEC->VEquality; 

		pVEConf->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
		memset(pVEConf->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
		pVEConf->RateControl->FrameRateLimit = oget_framerate2(pVEC->frameratelimit);
		pVEConf->RateControl->EncodingInterval = pVEC->encodinginterval;

		pVEConf->MPEG4 = NULL;
		pVEConf->H264 = NULL;

		switch (pVEC->Vencoder) {
			case tt__VideoEncoding__JPEG:	// JPEG
				pVEConf->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);		
				break;

			case tt__VideoEncoding__H264:	// H.264
				pVEConf->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
				pVEConf->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
				memset(pVEConf->H264, 0x00, sizeof(struct tt__H264Configuration));
				pVEConf->H264->GovLength = pVEC->H264govlength;
				pVEConf->H264->H264Profile = pVEC->H264Profile;
				break;

			case tt__VideoEncoding__MPEG4:	// MPEG4
			default:
				pVEConf->RateControl->BitrateLimit = 0;
				printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
				break;
		}

#if 1
		pVEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pVEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));

		pVEConf->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		memset(pVEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
		pVEConf->Multicast->Address->Type = pVEC->Multicast_Conf.nIPType;

		pVEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pVEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);

		pVEConf->Multicast->Port = pVEC->Multicast_Conf.port;
		pVEConf->Multicast->TTL = pVEC->Multicast_Conf.ttl;
		pVEConf->Multicast->AutoStart = pVEC->Multicast_Conf.autostart;
		pVEConf->Multicast->__size = 0;
		pVEConf->Multicast->__any = NULL;
		pVEConf->Multicast->__anyAttribute = NULL;
#else
		pVEConf->Multicast = NULL;
#endif

		pVEConf->SessionTimeout = pVEC->sessiontimeout;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetVideoEncoderConfigurations);
}


/** Auto-test server operation __trt__GetAudioSourceConfigurations */
int __trt__GetAudioSourceConfigurations(struct soap *soap, 
struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations, 
struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int i;

	trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = oSysInfo->SizeDefaultASC;

	trt__GetAudioSourceConfigurationsResponse->Configurations = (struct tt__AudioSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration) * trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetAudioSourceConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioSourceConfiguration) * trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations);

	for (i = 0; i < trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations; i++) {
		struct tt__AudioSourceConfiguration *pASConf = &(trt__GetAudioSourceConfigurationsResponse->Configurations[i]);
		ASC_t *pASC = &(oSysInfo->ASC[i]);

		pASConf->Name = soap_strdup(soap, pASC->Aname);
		pASConf->UseCount = pASC->Acount;
		pASConf->token = soap_strdup(soap, pASC->Atoken);
		pASConf->SourceToken = soap_strdup(soap, pASC->Asourcetoken);
		pASConf->__size = 0;
		pASConf->__any = NULL;
		pASConf->__anyAttribute = NULL;
	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioSourceConfigurationsResponse);
#endif
	UNUSED(trt__GetAudioSourceConfigurations);
}


/** Auto-test server operation __trt__GetAudioEncoderConfigurations */
int __trt__GetAudioEncoderConfigurations(struct soap *soap, 
struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations, 
struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int i;
	char _IPAddr[SMALL_INFO_LENGTH];

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations = oSysInfo->SizeDefaultAEC;

	trt__GetAudioEncoderConfigurationsResponse->Configurations = (struct tt__AudioEncoderConfiguration *)soap_malloc(soap,sizeof(struct tt__AudioEncoderConfiguration)*trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetAudioEncoderConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioEncoderConfiguration)*trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations);

	for(i = 0; i < trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations; i++) {
		struct tt__AudioEncoderConfiguration *pAEConf = &(trt__GetAudioEncoderConfigurationsResponse->Configurations[i]);
		AEC_t *pAEC = &(oSysInfo->AEC[i]);

		pAEConf->Name = soap_strdup(soap, pAEC->AEname);
		pAEConf->UseCount = pAEC->AEusercount;
		pAEConf->token = soap_strdup(soap, pAEC->AEtoken);
		pAEConf->Encoding = pAEC->Aencoder;
		pAEConf->Bitrate = pAEC->Abitratelimit;
		pAEConf->SampleRate = pAEC->Asamplerate;

#if 1
		pAEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pAEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));

		pAEConf->Multicast->Port = pAEC->Multicast_Conf.port;
		pAEConf->Multicast->TTL = pAEC->Multicast_Conf.ttl;
		pAEConf->Multicast->AutoStart = pAEC->Multicast_Conf.autostart;

		pAEConf->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		memset(pAEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));

		pAEConf->Multicast->Address->Type = pAEC->Multicast_Conf.nIPType;
		pAEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pAEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap,_IPAddr);

		pAEConf->Multicast->Address->IPv6Address = NULL;
#else
		pAEConf->Multicast = NULL;
#endif

		pAEConf->SessionTimeout = pAEC->sessiontimeout;
		pAEConf->__size = 0;
		pAEConf->__any = NULL;
		pAEConf->__anyAttribute = NULL;
	}

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioEncoderConfigurationsResponse);
#endif
	UNUSED(trt__GetAudioEncoderConfigurations);
}


/** Auto-test server operation __trt__GetVideoAnalyticsConfigurations */
int __trt__GetVideoAnalyticsConfigurations(struct soap *soap, 
struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations, 
struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__GetVideoAnalyticsConfigurations);
	UNUSED(trt__GetVideoAnalyticsConfigurationsResponse);
}


/** Auto-test server operation __trt__GetMetadataConfigurations */
int __trt__GetMetadataConfigurations(struct soap *soap, 
struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations, 
struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetMetadataConfigurations);
	UNUSED(trt__GetMetadataConfigurationsResponse);
#if 0 	
	SysInfo *oSysInfo = GetSysInfo();
	int i = 0;
	int token_exist = 0;
	int j = 0;
	int num_token = 0;
	int index = 0;
	int num = 0;
	char _IPAddr[INFO_LENGTH];
	//NET_IPV4 ip;
	//ip.int32 = net_get_ifaddr(ETH_IF_NAME);
	sprintf(_IPAddr, ONVIF_SERVICE_ADDRESS, getIpAddress(HTTP_ONVIF_PORT));

	
	for(i = 0; i < oSysInfo->nProfileSize; i++)
	{
		token_exist = NOT_EXIST;
		for(j = 0; j< i ; j++)
		{
			if(strcmp(GetSysInfo()->Oprofile[ j].MDC.MDtoken, GetSysInfo()->Oprofile[ i].MDC.MDtoken) == 0)
			{
				token_exist = EXIST;
				break;
			}

		}
		if(token_exist == EXIST)
		{
			num_token++;
		}
	}
	
	trt__GetMetadataConfigurationsResponse->__sizeConfigurations = num_token;

	trt__GetMetadataConfigurationsResponse->Configurations=(struct tt__MetadataConfiguration *)soap_malloc(soap, num_token * sizeof(struct tt__MetadataConfiguration));
	for(i = 0; i< oSysInfo->nProfileSize; i++)
	{
		token_exist = NOT_EXIST;
		for(j = 0; j< i ; j++)
		{
			if(strcmp(GetSysInfo()->Oprofile[ j].MDC.MDtoken, GetSysInfo()->Oprofile[ i].MDC.MDtoken) == 0)
			{
				token_exist = EXIST;
			}

		}
		if(token_exist == EXIST)
		{
			trt__GetMetadataConfigurationsResponse->Configurations[num].Name = soap_strdup(soap, GetSysInfo()->Oprofile[ i].MDC.MDname);//"Face Detection";
			//trt__GetMetadataConfigurationsResponse->Configurations[num].Name = (char*) soap_malloc(soap, sizeof(char)*30);
			//strcpy( GetSysInfo()->Oprofile[ i].MDC.MDname, GetSysInfo()->Oprofile[ i].MDC.MDname);
			
			trt__GetMetadataConfigurationsResponse->Configurations[num].token = soap_strdup(soap, GetSysInfo()->Oprofile[ i].MDC.MDtoken);//"Face Detection";
			//trt__GetMetadataConfigurationsResponse->Configurations[num].token = (char*) soap_malloc(soap, sizeof(char)*50);
			//strcpy( trt__GetMetadataConfigurationsResponse->Configurations[num].token, GetSysInfo()->Oprofile[ i].MDC.MDtoken);
			
			trt__GetMetadataConfigurationsResponse->Configurations[num].UseCount = GetSysInfo()->Oprofile[ i].MDC.MDusecount;//1;
			trt__GetMetadataConfigurationsResponse->Configurations[num].SessionTimeout = GetSysInfo()->Oprofile[ i].MDC.sessiontimeout;
			trt__GetMetadataConfigurationsResponse->Configurations[num].PTZStatus = NULL;// (struct tt__PTZFilter*)soap_malloc(soap,sizeof(struct tt__PTZFilter));
			trt__GetMetadataConfigurationsResponse->Configurations[num].Events = NULL;// (struct tt__EventSubscription*)soap_malloc(soap,sizeof(struct tt__EventSubscription));
			trt__GetMetadataConfigurationsResponse->Configurations[num].Analytics = (int *)soap_malloc(soap,sizeof(int));
			*trt__GetMetadataConfigurationsResponse->Configurations[num].Analytics = 1;
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->Type = GetSysInfo()->Oprofile[ i].MDC.VMC.nIPType;
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
			//trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->IPv4Address[0] = (char*) soap_malloc(soap, sizeof(char)*50);
			//strcpy( trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->IPv4Address[0] , _IPAddr);
			
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Address->IPv6Address = NULL;
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->Port = GetSysInfo()->Oprofile[ i].MDC.VMC.port;
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->TTL = GetSysInfo()->Oprofile[ i].MDC.VMC.ttl;
			trt__GetMetadataConfigurationsResponse->Configurations[num].Multicast->AutoStart = GetSysInfo()->Oprofile[ i].MDC.VMC.autostart;
			num ++;
		}
	}
#endif
}


/** Auto-test server operation __trt__GetAudioOutputConfigurations */
int __trt__GetAudioOutputConfigurations(struct soap *soap, 
struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations, 
struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();

	trt__GetAudioOutputConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetAudioOutputConfigurationsResponse->Configurations = (struct tt__AudioOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfiguration));
	memset(trt__GetAudioOutputConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioOutputConfiguration));
	struct tt__AudioOutputConfiguration *pAOConf = trt__GetAudioOutputConfigurationsResponse->Configurations;
	AOC_t *pAOC = &(oSysInfo->AOC);

	pAOConf->Name = soap_strdup(soap, pAOC->AOname);
	pAOConf->UseCount = pAOC->AOusecount;
	pAOConf->token = soap_strdup(soap, pAOC->AOtoken);
	pAOConf->OutputToken = soap_strdup(soap, pAOC->AOutputToken);
	pAOConf->SendPrimacy = NULL;
	pAOConf->OutputLevel = pAOC->AOutputLevel;
	pAOConf->__size = 0; 
	pAOConf->__any = NULL;
	pAOConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioOutputConfigurationsResponse);
#endif
	UNUSED(trt__GetAudioOutputConfigurations);
}


/** Auto-test server operation __trt__GetAudioDecoderConfigurations */
int __trt__GetAudioDecoderConfigurations(struct soap *soap, 
struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations, 
struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();

	trt__GetAudioDecoderConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetAudioDecoderConfigurationsResponse->Configurations = (struct tt__AudioDecoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioDecoderConfiguration));
	memset(trt__GetAudioDecoderConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioDecoderConfiguration));
	struct tt__AudioDecoderConfiguration *pADConf = trt__GetAudioDecoderConfigurationsResponse->Configurations;
	ADC_t *pADC = &(oSysInfo->ADC);

	pADConf->Name = soap_strdup(soap, pADC->ADname);
	pADConf->UseCount = pADC->ADusecount;
	pADConf->token = soap_strdup(soap, pADC->ADtoken);
	pADConf->__size = 0;
	pADConf->__any = NULL;
	pADConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioDecoderConfigurationsResponse);
#endif
	UNUSED(trt__GetAudioDecoderConfigurations);
}

/** 
 * @brief Get Video Source Configuration
 * @This operation lists the video source configurations of a VideoSource.
 *   
 * @param Request	:_trt__GetVideoSourceConfiguration structure contains token of the video input.
 * @param Response	:_trt__GetVideoSourceConfigurationResponse structure contains requested VideoSourceConfiguration with the matching token. 
 * @return               : On success -  SOAP_OK.
 : On failure - No specific fault codes.
 */
int __trt__GetVideoSourceConfiguration(struct soap *soap, 
	struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration,
	struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(trt__GetVideoSourceConfiguration->ConfigurationToken, oSysInfo->VSC[idxVSC].Vtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	flprintf("token(%s) idx(%d)\r\n", trt__GetVideoSourceConfiguration->ConfigurationToken, idxVSC);

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetVideoSourceConfigurationResponse->Configuration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration));
	memset(trt__GetVideoSourceConfigurationResponse->Configuration, 0x00, sizeof(struct tt__VideoSourceConfiguration));

	struct tt__VideoSourceConfiguration *pVSConf = trt__GetVideoSourceConfigurationResponse->Configuration;
	VSC_t *pVSC = &(oSysInfo->VSC[idxVSC]);
	userVSCupdate(pVSC);

	pVSConf->Name = soap_strdup(soap, pVSC->Vname);
	pVSConf->token= soap_strdup(soap, pVSC->Vtoken);
	pVSConf->SourceToken = soap_strdup(soap, pVSC->Vsourcetoken);
	pVSConf->UseCount = pVSC->Vcount;//oSysInfo->Oprofile[i].VSC.Vcount;

	pVSConf->Bounds = (struct tt__IntRectangle *)soap_malloc(soap, sizeof(struct tt__IntRectangle));
	memset(pVSConf->Bounds, 0x00, sizeof(struct tt__IntRectangle));
	pVSConf->Bounds->x = pVSC->windowx;
	pVSConf->Bounds->y = pVSC->windowy;
	pVSConf->Bounds->width = pVSC->windowwidth;
	pVSConf->Bounds->height = pVSC->windowheight;
	pVSConf->__size = 0;
	pVSConf->__any = NULL;
	pVSConf->Extension = NULL;
	pVSConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

/** 
 * @brief Get Video Encoder Configuration
 * @This operation lists all existing video encoder configurations of an NVT.
 *   
 * @param Request	:_trt__GetVideoEncoderConfiguration structure contains an empty message.
 * @param Response	:_trt__GetVideoEncoderConfigurationResponse structure contains a list of all existing video encoder configurations in the NVT. 
 * @return               : On success - SOAP_OK.
 On failure - The requested configuration indicated with ConfigurationToken does not exist.
 */
int __trt__GetVideoEncoderConfiguration(struct soap *soap, 
struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, 
struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{	
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVEC;
	char _IPAddr[SMALL_INFO_LENGTH];

#if 1
	for (idxVEC = 0; idxVEC < oSysInfo->SizeDefaultVEC; idxVEC++) {
		if (strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, oSysInfo->VEC[idxVEC].VEtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetVideoEncoderConfigurationResponse->Configuration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration, 0x00, sizeof(struct tt__VideoEncoderConfiguration));

	struct tt__VideoEncoderConfiguration *pVEConf = trt__GetVideoEncoderConfigurationResponse->Configuration;
	VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);
	userVECupdate(pVEC, idxVEC);

	pVEConf->Name = soap_strdup(soap, pVEC->VEname);
	pVEConf->UseCount = pVEC->VEusercount;
	pVEConf->token = soap_strdup(soap, pVEC->VEtoken);
	pVEConf->Encoding = pVEC->Vencoder;

	pVEConf->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	memset(pVEConf->Resolution, 0x00, sizeof(struct tt__VideoResolution));
	pVEConf->Resolution->Width = pVEC->Rwidth;
	pVEConf->Resolution->Height = pVEC->Rheight;

	pVEConf->Quality = pVEC->VEquality; 

	pVEConf->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	memset(pVEConf->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
	pVEConf->RateControl->FrameRateLimit = oget_framerate2(pVEC->frameratelimit);   
	pVEConf->RateControl->EncodingInterval = pVEC->encodinginterval;

	pVEConf->MPEG4 = NULL;
	pVEConf->H264 = NULL;

	switch (pVEC->Vencoder) {
		case tt__VideoEncoding__JPEG:	// JPEG
			pVEConf->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);
			break;
		case tt__VideoEncoding__H264:	// H.264
			pVEConf->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
			pVEConf->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
			memset(pVEConf->H264, 0x00, sizeof(struct tt__H264Configuration));
			pVEConf->H264->GovLength = pVEC->H264govlength;
			pVEConf->H264->H264Profile = pVEC->H264Profile;//Baseline = 0, Main = 1, High = 3
			break;
		case tt__VideoEncoding__MPEG4:	// MPEG4
		default:
			pVEConf->RateControl->BitrateLimit = 0;
			printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
			break;
	}

#if 1
	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	pVEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(pVEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	pVEConf->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(pVEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	pVEConf->Multicast->Address->Type = pVEC->Multicast_Conf.nIPType;
	pVEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	pVEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
	pVEConf->Multicast->Address->IPv6Address = NULL;
	pVEConf->Multicast->Port = pVEC->Multicast_Conf.port;
	pVEConf->Multicast->TTL = pVEC->Multicast_Conf.ttl;
	pVEConf->Multicast->AutoStart = pVEC->Multicast_Conf.autostart;
	pVEConf->Multicast->__size = 0;
	pVEConf->Multicast->__any = NULL;
	pVEConf->Multicast->__anyAttribute = NULL;
#else
	pVEConf->Multicast = NULL;
#endif

	pVEConf->SessionTimeout = pVEC->sessiontimeout;

	pVEConf->__size = 0;
	pVEConf->__any = NULL;
	pVEConf->__anyAttribute = NULL;

#else
	int i, j;
	int index;
	int indexEncdr;
	int num_tokens = 0;

	char _IPAddr[30];

	sprintf(_IPAddr, "%s", getIpAddress(HTTP_ONVIF_PORT));

	flprintf("GET [%s]\r\n", trt__GetVideoEncoderConfiguration->ConfigurationToken);
	flprintf("\t%s-%d\r\n", oSysInfo->Oprofile[0].VECtoken, oSysInfo->Oprofile[0].sizeVEC);
	flprintf("\t%s-%d\r\n", oSysInfo->Oprofile[1].VECtoken, oSysInfo->Oprofile[1].sizeVEC);
	flprintf("\t%s-%d\r\n", oSysInfo->Oprofile[2].VECtoken, oSysInfo->Oprofile[2].sizeVEC);
	flprintf("\t%s-%d\r\n", oSysInfo->Oprofile[3].VECtoken, oSysInfo->Oprofile[3].sizeVEC);
	flprintf("\t%s-%d\r\n", oSysInfo->Oprofile[4].VECtoken, oSysInfo->Oprofile[4].sizeVEC);
	for(i = 0; i < oSysInfo->nProfileSize; i++)
	{
		for(j = 0; j < oSysInfo->Oprofile[i].sizeVEC; j++)
		{
			if(strcmp(trt__GetVideoEncoderConfiguration->ConfigurationToken, oSysInfo->Oprofile[i].VECtoken) == 0)
			{					
				Ptoken_exist = EXIST;
				index = i;
				break;
			}
		}
	}
	
	/* if ConfigurationToken does not exist */
	if(Ptoken_exist == NOT_EXIST) 
	{
		onvif_fault(soap,"","ter:InvalidArgVal", "ter:NoConfig","","");
		LEAVE();
		return SOAP_FAULT;
	}
	int idxVEC;
	idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[index].VECtoken);

	trt__GetVideoEncoderConfigurationResponse->Configuration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration, 0x00, sizeof(struct tt__VideoEncoderConfiguration));

	trt__GetVideoEncoderConfigurationResponse->Configuration->Name = soap_strdup(soap, oSysInfo->VEC[idxVEC].VEname);
	trt__GetVideoEncoderConfigurationResponse->Configuration->UseCount = oSysInfo->VEC[idxVEC].VEusercount;
	trt__GetVideoEncoderConfigurationResponse->Configuration->token = soap_strdup(soap, oSysInfo->VEC[idxVEC].VEtoken);
	//dvprintf("  oSysInfo->VEC[idxVEC].VEusercount %d \r\n", oSysInfo->VEC[idxVEC].VEusercount);
	//dvprintf("  oSysInfo->VEC[idxVEC].Vencoder %d \r\n", oSysInfo->VEC[idxVEC].Vencoder);
	//dvprintf("  oSysInfo->VEC[idxVEC].VEquality %f \r\n", oSysInfo->VEC[idxVEC].VEquality);
	trt__GetVideoEncoderConfigurationResponse->Configuration->Encoding = oSysInfo->VEC[idxVEC].Vencoder;

	
	trt__GetVideoEncoderConfigurationResponse->Configuration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration->Resolution, 0x00, sizeof(struct tt__VideoResolution));
	trt__GetVideoEncoderConfigurationResponse->Configuration->Resolution->Width = oSysInfo->VEC[idxVEC].Rwidth;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Resolution->Height = oSysInfo->VEC[idxVEC].Rheight;

	trt__GetVideoEncoderConfigurationResponse->Configuration->Quality = oSysInfo->VEC[idxVEC].VEquality; 
	//dvprintf("  trt__GetVideoEncoderConfigurationResponse->Configuration->Quality %f \r\n", trt__GetVideoEncoderConfigurationResponse->Configuration->Quality);

	
	trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
	trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl->FrameRateLimit = oget_framerate2(oSysInfo->VEC[idxVEC].frameratelimit);   
	trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl->EncodingInterval = oSysInfo->VEC[idxVEC].encodinginterval;

	trt__GetVideoEncoderConfigurationResponse->Configuration->MPEG4 = NULL;
	trt__GetVideoEncoderConfigurationResponse->Configuration->H264 = NULL;

	switch (oSysInfo->VEC[idxVEC].Vencoder) {
		case tt__VideoEncoding__JPEG:	// JPEG
			trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl->BitrateLimit = oget_bitratelimitJPEG(oSysInfo->VEC[idxVEC].bitratelimit);
			break;
		case tt__VideoEncoding__H264:	// H.264
			trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl->BitrateLimit = oget_bitratelimitH264(oSysInfo->VEC[idxVEC].bitratelimit);
			trt__GetVideoEncoderConfigurationResponse->Configuration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
			memset(trt__GetVideoEncoderConfigurationResponse->Configuration->H264, 0x00, sizeof(struct tt__H264Configuration));
			trt__GetVideoEncoderConfigurationResponse->Configuration->H264->GovLength = oSysInfo->VEC[idxVEC].H264govlength;
			trt__GetVideoEncoderConfigurationResponse->Configuration->H264->H264Profile = oSysInfo->VEC[idxVEC].H264Profile;//Baseline = 0, Main = 1, High = 3
			break;
		case tt__VideoEncoding__MPEG4:	// MPEG4
		default:
			trt__GetVideoEncoderConfigurationResponse->Configuration->RateControl->BitrateLimit = 0;
			printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, oSysInfo->VEC[idxVEC].Vencoder);
			break;
	}

	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast = NULL;
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Port = oSysInfo->VEC[idxVEC].Multicast_Conf.port;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->TTL = oSysInfo->VEC[idxVEC].Multicast_Conf.ttl;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->AutoStart = oSysInfo->VEC[idxVEC].Multicast_Conf.autostart;
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address->Type = oSysInfo->VEC[idxVEC].Multicast_Conf.nIPType;
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->Address->IPv6Address = NULL;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->__size = NULL;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->__any = NULL;
	trt__GetVideoEncoderConfigurationResponse->Configuration->Multicast->__anyAttribute = NULL;

	dvprintf("GetVideoEncoderConfigurationResponse >>%ld \r\n", oSysInfo->VEC[idxVEC].sessiontimeout);
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->SessionTimeout = oSysInfo->VEC[idxVEC].sessiontimeout;	

	dvprintf("GetVideoEncoderConfigurationResponse <<%ld \r\n",trt__GetVideoEncoderConfigurationResponse->Configuration->SessionTimeout);
	
	trt__GetVideoEncoderConfigurationResponse->Configuration->__size = 0;	/* sequence of elements <-any> */
	trt__GetVideoEncoderConfigurationResponse->Configuration->__any = NULL;
	trt__GetVideoEncoderConfigurationResponse->Configuration->__anyAttribute = NULL;	/* optional attribute of type xsd:anyType */
#endif

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __trt__GetAudioSourceConfiguration */
int __trt__GetAudioSourceConfiguration(struct soap *soap, 
struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration, 
struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxASC;

	for (idxASC = 0; idxASC < oSysInfo->SizeDefaultASC; idxASC++) {
		if (strcmp(trt__GetAudioSourceConfiguration->ConfigurationToken, oSysInfo->ASC[idxASC].Atoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	flprintf("token(%s) idx(%d)\r\n", trt__GetAudioSourceConfiguration->ConfigurationToken, idxASC);

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetAudioSourceConfigurationResponse->Configuration = (struct tt__AudioSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration));
	memset(trt__GetAudioSourceConfigurationResponse->Configuration, 0x00, sizeof(struct tt__AudioSourceConfiguration));

	struct tt__AudioSourceConfiguration *pASConf = trt__GetAudioSourceConfigurationResponse->Configuration;
	ASC_t *pASC = &(oSysInfo->ASC[idxASC]);

	pASConf->Name = soap_strdup(soap, pASC->Aname);
	pASConf->UseCount = pASC->Acount;
	pASConf->token = soap_strdup(soap, pASC->Atoken);
	pASConf->SourceToken= soap_strdup(soap, pASC->Asourcetoken);
	pASConf->__size = 0; 
	pASConf->__any = NULL;
	pASConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioSourceConfiguration);
	UNUSED(trt__GetAudioSourceConfigurationResponse);
#endif
}


/** Auto-test server operation __trt__GetAudioEncoderConfiguration */
int __trt__GetAudioEncoderConfiguration(struct soap *soap, 
struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration, 
struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxAEC;
	char _IPAddr[SMALL_INFO_LENGTH];

	for (idxAEC = 0; idxAEC < oSysInfo->SizeDefaultAEC; idxAEC++) {
		if (strcmp(trt__GetAudioEncoderConfiguration->ConfigurationToken, oSysInfo->AEC[idxAEC].AEtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetAudioEncoderConfigurationResponse->Configuration = (struct tt__AudioEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration));
	memset(trt__GetAudioEncoderConfigurationResponse->Configuration, 0x0, sizeof(struct tt__AudioEncoderConfiguration));

	struct tt__AudioEncoderConfiguration *pAEConf = trt__GetAudioEncoderConfigurationResponse->Configuration;
	AEC_t *pAEC = &(oSysInfo->AEC[idxAEC]);

	pAEConf->Name = soap_strdup(soap, pAEC->AEname);
	pAEConf->UseCount = pAEC->AEusercount;
	pAEConf->token = soap_strdup(soap, pAEC->AEtoken);
	pAEConf->Encoding = pAEC->Aencoder;
	pAEConf->Bitrate = pAEC->Abitratelimit;
	pAEConf->SampleRate = pAEC->Asamplerate;

#if 1
	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));
	
	pAEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(pAEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	pAEConf->Multicast->Address = (struct tt__IPAddress*)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(pAEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	pAEConf->Multicast->Address->Type = pAEC->Multicast_Conf.nIPType;
	pAEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	pAEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
	pAEConf->Multicast->Address->IPv6Address = NULL;
	pAEConf->Multicast->Port = pAEC->Multicast_Conf.port;	
	pAEConf->Multicast->TTL = pAEC->Multicast_Conf.ttl;	
	pAEConf->Multicast->AutoStart = pAEC->Multicast_Conf.autostart;
	pAEConf->Multicast->__size = 0;
	pAEConf->Multicast->__any = NULL;
	pAEConf->Multicast->__anyAttribute = NULL;
#else
	pAEConf->Multicast = NULL;
#endif

	pAEConf->SessionTimeout = pAEC->sessiontimeout;

	pAEConf->__size = 0;
	pAEConf->__any = NULL;
	pAEConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioEncoderConfiguration);
	UNUSED(trt__GetAudioEncoderConfigurationResponse);
#endif
}


/** Auto-test server operation __trt__GetVideoAnalyticsConfiguration */
int __trt__GetVideoAnalyticsConfiguration(struct soap *soap, 
struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration, 
struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__GetVideoAnalyticsConfiguration);
	UNUSED(trt__GetVideoAnalyticsConfigurationResponse);
}

/** 
 * @brief Get Meta Data Configuration
 * @This operation lists all video analytics configurations of a device.
 *   
 * @param Request	:_trt__GetAudioEncoderConfiguration structure contains an empty message.
 * @param Response	:_trt__GetAudioEncoderConfigurationResponse structure contains a list of all existing video analytics configurations in the device.
 * @return               : On success - SOAP_OK.
 On failure - The requested configuration indicated with ConfigurationToken does not exist

 */
int __trt__GetMetadataConfiguration(struct soap *soap, 
struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration, 
struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse)
{
	ENTER();
#if 0	
	SysInfo *oSysInfo = GetSysInfo();
	//char _name[LARGE_INFO_LENGTH];
	int i = 0,token_exist = NOT_EXIST;
	int index;
	char _IPAddr[INFO_LENGTH];
	//NET_IPV4 ip;
	//ip.int32 = net_get_ifaddr(ETH_IF_NAME);

	sprintf(_IPAddr, ONVIF_SERVICE_ADDRESS, getIpAddress(HTTP_ONVIF_PORT));
	for(i = 0; i< oSysInfo->nProfileSize; i++)
	{
		if(strcmp(trt__GetMetadataConfiguration->ConfigurationToken, GetSysInfo()->Oprofile[ i].profiletoken) == 0)
		{
			token_exist = EXIST;
			index = i;
			break;
		}
	}
	if(token_exist == NOT_EXIST)
	{
		onvif_fault(soap,"","ter:InvalidArgVal", "ter:NoProfile","","");
		return SOAP_FAULT;
	}

	trt__GetMetadataConfigurationResponse->Configuration = (struct tt__MetadataConfiguration *)soap_malloc(soap, sizeof(struct tt__MetadataConfiguration));
	memset(trt__GetMetadataConfigurationResponse->Configuration, 0x00, sizeof(struct tt__MetadataConfiguration));
	trt__GetMetadataConfigurationResponse->Configuration->Name = soap_strdup(soap, "test"/*GetSysInfo()->Oprofile[index].MDC.MDname*/);//"Face Detection";
	//trt__GetMetadataConfigurationResponse->Configuration->Name = (char*) soap_malloc(soap, sizeof(char)*50);
	//strcpy( trt__GetMetadataConfigurationResponse->Configuration->Name, "test" );
	
	trt__GetMetadataConfigurationResponse->Configuration->UseCount = 2;//GetSysInfo()->Oprofile[index].MDC.MDusecount;//1;
	trt__GetMetadataConfigurationResponse->Configuration->token = soap_strdup(soap, "test"/*GetSysInfo()->Oprofile[index].MDC.MDtoken*/);//"Face Detection";
	//trt__GetMetadataConfigurationResponse->Configuration->token = (char*) soap_malloc(soap, sizeof(char)*50);
	//strcpy( trt__GetMetadataConfigurationResponse->Configuration->token, "test");
	
	trt__GetMetadataConfigurationResponse->Configuration->PTZStatus =  (struct tt__PTZFilter*)soap_malloc(soap,sizeof(struct tt__PTZFilter));
	memset(trt__GetMetadataConfigurationResponse->Configuration->PTZStatus, 0x00, sizeof(struct tt__PTZFilter));
	trt__GetMetadataConfigurationResponse->Configuration->PTZStatus->Position = 0;
	trt__GetMetadataConfigurationResponse->Configuration->PTZStatus->Status = 0;
	trt__GetMetadataConfigurationResponse->Configuration->PTZStatus->__anyAttribute = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Events = (struct tt__EventSubscription*)soap_malloc(soap,sizeof(struct tt__EventSubscription));
	memset(trt__GetMetadataConfigurationResponse->Configuration->Events, 0x00, sizeof(struct tt__EventSubscription));
	trt__GetMetadataConfigurationResponse->Configuration->Events->Filter = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Events->SubscriptionPolicy = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Events->__size = 0;
	trt__GetMetadataConfigurationResponse->Configuration->Events->__any = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Events->__anyAttribute = NULL;

	trt__GetMetadataConfigurationResponse->Configuration->Analytics = (int *)soap_malloc(soap, sizeof(int));
	*(trt__GetMetadataConfigurationResponse->Configuration->Analytics) = 1;

	trt__GetMetadataConfigurationResponse->Configuration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(trt__GetMetadataConfigurationResponse->Configuration->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->Type = GetSysInfo()->Oprofile[index].MDC.VMC.nIPType;
	
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	
	*(trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->IPv4Address) = soap_strdup(soap, _IPAddr);
	//*(trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->IPv4Address) = (char*) soap_malloc(soap, sizeof(char)*50);
	//strcpy( *(trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->IPv4Address), _IPAddr);
	
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->Address->IPv6Address = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->Port = GetSysInfo()->Oprofile[ index].MDC.VMC.port;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->TTL = GetSysInfo()->Oprofile[ index].MDC.VMC.ttl;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->AutoStart = GetSysInfo()->Oprofile[ index].MDC.VMC.autostart;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->__size = 0;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->__any = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Multicast->__anyAttribute = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->SessionTimeout = 72000;
	trt__GetMetadataConfigurationResponse->Configuration->__size = 0;		
	trt__GetMetadataConfigurationResponse->Configuration->__any = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->AnalyticsEngineConfiguration = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->Extension = NULL;
	trt__GetMetadataConfigurationResponse->Configuration->__anyAttribute = NULL;

#endif
	LEAVE();
	return SOAP_OK;
	
	UNUSED(soap);
	UNUSED(trt__GetMetadataConfiguration);
	UNUSED(trt__GetMetadataConfigurationResponse);
}


/** Auto-test server operation __trt__GetAudioOutputConfiguration */
int __trt__GetAudioOutputConfiguration(struct soap *soap, 
struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration, 
struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();

	flprintf("token(%s)\r\n", trt__GetAudioOutputConfiguration->ConfigurationToken);

	if (strcmp(trt__GetAudioOutputConfiguration->ConfigurationToken, oSysInfo->AOC.AOtoken) != 0) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetAudioOutputConfigurationResponse->Configuration = (struct tt__AudioOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfiguration));
	memset(trt__GetAudioOutputConfigurationResponse->Configuration, 0x00, sizeof(struct tt__AudioOutputConfiguration));
	struct tt__AudioOutputConfiguration *pAOConf = trt__GetAudioOutputConfigurationResponse->Configuration;
	AOC_t *pAOC = &(oSysInfo->AOC);

	pAOConf->Name = soap_strdup(soap, pAOC->AOname);
	pAOConf->UseCount = pAOC->AOusecount;
	pAOConf->token = soap_strdup(soap, pAOC->AOtoken);
	pAOConf->OutputToken = soap_strdup(soap, pAOC->AOutputToken);
	pAOConf->SendPrimacy = NULL;
	pAOConf->OutputLevel = pAOC->AOutputLevel;
	pAOConf->__size = 0;
	pAOConf->__any = NULL;
	pAOConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioOutputConfiguration);
	UNUSED(trt__GetAudioOutputConfigurationResponse);
#endif
}


/** Auto-test server operation __trt__GetAudioDecoderConfiguration */
int __trt__GetAudioDecoderConfiguration(struct soap *soap, 
struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration, 
struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();

	flprintf("token(%s)\r\n", trt__GetAudioDecoderConfiguration->ConfigurationToken);

	if (strcmp(trt__GetAudioDecoderConfiguration->ConfigurationToken, oSysInfo->ADC.ADtoken) != 0) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetAudioDecoderConfigurationResponse->Configuration = (struct tt__AudioDecoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioDecoderConfiguration));
	memset(trt__GetAudioDecoderConfigurationResponse->Configuration, 0x00, sizeof(struct tt__AudioDecoderConfiguration));

	struct tt__AudioDecoderConfiguration *pADConf = trt__GetAudioDecoderConfigurationResponse->Configuration;
	ADC_t *pADC = &(oSysInfo->ADC);

	pADConf->Name = soap_strdup(soap, pADC->ADname);
	pADConf->UseCount = pADC->ADusecount;
	pADConf->token = soap_strdup(soap, pADC->ADtoken);
	pADConf->__size = 0;
	pADConf->__any = NULL;
	pADConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioDecoderConfiguration);
	UNUSED(trt__GetAudioDecoderConfigurationResponse);
#endif
}


/** Auto-test server operation __trt__GetCompatibleVideoEncoderConfigurations */
int __trt__GetCompatibleVideoEncoderConfigurations(struct soap *soap, 
struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations, 
struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVSC, idxVEC;
	char _IPAddr[SMALL_INFO_LENGTH];

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__GetCompatibleVideoEncoderConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	idxVSC = MatchProfileVSCToken(oSysInfo->Oprofile[idxPROF].VSCtoken);
	printf("VSC(%s) idx(%d) cnt(%d)\r\n", oSysInfo->Oprofile[idxPROF].VSCtoken, idxVSC, oSysInfo->VSC[idxVSC].Vcount);
	trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations = oSysInfo->VSC[idxVSC].Vcount;

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__VideoEncoderConfiguration)*trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations);

#if 1
	for (idxVEC = 0; idxVEC < trt__GetCompatibleVideoEncoderConfigurationsResponse->__sizeConfigurations; idxVEC++) {
		struct tt__VideoEncoderConfiguration *pVEConf = &(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations[idxVEC]);
		VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);
		userVECupdate(pVEC, idxVEC);

		pVEConf->Name = soap_strdup(soap, pVEC->VEname);
		pVEConf->UseCount = pVEC->VEusercount;
		pVEConf->token = soap_strdup(soap, pVEC->VEtoken);
		pVEConf->Encoding = pVEC->Vencoder;

		pVEConf->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
		memset(pVEConf->Resolution, 0x00, sizeof(struct tt__VideoResolution));
		pVEConf->Resolution->Width = pVEC->Rwidth;
		pVEConf->Resolution->Height = pVEC->Rheight;

		pVEConf->Quality = pVEC->VEquality;

		pVEConf->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
		memset(pVEConf->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
		pVEConf->RateControl->FrameRateLimit = oget_framerate2(pVEC->frameratelimit);
		pVEConf->RateControl->EncodingInterval = pVEC->encodinginterval;

		pVEConf->MPEG4 = NULL;
		pVEConf->H264 = NULL;

		switch (pVEC->Vencoder) {
			case tt__VideoEncoding__JPEG:	// JPEG
				pVEConf->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);
				break;

			case tt__VideoEncoding__H264:	// H.264
				pVEConf->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
				pVEConf->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
				memset(pVEConf->H264, 0x00, sizeof(struct tt__H264Configuration));
				pVEConf->H264->GovLength = pVEC->H264govlength;
				pVEConf->H264->H264Profile = pVEC->H264Profile;//Baseline = 0, Main = 1, High = 3
				break;

			case tt__VideoEncoding__MPEG4:	// MPEG4
			default:
				pVEConf->RateControl->BitrateLimit = 0;
				printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
				break;
		}

#if 1
		pVEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pVEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));

		pVEConf->Multicast->Port = pVEC->Multicast_Conf.port;
		pVEConf->Multicast->TTL = pVEC->Multicast_Conf.ttl;
		pVEConf->Multicast->AutoStart = pVEC->Multicast_Conf.autostart;

		pVEConf->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		memset(pVEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
		pVEConf->Multicast->Address->Type = pVEC->Multicast_Conf.nIPType;
		pVEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pVEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
		pVEConf->Multicast->Address->IPv6Address = NULL;

		pVEConf->Multicast->__size = 0;
		pVEConf->Multicast->__any = NULL;
		pVEConf->Multicast->__anyAttribute = NULL;
#else
		pVEConf->Multicast = NULL;
#endif

		pVEConf->SessionTimeout = pVEC->sessiontimeout;
		pVEConf->__size = 0;
		pVEConf->__any = NULL;
		pVEConf->__anyAttribute = NULL;
	}
#else
	idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[idxPROF].VECtoken);
	if (idxVEC == -1) {
		flprintf("Error, idxProf(%d) idxVEC(%d) token(%s)\r\n", idxPROF, idxVEC, oSysInfo->Oprofile[idxPROF].VECtoken);
	}

	VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);
	userVECupdate(pVEC, idxVEC);

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Name = soap_strdup(soap, pVEC->VEname);
	
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->token = soap_strdup(soap, pVEC->VEtoken);
	
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->UseCount = pVEC->VEusercount;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Encoding = pVEC->Vencoder;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Quality = pVEC->VEquality; 
	
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Resolution, 0x00, sizeof(struct tt__VideoResolution));
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Resolution->Width = pVEC->Rwidth;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Resolution->Height = pVEC->Rheight;
		
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl, 0x00, sizeof(struct tt__VideoRateControl));
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl->FrameRateLimit = oget_framerate2(pVEC->frameratelimit);
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl->EncodingInterval = pVEC->encodinginterval;

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->MPEG4 = NULL;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->H264 = NULL;

	switch (pVEC->Vencoder) {
		case tt__VideoEncoding__JPEG:	// JPEG
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl->BitrateLimit = oget_bitratelimitJPEG(pVEC->bitratelimit);
			break;

		case tt__VideoEncoding__H264:	// H.264
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl->BitrateLimit = oget_bitratelimitH264(pVEC->bitratelimit);
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
			memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->H264, 0x00, sizeof(struct tt__H264Configuration));
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->H264->GovLength = pVEC->H264govlength;
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->H264->H264Profile = pVEC->H264Profile;//Baseline = 0, Main = 1, High = 3
			break;

		case tt__VideoEncoding__MPEG4:	// MPEG4
		default:
			trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->RateControl->BitrateLimit = 0;
			printf("%s(%d) : Vencoder Error(%d)!\r\n", __func__, __LINE__, pVEC->Vencoder);
			break;
	}

#if 1
	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Port = pVEC->Multicast_Conf.port;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->TTL = pVEC->Multicast_Conf.ttl;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->AutoStart = pVEC->Multicast_Conf.autostart;
	
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	memset(trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address->Type = pVEC->Multicast_Conf.nIPType;

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->Address->IPv6Address = NULL;

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->__size = 0;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->__any = NULL;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast->__anyAttribute = NULL;
#else
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->Multicast = NULL;
#endif

	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->SessionTimeout = pVEC->sessiontimeout;	
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->__size = 0;	/* sequence of elements <-any> */
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->__any = NULL;
	trt__GetCompatibleVideoEncoderConfigurationsResponse->Configurations->__anyAttribute = NULL;	/* optional attribute of type xsd:anyType */
#endif

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __trt__GetCompatibleVideoSourceConfigurations */
int __trt__GetCompatibleVideoSourceConfigurations(struct soap *soap, 
struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations, 
struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVSC;
	int i = 0;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if(strcmp(trt__GetCompatibleVideoSourceConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetCompatibleVideoSourceConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetCompatibleVideoSourceConfigurationsResponse->Configurations = (struct tt__VideoSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfiguration) * trt__GetCompatibleVideoSourceConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleVideoSourceConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__VideoSourceConfiguration) * trt__GetCompatibleVideoSourceConfigurationsResponse->__sizeConfigurations);

	i = 0; // for (i = 0 ; i < sizeConfigurations ; i++ ) { ...
	struct tt__VideoSourceConfiguration *pVSConf = &(trt__GetCompatibleVideoSourceConfigurationsResponse->Configurations[i]);
	idxVSC = 0; // VSC는 1개이므로 index는 임의로 0으로 설정. VSC가 1개 이상일 경우 전면적 수정 필요
	VSC_t *pVSC = &(oSysInfo->VSC[idxVSC]);
	userVSCupdate(pVSC);

	flprintf("idx(%d) VSC(%s) idxVSC(%d)\r\n", i, pVSC->Vtoken, idxVSC);
	pVSConf->Name = soap_strdup(soap, pVSC->Vname);
	pVSConf->UseCount = pVSC->Vcount;
	pVSConf->token = soap_strdup(soap, pVSC->Vtoken);
	pVSConf->SourceToken = soap_strdup(soap, pVSC->Vsourcetoken);
	pVSConf->Bounds = (struct tt__IntRectangle *)soap_malloc(soap,sizeof(struct tt__IntRectangle));
	pVSConf->Bounds->x = pVSC->windowx;
	pVSConf->Bounds->y = pVSC->windowy;
	pVSConf->Bounds->width = pVSC->windowwidth;
	pVSConf->Bounds->height = pVSC->windowheight;
	pVSConf->__size = 0;
	pVSConf->__any = NULL;
	pVSConf->Extension = NULL;
	pVSConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __trt__GetCompatibleAudioEncoderConfigurations */
int __trt__GetCompatibleAudioEncoderConfigurations(struct soap *soap, 
struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations, 
struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxASC, idxAEC;
	char _IPAddr[SMALL_INFO_LENGTH];

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__GetCompatibleAudioEncoderConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	snprintf(_IPAddr, SMALL_INFO_LENGTH, "%s", getIpAddress(HTTP_ONVIF_PORT));

	idxASC = MatchProfileASCToken(oSysInfo->Oprofile[idxPROF].ASCtoken);
	flprintf("ASC(%s) idx(%d) cnt(%d)\r\n", oSysInfo->Oprofile[idxPROF].ASCtoken, idxASC, oSysInfo->ASC[idxASC].Acount);
	trt__GetCompatibleAudioEncoderConfigurationsResponse->__sizeConfigurations = oSysInfo->ASC[idxASC].Acount;

	trt__GetCompatibleAudioEncoderConfigurationsResponse->Configurations = (struct tt__AudioEncoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfiguration) * trt__GetCompatibleAudioEncoderConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleAudioEncoderConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioEncoderConfiguration) * trt__GetCompatibleAudioEncoderConfigurationsResponse->__sizeConfigurations);

	for (idxAEC = 0; idxAEC < trt__GetCompatibleAudioEncoderConfigurationsResponse->__sizeConfigurations; idxAEC++) {
		struct tt__AudioEncoderConfiguration *pAEConf = &(trt__GetCompatibleAudioEncoderConfigurationsResponse->Configurations[idxAEC]);
		AEC_t *pAEC = &(oSysInfo->AEC[idxAEC]);

		pAEConf->Name = soap_strdup(soap, pAEC->AEname);
		pAEConf->UseCount = pAEC->AEusercount;
		pAEConf->token = soap_strdup(soap, pAEC->AEtoken);
		pAEConf->Encoding = pAEC->Aencoder;
		pAEConf->Bitrate = pAEC->Abitratelimit;
		pAEConf->SampleRate = pAEC->Asamplerate;
		pAEConf->SessionTimeout = pAEC->sessiontimeout;

#if 1
		pAEConf->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		memset(pAEConf->Multicast, 0x00, sizeof(struct tt__MulticastConfiguration));

		pAEConf->Multicast->Port = pAEC->Multicast_Conf.port;
		pAEConf->Multicast->TTL = pAEC->Multicast_Conf.ttl;
		pAEConf->Multicast->AutoStart = pAEC->Multicast_Conf.autostart;

		pAEConf->Multicast->Address  = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		memset(pAEConf->Multicast->Address, 0x00, sizeof(struct tt__IPAddress));
		pAEConf->Multicast->Address->Type = pAEC->Multicast_Conf.nIPType;
		pAEConf->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		pAEConf->Multicast->Address->IPv4Address[0] = soap_strdup(soap, _IPAddr);
		pAEConf->Multicast->Address->IPv6Address = NULL;

		pAEConf->Multicast->__size = 0;
		pAEConf->Multicast->__any = NULL;
		pAEConf->Multicast->__anyAttribute = NULL;
#else
		pAEConf->Multicast = NULL;
#endif

		pAEConf->__size = 0;
		pAEConf->__any = NULL;
		pAEConf->__anyAttribute = NULL;
	}

#if 0
struct tt__AudioEncoderConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	enum tt__AudioEncoding Encoding;	/* required element of type tt:AudioEncoding */
	int Bitrate;	/* required element of type xsd:int */
	int SampleRate;	/* required element of type xsd:int */
	struct tt__MulticastConfiguration *Multicast;	/* required element of type tt:MulticastConfiguration */
	LONG64 SessionTimeout;	/* external */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

















	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetCompatibleAudioEncoderConfigurations);
	UNUSED(trt__GetCompatibleAudioEncoderConfigurationsResponse);
#endif
}


/** Auto-test server operation __trt__GetCompatibleAudioSourceConfigurations */
int __trt__GetCompatibleAudioSourceConfigurations(struct soap *soap, 
struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations, 
struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxASC;
	int i = 0;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if(strcmp(trt__GetCompatibleAudioSourceConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetCompatibleAudioSourceConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetCompatibleAudioSourceConfigurationsResponse->Configurations = (struct tt__AudioSourceConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfiguration) * trt__GetCompatibleAudioSourceConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleAudioSourceConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioSourceConfiguration) * trt__GetCompatibleAudioSourceConfigurationsResponse->__sizeConfigurations);

	i = 0; // for (i = 0 ; i < sizeConfigurations ; i++ ) { ...
	struct tt__AudioSourceConfiguration *pASConf = &(trt__GetCompatibleAudioSourceConfigurationsResponse->Configurations[i]);
	idxASC = 0; // ASC는 1개이므로 index는 임의로 0으로 설정. ASC가 1개 이상일 경우 전면적 수정 필요
	ASC_t *pASC = &(oSysInfo->ASC[idxASC]);

	flprintf("idx(%d) ASC(%s) idxASC(%d)\r\n", i, pASC->Atoken, idxASC);
	pASConf->Name = soap_strdup(soap, pASC->Aname);
	pASConf->UseCount = pASC->Acount;
	pASConf->token = soap_strdup(soap, pASC->Atoken);
	pASConf->SourceToken = soap_strdup(soap, pASC->Asourcetoken);
	pASConf->__size = 0;
	pASConf->__any = NULL;
	pASConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetCompatibleAudioSourceConfigurations);
	UNUSED(trt__GetCompatibleAudioSourceConfigurationsResponse);
#endif
}


/** Auto-test server operation __trt__GetCompatibleVideoAnalyticsConfigurations */
int __trt__GetCompatibleVideoAnalyticsConfigurations(struct soap *soap, 
struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations, 
struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__GetCompatibleVideoAnalyticsConfigurations);
	UNUSED(trt__GetCompatibleVideoAnalyticsConfigurationsResponse);
}


/** Auto-test server operation __trt__GetCompatibleMetadataConfigurations */
int __trt__GetCompatibleMetadataConfigurations(struct soap *soap, 
struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations, 
struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetCompatibleMetadataConfigurations);
	UNUSED(trt__GetCompatibleMetadataConfigurationsResponse);
}


/** Auto-test server operation __trt__GetCompatibleAudioOutputConfigurations */
int __trt__GetCompatibleAudioOutputConfigurations(struct soap *soap, 
struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations, 
struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if(strcmp(trt__GetCompatibleAudioOutputConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetCompatibleAudioOutputConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetCompatibleAudioOutputConfigurationsResponse->Configurations = (struct tt__AudioOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfiguration) * trt__GetCompatibleAudioOutputConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleAudioOutputConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioOutputConfiguration) * trt__GetCompatibleAudioOutputConfigurationsResponse->__sizeConfigurations);
	struct tt__AudioOutputConfiguration *pAOConf = trt__GetCompatibleAudioOutputConfigurationsResponse->Configurations;

	AOC_t *pAOC = &(oSysInfo->AOC);
	pAOConf->Name = soap_strdup(soap, pAOC->AOname);
	pAOConf->UseCount = pAOC->AOusecount;
	pAOConf->token = soap_strdup(soap, pAOC->AOtoken);
	pAOConf->OutputToken = soap_strdup(soap, pAOC->AOutputToken);
	pAOConf->SendPrimacy = NULL;
	pAOConf->OutputLevel = pAOC->AOutputLevel;
	pAOConf->__size = 0;
	pAOConf->__any = NULL;
	pAOConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetCompatibleAudioOutputConfigurations);
	UNUSED(trt__GetCompatibleAudioOutputConfigurationsResponse);
#endif
}


/** Auto-test server operation __trt__GetCompatibleAudioDecoderConfigurations */
int __trt__GetCompatibleAudioDecoderConfigurations(struct soap *soap, 
struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations, 
struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if(strcmp(trt__GetCompatibleAudioDecoderConfigurations->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetCompatibleAudioDecoderConfigurationsResponse->__sizeConfigurations = 1;

	trt__GetCompatibleAudioDecoderConfigurationsResponse->Configurations = (struct tt__AudioDecoderConfiguration *)soap_malloc(soap, sizeof(struct tt__AudioDecoderConfiguration) * trt__GetCompatibleAudioDecoderConfigurationsResponse->__sizeConfigurations);
	memset(trt__GetCompatibleAudioDecoderConfigurationsResponse->Configurations, 0x00, sizeof(struct tt__AudioDecoderConfiguration) * trt__GetCompatibleAudioDecoderConfigurationsResponse->__sizeConfigurations);
	struct tt__AudioDecoderConfiguration *pADConf = trt__GetCompatibleAudioDecoderConfigurationsResponse->Configurations;

	ADC_t *pADC = &(oSysInfo->ADC);
	pADConf->Name = soap_strdup(soap, pADC->ADname);
	pADConf->UseCount = pADC->ADusecount;
	pADConf->token = soap_strdup(soap, pADC->ADtoken);
	pADConf->__size = 0;
	pADConf->__any = NULL;
	pADConf->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__GetCompatibleAudioDecoderConfigurations);
	UNUSED(trt__GetCompatibleAudioDecoderConfigurationsResponse);
#endif
}


/** Auto-test server operation __trt__SetVideoSourceConfiguration */
int __trt__SetVideoSourceConfiguration(struct soap *soap, 
struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration, 
struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(trt__SetVideoSourceConfiguration->Configuration->token, oSysInfo->VSC[idxVSC].Vtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	flprintf("token(%s) idx(%d)\r\n", trt__SetVideoSourceConfiguration->Configuration->token, idxVSC);

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	int _width = 0, _height = 0;
	if (trt__SetVideoSourceConfiguration->Configuration->Bounds != NULL) {		 
		_width = trt__SetVideoSourceConfiguration->Configuration->Bounds->width;
		_height = trt__SetVideoSourceConfiguration->Configuration->Bounds->height;
	}

	if (!((_width >= WidthRangeMin && _width <= WidthRangeMax) && (_height >= HeightRangeMin && _height <= HeightRangeMax))) {		
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "<env:Text>out of range</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(oSysInfo->VSC[idxVSC].Vname, trt__SetVideoSourceConfiguration->Configuration->Name);
	//oSysInfo->VSC[idxVSC].Vcount = trt__SetVideoSourceConfiguration->Configuration->UseCount;
	oSysInfo->VSC[idxVSC].windowwidth = trt__SetVideoSourceConfiguration->Configuration->Bounds->width;
	oSysInfo->VSC[idxVSC].windowheight = trt__SetVideoSourceConfiguration->Configuration->Bounds->height;
	oSysInfo->VSC[idxVSC].windowx= trt__SetVideoSourceConfiguration->Configuration->Bounds->x;
	oSysInfo->VSC[idxVSC].windowy = trt__SetVideoSourceConfiguration->Configuration->Bounds->y;
	//trt__SetVideoSourceConfiguration->Configuration->__size;
	//trt__SetVideoSourceConfiguration->Configuration->__any;
	//trt__SetVideoSourceConfiguration->Configuration->Extension;	
	//trt__SetVideoSourceConfiguration->Configuration->__anyAttribute;	

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__SetVideoSourceConfigurationResponse);
}


/**
 * @brief Set Video Encoder Configuration 
 * @This operation modifies a video encoder configuration.
 *   
 * @param Request	:_trt__SetVideoEncoderConfiguration structure contains the configuration element contains the modified video encoder configuration. 
 * @param Response	:_trt__SetVideoEncoderConfigurationResponse structure contains message is empty.
 * @return               : On success - SOAP_OK.
 On failure - The configuration does not exist.or
 The configuration parameters are not possible to set.
 */
int __trt__SetVideoEncoderConfiguration(struct soap *soap, 
struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, 
struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{	
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVEC;
	int i;
	UINT _width = 0, _height = 0;
	float _quality = 0.0;
	Resolution codec_res = e_resEndorUnknown;

	for (idxVEC = 0; idxVEC < oSysInfo->SizeDefaultVEC; idxVEC++) {
		if (strcmp(trt__SetVideoEncoderConfiguration->Configuration->token, oSysInfo->VEC[idxVEC].VEtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {		
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->Resolution != NULL) {		 
		_width = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Width;		 
		_height = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Height;  
	}

	if (!((_width >= WidthRangeMin && _width <= WidthRangeMax) && (_height >= HeightRangeMin && _height <= HeightRangeMax))) {		
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "<env:Text>out of range</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}

	for (i = 0; i < ResolutionInfoLength; i++) {
		if ((_width == listResolution[i].nWidth) && (_height == listResolution[i].nHeight)) {
			codec_res = listResolution[i].rIndex;
			break;
		}
	}

	if (codec_res == e_resEndorUnknown) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "<env:Text>SET Resolution ERROR</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->H264) {
		if (trt__SetVideoEncoderConfiguration->Configuration->H264->GovLength > H264_Idrframe_MAX) {
			onvifInterfaceFault(soap, ovfSC_ConfigModify, "<env:Text>SET GovLength ERROR</env:Text>");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	_quality = trt__SetVideoEncoderConfiguration->Configuration->Quality;
#if 1
	if (!(_quality >= ONVIF_Quality_MIN && _quality <= ONVIF_Quality_MAX)) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "<env:Text>out of range</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}
#endif
	VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);

	strcpy(pVEC->VEname, trt__SetVideoEncoderConfiguration->Configuration->Name);
//	pVEC->VEusercount = trt__SetVideoEncoderConfiguration->Configuration->UseCount;
	pVEC->Vencoder = trt__SetVideoEncoderConfiguration->Configuration->Encoding;
	if (trt__SetVideoEncoderConfiguration->Configuration->Resolution) {
		pVEC->Rwidth = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Width;
		pVEC->Rheight = trt__SetVideoEncoderConfiguration->Configuration->Resolution->Height;
	}

	pVEC->VEquality = _quality;

	if (trt__SetVideoEncoderConfiguration->Configuration->RateControl) {
		pVEC->frameratelimit = oset_framerate(trt__SetVideoEncoderConfiguration->Configuration->RateControl->FrameRateLimit);
		pVEC->encodinginterval = trt__SetVideoEncoderConfiguration->Configuration->RateControl->EncodingInterval;
		if(trt__SetVideoEncoderConfiguration->Configuration->Encoding == tt__VideoEncoding__JPEG) {
			pVEC->bitratelimit = oset_bitratelimitJPEG(trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit);
		} else if(trt__SetVideoEncoderConfiguration->Configuration->Encoding == tt__VideoEncoding__H264) {
			pVEC->bitratelimit = oset_bitratelimitH264(trt__SetVideoEncoderConfiguration->Configuration->RateControl->BitrateLimit);
		} else {
			onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->MPEG4) {
		pVEC->Mpeggovlength = trt__SetVideoEncoderConfiguration->Configuration->MPEG4->GovLength;
		pVEC->Mpegprofile = trt__SetVideoEncoderConfiguration->Configuration->MPEG4->Mpeg4Profile;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->H264) {
		pVEC->H264Profile = trt__SetVideoEncoderConfiguration->Configuration->H264->H264Profile;
		pVEC->H264govlength = trt__SetVideoEncoderConfiguration->Configuration->H264->GovLength;
	}

	if (trt__SetVideoEncoderConfiguration->Configuration->Multicast) {
		pVEC->Multicast_Conf.autostart = trt__SetVideoEncoderConfiguration->Configuration->Multicast->AutoStart;
		pVEC->Multicast_Conf.port = trt__SetVideoEncoderConfiguration->Configuration->Multicast->Port;
		pVEC->Multicast_Conf.ttl = trt__SetVideoEncoderConfiguration->Configuration->Multicast->TTL;
	}

	pVEC->sessiontimeout = trt__SetVideoEncoderConfiguration->Configuration->SessionTimeout;

//	trt__SetVideoEncoderConfiguration->Configuration->__size;
//	trt__SetVideoEncoderConfiguration->Configuration->__any;
//	trt__SetVideoEncoderConfiguration->Configuration->__anyAttribute;	

	dvprintf("trt__SetVideoEncoderConfiguration->Configuration->__size %d\r\n",trt__SetVideoEncoderConfiguration->Configuration->__size);
	if (trt__SetVideoEncoderConfiguration->Configuration->__any)
		dvprintf("trt__SetVideoEncoderConfiguration->Configuration->__any %s\r\n", *trt__SetVideoEncoderConfiguration->Configuration->__any);
	dvprintf("trt__SetVideoEncoderConfiguration->Configuration->__anyAttribute %s\r\n", trt__SetVideoEncoderConfiguration->Configuration->__anyAttribute);

	if (pVEC->Vencoder == tt__VideoEncoding__H264) {
		if (idxVEC == IDX_2CH_MAIN_STREAM) {
			gtUser.uviH264[0].eResolution = codec_res;					
			gtUser.uviH264[0].sfFPS = pVEC->frameratelimit;
			gtUser.uviH264[0].nQuality = ONVIF_Quality2EncQ(pVEC->VEquality, pVEC->Vencoder);
			gtUser.uviH264[0].nIDRFrame = pVEC->H264govlength;
			gtUser.uviH264[0].nBitRate = pVEC->bitratelimit;
			gtUser.uviH264[0].eProfileMode = pVEC->H264Profile;

			flprintf("%d/%d/%d/%d/%d/%d\r\n", gtUser.uviH264[0].eResolution, gtUser.uviH264[0].sfFPS,
				gtUser.uviH264[0].nQuality, gtUser.uviH264[0].nIDRFrame, gtUser.uviH264[0].nBitRate,
				gtUser.uviH264[0].eProfileMode);

			while(MsgLock() == DEF_FAIL)																					vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_SIZE, gtUser.uviH264[0].eResolution) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_FPS, gtUser.uviH264[0].sfFPS) == DEF_FAIL)			vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_QUALITY, gtUser.uviH264[0].nQuality) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_IDRFRAME, gtUser.uviH264[0].nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRATE, gtUser.uviH264[0].nBitRate) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PROFILE, gtUser.uviH264[0].eProfileMode) == DEF_FAIL)	vTaskDelay(1);
			MsgUnlock();
			
			dvprintf("Hmain save res%d fps%d qua%d idr%d bit%d\r\n",gtUser.uviH264[0].eResolution,gtUser.uviH264[0].sfFPS,gtUser.uviH264[0].nQuality,gtUser.uviH264[0].nIDRFrame,gtUser.uviH264[0].nBitRate);
		}
#ifdef __H264_2CH__				
		else if(idxVEC == IDX_2CH_SUB_STREAM)
		{
			gtUser.uviH264[1].eResolution = codec_res;
			gtUser.uviH264[1].sfFPS =  pVEC->frameratelimit;
			gtUser.uviH264[1].nQuality = ONVIF_Quality2EncQ(pVEC->VEquality, pVEC->Vencoder);
			gtUser.uviH264[1].nIDRFrame = pVEC->H264govlength;
			gtUser.uviH264[1].nBitRate = pVEC->bitratelimit;					
			gtUser.uviH264[1].eProfileMode = pVEC->H264Profile;

			flprintf("%d/%d/%d/%d/%d/%d\r\n", gtUser.uviH264[1].eResolution, gtUser.uviH264[1].sfFPS,
				gtUser.uviH264[1].nQuality, gtUser.uviH264[1].nIDRFrame, gtUser.uviH264[1].nBitRate,
				gtUser.uviH264[1].eProfileMode);

			while(MsgLock() == DEF_FAIL)																					vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_SIZE, gtUser.uviH264[1].eResolution) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_FPS, gtUser.uviH264[1].sfFPS) == DEF_FAIL)			vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_QUALITY, gtUser.uviH264[1].nQuality) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_IDRFRAME, gtUser.uviH264[1].nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_BRATE, gtUser.uviH264[1].nBitRate) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_PROFILE, gtUser.uviH264[1].eProfileMode) == DEF_FAIL)	vTaskDelay(1);
			MsgUnlock();
			
			dvprintf("Hsub save res%d fps%d qua%d idr%d bit%d\r\n",gtUser.uviH264[1].eResolution,gtUser.uviH264[1].sfFPS,gtUser.uviH264[1].nQuality,gtUser.uviH264[1].nIDRFrame,gtUser.uviH264[1].nBitRate);
		}
#endif				
		else
		{
			gtUser.uviH264[0].eResolution = codec_res;
			gtUser.uviH264[0].sfFPS =  pVEC->frameratelimit;
			gtUser.uviH264[0].nQuality = ONVIF_Quality2EncQ(pVEC->VEquality, pVEC->Vencoder);
			gtUser.uviH264[0].nIDRFrame = pVEC->H264govlength;
			gtUser.uviH264[0].nBitRate = pVEC->bitratelimit;
			gtUser.uviH264[0].eProfileMode = pVEC->H264Profile;

			printf("%s(%d) : ERROR? %d/%d/%d/%d/%d/%d\r\n", __func__, __LINE__, gtUser.uviH264[0].eResolution, gtUser.uviH264[0].sfFPS,
				gtUser.uviH264[0].nQuality, gtUser.uviH264[0].nIDRFrame, gtUser.uviH264[0].nBitRate,
				gtUser.uviH264[0].eProfileMode);

			while(MsgLock() == DEF_FAIL)																					vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_SIZE, gtUser.uviH264[0].eResolution) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_FPS, gtUser.uviH264[0].sfFPS) == DEF_FAIL)			vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_QUALITY, gtUser.uviH264[0].nQuality) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_IDRFRAME, gtUser.uviH264[0].nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRATE, gtUser.uviH264[0].nBitRate) == DEF_FAIL)		vTaskDelay(1);
			while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PROFILE, gtUser.uviH264[0].eProfileMode) == DEF_FAIL)	vTaskDelay(1);
			MsgUnlock();
			
			dvprintf("H264 else save res%d fps%d qua%d idr%d bit%d\r\n",gtUser.uviH264[0].eResolution,gtUser.uviH264[0].sfFPS,gtUser.uviH264[0].nQuality,gtUser.uviH264[0].nIDRFrame,gtUser.uviH264[0].nBitRate);
		}
	}
	else if (pVEC->Vencoder == tt__VideoEncoding__JPEG) {
		gtUser.uviJpeg.eResolution = codec_res;
		gtUser.uviJpeg.sfFPS =  pVEC->frameratelimit;
		gtUser.uviJpeg.nQuality = ONVIF_Quality2EncQ(pVEC->VEquality, pVEC->Vencoder);
		gtUser.uviJpeg.nBitRate = pVEC->bitratelimit;

		while(MsgLock() == DEF_FAIL)																			vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_SIZE, gtUser.uviJpeg.eResolution) == DEF_FAIL) 	vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_QUALITY, gtUser.uviJpeg.nQuality) == DEF_FAIL) 	vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_FPS, gtUser.uviJpeg.sfFPS) == DEF_FAIL)			vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_BRATE, gtUser.uviJpeg.nBitRate) == DEF_FAIL)		vTaskDelay(1);
		MsgUnlock();

		dvprintf("JPEG save res%d fps%d qua%d idr%d bit%d\r\n",gtUser.uviJpeg.eResolution,gtUser.uviJpeg.sfFPS,gtUser.uviJpeg.nQuality,gtUser.uviJpeg.nIDRFrame,gtUser.uviJpeg.nBitRate);
	}						

	UserSave(sfls_USER);
	dvprintf("gtUser SAVE\r\n");

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__SetVideoEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__SetAudioSourceConfiguration */
int __trt__SetAudioSourceConfiguration(struct soap *soap, 
struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration, 
struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxASC;

	for (idxASC = 0; idxASC < oSysInfo->SizeDefaultASC; idxASC++) {
		if (strcmp(trt__SetAudioSourceConfiguration->Configuration->token, oSysInfo->ASC[idxASC].Atoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	flprintf("token(%s) idx(%d)\r\n", trt__SetAudioSourceConfiguration->Configuration->token, idxASC);

	/* if ConfigurationToken does not exist */
	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

// 이 부분이 올바른지는 확인이 필요하다.
// 유효한 SourceToken중에서 변경이 가능한건지? 아니면 다른 SourceToken으로 설정을 할 수 없는건지?
	if (strcmp(oSysInfo->ASC[idxASC].Asourcetoken, trt__SetAudioSourceConfiguration->Configuration->SourceToken) != 0) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(oSysInfo->ASC[idxASC].Aname, trt__SetAudioSourceConfiguration->Configuration->Name);
	//oSysInfo->ASC[idxASC].Acount = trt__SetAudioSourceConfiguration->Configuration->UseCount;
	//strcpy(oSysInfo->ASC[idxASC].Asourcetoken, trt__SetAudioSourceConfiguration->Configuration->SourceToken);
	//trt__SetVideoSourceConfiguration->Configuration->__size;
	//trt__SetVideoSourceConfiguration->Configuration->__any;
	//trt__SetVideoSourceConfiguration->Configuration->__anyAttribute;	

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__SetAudioSourceConfiguration);
#endif
	UNUSED(trt__SetAudioSourceConfigurationResponse);
}


/** Auto-test server operation __trt__SetAudioEncoderConfiguration */
int __trt__SetAudioEncoderConfiguration(struct soap *soap, 
struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration, 
struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxAEC;

	struct tt__AudioEncoderConfiguration *pAEConf = trt__SetAudioEncoderConfiguration->Configuration;

	for (idxAEC = 0; idxAEC < oSysInfo->SizeDefaultAEC; idxAEC++) {
		if (strcmp(pAEConf->token, oSysInfo->AEC[idxAEC].AEtoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {		
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (!(pAEConf->Bitrate >= G711_KBitrate_MIN && pAEConf->Bitrate <= G711_KBitrate_MAX)) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (!(pAEConf->SampleRate >= G711_KHzSampleRate_MIN && pAEConf->SampleRate <= G711_KHzSampleRate_MAX)) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (pAEConf->Encoding != tt__AudioEncoding__G711) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	AEC_t *pAEC = &(oSysInfo->AEC[idxAEC]);

	strcpy(pAEC->AEname, pAEConf->Name);
	pAEC->Abitratelimit = pAEConf->Bitrate;
	pAEC->Aencoder = pAEConf->Encoding;
	pAEC->Asamplerate = pAEConf->SampleRate;
	//pAEConf->UseCount;
	//pAEConf->token;
	//pAEConf->Multicast;
	//pAEConf->SessionTimeout;
	//pAEConf->__size;
	//pAEConf->__any;
	//pAEConf->__anyAttribute;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__SetAudioEncoderConfiguration);
#endif
	UNUSED(trt__SetAudioEncoderConfigurationResponse);
}


/** Auto-test server operation __trt__SetVideoAnalyticsConfiguration */
int __trt__SetVideoAnalyticsConfiguration(struct soap *soap, 
struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration, 
struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_VideoAnalyticsNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(trt__SetVideoAnalyticsConfiguration);
	UNUSED(trt__SetVideoAnalyticsConfigurationResponse);
}


/** Auto-test server operation __trt__SetMetadataConfiguration */
int __trt__SetMetadataConfiguration(struct soap *soap, 
struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration, 
struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__SetMetadataConfiguration);
	UNUSED(trt__SetMetadataConfigurationResponse);
}


/** Auto-test server operation __trt__SetAudioOutputConfiguration */
int __trt__SetAudioOutputConfiguration(struct soap *soap, 
struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration, 
struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();

	struct tt__AudioOutputConfiguration *pAOConf = trt__SetAudioOutputConfiguration->Configuration;
	AOC_t *pAOC = &(oSysInfo->AOC);

	if (strcmp(pAOC->AOtoken, pAOConf->token) != 0) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (strcmp(pAOC->AOutputToken, pAOConf->OutputToken) != 0) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if (!(pAOConf->OutputLevel <= 5 && pAOConf->OutputLevel >= 1)) {
		onvifInterfaceFault(soap, ovfSC_ConfigModify, "");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(pAOC->AOname, pAOConf->Name);
	pAOC->AOutputLevel = pAOConf->OutputLevel;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__SetAudioOutputConfiguration);
#endif
	UNUSED(trt__SetAudioOutputConfigurationResponse);
}


/** Auto-test server operation __trt__SetAudioDecoderConfiguration */
int __trt__SetAudioDecoderConfiguration(struct soap *soap, 
struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration, 
struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo* oSysInfo = GetSysInfo();

	struct tt__AudioDecoderConfiguration *pADConf = trt__SetAudioDecoderConfiguration->Configuration;
	ADC_t *pADC = &(oSysInfo->ADC);

	if (strcmp(pADC->ADtoken, pADConf->token) != 0) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

	strcpy(pADC->ADname, pADConf->Name);

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__SetAudioDecoderConfiguration);
#endif
	UNUSED(trt__SetAudioDecoderConfigurationResponse);
}

/** 
 * @brief Get Video Source Configuration
 * @This operation returns the available options when the video source parameters are reconfigured If a video source configuration is specified,the options shall concern that
 particular configuration.
 *   
 * @param Request	:_trt__GetVideoSourceConfigurationOptions structure contains optional tokens of a video source configuration and a media profile.
 * @param Response	:_trt__GetVideoSourceConfigurationOptionsResponse structure contains the video configuration options.
 * @return               : On success - SOAP_OK.
 On failure - The requested Video Input does not exist
 */  
int __trt__GetVideoSourceConfigurationOptions(struct soap *soap, 
struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions, 
struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxVSC, i;

	if (trt__GetVideoSourceConfigurationOptions->ConfigurationToken) {
		for (idxVSC = 0; idxVSC < oSysInfo->nProfileSize; idxVSC++) {
			if(strcmp(trt__GetVideoSourceConfigurationOptions->ConfigurationToken, oSysInfo->VSC[idxVSC].Vtoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetVideoSourceConfigurationOptions->ProfileToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if(strcmp(trt__GetVideoSourceConfigurationOptions->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}

		idxVSC = MatchProfileVSCToken(oSysInfo->Oprofile[idxPROF].VSCtoken);
	} else {
		idxVSC = 0; // ProfileToken NULL, ConfigurationToken NULL
	}

	trt__GetVideoSourceConfigurationOptionsResponse->Options = (struct tt__VideoSourceConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__VideoSourceConfigurationOptions));
	memset(trt__GetVideoSourceConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__VideoSourceConfigurationOptions));
	struct tt__VideoSourceConfigurationOptions *pVSCOpt = trt__GetVideoSourceConfigurationOptionsResponse->Options;

	pVSCOpt->BoundsRange = (struct tt__IntRectangleRange *)soap_malloc(soap, sizeof(struct tt__IntRectangleRange));
	memset(pVSCOpt->BoundsRange, 0x00,  sizeof(struct tt__IntRectangleRange));

	pVSCOpt->BoundsRange->XRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
	memset(pVSCOpt->BoundsRange->XRange, 0x00, sizeof(struct tt__IntRange));
	pVSCOpt->BoundsRange->XRange->Min = 0;
	pVSCOpt->BoundsRange->XRange->Max = 0;

	pVSCOpt->BoundsRange->YRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
	memset(pVSCOpt->BoundsRange->YRange, 0x00, sizeof(struct tt__IntRange));
	pVSCOpt->BoundsRange->YRange->Min = 0;
	pVSCOpt->BoundsRange->YRange->Max = 0;

	pVSCOpt->BoundsRange->WidthRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
	memset(pVSCOpt->BoundsRange->WidthRange, 0x00, sizeof(struct tt__IntRange));
	pVSCOpt->BoundsRange->WidthRange->Min = WidthRangeMin;
	pVSCOpt->BoundsRange->WidthRange->Max = WidthRangeMax;

	pVSCOpt->BoundsRange->HeightRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
	memset(pVSCOpt->BoundsRange->HeightRange, 0x00, sizeof(struct tt__IntRange));
	pVSCOpt->BoundsRange->HeightRange->Min = HeightRangeMin;
	pVSCOpt->BoundsRange->HeightRange->Max = HeightRangeMax;

	if (trt__GetVideoSourceConfigurationOptions->ProfileToken == NULL 
		&& trt__GetVideoSourceConfigurationOptions->ConfigurationToken == NULL) {
		pVSCOpt->__sizeVideoSourceTokensAvailable = oSysInfo->SizeDefaultVSC;
		pVSCOpt->VideoSourceTokensAvailable = (char **)soap_malloc(soap, sizeof(char *) * pVSCOpt->__sizeVideoSourceTokensAvailable);
		for (i = 0; i < pVSCOpt->__sizeVideoSourceTokensAvailable; i++) {				
			pVSCOpt->VideoSourceTokensAvailable[i] = soap_strdup(soap, oSysInfo->VSC[i].Vsourcetoken);
		}
	} else {
		pVSCOpt->__sizeVideoSourceTokensAvailable = 1;
		pVSCOpt->VideoSourceTokensAvailable = (char **)soap_malloc(soap, sizeof(char *) * pVSCOpt->__sizeVideoSourceTokensAvailable);
		pVSCOpt->VideoSourceTokensAvailable[0] = soap_strdup(soap, oSysInfo->VSC[idxVSC].Vsourcetoken);
	}

	pVSCOpt->Extension = NULL;
	pVSCOpt->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

/**
 * @brief Get Video Encoder COnfiguration Options. 
 * @This operation returns the available options when the video encoder parameters are reconfigured.
 *   
 * @param Request	:_trt__GetVideoEncoderConfigurationOptions structure contains  optional tokens of a video encoder configuration and a media profile.
 * @param Response	:_trt__GetVideoEncoderConfigurationOptionsResponse structure contains the video configuration options.
 * @return               : On success - SOAP_OK.
 On failure - No command specific faults!
 */
int __trt__GetVideoEncoderConfigurationOptions(struct soap *soap, 
struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, 
struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;
	int JPEG_profile = 0;
	int H264_profile = 0;
	int flag, step, substep;

	int idxVEC, nIdx;
	int MainStream = 0;
	int SubStream = 0;

	if (trt__GetVideoEncoderConfigurationOptions->ConfigurationToken) {
		for(i = 0; i < oSysInfo->nProfileSize; i++) {
			if (strcmp(oSysInfo->Oprofile[i].VECtoken, trt__GetVideoEncoderConfigurationOptions->ConfigurationToken) == 0) {
				flag = EXIST;
				idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[i].VECtoken);
				switch (oSysInfo->VEC[idxVEC].Vencoder) {
					case tt__VideoEncoding__JPEG:
						JPEG_profile = 1;
						break;
					case tt__VideoEncoding__H264:
						H264_profile = 1;
						if(idxVEC == IDX_2CH_MAIN_STREAM) {
							MainStream = 1;
						}
#ifdef __H264_2CH__
						else if(idxVEC == IDX_2CH_SUB_STREAM) {
							SubStream = 1;
						}
#endif
						break;
					case tt__VideoEncoding__MPEG4:
					default:
						printf("    %04d:%s: Vencoder Error!(%d)\r\n", __LINE__, __func__, oSysInfo->VEC[idxVEC].Vencoder);
						break;
				}
			}
		}

		if (flag == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetVideoEncoderConfigurationOptions->ProfileToken) {
		for (i = 0; i < oSysInfo->nProfileSize; i++) {
			if (strcmp(oSysInfo->Oprofile[i].profiletoken, trt__GetVideoEncoderConfigurationOptions->ProfileToken) == 0) {
				flag = EXIST;
				idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[i].VECtoken);
				switch (oSysInfo->VEC[idxVEC].Vencoder) {
					case tt__VideoEncoding__JPEG:
						JPEG_profile = 1;
						break;
					case tt__VideoEncoding__H264:
						H264_profile = 1;
						if(idxVEC == IDX_2CH_MAIN_STREAM) {
							MainStream = 1;
						}
#ifdef __H264_2CH__
						else if(idxVEC == IDX_2CH_SUB_STREAM) {
							SubStream = 1;
						}
#endif
						break;
					case tt__VideoEncoding__MPEG4:
					default:
						printf("    %04d:%s: Vencoder Error!(%d)\r\n", __LINE__, __func__, oSysInfo->VEC[idxVEC].Vencoder);
						break;
				}
			}
		}	

		if (flag == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else {
		flag = EXIST;
		JPEG_profile = 1;
		H264_profile = 1;
		MainStream = 1;
#ifdef __H264_2CH__
		SubStream = 1;
#endif
		printf("    %04d:%s: No Tocken...\r\n", __LINE__, __func__);
	}

	trt__GetVideoEncoderConfigurationOptionsResponse->Options = (struct tt__VideoEncoderConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__VideoEncoderConfigurationOptions));
	memset(trt__GetVideoEncoderConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__VideoEncoderConfigurationOptions));
	struct tt__VideoEncoderConfigurationOptions *pVECopt = trt__GetVideoEncoderConfigurationOptionsResponse->Options;

	pVECopt->QualityRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
	memset(pVECopt->QualityRange, 0x00, sizeof(struct tt__IntRange));

	// 화질의 단계가 있어야 한다(아주좋음 좋음 보통 나쁨 아주나쁨)
	// JPEG와 H.264는 이에 맞춰 Q를 설정해준다.
	// ex, 1,2,3,4,5가 있다면 1선택시 JPEG는 0, H.264는 10으로 설정해야 함
#if 1
	pVECopt->QualityRange->Min = ONVIF_Quality_MIN;
	pVECopt->QualityRange->Max = ONVIF_Quality_MAX;
#else
	if (JPEG_profile) {
		pVECopt->QualityRange->Min = JPEG_Quantize_MIN;
		pVECopt->QualityRange->Max = JPEG_Quantize_MAX;
	} else if (H264_profile) {
		pVECopt->QualityRange->Min = H264_Quantize_MIN;
		pVECopt->QualityRange->Max = H264_Quantize_MAX;
	}
#endif
	pVECopt->JPEG = NULL;
	pVECopt->MPEG4 = NULL;
	pVECopt->H264 = NULL;
	pVECopt->__anyAttribute = NULL;

	flprintf("JPEG(%d) H264(%d(%d/%d))\r\n", JPEG_profile, H264_profile, MainStream, SubStream);

	pVECopt->Extension = (struct tt__VideoEncoderOptionsExtension *)soap_malloc(soap, sizeof(struct tt__VideoEncoderOptionsExtension)); 
	memset(pVECopt->Extension, 0x00, sizeof(struct tt__VideoEncoderOptionsExtension));

	if (JPEG_profile) {
		pVECopt->JPEG = (struct tt__JpegOptions *)soap_malloc(soap, sizeof(struct tt__JpegOptions)); 
		memset(pVECopt->JPEG, 0x00, sizeof(struct tt__JpegOptions));

#if (HW_DS0==2) || (HW_DS1==2)
		pVECopt->JPEG->__sizeResolutionsAvailable = ResolutionInfoLength - IMG_JPEG_RES;
#else
		pVECopt->JPEG->__sizeResolutionsAvailable = 1;
#endif
		substep = IMG_JPEG_RES;

		pVECopt->JPEG->ResolutionsAvailable = (struct tt__VideoResolution *)soap_malloc(soap, pVECopt->JPEG->__sizeResolutionsAvailable * sizeof(struct tt__VideoResolution)); 
		memset(pVECopt->JPEG->ResolutionsAvailable, 0x00, pVECopt->JPEG->__sizeResolutionsAvailable * sizeof(struct tt__VideoResolution) );

		flprintf("JPEG Res Count(%d)/r/n", pVECopt->JPEG->__sizeResolutionsAvailable, substep);
		flprintf("JPEG Res Start(%d)/r/n", substep);

		for (step = 0; step < pVECopt->JPEG->__sizeResolutionsAvailable; step++) {
			pVECopt->JPEG->ResolutionsAvailable[step].Width = listResolution[substep].nWidth;
			pVECopt->JPEG->ResolutionsAvailable[step].Height = listResolution[substep].nHeight;
			substep++;
			flprintf("JPEG(%d) Res(%dx%d)/r/n", step, pVECopt->JPEG->ResolutionsAvailable[step].Width, pVECopt->JPEG->ResolutionsAvailable[step].Height);
		}

		pVECopt->JPEG->FrameRateRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange)); 
		memset(pVECopt->JPEG->FrameRateRange, 0x00, sizeof(struct tt__IntRange));
		pVECopt->JPEG->FrameRateRange->Min = 1;
		pVECopt->JPEG->FrameRateRange->Max = gptMsgShare.VIDEO_FPS;

		pVECopt->JPEG->EncodingIntervalRange = (struct tt__IntRange*)soap_malloc(soap, sizeof(struct tt__IntRange));
		memset(pVECopt->JPEG->EncodingIntervalRange, 0x00, sizeof(struct tt__IntRange));
		pVECopt->JPEG->EncodingIntervalRange->Min = 1;
		pVECopt->JPEG->EncodingIntervalRange->Max = gptMsgShare.VIDEO_FPS;
#if 0
		pVECopt->Extension->JPEG = (struct tt__JpegOptiodndl *)soap_malloc(soap, sizeof(struct tt__JpegOptiodndl));
		memset(pVECopt->Extension->JPEG, 0x00, sizeof(struct tt__JpegOptiodndl));
		struct tt__JpegOptiodndl *pJopt = pVECopt->Extension->JPEG;

		pJopt->__sizeResolutionsAvailable = pVECopt->JPEG->__sizeResolutionsAvailable;
		substep = IMG_JPEG_RES;

		pJopt->ResolutionsAvailable = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution) * pJopt->__sizeResolutionsAvailable);
		memset(pVECopt->JPEG->ResolutionsAvailable, 0x00, sizeof(struct tt__VideoResolution) * pJopt->__sizeResolutionsAvailable);

		for (step = 0; step < pJopt->__sizeResolutionsAvailable; step++) {
			pJopt->ResolutionsAvailable[step].Width = listResolution[substep].nWidth;
			pJopt->ResolutionsAvailable[step].Height = listResolution[substep].nHeight;
			substep++;
		}

		pJopt->FrameRateRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
		memset(pJopt->FrameRateRange, 0x00, sizeof(struct tt__IntRange));
		pJopt->FrameRateRange->Min = 1;
		pJopt->FrameRateRange->Max = gptMsgShare.VIDEO_FPS;

		pJopt->EncodingIntervalRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
		memset(pJopt->EncodingIntervalRange, 0x00, sizeof(struct tt__IntRange));
		pJopt->EncodingIntervalRange->Min = 1;
		pJopt->EncodingIntervalRange->Max = gptMsgShare.VIDEO_FPS;

		pJopt->BitrateRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
		memset(pJopt->BitrateRange, 0x00, sizeof(struct tt__IntRange));
		int untest; // warning! Min Max define!!
		pJopt->BitrateRange->Min = 1024 * 1;
		pJopt->BitrateRange->Max = 1024 * 50;

		pJopt->__size = 0;
		pJopt->__any = NULL;
		pJopt->__anyAttribute = NULL;
#endif
	}

	if (H264_profile) {
		pVECopt->H264 = (struct tt__H264Options *)soap_malloc(soap, sizeof(struct tt__H264Options) * (MainStream + SubStream));
		memset(pVECopt->H264, 0x00, sizeof(struct tt__H264Options) * (MainStream + SubStream));

		pVECopt->Extension->H264 = (struct tt__H264Optiodndl *)soap_malloc(soap, sizeof(struct tt__H264Optiodndl) * (MainStream + SubStream));
		memset(pVECopt->Extension->H264, 0x00, sizeof(struct tt__H264Optiodndl) * (MainStream + SubStream));

		for (nIdx = 0; nIdx < (MainStream + SubStream); nIdx++) {
			struct tt__H264Options *pH264Op = &(pVECopt->H264[nIdx]);
			substep = 0;
			if (MainStream == 1 && nIdx == 0) {
#if (HW_DS0==0) || (HW_DS1==0)
				pH264Op->__sizeResolutionsAvailable = ResolutionInfoLength - IMG_H264_RES;
#else
				pH264Op->__sizeResolutionsAvailable = 1;
#endif
				substep = IMG_H264_RES;
			} 
#ifdef __H264_2CH__
			else if(SubStream == 1) {	// subStream 요청은 nIdx가 0 or 1 일 case가 존재함
#if (HW_DS0==1) || (HW_DS1==1)
				pH264Op->__sizeResolutionsAvailable = ResolutionInfoLength - IMG_H264_2CH_RES;
#else
				pH264Op->__sizeResolutionsAvailable = 1;
#endif
				substep = IMG_H264_2CH_RES;
			}
#endif // __H264_2CH__

			pH264Op->ResolutionsAvailable = (struct tt__VideoResolution *)soap_malloc(soap, pH264Op->__sizeResolutionsAvailable * sizeof(struct tt__VideoResolution));
			memset(pH264Op->ResolutionsAvailable, 0x00, pH264Op->__sizeResolutionsAvailable * sizeof(struct tt__VideoResolution));
			for (step = 0; step < pH264Op->__sizeResolutionsAvailable; step++) {
				pH264Op->ResolutionsAvailable[step].Width = listResolution[substep].nWidth;
				pH264Op->ResolutionsAvailable[step].Height = listResolution[substep].nHeight;
				substep++;
			}

			pH264Op->GovLengthRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
			memset(pH264Op->GovLengthRange, 0x00, sizeof(struct tt__IntRange));
			pH264Op->GovLengthRange[0].Min = H264_Idrframe_MIN;
			pH264Op->GovLengthRange[0].Max = H264_Idrframe_MAX;

			pH264Op->FrameRateRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
			memset(pH264Op->FrameRateRange, 0x00, sizeof(struct tt__IntRange));
			pH264Op->FrameRateRange[0].Min = 1;
			pH264Op->FrameRateRange[0].Max = gptMsgShare.VIDEO_FPS;

			pH264Op->EncodingIntervalRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
			memset(pH264Op->EncodingIntervalRange, 0x00, sizeof(struct tt__IntRange));
			pH264Op->EncodingIntervalRange[0].Min = 1;
			pH264Op->EncodingIntervalRange[0].Max = gptMsgShare.VIDEO_FPS;

			pH264Op->__sizeH264ProfilesSupported = 2;
			pH264Op->H264ProfilesSupported = (enum tt__H264Profile *)soap_malloc(soap, pH264Op->__sizeH264ProfilesSupported * sizeof(enum tt__H264Profile));
			memset(pH264Op->H264ProfilesSupported, 0x00, pH264Op->__sizeH264ProfilesSupported * sizeof(enum tt__H264Profile));
			pH264Op->H264ProfilesSupported[0] = tt__H264Profile__Baseline;
			pH264Op->H264ProfilesSupported[1] = tt__H264Profile__Main;

			struct tt__H264Optiodndl *pH264EOp = &(pVECopt->Extension->H264[nIdx]);
			substep = 0;
			if (MainStream == 1 && nIdx == 0) {
				pH264EOp->__sizeResolutionsAvailable = pH264Op->__sizeResolutionsAvailable;
				substep = IMG_H264_RES;
			}
#ifdef __H264_2CH__
			else if(SubStream == 1) {	// subStream 요청은 nIdx가 0 or 1 일 case가 존재함
				pH264EOp->__sizeResolutionsAvailable = pH264Op->__sizeResolutionsAvailable;
				substep = IMG_H264_2CH_RES;
			}
#endif // __H264_2CH__

			pH264EOp->ResolutionsAvailable = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution) * pH264EOp->__sizeResolutionsAvailable);
			memset(pH264EOp->ResolutionsAvailable, 0x00, sizeof(struct tt__VideoResolution) * pH264EOp->__sizeResolutionsAvailable);
			for (step = 0; step < pH264EOp->__sizeResolutionsAvailable; step++) {
				pH264EOp->ResolutionsAvailable[step].Width = listResolution[substep].nWidth;
				pH264EOp->ResolutionsAvailable[step].Height = listResolution[substep].nHeight;
				substep++;
			}

			pH264EOp->GovLengthRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
			memset(pH264EOp->GovLengthRange, 0x00, sizeof(struct tt__IntRange));
			pH264EOp->GovLengthRange[0].Min = H264_Idrframe_MIN;
			pH264EOp->GovLengthRange[0].Max = H264_Idrframe_MAX;
			
			pH264EOp->FrameRateRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
			memset(pH264EOp->FrameRateRange, 0x00, sizeof(struct tt__IntRange));
			pH264EOp->FrameRateRange[0].Min = 1;
			pH264EOp->FrameRateRange[0].Max = gptMsgShare.VIDEO_FPS;
			
			pH264EOp->EncodingIntervalRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange)); 
			memset(pH264EOp->EncodingIntervalRange, 0x00, sizeof(struct tt__IntRange));
			pH264EOp->EncodingIntervalRange[0].Min = 1;
			pH264EOp->EncodingIntervalRange[0].Max = gptMsgShare.VIDEO_FPS;

			pH264EOp->__sizeH264ProfilesSupported = 2;
			pH264EOp->H264ProfilesSupported = (enum tt__H264Profile *)soap_malloc(soap, sizeof(enum tt__H264Profile) * pH264EOp->__sizeH264ProfilesSupported);
			memset(pH264EOp->H264ProfilesSupported, 0x00, sizeof(enum tt__H264Profile) * pH264EOp->__sizeH264ProfilesSupported);
			pH264EOp->H264ProfilesSupported[0] = tt__H264Profile__Baseline;
			pH264EOp->H264ProfilesSupported[1] = tt__H264Profile__Main;

			pH264EOp->BitrateRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
			memset(pH264EOp->BitrateRange, 0x00, sizeof(struct tt__IntRange));
			int untest; // warning! Min Max define!!
			pH264EOp->BitrateRange->Min = 128 * 1;
			pH264EOp->BitrateRange->Max = 1024 * 20;

			pH264EOp->__size = 0;
			pH264EOp->__any = NULL;
			pH264EOp->__anyAttribute = NULL;
		}
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __trt__GetAudioSourceConfigurationOptions */
int __trt__GetAudioSourceConfigurationOptions(struct soap *soap, 
struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions, 
struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxASC, i;

	if (trt__GetAudioSourceConfigurationOptions->ConfigurationToken) {
		for (idxASC = 0; idxASC < oSysInfo->nProfileSize; idxASC++) {
			if(strcmp(trt__GetAudioSourceConfigurationOptions->ConfigurationToken, oSysInfo->ASC[idxASC].Atoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetAudioSourceConfigurationOptions->ProfileToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if(strcmp(trt__GetAudioSourceConfigurationOptions->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}

		idxASC = MatchProfileASCToken(oSysInfo->Oprofile[idxPROF].ASCtoken);
	} else {
		idxASC = 0; // ProfileToken NULL, ConfigurationToken NULL
	}

	trt__GetAudioSourceConfigurationOptionsResponse->Options = (struct tt__AudioSourceConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__AudioSourceConfigurationOptions));
	memset(trt__GetAudioSourceConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__AudioSourceConfigurationOptions));
	struct tt__AudioSourceConfigurationOptions *pASCOpt = trt__GetAudioSourceConfigurationOptionsResponse->Options;

	if (trt__GetAudioSourceConfigurationOptions->ConfigurationToken == NULL
		&& trt__GetAudioSourceConfigurationOptions->ProfileToken == NULL) {
		pASCOpt->__sizeInputTokensAvailable = oSysInfo->SizeDefaultASC;
		pASCOpt->InputTokensAvailable = (char **)soap_malloc(soap, sizeof(char *) * pASCOpt->__sizeInputTokensAvailable);
		for (i = 0; i < pASCOpt->__sizeInputTokensAvailable; i++) {				
			pASCOpt->InputTokensAvailable[i] = soap_strdup(soap, oSysInfo->ASC[i].Asourcetoken);
		}
	} else {
		pASCOpt->__sizeInputTokensAvailable = 1;
		pASCOpt->InputTokensAvailable = (char **)soap_malloc(soap, sizeof(char *) * pASCOpt->__sizeInputTokensAvailable);
		pASCOpt->InputTokensAvailable[0] = soap_strdup(soap, oSysInfo->ASC[idxASC].Asourcetoken);
	}

	pASCOpt->Extension = NULL;
	pASCOpt->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioSourceConfigurationOptions);
	UNUSED(trt__GetAudioSourceConfigurationOptionsResponse);
#endif
}


/** Auto-test server operation __trt__GetAudioEncoderConfigurationOptions */
int __trt__GetAudioEncoderConfigurationOptions(struct soap *soap, 
struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions, 
struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, idxAEC;

	if (trt__GetAudioEncoderConfigurationOptions->ConfigurationToken) {
		for (idxAEC = 0; idxAEC < oSysInfo->nProfileSize; idxAEC++) {
			if(strcmp(trt__GetAudioEncoderConfigurationOptions->ConfigurationToken, oSysInfo->AEC[idxAEC].AEtoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetAudioEncoderConfigurationOptions->ProfileToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if(strcmp(trt__GetAudioEncoderConfigurationOptions->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}

		idxAEC = MatchProfileAECToken(oSysInfo->Oprofile[idxPROF].AECtoken);
	} else {
		idxAEC = 0; // ProfileToken NULL, ConfigurationToken NULL
	}

	AEC_t *pAEC = &(oSysInfo->AEC[idxAEC]);

	trt__GetAudioEncoderConfigurationOptionsResponse->Options = (struct tt__AudioEncoderConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfigurationOptions));
	memset(trt__GetAudioEncoderConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__AudioEncoderConfigurationOptions));
	struct tt__AudioEncoderConfigurationOptions *pAECopt = trt__GetAudioEncoderConfigurationOptionsResponse->Options;

	pAECopt->__sizeOptions = 1;

	pAECopt->Options = (struct tt__AudioEncoderConfigurationOption *)soap_malloc(soap, sizeof(struct tt__AudioEncoderConfigurationOption) * pAECopt->__sizeOptions);
	memset(pAECopt->Options, 0x00, sizeof(struct tt__AudioEncoderConfigurationOption) * pAECopt->__sizeOptions);

	pAECopt->Options->Encoding = pAEC->Aencoder;
	pAECopt->Options->BitrateList = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pAECopt->Options->BitrateList->__sizeItems = 1;
	pAECopt->Options->BitrateList->Items = (int *)soap_malloc(soap, sizeof(int) * pAECopt->Options->BitrateList->__sizeItems);
	pAECopt->Options->BitrateList->Items[0] = pAEC->Abitratelimit;
	pAECopt->Options->SampleRateList = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pAECopt->Options->SampleRateList->__sizeItems = 1;
	pAECopt->Options->SampleRateList->Items = (int *)soap_malloc(soap, sizeof(int) * pAECopt->Options->SampleRateList->__sizeItems);
	pAECopt->Options->SampleRateList->Items[0] = pAEC->Asamplerate;
	pAECopt->Options->__size = 0;
	pAECopt->Options->__any = NULL;
	pAECopt->Options->__anyAttribute = NULL;

	pAECopt->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioEncoderConfigurationOptions);
	UNUSED(trt__GetAudioEncoderConfigurationOptionsResponse);
#endif
}


/** Auto-test server operation __trt__GetMetadataConfigurationOptions */
int __trt__GetMetadataConfigurationOptions(struct soap *soap, 
struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions, 
struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetMetadataConfigurationOptions);
	UNUSED(trt__GetMetadataConfigurationOptionsResponse);
}


/** Auto-test server operation __trt__GetAudioOutputConfigurationOptions */
int __trt__GetAudioOutputConfigurationOptions(struct soap *soap, 
struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions, 
struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	if (trt__GetAudioOutputConfigurationOptions->ConfigurationToken) {
		if(strcmp(trt__GetAudioOutputConfigurationOptions->ConfigurationToken, oSysInfo->AOC.AOtoken) != 0) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetAudioOutputConfigurationOptions->ProfileToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if(strcmp(trt__GetAudioOutputConfigurationOptions->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	trt__GetAudioOutputConfigurationOptionsResponse->Options = (struct tt__AudioOutputConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__AudioOutputConfigurationOptions));
	memset(trt__GetAudioOutputConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__AudioOutputConfigurationOptions));
	struct tt__AudioOutputConfigurationOptions *pAOCopt = trt__GetAudioOutputConfigurationOptionsResponse->Options;

	pAOCopt->__sizeOutputTokensAvailable = 1;
	pAOCopt->OutputTokensAvailable = soap_malloc(soap, sizeof(char *) * pAOCopt->__sizeOutputTokensAvailable);
	pAOCopt->OutputTokensAvailable[0] = soap_strdup(soap, oSysInfo->AOC.AOutputToken);

	pAOCopt->__sizeSendPrimacyOptions = 0;
	pAOCopt->SendPrimacyOptions = NULL;

	pAOCopt->OutputLevelRange = (struct tt__IntRange *)soap_malloc(soap, sizeof(struct tt__IntRange));
	memset(pAOCopt->OutputLevelRange, 0x00, sizeof(struct tt__IntRange));
	pAOCopt->OutputLevelRange->Min = 1;
	pAOCopt->OutputLevelRange->Max = 5;

	pAOCopt->__size = 0;
	pAOCopt->__any = NULL;
	pAOCopt->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioOutputNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioOutputConfigurationOptions);
	UNUSED(trt__GetAudioOutputConfigurationOptionsResponse);
#endif
}


/** Auto-test server operation __trt__GetAudioDecoderConfigurationOptions */
int __trt__GetAudioDecoderConfigurationOptions(struct soap *soap, 
struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions, 
struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse)
{
	ENTER();
#ifdef __AUDIO__
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	if (trt__GetAudioDecoderConfigurationOptions->ConfigurationToken) {
		if(strcmp(trt__GetAudioDecoderConfigurationOptions->ConfigurationToken, oSysInfo->ADC.ADtoken) != 0) {
			onvifInterfaceFault(soap, ovfSC_NoConfig, "");
			LEAVE();
			return SOAP_FAULT;
		}
	} else if (trt__GetAudioDecoderConfigurationOptions->ProfileToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if(strcmp(trt__GetAudioDecoderConfigurationOptions->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}

		if (Ptoken_exist == NOT_EXIST) {
			onvifInterfaceFault(soap, ovfSC_NoProfile, "");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	trt__GetAudioDecoderConfigurationOptionsResponse->Options = (struct tt__AudioDecoderConfigurationOptions *)soap_malloc(soap,sizeof(struct tt__AudioDecoderConfigurationOptions));
	memset(trt__GetAudioDecoderConfigurationOptionsResponse->Options, 0x00, sizeof(struct tt__AudioDecoderConfigurationOptions));
	struct tt__AudioDecoderConfigurationOptions *pADCOpt = trt__GetAudioDecoderConfigurationOptionsResponse->Options;

#if 0
	pADCOpt->AACDecOptions = (struct tt__AACDecOptions *)soap_malloc(soap, sizeof(struct tt__AACDecOptions));
	pADCOpt->AACDecOptions->Bitrate = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->AACDecOptions->Bitrate->__sizeItems = 0;
	pADCOpt->AACDecOptions->Bitrate->Items = NULL;
	pADCOpt->AACDecOptions->SampleRateRange = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->AACDecOptions->SampleRateRange->__sizeItems = 0;
	pADCOpt->AACDecOptions->SampleRateRange->Items = NULL;
	pADCOpt->AACDecOptions->__size = 0;
	pADCOpt->AACDecOptions->__any = NULL;
	pADCOpt->AACDecOptions->__anyAttribute = NULL;
#else
	pADCOpt->AACDecOptions = NULL;
#endif

#if 1
	pADCOpt->G711DecOptions = (struct tt__G711DecOptions *)soap_malloc(soap, sizeof(struct tt__G711DecOptions));
	pADCOpt->G711DecOptions->Bitrate = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->G711DecOptions->Bitrate->__sizeItems = 1;
	pADCOpt->G711DecOptions->Bitrate->Items = (int *)soap_malloc(soap, sizeof(int) * pADCOpt->G711DecOptions->Bitrate->__sizeItems);
	pADCOpt->G711DecOptions->Bitrate->Items[0] = G711_KBitrate_MAX;
	pADCOpt->G711DecOptions->SampleRateRange = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->G711DecOptions->SampleRateRange->__sizeItems = 1;
	pADCOpt->G711DecOptions->SampleRateRange->Items = (int *)soap_malloc(soap, sizeof(int) * pADCOpt->G711DecOptions->SampleRateRange->__sizeItems);
	pADCOpt->G711DecOptions->SampleRateRange->Items[0] = G711_KHzSampleRate_MAX;
	pADCOpt->G711DecOptions->__size = 0;
	pADCOpt->G711DecOptions->__any = NULL;
	pADCOpt->G711DecOptions->__anyAttribute = NULL;
#else
	pADCOpt->G711DecOptions = NULL;
#endif

#if 0
	pADCOpt->G726DecOptions = (struct tt__G726DecOptions *)soap_malloc(soap, sizeof(struct tt__G726DecOptions));
	pADCOpt->G726DecOptions->Bitrate = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->G726DecOptions->Bitrate->__sizeItems = 0;
	pADCOpt->G726DecOptions->Bitrate->Items = NULL;
	pADCOpt->G726DecOptions->SampleRateRange = (struct tt__IntList *)soap_malloc(soap, sizeof(struct tt__IntList));
	pADCOpt->G726DecOptions->SampleRateRange->__sizeItems = 0;
	pADCOpt->G726DecOptions->SampleRateRange->Items = NULL;
	pADCOpt->G726DecOptions->__size = 0;
	pADCOpt->G726DecOptions->__any = NULL;
	pADCOpt->G726DecOptions->__anyAttribute = NULL;
#else
	pADCOpt->G726DecOptions = NULL;
#endif

	pADCOpt->Extension = NULL;
	pADCOpt->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
#else
	onvifInterfaceFault(soap, ovfSC_AudioDecodingNotSupported, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetAudioDecoderConfigurationOptions);
	UNUSED(trt__GetAudioDecoderConfigurationOptionsResponse);
#endif
}


/** Auto-test server operation __trt__GetGuaranteedNumberOfVideoEncoderInstances */
int __trt__GetGuaranteedNumberOfVideoEncoderInstances(struct soap *soap, 
struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances, 
struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF, nCnt = 0;

	if (trt__GetGuaranteedNumberOfVideoEncoderInstances->ConfigurationToken) {
		for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
			if (strcmp(trt__GetGuaranteedNumberOfVideoEncoderInstances->ConfigurationToken, oSysInfo->Oprofile[idxPROF].VSCtoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoConfig, "");
		LEAVE();
		return SOAP_FAULT;
	}

#ifdef __H264__
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264 = (int *)soap_malloc(soap, sizeof(int));
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264[0] = 1;
#ifdef __H264_2CH__
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264[0]++;
#endif
	nCnt += trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264[0];
#else
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->H264 = NULL;
#endif
#ifdef __JPEG__
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->JPEG = (int *)soap_malloc(soap, sizeof(int));
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->JPEG[0] = 1;
	nCnt += trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->JPEG[0];
#else
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->JPEG = NULL;
#endif
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->MPEG4 = NULL;
	trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse->TotalNumber = nCnt;

	LEAVE();
	return SOAP_OK;
}


/** 
 * @brief Get Stream Uri
 * @This operation requests a URI that can be used to initiate a live media stream using RTSP as the control protocol.
 *   
 * @param Request	:_trt__GetStreamUri structure variable members contains two parts. StreamType defines if a unicast or multicast media stream is requested. 
 * @param Response	:_trt__GetStreamUriResponse structure variables members contains the stable Uri to be used for requesting the media stream as well as parameters defining                           the lifetime of the Uri.
 * @return               : On success - SOAP_OK.
 On failure - an appropriate fault code(ONVIF Core Spec. ? Ver. 2.1)
 */

/** Auto-test server operation __trt__GetStreamUri */
int __trt__GetStreamUri(struct soap *soap, 
	struct _trt__GetStreamUri *trt__GetStreamUri,
	struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST, isStreamSetup = NOT_EXIST;
	int idxPROF, idxVEC;
	int _port = 0;
	char _IPAddr[SMALL_INFO_LENGTH];

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if(strcmp(trt__GetStreamUri->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "<env:Text>No Such ProfileToken</env:Text>");
		LEAVE();
		return SOAP_FAULT;
	}

	if (trt__GetStreamUri->StreamSetup != NULL) {
		switch (trt__GetStreamUri->StreamSetup->Stream) {
			case tt__StreamType__RTP_Unicast:
				isStreamSetup = EXIST;
				break;
			case tt__StreamType__RTP_Multicast:
				onvifInterfaceFault(soap, ovfSC_InvalidMulticastSettings, "");
				LEAVE();
				return SOAP_FAULT;
			default:
				isStreamSetup = NOT_EXIST;
				break;
		}

		if (trt__GetStreamUri->StreamSetup->Transport && isStreamSetup == EXIST) {
			switch (trt__GetStreamUri->StreamSetup->Transport->Protocol) {
				case tt__TransportProtocol__UDP:
				case tt__TransportProtocol__TCP:
				case tt__TransportProtocol__RTSP:
					_port = RTSP_PORT;
					isStreamSetup = EXIST;
					break;
				case tt__TransportProtocol__HTTP:
#if (RTSPoverHTTP==1)
					_port = HTTP_PORT;
					isStreamSetup = EXIST;
#else
					isStreamSetup = NOT_EXIST;
					flprintf("RTSPoverHTTP not supported!\r\n");
#endif
					break;
				default:
					flprintf("Protocol Type Error(%d)\r\n", trt__GetStreamUri->StreamSetup->Transport->Protocol);
					break;
			}
		} else {
			isStreamSetup = NOT_EXIST;
		}
	}

	if (isStreamSetup == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_InvalidStreamSetup, "");
		LEAVE();
		return SOAP_FAULT;
	}

	idxVEC = MatchProfileVECToken(oSysInfo->Oprofile[idxPROF].VECtoken);
	VEC_t *pVEC = &(oSysInfo->VEC[idxVEC]);

	const char *strStreamURL = "rtsp://%s:%d/%s";
	switch(pVEC->Vencoder)
	{
		case tt__VideoEncoding__JPEG:	// JPEG
			sprintf(_IPAddr, strStreamURL, getIpAddress(0), _port, JPEGURL);
			isStreamSetup = EXIST;
			break;
		case tt__VideoEncoding__H264:	// H.264
#ifdef __H264_2CH__
			if(idxVEC == IDX_2CH_MAIN_STREAM)
				sprintf(_IPAddr, strStreamURL, getIpAddress(0), _port, H264URL);
			else if(idxVEC == IDX_2CH_SUB_STREAM)
				sprintf(_IPAddr, strStreamURL, getIpAddress(0), _port, HSUBURL);
#else
			sprintf(_IPAddr, strStreamURL, getIpAddress(0), _port, H264URL);
#endif
			isStreamSetup = EXIST;
			break;
		case tt__VideoEncoding__MPEG4:	// MPEG4
		default:
			isStreamSetup = NOT_EXIST;
			sprintf(_IPAddr, strStreamURL, getIpAddress(0), _port, "error");
			flprintf("Vencoder Error(%d)!\r\n", pVEC->Vencoder);
			break;
	}

	if (isStreamSetup == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_InvalidStreamSetup, "");
		LEAVE();
		return SOAP_FAULT;
	}

	trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri *)soap_malloc(soap, sizeof(struct tt__MediaUri));
	memset(trt__GetStreamUriResponse->MediaUri, 0x00, sizeof(struct tt__MediaUri));

	trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap, _IPAddr);
	trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
	trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot = xsd__boolean__false_;
	trt__GetStreamUriResponse->MediaUri->Timeout = 72000;
	trt__GetStreamUriResponse->MediaUri->__size = 0;
	trt__GetStreamUriResponse->MediaUri->__any = NULL;
	trt__GetStreamUriResponse->MediaUri->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __trt__StartMulticastStreaming */
int __trt__StartMulticastStreaming(struct soap *soap, 
struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, 
struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__StartMulticastStreaming->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__StartMulticastStreamingResponse);
}


/** Auto-test server operation __trt__StopMulticastStreaming */
int __trt__StopMulticastStreaming(struct soap *soap, 
struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, 
struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__StopMulticastStreaming->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__StopMulticastStreamingResponse);
}


/** Auto-test server operation __trt__SetSynchronizationPoint */
int __trt__SetSynchronizationPoint(struct soap *soap, 
struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, 
struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__SetSynchronizationPoint->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__SetSynchronizationPointResponse);
}


/** Auto-test server operation __trt__GetSnapshotUri */
int __trt__GetSnapshotUri(struct soap *soap, 
struct _trt__GetSnapshotUri *trt__GetSnapshotUri, 
struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxPROF;

	for (idxPROF = 0; idxPROF < oSysInfo->nProfileSize; idxPROF++) {
		if (strcmp(trt__GetSnapshotUri->ProfileToken, oSysInfo->Oprofile[idxPROF].profiletoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		onvifInterfaceFault(soap, ovfSC_NoProfile, "");
		LEAVE();
		return SOAP_FAULT;
	}

	char _IPAddr[INFO_LENGTH];

	sprintf(_IPAddr, "http://%s/jpg/", getIpAddress(HTTP_ONVIF_PORT));

	trt__GetSnapshotUriResponse->MediaUri = (struct tt__MediaUri *)soap_malloc(soap, sizeof(struct tt__MediaUri));
	memset(trt__GetSnapshotUriResponse->MediaUri, 0x00, sizeof(struct tt__MediaUri));

	trt__GetSnapshotUriResponse->MediaUri->Uri = soap_strdup(soap, _IPAddr);
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = xsd__boolean__false_;
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = xsd__boolean__false_;
	trt__GetSnapshotUriResponse->MediaUri->Timeout = 72000;
	trt__GetSnapshotUriResponse->MediaUri->__size = 0;
	trt__GetSnapshotUriResponse->MediaUri->__any = NULL;
	trt__GetSnapshotUriResponse->MediaUri->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetSnapshotUri);
}


/** Auto-test server operation __trt__GetVideoSourceModes */
int __trt__GetVideoSourceModes(struct soap *soap, 
struct _trt__GetVideoSourceModes *trt__GetVideoSourceModes, 
struct _trt__GetVideoSourceModesResponse *trt__GetVideoSourceModesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetVideoSourceModes);
	UNUSED(trt__GetVideoSourceModesResponse);
}


/** Auto-test server operation __trt__SetVideoSourceMode */
int __trt__SetVideoSourceMode(struct soap *soap, 
struct _trt__SetVideoSourceMode *trt__SetVideoSourceMode, 
struct _trt__SetVideoSourceModeResponse *trt__SetVideoSourceModeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__SetVideoSourceMode);
	UNUSED(trt__SetVideoSourceModeResponse);
}


/** Auto-test server operation __trt__GetOSDs */
int __trt__GetOSDs(struct soap *soap, 
struct _trt__GetOSDs *trt__GetOSDs, 
struct _trt__GetOSDsResponse *trt__GetOSDsResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetOSDs);
	UNUSED(trt__GetOSDsResponse);
}


/** Auto-test server operation __trt__GetOSD */
int __trt__GetOSD(struct soap *soap, 
struct _trt__GetOSD *trt__GetOSD, 
struct _trt__GetOSDResponse *trt__GetOSDResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetOSD);
	UNUSED(trt__GetOSDResponse);
}


/** Auto-test server operation __trt__GetOSDOptions */
int __trt__GetOSDOptions(struct soap *soap, 
struct _trt__GetOSDOptions *trt__GetOSDOptions, 
struct _trt__GetOSDOptionsResponse *trt__GetOSDOptionsResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__GetOSDOptions);
	UNUSED(trt__GetOSDOptionsResponse);
}


/** Auto-test server operation __trt__SetOSD */
int __trt__SetOSD(struct soap *soap, 
struct _trt__SetOSD *trt__SetOSD, 
struct _trt__SetOSDResponse *trt__SetOSDResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__SetOSD);
	UNUSED(trt__SetOSDResponse);
}


/** Auto-test server operation __trt__CreateOSD */
int __trt__CreateOSD(struct soap *soap, 
struct _trt__CreateOSD *trt__CreateOSD, 
struct _trt__CreateOSDResponse *trt__CreateOSDResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__CreateOSD);
	UNUSED(trt__CreateOSDResponse);
}


/** Auto-test server operation __trt__DeleteOSD */
int __trt__DeleteOSD(struct soap *soap, 
struct _trt__DeleteOSD *trt__DeleteOSD, 
struct _trt__DeleteOSDResponse *trt__DeleteOSDResponse)
{
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoConfig, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(trt__DeleteOSD);
	UNUSED(trt__DeleteOSDResponse);
}

