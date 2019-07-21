/* **************************************************************************
 File Name	:	isp_app_ae.c
 Description:	EN673 - Auto Exposure function
 Designer	:	Lee, Hoon
 Modifier	:	Kim, Sunghoon / Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

//*******************************************************************************
// Reserved define & tables
//*******************************************************************************

extern INT AEControlErr;

#if (model_1M)

	#define AE_SYS60_FLICKER 		(int)LibAeShtLine(450)					// 20m    / (60p 1H : 22.222u) / 2 = 450 line => 1/100s
	#define AE_SYS50_FLICKER 		(int)LibAeShtLine(313)					// 16.66m / (50p 1H : 35.55u ) / 2 = 313 line => 1/120s

	#define AE_GAIN_TGT_OFST		DFSEL_Sens(0x80, 0x100, 0x0, 0x80, 0x80, 0x0, 0x80, 0x0, 0x0, 0x0)	// Gain min offset
	
//#elif (model_2M30p)
#elif (model_2M|model_2M30p)	// 2015911 - WHL


	#define AE_SYS60_FLICKER 		(int)LibAeShtLine(675)					// 40m    / (30p 1H : 29.629u) / 2 = 675 line => 1/50s
	#define AE_SYS50_FLICKER 		(int)LibAeShtLine(469)					// 33.33m / (25p 1H : 35.555u) / 2 = 469 line => 1/60s

	// model_2M와 model_2M30p 모두 적용되므로 DFSEL_Sens()사용 불가!!!
	#define AE_GAIN_TGT_OFST		0x80//DFSEL_Sens(0x80, 0x80, 0x0, 0x80, 0x80, 0x80, 0x8, 0x0, 0x80, 0x80)	// Gain min offset

#endif

	// AE Common
	#define AE_VTW_60				FR_VTW60
	#define AE_VTW_50				FR_VTW50
	#define AE_HTW_60				FR_HTW60
	#define AE_HTW_50				FR_HTW50

	//#define AE_SYS60_FLICKER 		(int)LibAeShtLine(677)					// 40m / (30p 1H : 29.629u) / 2 = 677 line => 1/50s
	//#define AE_SYS50_FLICKER 		(int)LibAeShtLine(936)					// 33.33m / (25p 1H : 35.55u) = 936 line => 1/30s
	#define AE_SHT_FLK				((MP(MpSysFreq)==MN_SYSFREQ_60) ? AE_SYS60_FLICKER : AE_SYS50_FLICKER)

	#define AE_WDR_LONG_TH_OFST		0x80

	#define AE_WDR_SHORT_MIN1		(int)LibAeShtLine(20)
	#define AE_WDR_SHORT_MIN2		(AE_WDR_SHORT_MIN1+10)
	#define	AE_WDR_SWGT_MAXCNT		(nPxCnt>>6)

	#define AE_ERR_MGN				0x8
	#define AE_ERR_MIN				0x1

#if model_LED_ASAT
	//#define AE_LED_ASAT_COND		((gnTdnChkOut==TDN_NIT)&&(MP(MpBackLight)!=MN_BL_WDR)&&MP(MpLedSatCtrl))
	#define AE_LED_ASAT_COND		(((gnTdnChkOut==TDN_NIT)&&(MP(MpLedOn)==MN_ON))&&(MP(MpBackLight)!=MN_BL_WDR)&&MP(MpLedSatCtrl))

	#define AE_LED_TGT_GAIN_FIX		((int)/*0x20*/0x30/*0x10*/)		// (iErrMgn<<1) 보다 커야 함
	#define AE_LED_MAX_SHIFT_BIT 	7
	#define AE_LED_MIN_SHIFT_BIT 	5
#else
	#define AE_LED_ASAT_COND		0
#endif

#if (model_Led==1)
	#define AE_LED_ON				{GpioSetHi(GPIO_LED_CTL);}		// IR-LED ON/OFF
	#define AE_LED_OFF				{GpioSetLo(GPIO_LED_CTL);}
#else
	#define AE_LED_ON 				IRIS_CLPHw(1375);	//IRIS_ONw(0x1);								// 170209 KSH
	#define AE_LED_OFF				IRIS_CLPHw(0);		//IRIS_ONw(0x0); 	// IR-LED Off 20141103 JYP	// 170209 KSH
#endif

#if (model_Iris_DC)
	#define AE_OUTDOOR_COND		((MP(MpIris)==MN_LENS_DC)&&(MP(MpDcMode)==MN_DC_OUT))																		// Origianl AE only
	#define AE_DEBLUR_COND		(((MP(MpIris)==MN_LENS_MNL)&&(MP(MpShtMode)==MN_SHT_DEBLUR)) || ((MP(MpIris)==MN_LENS_DC)&&(MP(MpDcMode)==MN_DC_DEBLUR)))	// Origianl AE only
#else
	#define	AE_OUTDOOR_COND		0									// Origianl AE only
	#define AE_DEBLUR_COND		(MP(MpShtMode)==MN_SHT_DEBLUR)		// Origianl AE only
#endif

#if (model_Iris==0)
	#define AE_IRS_OPN_Max		0x0
	#define AE_IRS_CLS_Max		0x0
	#define AE_IRS_STAT_Max		0

#elif (model_Iris==1) //-----------------------------------------------------------
	#define AE_IRS_OPN_Max		0x100
	#define AE_IRS_CLS_Max		0x300
	#define AE_IRS_STAT_Max		1200

#elif (model_Iris==2) //-----------------------------------------------------------
	#define AE_IRS_OPN_Max		0x145/*0x1c5*//*0x180*/
	#define AE_IRS_CLS_Max		0x345/*0x2c5*//*0x380*/
	#define AE_IRS_STAT_Max		/*1800*/(1200/*<<(FPS_VDI>30)*/)

#endif


#define AE_DEBUG_X		24
#define AE_WDR_DEBUG_Y	7


// Lib link ---------------------------------------

extern const WORD gwAeLssLog60Tbl[];
extern const WORD gwAeLssLog50Tbl[];

extern int AE_GAIN_TOTAL;
extern int AE_GAIN_TOTAL2;

extern void ISPM AE_SHTw(UINT val);
extern void ISPM AE_DSSw(UINT val);
extern void ISPM AE_AGCw(UINT val);
extern void ISPM AE_DGCw(UINT val);
extern void ISPM AE_FREQw(UINT val, UINT val1);
extern void ISPM AE_ETCw(UINT val);

extern UINT	ISPM LibAeShtLine(UINT anLine);
extern WORD ISPM AeSHT(int aiVal, UINT anManual, UINT anManualVal);
extern void ISPM AeAGC(int aiVal);
extern void ISPM AeIRS(const int aiVal);


extern const BYTE gbSensShtDly;
extern const BYTE gbSensAgcDly;
extern const BYTE *const gbVHssAgcTbl;	// extern const BYTE gbVHssAgcTbl0[]와 다름 !!!
extern const UINT gVHssAgcTblEa;

//extern int giIrsVal;
extern BYTE gbIrsDly;

extern int giShtVal;
extern BYTE gbShtDly;
extern BYTE gbVHssAgc;

extern int giShtSVal;
extern BYTE gbVHssSAgc;

extern int giAgcVal;
extern BYTE gbAgcDly;
extern int giAgcUpSpd;
extern int giAgcDnSpd;

extern int iCurBg;
extern int iCurSpot;
extern int iTgtSpotBg;

extern WORD ISPM AeSHT0(int aiVal, UINT anManual, UINT anManualVal);
extern void ISPM AeAGC0(int aiVal);
extern int ISPM AGC2POS(int aiAgc);
extern int ISPM POS2AGC(int aiPos);
extern void SetAeDevDly(const BYTE, const BYTE);
extern void InitAeCtrl(const BYTE, const BYTE, const BYTE*, const UINT, const WORD);
extern int ISPM ShtCtrl(const BYTE abCH, const int aiErr, const int aiMargin, const int aiGain, const int aiShtMin, const int aiShtMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abAgcOn, const int aiShtManual);
extern int ISPM AgcCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiAgcMin, const int aiAgcMax, const BYTE abEstOn, const BYTE abDlyOn, const BYTE abManual, const int aiAgcManual);
extern int ISPM IrsCent(const int aiErr, const int aiMargin, const int aiIrisOpenMax, const int aiIrisCloseMax, const int aiIrsPosMax);
extern int ISPM IrsCtrl(const int aiErr, const int aiMargin, const int aiGain, const int aiIrsCen, const BYTE abManual, const int aiIrsManual);
extern void ISPM IrsPos(const int aiErr, const int aiErrMgn, const int aiGain, const int aiIrsPosMin, const int aiIrsPosMax);
extern int ISPM ErrNor(const int aiErr, const int aiCur, const int aiTgt, const BYTE abHalf, const UINT anPosGainBit, const UINT anNegGainBit);
extern int ISPM ErrMgn(const int aAE_ERR_MGN_MAX, const int aAE_ERR_MGN_MIN, const int aAE_ERR_MIN, const BYTE abTgtChgCnt);
extern void ISPM LtPosThSet(void);
extern int ISPM LtPos(const int aiVal0, const int aiVal1);
extern int ISPM CurBg(const int aiTgt, const UINT nSlicLvl, const UINT nSlicCnt, const UINT nClipLvl, const UINT nClipCnt, const UINT nSum1, const UINT nPxCnt, const int aiTgtMin);
extern void ISPM CurSat(const UINT nClipLvl, const UINT nClipCnt, const UINT nSum2, const int iTgtOfs);
extern void ISPM SatCtrl(const BYTE abSatOff, int *apiErr, const int iErrMgn, const int nClipCnt);
extern int ISPM WdrAgcWgt(const BYTE abWdrAgcWgtOn, const int aiWdrAgcWgt, const int aiOffCnt, const int aiCtrlSpd);
extern void ISPM AeMon(const BYTE, const int, const int, const int, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE, const BYTE,	const int, const int, const int, const int, const int, const BYTE);

extern UINT gnAeLedWgtBit_Max;
extern UINT gnAeLedWgtBit_Min;
extern UINT gnAeLedTgt_Ofst;
extern UINT gnAeLedErrLmt_flag;
extern UINT gnAeLedDeblur_flag;
extern BYTE gbAeLedSatCtrl;
extern int  ISPM LibAeLedErrChg (const int, int, const UINT, const UINT, const UINT, const UINT);

extern UINT ISPM LibAeIrsIntp (UINT, UINT, UINT );

#if (model_float==1)
extern float32 ISPM LibAeSpeed (int, int, int);
#else
extern float ISPM LibAeSpeed (int, int, int);
#endif

extern int 	ISPM LibAeSpeedDc (int, int, int, int, int, int);


//*******************************************************************************
// Variables
//*******************************************************************************

UINT	gnAeState;	// 0:Shutter, 1:Iris, 2:AGC, 3:DSS

int		giCurAgc, giCurDss;
int		giCurDssStg;

#if 1
BYTE	gbWdrOn, gbWdrOnBuf=0, gbWdrOnBuf2=0/*, gbWdrOnBuf3=0*/;	// 170530 KSH
WORD 	/*gwWdrCnt=0,*/ gwWdrGmk=0x80<<4;
#endif

#if 1
int		giShtValBuf, giAgcValBuf;
UINT 	gnAeHssTVal, gnAeIrsTVal;
#endif

UINT	gnLSflag;

UINT 	gnAeAgcTVal;
UINT 	gnAeVtw, gnAeHtw;
int 	giErr1d,giErr2d;

//UINT	gnPgcOfst=0;
BYTE 	gbAeDebugOn;
BYTE 	gbAeF30Chk = 1;
BYTE 	gbDssRatioLmt = 0;

#if model_WDR_ROI
	UINT gnBoxROIWdr[2];
#endif
BYTE	gbBoxOnROIWdr;
BYTE	gbBoxFillROIWdr;
BYTE	gbBoxToneROIWdr;


#if 1//(model_Iris_DC)
	int 	giIrsCent = /*0x1E5*/(AE_IRS_CLS_Max+AE_IRS_OPN_Max)<<(6-1);	// mean		Enx EV38 AR0230 = 0x1E5
	int 	giIrsPos = AE_IRS_STAT_Max/*/2*/;
	int		giIrsValBuf;
#endif

	#define gbLssChgCnt	0


int iTgtMax = 0;
int iTgtMin = 0;
int iTgt = 0;
int iTgtVal = 0;
//UINT nClipCntIIR = 0;
//int iTgtIIR = 0;

int giShtMin = 0;
int giShtMax = 0;

int giAgcMin = 0;
int giAgcMax = 0;

int giShtDblMax = 0;
int giAgcDblMax = 0;

BYTE gbAeStg = AE_STG_SHT;

BYTE gbTgtChg = 0;

BYTE gbSensUpdate = 0;
//BYTE gbSensUpdateCnt = 0;
//*******************************************************************************
// Sub Functions
//*******************************************************************************
void ISPM AeDev(void)
{
#if AE_DEV_ON
	gbSensUpdate = 1;
	FUNC_TIME_OSD(gbMnDebugFnc==1, AeDev, 16, 24, 	
		AE_ETCw(0);
		AE_FREQw(0,0);
		AE_DSSw(0);
		AE_SHTw(0);
		AE_AGCw(0);
		AE_DGCw(0);
	)
	gbSensUpdate = 0;
#endif

	extern void SensorIn(const BYTE abOn);
	SensorIn(1);
}

void ISPM FreqAdjust(void)
{
//	gbAeF30Chk = LibAeF30Chk (model_2M, MN_FPS_LOW);

	if (MP(MpSysFreq)==MN_SYSFREQ_60) {
		gnAeVtw = AE_VTW_60;
		gnAeHtw = AE_HTW_60;
	}
	else {
		gnAeVtw = AE_VTW_50;
		gnAeHtw = AE_HTW_50;
	}

//	gnAeVtw = (model_Test==0) ? gnAeVtw * gbAeF30Chk : gnAeVtw;		// tmp

#if model_2M // ((model_Sens == SENS_IMX291)||(model_Sens == SENS_AR0237CSH)||(model_Sens == SENS_MN34229)||(model_Sens == SENS_MN34420))

//	if((FPS_VDO==50)||(FPS_VDO==60)) gnAeHtw = (gnAeHtw<<0); // 60P
//	else          					 gnAeHtw = (gnAeHtw<<1); // 30P
	gnAeHtw = (MN_FPS_HI) ? gnAeHtw : (gnAeHtw<<1);
#else
	gnAeVtw = (model_Test==0) ? gnAeVtw * gbAeF30Chk : gnAeVtw;
#endif

#if (model_Sens == SENS_AR0237CSH) // 180329 KSH - AR0237 Frame Control
	if(MN_FPS_HI) { AE_ETCw(1); }
	else		  { AE_ETCw(2); gnAeHtw>>=1; }
#endif

	AE_FREQw(gnAeVtw, gnAeHtw);
//	DispDatDec("gnAeHtw:", 31, 24, gnAeHtw);
}

