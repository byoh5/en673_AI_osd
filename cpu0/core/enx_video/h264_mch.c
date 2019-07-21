#include "dev.h"
#include "dev_inline.h"	// for inline
#include "h264.h"
#include "enx_video.h"
#include "enx_os.h"
#include "msg.h"
#include "user.h"
#include "bitratectl.h"

#ifdef __ISP__
#include "dev_isp.h"
#include "dev_regs_isp_macro.h"
#endif

extern void H264_DEFAULT_REG_SET(void);
extern void BT0_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel);
extern void BT1_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel);
extern void DS0_OFF(void);
extern void DS1_OFF(void);
extern void DS0_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);
extern void DS1_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight);


#ifdef __H264_MCH__

#include "h264_mch.h"
#include "enx_mch_video.h"

sH264Param hmc;

void h264_mch_set_disable(void)
{
	//set h264 disable
	hmc.enable = 0;
}

void h264_mch_set_enable(void)
{
	//set h264 enable
	hmc.enable = 1;
}

int h264_mch_get_enable_state(void)
{
	//set h264 enable
	return hmc.enable;
}

void h264_mch_set_channel_number(int channel_number)
{
	hmc.num_ch = channel_number;
}


int h264_mch_get_channel_number(void)
{
	return hmc.num_ch;
}

void h264_mch_set_enc_state(int enc_state)
{
	hmc.enc_state = enc_state;
}


int h264_mch_get_enc_state(void)
{
	return hmc.enc_state;
}

void h264_mch_set_vlc_state(int vlc_state)
{
	hmc.vlc_state = vlc_state;
}

int h264_mch_get_vlc_state(void)
{
	return hmc.vlc_state;
}

int h264_mch_get_hold_state(void)
{
	return hmc.hold;
}

void h264_mch_set_hold_state(int state)
{
	hmc.hold = state;
}

int h264_mch_get_isp_outmode(void)
{
	return hmc.isp_outmode;
}

void h264_mch_set_isp_outmode(int mode)
{
	hmc.isp_outmode = mode;
}

void h264_mch_increase_hold_state(void)
{
	hmc.hold++;
}

void h264_mch_decrease_hold_state(void)
{
	hmc.hold--;
}

u32 h264_mch_get_isp_frame(void)
{
	return hmc.isp_frame;
}

void h264_mch_set_isp_frame(u32 frame)
{
	hmc.isp_frame = frame;
}

u32 h264_mch_get_isp_wdr(void)
{
	return hmc.isp_wdr;
}

void h264_mch_set_isp_dss(u32 dss)
{
	hmc.isp_dss = dss;
}

u32 h264_mch_get_isp_dss(void)
{
	return hmc.isp_dss;
}
void h264_mch_set_isp_wdr(u32 wdr)
{
	hmc.isp_wdr = wdr;
}

int h264_mch_get_hscale(int ch)
{
	return hmc.chi[ch].hscale;
}

int h264_mch_get_vscale(int ch)
{
	return hmc.chi[ch].vscale;
}

int h264_mch_get_raw_valid_num(int ch)
{
	return hmc.que[ch].q_validrawnum;
}

int h264_mch_get_queue_size(int ch)
{
	return hmc.que[ch].q_size;
}

void h264_mch_increase_raw_valid_num(int ch)
{
	hmc.que[ch].q_validrawnum++;
}

void h264_mch_decrease_raw_valid_num(int ch)
{
	hmc.que[ch].q_validrawnum--;
}

int h264_get_period(int ch)
{
	return hmc.chi[ch].period;
}

void h264_mch_increase_pic(int ch)
{
	hmc.chi[ch].pic++;
}

u32 h264_mch_get_interval_pic(int ch, int interval)
{
	if(hmc.chi[ch].pic%((UINT)interval) == 1)
		printf("pic(%d) ",hmc.chi[ch].pic);
	return hmc.chi[ch].pic;
}

int h264_mch_get_pic(int ch)
{
	return hmc.chi[ch].pic;
}

void h264_mch_set_vlock_ready_state(int ch, int ready)
{	
	hmc.chi[ch].vlock_ready = ready;
}

int h264_mch_get_vlock_ready_state(int ch)
{
	return hmc.chi[ch].vlock_ready;
}

void h264_mch_increase_vlock_ready_state(int ch)
{		
	hmc.chi[ch].vlock_ready++;
	
	//if(hmc.chi[ch].vlock_ready > MAX_Q_CH_NUM)
	//{
	//	printf("drop oldest raw\r\n");
	//	hmc.chi[ch].vlock_ready	= MAX_Q_CH_NUM;
	///}
}

void h264_mch_decrease_vlock_ready_state(int ch)
{	
	hmc.chi[ch].vlock_ready--;
}


UINT h264_mch_get_width(int ch)
{
	return hmc.chi[ch].width;
}	

UINT h264_get_height(int ch) 
{
	return hmc.chi[ch].height;
}

UINT h264_mch_get_sensor_width(void)
{
	return hmc.sen_width;
}

UINT h264_mch_get_sensor_height(void) 
{
	return hmc.sen_height;
}

int h264_mch_check_queue_enc_ready(int ch)
{
	int res;
	
	if(hmc.que[ch].q_validnum < hmc.que[ch].q_size)
		res = 1;
	else
		res = 0;
	
	return res;	
}

int h264_mch_get_queue_validnum(int ch)
{
	return hmc.que[ch].q_validnum;	
}

int h264_mch_check_queue_vlc_ready(int ch)
{
	int res;

	if(hmc.que[ch].q_validnum > 0)	
		res = 1;
	else 
		res = 0;

	return res;
}

int h264_mch_get_timestamp(int ch)
{
	return hmc.video_tms * hmc.chi[ch].period;
}

int h264_mch_get_default_timestamp(void)
{
	return hmc.video_tms;
}

void h264_mch_set_default_timestamp(int tms)
{
	hmc.video_tms = tms;
}

int h264_mch_check_raw_index(int ch)
{	
	if(hmc.que[ch].q_validrawnum < MAX_RAW_NUM 
		&& hmc.chi[ch].vlock_ready < MAX_Q_CH_NUM)
		return 1;
	else 
		return 0;
}

int h264_mch_get_enc_queue(int ch)
{
	return hmc.que[ch].q_eidx;    
}

int h264_mch_get_raw_queue(int ch)
{
	return hmc.que[ch].q_ridx;
}

void h264_mch_set_vlcdonetick(int ch, int val)
{
	hmc.chi[ch].vlcdone_tick = val;
}

void h264_mch_increase_vlcdonetick(int ch)
{
	hmc.chi[ch].vlcdone_tick++;		
	if (hmc.chi[ch].vlcdone_tick > 99999999) {
			hmc.chi[ch].vlcdone_tick = 0;
	}
}

uint32 h264_mch_get_vlcdonetick(int ch)
{
	return hmc.chi[ch].vlcdone_tick;
}

void h264_mch_update_raw_queue(int ch)
{
	hmc.que[ch].q_ridx++;
	
	if(hmc.que[ch].q_ridx >= (MAX_RAW_NUM))
		hmc.que[ch].q_ridx = 0;
}

void h264_mch_set_cur_yaddr(int ch, int idx,u32 addr)
{
	hmc.chi[ch].cur[idx].yaddr = addr;
}


void h264_mch_set_cur_caddr(int ch, int idx, u32 addr)
{
	hmc.chi[ch].cur[idx].caddr = addr;
}

u32 h264_mch_get_cur_yaddr(int ch, int idx)
{
	return hmc.chi[ch].cur[idx].yaddr;
}


u32 h264_mch_get_cur_caddr(int ch, int idx)
{
	return hmc.chi[ch].cur[idx].caddr;
}

void h264_mch_default_isp_register(void)
{
	H264_DEFAULT_REG_SET();
}


