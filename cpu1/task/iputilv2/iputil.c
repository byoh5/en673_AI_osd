#include "dev.h"
#include "dev_inline.h"	// to use fast code

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
#include "lwip/inet.h"
#include "lwip/init.h"

#include "ddnsc.h"
#include "user.h"
#include "net_ascii.h"
//#include <time.h>
#include "timer_task.h"
#include "ipchange.h"
#include "iputil.h"
#include "dhcpc.h"

#include "init.h"
#include "msg.h"
#include <time.h>

#define IPUTIL_DEBUG
#ifdef IPUTIL_DEBUG
	#define iprintf(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define iprintf(fmt, args...) do {} while(0);
#endif


#ifdef __ETH__
/* ========  Import Valiables ===================*/
extern UINT uiUserinfoUpdate;

/* ========  Internal Valiables =================*/
static sys_thread_t   s_hIpUtilTask = NULL;
static unsigned long  s_xid  = 0;
static unsigned long  s_curXid;

/* ==============================================*/
/* ==== Internal Functions ======================*/
/* ==============================================*/
static u_int32_t get_newxid(void)
{
	return  (gptMsgShare.TIME);
}

static void make_packet(NETCONF_MESSAGE *packet, char opcode)
{
	char strNodeName[64];

	memset(packet, 0, sizeof(NETCONF_MESSAGE));
	packet->opcode = opcode;

	if (gtUser.bUseDhcp == DHCP_USE_ON)
		packet->nettype = E_NET_DHCP;
	else if (gtUser.bUseDhcp == DHCP_USE_OFF)
		packet->nettype = E_NET_STATIC;
	else
		packet->nettype = 0;

	packet->http_port = htonl(gtUser.nHttpPort);
	packet->xid = htonl(get_newxid());
	packet->magic = htonl(MAGIC);

	packet->ciaddr = gtUser.nIpAddr;// current ip address
	packet->miaddr = gtUser.nNetMask;// current netmask address
	packet->giaddr = gtUser.nGateway;
	memcpy(packet->chaddr, gtMyNetIf.hwaddr, 6);

	packet->d1iaddr = gtUser.nDnsSvr0;
	packet->d2iaddr = gtUser.nDnsSvr1;
	packet->d3iaddr = 0; // get from resolv.conf

	memset (strNodeName, 0, sizeof(strNodeName));
	strcpy (strNodeName, "cp100");

	sprintf((char *)(packet->vend), "%s; %s; %s; %s; %s",
			strNodeName, "svrsysname", "svrrelease", "svrversion", "svrmachine");

	packet->stream_port = htonl(gtUser.nRtspPort);

	packet->flag = 0x1 & gtUser.autoipoffon;	// 20140915 dhchai add

	// 20140915 dhchai add
	if(gtUser.bUseDhcp == DHCP_USE_OFF)
		packet->flag &= 0xFFFD;	// dhcp Disable
	else
		packet->flag |= ((0x1)<< 1);

	memset(packet->fw_ver, 0, sizeof(packet->fw_ver));
	char version[20];
	sprintf(version, "%d", gtSystem.nVersion);
	strcpy ((char *)packet->fw_ver, version/*SysVersion*/);	
	packet->running_time = (unsigned long)gptMsgShare.TIME - (unsigned long)gtUser.runningTime;
	//printf(" ADD : gptMsgShare.TIME = %u, gtUser.runningTime = %d, gptMsgShare.TIME - gtUser.runningTime=%d \r\n",gptMsgShare.TIME,gtUser.runningTime,(unsigned long)gptMsgShare.TIME - (unsigned long)gtUser.runningTime);
}


