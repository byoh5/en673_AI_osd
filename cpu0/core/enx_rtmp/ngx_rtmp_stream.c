#include "ngx_rtmp_stream.h"
#include "ngx_rtmp_amf.h"
#include "ngx_rtmp_core.h"
#include "ngx_rtmp_connect.h"

#ifdef RTMP_SUPPORT

 ngx_int_t
ngx_rtmp_cmd_create_stream(ngx_rtmp_session_t *s, ngx_rtmp_create_stream_t *v)
{
    /* support one message stream per connection */
      static double               stream;
      static double               trans;
    static  ngx_rtmp_header_t           h;

    static  ngx_rtmp_amf_elt_t  out_elts2[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "_result", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &trans, 0 },

        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &stream, sizeof(stream) },
    };

    trans = v->trans;
    stream = NGX_RTMP_MSID;

    ngx_memzero(&h, sizeof(h));

    h.csid = NGX_RTMP_CSID_AMF_INI;
    h.type = NGX_RTMP_MSG_AMF_CMD;

    return ngx_rtmp_send_amf(s, &h, out_elts2,
                             sizeof(out_elts2) / sizeof(out_elts2[0])) == NGX_OK ?
           RTMP_OK : NGX_ERROR;
}




 ngx_int_t
ngx_rtmp_cmd_create_stream_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
                                ngx_chain_t *in)
{printf("%s\n",__func__);
     static ngx_rtmp_create_stream_t     v;

     static ngx_rtmp_amf_elt_t  in_elts[] = {

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &v.trans, sizeof(v.trans) },
    };

    if (ngx_rtmp_receive_amf(s, in, in_elts,
                sizeof(in_elts) / sizeof(in_elts[0])))
    {
        return NGX_ERROR;
    }

    printf("createStream\n");

    return ngx_rtmp_cmd_create_stream(s, &v);
}



ngx_chain_t *
ngx_rtmp_create_sample_access(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;

     int                      access = 1;

    ngx_rtmp_amf_elt_t       access_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "|RtmpSampleAccess", 0 },

        { NGX_RTMP_AMF_BOOLEAN,
          ngx_null_string,
          &access, 0 },

        { NGX_RTMP_AMF_BOOLEAN,
          ngx_null_string,
          &access, 0 },
    };

    memset(&h, 0, sizeof(h));

    h.type = NGX_RTMP_MSG_AMF_META;
    h.csid = NGX_RTMP_CSID_AMF;
    h.msid = NGX_RTMP_MSID;

    return ngx_rtmp_create_amf(s, &h, access_elts,
                               sizeof(access_elts) / sizeof(access_elts[0]));
}

ngx_chain_t *
ngx_rtmp_create_status(ngx_rtmp_session_t *s, char *code, char* level,
                       char *desc)
{
    ngx_rtmp_header_t               h;
     double                   trans;

    ngx_rtmp_amf_elt_t       out_inf[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_string("level"),
          NULL, 0 },

        { NGX_RTMP_AMF_STRING,
          ngx_string("code"),
          NULL, 0 },

        { NGX_RTMP_AMF_STRING,
          ngx_string("description"),
          NULL, 0 },
    };

    ngx_rtmp_amf_elt_t       out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "onStatus", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &trans, 0 },

        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },

        { NGX_RTMP_AMF_OBJECT,
          ngx_null_string,
          out_inf,
          sizeof(out_inf) },
    };

    printf("create: status code='%s' level='%s' desc='%s'\n",
                   code, level, desc);
	trans = 0;

    out_inf[0].data = level;
    out_inf[1].data = code;
    out_inf[2].data = desc;

    memset(&h, 0, sizeof(h));

    h.type = NGX_RTMP_MSG_AMF_CMD;
    h.csid = NGX_RTMP_CSID_AMF;
    h.msid = NGX_RTMP_MSID;

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
}


ngx_int_t
ngx_rtmp_send_sample_access(ngx_rtmp_session_t *s)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_sample_access(s));
}

ngx_int_t
ngx_rtmp_send_status(ngx_rtmp_session_t *s, char *code, char* level, char *desc)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_status(s, code, level, desc));
}

