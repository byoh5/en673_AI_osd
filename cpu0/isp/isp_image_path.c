/* **************************************************************************
 File Name	: 	isp_image_path.c
 Description:	initialize functions
 Designer	:	Kim, Sunghoon
 Modifier	:	
 Date		:	18. 7. 17
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"	// to use fast code

//#include "h264.h"
#include "jpeg.h"

extern void InitIrq(void);
extern void InitWdt(UINT sec);
extern void InitTickTimer(const UINT anCntPerSec);

extern void UsrParReset(void);
extern void InitPar(void);
extern void Isp_SensorRst(void);
extern BYTE gbSensRstPort;

extern void Digital_interface_set(void);

const UINT gnTblHVLockI_Pos[8][2] = {{0x113,0x45c},{0x7df, 0x2e6},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}};
const UINT gnTblHVSPI_Pos[8][2] = {{0,8},{0, 6},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0},{ 0, 0}};

//-------------------------------------------------------------------------------------------------
void ISPCK_SET(const UINT anClkSel)
{
	ISPCLK_SEL(FPCK,anClkSel);		// ISP Pre pixel clock
	ISPCLK_SEL(BT_PCK,anClkSel);	// ISP Pre Sync Gen Block clock (Main Parallel/BT1120)
	ISPCLK_SEL(PPCK,anClkSel);		// ISP Post pixel clock
	ISPCLK_SEL(PSCK,anClkSel);		// ISP Post Sync Gen Block clock
	ISPCLK_SEL(FN_CK,anClkSel);		// Font clock
	ISPCLK_SEL(PR_CK,anClkSel);		// Box/Privacy clock
	ISPCLK_SEL(IMD_CK,anClkSel);	// IMD clock
	ISPCLK_SEL(DCKO0,anClkSel);		// Digital output clock for ch0 (FPGA Only)

	DCKO0_INVw(anClkSel);
}

void DDRCK_SET(const UINT anClkSel)
{
	ISPCLK_SEL(YCW_CK0,anClkSel);
	ISPCLK_SEL(YCW_CK1,anClkSel);
	ISPCLK_SEL(YCW_CK2,anClkSel);
}

void DS0CK_SET(const UINT anClkSel)
{
	ISPCLK_SEL(CDS0_PCK,anClkSel);
}

void DS1CK_SET(const UINT anClkSel)
{
	ISPCLK_SEL(CDS1_PCK,anClkSel);
}

void FPSCK_SET(const UINT anClkSel)
{
	extern void SensorIn(const BYTE abOn);
	SensorIn(0);

#if (model_Sens == SENS_OV2718)
	UNUSED_ARG(anClkSel);
	ISPCK_SET(0);
	DDRCK_SET(0);
	DS0CK_SET(0);
	DS1CK_SET(0);
#else
	ISPCK_SET(anClkSel);
	DDRCK_SET(anClkSel);
	DS0CK_SET(anClkSel);
	DS1CK_SET(anClkSel);
#endif
}

void FPSCK_MAIN_SET(const UINT anClkSel0, const UINT anClkSel1)
{
	ISPCK_SET(anClkSel1);
	DDRCK_SET(anClkSel0);
	DS0CK_SET(anClkSel1);
	DS1CK_SET(anClkSel1);
}

void FPSCK_SUB_SET(const UINT anClkSel0, const UINT anClkSel1)
{
	DDRCK_SET(anClkSel0);
	DS0CK_SET(anClkSel1);
	DS1CK_SET(anClkSel1);
}

//void JPG_ISEL_SET(const UINT anSel)
//{
//	JPG_ISELw(anSel);
//}

void H264_DEFAULT_REG_SET(void)
{
#ifdef __ISP__
	DDR_MEM_WADR_RSTw(1);
	DDR_MEM_RADR_RSTw(1);

  #if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))

	#if   (IMAGE_INPUT_MODE==2) // Second BT656 YC input Only
		UINT ycw_ck_sel = 5;
	#elif (IMAGE_INPUT_MODE==3) // Main BT1120 YC input Only
		UINT ycw_ck_sel = 2;
	#endif

	YCW_CK1_PDw(0);
	YCW_CK1_SELw(ycw_ck_sel);
	YCW_CK1_PDw(1);

	YCW_CK2_PDw(0);
	YCW_CK2_SELw(ycw_ck_sel);
	YCW_CK2_PDw(1);
           
  #endif

#endif
}

void JPGCK_SET(void)
{
	JPGCK_PDw(0);

#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))
  #if (model_Sens == SENS_OV2718)
		JPGCK_SELw(0);
  #else
	if ((jpeg_enc.isp_frame == 50) || (jpeg_enc.isp_frame == 60)) {
		JPGCK_SELw(0); 		// 0: ISP PLL(PLL1) / 2 clock, 148.5
	} else {
		JPGCK_SELw(1);		// 1: ISP PLL(PLL1) / 4 clock, 74.25
	}
  #endif
#elif (IMAGE_INPUT_MODE==2)
	JPGCK_SELw(5);			// 5: GPIO10 / 2 clock
#elif (IMAGE_INPUT_MODE==3)
	JPGCK_SELw(2);			// 2: SS_CKI input clock
#endif

	JPGCK_PDw(1);
}

void JPG_DEFAULT_REG_SET(void)
{
	JPG_ONw(1);

#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))
	JPG_IVSELw(0);			// 0: Main ISP Sync
	JPG_ISELw(4);			// 4: Main ISP image
#elif (IMAGE_INPUT_MODE==2)
	JPG_IVSELw(2);			// 2: Second BT1120/BT656/YC Image input channel V sync
	JPG_ISELw(6);			// 6: Second BT1120/BT656/YC input channel image
#elif (IMAGE_INPUT_MODE==3)
	JPG_IVSELw(1);			// 1: Main BT1120/BT656/YC input V sync
//	JPG_ISELw(2);			// 2: Main BT1120/BT656/YC input channel image
	JPG_ISELw(5);			// 5: Main BT1120/BT656/YC input channel image
#endif

	JPGCK_SET();

#if ((IMAGE_INPUT_MODE==0)||(IMAGE_INPUT_MODE==1))
	JPG_HBLK_SELw(1);
#endif
	JPG_HBLK_CYCw(0x897);

	JPG_MAXw(jpeg_enc.param.max_size);
//	ISP_JPG_QP0;	// quantize0 : set enx_jpeg_set_quantize()
//	ISP_JPG_QP1;	// quantize1 : set enx_jpeg_set_quantize()
//	ISP_JPG_ADR;	// stream address : set jpeg_capture_init(), jpeg_irq_handler()
}

void BT0_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel)
{
	BT_DS_HWI0w(anWidth);
	BT0_ISELw(anInputSel);	// 6:original select, 0,1:down scaler select, 
	BT0_VISELw(anSyncSel);
	BT0_HWSELw(0);			// BT_DS_HWI0 use
}

void BT1_SET(const u32 anWidth, const UINT anInputSel, const UINT anSyncSel)
{
	BT_DS_HWI1w(anWidth);
	BT1_ISELw(anInputSel);	// 6:original select, 0,1:down scaler select, 
	BT1_VISELw(anSyncSel);
	BT1_HWSELw(1);			// BT_DS_HWI1 use

	//TxStr("input_selection:",anInputSel,8);
}

void DS0_OFF(void)
{
	DS0_ONw(0);
	CDS0_PCK_PDw(0);
}

void DS0_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight)
{
	CDS0_PCK_PDw(1);

	CDS0_ISELw(anSyncSel);
	CDS0_VISELw(anSyncSel);

	DS0_VOFFw(anVoffsetSel);
	DS0_DKXw(anHscale);
	DS0_DKYw(anVscale);
	DS0_HWw(anWidth);
	DS0_VWw(anHeight);
	DS0_ONw(1);
}

void DS1_OFF(void)
{
	DS1_ONw(0);
	CDS1_PCK_PDw(0);
}

void DS1_SET(const UINT anSyncSel, const UINT anVoffsetSel, const u32 anHscale, const u32 anVscale, const u32 anWidth, const u32 anHeight)
{
	CDS1_PCK_PDw(1);

	CDS1_ISELw(anSyncSel);
	CDS1_VISELw(anSyncSel);

	DS1_VOFFw(anVoffsetSel);
	DS1_DKXw(anHscale);
	DS1_DKYw(anVscale);
	DS1_HWw(anWidth);
	DS1_VWw(anHeight);
	DS1_ONw(1);
}

#if (IMAGE_INPUT_MODE==1)
void ISPM PIP_Disp(void)
{ // PIP_Disp

	static BYTE bPipOn_Buf=0xFF, bPipColor_Buf=0xFF, bPip1_PosX_Buf=0xFF, bPip1_PosY_Buf=0xFF, bPip1_SizX_Buf=0xFF, bPip1_SizY_Buf=0xFF;

	BYTE bPipExcute = ((bPipOn_Buf!=MP(MpPipOn))||(bPipColor_Buf!=MP(MpPipColor))||(bPip1_PosX_Buf!=MP(MpPip1_PosX))
					 ||(bPip1_PosY_Buf!=MP(MpPip1_PosY))||(bPip1_SizX_Buf!=MP(MpPip1_SizX))||(bPip1_SizY_Buf!=MP(MpPip1_SizY)));

	if(bPipExcute) {
		
		PIP0_ONw(0); PIP0_BOX_ONw(0);

		if(MP(MpPipOn)) {
			
			BT_TRG_DIN0w(0);		// Image 깨지면 조절 ( 0 : Rising / 1 : Falling )
			POL_HSI0w(1);
			POL_VSI0w(1);
			VSYN_NAON0w(1);

			#define FR_HW_1920	1920 // 640
			#define FR_VW_1080	1080 // 480

			const UINT nPIP0_HSP = LibUtlInterp1D (MP(MpPip1_PosX), 0, 0xFF, 0, 0x3FF);
			const UINT nPIP0_VSP = LibUtlInterp1D (MP(MpPip1_PosY), 0, 0xFF, 1, 0x3FF);

			// Digital Input Setting
			HWI0w(FR_HW_1920);
			VWI0w(FR_VW_1080);

			HSPI0w(0);
			VSPI0w(0);

			ISPCLK_SEL(IBT0_PCK,0);
			ISPCLK_SEL(BT0_PCK,1);

			DSENS_ONw(1);			// 따블 센서 On
			BT0_656_DSELw(1);		// SS_DIN 상위 10Bit 선택 (10 ~ 19)
			BT0_656_SELw(1);		// BT656 SYNC 선택
			YC_MOD0w(0);			// YC Mode Off (Sync 사용시 1)
			BT_MODESD0w(1); 		// BT656 Mode
			YCBCR_MOD0w(0); 		// Y CB CR 출력 선택
			INSEL0w(0); 			// 입력 Image 선택

			// Down-Scailer CH0
			SetIsp(0x1aa,0x802010f0);	//	DS Aperture Default Set
			DS0_ONw(1);
			DS0_KXY_ONw(1);
			CDS0_ISELw(2);			// DS 입력 Second Sensor 선택
			CDS0_VISELw(2);
			ISPCLK_SEL(CDS0_PCK,5); // External 2'nd PCLK1/2

			DS0_DKXw(6400/MP(MpPip1_SizX)); // Org
			DS0_DKYw(6400/MP(MpPip1_SizX));
			DS0_HWw(((FR_HW_1920*MP(MpPip1_SizX))/100)); // Org
			DS0_VWw(((FR_VW_1080*MP(MpPip1_SizX))/100));
			DS0_HLPFw(1);
			DS0_VLPFw(1);

			// DDR Write H-Size
			ISP_HWSELw(0);					//	Add MJ
			BT_DS_HWI0w(DS0_HWr);

			// PIP CH0 Setting
			PIP0_RONw(1);
			PIP0_ONw(1);
			PIP0_BOX_ONw(1);
			PIP0_SUB_ISELw(1);
			PIP_MAIN_ISELw(0);
			ISPCLK_SEL(PIP_PPCK,1);

			PIP0_HSPw(nPIP0_HSP);
			PIP0_VSPw(nPIP0_VSP);
			PIP0_HWw((DS0_HWr>>2));
			PIP0_VWw(((DS0_VWr>>2)-1));

			//		   DDR R/W 설정
			ISPCLK_SEL(YCW_CK0,5); // External PCLK1/2
			ISPCLK_SEL(YCR_CK0,1); // Same with Main
			YC_FRC0_ONw(1); 		// Auto FRC On
			ISP_VISELw(2);			// Second Sensor Sync 선택
			ISP_ISELw(0);			// DS0 출력 선택
			ISP_CGOw(1);
			ISP_RADR0_AONw(1);
			ISP_RON0w(1);

			PIP_DDR_LTCw(0x90);

			PIP0_BOX_Yw((UINT)gbPrvcYCbCrTbl[MP(MpPipColor)][0]);
			PIP0_BOX_CBw((UINT)gbPrvcYCbCrTbl[MP(MpPipColor)][1]);
			PIP0_BOX_CRw((UINT)gbPrvcYCbCrTbl[MP(MpPipColor)][2]);

			// Encoder Input CH
//				BT0_ISELw(6);	// Main ISP Image input (Setting in H.264)
		}
		else {
			ISP_CGOw(0);
//				BT0_ISELw(0);
//				PIP0_ONw(0); PIP0_BOX_ONw(0);
		}
	}
	bPipOn_Buf   =MP(MpPipOn);
	bPipColor_Buf=MP(MpPipColor);
	
	bPip1_PosX_Buf=MP(MpPip1_PosX);
	bPip1_PosY_Buf=MP(MpPip1_PosY);
	bPip1_SizX_Buf=MP(MpPip1_SizX);
	bPip1_SizY_Buf=MP(MpPip1_SizY);
}
#endif

void Ext_Digital_DevInit(void)
{
#if (USE_DIGITAL_INPUT==1)

	UINT i=0;

	TxStrNoIRQ("EN332T Initializing...", 0, 0);

	gbSensRstPort = GPIO_SENS_RSTN;

	Isp_SensorRst();

	for (i=0; i<gnTbDSubInputInitEa; i++) EN331_Write((WORD)gnTbDSubInputAddr[i], gnTbDSubInputInit[i]);		// Digital

	#if 0 // for debuging
	UINT nRX_Data=0;
	
	for (i=0; i<gnTbDSubInputInitEa; i++) {EN331_Read((WORD)gnTbDSubInputAddr[i], &nRX_Data); printf("RX Data : 0x%08x\r\n", nRX_Data); }		// Digital
	#endif

	TxStrNoIRQ("EN332T Init Finish.", 0, 0);

	extern void BT_InputCheck(void);
	for (i=0; i<10; i++) BT_InputCheck();		

#elif (USE_DIGITAL_INPUT==2)
	
	extern BYTE PO2210_Write(BYTE appWaddr, BYTE abAddr, BYTE abBuf);

	UINT i=0;

	TxStrNoIRQ("BT656 - PO2210 Initializing...", 0, 0);

	for (i=0; i<gnTbDSubInputInitEa; i++) PO2210_Write(PO2210_DEVICE, (BYTE)((gnTbDSubInputInit[i]>>8)&0xFF), (BYTE)(gnTbDSubInputInit[i]&0xFF));		// Digital

	TxStrNoIRQ("BT656 - PO2210 Init Finish.", 0, 0);

	
#elif (USE_DIGITAL_INPUT==3)
	
	extern BYTE PO2210_Write(BYTE appWaddr, BYTE abAddr, BYTE abBuf);

	UINT i=0;

	TxStrNoIRQ("BT656 - PO3100 Initializing...", 0, 0);
	
	for (i=0; i<gnTbDSubInputInitEa; i++) PO2210_Write(PO3100_DEVICE, (BYTE)((gnTbDSubInputInit[i]>>8)&0xFF), (BYTE)(gnTbDSubInputInit[i]&0xFF));		// Digital

	TxStrNoIRQ("BT656 - PO3100 Init Finish.", 0, 0);

	
#elif (USE_DIGITAL_INPUT==5)

	UINT i=0;

	TxStrNoIRQ("BT1120 - EN331A Initializing...", 0, 0);

//			for (i=0; i<gnTbDSubInputInitEa; i++) EN331_Write((WORD)gnTbDSubInputInit[i][0], gnTbDSubInputInit[i][1]);		// Digital

	BYTE bErr=0;
	UINT nBuff_R=0;
		
	for (i=0; i<gnTbDSubInputInitEa; i++) {
		bErr = EN331_Write((WORD)gnTbDSubInputInit[i][0], gnTbDSubInputInit[i][1]);
		if(bErr) TxStrNoIRQ("EN331 Write Err", bErr, 8);

		EN331_Read((WORD)gnTbDSubInputInit[i][0], &nBuff_R);		// Digital
		if(gnTbDSubInputInit[i][1]==nBuff_R) TxStrNoIRQ("I/F Data Compare Pass : ", gnTbDSubInputInit[i][0], 8);
		else {
			TxStrNoIRQ("I/F Data Compare Fail : ", gnTbDSubInputInit[i][0], 8);
			TxStrNoIRQ("nBuff_Ex   : ", gnTbDSubInputInit[i][1], 8);
			TxStrNoIRQ("nBuff_R    : ", nBuff_R, 8);
	}
	}
	TxStrNoIRQ("BT1120 - EN331A Init Finish.", 0, 0);

#elif (USE_DIGITAL_INPUT==6)
	
	// Add another device initialization

#endif
}

#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
BYTE gbDInit_Finish=0;
#endif

void Digital_interface_set(void)
{
	TxStrNoIRQ("Digital_interface_set..", 0, 0);
	
#if (USE_DIGITAL_INPUT!=0) // Input Device Initial

	Ext_Digital_DevInit();

#endif

	// Input Interface Setting
	#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)) // Sub Channel Input Setting (BT656 & BT1120)
		
	#if (IMAGE_INPUT_MODE==2) // BT656 Input Setting
		ISPCLK_SEL(IBT0_PCK,0); // Second Image Input Channel parallel intput latch Clock ( 0 : Input Clock )
		ISPCLK_SEL(BT0_PCK,1);
		FPSCK_SUB_SET(1, 5);

		BT0_656_SELw(1);		// BT656 SYNC 선택
		BT0_656_DSELw(1);		// SS_DIN 상위 10Bit 선택 (10 ~ 19)
		BT_MODESD0w(1); 		// BT656 Mode

		BT_TRG_DIN0w(1);		// Image 깨지면 조절 ( 0 : Rising / 1 : Falling )
		POL_HSI0w(0);
		POL_VSI0w(1);
		
		VSYN_NAON0w(1);
		
		DSENS_ONw(1);			// Double Sensor On
		YC_MOD0w(0);			// YC Mode Off (Sync 사용시 1)
		YCBCR_MOD0w(0); 		// Y CB CR 출력 선택
	
		INSEL0w(0); 			// 입력 Image 선택

	#elif (IMAGE_INPUT_MODE==3) // BT1120 Input Setting
		ISPCLK_SEL(IBT_PCK,0);  // Main Image Input Channel parallel intput latch Clock ( 0 : Input Clock )
		FPSCK_MAIN_SET(1, 2);

		YCBCR_MODw(0);			// 0 : Y,CB,CR(4:2:2), 1 : Y,C(4:1:1) -> YCbCr -> Font YC -> Main/Sub/Jpeg

		BT_DIN_SELw(1);			// 16/20 bit BT1120 input

		BT0_656_SELw(0);		// BT1120 SYNC 선택
		BT1120_MODw(0);
		BT_MODESD0w(0); 		// BT1120 Mode

		SLVw(3);
		HSPIw(0);
		VSPIw(1);
		CH_DSELw(1);
		CH_HSSELw(1);
		CH_VSSELw(1);
		PCLK_ONw(1);
		IDDR_ONw(1);
		POL_HSIw(1);
		POL_VSIw(1);
		VSYN_NAONw(1);

		DSENS_ONw(0);			// Double Sensor Off

	#endif

		#define BT_FR_HW	((SENSOR_SIZE_W%16) ? (((SENSOR_SIZE_W>>4)+1)<<4) : SENSOR_SIZE_W)
		#define BT_FR_VW	((SENSOR_SIZE_H%16) ? (((SENSOR_SIZE_H>>4)+1)<<4) : SENSOR_SIZE_H)
	
		HLOCKI_POS0w(gnTblHVLockI_Pos[DIGITAL_INPUT_RES][0]); HSPI0w(gnTblHVSPI_Pos[DIGITAL_INPUT_RES][0]);
		VLOCKI_POS0w(gnTblHVLockI_Pos[DIGITAL_INPUT_RES][1]); VSPI0w(gnTblHVSPI_Pos[DIGITAL_INPUT_RES][1]);	

		// Down-Scailer CH0
		SetIsp(0x1aa,0x802010f0);	//	DS Aperture Default Set
		DS0_ONw(1);

		#if (IMAGE_INPUT_MODE==2) // BT656 Input Setting
		
		HWI0w(BT_FR_HW);
		VWI0w(BT_FR_VW);
		
		CDS0_ISELw(2);			// DS 입력 Second Sensor 선택
		CDS0_VISELw(2);
		ISPCLK_SEL(CDS0_PCK,5); // External 2'nd PCLK1/2
	
		// Digital Input Setting
		YCBR2YC_SELw(1);		// Active Change to Second Input
	
		#elif (IMAGE_INPUT_MODE==3) // Main BT1120 Input Setting
		
		HWIw(BT_FR_HW);
		VWIw(BT_FR_VW);
		
		CDS0_ISELw(1);
		CDS0_VISELw(1);
		ISPCLK_SEL(CDS0_PCK,2); // 2 : SS_CKI input clock
			
		// Digital Input Setting
		YCBR2YC_SELw(2);		// Active Change to Second Input
			
		#endif
	
		DS0_DKXw(64);
		DS0_DKYw(64);
		DS0_HWw(BT_FR_HW);
		DS0_VWw(BT_FR_VW);
		DS0_HLPFw(1);
		DS0_VLPFw(1);
		DS1_HLPFw(1);
		DS1_VLPFw(1);
	
		// DDR Write H-Size
		ISP_HWSELw(1);			//	Add MJ
		BT_DS_HWI1w(DS0_HWr);
		
//			CDS1_ISELw(4);			// Second Input to Codec D/S1
//			CDS1_VISELw(2); 		// Second Sync to Codec D/S1

	#endif
	

	#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)) // BT656 or BT1120 Input Setting

		// Common Initial
		DIn_InitMenu();

		UsrParReset();
		
		InitPar();
		
		InitIMD();

		// DDR Setting

		
		// Codec D/S Setting

		
		// Option (Font/Box/IMD/PIP/JPeg) Setting
		#if (IMAGE_INPUT_MODE==2)
			ISPCLK_SEL(PPCK,1); // for ABT		
	//		ISPCLK_SEL(PSCK,5); 	// Post Module Sync Clock  ( 5 : Input Clock/2 )
			ISPCLK_SEL(PR_CK,5);	// Box OSD Pixel Clock ( 5 : Input Clock/2 )
			ISPCLK_SEL(FN_CK,5);	// Font OSD Pixel Clock ( 5 : Input Clock/2 )
			ISPCLK_SEL(IMD_CK,5);	// IMD Pixel Clock ( 5 : Input Clock/2 )

			IMD_ISELw(2);			// Second input to IMD Module
			OSD_ISELw(2);			// Second input to OSD Module
			OSD_IVSELw(1);			// Second Sync to OSD Module
		
			JPG_IVSELw(2);			// Second Sync to JPG
			JPG_ISELw(6);			// Second Input to JPG
			ISPCLK_SEL(JPGCK,5);	// External 2'nd PCLK1/2
			JVLOCK_POSw(4);
		
			FONT_MUTEw(1); // 20170814
		
		#elif (IMAGE_INPUT_MODE==3)
			ISPCLK_SEL(PPCK,1); // for ABT		
//				ISPCLK_SEL(PSCK,2); 	// Post Module Sync Clock ( 2 : SS_CKI input clock )
			ISPCLK_SEL(PR_CK,2);	// Box OSD Pixel Clock ( 2 : SS_CKI input clock )
			ISPCLK_SEL(FN_CK,2);	// Font OSD Pixel Clock ( 2 : SS_CKI input clock )
			ISPCLK_SEL(IMD_CK,2);	// IMD Pixel Clock ( 2 : SS_CKI input clock )
			
			IMD_ISELw(1);			// Main input to IMD Module
			OSD_ISELw(1);			// Main input to OSD Module
//			OSD_IVSELw(1);			// Main Sync to OSD Module
			OSD_IVSELw(2);			// 20170814
			
			JPG_IVSELw(1);
//			JPG_ISELw(2);
			JPG_ISELw(5);
			ISPCLK_SEL(JPGCK,2);
//	JVLOCK_POSw(4);
			JVLOCK_POSw(0); // 20170814

			OSYNC_MODw(0);
			FONT_MUTEw(1); // 20170814

		#endif
				
		gbDInit_Finish=1;
		
	#endif
	
	TxStrNoIRQ("Digital_interface_set Finish.", 0, 0);
}

/* EOF */

