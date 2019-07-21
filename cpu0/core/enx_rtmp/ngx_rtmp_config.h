#include "board.h"
#include "dev.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "vsprintf.h"

#include "msg.h"
//#include "isp.h"
#include "enx_video.h"
#include "enx_os.h"

#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/tcp_impl.h"

//#include "rtsp_control.h"
#include "dev_types.h"
#include <lwipopts.h> 
#include "stream_buffering.h"

//#include "timer.h"

#ifdef RTMP_SUPPORT

#ifndef _NGX_CONFIG_H_INCLUDED_
#define _NGX_CONFIG_H_INCLUDED_

enum {
    RTSP_H264 = 2,
    RTSP_MJPEG = 3,
    RTSP_H264SUB = 4
};


#ifndef RTMP_DEF_PORT
#define RTMP_DEF_PORT 1935
#endif
#ifndef MAX_RTMP_NUM
#define MAX_RTMP_NUM 1
#endif
#define RTMP_OUT_CNT (184)//n%8
#define RTMP_CHUNK_SIZE (5840)//(1446)//(4096)//(5840)
#define RTMP_MAX_MESSAGE_SIZE (1024*1024)//(1*1024)
#define RTMP_MAX_STREAM (32)
#define RTMP_MAX_WINDOW (50000)//(1000000)//

#define  RTMP_OK          0
#define  RTMP_ERROR      -1
#define  RTMP_AGAIN      -2
#define  RTMP_BUSY       -3
#define  RTMP_DONE       -4
#define  RTMP_DECLINED   -5
#define  RTMP_ABORT      -6

#define  NGX_OK          0
#define  NGX_ERROR      -1
#define  NGX_AGAIN      -2
#define  NGX_BUSY       -3
#define  NGX_DONE       -4
#define  NGX_DECLINED   -5
#define  NGX_ABORT      -6
	

#define NGX_RTMP_FMS_VERSION        "FMS/3,0,1,123"
#define NGX_RTMP_CAPABILITIES       31

#define NGX_RTMP_MSID                   1

#define NGX_RTMP_CSID_AMF_INI           3
#define NGX_RTMP_CSID_AMF_INI2           4
#define NGX_RTMP_CSID_AMF               5
#define NGX_RTMP_CSID_AUDIO             6
#define NGX_RTMP_CSID_VIDEO             7

#define NGX_RTMP_MAX_NAME           256
#define NGX_RTMP_MAX_URL            256
#define NGX_RTMP_MAX_ARGS           NGX_RTMP_MAX_NAME


#define H264_TX_READY       0
#define H264_TX_DOING       1
#define H264_TX_END         2

enum {
    /* Uncompressed codec id is actually 0,
     * but we use another value for consistency */
    NGX_RTMP_AUDIO_UNCOMPRESSED     = 16,
    NGX_RTMP_AUDIO_ADPCM            = 1,
    NGX_RTMP_AUDIO_MP3              = 2,
    NGX_RTMP_AUDIO_LINEAR_LE        = 3,
    NGX_RTMP_AUDIO_NELLY16          = 4,
    NGX_RTMP_AUDIO_NELLY8           = 5,
    NGX_RTMP_AUDIO_NELLY            = 6,
    NGX_RTMP_AUDIO_G711A            = 7,
    NGX_RTMP_AUDIO_G711U            = 8,
    NGX_RTMP_AUDIO_AAC              = 10,
    NGX_RTMP_AUDIO_SPEEX            = 11,
    NGX_RTMP_AUDIO_MP3_8            = 14,
    NGX_RTMP_AUDIO_DEVSPEC          = 15,
};

/* Video codecs */
enum {
    NGX_RTMP_VIDEO_JPEG             = 1,
    NGX_RTMP_VIDEO_SORENSON_H263    = 2,
    NGX_RTMP_VIDEO_SCREEN           = 3,
    NGX_RTMP_VIDEO_ON2_VP6          = 4,
    NGX_RTMP_VIDEO_ON2_VP6_ALPHA    = 5,
    NGX_RTMP_VIDEO_SCREEN2          = 6,
    NGX_RTMP_VIDEO_H264             = 7
};

