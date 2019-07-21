#include <stdlib.h>									// for atoi, qsort

#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "ff.h"
#include "file.h"
#include "stream_buffering.h"

#include "muxer_avienc.h"
#include "avienc_info.h"

#include "msg.h"

extern void *pvPortMalloc( size_t xWantedSize );	// @heap_3
extern void vPortFree( void *pv );					// @heap_3

#if 0
#define ENTER() dprintf("%s IN\r\n", __func__);
#define LEAVE() dprintf("%s OUT\r\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#ifdef __DEVICE_SD__

static void avienc_info_file_getname(void *v_enc_t)
{
	ENTER();

	t_avienc_t *enc_t = (t_avienc_t *)v_enc_t;
	++(enc_t->mLastFileNum);
	if(enc_t->mLastFileNum > MAX_FILE_NUM)
		enc_t->mLastFileNum = 0;
	sprintf(enc_t->now_name, enc_t->file_format, enc_t->mLastFileNum);
#ifdef __IDX_WRITE__
//	sprintf(period_avi.idx_name, "%d:/%s/"SAVE_NAME_FORMAT".idx", MMC, SAVE_DIR, period_avi.mLastFileNum);
#endif

	LEAVE();
}

static int avienc_info_file_delete(void *v_enc_t, UINT nMaxDelete)
{
	ENTER();

	t_avienc_t *enc_t = (t_avienc_t *)v_enc_t;
	UINT res = 0;
	UINT cnt = 0;
	char strBuf[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};

	res = fat_get_freesize(MMC);
	while(res < MIX_FREE_SD_4CENT)
	{
		sprintf(strBuf, enc_t->file_format, enc_t->mFirstFileNum);
		sprintf(strDelBuf, "%s/%s", enc_t->dir_name, strBuf);

		if(f_unlink(strDelBuf) != FR_OK)
		{
			printf("f_unlink fail %s\r\n", strDelBuf);
		}
		else
		{
			printf("f_unlink ok %s\r\n", strDelBuf);
		}

		++(enc_t->mFirstFileNum);
		if(enc_t->mFirstFileNum > MAX_FILE_NUM)
			enc_t->mFirstFileNum = 0;
		
		if(++cnt>nMaxDelete)
			break;

		res = fat_get_freesize(MMC);
	}

	LEAVE();
	return res;
}

void avienc_info_sbuf_init(t_avienc_t *enc_t)
{
	ENTER();

	UINT video_frame_cnt = 0;
	UINT audio_frame_cnt = 0;
	UINT txts_frame_cnt = 0;
	UINT total_frame_cnt = 0;
	switch (enc_t->type) {
		case eRecNormal:
		case eRecEvent:
			video_frame_cnt = NUM_STREAM_H264;
#if (FAT_SDSAVE_AUD==1)
			audio_frame_cnt = NUM_STREAM_AUDIO;
#endif
#ifdef __AVI_TXTS__
			txts_frame_cnt = NUM_STREAM_ACC;
#endif
			break;
		default:
			printf("%s(%d) : Type[%d] Error?\r\n", __func__, __LINE__, enc_t->type);
			video_frame_cnt = NUM_STREAM_H264;
#if (FAT_SDSAVE_AUD==1)
			audio_frame_cnt = NUM_STREAM_AUDIO;
#endif
#ifdef __AVI_TXTS__
			txts_frame_cnt = NUM_STREAM_ACC;
#endif
			break;
	}

	total_frame_cnt = video_frame_cnt + audio_frame_cnt + txts_frame_cnt;
	if (sbuf_init(&enc_t->sbuf, total_frame_cnt) == DEF_FAIL) {
		printf("Type[%d] Avi Total Sbuffer init Fail\r\n", enc_t->type);
	}

	LEAVE();
}

void avienc_info_sbuf_reset(t_avienc_t *enc_t)
{
	ENTER();

	portENTER_CRITICAL();

	enc_t->sbuf.head = enc_t->sbuf.tail = 0;
	if (enc_t->sbuf.data) {
		memset(enc_t->sbuf.data, 0, sizeof(sbuf_data) * enc_t->sbuf.tot_num);
	} else {
		printf("%s, sbuf.data NULL! Error!\r\n", __func__);
	}

	portEXIT_CRITICAL();

	LEAVE();
}

