//******************************************************************************
//	Copyright(c) Young-Geun Kim 2003-
//
//	File Name:		exr1k_stb.c
//	Description:	Software Trace Buffer
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}), Eyenix Co.,Ltd.
//
//	Note:			Compiler option: -finstrument-functions
// 					 Each function ele will call the hook __cyg_profile_func_enter()  
// 					 Each function exit will call the hook __cyg_profile_func_exit()
// 					 Not to be profiled, use __attribute__((no_instrument_function)) 
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151007		ygkim		first designed
// -----------------------------------------------------------------------------
#include <string.h>
#include "dev.h"			
#include "dev_inline.h"	// to use fast code

#ifndef _STB_ENABLE

_NOINS void stb_init(ushort cr) { UNUSED(cr); }
_NOINS void stb_reset_buf(void) {}
_NOINS void stb_enable_trace(void) {}
_NOINS void stb_disable_trace(void) {}

_NOINS void stb_print_ring(void) {}
_NOINS bool stb_push_name(void* addr, const char* name) { UNUSED(addr); UNUSED(name); return 1; }
_NOINS void stb_print_name(void) {}
_NOINS void stb_print_info(void) {}
_NOINS void stb_print_trace(void)  {}
_NOINS void stb_print_level(void) {}
_NOINS void stb_print_stack(void) {}


//******************************************************************************
#else// _STB_ENABLE
//------------------------------------------------------------------------------
//#ifdef _STB_ENABLE
#define	STB_IM 		__attribute__((section(".stb_info")))		// 	STB Info memory
#define	STB_DM 		__attribute__((section(".stb_data")))		//	STB Data memory
#define	STB_VM		ISPD_DEV									//	STB Variable memory
//#else
//#define	STB_IM
//#define	STB_DM
//#endif

//------------------------------------------------------------------------------
#define STB_CALL_STACK_LEN	32		// Function call stack length (depth)

#define	STB_NAME_BUF_LEN	32		// Function name buffer length
#define	STB_NAME_LEN		16		// Function name length


//------------------------------------------------------------------------------
#define	STB_VERBOSE_LEVEL	1		// To print intermediate results: 0-NONE, 1-HIGH, 2-HIGH+MID, 3-HIGH+MID+LOW

#if(STB_VERBOSE_LEVEL>=1)
#define	HTRACE				TRACE
#else
#define	HTRACE				//
#endif

#if(STB_VERBOSE_LEVEL>=2)
#define	MTRACE				TRACE
#else
#define	MTRACE				//
#endif

#if(STB_VERBOSE_LEVEL>=3)
#define	LTRACE				TRACE
#else
#define	LTRACE				//
#endif

#define	CTRACE				TRACE

//------------------------------------------------------------------------------
// STB info
// cacheable and reliable location @SRAM
STB_VM ushort stb_cr = 0;			// stb control register
                            
STB_VM char stb_level = 0; 			// stb level
STB_VM char stb_min_level = 0; 		// stb min level during run

STB_VM int stb_ring_widx = 0; 		// write index: point to the next write
STB_VM int stb_ring_ridx = 0;		// read index: point to the next read
STB_VM int stb_ring_cnt = 0; 		// # of elements in buffer

STB_VM int stb_stack_top = 0; 		// points to next free element on stack

#if defined(_STB_NAME_ENABLE)
volatile int stb_name_top = 0;		// points to next free element on stack
#endif

// STB info: reserved 16 words fixed @ stb memory
typedef struct {
	ulong _this;			// info buffer address
	ushort cr;				// stb control register
	char min_level; 		// stb min level during run
	char _res2; 			// 
	
	ulong ring_buffer;
	int ring_size;			// ring size
	int ring_widx;  		// write index: point to the next write
	int ring_ridx; 			// read index: point to the next read
	int ring_cnt; 			// # of elements in buffer	

	ulong stack_buffer;
	int stack_top; 			// points to next free element on stack
	
	ulong name_buffer;
	int name_top;			// points to next free element on stack	
} tStbInfo;

STB_VM tStbInfo* pinfo; 

