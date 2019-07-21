/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include <string.h>

#include "dev.h"
#include "dev_inline.h"
#include "msg.h"

#include "user.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "fs.h"
#include "fsdata.h"
#include "ff.h"		// FATFS header
#include "file.h"
#include "fsfilelist.h"
#ifdef __WIFI__
#include "fsaplist.h"
#endif
#ifdef __ONVIF__
#include "enx_gsoap_http.h"
#endif

/** Set this to 1 to include "fsdata_custom.c" instead of "fsdata.c" for the
 * file system (to prevent changing the file included in CVS) */
#ifndef HTTPD_USE_CUSTOM_FSDATA
#define HTTPD_USE_CUSTOM_FSDATA 0
#endif

#if HTTPD_USE_CUSTOM_FSDATA
#include "fsdata_custom.c"
#else /* HTTPD_USE_CUSTOM_FSDATA */
#include "fsdata.c"
#endif /* HTTPD_USE_CUSTOM_FSDATA */

/*-----------------------------------------------------------------------------------*/

#if LWIP_HTTPD_CUSTOM_FILES
int fs_open_custom(struct fs_file *file, const char *name);
void fs_close_custom(struct fs_file *file);
#if LWIP_HTTPD_FS_ASYNC_READ
u8_t fs_canread_custom(struct fs_file *file);
u8_t fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg);
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
#endif /* LWIP_HTTPD_CUSTOM_FILES */

/*-----------------------------------------------------------------------------------*/
err_t
fs_open(struct fs_file *file, const char *name)
{
  const struct fsdata_file *f;

  if ((file == NULL) || (name == NULL)) {
     return ERR_ARG;
  }

#ifdef __ONVIF__
#if LWIP_HTTPD_CUSTOM_FILES
  file->is_custom_file = 0;
#endif
#endif

#if LWIP_HTTPD_CUSTOM_FILES
  if (fs_open_custom(file, name)) {
    file->is_custom_file = 1;
    return ERR_OK;
  }
  file->is_custom_file = 0;
#endif /* LWIP_HTTPD_CUSTOM_FILES */

  for (f = FS_ROOT; f != NULL; f = f->next) {
    if (!strcmp(name, (char *)f->name)) {
//    printf("FS_FILE open ok : %s\r\n", name);
      file->data = (const char *)f->data;
      file->len = f->len;
      file->index = f->len;
      file->pextension = NULL;
      file->http_header_included = f->http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
      file->chksum_count = f->chksum_count;
      file->chksum = f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
      file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
      return ERR_OK;
    }
  }
  /* file not found */
  return ERR_VAL;
}

/*-----------------------------------------------------------------------------------*/
void
fs_close(struct fs_file *file)
{
#ifdef __ONVIF__
  if(file->is_onvif_file)
  {
    fs_close_onvif(file);
  }
#endif

#if LWIP_HTTPD_CUSTOM_FILES
  if (file->is_custom_file) {
    fs_close_custom(file);
  }
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#if LWIP_HTTPD_FILE_STATE
  fs_state_free(file, file->state);
#endif /* #if LWIP_HTTPD_FILE_STATE */
  LWIP_UNUSED_ARG(file);
}
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_DYNAMIC_FILE_READ
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_read_async(struct fs_file *file, char *buffer, int count, fs_wait_cb callback_fn, void *callback_arg)
#else /* LWIP_HTTPD_FS_ASYNC_READ */
int
fs_read(struct fs_file *file, char *buffer, int count)
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
{
  int read;

  if(file->index == file->len) {
    return FS_READ_EOF;
  }
#if LWIP_HTTPD_FS_ASYNC_READ
#if LWIP_HTTPD_CUSTOM_FILES
  if (!fs_canread_custom(file)) {
    if (fs_wait_read_custom(file, callback_fn, callback_arg)) {
      return FS_READ_DELAYED;
    }
  }
#else /* LWIP_HTTPD_CUSTOM_FILES */
  LWIP_UNUSED_ARG(callback_fn);
  LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_CUSTOM_FILES */
#endif /* LWIP_HTTPD_FS_ASYNC_READ */

  read = file->len - file->index;
  if(read > count) {
    read = count;
  }

  MEMCPY(buffer, (file->data + file->index), read);
  file->index += read;

  return(read);
}
#endif /* LWIP_HTTPD_DYNAMIC_FILE_READ */
/*-----------------------------------------------------------------------------------*/
#if LWIP_HTTPD_FS_ASYNC_READ
int
fs_is_file_ready(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
  if (file != NULL) {
#if LWIP_HTTPD_CUSTOM_FILES
    if (!fs_canread_custom(file)) {
      if (fs_wait_read_custom(file, callback_fn, callback_arg)) {
        return 0;
      }
    }
#else /* LWIP_HTTPD_CUSTOM_FILES */
    LWIP_UNUSED_ARG(callback_fn);
    LWIP_UNUSED_ARG(callback_arg);
#endif /* LWIP_HTTPD_CUSTOM_FILES */
  }
  return 1;
}
#endif /* LWIP_HTTPD_FS_ASYNC_READ */
/*-----------------------------------------------------------------------------------*/
int
fs_bytes_left(struct fs_file *file)
{
  return file->len - file->index;
}

