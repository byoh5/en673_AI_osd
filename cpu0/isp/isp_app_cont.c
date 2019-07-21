/* **************************************************************************
 File Name	:	app_cont.c
 Description:	EN673 - Gamma, ACE & Defog function
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 8. 4 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "isp_app_cont.h"

//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------
// Init
void InitHisto(void)
{
	//OSD_MUTEw(0);
	//BOX_HSPw(0);
	//BOX_VSPw(0);

	BOSD_ONw(/*0x7ffff*/0x1ffff);
	BOX_ATT( 0,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT( 8,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 1,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT( 9,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 2,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(10,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 3,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(11,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 4,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(12,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 5,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(13,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 6,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(14,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT( 7,HistoFill,HistoTone,0xff,0x00,0x00); BOX_ATT(15,HistoFill,HistoTone,0xff,0x00,0x00);
	BOX_ATT(16,1,BOX_TONE_25,0xff,0x80,0x80);

	gbHistoM[0] = HISTO0_MAXr;
	gbHistoM[1] = HISTO1_MAXr;
	gbHistoM[2] = HISTO2_MAXr;
	gbHistoM[3] = HISTO3_MAXr;
	gbHistoM[4] = HISTO4_MAXr;
	gbHistoM[5] = HISTO5_MAXr;
	gbHistoM[6] = HISTO6_MAXr;
	gbHistoM[7] = HISTO7_MAXr;
	gbHistoM[8] = HISTO8_MAXr;
	gbHistoM[9] = HISTO9_MAXr;
	gbHistoM[10] = HISTO10_MAXr;
	gbHistoM[11] = HISTO11_MAXr;
	gbHistoM[12] = HISTO12_MAXr;
	gbHistoM[13] = HISTO13_MAXr;
	gbHistoM[14] = HISTO14_MAXr;
	gbHistoM[15] = (1<<HISTO_MAX_BIT);
}

void InitContrast(void)
{
	ACE_ON
	DEFOG_ON

	gnDefog_HS = UP(UpAE2_HSP) + DEFOG_X_OFF;								// Histogram detection area H start
	gnDefog_HE = UP(UpAE2_HEP) - DEFOG_X_OFF;								// "						H end
	gnDefog_VS = UP(UpAE2_VSP);											// "						V start
	gnDefog_VE = UP(UpAE2_VEP);											// "						V end

	UINT 	nHW = gnDefog_HE - gnDefog_HS + 1;
	UINT 	nVW = ((gnDefog_VE<<1)/3) - gnDefog_VS + 1;

	gnAe1Size = nHW * nVW;											// total size

	HEQ_HWw(0xa);													// Defog window setup
	HEQ_VWw(0xa);													// "
	HEQ_HSPw(0x1c0);												// "
	HEQ_VSPw(0x14);                                          		// "

	// Histogram X domain read
	#define HISTO_MAX_VAL0	0x10203040
	#define HISTO_MAX_VAL1	0x6080a0c0
	#define HISTO_MAX_VAL2	0xe0ff


	gnHistoM[10] = 0x100;

	UINT nHistoMax = HISTO_MAX_VAL0;
	gnHistoM[3] = nHistoMax&0xff;
	gnHistoM[2] = (nHistoMax>>8)&0xff;
	gnHistoM[1] = (nHistoMax>>16)&0xff;
	gnHistoM[0] = (nHistoMax>>24)&0xff;

	nHistoMax = HISTO_MAX_VAL1;
	gnHistoM[7] = nHistoMax&0xff;
	gnHistoM[6] = (nHistoMax>>8)&0xff;
	gnHistoM[5] = (nHistoMax>>16)&0xff;
	gnHistoM[4] = (nHistoMax>>24)&0xff;

	nHistoMax = HISTO_MAX_VAL2;
	gnHistoM[9] = nHistoMax&0xff;
	gnHistoM[8] = (nHistoMax>>8)&0xff;

	//gnDefogV = 0;
	//gnDwdrV = 0;

	HEQ_IIRKw(0xd0);												// IIR Speed
}

//-------------------------------------------------------------------------------------------------
// Gamma
UINT ISPM GetGamma(const UINT nG1, const UINT nG2, const UINT nGK)
{
	const UINT nG1L = nG1&0xffff;
	const UINT nG2L = nG2&0xffff;
	const UINT nG1H = nG1>>16;
	const UINT nG2H = nG2>>16;

	const UINT nGL = ((nG1L * (0x80-nGK)) + (nG2L * nGK))>>7;
	const UINT nGH = ((nG1H * (0x80-nGK)) + (nG2H * nGK))>>7;

	return (nGH<<16) | nGL;
}

void ISPM Gamma(void) // 180320 LWH
{
	gbDebugOn = (gbMnDebugFnc==4);

	int i;
	static BYTE bWdrOnBuf = 0xAA, bMpGammaBuf = 0xAA, bMpMinGammaYBuf = 0xAA;
	static BYTE bMpGammaDayBuf = 0xAA, bMpGammaNgtBuf = 0xAA, bMpGammaWdrBuf = 0xAA;
	static WORD wWdrGmkBuf = 0xFFFF;

	const BYTE bMpGamma = (gbWdrOn) ? MP(MpGammaWdr) : MP(MpGamma);

	const BYTE bAutoGammaOff = (MP(MpGammaDay) <= MP(MpGammaNgt)) || (MP(MpGammaDay) == 0) || (bMpGamma != MN_GAMMA_AUTO);

	gbGamma_Excute = (bMpGammaWdrBuf != MP(MpGammaWdr)) || (bMpGammaDayBuf != MP(MpGammaDay)) || (bMpGammaNgtBuf != MP(MpGammaNgt)) || (!bAutoGammaOff) || (bWdrOnBuf != gbWdrOn) || (bMpGammaBuf != MP(MpGamma)) || (wWdrGmkBuf != gwWdrGmk) || (bMpMinGammaYBuf != MP(MpMinGammaY)) || ((!AE_SAT_OFF)&&MP(MpMinGammaY)) ; // When the WDR or Gamma setting changed

	if(gbGamma_Excute)
	{
		if(gbWdrOn && (bMpGamma >= MN_GAMMA_WDR)) { // WDR Gamma
			const UINT nWdrGmk = gwWdrGmk>>4;

			if (UP(UpAE_WDR_STYLE_SEL)==0) {	// 141209
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, GetGamma(*(gnTblGamma_OY0+i), *(gnTblGamma_WY0+i), nWdrGmk));	// y gamma exchange
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, GetGamma(*(gnTblGamma_OC0+i), *(gnTblGamma_WY0+i), nWdrGmk));	// c gamma exchange
			}
			else {
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, GetGamma(*(gnTblGamma_OY0+i), *(gnTblGamma_WY1+i), nWdrGmk));	// y gamma exchange		// 150802
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, GetGamma(*(gnTblGamma_OC0+i), *(gnTblGamma_WC1+i), nWdrGmk));	// c gamma exchange		// 151118
			}

		}
		else { // Normal Gamma
	#if 1
			if(bAutoGammaOff) {
				const BYTE bMpGammaO = (bMpGamma < MN_GAMMA_AUTO) ? bMpGamma : MP(MpGammaDay) ;	// GAMMA가 AUTO로 설정되어 있지만 bAutoGammaOff 조건인 경우 MP(MpGammaDay) 사용

				const UINT *pnTblGmaY = gpTblGamma[bMpGammaO+3];
				const UINT *pnTblGmaC = (gbWdrOn) ? pnTblGmaY : gpTblGamma[bMpGammaO+0];

				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, *(pnTblGmaY+i));	// y gamma exchange
				for (i=0; i<9; i++) SetIsp(CGAMMA_BASE+i, *(pnTblGmaC+i));	// c gamma exchange
			}
			else {
				extern int iTgtMax;
				extern int ISPM TgtMinGet(const int aiTgt, const BYTE abV);
				const int iTgtOri = iTgtMax;//TgtMaxGet(0,0);
				const int iTgtOriMin = TgtMinGet(iTgtOri, MP(MpBrightnessMin));					// MP(MpBrightnessMin) : BRIGHTNESS NIGHT

				extern int TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin);
				const int iTgtSpotBgOri = TgtSpotBg(iTgtOri, MP(MpSatBrt), UP(UpAE_TGT_OFST));	// MP(MpSatBrt) : ANTI-SAT. WEIGHT

				BYTE bGammaIdx;
				extern UINT AutoGamma(const int, const int, const BYTE, const BYTE, BYTE*);
				UINT nAutoGmK = AutoGamma(iTgtSpotBgOri, iTgtOriMin, MP(MpGammaDay), MP(MpGammaNgt), &bGammaIdx);
				const UINT *pnTblGmaY0 = gpTblGamma[bGammaIdx+4];
				const UINT *pnTblGmaY1 = gpTblGamma[bGammaIdx+3];
				const UINT *pnTblGmaC0 = (gbWdrOn) ? pnTblGmaY0 : gpTblGamma[bGammaIdx+1];
				const UINT *pnTblGmaC1 = (gbWdrOn) ? pnTblGmaY1 : gpTblGamma[bGammaIdx+0];

				for (i=0; i<9; i++) {
					const UINT nGmaY = GetGamma(*(pnTblGmaY0+i), *(pnTblGmaY1+i), nAutoGmK);
					const UINT nGmaC = GetGamma(*(pnTblGmaC0+i), *(pnTblGmaC1+i), nAutoGmK);
					SetIsp(YGAMMA_BASE+i, nGmaY);	// y gamma exchange
					SetIsp(CGAMMA_BASE+i, nGmaC);	// c gamma exchange
				}
			}
	#else
			if(AE_SAT_OFF || (MP(MpMinGammaY)==0)) {
				for (i=0; i<9; i++) SetIsp(YGAMMA_BASE+i, *(pnTblGmaY+i));	// y gamma exchange
			}
		#if AE_CTRL_ADV
			else {
				extern int iTgtMax;
				extern int ISPM TgtMinGet(const int aiTgt, const BYTE abV);
				const int iTgtOri = iTgtMax;//TgtMaxGet(0,0);
				const int iTgtOriMin = TgtMinGet(iTgtOri, 0/*MP(MpBrightnessMin)*/);					// 0:최소가능밝기(BRIGHTNESS NIGHT)

			#if 1
				extern int TgtSpotBg(const int aiTgt, const BYTE abV, const int aiTgtMin);
				const int iTgtSpotBgOri = TgtSpotBg(iTgtOriMin, 20/*MP(MpSatBrt)*/, UP(UpAE_TGT_OFST));	// 20:최소가능밝기(ANTI-SAT. WEIGHT)
			#else
				int iTgtSpotBgOri = 20/*MP(MpSatBrt)*/ * (iTgtOriMin - UP(UpAE_TGT_OFST));	// 20:최소가능밝기(ANTI-SAT. WEIGHT)
				iTgtSpotBgOri = (iTgtOriMin - UP(UpAE_TGT_OFST)) - (((iTgtSpotBgOri<<2)-iTgtSpotBgOri+32)>>6);
				if(iTgtSpotBgOri < 0) iTgtSpotBgOri = 0;
			#endif

				BYTE bGammaIdx0;
				extern UINT ISPM SatGamma(const int, const int, BYTE*);
				const UINT nSatGmkY = SatGamma(iTgtOri, iTgtSpotBgOri, &bGammaIdx0);
				const UINT *pnTblGmaY0 = gpTblGamma[(MP(MpGamma)<<1)+bGammaIdx0+1];
				const UINT *pnTblGmaY1 = gpTblGamma[(MP(MpGamma)<<1)+bGammaIdx0+0];

				extern int iTgt;
				const UINT nSatGmkOff = ((iTgt-iTgtOriMin)<<7) / (iTgtOri-iTgtOriMin);	// iTgt==iTgtOri -> 0x80, iTgt==iTgtOriMin -> 0x0

				for (i=0; i<9; i++) {
					UINT nGmaY = GetGamma(*(pnTblGmaY0+i), *(pnTblGmaY1+i), nSatGmkY);

					if(MP(MpMinGammaMode)==MN_SAT_BACKLIGHT_DAY_ONLY) {
						nGmaY = GetGamma(*(pnTblGmaY+i), nGmaY, nSatGmkOff);						
					}

					SetIsp(YGAMMA_BASE+i, nGmaY);	// y gamma exchange

				}
			}
		#endif
	#endif
		}
		YGMA_ONw(UP(UpGAMMA_Y_ON));
		CGMA_ONw(UP(UpGAMMA_C_ON));
	}
	bWdrOnBuf = gbWdrOn; bMpGammaBuf = MP(MpGamma); wWdrGmkBuf = gwWdrGmk; // Setting value buffering
	bMpMinGammaYBuf = MP(MpMinGammaY);
	bMpGammaDayBuf = MP(MpGammaDay);
	bMpGammaNgtBuf = MP(MpGammaNgt);
	bMpGammaWdrBuf = MP(MpGammaWdr);
	
	//GrpAe(GA_ERR_DAY) = YGMY_01r;

	//DebugDisp(gbDebugOn, , "WDR_CNT_", 1, 24, gwWdrCnt 		);
	DebugDisp(gbDebugOn, , "WDR_GMK_", 2, 24, gwWdrGmk>>4	);
}

//-------------------------------------------------------------------------------------------------
// ACE & Defog
#if 1
void ISPM AceDefog(void)
{
	// Ace ---------------------------------------------------------------------------------
	const int iAceAgcPos = MINMAX(giCurAgc, ADNR_AGC_MIN, ADNR_AGC_MID);
	int 	iAceTh1=0, iAceTh2=0, iAceDth=3;
	int		iAceTh1S, iAceTh2S, iAceTh1E, iAceTh2E;
	BYTE 	bAceTblSel=0;

	#if (model_1M)
		ACE_HBw(0xa);
		ACE_VBw(0x5);
		
	#else
		if ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60)) { 		// Crop mode
			ACE_HBw(0xa);
			ACE_VBw(0x5);
		}
		else { 											// 2M
			// HBS,VBS=2 (Block Size = 128)
			ACE_HBw(0xf); // 15*128 = 1920
			ACE_VBw(0x8); //  8*128 = 1024 (Detection area should not exceed!)
		}
	#endif


	if (gbWdrOn==MN_ON) {									// WDR on

		ACEBLOCK_ON

		ACE_GMGNw(0x0);
//		ACE_DTHw(0x4);

		iAceDth = 0x7;
		iAceTh1 = UP(UpACEWDR1_TH);
		iAceTh2 = UP(UpACEWDR2_TH);
	}
	else {															// WDR off
		if ((MP(MpDefog)==MN_ON) || (MP(MpAce) != MN_4sOFF)) {		// ACE, Defog condition

			ACEBLOCK_ON

			if (MP(MpDefog)==MN_ON)	{
				ACE_GMGNw(0x0);
				bAceTblSel = MN_4sMID;
			}
			else {
				ACE_GMGNw(MP(MpAceGmgn)/*0x20*/);
				bAceTblSel = MP(MpAce);
			}

			switch (bAceTblSel) {
				case MN_4sLOW	:	iAceTh1S = 0;					iAceTh1 = UP(UpACETBL1_LOW);	iAceTh1E = UP(UpACETBL1_MID);
									iAceTh2S = 0;					iAceTh2 = UP(UpACETBL2_LOW);	iAceTh2E = UP(UpACETBL2_MID);
									break;
				default			:	
				case MN_4sMID	:	iAceTh1S = UP(UpACETBL1_LOW);	iAceTh1 = UP(UpACETBL1_MID);	iAceTh1E = UP(UpACETBL1_HIGH);
									iAceTh2S = UP(UpACETBL2_LOW);	iAceTh2 = UP(UpACETBL2_MID);	iAceTh2E = UP(UpACETBL2_HIGH);
									break;
				case MN_4sHI	:	iAceTh1S = UP(UpACETBL1_MID);	iAceTh1 = UP(UpACETBL1_HIGH);	iAceTh1E = (iAceTh1<<1)-iAceTh1S;
									iAceTh2S = UP(UpACETBL2_MID);	iAceTh2 = UP(UpACETBL2_HIGH);	iAceTh2E = (iAceTh2<<1)-iAceTh2S;
									break;
			}

			iAceTh1S = MAX(0,iAceTh1S);
			iAceTh1  = MAX(iAceTh1S,iAceTh1);
			iAceTh1E = MAX(iAceTh1,iAceTh1E);

			iAceTh1 = (MP(MpAceBrt)>32)  ? (((iAceTh1E-iAceTh1) * (MP(MpAceBrt) -32))>>5) + iAceTh1 : (((iAceTh1-iAceTh1S) * MP(MpAceBrt))>>5)  + iAceTh1S ;
			iAceTh2 = (MP(MpAceCont)>32) ? (((iAceTh2E-iAceTh2) * (MP(MpAceCont)-32))>>5) + iAceTh2 : (((iAceTh2-iAceTh2S) * MP(MpAceCont))>>5) + iAceTh2S ;

//			ACE_DTHw(0x3);
			iAceDth = 0x3;
		}
		else {														// all off condition

			ACEBLOCK_OFF

			ACE_GMGNw(0x0);

			iAceDth = 0x3;
			iAceTh1 = 0;
			iAceTh2 = 0;
		}
	}

	iAceTh1 = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, iAceTh1, 0x0);	// Decrease by AGC
	iAceTh2 = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, iAceTh2, 0x0);	// "

	ACE_DTHw(iAceDth);
	ACE_TH1w(iAceTh1);
	ACE_TH2w(iAceTh2);

	DebugDisp(gbDebugOn, , "ACE_TH1_", 4, 24, ACE_TH1r);
	DebugDisp(gbDebugOn, , "ACE_TH2_", 5, 24, ACE_TH2r);

	// Defog -------------------------------------------------------------------------------
	if (MP(MpShdDet))		return;							// because AE Window

	// Defog table
	UINT	DefogTbl[3];	// Low, Middle, High
	//Defog Table UserParameter
	DefogTbl[0] = UP(UpDEFOG_TH_LOW);	DefogTbl[1] = MAX(DefogTbl[0],(UINT)UP(UpDEFOG_TH_MID));	DefogTbl[2] = MAX(DefogTbl[1],(UINT)UP(UpDEFOG_TH_HIGH));

  	UINT 	nDefogThres = 0;
