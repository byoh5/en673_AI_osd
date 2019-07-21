//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_sprs.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150724		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef	_EXR1K_SPRS_H_		
#define	_EXR1K_SPRS_H_

#define	ADDR_MASK		0xFFFFFFFC

//******************************************************************************
// Exceptions
//------------------------------------------------------------------------------
#define XSR_NONE		0
#define XSR_RESET		1
#define XSR_BUSERR		2			//Insn./ Data
#define XSR_DPF			3
#define XSR_IPF			4
#define XSR_TICK		5
#define XSR_DALIGN		6
#define XSR_ILLEGAL		7
#define XSR_INT			8
#define XSR_DTLBMISS	9
#define XSR_ITLBMISS	10
#define XSR_RANGE		11
#define XSR_SYSCALL		12
#define XSR_FLOAT		13
#define XSR_TRAP		14
#define XSR_UNUSED		15


//******************************************************************************
// DU Test groups
//------------------------------------------------------------------------------
#define	EXR1K_DUTGCS_W				4
//#define	EXR1K_DUTGCS_BITS		19:16

#define	EXR1K_DUT_SPR_BITS			0			// SPR
#define	EXR1K_DUT_DRF_BITS			1			// Direct access RF
#define	EXR1K_DUT_DTB_BITS			2			// Direct access tract buffer
#define	EXR1K_DUT_IDC_BITS			3			// IC/DC

// group select
#define	EXR1K_DUTGCS_SPR			(1<<EXR1K_DUT_SPR_BITS)
#define	EXR1K_DUTGCS_DRF			(1<<EXR1K_DUT_DRF_BITS)
#define	EXR1K_DUTGCS_DTB			(1<<EXR1K_DUT_DTB_BITS)
#define	EXR1K_DUTGCS_IDC			(1<<EXR1K_DUT_IDC_BITS)

#define	EXR1K_DUTGCS(ADR)			(((ADR)>>16) & 0x7)


//******************************************************************************
// SPR groups
//------------------------------------------------------------------------------
#define	EXR1K_SPRGID_W				5				// Group index width
#define EXR1K_SPRRID_W 				11				// Register index width

// Group index
#define EXR1K_SPRGID_SYS			( 0<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_DMMU			( 1<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_IMMU			( 2<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_DC				( 3<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_IC				( 4<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_MAC			( 5<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_DU				( 6<< EXR1K_SPRRID_W)
//#define EXR1K_SPRGID_PERF			( 7<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_PM				( 8<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_PIC			( 9<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_TT				(10<< EXR1K_SPRRID_W)
#define EXR1K_SPRGID_FPU			(11<< EXR1K_SPRRID_W)
#define	EXR1K_SPRGID_IWB			(12<< EXR1K_SPRRID_W)
#define	EXR1K_SPRGID_DWB			(13<< EXR1K_SPRRID_W)
#define	EXR1K_SPRGID_TB				(14<< EXR1K_SPRRID_W)


//******************************************************************************
// GID1: DMMU
//------------------------------------------------------------------------------
//#define EXR1K_DMMUCR				(EXR1K_SPRGID_DMMU + 0)
#define EXR1K_DTLBMR_BASE(WAY)		(EXR1K_SPRGID_DMMU + 0x200 + (WAY) * 0x100)
#define EXR1K_DTLBMR_LAST(WAY)		(EXR1K_SPRGID_DMMU + 0x27f + (WAY) * 0x100)
#define EXR1K_DTLBTR_BASE(WAY)		(EXR1K_SPRGID_DMMU + 0x280 + (WAY) * 0x100)
#define EXR1K_DTLBTR_LAST(WAY)		(EXR1K_SPRGID_DMMU + 0x2ff + (WAY) * 0x100)

// Data MMU Control Register


// Data TLB Match Register
#define EXR1K_DTLBMR_V	  			0x00000001  // Valid
#define EXR1K_DTLBMR_LRU	   		0x000000c0  // Least Recently Used
#define EXR1K_DTLBMR_VPN	   		0xffffe000  // Virtual Page Number

// Data TLB Translate Register
#define EXR1K_DTLBTR_CI	   			0x00000002  // Cache Inhibit 
#define EXR1K_DTLBTR_URE	   		0x00000040  // User Read Enable 
#define EXR1K_DTLBTR_UWE	   		0x00000080  // User Write Enable
#define EXR1K_DTLBTR_SRE	   		0x00000100  // Supervisor Read Enable
#define EXR1K_DTLBTR_SWE	   		0x00000200  // Supervisor Write Enable
#define EXR1K_DTLBTR_PPN	   		0xffffe000  // Physical Page Number 

#define EXR1K_DTLB_PR_NOLIMIT 		(EXR1K_DTLBTR_URE|EXR1K_DTLBTR_UWE|EXR1K_DTLBTR_SRE|EXR1K_DTLBTR_SWE)


//******************************************************************************
// GID2: IMMU
//------------------------------------------------------------------------------
//#define EXR1K_IMMUCR				(EXR1K_SPRGID_IMMU + 0)
#define EXR1K_ITLBMR_BASE(WAY)		(EXR1K_SPRGID_IMMU + 0x200 + (WAY) * 0x100)
#define EXR1K_ITLBMR_LAST(WAY)		(EXR1K_SPRGID_IMMU + 0x27f + (WAY) * 0x100)
#define EXR1K_ITLBTR_BASE(WAY)		(EXR1K_SPRGID_IMMU + 0x280 + (WAY) * 0x100)
#define EXR1K_ITLBTR_LAST(WAY)		(EXR1K_SPRGID_IMMU + 0x2ff + (WAY) * 0x100)

