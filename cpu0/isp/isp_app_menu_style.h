/* **************************************************************************
 File Name	: 	isp_app_menu_style.h
 Description:	EN673 - Menu Style
 Designer	:	Kim, Sunghoon
 Date		:	18. 07. 26
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#define MenuValue(NAME, ...) {\
	const BYTE bMenu_##NAME[] = { __VA_ARGS__ };\
	if(abMenuStyle==0) MP(NAME) = gbMnParTblOld[NAME];\
	else if((UINT)(abMenuStyle-1) < numberof(bMenu_##NAME)) MP(NAME) = bMenu_##NAME[abMenuStyle-1];\
	else MP(NAME) = bMenu_##NAME[0];\
}

//*************************************************************************************************
// Value
//-------------------------------------------------------------------------------------------------
	#define MN_IRIS_OPEN	MN_LENS_MNL

#if(model_Iris_DC)
	#define MN_IRIS_AUTO	MN_LENS_DC
#elif(model_Iris_P)
	#define MN_IRIS_AUTO	MN_LENS_AUTO
#else
	#define MN_IRIS_AUTO	MN_LENS_MNL
#endif

#if (model_2M30p)
	#define MN_MODE_NOR		MN_FRAMEMODE_NORMAL_25_30
	#define MN_MODE_WDR		MN_FRAMEMODE_WDR_12_15
	#define MN_MODE_DNR		MN_FRAMEMODE_DNR_25_30
#else
	#define MN_MODE_NOR		MN_FRAMEMODE_NORMAL_25_30
	#define MN_MODE_WDR		MN_FRAMEMODE_WDR_25_30
	#define MN_MODE_DNR		MN_FRAMEMODE_DNR_25_30
#endif


//-------------------------------------------------------------------------------------------------
#if (model_Sens==SENS_OV2718)// || (model_Sens==SENS_IMX327)
	#define BRT_DAY_INDOOR	12
	#define BRT_NGT_INDOOR	17/*12*/
	#define BRT_DAY_OUTDOOR	10
	#define BRT_NGT_OUTDOOR	12
	#define BRT_DAY_CAR_REC	10/*8*//*7*/
	#define BRT_NGT_CAR_REC	8/*10*//*11*/
#else
	#define BRT_DAY_INDOOR	10
	#define BRT_NGT_INDOOR	17/*12*/
	#define BRT_DAY_OUTDOOR	10
	#define BRT_NGT_OUTDOOR	12
	#define BRT_DAY_CAR_REC	10/*8*//*7*/
	#define BRT_NGT_CAR_REC	8/*10*//*11*/
#endif

#if (model_Sens==SENS_IMX327)
	#define R_GAIN_INDOOR	19
	#define G_GAIN_INDOOR	19
	#define B_GAIN_INDOOR	20
	#define R_GAIN_OUTDOOR	18
	#define G_GAIN_OUTDOOR	18
	#define B_GAIN_OUTDOOR	20
	#define R_GAIN_CAR_REC	20
	#define G_GAIN_CAR_REC	20
	#define B_GAIN_CAR_REC	22
#else
	#define R_GAIN_INDOOR	20
	#define G_GAIN_INDOOR	20
	#define B_GAIN_INDOOR	20
	#define R_GAIN_OUTDOOR	20
	#define G_GAIN_OUTDOOR	20
	#define B_GAIN_OUTDOOR	20
	#define R_GAIN_CAR_REC	20
	#define G_GAIN_CAR_REC	20
	#define B_GAIN_CAR_REC	20
#endif

#if (model_Sens==SENS_IMX291)
	#define AWB_STYLE_CBB	1
#else
	#define AWB_STYLE_CBB	0
#endif


//*************************************************************************************************
// Table
//-------------------------------------------------------------------------------------------------
STRING1(STYLE,		_B,"STYLE");

STRING1(PREVIOUS,	_B,"PREVIOUS");
STRING1(IPC_INDOOR,	_B,"IPC INDOOR");
STRING1(IPC_OUTDOOR,_B,"IPC OUTDOOR");
STRING1(CAR_REC,	_B,"CAR REC");
STRING1(ACTION_REC,	_B,"ACTION REC");
//STRING1(WDR,		_B,"WDR");			// 이미 isp_app_menu_tbl.h에 있음
STRING1(LOW_BIT,	_B,"LOW BIT");
STRING1(CUSTOM,		_B,"CUSTOM");


#define INIT_STYLE	1	// 1:IPC_INDOOR, 2:IPC_OUTDOOR, 3:CAR_REC, 4:ACTION_REC, 5:WDR, 6:LOW_BIT, 7:CUSTOM


