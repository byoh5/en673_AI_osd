//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_sprs.c
//	Description:	EXR1K SPRs
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150724		ygkim		first designed
// -----------------------------------------------------------------------------
#include "dev.h"			// to use printf
#include "dev_inline.h"	// to use fast code

//******************************************************************************
// MxSpr
//------------------------------------------------------------------------------
#ifndef _INLINE
_NOINS void mtspr(uint spr, uint value)
{
	asm("l.mtspr %0,%1,0" : : "r" (spr), "r" (value));
}

_NOINS uint mfspr(uint spr)
{
	uint value;
	asm("l.mfspr %0,%1,0" : "=r" (value) : "r" (spr));
	return value;
}
#endif

//------------------------------------------------------------------------------
// Enable/Disable: Use only when code starts
_NOINS uint ext_int_enabled(void)	{	return mfspr(EXR1K_SR) & EXR1K_SR_IEE; 	}
_NOINS void enable_ext_int(void)	{ 	mtspr(EXR1K_SR, mfspr(EXR1K_SR)| EXR1K_SR_IEE); }
_NOINS void disable_ext_int(void)	{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)&~EXR1K_SR_IEE);	}

//------------------------------------------------------------------------------
_NOINS uint timer_enabled(void)		{	return mfspr(EXR1K_SR) & EXR1K_SR_TEE; 	}
_NOINS void enable_timer(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)| EXR1K_SR_TEE); } 	
_NOINS void disable_timer(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)&~EXR1K_SR_TEE);	}

//------------------------------------------------------------------------------
_NOINS uint xsr_enabled(void)		{	return mfspr(EXR1K_SR) & (EXR1K_SR_IEE|EXR1K_SR_TEE); }
_NOINS void enable_xsr(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)| (EXR1K_SR_IEE|EXR1K_SR_TEE));	}
_NOINS void disable_xsr(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)&~(EXR1K_SR_IEE|EXR1K_SR_TEE));	}

//------------------------------------------------------------------------------
_NOINS void enable_immu(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)| EXR1K_SR_IME);	}
_NOINS void disable_immu(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)&~EXR1K_SR_IME);	}
                                	
_NOINS void enable_dmmu(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)| EXR1K_SR_DME);	}
_NOINS void disable_dmmu(void)		{	mtspr(EXR1K_SR, mfspr(EXR1K_SR)&~EXR1K_SR_DME);	}

//------------------------------------------------------------------------------
// FP: Clear flags and enable exception
void enable_fp_xsr(void) 
{
  	uint value = mfspr(EXR1K_FPCSR) & ~EXR1K_FPCSR_ALLF;	// Clear flags in FPCSR
  	mtspr(EXR1K_FPCSR, value|EXR1K_FPCSR_FPEE);				// Re-enable floating point exceptions
}
  
void set_fp_round_mode(uint mode)
{
  	uint value = mfspr(EXR1K_FPCSR);	// Read the SPR
  	value &= ~EXR1K_FPCSR_RM;
	mtspr(EXR1K_FPCSR, value|mode);
}

uint get_fp_round_mode(void)
{
	return mfspr(EXR1K_FPCSR) & EXR1K_FPCSR_RM;
}

// Sets the rounding precision of subsequent extended double-precision operations.
// 	32: single precision, 64: double precision, 80: full extended double precision
void set_fp_round_precision(uint prec)	
{
  	UNUSED(prec);
}



//******************************************************************************
// Mxgpr
//------------------------------------------------------------------------------
#ifndef _INLINE
_NOINS uint mfgpr1(void)
{
	uint value;
	asm("l.or %0,r0,r1" : "=r" (value) : );
	return value;
}

_NOINS uint mfgpr9(void)
{
	uint value;
	asm("l.or %0,r0,r9" : "=r" (value) : );
	return value;
}

#endif


//******************************************************************************
// Cache
//------------------------------------------------------------------------------
//
_NOINS void disable_icache(void)
{
	// Disable IC: SR[ICE=4] = 0
	mtspr(EXR1K_SR, mfspr(EXR1K_SR) & ~EXR1K_SR_ICE);
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
}