void avienc_info_init_savetime(t_avienc_t *enc_t, UINT savetime, UINT video_fps, UINT audio_fps, UINT txts_fps)
{
	enc_t->aviinfo.option = AVI_VIDEO;
	enc_t->aviinfo.un32vidsmax = savetime * video_fps;
	UINT total_frame = enc_t->aviinfo.un32vidsmax;
#ifdef __AUDIO__
	if (audio_fps) {
		enc_t->aviinfo.option |= AVI_AUDIO;
		enc_t->aviinfo.un32audsmax = savetime * audio_fps;
		total_frame += enc_t->aviinfo.un32audsmax;
	}
#endif
#ifdef __AVI_TXTS__
	if (txts_fps) {
		enc_t->aviinfo.option |= AVI_TXTS;
		enc_t->aviinfo.un32txtsmax = savetime * txts_fps;
		total_frame += enc_t->aviinfo.un32txtsmax;
	}
#endif

	// total값보다 10% 더 많이 index buffer을 할당해준다.
	// 이것은 Video frame의 기준으로 녹화를 하고 있으므로, 
	// Video frame이 아닌 다른 frame이 더 많을 때 발생할 오버플로우를 
	// 방지하기 위해 index buffer을 추가 할당한다.
	//total_frame += (total_frame / 10);
	total_frame += (total_frame / 2);

	enc_t->idx_len = 0;
#ifndef __IDX_WRITE__
	if (enc_t->idx_size < total_frame) {
		enc_t->idx_size = total_frame;
		if (enc_t->idx_info == NULL) {
			enc_t->idx_info = pvPortMalloc(16 * total_frame);
			if (enc_t->idx_info == NULL) {
				printf("Type[%d] idx_info malloc fail\r\n", enc_t->type);
			}
		} else {
			enc_t->idx_info = pvPortRealloc(enc_t->idx_info, 16 * total_frame);
			if (enc_t->idx_info == NULL) {
				printf("Type[%d] idx_info realloc fail\r\n", enc_t->type);
			}
		}
	}
#endif
	UNUSED(audio_fps);
	UNUSED(txts_fps);
}

static int filenumber_compar(const void *a, const void *b)
{
	UINT wa = *(UINT *)a;
	UINT wb = *(UINT *)b;
	if(wa >= wb)	return 1;
	else			return -1;
}

void avienc_info_init_filenamelist(t_avienc_t *enc_t, int maxfilecount)
{
	char strBuf[_MAX_LFN+1] = {0};
	TCHAR strFileName[_MAX_LFN+1] = {0};
	FRESULT fres;
	FILINFO finfo;
	DIR dir;
	char *fn, *pcP;
	UINT nNum, nMaxNum = 0, nMinNum = maxfilecount - 1;
	UINT nCount = 0;

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
			if(dir.lfn_idx == 0xFFFF)	// short name
			{
				// format : [XXX_0000.avi] <- 12byte
				if(strlen(finfo.fname) == SAVE_FILENAME_LENS)
				{
					if(finfo.fsize == 0)
					{	// file size zero / how? -> delete?
						sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
						if(f_unlink(strBuf) != FR_OK)
							printf("f_unlink(zero size) fail %s\r\n", strBuf);
						else
							printf("f_unlink(zero size) ok %s\r\n", strBuf);
					}
					else
					{
						fn = finfo.fname;
						pcP = strchr(fn, '.');	// Find first char of suffix
						if(pcP)
						{
							if(strcmp(pcP, ".avi") == 0)
							{	// Suffix is corresponding ?
								fn[pcP-fn] = '\0';	// Add null character
								nNum = (UINT)atoi(strchr(fn, '_')+1);	// Ascii -> int
								if(nMaxNum <= nNum)	nMaxNum = nNum;
								if(nMinNum >= nNum)	nMinNum = nNum;
								nCount++;
							}
						} 
					}
				}
				else
				{
					// 15.05.13 : short file name중 12byte가 아닌것은 존재할 수 없다. 해당 파일 삭제
					sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.fname);
					if(f_unlink(strBuf) != FR_OK)
						printf("f_unlink(name format) fail %s\r\n", strBuf);
					else
						printf("f_unlink(name format) ok %s\r\n", strBuf);
				}
			}
			else
			{
				// 15.05.13 : long file name은 존재할 수 없다. 해당 파일 삭제
				sprintf(strBuf, "%s/%s", enc_t->dir_name, finfo.lfname);
				if(f_unlink(strBuf) != FR_OK)
					printf("f_unlink(name format) fail %s\r\n", strBuf);
				else
					printf("f_unlink(name format) ok %s\r\n", strBuf);
			}
		}
	}
	fres = f_closedir(&dir);

	if(nMinNum == MAX_FILE_NUM) nMinNum = 0;

	printf("%s : min %d max %d cnt %d\r\n", enc_t->dir_name, nMinNum, nMaxNum, nCount);
	enc_t->mFirstFileNum = nMinNum;
	enc_t->mLastFileNum = nMaxNum;

	if(nMinNum == 0 && nMaxNum == MAX_FILE_NUM)
	{	// 위의 조건이 참이라면 한바퀴 돈 상태로 nMaxNum가 Max인 경우가 아니다.
		UINT *nfilelist = (UINT *)pvPortMalloc(nCount * sizeof(UINT));
		UINT i, nGap = 0, nBigGap = 0, nGapMax = 0x00000000, nGpaMin = 0xFFFFFFFF;
		if(nfilelist == NULL)
		{
			printf("%s(%d) : malloc fail(%s)(%d)\r\n", __func__, __LINE__, enc_t->dir_name, nCount);
			return;
		}

		fres = f_opendir(&dir, enc_t->dir_name);
		for(i=0;;)
		{
			fres = f_readdir(&dir, &finfo);
			if((fres != FR_OK) || !finfo.fname[0])	break;
		
			if((finfo.fattrib & AM_DIR) == 0)
			{
				if(dir.lfn_idx == 0xFFFF)	// short name
				{
					if(strlen(finfo.fname) == SAVE_FILENAME_LENS)
					{	// format : [XXX_0000.avi] <- 12byte
						fn = finfo.fname;
						pcP = strchr(fn, '.');	// Find first char of suffix
						if(pcP)
						{
							if(strcmp(pcP, ".avi") == 0)
							{	// Suffix is corresponding ?
								fn[pcP-fn] = '\0';	// Add null character
								nNum = (UINT)atoi(strchr(fn, '_')+1);	// Ascii -> int
								nfilelist[i] = nNum;
								i++;
							}
						} 
					}
				}
			}
		}
		fres = f_closedir(&dir);
		qsort((char *)nfilelist, nCount, sizeof(UINT), filenumber_compar);
		for(i=0;i<nCount-1;i++)
		{
			nGap = nfilelist[i+1] - nfilelist[i];
			if(nGap > nBigGap)
			{
				nGapMax = nfilelist[i];
				nGpaMin = nfilelist[i+1];
				nBigGap = nGap;
			}
		}
		printf("%s : min %d max %d cnt %d\r\n", enc_t->dir_name, nGpaMin, nGapMax, nCount);
		enc_t->mFirstFileNum = nGpaMin;
		enc_t->mLastFileNum = nGapMax;

		vPortFree(nfilelist);
	}
}

