#include "ngx_rtmp_handshake.h"
#include "ngx_rtmp_core.h"
#include "ffmpeg_sha256.h"

#ifdef RTMP_SUPPORT

/////////////////////////////////////////////////////////////////////////

/* RTMP handshake :
 *
 *          =peer1=                      =peer2=
 * challenge ----> (.....[digest1]......) ----> 1537 bytes
 * response  <---- (...........[digest2]) <---- 1536 bytes
 *
 *
 * - both packets contain random bytes except for digests
 * - digest1 position is calculated on random packet bytes
 * - digest2 is always at the end of the packet
 *
 * digest1: HMAC_SHA256(packet, peer1_partial_key)
 * digest2: HMAC_SHA256(packet, HMAC_SHA256(digest1, peer2_full_key))
 */


/* Handshake keys */
static u_char
ngx_rtmp_server_key[] = {
    'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
    'F', 'l', 'a', 's', 'h', ' ', 'M', 'e', 'd', 'i', 'a', ' ',
    'S', 'e', 'r', 'v', 'e', 'r', ' ',
    '0', '0', '1',

    0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1,
    0x02, 0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
    0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
};


static u_char
ngx_rtmp_client_key[] = {
    'G', 'e', 'n', 'u', 'i', 'n', 'e', ' ', 'A', 'd', 'o', 'b', 'e', ' ',
    'F', 'l', 'a', 's', 'h', ' ', 'P', 'l', 'a', 'y', 'e', 'r', ' ',
    '0', '0', '1',

    0xF0, 0xEE, 0xC2, 0x4A, 0x80, 0x68, 0xBE, 0xE8, 0x2E, 0x00, 0xD0, 0xD1,
    0x02, 0x9E, 0x7E, 0x57, 0x6E, 0xEC, 0x5D, 0x2D, 0x29, 0x80, 0x6F, 0xAB,
    0x93, 0xB8, 0xE6, 0x36, 0xCF, 0xEB, 0x31, 0xAE
};


static const u_char
ngx_rtmp_server_version[4] = {
    0x0D, 0x0E, 0x0A, 0x0D
};


static const u_char
ngx_rtmp_client_version[4] = {
    0x0C, 0x00, 0x0D, 0x0E
};

static ngx_str_t            ngx_rtmp_server_full_key
    = { sizeof(ngx_rtmp_server_key), ngx_rtmp_server_key };
static ngx_str_t            ngx_rtmp_server_partial_key
    = { 36, ngx_rtmp_server_key };

static ngx_str_t            ngx_rtmp_client_full_key
    = { sizeof(ngx_rtmp_client_key), ngx_rtmp_client_key };
static ngx_str_t            ngx_rtmp_client_partial_key
    = { 30, ngx_rtmp_client_key };
 
