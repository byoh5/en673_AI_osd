#include "board.h"
#include "dev.h"


#ifdef RTMP_SUPPORT
//#include "ngx_rtmp_config.h"
//#include "rtsp_control.h"
#include "ngx_rtmp_config.h"
#include "rtsp_control.h"

#ifndef _NGX_SESSION_H_INCLUDED_
#define _NGX_SESSION_H_INCLUDED_

#define RTMP_HANDSHAKE_STATE		1
#define RTMP_COMMAND_STATE          2
#define RTMP_PUSH_STATE          	3
#define RTMP_PUSH_LIVE				4


typedef struct
{
//	u32 nFtchNum[2];
//	u32 bPp;
//	u16 seq;	   //rtp sequence num
//	u32 bfirst_ts; //jpeg rtp transfer is first time.
//	u32 elapsed_ts;
//	u32 prev_ts;
//	u32 used;		//0 - can be used, 1, not used
//	int nread_idx;
//	char sendFormat;
//	u32 sendIdx;

    u8*  base;                                                        
	u32 bFirst;
	u32 nSp;
	u32 noff;
    u32 nsize;     // size of jpeg image
    u32 iframe;
    u32 ts;        //rtp time stamp 
    char sendFlag;
	char sendDone; 
	u32 ntotalsize;     // size of jpeg image
} h264_tx_info_t;

typedef struct 
{
	struct netif *nif;

    int session;
    int rtmp_port[3];            /*0: video, 1:audio */
    u32 seq;
    u32 remote_ipaddr;          /* rtsp client's ip addr */
    u32 port;                   /* our rtsp port*/
    u32 video_type;             /*2: h.264, 3: mjpeg, 4:h.264 sub*/
	u32 mediaformat;
    u32 ssrc;
	u32 play;
	
    send_rtmp_t send_video_rtmp;
    send_rtmp_t send_audio_rtmp;
    send_rtmp_t send_text_rtmp;
//    rtm_session_t prtm_s[3];

    u8 cmd;
    u8 cur_state;
    u8 rtp_type;                /* 0: udp, 1:tcp*/
	u8 login;					/* 0:fail, 1:ok*/
	uint32				signature;	/* "RTMP" */ /* <-- FIXME wtf */
	ngx_rtmp_session_t *ngx_s;	
} __attribute__ ((packed)) rtmp_session_t;

typedef struct rtmp_conn
{
	u32 valid;
	u16 end;
	u16 poll_cnt;                         //if poll_con become 10, called tcp_abort() to pcb is aborted 

//	u32 rtp_type;                       
	u32 sequence_num;

//	H264_BufferData buffer;

	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;               
	rtmp_session_t prtmp_ses;             //  rtsp session infox
	h264_tx_info_t h264_info;			  //  jpeg transfer info
//	rtm_manage prtm_manage;
	void (*h264_print_frameNum)(u32 pre_seq, char* addr);
#ifdef __H264_2CH__
	sbuf_t sbuf[2];
#else
	sbuf_t sbuf[1];
#endif
	sbuf_t sbuf_g711;	
//	sbuf_t sbuf_text;
//  u32 num_buf;
	u32 jbuf_seq ;                        //  the used seq of tcp_jpeg's buffer that is in [0,1,...,NUM_JPEG_BUF-1]
	//u8 send_buf[NUM_JPEG_BUF][1460];      //  is contained jpeg data and header infos for rtp over tcp 
} rtmp_con_t;

rtmp_con_t rtmp_list[MAX_RTMP_NUM];

struct tcp_pcb *rtmp_pcb_li;

u_char url_application[128];
u_char url_tcUrl[128];
u_char url_stream_name[128];


int rtmp_setPort(UINT port);
void rtmp_close(struct tcp_pcb *pcb);
int rtmp_init(int port);
void put_h264_streams_rtmp(UINT addr, UINT size, UINT flags, UINT ch, UINT ts);
extern void put_g7xx_streams_rtmp(UINT addr, UINT size);
err_t rtmp_connect(void);


#endif
#endif

