#ifndef _MSG_H_
#define _MSG_H_

#include "develop.h"
#include "dev_types.h"

//*************************************************************************************************
// Queue
//-------------------------------------------------------------------------------------------------
enum {
	ETH_DATA = 1,
	WLIF_DATA,
	WLIF_EVT,
};

typedef enum { // netif array index
#if defined(__ETH__)
	enlETHERNET,
#endif
#if defined(__WIFI__)
	enlWIFISTA,
	enlWIFIUAP,
#endif
	enlEnd,
} NETIFLIST;

enum {
	WLIF_UAP_START,
	WLIF_STA_START,			/*WiFi Working Start by cpu1 */
	WLIF_UAP_ACTIVE,
	WLIF_STA_ACTIVE,
	WLIF_STA_CONNECT_WAIT,
	WLIF_CONNECTED,
	WLIF_DIS_CONNECTED,
	WLIF_SCAN,
	WLIF_UAP_BSS_ON,
	WLIF_UAP_BSS_OFF,
	WLIF_UAP_BSS_ERROR,
	WLIF_UAP_DISCONN,
	WLIF_INIT_FAIL = 0x1000,
	WLIF_STA_NO_AP = 0x1001,
	WLIF_STA_WPA_AUTH_FAIL = 0x1002,
	WLIF_NONE = 0xFFFF,
};

typedef struct {									// h264 stream : Cop -> Main
	UINT	iftype;								//	netif type(NETIFLIST)
	UINT	type;								//	0 - sta, 1 -uap
	UINT	event;								//	0 connected, 1 disconnected, 2 scan
	BYTE	mac_addr[8];						//	wifi mac address(6byte).
} wlif_priv_t;

// Ethernet bd

#define NETRX_ALIGN_SIZE   	8
#define NETRX_ALIGN(x) 		(((x) + NETRX_ALIGN_SIZE - 1) & ~(NETRX_ALIGN_SIZE - 1))

#define MSG_NETRX_NUM		150		// 1packet(NETRX_ALIGN(1514)==1520) * 170 < (256*1024)KB
#define NETRX_TYPE_CPU0		(1)
#define NETRX_TYPE_CPU1		(2)
#define NETRX_TYPE_SHARE	(NETRX_TYPE_CPU0|NETRX_TYPE_CPU1)
#define NETRX_TYPE_NONE		(4)

#define cNRQueue_isfull(cQueue)	(((((cQueue)->tail+1) % MSG_NETRX_NUM) == (cQueue)->head) ? DEF_OK : DEF_FAIL)
#define cNRQueue_isempty(cQueue)	(((cQueue)->head == (cQueue)->tail) ? DEF_OK : DEF_FAIL)
#define cNRQueue0_isfull(cQueue) (((((cQueue)->cpu0_tail+1) % MSG_NETRX_NUM) == (cQueue)->cpu0_head) ? DEF_OK : DEF_FAIL)
#define cNRQueue0_isempty(cQueue)        (((cQueue)->cpu0_head == (cQueue)->cpu0_tail) ? DEF_OK : DEF_FAIL)

typedef struct {
#if (ETH_MAC_PAUSE==1)
	UINT tx_pause;
	UINT tx_go;
	UINT macp_time;
	UINT macp_count;
#endif
#if (ETH_MAC_PAUSE==2)
	UINT tx_ifggap;
#endif
	BYTE buffer[1520];
} msgq_net_rx_shared_t;

typedef struct {
	BYTE flag;									// (0)drop? (1)cpu0? (2)cpu1? (3)all?
	BYTE type;									// (1)Eth, (2)Wi-Fi, (3)Wi-Fi evt
	WORD lenth;									// packet lenth
	UINT data;									// packet data address
	NETIFLIST iftype;							// netif type
} pkt_info;	// 12byte

typedef struct {
	UINT base;
	UINT end;
	UINT head_pos;

	UINT tail;
	UINT head;

	pkt_info info[MSG_NETRX_NUM];	// 12 * MSG_NETRX_NUM = 1800byte
} msgq_net_rx_t;

