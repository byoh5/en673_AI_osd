//*************************************************************************************************
// Implementation of functions defined in "portable.h" for the OpenRISC port.
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "msg.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

BYTE gbScheduleStarted = 0;						// Guard

portSTACK_TYPE *pXsrStack;						// Xsr stack pointer
portSTACK_TYPE	gnXsrStack[XSR_STACK_SIZE/4];	// Xsr stack

extern	void vTaskEnterCritical(void);
extern	void vTaskExitCritical(void);

//*************************************************************************************************
// Functions mapped to "portable.h"
//-------------------------------------------------------------------------------------------------
// TCB initialize (Called by "xTaskCreate")
// 00	r2
// 04	EXR1K_EPCR
// 08	EXR1K_ESR
// 0c	r3
// 10	r4
// 14	r5
// 18	r6
// 1c	r7
// 20	r8
// 24	r10
// 28	r11
// 2c	r12
// 30	r13
// 34	r14
// 38	r15
// 3c	r16
// 40	r17
// 44	r18
// 48	r19
// 4c	r20
// 50	r21
// 54	r22
// 58	r23
// 5c	r24
// 60	r25
// 64	r26
// 68	r27
// 6c	r28
// 70	r29
// 74	r30
// 78	r31
// 7c	r9
portSTACK_TYPE *pxPortInitialiseStack(portSTACK_TYPE *pxTopOfStack, pdTASK_CODE pxCode, void *pvParameters)
{
	int		i;

	pxTopOfStack -= 64;
	portSTACK_TYPE *pxOriginalTOS = pxTopOfStack;

							*pxTopOfStack++ = 0;																// R2
							*pxTopOfStack++ = (portSTACK_TYPE)pxCode;											// PC
							*pxTopOfStack++ = (portSTACK_TYPE)(mfspr(EXR1K_SR)|EXR1K_SR_IEE|EXR1K_SR_TEE|EXR1K_SR_SM);	// SR
							*pxTopOfStack++ = (portSTACK_TYPE)pvParameters;										// R3
	for(i=0; i< 5; i++)		*pxTopOfStack++ = 0;																// R4 ~R8
	for(i=0; i<10; i++)		*pxTopOfStack++ = 0;				 												// R10~R19
	for(i=0; i<10; i++) 	*pxTopOfStack++ = 0;																// R20~R29
							*pxTopOfStack++ = 0;																// R30
							*pxTopOfStack++ = 0;																// R31
							*pxTopOfStack   = 0;																// R9
                                                                                                                                	
	return	pxOriginalTOS;																						// Address of the stack
}

//-------------------------------------------------------------------------------------------------
// Start scheduler (Called by "vTaskStartScheduler")
portBASE_TYPE xPortStartScheduler(void)
{
	const portULONG	ulPeriod = (configCPU_CLOCK_HZ/configTICK_RATE_HZ);			// Tick period

	mtspr(EXR1K_TTMR, EXR1K_TTMR_IE|EXR1K_TTMR_M_RESTART|(EXR1K_TTMR_PERIOD&ulPeriod));	// Tick set
	mtspr(EXR1K_TTCR, 0);														//		"

	pXsrStack = gnXsrStack + ((XSR_STACK_SIZE-4)/4);							// Xsr stack

//	xxx = pvPortMalloc();														// Any function call which may enter/exit critical section are allowed here
//	...

	gbScheduleStarted = 1;														// Any function call which may enter/exit critical section are not allowed from now on.
	
	portRESTORE_CONTEXT();														// Start task by restore the first context

	return pdTRUE;																// Should not arrive here.
}

//-------------------------------------------------------------------------------------------------
// End scheduler (Called by "vTaskEndScheduler")
void vPortEndScheduler(void)
{
	mtspr(EXR1K_SR, mfspr(EXR1K_SR) & (~EXR1K_SR_TEE));								// Tick stop
//	...
}



//*************************************************************************************************
// Critical section
//-------------------------------------------------------------------------------------------------
//
void vPortEnterCritical(void)
{
	if(gbScheduleStarted)		vTaskEnterCritical();
}

//-------------------------------------------------------------------------------------------------
//
void vPortExitCritical(void)
{
	if(gbScheduleStarted)		vTaskExitCritical();
}



//*************************************************************************************************
// Debug aid
//-------------------------------------------------------------------------------------------------
// Stack overflow check
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
	printf("\r\n\r\n Error : Task stack overflow name is %s\r\n", (char*)pcTaskName);
	printf("Error : Task is %x", (UINT)*pxTask);
	//while(1);
}

//-------------------------------------------------------------------------------------------------
// Malloc fail check
void vApplicationMallocFailedHook(void)
{
	printf("Error : pvPortMalloc failed\r\n");
	while(1);
}

//-------------------------------------------------------------------------------------------------
// Idle task hook
void vApplicationIdleHook(void)
{
	printf("Idle task hook\r\n");
}

//*************************************************************************************************
// Xsr
//-------------------------------------------------------------------------------------------------
// Tick timer exception
void XsrTick(void)
{
//...............................................
// Watch update
	xsr_tick();

//...............................................
// Task switching
	mtspr(EXR1K_TTMR, mfspr(EXR1K_TTMR) & (~EXR1K_TTMR_IP));	// Interrupt clear

#if configUSE_PREEMPTION == 1
	{
		vTaskIncrementTick();		// ++tick count -> Check blocked task -> Move to ready list
		vTaskSwitchContext();		// Sets "pxCurrentTCB" with new
	}
#else
	{
		vTaskIncrementTick();
	}
#endif
}
