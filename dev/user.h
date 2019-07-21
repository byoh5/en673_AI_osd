#ifndef _USER_H_
#define _USER_H_

#include "factory.h"
#include "dev_types.h"
#include <time.h>

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//

typedef struct{
	char			sUserLoginAddr[LOGIN_MAX_NUM][64];	// Login User's address
	long			sUserLoginTime[LOGIN_MAX_NUM];		// Login Time
}__attribute__ ((packed)) tUserLoginInfo;

typedef enum {
	e_res1920x1080 = 0,	// 16:9
	e_res1280x720 = 1,	// 16:9
	e_res800x600,		// 4:3
	e_res704x480,		// 3:2
	e_res704x400,		// 16:9
	e_res640x480,		// 4:3
	e_res640x360,		// 16:9
	e_res320x240,		// 4:3
	e_resEndorUnknown
} Resolution;

typedef enum {
	e_s60f60 = 0,
	e_s60f30 = 1,
	e_s60f20 = 2,
	e_s60f15 = 3,
	e_s60f12 = 4,
	e_s60f10 = 5,
	e_s60f6 = 6,
	e_s60f5 = 7,
	e_s60f4 = 8,
	e_s60f3 = 9,
	e_s60f2 = 10,
	e_s60f1 = 11,
	e_s60End = 12,
	e_s60Unknown = 0xffffffff,

	e_s50f50 = 0,
	e_s50f25 = 1,
	e_s50f10 = 2,
	e_s50f5 = 3,
	e_s50f2 = 4,
	e_s50f1 = 5,
	e_s50End = 6,
	e_s50Unknown = 0xffffffff,

	e_s30f30 = 0,
	e_s30f15 = 1,
	e_s30f10 = 2,
	e_s30f6 = 3,
	e_s30f5 = 4,
	e_s30f3 = 5,
	e_s30f2 = 6,
	e_s30f1 = 7,
	e_s30End = 8,
	e_s30Unknown = 0xffffffff,

	e_s25f25 = 0,
	e_s25f5 = 1,
	e_s25f1 = 2,
	e_s25End = 3,
	e_s25Unknown = 0xffffffff,

	e_s15f15 = 0,
	e_s15f5 = 1,
	e_s15f3 = 2,
	e_s15f1 = 3,
	e_s15End = 4,
	e_s15Unknown = 0xffffffff,

	e_s12f12 = 0,
	e_s12f6 = 1,
	e_s12f4 = 2,
	e_s12f3 = 3,
	e_s12f2 = 4,
	e_s12f1 = 5,
	e_s12End = 6,
	e_s12Unknown = 0xffffffff,
}SensorFps;

typedef enum {
	e_isp60fps = 0,
	e_isp50fps = 1,
	e_isp30fps = 2,
	e_isp25fps = 3,
	e_isp15fps = 4,
	e_isp12fps = 5,
	e_ispEndorUnknown = 6,
} IspFps;

typedef enum {
	e_rate50mbps = 51200,
	e_rate45mbps = 46080,
	e_rate40mbps = 40960,
	e_rate35mbps = 35840,
	e_rate30mbps = 30720,
	e_rate25mbps = 25600,
	e_rate20mbps = 20480,
	e_rate19mbps = 19456,
	e_rate18mbps = 18432,
	e_rate17mbps = 17408,
	e_rate16mbps = 16384,
	e_rate15mbps = 15360,
	e_rate14mbps = 14336,
	e_rate13mbps = 13312,
	e_rate12mbps = 12288,
	e_rate11mbps = 11264,
	e_rate10mbps = 10240,
	e_rate9mbps = 9216,
	e_rate8mbps = 8192,
	e_rate7mbps = 7168,
	e_rate6mbps = 6144,
	e_rate5mbps = 5120,
	e_rate4mbps = 4096,
	e_rate3mbps = 3072,
	e_rate2mbps = 2048,
	e_rate1mbps = 1024,
	e_rate512kbps = 512,
	e_rate256kbps = 256,
	e_rate128kbps = 128,
	e_rateEndorUnknown = 0
} StreamBitRate;

typedef enum{
	e_jc50m = 0,
	e_jc45m,
	e_jc40m,
	e_jc35m,
	e_jc30m,
	e_jc25m,
	e_jc20m,
	e_jc19m,
	e_jc18m,
	e_jc17m,
	e_jc16m,
	e_jc15m,
	e_jc14m,
	e_jc13m,
	e_jc12m,
	e_jc11m,
	e_jc10m,
	e_jc9m,
	e_jc8m,
	e_jc7m,
	e_jc6m,
	e_jc5m,
	e_jc4m,
	e_jc3m,
	e_jc2m,
	e_jc1m,
	e_jcEndorUnknown,
}JpegConstantBitRate;