_NOINS void disable_dcache(void)
{
	// Disable DC: SR[DCE=3] = 0
	mtspr(EXR1K_SR, mfspr(EXR1K_SR) & ~EXR1K_SR_DCE);
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
	asm("l.nop");
}

_NOINS int check_dcache(void)
{
	ulong dccfgr;
	ulong cache_set_size;
	ulong cache_ways;
	ulong cache_block_size;

	dccfgr = mfspr(EXR1K_DCCFGR);
	cache_ways = 1 << (dccfgr & EXR1K_DCCFGR_NCW);
	cache_set_size = 1 << ((dccfgr & EXR1K_DCCFGR_NCS) >> 3);
	cache_block_size = (dccfgr & EXR1K_DCCFGR_CBS) ? 32 : 16;

	return cache_set_size * cache_ways * cache_block_size;
}

int get_dcache_block_size(void)
{
	return 16 << ((mfspr(EXR1K_DCCFGR) & EXR1K_DCCFGR_CBS) >> 7);				// 32 @CBS=1, 16 @CBS=0
}

void invalidate_dcache_range(uint sadr, uint eadr)		// only in 32KB 
{
#ifdef CONFIG_DCACHE_ENABLE
	uint cbs = ((mfspr(EXR1K_DCCFGR) & EXR1K_DCCFGR_CBS) >> 7);	//  0 @16B/L,  1 @32B/L
	uint blk_bits = 4 + cbs;					//  5 @CBS=1, 	 4 @CBS=0
	uint blk_size = 16 << cbs;					// 32 @CBS=1, 	16 @CBS=0
	sadr = ((sadr >> blk_bits) << blk_bits);	// multiple of 16/32
	while(sadr < eadr)
	{
		mtspr(EXR1K_DCBIR, sadr);
		sadr += blk_size;
	}
#else
	UNUSED(sadr);
	UNUSED(eadr);
#endif
}

void invalidate_dcache_range_all(void)				// all
{
#ifdef CONFIG_DCACHE_ENABLE
	uint cbs = ((mfspr(EXR1K_DCCFGR) & EXR1K_DCCFGR_CBS) >> 7);	// 0 @16B/L, 1 @32B/L
	uint blk_bits = 4 + cbs;										// 5 @CBS=1, 4 @CBS=0
	uint cid, sadr;
	uint cid_end = cbs ? 1024 : 2048;
	for(cid=0; cid<cid_end; cid++)
	{
		sadr = cid << blk_bits;
		mtspr(EXR1K_DCBIR, sadr);
	}
#endif
}

//******************************************************************************
// Debug
//------------------------------------------------------------------------------
// exception stops the core and turn over control to JTAG
_NOINS void enable_jtag_ctrl_exception(void)
{
  	mtspr(EXR1K_DSR,	 EXR1K_DSR_RSTE	 	// [0] Reset exception                
					//	|EXR1K_DSR_BUSEE	// [1] Bus error exception            
						|EXR1K_DSR_DPFE		// [2] Data Page Fault exception      
						|EXR1K_DSR_IPFE		// [3] Insn Page Fault exception      
					//	|EXR1K_DSR_TTE		// [4] Tick Timer exception           	// Used @CPU
						|EXR1K_DSR_AE		// [5] Alignment exception            
						|EXR1K_DSR_IIE		// [6] Illegal Instruction exception  
					//	|EXR1K_DSR_IE		// [7] Interrupt exception            	// Used @CPU
						|EXR1K_DSR_DME		// [8] DTLB miss exception            
						|EXR1K_DSR_IME		// [9] ITLB miss exception            
					//	|EXR1K_DSR_RE		// [10] Range exception               
					//	|EXR1K_DSR_SCE		// [11] System call exception         	// Used @CPU
						|EXR1K_DSR_FPE   	// [12] Floating Point Exception      
						|EXR1K_DSR_TE	  	// [13] Trap exception  				
					);	  	

	//TRACE("DSR: %x \r\n",mfspr(EXR1K_DSR));
}

_NOINS void disable_jtag_ctrl_exception(void)
{
	mtspr(EXR1K_DSR,0);
}


