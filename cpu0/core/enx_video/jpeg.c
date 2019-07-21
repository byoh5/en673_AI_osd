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
#include "dev_inline.h"
#include "enx_video.h"
#include "rtsp_cmd.h"
#include "jpeg.h"
#include "enx_os.h"
#include "user.h"
#include "msg.h"

#ifdef __ISP__
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h

#ifdef __JPEG__
// rtp_over_rtsp.c
extern void put_video_streams(UINT addr, UINT size, UINT flags, UINT video_type, UINT ts);

const BYTE qTB[100] = {
			192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,236,237,238,239,240,241,242,243,244,245,247,249,250,251,252,253,
			 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 95, 96, 97, 98, 99,100,101,102,103,104,105,106,107,108,109,
			 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 36, 50};
#define JPEG_Quantize(var)		jpeg_enc.param.qp_idx = var; \
								jpeg_enc.param.qp = qTB[var]; \
								JPG_QP0w((qTB[var]%64)); \
								JPG_QP1w((qTB[var]>>6));
#if (JPEG_PREVENT_OVF==1)
#define JPEG_BRMODE_SQRT_MIN_CAL(a) (a - (a / 40))
#define JPEG_BRMODE_SQRT_SHIFT		(7)
#endif

jpeg_enc_t jpeg_enc;

extern void jpeg_cbr_processing(UINT jpg_size);

extern void JPGCK_SET(void);
extern void JPG_DEFAULT_REG_SET(void);
extern void DS0_OFF(void);
extern void DS1_OFF(void);
extern void DS0_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);
extern void DS1_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);
/**============================================================================
	jpeg get functions
*==============================================================================*/
#if 1
void enx_jpeg_info_print(void)
{
	u32 i;
	jpeg_param_t *param = &(jpeg_enc.param);
	jpeg_image_buf_t *imgbuf = &(jpeg_enc.imgbuf);
	jpeg_brmode_t *brmode = &(jpeg_enc.brmode);

	printf(" ---		JPEG Information		---\r\n");
	printf("├Param Info\r\n");
	printf("│├Input source   : JPG_ISEL(%d)\r\n", JPG_ISELr);
	printf("│├Max Image Size : %d x %d\r\n", param->sen_width, param->sen_height);
	printf("│├Now Image Size : %d x %d\r\n", JPG_HWr, JPG_VWr);
	printf("│├Max JPEG Size  : %d Bytes\r\n", param->max_size == JPG_MAXr ? (int)param->max_size : -1);
	printf("│├QP             : %d / %d(ori:(%d/%d)/VBR:%d/CBR:%d)\r\n", param->qp_idx, param->qp, param->dynamic_qp, param->ori_qp, brmode->quantVBR, brmode->quantCBR);
	printf("│├Bitrate Mode   : %s\r\n", brmode->mode == e_brmQBR ? "QBR" : brmode->mode == e_brmVBR ? "VBR" : brmode->mode == e_brmCBR ? "CBR" : "?");
	printf("│├Bitrate        : %d bps / %d Kbps / %d Mbps\r\n", brmode->bitrate*8, (brmode->bitrate*8) >> 10, (brmode->bitrate*8) >> 20);
#if (JPEG_PREVENT_OVF==1)
	printf("│├DynamicBitrate : %d bps / %d Kbps / %d Mbps\r\n", brmode->dynamic_bitrate*8, (brmode->dynamic_bitrate*8) >> 10, (brmode->dynamic_bitrate*8) >> 20);
	printf("│├Sqrt Br Max/Min: R(%d~%d) D(%d~%d)\r\n", brmode->sqrt_rbr_max, brmode->sqrt_rbr_min, brmode->sqrt_dbr_max, brmode->sqrt_dbr_min);
#endif
	printf("│└Frame rate     : %d fps\r\n", jpeg_enc.max_frame / param->period);
	printf("├Buffer Info\r\n");
	printf("│├Buffer Count   : %d\r\n", imgbuf->index_lenth);
	for(i=0; i<imgbuf->index_lenth-1; i++)
	printf("│├imgbuf[%d].addr : 0x%08X ~ 0x%08X\r\n", i, imgbuf->buf[i].addr, imgbuf->buf[i].addr+JPEG_MAX_SIZE);
	printf("│└imgbuf[%d].addr : 0x%08X ~ 0x%08X\r\n", i, imgbuf->buf[i].addr, imgbuf->buf[i].addr+JPEG_MAX_SIZE);
	printf("└Total Size       : %d Bytes\r\n", JPEG_IMG_END - JPEG_IMG_BASE);
	printf(" --------------------------------------\r\n");
}
#endif

