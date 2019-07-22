//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "msg.h"

#ifdef	DEF_CPU1	//...........................
#include "FreeRTOS.h"							//
#include "task.h"								//
#include "queue.h"								//
#include "semphr.h"								//
#endif	// ......................................

//SH0RAM_BASE======================================================================================
SH0SRD volatile msgq_net_rx_shared_t gptMsgNetworkRXShared;
SH0SRD volatile msgq_net_rx_t gptMsgNetworkRX0;					// (RX) Ethernet packet CPU0 -> CPU0
SH0SRD volatile msgq_net_rx_t gptMsgNetworkRX1;					// (RX) Ethernet packet CPU0 -> CPU1
#ifdef __WIFI__
SH0SRD volatile msgq_eth_t gptMsgEthCpu1to0;					// (TX) Wi-Fi packet CPU1 -> CPU0
#endif
SH0SRD volatile msgq_pass_t	gptMsgCpu0to1;						// Job : cpu0 -> cpu1
SH0SRD volatile msgq_pass_t	gptMsgCpu1to0;						// Job : cou1 -> cpu0
#ifdef __DEVICE_SD__
SH0SRD volatile msgq_record_t gptMsgRecInfo;
#endif
#if (ENX_FTPD_use==1)
SH0SRD volatile cop2cpu_ftpdmb_t ftpd_mb;
#endif
SH0SRD volatile msgq_shell_t gptMsgShell;
SH0SRD volatile BYTE gbMnParTbl[512];


//SH1RAM_BASE======================================================================================
SH1SRD volatile share_msg_t gptMsgShare;
SH1SRD volatile isp_msg_t gptMsgISPTbl;
#if (NET_TX_BUFFER==1)
SH1SRD volatile msgq_net_tx_t gptMsgEthTx;						// (TX) CPU0 only - 1ms 1packet
#endif
#ifndef DEF_BOOT
#if (RTSPoverHTTP==1)
SH1SRD volatile msgq_tunnel_t gptMsgTunnel;
SH1SRD volatile msgq_txtosd_t gptMsgTxtOsd;
SH1SRD volatile msgq_tunnel_port_t tunnel_port[MSG_TUNNEL_NUM];
#endif
#endif
#ifdef __WIFI__
SH1SRD volatile msgq_wlif_cmd_t gptMsgWlifCmd;
SH1SRD volatile msgq_wlif_evt_t gptMsgWlifEvt;

#endif
SH1SRD volatile BYTE gptMsgSH01[4096];
SH1SRD volatile BYTE gptMsgSH02[4096];
SH1SRD volatile BYTE gptMsgSH03[4096];
SH1SRD volatile BYTE gptMsgSH04[4096];
SH1SRD volatile debug_msg_t gptMsgDebug;

//=================================================================================================

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
#ifndef DEF_BOOT
// Message ram clear
#ifdef DEF_CPU0
void InitShareSram(void)
{
	DmaMemSet_ip((BYTE *)SH0RAM_BASE, 0, SH0RAM_SIZE);
#ifdef JTAG_DEBUG_PRINT
	gptMsgShare.JPEG_STILL_ADDR = 0;
	gptMsgShare.JPEG_STILL_SIZE = 0;
	gptMsgShare.JPEG_STILL_FLAG = 0;
	gptMsgShare.WIFI_INTER_LOG = 0;
	gptMsgShare.JPEG_STILL_REF = 0;
	DmaMemSet_ip((BYTE *)(SH1RAM_BASE+0x28), 0, SH1RAM_SIZE-0x28);
#else
	DmaMemSet_ip((BYTE *)(SH1RAM_BASE), 0, SH1RAM_SIZE);
#endif
	gptMsgShare.MSG_DUMMY = 0x5AA55AA5;

	InitMsg(&gptMsgCpu1to0);
	InitMsg(&gptMsgCpu0to1);
	InitMsgShell(&gptMsgShell);

	gptMsgNetworkRX0.base = NETRX0_BUF_BASE;
	gptMsgNetworkRX0.end = NETRX0_BUF_END;
	gptMsgNetworkRX0.head_pos = NETRX0_BUF_BASE;

	gptMsgNetworkRX1.base = NETRX1_BUF_BASE;
	gptMsgNetworkRX1.end = NETRX1_BUF_END;
	gptMsgNetworkRX1.head_pos = NETRX1_BUF_BASE;

#if (NET_TX_BUFFER==1)
	gptMsgEthTx.base = NETTX_BUF_BASE;
	gptMsgEthTx.end = NETTX_BUF_END;
	gptMsgEthTx.head_pos = NETTX_BUF_BASE;
	gptMsgEthTx.tot_num = MSG_NETTX_NUM;
#endif

#if (RTSPoverHTTP==1)
	gptMsgTunnel.tot_num = MSG_TUNNEL_NUM;
	gptMsgTxtOsd.tot_num = MSG_TUNNEL_NUM;

#endif

#ifdef __DEVICE_SD__
	gptMsgRecInfo.tot_num = MSG_RECORD_NUM;
	gptMsgRecInfo.tail = 0;
	gptMsgRecInfo.head = 0;
#endif
}

