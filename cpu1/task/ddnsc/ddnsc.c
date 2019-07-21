//*************************************************************************************************
// DDNS client task
//*************************************************************************************************

/* ------------------------ OR32 includes -------------------------------- */
#include <stdio.h>
#include <time.h>

#include "dev.h"
#include "ddnsc.h"
#include "Base64.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"
#include "lwip/stats.h"
#include "lwip/opt.h"
#include "lwip/netifapi.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"
#include "lwip/sockets.h"
#include "lwip/mem.h"
#include "netif/etharp.h"
#include "lwip/autoip.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"
#include "lwip/init.h"

#include "ethernetif.h"

#include "msg.h"
#include "user.h"
#include "smtpc.h"

#if (ENX_DDNSC_use==1)

int ddnsSendFlag;
int ddnsTimeFlag;

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
//
char* DDNSAsk(ip_addr_t atDnsIp, char* myAddr)
{
		u32_t ddns_server_address;
		ddns_server_address = atDnsIp.addr;

		int sock;
//		char ipAddr[16];
//		char authorization[64];
		int timeout;
		char message[512]=
				"GET / HTTP/1.1\r\n"
				"Host: checkip.dyndns.com\r\n\r\n";

		char* messP;

		int str_len;
		struct sockaddr_in serv_addr;

		sock = socket(PF_INET, SOCK_STREAM, 0);
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family		= AF_INET;
		serv_addr.sin_addr.s_addr	= ddns_server_address;
		serv_addr.sin_port 			= htons(HTTP_PORT);

		timeout = DDNS_TCP_TIMEOUT;														// Set recv timeout
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

		connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

		write(sock, message, strlen(message));

		str_len =read(sock, message, sizeof(message));

		if(str_len < 0){
			printf("DDNS:Ask addr timeout!!!\r\n");
		}else{
		messP = message;
			printf("DDNS 05\r\n");
			while(str_len){
				if(str_len>128){
					printf("%.*s",128,messP);
					messP+=128;
					str_len-=128;
				}else{
					printf("%.*s",str_len,messP);
					str_len =0;
				}
			}
		}
		unsigned char ip[MAX_IP_STR_LEN];

		snprintf((char *)ip,strstr(message,"</body>")-(strstr(message,"Current IP Address: ")+19),"%s",strstr(message,"Current IP Address: ")+20);
		printf("\r\nCurrent IP Address : [%s]\r\n", ip);

		snprintf(myAddr,MAX_IP_STR_LEN,(char *)ip);

		close(sock);


		printf("DDNS 08\r\n");
		return myAddr;
}

//-------------------------------------------------------------------------------------------------
//
int DDNSRequest(ip_addr_t atDnsIp, char* ipAddr)
{
		u32_t ddns_server_address;
		ddns_server_address = atDnsIp.addr;

		int sock;
		char authorization[64];
		int timeout;
		char message[512];

		char *base64enc;
		char base64Temp[64];
		size_t outlen;
		snprintf(base64Temp,32,"%s:%s",gtUser.ddnsData.strDdnsAcId, gtUser.ddnsData.strDdnsPw);
		base64enc = base64_encode((const unsigned char *)base64Temp, strlen(base64Temp), &outlen);

		sprintf(message,"GET /nic/update?system=dyndns&hostname=%s&myip=%s HTTP/1.0\r\n",gtUser.ddnsData.strDdnsHostName,ipAddr);
		strcat(message,"Host: members.dyndns.org\r\n");
		sprintf(authorization,"Authorization: Basic %s\r\n",base64enc);
		strcat(message,authorization);
		strcat(message,"User-Agent: myclient/1.0 eyenix.com\r\n\r\n");

		char* messP;

		int str_len;
		struct sockaddr_in serv_addr;

		sock = socket(PF_INET, SOCK_STREAM, 0);
		memset(&serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family		= AF_INET;
		serv_addr.sin_addr.s_addr	= ddns_server_address;
		serv_addr.sin_port 			= htons(HTTP_PORT);

		timeout = DDNS_TCP_TIMEOUT;														// Set recv timeout

		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

		connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

		write(sock, message, strlen(message));

		str_len = read(sock, message, sizeof(message));

		if(str_len < 0){
			printf("DDNS:tcp timeout!!!\r\n");
		}else{
		messP = message;
			printf("DDNS 16\r\n");
			while(str_len){
				if(str_len>128){
					printf("%.*s",128,messP);
					messP+=128;
					str_len-=128;
				}else{
					printf("%.*s",str_len,messP);
					str_len =0;
				}
			}
		}

		close(sock);

		if(strstr(message,"good") != NULL)	return 1;
		if(strstr(message,"nochg") != NULL)	return 1;
		else	return 0;
}


/*
** base64_encodeblock
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
//static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
//void base64_encodeblock( BYTE in[3], BYTE out[4], int len )
//{
//	out[0] = cb64[ in[0] >> 2 ];
//	out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
//	out[2] = (BYTE) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
//	out[3] = (BYTE) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
//}
//-------------------------------------------------------------------------------------------------
/*
** decode
**
** decode a base64 encoded stream discarding padding, line breaks and noise
*/
#if 0
char * base64_encode(char *source, char *destination)
{
	BYTE in[3], out[4];
	int i, n, p,len;

	i=0;
	n=0;
	p=0;
	len=0;

	*destination=0;

	while( source[n]!=0 )
	{
		len = 0;
		for( i = 0; i < 3; i++ )
		{
			in[i] = source[n];
			if( source[n]!=0  )
			{
				len++;
				n++;
			}
			else
			{
				in[i] = 0;
			}
		}

		if( len )
		{
			base64_encodeblock( in, out, len );
			for( i = 0; i < 4; i++ )
			{
				destination[p]= out[i];
				p++;
			}
		}
	}

	destination[p]=0;
	return destination;
}
#endif
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Task
void DdnscTask(void *pvParameters)
{
	network_check_netif();

	ip_addr_t	tDnsIp;
	char		myAddr[16];
	UINT		checkTime = 0;
	int			ddnsSendCnt = 0;

	ddnsSendFlag = 1;
	ddnsTimeFlag = 1;

	for(;;)
	{
		if(gtUser.bUseDdns == DDNS_USE_ON && (ddnsSendFlag||ddnsTimeFlag))
		{
			checkTime = gptMsgShare.TIME;
			if(netconn_gethostbyname("checkip.dyndns.com", &tDnsIp) == ERR_OK)
			{
				if((strcmp(DDNSAsk(tDnsIp,myAddr), (const char *)gtUser.ddnsData.strDdnsDetectedAddr) != 0) || ddnsSendFlag) 		// Get my outter Addr
				{
					snprintf((char *)gtUser.ddnsData.strDdnsDetectedAddr, MAX_IP_STR_LEN, myAddr);		// Save Addr
					if(netconn_gethostbyname("members.dyndns.org", &tDnsIp) == ERR_OK)
					{
						if(DDNSRequest(tDnsIp,myAddr))								// Update DDNS
						{ 
							ddnsSendFlag=0;
							ddnsSendCnt=0;
						}
						else
						{
							ddnsSendCnt++;
							if(ddnsSendCnt == DDNS_RETRY_MAX)
							{
								ddnsSendFlag=0; 
								ddnsSendCnt=0;
							}
						}
					}
				}
				ddnsTimeFlag=0;
			}
		}

		vTaskDelay(100);
		if((checkTime+600) < gptMsgShare.TIME) ddnsTimeFlag = 1;   								// check addr  per 10min
	}

	UNUSED(pvParameters);
}
#endif
