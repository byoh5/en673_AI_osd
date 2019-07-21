#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_ARRAY_H_INCLUDED_
#define _NGX_ARRAY_H_INCLUDED_


static inline u_char *
ngx_strlchr(u_char *p, u_char *last, u_char c)
{
    while (p < last) {

        if (*p == c) {
            return p;
        }

        p++;
    }

    return NULL;
}

ngx_int_t
ngx_array_init(ngx_array_t *array, ngx_pool_t *pool, ngx_uint_t n, size_t size);

void *
ngx_array_push(ngx_array_t *a);

ngx_array_t *
ngx_array_create(ngx_pool_t *p, ngx_uint_t n, size_t size);

#endif
#endif
