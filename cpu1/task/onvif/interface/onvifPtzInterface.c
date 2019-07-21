#include "soapH.h"  
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "FreeRTOS.h" 
#include <stdio.h>
#include "onvifUtil.h"
#include "onvifSystemInfo.h"
#include "msg.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

int __tptz__GetServiceCapabilities(struct soap *soap, 
struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, 
struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse)
{
	ENTER();

	tptz__GetServiceCapabilitiesResponse->Capabilities = (struct tptz__Capabilities *)soap_malloc(soap, sizeof(struct tptz__Capabilities));	
	memset(tptz__GetServiceCapabilitiesResponse->Capabilities, 0x00, sizeof(struct tptz__Capabilities)); 

	tptz__GetServiceCapabilitiesResponse->Capabilities->__size = 1;
	tptz__GetServiceCapabilitiesResponse->Capabilities->EFlip = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));	
	memset(tptz__GetServiceCapabilitiesResponse->Capabilities->EFlip, 0x00, sizeof(enum xsd__boolean));
	*tptz__GetServiceCapabilitiesResponse->Capabilities->EFlip = xsd__boolean__false_; 
	
	tptz__GetServiceCapabilitiesResponse->Capabilities->Reverse = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));	
	memset(tptz__GetServiceCapabilitiesResponse->Capabilities->Reverse, 0x00, sizeof(enum xsd__boolean));
	*tptz__GetServiceCapabilitiesResponse->Capabilities->Reverse = xsd__boolean__false_; 
	tptz__GetServiceCapabilitiesResponse->Capabilities->GetCompatibleConfigurations = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));	
	memset(tptz__GetServiceCapabilitiesResponse->Capabilities->GetCompatibleConfigurations, 0x00, sizeof(enum xsd__boolean));
	*tptz__GetServiceCapabilitiesResponse->Capabilities->GetCompatibleConfigurations = xsd__boolean__false_;
	
	tptz__GetServiceCapabilitiesResponse->Capabilities->__anyAttribute = NULL;
	tptz__GetServiceCapabilitiesResponse->Capabilities->__any = NULL;
		
	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__GetServiceCapabilities);
}

int __tptz__GetConfigurations(struct soap *soap, 
struct _tptz__GetConfigurations *tptz__GetConfigurations, 
struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();

	tptz__GetConfigurationsResponse->__sizePTZConfiguration = 1;

	tptz__GetConfigurationsResponse->PTZConfiguration = (struct tt__PTZConfiguration *)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration, 0x00, sizeof(struct tt__PTZConfiguration));
		
	tptz__GetConfigurationsResponse->PTZConfiguration->Name = soap_strdup(soap, oSysInfo->PTZC.PTZname);
	tptz__GetConfigurationsResponse->PTZConfiguration->UseCount = oSysInfo->PTZC.PTZusecount;
	tptz__GetConfigurationsResponse->PTZConfiguration->token = soap_strdup(soap, oSysInfo->PTZC.PTZtoken);
	tptz__GetConfigurationsResponse->PTZConfiguration->NodeToken = soap_strdup(soap, oSysInfo->PTZC.Node[0].Token);
	
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace);
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsoluteZoomPositionSpace);
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativePanTiltTranslationSpace);
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativeZoomTranslationSpace);
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace);
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousZoomVelocitySpace);
	
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed = (struct tt__PTZSpeed *)soap_malloc(soap, sizeof(struct tt__PTZSpeed));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed, 0x00, sizeof(struct tt__PTZSpeed));
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt, 0x00, sizeof(struct tt__Vector2D));
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->x = oSysInfo->PTZC.DefaultPantilt_x;
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->y = oSysInfo->PTZC.DefaultPantilt_y;
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap, oSysInfo->PTZC.DefaultPantilt_space);
	
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->Zoom, 0x00, sizeof(struct tt__Vector1D));
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->x = oSysInfo->PTZC.DefaultZoom_x;
	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.DefaultZoom_space);

	tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZTimeout = (LONG64 *)soap_malloc(soap, sizeof(LONG64));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZTimeout, 0x00, sizeof(LONG64));
	*tptz__GetConfigurationsResponse->PTZConfiguration->DefaultPTZTimeout =  oSysInfo->PTZC.DefaultPTZTimeout;
	
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits = (struct tt__PanTiltLimits *)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits, 0x00, sizeof(struct tt__PanTiltLimits));
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range, 0x00, sizeof(struct tt__Space2DDescription));
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.PanTiltLimit_URI);
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Max = oSysInfo->PTZC.PanTiltLimit_XRange_Max;
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Min = oSysInfo->PTZC.PanTiltLimit_XRange_Min;
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->YRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Max	= oSysInfo->PTZC.PanTiltLimit_YRange_Max;
	tptz__GetConfigurationsResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Min	= oSysInfo->PTZC.PanTiltLimit_YRange_Min;
		
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits = (struct tt__ZoomLimits *)soap_malloc(soap, sizeof(struct tt__ZoomLimits));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits, 0x00, sizeof(struct tt__ZoomLimits));
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.ZoomLimit_XRange_URI);
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range->XRange->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	tptz__GetConfigurationsResponse->PTZConfiguration->ZoomLimits->Range->XRange->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;

	tptz__GetConfigurationsResponse->PTZConfiguration->Extension = NULL;
	tptz__GetConfigurationsResponse->PTZConfiguration->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__GetConfigurations);
}

int __tptz__GetPresets(struct soap *soap, 
	struct _tptz__GetPresets *tptz__GetPresets, 
	struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;
	
	if(tptz__GetPresets->ProfileToken!=NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__GetPresets->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
			}
		}
		
	}
	else
	{
		flprintf("Error, ProfileToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__GetPresets->ProfileToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	tptz__GetPresetsResponse->__sizePreset = oSysInfo->PTZC.SizePreset;
	tptz__GetPresetsResponse->Preset = (struct tt__PTZPreset *)soap_malloc(soap, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);	
	memset(tptz__GetPresetsResponse->Preset, 0x00, sizeof(struct tt__PTZPreset)*tptz__GetPresetsResponse->__sizePreset);

	for(i=0;i<tptz__GetPresetsResponse->__sizePreset;i++)
	{
		tptz__GetPresetsResponse->Preset[i].Name = soap_strdup(soap, oSysInfo->PTZC.Preset[i].Name);
		tptz__GetPresetsResponse->Preset[i].token = soap_strdup(soap, oSysInfo->PTZC.Preset[i].Token);
		tptz__GetPresetsResponse->Preset[i].PTZPosition = (struct tt__PTZVector *)soap_malloc(soap, sizeof(struct tt__PTZVector));	
		memset(tptz__GetPresetsResponse->Preset[i].PTZPosition, 0x00, sizeof(struct tt__PTZVector));
		tptz__GetPresetsResponse->Preset[i].PTZPosition->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));	
		memset(tptz__GetPresetsResponse->Preset[i].PTZPosition->PanTilt, 0x00, sizeof(struct tt__Vector2D));
		tptz__GetPresetsResponse->Preset[i].PTZPosition->PanTilt->space = soap_strdup(soap, oSysInfo->PTZC.Preset[i].Pantilt_space);
		tptz__GetPresetsResponse->Preset[i].PTZPosition->PanTilt->x = oSysInfo->PTZC.Preset[i].Pantilt_x;
		tptz__GetPresetsResponse->Preset[i].PTZPosition->PanTilt->y = oSysInfo->PTZC.Preset[i].Pantilt_y;
		tptz__GetPresetsResponse->Preset[i].PTZPosition->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));	
		memset(tptz__GetPresetsResponse->Preset[i].PTZPosition->Zoom, 0x00, sizeof(struct tt__Vector1D));
		tptz__GetPresetsResponse->Preset[i].PTZPosition->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.Preset[i].Zoom_space);
		tptz__GetPresetsResponse->Preset[i].PTZPosition->Zoom->x = oSysInfo->PTZC.Preset[i].Zoom_x;
		tptz__GetPresetsResponse->Preset[i].__anyAttribute = NULL;
	}

	LEAVE();
	return SOAP_OK;
}

