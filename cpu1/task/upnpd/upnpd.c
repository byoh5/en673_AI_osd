/* Copyright (c) 2013 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#include "upnpd.h"

#include "dev.h"
#include "user.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/tcpip.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "ethernetif.h"															// to use network_check_netif

//-------------------------------------------------------------------------------------------------
//
#if (ENX_UPNPD_use==1)
#if 0
void UpnpUdpTask(void *pvParameters)
{
	char	ipAddr[64];
	int		err;
	struct netif	*ptNetIf;
	int recv_sock;
	struct sockaddr_in addr;
	struct sockaddr_in send_addr;
	char sendString[512];

	// alive
	static const char* sendAlive =	"NOTIFY * HTTP/1.1\r\n"
									"HOST: 239.255.255.250:1900\r\n"
									"CACHE-CONTROL: max-age=2\r\n"
									"LOCATION: http://%s:%d/%s\r\n"
									"NT: upnp:rootdevice\r\n"
									"NTS: ssdp:alive\r\n"
									"SERVER: RTOS/1.0, UPnP/1.0, %s/1.0\r\n"
									"USN: uuid:%s::upnp:rootdevice\r\n"
									"\r\n";
	// byebye
	static const char sendBye[] =	"NOTIFY * HTTP/1.1\r\n"
									"HOST: 239.255.255.250:1900\r\n"
									"NT: upnp:rootdevice\r\n"
									"NTS: ssdp:byebye\r\n"
									"USN: uuid:%s::upnp:rootdevice\r\n"
									"\r\n";

	ptNetIf = (struct netif*)pvParameters;
	while(!netif_is_up(ptNetIf))	// Network is ready ?
	{
		vTaskDelay(10);
	}

	UpnpRecvMsg = upnpudp_alive;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = UPNP_MCAST_PORT;

	memset(&send_addr, 0, sizeof(send_addr));
	send_addr.sin_family = AF_INET;
	send_addr.sin_addr.s_addr = inet_addr(UPNP_MCAST_ADDR);
	send_addr.sin_port = UPNP_MCAST_PORT;

	for(;;)
	{
		recv_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if(recv_sock == -1)
		{
//			printf("UDP socket error! retry...\r\n");
			vTaskDelay(100);
			continue;
		}

		if(bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
//			printf("UDP bind error! retry...\r\n");
			close(recv_sock);
			vTaskDelay(100);
			continue;
		}
		break;
	}

	char strDevid[16] = "";
	for(;;)
	{
//		printf("UPNP : msg(%d)\r\n", UpnpRecvMsg);

		switch(UpnpRecvMsg)
		{
			case upnpudp_alive:
				NetASCII_IP_to_Str(gtUser.nIpAddr, &ipAddr, DEF_NO, &err);
				sprintf(sendString, sendAlive, ipAddr, UPNP_LOCAL_PORT, UPNP_Root_URL, UPNP_SERVER, gtUser.strDeviceId);
				strcpy(strDevid, gtUser.strDeviceId);
				break;
			case upnpudp_byebye:
				sprintf(sendString, sendBye, strDevid);
				UpnpRecvMsg = upnpudp_alive;
				break;
		}
		sendto(recv_sock, sendString, strlen(sendString), 0, (struct sockaddr *)&send_addr, sizeof(send_addr));

		vTaskDelay(UPNP_BRDCAST_PERIOD);
	}
}
#endif
#if 0
static void UpnpMsearchRecv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port)
{
	dprintf("UPNP M-Search\r\n");
	if(p != NULL)
	{
		int i;
		char *pc;

		printf("dst ip %x, port %d \r\n", addr->addr, port);
		pc = (char*)p->payload;
		pc[p->tot_len] = 0;

		dprintf("====================================\r\n");
		if(strncmp(pc, "M-SEARCH", 8) == 0)
		{
			for(i=0;i<p->tot_len;i++)
				dprintf("%c", pc[i]);
		}
		else
		{
			dprintf("etc...\r\n");
		}
		dprintf("====================================\r\n");
	}
}

#include "lwip/ip.h"
#include "lwip/udp.h"

void UpnpMsearch(void)
{
	static const char* sendAns ="HTTP/1.1 200 OK\r\n"
								"CACHE-CONTROL: max-age=2\r\n"
								"DATE: %s\r\n"
								"EXT: \r\n"
								"LOCATION: http://%s:%d/rootdesc1.xml\r\n"
								"SERVER: RTOS/1.0, UPnP/1.0, %s/1.0\r\n"
								"ST: urn:schemas-upnp-org:device:enxIPCam:1.0\r\n"
								"USN: uuid:%s::upnp:rootdevice\r\n"
								"\r\n";

	static struct udp_pcb* upnpd_udp_pcb;
	if(upnpd_udp_pcb == NULL)
	{
		dprintf("UPNPD : UDP(M-search) : INIT\r\n");
		upnpd_udp_pcb = udp_new();
		if(upnpd_udp_pcb != NULL)
		{
			err_t ert;
			ip_addr_t upnpip;
			upnpip.addr = inet_addr(UPNP_MCAST_ADDR);

			udp_bind(upnpd_udp_pcb, IP_ADDR_ANY, UPNP_MCAST_PORT);
			udp_recv(upnpd_udp_pcb, UpnpMsearchRecv, NULL);

			if((ert=igmp_joingroup(IP_ADDR_ANY, &upnpip)) != ERR_OK)
			{
				printf("%s, err %d\r\n", __func__, ert);		  
				return;	
			}
		}
		else
		{
			printf("UPNPD : UDP Fail\r\n");
		}
	}

	/*
	static struct udp_pcb* upnpd_udp_pcb;
	if(upnpd_udp_pcb == NULL)
	{
		dprintf("UPNPD : UDP(M-search) : INIT\r\n");
		upnpd_udp_pcb = udp_new();
		if(upnpd_udp_pcb != NULL)
		{
			err_t ert;
			ip_addr_t upnpip;
			upnpip.addr = inet_addr(UPNP_MCAST_ADDR);

			//upnpd_udp_pcb->
//			upnpd_udp_pcb->flags |= UDP_FLAGS_MULTICAST_LOOP;

			upnpd_udp_pcb->multicast_ip.addr = upnpip.addr;

			gtMyNetIf.flags |= NETIF_FLAG_IGMP;
			igmp_joingroup(INADDR_ANY, &upnpip);


			ip_set_option(upnpd_udp_pcb, SOF_BROADCAST);
			ert = udp_bind(upnpd_udp_pcb, INADDR_ANY, UPNP_MCAST_PORT);
			dprintf("UPNPD : UDP(M-search) : udp_bind(%d)\r\n", ert);
			ert = udp_connect(upnpd_udp_pcb, &upnpip, UPNP_MCAST_PORT);
			dprintf("UPNPD : UDP(M-search) : udp_connect(%d)\r\n", ert);


			
			udp_recv(upnpd_udp_pcb, UpnpMsearchRecv, &gtMyNetIf);
		}
		else
		{
			printf("UPNPD : UDP Fail\r\n");
		}
	}
	*/
}
#endif
#if 0

