/***************************************************************************
 File Name	: 	cpu.c
 Description:	EN673 - cpu device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 12. 17
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include <string.h>
#include "dev.h"
#include "exr1k_inline.h"

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#endif

_NOINS BYTE CpuIdGet(void)
{
	return CPU_ID;
}

BOOL GetKey(UINT Idx)
{
	UINT * pKey = (UINT *)(CPU_REG_BASE+0x40+Idx);
	return *pKey;
}

void ReleaseKey(UINT Idx)
{                          
	UINT * pKey = (UINT *)(CPU_REG_BASE+0x40+Idx);
	*pKey = 0;
}

static inline void hw_wait_us(UINT us)
{
	UINT clk_period = EXR1K_CLK / 1000000 * us;		// [clks/sec] * [sec/us] * [us] = [clks]
	UINT start_clk = mfspr(EXR1K_HTBTR);
	UINT clk;
	do {
		UINT stop_clk = mfspr(EXR1K_HTBTR);
		clk = stop_clk - start_clk;
		if(stop_clk < start_clk)
			clk = ~clk + 1;	// neg --> abs
	} while(clk < clk_period);
}

void WaitXus(UINT anUs)
{	// Wait X usec
	hw_wait_us(anUs);
}

void WaitXms(UINT anMs)
{	// Wait X msec
	hw_wait_us(anMs * 1000);
}

#ifndef _INLINE
#ifndef DEF_BOOT
_NOINS void WdtCpuSetStop(void)
{
#ifdef DEF_CPU1
	CPU_WDT1_T = 0x0;
#else
	CPU_WDT0_T = 0x0;
#endif
}

_NOINS void WdtCpuCountReset(void)
{
#ifdef DEF_CPU1
	CPU_WDT1_T = 0x3;
	CPU_WDT1_T = 0x2;
#else
	CPU_WDT0_T = 0x3;
	CPU_WDT0_T = 0x2;
#endif
}

_NOINS void WdtCpuSetTime(UINT sec)
{
#ifdef DEF_CPU1
	CPU_WDT1_LMT = BUS_CLK * sec;
#else
	CPU_WDT0_LMT = BUS_CLK * sec;
#endif
	WdtCpuCountReset();
}

_NOINS void WdtCpuReboot(void)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif
	WdtCpuSetTime(0);
	while(1);
}
#endif
#endif

/* EOF */
