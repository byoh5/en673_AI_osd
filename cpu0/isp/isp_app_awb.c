/* **************************************************************************
 File Name	: 	app_awb.c
 Description:	EN673 - Auto White Balance module
 Designer	:	Lee, Hoon
 Date		:	14. 12. 05
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "isp_app_awb_tbl.h"
#include "dev_inline.h"

//*******************************************************************************
// Reserved define & Variables
//*******************************************************************************
#define	AWB_ODM_CAP_USE		0									// Use for Eyenix engineering
#if AWB_ODM_CAP_USE
	#include "./misc/awb_odmcap.h"
#endif

#define TestMnl	0

extern int giCmat[3][3];
extern const s64 glMtxCf[9][6];

UINT 	gnAwbRGainIIR, gnAwbBGainIIR;
int  	giAwbDbIIR, giAwbDrIIR;

UINT 	gnAwbErr;
BYTE 	gbAwbDebugOn;
UINT	gnAwbOdmCapOn = 0;
int 	giAwbPrstTbl[4] = { 0, 0, 0x100<<2, 0x100<<2};		// Preset save

#if model_FPU == 1
	#define MTX_CF	gf32MtxCf
	float32 gf32MtxCf[9][6];
	extern void LibAwbCmat (int, int, float32 [9][6]);
#else
	#define MTX_CF	glMtxCf
	extern void LibAwbCmat (int, int, const s64 [9][6]);
#endif

extern const UINT gnTblIspModeAwbCor[][6];


//*************************************************************************************************
// Sub Functions
//*************************************************************************************************

void ISPM AwbCmapSet(void)
{
	int i;
	WORD* pwMapTbl = (WORD*)gwCmap;
	if(MP(MpAwb)==MN_AWB_AUTOEXT) pwMapTbl++;	

	for (i=0; i<64; i++) {
		SetIsp(AWBL_CM_BASE+i, 	*pwMapTbl);
		pwMapTbl += 2;
	}
}

void InitAwb(void)
{	// Initial AWB

//	ICSELw(0x1);		// matching with OCSEL
//	OZONE_SELw(0x3)				;		// ODM OSD Selection Register

	//ParSet(UpAWB_SPEED_MTRX, MP(MpAwbSpeed));
	//ParSet(UpAWB_AGC_WGT_MIN, MP(MpAwbAgcWgtMin));
	//ParSet(UpLSUP_ON, MP(MpLSUP_ON));
	//ParSet(UpAWB_YSLICE, MP(MpAwbYSliceMax));

	AWB_WIN1_HSPw(UP(UpAWB_HSP));
	AWB_WIN1_HWw(UP(UpAWB_HW));
	AWB_WIN1_VSPw(UP(UpAWB_VSP));
	AWB_WIN1_VWw(UP(UpAWB_VW));

	AWBLNOR_SELw(0x0)			;		// 1 : Always operate, 0 : Use color map

	AWBLOSD_ONw(0x0)       		;		// AWB OSD On/Off
	AWBL_OSD_SELw(0xfb)      	;		// OSD Form
	AWBL_CM_HSPw(0x1f8)			;		// Color map OSD position	//150422 HSH
	AWBL_CM_VSPw(0x138)			;		// "

	AWBL_CYSLICEw(UP(UpAWB_YSLICE));	// Y slice
	AWBL_CYCLIPw(UP(UpAWB_YCLIP));		// Y clip

	//AWBL0_DBOFFSETw(UP(UpAWB_DBOFFSET));	// Hue_Chroma_Menu_Set(0); 에서 처리하므로 주석
	//AWBL0_DROFFSETw(UP(UpAWB_DROFFSET));	//	"

	AWBL_DBGAIN0w  (AWB_DBGAIN);
	AWBL_DRGAIN0w  (AWB_DRGAIN);

	giAwbDbIIR = giDbDrMnl[1][0]<<4;
	giAwbDrIIR = giDbDrMnl[1][1]<<4;
	gnAwbRGainIIR = AWB_GAIN_DFLT<<4;
	gnAwbBGainIIR = AWB_GAIN_DFLT<<4;

	AwbCmapSet();

	UINT i;
	for (i=0; i<4; i++) giAwbPrstTbl[i] = (UD(UdAwbPrst+(i*4)+3)<<24) | (UD(UdAwbPrst+(i*4)+2)<<16) | (UD(UdAwbPrst+(i*4)+1)<<8) | (UD(UdAwbPrst+(i*4)+0)<<0);

#if 1//model_ISP_Save == 1
	const int iDbSum = giAwbPrstTbl[0]>>4;
	const int iDrSum = giAwbPrstTbl[1]>>4;
	const UINT nRGain = giAwbPrstTbl[2]>>4;
	const UINT nBGain = giAwbPrstTbl[3]>>4;

	if( (AWB_DB_MIN <= iDbSum && iDbSum <= AWB_DB_MAX) &&
		(AWB_DR_MIN <= iDrSum && iDrSum <= AWB_DR_MAX) &&
		(AWB_GAIN_MIN <= nRGain && nRGain <= AWB_GAIN_MAX) &&
		(AWB_GAIN_MIN <= nBGain && nBGain <= AWB_GAIN_MAX) ) {
			giAwbDbIIR = giAwbPrstTbl[0];
			giAwbDrIIR = giAwbPrstTbl[1];
			gnAwbRGainIIR = giAwbPrstTbl[2];
			gnAwbBGainIIR = giAwbPrstTbl[3];

			TxStrDec("AWB : Set to preset values.", 0, 0);
	}
	else {	// Press 모드 Data가 없는 경우 초기값 설정
		giAwbPrstTbl[0] = giAwbDbIIR;
		giAwbPrstTbl[1] = giAwbDrIIR;
		giAwbPrstTbl[2] = (int)gnAwbRGainIIR;
		giAwbPrstTbl[3] = (int)gnAwbBGainIIR;

		TxStrDec("AWB : Set to default values.", 0, 0);
	}

	//TxStrDec("Db:", iDbSum, 4);
	//TxStrDec("Dr:", iDrSum, 4);
	//TxStrDec("Gr:", nRGain, 4);
	//TxStrDec("Gb:", nBGain, 4);
#endif

//#if (model_Save==0)		// EEPROM only
//	TwiRdEep(TWI_EEP_AWBPRST_STT , (BYTE*)giAwbPrstTbl, 16);
//#else
//
//#endif

#if model_FPU == 1
	UINT j;
	for(j=0; j<9; j++) {
		for(i=0; i<6; i++) {
			const float a = (float)glMtxCf[j][i];
			gf32MtxCf[j][i] = TOFLOAT32(a);
		}
	}

	//extern float gfAwbMatGain2;
	const float gfAwbMatGain2 = (double)AWB_MTRX_GAIN * 256. / 100000000000000.;

	extern float32 gf32AwbMatGain;
	gf32AwbMatGain = TOFLOAT32(gfAwbMatGain2);
#else
	extern double gdAwbMatGain;
	gdAwbMatGain = (double)AWB_MTRX_GAIN;

	//extern double gdAwbMatGain2;
	//gdAwbMatGain2 = (double)AWB_MTRX_GAIN * 256. / 100000000000000.;
#endif
	
}

void ISPtoMENU(void)
{
	#define ISP_TO_MENU(ISP)	((ISP>=0x81) ? ISP-0x81 : (ISP==0x80) ? 0xff : ISP+0x7f)
	
	MP(MphueBn) = ISP_TO_MENU(BYHUENr);	// MENU BYHUEN DEFAULT
	MP(MphueBp) = ISP_TO_MENU(BYHUEPr);	// MENU BYHUEP DEFAULT
	MP(MphueRn) = ISP_TO_MENU(RYHUENr);	// MENU RYHUEN DEFAULT
	MP(MphueRp) = ISP_TO_MENU(RYHUEPr);	// MENU RYHUEP DEFAULT
}

void MENUtoISP(void)
{
	UINT phueval;

	#define MENU_TO_ISP(MENU)	((0x81 <= (MENU+0x81) && (MENU+0x81) <= 0xff ) ? (MENU+0x81) : ((MENU-0x7f)==0x80) ? 0x7f : (MENU-0x7f))

	phueval = MENU_TO_ISP(MP(MphueBn));	// HUE Bn
	ParSet(UpBY_Hue_Negative,phueval);
	BYHUENw(UP(UpBY_Hue_Negative));

	phueval = MENU_TO_ISP(MP(MphueBp));	// HUE Bp
	ParSet(UpBY_Hue_Positive,phueval);
	BYHUEPw(UP(UpBY_Hue_Positive));

	phueval = MENU_TO_ISP(MP(MphueRn));	// HUE Rn
	ParSet(UpRY_Hue_Negative,phueval);
	RYHUENw(UP(UpRY_Hue_Negative));

	phueval = MENU_TO_ISP(MP(MphueRp));	// HUE Rp
	ParSet(UpRY_Hue_Positive,phueval);
	RYHUEPw(UP(UpRY_Hue_Positive));
}

void Hue_Chroma_Menu_Set(int MenuState)
{
	if(MenuState==0)	// HUE, CHROMA INITAL
	{
		if((model_Sens==SENS_IMX291) && (MP(MpAwbStyle)==1)) {
			AWBL0_DBOFFSETw(0x2c);
			AWBL0_DROFFSETw(0xf5);
		}
		else {
			AWBL0_DBOFFSETw(UP(UpAWB_DBOFFSET));
			AWBL0_DROFFSETw(UP(UpAWB_DROFFSET));
		}
		
		SetIsp(0xE8,gnTblIspModeAwbCor[0][MP(MpAwbStyle)]);  
		SetIsp(0xE9,gnTblIspModeAwbCor[1][MP(MpAwbStyle)]);  
		
		ParSet(UpBY_Gain_Negative,BYGAINNr);
		ParSet(UpBY_Gain_Positive,BYGAINPr);
		ParSet(UpRY_Gain_Negative,RYGAINNr);
		ParSet(UpRY_Gain_Positive,RYGAINPr);
		
		ParSet(UpBY_Hue_Negative,BYHUENr);
		ParSet(UpBY_Hue_Positive,BYHUEPr);
		ParSet(UpRY_Hue_Negative,RYHUENr);
		ParSet(UpRY_Hue_Positive,RYHUEPr);

		// CHROMA MENU INITAL
		MP(MpgainBn)=BYGAINNr;
		MP(MpgainBp)=BYGAINPr;
		MP(MpgainRn)=RYGAINNr;
		MP(MpgainRp)=RYGAINPr;

		// HUE MENU INITAL
		ISPtoMENU();
	}
	else if(MenuState==1)	// CHROMA MENU INITAL WITH PAR
	{
		MP(MpgainBn)=UP(UpBY_Gain_Negative);
		BYGAINNw(UP(UpBY_Gain_Negative));

		MP(MpgainBp)=UP(UpBY_Gain_Positive);
		BYGAINPw(UP(UpBY_Gain_Positive));


		MP(MpgainRn)=UP(UpRY_Gain_Negative);
		RYGAINNw(UP(UpRY_Gain_Negative));

		MP(MpgainRp)=UP(UpRY_Gain_Positive);
		RYGAINPw(UP(UpRY_Gain_Positive));

		
		// HUE MENU INITAL WITH PAR
		BYHUENw(UP(UpBY_Hue_Negative));
		BYHUEPw(UP(UpBY_Hue_Positive));
		RYHUENw(UP(UpRY_Hue_Negative));
		RYHUEPw(UP(UpRY_Hue_Positive));

		ISPtoMENU();
	}
 	else if(MenuState==2)	// CHROMA VALUE CHANGE WITH MENU
 	{		
		ParSet(UpBY_Gain_Negative,MP(MpgainBn));
		BYGAINNw(MP(MpgainBn));
	
		ParSet(UpBY_Gain_Positive,MP(MpgainBp));
		BYGAINPw(MP(MpgainBp));
				
		ParSet(UpRY_Gain_Negative,MP(MpgainRn));
		RYGAINNw(MP(MpgainRn));
				
		ParSet(UpRY_Gain_Positive,MP(MpgainRp));
		RYGAINPw(MP(MpgainRp));
 	}
 	else if(MenuState==3)	// HUE VALUE CHANGE WITH MENU
 	{
		MENUtoISP();	
 	}
}


//*************************************************************************************************
// Main Functions
//*************************************************************************************************

void ISPM Awb(void)
{
	int		iDrSum=0, iDgSum=0, iDbSum=0;
	UINT	nTcnt=0, nHold=0;

	UINT	nRsum,  nGsum,  nBsum,  nRGain, nBGain, nAwbRgain, nAwbGgain, nAwbBgain;

	int		iRcm0=0x100,iRcm1=0x0,iRcm2=0x0,iGcm0=0x0,iGcm1=0x100,iGcm2=0x0,iBcm0=0x0,iBcm1=0x0,iBcm2=0x100;
	UINT	nRcm0,nRcm1,nRcm2,nGcm0,nGcm1,nGcm2,nBcm0,nBcm1,nBcm2;

//	double	oIntpRgain=1,oIntpBgain=1;

	int 	iAgcPos,iWgt;
	static BYTE	gbMnAwbBuf;

	gbAwbDebugOn = (gbMnDebugFnc==3);

//--------------------------------------------------------------------------------------------------
// Exception

	if (WDR_ONr&&((LS_FLGr&0x00000001)==AeLONG))	return;		// WDR (process at Short and apply to Long)
#if AWB_ODM_CAP_USE
	else if (gnAwbOdmCapOn&0x1) {
		AWBLOSD_ONw(1);
		OZONE_SELw (3);
		AwbOdmCap();
		return;
	}
#endif

// -------------------------------------------------------------------------------
// ODM Area

	if(gbAwbDebugOn&&(!gbMnBlcOsdOn)) {
		AWBLOSD_ONw(1);
		OZONE_SELw (3);
	}
	else {
		AWBLOSD_ONw(0);

	#if (model_1M)

			AWB_WIN1_HSPw(UP(UpAWB_HSP));
			AWB_WIN1_HWw(UP(UpAWB_HW));
			AWB_WIN1_VSPw(UP(UpAWB_VSP));
			AWB_WIN1_VWw(UP(UpAWB_VW));

	#elif (model_2M|model_2M30p)

		#if (model_2M)
			const BYTE b720CropMode = ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60));
		#else
			const BYTE b720CropMode = ((MP(MpOutFps)==MN_2M30_720p_25_30)||(MP(MpOutFps)==MN_2M30_720p_50_60));
		#endif
		
		if (b720CropMode) {
			AWB_WIN1_HSPw(UP(UpAWB_HSP_CROP));
			AWB_WIN1_HWw(UP(UpAWB_HW_CROP));
			AWB_WIN1_VSPw(UP(UpAWB_VSP_CROP));
			AWB_WIN1_VWw(UP(UpAWB_VW_CROP));
		}
		else {
			AWB_WIN1_HSPw(UP(UpAWB_HSP));
			AWB_WIN1_HWw(UP(UpAWB_HW));
			AWB_WIN1_VSPw(UP(UpAWB_VSP));
			AWB_WIN1_VWw(UP(UpAWB_VW));
		}
	#endif
	}

	if ((!((gbMnDebugFnc>=1)&&(gbMnDebugFnc<=3)))&&(gbMnBlcOsdOn==0)&&(MP(MpShdDet)!=1)&&(gbOzone_Hold!=1))	OZONE_SELw(0);		// final OSD Off condition // 150325 WHL
	
//--------------------------------------------------------------------------------------------------
// ColorMap Set

	if (MP(MpAwb)!=gbMnAwbBuf) {
		AwbCmapSet();
		gbMnAwbBuf = MP(MpAwb);
	}

//--------------------------------------------------------------------------------------------------
// Color Matrix control

	nTcnt		= AWBL_CCT_SUM_LOCKr	;											// Total block cnt
	iDrSum		= AWBL_CRDR_SUM_LOCKr	;     										// CR/DR(32b sign)
	iDgSum		= AWBL_CY_SUM_LOCKr		;											// CY	(31b)
	iDbSum		= AWBL_CBDB_SUM_LOCKr	;	     									// CB/DB(32b sign)

#if 1//model_ISP_Save == 1
	static BYTE bInitAwb = /*(model_ISP_Save == 1) ? 3 :*/ 1;
	if(bInitAwb) {
		//TxStrDec("AWB X:", iDgSum / (int)nTcnt, 4);
	}
	else {
		static BYTE bDispCnt = 5;
		if(bDispCnt) {
			//TxStrDec("AWB O:", iDgSum / (int)nTcnt, 4);
			bDispCnt--;
		}
	}
