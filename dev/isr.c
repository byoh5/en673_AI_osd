/* **************************************************************************
 File Name	: 	isr.c
 Description:	OR - Interrupt device driver
 Designer	:	Jang, YoungKyu
 Date		:	14. 12. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev.h"
#include "dev_inline.h"	// to use fast code

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

ISRD volatile BYTE gbXsrTaskSwitchNeeded = 0;		// Task switch is required
#endif

struct tIhnd tIntHandlers[MAX_INT_HANDLERS];		// Interrupt handlers table

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Initialize routine
int IntInit(void)
{
	int i;
	for(i=0; i<MAX_INT_HANDLERS; i++)
	{
		tIntHandlers[i].handler = 0;
		tIntHandlers[i].arg = 0;
	}
	return 0;
}

//-------------------------------------------------------------------------------------------------
// Add interrupt handler
int IntAdd(UINT anVec, void (* handler)(void *), void *arg)
{
	if(anVec >= MAX_INT_HANDLERS)	return -1;

//	printf("%s(%d) : CPU%d INT(%d)\r\n", __func__ ,__LINE__, CpuIdGet(), anVec);
	tIntHandlers[anVec].handler = handler;
	tIntHandlers[anVec].arg = arg;

	return 0;
}

//-------------------------------------------------------------------------------------------------
// Disable or Enable interrupt
int IrqDisable(UINT anVec)
{
	if(anVec >= MAX_INT_HANDLERS)	return -1;

	mtspr(EXR1K_PICMR, mfspr(EXR1K_PICMR) & ~(0x00000001L << anVec));

	return 0;
}

int IrqEnable(UINT anVec)
{
	if(anVec >= MAX_INT_HANDLERS)	return -1;

	mtspr(EXR1K_PICMR, mfspr(EXR1K_PICMR) | (0x00000001L << anVec));

	return 0;
}


//*************************************************************************************************
// Xsr
//-------------------------------------------------------------------------------------------------
//
//volatile UINT gnSR_Buffer;
ISRT void XsrInt(void)
{
	UINT picsr;
	UINT i;

#ifdef DEF_CPU1
	gbXsrTaskSwitchNeeded = 0;
#endif

	while((picsr = mfspr(EXR1K_PICSR))) {
		i = 0;
		while(i < MAX_INT_HANDLERS){
			if((picsr & (0x00000001L << i)) && (tIntHandlers[i].handler != 0)) {
				(*tIntHandlers[i].handler)(tIntHandlers[i].arg);
				mtspr(EXR1K_PICSR, mfspr(EXR1K_PICSR) & ~(0x00000001L << i));
			}
			i++;
		}
	}

//	gnSR_Buffer = mfspr(EXR1K_ESR)&0x4;

#ifdef DEF_CPU1
	if(gbXsrTaskSwitchNeeded)
		vTaskSwitchContext();				// Task switch required ?
#endif
}

//#endif
