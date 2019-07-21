#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dev.h"
#include "dev_inline.h"															// for inline

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "user.h"
#include "msg.h"

#include "lwip/api.h"

#include "timer_task.h"
#if (ENX_SNTPC_use==1)
#include "sntpc.h"
#if defined(__WIFI__)
#include "wifi.h"
#endif
#endif

#include <time.h>
#ifdef __RTC_LOAD__
#include "rtc.h"
#endif
#ifdef __GSS_BMA250__
#include "bma250.h"
#endif

///////////////////////////////////////////////////

///////////////////////////////////////////////////

//timer example
#if 0

void test_cb(void *ctx)
{
	printf("hello");
}

//id=timer_sched_timeout_cb(100, TIMEOUT_PERIODIC,test_cb,NULL);

#endif

//#define __ADC_TEST__
#ifdef __ADC_TEST__
static void adc_get_data(void *ctx)
{
	WORD data[3];
	data[0] = (ADC_CH0_DAT&0x3FF);	// ch 0
	data[1] = (ADC_CH1_DAT&0x3FF);	// ch 1
	data[2] = (ADC_CH2_DAT&0x3FF);	// ch 2
	printf("Data:0(0x%03X(%04d)) 1(0x%03X(%04d)) 2(0x%03X(%04d))\r\n", data[0], data[0], data[1], data[1], data[2], data[2]);

	UNUSED(ctx);
}
#endif

void timer_check(void *ctx)
{
#ifdef GPIO_LED3
	if(GpioGetPin(GPIO_LED3))	GpioSetLo(GPIO_LED3);
	else						GpioSetHi(GPIO_LED3);
#endif
	UNUSED(ctx);
}

void RuntimeMsg(void)
{
	UINT nCmd, nArg;
	UINT nChangeflag = 0;

	if (MsgGet(&gptMsgCpu0to1, &nCmd, &nArg) == DEF_OK) {
		switch (nCmd) {
			case MSG_CMD_SET_STREAM_PORT:
				printf("MSG_CMD_SET_STREAM_PORT: %u\r\n", nArg);
				gtUser.nRtspPort = nArg;
				nChangeflag = 1;
				break;
			default:
				printf("CMD(%u) ARG(%u)\r\n", nArg);
				break;
		}
	}

	if (nChangeflag == 1) {
		UserSave(sfls_USER);
	}
}

void timerTask(void* pvParameters)
{
	timer_tick_init(BUS_CLK, TIME_TICK); //100Hz TIME TICK

#ifdef __GSS_BMA250__
	AccData accRes;
	bma250_init();
	timer_sched_timeout_cb(1000, TIMEOUT_PERIODIC, (void (*)(void *))bma250_get, (void *)&accRes);
#endif

#ifdef __ADC_TEST__
	timer_sched_timeout_cb(10, TIMEOUT_PERIODIC, adc_get_data, NULL);
#endif

#if 0
	timer_sched_timeout_cb(100, TIMEOUT_PERIODIC, timer_check, NULL);
#endif

#if (ENX_SNTPC_use==1)
#if defined(__ETH__) || defined(__WIFI__)
	if (gtUser.bUseSntp == DEF_ON) {
#if defined(__WIFI__)
		if (gtWificfg.STAcfg.un8Switch == WIFI_OFF) {
			gtUser.bUseSntp = DEF_OFF;
		} else
#endif
		{
			sntpc_start();
		}
	}
#endif
#endif

	for(;;)
	{
		timer_poll();
		RuntimeMsg();
#ifdef __GSS_BMA250__
		if(accRes.nSet == 1)
		{
#if 0
			printf("Temp(%d) X(%d)(0x%08X)(%8.6f)\r\n", accRes.nTemp,
				accRes.AccX.data, accRes.AccNorX, accRes.AccNorX);
#else
#if 1
			printf("Temp(%d) X(%03d)(%.2hf) Y(%03d)(%.2hf) Z(%03d)(%.2hf)\r\n", accRes.nTemp,
				accRes.AccX.data, accRes.AccNorX,
				accRes.AccY.data, accRes.AccNorY,
				accRes.AccZ.data, accRes.AccNorZ);
#else
			printf("Temp(%d) X(%d)(%8.6f) Y(%d)(%8.6f) Z(%d)(%8.6f)\r\n", accRes.nTemp,
				accRes.AccX.data, accRes.AccNorX, accRes.AccNorX,
				accRes.AccY.data, accRes.AccNorY, accRes.AccNorY,
				accRes.AccZ.data, accRes.AccNorZ, accRes.AccNorZ);
#endif
#endif
			accRes.nSet = 0;
		}
#endif
#ifdef WDT_SYS
		WdtSysCountReset();
#endif
#ifdef WDT_CPU1
		WdtCpuCountReset();	// CPU1 WDT reset
#endif
		vTaskDelay(1); /* timer tick is 10 ms*/
	}

	vTaskDelete(NULL);
	UNUSED(pvParameters);
}

