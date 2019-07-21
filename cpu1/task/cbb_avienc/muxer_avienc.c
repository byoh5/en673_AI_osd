#include "dev.h"
#include "board.h"
#include "dev_types.h"
//#include "sys.h"
//#include "cache.h"
//#include "sd.h"
#include "msg.h"
//#include "int.h"

#include "user.h"
#include "file.h"
//#include "gpio.h"

#include "muxer_avienc.h"
#include "avienc_info.h"
#include "avienc_heap.h"
#include "avio_fat32.h"
#include "fmt_avienc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sys_arch.h"

#ifdef __CBB__
//#include "osd_za20s10.h"
#include "cbb_power.h"
#include "cbb_ctlmain.h"
#include "gpsrx.h"
#include "gsensor.h"
#endif

//#include "vsprintf.h"
#include "sdcard_check.h"

#ifdef  CONTIGUOUS_WRITE
extern FATFS gtFatFs[_VOLUMES];	// File system object
#endif

#define AUXCLUST
//#undef __JPEG__

#if 0
#define ENTER() dprintf("%s IN\n", __func__);
#define LEAVE() dprintf("%s OUT\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#ifdef __DEVICE_SD__

#ifdef ENX_AVISV_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

int g_normalgo = 0;
//int g_normalgo2 = 0;
int g_eventgo = 0;
int g_manualgo = 0;
//int max_size=MAX_AVI_FILE_SIZE;
#ifdef __CBB__
extern int is_update;
extern int bSD_State;
#endif

#ifdef ENX_PLAYBACK_DEBUG
	#define pbprintf(fmt, args...) printf(fmt, ##args);
#else
	#define pbprintf(fmt, args...) 
#endif

#if 1
static int Start_clustAVI = 0;//ksjdebug
static int Last_clustJPG = 0;//ksjdebug
#endif


//#define PLAYBACK_BUFFSIZE 200*1024
unsigned char *data_file = NULL;

/*const int nSAVEcnt[][3] = {
         {30,       15,       15},  //55/2 = 25 //  8GB
         {58,       40,       40},   //110/2 = 55 // 16GB
         {130,      90,       90} //free 28.9GB  {130,      60,       60}          
};*/
typedef struct {
	UINT normalmaxcnt;
	UINT eventmaxcnt;
	UINT manualmaxcnt;
}filecnt;
filecnt nSAVEcnt;


const int nFPSvalue[][12] = {
	{60, 30, 20, 15, 12, 10, 6, 5, 4, 3, 2, 1}, 
	{50, 25, 10, 5, 2, 1},
	{30, 15, 10, 6, 5, 3, 2, 1},
	{25, 5, 1},
	{15, 5, 3, 1}
};

const int nSIZEvalue[][2] = {
	{1920, 1080},
	{1280, 720},
	{800, 600},
	{640, 480},
	{320, 240},
};

#define MKTAG(a,b,c,d) (a | (b << 8) | (c << 16) | (d << 24))

extern sys_thread_t stt_avienc;
extern sys_thread_t stt_avidec;
#ifdef __CBB__
extern int is_error;
extern int ck_sdsave;
#endif

xSemaphoreHandle gssSem = NULL;
xSemaphoreHandle bufSem = NULL;
UINT bSD_FAT_mounted = 0;
UINT nSD_CARD_SIZE = 0;
//UINT nSD_CARD_FLAG = 0;
//UINT start_sclust = AVI_START_CLUST;
//UINT muxer_sclust;

typedef enum {
	AVI_OK,
	AVI_FULL,
	AVI_OPENFAIL,
	AVI_WRITEFAIL,
	AVI_STATEFAIL,
} AVI_ERROR;

t_avienc_t enc_nor;	// 주행 상시
t_avienc_t enc_evt;	// 주행 이벤트
t_avienc_t enc_mar;	// 주행 메뉴얼
t_avienc_t enc_play;
t_avienc_t *enc_ing;

#if 0 //for audio decode encode test
int8 MuLaw_Encode(int16 number)
{
   const uint16 MULAW_MAX = 0x1FFF;
   const uint16 MULAW_BIAS = 15;
   uint16 mask = 0x1000;
   uint8 sign = 0;
   uint8 position = 12;
   uint8 lsb = 0;
   if (number < 0)
   {
      number = -number;
      sign = 0x80;
   }
   number += MULAW_BIAS;
   if (number > MULAW_MAX)
   {
      number = MULAW_MAX;
   }
   for (; ((number & mask) != mask && position >= 5); mask >>= 1, position--)
        ;
   lsb = (number >> (position - 4)) & 0x0f;
   return (~(sign | ((position - 5) << 4) | lsb));
}

int16 MuLaw_Decode(int8 number)
{
   const uint16 MULAW_BIAS = 15;
   uint8 sign = 0, position = 0;
   int16 decoded = 0;
   number = ~number;
   if (number & 0x80)
   {
      number &= ~(1 << 7);
      sign = -1;
   }
   position = ((number & 0xF0) >> 4) + 5;
   decoded = ((1 << position) | ((number & 0x0F) << (position - 4))
             | (1 << (position - 5))) - MULAW_BIAS;
   return (sign == 0) ? (decoded) : (-(decoded));
}
#endif

int16 tbaudio[1460]={0};

/**
    @return 
        1 : mounted.
        0 : nothing.
*/
int muxer_avienc_sdCardMounted(void)
{
    return bSD_FAT_mounted;
}

#ifdef __CBB__
static eCBB_Mode oldcbmode, newcbmode;
#endif

void muxer_avienc_h264Buffering(u32 h264_ptr, u32 h264_size, u32 idr, int total)
{
	if(h264_size)
	{		
		
#ifdef __CBB__
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_sbuf_tot(&enc_nor, total);
				avienc_info_putVideo(&enc_nor, h264_ptr, h264_size, idr);
				avienc_info_sbuf_tot(&enc_evt, total);
				avienc_info_putVideo(&enc_evt, h264_ptr, h264_size, idr);
				avienc_info_sbuf_tot(&enc_mar, total);	
				avienc_info_putVideo(&enc_mar, h264_ptr, h264_size, idr);
				break;

			default:
				break;
		}
#else
		avienc_info_sbuf_tot(&enc_nor, total);
		avienc_info_putVideo(&enc_nor, h264_ptr, h264_size, idr);
		avienc_info_sbuf_tot(&enc_evt, total);
		avienc_info_putVideo(&enc_evt, h264_ptr, h264_size, idr);
		avienc_info_sbuf_tot(&enc_mar, total);	
		avienc_info_putVideo(&enc_mar, h264_ptr, h264_size, idr);
#endif
	}
}

#ifdef __AUDIO__
void muxer_avienc_g711Buffering(u32 g711_ptr, u32 g711_size, u32 flags, int total)
{
	if(g711_size)
	{
#ifdef __CBB__
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_sbuf_tot(&enc_nor, total);
				avienc_info_putAudio(&enc_nor, g711_ptr, g711_size, flags);
				avienc_info_sbuf_tot(&enc_evt, total);
				avienc_info_putAudio(&enc_evt, g711_ptr, g711_size, flags);
				avienc_info_sbuf_tot(&enc_mar, total);
				avienc_info_putAudio(&enc_mar, g711_ptr, g711_size, flags);
				break;
			default:
				break;
		}
#else
		avienc_info_putAudio(&enc_nor, g711_ptr, g711_size, flags);
		avienc_info_putAudio(&enc_evt, g711_ptr, g711_size, flags);
		avienc_info_putAudio(&enc_mar, g711_ptr, g711_size, flags);
#endif
	}
}
#endif

#ifdef __AVI_TXTS__
void muxer_avienc_txt1Buffering(u32 txts_ptr, u32 txts_size, u32 flags)
{
	if(txts_size)
	{
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_putTxt1(&enc_nor, txts_ptr, txts_size, flags);
				avienc_info_putTxt1(&enc_evt, txts_ptr, txts_size, flags);
				avienc_info_putTxt1(&enc_mar, txts_ptr, txts_size, flags);
				break;
			default:
				break;
		}
	}
}
#if 0
void muxer_avienc_txt2Buffering(u32 txts_ptr, u32 txts_size, u32 flags)
{
	if(txts_size)
	{
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_putTxt2(&enc_nor, txts_ptr, txts_size, flags);
				avienc_info_putTxt2(&enc_evt, txts_ptr, txts_size, flags);
				avienc_info_putTxt2(&enc_mar, txts_ptr, txts_size, flags);
				break;
			default:
				break;
		}
	}
}

void muxer_avienc_txt3Buffering(u32 txts_ptr, u32 txts_size, u32 flags)
{
	if(txts_size)
	{
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_putTxt3(&enc_nor, txts_ptr, txts_size, flags);
				avienc_info_putTxt3(&enc_evt, txts_ptr, txts_size, flags);
				avienc_info_putTxt3(&enc_mar, txts_ptr, txts_size, flags);
				break;
			default:
				break;
		}
	}
}
void muxer_avienc_txt4Buffering(u32 txts_ptr, u32 txts_size, u32 flags)
{
	if(txts_size)
	{
		switch(newcbmode)//cbbpower_get_mode())
		{
			case eCBB_Drive:
				avienc_info_putTxt4(&enc_nor, txts_ptr, txts_size, flags);
				avienc_info_putTxt4(&enc_evt, txts_ptr, txts_size, flags);
				avienc_info_putTxt4(&enc_mar, txts_ptr, txts_size, flags);
				break;
			default:
				break;
		}
	}
}
#endif
#endif

void muxer_avienc_error(t_avienc_t *pavi, AVI_ERROR state)
{
	ENTER();
#ifdef __CBB__
	bitchang(is_error, CBB_ERROR_SDWRITE, 1);
#endif
	//g711_mb.wBuzzCnt=5;
	
	//pavi->fpavi.flag |= FA__WRITTEN;
	f_close(&pavi->fpavi);

	if(state == AVI_FULL && state ==  AVI_WRITEFAIL)
	{
		pavi->state = AVIENC_IDLE;
	} 
	else
	{
#if 0 //#ifdef __CBB__
		if(cbbpower_get_mode() != eCBB_Off)
		{	// 다른 음성으로 변경해야 한다. / 리부팅?
			G726Playlist_put(eG726_REBOOT_SYSERROR);
			vTaskDelay(300);

			printf("%s(%d) : WDT GO\n", __func__, __LINE__);
			WdtSystemReset();

			while(1);
		}
#endif
		pavi->state = AVIENC_EXIT;
		gtUser.bSdAviSave = SDSAVE_USE_OFF;
		muxer_avienc_delete();
		bSD_FAT_mounted = 0;
		stt_avienc = NULL;
		stt_avidec = NULL;
		LEAVE();
		vTaskDelete(NULL);
	}

	LEAVE();
}

/**
@return 0  : ok
-1 : error 
*/static int
avienc_flush_buffer(t_avienc_t *pavi)
{
	int fres;
	UINT writesize;
	UINT status;
	// printf("sd busy %x \n",status);
	UINT wtime_s = xTaskGetTickCount();

#ifdef CONTIGUOUS_WRITE
	fres = avio_low_write(pavi,&writesize);
	if(fres == AVIO_OVER_CLUSTER)
		dprintf("%s, shortage reseved area in SD, sect %d, end %d \n",__func__,pavi->sect,pavi->end_sect);
	//f_sync(&(pavi->fpavi));
#else  
	fres = f_write(&(pavi->fpavi), (uint8 *)pavi->buffer.base, pavi->buffer.length, &writesize);

	if(pavi->buffer.length != writesize)
	{
		dprintf("%s Error : f_write All Write fail.(buff:%d)(write:%d)\n", __func__, pavi->buffer.length, writesize);

		if(fat_get_freesize(MMC) == 0)
		{
			dprintf("%s : SD freesize zero\n", __func__);
			muxer_avienc_error(pavi, AVI_FULL);
			goto done;
		}
	}
	if(writesize == 0)
	{
		dprintf("!!!!!!!!!!!!!!!write size 0!!!!!!!!!!!\n");
	}
#endif

	//UINT wtime_m = xTaskGetTickCount();
	//f_sync(&(pavi->fpavi));
	UINT wtime_e = xTaskGetTickCount();
	//tprintf("File Write(%.2f(%.2f+%.2f), %dbyte)\n", (wtime_e - wtime_s)/100.0, (wtime_m - wtime_s)/100.0, (wtime_e - wtime_m)/100.0, pavi->buffer.length);
	UINT wtime = (wtime_e - wtime_s)*10;

	if(wtime > 1000)
	{  
		dprintf("f_write(%04dms) : %dbyte\n", wtime, pavi->buffer.length);
		//vTaskDelay(1);//vTaskDelay(3);
	}

	if(fres != FR_OK)
	{
		dprintf("%s Error : f_write File system fail\n", __func__);
		dprintf("Err Msg : %s\n", put_rc(fres));
		muxer_avienc_error(pavi, AVI_WRITEFAIL);
		goto done;
	}
	
	printf("writesize : %d ", writesize);

	if(writesize == 0)
	{
		if(fat_get_freesize(MMC) == 0)
		{
			dprintf("%s : SD freesize zero\n", __func__);
			muxer_avienc_error(pavi, AVI_FULL);
			goto done;
		}

		dprintf("writesize == 0");goto done;
	}

	pavi->buffer.buf = pavi->buffer.base; // buffer의 처음으로 이동
	pavi->buffer.length -= writesize;   // buffer에 남은 데이터, 값이 0이여야 한다.
#ifndef CONTIGUOUS_WRITE
	pavi->filesize += writesize;
#endif

	//dprintf("%s, filesize %d \n",__func__,pavi->filesize);
	if(pavi->buffer.length != 0)
	{
		dprintf("%s Error : AVI buffer no clear(%d)\n", __func__, pavi->buffer.length);
		pavi->buffer.length = 0;
	}
	return 0;

done: 
	return -1;      
}

