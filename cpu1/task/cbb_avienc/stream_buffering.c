//*************************************************************************************************
// Master task
//*************************************************************************************************
#include <stdlib.h>

#include "dev.h"
#include "dev_inline.h"
#include "stream_buffering.h"
		
extern void *pvPortMalloc( size_t xWantedSize );					// @heap_3
extern void vPortFree( void *pv );	
#ifdef __DEVICE_SD__

/**
	@param
	@num : number of stream 

	@return 
		DEF_OK   : ok
		DEF_FAIL : FAIL
*/
int
sbuf_init(sbuf_t *psbuf, int tot_num)
{	
	memset(psbuf, 0, sizeof(sbuf_t));

	psbuf->head = 0;
	psbuf->tail = 0;
	psbuf->prev = 0;
	psbuf->tot_num = tot_num;

#if 0
	if(psbuf->data = (sbuf_data *)pvPortMalloc(sizeof(sbuf_data)*tot_num))
	{
		memset(psbuf->data, 0x00, sizeof(sizeof(sbuf_data)*tot_num));
		return DEF_OK;
	}
	else
		return DEF_FAIL;	
#endif
//		return DEF_OK;
}

int
sbuf_tot(sbuf_t *psbuf, int tot_num)
{
	if(psbuf->head < psbuf->tail)
	{
		if(psbuf->tail < tot_num)
			psbuf->tot_num = tot_num;
	}
	if(NUM_STREAM_H264 > tot_num)
		psbuf->tot_num = NUM_STREAM_H264;
	
	return DEF_OK; 
}

int
sbuf_put(sbuf_t *psbuf, u32 addr, u32 size, u32 flags)
{
	int	bRes = DEF_FAIL;

	if(cQueue_isfull(psbuf) == DEF_OK)
	{	// 이 조건문에 진입하면, 다음 데이터가 I가 아니라면 의미 없으므로 밀어버린다.
		num_loop(psbuf->head, psbuf->tot_num);
#if 1
		while(cQueue_isempty(psbuf) != DEF_OK)
		{
			if(psbuf->data[psbuf->head].flags != 1)
			{
				num_loop(psbuf->head, psbuf->tot_num);
			}
			else
				break;
		}
#endif
	}

	psbuf->data[psbuf->tail].addr = addr;
	psbuf->data[psbuf->tail].size = size;
	psbuf->data[psbuf->tail].flags = flags;

	num_loop(psbuf->tail, psbuf->tot_num);

	bRes = DEF_OK;

	return bRes;
}

//static UINT testsize = 0;
int
sbuf_get(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags)
{
	int	bRes = DEF_FAIL;
	if(cQueue_isempty(psbuf) == DEF_FAIL)
	{
		CRITICAL_BEGIN;

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;

		num_loop(psbuf->head, psbuf->tot_num);
		//if(testsize != psbuf->tot_num)
		//{
		//	testsize = psbuf->tot_num;
		//	//printf(" %d ",psbuf->tot_num);
		//}
		bRes = DEF_OK;

		CRITICAL_END;
	}
	return bRes;
}

int
sbuf_peek(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags)
{
	int	bRes = DEF_FAIL;
	if((cQueue_isempty(psbuf) == DEF_FAIL))
	{
		CRITICAL_BEGIN;

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;

		bRes = DEF_OK;

		CRITICAL_END;
	}
	
	return bRes;
}

int
sbuf_freebuffer(sbuf_t *psbuf)
{
	BYTE bRes = DEF_FAIL;
	CRITICAL_BEGIN;

	psbuf->head = 0;
	psbuf->tail = 0;

	memset(psbuf->data, 0, sizeof(sbuf_data)*psbuf->tot_num);

	bRes = DEF_OK;

	CRITICAL_END;

	return	bRes;
}

int
sbuf_free(sbuf_t *psbuf)
{
	BYTE bRes = DEF_FAIL;
	CRITICAL_BEGIN;

	psbuf->head = 0;
	psbuf->tail = 0;
	psbuf->tot_num = 0;
//	if(psbuf->data)
//		vPortFree(psbuf->data);

	memset(psbuf, 0, sizeof(sbuf_t));

	bRes = DEF_OK;

	CRITICAL_END;

	return	bRes;
}
#endif
