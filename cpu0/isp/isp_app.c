/* **************************************************************************
 File Name	: 	app.c
 Description:	EN673 - common application
 Designer	:	Lee, Hoon
 Date		:	14. 5. 9
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"

//*************************************************************************************************
// Variables 
//-------------------------------------------------------------------------------------------------
const char *gsBuildDate = "FW Date : "__DATE__" "__TIME__;
WORD gDataID = 0;

// User Parameter --------------------------------------------------
#if model_Par_Save
BYTE gbUsrParSaveChk=0;
BYTE gbUsrParReadChk=0;
#endif

BYTE gbUsrParTbl[USR_PAR_EA];

// User Data -------------------------------------------------------
BYTE gbUsrDataSaveChk=0;
BYTE gbUsrDataReadChk=0;

BYTE gbUsrDataTbl[USR_DATA_EA];

extern void Hue_Chroma_Menu_Set(int a);



//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
void InitDataID(void)
{
	const UINT gsBuildDateLen = strlen(gsBuildDate);
	gDataID  = (*(gsBuildDate + gsBuildDateLen-1) - '0');
	gDataID += (*(gsBuildDate + gsBuildDateLen-2) - '0')<<4;
	gDataID += (*(gsBuildDate + gsBuildDateLen-4) - '0')<<8;
	gDataID += (*(gsBuildDate + gsBuildDateLen-5) - '0')<<12;
}

void UsrParSet(const int aiAddr, const int aiInit)
{
#if (model_Par==0)
	UNUSED_ARG(aiAddr);
	UNUSED_ARG(aiInit);
#else
	switch(aiAddr) {

	#if 0
		#define RUNw(...)

		#define RUNwRUN(N,F)	case UP_ADDR(N) : F(UP(N)); break;

		#define ISPw(N,F)	RUNw(F)

		#undef UP_SET
		#define UP_SET(S,N,...)		ISPw(N,__VA_ARGS__)
		#include "isp_app_para_tbl.h"

		//UP_SET(S,N)
		//UP_SET(S,N,RUN(F))

		USR_PAR_LIST
	#else
		// Interface
	  	case UP_ADDR(UpINSEL) 	: INSELw(UP(UpINSEL));			break;	// 0x0026 - Common
		case UP_ADDR(UpONSEL) 	: ONSELw(UP(UpONSEL));			break;	// 0x00b6 - Sensor
		case UP_ADDR(UpTRG_DIN) : TRG_DINw(UP(UpTRG_DIN));		break;	// 0x0026 - Common
		case UP_ADDR(UpTRG_HSI) : TRG_HSIw(UP(UpTRG_HSI));		break;	// 0x0026 - Common
		case UP_ADDR(UpTRG_VSI) : TRG_VSIw(UP(UpTRG_VSI));		break;	// 0x0026 - Common
		case UP_ADDR(UpPOL_HSI) : POL_HSIw(UP(UpPOL_HSI));		break;	// 0x0026 - Common
		case UP_ADDR(UpPOL_VSI) : POL_VSIw(UP(UpPOL_VSI));		break;	// 0x0026 - Common
		case UP_ADDR(UpLDI_POL) : LDI_POLw(UP(UpLDI_POL));		break;	// 0x0015 - Common & (Delay)
		case UP_ADDR(UpLCK_DLY) : LCK_DLYw(UP(UpLCK_DLY));		break;	// 0x0012 - Delay
		case UP_ADDR(UpLDIDLY0) : LDIDLY0w(UP(UpLDIDLY0));		break;	// 0x0015 - Common & Delay
		case UP_ADDR(UpLDIDLY1) : LDIDLY1w(UP(UpLDIDLY1));		break;	// 0x0015 - Common & Delay
		case UP_ADDR(UpLDIDLY2) : LDIDLY2w(UP(UpLDIDLY2));		break;	// 0x0015 - Common & Delay
		case UP_ADDR(UpLDIDLY3) : LDIDLY3w(UP(UpLDIDLY3));		break;	// 0x0015 - Common & Delay

		case UP_ADDR(UpDCKO0_INV) : DCKO0_INVw(UP(UpDCKO0_INV));	break;	// 0x0011 - Common
		case UP_ADDR(UpDCKO0_DLY) : DCKO0_DLYw(UP(UpDCKO0_DLY));	break;	// 0x0012 - (Delay)

		case UP_ADDR(UpLDO0_SEL) : LDO0_SELw(UP(UpLDO0_SEL));  break;
		case UP_ADDR(UpLDO1_SEL) : LDO1_SELw(UP(UpLDO1_SEL));  break;
		case UP_ADDR(UpLDO2_SEL) : LDO2_SELw(UP(UpLDO2_SEL));  break;
		case UP_ADDR(UpLDO3_SEL) : LDO3_SELw(UP(UpLDO3_SEL));  break;

		// AE,AWB
		case UP_ADDR(UpAE_WDR_MOTION_SEL) : WDR_MOTION_ONw(UP(UpAE_WDR_MOTION_SEL)); break;

		//case UP_ADDR(UpAWB_YSLICE) : /*AWBL_CYSLICEw(UP(UpAWB_YSLICE));*/if(aiInit == 0) { menu_redraw(1,0); } break;

		case UP_ADDR(UpAWB_YCLIP) : AWBL_CYCLIPw(UP(UpAWB_YCLIP)); break;

		case UP_ADDR(UpAWB_DBOFFSET) : AWBL0_DBOFFSETw(UP(UpAWB_DBOFFSET)); break;
		case UP_ADDR(UpAWB_DROFFSET) : AWBL0_DROFFSETw(UP(UpAWB_DROFFSET)); break;

		//case UP_ADDR(UpBY_Gain_Negative) : BYGAINNw(UP(UpBY_Gain_Negative)); break;
		//case UP_ADDR(UpBY_Gain_Positive) : BYGAINPw(UP(UpBY_Gain_Positive)); break;
		//case UP_ADDR(UpRY_Gain_Negative) : RYGAINNw(UP(UpRY_Gain_Negative)); break;
		//case UP_ADDR(UpRY_Gain_Positive) : RYGAINPw(UP(UpRY_Gain_Positive)); break;
