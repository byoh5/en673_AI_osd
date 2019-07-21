#ifndef __EUX_LWIP_HELPER_H__
#define __EUX_LWIP_HELPER_H__

/**
    init lwip stack 
    register lwip timer handlers to timer
*/
void enx_lwip_init(void);
void enx_netif_init(void *ctx);

void enx_poll_netif(void);

extern struct netif *listNetif[3];

#endif