//////////////////ngx_rtmp_send.c
/* User control messages */

ngx_chain_t *
ngx_rtmp_create_stream_begin(ngx_rtmp_session_t *s, uint32 msid)
{
    printf("create: stream_begin msid=%u\n", msid);

    {
        NGX_RTMP_UCTL_START(s, NGX_RTMP_MSG_USER, NGX_RTMP_USER_STREAM_BEGIN);

        NGX_RTMP_USER_OUT4(msid);

        NGX_RTMP_USER_END(s);
    }
}

ngx_int_t
ngx_rtmp_send_stream_begin(ngx_rtmp_session_t *s, uint32 msid)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_stream_begin(s, msid));
}

int getH264data(ngx_rtmp_session_t* s, h264_tx_info_t* ph264_info,sbuf_t *getsbuf) 
{
	u32 base, nTs, i;
	uint8* un8DataPoint;
	uint32 un32FtchNum = 0;
	uint32 un32NALTest = 0, un32NALzeroCount = 0;

	if(s->btx_st != H264_TX_READY)
	{
		printf("H264 not ready\r\n");
		return RTMP_AGAIN;		
	}
	ph264_info->nSp = 0;
	
	//if(sbuf_get(getsbuf, &base, &ph264_info->ntotalsize, &ph264_info->iframe, &nTs) != DEF_OK)
	if(sbuf_get(getsbuf, (UINT *)&base, (UINT *)&ph264_info->ntotalsize, (UINT *)&ph264_info->iframe, (UINT *)&nTs) != DEF_OK)
	{
	    return RTMP_ERROR;
	}
	
	un8DataPoint = (u8 *)base;
	
	if(*(un8DataPoint) == 0x00)
	{
		for(i=0;i<20;i++)	// 현 위치부터 20바이트를 검사한다.(최대 sps(4+11), pps(4+5), IDR(4+1))
		{					// 권장사항 : un32FtchNum만큼 검사하는게 제일 좋다
			if(*(un8DataPoint+i) == 0x00)
			{
				un32NALzeroCount++;
				if(*(un8DataPoint+i+1) == 0x01 && un32NALzeroCount >= 2)	// NAL Start의 0x00이 2개 이상이다. 3, 4, 5개도 체크해야 함
				{
					un32NALzeroCount++;
					un32NALTest = 1;
					break;
				}
			}
			else
			{
				un32NALzeroCount = 0;
			}
		}
	}	
	
	if(un32NALTest == 0)
	{		
		while(ph264_info->ntotalsize==0 || ph264_info->iframe != 1)
		{
			if(sbuf_get(getsbuf, &base, &ph264_info->ntotalsize, &ph264_info->iframe, &nTs) != DEF_OK)
			{
				return RTMP_ERROR;
			}
		}
	}
	
	while(ph264_info->ntotalsize==0||(!ph264_info->sendFlag && ph264_info->iframe != 1))
	{
		if(sbuf_get(getsbuf, &base, &ph264_info->ntotalsize, &ph264_info->iframe, &nTs) != DEF_OK)
		{
			return RTMP_ERROR;
		}
	}
	//printf(" ttsz %d \n",ph264_info->ntotalsize);
	ph264_info->base = (u8 *)base;

	// ts update start
	s->start_timestamp += (nTs/100);///100;///1000;
	//printf("[nTs %ld s->start_timestamp  %d] ",nTs,s->start_timestamp );
	ph264_info->ts = s->start_timestamp;


	ph264_info->nsize = ph264_info->ntotalsize;
	s->btx_st = H264_TX_END;//H264_TX_DOING;
	ph264_info->bFirst = 1;
	//printf("H264 size %d\r\n",ph264_info->nsize);		
	return RTMP_OK;

}
 
ngx_chain_t *
ngx_rtmp_create_stream_eof(ngx_rtmp_session_t *s, uint32 msid)
{
 printf("create: stream_end msid=%u%d\n", msid);

 {
	 NGX_RTMP_UCTL_START(s, NGX_RTMP_MSG_USER, NGX_RTMP_USER_STREAM_EOF);

	 NGX_RTMP_USER_OUT4(msid);

	 NGX_RTMP_USER_END(s);
 }
}

