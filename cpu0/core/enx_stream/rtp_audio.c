/* Copyright (c) 2013-15 Eyenix Corporation. All rights reserved.
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

#include "lwipopts.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"

#include "main.h"
#include "enx_os.h"
#include "enx_audio.h"

#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_audio.h"

#ifdef __AUDIO__

#define SEND_G711_BUFSIZE (1460)

static int audioSW = 1;
BYTE audio[1460];

static inline u32 g711_rtp_hdr_init(u8 rtp_type, u8 *ap_pkt)
{
	rtp_hdr_t *prtphdr = (rtp_hdr_t*)ap_pkt;
	u32 overhead = 0;

	if (rtp_type == RTP_TCP || rtp_type == RTP_HTTP) {
		overhead += sizeof(tcp_over_hdr_t);	
	}

	prtphdr = (rtp_hdr_t*)(ap_pkt + overhead);
	/* Initialize RTP header */
	prtphdr->version = RTP_VERSION;
	prtphdr->p  = 0;
	prtphdr->x  = 0;
	prtphdr->cc = 0;
	prtphdr->m  = 1;
	prtphdr->pt = RTP_PT_G711;

	return overhead + sizeof(rtp_hdr_t);
}

/**
    @pbuf       : buffer pointer to be build with G711 

    @return     : size of rtp packet for G.711
*/
int rtp_build_g711_pkt(rtsp_con_t *prtsp_con, BYTE *pbuf)
{
	rtsp_session_t* prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t* prtp_ses = &prtsp_ses->prtp_s[AUDIO];
	UINT rtp_hdr_len = 0;
	volatile BYTE *pmsg = (volatile BYTE *)pbuf;
	volatile rtp_hdr_t *prtphdr = (volatile rtp_hdr_t*)(pbuf);
	volatile tcp_over_hdr_t *ptcp_over_hdr;

	UINT g711_addr, g711_size, g711_flag, g711_temp;
	if (sbuf_get(&(prtsp_con->sbuf_audio), &g711_addr, &g711_size, &g711_flag, &g711_temp) != DEF_OK) {
	    return 0;
	}

#if ENX_NETWORK_Debug
	gptMsgDebug.RTSP_frame_aud_get[prtsp_con->valid-1]++;
#endif

	rtp_hdr_len = g711_rtp_hdr_init(prtsp_ses->rtp_type, pbuf);       /* Init rtp header */

	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			break;
		case RTP_TCP:
		case RTP_HTTP:
			//rtsp interleaved mode ... rtp over tcp
			ptcp_over_hdr = (tcp_over_hdr_t*)(pmsg);
			prtphdr = (rtp_hdr_t*)(pmsg + sizeof(tcp_over_hdr_t));
			ptcp_over_hdr->magic = 0x24;
			ptcp_over_hdr->ch = prtsp_ses->rtp_port[AUDIO];
			ptcp_over_hdr->length = rtp_hdr_len + g711_size - sizeof(tcp_over_hdr_t);
			break;
	}

	// Ts update
	prtp_ses->start_rtptime += g711_size;//500;//enx_h264_get_ts(0);//g711_size;

	/* build rtp header */
	prtphdr->seq = ++prtp_ses->start_seq;
	prtphdr->ts = prtp_ses->start_rtptime;
	prtphdr->ssrc = prtp_ses->ssrc;
	prtphdr->pt = 0;
	/* rtp time update */

	pmsg += rtp_hdr_len;
	//printf("audio %d \r\n",g711_size);
	/* audio data copy */
	//memcpy(pmsg, (BYTE*)g711_control.buf, audio_size);
	//DmaMemCpy(pmsg, (BYTE *)g711_control.buf, audio_size, NULL);
	//DmaMemCpy(pmsg, (BYTE *)g711_addr, g711_size, NULL);
	DmaMemCpy_ip((BYTE *)pmsg, (BYTE *)g711_addr, g711_size);
	invalidate_dcache_range((uint)pmsg, (uint)(pmsg + g711_size));
	return (rtp_hdr_len + g711_size);
}

