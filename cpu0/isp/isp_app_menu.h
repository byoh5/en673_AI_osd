/* **************************************************************************
 File Name	: 	app_menu_new.h
 Description:	New Menu
 Designer	:	Kim Sung-hoon (shkim@eyenix.com)
 Date		:	12.03.30
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

//*************************************************************************************************
// Define
//-------------------------------------------------------------------------------------------------
#define MN_IV_POS       	10						// (fix)
#define MN_FONT_MAX_PATH	10						// number of String for Debugging (fix)
#define MN_FONT_SPACE		1						// number of Space for Debugging (fix)

// OSD On/Off
#define OSD_INIT()		FONT_ONw(0)
#define OSD_ON()		FONT_ONw(1)
#define OSD_OFF()

//----------------------------------------------------------------------------------------
// String
//#define _S(NAME)		s##NAME

#define STR_LIST1(NAME)						(PEXCH)_S(1##NAME)
#define STR_LIST2(NAME)		STR_LIST1(NAME),(PEXCH)_S(2##NAME)
#define STR_LIST3(NAME)		STR_LIST2(NAME),(PEXCH)_S(3##NAME)
#define STR_LIST4(NAME)		STR_LIST3(NAME),(PEXCH)_S(4##NAME)
#define STR_LIST5(NAME)		STR_LIST4(NAME),(PEXCH)_S(5##NAME)
#define STR_LIST6(NAME)		STR_LIST5(NAME),(PEXCH)_S(6##NAME)
#define STR_LIST7(NAME)		STR_LIST6(NAME),(PEXCH)_S(7##NAME)
#define STR_LIST8(NAME)		STR_LIST7(NAME),(PEXCH)_S(8##NAME)
#define STR_LIST9(NAME)		STR_LIST8(NAME),(PEXCH)_S(9##NAME)
#define STR_LISTA(NAME)		STR_LIST9(NAME),(PEXCH)_S(A##NAME)
#define STR_LISTB(NAME)		STR_LISTA(NAME),(PEXCH)_S(B##NAME)
#define STR_LISTC(NAME)		STR_LISTB(NAME),(PEXCH)_S(C##NAME)

#if MN_LNG_EA == 1
  #if model_CharMax == 1
	#define STRLEN(L,STR)		{ if(*((BYTE*)STR[1]) == 2) L = strlenEx((EXCH*)STR[0]); else L = strlen((char*)STR[0]); }
	#define DISPSTR(STR,...)	{ if(*((BYTE*)STR[1]) == 2) DispStrEx((EXCH*)STR[0],__VA_ARGS__); else DispStr((char*)STR[0],__VA_ARGS__); }
	#define DISPCLRSTR(STR,...)	{ if(*((BYTE*)STR[1]) == 2) DispClrStrEx((EXCH*)STR[0],__VA_ARGS__); else DispClrStr((char*)STR[0],__VA_ARGS__); }
	//#define PEXCH				UINT//EXCH*
	#define STRING1(NAME,C1,STR1)		const C1 _S(1##NAME)[] = STR1; const BYTE _S(0##NAME) = sizeof(C1); const PEXCH _S(NAME)[] = { (PEXCH)_S(1##NAME), (PEXCH)&_S(0##NAME) }
  #else
	#define STRLEN(L,STR)		L = strlen((const char*)STR)
	#define DISPSTR(STR,...)	DispStr((const char*)STR,__VA_ARGS__)
	#define DISPCLRSTR(STR,...)	DispClrStr((const char*)STR,__VA_ARGS__)
	//#define PEXCH				EXCH
	#define STRING1(NAME,C1,STR1)		const PEXCH _S(NAME)[] = STR1;
  #endif
	#define STRING2(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING3(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING4(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING5(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING6(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING7(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING8(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRING9(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGA(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGB(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
	#define STRINGC(NAME,C1,STR1,...)	STRING1(NAME,C1,STR1)
#else
	//#define PEXCH		UINT//EXCH*

  #if model_CharMax == 1
	#define LNG_INDEX(STR)		(MP(MpLanguageSel)>=((BYTE*)STR[0])[0] ? 1 : MP(MpLanguageSel)+1)
	#define LS(ARRAY)			ARRAY[LNG_INDEX(ARRAY)]
	#define STRLEN(L,STR)		{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) L = strlenEx((EXCH*)LS(STR)); else L = strlen((char*)LS(STR)); }
	#define DISPSTR(STR,...)	{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) DispStrEx((EXCH*)LS(STR),__VA_ARGS__); else DispStr((char*)LS(STR),__VA_ARGS__); }
	#define DISPCLRSTR(STR,...)	{ if( ((BYTE*)STR[0])[LNG_INDEX(STR)] == 2) DispClrStrEx((EXCH*)LS(STR),__VA_ARGS__); else DispClrStr((char*)LS(STR),__VA_ARGS__); }
	#define STR_LIST(NUM,NAME)	const PEXCH	_S(NAME)[] = { (PEXCH)_S(0##NAME), STR_LIST##NUM(NAME) }

	#define SIZE_LIST1(C1)			sizeof(C1)
	#define SIZE_LIST2(C1,...)		sizeof(C1),SIZE_LIST1(__VA_ARGS__)
	#define SIZE_LIST3(C1,...)		sizeof(C1),SIZE_LIST2(__VA_ARGS__)
	#define SIZE_LIST4(C1,...)		sizeof(C1),SIZE_LIST3(__VA_ARGS__)
	#define SIZE_LIST5(C1,...)		sizeof(C1),SIZE_LIST4(__VA_ARGS__)
	#define SIZE_LIST6(C1,...)		sizeof(C1),SIZE_LIST5(__VA_ARGS__)
	#define SIZE_LIST7(C1,...)		sizeof(C1),SIZE_LIST6(__VA_ARGS__)
	#define SIZE_LIST8(C1,...)		sizeof(C1),SIZE_LIST7(__VA_ARGS__)
	#define SIZE_LIST9(C1,...)		sizeof(C1),SIZE_LIST8(__VA_ARGS__)
	#define SIZE_LISTA(C1,...)		sizeof(C1),SIZE_LIST9(__VA_ARGS__)
	#define SIZE_LISTB(C1,...)		sizeof(C1),SIZE_LISTA(__VA_ARGS__)
	#define SIZE_LISTC(C1,...)		sizeof(C1),SIZE_LISTB(__VA_ARGS__)
	#define SIZE_LIST(NUM,NAME,...)	const BYTE	_S(0##NAME)[] = { NUM, SIZE_LIST##NUM(__VA_ARGS__) };

	// STR1,STR2,...�� ������ _P(...)������� ���� MACRO�Ұ���
	#define STRING1(NAME,C1,STR1)\
			SIZE_LIST(1,NAME,C1)\
			const C1	_S(1##NAME)[] = STR1;\
			STR_LIST(1,NAME)

	#define STRING2(NAME,C1,STR1,C2,STR2)\
			SIZE_LIST(2,NAME,C1,C2)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			STR_LIST(2,NAME)

	#define STRING3(NAME,C1,STR1,C2,STR2,C3,STR3)\
			SIZE_LIST(3,NAME,C1,C2,C3)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			STR_LIST(3,NAME)

	#define STRING4(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4)\
			SIZE_LIST(4,NAME,C1,C2,C3,C4)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			STR_LIST(4,NAME)

	#define STRING5(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5)\
			SIZE_LIST(5,NAME,C1,C2,C3,C4,C5)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			STR_LIST(5,NAME)

	#define STRING6(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6)\
			SIZE_LIST(6,NAME,C1,C2,C3,C4,C5,C6)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			STR_LIST(6,NAME)

	#define STRING7(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7)\
			SIZE_LIST(7,NAME,C1,C2,C3,C4,C5,C6,C7)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			STR_LIST(7,NAME)

	#define STRING8(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8)\
			SIZE_LIST(8,NAME,C1,C2,C3,C4,C5,C6,C7,C8)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			STR_LIST(8,NAME)
	
	#define STRING9(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9)\
			SIZE_LIST(9,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			STR_LIST(9,NAME)
	
	#define STRINGA(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA)\
			SIZE_LIST(A,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			STR_LIST(A,NAME)
	
	#define STRINGB(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB)\
			SIZE_LIST(B,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA,CB)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			STR_LIST(B,NAME)
	
	#define STRINGC(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB,CC,STRC)\
			SIZE_LIST(C,NAME,C1,C2,C3,C4,C5,C6,C7,C8,C9,CA,CB,CC)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			const CC	_S(C##NAME)[] = STRC;\
			STR_LIST(C,NAME)
	
  #else
	#define LNG_INDEX(STR)		(MP(MpLanguageSel)>=STR[0] ? 1 : MP(MpLanguageSel)+1)
	#define STRLEN(L,STR)		L = strlen((EXCH*)STR[LNG_INDEX(STR)])
	#define DISPSTR(STR,...)	DispStr((EXCH*)STR[LNG_INDEX(STR)],__VA_ARGS__)
	#define DISPCLRSTR(STR,...)	DispClrStr((EXCH*)STR[LNG_INDEX(STR)],__VA_ARGS__)
	#define STR_LIST(NUM,NAME)	const PEXCH	_S(NAME)[] = { NUM, STR_LIST##NUM(NAME) }

	// STR1,STR2,...�� ������ _P(...)������� ���� MACRO�Ұ���
	#define STRING1(NAME,C1,STR1)\
			const C1	_S(1##NAME)[] = STR1;\
			STR_LIST(1, NAME)

	#define STRING2(NAME,C1,STR1,C2,STR2)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			STR_LIST(2, NAME)

	#define STRING3(NAME,C1,STR1,C2,STR2,C3,STR3)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			STR_LIST(3, NAME)

	#define STRING4(NAME,C1,STR1,C2,STR2,C3,STR3, C4,STR4)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			STR_LIST(4, NAME)

	#define STRING5(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			STR_LIST(5, NAME)

	#define STRING6(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			STR_LIST(6, NAME)

	#define STRING7(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			STR_LIST(7, NAME)
	
	#define STRING8(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			STR_LIST(8, NAME)
	
	#define STRING9(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			STR_LIST(9, NAME)
	
	#define STRINGA(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			STR_LIST(A, NAME)
	
	#define STRINGB(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			STR_LIST(B, NAME)
	
	#define STRINGC(NAME,C1,STR1,C2,STR2,C3,STR3,C4,STR4,C5,STR5,C6,STR6,C7,STR7,C8,STR8,C9,STR9,CA,STRA,CB,STRB,CC,STRC)\
			const C1	_S(1##NAME)[] = STR1;\
			const C2	_S(2##NAME)[] = STR2;\
			const C3	_S(3##NAME)[] = STR3;\
			const C4	_S(4##NAME)[] = STR4;\
			const C5	_S(5##NAME)[] = STR5;\
			const C6	_S(6##NAME)[] = STR6;\
			const C7	_S(7##NAME)[] = STR7;\
			const C8	_S(8##NAME)[] = STR8;\
			const C9	_S(9##NAME)[] = STR9;\
			const CA	_S(A##NAME)[] = STRA;\
			const CB	_S(B##NAME)[] = STRB;\
			const CC	_S(C##NAME)[] = STRC;\
			STR_LIST(C, NAME)
  #endif
#endif

#define DrawStr(STR,...)	{ DISPSTR(_S(STR),__VA_ARGS__); }

#define DrawBar(VAL,MIN,MAX) {\
	DispClrDec(VAL, DRAW_Y, MN_SXSP, 3);\
	OsdDispBar(VAL, DRAW_Y, MIN, MAX);}


#define OsdMenuPos(V)		OsdMenuPos##V()

// Clear : Index finger & color on line
#define OsdMenuPos0()	if(giMenuNum) {\
						OsdAttrsStgPosAr(gbStgPos[giLV], MN_WHITE);\
						/*SetFont(gbMenuY+gbStgPos[giLV], MXSP-2, 0);*/	\
						}

