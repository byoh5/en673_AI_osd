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
#include "dev_isp.h"
#include "dev_regs_isp_macro.h"
#endif

#ifdef __H264__
#ifdef __H264_MCH__

#include "enx_mch_video.h"

#include "bitratectl.h"

#include "h264_mch.h"
// rtp_over_rtsp.c
void put_video_streams(UINT addr, UINT size, UINT flags, UINT video_type, UINT ts);

extern sH264Param hmc;

UINT i_frame = 0;

void enx_h264_set_sdsave(u32 value)
{
	hmc.avi_save = value;
}

int enx_h264_get_sdsave(void)
{
	return hmc.avi_save;
}

void enx_h264_set_quantize(int qp, int ch)
{
	hmc.chi[ch].cur_qp = qp;
	//printf("%s %dch qp %d\r\n", __func__, ch, hmc.chi[ch].cur_qp);
	H264_Quantize(ch, qp);
	H264_Default_Quantize(ch, qp);
}

int enx_h264_get_quantize(int ch)
{
	return hmc.chi[ch].cur_qp;
}

int enx_h264_get_default_quantize(int ch)
{
	return hmc.chi[ch].default_qp;
}

int enx_h264_get_maxframe(void)
{
	return hmc.max_fps;
}

void enx_h264_set_maxframe(int fps)
{
	hmc.max_fps = fps;
}

int enx_h264_get_bit_rate(int ch)
{
	return hmc.chi[ch].brsize;
}

int enx_h264_get_bit_rate_mode(int ch)
{
	return hmc.chi[ch].brmode;
}

void enx_h264_set_bit_rate_mode(int mode, int ch)
{
	hmc.chi[ch].brmode = mode;
	//printf("%s %dch mode %d\r\n", __func__, ch, hmc.chi[ch].brmode);

	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)
	enx_h264_set_quantize(hmc.chi[ch].default_qp, ch); //set default Q
	
	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

int enx_h264_get_idr_interval(int ch)
{
	return hmc.chi[ch].i_interval;
}

void enx_h264_set_idrframe_period(int interval, int ch)
{
	hmc.chi[ch].i_interval = interval;
	//printf("%s %dch interval %d\r\n", __func__, ch, hmc.chi[ch].i_interval);
	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
}

void enx_h264_set_frame_rate_sf(SensorFps period, u32 ch)
{
	u32 value = e_isp30fps;

	//jprintf("%s max_fps %d period %d\r\n",__func__,max_fps,period);

	switch(hmc.max_fps)
	{
		case 60: 	value = e_isp60fps; 	break;
		case 50: 	value = e_isp50fps;		break;
		case 30:	value = e_isp30fps;		break;
		case 25:	value = e_isp25fps;		break;
		case 15:	value = e_isp15fps;		break;
		case 12:	value = e_isp12fps;		break;
		default:	break;
	}

	hmc.chi[ch].period = nFrameRateDiv[value][period];
	hmc.chi[ch].fps = hmc.max_fps/hmc.chi[ch].period;

	enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)

	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
	//jprintf("ch %d period %d fps %d \r\n",ch, hmc.chi[ch].period, hmc.chi[ch].fps);
}

void enx_h264_set_frame_size_rs(int resolution, int ch)
{
	int width, height;

	switch(resolution){
		case e_res1920x1080:    width = 1920; height=1080;     break;
		case e_res1280x720:     width = 1280; height=720;     break;
		case e_res800x600:      width = 800;  height=600;     break;
		case e_res704x480:      width = 704;  height=480;     break;
		case e_res704x400:      width = 704;  height=400;     break;
		case e_res640x480:      width = 640;  height=480;     break;
		case e_res640x360:      width = 640;  height=360;     break;
		case e_res320x240:      width = 320;  height=240;     break;
		default:				width = 1920; height=1080;	  break;
	}

	hmc.chi[ch].width       = width;
	hmc.chi[ch].height      = height;

	hmc.chi[ch].hscale 		= (hmc.sen_width * 64) / hmc.chi[ch].width;
	hmc.chi[ch].vscale 		= (hmc.sen_height * 64) / hmc.chi[ch].height;

	init_mch_isp_resolution(ch);

	bitctl_init_seq(ch);
	bitctl_init_gop(ch);
	//printf("ch %d resolution %d width %d height %d \r\n",ch, resolution, hmc.chi[ch].width, hmc.chi[ch].height);
}

