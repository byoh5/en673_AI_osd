#include <stdio.h>

#include "dev.h"
#include "dev_inline.h"
#include "msg.h"

#include "enx_wifi_ti_helper.h"

#include "user.h"

#ifdef __WIFI__

#include "ti_ap_test.h"


int enx_wifi_helper_init(void)
{
	printf("func : %s, line : %d \n\r", __func__, __LINE__);
	getting_started_with_wlan_ap_main();
	return 0;
}

void enx_wifi_poll_netif(void)
{
	
}

#endif
