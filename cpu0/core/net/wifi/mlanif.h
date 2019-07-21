#ifndef __NETIF_NRWLANIF_H__
#define __NETIF_NRWLANIF_H__

#include <lwip/netif.h>
#include <lwip/err.h>

void mlanif_input(NETIFLIST iftype);

err_t mlanif_init(struct netif *netif);

#if NO_SYS
void mlanif_poll(struct netif* netif);
#endif

#endif
