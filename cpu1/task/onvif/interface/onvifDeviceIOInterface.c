#include "soapH.h"  
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "FreeRTOS.h" 
#include <stdio.h>

#include "onvifProfile.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

/** Auto-test server operation __tmd__GetServiceCapabilities */
int __tmd__GetServiceCapabilities(struct soap *soap, 
struct _tmd__GetServiceCapabilities *tmd__GetServiceCapabilities, 
struct _tmd__GetServiceCapabilitiesResponse *tmd__GetServiceCapabilitiesResponse)
{
	ENTER();

	tmd__GetServiceCapabilitiesResponse->Capabilities = (struct tmd__Capabilities *)soap_malloc(soap, sizeof(struct tmd__Capabilities));
	memset(tmd__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct tmd__Capabilities));
	struct tmd__Capabilities *Cap = tmd__GetServiceCapabilitiesResponse->Capabilities;

	Cap->VideoSources = 1;
	Cap->VideoOutputs = 0;
#ifdef __AUDIO__
	Cap->AudioSources = 1;
#if (ONVIF_BACKCHANNEL==1)
	Cap->AudioOutputs = 1;
#else
	Cap->AudioOutputs = 0;
#endif
#else
	Cap->AudioSources = 0;
	Cap->AudioOutputs = 0;
#endif
	Cap->RelayOutputs = 0;
	Cap->SerialPorts = 0;
	Cap->DigitalInputs = 0;

	Cap->__size = 0;
	Cap->__any = NULL;
	Cap->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tmd__GetServiceCapabilities);
}


/** Auto-test server operation __tmd__GetRelayOutputOptions */
int __tmd__GetRelayOutputOptions(struct soap *soap, 
struct _tmd__GetRelayOutputOptions *tmd__GetRelayOutputOptions, 
struct _tmd__GetRelayOutputOptionsResponse *tmd__GetRelayOutputOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetRelayOutputOptions);
	UNUSED(tmd__GetRelayOutputOptionsResponse);
}


/** Auto-test server operation __tmd__GetAudioSources */
int __tmd__GetAudioSources(struct soap *soap, 
struct _trt__GetAudioSources *trt__GetAudioSources, 
struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse)
{
	ENTER();

	trt__GetAudioSourcesResponse->__sizeAudioSources = 1;
	trt__GetAudioSourcesResponse->AudioSources = (struct tt__AudioSource *)soap_malloc(soap, sizeof(struct tt__AudioSource));
	memset(trt__GetAudioSourcesResponse->AudioSources, 0x00, sizeof(struct tt__AudioSource));
	trt__GetAudioSourcesResponse->AudioSources->token = soap_strdup(soap,"g711token");
	//trt__GetAudioSourcesResponse->AudioSources->token = (char *) soap_malloc(soap, sizeof(char)*10); 
	//strcpy(trt__GetAudioSourcesResponse->AudioSources->token, "g711token");

	trt__GetAudioSourcesResponse->AudioSources->Channels = 1;
	trt__GetAudioSourcesResponse->AudioSources->__size = 0;
	trt__GetAudioSourcesResponse->AudioSources->__any = NULL;
	trt__GetAudioSourcesResponse->AudioSources->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(trt__GetAudioSources);
}


/** Auto-test server operation __tmd__GetAudioOutputs */
int __tmd__GetAudioOutputs(struct soap *soap, 
struct _trt__GetAudioOutputs *trt__GetAudioOutputs, 
struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetAudioOutputs);
	UNUSED(trt__GetAudioOutputsResponse);
}

/**
 * @brief Get Video Source 
 * @This operation lists all available video sources for the device.
 *   
 * @param Request	: _trt__GetVideoSources structure contains an empty message.  
 * @param Response	: _trt__GetVideoSourcesResponse structure contains a list of
                      structures describing all available video sources of the device.
 * @return               : On success SOAP_OK.
 : On failure Audio or Audio Outputs are not supported by the NVT.

 */
