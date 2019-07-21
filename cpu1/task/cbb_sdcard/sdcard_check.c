
#include "dev.h"
#include "dev_inline.h"
#include <time.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sys_arch.h"

#include "lwip/ip_addr.h"

#include "user.h"
#include "msg.h"
//#include "muxer_avidec.h"

#ifdef __CBB__
#include "cbb_power.h"
#include "cbb_ctlmain.h"
#endif

#include "file.h"
#include "muxer_avienc.h"
#include "avienc_info.h"

#include "timer_task.h"
#include "sdcard_check.h"

#ifdef __DEVICE_SD__
// func CheckSDstate
//#include "sd.h"
#define ENX_SDdetect_DEBUG			// printf 주석용

#ifdef ENX_SDdetect_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

sys_thread_t stt_avienc = NULL;
sys_thread_t stt_avidec = NULL;

extern UINT bSD_FAT_mounted;
#ifdef __CBB__
extern int ck_sdsave;
#endif
#ifdef CBB_SD_OVERWRITE
extern t_avienc_t enc_nor;	// 주행 상시
extern t_avienc_t enc_evt;	// 주행 이벤트
extern t_avienc_t enc_mar;	// 주행 메뉴얼

extern int g_normalgo;
extern int g_eventgo;
extern int g_manualgo;
#endif

#ifdef __CBB__
extern int is_error;
extern int is_update;
extern BYTE is_BatLow;
#endif

int bSD_State = sd_OFF;
int is_sdformat = 0;
/*
void SdInit(void)
{
#ifdef __CBB__
	GpioOutDir(GPIO_SDEN);	// SD card
	GpioSetHi(GPIO_SDEN);
	vTaskDelay(50);
#endif
	SdDiCmd();			// Command interrupt disable
	SdDiDat();			// Data interrupt disable
	SdCfg(0xF, 500, 2);	// 4.64MHz(CLK:148.5MHz), 500ms timeout set
}
*/
#ifdef CBB_SD_OVERWRITE

void SdSizeCheck(void)
{
	UINT nSDSize = Sdio0GetAUSize();//SdCard_GetSize();
	nSDSize = nSDSize / 1024;
	if(nSDSize <= 8)
		nSD_CARD_SIZE = 8;	// 8GB
	else if(nSDSize <= 16)
		nSD_CARD_SIZE = 16;	// 16GB
	else if(nSDSize <= 32)
		nSD_CARD_SIZE = 32;	// 32GB
	else
		tprintf("Size Check Error (%d)\n", nSDSize);
}
#endif

