/* **************************************************************************
 File Name	: 	app_etc.c
 Description:	EN673 - etc. Functions
 Designer	:	Lee, Hoon
 Modifier	:	Kim, Sunghoon, Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"
#include "isp_app_etc_tbl.h"


//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
void IspAgcSet(void)
{
	extern int AE_GAIN_TOTAL;
	extern int AE_GAIN_TOTAL2;
	const int iAeAgcTVal = AE_GAIN_TOTAL + AE_GAIN_TOTAL2;
	
	ADNR_AGC_MIN = (MP(MpShpAgcLow)  * iAeAgcTVal) / 100;
	ADNR_AGC_MID = (MP(MpShpAgcMid)  * iAeAgcTVal) / 100;
	ADNR_AGC_MAX = (MP(MpShpAgcHigh) * iAeAgcTVal) / 100;
	if(ADNR_AGC_MID < ADNR_AGC_MIN) ADNR_AGC_MID = ADNR_AGC_MIN;
	if(ADNR_AGC_MAX < ADNR_AGC_MID) ADNR_AGC_MAX = ADNR_AGC_MID;
}

void ISPM IspAgcCurSet(void)
{
	const BYTE iAgcPos = (giCurAgc<=ADNR_AGC_MIN) ? 0 : (giCurAgc<=ADNR_AGC_MID) ? 1 : (giCurAgc<=ADNR_AGC_MAX) ? 2 : 3;

	gbIspAgcSta = (ADNR_AGC_MID==ADNR_AGC_MIN) || (ADNR_AGC_MID==ADNR_AGC_MAX) || (iAgcPos==0) ? 0 : (iAgcPos-1);

	gbIspAgcEnd = (ADNR_AGC_MAX<=ADNR_AGC_MIN) ? 0 : (ADNR_AGC_MID==ADNR_AGC_MIN) || (ADNR_AGC_MID==ADNR_AGC_MAX) || (iAgcPos==3) ? 2 : iAgcPos;
}

void Adnr_CmdCheck(void) // 180320 LWH
{
	//static BYTE gbAdnr_WaitCnt=3;

	// Exception Condition
#if (model_2M30p)
	#define NOR_MODE	(MP(FrameMode) == MN_FRAMEMODE_NORMAL_25_30)
#else
	#define NOR_MODE	((MP(FrameMode) == MN_FRAMEMODE_NORMAL_25_30) || (MP(FrameMode) == MN_FRAMEMODE_NORMAL_50_60))
#endif

#if (model_Sens == SENS_OV2718)		// OV2718 Parallel 27MHz 동작 시 Sensor 출력의 HLOCK이 흔들림 -> VLOCK이 흔들림 -> DNR 시 간헐적으로 영상이 깨지는 경우 발생 -> HLOCK이 흔들리지 않는 Sensor initial setting이 필요
	const BYTE bMpAdnr = MN_4sOFF;
#else
	const BYTE bMpAdnr = ((MP(MpBackLight) == MN_BL_WDR) || (gbWdrOn==MN_ON) || (gbWdrOnBuf2==MN_ON) || MN_FPS_HI || NOR_MODE) ? MN_4sOFF : MP(MpAdnr) ;
#endif

	if(bMpAdnr == MN_4sOFF) gbAdnr_WaitCnt=3; // Priority : WDR > DNR
	else if(gbAdnr_WaitCnt) gbAdnr_WaitCnt--; // wait for stable image

	gbAdnr_Excute = (gbAdnr_WaitCnt==0);
}

void ISPM Adnr(void) // 180320 LWH
{	// Digital noise reduction (3D Only)
	static BYTE bIspResetCnt = 0;
	if(bIspResetCnt) { bIspResetCnt--; return; }

	Adnr_CmdCheck();
	
	if(gbAdnr_Excute) {

		if(giCurDssStg > 1) return;
		else if(giCurDssStg==1) {
			giCurDssStg = 0;
			isp_reset();
			bIspResetCnt = 1/*PAR00*/;	// isp_reset 후 최소 2frame 후에 DNR3D_ONw(1) 해야 함
			return;
		}
	
		int iGain=0,iFk=0,iTh=0;
		const BYTE bTblSel_0 = (MP(MpAdnr) == MN_4sHI) ? 2 : (MP(MpAdnr) == MN_4sMID) ? 1 : (MP(MpAdnr) == MN_4sLOW) ? 0 : UP(UpMpAdnr) ;
		
		if((gbIspAgcSta < gbIspAgcEnd) && (MP(MpDnrIncrease) == MN_ON)) {
			iGain = LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DGainTbl[bTblSel_0][gbIspAgcSta], Adnr3DGainTbl[bTblSel_0][gbIspAgcEnd]);
			iTh   = LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DThrsTbl[bTblSel_0][gbIspAgcSta], Adnr3DThrsTbl[bTblSel_0][gbIspAgcEnd]);
			iFk   = LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Adnr3DFkTbl  [bTblSel_0][gbIspAgcSta], Adnr3DFkTbl  [bTblSel_0][gbIspAgcEnd]);
		}
		else {
			const BYTE bTblSel_1 = (MP(MpDnrIncrease) == MN_ON) ? gbIspAgcSta : 0 ;
			iGain = Adnr3DGainTbl[bTblSel_0][bTblSel_1];
			iTh   = Adnr3DThrsTbl[bTblSel_0][bTblSel_1];
			iFk   = Adnr3DFkTbl  [bTblSel_0][bTblSel_1];
		}
		
		// Adding user parameter gain weight
		const BYTE b3D_DNR_GAIN = (MP(MpAdnr) == MN_4sLOW) ? UP(Up3D_DNR_GAIN_LOW)    :
		                          (MP(MpAdnr) == MN_4sMID) ? UP(Up3D_DNR_GAIN_MIDDLE) :
								  (MP(MpAdnr) == MN_4sHI ) ? UP(Up3D_DNR_GAIN_HIGH)   : 0 ;

		iGain = MIN((iGain * b3D_DNR_GAIN)>>7, 0x3F);
		iTh   = MIN((iTh   * b3D_DNR_GAIN)>>7, 0xFF);
		iFk   = MIN((iFk   * b3D_DNR_GAIN)>>7, 0xFF);
		
		// Post Processing
		DNR3D_ONw(1);
		DDR_RD_MODw(1); // DDR set to DNR Mode
		
		DNR3D_GAw((UINT)iGain);		// 0x3F
		DNR3D_THw((UINT)iTh  );		// 0xFF
		DNR3D_FKw((UINT)iFk  );		// 0xFF
	}
	else DNR3D_ONw(0);
}

