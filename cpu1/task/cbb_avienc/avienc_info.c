#include <stdio.h>
#include <time.h>

#include "dev.h"
#include "dev_types.h"
#include "dev_inline.h"
#include "ff.h"
#include "file.h"
#include "stream_buffering.h"

#include "muxer_avienc.h"
#include "avienc_info.h"
#include "avienc_heap.h"
//#include "vsprintf.h"
#include "msg.h"

#ifdef __CBB__
#include "gpsrx.h"
#endif
extern void *pvPortMalloc( size_t xWantedSize );	// @heap_3
extern void vPortFree( void *pv );					// @heap_3

#if 0
#define ENTER() dprintf("%s IN\r\n", __func__);
#define LEAVE() dprintf("%s OUT\r\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

//#undef __JPEG__

#ifdef __DEVICE_SD__
static void avienc_info_file_getname(void *v_enc_t)
{
	printf("avienc_info_file_getname\n");
	ENTER();
	BYTE temp,temp0,cnt;
	char strBuf[116] = {0};
	char *strPtr;
	char *strPtr2;
	char *strRMGMC[20];

	t_avienc_t *enc_t = (t_avienc_t *)v_enc_t;
	struct tm *tmprint;

	while(gptMsgShare.TimeLock)
		vTaskDelay(1);
	
	gptMsgShare.TimeLock = 1;
	tmprint = localtime(&gptMsgShare.TIME);
	gptMsgShare.TimeLock = 0;
	sprintf(enc_t->now_name, enc_t->file_format, tmprint->tm_year+1900-2000, tmprint->tm_mon+1, tmprint->tm_mday, tmprint->tm_hour, tmprint->tm_min, tmprint->tm_sec);
	dprintf("time:%s",enc_t->now_name);
#if 0
#if 0 //EXIF use
	if(strlen(saved_info_gps))
	{
		dprintf("Gps:%s",saved_info_gps);
		strcpy(strBuf, saved_info_gps);
		strPtr = strstr(strBuf, "*");
		*strPtr = ',';
		strPtr = strBuf;
		for(cnt=0;strPtr;cnt++)	// strsep func
		{
			strRMGMC[cnt] = strPtr;
			strPtr = strstr(strPtr, ",");
			if(strPtr != NULL)
			{
				*strPtr = NULL;
				strPtr += 1;
			}
		}
		if((strcmp(strRMGMC[2], "A") == 0)&&(strcmp(strRMGMC[12], "A") == 0 || strcmp(strRMGMC[12], "D") == 0))
		{
			temp=strlen(strRMGMC[3]);
			if(temp)
			{
				if(temp>10)
					temp=10;
				for(temp0=0;temp0<temp;temp0++)
				{
					enc_t->now_name[16+temp0]=strRMGMC[3][temp0];
					if(strRMGMC[3][temp0]=='.')
						enc_t->now_name[16+temp0]='_';
				}
				if(temp0<10)
				{
					for(;temp0<10;temp0++)
					{
						enc_t->now_name[16+temp0]='0';
					}
					enc_t->now_name[20]='_';
				}
			}
			if(strlen(strRMGMC[4]))
			{
				enc_t->now_name[26]=strRMGMC[4][0];
			}
			temp=strlen(strRMGMC[5]);
			if(temp)
			{
				if(temp>11)
					temp=11;
				for(temp0=0;temp0<temp;temp0++)
				{
					enc_t->now_name[28+temp0]=strRMGMC[5][temp0];
					if(strRMGMC[5][temp0]=='.')
						enc_t->now_name[28+temp0]='_';
				}
				if(temp0<11)
				{
					for(;temp0<11;temp0++)
					{
						enc_t->now_name[28+temp0]='0';
					}
					enc_t->now_name[33]='_';
				}
			}
			if(strlen(strRMGMC[6]))
			{
				enc_t->now_name[39]=strRMGMC[6][0];
			}
			
			temp=strlen(strRMGMC[7]);
			if(temp)
			{
				if(temp>5)
					temp=5;
				for(temp0=0;temp0<temp;temp0++)
				{
					enc_t->now_name[41+temp0]=strRMGMC[7][temp0];
					if(strRMGMC[7][temp0]=='.')
						enc_t->now_name[41+temp0]='_';
				}
				if(temp0<5)
				{
					for(;temp0<5;temp0++)
					{
						enc_t->now_name[41+temp0]='0';
					}
					enc_t->now_name[42]='_';
				}
			}
			temp=strlen(strRMGMC[8]);
			if(temp)
			{
				if(temp>5)
					temp=5;
				for(temp0=0;temp0<temp;temp0++)
				{
					enc_t->now_name[47+temp0]=strRMGMC[8][temp0];
					if(strRMGMC[8][temp0]=='.')
						enc_t->now_name[47+temp0]='_';
				}
				if(temp0<5)
				{
					for(;temp0<5;temp0++)
					{
						enc_t->now_name[47+temp0]='0';
					}
					enc_t->now_name[49]='_';
				}
			}
		}
		dprintf(" ]]\n");
	}
#else


#ifdef __JPEGEXIF__	
	///default gps data for test
	//strcpy(saved_info_gps,"$GPRMC,041553.00,A,3714.72993,N,12703.60998,E,1.100,,100316,,,A*7D");
	//strcpy(saved_info_gps,"$GPRMC,124030.00,A,3714.79350,N,12703.60758,GSV,3,1,12,01,40,189,,07,56,274,18,08,66,019,27,09,02,232,05*79");
	//strcpy(saved_info_gps,"$GPRMC,090440.00,A,4717.11437,N,00833.91522,E,0.004,77.52,021215,,,A*71");
	//strcpy(saved_info_gps,"$GPRMC,090440.00,A,17.11437,S,33.91522,W,0.004,77.52,021215,,,A*71");
	//printf("\nGPS %s \n",saved_info_gps);
	
	strcpy(enc_t->gpsdata, "0000_00000N-00000_00000E-0_000-00_00");

	if(strlen(saved_info_gps))
	{
		dprintf("Gps:%s\n",saved_info_gps);
		strcpy(strBuf, saved_info_gps);
		strPtr = strstr(strBuf, "*");
		*strPtr = ',';
		strPtr = strBuf;
		for(cnt=0;strPtr;cnt++)	// strsep func
		{
			if(cnt>13)
				break;
			//strcpy(enc_t->pstrRMGMC[cnt],strPtr);
			strPtr2 = strPtr;
			strPtr = strstr(strPtr, ",");
			if(strPtr != NULL)
			{
				*strPtr = NULL;
				strPtr += 1;
				if(strlen(strPtr2))
					strcpy(enc_t->pstrRMGMC[cnt],strPtr2);
				else
					strcpy(enc_t->pstrRMGMC[cnt],"");
			}
		}
	    
		exif_parser(enc_t);
		//dprintf(" ]]\n");
	}	  
	else
	{
		exif_default(enc_t);
	}
#else
	cnt=0;
	for(temp=0;temp<112;temp++)
	{
		if(saved_info_gps[temp]==',')
		{
			cnt++;	
			if(saved_info_gps[temp+1]!=',')
			{
				if(cnt==3)
				{
					temp++;
					for(temp0=0;temp0<10;temp0++)
					{
						enc_t->now_name[16+temp0]=saved_info_gps[temp+temp0];
					}
					enc_t->now_name[20]='_';
					temp +=temp0-1;
				}
				else if(cnt==4)
				{
					temp++;
					enc_t->now_name[26]=saved_info_gps[temp];  //N S
				}
				else if(cnt==5)
				{
					temp++;
					for(temp0=0;temp0<11;temp0++)
					{
						enc_t->now_name[28+temp0]=saved_info_gps[temp+temp0];
					}
					enc_t->now_name[33]='_';
					temp +=temp0-1;
				}
				else if(cnt==6)
				{
					temp++;
					enc_t->now_name[39]=saved_info_gps[temp];  //W E
				}
				else if(cnt==7)
				{
					temp++;
					for(temp0=0;temp0<5;temp0++)
					{
						enc_t->now_name[41+temp0]=saved_info_gps[temp+temp0];
					}
					enc_t->now_name[42]='_';
					temp +=temp0-1;
				}
				else if(cnt==8)
				{
					temp++;
					for(temp0=0;temp0<5;temp0++)
					{
						enc_t->now_name[47+temp0]=saved_info_gps[temp+temp0];
					}
					enc_t->now_name[49]='_';
					temp +=temp0-1;
					break;
				}
			}
		}
	}

#endif
#endif	
#endif
	LEAVE();
}
#ifdef AVIENC_HEAP_USE
static int avienc_info_file_delete(void *v_enc_t, UINT nMaxDelete, INT nFreeSize)
{
	ENTER();

	t_avienc_t *enc_t = (t_avienc_t *)v_enc_t;
	UINT cnt = 0;
	char strBuf[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};
	char strtemp[_MAX_LFN+1] = {0};
	FRESULT fres;

	if(nFreeSize <= 0)
		nFreeSize = fat_get_freesize(MMC);

	while(nFreeSize < MIX_FREE_SD_4CENT)
	{
		if(encHeap_isEmpty(enc_t->filenamelist))
		{	// 파일리스트에 파일 없는데..? 이런 케이스는 발생하면 안된다.
			// 주차모드disable인 경우 주차모션, 주차충격에서 발생할 수 있다
			printf("%s(%d) : filenamelist empty / type(%d)\n", __func__, __LINE__, enc_t->type);
			break;
		}
		encHeap_deleteQueue(enc_t->filenamelist, strBuf);

		
		sprintf(strDelBuf, "%s/%s", enc_t->dir_name, strBuf);
		if(fres=f_unlink(strDelBuf) != FR_OK)
			printf("info f_unlink fail %s   (%s)\n", strDelBuf, put_rc(fres));
		else
			printf("info f_unlink ok %s\n", strDelBuf);

#ifdef __JPEG__
		
		strncpy(strtemp, strBuf, strlen(strBuf)-4);
		sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
		if(fres=f_unlink(strDelBuf) != FR_OK)
			printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
		else
			printf("info jpeg f_unlink ok %s\n", strDelBuf);
#endif		
		
		if(++cnt>nMaxDelete)
			break;

		nFreeSize = fat_get_freesize(MMC);
	}

	LEAVE();
	return nFreeSize;
}
#else
static int avienc_info_file_delete(void *v_enc_t)
{
	ENTER();

	t_avienc_t *enc_t = (t_avienc_t *)v_enc_t;
	UINT res = FR_OK;
	UINT size = 0;
	UINT cnt = 0;
	char strBuf[_MAX_LFN+1] = {0};	
	char strjpgBuf[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};
	char strtemp[_MAX_LFN+1] = {0};
	FRESULT fres;
	int maxsize;

	//printf("%s(%d) : file free size type(%d)\n", __func__, __LINE__, enc_t->type);
	size = fat_get_freesize(MMC);
	
	while(size < MIX_FREE_SD_4CENT)
	{
		res = fat_get_firstfile(enc_t->dir_name, strBuf);

		if(FR_OK != res)
			return res;
		
		printf("enc_t->dir_name %s strBuf (%s)\n", enc_t->dir_name, strBuf);
		sprintf(strDelBuf, "%s/%s", enc_t->dir_name, strBuf);

		fres = f_unlink(strDelBuf);
		if(fres != FR_OK)
		{
			printf("info f_unlink first fail %s(%s)\n", strDelBuf, put_rc(fres));
			fres = f_unlink(strDelBuf);
			if(fres != FR_OK)
			{
				printf("info f_unlink second fail %s(%s)\n", strDelBuf, put_rc(fres));
			}
			else
				printf("info f_unlink ok %s\n", strDelBuf);
#ifdef __JPEG__					
			strncpy(strtemp, strBuf, strlen(strBuf)-4);
			sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
			if(fres=f_unlink(strDelBuf) != FR_OK)
				printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
			else
				printf("info jpeg f_unlink ok %s\n", strDelBuf);
#endif			
		}
		else
			printf("info f_unlink ok %s\n", strDelBuf);
#if 0
		sprintf(strDelBuf, "%d:/%s", MMC, MMC_DIR_THUMBNAIL);
		res = fat_get_firstfile(strDelBuf, strBuf);		
		sprintf(strDelBuf, "%d:/%s/%s", MMC, MMC_DIR_THUMBNAIL, strBuf);
		
		if(fres=f_unlink(strDelBuf) != FR_OK)
			printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
		else
			printf("info jpeg f_unlink ok %s\n", strDelBuf);
#endif		
#ifdef __JPEG__
		
		strncpy(strtemp, strBuf, strlen(strBuf)-4);
		sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
		if(fres=f_unlink(strDelBuf) != FR_OK)
			printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
		else
			printf("info jpeg f_unlink ok %s\n", strDelBuf);
				
#endif		
		if(++cnt>100)
			break;
		size = fat_get_freesize(MMC);
	}

	LEAVE();
	return res;
}