typedef enum{
	e_hc20m = 0,
	e_hc19m,
	e_hc18m,
	e_hc17m,
	e_hc16m,
	e_hc15m,
	e_hc14m,
	e_hc13m,
	e_hc12m,
	e_hc11m,
	e_hc10m,
	e_hc9m,
	e_hc8m,
	e_hc7m,
	e_hc6m,
	e_hc5m,
	e_hc4m,
	e_hc3m,
	e_hc2m,
	e_hc1m,
	e_hc512k,
	e_hc256k,
	e_hc128k,
	e_hcEndorUnknown,
}H264ConstantBitRate;

typedef enum {
	e_brmQBR = 0,	// Video Bitrate: Q Fixed
	e_brmVBR = 1, 	// Video Bitrate: Variable
	e_brmCBR = 2,	// Video Bitrate: Constant
	e_brmCVBR = 3,	// Video Bitrate: Constant Variable
	e_brmEndorUnknown = 4,
} BitRateMode;

typedef enum {
	e_pmBaseline = 0,
	e_pmMainCB = 1,
	e_pmEndorUnknown = 2,
} H264ProfileMode;

typedef enum {
	e_H264_1CH = 0,
	e_H264_2CH = 1,
} VideoChannel;

typedef struct{
	Resolution	eResolution;	//  4Byte
	UINT		nQuality;		//  4Byte
	SensorFps	sfFPS;			//  4Byte
	BitRateMode	eBRMode;		//  4Byte
	UINT		nBitRate;		//  4Byte
	UINT		nIDRFrame;		//  4Byte, H.264 only
	H264ProfileMode eProfileMode;	//  4Byte, H.264 only
}uservideoinfo;					// 28Byte

typedef struct{
	char		strDdnsHostName[64];	// Network - DDNS - DDNS Host Name
	char		strDdnsAcId[16];		// Network - DDNS - DDNS Account ID
	char		strDdnsPw[16];			// Network - DDNS - DDNS Password
	char		strDdnsDetectedAddr[16];// Network - DDNS - DDNS Detected Address Ask to http://checkip.dyndns.com
}ddns_data;

typedef struct{
	char		strSmtpSvr[64];			// Network - SMTP - SMTP Server Address
	UINT		nSmtpPort;				// Network - SMTP - SMTP Server Port
	char		strBoardMailAddr[64];	// Network - SMTP - EN673 E-Mail Address
	char		strBoardMailPw[16];		// Network - SMTP - EN673 E-Mail Password
	char		strUserMailAddr[MAILING_NUM][64];// Network - SMTP - User E-Mail Address
}smtp_data;

typedef enum {
	e_tzM1200 = 0,
	e_tzM1100,
	e_tzM1000,
	e_tzM0930,
	e_tzM0900,
	e_tzM0800,
	e_tzM0700,
	e_tzM0600,
	e_tzM0500,
	e_tzM0400,
	e_tzM0330,
	e_tzM0300,
	e_tzM0200,
	e_tzM0100,
	e_tzN0000,
	e_tzP0100,
	e_tzP0200,
	e_tzP0300,
	e_tzP0330,
	e_tzP0400,
	e_tzP0430,
	e_tzP0500,
	e_tzP0530,
	e_tzP0545,
	e_tzP0600,
	e_tzP0630,
	e_tzP0700,
	e_tzP0800,
	e_tzP0845,
	e_tzP0900,
	e_tzP0930,
	e_tzP1000,
	e_tzP1030,
	e_tzP1100,
	e_tzP1200,
	e_tzP1245,
	e_tzP1300,
	e_tzP1400,
}TimeZone;

//...............................................
// Board global information
typedef struct {
	UINT			*_pBootAddr;			// fixed! 0x06FFF000, ECMb - System boot address
	UINT			nMagicNum;				// fixed! 0x06FFF004, ECMb - Magic number
	UINT			nDateID;				// fixed! 0x06FFF008, Compile Datetime ID
	UINT			nVersion;				// fixed! 0x06FFF00C, S/W version
	BYTE			nMacAddress[8];			// fixed! 0x06FF0010, ECMb - Network - Mac - use 6byte / alloc 8byte / 00 00 00 00 00 00 xx xx
	UINT			nRes0;					// fixed! 0x06FFF018, Customer Value
	UINT			nRes1;					// fixed! 0x06FFF01C, Customer Value
	BYTE			nSN[32];				// fixed! 0x06FFF020, Customer Serial Number
} tSystem; // Size: 64byte(0x24)

