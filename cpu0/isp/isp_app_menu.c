/* **************************************************************************
 File Name	: 	menu.c
 Description:	EN673 - Menu function
 Designer	:	Kim, Sunghoon
 Date		:	14. 6. 17
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "isp_app_menu.h"
#include "isp_app_menu_tbl.h"
#include "isp_app_menu_style.h"


//*************************************************************************************************
// Global Menu Parameter & variables
//-------------------------------------------------------------------------------------------------
	BYTE gbMnParSaveChk = 0;
	BYTE gbMnParReadChk = 0;
//	BYTE gbMnParTbl[MN_PAR_EA], 
	BYTE gbMnParTblOld[MN_PAR_EA];	// Storage for MENU Parameter

	BYTE gbMenuStyle = 0;

	BYTE gbMnSetupOn = 0;

	BYTE gbMnBlcOsdOn = 0;

	BYTE gbBaudrate = 0;
	BYTE gbBaudrateSetDone = 0;

#if model_TestOSD == 0
	BYTE gbMnDebugFnc = 0;
	//BYTE gbMnDebugFnc = 1;
#else
  #if IRIS_DBG
	BYTE gbMnDebugFnc = 1;
  #else
	BYTE gbMnDebugFnc = 8;
  #endif
#endif

	BYTE gbMnDebugBypass = 0;
	BYTE gbMenuState = 0;
	BYTE gbMenuPushState = 0;

	BYTE gbMnUpReset = 0;

	BYTE gbMnParID = 0;
	BYTE gbMnParByte = 3;

#if model_ExitMenu == 1 || model_ExitMenu == 2 || model_ExitMenu == 3
	BYTE gbMnExit = 0;
#endif

#if(model_Lens==0)

#elif(model_Lens==1)

#else
  #if (model_Iris==4)
	BYTE gbMnIrisCal = 0;
  #endif
	BYTE gbMnPresetAct = 0;
	BYTE gbMnPresetSave = 0;
	BYTE gbMnZoomDirSel = 0;
	BYTE gbMnFocusDirSel = 0;
	BYTE gbMnInitialState = 0;
	BYTE gbMnPuntCorrection = 0;
#endif

#if(model_Iris_P)	//141124 HSH Main
	extern UINT gnAEIrisStep, gnAEIrisMin_Def,gnAEIrisMax_Def,gnAEIrisMin_Pos,gnAEIrisMax_Pos, gnAEIrisMin,gnAEIrisMax;
	BYTE gbMnIrisFlag = 0;
	BYTE gbMnIrisSet = 0;
#endif

#if 1//model_WDR_ROI
	int giKeyAdd = 0;
#endif

extern void Hue_Chroma_Menu_Set(int MenuState);

//*************************************************************************************************
// variables
//-------------------------------------------------------------------------------------------------
BYTE    gbStgPos[5]     = {MN_IV_POS,MN_IV_POS,MN_IV_POS,MN_IV_POS,MN_IV_POS};                // Index = 0~3 : Stage Level,      Value = 0~8 : Position,  other : invalid
BYTE	gbStgSubPos[5]	= {0,0,0,0,0};									// special case stage
int     giLV            = -1;											// menu step level

BYTE 	gbPushCnt		= 0;

#define gbMenuY	  MN_YSTART
//BYTE	gbMenuY	= MN_YSTART;
UINT	DRAW_Y	= MN_YSTART;

BYTE 	gbIMDMenuStep	= 0;

#if model_WDR_ROI
	BYTE	gbROIWdrBoxFlag = 0;
#endif

BYTE	MN_MXSP = MN_MXSP_DEF;

//*************************************************************************************************
// Fixed define Functions
//-------------------------------------------------------------------------------------------------
//#define OsdAttrsStgPos(Y, ATTR)         			SetFontAttrs(gbMenuY+Y, MN_MXSP, MN_LINESIZE, ATTR)
#define OsdAttrsStgPosAr(Y, ATTR)					SetFontAttrs(gbMenuY+Y, MN_MXSP/*-2*/, MN_LINESIZE/*+2*/, ATTR)		// 160330 KSH

//*************************************************************************************************
// Sub Functions
//-------------------------------------------------------------------------------------------------
void DispHex(const UINT anDispY, const UINT anDispX, const UINT anVal, const UINT anLen)
{
	uint2strh(gbStr, anVal, anLen);
	DispStr((const char*)gbStr, anDispY, anDispX, anLen);
}

void DispDec(const UINT anDispY, const UINT anDispX, const int aiVal, const UINT anLen)
{
	int2str(gbStr, aiVal, anLen);
	DispStr((const char*)gbStr, anDispY, anDispX, anLen);
}

void DispBin(const UINT anDispY, const UINT anDispX, const UINT anVal, const UINT anLen)
{
	uchar2strb(gbStr, anVal, anLen);
	DispStr((const char*)gbStr, anDispY, anDispX, anLen);
}

void DispPer(const UINT anDispY, const UINT anDispX, const int aiVal, const int aiValMax, const UINT anLen)
{
	per2str(gbStr, aiVal, aiValMax, anLen);
	DispStr((const char*)gbStr, anDispY, anDispX, anLen);
}

void DispDatHex(char* cStr, const UINT anDispY, const UINT anDispX, const UINT anVal) //noclear
{	// Value Monitoring tool (limit to MN_FONT_MAX_PATH)
	DispStr(cStr, anDispY, anDispX, MN_FONT_MAX_PATH);
	DispHex(anDispY, ((anDispX+MN_FONT_MAX_PATH)+MN_FONT_SPACE+(MN_FONT_MAX_PATH-8)), anVal, 8);
}

void DispDatDec(char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal) //noclear
{	// Value Monitoring tool
	DispStr(cStr, anDispY, anDispX, MN_FONT_MAX_PATH);
	DispDec(anDispY, ((anDispX+MN_FONT_MAX_PATH)+MN_FONT_SPACE), aiVal, MN_FONT_MAX_PATH);
}

void DispDatPer(char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal, const int aiValMax) //noclear
{	// Value Monitoring tool
	DispStr(cStr, anDispY, anDispX, MN_FONT_MAX_PATH);
	DispPer(anDispY, ((anDispX+MN_FONT_MAX_PATH)+MN_FONT_SPACE), aiVal, aiValMax, MN_FONT_MAX_PATH);
}

void DispDatDec2(const BYTE abON, char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal, const UINT anLen) //noclear
{	// Value Monitoring tool
	if(abON) {
		const UINT nX = DispStr(cStr, anDispY, anDispX, MN_FONT_MAX_PATH);
		DispDec(anDispY, ((anDispX+nX/*MN_FONT_MAX_PATH*/)+MN_FONT_SPACE), aiVal, anLen/*MN_FONT_MAX_PATH*/);
	}
}

void DispClrDec(UINT anVal, UINT anPosY, UINT anPosX, UINT anLen)
{
    UINT nDiv;
    UINT nPos = 0;
	BYTE gbVals[3+1];

    if(anVal>999) return;

    nDiv = anVal/100;
    anVal -= nDiv*100;
    if(nDiv) gbVals[nPos++] = '0' + nDiv;

    nDiv = anVal/10;
    anVal -= nDiv*10;
    if(nDiv || nPos) gbVals[nPos++] = '0' + nDiv;

    gbVals[nPos++] = '0' + anVal;
	gbVals[nPos] = 0;

	if(anLen) DispClrStr((const char*)gbVals, anPosY, anPosX, nPos, anLen);
	else	  DispStr((const char*)gbVals, anPosY, anPosX, nPos);
}

//-------------------------------------------------------------------------------------------------
void OsdClearAll(void)
{	// init ID
    UINT i;


#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
	ISPCLK_SEL(PPCK,1); // for ABT
	ISPCLK_SEL(FN_CK,1);
#endif

	for(i=0; i<ISP_FONT_ID_EA; i++){									// Initial Font ID
		if (i==(ISP_FONT_ID_EA-1))			{SetFontId(i, 0x1ff);}			// Page returnID
		else if (((i+1)%ISP_FONT_EAX) == 0)	{SetFontId(i, 0x1fe);}			// Line returnID
		else 								{SetFontId(i, SPACE_CHAR_ID);}	// fill space
	}
	
#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
	ISPCLK_SEL(FN_CK,5);
#endif
}

//-------------------------------------------------------------------------------------------------
void OsdWhiteAll(void)
{
    UINT i;
    for(i=0; i<ISP_FONT_ATTR_EA; i++) SetFontAttr(i, MN_WHITE);
}

//-------------------------------------------------------------------------------------------------
void OsdDispBar(UINT anVal, UINT anPosY, UINT anMin, UINT anMax)
{
	if(anVal > anMin) anVal -= anMin;
	else anVal = 0;

	//if(anMax > anMin) anMax -= anMin;
	//else anMax = anMin;
	anMax -= anMin;                                        // !! Must be 'anMax > anMin'

    UINT Pos1;
	UINT Pos2;

     if(anVal >= anMax) {
		Pos1 = (MN_BAR_SIZE-1);                                       // Set Max Position
		Pos2 = 2;
     }
    else {
		anMax <<= 5;                   // x32 : 32 > 7x3  -->  Increase Resolution
		anVal <<= 5;

		const UINT nDiv1 = (anMax / MN_BAR_SIZE) + 1;
		const UINT nDiv2 = ((nDiv1 - 1) / 3) + 1;

		Pos1 = anVal / nDiv1;
		Pos2 = (anVal % nDiv1) / nDiv2;
    }

    UINT Typ = 0;

	switch(Pos2) {
	case 0 : Typ = (Pos1==(MN_BAR_SIZE-1)) ? _IOI : _IOO; break;
	case 1 : Typ = (Pos1==(MN_BAR_SIZE-1)) ? _OII : (Pos1==0) ? _IIO : _OIO; break;
	case 2 : Typ = (Pos1==0) ? _IOI : _OOI; break;
	}

    UINT i;

	SETFONTID(anPosY,MN_XBAR,_IOO);
	SETFONTID(anPosY,MN_XBAR+(MN_BAR_SIZE-1),_OOI);
	for(i=1; i<(MN_BAR_SIZE-1); i++) SETFONTID(anPosY,MN_XBAR+i,_OOO);
	SETFONTID(anPosY,MN_XBAR+Pos1,Typ);

	for(i=(MN_XBAR+MN_BAR_SIZE); i<(UINT)(MN_SXSP+MN_SUBSIZE); i++) SETFONTID(anPosY,i,' ');
}

void OsdCamTitle(void)
{
	DispClr(0, ISP_FONT_EAX-9, 0x8);	SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_WHITE);
	DispClr(19, 0, 0x8);				SetFontAttrs(19, 0, 0x8, MN_WHITE);
	DispClr(17, 0, 0x8);				SetFontAttrs(17, 0, 0x8, MN_WHITE);

	//if(gbMnDebugFnc == 0) {
		switch(MP(MpCamTitleOn)) {
			case MN_CT_RIGHT_UP  :	DispStr((const char*)&gbCamTitle(0), 0, ISP_FONT_EAX-9, 0x8);
									SetFontAttrs(0, ISP_FONT_EAX-9, 0x8, MN_BLACK);
									break;

			case MN_CT_LEFT_DOWN : 
			  #if (model_2M30p)
				if (MP(MpOutFps)!=MN_2M_1080p_25_30) { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }
				else								 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }

			  #elif (model_2M)
				if ((MP(MpOutFps)==MN_2M_1080p_25_30)||(MP(MpOutFps)==MN_2M_1080p_50_60))
					                                 { DispStr((const char*)&gbCamTitle(0), 17, 0, 0x8); SetFontAttrs(17, 0, 0x8, MN_BLACK); }
				else 								 { DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8); SetFontAttrs(19, 0, 0x8, MN_BLACK); }

			  #else
				DispStr((const char*)&gbCamTitle(0), 19, 0, 0x8);
				SetFontAttrs(19, 0, 0x8, MN_BLACK);
				
			  #endif
				break;

			default :	break;
		}
	//}
}


//*************************************************************************************************
// Add Functions
//-------------------------------------------------------------------------------------------------
int  giMenuDispPos	= 0;
int  giMenuDispChg	= 0;
int  giGrayOnly		= 0;
int  giMenuNum		= 0;

PEXCH *gbMenuTitle	= (PEXCH*)_S(OFF);
PEXCH *gbMenuList	= 0;
PEXCH *gbMenuVal	= 0;

PEXCH *gbStgTitle[4] = {(PEXCH*)_S(OFF),};

#if model_CharMax == 1
UINT ISPM strlenEx(const EXCH* apbStr)
{	// String length
	UINT	nLen = 0;
    while(*apbStr++) nLen++;
	return nLen;
}
#endif

void menu_redraw(int iChg, int iGrayOnly)
{
	//OsdClearAll();
	if(giLV < 0) return;

	giMenuDispPos = gbStgPos[giLV] + 1;
	giMenuDispChg = iChg;
	giGrayOnly = iGrayOnly;
	gbStgPos[giLV] = 0;
}

void menu_in(const PEXCH* Title)
{
	gbMenuTitle = (PEXCH*)Title;

	if(giLV >= 0) {		// 이전 메뉴가 있는 경우 (이전 메뉴가 MENU_EXT형식이면 안됨!!!)
		OsdMenuPos(0);
		//OsdClearAll();
	}
	else {				// 이전 메뉴가 없는 경우
		OSD_ON();
		ChangeMenuSize();
	}

	giLV++;
	gbStgPos[giLV] = 0;
	gbStgTitle[giLV] = gbMenuTitle;

	//OsdMenuPos(1)
	giMenuDispPos = gbStgPos[giLV] + 1;
	giMenuDispChg = 1;
	giGrayOnly = 0;
}

void menu_close(void)
{ //메뉴 닫기
	giLV = -1;
	gbMenuTitle = (PEXCH*)_S(OFF);

	gbMenuStyle = 0;

	//gbMnSetupOn = 0;

	int i;

	if(giMenuNum == 0) giMenuNum = 16;	// 16 = 메뉴 항목 최대 가능 수

	// TxStr("giMenuNum ", giMenuNum, 2);

	DispClr(MN_TITLE, MN_MXSP, MN_LINESIZE);
	for(i=0; i<giMenuNum; i++)
	{
		DispClr(gbMenuY+i, MN_MXSP, MN_LINESIZE);
	}
	giMenuNum = 0;
	giMenuDispPos = 1;
	gbMenuVal = 0;
	gbMenuList = 0;

	OsdClearAll();
	OsdWhiteAll();

	OsdCamTitle();

	OSD_OFF();
	ChangeMenuSize();
}

void menu_out(const int aiClearFinger/*const PEXCH* Title*/)
{ //이전단계 메뉴
	//gbMenuTitle = (PEXCH*)Title;

	if(aiClearFinger) { OsdMenuPos(0); }
	//OsdClearAll();

	giLV--;

	//if(MENU_TITLE_IS(OFF)) giLV = -1;
	//if(((const PEXCH*)Title) == _S(OFF)) { giLV = -1; gbMenuTitle = (PEXCH*)_S(OFF); }

	if(giLV >= 0) {		// 현재 메뉴가 있는 경우
		gbMenuTitle = gbStgTitle[giLV];

		//OsdMenuPos(1);
		giMenuDispPos = gbStgPos[giLV] + 1;
		giMenuDispChg = 1;
		giGrayOnly = 0;
		gbStgPos[giLV] = 0;
	}
	else
	{				// 현재 메뉴가 없는 경우
		menu_close();
	}
}

