#ifndef _BOARD_H_
#define _BOARD_H_

//******************************************************************************
// Board
//------------------------------------------------------------------------------
// Device Version
#define EN673_VERSION			0x017					// major/minor/revision (+Boot) (+Cpu1)
//#define __EN673M__				//EN673 Revision Model 

#ifdef __EN673M__
#define EN673_MODEL				"EYENIXEN673M"			// boot/main/reset.s - gbModel(Max 12byte)
#else
#define EN673_MODEL				"EYENIXEN673 "			// boot/main/reset.s - gbModel(Max 12byte)
#endif

// System Clock Setting	(+Boot)							// 297.0MHZ(110), 283.5MHZ(105)
#define CLK_VALUE				(74)					// 270.0MHz(100), 256.5MHz(95), 243.0MHz(90), 229.5MHz(85), 216.0MHz(80)
#define BUS_CLK					(CLK_VALUE * 2700000)	//*199.8MHz( 74), 189.0MHz(70), 162.0MHz(60), 135.0MHz(50), 108.0MHz(40)

#define	ISP_CLK_MODE			1						//*0 : 148.5Mhz
														// 1 : 162.0Mhz

#define _DEBUGCODE				0						//*0
														// 1
														// 2
														// 3 : cpu0 use UART1 (+All)

#define flprintf(fmt, args...) do { printf("    %04d:%s: "fmt, __LINE__, __func__, ##args); } while(0);
//#define flprintf(fmt, args...)

//------------------------------------------------------------------------------
// Input image Setting


#ifndef USING_CONFIG_IMGPATH_SCRIPT // Use Configuration Tool (*.mk file)


	#define IMAGE_INPUT_MODE		0					//*0 : Sensor Only
														// 1 : Sensor & BT656 (Sub Channel - PIP)
														// 2 : BT656 (Sub Channel - 8/10bit mode) Only
														// 3 : BT1120 (16/20bit mode) Only

	#define USE_DIGITAL_INPUT		0					//*0 : No use
														// 1 : Use EN332T - BT656
														// 2 : Use PS2210 - BT656
														// 3 : Use PS3100 - BT656 (TBD)
														// 4 : Use ISX016 - BT656 (TBD)
														// 5 : Use EN331A - BT1120

	#define DIGITAL_INPUT_RES		0					//*0 : 1920x1080 - 16:9
														// 1 : 1280x720  - 16:9
														// 2 : 800x600   -  4:3
														// 3 : 704x480   -  3:2
														// 4 : 704x400   - 16:9
														// 5 : 640x480   -  4:3
														// 6 : 640x360   - 16:9
														// 7 : 320x240   -  4:3
														// 8 : Unknown Format

#endif // End of USING_CONFIG_IMGPATH_SCRIPT

	#define SDI_UCC_MODE			0					//*0 : SDI UCC X
														// 1 : SDI UCC O

//------------------------------------------------------------------------------
// Functionality Setting
#define __ISP__
#define __IRQ_UART0__

#ifdef OPTION_FLAG_1
#define SDIO_CH_SWAP			1						//*0 : SDIO0(MicroSD) SDIO1(Wi-Fi)
														// 1 : SDIO0(Wi-Fi) SDIO1(MicroSD)
#else
#define SDIO_CH_SWAP			0						//*0 : SDIO0(MicroSD) SDIO1(Wi-Fi)
														// 1 : SDIO0(Wi-Fi) SDIO1(MicroSD)
#endif

#define USER_DEFAULT			1						// 0 : flash memory load data(gtSystem, gtUser)
														//*1 : default load data(board.h setting)

#define DEFAULT_TIME_SET		1541030400				// GMT 2018.11.01 00:00:00 = 1541030400

#define LVDS_MARGIN_TEST		0						//*0 : No use
														// 1 : Include lvds margin test code
//#define ONVIF_PTZ_KEY

#define JTAG_DEBUG_PRINT

#define ECM_REMOTE

//------------------------------------------------------------------------------
// Ethernet Configuration
#ifdef __ETH__
#define __ETH_KSZ8081MNX__
//#define __ETH_KSZ9031RNX__

#define PHY_ADDR				1						// Ethernet PHY chip address(MDIO)

#define PHY_AUTONEG				1						// 0 : No use
														//*1 : PHY auto negotiation

#define PHY_MDIO_TEST			0						//*0 : No use
														// 1 : PHY chip address(MDIO) check mode

#define ETH_MAC_TEST			1						// 0 : No use
														//*1 : MAC address check mode(all zero? or 40bit 1? or LSB FF:FF:FF?)

#define ETH_MAC_PAUSE			1						// 0 : No use
														//*1 : Ethernet Mac Pause Enable(802.1D flow control) / ETH TX DMA delay
														// 2 : Ethernet Mac Pause Enable(802.1D flow control) / ETH_TX_IFGGAP control

