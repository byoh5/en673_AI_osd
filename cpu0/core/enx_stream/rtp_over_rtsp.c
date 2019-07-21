/* Copyright (c) 2013-16 Eyenix Corporation. All rights reserved.
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

#include "dev.h"
#include "dev_inline.h"

#include "msg.h"

#include "main.h"
#include "enx_video.h"
#include "enx_os.h"

#include "lwip/memp.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/tcp_impl.h"

#include "rtp_hdr_gen.h"
#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_over_rtsp.h"
#include "rtp_h264.h"
#include "rtp_audio.h"
#include "rtcp_recv.h"
#include "jpeg.h"

/* info struct for rtsp connection */
rtsp_con_t rtsp_list[MAX_RTSP_NUM];	// (MAX_RTSP_NUM * MAX(57540))byte

static u32 nrtsp_user = 0;

static struct tcp_pcb *rtsp_pcb_li;

/**
    register rtp send function to rtsp for video.
    @ media : VIDEO, AUDIO
*/
void rtp_register_streamingfn(u32 media, rtsp_con_t *prtsp_conn)
{
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
#ifdef __ISP__
	if (media == VIDEO) {
		prtsp_ses->send_video_rtp = NULL;
		prtsp_ses->send_video_rtcp = NULL;
		switch(prtsp_ses->rtp_type) {
			case RTP_UDP:
				switch(prtsp_ses->video_type) {
#ifdef __H264__
					case RTSP_H264:
#ifdef __H264_2CH__
					case RTSP_H264SUB:
#endif
						prtsp_ses->send_video_rtp = send_RTPoverUDP_Packet;
						break;
#endif
#ifdef __JPEG__
					case RTSP_MJPEG:
						prtsp_ses->send_video_rtp = rtp_tx_jpeg;
						break;
#endif
				}
				break;

			case RTP_TCP:
			case RTP_HTTP:
				switch (prtsp_ses->video_type) {
#ifdef __H264__
					case RTSP_H264:
#ifdef __H264_2CH__
					case RTSP_H264SUB:
#endif
						prtsp_ses->send_video_rtp = send_RTPoverTCP_Packet;
						break;
#endif
#ifdef __JPEG__
					case RTSP_MJPEG:
						prtsp_ses->send_video_rtp = rtp_tx_jpeg_over_tcp;
						break;
#endif
				}
				break;
		}
#if (RTSP_SUPPORT_RTCP_SR==1)
		prtsp_ses->send_video_rtcp = send_rtcp_sr;
#endif
	}
#endif    //end of __ISP__

#ifdef __AUDIO__
	if(media == AUDIO) {
		prtsp_ses->send_audio_rtp = NULL;
		prtsp_ses->send_audio_rtcp = NULL;
		switch(prtsp_ses->rtp_type) {
			case RTP_UDP:
				prtsp_ses->send_audio_rtp = rtp_tx_g711;
				break;

			case RTP_TCP:
			case RTP_HTTP:
				prtsp_ses->send_audio_rtp = rtp_tx_g711_over_tcp;
				break;
		}
#if (RTSP_SUPPORT_RTCP_SR==1)
		prtsp_ses->send_audio_rtcp = send_rtcp_sr;
#endif
	}
#endif
}

#define DEF_RTSP_POLL 1
#if DEF_RTSP_POLL
#define RTP_TIME_OUT 60	// 60sec
static void rtsp_close(struct tcp_pcb *pcb);