//	UINT 	nDefogWeight = 3;		//Defog off  (Weight 0 : Defog 0%, 1 : Defog 25%, 2 : Defog 50%, 3 : Defog 75%, else Defog 100% coverage)
	UINT 	nDefogWeight = 0xC0;	//Defog off  (Weight 0 : Defog 0%, 0x40 : Defog 25%, 0x80 : Defog 50%, 0xC0 : Defog 75%, else Defog 100% coverage)

//	UINT 	nHS = 0;
//	UINT 	nHE = 0;
//	UINT 	nVS = 0;
//	UINT 	nVE = 0;

	gbContrastMode 	= MP(MpDefog);							// menu parameter   OFF/ON
	gbDefogMode 	= MP(MpDefogMode);						// "				MANUAL/AUTO
	gbDefogLevel 	= MP(MpDefogLevel);						// "				STRENGTH LOW/MID/HIGH
	//gbDefogLevel 	= 1/*(AE_SAT_OFF)*/ ? MP(MpDefogLevel) : MN_3sHI ;	// "				STRENGTH LOW/MID/HIGH

	if (gbContrastMode==MN_ON) {	// Defog on

		if(gbDefogMode == MN_DEFOG_AUTO) {
			UINT 	nV_OFF;

//			if (MP(MpFlip)==MN_ON) 	nV_OFF = VWIr - ((gnDefog_VE<<1)/3) - 24;	// Flip condition
//			else 					nV_OFF = 0;
 			nV_OFF = 0;

			// Must be Same 'gnAe1Size' !!!
//			nHS = gnDefog_HS + DEFOG_X_OFF;
//	 		nHE = gnDefog_HE - DEFOG_X_OFF;
//			nVS = gnDefog_VS + nV_OFF;
//			nVE = (gnDefog_VE<<1)/3 + nV_OFF;

 			#if (model_1M)
					gnDefog_HS = UP(UpAE2_HSP) + DEFOG_X_OFF;
					gnDefog_HE = UP(UpAE2_HEP) - DEFOG_X_OFF;
					gnDefog_VS = UP(UpAE2_VSP);
					gnDefog_VE = UP(UpAE2_VEP);
					HEQ_HWw(UP(UpDEFOG_720_HW));	// Defog window setup
					HEQ_VWw(UP(UpDEFOG_720_VW));	// "
					HEQ_HSPw(UP(UpDEFOG_720_HSP));	// "
					HEQ_VSPw(UP(UpDEFOG_720_VSP));  // "
			#else
				if ((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60)) { 		// Crop mode
					gnDefog_HS = UP(UpAE2_HSP) + DEFOG_X_OFF + 0x1e;
					gnDefog_HE = UP(UpAE2_HEP) - DEFOG_X_OFF - 0x15a; //
					gnDefog_VS = UP(UpAE2_VSP);
					gnDefog_VE = UP(UpAE2_VEP) - 0x164; 				// 2d0
					HEQ_HWw(UP(UpDEFOG_720_HW));	// Defog window setup
					HEQ_VWw(UP(UpDEFOG_720_VW));	// "
					HEQ_HSPw(UP(UpDEFOG_720_HSP));	// "
					HEQ_VSPw(UP(UpDEFOG_720_VSP));  // "
				}
				else { 											// 2M
					gnDefog_HS = UP(UpAE2_HSP) + DEFOG_X_OFF;
					gnDefog_HE = UP(UpAE2_HEP) - DEFOG_X_OFF;
					gnDefog_VS = UP(UpAE2_VSP);
					gnDefog_VE = UP(UpAE2_VEP);
					HEQ_HWw(UP(UpDEFOG_1080_HW));	// Defog window setup
					HEQ_VWw(UP(UpDEFOG_1080_VW));	// "
					HEQ_HSPw(UP(UpDEFOG_1080_HSP));	// "
					HEQ_VSPw(UP(UpDEFOG_1080_VSP)); // "
				}
			#endif

			UINT 	nHW = gnDefog_HE - gnDefog_HS + 1;
			UINT 	nVW = ((gnDefog_VE<<1)/3) - gnDefog_VS + 1;

			gnAe1Size = nHW*nVW;

			AE1_HSPw(gnDefog_HS);								// AE1 area H (Histogram window)
			AE1_HEPw(gnDefog_HE);								// "

			AE1_VSPw(gnDefog_VS);								// AE1 area V (Histogram window)
			AE1_VEPw((gnDefog_VE<<1)/3 + nV_OFF);				//

			//															0  1  2  3     4     5     6     7     8  9
			// HISTO10_MAX : 10203040 6080a0c0 0000e0ff				-> 10 20 30 40    60    80    a0    c0    e0 ff 100
			// HISTO15_MAX : 08101820 28303840 48505860 68707f00	->  8 10 18 20 28 30 38 40 48 50 58 60 68 70 7f  80
			//															0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
			gnHistoV[0] = HISTO0_LOCKr;
			gnHistoV[1] = HISTO1_LOCKr;
			gnHistoV[2] = HISTO2_LOCKr;
			gnHistoV[3] = HISTO3_LOCKr;
			gnHistoV[4] = HISTO4_LOCKr + HISTO5_LOCKr;
			gnHistoV[5] = HISTO6_LOCKr + HISTO7_LOCKr;
			gnHistoV[6] = HISTO8_LOCKr + HISTO9_LOCKr;
			gnHistoV[7] = HISTO10_LOCKr + HISTO11_LOCKr;
			gnHistoV[8] = HISTO12_LOCKr + HISTO13_LOCKr;;
			gnHistoV[9] = HISTO14_LOCKr;
		}

		LibDefog(&nDefogThres, &nDefogWeight, DefogTbl, (AE_CTRL_ORIGINAL) ? (gnAeState==3) : (gbAeStg==AE_STG_DSS)); 				// Defog library
	}