#define ETH_MAC_TX_IFGGAP		10						//*9 : Interframe Gap (100Mbps: 960ns)
														// 10: Interframe Gap (100Mbps: 1040ns)

#define ETH_SHELL_TEST			0						//*0 : No use
														// 1 : Shell Test Code Enable

#define PHY_LOOPBACK_TEST		0						//*0 : No use
														// 1 : Include phy loopback test code

#define ETH_EXCESSIVE_PACKETS_CK	1					// 0 : No use
														// 1 : Blocks too many packets. However, there is a scenario where the system reboots.

#define ENX_TERMD_use			1						// 0 : No use
														// 1 : network terminal server
#endif

//------------------------------------------------------------------------------
// Wi-Fi Configuration
#ifdef __WIFI__
#define WIFI_DEFAULT			1						//*0 : flash memory load data(gtWificfg)
														// 1 : default load data(board.h setting)

// AP(Access Point) mode
#define WIFI_AP_SSID_MAC		1						// 0 : user Wi-Fi AP SSID => EN673_WIFI_TEST
														//*1 : AP SSID + MAC Address(LSB 24bit) => EN673_xxxxxx

#if WIFI_AP_SSID_MAC
#define WIFI_AP_SSID			"EN673_"				// broadcast to SSID
#else
#define WIFI_AP_SSID			"EN673_WIFI_TEST"		// broadcast to SSID
#endif
#define WIFI_AP_SWITCH			WIFI_ON					// on(WIFI_ON)? off(WIFI_OFF)?
#define WIFI_AP_AUTH			wifi_WPA2_PSK			// (wifi_OPEN) or (wifi_WPA2_PSK)
#define WIFI_AP_KEY				"12345678"				// KEY
#define WIFI_AP_CH				5						// default channel

//#define custom_MAC_UAP
#ifdef custom_MAC_UAP
#define WIFI_UAP_MACADDRESS		{0x00,0x11,0x22,0x33,0x55,0x35}
#endif

// Station mode
#define WIFI_STA_SWITCH			WIFI_OFF				// on(WIFI_ON)? off(WIFI_OFF)?
#define WIFI_STA_SSID			"eyenix_1002"			// connect to SSID
#define WIFI_STA_AUTH			wifi_WPA2_PSK			// (wifi_OPEN) or (wifi_WPA2_PSK)
#define WIFI_STA_KEY			"eyenix12"				// KEY
//#define custom_MAC_STA
#ifdef custom_MAC_STA
#define WIFI_STA_MACADDRESS		{0x00,0x11,0x22,0x33,0x55,0x34}
#endif
#endif

//------------------------------------------------------------------------------
// Network Configuration
#define NET_DHCP				DHCP_USE_OFF			// DHCP Switch
//#define __ZERO_CONFIG_IP__							// Zero configuration(Auto IP)

//#define NET_RX_POLLING								// CPU0 NET RX Polling type

#define NET_TX_BUFFER			0						//*0 : cpu0 Ethernet TX bypass
														// 1 : cpu0 Ethernet TX buffering / Max bps => 24Mbps

#define NET_LWIPERF				0						//*0 : not include iPerf
														// 1 : include CPU0 iPerf(TCP only)
														// 2 : include CPU1 iPerf(TCP only)

//------------------------------------------------------------------------------
//RTC Setting
#define __RTC_LOAD__
#ifdef __RTC_LOAD__
	#define __RTC_PCF8563__								// EVM option board
//	#define __RTC_S35390A__
#endif

//------------------------------------------------------------------------------
//Gsensor
//#define __GSS_BMA250__

//------------------------------------------------------------------------------
// TimeZone
#define DEF_TIMEZONE			e_tzP0900

//------------------------------------------------------------------------------
// FileSystem Setting
#define __FAT_LOAD__
#ifdef __FAT_LOAD__
	#define __FLS_FAT_LOAD__							// FLS_OFFSET : FLS FAT start point

	#define __DEVICE_SD__								// MicroSDcard Load
//	#define __MEM_FAT_LOAD__							// Ram disk load

	#define ENX_FTPD_use			1					// 0 : FTPD unuse
														//*1 : FTPD use

	#define FAT_FTPSERVER			FTPD_USE_ON

	#ifdef __DEVICE_SD__
//		#define __AVI_TXTS__							// Acceleration Sensor : sd save(avi file)
//		#define __IDX_WRITE__

		#define FAT_SDSAVE			SDSAVE_USE_ON

		#define AVI_BUFFERING		2					// 2s
		#define NUM_STREAM_H264 	(AVI_BUFFERING*30)	//.Caution, This value should fixed. = 2s
		#define NUM_STREAM_AUDIO	(AVI_BUFFERING*10)	// 1f = 100ms => 5s = 50f
	  #ifdef __AVI_TXTS__
		#define NUM_STREAM_ACC		(AVI_BUFFERING*15)
	  #else
		#define NUM_STREAM_ACC		0
	  #endif

		#define SDIO0_SAFE_MODE		0					//*0: 50MHz - High speed mode(default)
														// 1: 33MHz - High speed mode(low speed)
	#endif
