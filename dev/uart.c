/***************************************************************************
 File Name	: 	uart.c
 Description:	EN673 - UART Device Driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev.h"
#include "dev_inline.h"

char gUartNumcode[] = "0123456789ABCDEF";

void Uart0_Init(UINT Speed_bps)
{
#ifdef _SIM_MODE
	UART0_CLKDIV = 0;
#else
	UART0_CLKDIV = ((BUS_CLK+(Speed_bps<<3))/(Speed_bps<<4)) - 1;
#endif
	UART0_STOP_BIT = 0;
	UART0_PARITY_EN = 0;
	UART0_RX_IRQ_EN = 0;
	UART0_TX_IRQ_EN = 0;
	UART0_TX_TYPE = 1;
}

void Uart0_Deinit(void)
{
	UART0_CLKDIV = 0;
	UART0_STOP_BIT = 0;
	UART0_PARITY_EN = 0;
	UART0_RX_IRQ_EN = 0;
	UART0_TX_IRQ_EN = 0;
}

#ifndef _INLINE
_NOINS void Uart0_Tx(char dat)
{
	while(UART0_TX_FULL);
	UART0_TX_DAT = dat;
}

_NOINS void Uart0_Rx(char *dat)
{
	while(UART0_RX_EMPTY);
	*dat = (char)UART0_RX_DAT;
}

_NOINS BYTE Uart0_RxExist(void)
{
	return !UART0_RX_EMPTY;							// Rx data ready
}

_NOINS BYTE Uart0_GetByte(void)
{
	return UART0_RX_DAT;							// Get
}
#endif

_NOINS void Uart0_Str(const char *text)
{
	while(*text) {
		if(*text == '\n')	Uart0_Tx('\r');
		Uart0_Tx(*text++);
	}
}

_NOINS void Uart0_Byte(BYTE data)
{
	Uart0_Num((data&0xf0)>>4);
	Uart0_Num((data&0x0f));
}

_NOINS void Uart0_Hex(int data)
{
	int i;
	for(i=28; i>=0; i-=4) Uart0_Num((data>>i)&0x0000000f);
}

_NOINS void Uart0_Dec(unsigned char data)
{
	Uart0_Num(data/100);
	Uart0_Num((data/10)%10);
	Uart0_Num(data%10);
}

_NOINS void Uart0_Printf(char *text, int dat)
{
	Uart0_Str(text);
	Uart0_Hex(dat);
	Uart0_Tx(0x0d);
	Uart0_Tx(0x0a);
}

#ifndef DEF_BOOT
void Uart1_Init(UINT Speed_bps)
{
	GpioFuncPin(GPIO_16);
	GpioFuncPin(GPIO_17);

#ifdef _SIM_MODE
	UART1_CLKDIV = 0;
#else
	UART1_CLKDIV = ((BUS_CLK+(Speed_bps<<3))/(Speed_bps<<4));
#endif
	UART1_STOP_BIT = 0;
	UART1_PARITY_EN = 0;
	UART1_RX_IRQ_EN = 0;
	UART1_TX_IRQ_EN = 0;
}

void Uart1_Deinit(void)
{
	UART1_CLKDIV = 0;
	UART1_STOP_BIT = 0;
	UART1_PARITY_EN = 0;
	UART1_RX_IRQ_EN = 0;
	UART1_TX_IRQ_EN = 0;

	GpioFuncPinOff(GPIO_16);
	GpioFuncPinOff(GPIO_17);
}

#ifndef _INLINE
_NOINS void Uart1_Tx(char dat)
{
	while(UART1_TX_FULL);
	UART1_TX_DAT = dat;
}

_NOINS void Uart1_Rx(char *dat)
{
	while(UART1_RX_EMPTY);
	*dat = (char)UART1_RX_DAT;
}

_NOINS BYTE Uart1_RxExist(void)
{
	return !UART1_RX_EMPTY;							// Rx data ready
}

_NOINS BYTE Uart1_GetByte(void)
{
	return UART1_RX_DAT;							// Get
}
#endif

_NOINS void Uart1_Str(const char *text)
{
	while(*text) {
		if(*text == '\n')	Uart1_Tx('\r');
		Uart1_Tx(*text++);
	}
}

_NOINS void Uart1_Hex(int data)
{
	int i;
	for(i=28; i>=0; i-=4) Uart1_Num((data>>i)&0x0000000f);
}

#if 0
void Uart1_Echo(void)
{
	BYTE get;

	Uart1_Init(eUARTspeed115200);

	while(1)
	{
		while(Uart1_RxExist() == 0)
			vTaskDelay(1);

		get = Uart1_GetByte();

		Uart1_Tx(get);

		vTaskDelay(1);
	}
}
#endif

void Uart2_Init(UINT Speed_bps)
{
	GpioFuncPin(GPIO_14);
	GpioFuncPin(GPIO_15);

#ifdef _SIM_MODE
	UART2_CLKDIV = 0;
#else
	UART2_CLKDIV = ((BUS_CLK+(Speed_bps<<3))/(Speed_bps<<4));
#endif
	UART2_STOP_BIT = 0;
	UART2_PARITY_EN = 0;
	UART2_RX_IRQ_EN = 0;
	UART2_TX_IRQ_EN = 0;
}

void Uart2_Deinit(void)
{
	UART2_CLKDIV = 0;
	UART2_STOP_BIT = 0;
	UART2_PARITY_EN = 0;
	UART2_RX_IRQ_EN = 0;
	UART2_TX_IRQ_EN = 0;

	GpioFuncPinOff(GPIO_14);
	GpioFuncPinOff(GPIO_15);
}

#ifndef _INLINE
_NOINS void Uart2_Tx(char dat)
{
	while(UART2_TX_FULL);
	UART2_TX_DAT = dat;
}

_NOINS void Uart2_Rx(char *dat)
{
	while(UART2_RX_EMPTY);
	*dat = UART2_RX_DAT;
}

_NOINS BYTE Uart2_RxExist(void)
{
	return !UART2_RX_EMPTY;							// Rx data ready
}

_NOINS BYTE Uart2_GetByte(void)
{
	return UART2_RX_DAT;							// Get
}
#endif
#endif
