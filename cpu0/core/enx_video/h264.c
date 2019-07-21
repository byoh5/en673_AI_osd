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

#include "msg.h"
#include "user.h"

#include "main.h"
#include "enx_os.h"

#ifdef __H264__
#ifndef __H264_MCH__

#include "h264.h"
#include "enx_video.h"

#ifdef __ISP__
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h
#endif

#include "bitratectl.h"

extern void H264_DEFAULT_REG_SET(void);
extern void BT0_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel);
extern void BT1_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel);
extern void DS0_OFF(void);
extern void DS1_OFF(void);
extern void DS0_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);
extern void DS1_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);
//******************************************************************************
// Init
//------------------------------------------------------------------------------
#if 1
void enx_h264_info_print(void)
{
	h264_image_buf_t *imgbuf;
	h264_param_t *param;
	h264_cbr_t *cbr;
	u32 ch, stp_size, buf_size, stm_size, tot_size, i;
	printf(" ---        H264 Information        ---\r\n");
	for (ch = 0; ch < h264_enc.num_ch; ch++) {
		imgbuf = &(h264_enc.chinfo[ch].imgbuf);
		param = &(h264_enc.chinfo[ch].param);
		cbr = &(h264_enc.chinfo[ch].cbr);

		stp_size = (imgbuf->stpsize.stp_size * 2) + (imgbuf->stpsize.stp_info_size * 2);
		buf_size = (imgbuf->bufsize.y_size * 4) + (imgbuf->bufsize.c_size * 4);
		stm_size = imgbuf->stream_end - imgbuf->stream_base;
		tot_size = stp_size + buf_size + stm_size;

		printf("Channel        : %d\r\n", ch);
		printf("├Param Info\r\n");
		printf("│├Image Size : %d x %d\r\n", param->width, param->height);
		printf("│├Stride Size: %d x %d\r\n", param->ystride, param->cstride);
		printf("│├ProfileIDC : %d(%s)\r\n", param->profile_idc, param->profile_idc == e_pBaseline ? "BASELINE" : param->profile_idc == e_pMain ? "MAIN" : "None");
		printf("│├LevelIDC   : %d\r\n", param->level_idc);
		printf("│├EntropyMode: %d(%s)\r\n", param->entropy, param->entropy == e_eCAVLC ? "CAVAL" : param->entropy == e_eCABAC ? "CABAC" : "None");
		if(param->entropy == e_eCABAC)
		printf("│├CabacIDC   : %d\r\n", param->cabac_idc);
		printf("│├I-Interval : %d\r\n", param->i_interval);
		printf("│├QP         : CUR(%d), PPS(%d)\r\n", param->cur_qp, param->pps_qp);
		printf("│├Bitrate Md : %s\r\n", cbr->mode == e_brmQBR ? "QBR" : cbr->mode == e_brmVBR ? "VBR" : cbr->mode == e_brmCBR ? "CBR" : cbr->mode == e_brmCVBR ? "CVBR" : "?");
		printf("│├Bitrate    : %d bps / %d Kbps / %d Mbps\r\n", cbr->bitrate*8, (cbr->bitrate*8) >> 10, (cbr->bitrate*8) >> 20);
		printf("│├Frame rate : %d fps\r\n", h264_enc.max_frame / param->period);
		printf("│└etc        : Srchmode(%d) MB(%d) cropbot(%d)\r\n", param->srchmode, param->num_mb, param->crop_b);
		printf("├Stream Info\r\n");
		printf("│├unit size  : STP(%dByte), INFO(%dByte), Count(%d)\r\n", imgbuf->stpsize.stp_size, imgbuf->stpsize.stp_info_size, imgbuf->stpsize.stp_count);
		for(i=0;i<imgbuf->stpsize.stp_count-1;i++)
		printf("│├STP%d       : STP(0x%08X~0x%08X), INFO(0x%08X~0x%08X)\r\n", i, imgbuf->stp[i].stp_addr, imgbuf->stp[i].stp_addr+imgbuf->stpsize.stp_size, imgbuf->stp[i].stp_info_addr, imgbuf->stp[i].stp_info_addr+imgbuf->stpsize.stp_info_size);
		printf("│├STP%d       : STP(0x%08X~0x%08X), INFO(0x%08X~0x%08X)\r\n", imgbuf->stpsize.stp_count-1, imgbuf->stp[imgbuf->stpsize.stp_count-1].stp_addr, imgbuf->stp[imgbuf->stpsize.stp_count-1].stp_addr+imgbuf->stpsize.stp_size, imgbuf->stp[imgbuf->stpsize.stp_count-1].stp_info_addr, imgbuf->stp[imgbuf->stpsize.stp_count-1].stp_info_addr+imgbuf->stpsize.stp_info_size);
		printf("│└SIZE       : %d Bytes\r\n", stp_size);
		printf("├Buffer Info\r\n");
		printf("│├unit size  : Y(%dByte), C(%dByte)\r\n", imgbuf->bufsize.y_size, imgbuf->bufsize.c_size);
		printf("│├CUR0       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", imgbuf->cur[0].y_addr, imgbuf->cur[0].y_addr+imgbuf->bufsize.y_size, imgbuf->cur[0].c_addr, imgbuf->cur[0].c_addr+imgbuf->bufsize.c_size);
		printf("│├CUR1       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", imgbuf->cur[1].y_addr, imgbuf->cur[1].y_addr+imgbuf->bufsize.y_size, imgbuf->cur[1].c_addr, imgbuf->cur[1].c_addr+imgbuf->bufsize.c_size);
		printf("│├REF0       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", imgbuf->ref[0].y_addr, imgbuf->ref[0].y_addr+imgbuf->bufsize.y_size, imgbuf->ref[0].c_addr, imgbuf->ref[0].c_addr+imgbuf->bufsize.c_size);
		printf("│├REF1       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", imgbuf->ref[1].y_addr, imgbuf->ref[1].y_addr+imgbuf->bufsize.y_size, imgbuf->ref[1].c_addr, imgbuf->ref[1].c_addr+imgbuf->bufsize.c_size);
		printf("│└SIZE       : %d Bytes\r\n", buf_size);
		printf("├Stream Buffer\r\n");
		printf("│├BASE       : 0x%08X ~ 0x%08X\r\n", imgbuf->stream_base, imgbuf->stream_end);
		printf("│└SIZE       : %d Bytes\r\n", stm_size);
		printf("└Total Size     : %d Bytes\r\n", tot_size);
	}
	printf(" --------------------------------------\r\n");
}
#endif