//------------------------------------------------------------------------------
// STB data
// function call info @ring buffer @STB_DM
typedef struct {            
  	bool 	is_enter; 		// 1: __cyg_profile_func_enter(), 0: __cyg_profile_func_exit()
  	char 	level;			// call level
  	ushort	tstamp;			// time stamp
  	ulong 	this_fn; 		// current function address
  	ulong 	call_site; 		// return address of the caller function
  	uint	sr;				// supervision register
  	uint	sp;				// stack pointer
} tStbRingElement;
		
STB_VM tStbRingElement* stb_ring_buffer = NULL;										
STB_VM int stb_ring_size = 0;

// function call stack
typedef struct {
	ulong	idx;
	ulong	call_site;
	uint	sp;
} tStbStackElement;
STB_VM tStbStackElement stb_stack_buffer[STB_CALL_STACK_LEN];

// function name buffer
#if defined(_STB_NAME_ENABLE)
typedef struct {
  	ulong 	addr; 					// function address
  	char	name[STB_NAME_LEN];		// function name
} tCygNameElement;

tCygNameElement stb_name_buffer[STB_NAME_BUF_LEN];
#endif


//------------------------------------------------------------------------------
void stb_store_ring(bool is_enter, void* this_fn, void* call_site);
const char* stb_find_name(ulong addr);
void stb_update_info(void);
void stb_print_raw_element(bool is_enter, char level, ulong this_fn, ulong call_site);
void stb_print_raw_element2(bool is_enter, char level, ulong this_fn, ulong call_site);
void stb_print_element(const tStbRingElement* pEle);


//******************************************************************************
//
//------------------------------------------------------------------------------
/*
_NOINS void stb_print_raw_element2(bool is_enter, char level, ulong this_fn, ulong call_site) 
{
	int s;
    HTRACE("[%8d] %d %d ", stb_ring_widx, htb_enabled(), xsr_enabled());
	for(s=stb_min_level; s<level; s++) HTRACE("|  ");
    HTRACE("%c  %08x \t%s\t  %08x\r\n",   
    	is_enter ? '{':'}', 	call_site,
		is_enter ? "->":"<-",	this_fn);
}
*/


//******************************************************************************
// cyg profile functions
//	this_fn: 	Address of the current function
//	call_site: 	Return address to the function which called this_fn
//------------------------------------------------------------------------------
_NOINS void __cyg_profile_func_enter(void* this_fn, void* call_site) 
{
	CRITICAL_BEGIN;

	// store func@enter
  	if((stb_cr&EXR1K_STBCR_STBEN)) {
  		CRITICAL_HTB_DEN(0);
  			
		//CTRACE("@ENTER: 0x%08x -> 0x%08x\r\n", call_site, this_fn);
		//stb_print_raw_element2(1, stb_level, (ulong)this_fn, (ulong)call_site);
  		//stb_store_ring(1, this_fn, call_site, CRITICAL_SR(0));
  		stb_store_ring(1, this_fn, call_site);
  		
  		CRITICAL_HTB_EN(0);
  	}

	CRITICAL_END;
}

		
_NOINS void __cyg_profile_func_exit(void* this_fn, void* call_site) 			// $CMT-ygkim-151021: disable to make HTB remain @ error addr's
{
	CRITICAL_BEGIN;

	// store func@exit
  	if((stb_cr&EXR1K_STBCR_STBEN)) {
  		CRITICAL_HTB_DEN(0);

		//CTRACE("@EXIT : 0x%08x <- 0x%08x\r\n", call_site, this_fn);
		//stb_print_raw_element2(0, stb_level-1, (ulong)this_fn, (ulong)call_site);
  		//stb_store_ring(0, this_fn, call_site, CRITICAL_SR(0));
  		stb_store_ring(0, this_fn, call_site);

  		CRITICAL_HTB_EN(0);
  	}

	CRITICAL_END;
}																				
																				

