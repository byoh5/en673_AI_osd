#ifndef __ONVIFUTIL_H__
#define __ONVIFUTIL_H__

////////////////////////////////////////////////////////////////////////////////
// ONVIF Fault Code ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Subcode
typedef enum {
	ovfSC_AudioNotSupported,
	ovfSC_MaxNVTProfiles,
	ovfSC_AudioOutputNotSupported,
	ovfSC_AudioDecodingNotSupported,
	ovfSC_IncompleteConfiguration,
	ovfSC_ConfigurationConflict,
	ovfSC_MaxOSDs,
	ovfSC_ProfileExists,
	ovfSC_NoConfig,
	ovfSC_NoProfile,
	ovfSC_DeletionOfFixedProfile,
	ovfSC_ConfigModify,
	ovfSC_VideoAnalyticsNotSupported,
	ovfSC_InvalidStreamSetup,
	ovfSC_StreamConflict,
	ovfSC_InvalidMulticastSettings,
	ovfSC_NoVideoSourceMode,
	ovfSC_NoSource,
	ovfSC_NoImagingForSource,
	ovfSC_SettingsInvalid,
	ovfSC_PTZNotSupported,
	ovfSC_PTZNoEntity,
	ovfSC_PTZInvalidPosition,
	ovfSC_PTZPresetExist,
	ovfSC_InvalidIPv4Address,
	ovfSC_InvalidIPv6Address,
	ovfSC_InvalidDnsName,
	ovfSC_TimeSyncedToNtp,
	ovfSC_NONE, // unused, MIFault length
} MIFault;

extern void onvifInterfaceFault(struct soap *soap, MIFault code, char *Detail);

////////////////////////////////////////////////////////////////////////////////
// ONVIF UTIL FUNCTIONS ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

extern void explodeitem(char *in, char dl, char list[][100]);
extern char *strparser(char *str);
extern char *anyparser(char *str);
extern long epoch_convert_switch(int value, char convert, int time);
extern long periodtol(char *ptr);
extern unsigned long iptol(char *ip);
extern int isValidIp4 (char *str);
extern int isValidHostname (char *str);
extern char * getIpAddress(int port);

//#define AllocCharCpyN(soap, str) strcpy((char *)soap_malloc(soap, sizeof(char)*strlen(str)), str)
//#define AllocIntCpyN(soap) (int *)soap_malloc(soap, sizeof(int))
#endif