static err_t rtsp_poll(void *arg, struct tcp_pcb *pcb)
{
	rtsp_con_t *prtsp_conn = arg;
	if (prtsp_conn != NULL) {
		rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
		if (prtsp_ses != NULL) {
			if (prtsp_ses->out_size != 0) {
				err_t err;
				flprintf("tcp_write, %dbyte\r\n", prtsp_ses->out_size);
				err = tcp_write(pcb, (const void*)prtsp_ses->out_buffer, prtsp_ses->out_size, TCP_WRITE_FLAG_COPY);
				if (err != ERR_OK) {
					flprintf("%s tcp write fail(err:%s)\r\n", sltLocalTime.data, lwip_strerr(err));
					hexDump("TX_Packet", prtsp_ses->out_buffer, prtsp_ses->out_size);
				} else {
#if ENX_NETWORK_Debug
					gptMsgDebug.RTSP_packet_tx[prtsp_conn->valid-1]++;
#endif
					prtsp_ses->out_size = 0;
				}
			}
		}

		if(prtsp_conn->poll_cnt++ == RTP_TIME_OUT) {
			printf("RTSP_TIMEOUT\r\n");
			rtsp_close(pcb);
			tcp_abort(pcb);
			return ERR_ABRT;
		}
	}
	return ERR_OK;
}

err_t rtsp_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	rtsp_con_t *prtsp_conn = arg;
	prtsp_conn->poll_cnt = 0;
	return ERR_OK;

	UNUSED(tpcb);
	UNUSED(len);
}
#endif

static void h264_print_frame_num(u32 *detectErr, u8* addr)
{
	BYTE frameNum = 0;
	BYTE frameNal = *(addr + 4);
	if (frameNal == 0x61) {			// P frame
		frameNum = (*(((int *)addr) + 1) >> 13) & 0xFF;
		// H.264 Frame Number Info
		// *(((int *)addr)+1) (0000 0000 0001 1111 1110 0000 0000 0000)
		if (frameNum - *detectErr == 1) {
			//ex)E0 A0 = (8*0) + ((1010 0000)>>5) = Frame 5,
			//   E1 20 = (8*1) + ((0010 0000)>>5) = Frame 9
			*detectErr = frameNum;
		} else {
			printf("addr(0x%08X), newNum(%02d), oldNum(%02d)\r\n    Frame Number Detect Error!!\r\n", addr, frameNum, *detectErr);
			*detectErr = 0xFFFFFFFF;
		}
	} else if (frameNal == 0x67) {	// SPS+PPS+I frame
		*detectErr = 0;
	}
}