//******************************************************************************
// function call stack to store level information
//------------------------------------------------------------------------------
_NOINS bool stb_push_stack(tStbStackElement* ele) 
{
	bool err = 0;
	
	if((stb_cr&EXR1K_STBCR_FCSE) && (stb_stack_top>=STB_CALL_STACK_LEN)) {
		stb_disable_trace();
		HTRACE("\r\nSTB stack overflow!! Check the depth and increase!\r\n");
		BREAK;
		err = 1;
	}	
	else {
		stb_stack_buffer[stb_stack_top] = *ele;
		MTRACE("\r\n@stack push[%d]: ring idx: %08x\r\n", stb_stack_top, ele->idx);

  		if(stb_stack_top<STB_CALL_STACK_LEN-1) stb_stack_top++;  		
  	}
  	return err;
}
		
_NOINS tStbStackElement* stb_pop_stack(void) 
{
  	if(stb_stack_top>0) {
    	stb_stack_top--;
		MTRACE("\r\n@stack pop[%d]: ring idx: %08x\r\n", stb_stack_top, stb_stack_buffer[stb_stack_top].idx);
    	return &stb_stack_buffer[stb_stack_top];
  	}
  	MTRACE("\r\n@stack pop[0]: addr: 0\r\n");
  	return NULL; // error
}
		
_NOINS tStbStackElement* stb_get_stack_top(void) 
{
  	if(stb_stack_top>=0) {
		MTRACE("\r\n@stack get top[%d]: ring idx: %08x\r\n", stb_stack_top, stb_stack_buffer[stb_stack_top].idx);
    	return &stb_stack_buffer[stb_stack_top];
  	}
  	MTRACE("\r\n@stack get top[0]: addr: 0\r\n");
  	return NULL; // error
}

_NOINS void stb_print_stack(void)
{
	HTRACE("Function Call Stack Info: count=%d\r\n", stb_stack_top);
  	int i;

  	for(i=0; i<STB_CALL_STACK_LEN/*stb_stack_top*/; i++) {
  		int idx = stb_stack_buffer[i].idx;
  		ulong call_site = stb_ring_buffer[idx].call_site;
  		ulong this_fn = stb_ring_buffer[idx].this_fn;
  		//const char* name = NULL;
  		//#if defined(_STB_NAME_ENABLE)
  		//name = stb_find_name(this_fn);
  		//#endif
  		
  		HTRACE(" [%2d]: %c idx: %6d, call_site: 0x%08x, this_fn: 0x%08x\r\n", i, (i==stb_stack_top ? 'T':' '), idx, call_site, this_fn);
  	}
  	HTRACE("\r\n");
}



//******************************************************************************
// function ring buffer to store trace information
//------------------------------------------------------------------------------
_NOINS int stb_put_ring(tStbRingElement* pEle)
{
  	if(stb_ring_cnt>=stb_ring_size) stb_cr |= EXR1K_STBCR_OV;
  	
  	// check OVE & OV	
	if((stb_cr&EXR1K_STBCR_OVE) && (stb_cr&EXR1K_STBCR_OV)) {
		disable_htb();
		stb_disable_trace();
		puts_("STB:OVE-stop. Press 'c' @JTAG to coninue...\r\n");		
		BREAK;
		//stb_print_ring();		
		//stb_print_trace();		
		//stb_reset_buf();		
		//stb_enable_trace();		
  	} 
  	
  	// store function info
   	stb_ring_buffer[stb_ring_widx] = *pEle;
	int cur_widx = stb_ring_widx;
	LTRACE("@ring put[%d]: this_fn: %08x, call_site: %08x\r\n", stb_ring_widx, pEle->this_fn, pEle->call_site);

	// update stb info
	stb_ring_widx = (stb_ring_widx+1) % stb_ring_size;
	if((stb_cr&EXR1K_STBCR_OV)) stb_ring_ridx = stb_ring_widx;				// next to read (oldest) == next to write
	stb_ring_cnt = min(stb_ring_cnt+1, stb_ring_size);

	if(stb_ring_cnt!=stb_ring_size/*OV*/ && stb_ring_widx != stb_ring_cnt) { 
		disable_htb();
		stb_disable_trace();
		HTRACE("widx = %d, cnt = %d, t=%d\r\n", stb_ring_widx, stb_ring_cnt, pEle->tstamp);
		int i;
		for(i=stb_ring_widx; i<stb_ring_cnt; i++) {
			stb_print_element(stb_ring_buffer+i);
		}
		BREAK;
		stb_print_ring();		
		stb_print_trace();		
		BREAK;
	}

	stb_update_info();  									
  	return cur_widx;
}
		
