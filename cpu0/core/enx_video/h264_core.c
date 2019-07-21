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

#include "dev.h"
#include "dev_inline.h"	// for inline
#include "h264.h"
#include "enx_video.h"
#include "enx_os.h"
#include "msg.h"
#include "user.h"

#ifdef __ISP__
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h
#endif

#ifdef __H264__
#ifndef __H264_MCH__

#include "bitratectl.h"

// rtp_over_rtsp.c
void put_video_streams(UINT addr, UINT size, UINT flags, UINT video_type, UINT ts);

ISRD h264_enc_t h264_enc;

//******************************************************************************
// Debug
//------------------------------------------------------------------------------
#if 0
void H264_regprintf(void)
{
	printf("=======================================\r\n");
	printf("H264_SIZE_T		: 0x%08X\r\n", H264_SIZE_T);			// 0x000
	printf("H264_STRIDE_T		: 0x%08X\r\n", H264_STRIDE_T);	// 0x008
	printf("H264_TMBNUM	 	: 0x%08X\r\n", H264_TMBNUM);			// 0x010
	printf("H264_PICTYPE_T	 	: 0x%08X\r\n", H264_PICTYPE_T);	// 0x020
	printf("H264_QSCALE	 	: 0x%08X\r\n", H264_QSCALE);			// 0x028
	printf("H264_ENCOPT_T	 	: 0x%08X\r\n", H264_ENCOPT_T);	// 0x030
	printf("H264_HDRINFO1_T	 	: 0x%08X\r\n", H264_HDRINFO1_T);	// 0x040
	printf("H264_HDRINFO2_T	 	: 0x%08X\r\n", H264_HDRINFO2_T);	// 0x048
	printf("H264_HDRINSERT_T	: 0x%08X\r\n", H264_HDRINSERT_T);	// 0x050
	printf("H264_PREPTR	 	: 0x%08X\r\n", H264_PREPTR);			// 0x060
	printf("H264_PSTPTR	 	: 0x%08X\r\n", H264_PSTPTR);			// 0x068
	printf("H264_INTRVC	 	: 0x%08X\r\n", H264_INTRVC);			// 0x080
	printf("H264_CURYBUF	 	: 0x%08X\r\n", H264_CURYBUF);		// 0x088
	printf("H264_CURCBUF	 	: 0x%08X\r\n", H264_CURCBUF);		// 0x090
	printf("H264_CUREBUF	 	: 0x%08X\r\n", H264_CUREBUF);		// 0x098
	printf("H264_REFYBUF	 	: 0x%08X\r\n", H264_REFYBUF);		// 0x0A0
	printf("H264_REFCBUF	 	: 0x%08X\r\n", H264_REFCBUF);		// 0x0A8
	printf("H264_RECYBUF	 	: 0x%08X\r\n", H264_RECYBUF);		// 0x0B0
	printf("H264_RECCBUF	 	: 0x%08X\r\n", H264_RECCBUF);		// 0x0B8
	printf("H264_STRMBUF	 	: 0x%08X\r\n", H264_STRMBUF);		// 0x0C0
	printf("H264_SBUFEND	 	: 0x%08X\r\n", H264_SBUFEND);		// 0x0C8
	printf("H264_STPSBUFW	 	: 0x%08X\r\n", H264_STPSBUFW);	// 0x0D0
	printf("H264_STPIBUFW	 	: 0x%08X\r\n", H264_STPIBUFW);	// 0x0D8
	printf("H264_STPIBUFR	 	: 0x%08X\r\n", H264_STPIBUFR);	// 0x0E0
	printf("H264_SBUFSIZE	 	: 0x%08X\r\n", H264_SBUFSIZE);	// 0x0E8
	printf("H264_AXIINFO1_T	 	: 0x%08X\r\n", H264_AXIINFO1_T);	// 0x0F0
	printf("H264_AXIINFO2_T	 	: 0x%08X\r\n", H264_AXIINFO2_T);	// 0x0F8
	printf("H264_ENCSTART_T	 	: 0x%08X\r\n", H264_ENCSTART_T);	// 0x100
	printf("H264_STATUS_T	 	: 0x%08X\r\n", H264_STATUS_T);	// 0x108
	printf("H264_GENBITS	 	: 0x%08X\r\n", H264_GENBITS);		// 0x110
	printf("H264_SBUFWPOS	 	: 0x%08X\r\n", H264_SBUFWPOS);	// 0x118
	printf("H264_INTRMASK_T	 	: 0x%08X\r\n", H264_INTRMASK_T);	// 0x120
	printf("H264_OPWAIT_T 		: 0x%08X\r\n", H264_OPWAIT_T);	// 0x128
	printf("H264_STRMPACK_T		: 0x%08X\r\n", H264_STRMPACK_T);	// 0x180
	printf("H264_STRWAIT 		: 0x%08X\r\n", H264_STRWAIT);		// 0x188
	printf("H264_ECHNNEL 		: 0x%08X\r\n", H264_ECHNNEL);		// 0x190
	printf("H264_MCHNNEL 		: 0x%08X\r\n", H264_MCHNNEL);		// 0x198
	printf("H264_PREBITS 		: 0x%08X\r\n", H264_PREBITS);		// 0x1A8
	printf("H264_MEMYBGN 		: 0x%08X\r\n", H264_MEMYBGN);		// 0x280
	printf("H264_MEMYEND 		: 0x%08X\r\n", H264_MEMYEND);		// 0x288
	printf("H264_MEMCBGN 		: 0x%08X\r\n", H264_MEMCBGN);		// 0x290
	printf("H264_MEMCEND 		: 0x%08X\r\n", H264_MEMCEND);		// 0x298
	printf("H264_MISIZE_T 		: 0x%08X\r\n", H264_MISIZE_T);	// 0x300
	printf("H264_MCABAC_T 		: 0x%08X\r\n", H264_MCABAC_T);	// 0x308
	printf("H264_MQSCALE 		: 0x%08X\r\n", H264_MQSCALE);		// 0x310
	printf("H264_MPFRAME 		: 0x%08X\r\n", H264_MPFRAME);		// 0x318
	printf("=======================================\r\n");
}
#endif

