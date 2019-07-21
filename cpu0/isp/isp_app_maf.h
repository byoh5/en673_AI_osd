/* **************************************************************************
 File Name	: 	isp_app_maf.h
 Description	:	Motorized AF Function Header file
 Designer		:	Lee, Wanhee
 Date		:	17.11.07
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
	
#ifndef __MOTORIZED_AF_H__
#define __MOTORIZED_AF_H__

//=================================================================================================
//	define
//=================================================================================================
#define	MAF_DEBUG			PAR1E //*0 : Normal Mode
                              	  // 1 : Debug Mode

#define MAF_BOSD_DISP		1	 // 0 : No Box OSD Display
                                 //*1 : Box OSD Display Style #1 (Zoom&Focus / Coordinates)
                                 // 2 : Box OSD Display Style #2 (Zoom / Bar)

#define MAF_FOSD_DISP		1	 //*0 : No Font OSD Display
                                 // 1 : Font OSD Display (Control State)


//=================================================================================================
//	Main function
//=================================================================================================
void MAF_Init(void);
void Motorized_AF(void);

#define MAF_EXCUTION_CHECK	if(MN_FPS_HI) {	static BYTE bExcuteFlag=0;\
											bExcuteFlag=(bExcuteFlag==0);\
											if(!bExcuteFlag) return; }

BYTE gbAF_MainState=0;
BYTE gbMAF_DevMode=0;
BYTE gbMaf_AgingTest=0;
BYTE gbMAF_Excute=0;


#define MAF_ODM_H			0x00200398
#define MAF_ODM_V			0x00090432


//=================================================================================================
//	Lens model Settings #1 - Lens position Setting
//=================================================================================================
//                        YT2812 YT3013 YT3017 YT30022 YT3021 Cstm
// Holder 15mm
//UINT gnMaf_Range_Z[10]   = {5120,  5120,  5120, 0x1400,0x1300,5000,   0,   0,   0,   0};
//UINT gnMaf_Start_Z[10]   = { 600,   600,   600, 0x0100,0x0100, 250,   0,   0,   0,   0};
//UINT gnMaf_End_Z[10]     = {4608,  4608,  4608, 0x1300,0x1200,4750,   0,   0,   0,   0};
//UINT gnMaf_LDof_Z[10]    = {3072,  3072,  3072, 0x0D00,0x0C00,3750,   0,   0,   0,   0};
//
//UINT gnMaf_Range_F[10]   = {5120,  6144,  4864, 0x1A00,0x1600,5000,   0,   0,   0,   0};
//UINT gnMaf_Start_F[10]   = { 600,   600,   256, 0x0100,0x0100, 250,   0,   0,   0,   0};
//UINT gnMaf_End_F[10]     = {4800,  5824,  4608, 0x1900,0x1500,4750,   0,   0,   0,   0};

// Holder 13.7mm
UINT gnMaf_Range_Z[10]   = {5000,  5120,  5120, 0x1400,0x1300,5000,   0,   0,   0,   0};
UINT gnMaf_Start_Z[10]   = { 600,   600,   600, 0x0100,0x0100, 250,   0,   0,   0,   0};
UINT gnMaf_End_Z[10]     = {4500,  4608,  4608, 0x1300,0x1200,4750,   0,   0,   0,   0};
UINT gnMaf_LDof_Z[10]    = {3072,  3072,  3072, 0x0D00,0x0C00,3750,   0,   0,   0,   0};

UINT gnMaf_Range_F[10]   = {4780,  6144,  4864, 0x1A00,0x1600,5000,   0,   0,   0,   0};
UINT gnMaf_Start_F[10]   = { 300,   600,   256, 0x0100,0x0100, 250,   0,   0,   0,   0};
UINT gnMaf_End_F[10]     = {4500,  5824,  4608, 0x1900,0x1500,4750,   0,   0,   0,   0};



UINT gnMaf_RvsStep_Z[10] = {   0,     0,     0,   0,   0,   0,   0,   0,   0,   0};
UINT gnMaf_RvsStep_F[10] = {0x30,  0x30,  0x30,   0x30,0x30,0x30,0x30,0x30,0x30,0x30};

#define MAF_MAX_STEP		(16<<1)
#define MAF_MNL_STEP_F		(10<<1)

#define MAF_INIT_STEP_Z		MAF_RANGE_Z
#define MAF_INIT_STEP_F		MAF_RANGE_F


//=================================================================================================
//	Control Command check
//=================================================================================================
void MAF_CmdCheck(void);

BYTE gbMAF_TrgCnt=0, gbMAF_Calibration=0, gbDnN_Changed=0;

//=================================================================================================
//	MAF Function Test
//=================================================================================================
void MAF_Test_Mode(void);


//=================================================================================================
//	Motor Speed Setting
//=================================================================================================
void MAF_SpeedSetting(void);


//=================================================================================================
//	Direction Control
//=================================================================================================
void MAF_DirectionControl(void);

//BYTE gbMaf_Direction=0; // 0x01(Up) : Tele, 0x02(Down) : Wide, 0x04(Right) : Far, 0x08(Left) : Near

BYTE gbMAF_Tele,gbMAF_Wide,gbMAF_Near,gbMAF_Far;

#define MAF_TELE_Z		gbMAF_Tele
#define MAF_WIDE_Z		gbMAF_Wide
#define MAF_NEAR_F		gbMAF_Near
#define MAF_FAR_F		gbMAF_Far

//=================================================================================================
//	External Control (Onvif/PELCO)
//=================================================================================================
void MAF_ExternalControl(void);


//=================================================================================================
//	Motor Manual Control
//=================================================================================================
void MAF_MnlMotorControl(BYTE abExcute);
void MAF_ZoomTracking(void);
void MAF_ZoomTracking_Back(void);
void MAF_Current_Mng(void);

BYTE gbMAF_ZoomTrkEn=0, gbManualCtr_F=0, gbMaf_MnlSpeedCnt_F=0, gbMAF_KeyBuf=0;
UINT gnAgingTest_Cnt=0;

typedef struct _MAF_MOTOR_STRUCTURE {
	UINT nCurPos;				/* Motor Current Position */
	UINT nTgtPos;				/* Motor Target Position */
	BYTE bPosGo;				/* Position Control Enable */
	BYTE bActive;				/* Active State */
	BYTE bDirFlag;				/* Control Direction Flag */
	BYTE bDirFlagBuf;			/* Control Direction Flag Buffer */
	BYTE bReverseCnt;			/* Reverse counter for position loss */
	BYTE bFreq;					/* Motor Frequency */
	BYTE bMovingState;			/* Moving State for Acceleration control */
	BYTE bMovingFlag;			/* Motor Operating Enable condition */
	BYTE bSpeedCnt;				/* Motor Speed Counter */
	BYTE bStateStep;			/* Motor Phase step */
	BYTE bStateStepBuf;			/* Motor Phase step Buffer */
} Maf_motor_structure;