/**
    add rtsp user
*/
rtsp_con_t* rtsp_add_user(struct tcp_pcb *pcb)
{
	rtsp_session_t *prtsp_ses;
	rtp_session_t *prtp_ses;
	rtp_tx_info_t *prtp_info;
	int i = 0, j;

	if (nrtsp_user < MAX_RTSP_NUM) {
		/* search rtsp user */
		for (i = 0; i < MAX_RTSP_NUM; i++) {
			if (rtsp_list[i].valid == 0) {
				break;
			}
		}

		enx_os_memset((char*)&rtsp_list[i], 0, sizeof(rtsp_con_t));

		rtsp_list[i].valid = i + 1;		/* rtsp_list[i] is marked in used */
		rtsp_list[i].tpcb = pcb;		/* accepted pcb that is used for video and audio transfer */
		rtsp_list[i].jbuf_seq  = 0;		/* send buff index init */
		rtsp_list[i].poll_cnt  = 0;		/* polling cnt */

		sbuf_init(&rtsp_list[i].sbuf_video, NUM_H264_SBUF);
#ifdef __AUDIO__
		sbuf_init(&rtsp_list[i].sbuf_audio, NUM_G711_SBUF);
#endif

		// init rtsp info
		prtsp_ses = (rtsp_session_t *)&(rtsp_list[i].prtsp_ses);
		prtsp_ses->session = 0;
		prtsp_ses->remote_ipaddr = pcb->remote_ip.addr;
		prtsp_ses->port = pcb->local_port;
		prtsp_ses->video_type = 0xFFFFFFFF;
		prtsp_ses->play_query = RTSPpq_NONE;
		prtsp_ses->mediaformat = 96;
		prtsp_ses->login = 0;
		for (j = 0; j < 4; j++) {
			prtsp_ses->rtp_port[i] = -1;
			prtsp_ses->rtcp_port[i] = -1;
		}

		prtsp_ses->local_ip = pcb->local_ip.addr;
		prtsp_ses->local_port = pcb->local_port;
		prtsp_ses->remote_ip = pcb->remote_ip.addr;
		prtsp_ses->remote_port = pcb->remote_port;

		prtp_ses = (rtp_session_t *)&prtsp_ses->prtp_s[0];
		prtp_ses->start = 0;

		// video tx info is init
		prtp_info = &(rtsp_list[i].prtp_info);
		prtp_info->btx_st = 0;
		prtp_info->ts = 0;

		rtsp_list[i].sequence_num = 0xFFFFFFFF;
		rtsp_list[i].h264_print_frameNum = h264_print_frame_num;

		printf("USER Conn(%d=>%d) : Local(%IP:%u) Remote(%IP:%u)\r\n", nrtsp_user, nrtsp_user+1,
				pcb->local_ip.addr, pcb->local_port, pcb->remote_ip.addr, pcb->remote_port);

		nrtsp_user++;

		gptMsgShare.rtsp_conn_cnt = nrtsp_user;
		gptMsgShare.rtsp_conn_info[i].server_addr = pcb->local_ip.addr;
		gptMsgShare.rtsp_conn_info[i].server_port = pcb->local_port;
		gptMsgShare.rtsp_conn_info[i].client_port = pcb->remote_port;
		gptMsgShare.rtsp_conn_info[i].client_addr = pcb->remote_ip.addr;
		gptMsgShare.rtsp_conn_info[i].video_type = 0xFFFF;
		gptMsgShare.rtsp_conn_info[i].rtp_type = 0xFFFF;
		gptMsgShare.rtsp_conn_info[i].connect_time = gptMsgShare.TIME;

		return &rtsp_list[i];
	}

	printf("USER Conn Fail\r\n");
	return (rtsp_con_t*)NULL;
}

/**
    remove rtsp user
*/
void rtsp_del_user_wait(void *ctx)
{
	rtsp_con_t *prtsp_conn = (rtsp_con_t *)ctx;
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);

	if (prtsp_conn) {
		if (prtsp_conn->valid) {
			rtsp_close_rtp(prtsp_conn);
			rtsp_close_rtcp(prtsp_conn);

			int idx = prtsp_conn->valid - 1;
			prtsp_conn->valid = 0;
			if (nrtsp_user > 0) {
				nrtsp_user--;
			}

			gptMsgShare.rtsp_conn_cnt = nrtsp_user;
			gptMsgShare.rtsp_conn_info[idx].server_addr = 0;
			gptMsgShare.rtsp_conn_info[idx].server_port = 0;
			gptMsgShare.rtsp_conn_info[idx].client_port = 0;
			gptMsgShare.rtsp_conn_info[idx].client_addr = 0;
			gptMsgShare.rtsp_conn_info[idx].video_type = 0xFFFF;
			gptMsgShare.rtsp_conn_info[idx].rtp_type = 0xFFFF;
			gptMsgShare.rtsp_conn_info[idx].connect_time = 0xFFFFFFFF;
		}
	}

	printf("USER Disconn(%d=>%d) : Local(%IP:%u) Remote(%IP:%u)\r\n", nrtsp_user+1, nrtsp_user,
			prtsp_ses->local_ip,  prtsp_ses->local_port, prtsp_ses->remote_ip, prtsp_ses->remote_port);
}

void rtsp_del_user(struct tcp_pcb *pcb)
{
	rtsp_con_t *prtsp_conn = pcb->callback_arg;
	if(prtsp_conn) {
		if (prtsp_conn->close_wait == 0) {
			rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
			prtsp_conn->close_wait = 1;
			prtsp_ses->send_video_rtp = NULL;
			prtsp_ses->send_video_rtcp = NULL;
			prtsp_ses->send_audio_rtp = NULL;
			prtsp_ses->send_audio_rtcp = NULL;
			//timer_sched_timeout_cb(100, TIMEOUT_ONESHOT, rtsp_del_user_wait, prtsp_conn);
			rtsp_del_user_wait(prtsp_conn);
		}
	}
}

