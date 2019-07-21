#ifndef __AVIENC_HEAP__H__
#define __AVIENC_HEAP__H__

#include "dev.h"
#include "board.h"
#ifdef AVIENC_HEAP_USE


typedef struct {
	char filename[60];
} encHData;

typedef struct {
	int usedsize;
	int capacity;
	encHData* node;
	encHData delHData;
} encHeap;

#define encHeap_isEmpty(h)	((h)->usedsize == 0)

#include "muxer_avienc.h"
extern encHeap* encHeap_create(int capacity);
extern void encHeap_destroy(encHeap* h);
extern void encHeap_deleteQueue(encHeap* h, char* filename);
extern int encHeap_addQueue(encHeap* h, char* filename);
extern int encHeap_checkQueue(encHeap* h);
extern	void exif_parser(void *penc);
#endif
#endif	// __AVIENC_HEAP__H__