#if (NET_TX_BUFFER==1)
#define MSG_NETTX_NUM		300

typedef struct {
	UINT base;
	UINT end;
	UINT head_pos;
	UINT tot_num;
	UINT timestamp;

	UINT cpu0_tail;	// output
	UINT cpu0_head;	// input

	// ---- 32byte

	BYTE buffer[1520];

	pkt_info info0[MSG_NETTX_NUM];
} msgq_net_tx_t;
#endif

#define MSG_ETH_NUM			15
typedef struct{									// Ethernet buffer descriptor
    UINT    ready;                              // cpu is ready,
    UINT	msg_type;							//		0 : eth, 1 : mac
	UINT	lock;								//		"
	UINT	head;								//		"
	UINT	tail;								//		"
	UINT	num;								//		"
	UINT	netif[MSG_ETH_NUM];
	UINT    type[MSG_ETH_NUM];                  //
	UINT	length[MSG_ETH_NUM];				//		"
//	BYTE	dummy2[384];						//		"
	BYTE	pkt[MSG_ETH_NUM][1520];				//		"
} msgq_eth_t;									//		"

//...............................................
// Pass message
#define MSGQ_PASS_NUM			62
typedef struct{									// Job : Main <-> Cop
	UINT head;									//		"
	UINT tail;									//		"
	UINT tot_num;								//		"
	UINT _res;									//		"
	UINT cmd[MSGQ_PASS_NUM];					//		"
	UINT arg[MSGQ_PASS_NUM];					//		"
} msgq_pass_t;								// 512Byte

//...............................................
// Shell message(140Byte)
#define MSG_SHELL_NUM		3
#define MSG_SHELL_MAXLEN	40
typedef struct{									// Shell : cpu0 -> cpu1
	UINT lock;									//		"
	UINT head;									//		"
	UINT tail;									//		"
	UINT index;									//		"
	UINT tot_num;								//		"
	char arg[MSG_SHELL_NUM][MSG_SHELL_MAXLEN];	//		"
} msgq_shell_t;								//		"

//...............................................
// H.264, G.711 data
#ifdef __DEVICE_SD__
#define MSG_RECORD_NUM		(NUM_STREAM_H264+NUM_STREAM_AUDIO)

enum {
	eSTREAMMSG_H2641I = 1,
	eSTREAMMSG_H2641P,
//	eSTREAMMSG_H2642I,
//	eSTREAMMSG_H2642P,
//	eSTREAMMSG_JPEG,
	eSTREAMMSG_AUDIO,
	eSTREAMMSG_TXTS
};

typedef struct {
	UINT addr;									// address
	UINT size;									// size
	UINT type	: 8;							// 1,2(h264 1) 3,4(h264 2) 5(jpeg) 6(audio) 7(gss) 8(gps)
	UINT ts		:24;							// time stamp
} stream_info;	// 12byte

typedef struct {
	UINT tot_num;								// = MSG_STREAM_NUM

	UINT tail;									// input
	UINT head;									// output(streaming)

	stream_info info[MSG_RECORD_NUM];			// 12 * MSG_STREAM_NUM = 1800byte
} msgq_record_t;
#endif

typedef struct {
    UINT ready;
    UINT resp;                                  //0: no resp, 1 : ok, 2 : waiting
    BYTE cmd[4096];								// EYESH_CMDLEN(cpu1/main/inter_shell.h)
} msgq_wlif_cmd_t;

#define WLIF_NUM_EVENT 10
typedef struct {
	UINT head;
	UINT tail;
	UINT num;
	UINT cnt;
	UINT lock[WLIF_NUM_EVENT];
	wlif_priv_t event[WLIF_NUM_EVENT];
} msgq_wlif_evt_t;

#if (ENX_FTPD_use==1)
#define FTPDATA_info_MAX 10

typedef struct {
	UINT addr;
	UINT size;
	UINT flag;
}FTPDATA_data;