//******************************************************************************
// Code
//------------------------------------------------------------------------------
void enx_h264_raw_update(int ch, int step)
{
	h264_channel_info *chinfo = &h264_enc.chinfo[ch];
	if (chinfo->vlock_ready == 0) {
		if (step == 0) {
			chinfo->raw_buffer_flag = chinfo->raw_buffer_flag == 0 ? 1 : 0;
		}

#ifdef __ISP__
		h264_ibuf_t *ibuf_t = &chinfo->imgbuf.cur[chinfo->raw_buffer_flag];
		if (ch == 0) {	// main(ISP_YADR1) use
			ISP_YADR1w(((ibuf_t->y_addr) >> 4) << 1);
			ISP_CADR1w(((ibuf_t->c_addr) >> 4) << 1);
			ISP_GO1w(1);
			ISP_GO1w(0);
		} else {		// sub(ISP_YADR0) use
			ISP_YADR0w(((ibuf_t->y_addr) >> 4) << 1);
			ISP_CADR0w(((ibuf_t->c_addr) >> 4) << 1);
			ISP_GO0w(1);
			ISP_GO0w(0);
		}
#endif

		if (step == 1) {
			chinfo->raw_buffer_flag = chinfo->raw_buffer_flag == 0 ? 1 : 0;
		}

		chinfo->vlock_ready = 1;
	}
}

//******************************************************************************
// Processing
//------------------------------------------------------------------------------
u32 enx_h264_get_ts(int ch)
{	// H.264 ts = (90000 / gptMsgShare.VIDEO_FPS) * h264_enc.period
	u32 res = h264_enc.video_default_ts * (h264_enc.chinfo[ch].vlock_tscnt + 1) * h264_enc.chinfo[ch].param.period;
	h264_enc.chinfo[ch].vlock_tscnt = 0;
	return res;
}

UINT enx_h264_enc_check(u32 mode, u32 wdr, u32 dss)
{
	char *strModes[2] = {"noWDR", "WDR"};
	UINT change_flag = DEF_NO;
	if (mode != h264_enc.isp_frame || wdr != h264_enc.isp_wdr || dss != h264_enc.isp_dss) {
		change_flag = DEF_YES;
		h264_enc.isp_frame = mode;
		h264_enc.isp_wdr = wdr;
		h264_enc.isp_dss = dss;
		if (h264_enc.isp_wdr == 0) {
			h264_enc.max_frame = h264_enc.isp_frame;
//			h264_enc.video_default_ts = 90000 / h264_enc.max_frame;
			h264_enc.video_default_ts = ((90000*(h264_enc.isp_dss+1))/h264_enc.max_frame);
			if (h264_enc.max_frame == 60) {
				if (h264_enc.chinfo[0].param.width > 1280) {
					enx_h264_set_frame_rate_sf(e_s60f30, 0);
					enx_h264_set_frame_rate_sf(e_s60f30, 1);
				} else {
					enx_h264_set_frame_rate_sf(e_s60f60, 0);
					enx_h264_set_frame_rate_sf(e_s60f60, 1);
				}
			} else if (h264_enc.max_frame == 50) {
				if (h264_enc.chinfo[0].param.width > 1280) {
					enx_h264_set_frame_rate_sf(e_s50f25, 0);
					enx_h264_set_frame_rate_sf(e_s50f25, 1);
				} else {
					enx_h264_set_frame_rate_sf(e_s50f50, 0);
					enx_h264_set_frame_rate_sf(e_s50f50, 1);
				}
			} else if (h264_enc.max_frame == 30) {
				enx_h264_set_frame_rate_sf(e_s30f30, 0);
				enx_h264_set_frame_rate_sf(e_s30f30, 1);
			} else if (h264_enc.max_frame == 25) {
				enx_h264_set_frame_rate_sf(e_s25f25, 0);
				enx_h264_set_frame_rate_sf(e_s25f25, 1);
			}
		} else {																// WDR을 켰고...
			if (h264_enc.isp_wdr != 1) {
				printf("WDR value?(%d)\r\n", h264_enc.isp_wdr);
				h264_enc.isp_wdr = 1;
			}
			h264_enc.max_frame = h264_enc.isp_frame / 2;				// isp_frame은 60,50,30,25가 온다.
			h264_enc.video_default_ts = (90000 * 2) / h264_enc.isp_frame;
			if (h264_enc.max_frame == 30) {
				enx_h264_set_frame_rate_sf(e_s30f30, 0);
				enx_h264_set_frame_rate_sf(e_s30f30, 1);
			} else if (h264_enc.max_frame == 25) {
				enx_h264_set_frame_rate_sf(e_s25f25, 0);
				enx_h264_set_frame_rate_sf(e_s25f25, 1);
			} else if (h264_enc.max_frame == 15) {
				enx_h264_set_frame_rate_sf(e_s15f15, 0);
				enx_h264_set_frame_rate_sf(e_s15f15, 1);
			} else if (h264_enc.max_frame == 12) {
				enx_h264_set_frame_rate_sf(e_s15f15, 0);	// 12.5 fps?
				enx_h264_set_frame_rate_sf(e_s15f15, 1);	// 12.5 fps?
			}
		}
		printf("H.264 rate change(%s): %d/%d/%d/%d\r\n", strModes[h264_enc.isp_wdr], h264_enc.isp_frame, h264_enc.max_frame, h264_enc.isp_wdr, h264_enc.video_default_ts);
	}

	return change_flag;
}

