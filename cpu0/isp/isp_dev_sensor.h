/* **************************************************************************
 File Name	: 	isp_dev_sensor.h
 Description:	EN673 - miscellaneous define
 Designer	:	Kim, Sunghoon
 Date		:	14. 6. 18
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

//-------------------------------------------------------------------------------------------------
// System definition
#define		model_Pana		((model_Sens==SENS_MN34210)||(model_Sens==SENS_MN34229)||(model_Sens==SENS_MN34227)||(model_Sens==SENS_MN34420))
#define		model_Sony		((model_Sens==SENS_IMX225)||(model_Sens==SENS_IMX291)||(model_Sens==SENS_IMX327)||(model_Sens==SENS_IMX307)||(model_Sens==SENS_IMX323))
#define		model_Aptn		(model_Sens==SENS_AR0237CSH)
#define		model_Omni		(model_Sens==SENS_OV2718)

#define 	model_SensIntf	(((model_Sens==SENS_IMX323)||(model_Sens==SENS_OV2718)/*||(model_Sens==SENS_IMX122)*/) ? 1 : 0) /* 0 : LVDS, 1 : Parallel*/

//-------------------------------------------------------------------------------------------------
// Default LVDS timing (Data,Clock Delay & LVDS setup, SLV[31:30],CH_DSEL[29],CH_HSSEL[28],CH_VSSEL[27],TRG_DIN[26],TRG_HSI[25],TRG_VSI[24],POL_HSI[23],POL_VSI[22],SHSO_INV[21],SVSO_INV[20],SHSO_TRG[19],SVSO_TRG[18],IDDR_ON[17],ISDR_OP[16],DDRFF_RS[15],SORD[14],CATP[13],ASYNC_ON[12],VSYN_NAON[11],PDI_MOD[10:9],SYN_BYS[8],ICSEL[5:4],INSEL_MOD[3],INSEL[2:0])
#if model_Sens == SENS_AR0237CSH	// EVM,		Custom Board,
	#if ISP_CLK_MODE==1 // 180403 LWH - LVDS Timing
		#define ISP_REG_028_0   DLY_VAL(0xaee45b6a/*0xaee42491*/, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_25_30/Flip Off
		#define ISP_REG_028_1   DLY_VAL(0xaee45b6a/*0xaee42491*/, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_25_30/Flip On
		#define ISP_REG_028_2   DLY_VAL(0xaee45b6a/*0xaee42491*/, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_50_60/Flip Off
		#define ISP_REG_028_3   DLY_VAL(0xaee45b6a/*0xaee42491*/, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_50_60/Flip On
	#else
		#define ISP_REG_028_0   DLY_VAL(0xaee45b6a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_25_30/Flip Off
		#define ISP_REG_028_1   DLY_VAL(0xaee45b6a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_25_30/Flip On
		#define ISP_REG_028_2   DLY_VAL(0xaee45b6a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_50_60/Flip Off
		#define ISP_REG_028_3   DLY_VAL(0xaee45b6a, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000)  // 1080_50_60/Flip On
	#endif
#elif model_Sens == SENS_IMX291
	#if	ISP_CLK_MODE==1
		#define ISP_REG_028_0	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#else
		#define ISP_REG_028_0	DLY_VAL(0x8ee44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ee44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ee44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ee44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#endif
#elif model_Sens == SENS_IMX327
	#if	ISP_CLK_MODE==1
		#define ISP_REG_028_0	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#else
		#define ISP_REG_028_0	DLY_VAL(0x8ee44926, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ee44926, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ce40004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ce40004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#endif
#elif model_Sens == SENS_IMX307
	#if	ISP_CLK_MODE==1
		#define ISP_REG_028_0	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ae44922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#else
		#define ISP_REG_028_0	DLY_VAL(0x8ee44926, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
		#define ISP_REG_028_1	DLY_VAL(0x8ee44926, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
		#define ISP_REG_028_2	DLY_VAL(0x8ce40004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
		#define ISP_REG_028_3	DLY_VAL(0x8ce40004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On
	#endif