// Instruction MMU Control Register


// Instruction TLB Match Register
#define EXR1K_ITLBMR_V	   			0x00000001  // Valid 
#define EXR1K_ITLBMR_LRU	   		0x000000c0  // Least Recently Used 
#define EXR1K_ITLBMR_VPN	   		0xffffe000  // Virtual Page Number 

// Instruction TLB Translate Register
#define EXR1K_ITLBTR_CI	   			0x00000002  // Cache Inhibit 
#define EXR1K_ITLBTR_SXE	   		0x00000040  // User Read Enable 
#define EXR1K_ITLBTR_UXE	   		0x00000080  // User Write Enable 
#define EXR1K_ITLBTR_PPN	   		0xffffe000  // Physical Page Number 

#define EXR1K_ITLB_PR_NOLIMIT  		(EXR1K_ITLBTR_SXE|EXR1K_ITLBTR_UXE)


//******************************************************************************
// GID3: Data cache (DC)
//------------------------------------------------------------------------------
//#define EXR1K_DCCR				(EXR1K_SPRGID_DC + 0)
//#define EXR1K_DCBPR				(EXR1K_SPRGID_DC + 1)
#define EXR1K_DCBFR					(EXR1K_SPRGID_DC + 2)
#define EXR1K_DCBIR					(EXR1K_SPRGID_DC + 3)
//#define EXR1K_DCBWR				(EXR1K_SPRGID_DC + 4)
//#define EXR1K_DCBLR				(EXR1K_SPRGID_DC + 5)
#define EXR1K_DCR_BASE(WAY)			(EXR1K_SPRGID_DC + 0x200 + (WAY) * 0x200)
#define EXR1K_DCR_LAST(WAY)			(EXR1K_SPRGID_DC + 0x3ff + (WAY) * 0x200)


// Data Cache Control register
//#define EXR1K_DCCR_EW	   			0x000000ff  // Enable ways 


//******************************************************************************
// GID4: Instruction cache (IC)
//------------------------------------------------------------------------------
//#define EXR1K_ICCR				(EXR1K_SPRGID_IC + 0)
//#define EXR1K_ICBPR				(EXR1K_SPRGID_IC + 1)
#define EXR1K_ICBIR					(EXR1K_SPRGID_IC + 2)
//#define EXR1K_ICBLR				(EXR1K_SPRGID_IC + 3)
#define EXR1K_ICR_BASE(WAY)			(EXR1K_SPRGID_IC + 0x200 + (WAY) * 0x200)
#define EXR1K_ICR_LAST(WAY)			(EXR1K_SPRGID_IC + 0x3ff + (WAY) * 0x200)

// Insn Cache Control register
//#define EXR1K_ICCR_EW	   			0x000000ff  // Enable ways 


//******************************************************************************
// GID5: MAC
//------------------------------------------------------------------------------
#define EXR1K_MACLO 				(EXR1K_SPRGID_MAC + 1)
#define EXR1K_MACHI 				(EXR1K_SPRGID_MAC + 2)


//******************************************************************************
// GID6: Debug
//------------------------------------------------------------------------------
#define EXR1K_DMR1					(EXR1K_SPRGID_DU + 16)
#define EXR1K_DMR2					(EXR1K_SPRGID_DU + 17)
#define EXR1K_DSR					(EXR1K_SPRGID_DU + 20)
#define EXR1K_DRR					(EXR1K_SPRGID_DU + 21)


// Debug Control registers

// DCR: Comparison Condition

// DCR: Compare To

// Debug Mode 1 register
#define EXR1K_DMR1_ST	  			0x00400000  // Single-step trace
#define EXR1K_DMR1_BT	  			0x00800000  // Branch trace 
#define	EXR1K_DMR1_NE				0x04000000	// NOP enable @Single-step trace

// Debug Mode 2 register. AWTC and WGB corrected by JPB

// Debug watchpoint counter registers

// Debug stop register
#define EXR1K_DSR_RSTE				0x00000001  // [0] Reset exception 
#define EXR1K_DSR_BUSEE				0x00000002  // [1] Bus error exception 
#define EXR1K_DSR_DPFE				0x00000004  // [2] Data Page Fault exception 
#define EXR1K_DSR_IPFE				0x00000008  // [3] Insn Page Fault exception 
#define EXR1K_DSR_TTE				0x00000010  // [4] Tick Timer exception 
#define EXR1K_DSR_AE				0x00000020  // [5] Alignment exception 
#define EXR1K_DSR_IIE				0x00000040  // [6] Illegal Instruction exception 
#define EXR1K_DSR_IE				0x00000080  // [7] Interrupt exception 
#define EXR1K_DSR_DME				0x00000100  // [8] DTLB miss exception 
#define EXR1K_DSR_IME				0x00000200  // [9] ITLB miss exception 
#define EXR1K_DSR_RE				0x00000400  // [10] Range exception 
#define EXR1K_DSR_SCE				0x00000800  // [11] System call exception 
#define EXR1K_DSR_FPE     			0x00001000  // [12] Floating Point Exception 
#define EXR1K_DSR_TE				0x00002000  // [13] Trap exception
#define	EXR1K_DSR_TET				0x00004000	// [14] Ticktimer Exception Trace Enable: If SR[TEE],ESR[TEE] disable incorrectly, stall CPU
#define	EXR1K_DSR_IET				0x00008000	// [15] Interrupt Exception Trace Enable: If SR[IEE],ESR[TEE] disable incorrectly, stall CPU
#define	EXR1K_DSR_MET				0x00010000	// [16] Multiple Exception Trace

