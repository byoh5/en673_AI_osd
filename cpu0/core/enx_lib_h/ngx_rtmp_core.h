#include "board.h"
#include "ngx_rtmp_config.h"
#include "ngx_rtmp_alloc.h"

#ifdef RTMP_SUPPORT
 
#ifndef _NGX_CORE_H_INCLUDED_
#define _NGX_CORE_H_INCLUDED_

#define NGX_CONF_BUFFER  4096
 
#define RTMP_PACKETSIZE (1538)
 
 
#define LF     (u_char) '\n'
#define CR     (u_char) '\r'
#define CRLF   "\r\n"
 
#  define EVP_MD_FLAG_PKEY_METHOD_SIGNATURE       0x0004
 
#  define EVP_MD_FLAG_DIGALGID_ABSENT             0x0008
 
#define NGX_RTMP_DEFAULT_CHUNK_SIZE     128
 
#define NGX_RTMP_MAX_CHUNK_HEADER       18
#define NGX_RTMP_REFCOUNT_TYPE              uint32
#define NGX_RTMP_REFCOUNT_BYTES             sizeof(NGX_RTMP_REFCOUNT_TYPE)

/*
 *        AAAA  number of arguments
 *      FF      command flags
 *    TT        command type, i.e. HTTP "location" or "server" command
 */

#define NGX_CONF_NOARGS      0x00000001
#define NGX_CONF_TAKE1       0x00000002
#define NGX_CONF_TAKE2       0x00000004
#define NGX_CONF_TAKE3       0x00000008
#define NGX_CONF_TAKE4       0x00000010
#define NGX_CONF_TAKE5       0x00000020
#define NGX_CONF_TAKE6       0x00000040
#define NGX_CONF_TAKE7       0x00000080

#define NGX_CONF_MAX_ARGS    8

#define NGX_CONF_TAKE12      (NGX_CONF_TAKE1|NGX_CONF_TAKE2)
#define NGX_CONF_TAKE13      (NGX_CONF_TAKE1|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE23      (NGX_CONF_TAKE2|NGX_CONF_TAKE3)

#define NGX_CONF_TAKE123     (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3)
#define NGX_CONF_TAKE1234    (NGX_CONF_TAKE1|NGX_CONF_TAKE2|NGX_CONF_TAKE3   \
                              |NGX_CONF_TAKE4)

#define NGX_CONF_ARGS_NUMBER 0x000000ff
#define NGX_CONF_BLOCK       0x00000100
#define NGX_CONF_FLAG        0x00000200
#define NGX_CONF_ANY         0x00000400
#define NGX_CONF_1MORE       0x00000800
#define NGX_CONF_2MORE       0x00001000
#define NGX_CONF_MULTI       0x00000000  /* compatibility */

#define NGX_RTMP_MODULE                 0x504D5452     /* "RTMP" */

#define NGX_RTMP_MAIN_CONF              0x02000000
#define NGX_RTMP_SRV_CONF               0x04000000
#define NGX_RTMP_APP_CONF               0x08000000
#define NGX_RTMP_REC_CONF               0x10000000

#define NGX_DIRECT_CONF      0x00010000

#define NGX_MAIN_CONF        0x01000000
#define NGX_ANY_CONF         0x0F000000

#define NGX_RTMP_LIMIT_SOFT         0
#define NGX_RTMP_LIMIT_HARD         1
#define NGX_RTMP_LIMIT_DYNAMIC      2

/* RMTP control message types */
#define NGX_RTMP_USER_STREAM_BEGIN      0
#define NGX_RTMP_USER_STREAM_EOF        1
#define NGX_RTMP_USER_STREAM_DRY        2
#define NGX_RTMP_USER_SET_BUFLEN        3
#define NGX_RTMP_USER_RECORDED          4
#define NGX_RTMP_USER_PING_REQUEST      6
#define NGX_RTMP_USER_PING_RESPONSE     7
#define NGX_RTMP_USER_UNKNOWN           8
#define NGX_RTMP_USER_BUFFER_END        31



