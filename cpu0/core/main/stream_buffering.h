#ifndef _STREAM_BUFFERING_H_
#define _STREAM_BUFFERING_H_

// struct of h264, g7xx stream buffer

#define NUM_JPEG_SBUF 4
#define NUM_H264_SBUF 60
#define NUM_G711_SBUF 100

#include "dev_types.h"

typedef struct {
	UINT addr;
	UINT size;
	UINT flags;
	UINT ts;
}sbuf_data;

typedef struct {
	UINT head;
	UINT tail;
	UINT tot_num;    // total num of buffer
	sbuf_data	data[100];
}sbuf_t;

int sbuf_init(sbuf_t *phsbuf, UINT tot_num);

int sbuf_put(sbuf_t *psbuf, UINT addr, UINT size, UINT flags, UINT ts);

int sbuf_get(sbuf_t *psbuf, UINT *addr, UINT *size, UINT *flags, UINT *ts);

int sbuf_peek(sbuf_t *psbuf, UINT *addr, UINT *size, UINT *flags, UINT *ts);

int sbuf_freebuffer(sbuf_t *psbuf);

#endif
