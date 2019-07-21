/* **************************************************************************
 File Name	: 	dev_isp.c
 Description:	EN673 - ISP device driver
 Designer	:	Sung, Min-Je / Kim, Sung-Hoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 08. 13
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

UINT gnFontdummy;

volatile BYTE gbVdiIrq = 0;
volatile BYTE gbVdwIrq = 0;
volatile BYTE gbVdoIrq = 0;
volatile BYTE gbJpgIrq = 0;

volatile BYTE gbVdiIrqOn = 0;
volatile BYTE gbVdwIrqOn = 0;

#if 0
void InitWaitVd(const int aiTestPatternOn)
{
	if(gbVdiIrqOn == 0 && gbVdwIrqOn == 0) {
		TxStrDecNoIRQ("Wait Vd Init...", aiTestPatternOn, 1);
		WaitUs(33333);
	}

	if(gbVdiIrqOn == 0) {
		if(gbVdiIrq) {
			gbVdiIrqOn = 1;
			TxStrDecNoIRQ("  >>VDI OK...", model_Save, 0);
		}
		else {
			TxStrDecNoIRQ("  >>NO VDI...", model_Save, 0);
		}
	}

	if(gbVdwIrqOn == 0) {
		if(gbVdwIrq) {
			gbVdwIrqOn = 1;
			TxStrDecNoIRQ("  >>VDW OK...", model_Save, 0);
		}
		else {
			TxStrDecNoIRQ("  >>NO VDW...", model_Save, 0);
		}
	}
	TxStrNoIRQ("    ", GetIsp(0x40), 8);	// 2015127 - WHL

	if(!gbVdiIrqOn && aiTestPatternOn) {
		TestPatternOn();
	}
}

void ISPM0 WaitVdi(void)
{	// Wait VDI interrupt
	if(gbVdiIrqOn) {
		while(!gbVdiIrq);
		gbVdiIrq = 0;
	}
	else {
		WaitUs(33333);
	}
}

void ISPM0 WaitVdw(void)
{	// Wait VDW interrupt
	if(gbVdwIrqOn) {
		while(!gbVdwIrq);
		gbVdwIrq = 0;
	}
	else {
		WaitUs(33333);
	}
}
#endif

#if 0
void ISPM0 WaitVdo(void)
{	// Wait VDO interrupt
	while(!gbVdoIrq);
	gbVdoIrq = 0;
}
#endif

volatile UINT gnIsrISP0 = 0;
volatile UINT gnIsrISP1 = 0;
volatile UINT gnIsrISP2 = 0;
volatile UINT gnIsrISP5 = 0;

volatile UINT gnIsrJpeg = 0;

TII(volatile UINT gnIsrISP3 = 0;)
TII(volatile UINT gnIsrISP4 = 0;)
TII(volatile UINT gnTick_ISP = 0;)

BYTE gnIsrStatus[7];
BYTE gnIsrStatusPos = 0;

extern void enx_isp_i_irq_handler(u32 mode, u32 wdr);
extern void enx_isp_o_irq_handler(u32 mode, u32 wdr);
extern void enx_isp_d_irq_handler(u32 mode);
extern void enx_isp_irq_handler(u32 mode, u32 wdr, u32 dss);
extern void jpeg_irq_handler(void *ctx);

extern void LED_Disp(void);
//extern void IF_Funcs(void);
extern void isp_main(void);
extern void isp_main_sub(void);

UINT gnHTBTR_Start=0, gnHTBTR_End=0;

UINT framecnt = 0;

UINT ISPM0 Start_HTBTR(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	return mfspr(EXR1K_HTBTR);
}

UINT ISPM0 End_HTBTR(UINT nStart)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	UINT nEnd_HTBTR = mfspr(EXR1K_HTBTR);
	UINT dep, time_us;

	if(nStart < nEnd_HTBTR)
		dep = nEnd_HTBTR - nStart ;
	else
		dep = (0xFFFFFFFF - nStart ) + nEnd_HTBTR;
		time_us = dep/(BUS_CLK/1000000);

	return time_us;
}

BYTE gbIsp_Init_Flag=1;
extern BYTE gbConnection_Freq; // 0 : 50Hz, 1 : 60Hz
#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
BYTE gbIsrIrqState=0;
#endif
void ISPM0 IsrISP(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	while(1) {	// 2015129 - WHL
		const WORD IsrStatus = ISP_RIRQ0r;
		if(IsrStatus==0) break;

		if(gnIsrStatusPos < numberof(gnIsrStatus)) {
			gnIsrStatus[gnIsrStatusPos++] = IsrStatus;
		}

	//if(gtUartQue.wTxLen < (UART_BUF_SIZE>>1)) TxStr("IsrISP:", IsrStatus, 1);
#if (model_SensIntf==1)
	/*extern BYTE gbParallelOn;
	if(gbParallelOn == 0 && gnIsrISP0 > 100) {
		TxStrNoIRQ("1...", 0, 0);
	}*/
