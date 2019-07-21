/* **************************************************************************
 File Name	: 	app_mode.h
 Description:	EN673 - Out mode function
 Designer	:	Lee, Wanhee
 Date		:	16. 11. 1
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

//----------------------------------------------------------------------------------------
// Out Mode
//----------------------------------------------------------------------------------------	
BYTE gbSyncupCnt=6;

extern BYTE gbModeChgOn;

//----------------------------------------------------------------------------------------
// Common ISP Initial Table
//----------------------------------------------------------------------------------------

//#define gnTblIspInitCommon	Sens(gnTblIspInitCommon)
//#define gnTblIspModeCommon	Sens(gnTblIspModeCommon)
//#define gTblIspInitCommonEa	Sens(gTblIspInitCommonEa)
//#define gTblIspModeCommonEa	Sens(gTblIspModeCommonEa)

#if (model_1M)
	const UINT gnTblIspInitCommon[][IspInitSize] = {
			//  Sensor Number : 0   1          2   3   4          5   6   7   8   9
			//  Sensor Name   :     MN34210            IMX225         
			{0x0012, DFSEL_Sens(0x0,0x3000000 ,0x0,0x0,0x3000000 ,0x0,0x0,0x0,0x0,0x0)},  // SLV[31:30],CH_DSEL[29],CH_HSSEL[28],CH_VSSEL[27],TRG_DIN[26],TRG_HSI[25],TRG_VSI[24],POL_HSI[23],POL_VSI[22],SHSO_INV[21],SVSO_INV[20],SHSO_TRG[19],SVSO_TRG[18],IDDR_ON[17],ISDR_OP[16],DDRFF_RS[15],SORD[14],CATP[13],ASYNC_ON[12],VSYN_NAON[11],PDI_MOD[10:9],SYNC_BYS[8],ICSEL[5:4],INSEL_MOD[3],INSEL[2:0]
			{0x0014, DFSEL_Sens(0x0,0x00080007,0x0,0x0,0x00080007,0x0,0x0,0x0,0x0,0x0)},  // HLOCKI_POS[27:16],VLOCKI_POS[10:0]
			{0x0016, DFSEL_Sens(0x0,0x050002d0,0x0,0x0,0x050002d0,0x0,0x0,0x0,0x0,0x0)},  // HWI[27:16],VWI[10:0]
			{0x0018, DFSEL_Sens(0x0,0x82e20072,0x0,0x0,0x80000000,0x0,0x0,0x0,0x0,0x0)},  // OSYNC_MOD[31],VLOCKI2_POS[27:16],HLOCKI2_POS[11:0]
			{0x0029, DFSEL_Sens(0x0,0xffff0000,0x0,0x0,0xffff0000,0x0,0x0,0x0,0x0,0x0)},  // SYNC_COD0[31:16],SYNC_COD1[15:0]
			{0x002a, DFSEL_Sens(0x0,0x00000002,0x0,0x0,0x00000000,0x0,0x0,0x0,0x0,0x0)},  // SYNC_COD2[31:16],SYNC_COD3_0[15:0]
			{0x002b, DFSEL_Sens(0x0,0x00000003,0x0,0x0,0x08000ab0,0x0,0x0,0x0,0x0,0x0)},  // SYNC_COD3_1[31:16],SYNC_COD3_2[15:0]
			{0x002c, DFSEL_Sens(0x0,0x03000000,0x0,0x0,0x83e00000,0x0,0x0,0x0,0x0,0x0)},  // LVDS Sync, *BLANK_SOL_ON[1],*SONY_V_ON[0]
			{0x0038, DFSEL_Sens(0x0,0x0       ,0x0,0x0,0x0       ,0x0,0x0,0x0,0x0,0x0)},  // 15.04.06 WHL - CMP_GROFS[25:16],CMP_GBOFS[9:0] 0 -> 0x3C403C4
			{0x0039, DFSEL_Sens(0x0,0x0       ,0x0,0x0,0x0       ,0x0,0x0,0x0,0x0,0x0)},  // 15.04.06 WHL - CMP_ROFS[25:16],CMP_BOFS[9:0]	  0 -> 0x3C403C4
			{0x0041, DFSEL_Sens(0x0,0x10780   ,0x0,0x0,0x505     ,0x0,0x0,0x0,0x0,0x0)},  // Defect Area
			{0x0042, DFSEL_Sens(0x0,0x5043c   ,0x0,0x0,0x202DA   ,0x0,0x0,0x0,0x0,0x0)},  //		"
			{0x0051, DFSEL_Sens(0x0,0x3800d8  ,0x0,0x0,0x3800d8  ,0x0,0x0,0x0,0x0,0x0)},  // WDR Default Set
			{0x0052, DFSEL_Sens(0x0,0x680300  ,0x0,0x0,0x680300  ,0x0,0x0,0x0,0x0,0x0)},  //		" 
			{0x00da, DFSEL_Sens(0x0,0x201111  ,0x0,0x0,0x201111  ,0x0,0x0,0x0,0x0,0x0)},  // Edge Sensing RGB Interpolate Default Set
//			{0x00e8, DFSEL_Sens(0x0,0x5048504a,0x0,0x0,0x60606060,0x0,0x0,0x0,0x0,0x0)},  // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
//			{0x00e9, DFSEL_Sens(0x0,0x10f700ef,0x0,0x0,0xef00e2  ,0x0,0x0,0x0,0x0,0x0)},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			{0x00f1, DFSEL_Sens(0x0,0x80f81008,0x0,0x0,0x80f81008,0x0,0x0,0x0,0x0,0x0)}   // ACE init
			};

	// Common Mode initial
	const UINT gnTblIspModeCommon[][IspModeSize] = {	// Mode 별 고정값 - 함부로 수정하지 마시오!!!
			//Addr		720 25P		  720 30P		  720 50P		  720 60P
#if(ISP_CLK_MODE==1)
			{0x0017,	0x10de02ed	, 0xe0e02ed		, 0x86e02ed		, 0x70602ed	,0x0,0x0,0x0,0x0	},	// HTWO[31:16],VTWO[15:0]
#else
			{0x0017,	0xf7602ed	, 0xce202ed		, 0x7ba02ed		, 0x67002ed	,0x0,0x0,0x0,0x0	},	// HTWO[31:16],VTWO[15:0]
#endif
			{0x001a,	0x850c02d4	, 0x850c02d4	, 0x850c02d4	, 0x850c02d4,0x0,0x0,0x0,0x0	},	// HVWIO_SYN[31], HWO[27:16],  VWO[11:0] = 0x43f -> 0x440 for MD
			{0x00f0,	0x005000a1	, 0x005000a1	, 0x005000a1	, 0x005000a1,0x0,0x0,0x0,0x0	},	// DCBAR_ON[31],DCBAR_ST[27:16],DCBAR_HW[9:0]
			{0x0120,	0x41f0508	, 0x41f0508		, 0x41f0508		, 0x41f0508 ,0x0,0x0,0x0,0x0	} 	// MASK_CHAD[29:21],HALF_CHAD[20:12],FONT_HW[11:0]
        	};

#elif (model_2M)	// model_2M60p

	const UINT gnTblIspInitCommon[][IspInitSize] = {
			//  Sensor Number : 10  11         12         13         14         15         16  17  18         19
			//  Sensor Name   :     MN34229    MN34420    IMX307     IMX327     IMX291             AR0230CSH
			{0x0012, DFSEL_Sens(0x0,0x3001020 ,0x3001010 ,0x3000000 ,0x3000000 ,0x3000000 ,0x0,0x0,0xC000000 ,0x0)},  // SLV[31:30],CH_DSEL[29],CH_HSSEL[28],CH_VSSEL[27],TRG_DIN[26],TRG_HSI[25],TRG_VSI[24],POL_HSI[23],POL_VSI[22],SHSO_INV[21],SVSO_INV[20],SHSO_TRG[19],SVSO_TRG[18],IDDR_ON[17],ISDR_OP[16],DDRFF_RS[15],SORD[14],CATP[13],ASYNC_ON[12],VSYN_NAON[11],PDI_MOD[10:9],SYNC_BYS[8],ICSEL[5:4],INSEL_MOD[3],INSEL[2:0]
			{0x0014, DFSEL_Sens(0x0,0x0       ,0x00004   ,0x90000   ,0x90000   ,0x90000   ,0x0,0x0,0xf0010004,0x0)},  // HLOCKI_POS[27:16],VLOCKI_POS[10:0]
			{0x0016, DFSEL_Sens(0x0,0x788043f ,0x788043f ,0x788043f ,0x788043f ,0x788043f ,0x0,0x0,0x7880440 ,0x0)},  // HWI[27:16],VWI[10:0]
//			{0x0018, DFSEL_Sens(0x0,0x80000050,0x80100059,0x0,0x0,0x80040059,0x0,0x0,0x80060053,0x0)},  // OSYNC_MOD[31],VLOCKI2_POS[27:16],HLOCKI2_POS[11:0]
			{0x0018, DFSEL_Sens(0x0,0x80000050,0x80100059,0x00040059,0x00040059 ,0x00040059,0x0,0x0,0x00060057,0x0)},  // OSYNC_MOD[31],VLOCKI2_POS[27:16],HLOCKI2_POS[11:0] // 180403 LWH - LVDS Timing
			{0x0029, DFSEL_Sens(0x0,0xffff0000,0xffff0000,0xffff0000,0xffff0000,0xffff0000,0x0,0x0,0xffff0000,0x0)},  // SYNC_COD0[31:16],SYNC_COD1[15:0]
			{0x002a, DFSEL_Sens(0x0,0x00000002,0x00000002,0x00000000,0x00000000,0x00000000,0x0,0x0,0xc00     ,0x0)},  // SYNC_COD2[31:16],SYNC_COD3_0[15:0]
			{0x002b, DFSEL_Sens(0x0,0x00000003,0x00000003,0x08000ab0,0x08000ab0,0x08000ab0,0x0,0x0,0x8000900 ,0x0)},  // SYNC_COD3_1[31:16],SYNC_COD3_2[15:0]
			{0x002c, DFSEL_Sens(0x0,0x03000000,0x03000000,0x87e00000,0x87e00000,0x87e00000,0x0,0x0,0xa0200000,0x0)},  // LVDS Sync, *BLANK_SOL_ON[1],*SONY_V_ON[0]
			{0x0038, DFSEL_Sens(0x0,0x0       ,0x0       ,0x0       ,0x0       ,0x03100310,0x0,0x0,0x0       ,0x0)},  // 15.04.06 WHL - CMP_GROFS[25:16],CMP_GBOFS[9:0] 0 -> 0x3C403C4
			{0x0039, DFSEL_Sens(0x0,0x0       ,0x0       ,0x0       ,0x0       ,0x03100310,0x0,0x0,0x0       ,0x0)},  // 15.04.06 WHL - CMP_ROFS[25:16],CMP_BOFS[9:0]	  0 -> 0x3C403C4
			{0x0041, DFSEL_Sens(0x0,0x10780   ,0x10780   ,0x10780   ,0x10780   ,0x10780   ,0x0,0x0,0x10780   ,0x0)},  // Defect Area
			{0x0042, DFSEL_Sens(0x0,0x5043c   ,0x5043c   ,0x5043c   ,0x5043c   ,0x5043c   ,0x0,0x0,0x5043c   ,0x0)},  //		"
			{0x0051, DFSEL_Sens(0x0,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x0,0x0,0x3800d8  ,0x0)},  // WDR Default Set
			{0x0052, DFSEL_Sens(0x0,0x680300  ,0x680300  ,0x680300  ,0x680300  ,0x680300  ,0x0,0x0,0x680300  ,0x0)},  //		" 
			{0x00da, DFSEL_Sens(0x0,0x201111  ,0x201111  ,0x201111  ,0x201111  ,0x201111  ,0x0,0x0,0x201111  ,0x0)},  // Edge Sensing RGB Interpolate Default Set
//			{0x00e8, DFSEL_Sens(0x0,0x80808080,0x60606060,0x7045505a,0x60555042,0x7045505a/*0x60606060*/,0x0,0x0,0x70505560,0x0)},  // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
//			{0x00e9, DFSEL_Sens(0x0,0xef00e2  ,0xef00e2  ,0xf5f5e0d5,0xc0d0c0e0,0xf5f5e0d5/*0xef00e2*/  ,0x0,0x0,0xedf3c0eb,0x0)},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			{0x00f1, DFSEL_Sens(0x0,0x40003000,0x40003000,0x40003000,0x40003000,0x40003000,0x0,0x0,0x40003000,0x0)}   // ACE init
			};
	
	// Common Mode initial
	const UINT gnTblIspModeCommon[][IspModeSize] = {	// Mode 별 고정값 - 함부로 수정하지 마시오!!!
			//Addr   1080 25P    1080 30P    1080 50P    1080 60P
#if(ISP_CLK_MODE==1)			
			{0x0017, 0x0b3e0464, 0x095e0464, 0x0b3e0464, 0x095e0464, 0x0, 0x0, 0x0, 0x0},	// HTWO[31:16],VTWO[15:0]
#else
			{0x0017, 0x0a4e0464, 0x08960464, 0x0a4e0464, 0x08960464, 0x0, 0x0, 0x0, 0x0},	// HTWO[31:16],VTWO[15:0]
#endif
			{0x001a, 0x87880440, 0x87880440, 0x87880440, 0x87880440, 0x0, 0x0, 0x0, 0x0},	// HVWIO_SYN[31], HWO[27:16],  VWO[11:0] = 0x43f -> 0x440 for MD
			{0x00f0, 0x005500f0, 0x005500f0, 0x005500f0, 0x005500f0, 0x0, 0x0, 0x0, 0x0},	// DCBAR_ON[31],DCBAR_ST[27:16],DCBAR_HW[9:0]
			{0x0120, 0x041f0788, 0x041f0788, 0x041f0788, 0x041f0788, 0x0, 0x0, 0x0, 0x0} 	// MASK_CHAD[29:21],HALF_CHAD[20:12],FONT_HW[11:0]
			};