static void h264_default_param(u32 raw_img_width, u32 raw_img_height)
{
	u32 ch;

	// Sensor Size Set
#ifdef __ISP__
	MYCW_HWOw(raw_img_width+8);
#endif
	h264_enc.sen_height = raw_img_height;
	h264_enc.sen_width = raw_img_width;

	for (ch = 0; ch < h264_enc.num_ch; ch++) {
		h264_enc.chinfo[ch].imgbuf.pre_idxnum1 = 1;

		h264_param_t *param = &(h264_enc.chinfo[ch].param);
		param->level_idc = LEVEL_IDC;
		param->cabac_idc = 0;
		param->width = h264_enc.sen_width;
		param->height = h264_enc.sen_height;
		param->hscale = (h264_enc.sen_width * 64) / h264_enc.sen_width;
		param->vscale = (h264_enc.sen_height * 64) / h264_enc.sen_height;
		param->num_mb = ((h264_enc.sen_width + 15) >> 4) * ((h264_enc.sen_height + 15) >> 4);
		param->crop_b = (param->height&0xF) ? 1 : 0;
		param->srchmode = (param->height >= 720) ? 2 : 1;
		param->ystride = ((param->width + 15) >> 4) << 4;
		param->cstride = param->ystride;
	}
}

static void h264_default_register(void)
{
	H264_INTRVC 	= 1;			   // 0 : cb,cr seperate, 1 : cb,cr interleave
	H264_AXIINFO1_T	= (0xF<<4) | (0xF<<0);
	H264_AXIINFO2_T	= (0<<7) | (1<<6) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | (1<<0);
//	H264_OPWAIT_T	= (255<<16) | (255<<8) | (0<<0);
//	H264_OPWAIT_T	= (128<<16) | (128<<8) | (0<<0);
//	H264_OPWAIT_T	= (63<<16) + (127<<8) + (31);
	H264_OPWAIT_T	= (0<<16) + (0<<8) + (0);

	H264_DEFAULT_REG_SET();
}

