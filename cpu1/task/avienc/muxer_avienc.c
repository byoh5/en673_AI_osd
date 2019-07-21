#include <time.h>

#include "dev.h"																
#include "dev_inline.h"	// for inline

#include "msg.h"
#include "user.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "file.h"

#include "muxer_avienc.h"
#include "avienc_info.h"
#include "fmt_avienc.h"

#include "timer_task.h"

#if 0
#define ENTER() dprintf("%s IN\r\n", __func__);
#define LEAVE() dprintf("%s OUT\r\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#ifdef __DEVICE_SD__

//#define ENX_AVISV_DEBUG			// printf 주석용

#ifdef ENX_AVISV_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

t_avienc_t gAvienc[AVI_RecTypeCnt];
#if 0
= {
	{
		.type = eRecNormal,
		.recording = 0,
		.state = AVIENC_IDLE,
		.pf_sd_free = NULL,
		.pf_get_file_name = NULL,
	},
	{
		.type = eRecEvent,
		.recording = 0,
		.state = AVIENC_IDLE,
		.pf_sd_free = NULL,
		.pf_get_file_name = NULL,
	},
};
#endif

const int nFPSvalue[][12] = {
	{60, 30, 20, 15, 12, 10, 6, 5, 4, 3, 2, 1}, 
	{50, 25, 10, 5, 2, 1},
	{30, 15, 10, 6, 5, 3, 2, 1},
	{25, 5, 1},
	{15, 5, 3, 1},
	{12, 6, 4, 3, 2, 1},
};

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

static xSemaphoreHandle bufSem = NULL;
UINT bSD_FAT_mounted = 0;
UINT nAvienc_Mode_now = 0;
UINT nAvienc_Mode_old = 0;
int nAvienc_Fps_old = 0;

typedef enum {
	AVI_OK,
	AVI_FULL,
	AVI_OPENFAIL,
	AVI_WRITEFAIL,
	AVI_WRITEFAIL_DISKFULL
} AVI_ERROR;

t_avienc_t *enc_ing;

extern int muxer_avienc_change_savetime(t_avienc_t *pavi, int start);

/**
    @return 
        1 : mounted.
        0 : nothing.
*/
int muxer_avienc_sdCardMounted(void)
{
    return bSD_FAT_mounted;
}

void muxer_avienc_go(int var)
{
	gAvienc[var].recording = 1;	// 녹화 활성화
}

void muxer_avienc_stop(int var)
{
	gAvienc[var].recording = 0;	// 녹화 비활성화
}

int muxer_avienc_state(int var)
{
	return gAvienc[var].recording;
}

void muxer_avienc_all_stop(void)
{
	muxer_avienc_stop(eRecNormal);
	muxer_avienc_stop(eRecEvent);
#if JPEG_SDSAVE
	muxer_avienc_stop(eRecSnapshot);
#endif
}

t_avienc_t *muxsr_avienc_get_t_avienc_t(void)
{
	return enc_ing;
}

void muxer_avienc_Buffering(u32 addr, u32 size, u32 flag, u32 type)
{
	if (size) {
		avienc_info_put(&gAvienc[eRecNormal], addr, size, flag, type);
		avienc_info_put(&gAvienc[eRecEvent], addr, size, flag, type);
	}
}

void muxer_avienc_error(t_avienc_t *pavi, AVI_ERROR state)
{
	ENTER();

#ifdef __IDX_WRITE__
	f_close(&pavi->fpidx);
	f_unlink(pavi->idx_name);
#endif
	f_close(&pavi->fpavi);

#ifdef GPIO_RECORDING_LED
	GpioSetLo(GPIO_RECORDING_LED);
#endif

	if (state == AVI_FULL) {
		pavi->state = AVIENC_IDLE;
	} else {
		pavi->state = AVIENC_EXIT;
		muxer_avienc_all_stop();
		printf("%s(%d) : AVIENC ERROR\r\n", __func__, __LINE__);
	}

	LEAVE();
}

