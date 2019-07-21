//*************************************************************************************************
// Dhcp client task
//*************************************************************************************************

#include "dev.h"
#include "dhcpc.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "lwip/memp.h"
#include "lwip/stats.h"
#include "lwip/dhcp.h"
#include "lwip/dns.h"

#include "ethernetif.h"

#include "msg.h"
#include "user.h"

#include "init.h"
#include "httpd.h"
#include "upnpd.h"
#include "ipchange.h"

extern UINT uiUserinfoUpdate;

//*************************************************************************************************
// Dhcp
//-------------------------------------------------------------------------------------------------
//
void DhcpcTask(void *pvParameters)
{
	struct netif *ptNetIf = (struct netif *)pvParameters;

	if(bitcheck(uiUserinfoUpdate, UIN_NW_INT_DHCP))
	{
		uiUserinfoUpdate = bitchang(uiUserinfoUpdate, UIN_NW_INT_DHCP, 0);
		printf("DHCP Task : Delay\r\n");
		vTaskDelay(100);	// 웹브라우저 변경 중....
	}
//	vTaskDelay(100);	

	while(gptMsgShare.ETH_ON != 1)
		vTaskDelay(1);

	UpnpRecvMsg = upnpudp_byebye;

	do {
		printf("DHCP Task : dhcp_start\r\n");

		if (netif_is_up(ptNetIf)) {
			netif_set_down(ptNetIf);
		}

		ip_addr_set_zero(&ptNetIf->ip_addr);
		ip_addr_set_zero(&ptNetIf->netmask);
		ip_addr_set_zero(&ptNetIf->gw);

		err_t ert = dhcp_start(ptNetIf);
		if (ert != ERR_OK) {
			printf("DHCP ERROR (%d)\r\n", ert);
		}

		vTaskDelay(300);

		while (!netif_is_up(ptNetIf)) {
			vTaskDelay(1);
		}

		ip_addr_t add1 = dns_getserver(0);
		ip_addr_t add2 = dns_getserver(1);

		gtUser.nIpAddr	= ptNetIf->ip_addr.addr;
		gtUser.nNetMask = ptNetIf->netmask.addr;
		gtUser.nGateway = ptNetIf->gw.addr;
		gtUser.nDnsSvr0 = add1.addr;
		gtUser.nDnsSvr1 = add2.addr;

		printf("DHCP : IP set : %IP\r\n", ptNetIf->ip_addr.addr);
		printf("DHCP : SM set : %IP\r\n", ptNetIf->netmask.addr);
		printf("DHCP : GW set : %IP\r\n", ptNetIf->gw.addr);
		printf("DHCP : DNS0 set : %IP\r\n", add1.addr);
		printf("DHCP : DNS1 set : %IP\r\n", add2.addr);

		while(MsgLock() == DEF_FAIL)
		{
			vTaskDelay(1);
		}

		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_IP, ptNetIf->ip_addr.addr) == DEF_FAIL) 	vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_SM, ptNetIf->netmask.addr) == DEF_FAIL)		vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_GW, ptNetIf->gw.addr) == DEF_FAIL)			vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS0, add1.addr) == DEF_FAIL)				vTaskDelay(1);
		while(MsgPut_nolock(&gptMsgCpu1to0, MSG_CMD_SET_NET_DNS1, add2.addr) == DEF_FAIL)				vTaskDelay(1);

		MsgUnlock();

		UserSave(sfls_USER);

//		dhcp_stop(ptNetIf);
//		dhcp_inform(ptNetIf);
	
		vTaskDelete(NULL);	// Task Kill

		while(netif_is_up(ptNetIf))	// 된다면 무한반복, 안되면 반복문 탈출
		{
			vTaskDelay(10);
			printf("[%X]", ptNetIf->flags);
		}

	}while(1);

	vTaskDelete(NULL);
}
