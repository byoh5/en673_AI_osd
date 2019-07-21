	/* **************************************************************************
 File Name	: 	isp_app_maf.c
 Description	:	Motorized AF Function
 Designer		:	Lee, Wanhee
 Date		:	17.11.07
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"

#if (model_Lens==1)
#include "isp_app_maf.h"

//=================================================================================================
//	Main function 
//=================================================================================================
void Motorized_AF(void) // 180320 LWH
{
	static BYTE bInitFlag=1;	

	gnMAF_PTime[0]=Start_HTBTR(); // Processing Time Check
	
	// Initialization
	if(bInitFlag) { MAF_Init(); bInitFlag=0; }

	MAF_CmdCheck();

	if(gbMAF_Excute) {

		if(MP(MpMAF_TestMode)) Lens_AgingTest(); // Lens Aging Test Mode
		else {
			switch (gbAF_MainState) {
				case 1 : { // Lens Initialization
					Lens_Initialization();
					break;
				}
				case 2 : { // Trace Curve Auto Calibration
					TraceCurve_Calibration();
					break;
				}
				default : { // Normal Operation
					MAF_ExternalControl();

					// Motor Control
					MAF_MnlMotorControl((gbMenuState==0)); // Control @Menu Off
					MAF_AutoFocus();
					break;
				}
			}
		}		
		MAF_Current_Mng();    // Motor Sleep Mode Managing
		
		MAF_Lens_StateDisp(); // State Display(Box/Font)	
		MAF_DebugingDisp();   // State Display(Font/Grp)

		if(PAR09) MAF_Test_Mode(); // Extern control test enable
	}	
	gnMAF_PTime[1]=End_HTBTR(gnMAF_PTime[0]); // Processing Time Check
}

//=================================================================================================
//	Basic Environment Setting
//=================================================================================================
void MAF_Init(void)
{
	int i=0;
	
	for(i=0; i<MAF_TRACKING_TBL_STEP; i++) gnMaf_TblPosition_Z[i]=(gnMaf_Start_Z[MP(MpMAF_LensModel)]+(gnMaf_End_Z[MP(MpMAF_LensModel)]-gnMaf_Start_Z[MP(MpMAF_LensModel)])*i/(MAF_TRACKING_TBL_STEP-1));

	giMaf_GainRange[0]=0;
	giMaf_GainRange[1]=Sens(AE_GAIN_TOTAL);

	gnMaf_LibCurPos_F  = &Maf_Fmotor.nCurPos;
	gnMaf_LibTgtPos_F  = &Maf_Fmotor.nTgtPos;
	gbMaf_LibTgtCtr_Go = &Maf_Fmotor.bPosGo;

	MP(MpMAF_MotorInit)=0;
	
	gbAF_MainState=(MP(MpMAF_mode)!=2);

//	ISP AF ODM Environment Setting
	AF_YSELw(3);
	AF_YLPFw(0);
	
	SetIsp(0x1CA, MAF_ODM_H);
	SetIsp(0x1CB, MAF_ODM_V);
	SetIsp(0x1CE, 0x03C00000);
	SetIsp(0x1CF, 0x06870000);
}

//=================================================================================================
//	Control Command check
//=================================================================================================
void MAF_CmdCheck(void)
{
	static BYTE bMAF_LensModelBuf=0;
	
	if(bMAF_LensModelBuf!=MP(MpMAF_LensModel)) {
		MAF_Init();
		bMAF_LensModelBuf=MP(MpMAF_LensModel);
	}	
	
	// Check Setting Value
	if(MP(MpMAF_Filter)) {
		static BYTE bMAF_DnN_Cnt=0;
		
		bMAF_DnN_Cnt = (gbDnN_Changed!=(BYTE)gnTdnChkOut) ? MAF_ONESHOT_FILTER_TIME : //(MAF_ONESHOT_TRG_TIME<<1);
		               (bMAF_DnN_Cnt>1) ? (bMAF_DnN_Cnt-1) : 0;

		if(bMAF_DnN_Cnt==1) gbAutoFocus_Flag = 1;
	}
	gbDnN_Changed=(BYTE)gnTdnChkOut;
	
	gbAF_MainState = (MP(MpMAF_MotorInit)) ? 1 : (gbMAF_Calibration) ? 2 : gbAF_MainState;	
	if(MP(MpMAF_MotorInit)==1) MP(MpMAF_MotorInit) =0;

	// Exception state
	static BYTE bExcuteFlag=0;

	//if(MN_FPS_HI) bExcuteFlag=(bExcuteFlag==0);
	bExcuteFlag = (MN_FPS_HI) ? !bExcuteFlag : 0;
		
	//gbMAF_Excute = (!((gbWdrOn&&(gnLSflag==0))||bExcuteFlag)); // WDR On & Short mode || FPS High
	gbMAF_Excute = (!((gbWdrOn&&(LS_FLGr==AeLONG))||bExcuteFlag)); // WDR On & Short mode || FPS High

}

//=================================================================================================
//	Lens Initialization
//=================================================================================================
void Lens_Initialization(void)
{
	switch(gbMaf_InitState) {
		case 0 : { // Motor Move to Zero Point & Position Initialization
			gbAutoFocus_Flag = gbMAF_OneTimeState = 0;
			
			Maf_Zmotor.nCurPos = gnMaf_Range_Z[MP(MpMAF_LensModel)];
			Maf_Fmotor.nCurPos = gnMaf_Range_F[MP(MpMAF_LensModel)];
			Maf_Zmotor.nTgtPos = Maf_Fmotor.nTgtPos = 0;
			Maf_Zmotor.bPosGo = Maf_Fmotor.bPosGo = 1;
			gbMaf_InitState=1;
			gbMaf_GlbRst=1;
			break;
		}
		case 1 : {
			if((Maf_Zmotor.bPosGo==0)&&(Maf_Fmotor.bPosGo==0)) gbMaf_InitState++;
			break;
		}
		case 2 : { // Move to Margin
			Maf_Zmotor.nTgtPos = gnMaf_TblPosition_Z[0];
			Maf_Fmotor.nTgtPos = gnMaf_TblPosition_F[MP(MpMAF_LensModel)][0];
			Maf_Zmotor.bPosGo = Maf_Fmotor.bPosGo = 1;
			gbMaf_InitState=3;
			break;
		}
		case 3 : {
			if((Maf_Zmotor.bPosGo==0)&&(Maf_Fmotor.bPosGo==0)) gbMaf_InitState++;
			break;
		}
		default : { //case 4 :
			gbAF_MainState=gbMaf_InitState=0;
			gbMAF_TrgCnt = MAF_ONESHOT_TRG_TIME;
			break;
		}
	}
}

//=================================================================================================
//	Trace Curve Auto Calibration
//=================================================================================================
void TraceCurve_Calibration(void)
{
	UINT i=0;
	static BYTE bInitState[2]={0,0};

	if(gbMaf_GlbRst) { gbMaf_GlbRst=bInitState[0]=bInitState[1]=0; }
	
	if(bInitState[0]==(sizeof(gnMaf_TblPosition_Z)/sizeof(gnMaf_TblPosition_Z[0]))) {
		gbAF_MainState=bInitState[1]=bInitState[0]=0;
				
		printf("\r\nTrace Curve Calibration Finish~!!");
		printf("\r\n---------------------------------");
		printf("\r\nZoom Total Range  : 0x%04x", gnMaf_Range_Z[MP(MpMAF_LensModel)]);
		printf("\r\nZoom Start Posit  : 0x%04x", gnMaf_Start_Z[MP(MpMAF_LensModel)]);
		printf("\r\nZoom End   Posit  : 0x%04x", gnMaf_End_Z[MP(MpMAF_LensModel)]);
		printf("\r\n---------------------------------");
		printf("\r\nFocus Total Range : 0x%04x", gnMaf_Range_F[MP(MpMAF_LensModel)]);
		printf("\r\nFocus Start Posit : 0x%04x", gnMaf_Start_F[MP(MpMAF_LensModel)]);
		printf("\r\nFocus End   Posit : 0x%04x", gnMaf_End_F[MP(MpMAF_LensModel)]);
		printf("\r\n---------------------------------");
		
		printf("\r\n{ ");
		for(i=0; i<(sizeof(gnMaf_TblPosition_Z)/sizeof(gnMaf_TblPosition_Z[0])); i++) {
			if(i==(sizeof(gnMaf_TblPosition_Z)/sizeof(gnMaf_TblPosition_Z[0])-1)) printf("0x%04x", gnMaf_TblPosition_F[MP(MpMAF_LensModel)][i]);
			else printf("0x%04x,", gnMaf_TblPosition_F[MP(MpMAF_LensModel)][i]);
		}
		printf(" },\r\n");
	}
	else {
		if(bInitState[1]==0) {
			
			gbAutoFocus_Flag = gbMAF_OneTimeState = 0;
			
			printf("Zoom Trace Table #%d\r\n", bInitState[0]);
			Maf_Zmotor.nTgtPos = gnMaf_TblPosition_Z[bInitState[0]];
			Maf_Zmotor.bPosGo = 1;
			bInitState[1]++;
		}
		else if(bInitState[1]==1) {
			if(Maf_Zmotor.bPosGo==0) bInitState[1]++;
		}
		else if(bInitState[1]==2) {
			gbAutoFocus_Flag=1;
			bInitState[1]++;
		}
		else {
			if(gbAutoFocus_Flag==0) {
				gnMaf_TblPosition_F[MP(MpMAF_LensModel)][bInitState[0]] = Maf_Fmotor.nCurPos;
				bInitState[0]++;
				bInitState[1]=0;
			}
			else {
				MAF_DataBuffering();
				MAF_PreProcessing();
				MAF_OneTimeAF();
			}
		}
	}
}

//=================================================================================================
//	External Control (Onvif/PELCO)
//=================================================================================================
void MAF_ExternalControl(void)
{
	if(MP(MpMAF_mode)!=2) {
		// Physical Key
		MAF_TELE_Z = (pKEY_U) ? 1 : 0; //Key_UP
		MAF_WIDE_Z = (pKEY_D) ? 1 : 0; //Key_Down
		MAF_NEAR_F = (pKEY_R) ? 1 : 0; //Key_Right
		MAF_FAR_F  = (pKEY_L) ? 1 : 0; //Key_Left

#if 1 // Key from Web page & Remote ECM Key (UKey)

		#define KEY_KEEP_CNT 4
		static BYTE bKEY_VT_Cnt[2]={0,0}, bVT_Key=0;
		if(gbMAF_KeyBuf) {
			if(bKEY_VT_Cnt[0]) {
				bKEY_VT_Cnt[0]=bKEY_VT_Cnt[1]=KEY_KEEP_CNT;
				bVT_Key=gbMAF_KeyBuf;
			}
			else {
				bKEY_VT_Cnt[0]=KEY_KEEP_CNT;
				bKEY_VT_Cnt[1]=1;
				bVT_Key=gbMAF_KeyBuf;
			}
		}
		gbMAF_KeyBuf=0;
			
		if(bKEY_VT_Cnt[1]) {
			bKEY_VT_Cnt[1]--;

			MAF_TELE_Z = ((bVT_Key   )&1) ? 1 : 0; //Key_UP
			MAF_WIDE_Z = ((bVT_Key>>1)&1) ? 1 : 0; //Key_Down

			MAF_NEAR_F = ((bVT_Key>>2)&1) ? 1 : 0; //Key_Right
			MAF_FAR_F  = ((bVT_Key>>3)&1) ? 1 : 0; //Key_Left
		}
		else MAF_TELE_Z=MAF_WIDE_Z=MAF_NEAR_F=MAF_FAR_F=0;
		
		if(bKEY_VT_Cnt[0]) bKEY_VT_Cnt[0]--;
		
#endif

#if 1 // Web page continuous Key control
		static UINT nKEY_VALUE_Buf[2]={0,0};
	
		if(nKEY_VALUE_Buf[0]!=gptMsgISPTbl.KEY_VALUE) {
			nKEY_VALUE_Buf[0]=nKEY_VALUE_Buf[1]=gptMsgISPTbl.KEY_VALUE;
		}
	
		if(nKEY_VALUE_Buf[1]) {
			nKEY_VALUE_Buf[1]--;
	
			switch(gptMsgISPTbl.KEY_DIRECTION) { // 0:None, 1:Zoom-In, 2:Zoom-Out, 3:Focus-Near, 4:Focus-Far
				case 1 : { MAF_TELE_Z = 1; break; }
				case 2 : { MAF_WIDE_Z = 1; break; }
				case 3 : { MAF_NEAR_F = 1; break; }
				case 4 : { MAF_FAR_F  = 1; break; }
			}
			if(nKEY_VALUE_Buf[1]==0) gptMsgISPTbl.KEY_VALUE=0;
		}
#endif
	} // End of if(MP(MpMAF_mode)!=2)

//	Zoom  Motor Speed Set (7Steps or 3Steps)
//	Focus Motor Speed Set (7Steps or 3Steps)

//	Zoom Motor Direction(Wide<->Tele) Control (By Key, PELCO)
//	Focus Motor Direction(Near<->Far) Control (By Key, PELCO)

	if(gptMsgISPTbl.ISP_FOCUS_DIR&0x1) { // Focus Manual Control (Plus Direction)
		Maf_Fmotor.nTgtPos = MINMAX(Maf_Fmotor.nCurPos+(gptMsgISPTbl.ISP_FOCUS_VALUE)*(gnMaf_End_F[MP(MpMAF_LensModel)]-gnMaf_Start_F[MP(MpMAF_LensModel)])/100,gnMaf_Start_F[MP(MpMAF_LensModel)],gnMaf_End_F[MP(MpMAF_LensModel)]);
		Maf_Fmotor.bPosGo = 1;
		gptMsgISPTbl.ISP_FOCUS_DIR = 0;
	}
	else if(gptMsgISPTbl.ISP_FOCUS_DIR&0x2) { // Focus Manual Control (Minus Direction)
		Maf_Fmotor.nTgtPos = MINMAX(Maf_Fmotor.nCurPos-(INT)(gptMsgISPTbl.ISP_FOCUS_VALUE*(gnMaf_End_F[MP(MpMAF_LensModel)]-gnMaf_Start_F[MP(MpMAF_LensModel)])/100),gnMaf_Start_F[MP(MpMAF_LensModel)],gnMaf_End_F[MP(MpMAF_LensModel)]);
		Maf_Fmotor.bPosGo = 1;
		gptMsgISPTbl.ISP_FOCUS_DIR = 0;
	}	
	gnMaf_FPosRange[0]=gnMaf_Start_F[MP(MpMAF_LensModel)];
	gnMaf_FPosRange[1]=gnMaf_End_F[MP(MpMAF_LensModel)]  ;

#if 1 // Onvif PTZ Control
	static BYTE bControlState=0;

	if(gbPTZKeyVal&0x80) {
		if(bControlState==0) {
			Maf_Zmotor.nTgtPos = gnMaf_Start_Z[MP(MpMAF_LensModel)]+((gnMaf_End_Z[MP(MpMAF_LensModel)]-gnMaf_Start_Z[MP(MpMAF_LensModel)])/100*(int)(gbPTZKeyVal&0x7F));
			Maf_Zmotor.bPosGo = 1;
			bControlState++;
		}
		else if(bControlState==1) {
			if(Maf_Zmotor.bPosGo==0) bControlState++;
		}
		else if(bControlState==2) {
			gbAutoFocus_Flag = 1;
			bControlState++;
		}
		else if(bControlState==3) {
			if(Maf_Fmotor.bPosGo==0) {
				gbPTZKeyVal=0;
				gptMsgISPTbl.ISP_ZOOM_UKEY=0;
				bControlState=0;
			}
		}
	}
	else if(gbPTZKeyVal&0x8000) {
		if(bControlState==0) {
			Maf_Fmotor.nTgtPos = MINMAX(gnMaf_Start_F[MP(MpMAF_LensModel)]+((gnMaf_End_F[MP(MpMAF_LensModel)]-gnMaf_Start_F[MP(MpMAF_LensModel)])/100+(int)(gbPTZKeyVal&0x7FFF)),0,gnMaf_End_F[MP(MpMAF_LensModel)]);
			Maf_Fmotor.bPosGo = 1;
			bControlState++;
		}
		else if(bControlState==1) {
			if(Maf_Fmotor.bPosGo==0) {
				gbPTZKeyVal=0;
				gptMsgISPTbl.ISP_ZOOM_UKEY=0;
				bControlState=0;
			}
		}
	}
#endif
}

//=================================================================================================
//	Motor Manual Control
//=================================================================================================
void MAF_MnlMotorControl(BYTE abExcute)
{	
	static BYTE bActBuf_Z=0, bActBuf_F=0;
		
	if(abExcute) { // Key Control @Menu off
		
		gbMAF_ZoomTrkEn = gbManualCtr_F = 0;

		if(gbMAF_DevMode==1) { // Development Mode
		
//			Maf_Zmotor.nTgtPos = (MAF_WIDE_Z) ? (Maf_Zmotor.nCurPos-MAF_MAX_STEP) :
			Maf_Zmotor.nTgtPos = (MAF_WIDE_Z) ? ((Maf_Zmotor.nCurPos>MAF_MAX_STEP) ? (Maf_Zmotor.nCurPos-MAF_MAX_STEP) : 0) :
								 (MAF_TELE_Z) ? (Maf_Zmotor.nCurPos+MAF_MAX_STEP) : Maf_Zmotor.nTgtPos;
		
//			Maf_Fmotor.nTgtPos = (MAF_NEAR_F) ? (Maf_Fmotor.nCurPos-MAF_MNL_STEP_F) :
			Maf_Fmotor.nTgtPos = (MAF_NEAR_F) ? ((Maf_Fmotor.nCurPos>MAF_MNL_STEP_F) ? (Maf_Fmotor.nCurPos-MAF_MNL_STEP_F) : 0) :
								 (MAF_FAR_F ) ? (Maf_Fmotor.nCurPos+MAF_MNL_STEP_F) : Maf_Fmotor.nTgtPos;
								  
			Maf_Zmotor.bPosGo = (MAF_WIDE_Z|MAF_TELE_Z) ? 1 : Maf_Zmotor.bPosGo;
			Maf_Fmotor.bPosGo = (MAF_NEAR_F|MAF_FAR_F ) ? 1 : Maf_Fmotor.bPosGo;

			gbMnDebugFnc=2;
			if(gbSAFDebugOn==0) { OsdWhiteAll(); OsdClearAll(); ChangeMenuSize(); }
		}
		else { // Normal Mode
		
	
		// Zoom Manual Control
			if(MAF_WIDE_Z|MAF_TELE_Z) {

				MAF_ZoomTracking();

				gbAutoFocus_Flag = gbMAF_OneTimeState = gbMAF_TrgCnt = 0;
			}
			else if(bActBuf_Z!=(MAF_WIDE_Z|MAF_TELE_Z)) {
				Maf_Zmotor.nTgtPos = Maf_Zmotor.nCurPos;
				Maf_Zmotor.bPosGo = 1;
				gbMAF_TrgCnt = MAF_ONESHOT_TRG_TIME; //30;
			}
			bActBuf_Z=(MAF_WIDE_Z|MAF_TELE_Z);

			if(gbMAF_TrgCnt==1) gbAutoFocus_Flag = (MP(MpMAF_mode)==0); // AF Excution @Zoom Trigger
			gbMAF_TrgCnt = (gbMAF_TrgCnt>1) ? (gbMAF_TrgCnt-1) : 0;
				
	
		// Focus Manual Control
			if(MAF_NEAR_F|MAF_FAR_F) {
				
				gbAutoFocus_Flag = 0;
				
				if(gbMaf_MnlSpeedCnt_F<MAF_MNL_STEP_F) { // Minimum Step

					if(MAF_NEAR_F) Maf_Fmotor.nTgtPos = (Maf_Fmotor.nCurPos>(gnMaf_Start_F[MP(MpMAF_LensModel)]+MAF_MNL_STEP_F)) ? (Maf_Fmotor.nCurPos-MAF_MNL_STEP_F) : gnMaf_Start_F[MP(MpMAF_LensModel)];
					else           Maf_Fmotor.nTgtPos = (Maf_Fmotor.nCurPos<(gnMaf_End_F[MP(MpMAF_LensModel)]  -MAF_MNL_STEP_F)) ? (Maf_Fmotor.nCurPos+MAF_MNL_STEP_F) : gnMaf_End_F[MP(MpMAF_LensModel)]  ;
					
					gbMaf_MnlSpeedCnt_F++;
				}
				else Maf_Fmotor.nTgtPos = (MAF_NEAR_F) ? gnMaf_Start_F[MP(MpMAF_LensModel)] : gnMaf_End_F[MP(MpMAF_LensModel)];

				Maf_Fmotor.bPosGo = gbManualCtr_F = 1;

				gbMAF_OneTimeState = 0;
			}
			else if(bActBuf_F!=(MAF_NEAR_F|MAF_FAR_F)) {
				Maf_Fmotor.nTgtPos = Maf_Fmotor.nCurPos;
				Maf_Fmotor.bPosGo = gbManualCtr_F = 1;
				gbMaf_MnlSpeedCnt_F = 0;
			}
			bActBuf_F=(MAF_NEAR_F|MAF_FAR_F);
		}
	}
	else { gbMAF_ZoomTrkEn = Maf_Zmotor.bPosGo = Maf_Fmotor.bPosGo = 0; } // Motor control stop @ Menu On
}

//=================================================================================================
//	Zoom Tracking Function
//=================================================================================================
void MAF_ZoomTracking(void)
{
	UINT i=0;
	BYTE bTraceNo[2]={0,0};
	
	if(MAF_WIDE_Z) Maf_Zmotor.nTgtPos = (Maf_Zmotor.nCurPos>(gnMaf_Start_Z[MP(MpMAF_LensModel)]+MAF_MAX_STEP)) ? (Maf_Zmotor.nCurPos-MAF_MAX_STEP) : gnMaf_Start_Z[MP(MpMAF_LensModel)];
	else           Maf_Zmotor.nTgtPos = (Maf_Zmotor.nCurPos<(gnMaf_End_Z[MP(MpMAF_LensModel)]  -MAF_MAX_STEP)) ? (Maf_Zmotor.nCurPos+MAF_MAX_STEP) : gnMaf_End_Z[MP(MpMAF_LensModel)]  ;

	// Control Aception condition
	gbMAF_ZoomTrkEn = ((MAF_WIDE_Z&(Maf_Zmotor.nCurPos>gnMaf_Start_Z[MP(MpMAF_LensModel)]))||(MAF_TELE_Z&(Maf_Zmotor.nCurPos<gnMaf_End_Z[MP(MpMAF_LensModel)])));
	
	if(Maf_Zmotor.nTgtPos==gnMaf_Start_Z[MP(MpMAF_LensModel)]) { // Wide side Exception condition
		Maf_Fmotor.nTgtPos = gnMaf_TblPosition_F[MP(MpMAF_LensModel)][0];	
		bTraceNo[0]=0;
		bTraceNo[1]=1;
	}
	else if(Maf_Zmotor.nTgtPos==gnMaf_End_Z[MP(MpMAF_LensModel)]) { // Tele side Exception condition
		Maf_Fmotor.nTgtPos = gnMaf_TblPosition_F[MP(MpMAF_LensModel)][MAF_TRACKING_TBL_STEP-1];
		bTraceNo[0]=(MAF_TRACKING_TBL_STEP-2);
		bTraceNo[1]=(MAF_TRACKING_TBL_STEP-1);
	}
	else {
		for(i=0;i<MAF_TRACKING_TBL_STEP; i++) {
		if	   (gnMaf_TblPosition_Z[i]==Maf_Zmotor.nTgtPos) bTraceNo[0] = (MAF_TELE_Z) ? (i-1) : i;
		else if(gnMaf_TblPosition_Z[i] <Maf_Zmotor.nTgtPos) bTraceNo[0] = i;
		}
		for(i=(MAF_TRACKING_TBL_STEP-1);i>0; i--) {
			if	   (gnMaf_TblPosition_Z[i]==Maf_Zmotor.nTgtPos) bTraceNo[1] = (MAF_WIDE_Z) ? (i+1) : i;
			else if(gnMaf_TblPosition_Z[i] >Maf_Zmotor.nTgtPos) bTraceNo[1] = i;
		}
		Maf_Fmotor.nTgtPos = (UINT)LibUtlInterp1D ((int)Maf_Zmotor.nTgtPos, (int)gnMaf_TblPosition_Z[bTraceNo[0]], (int)gnMaf_TblPosition_Z[bTraceNo[1]], (int)gnMaf_TblPosition_F[MP(MpMAF_LensModel)][bTraceNo[0]], (int)gnMaf_TblPosition_F[MP(MpMAF_LensModel)][bTraceNo[1]]);	}

	if(ABS((int)Maf_Fmotor.nTgtPos-(int)Maf_Fmotor.nCurPos)>MAF_MAX_STEP) {

		if(Maf_Fmotor.nTgtPos<Maf_Fmotor.nCurPos) Maf_Fmotor.nTgtPos = (Maf_Fmotor.nCurPos>(gnMaf_Start_F[MP(MpMAF_LensModel)]+MAF_MAX_STEP)) ? (Maf_Fmotor.nCurPos-MAF_MAX_STEP) : gnMaf_Start_F[MP(MpMAF_LensModel)];
		else                                      Maf_Fmotor.nTgtPos = (Maf_Fmotor.nCurPos<(gnMaf_End_F[MP(MpMAF_LensModel)]  -MAF_MAX_STEP)) ? (Maf_Fmotor.nCurPos+MAF_MAX_STEP) : gnMaf_End_F[MP(MpMAF_LensModel)]  ;

		Maf_Zmotor.nTgtPos = (UINT)LibUtlInterp1D ((int)Maf_Fmotor.nTgtPos, (int)gnMaf_TblPosition_F[MP(MpMAF_LensModel)][bTraceNo[0]], (int)gnMaf_TblPosition_F[MP(MpMAF_LensModel)][bTraceNo[1]], (int)gnMaf_TblPosition_Z[bTraceNo[0]], (int)gnMaf_TblPosition_Z[bTraceNo[1]]);
		
		Maf_Zmotor.nTgtPos = ((MAF_WIDE_Z==1)&&(Maf_Zmotor.nTgtPos>Maf_Zmotor.nCurPos)) ? Maf_Zmotor.nCurPos : // Exception Condition
							 ((MAF_TELE_Z==1)&&(Maf_Zmotor.nTgtPos<Maf_Zmotor.nCurPos)) ? Maf_Zmotor.nCurPos : Maf_Zmotor.nTgtPos;
	}
	Maf_Zmotor.bPosGo = Maf_Fmotor.bPosGo = gbMAF_ZoomTrkEn;
}

//=================================================================================================
//	Auto Focus (No LIB)
//=================================================================================================
void MAF_DataBuffering(void)
{
	gnMaf_AF_DATA[0]=AF_SUM1_LOCKr;
	gnMaf_AF_DATA[1]=AF_SUM2_LOCKr;
	gnMaf_Y_DATA[0]=AF_YSUM1_LOCKr;
	gnMaf_Y_DATA[1]=AF_YSUM2_LOCKr;
	giMaf_CurGain=giCurAgc;
}

#define MAF_MAX_FRAME_CNT 30 // 1S @30FPS
void MAF_OneTimeAF_Check(void)
{
	static UINT nMAF_ChkCnt=0;

	nMAF_ChkCnt = (gbAutoFocus_Flag) ? (nMAF_ChkCnt+1) : 0;

	if(nMAF_ChkCnt>MAF_MAX_FRAME_CNT) {
		gbAutoFocus_Flag=0; // AF Stop
		// Add Error Message
	}
}

void MAF_AutoFocus(void)	// Oneshot AF(.LIB)
{
	MAF_DataBuffering();
	MAF_PreProcessing();

	if(gbAutoFocus_Flag) MAF_OneTimeAF();

//	MAF_OneTimeAF_Check();
}

//=================================================================================================
//	Lens State Display
//=================================================================================================
void MAF_Lens_StateDisp(void)
{
#if (MAF_BOSD_DISP!=0)
	static BYTE bMAF_BoxCnt=0;

//	if(Maf_Zmotor.bPosGo|Maf_Fmotor.bPosGo) bMAF_BoxCnt = BAF_BOXOSD_TIME;
	if(Maf_Zmotor.bPosGo|Maf_Fmotor.bPosGo|gbMAF_DevMode) bMAF_BoxCnt = BAF_BOXOSD_TIME;
	else {
		bMAF_BoxCnt = (bMAF_BoxCnt>0) ? bMAF_BoxCnt-1 : 0;
	}

	if(bMAF_BoxCnt) {
		
		gbMAFBoxFlag=1;

	#if (MAF_BOSD_DISP==1) // State Display (Box OSD STYLE #1 / Coordinates)

		if(bMAF_BoxCnt==BAF_BOXOSD_TIME) {
			gnBoxPosMAF_Y[3] = (((LibUtlInterp1D(gnMaf_End_F[MP(MpMAF_LensModel)]  , 0, gnMaf_Range_F[MP(MpMAF_LensModel)], 0x140, 0x040)<<16)&0xFFFF0000) // Active Area
				                |(LibUtlInterp1D(gnMaf_Start_F[MP(MpMAF_LensModel)], 0, gnMaf_Range_F[MP(MpMAF_LensModel)], 0x140, 0x040)     &0x0000FFFF));
			gnBoxPosMAF_X[3] = (((LibUtlInterp1D(gnMaf_Start_Z[MP(MpMAF_LensModel)], 0, gnMaf_Range_Z[MP(MpMAF_LensModel)], 0x580, 0x720)<<16)&0xFFFF0000)
				                |(LibUtlInterp1D(gnMaf_End_Z[MP(MpMAF_LensModel)]  , 0, gnMaf_Range_Z[MP(MpMAF_LensModel)], 0x580, 0x720)     &0x0000FFFF));
		}		
		gnBoxPosMAF_Y[2] = LibUtlInterp1D (Maf_Fmotor.nCurPos, 0, gnMaf_Range_F[MP(MpMAF_LensModel)], 0x140, 0x40); // Focus Pos
		gnBoxPosMAF_Y[2] = (((gnBoxPosMAF_Y[2]<<16)&0xFFFF0000)|(gnBoxPosMAF_Y[2]&0x0000FFFF));
		gnBoxPosMAF_X[1] = LibUtlInterp1D (Maf_Zmotor.nCurPos, 0, gnMaf_Range_Z[MP(MpMAF_LensModel)], 0x580, 0x720); // Zoom Pos
		gnBoxPosMAF_X[1] = (((gnBoxPosMAF_X[1]<<16)&0xFFFF0000)|(gnBoxPosMAF_X[1]&0x0000FFFF));
		gnBoxPosMAF_Y[0] = gnBoxPosMAF_Y[2]; // Zoom & Focus Point
		gnBoxPosMAF_X[0] = gnBoxPosMAF_X[1];

		if(bMAF_BoxCnt==1) gnBoxOnMAF = 0;
		else               gnBoxOnMAF = MAF_BOX_BITMASK;
		
	#elif (MAF_BOSD_DISP==2) // State Display (Box OSD STYLE #2 / Bar)
	
		if(bMAF_BoxCnt==BAF_BOXOSD_TIME) {
			gnBoxPosMAF_Y[4] = 0x03d403fc;
			gnBoxPosMAF_X[4] = 0x01e005a0;
			gnBoxPosMAF_Y[3] = 0x03ce0402;
			gnBoxPltMAF[3]	 = 0x00fd13b0;
			gnBoxToneMAF 	 = 0x90000000;
		}
//		gnBoxPosMAF_X[3] = LibUtlInterp1D (Maf_Zmotor.nCurPos, 0, gnMaf_Range_Z[MP(MpMAF_LensModel)], 0x1E0, 0x5A0); // Zoom Pos - Total
		gnBoxPosMAF_X[3] = LibUtlInterp1D (Maf_Zmotor.nCurPos, gnMaf_Start_Z[MP(MpMAF_LensModel)], gnMaf_End_Z[MP(MpMAF_LensModel)], 0x1E0, 0x5A0); // Zoom Pos - Active
		gnBoxPosMAF_X[3] = ((((gnBoxPosMAF_X[3]-5)<<16)&0xFFFF0000)|((gnBoxPosMAF_X[3]+5)&0x0000FFFF));

		if(bMAF_BoxCnt==1) gnBoxOnMAF = gnBoxFillMAF = 0;
		else			   gnBoxOnMAF = gnBoxFillMAF = 0xC0000000; //MAF_BOX_BITMASK;
	#endif
	}
	else gnBoxOnMAF = gbMAFBoxFlag = 0;
#endif

#if (MAF_FOSD_DISP==1) // State Display (Font OSD)
	static BYTE bMAF_Disp_Cnt=0;

	bMAF_Disp_Cnt = (bMAF_Disp_Cnt<40) ? bMAF_Disp_Cnt+1 : 0;

	if(bMAF_Disp_Cnt<30) {
 		if     (MAF_LENS_INIT) DispStr("LENS INITIALIZING...", 16, 0, 20);
 		else if(MAF_AUTOFOCUS) DispStr("FOCUS SCANNING...   ", 16, 0, 20);
 		else if(MAF_ZTRACKING) DispStr("ZOOM TRACKING...    ", 16, 0, 20);
 		else if(MAF_AGINGTEST) DispStr("LENS AGING TEST...  ", 16, 0, 20);
// 		else if(MAF_PRESET_MV) DispStr("PRE-SET MOVING...   ", 16, 0, 20);
		else DispClr(16, 0, 20);
	}
	else DispClr(16, 0, 20);
#endif
}

//=================================================================================================
//	Fucntion Debuging Display (Font/Grp)
//=================================================================================================
void MAF_DebugingDisp(void)
{
	gbSAFDebugOn = (gbMnDebugFnc==2);

	if(gbSAFDebugOn) {
//		if(UP(UpTUNING_MODE_EN)) {
			GRP0 = (UINT)gnMaf_TgtPeakVal_F; //AF_SUM1_LOCKr;
			GRP1 = (UINT)gnAF1_Normalization;
			GRP2 = (UINT)AF_SUM1_LOCKr;
			GRP3 = (UINT)Maf_Zmotor.nCurPos;
			GRP4 = (UINT)Maf_Fmotor.bPosGo;
			GRP5 = (UINT)Maf_Fmotor.nCurPos;
			GRP6 = (UINT)Maf_Fmotor.nTgtPos;
			GRP7 = (UINT)giMAF1_DirCntBuf[0];
			TxGrpFnc();
//		}
		OZONE_SELw(2);
	}

	// Processing Time
	DebugDisp(gbSAFDebugOn, Dec, "MafPc[us", 23, 24, gnMAF_PTime[1]);
	DebugDisp(gbSAFDebugOn, Dec, "Timer[us", 24, 24, gnMAF_PTime[3]);
	gnMAF_PTime[3]=0;

	// AF Flag / Test & State
	DebugDisp(gbSAFDebugOn, Hex, "MainStat", 26, 24, gbAF_MainState);
	DebugDisp(gbSAFDebugOn, Hex, "bAF_Flag", 27, 24, gbAutoFocus_Flag);
	DebugDisp(gbSAFDebugOn, Hex, "AFTrgCnt", 28, 24, gbMAF_TrgCnt);
	DebugDisp(gbSAFDebugOn, Hex, "AF_State", 29, 24, gbMAF_OneTimeState);
	DebugDisp(gbSAFDebugOn, Hex, "AT-Times", 30, 24, gnAgingTest_Cnt);

	// Position Info
	DebugDisp(gbSAFDebugOn, Hex, "PosCtr_Z", 32, 24, Maf_Zmotor.bPosGo);
	DebugDisp(gbSAFDebugOn, Hex, "CurPos_Z", 33, 24, Maf_Zmotor.nCurPos);
	DebugDisp(gbSAFDebugOn, Hex, "TgtPos_Z", 34, 24, Maf_Zmotor.nTgtPos);
	DebugDisp(gbSAFDebugOn, Hex, "MvStat_Z", 35, 24, Maf_Zmotor.bMovingState);
	
	DebugDisp(gbSAFDebugOn, Hex, "PosCtr_F", 37, 24, Maf_Fmotor.bPosGo);
	DebugDisp(gbSAFDebugOn, Hex, "CurPos_F", 38, 24, Maf_Fmotor.nCurPos);
	DebugDisp(gbSAFDebugOn, Hex, "TgtPos_F", 39, 24, Maf_Fmotor.nTgtPos);
	DebugDisp(gbSAFDebugOn, Hex, "MvStat_F", 40, 24, Maf_Fmotor.bMovingState);
	DebugDisp(gbSAFDebugOn, Hex, "LENS NO.", 40, 24, MP(MpMAF_LensModel));
}

//=================================================================================================
//	Motor Current Managing
//=================================================================================================
void MAF_Current_Mng(void)
{
	if(Maf_Zmotor.bMovingState==0) { // Zoom Motor Current Save (All Low)
		MAF_HOLD_Z
	}
	if((Maf_Fmotor.bMovingState==0)&&(gbAutoFocus_Flag==0)) { // Focus Motor Current Save (All Low)
		MAF_HOLD_F
	}
	gbMaf_TimerEn = (Maf_Zmotor.bPosGo|Maf_Fmotor.bPosGo|gbAutoFocus_Flag); // Timer Control Enable condition
}

//=================================================================================================
//	GPIO Drive Handling (Timer IRQ Base)
//=================================================================================================
BYTE gbMaf_FullCnt_Z=0, gbMaf_FullCnt_F=0;
		
void ISPM0 MAF_MovingState(void)
{	
	// Z-Position Acceleration step1->step2->step3
	if(Maf_Zmotor.bPosGo) {
        if((gbMAF_DevMode)&&(gbMenuState==1)) Maf_Zmotor.bPosGo=0; // 180514 JJH		
		else if(Maf_Zmotor.nCurPos!=Maf_Zmotor.nTgtPos) {
			Maf_Zmotor.bMovingState = ((Maf_Zmotor.bMovingState<MOTOR_STATE3)&gbMaf_FullCnt_Z) ? (Maf_Zmotor.bMovingState+1) : MOTOR_STATE3;
			Maf_Zmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Zmotor.bMovingState+1]-1)/*500PPS*/;
