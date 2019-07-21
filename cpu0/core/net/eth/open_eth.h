#ifndef __OPEN__ETH__
#define __OPEN__ETH__

#include "arch/cc.h"

#ifndef DEF_OETH_TX_IRQ
#define DEF_OETH_TX_IRQ 0
#endif

//******************************************************************************
// Misc
//------------------------------------------------------------------------------

/* Buffer number (must be 2^n)  */
#define OETH_RXBUF_NUM 32
#define OETH_RXBD_NUM_MASK  (OETH_RXBUF_NUM-1)

#define ETH_ZLEN  60    /* Min. octets in frame sans FCS */
#define MAX_FRAME_SIZE    1520

typedef struct {
    u8 dst[6];
    u8 src[6];
    u16 type;
}__attribute__ ((packed)) eth_hdr_t;

#if 0
struct udp_hdr
{
	u16 src;
	u16 dest;
	u16 len;
	u16 chksum;
}__attribute__ ((packed));
#endif

/*macro */
#define eth_bp(nBase)  (nBase)
#define eth_hdr(nBase) ((eth_hdr_t *)eth_bp(nBase))
#define ip_hdr(nBase)  ((struct ip_hdr *)((u8_t *)eth_bp(nBase)+sizeof(eth_hdr_t)))
#define udp_hdr(nBase) ((struct udp_hdr *)((u8 *)ip_hdr(nBase) + 20))
#define tcp_hdr(nBase) ((struct tcp_hdr *)((u8_t *)ip_hdr(nBase) + IPH_HL(ip_hdr(nBase)) * 4))

/*
********************************************************************************
*                                               EXTERNS
********************************************************************************
*/

extern void isr_oeth_rx(void *ctx);
extern void isr_oeth_rx_loopback(void *ctx);

#if (NET_TX_BUFFER==1)
extern void enx_eth_netif_tx(void);
#endif

/*
********************************************************************************
*                                             Macros
********************************************************************************
*/

#endif
