#include "dev.h"

#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "ethif.h"
#include "open_eth.h"
#include "msg.h"
#include "enx_lwip_helper.h"

#ifdef __ETH__
static struct netif eth_netif;
void enx_eth_helper_init(void)
{
	listNetif[enlETHERNET] = &eth_netif;

	memset(listNetif[enlETHERNET], 0, sizeof(struct netif));

	netif_add(listNetif[enlETHERNET], NULL, NULL, NULL, NULL, ethif_init, ethernet_input);

	IntAdd(IRQ_ETH, (void*)isr_oeth_rx, NULL);
	IrqEnable(IRQ_ETH);
}

/**
 *  This function poll all netifs registerd with ewl.
 *  and receive rx packet from wifi and hand over lwip
 *
 *  here, lwip handles rx packet from WiFi
*/
ISRT void enx_eth_poll_netif(void)
{
	ethif_input(listNetif[enlETHERNET]);
#if (NET_TX_BUFFER==1)
	enx_eth_netif_tx();
#endif
}
#endif
