#ifndef _SD_SAVE_H_
#define _SD_SAVE_H_

#include "fmt_avienc.h"
#include "stream_buffering.h"

#define AVI_ALIGN_SIZE   	4
#define AVI_ALIGN(x) 		(((x) + AVI_ALIGN_SIZE - 1) & ~(AVI_ALIGN_SIZE - 1))

#define AVI_RecTypeCnt		3	// Normal, Event, Snapshot
enum {
	eRecNormal = 0,
	eRecEvent = 1,
	eRecSnapshot = 2
};

void muxer_avienc_init(void);
void muxer_avienc_delete(void);
void muxer_avienc_h264Buffering(u32 h264_ptr, u32 h264_size, u32 idr);
void muxer_avienc_g711Buffering(u32 g711_ptr, u32 g711_size, u32 flags);
void muxer_avienc_txtsBuffering(u32 txts_ptr, u32 txts_size, u32 flags);
void muxer_avienc(void);
void muxer_avienc_task(void* pvParameters);

/**
	register irq and init sbuf for buffering
	this function should be called first before h264 working in cpu1 
*/
void muxer_init_buffering(void);

typedef enum {
	AVIENC_IDLE,
	AVIENC_OPEN,
	AVIENC_FIRST,
	AVIENC_FIRST_NEXT,
	AVIENC_WRITE,
	AVIENC_CLOSE,
	AVIENC_EXIT
}AVIENC_STATE;

typedef struct {
	uint32 flags; /*0 : i-frame, 1: p-frame */
	uint32 size;
	uint32 addr;
	uint32 type;
	uint32 state; // 0:data no, 1:data ok
} t_block_t;

typedef struct {
	uint8* buf;
	uint8* base;
	uint8* end;
	uint32 length;
	uint32 total;
}t_avienc_buf_t;

typedef int (*sd_free_t) (void *enc_t, UINT nMaxDelete);
typedef void (*get_file_name_t) (void *enc_t);

typedef struct {
	int type;							// avi save type(AVI_PERIODIC_SW, AVI_EVENT_SW)
	int recording;						// avi rec?
	AVIENC_STATE state;					// avienc state

	char dir_name[80];					// file path
	char file_format[40];				// file name format
	FIL fpavi;							// avi file
	char now_name[80];					// file name
#ifdef __IDX_WRITE__
	FIL fpidx;							// idx file
	char idx_name[80];					// index file name
#endif
	uint32 filesize;					// file size

//	sbuf_t h264_sbuf;					// H.264 stream queue
//	sbuf_t g711_sbuf;					// G.711 stream queue
//	sbuf_t txts_sbuf;					// TEXT  stream queue
	sbuf_t sbuf;						// H.264, G.711, etc. stream queue

	UINT stime;							// start time of avi encoding
	UINT etime;							// end time of avi encoding

	UINT mFirstFileNum;					// 
	UINT mLastFileNum;					// 

	t_avienc_buf_t buffer;				// H.264 addr, size(buffer)
	t_avi_t aviinfo;					// H.264 FPS, Frame Size, FIle Size, etc...
	t_block_t datablock;				// addr, size, flags

	// INDEX Info
#ifdef __IDX_WRITE__
	t_avienc_buf_t idxbuf;				// data index buffer
#else
	UINT idx_size;
	UCHAR *idx_info;					// avi index buffer
#endif
	UINT idx_len;						// length of idx_info

	// pointer to function
	sd_free_t       pf_sd_free;			// pointer to function to make space for SD CARD
	get_file_name_t pf_get_file_name;	// pointer to function to make name for avi files
}t_avienc_t;

extern void muxer_avienc_go(int var);
extern void muxer_avienc_stop(int var);
extern int muxer_avienc_state(int var);
extern void muxer_avienc_all_stop(void);
extern t_avienc_t *muxsr_avienc_get_t_avienc_t(void);
extern void muxer_avienc_process(t_avienc_t *pavi);

extern void muxer_jpegstill_process(void *ctx);

extern t_avienc_t gAvienc[AVI_RecTypeCnt];

#endif
