#include "dev.h"
#include "msg.h"
#include "wlif_tools.h"

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "wifi_cfg.h"
extern tWificfg gtWificfg;
#endif	// ......................................

#define MAC2STR(a) (a)[0], (a)[1], (a)[2], (a)[3], (a)[4], (a)[5]
#define MACSTR "%02x:%02x:%02x:%02x:%02x:%02x"

/* 
	we scan aps and wait results.
*/
#ifdef __WIFI__
void cpu2cop_wlif_scan(void *arg)
{
	wlif_scan_t *pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
	cpu2cop_wlif_execmd("scan");

	gptMsgWlifCmd.resp = 0; // ask cpu1 to reply for cpu0
 	while(gptMsgWlifCmd.resp != 1)
		vTaskDelay(1);

 	pwlif_scan = (wlif_scan_t *)gptMsgWlifCmd.cmd;
 	if(!pwlif_scan->num)
 	{
 		printf("We have on scan\r\n");
 	}
 	else
 	{
 		int i = 0;
 		wl_scans_t *pscan;
 		printf("bssid / frequency / signal level / flags / ssid  \r\n");
 		for (i = 0; i < pwlif_scan->num; i++) {
			pscan = &pwlif_scan->wl_scans[i];
			printf(MACSTR "\t%d\t%d\t %s %s \r\n",
				  MAC2STR(pscan->bssid),
				  pscan->freq,
				  (char)pscan->level,
				  pscan->flags,
				  pscan->ssid);	
		}
 	}

#ifdef DEF_CPU1
	// STA mode에서 주변 AP검색 시, EN673 AP를 OFF시켰으므로 다시 ON
	if(gtWificfg.APcfg.un8Switch == WIFI_ON)
	{
		if(gtWificfg.STAcfg.un8Connect == WIFI_OFF)
		{
			cpu2cop_wlif_execmd("apset "WIFI_UAP_SET_CMD_BSSON);
		}
	}
#endif

	UNUSED(arg);
}
#endif