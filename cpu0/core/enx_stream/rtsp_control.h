/* Copyright (c) 2013 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#ifndef _RTSP_CONTROL_H_
#define _RTSP_CONTROL_H_

#include "dev.h"
#include "dev_types.h"
#include "lwipopts.h"
#include "stream_buffering.h"

/**   
	DEFINES
*/

#define NUM_JPEG_BUF NUM_TCP_SND_BUF

// CAUTION If you increse this value , it requres many memory. 
#ifndef MAX_RTSP_NUM
#define MAX_RTSP_NUM RTSP_info_MAX
#endif

// Set this to 1 to support RTSP request coming in in multiple packets/pbufs
#ifndef RTSPD_SUPPORT_REQUESTLIST
#define RTSPD_SUPPORT_REQUESTLIST 1
#endif

#if RTSPD_SUPPORT_REQUESTLIST
// Number of rx pbufs to enqueue to parse an incoming request (up to the first
// newline)
#ifndef RTSPD_REQ_QUEUELEN
#define RTSPD_REQ_QUEUELEN             5
#endif

// Defines the maximum length of a RTSP request line (up to the first CRLF,
// copied from pbuf into this a global buffer when pbuf- or packet-queues
// are received - otherwise the input pbuf is used directly)
#ifndef RTSPD_MAX_REQ_LENGTH
#define RTSPD_MAX_REQ_LENGTH           (2047*4)
#endif

// Minimum length for a valid RTSP/1.0 request: "$..." -> 4 bytes
#define RTSPD_MIN_REQ_LEN 4

// Number of (TCP payload-) bytes (in pbufs) to enqueue to parse and incoming
// request (up to the first double-newline)
#ifndef RTSPD_REQ_BUFSIZE
#define RTSPD_REQ_BUFSIZE              RTSPD_MAX_REQ_LENGTH
#endif
#endif // RTSPD_SUPPORT_REQUESTLIST

#ifndef RTSP_SUPPORT_AUTHORIZATION
#define RTSP_SUPPORT_AUTHORIZATION 0
#endif

#define RTSP_SUPPORT_RTCP_SR 1

/* jpeg : 24, h.264 22 */
#define PG_PAGE (TCP_MSS-24)//1024//(1460-22)

typedef struct {
	u32 start_rtptime;
	u32 ssrc;
	u32 start;
	u16 start_seq;

	u32 packet_cnt;
	u32 total_length;
} __attribute__ ((packed)) rtp_session_t;

typedef char (*send_rtp_t)(void *ctx);
typedef int (*send_rtcp_t)(void *ctx, int media_type);

enum {
	RTSPpq_NONE = 0,
	RTSPpq_VIDEO = 1,
	RTSPpq_AUDIO = 2,
	RTSPpq_METADATA = 4,
	RTSPpq_BACKCHANNEL = 8,
} RTSPqpList;

typedef struct {
	struct netif *nif;

	int session;

	u32 local_ip;
	u32 remote_ip;
	u16 local_port;
	u16 remote_port;

	// new struct를 생성해서 각 스트림단위마다 생성해야 함
	// rtp, rtcp port, rtcp_send_time, send_rtp_t들, send_rtcp_t들을 별도 구조체로
	// 구성할 필요성이 있다

	/* index 0:video, 1:audio, 2:metadata, 3:backchannel */
	int rtp_port[4];			/* rtp port list */
	int rtcp_port[4];			/* rtcp port list */

	u32 seq;
	u32 remote_ipaddr;          /* rtsp client's ip addr */
	u32 port;                   /* our rtsp port */
	u32 video_type;             /* 0:H264_1, 1:H264_2, 9:mjpeg */
	u32 setup_backch;			/* backchannel setup */
	u32 play_query;				/* 1:video, 2:audio, 4:metadata, 8:backchannel */
	u32 mediaformat;
	u32 play;

	char rtsp_uri[512];

	u32 rtp_session_timer_id;
	u32 rtcp_send_time[4];
	send_rtp_t send_video_rtp;
	send_rtp_t send_audio_rtp;
	u32 rtcp_video_timerid;
	u32 rtcp_audio_timerid;
	send_rtcp_t send_video_rtcp;
	send_rtcp_t send_audio_rtcp;
	rtp_session_t prtp_s[4];
	char out_buffer[1460];
	u32 out_size;

	u8 cmd;
	u8 cur_state;
	u8 rtp_type;                /* 0:udp, 1:tcp */
	u8 login;					/* 0:fail, 1:ok */
} __attribute__ ((packed)) rtsp_session_t;