// Display : Index finger & color on line
#define OsdMenuPos1()	if(giMenuNum) {\
						OsdAttrsStgPosAr(gbStgPos[giLV], MN_YELLOW);\
						/*SetFont(gbMenuY+gbStgPos[giLV], MXSP-2, 0)*/	\
						}

//----------------------------------------------------------------------------------------
// Menu
#define S_LIST1(O1)				_S(O1)
#define S_LIST2(O1, ...)		_S(O1),S_LIST1(__VA_ARGS__)
#define S_LIST3(O1, ...)		_S(O1),S_LIST2(__VA_ARGS__)
#define S_LIST4(O1, ...)		_S(O1),S_LIST3(__VA_ARGS__)
#define S_LIST5(O1, ...)		_S(O1),S_LIST4(__VA_ARGS__)
#define S_LIST6(O1, ...)		_S(O1),S_LIST5(__VA_ARGS__)
#define S_LIST7(O1, ...)		_S(O1),S_LIST6(__VA_ARGS__)
#define S_LIST8(O1, ...)		_S(O1),S_LIST7(__VA_ARGS__)
#define S_LIST9(O1, ...)		_S(O1),S_LIST8(__VA_ARGS__)
#define S_LIST10(O1, ...)		_S(O1),S_LIST9(__VA_ARGS__)
#define S_LIST11(O1, ...)		_S(O1),S_LIST10(__VA_ARGS__)
#define S_LIST12(O1, ...)		_S(O1),S_LIST11(__VA_ARGS__)
#define S_LIST13(O1, ...)		_S(O1),S_LIST12(__VA_ARGS__)
#define S_LIST14(O1, ...)		_S(O1),S_LIST13(__VA_ARGS__)
#define S_LIST15(O1, ...)		_S(O1),S_LIST14(__VA_ARGS__)
#define S_LIST16(O1, ...)		_S(O1),S_LIST15(__VA_ARGS__)