void h264_default_buffer(u32 h264_buf_addr, u32 h264_ch1_size, u32 h264_ch2_size)
{
	UINT paddr = h264_buf_addr;
	UINT STRMSIZE[2];
	h264_channel_info *chinfo;
	h264_image_buf_t *imgbuf;
	h264_param_t *param;
	u32 ch;

	STRMSIZE[0] = h264_ch1_size;
	STRMSIZE[1] = h264_ch2_size;

	UINT horizontal_buffer_size;
	UINT vertical_buffer_size;
	UINT reconstruction_buffer_size;

	UINT bank1addr = 0x09000000;
	UINT bank2addr = 0x0a000000;
	UINT bank3addr = 0x0b000000;

	for (ch = 0; ch < h264_enc.num_ch; ch++) {
		chinfo = &(h264_enc.chinfo[ch]);
		imgbuf = &(chinfo->imgbuf);
		param = &(chinfo->param);

		horizontal_buffer_size = ((param->width + 32) >> 4) << 4;
		vertical_buffer_size = ((param->height + 32) >> 4) << 4;
		reconstruction_buffer_size = horizontal_buffer_size * vertical_buffer_size;

		param->ysize = H264_ALIGN(reconstruction_buffer_size);
		param->csize = H264_ALIGN(reconstruction_buffer_size >> 1);

		imgbuf->bufsize.y_size = param->ysize;
		imgbuf->bufsize.c_size = param->csize;

		imgbuf->stpsize.stp_size = param->csize + param->csize / 2;
//		imgbuf->stpsize.stp_info_size = (((((param->width + 15) >> 4) * ((param->height + 15) >> 4)) + 511) >> 9) << 9;
		imgbuf->stpsize.stp_info_size = 8 * 1024;
	}

	// RAW image buffer ===============================================

	h264_enc.chinfo[0].imgbuf.cur[0].y_addr = H264_ALIGN(bank1addr);
	h264_enc.chinfo[0].imgbuf.cur[1].y_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.cur[0].y_addr + h264_enc.chinfo[0].imgbuf.bufsize.y_size);

	h264_enc.chinfo[0].imgbuf.cur[0].c_addr = H264_ALIGN(bank3addr - h264_enc.chinfo[0].imgbuf.bufsize.c_size);
	h264_enc.chinfo[0].imgbuf.cur[1].c_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.cur[0].c_addr - h264_enc.chinfo[0].imgbuf.bufsize.c_size);

	h264_enc.chinfo[1].imgbuf.cur[0].c_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.cur[1].c_addr - h264_enc.chinfo[1].imgbuf.bufsize.c_size);
	h264_enc.chinfo[1].imgbuf.cur[1].c_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.cur[0].c_addr - h264_enc.chinfo[1].imgbuf.bufsize.c_size);

	h264_enc.chinfo[1].imgbuf.cur[0].y_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.cur[1].c_addr - h264_enc.chinfo[1].imgbuf.bufsize.y_size);
	h264_enc.chinfo[1].imgbuf.cur[1].y_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.cur[0].y_addr - h264_enc.chinfo[1].imgbuf.bufsize.y_size);

	// H.264 stream buffer ============================================

	h264_enc.chinfo[0].stream_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.cur[1].y_addr + h264_enc.chinfo[0].imgbuf.bufsize.y_size);
	h264_enc.chinfo[0].imgbuf.stream = h264_enc.chinfo[0].stream_addr;
	h264_enc.chinfo[0].imgbuf.stream_base = h264_enc.chinfo[0].imgbuf.stream;
	h264_enc.chinfo[0].imgbuf.stream_end = H264_ALIGN(bank2addr+0x500000);

	h264_enc.chinfo[1].stream_addr = H264_ALIGN(bank2addr+0x500000);
	h264_enc.chinfo[1].imgbuf.stream = h264_enc.chinfo[1].stream_addr;
	h264_enc.chinfo[1].imgbuf.stream_base = h264_enc.chinfo[1].imgbuf.stream;
	h264_enc.chinfo[1].imgbuf.stream_end = H264_ALIGN(h264_enc.chinfo[1].imgbuf.cur[1].y_addr);

	// H.264 internal buffer ============================================

	h264_enc.chinfo[0].imgbuf.stpsize.stp_count = MAX_STP_BUF;
	h264_enc.chinfo[1].imgbuf.stpsize.stp_count = MAX_STP_BUF;

	h264_enc.chinfo[0].imgbuf.stp[0].stp_info_addr = H264_ALIGN(bank3addr);
	h264_enc.chinfo[0].imgbuf.stp[1].stp_info_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.stp[0].stp_info_addr + h264_enc.chinfo[0].imgbuf.stpsize.stp_info_size);
	h264_enc.chinfo[0].imgbuf.stp[0].stp_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.stp[1].stp_info_addr + h264_enc.chinfo[0].imgbuf.stpsize.stp_info_size);
	h264_enc.chinfo[0].imgbuf.stp[1].stp_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.stp[0].stp_addr + h264_enc.chinfo[0].imgbuf.stpsize.stp_size);

	h264_enc.chinfo[0].imgbuf.ref[0].y_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.stp[1].stp_addr + h264_enc.chinfo[0].imgbuf.stpsize.stp_size);
	h264_enc.chinfo[0].imgbuf.ref[0].c_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.ref[0].y_addr + h264_enc.chinfo[0].imgbuf.bufsize.y_size);
	h264_enc.chinfo[0].imgbuf.ref[1].y_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.ref[0].c_addr + h264_enc.chinfo[0].imgbuf.bufsize.c_size);
	h264_enc.chinfo[0].imgbuf.ref[1].c_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.ref[1].y_addr + h264_enc.chinfo[0].imgbuf.bufsize.y_size);

	h264_enc.chinfo[1].imgbuf.stp[0].stp_info_addr = H264_ALIGN(h264_enc.chinfo[0].imgbuf.ref[1].c_addr + h264_enc.chinfo[0].imgbuf.bufsize.c_size);
	h264_enc.chinfo[1].imgbuf.stp[1].stp_info_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.stp[0].stp_info_addr + h264_enc.chinfo[1].imgbuf.stpsize.stp_info_size);
	h264_enc.chinfo[1].imgbuf.stp[0].stp_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.stp[1].stp_info_addr + h264_enc.chinfo[1].imgbuf.stpsize.stp_info_size);
	h264_enc.chinfo[1].imgbuf.stp[1].stp_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.stp[0].stp_addr + h264_enc.chinfo[1].imgbuf.stpsize.stp_size);

	h264_enc.chinfo[1].imgbuf.ref[0].y_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.stp[1].stp_addr + h264_enc.chinfo[1].imgbuf.stpsize.stp_size);
	h264_enc.chinfo[1].imgbuf.ref[0].c_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.ref[0].y_addr + h264_enc.chinfo[1].imgbuf.bufsize.y_size);
	h264_enc.chinfo[1].imgbuf.ref[1].y_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.ref[0].c_addr + h264_enc.chinfo[1].imgbuf.bufsize.c_size);
	h264_enc.chinfo[1].imgbuf.ref[1].c_addr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.ref[1].y_addr + h264_enc.chinfo[1].imgbuf.bufsize.y_size);

	paddr = H264_ALIGN(h264_enc.chinfo[1].imgbuf.ref[1].c_addr + h264_enc.chinfo[1].imgbuf.bufsize.c_size);

	if (paddr > H264_STRM_END) { // H264_STRM_END recalculation!
		printf("%d : STPCNT(%d)\r\n", __LINE__, h264_enc.chinfo[0].imgbuf.stpsize.stp_count);
		enx_h264_info_print();
		while (1) {
			printf("%s(%d) Buffer overflow!!(pos:0x%08X,END:0x%08X)\r\n", __func__, __LINE__, paddr, H264_STRM_END);
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

#ifdef __H264_TEST__
void make_raw(UINT SIZE_W, UINT SIZE_H){
/*
	UINT i;
	for (i=0;i<1920*1080;i++) {
		*(CHAR*)(RAWY0_IMG_1080BASE+i) = 0xff;
	}
	for (i=0;i<1920*1080;i++) {
		*(CHAR*)(RAWY1_IMG_1080BASE+i) = 0x00;
	}

	for (i=0;i<1920*1080/2;i++) {
		*(CHAR*)(RAWC0_IMG_1080BASE+i) = 0x00;
	}
	for (i=0;i<1920*1080/2;i++) {
		*(CHAR*)(RAWC1_IMG_1080BASE+i) = 0xff;
	}
*/
	UINT x,y;

#define __AAA 30

	BYTE *pY, *pCb, *pCr;
	pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].y_addr;
	pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[0].c_addr;
	pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[0].c_addr+1);
	for (y = 0; y < SIZE_H; y++) {
		for (x = 0; x < SIZE_W; x++) {
			if (x%__AAA==0||y%__AAA==0)			*pY = 16;
			else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 81;
			else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
			else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
			else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 55;
			else								*pY = 235;
			pY++;
		}
	}
	for (y = 0; y < SIZE_H / 2; y++) {
		for (x = 0; x < SIZE_W / 2; x++) {
			if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
			else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 90;
			else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
			else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
			else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 55;
			else								*pCb = 128;
			if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
			else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 240;
			else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
			else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 110;
			else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
			else								*pCr = 128;
			pCb+=2;
			pCr+=2;
		}
	}
	pY = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].y_addr;
	pCb = (BYTE *)h264_enc.chinfo[0].imgbuf.cur[1].c_addr;
	pCr = (BYTE *)(h264_enc.chinfo[0].imgbuf.cur[1].c_addr+1);
	for (y = 0; y < SIZE_H; y++) {
		for (x = 0; x < SIZE_W; x++) {
			if (x%__AAA==0||y%__AAA==0)			*pY = 16;
			else if (x<SIZE_W/2 && y<SIZE_H/2)	*pY = 55;
			else if (x>SIZE_W/2 && y<SIZE_H/2)	*pY = 145;
			else if (x<SIZE_W/2 && y>SIZE_H/2)	*pY = 41;
			else if (x>SIZE_W/2 && y>SIZE_H/2)	*pY = 81;
			else								*pY = 235;
			pY++;
		}
	}
	for (y = 0; y < SIZE_H / 2; y++) {
		for (x = 0; x < SIZE_W / 2; x++) {
			if (x%__AAA==0||y%__AAA==0)			*pCb = 128;
			else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCb = 55;
			else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCb = 54;
			else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCb = 240;
			else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCb = 90;
			else								*pCb = 128;
			if (x%__AAA==0||y%__AAA==0)			*pCr = 128;
			else if (x<SIZE_W/4 && y<SIZE_H/4)	*pCr = 110;
			else if (x>SIZE_W/4 && y<SIZE_H/4)	*pCr = 34;
			else if (x<SIZE_W/4 && y>SIZE_H/4)	*pCr = 240;
			else if (x>SIZE_W/4 && y>SIZE_H/4)	*pCr = 55;
			else								*pCr = 128;
			pCb+=2;
			pCr+=2;
		}
	}
}
#endif

