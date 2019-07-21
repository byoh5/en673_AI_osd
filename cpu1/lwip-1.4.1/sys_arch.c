//*************************************************************************************************
//
//*************************************************************************************************

#include <string.h>
#include <time.h>

#include "dev.h"
#include "dev_inline.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"

#include "timer_task.h"

#define MS_TO_TICKS(ms)				(portTickType	  )((portTickType)(ms   ) / portTICK_RATE_MS)
#define TICKS_TO_MS(ticks)			(unsigned portLONG)((portTickType)(ticks) * portTICK_RATE_MS)

//LwIP FreeRTOS port
void sys_init(void)
{
	//LWIP_ASSERT("sys_init: not called first", gpTcb==NULL);
	return;
}

u32_t sys_now(void)
{
	portTickType	xTicks;

	xTicks = xTaskGetTickCount();

	return	(u32_t)TICKS_TO_MS(xTicks);
}


#if !NO_SYS
// ---------------------------------- Semaphore stuff --------------------------------------------
/**
 * Creates and returns a new semaphore. The "count" argument specifies
 * the initial state of the semaphore.
 */
err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
	vSemaphoreCreateBinary(*sem);

	if(sem!=SYS_SEM_NULL){
		if(count==0)
		    xSemaphoreTake(*sem, 1);
		SYS_STATS_INC(sem.used);
	}
	else{
		LWIP_ASSERT("sys_sem_new: sem != SYS_SEM_NULL", sem != SYS_SEM_NULL);
        return ERR_MEM;
	}

	return	ERR_OK;
}

/**
 * Deallocates a semaphore.
 */
void sys_sem_free(sys_sem_t *sem)
{
    LWIP_ASSERT("sys_sem_free: sem != SYS_SEM_NULL", sem != SYS_SEM_NULL);

	if(sem!=SYS_SEM_NULL)
	{
        vQueueDelete(*sem);
        SYS_STATS_DEC(sem.used);
	}
}

/**
 * Signals a semaphore.
 */
void sys_sem_signal(sys_sem_t *sem)
{
    LWIP_ASSERT("sys_sem_signal: sem != SYS_SEM_NULL", sem != SYS_SEM_NULL);

	if(xSemaphoreGive(*sem)!=pdPASS)
	{
		LWIP_ASSERT("sys_sem_signal : xSemaphoreGive(sem)!=pdPASS", 0);
	}
}

/**
 * Blocks the thread while waiting for the semaphore to be
 * signaled. If the "timeout" argument is non-zero, the thread should
 * only be blocked for the specified time (measured in
 * milliseconds). If the "timeout" argument is zero, the thread should be
 * blocked until the semaphore is signaled.
 *
 * If the timeout argument is non-zero, the return value is the number of
 * milliseconds spent waiting for the semaphore to be signaled. If the
 * semaphore wasn't signaled within the specified time, the return value is
 * SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
 * (i.e., it was already signaled), the function may return zero.
 *
 * Notice that lwIP implements a function with a similar name,
 * sys_sem_wait(), that uses the sys_arch_sem_wait() function.
 */

u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
	portBASE_TYPE	xSts;
	portTickType	xTickS;

    LWIP_ASSERT("sys_arch_sem_wait: sem != SYS_SEM_NULL", *sem != SYS_SEM_NULL);

	xTickS = xTaskGetTickCount();

	if(timeout==0)		xSts = xSemaphoreTake(*sem, portMAX_DELAY);
	else				xSts = xSemaphoreTake(*sem, MS_TO_TICKS(timeout));

	if(xSts==pdTRUE){							 // Semaphore was signaled
		return		TICKS_TO_MS(xTaskGetTickCount()-xTickS);
	}
	else{
//		LWIP_ASSERT("sys_arch_sem_wait: xSts==pdTRUE", xSts==pdTRUE);
		return		SYS_ARCH_TIMEOUT;
	}
}

#endif


// ------------------------------------ Mutex Stuff -----------------------------------------------
#if ! LWIP_COMPAT_MUTEX

/** Create a new mutex
 * @param mutex pointer to the mutex to create
 * @return a new mutex */
err_t sys_mutex_new(sys_mutex_t *mutex)
{
	*mutex = xSemaphoreCreateMutex();

	if (!*mutex) {
		SYS_STATS_INC(mutex.err);
		return ERR_MEM;
	}

	SYS_STATS_INC_USED(mutex);

	return ERR_OK;
}

/** Lock a mutex
 * @param mutex the mutex to lock */