void init_mch_isp_resolution(int ch)
{

#ifdef __ISP__
	UINT sync_selection = 0, input_selection = 0, voffset_selection = 0;
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))	// Main ISP Input Image
	voffset_selection = 2;
	input_selection = 6;	// 6: Main ISP Image input
	sync_selection = 0;
#elif (IMAGE_INPUT_MODE==2)	// Second BT656/YC input Only
	input_selection = 5;	// 5: Second BT656/YC input image (Use Font/BOX OSD)
	sync_selection = 2;
#elif (IMAGE_INPUT_MODE==3)	// Main BT1120/YC input Only
	input_selection = 4;	// 4: Main BT1120/YC input image (Use Font/BOX OSD)
	sync_selection = 1;
#endif
	if(ch == 0)
	{
		if(h264_mch_get_sensor_width() == h264_mch_get_width(ch) 
			&& h264_mch_get_sensor_height() == h264_get_height(ch) )
		{
			BT1_SET(h264_mch_get_width(ch), input_selection, sync_selection);

			if(HW_DS0 == 0) {
				DS0_OFF();
			}
			else if(HW_DS1 == 0) {
				DS1_OFF();
			}
		}
		else {
			if(HW_DS0 == 0) {
				DS0_SET(sync_selection, voffset_selection, h264_mch_get_hscale(ch), h264_mch_get_vscale(ch), h264_mch_get_width(ch), h264_get_height(ch));
				BT1_SET(h264_mch_get_width(ch), 0, sync_selection);
			}
			else if(HW_DS1 == 0) {
				DS1_SET(sync_selection, voffset_selection, h264_mch_get_hscale(ch), h264_mch_get_vscale(ch), h264_mch_get_width(ch), h264_get_height(ch));
				BT1_SET(h264_mch_get_width(ch), 1, sync_selection);
			}
		}
	}
#if (IMAGE_INPUT_MODE!=1)
	else if(ch == 1)	// 2ch mode(sub)
	{
		if(h264_mch_get_sensor_width() == h264_mch_get_width(ch) 
			&& h264_mch_get_sensor_height() == h264_get_height(ch))
		{
			BT0_SET(h264_mch_get_width(ch), input_selection, sync_selection);

			if(HW_DS0 == 1) {
				DS0_OFF();
			}
			else if(HW_DS1 == 1) {
				DS1_OFF();
			}
		}
		else {
			if(HW_DS0 == 1) {
				DS0_SET(sync_selection, voffset_selection, h264_mch_get_hscale(ch), h264_mch_get_vscale(ch), h264_mch_get_width(ch), h264_get_height(ch));
				BT0_SET(h264_mch_get_width(ch), 0, sync_selection);
			}
			else if(HW_DS1 == 1) {
				DS1_SET(sync_selection, voffset_selection, h264_mch_get_hscale(ch), h264_mch_get_vscale(ch), h264_mch_get_width(ch), h264_get_height(ch));
				BT0_SET(h264_mch_get_width(ch), 1, sync_selection);
			}
		}
	}
#endif
	else {
		printf("%s(%d) : ch(%d) size change(%d, %d)???\r\n", __func__, __LINE__, ch, h264_mch_get_width(ch), h264_get_height(ch));
	}
#endif	//__ISP__

}

#ifdef __H264_CHECK_TIME__

void h264_mch_check_enc_starttime(void)
{	
	u32 mtime;	
	UINT dep;

	//set start tic time
	hmc.start_clk_h264 = mfspr(EXR1K_HTBTR);
	
	if(hmc.stop_clk_h264 < hmc.start_clk_h264)
		dep = hmc.start_clk_h264 - hmc.stop_clk_h264;
	else
		dep = (0xFFFFFFFF - hmc.stop_clk_h264) + hmc.start_clk_h264;

	mtime = (dep)/(EXR1K_CLK / 1000);
	
	if(mtime > 20)
		jprintf("ENC gap %dms\r\n", mtime);
	
}

void h264_mch_check_vlc_starttime(void)
{	
	u32 mtime;	
	
	//set start tic time
	hmc.start_clk_vlc = mfspr(EXR1K_HTBTR);
	mtime = (hmc.start_clk_vlc - hmc.stop_clk_vlc)/(EXR1K_CLK / 1000);
	
	//jprintf("VLC gap %dms\r\n", mtime);
}

void h264_mch_check_enc_endtime(int ch)
{
	u32 mtime;	
	UINT dep;
	int idx;
	idx = hmc.que[ch].q_vidx;
	
	//check end time
	hmc.stop_clk_h264 = mfspr(EXR1K_HTBTR);
	
	if(hmc.start_clk_h264 < hmc.stop_clk_h264)
		dep = hmc.stop_clk_h264 - hmc.start_clk_h264;
	else
		dep = (0xFFFFFFFF - hmc.start_clk_h264) + hmc.stop_clk_h264;

	mtime = (dep)/(EXR1K_CLK / 1000);

	if(mtime > 30)
		jprintf("!%dch ENC end time %dms\r\n", ch, mtime);
	
	
#ifdef VLC_CONTROL_Q
	if(/*mtime > 28 && */h264_mch_get_raw_valid_num(ch) > 2 && VLCControlQ[ch] == 0)
	{	
		int buffergap = h264_mch_get_raw_valid_num(ch) - 2; //1,2,3,4
		int timegap = mtime - 24;//20	//0,1,2,3,4,5,6,7,8,9,10....

		if(timegap < 0)
			timegap = 0;

		if(hmc.chi[ch].slice_type[idx] == 0)//Iframe
		{
			VLCControlQ[ch] = buffergap + timegap;
		}
		else
		{
			VLCControlQ[ch] = buffergap + (timegap/3);
		}
		VLCControlQ[ch] |= (ch<<7); 	
		VLCControlQ[ch] |= (hmc.chi[ch].slice_type[idx]<<6);
		
		jprintf("!Set High %dch slice %d +Q%d\n", ((VLCControlQ[ch]&0x80)>>7),((VLCControlQ[ch]&0x40)>>6), (VLCControlQ[ch]&0x3F));
	}
#endif	//VLC_CONTROL_Q

#ifdef __ISP__
#ifdef __HW_OSD_H264__
		// OSD Log: Encoding Time

	static int buf_len2[2][2] = {{0}};
	if (gptMsgDebug.H264_POLL_LOG & 0x14) {

		UINT time_us;
		time_us = dep/(BUS_CLK/1000000);

		if(gptMsgDebug.H264_POLL_LOG & 0x4) {
			printf("H.264(%d) ENC(%dus)\r\n", H264_MCHNNEL, time_us);
		}

		if (gptMsgDebug.H264_POLL_LOG & 0x10) {
			const char output_set[6][6] = {"30NOR", "15WDR", "30DNR", "60NOR", "30WDR", "60DNR"};
			if (hmc.chi[ch].slice_type[idx] == 0) {
				char buffer[40] = {0};
				buf_len2[ch][0] = sprintf(buffer, "%d-%s-%s IDR(%d)", ch, H264_PROFILEIDC == e_pBaseline ? "BP" : H264_PROFILEIDC == e_pMain ? "MP" : "xx", output_set[MP(FrameMode)], hmc.chi[ch].i_interval);
				DispStr(buffer, 10+(ch*2), 0, buf_len2[ch][0]);
				if (gptMsgDebug.H264_TIME_CNT[ch] > 0) {
					buf_len2[ch][1] = sprintf(buffer, "Avg ENC:%dus Max:%dus", gptMsgDebug.H264_TIME_SUM[ch] / gptMsgDebug.H264_TIME_CNT[ch], gptMsgDebug.H264_TIME_MAX[ch]);
					DispStr(buffer, 11+(ch*2), 0, buf_len2[ch][1]);
				}
				gptMsgDebug.H264_TIME_SUM[ch] = 0;
				gptMsgDebug.H264_TIME_CNT[ch] = 0;
				gptMsgDebug.H264_TIME_MAX[ch] = 0;
			}
			gptMsgDebug.H264_TIME_CNT[ch] += 1;
			gptMsgDebug.H264_TIME_SUM[ch] += time_us;
			if(gptMsgDebug.H264_TIME_MAX[ch] < time_us)
				gptMsgDebug.H264_TIME_MAX[ch] = time_us;
		} else {
			if (buf_len2[ch][0]) {
				DispClr(10+(ch*2), 0, buf_len2[ch][0]);
				DispClr(11+(ch*2), 0, buf_len2[ch][1]);
				buf_len2[ch][0] = 0;
				buf_len2[ch][1] = 0;
			}
		}
	} else {
		if (buf_len2[ch][0]) {
			DispClr(10+(ch*2), 0, buf_len2[ch][0]);
			DispClr(11+(ch*2), 0, buf_len2[ch][1]);
			buf_len2[ch][0] = 0;
			buf_len2[ch][1] = 0;
		}
	}
#endif
#endif

}

