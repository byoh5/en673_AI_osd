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

#ifdef __ISP__
#ifdef __JPEG__

#include "dev_inline.h"
#include "msg.h"

#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "main.h"
#include "enx_video.h"
#include "enx_os.h"

#include "rtp_over_rtsp.h"
#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_hdr_gen.h"

#include "jpeg.h"

inline u32 get_page_of_image(u32 anSp, u32 anFsize, u8 abPp, u32 anBase)
{
	return ((anSp+PG_PAGE) <= anFsize) ? PG_PAGE : (anFsize - anSp);
	UNUSED(abPp);
	UNUSED(anBase);
}

/**
    build and initialize rtp header(include rtp over tcp) for jpeg 

    @bfirst : Is first rtp packet for transmitting jpeg ?
*/
static inline 
u32 jpeg_rtp_hdr_init(u8 rtp_type, u8 *ap_pkt, int bfirst, u8 an_width, u8 an_height) 
{
	jpeghdr_t *pjpghdr;  
	jpeghdr_qtable_t *pjqtbl;
	rtp_hdr_t *prtphdr ;
	u32 overhead=0;

    //printf("%s, rtp_type %d bfirst %d\r\n",__func__,rtp_type,bfirst);
	if(rtp_type == RTP_TCP || rtp_type == RTP_HTTP)
        overhead += sizeof(tcp_over_hdr_t);

	prtphdr = (rtp_hdr_t *)(ap_pkt+overhead);
	
	/* Initialize RTP header */
	prtphdr->version = RTP_VERSION;
	prtphdr->p  = 0;
	prtphdr->x  = 0;
	prtphdr->cc = 0;
	prtphdr->m  = 0;
	prtphdr->pt = RTP_PT_JPEG;

	overhead += sizeof(rtp_hdr_t);
	
	pjpghdr = (jpeghdr_t*)(ap_pkt+overhead);
	pjpghdr->tspec  = 0;
	pjpghdr->off    = 0;
	pjpghdr->type   = 0;  /* 4:2:2 jpeg format */
	
	/* Q values 128-255 indicate that a Quantization Table header appears 
	    after the main JPEG 	*/    
	pjpghdr->q      = 128; 
	pjpghdr->width  = an_width; //img hight size  
	pjpghdr->height = an_height; //img width size
	overhead += sizeof(jpeghdr_t);

	if(bfirst == 0)
		return overhead;
	
	
	pjqtbl = (jpeghdr_qtable_t *)(ap_pkt+overhead);
	
	pjqtbl->mbz = 0;
	pjqtbl->precision = 0;
	pjqtbl->length = 128;      
	
	return overhead+sizeof(jpeghdr_qtable_t);
}