typedef struct {
	UINT			nMagicNum;				// fixed! 0x06FFE000, Magic Number
	UINT			nDateID;				// fixed! 0x06FFE004, Compile Datetime ID

	char			strUserId[16];			// System - User - User ID
	char			strUserPw[16];			// System - User - User Password

	UINT			nSystemTime;			// System - Date - 기기 처음 킨 시간
#if (ENX_SNTPC_use==1)
	UINT			bUseSntp;				// System - Date - SNTP enable	1:on, 0:off
	char			strSntpSvr[64];			// System - Date - SNTP Server Address
	UINT			nSntpPeriodic;			// System - Date - SNTP Periodic Time
	UINT			nSntpRetrySec;			// System - Date - SNTP Retry Sec
	UINT			nSntpRetryMaxcnt;		// System - Date - SNTP Retry Max cnt
#endif
	UINT			nTimeSummer;			// System - Date - use Summer Time
	TimeZone		nTimeZone;				// System - Date - TimeZone
	TimeZone		onvif_nTimeZone;		// System - Date - TimeZone

	UINT			nIpAddr;				// Network - Tcp - IP Address
	UINT			nNetMask;				// Network - Tcp - Netmask Address
	UINT			nGateway;				// Network - Tcp - Gateway Address
	UINT			nDnsSvr0;				// Network - Tcp - DNS Server Address 1
	UINT			nDnsSvr1;				// Network - Tcp - DNS Server Address 2
	UINT			bUseDhcp;				// Network - Tcp - Dhcp enable	0:on, 1:off

	char			strDeviceId[16];		// Network - UPNP - Device ID
	char			strDeviceName[64];		// Network - UPNP - Device Name

	UINT			nRtspPort;				// Network - RTP - RTP Port Number

	uservideoinfo	uviJpeg;				// Video - JPEG Control
	uservideoinfo	uviH264[2];				// Video - H.264 Control

#if (ENX_FTPD_use==1)
	UINT			bFtpServer;				// Network - FTP - on, off
#endif
#ifdef __DEVICE_SD__
	UINT			bSdAviSave;				// System - User - SD Card Avi Save 0:on, 1:off
#endif
#ifdef __AUDIO__
	UINT			bAudio;					// Audio - Switch - on(1), off(0)
#endif

#if (ENX_DDNSC_use==1)
	UINT			bUseDdns;				// Network - DDNS - DDNS enable
	ddns_data		ddnsData;				// Network - DDNS
#endif
#if (ENX_SMTPC_use==1)
	UINT			bUseSmtp;				// Network - SMTP - SMTP enable
	smtp_data		smtpData;				// Network - SMTP
#endif

	UINT 			nHttpPort;				// Network - IPutil
	UINT			autoipoffon;			// Network - IPutil
	UINT 			runningTime;			// Network - IPutil
	struct tm 		tTime;					// Network - IPutil
}tUser;

typedef enum {
	sfls_SYSTEM = 1,		//  4KB(- 4KB)
	sfls_USER,				//  4KB(- 8KB)
#if defined(__ISP__)
	sfls_ISP_SHD,			//  4KB(-40KB)
	sfls_ISP_MENU,			//  4KB(-36KB)
	sfls_ISP_MENU_BAK,		//  4KB(-32KB)
	sfls_ISP_USER,			//  4KB(-28KB)
	sfls_ISP_DATA,			//  4KB(-24KB)
	sfls_ISP_BLK0,			//  4KB(-20KB)
	sfls_ISP_BLK1,			//  4KB(-16KB)
	sfls_ISP_BLK2,			//  4KB(-12KB)
#endif
#if defined(__ONVIF__)
	sfls_ONVIF,				// 12KB(-52KB)
#endif
#if defined(__WIFI__)
	sfls_WIFI,				//  4KB(-56KB)
#endif
}sfls_fixed_area;

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern UINT getDateID(void);
extern BYTE UserSave(sfls_fixed_area index);
extern BYTE UserLoad(sfls_fixed_area index);
extern BYTE UserErase(sfls_fixed_area index);

//-------------------------------------------------------------------------------------------------
// Variable
extern volatile tSystem gtSystem;
extern volatile tUser gtUser;
#if defined(__ISP__)
extern volatile BYTE *gtISP;
#endif
#if defined(__ONVIF__)
extern volatile BYTE *gtONVIF;
extern volatile UINT gtONVIF_size;
#endif
#if defined(__WIFI__)
extern volatile BYTE *gtWIFI;
extern volatile UINT gtWIFI_size;
#endif

//-------------------------------------------------------------------------------------------------
// New Test!
typedef struct {
	char strResolution[12];
	Resolution rIndex;
	UINT nWidth;
	UINT nHeight;
} ResolutionInfo;

#define ResolutionInfoLength e_resEndorUnknown

extern const ResolutionInfo listResolution[ResolutionInfoLength];


typedef struct {
	UINT nFPS;
	SensorFps sfFPS;
} VideoFPSInfo;

extern UINT ssfps2fps(UINT max_fps, SensorFps fps);
extern SensorFps fps2ssfps(UINT max_fps, UINT fps);

#endif // _USER_H_