enum {
	eFtpd_NONE = 0,
	eFtpd_DATAPORT_OPEN,	// 1
	eFtpd_DATAPORT_OK,		// 2
	eFtpd_DATAPORT_CONN,	// 3
	eFtpd_DATAPORT_PASV,	// 4
	eFtpd_DATAPORT_LIST,	// 5
	eFtpd_DATAPORT_RETR,	// 6
	eFtpd_DATAPORT_STOR,	// 7
	eFtpd_DATAPORT_STOP,	// 8
	eFtpd_DATAPORT_CLOSE,	// 9
	eFtpd_DATAPORT_CLOSE2,	// 10
	eFtpd_FAIL = 0xFFFFFFFF
};

typedef struct {
	UINT total_size;
	UINT port;
	UINT cmd;
	UINT state;
	UINT datastate;
	FTPDATA_data data[2];
}FTPDATA_info;

typedef struct {
	FTPDATA_info ftp_data[FTPDATA_info_MAX];
}cop2cpu_ftpdmb_t;
#endif

typedef struct {
	UINT server_addr;
	WORD server_port;
	WORD client_port;
	UINT client_addr;
	WORD video_type;	// H.264(1,2), MJPEG
	WORD rtp_type;		// UDP, TCP
	UINT connect_time;	// connect time, unused=>0xffffffff
}rtsp_connect_info;

enum {
	JPEG_SNAP_IDE = 0,
	JPEG_SNAP_PROC= 1,
	JPEG_SNAP_STR = 2,
	JPEG_SNAP_ING = 3,
	JPEG_SNAP_END = 4,
	JPEG_SNAP_JTAG= 9
};

enum {
	OSD_FRAME  = 0x01,
	OSD_TIME   = 0x02,
	OSD_H264_1 = 0x04,
	OSD_H264_2 = 0x08,
	OSD_JPEG   = 0x10
};

#if (PHY_LOOPBACK_TEST==1)
enum {
	ePlb_idle,		// 0:none, idle
	ePlb_ok,		// 1:test oK
	ePlb_data,		// 2:data error, compare fail
	ePlb_zero,		// 3:zero data
	ePlb_outside,	// 4:data length underflow or overflow
	ePlb_timeout,	// 5:timeout
};
#endif

typedef enum {
	eDltCPU0ETHRX_dp	= 0x00000001,	// hexDump: RxPkt0
	eDltCPU0ETHPROC		= 0x00000002,	// "a"
	eDltCPU1ETHRX_dp	= 0x00000004,	// hexDump: RxPkt1
	eDltCPU1ETHPROC		= 0x00000008,	// "b"
	eDltETHRXSIZE		= 0x00000010,	// (data)
	eDltCPU1ETHRX_dp1	= 0x00000020,	// hexDump: RxPkt1(in CPU1)
	eDltRES0			= 0x00000040,	// -
	eDltCPU0UAPPROC		= 0x00000080, 	// "c" - ...
	eDltCPU1UAPPROC		= 0x00000100,	// "d" - ...
	eDltCPU0STAPROC 	= 0x00000200, 	// "c" - ...
	eDltCPU1STAPROC 	= 0x00000400, 	// "d" - ...
	eDltCPU1ETHRX_err	= 0x00000800,	// (data)
	eDltCPU0ETHPAUSECNT = 0x00001000,	// log output
	eDltEnd,
} Debug_LogType;