_NOINS tStbRingElement* stb_get_ring(void) 
{
  	tStbRingElement* pRing = NULL;

  	//if(!stb_ovf && stb_ring_ridx==stb_ring_widx) err = 1;
  	if(stb_ring_cnt>0) {
    	pRing = &stb_ring_buffer[stb_ring_ridx];
    	//HTRACE("@ring get[%d] %d: this_fn: %08x, call_site: %08x @%08x\r\n", stb_ring_ridx, stb_ring_size, pEle->this_fn, pEle->call_site, pEle);
		
    	stb_ring_ridx = (stb_ring_ridx+1) % stb_ring_size;
    	stb_ring_cnt--;
		stb_update_info();  	
  	}
  	
  	return pRing;
}
		
_NOINS void stb_store_ring(bool is_enter, void* this_fn, void* call_site)
{
	static bool prv_is_enter = 0;
	
	// @exit
	if(!is_enter) {
		// decrease level
		stb_level--;	
		
		tStbStackElement* sEle = stb_pop_stack();
		if(sEle) {
			// pop stack the return address @ call stack
			int idx_enter = sEle->idx;
			tStbRingElement* pEnter = stb_ring_buffer+idx_enter;	// for checking data consistency btw stack = ring @DDR
			
			// check LRE
			if(stb_cr&EXR1K_STBCR_LRE) {
				ulong call_site_enter = sEle->call_site;
				if(call_site_enter!=(ulong)call_site) {					
					disable_htb();
					/*
					if(call_site_enter== 0x0608B41C) {
						stb_disable_trace();
						HTRACE("\r\nSTB:LRE - !!\r\n");
						BREAK;
					}
					*/
					BREAK;
					stb_disable_trace();
					HTRACE("\r\nSTB:LRE - Calling order error!!\r\n");
					HTRACE("stb_level=%2d, stb_stack_top=%2d, call_site @stack: %08x, call_site @ring: %08x [%d], call_site @exit: %08x [%d]\r\n", 
							stb_level, stb_stack_top, call_site_enter, pEnter->call_site, idx_enter, call_site, stb_ring_widx);
					stb_print_element(pEnter);
					stb_print_raw_element(is_enter, stb_level, (ulong)this_fn, (ulong)call_site);
					stb_print_stack();
					BREAK;				
					stb_print_ring();
					stb_print_trace();
					//stb_reset_buf();
					//stb_enable_trace();
				}
			}

			// check RDW: Only Warning!!
			if(stb_cr&EXR1K_STBCR_RDW) {
				ulong call_site_enter = sEle->call_site;
				if(call_site_enter!=pEnter->call_site) {					
					HTRACE("\r\nSTB:RDW - Ring Data Error!!\r\n");
					HTRACE("stb_level=%2d, stb_stack_top=%2d, call_site @stack: %08x, call_site @ring: %08x [%d], call_site @exit: %08x [%d]\r\n", 
							stb_level, stb_stack_top, call_site_enter, pEnter->call_site, idx_enter, call_site, stb_ring_widx);
				}
			}

			// check SPE
			if(stb_cr&EXR1K_STBCR_SPE) {
				ulong sp_enter = sEle->sp;
				ulong sp = mfgpr1();
				if(sp_enter!=(ulong)sp) {
					disable_htb();
					//BREAK;
					stb_disable_trace();
					HTRACE("\r\nSTB:SPE - SP error!!\r\n");
					HTRACE("stb_level=%2d, stb_stack_top=%2d, sp @stack: %08x, sp @ring: %08x [%d], sp @exit: %08x [%d]\r\n", 
							stb_level, stb_stack_top, sp_enter, pEnter->sp, idx_enter, sp, stb_ring_widx);
					stb_print_element(pEnter);
					stb_print_raw_element(is_enter, stb_level, (ulong)this_fn, (ulong)call_site);
					BREAK;				
					stb_print_stack();
					stb_print_ring();
					stb_print_trace();
					//stb_reset_buf();
					//stb_enable_trace();
				}
			}
		}
	}

	// @enter
	if(is_enter) {
		// check XCE
		if(stb_cr&EXR1K_STBCR_XCE && prv_is_enter) {
			if(stb_stack_top>=1) {
				int idx_prv_enter = stb_stack_buffer[stb_stack_top-1].idx;
				tStbRingElement* pPrvEnter = stb_ring_buffer+idx_prv_enter;
				ulong xsr_addr = (ulong)default_xsr;				
				//int prv_xsr_no = ((ulong)(pPrvEnter->call_site) >> 8) & 0x1f;
				//int xsr_no = ((ulong)(call_site) >> 8) & 0x1f;
				
				//if(pPrvEnter->is_enter && pPrvEnter->this_fn==xsr_addr && (ulong)this_fn==xsr_addr && prv_xsr_no==0x5 && xsr_no==0xc) {
				if(pPrvEnter->is_enter && pPrvEnter->this_fn==xsr_addr && (ulong)this_fn==xsr_addr) {
					disable_htb();
					//BREAK;
					stb_disable_trace();
					HTRACE("\r\nSTB:XCE - Exception happens twice!\r\n");
					HTRACE("ESR: %08x\r\n", mfspr(EXR1K_ESR));
					HTRACE("stb_level=%2d, stb_stack_top=%2d, this_fn @prv enter: %08x [%d], this_fn @enter [%d]: %08x\r\n", 
							stb_level, stb_stack_top, pPrvEnter->this_fn, idx_prv_enter, this_fn, stb_ring_widx);
					stb_print_element(pPrvEnter);
					stb_print_raw_element(is_enter, stb_level, (ulong)this_fn, (ulong)call_site);
					BREAK;
					stb_print_stack();
					stb_print_ring();
					stb_print_trace();
					BREAK;
				}
			}
		}			
	}

	// check SPOVE
	if(stb_cr&EXR1K_STBCR_SPOVE) {
		ulong sp = mfgpr1();
		if(sp<_stack_e || sp>_stack_s) {
			disable_htb();
			stb_disable_trace();
			HTRACE("\r\nSTB:SPOVE - Stack overflow!\r\n");
			HTRACE("SP: %08x, end: %08x start: %08x\r\n", sp,_stack_e, _stack_s);
			stb_print_raw_element(is_enter, stb_level, (ulong)this_fn, (ulong)call_site);
			BREAK;				
			stb_print_stack();
			stb_print_ring();
			stb_print_trace();
			BREAK;
		}
	}

	// store function info
	uint sp = mfgpr1();
	
  	tStbRingElement rEle;
  	rEle.is_enter 	= is_enter;
  	rEle.level 		= stb_level;
  	rEle.tstamp		= (ushort)mfspr(EXR1K_HTBTR);
  	rEle.this_fn 	= (ulong)this_fn;
  	rEle.call_site 	= (ulong)call_site;					// @is_enter: r9
  	rEle.sr			= mfspr(EXR1K_SR)|(mfspr(EXR1K_CSR)<<31);
  	rEle.sp			= sp;
  	int widx = stb_put_ring(&rEle);
	
	// @enter
	if(is_enter) {
		// push caller address @ call stack
		//stb_push_stack((ulong)call_site);
		tStbStackElement sEle;
		sEle.idx		= (ulong)widx;
		sEle.call_site	= (ulong)call_site;
		sEle.sp			= sp;
		stb_push_stack(&sEle);

		// increase level
		stb_level++;
	}

	//if(stb_level!=stb_stack_top) {
	//	stb_disable_trace();
	//	HTRACE("\r\nSTB level error!!\r\n");
	//	HTRACE("\r\nstb_level=%d, stb_stack_top=%d\r\n",stb_level, stb_stack_top);
	//	HTRACE("enter: %d, level: %2d, this_fn: 0x%08x, call_site: 0x%08x\r\n", is_enter, stb_level, this_fn, (ulong)call_site&ADDR_MASK);
	//	BREAK;	
	//}
		
  	if(stb_level<stb_min_level) stb_min_level = stb_level;
  	prv_is_enter = is_enter;	
}

