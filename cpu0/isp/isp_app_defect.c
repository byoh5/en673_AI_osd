/* ************************************************************************************************
File Name	: 	app_defect.c
Description :	EN673 - Defect Correction
Designer	:	Lee, Hoon
Date		:	11.5.31

Operation Sequance >>
			pre set up : DSS Max, AGC Middle, DNR High, IRIS close
			Detection  : DCP Defect mem clear -> DCP Defect detection -> DCP Defect mem read -> write Flash
			Correction : DCP Defect mem clear -> Read Flash -> DCP Defect mem write -> DCP Defect correction


 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
************************************************************************************************* */

#include "dev_isp.h"


BYTE gbDefCnt=0;
BYTE gbMnDefectBuf;
//UINT TMP;

//*************************************************************************************************
// Top
//-------------------------------------------------------------------------------------------------
// Auto Defect

#define DF_GA_TBL		
#define DFS_GA_TBL 		

	const int DefectDifGaTbl[3]  = {0x20, 0x30, 0x40};		//  AUTO Defect gain table (Low Agc, Mid Agc, Hi Agc)    
	const int DefectSDifGaTbl[3] = {0x10, 0x08, 0x00};		// AUTO Defect spot gain table (Low Agc, Mid Agc, Hi Agc)

void ISPM DefectAuto(void)
{	// Auto Defect correction 

	int iAgcPos, iGain=0, iSGain = 0;

	if (MP(MpDefDet))	{ DF_DIFGAw(0);	return ; }

	iAgcPos = (giCurAgc<=xUP(UpDEFECT_AGC_MIN)) ? 0 :
			  (giCurAgc<=xUP(UpDEFECT_AGC_MID)) ? 1 :
			  (giCurAgc<=xUP(UpDEFECT_AGC_MAX)) ? 2 : 3;

	switch (iAgcPos) {
		case 0 :	iGain 	= DefectDifGaTbl[0];	break;
		case 1 :	iGain 	= LibUtlInterp1D (giCurAgc, xUP(UpDEFECT_AGC_MIN), xUP(UpDEFECT_AGC_MID), DefectDifGaTbl[0], DefectDifGaTbl[1]); break;
		case 2 :	iGain 	= LibUtlInterp1D (giCurAgc, xUP(UpDEFECT_AGC_MID), xUP(UpDEFECT_AGC_MAX), DefectDifGaTbl[1], DefectDifGaTbl[2]); break;
		case 3 :	iGain 	= DefectDifGaTbl[2];	break;
	}
	iGain = MIN(((iGain * UP(UpDEFECT_GA))>>7), 0x3F);	// 2015615 HSH

	switch (iAgcPos) {
		case 0 :	iSGain 	= DefectSDifGaTbl[0];	break;
		case 1 :	iSGain 	= LibUtlInterp1D (giCurAgc, xUP(UpDEFECT_AGC_MIN), xUP(UpDEFECT_AGC_MID), DefectSDifGaTbl[0], DefectSDifGaTbl[1]); break;
		case 2 :	iSGain 	= LibUtlInterp1D (giCurAgc, xUP(UpDEFECT_AGC_MID), xUP(UpDEFECT_AGC_MAX), DefectSDifGaTbl[1], DefectSDifGaTbl[2]); break;
		case 3 :	iSGain 	= DefectSDifGaTbl[2];	break;
	}
	iSGain = MIN(((iSGain * UP(UpDEFECT_SPOT_GA))>>7), 0x3F);	// 2015615 HSH

	DF_DIFGAw(iGain);
	//DF_DIFTHw(UP(UpDEFECT_DIFTH));		// 12.1.9	// 15.06.17 KSH 

	DFS_DIFGAw(iSGain);
//	DispDat("DF_DIFGA"	, 10, 0, iGain);
}

//-------------------------------------------------------------------------------------------------
// Defect Top
#if 0
void ISPM Defect(void)
{

	// Auto Defect ----------------------------------	
	DefectAuto();	

	// Manual Defect --------------------------------
	if(MP(MpDefDet)){
		if(MP(MpDefDet)^gbMnDefectBuf)	gbDefCnt = 0;
		FONT_ONw(1);	
		DefectDet();
		gbMnDefectBuf = MP(MpDefDet);
	}
	else 	DefectCor();

//	DispDat("CNT_____"	, 9, 2, gbDefCnt);
//	DispDat("CMD_____"	,10, 2, DF_CMDr);
}
#endif


