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
#include "msg.h"

#include "main.h"
#include "enx_os.h"

#include "lwip/memp.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/tcp_impl.h"

#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_hdr_gen.h"
#include "rtcp_recv.h"

#define RTCP_HEADERSZ          4

#define ENX_RTCP_DGB_COMMAND	DEF_OFF
#define ENX_RTCP_DGB_PROCESSING	DEF_OFF
#define ENX_RTCP_DGB_RESPONSE	DEF_OFF
#define ENX_RTCP_DGB_DEBUG		DEF_OFF
#define ENX_RTCP_DGB_ERROR		DEF_ON

#define tprintf(debug, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);

static int rtcp_sr(rtcppk_t* rtcppk)
{
//	rtcppk->data.sr.sr_ssrc;
	tprintf(ENX_RTCP_DGB_DEBUG, "[SR]\r\n");
	return 0;

	UNUSED(rtcppk);
}

static int rtcp_rr(rtcppk_t* rtcppk)
{
//	rtcppk->data.rr.rr_ssrc;
//	rtcppk_rblock* rb = rtcppk->data.rr.rr_rb;
	tprintf(ENX_RTCP_DGB_DEBUG, "[RR]\r\n");
	return 0;

	UNUSED(rtcppk);
}

static int rtcp_sdes(rtcppk_t* rtcppk)
{
	tprintf(ENX_RTCP_DGB_DEBUG, "[SDES]\r\n");
	return 0;

	UNUSED(rtcppk);
}

static int rtcp_bye(rtcppk_t* rtcppk)
{
	tprintf(ENX_RTCP_DGB_DEBUG, "[BYE]\r\n");
	return 0;

	UNUSED(rtcppk);
}

static int rtcp_app(rtcppk_t* rtcppk)
{
	tprintf(ENX_RTCP_DGB_DEBUG, "[APP]\r\n");
	return 0;

	UNUSED(rtcppk);
}

int rtcp_process(rtsp_con_t *prtsp_conn, rtcppk_t* rtcppk, uint16 pklen)
{
//	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
//	rtcp_rx_info_t *prtcp_info = &(prtsp_conn->prtcp_info);
	int nBYEflag = 0;
	while (pklen > RTCP_HEADERSZ) {
		if (pklen < RTCP_LENGTH_BYTE(&rtcppk->header)) {
			tprintf(ENX_RTCP_DGB_ERROR, "p->tot_len(%d) rtcppk len(%d)\r\n", pklen, RTCP_LENGTH_BYTE(&rtcppk->header));
			break;
		}
 
		if (rtcppk->header.rtcp_ver != RTCP_VERSION) {
			tprintf(ENX_RTCP_DGB_ERROR, "version error(%d)\r\n", rtcppk->header.rtcp_ver);
		} else {
			switch (rtcppk->header.rtcp_type) {
				case RTCP_SR:   rtcp_sr(rtcppk);	break;
				case RTCP_RR:   rtcp_rr(rtcppk);	break;
				case RTCP_SDES: rtcp_sdes(rtcppk);	break;
				case RTCP_BYE:  rtcp_bye(rtcppk);	nBYEflag = 1; break;
				case RTCP_APP:  rtcp_app(rtcppk);	break;
				default:		tprintf(ENX_RTCP_DGB_ERROR, "error type\r\n"); return -1;
			}
		}

        pklen -= RTCP_LENGTH_BYTE(&rtcppk->header);  
        rtcppk = (rtcppk_t *)((char *)rtcppk + RTCP_LENGTH_BYTE(&rtcppk->header));  
    }

	return nBYEflag;

	UNUSED(prtsp_conn);
}

static void rtcp_bye_pcb(rtsp_con_t *prtsp_conn, int m_type, struct udp_pcb *pcb)
{
	if (RTP_UDP_PCB <= m_type*2+1) {
		tprintf(ENX_RTCP_DGB_ERROR, "RTP UDP PCB array overflow!(Max:%d, m_type:%d)\r\n", 
			RTP_UDP_PCB, m_type);
		return;
	}

	if (prtsp_conn->upcb[m_type*2+1]) {
		if (pcb) {
			if (pcb->local_port == prtsp_conn->upcb[m_type*2+1]->local_port) {
				if (prtsp_conn->upcb[m_type*2]) {
					udp_remove(prtsp_conn->upcb[m_type*2]);
					prtsp_conn->upcb[m_type*2] = NULL;
				}
				udp_remove(prtsp_conn->upcb[m_type*2+1]);
				prtsp_conn->upcb[m_type*2+1] = NULL;
			} else if (pcb == prtsp_conn->upcb[m_type*2+1]) {
				tprintf(ENX_RTCP_DGB_ERROR, "RTP UDP PCB local_port error?(%d/%d)\r\n", 
					pcb->local_port, prtsp_conn->upcb[m_type*2+1]->local_port);
			}
		} else {
			tprintf(ENX_RTCP_DGB_ERROR, "RTP UDP PCB null?(m_type:%d, port:%d)\r\n", 
				m_type, prtsp_conn->upcb[m_type*2+1]->local_port);
		}
	}
}