#endif

void avienc_info_sbuf_init(t_avienc_t *enc_t)
{
	ENTER();

	UINT video_frame_cnt = 0;
	UINT audio_frame_cnt = 0;
	switch(enc_t->type)
	{
		case AVI_NORMAL_SW:
		case AVI_EVENT_SW:
		case AVI_MANUAL_SW:
			video_frame_cnt = NUM_STREAM_H264;
			audio_frame_cnt = NUM_STREAM_AUDIO;
			break;
		default:
			printf("%s(%d) : Type[%d] Error?\n", __func__, __LINE__, enc_t->type);
			video_frame_cnt = NUM_STREAM_H264;
			audio_frame_cnt = NUM_STREAM_AUDIO;
			break;
	}
	
	if(sbuf_init(&enc_t->h264_sbuf, video_frame_cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi H.264 Sbuffer init Fail\n", enc_t->type);
	}
#ifdef __AUDIO__
	if(sbuf_init(&enc_t->g711_sbuf, audio_frame_cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi G.711 Sbuffer init Fail\n", enc_t->type);
	}
#endif
#ifdef __AVI_TXTS__
	if(sbuf_init(&enc_t->txt1_sbuf, NUM_STREAM_GPS) == DEF_FAIL)
	{
		printf("Type[%d] Avi TXT1 Sbuffer init Fail\n", enc_t->type);
	}
/*
	if(sbuf_init(&enc_t->txt2_sbuf, NUM_STREAM_ACC) == DEF_FAIL)
	{
		printf("Type[%d] Avi TXT2 Sbuffer init Fail\n", enc_t->type);
	}
*/
#if 0
	if(sbuf_init(&enc_t->txt3_sbuf, NUM_STREAM_TIME) == DEF_FAIL)
	{
		printf("Type[%d] Avi TXT3 Sbuffer init Fail\n", enc_t->type);
	}
	if(sbuf_init(&enc_t->txt4_sbuf, NUM_STREAM_OBD) == DEF_FAIL)
	{
		printf("Type[%d] Avi TXT4 Sbuffer init Fail\n", enc_t->type);
	}
#endif
#endif
	
	LEAVE();
}

void avienc_info_sbuf_tot(t_avienc_t *enc_t, int cnt)
{
	if(sbuf_tot(&enc_t->h264_sbuf, cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi H.264 Sbuffer sbuf_tot Fail\n", enc_t->type);
	}
#ifdef __AUDIO__
	if(sbuf_tot(&enc_t->g711_sbuf, cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi G.711 Sbuffer sbuf_tot Fail\n", enc_t->type);
	}
#endif
#ifdef __AVI_TXTS__
	if(sbuf_tot(&enc_t->txt1_sbuf, cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi txt1_sbuf Sbuffer sbuf_tot Fail\n", enc_t->type);
	}
/*
	if(sbuf_tot(&enc_t->txt2_sbuf, cnt) == DEF_FAIL)
	{
		printf("Type[%d] Avi txt2_sbuf Sbuffer sbuf_tot Fail\n", enc_t->type);
	}
*/
#endif

	
}
void avienc_info_sbuf_reset(t_avienc_t *enc_t)
{
	ENTER();

	CRITICAL_BEGIN;

	enc_t->h264_sbuf.head = enc_t->h264_sbuf.tail = 0;
	if(enc_t->h264_sbuf.data)
		memset(enc_t->h264_sbuf.data, 0, sizeof(sbuf_data)*enc_t->h264_sbuf.tot_num);
#ifdef __AUDIO__
	enc_t->g711_sbuf.head = enc_t->g711_sbuf.tail = 0;
	if(enc_t->g711_sbuf.data)
		memset(enc_t->g711_sbuf.data, 0, sizeof(sbuf_data)*enc_t->g711_sbuf.tot_num);
#endif
#ifdef __AVI_TXTS__
	enc_t->txt1_sbuf.head = enc_t->txt1_sbuf.tail = 0;
	if(enc_t->txt1_sbuf.data)
		memset(enc_t->txt1_sbuf.data, 0, sizeof(sbuf_data)*enc_t->txt1_sbuf.tot_num);
/*	enc_t->txt2_sbuf.head = enc_t->txt2_sbuf.tail = 0;
	if(enc_t->txt2_sbuf.data)
		memset(enc_t->txt2_sbuf.data, 0, sizeof(sbuf_data)*enc_t->txt2_sbuf.tot_num);
*/
#if 0
	enc_t->txt3_sbuf.head = enc_t->txt3_sbuf.tail = 0;
	if(enc_t->txt3_sbuf.data)
		memset(enc_t->txt3_sbuf.data, 0, sizeof(sbuf_data)*enc_t->txt3_sbuf.tot_num);
	enc_t->txt4_sbuf.head = enc_t->txt4_sbuf.tail = 0;
	if(enc_t->txt4_sbuf.data)
		memset(enc_t->txt4_sbuf.data, 0, sizeof(sbuf_data)*enc_t->txt4_sbuf.tot_num);
#endif
#endif

	CRITICAL_END;

	LEAVE();
}

void avienc_info_init_savetime(t_avienc_t *enc_t, UINT savetime, UINT video_fps, UINT audio_fps, UINT txt1_fps, UINT txt2_fps, UINT txt3_fps, UINT txt4_fps)
{
	enc_t->idx_len = 0;

	enc_t->aviinfo.option = AVI_VIDEO;
	enc_t->aviinfo.un32vidsmax = savetime * video_fps;
	//UINT total_frame = (savetime * video_fps);
	UINT total_frame = enc_t->aviinfo.un32vidsmax;
#ifdef __AUDIO__
	if(audio_fps)
	{
		enc_t->aviinfo.option |= AVI_AUDIO;
		enc_t->aviinfo.un32audsmax = savetime * audio_fps;
		total_frame += (savetime * audio_fps);
	}
#endif
#ifdef __AVI_TXTS__
	if(txt1_fps)
	{
		enc_t->aviinfo.option |= AVI_TXTS1;
		enc_t->aviinfo.un32txt1max = savetime * txt1_fps;	// 1s : Gsensor
		total_frame += (savetime * txt1_fps);
	}
/*	
	if(txt2_fps)
	{
		enc_t->aviinfo.option |= AVI_TXTS2;
		enc_t->aviinfo.un32txt2max = savetime * txt2_fps;	// 1s : GPS
		total_frame += (savetime * txt2_fps);
	}
*/
#if 0
	if(txt3_fps)
	{
		enc_t->aviinfo.option |= AVI_TXTS3;
		enc_t->aviinfo.un32txt3max = savetime * txt3_fps;	// 1s : Time
		total_frame += (savetime * txt3_fps);
	}
	if(txt4_fps)
	{
		enc_t->aviinfo.option |= AVI_TXTS4;
		enc_t->aviinfo.un32txt4max = savetime * txt4_fps;	// 1s : OBD
		total_frame += (savetime * txt4_fps);
	}
#endif
#endif
	if(enc_t->idx_info == NULL)
	{
		enc_t->idx_info = pvPortMalloc(16*total_frame);
		if(enc_t->idx_info == NULL)
		{
			printf("Type[%d] idx_info malloc fail\n", enc_t->type);
		}
	}
}

#ifdef AVIENC_HEAP_USE
void avienc_info_init_filenamelist(t_avienc_t *enc_t, int maxfilecount)
{
	enc_t->filenamelist = encHeap_create(maxfilecount);
	if(enc_t->filenamelist == NULL)
	{
		printf("Type[%d] Avi H.264 filenamelist init Fail\n", enc_t->type);
		return;
	}

	char strBuf[_MAX_LFN+1] = {0};
	TCHAR strFileName[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};
	char strtemp[_MAX_LFN+1] = {0};
	FRESULT fres;
	FILINFO finfo;
	DIR dir;

	fres = f_opendir(&dir, enc_t->dir_name);
	finfo.lfname = strFileName;
	finfo.lfsize = _MAX_LFN;
	for(;;)
	{
		fres = f_readdir(&dir, &finfo);
		if((fres != FR_OK) || !finfo.fname[0])
		{	// file 없음
			fres = FR_OK;
			break;
		}

		if((finfo.fattrib & AM_DIR) == 0)
		{
			if(dir.lfn_idx == 0xFFFF)
			{
				// 15.05.13 : short file name은 존재할 수 없다. 해당 파일 삭제
				sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
				if(f_unlink(strBuf) != FR_OK)
					printf("info f_unlink(name format) fail %s\n", strBuf);
				else
					printf("info f_unlink(name format) ok %s\n", strBuf);
#ifdef __JPEG__
		
				strncpy(strtemp, finfo.fname, strlen(finfo.fname)-4);
				sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
				if(fres=f_unlink(strDelBuf) != FR_OK)
					printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
				else
					printf("info jpeg f_unlink ok %s\n", strDelBuf);
#endif								
			}
			else
			{
				// format : [XXX_YYMMDD_hhmmss.avi] <- 21byte
				if(strlen(finfo.lfname) == SAVE_FILENAME_LENS)
				{
					if(finfo.fsize == 0)
					{	// file size zero / how? -> delete?
						sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.lfname);
						//remove_chain(dj.fs, dclst);
						if(f_unlink(strBuf) != FR_OK)
							printf("info f_unlink(zero size) fail %s\n", strBuf);
						else
							printf("info f_unlink(zero size) ok %s\n", strBuf);
						
#ifdef __JPEG__

						strncpy(strtemp, finfo.lfname, strlen(finfo.lfname)-4);
						sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
						
						if(fres=f_unlink(strDelBuf) != FR_OK)
							printf("info jpeg f_unlink(zero size) fail %s(%s)\n", strDelBuf, put_rc(fres));
						else
							printf("info jpeg f_unlink(zero size) ok %s\n", strDelBuf);
#endif


					}
					else if(encHeap_addQueue(enc_t->filenamelist, finfo.lfname) == 1)
					{	// heap가 overflow 되어(삽입은 성공상태) 삭제할 file이 존재함
						sprintf(strBuf, "%s/%s", enc_t->dir_name, enc_t->filenamelist->delHData.filename);
						if(f_unlink(strBuf) != FR_OK)
							printf("info f_unlink(heap over) fail %s\n", strBuf);
						else
							printf("info f_unlink(heap over) ok %s\n", strBuf);
#ifdef __JPEG__
						
						strncpy(strtemp, enc_t->filenamelist->delHData.filename, strlen(enc_t->filenamelist->delHData.filename)-4);
						sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
						if(fres=f_unlink(strDelBuf) != FR_OK)
							printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
						else
							printf("info jpeg f_unlink ok %s\n", strDelBuf);						
#endif		
						memset(&enc_t->filenamelist->delHData, 0, sizeof(encHData));
					}
				}
				else
				{
					// 15.05.13 : long file name중 21byte가 아닌것은 존재할 수 없다. 해당 파일 삭제
					sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.lfname);
					if(f_unlink(strBuf) != FR_OK)
						printf("info f_unlink(name format) fail %s\n", strBuf);
					else
						printf("info f_unlink(name format) ok %s\n", strBuf);
#ifdef __JPEG__
					
					strncpy(strtemp, finfo.lfname, strlen(finfo.lfname)-4);
					sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
					if(fres=f_unlink(strDelBuf) != FR_OK)
						printf("info jpeg f_unlink fail %s(%s)\n", strDelBuf, put_rc(fres));
					else
						printf("info jpeg f_unlink ok %s\n", strDelBuf);
#endif
					
				}
			}
		}
	}
	f_closedir(&dir);
}
#endif

#ifdef CONTIGUOUS_WRITE  
void avienc_info_default(t_avienc_t *enc_t, UINT offset, UINT start, UINT end, UINT len, UINT len2)
{
	enc_t->state = AVIENC_OFF;
	enc_t->buffer.buf = enc_t->buffer.base;//SDSAVE_BUF_BASE;
	enc_t->buffer.length = 0;
	enc_t->clust_offset = offset;
	enc_t->clust_start = start;
	enc_t->clust_end = end;
	enc_t->maxdata_len = len;
	enc_t->maxthumb_len = len2;
}
#else
void avienc_info_default(t_avienc_t *enc_t)
{
	enc_t->state = AVIENC_OFF;
	enc_t->buffer.buf =  enc_t->buffer.base;
	enc_t->buffer.length = 0;
}
#endif
void avienc_info_init(t_avienc_t *enc_t, int type, char *dir_name, char *file_format)
{
	ENTER();

	memset((char *)enc_t, 0, sizeof(t_avienc_t));

	enc_t->type = type;
	enc_t->state = AVIENC_OFF;

	sprintf(enc_t->dir_name, "%d:/%s", MMC, dir_name);
	
	strcpy(enc_t->file_format, file_format);
	
	//printf("avienc_info_init enc_t->file_format %s %d\n",enc_t->file_format,strlen(enc_t->file_format));
	enc_t->pf_sd_free = avienc_info_file_delete;
	enc_t->pf_get_file_name = avienc_info_file_getname;
#if 1
	enc_t->buffer.base = pvPortMalloc(SDSAVE_BUF_SIZE);
	if(enc_t->buffer.base == NULL)
	{
		printf("%s(%d) : Make AVI Buffer malloc Fail(type:%d)\r\n", __func__, __LINE__, type);
		while(1);
	}
	else
	{
		printf("%s(%d) : Make AVI Buffer malloc(0x%08X)\r\n", __func__, __LINE__, enc_t->buffer.base);
	}
	enc_t->buffer.end = enc_t->buffer.base + SDSAVE_BUF_SIZE;
	enc_t->buffer.buf = enc_t->buffer.base;
	enc_t->buffer.length = 0;
	enc_t->buffer.total = SDSAVE_BUF_SIZE;
#else
	enc_t->buffer.buf = SDSAVE_BUF_BASE;
	enc_t->buffer.base = SDSAVE_BUF_BASE;
	enc_t->buffer.end = SDSAVE_BUF_END;
	enc_t->buffer.length = 0;
	enc_t->buffer.total = SDSAVE_BUF_SIZE;
#endif
	LEAVE();
}

void avienc_info_deinit(t_avienc_t *enc_t)
{
	ENTER();
#ifdef AVIENC_HEAP_USE
	if(enc_t->filenamelist != NULL)
		encHeap_destroy(enc_t->filenamelist);
#endif

	enc_t->idx_len = 0;

	if(enc_t->buffer.base)
	{
		vPortFree(enc_t->buffer.base);
		enc_t->buffer.base = NULL;
	}

	if(enc_t->idx_info)
	{
		vPortFree(enc_t->idx_info);
		enc_t->idx_info = NULL;
	}

	LEAVE();
}

/**
	@return 
	0    : fail
	else : success 
*/
int avienc_info_putVideo(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->h264_sbuf, addr, size, idr);
}

#ifdef __AUDIO__
/**
	@return 
	0    : fail
	else : success 
*/
int avienc_info_putAudio(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->g711_sbuf, addr, size, idr);
}
#endif


#ifdef __AVI_TXTS__
/**
	@return 
	0    : fail
	else : success 
*/
int avienc_info_putTxt1(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->txt1_sbuf, addr, size, idr);
}
int avienc_info_putTxt2(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->txt2_sbuf, addr, size, idr);
}
#if 0
int avienc_info_putTxt3(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->txt3_sbuf, addr, size, idr);
}
int avienc_info_putTxt4(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr)
{
	return sbuf_put(&enc_t->txt4_sbuf, addr, size, idr);
}
#endif
#endif

#endif
