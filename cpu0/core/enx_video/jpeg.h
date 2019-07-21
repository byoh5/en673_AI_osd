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

#ifndef _JPEG_H_
#define _JPEG_H_

#include "user.h"			// use to type(Resolution, SensorFps, JpegConstantBitRate)

#define JPEG_ALIGN_SIZE   	32
#define JPEG_ALIGN(x) 		(((x) + JPEG_ALIGN_SIZE - 1) & ~(JPEG_ALIGN_SIZE - 1))

typedef struct {
	u32 mode;
	u32 quantCBR;
	u32 quantVBR;
	u32 mbps;
	u32 bitrate;
#if (JPEG_PREVENT_OVF==1)
	u32 dynamic_bitrate;
	u32 sqrt_rbr_max;
	u32 sqrt_rbr_min;
	u32 sqrt_dbr_max;
	u32 sqrt_dbr_min;
#endif
} jpeg_brmode_t;

typedef struct {
	u32 sen_width;	// sensor image width
	u32 sen_height;	// sensor image height
//	u32 width;		// jpeg image width
//	u32 height;		// jpeg image height
	u32 hscale;		// scaler ratio(width)
	u32 vscale;		// scaler ratio(height)
    u32 period;		// contorl jpeg frame rate
	u32 qp;			// qan parameter
	u32 qp_idx;		// qan index
	u32 ori_qp;		// original qan parameter
	u32 dynamic_qp;	// QBRmode qan parameter
	u32 max_size;	// jpeg max stream size
} jpeg_param_t;

typedef struct {
	u32 addr;
	u32 size;
	u32 lock;
} jpeg_ibuf_t;

typedef struct {
	u32 index;
	u32 index_lenth;

	u32 stream;
	u32 stream_base;
	u32 stream_end;
#if JPEG_BUF_CNT != 0
	jpeg_ibuf_t buf[JPEG_BUF_CNT];
#endif
} jpeg_image_buf_t;

typedef struct {
	u32 buf_len[2];
} jpeg_osd_t;

typedef struct {
    u32 enable;
	u32 done;
	u32 hold;

	u32 vlock_tscnt;
    u32 vlock_tick;				// vlock tick

    u32 bvlock;					// vlock 1, is set by vlock irq

	u32 isp_wdr;
	u32 isp_frame;
	u32 max_frame;
	u32 isp_dss;

	u32 of_count_per_sec;		// overflow count / sec
	u32 of_ovf;					// overflow
	u32 of_big;					// overflow
	u32 of_max_size;			// overflow size

	u32 video_default_ts;

	jpeg_param_t param;			// jpeg parameter info
	jpeg_image_buf_t imgbuf;	// jpeg stream buffer info
	jpeg_brmode_t brmode;
	jpeg_osd_t osd;				// jpeg osd info
} jpeg_enc_t;

void enx_jpeg_info_print(void);
u32 enx_jpeg_get_ts(void);
u32 enx_jpeg_get_height(void);
u32 enx_jpeg_get_width(void);

void jpeg_irq_handler(void *ctx);
UINT enx_jpeg_enc_start(u32 mode, u32 wdr, u32 dss);

void enx_jpeg_start(void);
void enx_jpeg_init(u32 raw_width, u32 raw_height);

void enx_jpeg_set_ts(u32 value);
void enx_jpeg_set_maxframe(u32 value);
void enx_jpeg_set_frame_size_rs(Resolution size);
void enx_jpeg_set_frame_rate_sf(SensorFps period);
void enx_jpeg_set_quantize(u32 qp);
void enx_jpeg_set_bit_rate(JpegConstantBitRate cbr);
void enx_jpeg_set_bit_rate_mode(u32 nBrmode);

extern jpeg_enc_t jpeg_enc;

#endif // _JPEG_H_
