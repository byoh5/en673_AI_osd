//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		dev_regs.h
//	Description:	EN673 device register defininition
//	Author:			EN673 team
//
//	Note:			Only include register definition and macros
//					DO NOT INCLUDE function definition here. It will increase the code size!!
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151026		ygkim		separated from "dev.h" to share the register information with IP SW
//	0.2	160601		whlee		EN673 Device driver Reg defines (Modified by EN673)
//------------------------------------------------------------------------------
#ifndef _DEV_REGS_H_
#define	_DEV_REGS_H_

/*
 INDEX	:
		 1. TYPE DEF
		 2. Memory Size & Base Address Definition
		 3. IRQ Number Def
		 4. SYSTEM
		 5. UTIL
		 6. ADC
		 7. CPU
		 8. DDR
		 9. DMA
		10. GPIO
		11. ISR
		12. SERIAL FLASH
		13. TIMER
		14. ETHERNET
		15. H.264
		16. SDIO
		17. I2C
		18. AUD
		19. SPI
		20. UART
		21. DEVICE TEST
		22. ISP
		23. PAD
*/


//******************************************************************************
// 1. TYPE DEF
//------------------------------------------------------------------------------
#include "dev_types.h"

//------------------------------------------------------------------------------
//
#define	_regs_					typedef	union{ULONG a; struct{	// Register union start
#define	_rege_					};}							// Register union end
#define	_am(uon,base,addr)		((volatile uon*)(UINT*)(base+(addr)))->a		// All macro
#define	_bm(uon,base,addr,mne)	((volatile uon*)(UINT*)(base+(addr)))->mne	// Bit macro


//******************************************************************************
// 2. Memory Size & Base Address Definition
//------------------------------------------------------------------------------
// Memory Size (board.h define)

// Base Address Definition
#define ETHRX_BASE				0x90000000	//	OK
#define ETHTX_BASE				0x91000000	//	OK
#if SDIO_CH_SWAP
#define SDIO0_BASE				0x93000000	//	OK
#define SDIO1_BASE				0x92000000	//	OK
#else
#define SDIO0_BASE				0x92000000	//	OK
#define SDIO1_BASE				0x93000000	//	OK
#endif
#define AUD_BASE				0x94000000	//	OK
#define ADC_BASE				0x95000000	//	OK
#define UTIL_BASE				0x96000000	//	OK

// Reg address
#define	DMA_REG_BASE			0xf1000000	//	CPU0 | CPU1  OK
#define ISP_REG_BASE			0xf1100000	//	CPU0 | CPU1  OK
#define UART0_REG_BASE			0xf8000000	//	CPU0 & CPU1  OK
#define UART1_REG_BASE			0xf8100000	//	CPU0 & CPU1  OK
#define UART2_REG_BASE			0xf8200000	//	CPU0 & CPU1  OK
#define GPIO_REG_BASE			0xf8300000	//	CPU0 & CPU1  OK
#define I2C0_REG_BASE			0xf8400000	//	CPU0 & CPU1  OK
#define I2C1_REG_BASE			0xf8500000	//	CPU0 & CPU1  OK
#define I2C2_REG_BASE			0xf8600000	//	CPU0 & CPU1  OK
#define SPI0_REG_BASE			0xf8700000	//	CPU0 & CPU1  OK
#define SPI1_REG_BASE			0xf8800000	//	CPU0 & CPU1  OK
#define SPI2_REG_BASE			0xf8900000	//	CPU0 & CPU1  OK
#define TIMER_REG_BASE			0xf8a00000	//	CPU0 & CPU1  OK
#define AUD_REG_BASE			0xf8b00000	//	CPU0 & CPU1  OK
#define ETH_REG_BASE			0xf8c00000	//	CPU0 & CPU1  OK
#if SDIO_CH_SWAP
#define SDIO0_REG_BASE			0xf8e00000	//	CPU0 & CPU1  OK
#define SDIO1_REG_BASE			0xf8d00000	//	CPU0 & CPU1  OK
#else
#define SDIO0_REG_BASE			0xf8d00000	//	CPU0 & CPU1  OK
#define SDIO1_REG_BASE			0xf8e00000	//	CPU0 & CPU1  OK
#endif
#define	H264_REG_BASE			0xf8f00000	//	CPU0 & CPU1  OK
#define UTIL_REG_BASE			0xf9000000	//  CPU0 & CPU1  OK
#define ADC_REG_BASE			0xf9100000	//	CPU0 & CPU1  OK
#define	SFLS_REG_BASE			0xf9200000	//	CPU0 & CPU1  OK
#define DDR_REG_BASE			0xf9300000	//	CPU0 & CPU1  OK
#define CPU_REG_BASE			0xf9400000	//	CPU0 & CPU1  OK
#define	SYS_REG_BASE			0xf9500000	//	CPU0 & CPU1  OK
#define	PAD_REG_BASE			0xf9600000	//	CPU0 & CPU1  OK

#define TRUE					1
#define FALSE					0

//******************************************************************************
// 3. IRQ Number Def
//------------------------------------------------------------------------------

#define	IRQ_CPU					2
#define	IRQ_H264				3
#define	IRQ_MERGE				4
#define	IRQ_ISP					5
#define	IRQ_DMA					6
#define	IRQ_ETH					7
#if SDIO_CH_SWAP
#define	IRQ_SDIO0				9
#define	IRQ_SDIO1				8
#else
#define	IRQ_SDIO0				8
#define	IRQ_SDIO1				9
#endif
#define	IRQ_SFLS				10
#define IRQ_ADC					11
#define IRQ_AUD					12
#define IRQ_TIMER				13
#define	IRQ_UART0				14
#define	IRQ_UART1				15
#define	IRQ_UART2				16
#define	IRQ_GPIO				17
#define	IRQ_I2C0				18
#define	IRQ_I2C1				19
#define	IRQ_I2C2				20
#define	IRQ_SPI0				21
#define	IRQ_SPI1				22
#define	IRQ_SPI2				23
//-------------------------------------------------------------------------------------------------
// Macro
#define MAX_INT_HANDLERS	22		// Number of interrupt handlers

struct tIhnd {						// Handler entry
	void 	(*handler)(void *);
	void	*arg;
};

//******************************************************************************
// 4. SYSTEM
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT PLL0_AUTO_RS   : 1;    //[   31]: oPLL0_AUTO_RS;
        UINT PLL0_BP        : 1;    //[   30]: oPLL0_BP;
        UINT _res0          :10;    //[19:16]: oPLL0_M;
        UINT PLL0_M         : 4;    //[14: 8]: oPLL0_N;
        UINT _res1          : 1;    //[ 6: 0]: oPLL0_K;
        UINT PLL0_N         : 7;
        UINT _res2          : 1;
        UINT PLL0_K         : 7;     _rege_  _SYS_PLL0;

_regs_  UINT PLL1_AUTO_RS   : 1;    //[   31]: oPLL1_AUTO_RS;
        UINT PLL1_BP        : 1;    //[   30]: oPLL1_BP;
        UINT PLL1_PD        : 1;    //[19:16]: oPLL1_M;
        UINT _res0          : 9;    //[14: 8]: oPLL1_N;
        UINT PLL1_M         : 4;    //[ 6: 0]: oPLL1_K;
        UINT _res1          : 1;
        UINT PLL1_N         : 7;
        UINT _res2          : 1;
        UINT PLL1_K         : 7;     _rege_  _SYS_PLL1;

_regs_    UINT _res0        :25;    //[6]: oSYS_WDREQ;
        UINT SYS_WDREQ      : 1;    //[5]: DDR_RST;
        UINT DDR_RST        : 1;    //[4]: H264_RST;
        UINT H264_RST       : 1;    //[3]: ISP_RST;
        UINT ISP_RST        : 1;    //[1]: CPU1_RST;
        UINT _res1          : 1;    //[0]: CPU0_RST;
        UINT CPU1_RST       : 1;
        UINT CPU0_RST       : 1;    _rege_  _SYS_RST;

_regs_                              _rege_  _SYS_IO_MUX;

_regs_  UINT HCK_PD         : 1;  // [31]  : oHCK_PD;
        UINT DCK_PD         : 1;  // [30]  : oDCK_PD;
        UINT CPU1_CK_EN     : 1;  // [29]  : oCPU1_CK_EN;
        UINT _res0          :11;
        UINT JTAG_SEL       : 1;  // [17]  : oJTAG_SEL;
        UINT PLL1_CKEN      : 1;  // [16]  : oPLL1_CKEN;
        UINT _res1          : 2;  //
        UINT PLL1_ACTL_EN   : 1;  // [13]  : PLL1_CH_EN;
        UINT PLL0_ACTL_EN   : 1;  // [12]  : PLL0_CH_EN;
        UINT FONT_WLTC      : 3;  // [11:9]: oFONT_WLTC;
        UINT _res2          : 1;  //
        UINT DCK_SEL        : 1;  // [7]   : oDCK_SEL;
        UINT MBCK_SEL       : 2;  // [6:5] : oMBCK_SEL;
        UINT _res3          : 3;  //
        UINT HCK_SEL        : 1;  // [1]   : oHCK_SEL;
        UINT _res4          : 1;    _rege_  _SYS_CLK_SEL;

_regs_                              _rege_  _SYS_WDT;

_regs_  UINT _res0          :30;
        UINT WDT_RST        : 1;
        UINT WDT_EN         : 1;    _rege_  _SYS_WDT_CFG;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define SYS_PLL0_AUTO_RS    _bm(_SYS_PLL0     , SYS_REG_BASE , 0x00, PLL0_AUTO_RS   )   // RW
#define SYS_PLL0_BP         _bm(_SYS_PLL0     , SYS_REG_BASE , 0x00, PLL0_BP        )   // RW
#define SYS_PLL0_M          _bm(_SYS_PLL0     , SYS_REG_BASE , 0x00, PLL0_M         )   // RW
#define SYS_PLL0_N          _bm(_SYS_PLL0     , SYS_REG_BASE , 0x00, PLL0_N         )   // RW
#define SYS_PLL0_K          _bm(_SYS_PLL0     , SYS_REG_BASE , 0x00, PLL0_K         )   // RW
#define SYS_PLL0_T          _am(_SYS_PLL0     , SYS_REG_BASE , 0x00                 )   // RW

#define SYS_PLL1_AUTO_RS    _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_AUTO_RS   )   // RW
#define SYS_PLL1_BP         _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_BP        )   // RW
#define SYS_PLL1_PD         _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_PD        )   // RW
#define SYS_PLL1_M          _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_M         )   // RW
#define SYS_PLL1_N          _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_N         )   // RW
#define SYS_PLL1_K          _bm(_SYS_PLL1     , SYS_REG_BASE , 0x04, PLL1_K         )   // RW
#define SYS_PLL1_T          _am(_SYS_PLL1     , SYS_REG_BASE , 0x04                 )   // RW

#define SYS_RST             _am(_SYS_RST     , SYS_REG_BASE , 0x0c                  )   // RW
#define SYS_SYS_RSTREQ      _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, SYS_WDREQ       )   // RW
#define SYS_DDR_RST         _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, DDR_RST         )   // RW
#define SYS_H264_RST        _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, H264_RST        )   // RW
#define SYS_ISP_RST         _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, ISP_RST         )   // RW
#define SYS_CPU1_RST        _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, CPU1_RST        )   // RW
#define SYS_CPU0_RST        _bm(_SYS_RST     , SYS_REG_BASE , 0x0c, CPU0_RST        )   // RW

#define SYS_IO_MUX          _am(_SYS_IO_MUX  , SYS_REG_BASE , 0x14                  )   // RW

#define SYS_HCK_PD          _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, HCK_PD          )   // RW
#define SYS_DCK_PD          _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, DCK_PD          )   // RW
#define SYS_CPU1_CK_EN      _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, CPU1_CK_EN      )   // RW
#define SYS_JTAG_SEL        _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, JTAG_SEL        )   // RW
#define SYS_PLL1_CKEN       _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, PLL1_CKEN       )   // RW
#define SYS_PLL1_ACTL_EN    _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, PLL1_ACTL_EN    )   // RW
#define SYS_PLL0_ACTL_EN    _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, PLL0_ACTL_EN    )   // RW
#define SYS_FONT_WLTC       _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, FONT_WLTC       )   // RW
#define SYS_DCK_SEL         _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, DCK_SEL         )   // RW
#define SYS_MBCK_SEL        _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, MBCK_SEL        )   // RW
#define SYS_HCK_SEL         _bm(_SYS_CLK_SEL , SYS_REG_BASE , 0x1c, HCK_SEL         )   // RW

#define SYS_WDT_CNT         _am(_SYS_WDT     , SYS_REG_BASE , 0x20                  )   // RW

#define SYS_WDT_LMT         _am(_SYS_WDT     , SYS_REG_BASE , 0x24                  )   // RW

#define SYS_WDT_RST         _bm(_SYS_WDT_CFG , SYS_REG_BASE , 0x28, WDT_RST         )   // RW
#define SYS_WDT_EN          _bm(_SYS_WDT_CFG , SYS_REG_BASE , 0x28, WDT_EN          )   // RW
#define SYS_WDT_T           _am(_SYS_WDT_CFG , SYS_REG_BASE , 0x28                  )   // RW

//------------------------------------------------------------------------------
// Extern
#ifndef _INLINE
extern void WdtSysSetStop(void);
extern void WdtSysCountReset(void);
extern void WdtSysSetTime(UINT sec);
extern void WdtSysReboot(void);
#endif

//******************************************************************************
// 5. UTIL
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res0          :24;
        UINT ENC_LAW        : 1;
        UINT DEC_LAW        : 1;
        UINT ENDI_SIZE      : 2;
        UINT _res1          : 1;
        UINT MODE           : 3;    _rege_  _UTIL_CONT;

_regs_  UINT AESKEY         :32;    _rege_  _UTIL_AESKEY;

_regs_  UINT _res           :16;
        UINT CHKSUM         :16;    _rege_  _UTIL_CHKSUM;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define UTIL_ENC_LAW        _bm(_UTIL_CONT   , UTIL_REG_BASE, 0x00, ENC_LAW         )   // RW
#define UTIL_DEC_LAW        _bm(_UTIL_CONT   , UTIL_REG_BASE, 0x00, DEC_LAW         )   // RW
#define UTIL_ENDI_SIZE      _bm(_UTIL_CONT   , UTIL_REG_BASE, 0x00, ENDI_SIZE       )   // RW
#define UTIL_MODE           _bm(_UTIL_CONT   , UTIL_REG_BASE, 0x00, MODE            )   // RW

#define UTIL_AESKEY0        _am(_UTIL_AESKEY , UTIL_REG_BASE, 0x04                  )   // RW
#define UTIL_AESKEY1        _am(_UTIL_AESKEY , UTIL_REG_BASE, 0x08                  )   // RW
#define UTIL_AESKEY2        _am(_UTIL_AESKEY , UTIL_REG_BASE, 0x0c                  )   // RW
#define UTIL_AESKEY3        _am(_UTIL_AESKEY , UTIL_REG_BASE, 0x10                  )   // RW

#define UTIL_CHKSUM         _bm(_UTIL_CHKSUM , UTIL_REG_BASE, 0x14, CHKSUM          )   // RO

//------------------------------------------------------------------------------
// Extern

enum {
	G711_aLaw = 0,
	G711_uLaw = 1
};

//	extern void AesInit();
extern void AesEncryt(BYTE * Dst, BYTE * Src, UINT BkCnt);
extern WORD ChkSum(BYTE * Src, UINT Len);
extern void EndianConv(BYTE * Dst, BYTE * Src, UINT Len, BYTE Size);
extern void G711Enc(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law);
extern void G711Dec(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law);
extern void PrintHex(BYTE * Dat, UINT Len);

//******************************************************************************
// 6. ADC
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT CLK_LMT        :16;
        UINT _res0          : 6;
        UINT CKEN           : 1;
        UINT EN             : 1;
        UINT _res1          : 5;
        UINT IRQ            : 1;
        UINT IRQ_EN         : 1;
        UINT IRQ_CLR        : 1;    _rege_  _ADC_CFG;

_regs_  UINT EN             : 1;
        UINT CH             : 2;
        UINT _res0          : 5;
        UINT _res1          : 8;
        UINT _res2          : 1;
        UINT BSIGN          : 1;
        UINT MUL            : 3;
        UINT LMT            :11;    _rege_  _BUF_CFG;

_regs_  UINT EN             : 1;
        UINT CSIGN          : 1;
        UINT MUL            : 3;
        UINT _res           :11;
        UINT DAT            :16;    _rege_  _CH_CFG;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define ADC_CLK_LMT         _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, CLK_LMT      )   // RW
#define ADC_CKEN            _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, CKEN         )   // RW
#define ADC_EN              _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, EN           )   // RW
#define ADC_IRQ             _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, IRQ          )   // RO
#define ADC_IRQ_EN          _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, IRQ_EN       )   // RW
#define ADC_IRQ_CLR         _bm(_ADC_CFG        , ADC_REG_BASE , 0x00, IRQ_CLR      )   // WO

#define ADC_BUF_EN          _bm(_BUF_CFG        , ADC_REG_BASE , 0x04, EN           )   // RW
#define ADC_BUF_CH          _bm(_BUF_CFG        , ADC_REG_BASE , 0x04, CH           )   // RW
#define ADC_BUF_SIGN        _bm(_BUF_CFG        , ADC_REG_BASE , 0x04, BSIGN        )   // RW
#define ADC_BUF_MUL         _bm(_BUF_CFG        , ADC_REG_BASE , 0x04, MUL          )   // RW
#define ADC_BUF_LMT         _bm(_BUF_CFG        , ADC_REG_BASE , 0x04, LMT          )   // RW

#define ADC_CH0_EN          _bm(_CH_CFG         , ADC_REG_BASE , 0x08, EN           )   // RW
#define ADC_CH0_SIGN        _bm(_CH_CFG         , ADC_REG_BASE , 0x08, CSIGN        )   // RW
#define ADC_CH0_MUL         _bm(_CH_CFG         , ADC_REG_BASE , 0x08, MUL          )   // RW
#define ADC_CH0_DAT         _bm(_CH_CFG         , ADC_REG_BASE , 0x08, DAT          )   // RW

#define ADC_CH1_EN          _bm(_CH_CFG         , ADC_REG_BASE , 0x0c, EN           )   // RW
#define ADC_CH1_SIGN        _bm(_CH_CFG         , ADC_REG_BASE , 0x0c, CSIGN        )   // RW
#define ADC_CH1_MUL         _bm(_CH_CFG         , ADC_REG_BASE , 0x0c, MUL          )   // RW
#define ADC_CH1_DAT         _bm(_CH_CFG         , ADC_REG_BASE , 0x0c, DAT          )   // RW

#define ADC_CH2_EN          _bm(_CH_CFG         , ADC_REG_BASE , 0x10, EN           )   // RW
#define ADC_CH2_SIGN        _bm(_CH_CFG         , ADC_REG_BASE , 0x10, CSIGN        )   // RW
#define ADC_CH2_MUL         _bm(_CH_CFG         , ADC_REG_BASE , 0x10, MUL          )   // RW
#define ADC_CH2_DAT         _bm(_CH_CFG         , ADC_REG_BASE , 0x10, DAT          )   // RW

//------------------------------------------------------------------------------
// Extern
extern void AdcInit(void);
extern void AdcIsr(void);

//******************************************************************************
// 7. CPU
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res1          :26;
        UINT ID             : 2;
        UINT _res2          : 2;
        UINT IRQ1           : 1;
        UINT IRQ0           : 1;    _rege_  _CPU_INFO;

_regs_  UINT _res           :30;
        UINT IRQ_EN         : 1;
        UINT IRQ_CLR        : 1;    _rege_  _CPU_IRQ;

_regs_  UINT _res           :30;
        UINT WDT_RST        : 1;
        UINT WDT_EN         : 1;    _rege_  _CPU_WDT;

_regs_                              _rege_  _CPU_WDTCNT;

_regs_                              _rege_  _CPU_KEY;

_regs_                              _rege_  _CPU_SHARED;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define CPU_ID              _bm(_CPU_INFO       , CPU_REG_BASE , 0x00, ID           )   // RO
#define CPU_IRQ1            _bm(_CPU_INFO       , CPU_REG_BASE , 0x00, IRQ1         )   // RO
#define CPU_IRQ0            _bm(_CPU_INFO       , CPU_REG_BASE , 0x00, IRQ0         )   // RO

#define CPU_IRQ0_EN         _bm(_CPU_IRQ        , CPU_REG_BASE , 0x04, IRQ_EN       )   // RW
#define CPU_IRQ0_CLR        _bm(_CPU_IRQ        , CPU_REG_BASE , 0x04, IRQ_CLR      )   // WO

#define CPU_IRQ1_EN         _bm(_CPU_IRQ        , CPU_REG_BASE , 0x08, IRQ_EN       )   // RW
#define CPU_IRQ1_CLR        _bm(_CPU_IRQ        , CPU_REG_BASE , 0x08, IRQ_CLR      )   // WO

#define CPU_WDT0_RST        _bm(_CPU_WDT        , CPU_REG_BASE , 0x10, WDT_RST      )   // WO
#define CPU_WDT0_EN         _bm(_CPU_WDT        , CPU_REG_BASE , 0x10, WDT_EN       )   // RW
#define CPU_WDT0_T          _am(_CPU_WDT        , CPU_REG_BASE , 0x10               )   // RW

#define CPU_WDT1_RST        _bm(_CPU_WDT        , CPU_REG_BASE , 0x14, WDT_RST      )   // WO
#define CPU_WDT1_EN         _bm(_CPU_WDT        , CPU_REG_BASE , 0x14, WDT_EN       )   // RW
#define CPU_WDT1_T          _am(_CPU_WDT        , CPU_REG_BASE , 0x14               )   // RW

#define CPU_WDT0_CNT        _am(_CPU_WDTCNT     , CPU_REG_BASE , 0x1c               )   // RO

#define CPU_WDT1_CNT        _am(_CPU_WDTCNT     , CPU_REG_BASE , 0x20               )   // RO

