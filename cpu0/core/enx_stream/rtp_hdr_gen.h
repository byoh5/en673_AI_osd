/* Copyright (c) 2013-16 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#ifndef __RTP_HDR_GEN__
#define __RTP_HDR_GEN__

/* The following definition is from RFC1890 */
#define RTP_PT_G711       100
#define RTP_PT_G726_32    100
#define RTP_PT_JPEG       26
#define RTP_VERSION       2

#define JPEG_QUANTABLE 128
#define JRTP_1STHDR_SIZE  (sizeof(rtp_hdr_t)+ sizeof(jpeghdr_t) + sizeof(jpeghdr_qtable_t))
#define JRTP_DEF_HDR_SIZE (sizeof(rtp_hdr_t)+ sizeof(jpeghdr_t))
/*
 * RTP data header from RFC1889
 */
// 4 byte
typedef struct {
	uint8 un8Magic;
	uint8 un8Channel;
	uint16 un16Length;
}__attribute__ ((packed)) rthInterleaved;

// 12 byte
typedef struct {
	uint32 un2RTPVersion:2;
	uint32 un1Padding:1;
	uint32 un1Extension:1;
	uint32 un4CSRCcount:4;
	uint32 un1Markerbit:1;
	uint32 un7Payloadtype:7;
	uint16 un16Sequencenum;
	uint32 un32Timestamp;
	uint32 un32SSIdentifier;
}__attribute__ ((packed)) rthRTPHeader;

// 2 byte
typedef struct {
	uint16 un1Fbit:1;
	uint16 un2NRI:2;
	uint16 un5Type:5;
	uint16 un1Startbit:1;
	uint16 un1Endbit:1;
	uint16 un1Forbiddenbit:1;
	uint16 un5Nalunittype:5;
}__attribute__ ((packed)) rthFUHeader;

#if 0
typedef struct {
	u32 version:2;   /* protocol version */
	u32 p:1;         /* padding flag */
	u32 x:1;         /* header extension flag */
	u32 cc:4;        /* CSRC count */
	u32 m:1;         /* marker bit */
	u32 pt:7;        /* payload type */
	u16 seq;         /* sequence number */
	u32 ts;          /* timestamp */
	u32 ssrc;        /* synchronization source */
} __attribute__ ((packed)) rtp_hdr_t;
#else
//rtp over tcp
typedef struct {
	u32 version:2;   /* protocol version */
	u32 p:1;         /* padding flag */
	u32 x:1;         /* header extension flag */
	u32 cc:4;        /* CSRC count */
	u32 m:1;         /* marker bit */
	u32 pt:7;        /* payload type */
	u16 seq;         /* sequence number */
	u32 ts;          /* timestamp */
	u32 ssrc;        /* synchronization source */
}__attribute__ ((packed)) rtp_hdr_t;

typedef struct {
	u32 magic :8;
	u32 ch    :8;
	u32 length:16;        
} __attribute__ ((packed)) tcp_over_hdr_t;
#endif

typedef struct  {
	u32 tspec:8;   /* type-specific field */
	u32 off:24;    /* fragment byte offset */
	u8 type;       /* id of jpeg decoder params */
	u8 q;          /* quantization factor (or table id) */
	u8 width;      /* frame width in 8 pixel blocks */
	u8 height;     /* frame height in 8 pixel blocks */
} __attribute__ ((packed)) jpeghdr_t;

typedef struct  {
  u8  mbz;
  u8  precision;
  u16 length;
} __attribute__ ((packed)) jpeghdr_qtable_t;

#endif /*__RTP_HDR_GEN__ */