void sys_mutex_lock(sys_mutex_t *mutex)
{
	// wait on mutex forever
	while (xSemaphoreTake(*mutex, 10000) != pdTRUE);
}

/** Unlock a mutex
 * @param mutex the mutex to unlock */
void sys_mutex_unlock(sys_mutex_t *mutex)
{
	xSemaphoreGive(*mutex);
}

/** Delete a semaphore
 * @param mutex the mutex to delete */
void sys_mutex_free(sys_mutex_t *mutex)
{
	vQueueDelete(*mutex);
	SYS_STATS_DEC(mutex.used);
}

#endif



// ---------------------------------- MessageBox stuff --------------------------------------------
/**
 * Creates an empty mailbox for maximum "size" elements. Elements stored
 * in mailboxes are pointers. You have to define macros "_MBOX_SIZE"
 * in your lwipopts.h, or ignore this parameter in your implementation
 * and use a default size.
 */

err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
	*mbox = xQueueCreate(size, sizeof(void *)); // XXX correct? We are posting pointers so I guess it is...

	if (!*mbox) {
		SYS_STATS_INC(mbox.err);
		return ERR_MEM;
	}

	SYS_STATS_INC_USED(mbox);

	return ERR_OK;
}


/**
 * Deallocates a mailbox. If there are messages still present in the
 * mailbox when the mailbox is deallocated, it is an indication of a
 * programming error in lwIP and the developer should be notified.
 */
void sys_mbox_free(sys_mbox_t *mbox)
{
	void *msg = NULL;

    LWIP_ASSERT("sys_mbox_free: mbox != SYS_MBOX_NULL", *mbox != SYS_MBOX_NULL);

	if(*mbox != SYS_MBOX_NULL){
		while(uxQueueMessagesWaiting(*mbox)!=0){							// Any message in queue ?
			vTaskDelay(1);
			if(sys_arch_mbox_fetch(mbox, &msg, 1) != SYS_ARCH_TIMEOUT){
				LWIP_ASSERT("sys_mbox_free: memory leak (msg != NULL)", msg == NULL);
			}
		}
		vQueueDelete(*mbox);
		SYS_STATS_DEC(mbox.used);
	}
}

/**
 * Posts the "msg" to the mailbox. This function have to block until
 * the "msg" is really posted.
 */
void sys_mbox_post(sys_mbox_t *mbox, void *msg)
{
	portBASE_TYPE	xSts;

	xSts = xQueueSend(*mbox, &msg, 0);					// Queue must not be full - Otherwise it is an error.
	if(xSts != pdPASS)
		printf("xSts %x",xSts);
	LWIP_ASSERT("sys_mbox_post: xSts == pdPASS", xSts==pdPASS);
}

/**
 * Try to post the "msg" to the mailbox. Returns ERR_MEM if this one
 * is full, else, ERR_OK if the "msg" is posted.
 */
err_t sys_mbox_trypost(sys_mbox_t *mbox, void *data)

{
	LWIP_ASSERT("sys_mbox_trypost: mbox != SYS_MBOX_NULL", *mbox!=SYS_MBOX_NULL);

	if(xQueueSend(*mbox, &data, 0)==pdPASS)		return	ERR_OK;
	else										return	ERR_MEM;
}


/**
 * Blocks the thread until a message arrives in the mailbox, but does
 * not block the thread longer than "timeout" milliseconds (similar to
 * the sys_arch_sem_wait() function). If "timeout" is 0, the thread should
 * be blocked until a message arrives. The "msg" argument is a result
 * parameter that is set by the function (i.e., by doing "*msg =
 * ptr"). The "msg" parameter maybe NULL to indicate that the message
 * should be dropped.
 *
 * The return values are the same as for the sys_arch_sem_wait() function:
 * Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
 * timeout.
 *
 * Note that a function with a similar name, sys_mbox_fetch(), is
 * implemented by lwIP.
 */
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout)
{
	void					*ret_msg;
	portBASE_TYPE   xSts;
	portTickType    xTickS;

//	Uart0_Printf("sys_arch_mbox_fetch ", 0);

	LWIP_ASSERT("sys_arch_mbox_fetch: mbox != SYS_MBOX_NULL", *mbox!=SYS_MBOX_NULL);

	xTickS = xTaskGetTickCount();

	if(timeout==0)		xSts = xQueueReceive(*mbox, &ret_msg, portMAX_DELAY);
	else				xSts = xQueueReceive(*mbox, &ret_msg, MS_TO_TICKS(timeout));

	if(xSts==pdTRUE){
		if(msg)		*msg = ret_msg;
		return		TICKS_TO_MS(xTaskGetTickCount() - xTickS);
	}
	else{
		if(msg)		*msg = NULL;
	}
	
    return      SYS_ARCH_TIMEOUT;
}

