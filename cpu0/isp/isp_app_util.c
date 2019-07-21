/* **************************************************************************
 File Name	: 	app_util.c
 Description:	Utillity
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"

//*************************************************************************************************
// Assembler call functions
//-------------------------------------------------------------------------------------------------
// .data section initilize
//void _DataSectInit(void)
//{
//	UINT i;
//#if 1
//	UINT *pDst = (UINT*)_data_s;
//	UINT *pSrc = (UINT*)_rom_data_s;
//	const UINT nNum = (_data_e - _data_s)>>2;
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = *(pSrc++);
//	}
//
//	BYTE *pbDst = (BYTE*)pDst;
//	BYTE *pbSrc = (BYTE*)pSrc;
//	const UINT nMod = (_data_e - _data_s) - (nNum<<2);
//	for(i=0; i<nMod; i++)
//	{
//		*(pbDst++) = *(pbSrc++);
//	}
//#else
//	BYTE *pDst = (BYTE*)_data_s;
//	BYTE *pSrc = (BYTE*)_rom_data_s;
//	const UINT nNum = _data_e - _data_s;
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = *(pSrc++);
//	}
//#endif
//}
//
////-------------------------------------------------------------------------------------------------
//// .bss section initilize
//void _BssSectInit(void)
//{
//	UINT i;
//#if 1
//	UINT *pDst = (UINT*)_bss_s;
//	const UINT nNum = (_bss_e - _bss_s)>>2;
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = 0;
//	}
//
//	BYTE *pbDst = (BYTE*)pDst;
//	const UINT nMod = (_bss_e - _bss_s) - (nNum<<2);
//	for(i=0; i<nMod; i++)
//	{
//		*(pbDst++) = 0;
//	}
//#else
//	BYTE *pDst = (BYTE*)_bss_s;
//	const UINT nNum = _bss_e - _bss_s;
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = 0;
//	}
//#endif
//}
//
////void PRINT_MESSAGE1(void)
////{
////
////}
//
//void _ResetSectInit(void)
//{
//	UINT i;
//
//#if model_Ispm == 1
//	UINT *pDst = (UINT*)(_ispm_text_s&0xFF000000)/*SRAM_BASE*/;
//#else
//	UINT *pDst = (UINT*)(_data_s&0xFF000000)/*SRAM_BASE*/;		// ISPM과 DSPM이 따로 있는 경우 주의
//#endif
//	UINT *pSrc = (UINT*)_reset_s/*FLS_BASE*/;
//	UINT nNum = 0xF00>>2;
//
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = *(pSrc++);
//	}
//
//#if model_Ispm == 1
//	pDst = (UINT*)_ispm_text_s;
//	pSrc = (UINT*)_rom_ispm_text_s;
//	nNum = (_ispm_text_e - _ispm_text_s)>>2;
//
//	for(i=0; i<nNum; i++)
//	{
//		*(pDst++) = *(pSrc++);
//	}
//#endif
//}


//*************************************************************************************************
// String
//-------------------------------------------------------------------------------------------------
// String
#if 0	// 20150808 - It defined in "stdlib.c"
UINT ISPM strlen(const char *apbStr)
{	// String length
	UINT	nLen = 0;
    while(*apbStr++) nLen++;
	return nLen;
}
#endif

void ISPM uchar2strb(char* apbStr, BYTE anVal, UINT anLen)
{
	UINT	i;

	apbStr[anLen] = '\0';	// CAUTION!
	apbStr += anLen-1;
	for(i=0; i<anLen; i++){
		*apbStr-- = (anVal&0x1) ? '1' : '0' ;
		anVal >>= 1;
    }
}

void ISPM uint2strh(char* apbStr, UINT anVal, UINT anLen)
{	// Value -> String Hex
	UINT	i;

	apbStr[anLen] = '\0';	// CAUTION!
	apbStr += anLen-1;
	for(i=0; i<anLen; i++){
		const BYTE bBuf = anVal & 0xf;
		*apbStr-- = (bBuf >= 0xA) ? bBuf - 0xA + 'A' : bBuf + '0' ;
		anVal >>= 4;
    }
}

void ISPM int2str(char* apbStr, int aiVal, UINT anLen)
{	// Value -> String Dec
	UINT	i;
	UINT	nMod;

	apbStr[anLen] = '\0';	// CAUTION!

	int neg;

	if(aiVal < 0) {
		neg = 1;
		aiVal = -aiVal;
	}
	else {
		neg = 0;
	}

	for(i=0; i<anLen-neg; i++){
		if(aiVal >= 10) {
			nMod = aiVal / 10;
			apbStr[anLen-i-1] = '0' + (aiVal - (nMod * 10));
			aiVal = nMod;
		}
		else {
			break;
		}
	}

	if(i<anLen && aiVal >= 0) {
		apbStr[anLen-i-1] = '0' + aiVal;
		i++;
	}

	if(i<anLen && neg) {
		apbStr[anLen-i-1] = '-';
		i++;
	}

	for(; i<anLen; i++) {
		apbStr[anLen-i-1] = ' ';
	}
}

