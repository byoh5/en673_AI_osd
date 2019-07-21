#ifdef __WIFI__

#include "dev.h"
#include "dev_inline.h"

#include "msg.h"

#include "lwip/opt.h"
#include "lwip/def.h"
#include "lwip/mem.h"
#include "lwip/pbuf.h"
#include "lwip/stats.h"
#include "lwip/sys.h"
#include "netif/etharp.h"
#include "lwip/igmp.h"
#include "lwip/tcp_impl.h"
#include "lwip/udp.h"

#include "ewl.h"
#include "enx_lwip_helper.h"

#if (RTSPoverHTTP==1)
#include "http_tunnel.h"
#endif

//#define NO_PENDING_Q

#define IFNAME0 'w'
#define IFNAME1 'l'

#ifndef ETH_P_EAPOL
#define ETH_P_EAPOL 0x888e
#endif

#ifndef NO_PENDING_Q
#define NUM_TX_QUEUE 200
typedef struct {
    struct pbuf *buf[NUM_TX_QUEUE];
    u8 first;
    u8 last;
    u8 size;
} pbufq_t;

#define PQUEUE_EMPTY(q) (q.size == 0)
#define PQUEUE_FULL(q) (q.size == NUM_TX_QUEUE)
#define PQUEUE_FIRST(q) (q.buf[q.first])
#define PQUEUE_DEQUEUE(q)                                   \
        ({                                                  \
                if(q.size == 1)                             \
                {                                           \
                                                            \
                    q.first = -1;                           \
                    q.last  = -1;                           \
                }                                           \
                else                                        \
                {                                           \
                    q.first = (q.first + 1) % NUM_TX_QUEUE;\
                }                                           \
                q.size-- ;                                  \
        })
        
#define PQUEUE_ENQUEUE(q, p)                                \
        ({                                                  \
                if(q.size == 0)                             \
                {                                           \
                    q.first = 0;                            \
                    q.last  = 0;                            \
                }                                           \
                else                                        \
                    q.last = (q.last + 1) % NUM_TX_QUEUE;   \
                q.buf[q.last] = p;                          \
                q.size++;                                   \
        }) 
/* Modes of operation */

/* tx pending queue*/
static pbufq_t tx_pending;
#endif

/**
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @return a pbuf filled with the received packet (including MAC header)
 *         NULL on memory error
 */
