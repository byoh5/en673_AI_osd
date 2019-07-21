/***************************************************************************
 File Name	: 	spi.c
 Description:	EN673 - SPI device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 25
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"
#include "dev_inline.h"

void Spi0_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection)
{
	SPI0_EN      = 1;			// SPI controller enabled
	SPI0_WS		 = WordSize;	// SPI word size / 0:8bit, 1:16bit, 2:24bit, 3:32bit
	SPI0_CS_OEN  = 0;			// SPI chip select output enable
	SPI0_CS      = 1;			// SPI chip select output High
	SPI0_BITMODE = BitDirection;// SPI bit direction / 0:MSB First, 1:LSB First
	SPI0_CLKDIV  = (BUS_CLK/(4*Speed_Hz)-1);
	SPI0_IRQ_EN  = 0;			// Interrupt Disabled
	// L(Low) H(High) Pec(Positive Edge clock) Nec(Negative Edge clock)
	SPI0_CLKMODE = 0;			// 0:L+Pec, 1:L+Nec, 2:H+Pec, 3:H+Nec
}

void Spi0_Deinit(void)
{
	SPI0_WS      = 0;
	SPI0_CS_OEN  = 1;
	SPI0_CS      = 1;
	SPI0_BITMODE = 0;
	SPI0_CLKDIV  = 0;
	SPI0_IRQ_EN  = 0;
	SPI0_CLKMODE = 0;
	SPI0_EN      = 0;
}

void Spi0_CsLo(void)
{
	SPI0_CS = 0;
}

void Spi0_CsHi(void)
{
	SPI0_CS = 1;
}

void Spi0_Write(BYTE * dat)
{
	SPI0_TX_DAT = (SPI0_WS==0)? ((UINT)(*dat)) :
				  (SPI0_WS==1)? (((UINT)(*dat)<<8)|((UINT)(*(dat+1)))) :
				  (SPI0_WS==2)? (((UINT)(*dat)<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*(dat+2)))) :
				  (SPI0_WS==3)? (((UINT)(*dat)<<24)|((UINT)(*(dat+1))<<16)|((UINT)(*(dat+2))<<8)|((UINT)(*(dat+3)))) : SPI0_TX_DAT;
	SPI0_RW = 1;
	SPI0_GO = 1;
	while(SPI0_GO);
}

void Spi0_Read(BYTE * dat)
{
	SPI0_RW = 2;
	SPI0_GO = 1;
	while(SPI0_GO);
	*dat++ = (SPI0_RX_DAT&0xff000000)>>24;
	if(SPI0_WS>0) *dat++ = (SPI0_RX_DAT&0x00ff0000)>>16;
	if(SPI0_WS>1) *dat++ = (SPI0_RX_DAT&0x0000ff00)>>8;
	if(SPI0_WS>2) *dat++ = (SPI0_RX_DAT&0x000000ff);
	return;
}

void Spi0_RW(BYTE * WrDat, BYTE * RdDat)
{
	SPI0_TX_DAT = (SPI0_WS==0)? ((UINT)(*WrDat)) :
				  (SPI0_WS==1)? (((UINT)(*WrDat)<<8)|((UINT)(*(WrDat+1)))) :
				  (SPI0_WS==2)? (((UINT)(*WrDat)<<16)|((UINT)(*(WrDat+1))<<8)|((UINT)(*(WrDat+2)))) :
				  (SPI0_WS==3)? (((UINT)(*WrDat)<<24)|((UINT)(*(WrDat+1))<<16)|((UINT)(*(WrDat+2))<<8)|((UINT)(*(WrDat+3)))) : SPI0_TX_DAT;
	SPI0_RW = 3;
	SPI0_GO = 1;
	while(SPI0_GO);
	*RdDat++ = (SPI0_RX_DAT&0xff000000)>>24;
	if(SPI0_WS>0) *RdDat++ = (SPI0_RX_DAT&0x00ff0000)>>16;
	if(SPI0_WS>1) *RdDat++ = (SPI0_RX_DAT&0x0000ff00)>>8;
	if(SPI0_WS>2) *RdDat++ = (SPI0_RX_DAT&0x000000ff);
	return;
}

void Spi1_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection)
{
	GpioFuncPin(GPIO_01);
	GpioFuncPin(GPIO_02);
	GpioFuncPin(GPIO_03);
	GpioFuncPin(GPIO_04);

	// CH1 - Master
	SPI1_EN 	 = 1;			// SPI Enabled
	SPI1_WS 	 = WordSize;	// 0:8bit, 1:16bit, 2:24bit, 3:32bit
	SPI1_CS_OEN  = 0;			// SPI Chip select output enable
	SPI1_CS 	 = 1;			// SPI Chip select output High
	SPI1_BITMODE = BitDirection;// 0:MSB First, 1:LSB First
	SPI1_CLKDIV  = (BUS_CLK/(4*Speed_Hz)-1);
	SPI1_IRQ_EN  = 0;			// Interrupt Disabled
	// L(Low) H(High) Pec(Positive Edge clock) Nec(Negative Edge clock)
	SPI1_CLKMODE = 0;			// 0:L+Pec, 1:L+Nec, 2:H+Pec, 3:H+Nec
}

void Spi1_Deinit(void)
{
	GpioFuncPinOff(GPIO_01);
	GpioFuncPinOff(GPIO_02);
	GpioFuncPinOff(GPIO_03);
	GpioFuncPinOff(GPIO_04);

	SPI1_WS      = 0;
	SPI1_CS_OEN  = 1;
	SPI1_CS      = 1;
	SPI1_BITMODE = 0;
	SPI1_CLKDIV  = 0;
	SPI1_IRQ_EN  = 0;
	SPI1_CLKMODE = 0;
	SPI1_EN      = 0;
}

void Spi1_CsLo(void)
{
	SPI1_CS = 0;
}

void Spi1_CsHi(void)
{
	SPI1_CS = 1;
}

void Spi1_Write(BYTE * dat)
{
	SPI1_TX_DAT = (SPI1_WS==0)? ((UINT)(*dat)) :
				  (SPI1_WS==1)? (((UINT)(*dat)<<8)|((UINT)(*(dat+1)))) :
				  (SPI1_WS==2)? (((UINT)(*dat)<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*(dat+2)))) :
				  (SPI1_WS==3)? (((UINT)(*dat)<<24)|((UINT)(*(dat+1))<<16)|((UINT)(*(dat+2))<<8)|((UINT)(*(dat+3)))) : SPI1_TX_DAT;
	SPI1_RW = 1;
	SPI1_GO = 1;
	while(SPI1_GO);
}

void Spi1_Read(BYTE * dat)
{
	SPI1_RW = 2;
	SPI1_GO = 1;
	while(SPI1_GO);
	*dat++ = (SPI1_RX_DAT&0xff000000)>>24;
	if(SPI1_WS>0) *dat++ = (SPI1_RX_DAT&0x00ff0000)>>16;
	if(SPI1_WS>1) *dat++ = (SPI1_RX_DAT&0x0000ff00)>>8;
	if(SPI1_WS>2) *dat++ = (SPI1_RX_DAT&0x000000ff);
	return;
}

void Spi1_RW(BYTE * WrDat, BYTE * RdDat)
{
	SPI1_TX_DAT = (SPI1_WS==0)? ((UINT)(*WrDat)) :
				  (SPI1_WS==1)? (((UINT)(*WrDat)<<8)|((UINT)(*(WrDat+1)))) :
				  (SPI1_WS==2)? (((UINT)(*WrDat)<<16)|((UINT)(*(WrDat+1))<<8)|((UINT)(*(WrDat+2)))) :
				  (SPI1_WS==3)? (((UINT)(*WrDat)<<24)|((UINT)(*(WrDat+1))<<16)|((UINT)(*(WrDat+2))<<8)|((UINT)(*(WrDat+3)))) : SPI1_TX_DAT;
	SPI1_RW = 3;
	SPI1_GO = 1;
	while(SPI1_GO);
	*RdDat++ = (SPI1_RX_DAT&0xff000000)>>24;
	if(SPI1_WS>0) *RdDat++ = (SPI1_RX_DAT&0x00ff0000)>>16;
	if(SPI1_WS>1) *RdDat++ = (SPI1_RX_DAT&0x0000ff00)>>8;
	if(SPI1_WS>2) *RdDat++ = (SPI1_RX_DAT&0x000000ff);
	return;
}

void Spi2_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection)
{
	GpioFuncPin(GPIO_20);
	GpioFuncPin(GPIO_21);
	GpioFuncPin(GPIO_22);
	GpioFuncPin(GPIO_23);

	// CH2 - Master
	SPI2_EN 	 = 1;			// SPI Enabled
	SPI2_WS 	 = WordSize;	// 0:8bit, 1:16bit, 2:24bit, 3:32bit
	SPI2_CS_OEN  = 0;			// SPI Chip select output enable
	SPI2_CS 	 = 1;			// SPI Chip select output High
	SPI2_BITMODE = BitDirection;// 0:MSB First, 1:LSB First
	SPI2_CLKDIV  = (BUS_CLK/(4*Speed_Hz)-1);
	SPI2_IRQ_EN  = 0;			// Interrupt Disabled
	// L(Low) H(High) Pec(Positive Edge clock) Nec(Negative Edge clock)
	SPI2_CLKMODE = 0;			// 0:L+Pec, 1:L+Nec, 2:H+Pec, 3:H+Nec

}

void Spi2_Deinit(void)
{
	GpioFuncPinOff(GPIO_20);
	GpioFuncPinOff(GPIO_21);
	GpioFuncPinOff(GPIO_22);
	GpioFuncPinOff(GPIO_23);

	SPI2_WS      = 0;
	SPI2_CS_OEN  = 1;
	SPI2_CS      = 1;
	SPI2_BITMODE = 0;
	SPI2_CLKDIV  = 0;
	SPI2_IRQ_EN  = 0;
	SPI2_CLKMODE = 0;
	SPI2_EN      = 0;
}

void Spi2_CsLo(void)
{
	SPI2_CS = 0;
}

void Spi2_CsHi(void)
{
	SPI2_CS = 1;
}

void Spi2_Write(BYTE * dat)
{
	SPI2_TX_DAT = (SPI2_WS==0)? ((UINT)(*dat)) :
				  (SPI2_WS==1)? (((UINT)(*dat)<<8)|((UINT)(*(dat+1)))) :
				  (SPI2_WS==2)? (((UINT)(*dat)<<16)|((UINT)(*(dat+1))<<8)|((UINT)(*(dat+2)))) :
				  (SPI2_WS==3)? (((UINT)(*dat)<<24)|((UINT)(*(dat+1))<<16)|((UINT)(*(dat+2))<<8)|((UINT)(*(dat+3)))) : SPI2_TX_DAT;
	SPI2_RW = 1;
	SPI2_GO = 1;
	while(SPI2_GO);
}

void Spi2_Read(BYTE * dat)
{
	SPI2_RW = 2;
	SPI2_GO = 1;
	while(SPI2_GO);
	*dat++ = (SPI2_RX_DAT&0xff000000)>>24;
	if(SPI2_WS>0) *dat++ = (SPI2_RX_DAT&0x00ff0000)>>16;
	if(SPI2_WS>1) *dat++ = (SPI2_RX_DAT&0x0000ff00)>>8;
	if(SPI2_WS>2) *dat++ = (SPI2_RX_DAT&0x000000ff);
	return;
}

void Spi2_RW(BYTE * WrDat, BYTE * RdDat)
{
	SPI2_TX_DAT = (SPI2_WS==0)? ((UINT)(*WrDat)) :
				  (SPI2_WS==1)? (((UINT)(*WrDat)<<8)|((UINT)(*(WrDat+1)))) :
				  (SPI2_WS==2)? (((UINT)(*WrDat)<<16)|((UINT)(*(WrDat+1))<<8)|((UINT)(*(WrDat+2)))) :
				  (SPI2_WS==3)? (((UINT)(*WrDat)<<24)|((UINT)(*(WrDat+1))<<16)|((UINT)(*(WrDat+2))<<8)|((UINT)(*(WrDat+3)))) : SPI2_TX_DAT;
	SPI2_RW = 3;
	SPI2_GO = 1;
	while(SPI2_GO);
	*RdDat++ = (SPI2_RX_DAT&0xff000000)>>24;
	if(SPI2_WS>0) *RdDat++ = (SPI2_RX_DAT&0x00ff0000)>>16;
	if(SPI2_WS>1) *RdDat++ = (SPI2_RX_DAT&0x0000ff00)>>8;
	if(SPI2_WS>2) *RdDat++ = (SPI2_RX_DAT&0x000000ff);
	return;
}

/* EOF */