#define S_LISTm1(M1,C1)			_S(M1)
#define S_LISTm2(M1,C1, ...)	_S(M1),S_LISTm1(__VA_ARGS__)
#define S_LISTm3(M1,C1, ...)	_S(M1),S_LISTm2(__VA_ARGS__)
#define S_LISTm4(M1,C1, ...)	_S(M1),S_LISTm3(__VA_ARGS__)
#define S_LISTm5(M1,C1, ...)	_S(M1),S_LISTm4(__VA_ARGS__)
#define S_LISTm6(M1,C1, ...)	_S(M1),S_LISTm5(__VA_ARGS__)
#define S_LISTm7(M1,C1, ...)	_S(M1),S_LISTm6(__VA_ARGS__)
#define S_LISTm8(M1,C1, ...)	_S(M1),S_LISTm7(__VA_ARGS__)
#define S_LISTm9(M1,C1, ...)	_S(M1),S_LISTm8(__VA_ARGS__)
#define S_LISTm10(M1,C1, ...)	_S(M1),S_LISTm9(__VA_ARGS__)
#define S_LISTm11(M1,C1, ...)	_S(M1),S_LISTm10(__VA_ARGS__)
#define S_LISTm12(M1,C1, ...)	_S(M1),S_LISTm11(__VA_ARGS__)
#define S_LISTm13(M1,C1, ...)	_S(M1),S_LISTm12(__VA_ARGS__)
#define S_LISTm14(M1,C1, ...)	_S(M1),S_LISTm13(__VA_ARGS__)
#define S_LISTm15(M1,C1, ...)	_S(M1),S_LISTm14(__VA_ARGS__)
#define S_LISTm16(M1,C1, ...)	_S(M1),S_LISTm15(__VA_ARGS__)

