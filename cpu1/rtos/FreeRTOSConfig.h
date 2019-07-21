#ifndef _FREERTOS_CONFIG_H_
#define _FREERTOS_CONFIG_H_

//*************************************************************************************************
// Application specific definitions
//-------------------------------------------------------------------------------------------------
// About clock
#define configCPU_CLOCK_HZ					((ULONG)BUS_CLK)			// Clock speed
#define configTICK_RATE_HZ					((portTickType)TIME_TICK)	// Task switching rate

//...............................................
// About memory
#define configTOTAL_HEAP_SIZE				((size_t)0)					// Because we use "heap_3.c"
#define configMINIMAL_STACK_SIZE			((WORD)(IDLE_STACK_SIZE/4))

//...............................................
//
#define configUSE_PREEMPTION				1
#define configMAX_PRIORITIES				((unsigned portBASE_TYPE)8)
#define configMAX_TASK_NAME_LEN				8
#define configUSE_16_BIT_TICKS				0
#define configUSE_IDLE_HOOK					0
#define configUSE_TICK_HOOK					0
#define configUSE_TRACE_FACILITY			1					// Do not define after development .CAUTION
#define configIDLE_SHOULD_YIELD				1
#define configCHECK_FOR_STACK_OVERFLOW		2					// Do not define after development .CAUTION
#define configUSE_MALLOC_FAILED_HOOK		0					// Do not define after development .CAUTION
#define configUSE_COUNTING_SEMAPHORES		1

//-------------------------------------------------------------------------------------------------
// Co-routine definitions
#define configUSE_CO_ROUTINES				0
#define configMAX_CO_ROUTINE_PRIORITIES		0

//-------------------------------------------------------------------------------------------------
// Set the following definitions to 1 to include the API function, or zero to exclude the API function
#define INCLUDE_vTaskPrioritySet			1					// Do not define after development .CAUTION
#define INCLUDE_uxTaskPriorityGet			1					// Do not define after development .CAUTION
#define INCLUDE_vTaskDelete					1
#define INCLUDE_vTaskCleanUpResources		1
#define INCLUDE_vTaskSuspend				1
#define INCLUDE_vTaskDelayUntil				1
#define INCLUDE_vTaskDelay					1
#define INCLUDE_xTaskGetCurrentTaskHandle	1
#define INCLUDE_uxTaskGetStackHighWaterMark	1					// Do not define after development .CAUTION
#define INCLUDE_xTaskGetSchedulerState		1					// Do not define after development .CAUTION
#define INCLUDE_xTaskResumeFromISR			1

//-------------------------------------------------------------------------------------------------
// Trace utility
//#define traceTASK_SWITCHED_IN
//#define traceTASK_SWITCHED_OUT
//#define traceBLOCKING_ON_QUEUE_RECEIVE
//#define traceBLOCKING_ON_QUEUE_SEND
//#define traceQUEUE_CREATE( pxNewQueue )
//#define traceQUEUE_CREATE_FAILED()						printf("Error RTOS : Queue create failed\r\n");
//#define traceCREATE_MUTEX( pxNewQueue )
//#define traceCREATE_MUTEX_FAILED()
//#define traceGIVE_MUTEX_RECURSIVE( pxMutex )
//#define traceGIVE_MUTEX_RECURSIVE_FAILED( pxMutex )
//#define traceTAKE_MUTEX_RECURSIVE( pxMutex )
//#define traceCREATE_COUNTING_SEMAPHORE()
//#define traceCREATE_COUNTING_SEMAPHORE_FAILED()
//#define traceQUEUE_SEND( pxQueue )
//#define traceQUEUE_SEND_FAILED( pxQueue )
//#define traceQUEUE_RECEIVE( pxQueue )
//#define traceQUEUE_PEEK( pxQueue )
//#define traceQUEUE_RECEIVE_FAILED( pxQueue )
//#define traceQUEUE_SEND_FROM_ISR( pxQueue )
//#define traceQUEUE_SEND_FROM_ISR_FAILED( pxQueue )
//#define traceQUEUE_RECEIVE_FROM_ISR( pxQueue )
//#define traceQUEUE_RECEIVE_FROM_ISR_FAILED( pxQueue )
//#define traceQUEUE_DELETE( pxQueue )
//#define traceTASK_CREATE( pxNewTCB )
//#define traceTASK_DELETE( pxTaskToDelete )
//#define traceTASK_DELAY_UNTIL()
//#define traceTASK_DELAY()
//#define traceTASK_PRIORITY_SET( pxTask, uxNewPriority )
//#define traceTASK_SUSPEND( pxTaskToSuspend )
//#define traceTASK_RESUME( pxTaskToResume )
//#define traceTASK_RESUME_FROM_ISR( pxTaskToResume )
//#define traceTASK_INCREMENT_TICK( xTickCount )


#endif /* _FREERTOS_CONFIG_H_ */