#define MAXBUF 256
#define PUERTO 5000
#define GRUPO "224.0.1.1"

/*
 * PACK_STRUCT_BEGIN
 * struct ip_addr {
 *    PACK_STRUCT_FIELD(u32_t addr);
 * } PACK_STRUCT_STRUCT;
 * PACK_STRUCT_END
*/

struct udp_pcb* upcb;

static void 
multicast_udp_recv_cb(void *arg,
                      struct udp_pcb *pcb,
                      struct pbuf *p,
                      struct ip_addr *ip,
                      u16_t port)
{
    int len;
    char *buf;
    if (p != NULL) {
        printf("%s dst ip %x, port %d \r\n", __func__,ip->addr, port);
        buf = p->payload;
        buf[p->tot_len] = 0;
        printf("len %d : %s \r\n", p->tot_len, buf);
        //_hexdump(__func__,p->payload,p->tot_len,1);
        //udp_sendto(pcb, p,ip, port); //dest port
       // pbuf_free(p);
    }
}

/**
    @return 
        success :  0
        fail    : -1    
*/
int
multicast_udp_start(void)
{
    struct ip_addr groupaddr;
    err_t err;

    upcb = udp_new();
    if (upcb == NULL) {
        printf("udp [%p]: could not allocate pcb\r\n", upcb);
        return -1;
    }

    udp_bind(upcb, IP_ADDR_ANY, PUERTO);
    udp_recv(upcb, multicast_udp_recv_cb, NULL);
//    if (    
    groupaddr.addr = inet_addr(GRUPO);

    printf("groupaddr.addr %x \r\n",groupaddr.addr);
    
    if((err = igmp_joingroup(IP_ADDR_ANY,(struct ip_addr *)&groupaddr)) != ERR_OK)
    {
        printf("%s, err %d \r\n", __func__,err);        
        return -1;  
    };
    return 0;
}

