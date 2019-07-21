#ifndef _SHELL_USR_NET_H_
#define _SHELL_USR_NET_H_

#include "shell.h"


//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern INT16S cmd_ifconfig(INT32S argc, char *argv[]);
extern INT16S cmd_dhcp(INT32S argc, char *argv[]);
#if (ENX_SNTPC_use==1)
extern INT16S cmd_sntp(INT32S argc, char *argv[]);
#endif
extern INT16S cmd_netstat(INT32S argc, char *argv[]);
#if (ENX_FTPD_use==1)
extern INT16S UsrFtpdCmd(INT32S argc, char *argv[]);
#endif
#ifdef __WIFI__
extern INT16S cmd_wifistatus(INT32S argc, char *argv[]);
extern INT16S cmd_wifionoff(INT32S argc, char *argv[]);
#endif
#ifdef __ETH__
extern INT16S cmd_mac(INT32S argc, char *argv[]);
extern INT16S cmd_ethctl(INT32S argc, char *argv[]);
#endif
extern INT16S cmd_nettx_test(INT32S argc, char *argv[]);
extern INT16S cmd_rtsp_state(INT32S argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sIfconfigCmd[];
extern const char *sDhcpCmd[];
#if (ENX_SNTPC_use==1)
extern const char *sSntpCmd[];
#endif
extern const char *sNetstatCmd[];
#if (ENX_FTPD_use==1)
extern const char *sFtpdCmd[];
#endif
#ifdef __WIFI__
extern const char *sWiFiStatus[];
extern const char *sWiFiSW[];
#endif
#ifdef __ETH__
extern const char *sMacCmd[];
extern const char *sEthCmd[];
#endif
extern const char *sNetTx[];
extern const char *sRtspState[];

#endif