/**
    close rtsp sesstion
*/
static void rtsp_close(struct tcp_pcb *pcb)
{
	if(pcb == NULL) {
		flprintf("\r\n");
		return;
	}
	rtsp_del_user(pcb);
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_poll(pcb, NULL, 2);
	tcp_err(pcb, NULL);
	tcp_close(pcb);
	pcb = NULL;
}

static void rtsp_err(void *arg, err_t err)
{
	rtsp_con_t *prtsp_conn = (rtsp_con_t *)arg;

	if (arg != NULL) {
		switch (err) {
			case ERR_ABRT:
			case ERR_RST:
			case ERR_CLSD:
			case ERR_CONN:
				rtsp_close(prtsp_conn->tpcb);
				break;
		}
	}
	printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
}

/**
  be called when tcp data is received.
*/
static err_t rtsp_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	err_t parsed = ERR_ABRT;
	rtsp_con_t *prtsp_conn;
	rtsp_session_t *prtsp_ses;
	int teardown = 0;

	prtsp_conn = arg;
	if ((err != ERR_OK) || (p == NULL) || (prtsp_conn == NULL)) {
		// error or closed by other side?
		if (p != NULL) {
			// Inform TCP that we have taken the data.
			tcp_recved(pcb, p->tot_len);
			pbuf_free(p);
		}
		if (prtsp_conn == NULL) {
			printf("Error, rtsp_recv: prtsp_conn is NULL\r\n");
		}
		if ((p == NULL) || (err != ERR_OK)) {
			printf("Error, rtsp_recv: disconnect, close(err:%d)\r\n", err);
		}
		rtsp_close(pcb);
	} else { //tcp data is received ...
#if 0//ENX_DEBUG
		printf("================================================\r\n");
		int i;
		char *tktk = p->payload;
		for (i = 0; i < p->tot_len; i++) {
			printf("%c", tktk[i]);
		}
		printf("\r\n================================================\r\n");
#endif
		prtsp_conn->poll_cnt = 0; //reset poll_cnt
		prtsp_ses = &(prtsp_conn->prtsp_ses);
		if(prtsp_ses->video_type == 0xFFFFFFFF) {
			char *rcvBuf = p->payload;
			if (rcvBuf[0] == '$') {
				// Why?												// 추가검증필요
			} else {
				int nUrlLen;
				char *rcvPath, *rcvUriS, *rcvUriE;
				prtsp_ses->cur_state = STATE_INIT;

				rcvUriS = strnstr(rcvBuf, " ", p->tot_len) + 1;
				nUrlLen = p->tot_len - ((rcvUriS + 1) - rcvBuf);
				rcvUriE = strnstr(rcvUriS + 1, " ", nUrlLen);
				nUrlLen = rcvUriE - rcvUriS;

				char rcvURL[1024] = {0};
				strncpy(rcvURL, rcvUriS, nUrlLen);
#ifdef __H264__
				if ((rcvPath = strnstr(rcvURL, "/"H264URL, nUrlLen)) != NULL) {
					nUrlLen = strlen(rcvPath) - 1;
					if (strncmp(rcvPath+1, H264URL, nUrlLen) == 0) {
	//					printf("Stream Type:H264\r\n");
						prtsp_ses->video_type = RTSP_H264;
					}
				}
#ifdef __H264_2CH__
				else if ((rcvPath = strnstr(rcvURL, "/"HSUBURL, nUrlLen)) != NULL) {
					nUrlLen = strlen(rcvPath) - 1;
					if (strncmp(rcvPath+1, HSUBURL, nUrlLen) == 0) {
	//					printf("Stream Type:H264-sub\r\n");
						prtsp_ses->video_type = RTSP_H264SUB;
					}
				}
#endif
#ifdef __JPEG__
				else
#endif
#endif
#ifdef __JPEG__
				if((rcvPath = strnstr(rcvURL, "/"JPEGURL, nUrlLen)) != NULL) {
					nUrlLen = strlen(rcvPath) - 1;
					if(strncmp(rcvPath+1, JPEGURL, nUrlLen) == 0) {
	//					printf("Stream Type:MJPEG\r\n");
						prtsp_ses->video_type = RTSP_MJPEG;
					}
				}
#endif
#if !(defined(__H264__) || defined(__JPEG__))
				UNUSED(rcvPath);
#endif

				strlcpy(prtsp_ses->rtsp_uri, rcvURL, 1024);

				gptMsgShare.rtsp_conn_info[prtsp_conn->valid-1].video_type = prtsp_ses->video_type;
			}

			// how netif?
			prtsp_ses->nif = NULL;
			struct netif *nif = netif_list;
			for (nif = netif_list; nif != NULL; nif = nif->next) {
				if (nif->ip_addr.addr == pcb->local_ip.addr) {
					prtsp_ses->nif = nif;
					break;
				}
			}
		}

		/* Inform TCP that we have taken the data. */
		tcp_recved(pcb, p->tot_len);

#if ENX_NETWORK_Debug
		gptMsgDebug.RTSP_packet_rx[prtsp_conn->valid-1]++;
#endif

		// Processing
		prtsp_ses->out_size = 0;
		parsed = rtsp_session_proc(prtsp_conn, &p, &teardown);

#if RTSPD_SUPPORT_REQUESTLIST
		if (parsed != ERR_INPROGRESS) {
			// request fully parsed or error
			if (prtsp_conn->req != NULL) {
				pbuf_free(prtsp_conn->req);
				prtsp_conn->req = NULL;
			}
		}
#endif // RTSPD_SUPPORT_REQUESTLIST
		if (p != NULL) {
			pbuf_free(p);
		}

		pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
		pcb->flags |= TF_ACK_NOW; //diable delayed ack function

		if (parsed == ERR_OK) {
			if (prtsp_ses->out_size > 0) {
				err = tcp_write(pcb, (const void*)prtsp_ses->out_buffer, prtsp_ses->out_size, TCP_WRITE_FLAG_COPY);
				if (err != ERR_OK) {
					flprintf("%s tcp write fail(err:%s)\r\n", sltLocalTime.data, lwip_strerr(err));
					hexDump("TX_Packet", prtsp_ses->out_buffer, prtsp_ses->out_size);
				} else {
#if ENX_NETWORK_Debug
					gptMsgDebug.RTSP_packet_tx[prtsp_conn->valid-1]++;
#endif
					prtsp_ses->out_size = 0;
				}
			}
			if (teardown == 2) {
				printf("call : rtsp_recv : Close(bye msg)\r\n");
				rtsp_close(pcb);
			}
		} else if (parsed == ERR_ARG) {
			printf("%s call : rtsp_recv : ERR_ARG, Close\r\n", sltLocalTime.data);
			rtsp_close(pcb);
		}
	}
	return ERR_OK;
}