#define CPU_WDT0_LMT        _am(_CPU_WDTCNT     , CPU_REG_BASE , 0x28               )   // RW

#define CPU_WDT1_LMT        _am(_CPU_WDTCNT     , CPU_REG_BASE , 0x2c               )   // RW

//------------------------------------------------------------------------------
// Critical Section handling for sharing resources: ID = 0...31
#define CPU_KEY0            _am(_CPU_KEY        , CPU_REG_BASE , 0x40               )   // RW
#define CPU_KEY1            _am(_CPU_KEY        , CPU_REG_BASE , 0x44               )   // RW
#define CPU_KEY2            _am(_CPU_KEY        , CPU_REG_BASE , 0x48               )   // RW
#define CPU_KEY3            _am(_CPU_KEY        , CPU_REG_BASE , 0x4C               )   // RW
#define CPU_KEY4            _am(_CPU_KEY        , CPU_REG_BASE , 0x50               )   // RW
#define CPU_KEY5            _am(_CPU_KEY        , CPU_REG_BASE , 0x54               )   // RW
#define CPU_KEY6            _am(_CPU_KEY        , CPU_REG_BASE , 0x58               )   // RW
#define CPU_KEY7            _am(_CPU_KEY        , CPU_REG_BASE , 0x5C               )   // RW
#define CPU_KEY8            _am(_CPU_KEY        , CPU_REG_BASE , 0x60               )   // RW
#define CPU_KEY9            _am(_CPU_KEY        , CPU_REG_BASE , 0x64               )   // RW
#define CPU_KEY10           _am(_CPU_KEY        , CPU_REG_BASE , 0x68               )   // RW
#define CPU_KEY11           _am(_CPU_KEY        , CPU_REG_BASE , 0x6C               )   // RW
#define CPU_KEY12           _am(_CPU_KEY        , CPU_REG_BASE , 0x70               )   // RW
#define CPU_KEY13           _am(_CPU_KEY        , CPU_REG_BASE , 0x74               )   // RW
#define CPU_KEY14           _am(_CPU_KEY        , CPU_REG_BASE , 0x78               )   // RW
#define CPU_KEY15           _am(_CPU_KEY        , CPU_REG_BASE , 0x7C               )   // RW
#define CPU_KEY16           _am(_CPU_KEY        , CPU_REG_BASE , 0x80               )   // RW
#define CPU_KEY17           _am(_CPU_KEY        , CPU_REG_BASE , 0x84               )   // RW
#define CPU_KEY18           _am(_CPU_KEY        , CPU_REG_BASE , 0x88               )   // RW
#define CPU_KEY19           _am(_CPU_KEY        , CPU_REG_BASE , 0x8C               )   // RW
#define CPU_KEY20           _am(_CPU_KEY        , CPU_REG_BASE , 0x90               )   // RW
#define CPU_KEY21           _am(_CPU_KEY        , CPU_REG_BASE , 0x94               )   // RW
#define CPU_KEY22           _am(_CPU_KEY        , CPU_REG_BASE , 0x98               )   // RW
#define CPU_KEY23           _am(_CPU_KEY        , CPU_REG_BASE , 0x9C               )   // RW
#define CPU_KEY24           _am(_CPU_KEY        , CPU_REG_BASE , 0xA0               )   // RW
#define CPU_KEY25           _am(_CPU_KEY        , CPU_REG_BASE , 0xA4               )   // RW
#define CPU_KEY26           _am(_CPU_KEY        , CPU_REG_BASE , 0xA8               )   // RW
#define CPU_KEY27           _am(_CPU_KEY        , CPU_REG_BASE , 0xAC               )   // RW
#define CPU_KEY28           _am(_CPU_KEY        , CPU_REG_BASE , 0xB0               )   // RW
#define CPU_KEY29           _am(_CPU_KEY        , CPU_REG_BASE , 0xB4               )   // RW
#define CPU_KEY30           _am(_CPU_KEY        , CPU_REG_BASE , 0xB8               )   // RW
#define CPU_KEY31           _am(_CPU_KEY        , CPU_REG_BASE , 0xBC               )   // RW

#define CPU_SHARED0         _am(_CPU_SHARED     , CPU_REG_BASE , 0xC0               )   // RW : CPU IRQ 1 -> 0
#define CPU_SHARED1         _am(_CPU_SHARED     , CPU_REG_BASE , 0xC4               )   // RW : CPU IRQ 0 -> 1
#define CPU_SHARED2         _am(_CPU_SHARED     , CPU_REG_BASE , 0xC8               )   // RW : JTAG IRQ -> 1
#define CPU_SHARED3         _am(_CPU_SHARED     , CPU_REG_BASE , 0xCC               )   // RW

//------------------------------------------------------------------------------
// Extern

// CS Mapping: PERI_SHARED, COMMON_SHARED

#define CS_SFLS				CPU_KEY0
#define	CS_UART0			CPU_KEY1
#define	CS_ETHERNET			CPU_KEY2
#define	CS_PASSMSG    		CPU_KEY3
#define CS_WIFI_TX			CPU_KEY4
#define CS_ISPCMD			CPU_KEY5
#define CS_STMRECMSG		CPU_KEY6
#define CS_DEBUGc1			CPU_KEY7