#endif

		TII(static UINT nTickSta_ISP = 0;)

		//TxStrNoIRQ("----->", IsrStatus, 8);

	#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))
		/*0x01*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKI )) { CLI_VLOCKIw(1); gbVdiIrq = 1; gnIsrISP0++; enx_isp_i_irq_handler(FPS_VDO, gbWdrOn); }
		/*0x02*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW )) { CLI_VLOCKWw(1); gbVdwIrq = 1; gnIsrISP1++; }
		/*0x04*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKO )) { CLI_VLOCKOw(1); gbVdoIrq = 1; gnIsrISP2++; if(!gbIsp_Init_Flag) { if(!gnClkChgCnt) enx_isp_o_irq_handler(FPS_VDO, gbWdrOn); LED_Disp(); gnHTBTR_Start=Start_HTBTR(); isp_main(); gnHTBTR_End=End_HTBTR(gnHTBTR_Start); /*IF_Funcs();*/ } }
		/*0x08*/if(IsrStatus&(0x1<<ISP_IRQ_JPGEND )) { CLI_JPGw(1); gbJpgIrq = 1; if(!gnClkChgCnt) jpeg_irq_handler(NULL); }
		/*0x10*/if(IsrStatus&(0x1<<ISP_IRQ_USER0  )) { CLI_UIRQ0w(1); TII(nTickSta_ISP = GetTT(); gnIsrISP3++;) }					// VDI 기준
		/*0x20*/if(IsrStatus&(0x1<<ISP_IRQ_USER1  )) { CLI_UIRQ1w(1); /*FreqAdjust();*/ /*FpsClkSet();*/ TII(TICK_END(ISP); gnTick_ISP = nTick_ISP; gnIsrISP4++;) }	// VDO 기준
 		/*0x40*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW2)) { CLI_VLOCKW2w(1); enx_isp_irq_handler(FPS_VDO, gbWdrOn, giCurDss); gnIsrISP5++;}	// VDI & W

	#elif(IMAGE_INPUT_MODE==2)	// BT656 Input
		/*0x01*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKI )) { CLI_VLOCKIw(1); gbVdiIrq = 1; gnIsrISP0++; }
		/*0x02*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW )) { CLI_VLOCKWw(1); gbVdwIrq = 1; gnIsrISP1++; }
		/*0x04*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKO )) { CLI_VLOCKOw(1); gbVdoIrq = 1; gnIsrISP2++; }
		/*0x08*/if(IsrStatus&(0x1<<ISP_IRQ_JPGEND )) { CLI_JPGw(1); gbJpgIrq = 1; jpeg_irq_handler(NULL); gnIsrJpeg++; }
		/*0x10*/if(IsrStatus&(0x1<<ISP_IRQ_USER0  )) { CLI_UIRQ0w(1);	TII(nTickSta_ISP = GetTT(); gnIsrISP3++;) }					// VDI 기준
		/*0x20*/if(IsrStatus&(0x1<<ISP_IRQ_USER1  )) { CLI_UIRQ1w(1); TII(TICK_END(ISP); gnTick_ISP = nTick_ISP; gnIsrISP4++;) }	// VDO 기준
		/*0x40*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW2)) { CLI_VLOCKW2w(1); }	// VDI & W

	#elif(IMAGE_INPUT_MODE==3)	// BT1120 Input
		/*0x01*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKI )) { CLI_VLOCKIw(1);framecnt++; gbVdiIrq = 1; gnIsrISP0++; enx_isp_i_irq_handler(BT1120_FPS, 0); }
		/*0x02*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW )) { CLI_VLOCKWw(1); gbVdwIrq = 1; gnIsrISP1++; }
		/*0x04*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKO )) { CLI_VLOCKOw(1); gbVdoIrq = 1; gnIsrISP2++;enx_isp_d_irq_handler(BT1120_FPS); }
		/*0x08*/if(IsrStatus&(0x1<<ISP_IRQ_JPGEND )) { CLI_JPGw(1); gbJpgIrq = 1; jpeg_irq_handler(NULL); gnIsrJpeg++; }
		/*0x10*/if(IsrStatus&(0x1<<ISP_IRQ_USER0  )) { CLI_UIRQ0w(1);	TII(nTickSta_ISP = GetTT(); gnIsrISP3++;) }					// VDI 기준
		/*0x20*/if(IsrStatus&(0x1<<ISP_IRQ_USER1  )) { CLI_UIRQ1w(1); TII(TICK_END(ISP); gnTick_ISP = nTick_ISP; gnIsrISP4++;) }	// VDO 기준
		/*0x40*/if(IsrStatus&(0x1<<ISP_IRQ_VLOCKW2)) { CLI_VLOCKW2w(1); enx_isp_irq_handler(BT1120_FPS, 0, 0); }	// VDI & W

	#endif
	}
}