// Debug reason register
#define EXR1K_DRR_RSTE				0x00000001  // [0] Reset exception                 
#define EXR1K_DRR_BUSEE				0x00000002  // [1] Bus error exception             
#define EXR1K_DRR_DPFE				0x00000004  // [2] Data Page Fault exception       
#define EXR1K_DRR_IPFE				0x00000008  // [3] Insn Page Fault exception       
#define EXR1K_DRR_TTE				0x00000010  // [4] Tick Timer exception            
#define EXR1K_DRR_AE				0x00000020  // [5] Alignment exception             
#define EXR1K_DRR_IIE				0x00000040  // [6] Illegal Instruction exception   
#define EXR1K_DRR_IE				0x00000080  // [7] Interrupt exception             
#define EXR1K_DRR_DME				0x00000100  // [8] DTLB miss exception             
#define EXR1K_DRR_IME				0x00000200  // [9] ITLB miss exception             
#define EXR1K_DRR_RE				0x00000400  // [10] Range exception                
#define EXR1K_DRR_SCE				0x00000800  // [11] System call exception          
#define EXR1K_DRR_FPE     			0x00001000  // [12] Floating Point Exception       
#define EXR1K_DRR_TE				0x00002000  // [13] Trap exception                 
#define	EXR1K_DSR_TET				0x00004000	// [14] Ticktimer Exception Trace Enable: If SR[TEE],ESR[TEE] disable incorrectly, stall CPU
#define	EXR1K_DSR_IET				0x00008000	// [15] Interrupt Exception Trace Enable: If SR[IEE],ESR[TEE] disable incorrectly, stall CPU
#define	EXR1K_DSR_MET				0x00010000	// [16] Multiple Exception Trace
#define	EXR1K_DSR_ET				0x00020000	// [17] Exception Trace


//******************************************************************************
// GID7: Performance Counters
//------------------------------------------------------------------------------
//#define EXR1K_PERFCR(N)			(EXR1K_SPRGID_PERF + (N))
//#define EXR1K_PERFMR(N)			(EXR1K_SPRGID_PERF + 8 + (N))

// Performance counters mode registers


//******************************************************************************
// GID8: Power Management (PM)
//------------------------------------------------------------------------------
#define EXR1K_PMR 					(EXR1K_SPRGID_PM + 0)

// Power management register
#define EXR1K_PMR_SDF				0x0000000f  // Slow down factor 
#define EXR1K_PMR_DME				0x00000010  // Doze mode enable 
#define EXR1K_PMR_SME				0x00000020  // Sleep mode enable 
#define EXR1K_PMR_DCGE				0x00000040  // Dynamic clock gating enable 
#define EXR1K_PMR_SUME				0x00000080  // Suspend mode enable 


//******************************************************************************
// GID9: PIC
//------------------------------------------------------------------------------
#define EXR1K_PICMR 				(EXR1K_SPRGID_PIC + 0)
#define EXR1K_PICPR 				(EXR1K_SPRGID_PIC + 1)
#define EXR1K_PICSR 				(EXR1K_SPRGID_PIC + 2)

// Fields
#define EXR1K_PICMR_IUM				0xfffffffc  // Interrupt unmask 
#define EXR1K_PICPR_IPRIO			0xfffffffc  // Interrupt priority 
#define EXR1K_PICSR_IS				0xffffffff  // Interrupt status 


//******************************************************************************
// GID10: Tick Timer
//------------------------------------------------------------------------------
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


//******************************************************************************
// GID12,13: WB match
//------------------------------------------------------------------------------
#define	EXR1K_WVR(GID,N)			(GID + (N))
#define	EXR1K_WCR(GID,N)			(GID + 8 + (N))
#define	EXR1K_WMR(GID)				(GID + 16)
#define	EXR1K_WSR(GID)				(GID + 17)
#define	EXR1K_WDTR(GID)				(GID + 18)
#define	EXR1K_WAR(GID)				(GID + 19)

#define EXR1K_IWVR(N)				EXR1K_WVR(EXR1K_SPRGID_IWB,N)
#define EXR1K_IWCR(N)				EXR1K_WCR(EXR1K_SPRGID_IWB,N)
#define EXR1K_IWMR					EXR1K_WMR(EXR1K_SPRGID_IWB)	
#define EXR1K_IWSR					EXR1K_WSR(EXR1K_SPRGID_IWB)	
#define EXR1K_IWDTR					EXR1K_WDTR(EXR1K_SPRGID_IWB)	
#define EXR1K_IWAR					EXR1K_WAR(EXR1K_SPRGID_IWB)	

#define EXR1K_DWVR(N)				EXR1K_WVR(EXR1K_SPRGID_DWB,N)
#define EXR1K_DWCR(N)				EXR1K_WCR(EXR1K_SPRGID_DWB,N)
#define EXR1K_DWMR					EXR1K_WMR(EXR1K_SPRGID_DWB)	
#define EXR1K_DWSR					EXR1K_WSR(EXR1K_SPRGID_DWB)	
#define EXR1K_DWDTR					EXR1K_WDTR(EXR1K_SPRGID_DWB)	
#define EXR1K_DWAR					EXR1K_WAR(EXR1K_SPRGID_DWB)	

