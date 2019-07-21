#ifndef __ONVIF_SYSTEM_INFO_H__
#define __ONVIF_SYSTEM_INFO_H__

#include "soapStub.h"

#define SUCCESS 0
#define FAILED 1

#define IP_INFO_LENGTH 	60

#define SMALL_INFO_LENGTH 	50
#define LARGE_INFO_LENGTH 	512
#define INFO_LENGTH 		128
#define MAX_PROF_TOKEN 		50
#define MID_INFO_LENGTH 	128
#define MACH_ADDR_LENGTH 	40

#define MAX_SCOPE			20
#define MAX_SCOPE_LENGTH 	62

#define ONVIF_SERVICE_SIZE_DEFAULT 	1
#define ONVIF_SERVICE_SIZE_MAX 		5//4//6

#define MAX_PROFILE 		5
#define MAX_USER 			5

#define PTZ_PRESET_MAX_SIZE 	10
#define PTZ_NODE_MAX_SIZE 		2

#define ONVIF_HARDWARE 		"Eyenix Co.,Ltd"
#define ONVIF_NAME 			"EN673ONVIF"

#define MANUFACTURER 		"Manufacturer"
#define MODEL_NAME 			"Model Name"
#define FIRMWARE_VER		"Firmware Version"	
#define SERIALNUMBER		"Serial Number"
#define HARDWAREID		    "Hardware ID"

#define DEVICE_SERVICE_URL 	"http://www.onvif.org/ver10/device/wsdl" 	
#define MEDIA_SERVICE_URL	"http://www.onvif.org/ver10/media/wsdl"	
#define EVENT_SERVICE_URL 	"http://www.onvif.org/ver10/events/wsdl"	
#define IMAGING_SERVICE_URL	"http://www.onvif.org/ver20/imaging/wsdl"	
#define DEVICEIO_SERVICE_URL "http://www.onvif.org/ver10/deviceIO/wsdl"	
#define ANALYTICS_SERVICE_URL "http://www.onvif.org/ver20/analytics/wsdl"	
#define PANTILT_SERVICE_URL "http://www.onvif.org/ver20/ptz/wsdl"

#define DEVICE_SERVICE_ADDRESS	"http://%s/onvif/device_service"	
#define MEDIA_SERVICE_ADDRESS	"http://%s/onvif/Media"	
#define EVENT_SERVICE_ADDRESS	"http://%s/onvif/Events"
#define IMAGING_SERVICE_ADDRESS	"http://%s/onvif/Imaging"	
#define ANALY_SERVICE_ADDRESS   "http://%s/onvif/Analytics"
#define PTZ_SERVICE_ADDRESS  	"http://%s/onvif/ptz_services"
#define DEVIO_SERVICE_ADDRESS  	"http://%s/onvif/DeviceIO"
#define DISP_SERVICE_ADDRESS  	"http://%s/onvif/diplay_services"
#define RECORD_SERVICE_ADDRESS 	"http://%s/onvif/recording_services"
#define SEARCH_SERVICE_ADDRESS 	"http://%s/onvif/search_services"
#define REPLY_SERVICE_ADDRESS 	"http://%s/onvif/replay_services"
#define RECV_SERVICE_ADDRESS 	"http://%s/onvif/receiver_services"
#define ANALYDV_SERVICE_ADDRESS	"http://%s/onvif/analyticsdevice_services"
#define ONVIF_SERVICE_ADDRESS	"http://%s/onvif/services"

#define ONVIF_DEVICE_SCOPE_TYPE 		"onvif://www.onvif.org/type/"
#define ONVIF_DEVICE_SCOPE_PROFILE		"onvif://www.onvif.org/Profile/"
#define ONVIF_DEVICE_SCOPE_LOCATION 	"onvif://www.onvif.org/location/country/"
#define ONVIF_DEVICE_SCOPE_HARDWARE 	"onvif://www.onvif.org/hardware/"
#define ONVIF_DEVICE_SCOPE_NAME 		"onvif://www.onvif.org/name/"

