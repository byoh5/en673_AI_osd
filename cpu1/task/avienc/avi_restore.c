/* ------------------------ OR32 includes -------------------------------- */
#include <stdio.h>															

/* ------------------------ EN673 includes ------------------------------- */
#include "dev.h"
#include "factory.h"
#include "develop.h"
#include "user.h"

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ Project includes ------------------------------ */
#include "avi_restore.h"
#include "fmt_avienc.h"

/* ------------------------ Util includes --------------------------------- */
#include "ff.h"
#include "file.h"

#ifdef __DEVICE_SD__

//#define ENX_AVIRE_DEBUG			// printf 주석용

#ifdef ENX_AVIRE_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

#define AVI_ALIGN_SIZE   	2
#define AVI_ALIGN(x) 		(((x) + AVI_ALIGN_SIZE - 1) & ~(AVI_ALIGN_SIZE - 1))

typedef struct {
	char strPath[40];
	char strAviFile[_MAX_LFN+1];
	char strIdxFile[_MAX_LFN+1];

	DWORD offset_max;
	DWORD check_size;
	t_avi_t avi_info;
}AVI_RESTORE_t;

static UINT idxfile_search(AVI_RESTORE_t *arData)
{
	FRESULT fres;
	DIR dir;
	TCHAR strFileName[_MAX_LFN+1] = {0};
	FILINFO finfo;
	UINT cnt = 0;

	fres = f_opendir(&dir, arData->strPath);
	finfo.lfname = strFileName;
	finfo.lfsize = _MAX_LFN;

	for(;;)
	{
    	fres = f_readdir(&dir, &finfo);
    	if((fres != FR_OK) || !finfo.fname[0])
		{
			fres = FR_OK;
			break;
		}

		if(dir.lfn_idx == 0xFFFF)
		{
			if(strstr(finfo.fname, ".idx") != NULL)
			{
				cnt++;
				sprintf(arData->strIdxFile, "%s/%s", arData->strPath, finfo.fname);
				tprintf("search %s\r\n", arData->strIdxFile);
			}
		}
		else
		{
			if(strstr(finfo.lfname, ".idx") != NULL)
			{
				cnt++;
				sprintf(arData->strIdxFile, "%s/%s", arData->strPath, finfo.lfname);
				tprintf("search %s\r\n", arData->strIdxFile);
			}
		}
    }
	return cnt;
}

static UINT avifile_search(AVI_RESTORE_t *arData)
{
	UINT cnt = 0;
	FRESULT fres;
	FILINFO finfo;

	// .idx 잘라내기
	tprintf("[.idx] cut : [%s]\r\n", arData->strIdxFile);
	cnt = strcspn(arData->strIdxFile, ".idx");
	if(cnt == strlen(arData->strIdxFile))
	{
		// filename이 idx로 종료되지 않음
		tprintf("[.idx] not search : [%s]\r\n", arData->strIdxFile);
		return DEF_FAIL;
	}
	strncpy(arData->strAviFile, arData->strIdxFile, cnt);
	arData->strAviFile[cnt] = '\0';
	strcat(arData->strAviFile, ".avi");

	tprintf("avi path : [%s]\r\n", arData->strAviFile);
	tprintf("idx path : [%s]\r\n", arData->strIdxFile);

	// file 유무 확인
	fres = f_stat(arData->strAviFile, &finfo);
	if(fres == FR_OK && finfo.fsize > 0)
	{
		tprintf("avi file(%s/%ubyte) check ok\r\n", arData->strAviFile, finfo.fsize);
		return DEF_OK;
	}
	else
	{
		tprintf("avi file(%s) check fail\r\n", arData->strAviFile);
		fres = f_unlink(arData->strIdxFile);
		tprintf("idx file(%s) unlink(%s)\r\n", arData->strIdxFile, put_rc(fres));
		return DEF_FAIL;
	}
}

