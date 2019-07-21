#include "exr1k_sprs.h"
#include "exr1k_asm.h"

	
//******************************************************************************
// Instruction cache enable
//------------------------------------------------------------------------------
ENTRY(_enable_icache)
    l.addi  r1,r1,-24			// dec stack
    l.sw    0(r1),r3			// save gprs
    l.sw    4(r1),r4
    l.sw    8(r1),r5
    l.sw    12(r1),r6
    l.sw    16(r1),r7
    l.sw    20(r1),r14

	// Check if IC present: UPR[ICP=2] ?
	l.mfspr r3,r0,EXR1K_UPR
	l.andi  r4,r3,EXR1K_UPR_ICP
	l.sfeq  r4,r0
	l.bf    _enable_icache_end
	l.nop
	
//	SCALL(_disable_icache)
	// Disable IC: SR[ICE=4] = 0
	l.mfspr r6,r0,EXR1K_SR
	l.addi  r5,r0,-1
	l.xori  r5,r5,EXR1K_SR_ICE
	l.and   r5,r6,r5
	l.mtspr r0,r5,EXR1K_SR
	
	// Set cache block size
	// r14: cache block size = 32, 16 @BS=0, 32 @@BS=1. BS = ICCFGR[7]
	l.mfspr r3,r0,EXR1K_ICCFGR
	l.andi  r4,r3,EXR1K_ICCFGR_CBS
	l.srli  r5,r4,7
	l.ori   r6,r0,16
	l.sll   r14,r6,r5
	
	// Set number of cache sets
	// r7: # of cache sets = 256
	// r5: log(# of cache sets) = 8
	l.andi  r4,r3,EXR1K_ICCFGR_NCS
	l.srli  r5,r4,3
	l.ori   r6,r0,1
	l.sll   r7,r6,r5
	
	// Invalidate IC
	l.addi  r6,r0,0
	l.sll   r5,r14,r5
_ic_loop:	
	l.mtspr r0,r6,EXR1K_ICBIR
	l.sfne  r6,r5
	l.bf    _ic_loop
	l.add   r6,r6,r14
	
	// Enable IC
	l.mfspr r6,r0,EXR1K_SR
	l.ori   r6,r6,EXR1K_SR_ICE
	l.mtspr r0,r6,EXR1K_SR
	FILL_NOPS

_enable_icache_end:
    l.lwz   r14,20(r1)		
    l.lwz   r7,16(r1)		
    l.lwz   r6,12(r1)		
    l.lwz   r5,8(r1)		
    l.lwz   r4,4(r1)		
    l.lwz   r3,0(r1)			// restore gprs
    l.addi  r1,r1,24			// inc stack
	// Return
	l.jr	r9
	l.nop


//******************************************************************************
// Data cache enable
//------------------------------------------------------------------------------
ENTRY(_enable_dcache)
    l.addi  r1,r1,-24			// dec stack
    l.sw    0(r1),r3			// save gprs
    l.sw    4(r1),r4
    l.sw    8(r1),r5
    l.sw    12(r1),r6
    l.sw    16(r1),r7
    l.sw    20(r1),r14

	// Check if IC present: UPR[DCP=1] ?
    l.mfspr r3,r0,EXR1K_UPR
    l.andi  r4,r3,EXR1K_UPR_DCP
    l.sfeq  r4,r0
    l.bf    _enable_dcache_end
    l.nop
    
    // Disable DC: SR[DCE=3] = 0
    l.mfspr r6,r0,EXR1K_SR
    l.addi  r5,r0,-1
    l.xori  r5,r5,EXR1K_SR_DCE
	l.and   r5,r6,r5
    l.mtspr r0,r5,EXR1K_SR

	// Set cache block size
	// r14: cache block size = 32, 16 @BS=0, 32 @@BS=1. BS = DCCFGR[7]
    l.mfspr r3,r0,EXR1K_DCCFGR
    l.andi  r4,r3,EXR1K_DCCFGR_CBS
    l.srli  r5,r4,7
    l.ori   r6,r0,16
    l.sll   r14,r6,r5
    
	// Set number of cache sets
	// r7: # of cache sets = 256
	// r5: log(# of cache sets) = 8
	l.andi  r4,r3,EXR1K_DCCFGR_NCS
	l.srli  r5,r4,3
    l.ori   r6,r0,1
    l.sll   r7,r6,r5
    
	// Invalidate DC
    l.addi  r6,r0,0
    l.sll   r5,r14,r5
_dc_loop:
    l.mtspr r0,r6,EXR1K_DCBIR
    l.sfne  r6,r5
    l.bf    _dc_loop
	l.add   r6,r6,r14
	
    // Enable DC
    l.mfspr r6,r0,EXR1K_SR
    l.ori   r6,r6,EXR1K_SR_DCE
    l.mtspr r0,r6,EXR1K_SR

_enable_dcache_end:
    l.lwz   r14,20(r1)		
    l.lwz   r7,16(r1)		
    l.lwz   r6,12(r1)		
    l.lwz   r5,8(r1)		
    l.lwz   r4,4(r1)		
    l.lwz   r3,0(r1)			// restore gprs
    l.addi  r1,r1,24			// inc stack
	// Return
	l.jr	r9
	l.nop
	
	
	