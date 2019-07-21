#ifndef _PORTMACRO_H_
#define _PORTMACRO_H_

#include "lib.h"

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
// Data type
#define	portCHAR		char
//#define	portFLOAT		float
//#define	portDOUBLE		double
#define	portLONG		long
#define	portSHORT		short
#define	portWLONG		long long

#define	portUSHORT		unsigned short
#define	portULONG		unsigned long
#define	portUWLONG		unsigned long long

#define	portSTACK_TYPE	unsigned portLONG
#define	portBASE_TYPE	long

//-------------------------------------------------------------------------------------------------
// Tick type
#if configUSE_16_BIT_TICKS == 1
	typedef portUSHORT		portTickType;
	#define portMAX_DELAY	(portTickType)0xffff
#else
	typedef portULONG		portTickType;
	#define portMAX_DELAY	(portTickType)0xffffffff
#endif

//-------------------------------------------------------------------------------------------------
// Architecture
#define portSTACK_GROWTH				(-1)
#define portTICK_RATE_MS				((portTickType)1000/configTICK_RATE_HZ)
#define portBYTE_ALIGNMENT				4
#define	portCRITICAL_NESTING_IN_TCB		1				// Critical nesting function

//-------------------------------------------------------------------------------------------------
// Task function macros(Described on the FreeRTOS.org WEB site)
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters )	void vFunction( void *pvParameters )
#define portTASK_FUNCTION( vFunction, pvParameters )		void vFunction( void *pvParameters )



//*************************************************************************************************
// Macro
//-------------------------------------------------------------------------------------------------
// Critical section
#define portDISABLE_INTERRUPTS()	CRITICAL_BEGIN
#define portENABLE_INTERRUPTS()		CRITICAL_END

//#define portDISABLE_INTERRUPTS()	mtspr(EXR1K_SR, mfspr(EXR1K_SR) & ~(EXR1K_SR_IEE|EXR1K_SR_TEE))
//#define portENABLE_INTERRUPTS()	mtspr(EXR1K_SR, mfspr(EXR1K_SR) |  (EXR1K_SR_IEE|EXR1K_SR_TEE))

extern void vPortEnterCritical(void);
extern void vPortExitCritical(void);
#define portENTER_CRITICAL()		vPortEnterCritical()
#define portEXIT_CRITICAL()			vPortExitCritical()

//-------------------------------------------------------------------------------------------------
// Kernel util
#define portYIELD()					asm volatile ("	l.sys	0x0						")

#define portNOP()					asm volatile ("	l.nop	0x0						")

//-------------------------------------------------------------------------------------------------
// Restore context (Same as restore procedure of context switch in tick exception)
#define portRESTORE_CONTEXT()		{													\
	/* pxCurrentTCB->R1	*/			asm volatile("	l.movhi	r3,hi(pxCurrentTCB)		");	\
									asm volatile("	l.ori 	r3,r3,lo(pxCurrentTCB)	");	\
									asm volatile("	l.lwz	r3,0x0(r3)	  			");	\
									asm volatile("	l.lwz	r1,0x0(r3)  			");	\
						        	asm volatile("	                                ");	\
	/* ESR */						asm volatile("	l.lwz	r3,0x08(r1)				");	\
									asm volatile("	l.mtspr	r0,r3,0x40				");	\
						        	asm volatile("	                                ");	\
	/* EPC */						asm volatile("	l.lwz	r3,0x04(r1)				");	\
									asm volatile("	l.mtspr	r0,r3,0x20				");	\
						        	asm volatile("	                                ");	\
									asm volatile("	l.movhi r3,hi(_restore_gpr)		");	\
									asm volatile("	l.ori   r3,r3,lo(_restore_gpr)	");	\
									asm volatile("	l.jalr	r3						");	\
									asm volatile("	l.nop							");	\
						        	asm volatile("	                                ");	\
									asm volatile("	l.lwz	r3,0x0c(r1)             ");	\
									asm volatile("	l.lwz   r9,0x7c(r1)             ");	\
									asm volatile("	l.addi  r1,r1,256               ");	\
									asm volatile("	l.rfe                           ");	\
									asm volatile("	l.nop                           ");	\
									}


#endif /* _PORTMACRO_H_ */