void ISPM per2str(char* apbStr, const int aiVal, const int aiValMax, UINT anLen)
{
	int2str(apbStr, (aiVal * 10000) / aiValMax, anLen-2);
	apbStr[anLen] = '\0';	// CAUTION!
	apbStr[anLen-1] = '%';
	apbStr[anLen-2] = apbStr[anLen-3];
	apbStr[anLen-3] = apbStr[anLen-4];
	apbStr[anLen-4] = '.';
}

UINT ISPM hex2dec(const UINT hex)
{
	UINT i, d=1, dec=0;

	for(i=0; i<32; i+=4) {
		const UINT digit = (hex>>i)&0xf;
		//dec += ((digit < 0xa) ? digit : 9) * d;
		if(digit < 0xa) dec += digit * d;
		else			dec = d * 10 - 1;
		d *= 10;
	}

	return dec;
}

UINT ISPM dec2hex(UINT dec)
{
	UINT i, h=1, hex=0;

	for(i=0; i<8; i++) {
		const UINT nMod = dec / 10;
		hex += (dec - (nMod * 10)) * h;
		dec = nMod;
		h *= 16;
	}

	return hex;
}

void CheckByteOrdering(void)
{
	const UINT nTestVal = 0x98765432;
	char *pbTest = (char*)(&nTestVal);
	if(*(pbTest+1) == 0x76) TxStr("Byte Ordering : MSB ", 0,0);
	else if(*(pbTest+1) == 0x54) TxStr("Byte Ordering : LSB ", 0,0);
	else TxStr("Byte Ordering : Unknown ", 0,0);
}

//*************************************************************************************************
// Wait
//-------------------------------------------------------------------------------------------------
// Wait time
UINT gnWaitTimeOffset =  0;	// Clock
UINT gnWaitTimeUnit   = 13;	// Clock / [1 cycle of while{ } in WaitUs()], ISPM ON & I-Cache ON,

void ISPM0 WaitUs(UINT anUs)	// MAX = 1000000 / [anCntPerSec in InitTickTimer()], Guarantee 500000 (㎲)
{	// Wait X usec
	const UINT nTargerClk = ((MCLK/10000) * anUs) / 100;	// = 74.25 * anUs,  MCLK/10000 = 7425,

	TICK_STA(1);

	do {
		TICK_END(1);
		if((nTick_1 + gnWaitTimeOffset) >= nTargerClk) break;
	} while(1/*(nTick_1 + gnWaitTimeOffset) < nTargerClk*/);	// It can not be used nTick_1. Because nTick_1 is a local variable.
}

#if 0
void WaitXus(UINT anUs)
{	// Wait X usec
	volatile UINT	i, j;

	for(j=0; j<anUs; j++){
		for(i=0; i<2; i++){
			asm("l.nop" : :);
		}
	}
}

void WaitXms(UINT anMs)
{	// Wait X msec
	volatile UINT	i, j;

	for(j=0; j<anMs; j++){
		for(i=0; i<2000; i++){
			asm("l.nop" : :);
		}
	}
}
#endif


//-------------------------------------------------------------------------------------------------
// check douwnload
#if 0
void ISPM FirmUpdate(BYTE abOn)
{	// Entrance to Boot Loader for UART Program down

	if (!abOn) { return; }

	//SfWrEn;
	//POREG_ENw(1);											// Reg update at VlockO flag
	//WaitVdi();
	//WaitVdi();

	//DI();

	while(gtUartQue.wTxLen != 0);

	//SFSEAw((FLASH_SECT_APP_STT-1)*FLASH_SECT_SIZE)	;		// Sector erase
	//sfls_erase_sect((FLASH_SECT_APP_STT-1)*FLASH_SECT_SIZE);
	sfls_erase_sect(_reset_s - 1);

	//WaitVdi();

	SFLS_WB_WR_EN = 1;
	//sfw8(FLASH_SECT_APP_STT*FLASH_SECT_SIZE-1, 0x80);		// Download flag set
	_wr8(_reset_s - 1, 0x80);

	//POREG_ENw(1);											// Reg update at VlockO flag
	//WaitVdi();
	//WaitVdi();

//	SfWrDis;

//	SetIsp(0x0,0x10420);									// HCLK default 108MHz

	//WdtCfg(200);
	WDT_CNT_LMT = 200 * (MCLK/1000);						// 200 msec

	//POREG_ENw(1);											// Reg update at VlockO flag
	//WaitVdi();

	while(1);												// WDT reset

}
#endif