void menu_dir(const int aiOn)
{
	if(!aiOn) {
		OsdAttrsStgPosAr(gbStgPos[giLV], MN_GRAY);
		gbMenuVal = (PEXCH*)_S(NOT_USED);
	}

	if(giMenuDispPos) {
		if(aiOn) {
			if(giMenuDispPos != gbStgPos[giLV] + 1) OsdAttrsStgPosAr(gbStgPos[giLV], MN_WHITE);
		}
		else {
			if(giMenuDispPos == gbStgPos[giLV] + 1) giMenuDispPos++;
			if(giGrayOnly == 0) DISPCLRSTR(_S(NOT_USED), gbMenuY+gbStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
		}
		gbStgPos[giLV]++;
	}
}

void menu_one(const int aiOn, const PEXCH* Str)
{
	gbMenuVal = (PEXCH*)Str;

	if(giMenuDispPos && (aiOn || giGrayOnly)) {
		DISPCLRSTR(Str, gbMenuY+gbStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	}

	menu_dir(aiOn);
}

BYTE menu_push(const int aiOn, volatile BYTE *Val, const PEXCH* StrNor, const PEXCH* StrPush, const BYTE abDelay)
{
	BYTE bPushOn = 0;

	if(giMenuDispPos && (aiOn || giGrayOnly)) {
		gbPushCnt = 0;
		*Val = 0;
		goto DISPLAY_PUSH_STR;
	}

	if(giMenuDispPos == 0) {
		gbMenuPushState = 1;
		if(pKEY_C) {
			if(gbPushCnt < 0xFF) gbPushCnt++;
			if(gbPushCnt == abDelay) {
				bPushOn = 1;
				*Val = 1;
				goto DISPLAY_PUSH_STR;
			}
		}
		else {
			if(gbPushCnt) {
				gbPushCnt = 0;
				*Val = 0;
				goto DISPLAY_PUSH_STR;
			}
		}
	}

	gbMenuVal = (PEXCH*)((*Val) ? StrPush : StrNor);
	menu_dir(aiOn);
	return 0;

DISPLAY_PUSH_STR:
	gbMenuVal = (PEXCH*)((*Val) ? StrPush : StrNor);
	DISPCLRSTR(gbMenuVal, gbMenuY+gbStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	menu_dir(aiOn);
	return bPushOn;
}

void menu_str(const int aiOn, volatile BYTE *Val, int OpNum, const PEXCH** Str)
{
	//if(giMenuDispPos && aiOn) {
	//	if(*Val >= OpNum) *Val = 0;
	//	const BYTE bOld = *Val;
	//	while(1) {
	//		gbMenuVal = (PEXCH*)Str[*Val];
	//		if(gbMenuVal == _S()) {
	//			*Val = *Val + 1;
	//			if(*Val >= OpNum) *Val = 0;
	//			if(*Val == bOld) {
	//				//*Val = 0;
	//				gbMenuVal = (PEXCH*)Str[*Val];
	//				goto DISPLAY_STR;
	//			}
	//		}
	//		else break;
	//	}
	//}

	if(giMenuDispPos && aiOn) {		// 170530 KSH
		if(*Val >= OpNum) *Val = 0;
		const BYTE bOld = *Val;
		gbMenuVal = (PEXCH*)Str[*Val];

		while(gbMenuVal == _S()) {
			*Val = *Val + 1;
			if(*Val >= OpNum) *Val = 0;
			gbMenuVal = (PEXCH*)Str[*Val];
			if(*Val == bOld) goto DISPLAY_STR;
		}
	}

	do {
		if(giMenuDispPos == 0 && aiOn) {
			if(KEY_L)	*Val = (*Val == 0) ? OpNum - 1 : *Val - 1;
			if(KEY_R)	*Val = (*Val >= OpNum - 1) ? 0 : *Val + 1;
		}
		gbMenuVal = (PEXCH*)Str[*Val];
	} while(gbMenuVal == _S());

DISPLAY_STR:
	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		DISPCLRSTR(Str[*Val], gbMenuY+gbStgPos[giLV], MN_SXSP, MN_SUBSIZE, MN_SUBSIZE);
	}

	menu_dir(aiOn);
}

void menu_val(volatile BYTE *Val, UINT anMin, UINT anMax, UINT anRotation)
{
	if(giMenuDispPos == 0) {
		UINT nA, nB;

		if(anRotation) {
			nA = anMax;
			nB = anMin;
		}
		else {
			nA = anMin;
			nB = anMax;
		}

		if(anMax > (anMin+20)) {
			KeyAccLevel(&giKeyAdd);
			if(KEY_L) *Val = (((int)(*Val) - giKeyAdd) <= (int)anMin) ? (BYTE)nA : *Val - giKeyAdd;
			if(KEY_R) *Val = (((int)(*Val) + giKeyAdd) >= (int)anMax) ? (BYTE)nB : *Val + giKeyAdd;
		}
		else {
			if(KEY_L) *Val = (*Val <= anMin) ? (BYTE)nA : *Val - 1;
			if(KEY_R) *Val = (*Val >= anMax) ? (BYTE)nB : *Val + 1;
		}
	}
	else {
		if(*Val < anMin) *Val = anMin;
		if(*Val > anMax) *Val = anMax;
	}
}

void menu_bar(const int aiOn, volatile BYTE *Val, const UINT anMin, const UINT anMax, const UINT anRotation, const PEXCH *Unit)
{
	if(aiOn) menu_val(Val, anMin, anMax, anRotation);

	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		const UINT nPosY = gbMenuY+gbStgPos[giLV];
    	DispClrDec(*Val, nPosY, MN_SXSP, 3);
		if(Unit) {
			DISPCLRSTR(Unit, nPosY, MN_SXSP+3, MN_SUBSIZE-3, MN_SUBSIZE-3);
		}
		else {
			OsdDispBar(*Val, nPosY, anMin, anMax);
		}
	}

	//if(gbMenuList) TxStr((BYTE*)MENU_NAME, *Val, "\n");

	gbMenuVal = 0;

	menu_dir(aiOn);
}

#if model_WDR_ROI
void menu_val_ex(const int aiOn, BYTE *apbValL, BYTE *apbValH, const WORD awMin, const WORD awMax)
{
	int iVal = ((*apbValH)<<8) | (*apbValL);

	if(giMenuDispPos == 0 && aiOn)
	{
		KeyAccLevel(&giKeyAdd);

		if(KEY_L) {
			const int iValTmp = iVal - giKeyAdd;
			iVal = MAX(awMin, iValTmp);
		}
		if(KEY_R) {
			const int iValTmp = iVal + giKeyAdd;
			iVal = MIN(awMax, iValTmp);
		}

		*apbValH = (iVal>>8)&0xff;
		*apbValL = iVal&0xff;

		gbMenuVal = 0;
	}

	if((giMenuDispPos || KEY_R || KEY_L) && (aiOn || giGrayOnly)) {
		int2str(gbStr, iVal, (iVal < 10) ? 1 : (iVal < 100) ? 2 : (iVal < 1000) ? 3 : 4);
		DispClrStr(gbStr, DRAW_Y, MN_SXSP, 4, MN_SUBSIZE);
	}
	
	menu_dir(aiOn);
}
#endif

void menu_pos(const PEXCH* Title, int MenuNum, const PEXCH** Str)
{
	if(giMenuDispChg)
	{
		if(MenuNum) giMenuDispChg = 0;

		int i;

		STRLEN(i, Title);
		//DispClr(MN_TITLE, h760(0x8,MN_MXSP), LINESIZE - h760(0x8,MN_MXSP) - 3);
		DispClr(MN_TITLE, MN_MXSP, MN_LINESIZE);
		DISPSTR(Title, MN_TITLE, MN_XCENT - (i>>1) - (giLV==1), i);		// 160330 KSH

		//gbMenuY = MN_YSTART;

		for(i=0; i<MenuNum; i++)
		{
			DISPCLRSTR(Str[i], gbMenuY+i, MN_MXSP, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/);
		}

		for(; i<giMenuNum; i++)
		{
			DispClr(gbMenuY+i, MN_MXSP, MN_LINESIZE);
		}
		giMenuNum = MenuNum;

		extern BYTE gbIMDFontY, gbIMDFontX;
		if(MP(MpItl)==MN_ON && MP(MpItlAlarm)==MN_ON && (gbMenuY+giMenuNum)<=gbIMDFontY) DispClr(gbIMDFontY, gbIMDFontX, 18);	// 170725 KSH
	}
	else if(!giMenuDispPos){		// 13.11.8 LJH 동시 키 오류  방지
		if((KEY_U || KEY_D) && MenuNum) {
			if((const PEXCH*)gbMenuVal != _S(NOT_USED)) OsdMenuPos(0);	// !! if문은 MN_WHITE에만 영향, finger clear는 항상 적용

			if(KEY_U)	gbStgPos[giLV] = (gbStgPos[giLV] == 0) ? MenuNum - 1 : gbStgPos[giLV] - 1;
			if(KEY_D)	gbStgPos[giLV] = (gbStgPos[giLV] == MenuNum - 1) ? 0 : gbStgPos[giLV] + 1;

			//OsdMenuPos(1);
		}
	}

	DRAW_Y = gbMenuY+gbStgPos[giLV];

	if(MenuNum && Str) gbMenuList = (PEXCH*)Str[gbStgPos[giLV]];
}

void MenuResetFnc(void)	// 실행위치 : MenuSave()의 복귀, InitMenu(), MenuReset()
{
	FrameRateLvSet();								// Set MP(MpFrameRateLv), 'MP(FrameMode) & MP(MpWdrWgt) & MP(MpAdnr)' must be set before it !!!
	
	OsdCamTitle();									// MenuSave()의 복귀에서 실행되지 않아도 menu_close()에서 실행됨

	AntiSatStabilizingSet();						// 메뉴값(MP(MpErrChtMax),MP(MpErrChtMin))을 설정하므로 InitMenu()에서 실행되지 않아도 됨

	ShutterMenuGet();								// AeAdv()에서 사용

	IspAgcSet();

	// OSD MENU에서 ParSet(...) 설정하는 경우 이 곳에서도 해야 함
	ParSet(UpAWB_SPEED_MTRX, MP(MpAwbSpeed));		// Awb()에서 사용
	ParSet(UpAWB_AGC_WGT_MIN, MP(MpAwbAgcWgtMin));	//		"
	ParSet(UpLSUP_ON, MP(MpLSUP_ON));				//		"
	ParSet(UpAWB_YSLICE, MP(MpAwbYSliceMax));		//		"

	ParSet(UpAE_WDR_LTGT_OFST2, MP(MpWdrABrt));		// Ae()에서 사용
	ParSet(UpAE_WDR_STGT_OFST2, MP(MpWdrABrtS));	//		"
	ParSet(UpACEWDR1_TH, MP(MpWdrDBrt));			// AceDefog()에서 사용
	ParSet(UpACEWDR2_TH, MP(MpWdrDCont));			//		"
	Hue_Chroma_Menu_Set(0);
	//printf("hihi\r\n");
	
}

void MenuStyleSet(const BYTE abMenuStyle)
{
	MENU_STYLE_SET
	
	MenuResetFnc();
}

void MenuSave(UINT anSaveOn)
{
	int i;
//	UINT bParSaveChk=0;

 	if (!anSaveOn) {										// No save & Exit
		//for(i=0; i<MN_PAR_EA; i++) gbMnParTblOld[i] = gbMnParTbl[i];		// 아무것도 안함	// 15.08.24 ksh
		for(i=0; i<MN_PAR_EA; i++) gbMnParTbl[i] = gbMnParTblOld[i];		// 복귀				// 15.08.24 ksh
		SetFontChg();
		MenuResetFnc();
 	}
	else {													// Save
		MP(MP_START) = (BYTE)((MN_DATA_ID>>8)&0xff);
		MP(MP_END)	 = (BYTE)((MN_DATA_ID)&0xff);

		for(i=0; i<MN_PAR_EA; i++) {
			if(gbMnParTblOld[i] != gbMnParTbl[i])
			{
				gbMnParTblOld[i] = gbMnParTbl[i];
//				bParSaveChk = 1;
				gbMnParSaveChk = 1;		// EN778
			}
		}
	}

	//TxStr("gbMnParSaveChk ", gbMnParSaveChk, 2);
}

void ChangeMenuSize(void)
{
	if(gbMnDebugFnc == 0) {
		MN_MXSP = MN_MXSP_DEF;
	#if (model_1M)
		MN_POS_SIZ_720P
		
	#elif(model_2M30p)
		if (MP(MpOutFps)!=MN_2M_1080p_25_30) 	MN_POS_SIZ_720P_CROP
		else									MN_POS_SIZ_LARGE
		
	#elif(model_2M)
		//if      (MP(MpOutFps)==MN_2M_1080p_25_30) 	MN_POS_SIZ_LARGE
		//else if (MP(MpOutFps)==MN_2M_1080p_50_60) 	MN_POS_SIZ_LARGE
		//else										MN_POS_SIZ_720P_CROP
		if(MENU_TITLE_IS(OFF))	MN_POS_SIZ_720P_CROP
		else					MN_POS_SIZ_LARGE
	#else
	
	#endif
	}
	else {
		MN_MXSP = 0;
//#else
		MN_POS_SIZ_SMALL	// Test & Debug
//#endif
	}
}


//*************************************************************************************************
// Initial
//-------------------------------------------------------------------------------------------------
void InitMenuSet(void)
{
	MENU_LIB_LIST
}

void InitFontOSD(void)
{	// Init FontOSD

	//----------------------------------------------------------------------------------------
	// OSD FONT
	FONT_ONw(1);

	MASK_CHADw(SPACE_CHAR_ID);	// null char
	HALF_CHADw(0x1f0);			// not use half char

#if 1
	FONT_LV0w(0x803010);													// Color plt MN_BLACK
	FONT_LV1w(0xb48080);													// Color plt MN_WHITE
	FONT_LV2w(0x708080);													// Color plt
	FONT_LV3w(0xa010a0);													// Color plt MN_YELLOW
#else
	FONT_LV0w(0x5000f0/*0x8080*/  );										// Color plt MN_BLACK
	FONT_LV1w(0xff0000);													// Color plt MN_WHITE
	FONT_LV2w(0x808080/*0x7c9a30*/);										// Color plt
	FONT_LV3w(0xa010a0);													// Color plt MN_YELLOW
#endif

	ChangeMenuSize();

	OsdWhiteAll();															// ATTR reset
	//OsdAttrsStgPos(0, MN_YELLOW);

	OsdClearAll();

	OsdCamTitle();															// Display CAM title

	SetFontChg();															// Language sel
//		InitFontChar(FONT_SEL0); // English
}

void InitMenu(void)
{	// Initiate Menu
	TxStrDecNoIRQ("Menu Init... ", 0, 0);

	UINT i;
	UINT nResetKey = 0;

	InitMenuSet();

	#if (model_Key==0)		// AD Key
		nResetKey = (ADC_KEYr<(((UINT)UP(UpADC_LEVEL_U)+(UINT)UP(UpADC_LEVEL_D))>>1));           // 141031 LH

	#elif (model_Key==1)	// GPIO Key
		nResetKey = !GpioGetPin(GPIO_KEY_U);									// if up key is pressed

	#else
//		nResetKey = 0;

	#endif


#if model_MenuDefSet == 1
	if (((WORD)((MSP(MP_START)<<8)|MSP(MP_END))!=(WORD)MN_DATA_ID)||nResetKey) {	// Parameter reset condition
		MenuResetSet(1);
		gbMnParSaveChk = 1;
		TxStrNoIRQ("MENU SET PAR reset ", 0, 0);
	}
#endif

	for(i=0; i<MN_PAR_EA; i++) gbMnParTblOld[i] = gbMnParTbl[i];     		// Make backup - 이미 Init() -> AppLoadPar() 에서 Flash로부터 gbMnParTbl에 저장됨

	if (MP_END_REAL>MP_END)	TxStrNoIRQ("MENU PAR size over ", 0, 0);			// Parameter verify
	if (((WORD)((MP(MP_START)<<8)|MP(MP_END))!=(WORD)MN_DATA_ID)||nResetKey) {	// Parameter reset condition
		MenuReset();
		MenuSave(MN_ON);

		TxStrNoIRQ("MENU PAR reset ", 0, 0);
    }
	else {
		MenuResetFnc();	// MenuReset() 안에서도 실행함
	}
	UartBaudRate();
	InitFontOSD();
}

//------------------------------------------------------------------------------
void MenuReset(void)
{	// parameter reset
	UINT i = 0;

#if model_MenuDefSet == 1
	for(i=0; i<MN_PAR_EA; i++) gbMnParTbl[i] = gbMnSetParTbl[i];

	//MenuResetFnc();
	MenuStyleSet(INIT_STYLE);
}

void MenuResetSet(const int aiReset)
{
	UINT i = 0;

	if(aiReset == 0) {	// aiReset = 0:MENU - SAVE, 1:InitMenu()
		for(i=0; i<MN_PAR_EA; i++) gbMnSetParTbl[i] = gbMnParTbl[i];
		gbMnSetParSaveChk = 1;
		return;
	}
#endif

	//----------------------------------------------------------------------------------------
	MP(MP_START)		= (BYTE)((MN_DATA_ID>>8)&0xff);		// Parameter verify
	MP(MP_END)			= (BYTE)((MN_DATA_ID)&0xff);			// "

	// FrameRate
	MP(MpOutFps)		= UpMpOutFps/*UP(UpMpOutFps)*/;
	MP(FrameMode)		= UP(UpFrameMode);

	MP(MpWdrWgt)		= UP(UpMpWdrWgt);
	MP(MpAdnr)			= UP(UpMpAdnr);
	MP(MpFrameRateLv)	= MN_3sLOW;
	FrameRateLvSet();	// Set MP(MpFrameRateLv), 'MP(FrameMode) & MP(MpWdrWgt) & MP(MpAdnr)' must be set before it !!!
	
	// System
	MP(MpSysFreq)		= UP(UpMpSysFreq);
	//MP(MpOutSclUser)	= UP(UpMpOutSclUser);
	MP(MpCBar)			= UP(UpMpCBar);
	MP(MpReset)			= MN_OFF/*UP(UpMpReset)*/;
	MP(MpLanguageSel)	= UP(UpMpLanguageSel);
	MP(MpOutFromat)		= UP(UpMpOutFromat);
	MP(MpCamID)			= UP(UpMpCamID);
	MP(MpBaudrate)		= UP(UpMpBaudrate);
	
	MP(MpCamTitleOn)	= UP(UpMpCamTitleOn);
	gbCamTitle(0) 		= UP(UpMpCamTitle0);
	gbCamTitle(1) 		= UP(UpMpCamTitle1);
	gbCamTitle(2) 		= UP(UpMpCamTitle2);
	gbCamTitle(3) 		= UP(UpMpCamTitle3);
	gbCamTitle(4) 		= UP(UpMpCamTitle4);
	gbCamTitle(5) 		= UP(UpMpCamTitle5);
	gbCamTitle(6) 		= UP(UpMpCamTitle6);
	gbCamTitle(7) 		= UP(UpMpCamTitle7);

	// AE
	MP(MpIris)          = UP(UpMpIris);
    MP(MpIrisLvl)       = UP(UpMpIrisLvl);
	MP(MpBrightness)    = UP(UpMpBrightness);
	MP(MpBrightnessMin) = UP(UpMpBrightnessMin);

	MP(MpLSpotLv)       = UP(UpMpLSpotLv);
	MP(MpLSpotTh)       = 0;
	MP(MpAllSpeed)		= UP(UpMpAllSpeed);
	MP(MpAgcSpeed)		= UP(UpMpAgcSpeed);
	MP(MpIrsSpeed)		= UP(UpMpIrsSpeed);
	MP(MpOpenSpeed)     = UP(UpMpOpenSpeed);
	MP(MpCloseSpeed)    = UP(UpMpCloseSpeed);
	MP(MpSmallSpeed)    = UP(UpMpSmallSpeed);
	//MP(MpClipMax)       = MN_OFF;
	MP(MpSatBrt)        = UP(UpMpSatBrt);
	MP(MpAdptAe)		= MN_ADPT_AE_BR/*MN_ADPT_AE_AGC*/;
	MP(MpMinGammaY)     = UP(UpMpMinGammaY);
	MP(MpMinGammaMode)  = MN_SAT_BACKLIGHT_DAY_ONLY;
	//MP(MpMinGammaC)     = 0;
	MP(MpAntiSatOn)     = UP(UpMpAntiSatOn);
	MP(MpStabilizing)   = UP(UpMpStabilizing);
	MP(MpSpotSpd)       = 10;
	
    MP(MpShutter)       = UP(UpMpShutter);
    MP(MpShutSpd)       = UP(UpMpShutSpd);
	MP(MpShtMin)        = UP(UpMpShtMin);
	MP(MpDeblurMin)     = UP(UpMpDeblurMin);
	MP(MpShtMax)        = UP(UpMpShtMax);
    MP(MpDcMode)        = UP(UpMpDcMode);
    MP(MpShtMode)       = UP(UpMpShtMode);
    MP(MpDss)           = UP(UpMpDss);
    MP(MpAgc)           = UP(UpMpAgc);
	MP(MpAgcMin)        = UP(UpMpAgcMin);

	MP(MpBackLightNor)	= UP(UpMpBackLight);
    MP(MpBackLight)     = UP(UpMpBackLight);
    MP(MpHlMaskThrs)    = UP(UpMpHlMaskThrs);
    MP(MpHlMaskColor)   = UP(UpMpHlMaskColor);
	MP(MpBlcMode)       = MN_BLC_MODE_CUSTOMIZE;
    MP(MpBlcPosX)       = UP(UpMpBlcPosX);
    MP(MpBlcPosY)       = UP(UpMpBlcPosY);
    MP(MpBlcSizX)       = UP(UpMpBlcSizX);
    MP(MpBlcSizY)       = UP(UpMpBlcSizY);
	//MP(MpWdrWgt)		= UP(UpMpWdrWgt);
	MP(MpWdrBlc)        = 0;
	MP(MpBlcOsdOn)      = 0;
	MP(MpWdrABrt)       = UpAE_WDR_LTGT_OFST2;	//ParSet(UpAE_WDR_LTGT_OFST2, MP(MpWdrABrt));
	MP(MpWdrABrtS)      = UpAE_WDR_STGT_OFST2;	//ParSet(UpAE_WDR_STGT_OFST2, MP(MpWdrABrtS));
#if model_WDR_ROI
	MP(MpROIWdrMode)	= MN_OFF;

	for(i=0; i<4; i++)
	{
		gbMnROIWdrCfg(i)->bAction = 0x1;

		gbMnROIWdrCfg(i)->bPosX_Lo = ((FR_HW/6+(200*i))&0xff);
		gbMnROIWdrCfg(i)->bPosX_Up = (((FR_HW/6+(200*i))>>8)&0xff);

		gbMnROIWdrCfg(i)->bPosY_Lo = ((FR_VW/6+(150*i))&0xff);
		gbMnROIWdrCfg(i)->bPosY_Up = (((FR_VW/6+(150*i))>>8)&0xff);

		gbMnROIWdrCfg(i)->bSizX_Lo = 0x00;
		gbMnROIWdrCfg(i)->bSizX_Up = 0x02;

		gbMnROIWdrCfg(i)->bSizY_Lo = 0xb0;
		gbMnROIWdrCfg(i)->bSizY_Up = 0x01;
	}
#endif

	// TDN, LED
	MP(MpTdn)           = UP(UpMpTdn);
	MP(MpTdnThrs)       = UP(UpMpTdnThrs);
	MP(MpTdnMarg)       = UP(UpMpTdnMarg);
	MP(MpTdnDly)        = UP(UpMpTdnDly);
	MP(MpLedOn)         = UP(UpMpLedOn);
	MP(MpLedSatCtrl)    = UP(UpMpLedSatCtrl);
	MP(MpTdnSw)         = UP(UpMpTdnSw);
	MP(MpTdnDNThres)    = UP(UpMpTdnDNThres);
	MP(MpTdnNDThres)    = UP(UpMpTdnNDThres);
	
	// ACE,Defog
	MP(MpAce)           = UP(UpMpAce);
	MP(MpAceGmgn)       = UP(UpMpAceGmgn);
	MP(MpAceBrt)        = UP(UpMpAceBrt);
	MP(MpAceCont)       = UP(UpMpAceCont);
	MP(MpDefog)         = UP(UpMpDefog);
	MP(MpDefogMode)     = UP(UpMpDefogMode);
	MP(MpDefogLevel)    = UP(UpMpDefogLevel);
	MP(MpWdrDBrt)       = UpACEWDR1_TH;		//ParSet(UpACEWDR1_TH, MP(MpWdrDBrt));
	MP(MpWdrDCont)      = UpACEWDR2_TH;		//ParSet(UpACEWDR2_TH, MP(MpWdrDCont));

    // AWB
    MP(MpAwb)           = UP(UpMpAwb);
    MP(MpAwbPrst)       = UP(UpMpAwbPrst);
    MP(MpAwbMnl)        = UP(UpMpAwbMnl);
    MP(MpRgain)         = UP(UpMpRgain);
    MP(MpBgain)         = UP(UpMpBgain);
    MP(MpSaturation)    = UP(UpMpSaturation);
    MP(MpSaturationR)   = UP(UpMpSaturationR);
    MP(MpSaturationB)   = UP(UpMpSaturationB);

	MP(MpAwbSpeed)      = UpAWB_SPEED_MTRX;		//ParSet(UpAWB_SPEED_MTRX, MP(MpAwbSpeed));
	MP(MpAwbAgcWgtMin)  = UpAWB_AGC_WGT_MIN;	//ParSet(UpAWB_AGC_WGT_MIN, MP(MpAwbAgcWgtMin));
	MP(MpLSUP_ON)       = UpLSUP_ON;			//ParSet(UpLSUP_ON, MP(MpLSUP_ON));
	MP(MpAwbYSliceMax)  = UpAWB_YSLICE;			//ParSet(UpAWB_YSLICE, MP(MpAwbYSliceMax));
	MP(MpAwbYSliceMin)  = 4;
	MP(MpAwbSliceCntMin)= 8;
	MP(MpAwbSliceCntMax)= 13;
	MP(MpCES_ON)        = UP(UpMpCES_ON);
	MP(MpWdrCES_ON)     = UP(UpMpWdrCES_ON);
	MP(MpAwbStyle)      = 0; // Default Style #0

    // ETC
    //MP(MpAdnr)            = UP(UpMpAdnr);
    //MP(MpSharpnessSel)    = 0;
    MP(MpSharpness)     = UP(UpMpSharpness);
	MP(MpShpSmallEdge)  = UP(UpMpShpSmallEdge);
	MP(MpShpBigEdge)    = UP(UpMpShpBigEdge);
	MP(MpShpAgcLow)     = UP(UpMpShpAgcLow);
	MP(MpShpAgcMid)     = UP(UpMpShpAgcMid);
	MP(MpShpAgcHigh)    = UP(UpMpShpAgcHigh);
    MP(MpGamma)         = UP(UpMpGamma);
	MP(MpGammaDay)      = MN_GAMMA_055;
	MP(MpGammaNgt)      = MN_GAMMA_045;
	MP(MpGammaWdr)      = MN_GAMMA_050;
    MP(MpFlip)          = UP(UpMpFlip);
    MP(MpMirror)        = UP(UpMpMirror);
    MP(MpShading)       = UP(UpMpShading);
    MP(MpShdWeight)     = UP(UpMpShdWeight);
	MP(MpDnrIncrease)   = MN_OFF;

	// Privacy Box
    MP(MpPrivacy)       = UP(UpMpPrivacy);
    MP(MpPvcZone)       = UP(UpMpPvcZone);
    MP(MpPvcFormat)     = UP(UpMpPvcFormat);
    MP(MpPvcTrans)      = UP(UpMpPvcTrans);
    MP(MpPvcYLevel)     = UP(UpMpPvcYLevel);
    MP(MpPvcCRLevel)    = UP(UpMpPvcCRLevel);
    MP(MpPvcCBLevel)    = UP(UpMpPvcCBLevel);
	for (i=0; i<PVC_EA; i++) {
		gbMnPvcCfg(i)->bAction = 1;
		gbMnPvcCfg(i)->bPosX   = 8+((i%6)*4);
		gbMnPvcCfg(i)->bPosY   = 2+((i/6)*4);
		gbMnPvcCfg(i)->bSizX   = 3;
		gbMnPvcCfg(i)->bSizY   = 3;
	}

	// IMD
    MP(MpItl)           = UP(UpMpItl);
    //MP(MpItlRes)      = 0;
    MP(MpItlSens)       = UP(UpMpItlSens);
    MP(MpItlWindowuse)  = UP(UpMpItlWindowuse);
    MP(MpItlDettone)    = UP(UpMpItlDettone);
    MP(MpItlRectFill)   = UP(UpMpItlRectFill);

#if(!IMD_MASK_MODE)			// ghlee
	gbMnImdCfg(0)->bAction = MN_ON;
	gbMnImdCfg(0)->bPosX   = 1;				gbMnImdCfg(0)->bPosY   = 1;
	gbMnImdCfg(0)->bSizX   = IMD_HW-2;		gbMnImdCfg(0)->bSizY   = (IMD_VW-2);

	for(i=1; i<MASK_EA; i++)
	{
		gbMnImdCfg(i)->bAction = MN_OFF;
		gbMnImdCfg(i)->bPosX   = ((IMD_HW/3)*i)-10;		gbMnImdCfg(i)->bPosY   = (IMD_VW/3)*i;
		gbMnImdCfg(i)->bSizX   = IMD_HW/3;				gbMnImdCfg(i)->bSizY   = IMD_VW/3;
	}
#else						//Masking Mode

	for(i=0; i<MASK_EA; i++)
	{
		gbMnImdCfg(i)->bAction = 1;
		gbMnImdCfg(i)->bPosX   = (IMD_HW/10)*(i+1);		gbMnImdCfg(i)->bPosY   = ((IMD_VW/5)*(i+1))-5;
		gbMnImdCfg(i)->bSizX   = 10;					gbMnImdCfg(i)->bSizY   = 10;
	}

#endif

	MP(MpItlMaskOsd)    = UP(UpMpItlMaskOsd);
    MP(MpItlAlarm)      = UP(UpMpItlAlarm);
    MP(MpItlSigno)      = UP(UpMpItlSigno);

	// Hidden MENU
	MP(MpShdDet)		= MN_OFF;
	MP(MpDefDet)		= MN_OFF;
	//MP(MpDefectThrs)	= 8;

	// Test
	//MP(MpClockOnOff)	= MN_OFF;
	//MP(MpTest)			= MN_OFF;

#if(model_Lens==0)	// 2016610 - WHL
    MP(MpFad_On)        = UP(UpMpFad_On);
    MP(MpFad_Zone)      = UP(UpMpFad_Zone);
    MP(MpFad_Trans)     = UP(UpMpFad_Trans);
    MP(MpFad_PosX)      = UP(UpMpFad_PosX);
    MP(MpFad_PosY)      = UP(UpMpFad_PosY);
    MP(MpFad_SizX)      = UP(UpMpFad_SizX);
    MP(MpFad_SizY)      = UP(UpMpFad_SizY);
#endif

	// 20130724
#if(model_Lens==1)	 //TwoMotorAF
  #if 1
	MP(MpMAF_mode)		= UP(UpMpMAF_mode)		 ;
	MP(MpMAF_Filter)	= UP(UpMpMAF_Filter)	 ;
	MP(MpMAF_MotorInit)	= UP(UpMpMAF_MotorInit)	 ;
	MP(MpMAF_TestMode)	= UP(UpMpMAF_TestMode)	 ;
	MP(MpMAF_TotalPos_Z)= UP(UpMpMAF_TotalPos_Z) ;
	MP(MpMAF_StartPos_Z)= UP(UpMpMAF_StartPos_Z) ;
	MP(MpMAF_EndPos_Z)	= UP(UpMpMAF_EndPos_Z)	 ;
	MP(MpMAF_TotalPos_F)= UP(UpMpMAF_TotalPos_F) ;
	MP(MpMAF_StartPos_F)= UP(UpMpMAF_StartPos_F) ;
	MP(MpMAF_EndPos_F)	= UP(UpMpMAF_EndPos_F)	 ;
	MP(MpMAF_LensModel) = UP(UpMpMAF_LensModel)  ;
  #else
	MP(MpAFmode)		= 0;
	MP(MpScanning)		= 0;
	MP(MpOnepushAF)		= 0;
	MP(MpSyncTDN)		= 0;
  #endif
#endif

#if(model_Iris_P)	//141124 HSH Main
	MP(MpIRISMINOFS)	= 128;
	MP(MpIRISMAXOFS)	= 128;
#endif

#if	(IMAGE_INPUT_MODE==1)
	MP(MpPipOn)			= UP(UpMpPipOn);
	MP(MpPipColor)		= UP(UpMpPipColor);
	MP(MpPip1_PosX)		= UP(UpMpPip1_PosX);
	MP(MpPip1_PosY)		= UP(UpMpPip1_PosY);
	MP(MpPip1_SizX)		= UP(UpMpPip1_SizX);
	MP(MpPip1_SizY)		= UpMpPip1_SizY;
#endif
	
#if (DOOR_BELL_CAMERA==1)
	MP(MpDoorCam_LP_Mode) =	UP(UpMpDoorCam_LP_Mode);
#endif


#if model_MenuDefSet == 1

	for(i=0; i<MN_PAR_EA; i++) gbMnSetParTbl[i] = gbMnParTbl[i];
	gbMnSetParSaveChk = 1;

#else

	//MenuResetFnc();
	MenuStyleSet(INIT_STYLE);

#endif
}

#if(model_Iris_P)	//141124 HSH Main
void MenuIrisSet(void)
{
	static BYTE bIrisSetBuf[2];

	if(gbMnIrisFlag==1){
		bIrisSetBuf[0]=MP(MpIris);
		bIrisSetBuf[1]=MP(MpIrisLvl);
	}
	else if(gbMnIrisFlag==2){
		MP(MpIris)=MN_LENS_MNL;
		
	#if(model_Iris==4)
		MP(MpIrisLvl)=20;
	
	#else// if(model_Iris==3)
		MP(MpIrisLvl)=0;
	
	#endif
	}
	else if(gbMnIrisFlag==3){
		MP(MpIris)=MN_LENS_MNL;

	#if(model_Iris==4)
		MP(MpIrisLvl)=0;
	
	#else// if(model_Iris==3)
		MP(MpIrisLvl)=20;
	
	#endif
	}
	else if(gbMnIrisFlag==0){
		MP(MpIris)=bIrisSetBuf[0];
		MP(MpIrisLvl)=bIrisSetBuf[1];
	}
}
#endif

//	UINT ISPM DispImg(UINT anIdx, UINT anPosY, UINT anPosX, UINT anCnt)
//	{	
//		//printf("print OSD anPosY%d, anPosX%d, anCnt%d \r\n",anPosY, anPosX, anCnt);
//	
//		DrawStr(WARNINGTEST1,anPosY, anPosX, anCnt);
//		DrawStr(WARNINGTEST2,anPosY+1, anPosX, anCnt);
//	
//		//DispStr("\x07\x08\x09\x0a\x0b",anPosY+2, anPosX, anCnt);
//		return 0;
//	}

//*************************************************************************************************
// Menu
//-------------------------------------------------------------------------------------------------
void Menu(void)
{
	if (MP(MpDefDet)==1)	return;

	static BYTE gbPassCnt1=0;
	static BYTE gbPassCnt2=0;
	static BYTE gbPassCnt3=0;
	static BYTE gbPassCnt4=0;
	static BYTE gbPassCnt5=0;
	static BYTE gbPassCnt6=0;

	const BYTE gbPassKeys1[] = MENU_PK_SETUP;
	const BYTE gbPassKeys2[] = MENU_PK_DEBUG;

	const int iSetMnOn = (!MENU_TITLE_IS(OFF)) ? (LibUtlKeyPass(gbPassKeys1, sizeof(gbPassKeys1), &gbPassCnt1) == 1) : 0 ;

	//----------------------------------------------------------------------------------------
	// Menu Exit
  #if model_ExitMenu == 1 || model_ExitMenu == 2 || model_ExitMenu == 3
	if(MENU_NAME_IS(RETURN)) {
		if(KEY_L || KEY_R) {
		  #if model_ExitMenu == 3
			menu_val(&gbMnExit, 0, 2, 1);
			switch(gbMnExit) {
				case 0 : MENU_NAME_REDRAW(RETURN);	MENU_VAL_REDRAW(e);			break;
				case 1 : MENU_NAME_REDRAW(nEXIT);	MENU_VAL_REDRAW(SAVEe);		break;
				case 2 : MENU_NAME_REDRAW(nEXIT);	MENU_VAL_REDRAW(CANCELe);	break;
			}
		  #else
			gbMnExit = !gbMnExit;
			if(gbMnExit) { MENU_NAME_REDRAW(nEXIT); }
			else		 { MENU_NAME_REDRAW(RETURN); }
		  #endif
		}
		else if(KEY_C && gbMnExit) {
		  #if model_ExitMenu == 1 || model_ExitMenu == 3
			//const BYTE bSetFontChgOn = (gbMnParTbl[MpLanguageSel] != gbMnParTblOld[MpLanguageSel]) && (gbMnExit == 2);	// 160223 KSH
			MenuSave(gbMnExit == 1);
			//if(bSetFontChgOn) { SetFontChg(); }																			// 160223 KSH
			gbMnExit = 0;
		  #endif
			MENU_CLOSE();
		}
	}
  #endif

  #if model_ExitMenu == 2
	if((gbMnExit == 1) && MENU_TITLE_IS(OFF)) {
		gbMnExit = 0;
		MENU_OPEN(SAVEnEXIT);
	}
  #endif


	MENU_START

// Main MENU --------------------------------------------------------------------------------
// EN673

#if(model_Iris==0)						// no Iris
	MENU_SET( 10, MENU, MN_ON, MN_OFF,
			//ADVANCED_AE,	MENU_ONEi(MN_ON, e, MN_ON, ADVANCED_AE, ),

#elif(model_Iris_DC)					// DC Iris
	MENU_SET( 11, MENU, MN_ON, MN_OFF,
			IRIS,			MENU_STRn(MN_ON, if_KEY_LR(ShutterMenuGet()), MP(MpIris), 2, ELC, ALC),
#else									// pIris, AF Iris
	MENU_SET( 11, MENU, MN_ON, MN_OFF,
			IRIS,			MENU_STRi(MN_ON, MP(MpIris)==MN_LENS_MNL, IRIS, , if_KEY_LR(ShutterMenuGet()), MP(MpIris), 2, MANUALe, AUTO),
#endif

			EXPOSURE,		MENU_ONEi(MN_ON, e, MN_ON, EXPOSURE, ),

//				BACKLIGHT, 		MENU_STRi(MN_ON, MP(MpBackLight)!= MN_OFF, BACKLIGHT, 	if (MENU_VAL_IS(HLCe)) 		{MENU_TITLE_CHANGE(HLC)}
//																						else if (MENU_VAL_IS(BLCe)) {MENU_TITLE_CHANGE(BLC)}
//																						else 						{MENU_TITLE_CHANGE(WDR)}, if_KEY_LR(/*MENU_REDRAW()*/), MP(MpBackLight), 4, OFF, HLCe, BLCe, WDRe),

			BACKLIGHT, 		({ if(!WDR_MODE) {
								MENU_STR( MN_ON, MP(MpBackLight), 3, OFF, HLCe, BLCe)
								MENU_IN(MP(MpBackLight)!=MN_OFF,BACKLIGHT,	if (MENU_VAL_IS(HLCe)) 		{MENU_TITLE_CHANGE(HLC)}
																			else if (MENU_VAL_IS(BLCe)) {MENU_TITLE_CHANGE(BLC)})
								MENU_CODE(if_KEY_LR(/*MENU_REDRAW()*/ MP(MpBackLightNor) = MP(MpBackLight);))
							} else {
								MENU_STR( MN_ON, MP(MpWdrBlc), 2, OFF, BLCe)    MENU_IN(MENU_VAL_IS(BLCe), BLC,)
							} }),

		#if(model_Lens==0)
			FOCUS_ADJ, 		MENU_STRi(MN_ON, MENU_VAL_IS(ONe) ,FOCUS_ADJ, MENU_OFF_GRAY_ONLY();, if_KEY_LR(MENU_REDRAW_GRAY_ONLY()), MP(MpFad_On), 2, OFF, ONe), // 150325 WHL
			
		#elif(model_Lens==1)
			MOTORIZED,		({ if(gbMnSetupOn==1) {
								MENU_ONEi(MN_ON, e, MN_ON, MOTOR_SETUP, )	MENU_NAME_REDRAW(MOTOR_SETUP)
							} else {
								MENU_ONEi(MN_ON, e, MN_ON, MOTORIZED, )		MENU_NAME_REDRAW(MOTORIZED)
							} }),
		#endif
			
			DAYnNIGHT,		MENU_STRi(MN_ON, (MENU_VAL != _S(COLOR)), DAYnNIGHT, , ,MP(MpTdn), 4, AUTOe, COLOR, BnWe, EXTERNe),		// 141120 LH
			COLORm,			MENU_ONEi(MN_ON, e, MN_ON, COLORm, ),
																						
//				DNR,			MENU_STRn(MN_ON/*MP(MpBackLight)!= MN_BL_WDR*/, , MP(MpAdnr), 4, OFF, LOW, MIDDLE, HIGH),
			IMAGE,			MENU_ONEi(MN_ON, e, MN_ON, IMAGE, ),
			MOTION, 		MENU_STRi(MN_ON, MENU_VAL_IS(ONe) ,MOTION, MENU_OFF_GRAY_ONLY();, , MP(MpItl), 2, OFF, ONe),				//lgh

		#if(model_Lens==0)
			OUTPUTSET,      MENU_ONEi(MP(MpFad_On)==MN_OFF, e, MN_ON, OUTPUTSET, ),
		#else
			OUTPUTSET,      MENU_ONEi(MN_ON, e, MN_ON, OUTPUTSET, ),
		#endif

			SYSTEM, 		MENU_ONEi(MN_ON, e, !iSetMnOn, SYSTEM, /*MENU_OFF_GRAY_ONLY();*/ ),		// if not Setup menu
		#if model_ExitMenu == 2
			EXIT,			MENU_ONEo(MN_ON,  , MN_ON, gbMnExit = 1;))
		#elif model_ExitMenu == 3
			EXIT,			MENU_STRo(MN_ON, MN_ON,
								//const BYTE bSetFontChgOn = (gbMnParTbl[MpLanguageSel] != gbMnParTblOld[MpLanguageSel]) && (gbMnExit == 1);	// 160223 KSH
								MenuSave(gbMnExit==0);
								//if(bSetFontChgOn) { SetFontChg(); }	// 160223 KSH
								gbMnExit = 0;, , gbMnExit, 2, SAVEe, CANCELe))
		#else
			EXIT,			MENU_ONEo(MN_ON,  , MN_ON, MenuSave(MN_ON);))
		#endif

#if (model_Iris_P)
	// MENU - IRIS
	MENU_SET( 2, IRIS, MN_ON, MN_OFF,
			LEVEL,			MENU_BARn(MN_ON, , MP(MpIrisLvl), 0, 20, 1 ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
#endif

#if(model_Lens==0) //  // 150325 WHL
	// MENU - Focus ADJ
	MENU_SET( 7, FOCUS_ADJ, MN_ON, MN_OFF,
			FOCUS_AREA,		MENU_STRn(MN_ON, , MP(MpFad_Zone), 3, NARROW, MIDDLE, WIDE),
			DISP_TONE,		MENU_VARn(MN_ON, , MP(MpFad_Trans), 0, 2, 1, ),
			DISP_H_POS, 	MENU_VARn(MN_ON, , MP(MpFad_PosX), 0, 60, 0, ),
			DISP_V_POS, 	MENU_VARn(MN_ON, , MP(MpFad_PosY), 0, 34, 0, ),
			DISP_H_SIZE, 	MENU_VARn(MN_ON, , MP(MpFad_SizX), 0, 60, 0, ),
			DISP_V_SIZE, 	MENU_VARn(MN_ON, , MP(MpFad_SizY), 0, 34, 0, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
#endif

#if(model_Lens==1)
	// MENU - MOTORIZED
	MENU_SET( 4, MOTORIZED, MN_ON, MN_OFF,
			AF_MODE,			MENU_STRn(MN_ON, if((MP(MpMAF_mode)==0)&&KEY_C) gbAutoFocus_Flag=1;, MP(MpMAF_mode), 3, ONESHOTp, MANUAL, MAF_DISABLE),
			DnN_FILTER,			MENU_STRn(MN_ON, , MP(MpMAF_Filter), 2, SYNC_OFF, SYNC_ON),
			INITIAL,			MENU_PUSH(MN_ON, MP(MpMAF_MotorInit), ONp, PUSHING, PUSH_DELAY_NOT/*PUSH_DELAY_NOR*/, ),
			RETURN,				MENU_ONEo(MN_ON, e, MN_ON, ))
#endif

#define MENU_STR_SHUTTER_SPEED(V,CODE) \
	({if(MP(MpSysFreq) == MN_SYSFREQ_60){\
		if(MN_FPS_LOW)	{ MENU_STR(MN_ON, V, 11, SHUT_30, SHUT_60, SHUT_120, SHUT_250, SHUT_500, SHUT_1000, SHUT_2000, SHUT_4000, SHUT_8000, SHUT_15000, SHUT_30000)}\
		else			{ MENU_STR(MN_ON, V, 11, SHUT_60, SHUT_120, SHUT_250, SHUT_500, SHUT_1000, SHUT_2000, SHUT_4000, SHUT_8000, SHUT_15000, SHUT_30000, SHUT_60000)}\
	}else{\
		if(MN_FPS_LOW)	{ MENU_STR(MN_ON, V, 11, SHUT_25, SHUT_50, SHUT_100, SHUT_200, SHUT_400, SHUT_800, SHUT_1600, SHUT_3200, SHUT_6400, SHUT_12800, SHUT_25600)}\
		else			{ MENU_STR(MN_ON, V, 11, SHUT_50, SHUT_100, SHUT_200, SHUT_400, SHUT_800, SHUT_1600, SHUT_3200, SHUT_6400, SHUT_12800, SHUT_25600, SHUT_51200)} }\
	MENU_CODE(CODE) })


#if AE_CTRL_ADV
	// MENU - EXPOSURE
	MENU_SET( 7, EXPOSURE, MN_ON, MN_OFF,
			BRIGHTNESS, 	//MENU_ONEi(MN_ON, e, MN_ON, BRIGHTNESS, ),
							({if(gbMnSetupOn==1){
								MENU_ONEi(MN_ON, e, MN_ON, BRIGHTNESS, )
							}else{
								MENU_BARn(MN_ON, , MP(MpBrightness), 0, 20, 1)} }),

			//ADAPTIVE_AE,	MENU_STRn(MN_ON, , MP(MpAdptAe), 4, OFF, AGC, BRIGHTNESS, /*MINp_LIGHT*/),
			ANTI_SAT,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), ANTI_SAT, , , MP(MpAntiSatOn), 2, OFF, ONe),

			SHUTTER,		({  MENU_STR( MN_ON, MP(MpShutter), 3, AUTOe/*AUTO*/, MANUALe, FLICKER)
								MENU_IN(MENU_VAL_IS(AUTOe) || MENU_VAL_IS(MANUALe), SHUTTER,	if (MENU_VAL_IS(AUTOe))		   {MENU_TITLE_CHANGE(SHUTTER_AUTO)}
																								else if (MENU_VAL_IS(MANUALe)) {MENU_TITLE_CHANGE(SHUTTER_MANUAL)})  }),
			SENS_UP,		({if(MN_FPS_LOW){
								MENU_STR(MN_ON, MP(MpDss), 6, OFF, X2, X4, X8, X16, X32)			// 14.2.13
							}else{
								MENU_STR(MN_ON, MP(MpDss), 7, OFF, X2, X4, X8, X16, X32, X64)} }),	// 14.2.13

			AGC/*AGC_MAX_*/,//MENU_BARn(MN_ON, , MP(MpAgc), 0, 255, 1),
							({menu_bar(MN_ON, &MP(MpAgc), 0, 255, 1, 0);
							if(gbMnSetupOn==1){
								MENU_IN(MN_ON, AGC, )
								SETFONTID(DRAW_Y, MN_SXSP+10, 0xa6);
							}}),

			AE_SPEED,		//MENU_STRi(MN_ON, MN_ON, AE_SPEED, , , MP(MpSmallSpeed), 3, HIGHe, MIDDLEe, LOWe),
							({menu_bar(MN_ON, &MP(MpAllSpeed), 0, 20, 1, 0);
							if(gbMnSetupOn==1){
								MENU_IN(MN_ON, AE_SPEED, )
								SETFONTID(DRAW_Y, MN_SXSP+10, 0xa6);
							}}),

			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 3, BRIGHTNESS, MN_ON, MN_OFF,
			DAY,			//MENU_BARn(MN_ON, , MP(MpBrightness), 0, 20, 1),
							/*({ menu_bar(MN_ON, &MP(MpBrightness), 0, 20, 1, 0);
							   DispClrDec(TgtMaxGet(0,0), DRAW_Y, MN_SXSP, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
							({ menu_bar(MN_ON, &MP(MpBrightness), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMaxGet(0,0), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),
							   	
			NIGHT,			//MENU_BARn(MN_ON, , MP(MpBrightnessMin), 0, 20, 1),
							/*({ menu_bar(MN_ON, &MP(MpBrightnessMin), 0, 20, 1, 0);
							   DispClrDec(TgtMinGet(TgtMaxGet(0,0), MP(MpBrightnessMin)), DRAW_Y, MN_SXSP, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),*/
							({ menu_bar(MN_ON, &MP(MpBrightnessMin), 0, 20, 1, _S());
							   SETFONTID(DRAW_Y, MN_SXSP+3, ':');
							   DispClrDec(TgtMinGet(TgtMaxGet(0,0), MP(MpBrightnessMin)), DRAW_Y, MN_SXSP+5, 3);
							   MENU_CODE(if_KEY_LR(MENU_REDRAW())) }),

			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - EXPOSURE:ANTI_SAT
	MENU_SET( 5, ANTI_SAT, (gbMnSetupOn==1), MN_OFF,
			WEIGHT,			MENU_BARn(MN_ON, , MP(MpSatBrt), 0, 20, 1),
			LEVEL,			MENU_BARn(MN_ON, , MP(MpLSpotLv), 0, 20, 1),
			CTRLp_SPEED,	MENU_BARn(MN_ON, , MP(MpSpotSpd), 0, 20, 1),
			STABILIZING,	MENU_STRn(MN_ON, if_KEY_LR(AntiSatStabilizingSet()), MP(MpStabilizing), 4, OFF, LOW, MIDDLE, HIGH),
			//ERR_CHT_MAX,	MENU_BARn(MN_ON, if_KEY_LR(MENU_REDRAW()), MP(MpErrChtMax), MP(MpErrChtMin), ERR_CHT_SIZ, 1),
			//ERR_CHT_MIN,	MENU_BARn(MN_ON, if_KEY_LR(MENU_REDRAW()), MP(MpErrChtMin), 1, MP(MpErrChtMax), 1),
			
			//nBACKLIGHT,		MENU_STRn(MN_ON, , MP(MpMinGammaY), 4, OFF, LOW, MIDDLE, HIGH),
			//nBACKLIGHT,		MENU_STRi(MN_ON, MP(MpMinGammaY)!=MN_OFF, nBACKLIGHT, , , MP(MpMinGammaY), 4, OFF, LOWe, MIDDLEe, HIGHe),
			//AE_GAMMA,		MENU_ONEi(MN_ON, e, MN_ON, AE_GAMMA, ),
			//AE_GAMMA,		MENU_STRn(MN_ON, , MP(MpMinGammaY), 4, GAMMA_045, GAMMA_040, GAMMA_035, GAMMA_030),

			//SPOT_TH,		MENU_BARn(MN_ON, , MP(MpLSpotTh), 0, 20, 1),
			//CLIP_MAX,		MENU_STRn(MN_ON, , MP(MpClipMax), 2, OFF, ON),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - EXPOSURE:ANTI_SAT
	MENU_SET( 2, ANTI_SAT, (gbMnSetupOn==0), MN_OFF,
			WEIGHT,			MENU_BARn(MN_ON, , MP(MpSatBrt), 0, 20, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 3, nBACKLIGHT, MN_ON, MN_OFF,
			MODE,			MENU_STRn(MN_ON, , MP(MpMinGammaMode), 2, ALWAYS, DAY_ONLY),
			LEVEL,			MENU_STRn(MN_ON, , MP(MpMinGammaY), 4, OFF, LOW, MIDDLE, HIGH),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - EXPOSURE:AE_SPEED
	MENU_SET( 7, AE_SPEED, MN_ON, MN_OFF,
			AE_SPEED,		MENU_BARn(MN_ON, , MP(MpAllSpeed), 0, 20, 1),
			STABILIZING,	MENU_STRn(MN_ON, , MP(MpSmallSpeed), 3, LOW, MIDDLE, HIGH),
			DtoB_SLOPE,		MENU_BARn(MN_ON, , MP(MpOpenSpeed), 0, 20, 1),
			BtoD_SLOPE,		MENU_BARn(MN_ON, , MP(MpCloseSpeed), 0, 20, 1),
			AGC_SLOPE,		MENU_BARn(MN_ON, , MP(MpAgcSpeed), 0, 20, 1),
			IRIS_SLOPE,		MENU_BARn(MN_ON, , MP(MpIrsSpeed), 0, 20, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 3, AGC, MN_ON, MN_OFF,
			AGC,			MENU_BARn(MN_ON, , MP(MpAgc), 0, 255, 1),
			AGC_MIN_,		MENU_BARn(MN_ON, , MP(MpAgcMin), 0, 255, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - ADVANCED_AE:AE_GAMMA
	/*MENU_SET( 3, AE_GAMMA, MN_ON, MN_OFF,
			Y_GAMMA,		MENU_STRn(MN_ON, , MP(MpMinGammaY), 4, GAMMA_045, GAMMA_040, GAMMA_035, GAMMA_030),
			C_GAMMA,		MENU_STRn(MN_ON, , MP(MpMinGammaC), 4, GAMMA_030, GAMMA_025, GAMMA_020, GAMMA_015),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))*/


	// MENU - EXPOSURE - SHUTTER:AUTO
  #if(model_Iris==0)						// no Iris
	MENU_SET( 5, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==1), MN_OFF,
  #elif(model_Iris_DC)					// DC Iris
	MENU_SET( 6, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==1), MN_OFF,
			nIRIS,			MENU_STRn(MN_ON, if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpIris), 2, ELC, ALC),
  #else									// pIris, AF Iris
	MENU_SET( 6, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==1), MN_OFF,
			nIRIS,			MENU_STRi(MN_ON, MP(MpIris)==MN_LENS_MNL, IRIS, , if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpIris), 2, MANUALe, AUTO),
  #endif
			AUTO_MODE,
				#if(model_Iris_DC)
					({if(MP(MpIris)==MN_LENS_MNL) {
							MENU_STR(MN_ON, MP(MpShtMode), 3, NORMAL, DEBLUR, CUSTOMIZE )				// CAUTION ! -> MENU_STR
					}else{	//MP(MpIris) == MN_LENS_DC
							MENU_STR(MN_ON, MP(MpDcMode), 4, INDOOR, OUTDOOR, DEBLUR, CUSTOMIZE)} MENU_CODE(if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();)) }),
				#else
							MENU_STRn(MN_ON, if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpShtMode), 3, NORMAL, DEBLUR, CUSTOMIZE ),			// CAUTION ! -> MENU_STRn
				#endif
			SHUTTER_MIN,	MENU_STR_SHUTTER_SPEED(gbMpShtMin,    if_KEY_LR( ShutterMenuSet(); MENU_REDRAW();)),
			DEBLUR_MIN,		MENU_STR_SHUTTER_SPEED(gbMpDeblurMin, if_KEY_LR( ShutterMenuSet(); MENU_REDRAW();)),
			SHUTTER_MAX,	MENU_STR_SHUTTER_SPEED(gbMpShtMax,    if_KEY_LR( ShutterMenuSet(); MENU_REDRAW();)),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


	// MENU - EXPOSURE - SHUTTER:AUTO
  #if(model_Iris==0)						// no Iris
	MENU_SET( 2, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==0), MN_OFF,
  #elif(model_Iris_DC)					// DC Iris
	MENU_SET( 3, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==0), MN_OFF,
			nIRIS,			MENU_STRn(MN_ON, if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpIris), 2, ELC, ALC),
  #else									// pIris, AF Iris
	MENU_SET( 3, SHUTTER_AUTO, (MP(MpShutter)==MN_SHUT_AUTO)&&(gbMnSetupOn==0), MN_OFF,
			nIRIS,			MENU_STRi(MN_ON, MP(MpIris)==MN_LENS_MNL, IRIS, , if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpIris), 2, MANUALe, AUTO),
  #endif
			AUTO_MODE,
				#if(model_Iris_DC)
					({if(MP(MpIris)==MN_LENS_MNL) {
							MENU_STR(MN_ON, MP(MpShtMode), 3, NORMAL, DEBLUR, )				// CAUTION ! -> MENU_STR
					}else{	//MP(MpIris) == MN_LENS_DC
							MENU_STR(MN_ON, MP(MpDcMode), 4, INDOOR, OUTDOOR, DEBLUR, )} MENU_CODE(if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();)) }),
				#else
							MENU_STRn(MN_ON, if_KEY_LR(ShutterMenuGet(); MENU_REDRAW();), MP(MpShtMode), 3, NORMAL, DEBLUR, ),			// CAUTION ! -> MENU_STRn
				#endif
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


#else
	MENU_SET( 5, EXPOSURE, MN_ON, MN_OFF,
			BRIGHTNESS, 	MENU_BARn(MN_ON, , MP(MpBrightness), 0, 20, 1),
			SHUTTER,		({  MENU_STR( MN_ON, MP(MpShutter), 3, AUTOe, MANUALe, FLICKER)
								MENU_IN(MENU_VAL_IS(AUTOe) || MENU_VAL_IS(MANUALe), SHUTTER,	if (MENU_VAL_IS(AUTOe))		   {MENU_TITLE_CHANGE(SHUTTER_AUTO)}
																								else if (MENU_VAL_IS(MANUALe)) {MENU_TITLE_CHANGE(SHUTTER_MANUAL)})  }),
			SENS_UP,
					({if(MN_FPS_LOW){
							MENU_STR(MN_ON, MP(MpDss), 6, OFF, X2, X4, X8, X16, X32)				// 14.2.13
					}else{
							MENU_STR(MN_ON, MP(MpDss), 7, OFF, X2, X4, X8, X16, X32, X64)} }),	// 14.2.13
			AGC,			MENU_BARn(MN_ON, , MP(MpAgc), 0, 255, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 2, SHUTTER_AUTO, MP(MpShutter)==MN_SHUT_AUTO, MN_OFF,
			AUTO_MODE,
				#if(model_Iris_DC)
					({if(MP(MpIris)==MN_LENS_MNL) {
							MENU_STR(MN_ON, MP(MpShtMode), 2, NORMAL, DEBLUR)				// CAUTION ! -> MENU_STR
					}else{	//MP(MpIris) == MN_LENS_DC
							MENU_STR(MN_ON, MP(MpDcMode), 3, INDOOR, OUTDOOR, DEBLUR)} }),
				#else
							MENU_STRn(MN_ON, , MP(MpShtMode), 2, NORMAL, DEBLUR ),			// CAUTION ! -> MENU_STRn
				#endif
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
#endif


	// MENU - EXPOSURE - SHUTTER:MANUAL
	MENU_SET( 2, SHUTTER_MANUAL, MP(MpShutter)==MN_SHUT_MNL, MN_OFF,
			SPEED,			MENU_STR_SHUTTER_SPEED(MP(MpShutSpd),),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - BACKLIGHT:HLC
	MENU_SET( 3, HLC, MP(MpBackLight) == MN_BL_HLC, MN_OFF,
			LEVEL,			MENU_BARn(MN_ON, , MP(MpHlMaskThrs), 0, 20, 1),
			COLOR,			MENU_STRn(MN_ON, , MP(MpHlMaskColor), 9, WHT, YEL, CYN, GRN, MAG, RED, BLU, BLK, CUSTOMIZE),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - BACKLIGHT:BLC
	MENU_SET( 6, BLC, MP(MpBackLight) == MN_BL_BLC || ((MP(MpWdrBlc)!=MN_OFF) && WDR_MODE), MN_OFF,
			MODE,			MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()), MP(MpBlcMode), 3, CUSTOMIZE, CENTER, SPOT),
			HPOS,			MENU_VARn(MP(MpBlcMode)==MN_BLC_MODE_CUSTOMIZE, , MP(MpBlcPosX), 0, 20, 1, ),
			VPOS,			MENU_VARn(MP(MpBlcMode)==MN_BLC_MODE_CUSTOMIZE, , MP(MpBlcPosY), 0, 20, 1, ),
			HSIZE,			MENU_VARn(MP(MpBlcMode)==MN_BLC_MODE_CUSTOMIZE, , MP(MpBlcSizX), 0, 20, 1, ),
			VSIZE,			MENU_VARn(MP(MpBlcMode)==MN_BLC_MODE_CUSTOMIZE, , MP(MpBlcSizY), 0, 20, 1, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 7, WDR_ADJUST, MN_ON, MN_OFF,
			WEIGHT,			MENU_STRn(MN_ON, , MP(MpFrameRateLv), 3, LOW, MIDDLE, HIGH),
			Ap_BRIGHT_L,	MENU_VARn(MN_ON, if_KEY_LR(ParSet(UpAE_WDR_LTGT_OFST2, MP(MpWdrABrt))), MP(MpWdrABrt), 0, 255, 1, ),
			Ap_BRIGHT_S,	MENU_VARn(MN_ON, if_KEY_LR(ParSet(UpAE_WDR_STGT_OFST2, MP(MpWdrABrtS))), MP(MpWdrABrtS), 0, 255, 1, X4 ),
			Dp_BRIGHT,		MENU_BARn(MN_ON, if_KEY_LR(ParSet(UpACEWDR1_TH, MP(MpWdrDBrt))), MP(MpWdrDBrt), 0, 255, 1 ),
			Dp_CONTRAST,	MENU_BARn(MN_ON, if_KEY_LR(ParSet(UpACEWDR2_TH, MP(MpWdrDCont))), MP(MpWdrDCont), 0, 255, 1 ),
			GAMMA,			MENU_STRi(MN_ON, MENU_VAL_IS(AUTOe), AUTO_GAMMA, , , MP(MpGammaWdr), 9, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075, AUTOe, DEFAULT),
		 	RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - BACKLIGHT:WDR 	// 161010 LGH : Add ROI WDR mode 
//		MENU_SET( 2, WDR, MP(MpBackLight) == MN_BL_WDR, MN_OFF,
//				WEIGHT,			MENU_STRn(MN_ON, , MP(MpWdrWgt), 3, LOW, MIDDLE, HIGH),
//			 	RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

#if model_WDR_ROI
	// MENU - BACKLIGHT:WDR 	// 161010 LGH : Add ROI WDR mode 
	MENU_SET( 3, WDR, MP(MpBackLight) == MN_BL_WDR, MN_OFF,
			MODE,			MENU_STRi(MN_ON, MENU_VAL_IS(ROI_WDRe), ROI_WDR, MENU_OFF_GRAY_ONLY(); gbROIWdrBoxFlag = 1;, , MP(MpROIWdrMode), 2, NORMAL, ROI_WDRe),
			WEIGHT,			MENU_STRn(MN_ON, , MP(MpWdrWgt), 3, LOW, MIDDLE, HIGH),
		 	RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// WDR Window Mode Setting
	MENU_SET( 7, ROI_WDR, MN_ON, MN_OFF,
			WINDOWZONE,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpROIWdrZone), 0, 3, 1,),
			WINDOWUSE,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction, 2, OFF, ON),
			HPOS,			MENU_VARe(gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction, , gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosX_Lo, gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosX_Up, 0, gnHWMax),
			VPOS,			MENU_VARe(gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction, , gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosY_Lo, gbMnROIWdrCfg(MP(MpROIWdrZone))->bPosY_Up, 0, gnVWMax),
			HSIZE,			MENU_VARe(gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction, , gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizX_Lo, gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizX_Up, 0, gnHWMax),
			VSIZE,			MENU_VARe(gbMnROIWdrCfg(MP(MpROIWdrZone))->bAction, , gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizY_Lo, gbMnROIWdrCfg(MP(MpROIWdrZone))->bSizY_Up, 0, gnVWMax),							  	
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, gbROIWdrBoxFlag = 0;))
#endif

	MENU_SET( 6, DAYnNIGHT, MP(MpTdn) == MN_TDN_AUTO, MN_OFF,
			IR_LED,			MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()) , MP(MpLedOn), 2, OFF, ON),
		#if model_LED_ASAT
			ANTI_SAT,		MENU_BARn((MP(MpLedOn)==MN_ON), , MP(MpLedSatCtrl), 0, 20, 1),	// 141120 LH
		#else
			ANTI_SAT,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), ANTI_SAT, , , MP(MpAntiSatOn), 2, OFF, ONe),
		#endif
			AGC_THRES,		MENU_BARn(MN_ON, , MP(MpTdnThrs), 0, 20, 1),
			AGC_MARGIN,		MENU_BARn(MN_ON, , MP(MpTdnMarg), 0, 20, 1),
			DELAY,			MENU_STRn(MN_ON, , MP(MpTdnDly), 3, LOW, MIDDLE, HIGH),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 3, DAYnNIGHT, MP(MpTdn) == MN_TDN_NIGHT, MN_OFF,
			IR_LED,			MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()) , MP(MpLedOn), 2, OFF, ON),
		#if model_LED_ASAT
			ANTI_SAT,		MENU_BARn((MP(MpLedOn)==MN_ON), , MP(MpLedSatCtrl), 0, 20, 1),	// 141120 LH
		#else
			ANTI_SAT,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), ANTI_SAT, , , MP(MpAntiSatOn), 2, OFF, ONe),
		#endif
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 7, DAYnNIGHT, MP(MpTdn) == MN_TDN_EXTERN, MN_OFF,
			IR_LED,			MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()) , MP(MpLedOn), 2, OFF, ON),
		#if model_LED_ASAT
			ANTI_SAT,		MENU_BARn((MP(MpLedOn)==MN_ON), , MP(MpLedSatCtrl), 0, 20, 1),	// 141120 LH
		#else
			ANTI_SAT,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), ANTI_SAT, , , MP(MpAntiSatOn), 2, OFF, ONe),
		#endif
			EXTERN_SW, 		MENU_STRn(MN_ON, , MP(MpTdnSw), 2, HIGH, LOW), 				// HIGH : Day if CDS level is HIGH, LOW : Day if CDS level is LOW
			DN_THRES,		MENU_BARn(MN_ON, , MP(MpTdnDNThres), 0, 20, 1),
			ND_THRES,		MENU_BARn(MN_ON, , MP(MpTdnNDThres), 0, 20, 1),
			DELAY,			MENU_STRn(MN_ON, , MP(MpTdnDly), 3, LOW, MIDDLE, HIGH),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 9, COLORm, (gbMnSetupOn==1), MN_OFF,
			AWB,			MENU_STRi(MN_ON, MENU_VAL_IS(MANUALe), AWB, ,
										MENU_VAL_PUSH(PRESETp, PUSHING, PUSH_DELAY_NOR, MP(MpAwbPrst) = MN_OFF, MP(MpAwbPrst) = MN_ON),
										MP(MpAwb), 4, AUTO, AUTOext, PRESETp, MANUALe),
			AWB_STYLE,		MENU_STRn(MN_ON, if_KEY_LR(Hue_Chroma_Menu_Set(0)), MP(MpAwbStyle), 6, A_TYPE, B_TYPE, C_TYPE, D_TYPE, E_TYPE, F_TYPE),
