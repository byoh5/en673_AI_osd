/**

#define SOAP_INVALID_SOCKET ((SOAP_SOCKET)-1)
#define soap_valid_socket(n) ((n) != SOAP_INVALID_SOCKET)

할당 되어야 할 구조체
soap->fclosesocket = 

*/

#include "dev.h"
#include "factory.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "stdsoap2.h"
#include "stdlib.h"

#include "FreeRTOS.h" 

int 
bind_server_udp1(int server_s)
{dvprintf("%s %d \r\n",__func__,__LINE__);
	struct sockaddr_in local_addr;
	int ret;
	memset(&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(DISCOVERY_PORT);
	ret = bind(server_s,(struct sockaddr*)&local_addr,sizeof(local_addr));
	
	return ret;
}

int 
create_server_socket_udp(void)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
    int server_udp;
	struct ip_mreq mreq;
///	unsigned char one = 1;
///	int sock_opt = 1;
	
    server_udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server_udp == -1) {
        dvprintf("unable to create socket\r\n");
    }
	
	mreq.imr_multiaddr.s_addr = inet_addr(DISCOVERY_IP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(server_udp,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))==-1){
		dvprintf("memberchip error\r\n");
		return;
	}

    return server_udp;
}

int 
create_server_socket_tcp(void)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
    int server_tcp;
///	struct ip_mreq mreq;
///	unsigned char one = 1;
///	int sock_opt = 1;
	
    server_tcp = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_tcp == -1) {
        printf("create_server_socket_tcp unable to create socket\r\n");
    }
	
	//mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	//if(setsockopt(server_tcp,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq))==-1){
	//	dvprintf("memberchip error\r\n");
	//	return;
	//}

    return server_tcp;
}

int 
create_connect_socket_udp(void)
{dvprintf("%s %d \r\n",__func__,__LINE__);
    int connect_udp;

	struct ip_mreq mreq2;
	unsigned int localIp;			
	localIp = inet_addr(IP_ADDRESS);
	
    connect_udp = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (connect_udp == -1) {
        dvprintf("unable to create socket\r\n");
    }
	

	mreq2.imr_multiaddr.s_addr = inet_addr(DISCOVERY_IP);
	mreq2.imr_interface.s_addr = htonl(localIp);

	if(setsockopt(connect_udp,IPPROTO_IP, IP_ADD_MEMBERSHIP,&mreq2,sizeof(mreq2))==-1){
		dvprintf("memberchip error 2\r\n");
		return SOAP_ERR;
	}

    return connect_udp;
}

int
enx_soap_bind(struct soap *soap, int port, int backlog)
{dvprintf("%s %d \r\n",__func__,__LINE__);
	struct sockaddr_in *plocal_addr;
	
	if (soap_valid_socket(soap->master))
	{ 
		soap->fclosesocket(soap, soap->master);
		soap->master = SOAP_INVALID_SOCKET;
	}

	soap->socket = SOAP_INVALID_SOCKET;
	soap->errmode = 1;

	soap->master = (int)lwip_socket(AF_INET, SOCK_STREAM, 0);
	soap->errmode = 0;
	
	
	if (!soap_valid_socket(soap->master))
	{ 
		soap->errnum = soap_socket_errno(soap->master);
		return SOAP_INVALID_SOCKET;
	}

	plocal_addr = calloc(0,sizeof(struct sockaddr_in));
	plocal_addr->sin_family = AF_INET;
	plocal_addr->sin_len = sizeof(struct sockaddr_in);
	plocal_addr->sin_addr.s_addr = htonl(INADDR_ANY);
	plocal_addr->sin_port = port;	
	soap->user = plocal_addr;
	soap->peerlen = sizeof(struct sockaddr_in);
	soap->errmode = 2;


    
	if(lwip_bind(soap->master, (struct sockaddr*)plocal_addr, (int)soap->peerlen))
	{ 
		//soap->errnum = soap_socket_errno(soap->master);
		//DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Could not bind to host\n"));
		soap_closesock(soap);
		//soap_set_receiver_error(soap, tcp_error(soap), "bind failed in soap_bind()", SOAP_TCP_ERROR);
		return SOAP_INVALID_SOCKET;
	}

	if (!(soap->omode & SOAP_IO_UDP) && lwip_listen(soap->master, backlog))
	{ 
		//soap->errnum = soap_socket_errno(soap->master);
		//DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Could not bind to host\n"));
		soap_closesock(soap);
		//soap_set_receiver_error(soap, tcp_error(soap), "listen failed in soap_bind()", SOAP_TCP_ERROR);
		return SOAP_INVALID_SOCKET;
	}
  return soap->master;
}



