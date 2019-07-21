/* **************************************************************************
 File Name	: 	isp_dev_misc.h
 Description:	EN673 - miscellaneous define
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */


#if 	(model_TgtBd==0)
	#define DFSEL_TgtBd(val0, val1, val2)	val0
#elif	(model_TgtBd==1)
	#define DFSEL_TgtBd(val0, val1, val2)	val1
#else
	#define DFSEL_TgtBd(val0, val1, val2)	val2
#endif

#if 	(model_Iris==0)
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val0
#elif	(model_Iris==1)
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val1
#elif	(model_Iris==2)
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val2
#elif	(model_Iris==3)
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val3
#elif	(model_Iris==4)
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val4
#else
	#define DFSEL_Irs(val0, val1, val2, val3, val4, val5)		val5
#endif


//-------------------------------------------------------------------------------------------------
// Type definition

typedef long long int			s64;
typedef long long unsigned int	u64;

// 2016415 - WHL : Add FPU
//	extern char gNumcode[17];
//	
//	extern char* strrev(char* s);
//	extern int uitoan(uint value, char* str, int base, int width, char fill_val);
//	extern int ftoan(float32 f, char* str, int width, char fill_val, int prec);
//	extern int ftox(float32 f, char* str);
//	
//	extern void hfloat_test(void);

#if 0 // 180319 KDH 
#if model_CharMax == 1
	#define EXCH		WORD
	#define EXCH_ADD	1
#else
	#define EXCH		char
	#define EXCH_ADD	0
#endif
#endif

#if 0	// 20150808 - It defined in "exr1k.h"
//#define ISPM0	ISPM
//#define ISPMt	ISPM
//#define ISPMp	ISPM
#define ISPM0	
#define ISPMt	
#define ISPMp	
#else	// 20150808 - It defined in "exr1k.h"
#if model_Ispm == 1
	extern const UINT _rom_ispm_text_s;
	extern const UINT _ispm_text_s;
	extern const UINT _ispm_text_e;

	#define ISPM0	__attribute__((section(".ispmtext")))
	#define ISPM	//__attribute__((section(".ispmtext")))
	#define ISPMt	//__attribute__((section(".ispmtext")))

  #if model_Aptn		// for I2C
	#define ISPMp	__attribute__((section(".ispmtext")))
  #else
	#define ISPMp
  #endif

	//extern volatile UINT gnRomTemp;
	//#define _J(func)	func//(*func + gnRomTemp)
#else
	#define ISPM0
	#define ISPM
	#define ISPMt

	#define ISPMp

	//#define _J(func)	func
#endif
#endif

#define DATA	__attribute__((section(".data")))

extern const UINT _reset_s;
extern const UINT _rom_data_e;
extern const UINT _rom_data_s;
extern const UINT _data_e;
extern const UINT _data_s;
extern const UINT _bss_e;
extern const UINT _bss_s;
extern const UINT _stack_e;
extern const UINT _stack_s;
extern const UINT _stack;

//extern const BYTE	gbMacAddr[6];				// Be careful. This is not variable. It's in flash


// -----------------------------------------------------------------------------------------------
// Clock & Frame info (reserved)
//#define FPS_DEF		60
#define FPS_DEF		30

#define FPS			((MP(MpSysFreq) == MN_SYSFREQ_50) ? 25 : 30)//FR
//#define FR_MSEC		((MP(MpSysFreq) == MN_SYSFREQ_50) ? 40 : 33)//FR_DUTY

extern UINT FPS_VDI;
extern UINT FPS_VDO;


//-------------------------------------------------------------------------------------------------
// Macro

#if 1	// 201562 - WHL - Org

  #if (model_IspRegCheck==1)
	extern void _wr8_fnc(const UINT anAddr, const BYTE abData);
	extern void _wr32_fnc(const UINT anAddr, const UINT anData);
	#define	_wr8					_wr8_fnc
	#define	_wr32					_wr32_fnc
  #else
	#define	_wr8(a,d)				(*((volatile BYTE *)(a)) = (d))		// Write to peripheral	- Only used for Serial Flash Write
	#define	_wr32(a,d)				(*((volatile UINT *)(a)) = (d))		//			"			- Always used for ISP Register Write
  #endif

	#define	_rd8(a)					(*((volatile BYTE *)(a)))			// Read from peripheral	- Only used for Serial Flash Read
	#define	_rd32(a)				(*((volatile UINT *)(a)))			//			"			- Always used for ISP Register Read