static int send_packet(char *pbuf, int len, struct sockaddr *pto)
{
	//printf("%s\r\n",__func__);
	int i, send_sock = -1;
	struct sockaddr_in addr;
	struct ip_mreq mreq;
	for( i = 0 ; i < 5 ; i++ )
	{
		if (send_sock < 0)
		{
			send_sock = socket(AF_INET, SOCK_DGRAM, 0);
			if(send_sock == -1)
			{
				iprintf("Iputil : UDP send_socket error! retry...\n");
				vTaskDelay(100);
				continue;
			}
			break;
		}
	}

	if (send_sock < 0)
	{
		return -1;
	}

	mreq.imr_multiaddr.s_addr = htonl(INADDR_BROADCAST);
	mreq.imr_interface.s_addr = htonl(INADDR_BROADCAST);

	if(setsockopt(send_sock, SOL_SOCKET ,SO_BROADCAST,&mreq,sizeof(mreq))==-1){
		//printf ("ERROR iputil : setsockopt\n");
		//vTaskDelay(200);
		closesocket (send_sock);
		return -2;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_len = sizeof(struct sockaddr_in);
	addr.sin_port = (IP_UTIL_SEND_PORT);
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	//printf("addr.sin_len %d addr.sin_port %d\r\n", addr.sin_len, addr.sin_port);

	if(connect(send_sock, (struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1)
	{
		iprintf ("ERROR iputil : send_packet - connect\n");
		//vTaskDelay(200);
		closesocket (send_sock);
		return -2;
	}

	if (sendto( send_sock, pbuf, len,  0, (struct sockaddr *)&addr, sizeof(addr)) == -1)
	{
		iprintf ("ERROR iputil : send_packet - sendto\n");
		//vTaskDelay(200);
		closesocket (send_sock);
		return -3;
	}

	//vTaskDelay(200);

	closesocket (send_sock);
	return 0;
	UNUSED(pto);

}

static int setup_network(NETCONF_MESSAGE *packet, void *ext)
{
    int i;
	int bMacSame = 1;

	//printf("setup network packet->nettype %d\r\n",packet->nettype);

	for (i=0; i<6; i++)
	{
		if (packet->chaddr[i] != gtMyNetIf.hwaddr[i])
		{
			bMacSame = 0;
			break;
		}
	}

	if (packet->nettype == E_NET_STATIC)
	{
		UINT tmpUiUserinfoUpdate = 0;
		if (bMacSame && (packet->yiaddr != gtUser.nIpAddr))
		{
			//printf ("Set Network IP....\r\n");
			tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_IP, 1);
			gtUser.nIpAddr = packet->yiaddr;
			ip_addr_t TempValue;
			TempValue.addr = gtUser.nIpAddr;
			netif_set_ipaddr(&gtMyNetIf, &TempValue);
		}

		if (bMacSame && (packet->miaddr != gtUser.nNetMask))
		{
			//printf ("Set Network Netmask ...\r\n");
			tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_SN, 1);
			gtUser.nNetMask = packet->miaddr;
			ip_addr_t TempValue;
			TempValue.addr = gtUser.nNetMask;
			netif_set_netmask(&gtMyNetIf, &TempValue);
		}

		if (bMacSame && (packet->giaddr != gtUser.nGateway))
		{
			//printf ("Set Network Gateway ...\r\n");
			tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_GW, 1);
			gtUser.nGateway = packet->giaddr;
			ip_addr_t TempValue;
			TempValue.addr = gtUser.nGateway;
			netif_set_gw(&gtMyNetIf, &TempValue);
		}

		if (packet->d1iaddr != gtUser.nDnsSvr1)
		{
			//printf ("Set Network DNS0 ...\r\n");
			tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_DNS0, 1);
			gtUser.nDnsSvr0 = packet->d1iaddr;
		}

		if (packet->d1iaddr  != gtUser.nDnsSvr1)
		{
			//printf ("Set Network DNS1 ...\r\n");
			tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_DNS1, 1);
			gtUser.nDnsSvr1 = packet->d2iaddr;
		}

#if 0
		if (packet->miaddr == gtUser.nIpAddr)
		{
			for (i = 0; i<6; i++)
			{
				if (packet->chaddr[i] != gtMyNetIf.hwaddr[i])
				{

					BYTE *bUsrMacBuf;
					int ret=0;
					unsigned char *p = packet->chaddr;

					printf("CMD_SET_MAC_ADDR :%x,%x,%x,%x,%x,%x \n",p[0], p[1], p[2], p[3], p[4], p[5]);

					bUsrMacBuf=(BYTE *)malloc(4096);
					memset(bUsrMacBuf, 0, 4096);

					memcpy (gbMacAddr, p, 6);
					EthSetMacAdr(gbMacAddr);
					UserMacSave(bUsrMacBuf);

					free(bUsrMacBuf);
					vTaskDelay(50);

					WdtReset();

					break;
				}
			}
		}