// size : -1 : buffer write & buffer no add
// size : >0 : check buffer & buffer add
#if 0
void muxer_avienc_bufferadd(t_avienc_t *pavi, uint8 *data, uint32 size)
{
	FRESULT fres;
	UINT writesize = 0, left, wb=0;
	if(data == NULL)
	{
		if(size == 0)
		{
			avienc_flush_buffer(pavi);
			//dprintf("bufferadd err !!!!!!!!!!!!!!\n");
			return;
		}
		else
		{
			dprintf("bufferadd err !!!! sz %d \n", size);
		}
	}

	while(size)
	{
		left = pavi->buffer.total - pavi->buffer.length;

		if(size >= left )	wb = left;
		else				wb = size;

		if(wb > 4)
		{
			//DmaMemCpy((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
			//invalidate_dcache_range(pavi->buffer.buf, pavi->buffer.buf + wb);
			DmaMemCpy((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
			invalidate_dcache_range((UINT)pavi->buffer.buf, ((UINT)pavi->buffer.buf) + wb);
			
		}
		else
		{
			dprintf("memcpy %d ",wb);
		 	memcpy((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
		}
		
		pavi->buffer.length += wb;
		pavi->buffer.buf	+= wb;
		pavi->filesize		+= wb;
		
		if(pavi->buffer.length == pavi->buffer.total)
		{		
			avienc_flush_buffer(pavi);
			//vTaskDelay(1);
		}
		
		data += wb;
		size -= wb;
/*		
		if(gtUser.bSdAviSave == SDSAVE_USE_OFF)
		{UartPutByte('1>');
			pavi->state = AVIENC_EXIT;
			break;
		}
*/
	}
	//printf("3");

done:
	return;
}
#else
#if 1
void muxer_avienc_bufferadd(t_avienc_t *pavi, uint8 *data, int32 size)
{
   FRESULT fres;
   UINT writesize = 0, left, wb;
   if(!data)
   {
     avienc_flush_buffer(pavi);
     return;
   }

	size = AVI_ALIGN(size);

   while(size)
   {
     left = pavi->buffer.total - pavi->buffer.length;

     if(size >= left)    wb = left;
     else                                      wb = size;

#if 0
     DmaMemCpy_ip((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
     invalidate_dcache_range(pavi->buffer.buf, pavi->buffer.buf + wb);
#else
#if 1
	DmaMemCpy_ip((BYTE *)pavi->buffer.buf, (BYTE *)data, wb);
#else
     int i;
     for(i=0;i<wb;i++)
     {
        pavi->buffer.buf[i] = data[i];
     }
#endif
#endif
     pavi->buffer.length += wb;
     pavi->buffer.buf    += wb;
     pavi->filesize      += wb;

     if(pavi->buffer.length == pavi->buffer.total)
     {
        if(pavi->buffer.buf != pavi->buffer.end)
        {
           printf("Buffer(%d) End(%d)\r\n", pavi->buffer.buf, pavi->buffer.end);
        }
        avienc_flush_buffer(pavi);
     }

     data += wb;
     size -= wb;
   }

done:
   return;
}

#else
void muxer_avienc_bufferadd(t_avienc_t *pavi, uint8 *data, int32 size)
{
	// size over?
	FRESULT fres;
	UINT writesize = 0;
	if((pavi->buffer.buf + size + AVI_ALIGN_SIZE > pavi->buffer.end) || size == -1)
	{								// Buffer Write

		if(pavi->buffer.length <= 0)
		{	// no data
			pavi->buffer.length = 0;
			goto done;
		}
#ifdef CONTIGUOUS_WRITE
			fres = avio_low_write(pavi,&writesize);
			if(fres == AVIO_OVER_CLUSTER)
				dprintf("%s, shortage reseved area in SD, sect %d, end %d \n",__func__,pavi->sect,pavi->end_sect);
			//f_sync(&(pavi->fpavi));
#else

		//UINT wtime_s = xTaskGetTickCount();
		fres = f_write(&(pavi->fpavi), (uint8 *)pavi->buffer.base, pavi->buffer.length, &writesize);
		//UINT wtime_e = xTaskGetTickCount();
		tprintf("File Write(%dms, %dbyte)\r\n", (wtime_e - wtime_s)*10, pavi->buffer.length);
		if(fres != FR_OK)
		{
			printf("%s Error : f_write File system fail\r\n", __func__);
			printf("Err Msg : %s\r\n", put_rc(fres));
			muxer_avienc_error(pavi, AVI_WRITEFAIL);
			goto done;
		}
#endif
		if(pavi->buffer.length != writesize)
		{
			printf("%s Error : f_write All Write fail.(buff:%d)(write:%d)\r\n", __func__, pavi->buffer.length, writesize);
			if(fat_get_freesize(MMC) == 0)
			{
				printf("%s : SD freesize zero\r\n", __func__);
				muxer_avienc_error(pavi, AVI_FULL);
				goto done;
			}
		}

		if(writesize == 0)
		{
			if(fat_get_freesize(MMC) == 0)
			{
				printf("%s : SD freesize zero\r\n", __func__);
				muxer_avienc_error(pavi, AVI_FULL);
				goto done;
			}
		}

		pavi->buffer.buf = pavi->buffer.base;	// buffer의 처음으로 이동
		pavi->buffer.length -= writesize;		// buffer에 남은 데이터, 값이 0이여야 한다.

		pavi->filesize += writesize;

		if(pavi->buffer.length != 0)
		{
			printf("%s Error : AVI buffer no clear(%d)\r\n", __func__, pavi->buffer.length);
			pavi->buffer.length = 0;
		}

		if(size == -1)
			goto done;
	}

	if(size != -1)
	{
		if(size >= (int32)(pavi->buffer.total))	// Buffer Over Flow Processing
		{
			size = AVI_ALIGN(size);
			writesize = 0;

#ifdef CONTIGUOUS_WRITE
				fres = avio_low_write(pavi,&writesize);
				if(fres == AVIO_OVER_CLUSTER)
					dprintf("%s, shortage reseved area in SD, sect %d, end %d \n",__func__,pavi->sect,pavi->end_sect);
				//f_sync(&(pavi->fpavi));
#else
			//UINT wtime_s = xTaskGetTickCount();
			fres = f_write(&(pavi->fpavi), (uint8 *)data, size, &writesize);
			//UINT wtime_e = xTaskGetTickCount();
			tprintf("File Write(%dms, %dbyte)\r\n", (wtime_e - wtime_s)*10, size);
			if(fres != FR_OK)
			{
				printf("%s Error : f_write File system fail\r\n", __func__);
				printf("Err Msg : %s\r\n", put_rc(fres));
				muxer_avienc_error(pavi, AVI_WRITEFAIL);
				goto done;
			}
#endif
			if(size != (int32)writesize)
			{
				printf("%s Error : f_write All Write fail.(size:%d)(write:%d)\r\n", __func__, size, writesize);
				if(fat_get_freesize(MMC) == 0)
				{
					printf("%s : SD freesize zero\r\n", __func__);
					muxer_avienc_error(pavi, AVI_FULL);
					goto done;
				}
			}

			pavi->filesize += writesize;
		}
		else							// Buffer Add
		{
			DmaMemCpy_ip((BYTE *)pavi->buffer.buf, (BYTE *)data, size);
			invalidate_dcache_range((uint)pavi->buffer.buf, ((uint)pavi->buffer.buf) + size);
			
			pavi->buffer.buf += AVI_ALIGN(size);	// buf_loc = buf_loc + (size + ALIGN);
			pavi->buffer.length += AVI_ALIGN(size);
		}
	}

done:
	return;
}
#endif
#endif
BYTE *muxer_avienc_makeaviheader(t_avienc_t *pavi)
{
	int fps = 2;
	switch(pavi->state)
	{
		case AVIENC_OPEN:
			switch(gtUser.nMaxFPS)
			{
				case 60:	fps = 0;	break;
				case 50:	fps = 1;	break;
				case 30:	fps = 2;	break;
				case 25:	fps = 3;	break;
				case 15:	fps = 4;	break;
			}
			pavi->aviinfo.f_fps = nFPSvalue[fps][gtUser.uviH264.sfFPS];
			pavi->aviinfo.i_width =	nSIZEvalue[gtUser.uviH264.eResolution][0];
			pavi->aviinfo.i_height = nSIZEvalue[gtUser.uviH264.eResolution][1];
			pavi->aviinfo.i_riff = 0;
			pavi->aviinfo.i_movi_end = 0;
			pavi->aviinfo.i_header_end = AVI_HEADSIZE - 4;
			pavi->aviinfo.un32vidscount = 0;
			pavi->aviinfo.un32audscount = 0;
			pavi->aviinfo.un32txt1count = 0;
			pavi->aviinfo.un32txt2count = 0;
			pavi->aviinfo.un32txt3count = 0;
			pavi->aviinfo.un32txt4count = 0;
			pavi->aviinfo.option |= (AVI_TXTS1 | AVI_TXTS2 | AVI_TXTS3 | AVI_TXTS4);
			return avi_write_header(&(pavi->aviinfo));
			break;
		case AVIENC_CLOSE:
		case AVIENC_EXIT:
			if(pavi->aviinfo.un32txt1count != 0)	pavi->aviinfo.option |=  AVI_TXTS1;
			else									pavi->aviinfo.option &= ~AVI_TXTS1;
			if(pavi->aviinfo.un32txt2count != 0)	pavi->aviinfo.option |=  AVI_TXTS2;
			else									pavi->aviinfo.option &= ~AVI_TXTS2;
			if(pavi->aviinfo.un32txt3count != 0)	pavi->aviinfo.option |=  AVI_TXTS3;
			else									pavi->aviinfo.option &= ~AVI_TXTS3;
			if(pavi->aviinfo.un32txt4count != 0)	pavi->aviinfo.option |=  AVI_TXTS4;
			else									pavi->aviinfo.option &= ~AVI_TXTS4;
			return avi_write_header(&(pavi->aviinfo));
		default:
			dprintf("%s Error Call\n", __func__);
			return NULL;
	}
}

int muxer_avienc_idle(t_avienc_t *pavi)
{
	char strtemp[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};

	gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;

	//dprintf("type %d cnt %d\n",pavi->type, pavi->filenamelist->usedsize);
	
	printf("muxer_avienc_idle 0\r\n");
	muxer_avienc_deletefile(pavi);		
	pavi->pf_get_file_name(pavi);
	printf("muxer_avienc_idle 1\r\n");
#if 1	
	if(encHeap_checkQueue(pavi->filenamelist) == 1)
	{	// heap가 overflow 되어(삽입은 성공상태) 삭제할 file이 존재함
		sprintf(strDelBuf, "%s/%s", pavi->dir_name, pavi->filenamelist->delHData.filename);
		if(f_unlink(strDelBuf) != FR_OK)
		{
			printf("f_unlink fail %s\n", strDelBuf);
		}
		else
		{
		//	dprintf("f_unlink ok %s\n", strDelBuf);
		}

#ifdef __JPEG__		
		strncpy(strtemp, pavi->filenamelist->delHData.filename, strlen(pavi->filenamelist->delHData.filename)-4);
		sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
		if(f_unlink(strDelBuf) != FR_OK)
		{
			printf("jpg f_unlink fail %s\n", strDelBuf);
		}
		else
		{
		//	dprintf("jpg f_unlink ok %s\n", strDelBuf);
		}
#endif
		memset(&pavi->filenamelist->delHData, 0, sizeof(encHData));
	}
#endif	
	//dprintf("type %d cnt %d\n",pavi->type, pavi->filenamelist->usedsize);

#ifdef __JPEG__
	if(jpeg_thumbnail_save(pavi) == -1)
	{			
		int res;
		if(pavi->pf_sd_free)
		{
			res = pavi->pf_sd_free(pavi, 0, 0);
			if(res != FR_OK)
			{
				res = pavi->pf_sd_free(pavi, 0, 0);
			}
		}			
		pavi->state = AVIENC_IDLE;
		//muxer_avienc_delete();
		//gtUser.bSdAviSave = SDSAVE_USE_OFF;
		//pavi->state = AVIENC_OFF;
		
		return 0;
	}
#endif		

//	if(g711_mb.sleepmodeE == 1)
//	{		
//		pavi->state = AVIENC_OFF;
//		EN672_sleepmode(1);
//		g711_mb.sleepmodeE = 0;
//		return 0;
//	}
	// 초기화
	pavi->state = AVIENC_OPEN;

	return muxer_avienc_open(pavi);
}

int muxer_avienc_open(t_avienc_t *pavi)
{	printf("muxer_avienc_open \n");
	ENTER();

	FRESULT fres;
	UINT nFreePer = 0, nOverCnt = 0;
	UINT stime_end, stime_end1;

	// File Path Create	
	char strtemp[_MAX_LFN+1] = {0};
	char strBuf[_MAX_LFN+1] = {0};
	char strDelBuf[_MAX_LFN+1] = {0};
	char strJpgBuf[_MAX_LFN+1] = {0};

	strcpy(strJpgBuf, pavi->now_name);
	
	sprintf(pavi->now_name, "%s%s", pavi->now_name, SAVE_AVI_FORMAT);
	sprintf(strBuf, "%s/%s", pavi->dir_name, pavi->now_name);
	printf("muxer_avienc_open strBuf %s\n",strBuf);

	// Start Time Get
	pavi->stime = xTaskGetTickCount();
	
	// File Open
	if((fres=f_open(&pavi->fpavi, strBuf, FA_WRITE | FA_CREATE_ALWAYS)) != FR_OK)
	{
		dprintf("%s Error : f_open AVI data File system fail : %s\n", __func__, strBuf);
		dprintf("Err Msg : %s\n", put_rc(fres));
		//muxer_avienc_error(pavi, AVI_OPENFAIL);
		//jpeg delete	
		sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strJpgBuf, SAVE_JPEG_THUMBNAIL_FORMAT);
		if(f_unlink(strDelBuf) != FR_OK)
		{
			printf("jpg f_unlink fail %s\n", strDelBuf);
		}
		else
		{
			dprintf("jpg f_unlink ok %s\n", strDelBuf);
		}
				
		int res;
		if(pavi->pf_sd_free)
		{
			res = pavi->pf_sd_free(pavi, 0, 0);
			if(res != FR_OK)
			{
				res = pavi->pf_sd_free(pavi, 0, 0);
			}
		}			
		pavi->state = AVIENC_IDLE;
		goto done;
		goto done;
	}
	else
	{
	
		stime_end1 = xTaskGetTickCount();
		pavi->filesize = 0;
		pavi->buffer.buf = pavi->buffer.base;
		pavi->buffer.length = 0;

		pavi->idx_len = 0;
#ifdef CONTIGUOUS_WRITE    
		pavi->sect = 0;
		pavi->org_sect = 0;
			
#ifdef AUXCLUST			
		pavi->org_sect = alloc_aux_clusters(&pavi->fpavi, pavi->clust_start, pavi->clust_end, pavi->maxdata_len, pavi->type);
#else
		pavi->org_sect = avio_alloc_clusters(&pavi->fpavi, pavi->clust_start, pavi->maxdata_len,  0);
#endif		
		if (!pavi->org_sect) {
			dprintf("Function failed due to any error or insufficient contiguous area.\n");
			//muxer_avienc_error(pavi, AVI_WRITEFAIL);
			f_close(&pavi->fpavi);
			if(f_unlink(strBuf) != FR_OK)
			{
				printf("f_unlink fail %s\n", strBuf);
			}
			else
			{
				dprintf("f_unlink ok %s\n", strBuf);
			}	

			//jpeg delete	
		    sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strJpgBuf, SAVE_JPEG_THUMBNAIL_FORMAT);
			if(f_unlink(strDelBuf) != FR_OK)
			{
				printf("jpg f_unlink fail %s\n", strDelBuf);
			}
			else
			{
				dprintf("jpg f_unlink ok %s\n", strDelBuf);
			}


			
			int res;
			if(pavi->pf_sd_free)
			{
				res = pavi->pf_sd_free(pavi, 0, 0);
				if(res != FR_OK)
				{
					res = pavi->pf_sd_free(pavi, 0, 0);
				}
			}			
			//muxer_avienc_delete();
			//gtUser.bSdAviSave = SDSAVE_USE_OFF;
			//pavi->state = AVIENC_OFF;
			pavi->state = AVIENC_IDLE;
			goto done;
		}

		stime_end = xTaskGetTickCount();
		
		static UINT TOTALTIME = 0, TOTALCNT = 0;
		static UINT MAXTIME = 0, LOWTIME = 10000;	
		static UINT ETOTALTIME = 0, ETOTALCNT = 0;
		static UINT EMAXTIME = 0, ELOWTIME = 10000;	
		
		if( (pavi->fpavi.sclust > pavi->fpavi.fs->last_clust))
		{
			printf("!!!!!!!!!!!!!! sclust! %d last_clust! %d!!!!!!!!!\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			f_close(&pavi->fpavi);

			if(f_unlink(strBuf) != FR_OK)
			{
				printf("f_unlink fail %s\n", strBuf);
			}
			else
			{
				dprintf("f_unlink ok %s\n", strBuf);
			}	
			
			sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strJpgBuf, SAVE_JPEG_THUMBNAIL_FORMAT);
			if(f_unlink(strDelBuf) != FR_OK)
			{
				printf("jpg f_unlink fail %s\n", strDelBuf);
			}
			else
			{
				dprintf("jpg f_unlink ok %s\n", strDelBuf);
			}

			
			pavi->state = AVIENC_IDLE;
			//muxer_avienc_deletefile(pavi);		
			goto done;
		}		
		
		if((pavi->type == AVI_EVENT_SW)	|| (pavi->type == AVI_MANUAL_SW))
		{
			ETOTALTIME += (stime_end - stime_end1)*10;			
			ETOTALCNT += 1;		
			if(EMAXTIME <(stime_end - stime_end1)*10)
				EMAXTIME = (stime_end - stime_end1)*10;
			if(ELOWTIME >(stime_end - stime_end1)*10)
				ELOWTIME = (stime_end - stime_end1)*10;
		
			if(((stime_end1 - pavi->stime)*10)+((stime_end - stime_end1)*10) > 200)
			{
				printf("f_open(%04dms) sect(%04dms) %s A %04dms M %04dms L %04dms \n", (stime_end1 - pavi->stime)*10, (stime_end - stime_end1)*10, strBuf, ETOTALTIME/ETOTALCNT, EMAXTIME, ELOWTIME);
			}
			else
			{
				printf("f_open %s\n", strBuf);			
			}

			if(pavi->fpavi.sclust < (pavi->clust_start) 
				|| pavi->fpavi.sclust > (pavi->clust_end))
			{			
				printf("sclust! %d last_clust %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}

			if(pavi->fpavi.fs->last_clust < (pavi->clust_start) 
				|| pavi->fpavi.sclust > (pavi->clust_end))
			{			
				printf("sclust %d last_clust! %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}
						
			if( (pavi->fpavi.fs->last_clust - pavi->fpavi.sclust) != (EVN_AVI_FILE_SIZE/BYTE_PER_CLUST)-1)
			{
				printf("sclust! %d last_clust! %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}
		}
		else
		{

			TOTALTIME += (stime_end - stime_end1)*10;			
			TOTALCNT += 1;		
			if(MAXTIME <(stime_end - stime_end1)*10)
				MAXTIME = (stime_end - stime_end1)*10;
			if(LOWTIME >(stime_end - stime_end1)*10)
				LOWTIME = (stime_end - stime_end1)*10;		
			
			if(((stime_end1 - pavi->stime)*10)+((stime_end - stime_end1)*10) > 200)
			{
				printf("f_open(%04dms) sect(%04dms) %s A %04dms M %04dms L %04dms \n", (stime_end1 - pavi->stime)*10, (stime_end - stime_end1)*10, strBuf, TOTALTIME/TOTALCNT, MAXTIME, LOWTIME);
			}
			else
			{
				printf("f_open %s\n", strBuf);			
			}

			if(pavi->fpavi.sclust > (pavi->clust_end))
			{			
				printf("sclust! %d last_clust %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}

			if(pavi->fpavi.sclust > (pavi->clust_end))
			{			
				printf("sclust %d last_clust! %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}
			
			if( (pavi->fpavi.fs->last_clust - pavi->fpavi.sclust) != (MAX_AVI_FILE_SIZE/BYTE_PER_CLUST)-1)
			{
				printf("sclust! %d last_clust! %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust); // 종료위치
			}
		}

#ifdef __JPEG__		
		Start_clustAVI = pavi->fpavi.sclust;
		if(Last_clustJPG < Start_clustAVI)
		{
			if((Start_clustAVI - Last_clustJPG) == 1)
			{
				//dprintf("\n OK !!\n");
			}
			else
			{
				printf("\n ERROR!! Start_clustAVI %d Last_clustJPG %d \n", Start_clustAVI, Last_clustJPG);
				//muxer_avienc_error(pavi, AVI_WRITEFAIL);
				f_close(&pavi->fpavi);
				if(f_unlink(strBuf) != FR_OK)
				{
					printf("f_unlink fail %s\n", strBuf);
				}
				else
				{
					dprintf("f_unlink ok %s\n", strBuf);
				}				
				//delete jpg			
				sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strJpgBuf, SAVE_JPEG_THUMBNAIL_FORMAT);
				if(f_unlink(strDelBuf) != FR_OK)
				{
					printf("jpg f_unlink fail %s\n", strDelBuf);
				}
				else
				{
					dprintf("jpg f_unlink ok %s\n", strDelBuf);
				}


				
				int res;
				if(pavi->pf_sd_free)
				{
					res = pavi->pf_sd_free(pavi, 0, 0);
					if(res != FR_OK)
					{
						res = pavi->pf_sd_free(pavi, 0, 0);
					}
				}	
				pavi->state = AVIENC_IDLE;				
				goto done;		
			}
		}
		else
		{
			printf("\n clust over!!\n");
		}
#endif		
		pavi->sect      = pavi->org_sect;
		pavi->end_sect  = pavi->org_sect + pavi->maxdata_len/*MAX_AVI_FILE_SIZE*//512;
#endif

#if 1	
		if(encHeap_addQueue(pavi->filenamelist, pavi->now_name) == 1)
		{	// heap가 overflow 되어(삽입은 성공상태) 삭제할 file이 존재함
			if(strcmp(pavi->now_name, pavi->filenamelist->delHData.filename) == 0)
			{
				// 방금 open한 파일이 삭제할 대상이 됨 : 이것 해결방안은???????
				dprintf("%s Error : f_open old file name??? : %s\n", __func__, pavi->now_name);
				muxer_avienc_delete();
				pavi->state = AVIENC_OFF;
				gtUser.bSdAviSave = SDSAVE_USE_OFF;//muxer_avienc_error(pavi, AVI_OPENFAIL);
				return 0;
			}
			sprintf(strDelBuf, "%s/%s", pavi->dir_name, pavi->filenamelist->delHData.filename);
			if(f_unlink(strDelBuf) != FR_OK)
			{
				printf("f_unlink fail %s\n", strDelBuf);
			}
			else
			{
				dprintf("f_unlink ok %s\n", strDelBuf);
			}

#ifdef __JPEG__		
			strncpy(strtemp, pavi->filenamelist->delHData.filename, strlen(pavi->filenamelist->delHData.filename)-4);
			sprintf(strDelBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, strtemp, SAVE_JPEG_THUMBNAIL_FORMAT);
			if(f_unlink(strDelBuf) != FR_OK)
			{
				printf("jpg f_unlink fail %s\n", strDelBuf);
			}
			else
			{
				dprintf("jpg f_unlink ok %s\n", strDelBuf);
			}
#endif
			memset(&pavi->filenamelist->delHData, 0, sizeof(encHData));
		}
#endif		
		

	}

	
	// Make AVI Header, Buffer Write AVI Header, Header Size Check
	BYTE *aviheader = muxer_avienc_makeaviheader(pavi);
	muxer_avienc_bufferadd(pavi, aviheader, AVI_HEADSIZE);

	pavi->state = AVIENC_FIRST;
	return 1;

done:
	LEAVE();
	return 0;
}

int muxer_avienc_frist_video(t_avienc_t *pavi)
{
	if(sbuf_get(&(pavi->h264_sbuf), &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)	// 데이터 존재확인
	{
		if(pavi->datablock.flags == 1)	 // I frame 이라면
		{
			/*
				key frame means I-frame of H.264 
				we flushs sbuf for audio and txt
			*/

#ifdef __AUDIO__
			// auds도 key frame으로 이동
			while(cQueue_isempty(&(pavi->g711_sbuf)) != DEF_OK)
			{
				if(pavi->g711_sbuf.data[pavi->g711_sbuf.head].flags != 1)
				{
					sbuf_head_next(&(pavi->g711_sbuf));
				}
				else
					break;
			}
#endif

#ifdef __AVI_TXTS__
			// txts도 key frame으로 이동
			while(cQueue_isempty(&(pavi->txt1_sbuf)) != DEF_OK)
			{
				if(pavi->txt1_sbuf.data[pavi->txt1_sbuf.head].flags != 1)
				{
					sbuf_head_next(&(pavi->txt1_sbuf));
				}
				else
					break;
			}
#endif
			// 초기화
			pavi->state = AVIENC_WRITE;
			pavi->datablock.state = 1;	// datablock에 데이터 존재(video)
			muxer_avienc_write(pavi);

			// 초기화
			//pavi->state = AVIENC_WRITE;
			//pavi->datablock.state = 1;	// datablock에 데이터 존재(video)
			//muxer_avienc_write(pavi);
			
			//dprintf("video saving... ");
			//UartPutByte('>');
			return 1;
		}
	}
	return 0;
}

BYTE baudio_g[1024] = {0};
void muxer_avienc_write_process(t_avienc_t *pavi, int type)
{
	ENTER();
	//printf("avi write type %d\r\n", type);
#if 0
	if(type == H264_STREAM && pavi->datablock.flags)
	{	// 최소 1s 단위의 write command
		muxer_avienc_bufferadd(pavi, NULL, -1);
	}
#endif
  // pos (or offset) indicates the start of the chunk in bytes relative to the 
  // movi list chunk
  // index add : 00dc flags pos size
  // pavi->aviinfo.i_header_end inicates offset to the movi list chunk
  // pavi->buffer.length is offset in pavi->buffer
  // pavi->filesize is current length of avi file

	// index add : 00dc flags pos size 
//	int pos = (pavi->filesize + pavi->buffer.length) - pavi->aviinfo.i_header_end;
	int pos = (pavi->filesize) - pavi->aviinfo.i_header_end;
	AVIINDEX_t avi_idx = avi_write_index(type, pos, pavi->datablock.size, pavi->datablock.flags); // 16byte
	if(pavi->idx_info != NULL)
	{
		uint8 *idx_pos = (pavi->idx_info + pavi->idx_len);
		//DmaMemCpy((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t), NULL);
		//invalidate_dcache_range(idx_pos, idx_pos + sizeof(AVIINDEX_t));
		//memcpy((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t));
		//DmaMemCpy((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t));
		//invalidate_dcache_range((UINT)idx_pos, ((UINT)idx_pos) + sizeof(AVIINDEX_t));
		memcpy((BYTE *)idx_pos, (BYTE *)&avi_idx, sizeof(AVIINDEX_t));
		pavi->idx_len += sizeof(AVIINDEX_t);
	}
	else
	{
		dprintf("err idx info null!!!!\n");
	}
	
	// data add : 00dc size data
	FCC_CB_t fcccb = avi_write_chunkheader(type, pavi->datablock.size);			// 8byte
	muxer_avienc_bufferadd(pavi, (uint8 *)&fcccb, sizeof(FCC_CB_t));
	muxer_avienc_bufferadd(pavi, (uint8 *)pavi->datablock.addr, AVI_ALIGN(pavi->datablock.size));

done:
	//UartPutByte('.');
	LEAVE();
}

void muxer_avienc_write(t_avienc_t *pavi)
{
	ENTER();

	if(pavi->datablock.state == 1)
	{
		//UartPutByte('H');
		//printf("h");
		muxer_avienc_write_process(pavi, H264_STREAM);
		(pavi->aviinfo.un32vidscount)++;
		pavi->datablock.state = 0;
	}

#ifdef __AUDIO__
	if(pavi->aviinfo.option & AVI_AUDIO)
	{		
		//printf("a");
		if(pavi->aviinfo.un32audscount < pavi->aviinfo.un32audsmax)
		{
			if(sbuf_get(&pavi->g711_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
			{	
#if 0
				uint32 idx1;
				int8 adb;
				int8 adbd; 
				UINT *paddr;
		
				paddr = (uint8 *)pavi->datablock.addr;
				//printf("pavi->datablock.size (%d) ",pavi->datablock.size);
				for(idx1 = 0; idx1 < pavi->datablock.size; idx1++)
				{
					memcpy(adb,paddr+idx1, sizeof(uint8));
					//printf("(%x) ",adb);
					tbaudio[idx1] = MuLaw_Decode(adb);
					//printf("[%0.4x ",(int16)tbaudio[idx1] );
					if( 0xffffe08f == tbaudio[idx1] )
						tbaudio[idx1] = 0x7f;
					//else if( 0x0831 == tbaudio[idx1] ||0x08b1 == tbaudio[idx1])
					//	tbaudio[idx1] = 0x7f;
					//printf("%0.4x] ",(int16)tbaudio[idx1] );
					adbd = MuLaw_Encode(tbaudio[idx1] );
					//printf("_%x_ ",*((&g711_mb.addr)+idx));
					//if((adb - adbd) != 0)
					//	printf("failed %d: %x %x %x \n",idx, adb,adbd,tbaudio[idx]);
					//*((&pavi->datablock.addr)+idx1) = adbd;
					//printf(" _%x_ ",adbd);
					memcpy((paddr)+idx1,adbd,sizeof(uint16));
				}
				//printf("\n\n");
#endif
				muxer_avienc_write_process(pavi, G711_STREAM);
				pavi->aviinfo.un32audscount++;
			}
		}
	}
#endif

#ifdef __AVI_TXTS__
	if(pavi->aviinfo.option & AVI_TXTS1)
	{
		if(pavi->aviinfo.un32txt1count < pavi->aviinfo.un32txt1max)
		{
//			if(pavi->aviinfo.un32vidscount % 2 == 0)
			{
				if(sbuf_get(&pavi->txt1_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
				{
					//UartPutByte('A');
					muxer_avienc_write_process(pavi, TXT1_STREAM);
					(pavi->aviinfo.un32txt1count)++;
				}
			}
		}
	}
/*	if(pavi->aviinfo.option & AVI_TXTS2)
	{
		if(pavi->aviinfo.un32txt2count < pavi->aviinfo.un32txt2max)
		{
			//if(pavi->aviinfo.un32vidscount % 2 == 0)
			{
				if(sbuf_get(&pavi->txt2_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
				{
					muxer_avienc_write_process(pavi, TXT2_STREAM);
					(pavi->aviinfo.un32txt2count)++;
				}
			}
		}
	}*/

#endif

	if(pavi->aviinfo.un32vidscount >= pavi->aviinfo.un32vidsmax)
	{
#ifdef __AUDIO__
		if(pavi->aviinfo.option & AVI_AUDIO)
		{
			//printf("h aud%d audsmax%d\r\n",pavi->aviinfo.un32audscount, pavi->aviinfo.un32audsmax);
			if(pavi->aviinfo.un32audscount >= pavi->aviinfo.un32audsmax)
			{
#ifdef __AVI_TXTS__
				if(pavi->aviinfo.option & AVI_TXTS1)
				{
					if(pavi->aviinfo.un32txt1count >= pavi->aviinfo.un32txt1max)
					{
						pavi->state = AVIENC_CLOSE;
					}	
					else
					{
						//UartPutByte('g');
					}
				}	
#else
				
				pavi->state = AVIENC_CLOSE;
#endif	
			}
			else
			{			
				///UartPutByte('a');
				while(pavi->aviinfo.un32audscount < pavi->aviinfo.un32audsmax)
				{
					if(sbuf_get(&pavi->g711_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
					{ //UartPutByte('b');
						printf("w ");
						muxer_avienc_write_process(pavi, G711_STREAM);
						(pavi->aviinfo.un32audscount)++;
					}
					else						
						pavi->state = AVIENC_CLOSE;
					vTaskDelay(1);
				}
			}
		}
		else
#endif
		{
			pavi->state = AVIENC_CLOSE;

			//muxer_avienc_close(pavi);
		}
	}

done:
	LEAVE();
}

#ifdef __AVIENC_IDX_CHECK__
int muxer_avienc_idx_check(t_avienc_t *pavi)
{
           AVIINDEX_t *avi_idx;
           int nRes = 0;
           int nCnt = 0;
           int nFrameCheck = 0;
           int nICheck = 0;
           int i;

	if(pavi->idx_len == 0)
		return 2;

	avi_idx = pavi->idx_info;
	nCnt = pavi->idx_len / sizeof(AVIINDEX_t);

	for(i=0;i<nCnt;i++)
	{
		if(avi_idx[i].dwChunkId == FCC('0','0','d','c'))
		{          // video 정보
			if(avi_idx[i].dwFlags == NUM32(AVIIF_KEYFRAME))
			{          // I 인 경우
				if(nFrameCheck % 30 != 0)
				{
					nRes = 1;
				}
				nICheck++;
			}
		nFrameCheck++;
		}
	}

           dprintf("Idx Info Count(%d), Video Count(%d), I(%d)\n", nCnt, nFrameCheck, nICheck);

           return nRes;
}
#endif


void muxer_avienc_close(t_avienc_t *pavi)
{
	ENTER();
	

	FRESULT fres;

#ifndef CONTIGUOUS_WRITE  
	muxer_avienc_bufferadd(pavi, NULL, -1);
#endif

#ifndef CONTIGUOUS_WRITE  
	pavi->filesize = f_tell(&(pavi->fpavi));
#endif
//	pavi->aviinfo.i_movi_end = pavi->filesize + pavi->buffer.length;
	pavi->aviinfo.i_movi_end = pavi->filesize;
	printf("File Close file size : (%d)(%d)\r\n", pavi->filesize, pavi->buffer.length);

	// Write Idx Data
	if(pavi->idx_info != NULL)
	{
		//dprintf("write idx\n");
		FCC_CB_t fcccb = avi_write_chunkheader(IDX1_ARRAY, pavi->idx_len);
		muxer_avienc_bufferadd(pavi, (uint8 *)&fcccb, sizeof(FCC_CB_t));		// 8byte
		muxer_avienc_bufferadd(pavi, (uint8 *)pavi->idx_info, AVI_ALIGN(pavi->idx_len));	// 16byte * framecount
		muxer_avienc_bufferadd(pavi, NULL, -1);	// buffer write(size:-1)

#ifdef __AVIENC_IDX_CHECK__
		 fres = muxer_avienc_idx_check(pavi);	
		 if(fres == 1)
		 {
			dprintf("%s file Idx Error\n", pavi->now_name);
			//gtUser.bSdAviSave = SDSAVE_USE_OFF;
		 }
		 else if(fres == 2)
	 	 {
	 	 	dprintf("%s file Idx empty !!!!!\n", pavi->now_name);
			//gtUser.bSdAviSave = SDSAVE_USE_OFF;
	 	 }
#endif
		
	}
	else
	{
		dprintf("write idx Err !!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}

#ifdef CONTIGUOUS_WRITE
	//Make AVI Header, Write AVI Header
	pavi->aviinfo.i_riff = pavi->filesize;
	pavi->fpavi.fsize = pavi->filesize;
	BYTE *aviheader = muxer_avienc_makeaviheader(pavi);
	muxer_avienc_bufferadd(pavi, aviheader, AVI_HEADSIZE);
	pavi->sect = pavi->org_sect;
	muxer_avienc_bufferadd(pavi, NULL, -1);
#else
	// Make AVI Header, Write AVI Header
	pavi->filesize = f_tell(&(pavi->fpavi));
	pavi->aviinfo.i_riff = pavi->filesize;
	BYTE *aviheader = muxer_avienc_makeaviheader(pavi);
	muxer_avienc_bufferadd(pavi, aviheader, AVI_HEADSIZE);

	f_lseek(&pavi->fpavi, 0);	// fpavi pos : 0
	muxer_avienc_bufferadd(pavi, NULL, -1);
#endif

	// File Close
	UINT etime_s = xTaskGetTickCount();
	
//	gtUser.lastFatClust = pavi->fpavi.fs->last_clust;
//	printf(" last clust %ld \n",gtUser.lastFatClust);
//	UserSave((BYTE*)&gtUser);

	//pavi->fpavi.flag |= FA__WRITTEN;	
	
#ifdef CLOSE_LSEEK		
	if(aux_file_lseek(&pavi->fpavi, pavi->maxdata_len) == -1)
	{
		dprintf("[ f_lseek after  missmatch sclust %d last %d\n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust);
		
		if((fres=f_close(&pavi->fpavi)) != FR_OK)
		{
			dprintf("%s Error : f_close File system fail\n", __func__);
			dprintf("Err Msg : %s\n", put_rc(fres));
		}
		f_unlink(pavi->now_name);
		//jpg delete
		
		pavi->state = AVIENC_IDLE;
		return;
	}
#endif		
	if((fres=f_close(&pavi->fpavi)) != FR_OK)
	{
		dprintf("%s Error : f_close File system fail\n", __func__);
		dprintf("Err Msg : %s\n", put_rc(fres));
	}
	else
	{
		// End Time Get
		pavi->etime = xTaskGetTickCount();
		float tot_time = (pavi->etime - pavi->stime)/100.0;
		//dprintf("f_close(%04dms) : %s : %dbyte/%ds/%dKBps\n", (pavi->etime - etime_s)*10, pavi->now_name, pavi->aviinfo.i_riff, (UINT)tot_time, (UINT)(pavi->aviinfo.i_riff/tot_time/1024*8));
		dprintf("f_close(%04dms) : %dbyte/%ds/%dKBps\n", (pavi->etime - etime_s)*10, pavi->aviinfo.i_riff, (UINT)tot_time, (UINT)(pavi->aviinfo.i_riff/tot_time/1024*8));
	}

	if(pavi->type == AVI_EVENT_SW)	{	g_eventgo = 0;	g_normalgo = 1;	}
	if(pavi->type == AVI_MANUAL_SW)	{	g_manualgo = 0;	g_normalgo = 1;	}

	pavi->state = AVIENC_IDLE;

done://dprintf("muxer_avienc_close 2\n");
	enc_ing = NULL;

	LEAVE();
}

void muxer_avienc_process(t_avienc_t *pavi)
{
	int ck;
	//printf("muxer_avienc_process state %d\r\n",pavi->state);
	//printf("p");
	switch(pavi->state)
	{	
		case AVIENC_OFF:			
			if(gtUser.bSdAviSave == SDSAVE_USE_ON
				&& h264_mb.playSwitch == 0 
				&& h264_mb.playbackgo == 0)
			{
				printf("muxer_avienc_process AVIENC_ON!!! \n");
				pavi->state = AVIENC_IDLE;
			}
			vTaskDelay(1);
			break;
		case AVIENC_IDLE:	// => AVIENC_OPEN call => AVIENC_WRITE call
		  	//printf("AVIENC_IDLE call\r\n");
			
			if(gtUser.bSdAviSave == SDSAVE_USE_OFF)
			{
				printf("AVIENC_IDLE SDSAVE_USE_OFF call\r\n");
				muxer_avienc_delete();
				bSD_FAT_mounted = 0;
				g_normalgo = 0;
				g_eventgo = 0;
				g_manualgo = 0;
				enc_ing = NULL;
				pavi->state = AVIENC_OFF;
				//gptMsgShare.EVENTstate = 0;
				//stt_avienc = NULL;
				//vTaskDelete(NULL);
				return 0;
			}
			enc_ing = pavi;
						
			if(muxer_avienc_idle(pavi) == 0)
				break;
		case AVIENC_FIRST:
			if(muxer_avienc_frist_video(pavi))
				break;
		case AVIENC_WRITE:	// => AVIENC_WRITE loop => AVIENC_CLOSE call
			//	UartPutByte(',');
			//printf(",");
			if(gtUser.bSdAviSave == SDSAVE_USE_ON)
			{
				enc_ing = pavi;
				//We, first should check if pavi->totl is larger than MAX_AVI_FILE_SIZE
				if/*while*/(sbuf_peek(&pavi->h264_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
				{//printf("1");
#ifdef  CONTIGUOUS_WRITE				
					if(pavi->filesize + pavi->datablock.size + pavi->idx_len > pavi->maxdata_len/*MAX_AVI_FILE_SIZE*/)
					{
						pavi->state = AVIENC_CLOSE;						
						//muxer_avienc_close(pavi);
						break;
					}
#endif					

					if(pavi->aviinfo.un32vidscount < pavi->aviinfo.un32vidsmax)
					{
						if(sbuf_get(&pavi->h264_sbuf, &(pavi->datablock.addr), &(pavi->datablock.size), &(pavi->datablock.flags)) == DEF_OK)
						{
							//printf("pavi->datablock.size %d ", pavi->datablock.size);
							pavi->datablock.state = 1;
							muxer_avienc_write(pavi);
						}
					}
#ifdef __AUDIO__
					else
					{
						pavi->datablock.state = 0;
						muxer_avienc_write(pavi);
					}
#endif

					if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
					{
						//UartPutByte('+');
						break;
					}
					else if((gtUser.bSdAviSave == SDSAVE_USE_OFF)
#ifdef __CBB__						
						||is_update
#endif						
						)
					{
						//UartPutByte('>');
						pavi->state = AVIENC_EXIT;
						break;
					}
					//vTaskDelay(1);
				}
			}
			else
			{//UartPutByte('=');
				pavi->state = AVIENC_EXIT;
			}
			break;
		case AVIENC_CLOSE:	// CLOSE 작업중이다.
			//tprintf("AVIENC_CLOSE call\n");
			muxer_avienc_close(pavi);
			break;
		case AVIENC_EXIT:	// User Command(Shell or GPIO)
			tprintf("AVIENC_EXIT call\n");
			muxer_avienc_close(pavi);
			muxer_avienc_delete();
			bSD_FAT_mounted = 0;
			gtUser.bSdAviSave = SDSAVE_USE_OFF;
			g_normalgo = 0;
			g_eventgo = 0;
			g_manualgo = 0;
			pavi->state = AVIENC_OFF;	
			enc_ing = NULL;
			vTaskDelay(1);
			//gptMsgShare.EVENTstate = 0;
			stt_avienc = NULL;
			vTaskDelete(NULL);
			break;
		default:
			muxer_avienc_error(pavi, AVI_STATEFAIL);
			break;
	}
}

void muxer_avienc_normal_go(void)
{
	g_normalgo = 1;	// 상시 녹화 활성화
}

int muxer_avienc_normal_state(void)
{
	return g_normalgo;
}

void muxer_avienc_event_go(void)
{
	if(
#ifdef __CBB__		
		(bSD_State!=sd_SAVE)||
#endif
		(g_manualgo))
		return;
	//g711_mb.wBuzzCnt=5;
	g_eventgo = 1;	// 충격 녹화 활성화
}

int muxer_avienc_event_state(void)
{
	return g_eventgo;
}

void muxer_avienc_manual_go(void)
{
	if(
#ifdef __CBB__		
		(bSD_State!=sd_SAVE)||
#endif		
		(g_eventgo))
		return;
	g711_mb.wBuzzCnt=5;
	g_manualgo = 1;	// 메뉴얼 녹화 활성화
}

int muxer_avienc_manual_state(void)
{
	return g_manualgo;
}


t_avienc_t *muxsr_avienc_get_t_avienc_t(void)
{
	return enc_ing;
}


void IsrH264Enc(void *arg)/*sbuf_irq_handler*/
{	
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	//SYS_IRQ1 = 0;
	//printf("s\r\n");

#ifdef __AVI_TXTS__
	static uint32 vl = 0, q = 2;
#endif

#ifdef __CBB__
	newcbmode = cbbpower_get_mode();
	if(newcbmode == eCBB_Off)
		newcbmode = oldcbmode;
	oldcbmode = newcbmode;
#endif

	if(h264_mb.ready/* && h264_mb.type == 1 */)
	{
#ifdef __CBB__
		ck_sdsave = 0;
#endif
		h264_mb.ready = 0;
		muxer_avienc_h264Buffering(h264_mb.addr, h264_mb.size, h264_mb.type, h264_mb.total);
		


#ifdef __AUDIO__
	if(g711_mb.ready)
	{
		g711_mb.ready = 0;
		//printf(" %d ",g711_mb.size);

		muxer_avienc_g711Buffering(g711_mb.addr, g711_mb.size, g711_mb.type, h264_mb.total);
	}
#endif

#ifdef __CBB__
#ifdef __AVI_TXTS__
		//if(h264_mb.type)
		{	// GPS info
			vl = 0;
			GPSInfo *info_gps;
			if(gps_info_get(&info_gps) == DEF_OK)
			{
				//save data
				muxer_avienc_txt1Buffering(info_gps->info[eGPS_RMC].strData, GPSData_len, h264_mb.type);
				memcpy(saved_info_gps,info_gps->info[eGPS_RMC].strData,GPSData_len);
#ifdef __GPS__
				//printf("put_text_streams ");
				txts_mb.addr = info_gps->info[eGPS_RMC].strData;
				txts_mb.size = GPSData_len;
#endif		
			}
			else
			{
				//copy data				
				muxer_avienc_txt1Buffering(saved_info_gps, GPSData_len, h264_mb.type);
#ifdef __GPS__
			//printf("put_text_streams ");
				txts_mb.addr = saved_info_gps;
				txts_mb.size = GPSData_len;
#endif		
				
			}
			txts_mb.ready = 1;//ksj_151228_stream 
		}
#endif
#endif
	}

	if(g711_mb.manual == 1)
	{
		//printf("bt button on!!\n");
		if(g_manualgo == 0)
		{
			muxer_avienc_manual_go();
			dprintf("manual button on!!\n");
		}	
		g711_mb.manual = 0;
	}

#ifdef __CBB__
	if(gssSem)
		xSemaphoreGiveFromISR(gssSem, &xHigherPriorityTaskWoken);
#endif
	if(bufSem)
		xSemaphoreGiveFromISR(bufSem, &xHigherPriorityTaskWoken);
	if(xHigherPriorityTaskWoken)
		gbXsrTaskSwitchNeeded = 1;	// Task switch required?
}

void muxer_avienc_init(void)
{
	ENTER();
	int nTotal = 0, nEvtTotal = 0;
	UINT muxer_sclust, muxer_sclust_evt, muxer_eclust, muxer_sclust_patition;
	
    if(!Sdio0GetInstall())
    {
		bSD_FAT_mounted = 1;

#ifdef CONTIGUOUS_WRITE
        muxer_sclust = avio_get_cluster_offset(&gtFatFs[MMC], MAX_AVI_FILE_SIZE + MAX_JPEG_FILE_SIZE);
		muxer_sclust_evt = avio_get_cluster_offset(&gtFatFs[MMC], EVN_AVI_FILE_SIZE + MAX_JPEG_FILE_SIZE);
		muxer_eclust = avio_get_cluster_offset(&gtFatFs[MMC], 0);

		muxer_sclust_patition = (muxer_eclust/100)*DEFAULT_RATIO_NORMAL;
		dprintf("muxer_sclust %d muxer_sclust_evt %d muxer_sclust_patition %d muxer_eclust %d \n",muxer_sclust, muxer_sclust_evt, muxer_sclust_patition, muxer_eclust);

#if 0
		nTotal = (muxer_sclust_patition - (muxer_sclust*6)) / muxer_sclust;
		nEvtTotal = (muxer_eclust - muxer_sclust_patition - (muxer_sclust_evt*6))/muxer_sclust_evt;

		muxer_sclust_patition = nTotal * muxer_sclust + (muxer_sclust*4);
		muxer_eclust = muxer_sclust_patition + (((nEvtTotal/2)+(nEvtTotal/2)) * muxer_sclust_evt) + (muxer_sclust_evt*4);
		dprintf("fixed muxer_sclust %d muxer_sclust_evt %d muxer_sclust_patition %d muxer_eclust %d \n",muxer_sclust, muxer_sclust_evt, muxer_sclust_patition, muxer_eclust);
#else
		nTotal = (muxer_sclust_patition - (muxer_sclust*4)) / muxer_sclust;
		nEvtTotal = (muxer_eclust - muxer_sclust_patition - (muxer_sclust_evt*4))/muxer_sclust_evt;
#endif
		nSAVEcnt.normalmaxcnt = nTotal;
		nSAVEcnt.eventmaxcnt = nEvtTotal/2;
		nSAVEcnt.manualmaxcnt = nEvtTotal/2;
		
		dprintf("NORMAL %d  EVENT %d  MANUAL %d possible\n",nSAVEcnt.normalmaxcnt, nSAVEcnt.eventmaxcnt, nSAVEcnt.manualmaxcnt) ;

		
#endif

//		if(nSD_CARD_SIZE == 8)	// 8GB
//		{
//			nSD_CARD_FLAG = 0;
//		}
//		else if(nSD_CARD_SIZE == 16)	// 16GB
//		{
//			nSD_CARD_FLAG = 1;
//		}
//		else if(nSD_CARD_SIZE == 32)	// 32GB
//		{
//			nSD_CARD_FLAG = 2;
//		}

		// 주행 - 상시
#ifdef CONTIGUOUS_WRITE  
		avienc_info_default(&enc_nor, muxer_sclust, muxer_sclust, muxer_sclust_patition, MAX_AVI_FILE_SIZE, MAX_JPEG_FILE_SIZE);
#else
		avienc_info_default(&enc_nor);
#endif
#ifdef AVIENC_HEAP_USE		
		avienc_info_init_filenamelist(&enc_nor, nSAVEcnt.normalmaxcnt);	// max
#endif		
		avienc_info_init_savetime(&enc_nor, SAVE_NOR_TIME_MAX, SAVE_NOR_VIDEO_FPS, AUDIO_FPS, TXTS_GPS_FPS , TXTS_GSS_FPS, TXTS_TIME_FPS, TXTS_OBD_FPS);

		// 주행 - 충격
#ifdef CONTIGUOUS_WRITE  
		avienc_info_default(&enc_evt, muxer_sclust_evt, muxer_sclust_patition, muxer_eclust, EVN_AVI_FILE_SIZE, MAX_JPEG_FILE_SIZE);
#else		
		avienc_info_default(&enc_evt);
#endif
#ifdef AVIENC_HEAP_USE		
		avienc_info_init_filenamelist(&enc_evt, nSAVEcnt.eventmaxcnt);	// 60 / 30
#endif		
		avienc_info_init_savetime(&enc_evt, SAVE_EVT_TIME_MAX, SAVE_EVT_VIDEO_FPS, AUDIO_FPS, TXTS_GPS_FPS, TXTS_GSS_FPS, TXTS_TIME_FPS, TXTS_OBD_FPS);

		// 주행 - 수동
#ifdef CONTIGUOUS_WRITE  
		avienc_info_default(&enc_mar, muxer_sclust_evt, muxer_sclust_patition, muxer_eclust, EVN_AVI_FILE_SIZE, MAX_JPEG_FILE_SIZE);
#else		
		avienc_info_default(&enc_mar);
#endif
#ifdef AVIENC_HEAP_USE		
		avienc_info_init_filenamelist(&enc_mar, nSAVEcnt.manualmaxcnt);	// 20 / 10
#endif		
		avienc_info_init_savetime(&enc_mar, SAVE_MAN_TIME_MAX, SAVE_MAN_VIDEO_FPS, AUDIO_FPS, TXTS_GPS_FPS, TXTS_GSS_FPS, TXTS_TIME_FPS, TXTS_OBD_FPS);
    }
#ifdef __CBB__
#ifdef __GPS__
	memset(saved_info_gps,0x00,sizeof(char)*GPSData_len+4);
#endif
#endif
	dprintf("muuxer_avienc_init end\r\n") ;

	LEAVE();
}

void muxer_avienc_delete(void)
{
	ENTER();

	avienc_info_deinit(&enc_nor);
	avienc_info_deinit(&enc_evt);
	avienc_info_deinit(&enc_mar);

	LEAVE();
}

void muxer_avienc_deletefile(t_avienc_t *pavi)
{
	FRESULT fres;
	UINT nFreePer = 0, nOverCnt = 0;
	UINT wtime_s;
	UINT wtime_e;
	UINT wtime;

	wtime_s = xTaskGetTickCount();
	printf("%s\r\n",__func__);

#ifdef AVIENC_HEAP_USE
		// ON인 경우 주행충격과 주행수동의 overflow된 것만 삭제해야 한다.
		
		nFreePer = fat_get_freesize(MMC);	// 남은 공간이 5% 미만이면

		if(enc_nor.filenamelist->usedsize > nSAVEcnt.normalmaxcnt)
		{	// 주행충격의 기본값보다 usedsize가 크다면,
			if(enc_nor.pf_sd_free)
			{
				nOverCnt = enc_nor.filenamelist->usedsize - nSAVEcnt.normalmaxcnt;
				nFreePer = enc_nor.pf_sd_free(&enc_nor, nOverCnt, nFreePer);
				if(nFreePer < MIX_FREE_SD_4CENT)
				{
					dprintf("%s(%d) : enc_nor min\n", __func__, __LINE__);
				}				
			}
		}

		if(enc_evt.filenamelist->usedsize > nSAVEcnt.eventmaxcnt)
		{	// 주행충격의 기본값보다 usedsize가 크다면,
			if(enc_evt.pf_sd_free)
			{
				nOverCnt = enc_evt.filenamelist->usedsize - nSAVEcnt.eventmaxcnt;
				nFreePer = enc_evt.pf_sd_free(&enc_evt, nOverCnt, nFreePer);
				if(nFreePer < MIX_FREE_SD_4CENT)
				{
					dprintf("%s(%d) : enc_evt min\n", __func__, __LINE__);
				}				
			}
		}
		
		if(enc_mar.filenamelist->usedsize > nSAVEcnt.manualmaxcnt)
		{	// 주행충격의 기본값보다 usedsize가 크다면,
			if(enc_mar.pf_sd_free)
			{	// 넘친 값을 계산하여 최대 그 값만큼 삭제
				nOverCnt = enc_mar.filenamelist->usedsize - nSAVEcnt.manualmaxcnt;
				nFreePer = enc_mar.pf_sd_free(&enc_mar, nOverCnt, nFreePer);
				if(nFreePer < MIX_FREE_SD_4CENT)
				{
					dprintf("%s(%d) : enc_mar min\n", __func__, __LINE__);
				}
			}
		}

//		nFreePer = fat_get_freesize(MMC);	// 남은 공간이 5% 미만이면
		while(nFreePer < MIX_FREE_SD_4CENT)
		{
			fres = enc_nor.pf_sd_free(&enc_nor, nOverCnt, nFreePer);
			if(fres != FR_OK)
			{
				fres = enc_nor.pf_sd_free(&enc_nor, nOverCnt, nFreePer);
			}
		
			nFreePer = fat_get_freesize(MMC);
		}
	
#else

	if(pavi->pf_sd_free)
	{

		nFreePer = fat_get_freesize(MMC);
		while(nFreePer < MIX_FREE_SD_4CENT)
		{
			fres = pavi->pf_sd_free( pavi, nOverCnt, nFreePer);
			if(fres != FR_OK)
			{
				fres = enc_nor.pf_sd_free(&enc_nor, nOverCnt, nFreePer);
			}
			if(fres != FR_OK)
			{
				fres = enc_evt.pf_sd_free(&enc_evt, nOverCnt, nFreePer);
			}
			if(fres != FR_OK)
			{
				fres = enc_mar.pf_sd_free(&enc_mar, nOverCnt, nFreePer);
			}
			
			nFreePer = fat_get_freesize(MMC);
		}
	}
#endif	

	wtime_e = xTaskGetTickCount();
	wtime = (wtime_e - wtime_s)*10;

	if(wtime > 1000)
	{  
		dprintf("delete (%04dms)\n", wtime);
	}

}

int muxer_avienc_sbuf_depth_sum(void)
{
	//printf("muxer_avienc_sbuf_depth_sum cbbpower_get_mode() %d\n",cbbpower_get_mode());
	int a = 0, b = 0, c = 0;
#ifdef __CBB__
	switch(cbbpower_get_mode())
	{
		case eCBB_Drive:
			a = (cQueue_isempty((&enc_nor.h264_sbuf)) ? DEF_FAIL : DEF_OK);
			b = (cQueue_isempty((&enc_evt.h264_sbuf)) ? DEF_FAIL : DEF_OK);
			c = (cQueue_isempty((&enc_mar.h264_sbuf)) ? DEF_FAIL : DEF_OK);
			return (a | b | c);
		case eCBB_Off:
			return 1;
		default:
			return 0;
	}
#else
	int d = 0, e = 0;
	a = (cQueue_isempty((&enc_nor.h264_sbuf)) ? DEF_FAIL : DEF_OK);
	b = (cQueue_isempty((&enc_evt.h264_sbuf)) ? DEF_FAIL : DEF_OK);
	c = (cQueue_isempty((&enc_mar.h264_sbuf)) ? DEF_FAIL : DEF_OK);
	return (a | b | c );
#endif
}

/**
	h.264 stream is buffered in ddr and written to sd card. 	
*/
//extern int is_settingini;
void muxer_avienc(void)
{
	ENTER();

	//if(!muxer_avienc_sdCardMounted())
	//{
	//	vTaskDelay(1);
	 //   goto done;
	//
	//}

#ifdef __CBB__
	//printf("cbbpower_get_mode() %d\n",cbbpower_get_mode());
	//printf("e");
	switch(cbbpower_get_mode())
	{
		case eCBB_Drive:
			if(g_eventgo)	// 주행(충격)녹화 활성화 중이라면
			{
				if(g_normalgo)	// 주행(상시)녹화는
				{
					if(enc_nor.state == AVIENC_OPEN || enc_nor.state == AVIENC_WRITE)
					{
						enc_nor.state = AVIENC_CLOSE;
						//muxer_avienc_close(&enc_nor);	// 종료하고
						muxer_avienc_process(&enc_nor);
					}
					g_normalgo = 0;
					goto done;
				}
				muxer_avienc_process(&enc_evt);		// 주행(충격)녹화 시작
			}
			else if(g_manualgo)	// 주행(메뉴얼)녹화 활성화 중이라면
			{
				if(g_normalgo)	// 주행(상시)녹화는
				{
					if(enc_nor.state == AVIENC_OPEN || enc_nor.state == AVIENC_WRITE)
					{
						enc_nor.state = AVIENC_CLOSE;
						//muxer_avienc_close(&enc_nor);	// 종료하고
						muxer_avienc_process(&enc_nor);
					}
					g_normalgo = 0;
					goto done;
				}
				muxer_avienc_process(&enc_mar);		// 주행(메뉴얼)녹화 시작
			}
			else if(g_normalgo)	// 주행(상시)녹화 활성화 중이라면
			{
				muxer_avienc_process(&enc_nor);		// 주행(상시)녹화 시작
			}
			else
			{
				if(gtUser.bSdAviSave == SDSAVE_USE_ON
					&& enc_nor.state == AVIENC_EXIT 
					&& enc_evt.state == AVIENC_EXIT 
					&& enc_mar.state == AVIENC_EXIT)
				{	// close 할 것도 없음.
					muxer_avienc_delete();
					bSD_FAT_mounted = 0;
					gtUser.bSdAviSave = SDSAVE_USE_OFF;
					g_normalgo = 0;
					g_eventgo = 0;
					g_manualgo = 0;
					//gptMsgShare.EVENTstate = 0;
					stt_avienc = NULL;
					vTaskDelete(NULL);
				}
				vTaskDelay(1);
			}
			break;
		case eCBB_Off:
			
			if(g_normalgo)
			{
				//enc_nor.state = AVIENC_EXIT;
				muxer_avienc_process(&enc_nor);	// Task Kill
			}
			else if(g_eventgo)
			{
				//enc_evt.state = AVIENC_EXIT;
				muxer_avienc_process(&enc_evt);	// Task Kill
			}
			else if(g_manualgo)
			{
				//enc_mar.state = AVIENC_EXIT;
				muxer_avienc_process(&enc_mar);	// Task Kill
			}
			else
			{
				if(gtUser.bSdAviSave == SDSAVE_USE_ON
					&& enc_nor.state == AVIENC_EXIT 
					&& enc_evt.state == AVIENC_EXIT 
					&& enc_mar.state == AVIENC_EXIT)
				{
					muxer_avienc_delete();
					bSD_FAT_mounted = 0;
					g_normalgo = 0;
					g_eventgo = 0;
					g_manualgo = 0;
					//gptMsgShare.EVENTstate = 0;
					stt_avienc = NULL;
					vTaskDelete(NULL);
				}
				vTaskDelay(1);
			}
			break;
		default:
			break;
	}
#else
	if(g_eventgo)	// 주행(충격)녹화 활성화 중이라면
	{
		if(g_normalgo)	// 주행(상시)녹화는
		{
			if(enc_nor.state == AVIENC_OPEN || enc_nor.state == AVIENC_WRITE)
			{
				enc_nor.state = AVIENC_CLOSE;
				//muxer_avienc_close(&enc_nor);	// 종료하고
			}
			muxer_avienc_process(&enc_nor);	
			g_normalgo = 0;
			goto done;
		}

		if(enc_evt.state == AVIENC_IDLE)
		{
		
			//enc_evt.pf_get_file_name(&enc_evt);
			//muxer_avienc_deletefile(&enc_evt);

//#ifdef __JPEG__
//			jpeg_thumbnail_save(&enc_evt);
//#endif
		}

		muxer_avienc_process(&enc_evt);		// 주행(충격)녹화 시작
	}
	else if(g_manualgo)	// 주행(메뉴얼)녹화 활성화 중이라면
	{
		if(g_normalgo)	// 주행(상시)녹화는
		{
			if(enc_nor.state == AVIENC_OPEN || enc_nor.state == AVIENC_WRITE)
			{
				enc_nor.state = AVIENC_CLOSE;
				//muxer_avienc_close(&enc_nor);	// 종료하고
			}
			muxer_avienc_process(&enc_nor); 

			g_normalgo = 0;
			goto done;
		}

		if(enc_mar.state == AVIENC_IDLE)
		{
			//enc_mar.pf_get_file_name(&enc_mar);
			//muxer_avienc_deletefile(&enc_mar);
//#ifdef __JPEG__
//			jpeg_thumbnail_save(&enc_mar);
//#endif
		}
		muxer_avienc_process(&enc_mar);		// 주행(메뉴얼)녹화 시작
	}
	else if(g_normalgo)	// 주행(상시)녹화 활성화 중이라면
	{
		// File Name Get
		if(enc_nor.state == AVIENC_IDLE)
		{
			//enc_nor.pf_get_file_name(&enc_nor);
			//muxer_avienc_deletefile(&enc_nor);
//#ifdef __JPEG__
//			jpeg_thumbnail_save(&enc_nor);
//#endif
		}
		muxer_avienc_process(&enc_nor);		// 주행(상시)녹화 시작
	}
	else
	{
		if(enc_nor.state == AVIENC_EXIT && enc_evt.state == AVIENC_EXIT && enc_mar.state == AVIENC_EXIT)
		{	// close 할 것도 없음.
			muxer_avienc_delete();
			bSD_FAT_mounted = 0;
			gtUser.bSdAviSave = SDSAVE_USE_OFF;
			g_normalgo = 0;
			g_eventgo = 0;
			g_manualgo = 0;
			//gptMsgShare.EVENTstate = 0;
			stt_avienc = NULL;
			vTaskDelete(NULL);
		}
	}
#endif

done:
	LEAVE();
}

/**
	register irq and init sbuf for buffering
	this function should be called first before h264 working in cpu1 
*/
void muxer_init_buffering(void)
{
	ENTER();

	avienc_info_init(&enc_nor, AVI_NORMAL_SW, SAVE_NOR_DIR, SAVE_NOR_NAME_FORMAT);	
	avienc_info_init(&enc_evt, AVI_EVENT_SW, SAVE_EVT_DIR, SAVE_EVT_NAME_FORMAT);
	avienc_info_init(&enc_mar, AVI_MANUAL_SW, SAVE_MAN_DIR, SAVE_MAN_NAME_FORMAT);
	avienc_info_init(&enc_play, AVI_NORMAL_SW, SAVE_NOR_DIR, SAVE_NOR_NAME_FORMAT);
	

	avienc_info_sbuf_init(&enc_nor);
	avienc_info_sbuf_init(&enc_evt);
	avienc_info_sbuf_init(&enc_mar);
	avienc_info_sbuf_init(&enc_play);
	

#ifdef __CBB__
	gssSem = xSemaphoreCreateCounting(NUM_STREAM_ACC, 0);
#endif
	bufSem = xSemaphoreCreateCounting(NUM_STREAM_H264, 0);
	//IntAdd(IRQ_SYS1, (void*)sbuf_irq_handler, 0);
    //Ei(IRQ_SYS1);

	int i = 0;
	for(i=0;i<sizeof(baudio_g);i++)
		baudio_g[i] = 0xFF;

	LEAVE();
}

#include "wifi.h"
void muxer_avienc_task(void* pvParameters)
{
	dprintf("\nmuxer_avienc_task\n");
/* 
	we first init sd card driver 
*/
#ifdef __JPEG__
	gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;

	while(1)
	{
		if(gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_END && gptMsgShare.JPEG_STILL_SIZE > 0)
			break;
		
		vTaskDelay(1);
		//printf(" J ");
	}
	printf("\nJpeg ready!\n");
	
//	while(gtWifiAPcfg.un32State != WLIF_UAP_ACTIVE)
//		vTaskDelay(1);	
//	printf("\nwifi ready!\n");
#endif
	muxer_avienc_init();
	//dprintf("Start Recording Sd-Card.\n");
	
	for(;;)
	{		//printf("t");
		if(xSemaphoreTake(bufSem, portMAX_DELAY) == pdTRUE)
		{	//printf("g");
			if(muxer_avienc_sbuf_depth_sum())
			{
				//printf("\nmuxer_avienc\n");
				muxer_avienc();		
				//vTaskDelay(1);
			}
			else
			{
				vTaskDelay(1);
			}
		}
		else
		{
				vTaskDelay(1);
		}
	}
}
#ifdef __CBB__
int PutAviData(UINT addr, UINT size, UINT type)
{
	while(h264_mb.ready == 1)
	{
		vTaskDelay(1);
		if(h264_mb.playbackgo==0)
		{
			return 0;
		}
		if(h264_mb.pause==1)
		{
			return 0;
		}

	}


	if(h264_mb.ready == 0)
	{
		//printf(" type %d ",type);
		h264_mb.type = type;
		h264_mb.addr = addr;
		h264_mb.size = size;
		
		h264_mb.ready = 1;
		h264_mb.update = 0;
		//vTaskDelay(1);
		vTaskDelay(1);
	}
	//vTaskDelay(1);
	return 0;
}

int PutAviDataIdx(t_avienc_t *pavi, UINT addr, UINT size, UINT type)
{
	while(h264_mb.ready == 1)
	{//UartPutByte('1');
		vTaskDelay(1);
		if(h264_mb.playbackgo==0)
		{
			return 0;
		}
		if(h264_mb.pause==1)
		{
			return 0;
		}
		if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
		{
			return 0;
		}
	}

	if(h264_mb.ready == 0)
	{
		//printf(" type %d ",type);
		h264_mb.type = type;
		h264_mb.addr = addr;
		h264_mb.size = size;
		
		h264_mb.ready = 1;
		h264_mb.update = 0;
		vTaskDelay(1);
	}
	//vTaskDelay(1);
	return 0;
}


int PutAviDataAudIdx(t_avienc_t *pavi, UINT addr, UINT size, UINT type)
{//UartPutByte('A');
	while(g711_mb.ready == 1)
	{//UartPutByte('2');
		vTaskDelay(1);
		if(h264_mb.playbackgo==0)
		{
			//UartPutByte('a');
			return 0;
		}
		if(h264_mb.pause==1)
		{
			//UartPutByte('a');
			return 0;
		}
		if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
		{
			return 0;
		}
	}

	if(g711_mb.ready == 0)
	{//vTaskDelay(5);
		//printf(" type %d ",type);
		g711_mb.type = type;
		g711_mb.addr = addr;
		g711_mb.size = size;
		
		g711_mb.ready = 1;
		g711_mb.update = 0;
		//vTaskDelay(1);//ksj_151228_stream 
	}
	//vTaskDelay(1);//ksj_151228_stream 
	return 0;
}

int PutAviDataTextIdx(t_avienc_t *pavi, UINT addr, UINT size, UINT type)
{//UartPutByte('A');
	while(txts_mb.ready == 1)
	{//UartPutByte('3');
		vTaskDelay(1);
		if(h264_mb.playbackgo==0)
		{
			return 0;
		}
		if(h264_mb.pause==1)
		{
			return 0;
		}
		if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
		{
			return 0;
		}

	}

	if(txts_mb.ready == 0)
	{//vTaskDelay(5);
		txts_mb.addr = addr;
		txts_mb.size = size;
		
		txts_mb.ready = 1;
		txts_mb.update = 0;
		//vTaskDelay(1);
	}
	//vTaskDelay(1);
	return 0;
}

static int framenum = 0;
UINT muxer_aviplayback_idle(t_avienc_t *pavi)
{
	pavi->state = AVIENC_OPEN;
	pavi->datablock.state = 1;	// datablock에 데이터 존재(video)
	muxer_aviplayback_open(pavi);
	return 1;
}


void muxer_aviplayback_error(t_avienc_t *pavi, AVI_ERROR state)
{
	ENTER();

	//bitchang(is_error, CBB_ERROR_SDWRITE, 1);

	f_close(&pavi->fpavi);

	if(state == AVI_FULL)
	{
		pavi->state = AVIENC_IDLE;
	}
	else
	{
#if 0 //#ifdef __CBB__
		if(cbbpower_get_mode() != eCBB_Off)
		{	// 다른 음성으로 변경해야 한다. / 리부팅?
			G726Playlist_put(eG726_REBOOT_SYSERROR);
			vTaskDelay(300);

			printf("%s(%d) : WDT GO\n", __func__, __LINE__);
			WdtSystemReset();

			while(1);
		}
#endif
		pavi->state = AVIENC_EXIT;
		gtUser.bSdAviSave = SDSAVE_USE_OFF;
		//muxer_avienc_delete();
		bSD_FAT_mounted = 0;
		//stt_avidec = NULL;
		LEAVE();
		//vTaskDelete(NULL);
	}

	LEAVE();
}

void muxer_aviplayback_open(t_avienc_t *pavi)
{
	ENTER();
	
	FRESULT fres;
	t_avienc_t *enc_t = (t_avienc_t *)pavi;
	char strBuf[_MAX_LFN+1] = {0};

	// File Name Get
	//pavi->pf_get_file_name(pavi);
	//strcpy(pavi->now_name, TEST_PLAYBACK_FILE_NAME);
	//encHeap_oldQueue(enc_t->filenamelist, strfirstBuf);
	//sprintf(strDelBuf, "%s/%s", enc_t->dir_name, strBuf);

#ifdef __DEVICE_SD__
		char strMMCdir[8];
		sprintf(strMMCdir, "%d:", MMC);		
		f_chdrive(strMMCdir);		
#endif
#if 0 //neek
		if(f_chdir("NORMAL") == FR_OK)
		{
			char strGetcwd[256] = {0};
			f_getcwd(strGetcwd, 256);
			dprintf("%s", strGetcwd);
		}

		char strGetcwd[256] = {0};
		f_getcwd(strGetcwd, 256);
		pavi->playback.fileidx = h264_mb.index;
		//printf("\n cwd  %s h264_mb.index %d\n",strGetcwd,h264_mb.index);
		fat_old_file(strGetcwd, h264_mb.index, pavi->now_name);
		dprintf("\n");
#else
		if(f_chdir(h264_mb.dir_name) == FR_OK)
		{
			char strGetcwd[256] = {0};
			f_getcwd(strGetcwd, 256);
			dprintf("%s", strGetcwd);
		}

		char strGetcwd[256] = {0};
		f_getcwd(strGetcwd, 256);
		sprintf(pavi->dir_name, "%d:/%s", MMC, h264_mb.dir_name);
		strcpy(pavi->now_name, h264_mb.file_format);
		//printf("\n");
#endif


	// File Path Create
	sprintf(strBuf, "%s/%s", pavi->dir_name, pavi->now_name);
	dprintf("\nstrBuf %s\n",strBuf);
	// Start Time Get
	pavi->stime = xTaskGetTickCount();

	// File Open
	if((fres=f_open(&pavi->fpavi, strBuf, FA_READ)) == FR_OK)
	{
		
		UINT stime_end = xTaskGetTickCount();
		//dprintf("f_open(%.2f) : %s\n", (stime_end - pavi->stime)/100.0, strBuf);

		///dprintf("f_size %d \n",pavi->fpavi.fsize);
		pavi->state = AVIENC_READ;
	}
	else
	{
		dprintf("%s Error : f_open AVI data File system fail : %s\n", __func__, strBuf);
		dprintf("Err Msg : %s\n", put_rc(fres));
		muxer_aviplayback_error(pavi, AVI_OPENFAIL);
		h264_mb.GetPlayTimeLock = 0;
		goto done;
	}

done:
	LEAVE();
}

void muxer_aviplayback_read_list(t_avienc_t *pavi)
{
	ENTER();
	int i;
	int fres;
	UINT readsize;
	UINT status;
	UINT wtime_s = xTaskGetTickCount();

	int totalframes=0;
	int idxsize=0;

	pavi->playback.idxdata = NULL;
	pavi->playback.v.vdata = NULL;
	pavi->playback.a.adata = NULL;
	data_file = NULL;
	
	f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.ListHdrl, sizeof(AviHdrl_t), &readsize);

	if(readsize>0)
	{
		//dprintf("\n Playback file info readsz %d \n",readsize);

		if(RIFF_ID != pavi->playback.ListHdrl.hdrl.head.fcccb.fcc)
		{
			dprintf("RIFF id err\n");
			goto Err;
		}
		
		//printf("Group size %d \n",NUM32(pavi->playback.ListHdrl.hdrl.head.fcccb.cb));

		if(AVI_TYPE != pavi->playback.ListHdrl.hdrl.head.fccListType)
		{
			dprintf("AVI type err\n");
			goto Err;
		}
			

		if(LIST_ID != pavi->playback.ListHdrl.hdrl.list.fcccb.fcc)
		{
			dprintf("List id err\n");
			goto Err;
		}
		//printf("Group size %d \n",NUM32(pavi->playback.ListHdrl.hdrl.list.fcccb.cb));

		if(HDRL_TYPE != pavi->playback.ListHdrl.hdrl.list.fccListType)
		{
			dprintf("HDRL type err\n");
			goto Err;
		}


		
		if(AVIH_ID != pavi->playback.ListHdrl.hdrl.data.fcccb.fcc)
		{
			dprintf("AVIH ID err\n");
			goto Err;
		}		
		//printf("Chunk size %d \n",NUM32(pavi->playback.ListHdrl.hdrl.data.fcccb.cb));	
		
		//dprintf("Micro sec per Frame %d \n",NUM32(pavi->playback.ListHdrl.hdrl.data.mhData.dwMicroSecPerFrame));		
		
		//printf("Max Transfer Rate %d \n",NUM32(pavi->playback.ListHdrl.hdrl.data.mhData.dwMaxBytesPerSec));		
		
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwPaddingGranularity;	
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwFlags;				
		
		totalframes = NUM32(pavi->playback.ListHdrl.hdrl.data.mhData.dwTotalFrames);
		dprintf("Total Frames %d \n",totalframes);		
		h264_mb.endrange = totalframes/30;
		h264_mb.GetPlayTimeLock = 0;

		//pavi->playback.ListHdrl.hdrl.data.mhData.dwInitialFrames;		
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwStreams;				
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwSuggestedBufferSize;	

		//dprintf("Width %d \n",NUM32(pavi->playback.ListHdrl.hdrl.data.mhData.dwWidth));				

		//dprintf("Height %d \n",NUM32(pavi->playback.ListHdrl.hdrl.data.mhData.dwHeight));				
		
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwReserved[0];
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwReserved[1];
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwReserved[2];
		//pavi->playback.ListHdrl.hdrl.data.mhData.dwReserved[3];


		if(LIST_ID!=pavi->playback.ListHdrl.strlVideo.list.fcccb.fcc)
		{
			dprintf("List ID err\n");
			goto Err;
		}			
		//dprintf("Group size %d\n",NUM32(pavi->playback.ListHdrl.strlVideo.list.fcccb.cb));
		
		if(STRL_TYPE != pavi->playback.ListHdrl.strlVideo.list.fccListType)
		{
			dprintf("STRL Type err\n");
			goto Err;
		}	

		
		if(STRH_ID != pavi->playback.ListHdrl.strlVideo.stream.fcccb.fcc)
		{
			dprintf("STRH ID err\n");
			goto Err;
		}

		//printf("Chunk size %d\n",NUM32(pavi->playback.ListHdrl.strlVideo.stream.fcccb.cb));
		

		if(VIDS_TYPE != pavi->playback.ListHdrl.strlVideo.stream.shData.fccType)
		{
			dprintf("Video Type err\n");
			goto Err;
		}		
		
		if(H264_CODEC != pavi->playback.ListHdrl.strlVideo.stream.shData.fccHandler)
		{
			dprintf("Codec Type h264 err\n");
			goto Err;
		}	
		
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwFlags;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.wPriority;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.wLanguage;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwInitialFrames;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwScale;
		
		pavi->playback.fps = NUM32(pavi->playback.ListHdrl.strlVideo.stream.shData.dwRate);
		//dprintf("fps %d \n", pavi->playback.fps);
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwStart;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwLength;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwSuggestedBufferSize;
		//pavi->playback.ListHdrl.strlVideo.stream.shData.dwQuality;
		//printf("sample size %d\n",NUM32(pavi->playback.ListHdrl.strlVideo.stream.shData.dwSampleSize));


		if(STRF_ID != pavi->playback.ListHdrl.strlVideo.info.fcccb.fcc)
		{
			dprintf("STRF ID err\n");
			goto Err;
		}

		//printf("Chunk size %d\n",NUM32(pavi->playback.ListHdrl.strlVideo.info.fcccb.cb));

		//pavi->playback.ListHdrl.strlVideo.info.bmData.biSize;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biWidth;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biHeight;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biPlanes;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biBitCount;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biCompression;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biSizeImage;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biXPelsPerMeter;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biYpelsPermeter;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biClrUsed;
		//pavi->playback.ListHdrl.strlVideo.info.bmData.biClrImportant;
		
/*
		
		pavi->playback.ListHdrl.strlAudio.list;
		pavi->playback.ListHdrl.strlAudio.stream;
		pavi->playback.ListHdrl.strlAudio.info;

		*/

		if(LIST_ID!=pavi->playback.ListHdrl.strlAudio.list.fcccb.fcc)
		{
			dprintf("List ID err\n");
			goto Err;
		}			
		//dprintf("Group size %d\n",NUM32(pavi->playback.ListHdrl.strlAudio.list.fcccb.cb));
		
		if(STRL_TYPE != pavi->playback.ListHdrl.strlAudio.list.fccListType)
		{
			dprintf("STRL Type err\n");
			goto Err;
		}	

		
		if(STRH_ID != pavi->playback.ListHdrl.strlAudio.stream.fcccb.fcc)
		{
			dprintf("STRH ID err\n");
			goto Err;
		}

		//printf("Chunk size %d\n",NUM32(pavi->playback.ListHdrl.strlAudio.stream.fcccb.cb));
		

		if(AUDS_TYPE != pavi->playback.ListHdrl.strlAudio.stream.shData.fccType)
		{
			dprintf("Video Type err\n");
			goto Err;
		}		
		
		if(G711_CODEC != pavi->playback.ListHdrl.strlAudio.stream.shData.fccHandler)
		{
			dprintf("Codec Type h264 err\n");
			goto Err;
		}	
		
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwFlags;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.wPriority;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.wLanguage;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwInitialFrames;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwScale;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwRate;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwStart;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwLength;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwSuggestedBufferSize;
		//pavi->playback.ListHdrl.strlAudio.stream.shData.dwQuality;
		//printf("sample size %d\n",NUM32(pavi->playback.ListHdrl.strlAudio.stream.shData.dwSampleSize));


		if(STRF_ID != pavi->playback.ListHdrl.strlAudio.info.fcccb.fcc)
		{
			dprintf("STRF ID err\n");
			goto Err;
		}

		//printf("Chunk size %d\n",NUM32(pavi->playback.ListHdrl.strlAudio.info.fcccb.cb));

		//pavi->playback.ListHdrl.strlAudio.info.bmData.biSize;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biWidth;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biHeight;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biPlanes;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biBitCount;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biCompression;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biSizeImage;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biXPelsPerMeter;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biYpelsPermeter;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biClrUsed;
		//pavi->playback.ListHdrl.strlAudio.info.bmData.biClrImportant;



	}

	//list movi
	f_lseek(&(pavi->fpavi),0xff4);
	f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.ListMovi, sizeof(AviMovi_t), &readsize);
	if(readsize>0)
	{
		int movisize = 0;
		if(LIST_ID != pavi->playback.ListMovi.movi.fcccb.fcc)
		{
			dprintf("LIST ID err\n");
			goto Err;
		}

		movisize = NUM32(pavi->playback.ListMovi.movi.fcccb.cb);
		//dprintf("List Movi Group size %d \n",movisize);
		
		if(MOVI_TYPE != pavi->playback.ListMovi.movi.fccListType)
		{
			dprintf("Movi Type err\n");
			goto Err;							
		}
		
		
		//idx1
		UINT seeksize = 0xffc;
		seeksize += movisize;

		//dprintf("seeksize %d\n",seeksize);
		f_lseek(&(pavi->fpavi),seeksize);
		f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.ListIdx, sizeof(AviIdx_t), &readsize);
		if(readsize>0)
		{
			//dprintf("filepointer %x\n ",pavi->fpavi.fptr);
			//dprintf("readsize %d %x %x %x %x",readsize,pavi->playback.ListIdx.idxinfo.fcc&0xff000000,
			//	pavi->playback.ListIdx.idxinfo.fcc&0xff0000,
			//	pavi->playback.ListIdx.idxinfo.fcc&0xff00,
			//	pavi->playback.ListIdx.idxinfo.fcc&0xff);
			//
			if(IDX1_ID != pavi->playback.ListIdx.idxinfo.fcc)
			{
				dprintf("Idx Id err\n");
				goto Err;
			}
			
			idxsize = NUM32(pavi->playback.ListIdx.idxinfo.cb);
				
			//dprintf("Idx group size %d\n",idxsize);
			
/*
			if(totalframes != idxsize / 32)
			{
				if(totalframes != idxsize / 48)
				{
					printf("Idx info size err\n");
					goto Err;							
				}
		
			}
*/			
#ifndef __AVI_TXTS__
			pavi->playback.totalframes = totalframes;
#else
			pavi->playback.totalframes = totalframes*3;
#endif

			//idx info save 
			pavi->playback.idxdata = (BYTE*)H264_PLAYBACK_INDEX_BASE;//(AviIdxDatas_t*)pvPortMalloc((sizeof(AviIdxDatas_t)*2)*PLAYBACK_MAX_FRAME);
			/*if(pavi->playback.idxdata == NULL)
			{
				printf("idxdata Malloc failed\n");
				goto Err;
			}*/
			
			f_read(&(pavi->fpavi), (uint8 *)pavi->playback.idxdata, idxsize, &readsize);
			if(readsize>0)
			{
				dprintf("AVI FILE idxdata read SUCCESS! readsize %d\n",readsize);
				
				pavi->playback.v.vdata = (BYTE*)H264_PLAYBACK_BASE;
				pavi->playback.v.prereadsize = 0;
				pavi->playback.v.readsize = 0;
				pavi->playback.a.adata = (BYTE *)(H264_PLAYBACK_AUDIO_BASE);
				pavi->playback.a.prereadsize = 0;
				pavi->playback.a.readsize = 0;
				pavi->state = AVIENC_READ_IDX;
				pavi->playback.index = 0;
				dprintf("end range %d \n",h264_mb.endrange);
			}
			else
			{
				data_file = (BYTE*)H264_PLAYBACK_BASE;//H264_STRM_BASE;
				f_lseek(&(pavi->fpavi),0xff4);
				pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
				fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 12, &readsize);
				pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

				
				for(i=0;i<12;i++)
					pbprintf("%x",(uint8)data_file[i]);
				pbprintf(" ");
				pbprintf("fres %d readsize %d\n ",fres, readsize);


				
				pavi->state = AVIENC_READ2;
			}
			
		}			
		else
		{
			data_file = (BYTE*)H264_PLAYBACK_BASE;//H264_STRM_BASE;
			f_lseek(&(pavi->fpavi),0xff4);
			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 12, &readsize);
			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

			
			for(i=0;i<12;i++)
				pbprintf("%x",(uint8)data_file[i]);
			pbprintf(" ");
			pbprintf("fres %d readsize %d\n ",fres, readsize);


			
			pavi->state = AVIENC_READ2;
		}
	}

	
	LEAVE();
	return 0;

Err:
/*	if(pavi->playback.idxdata != NULL)
	{
		vPortFree(pavi->playback.idxdata);
		pavi->playback.idxdata = NULL;
	}*/
	pavi->state = AVIENC_EXIT;
	LEAVE();		
		return 0;
}


void muxer_aviplayback_read_first(t_avienc_t *pavi)
{
	ENTER();
	int i;
	int fres;
	UINT readsize;
	UINT status;
	UINT wtime_s = xTaskGetTickCount();

	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
	while(data_file[0] == 0x64 || data_file[0] == 0x63 )
	{
		if(data_file[0] == 0x63)
		{
			data_file[7] = data_file[4];
			data_file[6] = data_file[3];
			data_file[5] = data_file[2];			
			data_file[4] = data_file[1];	
			break;
		}
		
		pavi->fpavi.fptr -= 7;
		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
		
		for(i=0; i<8; i++)
			pbprintf("%x",(uint8)data_file[i]);
		pbprintf(" ");
	}
	
	pbprintf(" ");
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	for(i=0; i<8; i++)
		pbprintf("%x",(uint8)data_file[i]);
	pbprintf(" ");

	pbprintf("\nfrmNO %d ",++framenum);


	UINT lsize;

	BYTE temp[4];
	temp[0] = data_file[7];
	temp[1] = data_file[6];
	temp[2] = data_file[5];
	temp[3] = data_file[4];
	
	memcpy(&lsize,(UINT*)temp,4);
	//swap_byte_32(lsize);
	pbprintf("lsize %d",lsize);
	pbprintf(" ");
	
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

	if(lsize%2)
		lsize+1;
	pbprintf("[[lsize//2 %d lsize %d]]",lsize%2,lsize);
	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	pbprintf("read  size %d \n",lsize);
	
	//for(i=0; i<readsize; i++)
	//	printf("%x",(uint8)data_file[i]);

	pbprintf("fres %d readsize %d\n ",fres, readsize);
	
	if(data_file[readsize-1] == 0xFF)
		readsize= readsize-1;
	PutAviData(data_file, readsize, 1);

	
	pavi->state = AVIENC_READ3;

	
	return 0;

done:
	LEAVE();
}

void muxer_aviplayback_read_keep(t_avienc_t *pavi)
{
	ENTER();
	int i;
	int fres;
	UINT readsize;
	UINT status;
	UINT wtime_s = xTaskGetTickCount();
	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
	pbprintf(" ");
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	while(data_file[0] == 0x64 || data_file[0] == 0x63 )
	{
		pavi->fpavi.fptr -= 8;
		return 0;
	}
	for(i=0; i<8; i++)
		pbprintf("%x",(uint8)data_file[i]);
	
	pbprintf(" ");
	
	while(data_file[1] == 0x30 && data_file[2] == 0x31 )
	{
		data_file[4] = data_file[5];
		data_file[5] = data_file[6];
		data_file[6] = data_file[7];
		char *temp;
		fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
		
		data_file[7] = *temp;
		break;
	}
	while(data_file[0] == 0x62 || data_file[0] == 0x77 )
	{
		if(data_file[0] == 0x62)
		{
			data_file[0] = data_file[1];
			data_file[1] = data_file[2];
			data_file[2] = data_file[3];
			char *temp;
			fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
			
			data_file[3] = *temp;
			break;
		}
		
		pavi->fpavi.fptr -= 3;
		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 4, &readsize);
		
		for(i=0; i<4; i++)
			pbprintf("%x",(uint8)data_file[i]);
		pbprintf(" ");
	}


	UINT lsize;
	
	BYTE temp[4];
	temp[0] = data_file[7];
	temp[1] = data_file[6];
	temp[2] = data_file[5];
	temp[3] = data_file[4];
	
	memcpy(&lsize,(UINT*)temp,4);
	//swap_byte_32(lsize);
	pbprintf("lsize %d",lsize);
	pbprintf(" ");
	
	if(lsize%2)
		lsize+1;

	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
	while(data_file[0] == 0xFF)
	{		
		if(data_file[1] == 0x30 && data_file[2] == 0x30)
		{
			data_file[4] = data_file[5];
			data_file[5] = data_file[6];
			data_file[6] = data_file[7];
			char *temp;
			fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
			
			data_file[7] = *temp;
			break;
		}
		break;
	}

	pbprintf("\nfrmNO %d ",++framenum);
	pbprintf(" ");
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	for(i=0; i<8; i++)
		pbprintf("%x",(uint8)data_file[i]);
	pbprintf(" ");
	
	temp[0] = data_file[7];
	temp[1] = data_file[6];
	temp[2] = data_file[5];
	temp[3] = data_file[4];
	
	memcpy(&lsize,(UINT*)temp,4);
	//swap_byte_32(lsize);
	pbprintf("lsize %d",lsize);
	pbprintf(" ");
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	if(lsize%2)
		lsize+1;

	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
	pbprintf("read  size %d\n",readsize);
	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
	//for(i=0; i<readsize; i++)
	//	pbprintf("%x",(uint8)data_file[i]);

	pbprintf("fres %d readsize %d\n ",fres, readsize);
	
	if(data_file[readsize-1] == 0xFF)
		readsize= readsize-1;
	
	PutAviData(data_file, readsize, 0);



	
	pavi->state = AVIENC_READ;

	return 0;

done:
	LEAVE();
}

void muxer_aviplayback_read_keep2(t_avienc_t *pavi)
{
	int NoAudio = 0;
//UartPutByte('k');
	ENTER();
	int i;
	int fres;
	UINT readsize;
	UINT status;

//	UINT wtime_s = xTaskGetTickCount();
	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}

//	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 1, &readsize);
//	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

//	if(h264_mb.range > 0)
//	{
//		pbprintf("\n 0x12867c6 \n");
//		f_lseek(&(pavi->fpavi),0x12867c6);
//		h264_mb.range = 0;
//	}
	
	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
//	for(i=0; i<8; i++)
//		pbprintf("%x",(uint8)data_file[i]);
//	pbprintf("\n start \n");
	
	if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
	{
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	if(data_file[1] == 0x69 && data_file[2] == 0x64 && data_file[3] == 0x78 && data_file[4] == 0x31)
	{
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	
	if(data_file[0] == 0x63)
	{				
		char *tempc;
		data_file[0] = data_file[1];
		data_file[1] = data_file[2];
		data_file[2] = data_file[3];
		f_read(&(pavi->fpavi), (uint8 *)tempc, 1, &readsize);
		data_file[3] = *tempc;
					
//		pbprintf(" ");
//		pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
		pavi->fpavi.fptr += 4;
				
//		pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
//		pbprintf(" ");
		

		UINT lsize;
		BYTE temp[4];
		temp[0] = data_file[3];
		temp[1] = data_file[2];
		temp[2] = data_file[1];
		temp[3] = data_file[0];
		
		memcpy(&lsize,(UINT*)temp,4);
		//swap_byte_32(lsize);
//		pbprintf("lsize %d",lsize);
//		pbprintf(" ");
//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
//		pbprintf("\nfrmNO %d ",++framenum);
		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
//		pbprintf("read  size %d \n",readsize);
		
//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

		PutAviData(data_file, readsize, (data_file[0]==0x67));
		
	}
	else{
		if(data_file[0] == 0x30 && data_file[1] == 0x30)
		{
			NoAudio = 1;
		}
			
		while(data_file[0] == 0xFF)
		{		
			if(data_file[1] == 0x30 && data_file[2] == 0x30)
			{
				data_file[4] = data_file[5];
				data_file[5] = data_file[6];
				data_file[6] = data_file[7];
				char *temp;
				fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
				
				data_file[7] = *temp;
				NoAudio = 1;
				break;
			}
			break;
		}
		while(data_file[1] == 0x30 && data_file[2] == 0x31 )
		{
			data_file[4] = data_file[5];
			data_file[5] = data_file[6];
			data_file[6] = data_file[7];
			char *temp;
			fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
			
			data_file[7] = *temp;
			break;
		}
		while(data_file[0] == 0x62 || data_file[0] == 0x77 )
		{
			if(data_file[0] == 0x62)
			{
				data_file[0] = data_file[1];
				data_file[1] = data_file[2];
				data_file[2] = data_file[3];
				char *temp;
				fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
				
				data_file[3] = *temp;
				break;
			}
			
			pavi->fpavi.fptr -= 3;
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 4, &readsize);
			
//			for(i=0; i<4; i++)
//				pbprintf("%x",(uint8)data_file[i]);
//			pbprintf(" ");
		}

		
//		pbprintf(" ");
//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
//		for(i=0; i<8; i++)
//			pbprintf("%x",(uint8)data_file[i]);
//		pbprintf(" ");


		UINT lsize;
		
		BYTE temp[4];
		
		if(NoAudio == 0)
		{
			temp[0] = data_file[7];
			temp[1] = data_file[6];
			temp[2] = data_file[5];
			temp[3] = data_file[4];
			
			memcpy(&lsize,(UINT*)temp,4);
			//swap_byte_32(lsize);
//			pbprintf("lsize %d",lsize);
//			pbprintf(" ");
//			pbprintf("\nfrmNO %d ",++framenum);

//			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
			if(lsize%2)
				lsize+1;
			
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
			
//			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
//			for(i=0; i<8; i++)
//				pbprintf("%x",(uint8)data_file[i]);		
			while(data_file[0] == 0xFF)
			{		
				if(data_file[1] == 0x30 && data_file[2] == 0x30)
				{
					data_file[4] = data_file[5];
					data_file[5] = data_file[6];
					data_file[6] = data_file[7];
					char *temp;
					fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
					
					data_file[7] = *temp;
					break;
				}
//				pbprintf(" ");
//				pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
				
				pavi->fpavi.fptr -= 7;
				while((fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize))!=FR_OK)
				{
//					pbprintf("\nfres%d\n",fres);
//					for(i=0; i<8; i++)
//						pbprintf("%x",(uint8)data_file[i]);
				}
				
//				for(i=0; i<8; i++)
//					pbprintf("%x",(uint8)data_file[i]);
				
//				pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
//				pbprintf(" ");
				
				if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
				{
					pavi->state = AVIENC_EXIT;
					return 0;
				}
			}
		}
		if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
		{
			pavi->state = AVIENC_EXIT;
			return 0;
		}
		if(data_file[1] == 0x69 && data_file[2] == 0x64 && data_file[3] == 0x78 && data_file[4] == 0x31)
		{
			pavi->state = AVIENC_EXIT;
			return 0;
		}
		
//		pbprintf(" ");
//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
//		for(i=0; i<8; i++)
//			pbprintf("%x",(uint8)data_file[i]);
//		pbprintf(" ");
		
		temp[0] = data_file[7];
		temp[1] = data_file[6];
		temp[2] = data_file[5];
		temp[3] = data_file[4];
		
		memcpy(&lsize,(UINT*)temp,4);
		//swap_byte_32(lsize);
//		pbprintf("lsize %d",lsize);
//		pbprintf(" ");
//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
		if(lsize%2)
			lsize+1;

		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
//		pbprintf("read  size %d \n",readsize);

//		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
		if(h264_mb.pause==1)
		{
			//UartPutByte('a');
			pavi->state = AVIENC_PAUSE;
			return 0;
		}
		
		if(h264_mb.playbackgo==0)
		{
			//UartPutByte('a');
			pavi->state = AVIENC_EXIT;
			return 0;
		}
		
		if(data_file[readsize-1] == 0xFF)
				readsize= readsize-1;
		PutAviData(data_file, readsize, (data_file[4]==0x67));
		
	}
	return 0;

done:
	LEAVE();
}


int muxer_aviplayback_read_keep3(t_avienc_t *pavi)
{
	ENTER();
	int NoAudio = 0;
	int i;
	int fres;
	UINT readsize;
	UINT status;
	UINT wtime_s = xTaskGetTickCount();

	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}


//	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 1, &readsize);
//	pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

	fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
	for(i=0; i<8; i++)
		pbprintf("%x",(uint8)data_file[i]);
	
	if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
	{
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	if(data_file[1] == 0x69 && data_file[2] == 0x64 && data_file[3] == 0x78 && data_file[4] == 0x31)
	{
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	if(data_file[0] == 0x63)
	{				
		char *tempc;
		data_file[0] = data_file[1];
		data_file[1] = data_file[2];
		data_file[2] = data_file[3];
		f_read(&(pavi->fpavi), (uint8 *)tempc, 1, &readsize);
		data_file[3] = *tempc;
					
		pbprintf(" ");
		pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
		pavi->fpavi.fptr += 4;
				
		pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
		pbprintf(" ");
		

		UINT lsize;
		BYTE temp[4];
		temp[0] = data_file[3];
		temp[1] = data_file[2];
		temp[2] = data_file[1];
		temp[3] = data_file[0];
		
		memcpy(&lsize,(UINT*)temp,4);
		//swap_byte_32(lsize);
		pbprintf("lsize %d",lsize);
		pbprintf(" ");
		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		pbprintf("\nfrmNO %d ",++framenum);
		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
		pbprintf("read  size %d \n",readsize);
		
		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);

		PutAviData(data_file, readsize, (data_file[0]==0x67));
		
	}
	else{

		while(data_file[0] == 0xFF)
		{		
			if(data_file[1] == 0x30 && data_file[2] == 0x30)
			{
				data_file[4] = data_file[5];
				data_file[5] = data_file[6];
				data_file[6] = data_file[7];
				char *temp;
				fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
				
				data_file[7] = *temp;
				NoAudio = 1;
				break;
			}
			break;
		}
		
		while(data_file[1] == 0x30 || data_file[2] == 0x31 )
		{
			pavi->fpavi.fptr -= 7;
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
			
			for(i=0; i<8; i++)
			{
				pbprintf("%x",(uint8)data_file[i]);
			}
			pbprintf(" ");
		}
		
		while(data_file[0] == 0x62 || data_file[0] == 0x77 )
		{
			if(data_file[0] == 0x62)
			{
				data_file[0] = data_file[1];
				data_file[1] = data_file[2];
				data_file[2] = data_file[3];
				char *temp;
				fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
				
				data_file[3] = *temp;
				break;
			}
			
			pavi->fpavi.fptr -= 3;
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 4, &readsize);
			
			for(i=0; i<4; i++)
			{
				pbprintf("%x",(uint8)data_file[i]);
			}
			pbprintf(" ");
		}

		
		pbprintf(" ");
		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		for(i=0; i<8; i++)
		{
			pbprintf("%x",(uint8)data_file[i]);
		}
		pbprintf(" ");


		UINT lsize;
		
		BYTE temp[4];
		
		if(NoAudio == 0)
		{
			temp[0] = data_file[7];
			temp[1] = data_file[6];
			temp[2] = data_file[5];
			temp[3] = data_file[4];
			
			memcpy(&lsize,(UINT*)temp,4);
			//swap_byte_32(lsize);
			pbprintf("lsize %d",lsize);
			pbprintf(" ");
			pbprintf("\nfrmNO %d ",++framenum);

			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
			if(lsize%2)
				lsize+1;
			
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
			
			pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
			fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize);
			for(i=0; i<8; i++)
			{
				pbprintf("%x",(uint8)data_file[i]);		
			}
			while(data_file[0] == 0xFF)
			{		
				if(data_file[1] == 0x30 && data_file[2] == 0x30)
				{
					data_file[4] = data_file[5];
					data_file[5] = data_file[6];
					data_file[6] = data_file[7];
					char *temp;
					fres = f_read(&(pavi->fpavi), (uint8 *)temp, 1, &readsize);
					
					data_file[7] = *temp;
					break;
				}
				pbprintf(" ");
				pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
				
				pavi->fpavi.fptr -= 7;
				while((fres = f_read(&(pavi->fpavi), (uint8 *)data_file, 8, &readsize))!=FR_OK)
				{
					pbprintf("\nfres%d\n",fres);
					for(i=0; i<8; i++)
					{
						pbprintf("%x",(uint8)data_file[i]);
					}
				}
				
				for(i=0; i<8; i++)
				{
					pbprintf("%x",(uint8)data_file[i]);
				}
				
				pbprintf("\n pavi->fpavi.fptr %d\n",pavi->fpavi.fptr);
				pbprintf(" ");
				
				if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
				{
					pavi->state = AVIENC_EXIT;
					return 0;
				}
			}
		}
		if(data_file[0] == 0x69 && data_file[1] == 0x64 && data_file[2] == 0x78 && data_file[3] == 0x31)
		{
			pavi->state = AVIENC_EXIT;
			return 0;
		}
		if(data_file[1] == 0x69 && data_file[2] == 0x64 && data_file[3] == 0x78 && data_file[4] == 0x31)
		{
			pavi->state = AVIENC_EXIT;
			return 0;
		}
		
		pbprintf(" ");
		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		for(i=0; i<8; i++)
		{
			pbprintf("%x",(uint8)data_file[i]);
		}
		pbprintf(" ");
		
		temp[0] = data_file[7];
		temp[1] = data_file[6];
		temp[2] = data_file[5];
		temp[3] = data_file[4];
		
		memcpy(&lsize,(UINT*)temp,4);
		//swap_byte_32(lsize);
		pbprintf("lsize %d",lsize);
		pbprintf(" ");
		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
		if(lsize%2)
			lsize+1;

		fres = f_read(&(pavi->fpavi), (uint8 *)data_file, lsize, &readsize);
		pbprintf("read  size %d \n",readsize);

		pbprintf("[file pointer %d] ",pavi->fpavi.fptr);
		
		if(h264_mb.pause==1)
		{
			//UartPutByte('a');
			pavi->state = AVIENC_PAUSE;
			return 0;
		}
		
		if(h264_mb.playbackgo==0)
		{
			//UartPutByte('a');
			pavi->state = AVIENC_EXIT;
			return 0;
		}

		if(data_file[4]==0x67)
		{
			if(data_file[readsize-1] == 0xFF)
				readsize= readsize-1;
			
			PutAviData(data_file, readsize, (data_file[4]==0x67));
		}
		else
		{//UartPutByte('r');
			return -1;
		}
		
	}
	return 0;

done:
	LEAVE();
}

void muxer_aviplayback_read_idx(t_avienc_t *pavi, int idx)
{
	int i;
	if(h264_mb.range > 0)
	{
		pbprintf("\n h264_mb.range %d \n",h264_mb.range);
		pavi->playback.index = h264_mb.range*pavi->playback.fps*3;
		dprintf("\n pavi->playback.index  %d \n",pavi->playback.index);
		h264_mb.range = 0;
	}

	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	
	if(VIDEO_00dc_ID != pavi->playback.idxdata[idx].data.dwChunkId)
	{
		printf("VIDEO_00dc_ID check id err 1\n");
		return 0;
		//goto Err;
	}

	int fres;
	int readsize = 0;
	int sizecheck = 0;
	int offset = 0xffc + NUM32(pavi->playback.idxdata[idx].data.dwOffset);
	int readsz = NUM32(pavi->playback.idxdata[idx].data.dwSize);

	//printf("dwoffset %x %d offset %x %d",pavi->playback.idxdata[idx].data.dwOffset,pavi->playback.idxdata[idx].data.dwOffset, offset, offset);
	f_lseek(&(pavi->fpavi), offset);

	fres = f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.v.vinfo, 8, &readsize);
	//dprintf("read info fcc %x \n", pavi->playback.v.vinfo.fcc);
	//dprintf("read infor cb %x\n",pavi->playback.v.vinfo.cb);

	if(pavi->playback.v.readsize  > 0 && (pavi->playback.v.readsize + readsize) < (H264_VIDEO_SIZE))
		pavi->playback.v.vdata += pavi->playback.v.prereadsize;
	else
	{
		pavi->playback.v.readsize = 0;
		pavi->playback.v.vdata = (BYTE*)H264_PLAYBACK_BASE;//H264_STRM_BASE;
	}

	fres = f_read(&(pavi->fpavi), (uint8 *)pavi->playback.v.vdata, readsz, &readsize);
	
	if(VIDEO_00dc_ID != pavi->playback.v.vinfo.fcc)
	{
		dprintf("VIDEO_00dc_ID check id err 2\n");
		goto Err;
	}

	sizecheck = NUM32(pavi->playback.v.vinfo.cb);
	
	if(sizecheck != readsz)
	{
		dprintf("V size check err \n");
		goto Err;
	}
	
	if(readsize != sizecheck)
	{
		dprintf("size check read err \n");
		goto Err;
	}
	pavi->playback.v.prereadsize = readsize;
	pavi->playback.v.readsize += readsize;
	
	if(NUM32(pavi->playback.idxdata[idx].data.dwFlags)>0)
		pavi->playback.idxdata[idx].data.dwFlags = 1;
	
	//printf("f %d ",pavi->playback.idxdata[idx].videoidxdata.dwFlags);
	
	if(VIDEO_00dc_ID == pavi->playback.v.vinfo.fcc)
		PutAviDataIdx(pavi, pavi->playback.v.vdata, readsize, pavi->playback.idxdata[idx].data.dwFlags);
	else
	{
		dprintf("buffering add failed VIDEO_00dc_ID \n");
	}
	
	//pavi->playback.idxdata[idx].data;
	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	//printf("\n idx  %d \n",idx);
	return 0;

Err:
	pavi->state = AVIENC_EXIT;
	return 0;
}


void muxer_aviplayback_read_audidx(t_avienc_t *pavi, int idx)
{
	int i;
	if(h264_mb.range > 0)
	{
		pbprintf("\n h264_mb.range %d \n",h264_mb.range);
		pavi->playback.index = h264_mb.range*pavi->playback.fps*3;
		dprintf("\n pavi->playback.index  %d \n",pavi->playback.index);
		h264_mb.range = 0;
	}

	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	
	if(AUDIO_01wb_ID != pavi->playback.idxdata[idx].data.dwChunkId)
	{
		printf("AUDIO_01wb_ID check id err 3\n");
		return 0;
		//goto Err;
	}

	int fres;
	int readsize = 0;
	int sizecheck = 0;
	int offset = 0xffc + NUM32(pavi->playback.idxdata[idx].data.dwOffset);
	int readsz = NUM32(pavi->playback.idxdata[idx].data.dwSize);
	
	f_lseek(&(pavi->fpavi), offset);

	fres = f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.a.ainfo, 8, &readsize);

	if(pavi->playback.a.readsize  > 0 && (pavi->playback.a.readsize+readsize)  < H264_PLAYBACK_AUDIO_SIZE)
		pavi->playback.a.adata += pavi->playback.a.prereadsize;
	else
	{
		pavi->playback.a.readsize = 0;
		pavi->playback.a.adata = (BYTE*)(H264_PLAYBACK_AUDIO_BASE);//H264_STRM_BASE;
	}

	fres = f_read(&(pavi->fpavi), (uint8 *)pavi->playback.a.adata, readsz, &readsize);
	
	if(AUDIO_01wb_ID != pavi->playback.a.ainfo.fcc)
	{
		dprintf("AUDIO_01wb_ID check id err 4\n");
		goto Err;
	}

	sizecheck = NUM32(pavi->playback.a.ainfo.cb);
	
	//printf("sizecheck %d\n",sizecheck);
	
	if(sizecheck != readsz)
	{
		dprintf("A size check err \n");
		goto Err;
	}
	
	if(readsize != sizecheck)
	{
		dprintf("size check read err \n");
		goto Err;
	}
	pavi->playback.a.prereadsize = readsize;
	pavi->playback.a.readsize += readsize;
	
	if(NUM32(pavi->playback.idxdata[idx].data.dwFlags)>0)
		pavi->playback.idxdata[idx].data.dwFlags = 1;
	
	//printf("f %d ",pavi->playback.idxdata[idx].videoidxdata.dwFlags);
	
	if(AUDIO_01wb_ID == pavi->playback.a.ainfo.fcc)
		PutAviDataAudIdx(pavi, pavi->playback.a.adata, readsize, pavi->playback.idxdata[idx].data.dwFlags);
	else
	{
		dprintf("buffering add failed audio AUDIO_01wb_ID\n");
	}
	//pavi->playback.idxdata[idx].data;
	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		dprintf("playbackgo 0 AVIDEC_EXIT call ");
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	//dprintf("\n idx  %d \n",idx);
	return 0;

Err:
	dprintf("err AVIDEC_EXIT call ");
	pavi->state = AVIENC_EXIT;
	return 0;
}

void muxer_aviplayback_read_gpsidx(t_avienc_t *pavi, int idx)
{
	int i;
	if(h264_mb.range > 0)
	{
		pbprintf("\n h264_mb.range %d \n",h264_mb.range);
		pavi->playback.index = h264_mb.range*pavi->playback.fps*3;
		dprintf("\n pavi->playback.index  %d \n",pavi->playback.index);
		h264_mb.range = 0;
	}

	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		dprintf("playbackgo 0 AVIDEC_EXIT call ");
		pavi->state = AVIENC_EXIT;
		return 0;
	}
	
	if(TEXT_02tx_ID != pavi->playback.idxdata[idx].data.dwChunkId)
	{
		dprintf("TEXT_02tx_ID check id err %x\n",pavi->playback.idxdata[idx].data.dwChunkId);
		goto Err;
	}

	int fres;
	int readsize = 0;
	int sizecheck = 0;
	int offset = 0xffc + NUM32(pavi->playback.idxdata[idx].data.dwOffset);
	int readsz = NUM32(pavi->playback.idxdata[idx].data.dwSize);
	
	f_lseek(&(pavi->fpavi), offset);

	fres = f_read(&(pavi->fpavi), (uint8 *)&pavi->playback.t.tinfo, 8, &readsize);

	if(pavi->playback.t.readsize  > 0 && (pavi->playback.t.readsize+readsize) < (H264_PLAYBACK_TEXT_SIZE))
		pavi->playback.t.tdata += pavi->playback.t.prereadsize;
	else
	{
		pavi->playback.t.readsize = 0;
		pavi->playback.t.tdata = (BYTE*)(H264_PLAYBACK_TEXT_BASE);//H264_STRM_BASE;
	}

	fres = f_read(&(pavi->fpavi), (uint8 *)pavi->playback.t.tdata, readsz, &readsize);
	
	if(TEXT_02tx_ID != pavi->playback.t.tinfo.fcc)
	{
		dprintf("TEXT_02tx_ID check id err 5\n");
		goto Err;
	}

	sizecheck = NUM32(pavi->playback.t.tinfo.cb);
	
	//dprintf("sizecheck %d\n",sizecheck);
	
	if(sizecheck != readsz)
	{
		dprintf("G size check err \n");
		goto Err;
	}
	
	if(readsize != sizecheck)
	{
		dprintf("size check read err \n");
		goto Err;
	}
	pavi->playback.t.prereadsize = readsize;
	pavi->playback.t.readsize += readsize;
	
	if(NUM32(pavi->playback.idxdata[idx].data.dwFlags)>0)
		pavi->playback.idxdata[idx].data.dwFlags = 1;
	
	//dprintf("f %d ",pavi->playback.idxdata[idx].data.dwFlags);
	
	if(TEXT_02tx_ID == pavi->playback.t.tinfo.fcc)
		PutAviDataTextIdx(pavi, pavi->playback.t.tdata, readsize, pavi->playback.idxdata[idx].data.dwFlags);
	else
	{
		dprintf("buffering add failed audio TEXT_02tx_ID\n");
	}
	//pavi->playback.idxdata[idx].data;
	if(h264_mb.pause==1)
	{
		//UartPutByte('a');
		pavi->state = AVIENC_PAUSE;
		return 0;
	}
	
	if(h264_mb.playbackgo==0)
	{
		dprintf("playbackgo 0 AVIDEC_EXIT call ");
		pavi->state = AVIENC_EXIT;
		return 0;
	}

	//dprintf("\n idx  %d \n",idx);
	return 0;

Err:
	pavi->state = AVIENC_EXIT;
	return 0;
}

void muxer_aviplayback_close(t_avienc_t *pavi)
{
	ENTER();

	FRESULT fres;

	pavi->aviinfo.i_movi_end = pavi->filesize;


	// File Close
	UINT etime_s = xTaskGetTickCount();
	if((fres=f_close(&pavi->fpavi)) != FR_OK)
	{
		dprintf("%s Error : f_close File system fail\n", __func__);
		dprintf("Err Msg : %s\n", put_rc(fres));
	}
	else
	{
		// End Time Get
		pavi->etime = xTaskGetTickCount();
		float tot_time = (pavi->etime - pavi->stime)/100.0;
		dprintf("%s : total size(%d) time(%4.2f) bps(%.3fKBps) f_close(%.2f)\n", pavi->now_name, pavi->aviinfo.i_riff, tot_time, pavi->aviinfo.i_riff/tot_time/1024*8, (pavi->etime - etime_s)/100.0);
	}

//	if(pavi->playback.v.vdata != NULL)
//	{
//		vPortFree(pavi->playback.v.vdata);
//		pavi->playback.v.vdata = NULL;
//	}
//	if(data_file != NULL)
//	{
//		vPortFree(data_file);
//		data_file = NULL;
//	}
/*	if(pavi->playback.idxdata != NULL)
	{
		vPortFree(pavi->playback.idxdata);
		pavi->playback.idxdata = NULL;
	}
*/
	pavi->playback.v.vdata = NULL;
	pavi->playback.a.adata = NULL;
	pavi->playback.idxdata = NULL;

	pavi->state = AVIENC_EXIT;

done:
	enc_ing = NULL;

	LEAVE();
}

void muxer_aviplayback_process(t_avienc_t *pavi)
{
	switch(pavi->state)
	{
		case AVIENC_OFF:	//UartPutByte('.');
			//printf("muxer_aviplayback_process AVIENC_OFF \n");	
			if(gtUser.bSdAviSave == SDSAVE_USE_OFF
				&& h264_mb.playSwitch ==1 
				&& h264_mb.playbackgo == 1)
			{printf("muxer_aviplayback_process AVIDEC_ON!! \n");	
				h264_mb.ready = 0;
				g711_mb.ready = 0;
				txts_mb.ready = 0;
				pavi->state = AVIENC_IDLE;
			}
			vTaskDelay(1);
			break;
		case AVIENC_IDLE:	// => AVIENC_OPEN call => AVIENC_WRITE call			
			memset((BYTE*)H264_STRM_BASE, 0x00, H264_VIDEO_SIZE);
			printf("muxer_aviplayback_process AVIDEC_IDLE!! \n"); 
			
			static int idxcheck = 0;
			static int idxcheckcnt = 0; 

			h264_mb.pause = 0;		
			h264_mb.ready = 0;
			h264_mb.update = 0;
			h264_mb.range = 0;
			g711_mb.ready = 0;
			g711_mb.update = 0;
			txts_mb.ready = 0;
			txts_mb.update = 0;
			framenum = 0;
			//printf("\nAVIENC_IDLE\n");
			enc_ing = pavi;
			muxer_aviplayback_idle(pavi);
			if(pavi->state != AVIENC_READ)
			{
				goto proc_done;
			}
		case AVIENC_READ1:	
			dprintf("\nAVIENC_READ1\n");
			muxer_aviplayback_read_list(pavi);
			h264_mb.GetPlayTimeLock = 0;
			break;
		case AVIENC_READ2:	
			dprintf("\nAVIENC_READ2\n");
			muxer_aviplayback_read_first(pavi);
			break;
		case AVIENC_READ3:	
			dprintf("\nAVIENC_READ3\n");	
			muxer_aviplayback_read_keep(pavi);
			break;
		case AVIENC_READ4:
			if(h264_mb.ready==0)
			{
				pavi->state = AVIENC_READ;
			}
			break;
		case AVIENC_READ:	// => AVIENC_WRITE loop => AVIENC_CLOSE call
			dprintf("AVIENC_READ call\n");
			{
				//printf("\nAVIENC_READ\n");
				muxer_aviplayback_read_keep2(pavi);

				//vTaskDelay(100);
				if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
				{//printf("AVIENC_break call\n");
					break;
				}
					
			}
			break;
		case AVIENC_READ_IDX:
			{
				idxcheck = pavi->playback.index;
				//dprintf("AVIENC_READ_IDX %d / %d\n",pavi->playback.index, pavi->playback.totalframes);
				if(pavi->playback.index >= pavi->playback.totalframes)
				{
					pavi->state = AVIENC_EXIT;
				}
				else
				{

#if 0 				
					muxer_aviplayback_read_idx(pavi, pavi->playback.index);
					muxer_aviplayback_read_audidx(pavi, pavi->playback.index);
					pavi->playback.index++;
#else
					if(VIDEO_00dc_ID == pavi->playback.idxdata[pavi->playback.index].data.dwChunkId)
					{					
						muxer_aviplayback_read_idx(pavi, pavi->playback.index);
						pavi->playback.index++;
						//UartPutByte('v');
					}
					if(AUDIO_01wb_ID == pavi->playback.idxdata[pavi->playback.index].data.dwChunkId)
					{
						muxer_aviplayback_read_audidx(pavi, pavi->playback.index);
						pavi->playback.index++;
						//UartPutByte('a');
					}
					if (TEXT_02tx_ID == pavi->playback.idxdata[pavi->playback.index].data.dwChunkId)
					{
						muxer_aviplayback_read_gpsidx(pavi, pavi->playback.index);
						pavi->playback.index++;
						//UartPutByte('g');
					}
					//UartPutByte('_');
					vTaskDelay(1);
					
					if(idxcheck == pavi->playback.index)
						idxcheckcnt++;
					
					if(idxcheckcnt > 10)		
					{
						idxcheckcnt = 0;
						pavi->state = AVIENC_EXIT;
					}
/*
					static UINT testled2 = 0;
					if(testled2==0)
					{
						//dprintf("\nled 0\n");
						GpioSetLo(GPIO_LEDR1);
						GpioSetLo(GPIO_LEDG1);
						GpioSetLo(GPIO_LEDB1);
					}
					else if(testled2==100)
					{
						//dprintf("\nled 10\n");
						GpioSetHi(GPIO_LEDR1);
						GpioSetLo(GPIO_LEDG1);
						GpioSetLo(GPIO_LEDB1);	
					}
					else if(testled2>10)
					{
						testled2 = -1;
					}	*/				
#endif

				}
				if(pavi->state == AVIENC_IDLE || pavi->state == AVIENC_CLOSE || pavi->state == AVIENC_EXIT)
				{dprintf("AVIENC_break call\n");
					break;
				}
			}
			break;
		case AVIENC_PAUSE:
			if(h264_mb.pause==1)
			{
				//UartPutByte('a');
				pavi->state = AVIENC_PAUSE;
				vTaskDelay(1);
			}
			else
			{
				while(h264_mb.ready == 1)
				{
					//UartPutByte('=');
					h264_mb.ready = 0;
				}
				while(h264_mb.update == 1)
				{
					//UartPutByte('-');
					h264_mb.update = 0;
				}

				if(h264_mb.range > 0)
				{
					dprintf("\n h264_mb.range %d \n",h264_mb.range);
					pavi->playback.index = h264_mb.range*pavi->playback.fps*3;
					dprintf("\n pavi->playback.index  %d \n",pavi->playback.index);
					h264_mb.range = 0;
				}
				
				if(pavi->playback.fileidx != h264_mb.index)
					pavi->state = AVIENC_IDLE;
				else
					pavi->state = AVIENC_READ_IDX;
			}
			break;
		case AVIENC_CLOSE:	// CLOSE 작업중이다.
			dprintf("AVIENC_CLOSE call\n");
			break;
		case AVIENC_EXIT:	// User Command(Shell or GPIO)
			dprintf("AVIDEC_EXIT call\n");
			muxer_aviplayback_close(pavi);
			memset((BYTE*)H264_STRM_BASE, 0x00, H264_VIDEO_SIZE);
			//pavi->state = AVIENC_IDLE;
			h264_mb.playbackgo = 0;
			h264_mb.pause = 1;
			enc_ing = NULL;
			pavi->state = AVIENC_OFF;
			vTaskDelay(1);
			//h264_mb.playSwitch=0;
			//stt_avidec = NULL;
			//vTaskDelete(NULL);
			return 0;
		default:
			//printf("%s Error Call\n", __func__);
			break;
	}

	
proc_done:
/*	if(h264_mb.playbackgo == 0 && pavi->state != AVIENC_EXIT)
	{
		dprintf("\nAVIENC_EXIT\n");
		pavi->state = AVIENC_EXIT;
	}
*/	return 0;
}


void muxer_avidec(void)
{
	ENTER();

//	if(!muxer_avienc_sdCardMounted())
//	{
//		UartPutByte('_');
//		vTaskDelay(1);
//	    goto done;
//	}

	//printf("\ng_playbackgo\n");
	muxer_aviplayback_process(&enc_play);

done:
	LEAVE();
}



void muxer_avidec_init(void)
{
	ENTER();
	int nTotal = 0, nEvtTotal = 0;
	UINT muxer_sclust, muxer_sclust_evt, muxer_eclust, muxer_sclust_patition;

	//printf("\nmuxer_avidec_init\n");

    if(!Sdio0GetInstall())
    {
		bSD_FAT_mounted = 1;

#if 1//def CONTIGUOUS_WRITE
        muxer_sclust = avio_get_cluster_offset(&gtFatFs[MMC], MAX_AVI_FILE_SIZE + MAX_JPEG_FILE_SIZE);
		muxer_sclust_evt = avio_get_cluster_offset(&gtFatFs[MMC], EVN_AVI_FILE_SIZE + MAX_JPEG_FILE_SIZE);
		muxer_eclust = avio_get_cluster_offset(&gtFatFs[MMC], 0);

		muxer_sclust_patition = (muxer_eclust/100)*90;

		nTotal = (muxer_sclust_patition - (muxer_sclust*4)) / muxer_sclust;
		nEvtTotal = (muxer_eclust - muxer_sclust_patition - (muxer_sclust_evt*4))/muxer_sclust_evt;

		nSAVEcnt.normalmaxcnt = nTotal;
		nSAVEcnt.eventmaxcnt = nEvtTotal/2;
		nSAVEcnt.manualmaxcnt = nEvtTotal/2;

		muxer_sclust_patition = nTotal * muxer_sclust + (muxer_sclust*2);
		muxer_eclust = muxer_sclust_patition + (((nEvtTotal/2)+(nEvtTotal/2)) * muxer_sclust_evt) + (muxer_sclust_evt*2);
#endif

//		if(nSD_CARD_SIZE == 8)	// 8GB
//		{
//			nSD_CARD_FLAG = 0;
//		}
//		else if(nSD_CARD_SIZE == 16)	// 16GB
//		{
//			nSD_CARD_FLAG = 1;
//		}
//		else if(nSD_CARD_SIZE == 32)	// 32GB
//		{
//			nSD_CARD_FLAG = 2;
//		}

		// 주행 - 상시
#ifdef CONTIGUOUS_WRITE  
		//avienc_info_default(&enc_play, muxer_sclust, MAX_AVI_FILE_SIZE);
		avienc_info_default(&enc_play, muxer_sclust, muxer_sclust, muxer_sclust_patition, MAX_AVI_FILE_SIZE, MAX_JPEG_FILE_SIZE);
#else
//		avienc_info_default(&enc_play);
#endif
//#ifdef AVIENC_HEAP_USE		
//		avienc_info_init_filenamelist(&enc_play, nSAVEcnt.normalmaxcnt);	// max
//#endif		
////		avidec_info_init_savetime(&enc_play, SAVE_NOR_TIME_MAX, SAVE_NOR_VIDEO_FPS, AUDIO_FPS, TXTS_GPS_FPS, TXTS_GSS_FPS, TXTS_TIME_FPS, TXTS_OBD_FPS);

    }

	LEAVE();
}

void muxer_avidec_delete(void)
{
	ENTER();

	//avienc_info_deinit(&enc_play);

	LEAVE();
}


void muxer_aviplayback_task(void* pvParameters)
{
/* 
	we first init sd card driver 
*/
	printf("\nmuxer_aviplayback_task\n");

//	while(stt_avienc!=NULL)
//		vTaskDelay(1);
	//printf("\nPlayback Start\n");

	muxer_avidec_init();

	for(;;)
	{		
		muxer_avidec();
		//vTaskDelay(1);
	}
}
#endif

#ifdef __JPEG__

int jpeg_thumbnail_save(t_avienc_t *pavi)
{	printf("%s \r\n", __func__);
	//FIL fp;
	int nByteWritten;
	char strBuf[_MAX_LFN+1] = {0};
	
	UINT sect=0;
	pavi->stime = xTaskGetTickCount();
	printf("%s gptMsgShare.JPEG_STILL_FLAG %d \r\n", __func__, gptMsgShare.JPEG_STILL_FLAG);

	while(gptMsgShare.JPEG_STILL_FLAG != JPEG_SNAP_END)
	{
		printf(" %d \r\n", gptMsgShare.JPEG_STILL_FLAG);
		vTaskDelay(1);
	}
	
	UINT stime_end1 = xTaskGetTickCount();	
	
	if(gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_END && gptMsgShare.JPEG_STILL_SIZE> 0)
	{	
		dprintf("Jpg thumbnail sz  %d \n", gptMsgShare.JPEG_STILL_SIZE);
//		if(g711_mb.sleepmodeE == 1)
//			sprintf(strBuf, "%d:/%s/%s%s", MMC, MMC_DIR_SNABSHOT, pavi->now_name,SAVE_JPEG_THUMBNAIL_FORMAT);
//		else			
			sprintf(strBuf, "%d:/%s/%s%s", MMC, MMC_DIR_THUMBNAIL, pavi->now_name,SAVE_JPEG_THUMBNAIL_FORMAT);
		
		if(f_open(&pavi->fpavi, strBuf, FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
		{
			dprintf("Error : f_open File system fail %s \n", strBuf);
			return -1;
		}
		
		UINT stime_end2 = xTaskGetTickCount();	

#ifdef AUXCLUST
		sect = alloc_aux_clusters_thumb(&pavi->fpavi, pavi->clust_start, pavi->clust_end, pavi->maxthumb_len, pavi->maxdata_len, pavi->type);
#else
		sect = avio_alloc_clusters(&pavi->fpavi, pavi->clust_start, MAX_JPEG_FILE_SIZE,MAX_AVI_FILE_SIZE);
#endif
		UINT stime_end = xTaskGetTickCount();

		if(((stime_end2 - pavi->stime)*10)+((stime_end - stime_end2)*10)>200)
		{
			printf("open(%04dms) sect(%04dms) %s\n",(stime_end2 - pavi->stime)*10, (stime_end - stime_end2)*10, strBuf);
		}
		else
		{
			printf("open %s\n", strBuf);			
		}
		

		if((pavi->fpavi.fs->last_clust - pavi->fpavi.sclust) != (MAX_JPEG_FILE_SIZE/BYTE_PER_CLUST)-1)
		{
			printf("fp.sclust! %d fp.fs->last_clust! %d %d\n",pavi->fpavi.sclust,pavi->fpavi.fs->last_clust,(MAX_JPEG_FILE_SIZE/BYTE_PER_CLUST)-1); // 시작위치 
		}
#if 1		
		Last_clustJPG = pavi->fpavi.fs->last_clust;//ksjdebug
#endif
	    if(sect)
	    {
#ifdef __JPEGEXIF__	    
		    //memcpy(gptMsgShare.JPEG_STILL_ADDR, pavi->gpsdata, EXIF_TOTAL_LENGTH);		    
			DmaMemCpy_ip((BYTE *)gptMsgShare.JPEG_STILL_ADDR, (BYTE *)&pavi->gpsdata, EXIF_TOTAL_LENGTH);
			invalidate_dcache_range((UINT)gptMsgShare.JPEG_STILL_ADDR, ((UINT)gptMsgShare.JPEG_STILL_ADDR) + EXIF_TOTAL_LENGTH);
			
		    aux_file_low_write(&pavi->fpavi, gptMsgShare.JPEG_STILL_ADDR, gptMsgShare.JPEG_STILL_SIZE+EXIF_TOTAL_LENGTH, sect);     
#else
			aux_file_low_write(&pavi->fpavi, gptMsgShare.JPEG_STILL_ADDR, gptMsgShare.JPEG_STILL_SIZE, sect);	  
#endif
	    }
		else
		{
			//printf(" sect %d f_unlink %s\n",sect, strBuf);		
			f_close(&pavi->fpavi);
			if(f_unlink(strBuf) != FR_OK)
			{
				printf("f_unlink fail %s\n", strBuf);
			}
			else
			{
			//	dprintf("f_unlink ok %s\n", strBuf);
			}	

			//printf("jpg file error deleted. \n");
			return -1;
		}

#ifdef CLOSE_LSEEK		
	if(aux_file_lseek(&pavi->fpavi, pavi->maxthumb_len) == -1)
	{
		dprintf("[ f_lseek after  missmatch sclust %d last %d \n", pavi->fpavi.sclust, pavi->fpavi.fs->last_clust);

		f_close(&pavi->fpavi);
		f_unlink(pavi->now_name);
		return -1;
	}	
#endif	

	f_close(&pavi->fpavi);

	}
	else
	{
		dprintf("Jpg thumbnail failed!!!!!!!!!\n");
		//printf(" sect %d f_unlink %s\n",sect, strBuf);		
		f_close(&pavi->fpavi);
		if(f_unlink(strBuf) != FR_OK)
		{
			printf("f_unlink fail %s\n", strBuf);
		}
		else
		{
		//	dprintf("f_unlink ok %s\n", strBuf);
		}	
		
		//printf("jpg file error deleted. \n");
		return -1;
	}
	return 0;
}
#endif


#endif