/**
    get and build G711 RTP Packet.
*/
inline int rtp_get_audio_g711(rtsp_con_t *prtsp_con, BYTE *pbuf)
{
	if (cQueue_isempty(&(prtsp_con->sbuf_audio)) == DEF_OK) {
		return 0;
	}
    return rtp_build_g711_pkt(prtsp_con, pbuf);
}

/**
    rtp server used udp exploits only one port.
    and udp_pcb, remote ip and udp port number is required.
*/
char rtp_tx_g711(void *ctx)
{
	rtsp_con_t *prtsp_con = ctx;
	rtsp_session_t *prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[AUDIO];
	struct udp_pcb *pcb = prtsp_con->upcb[AUDIO*2];
	err_t ert = ERR_INPROGRESS;
	struct pbuf *p;
	struct ip_addr dst_addr;
	static UINT allocate_fail_time = 0;
	static UINT send_fail_time = 0;

	if (pcb == NULL) {
		return ert;
	}

	dst_addr.addr = prtsp_ses->remote_ipaddr;

	if (!(p = pbuf_alloc(PBUF_TRANSPORT, SEND_G711_BUFSIZE, PBUF_POOL))) {
		if (allocate_fail_time != gptMsgShare.TIME) {
			allocate_fail_time = gptMsgShare.TIME;
			printf("%s RTP(G.711/UDP) could not allocate pbuf_pool\r\n", sltLocalTime.data);
		}
		return ert;
	}

  	UINT len = rtp_get_audio_g711(prtsp_con, (BYTE *)p->payload);
	if (len) {
		if (audioSW) {
			pbuf_realloc(p, len);

			if (prtsp_ses->nif) {
				ert = udp_sendto_if(pcb, p, &dst_addr, prtsp_ses->rtp_port[AUDIO], prtsp_ses->nif);
			} else {
				ert = udp_sendto(pcb, p, &dst_addr, prtsp_ses->rtp_port[AUDIO]);
			}
			if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
				gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
				gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += len;
#endif
				prtp_ses->packet_cnt++;
				prtp_ses->total_length += len;
			} else {
				if (send_fail_time != gptMsgShare.TIME) {
					send_fail_time = gptMsgShare.TIME;
					printf("%s RTP(G.711/UDP) send error(%d)\r\n", sltLocalTime.data, ert);
				}
			}
		}
	}
	pbuf_free(p);

	return ert;
}

char rtp_tx_g711_over_tcp(void *ctx)
{
	rtsp_con_t *prtsp_con = ctx;
	rtsp_session_t *prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[AUDIO];
	struct tcp_pcb *pcb = prtsp_con->tpcb;
	err_t ert = ERR_INPROGRESS;

	pcb->flags &= ~TF_ACK_DELAY;
	tcp_nagle_disable(pcb);

	if (tcp_sndbuf(pcb) >= SEND_G711_BUFSIZE) {   //left TCP send buffer should be greater than or equal TCP_MSS 
		BYTE *prtp_buf = audio;//&(prtsp_con->send_buf[prtsp_con->jbuf_seq][0]);
		UINT len = rtp_get_audio_g711(prtsp_con, prtp_buf); 

		if (len) {
			if (audioSW) {
				ert = tcp_write(pcb, prtp_buf, len, TCP_WRITE_FLAG_COPY);
				if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
					gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
					gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += len;
#endif
					prtp_ses->packet_cnt++;
					prtp_ses->total_length += len;

					ert = tcp_output(pcb);
					if (ert != ERR_OK) {
						printf("%s RTP(G.711/TCP) tcp_output error(%d)\r\n", sltLocalTime.data, ert);
					}
				} else {
					printf("%s RTP(G.711/TCP) tcp_write error(%d)\r\n", sltLocalTime.data, ert);
					return ert;
				}
			}
		}
	}

	return ert;
}

void rtp_audio_switch(int onoff)
{
	printf("RTP audio switch(%d)\r\n", onoff);
	audioSW = onoff;
}