//	else nDefogWeight = 1;	// 2015921 - WHL : 25% at EN771								// Defog off
	else nDefogWeight = 64;	// 2015921 - WHL : 25% at EN673								// Defog off

	if (gbWdrOn==MN_ON) nDefogWeight = 1;	// 180426 KSH

	nDefogThres = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, nDefogThres, 0x0);	// Decrease by AGC
	nDefogWeight = LibUtlInterp1D(iAceAgcPos, ADNR_AGC_MIN, ADNR_AGC_MID, nDefogWeight, 0x0);	// "

	HEQ_THw(nDefogThres);								// apply to HEQ
	HEQ_WGw(nDefogWeight);									// Defog Intensity


	// Mon
	DebugDisp(gbDebugOn, , "DFG_THRS", 7, 24, HEQ_THr 	);
	DebugDisp(gbDebugOn, , "DFG_WG__", 8, 24, HEQ_WGr	);
}
#endif

void Histo(void)
{
	//if (MP(MpPrivacy)==MN_ON || gbMnFcsAdjOn) return;

	gbHistoM[0] = HISTO0_MAXr;
	gbHistoM[1] = HISTO1_MAXr;
	gbHistoM[2] = HISTO2_MAXr;
	gbHistoM[3] = HISTO3_MAXr;
	gbHistoM[4] = HISTO4_MAXr;
	gbHistoM[5] = HISTO5_MAXr;
	gbHistoM[6] = HISTO6_MAXr;
	gbHistoM[7] = HISTO7_MAXr;
	gbHistoM[8] = HISTO8_MAXr;
	gbHistoM[9] = HISTO9_MAXr;
	gbHistoM[10] = HISTO10_MAXr;
	gbHistoM[11] = HISTO11_MAXr;
	gbHistoM[12] = HISTO12_MAXr;
	gbHistoM[13] = HISTO13_MAXr;
	gbHistoM[14] = HISTO14_MAXr;
	gbHistoM[15] = (1<<HISTO_MAX_BIT);

	/*gbHistoM[0] = HISTO0_MAXr;*/			BOX_COLOR(0 , (gbHistoM[ 0]               )<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[1] = HISTO1_MAXr;*/			BOX_COLOR(1 , (gbHistoM[ 1] + gbHistoM[ 0])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[2] = HISTO2_MAXr;*/			BOX_COLOR(2 , (gbHistoM[ 2] + gbHistoM[ 1])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[3] = HISTO3_MAXr;*/			BOX_COLOR(3 , (gbHistoM[ 3] + gbHistoM[ 2])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[4] = HISTO4_MAXr;*/			BOX_COLOR(4 , (gbHistoM[ 4] + gbHistoM[ 3])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[5] = HISTO5_MAXr;*/			BOX_COLOR(5 , (gbHistoM[ 5] + gbHistoM[ 4])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[6] = HISTO6_MAXr;*/			BOX_COLOR(6 , (gbHistoM[ 6] + gbHistoM[ 5])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[7] = HISTO7_MAXr;*/			BOX_COLOR(7 , (gbHistoM[ 7] + gbHistoM[ 6])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[8] = HISTO8_MAXr;*/			BOX_COLOR(8 , (gbHistoM[ 8] + gbHistoM[ 7])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[9] = HISTO9_MAXr;*/			BOX_COLOR(9 , (gbHistoM[ 9] + gbHistoM[ 8])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[10] = HISTO10_MAXr;*/		BOX_COLOR(10, (gbHistoM[10] + gbHistoM[ 9])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[11] = HISTO11_MAXr;*/		BOX_COLOR(11, (gbHistoM[11] + gbHistoM[10])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[12] = HISTO12_MAXr;*/		BOX_COLOR(12, (gbHistoM[12] + gbHistoM[11])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[13] = HISTO13_MAXr;*/		BOX_COLOR(13, (gbHistoM[13] + gbHistoM[12])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[14] = HISTO14_MAXr;*/		BOX_COLOR(14, (gbHistoM[14] + gbHistoM[13])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);
	/*gbHistoM[15] = (1<<HISTO_MAX_BIT);*/	BOX_COLOR(15, (gbHistoM[15] + gbHistoM[14])<<(8-HISTO_MAX_BIT-1), 0x80, 0x80);

	UINT i;

	UINT nHistoV[HISTO_VAL_NUM];
	nHistoV[HISTO_VAL_NUM-1] = 0;
	UINT nMax1 = HISTO_VAL_NUM-1;
	UINT nMax2 = HISTO_VAL_NUM-1;
	UINT nHistoVsum = 0;
	//UINT nHistoVsumR = 0;

	//UINT nMaxIdx = 0;
	//UINT nMaxVal = 0;
	//UINT nMinIdx = 0;
	//UINT nMinVal = BitMask_21;

	nHistoV[0] = HISTO0_LOCKr;
	nHistoV[1] = HISTO1_LOCKr;
	nHistoV[2] = HISTO2_LOCKr;
	nHistoV[3] = HISTO3_LOCKr;
	nHistoV[4] = HISTO4_LOCKr;
	nHistoV[5] = HISTO5_LOCKr;
	nHistoV[6] = HISTO6_LOCKr;
	nHistoV[7] = HISTO7_LOCKr;
	nHistoV[8] = HISTO8_LOCKr;
	nHistoV[9] = HISTO9_LOCKr;
	nHistoV[10] = HISTO10_LOCKr;
	nHistoV[11] = HISTO11_LOCKr;
	nHistoV[12] = HISTO12_LOCKr;
	nHistoV[13] = HISTO13_LOCKr;
	nHistoV[14] = HISTO14_LOCKr;
	nHistoV[15] = HISTO15_LOCKr;

	for(i=0; i<HISTO_VAL_NUM; i++)
	{
		nHistoVsum += nHistoV[i];

		//nHistoV[i] = (nHistoV[i]<<4) / (gbHistoM[i] - ((i) ? gbHistoM[i-1] : 0));

		if(nHistoV[nMax1] < nHistoV[i]) {	nMax2 = nMax1;	nMax1 = i;	}
		else if(nHistoV[nMax2] < nHistoV[i]) {	nMax2 = i;	}

#if 0
		const UINT nHistoMsz = gbHistoM[i] - ((i==0) ? 0 : gbHistoM[i-1]);

		if(nMaxVal < nHistoV[i] && nHistoMsz >= 2) {
			nMaxVal = nHistoV[i];
			nMaxIdx = i;
		}

	#if 1
		if(nMinVal > nHistoV[i]) {
			nMinVal = nHistoV[i];
			nMinIdx = i;
		}
	#else
		if(i>0) {
			const UINT nSum = nHistoV[i] + nHistoV[i-1];
			if(nMinVal > nSum) {
				nMinVal = nSum;
				nMinIdx = i;
			}
		}
	#endif
#endif
	}

	const UINT nHistoVmax = nHistoV[nMax1];
	for(i=0; i<HISTO_VAL_NUM; i++)
	{
		nHistoV[i] *= HistoH;
		nHistoV[i] /= nHistoVmax;

#if 1//STR_HISTOGRAM == 1
		//if(MENU_TITLE_IS(OFF)) {
			#define STR_HISTO_Y		0
			#define STR_HISTO_X		38/*24*/

			DispHex(STR_HISTO_Y+i, STR_HISTO_X, gbHistoM[i], 2);
			SETFONTID(STR_HISTO_Y+i, STR_HISTO_X+2, ':');
			DispDec(STR_HISTO_Y+i, STR_HISTO_X+3, nHistoV[i], 3);
		//}
#endif
	}

	//----------------------------------------------------------------------------------------
	// Display Graph
	//DispDbgD("HistoVsum:", nHistoVsum, 10);
	//DispDbgD("HistoVmax:", nHistoVmax, 10);


	/*BOSD_ONw(0x1ffff);
	BOX_ATT( 0,0,0,0xff,0x00,0x00); BOX_ATT( 8,0,0,0xff,0x00,0x00);
	BOX_ATT( 1,0,0,0xff,0x00,0x00); BOX_ATT( 9,0,0,0xff,0x00,0x00);
	BOX_ATT( 2,0,0,0xff,0x00,0x00); BOX_ATT(10,0,0,0xff,0x00,0x00);
	BOX_ATT( 3,0,0,0xff,0x00,0x00); BOX_ATT(11,0,0,0xff,0x00,0x00);
	BOX_ATT( 4,0,0,0xff,0x00,0x00); BOX_ATT(12,0,0,0xff,0x00,0x00);
	BOX_ATT( 5,0,0,0xff,0x00,0x00); BOX_ATT(13,0,0,0xff,0x00,0x00);
	BOX_ATT( 6,0,0,0xff,0x00,0x00); BOX_ATT(14,0,0,0xff,0x00,0x00);
	BOX_ATT( 7,0,0,0xff,0x00,0x00); BOX_ATT(15,0,0,0xff,0x00,0x00);
	BOX_ATT(16,0,0,0xff,0x00,0x00);*/

	// Set Main Box
	BOX(16, HistoX, HistoY, HistoX+HistoW, HistoY+HistoH);

	UINT sx = HistoX;
	UINT ex = sx + (gbHistoM[0]<<HISTO_OUT_BIT);
	UINT sy = (HistoY + HistoH) - nHistoV[0];
	UINT ey = HistoY + HistoH;
	BOX(0,sx,sy,ex,ey);

	#define DispHistoGraph(N) \
	sx = ex;  ex = sx + ((gbHistoM[N] - gbHistoM[N-1])<<HISTO_OUT_BIT);  sy = (HistoY + HistoH) - nHistoV[N];  BOX(N,sx,sy,ex,ey);
	DispHistoGraph(1);
	DispHistoGraph(2);
	DispHistoGraph(3);
	DispHistoGraph(4);
	DispHistoGraph(5);
	DispHistoGraph(6);
	DispHistoGraph(7);
	DispHistoGraph(8);
	DispHistoGraph(9);
	DispHistoGraph(10);
	DispHistoGraph(11);
	DispHistoGraph(12);
	DispHistoGraph(13);
	DispHistoGraph(14);
	DispHistoGraph(15);

	//--------------------------------------------------------------
	extern int iTgt;
	//static BYTE bMpBrightness = 0;
	//static BYTE bMpSatBrt = 0;
	//static BYTE bMpLSpotLv = 0;
	static BYTE bHistoGab1d = 0;
	#define ERR_MGN		8
	const BYTE bHistoGab =	(iTgt <= (80-ERR_MGN)) ? 1 :
							(iTgt <= (152-ERR_MGN) ) ? 2 :
							(iTgt <= (224-ERR_MGN) ) ? 3 :
							//(iTgt <= (296-ERR_MGN) ) ? 4 :
													   4 ;

	static WORD wClipLv1d = 0;
	const WORD wClipLv = (MP(MpBackLight)==MN_BL_BLC) ? AE3_CLIPr : AE2_CLIPr;

	if(bHistoGab1d != bHistoGab || wClipLv1d != wClipLv /*|| bMpBrightness != MP(MpBrightness) || bMpSatBrt != MP(MpSatBrt) || bMpLSpotLv != MP(MpLSpotLv)*/)
	{
		bHistoGab1d = bHistoGab;
		wClipLv1d = wClipLv;
		//bMpBrightness = MP(MpBrightness);
		//bMpSatBrt = MP(MpSatBrt);
		//bMpLSpotLv = MP(MpLSpotLv);

		gbHistoM[0] = bHistoGab;
		for(i=1; i<8/*HISTO_MAX_NUM*/; i++) gbHistoM[i] = gbHistoM[i-1] + bHistoGab;
		for(i=8; i<12/*HISTO_MAX_NUM*/; i++) gbHistoM[i] = gbHistoM[i-1] + (bHistoGab+1);
		gbHistoM[12] = 58;
		gbHistoM[13] = 82;
		gbHistoM[14] = (wClipLv + 1 + (ParAe(PA_TGT_NIGHT_OFS) ? (int)ParAe(PA_TGT_NIGHT_OFS) : 64) - 8/*AE_ERR_MGN*/)>>3;
		gbHistoM[14] = MAX(0x6a, gbHistoM[14]);

		HISTO0_MAXw(gbHistoM[0]);
		HISTO1_MAXw(gbHistoM[1]);
		HISTO2_MAXw(gbHistoM[2]);
		HISTO3_MAXw(gbHistoM[3]);
		HISTO4_MAXw(gbHistoM[4]);
		HISTO5_MAXw(gbHistoM[5]);
		HISTO6_MAXw(gbHistoM[6]);
		HISTO7_MAXw(gbHistoM[7]);
		HISTO8_MAXw(gbHistoM[8]);
		HISTO9_MAXw(gbHistoM[9]);
		HISTO10_MAXw(gbHistoM[10]);
		HISTO11_MAXw(gbHistoM[11]);
		HISTO12_MAXw(gbHistoM[12]);
		HISTO13_MAXw(gbHistoM[13]);
		HISTO14_MAXw(gbHistoM[14]);
	}

#if 0
	//--------------------------------------------------------------
	#define DispHistoSel(N,I,Y,CB,CR) \
	sx = HistoX + (((I==0) ? 0 : gbHistoM[I-1])<<HISTO_OUT_BIT);\
	ex = HistoX + (gbHistoM[I]<<HISTO_OUT_BIT);\
	sy = HistoY - 10;\
	ey = sy + 10;\
	BOX(N,sx,sy,ex,ey)\
	BOX_ATT(N,HistoFill,HistoTone,Y,CB,CR);

	DispHistoSel(17,nMaxIdx,17,248,117);
	DispHistoSel(18,nMinIdx,50,101,248);

	return;
	//--------------------------------------------------------------
	const UINT nHistoMmax = gbHistoM[nMaxIdx] - ((nMaxIdx==0) ? 0 : gbHistoM[nMaxIdx-1]);

  #if 1
	if(nHistoMmax >= 2) {
		if(nMaxIdx > nMinIdx) {
			for(i=nMinIdx; i<nMaxIdx; i++) gbHistoM[i]++;
		}
		else if(nMaxIdx < nMinIdx) {
			for(i=nMaxIdx; i<nMinIdx; i++) gbHistoM[i]--;
		}
  #else
	if(nHistoVmax > nMinVal && nHistoMmax >= 2 && nMinIdx > 0) {
		if(nMaxIdx > nMinIdx) {
			for(i=nMinIdx-1; i<nMaxIdx-1; i++) gbHistoM[i] = gbHistoM[i+1];
			gbHistoM[nMaxIdx-1] = gbHistoM[nMaxIdx-1] + (nHistoMmax>>1);
		}
		else if(nMaxIdx < nMinIdx) {
			//TxStr("Sta ",gbHistoM[0], 2);

			for(i=nMinIdx-1; i>=nMaxIdx+1; i--) gbHistoM[i] = gbHistoM[i-1];
			gbHistoM[nMaxIdx] = ((nMaxIdx==0) ? 0 : gbHistoM[nMaxIdx-1]) + (nHistoMmax>>1);

			/*if(nMaxIdx==1) {
				TxStr("Msz ",nHistoMmax, 2);
				TxStr("End ",gbHistoM[1], 2);
			}*/
		}
  #endif
		if(nMaxIdx != nMinIdx) {
			HISTO0_MAXw(gbHistoM[0]);
			HISTO1_MAXw(gbHistoM[1]);
			HISTO2_MAXw(gbHistoM[2]);
			HISTO3_MAXw(gbHistoM[3]);
			HISTO4_MAXw(gbHistoM[4]);
			HISTO5_MAXw(gbHistoM[5]);
			HISTO6_MAXw(gbHistoM[6]);
			HISTO7_MAXw(gbHistoM[7]);
			HISTO8_MAXw(gbHistoM[8]);
			HISTO9_MAXw(gbHistoM[9]);
			HISTO10_MAXw(gbHistoM[10]);
			HISTO11_MAXw(gbHistoM[11]);
			HISTO12_MAXw(gbHistoM[12]);
			HISTO13_MAXw(gbHistoM[13]);
			HISTO14_MAXw(gbHistoM[14]);
		}
	}
#endif
}

/* EOF */

