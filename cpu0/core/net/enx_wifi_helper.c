#ifdef __WIFI__

/* ------------------------ OR32 includes -------------------------------- */
#include "dev.h"
#include "dev_inline.h"
#include "sdio.h"

#include "lwip/init.h"
#include "mlanif.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "lwip/dhcp.h"
#include "lwip/igmp.h"
#include "lwip/tcp.h"
#include "lwip/tcp_impl.h"
#include "enx_lwip_helper.h"

#include "ewl.h"
#include "wl_tools.h"
#include "wlif_tools.h"
#include "wifi_cfg.h"
#include "msg.h"
#include "open_eth.h"
#include "supplicant_init.h"

tWificfg gtWificfg;

/* external variables */
int tmr_id_uap_conn = -1;

void ewl_sdio_ext_irq_handler(void *arg);

/*****************************************************************************
    enx : Eyenix U2/U3
    Lwip is required for this library
    shell api & Wi-Fi api & timer api is combined to use Wi-Fi marvell
******************************************************************************/

/********************************************************
        Local Functions
********************************************************/

/*
    eux_sta_conn_cb & eux_sta_disconn_cb is called when wifi connect to Wi-Fi AP
    and lwip is up.
*/

/*
    When AP is connected, dhcp client is started.
    and lwip can be up.
*/
static void enx_sta_conn_cb(void *ctx)
{
	struct netif *_netif = ctx;
	netif_set_link_up(_netif);
	xfer_event_to_cpu0(WLIF_EVT, WLIF_CONNECTED, NULL, enlWIFISTA);
}

/*
    When AP is dis_connected, dhcp client is started.
    and lwip can be down.
*/
static void enx_sta_disconn_cb(void *ctx)
{
	struct netif *_netif = ctx;
	netif_set_link_down(_netif);
	xfer_event_to_cpu0(WLIF_EVT, WLIF_DIS_CONNECTED, NULL, enlWIFISTA);
}

static void enx_sta_netif_init(void)
{
	void *state = listNetif[enlWIFISTA]->state;
	netif_add(listNetif[enlWIFISTA], NULL, NULL, NULL, state, mlanif_init, ethernet_input);

#ifdef custom_MAC_STA
	u8 sta_mac[6] = WIFI_STA_MACADDRESS;				// STA mode MAC address
	ewl_set_hwaddr(listNetif[enlWIFISTA], sta_mac);		// STA mode MAC address set
#else
	ewl_get_hwaddr(listNetif[enlWIFISTA], listNetif[enlWIFISTA]->hwaddr);
#endif

	printf("STA MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
		listNetif[enlWIFISTA]->hwaddr[0], listNetif[enlWIFISTA]->hwaddr[1], listNetif[enlWIFISTA]->hwaddr[2],
		listNetif[enlWIFISTA]->hwaddr[3], listNetif[enlWIFISTA]->hwaddr[4], listNetif[enlWIFISTA]->hwaddr[5]);
}

/* 
    called when adding netif to sta mode of wifi is completed.
    register netif to lwip stack.   
*/
static void enx_sta_netif_init_complete(void *ctx)
{
	struct netif *_netif = ctx;

	netif_set_up(_netif);

	ewl_set_conn_cb((int)_netif->state, enx_sta_conn_cb, enx_sta_disconn_cb);
}

#if 1

#if 0
int 
ewlap_bss_reset(void);
void enx_uap_init(void)
{
    ewlap_bss_reset();    
    //ewlap_init_apcfg(gtWificfg.APcfg.strSSID);
    //printf("    
//    switch(gtWificfg.APcfg.un32Authentication)
//    {
//        case wifi_OPEN:
            ewlap_apcfg("SEC", "open");
//            break;
//        case wifi_WPA2_PSK:
//            ewlap_apcfg("SEC", "wpa2-psk");
//            ewlap_apcfg("KEY ", gtWificfg.APcfg.strKey);
//            break;
//    }
    
    char strnum[10];
//    sprintf(strnum, "%d", gtWificfg.APcfg.un8MaxSTA);
//    ewlap_apcfg("MAX_SCB", strnum);
    sprintf(strnum, "%d", 9);
    ewlap_apcfg("CHANNEL", strnum);
    
    //ewlap_get_cfg_11n();
    ewlap_set_apcfg();
}
#endif

/*
    When AP is dis_connected, dhcp client is started.
    And lwip can be down.
*/
static void enx_uap_disconn_cb(void *ctx)
{
	struct netif *_netif = ctx;
	if (_netif) { // event : MLAN_EVENT_ID_UAP_FW_BSS_IDLE
		netif_set_link_down(_netif);
		dprintf("uAP disconn : last / BSS IDLE\r\n");
	} else { // event : MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT
		dprintf("uAP : device disconnect\r\n");
	}
}

