#include "dev.h"

#include "ewl.h"

#ifdef __WIFI__
/* 
    This function initialize wpa_supplicant.
    
    return 
    0 : fail, 1 : success         
*/

static void
supplicant_tmr_cb(void *ctx)
{
	ewl_supplicant_run();
	UNUSED(ctx);
}

void supplicant_run(void)
{
    timer_sched_timeout_cb(100, TIMEOUT_PERIODIC, supplicant_tmr_cb, NULL);
}

/*
    Input  
    @ctx point to pointer for struct owlsh_ctx  
*/

int supplicant_run_init(void *ctx)
{
    return ewl_supplicant_init();
	UNUSED(ctx);
}
#endif