int __tptz__SetPreset(struct soap *soap, 
	struct _tptz__SetPreset *tptz__SetPreset, 
	struct _tptz__SetPresetResponse *tptz__SetPresetResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i, j;
	char ptoken[SMALL_INFO_LENGTH] = {0};

	if(tptz__SetPreset->ProfileToken!=NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__SetPreset->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
			}
		}
		
	}
	else
	{
		flprintf("Error, ProfileToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) 
	{
		if(tptz__SetPreset->ProfileToken != NULL)
		{
			flprintf("Error, NoSource(%s)\r\n", tptz__SetPreset->ProfileToken);
		}
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(tptz__SetPreset->PresetName != NULL)
	{
		printf("Set preset Name %s \r\n",tptz__SetPreset->PresetName);
	}

	if(tptz__SetPreset->PresetToken != NULL)
	{
		printf("Set preset Token %s\r\n", tptz__SetPreset->PresetToken);
	}

	if(tptz__SetPreset->PresetName != NULL)
	{
		for(i=0;i<oSysInfo->PTZC.SizePreset;i++)
		{		
			if (strcmp(tptz__SetPreset->PresetName, oSysInfo->PTZC.Preset[i].Name) == 0) 
			{
//				onvifInterfaceFault(soap, ovfSC_PTZPresetExist, "");
//				LEAVE();
//				return SOAP_FAULT;
				if(tptz__SetPreset->PresetToken != NULL)
				{
					for(j=0;j<oSysInfo->PTZC.SizePreset;j++)
					{		
						if (strcmp(tptz__SetPreset->PresetToken, oSysInfo->PTZC.Preset[j].Token) == 0) 
						{
							strcpy(ptoken, "DEFAULT");
							//strcpy(ptoken+strlen(ptoken), itoa(j));
							printf("Preset token1 %s\r\n", ptoken);
							tptz__SetPresetResponse->PresetToken = soap_strdup(soap, ptoken);
							break;
						}
					}			
				}
				break;
			}
		}
		if(PTZ_PRESET_MAX_SIZE > oSysInfo->PTZC.SizePreset)
		{
			oSysInfo->PTZC.SizePreset++;
				
			strcpy(oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Name, tptz__SetPreset->PresetName);

			if(strlen(ptoken))
			{
				strcpy(oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Token, ptoken);
				tptz__SetPresetResponse->PresetToken = soap_strdup(soap, ptoken);
			}
			else
			{
				strcpy(oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Token, tptz__SetPreset->PresetName);
				tptz__SetPresetResponse->PresetToken = soap_strdup(soap, oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Token);
			}
		}
	}
	else if(tptz__SetPreset->PresetToken != NULL)
	{
		for(i=0;i<oSysInfo->PTZC.SizePreset;i++)
		{		
			if (strcmp(tptz__SetPreset->PresetToken, oSysInfo->PTZC.Preset[i].Token) == 0) 
			{
//				onvifInterfaceFault(soap, ovfSC_PTZPresetExist, "");
//				LEAVE();
//				return SOAP_FAULT;
				strcpy(ptoken, "DEFAULT");
				//strcpy(ptoken+strlen(ptoken), itoa(j));
				printf("Preset token2 %s\r\n", ptoken);
				tptz__SetPresetResponse->PresetToken = soap_strdup(soap, ptoken);
				break;
			}
		}
		if(PTZ_PRESET_MAX_SIZE > oSysInfo->PTZC.SizePreset)
		{
			oSysInfo->PTZC.SizePreset++;
				
			strcpy(oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Name, tptz__SetPreset->PresetToken);
			strcpy(oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Token, tptz__SetPreset->PresetToken);
			
			tptz__SetPresetResponse->PresetToken = soap_strdup(soap, oSysInfo->PTZC.Preset[oSysInfo->PTZC.SizePreset-1].Token);
		}
	}

	LEAVE();
	return SOAP_OK;
}

int __tptz__RemovePreset(struct soap *soap, 
	struct _tptz__RemovePreset *tptz__RemovePreset, 
	struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i, j;
	
	if(tptz__RemovePreset->ProfileToken!=NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__RemovePreset->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
			}
		}
		
	}
	else
	{
		flprintf("Error, ProfileToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) 
	{
		if(tptz__RemovePreset->ProfileToken != NULL)
		{
			flprintf("Error, NoSource(%s)\r\n", tptz__RemovePreset->ProfileToken);
		}
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(tptz__RemovePreset->PresetToken != NULL)
	{
		for(i=0;i<oSysInfo->PTZC.SizePreset;i++)
		{		
			if (strcmp(tptz__RemovePreset->PresetToken, oSysInfo->PTZC.Preset[i].Token) == 0) 
			{
				if(i<oSysInfo->PTZC.SizePreset-1)
				{
					for(j=i;j<oSysInfo->PTZC.SizePreset-1;j++)
					{		
						strcpy(oSysInfo->PTZC.Preset[j].Name, oSysInfo->PTZC.Preset[j+1].Name);
						strcpy(oSysInfo->PTZC.Preset[j].Token, oSysInfo->PTZC.Preset[j+1].Token);
						
						strcpy(oSysInfo->PTZC.Preset[j+1].Name, "DEFAULT_PRESET");
						strcpy(oSysInfo->PTZC.Preset[j+1].Token, "DEFAULT_TOKKEN");
					}
					oSysInfo->PTZC.SizePreset--;
				}
				else
				{
					strcpy(oSysInfo->PTZC.Preset[i].Name, "DEFAULT_PRESET");
					strcpy(oSysInfo->PTZC.Preset[i].Token, "DEFAULT_TOKKEN");
					oSysInfo->PTZC.SizePreset--;
				}
				break;
			}
		}
	
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__RemovePresetResponse);
}

int __tptz__GotoPreset(struct soap *soap, 
	struct _tptz__GotoPreset *tptz__GotoPreset, 
	struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__GotoPreset->ProfileToken!=NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__GotoPreset->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
			}
		}
		
	}
	else
	{
		flprintf("Error, ProfileToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) 
	{
		if(tptz__GotoPreset->ProfileToken != NULL)
		{
			flprintf("Error, NoSource(%s)\r\n", tptz__GotoPreset->ProfileToken);
		}
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(tptz__GotoPreset->PresetToken != NULL)
	{
		for(i=0;i<oSysInfo->PTZC.SizePreset;i++)
		{
			if (strcmp(tptz__GotoPreset->PresetToken, oSysInfo->PTZC.Preset[i].Token) == 0) 
			{
				if(tptz__GotoPreset->Speed != NULL)
				{
					printf("Go to Preset \r\n");

					if(tptz__GotoPreset->Speed->PanTilt != NULL)
					{
						if(tptz__GotoPreset->Speed->PanTilt->space != NULL)
						{
							printf("Pantilt space %s x %d y %d\r\n",tptz__GotoPreset->Speed->PanTilt->space,tptz__GotoPreset->Speed->PanTilt->x,tptz__GotoPreset->Speed->PanTilt->y);
							strcpy(oSysInfo->PTZC.CurrentPantilt_space, tptz__GotoPreset->Speed->PanTilt->space);
							oSysInfo->PTZC.CurrentPantilt_x = tptz__GotoPreset->Speed->PanTilt->x;
							oSysInfo->PTZC.CurrentPantilt_y = tptz__GotoPreset->Speed->PanTilt->y;
						}
					}

					if(tptz__GotoPreset->Speed->Zoom != NULL)
					{
						if(tptz__GotoPreset->Speed->Zoom->space != NULL)
						{
							printf("Zoom space %s x %d \r\n",tptz__GotoPreset->Speed->Zoom->space, tptz__GotoPreset->Speed->Zoom->x);
							
							strcpy(oSysInfo->PTZC.CurrentZoom_space, tptz__GotoPreset->Speed->Zoom->space);
							oSysInfo->PTZC.CurrentZoom_x = tptz__GotoPreset->Speed->Zoom->x;
						}
					}
				}
			}
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__GotoPresetResponse);
}

int __tptz__GetStatus(struct soap *soap, 
	struct _tptz__GetStatus *tptz__GetStatus, 
	struct _tptz__GetStatusResponse *tptz__GetStatusResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__GetStatus->ProfileToken!=NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__GetStatus->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
			}
		}
		
	}
	else
	{
		flprintf("Error, ProfileToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) 
	{
		if(tptz__GetStatus->ProfileToken != NULL)
		{
			flprintf("Error, NoSource(%s)\r\n", tptz__GetStatus->ProfileToken);
		}
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	tptz__GetStatusResponse->PTZStatus = (struct tt__PTZStatus *)soap_malloc(soap, sizeof(struct tt__PTZStatus));	
	memset(tptz__GetStatusResponse->PTZStatus, 0x00, sizeof(struct tt__PTZStatus));
		
	tptz__GetStatusResponse->PTZStatus->Position = (struct tt__PTZVector *)soap_malloc(soap, sizeof(struct tt__PTZVector)); 
	memset(tptz__GetStatusResponse->PTZStatus->Position, 0x00, sizeof(struct tt__PTZVector));
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D)); 
	memset(tptz__GetStatusResponse->PTZStatus->Position->PanTilt, 0x00, sizeof(struct tt__Vector2D));
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->space = soap_strdup(soap, oSysInfo->PTZC.CurrentPantilt_space);
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->x = oSysInfo->PTZC.CurrentPantilt_x;
	tptz__GetStatusResponse->PTZStatus->Position->PanTilt->y = oSysInfo->PTZC.CurrentPantilt_y;

	tptz__GetStatusResponse->PTZStatus->Position->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D)); 
	memset(tptz__GetStatusResponse->PTZStatus->Position->Zoom, 0x00, sizeof(struct tt__Vector1D));
	tptz__GetStatusResponse->PTZStatus->Position->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.CurrentZoom_space);
	tptz__GetStatusResponse->PTZStatus->Position->Zoom->x = oSysInfo->PTZC.CurrentZoom_x;

	tptz__GetStatusResponse->PTZStatus->MoveStatus = (struct tt__PTZMoveStatus *)soap_malloc(soap, sizeof(struct tt__PTZMoveStatus)); 
	memset(tptz__GetStatusResponse->PTZStatus->MoveStatus, 0x00, sizeof(struct tt__PTZMoveStatus));
	tptz__GetStatusResponse->PTZStatus->MoveStatus->PanTilt = soap_malloc(soap, sizeof(enum tt__MoveStatus)); 
	*tptz__GetStatusResponse->PTZStatus->MoveStatus->PanTilt = oSysInfo->PTZC.CurrentPanTiltStatus;
	tptz__GetStatusResponse->PTZStatus->MoveStatus->Zoom = soap_malloc(soap, sizeof(enum tt__MoveStatus)); 
	*tptz__GetStatusResponse->PTZStatus->MoveStatus->Zoom = oSysInfo->PTZC.CurrentZoomStatus;

	tptz__GetStatusResponse->PTZStatus->UtcTime = (time_t)gptMsgShare.TIME; 
	tptz__GetStatusResponse->PTZStatus->Error = NULL;
	tptz__GetStatusResponse->PTZStatus->__size = 0; 
	tptz__GetStatusResponse->PTZStatus->__any = NULL;
	tptz__GetStatusResponse->PTZStatus->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

