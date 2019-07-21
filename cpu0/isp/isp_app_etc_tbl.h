/* **************************************************************************
 File Name	: 	app_etc_tbl.h
 Description:	EN673 - etc. Functions
 Designer	:	Lee, Hoon
 Date		:	12. 10. 29
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

int giIspAgc[3]={0,0,0};
BYTE gbIspAgcSta=0;
BYTE gbIspAgcEnd=0;

//*************************************************************************************************
// etc Function Table
//-------------------------------------------------------------------------------------------------
// ADNR
BYTE gbAdnr_Excute=0;
BYTE gbAdnr_WaitCnt=3;

#if (model_Sens == SENS_MN34227) || (model_Sens == SENS_MN34229) || (model_Sens == SENS_MN34420)
 	const int Adnr3DFkTbl[3][3]	= 	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x10,		0x40,		0x70	},		// Low strength
										{0x20, 		0x50, 		0x80	},		// Mid	"
										{0x30, 		0x60, 		0x90	}		// Hi	"
									};

	const int Adnr3DGainTbl[3][3] =	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x28,		0x10,		0x0c	},		// Low strength
										{0x20, 		0x0c, 		0x08	},		// Mid	"
										{0x18, 		0x08, 		0x04	}		// Hi	"
									};

	const int Adnr3DThrsTbl[3][3] =	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x00,		0x00,		0x00	},		// Low strength
										{0x00, 		0x00, 		0x00	},		// Mid	"
										{0x00, 		0x00, 		0x00	}		// Hi	"
									};
	
#else
	const int Adnr3DFkTbl[3][3]	= 	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x90,		0xc0,		0xe0	},		// Low strength
										{0xa0, 		0xd0, 		0xe0	},		// Mid	"
										{0xb0, 		0xd8, 		0xe0	}		// Hi	"
									};

	const int Adnr3DGainTbl[3][3] =	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x28,		0x10,		0x0c	},		// Low strength
										{0x20, 		0x0c, 		0x08	},		// Mid	"
										{0x18, 		0x08, 		0x04	}		// Hi	"
									};

	const int Adnr3DThrsTbl[3][3] =	{//  Low Agc,	Mid Agc,	Hi Agc
										{0x00,		0x00,		0x00	},		// Low strength
										{0x00, 		0x00, 		0x00	},		// Mid	"
										{0x00, 		0x00, 		0x00	}		// Hi	"
									};
#endif

//-------------------------------------------------------------------------------------------------
// Aperture
const int Apt55ThTbl[3]  =   //  Low Agc,	Mid Agc,	Hi Agc
								{0x10,		0xa0,		0x100	};	// 2015827 - WHL
//								{0x20,		0xa0,		0x100	};	// 2015827 - WHL

#if 0
const int Apt55GainTbl[3]=   //  Low Agc,	Mid Agc,	Hi Agc
								{0x30,		0x20,		0x10	};		// 150526 LH
//								{0x20,		0x18,		0x10	};	// 20170509 - WHL
#endif

const int Apt33ThTbl[3] =   //  Low Agc,	Mid Agc,	Hi Agc
								{0x20,		0x100,		0x100	};	// 2015827 - WHL
//								{0x40,		0x100,		0x100	};	// 20170509 - WHL

#if 0
const int Apt33GainTbl[3]=   //  Low Agc,	Mid Agc,	Hi Agc
									{0x80,		0x0,		0x0		};
#endif

//-------------------------------------------------------------------------------------------------
// Color Suppression
BYTE gbSup_Excute=0;

#if(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX327)||(model_Sens==SENS_IMX307)
							//Low Agc,	Mid Agc,	Hi Agc
const int LowCSupGaTbl[3] = {0x38,		0x24,		0x10	}; // 14.2.10
#else
const int LowCSupGaTbl[3] = {0x20,		0x18,		0x10	}; // 14.2.10

#endif

const int CesGainTbl[2]  = {0x10, 0x20}; // WDR Off/On
const int CesLClipTbl[2] = {0x14, 0x0A};

//-------------------------------------------------------------------------------------------------
// Privacy Color Table -            Y,  Cb, Cr
const BYTE gbPrvcYCbCrTbl[8][3] = {{235,128,128/*WHT*/},{218,  8,139/*YEL*/},{185,155,  8/*CYN*/},{168, 35, 19/*GRN*/},
								   { 67,221,237/*MAG*/},{ 50,101,248/*RED*/},{ 17,248,117/*BLU*/},{  0,128,128/*BLK*/}};

//-------------------------------------------------------------------------------------------------
// Mirror / Flip

