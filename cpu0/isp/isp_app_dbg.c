/* **************************************************************************
 File Name	: 	app_dbg.c
 Description:	Debugging & Testing functions
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Date		:	14. 6. 18
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"	// to use fast code

//*************************************************************************************************
// Debugging & Testing functions
//-------------------------------------------------------------------------------------------------
extern volatile UINT gnIsrGpio;
extern volatile UINT gnIsrTimer3;


#define UART_STR(STR)		TxStr(STR, 0, 0)/*Uart0_Str(MSG)*/
#define UART_VAL(STR,VAL)	TxStr(STR, VAL, 8)

UINT gnDispDbgY = DispDbgYmin;
char gbStr[ISP_FONT_MAX_PATH+1+1];

//*************************************************************************************************
// Debugging & Testing functions
//-------------------------------------------------------------------------------------------------
void DispDbgH(char* cStr, const int aiVal, const UINT anLen)
{
	const UINT nLen = DispStr(cStr, gnDispDbgY, DispDbgX, DispDbgStrLen);
	DispHex(gnDispDbgY++, DispDbgX+nLen, aiVal, anLen);
}

void DispDbgD(char* cStr, const int aiVal, const UINT anLen)
{
	const UINT nLen = DispStr(cStr, gnDispDbgY, DispDbgX, DispDbgStrLen);
	DispDec(gnDispDbgY++, DispDbgX+nLen, aiVal, anLen);
}

void DispDbgB(char* cStr, const int aiVal, const UINT anLen)
{
	const UINT nLen = DispStr(cStr, gnDispDbgY, DispDbgX, DispDbgStrLen);
	DispBin(gnDispDbgY++, DispDbgX+nLen, aiVal, anLen);
}

void DispDbgP(char* cStr, const int aiVal, const int aiValMax, const UINT anLen)
{
	const UINT nLen = DispStr(cStr, gnDispDbgY, DispDbgX, DispDbgStrLen);
	DispPer(gnDispDbgY++, DispDbgX+nLen, aiVal, aiValMax, anLen);
}

void TxStrMemAddrInfo0(void)
{
	TxStrNoIRQ("reset_s ",_reset_s,8);

//#if model_Ispm == 1
	TxStrNoIRQ("rom_ispm_text_s ",_ispm_text_lma_s,8);
	TxStrNoIRQ("ispm_text_s ",_ispm_text_s,8);
	TxStrNoIRQ("ispm_text_e ",_ispm_text_e,8);
//#endif

#if 1
	TxStrNoIRQ("rom_data_s ",_data_lma_s,8);
	//TxStrNoIRQ("rom_data_e ",_rom_data_e,8);
	TxStrNoIRQ("data_s ",_data_s,8);
	TxStrNoIRQ("data_e ",_data_e,8);
	TxStrNoIRQ("bss_s ",_bss_s,8);
	TxStrNoIRQ("bss_e ",_bss_e,8);
	TxStrNoIRQ("stack_s ",_stack_s,8);
	TxStrNoIRQ("stack_e ",_stack_e,8);
#endif

#if model_Ispm == 1
	TxStrNoIRQ("ROM  VAL: ", _rd32(_ispm_text_lma_s + 0xa0),8);
	TxStrNoIRQ("ISPM VAL: ", _rd32(_ispm_text_s + 0xa0),8);

	//extern int RxPP(BYTE bIn);
	//TxStr("RxPP: ", (UINT)((void*)RxPP),8);
#endif
}

void TxStrMemAddrInfo(void)
{
	TxStr("reset_s ",_reset_s,8);

#if model_Ispm == 1
	TxStr("rom_ispm_text_s ",_ispm_text_lma_s,8);
	TxStr("ispm_text_s ",_ispm_text_s,8);
	TxStr("ispm_text_e ",_ispm_text_e,8);
#endif

#if 1
	TxStr("rom_data_s ",_data_lma_s,8);
	//TxStr("rom_data_e ",_rom_data_e,8);
	TxStr("data_s ",_data_s,8);
	TxStr("data_e ",_data_e,8);
	TxStr("bss_s ",_bss_s,8);
	TxStr("bss_e ",_bss_e,8);
#endif

#if model_Ispm == 1
	TxStr("ROM  VAL: ", _rd32(_ispm_text_lma_s + 0xa0),8);
	TxStr("ISPM VAL: ", _rd32(_ispm_text_s + 0xa0),8);

	//extern int RxPP(BYTE bIn);
	//TxStr("RxPP: ", (UINT)((void*)RxPP),8);
#endif
}

#if STR_UART_DATA == 1
void ISPMt DispUartData(void)
{
	extern volatile UINT gnIsrUartTx;
	extern volatile UINT gnIsrUartTxF;
	extern volatile UINT gnIsrUartRx;
	extern volatile UINT gnIsrUartRxF;

	const UINT nDispDbgY = gnDispDbgY;
	gnDispDbgY = 31;
	DispDbgD("IsrUartTx :", gnIsrUartTx , 10);
	DispDbgD("IsrUartTxF:", gnIsrUartTxF, 10);
	DispDbgD("IsrUartRx :", gnIsrUartRx , 10);
	DispDbgD("IsrUartRxF:", gnIsrUartRxF, 10);
	
	DispDbgD("wTxLen    :", gtUartQue.wTxLen, 4);
	DispDbgH("pbTxHead  :", (UINT)gtUartQue.pbTxHead, 8);
	DispDbgH("pbTxTail  :", (UINT)gtUartQue.pbTxTail, 8);

	DispDbgD("wRxLen    :", gtUartQue.wRxLen, 4);
	DispDbgH("pbRxHead  :", (UINT)gtUartQue.pbRxHead, 8);
	DispDbgH("pbRxTail  :", (UINT)gtUartQue.pbRxTail, 8);
	gnDispDbgY = nDispDbgY;
}
#endif