//				COLORGAIN,		MENU_BARn(MN_ON, , MP(MpSaturation), 0, 20, 1 ),
			R_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturationR), 0, 40, 1 ),	// 2017419 - WHL : CBB TEST
			G_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturation), 0, 40, 1 ),
			B_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturationB), 0, 40, 1 ),
			AWB_LSUP,		MENU_ONEi(MN_ON, e, MN_ON, AWB_LSUP, ),
			AWB_SPEED,		MENU_BARn(MN_ON, if_KEY_LR(ParSet(UpAWB_SPEED_MTRX, MP(MpAwbSpeed))), MP(MpAwbSpeed), 0, 255, 1 ),
			HUE_CHROMA_CTRL,MENU_ONEi(MN_ON, e, MN_ON, HUE_CHROMA_CTRL,	),

			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
			
	MENU_SET( 6, COLORm, (gbMnSetupOn==0), MN_OFF,
			AWB,			MENU_STRi(MN_ON, MENU_VAL_IS(MANUALe), AWB, ,
										MENU_VAL_PUSH(PRESETp, PUSHING, PUSH_DELAY_NOR, MP(MpAwbPrst) = MN_OFF, MP(MpAwbPrst) = MN_ON),
										MP(MpAwb), 4, AUTO, AUTOext, PRESETp, MANUALe),
			AWB_STYLE,		MENU_STRn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(0)), MP(MpAwbStyle), 6, A_TYPE, B_TYPE, C_TYPE, D_TYPE, E_TYPE, F_TYPE),