UINT s1size = 0;


 void rtmp_calc_digest(const uint8 *src, int len, int gap,
                             const uint8 *key, int keylen, uint8 *dst)
{printf("%s 1\n",__func__);
    struct AVSHA *sha;
    uint8 hmac_buf[64+32] = {0};
    int i;

    sha = malloc(sizeof(AVSHA));
	printf("%s 2\n",__func__);

    if (keylen < 64) {
        memcpy(hmac_buf, key, keylen);
    } else {
        av_sha_init(sha, 256);
        av_sha_update(sha,key, keylen);
        av_sha_final(sha, hmac_buf);
    }
    for (i = 0; i < 64; i++)
        hmac_buf[i] ^= IPAD;
	printf("%s 3\n",__func__);

    av_sha_init(sha, 256);
	printf("%s av_sha_init\n",__func__);
    av_sha_update(sha, hmac_buf, 64);
	printf("%s av_sha_update\n",__func__);
    if (gap <= 0) {
        av_sha_update(sha, src, len);
    } else { //skip 32 bytes used for storing digest
		//printf("\n src %x %x %x %x len %d\n",*(src+0), *(src+1), *(src+2), *(src+3),gap);
        av_sha_update(sha, src, gap);
		//printf("\n src + gap + 32 %x %x %x %x len %d\n",*(src + gap +32+0), *(src + gap +32+1), *(src + gap +32+2), *(src + gap +32+3),len - gap - 32);
        av_sha_update(sha, src + gap + 32, len - gap - 32);
    }
    av_sha_final(sha, hmac_buf + 64);
	printf("%s 4\n",__func__);

    for (i = 0; i < 64; i++)
        hmac_buf[i] ^= IPAD ^ OPAD; //reuse XORed key for opad
    av_sha_init(sha, 256);
    av_sha_update(sha, hmac_buf, 64+32);
    av_sha_final(sha, dst);
	printf("%s 5\n",__func__);

    free(sha);
	printf("%s 6\n",__func__);
}
 
 int
 ngx_rtmp_make_digest(ngx_str_t *key1, ngx_rtmp_buf_t *src1,
		 u_char *skip1, u_char *dst1)
 {
	 printf("%s\n",__func__);
 #if 0
	u_char *testch;
	int gap, len;
	printf("%s (src1->last - src1->pos) %d\n",__func__, (src1->last - src1->pos));
	testch = (u_char *)malloc(sizeof(u_char)*(src1->last - src1->pos));
	//memset(testch, 0x00, sizeof(u_char)*(src1->last - src1->pos));
	printf("TEST MEMCPY len %d \n",src1->last - src1->pos);
	memcpy(testch, (u_char *)src1->pos , (src1->last - src1->pos));
	printf("TEST MEMCPY OK\n");
	
	u_char* srcpp = testch;//src1->pos;
	
	len = src1->last - src1->pos;

	if(skip1 == NULL)
		gap = 0;
	else
		gap = skip1 - src1->pos;
	
    uint8 *key = key1->data;
    int keylen = key1->len;
    uint8 *dst = dst1;

	struct AVSHA sha;
	uint8 *hmac_buf = NULL;//[64+32] = {0};
	hmac_buf = (uint8*)malloc(sizeof(uint8)*(64+32));
	memset(hmac_buf, 0x00, sizeof(uint8)*(64+32));
	int i;

	//sha = (AVSHA *)malloc(sizeof(AVSHA));
	printf("%s 2\n",__func__);

	if (keylen < 64) {
		memcpy(hmac_buf, key, keylen);
	} else {
		av_sha_init(&sha, 256);
		av_sha_update(&sha,key, keylen);
		av_sha_final(&sha, hmac_buf);
	}
	for (i = 0; i < 64; i++)
		hmac_buf[i] ^= IPAD;
	//printf("%s 3\n",__func__);

	av_sha_init(&sha, 256);
	//printf("%s av_sha_init\n",__func__);
	av_sha_update(&sha, hmac_buf, 64);
	printf("%s av_sha_update gap %d\n",__func__, gap);
	if (gap <= 0) {
		//printf("gap 0 !!\n");
		av_sha_update(&sha, srcpp, len);
	} else { //skip 32 bytes used for storing digest
	    //printf("gap %d !!\n",gap);
		//printf("\n src %x %x %x %x len %d\n",*(src+0), *(src+1), *(src+2), *(src+3),gap);
		//printf("%s av_sha_update 11 %d \n",__func__, gap);
		av_sha_update(&sha, srcpp, gap);
		//printf("\n src + gap + 32 %x %x %x %x len %d\n",*(src + gap +32+0), *(src + gap +32+1), *(src + gap +32+2), *(src + gap +32+3),len - gap - 32);
		//printf("%s av_sha_update 12\n",__func__);
		av_sha_update(&sha, srcpp + gap + 32, len - gap - 32);
	}
	av_sha_final(&sha, hmac_buf + 64);
	//printf("%s 4\n",__func__);

	for (i = 0; i < 64; i++)
		hmac_buf[i] ^= IPAD ^ OPAD; //reuse XORed key for opad
	printf("%s 5-1\n",__func__);
	av_sha_init(&sha, 256);
	//printf("%s 5-2\n",__func__);
	av_sha_update(&sha, hmac_buf, 64+32);
	//printf("%s 5-3\n",__func__);
	av_sha_final(&sha, dst);
	//printf("%s 5-4\n",__func__);

	//free(sha);
	free(testch);
	free(hmac_buf);
	printf("%s 6\n",__func__);

#else
	 u_char *testch;
	 u_char *hmac_buf;//[64+32] = {0,};
	 u_char* srcpp;
	 int gap, len;	 
	 int i;
	 u_char *key;
	 int keylen;
	 u_char *dst;
	 struct AVSHA sha;
	 
	 //printf("%s (src1->last - src1->pos) %d\n",__func__, (src1->last - src1->pos));
	 testch = (u_char *)malloc(sizeof(u_char)*(src1->last - src1->pos));
	 memset(testch, 0x00, sizeof(u_char)*(src1->last - src1->pos));
	 //printf("TEST MEMCPY len %d \n",src1->last - src1->pos);
	 memcpy(testch, src1->pos , sizeof(u_char)*(src1->last - src1->pos));
	 //printf("TEST MEMCPY OK\n");
	 
	 srcpp = testch;//src1->pos;
	 
	 len = src1->last - src1->pos;
 
	 if(skip1 == NULL)
		 gap = 0;
	 else
		 gap = skip1 - src1->pos;
	 
	 key = key1->data;
	 keylen = key1->len;
	 dst = dst1;
 
	 
	 //memset(hmac_buf, 0x00, sizeof(hmac_buf));
	 hmac_buf = (u_char*)malloc(sizeof(u_char)*(64+32));
	 memset(hmac_buf, 0x00, sizeof(u_char)*(64+32));
 
 
	 //sha = (AVSHA *)malloc(sizeof(AVSHA));
	 //printf("%s 2\n",__func__);
 
	 if (keylen < 64) {
		 memcpy(hmac_buf, key, keylen);
	 } else {
		 av_sha_init(&sha, 256);
		 av_sha_update(&sha,key, keylen);
		 av_sha_final(&sha, hmac_buf);
	 }
	 for (i = 0; i < 64; i++)
		 hmac_buf[i] ^= IPAD;
	 //printf("%s 3\n",__func__);
 
	 av_sha_init(&sha, 256);
	 //printf("%s av_sha_init\n",__func__);
	 av_sha_update(&sha, hmac_buf, 64);
	 //printf("%s av_sha_update gap %d\n",__func__, gap);
	 if (gap <= 0) {
		 //printf("gap 0 !!\n");
		 av_sha_update(&sha, srcpp, len);
	 } else { //skip 32 bytes used for storing digest
		 //printf("gap %d !!\n",gap);
		 //printf("\n src %x %x %x %x len %d\n",*(src+0), *(src+1), *(src+2), *(src+3),gap);
		 //printf("%s av_sha_update 11 %d \n",__func__, gap);
		 av_sha_update(&sha, srcpp, gap);
		 //printf("\n src + gap + 32 %x %x %x %x len %d\n",*(src + gap +32+0), *(src + gap +32+1), *(src + gap +32+2), *(src + gap +32+3),len - gap - 32);
		 //printf("%s av_sha_update 12\n",__func__);
		 av_sha_update(&sha, srcpp + gap + 32, len - gap - 32);
	 }
	 av_sha_final(&sha, hmac_buf + 64);
	 //printf("%s 4\n",__func__);
 
	 for (i = 0; i < 64; i++)
		 hmac_buf[i] ^= IPAD ^ OPAD; //reuse XORed key for opad
	 //printf("%s 5-1\n",__func__);
	 av_sha_init(&sha, 256);
	 //printf("%s 5-2\n",__func__);
	 av_sha_update(&sha, hmac_buf, 64+32);
	 //printf("%s 5-3\n",__func__);
	 av_sha_final(&sha, dst);
	 //printf("%s 5-4\n",__func__);
 
	 //free(sha);
	 free(testch);
	 free(hmac_buf);
	 //printf("%s 6\n",__func__);
 
 #endif
	 return RTMP_OK;
 }
 
 
 int
 ngx_rtmp_find_digest(ngx_rtmp_buf_t *b, ngx_str_t *key, size_t base)
 {printf("%s\n",__func__);
	 size_t 				 n, offs,digest_pos;
	 u_char 				 digest[NGX_RTMP_HANDSHAKE_KEYLEN] = {0x00,};
	 u_char 				*p;
	 int ti;
	 
	 offs = 0;
	 digest_pos =0;
	 
	 for (n = 0; n < 4; ++n) {
		 offs += b->pos[base + n];
		 printf("%x %d offs %d\n",b->pos[base + n],b->pos[base + n],offs);
	 }
	 
	 offs = (offs % 728) + base + 4;
	 p = b->pos + offs;
	 
	 //printf("offs %d\n",offs);
	 
	 for (n  = 0; n  < 4; n ++)
		 digest_pos += b->pos[n  + base];
	 
	 digest_pos = (digest_pos % 728) + base + 4;
	 
	 //printf("digest_pos %d\n",digest_pos);
 
	 //printf("key %s\n",key->data);
	 //printf("%s b->pos %x %x %x %x %x %x \n",__func__,*(b->pos+0),*(b->pos+1),*(b->pos+2),*(b->pos+3),*(b->pos+4),*(b->pos+5));
	 //printf("%s p %x %x %x %x %x %x \n",__func__,*(p+0),*(p+1),*(p+2),*(p+3),*(p+4),*(p+5));
	 //b->pos+=8;
	 //p = b->pos + digest_pos;
	 //printf("%s dgst %x %x %x %x %x %x \n",__func__,*(p+0),*(p+1),*(p+2),*(p+3),*(p+4),*(p+5));
 
#if 1
     if (ngx_rtmp_make_digest(key, b, p, digest) != RTMP_OK) {
  	  return RTMP_ERROR;
     }
#else
	 rtmp_calc_digest(b->pos, b->last - b->pos, offs, key->data, key->len, digest);
#endif    	
 
	 //printf("%s server made digest\n",__func__);
	 //for(ti=0;  ti<NGX_RTMP_HANDSHAKE_KEYLEN; ti++)
	 //{
	   //printf("%x ",digest[ti]);
	 //}
	// printf("%s client digest\n",__func__);
	 //for(ti=0;  ti<NGX_RTMP_HANDSHAKE_KEYLEN; ti++)
	 //{
	 //  printf("%x ",*(p+ti));
	 //}
 
     if (memcmp(digest, p, NGX_RTMP_HANDSHAKE_KEYLEN) == 0) {
  	 	printf("%s digest memcmp ok offs %d \n",__func__,offs);
  	   return offs;
     }
	 
	 //printf("%s RTMP_ERROR\n",__func__);
 
	 return RTMP_ERROR;
 }
 
 
  int
 rtmp_write_digest(ngx_rtmp_buf_t *b, ngx_str_t *key, size_t base)
 {//printf("%s\n",__func__);
	 size_t 				 n, offs;
	 u_char 				*p;
	 u_char 				 digest2[NGX_RTMP_HANDSHAKE_KEYLEN] = {0x00,};
 
	 offs = 0;
	 for (n = 8; n < 12; ++n) {
		 offs += b->pos[base + n];
		 printf("%x %d offs %d\n",b->pos[base + n],b->pos[base + n],offs);
	 }
	 offs = (offs % 728) + base + 12;
	 p = b->pos + offs;
 
	 if (ngx_rtmp_make_digest(key, b, p, digest2) != RTMP_OK) {
		 return RTMP_ERROR;
	 }
	 memcpy(b->pos + offs, digest2, NGX_RTMP_HANDSHAKE_KEYLEN);
	 
	 return RTMP_OK;
 }
 //static int rtmp_validate_digest(uint8 *buf, int off);
 
 
 /*
 static int
 rtmp_handshake_parse_challenge(ngx_rtmp_session_t *s,
		 ngx_str_t *peer_key, ngx_str_t *key)
 {printf("%s\n",__func__);
	 ngx_rtmp_buf_t 			 *b;
	 u_char 		   *p;
	 int			   offs;
 
	 int ti;//test
	 b = s->hs_buf;
	 if (*b->pos != '\x03') {
		 printf("handshake: unexpected RTMP version: %x",*b);
		 return RTMP_ERROR;
	 }
	 ++b->pos;
	 s->peer_epoch = 0;
	 memcpy(s->peer_epoch, b->pos, 4);
 
	 p = b->pos + 4;
	 //printf("handshake: peer version=%x.%x.%x.%x epoch=%ld",
	 // 	   p[3], p[2], p[1], p[0], s->peer_epoch);
	 
	 //if (*((int *)p) == 0) {
	 if(p[3]==0&&p[2]==0&& p[1]==0&& p[0]==0){
		 s->hs_old = 1;
		 return RTMP_OK;
	 }	 
	 //printf("handshake: ngx_rtmp_find_digest peer_key %d %s \n",peer_key->len, peer_key->data);
 
	// offs = ngx_rtmp_find_digest(b, peer_key, 772);
	 //offs = rtmp_validate_digest(b, 772);
	 //if (offs == RTMP_ERROR) 
	  offs = ngx_rtmp_find_digest(b, peer_key, 772);
	 if (offs == RTMP_ERROR) {
		 offs = ngx_rtmp_find_digest(b, peer_key, 8);
	 }
	 if (offs == RTMP_ERROR) {
		 printf("handshake: digest not found\n");
		 s->hs_old = 1;
		 return RTMP_OK;
	 }
	 //printf("handshake: digest found at pos=%i \n", offs);
	 b->pos += offs;
	 b->last = b->pos + NGX_RTMP_HANDSHAKE_KEYLEN;
	 //printf("b->pos %x %x %x %x b->last %x %x %x %x ",*(b->pos+0),*(b->pos+1),*(b->pos+2),*(b->pos+3),*(b->last+0),*(b->last+1),*(b->last+2),*(b->last+3) );
	 //s->hs_digest = (u_char *)malloc(sizeof(u_char)*NGX_RTMP_HANDSHAKE_KEYLEN);	 
	 s->hs_digest = ngx_palloc(s->connection->pool, NGX_RTMP_HANDSHAKE_KEYLEN);
	 
	 //if(s->hs_digest == NULL);
	 //{
	 //  printf("s->hs_digest == NULL\n");
	 //  s->hs_digest = (u_char *)malloc(sizeof(u_char)*NGX_RTMP_HANDSHAKE_KEYLEN);  
	 //}
		 
	 if (ngx_rtmp_make_digest(key, b, NULL, s->hs_digest)
			 != RTMP_OK)
	 {
		 return RTMP_ERROR;
	 }	 
	 //printf("%s hs_digest \n",__func__);
	 //for(ti=0;  ti<NGX_RTMP_HANDSHAKE_KEYLEN; ti++)
	 //{
	 //  printf("%x ",(u_char)(*(s->hs_digest+ti)));
	 //}
	 //printf("\n");
	 return RTMP_OK;
 }
 */
	 