int __tmd__GetVideoSources(struct soap *soap,
                           struct _trt__GetVideoSources *trt__GetVideoSources,
                           struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse)
{
	ENTER();
#if 0// 

	SysInfo* oSysInfo = GetSysInfo();
	int flag = 0, j =0;
	float _Framerate; 
	float _Brightness;
	float _Saturation; 
	float _Contrast; 
	float _Sharpness; 
	int   _WideDynamicMode;
	float _WideDynamicRange;
	int _IrCutFilter;
	int _Backlight; 
	float _BacklightCompensationLevel; 
	int _WhiteBalanceMode;
	float _WhiteBalanceCr;
	float _WhiteBalanceCb;
	int height;
	int size = 0;//GetSysInfo()->nprofile; 
	int i = 0;
	int n = 0;
	int lenVsrcToken = 0;
	int index;
	for(i = 0; i < oSysInfo->nProfileSize; i++)
	{
		flag = NOT_EXIST;
		for(j = 0; j <= i; j++)
		{
			if(strcmp(GetSysInfo()->Oprofile., GetSysInfo()->Oprofile[i].VSC.Vsourcetoken)==0)
			{
				flag = EXIST;
				break;
			}
		}
		if(flag == EXIST)
		{
			size++;
		}
	}
	//printf("onvif io size %d profile n %d\r\n",size,GetProfileSize());
	trt__GetVideoSourcesResponse->__sizeVideoSources = size;

	if(size <= 0)
	{
		onvif_fault(soap,"","ter:InvalidArgVal", "ter:NoConfig","","");
		return SOAP_FAULT;
	}
	trt__GetVideoSourcesResponse->VideoSources = (struct tt__VideoSource *)soap_malloc(soap, sizeof(struct tt__VideoSource) * size);
	memset(trt__GetVideoSourcesResponse->VideoSources, 0x00, sizeof(struct tt__VideoSource) * size);
	dvprintf("onvif io 1\r\n");

	for(i = 0; i < oSysInfo->nProfileSize ; i++)
	{
		flag = NOT_EXIST;
		for(j = 0; j <= i; j++)
		{
			if(strcmp(GetSysInfo()->Oprofile[j].VSC.Vsourcetoken, GetSysInfo()->Oprofile[i].VSC.Vsourcetoken)==0)
			{
				flag = EXIST;
				break;
			}
		}

		if(flag == EXIST)  
		{
			dvprintf("onvif io 2\r\n");

#if 1//def MN34220		
			_Framerate = oget_framerate();
			dvprintf("a _Framerate %f\r\n",_Framerate);
			dvprintf("b _Framerate %f\r\n",GetSysInfo()->gImagingSettings.Brightness);
			_Brightness = (float)(GetSysInfo()->gImagingSettings.Brightness * 100) / 255; 
			_Saturation = (float)(GetSysInfo()->gImagingSettings.ColorSaturation * 100) / 255; 
			_Contrast = (float)(GetSysInfo()->gImagingSettings.Contrast * 100) / 255; 
			_Sharpness = (float)(GetSysInfo()->gImagingSettings.Sharpness * 100) / 255;
			_WideDynamicMode = GetSysInfo()->gImagingSettings.WideDynamicRange.Mode;
			_WideDynamicRange = GetSysInfo()->gImagingSettings.WideDynamicRange.Level;
			_Backlight = GetSysInfo()->gImagingSettings.BacklightCompensation.Mode; 
			_BacklightCompensationLevel = GetSysInfo()->gImagingSettings.BacklightCompensation.Level;
			_IrCutFilter = GetSysInfo()->gImagingSettings.IrCutFilter;
			_WhiteBalanceMode = GetSysInfo()->gImagingSettings.WhiteBalance.Mode;
			_WhiteBalanceCr = GetSysInfo()->gImagingSettings.WhiteBalance.CrGain;
			_WhiteBalanceCb = GetSysInfo()->gImagingSettings.WhiteBalance.CbGain;
			
			//height = oget_resolutionheight();
			//printf("i %d n %d height %d\r\n", i, n, height);
			dvprintf("_Framerate %f\r\n",_Framerate);
			dvprintf("_Brightness %f\r\n",_Brightness);
			dvprintf("_Saturation %f\r\n",_Saturation);
			dvprintf("_Contrast %f\r\n",_Contrast);
			dvprintf("_Sharpness %f\r\n",_Sharpness);
			dvprintf("_WideDynamicRange %d\r\n",_WideDynamicRange);
			dvprintf("_Backlight %d\r\n",_Backlight);
			dvprintf("_BacklightCompensationLevel %f\r\n",_BacklightCompensationLevel);
			dvprintf("_WhiteBalance %d\r\n",_WhiteBalanceMode);
#endif	

			//		for(i=0; i< size; i++)
			//lenVsrcToken = strlen(GetSysInfo()->Oprofile[i].VSC.Vsourcetoken);
			
			trt__GetVideoSourcesResponse->VideoSources[n].token = soap_strdup(soap,GetSysInfo()->Oprofile[i].VSC.Vsourcetoken);
			//trt__GetVideoSourcesResponse->VideoSources[n].token = (char *) soap_malloc(soap, sizeof(char)*50); 
			//strcpy(trt__GetVideoSourcesResponse->VideoSources[n].token, GetSysInfo()->Oprofile[i].VSC.Vsourcetoken);

			trt__GetVideoSourcesResponse->VideoSources[n].Framerate 		= GetSysInfo()->Oprofile[i].VSC.Framerate;

			trt__GetVideoSourcesResponse->VideoSources[n].Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
			memset(trt__GetVideoSourcesResponse->VideoSources[n].Resolution, 0x00, sizeof(struct tt__VideoResolution));
			trt__GetVideoSourcesResponse->VideoSources[n].Resolution->Width  = GetSysInfo()->Oprofile[i].VSC.windowwidth;//width;
			trt__GetVideoSourcesResponse->VideoSources[n].Resolution->Height = GetSysInfo()->Oprofile[i].VSC.windowheight;

			//trt__GetVideoSourcesResponse->VideoSources[n].Imaging = NULL;
#if 1

			trt__GetVideoSourcesResponse->VideoSources[n].Imaging = (struct tt__ImagingSettings*)soap_malloc(soap, sizeof(struct tt__ImagingSettings));
			memset(trt__GetVideoSourcesResponse->VideoSources[n].Imaging, 0x00, sizeof(struct tt__ImagingSettings));
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Brightness = (float *) soap_malloc(soap, sizeof(float));
			*(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Brightness) = _Brightness;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->ColorSaturation = (float *) soap_malloc(soap, sizeof(float));
			*(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->ColorSaturation) = _Saturation;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Contrast = (float *) soap_malloc(soap, sizeof(float));
			*(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Contrast) = _Contrast;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->IrCutFilter = (int *) soap_malloc(soap, sizeof(int));
			*(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->IrCutFilter) =  _IrCutFilter;// dummy //{onv__IrCutFilterMode__ON = 0, onv__IrCutFilterMode__OFF = 1, onv__IrCutFilterMode__AUTO = 2}
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Sharpness = (float *) soap_malloc(soap, sizeof(float));
			*(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Sharpness) = _Sharpness;

			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->BacklightCompensation = (struct tt__BacklightCompensation*)soap_malloc(soap, sizeof(struct tt__BacklightCompensation));
			memset(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->BacklightCompensation, 0x00, sizeof(struct tt__BacklightCompensation));
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->BacklightCompensation->Mode = _Backlight;//Streaming_onvif->BacklightCompensationMode;  //{onv__BacklightCompensationMode__OFF = 0, onv__BacklightCompensationMode__ON = 1}
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->BacklightCompensation->Level = _BacklightCompensationLevel;//float
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Exposure = NULL;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->Focus = NULL;

			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WideDynamicRange = (struct tt__WideDynamicRange*)soap_malloc(soap, sizeof(struct tt__WideDynamicRange));
			memset(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WideDynamicRange, 0x00, sizeof(struct tt__WideDynamicRange));
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WideDynamicRange->Mode = _WideDynamicMode;	//{onv__WideDynamicMode__OFF = 0, onv__WideDynamicMode__ON = 1}
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WideDynamicRange->Level = _WideDynamicRange;// dummy float

			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance = (struct tt__WhiteBalance*)soap_malloc(soap, sizeof(struct tt__WhiteBalance));
			memset(trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance, 0x00, sizeof(struct tt__WhiteBalance));
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->Mode = _WhiteBalanceMode;	//{onv__WhiteBalanceMode__AUTO = 0, onv__WhiteBalanceMode__MANUAL = 1}
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->CrGain = _WhiteBalanceCr; // dummy
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->CbGain = _WhiteBalanceCb; // dummy
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->__size = 0;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->__any = NULL;
			trt__GetVideoSourcesResponse->VideoSources[n].Imaging->WhiteBalance->__anyAttribute = NULL;

#endif
			trt__GetVideoSourcesResponse->VideoSources[n].Extension = NULL;
			trt__GetVideoSourcesResponse->VideoSources[n].__anyAttribute = NULL;

			n++;
		}
	}
#endif
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(trt__GetVideoSources);
	UNUSED(trt__GetVideoSourcesResponse);
}