#define ONVIF_DEVICE_SCOPE_TRANSMITTER  "dn:NetworkVideoTransmitter tds:Device"//"dn:NetworkVideoTransmitter"

/*temp*/
#define TEST_ONVIF_DEVICE_SCOPE_UUID "urn:uuid:143e7f0e-8f7e-11b5-8462-%02x%02x%02x%02x%02x%02x"
//#define TEST_ONVIF_DEVICE_SCOPE_MAC "%02X-%02X-%02X-%02X-%02X-%02X"
#define TEST_ONVIF_DEVICE_SCOPE_MAC "%02x:%02x:%02x:%02x:%02x:%02x"

/*temp*/
#define DEVICE_SERVICE_VERSION_MAJOR 		2
#define DEVICE_SERVICE_VERSION_MINOR 		40
#define MEDIA_SERVICE_VERSION_MAJOR 		2
#define MEDIA_SERVICE_VERSION_MINOR 		40
#define EVENT_SERVICE_VERSION_MAJOR 		2
#define EVENT_SERVICE_VERSION_MINOR 		40
#define IMAGING_SERVICE_VERSION_MAJOR 		2
#define IMAGING_SERVICE_VERSION_MINOR 		40
#define PANTILT_SERVICE_VERSION_MAJOR 		2
#define PANTILT_SERVICE_VERSION_MINOR 		21
#define DEVICEIO_SERVICE_VERSION_MAJOR 		2
#define DEVICEIO_SERVICE_VERSION_MINOR 		21
#define ANALYTICS_SERVICE_VERSION_MAJOR 	2
#define ANALYTICS_SERVICE_VERSION_MINOR 	40

#define ONVIF_SERVICE_VERSION_MAJOR 		40//14
#define ONVIF_SERVICE_VERSION_MINOR 		2///1

/* temp */
#define MAJOR 		1
#define MINOR 		40

/* temp */
#define ETH_IF_NAME 			"en0"
#define WIFI_IF_NAME 		"wl1"
#define WIFI_IF_NAME_II 	"wl0"

#define DEFAULT_SCOPE 		"onvif"
#define EN673_APP_VERSION 	"EN673-v0.1.0"//EN673_ENTRY_VERSION

#define IDX_2CH_MAIN_STREAM 0
#define IDX_2CH_SUB_STREAM 1

enum
{
	EXIST = 0,
	NOT_EXIST
};
enum
{
	_false = 0,
	_true
};
/*enum
{
	FALSE = 0,
	TRUE
};*/

/*SYSTEM INFORMATIONS*/
enum
{
	/*SET DATA*/
	SFIELD_SET_DEFAULT = 0,
	SFIELD_SET_SNTP_SERVER,
	SFIELD_SET_MOTIONENABLE,
	SFIELD_SET_TIMEZONE,
	SFIELD_SET_DAYLIGHT,
	SFIELD_SET_HOSTNAME,
	SFIELD_SET_DNS,
	SFIELD_SET_IP,
	SFIELD_SET_DHCPC_ENABLE,
	SFIELD_SET_GATEWAY,
	SFIELD_SET_RELAYS_CONF,
	SFIELD_SET_RELAY_LOGICALSTATE,
	SFIELD_SET_BRIGHTNESS,
	SFIELD_SET_SATURATION,
	SFIELD_SET_CONTRAST,
	SFIELD_SET_SHARPNESS,
	SFIELD_SET_IMAGING,
	SFIELD_SET_VIDEOOUTPUT_CONF,
	SFIELD_SET_VIDEOSOURCE_CONF,
	SFIELD_SET_VIDEO_MODE,
	SFIELD_SET_VIDEOCODECCOMBO,
	SFIELD_SET_VIDEOCODECRES,
	SFIELD_SET_VIDEOENCODER_CONF,
	SFIELD_SET_MPEG41_BITRATE,
	SFIELD_SET_AUDIO_ENCODE,
	SFIELD_SET_AUDIO_SOURCE,
	SFIELD_SET_AUDIOENCODER_CONF,
	SFIELD_SET_AUDIOENABLE,
	SFIELD_SET_AUDIOMODE,
	SFIELD_SET_AUDIO_BITRATE,
	SFIELD_SET_AUDIO_SAMPLERATE,
	SFIELD_SET_VIDEOANALYTICS_CONF,
	SFIELD_SET_METADATA,
	