typedef struct {
	u32 un32FU_NalUnitType;
	u32 un32FU_NalRefIdc;

	u32 left;
	u8* data;

	u8* base;
	u32 bFirst;
	u32 nOffset;
	u32 noff;
	u32 btx_st;    // jpeg transter state
	u32 nsize;     // size of jpeg image
	u32 iframe;
	u32 ts;        //rtp time stamp
	u32 sendFlag;
	u32 sendDone;
} rtp_tx_info_t;

typedef struct {
	u32 valid;
	u32 mtype;
} rtcp_rx_info_t;

#if (RTSPoverHTTP==1)
enum {
	PCB_RTSP_TYPE,
	PCB_HTTP_FRONT,
};

typedef struct {
	u32 type;                             // 0:rtsp, 1:http front
} pcb_type_t;
#endif

#ifdef __AUDIO__
#define RTP_UDP_AUDIO_PCB (2)
#else
#define RTP_UDP_AUDIO_PCB (0)
#endif
#if (ONVIF_METADATA==1)
#define RTP_UDP_META_PCB (2)
#else
#define RTP_UDP_META_PCB (0)
#endif
#if (ONVIF_BACKCHANNEL==1)
#define RTP_UDP_BACKCH_PCB (2)
#else
#define RTP_UDP_BACKCH_PCB (0)
#endif
#define RTP_UDP_PCB (2+(RTP_UDP_AUDIO_PCB+RTP_UDP_META_PCB+RTP_UDP_BACKCH_PCB))

/**
    @rtp_type : is determined by SETUP command of RTSP
                0: udp, 1:tcp
    @upcb     : rtp server udp pcb       
*/
typedef struct rtsp_conn {
#if (RTSPoverHTTP==1)
	pcb_type_t pcb_type;
	void *ctx;                            // point to http front for http tunneling
#endif

	u32 valid;
	u16 end;
	u16 poll_cnt;                         // if poll_con become 10, called tcp_abort() to pcb is aborted 
	u32 close_wait;

#if RTSPD_SUPPORT_REQUESTLIST
	struct pbuf *req;

	// RTSP request is copied here from pbufs for simple parsing
	char req_buf[RTSPD_MAX_REQ_LENGTH];
	u32 req_left;
	u32 req_offset;
	u32 req_more;
#endif // RTSPD_SUPPORT_REQUESTLIST

//	u32 rtp_type;
	u32 sequence_num;

// Packet Test(BUILD_TEST==1)
//	u32 build_num;
//	u32 nal_count;
//	u32 nal_dump[30];

	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb[RTP_UDP_PCB];
	rtsp_session_t prtsp_ses;				// rtsp session info
	rtp_tx_info_t prtp_info;				// rtp transfer info
	rtcp_rx_info_t prtcp_info;				// rtcp rx info

	void (*h264_print_frameNum)(u32* pre_seq, u8* addr);
	sbuf_t sbuf_video;
#ifdef __AUDIO__
	sbuf_t sbuf_audio;
#endif
	u32 jbuf_seq;                        //  the used seq of tcp_jpeg's buffer that is in [0,1,...,NUM_JPEG_BUF-1]
	u8 send_buf[NUM_JPEG_BUF][1460];      //  is contained jpeg data and header infos for rtp over tcp 
} rtsp_con_t;

extern volatile char *gUserId;
extern volatile char *gUserPw;

#endif
