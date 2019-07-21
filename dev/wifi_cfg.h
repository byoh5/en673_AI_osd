#ifndef _WIFI_CFG_H_
#define _WIFI_CFG_H_

#include "lwip/ip_addr.h"

#define WIFI_SSID_MAX			32	// 1~32byte
#define WIFI_KEY_WPAnWPA2_MAX	63	// 8~63byte
#define WIFI_NAME_MAX			32	// 1~32byte
#define WIFI_ON		0
#define WIFI_OFF	1
/*
typedef enum {
	wifi_SouthKorea,
	wifi_China,
	wifi_Japan,
	wifi_USAnCanada
}eZone;
*/
typedef enum {
	wifi_OPEN,
	wifi_WPA_PSK,
	wifi_WPA2_PSK,
	wifi_WPAnWPA2_PSK
}eAuthentication;
/*
typedef enum {
	wifi_Off,
	wifi_WEP64,
	wifi_WEP128,
	wifi_TKIP,
	wifi_AES,
	wifi_TKIPnAES
}eEncryption;
*/
/*
typedef struct {
	u32 un8Switch;						// ON/OFF
	s32 strSSID[WIFI_SSID_MAX+1];		// SSID
	eZone un8Znoe;						// Zone
	u32 un8Channel;						// Channel(1~13)
	u32 un8SSIDBroadcast;				// SSID Broadcast
	eAuthentication un8Authentication;	// Authentication
	eEncryption un8Encryption;			// Encryption
	s32 strKey[WIFI_KEY_WPAnWPA2_MAX+1];	// KEY(size:5,13,63)
}__attribute__ ((packed)) tWifiAPcfg;
*/

typedef struct {
	UINT un32State;						// State				//4(1)
	UINT un8Switch;						// ON/OFF				//4(1)
	char strSSID[WIFI_SSID_MAX+4];		// SSID					//36(32)
	UINT un8Channel;						// Channel(1~13)		//4(1)
	eAuthentication un32Authentication;	// Authentication		//4(4)
	char strKey[WIFI_KEY_WPAnWPA2_MAX+1];	// KEY(size:5,13,63)	//64(63)
	UINT un8MaxSTA;						// Max station count	//4(1)

	ip_addr_t iptLocalip;				// 						//4(4)
	ip_addr_t iptNetmask;				// 						//4(4)

	UINT un8DHCPServer;					//						//4(1)
	ip_addr_t iptStartAddress;			//						//4(4)
	ip_addr_t iptEndAddress;			//						//4(4)
	UINT leaseTime;						//						//4(4)

	struct netif *niInterfaces;			//						//4(4)
}tWifiAPcfg;

typedef struct {
	UINT un32State;						//						//4(1)
	UINT un8Switch;						// ON/OFF				//4(1)
	UINT un8Connect;						// Connect / Disconnect	//4(1)
	char strSSID[WIFI_SSID_MAX+4];		// SSID					//36(32)
	eAuthentication un32Authentication;	// Authentication		//4(4)
	char strKey[WIFI_KEY_WPAnWPA2_MAX+1];	// KEY(size:5,13,63)	//64(63)

	ip_addr_t iptLocalip;				// 						//4(4)
	ip_addr_t iptNetmask;				// 						//4(4)
	ip_addr_t iptGateway;				// 						//4(4)
	ip_addr_t iptDNS0;					// 						//4(4)
	ip_addr_t iptDNS1;					// 						//4(4)
	UINT un8DHCPSwitch;					// 						//4(1)

	struct netif *niInterfaces;			//						//4(4)
}tWifiSTAcfg;

typedef struct {
	UINT nMagicNum;
	UINT nDateID;
	tWifiAPcfg APcfg;
	tWifiSTAcfg STAcfg;
}tWificfg;

#endif // _WIFI_H_