void InitAe(void)
{
	AE_LOCK_SELw(1);
	AE_WIN_SELw(7);

	// 2015826 - WHL - AE1 area set for Histogram window (Same with AE Window2)
	AE1_HSPw(UP(UpAE2_HSP));
	AE1_HEPw(UP(UpAE2_HEP));
	AE1_VSPw(UP(UpAE2_VSP));
	AE1_VEPw(UP(UpAE2_VEP));

	AE1_SLICEw(0x0);			// DEFOG
	AE1_CLIPw(0x3ff);			// "
	AE2_SLICEw(0x0);			// Main
	AE2_CLIPw(0x3ff);			// "
	AE3_SLICEw(0x0);			// BLC
	AE3_CLIPw(0x3ff);			// "
	AE4_SLICEw(0x0);
	AE4_CLIPw(0x3ff);

	WDR_MOTION_ONw(UP(UpAE_WDR_MOTION_SEL));		// 150615 LH

#if (model_Iris==4)

	TwiRdEep(TWI_EEP_IRIS_STTZ , (BYTE*)giIrisLmtVal, 8);

	if (((UINT)giIrisLmtVal[0]>AE_PIRS_POS_MAX)||((UINT)giIrisLmtVal[1]>AE_PIRS_POS_MAX)||((UINT)giIrisLmtVal[0]>(UINT)giIrisLmtVal[1])) {
		giIrisLmtVal[0] = AE_PIRS_BOT_DFLT;
		giIrisLmtVal[1] = AE_PIRS_TOP_DFLT;
	}
#endif

	// sync allign at system init
	FreqAdjust();

#if (model_Sens == SENS_OV2718)
	AE_DSSw(gnAeVtw);
#endif

	AeSHT(0, 1, gnAeVtw);

	//AeDev();

#if (model_Led == 0)							// 141120 IR-LED Logic PWM
	IRIS_ONw(0x1/*0x0*/);						// 170209 KSH
	IRIS_SWw(0x0);
	IRIS_MODw(0x1);
	IRIS_HSPw(0x0);
	IRIS_HEPw(1375);
	IRIS_VSPw(0x0);
	IRIS_VEPw(0xfff);

	IRIS_CLPLw(0x0);
	IRIS_CLPHw(1375);							// Pulse Duty control : 1 ~ 1375 (full)
#endif

#if AE_CTRL_ADV
	InitAeCtrl(gbSensShtDly, gbSensAgcDly, gbVHssAgcTbl, gVHssAgcTblEa, ERR_SPOT_SUP_GAB);
#endif
	
#ifdef ENABLE_PA
	ParAe(PA_IRS_GAIN_MIN_ERR)	= 100/*20*//*80*/;
	ParAe(PA_IRS_GAIN_MAX_ERR)	= 300/*200*/;

	ParAe(PA_AGC_GAIN_MIN_ERR)	= 20/*10*//*20*//*80*/;
	ParAe(PA_AGC_GAIN_MAX_ERR)	= 100/*30*//*200*/;

	ParAe(PA_ERR_MGN)			= 8/*4*/;	// BRIGHTNESS 20 에서 8 이상이어야 AGC헌팅 없음
	//ParAe(PA_ERR_UP_GAIN)		= 2;
	//ParAe(PA_ERR_DOWN_GAIN)		= 1;

#if 1	// NORMAL
	ParAe(PA_IRS_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_AGC_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_SHT_UP_SPEED)		= 200;
	//ParAe(PA_SHT_DOWN_SPEED)	= 2000;
	ParAe(PA_AGC_UP_SPEED)		= 20/*40*/;
	ParAe(PA_AGC_DOWN_SPEED)	= 200/*400*/;
	ParAe(PA_IRS_OPEN_SPEED)	= 1024;
	ParAe(PA_IRS_CLOSE_SPEED)	= 1024;
#else	// BLC
	ParAe(PA_IRS_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_AGC_GAIN_MIN_BIT)	= 2;
	//ParAe(PA_SHT_UP_SPEED)		= 200;
	//ParAe(PA_SHT_DOWN_SPEED)	= 2000;
	ParAe(PA_AGC_UP_SPEED)		= 40;
	ParAe(PA_AGC_DOWN_SPEED)	= 400;
	ParAe(PA_IRS_OPEN_SPEED)	= 512;
	ParAe(PA_IRS_CLOSE_SPEED)	= 512;
#endif

	ParAe(PA_TGT_NIGHT_OFS)		= 64;

	ParAe(PA_IRS_POS_SPEED)		= UpAE_SPEED_DC_VP;

	ParAe(PA_ERR_CHT_TH)		= 60;

	ParAe(PA_WDR_SHORT_MIN1)	= 8/*1*/;

	ParAe(PA_H264_SKIP_CNT)		= 6;

	//ParAe(PA_ERR_EST_OFF)		= 1;

	//ParAe(PA_SHT_MANUAL)		= 1125;

	//ParAe(PA_CTRL_ORIGINAL)		= 1;
	//ParAe(PA_SAT_OFF)			= 1;
#endif

	//ShutterMenuGet();

	//SHT_SET(4);
	//AeSHT0(giShtVal, 0, 0);

	AeDev();
}

WORD ISPM SensAeSHT(int aiVal, UINT anManual, UINT anManualVal)
{
	return AeSHT(aiVal, anManual, anManualVal);
}

void ISPM AntiSatStabilizingSet(void)
{
	MP(MpErrChtMax) =	(MP(MpStabilizing)==MN_4sOFF) ? 1 :
						(MP(MpStabilizing)==MN_4sLOW) ? (ERR_CHT_SIZ>>1) :
						(MP(MpStabilizing)==MN_4sMID) ? (ERR_CHT_SIZ>>1)+(ERR_CHT_SIZ>>2) :
														 ERR_CHT_SIZ ;

	MP(MpErrChtMin) =	(MP(MpStabilizing)==MN_4sOFF) ? 1 :
						(MP(MpStabilizing)==MN_4sLOW) ? (ERR_CHT_SIZ>>2) :
						(MP(MpStabilizing)==MN_4sMID) ? (ERR_CHT_SIZ>>1) :
														(ERR_CHT_SIZ<<1)/3 ;

	//TxStrDec("MpErrChtMax: ", MP(MpErrChtMax), 3);
	//TxStrDec("MpErrChtMin: ", MP(MpErrChtMin), 3);
}

void ISPM AeODM(void)
{
	UINT	nBlcPosX, nBlcPosY, nBlcSizX, nBlcSizY;


	if(gbAeDebugOn||(gbMnBlcOsdOn&&(MP(MpBlcMode)==MN_BLC_MODE_CUSTOMIZE))) {
		OZONE_SELw(1);
		//AE_WIN_SELw((MP(MpBackLight) == MN_BL_BLC) ? 6 : 2);
	}

#if (model_1M)

	AE2_HSPw(UP(UpAE2_HSP));
	AE2_HEPw(UP(UpAE2_HEP));
	AE2_VSPw(UP(UpAE2_VSP));
	AE2_VEPw(UP(UpAE2_VEP));
	
#elif (model_2M|model_2M30p)

	#if (model_2M)
		const BYTE b720CropMode = ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60));
	#else
		const BYTE b720CropMode = ((MP(MpOutFps)==MN_2M30_720p_25_30)||(MP(MpOutFps)==MN_2M30_720p_50_60));
	#endif
	
	if (b720CropMode) {
		AE2_HSPw(UP(UpAE2_HSP_CROP));
		AE2_HEPw(UP(UpAE2_HEP_CROP));
		AE2_VSPw(UP(UpAE2_VSP_CROP));
		AE2_VEPw(UP(UpAE2_VEP_CROP));
 	}
	else {
		AE2_HSPw(UP(UpAE2_HSP));
		AE2_HEPw(UP(UpAE2_HEP));
		AE2_VSPw(UP(UpAE2_VSP));
		AE2_VEPw(UP(UpAE2_VEP));
	}
#endif

	if(MP(MpDefog) == MN_OFF || MP(MpDefogMode) == MN_DEFOG_MANUAL) {	// Defog-Auto일 경우 AceDefog()함수에서 AE1 ODM 설정함
		AE1_HSPw(AE2_HSPr);
		AE1_HEPw(AE2_HEPr);
		AE1_VSPw(AE2_VSPr);
		AE1_VEPw(AE2_VEPr);
	}

	if((MP(MpBackLight)==MN_BL_BLC) || MP(MpWdrBlc)) {

 	#if (0) 											// absolute coordinate
		nBlcPosX = AE2_HSPr + (MP(MpBlcPosX)*50);
		nBlcPosY = AE2_VSPr + (MP(MpBlcPosY)*60);
		nBlcSizX = nBlcPosX + (MP(MpBlcSizX)*50);
		nBlcSizY = nBlcPosY + (MP(MpBlcSizY)*60);
		nBlcSizX = MIN(nBlcSizX, AE2_HEPr);
		nBlcSizY = MIN(nBlcSizY, AE2_VEPr);
		nBlcPosX = MIN(nBlcPosX, AE2_HEPr-1);
		nBlcPosY = MIN(nBlcPosY, AE2_VEPr-1);
	#else 												// relative coordinate
	 	UINT 	nBlcHw = ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60)) ? FR_HW_720 : FR_HW;	// 1M Sensor는 FR_HW_720 와 FR_HW 가 동일
		UINT 	nBlcVw = ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60)) ? FR_VW_720 : FR_VW;	// 1M Sensor는 FR_VW_720 와 FR_VW 가 동일

		const BYTE bBlcPosX = (MP(MpBlcMode)==MN_BLC_MODE_CENTER) ?  6 : (MP(MpBlcMode)==MN_BLC_MODE_SPOT) ? 8 : MP(MpBlcPosX) ;
		const BYTE bBlcPosY = (MP(MpBlcMode)==MN_BLC_MODE_CENTER) ?  5 : (MP(MpBlcMode)==MN_BLC_MODE_SPOT) ? 8 : MP(MpBlcPosY) ;
		const BYTE bBlcSizX = (MP(MpBlcMode)==MN_BLC_MODE_CENTER) ?  8 : (MP(MpBlcMode)==MN_BLC_MODE_SPOT) ? 4 : MP(MpBlcSizX) ;
		const BYTE bBlcSizY = (MP(MpBlcMode)==MN_BLC_MODE_CENTER) ? 10 : (MP(MpBlcMode)==MN_BLC_MODE_SPOT) ? 4 : MP(MpBlcSizY) ;

	 	nBlcPosX = AE2_HSPr + (nBlcHw*bBlcPosX/40);
		nBlcPosY = AE2_VSPr + (nBlcVw*bBlcPosY/20);
		nBlcSizX = nBlcPosX + (nBlcHw*bBlcSizX/40);
		nBlcSizY = nBlcPosY + (nBlcVw*bBlcSizY/20);

		nBlcSizX = MIN(nBlcSizX, AE2_HEPr);
		nBlcSizY = MIN(nBlcSizY, AE2_VEPr);
		nBlcPosX = MIN(nBlcPosX, AE2_HEPr-1);
		nBlcPosY = MIN(nBlcPosY, AE2_VEPr-1);
	#endif

	#if (0)
		AE3_HSPw(nBlcPosX);
		AE3_HEPw(nBlcSizX);
		AE3_VSPw(nBlcPosY);
		AE3_VEPw(nBlcSizY);

	#else
	  #if (0)
		int iBlcPosX, iBlcPosY, iBlcSizX, iBlcSizY;	// mirror, flip

		iBlcPosX = (int)AE2_HEPr - (int)(MP(MpBlcPosX)*50) - (int)(MP(MpBlcSizX)*50);
		iBlcPosY = (int)AE2_VEPr - (int)(MP(MpBlcPosY)*60) - (int)(MP(MpBlcSizY)*60);

		iBlcSizX = iBlcPosX + (int)(MP(MpBlcSizX)*50);
		iBlcSizY = iBlcPosY + (int)(MP(MpBlcSizY)*60);

		iBlcSizX = MIN(iBlcSizX, (int)AE2_HEPr)  ;
		iBlcSizX = MAX(iBlcSizX, 0)		      	 ;
		iBlcSizY = MIN(iBlcSizY, (int)AE2_VEPr)  ;
		iBlcSizY = MAX(iBlcSizY, 0)		 	  	 ;
		iBlcPosX = MIN(iBlcPosX, (int)AE2_HEPr-1);
		iBlcPosX = MAX(iBlcPosX, (int)AE2_HSPr  );
		iBlcPosY = MIN(iBlcPosY, (int)AE2_VEPr-1);
		iBlcPosY = MAX(iBlcPosY, (int)AE2_VSPr	);

		AE3_HSPw((MP(MpMirror)==MN_ON) ? (UINT)iBlcPosX : nBlcPosX);
		AE3_HEPw((MP(MpMirror)==MN_ON) ? (UINT)iBlcSizX : nBlcSizX);
		AE3_VSPw((MP(MpFlip)  ==MN_ON) ? (UINT)iBlcPosY : nBlcPosY);
		AE3_VEPw((MP(MpFlip)  ==MN_ON) ? (UINT)iBlcSizY : nBlcSizY);
	  #endif
		AE3_HSPw(nBlcPosX);
		AE3_HEPw(nBlcSizX);
		AE3_VSPw(nBlcPosY);
		AE3_VEPw(nBlcSizY);

	#endif

	} else {
		AE3_HSPw(AE2_HSPr);
		AE3_HEPw(AE2_HEPr);
		AE3_VSPw(AE2_VSPr);
		AE3_VEPw(AE2_VEPr);
	}

#if model_LED_ASAT
	if(AE_LED_ASAT_COND) {
		const UINT AE2_HW_Unit = AE2_HEPr - AE2_HSPr;
		const UINT AE2_VW_Unit = AE2_VEPr - AE2_VSPr;
		const UINT AE2_HW_Gab = (MP(MpLedSatCtrl) > 10) ? (AE2_HW_Unit>>2) + ((AE2_HW_Unit/10) * (MP(MpLedSatCtrl) - 10) + 10) / 20 :
														((AE2_HW_Unit>>1) * MP(MpLedSatCtrl) + 10) / 20;
		const UINT AE2_VW_Gab = (MP(MpLedSatCtrl) > 10) ? (AE2_VW_Unit>>2) + ((AE2_VW_Unit/10) * (MP(MpLedSatCtrl) - 10) + 10) / 20 :
														((AE2_VW_Unit>>1) * MP(MpLedSatCtrl) + 10) / 20;
		AE2_HSPw(AE2_HSPr+AE2_HW_Gab);
		AE2_HEPw(AE2_HEPr-AE2_HW_Gab);
		AE2_VSPw(AE2_VSPr+AE2_VW_Gab);
		AE2_VEPw(AE2_VEPr-AE2_VW_Gab);
	}
#endif

	/*if(MP(MpWdrBlc)) {
		AE2_HSPw(AE3_HSPr);
		AE2_HEPw(AE3_HEPr);
		AE2_VSPw(AE3_VSPr);
		AE2_VEPw(AE3_VEPr);
	}*/
}

int ISPM TgtMaxGet(const BYTE abWdrOn, const UINT anLSflag)
{
	int iTgtOut;

	if(abWdrOn) {
		if(anLSflag==AeSHORT) {
//			iTgtOut = (AE_WDR_LTGT_GAIN * MP(MpBrightness)) + UP(UpAE_WDR_LTGT_OFST);
			iTgtOut = (AE_WDR_LTGT_GAIN * MP(MpBrightness)) + ((UP(UpAE_WDR_STYLE_SEL)==0)? UP(UpAE_WDR_LTGT_OFST) : UP(UpAE_WDR_LTGT_OFST2)) ; // 150802
//			iTgtOut = (AE_WDR_LTGT_GAIN * MP(MpBrightness)) + UP(UpAE_WDR_LTGT_OFST2) + (int)(0x10*((int)MP(MpWdrWgt)-3));	// 150802
		}
		else {
//			iTgtOut = (AE_WDR_STGT_GAIN * MP(MpBrightness)) + UP(UpAE_WDR_STGT_OFST);
			iTgtOut = (AE_WDR_STGT_GAIN * MP(MpBrightness)) + ((UP(UpAE_WDR_STYLE_SEL)==0)? UP(UpAE_WDR_STGT_OFST) : (UP(UpAE_WDR_STGT_OFST2)<<2)) ; // 150802 : if want preserve Short DR
//			iTgtOut = (AE_WDR_STGT_GAIN * MP(MpBrightness)) + UP(UpAE_WDR_STGT_OFST) + (int)(0x30*((int)MP(MpWdrWgt)-3));	// 150802 : if want preserve Short contrast
		}
	}
	else {
		iTgtOut = (UP(UpAE_TGT_GAIN) * MP(MpBrightness)) + UP(UpAE_TGT_OFST);
	}
	
	return iTgtOut;
}