/** Auto-test server operation __tmd__GetVideoOutputs */
int __tmd__GetVideoOutputs(struct soap *soap, 
struct _tmd__GetVideoOutputs *tmd__GetVideoOutputs, 
struct _tmd__GetVideoOutputsResponse *tmd__GetVideoOutputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetVideoOutputs);
	UNUSED(tmd__GetVideoOutputsResponse);
}


/** Auto-test server operation __tmd__GetVideoSourceConfiguration */
int __tmd__GetVideoSourceConfiguration(struct soap *soap, 
struct _tmd__GetVideoSourceConfiguration *tmd__GetVideoSourceConfiguration, 
struct _tmd__GetVideoSourceConfigurationResponse *tmd__GetVideoSourceConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetVideoSourceConfiguration);
	UNUSED(tmd__GetVideoSourceConfigurationResponse);
}


/** Auto-test server operation __tmd__GetVideoOutputConfiguration */
int __tmd__GetVideoOutputConfiguration(struct soap *soap, 
struct _tmd__GetVideoOutputConfiguration *tmd__GetVideoOutputConfiguration, 
struct _tmd__GetVideoOutputConfigurationResponse *tmd__GetVideoOutputConfigurationResponse)
{
	ENTER();
#if 0 		
	SysInfo *oSysInfo = GetSysInfo();
	int i;
	int Ptoken_exist = NOT_EXIST;

