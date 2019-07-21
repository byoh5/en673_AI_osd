#include "ngx_rtmp_config.h"
#include "ngx_rtmp_send.h"
#include "ngx_rtmp_core.h"
#include "ngx_rtmp_connect.h"
#include "ngx_rtmp_amf.h"
#include "rtmp_session.h"
#include "user.h"

#ifdef RTMP_SUPPORT

BYTE *ngx_rtmp_video_codec = RTMP_CODEC_BASE;
BYTE *ngx_rtmp_audio_codec = RTMP_CODEC_BASE + (RTMP_CODEC_SIZE/2);

BYTE *ngx_rtmp_h264_buffer = RTMP_VIDEO_BASE;
BYTE *ngx_rtmp_g711_buffer = RTMP_AUDIO_BASE;

//static BYTE ngx_rtmp_video_codec[10*1024];
//static BYTE ngx_rtmp_audio_codec[10*1024];

//static BYTE ngx_rtmp_h264_buffer[1024*1024];
//static BYTE ngx_rtmp_g711_buffer[128*1024];

ngx_rtmp_chain_t *
rtmp_create_bandwidth(ngx_rtmp_session_t *s, uint32 ack_size,
                          uint8 limit_type)
{
    printf("create: bandwidth ack_size=%d limit=%d",
                   ack_size, (int)limit_type);

    {
//NGX_RTMP_USER_START(s, RTMP_MSG_BANDWIDTH);
		ngx_rtmp_header_t				__h;
		ngx_rtmp_chain_t					*__l;
		ngx_rtmp_buf_t						*__b;
		ngx_rtmp_core_srv_conf_t	   *__cscf;

		//__cscf = (ngx_rtmp_core_srv_conf_t	   *)ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module); 
		if(s->srv_conf[ngx_rtmp_core_module.ctx_index] == NULL)
			printf("srv_conf == NULL\n");
		else
			__cscf = s->srv_conf[ngx_rtmp_core_module.ctx_index];
		memset(&__h, 0, sizeof(__h));
		__h.type = NGX_RTMP_MSG_BANDWIDTH;
		__h.csid = 2;
		__l = ngx_rtmp_alloc_shared_buf(__cscf);
		if (__l == NULL) {
			return NULL;
		}
		__b = __l->buf;

//NGX_RTMP_USER_OUT4(ack_size);
		*(__b->last++) = ((u_char*)&ack_size)[0];
		*(__b->last++) = ((u_char*)&ack_size)[1];
		*(__b->last++) = ((u_char*)&ack_size)[2];
		*(__b->last++) = ((u_char*)&ack_size)[3];

//NGX_RTMP_USER_OUT1(limit_type);
		*(__b->last++) = ((u_char*)&limit_type)[0];

//NGX_RTMP_USER_END(s);
		ngx_rtmp_prepare_message(s, &__h, NULL, __l);                           \
		return __l;

    }
}

int
ngx_rtmp_send_bandwidth(ngx_rtmp_session_t *s, uint32 ack_size,
                        uint8 limit_type)
{
    return ngx_rtmp_send_shared_packet(s,
           rtmp_create_bandwidth(s, ack_size, limit_type));
}

ngx_rtmp_chain_t *
rtmp_create_chunk_size(ngx_rtmp_session_t *s, uint32 chunk_size)
{
    
    printf("create chunk size %d\r\n",chunk_size);

    {
//NGX_RTMP_USER_START(s, RTMP_MSG_CHUNK_SIZE);
		ngx_rtmp_header_t				__h;
		ngx_rtmp_chain_t					*__l;
		ngx_rtmp_buf_t						*__b;
		ngx_rtmp_core_srv_conf_t	   *__cscf;

		//__cscf = (ngx_rtmp_core_srv_conf_t	   *)ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module); 
		if(s->srv_conf[ngx_rtmp_core_module.ctx_index] == NULL)
			printf("srv_conf == NULL\n");
		else
			__cscf = s->srv_conf[ngx_rtmp_core_module.ctx_index];
		memset(&__h, 0, sizeof(__h));
		__h.type = NGX_RTMP_MSG_CHUNK_SIZE;
		__h.csid = 2;
		__l = ngx_rtmp_alloc_shared_buf(__cscf);
		if (__l == NULL) {
			return NULL;
		}
		__b = __l->buf;

//NGX_RTMP_USER_OUT4(chunk_size);
		*(__b->last++) = ((u_char*)&chunk_size)[0];
		*(__b->last++) = ((u_char*)&chunk_size)[1];
		*(__b->last++) = ((u_char*)&chunk_size)[2];
		*(__b->last++) = ((u_char*)&chunk_size)[3];

//NGX_RTMP_USER_END(s);
	    ngx_rtmp_prepare_message(s, &__h, NULL, __l);
	    return __l;

    }
}

int
ngx_rtmp_send_chunk_size(ngx_rtmp_session_t *s, uint32 chunk_size)
{
    return ngx_rtmp_send_shared_packet(s,
           rtmp_create_chunk_size(s, chunk_size));
}


int
ngx_rtmp_send_amf(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
                  ngx_rtmp_amf_elt_t *elts, size_t nelts)
{
    return ngx_rtmp_send_shared_packet(s,
           ngx_rtmp_create_amf(s, h, elts, nelts));
}


 ngx_int_t
ngx_rtmp_h264_send_meta(ngx_rtmp_session_t *s)
{
//    ngx_rtmp_mp4_ctx_t             *ctx;
    ngx_rtmp_core_srv_conf_t       *cscf;
    ngx_int_t                       rc;
    ngx_uint_t                      n;
    ngx_rtmp_header_t               h;
    ngx_chain_t                    *out;
//   ngx_rtmp_mp4_track_t           *t;
    double                          d;

     struct {
        double                      width;
        double                      height;
        double                      duration;
        double                      framerate;
        double                      video_codec_id;
        double                      audio_codec_id;
        double                      audio_sample_rate;
    }                               v;

    ngx_rtmp_amf_elt_t       out_inf[] = {

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("width"),
          &v.width, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("height"),
          &v.height, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("displayWidth"),
          &v.width, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("displayHeight"),
          &v.height, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("duration"),
          &v.duration, 0 },
			
		
		{ NGX_RTMP_AMF_NUMBER,
		  ngx_string("framerate"),
		  &v.framerate, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("videocodecid"),
          &v.video_codec_id, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("audiocodecid"),
          &v.audio_codec_id, 0 },

        { NGX_RTMP_AMF_NUMBER,
          ngx_string("audiosamplerate"),
          &v.audio_sample_rate, 0 },
			
		{ NGX_RTMP_AMF_STRING,
		  ngx_string("encoder"),
		  "Lavf56.4.101", 0 },

    };

    ngx_rtmp_amf_elt_t       out_elts[] = {

        { NGX_RTMP_AMF_STRING,
          ngx_null_string,
          "onMetaData", 0 },

        { NGX_RTMP_AMF_OBJECT,
          ngx_null_string,
          out_inf, sizeof(out_inf) },
    };