void h264_mch_check_vlc_endtime(int ch)
{
	int mtime;	
	int idx;
	int dep;
		
	//set end time
	hmc.stop_clk_vlc = mfspr(EXR1K_HTBTR);	
	
	if(hmc.start_clk_vlc < hmc.stop_clk_vlc)
		dep = hmc.stop_clk_vlc - hmc.start_clk_vlc;
	else
		dep = (0xFFFFFFFF - hmc.start_clk_vlc) + hmc.stop_clk_vlc;

	mtime = (dep)/(EXR1K_CLK / 1000);
	
	idx = hmc.que[ch].q_vidx;

	if(mtime > 20)
		jprintf("!%dch VLC end time slice_type %d %dms\r\n", ch, hmc.chi[ch].slice_type[idx] , mtime);
	
#ifdef VLC_CONTROL_Q
	if(/*mtime > 28 && */h264_mch_get_raw_valid_num(ch) > 2 && VLCControlQ[ch] == 0)
	{	
		int buffergap = h264_mch_get_raw_valid_num(ch) - 2; //1,2,3,4
		int timegap = mtime - 30;//20	//0,1,2,3,4,5,6,7,8,9,10....

		if(timegap < 0)
			timegap = 0;

		if(hmc.chi[ch].slice_type[idx] == 0)//Iframe
		{
			VLCControlQ[ch] = buffergap + timegap;
		}
		else
		{
			VLCControlQ[ch] = buffergap + (timegap/3);
		}
		VLCControlQ[ch] |= (ch<<7);		
		VLCControlQ[ch] |= (hmc.chi[ch].slice_type[idx]<<6);
		
		jprintf("!Set High %dch slice %d +Q%d\n", ((VLCControlQ[ch]&0x80)>>7),((VLCControlQ[ch]&0x40)>>6), (VLCControlQ[ch]&0x3F));
	}
#endif	//VLC_CONTROL_Q

#ifdef __ISP__
#ifdef __HW_OSD_H264__
		// OSD Log: Encoding Time

	static int buf_len2[2][2] = {{0}};
	if (gptMsgDebug.H264_POLL_LOG & 0x14) {
				
		UINT time_us;
		time_us = dep/(BUS_CLK/1000000);

		if(gptMsgDebug.H264_POLL_LOG & 0x4) {
			printf("H.264(%d) VLC(%dus)\r\n", H264_MCHNNEL, time_us);
		}

		if (gptMsgDebug.H264_POLL_LOG & 0x10) {
			const char output_set[6][6] = {"30NOR", "15WDR", "30DNR", "60NOR", "30WDR", "60DNR"};
			if (H264_MPFRAME == 0) {
				char buffer[40] = {0};
				buf_len2[ch][0] = sprintf(buffer, "%d-%s-%s IDR(%d)", ch, H264_PROFILEIDC == e_pBaseline ? "BP" : H264_PROFILEIDC == e_pMain ? "MP" : "xx", output_set[MP(FrameMode)], hmc.chi[ch].i_interval);
				DispStr(buffer, 14+(ch*2), 0, buf_len2[ch][0]);
				if (gptMsgDebug.H264_VLCTIME_CNT[ch] > 0) {
					buf_len2[ch][1] = sprintf(buffer, "Avg VLC:%dus Max:%dus", gptMsgDebug.H264_VLCTIME_SUM[ch] / gptMsgDebug.H264_VLCTIME_CNT[ch], gptMsgDebug.H264_VLCTIME_MAX[ch]);
					DispStr(buffer, 15+(ch*2), 0, buf_len2[ch][1]);
				}
				gptMsgDebug.H264_VLCTIME_SUM[ch] = 0;
				gptMsgDebug.H264_VLCTIME_CNT[ch] = 0;
				gptMsgDebug.H264_VLCTIME_MAX[ch] = 0;
			}
			gptMsgDebug.H264_VLCTIME_CNT[ch] += 1;
			gptMsgDebug.H264_VLCTIME_SUM[ch] += time_us;
			if(gptMsgDebug.H264_VLCTIME_MAX[ch] < time_us)
				gptMsgDebug.H264_VLCTIME_MAX[ch] = time_us;
		} else {
			if (buf_len2[ch][0]) {
				DispClr(14+(ch*2), 0, buf_len2[ch][0]);
				DispClr(15+(ch*2), 0, buf_len2[ch][1]);
				buf_len2[ch][0] = 0;
				buf_len2[ch][1] = 0;
			}
		}
	} else {
		if (buf_len2[ch][0]) {
			DispClr(14+(ch*2), 0, buf_len2[ch][0]);
			DispClr(15+(ch*2), 0, buf_len2[ch][1]);
			buf_len2[ch][0] = 0;
			buf_len2[ch][1] = 0;
		}
	}
#endif
#endif

}
#endif	//__H264_CHECK_TIME__

int h264_mch_check_enc_end_status_res(int status)
{
	int res;
	
	res = status & H264_STATUS_ENCEND;
		
	return res;
}

int h264_mch_check_vlc_end_status_res(int status)
{
	int res;
	
	res = status & H264_STATUS_VLCEND;
	
	return res;
}

int h264_mch_get_slice_type_res(void)
{
	int slice;
	
	slice = (H264_MPFRAME == 0) ? 1 : 0;
	
	return slice;
}

int h264_mch_get_enc_channel_number_res(void)
{
	int ch;
	
	ch = H264_ECHNNEL;
	
	return ch;
}

int h264_mch_get_vlc_channel_number_res(void)
{
	int ch;
	
	ch = H264_MCHNNEL;
	
	return ch;
}

int h264_mch_get_status_res(void)
{
	int status;
	
	status = H264_STATUS_T;
	H264_STATUS_T = status & 0xFFFF;
	
	return status;
}

u32 h264_mch_get_prebits_res(void)
{
	return (H264_PREBITS >> 3);
}

u32 h264_mch_get_stream_address_res(void)
{
	u32 addr;
	
 	addr = (H264_STRMBUF);
	
	return addr;
}

u32 h264_mch_get_genbits_res(void)
{
	return (H264_GENBITS >> 3);
}

int h264_mch_get_vlc_quant_res(void)
{
	return H264_MQSCALE;
}

void h264_mch_init_queue(int ch, int q_start, int q_end)
{
	//printf("%s \r\n",__func__);
	hmc.que[ch].q_ridx  = q_start;
	hmc.que[ch].q_eidx  = q_start;
	hmc.que[ch].q_vidx  = q_start;
	hmc.que[ch].q_start = q_start;
	hmc.que[ch].q_end   = q_end;
	hmc.que[ch].q_validnum = 0;
	hmc.que[ch].q_size  = q_end - q_start + 1; // 0~4 => 5
	//printf("queue ch %d start %d end %d \r\n",ch,q_start,q_end);
}


