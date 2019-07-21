
#include <stdio.h>

#include "soapH.h"
#include "stdsoap2.h"
#include "enx_gsoap_lwip.h"
#include "FreeRTOS.h" 

#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "dev.h"
#include "msg.h"
#include "onvifProfile.h"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#define CALL()  printf("CALL%04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()
#define LEAVE()
#define CALL()
#endif

int  __wsdd__Hello(struct soap* soap, 
	struct wsdd__HelloType *wsdd__Hello)
{
	CALL();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsdd__Hello);
}

int  __wsdd__Bye(struct soap* soap, 
	struct wsdd__ByeType *wsdd__Bye)
{
	CALL();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsdd__Bye);
}

int  
__wsdd__Probe(struct soap* soap, 
	struct wsdd__ProbeType *wsdd__Probe)
{
	ENTER();

	SysInfo* oSysInfo = GetSysInfo();
	int i;

	if(oSysInfo->lan_config.net.discovery_mode) {
		LEAVE();
		return SOAP_FAULT;
	}

	char _IPAddr[60];
	char _HwId[100]; 

	sprintf(_HwId, TEST_ONVIF_DEVICE_SCOPE_UUID, gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);
	sprintf(_IPAddr, "http://%s/onvif/device_service", getIpAddress(HTTP_ONVIF_PORT));

	//   char *items = "onvif://www.onvif.org/type/video_encoder "\
	//                                 "onvif://www.onvif.org/type/NetworkVideoTransmitter "\
	//                                  "onvif://www.onvif.org/Profile/Streaming "\
	//                                  "onvif://www.onvif.org/hardware/EN673 "\
	//                                  "onvif://www.onvif.org/name/EN673_ENTRY "\
	//                                  "onvif://www.onvif.org/location/country/korea";
	char items[600];
	memset(items, 0x00, sizeof(char) * 600);

	for (i = 0; i < oSysInfo->nScope; i++) {
		if (i == 0 && strlen(oSysInfo->Scope[i].str) > 0) {
		   strcpy(items, oSysInfo->Scope[i].str);
		} else if(strlen(oSysInfo->Scope[i].str)>0) {
		   strcat(items, " ");
		   strcat(items, oSysInfo->Scope[i].str);
		}
	}

	if (wsdd__Probe->Scopes) {
		if (wsdd__Probe->Scopes->__item && strlen(wsdd__Probe->Scopes->__item) > 5) {
			printf("wsdd__Probe->Scopes->__item %s\r\n",wsdd__Probe->Scopes->__item);
			if (  strstr(wsdd__Probe->Scopes->__item,"onvif://www.onvif.org/type")
				||strstr(wsdd__Probe->Scopes->__item,"onvif://www.onvif.org/Profile")
				||strstr(wsdd__Probe->Scopes->__item,"onvif://www.onvif.org/hardware")
				||strstr(wsdd__Probe->Scopes->__item,"onvif://www.onvif.org/name")
				||strstr(wsdd__Probe->Scopes->__item,"onvif://www.onvif.org/location")
			) {
				printf("default\r\n");
 			} else if (strstr(items, wsdd__Probe->Scopes->__item) == NULL) {
 				LEAVE();
				return SOAP_FAULT;
			}
		}

		if (wsdd__Probe->Scopes->MatchBy) {
			printf("wsdd__Probe->Scopes->MatchBy %s\r\n", wsdd__Probe->Scopes->MatchBy);
		}
	}

	wsdd__ProbeMatchesType ProbeMatches;
	ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchType));
	memset(ProbeMatches.ProbeMatch, 0x00, sizeof(struct wsdd__ProbeMatchType));

	ProbeMatches.ProbeMatch->XAddrs = soap_strdup(soap, _IPAddr);
	ProbeMatches.ProbeMatch->Types = soap_strdup(soap, ONVIF_DEVICE_SCOPE_TRANSMITTER);

	ProbeMatches.ProbeMatch->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap,sizeof(struct wsdd__ScopesType));
	memset(ProbeMatches.ProbeMatch->Scopes, 0x00, sizeof(struct wsdd__ScopesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap, sizeof(struct wsa__ReferencePropertiesType));
	memset(ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties, 0x00, sizeof(struct wsa__ReferencePropertiesType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap, sizeof(struct wsa__ReferenceParametersType));
	memset(ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters, 0x00, sizeof(struct wsa__ReferenceParametersType));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap, sizeof(struct wsa__ServiceNameType));
	memset(ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName, 0x00, sizeof(struct wsa__ServiceNameType));

	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = soap_strdup(soap, "ttl");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*));
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = soap_strdup(soap, "Any");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute = soap_strdup(soap, "Attribute");
	ProbeMatches.ProbeMatch->wsa__EndpointReference.Address = soap_strdup(soap, _HwId);

	//ws-discovery
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;

	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
	ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;

	ProbeMatches.__sizeProbeMatch = 1;
	ProbeMatches.ProbeMatch->Scopes->__item = soap_strdup(soap, items);

	ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;
	ProbeMatches.ProbeMatch->MetadataVersion = 10;

	soap->header->wsa__To = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous");

	soap->header->wsa__Action = soap_strdup(soap, "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches");

	soap->header->wsa__RelatesTo = (struct wsa__Relationship*)soap_malloc(soap, sizeof(struct wsa__Relationship));
	memset(soap->header->wsa__RelatesTo,0x00, sizeof(struct wsa__Relationship));
	soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
	soap->header->wsa__RelatesTo->RelationshipType = NULL;
	soap->header->wsa__RelatesTo->__anyAttribute = NULL;

	soap->header->wsa__MessageID = soap_strdup(soap, _HwId);
	soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches);

	LEAVE();
	return SOAP_OK;
}

