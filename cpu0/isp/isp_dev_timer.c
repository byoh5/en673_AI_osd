/* **************************************************************************
 File Name	: 	dev_timer.c
 Description:	EN673 - Timer device driver
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 08. 13
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"


volatile UINT gnIsrTimer3 = 0;
UINT gnTm0Test_Cnt, gnTm1Test_Cnt, gnTm2Test_Cnt, gnTm3Test_Cnt, gnTm4Test_Cnt, gnTm5Test_Cnt;
void ISPM0 Tm0_Func(void)
{
	gnTm0Test_Cnt++;
}


extern UINT framecnt;
UINT BT1120_FPS,BT1120_FLAG = 0;
void ISPM0 Tm1_Func(void)
{
	gnTm1Test_Cnt++;

	if(gnTm1Test_Cnt > 999){
		gnTm1Test_Cnt = 0;
	if(framecnt ==30 || framecnt ==50 || framecnt ==60 || framecnt ==25)
		BT1120_FPS = framecnt;
//	printf("%u/%u\r\n",framecnt,BT1120_FPS);

	framecnt = 0;
	}
	
	
#if (model_Lens==1)	// 20171107 - WHL
	if(gbMaf_TimerEn) MAF_PhaseControl();
	
#endif
}

void ISPM0 Tm2_Func(void)
{
	gnTm2Test_Cnt++;
}

void ISPM0 Tm3_Func(void)	// 15.4.20 WHL - for 2Motor Application
{
	gnTm3Test_Cnt++;
	gnIsrTimer3++;
}

void ISPM0 Tm4_Func(void)
{
	gnTm4Test_Cnt++;
}

void ISPM0 Tm5_Func(void)	// 15.4.20 WHL - for 2Motor Application
{
	gnTm5Test_Cnt++;
}

void ISPM0 IsrTimer(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
#if 1	// Org - Good effectiveness at in case of using few timer IRQ.
	UINT i;
	const BYTE bTIMER_IRQ = _rd8(TIMER_REG_BASE+3) & 0x3F;	// Default Byte Ordering is MSB(Most Significant Byte)

	for(i=0; i<TIMER_NUM; i++)
	{
		const BYTE bTimerBit = (0x1<<(TIMER_NUM-1-i));

		if( bTIMER_IRQ&bTimerBit ) {
			_rd8(TIMER_REG_BASE+2) |= bTimerBit;			// Default Byte Ordering is MSB(Most Significant Byte)

			switch (i) {	// 15.5.1 WHL
				case 0 : Tm0_Func(); break;	// Timer0 Interrupt Function Call
				case 1 : Tm1_Func(); break;	// Timer1 			"
				case 2 : Tm2_Func(); break;	// Timer2	 		"
				case 3 : Tm3_Func(); break;	// Timer3 			"
				case 4 : Tm4_Func(); break;	// Timer4 			"
				case 5 : Tm5_Func(); break;	// Timer5 			"
			}
		}
	}

#else	// Good effectiveness at in case of using many timer IRQ.
	if(TIM_CH0_IRQ) { TIM_CH0_IRQ_CLR=1; Tm0_Func(); }
	if(TIM_CH1_IRQ) { TIM_CH1_IRQ_CLR=1; Tm1_Func(); }
	if(TIM_CH2_IRQ) { TIM_CH2_IRQ_CLR=1; Tm2_Func(); }
	if(TIM_CH3_IRQ) { TIM_CH3_IRQ_CLR=1; Tm3_Func(); }
	if(TIM_CH4_IRQ) { TIM_CH4_IRQ_CLR=1; Tm4_Func(); }
	if(TIM_CH5_IRQ) { TIM_CH5_IRQ_CLR=1; Tm5_Func(); }

#endif
}


/* EOF */

