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
#include <time.h>

#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h

#include "main.h"
#include "enx_video.h"
#include "h264.h"
#include "jpeg.h"

BYTE nFrameRateDiv[6][12] = {
	{ 1, 2, 3, 4, 5, 6,10,12,15,20,30,60},	// 60fps
	{ 1, 2, 	  5,   10,		   25,50},	// 50fps
	{ 1, 2, 3,	  5, 6,10,	 15,	  30},	// 30fps
	{ 1,		  5,				  25},	// 25fps
	{ 1,	3,	  5,				  15},	// 15fps
	{ 1, 2, 3, 4,	 6, 			  12},	// 12.5fps
};

#ifndef __H264_MCH__
/**============================================================================
	vlock irq handler
*==============================================================================*/
#ifdef __ISP__
static inline void enx_isp_osd(void)
{
#ifdef __HW_OSD__
	char buffer[40] = {0};
#ifdef __HW_OSD_FRAME__
#define OSD_FRAME_X	0
#define OSD_FRAME_Y	1
//	static UINT frame_num = 0;
//	frame_num++;
//	if(frame_num > 99999999) {
//		frame_num = 0;
//	}

	static int buf_len_f = 0;
	if (gptMsgShare.VIDEO_OSD & OSD_FRAME) {
//		buf_len_f = sprintf(buffer, "%08d", frame_num);
		buf_len_f = sprintf(buffer, "%08d", gptMsgShare.VIDEO_TICK);
		DispStr(buffer, OSD_FRAME_Y, OSD_FRAME_X, buf_len_f);
	} else {
		if (buf_len_f) {
			DispClr(OSD_FRAME_Y, OSD_FRAME_X, buf_len_f);
			buf_len_f = 0;
		}
	}
#endif
#ifdef __HW_OSD_TIME__
#define OSD_TIME_X	0
#define OSD_TIME_Y	0
	static UINT buf_len_t = 0;
	if (gptMsgShare.VIDEO_OSD & OSD_TIME) {
		if (sltLocalTime.flag == 1) {
			sltLocalTime.flag = 0;
			buf_len_t = sltLocalTime.len;
			DispStr(sltLocalTime.data, OSD_TIME_Y, OSD_TIME_X, buf_len_t);
		}
	} else {
		if (buf_len_t) {
			DispClr(OSD_TIME_Y, OSD_TIME_X, buf_len_t);
			buf_len_t = 0;
		}
	}
#endif
#endif
}

void enx_isp_i_irq_handler(u32 mode, u32 wdr)
{
	gptMsgShare.VIDEO_FPS = mode >> wdr;

	UNUSED(mode);
	UNUSED(wdr);
}

#ifdef __H264__
#ifdef __H264_2CH__
#define FRAME_DROP_CHECK()		if(h264_enc.chinfo[0].vlock_ready)	nFrameDropCnt[0]++; \
								if(h264_enc.chinfo[1].vlock_ready)	nFrameDropCnt[1]++;
#else
#define FRAME_DROP_CHECK()		if(h264_enc.chinfo[0].vlock_ready)	nFrameDropCnt[0]++;
#endif
#endif

UINT nFrameDropCnt[2] = {0};

void enx_isp_o_irq_handler(u32 mode, u32 wdr)
{
#ifdef __H264__
	// hold�� MP(FrameMode)�� ����� ��� H.264 encoding�� �Ͻ������ϱ� ���� flag�̴�.
	if (h264_enc.isp_outmode != MP(FrameMode)) {
		h264_enc.isp_outmode = MP(FrameMode);
		h264_enc.hold = 4;
	}

	static UINT uCgoFlag = 0;
	if (((h264_enc.chinfo[0].param.width==1920) || (h264_enc.chinfo[1].param.width==1920)) && MP(FrameMode) >= 3) {
		if (uCgoFlag == 1) {
#ifdef __H264_2CH__
			ISP_CGO0w(0);
#endif
			ISP_CGO1w(0);
			uCgoFlag = 0;
		}
	} else {
		if (uCgoFlag == 0) {
#ifdef __H264_2CH__
			ISP_CGO0w(1);
#endif
			ISP_CGO1w(1);
			uCgoFlag = 1;
		}
	}

	// isp_frame�� isp_wdr�� enx_h264_enc_check�Լ����� ������·� ����
	if (mode != h264_enc.isp_frame || wdr != h264_enc.isp_wdr) {
		h264_enc.hold = 4;	// �׶� ������ ��� 1
	}

	if (h264_enc.hold) {
		h264_enc.hold--;
	}

	if (gptMsgShare.VIDEO_TICK % gptMsgShare.VIDEO_FPS == 0 && (nFrameDropCnt[0] || nFrameDropCnt[1])) {
#ifdef __H264_2CH__
		printf("%s Frame Drop(0ch:%d/1ch:%d)\r\n", sltLocalTime.data, nFrameDropCnt[0], nFrameDropCnt[1]);
		nFrameDropCnt[1] = 0;
#else
		printf("%s Frame Drop(0ch:%d)\r\n", sltLocalTime.data, nFrameDropCnt[0]);
#endif
		nFrameDropCnt[0] = 0;
	}

	if (h264_enc.enable == 1 && h264_enc.hold == 0) {
		UINT goFlag = 0;
		if (wdr) {
			if (LS_FLGr == 0) {
				goFlag = 1;
			}
		} else {
			if (((h264_enc.chinfo[0].param.width==1920) || (h264_enc.chinfo[1].param.width==1920)) && MP(FrameMode) >= 3) {
				if (gptMsgShare.VIDEO_TICK % 2 == 1) {
					goFlag = 1;
				}
			} else {
				goFlag = 1;
			}
		}

		if (goFlag == 1) {
			FRAME_DROP_CHECK();
			if (gptMsgShare.H264_ENC_ENABLE & 0x1)	enx_h264_raw_update(0, 1);
#ifdef __H264_2CH__
			if (gptMsgShare.H264_ENC_ENABLE & 0x2)	enx_h264_raw_update(1, 1);
#endif
		}
	}
#endif
	UNUSED(mode);
	UNUSED(wdr);
}

