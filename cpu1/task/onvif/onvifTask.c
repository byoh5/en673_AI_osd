
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <lwip/netif.h>
#include <enx_gsoap_lwip.h>
#include "onvif.nsmap"
#include "onvifProfile.h"
#include "enx_gsoap_http.h"

//struct soap *static_soap;
//xSemaphoreHandle xOnvifTaskSem;
//xSemaphoreHandle xOnvifhttpRxSem;
//xSemaphoreHandle xOnvifhttpTxSem;

struct soap * gsoap;

struct soap * enx_soap_start(void *ctx)
{
	struct soap * soap;    
	soap = soap_new();
	onvif_http_t *ponvif_http = ctx;
		
	if(soap)
	{
		soap_init(soap);
		soap_set_namespaces(soap, namespaces);
		soap->faccept			= NULL;
		soap->fopen 			= NULL;
		soap->fclose			= NULL;
		soap->fdisconnect		= enx_http2close;
		soap->fclosesocket		= enx_http2close;	// type warning!!!
		soap->fshutdownsocket	= enx_http2close;	// type warning!!!
		soap->fsend 			= enx_http2send;
		soap->frecv 			= enx_http2recv;	// type warning!!!
		soap->fpoll 			= NULL;
		soap->fmalloc			= NULL;
		soap->master			= 0;
		soap->socket			= 0;
		soap->user				= ctx;
		ponvif_http->priv		= soap;
	}
	else
		printf("%s, err \n",__func__);	 
	return soap;
}

void
enx_soap_serv(void *ctx)
{
    //printf("enter %s ctx %p\r\n",__func__,ctx);
    if(ctx)
        soap_serve(ctx);
    //printf("leave %s \r\n",__func__);
}


void
enx_soap_end(void *ctx)
{
    //printf("enter %s %d \r\n",__func__);
    struct soap * soap = ctx;
    if(ctx)
    {
        soap->user = NULL;
        soap_destroy(soap);	
        soap_end(soap);
        soap_done(soap);
        soap_free(soap);
        //soap_free(ctx);
    }
        
//    printf("leave %s\r\n",__func__);
}



/**
 * @brief onvif_task
 * @This function receive messages contain a onvif protocol and implement a request.
 *   
 */
#if 0
int 
onvif_task(void *pvParameters)
{	struct netif* ptNetIf = (struct netif*)pvParameters;
	while(!netif_is_up(ptNetIf))	// Network is ready ?
	{
			vTaskDelay(10);
	}
	dvprintf("%s\r\n",__func__);
//	printf("onvif_task \r\n");

#if 0	
//	static_soap = NULL;
	
	static_soap = soap_new();
	if(static_soap == NULL)
	{
		printf(" ONVIF ONVIF_ALLOC NULL!!!!!!!!! \r\n");
		return NULL;
	}

	soap_init(static_soap);
	soap_set_namespaces(static_soap, namespaces);
	
	static_soap->faccept 			= NULL;
	static_soap->fopen				= NULL;
	static_soap->fclose				= enx_http2close;
	static_soap->fdisconnect		= enx_http2disconnect;
	static_soap->fclosesocket 		= NULL;
	static_soap->fshutdownsocket	= enx_http2shutdownsocket;
	static_soap->fsend 				= enx_http2send;
	static_soap->frecv 				= enx_http2recv;
	static_soap->fpoll 				= NULL;
	static_soap->fmalloc 			= NULL;//enx_soap_malloc;
	static_soap->master 			= 0;
	static_soap->socket 			= 0;

	
	init_onvif_profiles(static_soap);
		
	vSemaphoreCreateBinary(xOnvifTaskSem);
	vTaskDelay(10);
	dvprintf("onvif_task e!! \r\n");

	while(1);

	soap_destroy(static_soap);
	soap_end(static_soap);
	soap_done(static_soap);
	soap_free(static_soap);
#else

	init_onvif_profiles();
	vSemaphoreCreateBinary(xOnvifTaskSem);
	vSemaphoreCreateBinary(xOnvifhttpRxSem);	
	vSemaphoreCreateBinary(xOnvifhttpTxSem);
	xSemaphoreTake(xOnvifhttpRxSem, portMAX_DELAY);
	xSemaphoreTake(xOnvifhttpTxSem, portMAX_DELAY);
	
	dvprintf("onvif_task e!! \r\n");
	
	//vTaskDelay(1000);

	struct soap * tgsoap;
	gsoap = soap_new();
	soap_init(gsoap);
	soap_set_namespaces(gsoap, namespaces);

	while(1)
	{	
		tgsoap = soap_copy(gsoap);
		if(tgsoap == NULL)
		{
			printf(" ONVIF ONVIF_ALLOC NULL!!!!!!!!! \r\n5");
			vTaskDelay(1);
			continue;
		}
	
		gsoap->user = tgsoap;
	
		tgsoap->faccept			= NULL;
		tgsoap->fopen			= NULL;
		tgsoap->fclose			= enx_http2close;
		tgsoap->fdisconnect		= enx_http2close;
		tgsoap->fclosesocket 	= enx_http2close;
		tgsoap->fshutdownsocket	= enx_http2close;
		tgsoap->fsend			= enx_http2send;
		tgsoap->frecv			= enx_http2recv;
		tgsoap->fpoll			= NULL;
		tgsoap->fmalloc			= NULL;//enx_soap_malloc;
		tgsoap->master			= 0;
		tgsoap->socket			= 0;
	
		xSemaphoreGive(xOnvifTaskSem);

		soap_serve(tgsoap); 
		tgsoap->user = NULL;	
		gsoap->user = NULL;
	
		soap_destroy(tgsoap);	
		soap_end(tgsoap);
		soap_done(tgsoap);
		soap_free(tgsoap);
		
		//printf(".\r\n");
		//vTaskDelay(1);
	}
	

	
	dvprintf("onvif_task free \r\n");

#endif
}
#endif