//			if     (/*SLOW*/) Maf_Zmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Zmotor.bMovingState-1]-1)/*100PPS*/;
//			else if(/*MIDL*/) Maf_Zmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Zmotor.bMovingState  ]-1)/*250PPS*/;
//			else    /*HIGH*/) Maf_Zmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Zmotor.bMovingState+1]-1)/*500PPS*/;
			gbMaf_FullCnt_Z = (Maf_Zmotor.bFreq==(Maf_Zmotor.bSpeedCnt-1)) ? 1 : 0;
			Maf_Zmotor.bFreq=(Maf_Zmotor.bFreq<Maf_Zmotor.bSpeedCnt) ? (Maf_Zmotor.bFreq+1) : 0;
		}
		else Maf_Zmotor.bPosGo=Maf_Zmotor.bActive=Maf_Zmotor.bMovingState=Maf_Zmotor.bFreq=0;
	}
	Maf_Zmotor.bMovingFlag=((Maf_Zmotor.bFreq==0)&Maf_Zmotor.bMovingState); // Motor Operating Enable condition

	// F-Position Acceleration step1->step2->step3
	if(Maf_Fmotor.bPosGo) {
        if((gbMAF_DevMode)&&(gbMenuState==1)) Maf_Fmotor.bPosGo=0; // 180514 JJH		
		else if(Maf_Fmotor.nCurPos!=Maf_Fmotor.nTgtPos) {
			Maf_Fmotor.bMovingState = ((Maf_Fmotor.bMovingState<MOTOR_STATE3)&gbMaf_FullCnt_F) ? (Maf_Fmotor.bMovingState+1) : MOTOR_STATE3; // Acceleration step1->step2->step3
			
			if(gbAutoFocus_Flag) {
//				Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState  ]-1);
				if(Maf_Zmotor.nCurPos<gnMaf_LDof_Z[MP(MpMAF_LensModel)]) Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState  ]-1)/*250PPS*/; // Wide Side AF Speed
				else                                                     Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState  ]   /*166PPS*/); // Tele Side AF Speed
			}
			else {
				if(gbManualCtr_F) { // Focus Manual Speed
					Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState-1]-1)/*100PPS*/;