#endif

//------------------------------------------------------------------------------
// Watchdog
#define __WDT__
#ifdef __WDT__
#define WDT_DEFAULT_TIME		10						// 10sec
#define WDT_SYS											// BOOT, CPU0, CPU1 watchdog
//#define WDT_CPU0										// CPU0 watchdog
//#define WDT_CPU1										// CPU1 watchdog
#endif

//------------------------------------------------------------------------------
#define __H264__										// H264 use
#ifdef __H264__
#define __H264_2CH__									// H264 2CH usb
//#define __H264_TEST__									// no ISP

  #ifdef __EN673M__
	#define __H264_MCH__			//Multi channel encoding mode
	#ifdef __H264_MCH__
		//#define __H264_CHECK_TIME__
		//#define BANK0_USE_STREAM
		//#define __H264_SERIAL_MODE__
	#endif
  #endif
//#define ISP_MBOX_H264_ROI_TEST
#endif

//------------------------------------------------------------------------------
// MJPEG Stream
#define __JPEG__
#ifdef __JPEG__
	#define JPEG_STREAM			1						// 0 : JPEG Stream(MJPEG) unuse
														//*1 : JPEG Stream(MJPEG) use

	#define JPEG_SNAPSHOT		1						// 0 : JPEG Still Shot unuse
														//*1 : JPEG Still Shot use
#if JPEG_SNAPSHOT
	#define JPEG_WEBVIEW		1						// 0 : JPEG Still Shot Web view unuse
														//*1 : JPEG Still Shot Web view use
#ifdef __DEVICE_SD__
	#define JPEG_SDSAVE			1						// 0 : JPEG Still Shot SDcard save unuse
														//*1 : JPEG Still Shot SDcard save use
#endif
#endif
	#define JPEG_PREVENT_OVF	1						// 0 : Disable JPEG buffer overflow protection.
														//*1: Enable JPEG buffer overflow protection.
#endif

//------------------------------------------------------------------------------
// Audio Stream
#define __AUDIO__
#ifdef __AUDIO__
//	#define __AUD_TLV320AIC3100__		// Option board Ver 1.0
	#define __AUD_ES8374__			// Option board Ver 2.0

	#define AUDIO_OUT			DEF_ON

	#define AUDIO_ADD_MUSIC		1						//*0 : Does not include test music.
														// 1 : Includes test music. Outputs to speakers. An additional 94720 bytes is required. It works with the shell command "i2s mode 0"(gptMsgShare.AUD_TX_MODE == 0). "AUD_TX_MODE" is 0 by default.

	#define AUDIO_RX			0						//*0 : unuse
														// 1 : use(ONVIF backchannel)
														// 2 : use(custom type)

	#define AUDIO_RX_TCP_PORT	4999
	#define AUDIO_RX_UDP_PORT	5000

  #ifdef __DEVICE_SD__
	#define FAT_SDSAVE_AUD		1						// 0 : AVI file audio save unuse
														//*1 : AVI file audio save use(Test...)
  #endif
#else
	#undef NUM_STREAM_AUDIO
	#define NUM_STREAM_AUDIO	0
#endif

//------------------------------------------------------------------------------
// RTMP Stream
//#define RTMP_SUPPORT
#ifdef RTMP_SUPPORT
	#define TEST_SERVER2								// google live stream server
//	#define TEST_SERVER5								// google live stream server
#endif

//------------------------------------------------------------------------------
// ONVIF Option
#ifdef __ONVIF__
#define ONVIF_METADATA			0						// 0 : disable metadata stream
														//*1 : enable metadata stream
#define ONVIF_BACKCHANNEL		0						// 0 : disable audio back channel
														//*1 : enable audio back channel

//ONVIF INTERFACE USE ============================
#define USE_DISCOVERY_INTERFACE
#define USE_DEVICE_INTERFACE
#define USE_MEDIA_INTERFACE
#define USE_EVENT_INTERFACE
#define ONVIF_NOTIFY_ENABLED
#define USE_ONVIF_SECURITY
#define USE_IMAGING_INTERFACE
//#define USE_ANALYTICS_INTERFACE
//#define USE_DEVICE_IO_INTERFACE
#define USE_PANTILT_INTERFACE
//#define USE_DISPLAY_INTERFACE
//#define USE_RECEIVER_INTERFACE
//#define USE_RECORDING_INTERFACE
//#define USE_SEARCHING_INTERFACE
//#define USE_REPLAY_INTERFACE

//#define ENX_ONVIF_DEBUG
#ifdef ENX_ONVIF_DEBUG
	#define dvprintf(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dvprintf(fmt, args...) do {} while(0);
#endif

//#define COUNTLOCK
#ifdef COUNTLOCK
#define TCPIP_MAX_INPKTS_QUEUED 10//60/6
#endif
#endif