#define NGX_CONF_UNSET       -1
#define NGX_CONF_UNSET_UINT  (u_int) -1
#define NGX_CONF_UNSET_PTR   (void *) -1
#define NGX_CONF_UNSET_SIZE  (size_t) -1
#define NGX_CONF_UNSET_MSEC  (ngx_msec_t) -1


#define NGX_CONF_OK          NULL
#define NGX_CONF_ERROR       (void *) -1

#define NGX_CONF_BLOCK_START 1
#define NGX_CONF_BLOCK_DONE  2
#define NGX_CONF_FILE_DONE   3

#define NGX_CORE_MODULE      0x45524F43  /* "CORE" */
#define NGX_CONF_MODULE      0x464E4F43  /* "CONF" */


#define NGX_MAX_CONF_ERRSTR  1024
 
#define rtmp_abs(value)       (((value) >= 0) ? (value) : - (value))
#define rtmp_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define rtmp_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))
 
#define RTMP_LIMIT_SOFT         0
#define RTMP_LIMIT_HARD         1
#define RTMP_LIMIT_DYNAMIC      2
 
#define NGX_RTMP_MSID                   1
	 
#define NGX_RTMP_CSID_AMF_INI           3
#define NGX_RTMP_CSID_AMF               5
#define NGX_RTMP_CSID_AUDIO             6
#define NGX_RTMP_CSID_VIDEO             7
 

#define ngx_rtmp_ref(b)                     \
	 *((NGX_RTMP_REFCOUNT_TYPE*)(b) - 1)
	 
#define ngx_rtmp_ref_set(b, v)              \
		 ngx_rtmp_ref(b) = v
	 
#define ngx_rtmp_ref_get(b)                 \
		 ++ngx_rtmp_ref(b)
 
#define ngx_rtmp_ref_put(b)                 \
		 --ngx_rtmp_ref(b)
#define ngx_rtmp_acquire_shared_chain(in)   \
		 ngx_rtmp_ref_get(in);					 \
		 
 #ifndef NGX_CYCLE_POOL_SIZE
#define NGX_CYCLE_POOL_SIZE     NGX_DEFAULT_POOL_SIZE
#endif
 
 
#define NGX_RTMP_MAIN_CONF_OFFSET  offsetof(ngx_rtmp_conf_ctx_t, main_conf)
#define NGX_RTMP_SRV_CONF_OFFSET   offsetof(ngx_rtmp_conf_ctx_t, srv_conf)
#define NGX_RTMP_APP_CONF_OFFSET   offsetof(ngx_rtmp_conf_ctx_t, app_conf)
 
