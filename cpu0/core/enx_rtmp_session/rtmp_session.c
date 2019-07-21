#include "board.h"
#include "dev.h"
#include "rtmp_session.h"

#ifdef RTMP_SUPPORT
#include "ngx_rtmp_config.h"
#include "ngx_rtmp_core.h"
#include "ngx_rtmp_handshake.h"
#include "ngx_rtmp_connect.h"
extern void ngx_rtmp_handshake_recv(ngx_rtmp_session_t *s);

static  u_int nrtmp_user = 0;
static ngx_rtmp_session_t gb_ngx_prtmp_ses[MAX_RTMP_NUM];
static ngx_conf_t gb_ngx_conf[MAX_RTMP_NUM];
ngx_connection_t  gb_ngx_c;
extern UINT s1size; 
void put_h264_streams_rtmp(UINT addr, UINT size, UINT flags, UINT ch, UINT ts)
{//UartPutByte('1');
	int i;

	for(i=0;i<MAX_RTMP_NUM;i++)
	{
		if(rtmp_list[i].valid == 1)
		{			
			//printf("v sbuf_put");
			//UartPutByte('-');
			sbuf_put(&(rtmp_list[i].sbuf[ch]), addr, size, flags, ts);		
		}
	}
}

void put_g7xx_streams_rtmp(UINT addr, UINT size)
{
	int i;

	for(i=0;i<MAX_RTMP_NUM;i++)
	{
		if(rtmp_list[i].valid == 1)
		{			
			//printf("a sbuf_put");
			sbuf_put(&(rtmp_list[i].sbuf_g711), addr, size, 0, 0);	
		}
	}
}


int rtmp_setPort(UINT port)
{printf("%s\n",__func__);
	if(gptMsgShare.RtmpPort == port)
		return 1;
	
	gptMsgShare.RtmpPort = port;
	if(gptMsgShare.RtmpPort == 0)
		gptMsgShare.RtmpPort = RTMP_DEF_PORT;
	return 0;
}



