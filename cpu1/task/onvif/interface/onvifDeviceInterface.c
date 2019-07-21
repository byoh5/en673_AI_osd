
/*Onvif Core Spec.Ver 2.5*/


#include "soapH.h"  
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "FreeRTOS.h" 
#include <stdio.h>
#include "onvifProfile.h"
#include "time.h"
#include "msg.h"
#include "timer_task.h"
#include "glb_tz_list.nsmap"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

void hello_discovery(void *pvParameters);
void bye_discovery(void);


/**
 * @brief Get Services 
 * @This message contains a request for GetServices.
 *   
 * @param Request	: _tds__GetServices structure contains a request for GetServices. 
			  Returns a collection of device services and their available capabilities 			 
 * @param Response	: _tds__GetServicesResponse contains the requested device capabilities.
 * @return               : On success - SOAP_OK.
 */
int __tds__GetServices(struct soap *soap, 
                       struct _tds__GetServices *tds__GetServices,
                       struct _tds__GetServicesResponse *tds__GetServicesResponse)
{
	ENTER();

	int i;
	BOOL isCap;

	struct _tds__GetServicesResponse *pSvcRp;
	
	char _IPAddr[IP_INFO_LENGTH];
	char *ipV4addr = getIpAddress(HTTP_ONVIF_PORT);

	isCap = tds__GetServices->IncludeCapability;
	pSvcRp = (struct _tds__GetServicesResponse *)tds__GetServicesResponse;

	dvprintf("%s  isCapability %d\r\n",__func__, isCap);	

    if(isCap == FALSE)
    {
		/* size */
		pSvcRp->__sizeService = ONVIF_SERVICE_SIZE_MAX;
		
		/*SERVICE ALLOC*/
    	pSvcRp->Service = (struct tds__Service *)soap_malloc(soap, pSvcRp->__sizeService * sizeof(struct tds__Service));
		memset(pSvcRp->Service, 0x00, pSvcRp->__sizeService * sizeof(struct tds__Service));
		
		for(i=0; i<pSvcRp->__sizeService; i++)
		{
			sprintf(_IPAddr, onvif_svc_address[i] , ipV4addr);
		    pSvcRp->Service[i].XAddr 			= soap_strdup(soap, _IPAddr);		
	        pSvcRp->Service[i].Namespace 		= soap_strdup(soap, onvif_svc_namespace[i]);		
	        pSvcRp->Service[i].Version 			= (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion));
			pSvcRp->Service[i].Version->Major 	= onvif_svc_version_major[i];
	        pSvcRp->Service[i].Version->Minor 	= onvif_svc_version_minor[i];
			pSvcRp->Service[i].Capabilities 	= NULL;
			pSvcRp->Service[i].__size 			= 0;
			pSvcRp->Service[i].__any 			= NULL;  
	        pSvcRp->Service[i].__anyAttribute 	= NULL;
			memset(pSvcRp->Service[i].Version, 0x00, sizeof(struct tt__OnvifVersion));
		}
    }
    else
    {
		/* size */
		pSvcRp->__sizeService = ONVIF_SERVICE_SIZE_MAX;

		/*SERVICE ALLOC*/
    	pSvcRp->Service = (struct tds__Service *)soap_malloc(soap, pSvcRp->__sizeService * sizeof(struct tds__Service));
		memset(pSvcRp->Service, 0x00, pSvcRp->__sizeService * sizeof(struct tds__Service));

		for(i=0; i<pSvcRp->__sizeService; i++)
		{
			sprintf(_IPAddr, onvif_svc_address[i] , ipV4addr);
		    pSvcRp->Service[i].XAddr 			= soap_strdup(soap, _IPAddr);
	        pSvcRp->Service[i].Namespace 		= soap_strdup(soap, onvif_svc_namespace[i]);
	        pSvcRp->Service[i].Version 			= (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion));
			memset(pSvcRp->Service[i].Version, 0x00, sizeof(struct tt__OnvifVersion));
	        pSvcRp->Service[i].Version->Major 	= onvif_svc_version_major[i];
	        pSvcRp->Service[i].Version->Minor 	= onvif_svc_version_minor[i];
	        pSvcRp->Service[i].Capabilities 	= (struct _tds__Service_Capabilities *) soap_malloc(soap, sizeof(struct _tds__Service_Capabilities));
			memset(pSvcRp->Service[i].Capabilities, 0x00, sizeof(struct _tds__Service_Capabilities));

			if(strcmp(DEVICE_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap, "<tds:Capabilities>" \
					"<tds:Network IPFilter=\"false\" ZeroConfiguration=\"false\" IPVersion6=\"false\"" \
					" DynDNS=\"true\" Dot11Configuration=\"false\" Dot1XConfigurations=\"0\" HostnameFromDHCP=\"true\"" \
					" NTP=\"0\" DHCPv6=\"false\"></tds:Network>" \
					"<tds:Security TLS1.0=\"false\" TLS1.1=\"false\" TLS1.2=\"false\" OnboardKeyGeneration=\"false\"" \
					" AccessPolicyConfig=\"true\" DefaultAccessPolicy=\"true\" Dot1X=\"false\" RemoteUserHandling=\"false\""\
					" X.509Token=\"false\" SAMLToken=\"false\" KerberosToken=\"false\" UsernameToken=\"true\" HttpDigest=\"true\""\
					" RELToken=\"false\" SupportedEAPMethods=\"0\" MaxUsers=\"5\"></tds:Security>"\
					"<tds:System DiscoveryResolve=\"false\" DiscoveryBye=\"true\" RemoteDiscovery=\"false\" SystemBackup=\"true\"" \
					" SystemLogging=\"false\" FirmwareUpgrade=\"true\" HttpFirmwareUpgrade=\"true\" HttpSystemBackup=\"false\"" \
					" HttpSystemLogging=\"false\" HttpSupportInformation=\"false\"></tds:System>" \
					"</tds:Capabilities>");
			}
			else if(strcmp(MEDIA_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap,
					"<trt:Capabilities SnapshotUri=\"true\" Rotation=\"false\""\
					" OSD=\"false\"><trt:ProfileCapabilities MaximumNumberOfProfiles=\"5\"></trt:ProfileCapabilities>"\
					"<trt:StreamingCapabilities RTPMulticast=\"false\" RTP_TCP=\"true\" RTP_RTSP_TCP=\"true\" NonAggregateControl=\"false\">"\
					"</trt:StreamingCapabilities></trt:Capabilities>");
			}
			else if(strcmp(EVENT_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap,
					"<tev:Capabilities WSSubscriptionPolicySupport=\"true\""\
					" WSPullPointSupport=\"true\" WSPausableSubscriptionManagerInterfaceSupport"\
					"=\"false\" MaxNotificationProducers=\"1\" MaxPullPoints=\"1\"></tev:Capabilities>");
			}
			else if(strcmp(IMAGING_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any =soap_strdup(soap, 
					"<timg:Capabilities ImageStabilization=\"false\"></timg:Capabilities>");
			}
			else if(strcmp(DEVICEIO_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				char strBuffer[256] = {0};
				snprintf(strBuffer, 255, "<tmd:Capabilities VideoSources=\"1\" VideoOutputs=\"0\" "\
					"AudioSources=\"%d\" AudioOutputs=\"%d\" RelayOutputs=\"0\" DigitalInputs=\"0\" "\
					"SerialPorts=\"0\"></tmd:Capabilities>", 
#ifdef __AUDIO__
#if (AUDIO_RX==1)
					1, 1);
#else
					1, 0);
#endif
#else
					0, 0);
#endif
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap, strBuffer);
			}
			else if(strcmp(ANALYTICS_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap, 
					"<tan:Capabilities RuleSupport=\"true\" AnalyticsModuleSupport=\"true\""\
					" CellBasedSceneDescriptionSupported=\"true\"/>");
			}
			else if(strcmp(PANTILT_SERVICE_URL, pSvcRp->Service[i].Namespace) == 0)
			{
				pSvcRp->Service[i].Capabilities->__any = soap_strdup(soap, 
					"<tptz:Capabilities EFlip=\"false\" Reverse=\"false\""\
					" GetCompatibleConfigurations=\"true\"/>");
			}

			pSvcRp->Service[i].__size 			= 0;
			pSvcRp->Service[i].__any 			= NULL;  
	        pSvcRp->Service[i].__anyAttribute 	= NULL;
		}

    }

	LEAVE();
	return SOAP_OK;
}


/**
 * @brief Get ServiceCapabilities 
 * @This message contains a request for GetServiceCapabilities.
 *   
 * @param Request	: tds__GetServiceCapabilities structure contains a request for GetServiceCapabilities. 
 * @param Response	: tds__GetServiceCapabilitiesResponse contains the requested device capabilities.
 * @return               : On success - SOAP_OK.
 */