void ISPM Sharpness(void)
{	// Aperture

	int i55Th, i55Gain, i33Th, i33Gain;

	const int i55Gain_S = (gbIspAgcSta==0) ? MP(MpShpBigEdge) : (gbIspAgcSta==1) ? (MP(MpShpBigEdge)>>1) : 0;
	const int i55Gain_E = (gbIspAgcEnd==0) ? MP(MpShpBigEdge) : (gbIspAgcEnd==1) ? (MP(MpShpBigEdge)>>1) : 0;

	const int i33Gain_S = (gbIspAgcSta==0) ? MP(MpShpSmallEdge) : 0;
	const int i33Gain_E = (gbIspAgcEnd==0) ? MP(MpShpSmallEdge) : 0;

	APT_ONw(1);
	APT_GAIN_ONw(1);

	if(gbIspAgcSta < gbIspAgcEnd) {
		i55Th 	= LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Apt55ThTbl[gbIspAgcSta], Apt55ThTbl[gbIspAgcEnd]);
		i55Gain = LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, i55Gain_S, i55Gain_E);
		i33Th 	= LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, Apt33ThTbl[gbIspAgcSta], Apt33ThTbl[gbIspAgcEnd]);
		i33Gain = LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, i33Gain_S, i33Gain_E);
	}
	else {
		i55Th  	= Apt55ThTbl[gbIspAgcSta];
		i55Gain	= i55Gain_S;
		i33Th	= Apt33ThTbl[gbIspAgcSta];
		i33Gain	= i33Gain_S;
	}

	// APT 5x5 Filter control
	i55Gain = MIN((i55Gain * MP(MpSharpness) / UP(UpSHARPNESS_GAIN)), 0xff);
	
	APT_GAIN1_POSw(i55Gain);
	APT_GAIN2_POSw(i55Gain);
	APT_GAIN1_NEGw(i55Gain);
	APT_GAIN2_NEGw(i55Gain);
	APT_TH1w(i55Th);
	APT_TH3w(0x80);
	
	// APT 3x3 Filter control
	i33Gain = MIN((i33Gain * MP(MpSharpness) / UP(UpSHARPNESS_GAIN)), 0xff);
	
	APT_GAIN3w(i33Gain);
	APT_SLIC3w(i33Th);
	APT_CLIP3w(0x80);
}

