/***************************************************************************
 File Name	: 	i2s.c
 Description:	EN673 - i2s device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"

void I2s_Init(void)
{

//	I2S_CLKDIV = 0x19;	//48k
	I2S_CLKDIV = 0x19;
//	I2S_CLKDIV = 0x32;	//24k
	I2S_MODE = 1;
	I2S_TX_EN = 1;
	I2S_RX_EN = 1;
	I2S_RX_IRQ_EN = 1;
//#if 0
//	int i;
//	UINT * pDat = RAM1_BASE;
//	for(i=0;i<1024;i+=2)
//	{
//		pDat[i] = 0x00008000;
//		pDat[i+1] = 0xffff8000;
//	}
//	DmaMemCpy(I2S_BASE, RAM1_BASE, 4096);
//#else
//	UINT * pAdr = RAM1_BASE + 0x100;
//	*pAdr = 0x01280000;
//	DmaMemCpy(I2S_BASE, *pAdr, 4096);
//	pAdr+=4096;
//#endif

//	I2S_TX_IRQ_EN = 1;
}
void I2s_Isr(void)
{
	if(I2S_RX_IRQ)
	{
		I2S_RX_IRQ_CLR = 1;
//		DmaMemCpy((BYTE*)I2S_BASE, (BYTE*)I2S_BASE, 2048);	// 15.04.28 WHL - Need to check
	}

//	I2S_TX_IRQ_CLR = 1;
//#if 0
//	DmaMemCpy(I2S_BASE, RAM1_BASE, 2048);
//#else
//	UINT * pAdr = RAM1_BASE + 0x100;
//	DmaMemCpy(I2S_BASE, *pAdr, 2048);
//	uart_hex(*pAdr);
//	uart_tx(0x0d);
//	uart_tx(0x0a);
//	if(*pAdr==0x01C00000) *pAdr = 0x01010000;
//	else *pAdr += 2048;
//#endif
}

/* EOF */
