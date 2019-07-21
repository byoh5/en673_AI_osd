/* **************************************************************************
 File Name	: 	dev_uart.c
 Description:	EN673 - UART device driver
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Date		:	14. 6. 18
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"

//-------------------------------------------------------------------------------------------------
// Utility
void ISPM UartTxStr(const char *apcStr)
{	// String out (Guarantee Tx)
	while(*apcStr){													// Check 'NULL' character
		//if(*apcStr == '\n')		UartTxByte('\r');					// Move cursor to left
		UartTxByte(*apcStr++);										// Put 1 byte
	}
}


//*************************************************************************************************
// Isr
//-------------------------------------------------------------------------------------------------

// Uart CH0
TUartQue gtUartQue;

#ifdef ECM_REMOTE
volatile int jtagTxFlag = -1;
#endif

volatile int giTxOn = 0;


void UartRstRxQue(void)
{
	gtUartQue.wRxLen = 0;
	gtUartQue.pbRxHead = &gtUartQue.bRxQue[0];
	gtUartQue.pbRxTail = &gtUartQue.bRxQue[0];
}

void UartRstTxQue(void)
{
	gtUartQue.wTxLen = 0;
	gtUartQue.pbTxHead = &gtUartQue.bTxQue[0];
	gtUartQue.pbTxTail = &gtUartQue.bTxQue[0];
}

void UartRstQue(void)
{
	UartRstRxQue();
	UartRstTxQue();
}

#if 0
BYTE ISPM UartPutByte(BYTE abChar)
{
	BYTE		bErr;

	//DI();		// 없어도 동작함

	if(gtUartQue.wTxLen < UART_BUF_SIZE){
		//gtUartQue.wTxLen++;
		UART0_TX_IRQ_EN = 0;
		giTxOn = 1;
		*gtUartQue.pbTxHead++ = abChar;
		if( gtUartQue.pbTxHead == &gtUartQue.bTxQue[UART_BUF_SIZE]){
			gtUartQue.pbTxHead  = &gtUartQue.bTxQue[0];
		}
		gtUartQue.wTxLen++;
		giTxOn = 0;
		/*if(gtUartQue.wTxLen == 1)*/	UART0_TX_IRQ_EN = 1;
		bErr = UART_NO_QUE_ERR;
	}
	else{
		bErr = UART_TX_QUE_FULL;
	}

	//EI();		// 없어도 동작함

	return (bErr);
}
#endif

#if STR_UART_DATA == 1
volatile UINT gnIsrUartTx  = 0;
volatile UINT gnIsrUartTxF = 0;
volatile UINT gnIsrUartRx  = 0;
volatile UINT gnIsrUartRxF = 0;
#endif

void ISPM UartTxByte(BYTE abByte)
{
#if 0
	volatile UINT vUartTxByte;
	while(1){
		//vUartTxByte	= UartIsFull();
		//if(!vUartTxByte) break;
		vUartTxByte = (gtUartQue.wTxLen < UART_BUF_SIZE);
		if(vUartTxByte) break;
	}

	UartPutByte(abByte);
#else

	SUD(
		if(gtUartQue.wTxLen >= UART_BUF_SIZE) {
			gnIsrUartTxF++;
			//GpioSetHi(GPIO_TEST2);
		}
		else {
			//GpioSetLo(GPIO_TEST2);
		}
	)
	
#if 0
	while(gtUartQue.wTxLen >= UART_BUF_SIZE);		// Wait until the Tx operate. If DI() state, The result in an infinite loop.
#else
	if(gtUartQue.wTxLen >= UART_BUF_SIZE) return;	// The Tx data is lost.
#endif

	UART0_TX_IRQ_EN = 0;
	giTxOn = 1;
	*gtUartQue.pbTxHead++ = abByte;
	if( gtUartQue.pbTxHead == &gtUartQue.bTxQue[UART_BUF_SIZE]){
		gtUartQue.pbTxHead  = &gtUartQue.bTxQue[0];
	}
	gtUartQue.wTxLen++;
	giTxOn = 0;
	/*if(gtUartQue.wTxLen == 1)*/	UART0_TX_IRQ_EN = 1;
#endif
}