ngx_int_t
ngx_rtmp_send_stream_eof(ngx_rtmp_session_t *s, uint32 msid)
{
 return ngx_rtmp_send_shared_packet(s,
		ngx_rtmp_create_stream_eof(s, msid));
}

ngx_chain_t *
ngx_rtmp_create_play_status(ngx_rtmp_session_t *s, char *code, char* level,
                            ngx_uint_t duration, ngx_uint_t bytes)
{
    ngx_rtmp_header_t               h;
     double                   dduration;
     double                   dbytes;

    ngx_rtmp_amf_elt_t       out_inf[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_string("code"),
          NULL, 0 },

        { NGX_RTMP_AMF_STRING,
          ngx_string("level"),
          NULL, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("duration"),
          &dduration, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("bytes"),
          &dbytes, 0 },
    };

    ngx_rtmp_amf_elt_t       out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "onPlayStatus", 0 },

        { NGX_RTMP_AMF_OBJECT,
          ngx_null_string,
          out_inf,
          sizeof(out_inf) },
    };

    printf("create: play_status code='%s' level='%s' "
                   "duration=%u bytes=%u\n",
                   code, level, duration, bytes);

    out_inf[0].data = code;
    out_inf[1].data = level;

    dduration = duration;
    dbytes = bytes;

    memset(&h, 0, sizeof(h));

    h.type = NGX_RTMP_MSG_AMF_META;
    h.csid = NGX_RTMP_CSID_AMF;
    h.msid = NGX_RTMP_MSID;
    h.timestamp = duration;

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
}

ngx_int_t
ngx_rtmp_send_play_status(ngx_rtmp_session_t *s, char *code, char* level,
                          ngx_uint_t duration, ngx_uint_t bytes)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_play_status(s, code, level, duration, bytes));
}

 ngx_int_t
ngx_rtmp_live_play(ngx_rtmp_session_t *s, ngx_rtmp_play_t *v)
{
    ngx_rtmp_live_app_conf_t       *lacf;
//    ngx_rtmp_live_ctx_t            *ctx;

 //   lacf = ngx_rtmp_get_module_app_conf(s, ngx_rtmp_live_module);

//    if (lacf == NULL || !lacf->live) {
//        goto next;
//    }

/*    printf("live: play: name='%s' start=%d duration=%d reset=%d\n",
                   v->name, (uint32) v->start,
                   (uint32) v->duration, (uint32) v->reset);
*/
    /* join stream as subscriber */

//    ngx_rtmp_live_join(s, v->name, 0);

//    ctx = ngx_rtmp_get_module_ctx(s, ngx_rtmp_live_module);
 //   if (ctx == NULL) {
 //       goto next;
 //   }

 //   ctx->silent = v->silent;

/////////////////////////////////////////////////////
//
    if (ngx_rtmp_send_stream_begin(s, NGX_RTMP_MSID) != NGX_OK) {
		printf("ngx_rtmp_send_stream_begin NGX_ERROR\n");
        return NGX_ERROR;
    }
	
/////////////////////////////////////////////////////

 //   if (!ctx->silent && !lacf->play_restart) {
        ngx_rtmp_send_status(s, "NetStream.Play.Start",
                             "status", "Start live");
      ngx_rtmp_send_sample_access(s);
//    }

//next:
	s->btx_st = H264_TX_READY;
	s->meta_sent = 0;
	s->start_timestamp = 0;
	s->header_sent  = 0;
	//s->sending = 0;
	s->header_sent_aud = 0;
	s->playstart = 1;	
	printf("%s s->meta_sent %d\n",__func__,s->meta_sent);

	//timer_sched_timeout_cb(30,TIMEOUT_PERIODIC, ngx_rtmp_play_send, NULL);

    return NGX_OK;
}


 ngx_int_t
