#include <stdio.h>

#include "dev.h"
#include "dev_inline.h"

#include <lwip/init.h>
#include <mlanif.h>
#include <lwip/netif.h>
#include <netif/etharp.h>
#include <lwip/dhcp.h>
#include <lwip/igmp.h>
#include "lwip/tcp.h"
#ifdef LWIP_141
#include <lwip/tcp_impl.h>
#endif


#include "ewl.h"
#include "wl_tools.h"
#include "wifi_cfg.h"
#include "msg.h"
//#include "sys.h"
//#include "cache.h"
#include "open_eth.h"
#include "enx_lwip_helper.h"
#include "enx_wifi_bt_helper.h"

#include "user.h"
#include "rtp_over_rtsp.h"

#ifdef __WIFI__

#include "hci.h"

tWificfg gtWificfg;

int wificonnection;
int bluetoothmode;

/* external variables */

struct netif *sta_netif = NULL;
struct netif *swap_netif = NULL;
int tmr_id_uap_conn = -1;

void ewl_sdio_ext_irq_handler(void *arg);
void enx_wifi_poll_netif(void);

int multicast_udp_start(void);
//void test_inquery_cb();

/*****************************************************************************
    enx : Eyenix U2 
    Lwip is required for this library
    shell api & wifi  api & timer api is combined to use wifi marvell
******************************************************************************/

/********************************************************
        Local Functions
********************************************************/

/*
    eux_sta_conn_cb & eux_sta_disconn_cb is called when wifi connect to WiFi AP
    and lwip is up.
*/

/*
    When AP is connected, dhcp client is started.
    and lwip can be up.
*/
static void enx_sta_conn_cb(void *ctx)
{
    struct netif *netif = ctx;
	u8 mac_addr[6] = {0};
    ewl_get_hwaddr(netif, mac_addr);
    xfer_event_to_cpu0(WLIF_EVT, WLIF_CONNECTED, mac_addr, netif);
}

/*
    When AP is dis_connected, dhcp client is started.
    and lwip can be down.
*/
static void enx_sta_disconn_cb(void *ctx)
{
    struct netif *netif = ctx;
    xfer_event_to_cpu0(WLIF_EVT, WLIF_DIS_CONNECTED, NULL, netif);
    if(netif_is_up(netif))
    {
        netif_set_down(netif);
    }
    else
    {
        //printf("sta link down\n");
    }
}

static void ip_status_cb(struct netif* netif)
{
    if(netif_is_up(netif))
    {

    }
    else
    {

    }
}

/* 
    called when adding netif to sta mode of wifi is completed.
    register netif to lwip stack.   
*/
static void enx_sta_init_complete_cb(void *ctx)
{
    ip_addr_t ipaddr, netmask, gw;
    struct netif *netif = ctx;
    u32 handle = netif->state;

    ip_addr_copy(ipaddr, gtWificfg.STAcfg.iptLocalip);
    ip_addr_copy(netmask, gtWificfg.STAcfg.iptNetmask);
    ip_addr_copy(gw, gtWificfg.STAcfg.iptLocalip);

    /* add wl to lwip interface list and set as default */
    netif_add(netif, &ipaddr, &netmask, &gw, netif->state,
              mlanif_init,   /* init */
              ethernet_input);

#ifdef __MAIN_WIFI__
	netif_set_default(netif);
#endif
    netif->state = handle;
    ewl_set_conn_cb(netif->state, enx_sta_conn_cb, enx_sta_disconn_cb);
}

static void enx_uap_conn_cb(void *ctx)
{
    struct netif *netif = ctx;
	if(netif)
	{	// event : MLAN_EVENT_ID_UAP_FW_BSS_ACTIVE
		dprintf("uAP conn : first / BSS Active\n");
	}
	else
	{	// event : MLAN_EVENT_ID_UAP_FW_STA_CONNECT
		dprintf("uAP : device connect\n");
		wificonnection = 1;
	}
}