//------------------------------------------------------------------------------
// Watch Control registers
#define	EXR1K_WCR_CC_W				3
#define	EXR1K_WCR_CT_W				3

#define	EXR1K_WCR_CC_LSB			0
#define	EXR1K_WCR_CC_MSB			2
#define	EXR1K_WCR_CT_LSB			3
#define	EXR1K_WCR_CT_MSB			5

// WCR: Comparison Condition
#define EXR1K_WCR_CC_DISABLE 		0
#define EXR1K_WCR_CC_EQ  			1
#define EXR1K_WCR_CC_LT   			2
#define EXR1K_WCR_CC_LE  			3
#define EXR1K_WCR_CC_GT  			4
#define EXR1K_WCR_CC_GE 			5
#define EXR1K_WCR_CC_NEQ 			6

// WCR: Compare To
#define EXR1K_WCR_CT_DISABLE 		(0 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_LD_DAT    		(1 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_ST_DAT     	(2 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_LS_DAT	    	(3 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_RES1    		(4 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_LD_ADR     	(5 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_ST_ADR     	(6 << EXR1K_WCR_CC_W)
#define EXR1K_WCR_CT_LS_ADR     	(7 << EXR1K_WCR_CC_W)


//------------------------------------------------------------------------------
// Watch Mode register
#define	EXR1K_WMR_CW_W				2

#define	EXR1K_WMR_CW_DISABLE		0x0
#define EXR1K_WMR_CW_MATCH			0x1			// single match
#define EXR1K_WMR_CW_AND_PCHAIN		0x2			// and the prev. chain watch
#define	EXR1K_WMR_CW_OR_PCHAIN		0x3			// or the prev. chain watch

// Bit fields
#define	EXR1K_WMR_BEE				0x00000001	// [0] Bus Error Enable
#define	EXR1K_WMR_RES0				0x00000002	// reserved
#define	EXR1K_WMR_WDTE				0x00000004	// [2] Watchdog Timer Enable
#define	EXR1K_WMR_OSCE				0x00000008	// [3] Output Signal Check Enable
#define	EXR1K_WMR_ISCE				0x00000010	// [4] Input Signal Check Enable
#define	EXR1K_WMR_ESCE				0x00000020	// [5] Error Signal Check Enable
#define	EXR1K_WMR_CW0_DISABLE		(EXR1K_WMR_CW_DISABLE		<< ( 0+6))
#define EXR1K_WMR_CW0_MATCH			(EXR1K_WMR_CW_MATCH			<< ( 0+6))    
#define EXR1K_WMR_CW0_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< ( 0+6))		
#define	EXR1K_WMR_CW0_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< ( 0+6))		
#define	EXR1K_WMR_CW1_DISABLE		(EXR1K_WMR_CW_DISABLE		<< ( 2+6))
#define EXR1K_WMR_CW1_MATCH			(EXR1K_WMR_CW_MATCH			<< ( 2+6))    
#define EXR1K_WMR_CW1_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< ( 2+6))		
#define	EXR1K_WMR_CW1_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< ( 2+6))		
#define	EXR1K_WMR_CW2_DISABLE		(EXR1K_WMR_CW_DISABLE		<< ( 4+6))
#define EXR1K_WMR_CW2_MATCH			(EXR1K_WMR_CW_MATCH			<< ( 4+6))    
#define EXR1K_WMR_CW2_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< ( 4+6))		
#define	EXR1K_WMR_CW2_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< ( 4+6))		
#define	EXR1K_WMR_CW3_DISABLE		(EXR1K_WMR_CW_DISABLE		<< ( 6+6))
#define EXR1K_WMR_CW3_MATCH			(EXR1K_WMR_CW_MATCH			<< ( 6+6))    
#define EXR1K_WMR_CW3_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< ( 6+6))		
#define	EXR1K_WMR_CW3_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< ( 6+6))		
#define	EXR1K_WMR_CW4_DISABLE		(EXR1K_WMR_CW_DISABLE		<< ( 8+6))
#define EXR1K_WMR_CW4_MATCH			(EXR1K_WMR_CW_MATCH			<< ( 8+6))    
#define EXR1K_WMR_CW4_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< ( 8+6))		
#define	EXR1K_WMR_CW4_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< ( 8+6))		
#define	EXR1K_WMR_CW5_DISABLE		(EXR1K_WMR_CW_DISABLE		<< (10+6))
#define EXR1K_WMR_CW5_MATCH			(EXR1K_WMR_CW_MATCH			<< (10+6))    
#define EXR1K_WMR_CW5_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< (10+6))		
#define	EXR1K_WMR_CW5_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< (10+6))		
#define	EXR1K_WMR_CW6_DISABLE		(EXR1K_WMR_CW_DISABLE		<< (12+6))
#define EXR1K_WMR_CW6_MATCH			(EXR1K_WMR_CW_MATCH			<< (12+6))    
#define EXR1K_WMR_CW6_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< (12+6))		
#define	EXR1K_WMR_CW6_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< (12+6))		
#define	EXR1K_WMR_CW7_DISABLE		(EXR1K_WMR_CW_DISABLE		<< (14+6))
#define EXR1K_WMR_CW7_MATCH			(EXR1K_WMR_CW_MATCH			<< (14+6))    
#define EXR1K_WMR_CW7_AND_PCHAIN	(EXR1K_WMR_CW_AND_PCHAIN	<< (14+6))		
#define	EXR1K_WMR_CW7_OR_PCHAIN		(EXR1K_WMR_CW_OR_PCHAIN		<< (14+6))		