typedef struct {
	// Used by ECM(JTAG) or etc.
	// Be careful not to change the memory address.
	UINT JPEG_STILL_ADDR;	// fixed! 0xF0010000, ECMb
	UINT JPEG_STILL_SIZE;	// fixed! 0xF0010004, ECMb
	UINT JPEG_STILL_FLAG;	// fixed! 0xF0010008, ECMb
	UINT WIFI_INTER_LOG;	// fixed! 0xF001000C, Wi-Fi library
	INT JPEG_STILL_REF;		// fixed! 0xF0010010, CODE
	UINT _JTAG_PRINT_SIZE;	// fixed! 0xF0010014, ECMb, Buffer Size
	UINT _JTAG_PRINT0_START;// fixed! 0xF0010018, ECMb, CPU0 JTAG log base
	UINT _JTAG_PRINT0_POINT;// fixed! 0xF001001C, ECMb, CPU0 JTAG log position
	UINT _JTAG_PRINT1_START;// fixed! 0xF0010020, ECMb, CPU1 JTAG log base
	UINT _JTAG_PRINT1_POINT;// fixed! 0xF0010024, ECMb, CPU1 JTAG log position
	BYTE _JTAG_COMMAND[256];// fixed! 0xF0010028, ECMb, J_shell Command position
#ifdef ECM_REMOTE
	BYTE _ECM_REMOTE_RX[12];// fixed! 0xF0010128, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_TX[12];// fixed! 0xF0010134, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_ISP[12];// fixed! 0xF0010140, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_Para[12];// fixed! 0xF001014c, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_Gamma[12];// fixed! 0xF0010158, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_Color[12];// fixed! 0xF0010164, ECMb, Isr_JUART Command position
	BYTE _ECM_REMOTE_Viewer[40];// fixed! 0xF0010170, ECMb, Isr_JUART Command position
	UINT _ECM_KEY;			// fixed! 0xF001018C, ECMb, Isr_JUART Command position
#else
	BYTE _ECM_REMOTE_DUMMY[116];
#endif
/////////////////////////////////////////////////////////////

	UINT MSG_DUMMY;			// ECM JTAG bug checker(buffer overflow)

	UINT CPU1_DATEID;		// compile datetime ID
	UINT CPU1_WDT_COUNT;	// WDT count

	// Video info
	UINT VIDEO_TICK;		// video total tick
	UINT VIDEO_FPS;			// max fps
	UINT VIDEO_OSD;			// font osd flag
	UINT H264_ENC_ENABLE;	// H.264 1ch(0x1), 2ch(0x2)

	// Ethernet info
	UINT ETH_ON;
	BYTE ETH_MAC[8];

	// Time info
	UINT TIME;
	INT TIMEZONE;
	UINT UPTIME;

	// Audio info
	UINT AUD_TX_DONE;
	UINT AUD_TX_MODE;
	UINT AUD_TX_GO;

	// RTSP info
	UINT RtspPort;
	UINT RtmpPort;
	UINT rtsp_conn_cnt;
	rtsp_connect_info rtsp_conn_info[RTSP_info_MAX];
	BYTE Rtsp_connect[2];
	BYTE Rtsp_idr[2];
	BYTE REC_ON;
} share_msg_t;

typedef struct {
	UINT ISPTblState;
	UINT ISPTblAddr;
	UINT ISP_UKEY;

	UINT ISP_ZOOM_UKEY;
	UINT ISP_FOCUS_VALUE;
	BYTE ISP_FOCUS_DIR; // [1:Focus Negative] [0:Focus Positive]

	BYTE KEY_DIRECTION;
	BYTE KEY_VALUE;
	BYTE ISP_MOTION;
	BYTE MENU_STYLE;
//	BYTE dummy;
} isp_msg_t;

typedef struct {
	UINT ETH_RX_CHECK;
	UINT ETH_TX_CHECK;
#if (PHY_LOOPBACK_TEST==1)
	UINT ETH_LOOPBACK_CHECK;
#endif

	UINT H264_POLL_LOG;
	UINT H264_TIME_SUM[2];
	UINT H264_TIME_MAX[2];
	UINT H264_TIME_CNT[2];
	UINT H264_VLCTIME_SUM[2];
	UINT H264_VLCTIME_MAX[2];
	UINT H264_VLCTIME_CNT[2];

	Debug_LogType dltFLAG;
	UINT CPU0_TICK;
	UINT CPU1_TICK;

#if ENX_NETWORK_Debug
	UINT ETH_packet_rx;
	UINT ETH_packet_tx;

	UINT RTSP_packet_tx[RTSP_info_MAX];
	UINT RTSP_packet_rx[RTSP_info_MAX];

	UINT RTCP_packet_tx[RTSP_info_MAX];
	UINT RTCP_packet_rx[RTSP_info_MAX];

	UINT RTSP_frame_vid_put[RTSP_info_MAX];
	UINT RTSP_frame_aud_put[RTSP_info_MAX];
	UINT RTSP_frame_vid_get[RTSP_info_MAX];
	UINT RTSP_frame_aud_get[RTSP_info_MAX];

	UINT RTP_packet_cnt_tx[RTSP_info_MAX];
	UINT RTP_packet_len_tx[RTSP_info_MAX]; 

//	UINT TxDB;
//	UINT RxDB;
//	UINT RTSP_Tx_Count;
//	UINT RTSP_Rx_Count;
//	UINT RTSP_Get_Count;
//	UINT RTSP_Put_Count;
#endif
} debug_msg_t;