int __tds__GetServiceCapabilities(struct soap *soap, 
	struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities,
	struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse)
{
	ENTER();

	int szCap;	
	struct _tds__GetServiceCapabilitiesResponse *pSvcRp;

	pSvcRp = (struct _tds__GetServiceCapabilitiesResponse *)tds__GetServiceCapabilitiesResponse;
	szCap = sizeof(struct tds__DeviceServiceCapabilities);
	
	pSvcRp->Capabilities = (struct tds__DeviceServiceCapabilities *)soap_malloc(soap, szCap);
	memset(pSvcRp->Capabilities, 0x00, szCap);
	
	/* NETWORK */
	szCap = sizeof(struct tds__NetworkCapabilities);
	pSvcRp->Capabilities->Network = (struct tds__NetworkCapabilities *)soap_malloc(soap, szCap);
	memset(pSvcRp->Capabilities->Network, 0x00, szCap);	
	
	pSvcRp->Capabilities->Network->IPFilter 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->IPFilter[0]				= xsd__boolean__false_;

	pSvcRp->Capabilities->Network->ZeroConfiguration 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->ZeroConfiguration[0] 	= xsd__boolean__false_;

	pSvcRp->Capabilities->Network->IPVersion6 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->IPVersion6[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->Network->DynDNS 					= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->DynDNS[0]				= xsd__boolean__true_;

	pSvcRp->Capabilities->Network->Dot11Configuration 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->Dot11Configuration[0]	= xsd__boolean__false_;

	pSvcRp->Capabilities->Network->Dot1XConfigurations 		= (int *)soap_malloc(soap, sizeof(int));
	pSvcRp->Capabilities->Network->Dot1XConfigurations[0]	= 0;

	pSvcRp->Capabilities->Network->HostnameFromDHCP			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->HostnameFromDHCP[0]		= xsd__boolean__true_;

	pSvcRp->Capabilities->Network->NTP						= (int *)soap_malloc(soap, sizeof(int));
	pSvcRp->Capabilities->Network->NTP[0]					= 0;
	
	pSvcRp->Capabilities->Network->DHCPv6					= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Network->DHCPv6[0]				= xsd__boolean__false_;

	pSvcRp->Capabilities->Network->__anyAttribute 			= NULL;

	/* SECURITY */
	szCap = sizeof(struct tds__SecurityCapabilities);
	pSvcRp->Capabilities->Security = (struct tds__SecurityCapabilities *)soap_malloc(soap, szCap);
	memset(pSvcRp->Capabilities->Security, 0x00, szCap);

	pSvcRp->Capabilities->Security->TLS1_x002e0 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->TLS1_x002e0[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->TLS1_x002e1 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->TLS1_x002e1[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->TLS1_x002e2 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->TLS1_x002e2[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->OnboardKeyGeneration 	= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->OnboardKeyGeneration[0]	= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->AccessPolicyConfig 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->AccessPolicyConfig[0]	= xsd__boolean__true_;

	pSvcRp->Capabilities->Security->DefaultAccessPolicy 	= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->DefaultAccessPolicy[0]	= xsd__boolean__true_;

	pSvcRp->Capabilities->Security->Dot1X 					= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->Dot1X[0]				= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->RemoteUserHandling 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->RemoteUserHandling[0]	= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->X_x002e509Token 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->X_x002e509Token[0]		= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->SAMLToken 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->SAMLToken[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->KerberosToken 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->KerberosToken[0]		= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->UsernameToken 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->UsernameToken[0]		= xsd__boolean__true_;

	pSvcRp->Capabilities->Security->HttpDigest 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->HttpDigest[0]			= xsd__boolean__true_;

	pSvcRp->Capabilities->Security->RELToken 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->Security->RELToken[0]				= xsd__boolean__false_;

	pSvcRp->Capabilities->Security->SupportedEAPMethods 	= soap_strdup(soap, "0");

	pSvcRp->Capabilities->Security->MaxUsers 				= (int *)soap_malloc(soap, sizeof(int));
	pSvcRp->Capabilities->Security->MaxUsers[0]				= MAX_USER;

	pSvcRp->Capabilities->Security->__anyAttribute 			= NULL;

	/* SYSTEM */
	szCap = sizeof(struct tds__SystemCapabilities);
	pSvcRp->Capabilities->System = (struct tds__SystemCapabilities *)soap_malloc(soap, szCap);
	memset(pSvcRp->Capabilities->System, 0x00, szCap);

	pSvcRp->Capabilities->System->DiscoveryResolve 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->DiscoveryResolve[0]		= xsd__boolean__false_;

	pSvcRp->Capabilities->System->DiscoveryBye 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->DiscoveryBye[0]			= xsd__boolean__true_;

	pSvcRp->Capabilities->System->RemoteDiscovery 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->RemoteDiscovery[0]		= xsd__boolean__false_;

	pSvcRp->Capabilities->System->SystemBackup 				= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->SystemBackup[0]			= xsd__boolean__true_;

	pSvcRp->Capabilities->System->FirmwareUpgrade 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->FirmwareUpgrade[0]		= xsd__boolean__true_;

	pSvcRp->Capabilities->System->SystemLogging 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->SystemLogging[0]			= xsd__boolean__false_;

	pSvcRp->Capabilities->System->HttpSystemBackup 			= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->HttpSystemBackup[0]		= xsd__boolean__false_;
	
	pSvcRp->Capabilities->System->HttpFirmwareUpgrade 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->HttpFirmwareUpgrade[0]		= xsd__boolean__true_;

	pSvcRp->Capabilities->System->HttpSystemLogging 		= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->HttpSystemLogging[0]		= xsd__boolean__false_;

	pSvcRp->Capabilities->System->HttpSupportInformation 	= (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
	pSvcRp->Capabilities->System->HttpSupportInformation[0]	= xsd__boolean__false_;

	pSvcRp->Capabilities->System->__anyAttribute 			= NULL;

	/* MISC */
 	pSvcRp->Capabilities->Misc = (struct tds__MiscCapabilities *)soap_malloc(soap, sizeof(struct tds__MiscCapabilities));
	memset(pSvcRp->Capabilities->Misc, 0x00, sizeof(struct tds__MiscCapabilities));	
	
	pSvcRp->Capabilities->Misc->AuxiliaryCommands 			= NULL;
	pSvcRp->Capabilities->Misc->__anyAttribute 				= NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetServiceCapabilities);
}

/**
 * @brief Get DeviceInformation 
 * @This message contains a request for GetDeviceInformation.
 *   
 * @param Request	: tds__GetDeviceInformation structure. 
 * @param Response	: tds__GetDeviceInformationResponse.
 * @return               : On success - SOAP_OK.
 */
int __tds__GetDeviceInformation(struct soap *soap, 
	struct _tds__GetDeviceInformation *tds__GetDeviceInformation, 
	struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{
	ENTER();

	struct _tds__GetDeviceInformationResponse *pDevInfo;

	dvprintf("%s MANUFACTURER %s MODEL_NAME %s FIRMWARE_VER %s SERIALNUMBER %s HARDWAREID %s\r\n",
				__func__, MANUFACTURER, MODEL_NAME, FIRMWARE_VER, SERIALNUMBER, HARDWAREID);	

	pDevInfo = (struct _tds__GetDeviceInformationResponse *)tds__GetDeviceInformationResponse;

	pDevInfo->Manufacturer 		= soap_strdup(soap, MANUFACTURER);
	pDevInfo->Model				= soap_strdup(soap, MODEL_NAME);
	pDevInfo->FirmwareVersion	= soap_strdup(soap, FIRMWARE_VER);
	pDevInfo->SerialNumber 		= soap_strdup(soap, SERIALNUMBER);
	pDevInfo->HardwareId		= soap_strdup(soap, HARDWAREID);

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetDeviceInformation);
}

/**
 * @brief Set SystemDateAndTime 
 * @This message contains a request for SetSystemDateAndTime.
 *   
 * @param Request	: tds__SetSystemDateAndTime. 
 * @param Response	: tds__SetSystemDateAndTimeResponse.
 * @return               : On success - SOAP_OK.
 */
int __tds__SetSystemDateAndTime(struct soap *soap, 
	struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, 
	struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{
	ENTER();

	struct _tds__SetSystemDateAndTime *pTime;

//	char value[INFO_LENGTH];
//	int ret;
	int _Year;
	int _Month;
	int _Day;
	int _Hour;
	int _Minute;
	int _Second;
//	int _DaylightSavings;
//	char *_TZ;
	int i;
	int exist =0;
	UINT var;

	pTime = (struct _tds__SetSystemDateAndTime *)tds__SetSystemDateAndTime;
#if (ENX_SNTPC_use==1)	
	gtUser.bUseSntp = pTime->DateTimeType; //Manual = 0, NTP = 1
#endif		
	/* DayLight *///summer time
	gtUser.nTimeSummer = pTime->DaylightSavings;
	
	/* Time Zone */
	if(pTime->TimeZone)
	{
		for(i = 0; i<MAX_TIMEZONE; i++)			
		{
			if(strcmp(GLB_TZ_LIST[i][0],pTime->TimeZone->TZ)==0)
			{
				exist = 1;
				setTimeZone(i);
				break;
			}
			if(strcmp(GLB_TZ_LIST[i][1],pTime->TimeZone->TZ)==0)
			{
				exist = 1;
				setTimeZone(i);
				break;
			}
			if(strcmp(GLB_TZ_LIST[i][2],pTime->TimeZone->TZ)==0)
			{
//				printf("GLB_TZ_LIST[%d][2] = %s\r\n",i,GLB_TZ_LIST[i][1]);
				var = atoi(GLB_TZ_LIST[i][1]);
//				printf("var = %d\r\n",var);
				exist = 1;
				gtUser.onvif_nTimeZone = i;
				setTimeZone(var);
				break;
			}
		}
		
		if(exist == 0)
		{
			onvif_fault(soap,"","ter:InvalidArgVal","ter:InvalidTimeZone","","");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	if(pTime->UTCDateTime)
	{
		_Year 	= pTime->UTCDateTime->Date->Year;
		_Month 	= pTime->UTCDateTime->Date->Month;
		_Day 	= pTime->UTCDateTime->Date->Day;
		_Hour 	= pTime->UTCDateTime->Time->Hour;
		_Minute = pTime->UTCDateTime->Time->Minute;
		_Second = pTime->UTCDateTime->Time->Second;

		if((_Hour>24) || (_Minute>60) || (_Second>60) || (_Month>12) || (_Day>31))
		{		
			onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidDateTime","","");
			LEAVE();
			return SOAP_FAULT;
		}
		
		struct tm tTimeSet;
		tTimeSet.tm_year = _Year - 1900;
		tTimeSet.tm_mon = _Month - 1;
		tTimeSet.tm_mday = _Day;
		tTimeSet.tm_hour = _Hour;
		tTimeSet.tm_min = _Minute;
		tTimeSet.tm_sec = _Second;
		dvprintf("y%d m%d d%d h%d m%d s%d\r\n",_Year,_Month,_Day,_Hour,_Minute, _Second);
		
		gptMsgShare.TIME = mktime(&tTimeSet);
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__SetSystemDateAndTimeResponse);
}

/**
 * @brief Get SystemDateAndTime 
 * @This message contains a request for GetSystemDateAndTime.
 *   
 * @param Request	: tds__GetSystemDateAndTime. 
 * @param Response	: tds__GetSystemDateAndTimeResponse.
 * @return               : On success - SOAP_OK.
 */
int __tds__GetSystemDateAndTime(struct soap *soap, 
    struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime,
    struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{
	ENTER();

	struct tm tmgmt;
	enx_get_tmtime(gptMsgShare.TIME, &tmgmt, DEF_NO);

	tds__GetSystemDateAndTimeResponse->SystemDateAndTime = (struct tt__SystemDateTime *)soap_malloc(soap, sizeof(struct tt__SystemDateTime));
	memset(tds__GetSystemDateAndTimeResponse->SystemDateAndTime, 0x00, sizeof(struct tt__SystemDateTime));
	struct tt__SystemDateTime *pSDT = tds__GetSystemDateAndTimeResponse->SystemDateAndTime;
#if (ENX_SNTPC_use==1)	
	pSDT->DateTimeType = gtUser.bUseSntp; 
#else
	pSDT->DateTimeType = tt__SetDateTimeType__Manual;
#endif
	pSDT->DaylightSavings = gtUser.nTimeSummer;
	pSDT->Extension = NULL;
	
	pSDT->TimeZone = (struct tt__TimeZone *)soap_malloc(soap, sizeof(struct tt__TimeZone));
	memset(pSDT->TimeZone, 0x00, sizeof(struct tt__TimeZone));

	int i,var,var1;


	var = atoi(GLB_TZ_LIST[gtUser.onvif_nTimeZone][1]);
	if(var == gtUser.nTimeZone){
		pSDT->TimeZone->TZ = soap_strdup(soap, GLB_TZ_LIST[gtUser.onvif_nTimeZone][2]);		
	}
	else{
		for(i = 0; i<MAX_TIMEZONE; i++){
			var1 = atoi(GLB_TZ_LIST[i][1]);
			if(var1 == gtUser.nTimeZone){
				pSDT->TimeZone->TZ = soap_strdup(soap, GLB_TZ_LIST[i][2]);
				break;
			}
		}
	}
	
	pSDT->UTCDateTime = (struct tt__DateTime *)soap_malloc(soap, sizeof(struct tt__DateTime));
	memset(pSDT->UTCDateTime, 0x00, sizeof(struct tt__DateTime));

	pSDT->UTCDateTime->Time = (struct tt__Time *)soap_malloc(soap, sizeof(struct tt__Time));
	memset(pSDT->UTCDateTime->Time, 0x00, sizeof(struct tt__Time));
	pSDT->UTCDateTime->Time->Hour = tmgmt.tm_hour;	
	pSDT->UTCDateTime->Time->Minute = tmgmt.tm_min;
	pSDT->UTCDateTime->Time->Second = tmgmt.tm_sec;

	dvprintf("Hour %d \r\n", pSDT->UTCDateTime->Time->Hour);
	dvprintf("Minute %d \r\n", pSDT->UTCDateTime->Time->Minute);
	dvprintf("Second %d \r\n", pSDT->UTCDateTime->Time->Second);

	pSDT->UTCDateTime->Date = (struct tt__Date *)soap_malloc(soap, sizeof(struct tt__Date));
	memset(pSDT->UTCDateTime->Date, 0x00, sizeof(struct tt__Date));
	pSDT->UTCDateTime->Date->Year = tmgmt.tm_year + 1900;
	pSDT->UTCDateTime->Date->Month = tmgmt.tm_mon + 1;
	pSDT->UTCDateTime->Date->Day = tmgmt.tm_mday;

	dvprintf("Year %d \r\n",pSDT->UTCDateTime->Date->Year);
	dvprintf("Month %d \r\n",pSDT->UTCDateTime->Date->Month);
	dvprintf("Day %d \r\n",pSDT->UTCDateTime->Date->Day);
	
	struct tm tmnow;
	enx_get_tmtime(gptMsgShare.TIME, &tmnow, DEF_YES);
	
	pSDT->LocalDateTime = (struct tt__DateTime *)soap_malloc(soap, sizeof(struct tt__DateTime));
	memset(pSDT->LocalDateTime, 0x00, sizeof(struct tt__DateTime));

	pSDT->LocalDateTime->Time = (struct tt__Time *)soap_malloc(soap, sizeof(struct tt__Time));
	memset(pSDT->LocalDateTime->Time, 0x00, sizeof(struct tt__Time));
	pSDT->LocalDateTime->Time->Hour = tmnow.tm_hour;
	pSDT->LocalDateTime->Time->Minute = tmnow.tm_min;
	pSDT->LocalDateTime->Time->Second = tmnow.tm_sec;

	dvprintf("LHour %d \r\n",pSDT->LocalDateTime->Time->Hour);
	dvprintf("LMinute %d \r\n",pSDT->UTCDateTime->Time->Minute);
	dvprintf("LSecond %d \r\n",pSDT->UTCDateTime->Time->Second);
	
	pSDT->LocalDateTime->Date = (struct tt__Date *)soap_malloc(soap, sizeof(struct tt__Date));
	memset(pSDT->LocalDateTime->Date, 0x00, sizeof(struct tt__Date));
	pSDT->LocalDateTime->Date->Year = tmnow.tm_year + 1900;
	pSDT->LocalDateTime->Date->Month = tmnow.tm_mon + 1;
	pSDT->LocalDateTime->Date->Day = tmnow.tm_mday;
	
	dvprintf("LYear %d \r\n",pSDT->LocalDateTime->Date->Year);
	dvprintf("LMonth %d \r\n",pSDT->LocalDateTime->Date->Month);
	dvprintf("LDay %d \r\n",pSDT->LocalDateTime->Date->Day);

	LEAVE();
	return SOAP_OK;
}


/**
 * @brief Setting System Factory Default of  IPNC.
 * @This operation reloads parameters of a device to their factory default values.
 * @param Request	: _tds__GetSystemFactoryDefault structure contains request for the following types of factory default to perform:
			 \A1\B0Hard\A1\B1: All parameters are set to their factory default value
			 \A1\B0Soft\A1\B1: All parameters except device vendor specific
 			parameters are set to their factory default values
 * @param Response	: _TimeRp structure variable members contains dummy response  .
 * @return               : On success - SOAP_OK.
 On failure - No command specific faults
 */
int __tds__SetSystemFactoryDefault(struct soap *soap, 
	struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, 
	struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{
	ENTER();


	switch (tds__SetSystemFactoryDefault->FactoryDefault) {
		case tt__FactoryDefaultType__Hard:
		{
			LoadDefUserInfo();
			UserSave(sfls_USER);

			init_onvif_profiles();
			OnvifCFG_Save();
		}
			break;
		case tt__FactoryDefaultType__Soft:
		{
			init_onvif_profiles();
			OnvifCFG_Save();
		}
			break;
	}

	SystemProcessCmd.IS_CALL = TRUE;
	SystemProcessCmd.CMD = SYSTEM_PROCESS_CMD_REBOOT_CALL;
	sys_thread_new("OnvifSystem", systemProcessFunction, NULL, UPNPUDPD_STACK_SIZE, REBOOT_TASK_PRIO);

	LEAVE();
	return SOAP_OK;
}


/**
 * @brief Updating firmware for IPNC.
 * @This operation upgrades a device firmware version
 * @param Request	: _tds__UpgradeSystemFirmware structure contains the firmware used for the upgrade.
 * @param Response	: _tds__UpgradeSystemFirmwareResponse structure variable members contains dummy response.
 * @return               : On success - SOAP_OK.
 On failure - The firmware was invalid, i.e., not supported by this device.
 */
int __tds__UpgradeSystemFirmware(struct soap *soap, 
	struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, 
	struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse)
{
	ENTER();

	struct _tds__UpgradeSystemFirmware *pUpfw;

	pUpfw = (struct _tds__UpgradeSystemFirmware *)tds__UpgradeSystemFirmware;

	if(pUpfw->Firmware->xop__Include.__ptr)
	dvprintf("__ptr %s\r\n",pUpfw->Firmware->xop__Include.__ptr);
	
	dvprintf("__size %s\r\n",pUpfw->Firmware->xop__Include.__size);
	
	if(pUpfw->Firmware->xop__Include.type)
		dvprintf("type %s\r\n",pUpfw->Firmware->xop__Include.type);

	if(pUpfw->Firmware->xop__Include.options)
		dvprintf("options %s\r\n",pUpfw->Firmware->xop__Include.options);

	if(pUpfw->Firmware->xmime__contentType)
		dvprintf("xmime__contentType %s\r\n",pUpfw->Firmware->xmime__contentType);	

	LEAVE();
	return SOAP_OK;
}



/**
 * @brief Reboot the IPNC.
 * @This function reboot the system.
 * @param Request	: _tds__SystemReboot structure contains dummy request.
 * @param Response	: _tds__SystemReboot structure contains the following message:
   			   "Rebooting".
 * @return               : On success - SOAP_OK.
 : On failure - No command specific faults.
 */
int __tds__SystemReboot(struct soap *soap, 
	struct _tds__SystemReboot *tds__SystemReboot, 
	struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{
	ENTER();

	tds__SystemRebootResponse->Message = soap_strdup(soap, "Rebooting in 20 seconds");

	SystemProcessCmd.IS_CALL = TRUE;
	SystemProcessCmd.CMD = SYSTEM_PROCESS_CMD_REBOOT_CALL;
	sys_thread_new("OnvifSystem", systemProcessFunction, NULL, _16KB_STACK_SIZE, REBOOT_TASK_PRIO);

	LEAVE();
	return SOAP_OK;
}

/**
 * @brief Restore System.
 * @This operation restores the system backup configuration files(s) previously retrieved from a device.

 * @param Request	: _tds__RestoreSystem structure contains the system backup file(s).
 * @param Response	: _tds__RestoreSystem structure dummy response.in this function response message is empity.
 * @return               : On success - SOAP_OK.
 : On failure - The backup file(s) are invalid.
 */
int __tds__RestoreSystem(struct soap *soap, 
	struct _tds__RestoreSystem *tds__RestoreSystem, 
	struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/**
 * @brief Get System backup
 * @This operation gets the  system backup configuration files(s) previously retrieved from adevice.
 * @param Request       :_tds__GetSystemBackup structure contains request for system backup.in this function message is empity.
 * @param Response      :_tds__GetSystemBackupResponse structure contains the system backup configuration files(s).
 * @return              : On success - SOAP_OK.
 : On failure - No command specific faults.

 */
int __tds__GetSystemBackup(struct soap *soap, 
	struct _tds__GetSystemBackup *tds__GetSystemBackup, 
	struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetSystemLog */
int __tds__GetSystemLog(struct soap *soap, 
	struct _tds__GetSystemLog *tds__GetSystemLog, 
	struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse)
{
	tds__GetSystemLogResponse->SystemLog = (struct tt__SystemLog *)soap_malloc(soap, sizeof(struct tt__SystemLog));
	memset(tds__GetSystemLogResponse->SystemLog, 0x00, sizeof(struct tt__SystemLog));
	struct tt__SystemLog *pGSL = tds__GetSystemLogResponse->SystemLog;
	pGSL->String= soap_strdup(soap, "SYSTEMPLOG");

	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetSystemSupportInformation */
int __tds__GetSystemSupportInformation(struct soap *soap, 
	struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, 
	struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}



/** 
 * @brief Get Scopes of the device
 * @This operation requests the scope parameters of a device. 

 *   
 * @param Request	: _tds__GetScopes structure varialbles members contains an empty message.
 * @param Response	: _tds__GetScopesResponse structure variable members ontains a list of URIs 
 defining the device scopes.
 * @return               : On success - SOAP_OK.
 : On failure - Scope list is empty.
 */
int __tds__GetScopes(struct soap *soap, 
	struct _tds__GetScopes *tds__GetScopes, 
	struct _tds__GetScopesResponse *tds__GetScopesResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i;

	tds__GetScopesResponse->__sizeScopes = oSysInfo->nScope;

	tds__GetScopesResponse->Scopes = (struct tt__Scope *)soap_malloc(soap, sizeof(struct tt__Scope) * tds__GetScopesResponse->__sizeScopes);
	memset(tds__GetScopesResponse->Scopes, 0x00, sizeof(struct tt__Scope) * tds__GetScopesResponse->__sizeScopes);
		
	for (i = 0; i < tds__GetScopesResponse->__sizeScopes; i++) {
		tds__GetScopesResponse->Scopes[i].ScopeDef = oSysInfo->Scope[i].type;
		tds__GetScopesResponse->Scopes[i].ScopeItem = soap_strdup(soap, oSysInfo->Scope[i].str);
//		flprintf("Scope[%d] type(%d) str(%s)\r\n", i, tds__GetScopesResponse->Scopes[i].ScopeDef, tds__GetScopesResponse->Scopes[i].ScopeItem);
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__SetScopes */
int __tds__SetScopes(struct soap *soap, 
	struct _tds__SetScopes *tds__SetScopes, 
	struct _tds__SetScopesResponse *tds__SetScopesResponse)
{
	ENTER();

	SysInfo *oSysInfo = GetSysInfo();
	int i, j;
	int exist = 0;

	if (tds__SetScopes->__sizeScopes > 0) {
		for (i = 0; i < tds__SetScopes->__sizeScopes; i++) {
			exist = 0;
			for (j = 0; j < oSysInfo->nScope; j++) {
				if (strcmp(oSysInfo->Scope[j].str, tds__SetScopes->Scopes[i]) == 0) {
					exist = 1;
					break;
				}
			}

			if(exist == 1) {
				continue;
			}

			oSysInfo->Scope[oSysInfo->nScope].type = 1;
			strcpy(oSysInfo->Scope[oSysInfo->nScope].str, tds__SetScopes->Scopes[i]);
			printf("Scope %dth type %d str %s \r\n", oSysInfo->nScope, oSysInfo->Scope[oSysInfo->nScope].type, oSysInfo->Scope[oSysInfo->nScope].str);
			oSysInfo->nScope++;
		}
		
		OnvifCFG_Save();
		
		sys_thread_new("wsddhello", hello_discovery, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__AddScopes */
int __tds__AddScopes(struct soap *soap, 
	struct _tds__AddScopes *tds__AddScopes, 
	struct _tds__AddScopesResponse *tds__AddScopesResponse)
{
	ENTER();

	int i,j;
	int exist = 0;

	if(tds__AddScopes->__sizeScopeItem)
	{	
		for(i = 0; i<tds__AddScopes->__sizeScopeItem; i++)
		{
			exist = 0;
			if(tds__AddScopes->ScopeItem[i])
			{		
				for(j=0; j<GetSysInfo()->nScope; j++)
				{					
					if(strcmp(GetSysInfo()->Scope[j].str,tds__AddScopes->ScopeItem[i])==0)
					{
						exist = 1;
						break;
					}
				}	

				if(exist == 1)
					continue;
				
				GetSysInfo()->Scope[GetSysInfo()->nScope].type = 1;
				strcpy(GetSysInfo()->Scope[GetSysInfo()->nScope].str,tds__AddScopes->ScopeItem[i]);
				printf("\n scope %dth type %d str %s \r\n",GetSysInfo()->nScope, GetSysInfo()->Scope[GetSysInfo()->nScope].type,GetSysInfo()->Scope[GetSysInfo()->nScope].str);
				GetSysInfo()->nScope++;

					
			}
		}

		OnvifCFG_Save();
		
		sys_thread_new("wsddhello",hello_discovery, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__RemoveScopes */
int __tds__RemoveScopes(struct soap *soap, 
	struct _tds__RemoveScopes *tds__RemoveScopes, 
	struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse)
{
	ENTER();

	int i,j;

	if(tds__RemoveScopes->__sizeScopeItem)
	{
		for(i = 0; i<tds__RemoveScopes->__sizeScopeItem; i++)
		{
			if(tds__RemoveScopes->ScopeItem[i])
			{
				for(j=0; j<GetSysInfo()->nScope; j++)
				{
					if(strcmp(GetSysInfo()->Scope[j].str, tds__RemoveScopes->ScopeItem[i])==0){
						printf("\n remove scope %dth %s \r\n",j,GetSysInfo()->Scope[j]);
						strcpy(GetSysInfo()->Scope[j].str,"");
						GetSysInfo()->Scope[j].type = 0;
						GetSysInfo()->nScope--;
					}
				}
			}
		}

		//rearrange need
		
		
		OnvifCFG_Save();

		sys_thread_new("wsddhello",hello_discovery, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetDiscoveryMode */
int __tds__GetDiscoveryMode(struct soap *soap, 
	struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, 
	struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse)
{
	ENTER();

	tds__GetDiscoveryModeResponse->DiscoveryMode = GetSysInfo()->lan_config.net.discovery_mode;

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__SetDiscoveryMode */
int __tds__SetDiscoveryMode(struct soap *soap, 
	struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, 
	struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse)
{
	ENTER();

	GetSysInfo()->lan_config.net.discovery_mode = tds__SetDiscoveryMode->DiscoveryMode;
	OnvifCFG_Save();

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetRemoteDiscoveryMode */
int __tds__GetRemoteDiscoveryMode(struct soap *soap, 
	struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, 
	struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__SetRemoteDiscoveryMode */
int __tds__SetRemoteDiscoveryMode(struct soap *soap, 
	struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, 
	struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetDPAddresses */
int __tds__GetDPAddresses(struct soap *soap, 
	struct _tds__GetDPAddresses *tds__GetDPAddresses, 
	struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetEndpointReference */
int __tds__GetEndpointReference(struct soap *soap, 
	struct _tds__GetEndpointReference *tds__GetEndpointReference, 
	struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetRemoteUser */
int __tds__GetRemoteUser(struct soap *soap, 
	struct _tds__GetRemoteUser *tds__GetRemoteUser, 
	struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse)
{	
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__SetRemoteUser */
int __tds__SetRemoteUser(struct soap *soap, 
	struct _tds__SetRemoteUser *tds__SetRemoteUser, 
	struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetUsers */
int __tds__GetUsers(struct soap *soap, 
	struct _tds__GetUsers *tds__GetUsers, 
	struct _tds__GetUsersResponse *tds__GetUsersResponse)
{
	ENTER();

	int i;

	tds__GetUsersResponse->__sizeUser = GetSysInfo()->nUserNumber;//NEW_ACOUNT;
	tds__GetUsersResponse->User = (struct tt__User*)soap_malloc(soap, sizeof(struct tt__User)*tds__GetUsersResponse->__sizeUser);
	memset(tds__GetUsersResponse->User, 0x00, sizeof(struct tt__User)*tds__GetUsersResponse->__sizeUser);

	for(i=0; i<tds__GetUsersResponse->__sizeUser; i++)
	{
		tds__GetUsersResponse->User[i].Username = soap_strdup(soap, GetSysInfo()->strGuestId[i]);
		tds__GetUsersResponse->User[i].UserLevel = GetSysInfo()->bUserLevel[i];
	}
	
	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__CreateUsers */
int __tds__CreateUsers(struct soap *soap, 
	struct _tds__CreateUsers *tds__CreateUsers, 
	struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{
	ENTER();

	int i,j;
 	if(tds__CreateUsers->__sizeUser)
	{
		for(i=0;i<tds__CreateUsers->__sizeUser;i++)
		{
			if(tds__CreateUsers->User[i].Username)
			{
				for(j=0;j<GetSysInfo()->nUserNumber;j++)
				{
					if(strcmp(tds__CreateUsers->User[i].Username, GetSysInfo()->strGuestId[j])==0)
					{
						onvif_fault(soap,"","ter:OperationProhibited","ter:UsernameClash","Username already exists","");
						LEAVE();
						return SOAP_FAULT;
					}
				}
				
				if(strlen(tds__CreateUsers->User[i].Username)>=16)
				{
					onvif_fault(soap,"","ter:OperationProhibited","ter:UsernameClash","Username length wrong","");
					LEAVE();
					return SOAP_FAULT;
				}
				else
				{			
					if(tds__CreateUsers->User[i].Password)
					{
						if(strlen(tds__CreateUsers->User[i].Password)<1 || strlen(tds__CreateUsers->User->Password)>16)
						{
							onvif_fault(soap,"","ter:OperationProhibited","ter:PasswordClash","Password length wrong","");
							LEAVE();
							return SOAP_FAULT;						
						}
						else
						{
							strcpy(GetSysInfo()->strGuestId[GetSysInfo()->nUserNumber],tds__CreateUsers->User[i].Username);
							strcpy(GetSysInfo()->strGuestPw[GetSysInfo()->nUserNumber],tds__CreateUsers->User[i].Password);
							GetSysInfo()->bUserLevel[GetSysInfo()->nUserNumber] = tds__CreateUsers->User[i].UserLevel;
							GetSysInfo()->nUserNumber++;
							OnvifCFG_Save();
						}
					}
					else
					{
						onvif_fault(soap,"","ter:OperationProhibited","ter:PasswordClash","Password Not exist","");
						LEAVE();
						return SOAP_FAULT;						
					}
				}
			}
		}
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__DeleteUsers */
int __tds__DeleteUsers(struct soap *soap, 
	struct _tds__DeleteUsers *tds__DeleteUsers, 
	struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{
	ENTER();

	int i,j;
	int exist = 0;

	if(tds__DeleteUsers->__sizeUsername)
	{
		for(i=0;i<tds__DeleteUsers->__sizeUsername;i++)
		{
			if(tds__DeleteUsers->Username[i])
			{
				for(j=0;j<GetSysInfo()->nUserNumber;j++)
				{
					if(strcmp(tds__DeleteUsers->Username[i], GetSysInfo()->strGuestId[j])==0)
					{							
						exist++;
						break;
					}
				}
			}
		}
		if(exist != tds__DeleteUsers->__sizeUsername)
		{
			onvif_fault(soap,"","ter:InvalidArgVal","ter:UsernameMissing","Username NOT recognized","");
			LEAVE();
			return SOAP_FAULT;						
		}
		
		for(i=0;i<tds__DeleteUsers->__sizeUsername;i++)
		{
			for(j=0;j<GetSysInfo()->nUserNumber;j++)
			{
				if(strcmp(tds__DeleteUsers->Username[i], GetSysInfo()->strGuestId[j])==0)
				{							
					GetSysInfo()->bUserLevel[j] = 0;
					strcpy(GetSysInfo()->strGuestId[j], GUEST1_ID);
					strcpy(GetSysInfo()->strGuestPw[j], GUEST1_PW);
		
					int k;
					for(k=j; k<GetSysInfo()->nUserNumber; k++)
					{
						if(k!=GetSysInfo()->nUserNumber-1 && k<GetSysInfo()->nUserNumber)
						{
							GetSysInfo()->bUserLevel[k] = GetSysInfo()->bUserLevel[k+1];
							strcpy(GetSysInfo()->strGuestId[k], GetSysInfo()->strGuestId[k+1]);
							strcpy(GetSysInfo()->strGuestPw[k], GetSysInfo()->strGuestPw[k+1]);
						}
					}
					GetSysInfo()->nUserNumber--;
					OnvifCFG_Save();
					break;
				}
			}
		}
	}	

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__SetUser */
int __tds__SetUser(struct soap *soap, 
	struct _tds__SetUser *tds__SetUser, 
	struct _tds__SetUserResponse *tds__SetUserResponse)
{
	ENTER();

	int i,j;
	int exist = 0;
	struct _tds__SetUser *pUsr;

	pUsr = tds__SetUser;		
		
 	if(pUsr->__sizeUser)
	{
		for(i=0;i<pUsr->__sizeUser;i++)
		{
			for(j=0;j<GetSysInfo()->nUserNumber;j++)
			{
				if(strcmp(pUsr->User[i].Username, GetSysInfo()->strGuestId[j])==0)
				{							
					exist++;
					break;
				}
			}
		}
		if(exist != pUsr->__sizeUser)
		{
			onvif_fault(soap,"","ter:InvalidArgVal","ter:UsernameMissing","Username NOT recognized","");
			LEAVE();
			return SOAP_FAULT;						
		}	
		
		for(i=0;i<pUsr->__sizeUser;i++)
		{
			if(pUsr->User[i].Username)
			{
				if(strlen(pUsr->User[i].Username)>=16)
				{
					onvif_fault(soap,"","ter:OperationProhibited","ter:UsernameClash","Username length wrong","");
					LEAVE();
					return SOAP_FAULT;
				}
				else
				{			
					for(j=0;j<GetSysInfo()->nUserNumber;j++)
					{
						if(strcmp(pUsr->User[i].Username, GetSysInfo()->strGuestId[j])==0)
						{
							
							if(pUsr->User[i].Password)
							{
								if(strlen(pUsr->User[i].Password)<1 || strlen(pUsr->User->Password)>16)
								{
									onvif_fault(soap,"","ter:OperationProhibited","ter:PasswordClash","Password length wrong","");
									LEAVE();
									return SOAP_FAULT;						
								}
								else
								{
									strcpy(GetSysInfo()->strGuestId[j],pUsr->User[i].Username);
									strcpy(GetSysInfo()->strGuestPw[j],pUsr->User[i].Password);
									GetSysInfo()->bUserLevel[j] = pUsr->User[i].UserLevel;
									OnvifCFG_Save();
									exist++;
								}
							}
							else
							{
								onvif_fault(soap,"","ter:OperationProhibited","ter:PasswordClash","Password Not exist","");
								LEAVE();
								return SOAP_FAULT;						
							}
						}
					}
				}
			}
		}
	}

	LEAVE();
	return SOAP_OK;
}


/** Auto-test server operation __tds__GetWsdlUrl */
int __tds__GetWsdlUrl(struct soap *soap, 
	struct _tds__GetWsdlUrl *tds__GetWsdlUrl, 
	struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse)
{
	ENTER();

	tds__GetWsdlUrlResponse->WsdlUrl = "http://www.onvif.org/Documents/Specifications.aspx";

	LEAVE();
	return SOAP_OK;
}


/**
 * @brief Get Capabilities of device 
 * @This message contains a request for device capabilities.
 *   
 * @param Request	: 
        _tds__GetCapabilities structures contains a request for device capabilities. 
 	    The client can either ask for all capabilities or just the capabilities 
 	        for a particular service category.
		All = 0, Analytics = 1, Device = 2, Events = 3, Imaging = 4, Media = 5, PTZ = 6

 * @param Response	
    : _tds__GetCapabilitiesResponse contains the requested device capabilities 
        using a hierarchical XML capability structure.

 * @return               : On success - SOAP_OK.
    On failure - The requested WSDL service category is not supported by the device.
 */
#if 0
int __tds__GetCapabilities(struct soap *soap,
	struct _tds__GetCapabilities *tds__GetCapabilities,
	struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
	ENTER();

	int bExt = TRUE;
	int bExtDeviceIO = TRUE;
	int bExtDisplay = FALSE;
	int bExtRecord = FALSE;
	int bExtSearch = FALSE;
	int bExtReplay = FALSE;
	int bExtReceiver = FALSE;
	int bExtAnalyticsDevice = FALSE;
	
	char _IPv4Address[IP_INFO_LENGTH] = {0};
 	char _IPv4[IP_INFO_LENGTH] = {0};
	strcpy(_IPv4, getIpAddress(HTTP_ONVIF_PORT));

	enum tt__CapabilityCategory *Category;
	struct _tds__GetCapabilities *pCap;
	struct _tds__GetCapabilitiesResponse *pCapRp;

	pCap = (struct _tds__GetCapabilities *)tds__GetCapabilities;
	pCapRp = (struct _tds__GetCapabilitiesResponse *)tds__GetCapabilitiesResponse;

	if (pCap == NULL) {
		printf("%s  tds__GetCapabilities == NULL SOAP_FAULT\r\n", __func__);
		LEAVE();
		return SOAP_FAULT;
	}

	if (pCapRp == NULL) {
		printf("%s  tds__GetCapabilitiesResponse == NULL SOAP_FAULT\r\n", __func__);
		LEAVE();
		return SOAP_FAULT;
	}

	Category = pCap->Category; 

	if (Category == NULL) {
		Category = (enum tt__CapabilityCategory *)soap_malloc(soap, sizeof(enum tt__CapabilityCategory));
		*Category = tt__CapabilityCategory__All;
	}

	flprintf("Category %d\r\n", *Category);	

	pCapRp->Capabilities = (struct tt__Capabilities *)soap_malloc(soap, sizeof(struct tt__Capabilities));
	memset(pCapRp->Capabilities, 0x00, sizeof(struct tt__Capabilities));

	if (*Category == tt__CapabilityCategory__All) {
#if 1
		pCapRp->Capabilities->Analytics = NULL;
#else
		pCapRp->Capabilities->Analytics = (struct tt__AnalyticsCapabilities *)soap_malloc(soap, sizeof(struct tt__AnalyticsCapabilities));
		memset(pCapRp->Capabilities->Analytics, 0x00, sizeof(struct tt__AnalyticsCapabilities));
		sprintf(_IPv4Address, ANALY_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Analytics->XAddr = soap_strdup(soap, _IPv4Address);

		pCapRp->Capabilities->Analytics->RuleSupport = xsd__boolean__false_;
		pCapRp->Capabilities->Analytics->AnalyticsModuleSupport = xsd__boolean__false_;
		pCapRp->Capabilities->Analytics->__size = 0;
		pCapRp->Capabilities->Analytics->__any = NULL;
		pCapRp->Capabilities->Analytics->__anyAttribute = NULL;
#endif
	}

	if ((*Category == tt__CapabilityCategory__Analytics)) {
		onvif_fault(soap,"env:Receiver", "ter:ActionNotSupported", "ter:NoSuchService","The requested WSDL service category is not supported by the NVT.",""); 
		LEAVE();
		return SOAP_FAULT;
	}

	if ((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Device)) {
		pCapRp->Capabilities->Device = (struct tt__DeviceCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceCapabilities));
		memset(pCapRp->Capabilities->Device, 0x00, sizeof(struct tt__DeviceCapabilities));
		struct tt__DeviceCapabilities *pDCap = pCapRp->Capabilities->Device;

		sprintf(_IPv4Address, DEVICE_SERVICE_ADDRESS, _IPv4);
		pDCap->XAddr = soap_strdup(soap, _IPv4Address);

//////////////////////////////////////////////////////////////////////////////// Start Device Network

		pDCap->Network = (struct tt__NetworkCapabilities *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilities));
		memset(pDCap->Network, 0x00, sizeof(struct tt__NetworkCapabilities));
		struct tt__NetworkCapabilities * pDNCap = pDCap->Network;

		pDNCap->IPFilter = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDNCap->IPFilter[0] = xsd__boolean__false_;

		pDNCap->ZeroConfiguration = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDNCap->ZeroConfiguration[0] = xsd__boolean__false_;

		pDNCap->IPVersion6 = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDNCap->IPVersion6[0] = xsd__boolean__false_;

		pDNCap->DynDNS = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDNCap->DynDNS[0] = xsd__boolean__true_;

		pDNCap->Extension = (struct tt__NetworkCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension));
		memset(pDNCap->Extension, 0x00, sizeof(struct tt__NetworkCapabilitiesExtension));
		struct tt__NetworkCapabilitiesExtension *pDNECap = pDNCap->Extension;

		pDNECap->__size = 0;
		pDNECap->__any = NULL;
		pDNECap->Dot11Configuration = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean)); 
		pDNECap->Dot11Configuration[0] = xsd__boolean__false_; 

		pDNECap->Extension = NULL;
//		pDNECap->Extension = (struct tt__NetworkCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension2));
//		memset(pDNECap->Extension, 0x00, sizeof(struct tt__NetworkCapabilitiesExtension2));
//		struct tt__NetworkCapabilitiesExtension2 *pDNE2Cap = pDNECap->Extension;
//
//		pDNE2Cap->__size = 0;
//		pDNE2Cap->__any = NULL;

//////////////////////////////////////////////////////////////////////////////// End Device Network
//////////////////////////////////////////////////////////////////////////////// Start Device System

		pDCap->System = (struct tt__SystemCapabilities *)soap_malloc(soap, sizeof(struct tt__SystemCapabilities));
		memset(pDCap->System, 0x00, sizeof(struct tt__SystemCapabilities));
		struct tt__SystemCapabilities *pDSCap = pDCap->System;
		
		pDSCap->DiscoveryResolve = xsd__boolean__false_;
		pDSCap->DiscoveryBye = xsd__boolean__true_;
		pDSCap->RemoteDiscovery = xsd__boolean__false_;
		pDSCap->SystemBackup = xsd__boolean__false_;
		pDSCap->SystemLogging = xsd__boolean__true_;
		pDSCap->FirmwareUpgrade = xsd__boolean__true_;
		pDSCap->__sizeSupportedVersions = 4;
		pDSCap->__anyAttribute = NULL;

		pDSCap->SupportedVersions = (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion) * pDSCap->__sizeSupportedVersions);
		memset(pDSCap->SupportedVersions, 0x00, sizeof(struct tt__OnvifVersion) * pDSCap->__sizeSupportedVersions);
		struct tt__OnvifVersion *pDSSVer = pDSCap->SupportedVersions;
		pDSSVer[0].Major = 2;
		pDSSVer[0].Minor = 40;
		pDSSVer[1].Major = 2;
		pDSSVer[1].Minor = 20;
		pDSSVer[2].Major = 2;
		pDSSVer[2].Minor = 10;
		pDSSVer[3].Major = 2;
		pDSSVer[3].Minor = 0;

		pDSCap->Extension = (struct tt__SystemCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension));
		memset(pDSCap->Extension, 0x00, sizeof(struct tt__SystemCapabilitiesExtension));
		struct tt__SystemCapabilitiesExtension *pDSECap = pDSCap->Extension;

		pDSECap->HttpSystemBackup = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDSECap->HttpSystemBackup[0] = xsd__boolean__false_;

		pDSECap->HttpFirmwareUpgrade = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDSECap->HttpFirmwareUpgrade[0] = xsd__boolean__true_;

		pDSECap->HttpSystemLogging = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDSECap->HttpSystemLogging[0] = xsd__boolean__true_;

		pDSECap->HttpSupportInformation = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDSECap->HttpSupportInformation[0] = xsd__boolean__false_;

		pDSECap->Extension = NULL;
//		pDSECap->Extension = (struct tt__SystemCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension2));
//		memset(pDSECap->Extension, 0x00, sizeof(struct tt__SystemCapabilitiesExtension2));
//		struct tt__SystemCapabilitiesExtension2 *pDSE2Cap = pDSECap->Extension;
//
//		pDSE2Cap->__size = 0;
//		pDSE2Cap->__any = NULL;

//////////////////////////////////////////////////////////////////////////////// End Device System
//////////////////////////////////////////////////////////////////////////////// Start Device IO

		pDCap->IO = (struct tt__IOCapabilities *)soap_malloc(soap, sizeof(struct tt__IOCapabilities));
		memset(pDCap->IO, 0x00, sizeof(struct tt__IOCapabilities));
		struct tt__IOCapabilities *pDICap = pDCap->IO;

		pDICap->InputConnectors = (int *)soap_malloc(soap, sizeof(int));
		pDICap->InputConnectors[0] = 0;

		pDICap->RelayOutputs = (int *)soap_malloc(soap, sizeof(int));
		pDICap->RelayOutputs[0] = 0;

		pDICap->Extension = NULL;

		pDICap->__anyAttribute = NULL;

//////////////////////////////////////////////////////////////////////////////// End Device IO

		pCapRp->Capabilities->Device->Security = (struct tt__SecurityCapabilities *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilities));
		memset(pCapRp->Capabilities->Device->Security, 0x00, sizeof(struct tt__SecurityCapabilities));

		pCapRp->Capabilities->Device->Security->TLS1_x002e1 = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->TLS1_x002e2 = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->OnboardKeyGeneration = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->AccessPolicyConfig = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->X_x002e509Token = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->SAMLToken = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->KerberosToken = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->RELToken = xsd__boolean__false_;
		pCapRp->Capabilities->Device->Security->__size = 0;
		pCapRp->Capabilities->Device->Security->__any = NULL;
		pCapRp->Capabilities->Device->Security->__anyAttribute = NULL;

		pCapRp->Capabilities->Device->Security->Extension = (struct tt__SecurityCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension));
		memset(pCapRp->Capabilities->Device->Security->Extension, 0x00, sizeof(struct tt__SecurityCapabilitiesExtension));

		pCapRp->Capabilities->Device->Security->Extension->TLS1_x002e0 = xsd__boolean__false_;

		pCapRp->Capabilities->Device->Security->Extension->Extension = (struct tt__SecurityCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension2));
		memset(pCapRp->Capabilities->Device->Security->Extension->Extension, 0x00, sizeof(struct tt__SecurityCapabilitiesExtension2));		
		struct tt__SecurityCapabilitiesExtension2 *SCapEx2 = pCapRp->Capabilities->Device->Security->Extension->Extension;

		SCapEx2->Dot1X = xsd__boolean__false_;
		SCapEx2->__sizeSupportedEAPMethod = 0;
		SCapEx2->SupportedEAPMethod = NULL;
		SCapEx2->RemoteUserHandling = xsd__boolean__false_;
		SCapEx2->__size = 0;
		SCapEx2->__any = NULL;

		pCapRp->Capabilities->Device->Extension = NULL;
		pCapRp->Capabilities->Device->__anyAttribute = NULL;
	}

#ifdef USE_EVENT_INTERFACE
	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Events))
	{			
		pCapRp->Capabilities->Events = (struct tt__EventCapabilities *)soap_malloc(soap, sizeof(struct tt__EventCapabilities));
		memset(pCapRp->Capabilities->Events, 0x00, sizeof(struct tt__EventCapabilities)  );
		sprintf(_IPv4Address, EVENT_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Events->XAddr = soap_strdup(soap, _IPv4Address);
		
		pCapRp->Capabilities->Events->WSSubscriptionPolicySupport = TRUE;	
		pCapRp->Capabilities->Events->WSPullPointSupport = TRUE; 
		pCapRp->Capabilities->Events->WSPausableSubscriptionManagerInterfaceSupport = FALSE;
		pCapRp->Capabilities->Events->__size			= 0;
		pCapRp->Capabilities->Events->__any 			= NULL;
		pCapRp->Capabilities->Events->__anyAttribute	= NULL;
	}
#endif

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Imaging))
	{		
		pCapRp->Capabilities->Imaging = (struct tt__ImagingCapabilities *)soap_malloc(soap, sizeof(struct tt__ImagingCapabilities));
		memset(pCapRp->Capabilities->Imaging, 0x00, sizeof(struct tt__ImagingCapabilities));

		sprintf(_IPv4Address, IMAGING_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Imaging->XAddr = soap_strdup(soap, _IPv4Address);
		pCapRp->Capabilities->Imaging->__anyAttribute = NULL;
	}

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Media))
	{			
		pCapRp->Capabilities->Media = (struct tt__MediaCapabilities *)soap_malloc(soap, sizeof(struct tt__MediaCapabilities));
		memset(pCapRp->Capabilities->Media, 0x00, sizeof(struct tt__MediaCapabilities));

		sprintf(_IPv4Address, MEDIA_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Media->XAddr = soap_strdup(soap, _IPv4Address);

		pCapRp->Capabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)soap_malloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
		memset(pCapRp->Capabilities->Media->StreamingCapabilities, 0x00, sizeof(struct tt__RealTimeStreamingCapabilities));

		pCapRp->Capabilities->Media->StreamingCapabilities->RTPMulticast = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean)); 
		pCapRp->Capabilities->Media->StreamingCapabilities->RTPMulticast[0] = xsd__boolean__false_;

		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP[0] = xsd__boolean__true_;

		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP[0] = xsd__boolean__true_;

		pCapRp->Capabilities->Media->StreamingCapabilities->Extension = NULL;
		pCapRp->Capabilities->Media->StreamingCapabilities->__anyAttribute = NULL;

		pCapRp->Capabilities->Media->Extension = (struct tt__MediaCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__MediaCapabilitiesExtension));
		memset(pCapRp->Capabilities->Media->Extension, 0x00, sizeof(struct tt__MediaCapabilitiesExtension));

		pCapRp->Capabilities->Media->Extension->ProfileCapabilities = (struct tt__ProfileCapabilities *)soap_malloc(soap, sizeof(struct tt__ProfileCapabilities));
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->MaximumNumberOfProfiles = MAX_PROFILE;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__any = NULL;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__anyAttribute = NULL;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__size = 0;
		pCapRp->Capabilities->Media->__size = 0;
		pCapRp->Capabilities->Media->__any = NULL;
		pCapRp->Capabilities->Media->__anyAttribute = NULL;
	}

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__PTZ))
	{
		pCapRp->Capabilities->PTZ = (struct tt__PTZCapabilities *)soap_malloc(soap, sizeof(struct tt__PTZCapabilities));
		memset(pCapRp->Capabilities->PTZ, 0x00, sizeof(struct tt__PTZCapabilities));

		sprintf(_IPv4Address, PTZ_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->PTZ->XAddr = soap_strdup(soap, _IPv4Address);
		pCapRp->Capabilities->PTZ->__size = 0;
		pCapRp->Capabilities->PTZ->__any = NULL;
		pCapRp->Capabilities->PTZ->__anyAttribute = NULL;
	}
//	if((*Category == tt__CapabilityCategory__PTZ))
//	{
//		onvif_fault(soap,"env:Receiver", "ter:ActionNotSupported", "ter:NoSuchService","The requested WSDL service category is not supported by the NVT.",""); 
//		return SOAP_FAULT;
//	}
	if((*Category == tt__CapabilityCategory__All) && bExt == TRUE)
	{		
		pCapRp->Capabilities->Extension = (struct tt__CapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__CapabilitiesExtension));
		memset(pCapRp->Capabilities->Extension, 0x00, sizeof(struct tt__CapabilitiesExtension)	);
		if(bExtDeviceIO == TRUE)
		{
			pCapRp->Capabilities->Extension->DeviceIO = (struct tt__DeviceIOCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceIOCapabilities));
			memset(pCapRp->Capabilities->Extension->DeviceIO, 0x00, sizeof(struct tt__DeviceIOCapabilities)  );
			sprintf(_IPv4Address, DEVIO_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->DeviceIO->XAddr = soap_strdup(soap, _IPv4Address);	
			
			pCapRp->Capabilities->Extension->DeviceIO->VideoSources = TRUE;
			pCapRp->Capabilities->Extension->DeviceIO->VideoOutputs = FALSE;
#ifdef __AUDIO__
			pCapRp->Capabilities->Extension->DeviceIO->AudioSources = TRUE;
#if (AUDIO_RX==1)
			pCapRp->Capabilities->Extension->DeviceIO->AudioOutputs = TRUE;
#else
			pCapRp->Capabilities->Extension->DeviceIO->AudioOutputs = FALSE;
#endif
#else
			pCapRp->Capabilities->Extension->DeviceIO->AudioSources = FALSE;
			pCapRp->Capabilities->Extension->DeviceIO->AudioOutputs = FALSE;
#endif
			pCapRp->Capabilities->Extension->DeviceIO->RelayOutputs = FALSE;
			pCapRp->Capabilities->Extension->DeviceIO->__size = 0;
			pCapRp->Capabilities->Extension->DeviceIO->__any = NULL;
			pCapRp->Capabilities->Extension->DeviceIO->__anyAttribute = NULL;
		}

		if(bExtDisplay == TRUE)
		{	
			pCapRp->Capabilities->Extension->Display = (struct tt__DisplayCapabilities *)soap_malloc(soap, sizeof(struct tt__DisplayCapabilities));
			memset(pCapRp->Capabilities->Extension->Display, 0x00, sizeof(struct tt__DisplayCapabilities));

			sprintf(_IPv4Address, DISP_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->Display->XAddr = soap_strdup(soap, _IPv4Address);
			pCapRp->Capabilities->Extension->Display->FixedLayout = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Display->__size = 0;
			pCapRp->Capabilities->Extension->Display->__any = NULL;
			pCapRp->Capabilities->Extension->Display->__anyAttribute = NULL;
		}
		if(bExtRecord == TRUE)
		{
			pCapRp->Capabilities->Extension->Recording = (struct tt__RecordingCapabilities *)soap_malloc(soap, sizeof(struct tt__RecordingCapabilities));
			memset(pCapRp->Capabilities->Extension->Recording, 0x00, sizeof(struct tt__RecordingCapabilities)  );

			sprintf(_IPv4Address, RECORD_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->Recording->XAddr = soap_strdup(soap, _IPv4Address);
			pCapRp->Capabilities->Extension->Recording->ReceiverSource = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Recording->MediaProfileSource = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Recording->DynamicRecordings = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Recording->DynamicTracks = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Recording->MaxStringLength = 100;
			pCapRp->Capabilities->Extension->Recording->__size = 0;
			pCapRp->Capabilities->Extension->Recording->__any = NULL;
			pCapRp->Capabilities->Extension->Recording->__anyAttribute = NULL;
		}
		if(bExtSearch == TRUE)
		{			
			pCapRp->Capabilities->Extension->Search = (struct tt__SearchCapabilities *)soap_malloc(soap, sizeof(struct tt__SearchCapabilities));
			memset(pCapRp->Capabilities->Extension->Search, 0x00, sizeof(struct tt__SearchCapabilities));

			sprintf(_IPv4Address, SEARCH_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->Search->XAddr = soap_strdup(soap, _IPv4Address);
			pCapRp->Capabilities->Extension->Search->MetadataSearch = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Search->__size = 0;
			pCapRp->Capabilities->Extension->Search->__any = NULL;
			pCapRp->Capabilities->Extension->Search->__anyAttribute = NULL;
		}
		if(bExtReplay == TRUE)
		{			
			pCapRp->Capabilities->Extension->Replay = (struct tt__ReplayCapabilities*)soap_malloc(soap, sizeof(struct tt__ReplayCapabilities));
			memset(pCapRp->Capabilities->Extension->Replay, 0x00, sizeof(struct tt__ReplayCapabilities)  ); 
			sprintf(_IPv4Address, REPLY_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->Replay->XAddr = soap_strdup(soap, _IPv4Address);
			
			pCapRp->Capabilities->Extension->Replay->__size = 0;
			pCapRp->Capabilities->Extension->Replay->__any = NULL;
			pCapRp->Capabilities->Extension->Replay->__anyAttribute = NULL;
		}
		if(bExtReceiver == TRUE)
		{
			pCapRp->Capabilities->Extension->Receiver = (struct tt__ReceiverCapabilities*)soap_malloc(soap, sizeof(struct tt__ReceiverCapabilities));
			memset(pCapRp->Capabilities->Extension->Receiver, 0x00, sizeof(struct tt__ReceiverCapabilities)  );
			sprintf(_IPv4Address, RECV_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->Receiver->XAddr = soap_strdup(soap, _IPv4Address);
			
			pCapRp->Capabilities->Extension->Receiver->RTP_USCOREMulticast = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Receiver->RTP_USCORETCP = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Receiver->RTP_USCORERTSP_USCORETCP = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->Receiver->SupportedReceivers = 0;
			pCapRp->Capabilities->Extension->Receiver->MaximumRTSPURILength = 0;
			pCapRp->Capabilities->Extension->Receiver->__size = 0;
			pCapRp->Capabilities->Extension->Receiver->__any = NULL;
			pCapRp->Capabilities->Extension->Receiver->__anyAttribute = NULL;
		}
		if(bExtAnalyticsDevice == TRUE)
		{
			pCapRp->Capabilities->Extension->AnalyticsDevice = (struct tt__AnalyticsDeviceCapabilities*)soap_malloc(soap, sizeof(struct tt__AnalyticsDeviceCapabilities));
			memset(pCapRp->Capabilities->Extension->AnalyticsDevice, 0x00, sizeof(struct tt__AnalyticsDeviceCapabilities)  );
			sprintf(_IPv4Address, ANALYDV_SERVICE_ADDRESS, _IPv4);
			pCapRp->Capabilities->Extension->AnalyticsDevice->XAddr = soap_strdup(soap, _IPv4Address);
			
			pCapRp->Capabilities->Extension->AnalyticsDevice->RuleSupport = (int *)soap_malloc(soap, sizeof(int));
			*(pCapRp->Capabilities->Extension->AnalyticsDevice->RuleSupport) = xsd__boolean__false_;
			pCapRp->Capabilities->Extension->AnalyticsDevice->Extension = NULL;
			pCapRp->Capabilities->Extension->AnalyticsDevice->__anyAttribute = NULL;
		}
		
		pCapRp->Capabilities->Extension->Extensions = NULL;
		pCapRp->Capabilities->Extension->__size = 0;
		pCapRp->Capabilities->Extension->__any = NULL;
	}

	LEAVE();
	return SOAP_OK;
}
#else
int __tds__GetCapabilities(struct soap *soap,
	struct _tds__GetCapabilities *tds__GetCapabilities,
	struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{
	ENTER();

	int bExt = TRUE;
	int bExtDeviceIO = TRUE;
	int bExtDisplay = FALSE;
	int bExtRecord = FALSE;
	int bExtSearch = FALSE;
	int bExtReplay = FALSE;
	int bExtReceiver = FALSE;
	int bExtAnalyticsDevice = FALSE;
	
	char _IPv4Address[IP_INFO_LENGTH] = {0};
 	char _IPv4[IP_INFO_LENGTH] = {0};
	strcpy(_IPv4, getIpAddress(HTTP_ONVIF_PORT));

	enum tt__CapabilityCategory *Category;
	struct _tds__GetCapabilities *pCap;
	struct _tds__GetCapabilitiesResponse *pCapRp;

	pCap = (struct _tds__GetCapabilities *)tds__GetCapabilities;
	pCapRp = (struct _tds__GetCapabilitiesResponse *)tds__GetCapabilitiesResponse;

	if (pCap == NULL) {
		printf("%s  tds__GetCapabilities == NULL SOAP_FAULT\r\n", __func__);
		LEAVE();
		return SOAP_FAULT;
	}

	if (pCapRp == NULL) {
		printf("%s  tds__GetCapabilitiesResponse == NULL SOAP_FAULT\r\n", __func__);
		LEAVE();
		return SOAP_FAULT;
	}

	Category = pCap->Category; 

	if (Category == NULL) {
		Category = (enum tt__CapabilityCategory *)soap_malloc(soap, sizeof(enum tt__CapabilityCategory));
		*Category = tt__CapabilityCategory__All;
	}

//	flprintf("Category %d\r\n", *Category);	

	pCapRp->Capabilities = (struct tt__Capabilities *)soap_malloc(soap, sizeof(struct tt__Capabilities));
	memset(pCapRp->Capabilities, 0x00, sizeof(struct tt__Capabilities));

	if (*Category == tt__CapabilityCategory__All) {
		pCapRp->Capabilities->Analytics = (struct tt__AnalyticsCapabilities *)soap_malloc(soap, sizeof(struct tt__AnalyticsCapabilities));
		memset(pCapRp->Capabilities->Analytics, 0x00, sizeof(struct tt__AnalyticsCapabilities));
		struct tt__AnalyticsCapabilities *pAsCap = pCapRp->Capabilities->Analytics;
		
		sprintf(_IPv4Address, ANALY_SERVICE_ADDRESS, _IPv4);
		pAsCap->XAddr = soap_strdup(soap, _IPv4Address);
		pAsCap->RuleSupport = xsd__boolean__false_;
		pAsCap->AnalyticsModuleSupport = xsd__boolean__false_;
		pAsCap->__size = 0;
		pAsCap->__any = NULL;
		pAsCap->__anyAttribute = NULL;
	}
	if (*Category == tt__CapabilityCategory__Analytics) {
		onvif_fault(soap,"env:Receiver", "ter:ActionNotSupported", "ter:NoSuchService", "The requested WSDL service category is not supported by the NVT.",""); 
		LEAVE();
		return SOAP_FAULT;
	}

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Device))
	{		
		pCapRp->Capabilities->Device = (struct tt__DeviceCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceCapabilities));
		memset(pCapRp->Capabilities->Device, 0x00, sizeof(struct tt__DeviceCapabilities));
		struct tt__DeviceCapabilities *pDiCap = pCapRp->Capabilities->Device;

		sprintf(_IPv4Address, DEVICE_SERVICE_ADDRESS, _IPv4);
		pDiCap->XAddr = soap_strdup(soap, _IPv4Address);

		pDiCap->Network = (struct tt__NetworkCapabilities *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilities));
		memset(pDiCap->Network, 0x00, sizeof(struct tt__NetworkCapabilities));

		pDiCap->Network->IPFilter = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->Network->IPFilter[0] = xsd__boolean__false_;

		pDiCap->Network->ZeroConfiguration = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->Network->ZeroConfiguration[0] = xsd__boolean__false_;

		pDiCap->Network->IPVersion6 = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->Network->IPVersion6[0] = xsd__boolean__false_;

		pDiCap->Network->DynDNS = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->Network->DynDNS[0] = xsd__boolean__true_;	

		pDiCap->Network->Extension = (struct tt__NetworkCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__NetworkCapabilitiesExtension));
		memset(pDiCap->Network->Extension, 0x00, sizeof(struct tt__NetworkCapabilitiesExtension));
		pDiCap->Network->Extension->__size = 0;
		pDiCap->Network->Extension->__any = NULL;
		pDiCap->Network->Extension->Dot11Configuration = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean)); 
		pDiCap->Network->Extension->Dot11Configuration[0] = xsd__boolean__false_; 
		pDiCap->Network->Extension->Extension = NULL;

		pDiCap->System = (struct tt__SystemCapabilities *)soap_malloc(soap, sizeof(struct tt__SystemCapabilities));
		memset(pDiCap->System, 0x00, sizeof(struct tt__SystemCapabilities));
		
		pDiCap->System->DiscoveryResolve = xsd__boolean__false_;
		pDiCap->System->DiscoveryBye = xsd__boolean__true_;
		pDiCap->System->RemoteDiscovery = xsd__boolean__false_;
		pDiCap->System->SystemBackup = xsd__boolean__false_;
		pDiCap->System->SystemLogging = xsd__boolean__false_;
		pDiCap->System->FirmwareUpgrade = xsd__boolean__true_;

		pDiCap->System->__sizeSupportedVersions = 4;
		pDiCap->System->SupportedVersions = (struct tt__OnvifVersion *)soap_malloc(soap, sizeof(struct tt__OnvifVersion) * pDiCap->System->__sizeSupportedVersions);
		memset(pDiCap->System->SupportedVersions, 0x00, sizeof(struct tt__OnvifVersion) * pDiCap->System->__sizeSupportedVersions);
		pDiCap->System->SupportedVersions[0].Major = 2;//ONVIF_SERVICE_VERSION_MAJOR;
		pDiCap->System->SupportedVersions[0].Minor = 40;//ONVIF_SERVICE_VERSION_MINOR;
		pDiCap->System->SupportedVersions[1].Major = 2;//20;
		pDiCap->System->SupportedVersions[1].Minor = 20;//2;
		pDiCap->System->SupportedVersions[2].Major = 2;//20;
		pDiCap->System->SupportedVersions[2].Minor = 10;//2;
		pDiCap->System->SupportedVersions[3].Major = 2;//ONVIF_SERVICE_VERSION_MAJOR;
		pDiCap->System->SupportedVersions[3].Minor = 0;//ONVIF_SERVICE_VERSION_MINOR;

		pDiCap->System->Extension = (struct tt__SystemCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension));
		memset(pDiCap->System->Extension, 0x00, sizeof(struct tt__SystemCapabilitiesExtension));

		pDiCap->System->Extension->__size = 0;
		pDiCap->System->Extension->__any = NULL;
		
		pDiCap->System->Extension->HttpSystemBackup = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->System->Extension->HttpSystemBackup[0] = xsd__boolean__false_;

		pDiCap->System->Extension->HttpFirmwareUpgrade = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->System->Extension->HttpFirmwareUpgrade[0] = xsd__boolean__true_;

		pDiCap->System->Extension->HttpSystemLogging = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->System->Extension->HttpSystemLogging[0] = xsd__boolean__false_;

		pDiCap->System->Extension->HttpSupportInformation = (enum xsd__boolean *)soap_malloc(soap, sizeof(enum xsd__boolean));
		pDiCap->System->Extension->HttpSupportInformation[0] = xsd__boolean__false_;

		pDiCap->System->Extension->Extension = NULL;

		pDiCap->System->__anyAttribute = NULL;

		pDiCap->IO = (struct tt__IOCapabilities *)soap_malloc(soap, sizeof(struct tt__IOCapabilities));
		memset(pDiCap->IO, 0x00, sizeof(struct tt__IOCapabilities));

		pDiCap->IO->InputConnectors = (int *)soap_malloc(soap, sizeof(int));
		pDiCap->IO->InputConnectors[0] = 0;

		pDiCap->IO->RelayOutputs = (int *)soap_malloc(soap, sizeof(int));
		pDiCap->IO->RelayOutputs[0] = 0;

		pDiCap->IO->Extension = NULL;
		pDiCap->IO->__anyAttribute = NULL;

		pDiCap->Security = (struct tt__SecurityCapabilities *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilities));
		memset(pDiCap->Security, 0x00, sizeof(struct tt__SecurityCapabilities));

		pDiCap->Security->TLS1_x002e1 = xsd__boolean__false_;
		pDiCap->Security->TLS1_x002e2 = xsd__boolean__false_;
		pDiCap->Security->OnboardKeyGeneration = xsd__boolean__false_;
		pDiCap->Security->AccessPolicyConfig = xsd__boolean__true_;
		pDiCap->Security->X_x002e509Token = xsd__boolean__false_;
		pDiCap->Security->SAMLToken = xsd__boolean__false_;
		pDiCap->Security->KerberosToken = xsd__boolean__false_;
		pDiCap->Security->RELToken = xsd__boolean__false_;
		pDiCap->Security->__size = 0;
		pDiCap->Security->__any = NULL;
		pDiCap->Security->__anyAttribute = NULL;
#if 1
		pDiCap->Security->Extension = (struct tt__SecurityCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension));
		memset(pDiCap->Security->Extension, 0x00, sizeof(struct tt__SecurityCapabilitiesExtension));

		pDiCap->Security->Extension->TLS1_x002e0 = xsd__boolean__false_;

		pDiCap->Security->Extension->Extension = (struct tt__SecurityCapabilitiesExtension2 *)soap_malloc(soap, sizeof(struct tt__SecurityCapabilitiesExtension2));
		memset(pDiCap->Security->Extension->Extension, 0x00, sizeof(struct tt__SecurityCapabilitiesExtension2));		
		struct tt__SecurityCapabilitiesExtension2 *SCapEx2 = pDiCap->Security->Extension->Extension;

		SCapEx2->Dot1X = xsd__boolean__false_;
		SCapEx2->__sizeSupportedEAPMethod = 0;
		SCapEx2->SupportedEAPMethod = NULL;
		SCapEx2->RemoteUserHandling = xsd__boolean__false_;
		SCapEx2->__size = 0;
		SCapEx2->__any = NULL;
#else
		pDiCap->Security->Extension = NULL;
#endif
		pDiCap->Extension = NULL;
		pDiCap->__anyAttribute = NULL;
	}

#ifdef USE_EVENT_INTERFACE
	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Events))
	{			
		pCapRp->Capabilities->Events = (struct tt__EventCapabilities *)soap_malloc(soap, sizeof(struct tt__EventCapabilities));
		memset(pCapRp->Capabilities->Events, 0x00, sizeof(struct tt__EventCapabilities)  );
		sprintf(_IPv4Address, EVENT_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Events->XAddr = soap_strdup(soap, _IPv4Address);
		
		pCapRp->Capabilities->Events->WSSubscriptionPolicySupport = TRUE;	
		pCapRp->Capabilities->Events->WSPullPointSupport = TRUE; 
		pCapRp->Capabilities->Events->WSPausableSubscriptionManagerInterfaceSupport = FALSE;
		pCapRp->Capabilities->Events->__size			= 0;
		pCapRp->Capabilities->Events->__any 			= NULL;
		pCapRp->Capabilities->Events->__anyAttribute	= NULL;
	}
#endif

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Imaging))
	{		
		pCapRp->Capabilities->Imaging = (struct tt__ImagingCapabilities *)soap_malloc(soap, sizeof(struct tt__ImagingCapabilities));
		memset(pCapRp->Capabilities->Imaging, 0x00, sizeof(struct tt__ImagingCapabilities));

		sprintf(_IPv4Address, IMAGING_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Imaging->XAddr = soap_strdup(soap, _IPv4Address);
		pCapRp->Capabilities->Imaging->__anyAttribute = NULL;
	}

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__Media))
	{			
		pCapRp->Capabilities->Media = (struct tt__MediaCapabilities *)soap_malloc(soap, sizeof(struct tt__MediaCapabilities));
		memset(pCapRp->Capabilities->Media, 0x00, sizeof(struct tt__MediaCapabilities));

		sprintf(_IPv4Address, MEDIA_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->Media->XAddr = soap_strdup(soap, _IPv4Address);

		pCapRp->Capabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities *)soap_malloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
		memset(pCapRp->Capabilities->Media->StreamingCapabilities, 0x00, sizeof(struct tt__RealTimeStreamingCapabilities));

		pCapRp->Capabilities->Media->StreamingCapabilities->RTPMulticast = (int *)soap_malloc(soap, sizeof(int)); 
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = (int *)soap_malloc(soap, sizeof(int));
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (int *)soap_malloc(soap, sizeof(int));

		pCapRp->Capabilities->Media->StreamingCapabilities->RTPMulticast[0] = xsd__boolean__false_; 
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP[0] = xsd__boolean__true_; 
		pCapRp->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP[0] = xsd__boolean__true_;	
		pCapRp->Capabilities->Media->StreamingCapabilities->Extension = NULL;
		pCapRp->Capabilities->Media->StreamingCapabilities->__anyAttribute = NULL;

		pCapRp->Capabilities->Media->Extension = (struct tt__MediaCapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__MediaCapabilitiesExtension));
		memset(pCapRp->Capabilities->Media->Extension, 0x00, sizeof(struct tt__MediaCapabilitiesExtension));
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities = (struct tt__ProfileCapabilities *)soap_malloc(soap, sizeof(struct tt__ProfileCapabilities));
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->MaximumNumberOfProfiles = MAX_PROFILE;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__any = NULL;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__anyAttribute = NULL;
		pCapRp->Capabilities->Media->Extension->ProfileCapabilities->__size = 0;
		pCapRp->Capabilities->Media->__size = 0;
		pCapRp->Capabilities->Media->__any = NULL;
		pCapRp->Capabilities->Media->__anyAttribute = NULL;
	}

	if((*Category == tt__CapabilityCategory__All) || (*Category == tt__CapabilityCategory__PTZ))
	{
		pCapRp->Capabilities->PTZ = (struct tt__PTZCapabilities *)soap_malloc(soap, sizeof(struct tt__PTZCapabilities));
		memset(pCapRp->Capabilities->PTZ, 0x00, sizeof(struct tt__PTZCapabilities));

		sprintf(_IPv4Address, PTZ_SERVICE_ADDRESS, _IPv4);
		pCapRp->Capabilities->PTZ->XAddr = soap_strdup(soap, _IPv4Address);
		pCapRp->Capabilities->PTZ->__size = 0;
		pCapRp->Capabilities->PTZ->__any = NULL;
		pCapRp->Capabilities->PTZ->__anyAttribute = NULL;
	}
//	if((*Category == tt__CapabilityCategory__PTZ))
//	{
//		onvif_fault(soap,"env:Receiver", "ter:ActionNotSupported", "ter:NoSuchService","The requested WSDL service category is not supported by the NVT.",""); 
//		return SOAP_FAULT;
//	}

	if((*Category == tt__CapabilityCategory__All) && bExt == TRUE)
	{		
		pCapRp->Capabilities->Extension = (struct tt__CapabilitiesExtension *)soap_malloc(soap, sizeof(struct tt__CapabilitiesExtension));
		memset(pCapRp->Capabilities->Extension, 0x00, sizeof(struct tt__CapabilitiesExtension));
		struct tt__CapabilitiesExtension *pCapExt = pCapRp->Capabilities->Extension;

		if(bExtDeviceIO == TRUE)
		{
			pCapExt->DeviceIO = (struct tt__DeviceIOCapabilities *)soap_malloc(soap, sizeof(struct tt__DeviceIOCapabilities));
			memset(pCapExt->DeviceIO, 0x00, sizeof(struct tt__DeviceIOCapabilities)  );

			sprintf(_IPv4Address, DEVIO_SERVICE_ADDRESS, _IPv4);
			pCapExt->DeviceIO->XAddr = soap_strdup(soap, _IPv4Address);
			pCapExt->DeviceIO->VideoSources = TRUE;
			pCapExt->DeviceIO->VideoOutputs = FALSE;
#ifdef __AUDIO__
			pCapExt->DeviceIO->AudioSources = TRUE;
#if (AUDIO_RX==1)
			pCapExt->DeviceIO->AudioOutputs = TRUE;
#else
			pCapExt->DeviceIO->AudioOutputs = FALSE;
#endif
#else
			pCapExt->DeviceIO->AudioSources = FALSE;
			pCapExt->DeviceIO->AudioOutputs = FALSE;
#endif
			pCapExt->DeviceIO->RelayOutputs = FALSE;
			pCapExt->DeviceIO->__size = 0;
			pCapExt->DeviceIO->__any = NULL;
			pCapExt->DeviceIO->__anyAttribute = NULL;
		}

		if(bExtDisplay == TRUE)
		{	
			pCapExt->Display = (struct tt__DisplayCapabilities *)soap_malloc(soap, sizeof(struct tt__DisplayCapabilities));
			memset(pCapExt->Display, 0x00, sizeof(struct tt__DisplayCapabilities));

			sprintf(_IPv4Address, DISP_SERVICE_ADDRESS, _IPv4);
			pCapExt->Display->XAddr = soap_strdup(soap, _IPv4Address);
			pCapExt->Display->FixedLayout = xsd__boolean__false_;
			pCapExt->Display->__size = 0;
			pCapExt->Display->__any = NULL;
			pCapExt->Display->__anyAttribute = NULL;
		}

		if(bExtRecord == TRUE)
		{
			pCapExt->Recording = (struct tt__RecordingCapabilities *)soap_malloc(soap, sizeof(struct tt__RecordingCapabilities));
			memset(pCapExt->Recording, 0x00, sizeof(struct tt__RecordingCapabilities)  );

			sprintf(_IPv4Address, RECORD_SERVICE_ADDRESS, _IPv4);
			pCapExt->Recording->XAddr = soap_strdup(soap, _IPv4Address);
			pCapExt->Recording->ReceiverSource = xsd__boolean__false_;
			pCapExt->Recording->MediaProfileSource = xsd__boolean__false_;
			pCapExt->Recording->DynamicRecordings = xsd__boolean__false_;
			pCapExt->Recording->DynamicTracks = xsd__boolean__false_;
			pCapExt->Recording->MaxStringLength = 100;
			pCapExt->Recording->__size = 0;
			pCapExt->Recording->__any = NULL;
			pCapExt->Recording->__anyAttribute = NULL;
		}

		if(bExtSearch == TRUE)
		{			
			pCapExt->Search = (struct tt__SearchCapabilities *)soap_malloc(soap, sizeof(struct tt__SearchCapabilities));
			memset(pCapExt->Search, 0x00, sizeof(struct tt__SearchCapabilities));

			sprintf(_IPv4Address, SEARCH_SERVICE_ADDRESS, _IPv4);
			pCapExt->Search->XAddr = soap_strdup(soap, _IPv4Address);
			pCapExt->Search->MetadataSearch = xsd__boolean__false_;
			pCapExt->Search->__size = 0;
			pCapExt->Search->__any = NULL;
			pCapExt->Search->__anyAttribute = NULL;
		}

		if(bExtReplay == TRUE)
		{			
			pCapExt->Replay = (struct tt__ReplayCapabilities*)soap_malloc(soap, sizeof(struct tt__ReplayCapabilities));
			memset(pCapExt->Replay, 0x00, sizeof(struct tt__ReplayCapabilities)  ); 
			sprintf(_IPv4Address, REPLY_SERVICE_ADDRESS, _IPv4);
			pCapExt->Replay->XAddr = soap_strdup(soap, _IPv4Address);
			
			pCapExt->Replay->__size = 0;
			pCapExt->Replay->__any = NULL;
			pCapExt->Replay->__anyAttribute = NULL;
		}

		if(bExtReceiver == TRUE)
		{
			pCapExt->Receiver = (struct tt__ReceiverCapabilities*)soap_malloc(soap, sizeof(struct tt__ReceiverCapabilities));
			memset(pCapExt->Receiver, 0x00, sizeof(struct tt__ReceiverCapabilities)  );
			sprintf(_IPv4Address, RECV_SERVICE_ADDRESS, _IPv4);
			pCapExt->Receiver->XAddr = soap_strdup(soap, _IPv4Address);
			
			pCapExt->Receiver->RTP_USCOREMulticast = xsd__boolean__false_;
			pCapExt->Receiver->RTP_USCORETCP = xsd__boolean__false_;
			pCapExt->Receiver->RTP_USCORERTSP_USCORETCP = xsd__boolean__false_;
			pCapExt->Receiver->SupportedReceivers = 0;
			pCapExt->Receiver->MaximumRTSPURILength = 0;
			pCapExt->Receiver->__size = 0;
			pCapExt->Receiver->__any = NULL;
			pCapExt->Receiver->__anyAttribute = NULL;
		}

		if(bExtAnalyticsDevice == TRUE)
		{
			pCapExt->AnalyticsDevice = (struct tt__AnalyticsDeviceCapabilities*)soap_malloc(soap, sizeof(struct tt__AnalyticsDeviceCapabilities));
			memset(pCapExt->AnalyticsDevice, 0x00, sizeof(struct tt__AnalyticsDeviceCapabilities)  );
			sprintf(_IPv4Address, ANALYDV_SERVICE_ADDRESS, _IPv4);
			pCapExt->AnalyticsDevice->XAddr = soap_strdup(soap, _IPv4Address);

			pCapExt->AnalyticsDevice->RuleSupport = (int *)soap_malloc(soap, sizeof(int));
			*(pCapExt->AnalyticsDevice->RuleSupport) = xsd__boolean__false_;
			pCapExt->AnalyticsDevice->Extension = NULL;
			pCapExt->AnalyticsDevice->__anyAttribute = NULL;
		}

		pCapExt->Extensions = NULL;
		pCapExt->__size = 0;
		pCapExt->__any = NULL;
	}

	LEAVE();
	return SOAP_OK;
}

#endif

/** Auto-test server operation __tds__SetDPAddresses */
int __tds__SetDPAddresses(struct soap *soap, 
	struct _tds__SetDPAddresses *tds__SetDPAddresses, 
	struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetDPAddresses);
	UNUSED(tds__SetDPAddressesResponse);
}


/** Auto-test server operation __tds__GetHostname */
int __tds__GetHostname(struct soap *soap, 
	struct _tds__GetHostname *tds__GetHostname, 
	struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();

	tds__GetHostnameResponse->HostnameInformation = (struct tt__HostnameInformation *)soap_malloc(soap, sizeof(struct tt__HostnameInformation));
	memset(tds__GetHostnameResponse->HostnameInformation, 0x00, sizeof(struct tt__HostnameInformation));

	// GetSysInfo()->lan_config.net.dhcp_enable;
	switch (gtUser.bUseDhcp) {
		case DHCP_USE_ON:
			tds__GetHostnameResponse->HostnameInformation->FromDHCP = xsd__boolean__true_;
			break;
		case DHCP_USE_OFF:
			tds__GetHostnameResponse->HostnameInformation->FromDHCP = xsd__boolean__false_;
			break;
	}
	tds__GetHostnameResponse->HostnameInformation->Name = soap_strdup(soap, oSysInfo->lan_config.hostname);
	tds__GetHostnameResponse->HostnameInformation->Extension = NULL;
	tds__GetHostnameResponse->HostnameInformation->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetHostname);
}


/** Auto-test server operation __tds__SetHostname */
int __tds__SetHostname(struct soap *soap, 
	struct _tds__SetHostname *tds__SetHostname, 
	struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{
	ENTER();

	if(strstr(tds__SetHostname->Name, "_")!=NULL)
	{	
		onvif_fault(soap,"", "ter:InvalidArgVal", "ter:InvalidHostname","Invalid name","<env:Text>hostname is illegality!</env:Text>"); 
		LEAVE();
		return SOAP_FAULT;
	}
	else
	{	
		strcpy(GetSysInfo()->lan_config.hostname, tds__SetHostname->Name);
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetHostnameResponse);
}


/** Auto-test server operation __tds__SetHostnameFromDHCP */
int __tds__SetHostnameFromDHCP(struct soap *soap, 
	struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, 
	struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetHostnameFromDHCP);
	UNUSED(tds__SetHostnameFromDHCPResponse);
}

/**
 * @brief Get DNS  
 * @This operation gets the following DNS settings from a device:
 -FromDHCP
 -Size Search Domain
 -SearchDomain
 -Size DNS From DHCP
 -Size DNS Manual 
 *       
 * @param Request	: _tds__GetDNS structures contains an an empty message.        
 * @param Response	: _tds__GetDNSResponse structure contains DNS credentials. 
 * @return               : On success - SOAP_OK.
 On failure - No command specific faults.
 */
int __tds__GetDNS(struct soap *soap, 
	struct _tds__GetDNS *tds__GetDNS, 
	struct _tds__GetDNSResponse *tds__GetDNSResponse)
{
	ENTER();

	char _dns[25];	
	struct sockaddr_in dns_ip;
	struct _tds__GetDNSResponse *pDns;

	dns_ip.sin_addr.s_addr = GetSysInfo()->lan_config.net.dns.s_addr;	
	strcpy(_dns, inet_ntoa(dns_ip.sin_addr));	
	
	dvprintf("dns %s\r\n",_dns);
	pDns = (struct _tds__GetDNSResponse *)tds__GetDNSResponse;
	
	pDns->DNSInformation = (struct tt__DNSInformation *)soap_malloc(soap, sizeof(struct tt__DNSInformation));
	memset(pDns->DNSInformation, 0x00, sizeof(struct tt__DNSInformation));
	pDns->DNSInformation->FromDHCP = !gtUser.bUseDhcp;//GetSysInfo()->lan_config.net.dhcp_enable;
	pDns->DNSInformation->__sizeSearchDomain = 1;
	dvprintf("dns 1\r\n");

	if(strlen(GetSysInfo()->lan_config.net.domainnameT)>0)
	{
		pDns->DNSInformation->SearchDomain = (char **)soap_malloc(soap, sizeof(char *));
		*(pDns->DNSInformation->SearchDomain) = soap_strdup(soap, GetSysInfo()->lan_config.net.domainnameT);
	}
	else
	{
		pDns->DNSInformation->SearchDomain = NULL;
	}
	dvprintf("dns 2\r\n");
	pDns->DNSInformation->Extension = NULL;
	pDns->DNSInformation->__anyAttribute = NULL;

	if((pDns->DNSInformation->FromDHCP) == 1) 
	{dvprintf("dns 3\r\n");
		pDns->DNSInformation->__sizeDNSFromDHCP = 1;
		pDns->DNSInformation->__sizeDNSManual = 0;
		pDns->DNSInformation->DNSFromDHCP = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		memset(pDns->DNSInformation->DNSFromDHCP, 0x00, sizeof(struct tt__IPAddress));
		pDns->DNSInformation->DNSFromDHCP->Type = 0; 
		dvprintf("dns 4\r\n");
		pDns->DNSInformation->DNSFromDHCP->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		*(pDns->DNSInformation->DNSFromDHCP->IPv4Address) = soap_strdup(soap, _dns);

		pDns->DNSInformation->DNSFromDHCP->IPv6Address = NULL;

		pDns->DNSInformation->DNSManual = NULL;
		dvprintf("dns 5\r\n");
	}
	else 
	{dvprintf("dns 6\r\n");
		pDns->DNSInformation->__sizeDNSFromDHCP = 0;
		pDns->DNSInformation->__sizeDNSManual = 1;
		pDns->DNSInformation->DNSManual = ((struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress)));
		memset(pDns->DNSInformation->DNSManual, 0x00, sizeof(struct tt__IPAddress));
		pDns->DNSInformation->DNSManual->Type = 0; 
		pDns->DNSInformation->DNSManual->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		*(pDns->DNSInformation->DNSManual->IPv4Address) = soap_strdup(soap, _dns);
		dvprintf("dns 7\r\n");
		pDns->DNSInformation->DNSManual->IPv6Address = NULL;
		
		pDns->DNSInformation->DNSFromDHCP = NULL;
	}
	dvprintf("dns e\r\n");

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetDNS);
}


/** Auto-test server operation __tds__SetDNS */
int __tds__SetDNS(struct soap *soap, 
	struct _tds__SetDNS *tds__SetDNS, 
	struct _tds__SetDNSResponse *tds__SetDNSResponse)
{
	ENTER();

	struct sockaddr_in dns_ip;
	int sys_dhcp;
	int value;
	char _IPv4Address[INFO_LENGTH];

	sys_dhcp =  !gtUser.bUseDhcp;
	value = tds__SetDNS->FromDHCP;

	if(tds__SetDNS->SearchDomain)
	{
		strcpy(GetSysInfo()->lan_config.net.domainnameT, *(tds__SetDNS->SearchDomain));
	}
	if (sys_dhcp != value) 
	{
		gtUser.bUseDhcp = !value;//GetSysInfo()->lan_config.net.dhcp_enable 
	}
	if(tds__SetDNS->__sizeSearchDomain)
	{
		tempdnssearchdomainsize = tds__SetDNS->__sizeSearchDomain;
	}
	else
	{
		tempdnssearchdomainsize = 0;
	}

	if(tds__SetDNS->DNSManual == NULL)
	{
		LEAVE();
		return SOAP_OK;
	}

	if(tds__SetDNS->DNSManual->Type != 0) 
	{
		onvif_fault(soap,"","ter:NotSupported", "ter:InvalidIPv6Address","","");
		LEAVE();
		return SOAP_FAULT;
	}	

	strcpy(_IPv4Address, *tds__SetDNS->DNSManual->IPv4Address);
	if(isValidIp4(_IPv4Address) == 0)
	{
		onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidIPv4Address","","");
		LEAVE();
		return SOAP_FAULT;
	}
	inet_aton(_IPv4Address, &dns_ip.sin_addr);
	GetSysInfo()->lan_config.net.dns = dns_ip.sin_addr;

	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetDNSResponse);
}


/** Auto-test server operation __tds__GetNTP */
int __tds__GetNTP(struct soap *soap, 
	struct _tds__GetNTP *tds__GetNTP, 
	struct _tds__GetNTPResponse *tds__GetNTPResponse)
{
	ENTER();

	char *_NTP = GetSysInfo()->lan_config.net.ntp_server;
	int dhcp_status = !gtUser.bUseDhcp;//GetSysInfo()->lan_config.net.dhcp_enable;

	tds__GetNTPResponse->NTPInformation = (struct tt__NTPInformation *)soap_malloc(soap, sizeof(struct tt__NTPInformation));
	memset(tds__GetNTPResponse->NTPInformation, 0x00, sizeof(struct tt__NTPInformation));

	tds__GetNTPResponse->NTPInformation->FromDHCP = dhcp_status;

	if(dhcp_status == 1) //FromDHCP
	{
		tds__GetNTPResponse->NTPInformation->__sizeNTPFromDHCP = 1;
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP = (struct tt__NetworkHost*)soap_malloc(soap, sizeof(struct tt__NetworkHost));
		memset(tds__GetNTPResponse->NTPInformation->NTPFromDHCP, 0x00, sizeof(struct tt__NetworkHost));		
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP->Type = 0;//IPV4_NTP;
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		*tds__GetNTPResponse->NTPInformation->NTPFromDHCP->IPv4Address = soap_strdup(soap, _NTP);

		tds__GetNTPResponse->NTPInformation->NTPFromDHCP->IPv6Address = NULL;
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP->DNSname = NULL;
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP->Extension = NULL;

		tds__GetNTPResponse->NTPInformation->__sizeNTPManual = 0;
		tds__GetNTPResponse->NTPInformation->NTPManual = NULL;
	}
	else // NTPManual || Static
	{
		tds__GetNTPResponse->NTPInformation->__sizeNTPManual = 1;
		tds__GetNTPResponse->NTPInformation->NTPManual = ((struct tt__NetworkHost *)soap_malloc(soap, sizeof(struct tt__NetworkHost)));
		tds__GetNTPResponse->NTPInformation->NTPManual->Type = 0;//IPV4_NTP; 
		tds__GetNTPResponse->NTPInformation->NTPManual->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		tds__GetNTPResponse->NTPInformation->NTPManual->IPv4Address[0] = soap_strdup(soap, _NTP);

		tds__GetNTPResponse->NTPInformation->NTPManual->IPv6Address = NULL;
		tds__GetNTPResponse->NTPInformation->NTPManual->DNSname = NULL;
		tds__GetNTPResponse->NTPInformation->NTPManual->Extension = NULL;

		tds__GetNTPResponse->NTPInformation->__sizeNTPFromDHCP = 0;
		tds__GetNTPResponse->NTPInformation->NTPFromDHCP = NULL;
	}
	
	tds__GetNTPResponse->NTPInformation->Extension = NULL;
	tds__GetNTPResponse->NTPInformation->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetNTP);
}


/** Auto-test server operation __tds__SetNTP */
int __tds__SetNTP(struct soap *soap, 
	struct _tds__SetNTP *tds__SetNTP, 
	struct _tds__SetNTPResponse *tds__SetNTPResponse)
{
	ENTER();

	int exist = NOT_EXIST;

	if (tds__SetNTP->FromDHCP == xsd__boolean__true_) {
		onvif_fault(soap,"","ter:NotSupported", "ter:SetDHCPNotAllowed","","");
		LEAVE();
		return SOAP_FAULT;
	}

	struct tt__NetworkHost *nhNTP = tds__SetNTP->NTPManual;
	switch (nhNTP->Type) {
		case tt__NetworkHostType__IPv4:
			if (nhNTP->IPv4Address != NULL) {
				if (isValidIp4(nhNTP->IPv4Address[0]) != 0) {
					exist = EXIST;
				}
			}

			if (exist == NOT_EXIST) {
				onvifInterfaceFault(soap, ovfSC_InvalidIPv4Address, "");
				LEAVE();
				return SOAP_FAULT;					
			}

			flprintf("Set NTP IPv4 [%s]\r\n", nhNTP->IPv4Address[0]);

			break;

		case tt__NetworkHostType__IPv6:
			onvifInterfaceFault(soap, ovfSC_InvalidIPv6Address, "");
			LEAVE();
			return SOAP_FAULT;

		case tt__NetworkHostType__DNS:
			if (nhNTP->DNSname[0] != NULL) {
				if (isValidHostname(nhNTP->DNSname[0]) != 0) {
					exist = EXIST;
				}
			}

			if (exist == NOT_EXIST) {
				onvifInterfaceFault(soap, ovfSC_InvalidDnsName, "");
				LEAVE();
				return SOAP_FAULT;
			}

			flprintf("Set NTP Host [%s]\r\n", nhNTP->DNSname[0]);

			break;
	}

//	sa.sin_family = AF_INET;
//	inet_aton(tds__SetNTP->NTPManual->IPv4Address[0], &sa.sin_addr.s_addr);
//	ControlSystemData(SFIELD_SET_SNTP_SERVER, (void *)host, strlen(host));	

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__SetNTPResponse);
}


/** Auto-test server operation __tds__GetDynamicDNS */
int __tds__GetDynamicDNS(struct soap *soap, 
	struct _tds__GetDynamicDNS *tds__GetDynamicDNS, 
	struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetDynamicDNS);
	UNUSED(tds__GetDynamicDNSResponse);
}


/** Auto-test server operation __tds__SetDynamicDNS */
int __tds__SetDynamicDNS(struct soap *soap, 
	struct _tds__SetDynamicDNS *tds__SetDynamicDNS, 
	struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetDynamicDNS);
	UNUSED(tds__SetDynamicDNSResponse);
}


/**
 * @brief Get Network Interfaces 
 * @This operation gets the following network interface configuration from a device:
 -Network interface token
 -Name
 -Hardware Id
 -MTU
 -IpAddress
 *   
 * @param Request	: _tds__GetNetworkInterfaces structures contains an empty message.        
 * @param Response	: _tds__GetNetworkInterfacesResponse structure contains an array of device network interfaces.
 * @return               : On success - SOAP_OK.
 : On failure - No command specific faults!

 */
int __tds__GetNetworkInterfaces(struct soap *soap, 
	struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, 
	struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{
	ENTER();

	struct _tds__GetNetworkInterfacesResponse *pNetRp;

	pNetRp = (struct _tds__GetNetworkInterfacesResponse *)tds__GetNetworkInterfacesResponse;
		
	if( pNetRp )
	{
		pNetRp->__sizeNetworkInterfaces = 1;

		char networkToken[8] = ETH_IF_NAME;
		char _mac[100];
		char _IPAddr[70]; 	
		
		sprintf(_mac, TEST_ONVIF_DEVICE_SCOPE_MAC, gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);
//		printf("%s : %s\r\n", __func__, _mac);
		sprintf(_IPAddr, "%s", getIpAddress(HTTP_ONVIF_PORT)); 

		pNetRp->NetworkInterfaces = (struct tt__NetworkInterface *)soap_malloc(soap, sizeof(struct tt__NetworkInterface));
		
		if(pNetRp->NetworkInterfaces)
		{
			memset(pNetRp->NetworkInterfaces, 0x00, sizeof(struct tt__NetworkInterface));	

			/*NetworkToken*/
			pNetRp->NetworkInterfaces->token = soap_strdup(soap, networkToken);

			/*NetworkEnabled*/
			pNetRp->NetworkInterfaces->Enabled = TRUE;

			/*NetworkInterfaceInfo*/
			pNetRp->NetworkInterfaces->Info = (struct tt__NetworkInterfaceInfo *)soap_malloc(soap, sizeof(struct tt__NetworkInterfaceInfo));
			memset(pNetRp->NetworkInterfaces->Info, 0x00, sizeof(struct tt__NetworkInterfaceInfo));	

			pNetRp->NetworkInterfaces->Info->Name = soap_strdup(soap, ETH_IF_NAME);
			pNetRp->NetworkInterfaces->Info->HwAddress = soap_strdup(soap, _mac);
			pNetRp->NetworkInterfaces->Info->MTU = (int *) soap_malloc(soap, sizeof(int)); 
			*(pNetRp->NetworkInterfaces->Info->MTU) = 1500;

			/*NetworkInterfaceLink*/
			pNetRp->NetworkInterfaces->Link = (struct tt__NetworkInterfaceLink*)soap_malloc(soap, sizeof(struct tt__NetworkInterfaceLink));
			memset( pNetRp->NetworkInterfaces->Link, 0x00, sizeof(struct tt__NetworkInterfaceLink));

			pNetRp->NetworkInterfaces->Link->AdminSettings = (struct tt__NetworkInterfaceConnectionSetting *)soap_malloc(soap, sizeof(struct tt__NetworkInterfaceConnectionSetting));
			memset(pNetRp->NetworkInterfaces->Link->AdminSettings, 0x00, sizeof(struct tt__NetworkInterfaceConnectionSetting));
			pNetRp->NetworkInterfaces->Link->AdminSettings->AutoNegotiation = TRUE;
			pNetRp->NetworkInterfaces->Link->AdminSettings->Speed = 100;
			pNetRp->NetworkInterfaces->Link->AdminSettings->Duplex = tt__Duplex__Full;
			
			pNetRp->NetworkInterfaces->Link->OperSettings = (struct tt__NetworkInterfaceConnectionSetting *)soap_malloc(soap, sizeof(struct tt__NetworkInterfaceConnectionSetting));
			memset(pNetRp->NetworkInterfaces->Link->OperSettings, 0x00, sizeof(struct tt__NetworkInterfaceConnectionSetting));
			pNetRp->NetworkInterfaces->Link->OperSettings->AutoNegotiation = TRUE;
			pNetRp->NetworkInterfaces->Link->OperSettings->Speed = 100;
			pNetRp->NetworkInterfaces->Link->OperSettings->Duplex = tt__Duplex__Full;
			
			pNetRp->NetworkInterfaces->Link->InterfaceType = 0;	/* required element of type tt:IANA-IfTypes */


			/*IPv4NetworkInterface*/
			pNetRp->NetworkInterfaces->IPv4 = (struct tt__IPv4NetworkInterface *)soap_malloc(soap, sizeof(struct tt__IPv4NetworkInterface));
			memset(pNetRp->NetworkInterfaces->IPv4, 0x00, sizeof(struct tt__IPv4NetworkInterface));
			pNetRp->NetworkInterfaces->IPv4->Enabled = TRUE;
			
			pNetRp->NetworkInterfaces->IPv4->Config = (struct tt__IPv4Configuration *)soap_malloc(soap, sizeof(struct tt__IPv4Configuration));
			memset(pNetRp->NetworkInterfaces->IPv4->Config, 0x00, sizeof(struct tt__IPv4Configuration));
			pNetRp->NetworkInterfaces->IPv4->Config->__sizeManual = 1;
			
			pNetRp->NetworkInterfaces->IPv4->Config->Manual = (struct tt__PrefixedIPv4Address *)soap_malloc(soap, sizeof(struct tt__PrefixedIPv4Address));
			memset(pNetRp->NetworkInterfaces->IPv4->Config->Manual, 0x00, sizeof(struct tt__PrefixedIPv4Address));
			
			pNetRp->NetworkInterfaces->IPv4->Config->Manual->Address = soap_strdup(soap, _IPAddr);
			pNetRp->NetworkInterfaces->IPv4->Config->Manual->PrefixLength = 16;

			pNetRp->NetworkInterfaces->IPv4->Config->LinkLocal = (struct tt__PrefixedIPv4Address *)soap_malloc(soap, sizeof(struct tt__PrefixedIPv4Address));
			memset(pNetRp->NetworkInterfaces->IPv4->Config->LinkLocal, 0x00, sizeof(struct tt__PrefixedIPv4Address));
			
			pNetRp->NetworkInterfaces->IPv4->Config->LinkLocal->Address = soap_strdup(soap, _IPAddr);
			pNetRp->NetworkInterfaces->IPv4->Config->LinkLocal->PrefixLength = 24;
			
			pNetRp->NetworkInterfaces->IPv4->Config->FromDHCP = (struct tt__PrefixedIPv4Address*)soap_malloc(soap, sizeof(struct tt__PrefixedIPv4Address));	
			memset(pNetRp->NetworkInterfaces->IPv4->Config->FromDHCP, 0x00, sizeof(struct tt__PrefixedIPv4Address));
			pNetRp->NetworkInterfaces->IPv4->Config->FromDHCP->Address = soap_strdup(soap, getIpAddress(HTTP_ONVIF_PORT));
			pNetRp->NetworkInterfaces->IPv4->Config->FromDHCP->PrefixLength = 16;
			
			pNetRp->NetworkInterfaces->IPv4->Config->DHCP = !gtUser.bUseDhcp;//GetSysInfo()->lan_config.net.dhcp_enable;
			pNetRp->NetworkInterfaces->IPv4->Config->__size = 0;
			pNetRp->NetworkInterfaces->IPv4->Config->__any = NULL;
			pNetRp->NetworkInterfaces->IPv4->Config->__anyAttribute = NULL;

			/*IPv6NetworkInterface*/
			pNetRp->NetworkInterfaces->IPv6 = NULL;
			//pNetRp->NetworkInterfaces->IPv6 = (struct tt__IPv6NetworkInterface*)soap_malloc(soap, sizeof(struct tt__IPv6NetworkInterface));
			//memset(pNetRp->NetworkInterfaces->IPv6, 0x00, sizeof(struct tt__IPv6NetworkInterface));
			//pNetRp->NetworkInterfaces->IPv6->Enabled = FALSE;

			/*NetworkInterfaceExtension*/
			pNetRp->NetworkInterfaces->Extension = NULL;	
			pNetRp->NetworkInterfaces->__anyAttribute = NULL;
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetNetworkInterfaces);
}


/** Auto-test server operation __tds__SetNetworkInterfaces */
int __tds__SetNetworkInterfaces(struct soap *soap, 
	struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, 
	struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{
	ENTER();

	ip_addr_t ipaddr, sys_ip;
	int value;
	int ret;
	int local_dhcp_status = !gtUser.bUseDhcp;
	struct _tds__SetNetworkInterfaces *pNetIf;
	struct _tds__SetNetworkInterfacesResponse *pNetIfRp;

	pNetIf = (struct _tds__SetNetworkInterfaces *)tds__SetNetworkInterfaces;
	pNetIfRp = (struct _tds__SetNetworkInterfacesResponse *)tds__SetNetworkInterfacesResponse;
		
	if(pNetIf->InterfaceToken != NULL)
	{
		if(strcmp(pNetIf->InterfaceToken, ETH_IF_NAME))
		{
			onvif_fault(soap,"","ter:InvalidArgVal", "ter:InvalidNetworkInterface","","");
			LEAVE();
			return SOAP_FAULT;
		}
	}

	if(pNetIf->NetworkInterface)
	{
		if(pNetIf->NetworkInterface->Link != NULL)
		{
			onvif_fault(soap,"","ter:InvalidArgVal", "ter:SettingLinkValuesNotSupported","","");
			LEAVE();
			return SOAP_FAULT;
		}
		if(pNetIf->NetworkInterface->IPv6 != NULL)
		{
			if(*pNetIf->NetworkInterface->IPv6->Enabled == xsd__boolean__true_)
			{	
				onvif_fault(soap,"","ter:InvalidArgVal", "ter:IPv6NotSupported","","");
				LEAVE();
				return SOAP_FAULT;
			}
		}	
		if(pNetIf->NetworkInterface->IPv4 == NULL)
		{
			onvif_fault(soap,"","ter:InvalidArgVal", "ter:IPv4ValuesMissing","","");
			LEAVE();
			return SOAP_FAULT;
		}
		if(pNetIf->NetworkInterface->IPv4->Enabled != NULL)
		{
			if(pNetIf->NetworkInterface->IPv4->Manual != NULL)
			{
				if(pNetIf->NetworkInterface->IPv4->Manual->Address != NULL)
				{
					if(isValidIp4(pNetIf->NetworkInterface->IPv4->Manual->Address) == 0) // Check IP address
					{
						onvif_fault(soap,"", "ter:InvalidArgVal", "ter:InvalidIPv4Address","","");
						LEAVE();
						return SOAP_FAULT;
					}
					if (ipaddr_aton(pNetIf->NetworkInterface->IPv4->Manual->Address, &ipaddr) == 0)
					{
						LEAVE();
						return SOAP_FAULT;
					}
				}
				
				if(pNetIf->InterfaceToken != NULL)
				{
					if ((sys_ip.addr = net_get_ifaddr(pNetIf->InterfaceToken)) == -1)
					{
						LEAVE();
						return SOAP_FAULT;
					}
				
					//if (sys_ip.addr != ipaddr.addr)
					//{
						ret = ControlSystemData(SFIELD_SET_IP, (void *)&ipaddr.addr, sizeof(ipaddr.addr));
					//}
				}
			}
			else
			{
				if(pNetIf->NetworkInterface->IPv4->DHCP != NULL)
				{
					if(*(pNetIf->NetworkInterface->IPv4->DHCP))
					{
						//if(local_dhcp_status != 1)
						//{
							//net_enable_dhcpcd();
							
						//}
						value = 1;
					}
					else
					{
						//if(local_dhcp_status != 0)
						//{
							//net_disable_dhcpcd();
						//}
						value = 0;
					}
				}
				if(local_dhcp_status != value)
				{
					ControlSystemData(SFIELD_SET_DHCPC_ENABLE, (void *)&value, sizeof(value));
				}
			}	

		}
	}

	pNetIfRp->RebootNeeded = TRUE;

	LEAVE();
	return SOAP_OK;
}

/**
 * @brief Get Network Protocol 
 * @This operation gets defined network protocols from a device.
 *   
 * @param Request	:_tds__GetNetworkProtocols structure contains an empty message.
 * @param Response	:_tds__GetNetworkProtocols structure members contains an array of defined protocols supported by the device.
 * @return               : On success - SOAP_OK.
 : On failure - No command specific faults

 */
int __tds__GetNetworkProtocols(struct soap *soap, 
	struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, 
	struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{
	ENTER();

	int _http_port;
	int _rtsp_port;
	struct _tds__GetNetworkProtocolsResponse *pNetRp;

	_http_port = GetSysInfo()->lan_config.net._http_port;
	_rtsp_port = GetSysInfo()->lan_config.net._rtsp_port;
	
	pNetRp = (struct _tds__GetNetworkProtocolsResponse *)tds__GetNetworkProtocolsResponse;

	pNetRp->__sizeNetworkProtocols = 2;

	pNetRp->NetworkProtocols = (struct tt__NetworkProtocol *)soap_malloc(soap,pNetRp->__sizeNetworkProtocols * sizeof(struct tt__NetworkProtocol)); 
	memset(pNetRp->NetworkProtocols, 0x00, pNetRp->__sizeNetworkProtocols * sizeof(struct tt__NetworkProtocol));
	pNetRp->NetworkProtocols[0].Name = 0;//HTTP;	// HTTP = 0, HTTPS = 1, RTSP = 2
	pNetRp->NetworkProtocols[0].Enabled = GetSysInfo()->lan_config.net._http_enable;
	pNetRp->NetworkProtocols[0].__sizePort = 1;
	
	pNetRp->NetworkProtocols[0].Port = (int *)soap_malloc(soap, sizeof(int));
	*(pNetRp->NetworkProtocols[0].Port) = _http_port;
	pNetRp->NetworkProtocols[0].Extension = NULL;

	pNetRp->NetworkProtocols[1].Name = 2;//RTSP;	
	pNetRp->NetworkProtocols[1].Enabled = GetSysInfo()->lan_config.net._rtsp_enable;
	pNetRp->NetworkProtocols[1].__sizePort = 1;
	
	pNetRp->NetworkProtocols[1].Port = (int *)soap_malloc(soap, sizeof(int));
	*(pNetRp->NetworkProtocols[1].Port) = _rtsp_port;
	pNetRp->NetworkProtocols[1].Extension = NULL;

	LEAVE();
	return SOAP_OK;
	
	UNUSED(tds__GetNetworkProtocols);
}


/** Auto-test server operation __tds__SetNetworkProtocols */
int __tds__SetNetworkProtocols(struct soap *soap, 
	struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, 
	struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{
	ENTER();

	if(tds__SetNetworkProtocols->__sizeNetworkProtocols)
	{
		if(tds__SetNetworkProtocols->NetworkProtocols->Name == 0)
		{
			GetSysInfo()->lan_config.net._http_port = *tds__SetNetworkProtocols->NetworkProtocols->Port;
			GetSysInfo()->lan_config.net._http_enable = tds__SetNetworkProtocols->NetworkProtocols->Enabled;
		}
		if(tds__SetNetworkProtocols->NetworkProtocols->Name == 2)
		{
			GetSysInfo()->lan_config.net._rtsp_port	=  *tds__SetNetworkProtocols->NetworkProtocols->Port;
			GetSysInfo()->lan_config.net._rtsp_enable = tds__SetNetworkProtocols->NetworkProtocols->Enabled;
		}
		if(tds__SetNetworkProtocols->NetworkProtocols->Name == 1)
		{
			onvif_fault(soap, "","ter:InvalidArgVal", "ter:ServiceNotSupported", NULL, NULL);
			LEAVE();
			return SOAP_FAULT;
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__SetNetworkProtocolsResponse);
}


/** Auto-test server operation __tds__GetNetworkDefaultGateway */
int __tds__GetNetworkDefaultGateway(struct soap *soap, 
	struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, 
	struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{
	ENTER();

	struct sockaddr_in gateway_ip;
	char _GatewayAddress[LARGE_INFO_LENGTH];

	if (tds__GetNetworkDefaultGatewayResponse == NULL) {
		LEAVE();
		return SOAP_FAULT;
	}

	gateway_ip.sin_addr.s_addr = GetSysInfo()->lan_config.net.gateway.s_addr;	
	strcpy(_GatewayAddress, inet_ntoa(gateway_ip.sin_addr));	
	dvprintf("_GatewayAddress %s\r\n",_GatewayAddress);
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway = (struct tt__NetworkGateway *)soap_malloc(soap,sizeof(struct tt__NetworkGateway));
	memset(tds__GetNetworkDefaultGatewayResponse->NetworkGateway, 0x00, sizeof(struct tt__NetworkGateway));
	
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway->__sizeIPv4Address = 1;
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway->__sizeIPv6Address = 0;
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));	
	*(tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv4Address) = soap_strdup(soap, _GatewayAddress);
	tds__GetNetworkDefaultGatewayResponse->NetworkGateway->IPv6Address = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetNetworkDefaultGateway);
}


/** Auto-test server operation __tds__SetNetworkDefaultGateway */
int __tds__SetNetworkDefaultGateway(struct soap *soap, 
	struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, 
	struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{
	ENTER();

	if(tds__SetNetworkDefaultGateway->__sizeIPv4Address)
	{
		if(tds__SetNetworkDefaultGateway->IPv4Address)
		{
			struct sockaddr_in dns_ip;
			inet_aton(*tds__SetNetworkDefaultGateway->IPv4Address, &dns_ip.sin_addr);
			GetSysInfo()->lan_config.net.gateway = dns_ip.sin_addr;
		}
	}

	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetNetworkDefaultGatewayResponse);
}


/** Auto-test server operation __tds__GetZeroConfiguration */
int __tds__GetZeroConfiguration(struct soap *soap, 
	struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, 
	struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse)
{
	ENTER();

	char _IPAddr[SMALL_INFO_LENGTH] = {0};	
	sprintf(_IPAddr, "%s", getIpAddress(HTTP_ONVIF_PORT));

	tds__GetZeroConfigurationResponse->ZeroConfiguration = (struct tt__NetworkZeroConfiguration *) soap_malloc(soap, sizeof(struct tt__NetworkZeroConfiguration));
	memset(tds__GetZeroConfigurationResponse->ZeroConfiguration, 0x00, sizeof(struct tt__NetworkZeroConfiguration));
	struct tt__NetworkZeroConfiguration *zeroCfg = tds__GetZeroConfigurationResponse->ZeroConfiguration;

	zeroCfg->InterfaceToken = soap_strdup(soap, ETH_IF_NAME);
	zeroCfg->Enabled = 1;

	zeroCfg->__sizeAddresses = 2;
	zeroCfg->Addresses = (char **)soap_malloc(soap, sizeof(char *) * zeroCfg->__sizeAddresses);
	zeroCfg->Addresses[0] = soap_strdup(soap, _IPAddr);
	zeroCfg->Addresses[1] = soap_strdup(soap, _IPAddr);

	zeroCfg->Extension = NULL;
	zeroCfg->__anyAttribute = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__GetZeroConfiguration);
}


/** Auto-test server operation __tds__SetZeroConfiguration */
int __tds__SetZeroConfiguration(struct soap *soap, 
	struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, 
	struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetZeroConfiguration);
	UNUSED(tds__SetZeroConfigurationResponse);
}


/** Auto-test server operation __tds__GetIPAddressFilter */
int __tds__GetIPAddressFilter(struct soap *soap, 
	struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, 
	struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetIPAddressFilter);
	UNUSED(tds__GetIPAddressFilterResponse);
}


/** Auto-test server operation __tds__SetIPAddressFilter */
int __tds__SetIPAddressFilter(struct soap *soap, 
	struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, 
	struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetIPAddressFilter);
	UNUSED(tds__SetIPAddressFilterResponse);
}


/** Auto-test server operation __tds__AddIPAddressFilter */
int __tds__AddIPAddressFilter(struct soap *soap, 
	struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter,
	struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__AddIPAddressFilter);
	UNUSED(tds__AddIPAddressFilterResponse);
}


/** Auto-test server operation __tds__RemoveIPAddressFilter */
int __tds__RemoveIPAddressFilter(struct soap *soap, 
	struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter,
	struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__RemoveIPAddressFilter);
	UNUSED(tds__RemoveIPAddressFilterResponse);
}


/** Auto-test server operation __tds__GetAccessPolicy */
int __tds__GetAccessPolicy(struct soap *soap, 
	struct _tds__GetAccessPolicy *tds__GetAccessPolicy, 
	struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetAccessPolicy);
	UNUSED(tds__GetAccessPolicyResponse);
}


