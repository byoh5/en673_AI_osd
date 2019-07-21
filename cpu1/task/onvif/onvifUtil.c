#include "dev.h"
#include "dev_inline.h"

#include "onvifSystemInfo.h"
#include "onvifUtil.h"
#include "user.h"
#include <stdio.h>
//#include "vsprintf.h"

#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/autoip.h"

#ifdef __WIFI__
#include "wifi.h"
#endif

extern struct netif *netif_list;

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

/*************************UTIL FUNCTIONS*************************/
/**
 * @brief explodeitem
 * @This function splits the given string based on a delimiter.
    'list' is an array which holds the split strings
  */


void explodeitem(char *in, char dl, char list[][100])
{
	int i = 0;
	int j = 0;
	int k = 0;
	int len = strlen(in);
	for(i = 0; i < len; i++)
	{
		if(in[i] == dl)
		{
			
			*( *(list + j) + k) = 0;
			j++;
			k = 0;
		}
		else
		{
			*( *(list + j) + k) = in[i];
			k++;
		}
	}
	*(*(list + j) + k) = 0;
	j++;
	**(list + j) = 0;
}


/* @brief Parse the scope URI and return the scope parameter */
char *strparser(char *str)
{
	ENTER();

	int i = 0;
	int j = 0;
//	char *retstr = pvPortMalloc(sizeof(char) * INFO_LENGTH);
	static char retstr[INFO_LENGTH] = {0};
	while(i < 3)
	{
		if(*str == '/')
		{
			i++;
		}
		str++;
	}
	while(*str != '/' && *str != '\0')
	{
		*(retstr + j) = *str;
		j++;
		str++;
	}
	*(retstr + j) = '\0';
	if(((strcmp(retstr, "type") == 0)) || (strcmp(retstr, "name") == 0) || (strcmp(retstr, "location") == 0) || (strcmp(retstr, "hardware") == 0))
	{
		LEAVE();
		return retstr;
	}
	else
	{
		LEAVE();
		return NULL;
	}
}

/*
 *  @brief Parsing the string for Motion and Facedetection parameters 
 *  @return Parameters of Motion and FD parameters
 */
char *anyparser(char *str)
{
	ENTER();

//	char *retstr = pvPortMalloc(sizeof(char)* INFO_LENGTH);
	static char retstr[INFO_LENGTH] = {0};

	while (*str != '>') {
		str++;
	}

	if (strstr(str,"wsnt:VideoAnalytics/wsnt:MotionDetection/") != NULL) {
		LEAVE();
		return str;
	}
	else if (strstr(str,"wsnt:VideoAnalytics/wsnt:FaceDetection/") != NULL) {
		LEAVE();
		return str;
	}
// <wsnt:TopicExpression xmlns="http://docs.oasis-open.org/wsn/b-2" xmlns:tns1="http://www.onvif.org/ver10/topics" 
// Dialect="http://docs.oasis-open.org/wsn/t-1/TopicExpression/Concrete">tns1:VideoSource/MotionAlarm</wsnt:TopicExpression>
	else if (strstr(str,"@Name") != NULL) {
		str = strstr(str,"@Name=");
		while (*str != '"') {
			str++;
		}
		str++;
		int i = 0;
		while (*str != ']') {
			*(retstr+i) = *str;
			str++;
			i++;
		}
		*(retstr + (--i)) = '\0';
		LEAVE();
		return retstr;
	}

	LEAVE();
	return NULL;
}

#define YEAR_IN_SEC 60*60*24*30*12
#define MONTH_IN_SEC 60*60*24*30
#define DAY_IN_SEC 60*60*24
#define HOURS_IN_SEC 60*60
#define MIN_IN_SEC 60

long epoch_convert_switch(int value, char convert, int time)
{
	ENTER();

	long seconds = 0;
#if 1
	switch(convert)
	{
		case 'Y': seconds = value * YEAR_IN_SEC ;
			  break;
		case 'M':
			  if(time == 1)
			  {
				  seconds = value * MIN_IN_SEC;
			  }
			  else
			  {
				  seconds = value * MONTH_IN_SEC;
			  }
			  break;
		case 'D': seconds = value * DAY_IN_SEC;
			  break;
		case 'H': seconds = value * HOURS_IN_SEC;
			  break;
		case 'S': seconds = value;
			  break;
	}
#endif

	LEAVE();
	return seconds;
}