extern BYTE gbMnDebugFnc;	// ISP debug mode(OSD align)

void enx_h264_irq_handler(void *arg)
{
//	printf("H");
	UINT status = H264_STATUS_T;
	H264_STATUS_T = status & 0xFFFF;

	if (status != 0x13) {
		if(status == 0x1) {
			printf("H.264: First Encoding(%d/%d)\r\n", H264_ECHNNEL, H264_MCHNNEL);
		} else {
			printf("H.264: Status(0x%04X)(%d/%d)\r\n", status, H264_ECHNNEL, H264_MCHNNEL);
		}
	}

#ifdef GPIO_H264_ENC_LED
	GpioSetLo(GPIO_H264_ENC_LED);
#endif

	if (status & H264_STATUS_ENCEND) {	//ENCODING END
//		printf("ENC END : per_sz(%d)\r\n", H264_PREBITS >> 3);

		if(gptMsgDebug.H264_POLL_LOG & 0x1) {
			printf("ENC-PFrame(%d) ", H264_PFRAME);
		}

		if ((u32)(H264_PREBITS >> 3) > h264_enc.chinfo[H264_ECHNNEL].imgbuf.stpsize.stp_size) {
			dprintf("h264_irq_handler : %dch H264_PREBITS : size(%d)\r\n", H264_ECHNNEL, (H264_PREBITS >> 3));
		}
	}

	if (status & H264_STATUS_VLCEND) {
//		printf("VLC END : strmbuf(0x%08X), gen_sz(%dbyte)\r\n\r\n", H264_STRMBUF, H264_GENBITS >> 3);
		h264_channel_info *chinfo = &(h264_enc.chinfo[H264_MCHNNEL]);
		h264_quant_control_t *quant = &(chinfo->quant);
		chinfo->stream_addr = H264_STRMBUF;
		chinfo->stream_size = H264_GENBITS >> 3;

//#ifdef ENX_DEBUG
//		if(chinfo->stream_size > chinfo->imgbuf.stpsize.stp_size)
//		{
//			printf("h264_irq_handler : %dch H264 overflow : size(%d)\r\n", H264_MCHNNEL, chinfo->stream_size);
//		}
//#endif

		UINT slicetype = H264_MPFRAME == 0 ? 1 : 0;

		// OSD Log: H.264 Encoding Infomation
#ifdef __ISP__
#ifdef __HW_OSD_H264__
#define OSD_H264_X	0
#define OSD_H264_Y	3
#define OSD_H264_Yd	14

		chinfo->vlcdone_tick++;
		if (chinfo->vlcdone_tick > 99999999) {
			chinfo->vlcdone_tick = 0;
		}

		char buffer[24] = {0};
		static int buf_len[2][4] = {{0}};
		static int osd_mode = 0;
		UINT selChnnel = H264_MCHNNEL;
		if ((selChnnel == 0 && gptMsgShare.VIDEO_OSD & OSD_H264_1) ||
			(selChnnel == 1 && gptMsgShare.VIDEO_OSD & OSD_H264_2)) {
	#if 1
			#undef OSD_H264_Y
			const UINT OSD_H264_Y = (gbMnDebugFnc) ? 17 : 1;

			static u32 stream_p_buf[2] = {0};
			static u32 cur_qp_buf[2] = {0};
			static u32 cur_qp_1d[2] = {0};

			//printf("TYPE:%d Q:%d SIZE:%d ID:%d\r\n", slicetype, cur_qp_1d[selChnnel], chinfo->stream_size, h264_enc.chinfo[selChnnel].param.i_interval);
			if(slicetype == 1) {
				if(gptMsgShare.REC_ON) DispStr("R", 0, 21, 1);

				buf_len[selChnnel][0] = sprintf(buffer, "%c%02d %6d", 'I', cur_qp_1d[selChnnel], chinfo->stream_size);
				DispStr(buffer, OSD_H264_Y+0+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][0]);

				const int iP_frame_size = (h264_enc.chinfo[selChnnel].param.i_interval > 0) ? h264_enc.chinfo[selChnnel].param.i_interval-1 : 1;

				buf_len[selChnnel][1] = sprintf(buffer, "%c%02d %6d", 'P', cur_qp_buf[selChnnel]/iP_frame_size, stream_p_buf[selChnnel]/iP_frame_size );
				DispStr(buffer, OSD_H264_Y+1+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][1]);

				stream_p_buf[selChnnel] += chinfo->stream_size;

				//buf_len[selChnnel][2] = sprintf(buffer, "%c %5dKBPS", (H264_PROFILEIDC == e_pBaseline) ? 'B' : H264_PROFILEIDC == e_pMain ? 'M' : 'X', stream_p_buf[selChnnel]>>7);
				buf_len[selChnnel][2] = sprintf(buffer, "BPS %5dK", stream_p_buf[selChnnel]>>7);
				DispStr(buffer, OSD_H264_Y+2+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][2]);

				stream_p_buf[selChnnel] = 0;
				cur_qp_buf[selChnnel] = 0;
			}
			else {
				stream_p_buf[selChnnel] += chinfo->stream_size;
				cur_qp_buf[selChnnel] += cur_qp_1d[selChnnel];
			}

			cur_qp_1d[selChnnel] = h264_enc.chinfo[selChnnel].param.cur_qp;
			osd_mode = gbMnDebugFnc;

		} else {
			if (buf_len[selChnnel][0]) {
				if ((selChnnel == 0 && gptMsgShare.VIDEO_OSD & ~OSD_H264_1) ||
					(selChnnel == 1 && gptMsgShare.VIDEO_OSD & ~OSD_H264_2)) {
					const UINT OSD_H264_Y = (osd_mode) ? 17 : 1;
					DispClr(OSD_H264_Y+0+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][0]);
					DispClr(OSD_H264_Y+1+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][1]);
					DispClr(OSD_H264_Y+2+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][2]);
					buf_len[selChnnel][0] = 0;
					buf_len[selChnnel][1] = 0;
					buf_len[selChnnel][2] = 0;
				}
			}
		}
	#else
			if (gbMnDebugFnc == 0) {
				osd_mode = 0;
				buf_len[selChnnel][0] = sprintf(buffer, "CH%d(%5s)", selChnnel, H264_CABACEN == 0 ? "CAVLC" : H264_CABACEN == 1 ? "CABAC" : "Unknown");
				DispStr(buffer, OSD_H264_Y+0+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][0]);
				buf_len[selChnnel][1] = sprintf(buffer, "%8s", H264_PROFILEIDC == e_pBaseline ? "Baseline" : H264_PROFILEIDC == e_pMain ? "Main" : "Unknown");
				DispStr(buffer, OSD_H264_Y+1+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][1]);
				buf_len[selChnnel][2] = sprintf(buffer, "%c%02d%7d", slicetype == 1 ? 'I' : 'P', h264_enc.chinfo[selChnnel].param.cur_qp, chinfo->stream_size);
				DispStr(buffer, OSD_H264_Y+2+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][2]);
				buf_len[selChnnel][3] = sprintf(buffer, "%08d", chinfo->vlcdone_tick);
				DispStr(buffer, OSD_H264_Y+3+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][3]);
			} else {
				osd_mode = 1;
				buf_len[selChnnel][0] = sprintf(buffer, "CH%d %8s %5s", selChnnel,
					H264_PROFILEIDC == e_pBaseline ? "Baseline" :
					H264_PROFILEIDC == e_pMain ? "Main" : "Unknown", H264_CABACEN == 0 ? "CAVLC" :
						H264_CABACEN == 1 ? "CABAC" : "Unknown");
				DispStr(buffer, OSD_H264_Yd+0+(selChnnel*2), OSD_H264_X, buf_len[selChnnel][0]);
				buf_len[H264_MCHNNEL][1] = sprintf(buffer, "    %c%02d %7d %04d", slicetype == 1 ? 'I' : 'P', h264_enc.chinfo[selChnnel].param.cur_qp, chinfo->stream_size, chinfo->vlcdone_tick);
				DispStr(buffer, OSD_H264_Yd+1+(selChnnel*2), OSD_H264_X, buf_len[selChnnel][1]);
			}
		} else {
			if (buf_len[selChnnel][0]) {
				if ((selChnnel == 0 && gptMsgShare.VIDEO_OSD & ~OSD_H264_1) ||
					(selChnnel == 1 && gptMsgShare.VIDEO_OSD & ~OSD_H264_2)) {
					if (osd_mode == 0) {
						DispClr(OSD_H264_Y+0+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][0]);
						DispClr(OSD_H264_Y+1+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][1]);
						DispClr(OSD_H264_Y+2+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][2]);
						DispClr(OSD_H264_Y+3+(selChnnel*5), OSD_H264_X, buf_len[selChnnel][3]);
					} else if (osd_mode == 1) {
						DispClr(OSD_H264_Yd+0+(selChnnel*2), OSD_H264_X, buf_len[selChnnel][0]);
						DispClr(OSD_H264_Yd+1+(selChnnel*2), OSD_H264_X, buf_len[selChnnel][1]);
					}
					buf_len[selChnnel][0] = 0;
					buf_len[selChnnel][1] = 0;
					buf_len[selChnnel][2] = 0;
					buf_len[selChnnel][3] = 0;
				}
			}
		}
	#endif
