#ifndef __ETHIF_H__
#define __ETHIF_H__

#include <lwip/netif.h>
#include <lwip/err.h>

err_t 
ethif_init(struct netif *netif);

void
ethif_input(void *arg);

#endif
