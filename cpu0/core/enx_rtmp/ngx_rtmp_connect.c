#include "ngx_rtmp_connect.h"
#include "ngx_rtmp_amf.h"
#include "ngx_rtmp_core.h"
#include "ngx_rtmp_array.h"
#include "rtmp_session.h"

#ifdef RTMP_SUPPORT


ngx_int_t
ngx_rtmp_cmd_connect(ngx_rtmp_session_t *s, ngx_rtmp_connect_t *v)
{
 ngx_rtmp_core_srv_conf_t	*cscf;
 ngx_rtmp_core_app_conf_t  **cacfp;
 ngx_uint_t 				 n;
 ngx_rtmp_header_t			 h;
 u_char 					*p;

  static  double			   trans;
  static  double			   capabilities = NGX_RTMP_CAPABILITIES;
  static  double			   object_encoding = 0;

 static ngx_rtmp_amf_elt_t  out_obj[] = {

	 { NGX_RTMP_AMF_STRING,
	   ngx_string("fmsVer"),
	   NGX_RTMP_FMS_VERSION, 0 },

	 { NGX_RTMP_AMF_NUMBER,
	   ngx_string("capabilities"),
	   &capabilities, 0 },
 };

static  ngx_rtmp_amf_elt_t  out_inf[] = {

	 { NGX_RTMP_AMF_STRING,
	   ngx_string("level"),
	   "status", 0 },

	 { NGX_RTMP_AMF_STRING,
	   ngx_string("code"),
	   "NetConnection.Connect.Success", 0 },

	 { NGX_RTMP_AMF_STRING,
	   ngx_string("description"),
	   "Connection succeeded.", 0 },

	 { NGX_RTMP_AMF_NUMBER,
	   ngx_string("objectEncoding"),
	   &object_encoding, 0 }
 };

 static ngx_rtmp_amf_elt_t  out_elts[] = {

	 { NGX_RTMP_AMF_STRING,
	   ngx_null_string,
	   "_result", 0 },

	 { NGX_RTMP_AMF_NUMBER,
	   ngx_null_string,
	   &trans, 0 },

	 { NGX_RTMP_AMF_OBJECT,
	   ngx_null_string,
	   out_obj, sizeof(out_obj) },

	 { NGX_RTMP_AMF_OBJECT,
	   ngx_null_string,
	   out_inf, sizeof(out_inf) },
 };

 if (s->connected) {
	 return NGX_ERROR;
 }

 cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

 trans = v->trans;

 /* fill session parameters */
 s->connected = 1;

 ngx_memzero(&h, sizeof(h));
 h.csid = NGX_RTMP_CSID_AMF_INI;
 h.type = NGX_RTMP_MSG_AMF_CMD;


#define NGX_RTMP_SET_STRPAR(name)                                             \
 s->name.len = ngx_strlen(v->name); 									   \
 s->name.data = ngx_palloc(s->connection->pool, s->name.len);			   \
 ngx_memcpy(s->name.data, v->name, s->name.len)

 NGX_RTMP_SET_STRPAR(app);
 NGX_RTMP_SET_STRPAR(args);
 NGX_RTMP_SET_STRPAR(flashver);
 NGX_RTMP_SET_STRPAR(swf_url);
 NGX_RTMP_SET_STRPAR(tc_url);
 NGX_RTMP_SET_STRPAR(page_url);

#undef NGX_RTMP_SET_STRPAR

 p = ngx_strlchr(s->app.data, s->app.data + s->app.len, '?');
 if (p) {
	 s->app.len = (p - s->app.data);
 }
 
 s->acodecs = v->acodecs;
 s->vcodecs = v->vcodecs;

#if 0
 /* find application & set app_conf */
 cacfp = cscf->applications.elts;
 printf("cscf->applications.nelts %d \n",cscf->applications.nelts);
 for(n = 0; n < cscf->applications.nelts; ++n, ++cacfp) {
	 printf("(*cacfp)->name.len %d %s \n",(*cacfp)->name.len, (*cacfp)->name.data);
	 if ((*cacfp)->name.len == s->app.len &&
		 ngx_strncmp((*cacfp)->name.data, s->app.data, s->app.len) == 0)
	 {
		 /* found app! */
		 s->app_conf = (*cacfp)->app_conf;
		 break;
	 }
 }

 if (s->app_conf == NULL) {
	 printf("connect: application not found: '%V'", &s->app);
	 return NGX_ERROR;
 }
#endif
 object_encoding = v->object_encoding;

 return ngx_rtmp_send_ack_size(s, cscf->ack_window) != NGX_OK 
 	||		ngx_rtmp_send_bandwidth(s, cscf->ack_window, NGX_RTMP_LIMIT_DYNAMIC) != NGX_OK
	||		ngx_rtmp_send_chunk_size(s, cscf->chunk_size) != NGX_OK 
	||		ngx_rtmp_send_amf(s, &h, out_elts,  sizeof(out_elts) / sizeof(out_elts[0]))	!= NGX_OK
		? NGX_ERROR : NGX_OK;
}


 int