/*
 *  @brief Converts XML period format (PnYnMnDTnHnMnS) to long data type
 */

long periodtol(char *ptr)
{
	ENTER();

	char buff[10] = "0";
	char convert;
	int i = 0; 
	int value = 0;
	int time = 0;
	int minus = 0;
	long cumulative = 0;
	if(*ptr == '-')
	{
		ptr++;
		minus = 1;dvprintf("periodtol minus = 1\r\n");
	}
	while(*ptr != '\0')
	{
		if(*ptr != 'P' && *ptr != 'T' &&*ptr != 'Y' 
			&&*ptr != 'M' &&*ptr != 'D' &&*ptr != 'H'	
			&&*ptr != 'S' && *ptr < '0' && *ptr > '9')
		{
			dvprintf("periodtol ptr %c\r\n",*ptr);
			dvprintf("periodtol ptr %x\r\n",*ptr);
			LEAVE();
			return -1;
		}
		if(*ptr == 'P' || *ptr == 'T')
		{
			ptr++;
			if(*ptr == 'T')
			{
				time = 1;
				ptr++;
			}
		}
		else
		{dvprintf("periodtol else\r\n");
			dvprintf("periodtol else\r\n");
			if(*ptr >= '0' && *ptr <= '9')
			{
				buff[i] = *ptr;
				dvprintf("buff[i] %c\r\n",buff[i]);
				dvprintf("buff[i] %c\r\n",buff[i]);
				i++;
				ptr++;
			}
			else
			{
				buff[i] = 0;
				value = atoi(buff);
				dvprintf("[ value %d]\r\n",value);
				memset(buff, 0, sizeof(buff));
				i = 0;
				convert = *ptr;
				ptr++;
				cumulative = cumulative + epoch_convert_switch(value, convert, time);
				dvprintf("cumulative %ld value %d\r\n",cumulative,value);
				dvprintf("cumulative %ld value %d\r\n",cumulative,value);
			}
		}
	}
	if(minus == 1)
	{
		LEAVE();
		return -cumulative;
	}
	else
	{
		LEAVE();
		return cumulative;
	}
}

unsigned long iptol(char *ip)
{
	ENTER();

	unsigned char o1,o2,o3,o4; /* The 4 ocets */
	char tmp[13] = "000000000000\0";
	short i = 11; /* Current Index in tmp */
	short j = (strlen(ip) - 1);
	do 
	{
		if ((ip[--j] == '.'))
		{
			i -= (i % 3);
		}
		else 
		{
			tmp[--i] = ip[j];
		}
	}while (i > -1);

	o1 = (tmp[0] * 100) + (tmp[1] * 10) + tmp[2];
	o2 = (tmp[3] * 100) + (tmp[4] * 10) + tmp[5];
	o3 = (tmp[6] * 100) + (tmp[7] * 10) + tmp[8];
	o4 = (tmp[9] * 100) + (tmp[10] * 10) + tmp[11];

	LEAVE();
	return (o1 * 16777216) + (o2 * 65536) + (o3 * 256) + o4;
}