void ISPM0 IsrUart(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	volatile BYTE bUART_RX_IRQ = UART0_RX_IRQ;
	volatile BYTE bUART_TX_IRQ = UART0_TX_IRQ;

	//static BYTE bRx[2] = { 0, 0 };

	while(bUART_RX_IRQ)
	{
		UART0_RX_IRQ_CLR = 1;

		volatile BYTE bBuf;

		do {
			if(UART0_RX_EMPTY) break;
			else {
				bBuf = UART0_RX_DAT;
			}

			SUD(gnIsrUartRx++;)
			
			//bBuf = UART0_RX_DAT;

			//if( bRx[1] == PC_STX && bRx[0] == PC_CMD_DOWN && bBuf == PC_ETX && (_reset_s&FLASH_ADDR_MASK) ) { gnDownOn = 1; }
			//bRx[1] = bRx[0]; bRx[0] = bBuf;	

			#if 0
				QueIn(gtUartQue, UART_BUF_SIZE, &bBuf, 1,
					SUD(gnIsrUartRxF++;)
					//TxStr("UART RX FULL", bDat,4);
				)
			#else
				if(gtUartQue.wRxLen < (UART_BUF_SIZE-1)) {
					#if STR_QUE_DATA == 1
						extern /*volatile*/ UINT gnQueIn0;
						extern /*volatile*/ UINT gnQueIn1;
						extern /*volatile*/ UINT gnQueIn2;
						gnQueIn2 = (gnQueIn2<<8) | ((gnQueIn1>>24)&0xFF);
						gnQueIn1 = (gnQueIn1<<8) | ((gnQueIn0>>24)&0xFF);
						gnQueIn0 = (gnQueIn0<<8) | bBuf;
					#endif

					*gtUartQue.pbRxHead++ = bBuf;
					if(gtUartQue.pbRxHead == (gtUartQue.bRxQue + UART_BUF_SIZE)) {
						gtUartQue.pbRxHead = gtUartQue.bRxQue;
					}
					gtUartQue.wRxLen++;
				}
				else { 
					SUD(gnIsrUartRxF++;)
					//TxStr("UART RX FULL", bDat,4);
				}
			#endif

		} while ( 1/*UART0_RX_EMPTY == 0*/ );

		bUART_RX_IRQ = UART0_RX_IRQ;
	}
#ifdef ECM_REMOTE
		BYTE J_ShellCmdBuf[68] = {0};
		int i = 0;
#endif	
	while(bUART_TX_IRQ)
	{
		SUD(gnIsrUartTx++;)
		
		//UART_TX_IRQ_CLR = 1;

		/*if(gnDownOn) {
			UART0_TX_IRQ_EN = 0;
			gtUartQue.wTxLen = 0;
			gtUartQue.pbTxTail = gtUartQue.pbTxHead;
			break;
		}*/

		if(giTxOn) { UART0_TX_IRQ_EN = 0; break; }
		else {
#ifdef ECM_REMOTE
				switch (jtagTxFlag)
				{
					case 0x00 : 
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_TX, 0, 12);
						break;
					case 0x01 :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_ISP, 0, 12);
						break;
					case 0x02 :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_Para, 0, 12);
						break;
					case 0x03 :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_Gamma, 0, 12);
						break;
					case 0x04 :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_Color, 0, 12);
						break;
					case 0x05 :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_Viewer, 0, 36);
						break;
					default :
						DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_TX, 0, 60);
						break;
				}