/**
    add rtsp user
*/
rtmp_con_t*
rtmp_add_user(struct tcp_pcb *pcb)
{printf("%s\n",__func__);
    rtmp_session_t *prtmp_ses;
    rtm_session_t *prtm_ses;
    h264_tx_info_t *ph264_info;
    int i=0,idx=0;
	ngx_str_t	*word;
	ngx_rtmp_conf_ctx_t 		*ctx;
	
#if 0//def RTMP_SUPPORT
	BYTE *test = memalign(1, 1);
	free(test);
	test = ngx_create_pool(1);
	if (test != NULL) {
		printf("RTMP_SUPPORT Memcheck A\r\n"); 
		ngx_destroy_pool(test);
	}
#endif

    if (nrtmp_user < MAX_RTMP_NUM)
    {
        /*search rtsp user */
        for(i=0;i<MAX_RTMP_NUM; i++)
        {
            if(rtmp_list[i].valid == 0) break;
        }

        enx_os_memset((u_char*)&rtmp_list[i],0,sizeof(rtmp_con_t));
//#ifdef __AUDIO__				
//        enx_os_memset((u_char*)&baudio[0],0xff,sizeof(baudio));
//#endif

        rtmp_list[i].valid = 1;			/*rtsp_list[i] is marked in used */
        rtmp_list[i].tpcb = pcb;		/*accepted pcb that is used for jpeg and audio transfer */
        rtmp_list[i].jbuf_seq  = 0;		/* send buff index init */
//      rtsp_list[i].num_buf  = 0;
        rtmp_list[i].poll_cnt  = 0;		/* polling cnt */

		sbuf_init(&rtmp_list[i].sbuf[0], NUM_H264_SBUF);
#ifdef __H264_2CH__
		sbuf_init(&rtmp_list[i].sbuf[1], NUM_H264_SBUF);
#endif
#ifdef __AUDIO__
		sbuf_init(&rtmp_list[i].sbuf_g711, NUM_G711_SBUF);
#endif
#ifdef __GPS_STREAM__
		sbuf_init(&rtmp_list[i].sbuf_text, NUM_TEXT_SBUF);
#endif

        //init rtmp session info
        prtmp_ses = (rtmp_session_t *)&(rtmp_list[i].prtmp_ses);
		ngx_memzero(prtmp_ses, sizeof(rtmp_session_t));

        prtmp_ses->session = 0;
		//prtmp_ses->tpcb = pcb;
        prtmp_ses->remote_ipaddr = pcb->remote_ip.addr;
        prtmp_ses->port = pcb->local_port;
		prtmp_ses->video_type = -1;
		prtmp_ses->mediaformat = 96 + i;
		prtmp_ses->login = 0;
		
        //jpeg tx info is init
        ph264_info = &(rtmp_list[i].h264_info);
        ph264_info->ts = 0;
		ph264_info->nSp = 0;

		////////////////////////////ngx information and initial 
		prtmp_ses->ngx_s = (ngx_rtmp_session_t *)&gb_ngx_prtmp_ses;
		ngx_memzero(prtmp_ses->ngx_s, sizeof(ngx_rtmp_session_t));
		prtmp_ses->ngx_s->tpcb = pcb;


		ngx_max_module = 0;
		for (idx = 0; ngx_modules[idx]; idx++) {
			ngx_modules[idx]->index = ngx_max_module++;
		}
		
		//rtmp init conf
		prtmp_ses->ngx_s->conf = (ngx_conf_t *)&gb_ngx_conf;
		ngx_memzero(prtmp_ses->ngx_s->conf, sizeof(ngx_conf_t));


		prtmp_ses->ngx_s->conf->pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE);
		if (prtmp_ses->ngx_s->conf->pool == NULL) {
			dprintf("prtmp_ses->conf->pool == NULL\n"); 
			return NULL;
		}

		prtmp_ses->ngx_s->conf->args = ngx_array_create(prtmp_ses->ngx_s->conf->pool, 2, sizeof(ngx_str_t));
		if (prtmp_ses->ngx_s->conf->args == NULL) {
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
			dprintf("prtmp_ses->conf->args == NULL\n"); 
			return NULL;
		}

		prtmp_ses->ngx_s->conf->ctx = ngx_pcalloc(prtmp_ses->ngx_s->conf->pool, ngx_max_module * sizeof(void *));
		if (prtmp_ses->ngx_s->conf->ctx == NULL) {
			dprintf("prtmp_ses->conf->ctx == NULL\n"); 
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
			return NULL;
		}
		
		
		prtmp_ses->ngx_s->connection = (ngx_connection_t	   *)&gb_ngx_c;
		
		prtmp_ses->ngx_s->connection->pool = ngx_create_pool(NGX_CYCLE_POOL_SIZE);
		if (prtmp_ses->ngx_s->connection->pool == NULL) {
			dprintf("prtmp_ses->ngx_s->connection->pool == NULL\n"); 
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
			return NULL;
		}

		prtmp_ses->ngx_s->conf->module_type = NGX_CORE_MODULE;
		prtmp_ses->ngx_s->conf->cmd_type = NGX_MAIN_CONF;
		
#if 1


		//////////////////////////////////////
		//add 'rtmp' name
		word = ngx_array_push(prtmp_ses->ngx_s->conf->args);
        if (word == NULL) {
			dprintf("word == NULL\n"); 
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
            return NULL;
        }

        word->data = ngx_pnalloc(prtmp_ses->ngx_s->conf->pool, strlen("rtmp"));
        if (word->data == NULL) {
			dprintf("word->data == NULL\n"); 
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
            return NULL;
        }
		strcpy(word->data,"rtmp");
		word->len = strlen("rtmp");

		/////////////////////////////////////
		
		if (ngx_conf_handler(prtmp_ses->ngx_s->conf) != NGX_OK) {
			dprintf("ngx_conf_handler(prtmp_ses->conf) != NGX_OK\n"); 
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
			return NULL;
		}
		////////////////////////////////////////////////
			
		//init session from main, service	
		ctx = prtmp_ses->ngx_s->conf->ctx;	
		prtmp_ses->ngx_s->main_conf = ctx->main_conf;
		prtmp_ses->ngx_s->srv_conf = ctx->srv_conf;
		//ngx_rtmp_core_srv_conf_t       *tsrvcscf	= ctx->srv_conf[0];	
		//printf("tsrvcscf[0]->max_streams %d \n",tsrvcscf->max_streams);
		//ngx_rtmp_core_srv_conf_t       *tsrvcscf2	= ctx->srv_conf[1];	
		//printf("tsrvcscf[1]->max_streams %d \n",tsrvcscf2->max_streams);
		



	
		ngx_rtmp_core_srv_conf_t       *cscf = ngx_rtmp_get_module_srv_conf(prtmp_ses->ngx_s, ngx_rtmp_core_module);
		//memset(cscf,0x00,sizeof(ngx_rtmp_core_srv_conf_t *));
		prtmp_ses->ngx_s->out_queue = cscf->out_queue;
		prtmp_ses->ngx_s->out_cork = cscf->out_cork;

		//cscf->chunk_size =128;

		//printf("cscf->max_streams %d sizeof(ngx_rtmp_stream_t) %d\n",cscf->max_streams, sizeof(ngx_rtmp_stream_t));
		//cscf->max_streams = 1;

		prtmp_ses->ngx_s->in_streams = ngx_pcalloc(prtmp_ses->ngx_s->conf->pool, sizeof(ngx_rtmp_stream_t)* cscf->max_streams);
		if (prtmp_ses->ngx_s->in_streams == NULL) {
			//ngx_rtmp_close_connection(c);
		//	ngx_destroy_pool(prtmp_ses->ngx_s->connection->pool);
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);
			return NULL;
		}			
		prtmp_ses->ngx_s->timeout = cscf->timeout;
		prtmp_ses->ngx_s->buflen = cscf->buflen;
#endif		
		ngx_rtmp_set_chunk_size(prtmp_ses->ngx_s, NGX_RTMP_DEFAULT_CHUNK_SIZE);		
		/////////////////////////////////////////////
		
		prtmp_ses->ngx_s->in_csid = 0;
		prtmp_ses->ngx_s->in_chunk_size = NGX_RTMP_DEFAULT_CHUNK_SIZE;
		prtmp_ses->cur_state = RTMP_HANDSHAKE_STATE;
		
		//prtmp_ses->ngx_s->hs_stage = NGX_RTMP_HANDSHAKE_SERVER_RECV_CHALLENGE;
		
		//if(prtmp_ses->ngx_s->hs_stage == NGX_RTMP_HANDSHAKE_SERVER_RECV_CHALLENGE)
		//prtmp_ses->ngx_s->hs_buf = malloc(sizeof(ngx_rtmp_buf_t));
		//if(prtmp_ses->ngx_s->hs_buf ==NULL)
		//	printf("!!!!!!!!!!!!!!!!!!!!hsbuf null!!!!!!!!!!!!!!\n");
				
		//ngx_rtmp_max_module = 0;
 	    //ngx_rtmp_core_create_srv_conf(prtmp_ses, &ngx_rtmp_core_module);