ngx_rtmp_cmd_connect_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
        ngx_rtmp_chain_t *in)
{
    size_t                      len;
	
	rtmp_connect_t	 v;
	
    rtmp_memzero(&v, sizeof(v));
	
	ngx_rtmp_amf_elt_t	in_cmd[] = {
	
		{ RTMP_AMF_STRING,
		  rtmp_string("app"),
		  v.app, sizeof(v.app) },
	
		{ RTMP_AMF_STRING,
		  rtmp_string("flashVer"),
		  v.flashver, sizeof(v.flashver) },
	
		{ RTMP_AMF_STRING,
		  rtmp_string("swfUrl"),
		  v.swf_url, sizeof(v.swf_url) },
	
		{ RTMP_AMF_STRING,
		  rtmp_string("tcUrl"),
		  v.tc_url, sizeof(v.tc_url) },
	
		{ RTMP_AMF_NUMBER,
		  rtmp_string("audioCodecs"),
		  &v.acodecs, sizeof(v.acodecs) },
	
		{ RTMP_AMF_NUMBER,
		  rtmp_string("videoCodecs"),
		  &v.vcodecs, sizeof(v.vcodecs) },
	
		{ RTMP_AMF_STRING,
		  rtmp_string("pageUrl"),
		  v.page_url, sizeof(v.page_url) },
	
		{ RTMP_AMF_NUMBER,
		  rtmp_string("objectEncoding"),
		  &v.object_encoding, 0},
	};
	
	ngx_rtmp_amf_elt_t	in_elts[] = {
	
		{ RTMP_AMF_NUMBER,
		  rtmp_null_string,
		  &v.trans, 0 },
	
		{ RTMP_AMF_OBJECT,
		  rtmp_null_string,
		  in_cmd, sizeof(in_cmd) },
	};

    if (ngx_rtmp_receive_amf(s, in, in_elts,
                (sizeof(in_elts) / sizeof(in_elts[0]))))
    {
        return RTMP_ERROR;
    }

    len = rtmp_strlen(v.app);
    if (len > 10 && !rtmp_memcmp(v.app + len - 10, "/_definst_", 10)) {
        v.app[len - 10] = 0;
    } else if (len && v.app[len - 1] == '/') {
        v.app[len - 1] = 0;
    }

    ngx_rtmp_cmd_fill_args(v.app, v.args);

/*    printf("connect: app='%s' args='%s' flashver='%s' swf_url='%s' "
            "tc_url='%s' page_url='%s' acodecs=%d vcodecs=%d "
            "object_encoding=%d\n",
            v.app, v.args, v.flashver, v.swf_url, v.tc_url, v.page_url,
            (uint32)v.acodecs, (uint32)v.vcodecs,
            (int)v.object_encoding);
*/	
	ngx_rtmp_amf_elt_t *tv = (ngx_rtmp_amf_elt_t *) in_elts[1].data;

    return ngx_rtmp_cmd_connect(s, &v);
}

void
ngx_rtmp_finalize_session(ngx_rtmp_session_t *s)
{
//    ngx_event_t		  *e;
// ngx_connection_t	*c;

// c = s->connection;
// if (c->destroyed) {
// printf("c->destroyed %d\n",c->destroyed);
//	 return;
// }

// printf("finalize session\n");

// c->destroyed = 1;
//    e = &s->close;
//    e->data = s;
//    e->handler = ngx_rtmp_close_session_handler;
//    e->log = c->log;

//    ngx_post_event(e, &ngx_posted_events);
 //rtmp_close(s->tpcb);
 rtmp_disconnect();
}

ngx_chain_t *
ngx_rtmp_create_push_connect(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;

    int                      connect = 1;
    double                   trans;

    ngx_rtmp_amf_elt_t       connect_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_string("app"),
          url_application, 0},
          
//#ifdef TEST_SERVER1		         
//          "live", 0 },
//#else
//#ifdef TEST_SERVER2
//			"live2", 0 },
//#else
//			"live", 0 },
//#endif
//#endif          

		{ NGX_RTMP_AMF_STRING,
		  ngx_string("type"),
		  "nonprivate", 0 },
		  
		{ NGX_RTMP_AMF_STRING,
		  ngx_string("tcUrl"),
		  url_tcUrl, 0},
		  
//#ifdef TEST_SERVER1		  
//		  "rtmp://192.168.2.215:1935/live", 0 },
//#else
//#ifdef TEST_SERVER2
//		  "rtmp://74.125.102.231:1935/live2", 0 },
//#else
//		  "rtmp://w.gslb.lecloud.com/live", 0 },
//#endif		  
//#endif
    };

    ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "connect", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,          
		  &trans, 0 },
          
        { NGX_RTMP_AMF_OBJECT,
          ngx_null_string,
          connect_elts, sizeof(connect_elts) },
    };

    memset(&h, 0, sizeof(h));

	trans = 1;

    h.type = NGX_RTMP_MSG_AMF_CMD;
    h.csid = NGX_RTMP_CSID_AMF_INI;
    h.msid = 0;

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
}

ngx_int_t
ngx_rtmp_send_push_connect(ngx_rtmp_session_t *s)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_push_connect(s));
}

#endif