//-------------------------------------------------------------------------------------------------
void ISPM CSup(void)
{
	// Low light color suppression ----------------------------
	int iGain = (gbIspAgcSta < gbIspAgcEnd) ? LibUtlInterp1D (giCurAgc, ADNR_AGC_STA, ADNR_AGC_END, LowCSupGaTbl[gbIspAgcSta], LowCSupGaTbl[gbIspAgcEnd]) : LowCSupGaTbl[gbIspAgcSta];
	iGain = MIN((iGain * UP(UpLSUP_GA))>>7, 0x3F);	// 15.06.13 KSH

	LSUP_ONw(!(!UP(UpLSUP_ON)||gbWdrOn));		// 141209 (gbWdrOn&&UpLSUP_ON) ? 0: 1	//HTY 20150613
	LSUP_GAw(iGain);
	LSUP_THw(UP(UpLSUP_TH));						//HTY 20150613

	// High light color suppression ----------------------------
	CSUP_ONw(UP(UpCSUP_ON));						//HTY 20150613
	iGain = MIN((CSUP_GA * UP(UpCSUP_GA))>>7, 0x3F);// 15.06.13 KSH
	CSUP_GAw(iGain);								// 15.06.13 KSH
//	CSUP_THw(UP(UpCSUP_TH));

#if 0
	static BYTE bMpCES_ON_Buf=0xEE, bWdrChkBuf=0xEE;

	if((bWdrChkBuf!=gbWdrOn)||(bMpCES_ON_Buf!=MP(MpCES_ON))) {
		CSUP_THw((gbWdrOn==1) ? UP(UpCSUP_WTH) : UP(UpCSUP_TH));	//WHL 20180807

		// Edge color suppression -------------------------------
		CES_ONw((gbWdrOn==1) ? MN_ON : MP(MpCES_ON));				//WHL 20180807
		CES_GAINw(CesGainTbl[gbWdrOn]);
		CES_LCLIPw(CesLClipTbl[gbWdrOn]);
	}
	bWdrChkBuf=gbWdrOn;
	bMpCES_ON_Buf=MP(MpCES_ON);
#else
	CSUP_THw((gbWdrOn==1) ? UP(UpCSUP_WTH) : UP(UpCSUP_TH));	//WHL 20180807
	
	// Edge color suppression -------------------------------
	const BYTE bMpCES_ON = (gbWdrOn==1) ? MP(MpWdrCES_ON) : MP(MpCES_ON);
	const BYTE bMpCES_ONrv = (40 < bMpCES_ON) ? 40 : 40 - bMpCES_ON;
	const BYTE bCES_LCLIP = (bMpCES_ONrv > 20) ? bMpCES_ONrv + (((bMpCES_ONrv-19)*(bMpCES_ONrv-20))>>1) : bMpCES_ONrv;

	CES_ONw(bMpCES_ON>0);				//WHL 20180807
	CES_GAINw((gbWdrOn==1) ? UP(UpCES_WGAIN) : UP(UpCES_GAIN));
	CES_LCLIPw((gbWdrOn==1) ? (bCES_LCLIP>>1)/*UP(UpCES_WLCLP)*/ : bCES_LCLIP/*UP(UpCES_LCLP)*/);
		
#endif	
}

//-------------------------------------------------------------------------------------------------
void ISPM HlMask(void)
{	// High Light Mask

	HLMASK_ONw((MP(MpBackLight)==MN_BL_HLC));
	HLMASK_THw(54+(MP(MpHlMaskThrs)*UP(UpHLMASK_THRS_STEP)));

	if(MP(MpHlMaskColor)<8) {	// Fixed Menu Color Table

		HLMASK_YLVw((UINT)gbPrvcYCbCrTbl[MP(MpHlMaskColor)][0]);
		HLMASK_BLVw((UINT)gbPrvcYCbCrTbl[MP(MpHlMaskColor)][1]);
		HLMASK_RLVw((UINT)gbPrvcYCbCrTbl[MP(MpHlMaskColor)][2]);
	}
	else {	// CUSTOMIZE

		HLMASK_YLVw((UINT)UP(UpHLMASK_CY));
		HLMASK_BLVw((UINT)UP(UpHLMASK_CB));
		HLMASK_RLVw((UINT)UP(UpHLMASK_CR));
	}
}