typedef struct ngx_pool_s        ngx_pool_t;
typedef u_int        rtmp_uint_t;
typedef int        rtmp_int_t;
typedef int        ngx_int_t;
typedef u_int 		ngx_uint_t;
typedef struct ngx_conf_s        ngx_conf_t;


typedef void *			  rtmp_buf_tag_t;

typedef struct ngx_rtmp_buf_s  ngx_buf_t;
typedef struct ngx_rtmp_buf_s  ngx_rtmp_buf_t;
typedef struct ngx_rtmp_chain_s 	  ngx_rtmp_chain_t;
typedef ngx_rtmp_chain_t ngx_chain_t;
typedef ngx_rtmp_chain_t * (*ngx_rtmp_amf_alloc_pt)(void *arg);
typedef struct rtmp_pool_s		  rtmp_pool_t;
typedef struct rtmp_pool_large_s  rtmp_pool_large_t;
typedef struct rtmp_pool_cleanup_s	rtmp_pool_cleanup_t;
typedef void (*rtmp_pool_cleanup_pt)(void *data);
typedef u_int  ngx_rbtree_key_t;
typedef ngx_rbtree_key_t	  ngx_msec_t;
typedef int 	   rtmp_flag_t;
typedef struct ngx_module_s 	 ngx_module_t;
typedef struct ngx_command_s	 ngx_command_t;
typedef struct ngx_conf_s		 rtmp_conf_t;
typedef struct ngx_connection_s  ngx_connection_t;

typedef struct {
    size_t      len;
    u_char     *data;
} ngx_str_t;


struct ngx_command_s {
    ngx_str_t             name;
    rtmp_uint_t            type;
    char               *(*set)(rtmp_conf_t *cf, ngx_command_t *cmd, void *conf);
    rtmp_uint_t            conf;
    rtmp_uint_t            offset;
    void                 *post;
};

struct ngx_module_s {
    rtmp_uint_t            ctx_index;
    rtmp_uint_t            index;

    rtmp_uint_t            spare0;
    rtmp_uint_t            spare1;
    rtmp_uint_t            spare2;
    rtmp_uint_t            spare3;

    rtmp_uint_t            version;

    void                 *ctx;
    ngx_command_t        *commands;
    rtmp_uint_t            type;


};

typedef struct {
    ngx_str_t             name;
    void               *(*create_conf)(void/*ngx_cycle_t*/ *cycle);
    char               *(*init_conf)(void/*ngx_cycle_t*/ *cycle, void *conf);
} ngx_core_module_t;

typedef struct {
    ngx_int_t             (*preconfiguration)(ngx_conf_t *cf);
    ngx_int_t             (*postconfiguration)(ngx_conf_t *cf);

    void                 *(*create_main_conf)(ngx_conf_t *cf);
    char                 *(*init_main_conf)(ngx_conf_t *cf, void *conf);

    void                 *(*create_srv_conf)(ngx_conf_t *cf);
    char                 *(*merge_srv_conf)(ngx_conf_t *cf, void *prev,
                                    void *conf);

    void                 *(*create_app_conf)(ngx_conf_t *cf);
    char                 *(*merge_app_conf)(ngx_conf_t *cf, void *prev,
                                    void *conf);
} ngx_rtmp_module_t;



struct rtmp_pool_cleanup_s {
    rtmp_pool_cleanup_pt   handler;
    void                 *data;
    rtmp_pool_cleanup_t   *next;
};

struct rtmp_pool_large_s {
    rtmp_pool_large_t     *next;
    void                 *alloc;
};

typedef struct {
    u_char               *last;
    u_char               *end;
    rtmp_pool_t           *next;
    u_int            failed;
} rtmp_pool_data_t;

struct rtmp_pool_s {
    rtmp_pool_data_t       d;
    size_t                max;
    rtmp_pool_t           *current;
    ngx_rtmp_chain_t          *chain;
    rtmp_pool_large_t     *large;
    rtmp_pool_cleanup_t   *cleanup;    
};

struct ngx_rtmp_buf_s {
    u_char          *pos;
    u_char          *last;
    off_t            file_pos;
    off_t            file_last;

