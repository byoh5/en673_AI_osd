//******************************************************************************
//	Copyright(c) Eyenix 2003-
//
//	File Name:		reset.S
//	Description:
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150730		ygkim		first designed
// -----------------------------------------------------------------------------
#include "exr1k_sprs.h"
#include "exr1k_asm.h"
#include "board.h"

		.extern	XsrBusErr
		.extern	XsrDpFault
		.extern	XsrIpFault
		.extern	XsrTick
		.extern	XsrAlign
		.extern	XsrIllInsn
		.extern	XsrInt
		.extern	XsrDtlbMiss
		.extern	XsrItlbMiss
		.extern	vTaskSwitchContext
		.extern	XsrFloatPoint
		.extern XsrSyscall
		.extern	XsrBreak
		.extern	XsrTrap

		.extern	_enable_icache
		.extern	_enable_dcache

		.extern	_init_data_section
		.extern	_init_bss_section

		.extern cache_init

//******************************************************************************
// Reset Section
		.section .reset, "ax"
//------------------------------------------------------------------------------
// Code memory information for boot loader
		.org	0x000

// LMA information
		WORD_ENTRY(_lma_size		)
		WORD_ENTRY(_lma_start		)		// reserved
		WORD_ENTRY(_reset_lma_s		)
		WORD_ENTRY(_reset_lma_e		)
		WORD_ENTRY(_text_lma_s		)
		WORD_ENTRY(_text_lma_e		)
		WORD_ENTRY(_ispm_text_lma_s	)
		WORD_ENTRY(_ispm_text_lma_e	)
		WORD_ENTRY(_ispm_data_lma_s	)
		WORD_ENTRY(_ispm_data_lma_e	)
		WORD_ENTRY(_rodata_lma_s	)
		WORD_ENTRY(_rodata_lma_e	)
		WORD_ENTRY(_data_lma_s		)
		WORD_ENTRY(_data_lma_e		)

// VMA information
		WORD_ENTRY(_reset_s     	)
		WORD_ENTRY(_reset_e     	)
		WORD_ENTRY(_text_s      	)
		WORD_ENTRY(_text_e      	)
		WORD_ENTRY(_ispm_text_s 	)
		WORD_ENTRY(_ispm_text_e 	)
		WORD_ENTRY(_ispm_data_s 	)
		WORD_ENTRY(_ispm_data_e 	)
		WORD_ENTRY(_rodata_s      	)
		WORD_ENTRY(_rodata_e      	)
		WORD_ENTRY(_data_s      	)
		WORD_ENTRY(_data_e      	)
		WORD_ENTRY(_bss_s       	)
		WORD_ENTRY(_bss_e       	)
		WORD_ENTRY(_stack_s       	)
		WORD_ENTRY(_stack_e       	)

		.global _heap_start
		.global _heap_end

_heap_start:
		.long	CPU1_HEAP_BASE

_heap_end:
		.long	(CPU1_HEAP_END-4)

//******************************************************************************
// Exceptions
//------------------------------------------------------------------------------
// Reset
		.org	0x100
ENTRY(_reset)
		l.nop
		CLEAR_ALL_GPRS

	// Jump to _start
		LOAD_CONST_2_GPR(r3, _start)
		l.jr	r3
		l.nop
    	FILL_NOPS


//#############################################################################
// Exception handler
//-----------------------------------------------------------------------------
// Macro
#define PROLOG_IP						;\
		l.addi  r1,r1,-EXCEPTION_STACK_SIZE				;\
		l.sw	0x0c(r1),r3				;\
		l.sw	0x10(r1),r4				;\
		l.sw    0x7c(r1),r9				;\
		LCALL(_save_gpr)				;

#define PROLOG_CONTEXT					;\
		l.mfspr	r3,r0,EXR1K_EPCR				;\
		l.sw	0x04(r1),r3				;\
										;\
		l.mfspr r3,r0,EXR1K_ESR					;\
		l.sw    0x08(r1),r3				;\
										;\
		l.movhi	r3,hi(pxCurrentTCB)		;\
		l.ori 	r3,r3,lo(pxCurrentTCB)	;\
		l.lwz	r3,0x0(r3)				;\
		l.sw 	0x0(r3),r1				;\
										;\
		l.movhi	r3,hi(pXsrStack)		;\
		l.ori 	r3,r3,lo(pXsrStack)		;\
		l.lwz	r1,0x0(r3)				;

#define EPILOG_CONTEXT					;\
		l.movhi	r3,hi(pxCurrentTCB)		;\
		l.ori 	r3,r3,lo(pxCurrentTCB)	;\
		l.lwz	r3,0x0(r3)				;\
		l.lwz	r1,0x0(r3)				;\
										;\
		l.lwz	r3,0x08(r1)				;\
		l.mtspr	r0,r3,EXR1K_ESR					;\
										;\
		l.lwz	r3,0x04(r1)				;\
		l.mtspr	r0,r3,EXR1K_EPCR				;

#define EPILOG_IP						;\
		LCALL(_restore_gpr)				;\
		l.lwz	r3,0x0c(r1)				;\
		l.lwz	r4,0x10(r1)				;\
		l.lwz   r9,0x7c(r1)				;\
		l.addi  r1,r1,EXCEPTION_STACK_SIZE				;\
		l.rfe							;\
		l.nop							;\
		FILL_NOPS						;

//-----------------------------------------------------------------------------
// BUS Error
		.org 0x200
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrBusErr)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Data Page Fault
		.org 0x300
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrDpFault)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Insn Page Fault
		.org 0x400
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrIpFault)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Timer
		.org 0x500
		PROLOG_IP
		PROLOG_CONTEXT
		LCALL(XsrTick)				// Task switch by tick
		EPILOG_CONTEXT
		EPILOG_IP