/** Auto-test server operation __tds__SetAccessPolicy */
int __tds__SetAccessPolicy(struct soap *soap, 
	struct _tds__SetAccessPolicy *tds__SetAccessPolicy, 
	struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetAccessPolicy);
	UNUSED(tds__SetAccessPolicyResponse);
}


/** Auto-test server operation __tds__CreateCertificate */
int __tds__CreateCertificate(struct soap *soap,
	struct _tds__CreateCertificate *tds__CreateCertificate,
	struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__CreateCertificate);
	UNUSED(tds__CreateCertificateResponse);
}


/** Auto-test server operation __tds__GetCertificates */
int __tds__GetCertificates(struct soap *soap, 
	struct _tds__GetCertificates *tds__GetCertificates, 
	struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetCertificates);
	UNUSED(tds__GetCertificatesResponse);
}


/** Auto-test server operation __tds__GetCertificatesStatus */
int __tds__GetCertificatesStatus(struct soap *soap, 
	struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, 
	struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetCertificatesStatus);
	UNUSED(tds__GetCertificatesStatusResponse);
}


/** Auto-test server operation __tds__SetCertificatesStatus */
int __tds__SetCertificatesStatus(struct soap *soap, 
	struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, 
	struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetCertificatesStatus);
	UNUSED(tds__SetCertificatesStatusResponse);
}