    u_char          *start;         /* start of buffer */
    u_char          *end;           /* end of buffer */
    rtmp_buf_tag_t    tag;
    //ngx_file_t      *file;
    ngx_rtmp_buf_t       *shadow;


    /* the buf's content could be changed */
    unsigned         temporary:1;

    /*
     * the buf's content is in a memory cache or in a read only memory
     * and must not be changed
     */
    unsigned         memory:1;

    /* the buf's content is mmap()ed and must not be changed */
    unsigned         mmap:1;

    unsigned         recycled:1;
    unsigned         in_file:1;
    unsigned         flush:1;
    unsigned         sync:1;
    unsigned         last_buf:1;
    unsigned         last_in_chain:1;

    unsigned         last_shadow:1;
    unsigned         temp_file:1;

    /* STUB */ int   num;
};

typedef struct {
    void        *elts;
    ngx_uint_t   nelts;
    size_t       size;
    ngx_uint_t   nalloc;
    ngx_pool_t  *pool;
} ngx_array_t;

struct ngx_conf_s {
    char                 *name;
    ngx_array_t          *args;

//    rtmp_cycle_t          *cycle;
    rtmp_pool_t           *pool;
//    rtmp_pool_t           *temp_pool;
//    rtmp_conf_file_t      *conf_file;

    void                 *ctx;
    rtmp_uint_t            module_type;
    rtmp_uint_t            cmd_type;

//    rtmp_conf_handler_pt   handler;
    char                 *handler_conf;
};
 

typedef struct {
    ngx_array_t             applications; /* ngx_rtmp_core_app_conf_t */
    ngx_str_t               name;
    void                  **app_conf;
} rtmp_core_app_conf_t;

typedef struct {
    uint32                csid;       /* chunk stream id */
    uint32                timestamp;  /* timestamp (delta) */
    uint32                mlen;       /* message length */
    uint8                 type;       /* message type id */
    uint32                msid;       /* message stream id */
} ngx_rtmp_header_t;

struct ngx_rtmp_chain_s {
    ngx_rtmp_buf_t    *buf;
    ngx_rtmp_chain_t  *next;
};


typedef struct {
    void                  **main_conf;
    void                  **srv_conf;
    void                  **app_conf;
} ngx_rtmp_conf_ctx_t;

typedef struct {
    ngx_array_t             applications; /* ngx_rtmp_core_app_conf_t */
    ngx_str_t               name;
    void                  **app_conf;
} ngx_rtmp_core_app_conf_t;

typedef struct {
    ngx_array_t             servers;    /* ngx_rtmp_core_srv_conf_t */
    ngx_array_t             listen;     /* ngx_rtmp_listen_t */

//    ngx_array_t             events[NGX_RTMP_MAX_EVENT];

//  ngx_hash_t              amf_hash;
    ngx_array_t             amf_arrays;
    ngx_array_t             amf;
} ngx_rtmp_core_main_conf_t;

typedef struct rtmp_core_srv_conf_s {
    ngx_array_t             applications; /* ngx_rtmp_core_app_conf_t */

    ngx_msec_t              timeout;
    ngx_msec_t              ping;
    ngx_msec_t              ping_timeout;
    rtmp_flag_t              so_keepalive;
    int               max_streams;

    u_int              ack_window;

    int               chunk_size;
    rtmp_pool_t             *pool;
    ngx_rtmp_chain_t            *free;
    ngx_rtmp_chain_t            *free_hs;
    size_t                  max_message;
    rtmp_flag_t              play_time_fix;
    rtmp_flag_t              publish_time_fix;
    rtmp_flag_t              busy;
    size_t                  out_queue;
    size_t                  out_cork;
    ngx_msec_t              buflen;

    ngx_rtmp_conf_ctx_t    *ctx;
} ngx_rtmp_core_srv_conf_t;

#define RTMP_MAX_NAME           256
#define RTMP_MAX_URL            256
#define RTMP_MAX_ARGS           RTMP_MAX_NAME

