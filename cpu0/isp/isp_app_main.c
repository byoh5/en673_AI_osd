/* **************************************************************************
 File Name	: 	app_main.c
 Description:	EN673 - Main function
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"	// to use fast code


//	#define EN673_DEV_MODE		// Debuging Mode for Development
//*******************************************************************************
// Main Top
//*******************************************************************************
void ISPM LED_Disp(void)
{
#if (model_Lens==0)
	if(MP(MpFad_On)) return;
#endif

	// VLOCKI
#ifdef GPIO_ISP_VI_LED
	if(GpioGetPin(GPIO_ISP_VI_LED))		GpioSetLo(GPIO_ISP_VI_LED);	// LED1
	else								GpioSetHi(GPIO_ISP_VI_LED);
#endif
}

extern BYTE	gbUKeyVal;
void ISPM IF_Funcs(void)
{
#ifdef ECM_REMOTE	
	if(gptMsgShare._ECM_KEY != 0) // ECM Remote (JTAG)
	{
		gbUKeyVal = gptMsgShare._ECM_KEY;
		gptMsgShare._ECM_KEY = 0;
	}
#endif
	if(gptMsgISPTbl.ISP_UKEY != 0) // Web page
	{
		gbUKeyVal = gptMsgISPTbl.ISP_UKEY;
		gptMsgISPTbl.ISP_UKEY = 0;
	}
	gbPTZKeyVal = gptMsgISPTbl.ISP_ZOOM_UKEY;

	Comm(); 								// Communication Interface

	KeyScan();								// Key input detection

#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))

	Menu(); 								// Menu

	AppSavePar();
	AppLoadPar();

#elif ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))

	#if USE_DIGITAL_INPUT_OSD // for Debuging - WHL

	/* Add Box Test Code */
	
	if (!(gbMnDebugBypass==1)) { // Function Bypass for Debugging. CAUTION !! Erase after development
	
		PrivacyBox();							// Box Privacy

		IMD();									// Motion detector

		BoxLast();								// Last Box control
	}
	#endif

	if((BT1120_FPS==50)||(BT1120_FPS==60)) {
//		if(BT1120_FPS==50)	HLOCKI_POS0w(0xa4c);
//		else				HLOCKI_POS0w(0x45c);	
		BT1120_FLAG=0;
	}
	else{
//		if(BT1120_FPS==25)	HLOCKI_POS0w(0xa4c);
//		else				HLOCKI_POS0w(0x45c);	
		BT1120_FLAG=0;
		}
#endif
}

void isp_main(void)
{
	static UINT nTickSta_VDO = 0;
	TICK_END(VDO);
	DispDatDec2(gbMnDebugFnc==1, "VLOCKO us", 18, 24, tick2us(VDO), 6);
	nTickSta_VDO = GetTT();


#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))

	if (!((gbMnDebugBypass==1)||(model_Test==1))) { // Function Bypass for Debugging. CAUTION !! Erase after development

		OutMode();							// Output mode management (fixed function)

		Focus;								// Auto Focus or Focus Assist

		Ae();								// Auto exposure

		TDN();								// Day & Night

		Gamma();							// Gamma control

		AceDefog(); 						// ACE & Defog

		Awb();								// Auto white balance
			
		Adnr();								// DNR Simple 3D Only
		
		Sharpness();						// Sharpness (aperture)

		Mirror();							// Mirror on/off

		Flip();								// Flip on/off

		PrivacyBox();						// Box Privacy

		HlMask();							// High light mask

		CSup(); 							// Color surpression

		ColorBar(); 						// Color Bar

		ColorSpace();						// Color space selelction

		IMD();								// Motion detector
		//extern void MotionTest(void);		// Motion detector Test
		//MotionTest();

		Shading();							// Shading correction

		DefectAuto();						// Defect correction

		BoxLast();							// Last Box control

		FrameModeSet();						// Frame Mode / Freq
		
		MsgMenuSet();						// 180319 KDH call menu function via Msg (cgi/onvif to isp)

		AeDev();

#if (DOOR_BELL_CAMERA==1)
		DoorBell_PowerCtr();

#endif

#if (IMAGE_INPUT_MODE==1)
		PIP_Disp(); 						// PIP (Picture In Picture - Support 2CH)

#endif

#if (LVDS_MARGIN_TEST==1)
		if( gbLvdsOn ) Lvds_Adjust();

#endif
	}
	else {

	}

	// For Sync Debugging	
	extern UINT gnHTBTR_End;
	DebugDisp((gbMnDebugFnc==8), Dec, "ISP Prcs", 17, 24, gnHTBTR_End);
	DebugDisp((gbMnDebugFnc==8), Dec, "FPS_VDI:", model_1M? 1:19, 24, FPS_VDI);
	DebugDisp((gbMnDebugFnc==8), Dec, "FPS_VDO:", model_1M? 2:20, 24, FPS_VDO);
	DebugDisp((gbMnDebugFnc==8), Dec, "Vlock-I:", model_1M? 4:22, 24, gnIsrISP0);
	DebugDisp((gbMnDebugFnc==8), Dec, "Vlock-W:", model_1M? 5:23, 24, gnIsrISP1);
	DebugDisp((gbMnDebugFnc==8), Dec, "Vlock-O:", model_1M? 6:24, 24, gnIsrISP2);
	
	DebugDisp((gbMnDebugFnc==8), Dec, "GPI IRQ:", model_1M? 8:26, 24, gnIsrGpio);


#elif ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)) // BT656 or BT1120


#endif
}

/* EOF */