#else	// model_2M30p

	const UINT gnTblIspInitCommon[][IspInitSize] = {
			//  Sensor Number : 20  21         22  		  23         24         25         26  27  28  29
			//  Sensor Name   :     MN34227    OV2718     IMX122     IMX323     AR0237CSP                 
			{0x0012, DFSEL_Sens(0x0,0x3001020 ,0xfcc31010,0xfcc30000,0xfcc30800,0xfcc30000,0x0,0x0,0x0,0x0)},  // SLV[31:30],CH_DSEL[29],CH_HSSEL[28],CH_VSSEL[27],TRG_DIN[26],TRG_HSI[25],TRG_VSI[24],POL_HSI[23],POL_VSI[22],SHSO_INV[21],SVSO_INV[20],SHSO_TRG[19],SVSO_TRG[18],IDDR_ON[17],ISDR_OP[16],DDRFF_RS[15],SORD[14],CATP[13],ASYNC_ON[12],VSYN_NAON[11],PDI_MOD[10:9],SYNC_BYS[8],ICSEL[5:4],INSEL_MOD[3],INSEL[2:0]
			{0x0014, DFSEL_Sens(0x0,0x120001  ,0x00030000,0x00000000,0x801000b4,0x90000   ,0x0,0x0,0x0,0x0)},  // HLOCKI_POS[27:16],VLOCKI_POS[10:0]
			{0x0016, DFSEL_Sens(0x0,0x788043f ,0x784043d ,0x784043d ,0x784043d ,0x788043f ,0x0,0x0,0x0,0x0)},  // HWI[27:16],VWI[10:0]
			{0x0018, DFSEL_Sens(0x0,0x80050058,0x00070050,0x80000000,0x00070058,0x80000000,0x0,0x0,0x0,0x0)},  // OSYNC_MOD[31],VLOCKI2_POS[27:16],HLOCKI2_POS[11:0]
			{0x0029, DFSEL_Sens(0x0,0xffff0000,0xffff0000,0xffff0000,0xffff0000,0xffff0000,0x0,0x0,0x0,0x0)},  // SYNC_COD0[31:16],SYNC_COD1[15:0]
			{0x002a, DFSEL_Sens(0x0,0x00000002,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x0)},  // SYNC_COD2[31:16],SYNC_COD3_0[15:0]
			{0x002b, DFSEL_Sens(0x0,0x00000003,0x08000ab0,0x08000ab0,0x08000ab0,0x08000ab0,0x0,0x0,0x0,0x0)},  // SYNC_COD3_1[31:16],SYNC_COD3_2[15:0]
			{0x002c, DFSEL_Sens(0x0,0x03000000,0x03c00000,0x03c00000,0x03c00000,0x87e00000,0x0,0x0,0x0,0x0)},  // LVDS Sync, *BLANK_SOL_ON[1],*SONY_V_ON[0]
			{0x0038, DFSEL_Sens(0x0,0x0       ,0x0		 ,0x3c403c4 ,0x3c403c4 ,0x0	   	  ,0x0,0x0,0x0,0x0)},  // 15.04.06 WHL - CMP_GROFS[25:16],CMP_GBOFS[9:0] 0 -> 0x3C403C4
			{0x0039, DFSEL_Sens(0x0,0x0       ,0x0		 ,0x3c403c4 ,0x3c403c4 ,0x0	   	  ,0x0,0x0,0x0,0x0)},  // 15.04.06 WHL - CMP_ROFS[25:16],CMP_BOFS[9:0]	  0 -> 0x3C403C4
			{0x0041, DFSEL_Sens(0x0,0x10780   ,0x10780   ,0x10780   ,0x10780   ,0x10780   ,0x0,0x0,0x0,0x0)},  // Defect Area
			{0x0042, DFSEL_Sens(0x0,0x5043c   ,0x5043c   ,0x5043c   ,0x5043c   ,0x5043c   ,0x0,0x0,0x0,0x0)},  //		"
			{0x0051, DFSEL_Sens(0x0,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x3800d8  ,0x0,0x0,0x0,0x0)},  // WDR Default Set
			{0x0052, DFSEL_Sens(0x0,0x680300  ,0x680300  ,0x680300  ,0x680300  ,0x680300  ,0x0,0x0,0x0,0x0)},  //		" 
			{0x00da, DFSEL_Sens(0x0,0x201111  ,0x201111  ,0x201111  ,0x201111  ,0x201111  ,0x0,0x0,0x0,0x0)},  // Edge Sensing RGB Interpolate Default Set
//			{0x00e8, DFSEL_Sens(0x0,0x60606060,0x5a60404a/*0x35406050*/,0x60606060,0x70606050,0x60606060,0x0,0x0,0x0,0x0)},  // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
//			{0x00e9, DFSEL_Sens(0x0,0xef00e2  ,0xc0f000ca/*0x03cbccbd*/,0xef00e2  ,0xb0d2f8e8,0xef00e2	,0x0,0x0,0x0,0x0)},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			{0x00f1, DFSEL_Sens(0x0,0x40003000,0x40003000,0x40003000,0x40003000,0x40003000,0x0,0x0,0x0,0x0)}   // ACE init
			};

	// Common Mode initial
	const UINT gnTblIspModeCommon[][IspModeSize] = {	// Mode 별 고정값 - 함부로 수정하지 마시오!!!
			//Addr	 x1080 25P	 x1080 30P
#if(ISP_CLK_MODE==1)			
			{0x0017, 0x0b3e0464, 0x095e0464, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},	// HTWO[31:16],VTWO[15:0]
#else
			{0x0017, 0xa4e0464 , 0x8960464 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},	// HTWO[31:16],VTWO[15:0]
#endif
			{0x001a, 0x87880440, 0x87880440, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},	// HVWIO_SYN[31], HWO[27:16],  VWO[11:0] = 0x43f -> 0x440 for MD
			{0x00f0, 0x009600f0, 0x009600f0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},	// DCBAR_ON[31],DCBAR_ST[27:16],DCBAR_HW[9:0]
			{0x0120, 0x041f0788, 0x41f0788 , 0x0, 0x0, 0x0, 0x0, 0x0, 0x0} 	// MASK_CHAD[29:21],HALF_CHAD[20:12],FONT_HW[11:0]
			};