int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec)
{
	if (nYear < 1970 || nYear > 2030) {
		printf("Error : you input(%d), Year 1970 ~ 2030\r\n", nYear);
		return DEF_FAIL;
	}
	if (nMonth < 1 || nMonth > 12) {
		printf("Error : you input(%d), Month 1 ~ 12\r\n", nMonth);
		return DEF_FAIL;
	}
	if (nDay < 1 || nDay > 31) {
		printf("Error : you input(%d), Day 1 ~ 31\r\n", nDay);
		return DEF_FAIL;
	}
	if (nHour > 23) {
		printf("Error : you input(%d), Hour 0 ~ 23\r\n", nHour);
		return DEF_FAIL;
	}
	if (nMin > 59) {
		printf("Error : you input(%d), Minute 0 ~ 59\r\n", nMin);
		return DEF_FAIL;
	}
	if (nSec > 59) {
		printf("Error : you input(%d), Sec 0 ~ 59\r\n", nSec);
		return DEF_FAIL;
	}

	struct tm tmget;
	tmget.tm_year = nYear - 1900;
	tmget.tm_mon = nMonth - 1;
	tmget.tm_mday = nDay;
	tmget.tm_hour = nHour;
	tmget.tm_min = nMin;
	tmget.tm_sec = nSec;
	tmget.tm_isdst = 0;
	if (nTimezone == TimeZone_GMT) {
		tmget.tm_isdst = 1;	// 입력값이 GMT+0시간이므로 1을 적용 (원래 이 용도가 아니다. 이것은 summer time을 위한 것.)
	} else if(nTimezone == TimeZone_LOC) {
		tmget.tm_isdst = 0;	// 입력값이 GMT+9시간이므로 0을 적용 (원래 이 용도가 아니다. 이것은 summer time을 위한 것.)
	}

	gptMsgShare.TIME = mktime(&tmget);
	gptMsgShare.TIMEZONE = nTimeZoneSec[getTimeZone()][2] + (gtUser.nTimeSummer * 3600);
	printf("MAKETIME : [%d]\r\n", gptMsgShare.TIME);

	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	printf("INPUT(GMT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	printf("INPUT(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	if (rtc_set_time(&tmout) == DEF_FAIL) {
		printf("RTC time set fail\r\n");
	}
#endif

	return DEF_OK;
}

int setTimeZone(TimeZone tzIndex)
{
	char strTimeZone[10] = {0};
	gtUser.nTimeZone = tzIndex;
	gptMsgShare.TIMEZONE = nTimeZoneSec[tzIndex][2] + (gtUser.nTimeSummer * 3600);
	sprintf(strTimeZone, "UTC%d:%d", -nTimeZoneSec[tzIndex][0], nTimeZoneSec[tzIndex][1]);
	return setenv("TZ", strTimeZone, 1);
}

TimeZone getTimeZone(void)
{
	int nHour = 0, nMin = 0, Mux = 0, i;
	char *out = getenv("TZ");
	if(out != NULL)
	{
		sscanf(out, "UTC%d:%d", &nHour, &nMin);
		if(nHour >= 0)	Mux = (-nHour * 3600) - (nMin * 30);
		else			Mux = (-nHour * 3600) + (nMin * 30);
		for(i=e_tzM1200;i<=e_tzP1400;i++)
		{
			if(nTimeZoneSec[i][2] == Mux)
			{
				if(gtUser.nTimeZone != (UINT)i)
					gtUser.nTimeZone = i;
				if(gptMsgShare.TIMEZONE != Mux)
					gptMsgShare.TIMEZONE = Mux + (gtUser.nTimeSummer * 3600);
				return i;
			}
		}
	}
	return e_tzN0000;
}

void enx_get_tmtime(time_t get_time, struct tm *tmtime, UINT is_local)
{
	if (tmtime == NULL) {
		return;
	}

	if (is_local == DEF_YES) {
		localtime_r(&get_time, tmtime);
	} else {
		gmtime_r(&get_time, tmtime);
	}
}