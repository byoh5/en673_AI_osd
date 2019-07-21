#ifndef _SD_SAVE_H_
#define _SD_SAVE_H_

#include "dev.h"
#include "dev_types.h"
#include "fmt_avienc.h"
#include "stream_buffering.h"
#include "avienc_heap.h"

#define AVI_NORMAL_SW	0x1
#define AVI_EVENT_SW	0x2
#define AVI_MANUAL_SW	0x4
enum {
	eRecNormal = 0,
	eRecEvent = 1,
	eRecSnapshot = 2
};

#define AVI_ALIGN_SIZE   	4
#define AVI_ALIGN(x) 		(((x) + AVI_ALIGN_SIZE - 1) & ~(AVI_ALIGN_SIZE - 1))

void muxer_avienc_init(void);
void muxer_avienc_delete(void);
void muxer_avienc_h264Buffering(u32 h264_ptr, u32 h264_size, u32 idr, int total);
void muxer_avienc_g711Buffering(u32 g711_ptr, u32 g711_size, u32 flags, int total);
void muxer_avienc_txtsBuffering(u32 txts_ptr, u32 txts_size, u32 flags);
void muxer_avienc(void);
void muxer_avienc_task(void* pvParameters);

/**
	register irq and init sbuf for buffering
	this function should be called first before h264 working in cpu1 
*/
void muxer_init_buffering(void);

/**
    @return 
        1 : mounted.
        0 : nothing.
*/
int muxer_avienc_sdCardMounted(void);

typedef enum {
	AVIENC_OFF,//0
	AVIENC_IDLE,
	AVIENC_OPEN,	
	AVIENC_READ1,	
	AVIENC_READ2,	
	AVIENC_READ3,//5
	AVIENC_READ4,
	AVIENC_READ,
	AVIENC_READ_IDX,

	AVIENC_FIRST,
	AVIENC_WRITE,//10
	AVIENC_PAUSE,
	AVIENC_CLOSE,
	AVIENC_EXIT
}AVIENC_STATE;

typedef struct {
	uint32 flags; /*0 : i-frame, 1: p-frame */
	uint32 size;
	uint32 addr;
	uint32 state; // 0:data no, 1:data ok
} t_block_t;

typedef struct {
	uint8* buf;
	uint8* base;
	uint8* end;
	uint32 length;
	uint32 total;
}t_avienc_buf_t;

typedef int (*sd_free_t) (void *enc_t, UINT nMaxDelete, INT nFreeSize);
typedef void (*get_file_name_t) (void *enc_t);

typedef struct {
	int type;							// avi save type(AVI_PERIODIC_SW, AVI_EVENT_SW)
	AVIENC_STATE state;					// avienc state

#ifdef CONTIGUOUS_WRITE  
	DWORD org_sect;
	DWORD sect;
	DWORD end_sect;
	DWORD clust_offset;	
	DWORD clust_start;
	DWORD clust_end;
	DWORD maxdata_len;
	DWORD maxthumb_len;
#endif
#ifdef AVIENC_HEAP_USE
	encHeap *filenamelist;				// file path file name list
#endif	
	char dir_name[80];					// file path
	char file_format[80];				// file name format
	FIL fpavi;							// avi file
	char now_name[80];					// file name
#ifdef __JPEGEXIF__	
	char gpsdata[EXIF_TOTAL_LENGTH];	
#endif
	char pstrRMGMC[14][20];
	uint32 filesize;					// file size

	sbuf_t h264_sbuf;					// H.264 stream queue
	sbuf_t g711_sbuf;					// G.711 stream queue
	sbuf_t txt1_sbuf;					// TEXT1 stream queue
	sbuf_t txt2_sbuf;					// TEXT2 stream queue
	sbuf_t txt3_sbuf;					// TEXT3 stream queue
	sbuf_t txt4_sbuf;					// TEXT4 stream queue

	UINT stime;							// start time of avi encoding
	UINT etime;							// end time of avi encoding

	t_avienc_buf_t buffer;				// H.264 addr, size(buffer)
//	t_avienc_buf_t idxbuf;				// data index buffer
	t_avi_t aviinfo;					// H.264 FPS, Frame Size, FIle Size, etc...
	t_block_t datablock;				// addr, size, flags

	// INDEX Info
	UCHAR *idx_info;					// avi index buffer
	UINT idx_len;						// length of idx_info

	// pointer to function
	sd_free_t       pf_sd_free;			// pointer to function to make space for SD CARD
	get_file_name_t pf_get_file_name;	// pointer to function to make name for avi files
	AVI_PARSER_t playback;
	
}t_avienc_t;

extern void muxer_avienc_normal_go(void);
extern int muxer_avienc_normal_state(void);
extern void muxer_avienc_event_go(void);
extern int muxer_avienc_event_state(void);
extern void muxer_avienc_manual_go(void);
extern int muxer_avienc_manual_state(void);

extern t_avienc_t *muxsr_avienc_get_t_avienc_t(void);
extern void muxer_avienc_process(t_avienc_t *pavi);
#ifdef __JPEG__
#ifdef __JPEG_SDSAVE__
extern void muxer_jpegstill_process(void *ctx);
#endif
extern int jpeg_thumbnail_save(t_avienc_t *pavi);
#endif

extern t_avienc_t enc_nor;	// 주행 상시
extern t_avienc_t enc_evt;	// 주행 이벤트
extern t_avienc_t enc_mar;	// 주행 메뉴얼
extern t_avienc_t *enc_ing;
extern t_avienc_t enc_play;

extern int g_normalgo;
extern int g_eventgo;
extern int g_manualgo;
extern void muxer_aviplayback_task(void* pvParameters);
extern void IsrH264Enc(void *arg);
extern UINT nSD_CARD_SIZE;



extern xSemaphoreHandle bufSem;
#endif