FRESULT muxer_avienc_bufferflush(t_avienc_t *pavi)
{
	FRESULT fres;
	UINT writesize = 0;

#ifdef GPIO_RECORDING_LED
	if(GpioGetPin(GPIO_RECORDING_LED))	GpioSetLo(GPIO_RECORDING_LED);
	else								GpioSetHi(GPIO_RECORDING_LED);
#endif

	fres = f_write(&(pavi->fpavi), (uint8 *)pavi->buffer.base, pavi->buffer.length, &writesize);
	if (fres != FR_OK) {
		printf("%s Error : f_write File system fail(addr:0x%08X, size:%d)\r\n", __func__, (uint8 *)pavi->buffer.base, pavi->buffer.length);
		printf("Err Msg : %s\r\n", put_rc(fres));
		muxer_avienc_error(pavi, AVI_WRITEFAIL);
		goto done;
	}

	if (pavi->buffer.length != writesize) {
		printf("%s Error : f_write All Write fail.(buff:%d)(write:%d)\r\n", __func__, pavi->buffer.length, writesize);
		if (fat_get_freesize(MMC) == 0) {
			printf("%s : SD freesize zeroG\r\n", __func__);
			muxer_avienc_error(pavi, AVI_WRITEFAIL_DISKFULL);
			goto done;
		}
	}

	if (writesize == 0) {
		if (fat_get_freesize(MMC) == 0) {
			printf("%s : SD freesize zeroF\r\n", __func__);
			muxer_avienc_error(pavi, AVI_WRITEFAIL_DISKFULL);
			goto done;
		}
	}

	pavi->buffer.buf = pavi->buffer.base;	// buffer의 처음으로 이동
	pavi->buffer.length -= writesize;		// buffer에 남은 데이터, 값이 0이여야 한다.
//	pavi->filesize += writesize;

	if (pavi->buffer.length != 0) {
		printf("%s Error : AVI buffer no clear(%d)\r\n", __func__, pavi->buffer.length);
		pavi->buffer.length = 0;
	}

done:
	return fres;
}