//*************************************************************************************************
// Console message(CPU0->CPU1)
//-------------------------------------------------------------------------------------------------
void InitMsgShell(volatile msgq_shell_t *p)
{
	p->tot_num = MSG_SHELL_NUM;
}
#endif

//*************************************************************************************************
// Job message
//-------------------------------------------------------------------------------------------------
#ifdef DEF_CPU0
void InitMsg(volatile msgq_pass_t *p)
{
	p->tot_num = MSGQ_PASS_NUM;
}

#endif

#ifdef DEF_CPU1
static xSemaphoreHandle xSemPut = NULL;
static xSemaphoreHandle xSemGet = NULL;
void InitMsgOS(void)
{
	if(xSemPut==NULL)
		vSemaphoreCreateBinary(xSemPut);
	if(xSemGet==NULL)
		vSemaphoreCreateBinary(xSemGet);
}
#endif

UINT MsgPut_nolock(volatile msgq_pass_t *p, UINT anCmd, UINT anArg)
{
	UINT bRes = DEF_FAIL;

	if(cQueue_isfull(p) != DEF_OK)
	{
#ifdef DEF_CPU1
		if(xSemaphoreTake(xSemPut, portMAX_DELAY))
		{
#else
		CRITICAL_BEGIN;
#endif

		p->cmd[p->tail] = anCmd;
		p->arg[p->tail] = anArg;

		num_loop(p->tail, p->tot_num);

		bRes = DEF_OK;

#ifdef DEF_CPU1
		xSemaphoreGive(xSemPut);
		}
#else
		CRITICAL_END;
#endif
	}

	return bRes;
}


UINT MsgPut(volatile msgq_pass_t *p, UINT anCmd, UINT anArg)
{
	UINT bRes = DEF_FAIL;

	if(MsgLock() == DEF_OK)
	{
		if(cQueue_isfull(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemPut, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			p->cmd[p->tail] = anCmd;
			p->arg[p->tail] = anArg;

			num_loop(p->tail, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemPut);
			}
#else
		CRITICAL_END;
#endif
		}
		MsgUnlock();
	}

	return bRes;
}

UINT MsgGet(volatile msgq_pass_t *p, UINT *anCmd, UINT *anArg)
{
	UINT bRes = DEF_FAIL;

	if(MsgLock() == DEF_OK)
	{
		if(cQueue_isempty(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemGet, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			*anCmd = p->cmd[p->head];
			*anArg = p->arg[p->head];
			p->cmd[p->head] = 0;
			p->arg[p->head] = 0;

			num_loop(p->head, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemGet);
			}
#else
			CRITICAL_END;
#endif
		}
		MsgUnlock();
	}

	return bRes;
}