void ISPM CurWDR(int *apiCur, int *apiTgt, int *apiErr, const UINT nClipLvl, const UINT nClipCnt, const UINT nSum2, const UINT nSum1, const UINT nPxCnt)
{
	static UINT gnClipLvlBuf;

	int iCur, iErr;
	int iTgt = TgtMaxGet(gbWdrOn, gnLSflag);

	if(gnLSflag==AeSHORT) {												// Long proc
		const UINT nClipLvLong = gnClipLvlBuf;

		iCur = (int)((nSum1 + (nClipCnt*nClipLvLong)) / nPxCnt);

		iTgt = MAX(iTgt-(giCurAgc/2), (iTgt/2));	// 13. 8. 4
		iErr = iTgt - iCur;

		if(AE_CTRL_ORIGINAL&&(MP(MpShutter)==MN_SHUT_MNL)&&(iErr<0)&&(gnAeState==0)) iErr = 0;	// 170331 KSH

		AE2_SLICEw(iTgt/3);		// 150802
		AE2_CLIPw(iTgt + AE_WDR_LCLIP_OFST);							// next long

		AE3_SLICEw(iTgt/3);		// 150802
		AE3_CLIPw(iTgt + AE_WDR_LCLIP_OFST);							// next long

		//GrpAe(GA_WDR_LONG_CLIP) = iTgt + AE_WDR_LCLIP_OFST;
	}
	else {																// Short proc
		const UINT nClipLvShort = gnClipLvlBuf;
	#if 0
		iCur = LibUtlInterp1D (MIN(nAe2ClipCnt, AE_WDR_SWGT_MAXCNT), 0, AE_WDR_SWGT_MAXCNT, nClipLvShort, (nClipCnt==0) ? 0 : nSum2 / nClipCnt );
	#else
		const int iSClipWgt = LibUtlInterp1D (MIN(nClipCnt, AE_WDR_SWGT_MAXCNT), 0, AE_WDR_SWGT_MAXCNT, 0, 0x100);
		iCur = ( (iSClipWgt * ((nClipCnt==0) ? 0 : nSum2 / nClipCnt)) + ((0x100-iSClipWgt) * nClipLvShort) ) >> 8;

		DebugDisp2(gbAeDebugOn, Dec, "WdrSCpW", AE_WDR_DEBUG_Y+6/*23*/, AE_DEBUG_X, iSClipWgt, 4)
	#endif

		//GrpAe(GA_WDR_SHORT_CLIP_1D) = nClipLvShort;

		iTgt = MAX(iTgt-(giCurAgc/2), (iTgt/2));	// 13. 8. 4
		iErr = iTgt - iCur;

		AE2_CLIPw(iTgt + AE_WDR_SCLIP_OFST);							// next short

		AE3_CLIPw(iTgt + AE_WDR_SCLIP_OFST);							// next short

		//GrpAe(GA_WDR_SHORT_CLIP) = iTgt + AE_WDR_SCLIP_OFST;
		//GrpAe(GA_WDR_SHORT_CUR) = iCur;
	}

	*apiCur = iCur;
	*apiTgt = iTgt;
	*apiErr = iErr;

	gnClipLvlBuf = nClipLvl;
}

int ISPM WdrCtrl(WORD *apwShtInt)
{
	#define AE_WDR_ON_COND	((gnLSflag==AeLONG)&&(MP(MpBackLight)==MN_BL_WDR)/*&&(MP(MpShutter)==MN_SHUT_AUTO)*/)
	#define AE_WDR_OFF_COND	(((gnLSflag==AeLONG/*AeSHORT*/)&&(MP(MpBackLight)!=MN_BL_WDR))/*||(MP(MpShutter)!=MN_SHUT_AUTO)*/)

	//--------------------------------------------------------------------------------
	// DNR OFF -> 3 Delay -> WDR ON
	static BYTE bWaitRd_Cnt = 4;
	//if(((MP(MpBackLight)==MN_BL_WDR) != (gbWdrOn==MN_ON)) && (bWaitRd_Cnt == 0)) { bWaitRd_Cnt = 3; return ; }	// Detecting WDR ON/OFF Change
	if(((MP(MpBackLight)==MN_BL_WDR) && (gbWdrOn==MN_OFF)) && (bWaitRd_Cnt == 0)) { bWaitRd_Cnt = 3; return 0; }		// Detecting WDR ON only
	if(bWaitRd_Cnt > 1) { bWaitRd_Cnt--; return 0; }

	if(giCurDssStg > 1) return 0;

	if((bWaitRd_Cnt==1) && (gnLSflag==AeLONG)) bWaitRd_Cnt = 0;

	if(AE_WDR_ON_COND && (giCurDssStg==1)) {
		giCurDssStg = 0;

		//TxStr("ISP reset.", 0, 0);
		//UINT nIsp0x5 = GetIsp(0x5);
		//UINT nIsp0x6 = GetIsp(0x6);
		//TxStrNoIRQ(" 5h:", nIsp0x5, 8);
		//TxStrNoIRQ(" 6h:", nIsp0x6, 8);

		isp_reset();

	#if 0//(model_Sens == SENS_OV2718)
		extern void InitSens(void);
		extern void InitSensCstm(void);
		InitSens();
		InitSensCstm();
		AE_FREQw(gnAeVtw, gnAeHtw);
		AE_DSSw(gnAeVtw);
		AeDev();
	#endif

		//nIsp0x5 = GetIsp(0x5);
		//nIsp0x6 = GetIsp(0x6);
		//TxStrNoIRQ(" 5h:", nIsp0x5, 8);
		//TxStrNoIRQ(" 6h:", nIsp0x6, 8);

		//return 0;
	}

	//--------------------------------------------------------------------------------
	//gbWdrOnBuf3 = gbWdrOnBuf2;	// 170530 KSH
	gbWdrOnBuf2 = gbWdrOnBuf;
	gbWdrOnBuf  = gbWdrOn;
	
	if(AE_WDR_ON_COND)			gbWdrOn = MN_ON;	// 170331 KSH
	else if(AE_WDR_OFF_COND) 	gbWdrOn = MN_OFF;	// 150729 LH : Out timing	170331 KSH

	if ((gbWdrOn==MN_ON)||(gbWdrOnBuf2==MN_ON)) {

		WDR_ONw(1);

		DDR_RD_MODw(0); // WDR DDR Priority

		//gwWdrCnt = (gwWdrCnt>=32) ? 32 : gwWdrCnt+1;

		//if(PAR00) {
//			if(!gbLssChgCnt) {
//				if (gwWdrCnt<=1)		STLL_ONw(1);	// 150729 LH : img crack at ON/OFF
//				else if (gwWdrCnt>=2)	STLL_ONw(0);
//			}
		//}

		int iSwgt = ((0x800>>2)+((MP(MpWdrWgt)*2)<<6));							// 150615 LH
		iSwgt = (MP(MpWdrWgt)==MN_3sLOW) ? ((iSwgt*UP(UpAE_WDR_SWGT_L))>>7) :
				(MP(MpWdrWgt)==MN_3sMID) ? ((iSwgt*UP(UpAE_WDR_SWGT_M))>>7) :
										   ((iSwgt*UP(UpAE_WDR_SWGT_H))>>7) ;
		WDR_SWGTw(MIN(iSwgt,0x3ff));

//		WDR_SWGTw((0x800>>2)+((MP(MpWdrWgt)*2)<<6));

		static WORD gwShtIntBuf;
		WORD wShtInt = *apwShtInt;

		if (gnLSflag==AeSHORT)	{
			wShtInt = (wShtInt>=gwShtIntBuf) ? gwShtIntBuf-1 : wShtInt;			// under long

			WDR_SSHUTw(/*ParAe(PA_STA) ? (ParAe(PA_STA)&0xFFF) : */wShtInt);												// Short weight
		}
		else {
			wShtInt = MAX(wShtInt, 1);
			WDR_LSHUTw(/*ParAe(PA_STA) ? ((ParAe(PA_STA)>>16)&0xFFF) : */wShtInt);												// Long weight

			WORD wWdrGmkNew = ((256<<4) * gwShtIntBuf) / wShtInt;				// Shutter Rate : 0 ~ 256

			#if (model_Aptn)													// 151020
			#define SHT_RT_WG_ON	96
			#define SHT_RT_WG_OFF	128
			#else
			#define SHT_RT_WG_ON	128											// slce 50%		Shutter Rate of WDR_GAMMA ON
			#define SHT_RT_WG_OFF	192											// clip 75%		Shutter Rate of WDR_GAMMA OFF
			#endif
			wWdrGmkNew = LibUtlInterp1D(MINMAX(wWdrGmkNew, SHT_RT_WG_ON<<4, SHT_RT_WG_OFF<<4), SHT_RT_WG_ON<<4, SHT_RT_WG_OFF<<4, 0x80<<4, 0x0);
			gwWdrGmk = ((0xf0 * gwWdrGmk) + ((0x100-0xf0) * wWdrGmkNew)) >> 8;

			//GrpAe(GA_WDR_GAMMA_WGT) = gwWdrGmk;
		}

		gwShtIntBuf = wShtInt;
		*apwShtInt = wShtInt;

		return 0;
	}
	else {
		return 1;
	}
}

void ISPM AEControlErrSet(void)
{
	//AEControlErr = ABS(iTgt-iCur);
	//printf(" AEControlErr %d iTgt %d iCur %d \r\n", AEControlErr iTgt, iCur);

	const int iTgtMax = TgtMaxGet(0,0);
#if 0
	const int iCurAll = (gbWdrOn) || (MP(MpBackLight)==MN_BL_BLC) ? ((nAe2SlicCnt*nAe2SlicLvl) + nAe2Sum1 + (nAe2ClipCnt*nAe2ClipLvl)) / nAe2PxCnt : 
																	((nAe3SlicCnt*nAe3SlicLvl) + nAe3Sum1 + (nAe3ClipCnt*nAe3ClipLvl)) / nAe3PxCnt ;
#else
	const int iCurAll = AE1_SUM1_LOCKr / (((AE1_VEPr - AE1_VSPr)+1) * ((AE1_HEPr - AE1_HSPr)+1));
#endif

	//GrpAe(GA_ERR_CHT_PRE) = AE1_SUM1_LOCKr>>16;

	if((gbWdrOn==MN_ON && gnLSflag==AeLONG) == 0) AEControlErr = iTgtMax - iCurAll;

	if(AEControlErr > 0) {
		AEControlErr = (AEControlErr<<9) / MAX(iCurAll,1);
	}
	//AEControlErr = ErrNor(AEControlErr, iCurAll, 0, 0, 1, 0);
	//AEControlErr = iErr;
	DebugDisp2(gbAeDebugOn, Dec, "AECtrlE", AE_WDR_DEBUG_Y+7, AE_DEBUG_X, AEControlErr, 6)
}