void h264_mch_init_vlc_validnum(int ch)
{
	//printf("%s \r\n",__func__);
	hmc.que[ch].q_validnum = 0;
}

int h264_mch_set_queue(int ch, int slicetype)
{
	int eidx;
	
	//printf("%s \r\n",__func__);
	
	eidx = hmc.que[ch].q_eidx;

	hmc.que[ch].pic[eidx]       = hmc.chi[ch].pic;
	//hmc.que[ch].time[eidx]        = time;
	hmc.que[ch].slicetype[eidx] = slicetype;
	hmc.que[ch].qp[eidx]        = hmc.chi[ch].cur_qp;
	hmc.chi[ch].stpsaddr[eidx]  = hmc.chi[ch].stpsaddr[eidx];
	hmc.chi[ch].stpiaddr[eidx]  = hmc.chi[ch].stpiaddr[eidx];

	//jprintf("ENC queue ch %d IDX %d pic %d slicetype %d qp %d stpaddress %x stpinfoaddress %x \r\n",ch, eidx, pic, slicetype, qp, stpsaddr, stpiaddr);

	return eidx;	
}

int h264_mch_set_update_queue(int ch)
{

	hmc.que[ch].q_validnum += 1;


	hmc.que[ch].q_eidx += 1;
	if(hmc.que[ch].q_eidx > hmc.que[ch].q_end) {
		hmc.que[ch].q_eidx = hmc.que[ch].q_start;
	}

	//jprintf("ENC update ch %d eidx %d \r\n", ch, hmc.que[ch].q_eidx);
	return hmc.que[ch].q_eidx;

}

void h264_mch_init_enc_queue(int ch)
{
	hmc.que[ch].q_eidx = hmc.que[ch].q_start;
}

int h264_mch_get_queue_index(int ch)
{
	return hmc.que[ch].q_vidx;    
}


int h264_mch_get_queue(int ch, int *pic, int *slicetype, int *qp, unsigned int *stpsaddr, unsigned int *stpiaddr)
{
    int vidx;
	
	//printf("%s \r\n",__func__);
    
    vidx = hmc.que[ch].q_vidx;
    
    *pic        = hmc.que[ch].pic[vidx];   
    //*time       = hmc.que[ch].time[vidx];   
    *slicetype  = hmc.que[ch].slicetype[vidx];   
    *qp         = hmc.que[ch].qp[vidx];   
    *stpsaddr   = hmc.chi[ch].stpsaddr[vidx];   
    *stpiaddr   = hmc.chi[ch].stpiaddr[vidx];   
	
	//jprintf("VLC VIDX %d get queue ch %d  pic %d slicetype %d qp %d stpsaddr %x stpiadd %x\r\n",vidx, ch,  *pic, *slicetype,*qp, *stpsaddr, *stpiaddr);
	
	return vidx;    
}

void h264_mch_init_vlc_queue(int ch)
{
	hmc.que[ch].q_vidx  = hmc.que[ch].q_start;
}

int h264_mch_get_update_queue(int ch)
{
   	hmc.que[ch].q_validnum -= 1;

	hmc.que[ch].q_vidx += 1;
	if(hmc.que[ch].q_vidx > hmc.que[ch].q_end) {
		hmc.que[ch].q_vidx = hmc.que[ch].q_start;
	}

	return hmc.que[ch].q_vidx;
}

void h264_mch_update_stream_size(int ch)
{
	hmc.chi[ch].strsize = (H264_GENBITS >> 3);
}

void h264_mch_update_strbuffer(int ch, int strsize)
{
	//hmc.chi[ch].strmaddr += H264_ALIGN(hmc.chi[ch].strsize);
	hmc.chi[ch].strmaddr += H264_ALIGN(strsize/*hmc.chi[ch].strsize*/);
	
	if((hmc.chi[ch].strmaddr + H264_ALIGN(hmc.chi[ch].stpsize)) >= hmc.chi[ch].strmendaddr)
	{
 		hmc.chi[ch].strmaddr = H264_ALIGN(hmc.chi[ch].strmbase);
	}
}

int h264_mch_get_framecnt(int ch)
{
	return hmc.chi[ch].frameCnt;
}

void h264_mch_set_framecnt(int ch, int cnt)
{
	hmc.chi[ch].frameCnt = cnt;
}

int h264_mch_get_flagframe(int ch)
{
	return hmc.chi[ch].flagFrame;
}

void h264_mch_set_flagframe(int ch, int cnt)
{
	hmc.chi[ch].flagFrame = cnt;
}

void h264_mch_init_param(int ch, int width, int height, int i_interval, int period, int resolution, int qp, int srchmode, int profile, int brmode, int brsize, int maxfps, int timestamp)
{
	unsigned int NRMMBNUM, LSTMBNUM, LSTSIZE;
		
#ifdef __ISP__
	MYCW_HWOw(width+8);
#endif

	hmc.sen_width 			= width;
	hmc.sen_height 			= height;	
	hmc.max_fps				= maxfps;  
	hmc.video_tms 			= timestamp;	
	
	enx_h264_set_frame_size_rs(resolution, ch);
	
	hmc.chi[ch].level_idc   = LEVEL_IDC;		
	hmc.chi[ch].resolution	= resolution;  
	hmc.chi[ch].hscale 		= (hmc.sen_width * 64) / hmc.chi[ch].width;
	hmc.chi[ch].vscale 		= (hmc.sen_height * 64) / hmc.chi[ch].height;	
	hmc.chi[ch].i_interval  = i_interval;  		
	hmc.chi[ch].cur_qp      = qp;  
	hmc.chi[ch].pps_qp      = qp;  	
	hmc.chi[ch].srchmode    = srchmode;   	
	hmc.chi[ch].brmode	 	= brmode;	
	hmc.chi[ch].pic		 	= 0;	
	
	enx_h264_set_bit_rate(brsize, ch);		
	enx_h264_set_frame_rate_sf(period, ch);	
	enx_h264_set_profile(profile, ch);

	NRMMBNUM = ((hmc.chi[ch].height+15)/16) * 8;
	LSTSIZE  = ((hmc.chi[ch].width&0x78)==0) ? 8 : ((hmc.chi[ch].width&0x70)>>4) + ((hmc.chi[ch].width&0x8)>>3);
    LSTMBNUM = ((hmc.chi[ch].height+15)/16) * LSTSIZE;
    hmc.chi[ch].num_mb = ((hmc.chi[ch].width+15)/16) * ((hmc.chi[ch].height+15)/16);
		
	//printf("num_mb %d width %d height %d \r\n", hmc.chi[ch].num_mb, hmc.chi[ch].width, hmc.chi[ch].height);	

    if(hmc.chi[ch].height&0x0f) hmc.chi[ch].crop_b = 1;
	else                        hmc.chi[ch].crop_b = 0; 
	
	//printf("crop_b %d height %d \r\n", hmc.chi[ch].num_mb, hmc.chi[ch].height);	

	hmc.chi[ch].vlock_ready = 0;

#ifdef __H264_CHECK_TIME__	
	hmc.start_clk_h264 = 0;
	hmc.stop_clk_h264 = 0;
	hmc.start_clk_vlc = 0;
	hmc.stop_clk_vlc = 0;
	hmc.tmp_clk_h264 = 10000;
	hmc.tmp_clk_vlc = 1000;
#endif

	//jprintf("ch %d sen_width %d sen_height %d level_idc %d resolution %d \r\n hscale %d vscale %d i_interval %d cur_qp %d pps_qp %d srchmode %d brmode %d \r\nbrsize %d pic %d max_fps %d video_tms %d num_mb %d crop_b %d\r\n",ch,
	//	hmc.sen_width,	hmc.sen_height,hmc.chi[ch].level_idc,hmc.chi[ch].resolution,hmc.chi[ch].hscale,hmc.chi[ch].vscale,hmc.chi[ch].i_interval,hmc.chi[ch].cur_qp,hmc.chi[ch].pps_qp,hmc.chi[ch].srchmode,hmc.chi[ch].brmode,
	//	hmc.chi[ch].brsize,hmc.chi[ch].pic,hmc.max_fps,hmc.video_tms,hmc.chi[ch].num_mb,hmc.chi[ch].crop_b);

	//bitctl_init_seq(ch, hmc.chi[ch].brsize, hmc.max_fps, hmc.chi[ch].cur_qp, hmc.chi[ch].i_interval, (hmc.chi[ch].width/16), (hmc.chi[ch].height/16));		
	bitctl_init_seq(ch);

}


