#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT

#define NGX_RTMP_MP4_BUFLEN_ADDON       1000

ngx_rtmp_chain_t *
rtmp_create_bandwidth(ngx_rtmp_session_t *s, uint32 ack_size,
                          uint8 limit_type);

int
ngx_rtmp_send_bandwidth(ngx_rtmp_session_t *s, uint32 ack_size,
                        uint8 limit_type);

ngx_rtmp_chain_t *
rtmp_create_chunk_size(ngx_rtmp_session_t *s, uint32 chunk_size);

int
ngx_rtmp_send_chunk_size(ngx_rtmp_session_t *s, uint32 chunk_size);

int
ngx_rtmp_send_amf(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
                  ngx_rtmp_amf_elt_t *elts, size_t nelts);
ngx_int_t
ngx_rtmp_h264_send_meta(ngx_rtmp_session_t *s);

ngx_chain_t *
ngx_rtmp_append_shared_bufs(ngx_rtmp_core_srv_conf_t *cscf,
    ngx_chain_t *head, ngx_chain_t *in);

ngx_int_t
ngx_rtmp_h264_send(ngx_rtmp_session_t *s, ngx_uint_t *ts);

ngx_int_t
ngx_rtmp_g711_send(ngx_rtmp_session_t *s, ngx_uint_t *ts);

void
ngx_rtmp_free_shared_chain(ngx_rtmp_core_srv_conf_t *cscf, ngx_rtmp_chain_t *in);

char*
ngx_rtmp_message_type(uint8 type);
 
void
ngx_rtmp_prepare_message(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
		ngx_rtmp_header_t *lh, ngx_rtmp_chain_t *out);

#endif