static UINT avifile_check(AVI_RESTORE_t *arData)
{
	BYTE header_info[AVI_HEADSIZE] = {0};
	FIL fp;
	FRESULT fres;
	UINT readsize;

	fres = f_open(&fp, arData->strAviFile, FA_READ);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strAviFile, put_rc(fres));
		return DEF_FAIL;
	}

	tprintf("header read start\r\n");
	fres = f_read(&fp, (void *)header_info, AVI_HEADSIZE, &readsize);
	if(fres != FR_OK)
	{
		tprintf("read fail(%s)\r\n", put_rc(fres));
		f_close(&fp);
		return DEF_FAIL;
	}
	if(readsize == 0)
	{
		tprintf("read fail\r\n");
		f_close(&fp);
		return DEF_FAIL;
	}
	tprintf("header read end\r\n");

	avi_videostream_t *avi_video;

	avi_video = (avi_videostream_t *)&header_info[sizeof(AVI_HEADER_t)];

	arData->avi_info.i_riff = fp.fsize;
	arData->avi_info.f_fps = NUM32(avi_video->stream.shData.dwRate);
	arData->avi_info.i_width = NUM16(avi_video->stream.shData.rcFrame.right);
	arData->avi_info.i_height = NUM16(avi_video->stream.shData.rcFrame.bottom);

	tprintf("avi info\r\n");
	tprintf("f_fps : %u\r\n", arData->avi_info.f_fps);
	tprintf("i_width : %u\r\n", arData->avi_info.i_width);
	tprintf("i_height : %u\r\n", arData->avi_info.i_height);
	tprintf("file size : %u\r\n", arData->avi_info.i_riff);

	tprintf("file close\r\n");
	f_close(&fp);

	return DEF_OK;
}


static UINT idxfile_check(AVI_RESTORE_t *arData)
{
	FIL fp;
	FRESULT fres;
	AVIINDEX_t aviindex;
	UINT readsize, offset, fptr;
	UINT old_offset_max, old_checksize, old_fptr, lastdata;

	fres = f_open(&fp, arData->strIdxFile, FA_READ);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strIdxFile, put_rc(fres));
		return DEF_FAIL;
	}

	DWORD chunk_vids = FCC('0','0','d','c');
	DWORD chunk_auds = FCC('0','1','w','b');
	DWORD chunk_txts = FCC('0','2','t','x');

	arData->avi_info.option = 0;
	arData->avi_info.un32vidscount = 0;
	arData->avi_info.un32audscount = 0;
	arData->avi_info.un32txtscount = 0;
	arData->offset_max = 0;

	tprintf("idx read start\r\n");
	while(1)
	{
		fres = f_read(&fp, (void *)&aviindex, sizeof(AVIINDEX_t), &readsize);
		if(fres != FR_OK)
		{
			tprintf("idx read fail(%s)\r\n", put_rc(fres));
			f_close(&fp);
			return DEF_FAIL;
		}
		if(readsize == 0)
		{
			tprintf("idx read end\r\n");
			f_close(&fp);
			break;
		}

		offset = NUM32(aviindex.dwOffset);
		if(offset + AVI_HEADSIZE > arData->avi_info.i_riff)
		{
			if(lastdata == 1)		arData->avi_info.un32vidscount--;
			else if(lastdata == 2)	arData->avi_info.un32audscount--;
			else if(lastdata == 3)	arData->avi_info.un32txtscount--;

			arData->offset_max = old_offset_max;
			arData->check_size = old_checksize;
			fptr = old_fptr;

//			tprintf("idx fptr %X\r\n", fptr);
//			tprintf("offset %u(%u+AVI_HEADSIZE), avi size %u\r\n", offset + AVI_HEADSIZE, offset, arData->avi_info.i_riff);
			break;
		}

		if(arData->offset_max < offset)
		{
			old_offset_max = arData->offset_max;
			old_checksize = arData->check_size;
			old_fptr = fptr;

			fptr = f_tell(&fp);
			arData->offset_max = offset;
			arData->check_size = NUM32(aviindex.dwSize);
		}

		if(aviindex.dwChunkId == chunk_vids)
		{
			arData->avi_info.un32vidscount++;
			lastdata = 1;
		}
		else if(aviindex.dwChunkId == chunk_auds)
		{
			arData->avi_info.un32audscount++;
			lastdata = 2;
		}
		else if(aviindex.dwChunkId == chunk_txts)
		{
			arData->avi_info.un32txtscount++;
			lastdata = 3;
		}

	}

	fres = f_open(&fp, arData->strIdxFile, FA_WRITE);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strIdxFile, put_rc(fres));
		return DEF_FAIL;
	}

	f_lseek(&fp, fptr);
	fres = f_truncate(&fp);
	tprintf("f_truncate(%s)\r\n", put_rc(fres));

	f_close(&fp);

	if(arData->avi_info.un32vidscount)
		arData->avi_info.option |= AVI_VIDEO;
	if(arData->avi_info.un32audscount)
		arData->avi_info.option |= AVI_AUDIO;
	if(arData->avi_info.un32txtscount)
		arData->avi_info.option |= AVI_TXTS;

	tprintf("index info\r\n");
	tprintf("option : %u\r\n", arData->avi_info.option);
	tprintf("vids cnt : %u\r\n", arData->avi_info.un32vidscount);
	tprintf("auds cnt : %u\r\n", arData->avi_info.un32audscount);
	tprintf("txts cnt : %u\r\n", arData->avi_info.un32txtscount);
	tprintf("offset max : %u(size:%u)\r\n", arData->offset_max, arData->check_size);

	return DEF_OK;
}

