#ifndef __RTP_G711__
#define __RTP_G711__

#include "rtp_hdr_gen.h"

void rtp_audio_switch(int onoff);

// send G.711 udp packet
char rtp_tx_g711(void *ctx);

// send G.711 tcp packet
char rtp_tx_g711_over_tcp(void *ctx);

// G.711 decoding
void rtp_rx_g711_backchannel(rthRTPHeader *RTPHdr, int left);

// recv G.711 udp packet
void rtp_rx_g711_over_udp_recv(void *arg, struct udp_pcb *pcb, struct pbuf *p, ip_addr_t *addr, u16_t port);

// initialize G.711 receive
int enx_rx_g711_init(void);

#endif