//				COLORGAIN,		MENU_BARn(MN_ON, , MP(MpSaturation), 0, 20, 1 ),
			R_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturationR), 0, 40, 1 ),	// 2017419 - WHL : CBB TEST
			G_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturation), 0, 40, 1 ),
			B_GAIN,			MENU_BARn(MN_ON, , MP(MpSaturationB), 0, 40, 1 ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET(9 ,HUE_CHROMA_CTRL,MN_ON,MN_OFF,
			Bn_CHROMA,		MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(2)) , MP(MpgainBn), 0, 255, 1 ),
			Bp_CHROMA,		MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(2)) , MP(MpgainBp), 0, 255, 1 ),
			Rn_CHROMA,		MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(2)) , MP(MpgainRn), 0, 255, 1 ),
			Rp_CHROMA,		MENU_BARn(MN_ON, if_KEY_LR(Hue_Chroma_Menu_Set(2)), MP(MpgainRp), 0, 255, 1 ),
			Y_HUE,			MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(3)) , MP(MphueBn), 0, 255, 1 ),
			B_HUE,			MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(3)) , MP(MphueBp), 0, 255, 1 ),
			G_HUE,			MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(3)), MP(MphueRn), 0, 255, 1 ),
			R_HUE,			MENU_BARn(MN_ON,if_KEY_LR(Hue_Chroma_Menu_Set(3)) , MP(MphueRp), 0, 255, 1 ),
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - COLOR - AWB_LSUP
	MENU_SET( 5, AWB_LSUP, MN_ON, MN_OFF,
			//HW_CES,			MENU_STRn(MN_ON, , MP(MpCES_ON), 2, OFF, ON),
			HW_CES_NOR,		MENU_BARn(MN_ON, , MP(MpCES_ON), 0, 40, 1 ),
			HW_CES_WDR,		MENU_BARn(MN_ON, , MP(MpWdrCES_ON), 0, 40, 1 ),
			HW_LSUP,		MENU_STRn(MN_ON, if_KEY_LR(ParSet(UpLSUP_ON, MP(MpLSUP_ON)) MENU_REDRAW()), MP(MpLSUP_ON), 2, OFF, ON),
			//LSUP_WEIGHT,	MENU_BARn(MP(MpLSUP_ON)==0, if_KEY_LR(ParSet(UpAWB_AGC_WGT_MIN, MP(MpAwbAgcWgtMin))), MP(MpAwbAgcWgtMin), 0, 255, 1 ),
			//AWB_YSLICE_MIN,	MENU_BARn(MN_ON, if_KEY_LR(MENU_REDRAW()), MP(MpAwbYSliceMin), 3, MP(MpAwbYSliceMax), 1 ),
			//AWB_YSLICE_MAX,	MENU_BARn(MN_ON, if_KEY_LR(ParSet(UpAWB_YSLICE, MP(MpAwbYSliceMax)) MENU_REDRAW()), MP(MpAwbYSliceMax), MP(MpAwbYSliceMin), 0x40/*UpAWB_YSLICE*/, 1 ),
			//AWB_SLICE_MIN,	MENU_BARn(MP(MpLSUP_ON)==0, if_KEY_LR(MENU_REDRAW()), MP(MpAwbSliceCntMin), 0, MP(MpAwbSliceCntMax), 1 ),
			//AWB_SLICE_MAX,	MENU_BARn(MP(MpLSUP_ON)==0, if_KEY_LR(MENU_REDRAW()), MP(MpAwbSliceCntMax), MP(MpAwbSliceCntMin), 16, 1 ),
			REDp_BY_AGC,	MENU_ONEi(MN_ON, e, MN_ON, REDUCE_BY_AGC, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - WHITE BAL:MANUAL
	MENU_SET( 4, AWB, MP(MpAwb) == MN_AWB_MNL, MN_OFF,
			C_TEMP,			MENU_STRn(MN_ON, , MP(MpAwbMnl), 3, TEMP1, TEMP2, TEMP3),
			RGAIN,			MENU_BARn(MN_ON, , MP(MpRgain), 0, 20, 1),
			BGAIN,			MENU_BARn(MN_ON, , MP(MpBgain), 0, 20, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE
#if (IMAGE_INPUT_MODE==1)
	MENU_SET( 10, IMAGE, MN_ON, MN_OFF,
#else
	MENU_SET( 9, IMAGE, MN_ON, MN_OFF,
#endif
//		MENU_SET(11, IMAGE, MN_ON, MN_OFF,
			SHARPNESS,		//MENU_BARn(MN_ON, , MP(MpSharpness), 0, 10, 1 ),
							({menu_bar(MN_ON, &MP(MpSharpness), 0, 10, 1, 0);
							if(gbMnSetupOn==1){
								MENU_IN(MN_ON, SHARPNESS, )
								SETFONTID(DRAW_Y, MN_SXSP+10, 0xa6);
							}}),

			//GAMMA,			MENU_STRn(MN_ON, , MP(MpGamma), 7, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075),
			//GAMMA, 			MENU_ONEi(MN_ON, e, MN_ON, GAMMA, ),
			GAMMA,			MENU_STRi(MN_ON, MENU_VAL_IS(AUTOe), AUTO_GAMMA, , , MP(MpGamma), 8, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075, AUTOe),

			MIRROR,			MENU_STRn(MN_ON, , MP(MpMirror), 2, OFF, ON),
			FLIP,			MENU_STRn(MN_ON, , MP(MpFlip), 2, OFF, ON),

			DWDR/*ACE*/,	//MENU_STRn(MN_ON, , MP(MpAce), 4, OFF, LOW, MIDDLE, HIGH),
							({MENU_STR(MN_ON, MP(MpAce), 4, OFF, LOW, MIDDLE, HIGH)
							if(gbMnSetupOn==1){
								MENU_IN(MN_ON, DWDR, )
								SETFONTID(DRAW_Y, MN_SXSP+6, 0xa6);
							}}),

			DEFOG,			MENU_STRi(MN_ON, MENU_VAL_IS(ONe), DEFOG, , , MP(MpDefog), 2, OFF, ONe),
			SHADING,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), SHADING, , , MP(MpShading), 2, OFF, ONe),
			PRIVACY,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), PRIVACY, MENU_OFF_GRAY_ONLY(), , MP(MpPrivacy), 2, OFF, ONe),
#if (IMAGE_INPUT_MODE==1)
			PIP,			MENU_STRi(MN_ON, MENU_VAL_IS(ONe), PIP, , , MP(MpPipOn), 2, OFF, ONe),
#endif
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - SHARPNESS
	MENU_SET( 5, SHARPNESS, MN_ON, MN_OFF,
			SHARPNESS,		MENU_BARn(MN_ON, , MP(MpSharpness), 0, 10, 1 ),
			SMALL_EDGE,		MENU_BARn(MN_ON, , MP(MpShpSmallEdge), 0, 255, 1),
			BIG_EDGE,		MENU_BARn(MN_ON, , MP(MpShpBigEdge),   0, 255, 1),
			REDp_BY_AGC,	MENU_ONEi(MN_ON, e, MN_ON, REDUCE_BY_AGC, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - ACE
	MENU_SET( 6, DWDR, MN_ON, MN_OFF,
			DWDR/*ACE*/,	MENU_STRn(MN_ON, , MP(MpAce), 4, OFF, LOW, MIDDLE, HIGH),
			WEIGHT,			MENU_BARn(MN_ON, , MP(MpAceGmgn), 0, 255, 1),
			Dp_BRIGHT,		MENU_BARn(MN_ON, , MP(MpAceBrt), 0, 64, 1),
			Dp_CONTRAST,	MENU_BARn(MN_ON, , MP(MpAceCont), 0, 64, 1),
			REDp_BY_AGC,	MENU_ONEi(MN_ON, e, MN_ON, REDUCE_BY_AGC, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - GAMMA
	MENU_SET( 3, AUTO_GAMMA, MN_ON, MN_OFF,
			DAY,			MENU_STRn(MN_ON, , MP(MpGammaDay), 7, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075),
			NIGHT,			MENU_STRn(MN_ON, , MP(MpGammaNgt), 7, GAMMA_045, GAMMA_050, GAMMA_055, GAMMA_060, GAMMA_065, GAMMA_070, GAMMA_075),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - DEFOG
	MENU_SET( 6, DEFOG, MN_ON, MN_OFF,
			MODE,			MENU_STRn(MN_ON, , MP(MpDefogMode), 2, MANUAL, AUTO),
			LEVEL,			MENU_STRn(MN_ON, , MP(MpDefogLevel), 3, LOW, MIDDLE, HIGH),
			Dp_BRIGHT,		MENU_BARn(MN_ON, , MP(MpAceBrt), 0, 64, 1),
			Dp_CONTRAST,	MENU_BARn(MN_ON, , MP(MpAceCont), 0, 64, 1),
			REDp_BY_AGC,	MENU_ONEi(MN_ON, e, MN_ON, REDUCE_BY_AGC, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - SHADING
	MENU_SET( 2, SHADING, MN_ON, MN_OFF,
			WEIGHT,			MENU_VARn(MN_ON, , MP(MpShdWeight), 0, 200, 1, PERCENT/*%*/),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - IMAGE - PRIVACY
	MENU_SET( 12, PRIVACY, MN_ON, MN_OFF,
			ZONENUM,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpPvcZone), 0, PVC_EA-1, 1, ),
			ZONEDISP,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnPvcCfg(MP(MpPvcZone))->bAction, 2, OFF, ON),
			HPOS,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bPosX, 0, BOX_XMAX, 0, ),
			VPOS,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bPosY, 0, BOX_YMAX, 0, ),
			HSIZE,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bSizX, 0, BOX_XMAX, 0, ),
			VSIZE,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bSizY, 0, BOX_YMAX, 0, ),
			PRV_MASKMOD,	MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpPvcFormat), 2, PRV_FILL, PRV_FORMAT),
			YLEVEL, 		MENU_BARn(MN_ON, , MP(MpPvcYLevel), 0, UP(UpBOX_COLOR_STEP), 1),
			CBLEVEL,		MENU_BARn(MN_ON, , MP(MpPvcCBLevel),0, UP(UpBOX_COLOR_STEP), 1),
			CRLEVEL,		MENU_BARn(MN_ON, , MP(MpPvcCRLevel),0, UP(UpBOX_COLOR_STEP), 1),
			TRANS,			MENU_VARn(MN_ON, , MP(MpPvcTrans), 0, 3, 1, ),
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, ))