void avienc_info_default(t_avienc_t *enc_t)
{
	enc_t->state = AVIENC_IDLE;
	enc_t->buffer.buf = enc_t->buffer.base;
	enc_t->buffer.length = 0;
#ifdef __IDX_WRITE__
	enc_t->idxbuf.buf = SDSAVE_IDX_BASE;
	enc_t->idxbuf.length = 0;
#endif
}

void avienc_info_init(t_avienc_t *enc_t, int type, char *dir_name, char *file_format)
{
	ENTER();

	memset((char *)enc_t, 0, sizeof(t_avienc_t));

	enc_t->type = type;
	enc_t->state = AVIENC_IDLE;

	sprintf(enc_t->dir_name, "%d:/%s", MMC, dir_name);
	strcpy(enc_t->file_format, file_format);
	enc_t->pf_sd_free = avienc_info_file_delete;
	enc_t->pf_get_file_name = avienc_info_file_getname;

#if 1
	enc_t->buffer.base = pvPortMalloc(SDSAVE_BUF_SIZE);
	if(enc_t->buffer.base == NULL)
	{
		printf("Type[%d] buffer malloc fail\r\n", enc_t->type);
		while(1);
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

#ifdef __IDX_WRITE__
	enc_t->idxbuf.buf = SDSAVE_IDX_BASE;
	enc_t->idxbuf.base = SDSAVE_IDX_BASE;
	enc_t->idxbuf.end = SDSAVE_IDX_END;
	enc_t->idxbuf.length = 0;
	enc_t->idxbuf.total = SDSAVE_IDX_SIZE;
#endif

	enc_t->idx_size = 0;

	LEAVE();
}

void avienc_info_deinit(t_avienc_t *enc_t)
{
	ENTER();

	enc_t->idx_len = 0;

	if (enc_t->buffer.base) {
		vPortFree(enc_t->buffer.base);
		enc_t->buffer.base = NULL;
	}
	
#ifndef __IDX_WRITE__
	if (enc_t->idx_info) {
		vPortFree(enc_t->idx_info);
		enc_t->idx_info = NULL;
	}
#endif

	LEAVE();
}

/**
	@return 
	0    : fail
	else : success 
*/
int avienc_info_put(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr, u32 type)
{
	return sbuf_put(&enc_t->sbuf, addr, size, idr, type);
}
#endif
