/* **************************************************************************
 File Name	: 	app_init.c
 Description:	initialize functions
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "isp_app_init.h"
#include "dev_inline.h"	// to use fast code

//-------------------------------------------------------------------------------------------------
//
void SensorIn(const BYTE abOn)	// 180704 KSH - Sensor 설정이 완료되기 전에 잘못된 영상이 입력되는 경우 WDR또는 DNR 메모리의 오동작 방지
{
#if (model_SensIntf==1)
	#define SENSOR_IN_ON	BT_DIN_SELw(0);
	#define SENSOR_IN_OFF	BT_DIN_SELw(1);
#else
	#define SENSOR_IN_ON	{SDES_PDw(0x00); LVDS_RXONw(0x01);}
	#define SENSOR_IN_OFF	{SDES_PDw(0x1f); LVDS_RXONw(0x00);}	// ISP REG. H/W Default 설정
#endif

	if(abOn) { SENSOR_IN_ON  }
	else     { SENSOR_IN_OFF }
}

void InitFontChar(const UINT anFont[][12])
{
	FONT_MUTEw(1); // Font Mute at the character over write

	UINT i, j;

#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
	ISPCLK_SEL(PPCK,1); // for ABT
	ISPCLK_SEL(FN_CK,1);
	
#endif

	for(j=0; j<ISP_FONT_CHAR_EA; j++){
		for(i=0; i<12; i++){
			SetFontChar( (j*12)+i, ((UINT)(anFont[j][i])));
		}
	}
	FONT_MUTEw(0);
	
#if (IMAGE_INPUT_MODE==2)
	ISPCLK_SEL(FN_CK,5);
	
#elif (IMAGE_INPUT_MODE==3)
	ISPCLK_SEL(FN_CK,2);
	
#endif
}

void Isp_SensorRst(void)
{
	WaitXus(1000);	// 180403 LWH - LVDS Timing 
	GpioSetLo(gbSensRstPort);
	WaitXus(1000);
	GpioSetHi(gbSensRstPort);
	WaitXus(1000);
}

void InitSensCstm(void)
{
	
}

void InitSensCheck(void)
{
	UINT nSensData = 0;

	nSensData = (GetSens(0x31,0xdc)&BitMask_03)>>1; // 307:0b10, 327:0b11, 291:0b00

	if( (model_Sens==13 && nSensData!=2) || (model_Sens==14 && nSensData!=3) || (model_Sens==15 && nSensData!=0)) {
		printf("***Please check the sensor settings. ");
//		DispStr("Please check the sensor",17,22,23);
		DispStr("Please check the sensor",2,0,23);

		switch(model_Sens) {
			case 13:
				if(nSensData==0) { DispStr("HW: IMX291 / FW: IMX307",3,0,23); printf("HW: IMX291 / FW: IMX307\r\n");}
				if(nSensData==3) { DispStr("HW: IMX327 / FW: IMX307",3,0,23); printf("HW: IMX327 / FW: IMX307\r\n");}
				break;
			case 14:
				if(nSensData==0) { DispStr("HW: IMX291 / FW: IMX327",3,0,23); printf("HW: IMX291 / FW: IMX327\r\n");}
				if(nSensData==2) { DispStr("HW: IMX307 / FW: IMX327",3,0,23); printf("HW: IMX307 / FW: IMX327\r\n");}
				break;
			case 15:
				if(nSensData==2) { DispStr("HW: IMX307 / FW: IMX291",3,0,23); printf("HW: IMX307 / FW: IMX291\r\n");}
				if(nSensData==3) { DispStr("HW: IMX327 / FW: IMX291",3,0,23); printf("HW: IMX327 / FW: IMX291\r\n");}
				break;
		}
	}
}


void InitSens(void)
{	
#if (model_Test!=1)	&& (model_NoSensor!=1)	// if not test 1

	gbSensCtrMode = SENS_CONTROL_MODE;
	
	Isp_SensorRst();

	InitSensRun();

	#if (model_Sens == SENS_OV2718)
		SetSensTwi_Omni(0x6c, 0x315a, 0x01); // 170807 LWH - Gain Def (0??->2~x63
		SetSensTwi_Omni(0x6c, 0x30b6, 0x03); // 170807 LWH - Shutter Def
//		SetSensTwi_Omni(0x6c, 0x3253, 0x83); // 170807 LWH - Test Pattern Added
	#endif

#else	// system test w/o sensor
	TestPatternOn();

#endif
}

void InitIspClk(void)
{	
	ISPCLK_SEL(FPCK,1);		// ISP Pre pixel clock
	ISPCLK_SEL(BT_PCK,1);	// ISP Pre Sync Gen Block clock (Main Parallel/BT1120)
	ISPCLK_SEL(PPCK,1);		// ISP Post pixel clock
	ISPCLK_SEL(PSCK,1);		// ISP Post Sync Gen Block clock
	ISPCLK_SEL(FN_CK,1);	// Font clock
	ISPCLK_SEL(PR_CK,1);	// Box/Privacy clock
	ISPCLK_SEL(IMD_CK,1);	// IMD clock
	ISPCLK_SEL(DCKO0,1);	// Digital output clock for ch0 (FPGA Only)
}

void InitIspCstm(void)
{
	TxStrNoIRQ(" ISP Cstm Init...", 0, 0);
	
	// ISP Write Page Address
	ISP_YADR_P0w(0x680000); // 0xB400000
	ISP_CADR_P0w(0x6C0000); // 0xB600000
	
	ISP_YADR_P1w(0x700000); // 0xB800000
	ISP_CADR_P1w(0x740000); // 0xBA00000
	
	ISP_YADR_P2w(0x780000); // 0xBC00000
	ISP_CADR_P2w(0x7C0000); // 0xBE00000

	// Down-Scaler Default LPF Set
	DS0_HLPFw(1); // 0 : No LPF, 1 : 3 Tab, 2 : 5 Tab
	DS0_VLPFw(1);
	
	DS1_HLPFw(2);
	DS1_VLPFw(2);
	
#if (model_Sens == SENS_IMX291) || (model_Sens == SENS_IMX327) || (model_Sens == SENS_IMX307)	// 20160119 - WHL

	MERGE_HWw(0x7a0);	// 1920 + Margin for Line Buffer H-Width
		
	CH_HSSELw(0);
	CH_VSSELw(0);
	POL_HSIw(1);
	POL_VSIw(0);
	TRG_HSIw(0);			//	20161020 Add
	TRG_VSIw(0);			//	20161020 Add
	
#elif (model_Sens == SENS_AR0237CSH)
	MERGE_HWw(0x7a0);	// 1920 + Margin for Line Buffer H-Width
		
	BLANK_SOL_ONw(1);
	LV_RPOSw(1);		// 161012 - MJS (Line memory read position control)

#elif (model_Sens == SENS_MN34227)

		
#elif (model_Sens == SENS_MN34210)	// 2015915 - WHL : FPGA Test for MN34210

	ISPCLK_SEL(PCKO0,4);	// 27MHz for Sensor Input Clock

#elif (model_Sens == SENS_IMX323)	// 2015922 - WHL : Only for Palallel I/F


#ifdef OPTION_FLAG_2
	ISPCLK_SEL(PCKO0,3);
	VLOCKI2_POSw(0x7);
	HLOCKI2_POSw(0x58);

	ISPCLK_SEL(IBT_PCK,0);
	PCLK_ONw(1);	// External Clk Input Enable ( 0 : Blocking External Clk )

	SLVw(3);
	CH_DSELw(1);
	CH_HSSELw(1);
	CH_VSSELw(1);

	IDDR_ONw(1);
	ISDR_OPw(1);
	PDI_MODw(1);

	POL_HSIw(1);
	POL_VSIw(1);

	VSYN_NAONw(1);			// 2016414 - WHL : When the external H/V sync input is not alignment, set to '1'. (parallel mode only)
	PARALLEL_SENSORw(1);	// 2016414 - WHL : Sub Sensor Header Board Enable
#else
	ISPCLK_SEL(PCKO0,3);

	ISPCLK_SEL(IBT_PCK,0);
	PCLK_ONw(1);	// External Clk Input Enable ( 0 : Blocking External Clk )

	PARALLEL_SENSORw(1);	// 2016414 - WHL : Sub Sensor Header Board Enable
#endif

#elif (model_Sens == SENS_OV2718)

	//ASYNC_VSTw(0x400);			// OV2718 Parallel 27MHz 동작 시 Sensor 출력의 HLOCK이 흔들림 -> VLOCK이 흔들림 -> DNR 시 간헐적으로 영상이 깨지는 경우 발생 -> 이를 개선하기 위해 조절(임시 개선 방법) -> 개선은 되었으나 매우 가끔 현상 발생 -> DSS 시 영상이 위로 조금 이동하는 문제 발생하여 주석처리
	//ASYNC_HWw(0x66/*0x69*/);	//    "

