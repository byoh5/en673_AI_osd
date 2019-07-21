/***************************************************************************
 File Name	: 	dma.h
 Description:	EN673 - DMA device driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 25
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "msg.h"

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#endif

#define SWAP_BYTES_IN_WORD(w)	(((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
#define DMA_BLOCK				(1024*2)

//*************************************************************************************************
// Phy
//-------------------------------------------------------------------------------------------------
//
#ifndef _INLINE	// for inline
_NOINS void DmaEi(void)
{	// Interrupt enable
	DMA_IRQ_EN = 1;
}

_NOINS void DmaDi(void)
{	// Interrupt disable
	DMA_IRQ_EN = 0;
}

_NOINS void DmaMemCpy_isr(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 0;
	DMA_DST = (UINT)apbDst;
	DMA_SRC = (UINT)apbSrc;
	DMA_LEN = (UINT)anNum;
	DMA_GO  = 1;
	while(DMA_GO);
}

_NOINS void DmaMemCpy_isr_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 0;
	DMA_DST = (UINT)apbDst;
	DMA_SRC = (UINT)apbSrc;
	DMA_LEN = (UINT)anNum;
	DMA_GO  = 1;
}

_NOINS void DmaMemSet_isr(BYTE *apbDst, BYTE abVal, UINT anNum)
{
	while(DMA_GO);
	DMA_MODE= 1;
	DMA_DST = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_VALUE = abVal;
	DMA_GO  = 1;
	while(DMA_GO);
}

_NOINS void DmaDChkSum_isr_async(BYTE *apbDst, UINT anNum)
{
	while(DMA_GO);
	DMA_SRC = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_MODE= 2;
	DMA_CHKSUM_CLR = 1;
	DMA_GO  = 1;
}

_NOINS void DmaDChkSum_isr(BYTE *apbDst, UINT anNum, WORD *apnChkSum)
{
	while(DMA_GO);
	DMA_SRC = (UINT)apbDst;
	DMA_LEN = (UINT)anNum;
	DMA_MODE= 2;
	DMA_CHKSUM_CLR = 1;
	DMA_GO  = 1;
	while(DMA_GO);
	asm volatile("l.nop");
	*apnChkSum = SWAP_BYTES_IN_WORD(DMA_CHKSUM);	// Checksum(16 bit accumulator)
}
#endif

//*************************************************************************************************
// Link
//-------------------------------------------------------------------------------------------------
void DmaMemCpy_ip_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	UINT dstp = (UINT)apbDst;
	UINT srcp = (UINT)apbSrc;
	UINT lenp = anNum;

	if((lenp>DMA_BLOCK) && ((dstp&0x3)||(srcp&0x3)))
	{
		for(lenp;lenp>DMA_BLOCK;lenp-=DMA_BLOCK)
		{
			DmaMemCpy_isr((BYTE *)dstp, (BYTE *)srcp, DMA_BLOCK);
			dstp += DMA_BLOCK;
			srcp += DMA_BLOCK;
		}

		if(lenp)
			DmaMemCpy_isr_async((BYTE *)dstp, (BYTE *)srcp, lenp);
	}
	else
	{
		DmaMemCpy_isr_async((BYTE *)dstp, (BYTE *)srcp, lenp);
	}

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif
}

void DmaMemCpy_ip(BYTE *apbDst, BYTE *apbSrc, UINT anNum)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	UINT dstp = (UINT)apbDst;
	UINT srcp = (UINT)apbSrc;
	UINT lenp = anNum;

	if((lenp>DMA_BLOCK) && ((dstp&0x3)||(srcp&0x3)))
	{
		for(lenp;lenp>DMA_BLOCK;lenp-=DMA_BLOCK)
		{
			DmaMemCpy_isr((BYTE *)dstp, (BYTE *)srcp, DMA_BLOCK);
			dstp += DMA_BLOCK;
			srcp += DMA_BLOCK;
		}

		if(lenp)
			DmaMemCpy_isr_async((BYTE *)dstp, (BYTE *)srcp, lenp);
	}
	else
	{
		DmaMemCpy_isr_async((BYTE *)dstp, (BYTE *)srcp, lenp);
	}

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif

	if(lenp)
		while(DMA_GO);
}

#ifndef DEF_BOOT
void DmaEthTxCpy_async(BYTE *apbSrc, UINT anNum)
{
	while(CS_ETHERNET) {
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}
	while(ETH_TX_FULL) {
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}

#if (ETH_MAC_PAUSE==1)
	while (gptMsgNetworkRXShared.tx_pause) {
#ifdef DEF_CPU1
		portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
		CRITICAL_BEGIN;
#endif
		gptMsgNetworkRXShared.tx_pause--;
		if (gptMsgNetworkRXShared.tx_pause == 0) {
#ifdef DEF_CPU1
			portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
			CRITICAL_END;
#endif
			break;
		}
		if (gptMsgNetworkRXShared.tx_go == 1) {
			gptMsgNetworkRXShared.tx_pause = 0;
#ifdef DEF_CPU1
			portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
			CRITICAL_END;
#endif
			break;
		}
#ifdef DEF_CPU1
		portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
		CRITICAL_END;
#endif
	}
#endif

#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	DmaMemCpy_isr_async((BYTE *)ETHTX_BASE, apbSrc, anNum);
#if ENX_NETWORK_Debug
	gptMsgDebug.ETH_packet_tx += anNum;
#endif

	if(gptMsgDebug.ETH_TX_CHECK == 1)
		Uart0_Tx('O');

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif

	CS_ETHERNET = 0;
}

void DmaEthTxCpy(BYTE *apbSrc, UINT anNum)
{
	while(CS_ETHERNET) {
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}
	while(ETH_TX_FULL) {
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}

#if (ETH_MAC_PAUSE==1)
	while (gptMsgNetworkRXShared.tx_pause) {
#ifdef DEF_CPU1
		portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
		CRITICAL_BEGIN;
#endif
		gptMsgNetworkRXShared.tx_pause--;
		if (gptMsgNetworkRXShared.tx_pause == 0) {
#ifdef DEF_CPU1
			portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
			CRITICAL_END;
#endif
			break;
		}
		if (gptMsgNetworkRXShared.tx_go == 1) {
			gptMsgNetworkRXShared.tx_pause = 0;
#ifdef DEF_CPU1
			portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
			CRITICAL_END;
#endif
			break;
		}
#ifdef DEF_CPU1
		portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
		CRITICAL_END;
#endif
	}
#endif

#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	DmaMemCpy_isr_async((BYTE *)ETHTX_BASE, apbSrc, anNum);
#if ENX_NETWORK_Debug
	gptMsgDebug.ETH_packet_tx += anNum;
#endif

	if(gptMsgDebug.ETH_TX_CHECK == 1)
		Uart0_Tx('O');

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif

	while(DMA_GO) {
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
		NOP();NOP();NOP();NOP();NOP();NOP();NOP();NOP();
	}

	CS_ETHERNET = 0;
}
#endif

//-------------------------------------------------------------------------------------------------
// Memory set with value (== memset())
void DmaMemSet_ip(BYTE *apbDst, BYTE abVal, UINT anNum)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	DmaMemSet_isr(apbDst, abVal, anNum);

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif
}

void DmaDChkSum_ip(BYTE *apbDst, UINT anNum, WORD *apnChkSum)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	DmaDChkSum_isr_async(apbDst, anNum);

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif

	while(DMA_GO);
	asm volatile("l.nop");
	*apnChkSum = SWAP_BYTES_IN_WORD(DMA_CHKSUM);	// Checksum(16 bit accumulator)
}

void DmaUChkSum_ip(BYTE *apbSrc, UINT anNum, WORD *apnChkSum)
{
#ifdef DEF_CPU1
	portENTER_CRITICAL();	// Critical section enter
#endif
#ifdef DEF_CPU0
	CRITICAL_BEGIN;
#endif

	if(apnChkSum != NULL)
	{
		UTIL_MODE = 1;
	}

	DmaMemCpy_isr((BYTE *)UTIL_BASE, apbSrc, anNum);

	if(apnChkSum != NULL)
	{
		*apnChkSum = SWAP_BYTES_IN_WORD(UTIL_CHKSUM);// Checksum(16 bit accumulator)
		UTIL_MODE = 0;
	}

#ifdef DEF_CPU1
	portEXIT_CRITICAL();	// Critical section exit
#endif
#ifdef DEF_CPU0
	CRITICAL_END;
#endif
}
/* EOF */
