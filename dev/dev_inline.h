//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		dev_inline.h
//	Description:	EN673 device inline function definition fpr ISP/IP
//	Author:			EN673 team
//
//	Note:			DO NOT INCLUDE IN ANY OTHER HEADER!!
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151030		ygkim		first designed
//------------------------------------------------------------------------------
#ifdef _INLINE
//#ifndef _DEV_INLINE_H_
//#define _DEV_INLINE_H_

#include "exr1k_inline.h"
#include "msg.h"
#ifdef DEF_CPU1
#include "FreeRTOS.h"
#endif

/*
 INDEX	:
		 1. TYPE DEF
		 2. Memory Size & Base Address Definition
		 3. IRQ Number Def
		 4. SYSTEM
		 5. UTIL
		 6. ADC
		 7. CPU
		 8. DDR
		 9. DMA
		10. GPIO
		11. ISR
		12. SERIAL FLASH (ISP)
		13. TIMER
		14. ETHERNET (IP Only)
		15. H.264 (IP Only)
		16. SDIO (IP Only)
		17. I2C
		18. AUD
		19. SPI
		20. UART
		21. UTIL
		22. WDT
		23. DEVICE TEST
		24. ISP
*/

//******************************************************************************
// 1. TYPE DEF
//------------------------------------------------------------------------------


//******************************************************************************
// 2. Memory Size & Base Address Definition
//------------------------------------------------------------------------------


//******************************************************************************
// 3. IRQ Number Def
//------------------------------------------------------------------------------


//******************************************************************************
// 4. SYSTEM
//------------------------------------------------------------------------------
INLINE void WdtSysSetStop(void)
{
	SYS_WDT_T = 0;		// WDT Disable
}

INLINE void WdtSysCountReset(void)
{
#ifdef DEF_BOOT
	gptMsgShare.CPU1_WDT_COUNT = 0;	// Count Reset
	SYS_WDT_T = 0x3;	// WDT Enable + Count Reset
#endif
#ifdef DEF_CPU0
	SYS_WDT_T = 0x3;	// WDT Enable + Count Reset
#endif
#ifdef DEF_CPU1
	gptMsgShare.CPU1_WDT_COUNT = 0; // Count Reset
#endif
}

INLINE void WdtSysSetTime(UINT sec)
{
	SYS_WDT_LMT = sec * BUS_CLK;
	WdtSysCountReset();
}

INLINE void WdtSysReboot(void)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif
	SYS_WDT_LMT = 0;
	SYS_WDT_T = 0x3;
	while(1);
}

//******************************************************************************
// 5. UTIL
//------------------------------------------------------------------------------


//******************************************************************************
// 6. ADC
//------------------------------------------------------------------------------


//******************************************************************************
// 7. CPU
//------------------------------------------------------------------------------
#ifndef DEF_BOOT
INLINE void WdtCpuSetStop(void)
{
#ifdef DEF_CPU1
	CPU_WDT1_T = 0x0;
#else
	CPU_WDT0_T = 0x0;
#endif
}

INLINE void WdtCpuCountReset(void)
{
#ifdef DEF_CPU1
	CPU_WDT1_T = 0x3;
	CPU_WDT1_T = 0x2;
#else
	CPU_WDT0_T = 0x3;
	CPU_WDT0_T = 0x2;
#endif
}

INLINE void WdtCpuSetTime(UINT sec)
{
#ifdef DEF_CPU1
	CPU_WDT1_LMT = BUS_CLK * sec;
#else
	CPU_WDT0_LMT = BUS_CLK * sec;
#endif
	WdtCpuCountReset();
}

INLINE void WdtCpuReboot(void)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif
	WdtCpuSetTime(0);
	while(1);
}
#endif

//******************************************************************************
// 8. DDR
//------------------------------------------------------------------------------


//******************************************************************************
// 9. DMA
//------------------------------------------------------------------------------
INLINE void DmaEi(void)
{	// Interrupt enable
	DMA_IRQ_EN = 1;
}

INLINE void DmaDi(void)
{	// Interrupt disable
	DMA_IRQ_EN = 0;
}