#endif
		enx_ipchange_proc(enlETHERNET, tmpUiUserinfoUpdate);

		vTaskDelay(10);
		UserSave(sfls_USER);
		vTaskDelay(10);	

		iprintf ("info : curXid(%x) xid (%x), sec(%d), nettype(%d)\r\n", s_curXid, packet->xid, packet->secs, packet->nettype);
		iprintf ("info : ip(%s), newip(%s)\r\n", inet_ntoa32(packet->ciaddr), inet_ntoa32(packet->yiaddr));
		iprintf ("info : miaddr(%s), giaddr(%s)\r\n", inet_ntoa32(packet->miaddr), inet_ntoa32(packet->giaddr));
		iprintf ("info : d1iaddr(%s), d2iaddr(%s)\r\n", inet_ntoa32(packet->d1iaddr), inet_ntoa32(packet->d2iaddr));
		iprintf ("info : chaddr(%s), chaddr2(%s)\r\n", inet_ntoa32(packet->chaddr), inet_ntoa32(packet->chaddr2));
		iprintf ("sinfo : ip(%s)\r\n", inet_ntoa32(gtUser.nIpAddr));
		iprintf ("sinfo : miaddr(%s), giaddr(%s)\r\n", inet_ntoa32(gtUser.nNetMask), inet_ntoa32(gtUser.nGateway));
		iprintf ("sinfo : d1iaddr(%s), d2iaddr(%s)\r\n", inet_ntoa32(gtUser.nDnsSvr0), inet_ntoa32(gtUser.nDnsSvr1));
	}
	else if (packet->nettype == E_NET_DHCP)
	{
		iprintf ("ERROR : iputil - Not Implementd DHCP!!!\n");
	}
	else if (packet->nettype == E_NET_PPPoE)
	{
		iprintf ("ERROR : iputil - Implementd POE!!!\n");
	}

	return 0;
	UNUSED(ext);
}

static int setup_Reboot()
{
	iprintf("System Reboot!\r\n\r\n");
	vTaskDelay(10);
	WdtSysReboot();

	return 0;
}

static int setup_macaddr(NETCONF_MESSAGE *packet, void *ext)
{
	iprintf("setup_macaddr packet->nettype %d\r\n",packet->nettype);
    int i;
	int bChanged = DEF_NO;
	if (packet->nettype == E_NET_STATIC)
	{
		iprintf ("sinfo : gtUser ip(%s)\r\n", inet_ntoa32(gtUser.nIpAddr));
		iprintf ("sinfo : packet yiaddr(%s)\r\n", inet_ntoa32(packet->yiaddr));
		iprintf ("sinfo : packet ciaddr(%s)\r\n", inet_ntoa32(packet->ciaddr));

		if (packet->ciaddr == gtUser.nIpAddr)
		{
			iprintf("packet->ciaddr == gtUser.nIpAddr\r\n");
			for (i=0; i<6; i++)
			{
				if (packet->chaddr[i] != packet->chaddr2[i])
				{
					iprintf("bChanged = DEF_YES\r\n");
					bChanged = DEF_YES;
					break;
				}
			}

			if (bChanged == DEF_YES)
			{
				bChanged = DEF_YES;
				for (i = 0; i<6; i++)
				{
					iprintf("packet->chaddr[%d] %x gtMyNetIf.hwaddr[%d] %x \r\n",i,packet->chaddr[i],i,gtMyNetIf.hwaddr[i]);
					if (packet->chaddr[i] != gtMyNetIf.hwaddr[i])
					{
						iprintf("bChanged = DEF_NO\r\n");
						bChanged = DEF_NO;
						break;
					}
				}

				if (bChanged == DEF_YES)
				{
					for (i = 0; i<6; i++)
					{
						iprintf("packet->chaddr2[%d] %x gtMyNetIf.hwaddr[%d] %x \r\n",i,packet->chaddr2[i],i,gtMyNetIf.hwaddr[i]);
						if (packet->chaddr2[i] != gtMyNetIf.hwaddr[i])
						{
							iprintf("bChanged = DEF_YES3\r\n");
							bChanged = DEF_YES;
							break;
						}
					}
					if (bChanged == DEF_YES)
					{
						BYTE *p = packet->chaddr2;

						iprintf("CMD_SET_MAC_ADDR :%x,%x,%x,%x,%x,%x \n",p[0], p[1], p[2], p[3], p[4], p[5]);

						memcpy((void *)gtSystem.nMacAddress, p, 6);
						UserSave(sfls_SYSTEM);
						WdtSysReboot();
					}
				}
			}
		}
//		sys_thread_new("ichange", IpChangeTask, &gtMyNetIf, _16KB_STACK_SIZE, CHANGE_TASK_PRIO);
	}

	return 0;
	UNUSED(ext);
}

