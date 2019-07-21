//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_inline.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:			This makes execution faster by reducing function-call overhead
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151020		ygkim		first designed
// -----------------------------------------------------------------------------
#ifdef _INLINE
//#ifndef	_EXR1K_INLINE_H_		
//#define	_EXR1K_INLINE_H_


#include "exr1k.h"
#include "exr1k_sprs.h"

//------------------------------------------------------------------------------
// If a compiler does not support explicit inlining, this macro should be defined to be `static'.
#undef INLINE

//#define INLINE __inline__
//#define INLINE extern inline
//#define INLINE static
//#define INLINE static inline
#define INLINE _NOINS static inline


//******************************************************************************
// MxSpr
//------------------------------------------------------------------------------
INLINE void mtspr(uint spr, uint value)
{
	asm volatile("l.mtspr %0,%1,0" : : "r" (spr), "r" (value));
}

INLINE uint mfspr(uint spr)
{
	uint value;
	asm volatile("l.mfspr %0,%1,0" : "=r" (value) : "r" (spr));
	return value;
}


//******************************************************************************
// Mxgpr
//------------------------------------------------------------------------------
INLINE uint mfgpr1(void)
{
	uint value;
	asm("l.or %0,r0,r1" : "=r" (value) : );
	return value;
}


//******************************************************************************
// Cache
//------------------------------------------------------------------------------
//void disable_icache(void);
//void disable_dcache(void);
//
//int check_dcache(void);
//int get_dcache_block_size(void);
//void invalidate_dcache_range(uint sadr, uint eadr);


//******************************************************************************
// JTAG debugger
//------------------------------------------------------------------------------
//INLINE void enable_jtag_ctrl_exception(void);
//INLINE void disable_jtag_ctrl_exception(void);


//******************************************************************************
// HTB: Hardware Trace Buffer
//------------------------------------------------------------------------------
INLINE void enable_htb(void)
{
	mtspr(EXR1K_HTBCR, 	EXR1K_HTBCR_EN 	|	// HTB enable
						EXR1K_HTBCR_NE	|	// NOP enable
						EXR1K_HTBCR_TE	);	// Timestamp enable: Enable Free running counter
	
	//mtspr(EXR1K_HTBCR, EXR1K_HTBCR_TS_IF|EXR1K_HTBCR_TE|EXR1K_HTBCR_NE|EXR1K_HTBCR_EN);	// TS=IF, DS2=npc
	//mtspr(EXR1K_HTBCR, EXR1K_HTBCR_DS2_MDATI|EXR1K_HTBCR_TS_IF|EXR1K_HTBCR_TE|EXR1K_HTBCR_NE|EXR1K_HTBCR_EN);	// TS=IF, DS2=MDATI
	
	//TRACE("TBCR: %x \r\n",mfspr(EXR1K_HTBCR));
}

INLINE void disable_htb(void)
{
	mtspr(EXR1K_HTBCR, mfspr(EXR1K_HTBCR) & ~EXR1K_HTBCR_EN);
}

INLINE uint htb_enabled(void)
{
	return mfspr(EXR1K_HTBCR) & EXR1K_HTBCR_EN;
}


//#endif//_EXR1K_INLINE_H_
#endif//_INLINE