//	
//			case UP_ADDR(UpBY_Hue_Negative) : BYHUENw(UP(UpBY_Hue_Negative)+0x80); break;
//			case UP_ADDR(UpBY_Hue_Positive) : BYHUEPw(UP(UpBY_Hue_Positive)+0x80); break;
//			case UP_ADDR(UpRY_Hue_Negative) : RYHUENw(UP(UpRY_Hue_Negative)+0x80); break;
//			case UP_ADDR(UpRY_Hue_Positive) : RYHUEPw(UP(UpRY_Hue_Positive)+0x80); break;

		//case UP_ADDR(UpBY_Hue_Negative) : BYHUENw(UP(UpBY_Hue_Negative)); break;
		//case UP_ADDR(UpBY_Hue_Positive) : BYHUEPw(UP(UpBY_Hue_Positive)); break;
		//case UP_ADDR(UpRY_Hue_Negative) : RYHUENw(UP(UpRY_Hue_Negative)); break;
		//case UP_ADDR(UpRY_Hue_Positive) : RYHUEPw(UP(UpRY_Hue_Positive)); break;

		//Defog Area Set
		case UP_ADDR(UpDEFOG_OSDON) : HEQ_OSDw(UP(UpDEFOG_OSDON));	break;
		//case UP_ADDR(UpDEFOG_HW) 	: HEQ_HWw(UP(UpDEFOG_HW));		break;
		//case UP_ADDR(UpDEFOG_HSP)	: HEQ_HSPw(UP(UpDEFOG_HSP));	break;
		//case UP_ADDR(UpDEFOG_VW) 	: HEQ_VWw(UP(UpDEFOG_VW));		break;
		//case UP_ADDR(UpDEFOG_VSP) 	: HEQ_VSPw(UP(UpDEFOG_VSP));	break;

		case UP_ADDR(UpDEFECT_DIFTH) : DF_DIFTHw(UP(UpDEFECT_DIFTH));		break;