// Main sensor operation clock (SS_CKO pin)
// 0:ISP_PLL clock, 1:ISP PLL(PLL1)/2, 2:ISP PLL(PLL1)/4, 3:ISP PLL(PLL1)/8, 4:CLK input clock(normal 27MHz)
//	ISPCLK_SEL(PCKO0,4);
	ISPCLK_SEL(PCKO0,3);

	ISPCLK_SEL(IBT_PCK,0);
	PCLK_ONw(1);	// External Clk Input Enable ( 0 : Blocking External Clk )

	DDR_RWDR_LTCw(0x50);

#elif (model_Sens == SENS_IMX225)	// 2015922 - WHL : Only for Palallel I/F

	#if (Sens225_IF_Mode==0)	// for Parallel Mode
		ISPCLK_SEL(PCKO0,4);
		ISPCLK_SEL(FPCK,1);
		ISPCLK_SEL(BT_PCK,1);
		
		ISPCLK_SEL(IBT_PCK,0);
		PCLK_ONw(1);	// External Clk Input Enable ( 0 : Blocking External Clk )
		
		SLVw(3);
		CH_DSELw(1);
		CH_HSSELw(1);
		CH_VSSELw(1);
		
		IDDR_ONw(1);
		ISDR_OPw(1);

		TRG_DINw(1);
		POL_HSIw(1);
		POL_VSIw(1);
		
		VSYN_NAONw(1);			// 2016414 - WHL : When the external H/V sync input is not alignment, set to '1'. (parallel mode only)
		PARALLEL_SENSORw(1);	// 2016414 - WHL : Sub Sensor Header Board Enable
		
		SetIsp(0x1c, 0x82000000);	// HSYNC_OFS_ON[31],HSYNC_OFS_VFLG_SEL[30],HSYNC_OFS_INV[29],HSYNC_OFS[26:24] for Sync Timing Align
	
	#elif (Sens225_IF_Mode==2)	// for LVDS Mode
	
		ISPCLK_SEL(PCKO0,3);
		
		SLVw(3);
		CH_DSELw(0);
		CH_HSSELw(0);
		CH_VSSELw(0);
		
