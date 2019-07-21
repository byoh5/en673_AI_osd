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

#include "dev.h"

#include "enx_os.h"

#include "msg.h"
#include "user.h"

#include "enx_audio.h"
#include "rtsp_control.h"	// audio_control_t

#ifdef __AUDIO__
#if 0
#define PAGE_G711 800 /* g711 data size for 100 ms  */

#define AVI_ALIGN_SIZE		(8)
#define AUD_ALIGN_SIZE   	(4)
#define AUD_ALIGN(x) 		(((x) + AUD_ALIGN_SIZE - 1) & ~(AUD_ALIGN_SIZE - 1))

static u32 audio_timer_id = INVALID_TIMER_ID;

static UINT aud_strm_base = AUD_DATA_BASE;
static UINT aud_strm_end  = AUD_DATA_BASE + AUDIO_BUF_SIZE;

static u32 avi_save;

/**
	we save g711 block in DDIR and make irq to main cpu
*/
void audio_get_g711(void *ctx)
{
//	if(AUD_TX_NUM_G711 <= 0)
//		return;

	// G.711 copy
	UINT getSize = 266;//AUD_TX_NUM_G711;
//	aud_strm_base += AVI_ALIGN_SIZE;
//	memcpy((void *)aud_strm_base, (void *)BASE_TX_G711, getSize);

	// Tail update
//	AUD_TX_G711_TAIL = AUD_TX_G711_TAIL + getSize;

	// sbuf put(stream)
//	put_g711_streams(aud_strm_base, getSize);

	// sd save go
	if(avi_save == DEF_ON)
	{
		g711_mb.addr = aud_strm_base;
		g711_mb.size = getSize;
		g711_mb.type = 0;//i_frame;
		g711_mb.ready = 1;
		CPU_SHARED1 |= eIP1_H264_ENC;
		CPU_IRQ1 = 1;
	}

	// jump
	aud_strm_base += AUD_ALIGN(getSize);
	if((aud_strm_base+(getSize+AVI_ALIGN_SIZE+AUD_ALIGN_SIZE)) > AUD_DATA_END)
		aud_strm_base = AUD_DATA_BASE;
}

void enx_g711_set_sdsave(u32 value)
{
	avi_save = value;
}

void enx_g711_stop(void)
{
	if(audio_timer_id != INVALID_TIMER_ID)
	{
		timer_cancel_timeout(audio_timer_id);
		audio_timer_id = INVALID_TIMER_ID;
	}
}

void enx_g711_start(void)
{
	if(audio_timer_id == INVALID_TIMER_ID)
	    audio_timer_id = timer_sched_timeout_cb(33, TIMEOUT_PERIODIC, audio_get_g711, NULL);
}
#endif
#endif