void enx_h264_set_bit_rate(H264ConstantBitRate cbr, int ch)
{
	int value = 20;
	switch(cbr)
	{
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

//	dprintf("H264[%d] : BR : %dKbps\r\n", ch, value * 100);

	hmc.chi[ch].brsize = value * 1024 * 1024 / 80;

	//enx_h264_set_quantize(IMG_H264_QULITY, ch); 	// param(cur_qp, pps_qp)
	enx_h264_set_quantize(hmc.chi[ch].default_qp, ch); //set default Q

	bitctl_init_seq(ch);
	bitctl_init_gop(ch);

}

void enx_h264_set_profile(int profilemode, int ch)
{
	switch(profilemode)
	{
		case e_pmBaseline:
			hmc.chi[ch].profile_idc = e_pBaseline;
			hmc.chi[ch].entropy = e_eCAVLC;
			hmc.chi[ch].cabac_idc = 0;
			break;
//		case e_pmMainCV:
//			hmc.chi[ch].profile_idc = e_pMain;
//			hmc.chi[ch].entropy = e_eCAVLC;
//			hmc.chi[ch].cabac_idc = 0;
//			break;
		case e_pmMainCB:
			hmc.chi[ch].profile_idc = e_pMain;
			hmc.chi[ch].entropy = e_eCABAC;
			hmc.chi[ch].cabac_idc = 0;
			break;
		default:
			printf("%s(%d) : Error profile(%d)!\r\n", __func__, __LINE__, profilemode);
			break;
	}
	//enx_h264_set_quantize(IMG_H264_QULITY, ch);		// param(cur_qp, pps_qp)
	enx_h264_set_quantize(hmc.chi[ch].default_qp, ch); //set default Q

	bitctl_init_seq(ch);
	//printf("ch %d profile idc %d entropy %d cabac idc %d \r\n",ch, hmc.chi[ch].profile_idc, hmc.chi[ch].entropy, hmc.chi[ch].cabac_idc);
}



int enx_h264_mch_get_profile(int ch)
{
	if(hmc.chi[ch].entropy == e_eCAVLC)
		return e_pmBaseline;

	if(hmc.chi[ch].entropy == e_eCABAC)
		return e_pmMainCB;

	 return e_pmEndorUnknown;
}

void enx_h264_mch_set_profile_dec(int var, int ch)
{
	hmc.chi[ch].main_dec = var;
	//printf("set %d percent decrease\r\n", hmc.chi[ch].main_dec*6);
}

int enx_h264_mch_get_profile_decrease(int ch)
{
	return hmc.chi[ch].main_dec;
}


int enx_h264_get_stp_size(int ch)
{
	return hmc.chi[ch].stpsize;
}

// 1iframe 0 pframe
int enx_h264_get_slice_type(int ch)
{
	int idx;
	idx = h264_mch_get_enc_queue(ch);

	return hmc.chi[ch].slice_type[idx];
}

u32 enx_h264_get_prebits(void)
{
	return (H264_PREBITS >> 3);
}

void enx_h264_on(void)
{
	//set h264 disable
	h264_mch_set_disable();

	//enx_video
	SYS_HCK_PD = 0;
	SYS_HCK_SEL = 0;	// 0:PLL0 1:PLL1 select
	SYS_HCK_PD = 1;
	SYS_H264_RST = 1;

#ifdef __DEVICE_SD__
	if(FAT_SDSAVE == SDSAVE_USE_ON)
		enx_h264_set_sdsave(DEF_ON);
	else
		enx_h264_set_sdsave(DEF_OFF);
#endif

	//enx_video
	MYCW_HWOw(SENSOR_SIZE_W+8);

	//set Ch num
	h264_mch_set_channel_number(1);
#ifdef __H264_2CH__
	h264_mch_set_channel_number(2);
#endif

	// CH0 (FHD) => 0 ~ 3 queue 사용.
	h264_mch_init_queue(MCH_0, 0,(MAX_Q_CH_NUM-1));

	//IMG_MAX_FPS	//90000/IMG_MAX_FPS//time stamp	//IMG_H264_PROFILE//profile cabac, cavlc
	//IMG_H264_QULITY// h.264 qp level 0~51	//IMG_H264_IDR// I frame period	//IMG_H264_FPS// 30fps/value = out fps
	//IMG_H264_RES//resolution 1ch	//IMG_BR_H// bitrate	//IMG_BR_HR// bitrate size
	h264_mch_init_param(MCH_0, SENSOR_SIZE_W, SENSOR_SIZE_H, IMG_H264_IDR, IMG_H264_FPS, IMG_H264_RES, IMG_H264_QULITY, 2, IMG_H264_PROFILE, IMG_BR_H, IMG_BR_HR, IMG_MAX_FPS, 90000/IMG_MAX_FPS); // encoder param 초기화 gop(30), qp(32), cabac

	enx_h264_mch_set_profile_dec(0, MCH_0);

#ifdef __H264_2CH__

	// CH1 (HD) -> 4 ~ 7 queue 사용.
	h264_mch_init_queue(MCH_1, 0, (MAX_Q_CH_NUM-1));

	//IMG_MAX_FPS	//90000/IMG_MAX_FPS//time stamp	//IMG_H264_2CH_PROFILE//profile cabac, cavlc
	//IMG_H264_2CH_QULITY	// h.264 qp level 0~51//IMG_H264_2CH_IDR// I frame period//IMG_H264_2CH_FPS// 30fps/value = out fps
	//IMG_H264_2CH_RES//resolution 2ch	//IMG_BR_H2// bitrate	//IMG_BR_H2R// bitrate size
	h264_mch_init_param(MCH_1, SENSOR_SIZE_W, SENSOR_SIZE_H, IMG_H264_2CH_IDR, IMG_H264_2CH_FPS, IMG_H264_2CH_RES, IMG_H264_2CH_QULITY, 2, IMG_H264_2CH_PROFILE, IMG_BR_H2, IMG_BR_H2R, IMG_MAX_FPS, 90000/IMG_MAX_FPS); // encoder param 초기화 gop(30), qp(32), cabac

	enx_h264_mch_set_profile_dec(0, MCH_1);

#endif
	// buffer allocation
//	base_addr =  H264_STRM_BASE;
//	base_addr =  h264_mch_init_buffer(MCH_0, base_addr, 0x1C8000, 0x400000);
//
//#ifdef __H264_2CH__
//	base_addr =  h264_mch_init_buffer(MCH_1, base_addr, 0x5A000, 0x200000);
//#endif
	h264_mch_init_buffer();

	H264_SBUFSIZE	= 0x200000;	//4*1024;

	//enx_video
	IntAdd(IRQ_H264, (void*)enx_h264_mch_irq_handler, 0);
	IrqEnable(IRQ_H264);

#ifdef __ISP__

	h264_mch_default_isp_register();	// codec default register

	//init_mch_isp_resolution(MCH_0);
#ifdef __H264_2CH__
	//init_mch_isp_resolution(MCH_1);
#endif

#endif

#ifdef __H264_TEST__

	h264_mch_make_raw(h264_get_width(MCH_0), h264_get_height(MCH_0));
	h264_mch_set_hold_state(0);
	h264_mch_set_vlock_ready_state(0, 1);
	h264_mch_set_enc_state(VLOCK_GO);
	h264_mch_set_vlc_state(VLOCK_GO);

#endif

	gptMsgShare.VIDEO_TICK = 0;

	//encoding enable
//	h264_mch_set_enable();
//	h264_mch_set_hold_state(0);
//	h264_mch_set_enc_state(VLOCK_GO);
//	h264_mch_set_vlc_state(VLOCK_GO);

}


/**
	takes 200ms
	prepare image capute for encoding
*/
void enx_h264_image_capture_ready(void)
{
	UINT tick_cnt = 1;
	volatile UINT tick_temp = 0;

	while(!gptMsgShare.VIDEO_TICK)				// 0 ~ 33.3ms
	{
#ifdef __WDT__
		enx_wdt_reset();
#endif
		WaitXus(1);
		if(++tick_cnt % 1000000 == 0)			// Check 1sec
		{
			printf("Video Input not ready!(%d)\r\n", __LINE__);
			if(tick_cnt >= 5000000)
				return;
		}
	}
	tick_temp = gptMsgShare.VIDEO_TICK;

	tick_cnt = 1;


	enx_h264_mch_raw_update(0);

	if(hmc.num_ch == 2)
		enx_h264_mch_raw_update(1);


	while(tick_temp == gptMsgShare.VIDEO_TICK)	// 16.6ms or 33.3ms
	{
#ifdef __WDT__
		enx_wdt_reset();
#endif
		WaitXus(1);
		if(++tick_cnt % 1000000 == 0)			// Check 1sec
		{
			printf("Video Input not ready!(%d)\r\n", __LINE__);
			if(tick_cnt >= 5000000)
				return;
		}
	}
	printf("H.264: Ready\r\n");

	//h264_enc.vlock_fisrt = VLOCK_GO;
	//encoding enable
	h264_mch_set_enable();
	h264_mch_set_hold_state(0);
	h264_mch_set_enc_state(VLOCK_GO);
	h264_mch_set_vlc_state(VLOCK_GO);
}

//******************************************************************************
// Code
//------------------------------------------------------------------------------
void enx_h264_mch_raw_update(int ch)
{
	int idx;

	if(h264_mch_check_raw_index(ch) == 0)
	{
		//jprintf("raw skip %dch v%d valid ENC %d Raw %d q size %d \n", ch,h264_get_vlock_ready_state(ch), h264_get_queue_validnum(ch), h264_get_raw_valid_num(ch),h264_get_queue_size(ch));
		//printf("raw skip %dch v%d valid ENC %d Raw %d q size %d \r\n",
		//ch,h264_mch_get_vlock_ready_state(ch), h264_mch_get_queue_validnum(ch), h264_mch_get_raw_valid_num(ch),h264_mch_get_queue_size(ch));
		return;
	}
	if(h264_mch_check_queue_enc_ready(ch) == 0)
	{
		//printf("enc buffer not enough  %dch v%d valid ENC %d Raw %d q size %d \r\n",
		//	ch, h264_mch_get_vlock_ready_state(ch), h264_mch_get_queue_validnum(ch), h264_mch_get_raw_valid_num(ch),h264_mch_get_queue_size(ch));
		//return;
	}

	idx = h264_mch_get_raw_queue(ch);

#ifdef __ISP__

	if(ch == 0)	{	// main(ISP_YADR1) use
		ISP_YADR1w(((h264_mch_get_cur_yaddr(ch, idx)) >> 4) << 1);
		ISP_CADR1w(((h264_mch_get_cur_caddr(ch, idx)) >> 4) << 1);
		ISP_GO1w(1);
		ISP_GO1w(0);
	}
	else {	// sub(ISP_YADR0) use
		ISP_YADR0w(((h264_mch_get_cur_yaddr(ch, idx)) >> 4) << 1);
		ISP_CADR0w(((h264_mch_get_cur_caddr(ch, idx)) >> 4) << 1);
		ISP_GO0w(1);
		ISP_GO0w(0);
	}

#endif

	h264_mch_update_raw_queue(ch);
	h264_mch_increase_vlock_ready_state(ch);
	h264_mch_increase_raw_valid_num(ch);
	//printf("%dch raw update raw idx %dth\r\n",ch, idx);
}

//******************************************************************************
// Processing
//------------------------------------------------------------------------------
u32 enx_h264_get_ts(int ch)
{	// H.264 ts = (90000 / gptMsgShare.VIDEO_FPS) * h264_enc.period
	return h264_mch_get_timestamp(ch);
}

UINT enx_h264_mch_enc_check(u32 mode, u32 wdr, u32 dss)
{
	//jprintf("%s \r\n",__func__);

	UINT change_flag = DEF_NO;
	if(mode != h264_mch_get_isp_frame() || wdr != h264_mch_get_isp_wdr() || dss != h264_mch_get_isp_dss())
	{
		change_flag = DEF_YES;
		h264_mch_set_isp_frame(mode);
		h264_mch_set_isp_wdr(wdr);
		h264_mch_set_isp_dss(dss);

		if(h264_mch_get_isp_wdr() == 0)
		{
			enx_h264_set_maxframe(h264_mch_get_isp_frame());
			h264_mch_set_default_timestamp(90000 / enx_h264_get_maxframe());

			if(enx_h264_get_maxframe() == 60)
			{
				if(h264_mch_get_width(0) > 1280)
				{
					enx_h264_set_frame_rate_sf(e_s60f30, 0);
					enx_h264_set_frame_rate_sf(e_s60f30, 1);
				}
				else
				{
					enx_h264_set_frame_rate_sf(e_s60f60, 0);
					enx_h264_set_frame_rate_sf(e_s60f60, 1);
				}
			}
			else if(enx_h264_get_maxframe() == 50)
			{
				if(h264_mch_get_width(0) > 1280)
				{
					enx_h264_set_frame_rate_sf(e_s50f25, 0);
					enx_h264_set_frame_rate_sf(e_s50f25, 1);
				}
				else
				{
					enx_h264_set_frame_rate_sf(e_s50f50, 0);
					enx_h264_set_frame_rate_sf(e_s50f50, 1);
				}
			}
			else if(enx_h264_get_maxframe() == 30)
			{
				enx_h264_set_frame_rate_sf(e_s30f30, 0);
				enx_h264_set_frame_rate_sf(e_s30f30, 1);
			}
			else if(enx_h264_get_maxframe() == 25)
			{
				enx_h264_set_frame_rate_sf(e_s25f25, 0);
				enx_h264_set_frame_rate_sf(e_s25f25, 1);
			}
			jprintf("rate change(noWDR) : %d/%d/%d/%d\r\n",
				h264_mch_get_isp_frame(), enx_h264_get_maxframe(), h264_mch_get_isp_wdr(), h264_mch_get_default_timestamp());
		}
		else
		{																// WDR을 켰고...
			//if (h264_mch_get_isp_wdr() != 1) {
			//	printf("WDR value?(%d)\r\n", h264_mch_get_isp_wdr());
			//	h264_mch_set_isp_wdr(1);
			//}

			enx_h264_set_maxframe(h264_mch_get_isp_frame() / 2);				// isp_frame은 60,50,30,25가 온다.
			h264_mch_set_default_timestamp((90000 * 2) / h264_mch_get_isp_frame());
			if(enx_h264_get_maxframe() == 30)
			{
				enx_h264_set_frame_rate_sf(e_s30f30, 0);
				enx_h264_set_frame_rate_sf(e_s30f30, 1);
			}
			else if(enx_h264_get_maxframe() == 25)
			{
				enx_h264_set_frame_rate_sf(e_s25f25, 0);
				enx_h264_set_frame_rate_sf(e_s25f25, 1);
			}
			else if(enx_h264_get_maxframe() == 15)
			{
				enx_h264_set_frame_rate_sf(e_s15f15, 0);
				enx_h264_set_frame_rate_sf(e_s15f15, 1);
			}
			else if(enx_h264_get_maxframe() == 12)
			{
				enx_h264_set_frame_rate_sf(e_s15f15, 0);	// 12.5 fps?
				enx_h264_set_frame_rate_sf(e_s15f15, 1);	// 12.5 fps?
			}
			jprintf("rate change(WDR) : %d/%d/%d/%d\r\n",
				h264_mch_get_isp_frame(), enx_h264_get_maxframe(), h264_mch_get_isp_wdr(), h264_mch_get_default_timestamp());
		}
	}

	return change_flag;
}

extern BYTE gbMnDebugFnc;	// ISP debug mode(OSD align)

void enx_h264_mch_irq_handler(void *arg)
{
	int ch;
	UINT status;

	status 	= h264_mch_get_status_res();
	//jprintf("!status %x \r\n",status);

	//ENC END////////////////////////////
	if(h264_mch_check_enc_end_status_res(status))
	{
		ch = h264_mch_get_enc_channel_number_res();
		enx_h264_mch_enc_end(ch, status);
	}

	//VLC END////////////////////////////
	if(h264_mch_check_vlc_end_status_res(status))
	{
		ch = h264_mch_get_vlc_channel_number_res();
		enx_h264_mch_vlc_end(ch, status);
	}

#ifdef GPIO_H264_ENC_LED
//	enx_h264_debug_led();
#endif

	UNUSED(arg);
}

void enx_h264_mch_enc_first_start(int ch)
{
	int slice_type;

	if(ch == 0)	h264_mch_set_enc_state(VLOCK_CH0_ENC_START);
	else	h264_mch_set_enc_state(ULOCK_CH1_ENC_START);

	h264_mch_set_vlock_ready_state(ch, 0);

	h264_mch_set_param(ch);
	h264_mch_set_enc_buffer(ch);
	h264_mch_encode_param(ch, &slice_type);
	h264_mch_encode_command(ch);

#ifdef __H264_CHECK_TIME__
	h264_mch_check_enc_starttime();
#endif
	h264_mch_set_queue(ch, slice_type);
	//h264_mch_decrease_raw_valid_num(ch);

}

void enx_h264_mch_enc_start(int ch)
{
	int slice_type;

	if(h264_mch_get_enc_state() == VLOCK_GO)
	{
		if(ch == 0)	h264_mch_set_enc_state(VLOCK_CH0_H264_FIRST);
		else	h264_mch_set_enc_state(ULOCK_CH1_H264_FIRST);
	}
	else
	{
		if(ch == 0)	h264_mch_set_enc_state(VLOCK_CH0_ENC_START);
		else	h264_mch_set_enc_state(ULOCK_CH1_ENC_START);
	}
	h264_mch_decrease_vlock_ready_state(ch);

	h264_mch_set_param(ch);
	h264_mch_set_enc_buffer(ch);
	h264_mch_encode_param(ch, &slice_type);
	h264_mch_encode_command(ch);

#ifdef __H264_CHECK_TIME__
	h264_mch_check_enc_starttime();
#endif
	h264_mch_set_queue(ch, slice_type);
	//h264_mch_decrease_raw_valid_num(ch);

}

void enx_h264_mch_vlc_start(int ch)
{
	if(ch == 0)	h264_mch_set_vlc_state(VLOCK_CH0_VLC_START);
	else	h264_mch_set_vlc_state(ULOCK_CH1_VLC_START);

	h264_mch_set_vlc_buffer(ch);
	h264_mch_vlc_command();

#ifdef __H264_CHECK_TIME__
	h264_mch_check_vlc_starttime();
#endif

}

void enx_h264_mch_enc_end(int ch, int status)
{
	//ENC END////////////////////////////
	if(h264_mch_check_enc_end_status_res(status))
	{
		//H264_STATUS_T = 0x01;
		//printf("!%dch ENCEND idx %d\r\n", ch, hmc.que[ch].q_eidx);

		if(h264_mch_get_enc_state() == VLOCK_CH0_ENC_START)
		{

			h264_mch_set_enc_state(VLOCK_CH0_ENC_END);
			//jprintf("!status %x A\r\n",status);
			//h264_mch_decrease_raw_valid_num(ch);/////////////////////////////////////////

			// 완료된 encoding 정보를 update한다.
#ifdef __H264_CHECK_TIME__
			h264_mch_check_enc_endtime(ch);
#endif
			bitctl_check_warning(ch);

			h264_mch_set_update_queue(ch);
			h264_mch_shift_buffer(ch);
			h264_mch_increase_pic(ch);
			//H264_regprintf();

			enx_h264_mch_check_enc_result(ch);
#ifdef __H264_TEST__
			//enx_h264_mch_enc_poll();
#else
			//enx_h264_mch_enc_poll();
			//enx_h264_mch_vlc_poll();
#endif
		}
		else if(h264_mch_get_enc_state() == ULOCK_CH1_ENC_START)
		{

			h264_mch_set_enc_state(ULOCK_CH1_ENC_END);
			//h264_mch_decrease_raw_valid_num(ch);/////////////////////////////////////////
			//jprintf("!status %x B\r\n",status);

#ifdef __H264_CHECK_TIME__
			h264_mch_check_enc_endtime(ch);
#endif
			bitctl_check_warning(ch);

			// 완료된 encoding 정보를 update한다.
			h264_mch_set_update_queue(ch);
			h264_mch_shift_buffer(ch);
			h264_mch_increase_pic(ch);
			//H264_regprintf();

			enx_h264_mch_check_enc_result(ch);
#ifdef __H264_TEST__
			//enx_mch_h264_enc_poll();
#else
			//enx_h264_mch_enc_poll();
			//enx_h264_mch_vlc_poll();
#endif
		}
		else
		{
			printf("ENC STATE ERROR!\r\n");
		}
	}
}

void enx_h264_mch_vlc_end(int ch, int status)
{


	//VLC END////////////////////////////
	if(h264_mch_check_vlc_end_status_res(status))
	{
		//H264_STATUS_T = 0x10;
		//printf("!VLCEND--\r\n");

		if(h264_mch_get_vlc_state() == VLOCK_CH0_VLC_START)
		{
			h264_mch_set_vlc_state(VLOCK_CH0_VLC_END);
			//jprintf("!status %x \r\n",status);

#ifdef __H264_CHECK_TIME__
			h264_mch_check_vlc_endtime(ch);
#endif

			h264_mch_get_update_queue(ch);

			//debug log pic number
			//h264_mch_get_interval_pic(0,30);

			//debug log resister information
			//H264_mch_regprintf();

			enx_h264_mch_update_streambuffer(ch);

#ifdef __H264_TEST__
			//enx_h264_mch_vlc_poll();
#else
//			enx_h264_mch_enc_poll();
//			enx_h264_mch_vlc_poll();
#ifdef __H264_RESET_TEST__
			h264_mch_vlc_test_reset();
#endif

#endif
		}
		else if(h264_mch_get_vlc_state() == ULOCK_CH1_VLC_START)
		{
			h264_mch_set_vlc_state(ULOCK_CH1_VLC_END);
			//jprintf("!status %x \r\n",status);

#ifdef __H264_CHECK_TIME__
			h264_mch_check_vlc_endtime(ch);
#endif

			h264_mch_get_update_queue(ch);

			//debug log pic number
//			h264_mch_get_interval_pic(0,30);

			//debug log resister information
			//H264_mch_regprintf();

			enx_h264_mch_update_streambuffer(ch);

#ifdef __H264_TEST__
			//enx_h264_vlc_poll();
#else
//			enx_h264_mch_enc_poll();
//			enx_h264_mch_vlc_poll();
#endif
		}
		else
		{
			printf("VLC STATE ERROR!\r\n");
			return;
		}
	}
}

void enx_h264_mch_check_enc_result(int ch)
{
	u32 stp_size_byte;
	u32 enc_result_byte;

	stp_size_byte = enx_h264_get_stp_size(ch);
	enc_result_byte = h264_mch_get_prebits_res();

	//printf("%dch H264_PREBITS : size(%d)\r\n", ch, (H264_PREBITS >> 3));

	if(enc_result_byte > stp_size_byte/2)
	{
		dprintf("h264_irq_handler : warning %dch H264_PREBITS : size(%d)\r\n", ch, enc_result_byte);
	}

	if(enc_result_byte > stp_size_byte)
	{
		dprintf("h264_irq_handler : %dch H264_PREBITS : size(%d)\r\n", ch, enc_result_byte);
	}

	if(ch == 0 && enc_result_byte>1*1024*1024)
		printf("!ENC %dch SIZE (%d)byte\r\n", ch, enc_result_byte);
	//
	//if(ch == 1 && enc_result_byte>300*1024)
	//	printf("!ENC %dch SIZE (%d)byte\r\n", ch, enc_result_byte);
	//jprintf("!ENC %dch SIZE (%d)byte\r\n", ch, enc_result_byte);

}

void enx_h264_mch_update_streambuffer(int ch)
{
	u32 straddr;
	u32 strsize;
	int slice_type;

	h264_mch_update_stream_size(ch);

	straddr = h264_mch_get_stream_address_res();
	strsize = h264_mch_get_genbits_res();
	slice_type = h264_mch_get_slice_type_res();

//	if(slicetype == 1)
			//printf("slicetype %d H264_GENBITS : size(%d)\r\n", slice_type, (H264_GENBITS >> 3));
//	printf("==========================%d %d======================\r\n",H264_GENBITS,H264_GENBITS >> 3);
		bitctl_update_frame_merge(slice_type, ch, strsize); //H264_GENBITS >>3 , slicetype

	//if(ch == 0 && strsize > 512*1024)
	//	printf("!VLC %dch STRM ADDR 0x%x SIZE [%d] \r\n", ch, straddr, strsize);
	//if(ch == 1 && strsize > 300*1024)
	//	printf("!VLC %dch STRM ADDR 0x%x SIZE [%d] \r\n", ch, straddr, strsize);
	//jprintf("!VLC %dch STRM ADDR 0x%x SIZE [%d] \r\n", ch, straddr, strsize);
	u32 new_ts = enx_h264_get_ts(ch);
	//printf("stream_ts %d \r\n", new_ts);

	put_video_streams(straddr, strsize, slice_type, ch, new_ts);

#ifdef __DEVICE_SD__
	if(enx_h264_get_sdsave() && ch == 0)
	{
		u8 stream_type = 0;
		if (slice_type == 1) 	stream_type = eSTREAMMSG_H2641I;
		else					stream_type = eSTREAMMSG_H2641P;
		MsgRecPut(straddr, strsize, new_ts, stream_type);
		CPU_SHARED1 |= eIP1_RECORD_INFO;
		CPU_IRQ1 = 1;
	}
#endif
	h264_mch_update_strbuffer(ch, strsize);

	//gptMsgShare.Buffer_Flag[ch] = 0;

#ifdef __ISP__
#ifdef __HW_OSD_H264__
#define OSD_H264_X	0
#define OSD_H264_Y	2
#define OSD_H264_Yd	14

	h264_mch_increase_vlcdonetick(ch);

	char buffer[24] = {0};
	static int buf_len[2][4] = {{0}};
	static int osd_mode = 0;

	if ((ch == 0 && gptMsgShare.VIDEO_OSD & OSD_H264_1) ||
		(ch == 1 && gptMsgShare.VIDEO_OSD & OSD_H264_2)) {
		if (gbMnDebugFnc == 0) {
			osd_mode = 0;
			buf_len[ch][0] = sprintf(buffer, "CH%d(%5s)", ch, H264_CABACEN == 0 ? "CAVLC" : H264_CABACEN == 1 ? "CABAC" : "Unknown");
			DispStr(buffer, OSD_H264_Y+0+(ch*4), OSD_H264_X, buf_len[ch][0]);
			buf_len[ch][1] = sprintf(buffer, "%8s", H264_PROFILEIDC == e_pBaseline ? "Baseline" : H264_PROFILEIDC == e_pMain ? "Main" : "Unknown");
			DispStr(buffer, OSD_H264_Y+1+(ch*4), OSD_H264_X, buf_len[ch][1]);
			buf_len[ch][2] = sprintf(buffer, "%c%02d%7d", slice_type == 1 ? 'I' : 'P', h264_mch_get_vlc_quant_res(), strsize);
			DispStr(buffer, OSD_H264_Y+2+(ch*4), OSD_H264_X, buf_len[ch][2]);
			buf_len[ch][3] = sprintf(buffer, "%08d", h264_mch_get_vlcdonetick(ch));
			DispStr(buffer, OSD_H264_Y+3+(ch*4), OSD_H264_X, buf_len[ch][3]);
		} else {
			osd_mode = 1;
			buf_len[ch][0] = sprintf(buffer, "CH%d %8s %5s", ch,
				H264_PROFILEIDC == e_pBaseline ? "Baseline" :
				H264_PROFILEIDC == e_pMain ? "Main" : "Unknown", H264_CABACEN == 0 ? "CAVLC" :
					H264_CABACEN == 1 ? "CABAC" : "Unknown");
			DispStr(buffer, OSD_H264_Yd+0+(ch*2), OSD_H264_X, buf_len[ch][0]);
			buf_len[H264_MCHNNEL][1] = sprintf(buffer, "	%c%02d %7d %04d", slice_type == 1 ? 'I' : 'P', h264_mch_get_vlc_quant_res(), strsize, h264_mch_get_vlcdonetick(ch));
			DispStr(buffer, OSD_H264_Yd+1+(ch*2), OSD_H264_X, buf_len[ch][1]);
		}
	} else {
		if (buf_len[ch][0]) {
			if ((ch == 0 && gptMsgShare.VIDEO_OSD & ~OSD_H264_1) ||
				(ch == 1 && gptMsgShare.VIDEO_OSD & ~OSD_H264_2)) {
				if (osd_mode == 0) {
					DispClr(OSD_H264_Y+0+(ch*4), OSD_H264_X, buf_len[ch][0]);
					DispClr(OSD_H264_Y+1+(ch*4), OSD_H264_X, buf_len[ch][1]);
					DispClr(OSD_H264_Y+2+(ch*4), OSD_H264_X, buf_len[ch][2]);
					DispClr(OSD_H264_Y+3+(ch*4), OSD_H264_X, buf_len[ch][3]);
				} else if (osd_mode == 1) {
					DispClr(OSD_H264_Yd+0+(ch*2), OSD_H264_X, buf_len[ch][0]);
					DispClr(OSD_H264_Yd+1+(ch*2), OSD_H264_X, buf_len[ch][1]);
				}
				buf_len[ch][0] = 0;
				buf_len[ch][1] = 0;
				buf_len[ch][2] = 0;
				buf_len[ch][3] = 0;
			}
		}
	}
#endif
#endif

}

void enx_h264_enc_poll(void)
{
	//printf(".");
	enx_h264_mch_enc_poll();
	enx_h264_mch_vlc_poll();
}

void enx_h264_mch_enc_poll(void)
{
	int ch0 = 0;
#ifdef __H264_2CH__
	int ch1 = 1;
#endif	//__H264_2CH__

	if (gptMsgDebug.H264_POLL_LOG & 0x8) {
		gptMsgDebug.H264_POLL_LOG &= ~0x8;

		printf("vlock_first(0x%03X) hold(%d) ready1(%d)\r\n",
			h264_mch_get_enc_state, h264_mch_get_hold_state(), h264_mch_get_vlock_ready_state(ch0));

#ifdef __H264_2CH__
		printf("ready2(%d)\r\n", h264_mch_get_vlock_ready_state(ch1));
#endif	//__H264_2CH__
	}

	//for first encoding
	if((h264_mch_get_hold_state() == 0)
		&& (h264_mch_get_enc_state() & (VLOCK_NONE |  VLOCK_GO)))
	{

#ifdef __H264_2CH__
		if(h264_mch_get_vlock_ready_state(ch1)
			&& h264_mch_get_raw_valid_num(ch1) > 0
		 	//&& (!((gptMsgShare.VIDEO_TICK+1) % h264_get_period(ch1)))
		 	)
		{
			//enx_h264_mch_enc_first_start(ch1);
			int	slice_type;

			//h264_set_vlock_ready_state(ch1,0);
			h264_mch_set_enc_state(ULOCK_CH1_ENC_START);

			//h264_decrease_vlock_ready_state(ch1);
			h264_mch_set_vlock_ready_state(ch1, 0);
			// CH0  parameter 설정.
			h264_mch_set_param(ch1);
			h264_mch_set_enc_buffer(ch1);
			h264_mch_encode_param(ch1, &slice_type);
			h264_mch_encode_command(ch1);

#ifdef __H264_CHECK_TIME__
			h264_mch_check_enc_starttime();
#endif	//__H264_CHECK_TIME__
			h264_mch_set_queue(ch1, slice_type);
			h264_mch_decrease_raw_valid_num(ch1);
		}
#else	//__H264_2CH__

		if(h264_mch_get_vlock_ready_state(ch0)
			&& h264_mch_get_raw_valid_num(ch0) > 0
		 	//&& (!((gptMsgShare.VIDEO_TICK+1) % h264_get_period(ch0)))
		 	)
		{
			//enx_h264_mch_enc_first_start(ch0);
			int	slice_type;

			//h264_set_vlock_ready_state(ch0,0);
			h264_mch_set_enc_state(VLOCK_CH0_ENC_START);

			//h264_decrease_vlock_ready_state(ch0);
			h264_mch_set_vlock_ready_state(ch0, 0);
			// CH0  parameter 설정.
			h264_mch_set_param(ch0);
			h264_mch_set_enc_buffer(ch0);
			h264_mch_encode_param(ch0, &slice_type);
			h264_mch_encode_command(ch0);

#ifdef __H264_CHECK_TIME__
			h264_mch_check_enc_starttime();
#endif	//__H264_CHECK_TIME__
			h264_mch_set_queue(ch0, slice_type);
			h264_mch_decrease_raw_valid_num(ch0);

		}
#endif	//__H264_2CH__

	}
	else if(h264_mch_get_enable_state() == 1
		&& h264_mch_get_hold_state() == 0)
	{
#ifdef __H264_2CH__

		if(h264_mch_get_vlock_ready_state(ch1)
			&& h264_mch_get_raw_valid_num(ch1) > 1
#ifdef __H264_SERIAL_MODE__
			&& !(h264_mch_get_vlc_state() & (VLOCK_CH0_VLC_START|ULOCK_CH1_VLC_START))
#endif	//__H264_SERIAL_MODE__
			&&  (h264_mch_get_enc_state() & (VLOCK_CH0_ENC_END | ULOCK_CH1_ENC_END))
			//&&  (h264_mch_get_enc_state() & (ULOCK_CH1_ENC_END))
			)
		{
			if(h264_mch_check_queue_enc_ready(ch1)
		 	//&& (!((gptMsgShare.VIDEO_TICK+1) % h264_get_period(ch1)))
		 	)
			{
				//enx_h264_mch_enc_start(ch1);
				int slice_type;


				// 다음 encoidng 을 위한 파라미터 설정.
				//if(h264_mch_get_enc_state() == VLOCK_GO)
				//{
				//	h264_mch_set_enc_state(ULOCK_CH1_H264_FIRST);
				//	//h264_set_vlock_ready_state(ch1,0);
				//}
				//else
					h264_mch_set_enc_state(ULOCK_CH1_ENC_START);


				h264_mch_decrease_vlock_ready_state(ch1);

				h264_mch_set_param(ch1);
				h264_mch_set_enc_buffer(ch1);
				h264_mch_encode_param(ch1, &slice_type);
				h264_mch_encode_command(ch1);

#ifdef __H264_CHECK_TIME__
				h264_mch_check_enc_starttime();
#endif	//__H264_CHECK_TIME__
				h264_mch_set_queue(ch1, slice_type);
				h264_mch_decrease_raw_valid_num(ch1);
			}
		}
		else

#endif	//__H264_2CH__
		if(h264_mch_get_vlock_ready_state(ch0)
			&& h264_mch_get_raw_valid_num(ch0) > 1
#ifdef __H264_SERIAL_MODE__
			&& !(h264_mch_get_vlc_state() & (VLOCK_CH0_VLC_START|ULOCK_CH1_VLC_START))
#endif	//__H264_SERIAL_MODE__
			&&  (h264_mch_get_enc_state() & (VLOCK_CH0_ENC_END | ULOCK_CH1_ENC_END))
			//&&  (h264_mch_get_enc_state() & (ULOCK_CH1_ENC_END))
			)
		{
			if(h264_mch_check_queue_enc_ready(ch0)
		 	//&& (!((gptMsgShare.VIDEO_TICK+1) % h264_get_period(ch0)))
		 	)
			{
				//enx_h264_mch_enc_start(ch0);
				int slice_type;


				//if(h264_mch_get_enc_state()== VLOCK_GO)
				//	h264_mch_set_enc_state(VLOCK_CH0_H264_FIRST);
				//else
					h264_mch_set_enc_state(VLOCK_CH0_ENC_START);

				h264_mch_decrease_vlock_ready_state(ch0);

				// 다음 encoidng 을 위한 파라미터 설정.
				h264_mch_set_param(ch0);
				h264_mch_set_enc_buffer(ch0);
				h264_mch_encode_param(ch0, &slice_type);
				h264_mch_encode_command(ch0);
#ifdef __H264_CHECK_TIME__
				h264_mch_check_enc_starttime();
#endif	//__H264_CHECK_TIME__
				h264_mch_set_queue(ch0, slice_type);
				h264_mch_decrease_raw_valid_num(ch0);
			}
		}
	}
}

void enx_h264_mch_vlc_poll(void)
{
	int ch0 = 0;
#ifdef __H264_2CH__
	int ch1 = 1;
#endif	//__H264_2CH__

	if(h264_mch_get_enable_state()== 1
		&& h264_mch_get_hold_state()== 0
		&& (h264_mch_get_vlc_state() & (VLOCK_GO | VLOCK_CH0_H264_FIRST | ULOCK_CH1_H264_FIRST | VLOCK_CH0_VLC_END | ULOCK_CH1_VLC_END))
		//&& (h264_mch_get_vlc_state() & (VLOCK_GO | VLOCK_CH0_H264_FIRST | ULOCK_CH1_H264_FIRST | ULOCK_CH1_VLC_END))
		)
	{
#ifdef __H264_2CH__

		 if(h264_mch_check_queue_vlc_ready(ch1))
		{
			//enx_h264_mch_vlc_start(ch1);
			h264_mch_set_vlc_state(ULOCK_CH1_VLC_START);

			h264_mch_set_vlc_buffer(ch1);
			h264_mch_vlc_command();
			//gptMsgShare.Buffer_Flag[ch1] = 1;
#ifdef __H264_CHECK_TIME__
			h264_mch_check_vlc_starttime();
#endif	//__H264_CHECK_TIME__
		}
		else
#endif	//__H264_2CH__

		if(h264_mch_check_queue_vlc_ready(ch0))
		{
			//enx_h264_mch_vlc_start(ch0);
			h264_mch_set_vlc_state(VLOCK_CH0_VLC_START);

			h264_mch_set_vlc_buffer(ch0);
			h264_mch_vlc_command();
			//gptMsgShare.Buffer_Flag[ch0] = 1;
#ifdef __H264_CHECK_TIME__
			h264_mch_check_vlc_starttime();
#endif	//__H264_CHECK_TIME__
		}
	}
}

void h264_mch_cbr_processing(int ch, UINT idr)
{
	u32 max_frame;
	UINT average;
	int un32QP;

	un32QP = enx_h264_get_quantize(ch);
	max_frame = enx_h264_get_maxframe();

	int frameCnt;
	int flagFrame;
	int cbrmode;
	int i_interval;
	UINT i_predic_sz;
	UINT p_predic_sz;
	UINT bitrate;
	UINT period;

	cbrmode = enx_h264_get_bit_rate_mode(ch);
	i_interval = enx_h264_get_idr_interval(ch);
	period = h264_get_period(ch);
	bitrate = enx_h264_get_bit_rate(ch);
	i_predic_sz = bitctl_get_previous_iframe_byte(ch);
	p_predic_sz = bitctl_get_previous_pframe_byte(ch);

	//높은 비트레이트 에서 낮은 비트레이트로 설정될 시 cbr->bitrate가 quant->i_predic_sz보다 작아서 average가 음수가 될 가능성이 있다
	if ((int)(bitrate - i_predic_sz) > 0) {
		average = (bitrate - i_predic_sz) * period / max_frame;
	} else {
		average = bitrate * period / max_frame;
	}

	frameCnt = h264_mch_get_framecnt(ch);
	flagFrame = h264_mch_get_flagframe(ch);

	//급격히 증가하는 비트레이트 막기
	if (frameCnt != 1) {
		if (((UINT)bitctl_get_previous_byte(ch) << 3) > (average << 5)) {
			if (flagFrame == 0)
			{
				flagFrame = 3;
				h264_mch_set_flagframe(ch, flagFrame);
			}
		}
	}
	if (flagFrame > 0) {
		un32QP += 2;
		if (un32QP > H264_Quantize_MAX) {
			un32QP = H264_Quantize_MAX;
		}
		flagFrame--;
		h264_mch_set_flagframe(ch, flagFrame);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (idr == 1) {
		frameCnt = 0;
		h264_mch_set_framecnt(ch, frameCnt);
	}

	if(bitctl_get_warning(ch,idr) != NONE)
	{
		printf("ENC WARNING");
		un32QP = (un32QP < (H264_Quantize_MAX)) ? un32QP+1 : H264_Quantize_MAX;
		enx_h264_set_quantize(un32QP, ch);
		return;
	}

	frameCnt = h264_mch_get_framecnt(ch);

	if (cbrmode == e_brmCBR) {	//CBR
//		if (h264_enc.max_frame / param->period == param->i_interval) { // IDR과 FPS가 같은 경우
//			if (h264_enc.max_frame == param->period) {	// 1fps(P가 없다)
			if (max_frame == period) {	// 1fps(P가 없다)
				// 이 경우 average식값과 i_predic_sz가 거의 같게 나와야 함
				if (i_predic_sz < bitrate) {
					un32QP = (un32QP > (H264_Quantize_MIN)) ? un32QP-1 : H264_Quantize_MIN;
				} else if (i_predic_sz > bitrate) {
					un32QP = (un32QP < (H264_Quantize_MAX)) ? un32QP+1 : H264_Quantize_MAX;
				}
			} else if (frameCnt == (i_interval - 1)) {	// i frame
				if (i_predic_sz < average) {
					un32QP = (un32QP > (H264_Quantize_MIN+1)) ? un32QP-2 : H264_Quantize_MIN;
				} else if (i_predic_sz > average) {
					un32QP = (un32QP < (H264_Quantize_MAX-1)) ? un32QP+2 : H264_Quantize_MAX;
				}
			} else {	// p frame
				if (p_predic_sz < average){
					un32QP = (un32QP > H264_Quantize_MIN) ? un32QP-1 : H264_Quantize_MIN;
				} else if(p_predic_sz > average){
					un32QP = (un32QP < H264_Quantize_MAX) ? un32QP+1 : H264_Quantize_MAX;
				}
			}
//		} else if (h264_enc.max_frame / param->period < param->i_interval){	// ex)30fps인데 idr:60인 경우(I가 없는 case)
//			printf("%s(%d) : bad case 1\r\n", __func__, __LINE__);
//		} else if (h264_enc.max_frame / param->period > param->i_interval){	// ex)30fps인데 idr:15인 경우(I가 여러개 case)
//			printf("%s(%d) : bad case 2\r\n", __func__, __LINE__);
//		}
	} else {	//VBR
		if (frameCnt == (i_interval - 1)) {
			un32QP = (un32QP < (H264_Quantize_MAX-1)) ? un32QP+2 : H264_Quantize_MAX;
		} else {
			if (p_predic_sz < average) {
				un32QP = (un32QP > enx_h264_get_default_quantize(ch)) ? un32QP-1 : enx_h264_get_default_quantize(ch);
			} else if (p_predic_sz > average) {
				un32QP = (un32QP < H264_Quantize_MAX) ? un32QP+1 : H264_Quantize_MAX;
			}
		}
	}

	if (un32QP > H264_Quantize_MAX) {
		printf("Bug!! Q over!! [%d]\r\n", un32QP);
		un32QP = H264_Quantize_MAX;
	}

	//printf("%dch type%d Q%d\r\n",ch, idr, un32QP);
	enx_h264_set_quantize(un32QP, ch);

	if (i_interval > 1) {
		frameCnt++;
		h264_mch_set_framecnt(ch, frameCnt);
	} else {
		frameCnt = 1;
		h264_mch_set_framecnt(ch, frameCnt);
	}
}

void enx_h264_info_print(void)
{
	int ch, i;
	unsigned int stp_size, buf_size, stm_size, tot_size;

	printf(" ---        H264 Information        ---\r\n");
	for(ch=0;ch<hmc.num_ch;ch++)
	{

		stp_size 	= (hmc.chi[ch].stpsize*MAX_Q_CH_NUM)+(hmc.chi[ch].stpinfosize*MAX_Q_CH_NUM);
		buf_size	=(hmc.chi[ch].ysize*MAX_RAW_NUM)+(hmc.chi[ch].csize*MAX_RAW_NUM)+ (hmc.chi[ch].ysize*2)+(hmc.chi[ch].csize*2);
		stm_size	= hmc.chi[ch].strmsize;
		tot_size	= stp_size + buf_size + stm_size;

		printf("Channel        : %d\r\n", ch);
		printf("├Param Info\r\n");
		printf("│├Image Size : %d x %d\r\n", hmc.chi[ch].width, hmc.chi[ch].height);
		printf("│├Stride Size: %d x %d\r\n", hmc.chi[ch].ystride, hmc.chi[ch].cstride);
		printf("│├ProfileIDC : %d(%s)\r\n", hmc.chi[ch].on_profile_idc, hmc.chi[ch].profile_idc == e_pBaseline ? "BASELINE" : hmc.chi[ch].profile_idc == e_pMain ? "MAIN" : "None");
		printf("│├LevelIDC   : %d\r\n", hmc.chi[ch].level_idc);
		printf("│├EntropyMode: %d(%s)\r\n", hmc.chi[ch].on_entropy, hmc.chi[ch].entropy == e_eCAVLC ? "CAVAL" : hmc.chi[ch].entropy == e_eCABAC ? "CABAC" : "None");
		if(hmc.chi[ch].entropy == e_eCABAC)
		printf("│├CabacIDC   : %d\r\n", hmc.chi[ch].on_cabac_idc);
		printf("│├I-Interval : %d\r\n", hmc.chi[ch].i_interval);
		printf("│├QP         : CUR(%d), PPS(%d)\r\n", hmc.chi[ch].cur_qp, hmc.chi[ch].pps_qp);
		printf("│├Bitrate Md : %s\r\n", hmc.chi[ch].brmode == e_brmQBR ? "QBR" : hmc.chi[ch].brmode == e_brmVBR ? "VBR" : hmc.chi[ch].brmode == e_brmCBR ? "CBR" : hmc.chi[ch].brmode == e_brmCVBR ? "CVBR" : "?");
		printf("│├Bitrate    : %d bps / %d Kbps / %d Mbps\r\n", hmc.chi[ch].brsize*8, (hmc.chi[ch].brsize*8) >> 10, (hmc.chi[ch].brsize*8) >> 20);
		printf("│├Frame rate : %d fps\r\n", hmc.max_fps/ hmc.chi[ch].period);
		printf("│└etc        : Srchmode(%d) MB(%d) cropbot(%d)\r\n", hmc.chi[ch].srchmode, hmc.chi[ch].num_mb, hmc.chi[ch].crop_b);
		printf("├Stream Info\r\n");
		printf("│├unit size  : STP(%dByte), INFO(%dByte), Count(%d)\r\n", hmc.chi[ch].stpsize, hmc.chi[ch].stpinfosize, MAX_Q_CH_NUM);
		for(i=0;i<MAX_Q_CH_NUM;i++)
		printf("│├STP%d       : STP(0x%08X~0x%08X), INFO(0x%08X~0x%08X)\r\n", i, hmc.chi[ch].stpsaddr[i], hmc.chi[ch].stpsaddr[i]+hmc.chi[ch].stpsize, hmc.chi[ch].stpiaddr[i], hmc.chi[ch].stpiaddr[i]+hmc.chi[ch].stpinfosize);
		printf("│└SIZE       : %d Bytes\r\n", stp_size);
		printf("├Buffer Info\r\n");
		printf("│├unit size  : Y(%dByte), C(%dByte)\r\n", hmc.chi[ch].ysize, hmc.chi[ch].csize);
		for(i=0;i<MAX_RAW_NUM;i++)
		printf("│├CUR%d      : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", i, hmc.chi[ch].cur[i].yaddr, hmc.chi[ch].cur[i].yaddr+hmc.chi[ch].ysize, hmc.chi[ch].cur[i].caddr, hmc.chi[ch].cur[i].caddr+hmc.chi[ch].csize);
		printf("│├REC       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", hmc.chi[ch].rec.yaddr, hmc.chi[ch].rec.yaddr+hmc.chi[ch].ysize, hmc.chi[ch].rec.caddr, hmc.chi[ch].rec.caddr+hmc.chi[ch].csize);
		printf("│├REF       : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", hmc.chi[ch].ref.yaddr, hmc.chi[ch].ref.yaddr+hmc.chi[ch].ysize, hmc.chi[ch].ref.caddr, hmc.chi[ch].ref.caddr+hmc.chi[ch].csize);
		printf("│└SIZE       : %d Bytes\r\n", buf_size);
		printf("├Stream Buffer\r\n");
		printf("│├BASE       : 0x%08X ~ 0x%08X\r\n", hmc.chi[ch].strmbase, hmc.chi[ch].strmendaddr);
		printf("│└SIZE       : %d Bytes\r\n", stm_size);
		printf("└Total Size     : %d Bytes\r\n", tot_size);
	}
	printf(" --------------------------------------\r\n");

}


#endif //__H264_MCH__
#endif	//