int  
__wsdd__ProbeMatches(struct soap* soap, 
	struct wsdd__ProbeMatchesType *wsdd__ProbeMatches)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsdd__ProbeMatches);
}

int  
__wsdd__Resolve(struct soap* soap, 
	struct wsdd__ResolveType *wsdd__Resolve)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsdd__Resolve);
}

int  
__wsdd__ResolveMatches(struct soap* soap, 
	struct wsdd__ResolveMatchesType *wsdd__ResolveMatches)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(wsdd__ResolveMatches);
}

int  
__dnrd__Hello(struct soap* soap, 
	struct wsdd__HelloType tdn__Hello, 
	struct wsdd__ResolveType *tdn__HelloResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tdn__Hello);
	UNUSED(tdn__HelloResponse);
}

int  
__dnrd__Bye(struct soap* soap, 
	struct wsdd__ByeType tdn__Bye, 
	struct wsdd__ResolveType *tdn__ByeResponse)
{
	ENTER();
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tdn__Bye);
	UNUSED(tdn__ByeResponse);
}

int  
__dndl__Probe(struct soap* soap, 
	struct wsdd__ProbeType tdn__Probe, 
	struct wsdd__ProbeMatchesType *tdn__ProbeResponse)
{
	ENTER();

#if 0 

	char _IPAddr[INFO_LENGTH];
	char _HwId[LARGE_INFO_LENGTH];
	
	int i = 0;
	int flag = 0;
	char list[20][100];
	int len = 0;
	int index = 0;
	
	if(tdn__Probe.Scopes != NULL)
	{
		if(strcmp(tdn__Probe.Scopes->__item, "") == 0)
		{
			flag = 1;
		}
		else
		{
			explodeitem(tdn__Probe.Scopes->__item, ' ', list);

			
			for(i = 0; **(list + i) != 0; i++)
			{
				len = strlen(ONVIF_DEVICE_SCOPE_PROFILE);
				if(!strncmp(list[i], ONVIF_DEVICE_SCOPE_PROFILE, len))
				{
					if(list[i][len] == 0)
					{
						flag = 1;
					}
					else if(!strcmp((*(list + i) /*+ len*/), GetScopes()->profile))
					{
						flag = 1;
					}
				}

				
				len = strlen(ONVIF_DEVICE_SCOPE_TYPE);
				if(!strncmp(list[i], ONVIF_DEVICE_SCOPE_TYPE, len))
				{
					if(list[i][len] == 0)
					{
						flag = 1;
					}
					else
					{
						index = 0;
						for(index =0; index<MAX_SCOPE_TYPE; index++)
						{
							if(GetScopes()->type[index][0])
							{
								if(!strcmp((*(list + i) /*+ len*/), GetScopes()->type[index]))
								{
									flag = 1;
									break;
								}
								index++;
							}
						}
					}
				}
				len = strlen(ONVIF_DEVICE_SCOPE_LOCATION);
				if(!strncmp(list[i], ONVIF_DEVICE_SCOPE_LOCATION, len))
				{
					if(list[i][len] == 0)
					{
						flag = 1;
					}
					else
					{
						index = 0;
						for(index =0; index<MAX_SCOPE_TYPE; index++)
						{
							if(GetScopes()->location[index][0])
							{
								if(!strcmp((*(list + i) /*+ len*/), GetScopes()->location[index]))
								{
									flag = 1;
									break;
								}
								index++;
							}
						}
					}
				}
				len = strlen(ONVIF_DEVICE_SCOPE_HARDWARE);
				if(!strncmp(list[i], ONVIF_DEVICE_SCOPE_HARDWARE, len))
				{
					if(list[i][len] == 0)
					{
						flag = 1;
					}
					else if(!strcmp((*(list + i) /*+ len*/), GetScopes()->hardware))
					{
						flag = 1;
					}
				}
				len = strlen(ONVIF_DEVICE_SCOPE_NAME);
				if(!strncmp(list[i], ONVIF_DEVICE_SCOPE_NAME, len))
				{
					if(list[i][len] == 0)
					{
						flag = 1;
					}
					else if(!strcmp((*(list + i) /*+ len*/), GetScopes()->name))
					{
						flag = 1;
					}
				}

			}
		}
	}
	else
	{
		flag = 1;
	}