#endif
			do {
				if(gtUartQue.wTxLen == 0) {
					UART0_TX_IRQ_EN = 0;
					break;
				}
				else {
					//SUD(gnIsrUartTx++;)

					/*if(gnDownOn) {
						gtUartQue.pbTxTail++;
					}
					else*/ {
						UART0_TX_DAT = *gtUartQue.pbTxTail++;
#ifdef ECM_REMOTE
						J_ShellCmdBuf[i++] = UART0_TX_DAT;
//						printf("0x%02x\r\n", UART0_TX_DAT);
#endif					
					}

					if(	gtUartQue.pbTxTail == &gtUartQue.bTxQue[UART_BUF_SIZE]) {
						gtUartQue.pbTxTail  = &gtUartQue.bTxQue[0];
					}
					gtUartQue.wTxLen--;
				}
			} while( UART0_TX_FULL == 0 );
		}
#ifdef ECM_REMOTE
		switch (jtagTxFlag)
		{
			case 0x00 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_TX, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
			case 0x01 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_ISP, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
			case 0x02 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_Para, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
			case 0x03 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_Gamma, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
			case 0x04 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_Color, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
			case 0x05 :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_Viewer, (BYTE *)J_ShellCmdBuf, 36);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+36));
				break;
			default :
				DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_TX, (BYTE *)J_ShellCmdBuf, 12);
				invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
				break;
		}
		jtagTxFlag = -1;
		i = 0;
#endif

		bUART_TX_IRQ = UART0_TX_IRQ;
	}
}

// 20151110 - WHL : Uart CH1
TUartQue gtUartQue1;

volatile int giTxOn1 = 0;


void UartRstRxQue1(void)
{
	gtUartQue1.wRxLen = 0;
	gtUartQue1.pbRxHead = &gtUartQue1.bRxQue[0];
	gtUartQue1.pbRxTail = &gtUartQue1.bRxQue[0];
}

void UartRstTxQue1(void)
{
	gtUartQue1.wTxLen = 0;
	gtUartQue1.pbTxHead = &gtUartQue1.bTxQue[0];
	gtUartQue1.pbTxTail = &gtUartQue1.bTxQue[0];
}

void UartRstQue1(void)
{
	UartRstRxQue1();
	UartRstTxQue1();
}

#if STR_UART_DATA == 1
volatile UINT gnIsrUartTx1  = 0;
volatile UINT gnIsrUartTxF1 = 0;
volatile UINT gnIsrUartRx1  = 0;
volatile UINT gnIsrUartRxF1 = 0;
#endif

void ISPM UartTxByte1(BYTE abByte)
{
	SUD(
		if(gtUartQue1.wTxLen >= UART_BUF_SIZE) {
			gnIsrUartTxF1++;
			//GpioSetHi(GPIO_TEST2);
		}
		else {
			//GpioSetLo(GPIO_TEST2);
		}
	)
	
#if 1
	while(gtUartQue1.wTxLen >= UART_BUF_SIZE);		// Wait until the Tx operate. If DI() state, The result in an infinite loop.
#else
	if(gtUartQue1.wTxLen >= UART_BUF_SIZE) return;	// The Tx data is lost.
#endif

	UART1_TX_IRQ_EN = 0;
	giTxOn1 = 1;
	*gtUartQue1.pbTxHead++ = abByte;
	if( gtUartQue1.pbTxHead == &gtUartQue1.bTxQue[UART_BUF_SIZE]){
		gtUartQue1.pbTxHead  = &gtUartQue1.bTxQue[0];
	}
	gtUartQue1.wTxLen++;
	giTxOn1 = 0;
	/*if(gtUartQue.wTxLen == 1)*/	UART1_TX_IRQ_EN = 1;
}