#define EXR1K_WMR_CW0_LSB			6
#define	EXR1K_WMR_CW0_MSB			7

//------------------------------------------------------------------------------
// Watch Status register
#define EXR1K_WSR_WDT				0x00000001  // WatchDog timer Status
#define EXR1K_WSR_OSC				0x00000002  // Output Signal Check Status
#define EXR1K_WSR_ISC				0x00000004  // Input Signal Check Status
#define EXR1K_WSR_ESC				0x00000008  // Error Signal Check Status
#define EXR1K_WSR_CW0				0x00000010  // CW0 status
#define EXR1K_WSR_CW1				0x00000020  // CW1 status
#define EXR1K_WSR_CW2				0x00000040  // CW2 status
#define EXR1K_WSR_CW3				0x00000080  // CW3 status
#define EXR1K_WSR_CW4				0x00000100  // CW4 status
#define EXR1K_WSR_CW5				0x00000200  // CW5 status
#define EXR1K_WSR_CW6				0x00000400  // CW6 status
#define EXR1K_WSR_CW7				0x00000800  // CW7 status

// bit fields
#define	EXR1K_WST_CW0_BIT			4

//******************************************************************************
// GID14: Hardware Trace Buffer
//------------------------------------------------------------------------------
#define	EXR1K_HTB_AW				8

#define	EXR1K_HTBAR					(EXR1K_SPRGID_TB + 0x000 + 0xff)	// TB: next address to write
#define	EXR1K_HTBCM_BASE			(EXR1K_SPRGID_TB + 0x100)			// TB: PC memory
#define	EXR1K_HTBIM_BASE			(EXR1K_SPRGID_TB + 0x200)			// TB: Insn memory
#define	EXR1K_HTBDM_BASE			(EXR1K_SPRGID_TB + 0x300)			// TB: Data memory
#define	EXR1K_HTBSM_BASE			(EXR1K_SPRGID_TB + 0x400)			// TB: Status memory
#define EXR1K_HTBTR 				(EXR1K_SPRGID_TB + 0x600)			// TB: Timestamp register
#define EXR1K_HTBCR 				(EXR1K_SPRGID_TB + 0x700)			// TB: Control register

// TB control register
#define EXR1K_HTBCR_EN	  			0x00000001  // [0] Enable
#define	EXR1K_HTBCR_NE				0x00000002	// [1] NOP enable 
#define	EXR1K_HTBCR_TE				0x00000004	// [2] Timestamp enable
//#define EXR1K_HTBCR_RES0			0x00000008	// [3] reserved
#define	EXR1K_HTBCR_TS_EX			0x00000000	// [5:4] trace select = EX
//#define EXR1K_HTBCR_TS_ID			0x00000010	// [5:4] trace select = ID
#define	EXR1K_HTBCR_TS_IF			0x00000020	// [5:4] trace select = IF
//#define EXR1K_HTBCR_TS_LSU		0x00000030	// [5:4] trace select = LSU
//#define EXR1K_HTBCR_DS0						// [7:6] data select 0
//#define EXR1K_HTBCR_DS1						// [9:8] data select 1
#define	EXR1K_HTBCR_DS2_NPC			0x00000000	// [11:10] data select 2
#define	EXR1K_HTBCR_DS2_MDATI		0x00000400	// [11:10] data select 2
#define	EXR1K_HTBCR_DS2_PC			0x00000c00	// [11:10] data select 2
//#define EXR1K_HTBCR_DS3						// [13:12] data select 3


//******************************************************************************
// GID0: System registers (SYSR)												
//------------------------------------------------------------------------------
#define EXR1K_VR					(EXR1K_SPRGID_SYS + 0)
#define EXR1K_UPR					(EXR1K_SPRGID_SYS + 1)
#define EXR1K_CPUCFGR				(EXR1K_SPRGID_SYS + 2)
#define EXR1K_DMMUCFGR				(EXR1K_SPRGID_SYS + 3)
#define EXR1K_IMMUCFGR				(EXR1K_SPRGID_SYS + 4)
#define EXR1K_DCCFGR				(EXR1K_SPRGID_SYS + 5)
#define EXR1K_ICCFGR				(EXR1K_SPRGID_SYS + 6)
#define EXR1K_DUCFGR				(EXR1K_SPRGID_SYS + 7)
#define EXR1K_PCCFGR				(EXR1K_SPRGID_SYS + 8)
#define EXR1K_NPC         			(EXR1K_SPRGID_SYS + 16)  
#define EXR1K_SR					(EXR1K_SPRGID_SYS + 17)  
#define EXR1K_PPC         			(EXR1K_SPRGID_SYS + 18)  
#define EXR1K_FPCSR       			(EXR1K_SPRGID_SYS + 20)  
#define EXR1K_ISR       			(EXR1K_SPRGID_SYS + 21)
#define EXR1K_CSR					(EXR1K_SPRGID_SYS + 31)
#define EXR1K_EPCR					(EXR1K_SPRGID_SYS + 32)  
#define EXR1K_EEAR					(EXR1K_SPRGID_SYS + 48)
#define EXR1K_ESR					(EXR1K_SPRGID_SYS + 64)
#define	EXR1K_RF0					(EXR1K_SPRGID_SYS + 0x400)