/////////////////////////////////////////////////////////////////////////////
		prtmp_ses->ngx_s->meta_sent = 0;
		prtmp_ses->ngx_s->start_timestamp = 0;
		prtmp_ses->ngx_s->playstart = 0;
		prtmp_ses->ngx_s->publish_state = 0;
		prtmp_ses->ngx_s->connected = 0;
		prtmp_ses->ngx_s->in_buffer = (u_char*)malloc(sizeof(u_char)*10*1460);
		if(prtmp_ses->ngx_s->in_buffer == NULL)
		{
			printf("ERORR alloc inbuffer !!!!!!!!!!!!!!!!!\n");
			 return (rtsp_con_t*)NULL;
		}
		prtmp_ses->ngx_s->snd_buffer = (u_char*)malloc(sizeof(u_char)*10*1460);
		if(prtmp_ses->ngx_s->snd_buffer == NULL)
		{
			printf("ERORR alloc snd_buffer !!!!!!!!!!!!!!!!!\n");
			 return (rtsp_con_t*)NULL;
		}
		ngx_rtmp_handshake(prtmp_ses->ngx_s);

		////stream video information
		prtmp_ses->ngx_s->video_width = 1920;
		prtmp_ses->ngx_s->video_height = 1080;
		prtmp_ses->ngx_s->video_framrate = 30;

 //       prtm_ses = (rtp_session_t *)&prtmp_ses->prtm_s[0];
//		prtm_ses->start = 0;
		
		prtmp_ses->video_type = RTSP_H264;

//      pjpeg_info->bfirst_ts = 1;
//      pjpeg_info->elapsed_ts = 0;
//      pjpeg_info->seq = prtp_ses->start_seq;
//      pjpeg_info->used = 0;


//		h264_mb.GetPlayTimeLock = 1;

        rtmp_list[i].sequence_num = -1;
//        rtmp_list[i].h264_print_frameNum = h264_print_frame_num;
#ifdef ENX_DEBUG
		dprintf("call : rtmp_add_user : RTMP Connection : current %d -> %d\n", nrtmp_user, nrtmp_user+1);
		dprintf("     : rtmp_add_user :  Local IP Address %u.%u.%u.%u:%u\n", (pcb->local_ip.addr>>24)&0xff,(pcb->local_ip.addr>>16)&0xff,(pcb->local_ip.addr>>8)&0xff,(pcb->local_ip.addr)&0xff, prtmp_ses->port);
		dprintf("     : rtmp_add_user : Remote IP Address %u.%u.%u.%u:%u\n", (prtmp_ses->remote_ipaddr>>24)&0xff,(prtmp_ses->remote_ipaddr>>16)&0xff,(prtmp_ses->remote_ipaddr>>8)&0xff,(prtmp_ses->remote_ipaddr)&0xff, pcb->remote_port);
#else
		dprintf("USER Conn : %u.%u.%u.%u:%u (%d=>%d)\n", (prtmp_ses->remote_ipaddr>>24)&0xff,(prtmp_ses->remote_ipaddr>>16)&0xff,(prtmp_ses->remote_ipaddr>>8)&0xff,(prtmp_ses->remote_ipaddr)&0xff, pcb->remote_port, nrtmp_user, nrtmp_user+1);
#endif
//        eth_netif.ip_addr.addr = pcb->local_ip.addr;
		nrtmp_user++;
        return &rtmp_list[i];
    }

	dprintf("USER Conn Fail\n"); 
    return (rtsp_con_t*)NULL;
}

/**
    remove rtsp user
*/
err_t
rtmp_del_user(struct tcp_pcb *pcb)
{printf("%s\n",__func__);
    rtmp_con_t *prtmp_conn = pcb->callback_arg;
	rtmp_session_t *prtmp_ses = &prtmp_conn->prtmp_ses;
	
	
    if(prtmp_conn)
    {
        if(prtmp_conn->valid)
        {
            prtmp_conn->valid = 0;
            if(nrtmp_user > 0)	nrtmp_user--;
        }
    }
	nrtmp_user=0;
#if 1
	if(prtmp_ses && prtmp_ses->ngx_s)
	{
		prtmp_ses->ngx_s->connected = 0;
		prtmp_ses->ngx_s->playstart = 0;

		ngx_rtmp_core_srv_conf_t       *cscf = ngx_rtmp_get_module_srv_conf(prtmp_ses->ngx_s, ngx_rtmp_core_module);
		//ngx_rtmp_get_module_main_conf 
		//ngx_rtmp_get_module_app_conf

		if(prtmp_ses->ngx_s->conf->pool)
			ngx_destroy_pool(prtmp_ses->ngx_s->conf->pool);

		if(prtmp_ses->ngx_s->connection->pool)
			ngx_destroy_pool(prtmp_ses->ngx_s->connection->pool);

		if(cscf->pool)
		{
			ngx_destroy_pool(cscf->pool);
		}

		if(prtmp_ses->ngx_s->in_buffer)
			free(prtmp_ses->ngx_s->in_buffer);
		if(prtmp_ses->ngx_s->snd_buffer)
			free(prtmp_ses->ngx_s->snd_buffer);

		prtmp_ses->ngx_s = NULL;
	}
	
	if(prtmp_ses)
	{
		dprintf("\nRTMP_H264 close prtmp_ses->video_type %d\n",prtmp_ses->video_type);
		
		//if(prtmp_ses->hs_buf != NULL)
		//	free(prtmp_ses->hs_buf);

		//if(payloadofRTMP != NULL)
		//{
		//	free(payloadofRTMP);
		//	payloadofRTMP = NULL;
		//}
		//if(sendofRTMP != NULL)
		//{
		//	free(sendofRTMP);
		//	sendofRTMP = NULL;
		//}

		//memset(prtmp_ses->ngx_s->in_buffer, 0x00, sizeof(prtmp_ses->ngx_s->in_buffer));
		//prtmp_ses->ngx_s->lenofRTMP = 0;
		
	//	if(pcb->remote_ip.addr!=rtsp_list[0].prtmp_ses.remote_ipaddr)
	//		return;
		//if(prtmp_ses->video_type == RTSP_H264)
		{
			dprintf("\nRTMP_H264 close \n");
			//enx_h264_set_const_bit_rate_mode(IMG_CBR_H, 0);
			//enx_h264_set_const_bit_rate(IMG_CBR_HR, 0);
		}
		
		//if(prtmp_ses->video_type == RTMP_H264PLAYBACK)

	}
#endif
#ifdef ENX_DEBUG
	dprintf("call : rtmp_del_user : RTMP Connection : current %d -> %d\n", nrtmp_user+1, nrtmp_user);
	dprintf("     : rtmp_del_user : Local IP Address %u.%u.%u.%u:%u\n", (pcb->local_ip.addr>>24)&0xff,(pcb->local_ip.addr>>16)&0xff,(pcb->local_ip.addr>>8)&0xff,(pcb->local_ip.addr)&0xff, pcb->local_port);
	dprintf("     : rtmp_del_user : Remote IP Address %u.%u.%u.%u:%u\n", (pcb->remote_ip.addr>>24)&0xff,(pcb->remote_ip.addr>>16)&0xff,(pcb->remote_ip.addr>>8)&0xff,(pcb->remote_ip.addr)&0xff, pcb->remote_port);
#else
	dprintf("USER Disconn : %u.%u.%u.%u:%u (%d=>%d)\n", (pcb->remote_ip.addr>>24)&0xff,(pcb->remote_ip.addr>>16)&0xff,(pcb->remote_ip.addr>>8)&0xff,(pcb->remote_ip.addr)&0xff, pcb->remote_port, nrtmp_user+1, nrtmp_user);
#endif

    return ERR_OK;
}

