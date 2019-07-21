#ifndef CGISSI_WIFI_H_
#define CGISSI_WIFI_H_

#define CGIfile_wifiap	"/wifi/appwifiap.cgi"
#define CGIfile_wifista	"/wifi/appwifista.cgi"

#define SSI_WIFI_ap		150
#define SSI_WIFI_sta	151
#define SSI_WIFI_scan	152

extern int App_WiFi_SSIHandler(int iIndex, char *pcInsert, int iInsertLen);

extern const char *App_WiFiAP_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *App_WiFiSTA_CGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

enum {
	WIFI_CGI_AP_OK				= 0x00000001,

	WIFI_CGI_AP_SW				= 0x00000002,
	WIFI_CGI_AP_SSID			= 0x00000004,
	WIFI_CGI_AP_CH				= 0x00000008,
	WIFI_CGI_AP_MAXSTA			= 0x00000010,
	WIFI_CGI_AP_AUTH			= 0x00000020,
	WIFI_CGI_AP_KEY				= 0x00000040,
	WIFI_CGI_AP_IP				= 0x00000080,
	WIFI_CGI_AP_SN				= 0x00000100,
	WIFI_CGI_AP_DHCP			= 0x00000200,
	WIFI_CGI_AP_DHCP_POOLNAME	= 0x00000400,
	WIFI_CGI_AP_DHCP_GATEWAY	= 0x00000800,
	WIFI_CGI_AP_DHCP_DNSSERVER	= 0x00001000,
	WIFI_CGI_AP_DHCP_STARTADD	= 0x00002000,
	WIFI_CGI_AP_DHCP_ENDADD		= 0x00004000,
	WIFI_CGI_AP_DHCP_MAXUSER	= 0x00008000,
	WIFI_CGI_AP_DHCP_LEASETIME	= 0x00010000
};

enum {
	WIFI_CGI_STA_OK				= 0x00000001,

	WIFI_CGI_STA_SW				= 0x00000002,
	WIFI_CGI_STA_SSID			= 0x00000004,
	WIFI_CGI_STA_AU				= 0x00000008,
	WIFI_CGI_STA_KEY			= 0x00000010,
	WIFI_CGI_STA_CONN			= 0x00000020,
	WIFI_CGI_STA_NETIF			= 0x00000040,
};

#endif