//-------------------------------------------------------------------------------------------------
void ISPM Mirror(void)
{	// Sensor Mirror
	static BYTE bMirrBuf=0xEE;

	if(bMirrBuf!=MP(MpMirror)) {
		MIRR_ONw(MP(MpMirror))
	}
	bMirrBuf=MP(MpMirror);
}

//-------------------------------------------------------------------------------------------------
void ISPM Flip(void)
{	// Sensor Flip
	static BYTE bFlipBuf=0xEE;

	if(bFlipBuf!=MP(MpFlip)) {
		FLP_ONw(MP(MpFlip))
	}
	bFlipBuf=MP(MpFlip);
}

//-------------------------------------------------------------------------------------------------
void ISPM ColorBar(void)
{	// Color bar display

	DCBAR_ONw(MP(MpCBar));	// Digital Color bar On/Off
}

#define QR_Det_PosX		20
#define QR_Det_PosY		6
#define QR_Det_SizX		23
#define QR_Det_SizY		23
#define QR_Det_BThr		10
#define QR_Det_FDly		120	// Detection Time
//-------------------------------------------------------------------------------------------------
void ISPM QR_DetSet(void)
{ // QR Code Detection Area Set

	UINT i=0;
	static BYTE bDetOnCnt=0, bDetOnCntState=0;

	bDetOnCnt = (PAR1A) ? QR_Det_FDly : bDetOnCnt;

	bDetOnCntState = (bDetOnCnt==QR_Det_FDly) ? ((bDetOnCntState<<1)|0x01) :
		                                        ((bDetOnCntState<<1)&0xFE) ;

	if(bDetOnCnt) {
		
		if(bDetOnCntState==0x01) {	// Load origin Prv Setting
		}
		else if(bDetOnCnt==1) {			// Recovery origin Prv Setting
		}
		else {							// QR Det Mode Enable
			MP(MpPrivacy) = 1; // Enable
			
			for(i=0; i<(PVC_EA-1); i++) {
				if(i==0) gbMnPvcCfg(i)->bAction = 1;
				else     gbMnPvcCfg(i)->bAction = 0;
			}
			gbMnPvcCfg(0)->bPosX = QR_Det_PosX;
			gbMnPvcCfg(0)->bPosY = QR_Det_PosY;
			gbMnPvcCfg(0)->bSizX = QR_Det_SizX;
			gbMnPvcCfg(0)->bSizY = QR_Det_SizY;
			
			MP(MpPvcFormat) = 1;
			MP(MpPvcYLevel) = QR_Det_BThr;
			MP(MpPvcTrans)  = 0;
		}		
		bDetOnCnt--;
	}
	else MP(MpPrivacy) = 0; // Disable
}

//-------------------------------------------------------------------------------------------------
void ISPM ColorSpace(void)
{ // Color space setup

	switch(MP(MpOutFromat)) {
		case MN_FRMT_UV :	RMYRw(0x276);RMYGw(0x20F);RMYBw(0x66);BMYRw(0x97);BMYGw(0x128);BMYBw(0x1BE); break;
		case MN_FRMT_SD :	RMYRw(0x20B);RMYGw(0x1B6);RMYBw(0x55);BMYRw(0xB0);BMYGw(0x15B);BMYBw(0x20B); break;
		case MN_FRMT_HD :	RMYRw(0x20B);RMYGw(0x1DB);RMYBw(0x30);BMYRw(0x78);BMYGw(0x193);BMYBw(0x20B); break;
	}
}

//-------------------------------------------------------------------------------------------------
void UartBaudRate(void)
{ // UART Baudrate
	//TxStrDecNoIRQ("UART Baudrate ", MP(MpBaudrate), 3);

	return;

	switch(MP(MpBaudrate)){
//			case MN_BR_2400		: UART0_CLKDIV = UB2400;	break;
		case MN_BR_4800		: UART0_CLKDIV = UB4800;		break;
		case MN_BR_9600		: UART0_CLKDIV = UB9600;		break;
		case MN_BR_57600	: UART0_CLKDIV = UB57600;	break;
		case MN_BR_115200	: UART0_CLKDIV = UB115200;	break;
		default				: UART0_CLKDIV = UB_INIT;	break;
	}
}

//-------------------------------------------------------------------------------------------------
UINT gnBoxPosPVC[PVC_EA*2];
UINT gnBoxPltPVC[PVC_EA];
UINT gnBoxTonePVC[2];
UINT gnBoxFillPVC;
UINT gnBoxOnPVC;
UINT gnBoxFmtPVC;
UINT gnBBoxPltPVC;

