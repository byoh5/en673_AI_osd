
#ifndef __EUX_WIFI_HELPER_H__
#define __EUX_WIFI_HELPER_H__


#include "wifi_cfg.h"
/*
    Marvell wifi is initialized. 
    where be called  timer, shell api 
    
    @return :  0 = wifi initialization is success.
              -1 = fail. 
*/
int
enx_wifi_helper_init(void);

int 
enx_wifi_bt_helper_init(void);

/**
 *  This function poll all netifs registerd with ewl.
 *  and receive rx packet from wifi and hand over lwip
 *
 *  here, lwip handles rx packet from WiFi
*/

void 
enx_bt_init_netif(void);
void
enx_wifi_poll_netif(void);

void 
enx_wifi_tx_from_cpu0(void);

void
enx_wifi_sta_start(void);

void 
enx_wifi_ap_start(void);

/**
	lwip net up or down for STA
*/

void wifi_sta_setNet(int on);

extern tWificfg gtWificfg;

extern struct netif* enx_wifi_get_ap_netif();
extern struct netif* enx_wifi_get_sta_netif();

extern struct netif *swap_netif;
extern struct netif *sta_netif;
extern int wificonnection;
extern int bluetoothmode;
#define INQUERY_INTERVAL_TIME 15000//10s
#endif