Maf_motor_structure Maf_Zmotor={0,0,0,0,0,0,0,0,0,0,0,0,0};
Maf_motor_structure Maf_Fmotor={0,0,0,0,0,0,0,0,0,0,0,0,0};

extern volatile UINT *gnMaf_LibCurPos_F;
extern volatile UINT *gnMaf_LibTgtPos_F;
extern volatile BYTE *gbMaf_LibTgtCtr_Go;

//=================================================================================================
//	Auto Focus (No Library)
//=================================================================================================
void MAF_DataBuffering(void);
void MAF_AutoFocus(void);

#define MAF_ONESHOT_TRG_TIME		40 // AF Enable delay time after Zoom move stop
#define MAF_ONESHOT_FILTER_TIME		(MAF_ONESHOT_TRG_TIME<<1) // AF Enable delay time after D&N Filter Change

#define MAF_TRY_MAX		5 // Max Retry Times @Not Peak Data when the focus point
#define MAF1_DIRV		4
#define MAF2_DIRV		2
#define MAF_YSCALE		0x0F
#define MAF_YALPHA_MIN	0x3E8
#define MAF_YALPHA_MAX	0x4B0
#define MAF_DAVG_OFS0	20
#define MAF_DAVG_OFS1	30

volatile int giMaf_AF_TryMax     = MAF_TRY_MAX; // Maximum AF Retry Times
volatile UINT gnMaf_FPosRange[2]; // Total Focus Range

// for Focus Direction Filter
volatile int giMaf_DirNo_P[2]    = { MAF1_DIRV, MAF2_DIRV};
volatile int giMaf_DirNo_M[2]    = {-MAF1_DIRV,-MAF2_DIRV};
volatile int giMaf_DAvg_Ofs[2]   = {MAF_DAVG_OFS0,MAF_DAVG_OFS1};

// for AF Data Normalization
volatile int giMaf_CurGain=0, giMaf_GainRange[2]={0,0};
volatile UINT gnMaf_AF_DATA[2]={0,0}, gnMaf_Y_DATA[2]={0,0};

volatile UINT gnMAF_YnmlAlpha[2] = {MAF_YALPHA_MIN,MAF_YALPHA_MAX};
volatile UINT gnMaf_Y_SCALE[2]   = {MAF_YSCALE,MAF_YSCALE};

// AF Complete Peak Data Success rate(Scanning Peak AF Data vs Finish AF Data)
volatile UINT gnMaf_TgtPeakMgn   = 50;//95; // % Matching Percentage