	if(flag == 1)
	{
		tdn__ProbeResponse->ProbeMatch = (struct wsdd__ProbeMatchesType *)soap_malloc(soap, sizeof(struct wsdd__ProbeMatchesType));
		memset(tdn__ProbeResponse->ProbeMatch, 0x00, sizeof(struct wsdd__ProbeMatchesType));
		tdn__ProbeResponse->ProbeMatch->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap,sizeof(struct wsdd__ScopesType));
		memset(tdn__ProbeResponse->ProbeMatch->Scopes, 0x00, sizeof(struct wsdd__ScopesType));
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap,sizeof(struct wsa__ReferencePropertiesType));
		memset(tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceProperties, 0x00, sizeof(struct wsa__ReferencePropertiesType));
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap,sizeof(struct wsa__ReferenceParametersType));
		memset(tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceParameters, 0x00, sizeof(struct wsa__ReferenceParametersType));
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap,sizeof(struct wsa__ServiceNameType));
		memset(tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ServiceName, 0x00, sizeof(struct wsa__ServiceNameType));
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *));
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*));

		tdn__ProbeResponse->__sizeProbeMatch = 1;
 		tdn__ProbeResponse->ProbeMatch->Scopes->__item = (char*) soap_malloc(soap, sizeof(char)*100);
		
		while(strstr(GetScopes()->type[i], DEFAULT_SCOPE) != NULL)
		{
			strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, GetScopes()->type[i]);
			strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, " ");
			i++;
		}
		
		i = 0;
		strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, GetScopes()->name);
		strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, " ");
		strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, GetScopes()->hardware);
		strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, " ");
		
		while(strstr(GetScopes()->location[i], DEFAULT_SCOPE) != NULL)
		{
			strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, GetScopes()->location[i]);
			strcat(tdn__ProbeResponse->ProbeMatch->Scopes->__item, " ");
			i++;
		}

		tdn__ProbeResponse->ProbeMatch->Scopes->MatchBy = NULL;
		sprintf(_IPAddr, DEVICE_SERVICE_ADDRESS, getIpAddress(HTTP_ONVIF_PORT));
		
		tdn__ProbeResponse->ProbeMatch->XAddrs = soap_strdup(soap,  _IPAddr);
		tdn__ProbeResponse->ProbeMatch->Types = soap_strdup(soap, tdn__Probe.Types); 
		tdn__ProbeResponse->ProbeMatch->MetadataVersion = 1;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;
		
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.PortType[0] = soap_strdup(soap, "ttl");

		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
		
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.__any[0] = soap_strdup(soap, "Any");		
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.__anyAttribute = soap_strdup(soap, "Attribute");
	
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.__size = 0;
		
		
		sprintf(_HwId,TEST_ONVIF_DEVICE_SCOPE_UUID,gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);
		
		tdn__ProbeResponse->ProbeMatch->wsa__EndpointReference.Address = soap_strdup(soap, _HwId);
		
	}
	else
	{
		onvif_fault(soap,"","ter:InvArgs","ter:InvArgs","","");
		return SOAP_FAULT;
	}	
#endif
	LEAVE();
	return SOAP_OK;

	UNUSED(soap);
	UNUSED(tdn__Probe);
	UNUSED(tdn__ProbeResponse);
}