INLINE void DmaMemCpy_isr_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 0;
	DMA_DST = (UINT)apbDst;
	DMA_SRC = (UINT)apbSrc;
	DMA_LEN = (UINT)anNum;
	DMA_GO  = 1;
}

INLINE void DmaMemCpy_isr(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 0;
	DMA_DST = (UINT)apbDst;
	DMA_SRC = (UINT)apbSrc;
	DMA_LEN = (UINT)anNum;
	DMA_GO  = 1;
	while(DMA_GO);
}

INLINE void DmaMemSet_isr(BYTE *apbDst, BYTE abVal, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 1;
	DMA_DST = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_VALUE = abVal;
	DMA_GO  = 1;
	while(DMA_GO);
}

INLINE void DmaDChkSum_isr_async(BYTE *apbDst, UINT anNum)
{
	while(DMA_GO);
	DMA_SRC = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_MODE= 2;
	DMA_CHKSUM_CLR = 1;
	DMA_GO  = 1;
}

#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
INLINE void DmaDChkSum_isr(BYTE *apbDst, UINT anNum, WORD *apnChkSum)
{
	while(DMA_GO);
	DMA_SRC = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_MODE= 2;
	DMA_CHKSUM_CLR = 1;
	DMA_GO  = 1;
	while(DMA_GO);
	*apnChkSum = SWAP_BYTES_IN_WORD(DMA_CHKSUM);	// Checksum(16 bit accumulator)
}

//******************************************************************************
// 10. GPIO
//------------------------------------------------------------------------------
INLINE void GpioEi(BYTE abSel)
{	// Direction : Gpio interrupt enable
	GPIO_IRQ_EN |= (0x00000001L<<abSel);
}

INLINE void GpioDi(BYTE abSel)
{	// Direction : Gpio interrupt disable
	GPIO_IRQ_EN &= ~(0x00000001L<<abSel);
}

INLINE void GpioRiseEdge(BYTE abSel)
{	// Direction : Gpio rising edge sel
	GPIO_EDGE_DIR |= (0x00000001L<<abSel);
}

INLINE void GpioFallEdge(BYTE abSel)
{	// Direction : Gpio falling edge sel
	GPIO_EDGE_DIR &= ~(0x00000001L<<abSel);
}

INLINE void GpioInDir(BYTE abSel)
{	// Direction : Gpio direction set to output
	GPIO_OEN |= (0x00000001L<<abSel);
}

INLINE void GpioOutDir(BYTE abSel)
{	// Direction : Gpio direction set to input
	GPIO_OEN &= ~(0x00000001L<<abSel);
}

INLINE BYTE GpioGetDir(BYTE abSel)
{	// Direction : Gpio direction get to input
	return (BYTE)((GPIO_OEN & (0x00000001L<<abSel)) >> abSel);
}

INLINE void GpioSetHi(BYTE abSel)
{	// Output Pin Setting : HIGH
	GPIO_O |=  (0x00000001L << abSel);
}

INLINE void GpioSetLo(BYTE abSel)
{	// Output Pin Setting : LOW
	GPIO_O &= ~(0x00000001L << abSel);
}

INLINE void GpioFuncPin(BYTE abSel)
{	// Multi Function Selectin
	SYS_IO_MUX |= (0x00000001L<<abSel);
}

INLINE void GpioFuncPinOff(BYTE abSel)
{	// GPIO Selectin
	SYS_IO_MUX &= ~(0x00000001L<<abSel);
}

INLINE BYTE GpioGetFuncPin(BYTE abSel)
{	// Multi Function Selectin
	return (BYTE)((SYS_IO_MUX & (0x00000001L<<abSel)) >> abSel);
}

INLINE BYTE GpioGetPin(BYTE abSel)
{
	return (BYTE)((GPIO_I & (0x00000001L<<abSel)) >> abSel);
}

INLINE void GpioPullUpEnable(BYTE abSel)
{	// GPIO Pull-up Enable(0:on)
	PAD_GPIO_PUEN &= ~(0x00000001L<<abSel);
}