#else
	#define bInitAwb 0
#endif

	nHold = (nTcnt<(UINT)UP(UpAWB_TCNT_LMT))? 1 : 0;											// AWB Hold flag by block cnt.

	if (MP(MpAwb)==MN_AWB_MNL){														// Manual Matrix
		switch (MP(MpAwbMnl)) {
			case MN_3sLOW :	iDbSum	= giDbDrMnl[MN_3sLOW][0];		iDrSum  = giDbDrMnl[MN_3sLOW][1];	break;
			case MN_3sMID :	iDbSum	= giDbDrMnl[MN_3sMID][0];		iDrSum	= giDbDrMnl[MN_3sMID][1];	break;
			case MN_3sHI  :	iDbSum	= giDbDrMnl[MN_3sHI][0] ;		iDrSum	= giDbDrMnl[MN_3sHI][1];	break;
			default 			: 	break;
		}
	}
	else if (bInitAwb || ((MP(MpAwb)==MN_AWB_PRESET)&&(MP(MpAwbPrst)==MN_OFF)) || (nHold&0x1) ) {	// Hold Matrix
		if (bInitAwb || (MP(MpAwb)==MN_AWB_PRESET)) {
			iDbSum = giAwbPrstTbl[0]>>4;
			iDrSum = giAwbPrstTbl[1]>>4;
		}
		else {
			iDbSum	= giAwbDbIIR >> 4;
			iDrSum	= giAwbDrIIR >> 4;
			nHold  += 0x10;
		}
	}
	else if (MP(MpAwb)==MN_AWB_AUTOEXT) {
		iDbSum	= giDbDrMnl[MN_3sMID][0];
		iDrSum	= giDbDrMnl[MN_3sMID][1];
	}
	else{
		iDbSum	= iDbSum / (int)nTcnt;												// Pixel mean
		iDrSum	= iDrSum / (int)nTcnt;												// "
		iDgSum	= iDgSum / (int)nTcnt;												// "
	}

	iDbSum 		= MIN(iDbSum,AWB_DB_MAX);											// ODM data limitaion
	iDbSum 		= MAX(iDbSum,AWB_DB_MIN);											// "
	iDrSum 		= MIN(iDrSum,AWB_DR_MAX);           								// "
	iDrSum 		= MAX(iDrSum,AWB_DR_MIN);           								// " max limit

	giAwbDbIIR 	= ( ((int)UP(UpAWB_SPEED_MTRX)*giAwbDbIIR) + ((int)(0x100-UP(UpAWB_SPEED_MTRX))*(iDbSum<<4)) ) >> 8;
	giAwbDrIIR 	= ( ((int)UP(UpAWB_SPEED_MTRX)*giAwbDrIIR) + ((int)(0x100-UP(UpAWB_SPEED_MTRX))*(iDrSum<<4)) ) >> 8;

	gnAwbErr	= MAX((UINT)ABS((giAwbDbIIR>>4) - iDbSum), (UINT)ABS((giAwbDrIIR>>4) - iDrSum));