/* @brief Check if IP is valid */
int isValidIp4 (char *str) 
{
	ENTER();

	int segs = 0;   /* Segment count. */     
	int chcnt = 0;  /* Character count within segment. */     
	int accum = 0;  /* Accumulator for segment. */      
	/* Catch NULL pointer. */      
	if (str == NULL) {
		LEAVE();
		return 0;
	}
	/* Process every character in string. */      
	while (*str != '\0') 
	{         
		/* Segment changeover. */          
		if (*str == '.') 
		{             
			/* Must have some digits in segment. */              
			if (chcnt == 0) {
				LEAVE();
				return 0;
			}
			/* Limit number of segments. */              
			if (++segs == 4) {
				LEAVE();
				return 0;
			}
			/* Reset segment values and restart loop. */              
			chcnt = accum = 0;             
			str++;             
			continue;         
		}  

		/* Check numeric. */          
		if ((*str < '0') || (*str > '9')) {
			LEAVE();
			return 0;
		}
		/* Accumulate and check segment. */          
		if ((accum = accum * 10 + *str - '0') > 255) {
			LEAVE();
			return 0;
		}
		/* Advance other segment specific stuff and continue loop. */          
		chcnt++;         
		str++;     
	}      
	/* Check enough segments and enough characters in last segment. */      
	if (segs != 3) {
		LEAVE();
		return 0;
	}
	if (chcnt == 0) {
		LEAVE();
		return 0;
	}
	/* Address okay. */
	LEAVE();
	return 1; 
} 

/* @brief Check if a hostname is valid */
int isValidHostname (char *str) 
{
	ENTER();

	/* Catch NULL pointer. */      
	if (str == NULL) 
	{
		LEAVE();
		return 0;      
	}
	/* Process every character in string. */      
	while (*str != '\0') 
	{         
		if ((*str >= 'a' && *str <= 'z') || (*str >= 'A' && *str <= 'Z') || (*str >= '0' && *str <= '9') || (*str == '.') || (*str == '-') )
		{
			str++;
		}
		else
		{
			LEAVE();
			return 0;
		}
	}
	LEAVE();
	return 1; 
}

char * getIpAddress(int port)
{
	ENTER();

	char *_IPv4; 	

	struct netif *netif;
	for(netif=netif_list;netif!=NULL;netif=netif->next)
	{
		dvprintf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\r\n", netif->name[0], netif->name[1], netif->num,
			netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

		dvprintf("	inet addr:%IP Mask:%IP GW:%IP\r\n", netif->ip_addr.addr, netif->netmask.addr, netif->gw.addr);

		dvprintf("	%s%s%s%s%s%s%sMTU:%d\r\n",
			netif->flags & NETIF_FLAG_UP ? "UP " : "",
			netif->flags & NETIF_FLAG_BROADCAST ? "BROADCAST " : "",
			netif->flags & NETIF_FLAG_POINTTOPOINT ? "PPP " : "",
			netif->flags & NETIF_FLAG_DHCP ? "DHCP " : "",
			netif->flags & NETIF_FLAG_LINK_UP ? "LINK-UP " : "",
			netif->flags & NETIF_FLAG_ETHARP ? "ETHARP " : "",
			netif->flags & NETIF_FLAG_IGMP ? "IGMP " : "",
			netif->mtu);
	}
#ifdef __WIFI__
	struct netif * pnetif;

	struct netif * pnetif_eth;
	struct netif * pnetif_wifi;
	struct netif * pnetif_wifi2;

	pnetif_eth = netif_find(ETH_IF_NAME);
	pnetif_wifi = netif_find(WIFI_IF_NAME);
	pnetif_wifi2 = netif_find(WIFI_IF_NAME_II);
	//printf("getIpAddress\r\n");
	if(pnetif_eth != NULL && netif_is_up(pnetif_eth))
	{
		pnetif = pnetif_eth;
		//printf("ETH_IF_NAME\r\n");
	}
	else if(pnetif_wifi != NULL && netif_is_up(pnetif_wifi))
	{
		pnetif = pnetif_wifi;
		//printf("WIFI_IF_NAME\r\n");
	}
	else if(pnetif_wifi2 != NULL && netif_is_up(pnetif_wifi2))
	{
		pnetif = pnetif_wifi2;
		//printf("WIFI_IF_NAME_II\r\n");
	}
	else
	{
		pnetif = pnetif_eth;
	}
#else
	struct netif * pnetif;
	pnetif = netif_find(ETH_IF_NAME);
#endif	

#if 1
	static char strbuf[16];
	_IPv4 = ipaddr_ntoa_r(&pnetif->ip_addr, strbuf, 16);
	//printf("_IPv4 %s \r\n",_IPv4);
#else
	_IPv4 = ipaddr_ntoa(&pnetif->ip_addr);
#endif

	if(port == 8080)
		strcat(_IPv4, ":8080");
	
	if(_IPv4 != NULL)		
		dvprintf("%s\r\n",_IPv4);

	LEAVE();
	return _IPv4;
}

