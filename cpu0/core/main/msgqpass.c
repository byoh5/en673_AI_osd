/**
 *Copyright 2016 eyenix CO., LTD.
 *ALL RIGHTS RESERVED.
 *This software is provided under license and contains proprietary
 *and confidential material which is the property of Eyenix.
 *
 *FileName     : msgqpass.c
 *Description  :
 *Reversion    :
 *Comment      : message pass
*/

#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h

#include "enx_os.h"

#include "msg.h"
#include "user.h"
#include "uxsr.h"

#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/udp.h"
#include "lwip/api.h"
#include "enx_lwip_helper.h"

#include "h264.h"
#include "jpeg.h"

#include "rtp_over_rtsp.h"
#include "enx_video.h"
#include "enx_audio.h"

#ifdef __AUDIO__
#include "rtp_audio.h"
#endif

#if (ENX_FTPD_use==1)
#include "ftp_data.h"
#endif
#ifdef __WIFI__
#include "inter_cpu.h"
#endif
#if (RTSPoverHTTP==1)
#include "http_tunnel.h"
#endif
#ifdef RTMP_SUPPORT
#include "rtmp_session.h"
#endif

#ifdef __H264__
#ifdef __H264_MCH__
#include "h264_mch.h"
#endif
#include "bitratectl.h"
#endif

#include "exr1k_sprs.h"

#include "msgqpass.h"

// TEST
#ifdef __ETH__
extern void isr_oeth_rx(void *ctx);
#if (PHY_LOOPBACK_TEST==1)
extern void isr_oeth_rx_loopback(void *ctx);
#endif
#endif
extern void enx_h264_on(void);
extern void jpeg_irq_handler(void *ctx);

#if defined(__ETH__) || defined(__WIFI__)
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/autoip.h"
extern struct netif *netif_list;
void cmd_ifconfig(void)
{
	printf("CPU%d ifconfig\r\n", CpuIdGet());
	struct netif *netif;
	for(netif=netif_list;netif!=NULL;netif=netif->next)
	{
		printf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\r\n", netif->name[0], netif->name[1], netif->num,
			netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

		printf("	inet addr:%IP Mask:%IP GW:%IP\r\n", netif->ip_addr.addr, netif->netmask.addr, netif->gw.addr);

		printf("	%s%s%s%s%s%s%sMTU:%d %s\r\n",
			netif->flags & NETIF_FLAG_UP ? "UP " : "",
			netif->flags & NETIF_FLAG_BROADCAST ? "BROADCAST " : "",
			netif->flags & NETIF_FLAG_POINTTOPOINT ? "PPP " : "",
			netif->flags & NETIF_FLAG_DHCP ? "DHCP " : "",
			netif->flags & NETIF_FLAG_LINK_UP ? "LINK-UP " : "",
			netif->flags & NETIF_FLAG_ETHARP ? "ETHARP " : "",
			netif->flags & NETIF_FLAG_IGMP ? "IGMP " : "",
			netif->mtu,
			netif == netif_default ? "DEFIF " : "");
	}
}

#include "lwip/tcp_impl.h"
#include "lwip/udp.h"
void cmd_netstat(void)
{
	struct tcp_pcb ** const sh_tcp_pcb_lists[] = {&tcp_listen_pcbs.pcbs, &tcp_bound_pcbs,
													&tcp_active_pcbs, &tcp_tw_pcbs};
	int i;
	char strBuffer[2][24];
	struct tcp_pcb *tpcb;
	struct udp_pcb *upcb;
	printf("-------------------------CPU%d netstat------------------------------\r\n", CpuIdGet());
	printf("Protocol  Local                  Remote                 State\r\n");
	printf("-------------------------------------------------------------------\r\n");
	for (i = 0; i < 4; i++) {
		for (tpcb = *sh_tcp_pcb_lists[i]; tpcb != NULL; tpcb = tpcb->next) {
			snprintf(strBuffer[0], 24, "%IP:%d", tpcb->local_ip.addr, tpcb->local_port);
			if (i == 0) { // LISTEN
				snprintf(strBuffer[1], 24, "%IP:*", tpcb->remote_ip.addr);
			} else { // BOUND, ACTIVE, TW
				snprintf(strBuffer[1], 24, "%IP:%d", tpcb->remote_ip.addr, tpcb->remote_port);
			}
			printf("TCP       %-21s  %-21s  %s\r\n", strBuffer[0], strBuffer[1], tcp_debug_state_str(tpcb->state));
		}
	}
	for (upcb = udp_pcbs; upcb != NULL; upcb = upcb->next) {
		snprintf(strBuffer[0], 24, "%IP:%d", upcb->local_ip.addr, upcb->local_port);
		snprintf(strBuffer[1], 24, "%IP:%d", upcb->remote_ip.addr, upcb->remote_port);
		printf("UDP       %-21s  %-21s\r\n", strBuffer[0], strBuffer[1]);
	}
	printf("-------------------------------------------------------------------\r\n");
}
#endif

