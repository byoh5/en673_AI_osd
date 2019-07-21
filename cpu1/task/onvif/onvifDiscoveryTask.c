
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <lwip/netif.h>
#include <enx_gsoap_lwip.h>

#include "dev.h"
#include "onvifProfile.h"
#include "enx_gsoap_http.h"
#include "enx_gsoap_lwip.h"

#include "lwip/tcpip.h"
#include "lwip/netdb.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "discovery.nsmap"

#if 0
#define ENTER() printf(" IN %04d:%s\r\n", __LINE__, __func__);
#define LEAVE() printf("OUT %04d:%s\r\n", __LINE__, __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#define DISCOVERY_URL "soap.udp://"DISCOVERY_IP":3702"
#define DISCOVERY_ACTION "http://schemas.xmlsoap.org/ws/2005/04/discovery/Hello"//"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello"
#define DISCOVERY_BYE_ACTION "http://schemas.xmlsoap.org/ws/2005/04/discovery/Bye"//"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello"


/**
 * @brief hello_discovery 
 * @This function send message contains a hello ws-discovery.
 *   
 */
void 
hello_discovery(void *pvParameters)
{
	ENTER();

	struct soap *soap;	

	network_check_netif();

	vTaskDelay(200);

	dvprintf("hello_discovery START \r\n");

	if(GetSysInfo()->lan_config.net.discovery_mode != 0) {
		LEAVE();
		vTaskDelete(NULL);
	}

	soap = soap_new();

	if(soap != NULL)
	{
		int ret2;

		dvprintf("hello s\r\n");

		soap_init1(soap, SOAP_IO_UDP);
		soap_set_namespaces(soap, wdd_namespaces); 	
		
		soap->fopen = enx_udp_connect;
		soap->fsend = enx_fsend;
		soap->faccept = enx_tcp_accept;
		soap->fclose  = enx_tcp_disconnect;
		soap->fclosesocket = enx_tcp_closesocket;
		soap->fshutdownsocket = enx_tcp_shutdownsocket;
		soap->frecv = enx_frecv;
		soap->fpoll = NULL;
		soap->fmalloc = NULL;//enx_soap_malloc;	
		//server_udp = create_connect_socket_udp();
		soap->errmode = 0;

		soap->socket = SOAP_INVALID_SOCKET;//server_udp;
		soap->master = SOAP_INVALID_SOCKET;//server_udp;
		//IGMP_Join("239.255.255.250");

		//vTaskDelay(200);

		int trycnt = 0;
		while(trycnt<6){
			dvprintf("soap_send___wsdd__Hello\r\n");
			ret2 = soap_send___wsdd__Hello(soap,DISCOVERY_URL, DISCOVERY_ACTION, get__wsdd__Hello(soap));
			vTaskDelay(1);
			trycnt++;
		}
		dvprintf("hello_discovery result %d \r\n",ret2);
		
		soap_destroy(soap);
		soap_end(soap);
		soap_done(soap);
		soap_free(soap);	
		dvprintf("hello e\r\n");
	}
	//while(1)
	//	vTaskDelay(1);

	LEAVE();
	vTaskDelete(NULL);
	UNUSED(pvParameters);
}

/**
 * @brief bye_discovery 
 * @This function send message contains a bye ws-discovery.
 *   
 */
void 
bye_discovery(void)
{
	ENTER();

	network_check_netif();

	if(GetSysInfo()->lan_config.net.discovery_mode != 0) {
		LEAVE();
		return;
	}

	int server_udp;		
	struct soap *soap;	
	soap = soap_new();

	int ret2;
	soap_init1(soap, SOAP_IO_UDP);
	soap_set_namespaces(soap, wdd_namespaces); 	
	
	soap->fopen = enx_udp_connect;
	soap->fsend = enx_fsend;
	soap->faccept = enx_tcp_accept;
	soap->fclose  = enx_tcp_disconnect;
	soap->fclosesocket = enx_tcp_closesocket;
	soap->fshutdownsocket = enx_tcp_shutdownsocket;
	soap->frecv = enx_frecv;
	soap->fpoll = NULL;
	soap->fmalloc = NULL;//enx_soap_malloc;	
	server_udp = create_connect_socket_udp();
	soap->errmode = 0;

	soap->socket = server_udp;
	soap->master = server_udp;
	//IGMP_Join("239.255.255.250");

	vTaskDelay(200);

	int trycnt = 0;
	while(trycnt<3){		
		dvprintf("soap_send___wsdd__Bye\r\n");
		ret2 = soap_send___wsdd__Bye(soap, DISCOVERY_URL, DISCOVERY_BYE_ACTION, get__wsdd__Bye(soap));
		trycnt++;
	}
	dvprintf("bye_discovery result %d \r\n",ret2);

	soap_destroy(soap);
	soap_end(soap);
	soap_done(soap);
	soap_free(soap);

	LEAVE();
}


/**
 * @brief discovery_task
 * @This function receive multicast messages contains a ws-discovery.
 *   		 
 */
void 
discovery_task(void *pvParameters)
{
	network_check_netif();

	int server_udp;	

	int retval=0;
	int retbind=0;
	struct soap *soap_udp;	
	
	if(GetSysInfo()->lan_config.net.discovery_mode == 0)
		sys_thread_new("wsddhello",hello_discovery, NULL, ONVIF_STACK_SIZE, DISCOVERY_TASK_PRIO);


	soap_udp = soap_new();	
	soap_init1(soap_udp, SOAP_IO_UDP);	
	soap_set_namespaces(soap_udp, wdd_namespaces);	
	
	soap_udp->faccept = NULL;//enx_tcp_accept;
	soap_udp->fopen   =  NULL;//enx_tcp_connect;
	soap_udp->fclose  = enx_tcp_disconnect;
	//soap_udp->fdisconnect = enx_tcp_disconnect;
	soap_udp->fclosesocket = enx_tcp_closesocket;
	soap_udp->fshutdownsocket = enx_tcp_shutdownsocket;
	soap_udp->fsend = enx_fsend;
	
	soap_udp->frecv = enx_frecv;
	soap_udp->fpoll = NULL;
	soap_udp->fmalloc = NULL;//enx_soap_malloc;

	server_udp = SOAP_INVALID_SOCKET;
	server_udp = create_server_socket_udp();

	retbind = bind_server_udp1(server_udp);
	
	soap_udp->master = server_udp;
	soap_udp->socket = server_udp;
	
	soap_udp->errmode = 0;
	soap_udp->bind_flags = 1;	
	soap_udp->user				= NULL;

	while(1){

		if(soap_udp != NULL)
		{			

			dvprintf("wsdd s\r\n");

			dvprintf("b%d ",retbind);
			retval = soap_serve(soap_udp); 
			dvprintf("sv%d ",retval);
			soap_destroy(soap_udp);		 
			soap_end(soap_udp);

		}
		vTaskDelay(10);
	}
	soap_destroy(soap_udp);
	soap_end(soap_udp);
	//soap_done(soap_udp);
	soap_free(soap_udp);
	soap_udp = NULL;			
	
	dvprintf("wsdd e\r\n");

	UNUSED(pvParameters);
}