/**
    close rtsp sesstion
*/
void
rtmp_close(struct tcp_pcb *pcb)
{printf("%s\n",__func__);
/*
	rtmp_con_t *prtmp_conn = (rtmp_con_t *)pcb;
	rtmp_session_t *prtmp_ses;
	
	if(prtmp_conn)
		prtmp_ses = &(prtmp_conn->prtmp_ses);

	if(prtmp_ses)
	{
		dprintf("\nRTMP_H264 close prtmp_ses->video_type %d\n",prtmp_ses->video_type);
		
		//if(prtmp_ses->hs_buf != NULL)
		//	free(prtmp_ses->hs_buf);

		//if(payloadofRTMP != NULL)
		//{
		//	free(payloadofRTMP);
		//	payloadofRTMP = NULL;
		//}
		//if(sendofRTMP != NULL)
		//{
		//	free(sendofRTMP);
		//	sendofRTMP = NULL;
		//}

		//memset(prtmp_ses->ngx_s->in_buffer, 0x00, sizeof(prtmp_ses->ngx_s->in_buffer));
		//prtmp_ses->ngx_s->lenofRTMP = 0;
		
	//	if(pcb->remote_ip.addr!=rtsp_list[0].prtmp_ses.remote_ipaddr)
	//		return;
		//if(prtmp_ses->video_type == RTSP_H264)
		{
			dprintf("\nRTMP_H264 close \n");
			//enx_h264_set_const_bit_rate_mode(IMG_CBR_H, 0);
			//enx_h264_set_const_bit_rate(IMG_CBR_HR, 0);
		}
		
		//if(prtmp_ses->video_type == RTMP_H264PLAYBACK)

	}*/
	if(pcb == NULL)
		return;
	rtmp_del_user(pcb);
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb,NULL);
	tcp_close(pcb);
//	gptMsgCop2Cpu->Connected_ip=0;
}

static void
rtmp_err(void *arg, err_t err)
{printf("%s\n",__func__);
	LWIP_UNUSED_ARG(err);
	rtmp_con_t *prtmp_conn = (rtmp_con_t *)arg;

	if(arg != NULL)
	{
		switch(err)
		{
			case ERR_ABRT:
			case ERR_RST:
			case ERR_CLSD:
			case ERR_CONN:
				dprintf("err\n");
				//prtmp_conn->prtmp_ses.ngx_s->sending = 0;
				rtmp_close(prtmp_conn->tpcb);
				break;
		}
	}
	dprintf("%s(%d) : err(%d)\n", __func__, __LINE__, err);
}

void
rtmp_disconnect(void)
{printf("%s\n",__func__);
    int i = 0;

    /*search rtsp user */
    for(i=0;i<MAX_RTMP_NUM;i++)
    {
        if(rtmp_list[i].valid)
		{
			dprintf("disconnect\n");
			rtmp_close(rtmp_list[i].tpcb);
			break;
       	}
    }
}

void rtmp_all_disconnect(void)
{printf("%s\n",__func__);
	int i;
	for(i=0;i<MAX_RTMP_NUM; i++)
	{
		if(rtmp_list[i].valid)
		{
			printf("alldisconnect rtmp_close call\n");
			rtmp_close(rtmp_list[i].tpcb);
		}
	}
	nrtmp_user = 0;
}


void debug_print_flags(u8_t flags)
{
  if (flags & TCP_FIN) {
    printf(" FIN ");
  }
  if (flags & TCP_SYN) {
    printf("SYN ");
  }
  if (flags & TCP_RST) {
    printf("RST ");
  }
  if (flags & TCP_PSH) {
    printf("PSH ");
  }
  if (flags & TCP_ACK) {
    printf("ACK ");
  }
  if (flags & TCP_URG) {
    printf("URG ");
  }
  if (flags & TCP_ECE) {
    printf("ECE ");
  }
  if (flags & TCP_CWR) {
    printf("CWR ");
  }
  printf("\n");
}


void ngx_rtmp_stream(void *ts)
{
	ngx_rtmp_session_t *s = (ngx_rtmp_session_t *)ts;
	s->btx_st = H264_TX_READY;
	s->meta_sent = 1;
	s->start_timestamp = 0;
	s->header_sent  = 0;
	//s->sending = 0;
	s->header_sent_aud = 0;
	s->playstart = 1;

}