//******************************************************************************
// HTB: Hardware Trace Buffer
//------------------------------------------------------------------------------
#ifndef _INLINE
_NOINS void enable_htb(void)
{
	mtspr(EXR1K_HTBCR,	 EXR1K_HTBCR_EN 	// HTB enable
						|EXR1K_HTBCR_NE		// NOP enable
						|EXR1K_HTBCR_TE	);	// Timestamp enable: Enable Free running counter
						
//	mtspr(EXR1K_HTBCR, 	 EXR1K_HTBCR_EN 	// HTB enable
//						|EXR1K_HTBCR_NE		// NOP enable
//						|EXR1K_HTBCR_TE		// Timestamp enable: Enable Free running counter
//						|EXR1K_HTBCR_TS_IF);// TS=IF, DS2=npc
	//mtspr(EXR1K_HTBCR, EXR1K_HTBCR_DS2_MDATI|EXR1K_HTBCR_TS_IF|EXR1K_HTBCR_TE|EXR1K_HTBCR_NE|EXR1K_HTBCR_EN);	// TS=IF, DS2=MDATI
	
	//TRACE("TBCR: %x \r\n",mfspr(EXR1K_HTBCR));
}

_NOINS void disable_htb(void)
{
	mtspr(EXR1K_HTBCR, mfspr(EXR1K_HTBCR) & ~EXR1K_HTBCR_EN);
}

_NOINS uint htb_enabled(void)
{
	return mfspr(EXR1K_HTBCR) & EXR1K_HTBCR_EN;
}

#endif


//******************************************************************************
// STB: Software Trace Buffer
//------------------------------------------------------------------------------
#ifdef _STB_ENABLE
_NOINS void enable_stb(void)
{
	stb_init(	 EXR1K_STBCR_STBEN 		// STB enable
			//	|EXR1K_STBCR_HTBEN 		// HTB enable @ STB functions: 			To make HTB records only @ actual functions
#ifndef DEF_CPU1			
				|EXR1K_STBCR_LRE 		// Link Register Exception Enable: 		LR @enter == LR @exit ?    
				|EXR1K_STBCR_SPE		// Stack Pointer Exception Enable: 		SP @enter == SP @exit ?   
				|EXR1K_STBCR_SPOVE		// Stack Pointer Overflow Exception Enable: SP in [_stack_e, _stack_s] ?
#endif
				|EXR1K_STBCR_FCSE		// Function Call Stack Exception Enable: Stack overflow?
				|EXR1K_STBCR_XCE		// Exception Call Exception Enable: 	Xsr called twice ?         
				|EXR1K_STBCR_TEE		// Ticktimer Exception Enable: 			SR[TEE] @enter == SR[TEE] @exit ?  
				|EXR1K_STBCR_IEE		// Interrupt Exception Enable: 			SR[IEE] @enter == SR[IEE] @exit ?
			//	|EXR1K_STBCR_OVE		// Overflow flag Exception: 			Ring buffer overflow ? Controlled by User		
				);
}
#else
_NOINS void enable_stb(void) {}
#endif



//******************************************************************************
// Hardware Watch Point & Watch Dog Timer
//------------------------------------------------------------------------------
// HW Watch Point
void enable_hwp(uint gid) 
{
	//ok
	//mtspr(EXR1K_WDTR(gid), MS2CLKCNT(5000));
	//mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) |EXR1K_WMR_BEE|EXR1K_WMR_WDTE|EXR1K_WMR_OSCE|EXR1K_WMR_ISCE|EXR1K_WMR_ESCE);

	//error
	//uint clk = (EXR1K_CLK/1000)*(msec);
	//mtspr(EXR1K_WDTR(gid), MS2CLKCNT(ms));
	//mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) |EXR1K_WMR_BEE|EXR1K_WMR_WDTE);//|EXR1K_WMR_OSCE|EXR1K_WMR_ISCE|EXR1K_WMR_ESCE);	// error
	mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) |EXR1K_WMR_BEE|EXR1K_WMR_OSCE|EXR1K_WMR_ISCE|EXR1K_WMR_ESCE);

	//mtspr(EXR1K_WDTR(gid), MS2CLKCNT(msec));
	////mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) |EXR1K_WMR_BEE|EXR1K_WMR_WDTE);//|EXR1K_WMR_OSCE|EXR1K_WMR_ISCE|EXR1K_WMR_ESCE);	// error
	//mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) |EXR1K_WMR_BEE|EXR1K_WMR_WDTE|EXR1K_WMR_OSCE|EXR1K_WMR_ISCE|EXR1K_WMR_ESCE);
	
}