//						if     (/*SLOW*/) Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState-1]-1)/*100PPS*/;
//						else if(/*MIDL*/) Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState  ]-1)/*250PPS*/;
//						else    /*HIGH*/) Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState+1]-1)/*500PPS*/;
				}
				else Maf_Fmotor.bSpeedCnt=(gbMotorSpeedStep[Maf_Fmotor.bMovingState+1]-1)/*500PPS*/;
			}
			gbMaf_FullCnt_F = (Maf_Fmotor.bFreq==(Maf_Fmotor.bSpeedCnt-1)) ? 1 : 0;
			Maf_Fmotor.bFreq=(Maf_Fmotor.bFreq<Maf_Fmotor.bSpeedCnt) ? (Maf_Fmotor.bFreq+1) : 0;
		}
		else Maf_Fmotor.bPosGo=Maf_Fmotor.bActive=Maf_Fmotor.bMovingState=Maf_Fmotor.bFreq=0;
	}
	Maf_Fmotor.bMovingFlag=((Maf_Fmotor.bFreq==0)&Maf_Fmotor.bMovingState); // Motor Operating Enable condition

	
	// Z-Active Condition
	if(Maf_Zmotor.bMovingFlag) { // Default
		
		if(gbAF_MainState==1) Maf_Zmotor.bActive = (Maf_Zmotor.nCurPos>Maf_Zmotor.nTgtPos) ? 1 : 2; // Debuging or Initialzation
		else { // Normal Operation (Area Limit)
			if(Maf_Zmotor.nCurPos>Maf_Zmotor.nTgtPos) { // Wide Direction
				Maf_Zmotor.bActive     = (Maf_Zmotor.nCurPos>gnMaf_Start_Z[MP(MpMAF_LensModel)]) ? 1 : 0;
				Maf_Zmotor.bDirFlag    = (Maf_Zmotor.nCurPos>gnMaf_Start_Z[MP(MpMAF_LensModel)]) ? 1 : Maf_Zmotor.bDirFlag;
				Maf_Zmotor.bReverseCnt = (Maf_Zmotor.bDirFlagBuf!=Maf_Zmotor.bDirFlag) ? gnMaf_RvsStep_Z[MP(MpMAF_LensModel)] : Maf_Zmotor.bReverseCnt;
				Maf_Zmotor.bDirFlagBuf = Maf_Zmotor.bDirFlag;
			}
			else {                                        // Tele Direction
				Maf_Zmotor.bActive     = (Maf_Zmotor.nCurPos<gnMaf_End_Z[MP(MpMAF_LensModel)]) ? 2 : 0;
				Maf_Zmotor.bDirFlag    = (Maf_Zmotor.nCurPos<gnMaf_End_Z[MP(MpMAF_LensModel)]) ? 2 : Maf_Zmotor.bDirFlag;
				Maf_Zmotor.bReverseCnt = (Maf_Zmotor.bDirFlagBuf!=Maf_Zmotor.bDirFlag) ? gnMaf_RvsStep_Z[MP(MpMAF_LensModel)] : Maf_Zmotor.bReverseCnt;
				Maf_Zmotor.bDirFlagBuf = Maf_Zmotor.bDirFlag;
			}
		}
	}
	else Maf_Zmotor.bActive=0;

	// F-Active Condition
	if(Maf_Fmotor.bMovingFlag) { // Default
		
		if(gbAF_MainState==1) Maf_Fmotor.bActive = (Maf_Fmotor.nCurPos>Maf_Fmotor.nTgtPos) ? 1 : 2; // Debuging or Initialzation
		else { // Normal Operation (Area Limit)
			if(Maf_Fmotor.nCurPos>Maf_Fmotor.nTgtPos) { // Near Direction
				Maf_Fmotor.bActive     = (Maf_Fmotor.nCurPos>gnMaf_Start_F[MP(MpMAF_LensModel)]) ? 1 : 0;
				Maf_Fmotor.bDirFlag    = (Maf_Fmotor.nCurPos>gnMaf_Start_F[MP(MpMAF_LensModel)]) ? 1 : Maf_Fmotor.bDirFlag;
				Maf_Fmotor.bReverseCnt = (Maf_Fmotor.bDirFlagBuf!=Maf_Fmotor.bDirFlag) ? gnMaf_RvsStep_F[MP(MpMAF_LensModel)] : Maf_Fmotor.bReverseCnt;
//				Maf_Fmotor.bReverseCnt = (Maf_Fmotor.bDirFlagBuf!=Maf_Fmotor.bDirFlag) ? PAR1B : Maf_Fmotor.bReverseCnt;
				Maf_Fmotor.bDirFlagBuf = Maf_Fmotor.bDirFlag;
			}
			else {                                        // Far Direction
				Maf_Fmotor.bActive     = (Maf_Fmotor.nCurPos<gnMaf_End_F[MP(MpMAF_LensModel)]) ? 2 : 0;
				Maf_Fmotor.bDirFlag    = (Maf_Fmotor.nCurPos<gnMaf_End_F[MP(MpMAF_LensModel)]) ? 2 : Maf_Fmotor.bDirFlag;
				Maf_Fmotor.bReverseCnt = (Maf_Fmotor.bDirFlagBuf!=Maf_Fmotor.bDirFlag) ? gnMaf_RvsStep_F[MP(MpMAF_LensModel)] : Maf_Fmotor.bReverseCnt;
//				Maf_Fmotor.bReverseCnt = (Maf_Fmotor.bDirFlagBuf!=Maf_Fmotor.bDirFlag) ? PAR1B : Maf_Fmotor.bReverseCnt;
				Maf_Fmotor.bDirFlagBuf = Maf_Fmotor.bDirFlag;
			}
		}
	}
	else Maf_Fmotor.bActive=0;

	if(gbMAF_DevMode==1) {
		if     (MAF_WIDE_Z) Maf_Zmotor.bActive=1;
		else if(MAF_TELE_Z) Maf_Zmotor.bActive=2;
		else                Maf_Zmotor.bActive=0;
		
		if     (MAF_NEAR_F) Maf_Fmotor.bActive=1;
		else if(MAF_FAR_F ) Maf_Fmotor.bActive=2;
		else                Maf_Fmotor.bActive=0;
	}
}