#include "lwip/tcp_impl.h"
//#define ip_hdr(p)  ((struct ip_hdr *)((u8_t *)p+sizeof(struct eth_hdr)))
//#define tcp_hdr(p) ((struct tcp_hdr *)((u8_t *)ip_hdr(p) + IPH_HL(ip_hdr(p)) * 4))                    
struct pbuf *low_level_wlif_input(struct netif *netif, NETIFLIST iftype)
{
	struct tcp_pcb ** const sh_tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
													&tcp_active_pcbs, &tcp_tw_pcbs};

	struct pbuf *q;
	struct ip_hdr *iphdr;
	struct tcp_hdr *tcphdr;
	struct udp_hdr *udphdr;
	struct eth_hdr *ethhdr;

	void *p;
	volatile UINT nFlag = 0;
	int i;
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;
	volatile pkt_info *pkinfo;

	if ((q = ewl_pkt_rx(netif))) {
#if ETH_PAD_SIZE
		pbuf_header(q, ETH_PAD_SIZE);
#endif
		/* identify the IP header */
		ethhdr = (struct eth_hdr *)q->payload;
		p = (u8_t *)q->payload + (s16_t)sizeof(struct eth_hdr);
		iphdr = (struct ip_hdr *)p;
		tcphdr = (struct tcp_hdr *)((u8_t *)p + IPH_HL(iphdr) * 4);
		udphdr = (struct udp_hdr *)((u8_t *)p + 20);

		nFlag = NETRX_TYPE_CPU1;
		if (ethhdr->type == ETHTYPE_ARP) {
			nFlag = NETRX_TYPE_SHARE;
		} else if (ethhdr->type == ETHTYPE_IP && iphdr->dest.addr == netif->ip_addr.addr) {
			switch (iphdr->_proto) {
				case IP_PROTO_TCP:		// send cpu1 or cpu0(RTSP(554), RTMP(.), RTSPoverHTTP(80)?, iperf(5001), AUDIORX(4999))
					if (tcphdr->dest != HTTP_PORT) {
						for (i = 0; i < 4; i++) {
							for(tpcb = *sh_tcp_pcb_lists[i]; tpcb != NULL; tpcb = tpcb->next) {
								if (tcphdr->dest == tpcb->local_port)  {
									nFlag = NETRX_TYPE_CPU0;
									break;
								}
							}
						}
					} else {	// 이 경우는 80인데, RTSPoverHTTP인지 그냥 HTTP인지 판단해야 한다.
#if (RTSPoverHTTP==1)
						if (tunnel_check(tcphdr->src, tcphdr->dest) == 1) {
							nFlag = NETRX_TYPE_CPU0;
						}
#endif
					}
					break;
				case IP_PROTO_UDP:		// send cpu1 or cpu0(AUDIORX(5000), RTCP(.))
//					if ((iphdr->dest.addr >> 28) == 0xE) {
//						printf("Mcast Input!(0x%08X)\r\n", iphdr->dest.addr);
//					}
					for(upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) { // UDP RTCP get
						if (udphdr->dest == upcb->local_port) {
							nFlag = NETRX_TYPE_CPU0;
							break;
						}
					}
					break;
				case IP_PROTO_ICMP: 	// send cpu1
//					printf("\r\nICMP-RX %IP<-%IP Size:%d\r\n", iphdr->dest.addr, iphdr->src.addr, q->tot_len);
					break;
				case IP_PROTO_IGMP: 	// send cpu1
//					printf("\r\nIGMP-RX %IP<-%IP Size:%d\r\n", iphdr->dest.addr, iphdr->src.addr, q->tot_len);
					break;
				case IP_PROTO_UDPLITE:	// send cpu1
					break;
				default:				// send cpu1
					break;
			}
		}

		if (nFlag & NETRX_TYPE_CPU1) {
#if ETH_PAD_SIZE
			pbuf_header(q, -ETH_PAD_SIZE);
#endif
			CRITICAL_BEGIN;
			if (cNRQueue_isfull(&gptMsgNetworkRX1) == DEF_OK) {
//				printf("FUL1 Drop H(%03d) T(%03d)\r\n", gptMsgNetworkRX1.head, gptMsgNetworkRX1.tail);
			} else {
				if ((gptMsgNetworkRX1.head_pos + 1520) >= gptMsgNetworkRX1.end) {	// 현 위치와 len의 값이 end보다 크다면 처음으로 되돌아간다.
					printf("%04d:%s : Overflow!\r\n", __LINE__, __func__);			// 그러나 tail의 값에 head_pos가 조절되야 하므로, 
					gptMsgNetworkRX1.head_pos = gptMsgNetworkRX1.base;				// 여기에 진입한다면 board.h, msg.h의 메모리조정을 다시 해야 한다.
				}
				struct pbuf *r;
				UINT nTotLen = 0;
				for (r = q; r != NULL; r = r->next) {	// Chained "pbuf"
					DmaMemCpy_isr((BYTE *)(gptMsgNetworkRX1.head_pos+nTotLen), (BYTE *)(r->payload), (UINT)r->len);
					nTotLen += r->len;
				}
				pkinfo = &(gptMsgNetworkRX1.info[gptMsgNetworkRX1.tail]);
				pkinfo->flag = nFlag;
				pkinfo->type = WLIF_DATA;
				pkinfo->lenth = q->tot_len;
				pkinfo->data = gptMsgNetworkRX1.head_pos;
				pkinfo->iftype = iftype;
				num_loop(gptMsgNetworkRX1.tail, MSG_NETRX_NUM);	// next head
				if (gptMsgNetworkRX1.tail == 0) {
					gptMsgNetworkRX1.head_pos = gptMsgNetworkRX1.base;
				} else {
					gptMsgNetworkRX1.head_pos += 1520;			// next head_pos
				}
			}
#ifndef NET_RX_POLLING
			CPU_SHARED1 |= eIP1_WIF_RX;
			CPU_IRQ1 = 1;
#endif
			CRITICAL_END;
			if (nFlag == NETRX_TYPE_CPU1) {		 // cpu1 only 인 경우에만 pbuf_free 한다.
				pbuf_free(q);
			}
#if ETH_PAD_SIZE
			else {
				pbuf_header(q, ETH_PAD_SIZE);	// cpu0도 사용해야 하면 pbuf header을 원상복귀한다.
			}
#endif
		}

		if (nFlag & NETRX_TYPE_CPU0) {			// else if를 사용하지 않는다. NETRX_TYPE_SHARE를 위해
			if (gptMsgDebug.dltFLAG & eDltCPU0UAPPROC) {
				printf("c");
			}
			return q;
		}
	}
	return NULL;
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
static err_t
low_level_wlif_output(struct netif *netif, struct pbuf *p)
{  
    int ret=0;
#ifdef NO_PENDING_Q    
	struct pbuf *q; 
	
	if(!PQUEUE_FULL(tx_pending))
	{
		PQUEUE_ENQUEUE(tx_pending, p);
		pbuf_ref(p);
	}
	else
	{
		u32 pending=0;
		ret = -1;
		ewl_is_tx_pending(netif,&pending);
	}

	while(!PQUEUE_EMPTY(tx_pending))
	{
		if(!ewl_is_tx_pending(netif,NULL))
		{
			q = PQUEUE_FIRST(tx_pending);
			PQUEUE_DEQUEUE(tx_pending);
			//printf("%s, q->ref %d \r\n",__func__,q->ref);
			q->ref--;
			if(ewl_start_tx(q,netif) < 0)
				ret = 0;
			else
				ret = -1;
		}
		else
			break;
	}
#else
//	printf("PACKET OUTPUT!!(size:%dbyte)\r\n", p->tot_len);
    ret = ewl_start_tx(p,netif);    
#endif
    if(!ret)
        return ERR_OK; /* no one seems to check this anyway */
    else
        return ERR_IF;
}

