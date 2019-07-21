#ifndef _STREAM_BUFFERING_H_
#define _STREAM_BUFFERING_H_

typedef struct {
	u32 addr;
	u32 size;
	u32 flags;
	u32 type;
} sbuf_data;

typedef struct {
	u32 head;
	u32 tail;
	u32 tot_num;			/* total num of buffer */
	sbuf_data *data;
} sbuf_t;

#define sbuf_head_next(psbuf)	{														\
									(psbuf)->head = (psbuf)->head + 1;					\
									(psbuf)->head = (psbuf)->head % (psbuf)->tot_num;	\
								}

#define sbuf_tail_next(psbuf)	{														\
									(psbuf)->tail = (psbuf)->tail + 1;					\
									(psbuf)->tail = (psbuf)->tail % (psbuf)->tot_num;	\
								}

int sbuf_init(sbuf_t *psbuf, int tot_num);

int sbuf_put(sbuf_t *psbuf, u32 addr, u32 size, u32 flags, u32 type);

int sbuf_get(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags, u32 *type);

int sbuf_peek(sbuf_t *psbuf, u32 *addr, u32 *size, u32 *flags, u32 *type);

int sbuf_freebuffer(sbuf_t *psbuf);

#endif
