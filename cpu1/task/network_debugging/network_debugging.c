#include "dev.h"																
#include "dev_inline.h"	// for inline
#include <stdio.h>

#include "timer_task.h"
#include "network_debugging.h"

#if (ENX_NETWORK_Debug==1)

void Debugging_Task(void *ctx)
{
	UINT i;
	UINT old_time = gptMsgShare.TIME;
	UINT Video_Tick = gptMsgShare.VIDEO_TICK;
	UINT Tx_Max = 0, Rx_Max = 0, Tx_Min = 0, Rx_Min = 0;
	UINT TxperMin = 0, RxperMin = 0, TxperSec = 0, RxperSec = 0;
	struct tm tmout;

	gptMsgDebug.ETH_packet_tx = 0;
	gptMsgDebug.ETH_packet_rx = 0;
	for (i = 0; i < RTSP_info_MAX; i++) {
		gptMsgDebug.RTSP_packet_tx[i] = 0;
		gptMsgDebug.RTSP_packet_rx[i] = 0;
		gptMsgDebug.RTCP_packet_tx[i] = 0;
		gptMsgDebug.RTCP_packet_rx[i] = 0;
		gptMsgDebug.RTSP_frame_vid_put[i] = 0;
		gptMsgDebug.RTSP_frame_vid_get[i] = 0;
		gptMsgDebug.RTSP_frame_aud_put[i] = 0;
		gptMsgDebug.RTSP_frame_aud_get[i] = 0;
		gptMsgDebug.RTP_packet_len_tx[i] = 0;
		gptMsgDebug.RTP_packet_cnt_tx[i] = 0;
	}

	while(1) {
		if (old_time+1 == gptMsgShare.TIME) {
			old_time = gptMsgShare.TIME;
			Tx_Max = TxperSec > Tx_Max ? TxperSec : Tx_Max;
			Rx_Max = RxperSec > Rx_Max ? RxperSec : Rx_Max;
			Tx_Min = TxperSec < Tx_Min ? TxperSec : Tx_Min;
			Rx_Min = RxperSec < Rx_Min ? RxperSec : Rx_Min;
			TxperMin += TxperSec;
			RxperMin += RxperSec;
			TxperSec = gptMsgDebug.ETH_packet_tx / (1024 / 8); // Kbps 단위로 변경 
			RxperSec = gptMsgDebug.ETH_packet_rx / (1024 / 8); // Kbps 단위로 변경 
			gptMsgDebug.ETH_packet_tx = 0;
			gptMsgDebug.ETH_packet_rx = 0;

			if (gptMsgShare.TIME % 60 == 0) {
				enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
				printf("--------------------------%04d-%02d-%02d %02d:%02d:%02d--------------------------\r\n",tmout.tm_year+1900,tmout.tm_mon+1,tmout.tm_mday,tmout.tm_hour,tmout.tm_min,tmout.tm_sec);

				printf("|                |  Kbp/min  | Avr(Kbps) |  Max(Kbps) |  Min(Kbps) |\r\n");
				printf("|--------------------------------------------------------------------|\r\n");
				printf("|   Ethernet TX  | %10u | %10u | %10u | %10u |\r\n", TxperMin, TxperMin / 60, Tx_Max, Tx_Min);
				printf("|   Ethernet RX  | %10u | %10u | %10u | %10u |\r\n", RxperMin, RxperMin / 60, Rx_Max, Rx_Min);
				printf("|--------------------------------------------------------------------|\r\n");

				printf("|    Frame Count |RTSP-PC|RTCP-PC|RTP-PC|      RTP-FC       |  RTP   |\r\n");
				printf("|  %5d frame/m | TX RX | TX RX |  TX  |VPUT VGET APUT AGET|  Kbps  |\r\n", gptMsgShare.VIDEO_TICK - Video_Tick);
				printf("|--------------------------------------------------------------------|\r\n");
				for (i = 0; i < RTSP_info_MAX; i++) {
					printf("|%u|%s|%s| %2u %2u | %2u %2u |%6u|%4u %4u %4u %4u| %6u |\r\n", i, 
						gptMsgShare.rtsp_conn_info[i].rtp_type == 0 ? " UDP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 1 ? " TCP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 2 ? "HTTP" : gptMsgShare.rtsp_conn_info[i].rtp_type == 0xFFFF ? " -- " : " ?? ",
						gptMsgShare.rtsp_conn_info[i].video_type == 0 ? "H.264 1st" : gptMsgShare.rtsp_conn_info[i].video_type == 1 ? "H.264 2nd" : gptMsgShare.rtsp_conn_info[i].video_type == 9 ? "  MJPEG  " : gptMsgShare.rtsp_conn_info[i].video_type == 0xFFFF ? "   ---   " : "   ???   ",
						gptMsgDebug.RTSP_packet_tx[i], gptMsgDebug.RTSP_packet_rx[i],
						gptMsgDebug.RTCP_packet_tx[i], gptMsgDebug.RTCP_packet_rx[i],
						gptMsgDebug.RTP_packet_cnt_tx[i],
						gptMsgDebug.RTSP_frame_vid_put[i], gptMsgDebug.RTSP_frame_vid_get[i], gptMsgDebug.RTSP_frame_aud_put[i], gptMsgDebug.RTSP_frame_aud_get[i],
						((gptMsgDebug.RTP_packet_len_tx[i] / 60) * 8) >> 10);
					gptMsgDebug.RTSP_packet_tx[i] = 0;
					gptMsgDebug.RTSP_packet_rx[i] = 0;
					gptMsgDebug.RTCP_packet_tx[i] = 0;
					gptMsgDebug.RTCP_packet_rx[i] = 0;
					gptMsgDebug.RTSP_frame_vid_put[i] = 0;
					gptMsgDebug.RTSP_frame_vid_get[i] = 0;
					gptMsgDebug.RTSP_frame_aud_put[i] = 0;
					gptMsgDebug.RTSP_frame_aud_get[i] = 0;
					gptMsgDebug.RTP_packet_len_tx[i] = 0;
					gptMsgDebug.RTP_packet_cnt_tx[i] = 0;
				}
				printf("|------------------------------------PC(Packet Count)-FC(Frame Count)|\r\n");
				Video_Tick = gptMsgShare.VIDEO_TICK;
				Tx_Max = 0;
				Rx_Max = 0;
				Tx_Min = 0xffffffff;
				Rx_Min = 0xffffffff;
				TxperMin = 0;
				RxperMin = 0;
			}
		}
		if ((gptMsgShare.TIME-old_time)>2)
			old_time = gptMsgShare.TIME;

		vTaskDelay(1);
	
	}	
	vTaskDelete(NULL);

	UNUSED(ctx);
}
#endif
