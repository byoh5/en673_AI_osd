//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_asm.h
//	Description:	Assembly macros
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150806		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef _EXR1K_ASM_H_
#define	_EXR1K_ASM_H_


//******************************************************************************
// Stack
//------------------------------------------------------------------------------
#define	SAFEZONE	128
#define EXCEPTION_STACK_SIZE (128 + SAFEZONE)

// Push a reg to the stack: r1 - the current filled slot
#define PUSH(gpr)						 \
	l.addi	r1,r1,-4	/* stack-- */	;\
	l.sw	0(r1),gpr	/* Push */		
	
// Pop a reg from the stack:
#define POP(gpr)						 \
	l.lwz	gpr,0(r1)	/* Pop */		;\
	l.addi	r1,r1,+4	/* stack++ */	


//******************************************************************************
// Load/Store Symbol/Sonstant
//------------------------------------------------------------------------------
//
#define	ENTRY(symbol)					 \
	.global	symbol						;\
symbol:

#define	WORD_ENTRY(symbol)				 \
	ENTRY(symbol)						;\
	.word	_##symbol

#define	LONG_ENTRY(symbol)				 \
	ENTRY(symbol)						;\
	.long	_##symbol

#define	VAR_ENTRY(symbol)				 \
	ENTRY(symbol)						;\
	.word	0x0

#define CLEAR_GPR(gpr) 					 \
	l.or    gpr, r0, r0

#define	LOAD_CONST_2_GPR(gpr,val)	 	 \
	l.movhi	gpr, hi(val)				;\
	l.ori	gpr, gpr, lo(val)

#define	LOAD_SYMBOL_2_GPR(gpr,symbol)	 \
	.global	symbol						;\
	LOAD_CONST_2_GPR(gpr,symbol)

#define LOAD_STR_2_GPR(gpr,str)			 \
	.section .rodata					;\
1:										;\
	.string	str							;\
										;\
	.section .text						;\
	l.movhi	gpr, hi(1b)	/* 1: @back */	;\
	l.ori	gpr, gpr, lo(1b)


//******************************************************************************
// Call/ Xsr
//------------------------------------------------------------------------------
#define LCALL(func)                      \
    l.movhi r3,hi(func)                 ;\
    l.ori   r3,r3,lo(func)              ;\
    l.jalr  r3                          ;\
    l.nop

#define SCALL(func)                      \
	l.jal	func						;\
    l.nop

#define SCALL_DEFAULT_XSR				 \
	l.mfspr	r3, r0, EXR1K_NPC			;\
	l.mfspr	r4, r0, EXR1K_EPCR			;\
	SCALL(default_xsr)

#define PROLOG                           \
    l.addi  r1,r1,-EXCEPTION_STACK_SIZE ;\
    l.sw    0x1c(r1),r9                 ;\
	SCALL(_save_gpr)

#define EPILOG                           \
	SCALL(_restore_gpr)					;\
    l.lwz   r9,0x1c(r1)                 ;\
    l.addi  r1,r1,EXCEPTION_STACK_SIZE  ;\
    l.rfe                               ;\
    l.nop								;\
    FILL_NOPS

#if 0

#define TEST_PROLOG                      \
    l.addi  r1,r1,-EXCEPTION_STACK_SIZE ;\
    l.sw    0x04(r1),r3                 ;\
    l.sw    0x1c(r1),r9                 ;\
	SCALL(_save_gpr)                    ;\
    l.movhi r3,hi(__except_stack)       ;\
    l.ori   r3,r3,lo(__except_stack)    ;\
    l.sw    0x00(r3),r1                 ;\
    l.addi  r3,r3,-4                    ;\
    l.addi  r1,r3,0

#define TEST_EPILOG                      \
    l.movhi r3,hi(__except_stack)       ;\
    l.ori   r3,r3,lo(__except_stack)    ;\
    l.lwz   r1,0x0(r3)                  ;\
	SCALL(_restore_gpr)					;\
	l.lwz   r3,0x04(r1)                 ;\
    l.lwz   r9,0x1c(r1)                 ;\
    l.addi  r1,r1,EXCEPTION_STACK_SIZE  ;\
    l.rfe                               ;\
    l.nop								;\
    FILL_NOPS