/** Auto-test server operation __tds__DeleteCertificates */
int __tds__DeleteCertificates(struct soap *soap, 
	struct _tds__DeleteCertificates *tds__DeleteCertificates, 
	struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__DeleteCertificates);
	UNUSED(tds__DeleteCertificatesResponse);
}


/** Auto-test server operation __tds__GetPkcs10Request */
int __tds__GetPkcs10Request(struct soap *soap, 
	struct _tds__GetPkcs10Request *tds__GetPkcs10Request, 
	struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetPkcs10Request);
	UNUSED(tds__GetPkcs10RequestResponse);
}


/** Auto-test server operation __tds__LoadCertificates */
int __tds__LoadCertificates(struct soap *soap, 
	struct _tds__LoadCertificates *tds__LoadCertificates, 
	struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__LoadCertificates);
	UNUSED(tds__LoadCertificatesResponse);
}


/** Auto-test server operation __tds__GetClientCertificateMode */
int __tds__GetClientCertificateMode(struct soap *soap, 
	struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, 
	struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetClientCertificateMode);
	UNUSED(tds__GetClientCertificateModeResponse);
}


/** Auto-test server operation __tds__SetClientCertificateMode */
int __tds__SetClientCertificateMode(struct soap *soap, 
	struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, 
	struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetClientCertificateMode);
	UNUSED(tds__SetClientCertificateModeResponse);
}