static err_t
rtmp_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{//printf("%s\n",__func__);
    rtmp_con_t *prtmp_conn;
    rtmp_session_t *prtmp_ses;
    int snd_len = 0, teardown=0;
	int i;
	u_char* payload;
	u_char *pRTMP;

    prtmp_conn = arg;
    prtmp_conn->poll_cnt = 0; //reset poll_cnt
	prtmp_ses = &(prtmp_conn->prtmp_ses);
    if (err == ERR_OK && p != NULL) //tcp data is received ...
    {
#if 0//def ENX_DEBUG
		printf("================================================\n");

		debug_print_flags(p->flags);
		//printf("rcv_next %d \n",pcb->rcv_nxt);
		BYTE *tktk = p->payload;
		for(i=0;i<100;i++)
		{
			printf("%x ", (BYTE)tktk[i]);
		}
		for(i=0;i<16;i++)
		{
			printf("%x", (BYTE)tktk[i]);
		}		
		printf("\n");
//		for(i=0;i<16;i++)
//		{
//			printf("%c", (char)tktk[i]);
//		}
		printf("\n================================================\n");
#endif

		payload = p->payload;
		pRTMP = prtmp_ses->ngx_s->in_buffer;//payloadofRTMP;

		if (p->flags & TCP_FIN)
		{		   
		   //DmaMemCpy(pRTMP + lenofRTMP, payload, p->tot_len, NULL);
		   //invalidate_dcache_range(pRTMP + lenofRTMP,pRTMP + lenofRTMP +p->tot_len);
		
		    if(*payload == 0xC3)
		   	{
			   printf("payload has a flag 0xC7 \n");
			   memcpy(pRTMP + prtmp_ses->ngx_s->lenofRTMP, payload+1, p->tot_len-1);
			   prtmp_ses->ngx_s->lenofRTMP += p->tot_len-1;
		   	}
		   	else
		   	{
			   memcpy(pRTMP + prtmp_ses->ngx_s->lenofRTMP, payload, p->tot_len);
			   prtmp_ses->ngx_s->lenofRTMP += p->tot_len;
		    }	
			
			//int cki = 0;
			//for(cki=0; cki<prtmp_ses->ngx_s->lenofRTMP; cki++)
		   	//{
		   	//	if(*(pRTMP + cki) == 0xC4)
	   		//	{
	   		//		printf("%dth\n",cki);	   				
	   		//	}
		   	//}			
		   //printf("payload totlen %d %d\n",p->tot_len,prtmp_ses->ngx_s->lenofRTMP);
		   //printf(" %x %x %x totlen %d \n",(u_char)*(payload+ p->tot_len-3),(u_char)*(payload+ p->tot_len-2),(u_char)*(payload+ p->tot_len-1),lenofRTMP);
		  // printf(" %x %x %x totlen %d \n",*(payloadofRTMP+ lenofRTMP-3),*(payloadofRTMP+ lenofRTMP-2),*(payloadofRTMP+ lenofRTMP-1),lenofRTMP);
		}
		else
		{			
			//DmaMemCpy(pRTMP, payload, p->tot_len, NULL);
			//invalidate_dcache_range(pRTMP, pRTMP + p->tot_len);
			
			memcpy(pRTMP+prtmp_ses->ngx_s->lenofRTMP, payload, p->tot_len);
			prtmp_ses->ngx_s->lenofRTMP += p->tot_len;
			//printf(" %x %x %x totlen %d \n",*(payloadofRTMP),*(payloadofRTMP+1),*(payloadofRTMP+2),lenofRTMP);
			goto continued;
			
		}
		
		
		//printf("handshake: prtmp_ses->cur_state %d\r\n",prtmp_ses->cur_state);
		
		
		//printf("prtmp_ses->cur_state %d p->tot_len %d pos %x last-pos %d %x\n",
		//	prtmp_ses->cur_state, prtmp_ses->ngx_s->lenofRTMP, *prtmp_ses->ngx_s->hs_buf->pos, 
		//	prtmp_ses->ngx_s->hs_buf->last - prtmp_ses->ngx_s->hs_buf->pos,*prtmp_ses->ngx_s->hs_buf->last);

		switch(prtmp_ses->cur_state)
		{
			case RTMP_HANDSHAKE_STATE:
				{
					prtmp_ses->ngx_s->hs_buf->start = prtmp_ses->ngx_s->hs_buf->pos = pRTMP;
					prtmp_ses->ngx_s->hs_buf->end = prtmp_ses->ngx_s->hs_buf->last = prtmp_ses->ngx_s->hs_buf->pos + prtmp_ses->ngx_s->lenofRTMP;
					
					//printf("RTMP_HANDSHAKE_STATE \r\n");
					
					ngx_rtmp_handshake_recv(prtmp_ses->ngx_s);
					if(prtmp_ses->ngx_s->hs_stage == NGX_RTMP_HANDSHAKE_SERVER_DONE)
					{
						prtmp_ses->cur_state = RTMP_COMMAND_STATE;
						if(s1size < prtmp_ses->ngx_s->lenofRTMP)
						{
							prtmp_ses->ngx_s->hs_buf->start = prtmp_ses->ngx_s->hs_buf->pos = pRTMP + (s1size - 1);

							for(i=0;i<prtmp_ses->ngx_s->lenofRTMP-(s1size-1);i++)
							{
								if(*(pRTMP+ ((s1size-1)+i)) == 0xc3)
								{
									memcpy(pRTMP+ ((s1size-1)+i), pRTMP+ ((s1size-1)+i+1),(prtmp_ses->ngx_s->lenofRTMP-(s1size-1))- ( i + 1));
									prtmp_ses->ngx_s->lenofRTMP -= 1;
								}
								//printf("%x ", (BYTE)*(pRTMP+ ((s1size-1)+i)));
							}
							prtmp_ses->ngx_s->hs_buf->end = prtmp_ses->ngx_s->hs_buf->last = prtmp_ses->ngx_s->hs_buf->pos + (prtmp_ses->ngx_s->lenofRTMP-(s1size-1));				
							ngx_rtmp_recv(prtmp_ses->ngx_s);			
						}
					}
					if(prtmp_ses->ngx_s->hs_stage == NGX_RTMP_HANDSHAKE_CLIENT_DONE)
					{
						prtmp_ses->cur_state = RTMP_PUSH_STATE;
						ngx_rtmp_send_push_connect(prtmp_ses->ngx_s);
					}
				}
				break;
			case RTMP_COMMAND_STATE:
				{							
					prtmp_ses->ngx_s->in_csid = 0;
					
					if (prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in == NULL)
					{
						prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in = ngx_rtmp_alloc_in_buf(prtmp_ses->ngx_s);
			            if (prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in == NULL) {
			                printf("in buf NULL\n");
			                ngx_rtmp_finalize_session(prtmp_ses->ngx_s);
			                return;
			            }
					}
					prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->start = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->pos = pRTMP;
					prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->end = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->last = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->pos + prtmp_ses->ngx_s->lenofRTMP;

					if(prtmp_ses->ngx_s->playstart == 1)
						break;
					ngx_rtmp_recv(prtmp_ses->ngx_s);	
					
					//memset(prtmp_ses->ngx_s->in_buffer, 0x00, sizeof(prtmp_ses->ngx_s->in_buffer));
					//memset(prtmp_ses->ngx_s->snd_buffer, 0x00,  sizeof(prtmp_ses->ngx_s->snd_buffer));
					//prtmp_ses->ngx_s->lenofRTMP = 0;						
				}
				break;
			case RTMP_PUSH_STATE:
				{
					prtmp_ses->ngx_s->in_csid = 0;
					if (prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in == NULL)
					{
						prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in = ngx_rtmp_alloc_in_buf(prtmp_ses->ngx_s);
			            if (prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in == NULL) {
			                printf("in buf NULL\n");
			                ngx_rtmp_finalize_session(prtmp_ses->ngx_s);
			                return;
			            }
					}
					prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->start = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->pos = pRTMP;
					prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->end = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->last = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->pos + prtmp_ses->ngx_s->lenofRTMP;
#ifndef TEST_SERVER3
#ifndef TEST_SERVER5
					if(prtmp_ses->ngx_s->hs_stage == NGX_RTMP_HANDSHAKE_CLIENT_DONE)
					{
						prtmp_ses->ngx_s->hs_stage++;
						ngx_rtmp_handshake_done(prtmp_ses->ngx_s);
						
						if(s1size)
						{
							if(s1size < prtmp_ses->ngx_s->lenofRTMP)
							{
							
								prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->start = prtmp_ses->ngx_s->in_streams[prtmp_ses->ngx_s->in_csid].in->buf->pos = pRTMP + (s1size - 1);
								prtmp_ses->ngx_s->lenofRTMP = prtmp_ses->ngx_s->lenofRTMP-(s1size - 1);	
								
								ngx_rtmp_recv(prtmp_ses->ngx_s);
							}
						}
						//return ERR_OK;
						prtmp_ses->ngx_s->lenofRTMP = 0;	
					}
					else
#endif						
#endif						
					{
						
						ngx_rtmp_recv(prtmp_ses->ngx_s);	
						
						if(prtmp_ses->ngx_s->hs_stage == NGX_RTMP_PUSH_CLIENT_DONE)
						{
							 memset(prtmp_ses->ngx_s->in_buffer, 0x00, sizeof(prtmp_ses->ngx_s->in_buffer));
							 //memset(sendofRTMP, 0x00,  sizeof(u_char)*RTMP_PACKETSIZE);
							 prtmp_ses->ngx_s->lenofRTMP = 0;	
							// memset(prtmp_ses->ngx_s->in_buffer, 0x00, sizeof(prtmp_ses->ngx_s->in_buffer));
							// prtmp_ses->ngx_s->lenofRTMP = 0;
							 
							ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(prtmp_ses->ngx_s, ngx_rtmp_core_module);
							cscf->chunk_size = 128;
							 
							prtmp_ses->cur_state = RTMP_PUSH_LIVE;		
#ifdef TEST_SERVER1							
							ngx_rtmp_send_setdataFrame(prtmp_ses->ngx_s);
#endif							
							/*prtmp_ses->ngx_s->btx_st = H264_TX_READY;
							prtmp_ses->ngx_s->meta_sent = 1;
							prtmp_ses->ngx_s->start_timestamp = 0;
							prtmp_ses->ngx_s->header_sent  = 0;
							prtmp_ses->ngx_s->sending = 0;
							prtmp_ses->ngx_s->header_sent_aud = 0;
							prtmp_ses->ngx_s->playstart = 1;*/
							timer_sched_timeout_cb(100,TIMEOUT_ONESHOT, ngx_rtmp_stream, prtmp_ses->ngx_s);
							//prtmp_ses->ngx_s->lenofRTMP = 0;	

						}
					}
				}
				break;
			case RTMP_PUSH_LIVE:
				{	
					//ngx_rtmp_send_setdataFrame(prtmp_ses->ngx_s);
/*							
					prtmp_ses->ngx_s->btx_st = H264_TX_READY;
					prtmp_ses->ngx_s->meta_sent = 1;
					prtmp_ses->ngx_s->start_timestamp = 0;
					prtmp_ses->ngx_s->header_sent  = 0;
					prtmp_ses->ngx_s->sending = 0;
					prtmp_ses->ngx_s->header_sent_aud = 0;
					prtmp_ses->ngx_s->playstart = 1;	*/		
					prtmp_ses->ngx_s->lenofRTMP = 0;
				}
				break;
			default:
				break;
		}
		
		//printf("end recv \n");
		//rtmp_close(pcb);
		
		//dprintf("call : rtmp_recv : teardown\n");		
		//teardown = 2;

continued:
        /* Inform TCP that we have taken the data. */
		tcp_recved(pcb, p->tot_len);

 //       if(snd_len > 0)
 //       {
 //           err = tcp_write(pcb, (const void*)prtmp_ses->in_buffer, snd_len, TCP_WRITE_FLAG_COPY);
 //       }

		if(teardown == 2)
		{
			dprintf("call : rtmp_recv : Close(bye msg)\n");
			rtmp_close(pcb);
		}
		
		tcp_output(pcb);
		
		if(p)
			pbuf_free(p);

		pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
		pcb->flags |= TF_ACK_NOW; //diable delayed ack function
    }
	else
	{
		tcp_output(pcb);
/*		if(p == NULL) //tcp connection is to be closed
		{
			dprintf("call : rtmp_recv : Close(disconnect)\n");
			rtmp_close(pcb);
			return ERR_OK;
		}
		else if(p)
		{
			pbuf_free(p);
		}
*/			
	}
    return ERR_OK;
}

