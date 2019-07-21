#include <stdio.h>
#include <math.h>
#include "dev.h"
#include "dev_regs.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "sys_arch.h"

//#include "iic.h"
#include "msg.h"
#include "user.h"

#ifdef __CBB__
#include "cbb_power.h"
#endif

#include "gsensor.h"

#if 0//def __CBB__

//#define ENX_GSENSOR_DEBUG			// printf 주석용

#ifdef ENX_GSENSOR_DEBUG
	#define gprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define gprintf(fmt, args...) do {} while(0);
#endif

extern BYTE sensor_addr;
#ifndef AVI_BUFFERING
#define AVI_BUFFERING 2
#endif
#define GSensorQueue_cnt	(AVI_BUFFERING*15)

typedef struct {
	unsigned int head;
	unsigned int tail;
	unsigned int tot_num;
	GSensorinfo data[GSensorQueue_cnt];
}GSensorinfo_Queue;

static GSensorinfo_Queue gss_info;
static int gState;

int gsensor_state(void)
{
	return gState;
}

void gsensor_start(void)
{
	// gsensor interrupt enable
	GpioEi(GPIO_GSENSOR_1);
//	GpioEi(GPIO_GSENSOR_2);

	gState = DEF_ON;
}

void gsensor_stop(void)
{
	// gsensor interrupt disable
	GpioDi(GPIO_GSENSOR_1);
//	GpioDi(GPIO_GSENSOR_2);

	gState = DEF_OFF;
}

extern sys_thread_t stt_avienc;
void gsensor_isr(BYTE gpio_num)
{
	switch(gpio_num)
	{
		case GPIO_GSENSOR_1:
			if(gtUser.bSdAviSave == SDSAVE_USE_ON)
			{
				if(stt_avienc != NULL)
				{
					if(cbbpower_get_mode() == eCBB_Drive)
					{
						//gprintf("event : park(%d), impact(%d)\n", muxer_avienc_parking_state(), muxer_avienc_impact_state());
						if(muxer_avienc_event_state() == 0)
						{
							gprintf("gsensor event save\n");
							muxer_avienc_event_go();
						}
					}
				}
			}
			gprintf("=HighG=\n");
			break;
		//case GPIO_GSENSOR_2:
		//	gprintf("*Orient*\n");
		//	break;
	}
}

#define Gsensor_addr 0x30

// measurement range selection
BYTE gsensor_set_g_range(UINT is_save, BYTE g)
{
#define BMA250_G_RANGE 0x0F
	BYTE bRes = DEF_FAIL, bG;
	switch(g)
	{
		case 2:		bG = 0x03;	break;	// +-2, 3.91mg/LSB
		case 4:		bG = 0x05;	break;	// +-4, 7.81mg/LSB
		case 8:		bG = 0x08;	break;	// +-8, 15.62mg/LSB
		case 16:	bG = 0x0C;	break;	// +-16, 31.25mg/LSB
		default:	goto done;
	}
	I2Clock('Y');
	IicW8(Gsensor_addr, BMA250_G_RANGE, bG, &bRes);
	I2Cunlock();
	gtUser.giData.acc_range = g;
	if(is_save == DEF_YES)
		UserSave(&gtUser);
done:
	return bRes;
}

BYTE gsensor_get_g_range(BYTE *g)
{
#define BMA250_G_RANGE 0x0F
	BYTE bRes = DEF_FAIL, bG;
	I2Clock('Z');
	bG = IicR8(Gsensor_addr, BMA250_G_RANGE, &bRes);
	I2Cunlock();
	switch(bG)
	{
		case 0x03:	*g = 2;		break;
		case 0x05:	*g = 4;		break;
		case 0x08:	*g = 8;		break;
		case 0x0C:	*g = 16;	break;
	}
done:
	return bRes;
}