#endif
#endif

		u32 new_ts = enx_h264_get_ts(H264_MCHNNEL);
		if (new_ts != chinfo->stream_ts && gptMsgDebug.H264_POLL_LOG & 0x2) {
			printf("TS[%d] Change! %d -> %d\r\n", H264_MCHNNEL, chinfo->stream_ts, new_ts);
		}
		chinfo->stream_ts = new_ts;

		put_video_streams(chinfo->stream_addr, chinfo->stream_size, slicetype, H264_MCHNNEL, chinfo->stream_ts);
#ifdef RTMP_SUPPORT
		put_h264_streams_rtmp(chinfo->stream_addr, chinfo->stream_size, slicetype, H264_MCHNNEL, chinfo->stream_ts);
#endif

		chinfo->imgbuf.stream += H264_ALIGN(chinfo->stream_size);
		if ((chinfo->imgbuf.stream + (chinfo->imgbuf.stpsize.stp_size + h264_enc.avi_align_size)) >= chinfo->imgbuf.stream_end) {
			chinfo->imgbuf.stream = chinfo->imgbuf.stream_base;
		}

#ifdef __DEVICE_SD__
		if (enx_h264_get_sdsave() == DEF_ON && H264_MCHNNEL == 0) {
			u8 stream_type = 0;
			if (slicetype == 1) 	stream_type = eSTREAMMSG_H2641I;
			else					stream_type = eSTREAMMSG_H2641P;
			MsgRecPut(chinfo->stream_addr, chinfo->stream_size, chinfo->stream_ts, stream_type);
			CPU_SHARED1 |= eIP1_RECORD_INFO;
			CPU_IRQ1 = 1;
		}	
