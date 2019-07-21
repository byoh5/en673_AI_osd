/*
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "dev.h"
#include "dev_inline.h"

#include <lwip/opt.h>
#include <lwip/def.h>
#include <lwip/mem.h>
#include <lwip/pbuf.h>
#include <lwip/stats.h>
#include <lwip/sys.h>
#include <netif/etharp.h>
#include <lwip/igmp.h>
#include <lwip/tcp_impl.h>

#define IFNAME0 'e'
#define IFNAME1 'n'

#include "open_eth.h"
#include "msg.h"

#ifdef __ETH__
/**
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
ISRT static struct pbuf *low_level_input(struct netif *netif)
{
	struct pbuf *p = NULL, *q = NULL;
	BYTE *pkt;
	WORD wLen, nTotLen = 0;
	volatile pkt_info *pkinfo;

	while (1) {
		pkinfo = &(gptMsgNetworkRX0.info[gptMsgNetworkRX0.head]);
	
		// quene가 비었는가?
		if (cNRQueue_isempty(&gptMsgNetworkRX0) == DEF_OK) {
			return NULL;
		}

//		printf("inp H(%04d) T(%04d) FLAG(%d) LEN(%d)\r\n", gptMsgEthCpu0to1.head, gptMsgEthCpu0to1.cpu1_tail, gptMsgEthCpu0to1.info[gptMsgEthCpu0to1.cpu1_tail].flag, gptMsgEthCpu0to1.info[gptMsgEthCpu0to1.cpu1_tail].lenth);
		// 0x1과 0x3인 데이터만 get
		if(pkinfo->flag & 0x1) {
			break;
		}

		// 여기로 올 이유가 없다...!
		//printf("[IP0] inp H(%04d) T(%04d) FLAG(%d) LEN(%d)\r\n", gptMsgEthCpu0to1.cpu1_head, gptMsgEthCpu0to1.cpu1_tail, gptMsgEthCpu0to1.info1[gptMsgEthCpu0to1.cpu1_head].flag, gptMsgEthCpu0to1.info1[gptMsgEthCpu0to1.cpu1_head].lenth);

		// 다음으로 넘어간다.
		num_loop(gptMsgNetworkRX0.head, MSG_NETRX_NUM);
	}

	if (cNRQueue_isempty(&gptMsgNetworkRX0) == DEF_FAIL) {
		CRITICAL_BEGIN;

		pkinfo = &(gptMsgNetworkRX0.info[gptMsgNetworkRX0.head]);

		wLen = pkinfo->lenth;
		pkt = (BYTE *)pkinfo->data;

		if ((gptMsgDebug.dltFLAG & eDltCPU0ETHPROC) && (pkinfo->type == ETH_DATA)) {
			printf("a");
		}

		p = pbuf_alloc(PBUF_RAW, wLen+ETH_PAD_SIZE, PBUF_POOL);
		if (p == NULL) {
			printf("pbuf_alloc(ID:%d) fail H(%04d) T(%04d) Size(%04d)\r\n", CPU_ID, gptMsgNetworkRX0.head, gptMsgNetworkRX0.tail, wLen);
		} else {
#if ETH_PAD_SIZE
			pbuf_header(p, -ETH_PAD_SIZE);								// Drop the padding word
#endif
			for(q=p; q!=NULL; q=q->next)
			{
				if(q->len)
				{	// Copy to "pbuf"
					DmaMemCpy_isr(q->payload, pkt + nTotLen, (UINT)q->len);
					invalidate_dcache_range((uint)(q->payload), (uint)(q->payload + q->len));
				}
				nTotLen += q->len;
			}
#if ETH_PAD_SIZE
			pbuf_header(p, ETH_PAD_SIZE);								// Reclaim the padding word
#endif
		}
		num_loop(gptMsgNetworkRX0.head, MSG_NETRX_NUM);

		CRITICAL_END;
	}
	else
	{
		printf("rx0 empty H(%04d) T(%04d)\r\n", gptMsgNetworkRX0.head, gptMsgNetworkRX0.tail);
	}

	return p;

	UNUSED(netif);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and
 *        type)
 * @return ERR_OK if the packet could be sent
 *         an err_t value if the packet couldn't be sent
 *
 * @note Returning ERR_MEM here if a DMA queue of your MAC is full can lead to
 *       strange results. You might consider waiting for space in the DMA queue
 *       to become availale since the stack doesn't retry to send a packet
 *       dropped because of memory failure (except for the TCP timers).
 */