#if (AUDIO_RX)

typedef struct {
	u8_t bitrate;
	u8_t reserved;
	u16_t rev;
	u16_t seq;
	u16_t length;
	u8_t streams;
} G711Packet;

ISRD G711Buffer g711rx = {
	.strm_base = AUD_SPKBUF_BASE,
	.old_strm_base = AUD_SPKBUF_BASE,
	.head = 0,
	.tail = 0,
	.tot_num = G711_RX_COUNT
};

#define G711_RXDATA_SIZE 1024

#if (AUDIO_RX==1)
void rtp_rx_g711_backchannel(rthRTPHeader *RTPHdr, int left)
{
	BYTE *data = ((BYTE *)RTPHdr) + sizeof(rthRTPHeader);
	
	if (g711rx.old_seq != RTPHdr->un16Sequencenum) {
		CRITICAL_BEGIN;

		//printf("Dec");
		G711Dec((BYTE *)g711rx.strm_base, data, left, G711_uLaw);
		int nLowLength = left * 2;

		if (cQueue_isfull(&g711rx) == DEF_OK) {
			printf("%04d%s : Error, sbuf is full\r\n", __LINE__, __func__);
		}

		int length = nLowLength + g711rx.old_size;
		while (length >= G711_RXDATA_SIZE) {
			g711_data *auddata = &g711rx.sbuf[g711rx.tail];
			auddata->addr = g711rx.old_strm_base;
			auddata->size = G711_RXDATA_SIZE;
			//printf("Next");
			num_loop(g711rx.tail, g711rx.tot_num);

			length -= G711_RXDATA_SIZE;
			g711rx.old_strm_base += G711_RXDATA_SIZE;	// 남은 데이터의 새로운 시작위치
//			g711rx.old_size = length;					// 남은 데이터의 크기
		}

		// strm_base를 buffer의 끝으로 이동
		g711rx.strm_base += nLowLength;
		g711rx.old_size = length;

		if (g711rx.strm_base + (G711_RXDATA_SIZE*3) >= AUD_SPKBUF_END) {
			if (g711rx.old_size) {
				DmaMemCpy_isr((BYTE *)AUD_SPKBUF_BASE, (BYTE *)g711rx.old_strm_base, g711rx.old_size);
			}
			g711rx.old_strm_base = AUD_SPKBUF_BASE;
			g711rx.strm_base = AUD_SPKBUF_BASE + g711rx.old_size;
		}
		CRITICAL_END;

		g711rx.old_seq = RTPHdr->un16Sequencenum;
	}
	else {
		printf("%s Err Seq(%d/%d)\r\n", __func__, g711rx.old_seq, RTPHdr->un16Sequencenum);
	}
}

void rtp_rx_g711_over_udp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	if (p == NULL) {
		printf("p == NULL\r\n");
		return;
	}
	if (p->next == NULL) {
//		tprintf(ENX_RTCP_DGB_DEBUG, "p->next == NULL\r\n");
	}