#define M_CODE1(NUM,M1,C1)			case NUM-1 : C1; M_CODE_OUT;
#define M_CODE2(NUM,M1,C1, ...)		case NUM-2 : C1; M_CODE_OUT;	M_CODE1(NUM,__VA_ARGS__)
#define M_CODE3(NUM,M1,C1, ...)		case NUM-3 : C1; M_CODE_OUT;	M_CODE2(NUM,__VA_ARGS__)
#define M_CODE4(NUM,M1,C1, ...)		case NUM-4 : C1; M_CODE_OUT;	M_CODE3(NUM,__VA_ARGS__)
#define M_CODE5(NUM,M1,C1, ...)		case NUM-5 : C1; M_CODE_OUT;	M_CODE4(NUM,__VA_ARGS__)
#define M_CODE6(NUM,M1,C1, ...)		case NUM-6 : C1; M_CODE_OUT;	M_CODE5(NUM,__VA_ARGS__)
#define M_CODE7(NUM,M1,C1, ...)		case NUM-7 : C1; M_CODE_OUT;	M_CODE6(NUM,__VA_ARGS__)
#define M_CODE8(NUM,M1,C1, ...)		case NUM-8 : C1; M_CODE_OUT;	M_CODE7(NUM,__VA_ARGS__)
#define M_CODE9(NUM,M1,C1, ...)		case NUM-9 : C1; M_CODE_OUT;	M_CODE8(NUM,__VA_ARGS__)
#define M_CODE10(NUM,M1,C1, ...)	case NUM-10 : C1; M_CODE_OUT;	M_CODE9(NUM,__VA_ARGS__)
#define M_CODE11(NUM,M1,C1, ...)	case NUM-11 : C1; M_CODE_OUT;	M_CODE10(NUM,__VA_ARGS__)
#define M_CODE12(NUM,M1,C1, ...)	case NUM-12 : C1; M_CODE_OUT;	M_CODE11(NUM,__VA_ARGS__)
#define M_CODE13(NUM,M1,C1, ...)	case NUM-13 : C1; M_CODE_OUT;	M_CODE12(NUM,__VA_ARGS__)
#define M_CODE14(NUM,M1,C1, ...)	case NUM-14 : C1; M_CODE_OUT;	M_CODE13(NUM,__VA_ARGS__)
#define M_CODE15(NUM,M1,C1, ...)	case NUM-15 : C1; M_CODE_OUT;	M_CODE14(NUM,__VA_ARGS__)
#define M_CODE16(NUM,M1,C1, ...)	case NUM-16 : C1; M_CODE_OUT;	M_CODE15(NUM,__VA_ARGS__)

