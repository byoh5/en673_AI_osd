
#include "enx_gsoap_http.h"
#include "lwip/pbuf.h"
#include <stdio.h>
#include "httpd.h"
#include "wsseapi.h"

#define ONVIF_HTTP_HDR   "Content-Type: application/soap+xml; charset=utf-8\r\n"\
                         "Content-Length: %d\r\n"\
                         "Connection: close\r\n\r\n"

static const tOnvif g_psConfigOnvifs = {
	.pfnHttp2OnvifHandler = enx_http2onvif,
	.pfnOnvif2HttpHandler = enx_onvif2http
};

extern xSemaphoreHandle xOnvifTaskSem;
extern xSemaphoreHandle xOnvifhttpRxSem;
extern xSemaphoreHandle xOnvifhttpTxSem;
extern struct soap * gsoap;

/**
    .Caution : need error handler
    @param payload : points to struct pbuf *
    @param wr_pose : current write pointer for post data 
    @parm  n       :  contents size of post message
*/
void
enx_http2onvif(onvif_http_t *onvif_http, void *pbuf, int wr_pos, size_t n, int securityClear)
{dvprintf("enx_http2onvif\r\n");
    struct pbuf *p = (struct pbuf*)pbuf;
    struct pbuf *q = p;
    int length = 0;
    char *post_payload;
    dvprintf("%s %d n %d wr_pos %d onvif_http->rx_len %d\r\n",__func__,__LINE__,n,wr_pos,onvif_http->rx_len);
    if(wr_pos == 0)
    {        
		//printf("onvifalloc ");
		if(n>0)
		{
        if((onvif_http->rxbuf = pvPortMalloc(n)))
        {                      
            post_payload = onvif_http->rxbuf;
            while(q!=NULL)
            {
					int len;

					if( q->len > n - length)
					{
						dvprintf("A q->len(%d) > n(%d) - onvif_http->rx_len(%d)!!!!!\r\n",q->len, n, onvif_http->rx_len);
						len = n-length;
            }
					else
					{
						len = q->len;
					}

					if(len != 0)
					{					
						if(length+len > n)
							printf("length(%d)+len(%d) > n (%d) \r\n",length,len,n);
						
						memcpy(post_payload+length, (char*)q->payload, len/*q->len*/); 
						length += len;/*q->len*/;       
                  
}else
						printf("len == 0\r\n");
					q = q->next;
				}

            onvif_http->rx_len = length;    
        }
        else
            onvif_http->rx_len = 0;
		}else
			printf("n == 0\r\n");
    }
    else
    {
		if(n>0)
		{
			if(onvif_http->rxbuf != NULL)
			{
        post_payload = onvif_http->rxbuf +  onvif_http->rx_len;
        while(q!=NULL)
        {
					int len;
					if( q->len > n - onvif_http->rx_len - length)
					{
						dvprintf("B q->len(%d) > n(%d) - onvif_http->rx_len(%d)!!!!!\r\n",q->len, n, onvif_http->rx_len);
						len = n - onvif_http->rx_len - length;
					}
					else
					{
						len = q->len;
					}
					
					if(len != 0)
					{
						if(length+len > n)
							printf("length(%d)+len(%d) > n (%d) \r\n",length,len,n);
						
						memcpy(post_payload+length, (char*)q->payload, len/*q->len*/); 
						length += len/*q->len*/;  
					}     
					else
						printf("len == 0\r\n");
            q = q->next;
        }
        onvif_http->rx_len += length;
			}
		}else
			printf("n == 0\r\n");
    }  

    //we finished that onvif post data received.	
    if(n <= onvif_http->rx_len)
    {
    	if(onvif_http)
    	{
			dvprintf("%s %d \r\n",__func__,__LINE__);
    		struct soap* soap = enx_soap_start(onvif_http);
			dvprintf("%s %d \r\n",__func__,__LINE__);
    		if(soap)
    		{
				dvprintf("%s %d \r\n",__func__,__LINE__);
    			if(securityClear)
    			{
    				dvprintf("\n Wsse Clear Security \r\n");
		    		soap_wsse_ClearSecurity(soap);
				}
				else	
				{
					dvprintf("%s %d \r\n",__func__,__LINE__);
					soap_wsse_DefaultSecurity(soap);
				}

				dvprintf("%s %d \r\n",__func__,__LINE__);
    		    enx_soap_serv(soap);
				dvprintf("%s %d \r\n",__func__,__LINE__);
    		}
			dvprintf("%s %d \r\n",__func__,__LINE__);
    	}
    }
	dvprintf("%s E\r\n",__func__);    
	return;
}

/*
    gsoap recv function.
*/

size_t
enx_http2recv(struct soap *soap, const char *s, size_t n)
{ dvprintf("enx_http2recv\r\n");
    int len=0;
	    onvif_http_t *onvif_http = soap->user;
		if(onvif_http){
			dvprintf("%s %d onvif_http->rx_len %d n %d \r\n",__func__,__LINE__,onvif_http->rx_len,n);
		    
		    if(onvif_http->rx_len == 0) 
		        return 0;
			
		    if(onvif_http->rx_len > n)
		        len = n;
		    else
		        len = onvif_http->rx_len;
			if(onvif_http->rxbuf)
			{
				dvprintf("onvif_http->rxbuf\r\n");
			    memcpy(s,onvif_http->rxbuf,len);
				
			    vPortFree(onvif_http->rxbuf);
			    onvif_http->rxbuf = 0;
			    onvif_http->tx_len = 0;
			}
			else
			{
				dvprintf("onvif_http->rxbuf NULL\r\n");
				return 0;
			}
		}
    return len;

}