//			MERGE_HWw(0x500);	// 1280
		MERGE_HWw(0x580);	// 1280 + Margin for Line Buffer H-Width
		
//			SetIsp(0x1c, 0x82000000);	// HSYNC_OFS_ON[31],HSYNC_OFS_VFLG_SEL[30],HSYNC_OFS_INV[29],HSYNC_OFS[26:24] for Sync Timing Align

	#endif
	
#endif
}

void InitCPU(void)
{	
	TxStrNoIRQ(" CPU Init...", 0, 0);
		
}

void Check_I2cDevice(void)
{
	UINT i;

	TxStrNoIRQ(" Check_I2cDevice...", 0, 0);

	if(I2C0_ACT) {
		TxStrNoIRQ("  >>Another master is working...", 0, 0);
		while(I2C0_ACT);
	}

#if model_Save == 0
	for(i=0; i<10; i++) {
		if(i2c_write(EEPROM_DEVICE,1,0) == 0) {
			gbEepromOff = 0;
			TxStrNoIRQ("  >>EEPROM Connected...", 0, 0);
			break;
		}
		WaitUs(100);
	}
	if(gbEepromOff) TxStrNoIRQ("  >>EEPROM Not Connected...", 0, 0);
#endif

#if USE_DIGITAL_INPUT == 1

	UINT j;

	for(i=1; i; i++) {
		if(i2c_write(EN332_DEVICE,1,0) == 0) {
			gbEn331Off = 0;
			TxStrNoIRQ("  >>EN332T Connected...", 0, 0);
			for(j=0; j<10; j++) {
				GpioSetHi(GPIO_ISP_VI_LED);
				WaitXms(10);
				GpioSetLo(GPIO_ISP_VI_LED);
				WaitXms(10);
			}
			break;
		}
		WaitXms(100);
	}
	if(gbEn331Off) TxStrNoIRQ("  >>EN332T Not Connected...", 0, 0);
#endif

	UNUSED(i);
}

//*************************************************************************************************
// Init Top
//-------------------------------------------------------------------------------------------------
void InitAfODM(void)
{ //	ISP AF ODM Environment Setting
	AF_YSELw(3);
	AF_YLPFw(0);
	
	SetIsp(0x1CA, 0x00200398);
	SetIsp(0x1CB, 0x00090432);
	SetIsp(0x1CE, 0x03C00000);
	SetIsp(0x1CF, 0x06870000);
}