static void enx_uap_conn_cb(void *ctx)
{
	struct netif *_netif = ctx;
	if (_netif) { // event : MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE
		netif_set_link_up(_netif);
		dprintf("uAP conn : first / BSS Active\r\n");
	} else { // event : MLAN_EVENT_ID_UAP_FW_STA_CONNECT
		dprintf("uAP : device connect\r\n");
	}
}

static void enx_uap_netif_init(void)
{
	void *state = listNetif[enlWIFIUAP]->state;
	netif_add(listNetif[enlWIFIUAP], NULL, NULL, NULL, state, mlanif_init, ethernet_input);

#ifdef custom_MAC_UAP
	u8 uap_mac[6] = WIFI_UAP_MACADDRESS;				// AP mode MAC address
	ewl_set_hwaddr(listNetif[enlWIFIUAP], uap_mac);		// AP mode MAC address set
#else
	ewl_get_hwaddr(listNetif[enlWIFIUAP], listNetif[enlWIFIUAP]->hwaddr);
#endif
	printf("UAP MacAddress : %02X:%02X:%02X:%02X:%02X:%02X\r\n",
		listNetif[enlWIFIUAP]->hwaddr[0], listNetif[enlWIFIUAP]->hwaddr[1], listNetif[enlWIFIUAP]->hwaddr[2],
		listNetif[enlWIFIUAP]->hwaddr[3], listNetif[enlWIFIUAP]->hwaddr[4], listNetif[enlWIFIUAP]->hwaddr[5]);
}

/* 
    called when adding netif to s/w AP mode of wifi is completed.
    register netif to lwip stack.
*/
static void enx_uap_netif_init_complete(void *ctx)
{
	ip_addr_t ipaddr, netmask, gw;
	struct netif *_netif = ctx;

	ip_addr_copy(ipaddr, gtWificfg.APcfg.iptLocalip);
	ip_addr_copy(netmask, gtWificfg.APcfg.iptNetmask);
	ip_addr_copy(gw, gtWificfg.APcfg.iptLocalip);

	netif_set_addr(_netif, &ipaddr, &netmask, &gw);

	ewl_set_conn_cb((int)_netif->state, enx_uap_conn_cb, enx_uap_disconn_cb);

	printf("SSID CHECK : [%s]\r\n", gtWificfg.APcfg.strSSID);

//	set_sys_cfg_11n(0);//disable 11n / sd8787
	ewlap_bss_start(); // ¾à 700ms ¼Ò¿ä
	ewl_uap_get_freq();
//	ewlap_get_cfg_11n();
//	ewlap_get_apcfg();
//	wl_print_apcfg();
	netif_set_up(_netif);

//	Sdio1ClockDiv(eSDIO_CLK_50MHz);

}

#if 0
    ex1) wep network connect
    /* register network to be connected to AP.*/
    //id = ewl_add_netowrk();
    //ewl_set_network(id,"key_mgmt"       ,"NONE");
    //ewl_set_network(id,"ssid"           ,"netgear33");
    //ewl_set_network(id,"wep_key0"       ,"759478362c");
    //ewl_set_network(id,"wep_tx_keyindex",0);
    //ewl_set_network(id,"scan_ssid"      ,1);
    //ewl_enable_network(id);  

    ex2) wpa-psk connect
    id = ewl_add_netowrk();
    //ewl_set_network(id,"key_mgmt"       ,"NONE");
    ewl_set_network(id,"ssid"           ,"iptime_enx");
    ewl_set_network(id,"psk"       ,"sysfo_eyenix");
    //ewl_set_network(id,"wep_tx_keyindex",0);
    //ewl_set_network(id,"scan_ssid"      ,1);
    ewl_enable_network(id);
#endif
#endif

/**
    lwip net up or down for STA
*/
void wifi_sta_setLinkNet(int on)
{
	if(on == 1)
	{	
//		  ip_addr_t ipaddr, netmask, gw;  
//		  ip_addr_copy(ipaddr, gtWificfg.STAcfg.iptLocalip);
//		  ip_addr_copy(netmask, gtWificfg.STAcfg.iptNetmask);
//		  ip_addr_copy(gw, gtWificfg.STAcfg.iptGateway);
//		  netif_set_addr(listNetif[enlWIFISTA], &ipaddr, &netmask, &gw);
		netif_set_link_up(listNetif[enlWIFISTA]);
	}
	else if(on == 0)
	{
		netif_set_link_down(listNetif[enlWIFISTA]);
	}
}

void wifi_sta_setNet(int on)
{
    if(on == 1)
    {   
//        ip_addr_t ipaddr, netmask, gw;  
//        ip_addr_copy(ipaddr, gtWificfg.STAcfg.iptLocalip);
//        ip_addr_copy(netmask, gtWificfg.STAcfg.iptNetmask);
//        ip_addr_copy(gw, gtWificfg.STAcfg.iptGateway);
//        netif_set_addr(listNetif[enlWIFISTA], &ipaddr, &netmask, &gw);
        netif_set_up(listNetif[enlWIFISTA]);
    }
    else if(on == 0)
    {
        netif_set_down(listNetif[enlWIFISTA]);
    }
}