ngx_rtmp_cmd_play_init(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
        ngx_chain_t *in)
{printf("%s\n",__func__);
    ngx_rtmp_play_t          v;

    ngx_rtmp_amf_elt_t       in_elts3[] = {

        /* transaction is always 0 */
        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          NULL, 0 },

        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          &v.name, sizeof(v.name) },

        { NGX_RTMP_AMF_OPTIONAL | NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &v.start, 0 },

        { NGX_RTMP_AMF_OPTIONAL | NGX_RTMP_AMF_NUMBER,
          ngx_null_string,
          &v.duration, 0 },

        { NGX_RTMP_AMF_OPTIONAL | NGX_RTMP_AMF_BOOLEAN,
          ngx_null_string,
          &v.reset, 0 }
    };

    ngx_memzero(&v, sizeof(v));

    if (ngx_rtmp_receive_amf(s, in, in_elts3,
                             sizeof(in_elts3) / sizeof(in_elts3[0])))
    {
        return NGX_ERROR;
    }

    ngx_rtmp_cmd_fill_args(v.name, v.args);

    printf("play: name='%s' args='%s' start=%i duration=%i "
                  "reset=%i silent=%i\n",
                  v.name, v.args, (ngx_int_t) v.start,
                  (ngx_int_t) v.duration, (ngx_int_t) v.reset,
                  (ngx_int_t) v.silent);

    return ngx_rtmp_live_play(s, &v);
}


ngx_rtmp_chain_t *
rtmp_create_ack_size(ngx_rtmp_session_t *s, uint32 ack_size)
{
   printf("create: ack_size=%d\n", ack_size);

    //{
//printf("create: 0\n");
NGX_RTMP_USER_START(s, NGX_RTMP_MSG_ACK_SIZE);
/*
	ngx_rtmp_header_t				__h;
	ngx_rtmp_chain_t					*__l;
	ngx_rtmp_buf_t						*__b;
	ngx_rtmp_core_srv_conf_t	   *__cscf;

	//__cscf = (ngx_rtmp_core_srv_conf_t	   *)ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module); 
	if(s->srv_conf == NULL)
		printf("s->srv_conf is NULL\n");

	printf("ngx_rtmp_core_module.ctx_index %d\n",ngx_rtmp_core_module.ctx_index);
	if(s->srv_conf[ngx_rtmp_core_module.ctx_index] == NULL)
	{
		printf("srv_conf == NULL\n");
	}
	else
	{
		__cscf = s->srv_conf[ngx_rtmp_core_module.ctx_index];
	}
	printf("create: 1\n");
	memset(&__h, 0, sizeof(__h));
	__h.type = NGX_RTMP_MSG_ACK_SIZE;
	__h.csid = 2;
	__l = ngx_rtmp_alloc_shared_buf(__cscf);
	if (__l == NULL) {
		printf("rtmp_create_ack_size return NULL\n");
		return NULL;
	}
	__b = __l->buf;
		
*/
NGX_RTMP_USER_OUT4(ack_size);
/*printf("create: 2n");
	*(__b->last++) = ((u_char*)&ack_size)[0];
	*(__b->last++) = ((u_char*)&ack_size)[1];
	*(__b->last++) = ((u_char*)&ack_size)[2];
	*(__b->last++) = ((u_char*)&ack_size)[3];
*/
NGX_RTMP_USER_END(s);
/*printf("create: 3\n");
    ngx_rtmp_prepare_message(s, &__h, NULL, __l);
    return __l;		
	*/	
//    }
}

int
ngx_rtmp_send_ack_size(ngx_rtmp_session_t *s, uint32 ack_size)
{printf("%s\n",__func__);
    return ngx_rtmp_send_shared_packet(s,
           rtmp_create_ack_size(s, ack_size));
}


 ngx_chain_t *
ngx_rtmp_create_release_stream(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;
#ifdef TEST_SERVER1
			ngx_rtmp_header_t				lh;
#endif

	
#ifdef TEST_SERVER3
		ngx_rtmp_header_t				lh;
#endif
#ifdef TEST_SERVER5
			 ngx_rtmp_header_t				 lh;
#endif

     int                      connect = 1;
     double                   trans;

    ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "releaseStream", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,          
		  &trans, 0 },
          
        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },
          
        { NGX_RTMP_AMF_STRING,
		  ngx_null_string,
    	  url_stream_name, 0 },

