/***************************************************************************
 File Name	: 	secu.c
 Description:	EN673 - AES128 Encrytor
 Designer	:	YoungKyu, Jang
 Date		:	15. 6. 16 
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"

void Secu_Init(void)
{
	SECU_AESKEY0 = 0x12345678;
	SECU_AESKEY1 = 0x12345678;
	SECU_AESKEY2 = 0x12345678;
	SECU_AESKEY3 = 0x12345678;
}

void AesEncryt(UINT * Src, UINT * Dst, UINT BkCnt)
{
	DmaMemCpy((BYTE*) SECU_BASE, (BYTE*) Src, (BkCnt<<4));
	DmaMemCpy((BYTE*) Dst, (BYTE*) SECU_BASE, (BkCnt<<4));
}