void h264_mch_init_buffer(void)
{
	int i;
	
	int mch;

	u32 ysize, csize, ysize_a, csize_a;
	unsigned int paddr;
	unsigned int streamsize;

	
	int ch;
	unsigned int buf_addr;
	unsigned int max_stp_size;
	unsigned int max_stp_info_size;
	unsigned int max_strm_size;
	unsigned int max_strm_end;

	//ch0 stp size, stream buffer size//////////////////
//#ifdef BANK0_USE_STREAM
//	buf_addr			= BANK0_STREAM_BASE;
//#else
	buf_addr			= DDR_BK1_BASE;
//#endif

	ch					= 0;
	max_stp_size		= 0x200000;//2M	0x120000;//1.125Mbyte  //0x150000; //1.5M byte   
	max_stp_info_size	= 0x2000;
	////////////////////////////////////////////////////
	
	//printf("%s ch%d \r\n",__func__, ch);

	//printf("=======================================================================\r\n");
	//printf("ch %d buf_addr 0x%x \r\nmax_stp_size 0x%x \r\nwidth %d height %d\r\n", 
	//	ch, buf_addr, max_stp_size, hmc.chi[ch].width, hmc.chi[ch].height);

	//printf("=======================================================================\r\n");

	paddr			= buf_addr;
	UINT reconstruction_buffer_size;

	ysize = ((hmc.chi[ch].width + 32) >> 4) << 4;
	csize = ((hmc.chi[ch].height + 32) >> 4) << 4;
	reconstruction_buffer_size = ysize * csize;

	ysize_a = H264_ALIGN(reconstruction_buffer_size);
	csize_a = H264_ALIGN(reconstruction_buffer_size >> 1);
	
	hmc.chi[ch].ysize = ysize_a;
	hmc.chi[ch].csize = csize_a;
	
	//printf("ysize 0x%x csize 0x%x \r\nysize_a 0x%x csize_a 0x%x \r\n",
	//	ysize,csize,ysize_a,csize_a); 

	//printf("=======================================================================\r\n");


	//set cur y 0~6
	for(mch=0;mch<MAX_Q_CH_NUM; mch++)
	{
		//hmc.chi[ch].cur[mch].yaddr	= H264_ALIGN(paddr);
		//paddr = H264_ALIGN(hmc.chi[ch].cur[mch].yaddr + ysize_a); 
		hmc.chi[ch].cur[mch].yaddr	= H264_ALIGN(paddr + ((mch%MAX_RAW_NUM)*(ysize_a)));	
		//printf("%dth mch cur y addr 0x%x \r\n", mch, hmc.chi[ch].cur[mch].yaddr);
			
	}
	paddr = H264_ALIGN(paddr + (ysize_a*MAX_RAW_NUM));	
	//printf("=======================================================================\r\n");

	// stream buffer;
	max_strm_end	= DDR_BK3_BASE - (((csize_a)*MAX_RAW_NUM) + ((max_stp_size + max_stp_info_size)*MAX_Q_CH_NUM)) - 0x1000; //0x400000+0x80000;//4M byte
	max_strm_size	= max_strm_end - H264_ALIGN(paddr);
		
	hmc.chi[ch].strmbase		= H264_ALIGN(paddr);
	hmc.chi[ch].strmendaddr 	= H264_ALIGN(max_strm_end);

	hmc.chi[ch].strmaddr		= hmc.chi[ch].strmbase;
	hmc.chi[ch].strmsize		= max_strm_size;
		
	paddr = hmc.chi[ch].strmendaddr;

	//printf("strm base ddr 0x%x \r\n strmend end addr 0x%x \r\nmax_strm_size 0x%x\r\n",
	//	hmc.chi[ch].strmbase, hmc.chi[ch].strmendaddr, max_strm_size);

	//printf("=======================================================================\r\n");



	//set stp 
	for(i=hmc.que[ch].q_start; i<=hmc.que[ch].q_end; i++) {

		streamsize					= max_stp_size;
		streamsize					= ((streamsize + 1023)/1024)*1024;

		hmc.chi[ch].stpsaddr[i] 	= H264_ALIGN(paddr);
		hmc.chi[ch].stpiaddr[i] 	= H264_ALIGN(hmc.chi[ch].stpsaddr[i] + streamsize);
		paddr						= H264_ALIGN(hmc.chi[ch].stpiaddr[i] + max_stp_info_size);//info 8K byte
		//printf("stp[%d] addr 0x%x stp[%d]info addr 0x%x \r\n", 
		//	i, hmc.chi[ch].stpsaddr[i], i, hmc.chi[ch].stpiaddr[i]);
	}

	hmc.chi[ch].stpsize = max_stp_size;
	hmc.chi[ch].stpinfosize = max_stp_info_size;
	//printf("stp size 0x%x \r\n",hmc.chi[ch].stpsize);

	//printf("=======================================================================\r\n");

	//if(hmc.chi[ch].strmendaddr > DDR_BK2_BASE)
	//{
	//	printf("hmc.chi[ch].strmendaddr %x > DDR_BK2_BASE %x !!!!!!!!!!!!!!\r\n", hmc.chi[ch].strmendaddr, DDR_BK2_BASE);
	//	while(1);
	//}
	//else
	//{
	//	printf("%x ~ %x temp %x \r\n", hmc.chi[ch].strmendaddr, DDR_BK2_BASE, DDR_BK2_BASE - hmc.chi[ch].strmendaddr);
	//}
	//set cur c 0~6
	//paddr = DDR_BK2_BASE+4096;
	
	for(mch=0;mch<MAX_Q_CH_NUM; mch++)
	{
		//hmc.chi[ch].cur[mch].caddr	= H264_ALIGN(paddr);
		//paddr = H264_ALIGN(hmc.chi[ch].cur[mch].caddr + csize_a); 	
		hmc.chi[ch].cur[mch].caddr	= H264_ALIGN(paddr + ((mch%MAX_RAW_NUM)*(csize_a)));	
		//printf("%dth mch cur c addr 0x%x \r\n", mch, hmc.chi[ch].cur[mch].caddr);
			
	}
	paddr = H264_ALIGN(paddr + (csize_a*MAX_RAW_NUM));	
	//printf("=======================================================================\r\n");







	
	if(paddr > DDR_BK3_BASE)
	{
		printf("stp end paddr 0x%x > DDR_BK3_BASE addr 0x%x !!!!!!!!!!!!!!!!!!\r\n", 
			paddr, DDR_BK3_BASE);
		while(1);
	}
	//else
	//{
	//	printf("addr 0x%x ~ addr 0x%x temp size 0x%x \r\n", 
	//		paddr, DDR_BK3_BASE, DDR_BK3_BASE - paddr);
	//}


	paddr = DDR_BK3_BASE;
	// recon
	hmc.chi[ch].rec.yaddr	= H264_ALIGN(paddr);
	//hmc.chi[ch].rec.uaddr = hmc.chi[ch].rec.yaddr + ysize_a;
	hmc.chi[ch].rec.caddr	= H264_ALIGN(hmc.chi[ch].rec.yaddr + ysize_a);
	
	paddr = H264_ALIGN(hmc.chi[ch].rec.caddr + csize_a);
	
	// reference 
	hmc.chi[ch].ref.yaddr	= H264_ALIGN(paddr);
	//hmc.chi[ch].ref.uaddr 	= hmc.chi[ch].ref.yaddr + ysize_a;
	hmc.chi[ch].ref.caddr	= H264_ALIGN(hmc.chi[ch].ref.yaddr + ysize_a);

	paddr = H264_ALIGN(hmc.chi[ch].ref.caddr + csize_a);

	//printf("rec y addr 0x%x rec c addr 0x%x \r\nref y addr 0x%x ref c addr 0x%x \r\n",
	//	hmc.chi[ch].rec.yaddr,hmc.chi[ch].rec.caddr,hmc.chi[ch].ref.yaddr,hmc.chi[ch].ref.caddr); 

	//printf("=======================================================================\r\n");

	

	
#ifdef __H264_2CH__	

	//ch0 stp size, stream buffer size//////////////////
	ch = 1;
	max_stp_size		= 0x100000; //0xC0000;//832K byte	//0x100000; //0x80000;///0xC0000;//832K byte //0x5A000;//360K byte
	max_stp_info_size	= 0x2000;
	////////////////////////////////////////////////////
	
	//printf("\r\n%s ch %d\r\n",__func__, ch);

	//printf("=======================================================================\r\n");
	//printf("ch %d paddr 0x%x \r\nmax_stp_size 0x%x \r\nwidth %d height %d\r\n", 
	//	ch, paddr, max_stp_size, hmc.chi[ch].width, hmc.chi[ch].height);

	//printf("=======================================================================\r\n");



	ysize = ((hmc.chi[ch].width + 32) >> 4) << 4;
	csize = ((hmc.chi[ch].height + 32) >> 4) << 4;
	reconstruction_buffer_size = ysize * csize;

	ysize_a = H264_ALIGN(reconstruction_buffer_size);
	csize_a = H264_ALIGN(reconstruction_buffer_size >> 1);
	
	hmc.chi[ch].ysize = ysize_a;
	hmc.chi[ch].csize = csize_a;
	
	//printf("ysize 0x%x csize 0x%x \r\nysize_a 0x%x csize_a 0x%x \r\n",
	//	ysize,csize,ysize_a,csize_a); 

	//printf("=======================================================================\r\n");

	
	// recon
	hmc.chi[ch].rec.yaddr	= H264_ALIGN(paddr);
	//hmc.chi[ch].rec.uaddr = hmc.chi[ch].rec.yaddr + ysize_a;
	hmc.chi[ch].rec.caddr	= H264_ALIGN(hmc.chi[ch].rec.yaddr + ysize_a);
	
	paddr = H264_ALIGN(hmc.chi[ch].rec.caddr + csize_a);
	
	// reference 
	hmc.chi[ch].ref.yaddr	= H264_ALIGN(paddr);
	//hmc.chi[ch].ref.uaddr 	= hmc.chi[ch].ref.yaddr + ysize_a;
	hmc.chi[ch].ref.caddr	= H264_ALIGN(hmc.chi[ch].ref.yaddr + ysize_a);

	paddr = H264_ALIGN(hmc.chi[ch].ref.caddr + csize_a);

	//printf("rec y addr 0x%x rec c addr 0x%x \r\nref y addr 0x%x ref c addr 0x%x \r\n",
	//	hmc.chi[ch].rec.yaddr,hmc.chi[ch].rec.caddr,hmc.chi[ch].ref.yaddr,hmc.chi[ch].ref.caddr); 

	//printf("=======================================================================\r\n");

	


	//set cur y,c 0~6
	for(mch=0;mch<MAX_Q_CH_NUM; mch++)
	{
		//hmc.chi[ch].cur[mch].yaddr	= H264_ALIGN(paddr);
		//hmc.chi[ch].cur[mch].caddr	= H264_ALIGN(hmc.chi[ch].cur[mch].yaddr + ysize_a);
		//paddr = H264_ALIGN(hmc.chi[ch].cur[mch].caddr + csize_a);
		
		hmc.chi[ch].cur[mch].yaddr	= H264_ALIGN(paddr + ((mch%MAX_RAW_NUM)*(ysize_a + csize_a)));
		hmc.chi[ch].cur[mch].caddr	= H264_ALIGN(hmc.chi[ch].cur[mch].yaddr + ysize_a);
		//printf("%dth mch cur y addr 0x%x cur c addr 0x%x \r\n", 
		//	mch, hmc.chi[ch].cur[mch].yaddr, hmc.chi[ch].cur[mch].caddr);
			
	}
	paddr = H264_ALIGN(paddr + ((ysize_a + csize_a)*MAX_RAW_NUM));

	//printf("=======================================================================\r\n");

	//set stp 
	for(i=hmc.que[ch].q_start; i<=hmc.que[ch].q_end; i++) {

		streamsize			= max_stp_size;
		streamsize			= ((streamsize + 1023)/1024)*1024;

		hmc.chi[ch].stpsaddr[i] 	= H264_ALIGN(paddr);
		hmc.chi[ch].stpiaddr[i] 	= H264_ALIGN(hmc.chi[ch].stpsaddr[i] + streamsize);
		paddr						= H264_ALIGN(hmc.chi[ch].stpiaddr[i] + max_stp_info_size);//info 8K byte
		//printf("stp[%d] addr 0x%x stp[%d]info addr 0x%x \r\n", i, hmc.chi[ch].stpsaddr[i], i, hmc.chi[ch].stpiaddr[i]);
	}

	hmc.chi[ch].stpsize 	= max_stp_size;
	hmc.chi[ch].stpinfosize = max_stp_info_size;
	//printf("stp size 0x%x \r\n",hmc.chi[ch].stpsize);

	//printf("=======================================================================\r\n");
	// stream buffer;
	max_strm_size		= (DDR_BK0_BASE + DDR_SIZE) - H264_ALIGN(paddr) - 0x1000;//2M byte
#ifdef BANK0_USE_STREAM
	paddr			= H264_ALIGN(BANK0_STREAM_BASE);
	max_strm_size  	= BANK0_STREAM_SIZE;
#endif
	hmc.chi[ch].strmbase		= H264_ALIGN(paddr);
	hmc.chi[ch].strmendaddr 	= H264_ALIGN(paddr + (max_strm_size));

	hmc.chi[ch].strmaddr	= hmc.chi[ch].strmbase;	
	hmc.chi[ch].strmsize		= max_strm_size;
	
	paddr = hmc.chi[ch].strmendaddr;

	//printf("strm base addr 0x%x \r\n strmend end addr 0x%x max_strm_size 0x%x\r\n",
	//	hmc.chi[ch].strmbase, hmc.chi[ch].strmendaddr, max_strm_size);

	//printf("=======================================================================\r\n");
	
	if(paddr > DDR_BK0_BASE + DDR_SIZE)
	{
		printf("h264 end paddr 0x%x > DDR_BK0_BASE + DDR_SIZE 0x%x !!!!!!!!!!!!!!!!!!\r\n",
			paddr, DDR_BK0_BASE + DDR_SIZE);
		while(1);
	}
	else
	{
		printf("addr 0x%x ~ addr 0x%x temp size 0x%x \r\n", 
			paddr, DDR_BK0_BASE + DDR_SIZE, DDR_BK0_BASE + DDR_SIZE - paddr);
	}	
#endif	
}