//#ifdef TEST_SERVER1
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//		  "h264", 0 },
//#else
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//#ifdef TEST_SERVER2		  
//		  "s6a3-hkj1-ar60-3wbr", 0 },
//#else
//		  "201605303000000hj99", 0 },
//#endif
//#endif		  
    };

    memset(&h, 0, sizeof(h));

	trans = 2;

    h.type = NGX_RTMP_MSG_AMF_CMD;
    h.csid = NGX_RTMP_CSID_AMF_INI;
    h.msid = 0;
#ifdef TEST_SERVER1
		lh = h;
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
								   sizeof(out_elts) / sizeof(out_elts[0]));
#else	
#ifdef TEST_SERVER3
	lh = h;
    return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
#else
#ifdef TEST_SERVER5
		lh = h;
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
								   sizeof(out_elts) / sizeof(out_elts[0]));
#else

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
#endif
#endif
#endif
}

 ngx_int_t
ngx_rtmp_send_release_stream(ngx_rtmp_session_t *s)
{printf("%s\n",__func__);
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_release_stream(s));
}


 ngx_chain_t *
ngx_rtmp_create_publish(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;

     int                      connect = 1;
     double                   trans;

    ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "publish", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,          
		  &trans, 0 },
          
        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },
          
		{ NGX_RTMP_AMF_STRING,
		  ngx_null_string,
		  url_stream_name, 0 },
		  
		{ NGX_RTMP_AMF_STRING,
		  ngx_null_string,
		  "live", 0 },


//#ifdef TEST_SERVER1
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//		  "h264", 0 },
//		  
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//		  "live", 0 },
//#else
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//#ifdef TEST_SERVER2		  
//		  "s6a3-hkj1-ar60-3wbr", 0 },
//#else
//		  "201605303000000hj99", 0 },
//#endif
//		  
//		{ NGX_RTMP_AMF_STRING,
//		  ngx_null_string,
//		  "live", 0 },
//#endif		  
    };

    memset(&h, 0, sizeof(h));
#ifdef TEST_SERVER1
	trans = 6;
#else
#ifdef TEST_SERVER3
	trans = 6;
#else
#ifdef TEST_SERVER5
		trans = 6;
#else

	trans = 5;
#endif
#endif
#endif

    h.type = NGX_RTMP_MSG_AMF_CMD;
#ifdef TEST_SERVER1
    h.csid = NGX_RTMP_CSID_AMF_INI+1;
#else
#ifdef TEST_SERVER3
    h.csid = NGX_RTMP_CSID_AMF_INI+1;
#else
#ifdef TEST_SERVER5
		h.csid = NGX_RTMP_CSID_AMF_INI+1;
#else

    h.csid = NGX_RTMP_CSID_AMF_INI;
#endif
#endif
#endif
    h.msid = 1;//0;

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
}

 ngx_int_t
ngx_rtmp_send_publish(ngx_rtmp_session_t *s)
{printf("%s\n",__func__);
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_publish(s));
}

 ngx_chain_t *
ngx_rtmp_create_fcpublish(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;
#ifdef TEST_SERVER1
		ngx_rtmp_header_t				lh;
#endif
#ifdef TEST_SERVER3
	ngx_rtmp_header_t				lh;
#endif
#ifdef TEST_SERVER5
		 ngx_rtmp_header_t				 lh;
#endif

     int                      connect = 1;
     double                   trans;

    ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "FCPublish", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,          
		  &trans, 0 },
          
        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },
		
		{ NGX_RTMP_AMF_STRING,
		  ngx_null_string,
     	  url_stream_name, 0 },

//#ifdef TEST_SERVER1
//			"h264", 0 },
//#else
//#ifdef TEST_SERVER2		  
//			"s6a3-hkj1-ar60-3wbr", 0 },
//#else
//			"201605303000000hj99", 0 },
//#endif
//#endif
    };

    memset(&h, 0, sizeof(h));

	trans = 3;

    h.type = NGX_RTMP_MSG_AMF_CMD;
    h.csid = NGX_RTMP_CSID_AMF_INI;
    h.msid = 0;