//------------------------------------------------------------------------------
// IPUtil option
#define ENX_IPUTIL_use			1						// 0 : IPUTIL unuse
														//*1 : IPUTIL use

//------------------------------------------------------------------------------
// Nework Debug option
#define ENX_NETWORK_Debug		0						//*0 : Network debug unuse
														// 1 : Network debug use

//------------------------------------------------------------------------------
// SNTPC option
#define ENX_SNTPC_use			1						// 0 : SNTPC unuse
														//*1 : SNTPC use

#define SNTP_START				DEF_ON					// DEF_OFF(0): not work when booting.
														//*DEF_ON(1): work when booting.

//------------------------------------------------------------------------------
// UPNPD option
#define ENX_UPNPD_use			0						//*0 : UPNPD unuse
														// 1 : UPNPD use

//------------------------------------------------------------------------------
// SMTP option
#define ENX_SMTPC_use			0						//*0 : SMTPC unuse
														// 1 : SMTPC use

//------------------------------------------------------------------------------
// DDNS option
#define ENX_DDNSC_use			0						//*0 : DDNSC unuse
														// 1 : DDNSC use

//------------------------------------------------------------------------------
// DownScaler Switch(0:H264main, 1:H264sub, 2:MJPEG)
#define HW_DS0					0
#define HW_DS1					1

//------------------------------------------------------------------------------
// Hardware OSD
#define __HW_OSD__
#ifdef __HW_OSD__
	#define __HW_OSD_FRAME__
	#define __HW_OSD_TIME__
	#define __HW_OSD_H264__
	#define __HW_OSD_JPEG__
#endif

//------------------------------------------------------------------------------
// Stream URL
#define H264URL					"h264"					// H264 CH.1
#ifdef __H264_2CH__
	#define HSUBURL 			"hsub"					// H264 CH.2
#endif
#define JPEGURL					"jpeg"					// M-JPEG
//#define PLAYBACK	"h264/playback"						// playback
//#define PLAYBACKRETRY	"h264/playback/video"			// playback

#define RTSPoverHTTP			1						//*0 : RTSP over HTTP unuse
														// 1 : Malfunction!!!

//------------------------------------------------------------------------------
// Sensor Frame Rate
//#define __SS_60RATE__
//#define __SS_50RATE__
#define __SS_30RATE__
//#define __SS_25RATE__
//#define __SS_15RATE__

//ISP Default Image Size & Default Video Setting
#if (DIGITAL_INPUT_RES==0)
#define SENSOR_SIZE_W		1920
#define SENSOR_SIZE_H		1080
#elif (DIGITAL_INPUT_RES==1)
#define SENSOR_SIZE_W		1280
#define SENSOR_SIZE_H		 720
#elif (DIGITAL_INPUT_RES==2)
#define SENSOR_SIZE_W		 800
#define SENSOR_SIZE_H		 600
#elif (DIGITAL_INPUT_RES==3)
#define SENSOR_SIZE_W		 704
#define SENSOR_SIZE_H		 480
#elif (DIGITAL_INPUT_RES==4)
#define SENSOR_SIZE_W		 704
#define SENSOR_SIZE_H		 400
#elif (DIGITAL_INPUT_RES==5)
#define SENSOR_SIZE_W		 640
#define SENSOR_SIZE_H		 480
#elif (DIGITAL_INPUT_RES==6)
#define SENSOR_SIZE_W		 640
#define SENSOR_SIZE_H		 360
#elif (DIGITAL_INPUT_RES==7)
#define SENSOR_SIZE_W		 320
#define SENSOR_SIZE_H		 240
#else // Unknown Foramt - Custom res??
#define SENSOR_SIZE_W		   0
#define SENSOR_SIZE_H		   0
#endif

#define	IMG_JPEG_RES		DIGITAL_INPUT_RES		// Maximum JPEG resolution
#define	IMG_H264_RES		DIGITAL_INPUT_RES		// Maximum 1ch H.264 resolution
#define	IMG_H264_2CH_RES	e_res640x360			// Maximum 2ch H.264 resolution

#ifdef __SS_60RATE__
	#define IMG_MAX_FPS			60						// Maximum Frame Rate
	#define IMG_JPEG_FPS		e_s60f60				// Default JPEG Frame Rate
	#define IMG_H264_FPS		e_s60f60				// Default 1ch H.264 Frame Rate
	#define IMG_H264_2CH_FPS	e_s60f60				// Default 2ch H.264 Frame Rate
	#define IMG_H264_IDR		60
	#define IMG_H264_2CH_IDR	60
#endif
#ifdef __SS_30RATE__
	#define IMG_MAX_FPS			30						// Maximum Frame Rate
	#define IMG_JPEG_FPS		e_s30f30				// Default JPEG Frame Rate
	#define IMG_H264_FPS		e_s30f30				// Default 1ch H.264 Frame Rate
	#define IMG_H264_2CH_FPS	e_s30f30				// Default 2ch H.264 Frame Rate
	#define IMG_H264_IDR		30
	#define IMG_H264_2CH_IDR	30