INLINE void GpioPullUpDisable(BYTE abSel)
{	// GPIO Pull-up Disable(1:off)
	PAD_GPIO_PUEN |= (0x00000001L<<abSel);
}

//******************************************************************************
// 11. ISR
//------------------------------------------------------------------------------


//******************************************************************************
// 12. SERIAL FLASH (ISP)
//------------------------------------------------------------------------------


//******************************************************************************
// 13. TIMER
//------------------------------------------------------------------------------


//******************************************************************************
// 14. ETHERNET
//------------------------------------------------------------------------------


//******************************************************************************
// 15. H.264
//------------------------------------------------------------------------------


//******************************************************************************
// 16. SDIO
//------------------------------------------------------------------------------


//******************************************************************************
// 17. I2C
//------------------------------------------------------------------------------


//******************************************************************************
// 18. AUD
//------------------------------------------------------------------------------



//******************************************************************************
// 19. SPI
//------------------------------------------------------------------------------


//******************************************************************************
// 20. UART
//------------------------------------------------------------------------------
INLINE void Uart0_Tx(char dat)
{
	while(UART0_TX_FULL);
	UART0_TX_DAT = dat;
}

INLINE void Uart0_Rx(char *dat)
{
	while(UART0_RX_EMPTY);
	*dat = (char)UART0_RX_DAT;
}

INLINE BYTE Uart0_RxExist(void)	
{
	return !UART0_RX_EMPTY;							// Rx data ready
}

INLINE BYTE Uart0_GetByte(void)
{
	return UART0_RX_DAT;							// Get
}

INLINE void Uart1_Tx(char dat)
{
	while(UART1_TX_FULL);
	UART1_TX_DAT = dat;
}

INLINE void Uart1_Rx(char *dat)	
{
	while(UART1_RX_EMPTY);
	*dat = (char)UART1_RX_DAT;
}

INLINE BYTE Uart1_RxExist(void)
{
	return !UART1_RX_EMPTY;							// Rx data ready
}

INLINE BYTE Uart1_GetByte(void)
{
	return UART1_RX_DAT;							// Get
}

INLINE void Uart2_Tx(char dat)
{
	while(UART2_TX_FULL);
	UART2_TX_DAT = dat;
}

INLINE void Uart2_Rx(char *dat)
{
	while(UART2_RX_EMPTY);
	*dat = UART2_RX_DAT;
}

INLINE BYTE Uart2_RxExist(void)
{
	return !UART2_RX_EMPTY;							// Rx data ready
}

INLINE BYTE Uart2_GetByte(void)
{
	return UART2_RX_DAT;							// Get
}

//******************************************************************************
// 21. UTIL
//------------------------------------------------------------------------------

//******************************************************************************
// 22. WDT
//------------------------------------------------------------------------------


//******************************************************************************
// 23. DEVICE TEST
//------------------------------------------------------------------------------

//******************************************************************************
// 24. ISP
//------------------------------------------------------------------------------
#if 0
INLINE void DispStr(const char* cStr, UINT anPosX, UINT anPosY)
{
#define	ISP_BASE_FONT_ID	0x1000
#define ISP_FONT_EAX		46
#define ISP_FONT_MAX		20

	UINT i, nFontdummy, nPos;
//	nPos = ISP_BASE_FONT_ID + (anPosY * ISP_FONT_EAX) + anPosX;
	nPos = ISP_REG_BASE + ((ISP_BASE_FONT_ID + (anPosY * ISP_FONT_EAX) + anPosX) << 2);
	for(i=0; i<ISP_FONT_MAX && (*(cStr+i)); i++)
	{
//		_wr32(ISP_REG_BASE+(((UINT)(nPos+i))<<2), ((UINT)((BYTE)cStr[i])&0x1ff));
//		nFontdummy = *(volatile unsigned int*)(ISP_REG_BASE+(((UINT)(nPos+i))<<2));
		_wr32(nPos+(i<<2), ((UINT)((BYTE)cStr[i])&0x1ff));
		nFontdummy = *(volatile unsigned int*)(nPos+(i<<2));
	}
}
#endif

//#endif//_DEV_INLINE_H_
#endif//_INLINE