#define MENU_STYLE_SET	\
/*                         IPC_INDOOR,      IPC_OUTDOOR,     CAR_REC,         ACTION_REC,      WDR,             LOW_BIT,         CUSTOM        */\
MenuValue(MpIris,          MN_IRIS_OPEN,    MN_IRIS_OPEN,    MN_IRIS_OPEN,    MN_IRIS_OPEN,    MN_IRIS_OPEN,    MN_IRIS_AUTO,    MN_IRIS_AUTO   )\
MenuValue(MpShtMode,       MN_SHT_NORMAL,   MN_SHT_NORMAL,   MN_SHT_NORMAL,   MN_SHT_DEBLUR,   MN_SHT_NORMAL,   MN_SHT_NORMAL,   MN_SHT_NORMAL  )\
MenuValue(MpDcMode,        MN_DC_IN)\
MenuValue(MpBackLightNor,  MN_BL_OFF)\
MenuValue(MpBrightness,    BRT_DAY_INDOOR,  BRT_DAY_OUTDOOR, BRT_DAY_CAR_REC, BRT_DAY_INDOOR,  BRT_DAY_INDOOR, 	BRT_DAY_INDOOR,  BRT_DAY_INDOOR )\
MenuValue(MpBrightnessMin, BRT_NGT_INDOOR,  BRT_NGT_OUTDOOR, BRT_NGT_CAR_REC, BRT_NGT_INDOOR,  BRT_NGT_INDOOR, 	BRT_NGT_INDOOR,  BRT_NGT_INDOOR )\
MenuValue(MpAgc,           200,             200,             20,              20,              200,             200,             200            )\
MenuValue(MpAntiSatOn,     MN_OFF,          MN_OFF,          MN_ON,           MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF         )\
MenuValue(MpSatBrt,        10,              10,              10,              10,              10,              10,              10             )/* AntiSat Weight */\
MenuValue(FrameMode,       MN_MODE_DNR,     MN_MODE_DNR,     MN_MODE_DNR,     MN_MODE_DNR,     MN_MODE_WDR,     MN_MODE_DNR,     MN_MODE_DNR    )\
MenuValue(MpWdrWgt,        MN_3sHI)\
MenuValue(MpAdnr,          MN_4sMID,        MN_4sMID,        MN_4sLOW,        MN_4sLOW,        MN_4sMID,        MN_4sMID,        MN_4sMID       )\
MenuValue(MpDnrIncrease,   MN_ON,           MN_ON,           MN_OFF,          MN_OFF,          MN_ON,           MN_ON,           MN_ON          )\
MenuValue(MpFrameRateLv,   MN_3sLOW)\
MenuValue(MpSharpness,     5,               5,               5,               5,               5,               1,               5              )\
MenuValue(MpShpSmallEdge,  128,             128,             64,              64,              128,             64,              128            )\
MenuValue(MpShpBigEdge,    10,              10,              8,               8,               10,              0,               10             )\
MenuValue(MpGamma,         MN_GAMMA_AUTO,   MN_GAMMA_AUTO,   MN_GAMMA_AUTO,   MN_GAMMA_AUTO,   MN_GAMMA_055,    MN_GAMMA_065,    MN_GAMMA_AUTO  )\
MenuValue(MpGammaDay,      MN_GAMMA_055,    MN_GAMMA_065,    MN_GAMMA_055,    MN_GAMMA_055,    MN_GAMMA_055,    MN_GAMMA_065,    MN_GAMMA_055   )\
MenuValue(MpGammaNgt,      MN_GAMMA_045,    MN_GAMMA_055,    MN_GAMMA_045,    MN_GAMMA_045,    MN_GAMMA_045,    MN_GAMMA_055,    MN_GAMMA_045   )\
MenuValue(MpAce,           MN_4sOFF)\
MenuValue(MpDefog,         MN_ON,           MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF         )\
MenuValue(MpDefogMode,     MN_DEFOG_MANUAL)\
MenuValue(MpDefogLevel,    MN_3sLOW)\
MenuValue(MpMirror,        MN_OFF,          MN_OFF,          MN_ON,           MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF         )\
MenuValue(MpFlip,          MN_OFF,          MN_OFF,          MN_ON,           MN_OFF,          MN_OFF,          MN_OFF,          MN_OFF         )\
MenuValue(MpTdn,           MN_TDN_AUTO,     MN_TDN_AUTO,     MN_TDN_DAY,      MN_TDN_DAY,      MN_TDN_AUTO,     MN_TDN_AUTO,     MN_TDN_AUTO    )\
MenuValue(MpSaturationR,   R_GAIN_INDOOR,   R_GAIN_OUTDOOR,  R_GAIN_CAR_REC,  R_GAIN_INDOOR,   R_GAIN_INDOOR, 	R_GAIN_INDOOR,   R_GAIN_INDOOR  )\
MenuValue(MpSaturation,    G_GAIN_INDOOR,   G_GAIN_OUTDOOR,  G_GAIN_CAR_REC,  G_GAIN_INDOOR,   G_GAIN_INDOOR, 	G_GAIN_INDOOR,   G_GAIN_INDOOR  )\
MenuValue(MpSaturationB,   B_GAIN_INDOOR,   B_GAIN_OUTDOOR,  B_GAIN_CAR_REC,  B_GAIN_INDOOR,   B_GAIN_INDOOR, 	B_GAIN_INDOOR,   B_GAIN_INDOOR  )\
MenuValue(MpAwbStyle,      0,               0,               AWB_STYLE_CBB,   0,               0,               0,               0              )

