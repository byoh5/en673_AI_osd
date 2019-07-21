#include "dev.h"
#include "dev_inline.h"

#include "FreeRTOS.h" 
#include <stdio.h>

#include "onvifSystemInfo.h"
#include "onvifUtil.h"

#include "msg.h"
#include "user.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

int __timg__GetServiceCapabilities(struct soap *soap, 
struct _timg__GetServiceCapabilities *timg__GetServiceCapabilities, 
struct _timg__GetServiceCapabilitiesResponse *timg__GetServiceCapabilitiesResponse)
{
	ENTER();

	timg__GetServiceCapabilitiesResponse->Capabilities = (struct timg__Capabilities *)soap_malloc(soap, sizeof(struct timg__Capabilities));
	memset(timg__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct timg__Capabilities));

	timg__GetServiceCapabilitiesResponse->Capabilities->__size = 0;
	timg__GetServiceCapabilitiesResponse->Capabilities->__any = NULL;
	timg__GetServiceCapabilitiesResponse->Capabilities->ImageStabilization = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	timg__GetServiceCapabilitiesResponse->Capabilities->ImageStabilization[0] = xsd__boolean__false_;
	timg__GetServiceCapabilitiesResponse->Capabilities->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(timg__GetServiceCapabilities);
}

int __timg__GetImagingSettings(struct soap *soap, 
	struct _timg__GetImagingSettings *timg__GetImagingSettings, 
	struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__GetImagingSettings->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__GetImagingSettings->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	timg__GetImagingSettingsResponse->ImagingSettings = (struct tt__ImagingSettings20 *)soap_malloc(soap, sizeof(struct tt__ImagingSettings20));
	memset(timg__GetImagingSettingsResponse->ImagingSettings, 0x00, sizeof(struct tt__ImagingSettings20));
	struct tt__ImagingSettings20 *pIS20 = timg__GetImagingSettingsResponse->ImagingSettings;
	ImagingSettings_t *pVSCIS = &(oSysInfo->VSC[idxVSC].gImagingSettings);

	pIS20->BacklightCompensation = (struct tt__BacklightCompensation20 *)soap_malloc(soap, sizeof(struct tt__BacklightCompensation20));
	switch (MP(MpBackLight)) {
		case MN_BL_OFF:
		case MN_BL_BLC:
		case MN_BL_WDR:
			pIS20->BacklightCompensation->Mode = tt__BacklightCompensationMode__OFF;
			break;
		case MN_BL_HLC:
			pIS20->BacklightCompensation->Mode = tt__BacklightCompensationMode__ON;
			break;
	}
	pVSCIS->BacklightCompensation.Mode = pIS20->BacklightCompensation->Mode;
	pIS20->BacklightCompensation->Level = (float *)soap_malloc(soap, sizeof(float));
	pIS20->BacklightCompensation->Level[0] = pVSCIS->BacklightCompensation.Level = MP(MpHlMaskThrs);

	pIS20->Brightness = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Brightness[0] = pVSCIS->Brightness = MP(MpBrightness);

	pIS20->ColorSaturation = (float *)soap_malloc(soap, sizeof(float));
	pIS20->ColorSaturation[0] = pVSCIS->ColorSaturation = MP(MpSaturation);

	pIS20->Contrast = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Contrast[0] = pVSCIS->Contrast = MP(MpAce);

#if 1
	pIS20->Exposure = (struct tt__Exposure20 *)soap_malloc(soap, sizeof(struct tt__Exposure20));
	pIS20->Exposure->Mode = pVSCIS->Exposure.Mode; // tt__ExposureMode__AUTO, tt__ExposureMode__MANUAL
	pIS20->Exposure->Priority = (enum tt__ExposurePriority *)soap_malloc(soap, sizeof(enum tt__ExposurePriority));
	pIS20->Exposure->Priority[0] = pVSCIS->Exposure.Priority; // tt__ExposurePriority__LowNoise, tt__ExposurePriority__FrameRate
	pIS20->Exposure->Window = (struct tt__Rectangle *)soap_malloc(soap, sizeof(struct tt__Rectangle));
	pIS20->Exposure->Window->bottom = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->bottom[0] = pVSCIS->Exposure.Window.bottom;
	pIS20->Exposure->Window->top = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->top[0] = pVSCIS->Exposure.Window.top;
	pIS20->Exposure->Window->right = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->right[0] = pVSCIS->Exposure.Window.right;
	pIS20->Exposure->Window->left = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->left[0] = pVSCIS->Exposure.Window.left;
	pIS20->Exposure->MinExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinExposureTime[0] = pVSCIS->Exposure.MinExposureTime;
	pIS20->Exposure->MaxExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxExposureTime[0] = pVSCIS->Exposure.MaxExposureTime;
	pIS20->Exposure->MinGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinGain[0] = pVSCIS->Exposure.MinGain;
	pIS20->Exposure->MaxGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxGain[0] = pVSCIS->Exposure.MaxGain;
	pIS20->Exposure->MinIris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinIris[0] = pVSCIS->Exposure.MinIris;
	pIS20->Exposure->MaxIris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxIris[0] = pVSCIS->Exposure.MaxIris;
	pIS20->Exposure->ExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->ExposureTime[0] = pVSCIS->Exposure.ExposureTime;
	pIS20->Exposure->Gain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Gain[0] = pVSCIS->Exposure.Gain;
	pIS20->Exposure->Iris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Iris[0] = pVSCIS->Exposure.Iris;

#if 1
	pIS20->Focus = (struct tt__FocusConfiguration20 *)soap_malloc(soap, sizeof(struct tt__FocusConfiguration20));
#if (model_Lens==1)
	pIS20->Focus->AutoFocusMode = pVSCIS->Focus.AutoFocusMode = MP(MpMAF_mode);
#else
	pIS20->Focus->AutoFocusMode = 0;
#endif
#if 0
	pIS20->Focus->DefaultSpeed = NULL;
	pIS20->Focus->NearLimit = NULL;
	pIS20->Focus->FarLimit = NULL;
#else
	pIS20->Focus->DefaultSpeed = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->DefaultSpeed[0] = pVSCIS->Focus.DefaultSpeed;
	pIS20->Focus->NearLimit = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->NearLimit[0] = pVSCIS->Focus.NearLimit;
	pIS20->Focus->FarLimit = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->FarLimit[0] = pVSCIS->Focus.FarLimit;
#endif
	pIS20->Focus->Extension = NULL;
	pIS20->Focus->__anyAttribute = NULL;
#else
	pIS20->Focus = NULL;
#endif
#else
	pIS20->Exposure = (struct tt__Exposure20 *)soap_malloc(soap, sizeof(struct tt__Exposure20));
	pIS20->Exposure->Mode = 0; // tt__ExposureMode__AUTO, tt__ExposureMode__MANUAL
	pIS20->Exposure->Priority = (enum tt__ExposurePriority *)soap_malloc(soap, sizeof(enum tt__ExposurePriority));
	pIS20->Exposure->Priority[0] = 0; // tt__ExposurePriority__LowNoise, tt__ExposurePriority__FrameRate
	pIS20->Exposure->Window = (struct tt__Rectangle *)soap_malloc(soap, sizeof(struct tt__Rectangle));
	pIS20->Exposure->Window->bottom = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->bottom[0] = 0.0;
	pIS20->Exposure->Window->top = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->top[0] = 0.0;
	pIS20->Exposure->Window->right = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->right[0] = 0.0;
	pIS20->Exposure->Window->left = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Window->left[0] = 0.0;
	pIS20->Exposure->MinExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinExposureTime[0] = 0.0;
	pIS20->Exposure->MaxExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxExposureTime[0] = 0.0;
	pIS20->Exposure->MinGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinGain[0] = 0.0;
	pIS20->Exposure->MaxGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxGain[0] = 0.0;
	pIS20->Exposure->MinIris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MinIris[0] = 0.0;
	pIS20->Exposure->MaxIris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->MaxIris[0] = 0.0;
	pIS20->Exposure->ExposureTime = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->ExposureTime[0] = 0.0;
	pIS20->Exposure->Gain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Gain[0] = 0.0;
	pIS20->Exposure->Iris = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Exposure->Iris[0] = 0.0;

	pIS20->Focus = (struct tt__FocusConfiguration20 *)soap_malloc(soap, sizeof(struct tt__FocusConfiguration20));
	pIS20->Focus->AutoFocusMode = 0; // tt__AutoFocusMode__AUTO, tt__AutoFocusMode__MANUAL
	pIS20->Focus->DefaultSpeed = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->DefaultSpeed[0] = 0.0;
	pIS20->Focus->NearLimit = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->NearLimit[0] = 0.0;
	pIS20->Focus->FarLimit = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Focus->FarLimit[0] = 0.0;
	pIS20->Focus->Extension = NULL;
	pIS20->Focus->__anyAttribute = NULL;
#endif

	pIS20->IrCutFilter = (enum tt__IrCutFilterMode *)soap_malloc(soap, sizeof(enum tt__IrCutFilterMode));
	switch (MP(MpTdn)) {
		case MN_TDN_AUTO:
			pIS20->IrCutFilter[0] = tt__IrCutFilterMode__AUTO;
			break;
		case MN_TDN_DAY:
		case MN_TDN_EXTERN:
			pIS20->IrCutFilter[0] = tt__IrCutFilterMode__ON;
			break;
		case MN_TDN_NIGHT:
			pIS20->IrCutFilter[0] = tt__IrCutFilterMode__OFF;
			break;
	}
	pVSCIS->IrCutFilter = pIS20->IrCutFilter[0];

	pIS20->Sharpness = (float *)soap_malloc(soap, sizeof(float));
	pIS20->Sharpness[0] = pVSCIS->Sharpness = MP(MpSharpness);

	pIS20->WideDynamicRange = (struct tt__WideDynamicRange20 *)soap_malloc(soap, sizeof(struct tt__WideDynamicRange20));
#if model_2M30p
	switch (MP(FrameMode)) {
		case MN_FRAMEMODE_NORMAL_25_30:
		case MN_FRAMEMODE_DNR_25_30:
			pIS20->WideDynamicRange->Mode = tt__WideDynamicMode__OFF;
			break;
		case MN_FRAMEMODE_WDR_12_15:
			pIS20->WideDynamicRange->Mode = tt__WideDynamicMode__ON;
			break;
	}
#else
	switch (MP(FrameMode)) {
		case MN_FRAMEMODE_NORMAL_25_30:
		case MN_FRAMEMODE_NORMAL_50_60:
		case MN_FRAMEMODE_DNR_25_30:
		case MN_FRAMEMODE_DNR_50_60:
			pIS20->WideDynamicRange->Mode = tt__WideDynamicMode__OFF;
			break;
		case MN_FRAMEMODE_WDR_12_15:
		case MN_FRAMEMODE_WDR_25_30:
			pIS20->WideDynamicRange->Mode = tt__WideDynamicMode__ON;
			break;
	}
#endif
	pVSCIS->WideDynamicRange.Mode = pIS20->WideDynamicRange->Mode;
	pIS20->WideDynamicRange->Level = (float *)soap_malloc(soap, sizeof(float));
	pIS20->WideDynamicRange->Level[0] = pVSCIS->WideDynamicRange.Level = MP(MpFrameRateLv); // MP(MpWdrWgt);

	pIS20->WhiteBalance = (struct tt__WhiteBalance20 *)soap_malloc(soap, sizeof(struct tt__WhiteBalance20));
	switch (MP(MpAwb)) {
		case MN_AWB_AUTO:
		case MN_AWB_AUTOEXT:
		case MN_AWB_PRESET:
			pIS20->WhiteBalance->Mode = tt__WhiteBalanceMode__AUTO;
			break;
		case MN_AWB_MNL:
			pIS20->WhiteBalance->Mode = tt__WhiteBalanceMode__MANUAL;
			break;
	}
	pVSCIS->WhiteBalance.Mode = pIS20->WhiteBalance->Mode;
	pIS20->WhiteBalance->CrGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->WhiteBalance->CrGain[0] = pVSCIS->WhiteBalance.CrGain = MP(MpRgain);
	pIS20->WhiteBalance->CbGain = (float *)soap_malloc(soap, sizeof(float));
	pIS20->WhiteBalance->CbGain[0] = pVSCIS->WhiteBalance.CbGain = MP(MpBgain);
	pIS20->WhiteBalance->Extension = NULL;
	pIS20->WhiteBalance->__anyAttribute = NULL;

	pIS20->Extension = NULL;
	pIS20->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

int __timg__SetImagingSettings(struct soap *soap, 
struct _timg__SetImagingSettings *timg__SetImagingSettings, 
struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__SetImagingSettings->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__SetImagingSettings->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(timg__SetImagingSettings->ImagingSettings == NULL) {
		flprintf("Error, SettingsInvalid(%s)\r\n", timg__SetImagingSettings->ImagingSettings);
		onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
		LEAVE();
		return SOAP_FAULT;
	}

	struct tt__ImagingSettings20 *pIS20 = timg__SetImagingSettings->ImagingSettings;
	ImagingSettings_t *pVSCIS = &(oSysInfo->VSC[idxVSC].gImagingSettings);

	if (pIS20->BacklightCompensation) {
		if (pVSCIS->BacklightCompensation.Mode != pIS20->BacklightCompensation->Mode) {
			switch (pIS20->BacklightCompensation->Mode) {
				case tt__BacklightCompensationMode__OFF:
					MP(MpBackLight) = MN_BL_OFF;
					break;
				case tt__BacklightCompensationMode__ON:
					MP(MpBackLight) = MN_BL_HLC;
					break;
				default:
					flprintf("Error, Backlight Mode(%d)\r\n", pIS20->BacklightCompensation->Mode);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
			}
			pVSCIS->BacklightCompensation.Mode = pIS20->BacklightCompensation->Mode;
		}

		if (pIS20->BacklightCompensation->Level) {
			if(pIS20->BacklightCompensation->Level[0] < 0.0 || pIS20->BacklightCompensation->Level[0] > 20.0) {
				flprintf("Error, Backlight Level(%.1f)(0~20)\r\n", pIS20->BacklightCompensation->Level[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}

			MP(MpHlMaskThrs) = pVSCIS->BacklightCompensation.Level = pIS20->BacklightCompensation->Level[0];
		}
	}

	if (pIS20->Brightness) {
		if(pIS20->Brightness[0] < 0.0 || pIS20->Brightness[0] > 20.0) {
			flprintf("Error, Brightness(%.1f)(0~20)\r\n", pIS20->Brightness[0]);
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}

		MP(MpBrightness) = pVSCIS->Brightness = pIS20->Brightness[0];
	}

	if (pIS20->ColorSaturation) {
		if(pIS20->ColorSaturation[0] < 0.0 || pIS20->ColorSaturation[0] > 40.0) {
			flprintf("Error, ColorSaturation(%.1f)(0~40)\r\n", pIS20->ColorSaturation[0]);
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}

		MP(MpSaturation) = pVSCIS->ColorSaturation = pIS20->ColorSaturation[0];
		MP(MpSaturationR) = MP(MpSaturationB) = MP(MpSaturation);
	}

	if (pIS20->Contrast) {
		if (pIS20->Contrast[0] < 0.0 || pIS20->Contrast[0] > 3.0) {
			flprintf("Error, Contrast(%.1f)(0~3)\r\n", pIS20->Contrast[0]);
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}

		MP(MpAce) = pVSCIS->Contrast = pIS20->Contrast[0];	
	}

	if (pIS20->Exposure) {
		switch (pIS20->Exposure->Mode) {
			case tt__ExposureMode__AUTO:
				// MP(?)
				break;
			case tt__ExposureMode__MANUAL:
				// MP(?)
				break;
			default:
				flprintf("Error, Exposure Mode(%d)\r\n", pIS20->Exposure->Mode);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
		}
		pVSCIS->Exposure.Mode = pIS20->Exposure->Mode;	

		if (pIS20->Exposure->Priority) {
			switch (pIS20->Exposure->Priority[0]) {
				case tt__ExposurePriority__LowNoise:
					// MP(?)
					break;
				case tt__ExposurePriority__FrameRate:
					// MP(?)
					break;
				default:
					flprintf("Error, Exposure Priority(%d)\r\n", pIS20->Exposure->Priority);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
			}
			pVSCIS->Exposure.Priority = pIS20->Exposure->Priority[0];
		}

		if (pIS20->Exposure->Window) {
			if (pIS20->Exposure->Window->bottom) {
				if (pIS20->Exposure->Window->bottom[0] < 0.0 || pIS20->Exposure->Window->bottom[0] > 20.0) {
					flprintf("Error, Exposure bottom(%.1f)(0~20)\r\n", pIS20->Exposure->Window->bottom[0]);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
				}
				pVSCIS->Exposure.Window.bottom = pIS20->Exposure->Window->bottom[0];
			}

			if (pIS20->Exposure->Window->top) {
				if (pIS20->Exposure->Window->top[0] < 0.0 || pIS20->Exposure->Window->top[0] > 20.0) {
					flprintf("Error, Exposure top(%.1f)(0~20)\r\n", pIS20->Exposure->Window->top[0]);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
				}
				pVSCIS->Exposure.Window.top = pIS20->Exposure->Window->top[0];
			}

			if (pIS20->Exposure->Window->left) {
				if (pIS20->Exposure->Window->left[0] < 0.0 || pIS20->Exposure->Window->left[0] > 20.0) {
					flprintf("Error, Exposure left(%.1f)(0~20)\r\n", pIS20->Exposure->Window->left[0]);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
				}
				pVSCIS->Exposure.Window.left = pIS20->Exposure->Window->left[0];
			}

			if (pIS20->Exposure->Window->right) {
				if (pIS20->Exposure->Window->right[0] < 0.0 || pIS20->Exposure->Window->right[0] > 20.0) {
					flprintf("Error, Exposure right(%.1f)(0~20)\r\n", pIS20->Exposure->Window->right[0]);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
				}
				pVSCIS->Exposure.Window.right = pIS20->Exposure->Window->right[0];
			}
		}

		if (pIS20->Exposure->MinExposureTime) {
			if (pIS20->Exposure->MinExposureTime[0] < 0.0 || pIS20->Exposure->MinExposureTime[0] > 20.0) {
				flprintf("Error, Exposure MinExposureTime(%.1f)(0~20)\r\n", pIS20->Exposure->MinExposureTime[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MinExposureTime = pIS20->Exposure->MinExposureTime[0];
		}

		if (pIS20->Exposure->MaxExposureTime) {
			if (pIS20->Exposure->MaxExposureTime[0] < 0.0 || pIS20->Exposure->MaxExposureTime[0] > 100.0) {
				flprintf("Error, Exposure MaxExposureTime(%.1f)(0~100)\r\n", pIS20->Exposure->MaxExposureTime[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MaxExposureTime = pIS20->Exposure->MaxExposureTime[0];
		}

		if (pIS20->Exposure->MinGain) {
			if (pIS20->Exposure->MinGain[0] < 0.0 || pIS20->Exposure->MinGain[0] > 20.0) {
				flprintf("Error, Exposure MinGain(%.1f)(0~20)\r\n", pIS20->Exposure->MinGain[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MinGain = pIS20->Exposure->MinGain[0];
		}

		if (pIS20->Exposure->MaxGain) {
			if (pIS20->Exposure->MaxGain[0] < 0.0 || pIS20->Exposure->MaxGain[0] > 20.0) {
				flprintf("Error, Exposure MaxGain(%.1f)(0~20)\r\n", pIS20->Exposure->MaxGain[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MaxGain = pIS20->Exposure->MaxGain[0];
		}

		if (pIS20->Exposure->MinIris) {
			if (pIS20->Exposure->MinIris[0] < 0.0 || pIS20->Exposure->MinIris[0] > 20.0) {
				flprintf("Error, Exposure MinIris(%.1f)(0~20)\r\n", pIS20->Exposure->MinIris[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MinIris = pIS20->Exposure->MinIris[0];
		}

		if (pIS20->Exposure->MaxIris) {
			if (pIS20->Exposure->MaxIris[0] < 0.0 || pIS20->Exposure->MaxIris[0] > 20.0) {
				flprintf("Error, Exposure MaxIris(%.1f)(0~20)\r\n", pIS20->Exposure->MaxIris[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.MaxIris = pIS20->Exposure->MaxIris[0];
		}

		if (pIS20->Exposure->ExposureTime) {
			if (pIS20->Exposure->ExposureTime[0] < 0.0 || pIS20->Exposure->ExposureTime[0] > 20.0) {
				flprintf("Error, Exposure ExposureTime(%.1f)(0~20)\r\n", pIS20->Exposure->ExposureTime[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.ExposureTime = pIS20->Exposure->ExposureTime[0];
		}

		if (pIS20->Exposure->Gain) {
			if (pIS20->Exposure->Gain[0] < 0.0 || pIS20->Exposure->Gain[0] > 20.0) {
				flprintf("Error, Exposure Gain(%.1f)(0~20)\r\n", pIS20->Exposure->Gain[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.Gain = pIS20->Exposure->Gain[0];
		}

		if (pIS20->Exposure->Iris) {
			if (pIS20->Exposure->Iris[0] < 0.0 || pIS20->Exposure->Iris[0] > 20.0) {
				flprintf("Error, Exposure Iris(%.1f)(0~20)\r\n", pIS20->Exposure->Iris[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Exposure.Iris = pIS20->Exposure->Iris[0];
		}
	}

	if (pIS20->Focus) {
		switch (pIS20->Focus->AutoFocusMode) {
			case tt__AutoFocusMode__AUTO:
#if (model_Lens==1)
				MP(MpMAF_mode)=0;
#endif
				break;
			case tt__AutoFocusMode__MANUAL:
#if (model_Lens==1)
				MP(MpMAF_mode)=1;
#endif
				break;
			default:
				flprintf("Error, Focus Mode(%d)\r\n", pIS20->Focus->AutoFocusMode);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
		}
		pVSCIS->Focus.AutoFocusMode = pIS20->Focus->AutoFocusMode; 

		if (pIS20->Focus->DefaultSpeed) {
			if (pIS20->Focus->DefaultSpeed[0] < oSysInfo->PTZC.ZoomLimit_XRange_Min || pIS20->Focus->DefaultSpeed[0] > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
				flprintf("Error, Focus DefaultSpeed(%.1f)(%f~%f)\r\n", pIS20->Focus->DefaultSpeed[0], oSysInfo->PTZC.ZoomLimit_XRange_Min, oSysInfo->PTZC.ZoomLimit_XRange_Max);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Focus.DefaultSpeed = pIS20->Focus->DefaultSpeed[0];
		}

		if (pIS20->Focus->NearLimit) {
			if (pIS20->Focus->NearLimit[0] < oSysInfo->PTZC.ZoomLimit_XRange_Min || pIS20->Focus->NearLimit[0] > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
				flprintf("Error, Focus NearLimit(%.1f)(%f~%f)\r\n", pIS20->Focus->NearLimit[0], oSysInfo->PTZC.ZoomLimit_XRange_Min, oSysInfo->PTZC.ZoomLimit_XRange_Max);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Focus.NearLimit = pIS20->Focus->NearLimit[0];
		}

		if (pIS20->Focus->FarLimit) {
			if (pIS20->Focus->FarLimit[0] < oSysInfo->PTZC.ZoomLimit_XRange_Min || pIS20->Focus->FarLimit[0] > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
				flprintf("Error, Focus FarLimit(%.1f)(%f~%f)\r\n", pIS20->Focus->FarLimit[0], oSysInfo->PTZC.ZoomLimit_XRange_Min, oSysInfo->PTZC.ZoomLimit_XRange_Max);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			pVSCIS->Focus.FarLimit = pIS20->Focus->FarLimit[0];
		}
	}

	if (pIS20->IrCutFilter) {
		switch (pIS20->IrCutFilter[0]) {
			case tt__IrCutFilterMode__ON:
				MP(MpTdn) = MN_TDN_DAY;
				break;
			case tt__IrCutFilterMode__OFF:
				MP(MpTdn) = MN_TDN_NIGHT;
				break;
			case tt__IrCutFilterMode__AUTO:
				MP(MpTdn) = MN_TDN_AUTO;
				break;
			default:
				flprintf("Error, IrCutFilter(%d)(0~2)\r\n", pIS20->IrCutFilter[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
		}
		pVSCIS->IrCutFilter = pIS20->IrCutFilter[0];
	}

	if (pIS20->Sharpness) {
		if(pIS20->Sharpness[0] < 0.0 || pIS20->Sharpness[0] > 10.0) {
			flprintf("Error, Sharpness(%.1f)(0~10)\r\n", pIS20->Sharpness[0]);
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}

		MP(MpSharpness) = pVSCIS->Sharpness = pIS20->Sharpness[0];
	}

	if (pIS20->WideDynamicRange) {
		if (pVSCIS->WideDynamicRange.Mode != pIS20->WideDynamicRange->Mode) {
			switch (pIS20->WideDynamicRange->Mode) {
				case tt__WideDynamicMode__OFF:
#if model_2M30p
					if (MP(FrameMode) == MN_FRAMEMODE_WDR_12_15) {
						MP(FrameMode) = MN_FRAMEMODE_NORMAL_25_30;
					}
#else
					if (MP(FrameMode) == MN_FRAMEMODE_WDR_12_15) {
						MP(FrameMode) = MN_FRAMEMODE_NORMAL_25_30;
					} else if (MP(FrameMode) == MN_FRAMEMODE_WDR_25_30) {
						MP(FrameMode) = MN_FRAMEMODE_NORMAL_50_60;
					}
#endif					
					break;
				case tt__WideDynamicMode__ON:
#if model_2M30p
						MP(FrameMode) = MN_FRAMEMODE_WDR_12_15;
#else
						MP(FrameMode) = MN_FRAMEMODE_WDR_25_30;
#endif
					break;
				default:
					flprintf("Error, WDR Mode(%d)(0~1)\r\n", pIS20->WideDynamicRange->Mode);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
			}
			pVSCIS->WideDynamicRange.Mode = pIS20->WideDynamicRange->Mode;
		}

		if(pIS20->WideDynamicRange->Level[0] < 0.0 || pIS20->WideDynamicRange->Level[0] > 2.0) {
			flprintf("Error, WDR Level(%.1f)(0~2)\r\n", pIS20->WideDynamicRange->Level[0]);
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}
 
		MP(MpFrameRateLv) = pVSCIS->WideDynamicRange.Level = pIS20->WideDynamicRange->Level[0];
	}

	if (pIS20->WhiteBalance) {
		if (pVSCIS->WhiteBalance.Mode != pIS20->WhiteBalance->Mode) {
			switch (pIS20->WhiteBalance->Mode) {
				case tt__WhiteBalanceMode__AUTO:
					MP(MpAwb) = MN_AWB_AUTO;
					break;
				case tt__WhiteBalanceMode__MANUAL:
					MP(MpAwb) = MN_AWB_MNL;
					break;
				default:
					flprintf("Error, WhiteBalance Mode(%d)(0~1)\r\n", pIS20->WhiteBalance->Mode);
					onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
					LEAVE();
					return SOAP_FAULT;
			}
			pVSCIS->WhiteBalance.Mode = pIS20->WhiteBalance->Mode;
		}

		if (pIS20->WhiteBalance->CbGain) {
			if(pIS20->WhiteBalance->CbGain[0] < 0.0 || pIS20->WhiteBalance->CbGain[0] > 20.0) {
				flprintf("Error, WhiteBalance CbGain(%.1f)(0~20)\r\n", pIS20->WhiteBalance->CbGain[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}

			MP(MpBgain) = pVSCIS->WhiteBalance.CbGain = pIS20->WhiteBalance->CbGain[0];
		}

		if (pIS20->WhiteBalance->CrGain) {
			if(pIS20->WhiteBalance->CrGain[0] < 0.0 || pIS20->WhiteBalance->CrGain[0] > 20.0) {
				flprintf("Error, WhiteBalance CrGain(%.1f)(0~20)\r\n", pIS20->WhiteBalance->CrGain[0]);
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}

			MP(MpRgain) = pVSCIS->WhiteBalance.CrGain = pIS20->WhiteBalance->CrGain[0];
		}
	}

	if (pIS20->Extension) {

	}

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_SAVE, 0) == DEF_FAIL)
		vTaskDelay(1);

	LEAVE();
	return SOAP_OK;

	UNUSED(timg__SetImagingSettingsResponse);
}


int __timg__GetOptions(struct soap *soap, 
struct _timg__GetOptions *timg__GetOptions, 
struct _timg__GetOptionsResponse *timg__GetOptionsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__GetOptions->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__GetOptions->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	timg__GetOptionsResponse->ImagingOptions = (struct tt__ImagingOptions20 *)soap_malloc(soap, sizeof(struct tt__ImagingOptions20));
	memset(timg__GetOptionsResponse->ImagingOptions, 0x00, sizeof(struct tt__ImagingOptions20));
	struct tt__ImagingOptions20 *pIO20 = timg__GetOptionsResponse->ImagingOptions;
	
	pIO20->BacklightCompensation = (struct tt__BacklightCompensationOptions20 *)soap_malloc(soap, sizeof(struct tt__BacklightCompensationOptions20));
	memset(pIO20->BacklightCompensation, 0x00, sizeof(struct tt__BacklightCompensationOptions20));

	pIO20->BacklightCompensation->__sizeMode = 2;
	pIO20->BacklightCompensation->Mode = (enum tt__BacklightCompensationMode *)soap_malloc(soap, sizeof(enum tt__BacklightCompensationMode) * pIO20->BacklightCompensation->__sizeMode);
	pIO20->BacklightCompensation->Mode[0] = tt__BacklightCompensationMode__OFF;
	pIO20->BacklightCompensation->Mode[1] = tt__BacklightCompensationMode__ON;
	pIO20->BacklightCompensation->Level = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->BacklightCompensation->Level->Min = 0.0;
	pIO20->BacklightCompensation->Level->Max = 20.0;

	pIO20->Brightness = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Brightness->Min = 0.0;
	pIO20->Brightness->Max = 20.0;
	
	pIO20->ColorSaturation = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->ColorSaturation->Min = 0.0;
	pIO20->ColorSaturation->Max = 40.0;

	pIO20->Contrast = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Contrast->Min = 0.0;
	pIO20->Contrast->Max = 3.0;

	pIO20->Exposure = (struct tt__ExposureOptions20 *)soap_malloc(soap, sizeof(struct tt__ExposureOptions20));
	memset(pIO20->Exposure, 0x00, sizeof(struct tt__ExposureOptions20));
	pIO20->Exposure->__sizeMode = 2;
	pIO20->Exposure->Mode = (enum tt__ExposureMode *)soap_malloc(soap, sizeof(enum tt__ExposureMode) * pIO20->Exposure->__sizeMode);
	pIO20->Exposure->Mode[0] = tt__ExposureMode__AUTO;
	pIO20->Exposure->Mode[1] = tt__ExposureMode__MANUAL;
	pIO20->Exposure->__sizePriority = 2;
	pIO20->Exposure->Priority = (enum tt__ExposurePriority *)soap_malloc(soap, sizeof(enum tt__ExposurePriority) * pIO20->Exposure->__sizePriority);
	pIO20->Exposure->Priority[0] = tt__ExposurePriority__LowNoise;
	pIO20->Exposure->Priority[1] = tt__ExposurePriority__FrameRate;
	pIO20->Exposure->MinExposureTime = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MinExposureTime->Min = 0.0;
	pIO20->Exposure->MinExposureTime->Max = 20.0;
	pIO20->Exposure->MaxExposureTime = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MaxExposureTime->Min = 0.0;
	pIO20->Exposure->MaxExposureTime->Max = 100.0;
	pIO20->Exposure->MinGain = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MinGain->Min = 0.0;
	pIO20->Exposure->MinGain->Max = 20.0;
	pIO20->Exposure->MaxGain = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MaxGain->Min = 0.0;
	pIO20->Exposure->MaxGain->Max = 20.0;
	pIO20->Exposure->MinIris = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MinIris->Min = 0.0;
	pIO20->Exposure->MinIris->Max = 20.0;
	pIO20->Exposure->MaxIris = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->MaxIris->Min = 0.0;
	pIO20->Exposure->MaxIris->Max = 20.0;
	pIO20->Exposure->ExposureTime = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->ExposureTime->Min = 0.0;
	pIO20->Exposure->ExposureTime->Max = 20.0;
	pIO20->Exposure->Gain = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->Gain->Min = 0.0;
	pIO20->Exposure->Gain->Max = 20.0;
	pIO20->Exposure->Iris = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Exposure->Iris->Min = 0.0;
	pIO20->Exposure->Iris->Max = 20.0;

#if 1
	pIO20->Focus = (struct tt__FocusOptions20 *)soap_malloc(soap,sizeof(struct tt__FocusOptions20));
	pIO20->Focus->__sizeAutoFocusModes = 2;
	pIO20->Focus->AutoFocusModes = (enum tt__AutoFocusMode *)soap_malloc(soap, sizeof(enum tt__AutoFocusMode) * pIO20->Focus->__sizeAutoFocusModes);
	pIO20->Focus->AutoFocusModes[0] = tt__AutoFocusMode__AUTO;
	pIO20->Focus->AutoFocusModes[1] = tt__AutoFocusMode__MANUAL;
#if 0
	pIO20->Focus->DefaultSpeed = NULL;
	pIO20->Focus->NearLimit = NULL;
	pIO20->Focus->FarLimit = NULL;
#else
	pIO20->Focus->DefaultSpeed = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Focus->DefaultSpeed->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
	pIO20->Focus->DefaultSpeed->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	pIO20->Focus->NearLimit = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Focus->NearLimit->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;;
	pIO20->Focus->NearLimit->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	pIO20->Focus->FarLimit = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->Focus->FarLimit->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
	pIO20->Focus->FarLimit->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
#endif
	pIO20->Focus->Extension = NULL;
#else
	pIO20->Focus = NULL;
#endif

	pIO20->__sizeIrCutFilterModes = 3;
	pIO20->IrCutFilterModes = (enum tt__IrCutFilterMode *)soap_malloc(soap, sizeof(enum tt__IrCutFilterMode) * pIO20->__sizeIrCutFilterModes);
	pIO20->IrCutFilterModes[0] = tt__IrCutFilterMode__ON;
	pIO20->IrCutFilterModes[1] = tt__IrCutFilterMode__OFF;
	pIO20->IrCutFilterModes[2] = tt__IrCutFilterMode__AUTO;

	pIO20->Sharpness = (struct tt__FloatRange *)soap_malloc(soap,sizeof(struct tt__FloatRange));
	pIO20->Sharpness->Min = 0.0;
	pIO20->Sharpness->Max = 10.0;

	pIO20->WideDynamicRange = (struct tt__WideDynamicRangeOptions20 *)soap_malloc(soap,sizeof(struct tt__WideDynamicRangeOptions20));
	pIO20->WideDynamicRange->__sizeMode = 2;
	pIO20->WideDynamicRange->Mode = (enum tt__WideDynamicMode *)soap_malloc(soap, sizeof(enum tt__WideDynamicMode) * pIO20->WideDynamicRange->__sizeMode);
	pIO20->WideDynamicRange->Mode[0] = tt__WideDynamicMode__OFF;
	pIO20->WideDynamicRange->Mode[1] = tt__WideDynamicMode__ON;
	pIO20->WideDynamicRange->Level = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->WideDynamicRange->Level->Min = 0.0;
	pIO20->WideDynamicRange->Level->Max = 2.0;

	pIO20->WhiteBalance = (struct tt__WhiteBalanceOptions20 *)soap_malloc(soap,sizeof(struct tt__WhiteBalanceOptions20));
	memset(pIO20->WhiteBalance, 0x00, sizeof(struct tt__WhiteBalanceOptions20));
	pIO20->WhiteBalance->__sizeMode = 2;
	pIO20->WhiteBalance->Mode = (enum tt__WhiteBalanceMode *)soap_malloc(soap, sizeof(enum tt__WhiteBalanceMode) * pIO20->WhiteBalance->__sizeMode);
	pIO20->WhiteBalance->Mode[0] = tt__WhiteBalanceMode__AUTO;
	pIO20->WhiteBalance->Mode[1] = tt__WhiteBalanceMode__MANUAL;
	pIO20->WhiteBalance->YrGain = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->WhiteBalance->YrGain->Min = 0.0;
	pIO20->WhiteBalance->YrGain->Max = 20.0;
	pIO20->WhiteBalance->YbGain = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	pIO20->WhiteBalance->YbGain->Min = 0.0;
	pIO20->WhiteBalance->YbGain->Max = 20.0;
	pIO20->WhiteBalance->Extension = NULL;

	pIO20->Extension = NULL;
	pIO20->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}


int __timg__Move(struct soap *soap, 
struct _timg__Move *timg__Move, 
struct _timg__MoveResponse *timg__MoveResponse)
{
#if 1
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__Move->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__Move->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if (timg__Move->Focus->Absolute != NULL) {
		if (timg__Move->Focus->Absolute->Position < oSysInfo->PTZC.ZoomLimit_XRange_Min || timg__Move->Focus->Absolute->Position > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}

		if (timg__Move->Focus->Absolute->Speed) {
			int speed;
			speed = *timg__Move->Focus->Absolute->Speed;
			dvprintf("%s Absolute speed %d \r\n",__func__,speed);
			if (*timg__Move->Focus->Absolute->Speed < oSysInfo->PTZC.ZoomLimit_XRange_Min || *timg__Move->Focus->Absolute->Speed > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			dvprintf("%s Absolute position %f \r\n",__func__,timg__Move->Focus->Absolute->Position);
			if (timg__Move->Focus->Absolute->Position > 0) {
				gptMsgISPTbl.ISP_FOCUS_DIR = 0x1;
				gptMsgISPTbl.ISP_FOCUS_VALUE = (UINT)(timg__Move->Focus->Absolute->Position*100.f);
			} else {
				gptMsgISPTbl.ISP_FOCUS_DIR = 0x2;
				gptMsgISPTbl.ISP_FOCUS_VALUE = (UINT)(-timg__Move->Focus->Absolute->Position*100.f);				
			}
		}
	} else if (timg__Move->Focus->Relative != NULL) {
		if (timg__Move->Focus->Relative->Distance < oSysInfo->PTZC.ZoomLimit_XRange_Min || timg__Move->Focus->Relative->Distance > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}	

		if (timg__Move->Focus->Relative->Speed) {
			int speed;
			speed = *timg__Move->Focus->Relative->Speed;
			dvprintf("%s Relative speed\r\n",__func__,speed);

			if (*timg__Move->Focus->Relative->Speed < oSysInfo->PTZC.ZoomLimit_XRange_Min || *timg__Move->Focus->Relative->Speed > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
				onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
				LEAVE();
				return SOAP_FAULT;
			}
			dvprintf("%s Relative distance %f\r\n",__func__,timg__Move->Focus->Relative->Distance);

			if(timg__Move->Focus->Relative->Distance > 0) {
				gptMsgISPTbl.ISP_FOCUS_DIR = 0x1;
				gptMsgISPTbl.ISP_FOCUS_VALUE = (UINT)(timg__Move->Focus->Relative->Distance*100.f);				
			} else {
				gptMsgISPTbl.ISP_FOCUS_DIR = 0x2;
				gptMsgISPTbl.ISP_FOCUS_VALUE = (UINT)(-timg__Move->Focus->Relative->Distance*100.f);				
			}
		}

	}
	else if(timg__Move->Focus->Continuous != NULL)
	{
		int speed;
		speed = timg__Move->Focus->Continuous->Speed;
		dvprintf("%s Continuous speed\r\n",__func__,speed);

		if (timg__Move->Focus->Continuous->Speed < oSysInfo->PTZC.ZoomLimit_XRange_Min || timg__Move->Focus->Continuous->Speed > oSysInfo->PTZC.ZoomLimit_XRange_Max) {
			onvifInterfaceFault(soap, ovfSC_SettingsInvalid, "");
			LEAVE();
			return SOAP_FAULT;
		}
	}
	LEAVE();
	return SOAP_OK;
#else
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoImagingForSource, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(timg__Move);
#endif
	UNUSED(timg__MoveResponse);
}


int __timg__Stop(struct soap *soap, 
struct _timg__Stop *timg__Stop, 
struct _timg__StopResponse *timg__StopResponse)
{
#if 1
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__Stop->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__Stop->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(timg__StopResponse);
#else
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoImagingForSource, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(timg__Stop);
	UNUSED(timg__StopResponse);
#endif
}


int __timg__GetStatus(struct soap *soap, 
struct _timg__GetStatus *timg__GetStatus, 
struct _timg__GetStatusResponse *timg__GetStatusResponse)
{
#if 1
	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__GetStatus->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__GetStatus->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	timg__GetStatusResponse->Status = (struct tt__ImagingStatus20 *)soap_malloc(soap, sizeof(struct tt__ImagingStatus20));
	memset(timg__GetStatusResponse->Status, 0x00, sizeof(struct tt__ImagingStatus20));

	timg__GetStatusResponse->Status->FocusStatus20 = (struct tt__FocusStatus20 *)soap_malloc(soap, sizeof(struct tt__FocusStatus20));
	memset(timg__GetStatusResponse->Status->FocusStatus20, 0x00, sizeof(struct tt__FocusStatus20));

	timg__GetStatusResponse->Status->FocusStatus20->Position = 0;
	timg__GetStatusResponse->Status->FocusStatus20->MoveStatus = tt__MoveStatus__IDLE;
	timg__GetStatusResponse->Status->FocusStatus20->Error = NULL;
	timg__GetStatusResponse->Status->FocusStatus20->Extension = NULL;
	timg__GetStatusResponse->Status->FocusStatus20->__anyAttribute = NULL;
	
	timg__GetStatusResponse->Status->Extension = NULL;
	timg__GetStatusResponse->Status->__anyAttribute = NULL;

	UNUSED(soap);
	LEAVE();
	return SOAP_OK;
#else
	ENTER();
	onvifInterfaceFault(soap, ovfSC_NoImagingForSource, "");
	LEAVE();
	return SOAP_FAULT;

	UNUSED(soap);
	UNUSED(timg__GetStatus);
	UNUSED(timg__GetStatusResponse);
#endif
}


int __timg__GetMoveOptions(struct soap *soap, 
struct _timg__GetMoveOptions *timg__GetMoveOptions, 
struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int idxVSC;

	for (idxVSC = 0; idxVSC < oSysInfo->SizeDefaultVSC; idxVSC++) {
		if (strcmp(timg__GetMoveOptions->VideoSourceToken, oSysInfo->VSC[idxVSC].Vsourcetoken) == 0) {
			Ptoken_exist = EXIST;
			break;
		}
	}

	if (Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", timg__GetMoveOptions->VideoSourceToken);
		onvifInterfaceFault(soap, ovfSC_NoSource, "");
		LEAVE();
		return SOAP_FAULT;
	}

#if 1
	flprintf("Get %s\r\n", timg__GetMoveOptions->VideoSourceToken == NULL ? "NULL!!!!!" : timg__GetMoveOptions->VideoSourceToken);

	timg__GetMoveOptionsResponse->MoveOptions = (struct tt__MoveOptions20 *)soap_malloc(soap, sizeof(struct tt__MoveOptions20));
	memset(timg__GetMoveOptionsResponse->MoveOptions, 0x00, sizeof(struct tt__MoveOptions20));

	timg__GetMoveOptionsResponse->MoveOptions->Absolute = (struct tt__AbsoluteFocusOptions *)soap_malloc(soap, sizeof(struct tt__AbsoluteFocusOptions));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Absolute,0x00, sizeof(struct tt__AbsoluteFocusOptions));
	
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Position = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Absolute->Position,0x00, sizeof(struct tt__FloatRange));
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Position->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Position->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
	
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Speed = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Absolute->Speed, 0x00, sizeof(struct tt__FloatRange));
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Speed->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	timg__GetMoveOptionsResponse->MoveOptions->Absolute->Speed->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
	
	timg__GetMoveOptionsResponse->MoveOptions->Relative = (struct tt__RelativeFocusOptions20 *)soap_malloc(soap, sizeof(struct tt__RelativeFocusOptions20));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Relative,0x00, sizeof(struct tt__RelativeFocusOptions20));
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Distance = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Relative->Distance, 0x00, sizeof(struct tt__FloatRange));
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Distance->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Distance->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
	
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Speed = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Relative->Speed, 0x00, sizeof(struct tt__FloatRange));
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Speed->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	timg__GetMoveOptionsResponse->MoveOptions->Relative->Speed->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;

	timg__GetMoveOptionsResponse->MoveOptions->Continuous = (struct tt__ContinuousFocusOptions *)soap_malloc(soap, sizeof(struct tt__ContinuousFocusOptions *));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Continuous,0x00, sizeof(struct tt__ContinuousFocusOptions *));
	timg__GetMoveOptionsResponse->MoveOptions->Continuous->Speed = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));
	memset(timg__GetMoveOptionsResponse->MoveOptions->Continuous->Speed,0x00, sizeof(struct tt__FloatRange));		
	timg__GetMoveOptionsResponse->MoveOptions->Continuous->Speed->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	timg__GetMoveOptionsResponse->MoveOptions->Continuous->Speed->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;
#else
	onvifInterfaceFault(soap, ovfSC_NoImagingForSource, "");
#endif

	LEAVE();
	return SOAP_OK;

//	UNUSED(soap);
//	UNUSED(timg__GetMoveOptions);
//	UNUSED(timg__GetMoveOptionsResponse);
}