#define MENU_SET_NUM(TITLE,NUM, ...)		MS_PRE(TITLE)\
		S_LISTm##NUM(__VA_ARGS__) };		MS_SUF(TITLE,NUM)\
		M_CODE##NUM(NUM,__VA_ARGS__) }

#define M_CODE_OUT			/*goto menu_display_end*/break
		
#define MS_PRE(TITLE)		const PEXCH* TITLE##LIST[] = {
#define MS_SUF(TITLE,NUM)	menu_pos(_S(TITLE), NUM, TITLE##LIST);	switch(gbStgPos[giLV]) {

#define MENU_START\
		/*int i;*/	\
		UINT DRAW_ON;\
		/*UINT DRAW_Y;*/	\
	menu_start:\
		DRAW_ON = (KEY_L || KEY_R || giMenuDispPos);\
		/*DRAW_Y = gbMenuY+gbStgPos[giLV];*/	\
		if(0) {}

#define MENU_DISPLAY_END(...)\
	/*menu_display_end:*/	\
		if(giMenuDispPos) {\
			gbMenuList	= 0;\
			gbMenuVal	= 0;\
			if(giLV >= 0) {\
				if(gbStgPos[giLV] != giMenuNum) goto menu_start;\
				gbStgPos[giLV] = giMenuDispPos - 1;\
				OsdMenuPos(1);\
			}\
			giMenuDispPos = 0;\
			giMenuDispChg = 0;\
			giGrayOnly = 0;\
			__VA_ARGS__\
			goto menu_end;\
		} else {\
			if(KEY_U || KEY_D) OsdMenuPos(1);\
		}

#define MENU_DISPLAY_STAT\
	DispClr(15,MN_MXSP,MN_LINESIZE);		if(MENU_TITLE)	{ DISPSTR(MENU_TITLE,	15, MN_MXSP, MN_LINESIZE); }\
	DispClr(16,MN_MXSP,MN_SXSP-MN_MXSP);	if(MENU_NAME)	{ DISPSTR(MENU_NAME,	16, MN_MXSP, MN_SXSP-MN_MXSP); }\
	DispClr(16,MN_SXSP,MN_SUBSIZE);			if(MENU_VAL)	{ DISPSTR(MENU_VAL,		16, MN_SXSP, MN_SUBSIZE); }