u32 enx_jpeg_get_ts(void)
{	// mjpeg ts = (90000 / 30) * mjpeg_enc.period
	return jpeg_enc.video_default_ts * jpeg_enc.param.period;
}

u32 enx_jpeg_get_height(void)
{
	return JPG_VWr;
}

u32 enx_jpeg_get_width(void)
{
	return JPG_HWr;
}

/**============================================================================
	jpeg image write done interrupt
*==============================================================================*/
void jpeg_irq_handler(void *ctx)
{
	static int firstskip = 0;
	UINT of_flag = 0;

#ifdef GPIO_JPEG_STATE
	GpioSetLo(GPIO_JPEG_STATE);
#endif

	if (firstskip == 0) { // Skip the first video frame.
		firstskip = 1;
	} else if (JPG_FSIZE_HOLDr <= jpeg_enc.param.max_size) {	// Under max grid size ?
		if (JPG_OVFr==0 && JPG_BIGr==0) {	// Not overflow ?
			BYTE *jpeg_addr = (BYTE *)jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].addr;
			UINT jpeg_size = jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].size = JPG_FSIZE_HOLDr;

#if (JPEG_SNAPSHOT==1)
			UINT i;
			if (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_IDE) {	// JPEG snapshot start check
				gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_PROC;
				// snapshot buffer를 설정한다.
				for (i = 0; i < jpeg_enc.imgbuf.index_lenth; i++) {
					jpeg_enc.imgbuf.buf[i].lock = 0;
				}
				jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].lock = 1;

				gptMsgShare.JPEG_STILL_ADDR = (UINT)jpeg_addr;
				gptMsgShare.JPEG_STILL_SIZE = jpeg_size;
				gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
			} else if (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_JTAG && gptMsgShare.JPEG_STILL_SIZE == 0) {
				// ECM에서 JPEG_STILL_FLAG를 9로 변경하고, JPEG_STILL_SIZE를 0으로 설정하면
				// snapshot buffer에 저장한다. Size를 필히 나중에 설정해야 한다.
				for (i = 0; i < jpeg_enc.imgbuf.index_lenth; i++) {
					jpeg_enc.imgbuf.buf[i].lock = 0;
				}
				jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].lock = 1;
				gptMsgShare.JPEG_STILL_ADDR = (UINT)jpeg_addr;
				gptMsgShare.JPEG_STILL_SIZE = jpeg_size;
			}

			if (jpeg_enc.param.period == 1 || !(gptMsgShare.VIDEO_TICK%jpeg_enc.param.period)) {
				put_video_streams((UINT)jpeg_addr, jpeg_size, 1, RTSP_MJPEG, enx_jpeg_get_ts());
			}

			for (i = 0; i < jpeg_enc.imgbuf.index_lenth; i++) {
				num_loop(jpeg_enc.imgbuf.index, jpeg_enc.imgbuf.index_lenth);
				if (jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].lock == 0) {
					break;
				}
			}
#else
			if ((jpeg_enc.param.period == 1) || !(gptMsgShare.VIDEO_TICK % jpeg_enc.param.period)) {
				put_video_streams((UINT)jpeg_addr, jpeg_size, 1, RTSP_MJPEG, enx_jpeg_get_ts());
			}

			num_loop(jpeg_enc.imgbuf.index, jpeg_enc.imgbuf.index_lenth);
