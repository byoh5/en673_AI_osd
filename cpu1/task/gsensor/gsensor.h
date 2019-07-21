#ifndef __GSENSOR_TASK_H__
#define __GSENSOR_TASK_H__

#ifdef __CBB__

//#define GPIO_GSENSOR_1		0x6
//#define GPIO_GSENSOR_2		0x5

// Gsensor Event Th
#define GSS_Drive_1_X 6.0
#define GSS_Drive_1_Y 6.0
#define GSS_Drive_1_Z 6.0

#define GSS_Drive_2_X 5.0
#define GSS_Drive_2_Y 5.0
#define GSS_Drive_2_Z 5.0

#define GSS_Drive_3_X 4.0
#define GSS_Drive_3_Y 4.0
#define GSS_Drive_3_Z 4.0

#define GSS_Drive_4_X 3.0
#define GSS_Drive_4_Y 3.0
#define GSS_Drive_4_Z 3.0

#define GSS_Park_1 0.4
#define GSS_Park_2 0.3
#define GSS_Park_3 0.2
#define GSS_Park_4 0.1

#define GSenserData_len		(112)

typedef struct {
	int rev  : 22;
	int data : 10;
}AccDataPoint;

typedef struct {
	UINT nTemp;
	AccDataPoint AccX;
	AccDataPoint AccY;
	AccDataPoint AccZ;
	float AccNorX;
	float AccNorY;
	float AccNorZ;
}AccData;

typedef struct {
	AccData adData;
	char buffer[GSenserData_len+4];
}GSensorinfo;

extern int gsensor_state(void);
extern void gsensor_start(void);
extern void gsensor_stop(void);

extern void gsensor_isr(BYTE gpio_num);

extern BYTE gsensor_set_g_range(UINT is_save, BYTE g);
extern BYTE gsensor_get_g_range(BYTE *g);
extern BYTE gsensor_set_bandwidth(UINT is_save, BYTE ms);
extern BYTE gsensor_get_bandwidth(BYTE *ms);
extern BYTE gsensor_set_offset_filt(BYTE x, BYTE y, BYTE z);
extern BYTE gsensor_get_offset_filt(BYTE *x, BYTE *y, BYTE *z);

extern int gsensor_info_get(GSensorinfo **data);

extern void enx_gsensor_set_threshold(UINT is_save, eCBB_Mode eMode, UINT nStep);
extern void gsensorTask(void* pvParameters);

#endif

#endif