typedef struct {
    double                          trans;
    u_char                          app[RTMP_MAX_NAME];
    u_char                          args[RTMP_MAX_ARGS];
    u_char                          flashver[32];
    u_char                          swf_url[RTMP_MAX_URL];
    u_char                          tc_url[RTMP_MAX_URL];
    double                          acodecs;
    double                          vcodecs;
    u_char                          page_url[RTMP_MAX_URL];
    double                          object_encoding;
} rtmp_connect_t;

typedef struct {
    ngx_rtmp_header_t       hdr;
    uint32                dtime;
    uint32                len;        /* current fragment length */
    uint8                 ext;
    ngx_chain_t            *in;
} ngx_rtmp_stream_t;

typedef struct {
    int                           type;
    ngx_str_t                           name;
    void                               *data;
    size_t                              len;
} ngx_rtmp_amf_elt_t;

typedef struct {
    ngx_rtmp_chain_t                        *link, *first;
    size_t                              offset;
    ngx_rtmp_amf_alloc_pt               alloc;
    void                               *arg;
} ngx_rtmp_amf_ctx_t;

typedef struct 
{
	u32 packet_start_time;
	u32 packet_end_time;
	u32 packet_total_size;
	u32 packet_send_size;
	u32 packet_send_avg;
	u32 packet_cbr_avg;
}rtm_manage;

typedef struct {
    double                          trans;
    u_char                          app[NGX_RTMP_MAX_NAME];
    u_char                          args[NGX_RTMP_MAX_ARGS];
    u_char                          flashver[32];
    u_char                          swf_url[NGX_RTMP_MAX_URL];
    u_char                          tc_url[NGX_RTMP_MAX_URL];
    uint32                          acodecs;
    uint32                          vcodecs;
    u_char                          page_url[NGX_RTMP_MAX_URL];
    double                          object_encoding;
} ngx_rtmp_connect_t;


//////////////ngx_rtmp_cmd_module.h
typedef struct {
    double                          trans;
    double                          stream;
} ngx_rtmp_create_stream_t;

typedef struct {
    double                          stream;
} ngx_rtmp_close_stream_t;


typedef struct {
    u_char                          name[NGX_RTMP_MAX_NAME];
    u_char                          args[NGX_RTMP_MAX_ARGS];
    double                          start;
    double                          duration;
    int                             reset;
    int                             silent;
} ngx_rtmp_play_t;

typedef int        ngx_flag_t;
typedef struct ngx_rtmp_live_stream_s ngx_rtmp_live_stream_t;

typedef struct {
    uint64            bytes;
    uint64            bandwidth;      /* bytes/sec */

    time_t              intl_end;
    uint64            intl_bytes;
} ngx_rtmp_bandwidth_t;

struct ngx_rtmp_live_stream_s {
    u_char                              name[NGX_RTMP_MAX_NAME];
    ngx_rtmp_live_stream_t             *next;
//    ngx_rtmp_live_ctx_t                *ctx;
    ngx_rtmp_bandwidth_t                bw_in;
    ngx_rtmp_bandwidth_t                bw_in_audio;
    ngx_rtmp_bandwidth_t                bw_in_video;
    ngx_rtmp_bandwidth_t                bw_out;
    ngx_msec_t                          epoch;
    unsigned                            active:1;
    unsigned                            publishing:1;
};

typedef struct {
    ngx_int_t                           nbuckets;
    ngx_rtmp_live_stream_t            **streams;
    ngx_flag_t                          live;
    ngx_flag_t                          meta;
    ngx_msec_t                          sync;
    ngx_msec_t                          idle_timeout;
    ngx_flag_t                          atc;
    ngx_flag_t                          interleave;
    ngx_flag_t                          wait_key;
    ngx_flag_t                          wait_video;
    ngx_flag_t                          publish_notify;
    ngx_flag_t                          play_restart;
    ngx_flag_t                          idle_streams;
    ngx_msec_t                          buflen;
    ngx_pool_t                         *pool;
    ngx_rtmp_live_stream_t             *free_streams;
} ngx_rtmp_live_app_conf_t;


typedef int (*send_rtmp_t)(void *ctx);

typedef struct  
{           
//	double start_time;  /*assign to random number */
//	u8 valid;
//	void *pcb;		   /*mean udp pcb */
    u32 start_rtptime;
    u32 ssrc;
	u32 start;
	u16 start_seq;
} __attribute__ ((packed)) rtm_session_t;


