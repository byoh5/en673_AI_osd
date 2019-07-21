/* Copyright (c) 2013-2016 Eyenix Corporation. All rights reserved.
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

#include "user.h"
#include "msg.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "ff.h"
#include "file.h"

#include "fwupdate.h"
#ifdef __DEVICE_SD__
#include "muxer_avienc.h"					// for muxer_avienc_all_stop
#endif

//-------------------------------------------------------------------------------------------------
//

#define ENX_FWUP_DEBUG			// printf 주석용

#ifdef ENX_FWUP_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("[%04d]%s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

#define FLASH_BLOCK_SIZE	FAT_BLOCK_FLS

typedef struct {
	UINT* pBoot;
	UINT* pCpu;
	UINT* pCpu0;
	UINT* pCpu1;
	UINT Size;
	UINT bootSize;
	UINT cpuSize;
	UINT cpu0Size;
	UINT cpu1Size;
} FWUPDATE_BIN;

static int fwupdate_ing = -1;

//-------------------------------------------------------------------------------------------------
//
int fwupdate_process(void)
{	// -1:Idle, 0~99:Processing, 100:OK, -100:Fail
	return fwupdate_ing;
}

void fwupdate_encoding_stop(void)
{
#ifdef __DEVICE_SD__
	muxer_avienc_all_stop();	// 영상 녹화중이라면 멈춘다.
#endif
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_OFF) == DEF_FAIL)
		vTaskDelay(1);
	tprintf("\r\n");
	vTaskDelay(50);	// 500ms delay
}

void fwupdate_encoding_start(void)
{
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_ON) == DEF_FAIL)
		vTaskDelay(1);
	tprintf("\r\n");
}

int fwupdate_filecheck(DriveNum dn, char *file_url)
{
	FILINFO finfo;
	FRESULT fres;
#if _USE_LFN
	char lfilename[_MAX_LFN+1] = {0};
	finfo.lfname = lfilename;
	finfo.lfsize = _MAX_LFN;
#endif
	char firmware_name[_MAX_LFN+1];
	sprintf(firmware_name, "%d:/%s", dn, file_url);

	fres = f_stat(firmware_name, &finfo);

	switch(fres)
	{
		case FR_OK:
			break;
		case FR_NO_FILE:
		case FR_NO_PATH:
			return DEF_FAIL;
		default:
			tprintf("f_stat Error : %s\r\n", put_rc(fres));
			return DEF_FAIL;
	}

	tprintf("%s size : %dbyte\r\n", firmware_name, finfo.fsize);
	if(finfo.fsize == 0)
	{
		return DEF_FAIL;
	}

	return DEF_OK;
}

static UINT fwupdate_fileread(BYTE* ddr_addr, char* file_path)
{
	UINT bres = DEF_FAIL;
	UINT nReadSize;
	FIL fp;
	FRESULT fres;

	if(ddr_addr == NULL)
	{
		tprintf("ddr_addr Error : 0x%08X\r\n", ddr_addr);
		goto fileread_end;
	}
	else if(file_path == NULL)
	{
		tprintf("file_path Error : %s\r\n", file_path);
		goto fileread_end;
	}

	fres = f_open(&fp, file_path, FA_READ);
	if(fres != FR_OK)
	{
		tprintf("f_open Error : %s\r\n", put_rc(fres));
		goto fileread_end;
	}

	fres = f_read(&fp, ddr_addr, fp.fsize, &nReadSize);
	if(fres != FR_OK)
	{
		tprintf("f_read Error : %s\r\n", put_rc(fres));
		goto fileread_end;
	}

	fres = f_close(&fp);
	if(fres != FR_OK)
	{
		tprintf("f_close Error : %s\r\n", put_rc(fres));
		goto fileread_end;
	}

	bres = DEF_OK;

fileread_end:
	return bres;
}

static UINT fwupdate_check_version(BYTE *binary)
{
	UINT __new_ver = ((binary[0xAC]&0xf) << 8) | ((binary[0xAD]&0xf) << 4) | ((binary[0xAE]&0xf) << 0);
	printf("per version : %X.%X.%X\r\n", (EN673_VERSION>>8)&0xf, (EN673_VERSION>>4)&0xf, EN673_VERSION&0xf);
	printf("new version : %X.%X.%X\r\n", (__new_ver>>8)&0xf, (__new_ver>>4)&0xf, __new_ver&0xf);

	if (__new_ver > EN673_VERSION) {
		printf("new firmware\r\n");
		return DEF_OK;
	} else {
		printf("old firmware\r\n");
		return DEF_OK; // DEF_FAIL
	}
}

static UINT fwupdate_check_model(BYTE *binary)
{
	char __new_model[16] = {binary[0xA0], binary[0xA1], binary[0xA2], binary[0xA3],
							binary[0xA4], binary[0xA5], binary[0xA6], binary[0xA7],
							binary[0xA8], binary[0xA9], binary[0xAA], binary[0xAB]};
	printf("pre model : %s\r\n", EN673_MODEL);
	printf("new model : %s\r\n", __new_model);

	if (strcmp(__new_model, EN673_MODEL) == 0) {
		printf("EN673 firmware\r\n");
		return DEF_OK;
	} else {
		printf("other firmware\r\n");
		return DEF_FAIL;
	}
}

static UINT fwupdate_check(FWUPDATE_BIN *bininfo)
{
	// Binary Info
	bininfo->pBoot = (UINT *)FW_UPDATE_BASE;
	if (*(bininfo->pBoot) > BOOT_ADDRESS) {
		tprintf(" (bininfo->pBoot) : 0x%08X\r\n",  (bininfo->pBoot));
		tprintf("*(bininfo->pBoot) : 0x%08X\r\n", *(bininfo->pBoot));
		tprintf("Error bootloader\r\n");
		return DEF_FAIL;
	}

	if (fwupdate_check_model((BYTE *)bininfo->pBoot) == DEF_FAIL) {
		tprintf("Error bootloader\r\n");
		return DEF_FAIL;
	}

	if (fwupdate_check_version((BYTE *)bininfo->pBoot) == DEF_FAIL) {
		tprintf("Error bootloader\r\n");
		return DEF_FAIL;
	}

	bininfo->bootSize = *(bininfo->pBoot);
	bininfo->pCpu = (UINT*)((UINT)bininfo->pBoot + (UINT)bininfo->bootSize);
	bininfo->pCpu0 = bininfo->pCpu;
	if (*(bininfo->pCpu0) > SFLS_SIZE) {
		tprintf(" (bininfo->pCpu0) : 0x%08X\r\n",  (bininfo->pCpu0));
		tprintf("*(bininfo->pCpu0) : 0x%08X\r\n", *(bininfo->pCpu0));
		tprintf("Error core0 binary\r\n");
		return DEF_FAIL;
	}
	bininfo->cpu0Size = *(bininfo->pCpu0);
	bininfo->pCpu1 = (UINT*)((UINT)bininfo->pCpu0 + (UINT)bininfo->cpu0Size);
	if (*(bininfo->pCpu1) > SFLS_SIZE) {
		tprintf(" (bininfo->pCpu1) : 0x%08X\r\n",  (bininfo->pCpu1));
		tprintf("*(bininfo->pCpu1) : 0x%08X\r\n", *(bininfo->pCpu1));
		tprintf("Error core1 binary\r\n");
		return DEF_FAIL;
	}
	bininfo->cpu1Size = *(bininfo->pCpu1);
	bininfo->cpuSize = bininfo->cpu0Size + bininfo->cpu1Size;
	bininfo->Size = bininfo->bootSize + bininfo->cpuSize;
	if (bininfo->Size > SFLS_SIZE) {
		tprintf("*(bininfo->pBoot) : 0x%08X\r\n", *(bininfo->pBoot));
		tprintf("*(bininfo->pCpu0) : 0x%08X\r\n", *(bininfo->pCpu0));
		tprintf("*(bininfo->pCpu1) : 0x%08X\r\n", *(bininfo->pCpu1));
		tprintf("Error all binary\r\n");
		return DEF_FAIL;
	}
	tprintf("Binary Size(%dByte)\r\n", bininfo->bootSize + bininfo->cpuSize);
	tprintf("├MEM Boot Addr(0x%08X) Size(%dByte)\r\n", bininfo->pBoot, bininfo->bootSize);
	tprintf("└MEM CPU  Addr(0x%08X) Size(%dByte)\r\n", bininfo->pCpu, bininfo->cpuSize);
	tprintf("  ├  CPU0 Addr(0x%08X) Size(%dByte)\r\n", bininfo->pCpu0, bininfo->cpu0Size);
	tprintf("  └  CPU1 Addr(0x%08X) Size(%dByte)\r\n", bininfo->pCpu1, bininfo->cpu1Size);

	return DEF_OK;
}

static int fwupdate_flswrite(BYTE *pPos, BYTE *pData, UINT nSize)
{
	UINT i;
	BYTE pCheckbin[FLASH_BLOCK_SIZE] = {0};

	FlsErase((UINT)pPos);					// 현재 위치를 기존 데이터를 지운다.
	vTaskDelay(1);
	FlsWrite((UINT)pPos, pData, nSize);		// 현재 위치에 새로운 데이터를 쓴다.
	vTaskDelay(1);
	FlsRead(pCheckbin, (UINT)pPos, nSize);

	// Verify
	for(i=0;i<nSize;i++)
	{
		if(pCheckbin[i] != pData[i])
		{
			printf(": %d %x %x\r\n", i, pCheckbin[i], pData[i] );
			return DEF_FAIL;
		}
	}
	return DEF_OK;
}

// flash start address
static void fwupdate_binwrite(BYTE* pFlsStart, BYTE* pStartbin, UINT Sizeofbin, UINT TotalSizeofbin)
{
	BYTE* pFlash = (BYTE*)pFlsStart + SFLS_BASE;
	UINT nPos = 0, nSize = 0, nRes = 0;

	while(Sizeofbin)
	{
		nSize = (Sizeofbin >= FLASH_BLOCK_SIZE ? FLASH_BLOCK_SIZE : Sizeofbin);
		nRes = fwupdate_flswrite(pFlash + nPos, pStartbin + nPos, nSize);
		if(nRes == DEF_OK)
		{
			nPos += nSize;
			Sizeofbin -= nSize;
			fwupdate_ing = (nPos * 100) / TotalSizeofbin;
		}
		vTaskDelay(1);
	}
}

void fwupdate_Task(void *pvParameters)
{
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvParameters;
	FWUPDATE_BIN bininfo;
	UINT fwres = DEF_OK;

	tprintf("Start\r\n");
	fwupdate_ing = 0;

	if(fwinfo->is_encodingstop == DEF_YES)
	{
		tprintf("encoding stop start\r\n");
		fwupdate_encoding_stop();
		tprintf("encoding stop end\r\n");
	}

	// file => ddr copy
	if(fwinfo->type != eFWT_DDR)
	{
		if(fwupdate_fileread((BYTE *)FW_UPDATE_BASE, fwinfo->file_path) == DEF_FAIL)
		{
			tprintf("file read fail\r\n");
			fwres = DEF_FAIL;
			goto done;
		}
		else
		{
			tprintf("file read ok\r\n");
		}
	}

	// bin address, size check
	// 이 함수에 현재 체크하는 코드는 없다. 항상 DEF_OK만 리턴하고 있다.
	if(fwupdate_check(&bininfo) == DEF_FAIL)
	{
		tprintf("bin check fail\r\n");
		fwres = DEF_FAIL;
		goto done;
	}
	else
	{
		tprintf("bin check ok\r\n");
	}

	// boot area change
	if(fwinfo->is_areachange == DEF_YES)
	{
		tprintf("old gtUser._pBootAddr(0x%08X)\r\n", gtSystem._pBootAddr);
		if(gtSystem._pBootAddr == (UINT *)BOOT_ADDRESS)
		{	// 다른곳으로 변경
			gtSystem._pBootAddr = (UINT *)FW_UPDATE_AREA;
		}
		else
		{	// 원래위치로 변경(0x10000)
			gtSystem._pBootAddr = (UINT *)BOOT_ADDRESS;
		}
		tprintf("new gtUser._pBootAddr(0x%08X)\r\n", gtSystem._pBootAddr);
	}

	// boot.bin write
	if(fwinfo->is_bootwrite == DEF_YES)
	{
		tprintf("boot.bin write start\r\n");
		fwupdate_binwrite((BYTE*)0, (BYTE*)bininfo.pBoot, bininfo.bootSize, bininfo.Size);
		tprintf("boot.bin write end\r\n");
	}

	// cpu0.bin, cpu1.bin write
	tprintf("cpu.bin write start\r\n");
	fwupdate_binwrite((BYTE*)gtSystem._pBootAddr, (BYTE*)bininfo.pCpu, bininfo.cpuSize, bininfo.Size);
	tprintf("cpu.bin write end\r\n");

	if(fwinfo->is_areachange == DEF_YES)
	{
		tprintf("Next bootaddr %x\r\n", gtSystem._pBootAddr);
		UserSave(sfls_SYSTEM);
	}

	if(fwinfo->type != eFWT_DDR)
	{
		tprintf("%s f_unlink\r\n", fwinfo->file_path);
		f_unlink(fwinfo->file_path);
	}

done:
	if (fwres == DEF_OK) {
		fwupdate_ing = 100; // complete
	} else {
		fwupdate_ing = -100; // complete(fail)
	}
	tprintf("Stop\r\n");
	if (fwinfo->is_autoreboot == DEF_YES) {
		tprintf("System Reset\r\n");
		vTaskDelay(10);
		WdtSysReboot();
	} else if (fwinfo->is_malloc == DEF_YES) {
		vPortFree(fwinfo);
	}
	vTaskDelete(NULL);
}

#if 0
void fwupdate_example(void)
{	// DDR type
	FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
	fwinfo->ddr_addr = FW_UPDATE_BASE;
	fwinfo->type = eFWT_DDR;
	fwinfo->is_areachange = DEF_YES;
	fwinfo->is_bootwrite = DEF_NO;
	fwinfo->is_autoreboot = DEF_YES;
	fwupdate_encoding_stop();
	sys_thread_new("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
}
#endif