void enx_h264_on(void)
{
	SYS_HCK_PD = 0;
	SYS_HCK_SEL = 0;	// 0:PLL0 1:PLL1 select
	SYS_HCK_PD = 1;
	SYS_H264_RST = 1;

#ifdef __H264_2CH__
	enx_h264_init(2, SENSOR_SIZE_W, SENSOR_SIZE_H);
#else
	enx_h264_init(1, SENSOR_SIZE_W, SENSOR_SIZE_H);
#endif

	enx_h264_set_maxframe(IMG_MAX_FPS);
	enx_h264_set_ts(90000/IMG_MAX_FPS);
#ifdef __DEVICE_SD__
	enx_h264_set_sdsave(DEF_ON);
#else
	enx_h264_set_sdsave(DEF_OFF);
#endif

// CH.0 Setting
	enx_h264_set_profile(IMG_H264_PROFILE, 0);
	enx_h264_set_quantize(IMG_H264_QULITY, 0);		// param(cur_qp, pps_qp)
	enx_h264_set_idrframe_period(IMG_H264_IDR, 0);	// param(i_interval, max_frame)
	enx_h264_set_frame_rate_sf(IMG_H264_FPS, 0);	// param(period)
	enx_h264_set_frame_size_rs(IMG_H264_RES, 0);	// param(width, height, hscale, vscale, num_mb, crop+b, srchmode)
	enx_h264_set_bit_rate_mode(IMG_BR_H, 0);		// br(mode)
	enx_h264_set_bit_rate(IMG_BR_HR, 0);			// br(bitrate)

// CH.1 Setting
#ifdef __H264_2CH__
	enx_h264_set_profile(IMG_H264_2CH_PROFILE, 1);
	enx_h264_set_quantize(IMG_H264_2CH_QULITY, 1);		// h.264 qp level 0~51
	enx_h264_set_idrframe_period(IMG_H264_2CH_IDR, 1);	// I frame period
	enx_h264_set_frame_rate_sf(IMG_H264_2CH_FPS, 1);	// 30fps/value = out fps
	enx_h264_set_frame_size_rs(IMG_H264_2CH_RES, 1);
	enx_h264_set_bit_rate_mode(IMG_BR_H2, 1);
	enx_h264_set_bit_rate(IMG_BR_H2R, 1);
#endif

	h264_default_buffer(H264_STRM_BASE, H264_STRM_SIZE_1CH, H264_STRM_SIZE_2CH);
// 1ch only
	H264_SBUFSIZE = h264_enc.chinfo[0].imgbuf.stpsize.stp_size;

// end
	enx_h264_start();
}