#if (IMAGE_INPUT_MODE==1)
	// MENU - IMAGE - PIP
	MENU_SET( 5, PIP, MN_ON, MN_OFF,
//				PIP_CH,			MENU_STRn(MN_ON, , MP(MpPipCH), 3, PIP_MAIN, PIP_BT1120, PIP_BT656),	// 201728 - WHL : BT656 Input Only
			COLOR,			MENU_STRn(MN_ON, , MP(MpPipColor), 9, WHT, YEL, CYN, GRN, MAG, RED, BLU, BLK, CUSTOMIZE),
			HPOS,			MENU_VARn(MN_ON, , MP(MpPip1_PosX), 0, 0xFF, 0, ),
			VPOS,			MENU_VARn(MN_ON, , MP(MpPip1_PosY), 0, 0xFF, 0, ),
			HSIZE,			MENU_VARn(MN_ON, , MP(MpPip1_SizX), 0, 100, 0, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
#endif

	// MENU - MOTION
	MENU_SET( 8, MOTION, MN_ON, MN_OFF,
			DET_WINDOW,		MENU_ONEi(MN_ON, e, MN_ON, DET_WINDOW, MENU_OFF_GRAY_ONLY(); IMD_MASK_SEL_ON;),
			DET_TONE,		MENU_VARn(MN_ON, , MP(MpItlDettone), 0, 4, 1, ),
			MDRECT_FILL,	MENU_STRn(MN_ON, ,MP(MpItlRectFill), 2, OFF, ON),
			SENSITIVITY, 	MENU_BARn(MN_ON, ,MP(MpItlSens),0, IMD_SENS_MAX, 1),
			MOTION_OSD, 	MENU_STRn(MN_ON, ,MP(MpItlMaskOsd), 2, OFF, ON),
			TEXT_ALARM, 	MENU_STRn(MN_ON, ,MP(MpItlAlarm), 2, OFF, ON),
			SIGNAL_OUT, 	MENU_STRn(MN_ON, ,MP(MpItlSigno), 2, OFF, ON),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON,))

	MENU_SET( 7, DET_WINDOW, MN_ON, MN_OFF,
			WINDOWZONE,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpItlWindowuse), 0, MASK_EA-1, 1,),
			WINDOWUSE,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnImdCfg(MP(MpItlWindowuse))->bAction, 2, OFF, ON),
			DET_H_POS, 		MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bPosX, 0, gnIMD_Hw, 0, ),
			DET_V_POS, 		MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bPosY, 0, gnIMD_Vw, 0, ),
			DET_H_SIZE, 	MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bSizX, 0, gnIMD_Hw, 0, ),
			DET_V_SIZE, 	MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bSizY, 0, gnIMD_Vw, 0, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, IMD_MASK_SEL_OFF; ))
			

