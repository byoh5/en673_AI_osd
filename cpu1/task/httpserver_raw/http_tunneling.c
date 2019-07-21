#include "dev.h"
#include "dev_inline.h"

#include "msg.h"

#if (RTSPoverHTTP==1)

#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"

#include "http_tunneling.h"
#include "share_rtspoverhttp.h"

#define ENX_TUNNEL_DGB_COMMAND		DEF_OFF
#define ENX_TUNNEL_DGB_PROCESSING	DEF_OFF
#define ENX_TUNNEL_DGB_RESPONSE		DEF_OFF
#define ENX_TUNNEL_DGB_DEBUG		DEF_OFF
#define ENX_TUNNEL_DGB_ERROR		DEF_ON
#define ENX_TUNNEL_DGB_ENTEXT		DEF_OFF

#define tprintf(debug, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#if (ENX_TUNNEL_DGB_ENTEXT==DEF_ON)
#define tunnel_enter()	printf("%04d %s : ENTER\r\n", __LINE__, __func__);
#define tunnel_exit()	printf("%04d %s : EXIT\r\n", __LINE__, __func__);
#else
#define tunnel_enter()
#define tunnel_exit()
#endif

#define MAX_TUNNELING             RTSP_info_MAX

typedef struct {
	int is_valid;			// 1 = vaild, 0 = empty
	char sessionCookie[80];
} tunneling_t;

static tunneling_t rt_tunneling[MAX_TUNNELING];

void http_tunnel_init(void)
{
	tunnel_enter();
	memset(rt_tunneling, 0, sizeof(rt_tunneling));
	tunnel_exit();
}

static tunneling_t *http_tunnel_find(char const* sessionCookie)
{
	tunnel_enter();

	int i = 0;
	tunneling_t *p_tunnel = rt_tunneling;

	for(i=0; i<MAX_TUNNELING; i++)
	{
		if(p_tunnel[i].is_valid && !strcmp(p_tunnel[i].sessionCookie, sessionCookie))
		{
			tprintf(ENX_TUNNEL_DGB_PROCESSING, "find(idx:%d)\r\n", i);
			tunnel_exit();
			return &p_tunnel[i];
		}
	}

	tunnel_exit();
	return NULL;
}

static int http_tunnel_add(char const* sessionCookie)
{
	tunnel_enter();

	int i = 0;
	tunneling_t *p_tunnel = rt_tunneling;

	for(i=0; i<MAX_TUNNELING; i++)
	{
		if(!p_tunnel[i].is_valid)
		{
			strcpy(p_tunnel[i].sessionCookie, sessionCookie);
			p_tunnel[i].is_valid = 1;
			tprintf(ENX_TUNNEL_DGB_PROCESSING, "add(idx:%d)\r\n", i);
			tunnel_exit();
			return DEF_OK;
		}
	}

	tunnel_exit();
	return DEF_FAIL;
}

/**

  @param method 
          0 = HTTP GET CMD PCB   for TUNNELING
          1 = HTTP POST CMD PCB    "

  @param pcb
         point to struct tcp_pcb
*/
static void http_tunnel_send(TunnelCmdType method, char *cookie, void *pcb, char *uri)
{
	tunnel_enter();

	err_t err;
	tunnel_info tunnel;

	switch(method)
	{
		case eTunnelType_GET:
			err = tcp_send_empty_ack(pcb);
			if(err != ERR_OK)
			{
				tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! empty ack fail(%d)\r\n", err);
			}
			break;
		case eTunnelType_POST:
			break;
	}

	tunnel.cmdtype = method;
	strcpy(tunnel.uri, uri);
	memcpy(tunnel.cookie, cookie, 32);
	memcpy(&tunnel.pcb, pcb, sizeof(struct tcp_pcb));

	int i;
	for (i = 0; i < MSG_TUNNEL_NUM; i++) {
		if (!tunnel_port[i].valid) {
			tunnel_port[i].priv = &tunnel.pcb;
			tunnel_port[i].src = tunnel.pcb.local_port;
			tunnel_port[i].dst = tunnel.pcb.remote_port;
			tunnel_port[i].valid = 1;
			//tprintf(ENX_TUNNEL_DGB_DEBUG, "index(%d)\r\n", i);
			break;
		}
	}

	while(MsgTunnelPut(&gptMsgTunnel, &tunnel) == DEF_FAIL)
		vTaskDelay(1);

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_RTSPoverHTTP, 0) == DEF_FAIL)
		vTaskDelay(1);

	tunnel_exit();
}

int handle_http_Tunneling(TunnelCmdType method, const char *uri, struct pbuf *p, struct tcp_pcb *pcb)
{	// method = GET, POST
	tunnel_enter();

	if (p == NULL) {
		tprintf(ENX_TUNNEL_DGB_PROCESSING, "method(%d) uri(%s) (p == NULL)\r\n", method, uri);
		tunnel_exit();
		return 0;
	}

	char strCookie[40] = {0};
	char *data = p->payload;
	int nLength = 0, left = p->len, nCase = 0, nRes = 0;

#ifdef __H264__
	nCase |= (strcmp(uri, "/"H264URL) == 0) << 1;
#ifdef __H264_2CH__
	nCase |= (strcmp(uri, "/"HSUBURL) == 0) << 2;
#endif
#endif
#ifdef __JPEG__
	nCase |= (strcmp(uri, "/"JPEGURL) == 0) << 3;
#endif

	if(nCase)
	{
		// Look for various headers that we're interested in:
		lookForHeader("x-sessioncookie", data, left, strCookie, 32);
		nLength = strlen(strCookie);
		//tprintf(ENX_TUNNEL_DGB_DEBUG, "uri(%s) method(%s) nLength(%d)\r\n", uri, method == eTunnelType_GET ? "GET" : "POST", nLength);
		//tprintf(ENX_TUNNEL_DGB_DEBUG, "strCookie(%s)\r\n", strCookie);

		if(nLength)
		{
			pcb->nPortId = method;
			switch(method)
			{
				case eTunnelType_GET:
					tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
					nRes = http_tunnel_add(strCookie);
					break;
				case eTunnelType_POST:
					{
						//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
						tunneling_t *p_tunnel = http_tunnel_find(strCookie);
						if(p_tunnel)
						{
							p_tunnel->is_valid = 0;	// delete tunnel entry
							memset(p_tunnel->sessionCookie, 0, 80);
							nRes = DEF_OK;
						}
					}
					break;
				default:
					nRes = 0;
					pcb->nPortId = 0;
					break;
			}

			if(nRes)
			{
				//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
				//give pcb to cpu0
				http_tunnel_send(method, strCookie, pcb, (char *)uri); 
				//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
				// remove http get session

				if(tcp_input_pcb == pcb)
				{
					tcp_input_pcb = NULL;
					tprintf(ENX_TUNNEL_DGB_DEBUG, "!!!!!!!!!!!!!!!\r\n");
				}
				
//				tcp_pcb_remove(&tcp_active_pcbs, pcb);
				tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
			}
			else
			{
				tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! uri(%s) method(%s) strCookie(%s) nLength(%d)\r\n", uri, method == eTunnelType_GET ? "GET" : "POST", strCookie, nLength);
			}
		}
	}

	tunnel_exit();
	return nRes;
}
#endif
