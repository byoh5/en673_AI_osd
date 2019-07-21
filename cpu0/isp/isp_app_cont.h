/* **************************************************************************
 File Name	:	app_cont.h
 Description:	EN673 - Gamma, ACE & Defog function Header
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"

//=================================================================================================
//	reserved define & tables
//=================================================================================================
#define HISTO_MAX_BIT	7
#define HISTO_MAX_NUM	15
#define HISTO_VAL_NUM	(HISTO_MAX_NUM+1)

#define HISTO_OUT_BIT	/*3*/2						// Histogram Width


#if (model_1M)	// 2016313 - WHL
	
#define HistoX	0x200
#define HistoY	0xCC
#define HistoW	(1<<(HISTO_MAX_BIT+HISTO_OUT_BIT))
#define HistoH	400							// Histogram Height

#else

#define HistoX	1350/*50*//*1200*//*1000*/
#define HistoY	40/*620*//*460*//*400*/
#define HistoW	(1<<(HISTO_MAX_BIT+HISTO_OUT_BIT))
#define HistoH	400/*200*/							// Histogram Height
#endif


#define HistoFill	1
#define HistoTone	BOX_TONE_100

BYTE gbHistoM[HISTO_VAL_NUM];


#define ACEBLOCK_ON		ACE_BPw(0);						// Block bypass OFF(use)/ON
#define ACEBLOCK_OFF	ACE_BPw(1);

#define ACE_ON			ACE_ONw(1);						// ACE ON/OFF
#define ACE_OFF			ACE_ONw(0);

#define DEFOG_ON		HEQ_ONw(1);						// DEFOG ON/OFF
#define DEFOG_OFF		HEQ_ONw(0);


// -------------------------------------------------------------------------
// Gamma table
//#if (model_Contrast==1)	// New

const UINT	gnTblGamma_NY0[9] = {0x0000003d,0x00540073,0x009d00d7,0x01260191,0x01e20224,0x025e0292,0x02ed033d,0x038403c4,0x03ff0000}; // Y gamma-0 (0.45)
const UINT	gnTblGamma_NY1[9] = {0x00000009,0x0010001a,0x002d004c,0x008000d7,0x0123016a,0x01ac01eb,0x026102d0,0x0339039f,0x03ff0000}; // Y gamma-1 (0.75)
const UINT	gnTblGamma_NC0[9] = {0x0000009d,0x00c200ee,0x0126016a,0x01bd0224,0x026b02a3,0x02d202fb,0x033f0379,0x03ab03d8,0x03ff0000}; // c gamma-0 (0.3)
const UINT	gnTblGamma_NC1[9] = {0x00000018,0x00240037,0x00540080,0x00c20126,0x017701bd,0x01fd0238,0x02a30304,0x035e03b1,0x03ff0000}; // c gamma-1 (0.6)

//#else // Old		// 180426 KSH

const UINT	gnTblGamma_OY0[9] = {0x12      ,0x28005d  ,0xaf0110  ,0x183021b ,0x28a02e7 ,0x337037e ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};
//const UINT	gnTblGamma_OY1[9] = {0x9       ,0x140026  ,0x46007a  ,0xd20166  ,0x1e8025f ,0x2cc0337 ,0x3fe03ff ,0x3ff03ff ,0x3ff0000};
const UINT	gnTblGamma_OC0[9] = {0x18      ,0x320080  ,0xf00168  ,0x1e0026e ,0x2d0031f ,0x360039a ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};
//const UINT	gnTblGamma_OC1[9] = {0xd       ,0x190030  ,0x560096  ,0xf80192  ,0x2120282 ,0x2ea0348 ,0x3fc03ff ,0x3ff03ff ,0x3ff0000};

//#endif

