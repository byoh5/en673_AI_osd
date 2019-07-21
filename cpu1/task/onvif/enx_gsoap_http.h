#ifndef __ENX_GSOAP_HTTP_H__
#define __ENX_GSOAP_HTTP_H__

#include <stdsoap2.h>
#include <FreeRTOS.h> 
#include "dev.h"
typedef struct {
    char hdr[128];
    char *rxbuf; /*buffer to save http post message */
//    char *rx_p;  /*current poinnter to rxbuf  */
    int  rx_len;
    char *txbuf;
    char *p;
    int tot_len;
    int tx_len;
    void *priv; //point to soap struct
    int SecurityClear;
	int fault;
} onvif_http_t;

struct soap * enx_gsop_start(void *ctx);
void enx_soap_serv(void *ctx);
void enx_soap_end(void *ctx);

//extern onvif_http_t gonvif_http;

void
enx_http2onvif(onvif_http_t *onvif_http, void *pbuf, int wr_pos, size_t n, int securityClear);

size_t
enx_http2recv(struct soap *soap, const char *s, size_t n);

int
enx_http2send(struct soap *soap, const char *s, size_t n);
int
enx_soapsend(struct soap *soap, const char *s, size_t n);
size_t
enx_soaprecv(struct soap *soap, const char *s, size_t n);

void
enx_onvif2http(onvif_http_t *onvif_http, char **s, size_t *n);

void 
enx_onvif2Delete(onvif_http_t *onvif_http);

int
enx_onvifCheckSecurity(onvif_http_t *onvif_http);

int
enx_onvifDeleteSecurity(onvif_http_t *onvif_http);

int
enx_http2close(struct soap *soap);

int
enx_http2disconnect(struct soap *soap);

int
enx_http2shutdownsocket(struct soap *soap);

void onvif_init(void);

#endif //__ENX_GSOAP_HTTP_H__