/**
    we can determine h.264 or mjpeg depending on local port of tcp.
*/
static err_t rtsp_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	rtsp_con_t *prtsp;

	tcp_setprio(pcb, TCP_PRIO_MIN);
	prtsp = rtsp_add_user(pcb);

	if (prtsp == NULL) {
		rtsp_disconnect(MAX_RTSP_NUM);
		return ERR_MEM;
	}

	/* Tell TCP that this is the structure we wish to be passed for our callbacks. */
	tcp_arg(pcb, prtsp);

	/* Tell TCP that we wish to be informed of incoming data by a call
	to the rtsp_recv() function. */
	tcp_recv(pcb, rtsp_recv);
	/* Specifies the polling interval and the callback function that should be
	called to poll the application. The interval is specified in number of
	TCP coarse grained timer shots, which typically occurs twice a second */
#if DEF_RTSP_POLL
	tcp_poll(pcb, rtsp_poll, 2); // after 0.5 sec , call rtsp_pll()
	tcp_sent(pcb, rtsp_sent);
#endif
	tcp_err(pcb, rtsp_err);

	prtsp->prtp_info.sendDone = 1;

	return ERR_OK;

	UNUSED(arg);
	UNUSED(err);
}

void rtps_setPort(UINT port)
{
	gptMsgShare.RtspPort = port;
	if (gptMsgShare.RtspPort == 0) {
		gptMsgShare.RtspPort = RTSP_PORT;
	}
}

