#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_CONNECT_H_INCLUDED_
#define _NGX_CONNECT_H_INCLUDED_

/* RTMP message types */
#define NGX_RTMP_MSG_CHUNK_SIZE         1
#define NGX_RTMP_MSG_ABORT              2
#define NGX_RTMP_MSG_ACK                3
#define NGX_RTMP_MSG_USER               4
#define NGX_RTMP_MSG_ACK_SIZE           5
#define NGX_RTMP_MSG_BANDWIDTH          6
#define NGX_RTMP_MSG_EDGE               7
#define NGX_RTMP_MSG_AUDIO              8
#define NGX_RTMP_MSG_VIDEO              9
#define NGX_RTMP_MSG_AMF3_META          15
#define NGX_RTMP_MSG_AMF3_SHARED        16
#define NGX_RTMP_MSG_AMF3_CMD           17
#define NGX_RTMP_MSG_AMF_META           18
#define NGX_RTMP_MSG_AMF_SHARED         19
#define NGX_RTMP_MSG_AMF_CMD            20
#define NGX_RTMP_MSG_AGGREGATE          22
#define NGX_RTMP_MSG_MAX                22

#define NGX_RTMP_CONNECT                (NGX_RTMP_MSG_MAX + 1)
#define NGX_RTMP_DISCONNECT             (NGX_RTMP_MSG_MAX + 2)
#define NGX_RTMP_HANDSHAKE_DONE         (NGX_RTMP_MSG_MAX + 3)
#define NGX_RTMP_MAX_EVENT              (NGX_RTMP_MSG_MAX + 4)


#define NGX_RTMP_PUSH_CLIENT_START              11
#define NGX_RTMP_PUSH_CLIENT_DONE             	12	

ngx_int_t
ngx_rtmp_cmd_connect(ngx_rtmp_session_t *s, ngx_rtmp_connect_t *v);

int
ngx_rtmp_cmd_connect_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
    ngx_rtmp_chain_t *in);

void
ngx_rtmp_finalize_session(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_create_push_connect(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_send_push_connect(ngx_rtmp_session_t *s);

#endif
#endif