void
ngx_rtmp_fill_random_buffer(ngx_rtmp_buf_t *b)
{
 for (; b->last != b->end; ++b->last) {
	 *b->last = (u_char) rand();
 }
}


int
ngx_rtmp_handshake_create_challenge(ngx_rtmp_session_t *s,
	 const u_char version[4], ngx_str_t *key)
{printf("%s\n",__func__);
 ngx_rtmp_buf_t 		 *b;
 

 b = s->hs_buf;//printf("%s 1\n",__func__);
 //b->start = testshr_full;
 //b->end = testshr_full+sizeof(testshr_full);

 //printf("sizeof(sendofRTMP) %d\n",sizeof(sendofRTMP));
 //memset(sendofRTMP, 0x00, sizeof(sendofRTMP));
 b->start = s->snd_buffer;//
 b->end = b->start + RTMP_PACKETSIZE-1;
 
 b->last = b->pos = b->start;
 *b->last++ = '\x03';
 b->last = ngx_rtmp_rcpymem(b->last, &s->epoch, 4);
 //b->last = rtmp_cpymem(b->last, version, 4);
 memcpy(b->last, version, 4);
 
 ngx_rtmp_fill_random_buffer(b);
 ++b->pos;
 if (rtmp_write_digest(b, key, 0) != RTMP_OK) {
	 return RTMP_ERROR;
 }
 --b->pos;
 s1size = b->last - b->pos;
 //printf("S1 size %d \n",s1size);
 return RTMP_OK;
}