// bandwidth configuration
BYTE gsensor_set_bandwidth(UINT is_save, BYTE ms)
{
#define BMA250_BANDWIDTH 0x10
	BYTE bRes = DEF_FAIL, bMs;
	switch(ms)
	{
		case 64:	bMs = 0x08;	break;	// 64ms, 7.81Hz
		case 32:	bMs = 0x09;	break;	// 32ms, 15.63Hz
		case 16:	bMs = 0x0A;	break;	// 16ms, 31.25Hz
		case 8:		bMs = 0x0B;	break;	// 8ms, 62.5Hz
		case 4:		bMs = 0x0C;	break;	// 4ms, 125Hz
		case 2:		bMs = 0x0D;	break;	// 2ms, 250Hz
		case 1:		bMs = 0x0E;	break;	// 1ms, 500Hz
		case 0:		bMs = 0x0F;	break;	// 0.5ms, 1000Hz
		default:	goto done;
	}
	I2Clock('z');
	IicW8(Gsensor_addr, BMA250_BANDWIDTH, bMs, &bRes);
	I2Cunlock();
	gtUser.giData.acc_bandwidth = ms;
	if(is_save == DEF_YES)
		UserSave(&gtUser);	
done:
	return bRes;
}

BYTE gsensor_get_bandwidth(BYTE *ms)
{
#define BMA250_BANDWIDTH 0x10
	BYTE bRes = DEF_FAIL, bMs;
	I2Clock('y');
	bMs = IicR8(Gsensor_addr, BMA250_BANDWIDTH, &bRes);
	I2Cunlock();
	switch(bMs)
	{
		case 0x08:	*ms = 64;	break;
		case 0x09:	*ms = 32;	break;
		case 0x0A:	*ms = 16;	break;
		case 0x0B:	*ms = 8;	break;
		case 0x0C:	*ms = 4;	break;
		case 0x0D:	*ms = 2;	break;
		case 0x0E:	*ms = 1;	break;
		case 0x0F:	*ms = 0;	break;
	}
done:
	return bRes;
}

// contains the compensation value for filtered data for the x-axis, y-axis, z-axis
BYTE gsensor_set_offset_filt(BYTE x, BYTE y, BYTE z)
{
#define BMA250_OFFSET_FILT_X 0x38
#define BMA250_OFFSET_FILT_Y 0x39
#define BMA250_OFFSET_FILT_Z 0x3A
	BYTE bRes[3] = {DEF_FAIL, DEF_FAIL, DEF_FAIL};
	I2Clock('x');
	IicW8(Gsensor_addr, BMA250_OFFSET_FILT_X, x, &(bRes[0]));	// offset_filt_x
	IicW8(Gsensor_addr, BMA250_OFFSET_FILT_Y, y, &(bRes[1]));	// offset_filt_y
	IicW8(Gsensor_addr, BMA250_OFFSET_FILT_Z, z, &(bRes[2]));	// offset_filt_z
	I2Cunlock();
	return bRes[0] & bRes[1] & bRes[2];
}

BYTE gsensor_get_offset_filt(BYTE *x, BYTE *y, BYTE *z)
{
#define BMA250_OFFSET_FILT_X 0x38
#define BMA250_OFFSET_FILT_Y 0x39
#define BMA250_OFFSET_FILT_Z 0x3A
	BYTE bRes[3] = {DEF_FAIL, DEF_FAIL, DEF_FAIL};
	I2Clock('w');
	*x = IicR8(Gsensor_addr, BMA250_OFFSET_FILT_X, &(bRes[0]));	// offset_filt_x
	*y = IicR8(Gsensor_addr, BMA250_OFFSET_FILT_Y, &(bRes[1]));	// offset_filt_y
	*z = IicR8(Gsensor_addr, BMA250_OFFSET_FILT_Z, &(bRes[2]));	// offset_filt_z
	I2Cunlock();
	return bRes[0] & bRes[1] & bRes[2];
}

BYTE gsensor_set_high_th(BYTE high_th)
{
#define BMA250_HIGH_TH 0x26
	BYTE bRes = DEF_FAIL;
	I2Clock('v');
	IicW8(Gsensor_addr, BMA250_HIGH_TH, high_th, &bRes);	// high_th
	I2Cunlock();
	return bRes;
}