#ifdef TEST_SERVER1
		lh = h;
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
							   sizeof(out_elts) / sizeof(out_elts[0]));
	
#else
#ifdef TEST_SERVER3	
	lh = h;
	return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
						   sizeof(out_elts) / sizeof(out_elts[0]));

#else
#ifdef TEST_SERVER5	
		lh = h;
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
							   sizeof(out_elts) / sizeof(out_elts[0]));
	
#else

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
#endif
#endif
#endif	
}

 ngx_int_t
ngx_rtmp_send_fcpublish(ngx_rtmp_session_t *s)
{printf("%s\n",__func__);
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_fcpublish(s));
}

 ngx_chain_t *
ngx_rtmp_create_stream(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;
#ifdef TEST_SERVER1	
    ngx_rtmp_header_t               lh;
#endif	
#ifdef TEST_SERVER3	
    ngx_rtmp_header_t               lh;
#endif
#ifdef TEST_SERVER5
		 ngx_rtmp_header_t				 lh;
#endif

     int                      connect = 1;
     double                   trans;

    ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "createStream", 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_null_string,          
		  &trans, 0 },
          
        { NGX_RTMP_AMF_NULL,
          ngx_null_string,
          NULL, 0 },
    };

    memset(&h, 0, sizeof(h));

	trans = 4;

    h.type = NGX_RTMP_MSG_AMF_CMD;
    h.csid = NGX_RTMP_CSID_AMF_INI;
    h.msid = 0;
#ifdef TEST_SERVER1
		lh = h;
		
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
								   sizeof(out_elts) / sizeof(out_elts[0]));
#else
#ifdef TEST_SERVER3	
	lh = h;
	
    return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
#else
#ifdef TEST_SERVER5
		lh = h;
		
		return ngx_rtmp_create_amf2(s, &h, &lh, out_elts,
								   sizeof(out_elts) / sizeof(out_elts[0]));
#else

	return ngx_rtmp_create_amf(s, &h, out_elts, 
							   sizeof(out_elts) / sizeof(out_elts[0]));
#endif
#endif
#endif
}

 ngx_int_t
ngx_rtmp_send_create_stream(ngx_rtmp_session_t *s)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_stream(s));
}
 
ngx_chain_t *
ngx_rtmp_create_checkbw(ngx_rtmp_session_t *s)
{
 ngx_rtmp_header_t				 h;

  int					   connect = 1;
  double				   trans;

 ngx_rtmp_amf_elt_t  out_elts[] = {

	 { NGX_RTMP_AMF_STRING,
	   ngx_null_string,
	   "_checkbw", 0 },

	 { NGX_RTMP_AMF_NUMBER,
	   ngx_null_string, 		 
	   &trans, 0 },
	   
	 { NGX_RTMP_AMF_NULL,
	   ngx_null_string,
	   NULL, 0 },
 };

 memset(&h, 0, sizeof(h));

 trans = 5;

 h.type = NGX_RTMP_MSG_AMF_CMD;
 h.csid = NGX_RTMP_CSID_AMF_INI;
 h.msid = 0;

 return ngx_rtmp_create_amf(s, &h, out_elts,
							sizeof(out_elts) / sizeof(out_elts[0]));
}

ngx_int_t
ngx_rtmp_send_create_checkbw(ngx_rtmp_session_t *s)
{
 return ngx_rtmp_send_shared_packet(s,
		ngx_rtmp_create_checkbw(s));
}


 ngx_chain_t *