int 
__tptz__GetConfiguration(struct soap *soap, 
	struct _tptz__GetConfiguration *tptz__GetConfiguration, 
	struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;

	if(tptz__GetConfiguration->PTZConfigurationToken!=NULL)
	{		
		if (strcmp(tptz__GetConfiguration->PTZConfigurationToken, oSysInfo->PTZC.PTZtoken) == 0) {
			Ptoken_exist = EXIST;
		}
		
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__GetConfiguration->PTZConfigurationToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	tptz__GetConfigurationResponse->PTZConfiguration = (struct tt__PTZConfiguration *)soap_malloc(soap, sizeof(struct tt__PTZConfiguration));	
		memset(tptz__GetConfigurationResponse->PTZConfiguration, 0x00, sizeof(struct tt__PTZConfiguration));
		
	tptz__GetConfigurationResponse->PTZConfiguration->Name = soap_strdup(soap, oSysInfo->PTZC.PTZname);
	tptz__GetConfigurationResponse->PTZConfiguration->UseCount = oSysInfo->PTZC.PTZusecount;
	tptz__GetConfigurationResponse->PTZConfiguration->token = soap_strdup(soap, oSysInfo->PTZC.PTZtoken);
	tptz__GetConfigurationResponse->PTZConfiguration->NodeToken = soap_strdup(soap, oSysInfo->PTZC.Node[0].Token);
	
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace);
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultAbsoluteZoomPositionSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultAbsoluteZoomPositionSpace);
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultRelativePanTiltTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativePanTiltTranslationSpace);
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultRelativeZoomTranslationSpace = soap_strdup(soap, oSysInfo->PTZC.DefaultRelativeZoomTranslationSpace);
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace);
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultContinuousZoomVelocitySpace = soap_strdup(soap, oSysInfo->PTZC.DefaultContinuousZoomVelocitySpace);
	
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed = (struct tt__PTZSpeed *)soap_malloc(soap, sizeof(struct tt__PTZSpeed));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed, 0x00, sizeof(struct tt__PTZSpeed));
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt = (struct tt__Vector2D *)soap_malloc(soap, sizeof(struct tt__Vector2D));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt, 0x00, sizeof(struct tt__Vector2D));
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->x = oSysInfo->PTZC.DefaultPantilt_x;
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->y = oSysInfo->PTZC.DefaultPantilt_y;
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->space = soap_strdup(soap, oSysInfo->PTZC.DefaultPantilt_space);
	
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->Zoom = (struct tt__Vector1D *)soap_malloc(soap, sizeof(struct tt__Vector1D));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->Zoom, 0x00, sizeof(struct tt__Vector1D));
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->x = oSysInfo->PTZC.DefaultZoom_x;
	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->Zoom->space = soap_strdup(soap, oSysInfo->PTZC.DefaultZoom_space);

	tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZTimeout = (LONG64 *)soap_malloc(soap, sizeof(LONG64));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZTimeout, 0x00, sizeof(LONG64));
	*tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZTimeout =  oSysInfo->PTZC.DefaultPTZTimeout;
	
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits = (struct tt__PanTiltLimits *)soap_malloc(soap, sizeof(struct tt__PanTiltLimits));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits, 0x00, sizeof(struct tt__PanTiltLimits));
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range, 0x00, sizeof(struct tt__Space2DDescription));
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.PanTiltLimit_URI);
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Max = oSysInfo->PTZC.PanTiltLimit_XRange_Max;
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->XRange->Min = oSysInfo->PTZC.PanTiltLimit_XRange_Min;
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->YRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Max	= oSysInfo->PTZC.PanTiltLimit_YRange_Max;
	tptz__GetConfigurationResponse->PTZConfiguration->PanTiltLimits->Range->YRange->Min	= oSysInfo->PTZC.PanTiltLimit_YRange_Min;
		
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits = (struct tt__ZoomLimits *)soap_malloc(soap, sizeof(struct tt__ZoomLimits));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits, 0x00, sizeof(struct tt__ZoomLimits));
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range->URI = soap_strdup(soap, oSysInfo->PTZC.ZoomLimit_XRange_URI);
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range->XRange->Max = oSysInfo->PTZC.ZoomLimit_XRange_Max;
	tptz__GetConfigurationResponse->PTZConfiguration->ZoomLimits->Range->XRange->Min = oSysInfo->PTZC.ZoomLimit_XRange_Min;


	tptz__GetConfigurationResponse->PTZConfiguration->Extension = NULL;
	tptz__GetConfigurationResponse->PTZConfiguration->__anyAttribute = NULL;
		
	LEAVE();
	return SOAP_OK;
}