BYTE gsensor_get_high_th(BYTE *high_th)
{
#define BMA250_HIGH_TH 0x26
	BYTE bRes = DEF_FAIL;
	I2Clock('u');
	*high_th = IicR8(Gsensor_addr, BMA250_HIGH_TH, &bRes);	// high_th
	I2Cunlock();
	return bRes;
}

BYTE gsensor_set_low_th(BYTE low_th)
{
#define BMA250_LOW_TH 0x23
	BYTE bRes = DEF_FAIL;
	I2Clock('t');
	IicW8(Gsensor_addr, BMA250_LOW_TH, low_th, &bRes);	// low_th
	I2Cunlock();
	return bRes;
}

BYTE gsensor_get_low_th(BYTE *low_th)
{
#define BMA250_LOW_TH 0x23
	BYTE bRes = DEF_FAIL;
	I2Clock('s');
	*low_th = IicR8(Gsensor_addr, BMA250_LOW_TH, &bRes);	// low_th
	I2Cunlock();
	return bRes;
}

void enx_gsensor_set_threshold(UINT is_save, eCBB_Mode eMode, UINT nStep)
{
	if(nStep >= 1 && nStep <= 4)
	{
		switch(eMode)
		{
			case eCBB_Drive:
				gtUser.giData.acc_evt_driving = nStep;
				break;
			default:
				printf("%s(%d) : Error Mode value(%d) / Step(%d)\n", eMode, nStep);
				return;
		}
	}
	else
	{
		printf("%s(%d) : Error Step value(%d) / Mode(%d)\n", nStep, eMode);
		return;
	}
	if(is_save == DEF_YES)
	{
		//cbbsetting_put_ini(0);
		UserSave(&gtUser);
	}
}

static void gsensor_init(void)
{
	BYTE bErr;

	GpioInDir(GPIO_GSENSOR_1);
	//GpioInDir(GPIO_GSENSOR_2);

	// sensor setup
	// base data sheet : BMA250_DS002-05
	gtUser.giData.acc_range=16;
	gsensor_set_g_range(DEF_NO, gtUser.giData.acc_range);		// +-16G
	gsensor_set_bandwidth(DEF_NO, gtUser.giData.acc_bandwidth);	// 125Hz == 4ms
#if 1
	gsensor_set_offset_filt(0, 0x80, 0);
#else
	IicW8(Gsensor_addr, 0x36, 128, &bErr);	// offset reset
#endif

//	I2Clock();
//	IicW8(Gsensor_addr, 0x16, 0x40, &bErr);				// interrupt enable(orientation)
//	IicW8(Gsensor_addr, 0x0C, 0x04|0x02|0x01, &bErr);	// 
//	IicW8(Gsensor_addr, 0x17, 0x04|0x02|0x01, &bErr);	// interrupt enable(high-g z,y,x)
//	IicW8(Gsensor_addr, 0x19, 0x02, &bErr);				// high-g interrupt enable(INT1)
//	IicW8(Gsensor_addr, 0x1B, 0x40, &bErr);				// orientation interrupt enable(INT2)
//	IicW8(Gsensor_addr, 0x17, 0x08, &bErr);				// interrupt enable(low-g)
//	IicW8(Gsensor_addr, 0x19, 0x01, &bErr);				// low-g interrupt enable(INT1)
//	I2Cunlock();
}