_NOINS void stb_print_ring(void)
{
	HTRACE("Function Trace Info: count=%d\r\n", stb_ring_cnt);
	tStbRingElement ele;
  	int i;

  	for(i=0; i<stb_ring_cnt; i++) {
  		ele = stb_ring_buffer[i];
  		const char* name = NULL;
  		#if defined(_STB_NAME_ENABLE)
  		name = stb_find_name(ele.this_fn);
  		#endif
  		
  		putc_(i==stb_ring_widx ? 'w' : ' ');
  		putc_(i==stb_ring_ridx ? 'r' : ' ');
  		HTRACE(" %2d: enter: %d, level: %2d, this_fn: 0x%08x, call_site: 0x%08x: SR: 0x%08x SP: 0x%08x | %s\r\n", 
  				i, ele.is_enter, ele.level, ele.this_fn, ele.call_site&ADDR_MASK, ele.sr, ele.sp, name);
  	}
  	HTRACE("\r\n");
}

_NOINS void stb_print_level(void)
{
	HTRACE("cr=%08x, stb_level=%d, stb_stack_top=%d ridx=%d widx=%d\r\n",stb_cr, stb_level, stb_stack_top, stb_ring_ridx, stb_ring_widx);		
}


//******************************************************************************
// function name buffer to store name formation
//------------------------------------------------------------------------------
#if defined(_STB_NAME_ENABLE)
_NOINS bool stb_push_name(void* addr, const char* name)
{
	bool err = 0;

	if(stb_name_top>=STB_NAME_BUF_LEN) err = 1;
	else {	
		tCygNameElement* pEle = &stb_name_buffer[stb_name_top];
		pEle->addr = (ulong)addr;
		strncpy(pEle->name, name, STB_NAME_LEN);		
		pEle->name[STB_NAME_LEN-1] = '\0';			// for assurance
	
	  	if(stb_name_top<STB_NAME_BUF_LEN-1) stb_name_top++;
	}  		
  	return err;
}
		