static UINT avifile_truncate(AVI_RESTORE_t *arData)
{
	FIL fp;
	FRESULT fres;
	UINT readsize, checksize;
	UINT fptr;

	fres = f_open(&fp, arData->strAviFile, FA_READ);
	if(fres != FR_OK)
	{
		tprintf("%s open fail\r\n", arData->strAviFile);
		return DEF_FAIL;
	}

	f_lseek(&fp, arData->offset_max + AVI_HEADSIZE);
	f_read(&fp, (void *)&checksize, 4, &readsize);
	checksize = NUM32(checksize);

	if(arData->check_size != checksize)
	{
		tprintf("check size error idx(%d) avi(%d)\r\n", arData->check_size, checksize);
		f_close(&fp);
		return DEF_FAIL;
	}
	
	tprintf("check offset size : %u\r\n", checksize);
	checksize = AVI_ALIGN(checksize);

	f_lseek(&fp, arData->offset_max + AVI_HEADSIZE + checksize + 4);
	fptr = f_tell(&fp);
	f_close(&fp);
	
	fres = f_open(&fp, arData->strAviFile, FA_WRITE);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strAviFile, put_rc(fres));
		return DEF_FAIL;
	}

	f_lseek(&fp, fptr);
	fres = f_truncate(&fp);
	tprintf("f_truncate(%s)\r\n", put_rc(fres));

	f_close(&fp);

	return DEF_OK;
}

