/**
 *Copyright 2013 eyenix CO., LTD.
 *ALL RIGHTS RESERVED.
 *This software is provided under license and contains proprietary
 *and confidential material which is the property of Eyenix.
 *
 *FileName     : main.c
 *Description  :
 *Reversion    :
 *Comment      : first implementation
*/

#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "main.h"
#include "enx_os.h"
#include "msg.h"
#include "msgqpass.h"

#include "enx_lwip_helper.h"
#include "rtp_over_rtsp.h"

#include "h264.h"

#ifdef __AUDIO__
#include "enx_audio.h"
#endif

#ifdef __WIFI__
#include "inter_cpu.h"
#endif

#ifdef RTMP_SUPPORT
#include "rtmp_session.h"
#include "ngx_rtmp_stream.h"
#endif

#ifdef __H264__
#ifdef __H264_MCH__
#include "h264_mch.h"
#endif
#endif
extern void init(void);

StrLocalTime_t sltLocalTime;
inline void enx_strtime(void)
{
	static UINT old_sec = -1;
	if (old_sec != gptMsgShare.TIME) {
		struct tm tmprint;
		time_t loctime = gptMsgShare.TIME + gptMsgShare.TIMEZONE;
		localtime_r(&loctime, &tmprint);
		sltLocalTime.len = sprintf(sltLocalTime.data, "%04d/%02d/%02d %02d:%02d:%02d", tmprint.tm_year+1900, tmprint.tm_mon+1, tmprint.tm_mday, tmprint.tm_hour, tmprint.tm_min, tmprint.tm_sec);
		sltLocalTime.flag = 1;
		old_sec = gptMsgShare.TIME;

#if (ETH_MAC_PAUSE==1)
		if (gptMsgDebug.dltFLAG & eDltCPU0ETHPAUSECNT) {
			if (gptMsgNetworkRXShared.macp_count > 0) {
				CRITICAL_BEGIN;
				loctime = gptMsgNetworkRXShared.macp_time + gptMsgShare.TIMEZONE;
				localtime_r(&loctime, &tmprint);
				printf("[ETH] MAC Pause %02d:%02d:%02d : %5d\r\n", tmprint.tm_hour, tmprint.tm_min, tmprint.tm_sec, gptMsgNetworkRXShared.macp_count);
				gptMsgNetworkRXShared.macp_time = gptMsgShare.TIME;
				gptMsgNetworkRXShared.macp_count = 0;
				CRITICAL_END;
			}
		}
#endif
	}
}

#ifdef __WDT__
inline void enx_wdt_reset(void)
{
#ifdef WDT_SYS
	WdtSysCountReset(); // System WDT reset
	if (gptMsgShare.CPU1_WDT_COUNT > (WDT_DEFAULT_TIME*10)) {	// Check CPU1 Count
		CRITICAL_BEGIN;
		printf("CPU1 stop? System reboot!\r\n");
		WaitXms(100);
		WdtSysReboot();
	}
#endif
#ifdef WDT_CPU0
	WdtCpuCountReset(); // CPU0 WDT reset
#endif
}
#endif

ISRT int main(void)
{
	init();

	while (1) {
		timer_poll();
		enx_strtime();

		if (gptMsgShare.MSG_DUMMY != 0x5AA55AA5) {
			printf("ECM JTAG bug checking(buffer overflow:0x%08X)\r\n", gptMsgShare.MSG_DUMMY);
		}

#ifdef __H264__
		enx_h264_enc_poll();
#endif

		rtp_media_streamming();

		enx_poll_netif();

		extern volatile UINT gnIsrTick;
		extern volatile UINT gnIsrTickOn;
		extern void IF_Funcs(void);
		if(gnIsrTickOn && (gnIsrTick&3)==0) { gnIsrTickOn = 0; /*static UINT nCnt; if((++nCnt%25)==0) TxStrDec("", nCnt, 8);*/ IF_Funcs(); }	// '&3' : 100Hz / 4 = 25Hz

#ifdef __WIFI__
		inter_cpu_execmd();
		CRITICAL_BEGIN;
		xfer_pkt_from_cpu0();
		CRITICAL_END;
#endif

#ifdef RTMP_SUPPORT
		ngx_rtmp_play_send();
#endif

		RuntimeMsg();

#ifdef __WDT__
		enx_wdt_reset();
#endif
	}

	return 0;
}

/* EOF */