// Enter CS: Read until 0 and set 1 immediately
// Leave CS: Write 0 to clear
#ifdef PERI_SHARED
#define	EnterPeriCS(NAME)		CRITICAL_BEGIN; 		\
								while(CS_ ## NAME);
#define	LeavePeriCS(NAME)		CS_ ##NAME = 0; 		\
								CRITICAL_END;
#else
#define	EnterPeriCS(NAME)
#define	LeavePeriCS(NAME)
#endif

#ifdef COMMON_SHARED
#define	EnterCommonCS(NAME)		CRITICAL_BEGIN;			\
								while(CS_ ## NAME);
#define	LeaveCommonCS(NAME)		CS_ ##NAME = 0;			\
								CRITICAL_END;
#else
#define	EnterCommonCS(NAME)
#define	LeaveCommonCS(NAME)
#endif

enum {
	eIP0_DGT_INPUT		= 0x00000001,	// CPU 1 to 0
// ~eIP0_xxxxxxxxx		= 0x80000000,	// CPU 1 to 0

// ~eIP1_xxxxxxxxx		= 0x00000001,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000002,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000004,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000008,	// CPU 0 to 1
	eIP1_RECORD_INFO	= 0x00000010,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000020,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000040,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000080,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000100,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000200,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000400,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00000800,	// CPU 0 to 1
#ifdef JTAG_DEBUG_PRINT
	eIP1_J_SHELL_RX		= 0x00001000,	// JTAG to CPU1
#endif

#ifdef ECM_REMOTE
	eIP0_J_UART			= 0x00002000,	// JATG UART
#endif
// ~eIP1_xxxxxxxxx		= 0x00004000,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00008000,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00010000,	// CPU 0 to 1
#ifdef __ETH__
	eIP1_ETH_RX			= 0x00020000,	// CPU 0 to 1
#endif
#ifdef __WIFI__
	eIP1_WIF_EVT		= 0x00040000,	// CPU 0 to 1
	eIP1_WIF_RX			= 0x00080000,	// CPU 0 to 1
#endif
	eIP1_SHELL_RX		= 0x00100000,	// CPU 0 to 1
	eIP1_RESET_FPS		= 0x00200000,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00400000,	// CPU 0 to 1
// ~eIP1_xxxxxxxxx		= 0x00800000,	// CPU 0 to 1
#ifdef __ISP__
	eIP1_ISP_SAVE_SHD	= 0x01000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_MENU	= 0x02000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_MENU2	= 0x04000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_USER	= 0x08000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_DATA	= 0x10000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_BLK0	= 0x20000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_BLK1	= 0x40000000,	// CPU 0 to 1
	eIP1_ISP_SAVE_BLK2	= 0x80000000	// CPU 0 to 1
#endif
};

extern BYTE CpuIdGet(void);
extern BOOL GetKey(UINT Idx);
extern void ReleaseKey(UINT Idx);

extern void WaitXus(UINT anUs);
extern void WaitXms(UINT anMs);

extern void IsrSys0(void *dummy);
extern void IsrSys1(void *dummy);

#ifndef _INLINE
extern void WdtCpuSetStop(void);
extern void WdtCpuCountReset(void);
extern void WdtCpuSetTime(UINT sec);
extern void WdtCpuReboot(void);
#endif

//******************************************************************************
// 8. DDR
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT REF_EN         : 1;
        UINT _res           :19;
        UINT REF_LMT        :12;    _rege_  _DDR_LEF;

_regs_  UINT MR_REQ         : 1;
        UINT _res           : 9;
        UINT MR_CTL         : 2;
        UINT MR_CMD         :20;    _rege_  _DDR_CTL0;

_regs_  UINT _res0          : 4;
        UINT T_MR           : 4;
        UINT _res1          :13;
        UINT RD_LTC         : 3;
        UINT RD_EDGE        : 1;
        UINT _res2          : 7;    _rege_  _DDR_CTL1;

_regs_  UINT _res           : 4;
        UINT T_RP           : 4;
        UINT T_WR           : 4;
        UINT T_WTR          : 4;
        UINT T_RTP          : 4;
        UINT T_RTW          : 4;
        UINT T_RCD          : 4;
        UINT T_RRD          : 4;    _rege_  _DDR_TIM0;

_regs_  UINT _res           :14;
        UINT T_RFC          : 6;
        UINT T_RC           : 6;
        UINT T_RAS          : 6;    _rege_  _DDR_TIM1;

_regs_  UINT DLY_DQSIN      :16;
        UINT DLY_DMOUT3     : 4;
        UINT DLY_DMOUT2     : 4;
        UINT DLY_DMOUT1     : 4;
        UINT DLY_DMOUT0     : 4;    _rege_  _DDR_DLYCTL0;

_regs_  UINT DLY_DQOUT31    : 4;
        UINT DLY_DQOUT30    : 4;
        UINT DLY_DQOUT29    : 4;
        UINT DLY_DQOUT28    : 4;
        UINT DLY_DQOUT27    : 4;
        UINT DLY_DQOUT26    : 4;
        UINT DLY_DQOUT25    : 4;
        UINT DLY_DQOUT24    : 4;    _rege_  _DDR_DLYCTL1;

_regs_  UINT DLY_DQOUT23    : 4;
        UINT DLY_DQOUT22    : 4;
        UINT DLY_DQOUT21    : 4;
        UINT DLY_DQOUT20    : 4;
        UINT DLY_DQOUT19    : 4;
        UINT DLY_DQOUT18    : 4;
        UINT DLY_DQOUT17    : 4;
        UINT DLY_DQOUT16    : 4;    _rege_  _DDR_DLYCTL2;

_regs_  UINT DLY_DQOUT15    : 4;
        UINT DLY_DQOUT14    : 4;
        UINT DLY_DQOUT13    : 4;
        UINT DLY_DQOUT12    : 4;
        UINT DLY_DQOUT11    : 4;
        UINT DLY_DQOUT10    : 4;
        UINT DLY_DQOUT09    : 4;
        UINT DLY_DQOUT08    : 4;    _rege_  _DDR_DLYCTL3;

_regs_  UINT DLY_DQOUT07    : 4;
        UINT DLY_DQOUT06    : 4;
        UINT DLY_DQOUT05    : 4;
        UINT DLY_DQOUT04    : 4;
        UINT DLY_DQOUT03    : 4;
        UINT DLY_DQOUT02    : 4;
        UINT DLY_DQOUT01    : 4;
        UINT DLY_DQOUT00    : 4;    _rege_  _DDR_DLYCTL4;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define DDR_REF_EN          _bm(_DDR_LEF        , DDR_REG_BASE , 0x00, REF_EN       )   // RW
#define DDR_REF_LMT         _bm(_DDR_LEF        , DDR_REG_BASE , 0x00, REF_LMT      )   // RW
#define DDR_LEF             _am(_DDR_LEF        , DDR_REG_BASE , 0x00,              )   // RW

#define DDR_MR_REQ          _bm(_DDR_CTL0       , DDR_REG_BASE , 0x04, MR_REQ       )   // RW
#define DDR_MR_CTL          _bm(_DDR_CTL0       , DDR_REG_BASE , 0x04, MR_CTL       )   // RW
#define DDR_MR_CMD          _bm(_DDR_CTL0       , DDR_REG_BASE , 0x04, MR_CMD       )   // RW
#define DDR_CTL0_T          _am(_DDR_CTL0       , DDR_REG_BASE , 0x04               )   // RW

#define DDR_T_MR            _bm(_DDR_CTL1       , DDR_REG_BASE , 0x08, T_MR         )   // RW
#define DDR_RD_LTC          _bm(_DDR_CTL1       , DDR_REG_BASE , 0x08, RD_LTC       )   // RW
#define DDR_RD_EDGE         _bm(_DDR_CTL1       , DDR_REG_BASE , 0x08, RD_EDGE      )   // RW
#define DDR_CTL1_T          _am(_DDR_CTL1       , DDR_REG_BASE , 0x08               )   // RW

#define DDR_T_RP            _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_RP         )   // RW
#define DDR_T_WR            _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_WR         )   // RW
#define DDR_T_WTR           _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_WTR        )   // RW
#define DDR_T_RTP           _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_RTP        )   // RW
#define DDR_T_RTW           _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_RTW        )   // RW
#define DDR_T_RCD           _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_RCD        )   // RW
#define DDR_T_RRD           _bm(_DDR_TIM0       , DDR_REG_BASE , 0x0c, T_RRD        )   // RW
#define DDR_TIM0_T          _am(_DDR_TIM0       , DDR_REG_BASE , 0x0c               )   // RW

#define DDR_T_RFC           _bm(_DDR_TIM1       , DDR_REG_BASE , 0x10, T_RFC        )   // RW
#define DDR_T_RC            _bm(_DDR_TIM1       , DDR_REG_BASE , 0x10, T_RC         )   // RW
#define DDR_T_RAS           _bm(_DDR_TIM1       , DDR_REG_BASE , 0x10, T_RAS        )   // RW
#define DDR_TIM1_T          _am(_DDR_TIM1       , DDR_REG_BASE , 0x10               )   // RW

#define DDR_DLY_DQSIN       _bm(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14, DLY_DQSIN    )   // RW
#define DDR_DLY_DMOUT3      _bm(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14, DLY_DMOUT3   )   // RW
#define DDR_DLY_DMOUT2      _bm(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14, DLY_DMOUT2   )   // RW
#define DDR_DLY_DMOUT1      _bm(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14, DLY_DMOUT1   )   // RW
#define DDR_DLY_DMOUT0      _bm(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14, DLY_DMOUT0   )   // RW
#define DDR_DLYCTL0_T       _am(_DDR_DLYCTL0    , DDR_REG_BASE , 0x14               )   // RW

#define DDR_DLY_DQOUT31     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT31  )   // RW
#define DDR_DLY_DQOUT30     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT30  )   // RW
#define DDR_DLY_DQOUT29     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT29  )   // RW
#define DDR_DLY_DQOUT28     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT28  )   // RW
#define DDR_DLY_DQOUT27     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT27  )   // RW
#define DDR_DLY_DQOUT26     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT26  )   // RW
#define DDR_DLY_DQOUT25     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT25  )   // RW
#define DDR_DLY_DQOUT24     _bm(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18, DLY_DQOUT24  )   // RW
#define DDR_DLYCTL1_T       _am(_DDR_DLYCTL1    , DDR_REG_BASE , 0x18               )   // RW

#define DDR_DLY_DQOUT23     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT23  )   // RW
#define DDR_DLY_DQOUT22     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT22  )   // RW
#define DDR_DLY_DQOUT21     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT21  )   // RW
#define DDR_DLY_DQOUT20     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT20  )   // RW
#define DDR_DLY_DQOUT19     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT19  )   // RW
#define DDR_DLY_DQOUT18     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT18  )   // RW
#define DDR_DLY_DQOUT17     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT17  )   // RW
#define DDR_DLY_DQOUT16     _bm(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c, DLY_DQOUT16  )   // RW
#define DDR_DLYCTL2_T       _am(_DDR_DLYCTL2    , DDR_REG_BASE , 0x1c               )   // RW

#define DDR_DLY_DQOUT15     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT15  )   // RW
#define DDR_DLY_DQOUT14     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT14  )   // RW
#define DDR_DLY_DQOUT13     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT13  )   // RW
#define DDR_DLY_DQOUT12     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT12  )   // RW
#define DDR_DLY_DQOUT11     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT11  )   // RW
#define DDR_DLY_DQOUT10     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT10  )   // RW
#define DDR_DLY_DQOUT09     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT09  )   // RW
#define DDR_DLY_DQOUT08     _bm(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20, DLY_DQOUT08  )   // RW
#define DDR_DLYCTL3_T       _am(_DDR_DLYCTL3    , DDR_REG_BASE , 0x20               )   // RW

#define DDR_DLY_DQOUT07     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT07  )   // RW
#define DDR_DLY_DQOUT06     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT06  )   // RW
#define DDR_DLY_DQOUT05     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT05  )   // RW
#define DDR_DLY_DQOUT04     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT04  )   // RW
#define DDR_DLY_DQOUT03     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT03  )   // RW
#define DDR_DLY_DQOUT02     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT02  )   // RW
#define DDR_DLY_DQOUT01     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT01  )   // RW
#define DDR_DLY_DQOUT00     _bm(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24, DLY_DQOUT00  )   // RW
#define DDR_DLYCTL4_T       _am(_DDR_DLYCTL4    , DDR_REG_BASE , 0x24               )   // RW

//------------------------------------------------------------------------------
// Extern
extern void DdrInit(void);

//******************************************************************************
// 9. DMA
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res0          :16;
        UINT VALUE          : 8;
        UINT _res1          : 1;
        UINT CHKSUM_CLR     : 1;
        UINT MODE           : 2;
        UINT IRQ            : 1;
        UINT IRQ_EN         : 1;
        UINT IRQ_CLR        : 1;
        UINT GO             : 1;    _rege_  _DMA_CONT;

_regs_                              _rege_  _DMA_INT;

_regs_  UINT _res           :16;
        UINT CHKSUM         :16;    _rege_  _DMA_CHKSUM;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define DMA_VALUE           _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, VALUE        )   // RW
#define DMA_CHKSUM_CLR      _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, CHKSUM_CLR   )   // RW
#define DMA_MODE            _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, MODE         )   // RW
#define DMA_IRQ             _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, IRQ          )   // RO
#define DMA_IRQ_EN          _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, IRQ_EN       )   // RW
#define DMA_IRQ_CLR         _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, IRQ_CLR      )   // RW
#define DMA_GO              _bm(_DMA_CONT       , DMA_REG_BASE , 0x00, GO           )   // RW

#define DMA_SRC             _am(_DMA_INT        , DMA_REG_BASE , 0x04               )   // RW

#define DMA_DST             _am(_DMA_INT        , DMA_REG_BASE , 0x08               )   // RW

#define DMA_LEN             _am(_DMA_INT        , DMA_REG_BASE , 0x0c               )   // RW

#define DMA_CHKSUM          _bm(_DMA_CHKSUM     , DMA_REG_BASE , 0x10, CHKSUM       )   // RO

//------------------------------------------------------------------------------
// Extern

//void DmaMemCpy_isr_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
//void DmaMemCpy_isr(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
//void DmaMemSet_isr(BYTE *apbDst, BYTE abVal, UINT anNum);
//void DmaDChkSum_isr(BYTE *apbDst, UINT anNum, WORD *apnChkSum);
//extern void* DmaMemCpy(void* dst, const void* src, size_t len);
//extern void DmaMemSet(BYTE * dst, BYTE Value, UINT len);
//extern WORD DmaChkSum(BYTE * src, UINT len);

//******************************************************************************
// 10. GPIO
//------------------------------------------------------------------------------
// Bit field
_regs_                              _rege_  _GPIO;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define GPIO_I              _am(_GPIO           , GPIO_REG_BASE, 0x00               )   // RO

#define GPIO_O              _am(_GPIO           , GPIO_REG_BASE, 0x04               )   // RW

#define GPIO_OEN            _am(_GPIO           , GPIO_REG_BASE, 0x08               )   // RW

#define GPIO_EDGE_DIR       _am(_GPIO           , GPIO_REG_BASE, 0x0c               )   // RW

#define GPIO_IRQ_EN         _am(_GPIO           , GPIO_REG_BASE, 0x10               )   // RW

#define GPIO_IRQ_CLR        _am(_GPIO           , GPIO_REG_BASE, 0x14               )   // WO

#define GPIO_IRQ            _am(_GPIO           , GPIO_REG_BASE, 0x18               )   // RO

//------------------------------------------------------------------------------
// GPIO Assign Definition

#define GPIO_CNT			24

#define GPIO_00				0
#define GPIO_01				1
#define GPIO_02				2
#define GPIO_03				3
#define GPIO_04				4
#define GPIO_05				5
#define GPIO_06				6
#define GPIO_07				7
#define GPIO_08				8
#define GPIO_09				9
#define GPIO_10				10
#define GPIO_11				11
#define GPIO_12				12
#define GPIO_13				13
#define GPIO_14				14
#define GPIO_15				15
#define GPIO_16				16
#define GPIO_17				17
#define GPIO_18				18
#define GPIO_19				19
#define GPIO_20				20
#define GPIO_21				21
#define GPIO_22				22
#define GPIO_23				23


#ifndef USING_CONFIG_GPIO_SCRIPT // Use Configuration Tool (*.mk file)
//------------------------------------------------------------------------------
// LED control
#define LED_ISP_VI_CHECK								// GPIO_LED1
//#define LED_FOCUS_CHECK								// GPIO_LED1
#define LED_H264_ENC_CHECK								// GPIO_LED2
//#define LED_AVI_ENC_CHECK								// GPIO_LED2
//#define LED_DISKIO_CHECK								// GPIO_LED2

//------------------------------------------------------------------------------
// Board Evaluation Setting for IP (or common)
#ifdef LED_FOCUS_CHECK
#define GPIO_FOCUS_LED		GPIO_01	// ISP
#endif
#ifdef LED_ISP_VI_CHECK
#define GPIO_ISP_VI_LED		GPIO_01	// ISP
#endif
#ifdef LED_H264_ENC_CHECK
#define GPIO_H264_ENC_LED	GPIO_02	// IP
#endif
#ifdef LED_AVI_ENC_CHECK
#define GPIO_RECORDING_LED	GPIO_02	// IP
#endif
#ifdef LED_DISKIO_CHECK
#define GPIO_DISKIO_LED		GPIO_02	// IP
#endif
#define GPIO_ETH_IRQ		GPIO_14	// IP
//#define GPIO_JPEG_STATE	GPIO_15	// IP
//#define GPIO_ETC_STATE	GPIO_17	// IP
//#define GPIO_LED4			GPIO_20	// IP
//#define GPIO_SDIO0_RST	GPIO_20	// IP
#define GPIO_SDIO1_RST		GPIO_21	// IP
#if (IMAGE_INPUT_MODE==3)
#define GPIO_ETH_RST		GPIO_13	// IP	// 2017330 - WHL : 4M Board
#else
#define GPIO_ETH_RST		GPIO_22	// IP
#endif
//#define GPIO_ETH_RST		GPIO_13	// IP	// 2017330 - WHL : 4M Board
#define GPIO_AUD_RST		GPIO_23	// IP
//#define GPIO_ETH_RST		GPIO_13	// IP

#endif // end of USING_CONFIG_GPIO_SCRIPT


//------------------------------------------------------------------------------
// Board Evaluation Setting for ISP
#ifdef __ISP__

  #ifndef USING_CONFIG_ISP_SCRIPT // Use Configuration Tool (*.mk file)

	#define model_Key			0		//*0 : KEY input by ADC
										// 1 : KEY input by GPIO
										// 2 : no physical key (UART Key only)

	#define model_Lens			1		// Lens type (+MENU)
										//*0 : normal, 1 : Motorized AF

	#define model_MAF_Lens		0		// Motorized AF Lens model
										//*0 : YT2812, 1 : YT3013, 2 : YT3017, 3 : YT30022, 4 : YT3021, 5~9 : Cstom Lens (Needed Tuning)

	#define model_Iris			2		// IRIS control by
										// 0: (no use), 1: KIWI driver(DC), *2: MPWM(DC), 3: Step motor(pIris) 4: AF drvIC (PID)

	#define model_Tdn			2		// 0 : TDN control by 1 port 2 state (H, L)
										// 1 : TDN control by 1 port 3 state (H, L, Hi-z)
										//*2 : TDN control by 2 ports

	#define model_Cds			0		//*0 : CDS input by ADC
										// 1 : CDS input by GPIO

	#define model_Led			1		// 0 : IR-LED control by LPWM
										//*1 : IR-LED control by GPIO

	#define model_MotionOut		0		//*0 : GPIO Motion Alarm Disable
										// 1 : GPIO Motion Alarm Enable

  #endif // end of USING_CONFIG_ISP_SCRIPT


	// Options
	#define model_TgtBd 		0		// Target board - GPIO ordering, ADC ordering, ADC Key, TDN, PWM(IRIS)
										// 0: Enx Socket Bd or Enx EVM 4242 Bd, 1: customer Bd

	#define model_TgtBdDly		0		// Target board Delay - LVDS timing
										// 0: Enx Socket Bd or Enx EVM 4242 Bd, 1~9: customer Bd

  #ifdef OPTION_FLAG_0

	#define DOOR_BELL_CAMERA	1		//*0 : Disable, 1 : Enable (for Eyenix Door Bell System) (+MENU)

  #else

	#define DOOR_BELL_CAMERA	0		//*0 : Disable, 1 : Enable (for Eyenix Door Bell System) (+MENU)

  #endif



#ifndef USING_CONFIG_GPIO_SCRIPT // Use Configuration Tool (*.mk file)

//------------------------------------------------------------------------------
#if (model_TgtBd==1)				// GPIO Definition for Customer

#define GPIO_SENS_RSTN		GPIO_00	// Reserved for Sensor Reset



// Add GPIO define for customizer board ...










#else								// Enx Socket Bd or Enx EVM 4242 Bd --------------------

#define GPIO_SENS_RSTN		GPIO_00	// Reserved for Sensor Reset

#define	GPIO_TDN_1			GPIO_03	// TDN out Port1
#if (model_Tdn==2)
#define	GPIO_TDN_2			GPIO_04	// TDN out Port2
#endif

#if (model_Lens==1)
#define	GPIO_MTR_FCS_AP		GPIO_05	// Motor Focus Control A+
#define	GPIO_MTR_FCS_AM		GPIO_06	// "				   A-
#define	GPIO_MTR_FCS_BP		GPIO_07	// "			 	   B+
#define	GPIO_MTR_FCS_BM		GPIO_08	// "			       B-
#define	GPIO_MTR_ZOOM_AP	GPIO_09	// Motor Zoom Control  A+
#define	GPIO_MTR_ZOOM_AM	GPIO_10	// "				   A-
#define	GPIO_MTR_ZOOM_BP	GPIO_12	// "			 	   B+
#define	GPIO_MTR_ZOOM_BM	GPIO_20	// "			       B-
#elif (DOOR_BELL_CAMERA==1)
#define GPIO_MAIN_PW_HLD	GPIO_07	// Keep the main power @ High State
#define GPIO_MAIN_PW_CLS	GPIO_08	// Main Power On/Off (High : Power Off)
#define GPIO_SENS_PEN		GPIO_09	// Sensor Power Enable for Door Bell Cam
#define GPIO_DOOR_BELL		GPIO_16	// Door Bell State
#define GPIO_PIR_AWAKE		GPIO_20	// PIR Sensor State
#endif

#if (model_Cds==1)
#define	GPIO_CDS_IN			GPIO_10	// CDS input
#endif
#if (model_Led==1)
#define	GPIO_LED_CTL		GPIO_11	// LED control out (GPIO or LPWM)
#endif
#if (IMAGE_INPUT_MODE==3)
#define	GPIO_MPWM			GPIO_22	// Micom PWM out for IRIS control
#else
#define	GPIO_MPWM			GPIO_13	// Micom PWM out for IRIS control
#endif
#if (model_MotionOut==1)
#define GPIO_MOTION			GPIO_15	// Motion alarm out
#endif

// optional -----------------------------
#if (model_Key==1)					// GPIO Key
#define	GPIO_KEY_C					//
#define	GPIO_KEY_U					//
#define	GPIO_KEY_D					//
#define	GPIO_KEY_L					//
#define	GPIO_KEY_R					//
#endif

#define ADC_KEYr		ADC0_RDATr		// AD-Key
#define ADC_CDSr		ADC1_RDATr		// CDS IN

#endif // end of "model_TgtBd" setting

#else // Adding Customizing GPIO assign

	#if (DOOR_BELL_CAMERA==1)
		#define GPIO_MAIN_PW_HLD	GPIO_07	// Keep the main power @ High State
		#define GPIO_MAIN_PW_CLS	GPIO_08	// Main Power On/Off (High : Power Off)
		#define GPIO_SENS_PEN		GPIO_09	// Sensor Power Enable for Door Bell Cam
		#define GPIO_DOOR_BELL		GPIO_16	// Door Bell State
		#define GPIO_PIR_AWAKE		GPIO_20	// PIR Sensor State
	#endif

#endif // end of USING_CONFIG_GPIO_SCRIPT

#endif // end of "__ISP__"

//------------------------------------------------------------------------------
// extern

#ifndef _INLINE
extern void GpioInit(void);
extern void GpioEi(BYTE abSel);
extern void GpioDi(BYTE abSel);
extern void GpioRiseEdge(BYTE abSel);
extern void GpioFallEdge(BYTE abSel);
extern void GpioOutDir(BYTE abSel);
extern void GpioInDir(BYTE abSel);
extern BYTE GpioGetDir(BYTE abSel);
extern void GpioSetHi(BYTE abSel);
extern void GpioSetLo(BYTE abSel);
extern void GpioFuncPin(BYTE abSel);
extern void GpioFuncPinOff(BYTE abSel);
extern BYTE GpioGetFuncPin(BYTE abSel);
extern BYTE GpioGetPin(BYTE abSel);
extern void GpioPullUpEnable(BYTE abSel);
extern void GpioPullUpDisable(BYTE abSel);
#endif
#ifdef DEF_CPU1
extern void IsrGpio(void *ctx);
#endif

//******************************************************************************
// 11. ISR
//------------------------------------------------------------------------------
extern int IntInit(void);
extern int IntAdd(UINT anVec, void (* handler)(void *), void *arg);
extern int IrqDisable(UINT anVec);
extern int IrqEnable(UINT anVec);
extern void XsrInt(void);
extern void XsrBusErr(void);
extern void XsrAlign(void);
extern void XsrIllInsn(void);
extern void XsrFloatPoint(void);
extern void XsrSyscall(void);
extern void XsrBreak(void);
extern void XsrTrap(void);
extern void XsrDpFault(void);
extern void XsrIpFault(void);
extern void XsrDtlbMiss(void);
extern void XsrItlbMiss(void);
extern void xsr_tick(void);

//extern void mtspr(UINT anSpr, UINT anVal);
//extern UINT mfspr(UINT anSpr);
//extern void I2sIsr(void);
//extern void AdcIsr(void);
//extern void H264Isr(void);
//extern void EthIsr(void);

//******************************************************************************
// 12. SERIAL FLASH
//------------------------------------------------------------------------------
// Bit field
_regs_                              _rege_  _SFLS_DATIN;

_regs_                              _rege_  _SFLS_DATOUT;

_regs_  UINT _res           :16;
        UINT WB_RDCMD       : 8;
        UINT WB_WRCMD       : 8;    _rege_  _SFLS_WBCMD;

_regs_  UINT REG_CMD        : 8;
        UINT WB_ADR_EXT     : 1;
        UINT WB_WR_EN       : 1;
        UINT WB_WAIT_EN     : 1;
        UINT WB_GAP_EN      : 1;
        UINT WB_GAP_LEN     : 2;
        UINT WB_RDCMD_IOM   : 2;
        UINT WB_RDADR_IOM   : 2;
        UINT WB_RDDAT_IOM   : 2;
        UINT WB_WREN_IOM    : 2;
        UINT WB_WRCMD_IOM   : 2;
        UINT WB_WRADR_IOM   : 2;
        UINT WB_WRDAT_IOM   : 2;
        UINT WB_WAITCMD_IOM : 2;
        UINT WB_WAITDAT_IOM : 2;    _rege_  _SFLS_WBCONT;

_regs_  UINT _res           : 7;
        UINT REG_ADR        :25;    _rege_  _SFLS_CMDADR;

_regs_  UINT CLKDIV         : 4;
        UINT _res           :10;
        UINT REG_REQ        : 1;
        UINT WB_REQ         : 1;
        UINT REG_ADR_EXT    : 1;
        UINT REG_ADR_EN     : 1;
        UINT REG_GAP_EN     : 1;
        UINT REG_DAT_EN     : 1;
        UINT REG_WAIT_EN    : 1;
        UINT REG_DAT_RW     : 1;
        UINT REG_CMD_IOM    : 2;
        UINT REG_ADR_IOM    : 2;
        UINT REG_DAT_IOM    : 2;
        UINT REG_GAP_LEN    : 2;
        UINT REG_DAT_LEN    : 2;    _rege_  _SFLS_CMD;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define SFLS_DATIN          _am(_SFLS_DATIN     , SFLS_REG_BASE, 0x00               )   // RW

#define SFLS_DATOUT         _am(_SFLS_DATOUT    , SFLS_REG_BASE, 0x04               )   // RW

#define SFLS_WB_RDCMD       _bm(_SFLS_WBCMD     , SFLS_REG_BASE, 0x08, WB_RDCMD     )   // RW
#define SFLS_WB_WRCMD       _bm(_SFLS_WBCMD     , SFLS_REG_BASE, 0x08, WB_WRCMD     )   // RW
#define SFLS_WB_RWCMD_t     _am(_SFLS_WBCMD     , SFLS_REG_BASE, 0x08               )   // RW

#define SFLS_WBCONT         _am(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C               )   // RW
#define SFLS_REG_CMD        _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, REG_CMD      )   // RW
#define SFLS_WB_ADR_EXT     _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_ADR_EXT   )   // RW
#define SFLS_WB_WR_EN       _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WR_EN     )   // RW
#define SFLS_WB_WAIT_EN     _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WAIT_EN   )   // RW
#define SFLS_WB_GAP_EN      _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_GAP_EN    )   // RW
#define SFLS_WB_GAP_LEN     _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_GAP_LEN   )   // RW
#define SFLS_WB_RDCMD_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_RDCMD_IOM )   // RW
#define SFLS_WB_RDADR_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_RDADR_IOM )   // RW
#define SFLS_WB_RDDAT_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_RDDAT_IOM )   // RW
#define SFLS_WB_WREN_IOM    _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WREN_IOM  )   // RW
#define SFLS_WB_WRCMD_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WRCMD_IOM )   // RW
#define SFLS_WB_WRADR_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WRADR_IOM )   // RW
#define SFLS_WB_WRDAT_IOM   _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WRDAT_IOM )   // RW
#define SFLS_WB_WAITCMD_IOM _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WAITCMD_IOM)  // RW
#define SFLS_WB_WAITDAT_IOM _bm(_SFLS_WBCONT    , SFLS_REG_BASE, 0x0C, WB_WAITDAT_IOM)  // RW

#define SFLS_REG_ADR        _bm(_SFLS_CMDADR    , SFLS_REG_BASE, 0x10, REG_ADR      )   // RW

#define SFLS_CMD            _am(_SFLS_CMD       , SFLS_REG_BASE, 0x14               )   // RW
#define SFLS_CLK_DIV        _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, CLKDIV       )   // RW
#define SFLS_REG_REQ        _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_REQ      )   // RW
#define SFLS_REG_WB         _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, WB_REQ       )   // RW
#define SFLS_REG_ADR_EXT    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_ADR_EXT  )   // RW
#define SFLS_REG_ADR_EN     _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_ADR_EN   )   // RW
#define SFLS_REG_GAP_EN     _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_GAP_EN   )   // RW
#define SFLS_REG_DAT_EN     _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_DAT_EN   )   // RW
#define SFLS_REG_WAIT_EN    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_WAIT_EN  )   // RW
#define SFLS_REG_CMD_IOM    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_CMD_IOM  )   // RW
#define SFLS_REG_ADR_IOM    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_ADR_IOM  )   // RW
#define SFLS_REG_DAT_IOM    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_DAT_IOM  )   // RW
#define SFLS_REG_GAP_LEN    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_GAP_LEN  )   // RW
#define SFLS_REG_DAT_LEN    _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_DAT_LEN  )   // RW
#define SFLS_REG_DAT_RW     _bm(_SFLS_CMD       , SFLS_REG_BASE, 0x14, REG_DAT_RW   )   // RW

//------------------------------------------------------------------------------
// Extern
extern BYTE FlsRead(BYTE *apbUser, UINT apbFlsAddr, UINT anNum);
extern BYTE FlsWrite(UINT apbFlsAddr, const BYTE *apbUser, UINT anNum);
extern void FlsErase(UINT apbFlsAddr);

extern void SflsInit(void);
extern void sflsWriteEn(void);
extern void SflsEraseSect(UINT Adr);
extern void SflsEraseBlock(UINT Adr);
extern void SflsEraseAll(void);
extern void SflsWriteReg(UCHAR dat);
extern BYTE SflsReadReg(void);

//******************************************************************************
// 13. TIMER
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res0          : 2;
        UINT CH0_EN         : 1;
        UINT CH1_EN         : 1;
        UINT CH2_EN         : 1;
        UINT CH3_EN         : 1;
        UINT CH4_EN         : 1;
        UINT CH5_EN         : 1;
        UINT _res1          : 2;
        UINT CH0_IRQ_EN     : 1;
        UINT CH1_IRQ_EN     : 1;
        UINT CH2_IRQ_EN     : 1;
        UINT CH3_IRQ_EN     : 1;
        UINT CH4_IRQ_EN     : 1;
        UINT CH5_IRQ_EN     : 1;
        UINT _res2          : 2;
        UINT CH0_IRQ_CLR    : 1;
        UINT CH1_IRQ_CLR    : 1;
        UINT CH2_IRQ_CLR    : 1;
        UINT CH3_IRQ_CLR    : 1;
        UINT CH4_IRQ_CLR    : 1;
        UINT CH5_IRQ_CLR    : 1;
        UINT _res3          : 2;
        UINT CH0_IRQ        : 1;
        UINT CH1_IRQ        : 1;
        UINT CH2_IRQ        : 1;
        UINT CH3_IRQ        : 1;
        UINT CH4_IRQ        : 1;
        UINT CH5_IRQ        : 1;    _rege_  _TIM_CONT0;

_regs_  UINT _res0          : 2;
        UINT CH0_PWM_EN     : 1;
        UINT CH1_PWM_EN     : 1;
        UINT CH2_PWM_EN     : 1;
        UINT CH3_PWM_EN     : 1;
        UINT CH4_PWM_EN     : 1;
        UINT CH5_PWM_EN     : 1;
        UINT _res1          : 2;
        UINT CH0_CKEN       : 1;
        UINT CH1_CKEN       : 1;
        UINT CH2_CKEN       : 1;
        UINT CH3_CKEN       : 1;
        UINT CH4_CKEN       : 1;
        UINT CH5_CKEN       : 1;
        UINT CH0_DIV        : 8;
        UINT CH1_DIV        : 8;    _rege_  _TIM_CONT1;

_regs_  UINT CH2_DIV        : 8;
        UINT CH3_DIV        : 8;
        UINT CH4_DIV        : 8;
        UINT CH5_DIV        : 8;    _rege_  _TIM_CONT2;

_regs_  UINT CH0_LMT        :16;
        UINT CH1_LMT        :16;    _rege_  _TIM_CONT3;

_regs_  UINT CH2_LMT        :16;
        UINT CH3_LMT        :16;    _rege_  _TIM_CONT4;

_regs_  UINT CH4_LMT        :16;
        UINT CH5_LMT        :16;    _rege_  _TIM_CONT5;

_regs_  UINT CH0_TRIG       :16;
        UINT CH1_TRIG       :16;    _rege_  _TIM_CONT6;

_regs_  UINT CH2_TRIG       :16;
        UINT CH3_TRIG       :16;    _rege_  _TIM_CONT7;

_regs_  UINT CH4_TRIG       :16;
        UINT CH5_TRIG       :16;    _rege_  _TIM_CONT8;

_regs_  UINT CH0_CNT        :16;
        UINT CH1_CNT        :16;    _rege_  _TIM_CONT9;

_regs_  UINT CH2_CNT        :16;
        UINT CH3_CNT        :16;    _rege_  _TIM_CONT10;

_regs_  UINT CH4_CNT        :16;
        UINT CH5_CNT        :16;    _rege_  _TIM_CONT11;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define TIM_CH0_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH0_EN      )   // RW
#define TIM_CH1_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH1_EN      )   // RW
#define TIM_CH2_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH2_EN      )   // RW
#define TIM_CH3_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH3_EN      )   // RW
#define TIM_CH4_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH4_EN      )   // RW
#define TIM_CH5_EN          _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH5_EN      )   // RW
#define TIM_CH0_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH0_IRQ_EN  )   // RW
#define TIM_CH1_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH1_IRQ_EN  )   // RW
#define TIM_CH2_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH2_IRQ_EN  )   // RW
#define TIM_CH3_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH3_IRQ_EN  )   // RW
#define TIM_CH4_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH4_IRQ_EN  )   // RW
#define TIM_CH5_IRQ_EN      _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH5_IRQ_EN  )   // RW
#define TIM_CH0_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH0_IRQ_CLR )   // WO
#define TIM_CH1_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH1_IRQ_CLR )   // WO
#define TIM_CH2_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH2_IRQ_CLR )   // WO
#define TIM_CH3_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH3_IRQ_CLR )   // WO
#define TIM_CH4_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH4_IRQ_CLR )   // WO
#define TIM_CH5_IRQ_CLR     _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH5_IRQ_CLR )   // WO
#define TIM_CH0_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH0_IRQ     )   // RO
#define TIM_CH1_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH1_IRQ     )   // RO
#define TIM_CH2_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH2_IRQ     )   // RO
#define TIM_CH3_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH3_IRQ     )   // RO
#define TIM_CH4_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH4_IRQ     )   // RO
#define TIM_CH5_IRQ         _bm(_TIM_CONT0      , TIMER_REG_BASE, 0x00, CH5_IRQ     )   // RO
#define TIM_CH0_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH0_PWM_EN  )   // RW
#define TIM_CH1_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH1_PWM_EN  )   // RW
#define TIM_CH2_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH2_PWM_EN  )   // RW
#define TIM_CH3_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH3_PWM_EN  )   // RW
#define TIM_CH4_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH4_PWM_EN  )   // RW
#define TIM_CH5_PWM_EN      _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH5_PWM_EN  )   // RW
#define TIM_CH0_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH0_CKEN    )   // RW
#define TIM_CH1_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH1_CKEN    )   // RW
#define TIM_CH2_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH2_CKEN    )   // RW
#define TIM_CH3_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH3_CKEN    )   // RW
#define TIM_CH4_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH4_CKEN    )   // RW
#define TIM_CH5_CKEN        _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH5_CKEN    )   // RW
#define TIM_CH0_DIV         _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH0_DIV     )   // RW
#define TIM_CH1_DIV         _bm(_TIM_CONT1      , TIMER_REG_BASE, 0x04, CH1_DIV     )   // RW
#define TIM_CH2_DIV         _bm(_TIM_CONT2      , TIMER_REG_BASE, 0x08, CH2_DIV     )   // RW
#define TIM_CH3_DIV         _bm(_TIM_CONT2      , TIMER_REG_BASE, 0x08, CH3_DIV     )   // RW
#define TIM_CH4_DIV         _bm(_TIM_CONT2      , TIMER_REG_BASE, 0x08, CH4_DIV     )   // RW
#define TIM_CH5_DIV         _bm(_TIM_CONT2      , TIMER_REG_BASE, 0x08, CH5_DIV     )   // RW
#define TIM_CH0_LMT         _bm(_TIM_CONT3      , TIMER_REG_BASE, 0x0c, CH0_LMT     )   // RW
#define TIM_CH1_LMT         _bm(_TIM_CONT3      , TIMER_REG_BASE, 0x0c, CH1_LMT     )   // RW
#define TIM_CH2_LMT         _bm(_TIM_CONT4      , TIMER_REG_BASE, 0x10, CH2_LMT     )   // RW
#define TIM_CH3_LMT         _bm(_TIM_CONT4      , TIMER_REG_BASE, 0x10, CH3_LMT     )   // RW
#define TIM_CH4_LMT         _bm(_TIM_CONT5      , TIMER_REG_BASE, 0x14, CH4_LMT     )   // RW
#define TIM_CH5_LMT         _bm(_TIM_CONT5      , TIMER_REG_BASE, 0x14, CH5_LMT     )   // RW
#define TIM_CH0_TRIG        _bm(_TIM_CONT6      , TIMER_REG_BASE, 0x18, CH0_TRIG    )   // RW
#define TIM_CH1_TRIG        _bm(_TIM_CONT6      , TIMER_REG_BASE, 0x18, CH1_TRIG    )   // RW
#define TIM_CH2_TRIG        _bm(_TIM_CONT7      , TIMER_REG_BASE, 0x1c, CH2_TRIG    )   // RW
#define TIM_CH3_TRIG        _bm(_TIM_CONT7      , TIMER_REG_BASE, 0x1c, CH3_TRIG    )   // RW
#define TIM_CH4_TRIG        _bm(_TIM_CONT8      , TIMER_REG_BASE, 0x20, CH4_TRIG    )   // RW
#define TIM_CH5_TRIG        _bm(_TIM_CONT8      , TIMER_REG_BASE, 0x20, CH5_TRIG    )   // RW
#define TIM_CH0_CNT         _bm(_TIM_CONT9      , TIMER_REG_BASE, 0x24, CH0_CNT     )   // RW
#define TIM_CH1_CNT         _bm(_TIM_CONT9      , TIMER_REG_BASE, 0x24, CH1_CNT     )   // RW
#define TIM_CH2_CNT         _bm(_TIM_CONT10     , TIMER_REG_BASE, 0x28, CH2_CNT     )   // RW
#define TIM_CH3_CNT         _bm(_TIM_CONT10     , TIMER_REG_BASE, 0x28, CH3_CNT     )   // RW
#define TIM_CH4_CNT         _bm(_TIM_CONT11     , TIMER_REG_BASE, 0x2c, CH4_CNT     )   // RW
#define TIM_CH5_CNT         _bm(_TIM_CONT11     , TIMER_REG_BASE, 0x2c, CH5_CNT     )   // RW

