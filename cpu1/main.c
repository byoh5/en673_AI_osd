//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "msg.h"
#include "user.h"

#include "init.h"
#include "FreeRTOS.h"
#include "task.h"																// for vTaskStartScheduler
#ifdef __ONVIF__
#include "stdsoap2.h"
#endif

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
int main(void)
{
	//--------------------------------------------------------------------------
	// Setup Debugger: DO NOT MOVE because the STB starts @ main
#ifdef 	_DEBUG
	//enable_jtag_ctrl_exception();
	enable_hwp(EXR1K_SPRGID_IWB);
	enable_hwp(EXR1K_SPRGID_DWB);
	enable_hwdt(EXR1K_SPRGID_IWB, 10000);
	enable_hwdt(EXR1K_SPRGID_DWB, 10000);
	print_hwp_all_info(EXR1K_SPRGID_IWB);
	print_hwp_all_info(EXR1K_SPRGID_DWB);

	enable_htb();
	enable_stb();
#else
	mtspr(EXR1K_HTBCR, EXR1K_HTBCR_TE);
#endif//_DEBUG

	set_fp_round_mode(EXR1K_FPCSR_RM_RN);	// float32 사용시 사용

	memset((void *)&gtUser, 0, sizeof(gtUser));

	printf("Firmware Ver %X.%X.%X\r\n", (EN673_VERSION>>8)&0xf, (EN673_VERSION>>4)&0xf, EN673_VERSION&0xf);

	InitMsgOS();
	InitHw();
	InitUsr();
	InitSw();

#ifdef __ONVIF__
	float n=3.2;
	if (soap_isninff(n))
		return 0;
#endif

	vTaskStartScheduler();

	return	0;
}
