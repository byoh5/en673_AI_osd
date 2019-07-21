/* **************************************************************************
 File Name	: 	dev_irq.c
 Description:	OR - Interrupt device driver
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"


//*************************************************************************************************
// Interrupt Vect
//-------------------------------------------------------------------------------------------------
extern int IrqDisable(UINT);
extern int IrqEnable(UINT);
//extern void InitEnaIrq(void);

extern void ISPM0 ClrIrqTT(void);

extern void IsrGpio(void);
extern void IsrTimer(void);
extern void IsrUart(void);
extern void IsrUart1(void);
extern void IsrISP(void);
extern void IsrI2c(void);


//*************************************************************************************************
// Isr
//-------------------------------------------------------------------------------------------------
//
void InitIrq0(void)
{
	IntInit();
	IntAdd(IRQ_UART0	, (void*)IsrUart,  0);
	IrqEnable(IRQ_UART0 );
//	InitEnaIrq();
	mtspr(EXR1K_SR, mfspr(EXR1K_SR) | EXR1K_SR_IEE);
}

void InitIrq(void)
{
	//TxStrNoIRQ("IRQ Init...", 0, 0);

	add_xsr(XSR_INT, XsrInt);

#if 1
	//----------------------------------------------------------------------------------------
	// ISP interrupt
	extern void InitIspIrq(void);
	InitIspIrq();
#else
	//----------------------------------------------------------------------------------------
	// ISP interrupt
	VIRQI_ENw(1);		// VLOCKI, Sensor 입력 Sync 기준
	VIRQW_ENw(1);		// VLOCKI, Sensor 입력 Sync 기준 (WDR 기준 Sync)
	VIRQO_ENw(1);		// VLOCKO, ISP 출력 Sync 기준(Low Shutter 제외하고는 VLOKCI와 VLOCKO가 동일 함.)
	TII(UIRQ0_ENw(1);)	// User Interrupt 0 (VLOCKI 기준)
	TII(UIRQ1_ENw(1);)	// User Interrupt 1 (VLOCKO 기준)
	//INT_ENw(1);			// Coaxial Comm.
	//ADC_INT_ENw(1);		// ADC
	//USIRQ_ENw(1);		// Upstream
#endif

	//----------------------------------------------------------------------------------------
	// Interrupt handler initialize
	IntInit();

	//----------------------------------------------------------------------------------------
	// Isr registration
	IntAdd(IRQ_ISP  , (void*)IsrISP,   0);			// ISPx
#ifdef __IRQ_UART0__
	IntAdd(IRQ_UART0, (void*)IsrUart,  0);
#endif
//	IntAdd(IRQ_UART1, (void*)IsrUart1,  0);

	IntAdd(IRQ_TIMER, (void*)IsrTimer, 0);
//		IntAdd(IRQ_GPIO	, (void*)IsrGpio,  0);

#if model_I2cSlave == 1 || model_I2cSlave == 2
	IntAdd(IRQ_I2C0	, (void*)IsrI2c,  0);
#endif


	//----------------------------------------------------------------------------------------
	// Irq enable
	IrqEnable(IRQ_ISP   );			// ISPx
#ifdef __IRQ_UART0__
	IrqEnable(IRQ_UART0 );
#endif
//	IrqEnable(IRQ_UART1 );

	IrqEnable(IRQ_TIMER );
//		IrqEnable(IRQ_GPIO	);

#if model_I2cSlave == 1 || model_I2cSlave == 2
	IrqEnable(IRQ_I2C0	);
#endif

	//----------------------------------------------------------------------------------------
	// Global interrupt enable
//	InitEnaIrq();
	mtspr(EXR1K_SR, mfspr(EXR1K_SR) | EXR1K_SR_IEE);

#if (model_SensIntf==1)
	//gbParallelOn = 1;
#endif
}