int
rtmp_handshake_create_response(ngx_rtmp_session_t *s)
{printf("%s\n",__func__);
    ngx_rtmp_buf_t          *b;
    u_char             *p;
    ngx_str_t           key;
	u_char					digest3[NGX_RTMP_HANDSHAKE_KEYLEN] = {0x00,};

    b = s->hs_buf;
    b->pos = b->last = b->start + 1;
    ngx_rtmp_fill_random_buffer(b);
    if (s->hs_digest) {
        p = b->last - NGX_RTMP_HANDSHAKE_KEYLEN;
        key.data = s->hs_digest;
        key.len = NGX_RTMP_HANDSHAKE_KEYLEN;
        if (ngx_rtmp_make_digest(&key, b, p, digest3) != RTMP_OK) {
            return RTMP_ERROR;
        }
		memcpy(b->last - NGX_RTMP_HANDSHAKE_KEYLEN, digest3, NGX_RTMP_HANDSHAKE_KEYLEN);
    }

    return RTMP_OK;
}

void
ngx_rtmp_handshake_done(ngx_rtmp_session_t *s)
{
    ngx_rtmp_free_handshake_buffers(s);

    printf("handshake: done\n");
	
	if(s1size!=0 && s1size < s->lenofRTMP)
	{
		printf("s1size %d < lenofRTMP %d\n", s1size, s->lenofRTMP);
	}
	else
	{
		 //memset(payloadofRTMP, 0x00, sizeof(u_char)*RTMP_PACKETSIZE);
		 //memset(sendofRTMP, 0x00,  sizeof(u_char)*RTMP_PACKETSIZE);
		 //lenofRTMP = 0;
		 //memset(s->in_buffer, 0x00, sizeof(s->in_buffer));
		 s->lenofRTMP = 0;
	}
	 
//	 s->cur_state = RTMP_COMMAND_STATE;

//    if (ngx_rtmp_fire_event(s, NGX_RTMP_HANDSHAKE_DONE,
//                NULL, NULL) != NGX_OK)
//    {
//        ngx_rtmp_finalize_session(s);
//        return;
//    }

//    ngx_rtmp_cycle(s);
}