/** Auto-test server operation __tds__GetRelayOutputs */
int __tds__GetRelayOutputs(struct soap *soap, 
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


/** Auto-test server operation __tds__SetRelayOutputSettings */
int __tds__SetRelayOutputSettings(struct soap *soap, 
	struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings,
	struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetRelayOutputSettings);
	UNUSED(tds__SetRelayOutputSettingsResponse);
}


/** Auto-test server operation __tds__SetRelayOutputState */
int __tds__SetRelayOutputState(struct soap *soap, 
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


/** Auto-test server operation __tds__SendAuxiliaryCommand */
int __tds__SendAuxiliaryCommand(struct soap *soap, 
	struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, 
	struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SendAuxiliaryCommand);
	UNUSED(tds__SendAuxiliaryCommandResponse);
}


/** Auto-test server operation __tds__GetCACertificates */
int __tds__GetCACertificates(struct soap *soap, 
	struct _tds__GetCACertificates *tds__GetCACertificates, 
	struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetCACertificates);
	UNUSED(tds__GetCACertificatesResponse);
}


/** Auto-test server operation __tds__LoadCertificateWithPrivateKey */
int __tds__LoadCertificateWithPrivateKey(struct soap *soap,
	struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey,
	struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__LoadCertificateWithPrivateKey);
	UNUSED(tds__LoadCertificateWithPrivateKeyResponse);
}