wsdd__HelloType*
get__wsdd__Hello(struct soap *soap)
{
	ENTER();

	wsdd__HelloType *wsdd__Hello = NULL;

	int i;
	char _IPAddr[INFO_LENGTH];
	char _HwId[200];

	sprintf(_IPAddr, DEVICE_SERVICE_ADDRESS, getIpAddress(HTTP_ONVIF_PORT));
	
	sprintf(_HwId,TEST_ONVIF_DEVICE_SCOPE_UUID,gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);
	
//	char *items = "onvif://www.onvif.org/type/video_encoder "\
//				"onvif://www.onvif.org/type/NetworkVideoTransmitter "\
//				"onvif://www.onvif.org/Profile/Streaming "\
//				"onvif://www.onvif.org/hardware/EN673 "\
//				"onvif://www.onvif.org/name/EN673_ENTRY "\
//				"onvif://www.onvif.org/location/country/korea";
	char items[600];
	memset(items,0x00, sizeof(char)*600);
	
	for(i=0;i<GetSysInfo()->nScope;i++)
	{
		if(i==0 && strlen(GetSysInfo()->Scope[i].str)>0)
		{ 
		   strcpy(items, GetSysInfo()->Scope[i].str); 
		}
		else if(strlen(GetSysInfo()->Scope[i].str)>0)
		{ 
		   strcat(items, " ");
		   strcat(items, GetSysInfo()->Scope[i].str); 
		}		
	}

	wsdd__Hello = (struct wsdd__HelloType *)soap_malloc(soap, sizeof(struct wsdd__HelloType));
	
	wsdd__Hello->XAddrs = soap_strdup(soap, _IPAddr);
	wsdd__Hello->Types = soap_strdup(soap, ONVIF_DEVICE_SCOPE_TRANSMITTER);

	wsdd__Hello->Scopes = (struct wsdd__ScopesType*)soap_malloc(soap,sizeof(struct wsdd__ScopesType));
	wsdd__Hello->wsa__EndpointReference.ReferenceProperties = (struct wsa__ReferencePropertiesType*)soap_malloc(soap,sizeof(struct wsa__ReferencePropertiesType));
	wsdd__Hello->wsa__EndpointReference.ReferenceParameters = (struct wsa__ReferenceParametersType*)soap_malloc(soap,sizeof(struct wsa__ReferenceParametersType));
	wsdd__Hello->wsa__EndpointReference.ServiceName = (struct wsa__ServiceNameType*)soap_malloc(soap,sizeof(struct wsa__ServiceNameType));

	wsdd__Hello->wsa__EndpointReference.PortType = (char **)soap_malloc(soap, sizeof(char *));
	wsdd__Hello->wsa__EndpointReference.PortType[0] = soap_strdup(soap, "ttl");
	wsdd__Hello->wsa__EndpointReference.__any = (char **)soap_malloc(soap, sizeof(char*));
	wsdd__Hello->wsa__EndpointReference.__any[0] = soap_strdup(soap, "Any");
	wsdd__Hello->wsa__EndpointReference.__anyAttribute = soap_strdup(soap, "Attribute");
	wsdd__Hello->wsa__EndpointReference.Address = soap_strdup(soap, _HwId);
  	wsdd__Hello->Scopes->__item = soap_strdup(soap, items);
	wsdd__Hello->Scopes->MatchBy = NULL;	
	wsdd__Hello->MetadataVersion = 10;
	
	//ws-discovery
	wsdd__Hello->wsa__EndpointReference.ReferenceProperties->__size = 0;
	wsdd__Hello->wsa__EndpointReference.ReferenceProperties->__any = NULL;
	wsdd__Hello->wsa__EndpointReference.ReferenceParameters->__size = 0;
	wsdd__Hello->wsa__EndpointReference.ReferenceParameters->__any = NULL;

	wsdd__Hello->wsa__EndpointReference.ServiceName->__item = NULL;
	wsdd__Hello->wsa__EndpointReference.ServiceName->PortName = NULL;
	wsdd__Hello->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
	wsdd__Hello->wsa__EndpointReference.__size = 0;

	LEAVE();
	return wsdd__Hello;
}

wsdd__ByeType*
get__wsdd__Bye(struct soap *soap)
{
	ENTER();

	wsdd__ByeType *wsdd__Bye = NULL;
	char _HwId[200];

	sprintf(_HwId, TEST_ONVIF_DEVICE_SCOPE_UUID,
		gptMsgShare.ETH_MAC[0], gptMsgShare.ETH_MAC[1], gptMsgShare.ETH_MAC[2], 
		gptMsgShare.ETH_MAC[3], gptMsgShare.ETH_MAC[4], gptMsgShare.ETH_MAC[5]);

	wsdd__Bye = (struct wsdd__ByeType *)soap_malloc(soap, sizeof(struct wsdd__ByeType));
	memset(wsdd__Bye, 0x00, sizeof(struct wsdd__ByeType));

	wsdd__Bye->wsa__EndpointReference.Address = soap_strdup(soap, _HwId);

	LEAVE();
	return wsdd__Bye;
}