//			case UP_ADDR(UpDEFECT_CWEGHT) : DF_CWEGHTw(UP(UpDEFECT_CWEGHT));	break;

		case UP_ADDR(UpDEFECT_TSEL) : if(aiInit) ParSet(UpDEFECT_TSEL, 0);
									  DF_TSELw(UP(UpDEFECT_TSEL));	break;


		//#pragma push_macro("UpFrameMode");
		//#undef UpFrameMode
		//#pragma pop_macro("UpFrameMode")
		#define UP_MENU_SET(N,V,CODE)	N##_##_IDX : if(aiInit == 0) { V = UP0(N##_); CODE; menu_redraw(1,0); } break;
		#define UP_MENU_SET1(N,CODE)	N##_##_IDX : if(aiInit == 0) { CODE; menu_redraw(1,0); } break;
	
//		case UP_MENU_SET(UpMpOutFps			, MP(MpOutFps)		, )
		case UP_MENU_SET(UpFrameMode		, MP(FrameMode)		, FrameRateLvSet())
//		case UP_MENU_SET(UpMpFrameRateLv	, MP(MpFrameRateLv)	, )
		case UP_MENU_SET(UpMpWdrWgt			, MP(MpWdrWgt)		, FrameRateLvSet())
		case UP_MENU_SET(UpMpAdnr			, MP(MpAdnr)		, FrameRateLvSet())
		case UP_MENU_SET(UpMpSysFreq		, MP(MpSysFreq)		, )