void ISPM PrivacyBox(void)
{	// Box Privacy 32ea

	UINT i;
	UINT nPRVC_CY, nPRVC_CB, nPRVC_CR;
	gnBoxTonePVC[0] = 0, gnBoxTonePVC[1] = 0;

	int iPosX,iPosY,iSizX,iSizY;

	const UINT nPixelStep = MINMAX(UP(UpBOX_MV_PXL),BOX_MVPXL_MIN,BOX_MVPXL_MAX);		//141126 LGH
	const BYTE nColorStep = MINMAX(UP(UpBOX_COLOR_STEP),0x01,0xFF);					//141126 LGH


	gnBoxOnPVC = 0;		// init	14.2.20
	gnBoxFmtPVC = 0;

	if (MP(MpPrivacy)==MN_ON) {

		nPRVC_CY = MP(MpPvcYLevel)  * 255 / nColorStep;
		nPRVC_CB = MP(MpPvcCBLevel) * 255 / nColorStep;
		nPRVC_CR = MP(MpPvcCRLevel) * 255 / nColorStep;

		if(MP(MpPvcFormat)==1) {	// 2015112 - WHL : Add Bit Box Mode
			nPRVC_CY = nPRVC_CY<<2;
			nPRVC_CB = nPRVC_CB<<2;
			nPRVC_CR = nPRVC_CR<<2;
		}
		
		#define	PRVC_POS_H	(gbMnPvcCfg(i)->bPosX)
		#define	PRVC_POS_V	(gbMnPvcCfg(i)->bPosY)
		#define	PRVC_SIZ_H	(gbMnPvcCfg(i)->bSizX)
		#define	PRVC_SIZ_V	(gbMnPvcCfg(i)->bSizY)

	#if 0
		#if (model_1M)
			#define VWOr	(DFSEL_Sens(0,	0,	0,	0x50802d8   ,	0,	0,	0,	0,	0,	0)&0x7FF)&0x7FF)	// 107
		#elif (model_2M30p)
			#define VWOr	(DFSEL_Sens(0x7880442    ,	0,	0,	0x7880442    ,	0,	0,	0x7880442     ,	0,	0,	0)&0x7FF)	// 107
		#endif
	#endif

		for (i=0; i<PVC_EA; i++) {

			iPosX = PRVC_POS_H*nPixelStep;
			iSizX = PRVC_SIZ_H*nPixelStep;

			iPosY = PRVC_POS_V*nPixelStep;
			iSizY = PRVC_SIZ_V*nPixelStep;

			gnBoxPosPVC[(i*2)] 	 = (MAX((iPosY+iSizY), 0)) | (MAX(iPosY, 0)<<16);	// y pos
			gnBoxPosPVC[(i*2)+1] = (iPosX+iSizX) | (iPosX<<16);											// x pos

			gnBoxPltPVC[i]		 = (nPRVC_CY<<16)|(nPRVC_CB<<8)|(nPRVC_CR);																	// box attr

			if (i<16)	gnBoxTonePVC[0] |= MP(MpPvcTrans)<<(i*2);
			else		gnBoxTonePVC[1] |= MP(MpPvcTrans)<<((i-16)*2);

			gnBoxFillPVC |= 0x1<<i;

			gnBoxOnPVC |= ((gbMnPvcCfg(i)->bAction)&0x1)<<i;

			if(MP(MpPvcFormat)==1) {	// 2015112 - WHL : Add Bit Box Mode
				gnBoxFmtPVC |= ((gbMnPvcCfg(i)->bAction)&0x1)<<i;
				gnBBoxPltPVC = (nPRVC_CB<<20)|(nPRVC_CR<<10)|(nPRVC_CY);
			}
		}
	}	
}

