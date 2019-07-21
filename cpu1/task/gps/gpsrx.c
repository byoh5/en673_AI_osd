#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "dev.h"
#include "dev_inline.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

//#include "vsprintf.h"
//#include "gpio.h"
#include "view.h"
#include "msg.h"

#include "cbb_power.h"
#include "gpsrx.h"
#include "timer_task.h"

//#define ENX_GPSRX_DEBUG			// printf 주석용

#ifdef ENX_GPSRX_DEBUG
	#define gprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define gprintf(fmt, args...) do {} while(0);
#endif

#define GPSBuffer_len 4096
#ifndef AVI_BUFFERING
#define AVI_BUFFERING 2
#endif
#define GPSQueue_cnt (AVI_BUFFERING*30)

typedef struct {
	unsigned int head;
	unsigned int tail;
	unsigned int tot_num;
	char buffer[GPSBuffer_len];
}GPSirqRX_Queue;

typedef struct {
	unsigned int head;
	unsigned int tail;
	unsigned int tot_num;
	GPSInfo data[GPSQueue_cnt];

	unsigned int nSelect;

	char strCheckID[6+2];
	unsigned int nCheckPos;
	unsigned int nCheckID;

	unsigned int nCheckSum;
	unsigned int nCheckSumState;

	unsigned int nEnd;
}GPSinfo_Queue;

static GPSirqRX_Queue gps_data;
static GPSinfo_Queue gps_info;
static int gState;
int get_gpstime;
int gpsConnect=0;
char saved_info_gps[GPSData_len+4]={0};

int gpsrx_state(void)
{
	return gState;
}

void gpsrx_start(void)
{
	// gps on
	GpioSetHi(GPIO_GPS);

	// interrupt enable
	Ei(IRQ_VIEW);

	gState = DEF_ON;
	gpsConnect=0;
	gptMsgShare.gpsCONN = eGPS_DISCONN;
}

void gpsrx_stop(void)
{
	// gps off
	GpioSetLo(GPIO_GPS);

	// interrupt disable
	Di(IRQ_VIEW);

	gState = DEF_OFF;
}

void gps_info_print(GPSInfo *gpsinfo)
{
	int i;
	gprintf("=========================================\n");
	for(i=0;i<GPSMnemonicCnt;i++)
	{
		if(gpsinfo->info[i].nCheckSum)
		{
			gprintf("%02X : %s\n", gpsinfo->info[i].nCheckSum, gpsinfo->info[i].strData);
		}
	}
}

int gps_info_peek(GPSInfo **data)
{
	BYTE bRes = DEF_FAIL;
	if(cQueue_isempty(&gps_info) == DEF_FAIL)
	{
		*data = &gps_info.data[gps_info.head];

		bRes = DEF_OK;
	}
	return bRes;
}

int gps_info_get(GPSInfo **data)
{
	BYTE bRes = DEF_FAIL;
	if(cQueue_isempty(&gps_info) == DEF_FAIL)
	{
		*data = &gps_info.data[gps_info.head];

		num_loop(gps_info.head, gps_info.tot_num);

		bRes = DEF_OK;
	}
	return bRes;
}

static UINT gps_check_checksum(int nCheckSum, char *strData)
{
	int i,nCount;
	char strBuf[GPSData_len+4] = {0};
	char *strPtr = NULL;
	char *strPtr2 = NULL;
	char strRMGMC[14][20];

	if(nCheckSum == 0)
		return DEF_FAIL;
	
	strcpy(strBuf, strData);
	//dprintf("strBuf %s\n",strBuf);
	strPtr = strstr(strBuf, "*");
	*strPtr = ',';
	strPtr = strBuf;
	//dprintf("strPtr %s\n",strPtr);
	for(i=0;strPtr;i++) // strsep func
	{
		if(i>13)
			break;
		//strcpy(enc_t->pstrRMGMC[cnt],strPtr);
		strPtr2 = strPtr;
		strPtr = strstr(strPtr, ",");
		//dprintf("%d strPtr %s\n",i,strPtr);
		//dprintf("%d strPtr2 %s\n",i,strPtr2);
		if(strPtr != NULL)
		{
			*strPtr = NULL;
			strPtr += 1;
			if(strlen(strPtr2))
			{			
				//dprintf("strPtr2 %d %s %d\n", i, strPtr2, strlen(strPtr2));
				strcpy(strRMGMC[i],strPtr2);
			}
			else
			{			
				strcpy(strRMGMC[i],"");
			}
		}
		else if(strPtr2 != NULL)
		{
			if(strlen(strPtr2))
			{			
				//dprintf("strPtr2 %d %s %d\n", i, strPtr2, strlen(strPtr2));
				strcpy(strRMGMC[i],strPtr2);
			}
			else
			{			
				strcpy(strRMGMC[i],"");
			}
			*strPtr2 = NULL;
		}
		else
		{
			strcpy(strRMGMC[i],"");
		}
	}
	

	//t tt;
	//r(tt=0;tt<14;tt++)
	//printf(" %d %s \n",tt, strRMGMC[tt]);
	
	if(nCheckSum == strtoul(strRMGMC[13], NULL, 16))
		return DEF_OK;

	
	return DEF_FAIL;

}