	for(i = 0; i <= oSysInfo->nProfileSize; i++)
	{
		if(strcmp(tmd__GetVideoOutputConfiguration->VideoOutputToken, GetSysInfo()->Oprofile[i].VOC.name) == 0)
		{
			Ptoken_exist = EXIST;
			break;
		}
	}
	Ptoken_exist = EXIST;
	if(Ptoken_exist == NOT_EXIST)
	{
		onvif_fault(soap,"","ter:InvalidArgVal", "ter:NoConfig","","");
		return SOAP_FAULT;
	}
	
	tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration = (struct tt__VideoOutputConfiguration *)soap_malloc(soap, sizeof(struct tt__VideoOutputConfiguration));
	memset(tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration,0x00, sizeof(struct tt__VideoOutputConfiguration));
	tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->Name = soap_strdup(soap, GetSysInfo()->Oprofile[0].VOC.name);
	//tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->Name = (char *) soap_malloc(soap, sizeof(char)*50); 
	//strcpy(tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->Name, GetSysInfo()->Oprofile[0].VOC.name);
	
	tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->UseCount = GetSysInfo()->Oprofile[0].VOC.usecount;
	tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->token = soap_strdup(soap, GetSysInfo()->Oprofile[0].VOC.token);
	//tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->token = (char *) soap_malloc(soap, sizeof(char)*50); 
	//strcpy(tmd__GetVideoOutputConfigurationResponse->VideoOutputConfiguration->token, GetSysInfo()->Oprofile[0].VOC.token);
#endif
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetVideoOutputConfiguration);
	UNUSED(tmd__GetVideoOutputConfigurationResponse);
}


/** Auto-test server operation __tmd__GetAudioSourceConfiguration */
int __tmd__GetAudioSourceConfiguration(struct soap *soap, 
struct _tmd__GetAudioSourceConfiguration *tmd__GetAudioSourceConfiguration, 
struct _tmd__GetAudioSourceConfigurationResponse *tmd__GetAudioSourceConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetAudioSourceConfiguration);
	UNUSED(tmd__GetAudioSourceConfigurationResponse);
}