void
ngx_rtmp_handshake_send(ngx_rtmp_session_t       *s)
{printf("%s\n",__func__);
    int                   n;
    ngx_rtmp_buf_t           *b;


//    if (c->destroyed) {
//        return;
//    }

//    if (wev->timedout) {
//        ngx_log_error(NGX_LOG_INFO, c->log, NGX_ETIMEDOUT,
//                "handshake: send: client timed out");
//        c->timedout = 1;
//        ngx_rtmp_finalize_session(s);
//        return;
//    }

//    if (wev->timer_set) {
//        ngx_del_timer(wev);
//    }

    b = s->hs_buf;

    while(b->pos != b->last) {
        //n = c->send(c, b->pos, b->last - b->pos);
        if((b->last-b->pos) <= 1460)
        {
		    // printf("_\n");
			//int tti;
			//for(tti = 0; tti<b->last - b->pos; tti ++)
			//	printf("%x ", *(b->pos+tti));
			//printf("_\n");       
			
			n = tcp_write(s->tpcb, (const void*)b->pos,  b->last - b->pos, TCP_WRITE_FLAG_COPY);
			printf("%s tcp_write n %d %d\n",__func__,n, (b->last - b->pos));
			b->pos += (b->last - b->pos);		
        }
		else
		{
		    //printf("_\n");
			//int tti;
			//for(tti = 0; tti<1460; tti ++)
			//	printf("%x ", *(b->pos+tti));
			//printf("_\n");  	
			
			n = tcp_write(s->tpcb, (const void*)b->pos, 1460, TCP_WRITE_FLAG_COPY);
			printf("%s tcp_write n %d %d\n",__func__,n,1460);
			b->pos += 1460;
		}

        if (n != RTMP_OK) {
			dprintf("RTMP_ERR %d \n",n);
            ngx_rtmp_finalize_session(s);
            return;
        }

//        if (n == NGX_AGAIN || n == 0) {
//            ngx_add_timer(c->write, s->timeout);
//            if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
//                ngx_rtmp_finalize_session(s);
//            }
//            return;
//        }

//        b->pos += n;
    }
	
	//tcp_output(s->tpcb);
	//memset(s->snd_buffer, 0x00, sizeof(s->snd_buffer));
//    if (wev->active) {
//        ngx_del_event(wev, NGX_WRITE_EVENT, 0);
//    }

    ++s->hs_stage;
    printf("handshake: stage %ui\n", s->hs_stage);

    switch (s->hs_stage) {
        case NGX_RTMP_HANDSHAKE_SERVER_SEND_RESPONSE:
            if (s->hs_old) {
                //ngx_log_debug0(NGX_LOG_DEBUG_RTMP, s->connection->log, 0,
                //        "handshake: old-style response");
                s->hs_buf->pos = s->hs_buf->start + 1;
                s->hs_buf->last = s->hs_buf->end;
            } else if (rtmp_handshake_create_response(s) != RTMP_OK) {
                printf("handshake: response error\n");
                ngx_rtmp_finalize_session(s);
                return;
            }
            ngx_rtmp_handshake_send(s);
            break;

        case NGX_RTMP_HANDSHAKE_SERVER_RECV_RESPONSE:
            s->hs_buf->pos = s->hs_buf->last = s->hs_buf->start + 1;
            //ngx_rtmp_handshake_recv(c->read);
            break;

        case NGX_RTMP_HANDSHAKE_CLIENT_RECV_CHALLENGE:
            s->hs_buf->pos = s->hs_buf->last = s->hs_buf->start;
            //ngx_rtmp_handshake_recv(c->read);
            break;

        case NGX_RTMP_HANDSHAKE_CLIENT_DONE:
            ngx_rtmp_handshake_done(s);
            break;
    }
}

