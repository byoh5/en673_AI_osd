#include "ngx_rtmp_amf.h"
#include "ngx_rtmp_core.h"
#include "ngx_rtmp_connect.h"

#ifdef RTMP_SUPPORT

 int
ngx_rtmp_amf_get(ngx_rtmp_amf_ctx_t *ctx, void *p, size_t n)
{printf("%s\n",__func__);
    size_t          size;
    ngx_rtmp_chain_t    *l;
    size_t          offset;
    u_char         *pos, *last;
#ifdef NGX_DEBUG
    void           *op = p;
    size_t          on = n;
#endif

    if (!n)
        return RTMP_OK;
	if(ctx->link == NULL)
		printf("\nngx_rtmp_amf_get ctx->link NULL\n");

    for(l = ctx->link, offset = ctx->offset; l; l = l->next, offset = 0) {
		
        pos  = l->buf->pos + offset;
        last = l->buf->last;
//		printf("\nngx_rtmp_amf_get pos %x %x %x %x offset %d last %x %x %x %x n %d diff %d\n", *pos,*(pos+1), *(pos+2), *(pos+3), offset, *(last-3), *(last-2), *(last-1), *(last), n, last - (pos + n));

        if (last >= pos + n) {
            if (p) {
                p = ngx_cpymem(p, pos, n);
                //memcpy(p, pos, n);
				//printf("(%x %x)\n", *((u_char*)p), *pos);
            }
            ctx->offset = offset + n;
            ctx->link = l;

#ifdef NGX_DEBUG
			printf("\nread %x %d\n", *((u_char*)op), on);
#endif
			
            return RTMP_OK;
        }
		else
		{
			printf("\n%s RTMP DONE \n", __func__);
			return RTMP_DONE;
		}

        size = last - pos;

        if (p) {
            p = ngx_cpymem(p, pos, size);
            //memcpy(p, pos, size);
        }

        n -= size;
    }

    printf("\nAMF read eof (%d)", n);

    return RTMP_DONE;
}

 int
rtmp_amf_is_compatible_type(uint8 t1, uint8 t2)
{
    return t1 == t2
        || (t1 == RTMP_AMF_OBJECT && t2 == RTMP_AMF_MIXED_ARRAY)
        || (t2 == RTMP_AMF_OBJECT && t1 == RTMP_AMF_MIXED_ARRAY);
}

  void*
rtmp_amf_copy(void *dst, void* src, size_t len)
{
    size_t  k;

    if (dst == NULL || src == NULL) {
        return NULL;
    }

    for(k = 0; k < len; ++k) {
        ((u_char*)dst)[k] = ((u_char*)src)[k];
    }

    return dst;
}

 void*
ngx_rtmp_amf_reverse_copy(void *dst, void* src, size_t len)
{
#if 0
	size_t  k;

    if (dst == NULL || src == NULL) {
        return NULL;
    }

    for(k = 0; k < len; ++k) {
        ((u_char*)dst)[k] = ((u_char*)src)[len - 1 - k];
    }

    return dst;
#else
	return rtmp_amf_copy(dst, src, len);
#endif
}

 int
rtmp_amf_read_object(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts)
{
    uint8                 type;
    uint16                len;
    size_t                  n, namelen, maxlen;
    int               rc;
    u_char                  buf[2];

    maxlen = 0;
    for(n = 0; n < nelts; ++n) {
        namelen = elts[n].name.len;
        if (namelen > maxlen)
            maxlen = namelen;
    }

    for( ;; ) {

#if !(NGX_WIN32)
        char    name[maxlen];
#else
        char    name[1024];
        if (maxlen > sizeof(name)) {
            return RTMP_ERROR;
        }
#endif
        /* read key */				
		//printf("_%x_",*(ctx->link->buf->pos+ctx->offset));
		if(*(ctx->link->buf->pos+ctx->offset) == 0xC3)
			ctx->offset++;
				
        switch (ngx_rtmp_amf_get(ctx, buf, 2)) {
        case RTMP_DONE:
            /* Envivio sends unfinalized arrays */
            return RTMP_OK;
        case RTMP_OK:
            break;
        default:
			
            return RTMP_ERROR;
        }

        ngx_rtmp_amf_reverse_copy(&len, buf, 2);

        if (!len)
            break;

        if (len <= maxlen) {
            rc = ngx_rtmp_amf_get(ctx, name, len);

        } else {
            rc = ngx_rtmp_amf_get(ctx, name, maxlen);
            if (rc != RTMP_OK)
            	{
            	
                return RTMP_ERROR;
            	}
            rc = ngx_rtmp_amf_get(ctx, 0, len - maxlen);
        }
		if(rc == RTMP_DONE)
			return RTMP_OK;

        if (rc != RTMP_OK)
        	{
            return RTMP_ERROR;
        	}

        /* TODO: if we require array to be sorted on name
         * then we could be able to use binary search */
        for(n = 0; n < nelts
                && (len != elts[n].name.len
                    || rtmp_strncmp(name, elts[n].name.data, len));
                ++n);

        if (ngx_rtmp_amf_read(ctx, n < nelts ? &elts[n] : NULL, 1) != RTMP_OK)
        	{
            return RTMP_ERROR;
        	}
    }

    if (ngx_rtmp_amf_get(ctx, &type, 1) != RTMP_OK
        || type != RTMP_AMF_END)
    {
        return RTMP_ERROR;
    }

    return RTMP_OK;
}

 int