//	DispDatDec("AWB_P1 :", 20, 24, giAwbDrIIR);

	LibAwbCmat ((giAwbDbIIR>>4), (giAwbDrIIR>>4), MTX_CF);
//	DispDatDec("AWB_P2 :", 20, 24, giAwbDrIIR);

// ------------------------------------------------------------------
// Apply Pre ODM Matrix  (Sign[11], Integer[10:8], Float[7:0])
//
	if (!TestMnl) {
		iRcm0 =  MIN(giCmat[0][0],2047);	iRcm0 =  MAX(iRcm0,0    );
		iRcm1 =  MIN(giCmat[0][1],2047);	iRcm1 =  MAX(iRcm1,-2047);
		iRcm2 =  MIN(giCmat[0][2],2047);	iRcm2 =  MAX(iRcm2,-2047);
		iGcm0 =  MIN(giCmat[1][0],2047);	iGcm0 =  MAX(iGcm0,-2047);
		iGcm1 =  MIN(giCmat[1][1],2047);	iGcm1 =  MAX(iGcm1,0	);
		iGcm2 =  MIN(giCmat[1][2],2047);	iGcm2 =  MAX(iGcm2,-2047);
		iBcm0 =  MIN(giCmat[2][0],2047);	iBcm0 =  MAX(iBcm0,-2047);
		iBcm1 =  MIN(giCmat[2][1],2047);	iBcm1 =  MAX(iBcm1,-2047);
		iBcm2 =  MIN(giCmat[2][2],2047);	iBcm2 =  MAX(iBcm2,0    );
	}
	else {	// Manual control (affect to Post Matrix)
		iRcm0 = ((PRR_GAINr)>>11) ? -(PRR_GAINr&BitMask_11) : (PRR_GAINr&BitMask_11);
		iRcm1 = ((PRG_GAINr)>>11) ? -(PRG_GAINr&BitMask_11) : (PRG_GAINr&BitMask_11);
		iRcm2 = ((PRB_GAINr)>>11) ? -(PRB_GAINr&BitMask_11) : (PRB_GAINr&BitMask_11);
		iGcm0 = ((PGR_GAINr)>>11) ? -(PGR_GAINr&BitMask_11) : (PGR_GAINr&BitMask_11);
		iGcm1 = ((PGG_GAINr)>>11) ? -(PGG_GAINr&BitMask_11) : (PGG_GAINr&BitMask_11);
		iGcm2 = ((PGB_GAINr)>>11) ? -(PGB_GAINr&BitMask_11) : (PGB_GAINr&BitMask_11);
		iBcm0 = ((PBR_GAINr)>>11) ? -(PBR_GAINr&BitMask_11) : (PBR_GAINr&BitMask_11);
		iBcm1 = ((PBG_GAINr)>>11) ? -(PBG_GAINr&BitMask_11) : (PBG_GAINr&BitMask_11);
		iBcm2 = ((PBB_GAINr)>>11) ? -(PBB_GAINr&BitMask_11) : (PBB_GAINr&BitMask_11);
	}

	nRcm0 = ABS(iRcm0);					nRcm0 =  						    nRcm0;
	nRcm1 = ABS(iRcm1);     			nRcm1 = (iRcm1<0) ? (1<<11)|nRcm1 : nRcm1;
	nRcm2 = ABS(iRcm2);     			nRcm2 = (iRcm2<0) ? (1<<11)|nRcm2 : nRcm2;
	nGcm0 = ABS(iGcm0);     			nGcm0 = (iGcm0<0) ? (1<<11)|nGcm0 : nGcm0;
	nGcm1 = ABS(iGcm1);     			nGcm1 =  							nGcm1;
	nGcm2 = ABS(iGcm2);     			nGcm2 = (iGcm2<0) ? (1<<11)|nGcm2 : nGcm2;
	nBcm0 = ABS(iBcm0);     			nBcm0 = (iBcm0<0) ? (1<<11)|nBcm0 : nBcm0;
	nBcm1 = ABS(iBcm1);     			nBcm1 = (iBcm1<0) ? (1<<11)|nBcm1 : nBcm1;
	nBcm2 = ABS(iBcm2);     			nBcm2 =  							nBcm2;

	// Pre mat (temp)
	PRR_GAINw(nRcm0);
	PRG_GAINw(nRcm1);
	PRB_GAINw(nRcm2);
	PGR_GAINw(nGcm0);
	PGG_GAINw(nGcm1);
	PGB_GAINw(nGcm2);
	PBR_GAINw(nBcm0);
	PBG_GAINw(nBcm1);
	PBB_GAINw(nBcm2);

