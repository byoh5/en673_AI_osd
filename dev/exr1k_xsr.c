//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_xsr.c
//	Description:	EXR1K Exception Service Routines
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150729		ygkim		first designed
// -----------------------------------------------------------------------------
#include "dev.h"	// to use printf
#include "dev_inline.h"	// to use fast code

void buserr_xsr(void);
void syscall_xsr(void);

const char* except_strs[] = {
						"Unknown       ",   // 0
						"Reset         ",   // 1
						"Bus Error     ",   // 2
						"D-Page Fault  ",  	// 3
						"I-Page Fault  ",   // 4
						"Tick          ",   // 5
						"Data Align.   ",   // 6
						"Illegal Insn. ",   // 7
						"Ext. Int.     ",   // 8
						"D-TLB Miss    ",   // 9
						"I-TLB Miss    ",   // 10
						"Range Error   ",   // 11
						"System Call   ",   // 12
						"Floating-Point",   // 13
						"Trap          "    // 14
						};

void (*except_handlers[])(void) = {	0,    	// 0	: XSR_NONE
								    0,    	// 1    : XSR_RESET
				buserr_xsr			,    	// 2    : XSR_BUSERR
								    0,    	// 3    : XSR_DPF
								    0,    	// 4    : XSR_IPF
#ifdef DEF_BOOT
									0,    	// 5    : XSR_TICK
#else
				default_timer_xsr	 ,    	// 5    : XSR_TICK
#endif
								    0,    	// 6    : XSR_DALIGN
								    0,    	// 7    : XSR_ILLEGAL
								    0,    	// 8    : XSR_INT
								    0,    	// 9    : XSR_DTLBMISS
								    0,    	// 10   : XSR_ITLBMISS
								    0,    	// 11   : XSR_RANGE
				syscall_xsr			,    	// 12   : XSR_SYSCALL
								    0,    	// 13   : XSR_FLOAT
									0    	// 14   : XSR_TRAP
								};

const char* get_xsr_str(int n) { return except_strs[n]; }
uint get_xsr(int n) { return (uint)except_handlers[n]; }
  
void add_xsr(int n, void (*handler)(void))
{
	except_handlers[n] = handler;
}

#define	_XSR_DEBUG		//_DEBUG
#ifdef _XSR_DEBUG
int xsr_cnt = 0;  
#endif

ISRT void default_xsr(unsigned xsr_adr, unsigned epc)
{
	//enable_htb();
	int n = (xsr_adr >> 8) & 0x1f;
	if(except_handlers[n]) {
#ifdef _XSR_DEBUG		
		xsr_cnt++;
		if(xsr_cnt>1) {
			disable_htb();
			TRACE("xsr called %d times\r\n",xsr_cnt);
			BREAK;
		}
#endif		
		(*except_handlers[n])();
#ifdef _XSR_DEBUG		
		xsr_cnt--;
#endif		
	}
	else {
		//BREAK;
		disable_htb();
		#ifdef _STB_ENABLE
		stb_disable_trace();
		#endif
		THROW(ERROR,"No exception handler assigned!");
		printf("CPU%d: %s exception occurred @ EPC = 0x%08x \r\n", CPU_ID, except_strs[n], epc & 0xfffffffc);
		report(xsr_adr);
		report(epc);
		exit(-1);
	}
	//disable_htb();
}

//------------------------------------------------------------------------------
//
void buserr_xsr(void)
{
	disable_htb();
	// EPC could be located later than the actual exception PC bcz the WB bus excetion happens later than CPU execution stage @EXR1K
	printf("CPU%d: Bus error exception occurred @ EPC = 0x%08x, EEAR = 0x%08x \r\n", CpuIdGet(), mfspr(EXR1K_EPCR),mfspr(EXR1K_EEAR));
	printf("Enable JTAG control exceptions and check the trace buffer! \r\n");

	if(mfspr(EXR1K_IWSR)) 		print_hwp_info(EXR1K_SPRGID_IWB);
	else if(mfspr(EXR1K_DWSR)) 	print_hwp_info(EXR1K_SPRGID_DWB);
	else 						TRACE("Unknown bus error!!\r\n");

	exit(-1);
}

//------------------------------------------------------------------------------
// During running the code, possible to change SR @SYACALL only
void syscall_xsr(void)
{
	uint esr = mfspr(EXR1K_ESR);
	uint epcr = mfspr(EXR1K_EPCR);

	// Get 16 bit syscall number
	uint insn = *(uint*)(epcr-4);
	uint val = insn & 0xffff;

	if(val&SYS_SR_TED) esr &= ~EXR1K_SR_TEE;	//	TE disable
	if(val&SYS_SR_TEE) esr |=  EXR1K_SR_TEE;	//	TE enable
	if(val&SYS_SR_IED) esr &= ~EXR1K_SR_IEE;	//	IE disable
	if(val&SYS_SR_IEE) esr |=  EXR1K_SR_IEE;	//	IE enable

	mtspr(EXR1K_ESR, esr);	
}


//******************************************************************************
//
//------------------------------------------------------------------------------
_NOINS void sys_enable_timer(void)		{	sys_call(SYS_SR_TEE); }
_NOINS void sys_disable_timer(void)		{	sys_call(SYS_SR_TED); }
                                            	
_NOINS void sys_enable_ext_int(void)	{	sys_call(SYS_SR_IEE); }
_NOINS void sys_disable_ext_int(void)	{	sys_call(SYS_SR_IED); }

_NOINS void sys_enable_int(void)		{	sys_call(SYS_SR_IEE|SYS_SR_TEE); }
_NOINS void sys_disable_int(void)		{	sys_call(SYS_SR_IED|SYS_SR_TED); }
