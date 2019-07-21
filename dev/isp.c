/***************************************************************************
 File Name	: 	isp.c
 Description:	EN673 - ISP device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 25
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"

#if 0
volatile UINT FrmSkip = 0;

void IspInit(void)
{
	SYS_ISP_RST	= 1;
}
void IspIsr(void)
{
	UINT* pIsp = (UINT*)0xf5001900;
	*pIsp = *pIsp|0x00000001;
	if(FrmSkip>2)
	{
		H264_ENCSTART = 1;
	}
	if(FrmSkip>3)
	{
		H264_VLCREADY = 1;
		H264_VLCSTART = 1;
	}
	
	
	FrmSkip++;
}
#endif