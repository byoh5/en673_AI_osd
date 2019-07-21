/**
 *Copyright 2013 eyenix CO., LTD.
 *ALL RIGHTS RESERVED.
 *This software is provided under license and contains proprietary
 *and confidential material which is the property of Eyenix.
 *
 *FileName     :
 *Description  :
 *Reversion    :
 *Comment      : first implementation
*/

#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "open_eth.h"
#include "msg.h"
#include "lwip/def.h"
#include "lwip/tcp_impl.h"
#include "lwip/netif.h"
#include "netif/etharp.h"
#include "enx_lwip_helper.h"
#include <malloc.h>

#ifdef __ETH__

#if (RTSPoverHTTP==1)
#include "http_tunnel.h"
#endif

#if (ETH_EXCESSIVE_PACKETS_CK==1)
static UINT eth_rx_en_check = INVALID_TIMER_ID;
void enx_eth_rx_en_check(void *ctx)
{
	if (ETH_RX_EN == 0) {
		printf("CPU0 ETH RX unlock\r\n");
		ETH_RX_EN = 1;
	}
	eth_rx_en_check = INVALID_TIMER_ID;
	UNUSED(ctx);
}
#endif

/**
 *  isr_oeth_rx()
 * Description : (1) Process Receive Interrupts
 * Argument(s) : none.
 * Note(s)     :
 * 1) First, handle rtsp packet that have port number (554, typically)
   2) Other packet enquenu msg queue,
     then Main processor will handle this msg queeu.
*/
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"	// struct udp_pcb *udp_pcbs;
void isr_oeth_rx(void *ctx)
{
	struct tcp_pcb ** const sh_tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
													&tcp_active_pcbs, &tcp_tw_pcbs};
	volatile UINT nBase;
	volatile UINT nLen, nFlag;
	int i, j;
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;
	volatile pkt_info *pkinfo;

	gptMsgShare.ETH_ON = 1;

	do {
		nFlag = 0;
		for (j = 0; j < 5; j++) {
			nLen = (UINT)ETH_RX_LEN;
			if (nLen != 0)
				break;
			NOP();
		}
		if (nLen <= 1518 && nLen >= ETH_ZLEN) {
			nBase = (UINT)gptMsgNetworkRXShared.buffer;

			DmaMemCpy_isr((BYTE*)nBase, (BYTE *)ETHRX_BASE, nLen);					// ETHRX to SH_SRAM
//			asm volatile("l.nop");
//			invalidate_dcache_range((uint)(nBase), (uint)(nBase + 64));

			nFlag = NETRX_TYPE_CPU1;
#if ENX_NETWORK_Debug
			gptMsgDebug.ETH_packet_rx += nLen;
#endif
#if (ETH_MAC_PAUSE)
			if (((UINT*)nBase)[0] == 0x0180C200 && ((UINT*)nBase)[3] == 0x88080001) { // 01:80:C2:00:--:--, Legth/Type(0x8808), OpCode(0x0001)
				nFlag = NETRX_TYPE_NONE;
//				hexDump("IEEE Std 802.1D", gptMsgNetworkRXShared.buffer, nLen);
				UINT nPCount = ((WORD*)gptMsgNetworkRXShared.buffer)[8];
#if (ETH_MAC_PAUSE==1)
				if (nPCount == 0) {
					gptMsgNetworkRXShared.tx_go = 1;
				} else {
					gptMsgNetworkRXShared.macp_count++;
					gptMsgNetworkRXShared.tx_go = 0;
					gptMsgNetworkRXShared.tx_pause = nPCount;
				}
#elif (ETH_MAC_PAUSE==2)
				if (nPCount == 0) {
					ETH_TX_IFGGAP = ETH_MAC_TX_IFGGAP;
				} else {
					ETH_TX_IFGGAP = nPCount;
				}
#endif
			} else
#endif
			if (eth_hdr(nBase)->type == ETHTYPE_ARP) {	// send cpu0, cpu1
				nFlag = NETRX_TYPE_SHARE;
			} else if (eth_hdr(nBase)->type == ETHTYPE_IP && ip_hdr(nBase)->dest.addr == listNetif[enlETHERNET]->ip_addr.addr) {
				switch (ip_hdr(nBase)->_proto) {
					case IP_PROTO_TCP:		// send cpu1 or cpu0(RTSP(554), RTMP(.), RTSPoverHTTP(80)?, iperf(5001), AUDIORX(4999))
						if(tcp_hdr(nBase)->dest != HTTP_PORT)
						{
							for (i = 0; i < 4; i++) {
								for(tpcb = *sh_tcp_pcb_lists[i]; tpcb != NULL; tpcb = tpcb->next) {
									if(tcp_hdr(nBase)->dest == tpcb->local_port)
									{
										nFlag = NETRX_TYPE_CPU0;
										break;
									}
								}
							}
						} else {	// 이 경우는 80인데, RTSPoverHTTP인지 그냥 HTTP인지 판단해야 한다.
#if (RTSPoverHTTP==1)
							if (tunnel_check(tcp_hdr(nBase)->src, tcp_hdr(nBase)->dest) == 1) {
								nFlag = NETRX_TYPE_CPU0;
							}
#endif
						}
						break;
					case IP_PROTO_UDP:		// send cpu1 or cpu0(AUDIORX(5000), RTCP(.))
						{
//							if ((ip_hdr(nBase)->dest.addr >> 28) == 0xE) {
//								printf("Mcast Input!(0x%08X)\r\n", ip_hdr(nBase)->dest.addr);
//							}
							for(upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) { // UDP RTCP get
								if(udp_hdr(nBase)->dest == upcb->local_port)
								{
									nFlag = NETRX_TYPE_CPU0;
									break;
								}
							}
						}
						break;
					case IP_PROTO_ICMP:		// send cpu1
//						printf("\r\nICMP-RX %IP<-%IP Size:%d\r\n", ip_hdr(nBase)->dest.addr, ip_hdr(nBase)->src.addr, nLen);
						break;
					case IP_PROTO_IGMP:		// send cpu1
//						printf("\r\nIGMP-RX %IP<-%IP Size:%d\r\n", ip_hdr(nBase)->dest.addr, ip_hdr(nBase)->src.addr, nLen);
						break;
					case IP_PROTO_UDPLITE:	// send cpu1
						break;
					default:				// send cpu1
						break;
				}
			}

			if (gptMsgDebug.dltFLAG & eDltETHRXSIZE) {
				printf("%d ", nLen);
			}

			if (nFlag & NETRX_TYPE_CPU1) {
				if (cNRQueue_isfull(&gptMsgNetworkRX1) == DEF_OK) {
#if (ETH_EXCESSIVE_PACKETS_CK==1)
					ETH_RX_EN = 0;
					if (eth_rx_en_check == INVALID_TIMER_ID) {
						eth_rx_en_check = timer_sched_timeout_cb(1000, TIMEOUT_ONESHOT, enx_eth_rx_en_check, NULL);
					}
#endif
//					printf("FUL1 Drop H(%03d) T(%03d)\r\n", gptMsgNetworkRX1.head, gptMsgNetworkRX1.tail);
				} else {
					if ((gptMsgNetworkRX1.head_pos + 1520) >= gptMsgNetworkRX1.end) {	// 현 위치와 len의 값이 end보다 크다면 처음으로 되돌아간다.
						printf("%04d:%s : Overflow!\r\n", __LINE__, __func__);
						gptMsgNetworkRX1.head_pos = gptMsgNetworkRX1.base;
					}
					DmaMemCpy_isr((BYTE*)gptMsgNetworkRX1.head_pos, (BYTE *)nBase, nLen);	// SH_SRAM to DDR
					pkinfo = &(gptMsgNetworkRX1.info[gptMsgNetworkRX1.tail]);
					pkinfo->flag = nFlag;
					pkinfo->type = ETH_DATA;
					pkinfo->lenth = nLen;
					pkinfo->data = gptMsgNetworkRX1.head_pos;
					pkinfo->iftype = enlETHERNET;
					num_loop(gptMsgNetworkRX1.tail, MSG_NETRX_NUM);	// next head
					if (gptMsgNetworkRX1.tail == 0) {
						gptMsgNetworkRX1.head_pos = gptMsgNetworkRX1.base;
					} else {
						gptMsgNetworkRX1.head_pos += 1520;			// next head_pos
					}
					if (gptMsgDebug.dltFLAG & eDltCPU1ETHRX_dp) {
						hexDump("RxPkt1", (BYTE *)pkinfo->data, nLen);
					}
				}
#ifndef NET_RX_POLLING
				CPU_SHARED1 |= eIP1_ETH_RX;
				CPU_IRQ1 = 1;
#endif
			}

			if (nFlag & NETRX_TYPE_CPU0) {
				if (cNRQueue_isfull(&gptMsgNetworkRX0) == DEF_OK) {
//					printf("FUL0 Drop H(%03d) T(%03d)\r\n", gptMsgNetworkRX0.head, gptMsgNetworkRX0.tail);
				} else {
					if ((gptMsgNetworkRX0.head_pos + 1520) >= gptMsgNetworkRX0.end) {	// 현 위치와 len의 값이 end보다 크다면 처음으로 되돌아간다.
						printf("%04d:%s : Overflow!\r\n", __LINE__, __func__);
						gptMsgNetworkRX0.head_pos = gptMsgNetworkRX0.base;
					}
					DmaMemCpy_isr((BYTE*)gptMsgNetworkRX0.head_pos, (BYTE *)nBase, nLen);	// SH_SRAM to DDR
					pkinfo = &(gptMsgNetworkRX0.info[gptMsgNetworkRX0.tail]);
					pkinfo->flag = nFlag;
					pkinfo->type = ETH_DATA;
					pkinfo->lenth = nLen;
					pkinfo->data = gptMsgNetworkRX0.head_pos;
					pkinfo->iftype = enlETHERNET;
					num_loop(gptMsgNetworkRX0.tail, MSG_NETRX_NUM);	// next head
					if (gptMsgNetworkRX0.tail == 0) {
						gptMsgNetworkRX0.head_pos = gptMsgNetworkRX0.base;
					} else {
						gptMsgNetworkRX0.head_pos += 1520;			// next head_pos
					}
					if (gptMsgDebug.dltFLAG & eDltCPU0ETHRX_dp) {
						hexDump("RxPkt0", (BYTE *)pkinfo->data, nLen);
					}
				}
			}
		} else {	// 60보다 작거나 1518보다 큰 경우 drop
			printf("Error : ETH RX Size %dByte, reCheck(Size:%dbyte, EMPTY:%d)\r\n", nLen, ETH_RX_LEN, ETH_RX_EMPTY);
			break;
		}
	} while(!ETH_RX_EMPTY);

	ETH_RX_IRQCLR = 1;			// Clear IRQ

	return;

	UNUSED(ctx);
}

