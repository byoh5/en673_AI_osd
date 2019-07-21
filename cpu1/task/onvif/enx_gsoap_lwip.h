
#ifndef __ENX_GSOAP_LWIP__
#define __ENX_GSOAP_LWIP__

#include "stdsoap2.h"

int 
bind_server_udp1(int server_s);

int 
create_server_socket_udp(void);

int 
create_connect_socket_udp(void);

int
enx_soap_bind(struct soap *soap, int port, int backlog);

SOAP_SOCKET
enx_tcp_accept(struct soap *soap, SOAP_SOCKET s, struct sockaddr *a, int *n);

SOAP_SOCKET
enx_soap_accept(struct soap *soap);

int
enx_fsend(struct soap *soap, const char *s, size_t n);

int
enx_fsendhttp(struct soap *soap, const char *s, size_t n);

size_t
enx_frecv(struct soap *soap, char *s, size_t n);

int
enx_tcp_gethost(struct soap *soap, const char *addr, struct in_addr *inaddr);

SOAP_SOCKET
enx_tcp_connect(struct soap *soap, const char *endpoint, const char *host, int port);

int
enx_tcp_disconnect(struct soap *soap);

int
enx_tcp_shutdownsocket(struct soap *soap, SOAP_SOCKET fd, int how);

int
enx_tcp_closesocket(struct soap *soap, SOAP_SOCKET fd);

SOAP_SOCKET
enx_udp_connect(struct soap *soap, const char *endpoint, const char *host, int port);

void *
enx_soap_malloc(struct soap *soap, size_t n);

void
enx_soap_free(struct soap *soap, void *p);
#endif
