#ifndef __HTTP_FRONT_H__
#define __HTTP_FRONT_H__

#define RTSP_PARAM_STRING_MAX     64
#define MAX_TUNNELING             RTSP_info_MAX
#define FRONT_TX_BUFFER           1024

#include "rtsp_control.h"

typedef struct 
{
  int is_valid;	// = vaild, 0 = empty; 
  char sessionCookie[80];
 
  struct tcp_pcb *in_pcb;
  struct tcp_pcb *out_pcb; 
  void  *priv;   
} tunneling_t;


typedef struct 
{
  pcb_type_t pcb_type;
  int b_get;
  int b_post;
  int IsActive;
  
  int base64_remainings;
  u8 *last_crlf;
  
  u32 req_left;
  u32 req_alreay_seen;
  
  u8 req_buff[FRONT_TX_BUFFER];
  
  u32 resp_pos;
  u32 resp_left;
  u8  resp_buff[FRONT_TX_BUFFER];
  
  u8  urlSuffix[24];
  void *priv;
  rtsp_con_t *prtsp_conn; //for RTSP Context
  struct tcp_pcb *in_pcb;
  struct tcp_pcb *out_pcb;
  tunneling_t *p_tunnel;
  u32 is_connect;
} hs_front_t;


tunneling_t *
tunnel_find(hs_front_t *front, char const* sessionCookie);

int
tunnel_add(hs_front_t *front, char const* sessionCookie);

void
tunnel_port_del(void *pcb);

void
reset_request_buffer(hs_front_t *front);

void
handleHTTPCmd_notSupported(hs_front_t * front);

int
handleHTTPCmd_TunnelingPOST(hs_front_t *front, char const* sessionCookie);

void
handleHTTPCmd_TunnelingGET(hs_front_t *front, char const* sessionCookie);

void
handleHTTPCmd_StreamingGET(hs_front_t *front, char const * urlSuffix, char const* fullRequestStr);

int
http_front_send(struct tcp_pcb *pcb, hs_front_t * front);

hs_front_t *
http_front_alloc(void);

void
http_front_err(void *arg, err_t err);

err_t
http_front_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

void
http_front_init(void);

#endif