void rtcp_recv_fn(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	if (p == NULL) {
		tprintf(ENX_RTCP_DGB_ERROR, "p == NULL\r\n");
		return;
	}
	if (p->next == NULL) {
//		tprintf(ENX_RTCP_DGB_DEBUG, "p->next == NULL\r\n");
	}

	rtsp_con_t *prtsp_conn = (rtsp_con_t *)arg;
	rtcppk_t* rtcppk = (rtcppk_t*)p->payload;
	uint16 pklen = p->tot_len;
	int nBye;

//	if (pcb->local_port == prtsp_conn->upcb[VIDEO*2+1]->local_port) {
//		printf("UDP RTCP(V) IN\r\n");
//	} else if (pcb->local_port == prtsp_conn->upcb[AUDIO*2+1]->local_port) {
//		printf("UDP RTCP(A) IN\r\n");
//	}

//	hexDump((char *)__func__, (void *)rtcppk, pklen);

	prtsp_conn->poll_cnt = 0;

#if 0
	prtsp_conn->prtcp_info.valid = 1;
	if (prtsp_conn->upcb[VIDEO*2+1] == pcb) {
		prtsp_conn->prtcp_info.mtype = VIDEO;
	}
#ifdef __AUDIO__
	else if (prtsp_conn->upcb[AUDIO*2+1] == pcb) {
		prtsp_conn->prtcp_info.mtype = AUDIO;
	}
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
	else if (prtsp_conn->upcb[METADATA*2+1] == pcb) {
		prtsp_conn->prtcp_info.mtype = METADATA;
	}
#endif
#if (ONVIF_BACKCHANNEL==1)
	else if (prtsp_conn->upcb[BACKCHANNEL*2+1] == pcb) {
		prtsp_conn->prtcp_info.mtype = BACKCHANNEL;
	}
#endif
#endif
#endif

#if ENX_NETWORK_Debug
	gptMsgDebug.RTCP_packet_rx[prtsp_conn->valid-1]++;
#endif
	nBye = rtcp_process(prtsp_conn, rtcppk, pklen);

#if 1
	if (nBye == 1) {
		rtcp_bye_pcb(prtsp_conn, VIDEO, pcb);
#ifdef __AUDIO__
		rtcp_bye_pcb(prtsp_conn, AUDIO, pcb);
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
		rtcp_bye_pcb(prtsp_conn, METADATA, pcb);
#endif
#if (ONVIF_BACKCHANNEL==1)
		rtcp_bye_pcb(prtsp_conn, BACKCHANNEL, pcb);
#endif
#endif
	}
#endif

	pbuf_free(p);

	UNUSED(arg);
	UNUSED(pcb);
	UNUSED(addr);
	UNUSED(port);
}

void rtcp_make_sr(rtp_session_t *prtp_ses, BYTE *buffer)
{
	rtcppk_header *head = (rtcppk_header *)buffer;
	rtcppk_sr *sr = (rtcppk_sr *)(buffer + sizeof(rtcppk_header));

	head->rtcp_ver = 2;
	head->rtcp_pad = 0;
	head->rtcp_count = 0;
	head->rtcp_type = RTCP_SR;
	head->rtcp_length = 6;	// 6*4 = sr(24)

// http://lists.live555.com/pipermail/live-devel/2010-May/012207.html
	struct timeval now;
	gettimeofday(&now, NULL);
	UINT ntp_msw = gptMsgShare.TIME + 0x83AA7E80; // NTP timestamp most-significant word (1970 epoch -> 1900 epoch)
	UINT ntp_lsw = (UINT)(now.tv_usec / 15625.0 * 0x04000000 + 0.5); // NTP timestamp least-significant word
//	flprintf("PLAY RTCP TIME %10u check\r\n", prtp_ses->start_rtptime);

	sr->sr_ssrc = prtp_ses->ssrc;
	sr->sr_intts = ntp_msw;
	sr->sr_fracts = ntp_lsw;
	sr->sr_rtpts = prtp_ses->start_rtptime;
	sr->sr_packets = prtp_ses->packet_cnt;
	sr->sr_octets = prtp_ses->total_length;
}