//    ctx = ngx_rtmp_get_module_ctx(s, ngx_rtmp_mp4_module);
//    if (ctx == NULL) {
//        return NGX_OK;
//    }

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    ngx_memzero(&v, sizeof(v));
    v.width  = (double)s->video_width;//ctx->width;
    v.height = (double)s->video_height;//ctx->height;
    v.audio_sample_rate = 8000;//32000;//8000;//ctx->sample_rate;
    v.framerate = 30;

//    t = &ctx->tracks[0];
//    for (n = 0; n < ctx->ntracks; ++n, ++t) {
//        d = ngx_rtmp_mp4_to_rtmp_timestamp(t, t->duration) / 1000.;

//        if (v.duration < d) {
            v.duration = 0;//3000;//d;
//        }

 //       switch (t->type) {
 //           case NGX_RTMP_MSG_AUDIO:
                v.audio_codec_id = NGX_RTMP_AUDIO_G711U;//NGX_RTMP_AUDIO_AAC;// NGX_RTMP_AUDIO_G711U;//t->codec;
  //              break;
  //          case NGX_RTMP_MSG_VIDEO:
                v.video_codec_id = NGX_RTMP_VIDEO_H264;//t->codec;
  //              break;
 //       }
 //   }

    out = NULL;
    rc = ngx_rtmp_append_amf(s, &out, NULL, out_elts,
                             sizeof(out_elts) / sizeof(out_elts[0]));
    if (rc != NGX_OK || out == NULL) {
        return NGX_ERROR;
    }

    ngx_memzero(&h, sizeof(h));

    h.csid = NGX_RTMP_CSID_AMF;
    h.msid = NGX_RTMP_MSID;
    h.type = NGX_RTMP_MSG_AMF_META;

    ngx_rtmp_prepare_message(s, &h, NULL, out);
    rc = ngx_rtmp_send_message(s, out, 0);
    ngx_rtmp_free_shared_chain(cscf, out);

    return rc;
}

ngx_chain_t *
ngx_rtmp_append_shared_bufs(ngx_rtmp_core_srv_conf_t *cscf,
        ngx_chain_t *head, ngx_chain_t *in)
{//printf("%s\n",__func__);
    ngx_chain_t                    *l, **ll;
    u_char                         *p;
    size_t                          size;

    ll = &head;
    p = in->buf->pos;
    l = head;

    if (l) {
        for(; l->next; l = l->next);
        ll = &l->next;
    }

    for ( ;; ) {

        if (l == NULL || l->buf->last == l->buf->end) {
            l = ngx_rtmp_alloc_shared_buf(cscf);
            if (l == NULL || l->buf == NULL) {
                break;
            }

            *ll = l;
            ll = &l->next;
        }

        while (l->buf->end - l->buf->last >= in->buf->last - p) {
            l->buf->last = ngx_cpymem(l->buf->last, p,
                    in->buf->last - p);
            in = in->next;
            if (in == NULL) {
                goto done;
            }
            p = in->buf->pos;
        }

        size = l->buf->end - l->buf->last;
        l->buf->last = ngx_cpymem(l->buf->last, p, size);
        p += size;
    }

done:
    *ll = NULL;

    return head;
}

 ngx_int_t