//*******************************************************************************
// Main Functions
//*******************************************************************************
void ISPM AeOri(void)
{
		const UINT 	nAe2Sum1    = AE2_SUM1_LOCKr;
		const UINT 	nAe2Sum2    = AE2_SUM2_LOCKr;
		const UINT 	nAe3Sum1    = AE3_SUM1_LOCKr;
		const UINT 	nAe3Sum2    = AE3_SUM2_LOCKr;
		const UINT 	nAe2ClipCnt = AE2_CLCNT_LOCKr;
		const UINT 	nAe2SlicCnt = AE2_SLCNT_LOCKr;
		const UINT 	nAe3ClipCnt = AE3_CLCNT_LOCKr;
		//const UINT 	nAe3SlicCnt = AE3_SLCNT_LOCKr;

		const UINT	nAe2PxCnt	= ((AE2_VEPr - AE2_VSPr)+1) * ((AE2_HEPr - AE2_HSPr)+1);
		const UINT	nAe3PxCnt	= ((AE3_VEPr - AE3_VSPr)+1) * ((AE3_HEPr - AE3_HSPr)+1);
		const UINT	nAe2ClipLvl	= AE2_CLIPr;
		const UINT	nAe2SlicLvl	= AE2_SLICEr;
		const UINT	nAe3ClipLvl	= AE3_CLIPr;
		const UINT	nAe3SlicLvl	= AE3_SLICEr;

static 	int		giAePos=0, giAePosLmtBuf, giAeValBuf=600;
		int		iAePosLmt=0;

		int 	iShtVal=0, iAgcVal=0;	// Nor Pos
		int		/*iTgt=0,*/ iCur=0, iErr=0;
		WORD	wShtInt=0;		// SHT

		//UINT 	nSClipCntBuf=0;
		int		iWdrSLmt=0, iWdrAgcWgt=0;

		int		iHssLmt =0;		// giAePos
		int		iIrsLmt =0;		// giAePos, giIrsPos
		int		iAgcLmt =0;		// giAePos
		int		iLssLmt =0;		// giAePos

		int 	iIrsVal = 0;	// PWM

#if (model_float==1)
static  float32	gfAePosflt;
static	float32	gfAePosWdrflt=0;
static	float32	gfIrsShtPos;
#else
static  float	gfAePosflt;
static	float	gfAePosWdrflt=0;
static	float	gfIrsShtPos;
#endif

static	BYTE	/*gbMnSysFreqBuf,*/ gbMnLensBuf=-1;

	AeODM();

	gnLSflag = LS_FLGr;

// -------------------------------------------------------------------------------
// Input condition adj

	if(gbWdrOn){	// WDR mode --------------------------------------------------

		// CurWDR() 실행전에 gnLSflag 설정되어야 함
		if(MP(MpWdrBlc)) {
			CurWDR(&iCur, &iTgt, &iErr, nAe3ClipLvl, nAe3ClipCnt, nAe3Sum2, nAe3Sum1, nAe3PxCnt);
		}
		else {
			CurWDR(&iCur, &iTgt, &iErr, nAe2ClipLvl, nAe2ClipCnt, nAe2Sum2, nAe2Sum1, nAe2PxCnt);
		}

#if model_WDR_ROI
		ROI_WDR();
#endif

	}
	else if (MP(MpBackLight)==MN_BL_BLC) {	// BLC Mode -------------------------------------

		iCur = (int)((/*(nAe3SlicCnt*nAe3SlicLvl) +*/ nAe3Sum1 + (nAe3ClipCnt*nAe3ClipLvl)) / nAe3PxCnt);		// 170221 KSH
		iTgt = TgtMaxGet(0,0);
		iErr = iTgt - iCur;

#if model_LED_ASAT
		const int iAE_CLIP_OFST = AE_LED_ASAT_COND ?
									(MP(MpLedSatCtrl) > 10) ? (UP(UpAE_CLIP_OFST) * (40 - 3*(MP(MpLedSatCtrl)-10))) / 80 - 40 :
															  (UP(UpAE_CLIP_OFST)>>1) : UP(UpAE_CLIP_OFST);
#else
		const int iAE_CLIP_OFST = UP(UpAE_CLIP_OFST);
#endif

		const int iClipLv = MIN(iTgt + iAE_CLIP_OFST, 0x3ff);

		AE3_CLIPw(iClipLv);
		AE3_SLICEw(0);		// 13.3.26
	}
	else {		// Normal Mode -------------------------------------------------------

		const int iAe2ClipCnt = (MP(MpBackLight)==MN_BL_HLC) ? LibUtlInterp1D(MP(MpHlMaskThrs)+8, 0, 28, 0, nAe2ClipCnt) : (int)nAe2ClipCnt ;

		iCur = (int)(((nAe2SlicCnt*nAe2SlicLvl) + nAe2Sum1 + (iAe2ClipCnt*nAe2ClipLvl)) / nAe2PxCnt);

		iTgt = TgtMaxGet(0,0);
		if(MP(MpBackLight)==MN_BL_HLC) 	iTgt += (iTgt>>2);
		else				   			iTgt = ((MP(MpShutter)==MN_SHUT_AUTO)&&(!AE_DEBLUR_COND)) ?
												MAX(iTgt-(giCurAgc/2), (iTgt/2)):		// decrease by AGC
												MAX(iTgt-(giCurAgc/3), (iTgt/2));		// "
		
		iErr = iTgt - iCur;

#if model_LED_ASAT
		const int iAE_CLIP_OFST = AE_LED_ASAT_COND ?
									(MP(MpLedSatCtrl) > 10) ? 	(UP(UpAE_CLIP_OFST) * (40 - 3*(MP(MpLedSatCtrl)-10))) / 80 - 40 :
																(UP(UpAE_CLIP_OFST)>>1) : (UP(UpAE_CLIP_OFST) + (giCurAgc<<1));
#else
		const int iAE_CLIP_OFST = UP(UpAE_CLIP_OFST) + (giCurAgc<<1);
#endif

		const int iSliceLv = AE_LED_ASAT_COND ? 0 : iTgt/3;
		const int iClipLv = MIN(iTgt + iAE_CLIP_OFST, 0x3ff);

		AE2_SLICEw(iSliceLv);
		AE2_CLIPw(iClipLv);
	}

// -------------------------------------------------------------------------------
// Freq adjust
	FreqAdjust();

// -------------------------------------------------------------------------------
// Status mode
	
	extern BYTE gbAdnr_WaitCnt;
	if(MP(MpBackLight) == MN_BL_WDR || gbWdrOn || gbWdrOnBuf2 || (gbAdnr_WaitCnt<3)/*gbAdnr_Excute*//*DNR3D_ONr*/) {
		gbDssRatioLmt = 1;
	}
	else {
		switch (MP(MpDss)) {
			case MN_DSS_OFF	 :	gbDssRatioLmt = 1; break;	// x1 (fix)
			case MN_DSS_X2	 :	gbDssRatioLmt = 2; break;	// x2
			case MN_DSS_X4	 :	gbDssRatioLmt = 4; break;	// x4
			case MN_DSS_X8	 :	gbDssRatioLmt = 8; break;	// x8
			case MN_DSS_X16	 :	gbDssRatioLmt =16; break;	// x16
			case MN_DSS_X32  :	gbDssRatioLmt =32; break;	// x32
			case MN_DSS_X64  :	gbDssRatioLmt = MN_FPS_HI ? 64 : 32; break;	// x64
		}
	}

	gnAeHssTVal = (MP(MpSysFreq)==MN_SYSFREQ_60) ? gwAeLssLog60Tbl[gbAeF30Chk] :
												   gwAeLssLog50Tbl[gbAeF30Chk] ;

	gnAeIrsTVal = AE_IRS_STAT_Max;

	gnAeAgcTVal = AE_GAIN_TOTAL + AE_GAIN_TOTAL2;


	iHssLmt = 	(MP(MpShutter)==MN_SHUT_MNL)&&(gbWdrOn==MN_OFF)			? 0			:	// 170331 KSH
				(MP(MpShutter)==MN_SHUT_FLICKER) 						? AE_SHT_FLK:
			  	((gbWdrOn==MN_ON)||(gnAeState==3)||(!AE_DEBLUR_COND)) 	? (int)gnAeHssTVal : AE_DEBLUR_THRS;

	iIrsLmt = ((MP(MpIris)==MN_LENS_MNL)||(gbWdrOn==MN_ON)) ? 0				:
			  (model_Iris_DC && AE_OUTDOOR_COND)			? gnAeIrsTVal + (gnAeHssTVal-AE_OUTDOOR_THRS) :
			  												  gnAeIrsTVal 	;

	iAgcLmt = (MP(MpAgc)>=255) ? (int)gnAeAgcTVal :
								 (int)((((gnAeAgcTVal-AE_GAIN_TGT_OFST)*MP(MpAgc))/255)+AE_GAIN_TGT_OFST);

	iLssLmt = 	(/*(gbWdrOn==MN_ON)||*/(MP(MpShutter)==MN_SHUT_MNL)) ? 0 :	// 170607 KSH
				(gbWdrOn==MN_ON) ? (MP(MpSysFreq)==MN_SYSFREQ_60) ? ( (MP(MpShutter)==MN_SHUT_FLICKER) ? (gnAeHssTVal-AE_SYS60_FLICKER) : 0 ) :	// 170607 KSH
																	( (MP(MpShutter)==MN_SHUT_FLICKER) ? (gnAeHssTVal-AE_SYS50_FLICKER) : 0 ) :
		  		(MP(MpSysFreq)==MN_SYSFREQ_60) 					 ?
			  										gwAeLssLog60Tbl[(gbDssRatioLmt*gbAeF30Chk)]-gnAeHssTVal + ( (MP(MpShutter)==MN_SHUT_FLICKER) ? (gnAeHssTVal-AE_SYS60_FLICKER) : 0 ) :
													gwAeLssLog50Tbl[(gbDssRatioLmt*gbAeF30Chk)]-gnAeHssTVal + ( (MP(MpShutter)==MN_SHUT_FLICKER) ? (gnAeHssTVal-AE_SYS50_FLICKER) : 0 ) ;

	
	// LED ON/OFF
	if (((MP(MpTdn)==MN_TDN_NIGHT)||(MP(MpTdn)==MN_TDN_EXTERN)) &&	MP(MpLedOn) && (gnTdnChkOut==TDN_NIT))	AE_LED_ON
	else 																									AE_LED_OFF

#if model_LED_ASAT
	// Saturation detector
	gnAeLedWgtBit_Max 	= AE_LED_MAX_SHIFT_BIT;
	gnAeLedWgtBit_Min 	= AE_LED_MIN_SHIFT_BIT;
	gnAeLedTgt_Ofst 	= AE_LED_TGT_GAIN_FIX;
	gnAeLedErrLmt_flag 	= model_Iris_DC && ((MP(MpIris)==MN_LENS_DC)&&(MP(MpDcMode)==MN_DC_DEBLUR));
	gnAeLedDeblur_flag 	= AE_DEBLUR_COND;
	gbAeLedSatCtrl		= MP(MpLedSatCtrl);
	iErr = LibAeLedErrChg(AE_LED_ASAT_COND, iErr, nAe2Sum2, nAe2ClipCnt, nAe2ClipLvl, nAe2PxCnt);
#endif

	//HLMASK_ONw(!ParAe(PA_SAT_OFF));
	//HLMASK_THw(AE2_CLIPr>>2);


// Status manage ----------------------------------------------------------------------------------------------------------------------------

	if(gbWdrOn&&(gnLSflag==AeLONG)){	// ============================================ // Long time, Short proc, Long apply

		iAePosLmt 	= giAePosLmtBuf;

		iWdrSLmt 	= LibUtlInterp1D ( MINMAX(giShtValBuf, (int)gnAeHssTVal-AE_WDR_LONG_TH_OFST, (int)gnAeHssTVal-1), gnAeHssTVal-AE_WDR_LONG_TH_OFST, gnAeHssTVal-1, AE_WDR_SHORT_MIN1, AE_WDR_SHORT_MIN2);	// Sht limit
		iWdrAgcWgt	= LibUtlInterp1D ( MINMAX(giAgcValBuf, UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC, AE_WDR_MAX_WGT, UP(UpAE_WDR_MIN_WGT));	// 141209

#if (model_float==1)
		gfAePosWdrflt = _fadd(gfAePosWdrflt, LibAeSpeed(iErr, (AE_ERR_MGN+4), UP(UpAE_SPEED) ));
		gfAePosWdrflt  = MAX(gfAePosWdrflt, _itof(iWdrSLmt));													// over than short limit
		gfAePosWdrflt  = MIN(gfAePosWdrflt, _itof((giShtValBuf-1)));											// under than long
		gfAePosWdrflt  = _fdiv(_fadd((_fmul(gfAePosWdrflt,_itof(iWdrAgcWgt))),_itof(((0x100-iWdrAgcWgt)*(giShtValBuf-1)))),_itof(256));	// AGC wgt

		iShtVal		= _ftoi(gfAePosWdrflt);
#else
		gfAePosWdrflt += LibAeSpeed(iErr, AE_ERR_MGN+4, UP(UpAE_SPEED) );
		gfAePosWdrflt  = MAX(gfAePosWdrflt, (float)iWdrSLmt);													// over than short limit
		gfAePosWdrflt  = MIN(gfAePosWdrflt, (float)(giShtValBuf-1));											// under than long

		//GrpAe(GA_WDR_AGC_WGT_TEST) = LibUtlInterp1D ( MINMAX(giAgcValBuf, UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC, (int)gfAePosWdrflt, giShtValBuf-1);

		//GrpAe(GA_ERR_CHT_PRE) = gfAePosWdrflt;
		//GrpAe(GA_ERR_NIGHT) = iWdrAgcWgt;

		gfAePosWdrflt  = ((gfAePosWdrflt*(float)iWdrAgcWgt)+(float)((0x100-iWdrAgcWgt)*(giShtValBuf-1)))/256.f;	// AGC wgt

		//GrpAe(GA_AGC_VAL_PRE) = gfAePosWdrflt;

		//GrpAeF(GA_WDR_SHORT_POS) = gfAePosWdrflt;
		//GrpAe(GA_WDR_LONG_POS) = giShtValBuf;
		//GrpAe(GA_WDR_HSS_T_POS) = gnAeHssTVal;
		//GrpAe(GA_WDR_SHORT_LMT_POS) = iWdrSLmt;
		//GrpAe(GA_WDR_AGC_WGT) = iWdrAgcWgt;	

		iShtVal		= (int)gfAePosWdrflt;
#endif
		iAgcVal 	= giAgcValBuf;

		// apply control val --------------

		wShtInt 	= AeSHT(giShtValBuf, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );												// Long

		AeAGC(giAgcValBuf);

		iIrsVal  	= (model_Iris_DC) ? 0 			: // full open
										gnAeIrsTVal ; // full open

		AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : iIrsVal);

	}

	else {	// ======================================================================== // Short time, Long proc, Short apply

		iAePosLmt = iHssLmt + iIrsLmt + iAgcLmt + iLssLmt;								// Total limit val

		if (gbWdrOnBuf!=gbWdrOn) {														// WDR change
			if (gbWdrOn==MN_ON) 				 { giAePos = (MP(MpShutter)==MN_SHUT_MNL) ? iHssLmt-1 : giShtValBuf; gnAeState = 0;}			// 170331 KSH
			else if (gbWdrOn==MN_OFF)	{
				if (MP(MpIris)==MN_LENS_MNL) 	 { giAePos = iHssLmt-1;			gnAeState = 0;}
				else if (MP(MpIris)==MN_LENS_DC) { giAePos = iHssLmt+iIrsLmt-1;	gnAeState = 1;}
			}
#if (model_float==1)
			gfAePosflt = _itof(giAePos);
#else
			gfAePosflt = (float)giAePos;
#endif
		}
		else if ((MP(MpIris)!=gbMnLensBuf)&&(gbWdrOn==MN_OFF)) {							// Lens change
			switch 	(gnAeState) {
				case 0 : giAePos 	= iHssLmt+iIrsLmt-1;			gnAeState = 1; 	break;
				case 1 : giAePos 	= (iHssLmt<0) ? 0 : iHssLmt-1; 	gnAeState = 0;	break;
				case 2 : giAeValBuf = MIN(giAeValBuf, iAgcLmt);					 giAePos = iHssLmt + iIrsLmt + giAeValBuf; break;
				case 3 : giAeValBuf = MIN(giAeValBuf, iLssLmt+(int)gnAeHssTVal); giAePos = iIrsLmt + iAgcLmt + giAeValBuf; break;
			}
#if (model_float==1)
			gfAePosflt = _itof(giAePos);
#else
			gfAePosflt = (float)giAePos;
#endif
		}
		else if (iAePosLmt!=giAePosLmtBuf) {											// Limit val change
			switch 	(gnAeState) {
				case 0 : giAeValBuf = MIN(giAeValBuf, iHssLmt);					 giAePos = giAeValBuf; break;
				case 1 : giAeValBuf = MIN(giAeValBuf, iIrsLmt);					 giAePos = iHssLmt + giAeValBuf; break;
				case 2 : giAeValBuf = MIN(giAeValBuf, iAgcLmt);					 giAePos = iHssLmt + iIrsLmt + giAeValBuf; break;
				case 3 : giAeValBuf = MIN(giAeValBuf, iLssLmt+(int)gnAeHssTVal); giAePos = iIrsLmt + iAgcLmt + giAeValBuf; break;
			}
#if (model_float==1)
			gfAePosflt = _itof(giAePos);
#else
			gfAePosflt = (float)giAePos;
#endif
		}

#if (model_Iris_P)
		else {
#else
		else if (gnAeState!=1) {
#endif

			iErr = (iErr>0) ? iErr<<1 : iErr;	// Speed equalizer
			gfAePosflt += LibAeSpeed(iErr, AE_ERR_MGN, UP(UpAE_SPEED));


#if model_float == 0	// ??
			gfAePosflt  = MIN(gfAePosflt, (float)(iAePosLmt-1));
			gfAePosflt  = MAX(gfAePosflt, 0.f);
#endif

#if (model_float==1)
			giAePos = _ftoi(gfAePosflt);
#else
			giAePos = (int)gfAePosflt;
#endif

#if (model_Iris_P)
		}
#else
		}
		else gfAePosflt = (float)giAePos;
#endif


	// ------------------------------------------------------------------------------------------------------------ //

#if (model_Iris_DC)
		if (/*(MP(MpShutter)!=MN_SHUT_MNL)&&*/((MP(MpIris)==MN_LENS_MNL)||(gbWdrOn==MN_ON))&&(giAePos<iHssLmt)) {	//--// HSS status	// 170331 KSH
#else
		if (/*((MP(MpShutter)!=MN_SHUT_MNL)||(gbWdrOn==MN_ON))&&*/(giAePos<iHssLmt)) {	// 170331 KSH
#endif

	
#if (model_float==1)
			gfAePosflt  = (MP(MpShutter)==MN_SHUT_FLICKER) ? _itof(AE_SHT_FLK) : 	// Shutter limit (Flicker)
							MAX(gfAePosflt, _itof(AE_SHT_SLICE));			// Shutter limit
#else
			gfAePosflt  = (MP(MpShutter)==MN_SHUT_FLICKER) ? AE_SHT_FLK : 	// Shutter limit (Flicker)
							MAX(gfAePosflt, (float)AE_SHT_SLICE);			// Shutter limit
#endif

			giAePos  	= (int)gfAePosflt;

			iShtVal  	= giAePos;

			if((MP(MpShutter)==MN_SHUT_MNL)&&(gbWdrOn==MN_ON)) iShtVal = (int)LibAeShtLine(gnAeVtw>>MP(MpShutSpd));		// 180327 KSH

			iIrsVal  	= (model_Iris_DC) ? 0 :	// full open
											0 ; // full close
			iAgcVal  	= 0;

			giAeValBuf 	= iShtVal;
			gnAeState  	= 0;
		}

	#if (model_Iris_DC)
		else if ((MP(MpIris)==MN_LENS_DC)&&(gbWdrOn!=MN_ON)&&(giAePos<(iHssLmt+iIrsLmt))) {	// -----------------// IRIS status

			if ((giAePos-iHssLmt) > (int)gnAeIrsTVal)	goto DoorMode;		// jump to shutter ctl range in Outdoor mode

			iIrsVal = LibAeSpeedDc(iErr, AE_ERR_MGN, AE_IRS_OPN_Max, AE_IRS_CLS_Max, UP(UpAE_SPEED_DC), 0);
			IrsPos(iErr, AE_ERR_MGN, 9/*12*/, 0, gnAeIrsTVal);
			giIrsValBuf = iIrsVal;


			//GrpAe(GA_ERR) = iErr;
			//GrpAe(GA_AGC_VAL_PRE) = iIrsVal;
			//GrpAe(GA_IRS_CTRL_ORI) = iIrsVal - (giIrsCent>>6);	
			//GrpAe(GA_IRS_POS) = giIrsPos;
			//GrpAe(GA_ERR_DAY_ORI) = giIrsPos;
			// -------------------------------------------------------------------------------
		DoorMode :
			if (!AE_OUTDOOR_COND) {

#if (model_float==1)
				gfIrsShtPos = _itof(0);
#else
				gfIrsShtPos = 0.f;
#endif
				giAePos = MIN( iHssLmt + giIrsPos, (iHssLmt + iIrsLmt));
				giAeValBuf = giAePos - iHssLmt;

				iShtVal = iHssLmt;
			}
			else {
				if (giIrsPos >= (int)gnAeIrsTVal) {													// over full open
#if (model_float==1)
					gfIrsShtPos = _fadd(gfIrsShtPos,LibAeSpeed(iErr, AE_ERR_MGN, UP(UpAE_SPEED)-42));

					gfIrsShtPos  = MINMAX(gfIrsShtPos, _itof(0), _itof((gnAeHssTVal-AE_OUTDOOR_THRS)));

					iIrsVal = MAX( (giIrsValBuf-(_ftoi(gfIrsShtPos)<<1)), 0);							// increse open step
				}
				else gfIrsShtPos = _itof(0);
#else
					gfIrsShtPos += LibAeSpeed(iErr, AE_ERR_MGN, UP(UpAE_SPEED)-42);

					gfIrsShtPos  = MINMAX(gfIrsShtPos, 0.f, (float)(gnAeHssTVal-AE_OUTDOOR_THRS));

					iIrsVal = MAX( (giIrsValBuf-((int)gfIrsShtPos<<1)), 0);							// increse open step
				}
				else gfIrsShtPos = 0.f;
#endif

				//GrpAe(GA_IRS_SHT_POS) = gfIrsShtPos;

				giAePos = MIN( iHssLmt + giIrsPos + (int)gfIrsShtPos, (iHssLmt + iIrsLmt));
				giAeValBuf = giAePos - iHssLmt;

				iShtVal = (int)AE_OUTDOOR_THRS + (int)gfIrsShtPos;
				iShtVal = MINMAX(iShtVal, AE_OUTDOOR_THRS, (int)gnAeHssTVal);

			}

			iShtVal = (MP(MpShutter)==MN_SHUT_FLICKER) ? AE_SHT_FLK : iShtVal;

			iIrsVal  = MINMAX(iIrsVal, 0, 0x3fe);
			iAgcVal    = 0;

			gnAeState  = 1;

		}
	#else	// model_Iris_P

		else if ((MP(MpIris)==MN_LENS_AUTO)&&(gbWdrOn!=MN_ON)&&(giAePos<(iHssLmt+iIrsLmt))) {
			iIrsVal = giAePos - iHssLmt;
			iIrsVal = MAX(iIrsVal, 0);
			gfIrsShtPos = 0;

			iShtVal = iHssLmt;
			iAgcVal   = 0;

			giAeValBuf = iIrsVal;
			gnAeState  = 1;
		}
	#endif	// end of model_Iris_DC

		else if (giAePos<(iHssLmt+iIrsLmt+iAgcLmt)) {	// -----------------------------------------------------// AGC status

			iAgcVal = giAePos-(iHssLmt+iIrsLmt);
			iAgcVal = MAX(iAgcVal, 0);

			const int iDeblurHssLmt = (int)gnAeHssTVal-iHssLmt;
			int iShtValOfst = (!AE_DEBLUR_COND) ? 0 :
							  iDeblurHssLmt < ((iAgcLmt*UP(UpAE_DEBLUR_GAIN))>>7) ?
							  (iAgcVal*UP(UpAE_DEBLUR_GAIN))>>7 : (iAgcVal*iDeblurHssLmt)/iAgcLmt ;

			//GrpAe(GA_AGC_VAL) = iAgcVal;
			//GrpAe(GA_SHT_DEBLUR_OFST) = iShtValOfst;

			iShtVal = iHssLmt + iShtValOfst;
			iShtVal = MIN(iShtVal,(int)gnAeHssTVal);
			iShtVal = (MP(MpShutter)==MN_SHUT_FLICKER) ? AE_SHT_FLK : iShtVal;
			
			if((MP(MpShutter)==MN_SHUT_MNL)&&(gbWdrOn==MN_ON)) iShtVal = (int)LibAeShtLine(gnAeVtw>>MP(MpShutSpd));		// 180327 KSH

			iIrsVal = (model_Iris_DC) ?
							((MP(MpIris)==MN_LENS_DC) && (!AE_OUTDOOR_COND) && (gbWdrOn==MN_OFF)) ? MAX( (giIrsValBuf-(iAgcVal<<1)), 0) :
																																	 0	:
					  (model_Iris_P) ?																						 gnAeIrsTVal:
					  																										 gnAeIrsTVal;
			giAeValBuf = iAgcVal;
			gnAeState  = 2;
		}

		else {	// ---------------------------------------------------------------------------------------------// DSS status

			iShtVal = giAePos-(iIrsLmt+iAgcLmt);
			iShtVal = (MP(MpShutter)==MN_SHUT_FLICKER) ? MAX(iShtVal, (int)iHssLmt) 	:	// 13.10.21
   					  (AE_DEBLUR_COND) 				   ? MAX(iShtVal, (int)gnAeHssTVal) :
   					  									 MAX(iShtVal, (int)iHssLmt)		;

			iIrsVal  	= (model_Iris_DC) ? 0 			: // full open
											gnAeIrsTVal ; // full open

			iAgcVal = iAgcLmt;

			giAeValBuf = iShtVal;
			gnAeState  = 3;
		}
		// ----------------------------------------------------------------------------------------------------//
		
		// apply control val --------------

		if (gbWdrOn||gbWdrOnBuf2) {												// WDR Short /or fill Short to Long because turn off hunting
			wShtInt = AeSHT(giShtValBuf, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );												// Long
			AeAGC(giAgcValBuf);
			AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : ((model_Iris_DC) ? 0 : gnAeIrsTVal));
		}
		else {
			wShtInt = AeSHT(iShtVal, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );												// Long
			AeAGC(iAgcVal);
			AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : iIrsVal);
		}


	}	// End of // Short time, Long proc, Short apply


	//GrpAe(GA_IRS_VAL) = iIrsVal;
	//GrpAe(GA_SHT_INT) = wShtInt;
	//extern const BYTE Sens(gbAeHssFltTbl)[];
	//GrpAe(GA_HSS_FLT) = (wShtInt < 46) ? Sens(gbAeHssFltTbl)[wShtInt] : 0 ;

