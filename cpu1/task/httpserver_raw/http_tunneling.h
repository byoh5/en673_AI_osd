#ifndef __HTTPD_TUNNELING_H__
#define __HTTPD_TUNNELING_H__

void http_tunnel_init(void);

int handle_http_Tunneling(UINT method, const char *uri, struct pbuf *p, struct tcp_pcb *pcb);

#endif /* __HTTPD_TUNNELING_H__ */