#include <malloc.h>
void cmd_memstat(void)
{
	struct mallinfo minfo;
	minfo = mallinfo();
	printf("\r\n********************CPU%d Memory state********************\r\n", CpuIdGet());
	printf("arena : %d\r\n", (unsigned int) minfo.arena);			// This is the total size of memory allocated with sbrk by malloc, in bytes.
//	printf("sbmlks : %d\r\n", (unsigned int) minfo.smblks);			// This field is unused.
//	printf("hblks : %d\r\n", (unsigned int) minfo.hblks); 			// This is the total number of chunks allocated with mmap.
	printf("ordblks : %d\r\n", (unsigned int) minfo.ordblks);		// This is the number of chunks not in use.
//	printf("hblkhd : %d\r\n", (unsigned int) minfo.hblkhd);			// This is the total size of memory allocated with mmap, in bytes.
//	printf("usmblks : %d\r\n", (unsigned int) minfo.usmblks); 		// This field is unused.
//	printf("fsmblks : %d\r\n", (unsigned int) minfo.fsmblks); 		// This field is unused.
	printf("uordblks : %d\r\n", (unsigned int) minfo.uordblks);		// This is the total size of memory occupied by chunks handed out by malloc.
	printf("fordblks : %d\r\n", (unsigned int) minfo.fordblks);		// This is the total size of memory occupied by free (not in use) chunks.
	printf("keepcost : %d\r\n", (unsigned int) minfo.keepcost);		// This is the size of the top-most releasable chunk that normally borders the end of the heap.
	printf("*********************************************************\r\n");
}

void cmd_xsrlist(void)
{
	int i;
	printf("CPU%d XSR list\r\n", CpuIdGet());
	for(i=0;i<=14;i++)
	{
		printf("%02d:%s:\t0x%08X\r\n", i, get_xsr_str(i), get_xsr(i));
	}
}

#if defined(__ETH__) || defined(__WIFI__)
void cmd_tcp_all_close(void)
{
	// RTSP user all close
	rtsp_disconnect(0xFF);

	// TCP conncet all abort
	struct tcp_pcb *npcb, *pcb = tcp_active_pcbs;
	while(pcb) {
		npcb = pcb;
		pcb = pcb->next;
		tcp_abort(npcb);
	}
}
#endif