// size : -1 : buffer write & buffer no add
// size : >0 : check buffer & buffer add
void muxer_avienc_bufferadd(t_avienc_t *pavi, uint8 *data, int32 size)
{
	UINT left, wb;

	if (!data) {
		if (muxer_avienc_bufferflush(pavi) != FR_OK) {
			printf("%s: bufferflush fail1\r\n", __func__);
		}
		return;
	}

	while (size > 0) {
		left = pavi->buffer.total - pavi->buffer.length;

		if((UINT)size >= left)	wb = left;
		else					wb = size;

		DmaMemCpy_ip_async((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
		pavi->buffer.length += wb;
		pavi->buffer.buf    += wb;
		pavi->filesize      += wb;

		if (pavi->buffer.length == pavi->buffer.total) {
			if (muxer_avienc_bufferflush(pavi) != FR_OK) {
				printf("%s: bufferflush fail2(%dbyte)\r\n", __func__, pavi->filesize);
				break;
			}
		}

		data += wb;
		size -= wb;
	}

	return;
}

#ifdef __IDX_WRITE__
// size : -1 : idxbuf write & idxbuf no add
// size : >0 : check idxbuf & idxbuf add
void muxer_avienc_idxbufadd(t_avienc_t *pavi, AVIINDEX_t *data, uint32 end)
{
	// size over?
	FRESULT fres;
	UINT size = sizeof(AVIINDEX_t);
	UINT writesize = 0;
	if((pavi->idxbuf.buf + size > pavi->idxbuf.end) || end == -1)
	{								// Buffer Write
#ifdef GPIO_RECORDING_LED
		if(GpioGetPin(GPIO_RECORDING_LED))	GpioSetLo(GPIO_RECORDING_LED);
		else								GpioSetHi(GPIO_RECORDING_LED);
#endif

		if(pavi->idxbuf.length <= 0)
		{	// no data
			pavi->idxbuf.length = 0;
			goto done;
		}

		UINT wtime_s = xTaskGetTickCount();
		fres = f_write(&(pavi->fpidx), (uint8 *)pavi->idxbuf.base, pavi->idxbuf.length, &writesize);
		UINT wtime_e = xTaskGetTickCount();
		tprintf("Index File Write(%dms, %dbyte)\r\n", (wtime_e - wtime_s)*10, pavi->idxbuf.length);
		if(fres != FR_OK)
		{
			printf("%s Error : f_write File system fail\r\n", __func__);
			printf("Err Msg : %s\r\n", put_rc(fres));
			muxer_avienc_error(pavi, AVI_WRITEFAIL);
			goto done;
		}

		if(pavi->idxbuf.length != writesize)
		{
			printf("%s Error : f_write All Write fail.(buff:%d)(write:%d)\r\n", __func__, pavi->idxbuf.length, writesize);
			if(fat_get_freesize(MMC) == 0)
			{
				printf("%s : SD freesize zeroD\r\n", __func__);
				muxer_avienc_error(pavi, AVI_WRITEFAIL_DISKFULL);
				goto done;
			}
		}

		if(writesize == 0)
		{
			if(fat_get_freesize(MMC) == 0)
			{
				printf("%s : SD freesize zeroC\r\n", __func__);
				muxer_avienc_error(pavi, AVI_WRITEFAIL_DISKFULL);
				goto done;
			}
		}

		pavi->idxbuf.buf = pavi->idxbuf.base;	// idxbuf의 처음으로 이동
		pavi->idxbuf.length -= writesize;		// idxbuf에 남은 데이터, 값이 0이여야 한다.

		if(pavi->idxbuf.length != 0)
		{
			printf("%s Error : AVI idxbuf no clear(%d)\r\n", __func__, pavi->idxbuf.length);
			pavi->idxbuf.length = 0;
		}

		if(end == -1)
			goto done;
	}

	if(end != -1)
	{
		if(end >= pavi->idxbuf.total)	// Buffer Overflow Processing
		{
#ifdef GPIO_RECORDING_LED
			if(GpioGetPin(GPIO_RECORDING_LED))	GpioSetLo(GPIO_RECORDING_LED);
			else								GpioSetHi(GPIO_RECORDING_LED);
#endif

			UINT wtime_s = xTaskGetTickCount();
			writesize = 0;
			fres = f_write(&(pavi->fpidx), (uint8 *)data, size, &writesize);
			UINT wtime_e = xTaskGetTickCount();
			tprintf("Index File Write(%dms, %dbyte)\r\n", (wtime_e - wtime_s)*10, size);
			if(fres != FR_OK)
			{
				printf("%s Error : f_write File system fail\r\n", __func__);
				printf("Err Msg : %s\r\n", put_rc(fres));
				muxer_avienc_error(pavi, AVI_WRITEFAIL);
				goto done;
			}

			if(size != writesize)
			{
				printf("%s Error : f_write All Write fail.(size:%d)(write:%d)\r\n", __func__, size, writesize);
				if(fat_get_freesize(MMC) == 0)
				{
					tprintf("%s : SD freesize zeroB\r\n", __func__);
					muxer_avienc_error(pavi, AVI_WRITEFAIL_DISKFULL);
					goto done;
				}
			}
		}
		else							// Buffer Add
		{
			DmaMemCpy_ip((BYTE *)pavi->idxbuf.buf, (BYTE *)data, size);

			pavi->idxbuf.buf += size;
			pavi->idxbuf.length += size;
		}
	}

done:
	return;
}
#endif

BYTE *muxer_avienc_makeaviheader(t_avienc_t *pavi)
{
	switch (pavi->state) {
		case AVIENC_OPEN:
			pavi->aviinfo.f_fps = muxer_avienc_change_savetime(NULL, 1);
			pavi->aviinfo.i_width =	listResolution[gtUser.uviH264[0].eResolution].nWidth;
			pavi->aviinfo.i_height = listResolution[gtUser.uviH264[0].eResolution].nHeight;
			pavi->aviinfo.i_riff = 0;
			pavi->aviinfo.i_movi_end = 0;
			pavi->aviinfo.i_header_end = AVI_HEADSIZE - 4;
			pavi->aviinfo.un32vidicount = 0;
			pavi->aviinfo.un32vidscount = 0;
			pavi->aviinfo.un32audscount = 0;
			pavi->aviinfo.un32txtscount = 0;
			return avi_write_header(&(pavi->aviinfo));
			break;
		case AVIENC_CLOSE:
		case AVIENC_EXIT:
			return avi_write_header(&(pavi->aviinfo));
		default:
			printf("%s Error Call\r\n", __func__);
			return NULL;
	}
}

int muxer_avienc_idle(t_avienc_t *pavi);
int muxer_avienc_open(t_avienc_t *pavi);
int muxer_avienc_frist_video(t_avienc_t *pavi);
void muxer_avienc_write_process(t_avienc_t *pavi, int type);
void muxer_avienc_write(t_avienc_t *pavi);
void muxer_avienc_close(t_avienc_t *pavi);

int muxer_avienc_idle(t_avienc_t *pavi)
{
	// 초기화
	pavi->state = AVIENC_OPEN;
	return muxer_avienc_open(pavi);
}

int muxer_avienc_open(t_avienc_t *pavi)
{
	ENTER();
	
	FRESULT fres;

	// Sd Free Size?
	if (pavi->pf_sd_free) {
		if (pavi->pf_sd_free(pavi, MAX_FILE_NUM) == 0) {
			printf("%s : SD freesize zeroA\r\n", __func__);
			muxer_avienc_error(pavi, AVI_FULL);
			goto done;
		}
	}

	// File Name Get
	pavi->pf_get_file_name(pavi);

	// File Path Create
	char strBuf[_MAX_LFN+1] = {0};
	sprintf(strBuf, "%s/%s", pavi->dir_name, pavi->now_name);

	// Start Time Get
	pavi->stime = xTaskGetTickCount();

	// File Open
	if ((fres=f_open(&pavi->fpavi, strBuf, FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK) {
		printf("%s Error : f_open AVI data File system fail : %s\r\n", __func__, strBuf);
		printf("Err Msg : %s\r\n", put_rc(fres));
		muxer_avienc_error(pavi, AVI_OPENFAIL);
		goto done;
	} else {
		UINT stime_end = xTaskGetTickCount();
		printf("f_open(%dms) : %s\r\n", (stime_end - pavi->stime)*10, strBuf);
		gptMsgShare.REC_ON = 1;
#ifdef __IDX_WRITE__
		UINT start_time = xTaskGetTickCount();
		if((fres=f_open(&pavi->fpidx, pavi->idx_name, FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
		{
			f_close(&pavi->fpavi);
			printf("%s Error : f_open IDX data File system fail : %s\r\n", __func__, pavi->idx_name);
			printf("Err Msg : %s\r\n", put_rc(fres));
			muxer_avienc_error(pavi, AVI_OPENFAIL);
			goto done;
		}
		stime_end = xTaskGetTickCount();
		printf("f_open(%dms) : %s\r\n", (stime_end - start_time)*10, pavi->idx_name);
#endif
		pavi->filesize = 0;
		pavi->buffer.buf = pavi->buffer.base;
		pavi->buffer.length = 0;
#ifdef __IDX_WRITE__
		pavi->idxbuf.buf = pavi->idxbuf.base;
		pavi->idxbuf.length = 0;
#endif
		pavi->idx_len = 0;

		// Make AVI Header, Buffer Write AVI Header, Header Size Check
		BYTE *aviheader = muxer_avienc_makeaviheader(pavi);
		muxer_avienc_bufferadd(pavi, aviheader, AVI_HEADSIZE);

		pavi->state = AVIENC_FIRST;
		nAvienc_Mode_old = nAvienc_Mode_now;
		nAvienc_Mode_now = pavi->type;
		return 1;
	}

done:
	LEAVE();
	return 0;
}

int muxer_avienc_frist_video(t_avienc_t *pavi)
{
	if (sbuf_get(&(pavi->sbuf), &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags), &(pavi->datablock.type)) == DEF_OK) { // 데이터 존재확인
		if (pavi->datablock.flags == 1) { // I frame 이라면 초기화
			pavi->state = AVIENC_FIRST_NEXT;
			pavi->datablock.state = 1;	// datablock에 데이터 존재(video)
			return 1;
		}
	}
	return 0;
}

int muxer_avienc_nextI_video(t_avienc_t *pavi)
{
	while (sbuf_get(&(pavi->sbuf), &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags), &(pavi->datablock.type)) == DEF_OK) { // 데이터 존재확인
		if (pavi->datablock.flags == 1) { // I frame 이라면 초기화
			pavi->state = AVIENC_WRITE;
			pavi->datablock.state = 1;	// datablock에 데이터 존재(video)
			muxer_avienc_write(pavi);
			return 1;
		}
	}
	return 0;
}

void muxer_avienc_write_process(t_avienc_t *pavi, int type)
{
	ENTER();

	// index add : 00dc flags pos size 
	int pos = pavi->filesize - pavi->aviinfo.i_header_end;
	AVIINDEX_t avi_idx = avi_write_index(type, pos, pavi->datablock.size, pavi->datablock.flags); // 16byte
#ifdef __IDX_WRITE__
	pavi->idx_len += sizeof(AVIINDEX_t);
	muxer_avienc_idxbufadd(pavi, &avi_idx, 0);
#else
	if (pavi->idx_info != NULL) {
		uint8 *idx_pos = (pavi->idx_info + pavi->idx_len);
		DmaMemCpy_ip((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t));
		pavi->idx_len += sizeof(AVIINDEX_t);
	}
#endif 

	// data add : 00dc size data
	FCC_CB_t fcccb = avi_write_chunkheader(type, pavi->datablock.size);			// 8byte
	muxer_avienc_bufferadd(pavi, (uint8 *)&fcccb, sizeof(FCC_CB_t));
	muxer_avienc_bufferadd(pavi, (uint8 *)pavi->datablock.addr, AVI_ALIGN(pavi->datablock.size));

	pavi->datablock.flags = 0;
	pavi->datablock.size = 0;
	pavi->datablock.addr = 0;
	pavi->datablock.type = 0;
	pavi->datablock.state = 0;

	LEAVE();
}

void muxer_avienc_write(t_avienc_t *pavi)
{
	ENTER();

	if (pavi->datablock.state == 1) {
		int type = -1;
		switch (pavi->datablock.type) {
			case eSTREAMMSG_H2641I:
				(pavi->aviinfo.un32vidicount)++;
			case eSTREAMMSG_H2641P:
				(pavi->aviinfo.un32vidscount)++;
				type = H264_STREAM;
				break;
//			case eSTREAMMSG_H2642I:
//			case eSTREAMMSG_H2642P:
//			case eSTREAMMSG_JPEG:
//				break;
			case eSTREAMMSG_AUDIO:
				(pavi->aviinfo.un32audscount)++;
				type = G711_STREAM;
				break;
			case eSTREAMMSG_TXTS:
				(pavi->aviinfo.un32txtscount)++;
				type = TXTS_STREAM;
				break;
		}

		if (type != -1) {
			muxer_avienc_write_process(pavi, type);
		}
		pavi->datablock.state = 0;
	}

	LEAVE();
}

void muxer_avienc_close(t_avienc_t *pavi)
{
	ENTER();

	FRESULT fres;

	pavi->aviinfo.i_movi_end = pavi->filesize;

	// Write Idx Data
#ifndef __IDX_WRITE__
	if (pavi->idx_info != NULL) {
#endif
		FCC_CB_t fcccb = avi_write_chunkheader(IDX1_ARRAY, pavi->idx_len);
		muxer_avienc_bufferadd(pavi, (uint8 *)&fcccb, sizeof(FCC_CB_t));		// 8byte
#ifndef __IDX_WRITE__
		muxer_avienc_bufferadd(pavi, (uint8 *)pavi->idx_info, AVI_ALIGN(pavi->idx_len));	// 16byte * framecount
#endif
		muxer_avienc_bufferadd(pavi, NULL, -1);	// buffer write(size:-1)
#ifndef __IDX_WRITE__
	}
#endif

#ifdef __IDX_WRITE__
	UINT idx_size = 0;
	muxer_avienc_idxbufadd(pavi, NULL, -1);
	UINT idx_time_s = xTaskGetTickCount();
	if ((fres = f_close(&pavi->fpidx)) == FR_OK) {
		if (f_open(&pavi->fpidx, pavi->idx_name, FA_READ) == FR_OK) {
			UINT uiReadLength, uiWritesize;
			while (1) {
				f_read(&pavi->fpidx, pavi->buffer.base, pavi->buffer.total, &uiReadLength);
				if (uiReadLength == 0) {
					f_close(&pavi->fpidx);
					f_unlink(pavi->idx_name);
					break;
				}
				f_write(&(pavi->fpavi), pavi->buffer.base, uiReadLength, &uiWritesize);
				idx_size += uiWritesize;
			}
		}
	}
	UINT idx_time_e = xTaskGetTickCount();
	float idx_time = (idx_time_e - idx_time_s) / 100.0;
	printf("%s : idx1 write total size(%d/%d) time(%4.2f)\r\n", pavi->now_name, idx_size, pavi->idx_len, idx_time);
	// File Size check End
#endif

	// Make AVI Header, Write AVI Header
	pavi->filesize = f_tell(&(pavi->fpavi));
	pavi->aviinfo.i_riff = pavi->filesize;
	BYTE *aviheader = muxer_avienc_makeaviheader(pavi);
	muxer_avienc_bufferadd(pavi, aviheader, AVI_HEADSIZE);

	f_lseek(&pavi->fpavi, 0);	// fpavi pos : 0
	muxer_avienc_bufferadd(pavi, NULL, -1);

	// File Close
	UINT etime_s = xTaskGetTickCount();
	if ((fres=f_close(&pavi->fpavi)) != FR_OK) {
		printf("%s Error : f_close File system fail\r\n", __func__);
		printf("Err Msg : %s\r\n", put_rc(fres));
	} else { // End Time Get
		pavi->etime = xTaskGetTickCount();
		UINT tot_time = (pavi->etime - pavi->stime)*10;
		printf("f_close(%dms) : %s : %dbyte/%dms/%dKBps\r\n", (pavi->etime - etime_s)*10, pavi->now_name, pavi->aviinfo.i_riff, tot_time, (UINT)(pavi->aviinfo.i_riff/tot_time*8));
	}

	if (pavi->type == eRecEvent) {
		muxer_avienc_stop(eRecEvent);
		muxer_avienc_go(eRecNormal);
	}

#ifdef GPIO_RECORDING_LED
	GpioSetLo(GPIO_RECORDING_LED);
#endif

	pavi->state = AVIENC_IDLE;

	enc_ing = NULL;

	LEAVE();
}

int muxer_avienc_change_savetime(t_avienc_t *pavi, int start)
{
	int fpsval, fpsidx = 2;
	switch (gptMsgShare.VIDEO_FPS) {
		case 60:	fpsidx = 0;		break;
		case 50:	fpsidx = 1;		break;
		case 30:	fpsidx = 2;		break;
		case 25:	fpsidx = 3;		break;
		case 15:	fpsidx = 4;		break;
		case 12:	fpsidx = 5;		break;
	}
	fpsval = nFPSvalue[fpsidx][gtUser.uviH264[0].sfFPS];
	if (fpsval != nAvienc_Fps_old) {
		nAvienc_Fps_old = fpsval;

		if (start == 0) {
			pavi->state = AVIENC_CLOSE;
			muxer_avienc_close(pavi);
		}

		avienc_info_init_savetime(&gAvienc[eRecNormal], SAVE_NOR_TIME_MAX, fpsval, AUDIO_FPS, TXTS_FPS);
		avienc_info_init_savetime(&gAvienc[eRecEvent], SAVE_EVT_TIME_MAX, fpsval, AUDIO_FPS, TXTS_FPS);
	}
	return fpsval;
}

void muxer_avienc_process(t_avienc_t *pavi)
{
	switch (pavi->state) {
		case AVIENC_IDLE: // => AVIENC_OPEN call => AVIENC_FIRST call
//			dprintf("AVIENC_IDLE call\r\n");
			if (gtUser.bSdAviSave == SDSAVE_USE_OFF) {
				muxer_avienc_all_stop();
				printf("%s(%d) : AVIENC EXIT\r\n", __func__, __LINE__);
				break;
			}
			enc_ing = pavi;
			if(muxer_avienc_idle(pavi) == 0)
				break;
		case AVIENC_FIRST: // first_video로 data align을 맞추고(1이 리턴)
			if(muxer_avienc_frist_video(pavi) == 0)
				break;
		case AVIENC_FIRST_NEXT:
			if (nAvienc_Mode_now != nAvienc_Mode_old) { // 현재 인코딩 영상이 이전과 다른 것이라면 1s 버리기
				muxer_avienc_nextI_video(pavi);
				break;
			} else {
				// 현재 인코딩 영상이 이전과 같은 것이라면 녹화 시작
				pavi->state = AVIENC_WRITE;
				muxer_avienc_write(pavi);
			}
		case AVIENC_WRITE: // => AVIENC_WRITE loop => AVIENC_CLOSE call
//			dprintf("AVIENC_WRITE call\r\n");
			if (gtUser.bSdAviSave == SDSAVE_USE_ON) {
				enc_ing = pavi;

				muxer_avienc_change_savetime(pavi, 0);

				if (pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT) {
					break;
				} else if (gtUser.bSdAviSave == SDSAVE_USE_OFF) {
					pavi->state = AVIENC_EXIT;
					break;
				}

				while (sbuf_peek(&pavi->sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags), &(pavi->datablock.type)) == DEF_OK) {
					if ((pavi->datablock.type == eSTREAMMSG_H2641I) && (pavi->aviinfo.un32vidscount >= pavi->aviinfo.un32vidsmax)) {
						pavi->state = AVIENC_CLOSE;
						muxer_avienc_close(pavi);
					} else if (sbuf_get(&pavi->sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags), &(pavi->datablock.type)) == DEF_OK) {
						pavi->datablock.state = 1;
						muxer_avienc_write(pavi);
					} else {
						break;
					}

					if (pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT) {
						break;
					} else if (gtUser.bSdAviSave == SDSAVE_USE_OFF) {
						pavi->state = AVIENC_EXIT;
						break;
					}
				}
			} else {
				pavi->state = AVIENC_EXIT;
			}
			break;
		case AVIENC_CLOSE:	// CLOSE 작업중이다.
			break;
		case AVIENC_EXIT:	// User Command(Shell or GPIO)
//			dprintf("AVIENC_EXIT call\r\n");
			muxer_avienc_close(pavi);
#ifdef __IDX_WRITE__
			f_unlink(pavi->idx_name);
#endif
			muxer_avienc_all_stop();
			printf("%s(%d) : AVIENC EXIT\r\n", __func__, __LINE__);
			break;
		default:
			printf("%s Error Call\r\n", __func__);
			break;
	}
}

void IsrH264Enc(void *arg)
{
	stream_info info;
	while (MsgRecGet(&info) == DEF_OK) {
		switch (info.type) {
			case eSTREAMMSG_H2641I:
				muxer_avienc_Buffering(info.addr, info.size, 1, info.type);
				break;
			case eSTREAMMSG_H2641P:
#if (FAT_SDSAVE_AUD==1)
			case eSTREAMMSG_AUDIO:
#endif
#ifdef __AVI_TXTS__
			case eSTREAMMSG_TXTS:
#endif
				muxer_avienc_Buffering(info.addr, info.size, 0, info.type);
				break;
#if (FAT_SDSAVE_AUD!=1)
			case eSTREAMMSG_AUDIO:
#endif
#ifndef __AVI_TXTS__
			case eSTREAMMSG_TXTS:
#endif
//			case eSTREAMMSG_H2642I:
//			case eSTREAMMSG_H2642P:
//			case eSTREAMMSG_JPEG:
				break;
			default:
				printf("unknown r-type(%d)\r\n", info.type);
				break;
		}
	}

	if(bufSem == NULL) // not ready!
		return;

	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(bufSem, &xHigherPriorityTaskWoken);
	if (xHigherPriorityTaskWoken) {
		gbXsrTaskSwitchNeeded = 1;	// Task switch required?
	}

	UNUSED(arg);
}

void muxer_avienc_init(void)
{
	ENTER();

	if (!Sdio0GetInstall()) {
		bSD_FAT_mounted = 1;

		// 일반 녹화
		avienc_info_default(&gAvienc[eRecNormal]);
		avienc_info_init_filenamelist(&gAvienc[eRecNormal], MAX_FILE_NUM+1);

		// 이벤트 녹화
		avienc_info_default(&gAvienc[eRecEvent]);
		avienc_info_init_filenamelist(&gAvienc[eRecEvent], MAX_FILE_NUM+1);

		muxer_avienc_change_savetime(NULL, 1);
	}

	LEAVE();
}

void muxer_avienc_delete(void)
{
	ENTER();

//	avienc_info_deinit(&gAvienc[eRecNormal]);
//	avienc_info_deinit(&gAvienc[eRecEvent]);

	LEAVE();
}

int muxer_avienc_sbuf_depth_sum(void)
{
	int a = 0, b = 0;
	a = (cQueue_isempty((&gAvienc[eRecNormal].sbuf)) ? DEF_FAIL : DEF_OK);
	b = (cQueue_isempty((&gAvienc[eRecEvent].sbuf)) ? DEF_FAIL : DEF_OK);
	return (a | b);
}

#if JPEG_SDSAVE
void muxer_jpegstill_process(void *ctx)
{
	FIL fp;
	char filename[128];
	UINT nByteWritten;

	while(gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {	
		WaitXus(10);
	}
	gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;			// JPEG snapshot start
	gptMsgShare.JPEG_STILL_REF++;

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	sprintf(filename, "%d:/%s/%04d%02d%02d_%02d%02d%02d.jpg", MMC, MMC_DIR_SNAP, tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec);				// File name gen
	if (f_open(&fp, filename, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
		printf("Error : f_open File system fail\r\n");
	} else {
		BYTE* src_addr = (BYTE *)gptMsgShare.JPEG_STILL_ADDR;
		if (f_write(&fp, src_addr, gptMsgShare.JPEG_STILL_SIZE, &nByteWritten) != FR_OK) {
			printf("Error : f_write File system fail\r\n");
		}
		f_close(&fp);
	}

	gptMsgShare.JPEG_STILL_REF--;
	if (gptMsgShare.JPEG_STILL_REF == 0) {
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;			// JPEG snapshot idle state
	} else if (gptMsgShare.JPEG_STILL_REF < 0) {
		printf("%s(%d) : Error : JPEG_STILL_REF(%d)\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
		gptMsgShare.JPEG_STILL_REF = 0;
	}

	UNUSED(ctx);
}
#endif


/**
	h.264 stream is buffered in ddr and written to sd card. 	
*/
void muxer_avienc(void)
{
	ENTER();

	if (bSD_FAT_mounted == 1) {
		if (muxer_avienc_state(eRecEvent)) { // 이벤트 녹화가 활성화 중이라면
			if (muxer_avienc_state(eRecNormal)) { // 일반 녹화는
				switch (gAvienc[eRecNormal].state) {
					case AVIENC_OPEN:
					case AVIENC_FIRST:
					case AVIENC_FIRST_NEXT:
					case AVIENC_WRITE:
						gAvienc[eRecNormal].state = AVIENC_CLOSE;
						muxer_avienc_close(&gAvienc[eRecNormal]);	// 종료하고
						break;
					default:
						break;
				}
				muxer_avienc_stop(eRecNormal);	// 일반 녹화 중지
			}
			muxer_avienc_process(&gAvienc[eRecEvent]); 	// 이벤트 녹화 진행
		} else if (muxer_avienc_state(eRecNormal)) { // 일반 녹화 활성화 중이라면
			muxer_avienc_process(&gAvienc[eRecNormal]); // 일반 녹화 진행
		}

#if JPEG_SDSAVE
		if (muxer_avienc_state(eRecSnapshot) == 1) {
			muxer_jpegstill_process(NULL);
			muxer_avienc_stop(eRecSnapshot);
		}
#endif
	}

	LEAVE();
}

/**
	register irq and init sbuf for buffering
	this function should be called first before h264 working in cpu1 
*/
void muxer_init_buffering(void)
{
	ENTER();

	avienc_info_init(&gAvienc[eRecNormal], eRecNormal, SAVE_NOR_DIR, SAVE_NOR_NAME_FORMAT);
	avienc_info_sbuf_init(&gAvienc[eRecNormal]);

	avienc_info_init(&gAvienc[eRecEvent], eRecEvent, SAVE_EVT_DIR, SAVE_EVT_NAME_FORMAT);
	avienc_info_sbuf_init(&gAvienc[eRecEvent]);

	bufSem = xSemaphoreCreateCounting(MSG_RECORD_NUM, 0);

	LEAVE();
}

void muxer_avienc_task(void* pvParameters)
{
	nAvienc_Mode_now = 0;
	nAvienc_Mode_old = -1;

	while (1) {
		if (xSemaphoreTake(bufSem, portMAX_DELAY) == pdTRUE) {
			if (muxer_avienc_sbuf_depth_sum()) {
				muxer_avienc();
			}
		}
	}

	UNUSED(pvParameters);
}
#endif
