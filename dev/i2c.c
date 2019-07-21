/***************************************************************************
 File Name	: 	i2c.c
 Description:	EN673 - i2c device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"
#include "dev_inline.h"

void I2c0_Init(UINT Speed_Hz)
{
	I2C0_CLK_DIV = (BUS_CLK/(8*Speed_Hz)-1);	// Clock divider for I2C controller
	I2C0_MODE = 0;		// Controller mode / 0:Master, 1:Slave
	I2C0_BIT_MODE = 0;	// Data bit direction / 0:MSB first, 1:LSB first
	I2C0_IRQ_EN = 0;	// Interrupt enable / 0:Disabled, 1:Enabled
}

BYTE I2c0_Write(BYTE dat, BYTE last, BYTE repeat)
{
	I2C0_TX_DAT = dat;
	I2C0_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while(I2C0_MST_GO);
	return I2C0_MST_ACK;
}

BYTE I2c0_Read(BYTE last, BYTE repeat)
{
	I2C0_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while(I2C0_MST_GO);
	return I2C0_RX_DAT;
}

UINT I2c0_Check(BYTE addr)
{
	UINT i, flag = DEF_FAIL;
	for(i=0; i<10; i++)
	{
		if(I2c0_Write(addr, 1, 0) == 0)
		{
			flag = DEF_OK;
			break;
		}
		WaitXus(10);
	}
	return flag;
}

void I2c1_Init(UINT Speed_Hz)
{
	GpioFuncPin(GPIO_05);
	GpioFuncPin(GPIO_06);

	I2C1_CLK_DIV = (BUS_CLK/(8*Speed_Hz)-1);	// Clock divider for I2C controller
	I2C1_MODE = 0;		// Controller mode / 0:Master, 1:Slave
	I2C1_BIT_MODE = 0;	// Data bit direction / 0:MSB first, 1:LSB first
	I2C1_IRQ_EN = 0;	// Interrupt enable / 0:Disabled, 1:Enabled
}

BYTE I2c1_Write(BYTE dat, BYTE last, BYTE repeat)
{
	I2C1_TX_DAT = dat;
	I2C1_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while(I2C1_MST_GO);
	return I2C1_MST_ACK;
}

BYTE I2c1_Read(BYTE last, BYTE repeat)
{
	I2C1_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while(I2C1_MST_GO);
	return I2C1_RX_DAT;
}

UINT I2c1_Check(BYTE addr)
{
	UINT i, flag = DEF_FAIL;
	for(i=0; i<10; i++)
	{
		if(I2c1_Write(addr, 1, 0) == 0)
		{
			flag = DEF_OK;
			break;
		}
		WaitXus(10);
	}
	return flag;
}

void I2c1_Deinit(void)
{
	I2C1_CLK_DIV = (BUS_CLK/(8*100000)-1);
	I2C1_MODE = 0;
	I2C1_BIT_MODE = 0;
	I2C1_IRQ_EN = 0;

	GpioFuncPinOff(GPIO_05);
	GpioFuncPinOff(GPIO_06);
}

void I2c2_Init(UINT Speed_Hz)
{
	GpioFuncPin(GPIO_18);
	GpioFuncPin(GPIO_19);

	I2C2_CLK_DIV = (BUS_CLK/(8*Speed_Hz)-1);	// Clock divider for I2C controller
	I2C2_MODE = 0;		// Controller mode / 0:Master, 1:Slave
	I2C2_BIT_MODE = 0;	// Data bit direction / 0:MSB first, 1:LSB first
	I2C2_IRQ_EN = 0;	// Interrupt enable / 0:Disabled, 1:Enabled
}

BYTE I2c2_Write(BYTE dat, BYTE last, BYTE repeat)
{
	I2C2_TX_DAT = dat;
	I2C2_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 3);
	while(I2C2_MST_GO);
	return I2C2_MST_ACK;
}

BYTE I2c2_Read(BYTE last, BYTE repeat)
{
	I2C2_CONT_MST = ((repeat<<3) | ((repeat | last)<<2) | 1);
	while(I2C2_MST_GO);
	return I2C2_RX_DAT;
}

UINT I2c2_Check(BYTE addr)
{
	UINT i, flag = DEF_FAIL;
	for(i=0; i<10; i++)
	{
		if(I2c2_Write(addr, 1, 0) == 0)
		{
			flag = DEF_OK;
			break;
		}
		WaitXus(10);
	}
	return flag;
}

void I2c2_Deinit(void)
{
	I2C2_CLK_DIV = (BUS_CLK/(8*100000)-1);
	I2C2_MODE = 0;
	I2C2_BIT_MODE = 0;
	I2C2_IRQ_EN = 0;

	GpioFuncPinOff(GPIO_18);
	GpioFuncPinOff(GPIO_19);
}

/* EOF */