int __tptz__GetNodes(struct soap *soap, 
	struct _tptz__GetNodes *tptz__GetNodes, 
	struct _tptz__GetNodesResponse *tptz__GetNodesResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;

	tptz__GetNodesResponse->__sizePTZNode = oSysInfo->PTZC.SizeNode;
	
	tptz__GetNodesResponse->PTZNode = (struct tt__PTZNode *)soap_malloc(soap, sizeof(struct tt__PTZNode)*tptz__GetNodesResponse->__sizePTZNode);	
	memset(tptz__GetNodesResponse->PTZNode, 0x00, sizeof(struct tt__PTZNode)*tptz__GetNodesResponse->__sizePTZNode);

	for(i=0;i<tptz__GetNodesResponse->__sizePTZNode;i++)
	{
		tptz__GetNodesResponse->PTZNode[i].Name = soap_strdup(soap, oSysInfo->PTZC.Node[i].Name);
		tptz__GetNodesResponse->PTZNode[i].token = soap_strdup(soap, oSysInfo->PTZC.Node[i].Token);	
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces = (struct tt__PTZSpaces *)soap_malloc(soap, sizeof(struct tt__PTZSpaces));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces, 0x00, sizeof(struct tt__PTZSpaces));
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeAbsolutePanTiltPositionSpace = oSysInfo->PTZC.Node[i].__sizeAbsolutePanTiltPositionSpace; 
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace, 0x00, sizeof(struct tt__Space2DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_URI);
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_YRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_YRange.Min;
			
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace = oSysInfo->PTZC.Node[i].__sizeAbsoluteZoomPositionSpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_URI);
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Max = oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Min = oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeRelativePanTiltTranslationSpace = oSysInfo->PTZC.Node[i].__sizeRelativePanTiltTranslationSpace;	
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace, 0x00, sizeof(struct tt__Space2DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_URI);
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Max = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_YRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Min = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_YRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeRelativeZoomTranslationSpace = oSysInfo->PTZC.Node[i].__sizeRelativeZoomTranslationSpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_URI);
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeContinuousPanTiltVelocitySpace = oSysInfo->PTZC.Node[i].__sizeContinuousPanTiltVelocitySpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace, 0x00, sizeof(struct tt__Space2DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_URI);
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Max = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_YRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Min = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_YRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeContinuousZoomVelocitySpace = oSysInfo->PTZC.Node[i].__sizeContinuousZoomVelocitySpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_URI);
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizePanTiltSpeedSpace = oSysInfo->PTZC.Node[i].__sizePanTiltSpeedSpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_URI);
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_XRange.Min;
		
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__sizeZoomSpeedSpace = oSysInfo->PTZC.Node[i].__sizeZoomSpeedSpace;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ZoomSpeedSpace_URI);

		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[i].ZoomSpeedSpace_XRange.Max;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->ZoomSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[i].ZoomSpeedSpace_XRange.Min;
			
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->Extension = NULL;
		tptz__GetNodesResponse->PTZNode[i].SupportedPTZSpaces->__anyAttribute = NULL;
			
		tptz__GetNodesResponse->PTZNode[i].MaximumNumberOfPresets = oSysInfo->PTZC.Node[i].MaximumNumberOfPresets;
		tptz__GetNodesResponse->PTZNode[i].HomeSupported = oSysInfo->PTZC.Node[i].HomeSupported;
		tptz__GetNodesResponse->PTZNode[i].__sizeAuxiliaryCommands = oSysInfo->PTZC.Node[i].__sizeAuxiliaryCommands;	
		tptz__GetNodesResponse->PTZNode[i].AuxiliaryCommands = NULL;	
		tptz__GetNodesResponse->PTZNode[i].FixedHomePosition = soap_malloc(soap, sizeof(enum xsd__boolean));	
		memset(tptz__GetNodesResponse->PTZNode[i].FixedHomePosition, 0x00, sizeof(enum xsd__boolean));
		*tptz__GetNodesResponse->PTZNode[i].FixedHomePosition = oSysInfo->PTZC.Node[i].FixedHomePosition;
			
		tptz__GetNodesResponse->PTZNode[i].Extension = NULL; 
		tptz__GetNodesResponse->PTZNode[i].__anyAttribute = NULL;	
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__GetNodes);
}

int __tptz__GetNode(struct soap *soap, 
	struct _tptz__GetNode *tptz__GetNode, 
	struct _tptz__GetNodeResponse *tptz__GetNodeResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int node=0;
	int i;

	if(tptz__GetNode->NodeToken !=NULL)
	{		
		for(i=0;i<oSysInfo->PTZC.SizeNode;i++)
		{
			if (strcmp(tptz__GetNode->NodeToken, oSysInfo->PTZC.Node[i].Token) == 0) {
				Ptoken_exist = EXIST;
				node = i;
			}
		}		
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__GetNode->NodeToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	tptz__GetNodeResponse->PTZNode = (struct tt__PTZNode *)soap_malloc(soap, sizeof(struct tt__PTZNode));	
	memset(tptz__GetNodeResponse->PTZNode, 0x00, sizeof(struct tt__PTZNode));
	
	tptz__GetNodeResponse->PTZNode->Name = soap_strdup(soap, oSysInfo->PTZC.Node[node].Name);
	tptz__GetNodeResponse->PTZNode->token = soap_strdup(soap, oSysInfo->PTZC.Node[node].Token);	
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces = (struct tt__PTZSpaces *)soap_malloc(soap, sizeof(struct tt__PTZSpaces));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces, 0x00, sizeof(struct tt__PTZSpaces));
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeAbsolutePanTiltPositionSpace = oSysInfo->PTZC.Node[node].__sizeAbsolutePanTiltPositionSpace; 
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace, 0x00, sizeof(struct tt__Space2DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].AbsolutePanTiltPositionSpace_URI);
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Max = oSysInfo->PTZC.Node[node].AbsolutePanTiltPositionSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->XRange->Min = oSysInfo->PTZC.Node[node].AbsolutePanTiltPositionSpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Max = oSysInfo->PTZC.Node[node].AbsolutePanTiltPositionSpace_YRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsolutePanTiltPositionSpace->YRange->Min = oSysInfo->PTZC.Node[node].AbsolutePanTiltPositionSpace_YRange.Min;
		
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeAbsoluteZoomPositionSpace = oSysInfo->PTZC.Node[node].__sizeAbsoluteZoomPositionSpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].AbsoluteZoomPositionSpace_URI);
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Max = oSysInfo->PTZC.Node[node].AbsoluteZoomPositionSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->AbsoluteZoomPositionSpace->XRange->Min = oSysInfo->PTZC.Node[node].AbsoluteZoomPositionSpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeRelativePanTiltTranslationSpace = oSysInfo->PTZC.Node[node].__sizeRelativePanTiltTranslationSpace;	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace, 0x00, sizeof(struct tt__Space2DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].RelativePanTiltTranslationSpace_URI);
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[node].RelativePanTiltTranslationSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[node].RelativePanTiltTranslationSpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Max = oSysInfo->PTZC.Node[node].RelativePanTiltTranslationSpace_YRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativePanTiltTranslationSpace->YRange->Min = oSysInfo->PTZC.Node[node].RelativePanTiltTranslationSpace_YRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeRelativeZoomTranslationSpace = oSysInfo->PTZC.Node[node].__sizeRelativeZoomTranslationSpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].RelativeZoomTranslationSpace_URI);
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[node].RelativeZoomTranslationSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->RelativeZoomTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[node].RelativeZoomTranslationSpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeContinuousPanTiltVelocitySpace = oSysInfo->PTZC.Node[node].__sizeContinuousPanTiltVelocitySpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace, 0x00, sizeof(struct tt__Space2DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].ContinuousPanTiltVelocitySpace_URI);
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[node].ContinuousPanTiltVelocitySpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[node].ContinuousPanTiltVelocitySpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Max = oSysInfo->PTZC.Node[node].ContinuousPanTiltVelocitySpace_YRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousPanTiltVelocitySpace->YRange->Min = oSysInfo->PTZC.Node[node].ContinuousPanTiltVelocitySpace_YRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeContinuousZoomVelocitySpace = oSysInfo->PTZC.Node[node].__sizeContinuousZoomVelocitySpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].ContinuousZoomVelocitySpace_URI);
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[node].ContinuousZoomVelocitySpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ContinuousZoomVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[node].ContinuousZoomVelocitySpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizePanTiltSpeedSpace = oSysInfo->PTZC.Node[node].__sizePanTiltSpeedSpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].PanTiltSpeedSpace_URI);
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[node].PanTiltSpeedSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->PanTiltSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[node].PanTiltSpeedSpace_XRange.Min;
	
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__sizeZoomSpeedSpace = oSysInfo->PTZC.Node[node].__sizeZoomSpeedSpace;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[node].ZoomSpeedSpace_URI);

	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
	memset(tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[node].ZoomSpeedSpace_XRange.Max;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->ZoomSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[node].ZoomSpeedSpace_XRange.Min;
		
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->Extension = NULL;
	tptz__GetNodeResponse->PTZNode->SupportedPTZSpaces->__anyAttribute = NULL;
		
	tptz__GetNodeResponse->PTZNode->MaximumNumberOfPresets = oSysInfo->PTZC.Node[node].MaximumNumberOfPresets;
	tptz__GetNodeResponse->PTZNode->HomeSupported = oSysInfo->PTZC.Node[node].HomeSupported;
	tptz__GetNodeResponse->PTZNode->__sizeAuxiliaryCommands = oSysInfo->PTZC.Node[node].__sizeAuxiliaryCommands;	
	tptz__GetNodeResponse->PTZNode->AuxiliaryCommands = NULL;	
	tptz__GetNodeResponse->PTZNode->FixedHomePosition = soap_malloc(soap, sizeof(enum xsd__boolean));	
	memset(tptz__GetNodeResponse->PTZNode->FixedHomePosition, 0x00, sizeof(enum xsd__boolean));
	*tptz__GetNodeResponse->PTZNode->FixedHomePosition = oSysInfo->PTZC.Node[node].FixedHomePosition;
		
	tptz__GetNodeResponse->PTZNode->Extension = NULL; 
	tptz__GetNodeResponse->PTZNode->__anyAttribute = NULL;		

	LEAVE();
	return SOAP_OK;
}