//		case UP_MENU_SET(UpMpOutSclUser		, MP(MpOutSclUser)	, )
		case UP_MENU_SET(UpMpCBar			, MP(MpCBar)		, )

		case UP_ADDR(UpMpReset)       : /*MP(MpReset) = UP(UpMpReset);*/ if(aiInit == 0) { MenuReset(); SetFontChg(); menu_redraw(1,0); } break;

		case UP_MENU_SET(UpMpLanguageSel	, MP(MpLanguageSel)	, SetFontChg())
		case UP_MENU_SET(UpMpOutFromat		, MP(MpOutFromat)	, )
		case UP_MENU_SET(UpMpCamID			, MP(MpCamID)		, )
		case UP_MENU_SET(UpMpBaudrate		, MP(MpBaudrate)	, )
		case UP_MENU_SET(UpMpCamTitleOn		, MP(MpCamTitleOn)	, )
		case UP_MENU_SET(UpMpCamTitle0		, gbCamTitle(0)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle1		, gbCamTitle(1)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle2		, gbCamTitle(2)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle3		, gbCamTitle(3)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle4		, gbCamTitle(4)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle5		, gbCamTitle(5)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle6		, gbCamTitle(6)		, OsdCamTitle())
		case UP_MENU_SET(UpMpCamTitle7		, gbCamTitle(7)		, OsdCamTitle())
		
		case UP_MENU_SET(UpMpIris			, MP(MpIris)		, )
		case UP_MENU_SET(UpMpIrisLvl		, MP(MpIrisLvl)		, )
		case UP_MENU_SET(UpMpBrightness		, MP(MpBrightness)	, )
		case UP_MENU_SET(UpMpBrightnessMin	, MP(MpBrightnessMin),)
		case UP_MENU_SET(UpMpLSpotLv		, MP(MpLSpotLv)	    , )
		case UP_MENU_SET(UpMpShutter		, MP(MpShutter)		, )
		case UP_MENU_SET(UpMpShutSpd		, MP(MpShutSpd)		, )
		
		case UP_MENU_SET1(UpBY_Gain_Negative 		, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpBY_Gain_Positive 	 	, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpRY_Gain_Negative 		, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpRY_Gain_Positive 	 	, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpBY_Hue_Negative		 	, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpBY_Hue_Positive	 		, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpRY_Hue_Negative	 		, Hue_Chroma_Menu_Set(1))
		case UP_MENU_SET1(UpRY_Hue_Positive	 		, Hue_Chroma_Menu_Set(1))
	#if AE_CTRL_ADV
		case UP_MENU_SET(UpMpShtMin			, gbMpShtMin		, ShutterMenuSet())
		case UP_MENU_SET(UpMpDeblurMin		, gbMpDeblurMin		, ShutterMenuSet())
		case UP_MENU_SET(UpMpShtMax			, gbMpShtMax		, ShutterMenuSet())
	#endif
		case UP_MENU_SET(UpMpDcMode			, MP(MpDcMode)		, ShutterMenuGet())
		case UP_MENU_SET(UpMpShtMode		, MP(MpShtMode)		, ShutterMenuGet())
		case UP_MENU_SET(UpMpDss			, MP(MpDss)			, )
		case UP_MENU_SET(UpMpAgc			, MP(MpAgc)			, )
		case UP_MENU_SET(UpMpAgcMin			, MP(MpAgcMin)		, )

		case UP_MENU_SET(UpMpAllSpeed		, MP(MpAllSpeed)	, )
		case UP_MENU_SET(UpMpAgcSpeed		, MP(MpAgcSpeed)	, )
		case UP_MENU_SET(UpMpIrsSpeed		, MP(MpIrsSpeed)	, )
		case UP_MENU_SET(UpMpOpenSpeed		, MP(MpOpenSpeed)	, )
		case UP_MENU_SET(UpMpCloseSpeed		, MP(MpCloseSpeed)	, )
		case UP_MENU_SET(UpMpSmallSpeed		, MP(MpSmallSpeed)	, )
		case UP_MENU_SET(UpMpSatBrt			, MP(MpSatBrt)		, )
		case UP_MENU_SET(UpMpMinGammaY		, MP(MpMinGammaY)	, )
		case UP_MENU_SET(UpMpAntiSatOn		, MP(MpAntiSatOn)	, )
		case UP_MENU_SET(UpMpStabilizing	, MP(MpStabilizing)	, AntiSatStabilizingSet())

		case UP_ADDR(UpMpBackLight)   :	if(UP(UpMpBackLight) != MN_BL_HLC && UP(UpMpBackLight) != MN_BL_BLC) ParSet(UpMpBackLight, MN_BL_OFF);
										if(aiInit == 0) {
											if(WDR_MODE==0) { MP(MpBackLight) = UP(UpMpBackLight); menu_redraw(1,0); }
											MP(MpBackLightNor) = UP(UpMpBackLight);
										} break;

		case UP_MENU_SET(UpMpHlMaskThrs		, MP(MpHlMaskThrs)		, )
		case UP_MENU_SET(UpMpHlMaskColor	, MP(MpHlMaskColor)		, )
		case UP_MENU_SET(UpMpBlcPosX		, MP(MpBlcPosX)			, )
		case UP_MENU_SET(UpMpBlcPosY		, MP(MpBlcPosY)			, )
		case UP_MENU_SET(UpMpBlcSizX		, MP(MpBlcSizX)			, )
		case UP_MENU_SET(UpMpBlcSizY		, MP(MpBlcSizY)			, )
		case UP_MENU_SET(UpAE_WDR_LTGT_OFST2, MP(MpWdrABrt)			, )
		case UP_MENU_SET(UpAE_WDR_STGT_OFST2, MP(MpWdrABrtS)		, )

		case UP_MENU_SET(UpMpTdn			, MP(MpTdn)				, )
		case UP_MENU_SET(UpMpTdnThrs		, MP(MpTdnThrs)			, )
		case UP_MENU_SET(UpMpTdnMarg		, MP(MpTdnMarg)			, )
		case UP_MENU_SET(UpMpTdnDly			, MP(MpTdnDly)			, )
		case UP_MENU_SET(UpMpLedOn			, MP(MpLedOn)			, )
		case UP_MENU_SET(UpMpLedSatCtrl		, MP(MpLedSatCtrl)		, )
		case UP_MENU_SET(UpMpTdnSw			, MP(MpTdnSw)			, )
		case UP_MENU_SET(UpMpTdnDNThres		, MP(MpTdnDNThres)		, )
		case UP_MENU_SET(UpMpTdnNDThres		, MP(MpTdnNDThres)		, )

		case UP_MENU_SET(UpMpAce			, MP(MpAce)				, )
		case UP_MENU_SET(UpMpAceGmgn		, MP(MpAceGmgn)			, )
		case UP_MENU_SET(UpMpAceBrt			, MP(MpAceBrt)			, )
		case UP_MENU_SET(UpMpAceCont		, MP(MpAceCont)			, )
		case UP_MENU_SET(UpMpDefog			, MP(MpDefog)			, )
		case UP_MENU_SET(UpMpDefogMode		, MP(MpDefogMode)		, )
		case UP_MENU_SET(UpMpDefogLevel		, MP(MpDefogLevel)		, )
		case UP_MENU_SET(UpACEWDR1_TH		, MP(MpWdrDBrt)		    , )
		case UP_MENU_SET(UpACEWDR2_TH		, MP(MpWdrDCont)		, )

		case UP_MENU_SET(UpMpAwb			, MP(MpAwb)				, )
		case UP_MENU_SET(UpMpAwbPrst		, MP(MpAwbPrst)			, )
		case UP_MENU_SET(UpMpAwbMnl			, MP(MpAwbMnl)			, )
		case UP_MENU_SET(UpMpRgain			, MP(MpRgain)			, )
		case UP_MENU_SET(UpMpBgain			, MP(MpBgain)			, )
		case UP_MENU_SET(UpMpSaturation		, MP(MpSaturation)		, )
		case UP_MENU_SET(UpMpSaturationR	, MP(MpSaturationR)		, )
		case UP_MENU_SET(UpMpSaturationB	, MP(MpSaturationB)		, )
		case UP_MENU_SET(UpAWB_SPEED_MTRX	, MP(MpAwbSpeed)		, )
		case UP_MENU_SET(UpAWB_AGC_WGT_MIN	, MP(MpAwbAgcWgtMin)	, )
		case UP_MENU_SET(UpLSUP_ON			, MP(MpLSUP_ON)			, )
		case UP_MENU_SET(UpAWB_YSLICE		, MP(MpAwbYSliceMax)	, /*AWBL_CYSLICEw(UP(UpAWB_YSLICE))*/)
		case UP_MENU_SET(UpMpCES_ON			, MP(MpCES_ON)			, )
		case UP_MENU_SET(UpMpWdrCES_ON		, MP(MpWdrCES_ON)		, )

		case UP_MENU_SET(UpMpSharpness		, MP(MpSharpness)		, )
		case UP_MENU_SET(UpMpShpSmallEdge	, MP(MpShpSmallEdge)	, )
		case UP_MENU_SET(UpMpShpBigEdge		, MP(MpShpBigEdge)		, )
		case UP_MENU_SET(UpMpShpAgcLow		, MP(MpShpAgcLow)		, IspAgcSet())
		case UP_MENU_SET(UpMpShpAgcMid		, MP(MpShpAgcMid)		, IspAgcSet())
		case UP_MENU_SET(UpMpShpAgcHigh		, MP(MpShpAgcHigh)		, IspAgcSet())
		case UP_MENU_SET(UpMpGamma			, MP(MpGamma)			, )
		case UP_MENU_SET(UpMpFlip			, MP(MpFlip)			, )
		case UP_MENU_SET(UpMpMirror			, MP(MpMirror)			, )
		case UP_MENU_SET(UpMpShading		, MP(MpShading)			, )
		case UP_MENU_SET(UpMpShdWeight		, MP(MpShdWeight)		, )
		case UP_MENU_SET(UpMpPrivacy		, MP(MpPrivacy)			, )
		case UP_MENU_SET(UpMpPvcZone		, MP(MpPvcZone)			, )
		case UP_MENU_SET(UpMpPvcFormat		, MP(MpPvcFormat)		, )
		case UP_MENU_SET(UpMpPvcTrans		, MP(MpPvcTrans)		, )
		case UP_MENU_SET(UpMpPvcYLevel		, MP(MpPvcYLevel)		, )
		case UP_MENU_SET(UpMpPvcCRLevel		, MP(MpPvcCRLevel)		, )
		case UP_MENU_SET(UpMpPvcCBLevel		, MP(MpPvcCBLevel)		, )
		case UP_MENU_SET(UpMpItl			, MP(MpItl)				, )
		case UP_MENU_SET(UpMpItlSens		, MP(MpItlSens)			, )
		case UP_MENU_SET(UpMpItlWindowuse	, MP(MpItlWindowuse)	, )
		case UP_MENU_SET(UpMpItlDettone		, MP(MpItlDettone)		, )
		case UP_MENU_SET(UpMpItlRectFill	, MP(MpItlRectFill)		, )
		case UP_MENU_SET(UpMpItlMaskOsd		, MP(MpItlMaskOsd)		, )
		case UP_MENU_SET(UpMpItlAlarm		, MP(MpItlAlarm)		, )
		case UP_MENU_SET(UpMpItlSigno		, MP(MpItlSigno)		, )