void isp_reset(void)
{
#if 1
	#define ISP_REG_SIZ		0x1d0
	UINT gnTblIsp[ISP_REG_SIZ];

	int i;
	for(i=0; i<ISP_REG_SIZ; i++) gnTblIsp[i] = GetIsp(i);

	TxStrNoIRQ(" isp_reset:", gnTblIsp[ISP_REG_SIZ-1], 8);
	//TxStrNoIRQ(" UC:", UART0_CLKDIV, 8);

	SYS_ISP_RST	= 0;
	SYS_ISP_RST	= 1;

	for(i=0; i<5;           i++) SetIsp(i, gnTblIsp[i]);
	for(i=7; i<ISP_REG_SIZ; i++) SetIsp(i, gnTblIsp[i]);
	SetIsp(5, gnTblIsp[5]);
	SetIsp(6, gnTblIsp[6]);

	gnClkChgCnt = /*6*/ParAe(PA_H264_SKIP_CNT);	// NOR+DSS -> WDR 시 최소 5frame Skip(H.264&JPEG) 필요

	//TxStrNoIRQ(" UC:", UART0_CLKDIV, 8);
#else
	SYS_ISP_RST	= 0;
	SYS_ISP_RST	= 1;

	Cstm_OutMode(2);	// 161031 WHL : Default 2M30P

	InitIsp();					// for ISPx

	InitAfODM();
	
	FORCE_ABT_SOFFw(1);    // ABT Set after ISP Init
	
	HLMASK_ONw(1); HLMASK_THw(0);	// Block Masking // 2016331 - WHL
	
	InitIspClk();				// for ISPx
	
	InitIspCstm();

	//InitSens();

	//InitSensCstm();

	InitPar();

	//InitMenu();
	//UartBaudRate();
	InitFontOSD();

	//InitAe();
	AE_LOCK_SELw(1);
	AE_WIN_SELw(7);

	extern BYTE gbMustChg;
	gbMustChg = 1;
	OutMode();

	//SensorIn(1);
	//InitIrq();

	//InitContrast();
	ACE_ONw(1);						// ACE ON/OFF
	HEQ_ONw(1);						// DEFOG ON/OFF
	
	HEQ_HWw(0xa);													// Defog window setup
	HEQ_VWw(0xa);													// "
	HEQ_HSPw(0x1c0);												// "
	HEQ_VSPw(0x14);                                          		// "
	HEQ_IIRKw(0xd0);												// IIR Speed

	//InitAwb();
	AWBLNOR_SELw(0x0)			;		// 1 : Always operate, 0 : Use color map

	AWBLOSD_ONw(0x0)       		;		// AWB OSD On/Off
	AWBL_OSD_SELw(0xfb)      	;		// OSD Form
	AWBL_CM_HSPw(0x1f8)			;		// Color map OSD position	//150422 HSH
	AWBL_CM_VSPw(0x138)			;		// "

	AWBL_CYSLICEw(UP(UpAWB_YSLICE));	// Y slice
	AWBL_CYCLIPw(UP(UpAWB_YCLIP));		// Y clip

	AWBL0_DBOFFSETw(UP(UpAWB_DBOFFSET));
	AWBL0_DROFFSETw(UP(UpAWB_DROFFSET));
	AWBL_DBGAIN0w  (AWB_DBGAIN);
	AWBL_DRGAIN0w  (AWB_DRGAIN);

	extern void AwbCmapSet(void);
	AwbCmapSet();

	//InitIMD();
	SetIsp(0x0068, 0x803c0022);								// IMD_WON1 Enable & Area Setting

	WDR_ADRw((ISP_BUF_BASE >> 4) << 1);
#endif
}