#if (model_Sens == SENS_IMX225)

	BYTE gbMirFlpBuf = 0x10;

	#define MIRR_ONw(ON)	{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD); \
							  SetSens(SENS_SONY_ID2, 0x07, gbMirFlpBuf); }

	#define FLP_ONw(ON)		{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE); \
							  SetSens(SENS_SONY_ID2, 0x07, gbMirFlpBuf); }

#elif (model_Sens == SENS_IMX291) || (model_Sens == SENS_IMX327) || (model_Sens == SENS_IMX307)

	BYTE gbMirFlpBuf = 0x00;
	WORD gwSonyID = (SENS_CONTROL_MODE) ? SENS_SONY_ID2_TWI : SENS_SONY_ID2;
		

	#define MIRR_ONw(ON)	{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD); \
							  SetSens(gwSonyID, 0x07, gbMirFlpBuf); }

	#define FLP_ONw(ON)		{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE); \
							  SetSens(gwSonyID, 0x07, gbMirFlpBuf); }

	
#elif (model_Sens == SENS_IMX323)

	BYTE gbMirFlpBuf = 0x00;

	#if (SENS_CONTROL_MODE) // TWI
	#define MIRR_ONw(ON)	{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE); \
							  SetSens(0x01, 0x01, gbMirFlpBuf); }

	#define FLP_ONw(ON)		{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD); \
						  	  SetSens(0x01, 0x01, gbMirFlpBuf); }
	#else // SPI
	#define MIRR_ONw(ON)	{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x02) : (gbMirFlpBuf&0xFD); \
							  SetSens(SENS_SONY_ID2, 0x01, gbMirFlpBuf); }

	#define FLP_ONw(ON)		{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x01) : (gbMirFlpBuf&0xFE); \
							  SetSens(SENS_SONY_ID2, 0x01, gbMirFlpBuf); }
	#endif

#elif (model_Sens == SENS_MN34227) // Flip Only

	WORD gwMirFlpBuf=0x0001;

	#define MIRR_ONw(ON)	{}//UNUSED(ON)

	#define FLP_ONw(ON)		{ gwMirFlpBuf = (ON) ? (gwMirFlpBuf|0x0200) : (gwMirFlpBuf&0xFDFF); \
							  SetSens(0, 0x100, gwMirFlpBuf); }

#elif (model_Sens == SENS_AR0237CSH)

	WORD gwMirFlpBuf=0;

	#define MIRR_ONw(ON)	{ gwMirFlpBuf = (ON) ? (gwMirFlpBuf|0x4000) : (gwMirFlpBuf&0xBFFF); \
							  SetSensTwi_Aptn(SENS_AR0230_DEV, 0x3040, gwMirFlpBuf); }

	#define FLP_ONw(ON)		{ gwMirFlpBuf = (ON) ? (gwMirFlpBuf|0x8000) : (gwMirFlpBuf&0x7FFF); \
							  SetSensTwi_Aptn(SENS_AR0230_DEV, 0x3040, gwMirFlpBuf); }

#elif (model_Sens == SENS_OV2718)

	BYTE gbMirFlpBuf=0;

	#define MIRR_ONw(ON)	{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf&0xFB) : (gbMirFlpBuf|0x04); \
							  SetSensTwi_Omni(SENS_OV2718_DEV, 0x30c0, gbMirFlpBuf); }

	#define FLP_ONw(ON)		{ gbMirFlpBuf = (ON) ? (gbMirFlpBuf|0x08) : (gbMirFlpBuf&0xF7); \
							  SetSensTwi_Omni(SENS_OV2718_DEV, 0x30c0, gbMirFlpBuf); }


#else	// Not Support Mirr/Flip Functions in EN673
	#define MIRR_ONw(ON)	{}
	#define FLP_ONw(ON)		{}

#endif

// H-Total Count of Pre-Processing
#if (model_1M)
	#if (ISP_CLK_MODE==1)	// 1.3M - 162MHz   (60Hz / 50Hz)
		UINT gnHCNTI_SV_Tbl[2] = {0x707, 0x86f};

	#else					// 1.3M - 148.5MHz (60Hz / 50Hz)
		UINT gnHCNTI_SV_Tbl[2] = {0x671, 0x7bb};
	
	#endif

#elif (model_2M|model_2M30p)
	#if (ISP_CLK_MODE==1)	// 2M - 162MHz   (60Hz / 50Hz)
		UINT gnHCNTI_SV_Tbl[2] = {0x95f, 0xb3f};

	#else					// 2M - 148.5MHz (60Hz / 50Hz)
		UINT gnHCNTI_SV_Tbl[2] = {0x897, 0xa4f};
	
	#endif
#endif


/* EOF */