int __tptz__SetConfiguration(struct soap *soap, 
	struct _tptz__SetConfiguration *tptz__SetConfiguration, 
	struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int node = 0, i;

	if(tptz__SetConfiguration->PTZConfiguration->NodeToken != NULL)
	{
		for(i=0;i<oSysInfo->PTZC.SizeNode;i++)
		{
			if (strcmp(tptz__SetConfiguration->PTZConfiguration->NodeToken, oSysInfo->PTZC.Node[i].Token) == 0) {
				Ptoken_exist = EXIST;
				node = i;
				break;
			}
		}		
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__SetConfiguration->PTZConfiguration->NodeToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(tptz__SetConfiguration->PTZConfiguration->Name != NULL)
	{
		strcpy(oSysInfo->PTZC.PTZname, tptz__SetConfiguration->PTZConfiguration->Name);
	}
	
	oSysInfo->PTZC.PTZusecount = tptz__SetConfiguration->PTZConfiguration->UseCount;	
	
	if(tptz__SetConfiguration->PTZConfiguration->token != NULL)
	{
		strcpy(oSysInfo->PTZC.PTZtoken,tptz__SetConfiguration->PTZConfiguration->token);
	}
	
	if(tptz__SetConfiguration->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultAbsolutePantTiltPositionSpace,tptz__SetConfiguration->PTZConfiguration->DefaultAbsolutePantTiltPositionSpace);
	}		
	if(tptz__SetConfiguration->PTZConfiguration->DefaultAbsoluteZoomPositionSpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultAbsoluteZoomPositionSpace,tptz__SetConfiguration->PTZConfiguration->DefaultAbsoluteZoomPositionSpace);
	}	
	if(tptz__SetConfiguration->PTZConfiguration->DefaultRelativePanTiltTranslationSpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultRelativePanTiltTranslationSpace,tptz__SetConfiguration->PTZConfiguration->DefaultRelativePanTiltTranslationSpace);
	}	
	if(tptz__SetConfiguration->PTZConfiguration->DefaultRelativeZoomTranslationSpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultRelativeZoomTranslationSpace,tptz__SetConfiguration->PTZConfiguration->DefaultRelativeZoomTranslationSpace);
	}			
	if(tptz__SetConfiguration->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultContinuousPanTiltVelocitySpace,tptz__SetConfiguration->PTZConfiguration->DefaultContinuousPanTiltVelocitySpace);
	}			
	if(tptz__SetConfiguration->PTZConfiguration->DefaultContinuousZoomVelocitySpace != NULL)
	{
		strcpy(oSysInfo->PTZC.DefaultContinuousZoomVelocitySpace, tptz__SetConfiguration->PTZConfiguration->DefaultContinuousZoomVelocitySpace);
	}

	if(tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed != NULL)
	{
		if(tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt != NULL)
		{
			strcpy(oSysInfo->PTZC.DefaultPantilt_space,  tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt->space);
			oSysInfo->PTZC.DefaultPantilt_x = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt->x;
			oSysInfo->PTZC.DefaultPantilt_y = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->PanTilt->y;
		}
		
		if(tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->Zoom != NULL)
		{
			strcpy(oSysInfo->PTZC.DefaultZoom_space, tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->Zoom->space);
			oSysInfo->PTZC.DefaultZoom_x = tptz__SetConfiguration->PTZConfiguration->DefaultPTZSpeed->Zoom->x;
		}
	}

	if(tptz__SetConfiguration->PTZConfiguration->DefaultPTZTimeout != NULL)
	{
		oSysInfo->PTZC.DefaultPTZTimeout = *tptz__SetConfiguration->PTZConfiguration->DefaultPTZTimeout;
	}

	
	if(tptz__SetConfiguration->PTZConfiguration->PanTiltLimits != NULL)
	{
		if(tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range!= NULL)
		{
			if(tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range->URI != NULL)
			{
				strcpy(oSysInfo->PTZC.PanTiltLimit_URI, tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range->URI);
			}
			if(tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range->XRange != NULL)
			{
				oSysInfo->PTZC.PanTiltLimit_XRange_Max = tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range->XRange->Max;
				oSysInfo->PTZC.PanTiltLimit_XRange_Min = tptz__SetConfiguration->PTZConfiguration->PanTiltLimits->Range->XRange->Min;
			}				
		}
	}

	if(tptz__SetConfiguration->PTZConfiguration->ZoomLimits != NULL)
	{
		if(tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range != NULL)
		{
			if(tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range->URI != NULL)
			{
				strcpy(oSysInfo->PTZC.ZoomLimit_XRange_URI, tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range->URI);
			}	
			if(tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range->XRange != NULL)
			{
				oSysInfo->PTZC.ZoomLimit_XRange_Max = tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range->XRange->Max;
				oSysInfo->PTZC.ZoomLimit_XRange_Min = tptz__SetConfiguration->PTZConfiguration->ZoomLimits->Range->XRange->Min;
			}
		}
	}
	
	if(tptz__SetConfiguration->ForcePersistence)
	{
		printf("save the configuration\r\n");
		//
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__SetConfigurationResponse);
}

