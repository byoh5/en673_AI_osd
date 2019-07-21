#ifndef _NETWORKIF_H_
#define _NETWORKIF_H_

#include "msg.h"
#include "lwip/tcpip.h"

//*************************************************************************************************
// Macro
//-------------------------------------------------------------------------------------------------
typedef struct {
	char ifname[12];
	struct netif *_netif;
	xSemaphoreHandle xrx_sem;
	NETIFLIST iftype;
} netif_state_t;

typedef	enum {
	eIe8021d_IeeeStd8021DBridgeGroup,
	eIe8021d_IeeeMacSpecificControlProtocolsGroup0,
	eIe8021d_IeeeStd8023SlowProtocolsMulticast,
	eIe8021d_NearestNonTPMRBridgeGroup,
	eIe8021d_IeeeMacSpecificControlProtocolsGroup1,
	eIe8021d__Reserved0,
	eIe8021d__Reserved1,
	eIe8021d_MetroEthernetForumELMIProtocolGroup,
	eIe8021d_ProviderBridgeGroup,
	eIe8021d__Reserved2,
	eIe8021d__Reserved3,
	eIe8021d__Reserved4,
	eIe8021d__Reserved5,
	eIe8021d_ProviderBridgeMVRP,
	eIe8021d_IndividualLanScopeGroup_NearestBridgeGroup,
	eIe8021d_ReservedForFutureStandardization,
}IEEE8021D_DATA;

typedef	union {
	ULONG a;
	struct {
		UINT use            : 1;
        UINT _res           : 7;
        UINT igmpadr        :24;
	};
}IGMP_DATA;

#define IGMP_LIST_LENS 32

#define wltx_init_lock()    vSemaphoreCreateBinary(xtxwl_sem)
#define wltx_lock()         xSemaphoreTake(xtxwl_sem, portMAX_DELAY)
#define wltx_unlock()       xSemaphoreGive(xtxwl_sem)

#define wlrx_init_lock()    vSemaphoreCreateBinary(xrxwl_sem)
#define wlrx_lock()         xSemaphoreTake(xrxwl_sem, portMAX_DELAY)
#define wlrx_unlock()       xSemaphoreGive(xrxwl_sem)

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern err_t low_level_init(struct netif *netif, char name0, char name1);
extern void if_input(void *ctx);
extern void network_check_netif(void);
extern void network_default_netif(void);
extern void network_default_netif_get_ip(UINT *pIP);
extern void startLwip(void *arg);
extern err_t IGMP_Join(char *strIP);
extern err_t IGMP_Leave(char *strIP);

//-------------------------------------------------------------------------------------------------
// Variable
#if defined(__ETH__)
extern struct netif gtMyNetIf;	// Ethernet
#endif

#if defined(__WIFI__)
extern struct netif gtwlif_sta;// Wi-Fi STA mode
extern struct netif gtwlif_uap;// Wi-Fi UAP mode

extern xSemaphoreHandle xwlif_evt_sem;
#endif

extern xSemaphoreHandle xtxwl_sem;
extern xSemaphoreHandle xrxwl_sem;

extern IGMP_DATA igmp_list[];

extern netif_state_t netif_state[];

#endif // _NETWORKIF_H_