/** Auto-test server operation __tds__GetCertificateInformation */
int __tds__GetCertificateInformation(struct soap *soap,
	struct _tds__GetCertificateInformation *tds__GetCertificateInformation,
	struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetCertificateInformation);
	UNUSED(tds__GetCertificateInformationResponse);
}


/** Auto-test server operation __tds__LoadCACertificates */
int __tds__LoadCACertificates(struct soap *soap, 
	struct _tds__LoadCACertificates *tds__LoadCACertificates,
	struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__LoadCACertificates);
	UNUSED(tds__LoadCACertificatesResponse);
}


/** Auto-test server operation __tds__CreateDot1XConfiguration */
int __tds__CreateDot1XConfiguration(struct soap *soap,
	struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration,
	struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__CreateDot1XConfiguration);
	UNUSED(tds__CreateDot1XConfigurationResponse);
}


/** Auto-test server operation __tds__SetDot1XConfiguration */
int __tds__SetDot1XConfiguration(struct soap *soap, 
	struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, 
	struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__SetDot1XConfiguration);
	UNUSED(tds__SetDot1XConfigurationResponse);
}


/** Auto-test server operation __tds__GetDot1XConfiguration */
int __tds__GetDot1XConfiguration(struct soap *soap,
	struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, 
	struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetDot1XConfiguration);
	UNUSED(tds__GetDot1XConfigurationResponse);
}