int __tptz__GetConfigurationOptions(struct soap *soap, 
	struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, 
	struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__GetConfigurationOptions->ConfigurationToken!= NULL)
	{
		if (strcmp(tptz__GetConfigurationOptions->ConfigurationToken, oSysInfo->PTZC.PTZtoken) == 0) {
			Ptoken_exist = EXIST;
		}
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__GetConfigurationOptions->ConfigurationToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions = (struct tt__PTZConfigurationOptions *)soap_malloc(soap, sizeof(struct tt__PTZConfigurationOptions));	
	memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions, 0x00, sizeof(struct tt__PTZConfigurationOptions));
		
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces = (struct tt__PTZSpaces *)soap_malloc(soap, sizeof(struct tt__PTZSpaces));	
	memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces, 0x00, sizeof(struct tt__PTZSpaces));
	
	if(oSysInfo->PTZC.SizeNode > 0)
	{
		i=0;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeAbsolutePanTiltPositionSpace = 1;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace, 0x00, sizeof(struct tt__Space2DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->XRange->Max = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->XRange->Min = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_XRange.Min;		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->YRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->YRange->Max = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_YRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsolutePanTiltPositionSpace->YRange->Min = oSysInfo->PTZC.Node[i].AbsolutePanTiltPositionSpace_YRange.Min;

		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeAbsoluteZoomPositionSpace = 1;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace, 0x00, sizeof(struct tt__Space1DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace->XRange->Max = oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->AbsoluteZoomPositionSpace->XRange->Min = oSysInfo->PTZC.Node[i].AbsoluteZoomPositionSpace_XRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeRelativePanTiltTranslationSpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace, 0x00, sizeof(struct tt__Space2DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_XRange.Min;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->YRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->YRange->Max = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_YRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativePanTiltTranslationSpace->YRange->Min = oSysInfo->PTZC.Node[i].RelativePanTiltTranslationSpace_YRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeRelativeZoomTranslationSpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace, 0x00, sizeof(struct tt__Space1DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace->XRange->Max = oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->RelativeZoomTranslationSpace->XRange->Min = oSysInfo->PTZC.Node[i].RelativeZoomTranslationSpace_XRange.Min;
			
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeContinuousPanTiltVelocitySpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)soap_malloc(soap, sizeof(struct tt__Space2DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace, 0x00, sizeof(struct tt__Space2DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_XRange.Min;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->YRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->YRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->YRange->Max = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_YRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousPanTiltVelocitySpace->YRange->Min = oSysInfo->PTZC.Node[i].ContinuousPanTiltVelocitySpace_YRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeContinuousZoomVelocitySpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace, 0x00, sizeof(struct tt__Space1DDescription));
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace->XRange->Max = oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ContinuousZoomVelocitySpace->XRange->Min = oSysInfo->PTZC.Node[i].ContinuousZoomVelocitySpace_XRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizePanTiltSpeedSpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->PanTiltSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[i].PanTiltSpeedSpace_XRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__sizeZoomSpeedSpace++;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace = (struct tt__Space1DDescription *)soap_malloc(soap, sizeof(struct tt__Space1DDescription));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace, 0x00, sizeof(struct tt__Space1DDescription));	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace->URI = soap_strdup(soap, oSysInfo->PTZC.Node[i].ZoomSpeedSpace_URI);
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace->XRange = (struct tt__FloatRange *)soap_malloc(soap, sizeof(struct tt__FloatRange));	
		memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace->XRange, 0x00, sizeof(struct tt__FloatRange));
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace->XRange->Max = oSysInfo->PTZC.Node[i].ZoomSpeedSpace_XRange.Max;
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->ZoomSpeedSpace->XRange->Min = oSysInfo->PTZC.Node[i].ZoomSpeedSpace_XRange.Min;
		
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->Extension = NULL;	
		tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Spaces->__anyAttribute = NULL;	
	}
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout = (struct tt__DurationRange *)soap_malloc(soap, sizeof(struct tt__DurationRange));
	memset(tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout, 0x00, sizeof(struct tt__DurationRange));
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout->Max = oSysInfo->PTZC.MaxPTZTimeout;
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTZTimeout->Min = oSysInfo->PTZC.MinPTZTimeout;

	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->__size = 0;
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->__any = NULL;
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->PTControlDirection = NULL;
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->Extension = NULL;
	tptz__GetConfigurationOptionsResponse->PTZConfigurationOptions->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;
}

int __tptz__GotoHomePosition(struct soap *soap, 
	struct _tptz__GotoHomePosition *tptz__GotoHomePosition, 
	struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse)
{
#ifdef ONVIF_PTZ_KEY
	gptMsgISPTbl.ISP_UKEY = 0x10; // 2019-3-8 KDH
#endif

	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__GotoHomePosition);
	UNUSED(tptz__GotoHomePositionResponse);
}

int 
__tptz__SetHomePosition(struct soap *soap, 
	struct _tptz__SetHomePosition *tptz__SetHomePosition, 
	struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__SetHomePosition);
	UNUSED(tptz__SetHomePositionResponse);
}

int 
__tptz__ContinuousMove(struct soap *soap, 
	struct _tptz__ContinuousMove *tptz__ContinuousMove, 
	struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__ContinuousMove->ProfileToken!= NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__ContinuousMove->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__ContinuousMove->ProfileToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(tptz__ContinuousMove->Velocity != NULL)
	{
		printf("velocity \r\n");
		if(tptz__ContinuousMove->Velocity->PanTilt != NULL)		
		{
			printf("pantilt \r\n");
			if(tptz__ContinuousMove->Velocity->PanTilt->space != NULL)		
			{
				printf("pantilt space %s x %d y %d\r\n", tptz__ContinuousMove->Velocity->PanTilt->space, oSysInfo->PTZC.CurrentPantilt_x, oSysInfo->PTZC.CurrentPantilt_y);
				strcpy(oSysInfo->PTZC.CurrentPantilt_space, tptz__ContinuousMove->Velocity->PanTilt->space);
				oSysInfo->PTZC.CurrentPantilt_x = tptz__ContinuousMove->Velocity->PanTilt->x;
				oSysInfo->PTZC.CurrentPantilt_y = tptz__ContinuousMove->Velocity->PanTilt->y;
			}
			else
			{
				printf("pantilt x %d y %d\r\n", oSysInfo->PTZC.CurrentPantilt_x, oSysInfo->PTZC.CurrentPantilt_y);
				oSysInfo->PTZC.CurrentPantilt_x = tptz__ContinuousMove->Velocity->PanTilt->x;
				oSysInfo->PTZC.CurrentPantilt_y = tptz__ContinuousMove->Velocity->PanTilt->y;
			}
#ifdef ONVIF_PTZ_KEY
            if     (tptz__ContinuousMove->Velocity->PanTilt->y > 0) gptMsgISPTbl.ISP_UKEY = 0x01; // up
            else if(tptz__ContinuousMove->Velocity->PanTilt->y < 0) gptMsgISPTbl.ISP_UKEY = 0x02; // down
            else if(tptz__ContinuousMove->Velocity->PanTilt->x < 0) gptMsgISPTbl.ISP_UKEY = 0x08; // left
            else if(tptz__ContinuousMove->Velocity->PanTilt->x > 0) gptMsgISPTbl.ISP_UKEY = 0x04; // right
#endif 
		}	
		if(tptz__ContinuousMove->Velocity->Zoom != NULL)		
		{		
			printf("Zoom \r\n");
			if(tptz__ContinuousMove->Velocity->Zoom->space != NULL)		
			{
				printf("Zoom space %s x %d\r\n", tptz__ContinuousMove->Velocity->Zoom->space, tptz__ContinuousMove->Velocity->Zoom->x);
				
				strcpy(oSysInfo->PTZC.CurrentZoom_space, tptz__ContinuousMove->Velocity->Zoom->space);
//				oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
				if(0 == tptz__ContinuousMove->Velocity->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x00", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x0;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
				}
				else if(oSysInfo->PTZC.CurrentZoom_x < tptz__ContinuousMove->Velocity->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x01;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
					oSysInfo->PTZC.CurrentZoom_x -= 0.1f;
				}
				else
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x02", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x02;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
					oSysInfo->PTZC.CurrentZoom_x += 0.1f;
				}
			}
			else
			{				
//				oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;

				if(0 == tptz__ContinuousMove->Velocity->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x00", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x0;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
				}	
				else if(oSysInfo->PTZC.CurrentZoom_x < tptz__ContinuousMove->Velocity->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x01;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
					oSysInfo->PTZC.CurrentZoom_x -= 0.1f;
				}
				else
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x02", oSysInfo->PTZC.CurrentZoom_x, tptz__ContinuousMove->Velocity->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = 0x02;
					oSysInfo->PTZC.CurrentZoom_x = tptz__ContinuousMove->Velocity->Zoom->x;
					oSysInfo->PTZC.CurrentZoom_x += 0.1f;
				}
			}
		}
	}
	//tptz__ContinuousMove->Timeout;

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__ContinuousMoveResponse);
}