_NOINS const char* stb_find_name(ulong addr)
{
  	int i;

  	for(i=0; i<stb_name_top; i++) 
  		if(stb_name_buffer[i].addr==addr) return stb_name_buffer[i].name;
  	
  	return "???"; // not found
}

_NOINS void stb_print_name(void)
{
	HTRACE("Function Name Info: count=%d\r\n", stb_name_top);
  	int i;

  	for(i=0; i<stb_name_top; i++) 
  		HTRACE("%2d: addr: 0x%08x, name: %s\r\n", i, stb_name_buffer[i].addr, stb_name_buffer[i].name);
  	HTRACE("\r\n");
}

#else
_NOINS bool stb_push_name(void* addr, const char* name) { UNUSED(addr); UNUSED(name); return 1; }

_NOINS void stb_print_name(void) {}

#endif// STB_NAME_ENABLE

//******************************************************************************
// stb info
//------------------------------------------------------------------------------
_NOINS void stb_init_info(void)
{
	pinfo = (tStbInfo*)&__stb_info_s;											// $CMT-ygkim-151019: only fixed @link time, not fixed @ compile time
	memset(pinfo, sizeof(tStbInfo), 0);
	pinfo->_this = (ulong)pinfo;
	pinfo->cr = stb_cr;
	pinfo->min_level = stb_min_level;
	
	pinfo->ring_buffer = (ulong)stb_ring_buffer;
	pinfo->ring_size = stb_ring_size;
	pinfo->ring_widx = stb_ring_widx;
	pinfo->ring_ridx = stb_ring_ridx;
	pinfo->ring_cnt = stb_ring_cnt;

	pinfo->stack_buffer = (ulong)stb_stack_buffer;
	pinfo->stack_top = stb_stack_top;
	
	#if defined(_STB_NAME_ENABLE)
	pinfo->name_buffer = (ulong)stb_name_buffer;
	pinfo->name_top = stb_name_top;
	#endif
}

_NOINS void stb_update_info(void)
{
	pinfo->cr= stb_cr;
	pinfo->min_level = stb_min_level;
	
	pinfo->ring_widx = stb_ring_widx;
	pinfo->ring_ridx = stb_ring_ridx;
	pinfo->ring_cnt = stb_ring_cnt;

	pinfo->stack_top = stb_stack_top;
	
	#if defined(_STB_NAME_ENABLE)
	//pinfo->name_top = stb_name_top;
	#endif
}


