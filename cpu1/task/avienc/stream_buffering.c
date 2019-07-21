//*************************************************************************************************
// Master task
//*************************************************************************************************
#include "dev.h"
#include "dev_inline.h"	// to use fast code

extern void *pvPortMalloc( size_t xWantedSize );					// @heap_3
extern void vPortFree( void *pv );									// @heap_3

#ifdef __DEVICE_SD__
#include "stream_buffering.h"

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
	psbuf->tot_num = tot_num;
	psbuf->data = (sbuf_data *)pvPortCalloc(sizeof(sbuf_data), tot_num);

	if (psbuf->data != NULL) {
		return DEF_OK;
	} else {
		return DEF_FAIL;
	}
}

int
sbuf_put(sbuf_t *psbuf, u32 addr, u32 size, u32 flags, u32 type)
{
	int	bRes = DEF_FAIL;

	if (cQueue_isfull(psbuf) == DEF_OK) { // 이 조건문에 진입하면, 다음 데이터가 I가 아니라면 의미 없으므로 밀어버린다.
		num_loop(psbuf->head, psbuf->tot_num);
#if 1
		while (cQueue_isempty(psbuf) != DEF_OK) {
			if (psbuf->data[psbuf->head].flags != 1) {
				num_loop(psbuf->head, psbuf->tot_num);
			} else {
				break;
			}
		}
#endif
	}

	psbuf->data[psbuf->tail].addr = addr;
	psbuf->data[psbuf->tail].size = size;
	psbuf->data[psbuf->tail].flags = flags;
	psbuf->data[psbuf->tail].type = type;

	num_loop(psbuf->tail, psbuf->tot_num);

	bRes = DEF_OK;

	return bRes;
}

int
sbuf_get(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags, u32 *type)
{
	int	bRes = DEF_FAIL;
	if (cQueue_isempty(psbuf) == DEF_FAIL) {
		portENTER_CRITICAL();

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*type = psbuf->data[psbuf->head].type;

		num_loop(psbuf->head, psbuf->tot_num);

		bRes = DEF_OK;

		portEXIT_CRITICAL();
	}
	return bRes;
}

int
sbuf_peek(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags, u32 *type)
{
	int	bRes = DEF_FAIL;
	if (cQueue_isempty(psbuf) == DEF_FAIL) {
		portENTER_CRITICAL();

		*addr = psbuf->data[psbuf->head].addr;
		*size = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*type = psbuf->data[psbuf->head].type;

		bRes = DEF_OK;

		portEXIT_CRITICAL();
	}
	return bRes;
}

int
sbuf_freebuffer(sbuf_t *psbuf)
{
	BYTE bRes = DEF_FAIL;

	portENTER_CRITICAL();

	psbuf->head = 0;
	psbuf->tail = 0;
	memset(psbuf->data, 0, sizeof(sbuf_data)*psbuf->tot_num);

	bRes = DEF_OK;

	portEXIT_CRITICAL();

	return	bRes;
}

int
sbuf_free(sbuf_t *psbuf)
{
	BYTE bRes = DEF_FAIL;

	portENTER_CRITICAL();

	psbuf->head = 0;
	psbuf->tail = 0;
	psbuf->tot_num = 0;
	if (psbuf->data) {
		vPortFree(psbuf->data);
	}

	memset(psbuf, 0, sizeof(sbuf_t));

	bRes = DEF_OK;

	portEXIT_CRITICAL();

	return	bRes;
}
#endif
