#include "dev.h"
#include "dev_inline.h"

#include <string.h>

//#include "ff.h"

#include "user.h"
#include "msg.h"

#ifdef __WIFI__

#include "wifi.h"

tWificfg gtWificfg;

void WifiCFG_Default_AP(tWifiAPcfg *ap)
{
	ap->un32State = WLIF_NONE;
	ap->un8Switch = WIFI_AP_SWITCH;
	strcpy((char *)ap->strSSID, WIFI_AP_SSID);
	ap->un8Channel = WIFI_AP_CH;
	ap->un32Authentication = WIFI_AP_AUTH;
	strcpy((char *)ap->strKey, WIFI_AP_KEY);
	ap->un8MaxSTA = 5;

	IP4_ADDR(&ap->iptLocalip, 192, 169, 15, 1);
	IP4_ADDR(&ap->iptNetmask, 255, 255, 255, 0);
	ap->un8DHCPServer = WIFI_ON;	// DHCP server on
	IP4_ADDR(&ap->iptStartAddress, 192, 169, 15, 2);
	IP4_ADDR(&ap->iptEndAddress, 192, 169, 15, 254);
	ap->leaseTime = 864000;	// 10day
}

void WifiCFG_Default_STA(tWifiSTAcfg *sta)
{
	sta->un32State = WLIF_NONE;
	sta->un8Switch = WIFI_STA_SWITCH;
	sta->un8Connect = sta->un8Switch;
	strcpy((char *)sta->strSSID, WIFI_STA_SSID);
	sta->un32Authentication = WIFI_STA_AUTH;
	strcpy((char *)sta->strKey, WIFI_STA_KEY);
	sta->un8DHCPSwitch = NET_DHCP;	// connect to DHCP server

	if(sta->un8DHCPSwitch == WIFI_ON)
	{
		IP4_ADDR(&sta->iptLocalip, 0, 0, 0, 0);
		IP4_ADDR(&sta->iptNetmask, 0, 0, 0, 0);
		IP4_ADDR(&sta->iptGateway, 0, 0, 0, 0);
		IP4_ADDR(&sta->iptDNS0, 0, 0, 0, 0);
		IP4_ADDR(&sta->iptDNS1, 0, 0, 0, 0);
	}
	else
	{
		IP4_ADDR(&sta->iptLocalip, 192, 168, 0, 196);
		IP4_ADDR(&sta->iptNetmask, 255, 255, 0, 0);
		IP4_ADDR(&sta->iptGateway, 192, 168, 0, 1);
		IP4_ADDR(&sta->iptDNS0, 8, 8, 8, 8);
		IP4_ADDR(&sta->iptDNS1, 8, 8, 4, 4);
	}
}

u32 WifiCFG_Save(void)
{
	gtWIFI = (BYTE *)&gtWificfg;
	gtWIFI_size = sizeof(gtWificfg);

	printf("Save Wi-Fi configuration... ");
	gtWificfg.nMagicNum = MAGICNUM_WIFI;
	gtWificfg.nDateID = gptMsgShare.CPU1_DATEID;
	UINT res = UserSave(sfls_WIFI);
	if (res == DEF_OK) {
		printf("done.\r\n");
	} else {
		printf("save fails.\r\n");
	}

	return 0;
}

u32 WifiCFG_Load(void)
{
	gtWIFI = (BYTE *)&gtWificfg;
	gtWIFI_size = sizeof(gtWificfg);

	printf("Loading Wi-Fi configuration... ");
#if WIFI_DEFAULT
	if(1)	// default value load
#else
	UserLoad(sfls_WIFI);
	if((gtWificfg.nMagicNum!=MAGICNUM_WIFI) || (gtWificfg.nDateID!=gptMsgShare.CPU1_DATEID))
#endif
	{
		printf("load fails. Load default configuration.\r\n");
		WifiCFG_Default_AP(&gtWificfg.APcfg);
		WifiCFG_Default_STA(&gtWificfg.STAcfg);
		gtWificfg.nMagicNum = MAGICNUM_WIFI;
		gtWificfg.nDateID = gptMsgShare.CPU1_DATEID;
		UserSave(sfls_WIFI);
	} else {
		printf("done.\r\n");
	}

	return 0;
}
#endif