#endif

		static UINT var_cnt = 0;		// LOG용
		static UINT var_sum = 0;		// LOG용

		if (slicetype == 1) {	// I frame
			if(gptMsgDebug.H264_POLL_LOG & 0x1) {
				printf("%dframe\t%dbyte\t%dKbps\r\n", var_cnt, var_sum, (var_sum << 3) >> 10);
			}

			quant->seq = 0;
			quant->i_predic_sz = chinfo->stream_size;

			var_sum = chinfo->stream_size;			// LOG용
			var_cnt = 1;							// LOG용
		} else if (slicetype == 0) {	// P frame
			quant->seq++;
			quant->p_predic_sz = chinfo->stream_size;

			var_sum += chinfo->stream_size; 		// LOG용
			var_cnt++;								// LOG용
		}

		bitctl_update_frame_merge(slicetype, H264_MCHNNEL, (H264_GENBITS >> 3)); //H264_GENBITS >>3 , slicetype

#ifdef __ISP__
#ifdef __HW_OSD_H264__
		// OSD Log: Encoding Time
		static int buf_len2[2][2] = {{0}};
		if (gptMsgDebug.H264_POLL_LOG & 0x14) {
			h264_enc.ts_end = mfspr(EXR1K_HTBTR);
			UINT dep, time_us;
			if (h264_enc.ts_start < h264_enc.ts_end) {
				dep = h264_enc.ts_end - h264_enc.ts_start;
			} else {
				dep = (0xFFFFFFFF - h264_enc.ts_start) + h264_enc.ts_end;
			}
			time_us = dep / (BUS_CLK / 1000000);

			if (gptMsgDebug.H264_POLL_LOG & 0x4) {
				printf("H.264(%d) done(%dus)\r\n", H264_MCHNNEL, time_us);
			}

			if (gptMsgDebug.H264_POLL_LOG & 0x10) {
				const char output_set[6][6] = {"30NOR", "15WDR", "30DNR", "60NOR", "30WDR", "60DNR"};
				if (H264_MPFRAME == 0) {
					char buffer[40] = {0};
					buf_len2[selChnnel][0] = sprintf(buffer, "%d-%s-%s IDR(%d)  ", selChnnel, H264_PROFILEIDC == e_pBaseline ? "BP" : H264_PROFILEIDC == e_pMain ? "MP" : "xx", output_set[MP(FrameMode)], h264_enc.chinfo[selChnnel].param.i_interval);
					DispStr(buffer, 14+(selChnnel*2), 0, buf_len2[selChnnel][0]);
					if (gptMsgDebug.H264_TIME_CNT[selChnnel] > 0) {
						buf_len2[selChnnel][1] = sprintf(buffer, "Avg:%dus Max:%dus      ", gptMsgDebug.H264_TIME_SUM[selChnnel] / gptMsgDebug.H264_TIME_CNT[selChnnel], gptMsgDebug.H264_TIME_MAX[selChnnel]);
						DispStr(buffer, 15+(selChnnel*2), 0, buf_len2[selChnnel][1]);
					}
					gptMsgDebug.H264_TIME_SUM[selChnnel] = 0;
					gptMsgDebug.H264_TIME_CNT[selChnnel] = 0;
					gptMsgDebug.H264_TIME_MAX[selChnnel] = 0;
				}
				gptMsgDebug.H264_TIME_CNT[selChnnel] += 1;
				gptMsgDebug.H264_TIME_SUM[selChnnel] += time_us;
				if (gptMsgDebug.H264_TIME_MAX[selChnnel] < time_us) {
					gptMsgDebug.H264_TIME_MAX[selChnnel] = time_us;
				}
			} else {
				if (buf_len2[selChnnel][0]) {
					DispClr(14+(selChnnel*2), 0, buf_len2[selChnnel][0]);
					DispClr(15+(selChnnel*2), 0, buf_len2[selChnnel][1]);
					buf_len2[selChnnel][0] = 0;
					buf_len2[selChnnel][1] = 0;
				}
			}
		} else {
			if (buf_len2[selChnnel][0]) {
				DispClr(14+(selChnnel*2), 0, buf_len2[selChnnel][0]);
				DispClr(15+(selChnnel*2), 0, buf_len2[selChnnel][1]);
				buf_len2[selChnnel][0] = 0;
				buf_len2[selChnnel][1] = 0;
			}
		}