/**
    jpeg fetch from isp and build packet for jpep rtp over tcp

    @ts         : rtp timestamp : 90000Hz unit
    @prtp_ses   : refer to rt_stream.h
    @prtp_info  : refer to rt_stream.h
    @pbuf       : output image buffer for sending to network.
*/
static int
rtp_build_jpeg_pkt(u32 ts,
            rtp_session_t *prtp_ses,
            rtp_tx_info_t *prtp_info,
            u8 *pbuf)
{
    rtsp_session_t *prtsp_ses = container_of(prtp_ses,rtsp_session_t,prtp_s[VIDEO]);
    u8 *pmsg =  (u8 *)pbuf;
    u8 *pjpeg_base = prtp_info->base + prtp_info->nOffset;
    volatile rtp_hdr_t *prtphdr;
    volatile jpeghdr_t *pjpghdr;
    tcp_over_hdr_t *ptcp_over_hdr;
    u32 overhead = 0;
    u32 rtp_hdr_len;
    u32 data_len;
	u32 nFtchNum = 0;

    /* Image fetch start */
    nFtchNum = get_page_of_image(prtp_info->nOffset,   
                                                prtp_info->nsize,
                                                0,
                                                (u32)prtp_info->base);

    /* Is there image left ? */
    if(nFtchNum == 0) 
        return 0;

    data_len = nFtchNum;
    prtp_info->nOffset += data_len;
    
    /* make rtp packet */
    //...............................................
    rtp_hdr_len = jpeg_rtp_hdr_init(prtsp_ses->rtp_type,
                                    pmsg,
                                    prtp_info->bFirst,
                                    enx_jpeg_get_width()>>3,
                                    enx_jpeg_get_height()>>3);

    //printf("%s, rtp_hdr_len %d \r\n", __func__,rtp_hdr_len);                                    
    pmsg = pmsg+rtp_hdr_len;

    /* If the first rtp packet of jpeg, quantization table is needed.*/
    if(prtp_info->bFirst == 1)                                                 
    {
        prtp_info->bFirst = 0;
        data_len = nFtchNum - JPEG_HDR_LEN;
        /* Y quantization table */
        DmaMemCpy_ip((BYTE *)pmsg          ,   ((u8*)pjpeg_base+YQUNT_OFF),    QUNT_SIZE);
        /* C quantization table */
        DmaMemCpy_ip((BYTE *)pmsg+QUNT_SIZE,   ((u8*)pjpeg_base+CQUNT_OFF),    QUNT_SIZE);
        /* Image copy */
        DmaMemCpy_ip((BYTE *)pmsg+YC_QUNT_SIZE,((u8*)pjpeg_base+JPEG_HDR_LEN) ,data_len); 
        data_len += YC_QUNT_SIZE;
    }
    else if (prtp_info->bFirst == 0)
    {
        data_len = nFtchNum;
        /* Image copy */
        DmaMemCpy_ip((BYTE *)pmsg, (u8*)pjpeg_base, data_len);                        
    }

	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			break;
		case RTP_TCP:
		case RTP_HTTP:
			ptcp_over_hdr = (tcp_over_hdr_t *)pbuf;
			/* tcp interace mode flag, by this flag, become rtp over tcp */
			ptcp_over_hdr->magic  = 0x24; 
			/* ch 0 video rtp, ch 1 video rtcp, ch 2 audio rtcp, ch3 audio rtcp */													  
			ptcp_over_hdr->ch = prtsp_ses->rtp_port[VIDEO]; 							  
			ptcp_over_hdr->length = rtp_hdr_len + data_len - 4;
			overhead += sizeof(tcp_over_hdr_t);
			break;
	}

    prtphdr = (rtp_hdr_t *)(pbuf+overhead);
    prtphdr->seq = ++prtp_ses->start_seq;
    prtphdr->ts = ts;
    prtphdr->ssrc = prtp_ses->ssrc;
    overhead += sizeof(rtp_hdr_t);
    
    pjpghdr = (jpeghdr_t*)(pbuf+overhead);
    pjpghdr->off = prtp_info->noff;
    prtp_info->noff += data_len;

    /* end condition for JPEG transmit */
    if(prtp_info->btx_st == JPEG_TX_END)                                       
        prtphdr->m = 1;

    return (data_len+rtp_hdr_len);
}

/**
    read jpeg stream and build rtp packet for transmitting.
*/
static int
rtp_get_jpeg_image(rtsp_con_t *prtsp_con, u8 *pbuf)
{
	rtp_session_t *prtp_ses = &(prtsp_con->prtsp_ses.prtp_s[VIDEO]);
	rtp_tx_info_t *prtp_info = &(prtsp_con->prtp_info);
	u16 *pused = &(prtsp_con->end);
    int len = 0;
    switch(prtp_info->btx_st)
    {
        case JPEG_TX_READY:
            if(prtp_info->sendFlag == 1)
            {
                UINT ts, flag, base;
                if(sbuf_get(&(prtsp_con->sbuf_video), &base, (UINT *)&prtp_info->nsize, &flag, &ts) == DEF_OK)
                {
                	prtp_info->base = (u8 *)base;
                    prtp_ses->start_rtptime += ts;
                    prtp_info->ts = prtp_ses->start_rtptime;
                    prtp_info->sendFlag = 0;
#if ENX_NETWORK_Debug
					gptMsgDebug.RTSP_frame_vid_get[prtsp_con->valid-1]++;
#endif
                }
                else
                {
                    prtp_info->nsize = 0;
                    goto end;
                }
            }
            else
            {
                prtp_info->nsize = 0;
                goto end;
            }

            prtp_ses->start = 1;
            prtp_info->noff = 0;                                               // Init sdram read start point
            prtp_info->nOffset  = 0;                                           // Init PingPong
//          prtp_info->bPp  = 0;                                               // First frame indicator
            prtp_info->bFirst = 1;
            prtp_info->btx_st = JPEG_TX_DOING;

            len = rtp_build_jpeg_pkt(prtp_info->ts,prtp_ses,prtp_info,pbuf);
            break;
        case JPEG_TX_DOING:
            if((prtp_info->nsize-prtp_info->nOffset)<=PG_PAGE)                //end condition for JPEG transmit
            {
                prtp_info->btx_st = JPEG_TX_END;
                if((prtp_info->nsize-prtp_info->nOffset)!= 0){
                    prtp_info->sendDone = 1;
                }
            }

            len = rtp_build_jpeg_pkt(prtp_info->ts,prtp_ses,prtp_info,pbuf);

            if(prtp_info->btx_st == JPEG_TX_END)
            {
                prtp_info->btx_st = JPEG_TX_READY;
                *pused     = 1;
            }
            break;
        default:
            return 0;
    }
end:
    return len;
}