//------------------------------------------------------------------------------
// Extern
extern void TimerInit(void);

//******************************************************************************
// 14. ETHERNET
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res0          :14;
        UINT MDIO_OPCODE    : 2;
        UINT _res1          : 3;
        UINT MDIO_PHYADR    : 5;
        UINT _res2          : 3;
        UINT MDIO_REGADR    : 5;    _rege_  _ETH_REG0;

_regs_  UINT _res0          :16;
        UINT MDIO_CLKLMT    : 8;
        UINT _res1          : 6;
        UINT MDIO_CLKEN     : 1;
        UINT MDIO_STR       : 1;    _rege_  _ETH_REG1;

_regs_  UINT MDIO_TXDAT     :16;
        UINT MDIO_RXDAT     :16;    _rege_  _ETH_REG2;

_regs_  UINT _res           :16;
        UINT RX_MACADR0     :16;    _rege_  _ETH_REG3;

_regs_  UINT RX_MACADR1     :32;    _rege_  _ETH_REG4;

_regs_  UINT RX_MUL0ADR     :16;
        UINT RX_MUL1ADR     :16;    _rege_  _ETH_REG5;

_regs_  UINT RX_MUL2ADR     :16;
        UINT RX_MUL3ADR     :16;    _rege_  _ETH_REG6;

_regs_  UINT RX_MUL4ADR     :16;
        UINT RX_MUL5ADR     :16;    _rege_  _ETH_REG7;

_regs_  UINT RX_MUL6ADR     :16;
        UINT RX_MUL7ADR     :16;    _rege_  _ETH_REG8;

_regs_  UINT RX_MUL8ADR     :16;
        UINT RX_MUL9ADR     :16;    _rege_  _ETH_REG9;

_regs_  UINT RX_MUL10ADR    :16;
        UINT RX_MUL11ADR    :16;    _rege_  _ETH_REG10;

_regs_  UINT RX_MUL0FILT    : 1;
        UINT RX_MUL1FILT    : 1;
        UINT RX_MUL2FILT    : 1;
        UINT RX_MUL3FILT    : 1;
        UINT RX_MUL4FILT    : 1;
        UINT RX_MUL5FILT    : 1;
        UINT RX_MUL6FILT    : 1;
        UINT RX_MUL7FILT    : 1;
        UINT RX_MUL8FILT    : 1;
        UINT RX_MUL9FILT    : 1;
        UINT RX_MUL10FILT   : 1;
        UINT RX_MUL11FILT   : 1;
        UINT RX_MACFILT     : 1;
        UINT RX_CRCFILT     : 1;
        UINT _res           :18;    _rege_  _ETH_REG11;

_regs_  UINT RX_EN          : 1;
        UINT _res0          : 9;
        UINT RX_RCKEDGE     : 1;
        UINT RX_DATTYPE     : 1;
        UINT RX_RCKDLY      : 4;
        UINT RX_ERTYPE      : 2;
        UINT _res1          : 2;
        UINT RX_EMPTY       : 1;
        UINT RX_LEN         :11;    _rege_  _ETH_REG12;

_regs_  UINT _res0          : 1;
        UINT TX_DATTYPE     : 1;
        UINT TX_CLKOE       : 1;
        UINT TX_CLKEDGE     : 1;
        UINT TX_CLKSEL      : 1;
        UINT TX_CRSCHK      : 1;
        UINT TX_COLCHK      : 1;
        UINT TX_RTYEN       : 1;
        UINT _res1          :12;
        UINT TX_RTYLMT      : 4;
        UINT _res2          : 6;
        UINT TX_EMPTY       : 1;
        UINT TX_FULL        : 1;    _rege_  _ETH_REG13;

_regs_  UINT _res           : 8;
        UINT TX_TXENDLY     : 4;
        UINT TX_TXD0DLY     : 4;
        UINT TX_TXD1DLY     : 4;
        UINT TX_TXD2DLY     : 4;
        UINT TX_TXD3DLY     : 4;
        UINT TX_TCKDLY      : 4;    _rege_  _ETH_REG14;

_regs_  UINT TX_IFGGAP      :16;
        UINT _res           :10;
        UINT RX_IRQ         : 1;
        UINT TX_IRQ         : 1;
        UINT RX_IRQEN       : 1;
        UINT TX_IRQEN       : 1;
        UINT RX_IRQCLR      : 1;
        UINT TX_IRQCLR      : 1;    _rege_  _ETH_REG15;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define ETH_MDIO_OPCODE     _bm(_ETH_REG0       , ETH_REG_BASE , 0x00, MDIO_OPCODE  )   // RW
#define ETH_MDIO_PHYADR     _bm(_ETH_REG0       , ETH_REG_BASE , 0x00, MDIO_PHYADR  )   // RW
#define ETH_MDIO_REGADR     _bm(_ETH_REG0       , ETH_REG_BASE , 0x00, MDIO_REGADR  )   // RW

#define ETH_MDIO_CLKLMT     _bm(_ETH_REG1       , ETH_REG_BASE , 0x04, MDIO_CLKLMT  )   // RW
#define ETH_MDIO_CLKEN      _bm(_ETH_REG1       , ETH_REG_BASE , 0x04, MDIO_CLKEN   )   // RW
#define ETH_MDIO_STR        _bm(_ETH_REG1       , ETH_REG_BASE , 0x04, MDIO_STR     )   // RW

#define ETH_MDIO_TXDAT      _bm(_ETH_REG2       , ETH_REG_BASE , 0x08, MDIO_TXDAT   )   // RW
#define ETH_MDIO_RXDAT      _bm(_ETH_REG2       , ETH_REG_BASE , 0x08, MDIO_RXDAT   )   // RW

#define ETH_RX_MACADR0      _bm(_ETH_REG3       , ETH_REG_BASE , 0x0c, RX_MACADR0   )   // RW

#define ETH_RX_MACADR1      _bm(_ETH_REG4       , ETH_REG_BASE , 0x10, RX_MACADR1   )   // RW

#define ETH_RX_MUL0ADR      _bm(_ETH_REG5       , ETH_REG_BASE , 0x14, RX_MUL0ADR   )   // RW
#define ETH_RX_MUL1ADR      _bm(_ETH_REG5       , ETH_REG_BASE , 0x14, RX_MUL1ADR   )   // RW

#define ETH_RX_MUL2ADR      _bm(_ETH_REG6       , ETH_REG_BASE , 0x18, RX_MUL2ADR   )   // RW
#define ETH_RX_MUL3ADR      _bm(_ETH_REG6       , ETH_REG_BASE , 0x18, RX_MUL3ADR   )   // RW

#define ETH_RX_MUL4ADR      _bm(_ETH_REG7       , ETH_REG_BASE , 0x1c, RX_MUL4ADR   )   // RW
#define ETH_RX_MUL5ADR      _bm(_ETH_REG7       , ETH_REG_BASE , 0x1c, RX_MUL5ADR   )   // RW

#define ETH_RX_MUL6ADR      _bm(_ETH_REG8       , ETH_REG_BASE , 0x20, RX_MUL6ADR   )   // RW
#define ETH_RX_MUL7ADR      _bm(_ETH_REG8       , ETH_REG_BASE , 0x20, RX_MUL7ADR   )   // RW

#define ETH_RX_MUL8ADR      _bm(_ETH_REG9       , ETH_REG_BASE , 0x24, RX_MUL8ADR   )   // RW
#define ETH_RX_MUL9ADR      _bm(_ETH_REG9       , ETH_REG_BASE , 0x24, RX_MUL9ADR   )   // RW

#define ETH_RX_MUL10ADR     _bm(_ETH_REG10      , ETH_REG_BASE , 0x28, RX_MUL10ADR  )   // RW
#define ETH_RX_MUL11ADR     _bm(_ETH_REG10      , ETH_REG_BASE , 0x28, RX_MUL11ADR  )   // RW

#define ETH_RX_MUL0FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL0FILT  )   // RW
#define ETH_RX_MUL1FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL1FILT  )   // RW
#define ETH_RX_MUL2FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL2FILT  )   // RW
#define ETH_RX_MUL3FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL3FILT  )   // RW
#define ETH_RX_MUL4FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL4FILT  )   // RW
#define ETH_RX_MUL5FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL5FILT  )   // RW
#define ETH_RX_MUL6FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL6FILT  )   // RW
#define ETH_RX_MUL7FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL7FILT  )   // RW
#define ETH_RX_MUL8FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL8FILT  )   // RW
#define ETH_RX_MUL9FILT     _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL9FILT  )   // RW
#define ETH_RX_MUL10FILT    _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL10FILT )   // RW
#define ETH_RX_MUL11FILT    _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MUL11FILT )   // RW
#define ETH_RX_MACFILT      _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_MACFILT   )   // RW
#define ETH_RX_CRCFILT      _bm(_ETH_REG11      , ETH_REG_BASE , 0x2c, RX_CRCFILT   )   // RW

#define ETH_RX_EN           _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_EN        )   // RW
#define ETH_RX_RCKEDGE      _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_RCKEDGE   )   // RW
#define ETH_RX_DATTYPE      _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_DATTYPE   )   // RW
#define ETH_RX_RCKDLY       _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_RCKDLY    )   // RW
#define ETH_RX_ERTYPE       _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_ERTYPE    )   // RW
#define ETH_RX_EMPTY        _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_EMPTY     )   // RO
#define ETH_RX_LEN          _bm(_ETH_REG12      , ETH_REG_BASE , 0x30, RX_LEN       )   // RO

#define ETH_TX_DATTYPE      _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_DATTYPE   )   // RW
#define ETH_TX_CLKOE        _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_CLKOE     )   // RW
#define ETH_TX_CLKEDGE      _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_CLKEDGE   )   // RW
#define ETH_TX_CLKSEL       _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_CLKSEL    )   // RW
#define ETH_TX_CRSCHK       _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_CRSCHK    )   // RW
#define ETH_TX_COLCHK       _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_COLCHK    )   // RW
#define ETH_TX_RTYEN        _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_RTYEN     )   // RW
#define ETH_TX_RTYLMT       _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_RTYLMT    )   // RW
#define ETH_TX_EMPTY        _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_EMPTY     )   // RO
#define ETH_TX_FULL         _bm(_ETH_REG13      , ETH_REG_BASE , 0x34, TX_FULL      )   // RO

#define ETH_TX_TXENDLY      _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TXENDLY   )   // RW
#define ETH_TX_TXD0DLY      _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TXD0DLY   )   // RW
#define ETH_TX_TXD1DLY      _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TXD1DLY   )   // RW
#define ETH_TX_TXD2DLY      _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TXD2DLY   )   // RW
#define ETH_TX_TXD3DLY      _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TXD3DLY   )   // RW
#define ETH_TX_TCKDLY       _bm(_ETH_REG14      , ETH_REG_BASE , 0x38, TX_TCKDLY    )   // RW

#define ETH_TX_IFGGAP       _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, TX_IFGGAP    )   // RW
#define ETH_RX_IRQ          _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, RX_IRQ       )   // RO
#define ETH_TX_IRQ          _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, TX_IRQ       )   // RO
#define ETH_RX_IRQEN        _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, RX_IRQEN     )   // RW
#define ETH_TX_IRQEN        _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, TX_IRQEN     )   // RW
#define ETH_RX_IRQCLR       _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, RX_IRQCLR    )   // WO
#define ETH_TX_IRQCLR       _bm(_ETH_REG15      , ETH_REG_BASE , 0x3c, TX_IRQCLR    )   // WO

//------------------------------------------------------------------------------
// Extern
extern void EthInit(void);
extern void MdioRead(BYTE phyadr, BYTE regadr, WORD *data);
extern void MdioWrite(BYTE phyadr, BYTE regadr, WORD data);
extern void MdioInit(void);

//******************************************************************************
// 15. H.264
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res0          : 4;
        UINT IHSIZE         :12;
        UINT _res1          : 4;
        UINT IVSIZE         :12;    _rege_  _H264_IMGSIZE;          //0x0000

_regs_  UINT _res0          : 4;
        UINT YSTRIDEX       :12;
        UINT _res1          : 4;
        UINT CSTRIDEX       :12;    _rege_  _H264_STRIDE;           //0x0008

_regs_  UINT _res           :18;
        UINT TMBNUM         :14;    _rege_  _H264_TMBNUM;           //0x0010

_regs_  UINT _res           :30;
        UINT IDR_FLAG       : 1;
        UINT PFRAME         : 1;    _rege_  _H264_PICTYPE;          //0x0020

_regs_  UINT _res           :26;
        UINT QSCALE         : 6;    _rege_  _H264_QINIT;            //0x0028

_regs_  UINT _res0          :12;
        UINT ALN32          : 1;
        UINT QUANTMODE      : 1;
        UINT RINGTYPE       : 1;
        UINT _res1          : 3;
        UINT PPS_QP         : 6;
        UINT _res2          : 6;
        UINT SRCHMODE       : 2;    _rege_  _H264_ENCOPT;           //0x0030

_regs_  UINT _res           :20;
        UINT FRMBITS        : 4;
        UINT FRMNUM         : 8;    _rege_  _H264_HDRINFO1;         //0x0040

_regs_  UINT _res0          : 8;
        UINT PROFILEIDC     : 8;
        UINT _res1          : 4;
        UINT CABACIDC       : 2;
        UINT CABACEN        : 1;
        UINT CROPBOT        : 1;
        UINT LEVIDC         : 8;    _rege_  _H264_HDRINFO2;         //0x0048

_regs_  UINT SPS_SPSID      : 4;
        UINT PPS_SPSID      : 4;
        UINT PPS_PPSID      : 4;
        UINT SLH_PPSID      : 4;
        UINT _res           :14;
        UINT SPSINSRT       : 1;
        UINT PPSINSRT       : 1;    _rege_  _H264_HDRINSERT;        //0x0050

_regs_  UINT _res           :28;
        UINT PREPTR         : 4;    _rege_  _H264_SMEMWPTR;         //0x0060

_regs_  UINT _res           :28;
        UINT PSTPTR         : 4;    _rege_  _H264_SMEMRPTR;         //0x0068

_regs_  UINT _res           :31;
        UINT INTRVC         : 1;    _rege_  _H264_INTRVC;           //0x0080

_regs_                              _rege_  _H264_CURYBUF;          //0x0088

_regs_                              _rege_  _H264_CURCBUF;          //0x0090

_regs_                              _rege_  _H264_CUREBUF;          //0x0098

_regs_                              _rege_  _H264_REFYBUF;          //0x00A0

_regs_                              _rege_  _H264_REFCBUF;          //0x00A8

_regs_                              _rege_  _H264_RECYBUF;          //0x00B0

_regs_                              _rege_  _H264_RECCBUF;          //0x00B8

_regs_                              _rege_  _H264_STRMBUF;          //0x00C0

_regs_                              _rege_  _H264_SBUFEND;          //0x00C8

_regs_                              _rege_  _H264_STPSBUFW;         //0x00D0

_regs_                              _rege_  _H264_STPIBUFW;         //0x00D8

_regs_                              _rege_  _H264_STPIBUFR;         //0x00E0

_regs_                              _rege_  _H264_SBUFSIZE;         //0x00E8

_regs_  UINT _res           :24;
        UINT VLCBURST       : 4;
        UINT ENCBURST       : 4;    _rege_  _H264_AXIINFO1;         //0x00F0

_regs_  UINT _res0          :24;
        UINT USEIADDR       : 1;
        UINT KEEP4KB        : 1;
        UINT CLKRATIO       : 2;
        UINT KEEPORDER      : 1;
        UINT RNDROBIN       : 1;
        UINT EXTROBIN       : 1;
        UINT BIGENDIAN      : 1;    _rege_  _H264_AXIINFO2;         //0x00F8

_regs_  UINT _res0          :21;
        UINT STRMRESET      : 1;
        UINT _res1          : 4;
        UINT RINGINIT       : 1;
        UINT VLCSTART       : 1;
        UINT _res2          : 1;
        UINT FLUSHSTART     : 1;
        UINT VLCREADY       : 1;
        UINT ENCSTART       : 1;    _rege_  _H264_ENCSTART;         //0x0100

_regs_  UINT _res0          :25;
        UINT SBUFFULL       : 1;
        UINT _res1          : 1;
        UINT VLCEND         : 1;
        UINT _res2          : 1;
        UINT FLUSHEND       : 1;
        UINT EVEND          : 1;
        UINT ENCEND         : 1;    _rege_  _H264_STATUS;           //0x0108

_regs_  UINT _res           : 8;
        UINT GENBITS        :24;    _rege_  _H264_GENBITS;          //0x0110

_regs_                              _rege_  _H264_SBUFWPOS;         //0x0118

_regs_  UINT _res0          :24;
        UINT SERRMASK       : 1;
        UINT EERRMASK       : 1;
        UINT INTRMASK4      : 1;
        UINT INTRMASK3      : 1;
        UINT _res1          : 1;
        UINT INTRMASK2      : 1;
        UINT INTRMASK1      : 1;
        UINT INTRMASK0      : 1;    _rege_  _H264_INTRMASK;         //0x0120

_regs_  UINT _res           : 8;
        UINT CURWAIT        : 8;
        UINT REFWAIT        : 8;
        UINT VLCWAIT        : 8;    _rege_  _H264_OPWAIT;           //0x0128

_regs_  UINT _res           :10;
        UINT SFTBITS        : 6;
        UINT SFTDATA        :16;    _rege_  _H264_STRMPACK;         //0x0180

_regs_  UINT _res           :31;
        UINT SFTWAIT        : 1;    _rege_  _H264_STRMWAIT;         //0x0188

_regs_  UINT _res           :28;
        UINT ECHNNEL        : 4;    _rege_  _H264_ECHNNEL;          //0x0190

_regs_  UINT _res           :28;
        UINT MCHNNEL        : 4;    _rege_  _H264_MCHNNEL;          //0x0198

_regs_  UINT _res           : 4;
        UINT PREBITS        :28;    _rege_  _H264_PREBITS;          //0x01A8

_regs_  UINT _res           :25;
        UINT ROI_EN_0       : 1;
        UINT ROI_Q_0        : 6;    _rege_  _H264_ROI_0;            //0x0200

_regs_  UINT ROI_XS_0       : 8;
        UINT ROI_XE_0       : 8;
        UINT ROI_YS_0       : 8;
        UINT ROI_YE_0       : 8;    _rege_  _H264_ROI_0_POS;        //0x0208

_regs_  UINT _res           :25;
        UINT ROI_EN_1       : 1;
        UINT ROI_Q_1        : 6;    _rege_  _H264_ROI_1;            //0x0210

_regs_  UINT ROI_XS_1       : 8;
        UINT ROI_XE_1       : 8;
        UINT ROI_YS_1       : 8;
        UINT ROI_YE_1       : 8;    _rege_  _H264_ROI_1_POS;        //0x021C

_regs_  UINT _res           :25;
        UINT ROI_EN_2       : 1;
        UINT ROI_Q_2        : 6;    _rege_  _H264_ROI_2;            //0x0220

_regs_  UINT ROI_XS_2       : 8;
        UINT ROI_XE_2       : 8;
        UINT ROI_YS_2       : 8;
        UINT ROI_YE_2       : 8;    _rege_  _H264_ROI_2_POS;        //0x0228

_regs_  UINT _res           :25;
        UINT ROI_EN_3       : 1;
        UINT ROI_Q_3        : 6;    _rege_  _H264_ROI_3;            //0x0230