	/*GET DATA*/
	SFIELD_GET_SNTP_FQDN,

	/*ADD DATA*/
	SFIELD_ADD_VIDEOENCODER_CONF,
	SFIELD_ADD_AUDIOENCODER_CONF,
	SFIELD_ADD_METADATA,
	SFIELD_ADD_PROFILE,
	SFIELD_ADD_SCOPE,
	SFIELD_ADD_USER,
	SFIELD_ADD_VIDEOSOURCE_CONF,
	
	/*DEL DATA*/
	SFIELD_DEL_VIDEOENCODER_CONF,
	SFIELD_DEL_AUDIO_ENCODER,
	SFIELD_DEL_AUDIO_SOURCE,
	SFIELD_DEL_SCOPE,
	SFIELD_DEL_USER,
	SFIELD_DEL_PROFILE,

	/*MODE*/
	SFIELD_DIS_MODE
};

enum
{
	SYSTEM_PROCESS_CMD_DEFUALT = -1,
	SYSTEM_PROCESS_CMD_REBOOT_CALL = 0,
	SYSTEM_PROCESS_CMD_PROBE_MATCH_CALL
	
};


/*imaging struct*/
typedef struct {
	float CrGain;
	float CbGain;
	float Brightness;
	float Saturation;
	float Contrast;
	float Sharpness;
	int WideDynamicMode;
	float WideDynamicLevel;
	int IrCutFilterMode;
	int BacklightCompensationMode;
	float BackLightLevel;
	int WhiteBalanceMode;
}imaging_conf_t;

/*video source struct*/
typedef struct {
	char *Vtoken_t[MID_INFO_LENGTH];
	char *Vname_t[MID_INFO_LENGTH];
	char *Vsourcetoken_t[MID_INFO_LENGTH];
	int Vcount_t;
	int windowx_t;
	int windowy_t;
	int windowwidth_t;
	int windowheight_t;
}video_source_conf_in_typedef;

typedef struct {	
	int position;
	video_source_conf_in_typedef video_source_conf_in_t;
}video_source_conf;

/* video encoder struct*/
typedef struct {
	enum tt__IPType nIPType_t;
	char *IPv4Addr_t;
	int port_t;
	int ttl_t;
	enum xsd__boolean autostart_t;
}Multicast_Conf;

typedef struct {
	char *VEname_t;
	char *VEtoken_t;
	int VEusercount_t;
	float VEquality_t;
	enum tt__VideoEncoding Vencoder_t;
	int Rwidth_t;
	int Rheight_t;
	int frameratelimit_t;
	int encodinginterval_t;
	int bitratelimit_t;
	int Mpeggovlength_t;
	enum tt__Mpeg4Profile Mpegprofile_t;
	int H264govlength_t;
	enum tt__H264Profile H264profile_t;
	LONG64 sessiontimeout_t;
	Multicast_Conf Multicast_Conf_t;
}video_encoder_conf_in;

typedef struct {

	int position;
	video_encoder_conf_in video_encoder_conf_in_t;

}video_encoder_conf;

/* net struct */
typedef struct {
//	int dhcp_enable;
	struct in_addr dns;
	char domainnameT[SMALL_INFO_LENGTH];
	char ntp_server[SMALL_INFO_LENGTH];
	struct in_addr gateway;
	int discovery_mode;
	int _http_port;
	int _http_enable;
	int _rtsp_port;
	int _rtsp_enable;
}net_t;