#define MENU_DISABLE(MENU_ON)			if(!(MENU_ON)) { if(KEY_U || KEY_D) goto menu_start; }
#define MENU_IN(IN_ON,IN_NAME, ...)		if(KEY_C && (giMenuDispPos == 0) && (IN_ON)) { menu_in(_S(IN_NAME)); __VA_ARGS__ goto menu_start; }
#define MENU_OUT(OUT_ON, CF, ...)		if(KEY_C && (giMenuDispPos == 0) && (OUT_ON)) { __VA_ARGS__  menu_out(CF); goto menu_start; }
#define MENU_CODE(...)					if(giMenuDispPos == 0) { __VA_ARGS__ }
#define MENU_STR(MENU_ON,VAL,NUM, ...)	const PEXCH* __LIST[] = { S_LIST##NUM(__VA_ARGS__) }; menu_str(MENU_ON, &VAL, NUM, __LIST);	MENU_DISABLE(MENU_ON)

//-------------------------------------------------------------------------------------------------
// Define etc Font ID (ASCII)
#define	_IIO		133
#define	_IOI		134
#define	_OII		135
#define	_OOO		154
#define	_IOO		155
#define	_OIO		156
#define	_OOI		157

//*************************************************************************************************
// Menu Top
//-------------------------------------------------------------------------------------------------
#define _B			BYTE
#define _W			EXCH
#define _P(...)		{ __VA_ARGS__, 0 }

#define MENU_EXT(TITLE,MENU_ON,OUT_ON,OUT_CODE, ...)\
	else if((MENU_TITLE == _S(TITLE)) && (MENU_ON)) { menu_pos(_S(TITLE), 0, 0); if(giMenuDispChg) { giMenuDispChg = 0; __VA_ARGS__ }\
		MENU_OUT(OUT_ON, 0, OUT_CODE;)\
	}
#define MENU_SET(NUM,TITLE,MENU_ON,OUT_ON, ...)\
		else if((MENU_TITLE == _S(TITLE)) && (MENU_ON)) { MENU_SET_NUM(TITLE, NUM, __VA_ARGS__)\
			if(OUT_ON) {\
				if(giMenuDispPos) { DrawStr(ENTER_d_RETURN, MN_YSTART+NUM+1, MN_MXSP+2, 14); }\
				MENU_OUT(MN_ON, 1, DispClr(MN_YSTART+NUM+1, MN_MXSP+2, 14);)\
			}\
		}

// n : nothing
// i : in
// o : out
// ONE : enter
// STR : many menu	
#define MENU_ONEn(MENU_ON,STR)					menu_one(MENU_ON, _S(STR));	MENU_DISABLE(MENU_ON)
#define MENU_ONEi(MENU_ON,STR,IN_ON,IN,IN_CODE)	MENU_ONEn(MENU_ON,STR)	MENU_IN(IN_ON,IN,IN_CODE)
#define MENU_ONEo(MENU_ON,STR,OUT_ON,OUT_CODE)	MENU_ONEn(MENU_ON,STR)	MENU_OUT(OUT_ON,1,OUT_CODE)
	
#define MENU_STRn(MENU_ON,CODE,VAL,NUM, ...)					({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_CODE(CODE) })
#define MENU_STRi(MENU_ON,IN_ON,IN,IN_CODE,CODE,VAL,NUM, ...)	({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_IN(IN_ON,IN,IN_CODE)	MENU_CODE(CODE) })
#define MENU_STRo(MENU_ON,OUT_ON,OUT_CODE,CODE,VAL,NUM, ...)	({ MENU_STR(MENU_ON,VAL,NUM, __VA_ARGS__)	MENU_OUT(OUT_ON,1,OUT_CODE)	MENU_CODE(CODE) })

