/***************************************************************************
 File Name	: 	timer.c
 Description:	EN673 - Timer Device Driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev.h"

#if 0
void TimerInit(void)
{
	TIM_CH0_CKEN	= 1;
	TIM_CH0_IRQ_EN	= 0;
	TIM_CH0_PWM_EN	= 1;
	TIM_CH0_DIV		= 1;	//	FREQ = MCK /(DIV+1)*2)/(LMT+1) MCK = 81MHz
	TIM_CH0_LMT		= 3;	//	0 1 2 3 0 1 2 3 
	TIM_CH0_TRIG	= 1;	//	- - _ _ - - _ _	PWM 50% @ 5.1MHz 
	TIM_CH0_EN		= 1;             
}
#endif