/* lan configure struct */
typedef struct {
	char title[SMALL_INFO_LENGTH];
	char modelname[SMALL_INFO_LENGTH];
	char chipConfig[SMALL_INFO_LENGTH];
	char hostname[SMALL_INFO_LENGTH];
	net_t net;
	int nVideocombo;
	char nClickSnapFilename[SMALL_INFO_LENGTH];
	unsigned char nVideocodecres;
}lan_config_t;

/*ImagingSettings*/
typedef struct {
	enum tt__BacklightCompensationMode Mode;
	float Level;
}BacklightCompensation_t;

typedef struct {
	float bottom;	
	float top; 
	float right;
	float left;
}Rectangle_t;

typedef struct {
	enum tt__ExposureMode Mode; 
	enum tt__ExposurePriority Priority; 
	Rectangle_t Window;	
	float MinExposureTime;	
	float MaxExposureTime;	
	float MinGain;
	float MaxGain;	
	float MinIris;	
	float MaxIris;	
	float ExposureTime;
	float Gain; 
	float Iris; 
}Exposure_t;

typedef struct {
	enum tt__AutoFocusMode AutoFocusMode;
	float DefaultSpeed;
	float NearLimit;	
	float FarLimit; 
	int __size;
	char **__any;
	char *__anyAttribute;	
}FocusConfiguration_t;

typedef struct{
	enum tt__WideDynamicMode Mode;	
	float Level;	
}WideDynamicRange_t;

typedef struct{
	enum tt__WhiteBalanceMode Mode;
	float CrGain;
	float CbGain;
	int __size;
	char **__any;
	char *__anyAttribute;
}WhiteBalance_t;

typedef struct {	
	int __size;
	char **__any;
}ImagingSettingsExtension_t;

typedef struct {
	BacklightCompensation_t BacklightCompensation;
	float Brightness;	
	float ColorSaturation;
	float Contrast; 
	Exposure_t Exposure;	
	FocusConfiguration_t Focus;	
	enum tt__IrCutFilterMode IrCutFilter;
	float Sharpness;	
	WideDynamicRange_t WideDynamicRange; 
	WhiteBalance_t WhiteBalance; 
	ImagingSettingsExtension_t Extension; 
	char *__anyAttribute;	
}ImagingSettings_t;


/*scope struct*/
typedef struct {
	BYTE type;
	char str[MAX_SCOPE_LENGTH];
}onvif_scopes_t;


typedef union __NET_IPV4 {
    unsigned int   int32;
    unsigned char  str[4];
} NET_IPV4;


/* multicast struct*/
typedef struct {
	int port;
	int ttl;
	int autostart;
	int nIPType;
}Multicast_Conf_t;

/* media user struct */
//VideoSourceConfiguration
typedef struct {
	char Vname[SMALL_INFO_LENGTH];
	char Vtoken[SMALL_INFO_LENGTH];
	char Vsourcetoken[SMALL_INFO_LENGTH];
	int  Vcount;
	float Framerate;
	int  windowx;
	int  windowy;
	int  windowwidth;
	int  windowheight;
	ImagingSettings_t gImagingSettings;
} VSC_t;

//VideoEncoderConfiguration
typedef struct {
	char VEname[SMALL_INFO_LENGTH];
	char VEtoken[SMALL_INFO_LENGTH];
	int VEusercount;
	int Vencoder;
	int  Rwidth;
	int  Rheight;
	float  VEquality;
	int  frameratelimit;
	int  encodinginterval;
	int  bitratelimit;
	int  H264govlength;
	int  H264Profile;
	int  Mpeggovlength;
	int  Mpegprofile;
	LONG64  sessiontimeout;
	Multicast_Conf_t Multicast_Conf;
} VEC_t;

//VideoOutputConfiguration
typedef struct {
	char name[SMALL_INFO_LENGTH];
	int usecount;
	char token[SMALL_INFO_LENGTH];
} VOC_t;

//VideoAnalyticsConfiguration
typedef struct {
	char name[SMALL_INFO_LENGTH];
	int usecount;
	char token[SMALL_INFO_LENGTH];
} VAC_t;