//------------------------------------------------------------------------------
// Version Register
#define EXR1K_VR_VER				0xff000000  // Processor version 
#define EXR1K_VR_CFG				0x00ff0000  // Processor configuration 
#define EXR1K_VR_RES				0x00ff0000  // Reserved 
#define EXR1K_VR_REV				0x0000003f  // Processor revision 

#define EXR1K_VR_VER_OFF			24
#define EXR1K_VR_CFG_OFF			16
#define EXR1K_VR_REV_OFF			0


// Unit Present Register
#define EXR1K_UPR_UP	   			0x00000001  // UPR present 
#define EXR1K_UPR_DCP	   			0x00000002  // Data cache present 
#define EXR1K_UPR_ICP	   			0x00000004  // Instruction cache present 
#define EXR1K_UPR_DMP	   			0x00000008  // Data MMU present 
#define EXR1K_UPR_IMP	   			0x00000010  // Instruction MMU present 
#define EXR1K_UPR_MP	   			0x00000020  // MAC present 
#define EXR1K_UPR_DUP	   			0x00000040  // Debug unit present 
#define EXR1K_UPR_PCUP	   			0x00000080  // Performance counters unit present 
#define EXR1K_UPR_PMP	   			0x00000100  // Power management present 
#define EXR1K_UPR_PICP	   			0x00000200  // PIC present 
#define EXR1K_UPR_TTP	   			0x00000400  // Tick timer present 
#define	EXR1K_UPR_FPP				0x00000800	// FPU present
#define	EXR1K_UPR_TB				0x00001000	// TB present
#define EXR1K_UPR_RES	   			0x00fe0000  // Reserved 
#define EXR1K_UPR_CUP	   			0xff000000  // Context units present 

// CPU configuration register
#define EXR1K_CPUCFGR_NSGF   		0x0000000f  // Number of shadow GPR files 
#define EXR1K_CPUCFGR_CGF	   		0x00000010  // Custom GPR file 
#define EXR1K_CPUCFGR_OB32S  		0x00000020  // ORBIS32 supported 
#define EXR1K_CPUCFGR_OB64S  		0x00000040  // ORBIS64 supported 
#define EXR1K_CPUCFGR_OF32S  		0x00000080  // ORFPX32 supported 
#define EXR1K_CPUCFGR_OF64S  		0x00000100  // ORFPX64 supported 
#define EXR1K_CPUCFGR_OV64S  		0x00000200  // ORVDX64 supported 
#define EXR1K_CPUCFGR_RES	   		0xfffffc00  // Reserved 

// Data MMU Configuration Register
#define EXR1K_DMMUCFGR_NTW			0x00000003
#define EXR1K_DMMUCFGR_NTS			0x0000001C
#define EXR1K_DMMUCFGR_NAE			0x000000E0
#define EXR1K_DMMUCFGR_CRI			0x00000100
#define EXR1K_DMMUCFGR_PRI        	0x00000200
#define EXR1K_DMMUCFGR_TEIRI		0x00000400
#define EXR1K_DMMUCFGR_HTR			0x00000800

#define EXR1K_DMMUCFGR_NTW_OFF		0
#define EXR1K_DMMUCFGR_NTS_OFF		2

// Instruction MMU Configuration Register
#define EXR1K_IMMUCFGR_NTW			0x00000003
#define EXR1K_IMMUCFGR_NTS			0x0000001C
#define EXR1K_IMMUCFGR_NAE			0x000000E0
#define EXR1K_IMMUCFGR_CRI			0x00000100
#define EXR1K_IMMUCFGR_PRI			0x00000200
#define EXR1K_IMMUCFGR_TEIRI		0x00000400
#define EXR1K_IMMUCFGR_HTR			0x00000800

#define EXR1K_IMMUCFGR_NTW_OFF		0
#define EXR1K_IMMUCFGR_NTS_OFF		2

// Data Cache Configuration Register
#define EXR1K_DCCFGR_NCW			0x00000007
#define EXR1K_DCCFGR_NCS			0x00000078
#define EXR1K_DCCFGR_CBS			0x00000080
#define EXR1K_DCCFGR_CWS			0x00000100
#define EXR1K_DCCFGR_CCRI			0x00000200
#define EXR1K_DCCFGR_CBIRI			0x00000400
#define EXR1K_DCCFGR_CBPRI			0x00000800
#define EXR1K_DCCFGR_CBLRI			0x00001000
#define EXR1K_DCCFGR_CBFRI			0x00002000
#define EXR1K_DCCFGR_CBWBRI			0x00004000

#define EXR1K_DCCFGR_NCW_OFF      	0
#define EXR1K_DCCFGR_NCS_OFF      	3
#define EXR1K_DCCFGR_CBS_OFF		7

// Instruction Cache Configuration Register
#define EXR1K_ICCFGR_NCW			0x00000007
#define EXR1K_ICCFGR_NCS			0x00000078
#define EXR1K_ICCFGR_CBS			0x00000080
#define EXR1K_ICCFGR_CCRI			0x00000200
#define EXR1K_ICCFGR_CBIRI			0x00000400
#define EXR1K_ICCFGR_CBPRI			0x00000800
#define EXR1K_ICCFGR_CBLRI			0x00001000
#define	EXR1K_ICCFGR_CBFRI			0x00002000

#define EXR1K_ICCFGR_NCW_OFF      	0
#define EXR1K_ICCFGR_NCS_OFF      	3
#define EXR1K_ICCFGR_CBS_OFF		7