#if LWIP_HTTPD_CUSTOM_FILES
extern char _q_x2c(char hex_up, char hex_low);
extern size_t _q_urldecode(char *str);
int fs_open_custom(struct fs_file *file, const char *name)
{
	char filepath[256] = {0};
	u32_t mallocsize;
#ifdef __WIFI__
	if (strncmp(name, "/wifi/scanap.js", strlen("/wifi/scanap.js")) == 0) {
		UINT realsize = 0;
		file->data = enx_web_get_aplist(&realsize);
		if (file->data) { // malloc ok
			file->ffType = fftMalloc;
			file->len = realsize;
			file->index = realsize;
			file->pextension = NULL;
			file->http_header_included = 0;//f->http_header_included;
			return 1;
		} else {
			printf("%s(%d) : file malloc fail : %s\r\n", __func__, __LINE__, filepath);
			return 0;
		}
	} else 
#endif
#if JPEG_SNAPSHOT
	if (strncmp(name, "/still.jpg", strlen("/still.jpg")) == 0) {
		while (gptMsgShare.JPEG_STILL_FLAG == JPEG_SNAP_PROC) {
			WaitXus(10);
		}
		gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_STR;
		gptMsgShare.JPEG_STILL_REF++;
		file->data = (char *)gptMsgShare.JPEG_STILL_ADDR;
		if (file->data) { // malloc ok
			UINT jpeg_size = gptMsgShare.JPEG_STILL_SIZE;
			file->ffType = fftJpeg;
			file->len = jpeg_size;
			file->index = jpeg_size;
			file->pextension = NULL;
			file->http_header_included = 0;//f->http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
			file->chksum_count = 0;//f->chksum_count;
			file->chksum = NULL;//f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
			file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
			return 1;
		} else {
			gptMsgShare.JPEG_STILL_REF--;
			if (gptMsgShare.JPEG_STILL_REF == 0) {
				gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
			} else if (gptMsgShare.JPEG_STILL_REF < 0) {
				printf("%s(%d) : JPEG_STILL_REF(%d) Error!\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
				gptMsgShare.JPEG_STILL_REF = 0;
			}
			printf("%s(%d) : file malloc fail : %s\r\n", __func__, __LINE__, filepath);
			return 0;
		}
	}
	else
#endif
#ifdef __DEVICE_SD__
	if (strncmp(name, "/"MMC_DIR_ROOT_PATH"/", strlen("/"MMC_DIR_ROOT_PATH"/")) == 0) {
		// SD card File list
		char filename[512] = {0};
		strlcpy(filename, &name[8], 512);
		_q_urldecode(filename);

		char *strlsxml = strstr(filename, MMC_DIR_LS_XML);
		char *strlshtml = strstr(filename, MMC_DIR_LS_HTML);
		if (strlsxml != NULL || strlshtml != NULL) {
			if (gtUser.bSdAviSave == SDSAVE_USE_ON) {
				gtUser.bSdAviSave = SDSAVE_USE_OFF;
				flprintf("Web connect - SD File list - SDcard save Stop!\r\n");
				vTaskDelay(10);
			}

			char fileVpath[_MAX_LFN*2] = {0};
			int strlens = 0;
			if (strlsxml) {
				strlens = strlsxml - filename;	// [../sdsave/] ~ [ls.xml] inside char count
			} else {
				strlens = strlshtml - filename;	// [../sdsave/] ~ [ls.html] inside char count
			}
			strncpy(fileVpath, filename, strlens);
			if(fileVpath[strlens-1] == '/')
				fileVpath[strlens-1] = '\0';
			sprintf(filepath, "%d:/%s", MMC, fileVpath);

			UINT realsize = 0;
			if (strlsxml) {
				file->data = (const char *)enx_webls_get_xml_sdcheck(&realsize);
				if (file->data == NULL) {
					file->data = (const char *)enx_webls_get_xml(filepath, &realsize);
				}
			} else {
				file->data = (const char *)enx_webls_get_html_sdcheck(&realsize);
				if (file->data == NULL) {
					file->data = (const char *)enx_webls_get_html(filepath, &realsize);
				}
			}
			if(file->data) { // malloc ok
				file->ffType = fftMalloc;
				file->len = realsize;
				file->index = realsize;
				file->pextension = NULL;
				file->http_header_included = 0;//f->http_header_included;
				return 1;
			} else {
				printf("%s(%d) : file malloc fail : %s\r\n", __func__, __LINE__, filepath);
				return 0;
			}
		} else {
			snprintf(filepath, 255, "%d:/%s", MMC, &name[8]);
#if HTTPD_BIG_FILE_SEND
			if (f_open(&file->fat_file, filepath, FA_READ) != FR_OK) {
				printf("%s(%d) : file open fail : %s\n", __func__, __LINE__, filepath);
				return 0;
			}
			mallocsize = file->fat_file.fsize;
//			file->filename = mem_malloc(strlen(filepath));						// $CMT-hjlee-170807 - bigfile filename
//			strcpy(file->filename, filepath);									// $CMT-hjlee-170807 - bigfile filename

			file->ffType = fftBigfile;

			file->is_Big_custom_file = 1;
			file->is_Big_custom_file_sent_size = 0;
			file->is_Big_custom_file_left_size = mallocsize;

			printf("%s(%d) : file open size : %d\n", __func__, __LINE__, mallocsize);
			file->data = NULL;
			file->len = mallocsize;
			file->index = mallocsize;
			file->pextension = NULL;
			file->http_header_included = 0;//f->http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
			file->chksum_count = 0;//f->chksum_count;
			file->chksum = NULL;//f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
			file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */

			return 1;
#endif
		}
	} else
#endif
	{
		snprintf(filepath, 255, "%d:/%s%s", FLS, FLS_DIR_WEB_PATH, name);
	}
	
	if(f_open(&file->fat_file, filepath, FA_READ) != FR_OK)
	{
//		printf("CUSTOM_FILE open fail : %s\r\n", filepath);
		return 0;
	}

	mallocsize = file->fat_file.fsize;
	file->data = mem_malloc(mallocsize);
	if(file->data)	// malloc ok
	{
		UINT br;
		f_read(&file->fat_file, (void *)file->data, mallocsize, &br);
		if(br != mallocsize)
		{
			printf("%s(%d) : Warning! Readsize(%d) Filesize(%d)\r\n", __func__, __LINE__, br, mallocsize);
		}
		file->ffType = fftMalloc;
		file->len = mallocsize;
		file->index = mallocsize;
		file->pextension = NULL;
		file->http_header_included = 0;//f->http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
		file->chksum_count = 0;//f->chksum_count;
		file->chksum = NULL;//f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
		file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
		return 1;
	}
	else
	{
		printf("%s(%d) : file malloc fail : %s\r\n", __func__, __LINE__, filepath);
		return 0;
	}
}

void fs_close_custom(struct fs_file *file)
{
	if (file->fat_file.fs != NULL) {
		f_close(&file->fat_file);
	}

#if HTTPD_BIG_FILE_SEND
	if (file->ffType == fftBigfile) {
		if (file->is_Big_custom_file) {
			file->is_Big_custom_file = 0;
			file->is_Big_custom_file_left_size = 0;
			file->is_Big_custom_file_sent_size = 0;
		}
//		if(file->filename) {													// $CMT-hjlee-170807 - bigfile filename
//			mem_free(file->filename);											// $CMT-hjlee-170807 - bigfile filename
//			file->filename = NULL;												// $CMT-hjlee-170807 - bigfile filename
//		}																		// $CMT-hjlee-170807 - bigfile filename
		file->data = NULL;
	} else
#endif
	if (file->ffType == fftJpeg) {
		file->data = NULL;
		gptMsgShare.JPEG_STILL_REF--;
		if (gptMsgShare.JPEG_STILL_REF == 0) {
			gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
		} else if (gptMsgShare.JPEG_STILL_REF < 0) {
			printf("%s(%d) : JPEG_STILL_REF(%d) Error!\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
			gptMsgShare.JPEG_STILL_REF = 0;
		}
	} else if (file->ffType == fftMalloc) {
		if(file->data) {
			mem_free((void *)file->data);
		}
		file->data = NULL;
	} else { // ?
		printf("%s(%d) : Type(%d)?????\r\n", __func__, __LINE__, file->ffType);
	}
}

#if LWIP_HTTPD_FS_ASYNC_READ
u8_t fs_canread_custom(struct fs_file *file)
{
	return 0;
	UNUSED(file);
}

u8_t fs_wait_read_custom(struct fs_file *file, fs_wait_cb callback_fn, void *callback_arg)
{
	return 0;
	UNUSED(file);
	UNUSED(callback_fn);
	UNUSED(callback_arg);
//	callback_fn(/*http_state hs*/);
}
#endif /* LWIP_HTTPD_FS_ASYNC_READ */

#endif
#ifdef __ONVIF__
int fs_open_onvif(struct fs_file *file, const char *name)
{
	if(file->data)  
	{
		file->index = file->len;
		file->pextension = NULL;
		file->http_header_included = 0;//f->http_header_included;
#if HTTPD_PRECALCULATED_CHECKSUM
		file->chksum_count = 0;//f->chksum_count;
		file->chksum = NULL;//f->chksum;
#endif /* HTTPD_PRECALCULATED_CHECKSUM */
#if LWIP_HTTPD_FILE_STATE
		file->state = fs_state_init(file, name);
#endif /* #if LWIP_HTTPD_FILE_STATE */
		file->ffType = fftOnvif;
		return ERR_OK;
	}
	return ERR_MEM;
	UNUSED(name);
}

void fs_close_onvif(struct fs_file *file)
{
	if (file->data) {
		if (file->ffType == fftJpeg) {
			file->data = NULL;
			gptMsgShare.JPEG_STILL_REF--;
			if (gptMsgShare.JPEG_STILL_REF == 0) {
				gptMsgShare.JPEG_STILL_FLAG = JPEG_SNAP_IDE;
			} else if (gptMsgShare.JPEG_STILL_REF < 0) {
				printf("%s(%d) : JPEG_STILL_REF(%d) Error!\r\n", __func__, __LINE__, gptMsgShare.JPEG_STILL_REF);
				gptMsgShare.JPEG_STILL_REF = 0;
			}
		} else if(file->ffType == fftMalloc || file->ffType == fftOnvif) {
			if(file->data) {
				mem_free((void *)file->data);
			}
			file->data = NULL;
		} else {	// ?
			printf("%s(%d) : Type(%d)?????\r\n", __func__, __LINE__, file->ffType);
		}
	}
}
#endif