void h264_mch_set_param(int ch)
{
	int idx, slicetype, idr_flag;
	
	//printf("%s \r\n",__func__);
	
	if(ch >= hmc.num_ch) {
		printf("error : channel(%d)\n", ch);
		return;
	}
	
	idx = h264_mch_get_enc_queue(ch);

	///////////////////////////////////////
	slicetype = (hmc.chi[ch].pic%hmc.chi[ch].i_interval==0) ? 0 : 1;   // 0 - I Slice, 1 - P Slice
	///////////////////////////////////////
#if 1
	if(slicetype == 1 && gptMsgShare.Rtsp_connect[ch] == 1 && hmc.chi[ch].i_interval > 30 && hmc.avi_save != DEF_ON)
	{
		//printf(" %d ch rtsp connect! set i frame\r\n", ch);
		gptMsgShare.Rtsp_idr[ch] = hmc.chi[ch].i_interval;
		slicetype = 0; 
		hmc.chi[ch].i_interval = 3;
		gptMsgShare.Rtsp_connect[ch]++;
	}
	else if(slicetype == 0 && gptMsgShare.Rtsp_connect[ch] > 1)
	{
		if(gptMsgShare.Rtsp_connect[ch]++>3)
		{
			//printf("%d ch rtsp connect! set back\r\n", ch);
			gptMsgShare.Rtsp_connect[ch] = 0;
			if(gptMsgShare.Rtsp_idr[ch] > 0 && gptMsgShare.Rtsp_idr[ch] < 240 && gptMsgShare.Rtsp_idr[ch] != hmc.chi[ch].i_interval)
				hmc.chi[ch].i_interval = gptMsgShare.Rtsp_idr[ch];
		}
	}
#endif
	if(slicetype == 0)
	{
		hmc.chi[ch].on_profile_idc 	= hmc.chi[ch].profile_idc;
		hmc.chi[ch].on_entropy		= hmc.chi[ch].entropy;
		hmc.chi[ch].on_cabac_idc	= hmc.chi[ch].cabac_idc;
	}
	
	idr_flag = ((hmc.chi[ch].i_interval>1 && slicetype==0) || (hmc.chi[ch].i_interval==1)) ? 1 : 0;
	// i_interval == 1이면 imgbuf->pic%256은 0 ~ 255로 반복되야 한다.(8<<8이므로 8bit의 값 이내에서 반복)
	// 아닌 경우 i_interval이 30인 경우 imgbuf->pic%i_interval 0 ~ 29
	
	if(slicetype == 0)// 0 - I Slice, 1 - P Slice
	{
		bitctl_init_gop(ch);
	}	
	///////////////////////////////////////////////////////////////////////////// CBR / VBR
	if (hmc.chi[ch].brmode == e_brmVBR || hmc.chi[ch].brmode == e_brmCBR) {
		h264_mch_cbr_processing(ch, idr_flag);
	}
	else if(hmc.chi[ch].brmode == e_brmCVBR) {
		bitctl_init_frame_i_p(ch, slicetype);
	}	
	
	hmc.chi[ch].ystride = ((hmc.chi[ch].width + 15) >> 4) << 4;
	hmc.chi[ch].cstride = hmc.chi[ch].ystride;
	//printf("ystride %d cstride %d width %d\r\n", hmc.chi[ch].ystride, hmc.chi[ch].cstride, hmc.chi[ch].width);
	hmc.chi[ch].slice_type[idx] = slicetype;

	H264_SIZE_T 	= (hmc.chi[ch].width<<16) + (hmc.chi[ch].height);

	H264_STRIDE_T	= (hmc.chi[ch].ystride<<16) + hmc.chi[ch].cstride;//(hmc.chi[ch].width<<16) + (hmc.chi[ch].width/2);

	H264_TMBNUM 	= hmc.chi[ch].num_mb;

	H264_QSCALE		= hmc.chi[ch].cur_qp;//(0<<8) + (hmc.chi[ch].cur_qp);

	H264_ENCOPT_T	= (0<<19) + (1<<18) + (0<<17) /*+ (0<<16) */+ (hmc.chi[ch].pps_qp<<8)/* + (1<<2) */+ (hmc.chi[ch].srchmode<<0);	// ringtype = 0, 

	H264_HDRINFO1_T = (8<<8) + ((hmc.chi[ch].i_interval==1) ? 0 : (hmc.chi[ch].pic%hmc.chi[ch].i_interval));//(8<<8) + (0);
	H264_HDRINFO2_T = (hmc.chi[ch].on_profile_idc<<16) + (hmc.chi[ch].on_cabac_idc<<10) + (hmc.chi[ch].on_entropy<<9) + (hmc.chi[ch].crop_b<<8) + (hmc.chi[ch].level_idc&0xff);//(hmc.chi[ch].profile_idc<<16) + (0<<12) + (0<<10) + (0<<9) + (hmc.chi[ch].crop_b<<8) + (hmc.chi[ch].level_idc&0xff);
	H264_INTRVC 	= 1;//0;    	// 0 : cb,cr seperate, 1 : cb,cr interleave 
	
	H264_INTRMASK_T = 0x6;//0x2;//0x6;//0x00;
	
	H264_AXIINFO1_T = (15<<4) + (15);

	H264_AXIINFO2_T = (0<<7) | (1<<6) | (0<<4) | (0<<3) | (1<<2) | (1<<1) | (1<<0);
	
	////H264_OPWAIT_T	= (0<<16) + (0<<8) + (255); //request write delay, request read delay,
	H264_OPWAIT_T	= (0<<16) + (0<<8) + (0);
    //H264_OPWAIT_T	= (128<<16) + (128<<8) + (128);
	////H264_OPWAIT_T	= (0<<16) + (128<<8) + (0);
	//H264_OPWAIT_T	= (128<<16) + (0<<8) + (128);
	
	//H264_OPWAIT_T	= (0<<16) + (255<<8) + (128);
	//H264_OPWAIT_T	= (0<<16) + (128<<8) + (128);
	//H264_OPWAIT_T	= (128<<16) + (128<<8) + (128);
	//H264_OPWAIT_T	= (0<<16) + (0<<8) + (0);
	
//	H264_QUANTMODE = 1;//???
//	jprintf("ENC slicetype %d Q %d H264_HDRINFO1_T %x H264_ENCOPT_T %x\n",slicetype, H264_QSCALE, H264_HDRINFO1_T, H264_ENCOPT_T);
}

