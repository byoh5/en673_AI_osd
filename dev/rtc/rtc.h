#ifndef __RTC_H__
#define __RTC_H__

typedef struct {
	char name[20];
	int (*read_time)(struct tm *);
	int (*set_time)(struct tm *);
} rtc_device_t;

#define is_leap_year(year) ((!((year) % 4) && ((year) % 100)) || !((year) % 400))
#define bcd2bin(val) (((val) & 0x0f) + ((val) >> 4) * 10)
#define bin2bcd(val) ((((val) / 10) << 4) + (val) % 10)

extern int rtc_month_days(unsigned int month, unsigned int year);
extern int rtc_valid_tm(struct tm *tm);

extern void rtc_clean(void);
extern int rtc_set_time(struct tm *_tm);
extern int rtc_get_time(struct tm *_tm);

extern rtc_device_t rtc_device;

#endif