struct ngx_connection_s {
    void               *data;
//    ngx_event_t        *read;
//    ngx_event_t        *write;

//    ngx_socket_t        fd;

//    ngx_recv_pt         recv;
 //   ngx_send_pt         send;
 //   ngx_recv_chain_pt   recv_chain;
 //   ngx_send_chain_pt   send_chain;

 //   ngx_listening_t    *listening;

    off_t               sent;

  //  ngx_log_t          *log;

    ngx_pool_t         *pool;

//    struct sockaddr    *sockaddr;
//    socklen_t           socklen;
    ngx_str_t           addr_text;

    ngx_str_t           proxy_protocol_addr;

#if (NGX_SSL)
    ngx_ssl_connection_t  *ssl;
#endif

//    struct sockaddr    *local_sockaddr;
//    socklen_t           local_socklen;

    ngx_buf_t          *buffer;

//    ngx_queue_t         queue;

//    ngx_atomic_uint_t   number;

//    ngx_uint_t          requests;

    unsigned            buffered:8;

    unsigned            log_error:3;     /* ngx_connection_log_error_e */

    unsigned            unexpected_eof:1;
    unsigned            timedout:1;
    unsigned            error:1;
    unsigned            destroyed:1;

    unsigned            idle:1;
    unsigned            reusable:1;
    unsigned            close:1;

    unsigned            sendfile:1;
    unsigned            sndlowat:1;
    unsigned            tcp_nodelay:2;   /* ngx_connection_tcp_nodelay_e */
    unsigned            tcp_nopush:2;    /* ngx_connection_tcp_nopush_e */

    unsigned            need_last_buf:1;

#if (NGX_HAVE_IOCP)
    unsigned            accept_context_updated:1;
#endif

#if (NGX_HAVE_AIO_SENDFILE)
    unsigned            busy_count:2;
#endif

#if (NGX_THREADS)
    ngx_thread_task_t  *sendfile_task;
#endif
};


typedef struct 
{
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb; 
	ngx_chain_t 		   *out[RTMP_OUT_CNT+1];
	
	u32 video_height;			/* image hight*/
	u32 video_width;			/* image width*/
	u32 video_framrate;
	
    u_char *in_buffer;//[2*1460];
    u_char *snd_buffer;//[2*1460];
	u_int lenofRTMP;
	ngx_conf_t			 *conf;
//		ngx_event_t 			close;

	void				  **ctx;
	void				  **main_conf;
	void				  **srv_conf;
	void				  **app_conf;

	ngx_str_t			   *addr_text;
	int 					connected;
	u_int	publish_state;

	u_int meta_sent;
	u_int btx_st;
	uint32 start_timestamp;
	uint32 last_timestamp;
	u_int header_sent;	
	u_int header_sent_aud;
	BYTE video_sending;
	BYTE audio_sending;
	BYTE playstart;

//#if (nginx_version >= 1007005)
//		ngx_queue_t 			posted_dry_events;
//#else
//		ngx_event_t 		   *posted_dry_events;
//#endif

	/* client buffer time in msec */
	uint32				buflen;
	uint32				ack_size;

	/* connection parameters */
	ngx_str_t				app;
	ngx_str_t				args;
	ngx_str_t				flashver;
	ngx_str_t				swf_url;
	ngx_str_t				tc_url;
	uint32				acodecs;
	uint32				vcodecs;
	ngx_str_t				page_url;

	/* handshake data */
	ngx_buf_t			   *hs_buf;
	u_char				   *hs_digest;
	unsigned				hs_old:1;
	u_int				hs_stage;

	/* connection timestamps */
	ngx_msec_t				epoch;
	ngx_msec_t				peer_epoch;
	ngx_msec_t				base_time;
	uint32				current_time;

	/* ping */
//		ngx_event_t 			ping_evt;
	unsigned				ping_active:1;
	unsigned				ping_reset:1;

	/* auto-pushed? */
	unsigned				auto_pushed:1;
	unsigned				relay:1;
	unsigned				static_relay:1;

	/* input stream 0 (reserved by RTMP spec)
	 * is used as free chain link */

	ngx_rtmp_stream_t	   *in_streams;
	uint32				in_csid;
	u_int				in_chunk_size;
	ngx_pool_t			   *in_pool;
	uint32				in_bytes;
	uint32				in_last_ack;

	ngx_pool_t			   *in_old_pool;
	ngx_int_t				in_chunk_size_changing;

	ngx_connection_t	   *connection;

	/* circular buffer of RTMP message pointers */
	ngx_msec_t				timeout;
	uint32				out_bytes;
	size_t					out_pos, out_last;
	ngx_chain_t 		   *out_chain;
	u_char				   *out_bpos;
//	unsigned				out_buffer:1;
	size_t					out_queue;
	size_t					out_cork;
	
	u_char sending;
	u_char m_sndaudio;
	u_char m_sndvideo;

	
} __attribute__ ((packed)) ngx_rtmp_session_t;
typedef int                      ngx_fd_t;