//		case UP_MENU_SET(UpMpShdDet			, MP(MpShdDet)			, )
//		case UP_MENU_SET(UpMpDefDet			, MP(MpDefDet)			, )

	#if(model_Lens==0)
		case UP_MENU_SET(UpMpFad_On			, MP(MpFad_On)			, )
		case UP_MENU_SET(UpMpFad_Zone		, MP(MpFad_Zone)		, )
		case UP_MENU_SET(UpMpFad_Trans		, MP(MpFad_Trans)		, )
		case UP_MENU_SET(UpMpFad_PosX		, MP(MpFad_PosX)		, )
		case UP_MENU_SET(UpMpFad_PosY		, MP(MpFad_PosY)		, )
		case UP_MENU_SET(UpMpFad_SizX		, MP(MpFad_SizX)		, )
		case UP_MENU_SET(UpMpFad_SizY		, MP(MpFad_SizY)		, )
	#elif(model_Lens==1)
		case UP_MENU_SET(UpMpMAF_mode		, MP(MpMAF_mode)		, )
		case UP_MENU_SET(UpMpMAF_Filter		, MP(MpMAF_Filter)		, )
		case UP_MENU_SET(UpMpMAF_MotorInit	, MP(MpMAF_MotorInit)	, )
		case UP_MENU_SET(UpMpMAF_TestMode	, MP(MpMAF_TestMode)	, )
		case UP_MENU_SET(UpMpMAF_TotalPos_Z	, MP(MpMAF_TotalPos_Z)	, )
		case UP_MENU_SET(UpMpMAF_StartPos_Z	, MP(MpMAF_StartPos_Z)	, )
		case UP_MENU_SET(UpMpMAF_EndPos_Z	, MP(MpMAF_EndPos_Z)	, )
		case UP_MENU_SET(UpMpMAF_TotalPos_F	, MP(MpMAF_TotalPos_F)	, )
		case UP_MENU_SET(UpMpMAF_StartPos_F	, MP(MpMAF_StartPos_F)	, )
		case UP_MENU_SET(UpMpMAF_EndPos_F	, MP(MpMAF_EndPos_F)	, )
		case UP_MENU_SET(UpMpMAF_LensModel 	, MP(MpMAF_LensModel)	, )
	#endif

	#if	(IMAGE_INPUT_MODE==1)
		case UP_MENU_SET(UpMpPipOn			, MP(MpPipOn)			, )
		case UP_MENU_SET(UpMpPipColor		, MP(MpPipColor)		, )
		case UP_MENU_SET(UpMpPip1_PosX		, MP(MpPip1_PosX)		, )
		case UP_MENU_SET(UpMpPip1_PosY		, MP(MpPip1_PosY)		, )
		case UP_MENU_SET(UpMpPip1_SizX		, MP(MpPip1_SizX)		, )