//----------------------------------------------------------------------------------------
#if (model_IspRegCheck==1)
void ISPM0 _wr8_fnc(const UINT anAddr, const BYTE abData)
{
	(*((volatile BYTE *)(anAddr)) = (abData));
}

BYTE gbOnCnt = 0;
BYTE gbOffCnt = 0;
BYTE gbCntStop = 0;

void ISPM0 _wr32_fnc(const UINT anAddr, const UINT anData)
{
#if (model_SensIntf==1)
	#define TRACE_ADDR		(_REG_BASE_+(0x000f<<2))		// BT_DIN_SEL
	#define TRACE_BITH		27
	#define TRACE_BITL		27
#else
	//#define TRACE_ADDR		(_REG_BASE_+(0x0027<<2))		// SDES_PD
	//#define TRACE_BITH		0
	//#define TRACE_BITL		0
	#define TRACE_ADDR		(_REG_BASE_+(0x0028<<2))		// LVDS_RXON
	#define TRACE_BITH		31
	#define TRACE_BITL		31
#endif

	if(anAddr == TRACE_ADDR) {
		const UINT nPD = GetBit(_rd32(TRACE_ADDR),TRACE_BITH,TRACE_BITL);

		(*((volatile UINT *)(anAddr)) = (anData));

		if(gbCntStop) return;

		const UINT anOn = GetBit(anData,TRACE_BITH,TRACE_BITL);

		if(nPD >= 1 && anOn == 0) gbOffCnt++;
		if(nPD == 0 && anOn >= 1) gbOnCnt++;
	}
	else {
		(*((volatile UINT *)(anAddr)) = (anData));
	}
}
#endif

//*************************************************************************************************
// Utillity
//-------------------------------------------------------------------------------------------------
// Font
void ISPM SetFontAttrs(UINT anPosY, UINT anPosX, UINT anLen, UINT anAttr)
{	// Font attributes setting
	UINT i;
	for(i=0; i<anLen; i++) {
		SETFONTATTR(anPosY, anPosX+i, anAttr);
	}
}

UINT gnDispSpr=0;
UINT ISPM DispStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anLen)
{
	UINT i;
	
	gnDispSpr = mfspr(EXR1K_SR);

	for(i=0; i<anLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (BYTE)cStr[i]);
	}
	return i;
}

void ISPM DispClr(UINT anPosY, UINT anPosX, UINT anLen)
{
    UINT i;
	for(i=0; i<anLen; i++) {
		SETFONTID(anPosY, anPosX+i, SPACE_CHAR_ID);
	}
}

void ISPM DispClrStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen)
{
	UINT i;

	for(i=0; i<anStrLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (BYTE)cStr[i]);
	}

	for(; i<anClrLen; i++) {
		SETFONTID(anPosY, anPosX+i, SPACE_CHAR_ID);
	}
}

#if model_CharMax == 1
void ISPM DispStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anLen)
{
	UINT i;
	for(i=0; i<anLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (EXCH)(*(cStr+i)));
	}
}

void ISPM DispClrStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen)
{
	UINT i;

	for(i=0; i<anStrLen && (*(cStr+i)); i++) {
		SETFONTID(anPosY, anPosX+i, (EXCH)(*(cStr+i)));
	}

	for(; i<anClrLen; i++) {
		SETFONTID(anPosY, anPosX+i, 0/*' '*/);
	}
}
#endif

#define ISP_REG_WR_uDLY		0 // 100
void ISPM Wr8_Dly(UINT anAddr, BYTE abData)
{
	(*((volatile BYTE *)(anAddr)) = (abData));
	WaitUs(ISP_REG_WR_uDLY);
}

void ISPM Wr32_Dly(UINT anAddr, UINT anData)
{
	(*((volatile UINT *)(anAddr)) = (anData));
	WaitUs(ISP_REG_WR_uDLY);
}

#define ISP_REG_RD_uDLY		10
//BYTE ISPM Rd8_Dly(BYTE abAddr)
//{
//	BYTE bRd8_Buf = (*((volatile BYTE *)(abAddr)));
//	WaitUs(ISP_REG_RD_uDLY);
//	
//	return bRd8_Buf;
//}

UINT ISPM Rd32_Dly(UINT anAddr)
{
	UINT nRd32_Buf = (*((volatile UINT *)(anAddr)));
	WaitUs(ISP_REG_RD_uDLY);
	
	return nRd32_Buf;
}


/* EOF */