//-------------------------------------------------------------------------------------------------
// RGB gain control

	nRsum 	= AWBL_CCR_SUM_LOCKr	;											// R from Pre ODM Mat
	nGsum 	= AWBL_CCG_SUM_LOCKr	;       									// G "
	nBsum 	= AWBL_CCB_SUM_LOCKr	;       									// B "

	if (MP(MpAwb)==MN_AWB_MNL){													// Manual Gain
		nRsum 	= 0;
		nGsum 	= 0;
		nBsum 	= 0;
//		nRGain  = MP(MpRgain) * 26;												// x (gain dflt * 2(max multi) / 20(par range))
//		nBGain  = MP(MpBgain) * 26;
		nRGain  = (((int)MP(MpRgain)-10)>=0) ? (UINT)((float)(MP(MpRgain)-10)*AWB_GAIN_STEP_H)+AWB_GAIN_DFLT :
											   (UINT)((float)(MP(MpRgain)   )*AWB_GAIN_STEP_L)+AWB_GAIN_MIN  ;
		nBGain  = (((int)MP(MpBgain)-10)>=0) ? (UINT)((float)(MP(MpBgain)-10)*AWB_GAIN_STEP_H)+AWB_GAIN_DFLT :
											   (UINT)((float)(MP(MpBgain)   )*AWB_GAIN_STEP_L)+AWB_GAIN_MIN  ;
	}
	else if (bInitAwb || ((MP(MpAwb)==MN_AWB_PRESET)&&(MP(MpAwbPrst)==MN_OFF)) || (nHold&0x1) ) {	// Hold Gain
		if (bInitAwb || (MP(MpAwb)==MN_AWB_PRESET)) {					// Preset save example
			nRGain = giAwbPrstTbl[2]>>4;
			nBGain = giAwbPrstTbl[3]>>4;
		}
		else {
			nRGain  = gnAwbRGainIIR >> 4;
			nBGain  = gnAwbBGainIIR >> 4;
			nHold  += 0x100;
		}
		nRsum 	= 0;
		nGsum 	= 0;
		nBsum 	= 0;
	}
	else {																		// Auto Gain
		nRsum 	= nRsum/ nTcnt;
		nGsum 	= nGsum/ nTcnt;
		nBsum 	= nBsum/ nTcnt;

		nRsum 	= MAX(nRsum, 1);
		nGsum 	= MAX(nGsum, 1);
		nBsum 	= MAX(nBsum, 1);

//		nRGain  = (nGsum*AWB_GAIN_DFLT) / ((nRsum*AWB_GAIN_DFLT)/(gnAwbRGainIIR>>4));				// gain with default offset		// new
//		nBGain  = (nGsum*AWB_GAIN_DFLT) / ((nBsum*AWB_GAIN_DFLT)/(gnAwbBGainIIR>>4));				// "							// new

		nRGain = (nGsum*AWB_GAIN_DFLT)/nRsum;								// gain with default offset
		nBGain = (nGsum*AWB_GAIN_DFLT)/nBsum;
	}