// -------------------------------------------------------------------------------
// WDR Control
	if(WdrCtrl(&wShtInt))
	{
		//if (gbWdrOnBuf2!=gbWdrOnBuf3) {SYNC_UPw(1); /*TxStr("sync_up_", 1,  "\n");*/}	// 141031 SMJ : 1080p30 WDR OFF image crack

		WDR_ONw(0);

		//gwWdrCnt = (gwWdrCnt==0 ) ? 0  : gwWdrCnt-1;

#if (model_float==1)
		gfAePosWdrflt = (iShtVal>0) ? _itof(iShtVal-1) : _itof(0);
#else
		gfAePosWdrflt = (iShtVal>0) ? (float)(iShtVal-1) : 0.f;
#endif
	}

// -------------------------------------------------------------------------------
	//GrpAe(GA_ERR_CHT_PRE) = (int)gfAePosflt;
	//GrpAe(GA_ERR_NIGHT) = giAePos;
	//GrpAe(GA_AGC_VAL_PRE) = iShtVal;


	giErr2d = giErr1d;
	giErr1d = iErr;

	giAePosLmtBuf = iAePosLmt;
	giShtValBuf = iShtVal;

	giAgcValBuf = iAgcVal;

	gbMnLensBuf	= MP(MpIris);
	//gbMnSysFreqBuf = MP(MpSysFreq);

	giCurAgc = iAgcVal;

// -------------------------------------------------------------------------------
// Monitoring
	DebugDisp2(gbAeDebugOn, Dec, "Tgt    ",  3, AE_DEBUG_X, iTgt, 4)
	DebugDisp2(gbAeDebugOn, Dec, "Cur    ",  4, AE_DEBUG_X, iCur, 4)

	DebugDisp2(gbAeDebugOn, Dec, "SliceLv",  5, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3SlicLvl : nAe2SlicLvl, 4)
	DebugDisp2(gbAeDebugOn, Dec, "ClipLv ",  6, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipLvl : nAe2ClipLvl, 4)

	//DebugDisp(gbAeDebugOn, Dec, "Pos_______", 12, AE_DEBUG_X, giAePos   	)
	//DebugDisp(gbAeDebugOn, Dec, "PosLmt____", 13, AE_DEBUG_X, iAePosLmt 	)
	//DebugDisp(gbAeDebugOn, Dec, "State_____", 14, AE_DEBUG_X, gnAeState   	)
	//DebugDisp(gbAeDebugOn, Dec, "AePosSta__", 15, AE_DEBUG_X, giAeValBuf  	)

#if 1
	if(gbWdrOn){
		if (gnLSflag==AeSHORT) {
				DebugDisp2(gbAeDebugOn, Dec, "WdrLtgt", AE_WDR_DEBUG_Y+0/*17*/, AE_DEBUG_X, iTgt, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLcur", AE_WDR_DEBUG_Y+1/*18*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrSsht", AE_WDR_DEBUG_Y+5/*22*/, AE_DEBUG_X, wShtInt, 4)

		} else {
				DebugDisp2(gbAeDebugOn, Dec, "WdrStgt", AE_WDR_DEBUG_Y+2/*19*/, AE_DEBUG_X, iTgt, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrScur", AE_WDR_DEBUG_Y+3/*20*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLsht", AE_WDR_DEBUG_Y+4/*21*/, AE_DEBUG_X, wShtInt, 4)
		}
	}
	else if ((gbWdrOn==MN_OFF)&&(gbWdrOnBuf2==MN_ON)) {
		DispClr(AE_WDR_DEBUG_Y+0, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+1, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+2, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+3, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+4, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+5, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+6, AE_DEBUG_X, 7+1+4);
	}
#endif

}


//*******************************************************************************
// ADV. AE
//*******************************************************************************
#if AE_CTRL_ADV

BYTE gbMpShtMin    = MN_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
BYTE gbMpDeblurMin = MN_MNL_SHUT_DIV1;		// 1125 line, 1/30 sec
BYTE gbMpShtMax    = MN_MNL_SHUT_DIV1;		// 1125 line, 1/30 sec

void ShutterMenuGet(void)
{
	if(MP(MpIris)==MN_LENS_MNL) {
		switch(MP(MpShtMode)) {
			case MN_SHT_NORMAL:	gbMpShtMin		= MN_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
								gbMpDeblurMin	= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbMpShtMax		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case MN_SHT_DEBLUR: gbMpShtMin		= MN_MNL_SHUT_DIV512;	// 2 line, 1/15000 sec
								gbMpDeblurMin	= /*MN_MNL_SHUT_DIV8*/UP(UpAE_DEBLUR_THRS_VAL);	// 140 line, 1/250 sec
								gbMpShtMax		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			default:// MN_SHT_CUSTOM
								gbMpShtMin		= MP(MpShtMin);
								gbMpDeblurMin	= MP(MpDeblurMin);
								gbMpShtMax		= MP(MpShtMax);
		}
	}
	else {// MP(MpIris) == MN_LENS_DC
		switch(MP(MpDcMode)) {
			case MN_DC_IN:		gbMpShtMin		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbMpDeblurMin	= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbMpShtMax		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case MN_DC_OUT:		gbMpShtMin		= /*MN_MNL_SHUT_DIV8*/UP(UpAE_OUTDOOR_THRS_VAL);	// 140 line, 1/250 sec
								gbMpDeblurMin	= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								gbMpShtMax		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			case MN_DC_DEBLUR:	gbMpShtMin		= /*MN_MNL_SHUT_DIV8*/UP(UpAE_DEBLUR_THRS_VAL);	// 140 line, 1/250 sec
								gbMpDeblurMin	= /*MN_MNL_SHUT_DIV8*/UP(UpAE_DEBLUR_THRS_VAL);	// 140 line, 1/250 sec
								gbMpShtMax		= MN_MNL_SHUT_DIV1;	// 1125 line, 1/30 sec
								break;
			default:// MN_DC_CUSTOM
								gbMpShtMin		= MP(MpShtMin);
								gbMpDeblurMin	= MP(MpDeblurMin);
								gbMpShtMax		= MP(MpShtMax);
		}
	}
}

void ShutterMenuSet(void)
{
	if(gbMpShtMin < gbMpShtMax) {
		ShutterMenuGet();
		return;
	}

	if(gbMpDeblurMin > gbMpShtMin) gbMpDeblurMin = gbMpShtMin;
	if(gbMpDeblurMin < gbMpShtMax) gbMpDeblurMin = gbMpShtMax;

	MP(MpShtMin) = gbMpShtMin;
	MP(MpDeblurMin) = gbMpDeblurMin;
	MP(MpShtMax) = gbMpShtMax;

#if(model_Iris_DC)
	if(MP(MpIris)==MN_LENS_MNL) MP(MpShtMode) = MN_SHT_CUSTOM;
	else MP(MpDcMode) = MN_DC_CUSTOM;
#else
	MP(MpShtMode) = MN_SHT_CUSTOM;
#endif
}

int ISPM ClipGet(const int iClipLvD)
{
	const int iClipLvN = (AE_SAT_OFF) ? iClipLvD :
		(ParAe(PA_TGT_NIGHT) ? (int)ParAe(PA_TGT_NIGHT) : 1024 - (MP(MpLSpotLv)<<4)) - (1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64)) ;

	return LtPos(iClipLvD, iClipLvN);
}

void ISPM TgtChg(void)
{
	static BYTE bMpBrightness = 0xff;
	static BYTE bMpBrightnessMin = 0xff;
	static BYTE bMpAdptAe = 0xff;
	gbTgtChg = bMpBrightness != MP(MpBrightness) || bMpBrightnessMin != MP(MpBrightnessMin) || bMpAdptAe != MP(MpAdptAe);
	bMpBrightness = MP(MpBrightness);
	bMpBrightnessMin = MP(MpBrightnessMin);
	bMpAdptAe = MP(MpAdptAe);
}

int ISPM TgtGet(const int aiTgtMax, const int aiTgtMin, const BYTE abAeStg)
{
	int iTgt = aiTgtMax;

	#define VAR_TGT_CLIP	(ParAe(PA_SLICE)==0 && ParAe(PA_CLIP)==0)

	if(VAR_TGT_CLIP) {
		if(MP(MpAdptAe) == MN_ADPT_AE_AGC) {
			iTgt = MAX(iTgt-(giCurAgc>>1), (iTgt>>1));
		}
		else if(MP(MpAdptAe) == MN_ADPT_AE_BR) {

			if(gbTgtChg) {
				switch(abAeStg) {
					case AE_STG_IRS: iTgtVal = aiTgtMax; break;
					case AE_STG_SHT: iTgtVal = aiTgtMax; break;
					case AE_STG_DBL: iTgtVal = aiTgtMin; break;
					case AE_STG_AGC: iTgtVal = aiTgtMin; break;
					case AE_STG_DSS: iTgtVal = aiTgtMin; break;
					case AE_STG_TGT: if(iTgtVal < aiTgtMin)		 iTgtVal = aiTgtMin;
									 else if(iTgtVal > aiTgtMax) iTgtVal = aiTgtMax;
									 break;
				}
			}

			/*iTgt = LibUtlInterp1D(giShtVal, SHT_MIN, SHT_MAX, iTgt, iTgt>>2);
			*aiTgtDly = (*aiTgtDly < iTgt) ? *aiTgtDly+1 :
					  (*aiTgtDly > iTgt) ? *aiTgtDly-1 :
										 *aiTgtDly   ;
			iTgt = *aiTgtDly;*/

			//if(bMpBrightness != MP(MpBrightness) || bMpAdptAe != MP(MpAdptAe)) {
			//	*iTgtVal = iTgt;
			//	//IIRI(iTgtIIR, iTgt);
			//}
			iTgt = iTgtVal;

			//iTgt = LibUtlInterp1D(MAX(giShtVal,SHT_MIN+10), SHT_MIN+10, SHT_MAX, iTgt, iTgt>>2);
		}
	}

	return iTgt;
}

int ISPM TgtMinGet(const int aiTgt, const BYTE abV)
{
	return (abV < 10) ? ((aiTgt*12*abV)>>9) + 4 :
						((aiTgt * (38*(abV-10)+128))>>9);//(iTgtMax>>AE_TGT_MIN_BIT);
}

int ISPM TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin)
{
#if 1
	UNUSED(aiTgtMin);
	const int iTgtSpotBg0 = ((aiTgt*aiTgt)>>11) + (aiTgt>>1) - 6;
	return (abV<10) ? aiTgt - (aiTgt-iTgtSpotBg0)*abV/10 : (iTgtSpotBg0*(20-abV))/10 ;
#else
	const int iTgtDiff = aiTgt - aiTgtMin;
	int iTgtSpotBg0 = abV * iTgtDiff;
	iTgtSpotBg0 = iTgtDiff - (((iTgtSpotBg0<<2)-iTgtSpotBg0+32)>>6);
	if(iTgtSpotBg0 < 0) iTgtSpotBg0 = 0;
	return iTgtSpotBg0;
#endif
}