SOAP_SOCKET
enx_tcp_accept(struct soap *soap, SOAP_SOCKET s, struct sockaddr *a, int *n)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	SOAP_SOCKET fd;
	(void)soap;
	fd = lwip_accept(s, a, (SOAP_SOCKLEN_T*)n); /* portability note: see SOAP_SOCKLEN_T definition in stdsoap2.h */
	
	return fd;
}


/******************************************************************************/

SOAP_SOCKET
enx_soap_accept(struct soap *soap)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	struct sockaddr_in *plocal_addr = (struct sockaddr_in *)soap->user;
	int n = soap->peerlen;
///	register int err;

	soap->error = SOAP_OK;
	memset((void*)plocal_addr, 0, soap->peerlen);
	soap->socket = SOAP_INVALID_SOCKET;
	soap->errmode = 0;
	soap->keep_alive = 0;
  
	if (!soap_valid_socket(soap->master))
	{ 
		soap->errnum = 0;
		//soap_set_receiver_error(soap, tcp_error(soap), "no master socket in soap_accept()", SOAP_TCP_ERROR);
		return SOAP_INVALID_SOCKET;
	}


    soap->socket = soap->faccept(soap, soap->master, (struct sockaddr*)plocal_addr, &n);
    soap->peerlen = (size_t)n;
    
    if (soap_valid_socket(soap->socket))
    {
		soap->ip = ntohl(plocal_addr->sin_addr.s_addr);
		sprintf(soap->host, "%u.%u.%u.%u", (int)(soap->ip>>24)&0xFF, (int)(soap->ip>>16)&0xFF, (int)(soap->ip>>8)&0xFF, (int)soap->ip&0xFF);
		soap->port = (int)ntohs(plocal_addr->sin_port); /* does not return port number on some systems */
		
		//      DBGLOG(TEST,SOAP_MESSAGE(fdebug, "Accept socket %d at port %d from IP %s\n", soap->socket, soap->port, soap->host));
		
		soap->keep_alive = (((soap->imode | soap->omode) & SOAP_IO_KEEPALIVE) != 0);
		
		return soap->socket;
    }
    
	soap_closesock(soap);
	return SOAP_INVALID_SOCKET;
}

