/* Copyright (c) 2013 Eyenix Corporation. All rights reserved.
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

#ifndef _ENX_AUDIO_H_
#define _ENX_AUDIO_H_

#define G711_RX_COUNT	40

typedef struct {
	UINT addr;
	UINT size;
}g711_data;

typedef struct{
	UINT strm_base;
	UINT old_seq;
	UINT state;
	UINT length;

	UINT old_strm_base;
	UINT old_size;

	UINT over_strm_base;
	UINT over_size;

	UINT head;								
	UINT tail;								
	UINT tot_num;    // total num of buffer

	g711_data sbuf[G711_RX_COUNT];
}G711Buffer;

extern G711Buffer g711rx;

#if 0
extern void enx_g711_set_sdsave(u32 value);
extern void enx_g711_stop(void);
extern void enx_g711_start(void);
#endif

#endif
