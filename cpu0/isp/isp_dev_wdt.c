/* **************************************************************************
 File Name	: 	dev_wdt.c
 Description:	EN673 - Watchdog timer device driver
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 08. 13
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"

void InitWdt(UINT sec)
{
	SYS_WDT_LMT/*WDT_CNT_LMT*/ = MCLK * sec;
	
}

void SystemReset(void)
{
	SYS_WDT_LMT = 500 * (MCLK/1000);	// 500ms
	SYS_WDT_T = 0x2;
	while(1);
}