// Debug unit configuration register
#define EXR1K_DUCFGR_NDP      		0x00000007  // Number of matchpoints mask 
#define EXR1K_DUCFGR_NDP1     		0x00000000  // One matchpoint supported 
#define EXR1K_DUCFGR_NDP2     		0x00000001  // Two matchpoints supported 
#define EXR1K_DUCFGR_NDP3     		0x00000002  // Three matchpoints supported 
#define EXR1K_DUCFGR_NDP4     		0x00000003  // Four matchpoints supported 
#define EXR1K_DUCFGR_NDP5     		0x00000004  // Five matchpoints supported 
#define EXR1K_DUCFGR_NDP6     		0x00000005  // Six matchpoints supported 
#define EXR1K_DUCFGR_NDP7     		0x00000006  // Seven matchpoints supported 
#define EXR1K_DUCFGR_NDP8     		0x00000007  // Eight matchpoints supported 
#define EXR1K_DUCFGR_WPCI     		0x00000008  // Watchpoint counters implemented 

#define EXR1K_MATCHPOINTS_TO_NDP(n)	(1 == n ? EXR1K_DUCFGR_NDP1 : \
                              		 2 == n ? EXR1K_DUCFGR_NDP2 : \
                              		 3 == n ? EXR1K_DUCFGR_NDP3 : \
                              		 4 == n ? EXR1K_DUCFGR_NDP4 : \
                              		 5 == n ? EXR1K_DUCFGR_NDP5 : \
                              		 6 == n ? EXR1K_DUCFGR_NDP6 : \
                               		 7 == n ? EXR1K_DUCFGR_NDP7 : EXR1K_DUCFGR_NDP8)
#define EXR1K_MAX_MATCHPOINTS  		8
#define EXR1K_MAX_WATCHPOINTS  		(EXR1K_MAX_MATCHPOINTS + 2)

// Supervision Register
#define EXR1K_SR_SM          		0x00000001  // Supervisor Mode 
#define EXR1K_SR_TEE         		0x00000002  // Tick timer Exception Enable 
#define EXR1K_SR_IEE         		0x00000004  // Interrupt Exception Enable 
#define EXR1K_SR_DCE         		0x00000008  // Data Cache Enable 
#define EXR1K_SR_ICE         		0x00000010  // Instruction Cache Enable 
#define EXR1K_SR_DME         		0x00000020  // Data MMU Enable 
#define EXR1K_SR_IME         		0x00000040  // Instruction MMU Enable 
#define EXR1K_SR_LEE         		0x00000080  // Little Endian Enable 
#define EXR1K_SR_CE          		0x00000100  // CID Enable 
#define EXR1K_SR_F           		0x00000200  // Condition Flag 
#define EXR1K_SR_CY          		0x00000400  // Carry flag 
#define EXR1K_SR_OV          		0x00000800  // Overflow flag 
#define EXR1K_SR_OVE         		0x00001000  // Overflow flag Exception 
#define EXR1K_SR_DSX         		0x00002000  // Delay Slot Exception 
#define EXR1K_SR_EPH         		0x00004000  // Exception Prefix High 
#define EXR1K_SR_FO          		0x00008000  // Fixed one 
#define EXR1K_SR_TED	       		0x00010000  // Trap exception disable 
#define EXR1K_SR_RES         		0x0ffe0000  // Reserved 
#define EXR1K_SR_CID         		0xf0000000  // Context ID 

// FP Control Status Register
#define EXR1K_FPCSR_FPEE  			0x00000001  // Floating Point Exception Enable 
#define EXR1K_FPCSR_RM    			0x00000006  // Rounding Mode 
#define EXR1K_FPCSR_OVF   			0x00000008  // Overflow Flag 
#define EXR1K_FPCSR_UNF   			0x00000010  // Underflow Flag 
#define EXR1K_FPCSR_SNF   			0x00000020  // SNAN Flag 
#define EXR1K_FPCSR_QNF   			0x00000040  // QNAN Flag 
#define EXR1K_FPCSR_ZF    			0x00000080  // Zero Flag 
#define EXR1K_FPCSR_IXF   			0x00000100  // Inexact Flag 
#define EXR1K_FPCSR_IVF   			0x00000200  // Invalid Flag 
#define EXR1K_FPCSR_INF   			0x00000400  // Infinity Flag 
#define EXR1K_FPCSR_DZF   			0x00000800  // Divide By Zero Flag 
#define EXR1K_FPCSR_ALLF 			(EXR1K_FPCSR_OVF | EXR1K_FPCSR_UNF | EXR1K_FPCSR_SNF | \
									 EXR1K_FPCSR_QNF | EXR1K_FPCSR_ZF  | EXR1K_FPCSR_IXF | \
									 EXR1K_FPCSR_IVF | EXR1K_FPCSR_INF | EXR1K_FPCSR_DZF)

#define	RND_TONEAR					0		// round to nearest
#define	RND_TOZERO					1		// round to zero
#define	RND_UPWARD					2		// round to +inf
#define	RND_DNWARD					3		// round to -inf

#define EXR1K_FPCSR_RM_RN 			(RND_TONEAR<<1)
#define EXR1K_FPCSR_RM_RZ 			(RND_TOZERO<<1)
#define EXR1K_FPCSR_RM_RIP 			(RND_UPWARD<<1)
#define EXR1K_FPCSR_RM_RIN 			(RND_DNWARD<<1)

// Implementation-specific register (ISR): GID0::R21
#define	EXR1K_ISR_RDWE				0x00000001	// Read-During-Write Enable @ RF