/** Auto-test server operation __tds__GetDot1XConfigurations */
int __tds__GetDot1XConfigurations(struct soap *soap, 
	struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, 
	struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetDot1XConfigurations);
	UNUSED(tds__GetDot1XConfigurationsResponse);
}


/** Auto-test server operation __tds__DeleteDot1XConfiguration */
int __tds__DeleteDot1XConfiguration(struct soap *soap, 
	struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, 
	struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__DeleteDot1XConfiguration);
	UNUSED(tds__DeleteDot1XConfigurationResponse);
}


/** Auto-test server operation __tds__GetDot11Capabilities */
int __tds__GetDot11Capabilities(struct soap *soap, 
	struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, 
	struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetDot11Capabilities);
	UNUSED(tds__GetDot11CapabilitiesResponse);
}


/** Auto-test server operation __tds__GetDot11Status */
int __tds__GetDot11Status(struct soap *soap, 
	struct _tds__GetDot11Status *tds__GetDot11Status,
	struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetDot11Status);
	UNUSED(tds__GetDot11StatusResponse);
}


/** Auto-test server operation __tds__ScanAvailableDot11Networks */
int __tds__ScanAvailableDot11Networks(struct soap *soap, 
	struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks,
	struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__ScanAvailableDot11Networks);
	UNUSED(tds__ScanAvailableDot11NetworksResponse);
}