#define TEST_IRQ_PROLOG                  \
    l.addi  r1,r1,-EXCEPTION_STACK_SIZE ;\
    l.sw    0x04(r1),r3                 ;\
    l.sw    0x1c(r1),r9                 ;\
	SCALL(_save_gpr)                    ;\
    l.movhi r3,hi(__isr_stack)          ;\
    l.ori   r3,r3,lo(__isr_stack)       ;\
    l.sw    0x00(r3),r1                 ;\
    l.addi  r3,r3,-4                    ;\
    l.addi  r1,r3,0

#define TEST_IRQ_EPILOG                  \
    l.movhi r3,hi(__isr_stack)          ;\
    l.ori   r3,r3,lo(__isr_stack)       ;\
    l.lwz   r1,0x0(r3)                  ;\
	SCALL(_restore_gpr)					;\
	l.lwz   r3,0x04(r1)                 ;\
    l.lwz   r9,0x1c(r1)                 ;\
    l.addi  r1,r1,EXCEPTION_STACK_SIZE  ;\
    l.rfe                               ;\
    l.nop								;\
    FILL_NOPS

#endif

#define FUNC_EPILOG                      \
	SCALL(_restore_gpr)					;\
    l.lwz   r9,0x1c(r1)                 ;\
    l.addi  r1,r1,EXCEPTION_STACK_SIZE  ;\
    l.jr	r9                          ;\
    l.nop								;\


//******************************************************************************
// Gpr
//------------------------------------------------------------------------------
#define	CLEAR_GPRS						 \
	l.andi	r2,r0,0x0                   ;\
	l.andi	r3,r0,0x0                   ;\
	l.andi	r4,r0,0x0                   ;\
	l.andi	r5,r0,0x0                   ;\
	l.andi	r6,r0,0x0                   ;\
	l.andi	r7,r0,0x0                   ;\
	l.andi	r8,r0,0x0                   ;\
	l.andi	r9,r0,0x0                   ;\
	l.andi	r10,r0,0x0                  ;\
	l.andi	r11,r0,0x0                  ;\
	l.andi	r12,r0,0x0                  ;\
	l.andi	r13,r0,0x0                  ;\
	l.andi	r14,r0,0x0                  ;\
	l.andi	r15,r0,0x0                  ;\
	l.andi	r16,r0,0x0                  ;\
	l.andi	r17,r0,0x0                  ;\
	l.andi	r18,r0,0x0                  ;\
	l.andi	r19,r0,0x0                  ;\
	l.andi	r20,r0,0x0                  ;\
	l.andi	r21,r0,0x0                  ;\
	l.andi	r22,r0,0x0                  ;\
	l.andi	r23,r0,0x0                  ;\
	l.andi	r24,r0,0x0                  ;\
	l.andi	r25,r0,0x0                  ;\
	l.andi	r26,r0,0x0                  ;\
	l.andi	r27,r0,0x0                  ;\
	l.andi	r28,r0,0x0                  ;\
	l.andi	r29,r0,0x0                  ;\
	l.andi	r30,r0,0x0                  ;\
	l.andi	r31,r0,0x0

#define	CLEAR_ALL_GPRS					 \
    l.andi	r0,r0,0x0					;\
    l.andi	r1,r0,0x0					;\
    CLEAR_GPRS


//******************************************************************************
// Process control
//------------------------------------------------------------------------------
#define TBREAK							 \
	l.mfspr r3,r0,EXR1K_DSR				;\
    l.ori  r3,r0, EXR1K_DSR_TE			;\
    l.mtspr r0,r3, EXR1K_DSR			;\
	l.trap	0                           

#define	BREAK							 \
	l.jal enable_jtag_ctrl_exception	;\
	l.nop								;\
	l.trap	0


//******************************************************************************
// Code Filling
//------------------------------------------------------------------------------
#define	FILL_NOPS						 \
    l.nop								;\
    l.nop								;\
    l.nop								;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop

#define	FILL_NOPS_1CACHE_LINE			 \
    l.nop								;\
    l.nop								;\
    l.nop								;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop								;\
    l.nop								;\
    l.nop								;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop                               ;\
    l.nop


//******************************************************************************
// FPU
//------------------------------------------------------------------------------
#define SET_RM(rm)						 \
	l.mfspr r4, r0, EXR1K_FPCSR 		;\
	/* Clear rounding mode bits */		;\
	l.andi 	r4, r4,~EXR1K_FPCSR_RM		;\
	/* Set desired rounding mode bits */;\
	l.ori 	r4, r4, rm					;\
	l.mtspr r0, r4, EXR1K_FPCSR

#endif//_EXR1K_ASM_H_