/**
	takes 200ms
	prepare image capute for encoding
*/
void enx_h264_image_capture_ready(void)
{
	UINT tick_cnt = 1;
	volatile UINT tick_temp = 0;

	while (!gptMsgShare.VIDEO_TICK) { // 0 ~ 33.3ms
#ifdef __WDT__
		enx_wdt_reset();
#endif
		WaitXus(1);
		if (++tick_cnt % 1000000 == 0) { // Check 1sec
			printf("Video Input not ready!(%d)\r\n", __LINE__);
			if (tick_cnt >= 5000000) {
				return;
			}
		}
	}
	tick_temp = gptMsgShare.VIDEO_TICK;

	tick_cnt = 1;
	enx_h264_raw_update(0, 1);
	if (h264_enc.num_ch == 2) {
		enx_h264_raw_update(1, 1);
	}
	while (tick_temp == gptMsgShare.VIDEO_TICK) { // 16.6ms or 33.3ms
#ifdef __WDT__
		enx_wdt_reset();
#endif
		WaitXus(1);
		if (++tick_cnt % 1000000 == 0) { // Check 1sec
			printf("Video Input not ready!(%d)\r\n", __LINE__);
			if (tick_cnt >= 5000000) {
				return;
			}
		}
	}
	tick_temp = gptMsgShare.VIDEO_TICK;

	tick_cnt = 0;
	enx_h264_raw_update(0, 1);
	if (h264_enc.num_ch == 2) {
		enx_h264_raw_update(1, 1);
	}
	while (tick_temp == gptMsgShare.VIDEO_TICK) { // 16.6ms or 33.3ms
#ifdef __WDT__
		enx_wdt_reset();
#endif
		WaitXus(1);
		if (++tick_cnt % 1000000 == 0) { // Check 1sec
			printf("Video Input not ready!(%d)\r\n", __LINE__);
			if (tick_cnt >= 5000000) {
				return;
			}
		}
	}

	int ch;
	for(ch=0;ch<(int)h264_enc.num_ch;ch++)
	{
		//int ibps, int ifps, int iq, int igop, int macro_blc_w, int macro_blc_h
		bitctl_init_seq(ch);
	}

	printf("H.264: Ready\r\n");
	h264_enc.vlock_fisrt = VLOCK_GO;
}

void enx_h264_start(void)
{
	IntAdd(IRQ_H264, (void*)enx_h264_irq_handler, 0);
	IrqEnable(IRQ_H264);

	gptMsgShare.VIDEO_TICK = 0;

	h264_enc.enable = 1;
	h264_enc.vlock_fisrt = VLOCK_NONE;

#ifdef __H264_TEST__
	h264_param_t *param = &(h264_enc.chinfo[0].param);
	make_raw(param->width, param->height);
#endif
}