void DispTime(UINT anSec)
{
	const UINT nHour = anSec / 3600;
	anSec -= nHour * 3600;
	const UINT nMin  = anSec / 60;
	anSec -= nMin * 60;

	int2str(gbStr  , nHour, 3);	gbStr[3] = ':';
	int2str(gbStr+4, nMin , 2);	gbStr[6] = ':';
	int2str(gbStr+7, anSec, 2);	gbStr[9] = ' ';
	gbStr[10] = 0;

	DispStr((const char*)gbStr, gnDispDbgY++, DispDbgX, 9);			// ISPx
	//Uart0_Str(gbStr);
	//TxStr("TIME: ", anSec, 8);
	//TxStr(gbStr, gtUartQue.wTxLen, 8);
	//TxStr(gbStr, TIM_CH0_PWM_EN/*GPIO_MUX*/, 8);
}

void ISPM TxStrDec2(const char* apbStr0, UINT anVal0, UINT anValLen0, UINT anVal1, UINT anValLen1)
{
	char bStr[11];
	const UINT nValLen0 = MIN(10,anValLen0);
	const UINT nValLen1 = MIN(10,anValLen1);
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + nValLen0 + 1 + nValLen1 + 2;

	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_STR);
 	UartTxByte(bLen);

	if(apbStr0) UartTxStr(apbStr0);

	if(nValLen0) {
		int2str(bStr, anVal0, nValLen0);
		UartTxStr(bStr);
	}

	UartTxByte('-');
	
	if(nValLen1) {
		int2str(bStr, anVal1, nValLen1);
		UartTxStr(bStr);
	}

	UartTxByte('\r');
	UartTxByte('\n');

	UartTxByte(PC_ETX);
}

void TxStrWaitTime(const UINT anUs)
{
	#define WANT_WAIT_TIMEx2	(MCLK/(1000000/(anUs<<1)))//(MCLK/(1000/2))

	//TxStrDecNoIRQ("WaitTimeUnit ", gnWaitTimeUnit,10);
	TxStrDecNoIRQ("WaitTimeOffset ", gnWaitTimeOffset,10);

	//TxStrNoIRQ("WaitUs 1ms", ((MCLK/1000) - gnWaitTimeOffset) / gnWaitTimeUnit ,10);
	//TxStrNoIRQ("WaitUs 1ms", ((WANT_WAIT_TIMEx2 - (gnWaitTimeOffset<<1)) / gnWaitTimeUnit + 1)>>1,10);

#if 0
	Uart0_Tx(PC_STX);
	Uart0_Tx(PC_CMD_STR);
 	Uart0_Tx(8+4+4+5);
	int2str(gbStr, anUs, 8);
	Uart0_Str(gbStr);		// 8
	Uart0_Str("§Á: ");		// 4
	int2str(gbStr, ((WANT_WAIT_TIMEx2 - (gnWaitTimeOffset<<1)) / gnWaitTimeUnit + 1)>>1, 4);
	Uart0_Str(gbStr);		// 4
	Uart0_Str(" WT\r\n");	// 5
	Uart0_Tx(PC_ETX);
#else
	UNUSED_ARG(anUs);
#endif
}

void ISPMt TestWaitTime(const UINT anUs)
{
	FUNC_TIME_OUT_ID(_DI, 1, "WaitUs", DispDbgD, WaitUs(anUs); )
}

void ISPMt DispIsrStatus(void)
{
	extern BYTE gnIsrStatus[7];
	extern BYTE gnIsrStatusPos;
	if(gnIsrStatusPos)
	{
		UINT i;
		for(i=0; i<gnIsrStatusPos; i++) {
			DispDec(gnDispDbgY, DispDbgX+(i*3), gnIsrStatus[i], 2);
			gnIsrStatus[i] = 0;
		}
		gnIsrStatusPos = 0;

		for(; i<numberof(gnIsrStatus); i++) {
			SETFONTID(gnDispDbgY, DispDbgX+(i*3)  , '0');
			SETFONTID(gnDispDbgY, DispDbgX+(i*3)+1, '0');
		}
	}
	gnDispDbgY++;
}

#if 0
void TestAppSavePar(void)
{
	if(gnIsrISP2 == 60) {
		TxStrDec("gnIsrISP2", gnIsrISP2, 10);
		gbUsrParSaveChk = 1;
	}

	if(gnIsrISP2 == 65) {
		TxStrDec("gnIsrISP2", gnIsrISP2, 10);
		gbMnParSaveChk = 1;
	}

	if(gnIsrISP2 == 70) {
		TxStrDec("gnIsrISP2", gnIsrISP2, 10);
		gbUsrDataSaveChk = 1;
	}
}
#endif

/* EOF */

