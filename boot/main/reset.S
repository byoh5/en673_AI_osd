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

#define	SFLS_REG_BASE			0xf9200000
#define	SFLS_WBCMD				(SFLS_REG_BASE+0x08)
#define	SFLS_WBCONT				(SFLS_REG_BASE+0x0C)
#define	SFLS_CMDADR				(SFLS_REG_BASE+0x10)
#define	SFLS_CMD				(SFLS_REG_BASE+0x14)

#define CPU_REG_BASE			0xf9400000
#define	CPU_INFO				(CPU_REG_BASE+0)


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

	.org	0x0a0
// binary check key(12byte)
gbModel:
		.byte	0x45
		.byte	0x59
		.byte	0x45
		.byte	0x4e
		.byte	0x49
		.byte	0x58
		.byte	0x45
		.byte	0x4e
		.byte	0x36
		.byte	0x37
		.byte	0x33
#ifdef __EN673M__
		.byte	0x4d
#else
		.byte	0x20
#endif
gbVersion:
		.byte	(EN673_VERSION>>8)&0xf
		.byte	(EN673_VERSION>>4)&0xf
		.byte	(EN673_VERSION>>0)&0xf
		.byte	0xFF

//******************************************************************************
// Exceptions
//-----------------------------------------------------------------------------
// Reset
	.org	0x100
ENTRY(_reset)
		l.nop
		CLEAR_ALL_GPRS

	// Sfls Quad Mode for sim
		// get CPU ID and set flash if ID (@r2) =0 (@r4)
//		LOAD_CONST_2_GPR(r3,CPU_INFO)
//		l.lwz	r2,0(r3)
//		l.srli	r2,r2,4															// >>4
//		l.andi	r2,r2,0x03														// CPU ID
//		l.ori	r4,r0,0															//
//		l.sfne	r2,r4															// ID!=0
//		l.bf 	_jump_start
//		l.nop

//		SCALL(_set_flash)

	// Jump to _start
_jump_start:
		LOAD_CONST_2_GPR(r3, _start)
		l.jr	r3
		l.nop
    	FILL_NOPS

//-----------------------------------------------------------------------------
// BUS Error
	.org 0x200
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Data Page Fault
	.org 0x300
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Insn Page Fault
	.org 0x400
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Timer
	.org 0x500
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Data Aligment Error
	.org 0x600
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Illegal Insn
	.org 0x700
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// External Interrupt
	.org 0x800
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// DTLB miss
	.org 0x900
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// ITLB miss
	.org 0xa00
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Range Error
	.org 0xb00
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// System Call
	.org 0xc00
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Floating-point Error
	.org 0xd00
	    PROLOG
	    SCALL_DEFAULT_XSR
	    EPILOG

//-----------------------------------------------------------------------------
// Trap
	.org 0xe00
		PROLOG
	    SCALL_DEFAULT_XSR
		EPILOG


//******************************************************************************
// GPR functions
// Stack pointer & r9 are already set
//------------------------------------------------------------------------------
_save_gpr:
		l.sw    0x00(r1),r2
		l.sw	0x04(r1),r3
		l.sw	0x08(r1),r4
		l.sw	0x0c(r1),r5
		l.sw	0x10(r1),r6
		l.sw	0x14(r1),r7
		l.sw	0x18(r1),r8
		//l.sw	0x1c(r1),r9
		l.sw	0x20(r1),r10
		l.sw	0x24(r1),r11
		l.sw	0x28(r1),r12
		l.sw	0x2c(r1),r13
		l.sw	0x30(r1),r14
		l.sw	0x34(r1),r15
		l.sw	0x38(r1),r16
		l.sw	0x3c(r1),r17
		l.sw	0x40(r1),r18
		l.sw	0x44(r1),r19
		l.sw	0x48(r1),r20
		l.sw	0x4c(r1),r21
		l.sw	0x50(r1),r22
		l.sw	0x54(r1),r23
		l.sw	0x58(r1),r24
		l.sw	0x5c(r1),r25
		l.sw	0x60(r1),r26
		l.sw	0x64(r1),r27
		l.sw	0x68(r1),r28
		l.sw	0x6c(r1),r29
		l.sw	0x70(r1),r30
		l.sw	0x74(r1),r31
		l.jr	r9
		l.nop

_restore_gpr:
		l.lwz	r31,0x74(r1)
		l.lwz	r30,0x70(r1)
		l.lwz	r29,0x6C(r1)
		l.lwz	r28,0x68(r1)
		l.lwz	r27,0x64(r1)
		l.lwz	r26,0x60(r1)
		l.lwz	r25,0x5C(r1)
		l.lwz	r24,0x58(r1)
		l.lwz	r23,0x54(r1)
		l.lwz	r22,0x50(r1)
		l.lwz	r21,0x4C(r1)
		l.lwz	r20,0x48(r1)
		l.lwz	r19,0x44(r1)
		l.lwz	r18,0x40(r1)
		l.lwz	r17,0x3C(r1)
		l.lwz	r16,0x38(r1)
		l.lwz	r15,0x34(r1)
		l.lwz	r14,0x30(r1)
		l.lwz	r13,0x2C(r1)
		l.lwz	r12,0x28(r1)
		l.lwz	r11,0x24(r1)
		l.lwz	r10,0x20(r1)
		//l.lwz	r9 ,0x1c(r1)
		l.lwz	r8 ,0x18(r1)
		l.lwz	r7 ,0x14(r1)
		l.lwz	r6 ,0x10(r1)
		l.lwz	r5 ,0x0C(r1)
		l.lwz	r4 ,0x08(r1)
		l.lwz	r3 ,0x04(r1)
		l.lwz	r2 ,0x00(r1)
		l.jr 	r9
		l.nop


//******************************************************************************
// Set Flash
//------------------------------------------------------------------------------
//_set_flash:
//		LOAD_CONST_2_GPR(r2,0x0000EB02)
//		LOAD_CONST_2_GPR(r3,SFLS_WBCMD)
//		l.sw	0x0(r3),r2
//		LOAD_CONST_2_GPR(r2,0x387EAAAA)
//		LOAD_CONST_2_GPR(r3,SFLS_WBCONT)
//		l.sw	0x0(r3),r2
//		LOAD_CONST_2_GPR(r2,0x20030000)
//		LOAD_CONST_2_GPR(r3,SFLS_CMD)
//		l.sw	0x0(r3),r2
//		l.jr	r9
//		l.nop


//******************************************************************************
// Text section
		.section .text
//------------------------------------------------------------------------------
_start:
	// Init SR
	    l.ori  r1,r0, EXR1K_SR_SM|EXR1K_SR_EPH
	    l.mtspr r0,r1, EXR1K_SR
	    l.mtspr r0,r0, EXR1K_TTMR

	// Init ISR
//		l.ori	r1,r0, 1	// EXR1K_ISR_RDWE
//		l.mtspr r0,r1, EXR1K_ISR
		l.andi	r1,r0, 0	// EXR1K_ISR_RDWE
		l.mtspr r0,r1, EXR1K_ISR

	// Init Stack pointer @ R1
		LOAD_CONST_2_GPR(r1,__stack_s)

	// Init Data section
		LCALL(_init_text_section)
		LCALL(_init_data_section)		// SHOULD CALL
		LCALL(_init_bss_section)		// SHOULD CALL

	// Jump to main program entry point (argc = argv = 0)
		CLEAR_GPR(r3)
		CLEAR_GPR(r4)
		l.jal 	main
		l.nop

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
