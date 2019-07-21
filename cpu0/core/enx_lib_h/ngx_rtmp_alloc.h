#include "stdlib.h"
#include "board.h"
#include "ngx_rtmp_config.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_ALLOC_H_INCLUDED_
#define _NGX_ALLOC_H_INCLUDED_



typedef void (*ngx_pool_cleanup_pt)(void *data);

typedef struct ngx_pool_cleanup_s  ngx_pool_cleanup_t;

struct ngx_pool_cleanup_s {
    ngx_pool_cleanup_pt   handler;
    void                 *data;
    ngx_pool_cleanup_t   *next;
};


typedef struct ngx_pool_large_s  ngx_pool_large_t;

struct ngx_pool_large_s {
    ngx_pool_large_t     *next;
    void                 *alloc;
};


typedef struct {
    u_char               *last;
    u_char               *end;
    ngx_pool_t           *next;
    ngx_uint_t            failed;
} ngx_pool_data_t;


struct ngx_pool_s {
    ngx_pool_data_t       d;
    size_t                max;
    ngx_pool_t           *current;
    ngx_chain_t          *chain;
    ngx_pool_large_t     *large;
    ngx_pool_cleanup_t   *cleanup;
};


typedef struct {
    ngx_fd_t              fd;
    u_char               *name;
} ngx_pool_cleanup_file_t;


extern ngx_uint_t  ngx_pagesize;
extern ngx_uint_t  ngx_pagesize_shift;
extern ngx_uint_t  ngx_cacheline_size;

ngx_uint_t	ngx_pagesize;
ngx_uint_t	ngx_pagesize_shift;
ngx_uint_t	ngx_cacheline_size;

#define NGX_MAX_ALLOC_FROM_POOL  (ngx_pagesize - 1)

#define NGX_DEFAULT_POOL_SIZE    (16 * 1024)

#define NGX_POOL_ALIGNMENT       (16)
#define NGX_MIN_POOL_SIZE                                                     \
    ngx_align((sizeof(ngx_pool_t) + 2 * sizeof(ngx_pool_large_t)),            \
              NGX_POOL_ALIGNMENT)

void *ngx_alloc(size_t size);
void *ngx_calloc(size_t size);

void ngx_destroy_pool(ngx_pool_t *pool);

void ngx_reset_pool(ngx_pool_t *pool);

//void *ngx_palloc(ngx_pool_t *pool, size_t size);
void *ngx_pnalloc(ngx_pool_t *pool, size_t size);
void *ngx_pcalloc(ngx_pool_t *pool, size_t size);
void *ngx_pmemalign(ngx_pool_t *pool, size_t size, size_t alignment);
ngx_int_t ngx_pfree(ngx_pool_t *pool, void *p);

ngx_pool_t *ngx_create_pool(size_t size);

#define ngx_calloc_buf(pool) ngx_pcalloc(pool, sizeof(ngx_buf_t))


//ngx_pool_cleanup_t *ngx_pool_cleanup_add(ngx_pool_t *p, size_t size);
//void ngx_pool_run_cleanup_file(ngx_pool_t *p, ngx_fd_t fd);
//void ngx_pool_cleanup_file(void *data);
//void ngx_pool_delete_file(void *data);



 void *ngx_palloc_block(ngx_pool_t *pool, size_t size);
 void *ngx_palloc_large(ngx_pool_t *pool, size_t size);
 
 void *
 rtmp_memcpy(void *dst, const void *src, size_t n);
 
 void *
 ngx_rtmp_rmemcpy(void *dst, const void* src, size_t n);
 
 //void * ngx_rtmp_rmemcpy(void *dst, const void* src, size_t n);
#define ngx_rtmp_rcpymem(dst, src, n) \
	 (((u_char*)ngx_rtmp_rmemcpy(dst, src, n)) + (n))
	 
 //void *ngx_memcpy(void *dst, const void *src, size_t n);
 //#define rtmp_cpymem(dst, src, n)   (((u_char *) rtmp_memcpy(dst, src, n)) + (n))
 
#define ngx_free_chain(pool, cl)                                             \
    cl->next = pool->chain;                                                  \
    pool->chain = cl

ngx_chain_t *
ngx_alloc_chain_link(ngx_pool_t *pool);

#define ngx_free          free

#define NGX_HAVE_MEMALIGN (1)

#if (NGX_HAVE_POSIX_MEMALIGN || NGX_HAVE_MEMALIGN)

void *ngx_memalign(size_t alignment, size_t size);

#else

#define ngx_memalign(alignment, size)  ngx_alloc(size)

#endif

#endif
#endif