/**
    we can determine h.264 or mjpeg depending on local port of tcp.
*/
static err_t
rtmp_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	rtmp_con_t *prtmp;

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

#if 0 //def UPDATE_ON_DDR
	if(ISP_EI_VLOCK == 0)
	{
		return ERR_MEM;
	}
#endif

	tcp_setprio(pcb, TCP_PRIO_MIN);
//	if((gptMsgCop2Cpu->Connected_ip!=0)&&(gptMsgCop2Cpu->Connected_ip!=pcb->remote_ip.addr))
//	{
//		return ERR_MEM;
//	}
	prtmp = rtmp_add_user(pcb);

	if(prtmp == NULL)
	{
		printf("prtmp == NULL\n");
        rtmp_disconnect();
		return ERR_MEM;
	}
	
//	gptMsgCop2Cpu->Connected_ip=pcb->remote_ip.addr;

	/* Tell TCP that this is the structure we wish to be passed for our callbacks. */
	tcp_arg(pcb, prtmp);

//	tcp_sent(pcb, rtmp_sent);

	/* Tell TCP that we wish to be informed of incoming data by a call
	to the http_recv() function. */
	tcp_recv(pcb, rtmp_recv);
	/* Specifies the polling interval and the callback function that should be
	called to poll the application. The interval is specified in number of
	TCP coarse grained timer shots, which typically occurs twice a second */