void ISPM0 IsrUart1(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	volatile BYTE bUART1_RX_IRQ = UART1_RX_IRQ;
	volatile BYTE bUART1_TX_IRQ = UART1_TX_IRQ;

	//static BYTE bRx[2] = { 0, 0 };

	while(bUART1_RX_IRQ)
	{
		UART1_RX_IRQ_CLR = 1;

		volatile BYTE bBuf;

		do {
			if(UART1_RX_EMPTY) break;
			else {
				bBuf = UART1_RX_DAT;
			}

			SUD(gnIsrUartRx1++;)
			
			#if 0
			QueIn(gtUartQue1, UART_BUF_SIZE, &bBuf, 1,
				SUD(gnIsrUartRxF1++;)
//					TxStr("UART RX FULL", bBuf,4);
			)
			#else
				if(gtUartQue1.wRxLen < (UART_BUF_SIZE-1)) {
					#if STR_QUE_DATA == 1
						extern /*volatile*/ UINT gnQueIn0;
						extern /*volatile*/ UINT gnQueIn1;
						extern /*volatile*/ UINT gnQueIn2;
						gnQueIn2 = (gnQueIn2<<8) | ((gnQueIn1>>24)&0xFF);
						gnQueIn1 = (gnQueIn1<<8) | ((gnQueIn0>>24)&0xFF);
						gnQueIn0 = (gnQueIn0<<8) | bBuf;
					#endif

					*gtUartQue1.pbRxHead++ = bBuf;
					if(gtUartQue1.pbRxHead == (gtUartQue1.bRxQue + UART_BUF_SIZE)) {
						gtUartQue1.pbRxHead = gtUartQue1.bRxQue;
					}
					gtUartQue1.wRxLen++;
				}
				else { 
					SUD(gnIsrUartRxF++;)
					//TxStr("UART RX FULL", bDat,4);
				}
			#endif

		} while ( 1/*UART0_RX_EMPTY == 0*/ );

		bUART1_RX_IRQ = UART1_RX_IRQ;
	}
	
	while(bUART1_TX_IRQ)
	{
		SUD(gnIsrUartTx1++;)
		
		if(giTxOn1) { UART1_TX_IRQ_EN = 0; break; }
		else {

			do {
				if(gtUartQue1.wTxLen == 0) {
					UART1_TX_IRQ_EN = 0;
					break;
				}
				else {
					UART1_TX_DAT = *gtUartQue1.pbTxTail++;

					if(	gtUartQue1.pbTxTail == &gtUartQue1.bTxQue[UART_BUF_SIZE]) {
						gtUartQue1.pbTxTail  = &gtUartQue1.bTxQue[0];
					}
					gtUartQue1.wTxLen--;
				}
			} while( UART1_TX_FULL == 0 );
		}

		bUART1_TX_IRQ = UART1_TX_IRQ;
	}
}

//*************************************************************************************************
// JTAG UART
//-------------------------------------------------------------------------------------------------
//
#ifdef ECM_REMOTE
void Isr_JUART(void *dummy)
{
	//printf("func : %s \t line : %d\r\n", __FUNCTION__, __LINE__);

	BYTE J_ShellCmdBuf[12] = {0};
	int size;
	int i;
	
	DmaMemCpy_ip((BYTE *)J_ShellCmdBuf, (BYTE *)gptMsgShare._ECM_REMOTE_RX, 12);
	invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+12));
	DmaMemSet_ip((BYTE *)gptMsgShare._ECM_REMOTE_RX, 0, 60);

	jtagTxFlag = J_ShellCmdBuf[0];
	size = J_ShellCmdBuf[1];
	
	//printf("size : 0x%02x\r\n", *(J_ShellCmdBuf));
	
	for (i = 0; i < size - 1; i++)
	{
		if(gtUartQue.wRxLen < (UART_BUF_SIZE-1)) {
			*gtUartQue.pbRxHead++ = *(J_ShellCmdBuf+2+i);
			if(gtUartQue.pbRxHead == (gtUartQue.bRxQue + UART_BUF_SIZE)) {
				gtUartQue.pbRxHead = gtUartQue.bRxQue;
			}
			//printf("0x%02x\r\n", *(J_ShellCmdBuf+2+i));
			gtUartQue.wRxLen++;
		}
	}

	UNUSED(dummy);
}
#endif


/* EOF */