//-----------------------------------------------------------------------------
// Data Aligment Error
		.org 0x600
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrAlign)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Illegal Insn
		.org 0x700
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrIllInsn)
		EPILOG_IP

//-----------------------------------------------------------------------------
// External Interrupt
		.org 0x800
		PROLOG_IP
		PROLOG_CONTEXT
		LCALL(XsrInt)				// Task switch by isr
		EPILOG_CONTEXT
		EPILOG_IP

//-----------------------------------------------------------------------------
// DTLB miss
		.org 0x900
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrDtlbMiss)
		EPILOG_IP

//-----------------------------------------------------------------------------
// ITLB miss
		.org 0xa00
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrItlbMiss)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Range Error
		.org 0xb00
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrFloatPoint)
		EPILOG_IP

//-----------------------------------------------------------------------------
// System Call
		.org 0xc00
		PROLOG_IP
		PROLOG_CONTEXT
		LCALL(vTaskSwitchContext)	// Task switch by syscall (Yield)
		EPILOG_CONTEXT
		EPILOG_IP

//-----------------------------------------------------------------------------
// Floating-point Error
		.org 0xd00
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrBreak)
		EPILOG_IP

//-----------------------------------------------------------------------------
// Trap
		.org 0xe00
		PROLOG_IP
		SCALL_DEFAULT_XSR
//		LCALL(XsrTrap)
		EPILOG_IP


//******************************************************************************
// GPR functions
// Stack pointer & r3, r4, r9 are already set
//------------------------------------------------------------------------------
ENTRY(_save_gpr)
		l.sw    0x00(r1),r2
//		l.sw    0x10(r1),r4
		l.sw    0x14(r1),r5
		l.sw    0x18(r1),r6
		l.sw    0x1c(r1),r7
		l.sw    0x20(r1),r8
		l.sw    0x24(r1),r10
		l.sw    0x28(r1),r11
		l.sw    0x2c(r1),r12
		l.sw    0x30(r1),r13
		l.sw    0x34(r1),r14
		l.sw    0x38(r1),r15
		l.sw    0x3c(r1),r16
		l.sw    0x40(r1),r17
		l.sw    0x44(r1),r18
		l.sw    0x48(r1),r19
		l.sw    0x4c(r1),r20
		l.sw    0x50(r1),r21
		l.sw    0x54(r1),r22
		l.sw    0x58(r1),r23
		l.sw    0x5c(r1),r24
		l.sw    0x60(r1),r25
		l.sw    0x64(r1),r26
		l.sw    0x68(r1),r27
		l.sw    0x6c(r1),r28
		l.sw    0x70(r1),r29
		l.sw    0x74(r1),r30
		l.sw    0x78(r1),r31
		l.jr	r9
		l.nop

ENTRY(_restore_gpr)
		l.lwz   r31,0x78(r1)
		l.lwz   r30,0x74(r1)
		l.lwz   r29,0x70(r1)
		l.lwz   r28,0x6c(r1)
		l.lwz   r27,0x68(r1)
		l.lwz   r26,0x64(r1)
		l.lwz   r25,0x60(r1)
		l.lwz   r24,0x5c(r1)
		l.lwz   r23,0x58(r1)
		l.lwz   r22,0x54(r1)
		l.lwz   r21,0x50(r1)
		l.lwz   r20,0x4c(r1)
		l.lwz   r19,0x48(r1)
		l.lwz   r18,0x44(r1)
		l.lwz   r17,0x40(r1)
		l.lwz   r16,0x3c(r1)
		l.lwz   r15,0x38(r1)
		l.lwz   r14,0x34(r1)
		l.lwz   r13,0x30(r1)
		l.lwz   r12,0x2c(r1)
		l.lwz   r11,0x28(r1)
		l.lwz   r10,0x24(r1)
		l.lwz   r8 ,0x20(r1)
		l.lwz   r7 ,0x1c(r1)
		l.lwz   r6 ,0x18(r1)
		l.lwz   r5 ,0x14(r1)
//		l.lwz	r4 ,0x10(r1)
		l.lwz   r2 ,0x00(r1)
		l.jr	r9
		l.nop


//******************************************************************************
// Text section
		.section .text
//------------------------------------------------------------------------------
_start:
	// Init SR
	    l.ori  r1,r0, EXR1K_SR_SM|EXR1K_SR_EPH
	    l.mtspr r0,r1, EXR1K_SR
	    l.mtspr r0,r0, EXR1K_TTMR

	// Init Stack pointer @ R1
		LOAD_CONST_2_GPR(r1,__stack_s)

	// Init cache
		SCALL(_enable_icache)
		SCALL(_enable_dcache)

	// Init Data section
		//LCALL(_init_data_section)		// DO NOT CALL
		LCALL(_init_bss_section)		// SHOULD CALL

	// Jump to main program entry point (argc = argv = 0)
		CLEAR_GPR(r3)
		CLEAR_GPR(r4)
		LCALL(main)

	// If program exits, call exit routine
		l.addi  r3, r11, 0
		l.jal   exit
		l.nop

//******************************************************************************
// Code Dummy: Only for EXR1K
//------------------------------------------------------------------------------
		.section .reset_dummy, "ax"
_reset_dummy_code:
		FILL_NOPS_1CACHE_LINE

		.section .text_dummy, "ax"
_text_dummy_code:
		FILL_NOPS_1CACHE_LINE

		.section .ispm_text_dummy, "ax"
_ispm_text_dummy_code:
		FILL_NOPS_1CACHE_LINE

#ifdef _SIZE_FIXED
		.section .flagdata, "ax"
_flash_dummy_code:
		.word	0x00010203
		.word	0x04050607
		.word	0x08090a0b
		.word	0x0c0d0e0f
#endif