void rtsp_disconnect(UINT ntype)
{
	int i;

	if (ntype < MAX_RTSP_NUM) {
		if (rtsp_list[ntype].valid) {			// 활성화 된 index의 접속자 연결종료
			if (rtsp_list[ntype].close_wait == 0) {
				printf("rtsp_close user(%d)\r\n", ntype);
				rtsp_close(rtsp_list[ntype].tpcb);
			}
		}
	} else if (ntype == MAX_RTSP_NUM) {			// 활성화 된 가장 첫번째 접속자 연결종료
		for (i = 0; i < MAX_RTSP_NUM; i++) {
			if (rtsp_list[i].valid) {
				if (rtsp_list[i].close_wait == 0) {
					printf("rtsp_close 1st user(%d)\r\n", i);
					rtsp_close(rtsp_list[i].tpcb);
				}
				break;
			}
		}
	} else if (ntype == 0xFF) {
		for (i = 0; i < MAX_RTSP_NUM; i++) {
			if (rtsp_list[i].valid) {
				if (rtsp_list[i].close_wait == 0) {
					printf("rtsp_close all(%d)\r\n", i);	// 모든 접속자 연결종료
					rtsp_close(rtsp_list[i].tpcb);
				}
			}
		}
	}
}

int rtsp_port_change(UINT port)
{
	int res = DEF_FAIL;
	UINT old_rtspport = gptMsgShare.RtspPort;
	if (port != 0) {
		rtps_setPort(port);
	}

	struct tcp_pcb *old_rtsp_pcb_li = rtsp_pcb_li;
	struct tcp_pcb *loc_rtsp_pcb = tcp_new();			// allocate tcp pcb struct
	err_t ert = tcp_bind(loc_rtsp_pcb, IP_ADDR_ANY, gptMsgShare.RtspPort);	// rtsp port is 554(default)
	if (ert != ERR_OK) {
		printf("%s: tcp bind fail(%d)\r\n", __func__, ert);
		gptMsgShare.RtspPort = old_rtspport;
		res = DEF_FAIL;
	} else {
		rtsp_pcb_li = tcp_listen(loc_rtsp_pcb);			// listen tcp pcb is registerd
		if (rtsp_pcb_li == NULL) {
			tcp_close(loc_rtsp_pcb);					// close error tcp pcb
			printf("%s: listen fail(NULL)\r\n", __func__);
			gptMsgShare.RtspPort = old_rtspport;
			rtsp_pcb_li = old_rtsp_pcb_li;
			res = DEF_FAIL;
		} else {
			tcp_accept(rtsp_pcb_li, rtsp_accept);		// wait a new connection
			printf("Set RTSPd port to %u.\r\n", gptMsgShare.RtspPort);

			if (old_rtsp_pcb_li != NULL) {
				ert = tcp_close(old_rtsp_pcb_li);			// close old tcp pcb
				if (ert != ERR_OK) {
					printf("%s: listen close fail(%d)\r\n", __func__, ert);
				}
			}

			res = DEF_OK;
		}
	}
	return res;
}