#endif
#endif
	}

	h264_enc.vlock_next--;

	if (h264_enc.vlock_next > 0) {
		printf("H.264(%d) overtime(%d)\r\n", H264_MCHNNEL, h264_enc.vlock_next);
		h264_enc.vlock_next = 0;
	}

//	printf("%s(%d) : VLOCK_CH0_VLC_END\r\n", __func__, __LINE__);
	if (h264_enc.vlock_fisrt == VLOCK_CH0_ENC_START) {
		h264_enc.vlock_fisrt = VLOCK_CH0_VLC_END;
	} else if (h264_enc.vlock_fisrt == ULOCK_CH1_ENC_START) {
		h264_enc.vlock_fisrt = ULOCK_CH1_VLC_END;
	}

	UNUSED(arg);
}

void h264_cbr_processing(int ch, UINT idr)
{
	h264_channel_info *chinfo = &(h264_enc.chinfo[ch]);
	h264_cbr_t *cbr = &(chinfo->cbr);
	h264_quant_control_t *quant = &(chinfo->quant);
	h264_param_t *param = &(chinfo->param);
	UINT average, un32QP = param->cur_qp;
	u32 max_frame = h264_enc.max_frame;

	//////////////////////////////////////////// exception handling ////////////////////////////////////////////

	//높은 비트레이트 에서 낮은 비트레이트로 설정될 시 cbr->bitrate가 quant->i_predic_sz보다 작아서 average가 음수가 될 가능성이 있다
	if ((int)(cbr->bitrate - quant->i_predic_sz) > 0) {
		average = (cbr->bitrate - quant->i_predic_sz) * param->period / max_frame;
	} else {
		average = cbr->bitrate * param->period / max_frame;
	}

	//급격히 증가하는 비트레이트 막기
	if (quant->frameCnt != 1) {
		if ((chinfo->stream_size << 3) > (average << 5)) {
			if (quant->flagFrame == 0)
				quant->flagFrame = 3;
		}
	}
	if (quant->flagFrame > 0) {
		un32QP += 2;
		if (un32QP > H264_Quantize_MAX) {
			un32QP = H264_Quantize_MAX;
		}
		quant->flagFrame--;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (idr == 1) {
		quant->frameCnt = 0;
	}

	if (cbr->mode == e_brmCBR) {	//CBR
//		if (h264_enc.max_frame / param->period == param->i_interval) { // IDR과 FPS가 같은 경우
//			if (h264_enc.max_frame == param->period) {	// 1fps(P가 없다)
			if (max_frame == param->period) {	// 1fps(P가 없다)
				// 이 경우 average식값과 i_predic_sz가 거의 같게 나와야 함
				if (quant->i_predic_sz < cbr->bitrate) {
					un32QP = (un32QP > (H264_Quantize_MIN)) ? un32QP-1 : H264_Quantize_MIN;
				} else if (quant->i_predic_sz > cbr->bitrate) {
					un32QP = (un32QP < (H264_Quantize_MAX)) ? un32QP+1 : H264_Quantize_MAX;
				}
			} else if (quant->frameCnt == (param->i_interval - 1)) {	// i frame
				if (quant->i_predic_sz < average) {
					un32QP = (un32QP > (H264_Quantize_MIN+1)) ? un32QP-2 : H264_Quantize_MIN;
				} else if (quant->i_predic_sz > average) {
					un32QP = (un32QP < (H264_Quantize_MAX-1)) ? un32QP+2 : H264_Quantize_MAX;
				}
			} else {	// p frame
				if (quant->p_predic_sz < average){
					un32QP = (un32QP > H264_Quantize_MIN) ? un32QP-1 : H264_Quantize_MIN;
				} else if(quant->p_predic_sz > average){
					un32QP = (un32QP < H264_Quantize_MAX) ? un32QP+1 : H264_Quantize_MAX;
				}
			}
//		} else if (h264_enc.max_frame / param->period < param->i_interval){	// ex)30fps인데 idr:60인 경우(I가 없는 case)
//			printf("%s(%d) : bad case 1\r\n", __func__, __LINE__);
//		} else if (h264_enc.max_frame / param->period > param->i_interval){	// ex)30fps인데 idr:15인 경우(I가 여러개 case)
//			printf("%s(%d) : bad case 2\r\n", __func__, __LINE__);
//		}
	} else {	//VBR
		if (quant->frameCnt == (param->i_interval - 1)) {
			un32QP = (un32QP < (H264_Quantize_MAX-1)) ? un32QP+2 : H264_Quantize_MAX;
		} else {
			if (quant->p_predic_sz < average) {
				un32QP = (un32QP > quant->qscale) ? un32QP-1 : quant->qscale;
			} else if (quant->p_predic_sz > average) {
				un32QP = (un32QP < H264_Quantize_MAX) ? un32QP+1 : H264_Quantize_MAX;
			}
		}
	}

	if (un32QP > H264_Quantize_MAX) {
		printf("Bug!! Q over!! [%d]\r\n", un32QP);
		un32QP = H264_Quantize_MAX;
	}

	H264_Quantize(ch, un32QP);

	if (param->i_interval > 1) {
		quant->frameCnt++;
	} else {
		quant->frameCnt = 1;
	}
}

static void h264_enc_start(int ch)
{
//	printf("%s(ch:%d)\r\n", __func__, ch);
	UINT picM, picQ, picV, idr_flag, slicetype;	//, tmp_y, tmp_c
	h264_param_t *param = &(h264_enc.chinfo[ch].param);
	h264_image_buf_t *imgbuf = &(h264_enc.chinfo[ch].imgbuf);
	h264_ibuf_t *imgcur = &imgbuf->cur[h264_enc.chinfo[ch].raw_buffer_flag];

	picQ = imgbuf->pic % 2; 	// 0 or 1
	picV = picQ == 0 ? 1 : 0;
	picM = 2 * ch + picQ;	// 0ch(0 or 1) or 1ch(2 or 3)

///////////////////////////////////////////////////////////////////////////// Buffer swap, Slice select
#if 0
	tmp_y = imgbuf->ref.y_addr;
	tmp_c = imgbuf->ref.c_addr;
	imgbuf->ref.y_addr = imgbuf->rec.y_addr;
	imgbuf->ref.c_addr = imgbuf->rec.c_addr;
	imgbuf->rec.y_addr = tmp_y;
	imgbuf->rec.c_addr = tmp_c;
#endif

	slicetype = (imgbuf->pic%param->i_interval==0) ? 0 : 1;   // 0 - I Slice, 1 - P Slice
#if 1
	if (slicetype == 1 && gptMsgShare.Rtsp_connect[ch] == 1 && param->i_interval > 30 && h264_enc.avi_save != DEF_ON) {
//		printf("rtsp connect! set i frame\r\n");
		gptMsgShare.Rtsp_idr[ch] = param->i_interval;
		slicetype = 0;
		param->i_interval = 3;
		gptMsgShare.Rtsp_connect[ch]++;
	} else if (slicetype == 0 && gptMsgShare.Rtsp_connect[ch] > 1) {
		if (gptMsgShare.Rtsp_connect[ch]++>3) {
//			printf("rtsp connect! set back\r\n");
			gptMsgShare.Rtsp_connect[ch] = 0;
			if (gptMsgShare.Rtsp_idr[ch] > 0 && gptMsgShare.Rtsp_idr[ch] < 240 && gptMsgShare.Rtsp_idr[ch] != param->i_interval) {
				param->i_interval = gptMsgShare.Rtsp_idr[ch];
			}
		}
	}
#endif
	idr_flag = ((param->i_interval>1 && slicetype==0) || (param->i_interval==1)) ? 1 : 0;
	// i_interval == 1이면 imgbuf->pic%256은 0 ~ 255로 반복되야 한다.(8<<8이므로 8bit의 값 이내에서 반복)
	// 아닌 경우 i_interval이 30인 경우 imgbuf->pic%i_interval 0 ~ 29

///////////////////////////////////////////////////////////////////////////// CBR / VBR
	if (slicetype == 0) { // 0 - I Slice, 1 - P Slice
		bitctl_init_gop(ch);
	}

	if (h264_enc.chinfo[ch].cbr.mode == e_brmVBR || h264_enc.chinfo[ch].cbr.mode == e_brmCBR) {
		h264_cbr_processing(ch, idr_flag);
	} else if (h264_enc.chinfo[ch].cbr.mode == e_brmCVBR) {
		bitctl_init_frame_i_p(ch, slicetype);
	}

	if (idr_flag == 1) {
		param->pps_qp = param->cur_qp;
	}
///////////////////////////////////////////////////////////////////////////// Setting

	// Image size
	H264_SIZE_T 	= (param->width<<16) + (param->height);
	H264_STRIDE_T	= (param->ystride<<16) + param->cstride;//(param->width<<16) + (param->width); // no uv_interleave
	H264_TMBNUM 	= param->num_mb;

	// Encoding option
	H264_PICTYPE_T	= (idr_flag<<1) + (slicetype);
	H264_QSCALE 	= param->cur_qp;
	H264_ENCOPT_T	= (0<<19) + (1<<18) + (0<<17) + (param->pps_qp<<8) + (param->srchmode<<0);	// ringtype = 0,

	// Header information
	H264_HDRINFO1_T = (8<<8) + ((param->i_interval==1) ? 0 : (imgbuf->pic%param->i_interval));
	H264_HDRINFO2_T = (param->profile_idc<<16) + (param->cabac_idc<<10) + (param->entropy<<9) + (param->crop_b<<8) + (param->level_idc&0xff);
	H264_HDRINSERT_T= ((idr_flag==1) ? 0x03 : 0x00);

	// Coding buffer point
	H264_PREPTR 	= picM;
	H264_PSTPTR 	= imgbuf->pre_idxnum1;

	// Image buffer
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))	// Main ISP Input Image
	if ((h264_enc.sen_width == param->width) && (h264_enc.sen_height == param->height)) {
		H264_CURYBUF= imgcur->y_addr + (param->width * 2);
		H264_CURCBUF= imgcur->c_addr + (param->width);
	} else {
		H264_CURYBUF= imgcur->y_addr;
		H264_CURCBUF= imgcur->c_addr;
	}
