//*************************************************************************************************
// Unused exception
//*************************************************************************************************
#include <string.h>
#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "uxsr.h"

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// 0x200 : BUS Error
void XsrBusErr(void)
{
	Uart0_Printf("bus error exception occured 0 : ", 0);
	BREAK;
}

// 0x600 : Data Aligment Error
void XsrAlign(void)
{
	Uart0_Printf("align error exception occured 0 : ", 0);
	Uart0_Printf("EXR1K_EPCR is : ", mfspr(EXR1K_EPCR));
	Uart0_Printf("EXR1K_ESR is : ", mfspr(EXR1K_ESR));
	BREAK;
}

// 0x700 : Illegal Insn
void XsrIllInsn(void)
{
	Uart0_Printf("invalid instruction exception occured 0 : ", 0);
	Uart0_Printf("EXR1K_EPCR is : ", mfspr(EXR1K_EPCR));
	Uart0_Printf("EXR1K_ESR is : ", mfspr(EXR1K_ESR));
	BREAK;
}

// 0xb00 : Range Error
void XsrFloatPoint(void)
{
	Uart0_Printf("range exception occured 0 : ", 0);
	BREAK;
}

// 0xd00 : 
void XsrBreak(void)
{
	Uart0_Printf("break exception occured 0 : ", 0);
	while(1);
}

// 0xe00 : Trap
void XsrTrap(void)
{
	Uart0_Printf("trap exception occured 0 : ", 0);
	while(1);
}

#ifdef	DEF_CPU1	//...........................
#define PAGE_SIZE	8192			// Page size

#define DTLB_SETS	64				// Number of DTLB sets used (power of 2, max is 256)
#define DTLB_WAYS	1				// Number of DTLB ways (1, 2, 3 etc., max is 4)
#define ITLB_SETS	64				// Number of ITLB sets used (power of 2, max is 256)
#define ITLB_WAYS	1				// Number of ITLB ways (1, 2, 3 etc., max is 4)

#define DTLB_PR_NOLIMIT		   (EXR1K_DTLBTR_URE|EXR1K_DTLBTR_UWE|EXR1K_DTLBTR_SRE|EXR1K_DTLBTR_SWE)
#define ITLB_PR_NOLIMIT		   (EXR1K_ITLBTR_SXE|EXR1K_ITLBTR_UXE)

//-------------------------------------------------------------------------------------------------
// 0x300 : Data page fault exception handler
void XsrDpFault(void)
{
	int		i;
	int		iSet, iWay = 0;
	UINT	nEa;

	nEa  = mfspr(EXR1K_EEAR);					// Get EA that cause the exception
	iSet = (nEa/PAGE_SIZE) % DTLB_SETS;			// Find TLB iset

	for(i=0; i<DTLB_WAYS; i++){
		if((mfspr(EXR1K_DTLBMR_BASE(i) + iSet) & EXR1K_DTLBMR_VPN) == (nEa & EXR1K_DTLBMR_VPN)){
			iWay = i;
			break;
		}
	}

	mtspr(EXR1K_DTLBTR_BASE(iWay) + iSet, (nEa&EXR1K_DTLBTR_PPN) | DTLB_PR_NOLIMIT);		// Give permission

	Uart0_Printf("Dp fault exception occured : ", nEa);
}

//-------------------------------------------------------------------------------------------------
// 0x400 : Intstruction page fault exception handler
void XsrIpFault(void)
{
	int		i;
	int		iSet, iWay = 0;
	UINT	nEa;

	nEa  = mfspr(EXR1K_EEAR);					// Get EA that cause the exception
	iSet = (nEa/PAGE_SIZE) % ITLB_SETS;			// Find TLB iset

	for(i=0; i<ITLB_WAYS; i++){
		if((mfspr(EXR1K_ITLBMR_BASE(i) + iSet) & EXR1K_ITLBMR_VPN) == (nEa & EXR1K_ITLBMR_VPN)){
			iWay = i;
			break;
		}
	}

	mtspr(EXR1K_DTLBTR_BASE(iWay) + iSet, (nEa&EXR1K_DTLBTR_PPN) | ITLB_PR_NOLIMIT);

	Uart0_Printf("Ip fault exception occured : ", nEa);
}

// 0x900 : DTLB miss
void XsrDtlbMiss(void)
{
	int		i;
	int		iSet, iWay = 0;
	UINT	nEa;

	nEa  = mfspr(EXR1K_EEAR);					// Get EA that cause the exception
	iSet = (nEa/PAGE_SIZE) % DTLB_SETS;			// Find TLB iset

	for(i=0; i<DTLB_WAYS; i++){
		if((mfspr(EXR1K_DTLBMR_BASE(i) + iSet) & EXR1K_DTLBMR_LRU) == 0){	// The lower the number, more recent access
			iWay = i;
			break;
		}
	}

	mtspr(EXR1K_DTLBMR_BASE(iWay) + iSet, (nEa&EXR1K_DTLBMR_VPN) | EXR1K_DTLBMR_V);
	mtspr(EXR1K_DTLBTR_BASE(iWay) + iSet, (nEa&EXR1K_DTLBTR_PPN) | DTLB_PR_NOLIMIT);
//	mtspr(EXR1K_DTLBTR_BASE(iWay) + iSet, (nEa&EXR1K_DTLBTR_PPN));

//	Uart0_Printf("Dtlb miss exception occured : ", nEa);
}