/* media user struct */
//AudioSourceConfiguration
typedef struct {
	char Aname[SMALL_INFO_LENGTH];
	char Atoken[SMALL_INFO_LENGTH];
	char Asourcetoken[SMALL_INFO_LENGTH];
	int  Acount;
} ASC_t;

//AudioEncoderConfiguration
typedef struct {
	char AEname[SMALL_INFO_LENGTH];
	char AEtoken[SMALL_INFO_LENGTH];
	int AEusercount;
	int Aencoder;
	int  Abitratelimit;	
	int  Asamplerate;	
	Multicast_Conf_t Multicast_Conf;
	LONG64  sessiontimeout;
} AEC_t;

//AudioOutputConfiguration
typedef struct {
	char AOname[SMALL_INFO_LENGTH];
	char AOtoken[SMALL_INFO_LENGTH];
	int AOusecount;
	char AOutputToken[SMALL_INFO_LENGTH];
	int AOutputLevel;
} AOC_t;

//AudioDecoderConfiguration
typedef struct {
	char ADname[SMALL_INFO_LENGTH];
	char ADtoken[SMALL_INFO_LENGTH];
	int ADusecount;
} ADC_t;

//MetaDataConfiguration
typedef struct {
	char MDname[SMALL_INFO_LENGTH];
	char MDtoken[SMALL_INFO_LENGTH];
	int MDusecount;
	int  MDanalytics;
	LONG64  sessiontimeout;
	Multicast_Conf_t VMC;
		
} MDC_t;

//PTZConfiguration
typedef struct {
	char Name[SMALL_INFO_LENGTH];
	char Token[SMALL_INFO_LENGTH];
	float Pantilt_x;
	float Pantilt_y;	
	char Pantilt_space[SMALL_INFO_LENGTH];

	float Zoom_x;	
	char Zoom_space[SMALL_INFO_LENGTH];
} PTZC_preset_t;

typedef struct {
	char Token[SMALL_INFO_LENGTH];
	char Name[SMALL_INFO_LENGTH];
	
	int __sizeAbsolutePanTiltPositionSpace;
	char AbsolutePanTiltPositionSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange AbsolutePanTiltPositionSpace_XRange;
	struct tt__FloatRange AbsolutePanTiltPositionSpace_YRange;

	int __sizeAbsoluteZoomPositionSpace;
	char AbsoluteZoomPositionSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange AbsoluteZoomPositionSpace_XRange;
	
	int __sizeRelativePanTiltTranslationSpace;
	char RelativePanTiltTranslationSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange RelativePanTiltTranslationSpace_XRange;
	struct tt__FloatRange RelativePanTiltTranslationSpace_YRange;
	
	int __sizeRelativeZoomTranslationSpace;
	char RelativeZoomTranslationSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange RelativeZoomTranslationSpace_XRange;
	
	int __sizeContinuousPanTiltVelocitySpace;
	char ContinuousPanTiltVelocitySpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange ContinuousPanTiltVelocitySpace_XRange;
	struct tt__FloatRange ContinuousPanTiltVelocitySpace_YRange;
	
	int __sizeContinuousZoomVelocitySpace;
	char ContinuousZoomVelocitySpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange ContinuousZoomVelocitySpace_XRange;
	
	int __sizePanTiltSpeedSpace;
	char PanTiltSpeedSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange PanTiltSpeedSpace_XRange;
	
	int __sizeZoomSpeedSpace;
	char ZoomSpeedSpace_URI[MID_INFO_LENGTH];
	struct tt__FloatRange ZoomSpeedSpace_XRange;
	
	int MaximumNumberOfPresets;	
	enum xsd__boolean HomeSupported;
	int __sizeAuxiliaryCommands;
	char AuxiliaryCommands[MID_INFO_LENGTH];
	enum xsd__boolean FixedHomePosition;
} PTZC_node_t;

