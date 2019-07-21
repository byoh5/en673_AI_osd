// ==================================================================================
//	File Name			: en332s_main.c
//	Description			: EN334S register initailize
//	Date				: 2016.10.13
//	Created by			:
//	Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
//	Revision History	: 
// ==================================================================================
#include "../cpu/cpu_main.h"
#include "en332s_ini_table.h"
#include "enx_dev.h"

// ==================================================================================
// Initial Device
// ==================================================================================
void EN332S_Init(WORD DevAddr)
{
	BYTE IniCnt;
	for(IniCnt = 0; IniCnt < INI_Size ; IniCnt ++)
		ENX_I2Cw(DevAddr,EN332S_INI[IniCnt][0],EN332S_INI[IniCnt][1]);
}

// ==================================================================================
// OutModeCheck
// ==================================================================================
BYTE OutModeCheck(WORD DevAddr) 
{
	BYTE sw0, sw1, sw2, sw3;
	BYTE OutMode = 0;
	
	switch(DevAddr)
	{
		case ENX_DEV0_ADDR :	sw0 = ~GpioGetPin(GPIO_SW1);	OutMode = (sw0&0x1);	break;
		case ENX_DEV1_ADDR :	sw1 = ~GpioGetPin(GPIO_SW2);	OutMode = (sw1&0x1);	break;
		case ENX_DEV2_ADDR :	sw2 = ~GpioGetPin(GPIO_SW3);	OutMode = (sw2&0x1);	break;
		case ENX_DEV3_ADDR :	sw3 = ~GpioGetPin(GPIO_SW4);	OutMode = (sw3&0x1);	break;
	}
	
	return OutMode;
}

// ==================================================================================
// Auto input mode detector
// start -> read register -> input detect -> mode indication -> mode table write
// ==================================================================================
BYTE CableInCheck(WORD DevAddr) 
{
	BYTE DetFmt = 0;
	UINT InDetReg;
	UINT rNO_INPUT_B, rNO_INPUT2_B, rRSEL_RATE_B, rWL_FIX_B;

	ENX_I2Cr(DevAddr,0x302,&InDetReg);
		
	// Channel B cable detect	
	rNO_INPUT_B 	= 0x01&(InDetReg>>1);
	rNO_INPUT2_B 	= 0x01&(InDetReg>>0);
	rRSEL_RATE_B	= 0x03&(InDetReg>>4);
	rWL_FIX_B		= 0xff&(InDetReg>>8);

	if(!rNO_INPUT_B)	ENX_I2Cw(DevAddr,0x57,0x4001033);
	else				ENX_I2Cw(DevAddr,0x57,0x4001020);

	if((!rNO_INPUT2_B) && (rWL_FIX_B)){
		if(rRSEL_RATE_B == 0x0 && rWL_FIX_B == 0x80)			DetFmt = IN_HDSDI;
		else if(rRSEL_RATE_B == 0x2 && rWL_FIX_B == 0x40)		DetFmt = IN_3GSDI;
		else if(rRSEL_RATE_B == 0x1 && rWL_FIX_B == 0x20)		DetFmt = IN_EX270_HD;
		else if(rRSEL_RATE_B == 0x3 && rWL_FIX_B == 0x02)		DetFmt = IN_EX135_HD;
		else if(rRSEL_RATE_B == 0x0 && rWL_FIX_B == 0x90)		DetFmt = IN_EX1G5_4K;
		else if(rRSEL_RATE_B == 0x1 && rWL_FIX_B == 0x02)		DetFmt = IN_EX270_3G;}
	else	DetFmt = NO_INPUT;
	
	switch(DevAddr){
		case ENX_DEV0_ADDR :
			if(DetFmt==NO_INPUT)	GpioSetLo(GPIO07);
			else					GpioSetHi(GPIO07);
		break;
		case ENX_DEV1_ADDR :
			if(DetFmt==NO_INPUT)	GpioSetLo(GPIO08);
			else					GpioSetHi(GPIO08);
		break;
		case ENX_DEV2_ADDR :
			if(DetFmt==NO_INPUT)	GpioSetLo(GPIO09);
			else					GpioSetHi(GPIO09);
		break;
		case ENX_DEV3_ADDR :
			if(DetFmt==NO_INPUT)	GpioSetLo(GPIO10);
			else					GpioSetHi(GPIO10);
		break;
	}
	
	return DetFmt;

}