//******************************************************************************
// STB: Software Trace Buffer
//------------------------------------------------------------------------------
#define EXR1K_STBCR_STBEN          	0x0001  	// STB Enable
#define EXR1K_STBCR_HTBEN	    	0x0002  	// HTB Enable @STB enabled
//#define EXR1K_STBCR_RES0	    	0x0004  	
//#define EXR1K_STBCR_RES1    	  	0x0008  	
#define EXR1K_STBCR_LRE         	0x0010		// Link Register Exception Enable: LR @enter == LR @exit ?    
#define EXR1K_STBCR_SPE         	0x0020  	// Stack Pointer Exception Enable: SP @enter == SP @exit ?                         
#define EXR1K_STBCR_SPOVE       	0x0040  	// Stack Pointer Overflow Exception Enable: SP in [_stack_e, _stack_s] ?
#define EXR1K_STBCR_FCSE         	0x0080  	// Function Call Stack Exception Enable: Stack overflow?
#define EXR1K_STBCR_XCE           	0x0100  	// Exception Call Exception Enable: Xsr called twice ?                    
#define EXR1K_STBCR_TEE          	0x0200  	// Ticktimer Exception Enable: SR[TEE] @enter == SR[TEE] @exit ?                                                   
#define EXR1K_STBCR_IEE          	0x0400  	// Interrupt Exception Enable: SR[IEE] @enter == SR[IEE] @exit ?
#define EXR1K_STBCR_OV          	0x0800  	// Overflow flag 
#define EXR1K_STBCR_OVE         	0x1000  	// Overflow flag Exception: Ring buffer overflow ? Controlled by User
#define	EXR1K_STBCR_RDW				0x2000		// Ring Data Warning Enable: Ring Data == Stack Data ?


//******************************************************************************
// Single Precision Floating point (IEEE 754)
//
// Format: 11 sign bit, 8 exponent bits and 23 fraction bits.
//		seeeeeeeefffffffffffffffffffffff
//
//	normalized:		V = (-1)^s * 2^(e-127) * 1.f
// 	denormalized:	V = (-1)^s * 2^-126    * 0.f 
//
// Special values:			exp			fraction
//  Zeroes:                	0,    		0
//  Denormalized numbers:	0,   		not 0
//  Infinities:           	2^8 - 1, 	0
//  NaNs:                 	2^8 - 1, 	not 0,
//	 -	quiet NaN:			"			"		MSB(f) = 1, for FP errors
//	 -	signalling NaN:		"			"		MSB(f) = 0,	for unusal purposes		
//
// 	NaNs just propagated unchanged during op's
//------------------------------------------------------------------------------
#define FP_P_ZERO    				0x00000000	// +zero 
#define FP_N_ZERO    				0x80000000	// -zero 
#define FP_P_INF     				0x7f800000	// +inf
#define FP_P_NAN_B   				0x7f800001	// +NaN, not all 1s in frac, > FP_P_INF
#define FP_P_NAN     				0x7fc00000	// +qNaN, MSB(f)=1 
#define FP_N_INF     				0xff800000	// -inf
#define FP_N_NAN_B   				0xff800001	// -NaN, not all 1s in frac, > FP_N_INF
#define FP_N_NAN     				0xffc00000	// -qNaN, MSB(f)=1 

// Some NaNs without all fraction bits set (permitted under IEEE 754) 
                            		
// Some normalized numbers  		
#define FP_0_5      				0x3f000000	// +0.5 
#define FP_0_666667 				0x3f2aaaaa	// +0.666667
#define FP_ONE      				0x3f800000	// +1.0 
#define FP_1_5      				0x3fc00000	// +1.5 
#define FP_TWO      				0x40000000	// +2.0 
#define FP_THREE    				0x40400000	// +3.0 
#define FP_LARGEST_TO_INT 			0x4effffff 	// 2147483520 = 0x0x7FFFFF80
#define FP_N_LARGEST_TO_INT 		0xceffffff 	// -2147483520 = 0x0x80000080*
#define FP_HUGE1    				0x7e800000	// +1.0 * 2^+126 
#define FP_HUGE2    				0x7f000000	// +1.0 * 2^+127 
#define FP_N_0_5    				0xbf000000	// -0.5 
#define FP_N_ONE    				0xbf800000	// -1.0 
#define FP_N_1_5    				0xbfc00000	// -1.5 
#define FP_N_TWO    				0xc0000000	// -2.0 
#define FP_N_THREE  				0xc0400000	// -3.0 
#define FP_N_HUGE1  				0xfe800000	// -1.0 * 2^+126 
#define FP_N_HUGE2  				0xff000000	// -1.0 * 2^+127 
                            		
// Some denormalized numbers		 
#define FP_SMALLEST1 				0x00000001 	// +1.0 * 2^-150 
#define FP_SMALLEST2 				0x00000002 	// +1.0 * 2^-149 
#define FP_SMALL1   				0x00200000	// +1.0 * 2^-129 
#define FP_SMALL2   				0x00400000	// +1.0 * 2^-128 
#define FP_N_SMALLEST1 				0x80000001 	// -1.0 * 2^-150 
#define FP_N_SMALLEST2 				0x80000002 	// -1.0 * 2^-149 
#define FP_N_SMALL1 				0x80200000	// -1.0 * 2^-129 
#define FP_N_SMALL2 				0x80400000	// -1.0 * 2^-128 


#endif//_EXR1K_SPRS_H_