#define WIFI_LOG_WSSB 0x80
#define WIFI_LOG_WSPB 0x40
#define WIFI_LOG_RSSB 0x20
#define WIFI_LOG_RSPB 0x10

#define WIFI_LOG_WPB 0x8
#define WIFI_LOG_WSB 0x4
#define WIFI_LOG_RPB 0x2
#define WIFI_LOG_RSB 0x1

// streaming tcp_pcb info message
#ifndef DEF_BOOT
#if (RTSPoverHTTP==1)
#include "lwip/tcp.h"

typedef enum {
	eTunnelType_GET = 1,
	eTunnelType_POST = 2,
}TunnelCmdType;

typedef struct {
  TunnelCmdType cmdtype; /*0=GET , 1 = POST */
  char uri[32];
  char cookie[32];
  struct tcp_pcb pcb;
} tunnel_info;	// old msg_tunnel_t

#define MSG_TUNNEL_NUM      10

typedef struct{                                 // msg_tunnel : Main -> Cop
	UINT head;                                  //    "
	UINT tail;                                  //    "
	UINT tot_num;                               //    "
	tunnel_info arg[MSG_TUNNEL_NUM];            //    "
} msgq_tunnel_t;                             //    "


typedef struct {
  UINT active;
  UINT posX;
  UINT posY;
  char info[32];
} txtosd_info;	// old msg_tunnel_t

typedef struct{                                 // msg_tunnel : Main -> Cop
	UINT head;                                  //    "
	UINT tail;                                  //    "
	UINT tot_num;                               //    "
	txtosd_info arg[MSG_TUNNEL_NUM];            //    "
} msgq_txtosd_t;               



typedef struct {
  UINT valid;  // 0 = empty, 1= vaild
  void *priv; // point to struct tcp_pcb
  WORD src;
  WORD dst;
} msgq_tunnel_port_t;

#endif
#endif

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
enum{
	MSG_CMD_SET_STREAM_PORT,		// 
	MSG_CMD_SET_STREAM_KICK,		// 

	MSG_CMD_SET_JPEG_SIZE,			// 
	MSG_CMD_SET_JPEG_QUALITY,		// 
	MSG_CMD_SET_JPEG_FPS,			// 
	MSG_CMD_SET_JPEG_BRMODE,		// 
	MSG_CMD_SET_JPEG_BRATE,			// 

	MSG_CMD_SET_H264_1CH_PROFILE,	//
	MSG_CMD_SET_H264_1CH_SIZE,		// 
	MSG_CMD_SET_H264_1CH_QUALITY,	// 
	MSG_CMD_SET_H264_1CH_FPS,		// 
	MSG_CMD_SET_H264_1CH_IDRFRAME,	// 
	MSG_CMD_SET_H264_1CH_BRMODE,	// 
	MSG_CMD_SET_H264_1CH_BRATE,		// 

	MSG_CMD_SET_H264_2CH_PROFILE,	//
	MSG_CMD_SET_H264_2CH_SIZE,		// 
	MSG_CMD_SET_H264_2CH_QUALITY,	// 
	MSG_CMD_SET_H264_2CH_FPS,		// 
	MSG_CMD_SET_H264_2CH_IDRFRAME,	// 
	MSG_CMD_SET_H264_2CH_BRMODE,	// 
	MSG_CMD_SET_H264_2CH_BRATE,		// 