#endif
#ifdef __SS_50RATE__
	#define IMG_MAX_FPS			50						// Maximum Frame Rate
	#define IMG_JPEG_FPS		e_s50f50				// Default JPEG Frame Rate
	#define IMG_H264_FPS		e_s50f50				// Default 1ch H.264 Frame Rate
	#define IMG_H264_2CH_FPS	e_s50f50				// Default 2ch H.264 Frame Rate
	#define IMG_H264_IDR		50
	#define IMG_H264_2CH_IDR	50
#endif
#ifdef __SS_25RATE__
	#define IMG_MAX_FPS			25						// Maximum Frame Rate
	#define IMG_JPEG_FPS		e_s25f25				// Default JPEG Frame Rate
	#define IMG_H264_FPS		e_s25f25				// Default 1ch H.264 Frame Rate
	#define IMG_H264_2CH_FPS	e_s25f25				// Default 2ch H.264 Frame Rate
	#define IMG_H264_IDR		25
	#define IMG_H264_2CH_IDR	25
#endif
#ifdef __SS_15RATE__
	#define IMG_MAX_FPS			15						// Maximum Frame Rate
	#define IMG_JPEG_FPS		e_s15f15				// Default JPEG Frame Rate
	#define IMG_H264_FPS		e_s15f15				// Default 1ch H.264 Frame Rate
	#define IMG_H264_2CH_FPS	e_s15f15				// Default 2ch H.264 Frame Rate
	#define IMG_H264_IDR		15
	#define IMG_H264_2CH_IDR	15
#endif

#define IMG_H264_PROFILE		e_pmBaseline			// Default 1ch H.264 Profile(e_pmBaseline, e_pmMainCB)
#define IMG_H264_2CH_PROFILE	e_pmBaseline			// Default 2ch H.264 Profile(e_pmBaseline, e_pmMainCB)

#define IMG_JPEG_QULITY			50						// Default JPEG Image Quality
#define IMG_H264_QULITY			40						// Default 1ch H.264 Image Quality
#define IMG_H264_2CH_QULITY		50						// Default 2ch H.264 Image Quality

#define IMG_BR_J				e_brmCBR				// QBRmode(OFF), VBRmode, CBRmode
#define IMG_BR_JR				e_jc20m					// VBRmode, CBRmode only

#define IMG_BR_H				e_brmCVBR				// QBRmode(OFF), VBRmode, CBRmode
#define IMG_BR_HR				e_hc10m					// VBRmode, CBRmode only

#define IMG_BR_H2				e_brmCBR				// QBRmode(OFF), VBRmode, CBRmode
#define IMG_BR_H2R				e_hc2m					// VBRmode, CBRmode only

//encoder bitrate operation
#define ISP_AE_CHECK
#define ISP_AF_CHECK

//******************************************************************************
// 2. Memory Size & Base Address Definition
//------------------------------------------------------------------------------
#define RAM0_SIZE				(     32*1024)			//	32kB (Local) * 2
#define RAM1_SIZE				(     32*1024)			//	32kB (Shared) * 2

#define SFLS_SIZE				(16*1024*1024)
	#define BOOT_ADDRESS		(	  64*1024)			// boot.bin size 64Kbyte
#ifdef __WIFI__
	#define SFLS_WIFI_SIZE		(      4*1024)			// WIFI data area size in internal flash
#else
	#define SFLS_WIFI_SIZE		(           0)
#endif
#ifdef __ONVIF__
	#define SFLS_ONVIF_SIZE		(     16*1024)			// ONVIF data area size in internal flash
#else
	#define SFLS_ONVIF_SIZE		(           0)
#endif
#ifdef __ISP__
	#define SFLS_ISP_SIZE		(     32*1024)			// ISP data area size in internal flash
	#define SFLS_ISP_UNIT_SIZE	(      4*1024)			// ISP data area uint size in internal flash
#else
	#define SFLS_ISP_SIZE		(           0)
	#define SFLS_ISP_UNIT_SIZE	(      	    0)
#endif
	#define SFLS_USER_SIZE		(      4*1024)			// User data area size in internal flash
	#define SFLS_SYSTEM_SIZE	(      4*1024)			// System data area size in internal flash
	#define SFLS_FIXED_SIZE		(SFLS_ONVIF_SIZE+SFLS_ISP_SIZE+SFLS_USER_SIZE+SFLS_SYSTEM_SIZE+SFLS_WIFI_SIZE)
#ifdef __FLS_FAT_LOAD__
	#define FLS_OFFSET			(13*1024*1024)
#endif
	#define FW_UPDATE_AREA		( 6*1024*1024+512*1024)	// 6.5Mbyte