//-------------------------------------------------------------------------------------------------
void ISPM BoxLast(void)
{	// Last Box apply (priority, sharing control)

// LGH 20150410
#if (model_1M) 							
	BOX_HSPw(0x127);
	BOX_VSPw(0x22);

#else				//2M
		if((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60)){		//MD, Privacy Box Start Position
			BOX_HSPw(0x4F);	// 20161111 - WHL
			BOX_VSPw(0x1a);
		}
		else{
			#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)) // BT656 or BT1120 Input Setting
				BOX_HSPw(0x00);
				BOX_VSPw(0x00);
			#else
				BOX_HSPw(0x4F);	// 20161111 - WHL
				BOX_VSPw(0x09);
			#endif
		}

#endif

#if 1
	const BYTE DefogAutoOn = (MP(MpDefog) == MN_ON) && (MP(MpDefogMode) == MN_DEFOG_AUTO);
//	const BYTE HistoOn = (MP(MpPrivacy) == MN_OFF) && (MP(MpItl) == MN_OFF) && (gbMnFcsAdjOn == 0) /*&& STATUS_ON*/;
	//const BYTE HistoOn = (MP(MpPrivacy) == MN_OFF) && (MP(MpItl) == MN_OFF) && (gbMnFcsAdjOn == 0) && (MP(MpShdDet) == 0) && (gbMnDebugFnc == 8) /*&& STATUS_ON*/;	// 2015818 - WHL
	const BYTE HistoOn = (MP(MpPrivacy) == MN_OFF) && (MP(MpItl) == MN_OFF) && (MP(MpShdDet) == 0) && (DefogAutoOn == 0) && (gbMnDebugFnc == 1);	// 180516 KSH
	static BYTE HistoOn1d = 0;
	if(HistoOn1d != HistoOn) {
		HistoOn1d = HistoOn;
		if(HistoOn) {
			InitHisto();
		}
	}
	if(HistoOn) {
		Histo();
		return;
	}
#endif

	//----------------------------------------------------------------------------------------
	int i;

	#if (model_Lens==0)
		if(MP(MpFad_On)) {
			for(i=0; i<FADJ_EA; i++) { // Focus Assist
				SetIsp(BOXPOS_BASE+(i*2)  , gnBoxPosFADJ[(i*2)]);			// box y
				SetIsp(BOXPOS_BASE+(i*2+1), gnBoxPosFADJ[(i*2)+1]);		// box x
				SetIsp(BOXPLT_BASE+i      , gnBoxPltFADJ[i]);				// box attr
			}

			SetIsp(BOXTON_BASE  , gnBoxToneFADJ[0]);						// box tone msb
			SetIsp(BOXTON_BASE+1, gnBoxToneFADJ[1]);						// box tone lsb
			SetIsp(BOXFIL_BASE  , gnBox_BFL_FADJ);							// box fill tone

			BOSD_ONw(0x7ffff); // USE 19 Box
		}
		else {
	#endif
		for (i=0; i<ISP_BOX_EA; i++) {

			if (i<16) {			// privacy
				SetIsp(BOXPOS_BASE+(i*2)  , gnBoxPosPVC[(i*2)]);			// box y
				SetIsp(BOXPOS_BASE+(i*2+1), gnBoxPosPVC[(i*2)+1]);			// box x
				SetIsp(BOXPLT_BASE+i      , gnBoxPltPVC[i]);				// box attr
			}
#if model_WDR_ROI
			else if((gbROIWdrBoxFlag)&&(i==AE_ROI_WDR_BOX_NO)){				
				SetIsp(BOXPOS_BASE+(i*2)  , gnBoxROIWdr[0]);				// box y
				SetIsp(BOXPOS_BASE+(i*2+1), gnBoxROIWdr[1]);				// box x
				SetIsp(BOXPLT_BASE+i      , MASK_SEL_COLOR);				// box attr
			}
#endif
#if (model_Lens==1)
			else if((gbMAFBoxFlag)&&(i>=MAF_BOX_START)){
				SetIsp(BOXPOS_BASE+(i*2)  , gnBoxPosMAF_Y[(i-MAF_BOX_START)]);	// box y
				SetIsp(BOXPOS_BASE+(i*2+1), gnBoxPosMAF_X[(i-MAF_BOX_START)]);	// box x
				SetIsp(BOXPLT_BASE+i      , gnBoxPltMAF[(i-MAF_BOX_START)]);	// box attr
			}
#endif
			else {						// IMD
				SetIsp(BOXPOS_BASE+(i*2)  , gnBoxPosIMD[((i-16)*2)]);		// box y
				SetIsp(BOXPOS_BASE+(i*2+1), gnBoxPosIMD[((i-16)*2)+1]);		// box x
				SetIsp(BOXPLT_BASE+i      , gnBoxPltIMD[(i-16)]);			// box attr
			}
		}


#if (model_Lens==1)
		SetIsp(BOXTON_BASE  , (gnBoxToneIMD[0]|gnBoxToneMAF)&(~(UINT)(gbBoxToneROIWdr<<((AE_ROI_WDR_BOX_NO-16)*2))));		// box tone msb
		SetIsp(BOXTON_BASE+1, gnBoxTonePVC[0]);								// box tone lsb
	//	SetIsp(BOXFIL_BASE  , ((gnBoxFillIMD<<16)|gnBoxFillPVC|gnBoxFillMAF)&(~(UINT)(gbBoxFillROIWdr<<AE_ROI_WDR_BOX_NO)));	// box fill tone
		
		BOSD_ONw(gnBoxOnIMD<<16|gnBoxOnPVC|(gbBoxOnROIWdr<<AE_ROI_WDR_BOX_NO)|gnBoxOnMAF);	// box on/off
#else
		SetIsp(BOXTON_BASE  , gnBoxToneIMD[0]&(~(UINT)(gbBoxToneROIWdr<<((AE_ROI_WDR_BOX_NO-16)*2))));		// box tone msb
		SetIsp(BOXTON_BASE+1, gnBoxTonePVC[0]);								// box tone lsb
	//	SetIsp(BOXFIL_BASE  , ((gnBoxFillIMD<<16)|gnBoxFillPVC)&(~(UINT)(gbBoxFillROIWdr<<AE_ROI_WDR_BOX_NO)));	// box fill tone

		BOSD_ONw(gnBoxOnIMD<<16|gnBoxOnPVC|(gbBoxOnROIWdr<<AE_ROI_WDR_BOX_NO));	// box on/off
#endif
		// 2015112 - WHL : Add Bit Box Mode
		SetIsp(BOXFMT_BASE  , gnBoxFmtPVC);									// box format
		SetIsp(BBOXFMT_BASE , gnBBoxPltPVC);								// bit box fill format

#if (model_Lens==0)
		}
#endif
}