int
enx_fsend(struct soap *soap, const char *s, size_t n)
{//printf("%s %d \r\n",__func__,__LINE__);
	int nwritten;
//	char temp[101];
//	memcpy(temp,s,100);
//	temp[100]=0;
//	printf(" enx_fsend n %s %d\r\n",temp,n);
	while (n)
	{ 
		if (soap_valid_socket(soap->socket))
		{
//			ddvprintf(" enx_fsend S soap_valid_socket s_addr %u.%u.%u.%u port %d soap->omode %x\r\n",(int)(soap->peer.sin_addr.s_addr>>24)&0xFF, (int)(soap->peer.sin_addr.s_addr>>16)&0xFF, (int)(soap->peer.sin_addr.s_addr>>8)&0xFF, (int)soap->peer.sin_addr.s_addr&0xFF,soap->peer.sin_port,soap->omode);
			if ((soap->omode & SOAP_IO_UDP))
      		{ //dvprintf(" 1 soap->peerlen %d soap->connect_flags %x\r\n",soap->peerlen,soap->connect_flags);
				if (soap->peerlen)
          			nwritten = lwip_sendto(soap->socket, (char*)s, (SOAP_WINSOCKINT)n, soap->socket_flags, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
        		else
					nwritten = lwip_send(soap->socket, s, (SOAP_WINSOCKINT)n, soap->socket_flags);
			}
			else
			{	
			//dvprintf(" 2 soap->peerlen %d \r\n",soap->peerlen);
				if (soap->peerlen)
          			nwritten = lwip_sendto(soap->socket, (char*)s, (SOAP_WINSOCKINT)n, 0, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
        		else	
					nwritten = lwip_send(soap->socket, (void*)s, n, 0);
			}
			if (nwritten <= 0)
				return SOAP_EOF;
		}
		n -= nwritten;
		s += nwritten;
  }

  return SOAP_OK;
}

#define ONVIF_HTTP_HDR2   "POST %s HTTP/1.1 \r\nAccept-Encoding: gzip,deflate \r\nContent-Type: application/soap+xml; charset=utf-8\r\nHost: %s \r\nContent-Length:%d\r\nConnection: close\r\n\r\n"

int
enx_fsendhttp(struct soap *soap, const char *s, size_t n)
{ dvprintf("%s %d size %d\r\n", __func__, __LINE__, n);
	int nRes = SOAP_ERR;
	char *phttp = (char *)pvPortCalloc(n+300, 1);
	if(phttp != NULL)
	{
		sprintf(phttp, ONVIF_HTTP_HDR2, soap->path, soap->host, n);
		char *urls2 = strstr(phttp, "\r\n\r\n");
		int nwritten, flen = (urls2 - phttp) + 4;

		memcpy(phttp+flen, s, n);
		phttp[flen+n] = 0;

		n += flen;
		s = phttp;

		nRes = SOAP_OK;
		while (n)
		{
			if (soap_valid_socket(soap->socket))
			{
				if ((soap->omode & SOAP_IO_UDP))
				{
					if (soap->peerlen)
						nwritten = lwip_sendto(soap->socket, (char*)s, (SOAP_WINSOCKINT)n, soap->socket_flags, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
					else
						nwritten = lwip_send(soap->socket, s, (SOAP_WINSOCKINT)n, soap->socket_flags);
				}
				else
				{	
					if (soap->peerlen)
						nwritten = lwip_sendto(soap->socket, (char*)s, (SOAP_WINSOCKINT)n, 0/*soap->socket_flags*/, (struct sockaddr*)&soap->peer, (SOAP_WINSOCKINT)soap->peerlen);
					else
						nwritten = lwip_send(soap->socket, (void*)s, n, soap->socket_flags);
				}

				if (nwritten <= 0)
				{
					nRes = SOAP_EOF;
					break;
				}
			}
			n -= nwritten;
			s += nwritten;
		}

		if(phttp)
			vPortFree(phttp);
	}

	return nRes;
}

size_t
enx_frecv(struct soap *soap, char *s, size_t n)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	int r;
 	if (soap_valid_socket(soap->socket))
	{
		if((soap->omode & SOAP_IO_UDP))
		{
			SOAP_SOCKLEN_T k = (SOAP_SOCKLEN_T)sizeof(soap->peer);
			memset((void*)&soap->peer, 0, sizeof(soap->peer));
			r = lwip_recvfrom(soap->socket,
								s,
								(SOAP_WINSOCKINT)n, 
								soap->socket_flags,
								(struct sockaddr*)&soap->peer,
								&k);	/* portability note: see SOAP_SOCKLEN_T definition in stdsoap2.h */
			soap->peerlen = (size_t)k;
		}
		else
		{						
			r = lwip_recv(soap->socket, s, (int)n, soap->socket_flags);
		}
		s[r] =0;

		if (r >= 0)
			return (size_t)r;
		return 0;
	}
	return 0;
}


/******************************************************************************/
int
enx_tcp_gethost(struct soap *soap, const char *addr, struct in_addr *inaddr)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	soap_int32 iadd = -1;
	struct hostent hostent, *host = &hostent;
	
	iadd = inet_addr(addr);
	
	if (iadd != -1)
  	{ 
		memcpy(inaddr, &iadd, sizeof(iadd));
		return SOAP_OK;
	}
	
	if (gethostbyname_r(addr, &hostent, soap->buf, SOAP_BUFLEN, &host, &soap->errnum) < 0)
    	host = NULL;

	if (!host)
	{ 
		//DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Host name not found\n"));
		return SOAP_ERR;
	}
	memcpy(inaddr, host->h_addr, host->h_length);
	return SOAP_OK;
}

/******************************************************************************/

/**
	@param endpoint : do not process.
*/

SOAP_SOCKET
enx_tcp_connect(struct soap *soap, const char *endpoint, const char *host, int port)
{dvprintf("enx_tcp_connect  1 endpoint %s host %s port %d\r\n",endpoint,host,port);
	SOAP_SOCKET fd;
	struct sockaddr_in pdst_addr;
///	int err = 0;
	int timeout= 3000;

	if (soap_valid_socket(soap->socket))
		soap->fclosesocket(soap, soap->socket);
		
	soap->socket = SOAP_INVALID_SOCKET;
	
	soap->errmode = 0;
	
	fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (fd < 0)
	{
		return SOAP_INVALID_SOCKET;
	}
	
//	pdst_addr = calloc(0,sizeof(struct sockaddr_in));
	pdst_addr.sin_family = AF_INET;
	pdst_addr.sin_len = sizeof(struct sockaddr_in);
	if(host)
		pdst_addr.sin_addr.s_addr = inet_addr(host);
	else
		pdst_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	pdst_addr.sin_port = port;	
	soap->peer = pdst_addr;
	soap->peerlen = sizeof(struct sockaddr_in);
	
	//if(enx_tcp_gethost(soap, host,&pdst_addr.sin_addr))
	//{
	//	soap->fclosesocket(soap, fd);
	//	return SOAP_INVALID_SOCKET;
	//}
	soap->errmode = 0;
	
	lwip_setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

	if (lwip_connect(fd, (struct sockaddr*)&pdst_addr, soap->peerlen) < 0)
	{ 
		soap->fclosesocket(soap, fd);
		return SOAP_INVALID_SOCKET;
	}

	soap->socket = fd;
	soap->master = fd;
	dvprintf("enx_tcp_connect  fd %d\r\n",fd);
	return SOAP_OK;//soap->socket;

	UNUSED(endpoint);
}


SOAP_SOCKET
enx_udp_connect(struct soap *soap, const char *endpoint, const char *host, int port)
{dvprintf("enx_udp_connect  1 endpoint %s host %s port %d\r\n",endpoint,host,port);
	SOAP_SOCKET fd;
	struct sockaddr_in pdst_addr;
///	int err = 0;

	if (!soap_valid_socket(soap->socket))
	{
		///soap->fclosesocket(soap, soap->socket);
			
		//soap->socket = SOAP_INVALID_SOCKET;
		
		//soap->errmode = 0;
		
		fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);		
		if (fd < 0)
		{
//		dvprintf("enx_udp_connect SOAP_INVALID_SOCKET\r\n");
			return SOAP_INVALID_SOCKET;
		}
		
		soap->socket = fd;
	}
	else
	{
		fd = soap->socket;
	}
	
//	pdst_addr = calloc(0,sizeof(struct sockaddr_in));
	pdst_addr.sin_family = AF_INET;
	pdst_addr.sin_len = sizeof(struct sockaddr_in);
	if(host)
		pdst_addr.sin_addr.s_addr = inet_addr(host);
	else
		pdst_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	pdst_addr.sin_port = port;	
	soap->peer = pdst_addr;
	soap->peerlen = sizeof(struct sockaddr_in);
	
//	if(enx_tcp_gethost(soap, host,&pdst_addr.sin_addr))
//	{
	//	soap->fclosesocket(soap, fd);
	//	return SOAP_INVALID_SOCKET;
	//}
	soap->errmode = 0;
	
	if (lwip_connect(fd, (struct sockaddr*)&pdst_addr, soap->peerlen) < 0)
	{ 
		soap->fclosesocket(soap, fd);
		return SOAP_INVALID_SOCKET;
	}

//	dvprintf("enx_tcp_connect  2\r\n");
	return fd;

	UNUSED(endpoint);
}