#elif model_Sens == SENS_IMX225
	#define ISP_REG_028_0	DLY_VAL(0xace40005, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xace40005, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xace40005, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xace40005, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#elif model_Sens == SENS_MN34210
	#define ISP_REG_028_0	DLY_VAL(0xaed8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xaed8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xaed8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xaed8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#elif model_Sens == SENS_MN34227
	#define ISP_REG_028_0	DLY_VAL(0xaed80003, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xaed80003, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xaed80003, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xaed80003, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#elif model_Sens == SENS_MN34229
	#define ISP_REG_028_0	DLY_VAL(0xaed84922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xaed84922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xaed84922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xaed84922, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#elif model_Sens == SENS_MN34420
	#define ISP_REG_028_0	DLY_VAL(0xaed84923, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xaed84923, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xaed84923, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xaed84923, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#else
	#define ISP_REG_028_0	DLY_VAL(0xacd8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip Off
	#define ISP_REG_028_1	DLY_VAL(0xacd8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_25_30/Flip On
	#define ISP_REG_028_2	DLY_VAL(0xacd8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip Off
	#define ISP_REG_028_3	DLY_VAL(0xacd8024f, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000) // 1080_50_60/Flip On

#endif


// -----------------------------------------------------------------------------------------------
// Clock & Frame info (reserved)
#if model_1M
//	#define MCLK		BUS_CLK			// AHB clock (core) // 180323 KDH move to dev_isp_common.h
	#define	FR			60              // Frame pre sec
	#define FR_DUTY		16				// Frame duty time (msec)
	#define FR_HW		1280            // Active H width
	#define FR_VW		720             // Active V width
	#define FR_HW_720	1280			// Active H width 720p Crop
	#define FR_VW_720	720				// Active V width 720p Crop


	#if(ISP_CLK_MODE==1)

		#if (model_Sens == SENS_IMX225)
			#define FR_HTW50	(2160<<1)	// Total  H width
			#define FR_HTW60	(1800<<1) 	// Total  H width
		#else
			#define FR_HTW50	2160 		// Total  H width
			#define FR_HTW60	1800 		// Total  H width
		#endif

	#else

		#if (model_Sens == SENS_IMX225)
			#define FR_HTW50	(1980<<1)	// Total  H width
			#define FR_HTW60	(1650<<1)	// Total  H width
		#else
			#define FR_HTW50	1980 		// Total  H width
			#define FR_HTW60	1650 		// Total  H width
		#endif

	#endif
	
	#define FR_VTW50	750					// Total  V width
	#define FR_VTW60	750					// Total  V width

#elif (model_2M|model_2M30p)	// 2015911 - WHL

	
//	#define MCLK		BUS_CLK			// AHB clock (core) // 180323 KDH move to dev_isp_common.h
	#define FR			30				// Frame pre sec
	#define FR_DUTY		33				// Frame duty time (msec)
	#define FR_HW		1920			// Active H width
	#define FR_VW		1080			// Active V width
	#define FR_HW_720	1280			// Active H width 720p Crop
	#define FR_VW_720	720				// Active V width 720p Crop


	#if(ISP_CLK_MODE==1)

		#if (model_Sens == SENS_IMX323)
			#define FR_HTW50	(2880>>1)	// Total  H width
			#define FR_HTW60	(2400>>1)	// Total  H width
		#else
			#define FR_HTW50	2880		// Total  H width
			#define FR_HTW60	2400		// Total  H width
		#endif

	#else

		#if (model_Sens == SENS_IMX323)
			#define FR_HTW50	(2640>>1)	// Total  H width
			#define FR_HTW60	(2200>>1)	// Total  H width
		#else
			#define FR_HTW50	2640		// Total  H width
			#define FR_HTW60	2200		// Total  H width
		#endif

	#endif

	#define FR_VTW50	1125		// Total  V width
	#define FR_VTW60	1125		// Total  V width

#endif



//--------------------------------------------------------------------------------------------------
// Device driver(Lib)

