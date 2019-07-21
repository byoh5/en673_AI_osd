#ifndef __TIMER_TASK_H__
#define __TIMER_TASK_H__

#define TimeZone_GMT 1
#define TimeZone_LOC 0

int getSDState(void);

void timerTask(void* pvParameters);

int set_devicetime(int nTimezone, UINT nYear, UINT nMonth, UINT nDay, UINT nHour, UINT nMin, UINT nSec);
int setTimeZone(TimeZone tzIndex);
TimeZone getTimeZone(void);

#endif