// from Library
extern const int Sens(AE_GAIN_TOTAL);
extern void MAF_PreProcessing(void);
extern void MAF_OneTimeAF(void);

extern BYTE gbMAF_OneTimeState;
extern BYTE gbAutoFocus_Flag;
extern volatile BYTE gbMaf_Status;

extern UINT gnMaf_TgtPeakVal_F, giMAF1_DirCntBuf[15], gnAF1_Normalization, gnAF2_Normalization;

//=================================================================================================
//	Lens State Display
//=================================================================================================
#define BAF_BOXOSD_TIME		15

void MAF_Lens_StateDisp(void);
void MAF_DebugingDisp(void);

BYTE gbSAFDebugOn=0;

#if (MAF_BOSD_DISP!=0)
//#define MAF_BOX_START	27 // Box No.27~31
#define MAF_BOX_BITMASK		0xF8000000
UINT gnBoxPosMAF_Y[5] = {0x00c000c1, 0x00400140, 0x00c000c1, 0x00400140, 0x00400140};
UINT gnBoxPosMAF_X[5] = {0x06c006c1, 0x06c006c1, 0x05800720, 0x05800720, 0x05800720};
UINT gnBoxPltMAF[5]   = {0x004C54FF, 0x00fd13b0, 0x00fd13b0, 0x00df7f7f, 0x005f7f7f};
UINT gnBoxToneMAF = 0x2A400000;
UINT gnBoxFillMAF = 1;
UINT gnBoxOnMAF = 0;
BYTE gbMAFBoxFlag = 0;
#endif

#if(MAF_FOSD_DISP==1)
#define MAF_LENS_INIT	(gbAF_MainState==1)
#define MAF_AUTOFOCUS	(gbAutoFocus_Flag==1)
#define MAF_ZTRACKING	(gbMAF_ZoomTrkEn==1)
#define MAF_AGINGTEST	(MP(MpMAF_TestMode)==1)
#define MAF_PRESET_MV	(Maf_Zmotor.bPosGo|Maf_Fmotor.bPosGo)
#endif

//=================================================================================================
//	GPIO Drive Handling (Timer IRQ Base)
//=================================================================================================
#define MOTOR_STATE0		0	// Idle State
#define MOTOR_STATE1		1	// Speed Step 1 : First or Last Speed
#define MOTOR_STATE2		2	// Speed Step 2 : Middle Speed
#define MOTOR_STATE3		3	// Speed Step 3 - Max Speed (Moving)
#define MOTOR_STATE4		4
#define MOTOR_STATE5		5

BYTE gbMotorSpeedStep[5] = {20/*25PPS*/,10/*50PPS*/, 5/*100PPS*/,2/*250PPS*/,1/*500PPS*/};

#define MAF_STEP1		1
#define MAF_STEP2		2
#define MAF_STEP3		3
#define MAF_STEP4		4
#define MAF_STEP5		5
#define MAF_STEP6		6
#define MAF_STEP7		7
#define MAF_STEP8		8

#define MAF_H		GpioSetHi
#define MAF_L		GpioSetLo


//=================================================================================================
//	Lens model Settings #2 - Stepping port assign
//=================================================================================================
//                              YT2812            YT3013           YT3017            YT30022           YT3021         CSTM LENS
const BYTE gbMaf_AP_Z[10] = {GPIO_MTR_ZOOM_AP,GPIO_MTR_ZOOM_AP, GPIO_MTR_FCS_AP,GPIO_MTR_ZOOM_AP, GPIO_MTR_FCS_BP,GPIO_MTR_ZOOM_AP,0,0,0,0};
const BYTE gbMaf_AM_Z[10] = {GPIO_MTR_ZOOM_AM,GPIO_MTR_ZOOM_AM, GPIO_MTR_FCS_AM,GPIO_MTR_ZOOM_AM, GPIO_MTR_FCS_BM,GPIO_MTR_ZOOM_AM,0,0,0,0};
const BYTE gbMaf_BP_Z[10] = {GPIO_MTR_ZOOM_BP,GPIO_MTR_ZOOM_BP, GPIO_MTR_FCS_BP,GPIO_MTR_ZOOM_BP, GPIO_MTR_FCS_AM,GPIO_MTR_ZOOM_BP,0,0,0,0};
const BYTE gbMaf_BM_Z[10] = {GPIO_MTR_ZOOM_BM,GPIO_MTR_ZOOM_BM, GPIO_MTR_FCS_BM,GPIO_MTR_ZOOM_BM, GPIO_MTR_FCS_AP,GPIO_MTR_ZOOM_BM,0,0,0,0};
const BYTE gbMaf_AP_F[10] = { GPIO_MTR_FCS_AP, GPIO_MTR_FCS_AP,GPIO_MTR_ZOOM_BM, GPIO_MTR_FCS_AP,GPIO_MTR_ZOOM_AM, GPIO_MTR_FCS_AP,0,0,0,0};
const BYTE gbMaf_AM_F[10] = { GPIO_MTR_FCS_AM, GPIO_MTR_FCS_AM,GPIO_MTR_ZOOM_BP, GPIO_MTR_FCS_AM,GPIO_MTR_ZOOM_AP, GPIO_MTR_FCS_AM,0,0,0,0};
const BYTE gbMaf_BP_F[10] = { GPIO_MTR_FCS_BP, GPIO_MTR_FCS_BP,GPIO_MTR_ZOOM_AM, GPIO_MTR_FCS_BP,GPIO_MTR_ZOOM_BM, GPIO_MTR_FCS_BP,0,0,0,0};
const BYTE gbMaf_BM_F[10] = { GPIO_MTR_FCS_BM, GPIO_MTR_FCS_BM,GPIO_MTR_ZOOM_AP, GPIO_MTR_FCS_BM,GPIO_MTR_ZOOM_BP, GPIO_MTR_FCS_BM,0,0,0,0};

