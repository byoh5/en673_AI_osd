#ifndef __GPSRX_TASK_H__
#define __GPSRX_TASK_H__

//#define GPIO_GPS		0x1

#define GPSData_len		(112)
#define GPSMnemonicCnt	7

enum {
	eGPS_GGA = 0,
	eGPS_GLL = 1,
	eGPS_GSA = 2,
	eGPS_GSV = 3,
	eGPS_RMC = 4,
	eGPS_VTG = 5,
	eGPS_TXT = 6,
	eGPS_None = 255,
};

typedef struct {
	char strData[GPSData_len+4];
	int nCheckSum;
}GPSmnemonic;

typedef struct {
	GPSmnemonic info[GPSMnemonicCnt];
}GPSInfo;

extern char saved_info_gps[GPSData_len+4];
extern int gpsrx_state(void);
extern void gpsrx_start(void);
extern void gpsrx_stop(void);

extern int gps_info_get(GPSInfo **data);

void gpsTask(void* pvParameters);

#endif