#else	// 201562 - WHL - For Delay Reg  Write
	extern void  ISPM Wr8_Dly(UINT anAddr, BYTE abData);
	extern void ISPM Wr32_Dly(UINT anAddr, UINT anData);

	extern BYTE ISPM Rd8_Dly(BYTE abAddr);
	extern UINT ISPM Rd32_Dly(UINT anAddr);

	#define	_wr8					Wr8_Dly
	#define	_wr32					Wr32_Dly

	#define	_rd8(a)					Rd8_Dly
	#define	_rd32(a)				Rd32_Dly

#endif

#define sfw8(a,d)				_wr8( SFLS_BASE | ((a)&FLASH_ADDR_MASK), d)
#define sfw32(a,d)				_wr32(SFLS_BASE | ((a)&FLASH_ADDR_MASK), d)

#define sfr8(a)					_rd8( SFLS_BASE | ((a)&FLASH_ADDR_MASK))
#define sfr32(a)				_rd32(SFLS_BASE | ((a)&FLASH_ADDR_MASK))


#define UNUSED_ARG(x)			((void)(x))


// Arithmetic
#define ABS(x)					( (x) >= 0  ? (x) : -(x) )
#define MIN(x,y)				( (x) < (y) ? (x) :  (y) )
#define MAX(x,y)				( (x) > (y) ? (x) :  (y) )
#define MINMAX(x,min,max)		MIN(MAX(x, min), max)

#define ABSDIFF(A,B)			( (A)>(B) ? (A)-(B) : (B)-(A) )

#define	UROUND_DIV(NUME,DENO)	((((NUME)<<1) / (DENO) + 1) >> 1)	// Division and Round off unsigned number

#define GetBit(V,HB,LB)		(((V)>>(LB))&((1<<((HB)-(LB)+1))-1))
#define SetBit(V,HB,LB,D)	(V) = (((V) & (~(((1<<((HB)-(LB)+1))-1)<<(LB)))) | ((D)<<(LB)))

#define numberof(Arr)			(sizeof(Arr)/sizeof(Arr[0]))

#define CHANGE_VAL0(TYPE,DEF,VAL,TEMP,...) {\
			static TYPE d1_##TEMP = DEF;\
			if(VAL != d1_##TEMP) { __VA_ARGS__;	d1_##TEMP = VAL; }\
		}

