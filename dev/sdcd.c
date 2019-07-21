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
#include "dev_inline.h"

#include "portmacro.h"
extern void vTaskDelay( portTickType xTicksToDelay );

#ifdef __DEVICE_SD__

//#define __SDIO_IRQ__
//#define sdio_support_1_8v

//#define ENX_SDIO_CMD_DEBUG
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

#define SDIO0_DETECT_CHECK	if(SDIO0_DET)					\
							{								\
								eprintf("no detect\r\n");	\
								return DEF_FAIL;			\
							}

//*************************************************************************************************
// Reg
//-------------------------------------------------------------------------------------------------
//
typedef struct {
	_SDIO_CONT SDIO_CONT;
	_SDIO_CMD SDIO_CMD;
	_SDIO_CMDRESP SDIO_CMDRESP;
	_SDIO_CMDARG SDIO_CMDARG;
	_SDIO_RESPDAT SDIO_RESPDAT0;
	_SDIO_RESPDAT SDIO_RESPDAT1;
	_SDIO_RESPDAT SDIO_RESPDAT2;
	_SDIO_RESPDAT SDIO_RESPDAT3;
	_SDIO_DATLEN SDIO_DATLEN;
	_SDIO_CMD_TIMEOUT SDIO_CMD_TIMEOUT;
	_SDIO_DAT_TIMEOUT SDIO_DAT_TIMEOUT;
}SDIO_REG;

static SDIO_REG *_sdioreg;

//*************************************************************************************************
// Phy
//-------------------------------------------------------------------------------------------------
//
//void Sdio0EiDet(void) { SDIO0_DET_IRQ_EN = 1; } // Detect interrupt enable
//void Sdio0DiDet(void) { SDIO0_DET_IRQ_EN = 0; } // Detect interrupt disable
//void Sdio0EiCmd(void) { SDIO0_CMD_IRQ_EN = 1; } // Command interrupt enable
//void Sdio0DiCmd(void) { SDIO0_CMD_IRQ_EN = 0; } // Command interrupt disable
//void Sdio0EiDat(void) { SDIO0_DAT_IRQ_EN = 1; } // Data interrupt enable
//void Sdio0DiDat(void) { SDIO0_DAT_IRQ_EN = 0; } // Data interrupt disable
//void Sdio0EiCip(void) { SDIO0_CHIP_IRQ_EN = 1; } // Chip interrupt enable
//void Sdio0DiCip(void) { SDIO0_CHIP_IRQ_EN = 0; } // Chip interrupt disable

typedef enum {
	R1,
	R1b,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7
}CmdRespType;

