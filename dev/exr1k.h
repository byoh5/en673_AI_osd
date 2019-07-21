//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150724		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef	_EXR1K_H_		
#define	_EXR1K_H_

#include "dev.h"
#include "dev_types.h"
#include "exr1k_sprs.h"
//#include "exr1k_inline.h"

#define	EXR1K_CLK		BUS_CLK	// dev define

#define	MS2CLKCNT(msec)	(EXR1K_CLK/1000*(msec))
#define	CLKCNT2MS(cnt)	((cnt)/EXR1K_CLK*1000)


//******************************************************************************
//
//------------------------------------------------------------------------------
#ifdef _STB_ENABLE
#define	_NOINS	__attribute__((no_instrument_function))
#else
#define	_NOINS
#endif

#define INLINE

#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))


//******************************************************************************
// Boot: defined @mem.ld
//------------------------------------------------------------------------------
// LMA information
extern const uint _lma_size;
extern const uint _lma_start;
extern const uint _reset_lma_s;
extern const uint _reset_lma_e;
extern const uint _text_lma_s;
extern const uint _text_lma_e;
extern const uint _ispm_text_lma_s;
extern const uint _ispm_text_lma_e;
extern const uint _rodata_lma_s;
extern const uint _rodata_lma_e;
extern const uint _data_lma_s;
extern const uint _data_lma_e;

// VMA information
extern const uint _reset_s;
extern const uint _reset_e;
extern const uint _text_s;
extern const uint _text_e;
extern const uint _ispm_text_s;
extern const uint _ispm_text_e;
extern const uint _rodata_s;
extern const uint _rodata_e;
extern const uint _data_s;
extern const uint _data_e;
extern const uint _bss_s;
extern const uint _bss_e;
extern const uint _stack_s;
extern const uint _stack_e;

// STB
extern ulong __stb_info_s;				
extern ulong __stb_info_e;				
extern ulong __stb_e;					


//******************************************************************************
// Sprs
//------------------------------------------------------------------------------
// Critical section: TEE|IEE disable at once @CR
#define	CRITICAL_BEGIN			mfspr(EXR1K_CSR)
#define	CRITICAL_CSR			mfspr(EXR1K_CSR)
#define	CRITICAL_END			mtspr(EXR1K_CSR, 0)