ngx_rtmp_create_setdataFrame(ngx_rtmp_session_t *s)
{
    ngx_rtmp_header_t               h;

     double                   trans;

	
   
      double                      aucoid;
      double                      audarate;
      double                      ausamrate;
      double                      vicoid;
      double                      framrate;
      double                      width;
      double                      heigt;
	  double					  duration;
	  double					  videodatarate;
   	  double 				      audiodatarate;
	  double 					  audiosamplesize;
	  double					  filesz;
	  int 						  stereo;
		
#ifdef TEST_SERVER3
    ngx_rtmp_amf_elt_t       stat_inf[] = {

		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("duration"),
		   &duration, 0 },
	 
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("width"),
		   &width, 0 },
		 
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("height"),
		   &heigt, 0 },
			

		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("videodatarate"),
		   &videodatarate, 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("framerate"),
		   &framrate, 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("videocodecid"),
		   &vicoid, 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiodatarate"),
		   &audiodatarate, 0 },
			 /*

		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiosamplerate"),
		   &ausamrate, 0 },		 
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiosamplesize"),
		   &audiosamplesize, 0 },		 
		   
		 { NGX_RTMP_AMF_BOOLEAN,
		   ngx_string("stereo"),
		   &stereo, 0 },		 
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiocodecid"),
		   &aucoid, 0 },
		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("major_brand"),
		   "avc1", 0 },
		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("minor_version"),
		   "0", 0 },
		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("compatible_brands"),
		   "avc1isom", 0 },
		 
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("encoder"),
		   "Lavf56.4.101", 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("filesize"),
		   &filesz, 0 },
*/		   
    };

#else
#ifdef TEST_SERVER5
		ngx_rtmp_amf_elt_t		 stat_inf[] = {
	
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("duration"),
			   &duration, 0 },
		 
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("width"),
			   &width, 0 },
			 
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("height"),
			   &heigt, 0 },
				
	
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("videodatarate"),
			   &videodatarate, 0 },
			   
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("framerate"),
			   &framrate, 0 },
			   
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("videocodecid"),
			   &vicoid, 0 },
			   
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("audiodatarate"),
			   &audiodatarate, 0 },
				 
	
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("audiosamplerate"),
			   &ausamrate, 0 }, 	 
			   
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("audiosamplesize"),
			   &audiosamplesize, 0 },		 
			   
			 { NGX_RTMP_AMF_BOOLEAN,
			   ngx_string("stereo"),
			   &stereo, 0 },		 
			   
			 { NGX_RTMP_AMF_NUMBER,
			   ngx_string("audiocodecid"),
			   &aucoid, 0 },			
			 { NGX_RTMP_AMF_NUMBER,
				ngx_string("filesize"),
				&filesz, 0 },

			 { NGX_RTMP_AMF_STRING,
			   ngx_string("encoder"),
			   "Lavf57.42.100", 0 }
			   /*
			 { NGX_RTMP_AMF_STRING,
			   ngx_string("major_brand"),
			   "avc1", 0 },
			   
			 { NGX_RTMP_AMF_STRING,
			   ngx_string("minor_version"),
			   "0", 0 },
			   
			 { NGX_RTMP_AMF_STRING,
			   ngx_string("compatible_brands"),
			   "avc1isom", 0 },
			 

			   
			 
	*/		   
		};
	
#else

    ngx_rtmp_amf_elt_t       stat_inf[] = {
/*
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("duration"),
		   &duration, 0 },
	 */
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("width"),
		   &width, 0 },
		 
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("height"),
		   &heigt, 0 },
/*
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("videodatarate"),
		   &videodatarate, 0 },
	*/	   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("framerate"),
		   &framrate, 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("videocodecid"),
		   &vicoid, 0 },
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiodatarate"),
		   &audiodatarate, 0 },

		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiosamplerate"),
		   &ausamrate, 0 },		 
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiocodecid"),
		   &aucoid, 0 },
	
		   
		 { NGX_RTMP_AMF_NUMBER,
		   ngx_string("audiosamplesize"),
		   &audiosamplesize, 0 },		 
		   
		 { NGX_RTMP_AMF_BOOLEAN,
		   ngx_string("stereo"),
		   &stereo, 0 },		 
/*		   
		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("major_brand"),
		   "avc1", 0 },
		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("minor_version"),
		   "0", 0 },
*/
/*		   
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("compatible_brands"),
		   "avc1isom", 0 },
		 
		 { NGX_RTMP_AMF_STRING,
		   ngx_string("encoder"),
		   "Lavf56.4.101", 0 },*/
		 
    };