void h264_mch_set_enc_buffer(int ch)
{
	int idx;
	
	//printf("%s \r\n",__func__);
	
	// 이 부분 확인.. q_eidx는 set_queue에서 update가 되어서
	// 아래처럼 바로 가져오는게 맞는지 확인 할 것.
	idx = h264_mch_get_enc_queue(ch);//hmc.que[ch].q_eidx;

#ifdef __H264_TEST__
	H264_CURYBUF	= hmc.chi[ch].cur[idx].yaddr;
	H264_CURCBUF	= hmc.chi[ch].cur[idx].caddr;
#else
	//H264_CURYBUF	= hmc.chi[ch].cur.yaddr;
	//H264_CURCBUF	= hmc.chi[ch].cur.caddr;
	H264_CURYBUF	= hmc.chi[ch].cur[idx].yaddr;
	H264_CURCBUF	= hmc.chi[ch].cur[idx].caddr;
	
#endif	

	H264_RECYBUF 	= hmc.chi[ch].rec.yaddr;
	H264_RECCBUF	= hmc.chi[ch].rec.caddr;	
	H264_REFYBUF	= hmc.chi[ch].ref.yaddr;
	H264_REFCBUF	= hmc.chi[ch].ref.caddr;		
	

#ifdef __H264_TEST__
	//H264_STPSBUFW	= hmc.chi[ch].stpSaddr;
	//H264_STPIBUFW	= hmc.chi[ch].stpIaddr;
	H264_STPSBUFW	= hmc.chi[ch].stpsaddr[idx];
	H264_STPIBUFW	= hmc.chi[ch].stpiaddr[idx];	
#else
	H264_STPSBUFW	= hmc.chi[ch].stpsaddr[idx];
	H264_STPIBUFW	= hmc.chi[ch].stpiaddr[idx];
#endif

	H264_PREPTR 	=  (MAX_Q_CH_NUM * ch) + idx;
	//printf("%dch enc go IDX %d\r\n", ch, idx);

	//jprintf("ENC %dch IDX %d RawY 0x%x RawC 0x%x \r\n RecY 0x%x RecC 0x%x RefY 0x%x RefC 0x%x \r\n Stpd 0x%x Stpi 0x%x \r\n",
	//	ch, H264_PREPTR,H264_CURYBUF,H264_CURCBUF,H264_RECYBUF,H264_RECCBUF,H264_REFYBUF,H264_REFCBUF,H264_STPSBUFW,H264_STPIBUFW);

}