/** Auto-test server operation __tmd__GetAudioOutputConfiguration */
int __tmd__GetAudioOutputConfiguration(struct soap *soap,
struct _tmd__GetAudioOutputConfiguration *tmd__GetAudioOutputConfiguration, 
struct _tmd__GetAudioOutputConfigurationResponse *tmd__GetAudioOutputConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetAudioOutputConfiguration);
	UNUSED(tmd__GetAudioOutputConfigurationResponse);
}


/** Auto-test server operation __tmd__SetVideoSourceConfiguration */
int __tmd__SetVideoSourceConfiguration(struct soap *soap, 
struct _tmd__SetVideoSourceConfiguration *tmd__SetVideoSourceConfiguration, 
struct _tmd__SetVideoSourceConfigurationResponse *tmd__SetVideoSourceConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetVideoSourceConfiguration);
	UNUSED(tmd__SetVideoSourceConfigurationResponse);
}


/** Auto-test server operation __tmd__SetVideoOutputConfiguration */
int __tmd__SetVideoOutputConfiguration(struct soap *soap, 
struct _tmd__SetVideoOutputConfiguration *tmd__SetVideoOutputConfiguration, 
struct _tmd__SetVideoOutputConfigurationResponse *tmd__SetVideoOutputConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetVideoOutputConfiguration);
	UNUSED(tmd__SetVideoOutputConfigurationResponse);
}


/** Auto-test server operation __tmd__SetAudioSourceConfiguration */
int __tmd__SetAudioSourceConfiguration(struct soap *soap, 
struct _tmd__SetAudioSourceConfiguration *tmd__SetAudioSourceConfiguration,
struct _tmd__SetAudioSourceConfigurationResponse *tmd__SetAudioSourceConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetAudioSourceConfiguration);
	UNUSED(tmd__SetAudioSourceConfigurationResponse);
}


/** Auto-test server operation __tmd__SetAudioOutputConfiguration */
int __tmd__SetAudioOutputConfiguration(struct soap *soap, 
struct _tmd__SetAudioOutputConfiguration *tmd__SetAudioOutputConfiguration,
struct _tmd__SetAudioOutputConfigurationResponse *tmd__SetAudioOutputConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetAudioOutputConfiguration);
	UNUSED(tmd__SetAudioOutputConfigurationResponse);
}


/** Auto-test server operation __tmd__GetVideoSourceConfigurationOptions */
int __tmd__GetVideoSourceConfigurationOptions(struct soap *soap,
struct _tmd__GetVideoSourceConfigurationOptions *tmd__GetVideoSourceConfigurationOptions, 
struct _tmd__GetVideoSourceConfigurationOptionsResponse *tmd__GetVideoSourceConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetVideoSourceConfigurationOptions);
	UNUSED(tmd__GetVideoSourceConfigurationOptionsResponse);
}


/** Auto-test server operation __tmd__GetVideoOutputConfigurationOptions */
int __tmd__GetVideoOutputConfigurationOptions(struct soap *soap,
struct _tmd__GetVideoOutputConfigurationOptions *tmd__GetVideoOutputConfigurationOptions,
struct _tmd__GetVideoOutputConfigurationOptionsResponse *tmd__GetVideoOutputConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetVideoOutputConfigurationOptions);
	UNUSED(tmd__GetVideoOutputConfigurationOptionsResponse);
}


/** Auto-test server operation __tmd__GetAudioSourceConfigurationOptions */
int __tmd__GetAudioSourceConfigurationOptions(struct soap *soap, 
struct _tmd__GetAudioSourceConfigurationOptions *tmd__GetAudioSourceConfigurationOptions,
struct _tmd__GetAudioSourceConfigurationOptionsResponse *tmd__GetAudioSourceConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetAudioSourceConfigurationOptions);
	UNUSED(tmd__GetAudioSourceConfigurationOptionsResponse);
}