//-------------------------------------------------------------------------------------------------
//
BOOL Sdio0Cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe)
{
	if(Cmd == 12 || Cmd == 17 || Cmd == 18 || Cmd == 25 || Cmd == 24)
	{
		NOP();
		NOP();
		NOP();
		NOP();
		NOP();
	}
	else
		vTaskDelay(1);

	SDIO0_DETECT_CHECK;

	if(Cmd != 12)
	{
		while(SDIO0_BUSY | SDIO0_CMD_EN | SDIO0_DAT_EN)
			vTaskDelay(0);
	}
	else
	{
		while(SDIO0_CMD_EN | SDIO0_DAT_EN)
			vTaskDelay(0);
	}
//	printf("%s(%d) : BUSY(%d) CMD_EN(%d) DAT_EN(%d)\r\n", __func__, __LINE__, SDIO0_BUSY, SDIO0_CMD_EN, SDIO0_DAT_EN);

	// R1b resp type
//	if(Cmd == 7 || Cmd == 12 || Cmd == 20 || Cmd == 28 || Cmd == 29 || Cmd == 38)
//		SDIO0_CMD_BUSY_EN = 1;

	SDIO0_CMD_IDX = Cmd;
	SDIO0_CMD_ARG = Arg;
	SDIO0_CMD_RESPEN = RespEn;
	SDIO0_CMD_RESPTYPE = RespType;
	SDIO0_DAT_WE = DatWe;
	SDIO0_DAT_EN = DatEn;
	SDIO0_CMD_EN = 1;

	while(SDIO0_CMD_EN)
	{
		SDIO0_DETECT_CHECK;
	}

//	while(SDIO0_BUSY);
//		printf("%s(%d) : BUSY(%d) CMD_EN(%d) DAT_EN(%d)\r\n", __func__, __LINE__, SDIO0_BUSY, SDIO0_CMD_EN, SDIO0_DAT_EN);

	// R2 resp type(do not check CRCERR)
	// R3 resp type(do net check CRCERR)
	if(Cmd == 2 || Cmd == 9 || Cmd == 10 || Cmd == 41)
	{
		if(SDIO0_CMD_RESPTOUT)
		{
			eprintf("CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO0_CMD_CRCERR, SDIO0_CMD_RESP_CRC, SDIO0_CMD_RESPTOUT);
			return DEF_FAIL;
		}
	}
	else
	{
		if(SDIO0_CMD_CRCERR | SDIO0_CMD_RESPTOUT)
		{
			eprintf("CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO0_CMD_CRCERR, SDIO0_CMD_RESP_CRC, SDIO0_CMD_RESPTOUT);
			return DEF_FAIL;
		}
	}

	return DEF_OK;
}

void Sdio0ClockDiv(SDIO_CLK nSetKHz)
{
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
	eprintf("%sHz(CLK_DIV:%d)\r\n", strHzValue, nSetKHz);
	SDIO0_CLK_DIV = nSetKHz;
}

//-------------------------------------------------------------------------------------------------
// CMD8
#define CMD8_VHS			0x00000100		// [11: 8] : 2.7-3.6V
#define CMD8_PATTERN		0x000000aa		// [ 7: 0] : Just "0xaa"

// ACMD41
#define	ACMD41_HCS			0x40000000		// [   30] : SDHC or SDXC Supported
#define ACMD41_XPC			0x10000000		// [   28] : SDXC Maximum Performance
#define ACMD41_S18R			0x01000000		// [   24] : Switching to 1.8V Request
#define	ACMD41_VOLT			0x00ff8000		// [23:15] : Voltage window OCR

// OCR register
#define OCR_POWUP			0x80000000		// Power up procedure end
#define OCR_CCS				0x40000000		// Card Capacity Status(0:SDSC, 1:SDHCorSDXC)
#define OCR_UHSII			0x20000000		// UHS-II Card Status(0:Non, 1:UHS-II)
#define OCR_S18A			0x01000000		// Switching to 1.8V Accepted
#define OCR_VOLT			0x00ff8000		// Voltage window

// SCR register
#define SCR_SPEC_VER_0		0	/* Implements system specification 1.0 - 1.01 */
#define SCR_SPEC_VER_1		1	/* Implements system specification 1.10 */
#define SCR_SPEC_VER_2		2	/* Implements system specification 2.00 */

#define SCR_CCC_SWITCH		0x00000400		// [  :10] : High speed switch

// Status Mask
#define RCA_RCA_MASK		0xffff0000

// Sd power up retry count
#define	SD_TRY_CNT			100

#define bSD_CMD_ERR_CMD		0x4
#define bSD_CMD_ERR_CRC		0x2
#define bSD_CMD_ERR_TOUT	0x1

enum {
	SD_SPEED_CLASS0,
	SD_SPEED_CLASS2,
	SD_SPEED_CLASS4,
	SD_SPEED_CLASS6,
	SD_SPEED_CLASS10,
	SD_SPEED_REV
};

typedef struct {
	uint32 DAT_BUS_WIDTH				:2;
	uint32 SECURED_MODE 				:1;
	uint32 RSF							:7;
	uint32 _res1						:6;
	uint32 SD_CARD_TYPE 				:16;
	uint32 SIZE_OF_PROTECTED_AREA;
	uint32 SPEED_CLASS					:8;
	uint32 PERFORMANCE_MOVE 			:8;
	uint32 AU_SIZE						:4;
	uint32 _res2						:4;
	uint32 ERASE_SIZE					:16;
	uint32 ERASE_TIMEOUT				:6;
	uint32 ERASE_OFFSET 				:2;
	uint32 UHS_SPEED_GRADE				:4;
	uint32 UHS_AU_SIZE					:4;
	uint32 _res3						:8;
	uint32 _res4;
	uint32 _res5;
	uint32 _res6;
	uint32 _res7;
	uint32 _res8;
	uint32 _res9;
	uint32 _res10;
	uint32 _res11;
	uint32 _res12;
	uint32 _res13;
	uint32 _res14;
	uint32 _res15;
}__attribute__ ((packed)) SD_SSR;

typedef struct {
	uint32 OUT_OF_RANGE 				:1;
	uint32 ADDRESS_ERROR 				:1;
	uint32 BLOCK_LEN_ERROR 				:1;
	uint32 ERASE_SEQ_ERROR				:1;
	uint32 ERASE_PARAM					:1;
	uint32 WP_VIOLATION					:1;
	uint32 CARD_IS_LOCKED				:1;
	uint32 LOCK_UNLOCK_FAILED 			:1;
	uint32 COM_CRC_ERROR				:1;
	uint32 ILLEGAL_COMMAND				:1;
	uint32 CARD_ECC_FAILED 				:1;
	uint32 CC_ERROR						:1;
	uint32 ERROR						:1;
	uint32 _res1 						:1;
	uint32 _res2 						:1;
	uint32 CSD_OVERWRITE	 			:1;
	uint32 WP_ERASE_SKIP				:1;
	uint32 CARD_ECC_DISABLED			:1;
	uint32 ERASE_RESET					:1;
	uint32 CURRENT_STATE				:4;
	uint32 READY_FOR_DATA				:1;
	uint32 _res3						:2;
	uint32 APP_CMD						:1;
	uint32 _res4						:1;
	uint32 AKE_SEQ_ERROR				:1;
	uint32 _res5						:1;
	uint32 _res6						:1;
	uint32 _res7						:1;
}__attribute__ ((packed)) SD_SC;

typedef struct {
	uint32 CSD_STRUCTURE				:2;
	uint32 _res1						:6;
	uint32 TAAC							:8;
	uint32 NSAC							:8;
	uint32 TRAN_SPEED					:8;
	uint32 CCC							:12;
	uint32 READ_BL_LEN					:4;
	uint32 READ_BL_PARTIAL				:1;
	uint32 WRITE_BLK_MISALIGN			:1;
	uint32 READ_BLK_MISALIGN			:1;
	uint32 DSR_IMP						:1;
	uint32 _res2						:2;
	uint32 C_SIZE						:12;
	uint32 VDD_R_CURR_MIN				:3;
	uint32 VDD_R_CURR_MAX				:3;
	uint32 VDD_W_CURR_MIN				:3;
	uint32 VDD_W_CURR_MAX				:3;
	uint32 C_SIZE_MULT					:3;
	uint32 ERASE_BLK_EN					:1;
	uint32 SECTOR_SIZE					:7;
	uint32 WP_GRP_SIZE					:7;
	uint32 WP_GRP_ENABLE				:1;
	uint32 _res3						:2;
	uint32 R2W_FACTOR					:3;
	uint32 WRITE_BL_LEN					:4;
	uint32 WRITE_BL_PARTIAL				:1;
	uint32 _res4						:5;
	uint32 FILE_FORMAT_GRP				:1;
	uint32 COPY							:1;
	uint32 PERM_WRITE_PROTECT			:1;
	uint32 TMP_WRITE_PROTECT 			:1;
	uint32 FILE_FORMAT 					:2;
	uint32 _res5 						:2;
	uint32 CRC 							:7;
	uint32 _res6						:1;
}__attribute__ ((packed)) SD_CSD_v1;

typedef struct {
	uint32 CSD_STRUCTURE				:2;
	uint32 _res1						:6;
	uint32 TAAC							:8;
	uint32 NSAC							:8;
	uint32 TRAN_SPEED					:8;
	uint32 CCC							:12;
	uint32 READ_BL_LEN					:4;
	uint32 READ_BL_PARTIAL				:1;
	uint32 WRITE_BLK_MISALIGN			:1;
	uint32 READ_BLK_MISALIGN			:1;
	uint32 DSR_IMP						:1;
	uint32 _res2						:6;
	uint32 C_SIZE						:22;
	uint32 _res3						:1;
	uint32 ERASE_BLK_EN					:1;
	uint32 SECTOR_SIZE					:7;
	uint32 WP_GRP_SIZE					:7;
	uint32 WP_GRP_ENABLE				:1;
	uint32 _res4						:2;
	uint32 R2W_FACTOR					:3;
	uint32 WRITE_BL_LEN					:4;
	uint32 WRITE_BL_PARTIAL				:1;
	uint32 _res5						:5;
	uint32 FILE_FORMAT_GRP				:1;
	uint32 COPY							:1;
	uint32 PERM_WRITE_PROTECT			:1;
	uint32 TMP_WRITE_PROTECT 			:1;
	uint32 FILE_FORMAT 					:2;
	uint32 _res6 						:2;
	uint32 CRC 							:7;
	uint32 _res7						:1;
}__attribute__ ((packed)) SD_CSD_v2;

typedef union {
	UINT a[4];
	SD_CSD_v1 csd_v1;
	SD_CSD_v2 csd_v2;
}SD_CSD;

typedef struct {
	uint32 busy 						:1;
	uint32 CCS 							:1;
	uint32 UHSIICS 						:1;
	uint32 _res1						:4;
	uint32 VDD18						:1;
	uint32 VDD35_36						:1;
	uint32 VDD34_35						:1;
	uint32 VDD33_34 					:1;
	uint32 VDD32_33						:1;
	uint32 VDD31_32						:1;
	uint32 VDD30_31 					:1;
	uint32 VDD29_30						:1;
	uint32 VDD28_29						:1;
	uint32 VDD27_28 					:1;
	uint32 _res2 						:7;
	uint32 VDDLOW	 					:1;
	uint32 _res3						:7;
}__attribute__ ((packed)) SD_OCR;

typedef struct {
	uint32 MID 							:8;
	uint32 OID							:16;
	uint32 PNM1							:8;
	uint32 PNM2							:8;
	uint32 PNM3							:8;
	uint32 PNM4							:8;
	uint32 PNM5							:8;
	uint32 PRV 							:8;
	uint32 PSN;
	uint32 _res1 						:4;
	uint32 MDT 							:12;
	uint32 CRC 							:7;
	uint32 _res2 						:1;
}__attribute__ ((packed)) SD_CID;

typedef struct {
	uint32 SCR_STRUCTURE 				:4;
	uint32 SD_SPEC						:4;
	uint32 DATA_STAT_AFTER_ERASE		:1;
	uint32 SD_SECURITY					:3;
	uint32 SD_BUS_WIDTHS				:4;
	uint32 SD_SPEC3						:1;
	uint32 EX_SECURITY					:4;
	uint32 _res1						:9;
	uint32 CMD_SUPPORT					:2;
	uint32 _res2;
}__attribute__ ((packed)) SD_SCRreg;

typedef struct {
	UINT active;

	SD_OCR ocr;
	SD_CID cid;
	SD_CSD csd;

	UINT rca;
	// DSR

	SD_SCRreg scr;

	UINT t1[6];

	SD_SSR ssr;
	SD_SC sc;

	UINT voltage_mode;
}SD_STATE;

ISRD SD_STATE sdinfo;
#ifdef __SDIO_IRQ__
static ISPD UINT sdio_read_addr = 0;
static ISPD UINT sdio_write_addr = 0;
#endif

void Sdio_Print_OCR(SD_OCR *ocr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("ocr->busy     [   31] : %u(0x%08X)\r\n", ocr->busy, ocr->busy);
	printf("ocr->CCS      [   30] : %u(0x%08X)\r\n", ocr->CCS, ocr->CCS);
	printf("ocr->UHSIICS  [   29] : %u(0x%08X)\r\n", ocr->UHSIICS, ocr->UHSIICS);
	printf("ocr->_res1    [28:25] : %u(0x%08X)\r\n", ocr->_res1, ocr->_res1);
	printf("ocr->VDD18    [   24] : %u(0x%08X)\r\n", ocr->VDD18, ocr->VDD18);
	printf("ocr->VDD35_36 [   23] : %u(0x%08X)\r\n", ocr->VDD35_36, ocr->VDD35_36);
	printf("ocr->VDD34_35 [   22] : %u(0x%08X)\r\n", ocr->VDD34_35, ocr->VDD34_35);
	printf("ocr->VDD33_34 [   21] : %u(0x%08X)\r\n", ocr->VDD33_34, ocr->VDD33_34);
	printf("ocr->VDD32_33 [   20] : %u(0x%08X)\r\n", ocr->VDD32_33, ocr->VDD32_33);
	printf("ocr->VDD31_32 [   19] : %u(0x%08X)\r\n", ocr->VDD31_32, ocr->VDD31_32);
	printf("ocr->VDD30_31 [   18] : %u(0x%08X)\r\n", ocr->VDD30_31, ocr->VDD30_31);
	printf("ocr->VDD29_30 [   17] : %u(0x%08X)\r\n", ocr->VDD29_30, ocr->VDD29_30);
	printf("ocr->VDD28_29 [   16] : %u(0x%08X)\r\n", ocr->VDD28_29, ocr->VDD28_29);
	printf("ocr->VDD27_28 [   15] : %u(0x%08X)\r\n", ocr->VDD27_28, ocr->VDD27_28);
	printf("ocr->_res2    [14: 8] : %u(0x%08X)\r\n", ocr->_res2, ocr->_res2);
	printf("ocr->VDDLOW   [    7] : %u(0x%08X)\r\n", ocr->VDDLOW, ocr->VDDLOW);
	printf("ocr->_res3    [ 6: 0] : %u(0x%08X)\r\n", ocr->_res3, ocr->_res3);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_CID(SD_CID *cid)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("cid->MID    [127:120] : %u(0x%08X)\r\n", cid->MID, cid->MID);
	printf("cid->OID    [119:104] : %u(0x%08X)\r\n", cid->OID, cid->OID);
	printf("cid->PNM1   [103: 96] : %u(0x%08X) : %c\r\n", cid->PNM1, cid->PNM1, cid->PNM1);
	printf("cid->PNM2     [95:88] : %u(0x%08X) : %c\r\n", cid->PNM2, cid->PNM2, cid->PNM2);
	printf("cid->PNM3     [87:80] : %u(0x%08X) : %c\r\n", cid->PNM3, cid->PNM3, cid->PNM3);
	printf("cid->PNM4     [79:72] : %u(0x%08X) : %c\r\n", cid->PNM4, cid->PNM4, cid->PNM4);
	printf("cid->PNM5     [71:64] : %u(0x%08X) : %c\r\n", cid->PNM5, cid->PNM5, cid->PNM5);
	printf("cid->PRV      [63:56] : %u(0x%08X)\r\n", cid->PRV, cid->PRV);
	printf("cid->PSN      [55:24] : %u(0x%08X)\r\n", cid->PSN, cid->PSN);
	printf("cid->_res1    [23:20] : %u(0x%08X)\r\n", cid->_res1, cid->_res1);
	printf("cid->MDT      [19: 8] : %u(0x%08X)\r\n", cid->MDT, cid->MDT);
	printf("cid->CRC      [ 7: 1] : %u(0x%08X)\r\n", cid->CRC, cid->CRC);
	printf("cid->_res2    [    0] : %u(0x%08X)\r\n", cid->_res2, cid->_res2);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_CSD(SD_CSD *csd)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	if(csd->csd_v1.CSD_STRUCTURE == 0)
	{	// CSD Version 1.0
		printf("csd->CSD_STRUCTURE      [127:126] : %u(0x%08X)\r\n", csd->csd_v1.CSD_STRUCTURE, csd->csd_v1.CSD_STRUCTURE);
	}
	else if(csd->csd_v1.CSD_STRUCTURE == 1)
	{	// CSD Version 2.0
		printf("csd->CSD_STRUCTURE      [127:126] : %u(0x%08X)\r\n", csd->csd_v2.CSD_STRUCTURE, csd->csd_v2.CSD_STRUCTURE);
		printf("csd->_res1              [125:120] : %u(0x%08X)\r\n", csd->csd_v2._res1, csd->csd_v2._res1);
		printf("csd->TAAC               [119:112] : %u(0x%08X)\r\n", csd->csd_v2.TAAC, csd->csd_v2.TAAC);
		printf("csd->NSAC               [111:104] : %u(0x%08X)\r\n", csd->csd_v2.NSAC, csd->csd_v2.NSAC);
		printf("csd->TRAN_SPEED         [103: 96] : %u(0x%08X)\r\n", csd->csd_v2.TRAN_SPEED, csd->csd_v2.TRAN_SPEED);
		printf("csd->CCC                [ 95: 84] : %u(0x%08X)\r\n", csd->csd_v2.CCC, csd->csd_v2.CCC);
		printf("csd->READ_BL_LEN        [ 83: 80] : %u(0x%08X)\r\n", csd->csd_v2.READ_BL_LEN, csd->csd_v2.READ_BL_LEN);
		printf("csd->READ_BL_PARTIAL    [ 79: 79] : %u(0x%08X)\r\n", csd->csd_v2.READ_BL_PARTIAL, csd->csd_v2.READ_BL_PARTIAL);
		printf("csd->WRITE_BLK_MISALIGN [ 78: 78] : %u(0x%08X)\r\n", csd->csd_v2.WRITE_BLK_MISALIGN, csd->csd_v2.WRITE_BLK_MISALIGN);
		printf("csd->READ_BLK_MISALIGN  [ 77: 77] : %u(0x%08X)\r\n", csd->csd_v2.READ_BLK_MISALIGN, csd->csd_v2.READ_BLK_MISALIGN);
		printf("csd->DSR_IMP            [ 76: 76] : %u(0x%08X)\r\n", csd->csd_v2.DSR_IMP, csd->csd_v2.DSR_IMP);
		printf("csd->_res2              [ 75: 70] : %u(0x%08X)\r\n", csd->csd_v2._res2, csd->csd_v2._res2);
		printf("csd->C_SIZE             [ 69: 48] : %u(0x%08X)\r\n", csd->csd_v2.C_SIZE, csd->csd_v2.C_SIZE);
		printf("csd->_res3              [ 47: 47] : %u(0x%08X)\r\n", csd->csd_v2._res3, csd->csd_v2._res3);
		printf("csd->ERASE_BLK_EN       [ 46: 46] : %u(0x%08X)\r\n", csd->csd_v2.ERASE_BLK_EN, csd->csd_v2.ERASE_BLK_EN);
		printf("csd->SECTOR_SIZE        [ 45: 39] : %u(0x%08X)\r\n", csd->csd_v2.SECTOR_SIZE, csd->csd_v2.SECTOR_SIZE);
		printf("csd->WP_GRP_SIZE        [ 38: 32] : %u(0x%08X)\r\n", csd->csd_v2.WP_GRP_SIZE, csd->csd_v2.WP_GRP_SIZE);
		printf("csd->WP_GRP_ENABLE      [ 31: 31] : %u(0x%08X)\r\n", csd->csd_v2.WP_GRP_ENABLE, csd->csd_v2.WP_GRP_ENABLE);
		printf("csd->_res4              [ 30: 29] : %u(0x%08X)\r\n", csd->csd_v2._res4, csd->csd_v2._res4);
		printf("csd->R2W_FACTOR         [ 28: 26] : %u(0x%08X)\r\n", csd->csd_v2.R2W_FACTOR, csd->csd_v2.R2W_FACTOR);
		printf("csd->WRITE_BL_LEN       [ 25: 22] : %u(0x%08X)\r\n", csd->csd_v2.WRITE_BL_LEN, csd->csd_v2.WRITE_BL_LEN);
		printf("csd->WRITE_BL_PARTIAL   [ 21: 19] : %u(0x%08X)\r\n", csd->csd_v2.WRITE_BL_PARTIAL, csd->csd_v2.WRITE_BL_PARTIAL);
		printf("csd->_res5              [ 20: 16] : %u(0x%08X)\r\n", csd->csd_v2._res5, csd->csd_v2._res5);
		printf("csd->FILE_FORMAT_GRP    [ 15: 15] : %u(0x%08X)\r\n", csd->csd_v2.FILE_FORMAT_GRP, csd->csd_v2.FILE_FORMAT_GRP);
		printf("csd->COPY               [ 14: 14] : %u(0x%08X)\r\n", csd->csd_v2.COPY, csd->csd_v2.COPY);
		printf("csd->PERM_WRITE_PROTECT [ 13: 13] : %u(0x%08X)\r\n", csd->csd_v2.PERM_WRITE_PROTECT, csd->csd_v2.PERM_WRITE_PROTECT);
		printf("csd->TMP_WRITE_PROTECT  [ 12: 12] : %u(0x%08X)\r\n", csd->csd_v2.TMP_WRITE_PROTECT, csd->csd_v2.TMP_WRITE_PROTECT);
		printf("csd->FILE_FORMAT        [ 11: 10] : %u(0x%08X)\r\n", csd->csd_v2.FILE_FORMAT, csd->csd_v2.FILE_FORMAT);
		printf("csd->_res6              [  9:  8] : %u(0x%08X)\r\n", csd->csd_v2._res6, csd->csd_v2._res6);
		printf("csd->CRC                [  7:  1] : %u(0x%08X)\r\n", csd->csd_v2.CRC, csd->csd_v2.CRC);
		printf("csd->_res7              [  0:  0] : %u(0x%08X)\r\n", csd->csd_v2._res7, csd->csd_v2._res7);
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
	printf("ssr->DAT_BUS_WIDTH          [511:510] : %u(0x%08X)\r\n", ssr->DAT_BUS_WIDTH, ssr->DAT_BUS_WIDTH);
	printf("ssr->SECURED_MODE           [509:509] : %u(0x%08X)\r\n", ssr->SECURED_MODE, ssr->SECURED_MODE);
	printf("ssr->RSF                    [508:502] : %u(0x%08X)\r\n", ssr->RSF, ssr->RSF);
	printf("ssr->_res1                  [501:496] : %u(0x%08X)\r\n", ssr->_res1, ssr->_res1);
	printf("ssr->SD_CARD_TYPE           [495:480] : %u(0x%08X)\r\n", ssr->SD_CARD_TYPE, ssr->SD_CARD_TYPE);
	printf("ssr->SIZE_OF_PROTECTED_AREA [479:448] : %u(0x%08X)\r\n", ssr->SIZE_OF_PROTECTED_AREA, ssr->SIZE_OF_PROTECTED_AREA);
	printf("ssr->SPEED_CLASS            [447:440] : %u(0x%08X)\r\n", ssr->SPEED_CLASS, ssr->SPEED_CLASS);
	printf("ssr->PERFORMANCE_MOVE       [439:432] : %u(0x%08X)\r\n", ssr->PERFORMANCE_MOVE, ssr->PERFORMANCE_MOVE);
	printf("ssr->AU_SIZE                [431:428] : %u(0x%08X)\r\n", ssr->AU_SIZE, ssr->AU_SIZE);
	printf("ssr->_res2                  [427:424] : %u(0x%08X)\r\n", ssr->_res2, ssr->_res2);
	printf("ssr->ERASE_SIZE             [423:408] : %u(0x%08X)\r\n", ssr->ERASE_SIZE, ssr->ERASE_SIZE);
	printf("ssr->ERASE_TIMEOUT          [407:402] : %u(0x%08X)\r\n", ssr->ERASE_TIMEOUT, ssr->ERASE_TIMEOUT);
	printf("ssr->ERASE_OFFSET           [401:400] : %u(0x%08X)\r\n", ssr->ERASE_OFFSET, ssr->ERASE_OFFSET);
	printf("ssr->UHS_SPEED_GRADE        [399:396] : %u(0x%08X)\r\n", ssr->UHS_SPEED_GRADE, ssr->UHS_SPEED_GRADE);
	printf("ssr->UHS_AU_SIZE            [395:392] : %u(0x%08X)\r\n", ssr->UHS_AU_SIZE, ssr->UHS_AU_SIZE);
	printf("ssr->_res3                  [391:384] : %u(0x%08X)\r\n", ssr->_res3, ssr->_res3);
	printf("ssr->_res4                  [383:352] : %u(0x%08X)\r\n", ssr->_res4, ssr->_res4);
	printf("ssr->_res5                  [351:320] : %u(0x%08X)\r\n", ssr->_res5, ssr->_res5);
	printf("ssr->_res6                  [319:288] : %u(0x%08X)\r\n", ssr->_res6, ssr->_res6);
	printf("ssr->_res7                  [287:256] : %u(0x%08X)\r\n", ssr->_res7, ssr->_res7);
	printf("ssr->_res8                  [255:224] : %u(0x%08X)\r\n", ssr->_res8, ssr->_res8);
	printf("ssr->_res9                  [223:192] : %u(0x%08X)\r\n", ssr->_res9, ssr->_res9);
	printf("ssr->_res10                 [191:160] : %u(0x%08X)\r\n", ssr->_res10, ssr->_res10);
	printf("ssr->_res11                 [159:128] : %u(0x%08X)\r\n", ssr->_res11, ssr->_res11);
	printf("ssr->_res12                 [127: 96] : %u(0x%08X)\r\n", ssr->_res12, ssr->_res12);
	printf("ssr->_res13                 [ 95: 64] : %u(0x%08X)\r\n", ssr->_res13, ssr->_res13);
	printf("ssr->_res14                 [ 63: 32] : %u(0x%08X)\r\n", ssr->_res14, ssr->_res14);
	printf("ssr->_res15                 [ 31:  0] : %u(0x%08X)\r\n", ssr->_res15, ssr->_res15);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

void Sdio_Print_SCR(SD_SCRreg *scr)
{
	SDIO_CMD_LOG_START;

	printf("===================================================\r\n");
	printf("scr->SCR_STRUCTURE         [63:60] : %u(0x%08X)\r\n", scr->SCR_STRUCTURE, scr->SCR_STRUCTURE);
	printf("scr->SD_SPEC               [59:56] : %u(0x%08X)\r\n", scr->SD_SPEC, scr->SD_SPEC);
	printf("scr->DATA_STAT_AFTER_ERASE [55:55] : %u(0x%08X)\r\n", scr->DATA_STAT_AFTER_ERASE, scr->DATA_STAT_AFTER_ERASE);
	printf("scr->SD_SECURITY           [54:52] : %u(0x%08X)\r\n", scr->SD_SECURITY, scr->SD_SECURITY);
	printf("scr->SD_BUS_WIDTHS         [51:48] : %u(0x%08X)\r\n", scr->SD_BUS_WIDTHS, scr->SD_BUS_WIDTHS);
	printf("scr->SD_SPEC3              [47:47] : %u(0x%08X)\r\n", scr->SD_SPEC3, scr->SD_SPEC3);
	printf("scr->EX_SECURITY           [46:43] : %u(0x%08X)\r\n", scr->EX_SECURITY, scr->EX_SECURITY);
	printf("scr->_res1                 [42:34] : %u(0x%08X)\r\n", scr->_res1, scr->_res1);
	printf("scr->CMD_SUPPORT           [33:32] : %u(0x%08X)\r\n", scr->CMD_SUPPORT, scr->CMD_SUPPORT);
	printf("scr->_res2                 [31: 0] : %u(0x%08X)\r\n", scr->_res2, scr->_res2);
	printf("===================================================\r\n");

	SDIO_CMD_LOG_END;
}

UINT Sdio0GetActive(void)
{
	return sdinfo.active;
}

UINT Sdio0GetSectorCnt(void)
{
	SDIO_CMD_LOG_START;

	UINT nSector = 0;

	if(sdinfo.csd.csd_v1.CSD_STRUCTURE == 1)	// CSD 2.0
	{
		nSector = (sdinfo.csd.csd_v2.C_SIZE + 1) << 10;
		printf("CSD2.0 : C_SIZE(%u)\r\n", sdinfo.csd.csd_v2.C_SIZE);
		printf("         SECTOR(%u)\r\n", nSector);
	}
	else if(sdinfo.csd.csd_v1.CSD_STRUCTURE == 0)	// CSD 1.0
	{
		nSector = ((sdinfo.csd.csd_v1.C_SIZE + 1) * (0x1 << (sdinfo.csd.csd_v1.C_SIZE_MULT + 2)) * (0x1 << (sdinfo.csd.csd_v1.READ_BL_LEN))) >> 9;
		printf("CSD1.0 : C_SIZE(%u)\r\n", sdinfo.csd.csd_v1.C_SIZE);
		printf("         C_SIZE_MULT(%u)\r\n", sdinfo.csd.csd_v1.C_SIZE_MULT);
		printf("         BLOCK_LEN(%u)\r\n", sdinfo.csd.csd_v1.READ_BL_LEN);
		printf("         SECTOR(%u)\r\n", nSector);
	}
	else
	{
		printf("CSD Version error(%d)\r\n", sdinfo.csd.csd_v1.CSD_STRUCTURE);
	}

	SDIO_CMD_LOG_END;

	return nSector;
}

UINT Sdio0GetAUSize(void)
{
	return sdinfo.ssr.AU_SIZE;
}

static BOOL Sdio0GetRCA(void)
{	// CMD3(R6)
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd( 3, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[ 3] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	sdinfo.rca = nResp & RCA_RCA_MASK;

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0GetCID(int nType)
{	// CMD2, 10(R2): Type:0(CMD2) Type:1(CMD10)
	BOOL bRes = DEF_FAIL;
	UINT *pResp;
	UINT cmdType = 0;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.cid;

	if(nType == 0)
	{
		cmdType = 2;
		bRes = Sdio0Cmd(cmdType, 0x00000000, 1, 1, 0, 0);
	}
	else if(nType == 1)
	{
		cmdType = 10;
		bRes = Sdio0Cmd(cmdType, sdinfo.rca, 1, 1, 0, 0);
	}
	pResp[0] = SDIO0_RESP0;
	pResp[1] = SDIO0_RESP1;
	pResp[2] = SDIO0_RESP2;
	pResp[3] = SDIO0_RESP3;
	gprintf("[%2d] res(%d) RESP0(0x%08X)\r\n", cmdType, bRes, pResp[0]);
	gprintf("[%2d] res(%d) RESP1(0x%08X)\r\n", cmdType, bRes, pResp[1]);
	gprintf("[%2d] res(%d) RESP2(0x%08X)\r\n", cmdType, bRes, pResp[2]);
	gprintf("[%2d] res(%d) RESP3(0x%08X)\r\n", cmdType, bRes, pResp[3]);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_CID(&sdinfo.cid);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0GetCSD(void)
{	// CMD9(R2)
	BOOL bRes = DEF_FAIL;
	UINT *pResp;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.csd;

	bRes = Sdio0Cmd( 9, sdinfo.rca, 1, 1, 0, 0);
	pResp[0] = SDIO0_RESP0;
	pResp[1] = SDIO0_RESP1;
	pResp[2] = SDIO0_RESP2;
	pResp[3] = SDIO0_RESP3;
	gprintf("[ 9] res(%d) RESP0(0x%08X)\r\n", bRes, pResp[0]);
	gprintf("[ 9] res(%d) RESP1(0x%08X)\r\n", bRes, pResp[1]);
	gprintf("[ 9] res(%d) RESP2(0x%08X)\r\n", bRes, pResp[2]);
	gprintf("[ 9] res(%d) RESP3(0x%08X)\r\n", bRes, pResp[3]);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_CSD(&sdinfo.csd);
#endif

	if(sdinfo.csd.csd_v2.PERM_WRITE_PROTECT == 1)
	{
		eprintf("PERM_WRITE_PROTECT enable!\r\n");
		bRes = DEF_FAIL;
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0GetSSR(void)
{	// ACMD13(Data bus response / 64byte)
	BOOL bRes = DEF_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	nTemp = SDIO0_DAT_BL;
	SDIO0_DAT_BL = sizeof(SD_SSR);
	SDIO0_DATLEN = 1;

#ifdef __SDIO_IRQ__
	sdio_read_addr = (UINT)&sdinfo.ssr;
#endif

	bRes = Sdio0Cmd(13, 0x00000000, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;
	gprintf("[13] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	if(bRes == DEF_OK)
	{
		while(SDIO0_DAT_EN);

#ifdef __SDIO_IRQ__

#else
		BYTE *getData = (BYTE *)&sdinfo.ssr;
		DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO0_BASE, sizeof(SD_SSR));
		invalidate_dcache_range((UINT)getData, (UINT)(getData) + sizeof(SD_SSR));
#endif
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

#ifdef __SDIO_IRQ__
	sdio_read_addr = 0;
#endif

	SDIO0_DAT_BL = nTemp;

#ifdef ENX_SDIO_CMD_DEBUG
	if(bRes == DEF_OK)
		Sdio_Print_SSR(&sdinfo.ssr);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0GetSCR(void)
{	// ACMD51(Data bus response / 8byte)
	BOOL bRes = DEF_FAIL;
	UINT nResp, nTemp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	nTemp = SDIO0_DAT_BL;
	SDIO0_DAT_BL = sizeof(SD_SCRreg);
	SDIO0_DATLEN = 1;

#ifdef __SDIO_IRQ__
	sdio_read_addr = (UINT)&sdinfo.scr;
#endif

	bRes = Sdio0Cmd(51, 0x00000000, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
		while(SDIO0_DAT_EN);

#ifndef __SDIO_IRQ__
		BYTE *getData = (BYTE *)&sdinfo.scr;
		DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO0_BASE, sizeof(SD_SCRreg));
		invalidate_dcache_range((UINT)getData, (UINT)(getData) + sizeof(SD_SCRreg));
#endif
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

#ifdef __SDIO_IRQ__
	sdio_read_addr = 0;
#endif

	gprintf("[51] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO0_DAT_BL = nTemp;

#ifdef ENX_SDIO_CMD_DEBUG
	if(bRes == DEF_OK)
		Sdio_Print_SCR(&sdinfo.scr);
#endif

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0SwitchVoltage(void)
{	// CMD11 : Switch to 1.8V
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd(11, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[11] res(%d) RESP0(0x%08X)\r\n", bRes, nResp);

	// Switch 1.8V
	if(bRes == DEF_OK)
	{
		PAD_SDIO0_CMD_PU = 1;		// CMD pull-up disable
		PAD_SDIO0_DAT_PU = 1;		// DAT pull-up disable
		SDIO0_CLKEN = 0;			// CLK disable
		vTaskDelay(1);				// 5ms delay(min.)

		gprintf("Set Default speed mode\r\n");
		Sdio0ClockDiv(eSDIO_CLK_25MHz);

		PAD_SDIO0_MSEL = 0x3F;		// 1.8v
		SDIO0_CLKEN = 1;			// CLK enable
		PAD_SDIO0_CMD_PU = 0;		// CMD pull-up enable
		PAD_SDIO0_DAT_PU = 0;		// DAT pull-up enable

		sdinfo.voltage_mode = 1;
		eprintf("3.3v -> 1.8v ok\r\n");
	}
	else
	{
		sdinfo.voltage_mode = 0;
		eprintf("3.3v -> 1.8v fail\r\n");
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0BusWidthChange(void)
{	// ACMD6 : Bus Width 1bit -> 4bit
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	// Select a card
	bRes = Sdio0Cmd( 7, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[ 7] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// Unlock
//	bRes = Sdio0Cmd(42, 0x00000000, 1, 0, 0, 0);
//	nResp = SDIO0_RESP0;
//	printf("%s(%d) : [42] res(%d) RESP(0x%08X)\r\n", __func__, __LINE__, bRes, nResp);

	// ACMD6 : 1bit -> 4bit
	bRes = Sdio0Cmd(55, sdinfo.rca, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	bRes = Sdio0Cmd( 6, 0x00000002, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[ 6] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0ClockSwitch(int mode, int group, BYTE value, BYTE *getData)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0, nArg = 0, nTemp = 0;
	UINT nSize = 64;

	SDIO_CMD_LOG_START;

	nArg = mode << 31 | 0x00FFFFFF;
	nArg &= ~(0xFF << (group * 4));
	nArg |= value << (group * 4);

//	printf("nArg(mode:%d) : 0x%08X\r\n", mode, nArg);

	nTemp = SDIO0_DAT_BL;
	SDIO0_DAT_BL = nSize;
	SDIO0_DATLEN = 1;

#ifdef __SDIO_IRQ__
	sdio_read_addr = (UINT)getData;
#endif

	bRes = Sdio0Cmd( 6, nArg, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
		while(SDIO0_DAT_EN);

#ifndef __SDIO_IRQ__
		if(SDIO0_DAT_CRCERR)
		{
			gprintf("CRC Error\r\n");
			gprintf("[ 6] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
//		else
		{
			DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO0_BASE, nSize);
			invalidate_dcache_range((UINT)getData, ((UINT)getData) + nSize);
		}
#endif
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

#ifdef __SDIO_IRQ__
	sdio_read_addr = 0;
#endif

	gprintf("[ 6] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO0_DAT_BL = nTemp;

	SDIO_CMD_LOG_END;

	return bRes;
}

BOOL Sdio0SetClock(void)
{	// CMD6 : Check & Switch Clock
	BOOL bRes = DEF_FAIL;
	BYTE status[64] = {0};

	SDIO_CMD_LOG_START;

	if(sdinfo.ssr.SPEED_CLASS >= SD_SPEED_CLASS10)
	{
		if(sdinfo.scr.SD_SPEC < SCR_SPEC_VER_1)
		{
			gprintf("spec ver 1.0 or 1.01\r\n");
			goto if_speed;
		}

		if(!(sdinfo.csd.csd_v2.CCC & SCR_CCC_SWITCH))
		{
			gprintf("card lacks mandatory switch function, performance might suffer.\r\n");
			goto if_speed;
		}

		if(Sdio0ClockSwitch(0, 0, 1, status) == DEF_FAIL)
		{
			gprintf("problem reading switch capabilities, performance might suffer.\r\n");
			goto if_speed;
		}

		vTaskDelay(5);

#if 0
		// 4.3.10.4 Switch Function Status / Table4-13: Status Data Structure
		// Maximux Current/Power Consumption
		printf("status[00] : 0x%02X\r\n", status[0]);
		printf("status[01] : 0x%02X\r\n", status[1]);

		// Support Bits of Functions in Function Group 6
		printf("status[02] : 0x%02X\r\n", status[2]);
		printf("status[03] : 0x%02X\r\n", status[3]);

		// Support Bits of Functions in Function Group 5
		printf("status[04] : 0x%02X\r\n", status[4]);
		printf("status[05] : 0x%02X\r\n", status[5]);

		// Support Bits of Functions in Function Group 4
		printf("status[06] : 0x%02X\r\n", status[6]);
		printf("status[07] : 0x%02X\r\n", status[7]);

		// Support Bits of Functions in Function Group 3
		printf("status[08] : 0x%02X\r\n", status[8]);
		printf("status[09] : 0x%02X\r\n", status[9]);

		// Support Bits of Functions in Function Group 2
		printf("status[10] : 0x%02X\r\n", status[10]);
		printf("status[11] : 0x%02X\r\n", status[11]);

		// Support Bits of Functions in Function Group 1
		printf("status[12] : 0x%02X\r\n", status[12]);
		printf("status[13] : 0x%02X\r\n", status[13]);

		// Function Selection of Function Group6,5 
		printf("status[14] : 0x%02X\r\n", status[14]);

		// Function Selection of Function Group4, 3
		printf("status[15] : 0x%02X\r\n", status[15]);

		// Function Selection of Function Group2, 1
		printf("status[16] : 0x%02X\r\n", status[16]);

		// Data Structure Version
		printf("status[17] : 0x%02X\r\n", status[17]);
#else
#endif

		if(!(status[13] & 0x02))
		{
			eprintf("\r\n");
			goto if_speed;
		}

		if(Sdio0ClockSwitch(1, 0, 1, status) == DEF_FAIL)
		{
			eprintf("\r\n");
			goto if_speed;
		}

		if((status[16] & 0x0F) != 1)
		{
			eprintf("Problem switching card into high-speed mode!\r\n");
		}
		else
		{
			gprintf("Set High speed mode\r\n");
#if (SDIO0_SAFE_MODE==0)
			Sdio0ClockDiv(eSDIO_CLK_50MHz);
#elif (SDIO0_SAFE_MODE==1)
			Sdio0ClockDiv(eSDIO_CLK_33MHz);
#endif
//			Sdio0ClockDiv(eSDIO_CLK_100MHz);
//			Sdio0ClockDiv(eSDIO_CLK_25MHz);
		}

		bRes = DEF_OK;
	}
	else
	{
if_speed:
		gprintf("Set Default speed mode\r\n");
		Sdio0ClockDiv(eSDIO_CLK_25MHz);
		bRes = DEF_OK;
	}

	SDIO_CMD_LOG_END;

	return bRes;
}

BOOL Sdio0SetBlockLength(void)
{	// CMD16 : Define the block length
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd(16, SDIO0_DAT_BL, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	gprintf("[16] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

	return bRes;
}

BOOL Sdio0TuningCommand(void)
{	// CMD19 : 4.2.4.5 Tuning Command
	BOOL bRes = DEF_FAIL;
	UINT nResp, nTemp = 0, nSize = 64, i;
	BYTE getData[64] = {0};
	BYTE checkData[64] = {	0xFF,0x0F,0xFF,0x00,0xFF,0xCC,0xC3,0xCC,0xC3,0x3C,0xCC,0xFF,0xFE,0xFF,0xFE,0xEF,
							0xFF,0xDF,0xFF,0xDD,0xFF,0xFB,0xFF,0xFB,0xBF,0xFF,0x7F,0xFF,0x77,0xF7,0xBD,0xEF,
							0xFF,0xF0,0xFF,0xF0,0x0F,0xFC,0xCC,0x3C,0xCC,0x33,0xCC,0xCF,0xFF,0xEF,0xFF,0xEE,
							0xFF,0xFD,0xFF,0xFD,0xDF,0xFF,0xBF,0xFF,0xBB,0xFF,0xF7,0xFF,0xF7,0x7F,0x7B,0xDE};	

	SDIO_CMD_LOG_START;

	nTemp = SDIO0_DAT_BL;
	SDIO0_DAT_BL = nSize;
	SDIO0_DATLEN = 1;

	bRes = Sdio0Cmd(19, 0x00000000, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;
	gprintf("[19] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	if(bRes == DEF_OK)
	{
		while(SDIO0_DAT_EN);

		DmaMemCpy_ip((BYTE *)getData, (BYTE *)SDIO0_BASE, nSize);
		invalidate_dcache_range((UINT)getData, (UINT)(getData) + nSize);

		for(i=0;i<nSize;i++)
		{
			if(checkData[i] != getData[i])
			{
				eprintf("Tuning Block Pattern Mismatch\r\n");
				hexDump("Tuning Block Pattern", getData, nSize);
				Sdio0ClockDiv(eSDIO_CLK_25MHz);	// 낮춘 상태에서 다시 테스트 해야하지 않을까?
				break;
			}
		}
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

	SDIO0_DAT_BL = nTemp;

	SDIO_CMD_LOG_END;

	return bRes;
}

static BOOL Sdio0Initialization(void)
{	// ACMD41(R3)
	UINT *pResp;
	UINT nResp, nTryCnt = 0;
	BOOL bRes;

	SDIO_CMD_LOG_START;

	pResp = (UINT *)&sdinfo.ocr;

	do{
		if(SDIO0_DET)
		{
			eprintf("End(FAIL) : SDcard no detect\r\n");
			return DEF_FAIL;
		}
		if(++nTryCnt > SD_TRY_CNT)
		{
			eprintf("End(FAIL) : RESP(0x%08X)\r\n", *pResp);
			return DEF_FAIL;
		}

		vTaskDelay(10);	// Polling less than 100ms interval
		bRes = Sdio0Cmd(55, 0x00000000, 1, 0, 0, 0);
		nResp = SDIO0_RESP0;
		gprintf("[55] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
		bRes = Sdio0Cmd(41, ACMD41_HCS|ACMD41_XPC|ACMD41_VOLT
#ifdef sdio_support_1_8v
			|ACMD41_S18R
#endif
			, 1, 0, 0, 0);	//	ACMD41
		*pResp = SDIO0_RESP0;
		gprintf("[41] res(%d) RESP(0x%08X)\r\n", bRes, *pResp);
	}while(sdinfo.ocr.busy == 0);

#ifdef ENX_SDIO_CMD_DEBUG
	Sdio_Print_OCR(&sdinfo.ocr);
#endif

	SDIO_CMD_LOG_END;

	return DEF_OK;
}

static BOOL Sdio0Reset(void)
{	// CMD0, 8 : Power on(Reset)
	UINT nResp;
	BOOL bRes;

	SDIO_CMD_LOG_START;

	bRes = Sdio0Cmd( 0, 0x00000000, 0, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", bRes);
	vTaskDelay(1);

	bRes = Sdio0Cmd( 0, 0x00000000, 0, 0, 0, 0);
	gprintf("[ 0] res(%d)\r\n", bRes);
	vTaskDelay(1);

	bRes = Sdio0Cmd( 8, CMD8_VHS|CMD8_PATTERN, 1, 0, 0, 0);	//	Send IF Condition
	nResp = SDIO0_RESP0;

	gprintf("[ 8] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	if(nResp & 0x300)
	{
		SDIO_CMD_LOG_END;
		return DEF_OK;
	}
	else
	{
		eprintf("End(FAIL) : RESP(0x%08X)\r\n", nResp);
		SDIO_CMD_LOG_END;
		return DEF_FAIL;
	}
}

BYTE Sdio0InitProcess(void)
{
	SDIO_CMD_LOG_START;
	UINT errCode = 0;

	memset(&sdinfo, 0, sizeof(sdinfo));

	// CMD 0, 8
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0Reset() == DEF_FAIL)
	{
		errCode = 1;
		goto done_fail;
	}

	// ACMD 41
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0Initialization() == DEF_FAIL)
	{
		errCode = 2;
		goto done_fail;
	}

	sdinfo.voltage_mode = 0;
	if(sdinfo.ocr.VDD18 && sdinfo.ocr.CCS)
	{
		// CMD 11
		if(Sdio0SwitchVoltage() == DEF_FAIL)
		{
			errCode = 3;
		}
		else
		{
			sdinfo.voltage_mode = 1;
		}
	}

	// CMD 2
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetCID(0) == DEF_FAIL)
	{
		errCode = 4;
		goto done_fail;
	}

	// CMD 3
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetRCA() == DEF_FAIL)
	{
		errCode = 5;
		goto done_fail;
	}

	// CMD 9
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetCSD() == DEF_FAIL)
	{
		errCode = 6;
		goto done_fail;
	}

	// CMD 10
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetCID(1) == DEF_FAIL)
	{
		errCode = 7;
		goto done_fail;
	}

	// CMD 7, 42, ACMD 6
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0BusWidthChange() == DEF_FAIL)
	{
		errCode = 8;
		goto done_fail;
	}

	// ACMD 13
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetSSR() == DEF_FAIL)
	{
		errCode = 9;
		goto done_fail;
	}

	// ACMD 51
//	printf("%s(%d) : \r\n", __func__, __LINE__);
	if(Sdio0GetSCR() == DEF_FAIL)
	{
		errCode = 10;
		goto done_fail;
	}

	// CMD 6
	if(Sdio0SetClock() == DEF_FAIL)
	{
		errCode = 11;
		goto done_fail;
	}

	// CMD 19
	if(sdinfo.voltage_mode == 1)	// UHS-1 1.8v
	{
		if(Sdio0TuningCommand() == DEF_FAIL)
		{
			errCode = 12;
			goto done_fail;
		}
	}

	// CMD 16
	if(Sdio0SetBlockLength() == DEF_FAIL)
	{
		errCode = 13;
		goto done_fail;
	}

//	Sdio_Print_CSD(&sdinfo.csd);

	sdinfo.active = 1;

	printf("(%04d)%s : SDIO Init OK\r\n", __LINE__, __func__);
#ifdef __SDIO_IRQ__
	printf("(%04d)%s : sdio_read_addr(0x%08X)\r\n", __LINE__, __func__, &sdio_read_addr);
	printf("(%04d)%s : sdio_write_addr(0x%08X)\r\n", __LINE__, __func__, &sdio_write_addr);
#endif

	SDIO_CMD_LOG_END;
	return DEF_OK;
done_fail:

	printf("(%04d)%s : SDIO Init Fail(err:%d)\r\n", __LINE__, __func__, errCode);

	SDIO_CMD_LOG_END;
	return DEF_FAIL;
}

void Sdio0Init(void)
{
	SDIO_CMD_LOG_START;

	memset(&sdinfo, 0, sizeof(sdinfo));

	UINT BaseAddr = SDIO0_REG_BASE;
	_sdioreg = (SDIO_REG *)BaseAddr;

#if 0
	SDIO0_CMD_IRQ_EN = 0;	// Command interrupt disable
	SDIO0_DAT_IRQ_EN = 0;	// Data interrupt disable
#else
	_sdioreg->SDIO_CONT.CMD_IRQ_EN = 0;	// Command interrupt disable
	_sdioreg->SDIO_CONT.DAT_IRQ_EN = 0;	// Data interrupt disable
#endif

	PAD_SDIO0_MSEL = 0;	// 3.3v

	PAD_SDIO0_CMD_PU = 0;	// CMD Pull-up enable
	PAD_SDIO0_DAT_PU = 0;	// DAT Pull-up enable

#if 0
	SDIO0_EN = 1;
	SDIO0_CLK_DIV = eSDIO_CLK_400KHz;
	SDIO0_MODE = 1;
	SDIO0_DAT_BL = 0x200;
	SDIO0_CLKEN = 1;
#else
	_sdioreg->SDIO_CONT.EN = 1;
	_sdioreg->SDIO_CONT.CLK_DIV = eSDIO_CLK_400KHz;
	_sdioreg->SDIO_CONT.MODE = 1;
	_sdioreg->SDIO_CMD.DAT_BL = 0x200;
	_sdioreg->SDIO_CONT.CLKEN = 1;
#endif

#ifdef __SDIO_IRQ__
#if 0
//	SDIO0_CMD_IRQ_EN = 1;	// Command interrupt enable
	SDIO0_DAT_IRQ_EN = 1;	// Data interrupt enable
#else
//	_sdioreg->SDIO_CONT.CMD_IRQ_EN = 1;	// Command interrupt enable
	_sdioreg->SDIO_CONT.CMD_IRQ_EN = 1;	// Data interrupt enable
#endif
#endif

	SDIO_CMD_LOG_END;
}

void Sdio0Deinit(void)
{
	SDIO_CMD_LOG_START;

	SDIO0_CLK_DIV = eSDIO_CLK_400KHz;
	SDIO0_MODE = 1;
	SDIO0_DAT_BL = 0x200;	

	SDIO0_DAT_WE = 0;
	SDIO0_DAT_EN = 0;
	SDIO0_CMD_EN = 0;

	SDIO0_DET_IRQ_EN = 0;
	SDIO0_CHIP_IRQ_EN = 0;
	SDIO0_DAT_IRQ_EN = 0;
	SDIO0_CMD_IRQ_EN = 0;

	PAD_SDIO0_MSEL = 0;			// 3.3v

	PAD_SDIO0_CMD_PU = 1;		// CMD Pull-up disable
	PAD_SDIO0_DAT_PU = 1;		// DAT Pull-up disable

	SDIO0_CLKEN = 0;			// CLK Disable
	SDIO0_EN = 0;

	SDIO_CMD_LOG_END;
}

BYTE Sdio0E(UINT start_sctor, UINT end_sctor)
{
	BOOL bRes = DEF_FAIL;
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
	bRes = Sdio0Cmd(32, nBlocSp, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	tprintf("[32] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// CMD33 : Erase end   address
	bRes = Sdio0Cmd(33, nBlocEp, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	tprintf("[33] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	// CMD38 : Erase command
	bRes = Sdio0Cmd(38, 0x00000000, 1, 0, 0, 0);
	nResp = SDIO0_RESP0;
	tprintf("[38] res(%d) RESP(0x%08X)\r\n", bRes, nResp);

	SDIO_CMD_LOG_END;

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

BYTE Sdio0SectorW(const BYTE *buff, UINT sector)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0, nAddr = 0;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO0_DAT_BL, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);

	while(SDIO0_DAT_EN)
		vTaskDelay(0);

	SDIO0_DATLEN = 1;

#ifdef __SDIO_IRQ__
	sdio_write_addr = (UINT)buff;
#endif

	bRes = Sdio0Cmd(24, nAddr, 1, 0, 1, 1);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
		DmaMemCpy_ip_async((BYTE *)SDIO0_BASE, (BYTE *)buff, SDIO0_DAT_BL);

#ifdef __SDIO_IRQ__
		while(SDIO0_DAT_WE);
#else
		while(SDIO0_DAT_EN)
			vTaskDelay(0);
		SDIO0_DAT_WE = 0;
#endif

		if(SDIO0_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO0_DAT_EN = 0;
		SDIO0_DAT_WE = 0;
		tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

#ifdef __SDIO_IRQ__
	sdio_write_addr = 0;
#endif

//	tprintf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F');

//	SDIO_DAT_LOG_END;

	return bRes;
}

BYTE Sdio0W(BYTE *buff, UINT sector, UINT count)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0, nAddr = 0, i;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO0_DAT_BL, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);

	while(SDIO0_DAT_EN)
		vTaskDelay(0);

	SDIO0_DATLEN = count;

#ifdef __SDIO_IRQ__
	sdio_write_addr = (UINT)buff;
#endif

	bRes = Sdio0Cmd(25, nAddr, 1, 0, 1, 1);
	nResp = SDIO0_RESP0;

//	UINT write_cnt;

	if(bRes == DEF_OK)
	{
#ifdef __SDIO_IRQ__
		DmaMemCpy_ip((BYTE *)SDIO0_BASE, (BYTE *)sdio_write_addr, SDIO0_DAT_BL);
		sdio_write_addr += SDIO0_DAT_BL;

		while(SDIO0_DAT_WE);
#else
		for(i=0;i<count;)
		{
			while(SDIO0_DAT_FULL)
				vTaskDelay(0);

#if 1
			if(SDIO0_DAT_CRCERR)
			{
				tprintf("CRC Error\r\n");
				tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_ip_async((BYTE *)SDIO0_BASE, buff, SDIO0_DAT_BL);
			buff += SDIO0_DAT_BL;
			i++;
#else
			// 512*4byte write loop
			write_cnt = SdCardBlockIO(count, i);
			DmaMemCpy_ip((BYTE *)SDIO0_BASE, buff, (SDIO0_DAT_BL*write_cnt), NULL);
			buff += (SDIO0_DAT_BL*write_cnt);
			i += write_cnt;
#endif
		}

		while(SDIO0_DAT_EN)
			vTaskDelay(0);
		SDIO0_DAT_WE = 0;
#endif

		if(SDIO0_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO0_DAT_EN = 0;
		SDIO0_DAT_WE = 0;
		tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

#ifdef __SDIO_IRQ__
	sdio_write_addr = 0;
#endif

//	tprintf("[25] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F');

	if(bRes == DEF_OK)
	{
		bRes = Sdio0Cmd(12, 0, 1, 0, 0, 0);
		nResp = SDIO0_RESP0;
//		while(SDIO0_BUSY);
//		tprintf("[12] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	}

//	SDIO_DAT_LOG_END;

	return bRes;
}

BYTE Sdio0SectorR(const BYTE *buff, UINT sector)
{
	BOOL bRes = DEF_FAIL;
 	UINT nResp = 0, nAddr = 0;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO0_DAT_BL, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);

	while(SDIO0_DAT_EN)
		vTaskDelay(0);

	SDIO0_DATLEN = 1;

#ifdef __SDIO_IRQ__
	sdio_read_addr = (UINT)buff;
#endif

	bRes = Sdio0Cmd(17, nAddr, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
		while(SDIO0_DAT_EN)
			vTaskDelay(0);

#ifdef __SDIO_IRQ__
		// IRQ wait...
#else
		if(SDIO0_DAT_CRCERR)
		{
			tprintf("CRC Error\r\n");
			tprintf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
//		else
		{
			DmaMemCpy_ip((BYTE *)buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
			invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO0_DAT_BL);
		}
#endif
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

#ifdef __SDIO_IRQ__	
	sdio_read_addr = 0;
#endif

//	tprintf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F');

//	SDIO_DAT_LOG_END;

	return bRes;
}

BYTE Sdio0R(BYTE *buff, UINT sector, UINT count)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0, nAddr = 0, i;
	BYTE *_buff = buff;

//	SDIO_DAT_LOG_START;

	if(sdinfo.ocr.CCS) {
		nAddr = sector;
	} else {
		nAddr = sector << 9;
	}

//	tprintf("sector(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", nAddr, SDIO0_DAT_BL, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);

	while(SDIO0_DAT_EN)
		vTaskDelay(0);

	SDIO0_DATLEN = count;

#ifdef __SDIO_IRQ__
	sdio_read_addr = (UINT)buff;
#endif

	bRes = Sdio0Cmd(18, nAddr, 1, 0, 1, 0);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
#ifdef __SDIO_IRQ__
		while(SDIO0_DAT_EN);	// Irq Processing
#else
		for(i=0;i<count;i++)
		{
			while(SDIO0_DAT_EMPTY)
				vTaskDelay(0);

			if(SDIO0_DAT_CRCERR)
			{
				tprintf("CRC Error\r\n");
				tprintf("[18] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
			}

			DmaMemCpy_ip(buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
			buff += SDIO0_DAT_BL;
		}
		invalidate_dcache_range((UINT)_buff, ((UINT)_buff) + (count*SDIO0_DAT_BL));
#endif
	}
	else
	{
		SDIO0_DAT_EN = 0;
	}

#ifdef __SDIO_IRQ__
	sdio_read_addr = 0;
#endif

//	tprintf("[18] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F');

	if(bRes == DEF_OK)
	{
		bRes = Sdio0Cmd(12, 0, 1, 0, 0, 0);
		nResp = SDIO0_RESP0;
//		tprintf("[12] res(%d) RESP(0x%08X)\r\n", bRes, nResp);
	}

//	SDIO_DAT_LOG_END;

	return bRes;
}

UINT Sdio0GetInstall(void)
{
	return SDIO0_DET;
}

#ifdef __SDIO_IRQ__
//#include "portmacro.h"
void IsrSdio0(void *dummy)
{
	SDIO0_DAT_IRQ_CLR = 1;
#ifdef __SDIO_IRQ__
	if(SDIO0_DAT_WE)
	{
//		printf("(%04d)%s : SDIO Irq Write\r\n", __LINE__, __func__);
		if(SDIO0_DAT_EN)
		{
			Uart0_Tx('W');
//			UartPutHex(SDIO0_DATLEN);

			while(DMA_GO);
			DMA_DST = (UINT)SDIO0_BASE;
			DMA_SRC = (UINT)sdio_write_addr;
			DMA_LEN = (UINT)SDIO0_DAT_BL;
			DMA_MODE= 0;
			DMA_GO	= 1;
			while(DMA_GO);

//			DmaMemCpy((BYTE *)SDIO0_BASE, (BYTE *)sdio_write_addr, SDIO0_DAT_BL, NULL);
			sdio_write_addr += SDIO0_DAT_BL;
		}
		else
		{
			Uart0_Tx('w');
			SDIO0_DAT_WE = 0;
		}
	}
	else
	{
		Uart0_Tx('I');
		do {
//			printf("(%04d)%s : SDIO Irq Read(%d, %d)\r\n", __LINE__, __func__, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);
			if(SDIO0_DAT_EN)
				Uart0_Tx('R');
			else
				Uart0_Tx('r');

//			while(SDIO0_DAT_EMPTY)
//				UartPutByte('E');

//			portENTER_CRITICAL();
			while(DMA_GO);
			DMA_DST = (UINT)sdio_read_addr;
			DMA_SRC = (UINT)SDIO0_BASE;
			DMA_LEN = (UINT)SDIO0_DAT_BL;
			DMA_MODE= 0;
			DMA_GO	= 1;
			while(DMA_GO);
//			portEXIT_CRITICAL();

//			DmaMemCpy((BYTE *)sdio_read_addr, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL, NULL);
			invalidate_dcache_range(sdio_read_addr, sdio_read_addr + SDIO0_DAT_BL);
			sdio_read_addr += SDIO0_DAT_BL;

			if(SDIO0_DAT_EN)
				Uart0_Tx('@');
			else
				Uart0_Tx('#');

		}while(!SDIO0_DAT_EMPTY);
		Uart0_Tx('i');
	}
#endif
	SDIO0_DAT_IRQ_CLR = 1;

	UNUSED(dummy);
}
#endif
#endif