/**
    init h264
    @h264_buf_addr : h264 buf addr that h.264 code use internally
    @raw_img_width : raw image width
    @raw_img_height : raw image height
    @period        : frame rate is determined by 30/period
*/
void enx_h264_init(u32 num_ch, u32 raw_img_width, u32 raw_img_height)
{
	memset(&h264_enc, 0, sizeof(h264_enc));

	h264_enc.num_ch = num_ch;

	h264_default_param(raw_img_width, raw_img_height);	// codec default paramer
	h264_default_register();	// codec default register
}

//******************************************************************************
// Get
//------------------------------------------------------------------------------

//******************************************************************************
// Set function
//------------------------------------------------------------------------------
void enx_h264_set_roi_0(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale)
{
	H264_ROI_EN_0	= 1;
	H264_ROI_Q_0	= qscale;
	H264_ROI_POS_0	= x_start<<24 | x_end<<16 | y_start<<8 | y_end;
}

void enx_h264_set_roi_1(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale)
{
	H264_ROI_EN_1	= 1;
	H264_ROI_Q_1	= qscale;
	H264_ROI_POS_1	= x_start<<24 | x_end<<16 | y_start<<8 | y_end;
}

void enx_h264_set_roi_2(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale)
{
	H264_ROI_EN_2	= 1;
	H264_ROI_Q_2	= qscale;
	H264_ROI_POS_2	= x_start<<24 | x_end<<16 | y_start<<8 | y_end;
}

void enx_h264_set_roi_3(BYTE x_start, BYTE x_end, BYTE y_start, BYTE y_end, int qscale)
{
	H264_ROI_EN_3	= 1;
	H264_ROI_Q_3	= qscale;
	H264_ROI_POS_3	= x_start<<24 | x_end<<16 | y_start<<8 | y_end;
}

void enx_h264_set_roi_0_disable(void)
{
	H264_ROI_EN_0	= 0;
}

void enx_h264_set_roi_1_disable(void)
{
	H264_ROI_EN_1	= 0;
}

void enx_h264_set_roi_2_disable(void)
{
	H264_ROI_EN_2	= 0;
}

void enx_h264_set_roi_3_disable(void)
{
	H264_ROI_EN_3	= 0;
}

void enx_h264_set_sdsave(u32 value)
{
	h264_enc.avi_save = value;
	if(h264_enc.avi_save == DEF_ON)
		h264_enc.avi_align_size = 0;
	else
		h264_enc.avi_align_size = 0;
}

int enx_h264_get_sdsave(void)
{
	return h264_enc.avi_save;
}

void enx_h264_set_maxframe(u32 value)
{
	H264_IRQ_LOCK();
	h264_enc.max_frame = value;
	H264_IRQ_UNLOCK();
}

int enx_h264_get_maxframe(void)
{
	return h264_enc.max_frame;
}

void enx_h264_set_ts(u32 value)
{
	H264_IRQ_LOCK();
	h264_enc.video_default_ts = value;
	H264_IRQ_UNLOCK();
}

UINT h264_get_height(int ch)
{
	return h264_enc.chinfo[ch].param.height;
}

UINT h264_get_width(int ch)
{
	return h264_enc.chinfo[ch].param.width;
}

void enx_h264_set_profile(H264ProfileMode profilemode, int ch)
{
	H264_IRQ_LOCK();
	switch (profilemode) {
		case e_pmBaseline:
			h264_enc.chinfo[ch].param.profile_idc = e_pBaseline;
			h264_enc.chinfo[ch].param.entropy = e_eCAVLC;
			h264_enc.chinfo[ch].param.cabac_idc = 0;
			break;
		case e_pmMainCB:
			h264_enc.chinfo[ch].param.profile_idc = e_pMain;
			h264_enc.chinfo[ch].param.entropy = e_eCABAC;
			h264_enc.chinfo[ch].param.cabac_idc = 0;
			break;
		case e_pmEndorUnknown:
		default:
			printf("%s(%d) : Error profile(%d)!\r\n", __func__, __LINE__, profilemode);
			break;
	}

	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)
	enx_h264_set_quantize(h264_enc.chinfo[ch].quant.qscale, ch); //set default Q
	
	bitctl_init_seq(ch);

	H264_IRQ_UNLOCK();
}

void enx_h264_set_idrframe_period(int period, int ch)
{	// set h.246 IDR Frame period
	if (period < H264_Idrframe_MIN)			period = H264_Idrframe_MIN;
	else if (period > H264_Idrframe_MAX)	period = H264_Idrframe_MAX;
//	printf("H264[%d]: IDR Frame : %d\r\n", ch, period);
	H264_IRQ_LOCK();
	h264_enc.chinfo[ch].param.i_interval = period;
	h264_enc.chinfo[ch].param.max_frame = period * 16;
	H264_IRQ_UNLOCK();

	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)	
	enx_h264_set_quantize(h264_enc.chinfo[ch].quant.qscale, ch); //set default Q
	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

u32 enx_h264_get_idr_interval(int ch)
{
	return h264_enc.chinfo[ch].param.i_interval;
}

