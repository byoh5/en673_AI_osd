#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#include "user.h"

#define TimeZone_GMT 1
#define TimeZone_LOC 0

void timerTask(void* pvParameters);

int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec);
int setTimeZone(TimeZone tzIndex);
TimeZone getTimeZone(void);
void enx_get_tmtime(time_t get_time, struct tm *tmtime, UINT is_local);

#endif