#endif

#if 0
//-------------------------------------------------------------------------------------------------
// Task(Tcp)
void UpnpTcpTask(void *pvParameters)
{
//	vTaskDelete(NULL);

	struct netif	*ptNetIf;
	char	ipAddr[64];
	int 	err;
	int 	i;
	int 	len;
	char	*pc;
	char	rcvBuf[RCVBUFSIZE];
	static const char MsgFmt_0[200];
	static const char MsgFmt_1[200];
	int serv_sock;
	int clnt_sock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	int clnt_addr_size;
	
	char	MsgFmtXML[RCVBUFSIZE];

//////////////////////////////////////////////////////////////////////////////////////////
////http://IP_ADDRESS:UPNP_LOCAL_PORT/rootdesc1.xml///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	static const char* MsgRoot =	"<?xml version=\"1.0\"?>\r\n"
									"<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n"
										"<specVersion>\r\n"
											"<major>1</major>\r\n"
											"<minor>0</minor>\r\n"
										"</specVersion>\r\n"
										"<device>\r\n"
											"<deviceType>urn:schemas-upnp-org:device:%s:1.0</deviceType>\r\n"
											"<friendlyName>%s</friendlyName>\r\n"
											"<manufacturer>%s</manufacturer>\r\n"
											"<manufacturerURL>%s</manufacturerURL>\r\n"
											"<modelDescription>%s</modelDescription>\r\n"
											"<modelName>%s</modelName>\r\n"
											"<modelNumber>%s</modelNumber>\r\n"
											"<modelURL>rtsp://%s:%d</modelURL>\r\n"
											"<serialNumber>%s</serialNumber>\r\n"
											"<UDN>uuid:%s</UDN>\r\n"
											"<presentationURL>http://%s</presentationURL>\r\n"
											"<serviceList>\r\n"
												"<service>\r\n"
													"<serviceType>urn:schemas-upnp-org:service:VideoStream:1</serviceType>\r\n"
													"<serviceId>urn:upnp-org:serviceId:VStream</serviceId>\r\n"
													"<controlURL></controlURL>\r\n"
													"<eventSubURL></eventSubURL>\r\n"
													"<SCPDURL>/%s</SCPDURL>\r\n"
												"</service>\r\n"
											"</serviceList>\r\n"
										"</device>\r\n"
									"</root>\r\n";
//////////////////////////////////////////////////////////////////////////////////////////
////http://IP_ADDRESS:UPNP_LOCAL_PORT/rootdesc2.xml///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	static const char* MsgService = "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\r\n"
										"<specVersion>\r\n"
											"<major>1</major>\r\n"
											"<minor>0</minor>\r\n"
										"</specVersion>\r\n"
										"<serviceStateTable>\r\n"
											"<stateVariable sendEvents=\"no\">\r\n"
												"<name>H264</name>\r\n"
												"<dataType>string</dataType>\r\n"
												"<defaultValue>rtsp://%s:%d/%s</defaultValue>\r\n"
											"</stateVariable>\r\n"
											"<stateVariable sendEvents=\"no\">\r\n"
												"<name>JPEG</name>\r\n"
												"<dataType>string</dataType>\r\n"
												"<defaultValue>rtsp://%s:%d/%s</defaultValue>\r\n"
											"</stateVariable>\r\n"
										"</serviceStateTable>\r\n"
									"</scpd>\r\n";
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

	ptNetIf = (struct netif*)pvParameters;
	while(!netif_is_up(ptNetIf))	// Network is ready ?
	{
		vTaskDelay(10);
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = UPNP_LOCAL_PORT;

	for(;;)
	{
		serv_sock = socket(PF_INET, SOCK_STREAM, 0);
		if(serv_sock == -1)
		{
			dprintf("UPNP(T) : TCP socket error! retry...\r\n");
			vTaskDelay(100);
			continue;
		}

		if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
		{
			dprintf("UPNP(T) : TCP bind error! retry...\r\n");
			close(serv_sock);
			vTaskDelay(100);
			continue;
		}

		if(listen(serv_sock, 3) == -1)
		{
			dprintf("UPNP(T) : TCP listen error! retry...\r\n");
			close(serv_sock);
			vTaskDelay(100);
			continue;
		}

		for(;;)
		{
			clnt_addr_size = sizeof(clnt_addr);
			clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);	// serrv_sock의 sin_port에 맞는 포트값만 accept 가능
			if(clnt_sock == -1)
			{
				dprintf("UPNP(T) : TCP accept error! retry...\r\n");
				vTaskDelay(100);
				continue;
			}

			dprintf("UPNP(T) : UPNP : Accept Address : %s\r\n", inet_ntoa(clnt_addr.sin_addr));

			len = read(clnt_sock, rcvBuf, RCVBUFSIZE);
			pc = (char*)rcvBuf;
			for(i=0;i<len;i++)
			{
				rcvBuf[i] = pc[i];
			}
			rcvBuf[len] = '\0';

			NetASCII_IP_to_Str(gtUser.nIpAddr, &ipAddr, DEF_NO, &err);
			if(!strncmp(rcvBuf, "GET /"UPNP_Root_URL, strlen("GET /"UPNP_Root_URL)))
			{
				sprintf(MsgFmtXML, MsgRoot, UPNP_DEVICETYPE, gtUser.strDeviceName, UPNP_MANUFACTURER, UPNP_MANUFACTURER_URL, UPNP_MODELDESCRIPTION, UPNP_MODELNAME, UPNP_MODELNUMBER, ipAddr, gtUser.nRtspPort, UPNP_SERIALNUMBER, gtUser.strDeviceId, ipAddr, UPNP_Sub1_URL);
				sprintf(MsgFmt_0,"HTTP/1.1 200 OK\r\nCONTENT-LENGTH: %d\r\nCONTENT-TYPE: text/xml\r\n\r\n", strlen(MsgFmtXML));
			
				write(clnt_sock, MsgFmt_0, strlen(MsgFmt_0));
				write(clnt_sock, MsgFmtXML, strlen(MsgFmtXML));
			}
			else if(!strncmp(rcvBuf, "GET /"UPNP_Sub1_URL, strlen("GET /"UPNP_Sub1_URL)))
			{
				sprintf(MsgFmtXML, MsgService, ipAddr, gtUser.nRtspPort, H264URL, ipAddr, gtUser.nRtspPort, JPEGURL);
				sprintf(MsgFmt_1,"HTTP/1.1 200 OK\r\nCONTENT-LENGTH: %d\r\nCONTENT-TYPE: text/xml\r\n\r\n", strlen(MsgFmtXML));

				write(clnt_sock, MsgFmt_1, strlen(MsgFmt_1));
				write(clnt_sock, MsgFmtXML, strlen(MsgFmtXML));
			}
			close(clnt_sock);
		}
	}
}
#endif