#ifdef __DEVICE_SD__
#ifdef DEF_CPU0
UINT MsgRecPut(UINT addr, UINT size, UINT ts, UINT type)
{
	static UINT old_time = 0;
	static UINT drop_count = 0;

	if (gptMsgShare.TIME != old_time) {
		old_time = gptMsgShare.TIME;
		if (drop_count != 0) {
			printf("%u items for recording are dropped.\r\n", drop_count);
			drop_count = 0;
		}
	}

	UINT bRes = DEF_FAIL;
	while (StmRecMsgLock() == DEF_FAIL);
	if(cQueue_isfull(&gptMsgRecInfo) != DEF_OK) {
		stream_info *info = (stream_info *)&gptMsgRecInfo.info[gptMsgRecInfo.tail];
		info->addr = addr;
		info->size = size;
		info->ts = ts;
		info->type = type;
		num_loop(gptMsgRecInfo.tail, gptMsgRecInfo.tot_num);
		bRes = DEF_OK;
	} else {
		drop_count++;
	}
	StmRecMsgUnlock();
	return bRes;
}
#endif

#ifdef DEF_CPU1
UINT MsgRecGet(stream_info *getInfo)
{
	UINT bRes = DEF_FAIL;
	while (StmRecMsgLock() == DEF_FAIL);
	if(cQueue_isempty(&gptMsgRecInfo) != DEF_OK) {
		stream_info *info = (stream_info *)&gptMsgRecInfo.info[gptMsgRecInfo.head];
		getInfo->addr = info->addr;
		getInfo->size = info->size;
		getInfo->ts = info->ts;
		getInfo->type = info->type;
		num_loop(gptMsgRecInfo.head, gptMsgRecInfo.tot_num);
		bRes = DEF_OK;
	}
	StmRecMsgUnlock();
	return bRes;
}
#endif
#endif

#ifdef __WIFI__

#ifdef DEF_CPU1
void cpu2cop_wlif_execmd(char *cmd)
{
    int len;
    while(gptMsgWlifCmd.ready)	vTaskDelay(1);
    len = strlen(cmd);
    memcpy((BYTE *)gptMsgWlifCmd.cmd, cmd, len+1);
    gptMsgWlifCmd.ready = 1;
    while(gptMsgWlifCmd.ready)	vTaskDelay(1); //this variable should be alway volatile type
    //gptMsgWlif->ready = 0;
}
#endif

#ifdef DEF_CPU0
int cop2cpu_wlif_getcmd(char *cmd, UINT maxlen)
{
	if(gptMsgWlifCmd.ready == 1)
	{
		UINT len = strlen((char *)gptMsgWlifCmd.cmd);
		if(len >= maxlen)
		{	// developer check code
			printf("%s(%d) : cmd length over(max:%d, len:%d)\r\n", __func__, __LINE__, maxlen, len);
			while(1);
		}
		memcpy(cmd, (const void *)gptMsgWlifCmd.cmd, len);
		cmd[len] = 0;
		gptMsgWlifCmd.ready = 0;
		return len;
	}
	return 0;
}
#endif

int wlif_put_event(wlif_priv_t *priv)
{
	if(gptMsgWlifEvt.num < WLIF_NUM_EVENT)
	{
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].iftype = priv->iftype;
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].type   = priv->type;
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].event  = priv->event;
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[0] = priv->mac_addr[0];
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[1] = priv->mac_addr[1];
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[2] = priv->mac_addr[2];
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[3] = priv->mac_addr[3];
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[4] = priv->mac_addr[4];
		gptMsgWlifEvt.event[gptMsgWlifEvt.head].mac_addr[5] = priv->mac_addr[5];
		gptMsgWlifEvt.lock[gptMsgWlifEvt.head] = 1;
		gptMsgWlifEvt.num++;
		num_loop(gptMsgWlifEvt.head, WLIF_NUM_EVENT);
		return 1;
	}
	return 0;
}

#ifdef DEF_CPU1
void wlif_init_event(void)
{
	memset((void *)&gptMsgWlifEvt, 0, sizeof(msgq_wlif_evt_t));
}

