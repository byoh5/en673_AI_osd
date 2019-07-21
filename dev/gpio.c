/***************************************************************************
 File Name	: 	gpio.h
 Description:	EN673 - GPIO Device Driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev.h"

#ifndef _INLINE
_NOINS void GpioEi(BYTE abSel)
{	// Direction : Gpio interrupt enable
	GPIO_IRQ_EN |= (0x00000001L<<abSel);
}

_NOINS void GpioDi(BYTE abSel)
{	// Direction : Gpio interrupt disable
	GPIO_IRQ_EN &= ~(0x00000001L<<abSel);
}

_NOINS void GpioRiseEdge(BYTE abSel)
{	// Direction : Gpio rising edge sel
	GPIO_EDGE_DIR |= (0x00000001L<<abSel);
}

_NOINS void GpioFallEdge(BYTE abSel)
{	// Direction : Gpio falling edge sel
	GPIO_EDGE_DIR &= ~(0x00000001L<<abSel);
}

_NOINS void GpioInDir(BYTE abSel)
{	// Direction : Gpio direction set to output
	GPIO_OEN |= (0x00000001L<<abSel);
}

_NOINS void GpioOutDir(BYTE abSel)
{	// Direction : Gpio direction set to input
	GPIO_OEN &= ~(0x00000001L<<abSel);
}

_NOINS BYTE GpioGetDir(BYTE abSel)
{	// Direction : Gpio direction get to input
	return (BYTE)((GPIO_OEN & (0x00000001L<<abSel)) >> abSel);
}

_NOINS void GpioSetHi(BYTE abSel)
{	// Output Pin Setting : HIGH
	GPIO_O |=  (0x00000001L << abSel);
}

_NOINS void GpioSetLo(BYTE abSel)
{	// Output Pin Setting : LOW
	GPIO_O &= ~(0x00000001L << abSel);
}

_NOINS void GpioFuncPin(BYTE abSel)
{	// Multi Function Selection
	SYS_IO_MUX |= (0x00000001L<<abSel);
}

_NOINS void GpioFuncPinOff(BYTE abSel)
{	// GPIO Selectin
	SYS_IO_MUX &= ~(0x00000001L<<abSel);
}

_NOINS BYTE GpioGetFuncPin(BYTE abSel)
{	// Multi Function Selection
	return (BYTE)((SYS_IO_MUX & (0x00000001L<<abSel)) >> abSel);
}

_NOINS BYTE GpioGetPin(BYTE abSel)
{
	return (BYTE)((GPIO_I & (0x00000001L<<abSel)) >> abSel);
}

_NOINS void GpioPullUpEnable(BYTE abSel)
{	// GPIO Pull-up Enable(0:on)
	PAD_GPIO_PUEN &= ~(0x00000001L<<abSel);
}

_NOINS void GpioPullUpDisable(BYTE abSel)
{	// GPIO Pull-up Disable(1:off)
	PAD_GPIO_PUEN |= (0x00000001L<<abSel);
}

#endif//_INLINE

#ifdef DEF_CPU1
void IsrGpio(void *ctx)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	int i;
	for(i=0;i<GPIO_CNT;i++)
	{
		if(GPIO_IRQ & (0x00000001L << i))
		{
			GPIO_IRQ_CLR = (0x00000001L << i);						// Irq clear
			switch(i)
			{
				default:
					printf("GPIO Get[%d]\r\n", i);
					break;
			}
		}
	}
	UNUSED(ctx);
}
#endif