//-------------------------------------------------------------------------------------------------
// 0xa00 : ITLB miss exception handler
void XsrItlbMiss(void)
{
	int		i;
	int		iSet, iWay = 0;
	UINT	nEa;

	nEa  = mfspr(EXR1K_EEAR);					// Get EA that cause the exception
	iSet = (nEa/PAGE_SIZE) % ITLB_SETS;			// Find TLB iset

	for(i=0; i<ITLB_WAYS; i++){
		if((mfspr(EXR1K_ITLBMR_BASE(i) + iSet) & EXR1K_ITLBMR_LRU) == 0){
			iWay = i;
			break;
		}
	}

	mtspr(EXR1K_ITLBMR_BASE(iWay) + iSet, (nEa&EXR1K_ITLBMR_VPN) | EXR1K_ITLBMR_V);
	mtspr(EXR1K_ITLBTR_BASE(iWay) + iSet, (nEa&EXR1K_ITLBTR_PPN) | ITLB_PR_NOLIMIT);

	Uart0_Printf("Itlb miss exception occured : ", nEa);
}
#else
// 0x300 : Data page fault exception handler
void XsrDpFault(void)
{
	Uart0_Printf("data page fault exception occured 0 : ", 0);
	while(1);
}

// 0x400 : Intstruction page fault exception handler
void XsrIpFault(void)
{
	Uart0_Printf("instruction page fault exception occured 0 : ", 0);
	while(1);
}

// 0x900 : DTLB miss
void XsrDtlbMiss(void)
{
	Uart0_Printf("data TLB miss exception occured 0 : ", 0);
	while(1);
}

// 0xa00 : ITLB miss
void XsrItlbMiss(void)
{
	Uart0_Printf("instruction TLB miss exception occured 0 : ", 0);
	while(1);
}

// 0xc00 : System Call
void XsrSyscall(void)
{
	Uart0_Printf("system call exception occured 0 : ", 0);
	while(1);
}

#if 0
volatile UINT 	gTimerTicks;	
#define EXR1K_SPRRID_W 				11				// Register index width

#define EXR1K_SPRGID_TT				(10<< EXR1K_SPRRID_W)

#define EXR1K_TTMR 					(EXR1K_SPRGID_TT + 0)
#define EXR1K_TTCR 					(EXR1K_SPRGID_TT + 1)

// Tick Timer Control Register
#define EXR1K_TTCR_PERIOD			0x0fffffff  // Time Period 

#define EXR1K_TTMR_M      			0xc0000000  // [31:30] 	Tick mode 
#define EXR1K_TTMR_IE				0x20000000  // [29] 	Interrupt Enable 
#define EXR1K_TTMR_IP				0x10000000  // [28] 	Interrupt Pending 
#define EXR1K_TTMR_PERIOD			EXR1K_TTCR_PERIOD // [27:0] Tick Period

// EXR1K_TTMR_M fields
#define EXR1K_TTMR_M_DISABLE		0x00000000  // Disabled 
#define EXR1K_TTMR_M_RESTART		0x40000000  // Restart tick 
#define EXR1K_TTMR_M_STOP  			0x80000000  // Single run 
#define EXR1K_TTMR_M_NOSTOP			0xc0000000  // Continuous run 

void clear_timer(void) 	
{ 
	mtspr(EXR1K_TTMR, mfspr(EXR1K_TTMR)&~EXR1K_TTMR_IP); 						// Clear MR[IP]: to reset timer with same interval, that is, to restart timer
	//mtspr(EXR1K_TTMR, EXR1K_TTMR_IE | EXR1K_TTMR_M_RESTART | ((EXR1K_CLK/TICKS_PER_SEC) & EXR1K_TTMR_PERIOD));
}

void ISPM default_timer_xsr(void)
{
  	gTimerTicks++;	                            
	clear_timer();
}

char* except_strs[] = {
						"Unknown",       	// 0
						"Reset",         	// 1 
						"Bus Error",     	// 2
						"D-Page Fault",  	// 3
						"I-Page Fault", 	// 4
						"Tick",          	// 5
						"Data Align.",   	// 6
						"Illegal Insn.", 	// 7
						"Ext. Int.",     	// 8
						"D-TLB Miss",    	// 9
						"I-TLB Miss",    	// 10
						"Range Error",   	// 11
						"System Call",   	// 12
						"Floating-Point",	// 13
						"Trap"           	// 14
						};

void (*except_handlers[])(void) = {	0,    	// 0	: XSR_NONE		
								    0,    	// 1    : XSR_RESET		
								    0,    	// 2    : XSR_BUSERR		
								    0,    	// 3    : XSR_DPF			
								    0,    	// 4    : XSR_IPF			
								default_timer_xsr,    	// 5    : XSR_TICK		
								    0,    	// 6    : XSR_DALIGN		
								    0,    	// 7    : XSR_ILLEGAL		
								    0,    	// 8    : XSR_INT			
								    0,    	// 9    : XSR_DTLBMISS	
								    0,    	// 10   : XSR_ITLBMISS	
								    0,    	// 11   : XSR_RANGE		
								    0,    	// 12   : XSR_SYSCALL		
								    0,    	// 13   : XSR_FLOAT		
									0    	// 14   : XSR_TRAP		
								};
  
void add_xsr(int n, void (*handler)(void))
{
	except_handlers[n] = handler;
}

void default_xsr(unsigned xsr_adr, unsigned epc)
{
	int n = (xsr_adr >> 8) & 0x1f;
	if(except_handlers[n]) {
		(*except_handlers[n])();
	}
	else {

		printf("%s exception occurred @ EPC = 0x%08x \r\n", except_strs[n], epc & 0xfffffffc);

	}
}
#endif

#endif	// ......................................