rtmp_amf_read_variant(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts)
{
    uint8                 type;
    int              rc;
    size_t                  n;
    ngx_rtmp_amf_elt_t      elt;

    rc = ngx_rtmp_amf_get(ctx, &type, 1);
    if (rc != NGX_OK) {
        return rc;
    }

    rtmp_memzero(&elt, sizeof(elt));
    for (n = 0; n < nelts; ++n, ++elts) {
        if (type == elts->type) {
            elt.data = elts->data;
            elt.len  = elts->len;
        }
    }

    elt.type = type | RTMP_AMF_TYPELESS;

    return ngx_rtmp_amf_read(ctx, &elt, 1);
}


 int
rtmp_amf_read_array(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts)
{
    uint32                len;
    size_t                  n;
    u_char                  buf[4];

    /* read length */
    if (ngx_rtmp_amf_get(ctx, buf, 4) != RTMP_OK)
        return RTMP_ERROR;

    ngx_rtmp_amf_reverse_copy(&len, buf, 4);

    for (n = 0; n < len; ++n) {
        if (ngx_rtmp_amf_read(ctx, n < nelts ? &elts[n] : NULL, 1) != RTMP_OK)
            return RTMP_ERROR;
    }

    return RTMP_OK;
}


int
ngx_rtmp_amf_read(ngx_rtmp_amf_ctx_t *ctx, ngx_rtmp_amf_elt_t *elts,
        size_t nelts)
{
    void                  *data;
    int                   type;
    uint8                 type8;
    size_t                n;
    uint16                len;
    int                   rc;
    u_char                buf[8];
    uint32                max_index;
	
	//printf("ngx_rtmp_amf_read  nelts %d\n",nelts);
	
    for(n = 0; n < nelts; ++n) {
		//printf("ngx_rtmp_amf_read  n %d\n",n);
		//printf("ngx_rtmp_amf_read  n %d elts->type %d \n",n,elts->type);

        if (elts && elts->type & RTMP_AMF_TYPELESS) {
            type = elts->type & ~RTMP_AMF_TYPELESS;
            data = elts->data;
			//printf("ngx_rtmp_amf_read  RTMP_AMF_TYPELESS\n",n);

        } else {
            switch (ngx_rtmp_amf_get(ctx, &type8, 1)) {
                case RTMP_DONE:
                    if (elts->type & RTMP_AMF_OPTIONAL) {
                        return RTMP_OK;
                    }
				case RTMP_OK:
					break;
                case RTMP_ERROR:
					printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
            }
            type = type8;
			//printf("[type] %x\n",type);
            data = (elts &&
                    rtmp_amf_is_compatible_type(
                                 (uint8) (elts->type & 0xff), (uint8) type))
                ? elts->data
                : NULL;
			if(data == NULL)
				{
					//printf("data is null\n");
				}
            if (elts && (elts->type & RTMP_AMF_CONTEXT)) {
                if (data) {
					//printf("data = *ctx\n");
                    *(ngx_rtmp_amf_ctx_t *) data = *ctx;
                }
                data = NULL;
				//printf("data = NULL\n");
            }
        }

        switch (type) {
            case RTMP_AMF_NUMBER:
                if (ngx_rtmp_amf_get(ctx, buf, 8) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                ngx_rtmp_amf_reverse_copy(data, buf, 8);
                break;

            case RTMP_AMF_BOOLEAN:
                if (ngx_rtmp_amf_get(ctx, data, 1) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                break;

            case RTMP_AMF_STRING:
                if (ngx_rtmp_amf_get(ctx, buf, 2) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                ngx_rtmp_amf_reverse_copy(&len, buf, 2);

                if (data == NULL) {
                    rc = ngx_rtmp_amf_get(ctx, data, len);

                } else if (elts->len <= len) {
                    rc = ngx_rtmp_amf_get(ctx, data, elts->len - 1);
                    if (rc != RTMP_OK)
                    {//printf("%s %d\n",__func__,__LINE__);
                        return RTMP_ERROR;
                    }
                    ((char*)data)[elts->len - 1] = 0;
                    rc = ngx_rtmp_amf_get(ctx, NULL, len - elts->len + 1);

                } else {
                    rc = ngx_rtmp_amf_get(ctx, data, len);
                    ((char*)data)[len] = 0;
                }

                if (rc != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }

                break;

            case RTMP_AMF_NULL:
            case RTMP_AMF_ARRAY_NULL:
                break;

            case RTMP_AMF_MIXED_ARRAY:
                if (ngx_rtmp_amf_get(ctx, &max_index, 4) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }

            case RTMP_AMF_OBJECT:
                if (rtmp_amf_read_object(ctx, data,
                    data && elts ? elts->len / sizeof(ngx_rtmp_amf_elt_t) : 0
                    ) != RTMP_OK)
                {
               		//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                break;

            case RTMP_AMF_ARRAY:
                if (rtmp_amf_read_array(ctx, data,
                    data && elts ? elts->len / sizeof(ngx_rtmp_amf_elt_t) : 0
                    ) != RTMP_OK)
                {
                	//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                break;

            case RTMP_AMF_VARIANT_:
                if (rtmp_amf_read_variant(ctx, data,
                    data && elts ? elts->len / sizeof(ngx_rtmp_amf_elt_t) : 0
                    ) != RTMP_OK)
                {
                	//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                break;

            case RTMP_AMF_INT8:
                if (ngx_rtmp_amf_get(ctx, data, 1) != RTMP_OK) {
					
                    return RTMP_ERROR;
                }
                break;

            case RTMP_AMF_INT16:
                if (ngx_rtmp_amf_get(ctx, buf, 2) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                ngx_rtmp_amf_reverse_copy(data, buf, 2);
                break;

            case RTMP_AMF_INT32:
                if (ngx_rtmp_amf_get(ctx, buf, 4) != RTMP_OK) {
					//printf("%s %d\n",__func__,__LINE__);
                    return RTMP_ERROR;
                }
                ngx_rtmp_amf_reverse_copy(data, buf, 4);
                break;

            case RTMP_AMF_END:
                return RTMP_OK;

            default:
				//printf("%s %d\n",__func__,__LINE__);
                return RTMP_ERROR;
        }

        if (elts) {
			
            ++elts;
        }
    }

    return RTMP_OK;
}



int
ngx_rtmp_receive_amf(ngx_rtmp_session_t *s, ngx_rtmp_chain_t *in,
        ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    ngx_rtmp_amf_ctx_t     act;

    rtmp_memzero(&act, sizeof(act));
    act.link = in;
//    act.log = s->connection->log;

    return ngx_rtmp_amf_read(&act, elts, nelts);
}



ngx_rtmp_chain_t *
ngx_rtmp_create_amf2(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h, ngx_rtmp_header_t *lh,
                    ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    ngx_rtmp_chain_t                *first;
    int                   rc;
    ngx_rtmp_core_srv_conf_t   *cscf;

    //printf("create: amf nelts=%ld\n", nelts);

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    first = NULL;

    rc = rtmp_append_amf(s, &first, NULL, elts, nelts);

    if (rc != RTMP_OK && first) {
        ngx_rtmp_free_shared_chain(cscf, first);
        first = NULL;
    }

    if (first) {
        ngx_rtmp_prepare_message(s, h, lh, first);
    }
	

    return first;
}


ngx_rtmp_chain_t *
ngx_rtmp_create_amf(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
                    ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    ngx_rtmp_chain_t                *first;
    int                   rc;
    ngx_rtmp_core_srv_conf_t   *cscf;

    printf("create: amf nelts=%d\r\n", nelts);

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    first = NULL;

    rc = rtmp_append_amf(s, &first, NULL, elts, nelts);

    if (rc != RTMP_OK && first) {
        ngx_rtmp_free_shared_chain(cscf, first);
        first = NULL;
    }

    if (first) {
        ngx_rtmp_prepare_message(s, h, NULL, first);
    }
	

    return first;
}

 ngx_chain_t *
ngx_rtmp_alloc_amf_buf(void *arg)
{
    return ngx_rtmp_alloc_shared_buf((ngx_rtmp_core_srv_conf_t *)arg);
}



/* AMF sender */

/* NOTE: this function does not free shared bufs on error */
ngx_int_t
ngx_rtmp_append_amf(ngx_rtmp_session_t *s,
                    ngx_chain_t **first, ngx_chain_t **last,
                    ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    ngx_rtmp_amf_ctx_t          act;
    ngx_rtmp_core_srv_conf_t   *cscf;
    ngx_int_t                   rc;

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    memset(&act, 0, sizeof(act));
    act.arg = cscf;
    act.alloc = ngx_rtmp_alloc_amf_buf;
//    act.log = s->connection->log;

    if (first) {
        act.first = *first;
    }

    if (last) {
        act.link = *last;
    }

    rc = ngx_rtmp_amf_write(&act, elts, nelts);

    if (first) {
        *first = act.first;
    }

    if (last) {
        *last = act.link;
    }

    return rc;
}


 ngx_chain_t *
rtmp_alloc_amf_buf(void *arg)
{
    return ngx_rtmp_alloc_shared_buf((ngx_rtmp_core_srv_conf_t *)arg);
}

 ngx_int_t
ngx_rtmp_amf_put(ngx_rtmp_amf_ctx_t *ctx, void *p, size_t n)
{
    ngx_buf_t       *b;
    size_t          size;
    ngx_chain_t    *l, *ln;

#ifdef NGX_DEBUG
    ngx_rtmp_amf_debug("write", ctx->log, (u_char*)p, n);
#endif
    l = ctx->link;

	//if(ctx->link == NULL)
		//printf("%s ctx->link null\n",__func__);
	
    if (ctx->link && ctx->first == NULL) {
        ctx->first = ctx->link;
    }
	//printf("%s %x %x %x %x n %d \n",__func__,*((u_char*)p),*(((u_char*)p)+1),*(((u_char*)p)+2),*(((u_char*)p)+3),n);

    while(n) {
        b = l ? l->buf : NULL;
		//if(b == NULL)
		//printf("%s b == NULL \n",__func__);

        if (b == NULL || b->last == b->end) {

            ln = ctx->alloc(ctx->arg);
            if (ln == NULL) {
				//printf("%s ln == NULL \n",__func__);
                return NGX_ERROR;
            }

            if (ctx->first == NULL) {
				//printf("%s ctx->first == NULL \n",__func__);
                ctx->first = ln;
            }

            if (l) {
				//printf("%s l->next = ln \n",__func__);
                l->next = ln;
            }

            l = ln;
            ctx->link = l;
            b = l->buf;
        }

        size = b->end - b->last;
		//printf("%s size %d\n",__func__,size);

        if (size >= n) {
            b->last = ngx_cpymem(b->last, p, n);
            return NGX_OK;
        }

        b->last = ngx_cpymem(b->last, p, size);
        p = (u_char*)p + size;
		
        n -= size;
    }
	//printf("%s END\n",__func__);

    return NGX_OK;
}

 ngx_int_t
ngx_rtmp_amf_write_object(ngx_rtmp_amf_ctx_t *ctx,
        ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    uint16                len;
    size_t                  n;
    u_char                  buf[2];

    for(n = 0; n < nelts; ++n) {

        len = (uint16) elts[n].name.len;

        if (ngx_rtmp_amf_put(ctx,
                    ngx_rtmp_amf_reverse_copy(buf,
                        &len, 2), 2) != NGX_OK)
        {
            return NGX_ERROR;
        }

        if (ngx_rtmp_amf_put(ctx, elts[n].name.data, len) != NGX_OK) {
            return NGX_ERROR;
        }

        if (ngx_rtmp_amf_write(ctx, &elts[n], 1) != NGX_OK) {
            return NGX_ERROR;
        }
    }

    if (ngx_rtmp_amf_put(ctx, "\0\0", 2) != NGX_OK) {
        return NGX_ERROR;
    }

    return NGX_OK;
}

 ngx_int_t
ngx_rtmp_amf_write_array(ngx_rtmp_amf_ctx_t *ctx,
        ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    uint32                len;
    size_t                  n;
    u_char                  buf[4];
	
	//printf("%s nelts %d\n",__func__,nelts);
	
    len = nelts;
    if (ngx_rtmp_amf_put(ctx,
                ngx_rtmp_amf_reverse_copy(buf,
                    &len, 4), 4) != NGX_OK)
    {//printf("%s ngx_rtmp_amf_put NGX_ERROR\n",__func__);
        return NGX_ERROR;
    }

    for(n = 0; n < nelts; ++n) {
        if (ngx_rtmp_amf_write(ctx, &elts[n], 1) != NGX_OK) {
            return NGX_ERROR;
        }
    }
	//printf("%sEND\n",__func__);

    return NGX_OK;
}


ngx_int_t
ngx_rtmp_amf_write(ngx_rtmp_amf_ctx_t *ctx,
        ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    size_t                  n;
    ngx_int_t               type;
    uint8                 type8;
    void                   *data;
    uint16                len;
    uint32                max_index;
    u_char                  buf[8];
	
	//printf(" nelts %d\n",nelts);
	
    for(n = 0; n < nelts; ++n) {

        type = elts[n].type;
        data = elts[n].data;
        len  = (uint16) elts[n].len;
		//printf(" type %d len %d\n",type ,len);

        if (type & NGX_RTMP_AMF_TYPELESS) {
            type &= ~NGX_RTMP_AMF_TYPELESS;
        } else {
            type8 = (uint8)type;
            if (ngx_rtmp_amf_put(ctx, &type8, 1) != NGX_OK)
                return NGX_ERROR;
        }

        switch(type) {
            case NGX_RTMP_AMF_NUMBER:
                if (ngx_rtmp_amf_put(ctx,
                            ngx_rtmp_amf_reverse_copy(buf,
                                data, 8), 8) != NGX_OK)
                {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_BOOLEAN:
                if (ngx_rtmp_amf_put(ctx, data, 1) != NGX_OK) {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_STRING:
                if (len == 0 && data) {
                    len = (uint16) ngx_strlen((u_char*) data);
                }

                if (ngx_rtmp_amf_put(ctx,
                            ngx_rtmp_amf_reverse_copy(buf,
                                &len, 2), 2) != NGX_OK)
                {
                    return NGX_ERROR;
                }

                if (ngx_rtmp_amf_put(ctx, data, len) != NGX_OK) {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_NULL:
            case NGX_RTMP_AMF_ARRAY_NULL:
                break;

            case NGX_RTMP_AMF_MIXED_ARRAY:
                max_index = elts[n].len / sizeof(ngx_rtmp_amf_elt_t);
                if (ngx_rtmp_amf_put(ctx, &max_index, 4) != NGX_OK) {
                    return NGX_ERROR;
                }
			
				

            case NGX_RTMP_AMF_OBJECT:
                type8 = NGX_RTMP_AMF_END;
                if (ngx_rtmp_amf_write_object(ctx, data,
                        elts[n].len / sizeof(ngx_rtmp_amf_elt_t)) != NGX_OK
                    || ngx_rtmp_amf_put(ctx, &type8, 1) != NGX_OK)
                {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_ARRAY:
                if (ngx_rtmp_amf_write_array(ctx, data,
                        elts[n].len / sizeof(ngx_rtmp_amf_elt_t)) != NGX_OK)
                {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_INT8:
                if (ngx_rtmp_amf_put(ctx, data, 1) != NGX_OK) {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_INT16:
                if (ngx_rtmp_amf_put(ctx,
                            ngx_rtmp_amf_reverse_copy(buf,
                                data, 2), 2) != NGX_OK)
                {
                    return NGX_ERROR;
                }
                break;

            case NGX_RTMP_AMF_INT32:
                if (ngx_rtmp_amf_put(ctx,
                            ngx_rtmp_amf_reverse_copy(buf,
                                data, 4), 4) != NGX_OK)
                {
                    return NGX_ERROR;
                }
                break;

            default:
                return NGX_ERROR;
        }
    }

    return NGX_OK;
}

/* AMF sender */

/* NOTE: this function does not free shared bufs on error */

int
rtmp_append_amf(ngx_rtmp_session_t *s,
                    ngx_rtmp_chain_t **first, ngx_rtmp_chain_t **last,
                    ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    ngx_rtmp_amf_ctx_t          act;
    ngx_rtmp_core_srv_conf_t   *cscf;
    int                   rc;
	//printf("%s nelts %d\n",__func__, nelts);
    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    memset(&act, 0, sizeof(act));
    act.arg = cscf;
    act.alloc = rtmp_alloc_amf_buf;
//    act.log = s->connection->log;

    if (first) {
        act.first = *first;
    }

    if (last) {
        act.link = *last;
    }

    rc = ngx_rtmp_amf_write(&act, elts, nelts);//ngx_rtmp_amf_write_array(&act, elts, nelts);

    if (first) {
        *first = act.first;
    }

    if (last) {
        *last = act.link;
    }
	

    return rc;
}

 ngx_int_t
ngx_rtmp_amf_message_handler(ngx_rtmp_session_t *s,
        ngx_rtmp_header_t *h, ngx_chain_t *in)
{
    ngx_rtmp_amf_ctx_t          act;
    ngx_rtmp_core_main_conf_t  *cmcf;
    ngx_array_t                *ch;
//    ngx_rtmp_handler_pt        *ph;
    size_t                      len, n;

    u_char               func[128];

    ngx_rtmp_amf_elt_t   elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          func,   sizeof(func) },
    };

    /* AMF command names come with string type, but shared object names
     * come without type */
    if (h->type == NGX_RTMP_MSG_AMF_SHARED ||
        h->type == NGX_RTMP_MSG_AMF3_SHARED)
    {
        elts[0].type |= NGX_RTMP_AMF_TYPELESS;
    } else {
        elts[0].type &= ~NGX_RTMP_AMF_TYPELESS;
    }

    if ((h->type == NGX_RTMP_MSG_AMF3_SHARED ||
         h->type == NGX_RTMP_MSG_AMF3_META ||
         h->type == NGX_RTMP_MSG_AMF3_CMD)
         && in->buf->last > in->buf->pos)
    {
       
        ++in->buf->pos;
    }

 //   cmcf = ngx_rtmp_get_module_main_conf(s, ngx_rtmp_core_module);

    /* read AMF func name & transaction id */
    ngx_memzero(&act, sizeof(act));
    act.link = in;
//    act.log = s->connection->log;
    memset(func, 0, sizeof(func));

    if (ngx_rtmp_amf_read(&act, elts,
                sizeof(elts) / sizeof(elts[0])) != NGX_OK)
    {
        printf("AMF cmd failed\n");
        return NGX_ERROR;
    }

    /* skip name */
    in = act.link;
    in->buf->pos += act.offset;

    len = ngx_strlen(func);
	
	printf("AMF func '%s' passed to handler %d\n",
				   func, n);

/* 
	ch = ngx_hash_find(&cmcf->amf_hash,
            ngx_hash_strlow(func, func, len), func, len);

    if (ch && ch->nelts) {
        ph = ch->elts;
        for (n = 0; n < ch->nelts; ++n, ++ph) {
            ngx_log_debug3(NGX_LOG_DEBUG_RTMP, s->connection->log, 0,
                "AMF func '%s' passed to handler %d/%d",
                func, n, ch->nelts);
            switch ((*ph)(s, h, in)) {
                case NGX_ERROR:
                    return NGX_ERROR;
                case NGX_DONE:
                    return NGX_OK;
            }
        }
    } else {
        ngx_log_debug1(NGX_LOG_DEBUG_RTMP, s->connection->log, 0,
            "AMF cmd '%s' no handler", func);
    }
*/
	if(strcmp("connect",func)==0)
	{
		if(ngx_rtmp_cmd_connect_init(s, h, in) != RTMP_OK)
		{
			printf("ngx_rtmp_cmd_connect_init:	error\n");
			return NGX_ERROR;
		}
	}
	else if(strcmp("createStream",func)==0)
	{
		if(ngx_rtmp_cmd_create_stream_init(s, h, in) != RTMP_OK )
		{
			printf("ngx_rtmp_cmd_create_stream_init:	error\n");
			return NGX_ERROR;
		}
		
	}
	else if(strcmp("play",func)==0)
	{
		in->buf->last = in->buf->pos + h->mlen -7;
		if(ngx_rtmp_cmd_play_init(s, h, in) != RTMP_OK )
		{
			printf("ngx_rtmp_cmd_play_init:	error\n");
			return NGX_ERROR;
		}
	}
	else if(strcmp("onBWDone",func)==0)
	{	
		if(s->publish_state == 0)
		{
			s->publish_state = 1;
		} 
			ngx_rtmp_send_release_stream(s);
			ngx_rtmp_send_fcpublish(s);
			ngx_rtmp_send_create_stream(s);
			ngx_rtmp_send_create_checkbw(s);
	}
	else if(strcmp("_result",func)==0)
	{
#ifdef TEST_SERVER1
		
		if(s->publish_state == 0)
		{
			s->publish_state = 1;
			ngx_rtmp_send_release_stream(s);
			ngx_rtmp_send_fcpublish(s);
			ngx_rtmp_send_create_stream(s);
		}
		else if(s->publish_state == 1)
		{
			s->publish_state = 0;
			ngx_rtmp_send_publish(s);	
		}

#else
#ifdef TEST_SERVER2
		
		if(s->publish_state == 0)
		{
			s->publish_state = 1;
			ngx_rtmp_send_release_stream(s);
			ngx_rtmp_send_fcpublish(s);
			ngx_rtmp_send_create_stream(s);
		}
		else if(s->publish_state == 1)
		{
			s->publish_state = 0;
			ngx_rtmp_send_publish(s);	
		}

#else
#ifdef TEST_SERVER3
		if(s->publish_state == 0)
		{
			s->publish_state = 1;
			ngx_rtmp_send_release_stream(s);
			ngx_rtmp_send_fcpublish(s);
			ngx_rtmp_send_create_stream(s);
			ngx_rtmp_send_create_checkbw(s);
		}
		else if(s->publish_state == 1)
		{
			s->publish_state = 2;
			ngx_rtmp_send_publish(s);	
		}
#else
#ifdef TEST_SERVER5
		if(s->publish_state == 0)
		{
			s->publish_state = 1;
			ngx_rtmp_send_release_stream(s);
			ngx_rtmp_send_fcpublish(s);
			ngx_rtmp_send_create_stream(s);
			ngx_rtmp_send_create_checkbw(s);
		}
		else if(s->publish_state == 1)
		{
			s->publish_state = 2;
			ngx_rtmp_send_publish(s);	
		}

#else
		if(s->publish_state == 1)
		{		
			s->publish_state = 2;
			ngx_rtmp_send_publish(s);	
		}
		else if(s->publish_state == 2)
		{
			s->publish_state = 0;
			s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;
			ngx_rtmp_send_setdataFrame(s);
		}
#endif		
#endif
#endif		
#endif
	}
	else if(strcmp("onStatus",func)==0)
	{
#ifdef TEST_SERVER1
		//ngx_rtmp_send_setdataFrame(s);
		if(s->publish_state == 2)
		{
			s->publish_state = 0;
			s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;		
			
			//ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
			//cscf->chunk_size = 1460;

			ngx_rtmp_send_setdataFrame(s);
		}

#else
#ifdef TEST_SERVER2
		//ngx_rtmp_send_setdataFrame(s);
		printf("state %d \r\n",s->publish_state);
		if(s->publish_state == 0)
		{
			s->publish_state = 0;
			s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;		
			
			//ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
			//cscf->chunk_size = 1460;

			ngx_rtmp_send_setdataFrame(s);
		}

#else
#ifdef TEST_SERVER3
		if(s->publish_state == 2)
		{
			s->publish_state = 0;
			s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;		
			
			//ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
			//cscf->chunk_size = 1460;

			ngx_rtmp_send_setdataFrame(s);
		}

#else
#ifdef TEST_SERVER5
				if(s->publish_state == 2)
				{
					s->publish_state = 0;
					s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;		
					
					//ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
					//cscf->chunk_size = 1460;
		
					ngx_rtmp_send_setdataFrame(s);
				}
#else
		if(s->publish_state == 1)
		{
			s->publish_state = 2;
			ngx_rtmp_send_publish(s);	
		}
		else if(s->publish_state == 2)
		{
			s->publish_state = 0;
			s->hs_stage = NGX_RTMP_PUSH_CLIENT_DONE;			
			ngx_rtmp_send_setdataFrame(s);
		}
#endif		
#endif
#endif	
#endif
	}
    return NGX_OK;
}

#endif