//#ifdef __WIFI__
//#if DEF_RTSP_POLL
//	tcp_poll(pcb,rtmp_poll,2); // after 0.5 sec , call rtsp_pll()
//#endif
//#endif
	tcp_err(pcb, rtmp_err);
//	prtmp->pjpeg_info.sendDone = 1;
#if 0//def RTMP_SUPPORT
	BYTE *test = memalign(1, 1);
	free(test);
	test = ngx_create_pool(1);
	if (test != NULL) {
		printf("RTMP_SUPPORT Memcheck B\r\n"); 
		ngx_destroy_pool(test);
	}
#endif

//	h264_mb.endrange = 0;
	dprintf("RTMP ACCEPT\n");
	return ERR_OK;
}

int
rtmp_init(int port)
{	
	int i=0;
	
	rtmp_debug_on = 0;
	rtmp_send_msg = 0;

	//payloadofRTMP = (u_char *)malloc( sizeof(u_char)*RTMP_PACKETSIZE);
	//if(payloadofRTMP == NULL)
	//{printf("%spayloadofRTMP == NULL\n",__func__);
	//	return;
	//}
	//sendofRTMP = (u_char *)malloc(sizeof(u_char)*RTMP_PACKETSIZE);
	
	//memset(payloadofRTMP, 0x00,  sizeof(char)*RTMP_PACKETSIZE);
	//memset(sendofRTMP, 0x00,  sizeof(u_char)*RTMP_PACKETSIZE);
	printf("%s\n",__func__);

	rtmp_setPort(port);

	struct tcp_pcb *loc_rtmp_pcb;

	loc_rtmp_pcb = tcp_new();						//allocate tcp pcb struct

	tcp_bind(loc_rtmp_pcb, IP_ADDR_ANY, gptMsgShare.RtmpPort);	//rtmp port is 1935
	rtmp_pcb_li = tcp_listen(loc_rtmp_pcb);			//listen tcp pcb is registerd
	tcp_accept(rtmp_pcb_li, rtmp_accept);		//wait a new connection

	for(i=0;i<MAX_RTMP_NUM;i++)
	{
		rtmp_list[i].valid = 0;
		//rtmp_list[i].prtmp_ses.cur_state = 0;
		rtmp_list[i].upcb  = NULL;
	}
	
#if 0//def RTMP_SUPPORT
	BYTE *test = memalign(1, 1);
	free(test);
	test = ngx_create_pool(1);
	if (test != NULL) {
		printf("RTMP_SUPPORT Memcheck C\r\n"); 
		ngx_destroy_pool(test);
	}
#endif
	printf("%sE\n",__func__);
	
}

static err_t
rtmp_connected(void *arg, struct tcp_pcb *pcb, err_t err)
{printf("%s\n",__func__);

	LWIP_UNUSED_ARG(arg);
	LWIP_UNUSED_ARG(err);

	rtmp_con_t *prtmp;
	prtmp = rtmp_add_user(pcb);

	if(prtmp == NULL)
	{
		printf("prtmp == NULL\n");
        rtmp_disconnect();
		return ERR_MEM;
	}

	dprintf("RTMP connected\n");
	tcp_arg(pcb, prtmp);
	
	prtmp->prtmp_ses.cur_state = RTMP_HANDSHAKE_STATE;
	
	ngx_rtmp_client_handshake(prtmp->prtmp_ses.ngx_s);
	
	return ERR_OK;
}
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