#else
	H264_CURYBUF= imgcur->y_addr;
	H264_CURCBUF= imgcur->c_addr;
#endif
	H264_REFYBUF	= imgbuf->ref[picQ].y_addr;
	H264_REFCBUF	= imgbuf->ref[picQ].c_addr;
	H264_RECYBUF	= imgbuf->ref[picV].y_addr;
	H264_RECCBUF	= imgbuf->ref[picV].c_addr;
	H264_STRMBUF	= imgbuf->stream + h264_enc.avi_align_size;
	H264_STPSBUFW	= imgbuf->stp[picQ].stp_addr;
	H264_STPIBUFW	= imgbuf->stp[picQ].stp_info_addr;
	H264_STPIBUFR	= imgbuf->stp[imgbuf->pre_idxnum2].stp_info_addr;
//	H264_SBUFSIZE	= imgbuf->stpsize.stp_size;

	// Channel control
	H264_ECHNNEL	= ch;
	H264_MCHNNEL	= param->vlc_mch;

	// VLC control parameter
	H264_MISIZE_T	= param->vlc_misize;
	H264_MCABAC_T	= param->vlc_mcabac;
	H264_MQSCALE	= param->vlc_mqscale;
	H264_MPFRAME	= param->vlc_mpframe;

	param->vlc_mch = ch;
	param->vlc_misize = (param->width<<16) + (param->height);
	param->vlc_mcabac = (param->cabac_idc<<1) + param->entropy; // main profile only
	param->vlc_mqscale = param->cur_qp; 			// enc에 사용되었던 qp
	param->vlc_mpframe = slicetype; 				// enc의 slice type