void disable_hwp(uint gid) 
{
	mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) & ~EXR1K_WMR_BEE);
}

bool hwp_enabled(uint gid)
{
	return mfspr(EXR1K_WMR(gid)) & EXR1K_WMR_BEE;
}

void set_hwp(uint gid, uint wid, uint cmp_to, uint cmp_cnd, uint value, uint chain_cnd)
{
	mtspr(EXR1K_WVR(gid,wid), value);	
	mtspr(EXR1K_WCR(gid,wid), cmp_to|cmp_cnd);									
	mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) | (chain_cnd << (wid*EXR1K_WMR_CW_W + EXR1K_WMR_CW0_LSB)));
}

//------------------------------------------------------------------------------
// HW WDT
void enable_hwdt(uint gid, uint ms)
{
	mtspr(EXR1K_WDTR(gid), MS2CLKCNT(ms));
	mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) | EXR1K_WMR_WDTE);
}

void disable_hwdt(uint gid)
{
	mtspr(EXR1K_WDTR(gid), 0);
	mtspr(EXR1K_WMR(gid), mfspr(EXR1K_WMR(gid)) & ~EXR1K_WMR_WDTE);
}

//------------------------------------------------------------------------------
// Info
#ifdef _DEBUG
const char* cc_str[] = {""," ="," <","<="," >",">=","!=" };
const char* ct_str[] = {"","LDAT","SDAT"," DAT","","LADR","SADR"," ADR" };

void print_hwp_all_info(uint gid)
{	
	int i;
	char hw_ch = gid == EXR1K_SPRGID_IWB ? 'I' : 'D';
	TRACE("CPU%d %cHWP INFO: \r\n", CpuIdGet(), hw_ch);		
	/*
	for(i=0; i<8; i++) {
		TRACE("%cWVR%d: 0x%08x %cWCR%d: 0x%08x\r\n",hw_ch,i,mfspr(EXR1K_WVR(gid,i)), hw_ch,i,mfspr(EXR1K_WCR(gid,i)));
	}
	*/
	uint wmr = mfspr(EXR1K_WMR(gid));
	EnterPeriCS(UART0);
	TRACE("¦§%cWMR : 0x%08x\r\n",hw_ch,wmr);
	TRACE("¦§%cWAR : 0x%08x\r\n",hw_ch,mfspr(EXR1K_WAR(gid)));
	TRACE("¦§%cWSR : 0x%08x\r\n",hw_ch,mfspr(EXR1K_WSR(gid)));
	TRACE("¦§%cWDTR: 0x%08x\r\n",hw_ch,mfspr(EXR1K_WDTR(gid)));	
	TRACE("¦§%cHWP condition: ",hw_ch);
	if(wmr&EXR1K_WMR_WDTE)  TRACE("\r\n¦¢¦§WatchDog Timer: %d ms", CLKCNT2MS(mfspr(EXR1K_WDTR(gid))));
	if(wmr&EXR1K_WMR_OSCE)	TRACE("\r\n¦¢¦§Output Signals");
	if(wmr&EXR1K_WMR_ISCE)	TRACE("\r\n¦¢¦§Input Signals");
	if(wmr&EXR1K_WMR_ESCE)	TRACE("\r\n¦¢¦§Error Signals");

	for(i=0; i<8; i++) {
		uint cw = LDECBITS( wmr,  i*EXR1K_WMR_CW_W +EXR1K_WMR_CW0_MSB,  i*EXR1K_WMR_CW_W +EXR1K_WMR_CW0_LSB);
		
		if(cw==EXR1K_WMR_CW_MATCH) 			TRACE("\r\n¦¢¦§");
		else if(cw!=EXR1K_WMR_CW_DISABLE)	TRACE(" & ");

		if(cw!=EXR1K_WMR_CW_DISABLE) {
			uint wcr = mfspr(EXR1K_WCR(gid,i));
			uint ct = LDECBITS( wcr, EXR1K_WCR_CT_MSB, EXR1K_WCR_CT_LSB);
			uint cc = LDECBITS( wcr, EXR1K_WCR_CC_MSB, EXR1K_WCR_CC_LSB);
			
			TRACE("CW%d: %s %s 0x%08x ", i, ct_str[ct], cc_str[cc],mfspr(EXR1K_WVR(gid,i)));
		}		
	}
	
	TRACE("\r\n\r\n");
	LeavePeriCS(UART0);
}