int __tptz__RelativeMove(struct soap *soap, 
	struct _tptz__RelativeMove *tptz__RelativeMove, 
	struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__RelativeMove->ProfileToken!= NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__RelativeMove->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__RelativeMove->ProfileToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(tptz__RelativeMove->Translation != NULL)
	{
		if(tptz__RelativeMove->Translation->PanTilt != NULL)
		{
			if(tptz__RelativeMove->Translation->PanTilt->space != NULL)
			{
				strcpy(oSysInfo->PTZC.CurrentTransPantilt_space, tptz__RelativeMove->Translation->PanTilt->space);
				oSysInfo->PTZC.CurrentTransPantilt_x = tptz__RelativeMove->Translation->PanTilt->x;
				oSysInfo->PTZC.CurrentTransPantilt_y = tptz__RelativeMove->Translation->PanTilt->y;
			}
		}

		if(tptz__RelativeMove->Translation->Zoom != NULL)
		{
			if(tptz__RelativeMove->Translation->Zoom->space != NULL)
			{
				strcpy(oSysInfo->PTZC.CurrentTransZoom_space, tptz__RelativeMove->Translation->Zoom->space);
				if(oSysInfo->PTZC.CurrentZoom_x != tptz__RelativeMove->Translation->Zoom->x)
				{
					printf("Translation Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__RelativeMove->Translation->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = (UINT)(tptz__RelativeMove->Translation->Zoom->x*100.f);
					gptMsgISPTbl.ISP_ZOOM_UKEY != 0x80;
				}
			}
			else
			{
				if(oSysInfo->PTZC.CurrentZoom_x != tptz__RelativeMove->Translation->Zoom->x)
				{
					printf("Translation Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__RelativeMove->Translation->Zoom->x);
					gptMsgISPTbl.ISP_ZOOM_UKEY = (UINT)(tptz__RelativeMove->Translation->Zoom->x*100.f);
					gptMsgISPTbl.ISP_ZOOM_UKEY != 0x80;
				}		
			}
		}
	}

	if(tptz__RelativeMove->Speed != NULL)
	{
		if(tptz__RelativeMove->Speed->PanTilt != NULL)
		{
			if(tptz__RelativeMove->Speed->PanTilt->space != NULL)
			{
				strcpy(oSysInfo->PTZC.CurrentSpeedPantilt_space, tptz__RelativeMove->Speed->PanTilt->space);
				oSysInfo->PTZC.CurrentSpeedPantilt_x = tptz__RelativeMove->Speed->PanTilt->x;
				oSysInfo->PTZC.CurrentSpeedPantilt_y = tptz__RelativeMove->Speed->PanTilt->y;
			}
			else
			{
				oSysInfo->PTZC.CurrentSpeedPantilt_x = tptz__RelativeMove->Speed->PanTilt->x;
				oSysInfo->PTZC.CurrentSpeedPantilt_y = tptz__RelativeMove->Speed->PanTilt->y;
			}
		}

		if(tptz__RelativeMove->Speed->Zoom != NULL)
		{
			if(tptz__RelativeMove->Speed->Zoom->space != NULL)
			{
				strcpy(oSysInfo->PTZC.CurrentSpeedZoom_space, tptz__RelativeMove->Speed->Zoom->space);
				
				printf("Zoom Speed x %f / %f 0x02", oSysInfo->PTZC.CurrentZoom_x, tptz__RelativeMove->Speed->Zoom->x);
				oSysInfo->PTZC.CurrentSpeedZoom_x = tptz__RelativeMove->Speed->Zoom->x;
			}
			else
			{
				printf("Zoom Speed %f / %f 0x02", oSysInfo->PTZC.CurrentSpeedZoom_x, tptz__RelativeMove->Speed->Zoom->x);
				oSysInfo->PTZC.CurrentSpeedZoom_x = tptz__RelativeMove->Speed->Zoom->x;
			}
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__RelativeMoveResponse);
}

int 
__tptz__SendAuxiliaryCommand(struct soap *soap, 
	struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, 
	struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__SendAuxiliaryCommand);
	UNUSED(tptz__SendAuxiliaryCommandResponse);
}

int 
__tptz__AbsoluteMove(struct soap *soap, 
	struct _tptz__AbsoluteMove *tptz__AbsoluteMove, 
	struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__AbsoluteMove->ProfileToken!= NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__AbsoluteMove->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__AbsoluteMove->ProfileToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(tptz__AbsoluteMove->Position != NULL)
	{
		if(tptz__AbsoluteMove->Position->PanTilt != NULL)
		{
			if(tptz__AbsoluteMove->Position->PanTilt->space != NULL)
			{
				if(tptz__AbsoluteMove->Position->PanTilt->x < oSysInfo->PTZC.PanTiltLimit_XRange_Min 
					|| tptz__AbsoluteMove->Position->PanTilt->x > oSysInfo->PTZC.PanTiltLimit_XRange_Max)	
				{
					flprintf("Error, InvalidPosition x (%d)\r\n", tptz__AbsoluteMove->Position->PanTilt->x);
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}
				
				if(tptz__AbsoluteMove->Position->PanTilt->y < oSysInfo->PTZC.PanTiltLimit_YRange_Min 
					|| tptz__AbsoluteMove->Position->PanTilt->y > oSysInfo->PTZC.PanTiltLimit_YRange_Max)	
				{
					flprintf("Error, InvalidPosition y(%d)\r\n", tptz__AbsoluteMove->Position->PanTilt->y);
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}
				
				strcpy(oSysInfo->PTZC.CurrentPantilt_space, tptz__AbsoluteMove->Position->PanTilt->space);
				oSysInfo->PTZC.CurrentPantilt_x = tptz__AbsoluteMove->Position->PanTilt->x;
				oSysInfo->PTZC.CurrentPantilt_y = tptz__AbsoluteMove->Position->PanTilt->y;
			}
		}

		if(tptz__AbsoluteMove->Position->Zoom != NULL)
		{
			if(tptz__AbsoluteMove->Position->Zoom->space != NULL)
			{
				if(tptz__AbsoluteMove->Position->Zoom->x < oSysInfo->PTZC.ZoomLimit_XRange_Min
					|| tptz__AbsoluteMove->Position->Zoom->x > oSysInfo->PTZC.ZoomLimit_XRange_Max)	
				{
					flprintf("Error, InvalidPosition y(%d)\r\n", tptz__AbsoluteMove->Position->PanTilt->y);
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}
				strcpy(oSysInfo->PTZC.CurrentZoom_space, tptz__AbsoluteMove->Position->Zoom->space);
				
				if(oSysInfo->PTZC.CurrentZoom_x != tptz__AbsoluteMove->Position->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__AbsoluteMove->Position->Zoom->x);

					oSysInfo->PTZC.CurrentZoom_x = tptz__AbsoluteMove->Position->Zoom->x;
					gptMsgISPTbl.ISP_ZOOM_UKEY = (UINT)(oSysInfo->PTZC.CurrentZoom_x*100.f);
					gptMsgISPTbl.ISP_ZOOM_UKEY |= 0x00000080;
				}
			}
			else
			{
				if(oSysInfo->PTZC.CurrentZoom_x != tptz__AbsoluteMove->Position->Zoom->x)
				{
					printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x01", oSysInfo->PTZC.CurrentZoom_x, tptz__AbsoluteMove->Position->Zoom->x);

					oSysInfo->PTZC.CurrentZoom_x = tptz__AbsoluteMove->Position->Zoom->x;
					gptMsgISPTbl.ISP_ZOOM_UKEY = (UINT)(oSysInfo->PTZC.CurrentZoom_x*100.f);
					gptMsgISPTbl.ISP_ZOOM_UKEY |= 0x00000080;
				}	
			}
		}
	}
	
	if(tptz__AbsoluteMove->Speed != NULL)
	{
		if(tptz__AbsoluteMove->Speed->PanTilt != NULL)
		{
			if(tptz__AbsoluteMove->Speed->PanTilt->space != NULL)
			{
				if(tptz__AbsoluteMove->Speed->PanTilt->x < oSysInfo->PTZC.PanTiltLimit_XRange_Min 
					|| tptz__AbsoluteMove->Speed->PanTilt->x > oSysInfo->PTZC.PanTiltLimit_XRange_Max)	
				{
					flprintf("Error, InvalidPosition x (%d)\r\n", tptz__AbsoluteMove->Speed->PanTilt->x);
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}
				
				if(tptz__AbsoluteMove->Speed->PanTilt->y < oSysInfo->PTZC.PanTiltLimit_YRange_Min 
					|| tptz__AbsoluteMove->Speed->PanTilt->y > oSysInfo->PTZC.PanTiltLimit_YRange_Max)	
				{
					flprintf("Error, InvalidPosition y(%d)\r\n", tptz__AbsoluteMove->Speed->PanTilt->y );
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}				
				strcpy(oSysInfo->PTZC.CurrentSpeedPantilt_space, tptz__AbsoluteMove->Speed->PanTilt->space);
				oSysInfo->PTZC.CurrentSpeedPantilt_x = tptz__AbsoluteMove->Speed->PanTilt->x;
				oSysInfo->PTZC.CurrentSpeedPantilt_y = tptz__AbsoluteMove->Speed->PanTilt->y;
			}
			else
			{
				if(tptz__AbsoluteMove->Speed->PanTilt->x < oSysInfo->PTZC.PanTiltLimit_XRange_Min 
					|| tptz__AbsoluteMove->Speed->PanTilt->x > oSysInfo->PTZC.PanTiltLimit_XRange_Max)	
				{
					flprintf("Error, InvalidPosition x (%d)\r\n", tptz__AbsoluteMove->Speed->PanTilt->x);
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}
				
				if(tptz__AbsoluteMove->Speed->PanTilt->y < oSysInfo->PTZC.PanTiltLimit_YRange_Min 
					|| tptz__AbsoluteMove->Speed->PanTilt->y > oSysInfo->PTZC.PanTiltLimit_YRange_Max)	
				{
					flprintf("Error, InvalidPosition y(%d)\r\n", tptz__AbsoluteMove->Speed->PanTilt->y );
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}				
				strcpy(oSysInfo->PTZC.CurrentSpeedPantilt_space, tptz__AbsoluteMove->Speed->PanTilt->space);
				oSysInfo->PTZC.CurrentSpeedPantilt_x = tptz__AbsoluteMove->Speed->PanTilt->x;
				oSysInfo->PTZC.CurrentSpeedPantilt_y = tptz__AbsoluteMove->Speed->PanTilt->y;

			}
		}
		
		if(tptz__AbsoluteMove->Speed->Zoom != NULL)
		{
			if(tptz__AbsoluteMove->Speed->Zoom->space != NULL)
			{
				if(tptz__AbsoluteMove->Speed->Zoom->x < oSysInfo->PTZC.ZoomLimit_XRange_Min
					|| tptz__AbsoluteMove->Speed->Zoom->x > oSysInfo->PTZC.ZoomLimit_XRange_Max)	
				{
					flprintf("Error, InvalidPosition x(%d)\r\n", tptz__AbsoluteMove->Speed->Zoom->x );
					onvifInterfaceFault(soap, ovfSC_PTZInvalidPosition, "");
					LEAVE();
					return SOAP_FAULT;				
				}			
				strcpy(oSysInfo->PTZC.CurrentSpeedZoom_space, tptz__AbsoluteMove->Speed->Zoom->space);
				oSysInfo->PTZC.CurrentSpeedZoom_x = tptz__AbsoluteMove->Speed->Zoom->x;
			}
			else
			{
				printf("tptz__ContinuousMove->Velocity->Zoom->x %f / %f 0x02", oSysInfo->PTZC.CurrentSpeedZoom_x, tptz__AbsoluteMove->Speed->Zoom->x );
				oSysInfo->PTZC.CurrentSpeedZoom_x = tptz__AbsoluteMove->Speed->Zoom->x ;
			}
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__AbsoluteMoveResponse);
}

int __tptz__Stop(struct soap *soap, 
	struct _tptz__Stop *tptz__Stop, 
	struct _tptz__StopResponse *tptz__StopResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int Ptoken_exist = NOT_EXIST;
	int i;

	if(tptz__Stop->ProfileToken!= NULL)
	{
		for(i=0;i<oSysInfo->nProfileSize;i++)
		{
			if (strcmp(tptz__Stop->ProfileToken, oSysInfo->Oprofile[i].profiletoken) == 0) {
				Ptoken_exist = EXIST;
				break;
			}
		}
	}
	else
	{
		flprintf("Error, PTZConfigurationToken NULL \r\n");
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}
	
	if(Ptoken_exist == NOT_EXIST) {
		flprintf("Error, NoSource(%s)\r\n", tptz__Stop->ProfileToken);
		onvifInterfaceFault(soap, ovfSC_PTZNoEntity, "");
		LEAVE();
		return SOAP_FAULT;
	}

	if(tptz__Stop->PanTilt != NULL)
	{
		printf(" stop Pantilt %d \r\n", *tptz__Stop->PanTilt);
	}
	if(tptz__Stop->Zoom != NULL)
	{
		printf(" stop zoom %d \r\n", *tptz__Stop->Zoom);
		gptMsgISPTbl.ISP_ZOOM_UKEY = 0x0;
	}
	
	//tptz__Stop->PanTilt;
	//tptz__Stop->Zoom;

	LEAVE();
	return SOAP_OK;

	UNUSED(tptz__StopResponse);	
}

int 
__tptz__GetPresetTours(struct soap *soap, 
	struct _tptz__GetPresetTours *tptz__GetPresetTours, 
	struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__GetPresetTours);
	UNUSED(tptz__GetPresetToursResponse);
}

int 
__tptz__GetPresetTour(struct soap *soap, 
	struct _tptz__GetPresetTour *tptz__GetPresetTour, 
	struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__GetPresetTour);
	UNUSED(tptz__GetPresetTourResponse);
}