void ISPM0 MAF_PhaseControl(void)
{
	gnMAF_PTime[2]=Start_HTBTR(); // Processing Time Check
		
	MAF_MovingState();
	
	// Zoom Motor Control
	if(Maf_Zmotor.bActive) { // Active condition
		switch(Maf_Zmotor.bStateStep) { // Zoom Motor Driving Step
			case MAF_STEP1 : { MAF_H(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 1 1 0 0 (step1)
			case MAF_STEP2 : { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 0 1 0 0 (step2)
			case MAF_STEP3 : { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 0 1 1 0 (step3)
			case MAF_STEP4 : { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 0 0 1 0 (step4)
			case MAF_STEP5 : { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 0 0 1 1 (step5)
			case MAF_STEP6 : { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 0 0 0 1 (step6)
			case MAF_STEP7 : { MAF_H(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 1 0 0 1 (step7)
			case MAF_STEP8 : { MAF_H(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); break; } // 1 0 0 0 (step8)
		}
		if     (Maf_Zmotor.bActive==1) Maf_Zmotor.bStateStep = (Maf_Zmotor.bStateStep<MAF_STEP8) ? (Maf_Zmotor.bStateStep+MAF_STEP1) : MAF_STEP1; // Wide Direction
		else if(Maf_Zmotor.bActive==2) Maf_Zmotor.bStateStep = (Maf_Zmotor.bStateStep>MAF_STEP1) ? (Maf_Zmotor.bStateStep-MAF_STEP1) : MAF_STEP8; // Tele Direction
		
		if(Maf_Zmotor.bReverseCnt) Maf_Zmotor.bReverseCnt--;
//		else Maf_Zmotor.nCurPos = (Maf_Zmotor.bActive==1) ? (Maf_Zmotor.nCurPos-1) : (Maf_Zmotor.nCurPos+1); // Position_F Count
		else Maf_Zmotor.nCurPos = (Maf_Zmotor.bActive==1) ? ((Maf_Zmotor.nCurPos>0) ? (Maf_Zmotor.nCurPos-1) : Maf_Zmotor.nCurPos) : (Maf_Zmotor.nCurPos+1);
	}

	// Focus Motor Control	
	if(Maf_Fmotor.bActive) { // Active condition
		switch(Maf_Fmotor.bStateStep) { // Focus Motor Driving Step
			case MAF_STEP1 : { MAF_H(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 1 1 0 0 (step1)
			case MAF_STEP2 : { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 0 1 0 0 (step2)
			case MAF_STEP3 : { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 0 1 1 0 (step3)
			case MAF_STEP4 : { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 0 0 1 0 (step4)
			case MAF_STEP5 : { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 0 0 1 1 (step5)
			case MAF_STEP6 : { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 0 0 0 1 (step6)
			case MAF_STEP7 : { MAF_H(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_H(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 1 0 0 1 (step7)
			case MAF_STEP8 : { MAF_H(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); break; } // 1 0 0 0 (step8)
		}
		if     (Maf_Fmotor.bActive==1) Maf_Fmotor.bStateStep = (Maf_Fmotor.bStateStep<MAF_STEP8) ? (Maf_Fmotor.bStateStep+MAF_STEP1) : MAF_STEP1; // Near Direction
		else if(Maf_Fmotor.bActive==2) Maf_Fmotor.bStateStep = (Maf_Fmotor.bStateStep>MAF_STEP1) ? (Maf_Fmotor.bStateStep-MAF_STEP1) : MAF_STEP8; // Far  Direction

		if(Maf_Fmotor.bReverseCnt) Maf_Fmotor.bReverseCnt--;
//		else Maf_Fmotor.nCurPos = (Maf_Fmotor.bActive==1) ? (Maf_Fmotor.nCurPos-1) : (Maf_Fmotor.nCurPos+1); // Position_F Count
		else Maf_Fmotor.nCurPos = (Maf_Fmotor.bActive==1) ? ((Maf_Fmotor.nCurPos>0) ? (Maf_Fmotor.nCurPos-1) : Maf_Fmotor.nCurPos) : (Maf_Fmotor.nCurPos+1);
	}
	gnMAF_PTime[3]+=End_HTBTR(gnMAF_PTime[2]);
}

//=================================================================================================
//	Lens Aging Test
//=================================================================================================
#define MAF_AGING_POINT	8
BYTE gbMaf_AgingOrder[MAF_AGING_POINT]={0,3,6,8,10,7,5,2};

void Lens_AgingTest(void)
{
	static BYTE bTestState=0, bTestStateSub=0;

	if(bTestStateSub==0) {
		Maf_Zmotor.nTgtPos = gnMaf_TblPosition_Z[gbMaf_AgingOrder[bTestState]];
		Maf_Fmotor.nTgtPos = gnMaf_TblPosition_F[MP(MpMAF_LensModel)][gbMaf_AgingOrder[bTestState]];
		Maf_Zmotor.bPosGo = Maf_Fmotor.bPosGo = 1;
		bTestStateSub++;
	}
	else {
		if((Maf_Zmotor.bPosGo==0)&&(Maf_Fmotor.bPosGo==0)) {
			bTestStateSub=0;
			bTestState=(bTestState<(MAF_AGING_POINT-1)) ? bTestState+1 : 0;
			gnAgingTest_Cnt++;
		}
	}
}

//=================================================================================================
//	MAF Function Test
//=================================================================================================
void MAF_Test_Mode(void)
{
	// PreSet - Zoom
	static UINT PAR0A_Buf=0,PAR0C_Buf=0,PAR0F_Buf=0;
	
	if(PAR0A_Buf!=PAR0A) {
		Maf_Zmotor.bPosGo = 1;
		Maf_Zmotor.nTgtPos = PAR0B;
	}
	PAR0A_Buf = PAR0A;
	
	// PreSet - Focus
	if(PAR0C_Buf!=PAR0C) {
		Maf_Fmotor.bPosGo = 1;
		Maf_Fmotor.nTgtPos = PAR0D;
	}
	PAR0C_Buf = PAR0C;

	// OneTime AF Enable
	if(PAR0F_Buf!=PAR0F) {
		gbAutoFocus_Flag = 1;
	}
	PAR0F_Buf = PAR0F;

	if(PAR10) gbAF_MainState=PAR10; //Maf_Zmotor.nCurPos = Maf_Fmotor.nCurPos = 0;

	if(PAR1F) gbMaf_AgingTest=1;
	else      gbMaf_AgingTest=0;
}

#endif


/* EOF */