static struct udp_pcb* upnpd_udp_pcb;

static void upnpd_advertisement_send(void *arg)
{
	// alive
	static const char *sendAlive = "NOTIFY * HTTP/1.1\r\n"
									"HOST: 239.255.255.250:1900\r\n"
									"CACHE-CONTROL: max-age=2\r\n"
									"LOCATION: http://%s:%d/%s\r\n"
									"NT: upnp:rootdevice\r\n"
									"NTS: ssdp:alive\r\n"
									"SERVER: RTOS/1.0, UPnP/1.0, %s/1.0\r\n"
									"USN: uuid:%s::upnp:rootdevice\r\n"
									"\r\n";
	// byebye
	static const char *sendBye =	"NOTIFY * HTTP/1.1\r\n"
									"HOST: 239.255.255.250:1900\r\n"
									"NT: upnp:rootdevice\r\n"
									"NTS: ssdp:byebye\r\n"
									"USN: uuid:%s::upnp:rootdevice\r\n"
									"\r\n";

	struct pbuf* updpd_pbuf;
	unsigned int nSendSize = 0;
	char strSendMsg[512] = {0};
	static char ipAddr[16] = {0};
	static char strDevid[16];

	struct netif *nif = netif_list;
	for(nif = netif_list; nif != NULL; nif = nif->next)
	{
		if(!netif_is_up(nif))
			continue;
	
		ipaddr_ntoa_r(&nif->ip_addr, ipAddr, 16);

		switch(UpnpRecvMsg)
		{
			case upnpudp_alive:
				nSendSize = sprintf(strSendMsg, sendAlive, ipAddr, UPNP_LOCAL_PORT, UPNP_Root_URL, UPNP_SERVER, gtUser.strDeviceId);
				strcpy(strDevid, (char*)gtUser.strDeviceId);
				break;
			case upnpudp_byebye:
				nSendSize = sprintf(strSendMsg, sendBye, strDevid);
				UpnpRecvMsg = upnpudp_alive;
				break;
			default:
				dprintf("ERROR(%s(),%d) : upnp advertisement command fail\r\n", __func__, __LINE__);
				continue;
		}

		updpd_pbuf = pbuf_alloc(PBUF_TRANSPORT, nSendSize, PBUF_RAM);
		if(updpd_pbuf != NULL)
		{
			DmaMemCpy_ip((BYTE *)updpd_pbuf->payload, (BYTE *)strSendMsg, nSendSize);
			invalidate_dcache_range((uint)(updpd_pbuf->payload), (uint)(updpd_pbuf->payload+updpd_pbuf->len));

			err_t ert = udp_sendto_if(upnpd_udp_pcb, updpd_pbuf, &upnpd_udp_pcb->remote_ip, upnpd_udp_pcb->remote_port, nif);
			if(ert != ERR_OK)
			{
				dprintf("ERROR(%s(),%d) : upnp udp_send fail : error code(%d)\r\n", __func__, __LINE__, ert);
			}
			pbuf_free(updpd_pbuf);
		}
		else
		{
			dprintf("ERROR(%s(),%d) : upnp updpd_pbuf malloc fail\r\n", __func__, __LINE__);
		}
	}
	UNUSED(arg);
}