#define MENU_PUSH(MENU_ON,VAL,STR_NOR,STR_PUSH,PUSH_DELAY,PUSH_CODE)	if(menu_push(MENU_ON,&VAL,_S(STR_NOR),_S(STR_PUSH),PUSH_DELAY)) {PUSH_CODE;}	MENU_DISABLE(MENU_ON)
#define MENU_BARn(MENU_ON,CODE,VAL,MIN,MAX,ROT)			menu_bar(MENU_ON, &VAL, MIN, MAX, ROT, 0);			MENU_DISABLE(MENU_ON)	MENU_CODE(CODE)	
#define MENU_VARn(MENU_ON,CODE,VAL,MIN,MAX,ROT,UNIT)	menu_bar(MENU_ON, &VAL, MIN, MAX, ROT, _S(UNIT));	MENU_DISABLE(MENU_ON)	MENU_CODE(CODE)	
#define MENU_VARe(MENU_ON,CODE,VAL_L,VAL_H,MIN,MAX)		menu_val_ex(MENU_ON, &VAL_L, &VAL_H, MIN, MAX);		MENU_DISABLE(MENU_ON)	MENU_CODE(CODE)	
#define MENU_DIRn(MENU_ON,STA,VAL,DRAW,END) ({\
		STA\
		if((giMenuDispPos == 0) && MENU_ON) { VAL }\
		if(DRAW_ON && (MENU_ON || giGrayOnly)) { DRAW }\
		END\
		gbMenuVal = 0;	menu_dir(MENU_ON);	MENU_DISABLE(MENU_ON) })

#define MENU_CHANGE()			{ menu_redraw(1,0); goto menu_start; } // NAME, VAL ����
#define MENU_REDRAW()			{ menu_redraw(0,0); goto menu_start; } // �ٽ� �׸��µ�, GRAY�� NOT_USED
#define MENU_REDRAW_GRAY_ONLY()	{ menu_redraw(0,1); goto menu_start; } // �ٽ� �׸��µ�, GRAY�� NOT_USED ����
#define MENU_OFF_GRAY_ONLY()	  giGrayOnly = 1;	// MENU ���� �� IN_CODE���� ȣ��, 'NOT USED' ����� ������� ����.
#define MENU_VAL_REDRAW(VAL)	{ /*gbMenuVal = (PEXCH*)_S(VAL);*/ DISPCLRSTR(_S(VAL), DRAW_Y/*gbMenuY+gbStgPos[giLV]*/, MN_SXSP, MN_SUBSIZE, MN_SUBSIZE); } // VAR ������ ���� ǥ��
#define MENU_NAME_REDRAW(NAME)	{ /*gbMenuList = (PEXCH*)_S(NAME);*/ DISPCLRSTR(_S(NAME), DRAW_Y/*gbMenuY+gbStgPos[giLV]*/, MN_MXSP, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/, (MN_SXSP-MN_MXSP)/*MN_LINESIZE*/); }
#define MENU_OPEN(MENU)			menu_in(_S(MENU))
#define MENU_CLOSE()			{ OsdMenuPos(0); menu_close(); } // MENU ���� ��𼭵� �� �Լ��� �θ��� �޴��� ����
#define if_KEY_LR(...)			if(KEY_L || KEY_R) { __VA_ARGS__; }

#define MENU_VAL_PUSH(STR_NOR,STR_PUSH,PUSH_DELAY,/*PUSH_ON,*/CODE_NOR,CODE_PUSH) {\
			if(MENU_VAL_IS(STR_NOR)) {\
				if(pKEY_C/*PUSH_ON*/) {\
					if(gbPushCnt < 0xFF) gbPushCnt++;\
					if(gbPushCnt == PUSH_DELAY) {\
						CODE_PUSH;\
						MENU_VAL_REDRAW(STR_PUSH);\
					}\
				}\
				else {\
					if(gbPushCnt) {\
						gbPushCnt = 0;\
						CODE_NOR;\
						MENU_VAL_REDRAW(STR_NOR);\
					}\
				}\
			}}

#define MENU_TITLE_CHANGE(TITLE) { gbMenuTitle = (PEXCH*)_S(TITLE); gbStgTitle[giLV] = gbMenuTitle; }