void enx_h264_set_quantize(int quantize, int ch)
{	// quantize(52) : 0 ~ 51
	if (quantize < H264_Quantize_MIN)		quantize = H264_Quantize_MIN;
	else if (quantize > H264_Quantize_MAX)	quantize = H264_Quantize_MAX;
//	printf("H264[%d]: Quality : %d\r\n", ch, quantize);
	H264_IRQ_LOCK();
	H264_Quantize(ch, quantize);
	H264_Default_Quantize(ch, quantize);
	H264_IRQ_UNLOCK();
}

int enx_h264_get_quantize(int ch)
{
	return h264_enc.chinfo[ch].param.cur_qp;
}

int enx_h264_get_default_quantize(int ch)
{
	return h264_enc.chinfo[ch].quant.qscale;
}

void h264_set_frame_size(UINT width, UINT height, int ch)
{
	UINT old_width, old_height;
	old_width = width;
	old_height = height;

	// H264_IHSIZE는 16의 배수여야 한다.(기준 width) => H264_YSTRIDEX에 의해 32배수
	// H264_IVSIZE는 8의 배수여야 한다.(기준 height)
	// H264_YSTRIDEX가 32의 배수여야 한다.(기준 width)
	// H264_CSTRIDEX가 32의 배수여야 한다.(기준 width)
	if(width < 128) 			width = 128;						// 가로길이 최소 128
	else if(width % 32 != 0)  	width = (( width >> 5) + 1) << 5;	// 가로길이 32의 배수

	if(height < 104) 			height = 104;						// 세로길이 최소 104
	else if(height % 8 != 0)	height = ((height >> 3) + 1) << 3;	// 세로길이 8의 배수

	if ((HW_DS0 == ch || HW_DS1 == ch)) {
		printf("H264[%d] : Frame Size : %d(%d), %d(%d)\r\n", ch, width, old_width, height, old_height);
	} else {
		if ((h264_enc.sen_width != width) || (h264_enc.sen_height != height)) {
			printf("H264[%d] : Frame Size : Downscaler not assigned.\r\n", ch);
			return;
		}
	}

	H264_IRQ_LOCK();

	h264_param_t *param = &(h264_enc.chinfo[ch].param);
	param->width = width;
	param->height = height;
	param->hscale = (h264_enc.sen_width * 64) / width;
	param->vscale = (h264_enc.sen_height * 64) / height;
	param->num_mb = ((width + 15) >> 4) * ((height + 15) >> 4);
	param->crop_b = (param->height&0xF) ? 1 : 0;
	param->srchmode = (param->height >= 720) ? 2 : 1;

	param->ystride = ((param->width + 15) >> 4) << 4;
	param->cstride = param->ystride;

#ifdef __ISP__
	UINT sync_selection = 0, input_selection = 0, voffset_selection = 0;
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))	// Main ISP Input Image
	voffset_selection = 2;
	input_selection = 6;			// 6: Main ISP Image input
	sync_selection = 0;
#elif (IMAGE_INPUT_MODE==2)			// Second BT656/YC input Only
	input_selection = 5;			// 5: Second BT656/YC input image (Use Font/BOX OSD)
	sync_selection = 2;
#elif (IMAGE_INPUT_MODE==3)			// Main BT1120/YC input Only
	input_selection = 4;			// 4: Main BT1120/YC input image (Use Font/BOX OSD)
	sync_selection = 1;
#endif
	if (ch == 0) {
		if (h264_enc.sen_width == param->width && h264_enc.sen_height == param->height) {
			BT1_SET(param->width, input_selection, sync_selection);
			if (HW_DS0 == 0) {
				DS0_OFF();
			} else if (HW_DS1 == 0) {
				DS1_OFF();
			}
		} else {
			if (HW_DS0 == 0) {
				DS0_SET(sync_selection, voffset_selection, param->hscale, param->vscale, param->width, param->height);
				BT1_SET(param->width, 0, sync_selection);
			} else if (HW_DS1 == 0) {
				DS1_SET(sync_selection, voffset_selection, param->hscale, param->vscale, param->width, param->height);
				BT1_SET(param->width, 1, sync_selection);
			}
		}
	}
#if (IMAGE_INPUT_MODE!=1)
	else if (ch == 1) { // 2ch mode(sub)
		if (h264_enc.sen_width == param->width && h264_enc.sen_height == param->height) {
			BT0_SET(param->width, input_selection, sync_selection);
			if (HW_DS0 == 1) {
				DS0_OFF();
			} else if (HW_DS1 == 1) {
				DS1_OFF();
			}
		} else {
			if (HW_DS0 == 1) {
				DS0_SET(sync_selection, voffset_selection, param->hscale, param->vscale, param->width, param->height);
				BT0_SET(param->width, 0, sync_selection);
			} else if(HW_DS1 == 1) {
				DS1_SET(sync_selection, voffset_selection, param->hscale, param->vscale, param->width, param->height);
				BT0_SET(param->width, 1, sync_selection);
			}
		}
	}
#endif
	else {
		printf("%s(%d) : ch(%d) size change(%d, %d)???\r\n", __func__, __LINE__, ch, width, height);
	}
#endif

	H264_IRQ_UNLOCK();
}