int upnpd_advertisement_init(void)
{
	if(upnpd_udp_pcb == NULL)
	{
		upnpd_udp_pcb = udp_new();
		if(upnpd_udp_pcb != NULL)
		{
			UpnpRecvMsg = upnpudp_alive;
			err_t ert = udp_bind(upnpd_udp_pcb, IP_ADDR_ANY, UPNP_MCAST_PORT);
			if(ert != ERR_OK)
			{
				dprintf("ERROR(%s(),%d) : upnp udp_bind fail : error code(%d)\r\n", __func__, __LINE__, ert);
				return 0;
			}
			else
			{
				ipaddr_aton(UPNP_MCAST_ADDR, &(upnpd_udp_pcb->remote_ip));
				upnpd_udp_pcb->remote_port = UPNP_MCAST_PORT;
				return 1;
			}
		}
		else
		{
			dprintf("ERROR(%s(),%d) : malloc fail upnpd_udp_pcb\r\n", __func__, __LINE__);
			return 0;
		}
	}
	else
	{
		dprintf("ERROR(%s(),%d) : use upnpd_udp_pcb\r\n", __func__, __LINE__);
		return 0;
	}
	return 0;
}

void upnpd_msearch_init(void *pvParameters)
{
	IGMP_Join("239.255.255.250");

	UNUSED(pvParameters);
}

static struct tcp_pcb* upnpd_tcp_pcb;
static struct tcp_pcb* upnpd_tcp_pcb_li;

static err_t upnpd_send_close(struct tcp_pcb *pcb)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb, NULL);
	tcp_poll(pcb, NULL, 3);
	return tcp_close(pcb);
}