// 현재 시간(RTC)이 SD카드에 저장된 시간보다 과거인 경우 시간을 설정한다.
int SdTimeCheck(void)
{
	char strPath[128] = {0};
	TCHAR strFileName[_MAX_LFN+1] = {0};
	FRESULT res;
	FILINFO finfo;
	FATFS *fs;
	DIR dir;
	int nYear_Old, nMonth_Old, nDay_Old, nHour_Old, nMin_Old, nSec_Old;
	int nYear_New, nMonth_New, nDay_New, nHour_New, nMin_New, nSec_New;
	int nGetRes = 0, nNewDay = 0;
	nYear_New = nMonth_New = nDay_New = nHour_New = nMin_New = nSec_New = 0;

	sprintf(strPath, "%d:/%s", MMC, MMC_DIR_NOR);
	res = f_opendir(&dir, strPath);
	if(res != FR_OK)
		return 1;

	finfo.lfname = strFileName;
	finfo.lfsize = _MAX_LFN;

	// 현재시간을 가져온다. 이 시간은 RTC과 같다.
	struct tm *tmprint;
	while(gptMsgShare.TimeLock)
		vTaskDelay(1);
	
	gptMsgShare.TimeLock = 1; 
	tmprint = localtime(&gptMsgShare.TIME);
	gptMsgShare.TimeLock = 0;
	
	sprintf(strPath, SAVE_NOR_NAME_FORMAT, tmprint->tm_year+1900-2000, tmprint->tm_mon+1, tmprint->tm_mday, tmprint->tm_hour, tmprint->tm_min, tmprint->tm_sec);

	for(;;)
	{
		res = f_readdir(&dir, &finfo);
		if((res != FR_OK) || !finfo.fname[0])
		{
			res = FR_OK;
			break;
		}
		if(finfo.fattrib & AM_DIR)
			continue;
		if(dir.lfn_idx == 0xFFFF)
			continue;
		else
		{
			if(strlen(finfo.lfname) == SAVE_FILENAME_LENS)
			{
				nGetRes = sscanf(finfo.lfname, SAVE_NOR_NAME_FORMAT, 
					&nYear_Old, &nMonth_Old, &nDay_Old, &nHour_Old, &nMin_Old, &nSec_Old);
				if(nGetRes == 6)
				{	
					if(strcmp(strPath, finfo.lfname) == -1)
					{
						strcpy(strPath, finfo.lfname);
						nYear_New = nYear_Old;
						nMonth_New = nMonth_Old;
						nDay_New = nDay_Old;
						nHour_New = nHour_Old;
						nMin_New = nMin_Old;
						nSec_New = nSec_Old;
						nNewDay = 1;
					}
				}
			}
		}
    }

	if(nNewDay)
	{
		// 만약, 현재 시간이 가장 마지막 저장파일보다 과거라면, 
		// 가장 마지막 저장파일의 +1분을 현재 시간으로 설정한다. -> 60분 등 over할 가능성이 존재, 초를 무조건 59초로 한다.
		if(set_devicetime(TimeZone_LOC, nYear_New+2000, nMonth_New, nDay_New, nHour_New, nMin_New, 59) == DEF_FAIL)
		{
			printf("rtc time set fail\n");
			return 1;
		}
		return 0;
	}
	return 1;
}

void SDcardInitTask(void)
{
	FRESULT fres;
	bSD_State = sd_INIT;
	Sdio0Init();//SdInit();
	if(bSD_State == sd_OFF)	goto init_end;
	if(Sdio0InitProcess() == DEF_FAIL)
	{
		tprintf("SD card IN(init fail)\n");
		bSD_State = sd_ERR;
		goto init_end;
	}
	if(bSD_State == sd_OFF)	goto init_end;
	fres = drive_in(MMC);	// driver mount
	if(bSD_State == sd_OFF)	goto init_end;
	switch(fres)
	{
		case FR_OK:
		case FR_NO_FILESYSTEM:
			tprintf("SD card IN\n");
#ifdef CBB_SD_OVERWRITE			
			SdSizeCheck();
#endif
			drive_init(MMC);
			if(bSD_State == sd_OFF)	goto init_end;
#ifdef __CBB__
			if((firmwarefile_check(MMC, "XPFirmware.bin"))&&(is_BatLow==0))
#else
			if(firmwarefile_check(MMC, "u2.bin"))
#endif
			{
#ifdef __CBB__
				is_update = 1;
#endif
				firmwarefile_update(MMC, "XPFirmware.bin");
#ifdef __CBB__
				is_update = 0;
#endif
			}
			if(bSD_State == sd_OFF)	goto init_end;
#if 0 //def __CBB__			
			if(is_settingini == 0)	// 카드in인 경우는 상관없으나, ftp out후 init인 경우, setting이 변경되엇다면 get_ini하지 않는다. 
				cbbsetting_get_ini();
			cbbsetting_put_ini(0);
#endif
			//SdTimeCheck(); //ksj_151228_time_dismatch
			if(gtUser.bSdAviSave == SDSAVE_USE_ON /*&&g711_mb.sleepmode == 0 *//*&& !stt_avienc*/)
			{
				tprintf("SD save Start\n");
				////////////FPGA board TEST
				//gptMsgShare.H264_ENC_ENABLE = 1;
				///////////
				if(stt_avienc)
					muxer_avienc_init();
//				vTaskDelay(300);
				bSD_State = sd_SAVE;
#ifdef CBB_SD_OVERWRITE

#ifdef __CBB__				
				if(cbbpower_get_mode() == eCBB_Drive)
					muxer_avienc_normal_go();
#else
				muxer_avienc_normal_go();
#endif
#else
				muxer_avienc_go(eRecNormal);
#endif
				//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_SET_VLOCK, DEF_ON) == MSG_FULL)
				//{
				//	vTaskDelay(1);
				//}
				vTaskDelay(200);

				if(!stt_avienc)
					stt_avienc = sys_thread_new("avienc", muxer_avienc_task, NULL, MAKEAVI_STACK_SIZE, MAKEAVI_TASK_PRIO);
#ifdef CBB_SD_OVERWRITE
#ifdef __CBB__
				if(!stt_avidec)
					stt_avidec = sys_thread_new("avidec", muxer_aviplayback_task, NULL, MAKEAVI_STACK_SIZE, TIMER_TASK_PRIO/*MAKEAVI_TASK_PRIO*/);
#endif				
				if(enc_play.state != AVIENC_OFF)								
					enc_play.state = AVIENC_EXIT;
#endif
//				if(stt_avidec)
//					vTaskSuspend(stt_avidec);	
			}
			else
			{
				bSD_State = sd_IDLE;
			}
			gptMsgShare.SDstate = eSDstate_COP_SD_IN;
			break;
		default:
			tprintf("SD card IN(mount fail:err%d)\n", fres);
#ifdef __CBB__			
			bitchang(is_error, CBB_ERROR_SDMOUNT, 1);
#endif			
			bSD_State = sd_ERR;
			//g711_mb.wBuzzCnt=5;
			gptMsgShare.SDstate = eSDstate_COP_SD_FAIL;
			break;
	}
init_end:
	return;
//	vTaskDelete(NULL);
}
#define TIMEOUT_PLAYBACK 600