void h264_mch_set_vlc_buffer(int ch)
{
	int idx;
	int width, height;
	int qp;
	int slicetype;
	u32 stpiaddr;
//	u32 *ptest;
	
	//printf("%s \r\n",__func__);
	
	// 이 부분 확인.. q_vidx는 get_queue에서 update가 되어서
	// 아래처럼 바로 가져오는게 맞는지 확인 할 것.
	idx 			= h264_mch_get_queue_index(ch);//hmc.que[ch].q_vidx;
		  
	width 			= hmc.chi[ch].width;
	height 			= hmc.chi[ch].height;
	slicetype		= hmc.que[ch].slicetype[idx];	 
	qp 				= hmc.que[ch].qp[idx];   	
	stpiaddr		= hmc.chi[ch].stpiaddr[idx];	

	H264_MCHNNEL	= ch;
	H264_STPIBUFR	= stpiaddr;
	H264_PSTPTR 	= (MAX_Q_CH_NUM*ch) + idx;

	H264_MISIZE_T	= (width<<16) + (height);
	H264_MCABAC_T	= (hmc.chi[ch].on_cabac_idc<<1) + hmc.chi[ch].on_entropy;//(0<<2) + (cabac);
	H264_MQSCALE	= qp;
	H264_MPFRAME	= slicetype;

	H264_STRMBUF	= hmc.chi[ch].strmaddr;
	H264_SBUFEND	= H264_ALIGN(hmc.chi[ch].strmaddr + hmc.chi[ch].stpsize); //hmc.strmendaddr;
	
	//H264_SBUFSIZE	= max_str_size;	//4*1024;

	//jprintf("VLC %dch IDX %d stpi addr %x strm addr%x \r\n", ch, H264_PSTPTR, stpiaddr , H264_STRMBUF);
//	ptest = H264_STPIBUFR;
    //jprintf("VLC %dch IDX %d H264_STPIBUFR %x ptest %x *ptest %x H264_STRMBUF %x vlc start\r\n", ch, H264_PSTPTR, H264_STPIBUFR, ptest, *ptest, H264_STRMBUF);
}

#if 0
void h264_mch_shift_enc_buffer(int ch, int stpsize)
{
	jprintf("h264_mch_shift_enc_buffer\n");
	int i;
	
	DmaMemCpy_ip(hmc.chi[ch].stpsaddr[hmc.que[ch].q_eidx], hmc.chi[ch].stpSaddr, stpsize);
	invalidate_dcache_range((uint)(hmc.chi[ch].stpsaddr[hmc.que[ch].q_eidx]),(uint)(hmc.chi[ch].stpsaddr[hmc.que[ch].q_eidx]+stpsize));
#if 1
	for(i=0;i<stpsize; i++)
	{
		if(*(((BYTE*)hmc.chi[ch].stpsaddr[hmc.que[ch].q_eidx])+i) != *(((BYTE*)hmc.chi[ch].stpSaddr)+i))
			printf("D %dth dst %x src %x",i, *(((BYTE*)hmc.chi[ch].stpsaddr[hmc.que[ch].q_eidx])+i) != *(((BYTE*)hmc.chi[ch].stpSaddr)+i));
	}
#endif	
	DmaMemCpy_ip(hmc.chi[ch].stpiaddr[hmc.que[ch].q_eidx], hmc.chi[ch].stpIaddr, 0x4000);
	invalidate_dcache_range((uint)(hmc.chi[ch].stpiaddr[hmc.que[ch].q_eidx]),(uint)(hmc.chi[ch].stpiaddr[hmc.que[ch].q_eidx]+0x4000));
#if 1
	for(i=0;i<0x4000;i++)
	{
		if(*(((BYTE*)hmc.chi[ch].stpiaddr[hmc.que[ch].q_eidx])+i) != *(((BYTE*)hmc.chi[ch].stpIaddr)+i))
		{
			printf("I %dth dest %x src %x",i, *(((BYTE*)hmc.chi[ch].stpiaddr[hmc.que[ch].q_eidx])+i) != *(((BYTE*)hmc.chi[ch].stpIaddr)+i));		
		}
	}
#endif
}
#endif
void h264_mch_shift_buffer(int ch)
{
	//unsigned int tmp_y, tmp_u;
	unsigned int tmp_y, tmp_c;
	
	//printf("%s \r\n",__func__);
	
	tmp_y = hmc.chi[ch].ref.yaddr;
	//tmp_u = hmc.chi[ch].ref.uaddr;
	tmp_c = hmc.chi[ch].ref.caddr;
	
	hmc.chi[ch].ref.yaddr = hmc.chi[ch].rec.yaddr;
	//hmc.chi[ch].ref.uaddr = hmc.chi[ch].rec.uaddr;
	hmc.chi[ch].ref.caddr = hmc.chi[ch].rec.caddr;
	
	hmc.chi[ch].rec.yaddr = tmp_y;
	//hmc.chi[ch].rec.uaddr = tmp_u;
	
	hmc.chi[ch].rec.caddr = tmp_c;
	
}

void h264_mch_encode_param(int ch, int *type)
{
	int fnumpic, idr_flag;	
	int pic, interval;
	
	//printf("%s \r\n",__func__);
	
	pic = hmc.chi[ch].pic;
	interval = hmc.chi[ch].i_interval;
	
	*type = (pic%interval==0) ? 0 : 1;   // 0 - I Slice, 1 - P Slice
	
    fnumpic  = (interval==1) ? (pic%256) : (pic%interval);
    idr_flag = ((interval>1 && *type==0) || (interval==1 && pic==0)) ? 1 : 0;
	
	H264_PICTYPE_T	= (idr_flag<<1) + (*type);

	if(idr_flag==1) H264_HDRINSERT_T = (0x03);
	else   H264_HDRINSERT_T = (0x00);

	//jprintf("ENC H264_PICTYPE_T %x H264_HDRINSERT_T %x\n",H264_PICTYPE_T, H264_HDRINSERT_T);
	
}


void h264_mch_encode_command(int ch)
{
	int i;		
	//printf("%d ch enc go\r\n", ch);
	
	//jprintf("ENC %dch IDX %d enc start : pic(%d) eidx of raw %d Latest Raw IDX%d ValidNum%d vlock(%d)\r\n", 
	//ch, H264_PREPTR, h264_mch_get_pic(ch), h264_mch_get_enc_queue(ch), h264_mch_get_raw_queue(ch), h264_mch_get_raw_valid_num(ch), h264_mch_get_vlock_ready_state(ch));
	


	H264_ECHNNEL	= ch;
	H264_ENCSTART_T	= 0x01; // encoding command
	//jprintf("H264_ENCSTART_T %x\n",H264_ENCSTART_T);
//	h264_mch_print_all_reg();

	for(i=0; i<8; i++) {NOP();}
	
}							

void h264_mch_vlc_command(void)
{
	H264_ENCSTART_T = 0x02;
//	for(i=0; i<8; i++) {NOP();}
	H264_ENCSTART_T = 0x10;
}

#endif	//__H264_MCH__