#define DDR_SIZE				(64*1024*1024)
#ifdef __WIFI__
	#define CPU0_ROM_SIZE		(  1*1024*1024+768*1024) // CPU0 processor op-code size 1M+768K
#else
	#define CPU0_ROM_SIZE		(  1*1024*1024+256*1024) // CPU0 processor op-code size 1M+256K
#endif

#ifdef __ACTIVEX__
	#ifdef __ONVIF__
		#define CPU1_ROM_SIZE	(  6*1024*1024+384*1024)// CPU1 processor op-code size 6M+384K
	#else
		#define CPU1_ROM_SIZE	(  3*1024*1024)			// CPU1 processor op-code size 3M
	#endif
#else
	#ifdef __ONVIF__
		#define CPU1_ROM_SIZE	(  4*1024*1024+512*1024)// CPU1 processor op-code size 4M+512K
	#else
		#define CPU1_ROM_SIZE	(  1*1024*1024+256*1024)// CPU1 processor op-code size 1M+256K
	#endif
#endif

	#define CPU0_HEAP_SIZE		(  768*1024)			// 
	#define CPU1_HEAP_SIZE		(CPU1_HEAP_END-CPU1_HEAP_BASE)	// Bank0의 남은 공간 사용

#ifdef RTMP_SUPPORT
	#define RTMP_CODEC_SIZE 	(      10*1024)			// 
	#define RTMP_VIDEO_SIZE 	(  2*1024*1024)			// 
	#define RTMP_AUDIO_SIZE 	(     256*1024)			// 
#else
	#define RTMP_CODEC_SIZE 	(	         0) 		// 
	#define RTMP_VIDEO_SIZE 	(            0) 		// 
	#define RTMP_AUDIO_SIZE 	(	         0) 		// 
#endif

	#define NETRX_BUF_SIZE		(     512*1024)			// 
#if (NET_TX_BUFFER==1)
	#define NETTX_BUF_SIZE		(     512*1024)			// 
#else
	#define NETTX_BUF_SIZE		(            0)			// 
#endif

#ifdef __JPEG__
// (StreamUser(2)+JPEG_BUFF_SIZE(2)) * JPEG_MAX_SIZE
//	#define JPEG_MAX_SIZE		(     2097152)
//	#define JPEG_MAX_SIZE		(     1048576)
//	#define JPEG_MAX_SIZE		(      786432)
	#define JPEG_MAX_SIZE		(      524288)
//	#define JPEG_MAX_SIZE		(      262144)
	#define JPEG_DEF_BUF_CNT	(2)
	#define JPEG_STREAM_BUF_CNT	(2)
#if JPEG_STREAM
	#define JPEG_BUF_CNT		(JPEG_DEF_BUF_CNT+JPEG_STREAM_BUF_CNT)
#elif JPEG_SNAPSHOT
	#define JPEG_BUF_CNT		(JPEG_DEF_BUF_CNT)
#else
	#define JPEG_BUF_CNT		(0)
#endif	// #if JPEG_STREAM
	#define JPEG_IMG_SIZE		(JPEG_BUF_CNT*JPEG_MAX_SIZE)
#else
	#define JPEG_MAX_SIZE		(0)
	#define JPEG_IMG_SIZE		(0)
#endif	// #ifdef __JPEG__
#ifdef __DEVICE_SD__
	#define SDSAVE_BUF_SIZE		(     512*1024)			// write unit: Heap(malloc) use / not fixed memory address
	#define TXTS_BUF_SIZE		(	   32*1024)			// write unit: Heap(malloc) use / not fixed memory address
#else
	#define SDSAVE_BUF_SIZE 	(            0)
	#define TXTS_BUF_SIZE		(            0)
#endif
#ifdef __AUDIO__
	#define AUDIO_BUF_SIZE		(      64*1024)
#else
	#define AUDIO_BUF_SIZE	 	(            0)
#endif
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))	// Main ISP Input Image
	#define ISP_BUF_SIZE		(  2*1024*1024+640*1024)// ISP processing memory buffer
#else
	#define ISP_BUF_SIZE	 	(            0)
#endif
	#define FW_UPDATE_SIZE		( 16*1024*1024)			// Max Firmware size

#ifdef __MEM_FAT_LOAD__
	#define MEMDISK_SIZE			(  5*1024*1024)
	#define H264_STRM_SIZE			( 48*1024*1024 - MEMDISK_SIZE)
#else
	#define H264_STRM_SIZE			( 48*1024*1024)	
#endif


//#define H264_STRM_SIZE			( 48*1024*1024 - JPEG_IMG_SIZE)
#define H264_STRM_SIZE_1CH		(            0)			// auto set
#define H264_STRM_SIZE_2CH		(            0)			// auto set

#ifdef JTAG_DEBUG_PRINT
#define JTAG_PRINT_SIZE			(     128*1024)
#else
#define JTAG_PRINT_SIZE			(            0)
#endif