_regs_  UINT ROI_XS_3       : 8;
        UINT ROI_XE_3       : 8;
        UINT ROI_YS_3       : 8;
        UINT ROI_YE_3       : 8;    _rege_  _H264_ROI_3_POS;        //0x0238

_regs_  UINT DVADR          :16;
        UINT _res0          : 6;
        UINT DVIDX          : 2;
        UINT _res1          : 1;
        UINT DVNUM          : 3;
        UINT _res2          : 3;
        UINT DVSLCEN        : 1;    _rege_  _H264_DPARAMETER0;      //0x0240

_regs_	UINT _res0          : 3;
        UINT ORGH           :13;
        UINT _res1          : 4;
        UINT ORGV           :12;    _rege_  _H264_DPARAMETER1;      //0x0248

_regs_	UINT _res           :28;
        UINT SMCLR          : 1;
        UINT SMRST          : 1;
        UINT SMFLUSH        : 1;
        UINT SMCMD          : 1;    _rege_  _H264_MCMD;             //0x0380(W)

_regs_	UINT _res           :31;
        UINT SMSTATUS       : 1;    _rege_  _H264_MSTATUS;          //0x0380(R)

_regs_	UINT SMRQWAIT       :24;
        UINT _res1          : 3;
        UINT SMINTRMASK     : 1;
        UINT SMBUFTYPE      : 1;
        UINT SMSLCNUM       : 3;    _rege_  _H264_MPARAMETER;       //0x0388

_regs_                              _rege_  _H264_MEMYBGN;          //0x0280

_regs_                              _rege_  _H264_MEMYEND;          //0x0288

_regs_                              _rege_  _H264_MEMCBGN;          //0x0290

_regs_                              _rege_  _H264_MEMCEND;          //0x0298

_regs_  UINT _res0          : 4;
        UINT MIHSIZE        :12;
        UINT _res1          : 4;
        UINT MIVSIZE        :12;    _rege_  _H264_MISIZE;           //0x0300

_regs_  UINT _res           :29;
        UINT MCABACIDC      : 2;
        UINT MCABACEN       : 1;    _rege_  _H264_MCABAC;           //0x0308

_regs_  UINT _reg           :26;
        UINT MQSCALE        : 6;    _rege_  _H264_MQSCALE;          //0x0310

_regs_  UINT _reg           :31;
        UINT MPFRAME        : 1;    _rege_  _H264_MPFRAME;          //0x0318

_regs_  UINT _reg           :31;
        UINT MDVSLCEN       : 1;    _rege_  _H264_MDVSLCEN;         //0x0320

_regs_                              _rege_  _H264_MBUFS;            //0x0390

_regs_                              _rege_  _H264_MBUFE;            //0x0398

_regs_                              _rege_  _H264_S0ADR;            //0x03A0

_regs_                              _rege_  _H264_S1ADR;            //0x03A8

_regs_                              _rege_  _H264_S2ADR;            //0x03B0

_regs_                              _rege_  _H264_S3ADR;            //0x03B8

_regs_                              _rege_  _H264_S0SIZE;           //0x03C0

_regs_                              _rege_  _H264_S1SIZE;           //0x03C8

_regs_                              _rege_  _H264_S2SIZE;           //0x03D0

_regs_                              _rege_  _H264_S3SIZE;           //0x03D8

_regs_                              _rege_  _H264_SWADR;            //0x03E0

_regs_                              _rege_  _H264_SMSIZE;           //0x03E8

//------------------------------------------------------------------------------
// Addr/Bit macro
#define H264_IHSIZE         _bm(_H264_IMGSIZE   , H264_REG_BASE, 0x000, IHSIZE      )   // RW
#define H264_IVSIZE         _bm(_H264_IMGSIZE   , H264_REG_BASE, 0x000, IVSIZE      )   // RW
#define H264_SIZE_T         _am(_H264_IMGSIZE   , H264_REG_BASE, 0x000              )   // RW

#define H264_YSTRIDEX       _bm(_H264_STRIDE    , H264_REG_BASE, 0x008, YSTRIDEX    )   // RW
#define H264_CSTRIDEX       _bm(_H264_STRIDE    , H264_REG_BASE, 0x008, CSTRIDEX    )   // RW
#define H264_STRIDE_T       _am(_H264_STRIDE    , H264_REG_BASE, 0x008              )   // RW

#define H264_TMBNUM         _bm(_H264_TMBNUM    , H264_REG_BASE, 0x010, TMBNUM      )   // RW

#define H264_IDR_FLAG       _bm(_H264_PICTYPE   , H264_REG_BASE, 0x020, IDR_FLAG    )   // RW
#define H264_PFRAME         _bm(_H264_PICTYPE   , H264_REG_BASE, 0x020, PFRAME      )   // RW
#define H264_PICTYPE_T      _am(_H264_PICTYPE   , H264_REG_BASE, 0x020              )   // RW

#define H264_QSCALE         _bm(_H264_QINIT     , H264_REG_BASE, 0x028, QSCALE      )   // RW

#define H264_ALN32          _bm(_H264_ENCOPT    , H264_REG_BASE, 0x030, ALN32       )   // RW
#define H264_QUANTMODE      _bm(_H264_ENCOPT    , H264_REG_BASE, 0x030, QUANTMODE   )   // RW
#define H264_RINGTYPE       _bm(_H264_ENCOPT    , H264_REG_BASE, 0x030, RINGTYPE    )   // RW
#define H264_PPS_QP         _bm(_H264_ENCOPT    , H264_REG_BASE, 0x030, PPS_QP      )   // RW
#define H264_SRCHMODE       _bm(_H264_ENCOPT    , H264_REG_BASE, 0x030, SRCHMODE    )   // RW
#define H264_ENCOPT_T       _am(_H264_ENCOPT    , H264_REG_BASE, 0x030              )   // RW

#define H264_FRMBITS        _bm(_H264_HDRINFO1  , H264_REG_BASE, 0x040, FRMBITS     )   // RW
#define H264_FRMNUM         _bm(_H264_HDRINFO1  , H264_REG_BASE, 0x040, FRMNUM      )   // RW
#define H264_HDRINFO1_T     _am(_H264_HDRINFO1  , H264_REG_BASE, 0x040              )   // RW

#define H264_PROFILEIDC     _bm(_H264_HDRINFO2  , H264_REG_BASE, 0x048, PROFILEIDC  )   // RW
#define H264_CABACIDC       _bm(_H264_HDRINFO2  , H264_REG_BASE, 0x048, CABACIDC    )   // RW
#define H264_CABACEN        _bm(_H264_HDRINFO2  , H264_REG_BASE, 0x048, CABACEN     )   // RW
#define H264_CROPBOT        _bm(_H264_HDRINFO2  , H264_REG_BASE, 0x048, CROPBOT     )   // RW
#define H264_LEVIDC         _bm(_H264_HDRINFO2  , H264_REG_BASE, 0x048, LEVIDC      )   // RW
#define H264_HDRINFO2_T     _am(_H264_HDRINFO2  , H264_REG_BASE, 0x048              )   // RW

#define H264_SPS_SPSID      _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, SPS_SPSID   )   // RW
#define H264_PPS_SPSID      _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, PPS_SPSID   )   // RW
#define H264_PPS_PPSID      _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, PPS_PPSID   )   // RW
#define H264_SLH_PPSID      _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, SLH_PPSID   )   // RW
#define H264_SPSINSRT       _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, SPSINSRT    )   // RW
#define H264_PPSINSRT       _bm(_H264_HDRINSERT , H264_REG_BASE, 0x050, PPSINSRT    )   // RW
#define H264_HDRINSERT_T    _am(_H264_HDRINSERT , H264_REG_BASE, 0x050              )   // RW

#define H264_PREPTR         _bm(_H264_SMEMWPTR  , H264_REG_BASE, 0x060, PREPTR      )   // RW

#define H264_PSTPTR         _bm(_H264_SMEMRPTR  , H264_REG_BASE, 0x068, PSTPTR      )   // RW

#define H264_INTRVC         _bm(_H264_INTRVC    , H264_REG_BASE, 0x080, INTRVC      )   // RW

#define H264_CURYBUF        _am(_H264_CURYBUF   , H264_REG_BASE, 0x088              )   // RW
#define H264_CURCBUF        _am(_H264_CURCBUF   , H264_REG_BASE, 0x090              )   // RW
#define H264_CUREBUF        _am(_H264_CUREBUF   , H264_REG_BASE, 0x098              )   // RW
#define H264_REFYBUF        _am(_H264_REFYBUF   , H264_REG_BASE, 0x0A0              )   // RW
#define H264_REFCBUF        _am(_H264_REFCBUF   , H264_REG_BASE, 0x0A8              )   // RW
#define H264_RECYBUF        _am(_H264_RECYBUF   , H264_REG_BASE, 0x0B0              )   // RW
#define H264_RECCBUF        _am(_H264_RECCBUF   , H264_REG_BASE, 0x0B8              )   // RW
#define H264_STRMBUF        _am(_H264_STRMBUF   , H264_REG_BASE, 0x0C0              )   // RW
#define H264_SBUFEND        _am(_H264_SBUFEND   , H264_REG_BASE, 0x0C8              )   // RW
#define H264_STPSBUFW       _am(_H264_STPSBUFW  , H264_REG_BASE, 0x0D0              )   // RW
#define H264_STPIBUFW       _am(_H264_STPIBUFW  , H264_REG_BASE, 0x0D8              )   // RW
#define H264_STPIBUFR       _am(_H264_STPIBUFR  , H264_REG_BASE, 0x0E0              )   // RW
#define H264_SBUFSIZE       _am(_H264_SBUFSIZE  , H264_REG_BASE, 0x0E8              )   // RW

#define H264_VLCBURST       _bm(_H264_AXIINFO1  , H264_REG_BASE, 0x0F0, VLCBURST    )   // RW
#define H264_ENCBURST       _bm(_H264_AXIINFO1  , H264_REG_BASE, 0x0F0, ENCBURST    )   // RW
#define H264_AXIINFO1_T     _am(_H264_AXIINFO1  , H264_REG_BASE, 0x0F0              )   // RW

#define H264_USEIADDR       _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, USEIADDR    )   // RW
#define H264_KEEP4KB        _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, KEEP4KB     )   // RW
#define H264_CLKRATIO       _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, CLKRATIO    )   // RW
#define H264_KEEPORDER      _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, KEEPORDER   )   // RW
#define H264_RNDROBIN       _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, RNDROBIN    )   // RW
#define H264_EXTROBIN       _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, EXTROBIN    )   // RW
#define H264_BIGENDIAN      _bm(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8, BIGENDIAN   )   // RW
#define H264_AXIINFO2_T     _am(_H264_AXIINFO2  , H264_REG_BASE, 0x0F8              )   // RW

#define H264_STRMRESET      _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, STRMRESET   )   // RW
#define H264_RINGINIT       _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, RINGINIT    )   // RW
#define H264_VLCSTART       _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, VLCSTART    )   // RW
#define H264_FLUSHSTART     _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, FLUSHSTART  )   // RW
#define H264_VLCREADY       _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, VLCREADY    )   // RW
#define H264_ENCSTART       _bm(_H264_ENCSTART  , H264_REG_BASE, 0x100, ENCSTART    )   // RW
#define H264_ENCSTART_T     _am(_H264_ENCSTART  , H264_REG_BASE, 0x100              )   // RW

#define H264_SBUFFULL       _bm(_H264_STATUS    , H264_REG_BASE, 0x108, SBUFFULL    )   // RW
#define H264_VLCEND         _bm(_H264_STATUS    , H264_REG_BASE, 0x108, VLCEND      )   // RW
#define H264_FLUSHEND       _bm(_H264_STATUS    , H264_REG_BASE, 0x108, FLUSHEND    )   // RW
#define H264_EVEND          _bm(_H264_STATUS    , H264_REG_BASE, 0x108, EVEND       )   // RW
#define H264_ENCEND         _bm(_H264_STATUS    , H264_REG_BASE, 0x108, ENCEND      )   // RW
#define H264_STATUS_T       _am(_H264_STATUS    , H264_REG_BASE, 0x108              )   // RW

#define H264_GENBITS        _bm(_H264_GENBITS   , H264_REG_BASE, 0x110, GENBITS     )   // RW

#define H264_SBUFWPOS       _am(_H264_SBUFWPOS  , H264_REG_BASE, 0x118              )   // RW

#define H264_SERRMASK       _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, SERRMASK    )   // RW
#define H264_EERRMASK       _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, EERRMASK    )   // RW
#define H264_INTRMASK4      _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, INTRMASK4   )   // RW
#define H264_INTRMASK3      _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, INTRMASK3   )   // RW
#define H264_INTRMASK2      _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, INTRMASK2   )   // RW
#define H264_INTRMASK1      _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, INTRMASK1   )   // RW
#define H264_INTRMASK0      _bm(_H264_INTRMASK  , H264_REG_BASE, 0x120, INTRMASK0   )   // RW
#define H264_INTRMASK_T     _am(_H264_INTRMASK  , H264_REG_BASE, 0x120              )   // RW

#define H264_CURWAIT        _bm(_H264_OPWAIT    , H264_REG_BASE, 0x128, CURWAIT     )   // RW
#define H264_REFWAIT        _bm(_H264_OPWAIT    , H264_REG_BASE, 0x128, REFWAIT     )   // RW
#define H264_VLCWAIT        _bm(_H264_OPWAIT    , H264_REG_BASE, 0x128, VLCWAIT     )   // RW
#define H264_OPWAIT_T       _am(_H264_OPWAIT    , H264_REG_BASE, 0x128              )   // RW

#define H264_SFTBITS        _bm(_H264_STRMPACK  , H264_REG_BASE, 0x180, SFTBITS     )   // WO
#define H264_SFTDATA        _bm(_H264_STRMPACK  , H264_REG_BASE, 0x180, SFTDATA     )   // WO
#define H264_STRMPACK_T     _am(_H264_STRMPACK  , H264_REG_BASE, 0x180              )   // WO

#define H264_SFTWAIT        _bm(_H264_STRMWAIT  , H264_REG_BASE, 0x188, SFTWAIT     )   // RO

#define H264_ECHNNEL        _bm(_H264_ECHNNEL   , H264_REG_BASE, 0x190, ECHNNEL     )   // RW

#define H264_MCHNNEL        _bm(_H264_MCHNNEL   , H264_REG_BASE, 0x198, MCHNNEL     )   // RW

#define H264_PREBITS        _bm(_H264_PREBITS   , H264_REG_BASE, 0x1A8, PREBITS     )   // RW

#define H264_ROI_EN_0       _bm(_H264_ROI_0     , H264_REG_BASE, 0x200, ROI_EN_0    )   // RW
#define H264_ROI_Q_0        _bm(_H264_ROI_0     , H264_REG_BASE, 0x200, ROI_Q_0     )   // RW
#define H264_ROI_0          _am(_H264_ROI_0     , H264_REG_BASE, 0x200              )   // RW

#define H264_ROI_POS_0      _am(_H264_ROI_0_POS , H264_REG_BASE, 0x208              )   // RW

#define H264_ROI_EN_1       _bm(_H264_ROI_1     , H264_REG_BASE, 0x210, ROI_EN_1    )   // RW
#define H264_ROI_Q_1        _bm(_H264_ROI_1     , H264_REG_BASE, 0x210, ROI_Q_1     )   // RW
#define H264_ROI_1          _am(_H264_ROI_1     , H264_REG_BASE, 0x210              )   // RW

#define H264_ROI_POS_1      _am(_H264_ROI_1_POS , H264_REG_BASE, 0x21c              )   // RW

#define H264_ROI_EN_2       _bm(_H264_ROI_2     , H264_REG_BASE, 0x220, ROI_EN_2    )   // RW
#define H264_ROI_Q_2        _bm(_H264_ROI_2     , H264_REG_BASE, 0x220, ROI_Q_2     )   // RW
#define H264_ROI_2          _am(_H264_ROI_2     , H264_REG_BASE, 0x220              )   // RW

#define H264_ROI_POS_2      _am(_H264_ROI_2_POS , H264_REG_BASE, 0x228              )   // RW

#define H264_ROI_EN_3       _bm(_H264_ROI_3     , H264_REG_BASE, 0x230, ROI_EN_3    )   // RW
#define H264_ROI_Q_3        _bm(_H264_ROI_3     , H264_REG_BASE, 0x230, ROI_Q_3     )   // RW
#define H264_ROI_3          _am(_H264_ROI_3     , H264_REG_BASE, 0x230              )   // RW

#define H264_ROI_POS_3      _am(_H264_ROI_3_POS , H264_REG_BASE, 0x238              )   // RW

#define H264_DVADR          _bm(_H264_DPARAMETER0,H264_REG_BASE, 0x240, DVADR       )   // RW
#define H264_DVIDX          _bm(_H264_DPARAMETER0,H264_REG_BASE, 0x240, DVIDX       )   // RW
#define H264_DVNUM          _bm(_H264_DPARAMETER0,H264_REG_BASE, 0x240, DVNUM       )   // RW
#define H264_DVSLCEN        _bm(_H264_DPARAMETER0,H264_REG_BASE, 0x240, DVSLCEN     )   // RW

#define H264_ORGH           _bm(_H264_DPARAMETER1,H264_REG_BASE, 0x248, ORGH        )   // RW
#define H264_ORGV           _bm(_H264_DPARAMETER1,H264_REG_BASE, 0x248, ORGV        )   // RW

#define H264_MEMYBGN        _am(_H264_MEMYBGN   , H264_REG_BASE, 0x280              )   // RW
#define H264_MEMYEND        _am(_H264_MEMYEND   , H264_REG_BASE, 0x288              )   // RW
#define H264_MEMCBGN        _am(_H264_MEMCBGN   , H264_REG_BASE, 0x290              )   // RW
#define H264_MEMCEND        _am(_H264_MEMCEND   , H264_REG_BASE, 0x298              )   // RW

#define H264_MIHSIZE        _bm(_H264_MISIZE    , H264_REG_BASE, 0x300, MIHSIZE     )   // RW
#define H264_MIVSIZE        _bm(_H264_MISIZE    , H264_REG_BASE, 0x300, MIVSIZE     )   // RW
#define H264_MISIZE_T       _am(_H264_MISIZE    , H264_REG_BASE, 0x300              )   // RW

#define H264_MCABACIDC      _bm(_H264_MCABAC    , H264_REG_BASE, 0x308, MCABACIDC   )   // RW
#define H264_MCABACEN       _bm(_H264_MCABAC    , H264_REG_BASE, 0x308, MCABACEN    )   // RW
#define H264_MCABAC_T       _am(_H264_MCABAC    , H264_REG_BASE, 0x308              )   // RW

#define H264_MQSCALE        _bm(_H264_MQSCALE   , H264_REG_BASE, 0x310, MQSCALE     )   // RW

#define H264_MPFRAME        _bm(_H264_MPFRAME   , H264_REG_BASE, 0x318, MPFRAME     )   // RW

#define H264_MDVSLCEN       _bm(_H264_MDVSLCEN  , H264_REG_BASE, 0x320, MDVSLCEN    )   // RW

#define H264_SMCLR          _bm(_H264_MCMD      , H264_REG_BASE, 0x380, SMCLR       )   // WO
#define H264_SMRST          _bm(_H264_MCMD      , H264_REG_BASE, 0x380, SMRST       )   // WO
#define H264_SMFLUSH        _bm(_H264_MCMD      , H264_REG_BASE, 0x380, SMFLUSH     )   // WO
#define H264_SMCMD          _bm(_H264_MCMD      , H264_REG_BASE, 0x380, SMCMD       )   // WO

#define H264_SMSTATUS       _bm(_H264_MSTATUS   , H264_REG_BASE, 0x380, SMSTATUS    )   // RO

#define H264_SMRQWAIT       _bm(_H264_MPARAMETER, H264_REG_BASE, 0x388, SMRQWAIT    )   // RW
#define H264_SMINTRMASK     _bm(_H264_MPARAMETER, H264_REG_BASE, 0x388, SMINTRMASK  )   // RW
#define H264_SMBUFTYPE      _bm(_H264_MPARAMETER, H264_REG_BASE, 0x388, SMBUFTYPE   )   // RW
#define H264_SMSLCNUM       _bm(_H264_MPARAMETER, H264_REG_BASE, 0x388, SMSLCNUM    )   // RW

#define H264_MBUFS          _am(_H264_MBUFS     , H264_REG_BASE, 0x390              )   // RW
#define H264_MBUFE          _am(_H264_MBUFE     , H264_REG_BASE, 0x398              )   // RW
#define H264_S0ADR          _am(_H264_S0ADR     , H264_REG_BASE, 0x3A0              )   // RW
#define H264_S1ADR          _am(_H264_S1ADR     , H264_REG_BASE, 0x3A8              )   // RW
#define H264_S2ADR          _am(_H264_S2ADR     , H264_REG_BASE, 0x3B0              )   // RW
#define H264_S3ADR          _am(_H264_S3ADR     , H264_REG_BASE, 0x3B8              )   // RW
#define H264_S0SIZE         _am(_H264_S0SIZE    , H264_REG_BASE, 0x3C0              )   // RW
#define H264_S1SIZE         _am(_H264_S1SIZE    , H264_REG_BASE, 0x3C8              )   // RW
#define H264_S2SIZE         _am(_H264_S2SIZE    , H264_REG_BASE, 0x3D0              )   // RW
#define H264_S3SIZE         _am(_H264_S3SIZE    , H264_REG_BASE, 0x3D8              )   // RW
#define H264_SWADR          _am(_H264_SWADR     , H264_REG_BASE, 0x3E0              )   // RW
#define H264_SMSIZE         _am(_H264_SMSIZE    , H264_REG_BASE, 0x3E8              )   // RW

