#include "board.h"
#include "ngx_rtmp_config.h"
#include "rtmp_session.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_STREAM_H_INCLUDED_
#define _NGX_STREAM_H_INCLUDED_

ngx_int_t
ngx_rtmp_cmd_create_stream(ngx_rtmp_session_t *s, ngx_rtmp_create_stream_t *v);

ngx_int_t
ngx_rtmp_cmd_create_stream_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h, ngx_chain_t *in);

ngx_chain_t *
ngx_rtmp_create_sample_access(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_status(ngx_rtmp_session_t *s, char *code, char* level, char *desc);


ngx_int_t
ngx_rtmp_send_sample_access(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_stream_begin(ngx_rtmp_session_t *s, uint32 msid);

ngx_int_t
ngx_rtmp_send_stream_begin(ngx_rtmp_session_t *s, uint32 msid);

int
getH264data(ngx_rtmp_session_t* s, h264_tx_info_t* ph264_info,sbuf_t *getsbuf);

ngx_chain_t *
ngx_rtmp_create_stream_eof(ngx_rtmp_session_t *s, uint32 msid);

ngx_int_t
ngx_rtmp_send_stream_eof(ngx_rtmp_session_t *s, uint32 msid);

ngx_chain_t *
ngx_rtmp_create_play_status(ngx_rtmp_session_t *s, char *code, char* level,
						  ngx_uint_t duration, ngx_uint_t bytes);

ngx_int_t
ngx_rtmp_send_play_status(ngx_rtmp_session_t *s, char *code, char* level,
						ngx_uint_t duration, ngx_uint_t bytes);

void ngx_rtmp_play_send(void);

ngx_int_t
ngx_rtmp_live_play(ngx_rtmp_session_t *s, ngx_rtmp_play_t *v);

ngx_int_t
ngx_rtmp_cmd_play_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
    ngx_chain_t *in);

ngx_rtmp_chain_t *
rtmp_create_ack_size(ngx_rtmp_session_t *s, uint32 ack_size);

int
ngx_rtmp_send_ack_size(ngx_rtmp_session_t *s, uint32 ack_size);

ngx_chain_t *
ngx_rtmp_create_release_stream(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_release_stream(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_publish(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_publish(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_fcpublish(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_fcpublish(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_stream(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_create_stream(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_setdataFrame(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_setdataFrame(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_protocol_message_handler(ngx_rtmp_session_t *s,
	 ngx_rtmp_header_t *h, ngx_chain_t *in);
#endif
#endif