void
ngx_rtmp_free_handshake_buffers(ngx_rtmp_session_t *s)
{
    ngx_rtmp_core_srv_conf_t   *cscf;
    ngx_chain_t                *cl;

    if (s->hs_buf == NULL) {
        return;
    }
	//free(s->hs_buf);
    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
    cl = ngx_alloc_chain_link(cscf->pool);
    //if (cl == NULL) {
    //    return;
    //}
    cl->buf = s->hs_buf;
    cl->next = cscf->free_hs;
    cscf->free_hs = cl;
    s->hs_buf = NULL;
}


 ngx_buf_t *
ngx_rtmp_alloc_handshake_buffer(ngx_rtmp_session_t *s)
{
    ngx_rtmp_core_srv_conf_t   *cscf;
    ngx_chain_t                *cl;
    ngx_buf_t                  *b;

    //printf("handshake: allocating buffer\n");

    cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);

    if (cscf->free_hs) { //printf("cscf->free_hs\n");
        cl = cscf->free_hs;
        b = cl->buf;
        cscf->free_hs = cl->next;
        ngx_free_chain(cscf->pool, cl);

    } else { //printf("ngx_pcalloc\n");
        b = ngx_pcalloc(cscf->pool, sizeof(ngx_buf_t));
        if (b == NULL) { //printf("ngx_pcalloc b == NULL\n");
            return NULL;
        }
        b->memory = 1;
        b->start = ngx_pcalloc(cscf->pool, NGX_RTMP_HANDSHAKE_BUFSIZE);
        if (b->start == NULL) {//printf("b->start == NULL\n");
            return NULL;
        }
        b->end = b->start + NGX_RTMP_HANDSHAKE_BUFSIZE;
    }

    b->pos = b->last = b->start;

    return b;
}