static int setup_AutoIp(NETCONF_MESSAGE *packet, void *ext)
{
    int i;
	int bChanged = DEF_NO;
	int bMacSame = 1;

	for (i=0; i<6; i++)
	{
		if (packet->chaddr[i] != gtMyNetIf.hwaddr[i])
		{
			bMacSame = 0;
			break;
		}
	}

	if (packet->nettype == E_NET_STATIC)
	{
		if (bMacSame && ((packet->flag & 0x1) != gtUser.autoipoffon))
		{
			bChanged = DEF_YES;

			if( packet->flag & ((0x1)<< 0))
				gtUser.autoipoffon |= (0x1)<<0;
			else
				gtUser.autoipoffon &= 0xFFFE;

			if (bChanged == DEF_YES)
			{
				bChanged = DEF_NO;
				UserSave(sfls_USER);
			}
		}
	}

	return 0;
	UNUSED(ext);
}

static int setup_Dhcp(NETCONF_MESSAGE *packet, void *ext)
{
    int i;
	int bChanged = DEF_NO;
	int bMacSame = 1;
	UINT tmpUiUserinfoUpdate = 0;
	struct netif *ptNetIf = (struct netif*)&gtMyNetIf;

	//printf("1DHCHAI ADD : %s,Line=%d, flag=%d\n",__FUNCTION__,__FILE__,packet->flag);

	for (i=0; i<6; i++)
	{
		if (packet->chaddr[i] != gtMyNetIf.hwaddr[i])
		{
			bMacSame = 0;
			break;
		}
	}

	if (packet->nettype == E_NET_STATIC)
	{
		if (bMacSame && (((packet->flag & 0x2)>>1) != !(gtUser.bUseDhcp)))
		{
			//printf("2DHCP CHANGE : %s,Line%d, flag=%d\n",__FUNCTION__,__FILE__,packet->flag);

			bChanged = DEF_YES;

			// gtUser.bUseDhcp == 0? Enable:Disable
			gtUser.bUseDhcp = !((packet->flag & 0x2)>>1);

			if (gtUser.bUseDhcp == DHCP_USE_OFF)
			{
				bChanged = DEF_NO;

				dhcp_stop(ptNetIf);

				gtUser.nIpAddr	= ipaddr_addr(IP_ADDRESS);	// IP addr
				gtUser.nNetMask = ipaddr_addr(NET_MASK);	// Net mask addr
				gtUser.nGateway = ipaddr_addr(GW_ADDRESS);	// Gateway addr
				gtUser.nDnsSvr0 = ipaddr_addr(DNS_SRVR0);
				gtUser.nDnsSvr1 = ipaddr_addr(DNS_SRVR1);

				//printf("DeviceId changed \r\n");
				ip_addr_t dns1, dns2;
				dns1.addr = gtUser.nDnsSvr0;
				dns2.addr = gtUser.nDnsSvr1;
				dns_setserver(0, &dns1);							// Set DNS server 0 addr
				dns_setserver(1, &dns2);							//		"		  1  "
				//printf("dns changed \r\n");
				dhcp_inform(ptNetIf);

				//printf("dhcp informed \r\n");
				tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_IP, 1);
				tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_SN, 1);
				tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_GW, 1);
				tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_DNS0, 1);
				tmpUiUserinfoUpdate = bitchang(tmpUiUserinfoUpdate, UIN_NW_INT_DNS1, 1);

				enx_ipchange_proc(enlETHERNET, tmpUiUserinfoUpdate);
				//printf("ip changed \r\n");
			}
			else
			{
				sys_thread_new("Dhcpc", DhcpcTask, &gtMyNetIf, DHCPC_STACK_SIZE, DHCPC_TASK_PRIO);
			}

			//printf("gtUser.bUseDhcp=%d\n",gtUser.bUseDhcp);
		}

		UserSave(sfls_USER);

	}
	//printf("3DHCHAI ADD : %s,Line%d\n",__FUNCTION__,__FILE__);

	return 0;
	UNUSED(ext);
}