ngx_rtmp_h264_send(ngx_rtmp_session_t *s, ngx_uint_t *ts)
{
    ngx_buf_t                       in_buf;
    ngx_rtmp_header_t               h, lh;
    ngx_rtmp_core_srv_conf_t       *cscf;
    ngx_chain_t                    *out, in;
    uint32                        buflen, end_timestamp,
                                    timestamp, last_timestamp, rdelay,
                                    cur_timestamp;
    ssize_t                         ret;
    u_char                          fhdr[5];
    size_t                          fhdr_size;
    ngx_int_t                       rc;
    ngx_uint_t                      n;
	
	uint32 SPS_NUM;
	uint32 PPS_NUM;
	
	fhdr_size =0;

		
	sbuf_t *getsbuf = &(rtmp_list[0].sbuf[0]);
	h264_tx_info_t* ph264_info = &rtmp_list[0].h264_info;
	
	//printf("h264_send\r\n");
	
	//if(s->tpcb->snd_buf < 100*1024){
	//	printf("100*1024 NGX_AGAIN\r\n");
	//	return NGX_AGAIN;
	//}
	
	if(s->sending != 0)	
	{
		printf("ngx_rtmp_h264_send NGX_BUSY\r\n");
                ngx_rtmp_send(s);
		return NGX_BUSY;
	}
	
    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
	
    if (!s->meta_sent) {
        rc = ngx_rtmp_h264_send_meta(s);

        if (rc == NGX_OK) {
            s->meta_sent = 1;
        }
		printf("meta_sent\r\n");

        return rc;
    }


	int nret;
	nret = getH264data(s, ph264_info, getsbuf);
	if(nret<0)
	{
//		printf("getH264data nret %d", nret);
		return NGX_AGAIN;
	}
	//else
	//	printf("h264 size %d ",ph264_info->nsize);
	
    buflen = s->buflen + ph264_info->nsize;
	
	last_timestamp = 0;
	end_timestamp = s->start_timestamp + ph264_info->ts;
					/*(timer_get_ms() - s->epoch) + buflen*/;

	if(nret == RTMP_ERROR)
	{
		return RTMP_AGAIN;
	}
	else if(nret == NGX_AGAIN)
	{
		printf("getH264data RTMP_AGAIN\r\n");
			
	        //ngx_rtmp_send(s);
	
		return NGX_AGAIN;
	}


    timestamp = 0;

	ngx_memzero(&h, sizeof(h));

    h.msid = NGX_RTMP_MSID;
    h.type = 0x09;//(uint8) t->type;
    h.csid = 0x04;//t->csid;

    lh = h;

    ngx_memzero(&in, sizeof(in));
    ngx_memzero(&in_buf, sizeof(in_buf));

	if(ph264_info->nsize == 0)
	{
		printf("ph264_info->nsize %d\r\n",ph264_info->nsize);
		return NGX_AGAIN;
	}
	
	// NAL Start Get			
	// base image address에서 write할 위치 선정
	uint8* un8DataPoint = ph264_info->base;
	uint32 un32FtchNum = 0;
	
	// NAL 관련 변수
	uint32 un32NALTest = 0;
	uint32 un32NALzeroCount = 0;
	
	// NAL start인지 검사
	uint32 i=0;
	
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
	
	un32FtchNum = ph264_info->nsize - un32NALzeroCount;
	//printf("un32NALTest %d un32FtchNum %d ph264_info->nsize %d un32NALzeroCount %d\r\n",un32NALTest, un32FtchNum, ph264_info->nsize, un32NALzeroCount);
	
	// pjpeg_info->base는 항상 NAL단위로 start(0x61, 0x67, 0x68)
	if(un32NALTest)
	{
		// NAL식별자(0x00 0x00 0x00 0x01 or 0x00 0x00 0x01)는 전송하지 않는다.
		un8DataPoint += un32NALzeroCount;		// nbyte 이동

		// Get : Nal Unit Type
		uint32 nNalUnitType = *un8DataPoint & 0x1F;
		uint32 k = 0;
		switch(nNalUnitType)
		{
			case 0x1:
				//printf("P\r\n");
				break;
			case 0x7:	// SPS의 length는 frame size에 따라 가변적일 수 있다
				// 다음 NAL 위치를 검색하여 SPS의 크기를 정한다.
				k = 0;
				for(i=0;i<un32FtchNum;i++)
				{ 
					if(*(un8DataPoint+i) == 0x00)
					{
						k++;
						if(*(un8DataPoint+i+1) == 0x01 && k >= 2)	// NAL Start는 0x00이 2개 이상이다.
						{
							SPS_NUM = i + 1 - k;								
							un8DataPoint += SPS_NUM + 4;
							//printf("sps number %d \r\n",SPS_NUM);
							break;
						}
					}
					else
					{
						k = 0;
					}
				}
				
				// PPS
				// 다음 NAL 위치를 검색하여 PPS의 크기를 정한다.
				for(i=0;i<un32FtchNum;i++)
				{
					if(*(un8DataPoint+i) == 0x00)
					{
						k++;
						if(*(un8DataPoint+i+1) == 0x01 && k >= 2)	// NAL Start는 0x00이 2개 이상이다.
						{
							PPS_NUM = i + 1 - k;		
							un8DataPoint += PPS_NUM + 4;
							//printf("pps number %d \r\n",PPS_NUM);
							break;
						}
					}
					else
					{
						k = 0;
					}
				}
				
				if(*un8DataPoint & 0x1F == 0x5)
					printf("I %d\r\n",ph264_info->iframe);
				
				break;
			default:
				printf("no sps pps \r\n");
				return;
		}
	}
	else
	{
		printf("it's not h264 data\r\n");
		if(s->btx_st == H264_TX_END)
		{	// 마지막 데이터까지 패킷으로 다 만들었으면 현 상태를 종료한다
			s->btx_st = H264_TX_READY;
			ph264_info->nSp = 0;
			return NGX_AGAIN;
		}		
	}
	
	//static uint32 past_pps = 0,past_sps = 0;
	
    //if (!s->header_sent || ((ph264_info->iframe) && (past_pps!=PPS_NUM||past_sps!=SPS_NUM))) {
    //if ((ph264_info->iframe)) {
	if (!s->header_sent) {

		//printf("codec %x h264 %x\r\n",ngx_rtmp_video_codec,ngx_rtmp_h264_buffer);	
		//printf("codec %x h264 %x\r\n",RTMP_CODEC_BASE,RTMP_VIDEO_BASE);
			
        fhdr[0] = 0x07;
        fhdr[1] = 0;

        fhdr[0] |= 0x10;
        fhdr[2] = fhdr[3] = fhdr[4] = 0;
        fhdr_size = 5;

		printf("ph264_info->iframe %d\r\n",ph264_info->iframe);
		printf("[%x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x %x]\r\n"
		,*(ph264_info->base+0),*(ph264_info->base+1),*(ph264_info->base+2),*(ph264_info->base+3)
		,*(ph264_info->base+4),*(ph264_info->base+5),*(ph264_info->base+6),*(ph264_info->base+7)
		,*(ph264_info->base+8),*(ph264_info->base+9),*(ph264_info->base+10),*(ph264_info->base+11)
		,*(ph264_info->base+12),*(ph264_info->base+13),*(ph264_info->base+14),*(ph264_info->base+15)
		,*(ph264_info->base+16),*(ph264_info->base+17),*(ph264_info->base+18),*(ph264_info->base+19)
		,*(ph264_info->base+20),*(ph264_info->base+21),*(ph264_info->base+22),*(ph264_info->base+23)
		,*(ph264_info->base+24),*(ph264_info->base+25),*(ph264_info->base+26),*(ph264_info->base+27)
		,*(ph264_info->base+28),*(ph264_info->base+29));

		if(s->video_width == 1920)
		{
#if 0
			BYTE codec_info[] = {
				0x01 ,//configuration version
				0x42 ,//0x42 ,//AVC profile indication Baseline 0x42(66), Main 0x4d(77), Extended 0x58(88) ...	
				0xC0 ,//0x00 ,//profile compatibility
				0x2A ,//0x15 ,//0x1E ,//AVC level Indication
				0xFF ,//0x03 ,//LengthSizeMinusOne
				0xE1 ,//0x01 ,// numOfSequenceParameterSets  ,	SPS number
				0x00 ,0x1A ,//0x00 ,0x0B ,//sequenceParameterSetLength,   SPS length of 2 bytes ex)11
				0x67 ,0x42 ,0xc0 ,0x2a ,0xda ,0x01 ,0xe0 ,0x08 ,0xbf ,0x88 ,0xc0 ,0x44 ,0x00 ,0x00 ,0x03 ,0x00 ,0x04 ,0x00 ,0x00 ,0x03 ,0x00 ,0xca ,0x3c ,0x60 ,0xca ,0x80,//0x67 ,0x4d ,0x00 ,0x29 ,0x95 ,0xA0 ,0x1E ,0x00 ,0x89 ,0x79 ,0x50 ,//sps 11byte
				0x01 ,//numOfPictureParameterSets
				0x00 ,0x04 ,// pictureParameterSetLength , PPS lenth ex)5
				0x68, 0xce, 0x3c, 0x80//0x68 ,0xEE ,0x0d ,0x83, 0x30  //PPS
				};
#else
			BYTE codec_info[] = {
				0x01 ,//configuration version
				0x4d ,//0x42 ,//AVC profile indication Baseline 0x42(66), Main 0x4d(77), Extended 0x58(88) ...	
				0x00 ,//0x00 ,//profile compatibility
				0x29 ,//0x15 ,//0x1E ,//AVC level Indication
				0xFF ,//0x03 ,//LengthSizeMinusOne
				0xE1 ,//0x01 ,// numOfSequenceParameterSets  ,	SPS number
				0x00 ,0x0B ,//sequenceParameterSetLength,   SPS length of 2 bytes ex)11
				0x67 ,0x4d ,0x00 ,0x29 ,0x95 ,0xA0 ,0x1E ,0x00 ,0x89 ,0x79 ,0x50 ,//sps 11byte
				0x01 ,//numOfPictureParameterSets
				0x00 ,0x05 ,// pictureParameterSetLength , PPS lenth ex)5
				0x68, 0xce, 0x01, 0x26, 0x0c//0x68 ,0xEE ,0x0d ,0x83, 0x30  //PPS
				};


#endif
			ngx_memcpy(codec_info + 1, (ph264_info->base + 5), 3);
			codec_info[8-1] = SPS_NUM;					
			printf("SPS_NUM %d\r\n",PPS_NUM);
			codec_info[(8+SPS_NUM+3)-1] = PPS_NUM;				
			printf("PPS_NUM %d\r\n",PPS_NUM);
			ngx_memcpy(codec_info + 8, (ph264_info->base + 4), SPS_NUM);
			ngx_memcpy(codec_info +(8+SPS_NUM+3), (ph264_info->base + (4+SPS_NUM+4)), PPS_NUM);
			
			printf("header %x %x %x %x %x \r\n",*(ph264_info->base + (4+SPS_NUM+4)),	*(ph264_info->base + (4+SPS_NUM+4+1)),*(ph264_info->base + (4+SPS_NUM+4+2)),*(ph264_info->base + (4+SPS_NUM+4+3)),	*(ph264_info->base + (4+SPS_NUM+4+4)));

			ngx_memcpy(ngx_rtmp_video_codec , fhdr, fhdr_size); 		
			
			ngx_memcpy(ngx_rtmp_video_codec + fhdr_size, (BYTE*)codec_info, (8+SPS_NUM+3+PPS_NUM));

			
			in.buf = &in_buf;
			in_buf.pos	= ngx_rtmp_video_codec;
			in_buf.last = ngx_rtmp_video_codec + fhdr_size + (8+SPS_NUM+3+PPS_NUM);
	
		}
		else if(s->video_width == 1280)
		{
			BYTE codec_info[] = {
				0x01 ,//configuration version
				0x42 ,//AVC profile indication Baseline 0x42(66), Main 0x4d(77), Extended 0x58(88) ...  
				0xC0 ,//0x00 ,//profile compatibility
				0x28 ,//0x15 ,//0x1E ,//AVC level Indication
				0xFF ,//0x03 ,//LengthSizeMinusOne
				0xE1 ,//0x01 ,// numOfSequenceParameterSets  ,  SPS number
				0x00 ,0x0A ,//sequenceParameterSetLength,   SPS length of 2 bytes ex)11
				0x67, 0x42, 0x00, 0x29, 0x95, 0xA0, 0x14, 0x01, 0x6C, 0x40,
				0x01 ,//numOfPictureParameterSets
				0x00 ,0x04 ,// pictureParameterSetLength , PPS lenth ex)5
				0x68 ,0xCE ,0x1e ,0x0c, 0x00  //PPS
            	};
			
			codec_info[8-1] = SPS_NUM;		
			codec_info[(8+SPS_NUM+3)-1] = PPS_NUM;				

			memcpy(codec_info + 8, (ph264_info->base + (4)), SPS_NUM);
			codec_info[(8+SPS_NUM+3)-1] = PPS_NUM;				
			memcpy(codec_info +(8+SPS_NUM+3), (ph264_info->base + (4+SPS_NUM+4)), PPS_NUM);
			printf("header %x %x %x %x %x \n",*(ph264_info->base + (4+SPS_NUM+4)),
				*(ph264_info->base + (4+SPS_NUM+4+1)),
				*(ph264_info->base + (4+SPS_NUM+4+2)),
				*(ph264_info->base + (4+SPS_NUM+4+3)),
				*(ph264_info->base + (4+SPS_NUM+4+4)));
	
			ngx_memcpy(ngx_rtmp_video_codec , fhdr, fhdr_size);	
	
			ngx_memcpy(ngx_rtmp_video_codec + fhdr_size, (BYTE*)codec_info,  (8+SPS_NUM+3+PPS_NUM));
			
			in.buf = &in_buf;
			in_buf.pos	= ngx_rtmp_video_codec;
			in_buf.last = ngx_rtmp_video_codec +  (8+SPS_NUM+3+PPS_NUM) + fhdr_size;
		}
		else if(s->video_width == 640)
		{
			BYTE codec_info[] = {
				0x01 ,//configuration version
				0x42 ,//AVC profile indication Baseline 0x42(66), Main 0x4d(77), Extended 0x58(88) ...  
				0xC0 ,//0x00 ,//profile compatibility
				0x28 ,//0x15 ,//0x1E ,//AVC level Indication
				0xFF ,//0x03 ,//LengthSizeMinusOne
				0xE1 ,//0x01 ,// numOfSequenceParameterSets  ,  SPS number
				0x00 ,0x09 ,//sequenceParameterSetLength,   SPS length of 2 bytes ex)11
				0x67, 0x4f, 0x00, 0x29, 0x95, 0xA0, 0x28, 0x0f, 0x44,
				0x01 ,//numOfPictureParameterSets
				0x00 ,0x04 ,// pictureParameterSetLength , PPS lenth ex)5
				0x68 ,0xCE ,0x0f ,0x83, 0x00  //PPS
            	};

			memcpy(codec_info + 8, (ph264_info->base + (4)), SPS_NUM);
			codec_info[(8+SPS_NUM+3)-1] = PPS_NUM;				
			memcpy(codec_info +(8+SPS_NUM+3), (ph264_info->base + (4+SPS_NUM+4)), PPS_NUM);
			printf("header %x %x %x %x %x \n",*(ph264_info->base + (4+SPS_NUM+4)),
				*(ph264_info->base + (4+SPS_NUM+4+1)),
				*(ph264_info->base + (4+SPS_NUM+4+2)),
				*(ph264_info->base + (4+SPS_NUM+4+3)),
				*(ph264_info->base + (4+SPS_NUM+4+4)));
			
			ngx_memcpy(ngx_rtmp_video_codec , fhdr, fhdr_size);	
	
			ngx_memcpy(ngx_rtmp_video_codec + fhdr_size, (BYTE*)codec_info, (8+SPS_NUM+3+PPS_NUM));
			
			in.buf = &in_buf;
			in_buf.pos	= ngx_rtmp_video_codec;
			in_buf.last = ngx_rtmp_video_codec + (8+SPS_NUM+3+PPS_NUM) + fhdr_size;
			
		}
		
		out = ngx_rtmp_append_shared_bufs(cscf, NULL, &in);
		ngx_rtmp_prepare_message(s, &h, NULL, out);
		rc = ngx_rtmp_send_message(s, out, 0);
		
		ngx_rtmp_free_shared_chain(cscf, out);
		//printf("header_sent\r\n");

        //if (rc == NGX_AGAIN) {
		//	printf("header_sent RTMP_AGAIN\r\n");
        //    return NGX_AGAIN;
        //}
		
//		past_sps = SPS_NUM;
//		past_pps = PPS_NUM;
		//printf("past_sps %d past_pps %d \r\n", past_sps, past_pps);
        s->header_sent = 1;
/*		
		if(s->btx_st == H264_TX_END)
		{	// 마지막 데이터까지 패킷으로 다 만들었으면 현 상태를 종료한다
			s->btx_st = H264_TX_READY;
			ph264_info->nSp = 0;
			//printf("send NGX_OK\r\n");
			return NGX_OK;
		}		
		return NGX_OK;*/
    }

	cur_timestamp = ph264_info->ts;
	timestamp = cur_timestamp;
		
    if (timestamp > end_timestamp) {
        printf("h264: ahead %ldD > %ldD",
               timestamp, end_timestamp);

        if (ts) {
            *ts = last_timestamp;
        }

        return (uint32) (timestamp - end_timestamp);
    }		
	
	last_timestamp = s->last_timestamp;
		
    h.timestamp  = timestamp;
    lh.timestamp = last_timestamp;

	ngx_rtmp_h264_buffer +=fhdr_size + ph264_info->nsize + 32;
	//printf("buf length %d \r\n",in_buf.last - in_buf.pos);
	//printf("ngx_rtmp_h264_buffer %x < %x RTMP_VIDEO_END %x RTMP_VIDEO_BASE %x\r\n",ngx_rtmp_h264_buffer ,ngx_rtmp_h264_buffer + (100*1024), RTMP_VIDEO_END, RTMP_VIDEO_BASE);
	if((INT*)ngx_rtmp_h264_buffer + (100*1024) > RTMP_VIDEO_END)
	{
	//	printf("!!ngx_rtmp_h264_buffer %x RTMP_VIDEO_END %x RTMP_VIDEO_BASE %x\r\n",ngx_rtmp_h264_buffer , RTMP_VIDEO_END, RTMP_VIDEO_BASE);
		ngx_rtmp_h264_buffer = RTMP_VIDEO_BASE;
	}

	ngx_rtmp_h264_buffer += 8;
	
    fhdr_size = 5+4;

	memset(ngx_rtmp_h264_buffer, 0x00, fhdr_size + ph264_info->nsize);

	*(ngx_rtmp_h264_buffer+0) = 0x07;

	if (ph264_info->iframe) {
		*(ngx_rtmp_h264_buffer+0) |= 0x10;					
	}
	else {
		*(ngx_rtmp_h264_buffer+0) |= 0x20;//0x30;
	}


    rdelay = ph264_info->ts - last_timestamp;

    *(ngx_rtmp_h264_buffer+1) = 1; //AVC NALU
    *(ngx_rtmp_h264_buffer+2) = 0x00;//(rdelay >> 16) & 0xff;//0x00;//
    *(ngx_rtmp_h264_buffer+3) = 0x00;//(rdelay >> 8)  & 0xff;//0x00;//
    *(ngx_rtmp_h264_buffer+4) = 0x00;//rdelay & 0xff;//0x00;//0x4d 0x4f;//
    
	if (ph264_info->iframe) 
	{ 
		uint32 length;

		//length = ph264_info->nsize - (4 + SPS_NUM + 4 + PPS_NUM + 4) ;  
		length = SPS_NUM; 

		u_char *pp = (u_char*)&length;
		*(ngx_rtmp_h264_buffer+5) = pp[0];
		*(ngx_rtmp_h264_buffer+6) = pp[1];
		*(ngx_rtmp_h264_buffer+7) = pp[2];
		*(ngx_rtmp_h264_buffer+8) = pp[3];
	}
	else
	{
	    uint32 length = ph264_info->nsize - 4;  
		u_char *pp = (u_char*)&length;
		*(ngx_rtmp_h264_buffer+5) = pp[0];
		*(ngx_rtmp_h264_buffer+6) = pp[1];
		*(ngx_rtmp_h264_buffer+7) = pp[2];
		*(ngx_rtmp_h264_buffer+8) = pp[3];
	}
      	       
	s->current_time = timestamp;
	
	if (ph264_info->nsize + fhdr_size > RTMP_VIDEO_SIZE) {
		printf("h264 too big frame: %D>%uz\n",
					   ph264_info->nsize + fhdr_size , RTMP_VIDEO_SIZE);
		return NGX_AGAIN;//goto next;
	}
	
	//if((INT*)ngx_rtmp_h264_buffer + fhdr_size + ph264_info->nsize > RTMP_VIDEO_END)
	//{
	//	printf("!!ngx_rtmp_h264_buffer %x RTMP_VIDEO_END %x RTMP_VIDEO_BASE %x\r\n",ngx_rtmp_h264_buffer , RTMP_VIDEO_END, RTMP_VIDEO_BASE);
	//	ngx_rtmp_h264_buffer = RTMP_VIDEO_BASE;
	//	testloopmax = testloopcnt;
	//	testloopcnt = 0;
	//	testloop = 1;
	//	printf("testloopmax %d\r\n",testloopmax);
	//	return NGX_AGAIN;
	//}
	
	if (ph264_info->iframe) {
		
		//printf("i %x %x %x %x %x\r\n",*((BYTE*)ph264_info->base), *((BYTE*)ph264_info->base+1), *((BYTE*)ph264_info->base+2), *((BYTE*)ph264_info->base+3), *((BYTE*)ph264_info->base+4));
		//printf("---%x\r\n",*(ph264_info->base + (4 + SPS_NUM + 4 + PPS_NUM + 3)));
		
		ngx_memcpy(ngx_rtmp_h264_buffer + fhdr_size, (ph264_info->base + 4), SPS_NUM);
		
		uint32 length = PPS_NUM; 

		u_char *pp = (u_char*)&length;
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM)+0) = pp[0];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM)+1) = pp[1];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM)+2) = pp[2];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM)+3) = pp[3];
		ngx_memcpy(ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4, (ph264_info->base + (4+SPS_NUM+4)), PPS_NUM);

		length = ph264_info->nsize - (4 + SPS_NUM + 4 + PPS_NUM + 4) ;  		

		pp = (u_char*)&length;
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+0) = pp[0];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+1) = pp[1];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+2) = pp[2];
		*((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+3) = pp[3];

		DmaMemCpy_ip(((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+4),  ph264_info->base + (4 + SPS_NUM + 4 + PPS_NUM + 4), ph264_info->nsize - (4 + SPS_NUM + 4 + PPS_NUM + 4));
		invalidate_dcache_range(((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+4), ((ngx_rtmp_h264_buffer + fhdr_size + SPS_NUM + 4 + PPS_NUM)+4) + ( ph264_info->nsize - (4 + SPS_NUM + 4 + PPS_NUM + 4)));
		//memcpy((BYTE *)(ngx_rtmp_h264_buffer + fhdr_size), (BYTE *)(ph264_info->base + (4 + SPS_NUM + 4 + PPS_NUM + 4)), ph264_info->nsize - (4 + SPS_NUM + 4 + PPS_NUM + 4));

		in.buf = &in_buf;
		in_buf.pos	= ngx_rtmp_h264_buffer;

		in_buf.last = ngx_rtmp_h264_buffer + ph264_info->nsize + fhdr_size + SPS_NUM + 4 + PPS_NUM+4;

		
		//int testi;		
		//for(testi=0; testi<(10);testi++)
		//	printf("%x ",*(ph264_info->base+(4 + SPS_NUM + 4 + PPS_NUM + 4)+testi));	
		//printf("\r\n");
		//for(testi=0; testi<(20);testi++)
		//	printf("%x ",*(ngx_rtmp_h264_buffer+testi));
	}
	else
	{
		//printf("p %x %x %x %x %x\r\n",*((BYTE*)ph264_info->base), *((BYTE*)ph264_info->base+1), *((BYTE*)ph264_info->base+2), *((BYTE*)ph264_info->base+3), *((BYTE*)ph264_info->base+4));
		//printf("p %x %x %x %x %x\r\n",*((BYTE*)ph264_info->base+5), *((BYTE*)ph264_info->base+6), *((BYTE*)ph264_info->base+7), *((BYTE*)ph264_info->base+8), *((BYTE*)ph264_info->base+9));
		DmaMemCpy_ip(ngx_rtmp_h264_buffer + fhdr_size, ph264_info->base + 4, ph264_info->nsize - 4);
		invalidate_dcache_range(ngx_rtmp_h264_buffer + fhdr_size, ngx_rtmp_h264_buffer + fhdr_size + (ph264_info->nsize - 4));
		//memcpy((BYTE *)(ngx_rtmp_h264_buffer + fhdr_size), (BYTE *)(ph264_info->base + 4), ph264_info->nsize - 4 );

		in.buf = &in_buf;
		in_buf.pos	= ngx_rtmp_h264_buffer;

		in_buf.last = ngx_rtmp_h264_buffer + fhdr_size + (ph264_info->nsize - 4);

		//int testi,count=0;		
		//for(testi=0; testi<(100);testi++)
		//{
		//	if(ph264_info->base+testi == 0xff)
		//		count ++;
		//	if(count>5)
		//	printf("count%d\r\n",count);
		//}
		//int testi;		
		//for(testi=0; testi<(100);testi++)
		//	printf("%x ",*(ph264_info->base+testi));
		
		//printf("========\r\n");
		//int testi;
		//for(testi=0; testi<(20);testi++)
		//	printf("%x ",*(ngx_rtmp_h264_buffer+testi));
	}
	//printf("========\r\n");
	//printf("buf size %d\r\n",in_buf.last - in_buf.pos);
	
    out = ngx_rtmp_append_shared_bufs(cscf, NULL, &in);

    if(ph264_info->bFirst)
        ngx_rtmp_prepare_message(s, &h, &lh, out);
//	ngx_rtmp_prepare_message(s, &h, NULL, out);
	
	//printf("prepare+size %d\r\n",in_buf.last - in_buf.pos);
    rc = ngx_rtmp_send_message(s, out, 0);
    ngx_rtmp_free_shared_chain(cscf, out);
	
    if (rc == NGX_AGAIN) 
	{
		printf("NGX_AGAIN\n");
        return NGX_AGAIN;
    }

	ph264_info->sendFlag = 1;
	
	s->current_time = timestamp;
	s->last_timestamp = timestamp;	

	if(s->btx_st == H264_TX_DOING)
	{
		if(ph264_info->nsize - ph264_info->nSp == 0)
			s->btx_st = H264_TX_END;
		ph264_info->bFirst = 0;
	}
	if(s->btx_st == H264_TX_END)
	{	// 마지막 데이터까지 패킷으로 다 만들었으면 현 상태를 종료한다
		s->btx_st = H264_TX_READY;
		ph264_info->nSp = 0;
		//printf("send NGX_OK\r\n");
		return NGX_OK;
	}		
   
}

ngx_int_t
ngx_rtmp_g711_send(ngx_rtmp_session_t *s, ngx_uint_t *ts)
{
    ngx_buf_t                       in_buf;
    ngx_rtmp_header_t               h, lh;
    ngx_rtmp_core_srv_conf_t       *cscf;
    ngx_chain_t                    *out, in;
    uint32                        buflen, end_timestamp,
                                    timestamp, last_timestamp, rdelay,
                                    cur_timestamp;
    ssize_t                         ret;
    u_char                          fhdr[5];
    size_t                          fhdr_size;
    ngx_int_t                       rc;
    ngx_uint_t                      n;
	fhdr_size =0;
	
	sbuf_t *getsbuf = &(rtmp_list[0].sbuf[0]);
	h264_tx_info_t* ph264_info = &rtmp_list[0].h264_info;
	
	//if(s->tpcb->snd_buf < 1*512)
	//{
	//	printf("%s s->tpcb->snd_buf %d return NGX_AGAIN!!\n",__func__, s->tpcb->snd_buf);
	//	return NGX_AGAIN;
	//}
	
	if(s->sending != 0)
	{
		ngx_rtmp_send(s);
		//printf("ngx_rtmp_audio_send NGX_BUSY\r\n");
		//timer_delay(10);
		return NGX_BUSY;
	}
    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    timestamp = 0;

    ngx_memzero(&h, sizeof(h));

    h.msid = NGX_RTMP_MSID;
    h.type = 0x08;//(uint8) t->type;
    h.csid = 0x04;//t->csid;

    lh = h;


    ngx_memzero(&in, sizeof(in));
    ngx_memzero(&in_buf, sizeof(in_buf));


	
    if (!s->header_sent_aud) {

        fhdr[0] = 0x82;//0xaf;//t->fhdr;
        fhdr[1] = 0;

        if (0){//t->type == NGX_RTMP_MSG_VIDEO) {
            fhdr[0] |= 0x10;
            fhdr[2] = fhdr[3] = fhdr[4] = 0;
            fhdr_size = 5;
        } else {
            fhdr_size = 2;
        }

		BYTE testdata[] = {
			0x12 ,//configuration version
			0x90 
        	};
		
		ngx_memcpy(ngx_rtmp_audio_codec , fhdr, fhdr_size);	

		ngx_memcpy(ngx_rtmp_audio_codec + fhdr_size, (BYTE*)testdata, sizeof(testdata));
		
		in.buf = &in_buf;
		in_buf.pos	= ngx_rtmp_audio_codec;
		in_buf.last = ngx_rtmp_audio_codec + sizeof(testdata) + fhdr_size;
		
		
		out = ngx_rtmp_append_shared_bufs(cscf, NULL, &in);
		ngx_rtmp_prepare_message(s, &h, NULL, out);
		//ngx_rtmp_prepare_message(s, &h, ph264_info->bFirst ? &lh :NULL, out);
		rc = ngx_rtmp_send_message(s, out, 0);
		//printf("%s ngx_rtmp_send_message rc %d\n",__func__, rc);
		ngx_rtmp_free_shared_chain(cscf, out);

        if (rc == NGX_AGAIN) {
            return NGX_AGAIN;
        }

        s->header_sent_aud = 1;

		return NGX_OK;
    }

#if 0 //AAC AUDIO CODEC	
	BYTE testdata2[] = {
	    //0x44 //format 1/ chunk stream ID 4
	    //,0x00, 0x00, 0x00//time stamp
		//,0x00, 0x01, 0x77//Body size
		//,0x08//Audio Data Type ID
		0xaf//HE-AAC 44 Khz 16bit stereo
		,0x01, 0x00, 0xfe, 0x33, 0xae, 0xd4, 0x38, 0x62, 0x88, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xdc, 0x13
		,0xfa, 0xe3, 0xd3, 0xc9, 0xfc, 0x68, 0xf7, 0x3e, 0x3c, 0xe7, 0x64, 0xa3, 0x60, 0x48, 0xf8, 0x4b
		,0x3c, 0x43, 0xc1, 0x88, 0x21, 0x7b, 0x85, 0x10, 0x1e, 0x04, 0x95, 0x1d, 0xc1, 0x0a, 0x39, 0xd2
		,0x19, 0xfa, 0xa4, 0xb5, 0x98, 0x0c, 0x9f, 0x66, 0xdd, 0xb0, 0x42, 0x0d, 0xdf, 0x2a, 0xf1, 0x52
		,0x15, 0xdf, 0x45, 0xd7, 0x22, 0xb8, 0x04, 0x9e, 0x6e, 0x39, 0x62, 0xc7, 0x8c, 0x42, 0x2c, 0x32
		,0x58, 0xe4, 0x11, 0x9e, 0xaa, 0xe6, 0xb9, 0x1b, 0x30, 0xf9, 0x4c, 0x96, 0x0e, 0x11, 0x28, 0xa6
		,0xcc, 0xb8, 0x29, 0x73, 0x81, 0x1a, 0x80, 0x4f, 0x41, 0xa2, 0xdf, 0x15, 0xa2, 0xf2, 0x08, 0x01
		,0x05, 0xa3, 0xe9, 0x9b, 0xf6, 0xa0, 0x34, 0xac, 0x5e, 0xa9, 0xb4, 0xcf, 0x9c, 0x9f, 0x7c, 0x16
		,0x58, 0x1f, 0xca, 0x6c, 0x1f, 0x84, 0xfb, 0x97, 0x7a, 0x6e, 0x3e, 0xad, 0xf5, 0xe5, 0x7d, 0x15
		,0x38, 0xf9, 0xcb, 0x0f, 0xab, 0xeb, 0x00, 0xed, 0x6f, 0x1b, 0x24, 0x13, 0x7c, 0x53, 0x6a, 0x45
		,0xbc, 0xac, 0x1d, 0xc7, 0xcd, 0x3a, 0xaa, 0x98, 0xee, 0x19, 0x18, 0xff, 0x69, 0xe1, 0xbb, 0x55
		,0x73, 0x40, 0xb9, 0xf5, 0xcf, 0x59, 0x1d, 0xff, 0x05, 0x49, 0xb4, 0x2b, 0x6e, 0xaa, 0x57, 0x91
		,0x6e, 0x45, 0x03, 0xa4, 0x5b, 0xf3, 0xfd, 0x02, 0xc4, 0xa6, 0x35, 0x9b, 0x7b, 0xb2, 0xde, 0xef
		,0xe6, 0xf7, 0x75, 0x4b, 0xa8, 0x69, 0x10, 0x77, 0x7f, 0x83, 0x61, 0xe5, 0xe8, 0x99, 0xdb, 0xd3
		,0x6c, 0x7a, 0x0c, 0x8b, 0xf1, 0x41, 0x7a, 0xce, 0x33, 0x30, 0x5a, 0x2f, 0x61, 0xb1, 0x2e, 0xa7
		,0xa0, 0xe4, 0x91, 0xd9, 0x2b, 0x1a, 0x7d, 0xa6, 0x4e, 0x83, 0x6c, 0x85, 0x49, 0x1d, 0x0b, 0xfc
		,0x78, 0x5b, 0xa7, 0x71, 0xb2, 0x49, 0x3e, 0xa5, 0x35, 0xb3, 0x70, 0xda, 0x15, 0x35, 0x18, 0x73
		,0xee, 0xd9, 0x60, 0x9c, 0x15, 0x11, 0xee, 0x7a, 0xd3, 0xf9, 0x37, 0x5c, 0x48, 0xd8, 0xaa, 0x4b
		,0x25, 0x5a, 0x1a, 0x36, 0xab, 0x7c, 0x02, 0x4d, 0xf4, 0x20, 0x43, 0x60, 0xc3, 0x08, 0x52, 0x66
		,0xc8, 0x0d, 0x11, 0x6e, 0xdf, 0x72, 0xcb, 0x43, 0xe4, 0xe6, 0xef, 0x23, 0x34, 0x69, 0x35, 0x61
		,0x3e, 0xd1, 0x55, 0x7a, 0x7b, 0xa8, 0x90, 0xce, 0xa4, 0x68, 0x2d, 0x84, 0x47, 0x1b, 0xf7, 0xa5
		,0x21, 0x6a, 0xf3, 0x78, 0x68, 0x10, 0x6c, 0x75, 0xce, 0xac, 0x42, 0xc6, 0x73, 0x33, 0x2a, 0x69
		,0xb3, 0x76, 0x63, 0x23, 0x82, 0x8f, 0x0e, 0xcf, 0xeb, 0x1d, 0x5d, 0xd3, 0x34, 0x03, 0xb7, 0xa6
		,0x52, 0xcc, 0xee, 0xb7, 0x49, 0x57																			   
	};
#endif

	BYTE testdata2[1024];
	testdata2[0] = 0x82;	
	
	UINT g711_addr;
	UINT g711size;
	UINT g711_flag, g711_temp;
	
	if(sbuf_get(&(rtmp_list[0].sbuf_g711), &g711_addr, &g711size, &g711_flag, &g711_temp) != DEF_OK)
	{
		return NGX_AGAIN;
	}
	//printf("g711 size %d\r\n",g711size);
	//memcpy((BYTE *)testdata2+1, (BYTE *)g711_addr, g711size);
	DmaMemCpy_ip((BYTE *)testdata2+1, (BYTE *)g711_addr, g711size);
	invalidate_dcache_range(testdata2+1, testdata2+1 + g711size);

	DmaMemCpy_ip(ngx_rtmp_g711_buffer, testdata2 , g711size + 1);
	invalidate_dcache_range(ngx_rtmp_g711_buffer, ngx_rtmp_g711_buffer + g711size + 1);

	in.buf = &in_buf;
	in_buf.pos	= ngx_rtmp_g711_buffer;
	in_buf.last = ngx_rtmp_g711_buffer + g711size + 1 ;
	ngx_rtmp_g711_buffer += (in_buf.last - in_buf.pos) + 32;
	if((INT*)ngx_rtmp_g711_buffer + (512) > RTMP_AUDIO_END)
	{
	//	//printf("!!ngx_rtmp_g711_buffer %x RTMP_AUDIO_END %x RTMP_AUDIO_BASE %x\r\n",ngx_rtmp_g711_buffer , RTMP_AUDIO_END, RTMP_AUDIO_BASE);
		ngx_rtmp_g711_buffer = RTMP_AUDIO_BASE;
	}
	//printf("buf size %d\r\n",in_buf.last - in_buf.pos);
    out = ngx_rtmp_append_shared_bufs(cscf, NULL, &in);
	ngx_rtmp_prepare_message(s, &h, &lh, out);

    rc = ngx_rtmp_send_message(s, out, 0);
    ngx_rtmp_free_shared_chain(cscf, out);
	
    if (rc == NGX_AGAIN) {
        return NGX_AGAIN;
    }
	return NGX_OK;
}
 
 void
 ngx_rtmp_free_shared_chain(ngx_rtmp_core_srv_conf_t *cscf, ngx_rtmp_chain_t *in)
 {
	 ngx_rtmp_chain_t		 *cl;
	 //printf("%s\n",__func__);
	 if (ngx_rtmp_ref_put(in)) {
		 //printf("ngx_rtmp_ref_put return\n");
		 return;
	 }
 
	 for (cl = in; ; cl = cl->next) {
		 if (cl->next == NULL) {
			 	cl->next = cscf->free;
			 	cscf->free = in;
			 
			 return;
		 }
	 }
 }
 
 char*
 ngx_rtmp_message_type(uint8 type)
 {
	 static char*	 types[] = {
		 "?",
		 "chunk_size",
		 "abort",
		 "ack",
		 "user",
		 "ack_size",
		 "bandwidth",
		 "edge",
		 "audio",
		 "video",
		 "?",
		 "?",
		 "?",
		 "?",
		 "?",
		 "amf3_meta",
		 "amf3_shared",
		 "amf3_cmd",
		 "amf_meta",
		 "amf_shared",
		 "amf_cmd",
		 "?",
		 "aggregate"
	 };
 
	 return type < sizeof(types) / sizeof(types[0])
		 ? types[type]
		 : "?";
 }
 
 
 void
 ngx_rtmp_prepare_message(ngx_rtmp_session_t *s, ngx_rtmp_header_t *h,
		 ngx_rtmp_header_t *lh, ngx_rtmp_chain_t *out)
 {//printf("%s\n",__func__);
	 ngx_rtmp_chain_t				 *l;
	 u_char 					*p, *pp;
	 int				   hsize, thsize, nbufs;
	 uint32 				   mlen, timestamp, ext_timestamp;
	 static uint8			   hdrsize[] = { 12, 8, 4, 1 };
	 u_char 					 th[7];
	 ngx_rtmp_core_srv_conf_t	*cscf;
	 uint8					   fmt;
 //    rtmp_connection_t		   *c;
 
  //   c = s->connection;
	 cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
 
	 if (h->csid >= (uint32)cscf->max_streams) {
		 printf("RTMP out chunk stream too big: %ld >= %ld",
				 h->csid, cscf->max_streams);
		 rtmp_close(s);
		 return;
	 }
 
	 /* detect packet size */
	 mlen = 0;
	 nbufs = 0;
	 for(l = out; l; l = l->next) {
		 mlen += (l->buf->last - l->buf->pos);
		 ++nbufs;
	 }
 
	 fmt = 0;
	 if (lh && lh->csid && h->msid == lh->msid) {
		 ++fmt;
		 if (h->type == lh->type && mlen && mlen == lh->mlen) {
			 ++fmt;
			 if (h->timestamp == lh->timestamp) {
				 ++fmt;
			 }
		 }
		// printf("h->type %d,lh->type %d,mlen %d,lh->mlen %d",h->type,lh->type,mlen ,lh->mlen);
		 //printf("%s h->timestamp %d lh->timestamp %d\n",__func__,h->timestamp, lh->timestamp);
		 timestamp = h->timestamp - lh->timestamp;
		 
	 } else {
	 
		 //printf("%s h->timestamp %d\n",__func__,h->timestamp);
		 timestamp = h->timestamp;
	 }
 
	 if (lh) {
		 *lh = *h;
		 lh->mlen = mlen;
	 }
 
	 hsize = hdrsize[fmt];
 
	 //printf("RTMP prep %s (%d) fmt=%d csid=%ld timestamp=%ld "
	 // 	   "mlen=%ld msid=%ld nbufs=%d\n",
	 // 	   ngx_rtmp_message_type(h->type), (int)h->type, (int)fmt,
	 // 	   h->csid, timestamp, mlen, h->msid, nbufs);
 
	 ext_timestamp = 0;
	 if (timestamp >= 0x00ffffff) {
		 ext_timestamp = timestamp;
		 timestamp = 0x00ffffff;
		 hsize += 4;
	 }
 
	 if (h->csid >= 64) {
		 ++hsize;
		 if (h->csid >= 320) {
			 ++hsize;
		 }
	 }
 
	 /* fill initial header */
	 out->buf->pos -= hsize;
	 p = out->buf->pos;
 
	 /* basic header */
	 *p = (fmt << 6);
	 if (h->csid >= 2 && h->csid <= 63) {
		 *p++ |= (((uint8)h->csid) & 0x3f);
	 } else if (h->csid >= 64 && h->csid < 320) {
		 ++p;
		 *p++ = (uint8)(h->csid - 64);
	 } else {
		 *p++ |= 1;
		 *p++ = (uint8)(h->csid - 64);
		 *p++ = (uint8)((h->csid - 64) >> 8);
	 }
 
	 /* create fmt3 header for successive fragments */
	 thsize = p - out->buf->pos;
	 ngx_memcpy(th, out->buf->pos, thsize);
	 th[0] |= 0xc0;
	 //printf("%s timestamp %d\n",__func__,timestamp);
	 /* message header */
	 if (fmt <= 2) {
		 pp = (u_char*)&timestamp;
		 *p++ = pp[1];
		 *p++ = pp[2];
		 *p++ = pp[3];
		 if (fmt <= 1) {
			 pp = (u_char*)&mlen;
			 *p++ = pp[1];
			 *p++ = pp[2];
			 *p++ = pp[3];
			 *p++ = h->type;
			 if (fmt == 0) {
				 pp = (u_char*)&h->msid;
				 *p++ = pp[3];
				 *p++ = pp[2];
				 *p++ = pp[1];
				 *p++ = pp[0];
			 }
		 }
	 }
 
	 /* extended header */
	 if (ext_timestamp) {
		 pp = (u_char*)&ext_timestamp;
		 *p++ = pp[0];
		 *p++ = pp[1];
		 *p++ = pp[2];
		 *p++ = pp[3];
 
		 /* This CONTRADICTS the standard
		  * but that's the way flash client
		  * wants data to be encoded;
		  * ffmpeg complains */
		 if (cscf->play_time_fix) {
			 ngx_memcpy(&th[thsize], p - 4, 4);
			 thsize += 4;
		 }
	 }
 
	 /* append headers to successive fragments */
	 for(out = out->next; out; out = out->next) {
		 out->buf->pos -= thsize;
		 ngx_memcpy(out->buf->pos, th, thsize);
	 }
 }

#endif