void isp_Init(void)
{	// Init Top
	TICK_STA(INIT_TIME);

	InitDataID();

	const UINT nEXR1K_TTMR = mfspr(EXR1K_TTMR);
	SetWaitTimeOffset(MCLK, 1); // It must be run prior to InitTickTimer()
	mtspr(EXR1K_TTMR, nEXR1K_TTMR);

	///*if((_reset_s&FLASH_ADDR_MASK) == 0)*/ TxStrNoIRQ(0, 0, 0);	// for sync with the ECM

	TxStrNoIRQ("\t\t\t\t\t", 0, 0);//TxStrNoIRQ("\t\t\t\t\t", 0, 0);
	TxStrNoIRQ("----- EN673 Init. -----", 0, 0);

	//----------------------------------------------------------------------------------------
	// Init EN673
#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1)) // Init for ISP

	TxStrNoIRQ("ISP Initial...", 0, 0);
	
	Check_I2cDevice();

	gbSensRstPort = GPIO_SENS_RSTN;
	
	gbSensIfMode = model_SensIntf; 
	
	gbIspClkMode = ISP_CLK_MODE;// ISP Clock Mode Setting

	Cstm_OutMode(2);	// 161031 WHL : Default 2M30P

	InitIsp();					// for ISPx

	InitAfODM();
	
	FORCE_ABT_SOFFw(1);    // ABT Set after ISP Init
	
	HLMASK_ONw(1); HLMASK_THw(0);	// Block Masking // 2016331 - WHL
	
	InitIspClk();				// for ISPx
	
	InitIspCstm();

	//----------------------------------------------------------------------------------------
	#if 1// Init Sensor
 
		printf("Sensor Init Start [Sensor model No.%d]\r\n", model_Sens);
		FUNC_TIME_TX_NoIRQ("Sensor Initial time", InitSens(); )

		InitSensCstm();

	#else // ISP Test pattern without Sensor
		InitSensCstm();

		SLVw(0);
		CH_HSSELw(1);
		CH_VSSELw(1);
		INSELw(6);

	#endif

	//InitIrq();					// for IRQ
	
	//----------------------------------------------------------------------------------------

	// Init Function
		APP_READ_CHK_ALL
		AppLoadPar();												// load parameter from memory
	#if model_Par_Save
		if(gbUsrParSaveChk) UsrParReset();							// CAUTION !!! - Only it must be run in the initialization
	#else
		UsrParReset();												// CAUTION !!! - Only it must be run in the initialization
	#endif

		InitPar();													// LVDS timing, KEY, User Custom Paremeter, Etc.

		AppLoadParNone(0);

		InitMenu();

		InitSensCheck();

		//InitAe();

		OutMode();		// Mode 변경 시 WaitVdw() 등을 사용하는 경우 InitIrq()가 먼저 실행되야 함 -> EN673에서는 WaitVdw()를 사용하지 않으므로 InitIrq()가 먼저 실행되지 않아도 됨

		gnClkChgCnt = 0;// H264 & JPEG 를 SKIP하지 않음
		//gnClkChgCnt = 60;// 2초 동안 H264 & JPEG 를 SKIP

		InitAe();		// InitAe()에서 FreqAdjust()호출되어 MN_FPS_HI설정에 따라 동작하므로 OutMode()가 먼저 실행되어야 함

		//SensorIn(1);	// Sensor 설정이 완료되기 전에 잘못된 영상이 입력되는 경우 WDR또는 DNR 메모리의 오동작 방지 -> InitAe()에서 AeDev()호출하여 SensorIn(1) 실행 되므로 주석처리

		InitIrq();		// AR0237에서 LVSD_ON(SensorIn(1)) 전에 "VIRQI_ENw(1) & VIRQW_ENw(1) & IrqEnable(IRQ_ISP) & mtspr(... | EXR1K_SR_IEE)"를 먼저 하면 VLOCKI와 VLOCKW만 연속으로 발생함

		InitContrast();

		TDN_MECH_DAY //         WaitXus(100000); TDN_MECH_HLD       // TDN hold set in the TDN Function

		InitAwb();

		InitIMD();

//		InitShd();	// 2015818 - WHL
#endif

#if(IMAGE_INPUT_MODE!=0)

	InitIrq();					// for IRQ
	
	FORCE_ABT_SOFFw(1);

	Digital_interface_set(); // Digital Input(BT656 or BT1120) Intialization
#endif
	//----------------------------------------------------------------------------------------
	
	#if 0 // System Watchdog Timer Enable
		TxStrNoIRQ("WdtEna", 0, 4);
		WdtEna;							// Enable Watchdog
	#endif

	//----------------------------------------------------------------------------------------
	// Tx Info.
	TxStrNoIRQ("-----------------------------------", 0, 0);
	TxStrNoIRQ("FW Ver.    : ", (model_ver<<16) | gDataID, 8); // F/W Version & Compile Time(Min/Sec)
	TxStrNoIRQ("ISP Clk Mode : ", ISP_CLK_MODE, 2);
	TxStrDecNoIRQ(gsBuildDate, 0, 0);

#if (IMAGE_INPUT_MODE==2)
	TxStrNoIRQ("BT656 YC Multiplexed Input Interface", 0, 0);
	
#elif (IMAGE_INPUT_MODE==3)
	TxStrNoIRQ("BT1120 YC Separated Input Interface", 0, 0);
#else
	if(gbSensCtrMode) TxStrNoIRQ("Sens Reg Ctrl Mode : TWI", 0, 0);
	else			  TxStrNoIRQ("Sens Reg Ctrl Mode : SPI", 0, 0);
#endif

	TxStrNoIRQ("-----------------------------------", 0, 0);
	
	TICK_END(INIT_TIME);
	TxStrDecNoIRQ("Initial Time us:", tick2us(INIT_TIME),8);

	TxStrDecNoIRQ("----- EN673 Start -----", 0,0);
}

/* EOF */