static err_t upnpd_send_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
//////////////////////////////////////////////////////////////////////////////////////////
////http://IP_ADDRESS:UPNP_LOCAL_PORT/UPNP_Root_URL///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	static const char* sendRoot = \
		"<?xml version=\"1.0\"?>\r\n"
		"<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n"
			"<specVersion>\r\n"
				"<major>1</major>\r\n"
				"<minor>0</minor>\r\n"
			"</specVersion>\r\n"
			"<device>\r\n"
				"<deviceType>urn:schemas-upnp-org:device:%s:1.0</deviceType>\r\n"
				"<friendlyName>%s</friendlyName>\r\n"
				"<manufacturer>%s</manufacturer>\r\n"
				"<manufacturerURL>%s</manufacturerURL>\r\n"
				"<modelDescription>%s</modelDescription>\r\n"
				"<modelName>%s</modelName>\r\n"
				"<modelNumber>%s</modelNumber>\r\n"
				"<modelURL>rtsp://%s:%d</modelURL>\r\n"
				"<serialNumber>%s</serialNumber>\r\n"
				"<UDN>uuid:%s</UDN>\r\n"
				"<presentationURL>http://%s</presentationURL>\r\n"
				"<serviceList>\r\n"
					"<service>\r\n"
						"<serviceType>urn:schemas-upnp-org:service:VideoStream:1</serviceType>\r\n"
						"<serviceId>urn:upnp-org:serviceId:VStream</serviceId>\r\n"
						"<controlURL></controlURL>\r\n"
						"<eventSubURL></eventSubURL>\r\n"
						"<SCPDURL>/%s</SCPDURL>\r\n"
					"</service>\r\n"
				"</serviceList>\r\n"
			"</device>\r\n"
		"</root>\r\n";
//////////////////////////////////////////////////////////////////////////////////////////
////http://IP_ADDRESS:UPNP_LOCAL_PORT/UPNP_Sub1_URL///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
	static const char* sendService = \
		"<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\r\n"
			"<specVersion>\r\n"
				"<major>1</major>\r\n"
				"<minor>0</minor>\r\n"
			"</specVersion>\r\n"
			"<serviceStateTable>\r\n"
				"<stateVariable sendEvents=\"no\">\r\n"
					"<name>H264</name>\r\n"
					"<dataType>string</dataType>\r\n"
					"<defaultValue>rtsp://%s:%d/%s</defaultValue>\r\n"
				"</stateVariable>\r\n"
				"<stateVariable sendEvents=\"no\">\r\n"
					"<name>JPEG</name>\r\n"
					"<dataType>string</dataType>\r\n"
					"<defaultValue>rtsp://%s:%d/%s</defaultValue>\r\n"
				"</stateVariable>\r\n"
			"</serviceStateTable>\r\n"
		"</scpd>\r\n";
//////////////////////////////////////////////////////////////////////////////////////////
#define HTTP200OK_XML "HTTP/1.1 200 OK\r\nCONTENT-LENGTH: %d\r\nCONTENT-TYPE: text/xml\r\n\r\n"
//////////////////////////////////////////////////////////////////////////////////////////

	if(err == ERR_OK && p != NULL)
	{
		int nSendSize = 0;
		char sendBuf[RCVBUFSIZE*2], *pc;
		char MsgFmtXML[RCVBUFSIZE];

		tcp_recved(pcb, p->tot_len);
		pc = (char*)p->payload;
		pc[p->tot_len] = '\0';

		char ipAddr[16] = {0};
//		ip_addr_t _ipaddr;
//		ip4_addr_set_u32(&_ipaddr, gtUser.nIpAddr);
		ipaddr_ntoa_r(&pcb->local_ip, ipAddr, 16);
		if(!strncmp(pc, "GET /"UPNP_Root_URL, strlen("GET /"UPNP_Root_URL)))
		{
			nSendSize = sprintf(MsgFmtXML, sendRoot, UPNP_DEVICETYPE, gtUser.strDeviceName, UPNP_MANUFACTURER, UPNP_MANUFACTURER_URL, UPNP_MODELDESCRIPTION, UPNP_MODELNAME, UPNP_MODELNUMBER, ipAddr, gtUser.nRtspPort, UPNP_SERIALNUMBER, gtUser.strDeviceId, ipAddr, UPNP_Sub1_URL);
			sprintf(sendBuf, HTTP200OK_XML"%s", nSendSize, MsgFmtXML);
			tcp_write(pcb, sendBuf, strlen(sendBuf), 1);
		}
		else if(!strncmp(pc, "GET /"UPNP_Sub1_URL, strlen("GET /"UPNP_Sub1_URL)))
		{
			nSendSize = sprintf(MsgFmtXML, sendService, ipAddr, gtUser.nRtspPort, H264URL, ipAddr, gtUser.nRtspPort, JPEGURL);
			sprintf(sendBuf, HTTP200OK_XML"%s", nSendSize, MsgFmtXML);
			tcp_write(pcb, sendBuf, strlen(sendBuf), 1);
		}
		else
		{
			dprintf("UPNP(T) : GET error. Drop... AcceptAddr(%X)\r\n", pcb->local_ip.addr);
		}
		pbuf_free(p);
	}
	return ERR_OK;
	UNUSED(arg);
}

