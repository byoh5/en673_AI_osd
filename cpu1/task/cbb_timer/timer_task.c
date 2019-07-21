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
#include "sntpc.h"

#include <time.h>
#ifdef __RTC_LOAD__
#include "rtc.h"
#endif
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
	BYTE err[3];
	WORD data[3];
	data[0] = Adc_GetVal(1, &err[0]);	// ch 1
	data[1] = Adc_GetVal(2, &err[1]);	// ch 2
	data[2] = Adc_GetVal(3, &err[2]);	// ch 3
	printf("Data:1(0x%03X(%04d)E(%d)) 2(0x%03X(%04d)E(%d)) 3(0x%03X(%04d)E(%d))\r\n", data[0], data[0], err[0], data[1], data[1], err[1], data[2], data[2], err[2]);

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

#include "bma250.h"

void timerBootOK(void *ctx)
{
	gtUser.nbootTryCnt = 0;
	UserSave((BYTE*)&gtUser);
	UNUSED(ctx);
}

void timerTask(void* pvParameters)
{
	timer_tick_init(BUS_CLK, TIME_TICK); //100Hz TIME TICK

#ifdef __GSS_BMA250__
	AccData accRes;
	bma250_init();
	timer_sched_timeout_cb(1000, TIMEOUT_PERIODIC, (void (*)(void *))bma250_get, (void *)&accRes);
#endif

#ifdef __DEVICE_SD__
//	printf("t_avienc_t = sizeof(%d)\r\n", sizeof(t_avienc_t));
#endif

#ifndef __H264_TEST__
//	gtUser.uviH264.sfFPS = e_s30f10;
//	while(MsgJobPut(gptMsgJobMoci, MSG_CMD_SET_H264_1CH_FPS, gtUser.uviH264.sfFPS) == DEF_FAIL)	vTaskDelay(1);

//	gtUser.uviH264.eResolution = e_res640x480;
//	while(MsgJobPut(&gptMsgJobMoci, MSG_CMD_SET_H264_1CH_SIZE, gtUser.uviH264.eResolution) == DEF_FAIL)	vTaskDelay(1);
//	gtUser.uviH264_2.eResolution = e_res320x240;
//	while(MsgJobPut(&gptMsgJobMoci, MSG_CMD_SET_H264_2CH_SIZE, gtUser.uviH264_2.eResolution) == DEF_FAIL)	vTaskDelay(1);

//	gtUser.uviH264.nQuality= 20;
//	while(MsgJobPut(&gptMsgJobMoci, MSG_CMD_SET_H264_1CH_QUALITY, gtUser.uviH264.nQuality) == DEF_FAIL)		vTaskDelay(1);
//	gtUser.uviH264_2.nQuality = 20;
//	while(MsgJobPut(&gptMsgJobMoci, MSG_CMD_SET_H264_2CH_QUALITY, gtUser.uviH264_2.nQuality) == DEF_FAIL)	vTaskDelay(1);

//	printf("@@@@@@@@@@@@@@@@@");
#endif

//	vTaskDelay(200);

//#ifdef __DEVICE_SD__
//	tid_SDdetect = timer_sched_timeout_cb(200, TIMEOUT_PERIODIC, CheckSDstate, NULL);	// 100ms check
//#endif

#ifdef __ADC_TEST__
	timer_sched_timeout_cb(10, TIMEOUT_PERIODIC, adc_get_data, NULL);
#endif

#if 1
	timer_sched_timeout_cb(100, TIMEOUT_PERIODIC, timer_check, NULL);
#endif

	timer_sched_timeout_cb(5000, TIMEOUT_ONESHOT, timerBootOK, NULL);

	if(gtUser.bUseSntp == DEF_ON)
	{
		sntpc_start(NULL);
		tid_sntpc = timer_sched_timeout_cb(gtUser.nSntpPeriodic * 1000, TIMEOUT_PERIODIC, sntpc_start, NULL);
	}

	for(;;)
	{
		timer_poll();
#ifdef __GSS_BMA250__
		if(accRes.nSet == 1)
		{
#if 0
			printf("Temp(%d) X(%d)(0x%08X)(%8.6f)\r\n", accRes.nTemp,
				accRes.AccX.data, accRes.AccNorX, accRes.AccNorX);
#else
#if 1
			printf("Temp(%d) X(%03d)(%.2f) Y(%03d)(%.2f) Z(%03d)(%.2f)\r\n", accRes.nTemp,
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
#ifdef WDT_CPU1
		//CPU1 WDT reset
		CPU_WDT1_T = 0x3;
		CPU_WDT1_T = 0x2;
#endif
		vTaskDelay(1); /* timer tick is 10 ms*/
	}

	UNUSED(pvParameters);
}

int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec)
{
	if(nYear < 1970 || nYear > 2030)
	{
		printf("Error : you input(%d), Year 1970 ~ 2030\r\n", nYear);
		return DEF_FAIL;
	}
	if(nMonth < 1 || nMonth > 12)
	{
		printf("Error : you input(%d), Month 1 ~ 12\r\n", nMonth);
		return DEF_FAIL;
	}
	if(nDay < 1 || nDay > 31)
	{
		printf("Error : you input(%d), Day 1 ~ 31\r\n", nDay);
		return DEF_FAIL;
	}
	if(nHour > 23)
	{
		printf("Error : you input(%d), Hour 0 ~ 23\r\n", nHour);
		return DEF_FAIL;
	}
	if(nMin > 59)
	{
		printf("Error : you input(%d), Minute 0 ~ 59\r\n", nMin);
		return DEF_FAIL;
	}
	if(nSec > 59)
	{
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
	if(nTimezone == TimeZone_GMT)
	{
		tmget.tm_isdst = 1;	// 입력값이 GMT+0시간이므로 1을 적용
	}
	else if(nTimezone == TimeZone_LOC)
	{
		tmget.tm_isdst = 0;	// 입력값이 GMT+9시간이므로 0을 적용
	}

	gptMsgShare.TIME = mktime(&tmget);
	gptMsgShare.TIMEZONE = nTimeZoneSec[getTimeZone()][2] + (gtUser.nTimeSummer * 3600);
	printf("MAKETIME : [%d]\r\n", gptMsgShare.TIME);

	struct tm *tmprint = gmtime((const time_t *)&gptMsgShare.TIME);
	printf("INPUT(GMT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmprint->tm_year+1900, tmprint->tm_mon+1, tmprint->tm_mday, strWeek[tmprint->tm_wday], tmprint->tm_hour, tmprint->tm_min, tmprint->tm_sec);
	tmprint = localtime((const time_t *)&gptMsgShare.TIME);
	printf("INPUT(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmprint->tm_year+1900, tmprint->tm_mon+1, tmprint->tm_mday, strWeek[tmprint->tm_wday], tmprint->tm_hour, tmprint->tm_min, tmprint->tm_sec);

#ifdef __RTC_LOAD__
	tmprint = gmtime((const time_t *)&gptMsgShare.TIME);
	if(rtc_set_time(tmprint) == DEF_FAIL)
	{
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
	sprintf(strTimeZone, "UTC%d:%d", nTimeZoneSec[tzIndex][0], nTimeZoneSec[tzIndex][1]);
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

#if 0
#define UART1BAUD			(115200)
void Uart1Echo_polling(void)
{
	BYTE get;

	Uart1_Init(UART1BAUD);

	while(1)
	{
		while(Uart1_RxExist() == 0)
			vTaskDelay(1);

		get = Uart1_GetByte();

		Uart1_Tx(get);

		vTaskDelay(1);
	}
}

void IsrUart1(void *ctx)
{
	volatile BYTE bUART1_RX_IRQ = UART1_RX_IRQ;
	volatile BYTE bBuf;

	while(bUART1_RX_IRQ)
	{
		UART1_RX_IRQ_CLR = 1;
		do {
			if(UART1_RX_EMPTY) break;
			else {
				bBuf = UART1_RX_DAT;

				Uart1_Tx(bBuf);
		} while(1);

		bUART1_RX_IRQ = UART1_RX_IRQ;
	}
	UNUSED(ctx);
}

void Uart1Echo_irqinit(void)
{
	Uart1_Init(UART1BAUD);
	UART1_RX_IRQ_EN = 1;

	IntAdd(IRQ_UART1, (void*)IsrUart1, 0);
	IrqEnable(IRQ_UART1);
}
#endif