//	DispDatDec("AWB_P3 :", 20, 24, giAwbDrIIR);

	nRGain		= MAX(AWB_GAIN_MIN, nRGain);									// Gain limitation
	nRGain		= MIN(AWB_GAIN_MAX, nRGain);              						//	"
	nBGain		= MAX(AWB_GAIN_MIN, nBGain);      	    						//	"
	nBGain		= MIN(AWB_GAIN_MAX, nBGain);    	        					//	"

	gnAwbRGainIIR 	= ( (AWB_SPEED_GAIN*gnAwbRGainIIR) + ((0x100-AWB_SPEED_GAIN)*(nRGain<<4)) ) >> 8;
	gnAwbBGainIIR 	= ( (AWB_SPEED_GAIN*gnAwbBGainIIR) + ((0x100-AWB_SPEED_GAIN)*(nBGain<<4)) ) >> 8;

	gnAwbErr	= MAX(gnAwbErr, (UINT)ABS((int)(gnAwbRGainIIR>>4) - (int)nRGain));
	gnAwbErr	= MAX(gnAwbErr, (UINT)ABS((int)(gnAwbBGainIIR>>4) - (int)nBGain));

//-------------------------------------------------------------------------------------------------
// Final Gain with weights

	if (!TestMnl) {

		iAgcPos = (giCurAgc<=UP(UpAWB_AGC_MIN)) ? 0 :
				  (giCurAgc<=AWB_AGC_MAX) ? 1 : 2;

		const int iAWB_AGC_WGT_MIN = 0x40/*UP(UpAWB_AGC_WGT_MIN)*/;

		switch (iAgcPos) {
			case 0 :	iWgt 	= AWB_AGC_WGT_MAX;	break;
			case 1 :	iWgt 	= LibUtlInterp1D (giCurAgc, UP(UpAWB_AGC_MIN), AWB_AGC_MAX, AWB_AGC_WGT_MAX, iAWB_AGC_WGT_MIN/*UP(UpAWB_AGC_WGT_MIN)*/); break;
			case 2 :	iWgt 	= iAWB_AGC_WGT_MIN/*UP(UpAWB_AGC_WGT_MIN)*/;	break;
		}

		if(AE_SAT_OFF) {
//			AWBL_CYSLICEw(UP(UpAWB_YSLICE));
			AWBL_CYSLICEw(AE2_SLICEr); // follow AE Slice 2019-4-2 KDH 
		}
		else {
#if 0
			const int iCurBgSum1 = iCurBgSum1IIR>>(IIG+IKG-8);
			//const int iCurBgSum1Max = MIN(0x30, iTgt);

			const int iCurBgSum1Cut = MINMAX(iCurBgSum1, (0x10<<8), (0x30<<8));
			const int iWgtSat = LibUtlInterp1D(iCurBgSum1Cut, (0x10<<8), (0x30<<8), UP(UpAWB_AGC_WGT_MIN), AWB_AGC_WGT_MAX);
			if(iWgt > iWgtSat) iWgt = iWgtSat;
			
			const int iAWB_YSLICE = LibUtlInterp1D(iCurBgSum1Cut, (0x10<<8), (0x30<<8), MP(MpAwbYSliceMin), UP(UpAWB_YSLICE));
			AWBL_CYSLICEw(iAWB_YSLICE);
#endif
			//#define SLIC_CNT_NOR_MIN	700
			//#define SLIC_CNT_NOR_MAX	1023
			const int SLIC_CNT_NOR_MIN = MP(MpAwbSliceCntMin)<<6;
			const int SLIC_CNT_NOR_MAX = MP(MpAwbSliceCntMax)<<6;
			
			int iSlicCntNor;
			if(MP(MpBackLight)==MN_BL_BLC) {
				const UINT nAe2SlicCnt = AE2_SLCNT_LOCKr;
				const UINT nAe2PxCnt = ((AE2_VEPr - AE2_VSPr)+1) * ((AE2_HEPr - AE2_HSPr)+1);
				iSlicCntNor = (nAe2SlicCnt<<10)/nAe2PxCnt;
			}
			else {
				const UINT nAe3SlicCnt = AE3_SLCNT_LOCKr;
				const UINT nAe3PxCnt = ((AE3_VEPr - AE3_VSPr)+1) * ((AE3_HEPr - AE3_HSPr)+1);
				iSlicCntNor = (nAe3SlicCnt<<10)/nAe3PxCnt;
			}

			const int iSlicCntNorCur = MINMAX(iSlicCntNor, SLIC_CNT_NOR_MIN, SLIC_CNT_NOR_MAX);

			/*if(UP(UpLSUP_ON) == 0) {
				const int iWgtSat = LibUtlInterp1D(iSlicCntNorCur, SLIC_CNT_NOR_MIN, SLIC_CNT_NOR_MAX, AWB_AGC_WGT_MAX, UP(UpAWB_AGC_WGT_MIN));
				if(iWgt > iWgtSat) iWgt = iWgtSat;
			}*/
			
			if(UP(UpAWB_YSLICE) == MP(MpAwbYSliceMin)) {
//				AWBL_CYSLICEw(UP(UpAWB_YSLICE));
				AWBL_CYSLICEw(AE2_SLICEr); // follow AE Slice 2019-4-2 KDH 				
			}
			else {
				const int iAWB_YSLICE = LibUtlInterp1D(iSlicCntNorCur, SLIC_CNT_NOR_MIN, SLIC_CNT_NOR_MAX, UP(UpAWB_YSLICE), MP(MpAwbYSliceMin));
				AWBL_CYSLICEw(iAWB_YSLICE);
			}
		}

		iWgt = (gnTdnChkOut==TDN_DAY)? iWgt : 0;			// Day or B&W

		// ---------------------
#if 0
		int iDrCur = iDrSum, iWgt2;
		iDrCur = MAX(iDrCur, giDbDrMnl[1][1]);
		iDrCur = MIN(iDrCur, AWB_DR_MAX);
		iWgt2  = LibUtlInterp1D (iDrCur, giDbDrMnl[1][1], AWB_DR_MAX, AWB_AGC_WGT_MAX, (((AWB_AGC_WGT_MAX-iAWB_AGC_WGT_MIN/*UP(UpAWB_AGC_WGT_MIN)*/)<<3)>>4)+iAWB_AGC_WGT_MIN/*UP(UpAWB_AGC_WGT_MIN)*/);
//		DispDat("WGT2____"	, 13, 24, iWgt2);

		iWgt   = MIN(iWgt, iWgt2);
#endif

//		nAwbRgain 	= ((WORD)((float)(gnAwbRGainIIR>>4) * (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;		// divide by iWgt scale
//		nAwbGgain 	= ((WORD)((float)AWB_GAIN_DFLT   	* (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;
//		nAwbBgain 	= ((WORD)((float)(gnAwbBGainIIR>>4) * (0.5f+((float)MP(MpSaturation)*0.05f))) * iWgt) >> 8;

		const BYTE bMpSaturationR = (gbWdrOn) ? ((40-10)>MP(MpSaturationR)) ? (MP(MpSaturationR)+10) : 40 : MP(MpSaturationR) ;
		const BYTE bMpSaturationG = (gbWdrOn) ? ((40-10)>MP(MpSaturation))  ? (MP(MpSaturation) +10) : 40 : MP(MpSaturation)  ;
		const BYTE bMpSaturationB = (gbWdrOn) ? ((40-10)>MP(MpSaturationB)) ? (MP(MpSaturationB)+10) : 40 : MP(MpSaturationB) ;

		nAwbRgain 	= ((WORD)((float)(gnAwbRGainIIR>>4) * (0.5f+((float)bMpSaturationR*0.025f))) * iWgt) >> 8;		// divide by iWgt scale
		nAwbGgain 	= ((WORD)((float)AWB_GAIN_DFLT   	* (0.5f+((float)bMpSaturationG*0.025f))) * iWgt) >> 8;
		nAwbBgain 	= ((WORD)((float)(gnAwbBGainIIR>>4) * (0.5f+((float)bMpSaturationB*0.025f))) * iWgt) >> 8;
	}
	else {
		nAwbRgain 	= AWB_GAIN_DFLT;
		nAwbGgain 	= AWB_GAIN_DFLT;
		nAwbBgain 	= AWB_GAIN_DFLT;
	}

