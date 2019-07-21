/* **************************************************************************
 File Name	: 	dev_gpio.c
 Description:	EN673 - GPIO device driver
 Designer	:	Kim, Sunghoon
 Modifier 	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------

void GpioIsrEn(BYTE abSel)
{
	GpioInDir(abSel);		// (Default)
	GpioFuncPinOff(abSel);	// (Default)
	GpioRiseEdge(abSel);	// (Not Default)
	GpioEi(abSel);			// (Not Default)
}

//*************************************************************************************************
// Isr
//-------------------------------------------------------------------------------------------------
//
volatile UINT gnIsrGpio = 0;

void ISPM0 IsrGpio(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	int		i;
	
	for(i=0; i<GPIO_CNT; i++)
	{
		if( GPIO_IRQ & (0x00000001L << i) )
		{
			GPIO_IRQ_CLR = (0x00000001L << i);						// Irq clear

			gnIsrGpio++;

			//if(gtUartQue.wTxLen < (UART_BUF_SIZE>>1)) TxStrDec("GPIO IRQ :", i, 2);	// CAUTION!!! - If DI() & (gtUartQue.wTxLen >= UART_BUF_SIZE), Cause an infinite loop in UartTxByte()
			UartRstQue();
		}
	}
}