#endif

const UINT gTblIspInitCommonEa	= numberof(gnTblIspInitCommon);
const UINT gTblIspModeCommonEa	= numberof(gnTblIspModeCommon);


//----------------------------------------------------------------------------------------
// ISP Initial Table for each Sensor
//----------------------------------------------------------------------------------------
#if model_Sens == SENS_MN34210

	const UINT gnTblIspModeSensor[][IspModeSize] = {
			//Addr		720 25P 	  720 30P		  720 50P		  720 60P
			{0x0013,	0x7ba02ed	, 0x67002ed 	, 0x7ba02ed 	, 0x67002ed ,0x0,0x0,0x0,0x0	},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x140002	, 0x140002		, 0x140002		, 0x140002	,0x0,0x0,0x0,0x0	},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x4000c 	, 0x4000c		, 0x4000c		, 0x4000c	,0x0,0x0,0x0,0x0	},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x08000000	, 0x08000000	, 0x08000000	, 0x08000000,0x0,0x0,0x0,0x0	}	// OCSEL[31:30],YDLY[27:24]
			};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2		   STYLE #3			   STYLE #4			   STYLE #5			   STYLE #6
			{0x5048504a,	0x5048504a/*TBD*/, 0x5048504a/*TBD*/ , 0x5048504a/*TBD*/ , 0x5048504a/*TBD*/ , 0x5048504a/*TBD*/	}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0x10f700ef,	0x10f700ef/*TBD*/, 0x10f700ef/*TBD*/ , 0x10f700ef/*TBD*/ , 0x10f700ef/*TBD*/ , 0x10f700ef/*TBD*/	},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_IMX225
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		C720 25P	  C720 30P		  C720 50P		  C720 60P
			{0x0013,	0x7ba02ed	, 0x67002ed		, 0x7ba02ed		, 0x67002ed	,0x0,0x0,0x0,0x0	},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x040002	, 0x040002		, 0x040002		, 0x040002	,0x0,0x0,0x0,0x0	},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x570000	, 0x570000		, 0x570000		, 0x570000	,0x0,0x0,0x0,0x0	},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000	, 0x88000000	, 0x88000000,0x0,0x0,0x0,0x0	}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2		   	  STYLE #3 		      STYLE #4 	    	  STYLE #5 		      STYLE #6
			{0x60606060,	0x60606060/*TBD*/	, 0x60606060/*TBD*/ , 0x60606060/*TBD*/ , 0x60606060/*TBD*/	, 0x60606060/*TBD*/}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xef00e2  ,	0xef00e2/*TBD*/		, 0xef00e2/*TBD*/	, 0xef00e2/*TBD*/	, 0xef00e2/*TBD*/	, 0xef00e2/*TBD*/  },  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_MN34229
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		1080 25P	  1080 30P	  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464	, 0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x000000a	, 0x000000a	, 0x000000a	, 0x000000a		, 0			, 0			, 0			, 0			},	// 15.03.31 WHL - HSPI[27:16],VSPI[10:0]
			{0x0019,	0x00000013	, 0x00000013, 0x00000013, 0x00000013	, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000, 0x88000000, 0x88000000	, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2			  STYLE #3			  STYLE #4			  STYLE #5			  STYLE #6
			{0x80808080,	0x80808080/*TBD*/	, 0x80808080/*TBD*/ , 0x80808080/*TBD*/ , 0x80808080/*TBD*/	, 0x80808080/*TBD*/	}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0x00ef00e2,	0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_MN34420
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		1080 25P	  1080 30P	  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464	, 0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x0000018	, 0x0000018	, 0x0000018	, 0x0000018		, 0			, 0			, 0			, 0			},	// 15.03.31 WHL - HSPI[27:16],VSPI[10:0]
			{0x0019,	0x0000000d	, 0x0000000d, 0x0000000d, 0x0000000d	, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000, 0x88000000, 0x88000000	, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2			  STYLE #3			  STYLE #4			  STYLE #5			  STYLE #6
			{0x60606060,	0x60606060/*TBD*/	, 0x60606060/*TBD*/ , 0x60606060/*TBD*/ , 0x60606060/*TBD*/ , 0x60606060/*TBD*/	}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0x00ef00e2,	0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_IMX291
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		1080 25P	  1080 30P	  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464	, 0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x0000001	, 0x0000001	, 0x0000001	, 0x0000001		, 0			, 0			, 0			, 0			},	// 15.03.31 WHL - HSPI[27:16],VSPI[10:0]
			{0x0019,	0x00000002	, 0x00000002, 0x00000002, 0x00000002	, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000, 0x88000000, 0x88000000	, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2					STYLE #3		STYLE #4		STYLE #5		STYLE #6
			{0x4d445858,	0x60606060/*0x624a604c*/,	0x4d445858,		0x4d445858,		0x4d445858,		0x4d445858}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0x0bf80ded,	0x00be00dc/*0xf3eae0f3*/,	0x0bf80ded,		0x0bf80ded,		0x0bf80ded,		0x0bf80ded}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]			
			};