static err_t upnpd_send_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	return upnpd_send_close(tpcb);
	UNUSED(arg);
	UNUSED(len);
}

static err_t upnpd_send_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_MIN);
	tcp_sent(pcb, upnpd_send_sent);
	tcp_recv(pcb, upnpd_send_recv);
	tcp_err(pcb, NULL);
	tcp_poll(pcb, NULL, 3);
	return ERR_OK;
	UNUSED(arg);
	UNUSED(err);
}

static void upnpd_send_init(void)
{
	if(upnpd_tcp_pcb == NULL)
	{
		upnpd_tcp_pcb = tcp_new();
		if(upnpd_tcp_pcb != NULL)
		{
			err_t ert = tcp_bind(upnpd_tcp_pcb, IP_ADDR_ANY, UPNP_LOCAL_PORT);
			if(ert != ERR_OK)
			{
				dprintf("ERROR(%s(),%d) : upnp tcp_bind fail : error code(%d)\r\n", __func__, __LINE__, ert);
			}
			else
			{
				upnpd_tcp_pcb_li = tcp_listen(upnpd_tcp_pcb);
				if(upnpd_tcp_pcb_li != NULL)
				{
					tcp_accept(upnpd_tcp_pcb_li, upnpd_send_accept);
				}
				else
				{
					dprintf("ERROR(%s(),%d) : malloc fail upnpd_tcp_pcb_li\r\n", __func__, __LINE__);
					ert = tcp_close(upnpd_tcp_pcb);
					if(ert != ERR_OK)
					{
						dprintf("ERROR(%s(),%d) : tcp_close fail upnpd_tcp_pcb\r\n", __func__, __LINE__);
					}
				}
			}
		}
		else
		{
			dprintf("ERROR(%s(),%d) : malloc fail upnpd_tcp_pcb\r\n", __func__, __LINE__);
		}
	}
	else
	{
		dprintf("ERROR(%s(),%d) : use upnpd_tcp_pcb\r\n", __func__, __LINE__);
	}
}

void upnpd_init(void *pvParameters)
{
	network_check_netif();

	int nAdverState = -1;

	vTaskDelay(50);

#if 1
	dprintf("UPNPD Start\r\n");

	nAdverState = upnpd_advertisement_init();
//	upnpd_msearch_init();
	upnpd_send_init();

	dprintf("UPNPD Init OK\r\n");

	for(;;)
	{
		if(nAdverState)
		{
			upnpd_advertisement_send(upnpd_udp_pcb);
		}
		vTaskDelay(100);
	}

	IGMP_Leave("239.255.255.250");
	
#endif
	vTaskDelete(NULL);

//	sys_thread_new("Upnp_Udp", UpnpUdpTask, &gtMyNetIf, UPNPUDPD_STACK_SIZE, UPNPUDPD_TASK_PRIO);
//	sys_thread_new("Upnp_Tcp", UpnpTcpTask, &gtMyNetIf, UPNPTCPD_STACK_SIZE, UPNPTCPD_TASK_PRIO);
	UNUSED(pvParameters);
}
#endif
