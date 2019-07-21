#ifndef _FACTORY_H_
#define _FACTORY_H_

//*************************************************************************************************
// Model define
//-------------------------------------------------------------------------------------------------

//*************************************************************************************************
// Basic define (Do not modify)
//-------------------------------------------------------------------------------------------------


//*************************************************************************************************
// Initial factory reset condition set
//-------------------------------------------------------------------------------------------------
// Network
#define	IP_ADDRESS			"192.168.99.195"						// Ip addr(rel)
#define	NET_MASK			"255.255.0.0"						// Net mask
#define	GW_ADDRESS			"192.168.0.1"						// Gate way

#define	DNS_SRVR0			"8.8.8.8"							// Dns server 0
#define	DNS_SRVR1			"8.8.4.4"							// Dns server 1

#define DISCOVERY_IP 		"239.255.255.250"
#define DISCOVERY_PORT		3702

#define	SNTP_SRVR			"0.asia.pool.ntp.org"				// NTP server address "pool.ntp.org"
#define SNTP_PERIODIC		3600								// NTP server Periodic Time
#define SNTP_RETRY_CNT		2									// NTP server retry max count
#define SNTP_RETRY_SEC		10									// NTP server retry sec

#define DEVICE_ID			"EN673_"							// UPNP device id
#define CAMERA_NAME			"EN673_30P_FHD"						// UPNP Camera Name

#define	DHCP_USE_ON			0									// DHCP on
#define	DHCP_USE_OFF		1									// DHCP off

#define SDSAVE_USE_ON		0									// SDSave on
#define SDSAVE_USE_OFF		1									// SDSave off

#define FTPD_USE_ON			0									// FTP SERVER on
#define FTPD_USE_OFF		1									// FTP SERVER off

#define DDNS_USE_ON			0									// DDNS on
#define DDNS_USE_OFF		1									// DDNS off

#define DDNS_HOST_NAME		"shyou.dyndns.org"
#define	DDNS_ACCO_ID		"quartus0"
#define DDNS_PW				"firmware"

#define SMTP_USE_ON			0									// SMTP on
#define SMTP_USE_OFF		1									// SMTP off

#define SMTP_SRVR			"mail.eyenix.com"					// SMTP server address
#define SMTP_BOARD_MAIL		"board@eyenix.com"					// Mail address of board
#define SMTP_BOARD_PW		"firmware"

//-------------------------------------------------------------------------------------------------
#define	MAILING_NUM			4									// Number of mailing list

//-------------------------------------------------------------------------------------------------
#define USER_ID					"root"							// Web, FTP login Id
#define USER_PW					"1234"							// Web, FTP login Password

#define LOGIN_MAX_NUM			10								// How many Login Possible
#define LOGIN_SESSION_TIMEOUT	600								// Session Time out (How many secconds maintain session)
#define GUEST1_ID				"guest1"							// Web, FTP login Id
#define GUEST1_PW				"1234"							// Web, FTP login Password

//-------------------------------------------------------------------------------------------------
#define MAGICNUM_SYSTEM			0x5555aaaa
#define MAGICNUM_USER			0xaa55aa55
#define MAGICNUM_ISP			0xa5a5a5a5
#define MAGICNUM_ONVIF			0x55aaaa55
#define MAGICNUM_WIFI			0xaa5555aa

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------


#endif	// _FACTORY_H_