#elif model_Sens == SENS_IMX327
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		1080 25P	  1080 30P	  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464	, 0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x0000001	, 0x0000001	, 0x0000001	, 0x0000001		, 0			, 0			, 0			, 0			},	// 15.03.31 WHL - HSPI[27:16],VSPI[10:0]
			{0x0019,	0x00000002	, 0x00000002, 0x00000002, 0x00000002	, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000, 0x88000000, 0x88000000	, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2		STYLE #3		STYLE #4		STYLE #5		STYLE #6
			{0x623c3e46,	0x623c3e46,		0x623c3e46,		0x623c3e46,		0x623c3e46, 	0x623c3e46}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xcfdb19e7,	0xcfdb19e7,		0xcfdb19e7,		0xcfdb19e7,		0xcfdb19e7, 	0xcfdb19e7}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_IMX307
	
	const UINT gnTblIspModeSensor[][IspModeSize] = { // 20150618 - WHL
			//Addr		1080 25P	  1080 30P	  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464	, 0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x0000001	, 0x0000001	, 0x0000001	, 0x0000001		, 0			, 0			, 0			, 0			},	// 15.03.31 WHL - HSPI[27:16],VSPI[10:0]
			{0x0019,	0x00000002	, 0x00000002, 0x00000002, 0x00000002	, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000, 0x88000000, 0x88000000	, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2			  STYLE #3			  STYLE #4			  STYLE #5			  STYLE #6
			{0x95a09088,	0x95a09088/*TBD*/	, 0x95a09088/*TBD*/ , 0x95a09088/*TBD*/ , 0x95a09088/*TBD*/	, 0x95a09088/*TBD*/}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xc09fffd0,	0xc09fffd0/*TBD*/	, 0xc09fffd0/*TBD*/	, 0xc09fffd0/*TBD*/	, 0xc09fffd0/*TBD*/	, 0xc09fffd0/*TBD*/},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_AR0237CSH

	const UINT gnTblIspModeSensor[][IspModeSize] = {
			//Addr		1080 25P	  1080 30P		  1080 50P	  1080 60P
			{0x0013,	0xa4e0464	, 0x8960464		, 0xa4e0464	, 0x8960464		, 0xa4e0464		, 0x8960464		, 0xa4e0464		, 0x8960464	},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x00000		, 0x00000		, 0x00000	, 0x00000		, 0x1cd0093		, 0x1cd0093		, 0x1cd0093		, 0x1cd0093	},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x000000	, 0x000000		, 0x000000	, 0x000000		, 0x000000		, 0x000000		, 0x000000		, 0x000000	},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x08000000	, 0x08000000	, 0x08000000, 0x08000000	, 0x48000000	, 0x48000000	, 0x48000000	, 0x48000000}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE A		STYLE B			STYLE C			STYLE D			STYLE E			STYLE F
			{0x56435948,	0x56435948,		0x56435948, 	0x56435948, 	0x56435948, 	0x56435948}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xd6e808eb,	0xd6e808eb,		0xd6e808eb, 	0xd6e808eb, 	0xd6e808eb, 	0xd6e808eb}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			// SL EN673-AR0237 28x28 (Test용 Fixed Lens)
			//{0x64347264,	0x40444a44,		0x45444a44, 	0x4a444a44, 	0x45484f46, 	0x45435948}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			//{0xcfea7cd6,	0xe2e014ee,		0xe2e014ee, 	0xe2e014ee, 	0xe2e8fcf1, 	0xdee808eb}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			// SL EN673-AR0237 38x38
			//{0x5a50644d,	0x40444a44,		0x45444a44, 	0x4a444a44, 	0x45484f46, 	0x45435948}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			//{0xd8e63ce8,	0xe2e014ee,		0xe2e014ee, 	0xe2e014ee, 	0xe2e8fcf1, 	0xdee808eb}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_MN34227

	const UINT gnTblIspModeSensor[][IspModeSize] = {
			//Addr		1080 25P	  1080 30P
			{0x0013,	0xa4e0464	, 0x8960464		,0x0,0x0,0x0,0x0,0x0,0x0	},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x0			, 0x0			,0x0,0x0,0x0,0x0,0x0,0x0	},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x4    	    , 0x4    	    ,0x0,0x0,0x0,0x0,0x0,0x0	},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0x88000000	, 0x88000000	,0x0,0x0,0x0,0x0,0x0,0x0	}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2			  STYLE #3			  STYLE #4			  STYLE #5			  STYLE #6
			{0x60606060,	0x60606060/*TBD*/	, 0x60606060/*TBD*/ , 0x60606060/*TBD*/ , 0x60606060/*TBD*/	, 0x60606060/*TBD*/}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0x00ef00e2,	0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/	, 0x00ef00e2/*TBD*/},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_IMX323

	const UINT gnTblIspModeSensor[][IspModeSize] = {
			//Addr		1080 25P	  1080 30P
			{0x0013,	0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x000004	, 0x000004		, 0			, 0			, 0			, 0			, 0			, 0			},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x000004	, 0x000004		, 0			, 0			, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0xc8000000	, 0xc8000000	, 0			, 0			, 0			, 0			, 0			, 0			}	// OCSEL[31:30],OCFSEL_INV[28]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1		STYLE #2		STYLE #3		STYLE #4		STYLE #5		STYLE #6
			{0x70606050,	0x70606050,		0x70606050,		0x70606050,		0x70606050,		0x70606050}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xb0d2f8e8,	0xb0d2f8e8,		0xb0d2f8e8,		0xb0d2f8e8,		0xb0d2f8e8,		0xb0d2f8e8}, // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#elif model_Sens == SENS_OV2718

	const UINT gnTblIspModeSensor[][IspModeSize] = {
			//Addr		1080 25P	  1080 30P
			{0x0013,	0xa4e0464	, 0x8960464		, 0			, 0			, 0			, 0			, 0			, 0			},	// HTWI[31:16],VTWI[15:0]
			{0x0015,	0x00000E	, 0x00000E		, 0			, 0			, 0			, 0			, 0			, 0			},	// HSPI[27:16],VSPI[10:0]
			{0x0019,	0x00000E	, 0x00000E		, 0			, 0			, 0			, 0			, 0			, 0			},	// HSPO[27:16],VSPO[11:0]
			{0x001e,	0xc8000000	, 0xc8000000	, 0			, 0			, 0			, 0			, 0			, 0			}	// OCSEL[31:30],YDLY[27:24]
        	};

	const UINT gnTblIspModeAwbCor[][6] = {
			// STYLE #1 	STYLE #2		  STYLE #3			  STYLE #4	   		  STYLE #5	   		  STYLE #6
			{0x5a60404a,	0x5a60404a		, 0x5a60404a	 	, 0x5a60404a    , 0x5a60404a	,0x5a60404a}, // BYGAINN[31:24],BYGAINP[23:16],RYGAINN[15:8],RYGAINP[7:0]
			{0xc0f000ca,	0xc0f000ca		, 0xc0f000ca		, 0xc0f000ca	, 0xc0f000ca	,0xc0f000ca},  // BYHUEN[31:24],BYHUEP[23:16],RYHUEN[15:8],RYHUEP[7:0]
			};

#endif

const UINT gTblIspModeSensorEa	= numberof(gnTblIspModeSensor);


/* EOF */