////-------------------------------------------------------------------------------------------------
//// Post Matrix (final with R,B Gain)(Sign[12], Integer[10:8], Float[7:0])
//
	iRcm0 = (iRcm0 * (int)nAwbRgain) >> 8;				// divide by Gain scale
	iRcm1 = (iRcm1 * (int)nAwbRgain) >> 8;
	iRcm2 = (iRcm2 * (int)nAwbRgain) >> 8;
	iGcm0 = (iGcm0 * (int)nAwbGgain) >> 8;
	iGcm1 = (iGcm1 * (int)nAwbGgain) >> 8;
	iGcm2 = (iGcm2 * (int)nAwbGgain) >> 8;
	iBcm0 = (iBcm0 * (int)nAwbBgain) >> 8;
	iBcm1 = (iBcm1 * (int)nAwbBgain) >> 8;
	iBcm2 = (iBcm2 * (int)nAwbBgain) >> 8;

	nRcm0 = ABS(iRcm0);					nRcm0 =  						    nRcm0;
	nRcm1 = ABS(iRcm1);     			nRcm1 = (iRcm1<0) ? (1<<12)|nRcm1 : nRcm1;
	nRcm2 = ABS(iRcm2);     			nRcm2 = (iRcm2<0) ? (1<<12)|nRcm2 : nRcm2;
	nGcm0 = ABS(iGcm0);     			nGcm0 = (iGcm0<0) ? (1<<12)|nGcm0 : nGcm0;
	nGcm1 = ABS(iGcm1);     			nGcm1 =  							nGcm1;
	nGcm2 = ABS(iGcm2);     			nGcm2 = (iGcm2<0) ? (1<<12)|nGcm2 : nGcm2;
	nBcm0 = ABS(iBcm0);     			nBcm0 = (iBcm0<0) ? (1<<12)|nBcm0 : nBcm0;
	nBcm1 = ABS(iBcm1);     			nBcm1 = (iBcm1<0) ? (1<<12)|nBcm1 : nBcm1;
	nBcm2 = ABS(iBcm2);     			nBcm2 =  							nBcm2;

	WPRR_GAINw(nRcm0);
	WPRG_GAINw(nRcm1);
	WPRB_GAINw(nRcm2);
	WPGR_GAINw(nGcm0);
	WPGG_GAINw(nGcm1);
	WPGB_GAINw(nGcm2);
	WPBR_GAINw(nBcm0);
	WPBG_GAINw(nBcm1);
	WPBB_GAINw(nBcm2);