//	rtsp_con_t *prtsp_conn = (rtsp_con_t *)arg;

	if (p->len < sizeof(rthRTPHeader)) {
		hexDump((char *)__func__, p->payload, p->len);
		pbuf_free(p);
		return;
	}

	printf("UDP RTP(BackCH) IN(size:%d)\r\n", p->len - sizeof(rthRTPHeader));
	hexDump((char *)__func__, p->payload, 16);
	rthRTPHeader* RTPHdr = (rthRTPHeader*)p->payload;
	rtp_rx_g711_backchannel(RTPHdr, p->len - sizeof(rthRTPHeader));

	pbuf_free(p);

	UNUSED(arg);
	UNUSED(pcb);
	UNUSED(addr);
	UNUSED(port);
}
#elif (AUDIO_RX==2)
static void rx_g711_over_udp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	if(gptMsgShare.AUD_TX_MODE == e_AtxNetRXG711)
	{
		int nLowLength;
		G711Packet *getData = (G711Packet *)p->payload;
		if(g711rx.old_seq != getData->seq)
		{
			CRITICAL_BEGIN;
#if 1
			G711Dec((BYTE *)g711rx.strm_base, &getData->streams, getData->length, G711_uLaw);
			nLowLength = getData->length * 2;

			if(cQueue_isfull(&g711rx) == DEF_OK)
			{
				printf("%04d%s : Error, sbuf is full\r\n", __LINE__, __func__);
			}

			int length = nLowLength + g711rx.old_size;
			while(length >= G711_RXDATA_SIZE)
			{
				g711_data *auddata = &g711rx.sbuf[g711rx.tail];
				auddata->addr = g711rx.old_strm_base;
				auddata->size = G711_RXDATA_SIZE;
				num_loop(g711rx.tail, g711rx.tot_num);

				length -= G711_RXDATA_SIZE;
				g711rx.old_strm_base += G711_RXDATA_SIZE;	// 남은 데이터의 새로운 시작위치
				g711rx.old_size = length;					// 남은 데이터의 크기
			}

			// strm_base를 buffer의 끝으로 이동
			g711rx.strm_base += nLowLength;

			if(g711rx.strm_base + (G711_RXDATA_SIZE*3) >= AUD_SPKBUF_END)
			{
				if(g711rx.old_size)
				{
					DmaMemCpy_isr((BYTE *)AUD_SPKBUF_BASE, (BYTE *)g711rx.old_strm_base, g711rx.old_size);
				}
				g711rx.old_strm_base = AUD_SPKBUF_BASE;
				g711rx.strm_base = AUD_SPKBUF_BASE + g711rx.old_size;
			}
#else
			DmaMemCpy_isr((BYTE *)g711rx.strm_base, &getData->streams, getData->length);
			nLowLength = getData->length;

			if(cQueue_isfull(&g711rx) == DEF_OK)
			{
				printf("%04d%s : Error, sbuf is full\r\n", __LINE__, __func__);
			}

			int length = nLowLength + g711rx.old_size;
			if (length >= G711_RXDATA_SIZE)
			{
				g711_data *auddata = &g711rx.sbuf[g711rx.tail];
				auddata->addr = g711rx.old_strm_base;
				auddata->size = G711_RXDATA_SIZE;
				num_loop(g711rx.tail, g711rx.tot_num);

				length -= G711_RXDATA_SIZE;
				g711rx.old_strm_base += G711_RXDATA_SIZE;	// 남은 데이터의 새로운 시작위치
				g711rx.old_size = length;					// 남은 데이터의 크기
			} else {
				g711rx.old_size = length;
			}

			// strm_base를 buffer의 끝으로 이동
			g711rx.strm_base += nLowLength;

			if(g711rx.strm_base + (G711_RXDATA_SIZE*3) >= AUD_SPKBUF_END)
			{
				if(g711rx.old_size)
				{
					DmaMemCpy_isr((BYTE *)AUD_SPKBUF_BASE, (BYTE *)g711rx.old_strm_base, g711rx.old_size);
				}
				g711rx.old_strm_base = AUD_SPKBUF_BASE;
				g711rx.strm_base = AUD_SPKBUF_BASE + g711rx.old_size;
			}
#endif
			CRITICAL_END;

			g711rx.old_seq = getData->seq;
		}
	}

	pbuf_free(p);

	UNUSED(arg);
	UNUSED(pcb);
	UNUSED(addr);
	UNUSED(port);
}

static int rx_g711_over_udp_init(void)
{
	struct udp_pcb *loc_g711_pcb = udp_new();
	err_t ert = udp_bind(loc_g711_pcb, IP_ADDR_ANY, AUDIO_RX_UDP_PORT);
	if (ert != ERR_OK) {
		printf("%s: bind fail\r\n", __func__);
		return DEF_FAIL;
	}
	udp_recv(loc_g711_pcb, rx_g711_over_udp_recv, NULL);
	return DEF_OK;
}

static void rx_g711_over_tcp_close(struct tcp_pcb *pcb)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb, NULL);
	tcp_close(pcb);
}