void ISPM WdrWgtSet(void)
{
	 MP(MpWdrWgt) = (MP(MpFrameRateLv)==MN_3sLOW) ? MN_3sLOW :
					(MP(MpFrameRateLv)==MN_3sMID) ? MN_3sMID :
													MN_3sHI  ;
}

void ISPM AdnrSet(void)
{
	 MP(MpAdnr)	=	(MP(MpFrameRateLv)==MN_3sLOW) ? MN_4sLOW :
					(MP(MpFrameRateLv)==MN_3sMID) ? MN_4sMID :
													MN_4sHI  ;
}

void ISPM FrameModeSet(void)
{
	static BYTE bFrameModeBuf=0xEE, bFrameRateLvBuf=0xEE, bBackLightNorBuf=0xEE;
	const BYTE bActBuf = ((bFrameModeBuf!=MP(FrameMode))||(bFrameRateLvBuf!=MP(MpFrameRateLv))||(bBackLightNorBuf!=MP(MpBackLightNor)));

	bFrameModeBuf    = MP(FrameMode);
	bFrameRateLvBuf  = MP(MpFrameRateLv);
	bBackLightNorBuf = MP(MpBackLightNor);

	if(!bActBuf) return;	// No Action!!

	switch(MP(FrameMode)) {
		case MN_FRAMEMODE_NORMAL_25_30 :
			MP(MpOutFps)	= MN_2M_1080p_25_30;
			MP(MpBackLight) = MP(MpBackLightNor);
			break;
		case MN_FRAMEMODE_WDR_12_15 :
			MP(MpOutFps)	= MN_2M_1080p_25_30;
			MP(MpBackLight) = MN_BL_WDR;
			WdrWgtSet();
			break;
		case MN_FRAMEMODE_DNR_25_30 :
			MP(MpOutFps)	= MN_2M_1080p_25_30;
			MP(MpBackLight) = MP(MpBackLightNor);
			AdnrSet();
			break;
#if (model_1M|model_2M)
		case MN_FRAMEMODE_NORMAL_50_60 :
			MP(MpOutFps)	= MN_2M_1080p_50_60;
			MP(MpBackLight) = MP(MpBackLightNor);
			break;
		case MN_FRAMEMODE_WDR_25_30 :
			MP(MpOutFps)	= MN_2M_1080p_50_60;
			MP(MpBackLight) = MN_BL_WDR;
			WdrWgtSet();
			break;
		case MN_FRAMEMODE_DNR_50_60 :
			MP(MpOutFps)	= MN_2M_1080p_50_60;
			MP(MpBackLight) = MP(MpBackLightNor);
			AdnrSet();
			break;
#endif
	}

	DebugDisp((gbMnDebugFnc==8), Dec, "FrmMode:", model_1M? 10:28, 24, MP(FrameMode));
	DebugDisp((gbMnDebugFnc==8), Dec, "OutFps :", model_1M? 11:29, 24, MP(MpOutFps));
	DebugDisp((gbMnDebugFnc==8), Dec, "BackLgt:", model_1M? 12:30, 24, MP(MpBackLight));
	DebugDisp((gbMnDebugFnc==8), Dec, "MpWdrWgt:", model_1M? 13:31, 24, MP(MpWdrWgt));
	DebugDisp((gbMnDebugFnc==8), Dec, "MpAdnr  :", model_1M? 14:32, 24, MP(MpAdnr));
}