void
ngx_rtmp_handshake(ngx_rtmp_session_t *s)
{
 //   ngx_connection_t           *c;

 //   c = s->connection;
//    c->read->handler =  ngx_rtmp_handshake_recv;
//    c->write->handler = ngx_rtmp_handshake_send;

    printf("handshake: start server handshake\n");

    s->hs_buf = ngx_rtmp_alloc_handshake_buffer(s);
    s->hs_stage = NGX_RTMP_HANDSHAKE_SERVER_RECV_CHALLENGE;
	//memset(s->in_buffer, 0x00, sizeof(s->in_buffer));
	if(s->snd_buffer)
		memset(s->snd_buffer, 0x00, sizeof(RTMP_PACKETSIZE));
	s->lenofRTMP = 0;

//	if(s->hs_buf == NULL)
//		printf("handshake: s->hs_buf == NULL\n");
    //ngx_rtmp_handshake_recv(c->read);
}


ngx_int_t
ngx_rtmp_handshake_parse_challenge(ngx_rtmp_session_t *s,
        ngx_str_t *peer_key, ngx_str_t *key)
{
    ngx_buf_t              *b;
    u_char                 *p;
    ngx_int_t               offs;

    b = s->hs_buf;
    if (*b->pos != '\x03') {
        printf("handshake: unexpected RTMP version: %i\n",
                (ngx_int_t)*b->pos);
        return NGX_ERROR;
    }
    ++b->pos;
    s->peer_epoch = 0;
    ngx_rtmp_rmemcpy(&s->peer_epoch, b->pos, 4);

    p = b->pos + 4;
    printf("handshake: peer version=%i.%i.%i.%i epoch=%ldD\n",
            (ngx_int_t)p[3], (ngx_int_t)p[2],
            (ngx_int_t)p[1], (ngx_int_t)p[0],
            (uint32_t)s->peer_epoch);
    if (p[0] == 0&&p[1] == 0&&p[2] == 0&&p[3] == 0) {
        s->hs_old = 1;
        return NGX_OK;
    }

    offs = ngx_rtmp_find_digest(b, peer_key, 772);
    if (offs == NGX_ERROR) {
        offs = ngx_rtmp_find_digest(b, peer_key, 8);
    }
    if (offs == NGX_ERROR) {
        printf("handshake: digest not found\n");
        s->hs_old = 1;
        return NGX_OK;
    }
    printf("handshake: digest found at pos=%i\n", offs);
    b->pos += offs;
    b->last = b->pos + NGX_RTMP_HANDSHAKE_KEYLEN;
    //s->hs_digest = (u_char *)malloc(sizeof(u_char)*NGX_RTMP_HANDSHAKE_KEYLEN);
    //memset(s->hs_digest, 0x00, sizeof(u_char*)*NGX_RTMP_HANDSHAKE_KEYLEN);

	s->hs_digest = (u_char *)ngx_palloc(s->conf->pool, NGX_RTMP_HANDSHAKE_KEYLEN);
	//memset(s->hs_digest, 0x00, sizeof(u_char*)*NGX_RTMP_HANDSHAKE_KEYLEN);
    if (ngx_rtmp_make_digest(key, b, NULL, s->hs_digest)!= NGX_OK)
    {
        return NGX_ERROR;
    }
    
    return NGX_OK;
}


