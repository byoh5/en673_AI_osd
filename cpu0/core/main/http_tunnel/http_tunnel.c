#include "dev.h"
#include "dev_inline.h"

#include "msg.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"

#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_over_rtsp.h"
#include "rtp_h264.h"

#if (RTSPoverHTTP==1)

#include "http_front.h"
#include "http_tunnel.h"

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

void tunnel_port_init(void)
{
	tunnel_enter();

	int i; 
	for(i=0; i<MSG_TUNNEL_NUM; i++)
	{
		tunnel_port[i].valid = 0;
		tunnel_port[i].src = 0;
		tunnel_port[i].dst = 0;
	}

	tunnel_exit();
}

void tunnel_port_add(struct tcp_pcb *pcb)
{
	tunnel_enter();

	int i;
	for (i = 0; i < MSG_TUNNEL_NUM; i++) {
		if (!tunnel_port[i].valid) {
			tunnel_port[i].priv = pcb;
			tunnel_port[i].src = pcb->local_port;
			tunnel_port[i].dst = pcb->remote_port;
			tunnel_port[i].valid = 1;
			tprintf(ENX_TUNNEL_DGB_DEBUG, "index(%d)\r\n", i);
			break;
		}
	}

	tunnel_exit();
}

void tunnel_port_del(void *pcb)
{
	tunnel_enter();

	int i;
	struct tcp_pcb *ppcb = pcb;
	for (i = 0; i < MSG_TUNNEL_NUM; i++) {
		if (tunnel_port[i].valid) {
			
			if(tunnel_port[i].dst == ppcb->remote_port){
			//if (npcb == pcb) {				

				tunnel_port[i].priv = NULL;			
				tunnel_port[i].src = 0;
				tunnel_port[i].dst = 0;
				tunnel_port[i].valid = 0;
				tprintf(ENX_TUNNEL_DGB_DEBUG, "index(%d)\r\n", i);
			
				break;
			}

		}
	}

	tunnel_exit();
}

#if 0
err_t tunnel_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	tunnel_enter();

	if (p) {
		tcp_recved(pcb, p->len);
		pbuf_free(p);
	}

	tunnel_exit();
	return ERR_OK;
	UNUSED(arg);
	UNUSED(err);
}
#endif

void tunnel_get_pcb(tunnel_info *arg)
{
	tunnel_enter();

	struct tcp_pcb *pcb;
	hs_front_t *front;
	tunnel_info *pmsg_tunnel = arg;

	if (pmsg_tunnel->cmdtype == eTunnelType_POST) { //HTTP POST COMMAND
		tunneling_t *p_tunnel;
		//tprintf(ENX_TUNNEL_DGB_DEBUG, "POST %s\r\n", pmsg_tunnel->cookie);
		//tunnel_port_add(&pmsg_tunnel->pcb);
		//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		front = http_front_alloc();
		//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		if (front != NULL) {
			//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
			reset_request_buffer(front);
			p_tunnel = tunnel_find(front, pmsg_tunnel->cookie);
			//tprintf(ENX_TUNNEL_DGB_DEBUG, "p_tunnel(0x%08X)\r\n", p_tunnel);
			if (p_tunnel) {
				//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
				hs_front_t *prev_front;
				front->p_tunnel = p_tunnel;
				prev_front = (hs_front_t *)p_tunnel->priv;
				front->out_pcb = prev_front->out_pcb;
				//  pre_front don't anymore recevie.
				strcpy((char *)front->urlSuffix, pmsg_tunnel->uri);
				tprintf(ENX_TUNNEL_DGB_DEBUG, "arg(free-mem:0x%08X)\r\n", prev_front);
				free(prev_front);
			} else {
				tprintf(ENX_TUNNEL_DGB_DEBUG, "arg(free-mem:0x%08X)\r\n", front);
				free(front);

				tunnel_exit();

				return;
			}
			pcb = tcp_new();
			if(pcb != NULL)
			{
				front->in_pcb = pcb;
				memcpy(pcb, &pmsg_tunnel->pcb, sizeof(struct tcp_pcb));
				tcp_err(pcb, http_front_err);
				tcp_poll(pcb, NULL, 0);
				tcp_sent(pcb, NULL);
				tcp_arg(pcb, front);
				tcp_recv(pcb, http_front_recv);
				TCP_REG(&tcp_active_pcbs, pcb); 
				rtsp_con_t *prtsp = rtsp_add_user(front->out_pcb);
				front->prtsp_conn = prtsp;
				prtsp->ctx = front;
				tcp_arg(front->out_pcb, prtsp);
				tprintf(ENX_TUNNEL_DGB_DEBUG, "tcp_new rtsp_add_user \r\n");

				tunnel_exit();
				return;
			}        
			else
			{
				tprintf(ENX_TUNNEL_DGB_DEBUG, "tcp_new NULL \r\n");
			}
		}   
		else
		{
			tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! http front alloc fail\r\n");
		}
	} else if (pmsg_tunnel->cmdtype == eTunnelType_GET) {
		pcb = tcp_new();
		if (pcb != NULL) {
			memcpy(pcb, &pmsg_tunnel->pcb, sizeof(struct tcp_pcb));
			//tprintf(ENX_TUNNEL_DGB_DEBUG, "GET(%s) flags(0x%x) STATE(0x%x)\r\n", pmsg_tunnel->cookie, pcb->flags, pcb->state);
			tcp_err(pcb,  http_front_err);
			tcp_poll(pcb, NULL, 0);
			tcp_sent(pcb, NULL);
			tcp_arg(pcb,  NULL);
			tcp_recv(pcb, NULL);
			front = http_front_alloc();
			if (front != NULL) {
				//tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
				front->in_pcb = front->out_pcb = pcb;
				if (tunnel_add(front, pmsg_tunnel->cookie) == DEF_OK) {
					//tunnel_port_add(pcb);
					TCP_REG(&tcp_active_pcbs, (struct tcp_pcb *)pcb);
					handleHTTPCmd_TunnelingGET(front, pmsg_tunnel->cookie);
					http_front_send(front->out_pcb, front);
					tprintf(ENX_TUNNEL_DGB_DEBUG, "GET(%s) flags(0x%x) STATE(0x%x) http_front_send\r\n", pmsg_tunnel->cookie, pcb->flags, pcb->state);
				} else {
					tprintf(ENX_TUNNEL_DGB_ERROR, "tunnel_add fail\r\n");
				}
			} else {
				tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! http front alloc fail\r\n");
			}
		} else {
			tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! tcp new fail\r\n");
		}
	}

	tunnel_exit();
}

/**
  @return
    0 : failid
    1 : found 
*/
inline int 
tunnel_check(u16 src_port, u16 dst_port)
{
//	tunnel_enter();

	int i;
	//tprintf(ENX_TUNNEL_DGB_PROCESSING, "src(%d) dst(%d) - ", src_port, dst_port);
	for(i=0; i<MSG_TUNNEL_NUM; i++)
	{
		//tprintf(ENX_TUNNEL_DGB_PROCESSING, "%dth valid(%d) src(%d) dst(%d) ", i, tunnel_port[i].valid, tunnel_port[i].src, tunnel_port[i].dst);
		if((tunnel_port[i].valid==1) && (tunnel_port[i].src==dst_port) && (tunnel_port[i].dst==src_port))
		{
			//tprintf(ENX_TUNNEL_DGB_PROCESSING, "tunnel ok\r\n");
			return 1;
		}
	}
	//tprintf(ENX_TUNNEL_DGB_PROCESSING, "no tunnel\r\n");

//	tunnel_exit();
	return 0;
}
#endif