//******************************************************************************
// 16. SDIO
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT CLK_DIV        :10;
        UINT _res0          : 3;
        UINT EN             : 1;
        UINT CLKEN          : 1;
        UINT MODE           : 1;
        UINT _res1          : 3;
        UINT DET            : 1;
        UINT DET_IRQ        : 1;
        UINT DET_IRQ_EN     : 1;
        UINT DET_IRQ_CLR    : 1;
        UINT CHIP_IRQ       : 1;
        UINT CHIP_IRQ_EN    : 1;
        UINT CHIP_IRQ_CLR   : 1;
        UINT DAT_IRQ        : 1;
        UINT DAT_IRQ_EN     : 1;
        UINT DAT_IRQ_CLR    : 1;
        UINT CMD_IRQ        : 1;
        UINT CMD_IRQ_EN     : 1;
        UINT CMD_IRQ_CLR    : 1;    _rege_  _SDIO_CONT;

_regs_  UINT _res0          : 4;
        UINT DAT_BL         : 12;
        UINT _reg1          : 4;
        UINT CMD_RESP_BUSYEN: 1;
        UINT DAT_WE         : 1;
        UINT DAT_EN         : 1;
        UINT CMD_EN         : 1;
        UINT CMD_IDX        : 6;
        UINT CMD_RESPTYPE   : 1;
        UINT CMD_RESPEN     : 1;    _rege_  _SDIO_CMD;

_regs_  UINT _res0          : 4;
        UINT BUSY           : 1;
        UINT DAT_FULL       : 1;
        UINT DAT_EMPTY      : 1;
        UINT DAT_CRCERR     : 1;
        UINT _res1          : 2;
        UINT CMD_RESP_IDX   : 6;
        UINT _res2          : 1;
        UINT CMD_RESP_CRC   : 7;
        UINT _res3          : 6;
        UINT CMD_RESPTOUT   : 1;
        UINT CMD_CRCERR     : 1;    _rege_  _SDIO_CMDRESP;

_regs_  UINT CMD_ARG        :32;    _rege_  _SDIO_CMDARG;

_regs_  UINT DAT            :32;    _rege_  _SDIO_RESPDAT;

_regs_  UINT LEN            :32;    _rege_  _SDIO_DATLEN;

_regs_  UINT CMD_TIMEOUT    :32;    _rege_  _SDIO_CMD_TIMEOUT;

_regs_  UINT _res           :24;
        UINT DAT_TIMEOUT    : 8;    _rege_  _SDIO_DAT_TIMEOUT;

//------------------------------------------------------------------------------
// Addr/Bit macro(SDIO0)
#define SDIO0_CLK_DIV       _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CLK_DIV     )   // RW
#define SDIO0_EN            _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, EN          )   // RW
#define SDIO0_CLKEN         _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CLKEN       )   // RW
#define SDIO0_MODE          _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, MODE        )   // RW
#define SDIO0_DET           _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DET         )   // RO
#define SDIO0_DET_IRQ       _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DET_IRQ     )   // RO
#define SDIO0_DET_IRQ_EN    _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DET_IRQ_EN  )   // RW
#define SDIO0_DET_IRQ_CLR   _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DET_IRQ_CLR )   // WO
#define SDIO0_CHIP_IRQ      _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CHIP_IRQ    )   // RO
#define SDIO0_CHIP_IRQ_EN   _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CHIP_IRQ_EN )   // RW
#define SDIO0_CHIP_IRQ_CLR  _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CHIP_IRQ_CLR)   // WO
#define SDIO0_DAT_IRQ       _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DAT_IRQ     )   // RO
#define SDIO0_DAT_IRQ_EN    _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DAT_IRQ_EN  )   // RW
#define SDIO0_DAT_IRQ_CLR   _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, DAT_IRQ_CLR )   // WO
#define SDIO0_CMD_IRQ       _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CMD_IRQ     )   // RO
#define SDIO0_CMD_IRQ_EN    _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CMD_IRQ_EN  )   // RW
#define SDIO0_CMD_IRQ_CLR   _bm(_SDIO_CONT      , SDIO0_REG_BASE, 0x00, CMD_IRQ_CLR )   // WO

#define SDIO0_DAT_BL        _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, DAT_BL      )   // RW
#define SDIO0_CMD_RESP_BUSYEN _bm(_SDIO_CMD     , SDIO0_REG_BASE, 0x04, CMD_RESP_BUSYEN)// RW
#define SDIO0_DAT_WE        _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, DAT_WE      )   // RW
#define SDIO0_DAT_EN        _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, DAT_EN      )   // RW
#define SDIO0_CMD_EN        _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, CMD_EN      )   // RW
#define SDIO0_CMD_IDX       _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, CMD_IDX     )   // RW
#define SDIO0_CMD_RESPTYPE  _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, CMD_RESPTYPE)   // RW
#define SDIO0_CMD_RESPEN    _bm(_SDIO_CMD       , SDIO0_REG_BASE, 0x04, CMD_RESPEN  )   // RW

#define SDIO0_BUSY          _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, BUSY        )   // RO
#define SDIO0_DAT_FULL      _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, DAT_FULL    )   // RO
#define SDIO0_DAT_EMPTY     _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, DAT_EMPTY   )   // RO
#define SDIO0_DAT_CRCERR    _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, DAT_CRCERR  )   // RO
#define SDIO0_CMD_RESP_IDX  _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, CMD_RESP_IDX)   // RO
#define SDIO0_CMD_RESP_CRC  _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, CMD_RESP_CRC)   // RO
#define SDIO0_CMD_RESPTOUT  _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, CMD_RESPTOUT)   // RO
#define SDIO0_CMD_CRCERR    _bm(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08, CMD_CRCERR  )   // RO
#define SDIO0_CMD_RESP_T    _am(_SDIO_CMDRESP   , SDIO0_REG_BASE, 0x08              )   // RO

#define SDIO0_CMD_ARG       _am(_SDIO_CMDARG    , SDIO0_REG_BASE, 0x0c              )   // RW

#define SDIO0_RESP0         _am(_SDIO_RESPDAT   , SDIO0_REG_BASE, 0x10              )   // RW

#define SDIO0_RESP1         _am(_SDIO_RESPDAT   , SDIO0_REG_BASE, 0x14              )   // RW

#define SDIO0_RESP2         _am(_SDIO_RESPDAT   , SDIO0_REG_BASE, 0x18              )   // RW

#define SDIO0_RESP3         _am(_SDIO_RESPDAT   , SDIO0_REG_BASE, 0x1c              )   // RW

#define SDIO0_DATLEN        _am(_SDIO_DATLEN    , SDIO0_REG_BASE, 0x20              )   // RW

#define SDIO0_CMD_TIMEOUT   _am(_SDIO_CMD_TIMEOUT, SDIO0_REG_BASE, 0x24             )   // RW

#define SDIO0_DAT_TIMEOUT   _bm(_SDIO_DAT_TIMEOUT, SDIO0_REG_BASE, 0x28, DAT_TIMEOUT)   // RW

//------------------------------------------------------------------------------
// Addr/Bit macro(SDIO1)
#define SDIO1_CLK_DIV       _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CLK_DIV     )   // RW
#define SDIO1_EN            _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, EN          )   // RW
#define SDIO1_CLKEN         _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CLKEN       )   // RW
#define SDIO1_MODE          _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, MODE        )   // RW
#define SDIO1_DET           _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DET         )   // RO
#define SDIO1_DET_IRQ       _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DET_IRQ     )   // RO
#define SDIO1_DET_IRQ_EN    _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DET_IRQ_EN  )   // RW
#define SDIO1_DET_IRQ_CLR   _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DET_IRQ_CLR )   // WO
#define SDIO1_CHIP_IRQ      _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CHIP_IRQ    )   // RO
#define SDIO1_CHIP_IRQ_EN   _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CHIP_IRQ_EN )   // RW
#define SDIO1_CHIP_IRQ_CLR  _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CHIP_IRQ_CLR)   // WO
#define SDIO1_DAT_IRQ       _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DAT_IRQ     )   // RO
#define SDIO1_DAT_IRQ_EN    _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DAT_IRQ_EN  )   // RW
#define SDIO1_DAT_IRQ_CLR   _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, DAT_IRQ_CLR )   // WO
#define SDIO1_CMD_IRQ       _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CMD_IRQ     )   // RO
#define SDIO1_CMD_IRQ_EN    _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CMD_IRQ_EN  )   // RW
#define SDIO1_CMD_IRQ_CLR   _bm(_SDIO_CONT      , SDIO1_REG_BASE, 0x00, CMD_IRQ_CLR )   // WO

#define SDIO1_DAT_BL        _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, DAT_BL      )   // RW
#define SDIO1_CMD_RESP_BUSYEN _bm(_SDIO_CMD     , SDIO1_REG_BASE, 0x04, CMD_RESP_BUSYEN)// RW
#define SDIO1_DAT_WE        _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, DAT_WE      )   // RW
#define SDIO1_DAT_EN        _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, DAT_EN      )   // RW
#define SDIO1_CMD_EN        _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, CMD_EN      )   // RW
#define SDIO1_CMD_IDX       _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, CMD_IDX     )   // RW
#define SDIO1_CMD_RESPTYPE  _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, CMD_RESPTYPE)   // RW
#define SDIO1_CMD_RESPEN    _bm(_SDIO_CMD       , SDIO1_REG_BASE, 0x04, CMD_RESPEN  )   // RW

#define SDIO1_BUSY          _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, BUSY        )   // RO
#define SDIO1_DAT_FULL      _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, DAT_FULL    )   // RO
#define SDIO1_DAT_EMPTY     _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, DAT_EMPTY   )   // RO
#define SDIO1_DAT_CRCERR    _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, DAT_CRCERR  )   // RO
#define SDIO1_CMD_RESP_IDX  _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, CMD_RESP_IDX)   // RO
#define SDIO1_CMD_RESP_CRC  _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, CMD_RESP_CRC)   // RO
#define SDIO1_CMD_RESPTOUT  _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, CMD_RESPTOUT)   // RO
#define SDIO1_CMD_CRCERR    _bm(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08, CMD_CRCERR  )   // RO
#define SDIO1_CMD_RESP_T    _am(_SDIO_CMDRESP   , SDIO1_REG_BASE, 0x08              )   // RO

#define SDIO1_CMD_ARG       _am(_SDIO_CMDARG    , SDIO1_REG_BASE, 0x0c              )   // RW

#define SDIO1_RESP0         _am(_SDIO_RESPDAT   , SDIO1_REG_BASE, 0x10              )   // RW

#define SDIO1_RESP1         _am(_SDIO_RESPDAT   , SDIO1_REG_BASE, 0x14              )   // RW

#define SDIO1_RESP2         _am(_SDIO_RESPDAT   , SDIO1_REG_BASE, 0x18              )   // RW

#define SDIO1_RESP3         _am(_SDIO_RESPDAT   , SDIO1_REG_BASE, 0x1c              )   // RW

#define SDIO1_DATLEN        _am(_SDIO_DATLEN    , SDIO1_REG_BASE, 0x20              )   // RW

#define SDIO1_CMD_TIMEOUT   _am(_SDIO_CMD_TIMEOUT, SDIO1_REG_BASE, 0x24             )   // RW

#define SDIO1_DAT_TIMEOUT   _bm(_SDIO_DAT_TIMEOUT, SDIO1_REG_BASE, 0x28, DAT_TIMEOUT)   // RW

//------------------------------------------------------------------------------
// Extern
extern BOOL Sdio0Cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe);
extern BOOL Sdio1Cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe);

//******************************************************************************
// 17. I2C
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT RX_DAT         : 8;
        UINT TX_DAT         : 8;
        UINT _res0          :10;
        UINT ACT            : 1;
        UINT MODE           : 1;
        UINT BIT_MODE       : 1;
        UINT IRQ            : 1;
        UINT IRQ_EN         : 1;
        UINT IRQ_CLR        : 1;    _rege_  _I2C_CONT0;

_regs_  UINT CLK_DIV        :16;
        UINT _res           :16;    _rege_  _I2C_CONT1;

_regs_  UINT _res           :26;
        UINT MST_COL        : 1;
        UINT MST_ACK        : 1;
        UINT MST_REPEAT     : 1;
        UINT MST_LAST       : 1;
        UINT MST_RW         : 1;
        UINT MST_GO         : 1;    _rege_  _I2C_CONT_MST;

_regs_  UINT _res           :21;
        UINT SLV_ACK_IN     : 1;
        UINT SLV_GO         : 1;
        UINT SLV_RW         : 1;
        UINT SLV_ACK_OUT    : 1;
        UINT SLV_ADR        : 7;    _rege_  _I2C_CONT_SLV;

//------------------------------------------------------------------------------
// Addr/Bit macro(I2C0)
#define I2C0_CONT           _am(_I2C_CONT0       , I2C0_REG_BASE, 0x00              )
#define I2C0_RX_DAT         _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, RX_DAT      )   // RO
#define I2C0_TX_DAT         _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, TX_DAT      )   // RW
#define I2C0_ACT            _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, ACT         )   // RO
#define I2C0_MODE           _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, MODE        )   // RW
#define I2C0_BIT_MODE       _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, BIT_MODE    )   // RW
#define I2C0_IRQ            _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, IRQ         )   // RO
#define I2C0_IRQ_EN         _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, IRQ_EN      )   // RW
#define I2C0_IRQ_CLR        _bm(_I2C_CONT0       , I2C0_REG_BASE, 0x00, IRQ_CLR     )   // WO

#define I2C0_CLK_DIV        _bm(_I2C_CONT1       , I2C0_REG_BASE, 0x04, CLK_DIV     )   // RW

#define I2C0_CONT_MST       _am(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08              )
#define I2C0_MST_COL        _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_COL     )   // RO
#define I2C0_MST_ACK        _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_ACK     )   // RO
#define I2C0_MST_REPEAT     _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_REPEAT  )   // RW
#define I2C0_MST_LAST       _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_LAST    )   // RW
#define I2C0_MST_RW         _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_RW      )   // RW
#define I2C0_MST_GO         _bm(_I2C_CONT_MST    , I2C0_REG_BASE, 0x08, MST_GO      )   // WO

#define I2C0_CONT_SLV       _am(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c              )
#define I2C0_SLV_ACK_IN     _bm(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c, SLV_ACK_IN  )   // RO
#define I2C0_SLV_GO         _bm(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c, SLV_GO      )   // WO
#define I2C0_SLV_RW         _bm(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c, SLV_RW      )   // RW
#define I2C0_SLV_ACK_OUT    _bm(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c, SLV_ACK_OUT )   // RW
#define I2C0_SLV_ADR        _bm(_I2C_CONT_SLV    , I2C0_REG_BASE, 0x0c, SLV_ADR     )   // RW

//------------------------------------------------------------------------------
// Addr/Bit macro(I2C1)
#define I2C1_CONT           _am(_I2C_CONT0       , I2C1_REG_BASE, 0x00              )
#define I2C1_RX_DAT         _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, RX_DAT      )   // RO
#define I2C1_TX_DAT         _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, TX_DAT      )   // RW
#define I2C1_ACT            _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, ACT         )   // RO
#define I2C1_MODE           _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, MODE        )   // RW
#define I2C1_BIT_MODE       _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, BIT_MODE    )   // RW
#define I2C1_IRQ            _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, IRQ         )   // RO
#define I2C1_IRQ_EN         _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, IRQ_EN      )   // RW
#define I2C1_IRQ_CLR        _bm(_I2C_CONT0       , I2C1_REG_BASE, 0x00, IRQ_CLR     )   // WO

#define I2C1_CLK_DIV        _bm(_I2C_CONT1       , I2C1_REG_BASE, 0x04, CLK_DIV     )   // RW

#define I2C1_CONT_MST       _am(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08              )
#define I2C1_MST_COL        _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_COL     )   // RO
#define I2C1_MST_ACK        _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_ACK     )   // RO
#define I2C1_MST_REPEAT     _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_REPEAT  )   // RW
#define I2C1_MST_LAST       _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_LAST    )   // RW
#define I2C1_MST_RW         _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_RW      )   // RW
#define I2C1_MST_GO         _bm(_I2C_CONT_MST    , I2C1_REG_BASE, 0x08, MST_GO      )   // WO

#define I2C1_CONT_SLV       _am(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c              )
#define I2C1_SLV_ACK_IN     _bm(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c, SLV_ACK_IN  )   // RO
#define I2C1_SLV_GO         _bm(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c, SLV_GO      )   // WO
#define I2C1_SLV_RW         _bm(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c, SLV_RW      )   // RW
#define I2C1_SLV_ACK_OUT    _bm(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c, SLV_ACK_OUT )   // RW
#define I2C1_SLV_ADR        _bm(_I2C_CONT_SLV    , I2C1_REG_BASE, 0x0c, SLV_ADR     )   // RW

//------------------------------------------------------------------------------
// Addr/Bit macro(I2C2)
#define I2C2_CONT           _am(_I2C_CONT0       , I2C2_REG_BASE, 0x00              )
#define I2C2_RX_DAT         _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, RX_DAT      )   // RO
#define I2C2_TX_DAT         _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, TX_DAT      )   // RW
#define I2C2_ACT            _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, ACT         )   // RO
#define I2C2_MODE           _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, MODE        )   // RW
#define I2C2_BIT_MODE       _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, BIT_MODE    )   // RW
#define I2C2_IRQ            _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, IRQ         )   // RO
#define I2C2_IRQ_EN         _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, IRQ_EN      )   // RW
#define I2C2_IRQ_CLR        _bm(_I2C_CONT0       , I2C2_REG_BASE, 0x00, IRQ_CLR     )   // WO

#define I2C2_CLK_DIV        _bm(_I2C_CONT1       , I2C2_REG_BASE, 0x04, CLK_DIV     )   // RW

#define I2C2_CONT_MST       _am(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08              )
#define I2C2_MST_COL        _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_COL     )   // RO
#define I2C2_MST_ACK        _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_ACK     )   // RO
#define I2C2_MST_REPEAT     _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_REPEAT  )   // RW
#define I2C2_MST_LAST       _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_LAST    )   // RW
#define I2C2_MST_RW         _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_RW      )   // RW
#define I2C2_MST_GO         _bm(_I2C_CONT_MST    , I2C2_REG_BASE, 0x08, MST_GO      )   // WO

#define I2C2_CONT_SLV       _am(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c              )
#define I2C2_SLV_ACK_IN     _bm(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c, SLV_ACK_IN  )   // RO
#define I2C2_SLV_GO         _bm(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c, SLV_GO      )   // WO
#define I2C2_SLV_RW         _bm(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c, SLV_RW      )   // RW
#define I2C2_SLV_ACK_OUT    _bm(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c, SLV_ACK_OUT )   // RW
#define I2C2_SLV_ADR        _bm(_I2C_CONT_SLV    , I2C2_REG_BASE, 0x0c, SLV_ADR     )   // RW

//------------------------------------------------------------------------------
// Extern
extern void I2c0_Init(UINT Speed_Hz);
extern BYTE I2c0_Write(BYTE dat, BYTE last, BYTE repeat);
extern BYTE I2c0_Read(BYTE last, BYTE repeat);

extern void I2c1_Init(UINT Speed_Hz);
extern BYTE I2c1_Write(BYTE dat, BYTE last, BYTE repeat);
extern BYTE I2c1_Read(BYTE last, BYTE repeat);
extern void I2c1_Deinit(void);

extern void I2c2_Init(UINT Speed_Hz);
extern BYTE I2c2_Write(BYTE dat, BYTE last, BYTE repeat);
extern BYTE I2c2_Read(BYTE last, BYTE repeat);
extern UINT I2c2_Check(BYTE addr);
extern void I2c2_Deinit(void);

//******************************************************************************
// 18. AUD
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT CLK_DIV        : 8; // Internal generated clock divider BCK = MCK / ((DIV+1)*2)
        UINT CLK_SEL        : 1; // 0: BCK input 1: Internal generated clock
        UINT ACK_PD         : 1;
        UINT TXIRQ_EN       : 1;
        UINT TXIRQ_CLR      : 1;
        UINT TXIRQ          : 1;
        UINT RXIRQ_EN       : 1;
        UINT RXIRQ_CLR      : 1;
        UINT RXIRQ          : 1;
        UINT _res           : 1;
        UINT WB_DEPTH       : 3; // DataDepth for Bus 0: 4bit 1: 8bit 2: 12bit 3: 16bit 4: 20bit 5: 24bit 6: 28bit 7: 32bit
        UINT DECI_DEPTH     : 2; // MEMS Decimation Depth 0: 5bit 1: 6bit 2: 7bit 3: 8bit
        UINT PWM_CH         : 1; // PWM Output Type 0: Mono(I2S_DOUT only) 1: Stereo(I2S_DOUT, I2S_WCK)
        UINT PWM_SIGN       : 1;
        UINT IRQ_TYPE       : 1; // Buff IRQ Limit 0: 512Byte 1: 1024Byte
        UINT IO_MODE        : 1; // Interface Type 0: MEMS + PWM/PDM 1: I2S
        UINT IO_CLKOE       : 1; // Clock(BCK) Output En 0: Off 1: On
        UINT IO_DEPTH       : 3; // DataDepth for IO 0: 4bit 1: 8bit 2: 12bit 3: 16bit 4: 20bit 5: 24bit 6: 28bit 7: 32bit
        UINT TX_EN          : 1; // Tx Enable; 0: Off 1: On
        UINT RX_EN          : 1; // Rx Enable; 0: Off 1: On
                                    _rege_ _AUD_CONT;

_regs_  UINT PWM_CLKDIV     : 8;
        UINT RX_CLR         : 1;
        UINT TX_CLR         : 1;
        UINT _res           :18;
        UINT WB_INCH        : 2; // Data for Input Mono Src: WB_INCH = 1 or 2 Stereo Src : 0: Stereo 1: L Ch Only 2 :R Ch Only
        UINT WB_OUTCH       : 2; // Data for Output 0: Mono 1: Stereo
                                    _rege_ _AUD_PWM;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define AUD_CONT_T          _am(_AUD_CONT        , AUD_REG_BASE , 0x00              )
