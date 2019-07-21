/* **************************************************************************
 File Name	: 	app_tdn.c
 Description:	EN673 - TDN
 Designer	:	Lee, Hoon
 Modifier	:	Kim, Sunghoon
 Date		:	14. 8. 4
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"


UINT gnTdnChkOut=TDN_DAY;

#define ADC_LIMIT 				1023 							// 10bit ADC

BYTE gbTdnDebugOn=0;

void ISPM TDN(void)
{	// Day & Night Filter Changer
 	gbTdnDebugOn = (gbMnDebugFnc==7);

	static 	UINT gnDnChk, gnDnSeq;	// 170530 KSH
	static 	BYTE gbMnTdnBuf;
	static 	UINT gnDnChkBuf, gnDnCnt;
	int  iDnThrs, iDnDayThrs, iDnNightThrs;
	UINT nDnDelay;
	WORD wCdsIn = TDN_CDS_IN;
	static BYTE bTdnInitCnt=0;

	if(bTdnInitCnt<5) { bTdnInitCnt++; if(bTdnInitCnt==5) TDN_MECH_HLD } // TDN MECHA set to Hold state after 100ms at booting

	if (MP(MpTdn) == MN_TDN_EXTERN) {
		iDnThrs			= (int)TDN_THRS	;										// absolute from AGC limit
		iDnThrs			= (iDnThrs>=(int)ADC_LIMIT)		? ADC_LIMIT			:
						  (iDnThrs<=0)					? 0					:	iDnThrs;

		iDnDayThrs 		= (int)MP(MpTdnNDThres)*UP(UpTDN_EXTN_THRS_STEP);
		iDnDayThrs 		= (iDnDayThrs<=0)				? 0					:
						  (iDnDayThrs>=(int)ADC_LIMIT)	? (int)ADC_LIMIT	:	iDnDayThrs;
						  
		iDnNightThrs	= (int)MP(MpTdnDNThres)*UP(UpTDN_EXTN_THRS_STEP);
		iDnNightThrs 	= (iDnNightThrs>=(int)ADC_LIMIT) ? ADC_LIMIT		:
						  (iDnNightThrs<=0)				? 0					:	iDnNightThrs;
	}
	else {
		iDnThrs			= (int)TDN_THRS	;										// absolute from AGC limit
		iDnThrs			= (iDnThrs>=(int)gnAgcLim)		? gnAgcLim			:
					  	  (iDnThrs<=0)					? 0					:	iDnThrs;

		iDnDayThrs 		= (int)iDnThrs  - (int)TDN_MARG ;
		iDnDayThrs 		= (iDnDayThrs<=0)				? 0					:
						  (iDnDayThrs>=(int)gnAgcLim)	? (int)gnAgcLim		:	iDnDayThrs;

		iDnNightThrs 	= (int)iDnThrs  + (int)TDN_MARG ;
		iDnNightThrs 	= (iDnNightThrs>=(int)gnAgcLim) ? gnAgcLim			:
						  (iDnNightThrs<=0)				? 0					:	iDnNightThrs;
	}

	DebugDisp(gbTdnDebugOn, Hex, "AGC_____", 0, 24, giCurAgc);
	DebugDisp(gbTdnDebugOn, Hex, "CDS_IN__", 1, 24, TDN_CDS_IN);
	
	DebugDisp(gbTdnDebugOn, Hex, "N>D Thrs", 3, 24, iDnDayThrs);
	DebugDisp(gbTdnDebugOn, Hex, "D>N Thrs", 4, 24, iDnNightThrs);

	switch (MP(MpTdnDly)){						
		case 0	: 	nDnDelay = TDN_DLY_1 ;		break;		
		case 1	: 	nDnDelay = TDN_DLY_2 ;		break;		
		case 2	: 	nDnDelay = TDN_DLY_3 ;		break;		
		default :   nDnDelay = TDN_DLY_3 ;		break;		
	}

	if (MP(MpTdn)!=gbMnTdnBuf)					gnDnCnt=0;
		
	switch (MP(MpTdn)) {
		case MN_TDN_AUTO	:													// Change by internal AGC value

			if 		(giCurAgc<=iDnDayThrs)		gnDnChk = TDN_DAY;				// Day zone
			else if	(giCurAgc>=iDnNightThrs) 	gnDnChk = TDN_NIT;				// Night zone
			else 								gnDnChk = TDN_HLD;				// Hold zone

			if ((gnDnChk==gnDnChkBuf)&&(gnDnChk!=TDN_HLD)&&(gnDnChk!=gnTdnChkOut)) {
				if (gnDnCnt>=(nDnDelay+1))		{asm volatile("l.nop");}
				else if (gnDnCnt==nDnDelay)		{gnDnSeq=0;gnDnCnt++;}			// TDN sequance start, Delay cnt hold
				else							gnDnCnt++;						// Delay for smooth act
			}
			else 								gnDnCnt=0;							

//			DispDat("AGC_____", 0, 8, giCurAgc);
//			DispDat("DAY_____", 1, 8, iDnDayThrs);
//			DispDat("NIT_____", 2, 8, iDnNightThrs);
//
//			DispDat("CNT_____", 3, 8, gnDnCnt);
//			DispDat("DLY_____", 4, 8, nDnDelay+1);
//
// 			static int k = 0;
//			k++;
//			DispDat("CNT_____", 6, 8, k);
//			DispDat("CNT_____", 7, 8, k/FR_DUTY);

			
			break;

		case MN_TDN_DAY	:
			if (gbMnTdnBuf!=MN_TDN_DAY) 	{gnDnSeq=0; gnDnChk=TDN_DAY;}		
			break;

		case MN_TDN_NIGHT:
			if (gbMnTdnBuf!=MN_TDN_NIGHT) 	{gnDnSeq=0; gnDnChk=TDN_NIT;}		
			break;
		
		case MN_TDN_EXTERN:														// Change by external CDS sensor

 			#if (model_Cds == 1) 												// GPIO input
				wCdsIn = (MP(MpTdnSw) == MN_ON) ? wCdsIn : !wCdsIn;

				if (wCdsIn) 						gnDnChk = TDN_DAY;			// Day zone
				else 								gnDnChk = TDN_NIT;			// Night zone
			#else 																// ADC input
 				if (iDnDayThrs > iDnNightThrs/*MP(MpTdnSw) == MN_ON*/) { 		// EXTERN SW : LOW (=night)
 					if 		(wCdsIn>=iDnDayThrs)	gnDnChk = TDN_DAY;			// Day zone
					else if	(wCdsIn<=iDnNightThrs) 	gnDnChk = TDN_NIT;			// Night zone
					else 							gnDnChk = TDN_HLD;			// Hold zone
 				}
				else { 							// EXTERN SW : HIGH (=day) 
					if 		(wCdsIn<=iDnDayThrs)	gnDnChk = TDN_DAY;			// Day zone
					else if	(wCdsIn>=iDnNightThrs) 	gnDnChk = TDN_NIT;			// Night zone
					else 							gnDnChk = TDN_HLD;			// Hold zone
				}
			#endif

			if ((gnDnChk==gnDnChkBuf)&&(gnDnChk!=gnTdnChkOut)) {
				if (gnDnCnt>=(nDnDelay+1))		{asm volatile("l.nop");}
				else if (gnDnCnt==nDnDelay)		{gnDnSeq=0;gnDnCnt++;}			// TDN sequance start, Delay cnt hold
				else							gnDnCnt++;						// Delay for smooth act
			}
			else 								gnDnCnt=0;						
			break;	
	}

	gnDnChkBuf = gnDnChk;
	gbMnTdnBuf = MP(MpTdn);
	
	if (gnDnSeq<=20) {															// Change Sequance
		if (gnDnSeq==2) {
			switch (gnDnChk) {
				case TDN_NIT	:	TDN_MECH_NIT;	gnTdnChkOut=TDN_NIT; break;
				case TDN_DAY	:	TDN_MECH_DAY;	gnTdnChkOut=TDN_DAY; break;
				default 		:	TDN_MECH_HLD;	break;
			}
		}
		else if (gnDnSeq==16) TDN_MECH_HLD;
//		else if (gnDnSeq==8) TDN_MECH_HLD;
//		else if (gnDnSeq==30) TDN_MECH_HLD;
		gnDnSeq++;
	}
	
////  Monitoring (CAUTION ! Erase after engineering) ----------
//
//	DispDat("CNT       "	, 8, 24, gnDnCnt);
//	DispDat("CHK       "	, 9, 24, gnDnChk);
//	DispDat("SEQ       "	,10, 24, gnDnSeq);
//

	if(gbTdnDebugOn) {
		GRP0 = (UINT)giCurAgc		;
		GRP1 = (UINT)iDnThrs		;
		GRP2 = (UINT)iDnDayThrs		;
		GRP3 = (UINT)iDnNightThrs 	;
		GRP4 = (UINT)TDN_CDS_IN 	;
		GRP5 = (UINT)GpioGetPin(GPIO_LED_CTL) 	;
		TxGrpFnc();
}
}

/* EOF */