	MSG_CMD_SET_NET_LINK,			// 
	MSG_CMD_SET_NET_DEFIF,			// 
	MSG_CMD_SET_NET_MAC,			// 
	MSG_CMD_SET_NET_IP,				// 
	MSG_CMD_SET_NET_GW,				// 
	MSG_CMD_SET_NET_SM,				// 
	MSG_CMD_SET_NET_DNS0,			// 
	MSG_CMD_SET_NET_DNS1,			// 

	MSG_CMD_SET_AUDIO,				// 
	MSG_CMD_SET_ETH_IRQ,			// 

	MSG_CMD_VIEW_IFCONFIG,
	MSG_CMD_VIEW_NETSTAT,
	MSG_CMD_VIEW_MEMSTAT,

	MSG_CMD_FTPD_OPEN_DATAPORT,
	MSG_CMD_FTPD_CLOSE_DATAPORT,
	MSG_CMD_FTPD_LIST,
	MSG_CMD_FTPD_RETR,
	MSG_CMD_FTPD_STOR,

	MSG_CMD_RTSPoverHTTP,
	MSG_CMD_TXTOCD,
	MSG_CMD_GET_PCB,
	MSG_CMD_POST_PCB,

// ISP CMD(set only)
	MSG_CMD_SET_ISP_SAVE,
	MSG_CMD_SET_ISP_RESET,
	MSG_CMD_SET_ISP_TITLE,
	MSG_CMD_SET_ISP_FONT,
	MSG_CMD_SET_ISP_IRIS,
	MSG_CMD_SET_ISP_STYLE,

// TEST CMD
	MSG_CMD_SET_H264_W,				// 
	MSG_CMD_SET_H264_H,				// 
	MSG_CMD_SW_VLOCK_IRQ,			// 
	MSG_CMD_SW_H264_IRQ,			// 
	MSG_CMD_SW_ETHERNET,
	MSG_CMD_SET_H264_INIT,			//
	MSG_CMD_SET_H264_READY,			//
	MSG_CMD_CHECK_JPEG, 
	MSG_CMD_VIEW_XSR,				//
	MSG_CMD_VIEW_H264_INFO,
	MSG_CMD_VIEW_JPEG_INFO,			//
	MSG_CMD_SET_USER_ID,
	MSG_CMD_SET_USER_PW,
	MSG_CMD_RTMP_CONNECT,
	MSG_CMD_RTMP_DISCONNECT,
#if (PHY_LOOPBACK_TEST==1)
	MSG_CMD_ETH_LOOPBACK,
#endif
#if (LVDS_MARGIN_TEST==1)
	MSG_CMD_SET_LVDS,				//
#endif
	MSG_CMD_SET_H264_1CH_PROFILE_DEC,
	MSG_CMD_SET_H264_2CH_PROFILE_DEC,
	MSG_CMD_SDSAVE_ON_OFF,
	MSG_CMD_SET_H264_1CH_IFRAME_SIZE,		// 
	MSG_CMD_SET_H264_2CH_IFRAME_SIZE,		// 
	MSG_CMD_SET_H264_1CH_PFRAME_SIZE,		// 
	MSG_CMD_SET_H264_2CH_PFRAME_SIZE,		// 
	MSG_CMD_SET_H264_1CH_OVERBIT_SIZE,
	MSG_CMD_SET_H264_2CH_OVERBIT_SIZE,
};

enum{
	UIN_OK				= 0x00000001,	// 1
	UIN_UR_TIMEZONE		= 0x00000002,	// 
	UIN_UR_DATETIME 	= 0x00000004,	//
#if (ENX_SNTPC_use==1)
	UIN_UR_SNTP_SW		= 0x00000008,	// 
#endif
	UIN_UR_SD_SAVE		= 0x00000100,	// 
	UIN_UR_FTP			= 0x00000200,	// 
};

#define MsgLock()		(CS_PASSMSG ? DEF_FAIL : DEF_OK)
#define MsgUnlock()		{CS_PASSMSG = 0;}