void enx_wifi_sta_start(void)
{
    if (listNetif[enlWIFISTA]) {
        enx_sta_netif_init_complete(listNetif[enlWIFISTA]);
        supplicant_run_init(NULL);
        supplicant_run();
        xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_ACTIVE, listNetif[enlWIFISTA]->hwaddr, enlWIFISTA);
    }
}

void enx_wifi_ap_start(void)
{
    if (listNetif[enlWIFIUAP]) {
        enx_uap_netif_init_complete(listNetif[enlWIFIUAP]);
        xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_ACTIVE, (BYTE *)listNetif[enlWIFIUAP]->hwaddr, enlWIFIUAP);
    }
}

#if 0
void
sta_check_connection_cb(void *ctx)
{
    static int retry=0;
    
    retry++;
        
    if(retry > 30 || ewl_sta_is_connected())
    {   
        timer_cancel_timeout(tmr_id_uap_conn);
        tmr_id_uap_conn = -1; 
        xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_START, NULL, NULL);
        printf("retry %d, connect %d \r\n",retry, ewl_sta_is_connected());
        retry = 0;
    }     
}

void 
uap_check_connection_cb(void *ctx)
{
    static int retry=0;
    
    retry++;
        
    if(retry > 10 || ewlap_is_connected())
    {   
        timer_cancel_timeout(tmr_id_uap_conn);
        tmr_id_uap_conn = -1; 
        xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_START, NULL, NULL);
        printf("retry %d, connect %d \r\n",retry, ewlap_is_connected());
        retry = 0;
    }        
}

void enx_wifi_helper_mac_address(void *ctx)
{
	struct netif *netif = ctx;
	char mac_addr[6]={0x00,0x23,0x45,0xab,0xcd,0xef};
    ewl_set_hwaddr(netif, mac_addr);
	printf("set WiFi mac address \r\n");
}
#endif

void ewl_sdio_ext_irq_handler(void *dummy);
/*
    Marvell wifi is initialized. 
    where be called  timer, shell api 
    
    @return :  0 = wifi initialization is success.
              -1 = fail. 
*/
int enx_wifi_helper_init(void)
{
#ifdef GPIO_SDIO1_RST
	GpioSetLo(GPIO_SDIO1_RST);
	timer_delay(100);
	GpioSetHi(GPIO_SDIO1_RST);
	timer_delay(100);
#endif

	Sdio1Init();

	IntAdd(IRQ_SDIO1, (void*)ewl_sdio_ext_irq_handler, NULL);
	IrqEnable(IRQ_SDIO1);

	timer_delay(10);

	if (ewl_init() == EWL_SUCCESS) { /* wifi initialize */
		listNetif[enlWIFISTA] = ewl_add_netif(BSS_TYPE_STA);
		if (listNetif[enlWIFISTA]) {
			enx_sta_netif_init();
			xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_START, listNetif[enlWIFISTA]->hwaddr, enlWIFISTA);
		}

		listNetif[enlWIFIUAP] = ewl_add_netif(BSS_TYPE_UAP);
		if (listNetif[enlWIFIUAP]) {
			enx_uap_netif_init();
#ifdef SD8782_ENABLE
//			ewlap_sys_cfg_11n(1, 0x117e); //bw 75Mbps
#endif
			xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_START, listNetif[enlWIFIUAP]->hwaddr, enlWIFIUAP);
		}

		printf("Wi-Fi Start OK\r\n");
		return 0;
	}

	xfer_event_to_cpu0(WLIF_EVT, WLIF_INIT_FAIL, NULL, (UINT)NULL);
	printf("Wi-Fi Start Failed\r\n");
	return -1;
}

/**
 *  This function poll all netifs registerd with ewl.
 *  and receive rx packet from wifi and hand over lwip
 *
 *  here, lwip handles rx packet from WiFi
*/
void enx_wifi_poll_netif(void)
{
    ewl_poll();

    if(listNetif[enlWIFISTA])
        mlanif_input(enlWIFISTA);

    if(listNetif[enlWIFIUAP])
        mlanif_input(enlWIFIUAP);
}

struct netif* enx_wifi_get_ap_netif(void)
{
    return listNetif[enlWIFIUAP];
}

struct netif* enx_wifi_get_sta_netif(void)
{
    return listNetif[enlWIFISTA];
}

u32 wificheckid = -1;

void timer_wifi_check(void *ctx)
{
	static int cnt = 0;
	cnt++;
	if(cnt == 6)
	{
		cnt = 0;
		timer_cancel_timeout(wificheckid);
		IrqEnable(IRQ_ETH);
	}
	UNUSED(ctx);
}
#endif