// for SPI
#define SENS_SONY_ID2		0x2
#define SENS_SONY_ID3		0x3
#define SENS_SONY_ID4		0x4
#define SENS_SONY_ID5		0x5
#define SENS_SONY_ID6		0x6

// for TWI
#define SENS_SONY_ID2_TWI	0x30
#define SENS_SONY_ID3_TWI	0x31
#define SENS_SONY_ID4_TWI	0x32
#define SENS_SONY_ID5_TWI	0x33
#define SENS_SONY_ID6_TWI	0x34

#define SENS_SONY_DEV	 	0x34 	//161019 LWH
#define SENS_AR0230_DEV 	0x20 	//150424 LGH
#define SENS_OV2718_DEV	 	0x6C 	//170724 LWH

#if model_Pana
	extern void SetSensFnc(WORD dummy, WORD ADDR, WORD VAL);
	extern WORD GetSens(WORD ADDR);

#elif model_Sony
	extern void SetSensBurstFnc(BYTE ID, BYTE ADDR, BYTE* VAL, BYTE Length);

	extern void SetSensFnc(WORD ID, WORD ADDR, WORD VAL);
	extern BYTE GetSens(BYTE ID, BYTE ADDR);

#elif model_Aptn
	extern void SetSensFnc(BYTE DEV, WORD ADDR, WORD VAL);
	extern WORD GetSens(WORD ADDR);

#elif model_Omni
	extern void SetSensFnc(BYTE DEV, WORD ADDR, BYTE VAL);
	extern BYTE GetSens(WORD ADDR);

#else

#endif

#define SetSensBurst	SetSensBurstFnc
#define SetSens			SetSensFnc

#if   model_Sens == SENS_MN34227 || model_Sens == SENS_MN34229 || model_Sens == SENS_MN34420
	#define SPI0_CLKDIV_SENS	SPI_CLKDIV12	// MAX = SPI_CLKDIV4
#elif model_Sens == SENS_IMX291 || model_Sens == SENS_IMX327 || model_Sens == SENS_IMX307	// 2016415 - WHL
	#define SPI0_CLKDIV_SENS	SPI_CLKDIV12	// MAX = SPI_CLKDIV8
#else
	#define SPI0_CLKDIV_SENS	SPI_CLKDIV16	// MAX = SPI_CLKDIV64
#endif

#define SPI0_START_SENS		SPI_START_HIGH	// MN34227�� ��� HIGH�� �����ؾ� ��

#define		IspInitSize	2/*9*/
#define		IspModeSize	9


//--------------------------------------------------------------------------------------------------
// Lib Link

// System
#define InitIsp				Sens(InitIsp)
#define OutModeRun			Sens(OutModeRun)
#define InitSensRun			Sens(InitSensRun)

// AE
#define gwAeLssLog60Tbl		Sens(gwAeLssLog60Tbl)
#define gwAeLssLog50Tbl		gwAeLssLog60Tbl
#define LibAeShtLine		Sens(LibAeShtLine)
#define AeSHT				Sens(AeSHT)
#define AeAGC				Sens(AeAGC)

#define gbSensShtDly		Sens(gbSensShtDly)
#define gbSensAgcDly		Sens(gbSensAgcDly)
#define gbVHssAgcTbl		Sens(gbVHssAgcTbl)
#define gVHssAgcTblEa		Sens(gVHssAgcTblEa)
#define AeSHT0				Sens(AeSHT0)
#define AeAGC0				Sens(AeAGC0)
#define AGC2POS				Sens(AGC2POS)
#define POS2AGC				Sens(POS2AGC)

#define AE_SHTw			Sens(AE_SHTw)
#define AE_DSSw			Sens(AE_DSSw)
#define AE_AGCw			Sens(AE_AGCw)
#define AE_DGCw			Sens(AE_DGCw)
#define AE_FREQw		Sens(AE_FREQw)
#define AE_ETCw			Sens(AE_ETCw)

#define AE_GAIN_TOTAL		Sens(AE_GAIN_TOTAL)
#define AE_GAIN_TOTAL2		Sens(AE_GAIN_TOTAL2)


// AWB
#define glMtxCf				Sens(glMtxCf)