static UINT avifile_merge(AVI_RESTORE_t *arData)
{
	FIL fpavi, fpidx;
	FRESULT fres;
	UINT readsize, writesize;
	UINT idxsize, idxsize_check = 0;
	BYTE *avihead;

	fres = f_open(&fpavi, arData->strAviFile, FA_WRITE);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strAviFile, put_rc(fres));
		return DEF_FAIL;
	}
	fres = f_open(&fpidx, arData->strIdxFile, FA_READ);
	if(fres != FR_OK)
	{
		tprintf("%s open fail(%s)\r\n", arData->strIdxFile, put_rc(fres));
		return DEF_FAIL;
	}

	f_lseek(&fpavi, fpavi.fsize);	// eof move
	arData->avi_info.i_movi_end = fpavi.fsize - AVI_HEADSIZE + 4;

	// index head
	idxsize = sizeof(AVIINDEX_t) * (arData->avi_info.un32vidscount + arData->avi_info.un32audscount + arData->avi_info.un32txtscount);
	FCC_CB_t fcccb = avi_write_chunkheader(IDX1_ARRAY, idxsize);
	f_write(&fpavi, (void *)&fcccb, sizeof(FCC_CB_t), &writesize);		// 8byte

	BYTE *buf_base = pvPortMalloc(SDSAVE_BUF_SIZE);
	if(buf_base)
	{
		// index copy
		while(1)
		{
			f_read(&fpidx, (uint8 *)buf_base, SDSAVE_BUF_SIZE, &readsize);
			if(readsize == 0)
			{
				f_close(&fpidx);
				break;
			}
			f_write(&fpavi, (uint8 *)buf_base, readsize, &writesize);
			idxsize_check += writesize;
		}
		vPortFree(buf_base);

		tprintf("idxsize(%d), check(%d)\r\n", idxsize, idxsize_check);

		arData->avi_info.i_riff = f_tell(&fpavi);
		avihead = avi_write_header(&(arData->avi_info));
		fres = f_lseek(&fpavi, 0);
		fres = f_write(&fpavi, (void *)avihead, AVI_HEADSIZE, &writesize);		// 4096byte
		tprintf("%s header write(%s)\r\n", arData->strAviFile, put_rc(fres));
		fres = f_close(&fpavi);

		fres = f_unlink(arData->strIdxFile);
		tprintf("%s unlink(%s)\r\n", arData->strIdxFile, put_rc(fres));

		tprintf("%s avi restore processing end\r\n", arData->strAviFile);

		return DEF_OK;
	}
	else
	{
		tprintf("%s avi restore processing fail end\r\n", arData->strAviFile);
	}
	return DEF_FAIL;
}

static UINT avifile_process(char *strPath)
{
	AVI_RESTORE_t arData;
	UINT cnt = 0, err;

	memset((void *)&arData, 0, sizeof(AVI_RESTORE_t));
	
	sprintf(arData.strPath, "%d:/%s", MMC, strPath);
	
	cnt = idxfile_search(&arData);	// 목표가 될 idx파일을 찾는다.
	if(cnt == 0)
	{
		tprintf("%s not avi damaged\r\n", arData.strPath);
	}
	else
	{
		tprintf("%s avi damaged / restore start\r\n", arData.strIdxFile);
		if(avifile_search(&arData) == DEF_OK)	// 해당 idx에 맞는 avi파일을 찾는다. idx만 존재하고 avi파일이 없는 경우 idx 삭제
		{
			err = 0;
			err += avifile_check(&arData); 	// avi파일의 head정보를 읽어 기본정보 확인(fps, width, height)
			err += idxfile_check(&arData); 	// idx파일의 정보를 읽어 data type와 각 frame count, 유효한 최종 offset을 찾고 불필요한 idx를 제거한다.
			err += avifile_truncate(&arData);	// avi파일의 idx파일의 idx data에 근거하여 불필요한 데이터를 제거한다.
			err += avifile_merge(&arData); 	// avi파일와 idx파일을 합치고 head를 갱신하며 idx파일을 제거한다.

			if(err == 4)
			{
				tprintf("%s avi restore success\r\n", arData.strAviFile);
			}
			else
			{
				tprintf("%s avi restore fail\r\n", arData.strAviFile);
			}
		}
		else
		{
			tprintf("%s avi restore fail\r\n", arData.strAviFile);
		}
	}

	return cnt;
}

void avi_restore(void)
{
	tprintf("=====================================\r\n");
	tprintf("avi restore start\r\n");

	UINT cnt = 0;

	do{
		cnt = avifile_process(MMC_DIR_NOR);
	}while(cnt);

	do{
		cnt = avifile_process(MMC_DIR_EVT);
	}while(cnt);

	tprintf("avi restore end\r\n");
	tprintf("=====================================\r\n");
}
#endif