#ifdef __CPU0_FTP__
//static UINT playbacktimecheck = 0; 

void CheckFTPstate(void)
{
	static UINT old_SdAviSave;
	if(!Sdio0GetInstall()/*SD_CD_DETECT*/)	// SD card IN
	{
		if(gptMsgShare.FTPstate == eFTPstate_CONN)
		{	// ftp conn, sd save off
			if(is_sdformat)
				return;
#ifndef WIFI_AVIENC_STOP
			old_SdAviSave = gtUser.bSdAviSave;		// avi write의 switch의 상태값 저장
			gtUser.bSdAviSave = SDSAVE_USE_OFF;		// avi write의 현재 switch는 OFF
			
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, DEF_OFF) == DEF_FAIL) 
				vTaskDelay(1);
#endif
			//tprintf("FTP : SD save Stop Start\n");
#ifdef CBB_SD_OVERWRITE

			if(enc_nor.state != AVIENC_OFF)
				enc_nor.state = AVIENC_EXIT;			// avi write를 모두 멈춘다.
			if(enc_evt.state != AVIENC_OFF)
				enc_evt.state = AVIENC_EXIT;
			if(enc_mar.state != AVIENC_OFF)
				enc_mar.state = AVIENC_EXIT;
			if(enc_play.state != AVIENC_OFF)
				enc_play.state = AVIENC_EXIT;
			
			while(enc_ing != NULL )
			{
				//tprintf("FTP : SD save Stop Check1\n");
				vTaskDelay(1);
			}
			while(enc_play.state != AVIENC_OFF )			
			{
				//tprintf("FTP : SD save Stop Check\n");
				vTaskDelay(1);
			}
#endif			
			//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_DISCONNECT_RTSP, 0) == MSG_FULL)
			//{
			//	vTaskDelay(1);
			//}			
//			if(stt_avienc)
//				vTaskSuspend(stt_avienc);
//			if(stt_avidec)
//				vTaskSuspend(stt_avidec);
			//tprintf("FTP : SD save Stop End\n");
			drive_out(MMC);

			//tprintf("FTP : state COP_SD_OUT\n");
			h264_mb.playstart = 1;
			gptMsgShare.SDstate = eSDstate_COP_SD_OUT;
		}
		else if(gptMsgShare.SDstate == eSDstate_COP_SD_NOT)
		{
			// sd카드가 없다가 장착됨 : 여기에 진입할 일 없다?
			//tprintf("FTP : SD card in detect\n");
			old_SdAviSave = gtUser.bSdAviSave;
			gtUser.bSdAviSave = SDSAVE_USE_OFF;

			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, DEF_OFF) == DEF_FAIL) 
				vTaskDelay(1);
			tprintf("FTP : state COP_SD_OUT\n");
			gptMsgShare.SDstate = eSDstate_COP_SD_OUT;
		}
		else if(gptMsgShare.SDstate == eSDstate_CPU_SD_OUT || gptMsgShare.SDstate == eSDstate_CPU_SD_FAIL)
		{	// ftp disconn, sd save on
			avienc_info_sbuf_reset(&enc_nor);
			avienc_info_sbuf_reset(&enc_evt);
			avienc_info_sbuf_reset(&enc_mar);

#ifndef WIFI_AVIENC_STOP
			gtUser.bSdAviSave = old_SdAviSave;

			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
				vTaskDelay(1);

#endif
			bSD_State = sd_OFF;	// sd_OFF로 하여 초기화단계를 거친다
			gptMsgShare.SDstate = eSDstate_COP_SD_IN;
		}
	}
	else	// SD card OUT
	{
		gtUser.bSdAviSave = old_SdAviSave;
		
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
			vTaskDelay(1);
		
		bSD_State = sd_OFF;
		gptMsgShare.SDstate = eSDstate_COP_SD_NOT;
	}
}
#endif	// __CPU0_FTP__