int wlif_get_event(wlif_priv_t *priv)
{
	if(gptMsgWlifEvt.cnt)
	{
		priv->iftype = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].iftype;
		priv->type  = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].type;
		priv->event = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].event;
		priv->mac_addr[0] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[0];
		priv->mac_addr[1] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[1];
		priv->mac_addr[2] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[2];
		priv->mac_addr[3] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[3];
		priv->mac_addr[4] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[4];
		priv->mac_addr[5] = gptMsgWlifEvt.event[gptMsgWlifEvt.tail].mac_addr[5];
		gptMsgWlifEvt.lock[gptMsgWlifEvt.tail] = 0;
		gptMsgWlifEvt.cnt--;
		num_loop(gptMsgWlifEvt.tail, WLIF_NUM_EVENT);
		return 1;
	}
	return 0;
}
#endif

// ip1 -> ip0 or ip0 -> ip0
void xfer_event_to_cpu0(WORD type, WORD evt, BYTE *mac, NETIFLIST iftype)
{
    wlif_priv_t priv;
	priv.iftype = iftype;
	priv.event  = evt;
	priv.type   = type;
	if(mac)
		memcpy(priv.mac_addr, mac, 6);
	wlif_put_event(&priv);

	CPU_SHARED1 |= eIP1_WIF_EVT;
	if(CPU_IRQ1 == 0)
		CPU_IRQ1 = 1;
#ifdef DEF_CPU0
	while(CPU_IRQ1); //we are waiting until cpu1 clear sys_irq0.
#else
	while(CPU_IRQ1)
		vTaskDelay(1);
#endif
}
#endif

#ifdef DEF_CPU0	// copy to cpu0 (eth, wifi packet data)
#include "lwip/pbuf.h"
#include "lwip/netif.h"
#include "enx_lwip_helper.h"

// cpu1 -> cpu0 (wifi packet data send)
#ifdef __WIFI__
void xfer_pkt_from_cpu0(void)
{
	UINT nLen, type, iftype;
	BYTE *pkt;
	struct pbuf *q, *p;
	struct netif *netif;

	if(CS_WIFI_TX)	// 이미 1이면 사용중이다.
	{				// 이 레지스터를 읽는 순간 현상태를 반환하고 스스로 값이 1이 된다.
		return;
	}

	if(gptMsgEthCpu1to0.num)
	{
        type   = gptMsgEthCpu1to0.type[gptMsgEthCpu1to0.tail];
        nLen   = gptMsgEthCpu1to0.length[gptMsgEthCpu1to0.tail];
        pkt    = (BYTE *)gptMsgEthCpu1to0.pkt[gptMsgEthCpu1to0.tail];
        iftype = gptMsgEthCpu1to0.netif[gptMsgEthCpu1to0.tail];
		netif  = listNetif[iftype];

		if(type == WLIF_DATA)
		{
    		if((q = pbuf_alloc(PBUF_RAW, nLen+ETH_PAD_SIZE, PBUF_RAM)))
    		{
    			pbuf_header(q, -ETH_PAD_SIZE);
                UINT nTotLen = 0;
				for(p=q; p!=NULL; p=p->next){
					DmaMemCpy_isr((BYTE *)p->payload, pkt + nTotLen, (UINT)p->len);	// Copy to "pbuf"
					invalidate_dcache_range((uint)p->payload, (uint)p->payload + p->len);
					nTotLen += p->len;
				}
    			pbuf_header(q, ETH_PAD_SIZE);
				if(netif)
                {
                	netif->linkoutput(netif, q);
                }
                if(q)
                {
                    if(q->ref > 0)
    		            pbuf_free(q);
                    else
                        printf("%s(%d) : q->ref(%d)\r\n", __func__,__LINE__, q->ref);
                }
                else
               	{
                    printf("%s(%d) : pbuf free err\r\n", __func__,__LINE__);
               	}
    		}
			else
			{
				printf("%s(%d) : error? type(%d)\r\n", __func__, __LINE__, type);
			}
	    }
		else
		{
			printf("%s(%d) : error? type(%d) fail\r\n", __func__, __LINE__, type);
		}

//		printf("IP1-OUT : head(%02d) tail(%02d) num(%02d) len(%d)\r\n", gptMsgEthCpu1to0.head, gptMsgEthCpu1to0.tail, gptMsgEthCpu1to0.num, nLen);

		num_loop(gptMsgEthCpu1to0.tail, MSG_ETH_NUM);

		if((gptMsgEthCpu1to0.num) > 0)
		{
			gptMsgEthCpu1to0.num--;																								// Unlock
//			printf("IP1-OUT : head(%02d) tail(%02d) num(%02d)\r\n", gptMsgEthCpu1to0.head, gptMsgEthCpu1to0.tail, gptMsgEthCpu1to0.num);
		}
		else
		{
			printf("IP1-OUTE: head(%02d) tail(%02d) num(%02d)\r\n", gptMsgEthCpu1to0.head, gptMsgEthCpu1to0.tail, gptMsgEthCpu1to0.num);
		}
	}

	CS_WIFI_TX = 0;
}
#endif
#endif