void print_hwp_info(uint gid)
{	
	int i;
	char hw_ch = gid == EXR1K_SPRGID_IWB ? 'I' : 'D';
	uint wmr = mfspr(EXR1K_WMR(gid));
	uint wsr = mfspr(EXR1K_WSR(gid));

	EnterPeriCS(UART0);
	TRACE("¦§%cWMR : 0x%08x\r\n",hw_ch,wmr);
	TRACE("¦§%cWAR : 0x%08x\r\n",hw_ch,mfspr(EXR1K_WAR(gid)));
	TRACE("¦§%cWSR : 0x%08x\r\n",hw_ch,wsr);
	TRACE("¦§%c.Bus error occurs at: ",hw_ch);
	if(wsr&EXR1K_WSR_WDT) 	TRACE("\r\n¦¢¦§WatchDog Timer");
	if(wsr&EXR1K_WSR_OSC)	TRACE("\r\n¦¢¦§Output Signals");
	if(wsr&EXR1K_WSR_ISC)	TRACE("\r\n¦¢¦§Input Signals");
	if(wsr&EXR1K_WSR_ESC)	TRACE("\r\n¦¢¦§Error Signals");

	for(i=0; i<8; i++) {
		uint cw 	= LDECBITS(wmr,  i   *EXR1K_WMR_CW_W +EXR1K_WMR_CW0_MSB,  i   *EXR1K_WMR_CW_W +EXR1K_WMR_CW0_LSB);
		uint cw_nxt = LDECBITS(wmr, (i+1)*EXR1K_WMR_CW_W +EXR1K_WMR_CW0_MSB, (i+1)*EXR1K_WMR_CW_W +EXR1K_WMR_CW0_LSB);

		uint st		= LDECBIT(wsr, i	+EXR1K_WST_CW0_BIT);
		uint st_nxt	= LDECBIT(wsr, i+1	+EXR1K_WST_CW0_BIT);
		
		if(st || (st_nxt & (cw_nxt==EXR1K_WMR_CW_AND_PCHAIN || cw_nxt==EXR1K_WMR_CW_OR_PCHAIN))) {
			if(cw==EXR1K_WMR_CW_MATCH) 			TRACE("\r\n¦¢¦§");
			else if(cw!=EXR1K_WMR_CW_DISABLE)	TRACE(" & ");
	
			if(cw!=EXR1K_WMR_CW_DISABLE) {
				uint wcr = mfspr(EXR1K_WCR(gid,i));
				uint ct = LDECBITS( wcr, EXR1K_WCR_CT_MSB, EXR1K_WCR_CT_LSB);
				uint cc = LDECBITS( wcr, EXR1K_WCR_CC_MSB, EXR1K_WCR_CC_LSB);
				TRACE("CW%d: %s %s 0x%08x ", i, ct_str[ct], cc_str[cc],mfspr(EXR1K_WVR(gid,i)));
			}
		}
	}
	
	TRACE("\r\n\r\n");
	LeavePeriCS(UART0);
}


#else
void print_hwp_all_info(uint gid) {(void)(gid);};
void print_hwp_info(uint gid) {(void)(gid);};
#endif


//******************************************************************************
// test
//------------------------------------------------------------------------------
/*
ullong exr1k_test(ullong a)
{
	ullong b;
	b = a/ 10;
	
	return b;
}
*/
