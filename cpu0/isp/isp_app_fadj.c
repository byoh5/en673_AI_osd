/* **************************************************************************
 File Name	: 	app_focus_assist.c
 Description:	EN673 - Focus Assist
 Designer	:	Lee, Wanhee
 Date		:	13. 07. 28
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

#define EDGE_CNT_TEST	0
//*************************************************************************************************
// Lib Variable
//-------------------------------------------------------------------------------------------------
const UINT giFad_HOfs = 320, giFad_VOfs = 360;

UINT gnFA_Pos_Control_Y[3]     = {0,}; // Bar1~3		// !
UINT gnFA_Pos_Control_Y_Dly[3] = {0,}; // Bar1~3
UINT gnFA_Pos_Control_Y_Buf[3][10]; // Bar1~3 Buff		// !

int giFA_DirCnt[10] = {0,};	// !
int giFA1_SUM2_LOCKr=0, giFA1_YSUM2_LOCKr=0, giFA1_DetTPixel=0;

UINT gnFaPosY_Min=0, gnFaPosY_Max=0;
BYTE gbFad_SizY_Buf=0, gbOzone_Hold=0;
UINT gnFaDispScale[3] = {2,10,50};

#if (model_Lens==0)
//*************************************************************************************************
// Define & Variable
//-------------------------------------------------------------------------------------------------

EXTERN_STR(FOCUS_AREA);
EXTERN_STR(FOCUS_ADJ);

void FA_EnvChange(void);
void FocusAssist_Init(void);
void Focus_Assist(void);
extern void FocusADJ_Data_Processing(void);

const UINT gnFad_HSP1[3] = {0x140, 0x0C2, 0x040};
const UINT gnFad_HEP1[3] = {0x27e, 0x2FB, 0x37D}; // -320
const UINT gnFad_VSP1[3] = {0x113, 0x08A, 0x005};
const UINT gnFad_VEP1[3] = {0x325, 0x3AD, 0x43a}; // -360

UINT gnBoxPosFADJ[FADJ_EA*2];
UINT gnBoxPltFADJ[FADJ_EA];
const UINT gnBoxBToneFADJ[3][2] = {{0x0A,0xAAA80AAA},{0x05,0x55540555},{0,0}};
UINT gnBoxToneFADJ[2] = {0,0};
const UINT gnBox_BFL_FADJ = 0x0003FE3F;


#if (model_1M)		// 1M
	#define	FAD_FR_HW	(FR_HW/2)
	#define	FAD_FR_VW	FR_VW

	#define FA_BT_VSP	MIN(21,MP(MpFad_PosY))	// 150423 HSH
	#define FA_BT_VW	MP(MpFad_SizY)
	#define FA_BT_HSP	MIN(38,MP(MpFad_PosX))	// 150423 HSH
	#define FA_BT_HW	MP(MpFad_SizX)
	
#else 				// 2M
	#define	FAD_FR_HW	(((MP(MpOutFps)==MN_2M_1080p_25_30)|(MP(MpOutFps)==MN_2M_1080p_50_60)) ? FR_HW/2 : (FR_HW-640)/2)
	#define	FAD_FR_VW	(((MP(MpOutFps)==MN_2M_1080p_25_30)|(MP(MpOutFps)==MN_2M_1080p_50_60)) ? FR_VW   : (FR_VW-360))

	#define FA_BT_VSP	(((MP(MpOutFps)==MN_2M_1080p_25_30)|(MP(MpOutFps)==MN_2M_1080p_50_60)) ? MIN(32,MP(MpFad_PosY)) : MINMAX(MP(MpFad_PosY),12,32))	// 150423 HSH
	#define FA_BT_VW	MP(MpFad_SizY)
	#define FA_BT_HSP	(((MP(MpOutFps)==MN_2M_1080p_25_30)|(MP(MpOutFps)==MN_2M_1080p_50_60)) ? MIN(58,MP(MpFad_PosX)) : MIN(38,MP(MpFad_PosX)))			// 150423 HSH
	#define FA_BT_HW	MP(MpFad_SizX)
	
#endif

#define FA_B18_VSP	(FA_BT_VSP*32) // 50
#define FA_B18_VW	(FA_BT_VW *32) // 550
#define FA_B18_HSP	(FA_BT_HSP*32) // 50
#define FA_B18_HW	(FA_BT_HW *32) // 550

#define FA_B17_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B17_VW	(FA_B18_VW/10)
#define FA_B17_HSP	(FA_B18_HSP+FA_B18_HW/10)
#define FA_B17_HW	(FA_B18_HW/10)
#define FA_B16_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B16_VW	(FA_B18_VW/10)
#define FA_B16_HSP	(FA_B18_HSP+FA_B18_HW/5)
#define FA_B16_HW	(FA_B18_HW/10)

#define FA_B15_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B15_VW	(FA_B18_VW/10)
#define FA_B15_HSP	(FA_B18_HSP+FA_B18_HW/10*4)
#define FA_B15_HW	(FA_B18_HW/15)
#define FA_B14_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B14_VW	(FA_B18_VW/10)
#define FA_B14_HSP	(FA_B18_HSP+FA_B18_HW/10*4+FA_B18_HW/15)
#define FA_B14_HW	(FA_B18_HW/15)
#define FA_B13_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B13_VW	(FA_B18_VW/10)
#define FA_B13_HSP	(FA_B18_HSP+FA_B18_HW/10*4+FA_B18_HW/15*2)
#define FA_B13_HW	(FA_B18_HW/15)

#define FA_B12_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B12_VW	(FA_B18_VW/10)
#define FA_B12_HSP	(FA_B18_HSP+FA_B18_HW/10*7)
#define FA_B12_HW	(FA_B18_HW/20)
#define FA_B11_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B11_VW	(FA_B18_VW/10)
#define FA_B11_HSP	(FA_B18_HSP+FA_B18_HW/10*7+FA_B18_HW/20)
#define FA_B11_HW	(FA_B18_HW/20)
#define FA_B10_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B10_VW	(FA_B18_VW/10)
#define FA_B10_HSP	(FA_B18_HSP+FA_B18_HW/10*7+FA_B18_HW/10)
#define FA_B10_HW	(FA_B18_HW/20)
#define FA_B09_VSP	(FA_B18_VSP+FA_B18_VW/20)
#define FA_B09_VW	(FA_B18_VW/10)
#define FA_B09_HSP	(FA_B18_HSP+FA_B18_HW/10*7+FA_B18_HW/20*3)
#define FA_B09_HW	(FA_B18_HW/20)

#define FA_B08_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B08_VW	(FA_B18_VW*7/10)
#define FA_B08_HSP	(FA_B18_HSP+FA_B18_HW/10)
#define FA_B08_HW	(FA_B18_HW/5)
#define FA_B07_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B07_VW	(FA_B18_VW*7/10)
#define FA_B07_HSP	(FA_B18_HSP+FA_B18_HW/10*4)
#define FA_B07_HW	(FA_B18_HW/5)
#define FA_B06_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B06_VW	(FA_B18_VW*7/10)
#define FA_B06_HSP	(FA_B18_HSP+FA_B18_HW/10*7)
#define FA_B06_HW	(FA_B18_HW/5)

#define FA_B05_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B05_VW	(FA_B18_VW*7/10)
#define FA_B05_HSP	(FA_B18_HSP+FA_B18_HW/10)
#define FA_B05_HW	(FA_B18_HW/5)
#define FA_B04_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B04_VW	(FA_B18_VW*7/10)
#define FA_B04_HSP	(FA_B18_HSP+FA_B18_HW/10*4)
#define FA_B04_HW	(FA_B18_HW/5)
#define FA_B03_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B03_VW	(FA_B18_VW*7/10)
#define FA_B03_HSP	(FA_B18_HSP+FA_B18_HW/10*7)
#define FA_B03_HW	(FA_B18_HW/5)

#define FA_B02_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B02_VW	(FA_B18_VW*7/10)
#define FA_B02_HSP	(FA_B18_HSP+FA_B18_HW/10)
#define FA_B02_HW	(FA_B18_HW/5)
#define FA_B01_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B01_VW	(FA_B18_VW*7/10)
#define FA_B01_HSP	(FA_B18_HSP+FA_B18_HW/10*4)
#define FA_B01_HW	(FA_B18_HW/5)
#define FA_B00_VSP	(FA_B18_VSP+FA_B18_VW/5)
#define FA_B00_VW	(FA_B18_VW*7/10)
#define FA_B00_HSP	(FA_B18_HSP+FA_B18_HW/10*7)
#define FA_B00_HW	(FA_B18_HW/5)


// BOXxx_EX,SX Setting
#define FA_Pos_SY_BASE	(FA_B00_VSP<<16)
#define FA_Pos_SY_MIN  	FA_B00_VSP				// Data min pos
#define FA_Pos_SY_MAX  	(FA_B00_VSP+FA_B00_VW)	// Data max pos

BYTE gbFocusAssist_Init = 1;
BYTE gbFocusADJ_En  = 0;
BYTE gbFajd_LensBuf = 0;

// BOXxx_ATT Setting            		  ATT
const UINT gnFA_ATT_Init[FADJ_EA] = { 0x00f000c0, // Box 0 - Bar1/100-3
		                              0x00f000c0, // Box 1 - Bar1/100-2
		                              0x00f000c0, // Box 2 - Bar1/100-1
		                              0x0170483a, // Box 3 - Bar1/10-3
		                              0x0170483a, // Box 4 - Bar1/10-2
		                              0x0170483a, // Box 5 - Bar1/10-1
		                              0x01808080, // Box 6 - Bar1-3
		                              0x01808080, // Box 7 - Bar1-2
		                              0x01808080, // Box 8 - Bar1-1
		                              0x02108080, // Box 9 - Bar3-B1
		                              0x02B48080, // Box10 - Bar3-B2
		                              0x02108080, // Box11 - Bar3-B3
		                              0x02B48080, // Box12 - Bar3-B4
		                              0x02108080, // Box13 - Bar2-B1
		                              0x02B48080, // Box14 - Bar2-B2
		                              0x02108080, // Box15 - Bar2-B3
		                              0x02B48080, // Box16 - Bar1-B1
		                              0x02108080, // Box17 - Bar1-B2
		                              0x02B48080  // Box18 - Big Box
	                                };

//*************************************************************************************************
// Functions
//-------------------------------------------------------------------------------------------------

void ISPM FocusAssist_Init(void)
{
	int i;

	gbFocusAssist_Init=0;
	AF_WIN_SELw(0x01);
	AF_SLICEw(0);
	AF_EDGE_TH1w(0x100);
	AF_EDGE_TH2w(0x50);
	AF_CLIPw(0x3C0);
	AF_CLIP_ONw(0);
	
	AF_YSELw(2);
	AF_PDw(0);
	AF_CLIP_ONw(0);

	for(i=0; i<FADJ_EA; i++) {
		if(i<10) {
			if(i<3) gnFA_Pos_Control_Y_Dly[i] = 0;
			giFA_DirCnt[i] = 0;
		}
		gnBoxPltFADJ[i] = gnFA_ATT_Init[i];
	}
}

void ISPM FA_EnvChange(void)
{
	if(MP(MpFad_On)) {
		gbFocusAssist_Init = 1;
		gbFajd_LensBuf = MP(MpIris);
		MP(MpIris) = MN_LENS_MNL; // FOCUS ELC Mode
	}
	else MP(MpIris) = gbFajd_LensBuf;
}

#if (EDGE_CNT_TEST)
int giAF1_EGCNT_Buff[10]={0,};
int8 gbFocusLED_Assist_Dir[10]={0,};
int giAFEdge_Dc=0, giAFEdge_De=0;

#define FA_DAVG_OFS    		20
#define AF_DAVG_RATE    	1.5f
#define AFECNT_MAX			3

#define AFDATA_RESPONS		(ABS(giAFEdge_Dc)>giAFEdge_De)
#define AFDATA_INCREASE		(giAF1_EGCNT_Buff[0]>giAF1_EGCNT_Buff[1])
void FocusADJ_DirCnt(void)
{
	int i=0;
	static BYTE bInit_Flag=1;

	if(bInit_Flag) {
		bInit_Flag=0;
		for(i=0; i<10; i++) {
			giAF1_EGCNT_Buff[i]=0;
			gbFocusLED_Assist_Dir[i]=0;
		}
	}
	else {
		for(i=0; i<9; i++) giAF1_EGCNT_Buff[i+1]=giAF1_EGCNT_Buff[i];
		giAF1_EGCNT_Buff[0]=AF1_EGCNT_LOCKr;

		// AF Data Processing
		giAFEdge_Dc=((giAF1_EGCNT_Buff[0]-giAF1_EGCNT_Buff[3])+(giAF1_EGCNT_Buff[1]-giAF1_EGCNT_Buff[3])+(giAF1_EGCNT_Buff[2]-giAF1_EGCNT_Buff[3])
		            +(giAF1_EGCNT_Buff[3]-giAF1_EGCNT_Buff[4])+(giAF1_EGCNT_Buff[3]-giAF1_EGCNT_Buff[5])+(giAF1_EGCNT_Buff[3]-giAF1_EGCNT_Buff[6]));
		giAFEdge_De=(int)((float)(ABS(giAF1_EGCNT_Buff[0]-giAF1_EGCNT_Buff[1])+ABS(giAF1_EGCNT_Buff[1]-giAF1_EGCNT_Buff[2])+ABS(giAF1_EGCNT_Buff[2]-giAF1_EGCNT_Buff[3])
		            +ABS(giAF1_EGCNT_Buff[3]-giAF1_EGCNT_Buff[4])+ABS(giAF1_EGCNT_Buff[4]-giAF1_EGCNT_Buff[5])+ABS(giAF1_EGCNT_Buff[5]-giAF1_EGCNT_Buff[6]))*(float)AF_DAVG_RATE)+FA_DAVG_OFS;

		if(AFDATA_RESPONS) { // Response
			if(giAFEdge_Dc>0) gbFocusLED_Assist_Dir[0] = (gbFocusLED_Assist_Dir[0]<0) ? 0 : (gbFocusLED_Assist_Dir[0]>= AFECNT_MAX) ?  AFECNT_MAX : gbFocusLED_Assist_Dir[0]++;
			else 			  gbFocusLED_Assist_Dir[0] = (gbFocusLED_Assist_Dir[0]>0) ? 0 : (gbFocusLED_Assist_Dir[0]<=-AFECNT_MAX) ? -AFECNT_MAX : gbFocusLED_Assist_Dir[0]--;
		}
		else gbFocusLED_Assist_Dir[0]=0;	// No response
			
		if(!AFDATA_RESPONS) gbFocusLED_Assist_Dir[0]=0;	// No response
		else if(AFDATA_INCREASE) {						// Data Increase
			if     (gbFocusLED_Assist_Dir[1]< 0) gbFocusLED_Assist_Dir[0]=0;
			else if(gbFocusLED_Assist_Dir[0]< 3) gbFocusLED_Assist_Dir[0]++;
		}
		else {											// Data Decrease
			if     (gbFocusLED_Assist_Dir[1]> 0) gbFocusLED_Assist_Dir[0]=0;
			else if(gbFocusLED_Assist_Dir[0]>-3) gbFocusLED_Assist_Dir[0]--;
		}
		for(i=0; i<9; i++) gbFocusLED_Assist_Dir[i+1]=gbFocusLED_Assist_Dir[i];
	}
}
#endif

void ISPM FA_DebugMode(void)
{
	if(gbMnDebugFnc==2) OZONE_SELw(2);
	
	int iSUM_AF1_Qtization = (int)((AF_YSUM1_LOCKr>>2)&0x07FFFFFF)+(giFA1_DetTPixel*10) ;

	int giFA1SUM1_Data = (int)(((float)(AF_SUM1_LOCKr)/(float)(iSUM_AF1_Qtization))*5000.f);
	
	DebugDisp((gbMnDebugFnc==2), Dec, "AF1SUM1:", (model_1M ?  3 : 23), 24, AF_SUM1_LOCKr);
	DebugDisp((gbMnDebugFnc==2), Dec, "AF1DATA:", (model_1M ?  4 : 24), 24, giFA1SUM1_Data);
	DebugDisp((gbMnDebugFnc==2), Dec, "AF1YSM1:", (model_1M ?  5 : 25), 24, AF_YSUM1_LOCKr);	
	DebugDisp((gbMnDebugFnc==2), Dec, "AF1CPCN:", (model_1M ?  6 : 26), 24, AF_CLCNT_LOCKr);
	
	#if 1	//	Graphic Display Out
	if(gbMnDebugFnc==2) {
		
		#if (EDGE_CNT_TEST)
			GRP0 = AF_SUM1_LOCKr;
			GRP1 = AF_SUM2_LOCKr;
			GRP2 = gbFocusLED_Assist_Dir[0];
			GRP3 = AFDATA_RESPONS;
			GRP4 = AFDATA_INCREASE;
			GRP5 = (UINT)ABS((giAF1_EGCNT_Buff[0]-giAF1_EGCNT_Buff[1]));
			GRP6 = AFDATA_THR;
			GRP7 = giAF1_EGCNT_Buff[1];
			
		#else
			GRP0 = AF_SUM1_LOCKr;
			GRP1 = AF_SUM2_LOCKr;
			GRP2 = AF_YSUM1_LOCKr;
			GRP3 = AF_YSUM2_LOCKr;
			GRP4 = AF_SLCNT_LOCKr;
			GRP5 = AF_CLCNT_LOCKr;
			GRP6 = AF_EGCNT1_LOCKr;
			GRP7 = AF_EGCNT2_LOCKr;
			
		#endif
		TxGrp();
	}
	#endif
}

#if (EDGE_CNT_TEST) // Start of Focus_Assist(void)

void ISPM Focus_Assist(void)
{
	int i=0,j=0;
	UINT nHs=0,nHe=0,nVs=0,nVe=0;

	if(gbWdrOn&&(LS_FLGr==AeLONG)) return; // WDR On & Short mode // 2014.09.29
	
	FA_DebugMode();

	if(MP(MpFad_On)) {

		if(MP(MpFad_Zone)==0) { // Narrow
			nHs = FAD_FR_HW/7*2; nHe = FAD_FR_HW/7*5;
			nVs = FAD_FR_VW/4*1; nVe = FAD_FR_VW/4*3;
		}
		else if(MP(MpFad_Zone)==1) { // Middle
			nHs = FAD_FR_HW/6*1; nHe = FAD_FR_HW/6*5;
			nVs = FAD_FR_VW/6*1; nVe = FAD_FR_VW/6*5;
		}
		else {
			nHs = 0x40; nHe = (FAD_FR_HW-0x44); // Ofs Adj
			nVs =    6; nVe = FAD_FR_VW;
		}
		AF_HSPw(nHs); AF_VSPw(nVs); AF_HEPw(nHe); AF_VEPw(nVe);

		if(gbFocusAssist_Init) { FocusAssist_Init(); return; }
		else {
			FocusADJ_DirCnt();
		}
	}
}

#else

#define FAD_LED_OUT	0
#if (FAD_LED_OUT==1)
extern int giFA1SUM2[10];
int giFA_TopBuff=0;
BYTE bFA_TopChgFlag=0;
#endif

void ISPM Focus_Assist(void)
{ 	// gnFA_Pos_Control_Y_Dly : Green	- AF Top data (after focus scan)
 	// gnFA_Pos_Control_Y     : Yellow  - Current AF data
	int i=0,j=0;
	UINT nHs=0,nHe=0,nVs=0,nVe=0;

#if (FAD_LED_OUT==1)
	static BYTE bFA_LedState=0;
	static UINT nLed_FCntTgt=0;
	extern UINT gnTm0Test_Cnt;
	UINT nLed_FCnt=(gnTm0Test_Cnt%nLed_FCntTgt); // gnTm0Test_Cnt (100ms Timer Coutner)
#endif

	if(gbWdrOn&&(LS_FLGr==AeLONG)) return; // WDR On & Short mode // 2014.09.29
	
	FA_DebugMode();

	if(gbFocusADJ_En!=MP(MpFad_On)) {
		
		if(MP(MpFad_On)) {
			for(i=0; i<FADJ_EA*2; i++) gnBoxPosFADJ[i]=0;
		}
		FA_EnvChange();
	}
	gbFocusADJ_En   = MP(MpFad_On);
	

	if(gbFad_SizY_Buf!=MP(MpFad_SizY)) {
		for(i=0; i<3; i++) gnFA_Pos_Control_Y_Dly[i]=0;
	}
	gbFad_SizY_Buf = MP(MpFad_SizY);

	if(MP(MpFad_On)) {

		if(MP(MpFad_Zone)==0) { // Narrow
			nHs = FAD_FR_HW/7*2; nHe = FAD_FR_HW/7*5;
			nVs = FAD_FR_VW/4*1; nVe = FAD_FR_VW/4*3;
		}
		else if(MP(MpFad_Zone)==1) { // Middle
			nHs = FAD_FR_HW/6*1; nHe = FAD_FR_HW/6*5;
			nVs = FAD_FR_VW/6*1; nVe = FAD_FR_VW/6*5;
		}
		else {
			nHs = 0x40; nHe = (FAD_FR_HW-0x44); // Ofs Adj
			nVs =    6; nVe = FAD_FR_VW;
		}
		AF_HSPw(nHs); AF_VSPw(nVs); AF_HEPw(nHe); AF_VEPw(nVe);

		giFA1_DetTPixel = (UINT)((nHe-nHs)*(nVe-nVs));

		gnFaPosY_Min=FA_Pos_SY_MIN;
		gnFaPosY_Max=FA_Pos_SY_MAX;

		gnFaDispScale[0] = (UINT)MAX(1,( 2*32/(int)MP(MpFad_SizY)));
		gnFaDispScale[1] = (UINT)MAX(1,(10*32/(int)MP(MpFad_SizY)));
		gnFaDispScale[2] = (UINT)MAX(1,(50*32/(int)MP(MpFad_SizY)));


		gnBoxToneFADJ[0] = gnBoxBToneFADJ[MP(MpFad_Trans)][0];
		gnBoxToneFADJ[1] = gnBoxBToneFADJ[MP(MpFad_Trans)][1];

		if(MENU_TITLE_IS(FOCUS_ADJ)&MENU_NAME_IS(FOCUS_AREA)) {
			gbOzone_Hold=1;
			OZONE_SELw(2);
		}
		else gbOzone_Hold=0;

		if(gbFocusAssist_Init) { FocusAssist_Init(); return; }
		else {
			FocusADJ_Data_Processing();

			for(i=0; i<3; i++) {
				for(j=7; j<10; j++) if((giFA_DirCnt[j]==3)&&(giFA_DirCnt[0]==-3)) {
					gnFA_Pos_Control_Y_Dly[i] = gnFA_Pos_Control_Y_Buf[i][j];
#if (FAD_LED_OUT==1)
					giFA_TopBuff=giFA1SUM2[9-(BYTE)j];
					bFA_TopChgFlag=1;
#endif
				}

				gnBoxPosFADJ[(i*2)]   = (FA_Pos_SY_BASE|(FA_Pos_SY_MIN+gnFA_Pos_Control_Y[i]    ));
				gnBoxPosFADJ[(i*2+6)] = (FA_Pos_SY_BASE|(FA_Pos_SY_MIN+gnFA_Pos_Control_Y_Dly[i]));
			}
		}
		giFA1_SUM2_LOCKr  = AF_SUM1_LOCKr;
		giFA1_YSUM2_LOCKr = AF_YSUM1_LOCKr;

		gnBoxPosFADJ[1]  = ((FA_B00_HSP<<16)|(FA_B00_HSP+FA_B00_HW)); gnBoxPosFADJ[3]  = ((FA_B01_HSP<<16)|(FA_B01_HSP+FA_B01_HW));
		gnBoxPosFADJ[5]  = ((FA_B02_HSP<<16)|(FA_B02_HSP+FA_B02_HW)); gnBoxPosFADJ[7]  = ((FA_B03_HSP<<16)|(FA_B03_HSP+FA_B03_HW));
		gnBoxPosFADJ[9]  = ((FA_B04_HSP<<16)|(FA_B04_HSP+FA_B04_HW)); gnBoxPosFADJ[11] = ((FA_B05_HSP<<16)|(FA_B05_HSP+FA_B05_HW));

		gnBoxPosFADJ[12] = ((FA_B06_VSP<<16)|(FA_B06_VSP+FA_B06_VW)); gnBoxPosFADJ[12+1] = ((FA_B06_HSP<<16)|(FA_B06_HSP+FA_B06_HW));
		gnBoxPosFADJ[14] = ((FA_B07_VSP<<16)|(FA_B07_VSP+FA_B07_VW)); gnBoxPosFADJ[14+1] = ((FA_B07_HSP<<16)|(FA_B07_HSP+FA_B07_HW));
		gnBoxPosFADJ[16] = ((FA_B08_VSP<<16)|(FA_B08_VSP+FA_B08_VW)); gnBoxPosFADJ[16+1] = ((FA_B08_HSP<<16)|(FA_B08_HSP+FA_B08_HW));
		gnBoxPosFADJ[18] = ((FA_B09_VSP<<16)|(FA_B09_VSP+FA_B09_VW)); gnBoxPosFADJ[18+1] = ((FA_B09_HSP<<16)|(FA_B09_HSP+FA_B09_HW));
		gnBoxPosFADJ[20] = ((FA_B10_VSP<<16)|(FA_B10_VSP+FA_B10_VW)); gnBoxPosFADJ[20+1] = ((FA_B10_HSP<<16)|(FA_B10_HSP+FA_B10_HW));
		gnBoxPosFADJ[22] = ((FA_B11_VSP<<16)|(FA_B11_VSP+FA_B11_VW)); gnBoxPosFADJ[22+1] = ((FA_B11_HSP<<16)|(FA_B11_HSP+FA_B11_HW));
		gnBoxPosFADJ[24] = ((FA_B12_VSP<<16)|(FA_B12_VSP+FA_B12_VW)); gnBoxPosFADJ[24+1] = ((FA_B12_HSP<<16)|(FA_B12_HSP+FA_B12_HW));
		gnBoxPosFADJ[26] = ((FA_B13_VSP<<16)|(FA_B13_VSP+FA_B13_VW)); gnBoxPosFADJ[26+1] = ((FA_B13_HSP<<16)|(FA_B13_HSP+FA_B13_HW));
		gnBoxPosFADJ[28] = ((FA_B14_VSP<<16)|(FA_B14_VSP+FA_B14_VW)); gnBoxPosFADJ[28+1] = ((FA_B14_HSP<<16)|(FA_B14_HSP+FA_B14_HW));
		gnBoxPosFADJ[30] = ((FA_B15_VSP<<16)|(FA_B15_VSP+FA_B15_VW)); gnBoxPosFADJ[30+1] = ((FA_B15_HSP<<16)|(FA_B15_HSP+FA_B15_HW));
		gnBoxPosFADJ[32] = ((FA_B16_VSP<<16)|(FA_B16_VSP+FA_B16_VW)); gnBoxPosFADJ[32+1] = ((FA_B16_HSP<<16)|(FA_B16_HSP+FA_B16_HW));
		gnBoxPosFADJ[34] = ((FA_B17_VSP<<16)|(FA_B17_VSP+FA_B17_VW)); gnBoxPosFADJ[34+1] = ((FA_B17_HSP<<16)|(FA_B17_HSP+FA_B17_HW));
		gnBoxPosFADJ[36] = ((FA_B18_VSP<<16)|(FA_B18_VSP+FA_B18_VW)); gnBoxPosFADJ[36+1] = ((FA_B18_HSP<<16)|(FA_B18_HSP+FA_B18_HW));

		for(i=0; i<19; i++) gnBoxPosFADJ[i*2] = ((FR_VW-((gnBoxPosFADJ[i*2]>>16)&0xFFF))|((FR_VW-(gnBoxPosFADJ[i*2]&0xFFF))<<16));


		#if (FAD_LED_OUT==1)

			if(bFA_TopChgFlag) { bFA_TopChgFlag=0; bFA_LedState = 1; }

			if(bFA_LedState!=0) {
				if     (giFA1SUM2[9]>=(giFA_TopBuff*97/100)) bFA_LedState = 5;
				else if(giFA1SUM2[9]>=(giFA_TopBuff*85/100)) bFA_LedState = 4;
				else if(giFA1SUM2[9]>=(giFA_TopBuff*70/100)) bFA_LedState = 3;
				else if(giFA1SUM2[9]>=(giFA_TopBuff*50/100)) bFA_LedState = 2;
				else                                         bFA_LedState = 1;
			}

			switch (bFA_LedState) {
				case 0 : nLed_FCntTgt=20; break; // StandBy
//					case 1 : nLed_FCntTgt=16; break; // Scanning Complete (Over Collection)
				case 2 : nLed_FCntTgt=10; break; // Near Top
				case 3 : nLed_FCntTgt= 6; break; // Near Top
				case 4 : nLed_FCntTgt= 2; break; // Near Top
				case 5 : nLed_FCntTgt= 0; break; // Top
				default : break;
			}
#ifdef GPIO_FOCUS_LED
			if(bFA_LedState==1) GpioSetLo(GPIO_FOCUS_LED);
			else {
				if(nLed_FCnt<(nLed_FCntTgt>>1)) GpioSetLo(GPIO_FOCUS_LED);
				else                            GpioSetHi(GPIO_FOCUS_LED);
			}
#endif
//				bFA_LedState=(BYTE)PAR00; // for LED Debuging
			
			DebugDisp((gbMnDebugFnc==2), Dec, "LedFCnt:", (model_1M ? 18 : 38), 24, nLed_FCnt);
			DebugDisp((gbMnDebugFnc==2), Dec, "FCntTgt:", (model_1M ? 19 : 39), 24, nLed_FCntTgt);
			DebugDisp((gbMnDebugFnc==2), Dec, "L_State:", (model_1M ? 20 : 40), 24, bFA_LedState);
			DebugDisp((gbMnDebugFnc==2), Dec, "CmpRate:", (model_1M ? 22 : 42), 24, (giFA1SUM2[9]*100/giFA_TopBuff));
		#endif
	}
	else {
		#if (FAD_LED_OUT==1)
			bFA_LedState = 0;
		#endif
	}
}
#endif // End of Focus_Assist(void)

#endif

/* EOF */