#endif

			JPG_ADRw((jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].addr >> 4) << 1);
		} else {
			of_flag = 1;
		}
	} else {
		of_flag = 1;
	}

	UINT jpg_size = JPG_FSIZE_HOLDr;
	if (of_flag) {
//		printf("JPEG overflow!(flag:%d/%d)(%dbyte)\r\n", JPG_OVFr, JPG_BIGr, JPG_FSIZE_HOLDr);
		jpeg_enc.of_count_per_sec++;
		if (JPG_OVFr == 1) { jpeg_enc.of_ovf++; }
		if (JPG_BIGr == 1) { jpeg_enc.of_big++; }
		if (jpeg_enc.of_max_size < jpg_size) { jpeg_enc.of_max_size = jpg_size; }
	}

	if (jpeg_enc.brmode.mode != e_brmQBR) {
#if (JPEG_PREVENT_OVF==1)
		if (of_flag) {
			if (jpeg_enc.brmode.dynamic_bitrate > (1024 * 1024 / 8)) {
//				UINT old = jpeg_enc.brmode.dynamic_bitrate;
				jpeg_enc.brmode.dynamic_bitrate -= (1024 * 1024 / 8);
				jpeg_enc.brmode.sqrt_dbr_max = floorSqrt((jpeg_enc.brmode.dynamic_bitrate * jpeg_enc.param.period / jpeg_enc.max_frame)>>JPEG_BRMODE_SQRT_SHIFT);
				jpeg_enc.brmode.sqrt_dbr_min = JPEG_BRMODE_SQRT_MIN_CAL(jpeg_enc.brmode.sqrt_dbr_max);
//				printf("JPEG OVF - Dynamic bitrate(%d -> %d)\r\n", old, jpeg_enc.brmode.dynamic_bitrate);
			}
		}

		UINT jpg_sqrt = floorSqrt(jpg_size >> JPEG_BRMODE_SQRT_SHIFT); // Use 300 ticks.(>>7)
		if (jpg_sqrt > jpeg_enc.brmode.sqrt_dbr_max || jpg_sqrt < jpeg_enc.brmode.sqrt_dbr_min)
#endif
		{
			jpeg_cbr_processing(jpg_size);
		}

//		printf("Qidx(%2d) Size(%6d) %3d  Dbr(%3d~%3d)  Rbr(%3d~%3d)\r\n", jpeg_enc.brmode.quantCBR, jpg_size, jpg_sqrt,
//			jpeg_enc.brmode.sqrt_dbr_max, jpeg_enc.brmode.sqrt_dbr_min,
//			jpeg_enc.brmode.sqrt_rbr_max, jpeg_enc.brmode.sqrt_rbr_min);
	} else {
#if (JPEG_PREVENT_OVF==1)
		if (of_flag) {
			UINT old = jpeg_enc.param.dynamic_qp;
			jpeg_enc.param.dynamic_qp++;
			if(jpeg_enc.param.dynamic_qp > JPEG_Quantize_MAX) {
				jpeg_enc.param.dynamic_qp = JPEG_Quantize_MAX;
			}
			JPEG_Quantize(jpeg_enc.param.dynamic_qp);
			printf("JPEG OVF - Dynamic QP(%d -> %d)\r\n", old, jpeg_enc.param.dynamic_qp);
		}
#endif
	}

#ifdef __HW_OSD_JPEG__
#define OSD_JPEG_X 0
#define OSD_JPEG_Y 12
	u32 *buf_len = jpeg_enc.osd.buf_len;
	if (gptMsgShare.VIDEO_OSD & OSD_JPEG) {
		char buffer[16] = {0};
		buf_len[0] = sprintf(buffer, "JPEG(*%02d)(/%d)", JPG_QP0r, JPG_QP1r);		// len:13
		DispStr(buffer, OSD_JPEG_Y+0, OSD_JPEG_X, buf_len[0]);
		buf_len[1] = sprintf(buffer, "(%02d)%9d", jpeg_enc.param.qp_idx, jpg_size);	// len:13
		DispStr(buffer, OSD_JPEG_Y+1, OSD_JPEG_X, buf_len[1]);
	} else {
		if (buf_len[0]) {
			DispClr(OSD_JPEG_Y+0, OSD_JPEG_X, buf_len[0]);
			DispClr(OSD_JPEG_Y+1, OSD_JPEG_X, buf_len[1]);
			buf_len[0] = 0;
			buf_len[1] = 0;
		}
	}
#endif
	UNUSED(ctx);
}