//		case UP_MENU_SET(UpMpPip1_SizY		, MP(MpPip1_SizY)		, )
	#endif

	#if(DOOR_BELL_CAMERA==1)
		case UP_MENU_SET(UpMpDoorCam_LP_Mode, MP(MpDoorCam_LP_Mode)	, )
	#endif

	#endif
		default : break;
	}
#endif
}

void UsrParReset(void)
{	// User Parameter Reset
	gbUsrParTbl[0]				= 0;
	gbUsrParTbl[UP_START]		= (BYTE)((UP_DATA_ID>>8)&0xff);		// for verify
	gbUsrParTbl[UP_END]			= (BYTE)((UP_DATA_ID)&0xff);			// "

#if (model_Par==1) || (model_Par==2)
	#undef UP_SET
	#define UP_SET(S,N,...)		ParSet0(N##_,N)
	#include "isp_app_para_tbl.h"
	USR_PAR_LIST
#endif
}

void InitPar(void)
{
	int i;
	for(i=UP_START+1; i<UP_END_REAL-1; i++) UsrParSet(i, 1);
}

void UsrDataReset(void)
{	// User Data Reset
	UD(UD_START)		= (BYTE)((UD_DATA_ID>>8)&0xff);		// for verify
	UD(UD_END)			= (BYTE)((UD_DATA_ID)&0xff);			// "
}