void ISPM AeAdv(void)
{
	const UINT 	nAe2Sum1    = AE2_SUM1_LOCKr;
	const UINT 	nAe2Sum2    = AE2_SUM2_LOCKr;
	const UINT 	nAe3Sum1    = AE3_SUM1_LOCKr;
	const UINT 	nAe3Sum2    = AE3_SUM2_LOCKr;
	const UINT 	nAe2ClipCnt = AE2_CLCNT_LOCKr;
	const UINT 	nAe2SlicCnt = AE2_SLCNT_LOCKr;
	const UINT 	nAe3ClipCnt = AE3_CLCNT_LOCKr;
	const UINT 	nAe3SlicCnt = AE3_SLCNT_LOCKr;

	const UINT	nAe2PxCnt	= ((AE2_VEPr - AE2_VSPr)+1) * ((AE2_HEPr - AE2_HSPr)+1);
	const UINT	nAe3PxCnt	= ((AE3_VEPr - AE3_VSPr)+1) * ((AE3_HEPr - AE3_HSPr)+1);
	const UINT	nAe2ClipLvl	= AE2_CLIPr;
	const UINT	nAe2SlicLvl	= AE2_SLICEr;
	const UINT	nAe3ClipLvl	= AE3_CLIPr;
	const UINT	nAe3SlicLvl	= AE3_SLICEr;

	int		/*iTgt=0,*/ iCur=0, iErr=0;
	WORD	wShtInt=0;		// SHT

	static int iShtSVal=0;

	static BYTE gbMpIris=0xff;
	static BYTE gbMpDcMode=0xff;
	static UINT gnMnLensChg=0;
	if((gbMpIris!=MP(MpIris))||(gbMpDcMode!=MP(MpDcMode))) gnMnLensChg = (FPS_VDO<<2);		// IRIS 설정 메뉴 변경 시 4초 동안 IRIS Center 업데이트 하지 않음
	else if(gnMnLensChg) gnMnLensChg--;
	gbMpIris = MP(MpIris);
	gbMpDcMode = MP(MpDcMode);

	TgtChg();	// ErrMgn() & TgtGet() 호출전에 먼저 실행되야 함

	//GrpAe(GA_ERR_NIGHT_CNT) = giCurAgc;

	AeODM();

	gnLSflag = LS_FLGr;

	//--------------------------------------------------------------------------------
	// Freq adjust
	FreqAdjust();

	// LED ON/OFF
	if (((MP(MpTdn)==MN_TDN_NIGHT)||(MP(MpTdn)==MN_TDN_EXTERN)) &&	MP(MpLedOn) && (gnTdnChkOut==TDN_NIT))	AE_LED_ON
	else 																									AE_LED_OFF

	//HLMASK_ONw(!ParAe(PA_SAT_OFF));
	//HLMASK_THw(AE2_CLIPr>>2);

	const BYTE bWdrOn = (gbWdrOn||gbWdrOnBuf2);

	//static BYTE bWdrOffCnt = 0;
	//if((gbWdrOnBuf==MN_OFF) && (gbWdrOnBuf2==MN_ON)) bWdrOffCnt = FPS_VDO>>1;
	//else if(bWdrOffCnt) bWdrOffCnt--;

	static BYTE bWdrOnCnt = 0;
	if((bWdrOn==MN_ON) && (gbWdrOnBuf==MN_OFF)) bWdrOnCnt = MAX(gbSensShtDly, gbSensAgcDly)+1;
	else if(bWdrOnCnt) bWdrOnCnt--;

	extern BYTE gbAdnr_WaitCnt;
	if(MP(MpBackLight) == MN_BL_WDR || gbWdrOn || gbWdrOnBuf2 || (gbAdnr_WaitCnt<3)/*gbAdnr_Excute*//*DNR3D_ONr*/) {
		gbDssRatioLmt = 1;
	}
	else {
		switch (MP(MpDss)) {
			case MN_DSS_OFF	 :	gbDssRatioLmt = 1; break;	// x1 (fix)
			case MN_DSS_X2	 :	gbDssRatioLmt = 2; break;	// x2
			case MN_DSS_X4	 :	gbDssRatioLmt = 4; break;	// x4
			case MN_DSS_X8	 :	gbDssRatioLmt = 8; break;	// x8
			case MN_DSS_X16	 :	gbDssRatioLmt =16; break;	// x16
			case MN_DSS_X32  :	gbDssRatioLmt =32; break;	// x32
			case MN_DSS_X64  :	gbDssRatioLmt = MN_FPS_HI ? 64 : 32; break;	// x64
		}
	}

	/*const int*/ iTgtMax = TgtMaxGet(0,0);

	/*const int*/ iTgtMin = (bWdrOn) ? iTgtMax : TgtMinGet(iTgtMax, MP(MpBrightnessMin)) ;


	const int iErrMgn = ErrMgn(AE_ERR_MGN, AE_ERR_MGN>>1, AE_ERR_MIN, 16);

#ifdef ENABLE_PA
	ParAe(PA_AGC_GAIN_MIN_BIT) = MP(MpSmallSpeed);
#else
	int iShtUpSpd, iShtDnSpd, iIrsUpSpd, iIrsDnSpd;
#endif

// normalization of control speed of Shutter (Sensor의 실제 밝기가 클수록 Shutter 제어 Speed는 낮아짐)
#if (model_Sens==SENS_IMX327) || (model_Sens==SENS_OV2718)
	#define	MpShtUpSpeedUnit	3000	// MAX:9683 = SHT_MAX_GAIN / (21*10)
	#define	MpShtDownSpeedUnit	1600/*3000*/	// MAX:9683
#else
	#define	MpShtUpSpeedUnit	3500/*6000*/	// MAX:9683
	#define	MpShtDownSpeedUnit	3500/*6000*/	// MAX:9683
#endif

// normalization of control speed of AGC (AGC의 실제 최대 밝기가 크고 AGC_MAX값이 작을수록 AGC 제어 Speed는 낮아짐)
#if (model_Sens==SENS_OV2718)
	#define	MpAgcUpSpeedUnit	1920	// MAX:6091 = AGC_MAX_GAIN / (21*10)
	#define	MpAgcDownSpeedUnit	1920	// MAX:6091
#elif (model_Sens==SENS_AR0237CSH)
	#define	MpAgcUpSpeedUnit	512		// MAX:6091
	#define	MpAgcDownSpeedUnit	512		// MAX:6091
#elif (model_Sens==SENS_IMX327)
	#define	MpAgcUpSpeedUnit	210/*200*//*900*/		// MAX:6091
	#define	MpAgcDownSpeedUnit	128/*20*//*90*/		// MAX:6091
#else
	#define	MpAgcUpSpeedUnit	210/*128*/		// MAX:6091
	#define	MpAgcDownSpeedUnit	128		// MAX:6091
#endif

	BYTE bMpAgcSpeed = MP(MpAgcSpeed)+1;

	ParAe(PA_AGC_UP_SPEED)     = (MP(MpAgcSpeed)+1)*MpAgcUpSpeedUnit;
	ParAe(PA_AGC_DOWN_SPEED)   = (MP(MpAgcSpeed)+1)*(MpAgcDownSpeedUnit*10);

	ParAe(PA_SHT_UP_SPEED)     = (MP(MpOpenSpeed)+1)*MpShtUpSpeedUnit;
	ParAe(PA_SHT_DOWN_SPEED)   = (MP(MpCloseSpeed)+1)*(MpShtDownSpeedUnit*10);

	ParAe(PA_IRS_OPEN_SPEED)   = MP(MpIrsSpeed)*40/*100*/+24;
	ParAe(PA_IRS_CLOSE_SPEED)  = MP(MpIrsSpeed)*40/*100*/+24;

	if(MP(MpAllSpeed)>10) {
		#define AeSpeedAdjUp(V)		(V) = (V)*(MP(MpAllSpeed)-10)/10 + (V)
		AeSpeedAdjUp(bMpAgcSpeed);
		AeSpeedAdjUp(ParAe(PA_AGC_UP_SPEED));
		AeSpeedAdjUp(ParAe(PA_AGC_DOWN_SPEED));
		AeSpeedAdjUp(ParAe(PA_SHT_UP_SPEED));
		AeSpeedAdjUp(ParAe(PA_SHT_DOWN_SPEED));
		AeSpeedAdjUp(ParAe(PA_IRS_OPEN_SPEED));
		AeSpeedAdjUp(ParAe(PA_IRS_CLOSE_SPEED));
	}
	else {
		#define AeSpeedAdjDn(V,MIN)	(V) = ((V)-(MIN))*MP(MpAllSpeed)/10 + (MIN)
		AeSpeedAdjDn(bMpAgcSpeed,				1);
		AeSpeedAdjDn(ParAe(PA_AGC_UP_SPEED),    MpAgcUpSpeedUnit);
		AeSpeedAdjDn(ParAe(PA_AGC_DOWN_SPEED),  MpAgcDownSpeedUnit*10);
		AeSpeedAdjDn(ParAe(PA_SHT_UP_SPEED),    MpShtUpSpeedUnit);
		AeSpeedAdjDn(ParAe(PA_SHT_DOWN_SPEED),  MpShtDownSpeedUnit*10);
		AeSpeedAdjDn(ParAe(PA_IRS_OPEN_SPEED),  24);
		AeSpeedAdjDn(ParAe(PA_IRS_CLOSE_SPEED), 24);
	}

	LtPosThSet();

// -------------------------------------------------------------------------------
// Input condition adj

	if(gbWdrOn){	// WDR mode --------------------------------------------------

		// CurWDR() 실행전에 gnLSflag 설정되어야 함
		if(MP(MpWdrBlc)) {
			CurWDR(&iCur, &iTgt, &iErr, nAe3ClipLvl, nAe3ClipCnt, nAe3Sum2, nAe3Sum1, nAe3PxCnt);
		}
		else {
			CurWDR(&iCur, &iTgt, &iErr, nAe2ClipLvl, nAe2ClipCnt, nAe2Sum2, nAe2Sum1, nAe2PxCnt);
		}

#if model_WDR_ROI
		ROI_WDR();
#endif

	}
	else if (MP(MpBackLight)==MN_BL_BLC) {	// BLC Mode -------------------------------------

		iCur = (int)(((nAe3SlicCnt*nAe3SlicLvl) + nAe3Sum1 + (nAe3ClipCnt*nAe3ClipLvl)) / nAe3PxCnt);		// 170221 KSH

		iTgt = TgtGet(iTgtMax, iTgtMin, gbAeStg);

		iErr = iTgt - iCur;

		// Set Slice
		const int iSliceLv = CurBg(iTgt, nAe2SlicLvl, nAe2SlicCnt, nAe2ClipLvl, nAe2ClipCnt, nAe2Sum1, nAe2PxCnt, UP(UpAE_TGT_OFST));

		AE2_SLICEw(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);
		AE3_SLICEw(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);

		// Set Clip
		const int iClipLvD = MIN(iTgt + UP(UpAE_CLIP_OFST), 0x3ff);
		const int iClipLv = ClipGet(iClipLvD);

		CurSat(nAe3ClipLvl, nAe3ClipCnt, nAe3Sum2, (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64));

		AE2_CLIPw(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLvD);
		AE3_CLIPw(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLv);
	}
	else {		// Normal Mode -------------------------------------------------------

		const int iAe2ClipCnt = (MP(MpBackLight)==MN_BL_HLC) ? LibUtlInterp1D(MP(MpHlMaskThrs)+8, 0, 28, 0, nAe2ClipCnt) : (int)nAe2ClipCnt ;

		iCur = (int)(((nAe2SlicCnt*nAe2SlicLvl) + nAe2Sum1 + (iAe2ClipCnt*nAe2ClipLvl)) / nAe2PxCnt);

		iTgt = TgtGet(iTgtMax, iTgtMin, gbAeStg);
		
		iErr = iTgt - iCur;

		// Set Slice
		const int iSliceLv = CurBg(iTgt, nAe3SlicLvl, nAe3SlicCnt, nAe3ClipLvl, nAe3ClipCnt, nAe3Sum1, nAe3PxCnt, UP(UpAE_TGT_OFST));

		AE2_SLICEw(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);
		AE3_SLICEw(ParAe(PA_SLICE) ? (int)ParAe(PA_SLICE) : iSliceLv);

		// Set Clip
		const int iClipLvD = MIN(iTgt + UP(UpAE_CLIP_OFST), 0x3ff);
		const int iClipLv = ClipGet(iClipLvD);

		CurSat(nAe2ClipLvl, nAe2ClipCnt, nAe2Sum2, (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64));

		AE2_CLIPw(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLv);
		AE3_CLIPw(ParAe(PA_CLIP) ? (int)ParAe(PA_CLIP) : iClipLvD);
	}

	//GrpAe(GA_CUR) = iCur;
	//GrpAe(GA_CLIP_CNT_OFF) = iClipCntOff;
	//GrpAe(GA_CLIP_CNT_ON) = iClipCntON;
	//GrpAe(GA_CLIP_CNT) = nAe2ClipCnt;
	//GrpAe(GA_CUR_NIGHT) = iCurSpot;
	//GrpAe(GA_ERR_NIGHT) = iErrSpot;


#if 1//model_ISP_Save == 1
	static BYTE bInitAe = /*(model_ISP_Save == 1) ? 3 :*/ 2;
	if(bWdrOnCnt) bInitAe = bWdrOnCnt;
	if(bInitAe) {
		//TxStrDec("Ae X:", iCur, 4);
		//bInitAe--;
		//giCurAgc = AGC2POS(giAgcVal);
		//AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );
		//AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
		//goto AeMonitoring;
	}
	else {
		static BYTE bDispCnt = 5;
		if(bDispCnt) {
			//TxStrDec("Ae O:", iCur, 4);
			bDispCnt--;
		}
	}
#else
	#define bInitAe 0
#endif

// -------------------------------------------------------------------------------
// Status mode
	if(gbWdrOn&&(gnLSflag==AeLONG)){	// ============================================ // Long time, Short proc, Long apply

		const int iShtSpd = (iErr<0) ? (ParAe(PA_SHT_DOWN_SPEED)  ? ParAe(PA_SHT_DOWN_SPEED)  : 1200) : (ParAe(PA_SHT_UP_SPEED)   ? ParAe(PA_SHT_UP_SPEED)   : 200 );

		//iShtSpd = LibUtlInterp1D ( MINMAX(giCurAgc, UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC, iShtSpd, iShtSpd>>3);	// Sensor AGC
		//if(iErr < (-iErrMgn-1)) iErr = -iErrMgn-1 - LibUtlInterp1D ( MINMAX(giCurAgc, UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC, (iErr+iErrMgn+1)>>0, (iErr+iErrMgn+1)>>10);	// Sensor AGC
		GrpAe(GA_ERR_DAY_ORI) = iErr;

#if 1
	#if (model_Sens==SENS_IMX327) || (model_Sens==SENS_OV2718)
		#define WDR_SHORT_MIN1	/*8*/ParAe(PA_WDR_SHORT_MIN1)	// 너무 빠른 Shutter에서는 AGC를 써도 SUM2가 크게 변화하여 헌팅이 발생 -> 1에서 8로 변경
		#define WDR_SHORT_MIN2	40
		const int iShtSValMax = (giShtVal)>>1;					// WDR Short limit 50% of Long
	#else
		#define WDR_SHORT_MIN1	20
		#define WDR_SHORT_MIN2	40
		const int iShtSValMax = ((giShtVal<<1) + giShtVal)>>2;	// WDR Short limit 75% of Long
	#endif

		const int iWdrSLmt = LibUtlInterp1D ( MINMAX(giShtVal, (int)(gnAeVtw>>1)/*250*/, (int)(gnAeVtw-1)), gnAeVtw>>1/*250*/, gnAeVtw-1, WDR_SHORT_MIN1, WDR_SHORT_MIN2);	// Sht limit

		const int iShtSMin = MIN(iShtSValMax, iWdrSLmt);

		//if(iShtSValMax<iWdrSLmt && iErr < (-iErrMgn-1)) iErr = (-iErrMgn-1) - ((iErr+iErrMgn+1)>>3);

		// ShtCtrl(1, ...)에서 abAgcOn를 1로 사용하기 위해서는 AgcCtrl()의 giAgcVal도 2개가 있어야 함 -> 현재는 gbVHssSAgc + giAgcVal(Long Shutter) 로 동작 -> WDR 사용 시 AGC_MIN 적용하지 않음
		// 너무 빠른 Shutter에서는 AGC를 써도 SUM2가 크게 변화하여 헌팅이 발생 -> iErrMgn에 +4 적용
		if(bInitAe==0) ShtCtrl(1, iErr, iErrMgn+4, (iShtSpd/*>>NO_EST_SPD_DOWN_BIT*/), MAX(iShtSMin,1), giShtVal-1, 0, 0, 0, 0);	// over than short limit, under than long

		const int iWdrAgcWgt = LibUtlInterp1D ( MINMAX(giCurAgc, UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC), UP(UpAE_WDR_ON_AGC), AE_WDR_OFF_AGC, AE_WDR_MAX_WGT, UP(UpAE_WDR_MIN_WGT));	// Sensor AGC
		iShtSVal = WdrAgcWgt((bInitAe==0) && (UP(UpAE_WDR_ON_AGC) < giCurAgc), iWdrAgcWgt, 30, 18);	// CAUTION ! -> iShtSVal 와 gbVHssSAgc 는 비동기

		if(iShtSVal > iShtSValMax) iShtSVal = iShtSValMax;//SHT_SET1(iShtSValMax);
#else
		iShtSVal = SHT_SET1((giShtVal>1) ? giShtVal-1 : 1);
#endif

		// apply control val -------------- // Long
		wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );
		AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);
		//AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : ((model_Iris_DC) ? 0 : AE_IRS_STAT_Max));

		//GrpAeF(GA_WDR_SHORT_POS) = gfAePosWdrflt;
		//GrpAe(GA_WDR_LONG_POS) = giShtValBuf;
		//GrpAe(GA_WDR_HSS_T_POS) = gnAeHssTVal;
		//GrpAe(GA_WDR_SHORT_LMT_POS) = iWdrSLmt;
		//GrpAe(GA_WDR_AGC_WGT) = iWdrAgcWgt;

		AeMon(AE_SAT_OFF, iErrMgn, iCur, iTgt, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	}
	else {	// ======================================================================== // Short time, Long proc, Short apply

		if(ParAe(PA_SHT_MANUAL)) {
			SHT_MAX = (int)ParAe(PA_SHT_MANUAL);
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else if(MP(MpShutter)==MN_SHUT_MNL) {
			SHT_MAX = (int)(gnAeVtw>>MP(MpShutSpd));
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else if(MP(MpShutter)==MN_SHUT_FLICKER) {
			SHT_MAX = (MP(MpSysFreq)==MN_SYSFREQ_60) ? 675 : 469;
			SHT_MIN = SHT_MAX;
			SHT_DBL_MAX = SHT_MAX;
		}
		else {	// MP(MpShutter)==MN_SHUT_AUTO
			SHT_MIN = gnAeVtw>>gbMpShtMin;
			SHT_MAX = gnAeVtw>>gbMpDeblurMin;
			SHT_DBL_MAX = gnAeVtw>>gbMpShtMax;
		}

		if(ParAe(PA_AGC_MANUAL)) {
			AGC_MAX = ParAe(PA_AGC_MANUAL);
			AGC_MIN = ParAe(PA_AGC_MANUAL);
			AGC_DBL_MAX = AGC_MIN;
		}
		else {
			gnAeAgcTVal = AE_GAIN_TOTAL + AE_GAIN_TOTAL2;	// gnAeAgcTVal -> gnAgcLim -> TDN에서 사용

			const int iAgcLmt = (MP(MpAgc)>=255) ? (int)gnAeAgcTVal :
												   (int)((((gnAeAgcTVal-AE_GAIN_TGT_OFST)*MP(MpAgc))/255)+AE_GAIN_TGT_OFST);
			AGC_MAX = POS2AGC(iAgcLmt+1);

			//AGC_MIN = (bWdrOn) ? 0 : ((AGC_MAX*MP(MpAgcMin))>>5);
			const int iAgcMin = (MP(MpAgcMin)>=255) ? (int)(gnAeAgcTVal>>2) :
													  (int)(((gnAeAgcTVal)*MP(MpAgcMin))/(255<<2));
			AGC_MIN = (bWdrOn) ? 0 : POS2AGC(iAgcMin+1);
			AGC_MIN = MIN(AGC_MIN, AGC_MAX);


			if(SHT_MAX < SHT_DBL_MAX) {
				AGC_DBL_MAX = POS2AGC(((SHT_DBL_MAX-SHT_MAX)<<(7-2))/UP(UpAE_DEBLUR_GAIN));	// 7:UP(UpAE_DEBLUR_GAIN)=128 -> x1 , 2:4SHT=1AGCPOS,		Sensor AGC
				if(AGC_DBL_MAX > AGC_MAX) AGC_DBL_MAX = AGC_MAX;
			}
			else {
				AGC_DBL_MAX = AGC_MIN;
			}
		}


		const BYTE bDeblurOn = (SHT_MAX < SHT_DBL_MAX) && (AGC_MIN < AGC_DBL_MAX);
		const int iShtMax_Agc = (bDeblurOn) ? SHT_DBL_MAX : SHT_MAX ;
		const int iAgcMin_Agc = (bDeblurOn) ? AGC_DBL_MAX : AGC_MIN ;
		const int iShtMax_Dss = (gbDssRatioLmt<2) ? gnAeVtw : (gnAeVtw*gbDssRatioLmt)-1;
		const BYTE bAgcOn = (iAgcMin_Agc < AGC_MAX);
		const BYTE bShtOn = (SHT_MIN < SHT_MAX);
		const BYTE bIrsOn = (MP(MpIris)!=MN_LENS_MNL) && (ParAe(PA_IRS_MANUAL)==0);
		const BYTE bDssOn = (MP(MpShutter)!=MN_SHUT_MNL) && (iShtMax_Agc < iShtMax_Dss);
		const BYTE bTgtOn = (bWdrOn==MN_OFF);
		//const BYTE bEstOn = (bWdrOn==MN_OFF) && (bWdrOffCnt==0);
		const BYTE bEstOn = (bWdrOnCnt==0);
		
		if(gbWdrOnBuf2==MN_ON) SetAeDevDly(gbSensShtDly>>1, gbSensAgcDly>>1);
		else				   SetAeDevDly(gbSensShtDly, gbSensAgcDly);

		if(bInitAe) goto AeCtrl;

	// Status manage ----------------------------------------------------------------------------------------------------------------------------
		//if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = iErr;

		if(gbAeStg==AE_STG_IRS) {
			if(bIrsOn) {
				iErr = ErrNor(iErr, iCur, 0, 1, 0, 0);
			}
			else {
				iErr = ErrNor(iErr, iCur, 0, 0, 1, 0);
				giIrsPos = AE_IRS_STAT_Max;
				gbAeStg = AE_STG_SHT;
			}
		}
		else {
			iErr = ErrNor(iErr, iCur, 0, 0, 1, 0);
		}

		SatCtrl(AE_SAT_OFF, &iErr, iErrMgn, (MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipCnt : nAe2ClipCnt);

		const int iIrsSpd = (iErr<0) ? (ParAe(PA_IRS_CLOSE_SPEED) ? ParAe(PA_IRS_CLOSE_SPEED) : 1024) : (ParAe(PA_IRS_OPEN_SPEED) ? ParAe(PA_IRS_OPEN_SPEED) : 1024);
		/*const*/ int iShtSpd = (iErr<0) ? (ParAe(PA_SHT_DOWN_SPEED)  ? ParAe(PA_SHT_DOWN_SPEED)  : 1200) : (ParAe(PA_SHT_UP_SPEED)   ? ParAe(PA_SHT_UP_SPEED)   : 200 );
		/*const*/ int iAgcSpd = (iErr<0) ? (ParAe(PA_AGC_DOWN_SPEED)  ? ParAe(PA_AGC_DOWN_SPEED)  : 200 ) : (ParAe(PA_AGC_UP_SPEED)   ? ParAe(PA_AGC_UP_SPEED)   : 20  );
		
		//extern BYTE bErrNightCnt;
		//extern BYTE bErrOfsCnt;
		//static BYTE bErrNightCnt1d = 0xff;
		//static BYTE bErrOfsCnt1d   = 0xff;
		const BYTE bDlyOn = 0;//bWdrOffCnt/*0*/;//(bErrNightCnt1d != bErrNightCnt) || (bErrOfsCnt1d != bErrOfsCnt);
		//if(bEstOn && ((bErrNightCnt1d != bErrNightCnt) || (bErrOfsCnt1d != bErrOfsCnt))) bEstOn = 0;
		//bErrNightCnt1d = bErrNightCnt;
		//bErrOfsCnt1d = bErrOfsCnt;

		//if (/*bWdrOn ||*/ (bEstOn==0)) {
		//	iShtSpd >>= NO_EST_SPD_DOWN_BIT;
		//	iAgcSpd >>= NO_EST_SPD_DOWN_BIT;
		//}
		
		const BYTE BRIGHT_ON = iErr > iErrMgn;
		const BYTE DARKEN_ON = iErr < -iErrMgn;
		
		switch(gbAeStg) {
			case AE_STG_IRS :	// IRIS
				IRS_RUN(iErr, iErrMgn, iIrsSpd);
				SHT_SET(SHT_MIN);
				AGC_SET(AGC_MIN);
				
				if(BRIGHT_ON && giIrsPos>=AE_IRS_STAT_Max && gbIrsDly==0) {
					if(bShtOn) gbAeStg = AE_STG_SHT;
					else	   gbAeStg = AE_STG_TGT;
				}
				break;
			case AE_STG_SHT :	// SHT
				ShtCtrl(0, iErr, iErrMgn, iShtSpd, SHT_MIN, SHT_MAX, bEstOn, bDlyOn, 1, 0/*iShtManual*/);
				//AGC_SET(AGC_MIN);		// ShtCtrl()의 abAgcOn=1로써 AGC까지 제어하므로 주석처리

				if(DARKEN_ON && giShtVal<=SHT_MIN && gbShtDly==0) {
					if(bIrsOn) gbAeStg = AE_STG_IRS;
				}
				if(BRIGHT_ON && giShtVal>=SHT_MAX && gbShtDly==0) {
					gbAeStg = AE_STG_TGT;
				}
				break;
			case AE_STG_TGT :
				SHT_SET(SHT_MAX);
				AGC_SET(AGC_MIN);

				if(DARKEN_ON) {	// iErr < 0 : Close Ctrl : AGC -> SHT : iTgt -> iTgtMax
					//iTgtVal++;
					iTgtVal -= MIN(iErr>>1,-1);
					if((bTgtOn==0) || (iTgtVal >= iTgtMax)) {
						iTgtVal = iTgtMax;
						if(bShtOn)		gbAeStg = AE_STG_SHT;
						else if(bIrsOn) gbAeStg = AE_STG_IRS;
					}
				}
				if(BRIGHT_ON) {	// iErr > 0 : Open Ctrl : SHT -> AGC : iTgt -> iTgtMin
					//iTgtVal--;
					iTgtVal -= MAX(iErr>>1,1);
					if((bTgtOn==0) || (iTgtVal <= iTgtMin)) {
						iTgtVal = iTgtMin;
						if(bDeblurOn)	gbAeStg = AE_STG_DBL;
						else if(bAgcOn) gbAeStg = AE_STG_AGC;
						else if(bDssOn) gbAeStg = AE_STG_DSS;
					}
				}
				break;
			case AE_STG_DBL :
				ShtCtrl(0, iErr, iErrMgn, ((iShtSpd * (10 + bMpAgcSpeed) / 10)>>2)+1, SHT_MAX, SHT_DBL_MAX, bEstOn, bDlyOn, 0, 0/*iShtManual*/);
				const int iDeblurAgc = ((bDeblurOn) ? (AGC_DBL_MAX - AGC_MIN) * (giShtVal - SHT_MAX) / (SHT_DBL_MAX - SHT_MAX) : 0) + AGC_MIN;
				AGC_SET(MINMAX(iDeblurAgc, AGC_MIN, AGC_DBL_MAX));
				
				if(DARKEN_ON && giShtVal<=SHT_MAX && gbShtDly==0 && gbAgcDly==0) {
					gbAeStg = AE_STG_TGT;
				}
				if(BRIGHT_ON && giShtVal>=SHT_DBL_MAX && gbShtDly==0 && gbAgcDly==0) {
					if(bAgcOn)		gbAeStg = AE_STG_AGC;
					else if(bDssOn)	gbAeStg = AE_STG_DSS;
				}
				break;
			case AE_STG_AGC :	// AGC
				SHT_SET(iShtMax_Agc);
				AgcCtrl(iErr, iErrMgn, iAgcSpd, iAgcMin_Agc, AGC_MAX, bEstOn, bDlyOn, 0/*ParAe(PA_AGC_MANUAL)>0*/, 0/*ParAe(PA_AGC_MANUAL)*/);

				if(DARKEN_ON && giAgcVal <= iAgcMin_Agc && gbAgcDly==0) {
					if(bDeblurOn) gbAeStg = AE_STG_DBL;
					else		  gbAeStg = AE_STG_TGT;
				}
				if(BRIGHT_ON && giAgcVal >= AGC_MAX && gbAgcDly==0) {
					if(bDssOn) gbAeStg = AE_STG_DSS;
				}
				break;
			case AE_STG_DSS :	// AGC
				if(bDssOn) ShtCtrl(0, iErr, iErrMgn, iShtSpd, iShtMax_Agc, iShtMax_Dss, bEstOn, bDlyOn, 0, 0/*iShtManual*/);
				AGC_SET(AGC_MAX);
				
				if((bDssOn==0) || (DARKEN_ON && giShtVal <= iShtMax_Agc && gbShtDly==0)) {
					if(bAgcOn)		   gbAeStg = AE_STG_AGC;
					else if(bDeblurOn) gbAeStg = AE_STG_DBL;
					else			   gbAeStg = AE_STG_TGT;
				}
				break;
		}

		//GrpAe(GA_ERR_DAY) = AGC_DBL_MAX;
		if(gbWdrOn==0) GrpAe(GA_ERR_DAY_ORI) = iErr;
		//GrpAe(GA_ERR) = iErr;
		GrpAe(GA_ERR) = (AE_SAT_OFF) ? iCur : iCurBg ;
		//GrpAe(GA_SHT_DLY) = gbShtDly;
		//GrpAe(GA_SHT_VAL) = giShtVal;
		//GrpAe(GA_AGC_VAL) = giAgcVal/*iErr*//*giShtPos*/;
		//GrpAe(GA_ERR_CHT_PRE) = iCurSpotEst>>IIG;
		//GrpAe(GA_ERR_CHT_PRE) = giIrsPos;
		GrpAe(GA_ERR_NIGHT) = gbAeStg*100 + gbIrsDly*1000 + gbAgcDly*10 + gbShtDly;

		// ------------------------------------------------------------------------------------------------------------ //
AeCtrl:
		giCurAgc = AGC2POS(giAgcVal);

		if (bWdrOn) {												// WDR Short /or fill Short to Long because turn off hunting
			wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : iShtSVal, 0/*(MP(MpShutter)==MN_SHUT_MNL)*/, 0/*(gnAeVtw>>MP(MpShutSpd))*/ );
			AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : (((gbAeStg==AE_STG_SHT)&&(giShtSVal==iShtSVal)) ? gbVHssSAgc : 0) + giAgcVal);
			//AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : ((model_Iris_DC) ? 0 : AE_IRS_STAT_Max));
		}
		else {
			wShtInt = AeSHT0(ParAe(PA_SHT_MANUAL) ? (int)ParAe(PA_SHT_MANUAL) : giShtVal, (MP(MpShutter)==MN_SHUT_MNL), (gnAeVtw>>MP(MpShutSpd)) );
			AeAGC0(ParAe(PA_AGC_MANUAL) ? (int)ParAe(PA_AGC_MANUAL) : ((gbAeStg==AE_STG_SHT) ? gbVHssAgc : 0) + giAgcVal);		
		}
			

			// ------------------------------------------------------------------------------------------------------------ //
			static int iShtValDly = 0;
			if(iShtValDly < giShtVal) iShtValDly++;
			else					  iShtValDly = giShtVal;

			static int iTgtValDly = 0;
			if(iTgtValDly > iTgtVal) iTgtValDly--;
			else					 iTgtValDly = iTgtVal;


			int iIrsVal = 0;
		#ifdef ENABLE_PA
			if(ParAe(PA_IRS_MANUAL)) {	// for IRIS test
				static BYTE bIrsCnt = 0;
				iIrsVal = (giIrsCent>>6)/*0x270*/ + (int)ParAe(PA_IRS_MANUAL) - 1000;
				if(ParAe(PA_IRS_MANUAL) != 1000 && bIrsCnt==0) bIrsCnt = ParAe(PA_IRS_CNT)+1;
				
				if(bIrsCnt) bIrsCnt--;
				if(bIrsCnt==0) ParAe(PA_IRS_MANUAL) = 1000;
			} else
		#endif
			{
				// increse open step
				static BYTE iIncreseOpenCnt = 0;
				const int iIncreseOpen = ( ((iTgtMax-iTgtValDly)>>1) + ((bShtOn || bDeblurOn) ? (iShtValDly - SHT_MIN) : 0) + ((gbAeStg==AE_STG_AGC) ? ((giAgcVal - iAgcMin_Agc)<<2) : 0) ) ;
				iIrsVal = (MP(MpIris)==MN_LENS_MNL) || (gbAeStg==AE_STG_DSS) ? 0 :
						  giIrsValBuf - iIncreseOpen;	// gbAeStg==AE_STG_IRS 일 때 IRIS를 제어하고, 다른 Stage일 때 IRIS를 0(Open)으로 제어하면 IRIS 다시 제어 시 IRIS가 튀는(?) 현상 발생하므로 increse open step 필요

				if(giIrsValBuf && (gbAeStg!=AE_STG_IRS))
				{
					if(iIncreseOpen) iIncreseOpenCnt++;
					if(iIncreseOpenCnt==(FPS_VDO>>2)) {	// 초당 4씩 giIrsValBuf 감소 - 너무 빠르게 감소하면 다른 Stage일 때 IRIS를 0(Open)으로 제어하는 것과 동일하게 IRIS가 튀는 현상 발생하므로 천천히 감소해야 함
						iIncreseOpenCnt = 0;
						giIrsValBuf--;
					}
				}
			}

			iIrsVal = MINMAX(iIrsVal, 0, 0x3fe);
			AeIRS(MP(MpDefDet) ? AE_IRS_CLS_Max : iIrsVal);
		//}

		AeMon(AE_SAT_OFF, iErrMgn, iCur, iTgt, bIrsOn, bShtOn, bTgtOn, bDeblurOn, bAgcOn, bDssOn, AE_IRS_STAT_Max, iShtMax_Agc, iAgcMin_Agc, POS2AGC(gnAeAgcTVal+1), iShtMax_Dss, MP(MpDss));

		// -------------------------------------------------------------------------------
		// Ae State Auto Save
		if(bInitAe) bInitAe--;
	}

	//GrpAe(GA_ERR_DAY) = iTgtVal/*giShtVal*/;
	//GrpAe(GA_ERR_NIGHT_ORI) = iTgtMin/*iTgtValDly*//*iShtValDly*/;
	//GrpAe(GA_IRS_VAL) = iIrsVal;
	//GrpAe(GA_ERR_CHT_PRE) = /*iIrsVal*/giShtVal;
	//GrpAe(GA_AGC_VAL_PRE) = /*giIrsPos*/iShtSVal/*giAgcVal*/;// + giShtVal - 125;

// -------------------------------------------------------------------------------
// WDR Control
	if(WdrCtrl(&wShtInt))
	{
		//if (gbWdrOnBuf2!=gbWdrOnBuf3) {SYNC_UPw(1); /*TxStr("sync_up_", 1,  "\n");*/}	// 141031 SMJ : 1080p30 WDR OFF image crack

		WDR_ONw(0);

		//gwWdrCnt = (gwWdrCnt==0 ) ? 0  : gwWdrCnt-1;

		iShtSVal = SHT_SET1((giShtVal>1) ? giShtVal-1 : 1);
	}

// -------------------------------------------------------------------------------
	giErr2d = giErr1d;
	giErr1d = iErr;

// -------------------------------------------------------------------------------
// Monitoring
	if(gbWdrOn){
		DebugDisp2(gbAeDebugOn, Dec, "Tgt    ",  3, AE_DEBUG_X, iTgt, 4)
		DebugDisp2(gbAeDebugOn, Dec, "Cur    ",  4, AE_DEBUG_X, iCur, 4)

		DebugDisp2(gbAeDebugOn, Dec, "SliceLv",  5, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3SlicLvl : nAe2SlicLvl, 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipLv ",  6, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipLvl : nAe2ClipLvl, 4)

		if (gnLSflag==AeSHORT) {
				DebugDisp2(gbAeDebugOn, Dec, "WdrLtgt", AE_WDR_DEBUG_Y+0/*17*/, AE_DEBUG_X, iTgt, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLcur", AE_WDR_DEBUG_Y+1/*18*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrSsht", AE_WDR_DEBUG_Y+5/*22*/, AE_DEBUG_X, wShtInt, 4)

		} else {
				DebugDisp2(gbAeDebugOn, Dec, "WdrStgt", AE_WDR_DEBUG_Y+2/*19*/, AE_DEBUG_X, iTgt, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrScur", AE_WDR_DEBUG_Y+3/*20*/, AE_DEBUG_X, iCur, 4)
				DebugDisp2(gbAeDebugOn, Dec, "WdrLsht", AE_WDR_DEBUG_Y+4/*21*/, AE_DEBUG_X, wShtInt, 4)
		}
	}
	else if ((gbWdrOn==MN_OFF)&&(gbWdrOnBuf2==MN_ON)) {
		DispClr(AE_WDR_DEBUG_Y+4, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+5, AE_DEBUG_X, 7+1+4);
		DispClr(AE_WDR_DEBUG_Y+6, AE_DEBUG_X, 7+1+4);
	}
	else {
		DebugDisp2(gbAeDebugOn, Dec, "TgtMax ",  3, AE_DEBUG_X, iTgt, 4)
		DebugDisp2(gbAeDebugOn, Dec, "TgtMin ",  4, AE_DEBUG_X, iTgtSpotBg, 4)
		DebugDisp2(gbAeDebugOn, Dec, "Cur    ",  5, AE_DEBUG_X, (AE_SAT_OFF) ? iCur : iCurBg, 4)
		DebugDisp2(gbAeDebugOn, Dec, "CurSpot",  6, AE_DEBUG_X, iCurSpot, 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipCnt",  7, AE_DEBUG_X, MIN(9999,(MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipCnt : nAe2ClipCnt), 4)
		DebugDisp2(gbAeDebugOn, Dec, "ClipLv ",  8, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipLvl : nAe2ClipLvl, 4)
		DebugDisp2(gbAeDebugOn, Dec, "SliceLv",  9, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? nAe3SlicLvl : nAe2SlicLvl, 4)
		DebugDisp2(gbAeDebugOn, Dec, "SlicCnt", 10, AE_DEBUG_X, (MP(MpBackLight)==MN_BL_BLC) ? (nAe2SlicCnt<<10)/nAe2PxCnt : (nAe3SlicCnt<<10)/nAe3PxCnt, 4)
	}

#if 0
	if(!AE_SAT_OFF) {
		const UINT OSD_AE_Y = (gbMnDebugFnc) ? 20 : 4;
		DispClr(OSD_AE_Y, 0, 10);
		DispDec(OSD_AE_Y, 0, MIN(9999,(MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipCnt : nAe2ClipCnt)/101, 2);
		DispDec(OSD_AE_Y, 2, ((MP(MpBackLight)==MN_BL_BLC) ? nAe3ClipLvl : nAe2ClipLvl) + 1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64) - iCurSpot, 4);

		const int iCurPer = (iCurBg < iTgtSpotBg) ?	(iCurBg - iTgtSpotBg) * 100 / iTgtSpotBg :
													(iCurBg - iTgtSpotBg) * 100 / (iTgt - iTgtSpotBg);
		if(iCurBg > iTgt) {
			DispDec(OSD_AE_Y, 7, iCurPer - 100, 3);
			SETFONTID(OSD_AE_Y, 7, '+');
		} else { DispDec(OSD_AE_Y, 7, iCurPer, 3); }
	}
#endif
}

#else
	#define AeAdv	AeOri
#endif

void ISPM Ae(void)
{	// AE top

	gbAeDebugOn = (gbMnDebugFnc==1);			// Debug string on condition

	if (MP(MpShdDet))		return;				// Shading exception
	
	//----------------------------------------------------------------------------------------
#if (model_Lens==0)
	static BYTE bMnFcsAdjOnCnt = 0;
	
	if(MP(MpFad_On)) {
		if(bMnFcsAdjOnCnt < (FPS<<1)) {
			if(gbWdrOn==MN_OFF) bMnFcsAdjOnCnt++;
			else if(LS_FLGr==AeSHORT) bMnFcsAdjOnCnt+=2;
		}
	}
	else {
		bMnFcsAdjOnCnt = 0;
	}
	
	if(bMnFcsAdjOnCnt >= FPS) { /*OUTPUTSET변경을 지원하기 위해서는 FreqAdjust(); 와 AeAGC()안의 AE_ETCw()설정을 추가해야 함*/ return; }		// FcsAdj exception
#endif
	//----------------------------------------------------------------------------------------

#if (model_Sony)
		//WaitUs(300);							// delay for reg latch timing over 6H (Sony) -> AeDev()로 개선
#endif

	FUNC_TIME_OSD(gbMnDebugFnc==1, Ae, 15, 24, 
		if(AE_CTRL_ORIGINAL) AeOri();
		else				 AeAdv();
	)

	IspAgcCurSet();

#if AE_CTRL_ADV
	#define DSS_OFF_CNT		(gbSensShtDly + 2 /*+ PAR00*/)
#else
	#define DSS_OFF_CNT		4
#endif

	if(giCurDss) giCurDssStg = DSS_OFF_CNT;	// DSS가 완전히 OFF된 후 WDR또는 DNR이 동작하도록 Delay 필요
	else if(giCurDssStg > 1) giCurDssStg--;

	//GrpAe(GA_CUR_DSS_STG) = giCurDssStg;
	//GrpAe(GA_ERR_NIGHT_ORI) = giCurDss;

	AEControlErrSet();


	//if(gbAeDebugOn) { TxGrpFnc(); }
}


#if 0
void IrisPwmTest(void)
{
	PwmSetDty(PAR1C);

	const UINT 	nAe2Sum1    = AE2_SUM1_LOCKr  & 0x7fffffff ;
	const UINT 	nAe2Sum2    = AE2_SUM2_LOCKr  & 0x7fffffff ;
	const UINT 	nAe2ClipCnt = AE2_CLCNT_LOCKr & 0x001fffff ;
	const UINT 	nAe2SlicCnt = AE2_SLCNT_LOCKr & 0x001fffff ;

	const UINT	nAe2PxCnt	= ((AE2_VEPr - AE2_VSPr)+1) * ((AE2_HEPr - AE2_HSPr)+1);
	const UINT	nClipLvl 	= AE2_CLIPr;
	const UINT	nSlicLvl 	= AE2_SLICEr;

	UINT	nSumMean1=0;
	int		iTgt=0, iCur=0, iErr=0;

	const int iAe2ClipCnt = LibUtlInterp1D(MP(MpHlMaskThrs)+8, 0, 28, 0, nAe2ClipCnt);					
	if(MP(MpBackLight)==MN_BL_HLC)	nSumMean1 = ((nAe2SlicCnt*nSlicLvl) + nAe2Sum1 + (iAe2ClipCnt*nClipLvl)) / nAe2PxCnt;		//		"
	else							nSumMean1 = ((nAe2SlicCnt*nSlicLvl) + nAe2Sum1 + (nAe2ClipCnt*nClipLvl)) / nAe2PxCnt;		//		"

	iCur = (int)nSumMean1;

	iTgt = TgtMaxGet(0,0);
	if(MP(MpBackLight)==MN_BL_HLC) 	iTgt += (iTgt>>2);
	else				   			iTgt = ((MP(MpShutter)==MN_SHUT_AUTO)&&(!AE_DEBLUR_COND)) ?
											MAX(iTgt-(giCurAgc/2), (iTgt/2)):		// decrease by AGC
											MAX(iTgt-(giCurAgc/3), (iTgt/2));		// "

	iErr = iTgt - iCur;

	DebugDisp(gbAeDebugOn, Dec, "cur_______",  9, 24, iCur);
	DebugDisp(gbAeDebugOn, Dec, "Ae2Sum1", 19, 24, nAe2Sum1);
	DebugDisp(gbAeDebugOn, Dec, "Ae2Sum2", 20, 24, nAe2Sum2);

	GRP0 = iCur;
	GRP1 = nAe2Sum1;
	GRP2 = nAe2Sum2;
	TxGrp();
}
#endif

//*******************************************************************************
// ROI WDR Mode Function
//*******************************************************************************
#if model_WDR_ROI
void ISPM ROI_WDR(void)
{
	//ROI WDR Mode Setting
	UINT i = 0;
	BYTE bROIWdrOn = 0;

	if(!gbMenuState)	gbROIWdrBoxFlag = 0;
	
	if(MP(MpROIWdrMode)==MN_ON){

		UINT nSPosX,nSPosY,nEPosX,nEPosY,nSizX,nSizY;
		UINT nTmp = 0;
//			const BYTE bOffsetX = 72, bOffsetY = 14; // Org
		const BYTE bOffsetX = 0x22, bOffsetY = 0x28; // Test Only AR0237 Sensor
//			BYTE bOffsetX = (BYTE)PAR13, bOffsetY = (BYTE)PAR14; // For Off-Set Test
		
		for(i=0; i<4; i++){
			
			nSizX = ((gbMnROIWdrCfg(i)->bSizX_Up<<8)|gbMnROIWdrCfg(i)->bSizX_Lo);	
			nSizY = ((gbMnROIWdrCfg(i)->bSizY_Up<<8)|gbMnROIWdrCfg(i)->bSizY_Lo);

			nSPosX = ((gbMnROIWdrCfg(i)->bPosX_Up<<8)|gbMnROIWdrCfg(i)->bPosX_Lo) + bOffsetX;			
			nSPosY = ((gbMnROIWdrCfg(i)->bPosY_Up<<8)|gbMnROIWdrCfg(i)->bPosY_Lo) + bOffsetY;

			nEPosX = nSPosX + nSizX;
			nEPosY = nSPosY + nSizY;

			if(MP(MpMirror)==MN_ON){
				nTmp = nSPosX;
				nSPosX = FR_HW - nEPosX + (bOffsetX<<1);
				nEPosX = FR_HW - nTmp + (bOffsetX<<1);
			}

			if(MP(MpFlip)==MN_ON){
				nTmp = nSPosY;
				nSPosY = FR_VW - nEPosY + (bOffsetY<<1);
				nEPosY = FR_VW - nTmp + (bOffsetY<<1);
			}

			if((int)nSPosX < 0)				nSPosX = 0;
			if((int)nSPosY < 0)				nSPosY = 0;
			if(nEPosX > (FR_HW+bOffsetX))	nEPosX = (FR_HW+bOffsetX);
			if(nEPosY > (FR_VW+bOffsetY))	nEPosY = (FR_VW+bOffsetY);
			
//				if(i<2){															//because 0xFE,0xFF Reserved Area
//					SetIsp(ROIWDR_BASE+(i*2),		(ValSft_L12(nSPosX)|nSPosY)); 	//WDR Window HSP,VSP
//					SetIsp(ROIWDR_BASE+(i*2+1), 	(ValSft_L12(nEPosX)|nEPosY)); 	//WDR Window HEP,VEP 
//				}
//				else if(i==2){
//					SetIsp(ROIWDR_BASE+(i*2),		(ValSft_L12(nSPosX)|nSPosY)); 	//WDR Window2 Addr 0xFD  HSP,VSP	
//					SetIsp((ROIWDR_BASE+2)+(i*2+1), (ValSft_L12(nEPosX)|nEPosY)); 	//WDR Window2 Addr 0x100 HEP,VEP
//				}
//				else{																//Restart Address 0x101
//					SetIsp((ROIWDR_BASE+2)+(i*2),	(ValSft_L12(nSPosX)|nSPosY)); 	//WDR Window HSP,VSP
//					SetIsp((ROIWDR_BASE+2)+(i*2+1), (ValSft_L12(nEPosX)|nEPosY)); 	//WDR Window HEP,VEP
//				}

				SetIsp(ROIWDR_BASE+(i*2),		(ValSft_L12(nSPosX)|nSPosY)); 	//WDR Window HSP,VSP
				SetIsp(ROIWDR_BASE+(i*2+1), 	(ValSft_L12(nEPosX)|nEPosY)); 	//WDR Window HEP,VEP 

			bROIWdrOn |= (gbMnROIWdrCfg(i)->bAction)<<i;	//Area Check
		}

		if(gbROIWdrBoxFlag && (gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction)){	

//				const BYTE bRectOffsetX = 5, bRectOffsetY = 3;
			
			nSizX = ((gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizX_Up<<8)|gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizX_Lo);	
			nSizY = ((gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizY_Up<<8)|gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizY_Lo);
			
			nSPosX = ((gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosX_Up<<8)|gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosX_Lo);			
			nSPosY = ((gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosY_Up<<8)|gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosY_Lo);

			nEPosX = nSPosX + nSizX;
			nEPosY = nSPosY + nSizY;

			// EN673's Mirr/Flip function is operated by Sensor. So, don't need to conversion.
//				nSPosX = (MP(MpMirror)==MN_ON) ? (nSPosX - bRectOffsetX) : nSPosX;
//				nEPosX = (MP(MpMirror)==MN_ON) ? (nEPosX - bRectOffsetX) : nEPosX;

//				nSPosY = (MP(MpFlip)==MN_ON) ? (nSPosY + bRectOffsetY) : nSPosY;
//				nEPosY = (MP(MpFlip)==MN_ON) ? (nEPosY + bRectOffsetY) : nEPosY;

			if((int)nSPosX < 0)				nSPosX = 0;
			if((int)nSPosY < 0)				nSPosY = 0;
			if(nEPosX > (FR_HW+bOffsetX))	nEPosX = (FR_HW+bOffsetX);
			if(nEPosY > (FR_VW+bOffsetY))	nEPosY = (FR_VW+bOffsetY);

			gnBoxROIWdr[0] = ValSft_L16(nSPosY)|(nEPosY);					// Y
			gnBoxROIWdr[1] = ValSft_L16(nSPosX)|(nEPosX);					// X
			
			gbBoxOnROIWdr = 0x1;
			gbBoxFillROIWdr = 0x1;
			gbBoxToneROIWdr = 0x3;
		}

		else{
			gbBoxOnROIWdr = 0x0;
			gbBoxFillROIWdr = 0x0;
			gbBoxToneROIWdr = 0x0;
		}

		WDR_WND_ONw(bROIWdrOn); 							//WDR Window ON
		
		WDR_BACKGND_SELw(0x1);								//Long Image
		WDR_WINDOW_SELw(0x2);								//Short Image
	}
	
	else{													//WDR Normal Mode
	
		WDR_WND_ONw(0x0);									//WDR Window OFF
			
		WDR_BACKGND_SELw(0x0);								//WDR Image
		WDR_WINDOW_SELw(0x0);								//WDR Image
	}
	
}
#endif

/* EOF */

