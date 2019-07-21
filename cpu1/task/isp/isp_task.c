#include "dev.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_inline.h"															// for inline

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include <stdio.h>
#include "user.h"

#include "isp_task.h"

UINT gnISP_CtrState = 0;

UINT ISP_frame_value(void)
{
	switch (MP(FrameMode)) {
		case MN_FRAMEMODE_NORMAL_25_30:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 25 : 30;
		case MN_FRAMEMODE_WDR_12_15:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 12 : 15;
		case MN_FRAMEMODE_DNR_25_30:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 25 : 30;
#if !(model_2M30p)
		case MN_FRAMEMODE_NORMAL_50_60:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 50 : 60;
		case MN_FRAMEMODE_WDR_25_30:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 25 : 30;
		case MN_FRAMEMODE_DNR_50_60:
			return MP(MpSysFreq) == MN_SYSFREQ_50 ? 50 : 60;
#endif
		default:
			return 0;
	}
}

void ispTask(void* pvParameters)
{
	for(;;)
	{
		if(gnISP_CtrState) {

			portENTER_CRITICAL();

			while(CS_ISPCMD);			//	Critical Section Begin - SHARED MEMORY0 Busy

//			CPU_SHARED2 = (0x50000000|gnISP_CtrState);

			CS_ISPCMD = 0;				//	Critical Section End - SHARED MEMORY0 Fred

			portEXIT_CRITICAL();
 
 //			printf("\r\ngnISP_CtrState : 0x%08x\r\n", gnISP_CtrState);

			gnISP_CtrState = 0;
		}
		vTaskDelay(1); /* delay 1 == 10 ms*/
	}
	UNUSED(pvParameters);
}

/* EOF */