#define CHANGE_VAL1(TYPE,DEF,VAL,TEMP,...) \
			if(gbSensUpdate) return;\
			static TYPE d1_##TEMP = DEF;\
			if(VAL == d1_##TEMP) return;\
			d1_##TEMP = VAL;

#define CHANGE_VAL2(TYPE,DEF,VAL,TEMP,...) \
			static TYPE d0_##TEMP = DEF;\
			if(gbSensUpdate==0) { d0_##TEMP = VAL; return; }\
			static TYPE d1_##TEMP = DEF;\
			if(/*VAL*/d0_##TEMP == d1_##TEMP) return;\
			/*gbSensUpdateCnt++;*/\
			d1_##TEMP = /*VAL*/d0_##TEMP;\
			VAL = d0_##TEMP;

#define CHANGE_VALt(TYPE,DEF,VAL,TEMP,...)	CHANGE_VAL2(TYPE,DEF,VAL,TEMP,__VA_ARGS__)
#define CHANGE_VAL(TYPE,DEF,VAL,...)		CHANGE_VALt(TYPE,DEF,VAL,__LINE__,__VA_ARGS__)

#define CHANGE_VAL2t(TYPE,DEF,VAL,TEMP,...)	CHANGE_VAL1(TYPE,DEF,VAL,TEMP,__VA_ARGS__)
#define CHANGE_VALx(TYPE,DEF,VAL,...)		CHANGE_VAL2t(TYPE,DEF,VAL,__LINE__,__VA_ARGS__)


//-------------------------------------------------------------------------------------------------
// Misc
#if 0	// 20150808 - It defined in "stdlib.h"
#define	NULL		0
#endif
#define	OK			1
#define	NG			0
#define YES			OK
#define	NO			NG

#define BitMask_01			0x1
#define BitMask_02			0x3
#define BitMask_03			0x7
#define BitMask_04			0xf
#define BitMask_05			0x1f
#define BitMask_06			0x3f
#define BitMask_07			0x7f
#define BitMask_08			0xff
#define BitMask_09			0x1ff
#define BitMask_10			0x3ff
#define BitMask_11			0x7ff
#define BitMask_12			0xfff
#define BitMask_13			0x1fff
#define BitMask_14			0x3fff
#define BitMask_15			0x7fff
#define BitMask_16			0xffff
#define BitMask_17			0x1ffff
#define BitMask_18			0x3ffff
#define BitMask_19			0x7ffff
#define BitMask_20			0xfffff
#define BitMask_21			0x1fffff
#define BitMask_22			0x3fffff
#define BitMask_23			0x7fffff
#define BitMask_24			0xffffff
#define BitMask_25			0x1ffffff
#define BitMask_26			0x3ffffff
#define BitMask_27			0x7ffffff
#define BitMask_28			0xfffffff
#define BitMask_29			0x1fffffff
#define BitMask_30			0x3fffffff
#define BitMask_31			0x7fffffff
#define BitMask_32			0xffffffff

#define ValSft_R00(val)		((val)>> 0)
#define ValSft_R01(val)		((val)>> 1)
#define ValSft_R02(val)		((val)>> 2)
#define ValSft_R03(val)		((val)>> 3)
#define ValSft_R04(val)		((val)>> 4)
#define ValSft_R05(val)		((val)>> 5)
#define ValSft_R06(val)		((val)>> 6)
#define ValSft_R07(val)		((val)>> 7)
#define ValSft_R08(val)		((val)>> 8)
#define ValSft_R09(val)		((val)>> 9)
#define ValSft_R10(val)		((val)>>10)
#define ValSft_R11(val)		((val)>>11)
#define ValSft_R12(val)		((val)>>12)
#define ValSft_R13(val)		((val)>>13)
#define ValSft_R14(val)		((val)>>14)
#define ValSft_R15(val)		((val)>>15)
#define ValSft_R16(val)		((val)>>16)
#define ValSft_R17(val)		((val)>>17)
#define ValSft_R18(val)		((val)>>18)
#define ValSft_R19(val)		((val)>>19)
#define ValSft_R20(val)		((val)>>20)
#define ValSft_R21(val)		((val)>>21)
#define ValSft_R22(val)		((val)>>22)
#define ValSft_R23(val)		((val)>>23)
#define ValSft_R24(val)		((val)>>24)
#define ValSft_R25(val)		((val)>>25)
#define ValSft_R26(val)		((val)>>26)
#define ValSft_R27(val)		((val)>>27)
#define ValSft_R28(val)		((val)>>28)
#define ValSft_R29(val)		((val)>>29)
#define ValSft_R30(val)		((val)>>30)
#define ValSft_R31(val)		((val)>>31)

#define ValSft_L00(val)		((val)<< 0)
#define ValSft_L01(val)		((val)<< 1)
#define ValSft_L02(val)		((val)<< 2)
#define ValSft_L03(val)		((val)<< 3)
#define ValSft_L04(val)		((val)<< 4)
#define ValSft_L05(val)		((val)<< 5)
#define ValSft_L06(val)		((val)<< 6)
#define ValSft_L07(val)		((val)<< 7)
#define ValSft_L08(val)		((val)<< 8)
#define ValSft_L09(val)		((val)<< 9)
#define ValSft_L10(val)		((val)<<10)
#define ValSft_L11(val)		((val)<<11)
#define ValSft_L12(val)		((val)<<12)
#define ValSft_L13(val)		((val)<<13)
#define ValSft_L14(val)		((val)<<14)
#define ValSft_L15(val)		((val)<<15)
#define ValSft_L16(val)		((val)<<16)
#define ValSft_L17(val)		((val)<<17)
#define ValSft_L18(val)		((val)<<18)
#define ValSft_L19(val)		((val)<<19)
#define ValSft_L20(val)		((val)<<20)
#define ValSft_L21(val)		((val)<<21)
#define ValSft_L22(val)		((val)<<22)
#define ValSft_L23(val)		((val)<<23)
#define ValSft_L24(val)		((val)<<24)
#define ValSft_L25(val)		((val)<<25)
#define ValSft_L26(val)		((val)<<26)
#define ValSft_L27(val)		((val)<<27)
#define ValSft_L28(val)		((val)<<28)
#define ValSft_L29(val)		((val)<<29)
#define ValSft_L30(val)		((val)<<30)
#define ValSft_L31(val)		((val)<<31)

//-------------------------------------------------------------------------------------------------
// Test
#define ENABLE_I2C_ERR		0

#define ENABLE_WAIT_VLOCK	1

#define GRP_NUMBER			8

#define STR_I2C_MODE		0		// TxStrNoIRQ() in I2c_mode()

#define STATUS_ON			(gbMnDebugFnc == 8)

#if ENABLE_I2C_ERR == 1
	#define I2C_STA				BYTE bErr = 0;
	#define I2C_END				I2c_Err: TxStrNoIRQ("I2C ERR :", bErr, 2);  I2c_mode(0); return bErr;
	#define I2C_CODE(ERR,CODE)	if(CODE) { bErr = ERR; goto I2c_Err; }
#else
	#define I2C_STA
	#define I2C_END
	#define I2C_CODE(ERR,CODE)	CODE;
#endif

#if ENABLE_WAIT_VLOCK == 1
	#define EWV(T,F)	T
#else
	#define EWV(T,F)	F
#endif

#if STR_UART_DATA == 1
	#define SUD(T) T
#else
	#define SUD(T)
#endif

#if STR_I2C_DATA == 1
	#define SID(T) T
#else
	#define SID(T)
#endif

#if TEST_ISP_IRQ == 1
	#define TII(T) T
#else
	#define TII(T)
#endif


#if   ((model_TgtBdDly%10)==0)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val0
#elif ((model_TgtBdDly%10)==1)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val1
#elif ((model_TgtBdDly%10)==2)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val2
#elif ((model_TgtBdDly%10)==3)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val3
#elif ((model_TgtBdDly%10)==4)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val4
#elif ((model_TgtBdDly%10)==5)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val5
#elif ((model_TgtBdDly%10)==6)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val6
#elif ((model_TgtBdDly%10)==7)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val7
#elif ((model_TgtBdDly%10)==8)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val8
#elif ((model_TgtBdDly%10)==9)
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val9
#else
#define		DLY_VAL(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)
#endif


#if   ((model_Sens%10)==0)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val0
#elif ((model_Sens%10)==1)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val1
#elif ((model_Sens%10)==2)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val2
#elif ((model_Sens%10)==3)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val3
#elif ((model_Sens%10)==4)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val4
#elif ((model_Sens%10)==5)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val5
#elif ((model_Sens%10)==6)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val6
#elif ((model_Sens%10)==7)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val7
#elif ((model_Sens%10)==8)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val8
#elif ((model_Sens%10)==9)
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		val9
#else
#define		DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)
#endif

#ifndef __ISP_LIB__

#if (model_Par==0)
	#define		DFSEL_Par(par, def)		def

	//#define		ParDef(N,V)		enum { N = V };
	#define		ParSet(N,D)		
	#define		UP(N)			N
#else
	#define		DFSEL_Par(par, def)		def//par

	//#define		ParDef(N,V)		enum { N = V };//enum { N##_##_DEF = V }; /*enum { N##_SIZ = S };*/

	#define		ParSet0(N,D)	{if(N##_SIZ==4)     { gbUsrParTbl[N##_IDX] = (BYTE)(D); gbUsrParTbl[N##_IDX+1] = (BYTE)((D)>>8); gbUsrParTbl[N##_IDX+2] = (BYTE)((D)>>16); gbUsrParTbl[N##_IDX+3] = (BYTE)((D)>>24); }\
								else if(N##_SIZ==3) { gbUsrParTbl[N##_IDX] = (BYTE)(D); gbUsrParTbl[N##_IDX+1] = (BYTE)((D)>>8); gbUsrParTbl[N##_IDX+2] = (BYTE)((D)>>16); }\
								else if(N##_SIZ==2) { gbUsrParTbl[N##_IDX] = (BYTE)(D); gbUsrParTbl[N##_IDX+1] = (BYTE)((D)>>8); }\
								else                { gbUsrParTbl[N##_IDX] = (BYTE)(D); } }

	#define		ParSet(N,D)		ParSet0(N##_,D)

	#define		UP0(N)			((N##_SIZ==4) ? gbUsrParTbl[N##_IDX] | (gbUsrParTbl[N##_IDX+1]<<8) | (gbUsrParTbl[N##_IDX+2]<<16) | (gbUsrParTbl[N##_IDX+3]<<24) :\
								 (N##_SIZ==3) ? gbUsrParTbl[N##_IDX] | (gbUsrParTbl[N##_IDX+1]<<8) | (gbUsrParTbl[N##_IDX+2]<<16) :\
								 (N##_SIZ==2) ? gbUsrParTbl[N##_IDX] | (gbUsrParTbl[N##_IDX+1]<<8) :\
								                gbUsrParTbl[N##_IDX] )

	#define		UP(N)			UP0(N##_)

	#define		UP_ADDR(N)		N##_##_IDX
#endif

//	#define 	DFSEL_ParSens(par, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9)		DFSEL_Par(par, DFSEL_Sens(val0, val1, val2, val3, val4, val5, val6, val7, val8, val9))

#define		xUP(N)			N

#endif//__ISP_LIB__


#if (model_Par==0)
#define USR_PAR_LIST
enum {
	UP_START=0,
	UP_END_REAL
};
#else

#define PAR_LIST1(N,...)	N##_##_IDX __VA_ARGS__,
#define PAR_LIST2(N,...)	PAR_LIST1(N,__VA_ARGS__) N##_##1_IDX,
#define PAR_LIST3(N,...)	PAR_LIST2(N,__VA_ARGS__) N##_##2_IDX,
#define PAR_LIST4(N,...)	PAR_LIST3(N,__VA_ARGS__) N##_##3_IDX,

#define UP_SET(S,N,...)		PAR_LIST##S(N,__VA_ARGS__)
#include "isp_app_para_tbl.h"
enum {
	UP_START=0,
	USR_PAR_LIST
	UP_END_REAL
};

#undef UP_SET
#define UP_SET(S,N,...)		enum { N##_##_SIZ = S };
#include "isp_app_para_tbl.h"
USR_PAR_LIST

#endif


//----------------------------------------------------------------------------------------
// Library Link
#if   model_Sens ==			SENS_MN34210
	#define Sens(a) a##___##SENS_MN34210
#elif model_Sens ==			SENS_IMX225
	#define Sens(a) a##___##SENS_IMX225
#elif model_Sens ==			SENS_MN34229
	#define Sens(a) a##___##SENS_MN34229
#elif model_Sens ==			SENS_MN34420
	#define Sens(a) a##___##SENS_MN34420
#elif model_Sens ==			SENS_IMX291
	#define Sens(a) a##___##SENS_IMX291
#elif model_Sens ==			SENS_IMX327
	#define Sens(a) a##___##SENS_IMX327
#elif model_Sens ==			SENS_IMX307
	#define Sens(a) a##___##SENS_IMX307
#elif model_Sens ==			SENS_MN34227
	#define Sens(a) a##___##SENS_MN34227
#elif model_Sens ==			SENS_IMX323
	#define Sens(a) a##___##SENS_IMX323
#elif model_Sens ==			SENS_OV2718
	#define Sens(a) a##___##SENS_OV2718
#elif model_Sens ==			SENS_AR0237CSH
	#define Sens(a) a##___##SENS_AR0237CSH
#elif model_Sens ==			SENS_AR0237CSP
	#define Sens(a) a##___##SENS_AR0237CSP
#else
	//#define Sens(a) a##___##NOSENS
	#define Sens(a) a
#endif