/*
    When AP is dis_connected, dhcp client is started.
    and lwip can be down.
*/
static void enx_uap_disconn_cb(void *ctx)
{
    struct netif *netif = ctx;
	if(netif)
	{	// event : MLAN_EVENT_ID_UAP_FW_BSS_IDLE
		dprintf("uAP disconn : last / BSS IDLE\n");
//		if(nrtsp_user)
//		{
//			rtsp_all_disconnect();
//			wificonnection = 0;
//		}
	}
	else
	{	// event : MLAN_EVENT_ID_UAP_FW_STA_DISCONNECT
//		if((nrtsp_user == 0)&& gptMsgCop2Cpu->FTPstate == eFTPstate_NONE)
//		{
//			dprintf("uAP : device disconnect\n");
//			//rtsp_all_disconnect();
//			wificonnection = 0;
//			//wl_print_apcfg();
//		}
	}
}

int 
ewlap_bss_reset(void);
void enx_uap_init(void)
{
    ewlap_bss_reset();    
    //ewlap_init_apcfg(gtWificfg.APcfg.strSSID);
    //printf("    
    switch(gtWificfg.APcfg.un32Authentication)
    {
        case wifi_OPEN:
            ewlap_apcfg("SEC", "open");
            break;
        case wifi_WPA2_PSK:
            ewlap_apcfg("SEC", "wpa2-psk");
            ewlap_apcfg("KEY", gtWificfg.APcfg.strKey);
            break;
    }
    
    char strnum[10];
    sprintf(strnum, "%d", gtWificfg.APcfg.un8MaxSTA);
    ewlap_apcfg("MAX_SCB", strnum);
    sprintf(strnum, "%d", 9);
    ewlap_apcfg("CHANNEL", strnum);
    
    //ewlap_get_cfg_11n();
    //ewlap_set_apcfg();
}

int
ewlap_cfg_beacon_period(int action, u16 interval);

/* 
    called when adding netif to s/w AP mode of wifi is completed.
    register netif to lwip stack.   
*/

BYTE hex2ascii(BYTE hex)
{
	BYTE tmp;
	if((hex>=0)&&(hex<=0x09))
	{
		tmp=hex+'0';
	}
	else if((hex>=0x0a)&&(hex<=0x0f))
	{
		tmp=hex-0x0a+'A';
	}
	return tmp;
}

#include "wlif_tools.h"