//	DispDatDec("AWB_P4 :", 20, 24, giAwbDrIIR);

//-------------------------------------------------------------------------------------------------
// Preset save example	13.2.22
	static UINT	gnAwbPrstCnt,gnAwbPrstDone=0;

	if((MP(MpAwb)==MN_AWB_PRESET)&&(MP(MpAwbPrst)==MN_ON)) {	// preset key push

		if ((gnAwbPrstCnt>=0x40)&&(gnAwbErr<0x18)&&(!gnAwbPrstDone)) {		// save on condition
			gnAwbPrstCnt = 0;
			giAwbPrstTbl[0] = giAwbDbIIR;
			giAwbPrstTbl[1] = giAwbDrIIR;
			giAwbPrstTbl[2] = (int)gnAwbRGainIIR;
			giAwbPrstTbl[3] = (int)gnAwbBGainIIR;

			gnAwbPrstDone = 1;

			UINT i;
			for (i=0; i<4; i++)	 UD(UdAwbPrst+ 0+i)	= (giAwbPrstTbl[0]>>(i*8)) & 0xff;
			for (i=0; i<4; i++)	 UD(UdAwbPrst+ 4+i)	= (giAwbPrstTbl[1]>>(i*8)) & 0xff;
            for (i=0; i<4; i++)	 UD(UdAwbPrst+ 8+i)	= (giAwbPrstTbl[2]>>(i*8)) & 0xff;
            for (i=0; i<4; i++)	 UD(UdAwbPrst+12+i)	= (giAwbPrstTbl[3]>>(i*8)) & 0xff;
            gbUsrDataSaveChk = 1;

//			DebugDisp(1, Hex, "Verify", 17, 24, giAwbPrstTbl[0])
//			DebugDisp(1, Hex, "Verify", 18, 24, giAwbPrstTbl[1])
//			DebugDisp(1, Hex, "Verify", 19, 24, giAwbPrstTbl[2])
//			DebugDisp(1, Hex, "Verify", 20, 24, giAwbPrstTbl[3])

		}
		else gnAwbPrstCnt ++;

		gnAwbPrstCnt = MIN(gnAwbPrstCnt, 0x100);

	}
	else {
		gnAwbPrstCnt = 0;
		gnAwbPrstDone = 0;
	}

