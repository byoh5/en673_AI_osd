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

#ifndef _RTSP_COMMAND_H_
#define _RTSP_COMMAND_H_

#include "rtsp_control.h"

/*jpeg file format information in our H/W codec */
#define JPEG_HDR_LEN        613    /* jpeg header size -- Add header  */
#define YQUNT_OFF           0x19   /* Y quantization offset in header of jpeg */
#define CQUNT_OFF           0x5a   /* Y quantization offset in header of jpeg */
#define QUNT_SIZE           64     /* length of q tbl */
#define YC_QUNT_SIZE        128    /* length of both of c and q tbl */

#define APP_CFG_IP_ADDR_STR_THIS_HOST         "0.0.0.0"
#define CONFIG_AUDIO_RTP_TEST 1 

#define CONFIG_AUDIO_RTP DEF_ENABLED

#define STATE_INIT          0
#define STATE_READY         1
#define STATE_PLAY          2
#define STATE_PAUSE         3
#define STATE_END           4 

#define SETUP               0
#define PLAY                1
#define PAUSE               2
#define TEARDOWN            4
#define OPTIONS             5
#define DESCRIBE            6
#define GET_PARAMETER		7
#define SET_PARAMETER		8
#define GET                 101
#define POST                102
#define RTSP_INTERLEAVED	150
#define RTCP                200
#define ERROR				255

#define RTPCMDUPPORTED      5

#define READBUFFER          512
#define MSGLEN              512
#define CMSGLEN             128
#define DEBUG

#define AUDIO_TX_READY      0
#define AUDIO_TX_DOING      1
#define AUDIO_TX_END        2

#define JPEG_TX_READY       0
#define JPEG_TX_DOING       1
#define JPEG_TX_END         2

#define H264_TX_READY       0
#define H264_TX_DOING       1
#define H264_TX_END         2

enum {
    RTSP_H264 = 0,
    RTSP_H264SUB = 1,
    RTSP_MJPEG = 9
};

enum {
    RTP_UDP,
    RTP_TCP,
    RTP_HTTP,
};

enum {
	VIDEO,
#ifdef __AUDIO__
	AUDIO,
#endif
#if (ONVIF_METADATA==1)
	METADATA,
#endif
#if (ONVIF_BACKCHANNEL==1)
	BACKCHANNEL
#endif
};

#define CRLF "\r\n"

#define RTSP_MSG	"RTSP/1.0 %s\r\n" \
					"CSeq: %d\r\n"

#define RTSP_MSG_S	"RTSP/1.0 %s\r\n" \
					"CSeq: %d\r\n" \
					"Session: %d\r\n"

// Date
#define RTSP_MSG_DATE	"Date: %s, %s %02d %04d %02d:%02d:%02d GMT\r\n"
// Authenticate-Basic
#define RTSP_MSG_BAUTH	"WWW-Authenticate: Basic realm=\"%s\"\r\n"
// Authenticate-Digest
#define RTSP_MSG_DAUTH	"WWW-Authenticate: Digest realm=\"%s\", nonce=\"%s\", stale=\"%s\"\r\n"
// RTP-Info
#define RTSP_MSG_RTPINFO "RTP-Info: url=%s;seq=%u;rtptime=%u\r\n"

#define RTSP_200	"200 OK"
#define RTSP_400	"400 Bad Request"
#define RTSP_401	"401 Unauthorized"
#define RTSP_404	"404 Not Found"
#define RTSP_500	"500 Internal Server Error"
#define RTSP_501	"501 Not Implimented"

#define HDR_UDP_SETUP_REPLY	"Transport: " \
								"RTP/AVP;" \
								"unicast;" \
								"client_port=%d-%d;" \
								"server_port=%d-%d;" \
								"ssrc=%8x;" \
								"mode=\"PLAY\"\r\n"

#define HDR_TCP_SETUP_REPLY	"Transport: " \
								"RTP/AVP/TCP;" \
								"unicast;" \
								"interleaved=%d-%d;" \
								"ssrc=%8x;" \
								"mode=\"PLAY\"\r\n"

#if 0
#define HDR_OPTION_REPLY1	"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER, GET_PARAMETER\r\n" \
							"Supported: play-basic, gzipped-messages, implicit-play\r\n" \
							"Server: EN673 RTSP Server\r\n\r\n"

#define HDR_OPTION_REPLY2	"Public: OPTIONS, DESCRIBE, SETUP, TEARDOWN, PLAY, PAUSE, SET_PARAMETER, GET_PARAMETER\r\n" \
							"Supported: play-basic, gzipped-messages, implicit-play\r\n" \
							"Server: EN673 RTSP Server\r\n\r\n"
#else
#define HDR_OPTION_REPLY1	"Public: OPTIONS, DESCRIBE, PLAY, PAUSE, SETUP, TEARDOWN, SET_PARAMETER, GET_PARAMETER\r\n"

#define HDR_OPTION_REPLY2	"Public: OPTIONS, DESCRIBE, PLAY, PAUSE, SETUP, TEARDOWN, SET_PARAMETER, GET_PARAMETER\r\n"
#endif

//function prototype
#include "lwip/pbuf.h"
extern void rtsp_close_rtp(rtsp_con_t *prtsp_conn);
extern void rtsp_close_rtcp(rtsp_con_t *prtsp_conn);
extern err_t rtsp_session_proc(rtsp_con_t *prtsp_conn, struct pbuf **inp, int *pend);

#endif //__RT__STREAM__H__
