/***************************************************************************
 File Name	: 	sys.c
 Description:	EN673 - SYSTEM Control driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 11. 28
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"
#include "dev_inline.h"

#include "msg.h"
#include "user.h"

#ifdef DEF_CPU0
#if (IMAGE_INPUT_MODE==2)
void IsrVlock(void *dummy);
#endif

#ifdef ECM_REMOTE
void Isr_JUART(void *dummy);
#endif

void IsrSys0(void *dummy)
{
	CPU_IRQ0_CLR = 1;
#ifdef ECM_REMOTE
	if(CPU_SHARED2 & eIP0_J_UART)
	{
		CPU_SHARED2 &= ~eIP0_J_UART;
		Isr_JUART(dummy);
	}
#endif
#if (IMAGE_INPUT_MODE==2)
	if(CPU_SHARED0 & eIP0_DGT_INPUT)
	{
		CPU_SHARED0 &= ~eIP0_DGT_INPUT;
		IsrVlock(dummy);
	}
#else
	UNUSED(dummy);
#endif
}
#endif

#ifdef DEF_CPU1
#include "FreeRTOS.h"

#ifdef __DEVICE_SD__
extern void IsrH264Enc(void *arg);
#endif
#ifdef __IRQ_UART0__
extern void IsrShell(void *arg);
#endif
extern void IsrVideo(void *ctx);
#ifdef JTAG_DEBUG_PRINT
extern void Isr_JShell(void *arg);
#endif

void IsrSys1(void *dummy)
{
	CPU_IRQ1_CLR = 1;
#ifdef __ISP__
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_SHD)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_SHD;
		UserSave(sfls_ISP_SHD);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_MENU)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_MENU;
		UserSave(sfls_ISP_MENU);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_MENU2)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_MENU2;
		UserSave(sfls_ISP_MENU_BAK);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_USER)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_USER;
		UserSave(sfls_ISP_USER);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_DATA)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_DATA;
		UserSave(sfls_ISP_DATA);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_BLK0)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_BLK0;
		UserSave(sfls_ISP_BLK0);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_BLK1)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_BLK1;
		UserSave(sfls_ISP_BLK1);
	}
	if(gptMsgISPTbl.ISPTblState & eIP1_ISP_SAVE_BLK2)
	{
		gptMsgISPTbl.ISPTblState &= ~eIP1_ISP_SAVE_BLK2;
		UserSave(sfls_ISP_BLK2);
	}
#endif
#ifdef __ETH__
	if(CPU_SHARED1 & eIP1_ETH_RX)
	{
		CPU_SHARED1 &= ~eIP1_ETH_RX;
		IsrEthRx(dummy);
	}
#endif
#ifdef __WIFI__
	if(CPU_SHARED1 & eIP1_WIF_RX)
	{
		CPU_SHARED1 &= ~eIP1_WIF_RX;
		IsrWifiRx(dummy);
	}
	if(CPU_SHARED1 & eIP1_WIF_EVT)
	{
		CPU_SHARED1 &= ~eIP1_WIF_EVT;
		IsrWifiEvt(dummy);
	}
#endif
#ifdef __H264__
	if(CPU_SHARED1 & eIP1_RECORD_INFO)
	{
		CPU_SHARED1 &= ~eIP1_RECORD_INFO;
#ifdef __DEVICE_SD__
		IsrH264Enc(dummy);
#endif
	}
#endif
	if(CPU_SHARED1 & eIP1_SHELL_RX)
	{
		CPU_SHARED1 &= ~eIP1_SHELL_RX;
#ifdef __IRQ_UART0__
		IsrShell(dummy);
#endif
	}
	if(CPU_SHARED1 & eIP1_RESET_FPS)
	{
		CPU_SHARED1 &= ~eIP1_RESET_FPS;
		IsrVideo(dummy);
	}
#ifdef JTAG_DEBUG_PRINT
	if(CPU_SHARED2 & eIP1_J_SHELL_RX)
	{
		CPU_SHARED2 &= ~eIP1_J_SHELL_RX;
		Isr_JShell(dummy);
	}
#endif
	UNUSED(dummy);
}
#endif

#ifndef _INLINE
_NOINS void WdtSysSetStop(void)
{
	SYS_WDT_T = 0;		// WDT Disable
}

_NOINS void WdtSysCountReset(void)
{
#ifdef DEF_BOOT
	gptMsgShare.CPU1_WDT_COUNT = 0;	// Count Reset
	SYS_WDT_T = 0x3;	// WDT Enable + Count Reset
#endif
#ifdef DEF_CPU0
	SYS_WDT_T = 0x3;	// WDT Enable + Count Reset
#endif
#ifdef DEF_CPU1
	gptMsgShare.CPU1_WDT_COUNT = 0; // Count Reset
#endif
}

_NOINS void WdtSysSetTime(UINT sec)
{
	SYS_WDT_LMT = sec * BUS_CLK;
	WdtSysCountReset();
}

_NOINS void WdtSysReboot(void)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();
#else
	CRITICAL_BEGIN;
#endif
	SYS_WDT_LMT = 0;
	SYS_WDT_T = 0x3;
	while(1);
}
#endif
