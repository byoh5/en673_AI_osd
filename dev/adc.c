/***************************************************************************
 File Name	: 	adc.c
 Description:	EN673 - adc device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/

#include "dev.h"

void AdcInit(void)
{
	
	ADC_CLK_LMT = 0x80;	//	Clock Freq = MCK / ((ADC_CLK_LMT+1)*2)	Up to 16MHz
	ADC_CKEN = 1;		//	Sampling Rate = MCK / ((ADC_CLK_LMT+1)*32)	// Up to 1MSPS
	
	ADC_IRQ_EN = 0;
	
	ADC_CH0_EN 	= 1;
	ADC_CH0_SIGN = 0;	//	0:	Orignal 1: Signed
	ADC_CH0_MUL	= 0;	//	0:	Orignal 1: Orignal*2	2: Orignal*4	3: Orignal*8	4:	Orignal*16
	
	ADC_CH1_EN 	= 1;
	ADC_CH1_SIGN = 0;
	ADC_CH1_MUL	= 0;
	
	ADC_CH2_EN 	= 1;
	ADC_CH2_SIGN = 0;
	ADC_CH2_MUL	= 0;

	ADC_BUF_EN = 1;
	ADC_BUF_CH = 0;
	ADC_BUF_SIGN = 1;
	ADC_BUF_MUL = 1;
	ADC_BUF_LMT = 0x400;

	ADC_EN = 1;
}

void AdcIsr(void)
{
	ADC_IRQ_CLR = 1;
//	DmaMemCpy(RAM1_BASE, ADC_BASE, 0x400);
	G711Enc((BYTE *)SH1RAM_BASE, (BYTE *)ADC_BASE, 0x400, G711_aLaw); // A-Law
//	G711Dec(RAM1_BASE+0x1000, RAM1_BASE, 0x100, 0); // A-Law
}

/* EOF */