#if (model_2M30p)
	#define MENU_STR_FRAME_MODE_50	MENU_STR(MN_ON, MP(FrameMode), 3, 25P_NORMAL, 12P_WDR, 25P_DNR)
	#define MENU_STR_FRAME_MODE_60	MENU_STR(MN_ON, MP(FrameMode), 3, 30P_NORMAL, 15P_WDR, 30P_DNR)
#else	// (model_1M)|(model_2M)
	#define MENU_STR_FRAME_MODE_50	MENU_STR(MN_ON, MP(FrameMode), 6, 25P_NORMAL, 12P_WDR, 25P_DNR, 50P_NORMAL, 25P_WDR, 50P_DNR)
	#define MENU_STR_FRAME_MODE_60	MENU_STR(MN_ON, MP(FrameMode), 6, 30P_NORMAL, 15P_WDR, 30P_DNR, 60P_NORMAL, 30P_WDR, 60P_DNR)
#endif

	// MENU - OUTPUTSET
	MENU_SET( 4, OUTPUTSET, MN_ON, MN_OFF,	
			MODE,			
							({if(MP(MpSysFreq) == MN_SYSFREQ_50){
								MENU_STR_FRAME_MODE_50		MENU_CODE(if_KEY_LR(FrameRateLvSet(); MENU_REDRAW();))
							}
							else{
								MENU_STR_FRAME_MODE_60		MENU_CODE(if_KEY_LR(FrameRateLvSet(); MENU_REDRAW();)) }} ),

			LEVEL,			//MENU_STRn(MN_ON, , MP(MpFrameRateLv), 3, LOW, MIDDLE, HIGH),
							({	if(WDR_MODE) {
									MENU_STR(MN_ON, MP(MpFrameRateLv), 3, LOW, MIDDLE, HIGH)	MENU_NAME_REDRAW(WDR)
									if(gbMnSetupOn==1) {
										MENU_IN(MN_ON, WDR_ADJUST, )
										SETFONTID(DRAW_Y, MN_SXSP+6, 0xa6);
									}
								}
								else if(DNR_MODE) {
									MENU_STR(MN_ON, MP(MpFrameRateLv), 3, LOW, MIDDLE, HIGH)	MENU_NAME_REDRAW(DNR)
									if(gbMnSetupOn==1) {
										MENU_IN(MN_ON, DNR/*REDUCE_BY_AGC*/, MENU_OFF_GRAY_ONLY())
										SETFONTID(DRAW_Y, MN_SXSP+6, 0xa6);
									}
								}
								else {
									MENU_ONEn(MN_OFF, OFF)		MENU_NAME_REDRAW(LEVEL)
								}
							}),

			FREQ,			MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW()), MP(MpSysFreq), 2, 50HZ, 60HZ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
			
		// MENU - OUTPUTSET - DNR
	MENU_SET( 7, DNR, MN_ON, MN_OFF,
			WEIGHT,			MENU_STRn(MN_ON, , MP(MpFrameRateLv), 3, LOW, MIDDLE, HIGH),
			INCRpBY_AGC,	MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY()), MP(MpDnrIncrease), 2, OFF, ON),
			//REDp_BY_AGC,	MENU_ONEi(MN_ON, e, MN_ON, REDUCE_BY_AGC, ),
			LOW_AGC,		MENU_BARn(MP(MpDnrIncrease)==MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcLow),  0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			MID_AGC,		MENU_BARn(MP(MpDnrIncrease)==MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcMid),  0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			HIGH_AGC,		MENU_BARn(MP(MpDnrIncrease)==MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcHigh), 0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			AGC_MAX_,		MENU_BARn(MN_ON, , MP(MpAgc), 0, 255, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))
			

	// MENU - SYSTEM