/**
    Open port of rtsp.
    default port : 554
*/
void rtsp_init(void)
{
	int i = 0;

	rtps_setPort(RTSP_PORT);
	gptMsgShare.Rtsp_connect[0] = 0;
	gptMsgShare.Rtsp_connect[1] = 0;
	gptMsgShare.Rtsp_idr[0] = 0;
	gptMsgShare.Rtsp_idr[1] = 0;

	struct tcp_pcb *loc_rtsp_pcb = tcp_new();		// allocate tcp pcb struct
	tcp_bind(loc_rtsp_pcb, IP_ADDR_ANY, gptMsgShare.RtspPort);	// rtsp port is 554
	rtsp_pcb_li = tcp_listen(loc_rtsp_pcb);			// listen tcp pcb is registerd
	tcp_accept(rtsp_pcb_li, rtsp_accept);			// wait a new connection

	for (i = 0; i < MAX_RTSP_NUM; i++) {
		rtsp_list[i].valid = 0;
		gptMsgShare.rtsp_conn_info[i].server_addr = 0;
		gptMsgShare.rtsp_conn_info[i].server_port = 0;
		gptMsgShare.rtsp_conn_info[i].client_port = 0;
		gptMsgShare.rtsp_conn_info[i].client_addr = 0;
		gptMsgShare.rtsp_conn_info[i].video_type = 0xFFFF;
		gptMsgShare.rtsp_conn_info[i].rtp_type = 0xFFFF;
		gptMsgShare.rtsp_conn_info[i].connect_time = 0xFFFFFFFF;
	}
	gptMsgShare.rtsp_conn_cnt = 0;
}

/**
    transmit video stream over tcp/ip network.
*/
ISRT void rtp_media_streamming(void)
{
	int i;
	rtsp_con_t *prtsp_con;
	rtsp_session_t *prtsp_ses;
#if (RTSP_SUPPORT_RTCP_SR==1)
	UINT rtcp_current_time;
#endif
//	struct tcp_pcb *pcb = tcp_active_pcbs;

	for (i = 0; i < MAX_RTSP_NUM; i++) {
		prtsp_con = &rtsp_list[i];
		prtsp_ses = &(rtsp_list[i].prtsp_ses);
		if (prtsp_con->valid && prtsp_ses->cur_state == STATE_PLAY) {
#ifdef __AUDIO__
			if (cQueue_isempty(&(prtsp_con->sbuf_audio)) == DEF_FAIL) {
				if (prtsp_ses->send_audio_rtp) {
					if (prtsp_ses->send_audio_rtp(prtsp_con) == ERR_OK) {
#if (RTSP_SUPPORT_RTCP_SR==1)
						if(prtsp_ses->send_audio_rtcp) {
							if (prtsp_ses->rtcp_send_time[AUDIO] != 0) {
								rtcp_current_time = gptMsgShare.TIME;
								if ((int)(rtcp_current_time - prtsp_ses->rtcp_send_time[AUDIO]) >= RTCP_SR_INTERVAL) {	// 5sec 마다 RTCP SR 전송
									if (prtsp_ses->send_audio_rtcp(prtsp_con, AUDIO) == DEF_OK) {
										prtsp_ses->rtcp_send_time[AUDIO] = rtcp_current_time;
									}
								}
							}
						}
#endif
					}
				}
			}
#endif

			if (cQueue_isempty(&(prtsp_con->sbuf_video)) == DEF_FAIL) {
#ifdef __H264__
				if (prtsp_con->prtp_info.sendDone == 1) {
					if (prtsp_ses->video_type == RTSP_H264) {
						prtsp_con->prtp_info.sendDone = 0;
						prtsp_con->prtp_info.sendFlag = 1;
					}
#ifdef __H264_2CH__
					if (prtsp_ses->video_type == RTSP_H264SUB) {
						prtsp_con->prtp_info.sendDone = 0;
						prtsp_con->prtp_info.sendFlag = 1;
					}
#endif
				}
#endif
#ifdef __JPEG__
				if (prtsp_ses->video_type == RTSP_MJPEG) {
					prtsp_con->prtp_info.sendFlag = 1;
				}
#endif
			}

			if (prtsp_ses->send_video_rtp) {
				if (prtsp_ses->send_video_rtp(prtsp_con) == ERR_OK) {
#if (RTSP_SUPPORT_RTCP_SR==1)
					if(prtsp_ses->send_video_rtcp) {
						if (prtsp_ses->rtcp_send_time[VIDEO] != 0) {
							rtcp_current_time = gptMsgShare.TIME;
							if ((int)(rtcp_current_time - prtsp_ses->rtcp_send_time[VIDEO]) >= RTCP_SR_INTERVAL) { // RTCP_SR_INTERVAL초 마다 RTCP SR 전송
								if (prtsp_ses->send_video_rtcp(prtsp_con, VIDEO) == DEF_OK) {
									prtsp_ses->rtcp_send_time[VIDEO] = rtcp_current_time;
								}
							}
						}
					}
#endif
				}
			}
		}
	}
#if 0
	/* check rtsp port connected to client */
	while (pcb != NULL) {
		prtsp_con = pcb->callback_arg;
		if (prtsp_con == NULL) {
			goto next;
		}

		prtsp_ses = &(prtsp_con->prtsp_ses);
		if (prtsp_ses->cur_state == STATE_PLAY) {
#ifdef __AUDIO__
			if (cQueue_isempty(&(prtsp_con->sbuf_audio)) == DEF_FAIL) {
				if (prtsp_ses->send_audio_rtp) {
					prtsp_ses->send_audio_rtp(prtsp_con);
				}
			}
#endif
			if (cQueue_isempty(&(prtsp_con->sbuf_video)) == DEF_FAIL) {
#ifdef __H264__
				if (prtsp_con->prtp_info.sendDone == 1) {
					if (prtsp_ses->video_type == RTSP_H264) {
						prtsp_con->prtp_info.sendDone = 0;
						prtsp_con->prtp_info.sendFlag = 1;
					}
#ifdef __H264_2CH__
					if (prtsp_ses->video_type == RTSP_H264SUB) {
						prtsp_con->prtp_info.sendDone = 0;
						prtsp_con->prtp_info.sendFlag = 1;
					}
#endif
				}
#endif
#ifdef __JPEG__
				if (prtsp_ses->video_type == RTSP_MJPEG) {
					prtsp_con->prtp_info.sendFlag = 1;
				}
#endif
			}
			if (prtsp_ses->send_video_rtp) {
				prtsp_ses->send_video_rtp(prtsp_con);
			}
		}
next:
		pcb = pcb->next;
	}
#endif
}