//----------------------------------------------------------------------------------------
//void SetPa


// Common ----------------------------------------------------------
void ISPM AppSavePar(void)
{	// save parameter to memory
	static BYTE bTwiWrEep2Rdy = 1;	// TwiWrEep2() Ready

	if (bTwiWrEep2Rdy) {
			
		if (gbMnParSaveChk) {			// Menu Parameter

#if model_Save
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save
					#if 0 // Old
						SfWrite2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);
					
					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_MENU;
//						gbMnParTbl[0]=FLASH_INVALID;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbMnParTbl;

						CPU_IRQ1 = 1;
						
					#endif
				#else
					TwiWrEep2(TWI_EEP_MENU_STT, gbMnParTbl, MN_PAR_EA);
				#endif

				gbMnParSaveChk = 0;
				bTwiWrEep2Rdy = 0;

				TxStr("MENU PAR Save...", 0, 0);
			}
		}

#if model_Par_Save
		else if (gbUsrParSaveChk) {		// User Parameter

#if model_Save
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save
					#if 0 // Old
						SfWrite2(gbUsrParTbl, USR_PAR_EA, FLASH_USER_MODE);
					
					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_USER;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbUsrParTbl;
//						TxStrNoIRQ("gnISPTblAddr : ", gnISPTblAddr, 8);

						CPU_IRQ1 = 1;
						
					#endif
				#else	
					TwiWrEep2(TWI_EEP_USER_STT, gbUsrParTbl, USR_PAR_EA);
				#endif

				gbUsrParSaveChk = 0;
				bTwiWrEep2Rdy = 0;
			}
		}
#endif
		else if (gbUsrDataSaveChk) {	// User Data

#if model_Save
			if(CS_ISPCMD == 0)
#endif
			{
				#if model_Save
					#if 0 // Old
						SfWrite2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);
					
					#else
						gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_DATA;
						gptMsgISPTbl.ISPTblAddr =(UINT)&gbUsrDataTbl;
//						TxStrNoIRQ("gnISPTblAddr : ", gnISPTblAddr, 8);

						CPU_IRQ1 = 1;
						
					#endif
				#else
					TwiWrEep2(TWI_EEP_DATA_STT, gbUsrDataTbl, USR_DATA_EA);
				#endif

				gbUsrDataSaveChk = 0;
				bTwiWrEep2Rdy = 0;
			}
		}
	}

