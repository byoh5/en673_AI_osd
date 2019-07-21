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

#ifndef _RTP_OVER_RTSP_H_
#define _RTP_OVER_RTSP_H_

#include "rtsp_control.h"

int
rtsp_port_change(UINT port);

void 
rtsp_disconnect(UINT ntype);

rtsp_con_t*
rtsp_add_user(struct tcp_pcb *pcb);

void
rtsp_del_user(struct tcp_pcb *pcb);

void 
rtsp_init(void);

/* jpeg streaming functions */
char 
rtp_tx_jpeg_over_tcp(void *ctx);

char
rtp_tx_jpeg(void *ctx);

void 
rtp_media_streamming(void);

void
rtp_register_streamingfn(u32 media, rtsp_con_t *prtsp_conn);

void
put_video_streams(UINT addr, UINT size, UINT flags, UINT ch, UINT ts);

void
put_audio_streams(UINT addr, UINT size, UINT audio_type);

#endif
