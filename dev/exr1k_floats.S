//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		systfloat.S
//	Description:
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:			ref to systfloat.S
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151110		ygkim		first designed
// -----------------------------------------------------------------------------
#include "exr1k_asm.h"

#define	FUNC_ENTRY(symbol)				 \
	.align 4							;\
	ENTRY(symbol)

//******************************************************************************
	.section	.text
//------------------------------------------------------------------------------
//

FUNC_ENTRY(_itof)	// Integer to Float
	lf.itof.s r11, r3
	l.jr r9
	l.nop

FUNC_ENTRY(_ftoi)	// Float to Integer
	lf.ftoi.s r11, r3
	l.jr r9
	l.nop

FUNC_ENTRY(_fadd)	// +
	lf.add.s r11, r3, r4
	l.jr r9
	l.nop

FUNC_ENTRY(_fsub)	// -
	lf.sub.s r11, r3, r4
	l.jr r9
	l.nop

FUNC_ENTRY(_fmul)	//	*
	lf.mul.s r11, r3, r4
	l.jr r9
	l.nop

FUNC_ENTRY(_fdiv)	//	 /
	lf.div.s r11, r3, r4
	l.jr r9
	l.nop

FUNC_ENTRY(_feq)	// ==
	lf.sfeq.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop

FUNC_ENTRY(_fge)	// >=
	lf.sfge.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop

FUNC_ENTRY(_fgt)	// >
	lf.sfgt.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop

FUNC_ENTRY(_fle)	// <=
	lf.sfle.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop

FUNC_ENTRY(_flt)	// <
	lf.sflt.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop

FUNC_ENTRY(_fne)	// !=
	lf.sfne.s r3, r4
	l.bnf 1f
	l.addi r11, r0, 0
	l.addi r11, r0, 1
1:	l.jr r9
	l.nop