void getMacAddress(char *_MAC)
{
	ENTER();

	struct netif *netif;
	for(netif=netif_list;netif!=NULL;netif=NULL)
	{
		dvprintf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\r\n", netif->name[0], netif->name[1], netif->num,
			netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);
		_MAC[0] = netif->hwaddr[0];
		_MAC[1] = netif->hwaddr[1];
		_MAC[2] = netif->hwaddr[2];
		_MAC[3] = netif->hwaddr[3];
		_MAC[4] = netif->hwaddr[4];
		_MAC[5] = netif->hwaddr[5];
	}

	//ip_addr_t _ipaddr;	
	//ip4_addr_set_u32(&_ipaddr, netif->ip_addr);
	//_IPv4 = ipaddr_ntoa(&netif->ip_addr);

	//sprintf(_IPv4,"%d.%d.%d.%d",ip4_addr1(&netif_list->ip_addr), ip4_addr2(&netif_list->ip_addr), ip4_addr3(&netif_list->ip_addr), ip4_addr4(&netif_list->ip_addr));
	
	if(_MAC != NULL)		
		dvprintf("%s\r\n",_MAC);

	LEAVE();
}

////////////////////////////////////////////////////////////////////////////////
// ONVIF Fault Code ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Fault Code
enum {
	ovfFC_Receiver,
	ovfFC_Sender,
};

// Parent Subcode
enum {
	ovfPS_ActionNotSupported,
	ovfPS_Action,
	ovfPS_InvalidArgVal,
	ovfPS_OperationProhibited
};

typedef struct {
	UINT faultcode;
	UINT parentsubcode;
	char *description;
}OnvifFaultCodeInfo;	// index subcode