#if (RTSPoverHTTP==1)
BYTE MsgTunnelPut(volatile msgq_tunnel_t *p, void *anArg)
{
	UINT bRes = DEF_FAIL;

	if(MsgLock() == DEF_OK)
	{
		if(cQueue_isfull(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemPut, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			DmaMemCpy_isr((BYTE*)&(p->arg[p->tail]), (BYTE*)anArg, sizeof(tunnel_info));

			num_loop(p->tail, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemPut);
			}
#else
		CRITICAL_END;
#endif
		}
		MsgUnlock();
	}

	return bRes;
}

BYTE MsgTunnelGet(volatile msgq_tunnel_t *p, void *anArg)
{
	UINT bRes = DEF_FAIL;

	if(MsgLock() == DEF_OK)
	{
		if(cQueue_isempty(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemGet, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			DmaMemCpy_isr((BYTE*)anArg, (BYTE*)&(p->arg[p->head]), sizeof(tunnel_info));
			invalidate_dcache_range((uint)anArg, (uint)anArg + sizeof(tunnel_info));
			//DmaMemSet_isr((BYTE*)&(p->arg[p->head]), 0, sizeof(tunnel_info));

			num_loop(p->head, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemGet);
			}
#else
			CRITICAL_END;
#endif
		}
		MsgUnlock();
	}

	return bRes;
}
#endif


BYTE MsgTxtOsdPut(volatile msgq_txtosd_t *p, void *anArg)
{
	UINT bRes = DEF_FAIL;

	
		if(cQueue_isfull(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemPut, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			DmaMemCpy_isr((BYTE*)&(p->arg[p->tail]), (BYTE*)anArg, sizeof(txtosd_info));

			num_loop(p->tail, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemPut);
			}
#else
		CRITICAL_END;
#endif
		}
		

	return bRes;
}

BYTE MsgTxtOsdGet(volatile msgq_txtosd_t *p, void *anArg)
{
	UINT bRes = DEF_FAIL;

	
		if(cQueue_isempty(p) != DEF_OK)
		{
#ifdef DEF_CPU1
			if(xSemaphoreTake(xSemGet, portMAX_DELAY))
			{
#else
			CRITICAL_BEGIN;
#endif

			DmaMemCpy_isr((BYTE*)anArg, (BYTE*)&(p->arg[p->head]), sizeof(txtosd_info));
			invalidate_dcache_range((uint)anArg, (uint)anArg + sizeof(txtosd_info));
			//DmaMemSet_isr((BYTE*)&(p->arg[p->head]), 0, sizeof(tunnel_info));

			num_loop(p->head, p->tot_num);

			bRes = DEF_OK;

#ifdef DEF_CPU1
			xSemaphoreGive(xSemGet);
			}
#else
			CRITICAL_END;
#endif
		}
	

	return bRes;
}



#endif	// #ifndef DEF_BOOT