/**
    gsoap calls this function to send results to httpd
*/
int
enx_http2send(struct soap *soap, const char *s, size_t n)
{dvprintf("enx_http2send\r\n");
    onvif_http_t *onvif_http = soap->user;   
    char *p =s;
    p[n] =0;
	
    if(!onvif_http->tx_len)
    {
        if((onvif_http->txbuf = pvPortMalloc(soap->count)))
            onvif_http->p = onvif_http->txbuf;
    }
    else//ksjdebug
    {
    	onvif_http->p = onvif_http->txbuf;
    }
	
    if(onvif_http->txbuf && n)
    {       
        memcpy(onvif_http->p,s,n);
        onvif_http->p += n;
        onvif_http->tx_len += n;
    }
    
    if(onvif_http->tx_len >= soap->count)
    {
    	onvif_http->fault = soap->error;
        dvprintf("############ txlen %d, soap->count %d\r\n",onvif_http->tx_len,soap->count);
        //xSemaphoreGive(xOnvifhttpTxSem);
    }       
	dvprintf("%s %d E\r\n",__func__,__LINE__);
    return SOAP_OK;
}

/**
    gsoap calls this function to send results to httpd
*/
int
enx_soapsend(struct soap *soap, const char *s, size_t n)
{dvprintf("enx_soapsend\r\n");
	int nwritten;
	nwritten = write(soap->sendfd, s, (unsigned int)n);

    return SOAP_OK;
}

/*
    gsoap recv function.
*/

size_t
enx_soaprecv(struct soap *soap, const char *s, size_t n)
{ dvprintf("enx_soaprecv\r\n");
	int r;
	r = read(soap->recvfd, s, (unsigned int)n);

    return n;

}

/**
    gsoap send function
    @param s
        s should be free after used
*/	
void
enx_onvif2http(onvif_http_t *onvif_http, char **s, size_t *n)
{dvprintf("enx_onvif2http\r\n");
	//printf("\n\r%s %d priv %p\r\n",__func__,__LINE__,onvif_http->priv);
//	if(xSemaphoreTake(xOnvifhttpTxSem, portMAX_DELAY))
	{
		if(onvif_http->tx_len)
		{
			sprintf(onvif_http->hdr,ONVIF_HTTP_HDR,onvif_http->tx_len);
			*s = onvif_http->txbuf;
			*n = onvif_http->tx_len;
		}
		else
		{
			*s = NULL;
			*n = 0;
		}
	}
	if(onvif_http->priv)
	{	 
		enx_soap_end(onvif_http->priv);
	}
	dvprintf("%s %d E\r\n",__func__,__LINE__);
}

void
enx_onvif2Delete(onvif_http_t *onvif_http)
{dvprintf("enx_onvif2Delete\r\n");
    //printf("\n\r%s %d priv %p\r\n",__func__,__LINE__,onvif_http->priv);
	{
	    if(onvif_http->tx_len)
	    {	        
	    	if(onvif_http->txbuf)
	        	vPortFree(onvif_http->txbuf);
	        onvif_http->tx_len =0; 
	    }
	}
	if(onvif_http->priv)
	{    
	    enx_soap_end(onvif_http->priv);
	}
	//printf("%s %d E\r\n",__func__,__LINE__);
}

/**
    gsoap send function
    @param s
        s should be free after used
*/
int
enx_onvifCheckSecurity(onvif_http_t *onvif_http)
{	dvprintf("enx_onvifCheckSecurity\r\n");
	if(onvif_http->priv)
	{
		struct soap *psoap = onvif_http->priv;
		//printf("enx_onvifCheckSecurity psoap\r\n");
		return soap_wsse_CheckSecurity(psoap);
	}
	dvprintf("enx_onvifCheckSecurity -1\r\n");
	return -1;
}

int
enx_onvifDeleteSecurity(onvif_http_t *onvif_http)
{	dvprintf("enx_onvifDeleteSecurity\r\n");
	if(onvif_http->priv)
	{
		struct soap *psoap = onvif_http->priv;
		soap_wsse_delete_Security(psoap);
		return 0;
	}
	return -1;
}

int
enx_http2close(struct soap *soap)
{dvprintf("enx_http2close\r\n");
    dvprintf("%s %d \n",__func__,__LINE__);
	//systemProcessFunction();
    return SOAP_OK;

	UNUSED(soap);
}

int
enx_http2disconnect(struct soap *soap)
{dvprintf("enx_http2disconnect\r\n");
	dvprintf("%s %d \n",__func__,__LINE__);

    return SOAP_OK;

	UNUSED(soap);
}

int
enx_http2shutdownsocket(struct soap *soap)
{dvprintf("enx_http2shutdownsocket\r\n");
    dvprintf("%s %d \n",__func__,__LINE__);

    return SOAP_OK;

	UNUSED(soap);
}
void onvif_init(void)
{dvprintf("onvif_init\r\n");
	// Pass our CGI handlers to the HTTP server.
	http_set_onvif_handlers(&g_psConfigOnvifs);
}