const UINT	gnTblGamma_WY0[9] = {0x6       ,0x11002f  ,0x64009f  ,0xd9011d  ,0x1520182 ,0x1b901f9 ,0x2760302 ,0x38603de ,0x3ff0000};	// 14.09.04	0.7_1023 base
const UINT	gnTblGamma_WY1[9] = {0x0000003c,0x005b0082,0x00be0122,0x018f01e3,0x02130244,0x028502d4,0x035e03bb,0x03e803ff,0x03ff0000};	// 150802
const UINT	gnTblGamma_WC1[9] = {0x00000006,0x00120046,0x00960104,0x016801c7,0x0203023f,0x028002d0,0x035c03b6,0x03e803ff,0x03ff0000};	// 151118


#define		gnTblGamma_75		gnTblGamma_NY1
const UINT	gnTblGamma_70[9]  = {0x0000000d,0x00150022,0x0037005a,0x009300ef,0x013d0184,0x01c50203,0x027602e1,0x034503a5,0x03ff0000};
const UINT	gnTblGamma_65[9]  = {0x00000011,0x001b002b,0x0044006b,0x00a90109,0x015901a0,0x01e1021d,0x028c02f2,0x035103ab,0x03ff0000};
#define		gnTblGamma_60		gnTblGamma_NC1
const UINT	gnTblGamma_55[9]  = {0x00000021,0x00300047,0x00680098,0x00de0146,0x019801dd,0x021c0255,0x02bb0317,0x036a03b8,0x03ff0000};
const UINT	gnTblGamma_50[9]  = {0x0000002d,0x0040005a,0x008000b5,0x0100016a,0x01bb0200,0x023c0273,0x02d40329,0x037703be,0x03ff0000};
#define		gnTblGamma_45		gnTblGamma_NY0
const UINT	gnTblGamma_40[9]  = {0x00000054,0x006f0093,0x00c20100,0x015101bd,0x020c024c,0x028302b3,0x03080350,0x039103cb,0x03ff0000};
const UINT	gnTblGamma_35[9]  = {0x00000073,0x009300bb,0x00ee0130,0x018401ee,0x023a0276,0x02a902d6,0x03230364,0x039e03d1,0x03ff0000};
#define		gnTblGamma_30		gnTblGamma_NC0
//const UINT	gnTblGamma_25[9]  = {0x000000d7,0x01000130,0x016a01ae,0x02000261,0x02a102d4,0x02fd0321,0x035d038e,0x03b903de,0x03ff0000};
//const UINT	gnTblGamma_20[9]  = {0x00000126,0x01510184,0x01bd0200,0x024c02a3,0x02dc0308,0x032b0349,0x037b03a4,0x03c603e5,0x03ff0000};
//const UINT	gnTblGamma_15[9]  = {0x00000191,0x01bd01ee,0x02240260,0x02a302ed,0x031c033f,0x035c0374,0x039b03ba,0x03d403eb,0x03ff0000};

const UINT*	gpTblGamma[] = { gnTblGamma_40/*gnTblGamma_30*/, gnTblGamma_40/*gnTblGamma_35*/, gnTblGamma_40, gnTblGamma_45, gnTblGamma_50, gnTblGamma_55, gnTblGamma_60, gnTblGamma_65, gnTblGamma_70, gnTblGamma_75 };


// -------------------------------------------------------------------------
// ACE table
const int AceTbl[3][2] = { 	// TH1, TH2
							{0x10, 0x08},	// Low
							{0x30, 0x18},	// Mid
							{0x50, 0x20}	// Hi
							};

const int AceTblWdr[3][2] = {// TH1, TH2	// 14.09.04
							{0x30, 0x8},	// Low
							{0x40, 0x10},	// Mid
							{0x50, 0x18}	// Hi
							};


//*************************************************************************************************
// Variables
//-------------------------------------------------------------------------------------------------
BYTE gbDebugOn=0, gbGamma_Excute=0;


//=================================================================================================
//	Functions
//=================================================================================================
void InitHisto(void);
void InitContrast(void);
UINT GetGamma(const UINT nG1, const UINT nG2, const UINT nGK);
void Gamma(void);
void AceDefog(void);
void Histo(void);


/* EOF */