/**
 * This is similar to sys_arch_mbox_fetch, however if a message is not
 * present in the mailbox, it immediately returns with the code
 * SYS_MBOX_EMPTY. On success 0 is returned.
 *
 * To allow for efficient implementations, this can be defined as a
 * function-like macro in sys_arch.h instead of a normal function. For
 * example, a naive implementation could be:
 * #define sys_arch_mbox_tryfetch(mbox,msg) \
 * 		sys_arch_mbox_fetch(mbox,msg,1)
 * although this would introduce unnecessary delays.
 */
u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg)

{
	void					*ret_msg;
	portBASE_TYPE   xSts;

//	Uart0_Printf("sys_arch_mbox_tryfetch ", 0);

	LWIP_ASSERT("sys_arch_mbox_tryfetch: mbox != SYS_MBOX_NULL", *mbox!=SYS_MBOX_NULL);

	xSts = xQueueReceive(*mbox, &ret_msg, 0);

	if(xSts==pdTRUE){
		if(msg)		*msg = ret_msg;
		return		0;
	}
	else{
		if(msg)		*msg = NULL;
		return		SYS_MBOX_EMPTY;
	}
}


// ------------------------------------ Task stuff ----------------------------------------------
/**
 * Starts a new thread named "name" with priority "prio" that will begin its
 * execution in the function "thread()". The "arg" argument will be passed as an
 * argument to the thread() function. The stack size to used for this thread is
 * the "stacksize" parameter. The id of the new thread is returned. Both the id
 * and the priority are system dependent.
 */
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio)
{
	sys_thread_t objTask;
    int result;

    result = xTaskCreate(thread, (const signed char*)name, stacksize/4, arg, prio, &objTask);
	if(result == pdPASS)
	{
//		printf("sys_thread_new : pass : %s(%x) -size(%d)\r\n", name, objTask, stacksize);
		return objTask;
	}
	else
	{
		printf("sys_thread_new : fail : %s(%x) -size(%d)\r\n", name, objTask, stacksize);
		return NULL;
	}
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
//
#if 1
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
static u16_t
lwip_standard_chksum(void *dataptr, int len)
{
  u8_t *pb = (u8_t *)dataptr;
  u16_t *ps, t = 0;
  u32_t sum = 0;
  int odd = ((mem_ptr_t)pb & 1);

  /* Get aligned to u16_t */
  if (odd && len > 0) {
    ((u8_t *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (u16_t *)(void *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((u8_t *)&t)[0] = *(u8_t *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return (u16_t)sum;
}
#endif

u16_t my_chksum(void *dataptr, u16_t len)
{
	u16_t accdma, accdma2, accdma3;
	DmaDChkSum_ip((BYTE *)dataptr, (UINT)len, &accdma);
#if 1
	DmaDChkSum_ip((BYTE *)dataptr, (UINT)len, &accdma2);
	if (accdma != accdma2) {
		invalidate_dcache_range((uint)dataptr, (uint)dataptr+len);
		accdma3 = lwip_standard_chksum(dataptr, len);
		char date[32] = {0};
		struct tm tmout;
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
		sprintf(date, "%04d/%02d/%02d %02d-%02d-%02d", (tmout.tm_year+1900), tmout.tm_mon+1, tmout.tm_mday, tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		printf("%s[P:0x%08X]UCK(0x%04X)DCK(0x%04X)SCK(0x%04X)len(%4d)CPU%d\r\n", date, (UINT)dataptr, accdma, accdma2, accdma3, len, CpuIdGet());
		return accdma3;
	}
#endif
	return accdma2;
}

void my_copy(void* dst, void* src, unsigned long len)
{
#if 1
#ifdef CONFIG_DCACHE_ENABLE
    DmaMemCpy_ip((BYTE*)dst, (BYTE*)src, (UINT)len);
    invalidate_dcache_range((uint)dst, ((uint)dst)+len);  /* Maybe, we thick cache flush may bes not used. cache is write through */
#else
    DmaMemCpy_ip((BYTE*)dst, (BYTE*)src, (UINT)len);
#endif
#else
    memcpy(dst,src,len);
#endif
}