/** Auto-test server operation __tmd__GetAudioOutputConfigurationOptions */
int __tmd__GetAudioOutputConfigurationOptions(struct soap *soap, 
struct _tmd__GetAudioOutputConfigurationOptions *tmd__GetAudioOutputConfigurationOptions, 
struct _tmd__GetAudioOutputConfigurationOptionsResponse *tmd__GetAudioOutputConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetAudioOutputConfigurationOptions);
	UNUSED(tmd__GetAudioOutputConfigurationOptionsResponse);
}


/** Auto-test server operation __tmd__GetRelayOutputs */
int __tmd__GetRelayOutputs(struct soap *soap,
struct _tds__GetRelayOutputs *tds__GetRelayOutputs, 
struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetRelayOutputs);
	UNUSED(tds__GetRelayOutputsResponse);
}


/** Auto-test server operation __tmd__SetRelayOutputSettings */
int __tmd__SetRelayOutputSettings(struct soap *soap, 
struct _tmd__SetRelayOutputSettings *tmd__SetRelayOutputSettings, 
struct _tmd__SetRelayOutputSettingsResponse *tmd__SetRelayOutputSettingsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetRelayOutputSettings);
	UNUSED(tmd__SetRelayOutputSettingsResponse);
}


/** Auto-test server operation __tmd__SetRelayOutputState */
int __tmd__SetRelayOutputState(struct soap *soap, 
struct _tds__SetRelayOutputState *tds__SetRelayOutputState, 
struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetRelayOutputState);
	UNUSED(tds__SetRelayOutputStateResponse);
}


/** Auto-test server operation __tmd__GetDigitalInputs */
int __tmd__GetDigitalInputs(struct soap *soap, 
struct _tmd__GetDigitalInputs *tmd__GetDigitalInputs,
struct _tmd__GetDigitalInputsResponse *tmd__GetDigitalInputsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetDigitalInputs);
	UNUSED(tmd__GetDigitalInputsResponse);
}


/** Auto-test server operation __tmd__GetSerialPorts */
int __tmd__GetSerialPorts(struct soap *soap, 
struct _tmd__GetSerialPorts *tmd__GetSerialPorts, 
struct _tmd__GetSerialPortsResponse *tmd__GetSerialPortsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetSerialPorts);
	UNUSED(tmd__GetSerialPortsResponse);
}


/** Auto-test server operation __tmd__GetSerialPortConfiguration */
int __tmd__GetSerialPortConfiguration(struct soap *soap,
struct _tmd__GetSerialPortConfiguration *tmd__GetSerialPortConfiguration,
struct _tmd__GetSerialPortConfigurationResponse *tmd__GetSerialPortConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetSerialPortConfiguration);
	UNUSED(tmd__GetSerialPortConfigurationResponse);
}


/** Auto-test server operation __tmd__SetSerialPortConfiguration */
int __tmd__SetSerialPortConfiguration(struct soap *soap, 
struct _tmd__SetSerialPortConfiguration *tmd__SetSerialPortConfiguration,
struct _tmd__SetSerialPortConfigurationResponse *tmd__SetSerialPortConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SetSerialPortConfiguration);
	UNUSED(tmd__SetSerialPortConfigurationResponse);
}


/** Auto-test server operation __tmd__GetSerialPortConfigurationOptions */
int __tmd__GetSerialPortConfigurationOptions(struct soap *soap,
struct _tmd__GetSerialPortConfigurationOptions *tmd__GetSerialPortConfigurationOptions, 
struct _tmd__GetSerialPortConfigurationOptionsResponse *tmd__GetSerialPortConfigurationOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__GetSerialPortConfigurationOptions);
	UNUSED(tmd__GetSerialPortConfigurationOptionsResponse);
}


/** Auto-test server operation __tmd__SendReceiveSerialCommand */
int __tmd__SendReceiveSerialCommand(struct soap *soap,
struct _tmd__SendReceiveSerialCommand *tmd__SendReceiveSerialCommand, 
struct _tmd__SendReceiveSerialCommandResponse *tmd__SendReceiveSerialCommandResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tmd__SendReceiveSerialCommand);
	UNUSED(tmd__SendReceiveSerialCommandResponse);
}