UINT enx_jpeg_enc_start(u32 mode, u32 wdr, u32 dss)
{
	if (jpeg_enc.of_count_per_sec && (gptMsgShare.VIDEO_TICK % gptMsgShare.VIDEO_FPS == 0)) {
		printf("JPEG OVF - Drop:%d (f:%d/%d)(msize:%d)\r\n", jpeg_enc.of_count_per_sec, jpeg_enc.of_ovf, jpeg_enc.of_big, jpeg_enc.of_max_size);
		jpeg_enc.of_count_per_sec = 0;
		jpeg_enc.of_ovf = 0;
		jpeg_enc.of_big = 0;
		jpeg_enc.of_max_size = 0;
	}

	char *strModes[2] = {"noWDR", "WDR"};
	UINT change_flag = DEF_NO;
	//if (mode != jpeg_enc.isp_frame || wdr != jpeg_enc.isp_wdr) {
	if (mode != jpeg_enc.isp_frame || wdr != jpeg_enc.isp_wdr || dss != jpeg_enc.isp_dss) {
		change_flag = DEF_YES;
		jpeg_enc.isp_frame = mode;
		jpeg_enc.isp_wdr = wdr;
		jpeg_enc.isp_dss = dss;
		jpeg_enc.hold = 1;
		if (jpeg_enc.isp_wdr == 0) {
			jpeg_enc.max_frame = jpeg_enc.isp_frame;
			//jpeg_enc.video_default_ts = 90000 / jpeg_enc.max_frame;
			jpeg_enc.video_default_ts = (90000*(jpeg_enc.isp_dss+1)) / jpeg_enc.max_frame;
			if (jpeg_enc.max_frame == 60) {
				enx_jpeg_set_frame_rate_sf(e_s60f60);
			} else if (jpeg_enc.max_frame == 50) {
				enx_jpeg_set_frame_rate_sf(e_s50f50);
			} else if (jpeg_enc.max_frame == 30) {
				enx_jpeg_set_frame_rate_sf(e_s30f30);
			} else if (jpeg_enc.max_frame == 25) {
				enx_jpeg_set_frame_rate_sf(e_s25f25);
			}
		} else {
			if (jpeg_enc.isp_wdr != 1) {
				printf("WDR value?(%d)\r\n", jpeg_enc.isp_wdr);
				jpeg_enc.isp_wdr = 1;
			}
			jpeg_enc.max_frame = jpeg_enc.isp_frame / 2;				// isp_frame 60,50,30,25가 온다.
			jpeg_enc.video_default_ts = (90000 * 2) / jpeg_enc.isp_frame;
			if (jpeg_enc.max_frame == 30) {
				enx_jpeg_set_frame_rate_sf(e_s30f30);
			} else if (jpeg_enc.max_frame == 25) {
				enx_jpeg_set_frame_rate_sf(e_s25f25);
			} else if (jpeg_enc.max_frame == 15) {
				enx_jpeg_set_frame_rate_sf(e_s15f15);
			} else if (jpeg_enc.max_frame == 12) {
				enx_jpeg_set_frame_rate_sf(e_s25f25);	// 12.5 fps?
			}
		}
		printf("MJPEG rate change(%s): %d/%d/%d/%d\r\n", strModes[jpeg_enc.isp_wdr], jpeg_enc.isp_frame, jpeg_enc.max_frame, jpeg_enc.isp_wdr, jpeg_enc.video_default_ts, jpeg_enc.param.period);
	}

	if (jpeg_enc.hold) {
		if (jpeg_enc.hold == 1) {
			jpeg_enc.hold = 2;
		} else if (jpeg_enc.hold == 2) {
			jpeg_enc.hold = 0;
			JPGCK_SET();
		}
	} else if (jpeg_enc.enable) {
#ifdef GPIO_JPEG_STATE
		GpioSetHi(GPIO_JPEG_STATE);
#endif
		JPG_GOw(1);
		JPG_GOw(0);
	}

	return change_flag;
}

/**============================================================================
	jpeg init functions
*==============================================================================*/
static inline void jpeg_default_param(u32 raw_width, u32 raw_height)
{
	jpeg_enc.param.sen_width = raw_width;		// param.sen_width
	jpeg_enc.param.sen_height = raw_height;		// param.sen_height
	jpeg_enc.param.max_size = JPEG_MAX_SIZE;	// param.max_size
}