#define AUD_CLK_DIV         _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, CLK_DIV     )   // RW
#define AUD_CLK_SEL         _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, CLK_SEL     )   // RW
#define AUD_ACK_PD          _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, ACK_PD      )   // RW
#define AUD_TXIRQ_EN        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, TXIRQ_EN    )   // RW
#define AUD_TXIRQ_CLR       _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, TXIRQ_CLR   )   // WO
#define AUD_TXIRQ           _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, TXIRQ       )   // RO
#define AUD_RXIRQ_EN        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, RXIRQ_EN    )   // RW
#define AUD_RXIRQ_CLR       _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, RXIRQ_CLR   )   // WO
#define AUD_RXIRQ           _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, RXIRQ       )   // RO
#define AUD_WB_DEPTH        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, WB_DEPTH    )   // RW
#define AUD_DECI_DEPTH      _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, DECI_DEPTH  )   // RW
#define AUD_PWM_CH          _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, PWM_CH      )   // RW
#define AUD_PWM_SIGN        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, PWM_SIGN    )   // RW
#define AUD_IRQ_TYPE        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, IRQ_TYPE    )   // RW
#define AUD_IO_MODE         _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, IO_MODE     )   // RW
#define AUD_IO_CLKOE        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, IO_CLKOE    )   // RW
#define AUD_IO_DEPTH        _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, IO_DEPTH    )   // RW
#define AUD_TX_EN           _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, TX_EN       )   // RW
#define AUD_RX_EN           _bm(_AUD_CONT        , AUD_REG_BASE , 0x00, RX_EN       )   // RW

#define AUD_PWM_T           _am(_AUD_PWM         , AUD_REG_BASE , 0x04              )
#define AUD_PWM_CLKDIV      _bm(_AUD_PWM         , AUD_REG_BASE , 0x04, PWM_CLKDIV  )   // WR
#define AUD_RX_CLR          _bm(_AUD_PWM         , AUD_REG_BASE , 0x04, RX_CLR      )   // WO
#define AUD_TX_CLR          _bm(_AUD_PWM         , AUD_REG_BASE , 0x04, TX_CLR      )   // WO
#define AUD_WB_INCH         _bm(_AUD_PWM         , AUD_REG_BASE , 0x04, WB_INCH     )   // RW
#define AUD_WB_OUTCH        _bm(_AUD_PWM         , AUD_REG_BASE , 0x04, WB_OUTCH    )   // RW

//------------------------------------------------------------------------------
// Extern

typedef enum {
	e_AtxMusic,
	e_AtxEchoRaw,
	e_AtxEchoG711,
	e_AtxNetRXG711,
	e_AtxUser,
	e_AtxEvent,
	e_AtxSilent,
}AudOutputMode;

extern void Aud_RegShow(void);
extern void Aud_Init(BYTE CfgType);
extern void Aud_Isr(void *ctx);

//******************************************************************************
// 19. SPI
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT DAT            :32;    _rege_  _SPI_DAT;

_regs_  UINT EN             : 1;
        UINT _res0          : 7;
        UINT CLKDIV         : 8; // SCK = MCK / ((DIV+1)*2)
        UINT _res1          : 2;
        UINT BITMODE        : 1; // 0: MSB First  1: LSB First
        UINT CS_OEN         : 1; // 0: Enable     1: Disable
        UINT CS             : 1;
        UINT IRQ_EN         : 1;
        UINT IRQ_CLR        : 1;
        UINT IRQ            : 1;
        UINT ONEBIT         : 1; // 0: word mode  1: bit mode
        UINT CLKMODE        : 2; // Latch clock: 0: Low + PosEdge 1: Low + NegEdge 2: High + PosEdge 3: High + NegEdge
        UINT WS             : 2; // WordSize 0: 8bit 1: 16bit 2: 24bit 3: 32bit
        UINT RW             : 2; //          0: Read 1: Write 2: RW
        UINT GO             : 1;    _rege_  _SPI_CONT;

//------------------------------------------------------------------------------
// Addr/Bit macro(SPI0)
#define SPI0_RX_DAT         _am(_SPI_DAT         , SPI0_REG_BASE, 0x00              )   // RO

#define SPI0_TX_DAT         _am(_SPI_DAT         , SPI0_REG_BASE, 0x04              )   // RW

#define SPI0_SPI_CONT       _am(_SPI_CONT        , SPI0_REG_BASE, 0x08              )
#define SPI0_EN             _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, EN          )   // RW
#define SPI0_CLKDIV         _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, CLKDIV      )   // RW
#define SPI0_BITMODE        _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, BITMODE     )   // RW
#define SPI0_CS_OEN         _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, CS_OEN      )   // RW
#define SPI0_CS             _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, CS          )   // RW
#define SPI0_IRQ_EN         _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, IRQ_EN      )   // RW
#define SPI0_IRQ_CLR        _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, IRQ_CLR     )   // WO
#define SPI0_IRQ            _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, IRQ         )   // RO
#define SPI0_ONEBIT         _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, ONEBIT      )   // RW
#define SPI0_CLKMODE        _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, CLKMODE     )   // RW
#define SPI0_WS             _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, WS          )   // RW
#define SPI0_RW             _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, RW          )   // RW
#define SPI0_GO             _bm(_SPI_CONT        , SPI0_REG_BASE, 0x08, GO          )   // RW

//------------------------------------------------------------------------------
// Addr/Bit macro(SPI1)
#define SPI1_RX_DAT         _am(_SPI_DAT         , SPI1_REG_BASE, 0x00              )   // RO

#define SPI1_TX_DAT         _am(_SPI_DAT         , SPI1_REG_BASE, 0x04              )   // RW

#define SPI1_SPI_CONT       _am(_SPI_CONT        , SPI1_REG_BASE, 0x08              )
#define SPI1_EN             _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, EN          )   // RW
#define SPI1_CLKDIV         _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, CLKDIV      )   // RW
#define SPI1_BITMODE        _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, BITMODE     )   // RW
#define SPI1_CS_OEN         _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, CS_OEN      )   // RW
#define SPI1_CS             _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, CS          )   // RW
#define SPI1_IRQ_EN         _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, IRQ_EN      )   // RW
#define SPI1_IRQ_CLR        _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, IRQ_CLR     )   // WO
#define SPI1_IRQ            _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, IRQ         )   // RO
#define SPI1_ONEBIT         _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, ONEBIT      )   // RW
#define SPI1_CLKMODE        _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, CLKMODE     )   // RW
#define SPI1_WS             _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, WS          )   // RW
#define SPI1_RW             _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, RW          )   // RW
#define SPI1_GO             _bm(_SPI_CONT        , SPI1_REG_BASE, 0x08, GO          )   // RW

//------------------------------------------------------------------------------
// Addr/Bit macro(SPI2)
#define SPI2_RX_DAT         _am(_SPI_DAT         , SPI2_REG_BASE, 0x00              )   // RO

#define SPI2_TX_DAT         _am(_SPI_DAT         , SPI2_REG_BASE, 0x04              )   // RW

#define SPI2_SPI_CONT       _am(_SPI_CONT        , SPI2_REG_BASE, 0x08              )
#define SPI2_EN             _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, EN          )   // RW
#define SPI2_CLKDIV         _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, CLKDIV      )   // RW
#define SPI2_BITMODE        _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, BITMODE     )   // RW
#define SPI2_CS_OEN         _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, CS_OEN      )   // RW
#define SPI2_CS             _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, CS          )   // RW
#define SPI2_IRQ_EN         _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, IRQ_EN      )   // RW
#define SPI2_IRQ_CLR        _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, IRQ_CLR     )   // WO
#define SPI2_IRQ            _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, IRQ         )   // RO
#define SPI2_ONEBIT         _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, ONEBIT      )   // RW
#define SPI2_CLKMODE        _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, CLKMODE     )   // RW
#define SPI2_WS             _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, WS          )   // RW
#define SPI2_RW             _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, RW          )   // RW
#define SPI2_GO             _bm(_SPI_CONT        , SPI2_REG_BASE, 0x08, GO          )   // RW

//------------------------------------------------------------------------------
// Extern
extern void Spi0_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection);
extern void Spi0_Deinit(void);
extern void Spi0_Write(BYTE * dat);
extern void Spi0_Read(BYTE * dat);
extern void Spi0_RW(BYTE * WrDat, BYTE * RdDat);
extern void Spi0_CsLo(void);
extern void Spi0_CsHi(void);

extern void Spi1_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection);
extern void Spi1_Deinit(void);
extern void Spi1_Write(BYTE * dat);
extern void Spi1_Read(BYTE * dat);
extern void Spi1_RW(BYTE * WrDat, BYTE * RdDat);
extern void Spi1_CsLo(void);
extern void Spi1_CsHi(void);

extern void Spi2_Init(UINT Speed_Hz, UINT WordSize, UINT BitDirection);
extern void Spi2_Deinit(void);
extern void Spi2_Write(BYTE * dat);
extern void Spi2_Read(BYTE * dat);
extern void Spi2_RW(BYTE * WrDat, BYTE * RdDat);
extern void Spi2_CsLo(void);
extern void Spi2_CsHi(void);

//******************************************************************************
// 20. UART
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT CLKDIV         :12;
        UINT _res           : 6;
        UINT TX_TYPE        : 1;
        UINT STOP_BIT       : 1;
        UINT PARITY_EN      : 1;
        UINT PARITY_TYPE    : 1;
        UINT TX_IRQ         : 1;
        UINT TX_IRQ_EN      : 1;
        UINT TX_IRQ_CLR     : 1;
        UINT TX_EMPTY       : 1;
        UINT TX_FULL        : 1;
        UINT RX_IRQ         : 1;
        UINT RX_IRQ_EN      : 1;
        UINT RX_IRQ_CLR     : 1;
        UINT RX_EMPTY       : 1;
        UINT RX_FULL        : 1;    _rege_  _UART_CONT;

_regs_  UINT _res           :16;
        UINT TX_DAT         : 8;
        UINT RX_DAT         : 8;    _rege_  _UART_DAT;

//------------------------------------------------------------------------------
// Addr/Bit macro(UART0)
#define UART0_CLKDIV        _bm(_UART_CONT       , UART0_REG_BASE, 0x00, CLKDIV     )   // RW
#define UART0_TX_TYPE       _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_TYPE    )   // RW
#define UART0_STOP_BIT      _bm(_UART_CONT       , UART0_REG_BASE, 0x00, STOP_BIT   )   // RW
#define UART0_PARITY_EN     _bm(_UART_CONT       , UART0_REG_BASE, 0x00, PARITY_EN  )   // RW
#define UART0_PARITY_TYPE   _bm(_UART_CONT       , UART0_REG_BASE, 0x00, PARITY_TYPE)   // RW
#define UART0_TX_IRQ        _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_IRQ     )   // RO
#define UART0_TX_IRQ_EN     _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_IRQ_EN  )   // RW
#define UART0_TX_IRQ_CLR    _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_IRQ_CLR )   // WO
#define UART0_TX_EMPTY      _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_EMPTY   )   // RO
#define UART0_TX_FULL       _bm(_UART_CONT       , UART0_REG_BASE, 0x00, TX_FULL    )   // RO
#define UART0_RX_IRQ        _bm(_UART_CONT       , UART0_REG_BASE, 0x00, RX_IRQ     )   // RO
#define UART0_RX_IRQ_EN     _bm(_UART_CONT       , UART0_REG_BASE, 0x00, RX_IRQ_EN  )   // RW
#define UART0_RX_IRQ_CLR    _bm(_UART_CONT       , UART0_REG_BASE, 0x00, RX_IRQ_CLR )   // WO
#define UART0_RX_EMPTY      _bm(_UART_CONT       , UART0_REG_BASE, 0x00, RX_EMPTY   )   // RO
#define UART0_RX_FULL       _bm(_UART_CONT       , UART0_REG_BASE, 0x00, RX_FULL    )   // RO

#define UART0_TX_DAT        _bm(_UART_DAT        , UART0_REG_BASE, 0x04, TX_DAT     )   // WO
#define UART0_RX_DAT        _bm(_UART_DAT        , UART0_REG_BASE, 0x04, RX_DAT     )   // RO

//------------------------------------------------------------------------------
// Addr/Bit macro(UART1)
#define UART1_CLKDIV        _bm(_UART_CONT       , UART1_REG_BASE, 0x00, CLKDIV     )   // RW
#define UART1_TX_TYPE       _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_TYPE    )   // RW
#define UART1_STOP_BIT      _bm(_UART_CONT       , UART1_REG_BASE, 0x00, STOP_BIT   )   // RW
#define UART1_PARITY_EN     _bm(_UART_CONT       , UART1_REG_BASE, 0x00, PARITY_EN  )   // RW
#define UART1_PARITY_TYPE   _bm(_UART_CONT       , UART1_REG_BASE, 0x00, PARITY_TYPE)   // RW
#define UART1_TX_IRQ        _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_IRQ     )   // RO
#define UART1_TX_IRQ_EN     _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_IRQ_EN  )   // RW
#define UART1_TX_IRQ_CLR    _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_IRQ_CLR )   // WO
#define UART1_TX_EMPTY      _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_EMPTY   )   // RO
#define UART1_TX_FULL       _bm(_UART_CONT       , UART1_REG_BASE, 0x00, TX_FULL    )   // RO
#define UART1_RX_IRQ        _bm(_UART_CONT       , UART1_REG_BASE, 0x00, RX_IRQ     )   // RO
#define UART1_RX_IRQ_EN     _bm(_UART_CONT       , UART1_REG_BASE, 0x00, RX_IRQ_EN  )   // RW
#define UART1_RX_IRQ_CLR    _bm(_UART_CONT       , UART1_REG_BASE, 0x00, RX_IRQ_CLR )   // WO
#define UART1_RX_EMPTY      _bm(_UART_CONT       , UART1_REG_BASE, 0x00, RX_EMPTY   )   // RO
#define UART1_RX_FULL       _bm(_UART_CONT       , UART1_REG_BASE, 0x00, RX_FULL    )   // RO

#define UART1_TX_DAT        _bm(_UART_DAT        , UART1_REG_BASE, 0x04, TX_DAT     )   // WO
#define UART1_RX_DAT        _bm(_UART_DAT        , UART1_REG_BASE, 0x04, RX_DAT     )   // RO

//------------------------------------------------------------------------------
// Addr/Bit macro(UART2)
#define UART2_CLKDIV        _bm(_UART_CONT       , UART2_REG_BASE, 0x00, CLKDIV     )   // RW
#define UART2_TX_TYPE       _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_TYPE    )   // RW
#define UART2_STOP_BIT      _bm(_UART_CONT       , UART2_REG_BASE, 0x00, STOP_BIT   )   // RW
#define UART2_PARITY_EN     _bm(_UART_CONT       , UART2_REG_BASE, 0x00, PARITY_EN  )   // RW
#define UART2_PARITY_TYPE   _bm(_UART_CONT       , UART2_REG_BASE, 0x00, PARITY_TYPE)   // RW
#define UART2_TX_IRQ        _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_IRQ     )   // RO
#define UART2_TX_IRQ_EN     _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_IRQ_EN  )   // RW
#define UART2_TX_IRQ_CLR    _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_IRQ_CLR )   // WO
#define UART2_TX_EMPTY      _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_EMPTY   )   // RO
#define UART2_TX_FULL       _bm(_UART_CONT       , UART2_REG_BASE, 0x00, TX_FULL    )   // RO
#define UART2_RX_IRQ        _bm(_UART_CONT       , UART2_REG_BASE, 0x00, RX_IRQ     )   // RO
#define UART2_RX_IRQ_EN     _bm(_UART_CONT       , UART2_REG_BASE, 0x00, RX_IRQ_EN  )   // RW
#define UART2_RX_IRQ_CLR    _bm(_UART_CONT       , UART2_REG_BASE, 0x00, RX_IRQ_CLR )   // WO
#define UART2_RX_EMPTY      _bm(_UART_CONT       , UART2_REG_BASE, 0x00, RX_EMPTY   )   // RO
#define UART2_RX_FULL       _bm(_UART_CONT       , UART2_REG_BASE, 0x00, RX_FULL    )   // RO

#define UART2_TX_DAT        _bm(_UART_DAT        , UART2_REG_BASE, 0x04, TX_DAT     )   // WO
#define UART2_RX_DAT        _bm(_UART_DAT        , UART2_REG_BASE, 0x04, RX_DAT     )   // RO

//------------------------------------------------------------------------------
// Extern
enum {
	eUARTspeed115200 = 115200,
	eUARTspeed57600 = 57600,
	eUARTspeed38400 = 38400,
	eUARTspeed19200 = 19200,
	eUARTspeed14400 = 14400,
	eUARTspeed9600 = 9600,
	eUARTspeed4800 = 4800,
	eUARTspeed2400 = 2400,
	eUARTspeed1200 = 1200,
};

extern char gUartNumcode[];

extern void Uart0_Init(UINT Speed_bps);
extern void Uart0_Deinit(void);
#ifndef _INLINE
extern void Uart0_Tx(char dat);
extern void Uart0_Rx(char *dat);
extern BYTE Uart0_RxExist(void);
extern BYTE Uart0_GetByte(void);
#endif
extern void Uart0_Str(const char *text);
extern void Uart0_Byte(BYTE data);
extern void Uart0_Hex(int data);
extern void Uart0_Dec(unsigned char data);
extern void Uart0_Printf(char * text, int dat);
#define	Uart0_Num(n)	Uart0_Tx(gUartNumcode[(n)])


extern void Uart1_Init(UINT Speed_bps);
extern void Uart1_Deinit(void);
#ifndef _INLINE
extern void Uart1_Tx(char dat);
extern void Uart1_Rx(char *dat);
extern BYTE Uart1_RxExist(void);
extern BYTE Uart1_GetByte(void);
#endif
extern void Uart1_Str(const char *text);
extern void Uart1_Hex(int data);
#define	Uart1_Num(n)	Uart1_Tx(gUartNumcode[(n)])

extern void Uart2_Init(UINT Speed_bps);
extern void Uart2_Deinit(void);
#ifndef _INLINE
extern void Uart2_Rx(char *dat);
extern void Uart2_Tx(char dat);
extern BYTE Uart2_RxExist(void);
extern BYTE Uart2_GetByte(void);
#endif

//******************************************************************************
// 21. DEVICE TEST
//------------------------------------------------------------------------------

//******************************************************************************
// 22. ISP
//------------------------------------------------------------------------------

//******************************************************************************
// 23. PAD
//------------------------------------------------------------------------------
// Bit field
_regs_  UINT _res           :19;
        UINT DDR_A0_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A1_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A2_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A3_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A4_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A5_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A6_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A7_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A8_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A9_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_A10_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_A11_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_A12_DS     : 1;    _rege_  _PAD_DDRDS0;		// (0):8mA, 1:6mA

_regs_  UINT _res           :24;
        UINT DDR_CK_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_CKE_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_CS_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_RAS_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_CAS_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_WE_DS      : 1;								// (0):8mA, 1:6mA
        UINT DDR_BA0_DS     : 1;								// (0):8mA, 1:6mA
        UINT DDR_BA1_DS     : 1;    _rege_  _PAD_DDRDS1;		// (0):8mA, 1:6mA

_regs_  UINT _res0          : 3;
        UINT DDR_DQS0_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res1          : 3;
        UINT DDR_DQS1_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res2          : 3;
        UINT DDR_DQS2_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res3          : 3;
        UINT DDR_DQS3_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res4          : 3;
        UINT DDR_DM0_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res5          : 3;
        UINT DDR_DM1_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res6          : 3;
        UINT DDR_DM2_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res7          : 3;
        UINT DDR_DM3_DS     : 1;    _rege_  _PAD_DDRDS2;		// (0):8mA, 1:6mA

_regs_  UINT _res0          : 3;
        UINT DDR_DQ0_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res1          : 3;
        UINT DDR_DQ1_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res2          : 3;
        UINT DDR_DQ2_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res3          : 3;
        UINT DDR_DQ3_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res4          : 3;
        UINT DDR_DQ4_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res5          : 3;
        UINT DDR_DQ5_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res6          : 3;
        UINT DDR_DQ6_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res7          : 3;
        UINT DDR_DQ7_DS     : 1;    _rege_  _PAD_DDRDS3;		// (0):8mA, 1:6mA

_regs_  UINT _res0          : 3;
        UINT DDR_DQ8_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res1          : 3;
        UINT DDR_DQ9_DS     : 1;								// (0):8mA, 1:6mA
        UINT _res2          : 3;
        UINT DDR_DQ10_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res3          : 3;
        UINT DDR_DQ11_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res4          : 3;
        UINT DDR_DQ12_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res5          : 3;
        UINT DDR_DQ13_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res6          : 3;
        UINT DDR_DQ14_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res7          : 3;
        UINT DDR_DQ15_DS    : 1;    _rege_  _PAD_DDRDS4;		// (0):8mA, 1:6mA

_regs_  UINT _res0          : 3;
        UINT DDR_DQ16_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res1          : 3;
        UINT DDR_DQ17_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res2          : 3;
        UINT DDR_DQ18_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res3          : 3;
        UINT DDR_DQ19_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res4          : 3;
        UINT DDR_DQ20_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res5          : 3;
        UINT DDR_DQ21_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res6          : 3;
        UINT DDR_DQ22_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res7          : 3;
        UINT DDR_DQ23_DS    : 1;    _rege_  _PAD_DDRDS5;		// (0):8mA, 1:6mA

_regs_  UINT _res0          : 3;
        UINT DDR_DQ24_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res1          : 3;
        UINT DDR_DQ25_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res2          : 3;
        UINT DDR_DQ26_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res3          : 3;
        UINT DDR_DQ27_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res4          : 3;
        UINT DDR_DQ28_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res5          : 3;
        UINT DDR_DQ29_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res6          : 3;
        UINT DDR_DQ30_DS    : 1;								// (0):8mA, 1:6mA
        UINT _res7          : 3;
        UINT DDR_DQ31_DS    : 1;    _rege_  _PAD_DDRDS6;		// (0):8mA, 1:6mA

_regs_  UINT SS_VS_PUEN     : 1;								// 0:on, (1):off
        UINT SS_HS_PUEN     : 1;								// 0:on, (1):off
        UINT SS_CKI_PDEN    : 1;								// 0:on, (1):off
        UINT SS_CKO_PDEN    : 1;								// 0:on, (1):off
        UINT _res           : 8;
        UINT SS_DIN_PDEN    :20;    _rege_  _PAD_SSPullEN;		// 0:on, (1):off