BYTE gsensor_read(AccData *adOut)
{
	int i;
	BYTE Addr[7] = {0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8}; // x_l, x_m, y_l, y_m, z_l, z_m, temp
	UINT Data[7] = {0};

	while(IIC_BUSY)//ksj_151228_iic 
	{
		UartPutByte('G');
		//vTaskDelay(1);
		//printf("%s(%d) Start IIC_BUSY(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02);
		
		goto	_ErrIicR;
	}
	while(IIC_AL)//ksj_151228_iic 
	{
		UartPutByte('A');
		//vTaskDelay(1);
		//printf("%s(%d) Start IIC_AL(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02);
		goto	_ErrIicR;
	}
	IIC_TXR = Gsensor_addr;				// Start & Slave
	IIC_CR	= 0x80|0x10;				//	"				(STA|WR)
	while(IIC_TIP); 					//	"
	if(IIC_RXACK)	goto	 _ErrIicR;	// If no ack, error

	IIC_TXR = 0x2;						// Addr(0x2 ~ 0x08)
	IIC_CR	= 0x10; 					//	"				(WR)
	while(IIC_TIP); 					//	"
	if(IIC_RXACK)	goto	 _ErrIicR;	// If no ack, error

	IIC_TXR = (Gsensor_addr|0x01);		// Start & Slave(R)
	IIC_CR	= 0x80|0x10;				//	"				(STA|WR)
	while(IIC_TIP); 					//	"
	if(IIC_RXACK)	goto	 _ErrIicR;	// If no ack, error

	for(i=0;i<6;i++)
	{
		IIC_TXR = 0xff; 				// Data read
		IIC_CR	= 0x20;					//	"				(RD)
		while(IIC_TIP); 				//	"
		if(IIC_RXACK)	goto	_ErrIicR;	// If no ack, error
		Data[i] = IIC_RXR;
	}

	IIC_TXR = 0xff; 					// Data read
	IIC_CR	= 0x20|0x08|0x40;			//	"				(RD|NACK|STO)
	while(IIC_TIP); 					//	"
	Data[i] = IIC_RXR;

	if(IIC_BUSY)
	{
		printf("%s(%d) End IIC_BUSY(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02+i);
		
		IIC_TXR = 0xff; 				// Dummy
		IIC_CR	= 0x40; 				//	"				(STO)
		while(IIC_TIP); 				//	"
	}
	if(IIC_AL)
	{
		printf("%s(%d) End IIC_AL(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02+i);
		
		IIC_TXR = 0xff; 				// Dummy
		IIC_CR	= 0x40; 				//	"				(STO)
		while(IIC_TIP); 				//	"
	}

	adOut->nTemp = (Data[6] * 0.5) + 24;	// 0x00 == 24℃
	adOut->AccX.data = (((UINT)(Data[1] & 0x000000FF)) << 2) | (((UINT)(Data[0] >> 6)) & 0x00000003);
	adOut->AccY.data = (((UINT)(Data[3] & 0x000000FF)) << 2) | (((UINT)(Data[2] >> 6)) & 0x00000003);
	adOut->AccZ.data = (((UINT)(Data[5] & 0x000000FF)) << 2) | (((UINT)(Data[4] >> 6)) & 0x00000003);

	//normalization
	float unit = (gtUser.giData.acc_range * 2) / 1023.0;
	adOut->AccNorX = unit * adOut->AccX.data;
	adOut->AccNorY = unit * adOut->AccY.data;
	adOut->AccNorZ = unit * adOut->AccZ.data;

	return 1;

_ErrIicR:

	if(IIC_BUSY)
	{
		printf("%s(%d) Error IIC_BUSY(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02+i);
		IIC_TXR = 0xff; 				// Dummy
		IIC_CR	= 0x40; 				//	"				(STO)
		while(IIC_TIP); 				//	"
	}
	if(IIC_AL)
	{
		printf("%s(%d) Error IIC_AL(0x%02X:0x%02X)\n", __func__, __LINE__, Gsensor_addr, 0x02+i);
		
		IIC_TXR = 0xff; 				// Dummy
		IIC_CR	= 0x40; 				//	"				(STO)
		while(IIC_TIP); 				//	"
	}

	if(IIC_BUSY)						// I2C use?
	{
		IIC_TXR = 0xff; 				// Dummy
		IIC_CR	= 0x40; 				//	"				(STO)
		while(IIC_TIP); 				//	"
	}

	adOut->nTemp = 0;
	adOut->AccX.data = 0;
	adOut->AccY.data = 0;
	adOut->AccZ.data = 0;
	adOut->AccNorX = 0.0;
	adOut->AccNorY = 0.0;
	adOut->AccNorZ = 0.0;
	
	return 0;
}

static int gsensor_get(AccData *adOut)
{
	int bRes = DEF_OK;

	I2Clock('q');
	bRes=gsensor_read(adOut);
	I2Cunlock();

	if(bRes)
	{
//		gptMsgCop2Cpu->gsslock = 1;
		gptMsgCop2Cpu->gssX = adOut->AccNorX;
		gptMsgCop2Cpu->gssY = adOut->AccNorY;
		gptMsgCop2Cpu->gssZ = adOut->AccNorZ;
//		gptMsgCop2Cpu->gsslock = 0;
	}

#ifdef ENX_GSENSOR_DEBUG
	//gprintf("Temp(%2d), X(%+04d,%+.1f), Y(%+04d,%+.1f), Z(%+04d,%+.1f)\n", 
	//		adOut->nTemp, adOut->AccX.data, adOut->AccNorX, adOut->AccY.data, adOut->AccNorY, adOut->AccZ.data, adOut->AccNorZ);
#endif
	return bRes;
}

static int gsensor_info_put(AccData *adIn)
{
	int nRes = DEF_OK;

	if(cQueue_isfull(&gss_info) == DEF_OK)
	{	// overflow
//		gprintf("error overflow\n");
		nRes = DEF_FAIL;
	}

	memcpy(&gss_info.data[gss_info.tail].adData, adIn, sizeof(AccData));
	memset(gss_info.data[gss_info.tail].buffer, 0, GSenserData_len);
	sprintf(gss_info.data[gss_info.tail].buffer, "gsensor,%d,%d,%d", adIn->AccX.data, adIn->AccY.data, adIn->AccZ.data);
//	gprintf("%s\n", gss_info.data[gss_info.tail].buffer);

	num_loop(gss_info.tail, gss_info.tot_num);

	return nRes;
}

int gsensor_info_get(GSensorinfo **data)
{
	int nRes = DEF_FAIL;

	if(cQueue_isempty(&gss_info) == DEF_FAIL)
	{
		*data = &gss_info.data[gss_info.head];

		num_loop(gss_info.head, gss_info.tot_num);
		
		nRes = DEF_OK;
	}
	return nRes;
}

static void gsensor_info_init(void)
{
	memset(&gss_info, 0, sizeof(gss_info));
	gss_info.tot_num = GSensorQueue_cnt;
}

int gsensor_event_processing(float fdata, float th)
{
	int nRes = DEF_NO;
	if(th != 0.0)
	{
		if(fabs(fdata) >= th)
		{
			gsensor_isr(GPIO_GSENSOR_1);
			nRes = DEF_YES;
		}
	}
	return nRes;
}

void gsensor_event_check(AccData *adNow, AccData *adOld)
{
	float fnowdata, folddata;		// parking
	float fdatax, fdatay, fdataz;	// driving

	switch(cbbpower_get_mode())
	{
		case eCBB_Drive:
			fdatax = adNow->AccNorX - adOld->AccNorX;
			fdatay = adNow->AccNorY - adOld->AccNorY;
			fdataz = adNow->AccNorZ - adOld->AccNorZ;
			switch(gtUser.giData.acc_evt_driving)
			{
				case 1:
					if(gsensor_event_processing(fdatax, GSS_Drive_1_X) == DEF_YES)
						break;
					if(gsensor_event_processing(fdatay, GSS_Drive_1_Y) == DEF_YES)
						break;
					if(gsensor_event_processing(fdataz, GSS_Drive_1_Z) == DEF_YES)
						break;
					break;
				case 2:
					if(gsensor_event_processing(fdatax, GSS_Drive_2_X) == DEF_YES)
						break;
					if(gsensor_event_processing(fdatay, GSS_Drive_2_Y) == DEF_YES)
						break;
					if(gsensor_event_processing(fdataz, GSS_Drive_2_Z) == DEF_YES)
						break;
					break;
				case 3:
					if(gsensor_event_processing(fdatax, GSS_Drive_3_X) == DEF_YES)
						break;
					if(gsensor_event_processing(fdatay, GSS_Drive_3_Y) == DEF_YES)
						break;
					if(gsensor_event_processing(fdataz, GSS_Drive_3_Z) == DEF_YES)
						break;
					break;
				case 4:
					if(gsensor_event_processing(fdatax, GSS_Drive_4_X) == DEF_YES)
						break;
					if(gsensor_event_processing(fdatay, GSS_Drive_4_Y) == DEF_YES)
						break;
					if(gsensor_event_processing(fdataz, GSS_Drive_4_Z) == DEF_YES)
						break;
					break;
				default:
					break;
			}
			//gprintf("step(%d) fdatax(%.1f) fdatay(%.1f) fdataz(%.1f)\n", gtUser.giData.acc_evt_driving, fdatax, fdatay, fdataz);
			break;
	}
}

extern xSemaphoreHandle gssSem;
void gsensorTask(void* pvParameters)
{
	// init
	gsensor_info_init();
	gsensor_init();
	gsensor_start();

	AccData adData, adTemp;
	int cnt = 0, i = 0, q = 2, checksw = 0;
	BYTE gSenPos=1, gSenPosCnt=0,gSenRevCnt=0,bErr;

	for(;;)
	{
#if 0
		// 2frame(66.6ms) 당 1회 get
#if 1
		cnt++;
		if(cnt == 3)
		{
			vTaskDelay(6);
			cnt = 0;
		}
		else
		{
			vTaskDelay(7);
		}
#else
		// 3frame(99.9ms) 당 1회 get
		vTaskDelay(10);
#endif

		if(gsensor_get(&adData) == DEF_OK)
		{
			if(++i > 30)	checksw = 1;
			if(checksw)
			{
				gsensor_event_check(&adData, &adTemp);
			}
			gsensor_info_put(&adData);
			adTemp = adData;
		}
#else
		if(xSemaphoreTake(gssSem, portMAX_DELAY) == pdTRUE)
		{
			i++;
			if(cbbpower_get_mode() == eCBB_Drive)
				q = 2;	// 30fps mode / 2fps당 1개
			if(i%q==0)
			{
				if(gsensor_get(&adData) == DEF_OK)
				{
					//gprintf("X(%+04d), Y(%+04d), Z(%+04d)\n", 
					//		adData.AccX.data, adData.AccY.data, adData.AccZ.data);
#if 0
					if((adData.AccX.data<=(-80))&&(adData.AccY.data<=(-128))&&(adData.AccZ.data<80)&&(adData.AccZ.data>(-80))&&(gSenPos==0))
					{
						
						if(gptMsgCop2Cpu->VLOCK == 0)
						{	
							za20s10_mcu_switch(DEF_OFF);					
						
							gSenRevCnt=0;
							gSenPosCnt++;
							if(gSenPosCnt>15)
							{printf("!!");
								gSenPos=1;
								I2Clock('Y');
								IicW8(sensor_addr, 0x00, 0x00, &bErr);	// chang page A
								IicW8(sensor_addr, 0x18, 0xc0, &bErr);
								IicW8(sensor_addr, 0xB4, 0x01, &bErr);
								I2Cunlock();
							}
							za20s10_mcu_switch(DEF_ON);
						}
					}
					else if((adData.AccX.data>=(80))&&(adData.AccY.data<=(-128))&&(adData.AccZ.data<80)&&(adData.AccZ.data>(-80))&&(gSenPos==1))
					{
						if(gptMsgCop2Cpu->VLOCK == 0)
						{
							za20s10_mcu_switch(DEF_OFF);
							gSenRevCnt++;
							gSenPosCnt=0;
							if(gSenRevCnt>15)
							{printf("!!");
								gSenPos=0;
								I2Clock('Y');
								IicW8(sensor_addr, 0x00, 0x00, &bErr);	// chang page A
								IicW8(sensor_addr, 0x18, 0x00, &bErr);
								IicW8(sensor_addr, 0xB4, 0x00, &bErr);
								I2Cunlock();
							}
							za20s10_mcu_switch(DEF_ON);
						}
					}
					else
					{
						gSenRevCnt=0;
						gSenPosCnt=0;
					}
#endif
					if(i > 30)	checksw = 1;
					if(checksw)
					{
						gsensor_event_check(&adData, &adTemp);
					}
					gsensor_info_put(&adData);
					adTemp = adData;
				}
			}
		}
#endif

		if(cbbpower_get_mode() == eCBB_Off)
			break;
	}

	vTaskDelete(NULL);
}
#endif
