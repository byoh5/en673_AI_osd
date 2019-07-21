#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "stream_buffering.h"

/**
	@param
	@num : number of stream

	@return
		DEF_OK   : ok
		DEF_FAIL : FAIL
*/
int sbuf_init(sbuf_t *phsbuf, UINT tot_num)
{
	memset(phsbuf, 0, sizeof(sbuf_t));
	phsbuf->tot_num = tot_num;
    return 0;
}

int sbuf_put(sbuf_t *psbuf, UINT addr, UINT size, UINT flags, UINT ts)
{
	int	bRes = DEF_FAIL;

	psbuf->data[psbuf->tail].addr = addr;
	psbuf->data[psbuf->tail].size = size;
	psbuf->data[psbuf->tail].flags = flags;
	psbuf->data[psbuf->tail].ts = ts;

	num_loop(psbuf->tail, psbuf->tot_num);

	bRes = DEF_OK;

	return bRes;
}

int sbuf_get(sbuf_t *psbuf, UINT *addr, UINT *size, UINT *flags, UINT *ts)
{
	BYTE bRes = DEF_FAIL;
	if(cQueue_isempty(psbuf) == DEF_FAIL)
	{
		CRITICAL_BEGIN;

		*addr  = psbuf->data[psbuf->head].addr;
		*size  = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*ts    = psbuf->data[psbuf->head].ts;

		num_loop(psbuf->head, psbuf->tot_num);

		bRes = DEF_OK;

		CRITICAL_END;
	}
	return	bRes;
}

int sbuf_peek(sbuf_t *psbuf, UINT *addr, UINT *size, UINT *flags, UINT *ts)
{
	BYTE bRes = DEF_FAIL;
	if(cQueue_isempty(psbuf) == DEF_FAIL)
	{
		CRITICAL_BEGIN;

		*addr  = psbuf->data[psbuf->head].addr;
		*size  = psbuf->data[psbuf->head].size;
		*flags = psbuf->data[psbuf->head].flags;
		*ts    = psbuf->data[psbuf->head].ts;

	    bRes = DEF_OK;

		CRITICAL_END;
	}
	return bRes;
}

int sbuf_freebuffer(sbuf_t *psbuf)
{
	BYTE bRes = DEF_FAIL;

	CRITICAL_BEGIN;

	psbuf->head = 0;
	psbuf->tail = 0;

	bRes = DEF_OK;

	CRITICAL_END;

	return bRes;
}