void enx_uap_netif_init_complete_cb(void *ctx)
{
    ip_addr_t ipaddr, netmask, gw;
    struct netif *netif = ctx;
    u32 handle = netif->state;

#if 1    
    ip_addr_copy(ipaddr, gtWificfg.APcfg.iptLocalip);
    ip_addr_copy(netmask, gtWificfg.APcfg.iptNetmask);
    ip_addr_copy(gw, gtWificfg.APcfg.iptLocalip);
#else    
    IP4_ADDR(&gw, 192,168,15,1);
    IP4_ADDR(&ipaddr, 192,168,15,1);
    IP4_ADDR(&netmask, 255,255,255,0);
#endif

    /* add wl to lwip interface list */
    netif_add(netif, &ipaddr, &netmask, &gw, netif->state,
              mlanif_init,   /* init */
              ethernet_input /* handles ARP and IP packets */);
              
    //netif->state = handle;
    
    ewl_set_conn_cb(netif->state, enx_uap_conn_cb, enx_uap_disconn_cb);
	
	gtWificfg.APcfg.strSSID[SSID_LEN]=hex2ascii((netif->hwaddr[0]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+1]=hex2ascii(netif->hwaddr[0]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+2]=hex2ascii((netif->hwaddr[1]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+3]=hex2ascii(netif->hwaddr[1]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+4]=hex2ascii((netif->hwaddr[2]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+5]=hex2ascii(netif->hwaddr[2]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+6]=hex2ascii((netif->hwaddr[3]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+7]=hex2ascii(netif->hwaddr[3]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+8]=hex2ascii((netif->hwaddr[4]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+9]=hex2ascii(netif->hwaddr[4]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+10]=hex2ascii((netif->hwaddr[5]>>4)&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+11]=hex2ascii(netif->hwaddr[5]&0x0f);
	gtWificfg.APcfg.strSSID[SSID_LEN+12]='\0';
	dprintf("SSID CHECK : [%s]\n", gtWificfg.APcfg.strSSID);
	
    ewlap_apcfg(WIFI_UAP_SET_CMD_SSID, gtWificfg.APcfg.strSSID);
	

    switch(gtWificfg.APcfg.un32Authentication)
    {
        case wifi_OPEN:
            ewlap_apcfg("SEC", "open");
            break;
        case wifi_WPA2_PSK:
            ewlap_apcfg("SEC", "wpa2-psk");
            ewlap_apcfg("KEY", gtWificfg.APcfg.strKey);
            break;
    }

    char strnum[10];
    sprintf(strnum, "%d", gtWificfg.APcfg.un8MaxSTA);
    ewlap_apcfg("MAX_SCB", strnum);
    sprintf(strnum, "%d", gtWificfg.APcfg.un8Channel);
    ewlap_apcfg("CHANNEL", strnum);
 //   ewlap_init_apcfg(gtWificfg.APcfg.strSSID);    

//    ewlap_bss_reset();

   //	 sys_cfg_ssid(NULL);
   {
	 char ch=gtWificfg.APcfg.un8Channel;//44;
	 int mode=2;
	 int band =0;//1;
	 ewlap_sys_cfg_channel(&ch,&mode, &band,1);
	 
	 //ewlap_sys_cfg_channel(&ch,&mode, &band,0);
   }  
    ewlap_set_apcfg();
    //ewlap_cfg_beacon_period(1,100);//beacon
    //ewlap_cfg_beacon_period(0,0);//beacon

   // set_sys_cfg_11n(0);//disable 11n
    ewlap_bss_start();
    ewl_uap_get_freq();
    //ewlap_get_cfg_11n();
    //ewlap_get_apcfg();
    
/*      struct ewlap_cfg ap_cfg;
   while(!ewlap_get_apcfg(&ap_cfg))
    {
       printf("\n ap_cfg.ssid %d gtWificfg.APcfg.strSSID %d \n", strlen(ap_cfg.ssid), strlen(gtWificfg.APcfg.strSSID));
    	if(strlen(ap_cfg.ssid) != strlen(gtWificfg.APcfg.strSSID))
    	{
	        printf("\nerror ssid set again \n");
			ewlap_apcfg(WIFI_UAP_SET_CMD_SSID, gtWificfg.APcfg.strSSID);
				
			
			switch(gtWificfg.APcfg.un32Authentication)
			{
				case wifi_OPEN:
					ewlap_apcfg("SEC", "open");
					break;
				case wifi_WPA2_PSK:
					ewlap_apcfg("SEC", "wpa2-psk");
					ewlap_apcfg("KEY", gtWificfg.APcfg.strKey);
					break;
			}
		
			char strnum[10];
			sprintf(strnum, "%d", gtWificfg.APcfg.un8MaxSTA);
			ewlap_apcfg("MAX_SCB", strnum);
			sprintf(strnum, "%d", gtWificfg.APcfg.un8Channel);
			ewlap_apcfg("CHANNEL", strnum);
		
			ewlap_set_apcfg();
    	}
		else
		{
			break;
		}
	}    
*/	
	wl_print_apcfg();
    netif_set_up(netif);
    
	u8 mac_addr[6] = {0};
    ewl_get_hwaddr(netif, mac_addr);
    xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_ACTIVE, mac_addr, netif);
}

void SdioInit(void)
{
    //sdioSetbase(SDIO_WIFI_BASE);
    Sdio1DiCmd();                       // Command interrupt disable
    Sdio1DiDat();                       // Data interrupt disable
    Sdio1EiCip();                      // Interrupt from external chip enable
    //SD_EI_CD = 1;                    // Enable card detection irq
	SDIO1_EN = 1;
	SDIO1_CLKEN = 1;
    Sdio1Cfg(eSDIO_CLK_400KHz, 0x200, 0x1);

    //if(SDIO_WIFI_BASE == SDCD_BASE)
    //{       
    //    IntAdd(IRQ_SDCD1, (void*)ewl_sdio_ext_irq_handler, 0);    
    //    Ei(IRQ_SDCD1);                                   
    //}
    //else
    {
		IntAdd(IRQ_SDIO1, (void*)ewl_sdio_ext_irq_handler, NULL);
		IrqEnable(IRQ_SDIO1);
    }
	
}

/**
    lwip net up or down for STA
*/
void wifi_sta_setNet(int on)
{
    if(on == 1)
    {   
        ip_addr_t ipaddr, netmask, gw;  
        ip_addr_copy(ipaddr, gtWificfg.STAcfg.iptLocalip);
        ip_addr_copy(netmask, gtWificfg.STAcfg.iptNetmask);
        ip_addr_copy(gw, gtWificfg.STAcfg.iptGateway);
        netif_set_addr(sta_netif, &ipaddr, &netmask, &gw);
        netif_set_up(sta_netif);
    }
    else if(on == 0)
    {
        netif_set_down(sta_netif);
    }
}

void enx_wifi_sta_start(void)
{
    //sta_netif = ewl_add_netif(enx_sta_init_complete_cb, BSS_TYPE_STA);
    if(sta_netif)
    {
        enx_sta_init_complete_cb(sta_netif);
        supplicant_run_init();
        supplicant_run();
        xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_ACTIVE, NULL,NULL);
    }
}

void enx_wifi_ap_start(void)
{
    //swap_netif = ewl_add_netif(enx_uap_netif_init_complete_cb,BSS_TYPE_UAP);
    if(swap_netif)
        enx_uap_netif_init_complete_cb(swap_netif);        
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
        printf("retry %d, connect %d \n",retry, ewl_sta_is_connected());
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
        printf("retry %d, connect %d \n",retry, ewlap_is_connected());
        retry = 0;
    }        
}
#endif

//  lwbt_memp_init();
//  //register callback functions
//  ewl_set_cb_hci_ops(hci_init, hci_reset_all, hci_recv_frame);
// // ewl_bluetooth_init();
//
//  
//	l2cap_init();
//	sdp_init();
//	rfcomm_init();
//  bt_spp_start();
//  timer_sched_timeout_cb(1000,TIMEOUT_PERIODIC,bt_timer,NULL);

int 
ewl_bluetooth_init(void);
void lwbt_memp_init(void);
void bt_spp_start(void);
void
l2cap_init(void);
void sdp_init(void);
void rfcomm_init(void);

void
ewl_set_cb_hci_ops(void *(*hci_init)(void), 
                   void (*hci_reset)(void),
                   int (*rx_hci)(void*p));

#if 1
void 
bt_timer(void *arg)
{//dprintf("bt..... bt_timer \n");
  l2cap_tmr();
};
#endif

void enx_bt_init_netif(void)
{
    dprintf("bt..... Start ok \n");
    ewl_bluetooth_init();  
    {
        l2cap_init();
        sdp_init();
        bt_spp_start();
        timer_sched_timeout_cb(1000,TIMEOUT_PERIODIC,bt_timer,NULL);      
    }
    timer_sched_timeout_cb(40,TIMEOUT_PERIODIC,ewl_poll_bt,NULL);	//ksj_151228_stream 
}

void enx_wifi_init_netif(void)
{
	  
	sta_netif = ewl_add_netif(BSS_TYPE_STA);		
	if(sta_netif && gtWificfg.STAcfg.un8Switch == WIFI_ON)
	{		
		dprintf("sta_netif ");
	    xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_START, NULL, NULL); 					   
	}

    swap_netif = ewl_add_netif(BSS_TYPE_UAP);
    if(swap_netif && gtWificfg.APcfg.un8Switch == WIFI_ON)     
    {
    	dprintf("swap_netif ");
       xfer_event_to_cpu0(WLIF_EVT, WLIF_UAP_START, NULL, NULL);
    }
}
	

/*
    Marvell wifi is initialized. 
    where be called  timer, shell api 
    
    @return :  0 = wifi initialization is success.
              -1 = fail. 
*/
int enx_wifi_bt_helper_init(void)
{
    int id=0;
    int ret=0;
    
#ifdef GPIO_SDIO1_RST    
	GpioSetLo(GPIO_SDIO1_RST); 
	timer_delay(100);
	GpioSetHi(GPIO_SDIO1_RST);
	timer_delay(100);
#endif

	Sdio1Init();
//
	IntAdd(IRQ_SDIO1, (void*)ewl_sdio_ext_irq_handler, NULL);
	IrqEnable(IRQ_SDIO1);
    
//    SdioInit();
    //bt mem init
    lwbt_memp_init();
    //regster bt callback func
    ewl_set_cb_hci_ops(hci_init, hci_reset_all, hci_recv_frame);
    //strcpy(gtWificfg.STAcfg.strSSID, AP_SSID);
//    g711_mb->WifiErro=0;
//    g711_mb->btconnecting=0;
    /* sd8787 sdio init*/
    ret = ewl_sdio_init();
    
    if(ret =0)
    {
//       g711_mb->WifiErro=1;
      	//g711_mb->wBuzzCnt=5;
      return -1;
    }
    
    /* wifi initialize*/
    if(ewl_wifi_init() == EWL_SUCCESS)    
    {
        //u8 sta_mac[6]={0x00,0x11,0x22,0x33,0x55,0xaf};
        //u8 uap_mac[6]={0x00,0x11,0x22,0x33,0x55,0xef};

        dprintf("WiFi Start ok \n");
        
        //sta_netif = ewl_add_netif(BSS_TYPE_STA);        
//        swap_netif = ewl_add_netif(BSS_TYPE_UAP);
        
//        if(sta_netif)
//        {    
//            ewl_set_hwaddr(sta_netif,sta_mac);
//            xfer_event_to_cpu0(WLIF_EVT, WLIF_STA_START, NULL, NULL);                        
//        }
        
       // if(swap_netif)
       //     ewl_set_hwaddr(sta_netif,uap_mac);
        //if uap is inactive and sta is active, immediately , send event to cpu0
       //timer_sched_timeout_cb(4000,TIMEOUT_ONESHOT,enx_wifi_init_netif,NULL);
		enx_wifi_init_netif();
//        else //else we send event after uap is connected.
//            tmr_id_uap_conn = timer_sched_timeout_cb(1000, 
//                                                    TIMEOUT_PERIODIC,
//                                                    sta_check_connection_cb,
//                                                   NULL); 
//          return 0;
    }
    else
    {
        dprintf("WiFi Start Failed \n");
//		g711_mb->WifiErro=1;
		//g711_mb->wBuzzCnt=5;
        return -1;
    }
#if 1
	//enx_bt_init_netif();
	//timer_sched_timeout_cb(5000,TIMEOUT_ONESHOT,enx_bt_init_netif,NULL);
#endif	

	//timer_sched_timeout_cb(30,TIMEOUT_PERIODIC,ewl_poll,NULL);	//ksj_151228_stream 
	//timer_sched_timeout_cb(10,TIMEOUT_PERIODIC,ewl_poll_bt,NULL);

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

 //   if(sta_netif)
  //      mlanif_input(sta_netif);
        
  //  if(swap_netif)
  //      mlanif_input(swap_netif);
}

struct netif* enx_wifi_get_ap_netif()
{
    return swap_netif;
}

struct netif* enx_wifi_get_sta_netif()
{
    return sta_netif;
}

u32 wificheckid = -1;

void timer_wifi_check(void *ctx)
{
	static int cnt = 0;
	cnt++;
	if(cnt == 6)
	{
		timer_cancel_timeout(wificheckid);
//		Ei(IRQ_ETHRX);
	}
}
#endif