/**
 * This function will be called by wlif_poll() when a packet has been received
 * from the mac. Then the type of the received packet is determined and
 * the appropriate input function is called.
 *
 * @param netif the lwip network interface structure for this ethernetif
 */
void
mlanif_input(NETIFLIST iftype)
{
	struct netif *netif = listNetif[iftype];
	struct eth_hdr *ethhdr;
	struct pbuf *p;

	while((p = low_level_wlif_input(netif, iftype)))
	{
		/* points to packet payload, which starts with an Ethernet header */
		ethhdr = (struct eth_hdr *)p->payload;
		switch (htons(ethhdr->type)) 
		{
			/* IP or ARP packet? */
			case ETHTYPE_ARP:
			case ETHTYPE_IP:
				/* skip Ethernet header */
				//pbuf_header(p, -(s16_t)sizeof(struct eth_hdr));
				/* full packet send to tcpip_thread to process */
				if (netif->input(p, netif) != ERR_OK)
				{
					LWIP_DEBUGF(NETIF_DEBUG, ("mlanif_input: IP input error\r\n"));
					pbuf_free(p);
					p = NULL;
				}
				break;

			default:
				LWIP_DEBUGF(NETIF_DEBUG, ("mlanif_input: inval type\r\n"));
				pbuf_free(p);
				p = NULL;
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
static err_t
low_level_init(struct netif *netif, char name0, char name1)
{
	netif->name[0] = name0;
	netif->name[1] = name1;
	netif->hwaddr_len = ETHARP_HWADDR_LEN; // Set MAC hardware address length
	netif->mtu = 1500; // Maximum transfer unit
	netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP; // Device capabilities

#ifndef NO_PENDING_Q    
	tx_pending.first = -1;
	tx_pending.last = -1;
	tx_pending.size  = 0;
#endif

	return ERR_OK;
}

#if LWIP_IGMP

/**
 @ This function could be called to add or delete 
   a entry in the multicast filter table of the ethernet MAC.
*/
static err_t
mlanif_igmp_mac_filter( struct netif *netif, struct ip_addr *group, u8_t action)
{
	u8_t multicast_mac[6];
	multicast_mac[0] = 0x01;
	multicast_mac[1] = 0x00;
	multicast_mac[2] = 0x5E;
	multicast_mac[3] = ip4_addr2(group) & 0x7F;
	multicast_mac[4] = ip4_addr3(group);
	multicast_mac[5] = ip4_addr4(group);

	if (action == IGMP_ADD_MAC_FILTER)
	{
		printf("%s(%d) : WIFI IGMP ADD(0x%08X)\r\n", __func__, __LINE__, group->addr);
		ewl_set_multicast_addr(netif, multicast_mac);
		return ERR_OK;
	}
	else if (action == IGMP_DEL_MAC_FILTER)
	{
		// multicast delete func?
		printf("%s(%d) : Not define - multicast delete function\r\n", __func__, __LINE__);
		return ERR_IF;
	}

	return ERR_IF;
}

#endif
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
err_t
mlanif_init(struct netif *netif)
{
	LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_ARP
	netif->output = etharp_output;
#endif
	netif->linkoutput = low_level_wlif_output;
#if LWIP_IGMP
	netif->igmp_mac_filter = mlanif_igmp_mac_filter;
#endif

	return low_level_init(netif, IFNAME0, IFNAME1);
}
#endif