/** Auto-test server operation __tds__GetSystemUris */
int __tds__GetSystemUris(struct soap *soap, 
	struct _tds__GetSystemUris *tds__GetSystemUris, 
	struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse)
{
	ENTER();
	
	tds__GetSystemUrisResponse->SystemLogUris = (struct tt__SystemLogUriList *)soap_malloc(soap, sizeof(struct tt__SystemLogUriList));	
	memset(tds__GetSystemUrisResponse->SystemLogUris, 0x00, sizeof(struct tt__SystemLogUriList));
	
	tds__GetSystemUrisResponse->SystemLogUris->__sizeSystemLog = 0;
	tds__GetSystemUrisResponse->SystemLogUris->SystemLog = NULL;

	tds__GetSystemUrisResponse->SupportInfoUri = NULL;
	tds__GetSystemUrisResponse->SystemBackupUri = NULL;
	tds__GetSystemUrisResponse->Extension = NULL;

	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__GetSystemUris);
}


/** Auto-test server operation __tds__StartFirmwareUpgrade */
int __tds__StartFirmwareUpgrade(struct soap *soap, 
	struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, 
	struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse)
{
	ENTER();

	char uri[64] = {0};
	snprintf(uri, 63, "http://%s/appupdate2.cgi", getIpAddress(HTTP_ONVIF_PORT));

	tds__StartFirmwareUpgradeResponse->UploadUri = soap_strdup(soap, uri);
	tds__StartFirmwareUpgradeResponse->UploadDelay = 60;
	tds__StartFirmwareUpgradeResponse->ExpectedDownTime = 60;

	LEAVE();
	return SOAP_OK;

	UNUSED(tds__StartFirmwareUpgrade);
}


/** Auto-test server operation __tds__StartSystemRestore */
int __tds__StartSystemRestore(struct soap *soap, 
	struct _tds__StartSystemRestore *tds__StartSystemRestore, 
	struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tds__StartSystemRestore);
	UNUSED(tds__StartSystemRestoreResponse);
}