static inline void jpeg_default_buffer(void)
{
	jpeg_enc.imgbuf.index_lenth  = JPEG_DEF_BUF_CNT;
#if JPEG_STREAM
	jpeg_enc.imgbuf.index_lenth += JPEG_STREAM_BUF_CNT;
#endif

	UINT JPEG_ADDR = ((JPEG_IMG_BASE >> 4) << 4);
	UINT i;

	for (i = 0; i < jpeg_enc.imgbuf.index_lenth; i++) {
		jpeg_enc.imgbuf.buf[i].size = 0;
		jpeg_enc.imgbuf.buf[i].lock = 0;
		jpeg_enc.imgbuf.buf[i].addr = JPEG_ADDR;
		JPEG_ADDR += JPEG_ALIGN(jpeg_enc.param.max_size);
	}

#if JPEG_SNAPSHOT
	// JPEG snapshot buffer
	gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
	gptMsgShare.JPEG_STILL_SIZE = 0;
	gptMsgShare.JPEG_STILL_ADDR = 0;
	gptMsgShare.JPEG_STILL_REF = 0;
#endif

#if 0
	printf("jpeg_enc.imgbuf.buf[0].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[0].addr, (jpeg_enc.imgbuf.buf[0].addr>>4)<<1);
	printf("jpeg_enc.imgbuf.buf[1].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[1].addr, (jpeg_enc.imgbuf.buf[1].addr>>4)<<1);
#if JPEG_STREAM
	printf("jpeg_enc.imgbuf.buf[2].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[2].addr, (jpeg_enc.imgbuf.buf[2].addr>>4)<<1);
	printf("jpeg_enc.imgbuf.buf[3].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[3].addr, (jpeg_enc.imgbuf.buf[3].addr>>4)<<1);
	printf("jpeg_enc.imgbuf.buf[4].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[4].addr, (jpeg_enc.imgbuf.buf[4].addr>>4)<<1);
	printf("jpeg_enc.imgbuf.buf[5].addr : 0x%08X, ISP(0x%08X)\r\n", jpeg_enc.imgbuf.buf[5].addr, (jpeg_enc.imgbuf.buf[5].addr>>4)<<1);
#endif
#if JPEG_SNAPSHOT
	printf("gptMsgShare.JPEG_STILL_ADDR : 0x%08X\r\n", gptMsgShare.JPEG_STILL_ADDR);
#endif
	printf("real JPEG_ADDR_END(0x%08X), define JPEG_IMG_END(0x%08X)\r\n", JPEG_ADDR, JPEG_IMG_END);
#endif

	if (JPEG_ADDR > JPEG_IMG_END) {	// JPEG_IMG_END recalculation!
		enx_jpeg_info_print();
		while (1) {
			printf("%s(%d) Buffer overflow!!(pos:0x%08X,END:0x%08X)\r\n", __func__, __LINE__, JPEG_ADDR, JPEG_IMG_END);
#ifdef WDT_SYS
			WdtSysCountReset();
#endif
#ifdef WDT_CPU0
			WdtCpuCountReset(); // CPU0 WDT reset
#endif
			WaitXms(2000);
		}
	}
}

static inline void jpeg_capture_init(void)
{
	jpeg_enc.imgbuf.index = 0;

	JPG_ADRw((jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].addr >> 4) << 1);
//	printf("JPG_IMG_BASE :0x%08x, ISP_JPG_ADR :%08x real:%08x \r\n", JPEG_IMG_BASE, JPG_ADRr, (jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].addr>>4)<<1);
//	printf("ISP_JPG_ADR :%08x real:%08x\r\n", JPG_ADRr, (jpeg_enc.imgbuf.buf[jpeg_enc.imgbuf.index].addr>>4)<<1);

//	JPG_GO = 1;
//	enx_os_msleep(20);
//	JPG_GO = 1;
//	enx_os_msleep(20);
}

void enx_jpeg_start(void)
{
	JPGIRQ_ENw(1);	// JPEG done irq enable

	jpeg_capture_init();

	jpeg_enc.enable = 1;
	jpeg_enc.hold = 0;
}

void enx_jpeg_init(u32 raw_width, u32 raw_height)
{
	DmaMemSet_ip((BYTE *)&jpeg_enc, 0, sizeof(jpeg_enc));
	invalidate_dcache_range((uint)&jpeg_enc, ((uint)&jpeg_enc)+sizeof(sizeof(jpeg_enc)));

	/* default parameter setting */
	jpeg_default_param(raw_width, raw_height);
	JPG_DEFAULT_REG_SET();
	jpeg_default_buffer();
	
	JPG_HWw(raw_width);
	JPG_VWw(raw_height);
	
}

/**============================================================================
	jpeg set functions
*==============================================================================*/
void enx_jpeg_set_ts(u32 value)
{
	jpeg_enc.video_default_ts = value;
}

void enx_jpeg_set_maxframe(u32 value)
{
	jpeg_enc.max_frame = value;
#if (JPEG_PREVENT_OVF==1)
	jpeg_enc.param.dynamic_qp = jpeg_enc.param.ori_qp;
	jpeg_enc.brmode.dynamic_bitrate = jpeg_enc.brmode.bitrate;
	jpeg_enc.brmode.sqrt_rbr_max = floorSqrt((jpeg_enc.brmode.bitrate * jpeg_enc.param.period / jpeg_enc.max_frame)>>JPEG_BRMODE_SQRT_SHIFT);
	jpeg_enc.brmode.sqrt_dbr_max = jpeg_enc.brmode.sqrt_rbr_max;
	jpeg_enc.brmode.sqrt_rbr_min = JPEG_BRMODE_SQRT_MIN_CAL(jpeg_enc.brmode.sqrt_rbr_max);
	jpeg_enc.brmode.sqrt_dbr_min = jpeg_enc.brmode.sqrt_rbr_min;
#endif
}

static void jpeg_set_frame_size(u32 width, u32 height)
{
#define JPEG_DS_VAR	2 // HW_DS1 // 2

	if(width < 128)				width = 128;						// 가로길이 최소 128
	else if(width % 16 != 0)	width = ((width >> 4) + 1) << 4;	// 가로길이 16의 배수

	if(height < 128)			height = 128;						// 세로길이 최소 128
	else if(height % 8 != 0)	height = ((height >> 3) + 1) << 3;	// 세로길이 8의 배수

	if (HW_DS0 == JPEG_DS_VAR || HW_DS1 == JPEG_DS_VAR) {
		JPG_HWw(width);
		JPG_VWw(height);
		printf("JPEG : Frame Size : %d(%d), %d(%d)\r\n", width, JPG_HWr, height, JPG_VWr);
	} else {
		if ((jpeg_enc.param.sen_width != width) || (jpeg_enc.param.sen_height != height)) {
			printf("JPEG : Frame Size : Downscaler not assigned.\r\n");
			return;
		}
	}

	UINT sync_selection = 0, input_selection = 0, voffset_selection = 0;
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))	// Main ISP Input Image
	voffset_selection = 2;
	input_selection = 4;			// 4: Main ISP image
	sync_selection = 0;
#elif (IMAGE_INPUT_MODE==2)			// Second BT656/YC input Only
	voffset_selection = 0;
	input_selection = 6;			// 6: Second BT1120/BT656/YC input channel image
	sync_selection = 2;
#elif (IMAGE_INPUT_MODE==3)			// Main BT1120/YC input Only
	voffset_selection = 0;
	input_selection = 5;			// 5: Main BT1120/BT656/YC input channel image
	sync_selection = 1;
#endif
	if (jpeg_enc.param.sen_width == width && jpeg_enc.param.sen_height == height) {
		JPG_VOFSw(voffset_selection);
		JPG_ISELw(input_selection);
		if (HW_DS0 == JPEG_DS_VAR) {
			DS0_OFF();
		} else if (HW_DS1 == JPEG_DS_VAR) {
			DS1_OFF();
		}
	} else {
		if (HW_DS0 == JPEG_DS_VAR) {
			DS0_SET(sync_selection, voffset_selection, (jpeg_enc.param.sen_width * 64) / width, (jpeg_enc.param.sen_height * 64) / height, width, height);
			JPG_ISELw(0);	// 0: Down-scailer 0 image
		} else if (HW_DS1 == JPEG_DS_VAR) {
			DS1_SET(sync_selection, voffset_selection, (jpeg_enc.param.sen_width * 64) / width, (jpeg_enc.param.sen_height * 64) / height, width, height);
			JPG_ISELw(1);	// 1: Down-scailer 1 image
		} else {	// not use DS
			JPG_ISELw(input_selection);
		}
	}
}

void enx_jpeg_set_frame_size_rs(Resolution size)
{
	if (listResolution[size].rIndex == size) {
		jpeg_set_frame_size(listResolution[size].nWidth, listResolution[size].nHeight);
#if (JPEG_PREVENT_OVF==1)
		jpeg_enc.param.dynamic_qp = jpeg_enc.param.ori_qp;
		jpeg_enc.brmode.dynamic_bitrate = jpeg_enc.brmode.bitrate;
		jpeg_enc.brmode.sqrt_dbr_max = jpeg_enc.brmode.sqrt_rbr_max;
		jpeg_enc.brmode.sqrt_dbr_min = jpeg_enc.brmode.sqrt_rbr_min;
#endif
	} else {
		printf("%s Unknown Size Index(%d)\r\n", __func__, size);
	}
}

/*
	control frame rate
	@period : frame rate is determined by (MAX_FPS / period)
*/
void enx_jpeg_set_frame_rate_sf(SensorFps period)
{
	u32 value = e_isp30fps;
	switch (jpeg_enc.max_frame) {
		case 60:	value = e_isp60fps;	break;
		case 50:	value = e_isp50fps;	break;
		case 30:	value = e_isp30fps;	break;
		case 25:	value = e_isp25fps;	break;
		case 15:	value = e_isp15fps;	break;
		case 12:	value = e_isp12fps;	break;
		default:						break;
	}
	jpeg_enc.param.period = nFrameRateDiv[value][period];
#if (JPEG_PREVENT_OVF==1)
	jpeg_enc.param.dynamic_qp = jpeg_enc.param.ori_qp;
	jpeg_enc.brmode.dynamic_bitrate = jpeg_enc.brmode.bitrate;
	jpeg_enc.brmode.sqrt_rbr_max = floorSqrt((jpeg_enc.brmode.bitrate * jpeg_enc.param.period / jpeg_enc.max_frame)>>JPEG_BRMODE_SQRT_SHIFT);
	jpeg_enc.brmode.sqrt_dbr_max = jpeg_enc.brmode.sqrt_rbr_max;
	jpeg_enc.brmode.sqrt_rbr_min = JPEG_BRMODE_SQRT_MIN_CAL(jpeg_enc.brmode.sqrt_rbr_max);
	jpeg_enc.brmode.sqrt_dbr_min = jpeg_enc.brmode.sqrt_rbr_min;
#endif
}

void enx_jpeg_set_quantize(u32 qp)
{
	if(qp > JPEG_Quantize_MAX)	qp = JPEG_Quantize_MAX;
	jpeg_enc.brmode.quantCBR = qp;
	jpeg_enc.brmode.quantVBR = qp;
//	printf("JPEG: Quantize(%d)\r\n", qp);
	jpeg_enc.param.ori_qp = qp;
#if (JPEG_PREVENT_OVF==1)
	jpeg_enc.param.dynamic_qp = qp;
#endif
	JPEG_Quantize(qp);
}

void enx_jpeg_set_bit_rate(JpegConstantBitRate cbr)
{
	u32 value = 1;
	switch (cbr) {
		case e_jc50m:	value = 50;	break;
		case e_jc45m:	value = 45;	break;
		case e_jc40m:	value = 40;	break;
		case e_jc35m:	value = 35;	break;
		case e_jc30m:	value = 30;	break;
		case e_jc25m:	value = 25;	break;
		case e_jc20m:	value = 20;	break;
		case e_jc19m:	value = 19;	break;
		case e_jc18m:	value = 18;	break;
		case e_jc17m:	value = 17;	break;
		case e_jc16m:	value = 16;	break;
		case e_jc15m:	value = 15;	break;
		case e_jc14m:	value = 14;	break;
		case e_jc13m:	value = 13;	break;
		case e_jc12m:	value = 12;	break;
		case e_jc11m:	value = 11;	break;
		case e_jc10m:	value = 10;	break;
		case e_jc9m:	value = 9;	break;
		case e_jc8m:	value = 8;	break;
		case e_jc7m:	value = 7;	break;
		case e_jc6m:	value = 6;	break;
		case e_jc5m:	value = 5;	break;
		case e_jc4m:	value = 4;	break;
		case e_jc3m:	value = 3;	break;
		case e_jc2m:	value = 2;	break;
		case e_jc1m:	value = 1;	break;
		case e_jcEndorUnknown:
		default:		value = value;	break;
	}
//	printf("JPEG: BR(%dKbps)\r\n", value * 1000);
	jpeg_enc.brmode.mbps = value;
	jpeg_enc.brmode.bitrate = value * (1024 * 1024 / 8);
#if (JPEG_PREVENT_OVF==1)
	jpeg_enc.brmode.dynamic_bitrate = jpeg_enc.brmode.bitrate;
	jpeg_enc.brmode.sqrt_rbr_max = floorSqrt((jpeg_enc.brmode.bitrate * jpeg_enc.param.period / jpeg_enc.max_frame)>>JPEG_BRMODE_SQRT_SHIFT);
	jpeg_enc.brmode.sqrt_dbr_max = jpeg_enc.brmode.sqrt_rbr_max;
	jpeg_enc.brmode.sqrt_rbr_min = JPEG_BRMODE_SQRT_MIN_CAL(jpeg_enc.brmode.sqrt_rbr_max);
	jpeg_enc.brmode.sqrt_dbr_min = jpeg_enc.brmode.sqrt_rbr_min;
#endif
}

void enx_jpeg_set_bit_rate_mode(u32 nBrmode)
{
// BR_Nmode, BR_Vmode, BR_Cmode
//	printf("JPEG: BRmode(%s)\r\n", nBrmode == BR_Nmode ? "OFF" : nBrmode == BR_Vmode ? "VBR" : nBrmode == BR_Cmode ? "CBR" : "none");
	if (jpeg_enc.brmode.mode != e_brmQBR && nBrmode == e_brmQBR) {	// CBR, VBR에서 BR제어를 OFF 할 경우 기존의 QP로 변경
		enx_jpeg_set_quantize(jpeg_enc.param.ori_qp);
	}
	jpeg_enc.brmode.mode = nBrmode;
}

void jpeg_cbr_processing(UINT jpg_size)
{
#if (JPEG_PREVENT_OVF==1)
	UINT average = (UINT)(jpeg_enc.brmode.dynamic_bitrate * jpeg_enc.param.period / jpeg_enc.max_frame); 	//(Xmegabit/bite(8)/frame 수)
#else
	UINT average = (UINT)(jpeg_enc.brmode.bitrate * jpeg_enc.param.period / jpeg_enc.max_frame);	//(Xmegabit/bite(8)/frame 수)
#endif
	if (jpeg_enc.brmode.mode == e_brmCBR) {
		if (jpg_size > average) {
			if (jpeg_enc.brmode.quantCBR < JPEG_Quantize_MAX) {
				++(jpeg_enc.brmode.quantCBR);
			} else {
				jpeg_enc.brmode.quantCBR = JPEG_Quantize_MAX;
			}
		} else if (jpg_size < average) {
			if (jpeg_enc.brmode.quantCBR > JPEG_Quantize_MIN) {
				--(jpeg_enc.brmode.quantCBR);
			} else {
				jpeg_enc.brmode.quantCBR = JPEG_Quantize_MIN;
			}
		}
	} else if (jpeg_enc.brmode.mode == e_brmVBR) {
		if (jpg_size > average) {
			if (jpeg_enc.brmode.quantCBR < JPEG_Quantize_MAX) {
				++(jpeg_enc.brmode.quantCBR);
			} else {
				jpeg_enc.brmode.quantCBR = JPEG_Quantize_MAX;
			}
		} else if (jpg_size < average) {
			if (jpeg_enc.brmode.quantCBR > jpeg_enc.brmode.quantVBR) {
				--(jpeg_enc.brmode.quantCBR);
			} else {
				jpeg_enc.brmode.quantCBR = jpeg_enc.brmode.quantVBR;
			}
		}
	}
	JPEG_Quantize(jpeg_enc.brmode.quantCBR);
}
#else
void jpeg_irq_handler(void *ctx) { UNUSED(ctx); }
#endif
#else
void jpeg_irq_handler(void *ctx) { UNUSED(ctx); }
#endif