typedef struct {
	int SizePreset;
	PTZC_preset_t Preset[PTZ_PRESET_MAX_SIZE];

	int SizeNode;
	PTZC_node_t Node[PTZ_NODE_MAX_SIZE];

	char PTZname[SMALL_INFO_LENGTH];
	char PTZtoken[SMALL_INFO_LENGTH];
	int PTZusecount;

	float CurrentPantilt_x;
	float CurrentPantilt_y;	
	char CurrentPantilt_space[MID_INFO_LENGTH];

	float CurrentZoom_x;	
	char CurrentZoom_space[MID_INFO_LENGTH];
	
	float CurrentTransPantilt_x;
	float CurrentTransPantilt_y;	
	char CurrentTransPantilt_space[MID_INFO_LENGTH];

	float CurrentTransZoom_x;	
	char CurrentTransZoom_space[MID_INFO_LENGTH];
	
	float CurrentSpeedPantilt_x;
	float CurrentSpeedPantilt_y;	
	char CurrentSpeedPantilt_space[MID_INFO_LENGTH];

	float CurrentSpeedZoom_x;	
	char CurrentSpeedZoom_space[MID_INFO_LENGTH];
	
	enum tt__MoveStatus CurrentPanTiltStatus;
	enum tt__MoveStatus CurrentZoomStatus;
	
	char DefaultAbsolutePantTiltPositionSpace[MID_INFO_LENGTH];
	char DefaultAbsoluteZoomPositionSpace[MID_INFO_LENGTH];
	char DefaultRelativePanTiltTranslationSpace[MID_INFO_LENGTH];
	char DefaultRelativeZoomTranslationSpace[MID_INFO_LENGTH];
	char DefaultContinuousPanTiltVelocitySpace[MID_INFO_LENGTH];
	char DefaultContinuousZoomVelocitySpace[MID_INFO_LENGTH];

	float DefaultPantilt_x;
	float DefaultPantilt_y;	
	char DefaultPantilt_space[MID_INFO_LENGTH];

	float DefaultZoom_x;	
	char DefaultZoom_space[MID_INFO_LENGTH];
	
	char PanTiltLimit_URI[MID_INFO_LENGTH];	
	float PanTiltLimit_XRange_Min;	
	float PanTiltLimit_XRange_Max;	
	float PanTiltLimit_YRange_Min;	
	float PanTiltLimit_YRange_Max;	
	
	char ZoomLimit_XRange_URI[MID_INFO_LENGTH];
	float ZoomLimit_XRange_Min;
	float ZoomLimit_XRange_Max;

	LONG64 DefaultPTZTimeout;	
	LONG64 MinPTZTimeout;	
	LONG64 MaxPTZTimeout;
} PTZC_t;

/*profile struct*/
typedef struct {
	int fix;
	char profilename[SMALL_INFO_LENGTH];
	char profiletoken[SMALL_INFO_LENGTH];
	
	int useVSC;	// Whether to use Video Source Configuration
	int useVEC;	// Whether to use Video Encoder Configuration
	int useVOC;	// Whether to use Video Output Configuration
	int useVAC;	// Whether to use Video Analytics Configuration

	int useASC;	// Whether to use Audio Source Configuration
	int useAEC;	// Whether to use Audio Encoder Configuration
	int useAOC;	// Whether to use Audio Output Configuration
	int useADC;	// Whether to use Audio Decoder Configuration

	int useMDC;	// Whether to use Meta Data Configuration	
	int usePTZC;// Whether to use PTZ Configuration	

	char VSCtoken[SMALL_INFO_LENGTH];
	char VECtoken[SMALL_INFO_LENGTH];
	char VOCtoken[SMALL_INFO_LENGTH];
	char VACtoken[SMALL_INFO_LENGTH];

	char ASCtoken[SMALL_INFO_LENGTH];
	char AECtoken[SMALL_INFO_LENGTH];
	char AOCtoken[SMALL_INFO_LENGTH];
	char ADCtoken[SMALL_INFO_LENGTH];

	char MDCtoken[SMALL_INFO_LENGTH];
	char PTZCtoken[SMALL_INFO_LENGTH];
} Oprofile_t;