int send_rtcp_sr(void *ctx, int media_type)
{
	rtsp_con_t *prtsp_conn = (rtsp_con_t *)ctx;
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[media_type];
	struct udp_pcb *upcb = prtsp_conn->upcb[media_type*2+1];
	struct tcp_pcb *tpcb = prtsp_conn->tpcb;
	err_t ert = ERR_MEM;
	int payload_len = sizeof(rtcppk_header) + sizeof(rtcppk_sr);
	BYTE payload[64] = {0};	// tcp only
	BYTE *pos = NULL;
	struct pbuf *p = NULL;
	static UINT allocate_fail_time = 0;
	static UINT send_fail_time = 0;

	// add Interleaved
	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			if (!(p = pbuf_alloc(PBUF_TRANSPORT, payload_len, PBUF_POOL))) {
				if (allocate_fail_time != gptMsgShare.TIME) {
					allocate_fail_time = gptMsgShare.TIME;
					printf("%s RTCP(SR/UDP) could not allocate pbuf_pool\r\n", sltLocalTime.data);
				}
				return DEF_FAIL;
			}
			pos = p->payload;
			break;

		case RTP_TCP:
		case RTP_HTTP:
			payload_len += sizeof(rthInterleaved);
			{
			rthInterleaved* interleaved = (rthInterleaved *)payload;
			interleaved->un8Magic = '$';
			interleaved->un8Channel = media_type * 2 + 1;
			interleaved->un16Length = 28;	// head(4), sr(24)
			}
			pos = payload + 4;
			break;

		default:
			flprintf("RTCP SR rtp_type fail(type:%d)\r\n", prtsp_ses->rtp_type);
			return DEF_FAIL;
	}

	rtcp_make_sr(prtp_ses, pos);

	// Send วัดู.
	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			if (upcb) {
				ip_addr_t dst_addr;
				dst_addr.addr = prtsp_ses->remote_ipaddr;
				if (prtsp_ses->nif) {
					ert = udp_sendto_if(upcb, p, &dst_addr, prtsp_ses->rtcp_port[media_type], prtsp_ses->nif);
				} else {
					ert = udp_sendto(upcb, p, &dst_addr, prtsp_ses->rtcp_port[media_type]);
				}
				pbuf_free(p);
				if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
					gptMsgDebug.RTCP_packet_tx[prtsp_conn->valid-1]++;
#endif
//					printf("RTCP SEND(SR/UDP)\r\n"); // OK
				} else {
					if (send_fail_time != gptMsgShare.TIME) {
						send_fail_time = gptMsgShare.TIME;
						printf("%s RTCP(SR/UDP) send error(%d)\r\n", sltLocalTime.data, ert);
					}
					return DEF_FAIL;
				}
			}
			break;

		case RTP_TCP:
		case RTP_HTTP:
			if (tpcb) {
				if (tcp_sndbuf(tpcb) >= (UINT)payload_len && tcp_sndqueuelen(tpcb) < TCP_SND_QUEUELEN) {
					// TCP PCB Setting
					tpcb->flags &= ~(TF_ACK_DELAY);
					tcp_nagle_disable(tpcb);

					ert = tcp_write(tpcb, payload, payload_len, TCP_WRITE_FLAG_COPY);
					if (ert == ERR_OK) {
						ert = tcp_output(tpcb);
						if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
							gptMsgDebug.RTCP_packet_tx[prtsp_conn->valid-1]++;
#endif
//							printf("RTCP SEND(SR/TCP)\r\n"); // OK
						} else {
							if (send_fail_time != gptMsgShare.TIME) {
								send_fail_time = gptMsgShare.TIME;
								printf("%s RTCP(SR/TCP) tcp_output error(%d)\r\n", sltLocalTime.data, ert);
							}
							return DEF_FAIL;
						}
					} else {
						if (send_fail_time != gptMsgShare.TIME) {
							send_fail_time = gptMsgShare.TIME;
							printf("%s RTCP(SR/TCP) tcp_write error(%d) ?\r\n", sltLocalTime.data, ert);
						}
						return DEF_FAIL;
					}
				} else {
					return DEF_FAIL;
				}
			}
			break;

		default:
			flprintf("RTCP SR rtp_type fail(type:%d)\r\n", prtsp_ses->rtp_type);
			return DEF_FAIL;
	}
//	hexDump("SR output!", buffer, payload_len);

	return DEF_OK;
}

void send_video_rtcp_sr(void *ctx)
{
	send_rtcp_sr(ctx, VIDEO);
}

#ifdef __AUDIO__
void send_audio_rtcp_sr(void *ctx)
{
	send_rtcp_sr(ctx, AUDIO);
}
#endif
