#ifndef _WIFIIF_H_
#define _WIFIIF_H_

//*************************************************************************************************
// Macro
//-------------------------------------------------------------------------------------------------
// 


//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void network_wlif_sta_start(void);
extern void network_wlif_uap_start(void);
extern err_t igmp_mac_filter_set_mlanif(struct netif *netif, ip_addr_t *group, u8_t action);

//-------------------------------------------------------------------------------------------------
// Variable

#endif // _WIFIIF_H_