typedef struct{
	UINT nMagicNum;
	UINT nDateID;
	int nProfileSize;
	Oprofile_t Oprofile[MAX_PROFILE];
	
	onvif_scopes_t	Scope[MAX_SCOPE];
	
	BYTE nScope;
	BYTE nUserNumber;
	BYTE bUserLevel[MAX_USER];
	char strGuestId[MAX_USER][16];			// System - User - User ID
	char strGuestPw[MAX_USER][16];			// System - User - User Password
	lan_config_t lan_config;
	
	int SizeDefaultVSC;
	int SizeDefaultVEC;
//	int SizeDefaultVOC;
//	int SizeDefaultVAC;

	int SizeDefaultASC;
	int SizeDefaultAEC;
//	int SizeDefaultAOC;
//	int SizeDefaultADC;

//	int SizeDefaultMDC;

	VSC_t VSC[MAX_USER];	//Video Source Configuration
	VEC_t VEC[MAX_USER];	//Video Encoder Configuration
	VOC_t VOC;				//Video Output Configuration
	VAC_t VAC;				//Video Analytics Configuration

	ASC_t ASC[MAX_USER];	//Audio Source Configuration
	AEC_t AEC[MAX_USER];	//Audio Encoder Configuration
	AOC_t AOC;				//Audio Output Configuration
	ADC_t ADC;				//Audio Decoder Configuration

	MDC_t MDC;				//Metadata Decoder Configuration
	PTZC_t PTZC;			//Pantilt Zoom Configuration
}SysInfo;

typedef struct{
	int IS_CALL;
	int CMD;
}SystemProcessCmd_t;


extern char *onvif_svc_namespace[];
extern char *onvif_svc_address[];
extern int onvif_svc_version_major[];
extern int onvif_svc_version_minor[];

extern SystemProcessCmd_t SystemProcessCmd; 
extern SystemProcessCmd_t SystemProcessCmd_UDP; 
extern UINT uiUserinfoUpdate;

/*SYSTEM INFOMATIONS*/
extern SysInfo *GetSysInfo(void);
extern int GetProfileSize(void);
extern void SetProfileSize( int size);

extern void DataSetting(SysInfo *SysInfo);	

extern int oget_bitratelimitH264(int bitrate);
extern int oget_bitratelimitJPEG(int bitrate);

extern float oget_framerate(void);
extern float oget_framerateMax(void);
extern int oget_framerate2(int framelimit);
extern int oset_framerate(int frameReate);

extern int oset_bitratelimitH264(int limit);
extern int oset_bitratelimitJPEG(int limit);

extern u32_t net_get_ifaddr(char *ifname);
extern void net_get_hwaddr(char *ifname, char *tp);
extern int run_ntpclient(char *value);
extern int sys_set_date(int _Year, int _Month, int _Day);
extern int sys_set_time(int _Hour, int _Minute, int _Second);
extern char *oget_timezone(char *timeZone);

extern int MatchProfileVSCToken(char * ProfileVtoken);
extern int MatchProfileVECToken(char * ProfileVtoken);
extern int MatchProfileASCToken(char * ProfileAtoken);
extern int MatchProfileAECToken(char * ProfileAtoken);
//extern int MatchProfileAOCToken(char * ProfileAtoken);
//extern int MatchProfileADCToken(char * ProfileAtoken);

extern int ControlSystemData(int stData, void *pData, int szData);
extern void systemProcessFunction(void);

extern int OnvifCFG_dircheck(void);
extern int OnvifCFG_Save(void); 
extern int OnvifCFG_Load(void);
extern SysInfo gSysInfo;


extern char TimeZoneList[];
extern char TimeZoneList2[];

#endif /* __ONVIF_SYSTEM_INFO_H__ */


