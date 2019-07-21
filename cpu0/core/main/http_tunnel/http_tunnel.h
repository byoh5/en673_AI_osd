#ifndef __MSG_TUNNEL_H__
#define __MSG_TUNNEL_H__



void
tunnel_port_init(void);

void
tunnel_port_del(void *pcb);

inline int 
tunnel_check(u16 src_port, u16 dst_port);

void
tunnel_get_pcb(tunnel_info *arg);

#endif //__MSG_TUNNEL_H__