#if (DOOR_BELL_CAMERA==1)
	MENU_SET( 6, SYSTEM, (gbMnSetupOn==0), MN_OFF,
			DC_LP_MODE,		MENU_STRn(MN_ON, , MP(MpDoorCam_LP_Mode), 6, OFF, TIME_10S, TIME_15S, TIME_20S, TIME_25S, TIME_30S),
#else
	MENU_SET( 5, SYSTEM, (gbMnSetupOn==0), MN_OFF,
#endif
			//COM,			MENU_ONEi(MN_ON, e, MN_ON, COM, gbBaudrate = MP(MpBaudrate); gbBaudrateSetDone = 0; ),
			CAMTITLE,		MENU_STRi(MN_ON, MP(MpCamTitleOn)!=MN_CT_OFF, CAMTITLE, , if_KEY_LR(OsdCamTitle();), MP(MpCamTitleOn), 3, OFF, RIGHT_UPe, LEFT_DOWNe),
			LANGUAGE,		MENU_STRn(MN_ON, if_KEY_LR(SetFontChg(); MENU_CHANGE()), MP(MpLanguageSel), 5, ENG, CHN, CHNs, JPN, KOR),
			COLORBAR,		MENU_STRn(MN_ON, , MP(MpCBar), 2, OFF, ON),
			RESET,			MENU_PUSH(MN_ON, MP(MpReset), ONp, PUSHING, PUSH_DELAY_NOR, MenuReset(); SetFontChg(); MENU_CHANGE(); ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))


#if (DOOR_BELL_CAMERA==1)
	MENU_SET( 10, SYSTEM, (gbMnSetupOn==1), MN_OFF,
			DC_LP_MODE, 	MENU_STRn(MN_ON, , MP(MpDoorCam_LP_Mode), 6, OFF, TIME_10S, TIME_15S, TIME_20S, TIME_25S, TIME_30S),
#else
	MENU_SET( 9, SYSTEM, (gbMnSetupOn==1), MN_OFF,
#endif
			DEVELOPER,		MENU_STRn(MN_ON, if_KEY_LR(MENU_CHANGE()), gbMnSetupOn, 2, OFF, ON),
			STYLE,			MENU_STRn(MN_ON, if_KEY_LR(MenuStyleSet(gbMenuStyle); MENU_CHANGE()), gbMenuStyle, 8, PREVIOUS, IPC_INDOOR, IPC_OUTDOOR, CAR_REC, ACTION_REC, WDR, LOW_BIT, CUSTOM),
			//COM,			MENU_ONEi(MN_ON, e, MN_ON, COM, gbBaudrate = MP(MpBaudrate); gbBaudrateSetDone = 0; ),
			CAMTITLE,		MENU_STRi(MN_ON, MP(MpCamTitleOn)!=MN_CT_OFF, CAMTITLE, , if_KEY_LR(OsdCamTitle();), MP(MpCamTitleOn), 3, OFF, RIGHT_UPe, LEFT_DOWNe),
			LANGUAGE,		MENU_STRn(MN_ON, if_KEY_LR(SetFontChg(); MENU_CHANGE()), MP(MpLanguageSel), 5, ENG, CHN, CHNs, JPN, KOR),
			COLORBAR,		MENU_STRn(MN_ON, , MP(MpCBar), 2, OFF, ON),
			SHADINGDET, 	MENU_ONEo(MN_ON, ONp, MN_ON,
											MP(MpShdDet) = 1;
											MP(MpShading) = 0;
											MENU_CLOSE();
											goto menu_start;
											),
			CAMVERSION, 	MENU_DIRn(MN_ON
							/* MENU_DIRn:Pre Code */,
							/* MENU_DIRn:Value Change */,
							/* MENU_DIRn:Display Code */,
							static char bVal1[11] = "0000(0000)";
							uint2strh(bVal1, model_ver, 4);
							uint2strh(bVal1+5, gDataID, 4);
							bVal1[4] = '(';
							bVal1[9] = ')';
							bVal1[10] = 0;
							DispStr(bVal1, DRAW_Y, MN_SXSP, 10);
							/* MENU_DIRn:End Code */,),
			RESET,			MENU_PUSH(MN_ON, MP(MpReset), ONp, PUSHING, PUSH_DELAY_NOR, MenuReset(); SetFontChg(); MENU_CHANGE(); ),
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - SYSTEM - COM.
	MENU_SET( 4, COM, MN_ON, MN_OFF,
			CAMID,			MENU_VARn(MN_ON, , MP(MpCamID), 0, 255, 1, ),
			BAUDRATE,		MENU_STRn(MN_ON, if_KEY_LR(/*UartBaudRate()*/), gbBaudrate, 5, 2400, 4800, 9600, 57600, 115200),
			SET_DONE,		MENU_PUSH(MN_ON, gbBaudrateSetDone, ONp, PUSHING, PUSH_DELAY_NOR, MP(MpBaudrate) = gbBaudrate; UartBaudRate();),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - SYSTEM - COM. - CAMTITLE
	MENU_EXT( CAMTITLE, MN_ON, MN_ON, giMenuNum = 8;/*나가기 전에 SubMenu 7번째 라인까지 지움*/,

			OsdAttrsStgPosAr(1, MN_WHITE);
			OsdAttrsStgPosAr(2, MN_WHITE);
			OsdAttrsStgPosAr(3, MN_WHITE);

			SETFONTID(gbMenuY+1, MN_XTITL, 162);						// Display Arrow-down

			SETFONTATTR(gbMenuY+2, MN_XTITL, MN_YELLOW);				// Display Cam-title
			DispStr((const char*)&gbCamTitle(0), gbMenuY+2, MN_XTITL, 0x8);			//		"

			SETFONTATTR(gbMenuY+1, MN_XTITL, MN_YELLOW);				// Display line
			DrawStr(TLx8, gbMenuY+3, MN_XTITL, 8);

			//---------------------------------------------------------
			OsdAttrsStgPosAr(5, MN_WHITE);
			OsdAttrsStgPosAr(6, MN_WHITE);
			OsdAttrsStgPosAr(7, MN_WHITE);

			DrawStr(UPsDN_d_CHAR_SELECT,gbMenuY+5, MN_MXSP+3, 19);		// Display UP/DN Instruction
			DrawStr(LEsRI_d_POSITION,	gbMenuY+6, MN_MXSP+3, 16);		// Display LE/RI Instruction
			DrawStr(ENTER_d_RETURN,		gbMenuY+7, MN_MXSP+3, 14);		// Display ENTER Instruction
	)

#if model_ExitMenu == 2
	MENU_SET( 1, SAVEnEXIT, MN_ON, MN_OFF,
			SAVEnEXIT,		MENU_STRo(MN_ON, MN_ON,
								//const BYTE bSetFontChgOn = (gbMnParTbl[MpLanguageSel] != gbMnParTblOld[MpLanguageSel]) && (gbMnExit == 1);
								MenuSave(gbMnExit == 0);
								//if(bSetFontChgOn) { SetFontChg(); }
								gbMnExit = 0;
								, , gbMnExit, 2, CONFIRMe, CANCELe))
#endif

// Setup MENU --------------------------------------------------------------------------------

	MENU_SET(5, SETUP, MN_ON, MN_OFF,
			DEVELOPER,		MENU_STRn(MN_ON, , gbMnSetupOn, 2, OFF, ON),
			STYLE,			MENU_STRn(MN_ON, if_KEY_LR(MenuStyleSet(gbMenuStyle); MENU_CHANGE()), gbMenuStyle, 8, PREVIOUS, IPC_INDOOR, IPC_OUTDOOR, CAR_REC, ACTION_REC, WDR, LOW_BIT, CUSTOM),
  			SHADINGDET,		MENU_ONEo(MN_ON, ONp, MN_ON,
											MP(MpShdDet) = 1;
											MP(MpShading) = 0;
											MENU_CLOSE();
											goto menu_start;
											),
			CAMVERSION,		MENU_DIRn(MN_ON
							/* MENU_DIRn:Pre Code */,
							/* MENU_DIRn:Value Change */,
							/* MENU_DIRn:Display Code */,
							static char bVal1[11] = "0000(0000)";
							uint2strh(bVal1, model_ver, 4);
							uint2strh(bVal1+5, gDataID, 4);
							bVal1[4] = '(';
							bVal1[9] = ')';
							bVal1[10] = 0;
							DispStr(bVal1, DRAW_Y, MN_SXSP, 10);
							/* MENU_DIRn:End Code */,),
			RETURN,			MENU_ONEo(MN_ON, , MN_ON, MenuSave(MN_ON); ))

	MENU_SET( 5, REDUCE_BY_AGC, MN_ON, MN_OFF,
			LOW_AGC,		MENU_BARn(MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcLow),  0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			MID_AGC,		MENU_BARn(MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcMid),  0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			HIGH_AGC,		MENU_BARn(MN_ON, if_KEY_LR(IspAgcSet()), MP(MpShpAgcHigh), 0, 99, 1)	SETFONTID(DRAW_Y, MN_SXSP+2, '%'),
			AGC_MAX_,		MENU_BARn(MN_ON, , MP(MpAgc), 0, 255, 1),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, ))

  #if(model_Lens==1)
	MENU_SET(10, MOTOR_SETUP, MN_ON, MN_OFF,
//			MAF_LENS_SEL,		MENU_VARn(MN_ON, , MP(MpMAF_LensModel), 0, 9, 1, ),
			MAF_LENS_SEL,		MENU_STRn(MN_ON, , MP(MpMAF_LensModel), 6, MAF_LENS_YT2812, MAF_LENS_YT3013, MAF_LENS_YT3017, MAF_LENS_YT30022, MAF_LENS_YT3021, MAF_LENS_CSTM),
			MAF_DEVMODE,		MENU_STRn(MN_ON, , gbMAF_DevMode, 2, OFF, ON),
			MAF_TESTMODE,		MENU_STRn(MN_ON, , MP(MpMAF_TestMode), 2, OFF, ON),
			AF_MODE,			MENU_STRn(MN_ON, if((MP(MpMAF_mode)==0)&&KEY_C) gbAutoFocus_Flag=1;, MP(MpMAF_mode), 3, ONESHOTp, MANUAL, MAF_DISABLE),
			DnN_FILTER,			MENU_STRn(MN_ON, , MP(MpMAF_Filter), 2, SYNC_OFF, SYNC_ON),
			MAF_ZOOM_AREA,		MENU_ONEi(MN_ON, e, MN_ON, MAF_ZOOM_AREA, ),
			MAF_FOCUS_AREA,		MENU_ONEi(MN_ON, e, MN_ON, MAF_FOCUS_AREA, ),
			INITIAL,			MENU_PUSH(MN_ON, MP(MpMAF_MotorInit), ONp, PUSHING, PUSH_DELAY_NOT/*PUSH_DELAY_NOR*/, ),
			MAF_AUTO_TRACE,		MENU_PUSH(MN_ON, gbMAF_Calibration, ONp, PUSHING, PUSH_DELAY_NOT/*PUSH_DELAY_NOR*/, ),
			RETURN,				MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 4, MAF_ZOOM_AREA, MN_ON, MN_OFF,
			MAF_TOTAL_AREA,		MENU_VARn(MN_ON, , MP(MpMAF_TotalPos_Z), 0, 255, 1, ),
			MAF_START_POS,		MENU_VARn(MN_ON, , MP(MpMAF_StartPos_Z), 0, 255, 1, ),
			MAF_END_POS,		MENU_VARn(MN_ON, , MP(MpMAF_EndPos_Z), 0, 255, 1, ),
			RETURN,				MENU_ONEo(MN_ON, e, MN_ON, ))

	MENU_SET( 4, MAF_FOCUS_AREA, MN_ON, MN_OFF,
			MAF_TOTAL_AREA,		MENU_VARn(MN_ON, , MP(MpMAF_TotalPos_F), 0, 255, 1, ),
			MAF_START_POS,		MENU_VARn(MN_ON, , MP(MpMAF_StartPos_F), 0, 255, 1, ),
			MAF_END_POS,		MENU_VARn(MN_ON, , MP(MpMAF_EndPos_F), 0, 255, 1, ),
			RETURN,				MENU_ONEo(MN_ON, e, MN_ON, ))

		#endif


// Debug MENU --------------------------------------------------------------------------------

	MENU_SET(7, DEBUG, MN_ON, MN_OFF,
			FUNCTION,		MENU_STRn(MN_ON, if_KEY_LR(OsdWhiteAll(); OsdClearAll(); ChangeMenuSize(); OsdCamTitle(); MENU_CHANGE()) , gbMnDebugFnc, 9, OFF, AE, AF, AWB, DBG_4, DBG_5, IMD_DBG, TDN_DBG, STATUS),
			BYPASS,			MENU_STRn(MN_ON, 
								//if_KEY_LR(if(gbMnDebugBypass) { TIM_CH3_EN = 0; } else { TIM_CH3_EN = 1; } )
								, gbMnDebugBypass, 2, OFF, ON),
			UP_RESET,		({ if(gbPushCnt >= PUSH_DELAY_NOR) { if(pKEY_C) { MENU_ONEn(MN_ON,PUSHING) } else { gbPushCnt = 0; MENU_VAL_REDRAW(ONp); } }
							else { MENU_ONEn(MN_ON,ONp)  MENU_CODE( MENU_VAL_PUSH(ONp, PUSHING, PUSH_DELAY_NOR, , extern void UpReset(void); UpReset(); ) ) } }),
			PAR_ID,			MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW()), gbMnParID, 0, 31, 1, ),
			PAR_BYTE,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW()), gbMnParByte, 0,  3, 1, ),
			PAR_VAL,		MENU_VARn(MN_ON, , (*((BYTE*)(gnRxPar+gbMnParID)+gbMnParByte)), 0, 255, 1, ),	// Default Byte Ordering is MSB(Most Significant Byte)
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, /*MN_POS_SIZ_LARGE*/))


	MENU_DISPLAY_END()
	//MENU_DISPLAY_STAT