#if (NET_TX_BUFFER==1)
void enx_eth_netif_tx(void)
{
	if(cNRQueue0_isempty(&gptMsgEthTx) == DEF_OK)
	{
		return;
	}
//	printf("gptMsgEthTx.cpu0_head(%d) gptMsgEthTx.cpu0_tail(%d)\r\n", gptMsgEthTx.cpu0_head, gptMsgEthTx.cpu0_tail);

	UINT getTime = timer_get_ms();

	if(gptMsgEthTx.timestamp != getTime)
	{
		DmaEthTxCpy((BYTE *)gptMsgEthTx.info0[gptMsgEthTx.cpu0_head].data, gptMsgEthTx.info0[gptMsgEthTx.cpu0_head].lenth);	
		num_loop(gptMsgEthTx.cpu0_head, MSG_NETTX_NUM);

		if(cNRQueue0_isempty(&gptMsgEthTx) == DEF_FAIL)
		{
			DmaEthTxCpy((BYTE *)gptMsgEthTx.info0[gptMsgEthTx.cpu0_head].data, gptMsgEthTx.info0[gptMsgEthTx.cpu0_head].lenth); 
			num_loop(gptMsgEthTx.cpu0_head, MSG_NETTX_NUM);
		}

		gptMsgEthTx.timestamp = getTime;
	}
}
#endif

#if (PHY_LOOPBACK_TEST==1)
void isr_oeth_rx_loopback(void *ctx)
{
	volatile UINT nLen, i;
	do {
		nLen = (UINT)ETH_RX_LEN;
		if (nLen <= 1518 && nLen >= ETH_ZLEN) { // particular lense 
			DmaMemCpy_isr((BYTE*)gptMsgNetworkRXShared.buffer, (BYTE *)ETHRX_BASE, nLen);	// ETHRX to SH_SRAM
			for (i = 0; i < nLen; i++) {
				if (gptMsgNetworkRXShared.buffer[i] != gptMsgSH01[i]) {
					gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_data;
					break;
				}
			}
			if (gptMsgDebug.ETH_LOOPBACK_CHECK == ePlb_idle) {
				gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_ok;
			}
		} else {
			if (nLen == 0) { gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_zero; }
			else { gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_outside; }
			break;
		}
	}while(!ETH_RX_EMPTY);
	ETH_RX_IRQCLR = 1;	// Clear IRQ
	return;
	UNUSED(ctx);
}
#endif
#endif