#include "user.h"
#include "sys_arch.h"


sys_thread_t stt_testreboot2 = NULL;
/*
void testreset2(void* pvParameters)
{
	//while(1)
	{
		printf("reboot count %d \n",gtUser.ciData.motion_range);
		gtUser.ciData.motion_range += 1;
		UserSave(&gtUser);			
		
		if(gtUser.bSdAviSave == SDSAVE_USE_ON)
		{
			gtUser.bSdAviSave = SDSAVE_USE_OFF;
			vTaskDelay(300);
		}
		
		
		portDISABLE_INTERRUPTS();
		WdtSystemReset();
	}
}
*/
void CheckSDstate(void *pvParameters)
{
	static FRESULT fres;

	while(1)
	{
		if(h264_mb.linkstate==1)
		{
			memcpy(gtUser.bdaddrsave , h264_mb.bt_addr,6);
			memcpy(gtUser.storedlink_key, h264_mb.bt_link_key,16);
		
			
			dprintf("User info loading BT addr %x %x %x %x %x %x\n",gtUser.bdaddrsave[0],gtUser.bdaddrsave[1],gtUser.bdaddrsave[2],gtUser.bdaddrsave[3],gtUser.bdaddrsave[4],gtUser.bdaddrsave[5]);
			dprintf("User info loading h264_mb BT addr %x %x %x %x %x %x\n",h264_mb.bt_addr[0],h264_mb.bt_addr[1],h264_mb.bt_addr[2],h264_mb.bt_addr[3],h264_mb.bt_addr[4],h264_mb.bt_addr[5]);
			dprintf("User info loading storedlink_key %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",gtUser.storedlink_key[0],gtUser.storedlink_key[1],gtUser.storedlink_key[2],gtUser.storedlink_key[3],gtUser.storedlink_key[4],gtUser.storedlink_key[5],gtUser.storedlink_key[6],gtUser.storedlink_key[7],gtUser.storedlink_key[8],gtUser.storedlink_key[9],gtUser.storedlink_key[10],gtUser.storedlink_key[11],gtUser.storedlink_key[12],gtUser.storedlink_key[13],gtUser.storedlink_key[14],gtUser.storedlink_key[15]);
			dprintf("User info loading h264_mb storedlink_key %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x\n",h264_mb.bt_link_key[0],h264_mb.bt_link_key[1],h264_mb.bt_link_key[2],h264_mb.bt_link_key[3],h264_mb.bt_link_key[4],h264_mb.bt_link_key[5],h264_mb.bt_link_key[6],h264_mb.bt_link_key[7],h264_mb.bt_link_key[8],h264_mb.bt_link_key[9],h264_mb.bt_link_key[10],h264_mb.bt_link_key[11],h264_mb.bt_link_key[12],h264_mb.bt_link_key[13],h264_mb.bt_link_key[14],h264_mb.bt_link_key[15]);
		
			dprintf("bt link key saved !!\n");			
			//UserSave((BYTE*)&gtUser);	
			h264_mb.linkstate = 0;

			//if(stt_testreboot2 == NULL)
			//	stt_testreboot2 = sys_thread_new("test", testreset2, NULL, SHELL_STACK_SIZE, SHELL_TASK_PRIO);
		}

#ifdef __CPU0_FTP__
		if(bSD_State == sd_FTP)
		{
			CheckFTPstate();
			vTaskDelay(1);
			continue;
		}
#endif
#ifdef __CPU0_FTP__
		if(gptMsgShare.FTPstate != eFTPstate_NONE)
		{	// cpu0의 FTP에 접속함.
			bSD_State = sd_FTP;
			gtUser.bSdAviSave = SDSAVE_USE_OFF;
		
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
				vTaskDelay(1);
		
			h264_mb.playSwitch = 0;
			h264_mb.playbackgo = 0;
			h264_mb.playbacktimecheck=0;

			CheckFTPstate();
			vTaskDelay(1);
			continue;
		}
#else
		if(gptMsgShare.FTPstate != eFTPstate_NONE)
		{	// cpu0의 FTP에 접속함.
			bSD_State = sd_FTP;
			
			if(gtUser.bSdAviSave == SDSAVE_USE_ON)
			{
				gtUser.bSdAviSave = SDSAVE_USE_OFF;
				
				while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
					vTaskDelay(1);
				//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_SET_VLOCK, DEF_OFF) == MSG_FULL)
				//{
				//	vTaskDelay(1);
				//}
				
				vTaskDelay(60);		
			}
			
			h264_mb.playSwitch = 0;
			h264_mb.playbackgo = 0;
			h264_mb.playbacktimecheck=0;
			vTaskDelay(1);
			continue;
		}
#endif

		if(!Sdio0GetInstall()/*SD_CD_DETECT*/)	// SD card IN
		{
			static int err_check = 0;
		

			if(bSD_State == sd_OFF)
			{
				bSD_State = sd_INIT;	// 초기화 중... 해당 작업은 SDcardInitTask에서 진행
				SDcardInitTask();//sys_thread_new("SDInit", SDcardInitTask, NULL, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
			}
#ifndef __CPU0_FTP__		
			else if(bSD_State == sd_FTP)
			{
				bSD_State = sd_IDLE;	
				h264_mb.playbacktimecheck=0;
			}
#endif		
			else if(bSD_State == sd_IDLE)
			{
				if(is_sdformat)
				{
					vTaskDelay(1);
					continue;
				}
				err_check = 0;
				if(gtUser.bSdAviSave == SDSAVE_USE_ON /*&& g711_mb.sleepmode == 0*//*&& !stt_avienc*/)	// user command
				{
					tprintf("SD save Start 2\n");
					muxer_avienc_init();
	//				gptMsgShare.Connected_ip=0;
					bSD_State = sd_SAVE;
					drive_init(MMC);
#ifdef CBB_SD_OVERWRITE
					
#ifdef __CBB__					
					if(cbbpower_get_mode() == eCBB_Drive)
						muxer_avienc_normal_go();
#else
					muxer_avienc_normal_go();
#endif							
					
					if(enc_play.state != AVIENC_OFF)
						enc_play.state = AVIENC_EXIT;

					while(enc_play.state != AVIENC_OFF)
					{
						tprintf("playback Stop Check\n");
						vTaskDelay(1);
					}						
#endif					
	//				if(stt_avidec)
	//					vTaskSuspend(stt_avidec);
					
	//				if(stt_avienc)
	//					vTaskResume(stt_avienc);
				}
				else if(gtUser.bSdAviSave == SDSAVE_USE_OFF && h264_mb.playbackgo == 1/*&& stt_avienc == NULL && stt_avidec  == NULL*/)	// user command
				{
				
					tprintf("SD read Start\n");
					bSD_State = sd_READ;
	//				if(stt_avidec)
	//					vTaskResume(stt_avidec);
					//stt_avidec = sys_thread_new("avidec", muxer_aviplayback_task, NULL, MAKEAVI_STACK_SIZE, TIMER_TASK_PRIO/*MAKEAVI_TASK_PRIO*/);
				}
				else if(h264_mb.playSwitch == 1 && h264_mb.playbackgo == 0)
				{	
					drive_init(MMC);
					//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_SET_VLOCK, DEF_OFF) == MSG_FULL) 
					//{
					//	vTaskDelay(1);
					//}
					//printf("H264 ENCODING OFF\n");
					vTaskDelay(10);
					gtUser.bSdAviSave = SDSAVE_USE_OFF;
					
					while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
						vTaskDelay(1);				
					
					printf("SD SAVE OFF\n");
					//h264_mb.ready = 0;
					//g711_mb.ready = 0;
					//txts_mb.ready = 0;
					h264_mb.playbackgo = 1;
					printf("playback on\n");
	//				if(stt_avienc)
	//					vTaskSuspend(stt_avienc);
				}
				else			
				{
					h264_mb.playbacktimecheck++;
					if(h264_mb.playbacktimecheck>TIMEOUT_PLAYBACK/2)//5sec
					{
						if(h264_mb.playstart)
						{
							//vTaskDelay(200);
							//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_SET_VLOCK, DEF_ON) == MSG_FULL)
							//{
							//	vTaskDelay(1);
							//}
							vTaskDelay(200);
							h264_mb.playstart = 0;
						}
						if(h264_mb.playSwitch == 1)
						{					
							h264_mb.pause = 1;
							h264_mb.playSwitch = 0;
							h264_mb.playbackgo = 0;
							dprintf("playback mode off ");
						}
						h264_mb.playbacktimecheck = 0;
						gtUser.bSdAviSave = SDSAVE_USE_ON;
						
						while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
							vTaskDelay(1);
						
						dprintf("recording mode On! ");
					}
					else
					{
						//dprintf("recording mode off %d",h264_mb.playbacktimecheck);
#ifdef CBB_SD_OVERWRITE
						
						portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
						if(bufSem)
							xSemaphoreGiveFromISR(bufSem, &xHigherPriorityTaskWoken);
#endif						
					}
				}
			}
			else if(bSD_State == sd_SAVE)
			{
				err_check = 0;
				if(gtUser.bSdAviSave == SDSAVE_USE_OFF)	// user command
				{
					tprintf("SD save Stop\n");
#ifdef CBB_SD_OVERWRITE
					
					portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
					if(bufSem)
						xSemaphoreGiveFromISR(bufSem, &xHigherPriorityTaskWoken);
#endif					
					bSD_State = sd_IDLE;

				}
				else if(h264_mb.playSwitch == 1 && h264_mb.playbackgo == 0)
				{	
						
					gtUser.bSdAviSave = SDSAVE_USE_OFF;
					
					while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SDSAVE_ON_OFF, gtUser.bSdAviSave) == DEF_FAIL) 
						vTaskDelay(1);		
					
#ifdef CBB_SD_OVERWRITE
					
					if(enc_nor.state != AVIENC_OFF)
						enc_nor.state = AVIENC_EXIT;			// avi write를 모두 멈춘다.
					if(enc_evt.state != AVIENC_OFF)
						enc_evt.state = AVIENC_EXIT;
					if(enc_mar.state != AVIENC_OFF)
						enc_mar.state = AVIENC_EXIT;
					while(enc_ing != NULL )
					{
						tprintf("SD save Stop Check\n");
						vTaskDelay(1);
					}
#endif					
					//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_SET_VLOCK, DEF_OFF) == MSG_FULL)
					//{
					//	vTaskDelay(1);
					//}
					vTaskDelay(300);

	//				if(stt_avienc)
	//					vTaskSuspend(stt_avienc);			
					printf("SD SAVE OFF\n");
					//h264_mb.ready = 0;
					//g711_mb.ready = 0;
					//txts_mb.ready = 0;
					h264_mb.playbackgo = 1;
					printf("playback on\n");
					
				}
			}
			else if(bSD_State == sd_READ)
			{
				//printf("read %d %d %d\n",h264_mb.playSwitch, h264_mb.playbackgo, gtUser.bSdAviSave);

				if(h264_mb.playSwitch == 0 && h264_mb.playbackgo == 0)
				{
					bSD_State = sd_IDLE;
#ifdef CBB_SD_OVERWRITE

					if(enc_play.state != AVIENC_OFF)
						enc_play.state = AVIENC_EXIT;
					
					while(enc_play.state != AVIENC_OFF)
					{
						tprintf("playback Stop Check\n");
						vTaskDelay(1);
					}			
#endif					
				}
				
				else if(h264_mb.playSwitch == 1 && h264_mb.playbackgo == 0 && h264_mb.pause == 0)
				{
					printf("sd_IDLE PLAYBACK Go!!!!!!!!!!!!!!!\n");
					h264_mb.playbackgo = 1;
					//stt_avidec = sys_thread_new("avidec", muxer_aviplayback_task, NULL, MAKEAVI_STACK_SIZE, TIMER_TASK_PRIO/*MAKEAVI_TASK_PRIO*/);
				}
				else if(h264_mb.playSwitch == 1 && h264_mb.playbackgo == 0) 
				{
					h264_mb.playbacktimecheck++;
					if(h264_mb.playbacktimecheck>TIMEOUT_PLAYBACK/4)//10sec
					{
						if(h264_mb.playSwitch == 1)
						{					
							h264_mb.pause = 1;
							h264_mb.playSwitch = 0;
							h264_mb.playbackgo = 0;
#ifdef CBB_SD_OVERWRITE
							
							if(enc_play.state != AVIENC_OFF)
								enc_play.state = AVIENC_EXIT;

							while(enc_play.state != AVIENC_OFF)
							{
								tprintf("playback Stop Check\n");
								vTaskDelay(1);
							}	
#endif							
							bSD_State = sd_IDLE;

							//while(MsgRtpPut(gptMsgRtpMoci, MSG_CMD_DISCONNECT_RTSP, 0) == MSG_FULL)
							//{
							//	vTaskDelay(1);
							//}
							dprintf("playback mode off ");
						}
						h264_mb.playbacktimecheck = 0;
					}
					else
					{
						//dprintf("playback mode on %d",h264_mb.playbacktimecheck);
					}
				}
				else
				{
					h264_mb.playbacktimecheck = 0;
				}
					
			}
			else if(bSD_State == sd_ERR)
			{
				static int cnt = 0;
				cnt++;
				if(cnt >= 25)	// 25 == 5s
				{
					err_check++;
					tprintf("SDcard Error(%d) / reload\n", err_check);
					bSD_State = sd_OFF;
					cnt = 0;
					if(err_check >= 3)
					{
						tprintf("reload stop. reboot go\n");	// sd 카드 인식 실패 3회연속
#if 0 //#ifdef __CBB__					
						if(cbbpower_get_mode() != eCBB_Off)
						{
							G726Playlist_put(eG726_REBOOT_SYSERROR);
							vTaskDelay(300);
						}
#endif
						printf("%s(%d) : WDT GO\n", __func__, __LINE__);
						//WdtSystemReset();

						while(1)
						{
							vTaskDelay(1);
						}
					}
				}
			}
		}
		else if(Sdio0GetInstall()/*SD_CD_DETECT*/&& (bSD_State != sd_OFF))	// SD card OUT
		{
			bSD_State = sd_OFF;
			if(stt_avienc)
			{
#if 0 //#ifdef __CBB__
				if(cbbpower_get_mode() != eCBB_Off)
					G726Playlist_put(eG726_SDSAVE_STOP);
#endif
				tprintf("SD save Stop1\n");
				vTaskDelete(stt_avienc);
				muxer_avienc_delete();
				stt_avienc = NULL;
			}
			if(stt_avidec)
			{
				tprintf("SD read Stop\n");
				vTaskDelete(stt_avidec);
//#ifdef CBB_SD_OVERWRITE
//				
//				muxer_avidec_delete();
//#endif
				stt_avidec = NULL;
			}
			bSD_FAT_mounted = 0;
#ifdef CBB_SD_OVERWRITE		
			g_normalgo = 0;
			g_eventgo = 0;
			g_manualgo = 0;
			//gptMsgShare.EVENTstate = 0;
#endif			
#ifdef __CBB__		
			bitchang(is_error, CBB_ERROR_SDFORMAT, 0);
			bitchang(is_error, CBB_ERROR_SDMOUNT, 0);
#endif		
			drive_out(MMC); // driver unmount
			//SdCfg(0xF, 500, 2);	// 4.64MHz(CLK:148.5MHz), 500ms timeout set
			Sdio0Init();	// 400MHz set
			tprintf("SD card OUT\n");
			gptMsgShare.SDstate = eSDstate_COP_SD_NOT;
		}
		else if(Sdio0GetInstall()/*SD_CD_DETECT*/ && (bSD_State == sd_OFF))
		{
#if 0 //#ifdef __CBB__
			static int cnt = 0;
			if(cnt == 0)
			{
				G726Playlist_put(eG726_NOSDCARD);
			}
			cnt++;
			if(cnt == 3000)	// 50 == 10s, 300 == 1min, 3000 == 10min
				cnt = 0;
#endif
#ifdef __CPU0_FTP__
			bSD_State = sd_OFF;
			gptMsgShare.SDstate = eSDstate_COP_SD_NOT;
#endif
		}
		else if(bSD_State == sd_ERR)
		{
			static int cnt = 0;
			cnt++;
			if(cnt == 25)	// 25 == 5s
			{
				tprintf("SDcard Error / reload\n");
				bSD_State = sd_OFF;
				cnt = 0;
			}
		}
		else
		{	// sd카드가 없다.
#ifdef __CPU0_FTP__
			bSD_State = sd_OFF;
			gptMsgShare.SDstate = eSDstate_COP_SD_NOT;
#endif
		}
		vTaskDelay(5);
	}
}

void check_camera_sdsave(void *ctx)	// call 1sec
{
#ifdef __CBB__
	if(bSD_State == sd_FTP)
	{
		ck_sdsave = 0;
		return;
	}

	if(ck_sdsave >= 200)	// 2sec
	{
		//printf("SDsave : No Image data input\n");

		//if(cbbpower_get_mode() != eCBB_Off)
		//	G726Playlist_put(eG726_CAMERA_ERROR);

		ck_sdsave = 0;
	}
#endif	
}

#if 0
void check_sdcard_event(void *ctx)
{
	if(muxer_avienc_manual_state() == 0)
	{	// 메뉴얼 녹화 중이 아니면 메뉴얼 녹화 시작
		muxer_avienc_manual_go();
		G726Playlist_put(eG726_SDSAVE_USER);
	}
}
#endif

#endif