void enx_isp_irq_handler(u32 mode, u32 wdr, u32 dss)
{
	UINT change_flag = 0;

	gptMsgShare.VIDEO_TICK++;

#ifdef __JPEG__
	if (jpeg_enc.enable == 1) {
		change_flag |= enx_jpeg_enc_start(mode, wdr, dss);
	}
#endif

#ifdef __H264__
	if (h264_enc.enable == 1) {
		change_flag |= enx_h264_enc_check(mode, wdr, dss);
	}
#endif

	if (change_flag) {
		CPU_SHARED1 |= eIP1_RESET_FPS;
		CPU_IRQ1 = 1;
	}

#ifdef __HW_OSD__
	enx_isp_osd();
#endif

	UNUSED(mode);
	UNUSED(wdr);
}

#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
void enx_isp_d_irq_handler(u32 mode)
{
	gptMsgShare.VIDEO_FPS = mode;

#ifdef __H264__
	if (gptMsgShare.VIDEO_TICK % gptMsgShare.VIDEO_FPS == 0 && (nFrameDropCnt[0] || nFrameDropCnt[1])) {
#ifdef __H264_2CH__
		printf("%s Frame Drop(0ch:%d/1ch:%d)\r\n", sltLocalTime.data, nFrameDropCnt[0], nFrameDropCnt[1]);
		nFrameDropCnt[1] = 0;
#else
		printf("%s Frame Drop(0ch:%d)\r\n", sltLocalTime.data, nFrameDropCnt[0]);
#endif
		nFrameDropCnt[0] = 0;
	}

	if (h264_enc.enable == 1) {
		FRAME_DROP_CHECK();
		enx_h264_raw_update(0, 0);
#ifdef __H264_2CH__
		enx_h264_raw_update(1, 0);
#endif
	}
#endif

	UNUSED(mode);
}
#endif

#if (IMAGE_INPUT_MODE==2)
extern void LED_Disp(void);
extern BYTE gbIsrIrqState;
extern UINT gnConnection_State;
void IsrVlock(void *dummy)
{
	static BYTE b3GCheck=0;

	if((gnConnection_State==3)||(gnConnection_State==4)) b3GCheck = (b3GCheck) ? 0 : 1;
	else                                                 b3GCheck = 0 ;

	if(b3GCheck==0) {
		LED_Disp();

		gbIsrIrqState = 1;
		enx_isp_d_irq_handler(30);
		enx_isp_irq_handler(30, 0, 0);
	}
	UNUSED(dummy);
}
#endif
#else
void enx_isp_i_irq_handler(u32 mode, u32 wdr, u32 dss)	{	UNUSED(mode);	UNUSED(wdr);	UNUSED(dss);	}
void enx_isp_o_irq_handler(u32 mode, u32 wdr, u32 dss)	{	UNUSED(mode);	UNUSED(wdr);	UNUSED(dss);	}
void enx_isp_irq_handler(u32 mode, u32 wdr, u32 dss)	{	UNUSED(mode);	UNUSED(wdr);	UNUSED(dss);	}
#endif

UINT enx_frame_value(void)
{
	switch (MP(MpOutFps)) {
#if model_1M
		case MN_1M_720p_25_30:
#elif model_2M
		case MN_2M_1080p_25_30:
		case MN_2M_720p_25_30:
#elif model_2M30p
		case MN_2M30_1080p_25_30:
		case MN_2M30_720p_25_30:
#endif
			switch (MP(MpSysFreq)) {
				case MN_SYSFREQ_50:	return 25;
				case MN_SYSFREQ_60:	return 30;
			}
#if model_1M
		case MN_1M_720p_50_60:
#elif model_2M
		case MN_2M_1080p_50_60:
		case MN_2M_720p_50_60:
#elif model_2M30p
		case MN_2M30_720p_50_60:
#endif
			switch (MP(MpSysFreq)) {
				case MN_SYSFREQ_50:	return 50;
				case MN_SYSFREQ_60:	return 60;
			}
	}
	return 30;
}

#endif //__H264_MCH__