char tmp_strData[GPSData_len+4] = {0,};
int tmp_nCheckSum = 0;

int gps_info_put(char bBuf)
{
	char* teststr;
	int nRes = DEF_OK;
	if(bBuf == '$')
	{	// init, start, checksum start
		gps_info.nSelect = eGPS_None;
		
		gps_info.nCheckID = 0;
		gps_info.nCheckPos = 0;

		gps_info.nCheckSum = 0;
		gps_info.nCheckSumState = 1;

		gps_info.nEnd = 0;
	}
	else if(bBuf == '*')
	{	// checksum end
		gps_info.nCheckSumState = 0;
		if(gps_info.nSelect < GPSMnemonicCnt)
		{
			//dprintf("gps_info.nCheckSum %d\n",gps_info.nCheckSum);
			tmp_nCheckSum = gps_info.nCheckSum;
		}
		else
		{
			gps_info.nCheckID = 2;
			gprintf("error mnemonic id(%d) bBuf(%c,%d)\n", gps_info.nSelect, bBuf, bBuf);
		}
	}
	else if(bBuf == '\r' || bBuf == '\n')
	{	// data end
		if(gps_info.nSelect == eGPS_RMC)
		{
			tmp_strData[gps_info.nCheckPos] = NULL;
			//dprintf("data %s \n",tmp_strData);
			if(gps_check_checksum(tmp_nCheckSum, tmp_strData) == DEF_OK)
			{			
				//UartPutByte('_');
				//dprintf("\nchecksum pass!\n");	
				if(cQueue_isfull(&gps_info) == DEF_OK)
				{	// overflow
					gprintf("error overflow\n");
					nRes = DEF_FAIL;
				}
				num_loop(gps_info.tail, gps_info.tot_num);
				memset(&gps_info.data[gps_info.tail], 0, sizeof(gps_info.data[gps_info.tail])); 
				gps_info.data[gps_info.tail].info[gps_info.nSelect].nCheckSum = tmp_nCheckSum;
				strcpy(gps_info.data[gps_info.tail].info[gps_info.nSelect].strData, tmp_strData);
				//dprintf("GPS >> %s \n",gps_info.data[gps_info.tail].info[gps_info.nSelect].strData);
			}
			gps_info.nSelect = 0;			
		}
		return DEF_OK;
	}
	else
	{	// data(checksum xor, between '$' and '*')
		if(gps_info.nCheckSumState)
			gps_info.nCheckSum ^= bBuf;
		//dprintf("gps_info.nCheckSum2 %d\n",gps_info.nCheckSum);
	}

	if(gps_info.nCheckID == 0)
	{	// ID get($xxxxx) 6byte
		gps_info.strCheckID[gps_info.nCheckPos] = bBuf;
		gps_info.nCheckPos++;
		if(gps_info.nCheckPos == 6)
		{	// ID select
			gps_info.nCheckID = 1;
			if((teststr = strstr(gps_info.strCheckID, "RMC")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				// next tail, data init
				gpsConnect=1;
				gps_info.nSelect = eGPS_RMC;
			}
			else if((teststr = strstr(gps_info.strCheckID, "VTG")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_VTG;
			}
			else if((teststr = strstr(gps_info.strCheckID, "GGA")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_GGA;
			}
			else if((teststr = strstr(gps_info.strCheckID, "GSA")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_GSA;
			}
			else if((teststr = strstr(gps_info.strCheckID, "GLL")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_GLL;
			}
			else if((teststr = strstr(gps_info.strCheckID, "GSV")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_GSV;
			}
			else if((teststr = strstr(gps_info.strCheckID, "TXT")) != NULL)
			{
				//dprintf("\ngps[%s]\n",teststr);	
				gps_info.nSelect = eGPS_TXT;
			}
			else
			{
				gprintf("[%c][%c][%c][%c][%c][%c]\n", gps_info.strCheckID[0], gps_info.strCheckID[1], gps_info.strCheckID[2], gps_info.strCheckID[3], gps_info.strCheckID[4], gps_info.strCheckID[5]);
				gps_info.nCheckID = 2;
			}

			if(gps_info.nCheckID == 1)
			{
				if(gps_info.nSelect < GPSMnemonicCnt)
				{
					//dprintf("\nstrCheckID[%s]\n",gps_info.strCheckID);	
					memcpy(tmp_strData, gps_info.strCheckID, 6);
					tmp_nCheckSum = 0;
				}
				else
				{
					gps_info.nCheckID = 2;
					//gprintf("error mnemonic id(%d) bBuf(%c,%d)\n", gps_info.nSelect, bBuf, bBuf);
				}
			}
		}
	}
	else if(gps_info.nCheckID == 1)
	{
		if(gps_info.nSelect < GPSMnemonicCnt)
		{
			tmp_strData[gps_info.nCheckPos] = bBuf;
			gps_info.nCheckPos++;
			
		}
		else
		{
			gps_info.nCheckID = 2;
			gprintf("error mnemonic id(%d) bBuf(%c,%d)\n", gps_info.nSelect, bBuf, bBuf);
		}
	}
	else
	{
		gprintf("error mnemonic id(%d) bBuf(%c,%d)\n", gps_info.nSelect, bBuf, bBuf);
	}
	return nRes;
}

int gps_data_put(char bBuf)
{
	int nRes = DEF_OK;
	if(cQueue_isfull(&gps_data) == DEF_OK)
	{	// overflow
		gprintf("error overflow\n");
		nRes = DEF_FAIL;
	}
	gps_data.buffer[gps_data.tail] = bBuf;
	num_loop(gps_data.tail, gps_data.tot_num);
	return nRes;
}

int gps_data_get(char *bBuf)
{
	int nRes = DEF_FAIL;
	if(cQueue_isempty(&gps_data) == DEF_FAIL)
	{
		*bBuf = gps_data.buffer[gps_data.head];
		num_loop(gps_data.head, gps_data.tot_num);
		nRes = DEF_OK;
	}
	return nRes;
}

static void gpsrx_isr(void)
{
	volatile BYTE bIrqId, bBuf;
	bIrqId = U1II & 0xF;				// ID register must be read simultaneously. CAUTION
	while(bIrqId!=1){					// Read until interrupt pending bit is cleared
		switch(bIrqId){
			case 0x4:					// Rx irq
			case 0xC:					// Time out
				while(U1LS_DRDY)
				{
					bBuf = U1RB;
//					UartPutByte(bBuf);
					gps_data_put(bBuf);
				}
				break;
			default:
				break;
		}
		bIrqId = U1II & 0xF;
	}
}

static void gps_data_init(void)
{
	memset(&gps_data, 0, sizeof(gps_data));
	gps_data.tot_num = GPSBuffer_len;
}

static void gps_info_init(void)
{
	memset(&gps_info, 0, sizeof(gps_info));
	gps_info.tot_num = GPSQueue_cnt;
}

static void gpsrx_init(void)
{
	GpioOutDir(GPIO_GPS);

//	ViewInit();

	IntAdd(IRQ_VIEW, (void*)gpsrx_isr, 0);
}

static UINT gps_info_RMGMC_parser(GPSmnemonic *mnem)
{
	int i, nCount;
	char strBuf[100] = {0};
	char *strPtr;
	char *strRMGMC[20];

	if(mnem->nCheckSum == 0)
		return DEF_FAIL;

	strcpy(strBuf, mnem->strData);
	strPtr = strstr(strBuf, "*");
	*strPtr = ',';
	strPtr = strBuf;
	for(i=0;strPtr;i++)	// strsep func
	{
		strRMGMC[i] = strPtr;
		strPtr = strstr(strPtr, ",");
		if(strPtr != NULL)
		{
			*strPtr = NULL;
			strPtr += 1;
		}
	}
	// get time, get speed
	if(mnem->nCheckSum == strtoul(strRMGMC[13], NULL, 16))
	{	// CheckSum OK?
		if(strcmp(strRMGMC[2], "A") == 0 && (strcmp(strRMGMC[12], "A") == 0 || strcmp(strRMGMC[12], "D") == 0))
		{	// Status(2) == A(valid position) And Mode indicator(12) == A(Autonomous || D(Differential))
			if(get_gpstime == 1)
			{
				if(strlen(strRMGMC[1]) && strlen(strRMGMC[9]))
				{	// datetime set : UTC hhmmss.ss(1) And UTC ddmmyy(9)
					get_gpstime = 0;
					UINT nYear, nMonth, nDay, nHour, nMin, nSec, nResCNT;
					nResCNT = sscanf(strRMGMC[1], "%02d%02d%02d", &nHour, &nMin, &nSec);
					if(nResCNT == 3)
					{
						nResCNT = sscanf(strRMGMC[9], "%02d%02d%02d", &nDay, &nMonth, &nYear);
						if(nResCNT == 3)
						{						
							if(set_devicetime(TimeZone_GMT, nYear+2000, nMonth, nDay, nHour, nMin, nSec) == DEF_FAIL)
							{
								get_gpstime = 1;	// 실패 / 재시도
							}
						}
						else
						{
							get_gpstime = 1;	// 실패 / 재시도
						}
					}
					else
					{
						get_gpstime = 1;	// 실패 / 재시도
					}
				}
				else
				{	// 시간을 못얻어온 경우 다시 시도
					get_gpstime = 1;
				}
			}
			if(strlen(strRMGMC[7]))
			{	// speed set : 000.0 ~ 999.9knots
				float fSpeed ;//= floor(strtof(strRMGMC[7], NULL) * 1.852);	// 1kn == 1.852Km/h
//				if(gptMsgShare.gpslock == 0)
//				{
//					gptMsgShare.gpslock = 1;
					gptMsgShare.gpsCONN = eGPS_RUN;
					gptMsgShare.gpsKMH = fSpeed;
//					gptMsgShare.gpslock = 0;
//				}
			}
			gptMsgShare.gpsCONN = eGPS_RUN;
		}
		else
		{
//			if(gptMsgShare.gpslock == 0)
//			{
//				gptMsgShare.gpslock = 1;
				gptMsgShare.gpsCONN = eGPS_CONN_WAIT;		// GPS conn / not data
				gptMsgShare.gpsKMH = 0.0;
//				gptMsgShare.gpslock = 0;
//			}
			return DEF_FAIL;
		}
	}
	else
	{
		gprintf("ERROR : checksum(%X), (%X)\n", mnem->nCheckSum, strtoul(strRMGMC[13], NULL, 16));
		return DEF_FAIL;
	}
	return DEF_OK;
}

void gpsTask(void* pvParameters)
{
	UINT bCheckCount = 0, bCheckGPSnew = 0;
	UINT bGPS_OK = 0;
	char cBuf;
	GPSInfo *data;

	// init
	gpsrx_init();
	gps_data_init();
	gps_info_init();

//	vTaskDelay(200);
	
	gpsrx_start();

	for(;;)
	{
#ifdef __CBB__
#ifdef __DEVICE_SD__

		switch(cbbpower_get_mode())
		{
			case eCBB_Drive:
				if(gpsrx_state() == DEF_OFF)
					gpsrx_start();
				break;
			case eCBB_Off:
				gpsrx_stop();
				vTaskDelete(NULL);
				break;
		}
#endif		
#endif
		if(gps_data_get(&cBuf) == DEF_OK)
		{
			if(cBuf != NULL)
			{
				gps_info_put(cBuf);
				bCheckCount = 0;
				bCheckGPSnew = 0;
			}
		}
		//else
		//{
		//	vTaskDelay(1);//vTaskDelay(1);
			bCheckCount++;
		//}

		// time, speed get
		if(gps_info_peek(&data) == DEF_OK)
		{
//#ifdef ENX_GPSRX_DEBUG
//		if(gps_info_get(&data) == DEF_OK)
//			gps_info_print(data);
//#endif
			if(gps_info_RMGMC_parser(&(data->info[eGPS_RMC])) == DEF_OK)
			{
				if(bCheckGPSnew < 10)
					bCheckGPSnew++;
			}
		}
		else if(bCheckCount == 20)	// 10 == 1sec
		{	// Can't get GPS data. speed init(0.0)
			// GPS 정보를 2초간 받지 못하면, disconn 된 것으로 판단한다.
//			if(gptMsgShare.gpslock == 0)
//			{
//				gptMsgShare.gpslock = 1;
				gptMsgShare.gpsCONN = eGPS_DISCONN;	// GPS disconn
				gptMsgShare.gpsKMH = 0.0;
//				gptMsgShare.gpslock = 0;
//			}
			bCheckCount = 0;
			bCheckGPSnew = 0;
			bGPS_OK = 0;
		}

		//printf("bGPS_OK(%d) / GPSnew(%d) / GPSold(%d)\n", bGPS_OK, bCheckGPSnew, bCheckGPSold);

		if(bCheckGPSnew == 4)
		{
			if(bGPS_OK == 0)
			{
				//G726Playlist_put(eG726_GPSCONN);
				bGPS_OK = 1; 
			}
		}
		vTaskDelay(1);
	}
}