#if model_Save
	bTwiWrEep2Rdy = 1;
#else
	TwiWrEep2_continue			// continuous EEP ROM page Write 	
#endif
}

void ISPM AppLoadParNone(BYTE abSel)
{	// parameter loard ignore

	switch(abSel) {
		case 0 : { // Menu Parameter
#if (model_Lens==0)
			MP(MpFad_On) = 0;
#endif
			break;
		}
		case 1 : { // User Parameter
			break;
		}
		case 2 : { // User Data
			break;
		}
	}
}

void ISPM AppLoadPar(void)
{	// read parameter from memory

	if (gbMnParReadChk) {			// Menu Parameter

		#if model_Save
			#if 0 // Old
				SfRead2(gbMnParTbl, MN_PAR_EA, FLASH_MENU_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)&gbMnParTbl, (SFLS_BASE+FLASH_SECT_MENU_STT*FLASH_SECT_SIZE), MN_PAR_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#else
		    TwiRdEep(TWI_EEP_MENU_STT, gbMnParTbl, MN_PAR_EA);
		#endif
		AppLoadParNone(0);

		gbMnParReadChk = 0;
	}

#if model_Par_Save
	if (gbUsrParReadChk) {			// User Parameter

		#if model_Save
			#if 0 // Old
				SfRead2(gbUsrParTbl, USR_PAR_EA, FLASH_USER_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)gbUsrParTbl, (SFLS_BASE+FLASH_SECT_USER_STT*FLASH_SECT_SIZE), USR_PAR_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#else
			TwiRdEep(TWI_EEP_USER_STT, gbUsrParTbl, USR_PAR_EA);
		#endif

		if (UP_END_REAL>UP_END)	TxStrDecNoIRQ("USER PAR size over :", UP_END_REAL, 4);			// Size verify
		if ((WORD)((gbUsrParTbl[UP_START]<<8)|gbUsrParTbl[UP_END])!=(WORD)UP_DATA_ID) {			// reset condition
		
			gbUsrParSaveChk = 1;	// TxParRead()에 의해 Read하였을 때 잘못된 값이 나온 경우 잘못된 값을 그대로 Write하며, Reset 시 UsrParReset()이 실행되어 다시 올바른 값으로 Write 된다.
			TxStrNoIRQ("USER PAR reset ", ((gbUsrParTbl[UP_START]<<8)|gbUsrParTbl[UP_END])|(UP_DATA_ID<<16), 8);
	    }
		AppLoadParNone(1);
		
		gbUsrParReadChk = 0;
		
	}
#endif

	if (gbUsrDataReadChk) {			// User Data

		#if model_Save
			#if 0 // Old
				SfRead2(gbUsrDataTbl, USR_DATA_EA, FLASH_DATA_MODE);
			#else //New
				BYTE bRes=0;
				while(CS_SFLS);
				bRes = FlsRead((BYTE *)gbUsrDataTbl, (SFLS_BASE+FLASH_SECT_DATA_STT*FLASH_SECT_SIZE), USR_DATA_EA);	// 1 : Success, 0 : Fail
				CS_SFLS = 0;
			#endif
		#else
			TwiRdEep(TWI_EEP_DATA_STT, gbUsrDataTbl, USR_DATA_EA);
		#endif

		if (UD_END_REAL>UD_END)	TxStrNoIRQ("USER DATA size over ", 0, 0);			// Size verify
		if ((WORD)((UD(UD_START)<<8)|UD(UD_END))!=(WORD)UD_DATA_ID) {				// reset condition

			UsrDataReset();
			gbUsrDataSaveChk = 1;
			TxStrNoIRQ("USER DATA reset ", 0, 0);
	    }
		AppLoadParNone(2);
		
		gbUsrDataReadChk = 0;
		
	}
}

/* EOF */
	
