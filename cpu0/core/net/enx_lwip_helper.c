#include "dev.h"
#include "dev_inline.h"

#include "lwip/opt.h"
#include "lwip/init.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/igmp.h"
#include "lwip/tcp_impl.h"
#include "lwip/tcp.h"
#include "lwip/ip_frag.h"

#ifdef __ETH__
#include "enx_eth_helper.h"
#endif

#ifdef __WIFI__
#include "enx_wifi_helper.h"
#endif

struct netif *listNetif[enlEnd] = {NULL};	// 0(ETHIF) 1(Wi-Fi STA) 2(Wi-Fi UAP)

/********************************************************
		Local Functions
********************************************************/

#if LWIP_ARP
//static void enx_etharp_tmr_cb(void *ctx)
//{
//	etharp_tmr();
//	UNUSED(ctx);
//}
#endif

#if LWIP_DHCP
static void enx_dhcp_fine_tmr_cb(void *ctx)
{
	dhcp_fine_tmr();
	UNUSED(ctx);
}

static void enx_dhcp_coarse_tmr_cb(void *ctx)
{
	dhcp_coarse_tmr();
	UNUSED(ctx);
}
#endif

static void enx_tcp_tmr_cb(void *ctx)
{
	tcp_tmr();
	UNUSED(ctx);
}

#if LWIP_IGMP
static void enx_igmp_tmr_cb(void *ctx)
{
	igmp_tmr();
	UNUSED(ctx);
}
#endif

#if IP_REASSEMBLY
static void enx_ip_reass_tmr_cb(void *ctx)
{
	ip_reass_tmr();
	UNUSED(ctx);
}
#endif

/*
	added  arp ,tcp and dhcp timer for lwip
*/
static void enx_lwip_timer(void)
{
/* register lwip timer callbacks for tcp, arp and dhcp protocols */
#if LWIP_ARP
//	timer_sched_timeout_cb(5000, TIMEOUT_PERIODIC, enx_etharp_tmr_cb, NULL);
#endif

	timer_sched_timeout_cb(TCP_TMR_INTERVAL, TIMEOUT_PERIODIC, enx_tcp_tmr_cb, NULL);
#if LWIP_DHCP
	timer_sched_timeout_cb(DHCP_FINE_TIMER_MSECS, TIMEOUT_PERIODIC, enx_dhcp_fine_tmr_cb, NULL);
	timer_sched_timeout_cb(DHCP_COARSE_TIMER_MSECS, TIMEOUT_PERIODIC ,enx_dhcp_coarse_tmr_cb, NULL);
#endif

#if LWIP_IGMP
	timer_sched_timeout_cb(IGMP_TMR_INTERVAL,TIMEOUT_PERIODIC, enx_igmp_tmr_cb, NULL);
#endif

#if IP_REASSEMBLY
	timer_sched_timeout_cb(IP_TMR_INTERVAL, TIMEOUT_PERIODIC, enx_ip_reass_tmr_cb, NULL);
#endif
}

/*
    should call fuctions of timer init before this fuction.
*/
void enx_lwip_init(void)
{
	lwip_init();
	enx_lwip_timer();
}

void enx_netif_init(void *ctx)
{
#ifdef __ETH__
	enx_eth_helper_init();
#endif

#ifdef __WIFI__
#ifdef __BT__
	enx_wifi_bt_helper_init();
#else
	enx_wifi_helper_init();
#endif
#endif
	UNUSED(ctx);
}

/**
 *  This function poll all netifs registerd with ewl.
 *  and receive rx packet from wifi and hand over lwip
 *
 *  here, lwip handles rx packet from WiFi
*/
ISRT void enx_poll_netif(void)
{
#ifdef __ETH__
	enx_eth_poll_netif();
#endif

#ifdef __WIFI__
	enx_wifi_poll_netif();
#endif
}