_NOINS void stb_print_info(void)
{
  	HTRACE("STB INFO:\r\n");
  	HTRACE("¦§Info buffer : 0x%08x - 0x%08x\r\n", pinfo->_this, (ulong)&__stb_e);
  	HTRACE("¦§Ring buffer : 0x%08x - 0x%08x, Element size = %d\r\n", pinfo->ring_buffer, pinfo->ring_buffer + pinfo->ring_size, sizeof(tStbRingElement));
  	HTRACE("¦§Stack buffer: 0x%08x - 0x%08x, Element size = %d\r\n", pinfo->stack_buffer, pinfo->stack_buffer + sizeof(stb_stack_buffer), sizeof(ulong));
  	#if defined(_STB_NAME_ENABLE)
  	HTRACE("¦§Name  buffer: 0x%08x - 0x%08x, Element size = %d\r\n", pinfo->name_buffer, pinfo->name_buffer + sizeof(stb_name_buffer), sizeof(tCygNameElement));
  	#endif
  	HTRACE("¦§STBCR: 0x%08x\r\n", pinfo->cr);
  	HTRACE("¦§EN : %d\r\n", pinfo->cr&EXR1K_STBCR_STBEN ? 1:0);
  	HTRACE("¦§LRE: %d\r\n", pinfo->cr&EXR1K_STBCR_LRE ? 1:0);
  	HTRACE("¦§OV : %d\r\n", pinfo->cr&EXR1K_STBCR_OV ? 1:0);
  	HTRACE("¦§OVE: %d\r\n", pinfo->cr&EXR1K_STBCR_OVE ? 1:0);
  	HTRACE("¦¦Min level: %d: %d\r\n", stb_min_level, pinfo->min_level);
	//HTRACE(" - stb_ring_buffer[0].is_enter: 0x%08x\r\n",&(stb_ring_buffer[0].is_enter));
	//HTRACE(" - stb_ring_buffer[0].level: 0x%08x\r\n",&(stb_ring_buffer[0].level));
	//HTRACE(" - stb_ring_buffer[0].tstamp: 0x%08x\r\n",&(stb_ring_buffer[0].tstamp));
	//HTRACE(" - stb_ring_buffer[0].this_fn: 0x%08x\r\n",&(stb_ring_buffer[0].this_fn));
	//HTRACE(" - stb_ring_buffer[0].call_site: 0x%08x\r\n",&(stb_ring_buffer[0].call_site));
	//HTRACE(" - stb_ring_buffer[0].sr: 0x%08x\r\n",&(stb_ring_buffer[0].sr));
	//HTRACE(" - stb_ring_buffer[0].sp: 0x%08x\r\n",&(stb_ring_buffer[0].sp));
	//         
	//HTRACE(" - stb_ring_buffer[1].is_enter: 0x%08x\r\n",&(stb_ring_buffer[1].is_enter));
	//HTRACE(" - stb_ring_buffer[1].level: 0x%08x\r\n",&(stb_ring_buffer[1].level));
	//HTRACE(" - stb_ring_buffer[1].tstamp: 0x%08x\r\n",&(stb_ring_buffer[1].tstamp));
	//HTRACE(" - stb_ring_buffer[1].this_fn: 0x%08x\r\n",&(stb_ring_buffer[1].this_fn));
	//HTRACE(" - stb_ring_buffer[1].call_site: 0x%08x\r\n",&(stb_ring_buffer[1].call_site));
	//HTRACE(" - stb_ring_buffer[1].sr: 0x%08x\r\n",&(stb_ring_buffer[1].sr));	
	//HTRACE(" - stb_ring_buffer[1].sp: 0x%08x\r\n",&(stb_ring_buffer[1].sp));
}