int
enx_tcp_disconnect(struct soap *soap)
{dvprintf("%s %d \r\n",__func__,__LINE__);
	if(soap == NULL)
		return SOAP_ERR;
	if (soap_valid_socket(soap->socket) && (!(soap->omode & SOAP_IO_UDP)))
	{ 
		//soap->fshutdownsocket(soap, soap->socket, SOAP_SHUT_RDWR);
		soap->fclosesocket(soap, soap->socket);
		soap->socket = SOAP_INVALID_SOCKET;
	}
	return SOAP_OK;
}


int
enx_tcp_shutdownsocket(struct soap *soap, SOAP_SOCKET fd, int how)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	if(soap == NULL)
		return SOAP_ERR;

	//(void)soap;
	//DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Shutdown socket %d how=%d\n", (int)fd, how));
	return shutdown(fd, how);
}


int
enx_tcp_closesocket(struct soap *soap, SOAP_SOCKET fd)
{ dvprintf("%s %d \r\n",__func__,__LINE__);
	if(soap == NULL)
		return SOAP_ERR;

	//(void)soap;
	//DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Close socket %d\n", (int)fd));
	return soap_closesocket(fd);
}

void *
enx_soap_malloc(struct soap *soap, size_t n)
{
#if 1
	void *mal = pvPortMalloc(n);
	if(mal == NULL)
	{
		while(1)
		{
			printf("%s(%d) : NULL!!\r\n", __func__, __LINE__);
		}
	}
	return mal;
#else
	return pvPortMalloc(n);
#endif
}

void
enx_soap_free(struct soap *soap, void *p)
{
#if 1
	if(p == NULL)
		while(1)
		{
			printf("%s(%d) : NULL!!\r\n", __func__, __LINE__);
		}		
	else
#endif
	vPortFree(p);
	p = NULL;

	UNUSED(soap);
}