// -------------------------------------------------------------------------------
// AWB State Auto Save
	if(bInitAwb) bInitAwb--;


//	DispDatDec("AWB_P5 :", 20, 24, giAwbDrIIR);

//	DispDat("prstDB__"	, 16, 24, giAwbPrstTbl[0]);
//	DispDat("prstDR__"	, 17, 24, giAwbPrstTbl[1]);
//	DispDat("prstRG__"	, 18, 24, giAwbPrstTbl[2]);
//	DispDat("prstBG__"	, 19, 24, giAwbPrstTbl[3]);
//	DispDat("prstCNT_"	, 20, 24, gnAwbPrstCnt);
//	DispDat("err_____"	, 21, 24, gnAwbErr);
//-------------------------------------------------------------------------------------------------
// Monitoring

//	DebugDisp(1,Dec ,"AWB_Db_MAX", 32,10, AWB_DB_MAX);
//	DebugDisp(1,Dec ,"AWB_Db_MIN", 33,10, AWB_DB_MIN);
//
//	DebugDisp(1,Dec ,"AWB_Dr_MAX", 34,10, AWB_DR_MAX);
//	DebugDisp(1,Dec ,"AWB_Dr_MIN", 35,10, AWB_DR_MIN);

	DebugDisp(gbAwbDebugOn, Dec, "TCNT___d"	,  1, 24, nTcnt 	)
	DebugDisp(gbAwbDebugOn, Dec, "CRDR___d"	,  2, 24, iDrSum	)
	DebugDisp(gbAwbDebugOn, Dec, "CY_____d"	,  3, 24, iDgSum	)
	DebugDisp(gbAwbDebugOn, Dec, "CBDB___d"	,  4, 24, iDbSum	)
	DebugDisp(gbAwbDebugOn, Dec, "R______d"	,  6, 24, nRsum		)
	DebugDisp(gbAwbDebugOn, Dec, "G______d"	,  7, 24, nGsum		)
	DebugDisp(gbAwbDebugOn, Dec, "B______d"	,  8, 24, nBsum		)
	DebugDisp(gbAwbDebugOn,    , "R_Gain__"	, 10, 24, nAwbRgain	)
	DebugDisp(gbAwbDebugOn,    , "B_Gain__"	, 11, 24, nAwbBgain	)
	DebugDisp(gbAwbDebugOn,    , "WGT_____"	, 12, 24, iWgt		)
	DebugDisp(gbAwbDebugOn,    , "Hold____"	, 14, 24, nHold		)
	DebugDisp(gbAwbDebugOn,    , "Done____"	, 15, 24, gnAwbErr	)
//	DebugDisp(gbAwbDebugOn,    , "nRGain"	, 15, 24, nRGain	)
//	DebugDisp(gbAwbDebugOn,    , "nBGain"	, 16, 24, nBGain	)

}	// end of AWB

/* EOF */