/**
    rtp server used udp exploits only one port.
    and udp_pcb, remote ip and udp port number is required.
*/
char rtp_tx_jpeg(void *ctx)
{
	rtsp_con_t *prtsp_con = ctx;
	rtsp_session_t *prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[VIDEO];
	struct udp_pcb *pcb = prtsp_con->upcb[VIDEO];
	u32 len = 0;
	err_t ert = ERR_INPROGRESS;
	struct pbuf *p;
	u8_t *prtp_buf;
	struct ip_addr dst_addr;
	static UINT allocate_fail_time = 0;
	static UINT send_fail_time = 0;

	if (pcb == NULL) {
		return ert;
	}

	dst_addr.addr = prtsp_ses->remote_ipaddr;

	do {
		if (!(p = pbuf_alloc(PBUF_TRANSPORT, TCP_MSS, PBUF_POOL))) {
			if (allocate_fail_time != gptMsgShare.TIME) {
				allocate_fail_time = gptMsgShare.TIME;
				printf("%s RTP(MJPEG/UDP) could not allocate pbuf_pool\r\n", sltLocalTime.data);
			}
			return ert;
		}

		prtp_buf = p->payload;
		len = rtp_get_jpeg_image(prtsp_con, prtp_buf);
		if (len) {
			pbuf_realloc(p, len);
			if(prtsp_ses->nif) {
				ert = udp_sendto_if(pcb, p, &dst_addr, prtsp_ses->rtp_port[VIDEO], prtsp_ses->nif);
			} else {
				ert = udp_sendto(pcb, p, &dst_addr, prtsp_ses->rtp_port[VIDEO]);
			}
			if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
				gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
				gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += len;
#endif
				prtp_ses->packet_cnt++;
				prtp_ses->total_length += len;
			}

//			WaitXms(1);

			pbuf_free(p);                 
			if (ert != ERR_OK) {
				if (send_fail_time != gptMsgShare.TIME) {
					send_fail_time = gptMsgShare.TIME;
					printf("%s RTP(MJPEG/UDP) send error(%d)\r\n", sltLocalTime.data, ert);
				}
				break;
			}
		} else {
			pbuf_free(p);
			break;
		}
	} while(!prtsp_con->end);
	return ert;
}

char rtp_tx_jpeg_over_tcp(void *ctx)
{
	rtsp_con_t *prtsp_con = ctx;
	rtsp_session_t *prtsp_ses = &(prtsp_con->prtsp_ses);
	rtp_session_t *prtp_ses = (rtp_session_t *)&(prtsp_ses->prtp_s[VIDEO]);
	struct tcp_pcb *pcb = prtsp_con->tpcb;
	err_t ert = ERR_INPROGRESS;

	pcb->flags &= ~TF_ACK_DELAY;
	tcp_nagle_disable(pcb);

	while (tcp_sndbuf(pcb) >= TCP_MSS) {   //left TCP send buffer should be greater than or equal TCP_MSS 
		BYTE *prtp_buf = &(prtsp_con->send_buf[prtsp_con->jbuf_seq][0]);
		UINT len = rtp_get_jpeg_image(prtsp_con, prtp_buf);

		if (len == 0) {	// no jpeg data
			break;
		}

		ert = tcp_write(pcb, prtp_buf, len, 0);//TCP_WRITE_FLAG_COPY);
		if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
			gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
			gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += len;
#endif
			prtp_ses->packet_cnt++;
			prtp_ses->total_length += len;
		} else {
			printf("%s RTP(MJPEG/TCP) tcp_write error(%d)\r\n", sltLocalTime.data, ert);
			break;
		}

		num_loop(prtsp_con->jbuf_seq, NUM_JPEG_BUF);

		if (prtsp_con->end == 1) {
			break; //is complete to transmit a page of jpeg image
		}
	}

	ert = tcp_output(pcb);
	if (ert != ERR_OK) {
		printf("%s RTP(MJPEG/TCP) tcp_output error(%d)\r\n", sltLocalTime.data, ert);
	}

	return ert;
}
#endif
#endif