#endif
#endif
     ngx_rtmp_amf_elt_t  out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "@setDataFrame", 0 },
			
		{ NGX_RTMP_AMF_STRING,
		  ngx_null_string,
		  "onMetaData", 0 },
	
		//{ NGX_RTMP_AMF_OBJECT,
		//  ngx_null_string,
		//  stat_inf, sizeof(stat_inf) },
		
		{ NGX_RTMP_AMF_MIXED_ARRAY,
		  ngx_null_string,
		  stat_inf, sizeof(stat_inf)  },

		//{ NGX_RTMP_AMF_OBJECT,
		//  ngx_null_string,
		//  stat_inf, sizeof(stat_inf) },		  
		

    };

    memset(&h, 0, sizeof(h));
/*	
	SoundFormat
	(See notes following 
	table, for special 
	encodings)
	UB [4] Format of SoundData. The following values are defined:
	0 = Linear PCM, platform endian
	1 = ADPCM
	2 = MP3
	3 = Linear PCM, little endian
	4 = Nellymoser 16 kHz mono
	5 = Nellymoser 8 kHz mono
	6 = Nellymoser
	7 = G.711 A-law logarithmic PCM
	8 = G.711 mu-law logarithmic PCM
	9 = reserved
	10 = AAC
	11 = Speex
	14 = MP3 8 kHz
	15 = Device-specific sound
	Formats 7, 8, 14, and 15 are reserved.
	AAC is supported in Flash Player 9,0,115,0 and higher.
	Speex is supported in Flash Player 10 and higher.
*/
    h.type = NGX_RTMP_MSG_AMF_META;
    h.csid = NGX_RTMP_CSID_AMF_INI+1;
    h.msid = 1;
	
	trans = 7;

	duration = 0;
	videodatarate = 0;
	audiodatarate = 62.5;//125;
	audiosamplesize = 16;//8;//16;
	aucoid = 8;//10;
	audarate = 62.5;//125;//32;
    ausamrate = 8000;//44100;
    vicoid = 7;
    framrate = (double)s->video_framrate;
    width = (double)s->video_width;
    heigt = (double)s->video_height;
	stereo = 0;
	filesz = 0;

    return ngx_rtmp_create_amf(s, &h, out_elts,
                               sizeof(out_elts) / sizeof(out_elts[0]));
}

 ngx_int_t
ngx_rtmp_send_setdataFrame(ngx_rtmp_session_t *s)
{printf("%s\n",__func__);
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_setdataFrame(s));
}
 
ngx_int_t
ngx_rtmp_protocol_message_handler(ngx_rtmp_session_t *s,
	 ngx_rtmp_header_t *h, ngx_chain_t *in)
{
 ngx_buf_t				*b;
 u_char 				*p;
 uint32				 val;
 uint8				 limit;

 b = in->buf;

 if (b->last - b->pos < 4) {
	 printf("too small buffer for %d message: %d\n",
			 (int)h->type, b->last - b->pos);
	 return NGX_OK;
 }

 p = (u_char*)&val;
 p[0] = b->pos[0];
 p[1] = b->pos[1];
 p[2] = b->pos[2];
 p[3] = b->pos[3];

 switch(h->type) {
	 case NGX_RTMP_MSG_CHUNK_SIZE:
		 /* set chunk size =val */
		 ngx_rtmp_set_chunk_size(s, val);
		 break;

	 case NGX_RTMP_MSG_ABORT:
		 /* abort chunk stream =val */
		 break;

	 case NGX_RTMP_MSG_ACK:
		 /* receive ack with sequence number =val */
		 printf("receive ack seq=%d\n", val);
		 break;

	 case NGX_RTMP_MSG_ACK_SIZE:
		 /* receive window size =val */
		 printf("receive ack_size=%u\n", val);
		 s->ack_size = val;
		 break;

	 case NGX_RTMP_MSG_BANDWIDTH:
		 if (b->last - b->pos >= 5) {
			 limit = *(uint8*)&b->pos[4];

			 (void)val;
			 (void)limit;

			 printf("receive bandwidth=%d limit=%d",
				 val, (int)limit);

			 /* receive window size =val
			  * && limit */
		 }
		 break;

	 default:
		 return NGX_ERROR;
 }

 return NGX_OK;
}


#endif