#define StmRecMsgLock()		(CS_STMRECMSG ? DEF_FAIL : DEF_OK)
#define StmRecMsgUnlock()	{CS_STMRECMSG = 0;}

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
#ifdef DEF_CPU0
extern void InitShareSram(void);
extern void InitMsg(volatile msgq_pass_t *p);
extern void InitMsgShell(volatile msgq_shell_t *p);
#endif
#ifdef DEF_CPU1
extern void InitMsgOS(void);
#endif
extern UINT MsgPut_nolock(volatile msgq_pass_t *p, UINT anCmd, UINT anArg);
extern UINT MsgPut(volatile msgq_pass_t *p, UINT anCmd, UINT anArg);
extern UINT MsgGet(volatile msgq_pass_t *p, UINT *anCmd, UINT *anArg);
#ifdef __DEVICE_SD__
#ifdef DEF_CPU0
extern UINT MsgRecPut(UINT addr, UINT size, UINT ts, UINT type);
#endif
#ifdef DEF_CPU1
extern UINT MsgRecGet(stream_info *getInfo);
#endif
#endif

// Wi-Fi
extern void wlif_init_event(void);
extern int wlif_get_event(wlif_priv_t *priv);
extern void cpu2cop_wlif_execmd(char *cmd);
extern void xfer_event_to_cpu0(WORD type, WORD evt, BYTE *mac, NETIFLIST iftype);

#ifdef DEF_CPU0
extern void xfer_pkt_from_cpu0(void);
extern int cop2cpu_wlif_getcmd(char *cmd, UINT maxlen);
#endif





#ifndef DEF_BOOT
#if (RTSPoverHTTP==1)
extern BYTE MsgTunnelPut(volatile msgq_tunnel_t *p, void *anArg);
extern BYTE MsgTunnelGet(volatile msgq_tunnel_t *p, void *anArg);
extern BYTE MsgTxtOsdPut(volatile msgq_txtosd_t *p, void *anArg);
extern BYTE MsgTxtOsdGet(volatile msgq_txtosd_t *p, void *anArg);
#endif
#endif

//-------------------------------------------------------------------------------------------------
// Variable
extern volatile msgq_net_rx_shared_t gptMsgNetworkRXShared;
extern volatile msgq_net_rx_t gptMsgNetworkRX0;
extern volatile msgq_net_rx_t gptMsgNetworkRX1;
#ifdef __WIFI__
extern volatile msgq_eth_t gptMsgEthCpu1to0;
#endif
extern volatile msgq_pass_t gptMsgCpu1to0;
extern volatile msgq_pass_t gptMsgCpu0to1;
#ifdef __DEVICE_SD__
extern volatile msgq_record_t gptMsgRecInfo;
#endif
#if (ENX_FTPD_use==1)
extern volatile cop2cpu_ftpdmb_t ftpd_mb;
#endif
extern volatile msgq_shell_t gptMsgShell;
extern volatile BYTE gbMnParTbl[512];

extern volatile share_msg_t gptMsgShare;
extern volatile isp_msg_t gptMsgISPTbl;
#if (NET_TX_BUFFER==1)
extern volatile msgq_net_tx_t gptMsgEthTx;
#endif
#ifndef DEF_BOOT
#if (RTSPoverHTTP==1)
extern volatile msgq_tunnel_t gptMsgTunnel;
extern volatile msgq_txtosd_t gptMsgTxtOsd;
extern volatile msgq_tunnel_port_t tunnel_port[MSG_TUNNEL_NUM];
#endif
#endif
#ifdef __WIFI__
extern volatile msgq_wlif_cmd_t gptMsgWlifCmd;
extern volatile msgq_wlif_evt_t gptMsgWlifEvt;
#endif
extern volatile BYTE gptMsgSH01[4096];
extern volatile BYTE gptMsgSH02[4096];
extern volatile BYTE gptMsgSH03[4096];
extern volatile BYTE gptMsgSH04[4096];
extern volatile debug_msg_t gptMsgDebug;


#endif // _MSG_H_
