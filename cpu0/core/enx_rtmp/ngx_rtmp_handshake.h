#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_HANDSHAKE_H_INCLUDED_
#define _NGX_HANDSHAKE_H_INCLUDED_

#define SHA224_DIGEST_LENGTH	28
#define SHA256_DIGEST_LENGTH	32

#define NGX_RTMP_HANDSHAKE_KEYLEN                   SHA256_DIGEST_LENGTH
#define NGX_RTMP_HANDSHAKE_BUFSIZE                  1538



#define NGX_RTMP_HANDSHAKE_SERVER_RECV_CHALLENGE    1
#define NGX_RTMP_HANDSHAKE_SERVER_SEND_CHALLENGE    2
#define NGX_RTMP_HANDSHAKE_SERVER_SEND_RESPONSE     3
#define NGX_RTMP_HANDSHAKE_SERVER_RECV_RESPONSE     4
#define NGX_RTMP_HANDSHAKE_SERVER_DONE              5

#define NGX_RTMP_HANDSHAKE_CLIENT_SEND_CHALLENGE    6
#define NGX_RTMP_HANDSHAKE_CLIENT_RECV_CHALLENGE    7
#define NGX_RTMP_HANDSHAKE_CLIENT_RECV_RESPONSE     8
#define NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE     9
#define NGX_RTMP_HANDSHAKE_CLIENT_DONE              10

extern UINT s1size;

void rtmp_calc_digest(const uint8 *src, int len, int gap,
							const uint8 *key, int keylen, uint8 *dst);
int
ngx_rtmp_make_digest(ngx_str_t *key1, ngx_rtmp_buf_t *src1,
	 u_char *skip1, u_char *dst1);

int
ngx_rtmp_find_digest(ngx_rtmp_buf_t *b, ngx_str_t *key, size_t base);
int
rtmp_write_digest(ngx_rtmp_buf_t *b, ngx_str_t *key, size_t base);

void
ngx_rtmp_fill_random_buffer(ngx_rtmp_buf_t *b);

int
ngx_rtmp_handshake_create_challenge(ngx_rtmp_session_t *s,
 const u_char version[4], ngx_str_t *key);

int
rtmp_handshake_create_response(ngx_rtmp_session_t *s);

void
ngx_rtmp_handshake_done(ngx_rtmp_session_t *s);

void
ngx_rtmp_handshake_send(ngx_rtmp_session_t       *s);

void
ngx_rtmp_free_handshake_buffers(ngx_rtmp_session_t *s);

ngx_buf_t *
ngx_rtmp_alloc_handshake_buffer(ngx_rtmp_session_t *s);

void
ngx_rtmp_handshake(ngx_rtmp_session_t *s);

ngx_int_t
ngx_rtmp_handshake_parse_challenge(ngx_rtmp_session_t *s,
    ngx_str_t *peer_key, ngx_str_t *key);

void ngx_rtmp_handshake_recv(ngx_rtmp_session_t *s);

void ngx_rtmp_client_handshake(ngx_rtmp_session_t *s);

#endif
#endif