#if (NET_TX_BUFFER==1)
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE);			// Drop the padding word
#endif

	if(p->next == NULL)
	{
		if(p->tot_len < 60)
		{
			p->tot_len = 60;
		}

		if(gptMsgEthTx.head_pos + p->tot_len >= gptMsgEthTx.end)
		{	// 현 위치와 len의 값이 end보다 크다면 처음으로 되돌아간다.
			gptMsgEthTx.head_pos = gptMsgEthTx.base;
		}

		DmaMemCpy_ip((BYTE*)gptMsgEthTx.head_pos, (BYTE *)p->payload, p->tot_len);

		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].type = ETH_DATA;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].lenth = p->tot_len;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].data = gptMsgEthTx.head_pos;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].iftype = enlETHERNET;
		num_loop(gptMsgEthTx.cpu0_tail, MSG_NETTX_NUM);	// next head

		gptMsgEthTx.head_pos += NETRX_ALIGN(p->tot_len);			// next head_pos
	}
	else
	{
		struct pbuf *q = NULL;
		UINT nTotLen = 0;

//		if(p->next)
//			printf("p(%d) now(%d) next(%d)\r\n", p->tot_len, p->len, p->next->len);

		for(q = p; q != NULL; q = q->next) 
		{
			if(nTotLen+(q->len)>1600){
				printf("Error Ether tx size over 1600bytes!\r\n");
				return ERR_MEM;
			}
			DmaMemCpy_ip((BYTE *)gptMsgEthTx.buffer+nTotLen, (BYTE *)q->payload, q->len);
			nTotLen += q->len;
		}

		if(nTotLen != p->tot_len)
			printf("%s: nTotLen(%d) != p->tot_len(%d)\r\n", __func__, nTotLen, p->tot_len);

		if(nTotLen < 60)
			nTotLen = 60;

		if(gptMsgEthTx.head_pos + nTotLen >= gptMsgEthTx.end)
		{	// 현 위치와 len의 값이 end보다 크다면 처음으로 되돌아간다.
			gptMsgEthTx.head_pos = gptMsgEthTx.base;
		}

		DmaMemCpy_ip((BYTE*)gptMsgEthTx.head_pos, (BYTE *)gptMsgEthTx.buffer, nTotLen);

		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].type = ETH_DATA;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].lenth = nTotLen;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].data = gptMsgEthTx.head_pos;
		gptMsgEthTx.info0[gptMsgEthTx.cpu0_tail].iftype = enlETHERNET;
		num_loop(gptMsgEthTx.cpu0_tail, MSG_NETTX_NUM);	// next head

		gptMsgEthTx.head_pos += NETRX_ALIGN(nTotLen);			// next head_pos
	}

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);		// Reclaim the padding word
#endif	
	return ERR_OK; /* no one seems to check this anyway */
	UNUSED(netif);
}
#else
ISRD BYTE etherbuf_Tx[1600];
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
#if ETH_PAD_SIZE
    pbuf_header(p, -ETH_PAD_SIZE);			// Drop the padding word
#endif	

	if(p->next == NULL)
	{
		if(p->tot_len < 64)
		{
			p->tot_len = 64;
		}

		DmaEthTxCpy((BYTE *)p->payload, p->tot_len);
	}
	else
	{
		struct pbuf *q = NULL;
		UINT nTotLen = 0;

//		if(p->next)
//			printf("p(%d) now(%d) next(%d)\r\n", p->tot_len, p->len, p->next->len);

		for(q = p; q != NULL; q = q->next) 
		{
			if(nTotLen+(q->len)>1600){
				printf("Error Ether tx size over 1600bytes!\r\n");
				return ERR_MEM;
			}
			DmaMemCpy_ip((BYTE *)etherbuf_Tx+nTotLen, (BYTE *)q->payload, q->len);
//			invalidate_dcache_range(etherbuf_Tx, etherbuf_Tx+(q->len));
			nTotLen += q->len;
		}

		if(nTotLen != p->tot_len)
			printf("%s: nTotLen(%d) != p->tot_len(%d)\r\n", __func__, nTotLen, p->tot_len);

		if(nTotLen < 64)
			nTotLen = 64;

		DmaEthTxCpy_async((BYTE *)etherbuf_Tx, nTotLen);
	}

#if ETH_PAD_SIZE
    pbuf_header(p, ETH_PAD_SIZE);		// Reclaim the padding word
#endif	
	return ERR_OK; /* no one seems to check this anyway */
	UNUSED(netif);
}
#endif
/**
 * This function will be called by wlif_poll() when a packet has been received
 * from the mac. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
ISRT void ethif_input(void *arg)
{
    struct netif *netif = arg;
    struct eth_hdr *ethhdr;
    struct pbuf *p;

    if ((p = low_level_input(netif))) {

    /* points to packet payload, which starts with an Ethernet
       header */
        ethhdr = (struct eth_hdr *)p->payload;
//        printf("%s %d %d\r\n",__func__,__LINE__, ethhdr->type);
        switch (htons(ethhdr->type)) 
        {
		
            /* IP or ARP packet? */
            case ETHTYPE_ARP: 
            case ETHTYPE_IP:
#if PPPOE_SUPPORT
            /* PPPoE packet? */
            case ETHTYPE_PPPOEDISC:
            case ETHTYPE_PPPOE:
#endif /* PPPOE_SUPPORT */
                /* full packet send to tcpip_thread to process */
                if (netif->input(p, netif) != ERR_OK) {
					LWIP_DEBUGF(NETIF_DEBUG, ("mlanif_input: IP input error\r\n"));
					pbuf_free(p);
					p = NULL;
                }
                break;
            default:
                LWIP_DEBUGF(NETIF_DEBUG, ("mlanif_input: inval type\r\n"));
                pbuf_free(p);
                break;
        }
    }
}

/**
 * In this function, the hardware should be initialized.
 * Called from wlif_init().
 *
 * @param netif the already initialized lwip network interface structure
 *        for this ethernetif
 */
static err_t low_level_init(struct netif *netif, char name0, char name1)
{
	netif->name[0] = name0;
	netif->name[1] = name1;
	netif->hwaddr_len = NETIF_MAX_HWADDR_LEN; // Set MAC hardware address length
	netif->mtu = 1500; // Maximum transfer unit
	netif->flags = NETIF_FLAG_UP | NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP; // Device capabilities

	return ERR_OK;
}

/**
 * Should be called at the beginning of the program to set up the
 * network interface. It calls the function low_level_init() to do the
 * actual setup of the hardware.
 *
 * This function should be passed as a parameter to netif_add().
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return ERR_OK if the loopif is initialized
 *         ERR_MEM if private data couldn't be allocated
 *         any other err_t on error
 */
err_t ethif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_ARP
    netif->output = etharp_output;
#endif
    netif->linkoutput = low_level_output;
#if LWIP_IGMP
 //   netif->igmp_mac_filter = mlanif_igmp_mac_filter;
#endif

    return low_level_init(netif, IFNAME0, IFNAME1);
}
#endif