#ifdef BANK0_USE_STREAM
#define BANK0_STREAM_SIZE 		(	  0x600000) //6M byte
#else
#define BANK0_STREAM_SIZE 		(			 0)
#endif

//-------------------------------------------------------------------------------------------------
// Base address
#define SYSRAM_BASE				0x04000000	//	OK
#define SH0RAM_BASE				0xf0000000	//	OK
#define SH1RAM_BASE				0xf0010000	//	OK

#define SFLS_BASE				0x06000000	//	OK
	#define SFLS_END			(SFLS_BASE + SFLS_SIZE)
	#define SFLS_BOOT_BASE		(SFLS_BASE + BOOT_ADDRESS)
	#define SFLS_SYSTEM_BASE	(SFLS_END - SFLS_SYSTEM_SIZE)		//  4KB
	#define SFLS_USER_BASE		(SFLS_SYSTEM_BASE - SFLS_USER_SIZE) //	4KB
	#define SFLS_ISP_BASE		(SFLS_USER_BASE - SFLS_ISP_SIZE)	// 32KB
	#define SFLS_ONVIF_BASE		(SFLS_ISP_BASE - SFLS_ONVIF_SIZE)	// 16KB
	#define SFLS_WIFI_BASE		(SFLS_ONVIF_BASE - SFLS_WIFI_SIZE)	//  4KB

//#############################################################################################################################
//#             BANK0            #             BANK1            #             BANK2 		   #             BANK3            #
//#############################################################################################################################
//# CPU(code, heap), ISP         |RAW0|Stream0                         |Stream1 |RAW1|RAW1|RAW0|     H.264               |JPEG#
//# Network, Audio               | Y  |                                |        | Y  | C  | C  |    internal             |	  #
//#############################################################################################################################
//# CPU(code, heap), ISP		 |RAW0|Stream0						   |Stream1 |RAW1|RAW1|RAW0|	 H.264				      #
//# Network, Audio, JPEG		 | Y  | 							   |		| Y  | C  | C  |	internal			  	  #
//#############################################################################################################################

#define DDR_BASE				0x08000000	//	OK
	#define DDR_BK0_BASE		0x08000000	//	OK
	#define DDR_BK0_END			0x08FFFFFF	//	OK
	#define DDR_BK1_BASE		0x09000000	//	OK
	#define DDR_BK2_BASE		0x0A000000	//	OK
	#define DDR_BK3_BASE		0x0B000000	//	OK
	#define DDR_END				(DDR_BASE + DDR_SIZE)

	#define	CPU0_ROM_BASE		(DDR_BK0_BASE)
		#define CPU0_ROM_END	(CPU0_ROM_BASE + CPU0_ROM_SIZE)
	#define	CPU1_ROM_BASE		(CPU0_ROM_END)
		#define CPU1_ROM_END	(CPU1_ROM_BASE + CPU1_ROM_SIZE)
	#define CPU0_HEAP_BASE		(CPU1_ROM_END)
		#define CPU0_HEAP_END	(CPU0_HEAP_BASE + CPU0_HEAP_SIZE)
	#define ISP_BUF_BASE		(CPU0_HEAP_END)
		#define ISP_BUF_END 	(ISP_BUF_BASE + ISP_BUF_SIZE)
	#define JTAG_PRINT0_BASE	(ISP_BUF_END)
		#define JTAG_PRINT0_END	(JTAG_PRINT0_BASE + JTAG_PRINT_SIZE)
	#define JTAG_PRINT1_BASE	(JTAG_PRINT0_END)
		#define JTAG_PRINT1_END	(JTAG_PRINT1_BASE + JTAG_PRINT_SIZE)
	#define NETRX0_BUF_BASE		(JTAG_PRINT1_END)
		#define NETRX0_BUF_END	(NETRX0_BUF_BASE + (NETRX_BUF_SIZE>>1))
	#define NETRX1_BUF_BASE		(NETRX0_BUF_END)
		#define NETRX1_BUF_END	(NETRX1_BUF_BASE + (NETRX_BUF_SIZE>>1))
	#define NETTX_BUF_BASE		(NETRX1_BUF_END)
		#define NETTX_BUF_END	(NETTX_BUF_BASE + NETTX_BUF_SIZE)
	#define AUD_DATA_BASE		(NETTX_BUF_END)
		#define AUD_DATA_END	(AUD_DATA_BASE + AUDIO_BUF_SIZE)
	#define AUD_SPKBUF_BASE		(AUD_DATA_END)
		#define AUD_SPKBUF_END	(AUD_SPKBUF_BASE + AUDIO_BUF_SIZE)
	#define RTMP_CODEC_BASE		(AUD_SPKBUF_END)
		#define RTMP_CODEC_END	(RTMP_CODEC_BASE + RTMP_CODEC_SIZE)
	#define RTMP_VIDEO_BASE 	(RTMP_CODEC_END)
		#define RTMP_VIDEO_END	(RTMP_VIDEO_BASE + RTMP_VIDEO_SIZE)
	#define RTMP_AUDIO_BASE 	(RTMP_VIDEO_END)
		#define RTMP_AUDIO_END	(RTMP_AUDIO_BASE + RTMP_AUDIO_SIZE)
	#define JPEG_IMG_BASE		(RTMP_AUDIO_END)
		#define JPEG_IMG_END	(JPEG_IMG_BASE + JPEG_IMG_SIZE)