#include "lwip/tcpip.h"
#include "lwip/netdb.h"
#include "lwip/def.h"

err_t
rtmp_connect(void)
{
	struct tcp_pcb *loc_rtsp_pcb = NULL;
	err_t ert;

	loc_rtsp_pcb = tcp_new();						// allocate tcp pcb struct
	if(loc_rtsp_pcb != NULL)
	{
		ip_addr_t ipaddr;
		
#ifdef TEST_SERVER1		
		IP4_ADDR(&ipaddr,  192,168,2,215); //nginx local server
		strcpy(url_tcUrl, "rtmp://192.168.2.215:1935/live");
		strcpy(url_application, "live");
		strcpy(url_stream_name, "h264");
#endif

#ifdef TEST_SERVER2
		IP4_ADDR(&ipaddr,  74,125,102,231); //google.com sjkim 
		strcpy(url_tcUrl, "rtmp://74.125.102.231:1935/live2");
		strcpy(url_application, "live2");
		strcpy(url_stream_name, "s6a3-hkj1-ar60-3wbr");
#endif

#ifdef TEST_SERVER3
		IP4_ADDR(&ipaddr,  107,155,57,99);	//lecloud.com
		strcpy(url_tcUrl, "rtmp://w.gslb.lecloud.com/live");
		strcpy(url_application, "live");
		strcpy(url_stream_name, "201609023000002xt99");//201605303000000hj99
#endif
#ifdef TEST_SERVER4	
		IP4_ADDR(&ipaddr,  119,6,127,229 );	//testf
		strcpy(url_tcUrl, "rtmp://119.6.127.229:1935/myapp");
		strcpy(url_application, "myapp");
		strcpy(url_stream_name, "test1");
#endif
#ifdef TEST_SERVER5
		IP4_ADDR(&ipaddr,  58,229,157,169);	//lecloud.com
		strcpy(url_tcUrl, "rtmp://publishnetcentertest.bbtv.isplay.com/bbtv");
		strcpy(url_application, "bbtv");
		strcpy(url_stream_name, "bb.qemdtn3r.4ba9236c-b4cb-42e6-b8e6-8eb37e288861");//201605303000000hj99
#endif

		loc_rtsp_pcb->local_port  = 1935;
		ert = tcp_connect(loc_rtsp_pcb, &ipaddr, 1935, rtmp_connected); 
		printf("rtmp_connect tcp_connect ert  %d\n",ert);
		
		/* Tell TCP that we wish to be informed of incoming data by a call
		to the http_recv() function. */
		tcp_recv(loc_rtsp_pcb, rtmp_recv);
		
		/* Specifies the polling interval and the callback function that should be
		called to poll the application. The interval is specified in number of
		TCP coarse grained timer shots, which typically occurs twice a second */
		tcp_err(loc_rtsp_pcb, rtmp_err);
		
	}

}

void ngx_rtmp_play_send(void)
{ 
	if(rtmp_list[0].valid == 0)
	{
		return;
	}
 	rtmp_con_t *prtmp_con;
    rtmp_session_t *prtmp_ses;
    struct tcp_pcb *pcb;
	pcb = tcp_active_pcbs;

	if(pcb != NULL)
		prtmp_con = pcb->callback_arg;
	else
	{		
		return;
	}
	
	while(prtmp_con == NULL)
	{	

		if(pcb == NULL)	
			return;
		
		pcb = pcb->next;
		if(pcb == NULL)
		{			
			return;
		}
		prtmp_con = pcb->callback_arg;

	}

    prtmp_ses = &(prtmp_con->prtmp_ses);
	
	if(prtmp_ses == NULL)
		return;

    ngx_rtmp_session_t     *s = (ngx_rtmp_session_t     *)prtmp_ses->ngx_s;//e->data;

    if(s== NULL)
		return;

	if(s->playstart == 0)
		return;

	//printf("play sending %d\r\n",s->sending);
	//if(s->sending)
	//{
	//	ngx_rtmp_send(s);
	//	return;
	//}
	
	if(s->tpcb->snd_buf < TCP_MSS && tcp_sndqueuelen(s->tpcb) < TCP_SND_QUEUELEN)
	{
		printf("snd_buf %d tcp_sndqueuelen%d\r\n", s->tpcb->snd_buf, tcp_sndqueuelen(s->tpcb));
		return;
	}
	ngx_int_t               rc =NGX_OK;
    ngx_uint_t              ts;

    ts = 0;

	//static UINT testcnt = 0;
	//if(testcnt > 0xFFFFFFFF)
	//	testcnt = 0;
	//if(testcnt++%2 == 0)
	//while(1)
	{
	
		rc = ngx_rtmp_h264_send(s, &ts);
		if(s->tpcb->snd_buf < TCP_MSS && tcp_sndqueuelen(s->tpcb) < TCP_SND_QUEUELEN)
		{
		 	printf("snd_buf %d tcp_sndqueuelen%d\r\n", s->tpcb->snd_buf, tcp_sndqueuelen(s->tpcb));
			return;
		}
#ifdef __AUDIO__
		rc = ngx_rtmp_g711_send(s, &ts);		
		if(s->tpcb->snd_buf < (TCP_MSS/2) && tcp_sndqueuelen(s->tpcb) < (TCP_SND_QUEUELEN/2))
		{
			printf("snd_buf %d tcp_sndqueuelen%d\r\n", s->tpcb->snd_buf, tcp_sndqueuelen(s->tpcb));
			return;
		}
#endif
	}

    return;

                         

    printf("play: send done");

    ngx_rtmp_send_stream_eof(s, NGX_RTMP_MSID);

    ngx_rtmp_send_play_status(s, "NetStream.Play.Complete", "status", ts, 0);

    ngx_rtmp_send_status(s, "NetStream.Play.Stop", "status", "Stopped");
}

#endif