void IpUtilTask(void *pvParameters)
{
	int recv_sock = -1, recv_len, slen = 0;
	struct sockaddr_in addr, to;
	NETCONF_MESSAGE rnet_conf, snet_conf;
	unsigned long searchXid = 0;
	struct netif* ptNetIf = (struct netif *)pvParameters;
	int  ret;
	struct tm tmout;

	int update_sock = -1, update_len;
	struct sockaddr_in update_addr, update_to;
	char buffer;

	iprintf("IpUtilTask() ++\r\n");

	while(!netif_is_up(ptNetIf))
	{
	    iprintf("Waiting network is active\r\n");
		vTaskDelay(10);	// Network is ready ?
	}

	vTaskDelay(500);

	gtUser.runningTime = gptMsgShare.TIME;

	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);


	tmout.tm_mon = tmout.tm_mon - 1;
	tmout.tm_year = tmout.tm_year - 1900;

	s_xid = mktime(&tmout);

	while(1)
	{
		if (recv_sock < 0)
		{
			recv_sock = socket(AF_INET, SOCK_DGRAM, 0);

			if(recv_sock == -1)
			{
				iprintf("Iputil : UDP recv_socket error! retry...\r\n");
				vTaskDelay(20);
				continue;
			}
		}

#if 0 //REUSEADD Not implemented..
 		if (setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, (void *) &opt, sizeof(opt)) == -1)
        {
        	printf ("ERROR : %s - SO_RESUSERADDR", __FUNCTION__);
	  		close (recv_sock);
	  		continue;
    	}
