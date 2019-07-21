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

#ifndef _H264_H_
#define _H264_H_

#include "user.h"

#ifdef __H264__
#ifndef __H264_MCH__

// http://www.cardinalpeak.com/blog/the-h-264-sequence-parameter-set/
#define LEVEL_IDC		41	// 0x29 = Level 4.1

typedef enum{
	e_eCAVLC = 0,
	e_eCABAC = 1
}H264EntropyMode;

typedef enum{
	e_pBaseline = 66,
	e_pMain = 77
}H264Profile;

#define V_CH1A  128 
#define H_SCALE 0x100
#define V_SCALE 0x100

#define STM_BUF 16
#define MAX_STP_BUF	2

#ifndef H264_ROI_CONFIG
#define H264_ROI_CONFIG 0
#endif

#define H264_ALIGN_SIZE   	4096
#define H264_ALIGN(x) 		(((x) + H264_ALIGN_SIZE - 1) & ~(H264_ALIGN_SIZE - 1))

#define H264_Quantize(ch, quantize)	do { h264_enc.chinfo[ch].param.cur_qp = quantize; }while(0);
#define H264_Default_Quantize(ch, quantize)	do { h264_enc.chinfo[ch].quant.qscale = quantize; }while(0);

#define H264_IRQ_LOCK()		CRITICAL_BEGIN
#define H264_IRQ_UNLOCK()	CRITICAL_END


#define H264_STATUS_ENCEND		0x01
#define H264_STATUS_ENCVLCEND	0x02
#define H264_STATUS_FLUSHEND	0x04
#define H264_STATUS_VLCEND		0x10
#define H264_STATUS_SBUFFFULL	0x40
#define H264_STATUS_STRMERR		0x4000

typedef struct {
	u32 stp_count;
	u32 stp_size;
	u32 stp_info_size;
} h264_stpsize_t;

typedef struct {
	u32 stp_addr;
	u32 stp_info_addr;
} h264_stp_t;

typedef struct {
	u32 y_size;
	u32 c_size;
} h264_ibufsize_t;

typedef struct {
	u32 y_addr;
	u32 c_addr;
} h264_ibuf_t;

typedef struct {
	u32 stream;				/* h.264 output stream */  
	u32 stream_base;		/* h.264 stream region*/
	u32 stream_end;
	u32 internal_base;		/* h.264 H/W codec use this merory region */    
	h264_ibuf_t cur[2];
	h264_ibuf_t ref[2];
	h264_ibufsize_t bufsize;
	h264_stp_t stp[MAX_STP_BUF];
	h264_stpsize_t stpsize;

	u32 pic;
	u32 pre_idxnum1;
	u32 pre_idxnum2;
} h264_image_buf_t;

/* h264 codec parameter for each ch */
typedef struct {
	u32 profile_idc;
	u32 level_idc;
	u32 cabac_idc;
	u32 entropy;
	u32 width;
	u32 height;

	u32 ystride;
	u32 cstride;
	u32 ysize;
	u32 csize;

	u32 hscale;		// scaler ratio(width)
	u32 vscale;		// scaler ratio(height)
	u32 period;		// contorl h.264 frame rate : h.264 frame 30Hz(vlock)/period
	u32 i_interval;
	u32 max_frame;
	u32 cur_qp;
	u32 pps_qp;
	u32 num_mb;
	u32 crop_b;
	u32 srchmode;	/* if img_size is less then 720x480, then 1 else 2 is set */	

	u32 first_enc;

	u32 vlc_mch;
	u32 vlc_misize;
	u32 vlc_mcabac;
	u32 vlc_mqscale;
	u32 vlc_mpframe;
} h264_param_t; 

typedef struct {
	u32 mode;
	u32 bitrate;
} h264_cbr_t;

typedef struct {
    int seq;
    UINT qscale;
	UINT i_predic_sz;
    UINT p_predic_sz;
	UINT flagFrame;
	UINT frameCnt;
} h264_quant_control_t;

typedef struct {
	u32 vlock_ready;
	u32 vlock_tscnt;
	u32 vlcdone_tick;
	u32 stream_addr;		/* addr to current h264 stream output */
	u32 stream_size;		/* size of current h264 stream */
	u32 stream_ts;
	u32 raw_buffer_flag;
	h264_image_buf_t imgbuf;
	h264_param_t param;		/* codec parameter */
	h264_quant_control_t quant;
	h264_cbr_t cbr;			/* CBR/VBR info */
} h264_channel_info;

typedef struct {
	u32 enable;
	u32 vlock_fisrt;
	u32 vlock_next;
	u32 sen_height;			/* sensor image hight*/
	u32 sen_width;			/* sensor image width*/
//	u32 stream_done;		/* 1 = h264 vlc end, else 0*/
//	u32 stream_ready;		/* h264 stream ready 1 */  
	u32 num_ch;				/* number of h.264 channel */

	u32 ts_start;
	u32 ts_end;

	u32 hold;
	u32 isp_wdr;
	u32 isp_dss;
	u32 isp_frame;
	u32 isp_outmode;
	u32 max_frame;
	u32 video_default_ts;	//
	
	u32 avi_save;			// 0:off, 1:on
	u32 avi_align_size;

	h264_channel_info chinfo[2];
} h264_enc_t;

void enx_h264_info_print(void);

extern void enx_h264_on(void);
extern void enx_h264_image_capture_ready(void);
extern void enx_h264_raw_update(int ch, int step);
extern UINT enx_h264_enc_check(u32 mode, u32 wdr, u32 dss);
extern void enx_h264_enc_poll(void);

void h264_default_buffer(u32 h264_buf_addr, u32 h264_ch1_size, u32 h264_ch2_size);
void enx_h264_start(void);
void enx_h264_init(u32 num_ch, u32 raw_img_width, u32 raw_img_height);

void enx_h264_irq_handler(void *arg);

u32 enx_h264_get_ts(int ch);

#if H264_ROI_CONFIG
void enx_h264_set_roi_0(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale);
void enx_h264_set_roi_1(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale);
void enx_h264_set_roi_2(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale);
void enx_h264_set_roi_3(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale);
#endif
void enx_h264_set_sdsave(u32 value);
void enx_h264_set_maxframe(u32 value);
extern int enx_h264_get_sdsave(void);
extern int enx_h264_get_maxframe(void);
void enx_h264_set_ts(u32 value);
extern UINT h264_get_height(int ch);
extern UINT h264_get_width(int ch);
void enx_h264_set_profile(H264ProfileMode profilemode, int ch);
void enx_h264_set_idrframe_period(int period, int ch);
extern u32 enx_h264_get_idr_interval(int ch);
void enx_h264_set_quantize(int quantize, int ch);
extern int enx_h264_get_quantize(int ch);
extern int enx_h264_get_default_quantize(int ch);
void enx_h264_set_frame_size_rs(Resolution size, int ch);
void enx_h264_set_frame_rate_sf(SensorFps period, u32 ch);
extern int h264_get_period(u32 ch);
void enx_h264_set_bit_rate(H264ConstantBitRate cbr, int ch);
extern int enx_h264_get_bit_rate(int ch);
void enx_h264_set_bit_rate_mode(u32 nBrmode, int ch);

// TEST: call RuntimeMsg
extern void h264_set_frame_size(UINT width, UINT height, int ch);
extern int enx_h264_get_bit_rate_mode(int ch);

//
extern int enx_h264_get_stp_size(int ch);
extern int enx_h264_get_slice_type(int ch);
extern u32 enx_h264_get_prebits(void);

extern h264_enc_t h264_enc;
#endif
#endif
#endif // _H264_H_
