#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT
 
#ifndef _NGX_AMF_H_INCLUDED_
#define _NGX_AMF_H_INCLUDED_

 /* basic types */
#define RTMP_AMF_NUMBER             0x00
#define RTMP_AMF_BOOLEAN            0x01
#define RTMP_AMF_STRING             0x02
#define RTMP_AMF_OBJECT             0x03
#define RTMP_AMF_NULL               0x05
#define RTMP_AMF_ARRAY_NULL         0x06
#define RTMP_AMF_MIXED_ARRAY        0x08
#define RTMP_AMF_END                0x09
#define RTMP_AMF_ARRAY              0x0a
 
 /* extended types */
#define RTMP_AMF_INT8               0x0100
#define RTMP_AMF_INT16              0x0101
#define RTMP_AMF_INT32              0x0102
#define RTMP_AMF_VARIANT_           0x0103
 
 /* r/w flags */
#define RTMP_AMF_OPTIONAL           0x1000
#define RTMP_AMF_TYPELESS           0x2000
#define RTMP_AMF_CONTEXT            0x4000
 
#define RTMP_AMF_VARIANT            (RTMP_AMF_VARIANT_\
										 |RTMP_AMF_TYPELESS)


/* basic types */
#define NGX_RTMP_AMF_NUMBER             0x00
#define NGX_RTMP_AMF_BOOLEAN            0x01
#define NGX_RTMP_AMF_STRING             0x02
#define NGX_RTMP_AMF_OBJECT             0x03
#define NGX_RTMP_AMF_NULL               0x05
#define NGX_RTMP_AMF_ARRAY_NULL         0x06
#define NGX_RTMP_AMF_MIXED_ARRAY        0x08
#define NGX_RTMP_AMF_END                0x09
#define NGX_RTMP_AMF_ARRAY              0x0a

/* extended types */
#define NGX_RTMP_AMF_INT8               0x0100
#define NGX_RTMP_AMF_INT16              0x0101
#define NGX_RTMP_AMF_INT32              0x0102
#define NGX_RTMP_AMF_VARIANT_           0x0103	 
	 
 
 
 /* r/w flags */
#define NGX_RTMP_AMF_OPTIONAL           0x1000
#define NGX_RTMP_AMF_TYPELESS           0x2000
#define NGX_RTMP_AMF_CONTEXT            0x4000
 
#define NGX_RTMP_AMF_VARIANT            (NGX_RTMP_AMF_VARIANT_\
										 |NGX_RTMP_AMF_TYPELESS)

 int
ngx_rtmp_amf_get(ngx_rtmp_amf_ctx_t *ctx, void *p, size_t n);
 int
rtmp_amf_is_compatible_type(uint8 t1, uint8 t2);
  void*
rtmp_amf_copy(void *dst, void* src, size_t len);
  void*
ngx_rtmp_amf_reverse_copy(void *dst, void* src, size_t len);
 int
rtmp_amf_read_object(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts);
 int
rtmp_amf_read_variant(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts);
 int
rtmp_amf_read_array(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts);
int
ngx_rtmp_amf_read(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts);
int
ngx_rtmp_receive_amf(ngx_rtmp_session_t *s, ngx_rtmp_chain_t *in,
        ngx_rtmp_amf_elt_t *elts, size_t nelts);
        
ngx_rtmp_chain_t *
ngx_rtmp_create_amf(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
					ngx_rtmp_amf_elt_t *elts, size_t nelts);

ngx_chain_t *
ngx_rtmp_alloc_amf_buf(void *arg);

ngx_int_t
ngx_rtmp_append_amf(ngx_rtmp_session_t *s, ngx_chain_t **first, ngx_chain_t **last,
                    ngx_rtmp_amf_elt_t *elts, size_t nelts);
ngx_chain_t *
rtmp_alloc_amf_buf(void *arg);

ngx_int_t
ngx_rtmp_amf_put(ngx_rtmp_amf_ctx_t *ctx, void *p, size_t n);

ngx_int_t
ngx_rtmp_amf_write_object(ngx_rtmp_amf_ctx_t *ctx,
	 ngx_rtmp_amf_elt_t *elts, size_t nelts);

ngx_int_t
ngx_rtmp_amf_write_array(ngx_rtmp_amf_ctx_t *ctx,
    ngx_rtmp_amf_elt_t *elts, size_t nelts);

ngx_int_t
ngx_rtmp_amf_write(ngx_rtmp_amf_ctx_t *ctx,
    ngx_rtmp_amf_elt_t *elts, size_t nelts);

int
rtmp_append_amf(ngx_rtmp_session_t *s,
                ngx_rtmp_chain_t **first, ngx_rtmp_chain_t **last,
                ngx_rtmp_amf_elt_t *elts, size_t nelts);
ngx_int_t
ngx_rtmp_amf_message_handler(ngx_rtmp_session_t *s,
        ngx_rtmp_header_t *h, ngx_chain_t *in);
#endif
#endif