void ISPM FrameRateLvSet(void)
{
	MP(MpFrameRateLv) = WDR_MODE ? MP(MpWdrWgt) :
						DNR_MODE ? MP(MpAdnr)>0 ? MP(MpAdnr)-1 : 0 :
								   MP(MpFrameRateLv) ;

	FrameModeSet();
}

#if (DOOR_BELL_CAMERA==1)

#define LPMODE_FPS ((model_1M|model_2M) ? 60 : 30)

#define LPMODE_10S	10
#define LPMODE_15S	15
#define LPMODE_20S	20
#define LPMODE_25S	25
#define LPMODE_30S	30

void DoorBell_PowerCtr(void)
{
	// Test for Main Power Shut-Down
	static UINT nLPModeCnt=0;
	static BYTE bDoorCam_LP_ModeBuf=0xEE;
	
	UINT nLPModeFCnt = ((MP(MpDoorCam_LP_Mode)==1) ? (LPMODE_FPS*LPMODE_10S) :
						(MP(MpDoorCam_LP_Mode)==2) ? (LPMODE_FPS*LPMODE_15S) :
						(MP(MpDoorCam_LP_Mode)==3) ? (LPMODE_FPS*LPMODE_20S) :
						(MP(MpDoorCam_LP_Mode)==4) ? (LPMODE_FPS*LPMODE_25S) :
						(MP(MpDoorCam_LP_Mode)==5) ? (LPMODE_FPS*LPMODE_30S) : (LPMODE_FPS*LPMODE_10S));

	if(bDoorCam_LP_ModeBuf!=MP(MpDoorCam_LP_Mode)) { // Time Set Changed
		nLPModeCnt=0;
		bDoorCam_LP_ModeBuf=MP(MpDoorCam_LP_Mode);
	}

	if(MP(MpDoorCam_LP_Mode)) {
		if(!GpioGetPin(GPIO_PIR_AWAKE)||!GpioGetPin(GPIO_DOOR_BELL)) nLPModeCnt=0;
		else {
			if(nLPModeCnt==nLPModeFCnt) {
				GpioSetLo(GPIO_MAIN_PW_HLD);
				GpioSetHi(GPIO_MAIN_PW_CLS);
			}
			else nLPModeCnt++;
		}
	}
	else nLPModeCnt=0;

	DebugDisp((gbMnDebugFnc==8), Dec, "LPMdCnt:", 15, 24, nLPModeCnt);
	DebugDisp((gbMnDebugFnc==8), Dec, "PIR_In :", 16, 24, GpioGetPin(GPIO_PIR_AWAKE));
	DebugDisp((gbMnDebugFnc==8), Dec, "Bell_In:", 17, 24, GpioGetPin(GPIO_DOOR_BELL));
}	
#endif

void ISPM MsgMenuSet(void)
{
// 180319 KDH call menu function via Msg (cgi/onvif to isp)
	extern BYTE gbSetFontChg, gbMenuSave, gbMenuReset, gbOsdCamTitle, gbmenu_redraw, gbWebIris, gbmenu_style, gbMenuStyle;

	if(gbSetFontChg) { SetFontChg(); gbSetFontChg=0; } 
	if(gbMenuSave)	 { MenuSave(MN_ON); gbMenuSave=0; }
	if(gbMenuReset)  { MenuReset(); gbMenuReset=0; }
	if(gbOsdCamTitle){ OsdCamTitle(); gbOsdCamTitle=0; }
	if(gbmenu_redraw){ menu_redraw(1,0); gbmenu_redraw=0; }
	if(gbWebIris)	 { ShutterMenuGet(); gbWebIris=0; }
	if(gbmenu_style) { gbMenuStyle=gptMsgISPTbl.MENU_STYLE; gptMsgISPTbl.MENU_STYLE=0; MenuStyleSet(gbMenuStyle); gbMenuStyle=0; gbmenu_style=0; }

}

//-------------------------------------------------------------------------------------------------