#ifndef NGX_ALIGNMENT
#define NGX_ALIGNMENT   sizeof(unsigned long)    /* platform word */
#endif

#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))
#define ngx_align_ptr(p, a)                                                   \
    (u_char *) (((u_int) (p) + ((u_int) a - 1)) & ~((u_int) a - 1))


#define ngx_abort       abort

#define NGX_MODULE_V1          0, 0, 0, 0, 0, 0, 1
#define NGX_MODULE_V1_PADDING  0, 0, 0, 0, 0, 0, 0, 0
#define ngx_null_command  { ngx_null_string, 0, NULL, 0, 0, NULL }

#define NGX_INVALID_FILE         -1
#define NGX_FILE_ERROR           -1

#define ngx_file_size(sb)        (sb)->st_size




#define ngx_get_conf(conf_ctx, module)  conf_ctx[module.index]



#define ngx_conf_init_value(conf, default)                                   \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = default;                                                      \
    }

#define ngx_conf_init_ptr_value(conf, default)                               \
    if (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = default;                                                      \
    }

#define ngx_conf_init_uint_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_init_size_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_init_msec_value(conf, default)                              \
    if (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = default;                                                      \
    }

#define ngx_conf_merge_value(conf, prev, default)                            \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_ptr_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET_PTR) {                                        \
        conf = (prev == NGX_CONF_UNSET_PTR) ? default : prev;                \
    }

#define ngx_conf_merge_uint_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_UINT) {                                       \
        conf = (prev == NGX_CONF_UNSET_UINT) ? default : prev;               \
    }

#define ngx_conf_merge_msec_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_MSEC) {                                       \
        conf = (prev == NGX_CONF_UNSET_MSEC) ? default : prev;               \
    }

#define ngx_conf_merge_sec_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_size_value(conf, prev, default)                       \
    if (conf == NGX_CONF_UNSET_SIZE) {                                       \
        conf = (prev == NGX_CONF_UNSET_SIZE) ? default : prev;               \
    }

#define ngx_conf_merge_off_value(conf, prev, default)                        \
    if (conf == NGX_CONF_UNSET) {                                            \
        conf = (prev == NGX_CONF_UNSET) ? default : prev;                    \
    }

#define ngx_conf_merge_str_value(conf, prev, default)                        \
    if (conf.data == NULL) {                                                 \
        if (prev.data) {                                                     \
            conf.len = prev.len;                                             \
            conf.data = prev.data;                                           \
        } else {                                                             \
            conf.len = sizeof(default) - 1;                                  \
            conf.data = (u_char *) default;                                  \
        }                                                                    \
    }

#define ngx_conf_merge_bufs_value(conf, prev, default_num, default_size)     \
    if (conf.num == 0) {                                                     \
        if (prev.num) {                                                      \
            conf.num = prev.num;                                             \
            conf.size = prev.size;                                           \
        } else {                                                             \
            conf.num = default_num;                                          \
            conf.size = default_size;                                        \
        }                                                                    \
    }

#define ngx_conf_merge_bitmask_value(conf, prev, default)                    \
    if (conf == 0) {                                                         \
        conf = (prev == 0) ? default : prev;                                 \
    }

int rtmp_debug_on;
int rtmp_send_msg;

#endif
#endif