static void rx_g711_over_tcp_err(void *arg, err_t err)
{
	struct tcp_pcb *pcb = arg;
	if (arg != NULL) {
		switch (err) {
			case ERR_ABRT:
			case ERR_RST:
			case ERR_CLSD:
			case ERR_CONN:
				rx_g711_over_tcp_close(pcb);
				break;
		}
	}
	printf("%s(%d) : err(%d)\r\n", __func__, __LINE__, err);
}

static err_t rx_g711_over_tcp_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	if (err == ERR_OK) {
		if (p != NULL) {
			if (gptMsgShare.AUD_TX_MODE == e_AtxNetRXG711) {
				int nLowLength;
				G711Packet *getData = (G711Packet *)p->payload;
				if (g711rx.old_seq != getData->seq) {
					CRITICAL_BEGIN;

					G711Dec((BYTE *)g711rx.strm_base, &getData->streams, getData->length, G711_uLaw);
					nLowLength = getData->length * 2;

					if (cQueue_isfull(&g711rx) == DEF_OK) {
						printf("%04d%s : Error, sbuf is full\r\n", __LINE__, __func__);
					}

					int length = nLowLength + g711rx.old_size;
					while (length >= G711_RXDATA_SIZE) {
						g711_data *auddata = &g711rx.sbuf[g711rx.tail];
						auddata->addr = g711rx.old_strm_base;
						auddata->size = G711_RXDATA_SIZE;
						num_loop(g711rx.tail, g711rx.tot_num);

						length -= G711_RXDATA_SIZE;
						g711rx.old_strm_base += G711_RXDATA_SIZE;	// 남은 데이터의 새로운 시작위치
						g711rx.old_size = length;					// 남은 데이터의 크기
					}

					// strm_base를 buffer의 끝으로 이동
					g711rx.strm_base += nLowLength;

					if (g711rx.strm_base + (G711_RXDATA_SIZE*3) >= AUD_SPKBUF_END) {
						if (g711rx.old_size) {
							DmaMemCpy_isr((BYTE *)AUD_SPKBUF_BASE, (BYTE *)g711rx.old_strm_base, g711rx.old_size);
						}
						g711rx.old_strm_base = AUD_SPKBUF_BASE;
						g711rx.strm_base = AUD_SPKBUF_BASE + g711rx.old_size;
					}
					CRITICAL_END;

					g711rx.old_seq = getData->seq;
				}
			}

			/* Inform TCP that we have taken the data. */
			tcp_recved(pcb, p->tot_len);
			pbuf_free(p);
		} else {
			rx_g711_over_tcp_close(pcb);
		}
	}

	return ERR_OK;

	UNUSED(arg);
	UNUSED(pcb);
}

static err_t rx_g711_over_tcp_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	tcp_arg(pcb, pcb);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, rx_g711_over_tcp_recv);
	tcp_err(pcb, rx_g711_over_tcp_err);

	return ERR_OK;

	UNUSED(arg);
	UNUSED(err);
}

static int rx_g711_over_tcp_init(void)
{
	struct tcp_pcb *loc_g711_pcb = tcp_new();
	if (loc_g711_pcb) {
		err_t ert = tcp_bind(loc_g711_pcb, IP_ADDR_ANY, AUDIO_RX_TCP_PORT);
		if (ert != ERR_OK) {
			printf("%s: bind fail(%d)\r\n", __func__, ert);
			return DEF_FAIL;
		}
		loc_g711_pcb = tcp_listen(loc_g711_pcb);
		tcp_accept(loc_g711_pcb, rx_g711_over_tcp_accept);
		return DEF_OK;
	}
	return DEF_FAIL;
}
#endif

int enx_rx_g711_init(void)
{
#if (AUDIO_RX==1)
	g711rx.state = DEF_OK;
#elif (AUDIO_RX==2)
	g711rx.state = rx_g711_over_udp_init();
	rx_g711_over_tcp_init();
#endif
	return g711rx.state;
}
#endif
#endif