#define rtmp_string(str)     { sizeof(str) - 1, (u_char *) str }
#define rtmp_null_string     { 0, NULL }
#define rtmp_str_set(str, text)                                               \
	 (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define rtmp_str_null(str)   (str)->len = 0; (str)->data = NULL
 
#define rtmp_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define rtmp_strlen(s)       strlen((const char *) s)
 
#define rtmp_strchr(s1, c)   strchr((const char *) s1, (int) c)
#define rtmp_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)
 
 
#define rtmp_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define rtmp_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
 
#define rtmp_memzero(buf, n)       (void) memset(buf, 0, n)
#define rtmp_memset(buf, c, n)     (void) memset(buf, c, n)
#define rtmp_memcmp(s1, s2, n)  memcmp((const char *) s1, (const char *) s2, n)
 
#define ngx_string(str)     { sizeof(str) - 1, (u_char *) str }
#define ngx_null_string     { 0, NULL }
#define ngx_str_set(str, text)                                               \
	 (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
#define ngx_str_null(str)   (str)->len = 0; (str)->data = NULL
 
 
#define ngx_tolower(c)      (u_char) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c)      (u_char) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
 
 
#define ngx_memcpy(dst, src, n)   (void) memcpy(dst, src, n)
#define ngx_cpymem(dst, src, n)   (((u_char *) memcpy(dst, src, n)) + (n))
 
#define ngx_strncmp(s1, s2, n)  strncmp((const char *) s1, (const char *) s2, n)
 
 
 /* msvc and icc7 compile strcmp() to inline loop */
#define ngx_strcmp(s1, s2)  strcmp((const char *) s1, (const char *) s2)
 
 
#define ngx_strstr(s1, s2)  strstr((const char *) s1, (const char *) s2)
#define ngx_strlen(s)       strlen((const char *) s)
 
#define ngx_strchr(s1, c)   strchr((const char *) s1, (int) c)
 /*
  * msvc and icc7 compile memset() to the inline "rep stos"
  * while ZeroMemory() and bzero() are the calls.
  * icc7 may also inline several mov's of a zeroed register for small blocks.
  */
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#define ngx_memset(buf, c, n)     (void) memset(buf, c, n)

 
 
#define ngx_memmove(dst, src, n)   (void) memmove(dst, src, n)
#define ngx_movemem(dst, src, n)   (((u_char *) memmove(dst, src, n)) + (n))
#define ngx_abs(value)       (((value) >= 0) ? (value) : - (value))
#define ngx_max(val1, val2)  ((val1 < val2) ? (val2) : (val1))
#define ngx_min(val1, val2)  ((val1 > val2) ? (val2) : (val1))
 //#define ngx_cpymem(dst, src, n)	 (((u_char *) memcpy(dst, src, n)) + (n))
 
#define ngx_rtmp_get_module_main_conf(s, module)                             \
	 (s)->main_conf[module.ctx_index]
 
#define ngx_rtmp_get_module_srv_conf(s, module)  (s)->srv_conf[module.ctx_index]
#define ngx_rtmp_get_module_app_conf(s, module)  ((s)->app_conf ? \
	 (s)->app_conf[module.ctx_index] : NULL)
#define ngx_rtmp_conf_get_module_main_conf(cf, module)                       \
		 ((ngx_rtmp_conf_ctx_t *) cf->ctx)->main_conf[module.ctx_index]
#define ngx_rtmp_conf_get_module_srv_conf(cf, module)                        \
		 ((ngx_rtmp_conf_ctx_t *) cf->ctx)->srv_conf[module.ctx_index]
#define ngx_rtmp_conf_get_module_app_conf(cf, module)                        \
		 ((ngx_rtmp_conf_ctx_t *) cf->ctx)->app_conf[module.ctx_index]
	 
	 
#ifndef offsetof
#  define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif
 
extern ngx_module_t  ngx_rtmp_core_module;
extern ngx_module_t  ngx_rtmp_live_module;
extern ngx_uint_t 		 ngx_max_module;
extern ngx_uint_t  ngx_rtmp_max_module;
extern ngx_module_t *ngx_modules[];

 char *
ngx_rtmp_block(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);
 void *
ngx_rtmp_core_create_main_conf(ngx_conf_t *cf);

 void *
ngx_rtmp_core_create_srv_conf(ngx_conf_t *cf);

 char *ngx_rtmp_core_merge_srv_conf(ngx_conf_t *cf, void *parent,
    void *child);

 void *
ngx_rtmp_core_create_app_conf(ngx_conf_t *cf);
 char *
ngx_rtmp_core_merge_app_conf(ngx_conf_t *cf, void *parent, void *child);
 char *ngx_rtmp_core_server(ngx_conf_t *cf, ngx_command_t *cmd,
    void *conf);

char *
ngx_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

 
ngx_int_t 
ngx_conf_handler(ngx_conf_t *cf);

char *
ngx_rtmp_merge_applications(ngx_conf_t *cf, ngx_array_t *applications,
		void **app_conf, ngx_rtmp_module_t *module, ngx_uint_t ctx_index);

char *
ngx_rtmp_core_application(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

ngx_int_t
ngx_rtmp_live_postconfiguration(ngx_conf_t *cf);

void *
ngx_rtmp_live_create_app_conf(ngx_conf_t *cf);

char *
ngx_rtmp_live_merge_app_conf(ngx_conf_t *cf, void *parent, void *child);


 ngx_chain_t *
ngx_rtmp_alloc_in_buf(ngx_rtmp_session_t *s);

 ngx_int_t
ngx_rtmp_set_chunk_size(ngx_rtmp_session_t *s, ngx_uint_t size);
 
 ngx_rtmp_chain_t *
 ngx_rtmp_alloc_shared_buf(ngx_rtmp_core_srv_conf_t *cscf);




#define NGX_RTMP_USER_START(s, tp)                                          \
    ngx_rtmp_header_t               __h;                                    \
    ngx_rtmp_chain_t                    *__l;                                    \
    ngx_rtmp_buf_t                      *__b;                                    \
    ngx_rtmp_core_srv_conf_t       *__cscf;                                 \
    																		\
	__cscf = s->srv_conf[ngx_rtmp_core_module.ctx_index];				\
    memset(&__h, 0, sizeof(__h));                                           \
    __h.type = tp;                                                          \
    __h.csid = 2;                                                     \
    __l = ngx_rtmp_alloc_shared_buf(__cscf);                               \
    if (__l == NULL) {                                                      \
        return NULL;                                                        \
    }                                                                      \
    __b = __l->buf;

#define NGX_RTMP_UCTL_START(s, type, utype)                                 \
    NGX_RTMP_USER_START(s, type);                                           \
    *(__b->last++) = (u_char)((utype) >> 8);                                \
    *(__b->last++) = (u_char)(utype);

#define NGX_RTMP_USER_OUT1(v)                                               \
    *(__b->last++) = ((u_char*)&v)[0];

#define NGX_RTMP_USER_OUT4(v)                                               \
    *(__b->last++) = ((u_char*)&v)[0];                                      \
    *(__b->last++) = ((u_char*)&v)[1];                                      \
    *(__b->last++) = ((u_char*)&v)[2];                                      \
    *(__b->last++) = ((u_char*)&v)[3];

#define NGX_RTMP_USER_END(s)                                                \
    ngx_rtmp_prepare_message(s, &__h, NULL, __l);                           \
    return __l;

#define MODULE_V1          0, 0, 0, 0, 0, 0, 1
#define MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0


	  
u_char *
rtmp_cpystrn(u_char *dst, u_char *src, size_t n);

void
ngx_rtmp_cmd_fill_args(u_char name[RTMP_MAX_NAME],
	  u_char args[RTMP_MAX_ARGS]);

u_char *
rtmp_strlchr(u_char *p, u_char *last, u_char c);

void
ngx_rtmp_send(void *ts);


ngx_int_t
ngx_rtmp_send_message(ngx_rtmp_session_t *s, ngx_chain_t *out,
	 u_int priority);

int
ngx_rtmp_send_shared_packet(ngx_rtmp_session_t *s, ngx_rtmp_chain_t *cl);

void *
ngx_rtmp_core_create_main_conf(ngx_conf_t *cf);

void *
ngx_rtmp_core_create_srv_conf(ngx_conf_t *cf);;

char *
ngx_rtmp_core_merge_srv_conf(ngx_conf_t *cf, void *parent, void *child);


void *
ngx_rtmp_core_create_app_conf(ngx_conf_t *cf);


char *
ngx_rtmp_core_merge_app_conf(ngx_conf_t *cf, void *parent, void *child);

char *
ngx_rtmp_core_server(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);

 void
ngx_rtmp_recv(ngx_rtmp_session_t         *s);

void
ngx_rtmp_cycle(ngx_rtmp_session_t *s);

#endif
#endif