void ngx_rtmp_handshake_recv(ngx_rtmp_session_t *s)
{
	
	++s->hs_stage;
	printf("ngx_rtmp_handshake_recv s->hs_stage %d\n",s->hs_stage);
	switch (s->hs_stage) {
		case NGX_RTMP_HANDSHAKE_SERVER_SEND_CHALLENGE:
			{				
				//printf("ngx_rtmp_client_partial_key %d %s ngx_rtmp_server_full_key %d %s\n",ngx_rtmp_client_partial_key.len,ngx_rtmp_client_partial_key.data,ngx_rtmp_server_full_key.len, ngx_rtmp_server_full_key.data);
				if (ngx_rtmp_handshake_parse_challenge(s,&ngx_rtmp_client_partial_key,&ngx_rtmp_server_full_key) != RTMP_OK)
				{
					printf("handshake: error parsing challenge\n");
					dprintf("call : rtmp_recv : Close(disconnect)\n");
					ngx_rtmp_finalize_session(s);
					return RTMP_ERROR;
				}
				if (s->hs_old) {
					printf("handshake: old-style challenge\n");
					s->hs_buf->pos = s->hs_buf->start;
					s->hs_buf->last = s->hs_buf->end;
				} else if (ngx_rtmp_handshake_create_challenge(s,
							ngx_rtmp_server_version,
							&ngx_rtmp_server_partial_key) != RTMP_OK)
				{
					printf("handshake: error creating challenge\n");
					dprintf("call : rtmp_recv : Close(disconnect)\n");
					ngx_rtmp_finalize_session(s);
					return;
				}
				ngx_rtmp_handshake_send(s);
				
				 //memset(s->in_buffer, 0x00, sizeof(s->in_buffer));
				 //memset(sendofRTMP, 0x00,  sizeof(u_char)*RTMP_PACKETSIZE);
				 s->lenofRTMP = 0;
				 //memset(s->in_buffer, 0x00, sizeof(s->in_buffer));
		 		// s->lenofRTMP = 0;
			}
			break;
	
		case NGX_RTMP_HANDSHAKE_SERVER_DONE:
			printf("handshake: RTMP_HANDSHAKE_SERVER_DONE\n");
	
			ngx_rtmp_handshake_done(s);

			break;
		case NGX_RTMP_HANDSHAKE_CLIENT_RECV_RESPONSE:
#ifndef TEST_SERVER1
#ifndef TEST_SERVER2	

#ifndef TEST_SERVER3	
#ifndef TEST_SERVER5

			if (ngx_rtmp_handshake_parse_challenge(s,
						&ngx_rtmp_server_partial_key,
						&ngx_rtmp_client_full_key) != NGX_OK)
			{
				printf("handshake: error parsing challenge\n");
				dprintf("call : rtmp_recv : Close(disconnect)\n");
				ngx_rtmp_finalize_session(s);
				return;
			}
			s->hs_buf->pos = s->hs_buf->last = s->hs_buf->start + 1;
//				rtmp_recv(c->read);
			ngx_rtmp_handshake_recv(s);
#else
			s->hs_buf->pos  += 1;
			s->hs_buf->last = s->hs_buf->pos+s1size-1;
			s->hs_stage = NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE;

			printf("c\n");

			ngx_rtmp_handshake_send(s);
#endif
#else
			s->hs_buf->pos  += 1;
			s->hs_buf->last = s->hs_buf->pos+s1size-1;
			s->hs_stage = NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE;

			ngx_rtmp_handshake_send(s);
#endif	
#else
			s->hs_buf->pos  += 1;
			s->hs_buf->last = s->hs_buf->pos+s1size-1;
			s->hs_stage = NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE;

			ngx_rtmp_handshake_send(s);
#endif

#else
			s->hs_buf->pos	+= 1;
			s->hs_buf->last = s->hs_buf->pos+s1size-1;
			s->hs_stage = NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE;

			ngx_rtmp_handshake_send(s);

#endif
			//memset(payloadofRTMP, 0x00, sizeof(u_char)*RTMP_PACKETSIZE);
			//memset(sendofRTMP, 0x00,  sizeof(u_char)*RTMP_PACKETSIZE);
			//memset(s->in_buffer, 0x00, sizeof(s->in_buffer));
			s->lenofRTMP = 0;
			//s->lenofRTMP = 0;

			break;
	
		case NGX_RTMP_HANDSHAKE_CLIENT_SEND_RESPONSE:
		
			if (rtmp_handshake_create_response(s) != NGX_OK) 
			{
				printf("handshake: response error\n");
			//	dprintf("call : rtmp_recv : Close(disconnect)\n");
				ngx_rtmp_finalize_session(s);
				return;
			}	
			ngx_rtmp_handshake_send(s);
			break;

	}

}

void
ngx_rtmp_client_handshake(ngx_rtmp_session_t *s)
{
    ngx_connection_t           *c;

//    c = s->connection;
//    c->read->handler =  ngx_rtmp_handshake_recv;
//    c->write->handler = ngx_rtmp_handshake_send;

    printf("handshake: start client handshake\n");

    s->hs_buf = ngx_rtmp_alloc_handshake_buffer(s);
    s->hs_stage = NGX_RTMP_HANDSHAKE_CLIENT_SEND_CHALLENGE;

//#ifndef TEST_SERVER3	
	ngx_rtmp_core_srv_conf_t	   *cscf = ngx_rtmp_get_module_srv_conf(s, ngx_rtmp_core_module);
	cscf->chunk_size =128;
	cscf->max_streams = 32*4;
//#endif
    if (ngx_rtmp_handshake_create_challenge(s,
                ngx_rtmp_client_version,
                &ngx_rtmp_client_partial_key) != NGX_OK)
    {
        ngx_rtmp_finalize_session(s);
        return;
    }

//    if (async) {
//        ngx_add_timer(c->write, s->timeout);
//        if (ngx_handle_write_event(c->write, 0) != NGX_OK) {
//            ngx_rtmp_finalize_session(s);
//        }
//        return;
//    }

    ngx_rtmp_handshake_send(s);
}


#endif