int 
__tptz__GetPresetTourOptions(struct soap *soap, 
	struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, 
	struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__GetPresetTourOptions);
	UNUSED(tptz__GetPresetTourOptionsResponse);
}

int 
__tptz__CreatePresetTour(struct soap *soap, 
	struct _tptz__CreatePresetTour *tptz__CreatePresetTour, 
	struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__CreatePresetTour);
	UNUSED(tptz__CreatePresetTourResponse);
}

int 
__tptz__ModifyPresetTour(struct soap *soap, 
	struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, 
	struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__ModifyPresetTour);
	UNUSED(tptz__ModifyPresetTourResponse);
}

int 
__tptz__OperatePresetTour(struct soap *soap, 
	struct _tptz__OperatePresetTour *tptz__OperatePresetTour, 
	struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__OperatePresetTour);
	UNUSED(tptz__OperatePresetTourResponse);
}

int 
__tptz__RemovePresetTour(struct soap *soap, 
	struct _tptz__RemovePresetTour *tptz__RemovePresetTour, 
	struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__RemovePresetTour);
	UNUSED(tptz__RemovePresetTourResponse);
}

int 
__tptz__GetCompatibleConfigurations(struct soap *soap, 
	struct _tptz__GetCompatibleConfigurations *tptz__GetCompatibleConfigurations, 
	struct _tptz__GetCompatibleConfigurationsResponse *tptz__GetCompatibleConfigurationsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tptz__GetCompatibleConfigurations);
	UNUSED(tptz__GetCompatibleConfigurationsResponse);
}