// Motor drive current save
#define MAF_HOLD_Z  { MAF_L(gbMaf_AP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_Z[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_Z[MP(MpMAF_LensModel)]); }
#define MAF_HOLD_F  { MAF_L(gbMaf_AP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BP_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_AM_F[MP(MpMAF_LensModel)]); MAF_L(gbMaf_BM_F[MP(MpMAF_LensModel)]); }

void MAF_MovingState(void);
void MAF_PhaseControl(void);

BYTE gbMaf_TimerEn=0;
UINT gnMAF_PTime[4]={0,0,0,0};
extern UINT Start_HTBTR(void);
extern UINT End_HTBTR(UINT nStart);
extern void OsdWhiteAll(void);
extern void OsdClearAll(void);


//=================================================================================================
//	Lens Initialization
//=================================================================================================
void Lens_AgingTest(void);
void Lens_Initialization(void);

BYTE gbMaf_InitState=0, gbMaf_GlbRst=0;


//=================================================================================================
//	Lens model Settings #3 - Trace Curve Auto Calibration
//=================================================================================================
void TraceCurve_Calibration(void);

#define MAF_TRACKING_TBL_STEP	11//20

UINT gnMaf_TblPosition_Z[MAF_TRACKING_TBL_STEP]; // It Calculated by Zoom Active area definition

UINT gnMaf_TblPosition_F[10][MAF_TRACKING_TBL_STEP] = {
//	{ 0x0ac8,0x0cce,0x0e5e,0x0f8a,0x1073,0x1119,0x11ae,0x11e3,0x1247,0x125b,0x125d }, // YT2812 (@Holder 15mm)
//	{ 0x0295,0x0680,0x093c,0x0b51,0x0cf1,0x0e1d,0x0ef6,0x0f9c,0x1016,0x1067,0x10a7 }, // YT2812 (@Holder 13.7mm)
//	{ 0x0265,0x067e,0x095b,0x0b81,0x0d21,0x0e4d,0x0f36,0x0fdc,0x1056,0x10a7,0x10dc }, // YT2812 (@Holder 13.7mm)
	{ 0x037c,0x0712,0x09bd,0x0bb1,0x0d40,0x0e7c,0x0f44,0x0fea,0x105c,0x109c,0x10d7 }, // YT2812 (@Holder 13.7mm) // 190320
	{ 0x0568,0x0a92,0x0e3e,0x10e2,0x12d1,0x142b,0x1501,0x159b,0x15f8,0x1614,0x160d }, // YT3013  (@YTOT ICR Holder)
	{ 0x0974,0x0829,0x06f0,0x05b7,0x047e,0x0366,0x0290,0x020c,0x0261,0x03d7,0x0748 }, // YT3017 (@Holder 14mm) for EVM v2.0
	{ 0x01c5,0x0794,0x0c16,0x0f4f,0x11d2,0x139f,0x14f9,0x15f6,0x16b0,0x1733,0x17a5 }, // YT30013 (@YTOT ICR Holder)
	{ 0x14cd,0x105f,0x0d0d,0x0a94,0x08b1,0x0742,0x0648,0x0581,0x04fc,0x0495,0x0476 }, // YT3021  (@YTOT ICR Holder) - TBD
	{ 0x0ba2,0x0d85,0x0f04,0x100e,0x10d6,0x119b,0x11e1,0x1233,0x1252,0x1267,0x126b }, // Custom Lens Settings
	{    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0 },
	{    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0 },
	{    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0 },
	{    0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0,   0x0 }};


#endif // End of __MOTORIZED_AF_H__

/* EOF */