//******************************************************************************
// stb init/enable/disable
//------------------------------------------------------------------------------
_NOINS void stb_init(ushort cr)
{
	stb_ring_buffer = (tStbRingElement*)&__stb_info_e;							// $CMT-ygkim-151019: only fixed @link time, not fixed @ compile time
	stb_ring_size = ((ulong)&__stb_e - (ulong)&__stb_info_e)/sizeof(tStbRingElement);
	//HTRACE("%08x %08x %08x %08x: %08x, %d\r\n", pinfo, &__stb_info_s, &__stb_info_e, &__stb_e, stb_ring_buffer, stb_ring_size);
	//BREAK;	
	
	stb_level		= 0;
	stb_min_level	= 0;		// 1st init is enough
	
  	stb_ring_widx 	= 0;
  	stb_ring_ridx 	= 0;
  	stb_ring_cnt 	= 0;
  	
  	stb_stack_top 	= 0;		// 1st init is enough
  	
	#if defined(_STB_NAME_ENABLE)
  	stb_name_top	= 0;		// 1st init is enough
	#endif

	stb_cr = cr;

	stb_init_info();
	stb_print_info();
}		

_NOINS void stb_reset_buf(void)
{
	stb_cr ^= EXR1K_STBCR_OV;
	
  	stb_ring_widx 	= 0;
  	stb_ring_ridx 	= 0;
  	stb_ring_cnt 	= 0;
  	
	stb_update_info();
}		

_NOINS void stb_enable_trace(void)
{
	stb_cr |= EXR1K_STBCR_STBEN;
	pinfo->cr = stb_cr;
}

_NOINS void stb_disable_trace(void)
{
	stb_cr &= ~EXR1K_STBCR_STBEN;
	pinfo->cr = stb_cr;
}


//******************************************************************************
// Print
//------------------------------------------------------------------------------
_NOINS void stb_print_raw_element(bool is_enter, char level, ulong this_fn, ulong call_site) 
{
	uint caller_addr = is_enter ? (uint)(call_site) - 8 : (uint)call_site;

	int s;
	for(s=stb_min_level; s<level; s++) HTRACE("|  ");
    HTRACE("%c  ???: 0x%08x \t%s\t ???: 0x%08x @ %d\r\n",     	
    	is_enter ? '{':'}', 	caller_addr,
		is_enter ? "->":"<-",	this_fn, stb_ring_widx);
}

_NOINS void stb_print_element(const tStbRingElement* pEle) 
{
	uint caller_addr = pEle->is_enter ? (uint)(pEle->call_site) - 8 : (uint)pEle->call_site;
  	const char* caller_name = "???";
  	const char* thisfn_name = NULL;

	//ulong caller = stb_get_stack_top();
	//caller_name = stb_find_name(caller);       // more accurate to use use symbol table by comparing call_site address!!   			

#if defined(_STB_NAME_ENABLE)
	thisfn_name = stb_find_name(pEle->this_fn);
#endif
	
	int s;
	for(s=stb_min_level; s<pEle->level; s++) HTRACE("|  ");
    HTRACE("%c  %s: 0x%08x \t%s\t %s: 0x%08x | SR: 0x%08x SP: 0x%08x @ %08x, t = %d\r\n", 
    	pEle->is_enter ? '{':'}', 	caller_name, 	caller_addr,
		pEle->is_enter ? "->":"<-",	thisfn_name, 	pEle->this_fn,
		pEle->sr, pEle->sp, pEle, pEle->tstamp);
}
       	
       	
_NOINS void stb_print_trace(void) 
{
  	stb_disable_trace();	// for assurance
  	HTRACE("Print Function Trace:\r\n");
  	HTRACE(" - count = %d\r\n", stb_ring_cnt);
  	HTRACE(" - min. level = %d\r\n", stb_min_level); 
	
  	int cnt = stb_ring_cnt;
  	int i;
	
	//print trace
  	for(i=0; i<cnt; i++) 
  	{
  		tStbRingElement* pEle = stb_get_ring();
    	if(!pEle) HTRACE("ERROR getting element!\r\n");
    	else 
    	{    
    		pEle->call_site &= ADDR_MASK;
      		if(pEle->is_enter) {
      			stb_print_element(pEle);
        		//stb_push_stack(pEle->this_fn);
	      	} else {
	        	//stb_pop_stack();
				stb_print_element(pEle);
			}
			//stb_print_level();			
		}
  	}
  	HTRACE("Print Function Trace: done!\r\n");
}


#endif//_STB_ENABLE