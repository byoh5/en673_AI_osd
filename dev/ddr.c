/***************************************************************************
 File Name	: 	ddr.c
 Description:	EN673 - DDR device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 25
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/

#include "dev.h"

#define DDR_TEST_MODE 0

#if DDR_TEST_MODE
ISRT static BOOL MemCmp(BYTE *Src , BYTE* Dst , UINT Len)
{
	UINT i;
	
	for(i=0;i<Len;i++)
	{
		if(*Dst != *Src) return FALSE;
		Dst++;
		Src++;
	}
	return TRUE;
}

static BOOL DdrTest(UINT va)
{
#define DATASIZE 	0x500
#define LOOPOFOFSET 0x10

	UINT i;
	volatile BYTE * pRam = (BYTE *)(SYSRAM_BASE+0x1000);

	for(i=0;i<DATASIZE;i++)
	{
		*pRam++ = (i&0xff)*va;
	}

	for(i=0;i<LOOPOFOFSET;i++)
	{
		DmaMemCpy_ip((BYTE *)(DDR_BASE+i), (BYTE *)(SYSRAM_BASE+0x1000), DATASIZE);
		DmaMemCpy_ip((BYTE *)(SYSRAM_BASE+0x2000), (BYTE *)(DDR_BASE+i), DATASIZE);
		if(MemCmp((BYTE *)(SYSRAM_BASE+0x2000), (BYTE *)(SYSRAM_BASE+0x1000), DATASIZE)==FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}

static void DdrParamOut(UINT vout)
{
	DDR_DLYCTL0_T =                               (DDR_DLY_DQSIN<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL1_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL2_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL3_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL4_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
}

void DdrCal(void)
{
	const char *strindex = " 0 1 2 3 4 5 6 7 8 9 A B C D E F\n        ";
	UINT i;
	Uart0_Str("\nDDR Delay Test Start! LTC(");
	Uart0_Dec(DDR_RD_LTC);
	Uart0_Str(")EDGE(");
	Uart0_Dec(DDR_RD_EDGE);
	Uart0_Str(")\nDQS  IN");
	Uart0_Str(strindex);
	for(i=0;i<0x10;i++)
	{
		DDR_DLY_DQSIN = 0x1111 * i;
		if(DdrTest(i))
		{
			Uart0_Str("O ");
		}
		else
		{
			Uart0_Str("X ");
		}
	}
	Uart0_Str("\nDQS OUT");
	Uart0_Str(strindex);
	DDR_DLY_DQSIN = 0x7777;	// Test DQS IN
	for(i=0;i<0x10;i++)
	{
		DdrParamOut(i);
		if(DdrTest(i))
		{
			Uart0_Str("O ");
		}
		else
		{
			Uart0_Str("X ");
		}
	}
	Uart0_Str("\nTest End...\n");
}
#endif

static void DdrParamDly(UINT ltc, UINT edge, UINT vin, UINT vout)
{
	DDR_DLYCTL0_T =  (vin<<28) |  (vin<<24) |  (vin<<20) |  (vin<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL1_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL2_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL3_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);
	DDR_DLYCTL4_T = (vout<<28) | (vout<<24) | (vout<<20) | (vout<<16) | (vout<<12) | (vout<<8) | (vout<<4) | (vout);

	DDR_TIM0_T = (0x3<<24) | (0xA<<20) | (0x7<<16) | (0x2<<12) | (0x7<<8) | (0x1<<4) | (0x2);
	DDR_TIM1_T = (0x12<<12) | (0x1<<6) | (0x8);
	DDR_CTL1_T = (0x3<<24) | (ltc<<8) | (edge<<7);
}

static void DdrParamBypass(void)
{
#define SET_DDR_MR(REQ, CTL, RAS, CAS, WE, BA, A)	\
	DDR_CTL0_T = (REQ<<31) | (CTL<<20) | (RAS<<19) | (CAS<<18) | (WE<<17) | (BA<<13) | (A); \
	while(DDR_MR_REQ);	//	CKE Enable

#if (CLK_VALUE==100)
	DdrParamDly(2, 0, 0x8, 0x8);			// LTC(2), EDGE(0), IN(0x8), OUT(0x8)	270.0Mhz
#elif (CLK_VALUE==95)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	256.5Mhz
#elif (CLK_VALUE==90)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	243.0Mhz
#elif (CLK_VALUE==85)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	229.5Mhz
#elif (CLK_VALUE==80)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	216.0Mhz
#elif (CLK_VALUE==74)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	199.8Mhz
#elif (CLK_VALUE==70)
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	189.0Mhz
#elif (CLK_VALUE==65)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	175.5Mhz
#elif (CLK_VALUE==60)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	162.0Mhz
#elif (CLK_VALUE==55)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	148.5Mhz
#elif (CLK_VALUE==50)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	135.0Mhz
#elif (CLK_VALUE==45)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	121.5Mhz
#elif (CLK_VALUE==40)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	108.0Mhz
#elif (CLK_VALUE==35)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	 94.5Mhz
#elif (CLK_VALUE==30)
	DdrParamDly(1, 0, 0x8, 0x8);			// LTC(1), EDGE(0), IN(0x8), OUT(0x8)	 81.0Mhz
#elif (CLK_VALUE==25)
	DdrParamDly(1, 1, 0x8, 0x8);			// LTC(1), EDGE(1), IN(0x8), OUT(0x8)	 67.5Mhz
#elif (CLK_VALUE==20)
	DdrParamDly(0, 0, 0x8, 0x8);			// LTC(0), EDGE(0), IN(0x8), OUT(0x8)	 54.0Mhz
#elif (CLK_VALUE==15)
	DdrParamDly(0, 0, 0x8, 0x8);			// LTC(0), EDGE(0), IN(0x8), OUT(0x8)	 40.5Mhz
#elif (CLK_VALUE==10)
	DdrParamDly(0, 0, 0x8, 0x8);			// LTC(0), EDGE(0), IN(0x8), OUT(0x8)	 27.0Mhz
#else
	DdrParamDly(2, 1, 0x8, 0x8);			// LTC(2), EDGE(1), IN(0x8), OUT(0x8)	etc
#endif

	WaitXus(200);							//	Wait 200us
	SET_DDR_MR(1, 3, 1, 1, 1, 0, 0x000);	//	NOP
	WaitXus(200);							//	Wait 200us
	SET_DDR_MR(1, 2, 0, 1, 0, 0, 0x400);	//	PRE-ALL
	WaitXus(10);							//	Wait 10us
	SET_DDR_MR(1, 2, 0, 0, 1, 0, 0x000);	//	AUTO-REF
	SET_DDR_MR(1, 2, 0, 0, 1, 0, 0x000);	//	AUTO-REF
	WaitXus(1);								//	Wait 1us
	SET_DDR_MR(1, 2, 0, 0, 0, 0, 0x032);	//	MRS
	SET_DDR_MR(1, 2, 0, 0, 0, 2, 0x000);	//	EMRS

	DDR_REF_LMT = (UINT)(7680.0 / (1000000000.0 / BUS_CLK));	// compile phase¿¡¼­ ¿¬»êµÊ(def:0x600)
	DDR_REF_EN  = 1;
}

void DdrInit(void)
{
	SYS_DDR_RST = 1;
	SYS_DCK_PD	= 1;
//	SYS_DCK_SEL = 1;
	DdrParamBypass();
#if DDR_TEST_MODE
	int i, j;
	for(i=0;i<3;i++)
	{
		DDR_RD_LTC = i;
		for(j=0;j<2;j++)
		{
			DDR_RD_EDGE = j;
			DdrCal();
		}
	}
	while(1);
#endif
}