#endif
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr =  htonl(INADDR_ANY);	//IPADDR_BROADCAST;//INADDR_ANY;
		addr.sin_port = htons(IP_UTIL_LISTEN_PORT);
		iprintf("bind() ++\r\n");

		IGMP_Join("239.255.255.250");

		ret = bind(recv_sock, (struct sockaddr *)&addr, sizeof(addr));
		if( ret == -1)
		{
			iprintf("Iputil : UDP bind error! retry...\r\n");
			close(recv_sock);
			vTaskDelay(20);
			continue;
		}

		break;
	}

	recv_len = sizeof(to);
	while (1)
	{
		iprintf("recvfrom() ++\r\n");
		slen  = recvfrom(recv_sock, &rnet_conf, sizeof(rnet_conf), 0, (struct sockaddr*)&to, (socklen_t*)&recv_len);

		if(htonl(rnet_conf.magic) != MAGIC)
		{
			iprintf ("iputil : skip - mismatch magic\r\n");
			continue;
		}

		if (rnet_conf.opcode == MSG_IP_SEARCH)
		{
			iprintf ("iputil : MSG_IP_SEARCH searchXid %d rnet_conf.xid %d\r\n",searchXid,rnet_conf.xid);
			if (searchXid == rnet_conf.xid)
				continue;

			searchXid = rnet_conf.xid;
			make_packet(&snet_conf, MSG_CAM_ACK);
			send_packet((char *)&snet_conf, sizeof(snet_conf), (struct sockaddr *)&to);
			s_curXid = snet_conf.xid;
			iprintf ("iputil : MSG_IP_SEARCH E s_curXid %d\r\n",s_curXid);
		}
		else if (rnet_conf.opcode == MSG_IP_SET)
		{
			iprintf ("iputil : MSG_IP_SET\r\n");
			if (s_curXid == rnet_conf.xid)
			{
				setup_network(&rnet_conf, 0);
				s_curXid = 0;
			}
		}
		else if (rnet_conf.opcode == MSG_IP_ACK)
		{
			iprintf ("iputil : MSG_IP_ACK\r\n");
		}
		else if (rnet_conf.opcode == MSG_IP_REBOOT)
		{
			iprintf ("iputil : MSG_IP_REBOOT\r\n");
			if (s_curXid == rnet_conf.xid)
			{
				setup_Reboot();
				s_curXid = 0;
			}
		}
		else if (rnet_conf.opcode == MSG_IP_UPDATE)
		{
			iprintf ("iputil : MSG_IP_UPDATE\r\n");
			if (s_curXid == rnet_conf.xid)
			{
				while(1)
				{
					if (update_sock < 0)
					{
						update_sock = socket(AF_INET, SOCK_DGRAM, 0);

						if(update_sock == -1)
						{
							iprintf("Iputil : UDP recv_socket error! retry...\r\n");
							vTaskDelay(20);
							continue;
						}
					}

					memset(&update_addr, 0, sizeof(update_addr));
					update_addr.sin_family = AF_INET;
					update_addr.sin_addr.s_addr = to.sin_addr.s_addr; //htonl(INADDR_ANY);	//IPADDR_BROADCAST;//INADDR_ANY;
					update_addr.sin_port = htons(IP_UTIL_UPDATE_PORT);
					iprintf("bind() ++\r\n");

					connect(update_sock, (struct sockaddr*)&update_addr, sizeof(update_addr));

					break;
				}
				update_len = sizeof(update_to);
				while(1)
				{
					buffer = fwupdate_process();
					slen = sendto(update_sock, &buffer, sizeof(buffer), 0, NULL, 0);
					vTaskDelay(100);
					if(buffer == 100)
						break;
				}
				//vTaskDelay(10);
				//s_curXid = 0;
			}
		}
		else if (rnet_conf.opcode == MSG_IP_MACUPDATE)
		{
			iprintf ("iputil : MSG_IP_MACUPDATE\r\n");
			if (s_curXid == rnet_conf.xid)
			{
				setup_macaddr(&rnet_conf, 0);
				s_curXid = 0;
			}
		}
		else if (rnet_conf.opcode == MSG_CAM_AUTOIP)
		{
			iprintf ("iputil : MSG_CAM_AUTOIP\r\n");
			if (s_curXid == rnet_conf.xid)
			{
				setup_AutoIp(&rnet_conf, 0);
				s_curXid = 0;
			}
		}
		else if (rnet_conf.opcode == MSG_IP_DHCP)
		{
			iprintf("DHCP CHANGE : rnet_conf.opcode=%d rnet_conf.xid=%d, s_curXid=%d\r\n",rnet_conf.opcode,rnet_conf.xid,s_curXid);
			if (s_curXid == rnet_conf.xid)
			{
				setup_Dhcp(&rnet_conf, 0);
				s_curXid = 0;
			}
		}

		vTaskDelay(20);
	}
	UNUSED(pvParameters);
}


/* ===============================================*/
/* ========  External Functions  =================*/
/* ===============================================*/
void IpUtil_Init(void)
{
    if(s_hIpUtilTask == NULL)
    {
        s_hIpUtilTask = sys_thread_new("Iputil", IpUtilTask, &gtMyNetIf, _16KB_STACK_SIZE, IPUTIL_TASK_PRIO);
    }
}

void IpUtil_Deinit(void)
{
    if(s_hIpUtilTask != NULL)
    {
        vTaskDelete(s_hIpUtilTask);
		s_hIpUtilTask = NULL;
    }
}
#endif
