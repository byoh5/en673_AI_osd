#include "dev.h"

#ifdef __RTC_LOAD__
#ifdef __RTC_PCF8563__
#include <time.h>
#include "rtc.h"
#include "pcf8563.h"

#define I2C_CHECK	I2c2_Check
#define I2C_WRITE	I2c2_Write
#define I2C_READ	I2c2_Read

/* Cache VL bit value read at driver init since writing the RTC_SECOND
 * register clears the VL status.
 */
static int voltage_low;
static const unsigned char days_in_month[] =
	{ 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

static UINT pcf8563_check(void)
{
	return I2C_CHECK(PCF8563_I2C_WRITE);
}

/**
    @return 
        DEF_OK is ok else DEF_FAIL.
*/
static BYTE pcf8563_write_reg(BYTE reg, BYTE data)
{
	while(I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(data,1, 0))	return DEF_FAIL;
	return DEF_OK;
}

/**
    @return 
        DEF_OK is ok else DEF_FAIL.
*/
static BYTE pcf8563_write_tm(struct tm *_tm)
{
	while(I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if(I2C_WRITE(PCF8563_SECONDS, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_sec, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_min, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_hour, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_mday, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_wday, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_mon, 0, 0))	return DEF_FAIL;
	if(I2C_WRITE(_tm->tm_year, 1, 0))	return DEF_FAIL;
	return DEF_OK;
}

/**
    @return 
        DEF_OK is ok else DEF_FAIL.
*/
static BYTE pcf8563_read_reg(BYTE reg, BYTE *data)
{
	while(I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if(I2C_WRITE(reg, 1, 1))	return DEF_FAIL;
	while(I2C_WRITE(PCF8563_I2C_READ, 0, 0));
	*data = I2C_READ(1, 1);
	return DEF_OK;
}

/**
    @return 
        DEF_OK is ok else DEF_FAIL.
*/
static BYTE pcf8563_read_tm(struct tm *_tm)
{
	while(I2C_WRITE(PCF8563_I2C_WRITE, 0, 0));
	if(I2C_WRITE(PCF8563_SECONDS, 1, 1))	return DEF_FAIL;
	while(I2C_WRITE(PCF8563_I2C_READ, 0, 0));
	_tm->tm_sec = I2C_READ(0, 0);
	_tm->tm_min = I2C_READ(0, 0);
	_tm->tm_hour = I2C_READ(0, 0);
	_tm->tm_mday = I2C_READ(0, 0);
	_tm->tm_wday = I2C_READ(0, 0);
	_tm->tm_mon = I2C_READ(0, 0);
	_tm->tm_year = I2C_READ(1, 1);
	return DEF_OK;
}

static int pcf8563_get_datetime(struct tm *_tm)
{
	if(pcf8563_read_tm(_tm) == DEF_FAIL)
		return DEF_FAIL;

	_tm->tm_year  = bcd2bin(_tm->tm_year) + ((_tm->tm_mon & 0x80) ? 100 : 0);
	_tm->tm_sec = bcd2bin(_tm->tm_sec & 0x7F);
	_tm->tm_min = bcd2bin(_tm->tm_min & 0x7F);
	_tm->tm_hour = bcd2bin(_tm->tm_hour & 0x3F);
	_tm->tm_mday = bcd2bin(_tm->tm_mday & 0x3F); /* Not coded in BCD. */
	_tm->tm_mon = bcd2bin(_tm->tm_mon & 0x1F);
	_tm->tm_mon--; /* Month is 1..12 in RTC but 0..11 in linux */

//	printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);
	return rtc_valid_tm(_tm);
}

static int pcf8563_set_datetime(struct tm *_tm)
{
    int leap;
    int year;
    int century;    
//	printf("%s(%d) : sec(%d) min(%d) hour(%d) wday(%d) mday(%d) mon(%d) year(%d)\r\n", __func__, __LINE__, _tm->tm_sec, _tm->tm_min, _tm->tm_hour, _tm->tm_wday, _tm->tm_mday, _tm->tm_mon, _tm->tm_year);

	if(rtc_valid_tm(_tm) == DEF_FAIL)
		return DEF_FAIL;

    /* Convert from struct tm to struct rtc_time. */
    _tm->tm_year += 1900;
    _tm->tm_mon += 1;
    
    /*
     * Check if _tm->tm_year is a leap year. A year is a leap
     * year if it is divisible by 4 but not 100, except
     * that years divisible by 400 _are_ leap years.
     */
    year = _tm->tm_year;
    leap = (_tm->tm_mon == 2) && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);

	/* Perform some sanity checks. */
	if ((_tm->tm_year < 1970) ||
	    (_tm->tm_mon > 12) ||
	    (_tm->tm_mday == 0) ||
	    (_tm->tm_mday > days_in_month[_tm->tm_mon] + leap) ||
	    (_tm->tm_wday >= 7) ||
	    (_tm->tm_hour >= 24) ||
	    (_tm->tm_min >= 60) ||
	    (_tm->tm_sec >= 60))
		return DEF_FAIL;

	century = (_tm->tm_year >= 2000) ? 0x80 : 0;
	_tm->tm_year = _tm->tm_year % 100;

	_tm->tm_year = bin2bcd(_tm->tm_year);
	_tm->tm_mon = bin2bcd(_tm->tm_mon);
	_tm->tm_mday = bin2bcd(_tm->tm_mday);
	_tm->tm_hour = bin2bcd(_tm->tm_hour);
	_tm->tm_min = bin2bcd(_tm->tm_min);
	_tm->tm_sec = bin2bcd(_tm->tm_sec);
	_tm->tm_mon |= century;

    return pcf8563_write_tm(_tm);
}

/**
	register pcf8563 to rtc_device.
	and init pcf8563

	@return
		0 : OK
		1 : FAIL
*/
int pcf8563_init(void)
{
	BYTE u8Data;

	if(pcf8563_check() == DEF_FAIL) 
	{
		printf("  >>PCF8563 Not Connected...\r\n");
		goto err;
	}

	printf("  >>PCF8563 Connected...\r\n");

	/*
	* First of all we need to reset the chip. This is done by
	* clearing control1, control2 and clk freq and resetting
	* all alarms.
	*/
	if(pcf8563_write_reg(PCF8563_CONTROL1, 0x00) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_CONTROL1\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_CONTROL2, 0x00) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_CONTROL2\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_CLOCKOUT_FREQ, 0x00) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_CLOCKOUT_FREQ\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_TIMER_CONTROL, 0x03) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_TIMER_CONTROL\r\n", __func__, __LINE__);
		goto err;
	}

	/* Reset the alarms. */
	if(pcf8563_write_reg(PCF8563_MINUTE_ALARM, 0x80) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_MINUTE_ALARM\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_HOUR_ALARM, 0x80) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_HOUR_ALARM\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_DAY_ALARM, 0x80) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_DAY_ALARM\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_write_reg(PCF8563_WEEKDAY_ALARM, 0x80) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error RTC_WEEKDAY_ALARM\r\n", __func__, __LINE__);
		goto err;
	}

	if(pcf8563_read_reg(PCF8563_SECONDS, &u8Data) == DEF_FAIL)
	{
		printf("%s(%d) : RTC error PCF8563_SECONDS\r\n", __func__, __LINE__);
		goto err;
	}
	else
	{
		voltage_low = 0;
		if(u8Data & 0x80)
		{
			voltage_low = 1;
			printf("RTC Voltage Low - reliable date/time informatio\r\n");
		}
	}

	rtc_device.read_time = pcf8563_get_datetime;
	rtc_device.set_time  = pcf8563_set_datetime;
	strcpy(rtc_device.name, "PCF8563");

	return DEF_OK;
err:
    printf("rtc init is error\r\n");
    return DEF_FAIL;	 
}
#endif
#endif