//------------------------------------------------------------------------------
#define	CRITICAL_HTB_DEN(ID)	register uint htbcr##ID = mfspr(EXR1K_HTBCR);			\
								register uint htb_en##ID = htbcr##ID & EXR1K_HTBCR_EN;	\
								if(!(stb_cr & EXR1K_STBCR_HTBEN) && htb_en##ID)			\
									mtspr(EXR1K_HTBCR, htbcr##ID & ~EXR1K_HTBCR_EN);
								
#define	CRITICAL_HTBCR(ID)		htbcr##ID
								
#define	CRITICAL_HTB_EN(ID)		if(!(stb_cr & EXR1K_STBCR_HTBEN) && htb_en##ID)			\
									mtspr(EXR1K_HTBCR, htbcr##ID | EXR1K_HTBCR_EN);


//------------------------------------------------------------------------------
#define	HTB_DEN(ID)				register uint htbcr##ID = mfspr(EXR1K_HTBCR);			\
								register uint htb_en##ID = htbcr##ID & EXR1K_HTBCR_EN;	\
								if(htb_en##ID)											\
									mtspr(EXR1K_HTBCR, htbcr##ID & ~EXR1K_HTBCR_EN);
								
#define	HTB_EN(ID)				if(htb_en##ID)											\
									mtspr(EXR1K_HTBCR, htbcr##ID | EXR1K_HTBCR_EN);

//------------------------------------------------------------------------------
// Mxspr
#ifndef _INLINE
void mtspr(uint spr, uint value);
uint mfspr(uint spr);
#endif

uint ext_int_enabled(void);
void enable_ext_int(void);
void disable_ext_int(void);

uint timer_enabled(void);
void enable_timer(void);
void disable_timer(void);

uint xsr_enabled(void);
void enable_xsr(void);
void disable_xsr(void);

void enable_immu(void);
void disable_immu(void);
                       
void enable_dmmu(void);
void disable_dmmu(void);

void enable_fp_xsr(void);
void set_fp_round_mode(uint mode);
uint get_fp_round_mode(void);
void set_fp_round_precision(uint prec);


// Mxgpr
//------------------------------------------------------------------------------
#ifndef _INLINE
uint mfgpr1(void);
uint mfgpr9(void);
#endif

// Cache
void disable_icache(void);
void disable_dcache(void);

int check_dcache(void);
int get_dcache_block_size(void);
void invalidate_dcache_range(uint sadr, uint eadr);
void invalidate_dcache_range_all(void);

// JTAG debugger
void enable_jtag_ctrl_exception(void);
void disable_jtag_ctrl_exception(void);

// HTB: Hardware Trace buffer
#ifndef _INLINE
void enable_htb(void);
void disable_htb(void);
uint htb_enabled(void);
#endif

// STB: Software Trace Buffer
void enable_stb(void);

// Hardware Watch Point & Watch Dog Timer
void enable_hwp(uint gid);
void disable_hwp(uint gid);
bool hwp_enabled(uint gid);
void set_hwp(uint gid, uint wid, uint cmp_to, uint cmp_cnd, uint value, uint chain_cnd);

void enable_hwdt(uint gid, uint msec);
void disable_hwdt(uint gid);

void print_hwp_all_info(uint gid);
void print_hwp_info(uint gid);


//******************************************************************************
// Utils
//------------------------------------------------------------------------------
#ifndef NOP_NOP
#define NOP_NOP         0x0000      // Normal nop instruction
#define NOP_EXIT        0x0001      // End of simulation
#define NOP_REPORT      0x0002      // Simple report
#define NOP_PUTC        0x0004      // sim_putc instruction
#define NOP_PUTS        0x0008      // sim_puts instruction
#define NOP_PUTHEX		0x0010      // sim_puthex instruction
#endif


#define	_FILL_NOPS						 	\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop  0");                        \
    asm("l.nop  0");                        \
    asm("l.nop  0");                        \
    asm("l.nop  0");                        \
    asm("l.nop  0");                        \
    asm("l.nop	0");

#define	_FILL_NOPS_1CACHE_LINE			 	\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");						\
  	asm("l.nop	0");						\
    asm("l.nop	0");						\
    asm("l.nop	0");

void call(uint addr);

//------------------------------------------------------------------------------
// Process control
#define TRAP	asm("l.trap 0");
#define BREAK	{ enable_jtag_ctrl_exception(); asm("l.trap 0");	}

#define sys_call(v) asm volatile("l.sys\t%0": : "n"(v));


//******************************************************************************
// Exceptions
//------------------------------------------------------------------------------
const char* get_xsr_str(int n);
uint get_xsr(int n);
void add_xsr(int n, void (*handler)(void));
void default_xsr(unsigned xsr_adr, unsigned epc);
void syscall_xsr(void);

// Use when code runs: possible to change SR @SYSCALL only
#define	SYS_SR_TED	0x01	//	TE disable
#define	SYS_SR_TEE	0x02	//	TE enable
#define	SYS_SR_IED	0x04	//	IE disable
#define	SYS_SR_IEE	0x08	//	IE enable

void sys_enable_timer(void);
void sys_disable_timer(void);
                              	
void sys_enable_ext_int(void);
void sys_disable_ext_int(void);

void sys_enable_int(void);
void sys_disable_int(void);


//******************************************************************************
// Tick Timer
//------------------------------------------------------------------------------
#define TIME_TICK   100 //means timer's interrupt occured 100 times during 1 sec.

enum { TIMEOUT_ONESHOT=0, TIMEOUT_PERIODIC=1 };

#define INVALID_TIMER_ID 0xFFFFFFFF

struct timeval {
    long tv_sec;		        // Seconds
    long tv_usec;		        // Microseconds
};

// Default timer xsr
void reset_timer_ticks(void);
uint get_timer_ticks(void);
uint get_timer_ms_per_ticks(void);

void default_timer_xsr(void);

// Simple timer
void init_timer(int ticks_per_sec);
void stop_timer(void);
void clear_timer(void);

// Timeout timer
#if 1
u32 get_tick(void);
void xsr_tick(void);
void timer_tick_init(u32 cpu_clk, u32 tick_cnt);
void timer_init(void (*tick_isr)(void));
u32 timer_get_ms(void);
void timer_delay(u32 ms);
void timer_poll(void);
u32 timer_sched_timeout_cb(u32 ms, u32 type, void (*cb)(void *ctx), void* ctx);
void timer_cancel_timeout(u32 id);
int gettimeofday(struct timeval *tp, void * dummy);
u32 timeoffset(u32 offset);
#else
enum { TT_ONESHOT=0, TT_PERIODIC=1 };
void init_tmo_timer(uint ticks_per_sec);
int add_tmo_timer_handler(uint ms, uchar type, void (*cb)(void *ctx), void* ctx);
#endif

// Benchmark timer
void bench_timer_init(uint ticks_per_sec);
uint bench_tic(void);
uint bench_toc(void);
uint bench_time_ms(void);

#if defined(_DEBUG) & defined(_BENCHMARK)
#define	BENCHMARK(fname, func)										\
{	bench_tic();													\
	func;															\
	bench_toc();													\
	printf("BENCHMARK ( %s ): %d ms \r\n", fname, bench_time_ms());	\
}
#else
#define BENCHMARK(fname, func)	func
#endif

//******************************************************************************
// HW Benchmark Timer @ exr1k.h: near HTB header
// NOTE: 	Currently nested HWBENCHMARK inside functions not working properly !
// 			Overflow in HTBTR not considered !
//------------------------------------------------------------------------------
void hw_bench_timer_init(void);
uint hw_bench_tic(void);
uint hw_bench_toc(void);
uint hw_bench_time_clk(void);

#if 1// defined(_DEBUG) & defined(_HWBENCHMARK)
#define	HWBENCHMARK(fname, func)											\
{	hw_bench_tic();															\
	func;																	\
	hw_bench_toc();															\
	printf("BENCHMARK ( %s ): %d us\r\n", fname, hw_bench_time_clk());	\
}
#else
#define	init_hw_bench_timer	
#define HWBENCHMARK(fname, func)	func
#endif

//******************************************************************************
//
//------------------------------------------------------------------------------
extern void printtime(void);

#endif//_EXR1K_H_