#ifdef __H264_MCH__
	#ifdef BANK0_USE_STREAM
		#define BANK0_STREAM_BASE (DDR_BK0_END-BANK0_STREAM_SIZE)
		#define CORE_DDR_END	(BANK0_STREAM_BASE-0x1000)
	#else
		#define CORE_DDR_END	(DDR_BK0_END)
	#endif
#else
	#define CORE_DDR_END	(DDR_BK0_END)
#endif
	#define CPU1_HEAP_BASE		(JPEG_IMG_END)
//	#define CPU1_HEAP_BASE		(RTMP_AUDIO_END)
		#define CPU1_HEAP_END	(CORE_DDR_END)

#ifdef __ONVIF__
#if (CPU1_HEAP_END-CPU1_HEAP_BASE) < 0x180000	// 3.5MB or more.
#error "There is not enough CPU1 heap memory."
#endif
#else
#if (CPU1_HEAP_END-CPU1_HEAP_BASE) < 0x300000 // 3MB or more.
#error "There is not enough CPU1 heap memory."
#endif
#endif

	#define H264_STRM_BASE		(DDR_BK1_BASE)
		#define H264_STRM_END	(H264_STRM_BASE + H264_STRM_SIZE)
#ifdef __MEM_FAT_LOAD__
	#define MEMDISK_BASE		(H264_STRM_END)
		#define MEMDISK_END		(MEMDISK_BASE + MEMDISK_SIZE)
#endif

//	#define JPEG_IMG_BASE		(H264_STRM_END)
//		#define JPEG_IMG_END	(JPEG_IMG_BASE + JPEG_IMG_SIZE)

	#define FW_UPDATE_BASE		(DDR_BK2_BASE)
		#define FW_UPDATE_END	(FW_UPDATE_BASE + FW_UPDATE_SIZE)

//-------------------------------------------------------------------------------------------------
// Internal SRAM (32KB * 4)
#define SYSRAM_SIZE				(RAM0_SIZE)		// 	CPU0 & CPU1 - internal sram - 32KB
#define SH0RAM_SIZE				(RAM1_SIZE)		// 	CPU0 | CPU1 - share0 sram   - 32KB
#define SH1RAM_SIZE				(RAM1_SIZE)		// 	CPU0 | CPU1 - share1 sram   - 32KB

//-------------------------------------------------------------------------------------------------
// Check board.h
#ifdef __H264_TEST__
#ifdef __ISP__
#error "board.h unused ISP!!"
#endif
#endif

#ifdef __FLS_FAT_LOAD__
#if FLS_OFFSET > (14*1024*1024)
#error "flash memory file system offset 1"
#endif

#if FLS_OFFSET < ((CPU0_ROM_SIZE+CPU1_ROM_SIZE+SFLS_FIXED_SIZE)*2)
#error "flash memory file system offset 2"
#endif
#endif

#if H264_STRM_END > DDR_END
#error "board.h memory overflow(JPEG_IMG_END > DDR_END)"
#endif

#ifdef __ONVIF__
#if (ONVIF_BACKCHANNEL==1)
#if (AUDIO_RX!=1)
#error "ONVIF Backchannel -> #define AUDIO_RX != 1"
#endif
#endif

#if (AUDIO_RX==1)
#if (ONVIF_BACKCHANNEL!=1)
#error "ONVIF Backchannel -> #define ONVIF_BACKCHANNEL != 1"
#endif
#endif
#endif

#if 0 // Used for ISP development ----------
	#undef USER_DEFAULT
	#define USER_DEFAULT			0						//*0 : flash memory load data(gtSystem, gtUser)

	#undef HW_DS0
	#undef HW_DS1
	#define HW_DS0				1/*0*/
	#define HW_DS1				2/*1*/

	#undef IMG_JPEG_RES
	#undef IMG_JPEG_QULITY
	#undef IMG_BR_J
	#define	IMG_JPEG_RES		e_res704x400/*e_res1920x1080*/			// Maximum JPEG resolution
	#define IMG_JPEG_QULITY		10/*50*/						// Default JPEG Image Quality
	#define IMG_BR_J			e_brmQBR/*e_brmCBR*/				// QBRmode(OFF), VBRmode, CBRmode

	#undef IMG_BR_HR
	#define IMG_BR_HR			e_hc8m/*e_hc16m*/
#endif//------------------------------------

#endif//_BOARD_H_