// Condition for Menu call --------------------------------------------------------------------
	if(gbMenuPushState == 2) {
		gbMenuPushState = 0;
		MP(MpAwbPrst) = 0;
		MENU_REDRAW();
	}

	if(iSetMnOn&&MENU_NAME_IS(SYSTEM)) {										// Setup MENU start
		//gbMnSetupOn = 1;
		MENU_OPEN(SETUP);
		goto menu_start;
	}

	if(MENU_TITLE_IS(OFF)) {	// if not Main Menu

		if(LibUtlKeyPass(gbPassKeys2, sizeof(gbPassKeys2), &gbPassCnt2) == 1) {	// Debug MENU start
			//OsdWhiteAll(); OsdClearAll();
			//MN_MXSP = 0;
			//MN_POS_SIZ_SMALL
			MENU_OPEN(DEBUG);
			goto menu_start;
		}

//		if (KEY_C && (!gbPassCnt1) && (!gbPassCnt2)) {						// Main  MENU start
		if (KEY_C &&
			(!gbPassCnt2) && (!gbPassCnt3) && (!gbPassCnt4) &&
			(!gbPassCnt5) && (!gbPassCnt6)						) {
			//DispClr(gbIMDFontY,gbIMDFontX,18);
			MENU_OPEN(MENU);
			goto menu_start;
		}
	}
 	else if(KEY_E) 	MENU_CLOSE();		// Menu out by ECM KEY_E


// Menu Exit by Time out  -------------------------------------------------------------------------

#if (MN_AUTO_OFF)
	static UINT gnMnTimeCnt = 0;

 	if ( (!MENU_TITLE_IS(OFF))&&(gbMnDebugFnc==0) ) {

		if (KEY) gnMnTimeCnt = 0;
		else {
			 gnMnTimeCnt++;
			if ( gnMnTimeCnt >= ( MN_OFF_TIME * (MN_FPS_LOW ? (FR>>1) : FR) ) )
			{
				gnMnTimeCnt = 0;
				MenuSave(MN_OFF);
				MENU_CLOSE();
			}

		}
  	}
  	else 	gnMnTimeCnt = 0;

	/* Debug
		FONT_ONw(1);
		DispDatDec("giLV____",	1, 24, giLV);
		DispDatDec("MenuCnt_",  2, 24, gnMnTimeCnt);
	*/
#endif

// Exceptional Menu --------------------------------------------------------------------------------

// CAM_TITLE
	if(MENU_TITLE_IS(CAMTITLE)) {
		if(KEY_L || KEY_R) {
			SETFONTATTR(gbMenuY+2, MN_XTITL+gbStgPos[giLV], MN_WHITE);
			SETFONTATTR(gbMenuY+1, MN_XTITL+gbStgPos[giLV], MN_WHITE);
			SETFONTID(gbMenuY+1, MN_XTITL+gbStgPos[giLV], 0);

			menu_val(gbStgPos+giLV, 0, 7, 1);
			SETFONTATTR(gbMenuY+2, MN_XTITL+gbStgPos[giLV], MN_YELLOW);
			SETFONTATTR(gbMenuY+1, MN_XTITL+gbStgPos[giLV], MN_YELLOW);
			SETFONTID(gbMenuY+1, MN_XTITL+gbStgPos[giLV], 162);
		}
		/*
		DispStr("gbCamTitle", 0, 30, 10);
		DispClrDec(gbCamTitle(gbStgPos[giLV]), 0, 42, 10);
		*/
		if(KEY_D || KEY_U) {
			switch( gbCamTitle(gbStgPos[giLV]) ) {
				case ' ' :	if(KEY_U) gbCamTitle(gbStgPos[giLV]) = '0';
							if(KEY_D) gbCamTitle(gbStgPos[giLV]) = 'Z';	break;
				case '0' :	if(KEY_U) gbCamTitle(gbStgPos[giLV]) = '1';
							if(KEY_D) gbCamTitle(gbStgPos[giLV]) = ' ';	break;
				case '9' :	if(KEY_U) gbCamTitle(gbStgPos[giLV]) = 'A';
							if(KEY_D) gbCamTitle(gbStgPos[giLV]) = '8';	break;
				case 'A' :	if(KEY_U) gbCamTitle(gbStgPos[giLV]) = 'B';
							if(KEY_D) gbCamTitle(gbStgPos[giLV]) = '9';	break;
				case 'Z' :	if(KEY_U) gbCamTitle(gbStgPos[giLV]) = ' ';
							if(KEY_D) gbCamTitle(gbStgPos[giLV]) = 'Y';	break;
				default  :	if(KEY_U) gbCamTitle(gbStgPos[giLV])++;
							if(KEY_D) gbCamTitle(gbStgPos[giLV])--;		break;
			}
			DispStr((const char*)&gbCamTitle(0), gbMenuY+2, MN_XTITL, 0x8);
			OsdCamTitle();
		}
	}

//	if (MENU_TITLE_IS(BACKLIGHT)&&(MP(MpBackLight)==MN_BL_BLC))	gbMnBlcOsdOn = 1;
	if ((MENU_TITLE_IS(BLC)&&((MP(MpBackLight)==MN_BL_BLC)||(MP(MpWdrBlc)!=MN_OFF)))||MP(MpBlcOsdOn))	gbMnBlcOsdOn = 1;		
	else 																								gbMnBlcOsdOn = 0;

	// Comtile Menu Changed from other interface (webpage?) // DANNY_180716
	static BYTE bCamTitleOn_Buf=0xEE;
	if(bCamTitleOn_Buf!=MP(MpCamTitleOn)) { OsdCamTitle(); bCamTitleOn_Buf=MP(MpCamTitleOn); }


menu_end:

	gbMenuState = !MENU_TITLE_IS(OFF);	// 0:Menu Close 1:Menu Open

	return;
}

void DIn_InitMenu(void)
{	// Initiate Menu
	TxStrDecNoIRQ("Menu Init... ", 0, 0);
	
	MenuReset();
	
	UartBaudRate();
	
	InitFontOSD();
}

void DIn_Menu(void)
{

//		static BYTE gbPassCnt1=0;
	static BYTE gbPassCnt2=0;
	static BYTE gbPassCnt3=0;
	static BYTE gbPassCnt4=0;
	static BYTE gbPassCnt5=0;
	static BYTE gbPassCnt6=0;

//		const BYTE gbPassKeys1[] = MENU_PK_SETUP; // It's for setup menu
	const BYTE gbPassKeys2[] = MENU_PK_DEBUG; // It's for debuging menu

//		const int iSetMnOn = (!MENU_TITLE_IS(OFF)) ? (LibUtlKeyPass(gbPassKeys1, sizeof(gbPassKeys1), &gbPassCnt1) == 1) : 0 ;
	
	MENU_START

// Main MENU --------------------------------------------------------------------------------
// EN673

	MENU_SET( 3, MENU, MN_ON, MN_OFF,						
			PRIVACY,		MENU_STRi(MN_ON, MENU_VAL_IS(ONe), PRIVACY, MENU_OFF_GRAY_ONLY(), , MP(MpPrivacy), 2, OFF, ONe),
			MOTION, 		MENU_STRi(MN_ON, MENU_VAL_IS(ONe) ,MOTION, MENU_OFF_GRAY_ONLY();, , MP(MpItl), 2, OFF, ONe),				//lgh
			EXIT,			MENU_ONEo(MN_ON,  , MN_ON, MenuSave(MN_ON);))

	// MENU - IMAGE - PRIVACY
	MENU_SET( 12, PRIVACY, MN_ON, MN_OFF,
			ZONENUM,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpPvcZone), 0, PVC_EA-1, 1, ),
			ZONEDISP,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnPvcCfg(MP(MpPvcZone))->bAction, 2, OFF, ON),
			HPOS,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bPosX, 0, 60, 0, ),
			VPOS,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bPosY, 0, 34, 0, ),
			HSIZE,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bSizX, 0, 60, 0, ),
			VSIZE,			MENU_VARn(gbMnPvcCfg(MP(MpPvcZone))->bAction, , gbMnPvcCfg(MP(MpPvcZone))->bSizY, 0, 34, 0, ),
			PRV_MASKMOD,	MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpPvcFormat), 2, PRV_FILL, PRV_FORMAT),
			YLEVEL, 		MENU_BARn(MN_ON, , MP(MpPvcYLevel), 0, UP(UpBOX_COLOR_STEP), 1),
			CBLEVEL,		MENU_BARn(MN_ON, , MP(MpPvcCBLevel),0, UP(UpBOX_COLOR_STEP), 1),
			CRLEVEL,		MENU_BARn(MN_ON, , MP(MpPvcCRLevel),0, UP(UpBOX_COLOR_STEP), 1),
			TRANS,			MENU_VARn(MN_ON, , MP(MpPvcTrans), 0, 3, 1, ),
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, ))

	// MENU - MOTION
	MENU_SET( 8, MOTION, MN_ON, MN_OFF,
			DET_WINDOW,		MENU_ONEi(MN_ON, e, MN_ON, DET_WINDOW, MENU_OFF_GRAY_ONLY(); IMD_MASK_SEL_ON;),
			DET_TONE,		MENU_VARn(MN_ON, , MP(MpItlDettone), 0, 4, 1, ),
			MDRECT_FILL,	MENU_STRn(MN_ON, ,MP(MpItlRectFill), 2, OFF, ON),
			SENSITIVITY, 	MENU_BARn(MN_ON, ,MP(MpItlSens),0, IMD_SENS_MAX, 1),
			MOTION_OSD, 	MENU_STRn(MN_ON, ,MP(MpItlMaskOsd), 2, OFF, ON),
			TEXT_ALARM, 	MENU_STRn(MN_ON, ,MP(MpItlAlarm), 2, OFF, ON),
			SIGNAL_OUT, 	MENU_STRn(MN_ON, ,MP(MpItlSigno), 2, OFF, ON),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON,))

	MENU_SET( 7, DET_WINDOW, MN_ON, MN_OFF,
			WINDOWZONE,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), MP(MpItlWindowuse), 0, MASK_EA-1, 1,),
			WINDOWUSE,		MENU_STRn(MN_ON, if_KEY_LR(MENU_REDRAW_GRAY_ONLY();), gbMnImdCfg(MP(MpItlWindowuse))->bAction, 2, OFF, ON),
			DET_H_POS, 		MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bPosX, 0, gnIMD_Hw, 0, ),
			DET_V_POS, 		MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bPosY, 0, gnIMD_Vw, 0, ),
			DET_H_SIZE, 	MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bSizX, 0, gnIMD_Hw, 0, ),
			DET_V_SIZE, 	MENU_VARn(gbMnImdCfg(MP(MpItlWindowuse))->bAction, , gbMnImdCfg(MP(MpItlWindowuse))->bSizY, 0, gnIMD_Vw, 0, ),
			RETURN,			MENU_ONEo(MN_ON, e, MN_ON, IMD_MASK_SEL_OFF; ))

	
// Debug MENU --------------------------------------------------------------------------------

	MENU_SET(7, DEBUG, MN_ON, MN_OFF,
			FUNCTION,		MENU_STRn(MN_ON, if_KEY_LR(OsdWhiteAll(); OsdClearAll(); ChangeMenuSize(); OsdCamTitle(); MENU_CHANGE()) , gbMnDebugFnc, 9, OFF, AE, AF, AWB, DBG_4, DBG_5, IMD_DBG, TDN_DBG, STATUS),
			BYPASS, 		MENU_STRn(MN_ON, 
								//if_KEY_LR(if(gbMnDebugBypass) { TIM_CH3_EN = 0; } else { TIM_CH3_EN = 1; } )
								, gbMnDebugBypass, 2, OFF, ON),
			UP_RESET,		({ if(gbPushCnt >= PUSH_DELAY_NOR) { if(pKEY_C) { MENU_ONEn(MN_ON,PUSHING) } else { gbPushCnt = 0; MENU_VAL_REDRAW(ONp); } }
							else { MENU_ONEn(MN_ON,ONp)  MENU_CODE( MENU_VAL_PUSH(ONp, PUSHING, PUSH_DELAY_NOR, , extern void UpReset(void); UpReset(); ) ) } }),
			PAR_ID, 		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW()), gbMnParID, 0, 31, 1, ),
			PAR_BYTE,		MENU_VARn(MN_ON, if_KEY_LR(MENU_REDRAW()), gbMnParByte, 0,	3, 1, ),
			PAR_VAL,		MENU_VARn(MN_ON, , (*((BYTE*)(gnRxPar+gbMnParID)+gbMnParByte)), 0, 255, 1, ),	// Default Byte Ordering is MSB(Most Significant Byte)
			RETURN, 		MENU_ONEo(MN_ON, e, MN_ON, /*MN_POS_SIZ_LARGE*/))
	

	MENU_DISPLAY_END()

	if(MENU_NAME_IS(SYSTEM)) {										// Setup MENU start
		//gbMnSetupOn = 1;
		MENU_OPEN(SETUP);
		goto menu_start;
	}

	if(MENU_TITLE_IS(OFF)) {	// if not in Main Menu
	#if 0
		if (KEY_C) { MENU_OPEN(MENU); goto menu_start; }
	#else
		if(LibUtlKeyPass(gbPassKeys2, sizeof(gbPassKeys2), &gbPassCnt2) == 1) {	// Debug MENU start
			//OsdWhiteAll(); OsdClearAll();
			//MN_MXSP = 0;
			//MN_POS_SIZ_SMALL
			MENU_OPEN(DEBUG);
			goto menu_start;
		}

//		if (KEY_C && (!gbPassCnt1) && (!gbPassCnt2)) {						// Main  MENU start
		if (KEY_C &&
			(!gbPassCnt2) && (!gbPassCnt3) && (!gbPassCnt4) &&
			(!gbPassCnt5) && (!gbPassCnt6)						) {

			MENU_OPEN(MENU);
			goto menu_start;
		}
	#endif
	}
 	else if(KEY_E) 	MENU_CLOSE();		// Menu out by ECM KEY_E

menu_end:

	gbMenuState = !MENU_TITLE_IS(OFF);	// 0:Menu Close 1:Menu Open

	return;
}


/* EOF */