static OnvifFaultCodeInfo listOFCinfo[ovfSC_NONE] = {
/* ovfSC_AudioNotSupported */			{ovfFC_Receiver, ovfPS_ActionNotSupported, "The device does not support audio."},	
/* ovfSC_MaxNVTProfiles */				{ovfFC_Receiver, ovfPS_Action, "The maximum number of supported profiles has been reached."},
/* ovfSC_AudioOutputNotSupported */		{ovfFC_Receiver, ovfPS_ActionNotSupported, "Audio or Audio Outputs are not supported by the device."},
/* ovfSC_AudioDecodingNotSupported */	{ovfFC_Receiver, ovfPS_ActionNotSupported, "Audio or Audio Decoding is not supported by the device."},
/* ovfSC_IncompleteConfiguration */		{ovfFC_Receiver, ovfPS_Action, "Entities required by this action are missing in the specified profile."},
/* ovfSC_ConfigurationConflict */		{ovfFC_Receiver, ovfPS_Action, "The new settings conflicts with other uses of the configuration."},
/* ovfSC_MaxOSDs */						{ovfFC_Receiver, ovfPS_Action, "The maximum number of the OSD supported by the specified VideoSourceConfiguration has been reached."},
/* ovfSC_ProfileExists */				{ovfFC_Sender, ovfPS_InvalidArgVal, "A profile with the token ProfileToken already exists."},
/* ovfSC_NoConfig */					{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested configuration indicated by the ConfigurationToken does not exist."},
/* ovfSC_NoProfile */					{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested profile token ProfileToken does not exist."},
/* ovfSC_DeletionOfFixedProfile */		{ovfFC_Sender, ovfPS_Action, "The fixed Profile cannot be deleted."},
/* ovfSC_ConfigModify */				{ovfFC_Sender, ovfPS_InvalidArgVal, "The configuration parameters are not possible to set."},
/* ovfSC_VideoAnalyticsNotSupported */	{ovfFC_Sender, ovfPS_ActionNotSupported, "The device does not support video analytics."},
/* ovfSC_InvalidStreamSetup */			{ovfFC_Sender, ovfPS_InvalidArgVal, "Specification of StreamType or Transport part in StreamSetup is not supported."},
/* ovfSC_StreamConflict */				{ovfFC_Sender, ovfPS_OperationProhibited, "Specification of StreamType or Transport part in StreamSetup causes conflict with other streams."},
/* ovfSC_InvalidMulticastSettings */	{ovfFC_Sender, ovfPS_InvalidArgVal, "Not all configurations are configured for multicast."},
/* ovfSC_NoVideoSourceMode */			{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested video source mode does not exist."},
/* ovfSC_NoSource */					{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested VideoSource does not exist."},
/* ovfSC_NoImagingForSource */ 			{ovfFC_Receiver, ovfPS_ActionNotSupported, "The requested VideoSource does not support imaging settings."},
/* ovfSC_SettingsInvalid */				{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested settings are incorrect."},
/* ovfSC_PTZNotSupported */				{ovfFC_Receiver, ovfPS_ActionNotSupported, "The device does not support PTZ."},
/* ovfSC_PTZNoEntity */ 				{ovfFC_Sender, ovfPS_InvalidArgVal, "No such PTZNode on the device."},
/* ovfSC_PTZInvalidPosition */ 			{ovfFC_Sender, ovfPS_InvalidArgVal, "A space is referenced in an argument which is not supported by the PTZ Node."},
/* ovfSC_PTZPresetExist */				{ovfFC_Sender, ovfPS_InvalidArgVal, "The requested name already exist for another preset."},
/* ovfSC_InvalidIPv4Address */			{ovfFC_Sender, ovfPS_InvalidArgVal, "The suggested IPv4 address is invalid."},
/* ovfSC_InvalidIPv6Address */			{ovfFC_Sender, ovfPS_InvalidArgVal, "The suggested IPv6 address is invalid."},
/* ovfSC_InvalidDnsName */				{ovfFC_Sender, ovfPS_InvalidArgVal, "The suggested NTP server name is invalid."},
/* ovfSC_TimeSyncedToNtp */				{ovfFC_Sender, ovfPS_InvalidArgVal, "Current DateTimeType requires an NTP server."},
};

void onvifInterfaceFault(struct soap *soap, MIFault code, char *Detail)
{
#define ONVIF_NS_ENV "env"
#define ONVIF_NS_ERR "ter"

	soap->fault = (struct SOAP_ENV__Fault *)soap_malloc(soap, (sizeof(struct SOAP_ENV__Fault)));

	soap->fault->SOAP_ENV__Code = (struct SOAP_ENV__Code *)soap_malloc(soap, (sizeof(struct SOAP_ENV__Code)));
	struct SOAP_ENV__Code *envcode = soap->fault->SOAP_ENV__Code;
	switch(listOFCinfo[code].faultcode) {
		case ovfFC_Receiver:					envcode->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ENV ":Receiver"); break;
		case ovfFC_Sender:						envcode->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ENV ":Sender"); break;
		default:
			printf("    %04d:%s: FaultCode(%d) Not Supported\r\n", __LINE__, __func__, listOFCinfo[code].faultcode);
			envcode->SOAP_ENV__Value = NULL;
			break;
	}

	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode = (struct SOAP_ENV__Code *)soap_malloc(soap, (sizeof(struct SOAP_ENV__Code)));
	struct SOAP_ENV__Code *subcode1 = soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode;
	switch(listOFCinfo[code].parentsubcode) {
		case ovfPS_ActionNotSupported:			subcode1->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":ActionNotSupported"); break;
		case ovfPS_Action:						subcode1->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":Action"); break;
		case ovfPS_InvalidArgVal:				subcode1->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidArgVal"); break;
		case ovfPS_OperationProhibited:			subcode1->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":OperationProhibited"); break;
		default:
			printf("    %04d:%s: SubCode1(%d) Not Supported\r\n", __LINE__, __func__, listOFCinfo[code].parentsubcode);
			subcode1->SOAP_ENV__Value = NULL;
			break;
	}

	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Subcode = (struct SOAP_ENV__Code *)soap_malloc(soap, (sizeof(struct SOAP_ENV__Code)));
	struct SOAP_ENV__Code *subcode2 = soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Subcode;
	switch(code) {
		case ovfSC_AudioNotSupported:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":AudioNotSupported"); break;
		case ovfSC_MaxNVTProfiles:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":MaxNVTProfiles"); break;
		case ovfSC_AudioOutputNotSupported:		subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":AudioOutputNotSupported"); break;
		case ovfSC_AudioDecodingNotSupported:	subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":AudioDecodingNotSupported"); break;
		case ovfSC_IncompleteConfiguration:		subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":IncompleteConfiguration"); break;
		case ovfSC_ConfigurationConflict:		subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":ConfigurationConflict"); break;
		case ovfSC_MaxOSDs:						subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":MaxOSDs"); break;
		case ovfSC_ProfileExists:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":ProfileExists"); break;
		case ovfSC_NoConfig:					subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoConfig"); break;
		case ovfSC_NoProfile:					subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoProfile"); break;
		case ovfSC_DeletionOfFixedProfile:		subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":DeletionOfFixedProfile"); break;
		case ovfSC_ConfigModify:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":ConfigModify"); break;
		case ovfSC_VideoAnalyticsNotSupported:	subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":VideoAnalyticsNotSupported"); break;
		case ovfSC_InvalidStreamSetup:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidStreamSetup"); break;
		case ovfSC_StreamConflict:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":StreamConflict"); break;
		case ovfSC_InvalidMulticastSettings:	subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidMulticastSettings"); break;
		case ovfSC_NoVideoSourceMode:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoVideoSourceMode"); break;
		case ovfSC_NoSource:					subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoSource"); break;
		case ovfSC_NoImagingForSource:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoImagingForSource"); break;
		case ovfSC_SettingsInvalid:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":SettingsInvalid"); break;
		case ovfSC_PTZNotSupported:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":PTZNotSupported"); break;
		case ovfSC_PTZNoEntity:					subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":NoEntity"); break;
		case ovfSC_PTZInvalidPosition:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidPosition"); break;
		case ovfSC_PTZPresetExist:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":PresetExist"); break;
		case ovfSC_InvalidIPv4Address:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidIPv4Address"); break;
		case ovfSC_InvalidIPv6Address:			subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidIPv6Address"); break;
		case ovfSC_InvalidDnsName:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":InvalidDnsName"); break;
		case ovfSC_TimeSyncedToNtp:				subcode2->SOAP_ENV__Value = soap_strdup(soap, ONVIF_NS_ERR ":TimeSyncedToNtp"); break;
		default:
			printf("    %04d:%s: SubCode2(%d) Not Supported\r\n", __LINE__, __func__, code);
			subcode2->SOAP_ENV__Value = NULL;
			break;
	}

	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Subcode->SOAP_ENV__Subcode = NULL;

	soap->fault->SOAP_ENV__Reason = (struct SOAP_ENV__Reason *)soap_malloc(soap, sizeof(struct SOAP_ENV__Reason));
	soap->fault->SOAP_ENV__Reason->SOAP_ENV__Text = soap_strdup(soap, listOFCinfo[code].description);

	soap->fault->SOAP_ENV__Node = soap_strdup(soap, "http://www.w3.org/2003/05/soap-envelope/node/ultimateReceiver");
	soap->fault->SOAP_ENV__Role = soap_strdup(soap, "http://www.w3.org/2003/05/soap-envelope/role/ultimateReceiver");

	soap->fault->SOAP_ENV__Detail = (struct SOAP_ENV__Detail *)soap_malloc(soap, sizeof(struct SOAP_ENV__Detail));
	memset(soap->fault->SOAP_ENV__Detail, 0x00, sizeof(struct SOAP_ENV__Detail));
	soap->fault->SOAP_ENV__Detail->__any = soap_strdup(soap, Detail);
	soap->fault->SOAP_ENV__Detail->__type =	4;//SOAP_TYPE_string;

	soap->fault->faultcode = NULL;
	soap->fault->faultstring = NULL;
	soap->fault->faultactor = NULL;
	soap->fault->detail = NULL;
}
