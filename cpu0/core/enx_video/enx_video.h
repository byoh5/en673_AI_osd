/* Copyright (c) 2013-15 Eyenix Corporation. All rights reserved.
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

#ifndef _ENX_VIDEO_H_
#define _ENX_VIDEO_H_

#define VLOCK_NONE			0x000
#define VLOCK_GO			0x001
#define VLOCK_CH0_ENC_START	0x002
#define VLOCK_CH0_ENC_END	0x004
#define VLOCK_CH0_VLC_START	0x008
#define VLOCK_CH0_VLC_END	0x010
#define ULOCK_CH1_ENC_START	0x020
#define ULOCK_CH1_ENC_END	0x040
#define ULOCK_CH1_VLC_START	0x080
#define ULOCK_CH1_VLC_END	0x100

#define VLOCK_CH0_RAW_UP0	0x200
#define VLOCK_CH0_RAW_UP1	0x400
#define VLOCK_CH0_RAW_UP2	0x800

extern BYTE nFrameRateDiv[6][12];

extern UINT enx_frame_value(void);

#endif
