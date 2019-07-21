/* **************************************************************************
 File Name	: 	dev_isp_common.h
 Description:	EN673 - Device Driver
 Designer	:	Kim, Sunghoon / Lee, Wanhee
 Modifier	:	Kwon, DoHyung
 Date		:	17. 4. 6 / 15. 4. 24 / 18. 3. 19
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#ifndef _MODEL_SENS_ONLY_
	#include "dev_regs_isp_macro.h"
	#include "dev_types.h"
#endif

	#define MCLK		BUS_CLK			// AHB clock (core)


//*************************************************************************************************
// Software version info & fixed define
//-------------------------------------------------------------------------------------------------

	#define 	model_ver			0x6303		// Eyenix base code version (Fixed)

	#define 	SENS_MN34210			1		// Pana 1.3M 60P(also MN34219), LVDS Mode - TBD
	#define 	SENS_IMX225				4		// Sony	1.3M 60P(also IMX224 ), LVDS Mode - TBD
	
	#define 	SENS_MN34229			11		// Pana 2M 60p, LVDS Mode
	#define 	SENS_MN34420			12		// Pana 2M 60p, LVDS Mode
	#define 	SENS_IMX307				13		// Sony	2M 60p, LVDS Mode
	#define 	SENS_IMX327				14		// Sony	2M 60p, LVDS Mode
	#define 	SENS_IMX291				15		// Sony	2M 60p, LVDS Mode
	#define 	SENS_AR0237CSH 			18 		// Aptina 2M 60p, HiSpi Mode (PLCC Type)
	
	#define 	SENS_MN34227			21		// Pana	2M 30p(also MN34220), LVDS Mode
	#define 	SENS_OV2718				22		// Omni	2M 30p, Parallel Mode
	//		#define 	SENS_IMX122 			23		// Sony 2M 30p(also IMX222/IMX322), Parallel Mode - TBD
	#define 	SENS_IMX323				24		// Sony	2M 30p, Parallel Mode
	//		#define 	SENS_AR0237CSP			25		// Aptina 2M 30p, Parallel Mode(PLCC Type) - TBD


//*************************************************************************************************
// 1. System Mode user define
//-------------------------------------------------------------------------------------------------
#ifndef __ISP_LIB__
		
	#ifndef USING_CONFIG_ISP_SCRIPT // Use Configuration Tool (*.mk file)

	#define 	model_Sens				SENS_IMX327


	#define 	SENS_CONTROL_MODE		1		// 0 : SPI, *1 : TWI
	
	
	#define 	model_Shade 			0		// 0 : Shading table by Storage dumpping
												// 1 : Shading table by defined array
	
	#define 	model_Save				1		// 0 : save Parameter value to External EEPROM by I2C(TWI)
												//*1 : save Parameter value to internal Flash (default)
												// 2 : None save
	
	#define 	model_Par				1		// 0 : no use user parameter (use defined parameter)
												//*1 : use user parameter (use variable parameter)
	
	#define 	model_Par_Save			0		// 0 : No Save user parameter in Flash
												//*1 : Save user parameter in Flash
												
	#endif // End of USING_CONFIG_ISP_SCRIPT
												
	// Special Functions ------------------------------------------------------
		
	#define		model_ParID			0x12345678	// Enable parameter ID for control with ECM
	#define		model_HexID			0x1234		// Hex file ID of parameter for download with ECM
	
		
	// Options --------------------------------------------------------------

	#define		model_ADV_AE			1		// 0 : use Original AE
												//*1 : use Advanced AE (Some sensors are not supported : Panasonic Sensors & IMX225)
	
	#define 	model_Contrast			1		// 0 : Org Gamma Table
												//*1 : New Gamma Table
	
	#define 	model_Flash 			0		//*0 : Flash - Eon
												// 1 : Flash - Winbond
												// 2 : Flash - Giga Device
	
	#define 	model_Protect			1		// 0 : no use Flash Protection
												//*1 : use Flash Protection (default)
			
	#define 	model_I2cSlave			0		//*0 : I2C operate to Master mode (default)
												// 1 : I2C operate to Slave mode (can't use EEPROM save)
												// 2 : I2C operate to Master + Slave mode (can use EEPROM save)
														
	#define 	model_CharMax			1		// 0 : Font char max ea - 256 (BYTE)
												//*1 : Font char max ea - ISP_FONT_CHAR_EA (WORD)		
		
	#define		model_SetIsp			0		// 0 : Macro ISP Set(Old)
												//*1 : Function ISP Set(New)
												
	#define		model_IspRegCheck		0		//*0 : no use Check the change of ISP register
												// 1 : use Check the change of ISP register

	#define		model_LSPI_MODE			0		//*0 : Peripheral SPI Sensor I/F
												// 1 : Logic SPI Sensor I/F
		
	#define		Sens225_IF_Mode			1		// 0 : Parallel
												//*1 : LVDS
		
	#define 	model_Test				0		//*0 : normal operation
												// 1 : Test mode without sensor. Sensor have to drive to slave(HW)
												// 2 : Test mode without mode change function (30p only)
		
	#define		model_NoSensor			0		// 0 : normal operation
												// 1 : Test mode without sensor. Output test pattern
		
	#define		model_WDR_ROI			0		// 0 : no use (+MENU)
												// 1 : use (+MENU)

	#define		model_LED_ASAT			0		// 0 : no use (+Lib)(+MENU)
												// 1 : use (+Lib)(+MENU)
		
	#define		model_ExitMenu			3		//*3 : normal operation
		
		
#endif//__ISP_LIB__

	#define 	model_Ispm				1		// 0 : no use ISPM(+Lib)
												//*1 : use ISPM(+Lib)
	
	#define 	model_float 			0		//*0 : no use -mhard-float(+Lib)
												// 1 : use -mhard-float, not float comparison operation(+Lib)

	#define 	model_FPU				1		// 0 : no use FPU for AWB (+Lib)
												//*1 : use FPU for AWB (+Lib)

#if defined(model_Sens) && !defined(_MODEL_SENS_ONLY_)

#include <string.h>					// for strlen
#include "dev.h"

	#define		model_1M				((model_Sens>= 0)&&(model_Sens<10))
	#define		model_2M				((model_Sens>=10)&&(model_Sens<20))
	#define		model_2M30p				((model_Sens>=20)&&(model_Sens<30))

#ifdef DEF_CPU0
#include "dev_isp_misc.h"
#endif
///////misc.h
#if model_CharMax == 1
	#define EXCH		WORD
	#define EXCH_ADD	1
#else
	#define EXCH		char
	#define EXCH_ADD	0
#endif


//*************************************************************************************************
// 5. UART
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern
extern void UartRstQue(void);
extern void UartRstRxQue(void);

extern void UartTxByte(BYTE abByte);
extern void UartTxStr(const char *apcStr);

// Extern
extern void UartRstQue1(void);
extern void UartRstRxQue1(void);

extern void UartTxByte1(BYTE abByte);

//*************************************************************************************************
// 6. I2C
//-------------------------------------------------------------------------------------------------
#define I2C_CLK_SET(CH,SPEED)	I2C##CH##_CLK_DIV	= (MCLK/(8*SPEED)-1)


//*************************************************************************************************
// 7. TIMER
//-------------------------------------------------------------------------------------------------
#define		PwmEna				(DFSEL_TgtBd(TIM_CH5_EN=1     , TIM_CH5_EN=1     , TIM_CH5_EN=1                       ))	// enable
#define		PwmDis				(DFSEL_TgtBd(TIM_CH5_EN=0     , TIM_CH5_EN=0     , TIM_CH5_EN=0                       ))	// disable
#define		PwmSetPrd(anIn)		(DFSEL_TgtBd(TIM_CH5_LMT=(UINT)anIn , TIM_CH5_LMT=(UINT)anIn , TIM_CH5_LMT=(UINT)anIn ))	// PWM period
#define		PwmSetDty(anIn)		(DFSEL_TgtBd(TIM_CH5_TRIG=(UINT)anIn, TIM_CH5_TRIG=(UINT)anIn, TIM_CH5_TRIG=(UINT)anIn))	// PWM duty (Must be shorter than "TM2CNT_PWMW")
#define		PwmSetNum(anIn)		(DFSEL_TgtBd(                       ,                        ,                        ))	// PWM pulse number value
//extern void PwmCfg(BYTE abPreFact, BYTE abPol);

#define	TIMER_NUM			6

// 15.5.1 WHL - 108Mhz 1ms Target
#define TIMERx_LIMIT		0x2A30
#define TIMERx_CLKDIV		0x4
#define TIMERx_IRQ_HZ		100

#define TIMERtoSEC(IsrTimerCnt)		(IsrTimerCnt/TIMERx_IRQ_HZ)

#define TgtToLimit(CK,FRQ,DIV)			(((CK/2)/(DIV+1))/FRQ)	// Timer Clock = MCLK / 2 / ( Timer_Div+1)
//	#define TgtToLimit(CK,FRQ,DIV)			((CK/((DIV+1)*2))/FRQ)	// Timer Clock = MCLK / 2 / ( Timer_Div+1)

#define TM0_TGT_FRQ		1000	// 1000(ms)/Freq = 1ms
#define TM1_TGT_FRQ		1000	// 1000(ms)/Freq = 1ms
#define TM2_TGT_FRQ		100		// 1000(ms)/Freq = 10ms
#define TM3_TGT_FRQ		1000	// 1000(ms)/Freq = 1ms
#define TM4_TGT_FRQ		10000	// 1000(ms)/Freq = 100us
#define TM5_TGT_FRQ		100000	// 1000(ms)/Freq = 10us

#define TM0_TGT_DIV		99//90	//100	// 8bit (0~255)
#define TM1_TGT_DIV		14//100	//9
#define TM2_TGT_DIV		7//120	//14
#define TM3_TGT_DIV		14//150	//15
#define TM4_TGT_DIV		14//180	//14
#define TM5_TGT_DIV		14//250	//15
									
#define TM0_PWM_EN		0	// PWM Enable Set
#define TM1_PWM_EN		0
#define TM2_PWM_EN		0
#define TM3_PWM_EN		0
#define TM4_PWM_EN		0
#define TM5_PWM_EN		0
										
#define TM0_IRQ_EN		1	// 	Timer IRQ Enable Set (Timer IRQ Freq = Timer Clock / Timer limit)
#define TM1_IRQ_EN		1
#define TM2_IRQ_EN		1
#define TM3_IRQ_EN		1
#define TM4_IRQ_EN		1
#define TM5_IRQ_EN		1

#define TIMER_REG_SET(CH)	TIM_CH##CH##_DIV = TM##CH##_TGT_DIV;\
							TIM_CH##CH##_LMT = TgtToLimit(BUS_CLK,TM##CH##_TGT_FRQ,TM##CH##_TGT_DIV);\
							TIM_CH##CH##_TRIG = TIM_CH##CH##_LMT-1;\
							TIM_CH##CH##_EN = 1;\
							TIM_CH##CH##_CKEN = 1;\
							TIM_CH##CH##_PWM_EN = TM##CH##_PWM_EN;\
							TIM_CH##CH##_IRQ_EN = TM##CH##_IRQ_EN;


//*************************************************************************************************
// 8. SPI
//-------------------------------------------------------------------------------------------------

#define	SPI_CLK_SET(CH,SPEED)	SPI##CH##_CLKDIV = (MCLK/(4*SPEED)-1)


//*************************************************************************************************
// 12. ISP (EN673)
//-------------------------------------------------------------------------------------------------
// Misc
#define	ISP_BASE				ISP_REG_BASE	// Normal reg start addr.

//-------------------------------------------------------------------------------------------------
// Extern
extern void SetFontAttrs(UINT anPosY, UINT anPosX, UINT anLen, UINT anAttr);
extern UINT DispStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anLen);
extern void DispClr(UINT anPosY, UINT anPosX, UINT anLen);
extern void DispClrStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen);
#if model_CharMax == 1
extern void DispStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anLen);
extern void DispClrStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen);
#endif
//	extern UINT DispImg(UINT idx, UINT anPosY, UINT anPosX, UINT anCnt);


//*************************************************************************************************
// 14. ETC
//-------------------------------------------------------------------------------------------------

#define	MN_LENS_MNL		0
#define	MN_LENS_DC		1	// DC
#define	MN_LENS_AUTO	1	// pIRIS, AF AUTO

#ifndef __ISP_LIB__
#include "msg.h"

enum {
	MP_START=0,			// Head

	// System
	FrameMode,
	MpFrameRateLv,
	MpSysFreq,
	MpOutFps,
	//MpOutScl,
	//MpOutSclUser,
	MpCBar,
	MpReset,
	MpLanguageSel,
	MpOutFromat,
	MpCamID,
	MpBaudrate,
	MpCamTitleOn,
	MpCamTitleIdx,
	MpCamTitle = MpCamTitleIdx+8-1,

	// AE
	MpBrightness,
	MpBrightnessMin,
	MpIris,
	MpIrisLvl,
	MpShutter,
	MpShutSpd,
	MpShtMin,
	MpDeblurMin,
	MpShtMax,
	MpDcMode,
	MpShtMode,
	MpDss,
	MpAgc,
	MpAgcMin,		// AGC_MIN_
	MpLSpotLv,		// LEVEL
	MpLSpotTh,		// x
	MpAgcSpeed,		// AGC_SLOPE
	MpIrsSpeed,		// IRIS_SLOPE
	MpOpenSpeed,	// DtoB_SLOPE
	MpCloseSpeed,	// BtoD_SLOPE
	MpSmallSpeed,	// AE_SPEED
	MpAllSpeed,
	//MpClipMax,
	MpSatBrt,		// WEIGHT
	MpAdptAe,		// x
	MpMinGammaMode, // nBACKLIGHT
	MpMinGammaY,	// nBACKLIGHT
	//MpMinGammaC,
	MpErrChtMax,	// STABILIZING -> 
	MpErrChtMin,	// STABILIZING -> 
	MpStabilizing,	// STABILIZING -> AntiSatStabilizingSet() -> MP(MpErrChtMax) & MP(MpErrChtMin)
	MpSpotSpd,		// CTRL. SPEED
	MpAntiSatOn,	// ANTI_SAT

	// Backlight
	MpBackLight,
	MpBackLightNor,
	MpHlMaskThrs,
	MpHlMaskColor,
	MpBlcMode,	
	MpBlcPosX,
	MpBlcPosY,
	MpBlcSizX,
	MpBlcSizY,
	MpWdrBlc,		// WDR+BLC
	MpBlcOsdOn,		// for webpage

	//WDR_ROI	
#if model_WDR_ROI
	MpROIWdrMode,
	MpROIWdrZone,
	MpROIWdrCfgIdx,
	MpROIWdrCfg = MpROIWdrCfgIdx+(9*4)-1,
#endif
	MpWdrWgt,
	MpWdrABrt,
	MpWdrABrtS,

	// TDN, LED
	MpTdn,
	MpTdnThrs,
	MpTdnMarg,
	MpTdnDly,
	MpLedOn,
	MpLedSatCtrl,
	//MpSmartIR,
	MpTdnSw,
	MpTdnDNThres,
	MpTdnNDThres,

	// ACE,Defog
	MpAce,
	MpAceGmgn,
	MpAceBrt,
	MpAceCont,
	MpDefog,
	MpDefogMode,
	MpDefogLevel,
	MpWdrDBrt,
	MpWdrDCont,

	// AWB
	MpAwb,
	MpAwbPrst,
	MpAwbMnl,
	MpRgain,
	MpBgain,
	MpSaturation,
	MpSaturationR,	// 2017419 - WHL : CBB TEST
	MpSaturationB,
	MpgainBn,
	MpgainBp,
	MpgainRn,
	MpgainRp,
	MphueBn,
	MphueBp,
	MphueRn,
	MphueRp,
	MpAwbSpeed,
	MpAwbAgcWgtMin,
	MpLSUP_ON,
	MpCES_ON,
	MpWdrCES_ON,
	MpAwbStyle,

	MpAwbYSliceMin,
	MpAwbYSliceMax,
	MpAwbSliceCntMin,
	MpAwbSliceCntMax,

	// ETC
	MpAdnr,
	//MpHdaSharpness,
	//MpSharpnessSel,
	MpSharpness,
	MpShpSmallEdge,
	MpShpBigEdge,
	MpShpAgcLow,
	MpShpAgcMid,
	MpShpAgcHigh,
	MpGamma,
	MpGammaDay,
	MpGammaNgt,
	MpGammaWdr,
	MpFlip,
	MpMirror,
	MpShading,
	MpShdWeight,
	MpDnrIncrease,

	// Privacy Box
	MpPrivacy,
	MpPvcZone,
	MpPvcFormat,
	MpPvcTrans,
	MpPvcYLevel,
	MpPvcCRLevel,
	MpPvcCBLevel,
	MpPvcCfgIdx,
	MpPvcCfg = MpPvcCfgIdx+(5*32)-1,

	// IMD
	MpItl,
	//MpItlRes,
 	MpItlSens,
 	MpItlDettone,
 	MpItlRectFill,
	MpItlWindowuse,
	MpItlWinCfgIdx,
	MpItlWinCfg = MpItlWinCfgIdx+(5*4)-1,
 	MpItlMaskOsd,
 	MpItlAlarm,
 	MpItlSigno,

	// Hidden MENU
	MpShdDet,
	MpDefDet,
	//MpDefectThrs,

	// Test
	//MpClockOnOff,
	//MpTest,

#if(model_Lens==0)
	MpFad_On,
	MpFad_Zone,		// 150325 WHL - Focus Adj.
	MpFad_Trans,
	MpFad_PosX,
	MpFad_PosY,
	MpFad_SizX,
	MpFad_SizY,
	
#elif(model_Lens==1)
	MpMAF_mode,
	MpMAF_LensModel,
	MpMAF_Filter,
	MpMAF_MotorInit,
	MpMAF_TestMode,
	MpMAF_TotalPos_Z,
	MpMAF_StartPos_Z,
	MpMAF_EndPos_Z,
	MpMAF_TotalPos_F,
	MpMAF_StartPos_F,
	MpMAF_EndPos_F,

#endif

#if (IMAGE_INPUT_MODE==1)
	MpPipOn,	// 20151113 - WHL : PIP Menu Setup
	MpPipColor,
	MpPip1_PosX,
	MpPip1_PosY,
	MpPip1_SizX,
	MpPip1_SizY,
#endif

#if(DOOR_BELL_CAMERA==1)
	MpDoorCam_LP_Mode,
#endif

	MP_END_REAL
};

#define MP(I)					gbMnParTbl[I]

#endif // #ifndef __ISP_LIB__

enum{	// common
   	MN_OFF,
	MN_ON
};

enum {	// common
	MN_3sLOW,
	MN_3sMID,
	MN_3sHI
};

enum {	// common
   	MN_4sOFF,
	MN_4sLOW,
	MN_4sMID,
	MN_4sHI
};

#if (model_2M30p)

enum {	// MP(FrameMode)
	MN_FRAMEMODE_NORMAL_25_30,
	MN_FRAMEMODE_WDR_12_15,
	MN_FRAMEMODE_DNR_25_30
};
	#define WDR_MODE	(MP(FrameMode) == MN_FRAMEMODE_WDR_12_15)
	#define DNR_MODE	(MP(FrameMode) == MN_FRAMEMODE_DNR_25_30)

#else	// (model_1M)|(model_2M)

enum {	// MP(FrameMode)
	MN_FRAMEMODE_NORMAL_25_30,
	MN_FRAMEMODE_WDR_12_15,
	MN_FRAMEMODE_DNR_25_30,
	MN_FRAMEMODE_NORMAL_50_60,
	MN_FRAMEMODE_WDR_25_30,
	MN_FRAMEMODE_DNR_50_60
};
	#define WDR_MODE	(MP(FrameMode) == MN_FRAMEMODE_WDR_12_15 || MP(FrameMode) == MN_FRAMEMODE_WDR_25_30)
	#define DNR_MODE	(MP(FrameMode) == MN_FRAMEMODE_DNR_25_30 || MP(FrameMode) == MN_FRAMEMODE_DNR_50_60)
#endif

enum {	// MP(MpSysFreq)
	MN_SYSFREQ_50,
	MN_SYSFREQ_60
};

enum{	// MP(MpOutFps)
	MN_2M_1080p_25_30,
	MN_2M_1080p_50_60,
	MN_2M_720p_25_30,
	MN_2M_720p_50_60
};

enum{	// MP(MpOutFps)
	MN_2M30_1080p_25_30,
	MN_2M30_720p_25_30,
	MN_2M30_720p_50_60
};

enum{	// MP(MpOutFps)
	MN_1M_720p_25_30,
	MN_1M_720p_50_60
};

enum{	//MP(MpOutFromat)
	MN_FRMT_SD,
	MN_FRMT_HD,
	MN_FRMT_UV
};

enum{
	MN_BR_2400,
	MN_BR_4800,
	MN_BR_9600,
	MN_BR_57600,
	MN_BR_115200
};

enum {	//MP(MpShutter)
	MN_SHUT_AUTO,
	MN_SHUT_MNL,
	MN_SHUT_FLICKER
};

enum{	//MP(MpShutSpd)
	MN_MNL_SHUT_DIV1,	// 1125 line, 1/30 sec
	MN_MNL_SHUT_DIV2,	// 562 line, 1/60 sec
	MN_MNL_SHUT_DIV4,	// 281 line, 1/120 sec
	MN_MNL_SHUT_DIV8,	// 140 line, 1/250 sec
	MN_MNL_SHUT_DIV16,	// 70 line, 1/500 sec
	MN_MNL_SHUT_DIV32,	// 35 line, 1/1000 sec
	MN_MNL_SHUT_DIV64,	// 17 line, 1/2000 sec
	MN_MNL_SHUT_DIV128,	// 8 line, 1/4000 sec
	MN_MNL_SHUT_DIV256,	// 4 line, 1/8000 sec
	MN_MNL_SHUT_DIV512,	// 2 line, 1/15000 sec
	MN_MNL_SHUT_DIV1024	// 1 line, 1/30000 sec
};

enum{	//MP(MpDcMode)
	MN_DC_IN,
	MN_DC_OUT,
	MN_DC_DEBLUR,
	MN_DC_CUSTOM
};

enum{	//MP(MpShtMode)
	MN_SHT_NORMAL,
	MN_SHT_DEBLUR,
	MN_SHT_CUSTOM
};

enum {	//MP(MpDss)	// 14.2.13
	MN_DSS_OFF	,
	MN_DSS_X2	,
	MN_DSS_X4	,
	MN_DSS_X8	,
	MN_DSS_X16	,
	MN_DSS_X32	,
	MN_DSS_X64
};

enum{	//MP(MpBackLight)
	MN_BL_OFF,
	MN_BL_HLC,
	MN_BL_BLC,
	MN_BL_WDR
};

enum {	//MP(MpTdn)
	MN_TDN_AUTO,
	MN_TDN_DAY,
	MN_TDN_NIGHT,
	MN_TDN_EXTERN
};

enum{	//MP(MpDefogMode)
	MN_DEFOG_MANUAL,
	MN_DEFOG_AUTO
};

enum {	//MP(MpAwb)
	MN_AWB_AUTO,
	MN_AWB_AUTOEXT,
	MN_AWB_PRESET,
	MN_AWB_MNL
};

enum {	//MP(MpGamma)
	MN_GAMMA_045,
	MN_GAMMA_050,
	MN_GAMMA_055,
	MN_GAMMA_060,
	MN_GAMMA_065,
	MN_GAMMA_070,
	MN_GAMMA_075,

	MN_GAMMA_AUTO,

	MN_GAMMA_WDR
};

enum {	//MP(MpCamTitleOn)
	MN_CT_OFF,
	MN_CT_RIGHT_UP,
	MN_CT_LEFT_DOWN
};

enum {	//MP(MpAdptAe)
	MN_ADPT_AE_OFF,
	MN_ADPT_AE_AGC,
	MN_ADPT_AE_BR,
	MN_ADPT_AE_ML	// iTgtSpotBg : | SHT_MIN+10 ~ SHT_MAX | -> iTgtSpotBgOri ~ (iTgtSpotBgOri>>2) , SHT제어가 빨라서 안정화 어려움
};

enum {	//MP(MpMinGammaMode)
	MN_SAT_BACKLIGHT_ALWAYS,
	MN_SAT_BACKLIGHT_DAY_ONLY
};

enum {	//MP(MpBlcMode)
	MN_BLC_MODE_CUSTOMIZE,
	MN_BLC_MODE_CENTER,
	MN_BLC_MODE_SPOT
};

#define PVC_EA				16//32			// number of Privacy Box (max 32ea, CAUTION. share with IMD box ea)

typedef struct _tagPRIVACY{
	BYTE	bAction;
	BYTE	bPosX;
	BYTE	bPosY;
	BYTE	bSizX;
	BYTE	bSizY;
} _PRIVACY;

extern void PrivacyBox(void);

//------------------------------------------------------------------------------
// Last Box apply
extern UINT gnBoxPosPVC[PVC_EA*2];
extern UINT gnBoxPltPVC[PVC_EA];
extern UINT gnBoxTonePVC[2];
extern UINT gnBoxFillPVC;
extern UINT gnBoxOnPVC;
extern UINT gnBoxFmtPVC;	// 2015112 - WHL : Add Bit Box Mode

#define gbMnPvcCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpPvcCfgIdx +((INDEX))*5))

#define gbMnImdCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpItlWinCfgIdx +((INDEX))*5))

#endif // end of "model_Sens"

/* EOF */