_regs_  UINT I2S_DIN_PDEN   : 1;								// 0:on, (1):off
        UINT I2S_DOUT_PDEN  : 1;								// 0:on, (1):off
        UINT I2S_WCK_PDEN   : 1;								// 0:on, (1):off
        UINT I2S_BCK_PDEN   : 1;								// 0:on, (1):off
        UINT I2C0_SDA_PUEN  : 1;								// 0:on, (1):off
        UINT I2C0_SCL_PUEN  : 1;								// 0:on, (1):off
        UINT _res0          : 2;
        UINT SPI0_CS_PDEN   : 1;								// 0:on, (1):off
        UINT SPI0_MISO_PDEN : 1;								// 0:on, (1):off
        UINT SPI0_MOSI_PDEN : 1;								// 0:on, (1):off
        UINT SPI0_SCK_PDEN  : 1;								// 0:on, (1):off
        UINT _res1          : 4;
        UINT UART0_TX_PUEN  : 1;								// 0:on, (1):off
        UINT _res2          : 1;
        UINT JTAG_TDO_PDEN  : 1;								// 0:on, (1):off
        UINT _res3          :13;    _rege_  _PAD_PeriPullEN;

_regs_  UINT SDIO0_MSEL     : 6;								// (0):3.3v, 0x3F:1.8v
        UINT _res0          : 1;
        UINT SDIO0_DET_PUEN : 1;								// 0:on, (1):off
        UINT _res1          : 1;
        UINT SDIO0_CMD_PU   : 1;								// 0:on, (1):off
        UINT _res2          : 4;
        UINT SDIO0_CMD_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT _res3          : 1;
        UINT SDIO0_CLK_PU   : 1;								// 0:on, (1):off
        UINT _res4          : 4;
        UINT SDIO0_CLK_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT _res5          : 1;
        UINT SDIO0_DAT_PU   : 1;								// 0:on, (1):off
        UINT _res6          : 4;
        UINT SDIO0_DAT_DS   : 2;    _rege_  _PAD_SDIO0;			// (0):4mA, 1,2:8mA, 3:12mA

_regs_  UINT SDIO1_MSEL     : 6;								// (0):3.3v, 0x3F:1.8v
        UINT _res0          : 1;
        UINT SDIO1_DET_PUEN : 1;								// 0:on, (1):off
        UINT _res1          : 1;
        UINT SDIO1_CMD_PU   : 1;								// 0:on, (1):off
        UINT _res2          : 4;
        UINT SDIO1_CMD_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT _res3          : 1;
        UINT SDIO1_CLK_PU   : 1;								// 0:on, (1):off
        UINT _res4          : 4;
        UINT SDIO1_CLK_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT _res5          : 1;
        UINT SDIO1_DAT_PU   : 1;								// 0:on, (1):off
        UINT _res6          : 4;
        UINT SDIO1_DAT_DS   : 2;    _rege_  _PAD_SDIO1;			// (0):4mA, 1,2:8mA, 3:12mA

_regs_  UINT ETH_RXD_PDEN   : 4;								// 0:on, (0xF):off
        UINT ETH_TXD_PDEN   : 4;								// 0:on, (0xF):off
        UINT ETH_RCK_PDEN   : 1;								// 0:on, (1):off
        UINT ETH_RXDV_PDEN  : 1;								// 0:on, (1):off
        UINT ETH_RXER_PDEN  : 1;								// 0:on, (1):off
        UINT ETH_COL_PDEN   : 1;								// 0:on, (1):off
        UINT ETH_CRS_PDEN   : 1;								// 0:on, (1):off
        UINT ETH_MDIO_PUEN  : 1;								// 0:on, (1):off
        UINT ETH_MDC_PDEN   : 1;								// 0:on, (1):off
        UINT ETH_TCK_PDEN   : 1;								// 0:on, (1):off
        UINT ETH_TXEN_PDEN  : 1;								// 0:on, (1):off
        UINT SFLS_CLK_PDEN  : 1;								// 0:on, (1):off
        UINT SFLS_CS_PDEN   : 1;								// 0:on, (1):off
        UINT SFLS_DQ_PDEN   : 4;								// 0:on, (0xF):off
        UINT _res           : 9;    _rege_  _PAD_ETHPullEN;

_regs_  UINT _res           : 8;
        UINT GPIO_PUEN      :24;    _rege_  _PAD_GPIOPullEN;	// 0:on, (1):off

_regs_  UINT I2S_DOUT_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT I2S_WCK_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT I2S_BCK_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT HS_DS          : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT VS_DS          : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SS_CKO_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_CS_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_DQ0_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_DQ1_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_DQ2_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_DQ3_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SFLS_CLK_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT UART0_TX_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SPI0_MOSI_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SPI0_MISO_DS   : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT SPI0_SCK_DS    : 2;    _rege_  _PAD_PeriDS0;		// (0):4mA, 1,2:8mA, 3:12mA

_regs_  UINT SPI0_CS_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT I2C0_SDA_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT I2C0_SCL_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TCK_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TXD3_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TXD2_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TXD1_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TXD0_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_TXEN_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_MDC_DS     : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT ETH_MDIO_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO0_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO1_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO2_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO3_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO4_DS       : 2;    _rege_  _PAD_PeriDS1;		// (0):4mA, 1,2:8mA, 3:12mA

_regs_  UINT GPIO5_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO6_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO7_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO8_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO9_DS       : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO10_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO11_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO12_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO13_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO14_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO15_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO16_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO17_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO18_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO19_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO20_DS      : 2;    _rege_  _PAD_PeriDS2;		// (0):4mA, 1,2:8mA, 3:12mA

_regs_  UINT GPIO21_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO22_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT GPIO23_DS      : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT JTAG_TDO_DS    : 2;								// (0):4mA, 1,2:8mA, 3:12mA
        UINT _res           :24;    _rege_  _PAD_PeriDS3;

//------------------------------------------------------------------------------
// Addr/Bit macro
#define PAD_DDR_A0_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A0_DS   )   // RW
#define PAD_DDR_A1_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A1_DS   )   // RW
#define PAD_DDR_A2_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A2_DS   )   // RW
#define PAD_DDR_A3_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A3_DS   )   // RW
#define PAD_DDR_A4_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A4_DS   )   // RW
#define PAD_DDR_A5_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A5_DS   )   // RW
#define PAD_DDR_A6_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A6_DS   )   // RW
#define PAD_DDR_A7_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A7_DS   )   // RW
#define PAD_DDR_A8_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A8_DS   )   // RW
#define PAD_DDR_A9_DS       _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A9_DS   )   // RW
#define PAD_DDR_A10_DS      _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A10_DS  )   // RW
#define PAD_DDR_A11_DS      _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A11_DS  )   // RW
#define PAD_DDR_A12_DS      _bm(_PAD_DDRDS0      , PAD_REG_BASE , 0x00, DDR_A12_DS  )   // RW

#define PAD_DDR_CK_DS       _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_CK_DS   )   // RW
#define PAD_DDR_CKE_DS      _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_CKE_DS  )   // RW
#define PAD_DDR_CS_DS       _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_CS_DS   )   // RW
#define PAD_DDR_RAS_DS      _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_RAS_DS  )   // RW
#define PAD_DDR_CAS_DS      _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_CAS_DS  )   // RW
#define PAD_DDR_WE_DS       _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_WE_DS   )   // RW
#define PAD_DDR_BA0_DS      _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_BA0_DS  )   // RW
#define PAD_DDR_BA1_DS      _bm(_PAD_DDRDS1      , PAD_REG_BASE , 0x04, DDR_BA1_DS  )   // RW

#define PAD_DDR_DQS0_DS     _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DQS0_DS )   // RW
#define PAD_DDR_DQS1_DS     _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DQS1_DS )   // RW
#define PAD_DDR_DQS2_DS     _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DQS2_DS )   // RW
#define PAD_DDR_DQS3_DS     _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DQS3_DS )   // RW
#define PAD_DDR_DM0_DS      _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DM0_DS  )   // RW
#define PAD_DDR_DM1_DS      _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DM1_DS  )   // RW
#define PAD_DDR_DM2_DS      _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DM2_DS  )   // RW
#define PAD_DDR_DM3_DS      _bm(_PAD_DDRDS2      , PAD_REG_BASE , 0x08, DDR_DM3_DS  )   // RW

#define PAD_DDR_DQ0_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ0_DS  )   // RW
#define PAD_DDR_DQ1_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ1_DS  )   // RW
#define PAD_DDR_DQ2_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ2_DS  )   // RW
#define PAD_DDR_DQ3_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ3_DS  )   // RW
#define PAD_DDR_DQ4_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ4_DS  )   // RW
#define PAD_DDR_DQ5_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ5_DS  )   // RW
#define PAD_DDR_DQ6_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ6_DS  )   // RW
#define PAD_DDR_DQ7_DS      _bm(_PAD_DDRDS3      , PAD_REG_BASE , 0x0C, DDR_DQ7_DS  )   // RW

#define PAD_DDR_DQ8_DS      _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ8_DS  )   // RW
#define PAD_DDR_DQ9_DS      _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ9_DS  )   // RW
#define PAD_DDR_DQ10_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ10_DS )   // RW
#define PAD_DDR_DQ11_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ11_DS )   // RW
#define PAD_DDR_DQ12_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ12_DS )   // RW
#define PAD_DDR_DQ13_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ13_DS )   // RW
#define PAD_DDR_DQ14_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ14_DS )   // RW
#define PAD_DDR_DQ15_DS     _bm(_PAD_DDRDS4      , PAD_REG_BASE , 0x10, DDR_DQ15_DS )   // RW

#define PAD_DDR_DQ16_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ16_DS )   // RW
#define PAD_DDR_DQ17_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ17_DS )   // RW
#define PAD_DDR_DQ18_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ18_DS )   // RW
#define PAD_DDR_DQ19_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ19_DS )   // RW
#define PAD_DDR_DQ20_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ20_DS )   // RW
#define PAD_DDR_DQ21_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ21_DS )   // RW
#define PAD_DDR_DQ22_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ22_DS )   // RW
#define PAD_DDR_DQ23_DS     _bm(_PAD_DDRDS5      , PAD_REG_BASE , 0x14, DDR_DQ23_DS )   // RW

#define PAD_DDR_DQ24_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ24_DS )   // RW
#define PAD_DDR_DQ25_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ25_DS )   // RW
#define PAD_DDR_DQ26_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ26_DS )   // RW
#define PAD_DDR_DQ27_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ27_DS )   // RW
#define PAD_DDR_DQ28_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ28_DS )   // RW
#define PAD_DDR_DQ29_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ29_DS )   // RW
#define PAD_DDR_DQ30_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ30_DS )   // RW
#define PAD_DDR_DQ31_DS     _bm(_PAD_DDRDS6      , PAD_REG_BASE , 0x18, DDR_DQ31_DS )   // RW

#define PAD_SS_VS_PUEN      _bm(_PAD_SSPullEN    , PAD_REG_BASE , 0x1C, SS_VS_PUEN  )   // RW
#define PAD_SS_HS_PUEN      _bm(_PAD_SSPullEN    , PAD_REG_BASE , 0x1C, SS_HS_PUEN  )   // RW
#define PAD_SS_CKI_PDEN     _bm(_PAD_SSPullEN    , PAD_REG_BASE , 0x1C, SS_CKI_PDEN )   // RW
#define PAD_SS_CKO_PDEN     _bm(_PAD_SSPullEN    , PAD_REG_BASE , 0x1C, SS_CKO_PDEN )   // RW
#define PAD_SS_DIN_PDEN     _bm(_PAD_SSPullEN    , PAD_REG_BASE , 0x1C, SS_DIN_PDEN )   // RW

#define PAD_I2S_DIN_PDEN    _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2S_DIN_PDEN)   // RW
#define PAD_I2S_DOUT_PDEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2S_DOUT_PDEN)  // RW
#define PAD_I2S_WCK_PDEN    _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2S_WCK_PDEN)   // RW
#define PAD_I2S_BCK_PDEN    _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2S_BCK_PDEN)   // RW
#define PAD_I2C0_SDA_PUEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2C0_SDA_PUEN)  // RW
#define PAD_I2C0_SCL_PUEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, I2C0_SCL_PUEN)  // RW
#define PAD_SPI0_CS_PDEN    _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, SPI0_CS_PDEN)   // RW
#define PAD_SPI0_MISO_PDEN  _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, SPI0_MISO_PDEN) // RW
#define PAD_SPI0_MOSI_PDEN  _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, SPI0_MOSI_PDEN) // RW
#define PAD_SPI0_SCK_PDEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, SPI0_SCK_PDEN)  // RW
#define PAD_UART0_TX_PUEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, UART0_TX_PUEN)  // RW
#define PAD_JTAG_TDO_PDEN   _bm(_PAD_PeriPullEN  , PAD_REG_BASE , 0x20, JTAG_TDO_PDEN)  // RW
#if SDIO_CH_SWAP
#define PAD_SDIO0_MSEL      _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_MSEL  )   // RW
#define PAD_SDIO0_DET_PUEN  _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_DET_PUEN) // RW
#define PAD_SDIO0_CMD_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_CMD_PU)   // RW
#define PAD_SDIO0_CMD_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_CMD_DS)   // RW
#define PAD_SDIO0_CLK_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_CLK_PU)   // RW
#define PAD_SDIO0_CLK_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_CLK_DS)   // RW
#define PAD_SDIO0_DAT_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_DAT_PU)   // RW
#define PAD_SDIO0_DAT_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x28, SDIO0_DAT_DS)   // RW
#define PAD_SDIO0_T         _am(_PAD_SDIO0       , PAD_REG_BASE , 0x28              )   // RW

#define PAD_SDIO1_MSEL      _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_MSEL  )   // RW
#define PAD_SDIO1_DET_PUEN  _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_DET_PUEN) // RW
#define PAD_SDIO1_CMD_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_CMD_PU)   // RW
#define PAD_SDIO1_CMD_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_CMD_DS)   // RW
#define PAD_SDIO1_CLK_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_CLK_PU)   // RW
#define PAD_SDIO1_CLK_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_CLK_DS)   // RW
#define PAD_SDIO1_DAT_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_DAT_PU)   // RW
#define PAD_SDIO1_DAT_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x24, SDIO1_DAT_DS)   // RW
#define PAD_SDIO1_T         _am(_PAD_SDIO1       , PAD_REG_BASE , 0x24              )   // RW
#else
#define PAD_SDIO0_MSEL      _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_MSEL  )   // RW
#define PAD_SDIO0_DET_PUEN  _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_DET_PUEN) // RW
#define PAD_SDIO0_CMD_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_CMD_PU)   // RW
#define PAD_SDIO0_CMD_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_CMD_DS)   // RW
#define PAD_SDIO0_CLK_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_CLK_PU)   // RW
#define PAD_SDIO0_CLK_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_CLK_DS)   // RW
#define PAD_SDIO0_DAT_PU    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_DAT_PU)   // RW
#define PAD_SDIO0_DAT_DS    _bm(_PAD_SDIO0       , PAD_REG_BASE , 0x24, SDIO0_DAT_DS)   // RW
#define PAD_SDIO0_T         _am(_PAD_SDIO0       , PAD_REG_BASE , 0x24              )   // RW

#define PAD_SDIO1_MSEL      _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_MSEL  )   // RW
#define PAD_SDIO1_DET_PUEN  _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_DET_PUEN) // RW
#define PAD_SDIO1_CMD_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_CMD_PU)   // RW
#define PAD_SDIO1_CMD_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_CMD_DS)   // RW
#define PAD_SDIO1_CLK_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_CLK_PU)   // RW
#define PAD_SDIO1_CLK_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_CLK_DS)   // RW
#define PAD_SDIO1_DAT_PU    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_DAT_PU)   // RW
#define PAD_SDIO1_DAT_DS    _bm(_PAD_SDIO1       , PAD_REG_BASE , 0x28, SDIO1_DAT_DS)   // RW
#define PAD_SDIO1_T         _am(_PAD_SDIO1       , PAD_REG_BASE , 0x28              )   // RW
#endif
#define PAD_ETH_RXD_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_RXD_PDEN)   // RW
#define PAD_ETH_TXD_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_TXD_PDEN)   // RW
#define PAD_ETH_RCK_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_RCK_PDEN)   // RW
#define PAD_ETH_RXDV_PDEN   _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_RXDV_PDEN)  // RW
#define PAD_ETH_RXER_PDEN   _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_RXER_PDEN)  // RW
#define PAD_ETH_COL_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_COL_PDEN)   // RW
#define PAD_ETH_CRS_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_CRS_PDEN)   // RW
#define PAD_ETH_MDIO_PUEN   _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_MDIO_PUEN)  // RW
#define PAD_ETH_MDC_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_MDC_PDEN)   // RW
#define PAD_ETH_TCK_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_TCK_PDEN)   // RW
#define PAD_ETH_TXEN_PDEN   _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, ETH_TXEN_PDEN)  // RW
#define PAD_SFLS_CLK_PDEN   _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, SFLS_CLK_PDEN)  // RW
#define PAD_SFLS_CS_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, SFLS_CS_PDEN)   // RW
#define PAD_SFLS_DQ_PDEN    _bm(_PAD_ETHPullEN   , PAD_REG_BASE , 0x2C, SFLS_DQ_PDEN)   // RW

#define PAD_GPIO_PUEN       _bm(_PAD_GPIOPullEN  , PAD_REG_BASE , 0x30, GPIO_PUEN   )   // RW

#define PAD_I2S_DOUT_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, I2S_DOUT_DS )   // RW
#define PAD_I2S_WCK_DS      _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, I2S_WCK_DS  )   // RW
#define PAD_I2S_BCK_DS      _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, I2S_BCK_DS  )   // RW
#define PAD_HS_DS           _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, HS_DS       )   // RW
#define PAD_VS_DS           _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, VS_DS       )   // RW
#define PAD_SS_CKO_DS       _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SS_CKO_DS   )   // RW
#define PAD_SFLS_CS_DS      _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_CS_DS  )   // RW
#define PAD_SFLS_DQ0_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_DQ0_DS )   // RW
#define PAD_SFLS_DQ1_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_DQ1_DS )   // RW
#define PAD_SFLS_DQ2_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_DQ2_DS )   // RW
#define PAD_SFLS_DQ3_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_DQ3_DS )   // RW
#define PAD_SFLS_CLK_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SFLS_CLK_DS )   // RW
#define PAD_UART0_TX_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, UART0_TX_DS )   // RW
#define PAD_SPI0_MOSI_DS    _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SPI0_MOSI_DS)   // RW
#define PAD_SPI0_MISO_DS    _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SPI0_MISO_DS)   // RW
#define PAD_SPI0_SCK_DS     _bm(_PAD_PeriDS0     , PAD_REG_BASE , 0x34, SPI0_SCK_DS )   // RW

#define PAD_SPI0_CS_DS      _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, SPI0_CS_DS  )   // RW
#define PAD_I2C0_SDA_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, I2C0_SDA_DS )   // RW
#define PAD_I2C0_SCL_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, I2C0_SCL_DS )   // RW
#define PAD_ETH_TCK_DS      _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TCK_DS  )   // RW
#define PAD_ETH_TXD3_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TXD3_DS )   // RW
#define PAD_ETH_TXD2_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TXD2_DS )   // RW
#define PAD_ETH_TXD1_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TXD1_DS )   // RW
#define PAD_ETH_TXD0_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TXD0_DS )   // RW
#define PAD_ETH_TXEN_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_TXEN_DS )   // RW
#define PAD_ETH_MDC_DS      _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_MDC_DS  )   // RW
#define PAD_ETH_MDIO_DS     _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, ETH_MDIO_DS )   // RW
#define PAD_GPIO0_DS        _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, GPIO0_DS    )   // RW
#define PAD_GPIO1_DS        _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, GPIO1_DS    )   // RW
#define PAD_GPIO2_DS        _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, GPIO2_DS    )   // RW
#define PAD_GPIO3_DS        _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, GPIO3_DS    )   // RW
#define PAD_GPIO4_DS        _bm(_PAD_PeriDS1     , PAD_REG_BASE , 0x38, GPIO4_DS    )   // RW

#define PAD_GPIO5_DS        _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO5_DS    )   // RW
#define PAD_GPIO6_DS        _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO6_DS    )   // RW
#define PAD_GPIO7_DS        _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO7_DS    )   // RW
#define PAD_GPIO8_DS        _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO8_DS    )   // RW
#define PAD_GPIO9_DS        _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO9_DS    )   // RW
#define PAD_GPIO10_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO10_DS   )   // RW
#define PAD_GPIO11_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO11_DS   )   // RW
#define PAD_GPIO12_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO12_DS   )   // RW
#define PAD_GPIO13_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO13_DS   )   // RW
#define PAD_GPIO14_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO14_DS   )   // RW
#define PAD_GPIO15_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO15_DS   )   // RW
#define PAD_GPIO16_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO16_DS   )   // RW
#define PAD_GPIO17_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO17_DS   )   // RW
#define PAD_GPIO18_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO18_DS   )   // RW
#define PAD_GPIO19_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO19_DS   )   // RW
#define PAD_GPIO20_DS       _bm(_PAD_PeriDS2     , PAD_REG_BASE , 0x3C, GPIO20_DS   )   // RW

#define PAD_GPIO21_DS       _bm(_PAD_PeriDS3     , PAD_REG_BASE , 0x40, GPIO21_DS   )   // RW
#define PAD_GPIO22_DS       _bm(_PAD_PeriDS3     , PAD_REG_BASE , 0x40, GPIO22_DS   )   // RW
#define PAD_GPIO23_DS       _bm(_PAD_PeriDS3     , PAD_REG_BASE , 0x40, GPIO23_DS   )   // RW
#define PAD_JTAG_TDO_DS     _bm(_PAD_PeriDS3     , PAD_REG_BASE , 0x40, JTAG_TDO_DS )   // RW

//------------------------------------------------------------------------------
// Extern


#endif//_DEV_REGS_H_