void put_video_streams(UINT addr, UINT size, UINT flags, UINT video_type, UINT ts)
{
	int i;
	for (i = 0; i < MAX_RTSP_NUM; i++) {		// search rtsp user
		if (rtsp_list[i].valid && rtsp_list[i].prtsp_ses.cur_state == STATE_PLAY && (rtsp_list[i].prtsp_ses.play_query & RTSPpq_VIDEO)) {
			if (rtsp_list[i].prtsp_ses.video_type == video_type) {
				sbuf_put(&(rtsp_list[i].sbuf_video), addr, size, flags, ts);
#if ENX_NETWORK_Debug
				gptMsgDebug.RTSP_frame_vid_put[i]++;
#endif
			}
		}
	}
}

#ifdef __AUDIO__
void put_audio_streams(UINT addr, UINT size, UINT audio_type)
{
	int i;
	for (i = 0; i < MAX_RTSP_NUM; i++) {		// search rtsp user
		if (rtsp_list[i].valid && rtsp_list[i].prtsp_ses.cur_state == STATE_PLAY && (rtsp_list[i].prtsp_ses.play_query & RTSPpq_AUDIO)) {
			sbuf_put(&(rtsp_list[i].sbuf_audio), addr, size, 0, 0);
#if ENX_NETWORK_Debug
			gptMsgDebug.RTSP_frame_aud_put[i]++;
#endif
		}
	}
	UNUSED(audio_type);
}
#endif