#if (LVDS_MARGIN_TEST==1)
BYTE gbLvdsOn = 0;
#endif
BYTE gbSetFontChg=0, gbMenuSave=0, gbMenuReset=0, gbOsdCamTitle=0, gbmenu_redraw=0, gbmenu_style=0; // 180319 KDH call SetFontChg in isp
BYTE gbWebIris=0;
ISRT void RuntimeMsg(void)
{
//#ifdef __ISP__
//#ifdef __H264__
//	static UINT getTemp;
//#endif
//#endif
	UINT nCmd, nArg;

	if(MsgGet(&gptMsgCpu1to0, &nCmd, &nArg) == DEF_OK)
	{
//		printf("call : RuntimeMsg : cmd(%d) : arg(0x%08X)\r\n", nCmd, nArg);
		switch(nCmd)
		{
#ifdef __ISP__
#if (LVDS_MARGIN_TEST==1)
			case MSG_CMD_SET_LVDS:
				if(nArg) gbLvdsOn = 1;
				else gbLvdsOn = 0;
				break;
#endif
			case MSG_CMD_SET_STREAM_PORT:
				rtsp_port_change(nArg);
				MsgPut(&gptMsgCpu0to1, MSG_CMD_SET_STREAM_PORT, gptMsgShare.RtspPort);
				break;
			case MSG_CMD_SET_STREAM_KICK:
				rtsp_disconnect(nArg);
				break;
#ifdef __JPEG__
			case MSG_CMD_SET_JPEG_SIZE:
//				enx_h264_set_frame_size_rs(nArg, 1);
				enx_jpeg_set_frame_size_rs(nArg);
				break;
			case MSG_CMD_SET_JPEG_QUALITY:
				enx_jpeg_set_quantize(nArg);
				break;
			case MSG_CMD_SET_JPEG_FPS:
				enx_jpeg_set_frame_rate_sf(nArg);
				break;
			case MSG_CMD_SET_JPEG_BRMODE:
				enx_jpeg_set_bit_rate_mode(nArg);
				break;
			case MSG_CMD_SET_JPEG_BRATE:
				enx_jpeg_set_bit_rate(nArg);
				break;
#endif
#ifdef __H264__
#ifdef __H264_MCH__
			case MSG_CMD_SET_H264_1CH_PROFILE_DEC:
				enx_h264_mch_set_profile_dec(nArg, 0);
				break;
#ifdef __H264_2CH__
			case MSG_CMD_SET_H264_2CH_PROFILE_DEC:
				enx_h264_mch_set_profile_dec(nArg, 1);
				break;
#endif	//__H264_2CH__
#endif	//__H264_MCH__

			case MSG_CMD_SET_H264_1CH_PROFILE:
				enx_h264_set_profile(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_SIZE:
				enx_h264_set_frame_size_rs(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_QUALITY:
				enx_h264_set_quantize(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_IDRFRAME:
				enx_h264_set_idrframe_period(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_FPS:
				enx_h264_set_frame_rate_sf(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_BRMODE:
				enx_h264_set_bit_rate_mode(nArg, 0);
				break;
			case MSG_CMD_SET_H264_1CH_BRATE:
				enx_h264_set_bit_rate(nArg, 0);
				break;
#ifdef __H264_2CH__
			case MSG_CMD_SET_H264_2CH_PROFILE:
				enx_h264_set_profile(nArg, 1);
				break;
			case MSG_CMD_SET_H264_2CH_SIZE:
				enx_h264_set_frame_size_rs(nArg, 1);
//				enx_jpeg_set_frame_size_rs(nArg);
				break;
			case MSG_CMD_SET_H264_2CH_QUALITY:
				enx_h264_set_quantize(nArg, 1);
				break;
			case MSG_CMD_SET_H264_2CH_FPS:
				enx_h264_set_frame_rate_sf(nArg, 1);
				break;
			case MSG_CMD_SET_H264_2CH_IDRFRAME:
				enx_h264_set_idrframe_period(nArg, 1);
				break;
			case MSG_CMD_SET_H264_2CH_BRMODE:
				enx_h264_set_bit_rate_mode(nArg, 1);
				break;
			case MSG_CMD_SET_H264_2CH_BRATE:
				enx_h264_set_bit_rate(nArg, 1);
				break;
#endif	//__H264_2CH__

#endif	//__H264__
#endif	//__ISP__

#ifdef __AUDIO__
			case MSG_CMD_SET_AUDIO:
				rtp_audio_switch(nArg);
				break;
#endif
#if defined(__ETH__) || defined(__WIFI__)
			case MSG_CMD_SET_NET_DEFIF:
				switch (nArg) {
#ifdef __ETH__
					case enlETHERNET:
#endif
#ifdef __WIFI__
					case enlWIFISTA:
					case enlWIFIUAP:
#endif
						netif_set_default(listNetif[nArg]);
						break;
					default:
						printf("Error-%d set default network interface fail/CPU0\r\n", nArg);
						break;
				}
				break;
#ifdef __ETH__
			case MSG_CMD_SW_ETHERNET:
				if((UINT)nArg == DEF_OFF)
				{
					netif_set_down(listNetif[enlETHERNET]);
					IrqDisable(IRQ_ETH);
					IntAdd(IRQ_ETH, NULL, NULL);
				}
				else
				{
					IntAdd(IRQ_ETH, (void*)isr_oeth_rx, NULL);
					IrqEnable(IRQ_ETH);
					netif_set_up(listNetif[enlETHERNET]);
				}
				break;
#if (PHY_LOOPBACK_TEST==1)
			case MSG_CMD_ETH_LOOPBACK:
				IntAdd(IRQ_ETH, (void*)isr_oeth_rx_loopback, NULL);
				IrqEnable(IRQ_ETH);
				break;
#endif
			case MSG_CMD_SET_NET_LINK:
				if (nArg) {
					netif_set_link_up(listNetif[enlETHERNET]);
				} else {
					netif_set_link_down(listNetif[enlETHERNET]);
				}
				break;
			case MSG_CMD_SET_NET_MAC:
				listNetif[enlETHERNET]->hwaddr[0] = gptMsgShare.ETH_MAC[0];
				listNetif[enlETHERNET]->hwaddr[1] = gptMsgShare.ETH_MAC[1];
				listNetif[enlETHERNET]->hwaddr[2] = gptMsgShare.ETH_MAC[2];
				listNetif[enlETHERNET]->hwaddr[3] = gptMsgShare.ETH_MAC[3];
				listNetif[enlETHERNET]->hwaddr[4] = gptMsgShare.ETH_MAC[4];
				listNetif[enlETHERNET]->hwaddr[5] = gptMsgShare.ETH_MAC[5];
				break;
			case MSG_CMD_SET_NET_IP:
				{
				struct ip_addr ipaddr;
				ipaddr.addr = (u32)nArg;
				if (listNetif[enlETHERNET]->ip_addr.addr != ipaddr.addr) {
					cmd_tcp_all_close();
					netif_set_ipaddr(listNetif[enlETHERNET], &ipaddr);
				}
				}
				break;
			case MSG_CMD_SET_NET_GW:
				{
				struct ip_addr ipaddr;
				ipaddr.addr = (u32)nArg;
				if (listNetif[enlETHERNET]->gw.addr != ipaddr.addr) {
					cmd_tcp_all_close();
					netif_set_gw(listNetif[enlETHERNET], &ipaddr);
				}
				}
				break;
			case MSG_CMD_SET_NET_SM:
				{
				struct ip_addr ipaddr;
				ipaddr.addr = (u32)nArg;
				if (listNetif[enlETHERNET]->netmask.addr != ipaddr.addr) {
					cmd_tcp_all_close();
					netif_set_netmask(listNetif[enlETHERNET], &ipaddr);
				}
				}
				break;
			case MSG_CMD_SET_NET_DNS0:
				{
#if LWIP_DNS
				struct ip_addr ipaddr;
				ipaddr.addr = (u32)nArg;
				dns_setserver(0, &ipaddr);
#endif
				}
				break;
			case MSG_CMD_SET_NET_DNS1:
				{
#if LWIP_DNS
				struct ip_addr ipaddr;
				ipaddr.addr = (u32)nArg;
				dns_setserver(1, &ipaddr);
#endif
				}
				break;
#endif
			case MSG_CMD_SET_USER_ID:
				gUserId = (char *)nArg;
				break;
			case MSG_CMD_SET_USER_PW:
				gUserPw = (char *)nArg;
				break;
			case MSG_CMD_VIEW_IFCONFIG:
				cmd_ifconfig();
				break;
			case MSG_CMD_VIEW_NETSTAT:
				cmd_netstat();
				break;
#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)
			case MSG_CMD_FTPD_OPEN_DATAPORT:
				ftpdata_open(nArg);
				break;
			case MSG_CMD_FTPD_CLOSE_DATAPORT:
				ftpdata_close(nArg);
				break;
			case MSG_CMD_FTPD_LIST:
			case MSG_CMD_FTPD_RETR:
//			case MSG_CMD_FTPD_STOR:
				ftpdata_send_index(nArg);
				break;
#endif
#endif
#if (RTSPoverHTTP==1)
			case MSG_CMD_RTSPoverHTTP:
				{
					tunnel_info arg_tunnel;
					if (MsgTunnelGet(&gptMsgTunnel, &arg_tunnel) == DEF_OK) {
						tunnel_get_pcb(&arg_tunnel);
					}
				}
				break;
#endif
#ifdef RTMP_SUPPORT
			case MSG_CMD_RTMP_CONNECT:
				printf("MSG_CMD_RTMP_CONNECT\r\n");
				rtmp_connect();
				break;
			case MSG_CMD_RTMP_DISCONNECT:
				printf("MSG_CMD_RTMP_DISCONNECT\n");
				rtmp_all_disconnect();
				break;
#endif
#endif
			case MSG_CMD_VIEW_MEMSTAT:
				cmd_memstat();
				break;
			case MSG_CMD_VIEW_XSR:
				cmd_xsrlist();
				break;
#ifdef __ISP__
// 180319 KDH Indirect calls of ISP function in isp_main through global variable
			case MSG_CMD_SET_ISP_SAVE:
//				MenuSave(MN_ON);
				gbMenuSave=1;
				break;
			case MSG_CMD_SET_ISP_RESET:
//				MenuReset();
//				SetFontChg();
				gbMenuReset=1;
				gbSetFontChg=1;
				break;
			case MSG_CMD_SET_ISP_TITLE:
//				OsdCamTitle();
				gbOsdCamTitle=1;
				break;
			case MSG_CMD_SET_ISP_FONT:
//				SetFontChg();
//				menu_redraw(1,0);
				gbSetFontChg=1;
				gbmenu_redraw=1;
				break;
			case MSG_CMD_SET_ISP_IRIS: // IRIS change from web page
				gbWebIris=1;
				break;
			case MSG_CMD_SET_ISP_STYLE:
				gbmenu_style=1;
				break;
#if 0//def __H264__
			case MSG_CMD_SET_H264_W:
				getTemp = (UINT)nArg;
				break;
			case MSG_CMD_SET_H264_H:
				h264_set_frame_size(getTemp, (UINT)nArg, 0);
				break;
#endif
			case MSG_CMD_SW_VLOCK_IRQ:
				if((UINT)nArg == DEF_ON)	IrqEnable(IRQ_ISP);
				else						IrqDisable(IRQ_ISP);
				break;
#ifdef __H264__
			case MSG_CMD_SW_H264_IRQ:
				if((UINT)nArg == DEF_ON)	IrqEnable(IRQ_H264);
				else						IrqDisable(IRQ_H264);
				break;
			case MSG_CMD_SET_H264_INIT:
				CRITICAL_BEGIN;
				SYS_H264_RST = 0;
				enx_h264_on();
				CRITICAL_END;
				enx_h264_info_print();
				break;
			case MSG_CMD_SET_H264_READY:
				enx_h264_image_capture_ready();
				break;
			case MSG_CMD_VIEW_H264_INFO:
				enx_h264_info_print();
				break;
			case MSG_CMD_SDSAVE_ON_OFF:
				enx_h264_set_sdsave(nArg);
				break;

			case MSG_CMD_SET_H264_1CH_IFRAME_SIZE:
				bitctl_set_iframe_maxsize(nArg, 0);
				break;
			case MSG_CMD_SET_H264_2CH_IFRAME_SIZE:
				bitctl_set_iframe_maxsize(nArg, 1);
				break;
			case MSG_CMD_SET_H264_1CH_PFRAME_SIZE:
				bitctl_set_pframe_maxsize(nArg, 0);
				break;
			case MSG_CMD_SET_H264_2CH_PFRAME_SIZE:
				bitctl_set_pframe_maxsize(nArg, 1);
				break;
			case MSG_CMD_SET_H264_1CH_OVERBIT_SIZE:
				bitctl_set_overbitsize(nArg, 0);
				break;
			case MSG_CMD_SET_H264_2CH_OVERBIT_SIZE:
				bitctl_set_overbitsize(nArg, 1);
				break;
#endif
#ifdef __JPEG__
			case MSG_CMD_VIEW_JPEG_INFO:
				enx_jpeg_info_print();
				break;
			case MSG_CMD_CHECK_JPEG:
				jpeg_irq_handler(NULL);
				break;
#endif
#endif
			default:
				printf("call : Unknown : cmd(%d) : arg(0x%08X)\r\n", nCmd, nArg);
				break;
		}
	}
}