void enx_h264_set_frame_size_rs(Resolution size, int ch)
{
	if (listResolution[size].rIndex == size) {
		h264_set_frame_size(listResolution[size].nWidth, listResolution[size].nHeight, ch);
	} else {
		printf("%s Unknown Size Index(%d) Ch(%d)\r\n", __func__, size, ch);
	}

	//int ibps, int ifps, int iq, int igop, int macro_blc_w, int macro_blc_h
	enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)

	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

/*
	control frame rate
	@period : frame rate is determined by 30/period
*/
#if 0
static void h264_set_frame_rate(u32 period, u32 ch)
{
//	printf("H264[%d]: Frame Rate : %dfps\r\n", ch, h264_enc.max_frame / period);
	H264_IRQ_LOCK();
	h264_enc.chinfo[ch].param.period = period;
	H264_IRQ_UNLOCK();
}
#endif

void enx_h264_set_frame_rate_sf(SensorFps period, u32 ch)
{
	u32 value = e_isp30fps;
	switch (h264_enc.max_frame) {
		case 60:	value = e_isp60fps;	break;
		case 50:	value = e_isp50fps;	break;
		case 30:	value = e_isp30fps;	break;
		case 25:	value = e_isp25fps;	break;
		case 15:	value = e_isp15fps;	break;
		case 12:	value = e_isp12fps;	break;
		default:						break;
	}
	H264_IRQ_LOCK();
	h264_enc.chinfo[ch].param.period = nFrameRateDiv[value][period];
	H264_IRQ_UNLOCK();

	
	enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)

	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

int h264_get_period(u32 ch)
{
	return h264_enc.chinfo[ch].param.period;
}

void enx_h264_set_bit_rate(H264ConstantBitRate cbr, int ch)
{
	u32 value = 20;
	switch (cbr) {
		case e_hc20m:		value = 200;	break;
		case e_hc19m:		value = 190;	break;
		case e_hc18m:		value = 180;	break;
		case e_hc17m:		value = 170;	break;
		case e_hc16m:		value = 160;	break;
		case e_hc15m:		value = 150;	break;
		case e_hc14m:		value = 140;	break;
		case e_hc13m:		value = 130;	break;
		case e_hc12m:		value = 120;	break;
		case e_hc11m:		value = 110;	break;
		case e_hc10m:		value = 100;	break;
		case e_hc9m:		value = 90;		break;
		case e_hc8m:		value = 80;		break;
		case e_hc7m:		value = 70;		break;
		case e_hc6m:		value = 60;		break;
		case e_hc5m:		value = 50;		break;
		case e_hc4m:		value = 40;		break;
		case e_hc3m:		value = 30;		break;
		case e_hc2m:		value = 20;		break;
		case e_hc1m:		value = 10;		break;
		case e_hc512k:		value = 5;		break;
		case e_hc256k:		value = 2;		break;
		case e_hc128k:		value = 1;		break;
		case e_hcEndorUnknown:
		default:			value = value;	break;
	}
//	printf("H264[%d]: BR : %dKbps\r\n", ch, value * 100);
	H264_IRQ_LOCK();
	h264_enc.chinfo[ch].cbr.bitrate = value * 1024 * 1024 / 80;
	H264_IRQ_UNLOCK();

	//int ibps, int ifps, int iq, int igop, int macro_blc_w, int macro_blc_h
	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)	
	enx_h264_set_quantize(h264_enc.chinfo[ch].quant.qscale, ch); //set default Q
	
	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

int enx_h264_get_bit_rate(int ch)
{
	return h264_enc.chinfo[ch].cbr.bitrate;
}

void enx_h264_set_bit_rate_mode(u32 nBrmode, int ch)
{
// BR_Nmode, BR_Vmode, BR_Cmode
//	printf("H264[%d]: BRmode : %s\r\n", ch, nBrmode == BR_Nmode ? "OFF" : nBrmode == BR_Vmode ? "VBR" : nBrmode == BR_Cmode ? "CBR" : "none");
	//if(h264_enc.chinfo[ch].cbr.mode != e_brmQBR && nBrmode == e_brmQBR) { // CBR, VBR에서 BR제어를 OFF 할 경우 기존의 QP로 변경
	//	enx_h264_set_quantize(h264_enc.chinfo[ch].quant.qscale, ch);
	//}
	H264_IRQ_LOCK();
	h264_enc.chinfo[ch].cbr.mode = nBrmode;
	H264_IRQ_UNLOCK();

	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)	
	enx_h264_set_quantize(h264_enc.chinfo[ch].quant.qscale, ch); //set default Q
	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

int enx_h264_get_bit_rate_mode(int ch)
{
	return h264_enc.chinfo[ch].cbr.mode;
}

int enx_h264_get_stp_size(int ch)
{
	return h264_enc.chinfo[ch].imgbuf.stpsize.stp_size;
}

int enx_h264_get_slice_type(int ch)
{
	return h264_enc.chinfo[ch].param.vlc_mpframe;
}

u32 enx_h264_get_prebits(void)
{
	return (H264_PREBITS >> 3);
}
#endif
#endif