#if 0
	if (gptMsgShare.H264_ENC_LOG == 1) {
		gptMsgShare.H264_ENC_LOG = 0;
		H264_regprintf();
	}
#endif

	if (param->first_enc == 0) {
		param->first_enc = 1;
		H264_INTRMASK_T = 0x16; 				// Interrupt Masking All
		H264_ENCSTART_T = 0x01; 				// 처음 시작은 Encoding만 수행
	} else {
		H264_INTRMASK_T = 0x15;
		H264_ENCSTART_T = 0x03;
		NOP();
		NOP();
		NOP();
		H264_ENCSTART_T = 0x10;
	}

	if (gptMsgDebug.H264_POLL_LOG & 0x14) {
		h264_enc.ts_start = mfspr(EXR1K_HTBTR);
	}

#ifdef GPIO_H264_ENC_LED
	GpioSetHi(GPIO_H264_ENC_LED);
#endif

	imgbuf->pre_idxnum1 = picM;
	imgbuf->pre_idxnum2 = picQ;
	imgbuf->pic++;
	if (imgbuf->pic >= param->max_frame) {
		imgbuf->pic = 0;
	}
}

void enx_h264_enc_poll(void)
{
	h264_channel_info *chinfo0 = &h264_enc.chinfo[0];
	h264_channel_info *chinfo1 = &h264_enc.chinfo[1];

	if (gptMsgDebug.H264_POLL_LOG & 0x8) {
		gptMsgDebug.H264_POLL_LOG &= ~0x8;
		printf("vlock_first(0x%03X) hold(%d) ready(%d/%d)\r\n", h264_enc.vlock_fisrt, h264_enc.hold, chinfo0->vlock_ready, chinfo1->vlock_ready);
	}

	if (h264_enc.vlock_fisrt & (VLOCK_GO | VLOCK_CH0_VLC_END | ULOCK_CH1_VLC_END) && h264_enc.hold == 0) {
#ifdef __H264_2CH__
		if (chinfo1->vlock_ready) {
			chinfo1->vlock_ready = 0;
			if (!(gptMsgShare.VIDEO_TICK % chinfo1->param.period)) {
				chinfo1->vlock_tscnt += 1;
				h264_enc.vlock_fisrt = ULOCK_CH1_ENC_START;
				h264_enc_start(1);
				h264_enc.vlock_next++;
				chinfo1->vlock_tscnt--;
			}
		} else
#endif
		if (chinfo0->vlock_ready) {
			chinfo0->vlock_ready = 0;
			if (!(gptMsgShare.VIDEO_TICK % chinfo0->param.period)) {
				chinfo0->vlock_tscnt += 1;
				h264_enc.vlock_fisrt = VLOCK_CH0_ENC_START;
				h264_enc_start(0);
				h264_enc.vlock_next++;
				chinfo0->vlock_tscnt--;
			}
		}
	}
}
#endif //__H264_MCH__
#endif
