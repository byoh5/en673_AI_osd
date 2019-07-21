#include "dev.h"
#include <time.h>	// struct tm
#include "rtc.h"

rtc_device_t rtc_device= {
	.name = {0},
	.read_time = NULL,
	.set_time = NULL
};

void rtc_clean(void)
{
	memset(&rtc_device, 0, sizeof(rtc_device));
}

/**
	@return
		FAIL : DEF_FAIL
		OK   : DEF_OK
*/
int rtc_set_time(struct tm *_tm)
{
	if(rtc_device.set_time)
	{
		return rtc_device.set_time(_tm);
	}
	return DEF_FAIL;
}

/**
	@return
		FAIL : DEF_FAIL
		OK   : DEF_OK
*/
int rtc_get_time(struct tm *_tm)
{
	if(rtc_device.read_time)
	{
		return rtc_device.read_time(_tm);
	}
	return DEF_FAIL;
}