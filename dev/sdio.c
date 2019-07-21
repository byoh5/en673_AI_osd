/* Copyright (c) 2015 Eyenix Corporation. All rights reserved.
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
#include "sdio.h"

#ifdef __WIFI__

//#define sdio_support_1_8v

#define ENX_SDIO_CMD_DEBUG
//#define ENX_SDIO_DAT_DEBUG
#define ENX_SDIO_ERR_DEBUG

#ifdef ENX_SDIO_CMD_DEBUG
	#define gprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_CMD_LOG_START	{	printf("(%04d)%s : START\r\n", __LINE__, __func__);	}
	#define SDIO_CMD_LOG_END	{	printf("(%04d)%s : E N D\r\n", __LINE__, __func__);	}
#else
	#define gprintf(fmt, args...) do {} while(0);
	#define SDIO_CMD_LOG_START	{ }
	#define SDIO_CMD_LOG_END	{ }
#endif

#ifdef ENX_SDIO_DAT_DEBUG
	#define tprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
	#define SDIO_DAT_LOG_START	{	printf("(%04d)%s : START\r\n", __LINE__, __func__);	}
	#define SDIO_DAT_LOG_END	{	printf("(%04d)%s : E N D\r\n", __LINE__, __func__);	}
#else
	#define tprintf(fmt, args...) do {} while(0);
	#define SDIO_DAT_LOG_START	{ }
	#define SDIO_DAT_LOG_END	{ }
#endif

#ifdef ENX_SDIO_ERR_DEBUG
	#define eprintf(fmt, args...) do { printf("(%04d)%s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define eprintf(fmt, args...) do {} while(0);
#endif

//*************************************************************************************************
// Phy
//-------------------------------------------------------------------------------------------------
//
void Sdio1EiDet(void) { SDIO1_DET_IRQ_EN = 1; } // Detect interrupt enable
void Sdio1DiDet(void) { SDIO1_DET_IRQ_EN = 0; } // Detect interrupt disable
void Sdio1EiCmd(void) { SDIO1_CMD_IRQ_EN = 1; } // Command interrupt enable
void Sdio1DiCmd(void) { SDIO1_CMD_IRQ_EN = 0; } // Command interrupt disable
void Sdio1EiDat(void) { SDIO1_DAT_IRQ_EN = 1; } // Data interrupt enable
void Sdio1DiDat(void) { SDIO1_DAT_IRQ_EN = 0; } // Data interrupt disable
void Sdio1EiCip(void) { SDIO1_CHIP_IRQ_EN = 1; } // Chip interrupt enable
void Sdio1DiCip(void) { SDIO1_CHIP_IRQ_EN = 0; } // Chip interrupt disable

//-------------------------------------------------------------------------------------------------
//
BOOL Sdio1Cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe)
{
	WaitXus(1);

	if(Cmd != 12)
		while(SDIO1_BUSY | SDIO1_CMD_EN | SDIO1_DAT_EN);
	else
		while(SDIO1_CMD_EN | SDIO1_DAT_EN);
//	printf("%s(%d) : BUSY(%d) CMD_EN(%d) DAT_EN(%d)\r\n", __func__, __LINE__, SDIO1_BUSY, SDIO1_CMD_EN, SDIO1_DAT_EN);

	// R1b resp type
//	if(Cmd == 7 || Cmd == 12 || Cmd == 20 || Cmd == 28 || Cmd == 29 || Cmd == 38)
//		SDIO1_CMD_BUSY_EN = 1;

	SDIO1_CMD_IDX = Cmd;
	SDIO1_CMD_ARG = Arg;
	SDIO1_CMD_RESPEN = RespEn;
	SDIO1_CMD_RESPTYPE = RespType;
	SDIO1_DAT_WE = DatWe;
	SDIO1_DAT_EN = DatEn;
	SDIO1_CMD_EN = 1;
	while(SDIO1_CMD_EN);

//	while(SDIO1_BUSY);
//		printf("%s(%d) : BUSY(%d) CMD_EN(%d) DAT_EN(%d)\r\n", __func__, __LINE__, SDIO1_BUSY, SDIO1_CMD_EN, SDIO1_DAT_EN);

	// R2 resp type(do not check CRCERR)
	// R3 resp type(do net check CRCERR)
	if(Cmd == 2 || Cmd == 9 || Cmd == 10 || Cmd == 41)
	{
		if(SDIO1_CMD_RESPTOUT)
		{
			eprintf("CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO1_CMD_CRCERR, SDIO1_CMD_RESP_CRC, SDIO1_CMD_RESPTOUT);
			return SDIO_FAIL;
		}
	}
	else
	{
		if(SDIO1_CMD_CRCERR | SDIO1_CMD_RESPTOUT)
		{
			eprintf("CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO1_CMD_CRCERR, SDIO1_CMD_RESP_CRC, SDIO1_CMD_RESPTOUT);
			return SDIO_FAIL;
		}
	}

#if 0
	if(Cmd == 18 || Cmd == 17 || Cmd == 24 || Cmd == 25)
	{

	}
	else
	{
		vTaskDelay(1);
	}
#endif
	return SDIO_OK;
}

void Sdio1ClockDiv(SDIO_CLK nSetKHz)
{
#if 0
	UINT nKHzValue = (BUS_CLK / (2*1000*nSetKHz)) - 1;
	if(nKHzValue > 255)
	{
		eprintf("Sdio1 Clovk Div Error(%dKHz, Value(%d))\r\n", nSetKHz, nKHzValue);
		while(1);
	}
#else
	char strHzValue[8] = {0};
	switch(nSetKHz)	// 199.8MHz
	{
		case eSDIO_CLK_100KHz:	strHzValue[0] = '1'; strHzValue[1] = '0'; strHzValue[2] = '0'; strHzValue[3] = 'k'; break;
		case eSDIO_CLK_200KHz:	strHzValue[0] = '2'; strHzValue[1] = '0'; strHzValue[2] = '0'; strHzValue[3] = 'k'; break;
		case eSDIO_CLK_300KHz:	strHzValue[0] = '3'; strHzValue[1] = '0'; strHzValue[2] = '0'; strHzValue[3] = 'k'; break;
		case eSDIO_CLK_400KHz:	strHzValue[0] = '4'; strHzValue[1] = '0'; strHzValue[2] = '0'; strHzValue[3] = 'k'; break;
		case eSDIO_CLK_10MHz:	strHzValue[0] = '1'; strHzValue[1] = '0'; strHzValue[2] = 'M'; break;
		case eSDIO_CLK_25MHz:	strHzValue[0] = '2'; strHzValue[1] = '5'; strHzValue[2] = 'M'; break;
		case eSDIO_CLK_33MHz:	strHzValue[0] = '3'; strHzValue[1] = '3'; strHzValue[2] = 'M'; break;
		case eSDIO_CLK_50MHz:	strHzValue[0] = '5'; strHzValue[1] = '0'; strHzValue[2] = 'M'; break;
		case eSDIO_CLK_100MHz:	strHzValue[0] = '1'; strHzValue[1] = '0'; strHzValue[2] = '0'; strHzValue[3] = 'M'; break;
		default:				strHzValue[0] = 'E'; strHzValue[1] = 'r'; strHzValue[2] = 'r'; break;
	}
#endif
	eprintf("%sHz(CLK_DIV:%d)\r\n", strHzValue, nSetKHz);
	SDIO1_CLK_DIV = nSetKHz;
}

void Sdio1Cfg(SDIO_CLK nSetKHz, WORD DataBl, BYTE Mode)
{
	Sdio1ClockDiv(nSetKHz);
	SDIO1_MODE = Mode;
	SDIO1_DAT_BL = DataBl;
}

//ISRD SD_STATE sdinfo;

#if 0
void Sdio_Print_OCR(SD_OCR *ocr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("ocr->busy     [   31] : %d(0x%08X)\r\n", ocr->busy, ocr->busy);
	printf("ocr->CCS      [   30] : %d(0x%08X)\r\n", ocr->CCS, ocr->CCS);
	printf("ocr->UHSIICS  [   29] : %d(0x%08X)\r\n", ocr->UHSIICS, ocr->UHSIICS);
	printf("ocr->_res1    [28:25] : %d(0x%08X)\r\n", ocr->_res1, ocr->_res1);
	printf("ocr->VDD18    [   24] : %d(0x%08X)\r\n", ocr->VDD18, ocr->VDD18);
	printf("ocr->VDD35_36 [   23] : %d(0x%08X)\r\n", ocr->VDD35_36, ocr->VDD35_36);
	printf("ocr->VDD34_35 [   22] : %d(0x%08X)\r\n", ocr->VDD34_35, ocr->VDD34_35);
	printf("ocr->VDD33_34 [   21] : %d(0x%08X)\r\n", ocr->VDD33_34, ocr->VDD33_34);
	printf("ocr->VDD32_33 [   20] : %d(0x%08X)\r\n", ocr->VDD32_33, ocr->VDD32_33);
	printf("ocr->VDD31_32 [   19] : %d(0x%08X)\r\n", ocr->VDD31_32, ocr->VDD31_32);
	printf("ocr->VDD30_31 [   18] : %d(0x%08X)\r\n", ocr->VDD30_31, ocr->VDD30_31);
	printf("ocr->VDD29_30 [   17] : %d(0x%08X)\r\n", ocr->VDD29_30, ocr->VDD29_30);
	printf("ocr->VDD28_29 [   16] : %d(0x%08X)\r\n", ocr->VDD28_29, ocr->VDD28_29);
	printf("ocr->VDD27_28 [   15] : %d(0x%08X)\r\n", ocr->VDD27_28, ocr->VDD27_28);
	printf("ocr->_res2    [14: 8] : %d(0x%08X)\r\n", ocr->_res2, ocr->_res2);
	printf("ocr->VDDLOW   [    7] : %d(0x%08X)\r\n", ocr->VDDLOW, ocr->VDDLOW);
	printf("ocr->_res3    [ 6: 0] : %d(0x%08X)\r\n", ocr->_res3, ocr->_res3);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_CID(SD_CID *cid)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("cid->MID    [127:120] : %d(0x%08X)\r\n", cid->MID, cid->MID);
	printf("cid->OID    [119:104] : %d(0x%08X)\r\n", cid->OID, cid->OID);
	printf("cid->PNM1   [103: 96] : %d(0x%08X) : %c\r\n", cid->PNM1, cid->PNM1, cid->PNM1);
	printf("cid->PNM2     [95:88] : %d(0x%08X) : %c\r\n", cid->PNM2, cid->PNM2, cid->PNM2);
	printf("cid->PNM3     [87:80] : %d(0x%08X) : %c\r\n", cid->PNM3, cid->PNM3, cid->PNM3);
	printf("cid->PNM4     [79:72] : %d(0x%08X) : %c\r\n", cid->PNM4, cid->PNM4, cid->PNM4);
	printf("cid->PNM5     [71:64] : %d(0x%08X) : %c\r\n", cid->PNM5, cid->PNM5, cid->PNM5);
	printf("cid->PRV      [63:56] : %d(0x%08X)\r\n", cid->PRV, cid->PRV);
	printf("cid->PSN      [55:24] : %d(0x%08X)\r\n", cid->PSN, cid->PSN);
	printf("cid->_res1    [23:20] : %d(0x%08X)\r\n", cid->_res1, cid->_res1);
	printf("cid->MDT      [19: 8] : %d(0x%08X)\r\n", cid->MDT, cid->MDT);
	printf("cid->CRC      [ 7: 1] : %d(0x%08X)\r\n", cid->CRC, cid->CRC);
	printf("cid->_res2    [    0] : %d(0x%08X)\r\n", cid->_res2, cid->_res2);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_CSD(SD_CSD *csd)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	if(csd->csd_v1.CSD_STRUCTURE == 0)
	{	// CSD Version 1.0
		printf("csd->CSD_STRUCTURE      [127:126] : %d(0x%08X)\r\n", csd->csd_v1.CSD_STRUCTURE, csd->csd_v1.CSD_STRUCTURE);
	}
	else if(csd->csd_v1.CSD_STRUCTURE == 1)
	{	// CSD Version 2.0
		printf("csd->CSD_STRUCTURE      [127:126] : %d(0x%08X)\r\n", csd->csd_v2.CSD_STRUCTURE, csd->csd_v2.CSD_STRUCTURE);
		printf("csd->_res1              [125:120] : %d(0x%08X)\r\n", csd->csd_v2._res1, csd->csd_v2._res1);
		printf("csd->TAAC               [119:112] : %d(0x%08X)\r\n", csd->csd_v2.TAAC, csd->csd_v2.TAAC);
		printf("csd->NSAC               [111:104] : %d(0x%08X)\r\n", csd->csd_v2.NSAC, csd->csd_v2.NSAC);
		printf("csd->TRAN_SPEED         [103: 96] : %d(0x%08X)\r\n", csd->csd_v2.TRAN_SPEED, csd->csd_v2.TRAN_SPEED);
		printf("csd->CCC                [ 95: 84] : %d(0x%08X)\r\n", csd->csd_v2.CCC, csd->csd_v2.CCC);
		printf("csd->READ_BL_LEN        [ 83: 80] : %d(0x%08X)\r\n", csd->csd_v2.READ_BL_LEN, csd->csd_v2.READ_BL_LEN);
		printf("csd->READ_BL_PARTIAL    [ 79: 79] : %d(0x%08X)\r\n", csd->csd_v2.READ_BL_PARTIAL, csd->csd_v2.READ_BL_PARTIAL);
		printf("csd->WRITE_BLK_MISALIGN [ 78: 78] : %d(0x%08X)\r\n", csd->csd_v2.WRITE_BLK_MISALIGN, csd->csd_v2.WRITE_BLK_MISALIGN);
		printf("csd->READ_BLK_MISALIGN  [ 77: 77] : %d(0x%08X)\r\n", csd->csd_v2.READ_BLK_MISALIGN, csd->csd_v2.READ_BLK_MISALIGN);
		printf("csd->DSR_IMP            [ 76: 76] : %d(0x%08X)\r\n", csd->csd_v2.DSR_IMP, csd->csd_v2.DSR_IMP);
		printf("csd->_res2              [ 75: 70] : %d(0x%08X)\r\n", csd->csd_v2._res2, csd->csd_v2._res2);
		printf("csd->C_SIZE             [ 69: 48] : %d(0x%08X)\r\n", csd->csd_v2.C_SIZE, csd->csd_v2.C_SIZE);
		printf("csd->_res3              [ 47: 47] : %d(0x%08X)\r\n", csd->csd_v2._res3, csd->csd_v2._res3);
		printf("csd->ERASE_BLK_EN       [ 46: 46] : %d(0x%08X)\r\n", csd->csd_v2.ERASE_BLK_EN, csd->csd_v2.ERASE_BLK_EN);
		printf("csd->SECTOR_SIZE        [ 45: 39] : %d(0x%08X)\r\n", csd->csd_v2.SECTOR_SIZE, csd->csd_v2.SECTOR_SIZE);
		printf("csd->WP_GRP_SIZE        [ 38: 32] : %d(0x%08X)\r\n", csd->csd_v2.WP_GRP_SIZE, csd->csd_v2.WP_GRP_SIZE);
		printf("csd->WP_GRP_ENABLE      [ 31: 31] : %d(0x%08X)\r\n", csd->csd_v2.WP_GRP_ENABLE, csd->csd_v2.WP_GRP_ENABLE);
		printf("csd->_res4              [ 30: 29] : %d(0x%08X)\r\n", csd->csd_v2._res4, csd->csd_v2._res4);
		printf("csd->R2W_FACTOR         [ 28: 26] : %d(0x%08X)\r\n", csd->csd_v2.R2W_FACTOR, csd->csd_v2.R2W_FACTOR);
		printf("csd->WRITE_BL_LEN       [ 25: 22] : %d(0x%08X)\r\n", csd->csd_v2.WRITE_BL_LEN, csd->csd_v2.WRITE_BL_LEN);
		printf("csd->WRITE_BL_PARTIAL   [ 21: 19] : %d(0x%08X)\r\n", csd->csd_v2.WRITE_BL_PARTIAL, csd->csd_v2.WRITE_BL_PARTIAL);
		printf("csd->_res5              [ 20: 16] : %d(0x%08X)\r\n", csd->csd_v2._res5, csd->csd_v2._res5);
		printf("csd->FILE_FORMAT_GRP    [ 15: 15] : %d(0x%08X)\r\n", csd->csd_v2.FILE_FORMAT_GRP, csd->csd_v2.FILE_FORMAT_GRP);
		printf("csd->COPY               [ 14: 14] : %d(0x%08X)\r\n", csd->csd_v2.COPY, csd->csd_v2.COPY);
		printf("csd->PERM_WRITE_PROTECT [ 13: 13] : %d(0x%08X)\r\n", csd->csd_v2.PERM_WRITE_PROTECT, csd->csd_v2.PERM_WRITE_PROTECT);
		printf("csd->TMP_WRITE_PROTECT  [ 12: 12] : %d(0x%08X)\r\n", csd->csd_v2.TMP_WRITE_PROTECT, csd->csd_v2.TMP_WRITE_PROTECT);
		printf("csd->FILE_FORMAT        [ 11: 10] : %d(0x%08X)\r\n", csd->csd_v2.FILE_FORMAT, csd->csd_v2.FILE_FORMAT);
		printf("csd->_res6              [  9:  8] : %d(0x%08X)\r\n", csd->csd_v2._res6, csd->csd_v2._res6);
		printf("csd->CRC                [  7:  1] : %d(0x%08X)\r\n", csd->csd_v2.CRC, csd->csd_v2.CRC);
		printf("csd->_res7              [  0:  0] : %d(0x%08X)\r\n", csd->csd_v2._res7, csd->csd_v2._res7);
	}
	else
	{
		printf("(%04d)%s : CSD Version Check Error\r\n", __LINE__, __func__);
	}
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_SSR(SD_SSR *ssr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("ssr->DAT_BUS_WIDTH          [511:510] : %d(0x%08X)\r\n", ssr->DAT_BUS_WIDTH, ssr->DAT_BUS_WIDTH);
	printf("ssr->SECURED_MODE           [509:509] : %d(0x%08X)\r\n", ssr->SECURED_MODE, ssr->SECURED_MODE);
	printf("ssr->RSF                    [508:502] : %d(0x%08X)\r\n", ssr->RSF, ssr->RSF);
	printf("ssr->_res1                  [501:496] : %d(0x%08X)\r\n", ssr->_res1, ssr->_res1);
	printf("ssr->SD_CARD_TYPE           [495:480] : %d(0x%08X)\r\n", ssr->SD_CARD_TYPE, ssr->SD_CARD_TYPE);
	printf("ssr->SIZE_OF_PROTECTED_AREA [479:448] : %d(0x%08X)\r\n", ssr->SIZE_OF_PROTECTED_AREA, ssr->SIZE_OF_PROTECTED_AREA);
	printf("ssr->SPEED_CLASS            [447:440] : %d(0x%08X)\r\n", ssr->SPEED_CLASS, ssr->SPEED_CLASS);
	printf("ssr->PERFORMANCE_MOVE       [439:432] : %d(0x%08X)\r\n", ssr->PERFORMANCE_MOVE, ssr->PERFORMANCE_MOVE);
	printf("ssr->AU_SIZE                [431:428] : %d(0x%08X)\r\n", ssr->AU_SIZE, ssr->AU_SIZE);
	printf("ssr->_res2                  [427:424] : %d(0x%08X)\r\n", ssr->_res2, ssr->_res2);
	printf("ssr->ERASE_SIZE             [423:408] : %d(0x%08X)\r\n", ssr->ERASE_SIZE, ssr->ERASE_SIZE);
	printf("ssr->ERASE_TIMEOUT          [407:402] : %d(0x%08X)\r\n", ssr->ERASE_TIMEOUT, ssr->ERASE_TIMEOUT);
	printf("ssr->ERASE_OFFSET           [401:400] : %d(0x%08X)\r\n", ssr->ERASE_OFFSET, ssr->ERASE_OFFSET);
	printf("ssr->UHS_SPEED_GRADE        [399:396] : %d(0x%08X)\r\n", ssr->UHS_SPEED_GRADE, ssr->UHS_SPEED_GRADE);
	printf("ssr->UHS_AU_SIZE            [395:392] : %d(0x%08X)\r\n", ssr->UHS_AU_SIZE, ssr->UHS_AU_SIZE);
	printf("ssr->_res3                  [391:384] : %d(0x%08X)\r\n", ssr->_res3, ssr->_res3);
	printf("ssr->_res4                  [383:352] : %d(0x%08X)\r\n", ssr->_res4, ssr->_res4);
	printf("ssr->_res5                  [351:320] : %d(0x%08X)\r\n", ssr->_res5, ssr->_res5);
	printf("ssr->_res6                  [319:288] : %d(0x%08X)\r\n", ssr->_res6, ssr->_res6);
	printf("ssr->_res7                  [287:256] : %d(0x%08X)\r\n", ssr->_res7, ssr->_res7);
	printf("ssr->_res8                  [255:224] : %d(0x%08X)\r\n", ssr->_res8, ssr->_res8);
	printf("ssr->_res9                  [223:192] : %d(0x%08X)\r\n", ssr->_res9, ssr->_res9);
	printf("ssr->_res10                 [191:160] : %d(0x%08X)\r\n", ssr->_res10, ssr->_res10);
	printf("ssr->_res11                 [159:128] : %d(0x%08X)\r\n", ssr->_res11, ssr->_res11);
	printf("ssr->_res12                 [127: 96] : %d(0x%08X)\r\n", ssr->_res12, ssr->_res12);
	printf("ssr->_res13                 [ 95: 64] : %d(0x%08X)\r\n", ssr->_res13, ssr->_res13);
	printf("ssr->_res14                 [ 63: 32] : %d(0x%08X)\r\n", ssr->_res14, ssr->_res14);
	printf("ssr->_res15                 [ 31:  0] : %d(0x%08X)\r\n", ssr->_res15, ssr->_res15);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_SCR(SD_SCRreg *scr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("scr->SCR_STRUCTURE         [63:60] : %d(0x%08X)\r\n", scr->SCR_STRUCTURE, scr->SCR_STRUCTURE);
	printf("scr->SD_SPEC               [59:56] : %d(0x%08X)\r\n", scr->SD_SPEC, scr->SD_SPEC);
	printf("scr->DATA_STAT_AFTER_ERASE [55:55] : %d(0x%08X)\r\n", scr->DATA_STAT_AFTER_ERASE, scr->DATA_STAT_AFTER_ERASE);
	printf("scr->SD_SECURITY           [54:52] : %d(0x%08X)\r\n", scr->SD_SECURITY, scr->SD_SECURITY);
	printf("scr->SD_BUS_WIDTHS         [51:48] : %d(0x%08X)\r\n", scr->SD_BUS_WIDTHS, scr->SD_BUS_WIDTHS);
	printf("scr->SD_SPEC3              [47:47] : %d(0x%08X)\r\n", scr->SD_SPEC3, scr->SD_SPEC3);
	printf("scr->EX_SECURITY           [46:43] : %d(0x%08X)\r\n", scr->EX_SECURITY, scr->EX_SECURITY);
	printf("scr->_res1                 [42:34] : %d(0x%08X)\r\n", scr->_res1, scr->_res1);
	printf("scr->CMD_SUPPORT           [33:32] : %d(0x%08X)\r\n", scr->CMD_SUPPORT, scr->CMD_SUPPORT);
	printf("scr->_res2                 [31: 0] : %d(0x%08X)\r\n", scr->_res2, scr->_res2);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

UINT Sdio1GetActive(void)
{
	return sdinfo.active;
}

UINT Sdio1GetSectorCnt(void)
{
	SDIO_CMD_LOG_START;

	UINT nSector = 0;

	if(sdinfo.csd.csd_v1.CSD_STRUCTURE == 1)	// CSD 2.0
	{
		nSector = (sdinfo.csd.csd_v2.C_SIZE + 1) << 10;
		printf("CSD2.0 : C_SIZE(%d)\r\n", sdinfo.csd.csd_v2.C_SIZE);
		printf("         SECTOR(%d)\r\n", nSector);
	}
	else if(sdinfo.csd.csd_v1.CSD_STRUCTURE == 0)	// CSD 1.0
	{
		nSector = ((sdinfo.csd.csd_v1.C_SIZE + 1) * (0x1 << (sdinfo.csd.csd_v1.C_SIZE_MULT + 2)) * (0x1 << (sdinfo.csd.csd_v1.READ_BL_LEN))) >> 9;
		printf("CSD1.0 : C_SIZE(%d)\r\n", sdinfo.csd.csd_v1.C_SIZE);
		printf("         C_SIZE_MULT(%d)\r\n", sdinfo.csd.csd_v1.C_SIZE_MULT);
		printf("         BLOCK_LEN(%d)\r\n", sdinfo.csd.csd_v1.READ_BL_LEN);
		printf("         SECTOR(%d)\r\n", nSector);
	}
	else
	{
		printf("CSD Version error(%d)\r\n", sdinfo.csd.csd_v1.CSD_STRUCTURE);
	}

	SDIO_CMD_LOG_END;

	return nSector;
}

UINT Sdio1GetAUSize(void)
{
	return sdinfo.ssr.AU_SIZE;
}

static BOOL Sdio1GetRCA(void)
{	// CMD3(R6)
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio1Cmd( 3, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[ 3] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	sdinfo.rca = nResp & RCA_RCA_MASK;

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1GetCID_2(void)
{	// CMD2(R2)
	BOOL bRes = SDIO_FAIL;
	UINT *pResp;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.cid;

	bRes = Sdio1Cmd( 2, 0x00000000, 1, 1, 0, 0);
	pResp[0] = SDIO1_RESP0;
	gprintf("[ 2] res(%d) RESP0(0x%08X)\r\n", bRes, pResp[0]);
	pResp[1] = SDIO1_RESP1;
	gprintf("[ 2] res(%d) RESP1(0x%08X)\r\n", bRes, pResp[1]);
	pResp[2] = SDIO1_RESP2;
	gprintf("[ 2] res(%d) RESP2(0x%08X)\r\n", bRes, pResp[2]);
	pResp[3] = SDIO1_RESP3;
	gprintf("[ 2] res(%d) RESP3(0x%08X)\r\n", bRes, pResp[3]);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_CID(&sdinfo.cid);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1GetCID_10(void)
{	// CMD10(R2)
	BOOL bRes = SDIO_FAIL;
	UINT *pResp;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.cid;

	bRes = Sdio1Cmd(10, sdinfo.rca, 1, 1, 0, 0);
	pResp[0] = SDIO1_RESP0;
	gprintf("[10] res(%d) RESP0(0x%08X)\r\n", bRes, pResp[0]);
	pResp[1] = SDIO1_RESP1;
	gprintf("[10] res(%d) RESP1(0x%08X)\r\n", bRes, pResp[1]);
	pResp[2] = SDIO1_RESP2;
	gprintf("[10] res(%d) RESP2(0x%08X)\r\n", bRes, pResp[2]);
	pResp[3] = SDIO1_RESP3;
	gprintf("[10] res(%d) RESP3(0x%08X)\r\n", bRes, pResp[3]);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_CID(&sdinfo.cid);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1GetCSD(void)
{	// CMD9(R2)
	BOOL bRes = SDIO_FAIL;
	UINT *pResp;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.csd;

	bRes = Sdio1Cmd( 9, sdinfo.rca, 1, 1, 0, 0);
	pResp[0] = SDIO1_RESP0;
	gprintf("[ 9] res(%d) RESP0(0x%08X)\r\n", bRes, pResp[0]);
	pResp[1] = SDIO1_RESP1;
	gprintf("[ 9] res(%d) RESP1(0x%08X)\r\n", bRes, pResp[1]);
	pResp[2] = SDIO1_RESP2;
	gprintf("[ 9] res(%d) RESP2(0x%08X)\r\n", bRes, pResp[2]);
	pResp[3] = SDIO1_RESP3;
	gprintf("[ 9] res(%d) RESP3(0x%08X)\r\n", bRes, pResp[3]);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_CSD(&sdinfo.csd);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1GetSSR(void)
{	// ACMD13(Data bus response / 64byte)
	BOOL bRes = SDIO_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio1Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	nTemp = SDIO1_DAT_BL;
	SDIO1_DAT_BL = sizeof(SD_SSR);
	SDIO1_DATLEN = 1;

	bRes = Sdio1Cmd(13, 0x00000000, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		while(SDIO1_DAT_EN);

		BYTE *getData = (BYTE *)&sdinfo.ssr;
		DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO1_BASE, sizeof(SD_SSR));
		invalidate_dcache_range((UINT)getData, (UINT)(getData) + sizeof(SD_SSR));
	}
	else
	{
		SDIO1_DAT_EN = 0;
	}

	gprintf("[13] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	
	SDIO1_DAT_BL = nTemp;

#ifdef ENX_SDIO_CMD_DEBUG
	if(bRes == SDIO_OK)
		Sdio_Print_SSR(&sdinfo.ssr);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1GetSCR(void)
{	// ACMD51(Data bus response / 8byte)
	BOOL bRes = SDIO_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio1Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	nTemp = SDIO1_DAT_BL;
	SDIO1_DAT_BL = sizeof(SD_SCRreg);
	SDIO1_DATLEN = 1;

	bRes = Sdio1Cmd(51, 0x00000000, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		while(SDIO1_DAT_EN);

		BYTE *getData = (BYTE *)&sdinfo.scr;
		DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO1_BASE, sizeof(SD_SCRreg));
		invalidate_dcache_range((UINT)getData, (UINT)(getData) + sizeof(SD_SCRreg));
	}
	else
	{
		SDIO1_DAT_EN = 0;
	}

	gprintf("[51] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO1_DAT_BL = nTemp;

#ifdef ENX_SDIO_CMD_DEBUG
	if(bRes == SDIO_OK)
		Sdio_Print_SCR(&sdinfo.scr);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1SwitchVoltage(void)
{	// CMD11 : Switch to 1.8V
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	Sdio1Cmd(11, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[11] res(%d) RESP0(0x%08X)\r\n", bRes, nResp);
	// Switch 1.8V
	///////////////
	///////////////
	///////////////
	///////////////

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1BusWidthChang(void)
{	// ACMD6 : Bus Width 1bit -> 4bit
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	// Select a card
	bRes = Sdio1Cmd( 7, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[ 7] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// Unlock
//	bRes = Sdio1Cmd(42, 0x00000000, 1, 0, 0, 0);
//	nResp = SDIO1_RESP0;
//	printf("%s(%d) : [42] res(%d) RESP(0x%08X)\r\n", __func__, __LINE__, bRes, nResp);

	// ACMD6 : 1bit -> 4bit
	bRes = Sdio1Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	bRes = Sdio1Cmd( 6, 0x00000002, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[ 6] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1ClockSwitch(int mode, int group, BYTE value, BYTE *getData)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nArg = 0, nTemp = 0;
	UINT nSize = 64;

	SDIO_CMD_LOG_START;

	nArg = mode << 31 | 0x00FFFFFF;
	nArg &= ~(0xFF << (group * 4));
	nArg |= value << (group * 4);

	printf("nArg(mode:%d) : 0x%08X\r\n", mode, nArg);

	nTemp = SDIO1_DAT_BL;
	SDIO1_DAT_BL = nSize;
	SDIO1_DATLEN = 1;

	bRes = Sdio1Cmd( 6, nArg, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		while(SDIO1_DAT_EN);

		if(SDIO1_DAT_CRCERR)
		{
			gprintf("CRC Error\r\n");
			gprintf("[ 6] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
//		else
		{
			DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO1_BASE, nSize);
			invalidate_dcache_range((UINT)getData, ((UINT)getData) + nSize);
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
	}

	gprintf("[ 6] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO1_DAT_BL = nTemp;

	SDIO_CMD_LOG_END;

	return bRes;
}

BOOL Sdio1SetClock(void)
{	// CMD6 : Check & Switch Clock
	BOOL bRes = SDIO_FAIL;
	BYTE status[64];

	SDIO_CMD_LOG_START;

	if(sdinfo.ssr.SPEED_CLASS >= SD_SPEED_CLASS10)
	{
		if(sdinfo.scr.SD_SPEC < SCR_SPEC_VER_1)
		{
			printf("spec ver 1.0 or 1.01\r\n");
			goto if_speed;
		}

		if(!(sdinfo.csd.csd_v2.CCC & SCR_CCC_SWITCH))
		{
			printf("card lacks mandatory switch function, performance might suffer.\r\n");
			goto if_speed;
		}

		if(Sdio1ClockSwitch(0, 0, 1, status) == SDIO_FAIL)
		{
			printf("problem reading switch capabilities, performance might suffer.\r\n");
			goto if_speed;
		}

		if(!(status[13] & 0x02))
		{
			goto if_speed;
		}

		if(Sdio1ClockSwitch(1, 0, 1, status) == SDIO_FAIL)
		{
			goto if_speed;
		}

		if((status[16] & 0x0F) != 1)
		{
			printf("Problem switching card into high-speed mode!\r\n");
		}
		else
		{
			printf("(%04d)%s : Set High speed mode\r\n", __LINE__, __func__);
//			Sdio1ClockDiv(eSDIO_CLK_10MHz);
			Sdio1ClockDiv(eSDIO_CLK_50MHz);
		}

		bRes = SDIO_OK;
	}
	else
	{
if_speed:
		printf("(%04d)%s : Set Default speed mode\r\n", __LINE__, __func__);
		Sdio1ClockDiv(eSDIO_CLK_25MHz);
		bRes = SDIO_OK;
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

BOOL Sdio1SetBlockLength(void)
{	// CMD16 : Define the block length
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio1Cmd(16, SDIO1_DAT_BL, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	gprintf("[16] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio1Initialization(void)
{	// ACMD41(R3)
	UINT *pResp;
	UINT nResp, nTryCnt = 0;
	BOOL bRes;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.ocr;

	do{
		if(++nTryCnt > SD_TRY_CNT)
		{
			eprintf("End(FAIL) : RESP(0x%08X)\r\n", *pResp);
			return SDIO_FAIL;
		}

		bRes = Sdio1Cmd(55, 0x00000000, 1, 0, 0, 0);
		nResp = SDIO1_RESP0;
		gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
		bRes = Sdio1Cmd(41, ACMD41_HCS|ACMD41_XPC|ACMD41_VOLT
#ifdef sdio_support_1_8v
			|ACMD41_S18R
#endif
			, 1, 0, 0, 0);	//	ACMD41
		*pResp = SDIO1_RESP0;
		gprintf("[41] res(%d) RESP(0x%08X)\r\n", bRes, *pResp);
	}while(sdinfo.ocr.busy == 0);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_OCR(&sdinfo.ocr);
#endif

	SDIO_CMD_LOG_END;

	return SDIO_OK;
}

static BOOL Sdio1Reset(void)
{	// CMD0, 8 : Power on(Reset)
	UINT nResp;
	BOOL bRes;

	SDIO_CMD_LOG_START;

	bRes = Sdio1Cmd( 0, 0x00000000, 0, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", bRes);

	bRes = Sdio1Cmd( 0, 0x00000000, 0, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", bRes);

	bRes = Sdio1Cmd( 8, CMD8_VHS|CMD8_PATTERN, 1, 0, 0, 0);	//	Send IF Condition
	nResp = SDIO1_RESP0;

	gprintf("[ 8] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	if(nResp & 0x300)
	{
		SDIO_CMD_LOG_END;
		return SDIO_OK;
	}
	else
	{
		eprintf("End(FAIL) : RESP(0x%08X)\r\n", nResp);
		SDIO_CMD_LOG_END;
		return SDIO_FAIL;
	}
}

BYTE Sdio1InitProcess(void)
{
	SDIO_CMD_LOG_START;
	UINT errCode = 0;

	memset(&sdinfo, 0, sizeof(sdinfo));

	// CMD 0, 8
	if(Sdio1Reset() == SDIO_FAIL)
	{
		errCode = 1;
		goto done_fail;
	}

	// ACMD 41
	if(Sdio1Initialization() == SDIO_FAIL)
	{
		errCode = 2;
		goto done_fail;
	}

	if(sdinfo.ocr.VDD18)
	{
		// CMD 11
		if(Sdio1SwitchVoltage() == SDIO_FAIL)
		{
			errCode = 3;
			eprintf("3.3v -> 1.8v fail\r\n");
		}
	}

	// CMD 2
	if(Sdio1GetCID_2() == SDIO_FAIL)
	{
		errCode = 4;
		goto done_fail;
	}

	// CMD 3
	if(Sdio1GetRCA() == SDIO_FAIL)
	{
		errCode = 5;
		goto done_fail;
	}

	// CMD 9
	if(Sdio1GetCSD() == SDIO_FAIL)
	{
		errCode = 6;
		goto done_fail;
	}

	// CMD 10
	if(Sdio1GetCID_10() == SDIO_FAIL)
	{
		errCode = 7;
		goto done_fail;
	}

	// CMD 7, 42, ACMD 6
	if(Sdio1BusWidthChang() == SDIO_FAIL)
	{
		errCode = 8;
		goto done_fail;
	}

	// ACMD 13
	if(Sdio1GetSSR() == SDIO_FAIL)
	{
		errCode = 9;
		goto done_fail;
	}

	// ACMD 51
	if(Sdio1GetSCR() == SDIO_FAIL)
	{
		errCode = 10;
		goto done_fail;
	}

	// CMD 6
	if(Sdio1SetClock() == SDIO_FAIL)
	{
		errCode = 11;
		goto done_fail;
	}

	// CMD 16
	if(Sdio1SetBlockLength() == SDIO_FAIL)
	{
		errCode = 12;
		goto done_fail;
	}

	sdinfo.active = 1;

	printf("(%04d)%s : SDIO Init OK\r\n", __LINE__, __func__);

	SDIO_CMD_LOG_END;
	return SDIO_OK;
done_fail:

	printf("(%04d)%s : SDIO Init Fail(err:%d)\r\n", __LINE__, __func__, errCode);

	SDIO_CMD_LOG_END;
	return SDIO_FAIL;
}
#endif

void Sdio1PadVoltage(int voltage)
{
	if(voltage == SDIO_1V8)
		PAD_SDIO1_MSEL = 0x3F;	// 1.8v
	else	// etc SDIO_3V3
		PAD_SDIO1_MSEL = 0x00;	// 3.3v
}

void Sdio1Init(void)
{
	SDIO_CMD_LOG_START;

//	memset(&sdinfo, 0, sizeof(sdinfo));

	Sdio1DiCmd();	// Command interrupt disable
	Sdio1DiDat();	// Data interrupt disable
	Sdio1EiCip();	// Chip interrupt enable

	PAD_SDIO1_CMD_DS = 1;
	PAD_SDIO1_CLK_DS = 1;
	PAD_SDIO1_DAT_DS = 1;

	PAD_SDIO1_CMD_PU = 0;
	PAD_SDIO1_DAT_PU = 0;

	SDIO1_EN = 1;
	Sdio1Cfg(eSDIO_CLK_400KHz, 0x100, 1);
	SDIO1_CLKEN = 1;

	SDIO_CMD_LOG_END;
}

BYTE Sdio1RegW(BYTE data, UINT func, UINT addr)
{
	BOOL bRes = SDIO_FAIL;
 	UINT nResp = 0, nArg = 0;

//	SDIO_CMD_LOG_START;

	nArg = CMD52_WRITE | (func<<28) | (addr<<9) | data;
	bRes = Sdio1Cmd(52, nArg, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	if(bRes == SDIO_FAIL)
		gprintf("[52W] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

//	SDIO_CMD_LOG_END;

	return bRes;
}

BYTE Sdio1RegR(BYTE *data, UINT func, UINT addr)
{
	BOOL bRes = SDIO_FAIL;
 	UINT nResp = 0, nArg = 0;

//	SDIO_CMD_LOG_START;

	nArg = CMD52_READ | (func<<28) | (addr<<9);
	bRes = Sdio1Cmd(52, nArg, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	if(bRes == SDIO_FAIL)
		gprintf("[52R] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	if(bRes == SDIO_OK)
	{
		*data = nResp & 0xFF;
	}

//	SDIO_CMD_LOG_END;

	return bRes;
}

#if 0
BYTE Sdio1E(UINT start_sctor, UINT end_sctor)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp;
	UINT nBlocSp, nBlocEp;

	SDIO_CMD_LOG_START;

	if(sdinfo.ocr.CCS) {
		nBlocSp = start_sctor;
		nBlocEp = end_sctor;
	} else {
		nBlocSp = start_sctor << 9;
		nBlocEp = end_sctor << 9;
	}

	// CMD32 : Erase start address
	bRes = Sdio1Cmd(32, nBlocSp, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	tprintf("[32] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// CMD33 : Erase end   address
	bRes = Sdio1Cmd(33, nBlocEp, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	tprintf("[33] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// CMD38 : Erase command
	bRes = Sdio1Cmd(38, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO1_RESP0;
	tprintf("[38] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

BYTE Sdio1SectorW(const BYTE *buff, UINT sector)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nAddr = 0;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = 1;

	bRes = Sdio1Cmd(24, nAddr, 1, 0, 1, 1);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		DmaMemCpy_ip((BYTE *)SDIO1_BASE, (BYTE *)buff, SDIO1_DAT_BL);

		while(SDIO1_DAT_EN);
		SDIO1_DAT_WE = 0;

		if(SDIO1_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
		SDIO1_DAT_WE = 0;
		tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

//	tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

//	SDIO_DAT_LOG_END;

	return bRes;
}

// tot_count : 총 count
// now_count : 현재까지 작업한 count
// return : write 해야할 섹터 수
#if 0
static UINT SdCardBlockIO(UINT tot_count, UINT now_count)
{
	if((now_count + 4) <= tot_count)	return 4;
	else								return (tot_count - now_count);
}
#endif

BYTE Sdio1W(BYTE *buff, UINT sector, UINT count)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nAddr = 0, i;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = count;

	bRes = Sdio1Cmd(25, nAddr, 1, 0, 1, 1);
	nResp = SDIO1_RESP0;

//	UINT write_cnt;

	if(bRes == SDIO_OK)
	{
		for(i=0;i<count;i++)
		{
			while(SDIO1_DAT_FULL)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				tprintf("CRC Error\r\n");
				tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_ip((BYTE *)SDIO1_BASE, buff, SDIO1_DAT_BL);
			buff += SDIO1_DAT_BL;
		}

		while(SDIO1_DAT_EN);
		SDIO1_DAT_WE = 0;

		if(SDIO1_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
		SDIO1_DAT_WE = 0;
		tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

//	tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

	if(bRes == SDIO_OK)
	{
		bRes = Sdio1Cmd(12, 0, 1, 0, 0, 0);
		nResp = SDIO1_RESP0;
//		while(SDIO1_BUSY);
//		tprintf("[12] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	}

//	SDIO_DAT_LOG_END;

	return bRes;
}

BYTE Sdio1SectorR(const BYTE *buff, UINT sector)
{
	BOOL bRes = SDIO_FAIL;
 	UINT nResp = 0, nAddr = 0;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = 1;

	bRes = Sdio1Cmd(17, nAddr, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		while(SDIO1_DAT_EN);

		if(SDIO1_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
//		else
		{
			DmaMemCpy_ip((BYTE *)buff, (BYTE *)SDIO1_BASE, SDIO1_DAT_BL);
			invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO1_DAT_BL);
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
	}

//	tprintf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

//	SDIO_DAT_LOG_END;

	return bRes;
}

BYTE Sdio1R(BYTE *buff, UINT sector, UINT count)
{
	BOOL bRes = SDIO_FAIL;
	UINT nResp = 0, nAddr = 0, i;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = count;

	bRes = Sdio1Cmd(18, nAddr, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == SDIO_OK)
	{
		for(i=0;i<count;i++)
		{
			while(SDIO1_DAT_EMPTY)
				NOP();

			if(SDIO1_DAT_CRCERR)
			{
				tprintf("CRC Error\r\n");
				tprintf("[18] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_ip(buff, (BYTE *)SDIO1_BASE, SDIO1_DAT_BL);
			invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO1_DAT_BL);
			buff += SDIO1_DAT_BL;
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
	}

//	tprintf("[18] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

	if(bRes == SDIO_OK)
	{
		bRes = Sdio1Cmd(12, 0, 1, 0, 0, 0);
		nResp = SDIO1_RESP0;
//		tprintf("[12] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	}

//	SDIO_DAT_LOG_END;

	return bRes;
}

UINT Sdio1GetInstall(void)
{
	return SDIO1_DET;
}

void IsrSdio1(void *dummy)
{
	if(SDIO1_CHIP_IRQ)
	{
		SDIO1_CHIP_IRQ_CLR = 1;
		printf("%s(%d) : chip irq call\r\n", __func__, __LINE__);
	}
	else
	{
		SDIO1_DET_IRQ_CLR = 1;
		SDIO1_DAT_IRQ_CLR = 1;
		SDIO1_CMD_IRQ_CLR = 1;
		printf("%s(%d) : ? irq call\r\n", __func__, __LINE__);
	}
	printf("%s(%d) : DET(%d) CHIP(%d) DAT(%d) CMD(%d)\r\n",
		SDIO1_DET_IRQ, SDIO1_CHIP_IRQ, SDIO1_DAT_IRQ, SDIO1_CMD_IRQ);

	UNUSED(dummy);
}
#endif
#endif
