//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		stdio.c
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150725		ygkim		merged similar versions of string functions @ ISP & IP's
// -----------------------------------------------------------------------------
#include <string.h>			// for strlen
#include "dev.h"
#include "dev_inline.h"

#include "msg.h"
#include "vsnprintf.h"

#define PRINTFBUF_SIZE	1024

//******************************************************************************
// 4. Console I/O
//------------------------------------------------------------------------------
#if 1
int sprintf(char* str, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int len = vsnprintf(str, 0xffffffff, format, args);
	va_end(args);

	return len;
}
#endif

#if 1
int snprintf(char* str, size_t n, const char* format, ...)
{
	va_list args;
	va_start(args, format);
	int len = vsnprintf(str, n, format, args);
	va_end(args);

	return len;
}
#endif

#ifdef JTAG_DEBUG_PRINT
_NOINS static void __ecm_jprintf(char* buf, int len)
{
	volatile UINT new_base, *base = NULL;
	int fir_size = 0, sec_size = 0;

	if (CPU_ID == 0) {
		base = &(gptMsgShare._JTAG_PRINT0_POINT);
		sec_size = (*base + len) - (JTAG_PRINT0_END - 1);
	} else {
		base = &(gptMsgShare._JTAG_PRINT1_POINT);
		sec_size = (*base + len) - (JTAG_PRINT1_END - 1);
	}

	if (sec_size >= 0) {
		fir_size = len - sec_size;
		if(fir_size > 0) {
			DmaMemCpy_ip((BYTE *)*base, (BYTE *)buf, fir_size);
		}

		if (CPU_ID == 0) {
			new_base = gptMsgShare._JTAG_PRINT0_START;
		} else {
			new_base = gptMsgShare._JTAG_PRINT1_START;
		}

		if (sec_size > 0) {
			DmaMemCpy_ip((BYTE *)new_base, (BYTE *)(buf + fir_size), sec_size);
			new_base += sec_size;
		}
		*base = new_base;
	} else {
		DmaMemCpy_ip((BYTE *)*base, (BYTE *)buf, len);
		*base += len;
	}
}

_NOINS int ecm_jprintf(const char *format, ...)
{
	char buf[PRINTFBUF_SIZE];
	va_list args;
	va_start(args, format);
	int len = vsnprintf(buf, PRINTFBUF_SIZE, format, args);
	va_end(args);

	__ecm_jprintf(buf, len);

	return len;
}
#endif

_NOINS static int __ecm_uprintf(char* buf, int len)
{
#define PC_STX 0x02
#define PC_CMD_STR 0xB0
#define PC_ETX 0x03

	if (len > 255) {
		Uart0_Tx(PC_STX);
		Uart0_Tx(PC_CMD_STR);
		Uart0_Tx(20+8);
		Uart0_Str("printf overflow!(");
		Uart0_Hex(len);
		Uart0_Tx(')');
		Uart0_Tx('\r');
		Uart0_Tx('\n');
		Uart0_Tx(PC_ETX);
		len = 255;
	}

	Uart0_Tx(PC_STX);
	Uart0_Tx(PC_CMD_STR);
	Uart0_Tx(len);
	while (*buf) {
		Uart0_Tx(*buf++);
	}
	Uart0_Tx(PC_ETX);

	return len;
}

_NOINS int ecm_uprintf(const char *format, ...)
{
	char buf[PRINTFBUF_SIZE];
	va_list args;
	va_start(args, format);
	int len = vsnprintf(buf, PRINTFBUF_SIZE, format, args);
	va_end(args);

	__ecm_uprintf(buf, len);

	return len;
}

_NOINS int uart_printf(const char *format, ...)
{
	char buf[PRINTFBUF_SIZE], *pbuf;
	va_list args;
	va_start(args, format);
  	int len = vsnprintf(buf, PRINTFBUF_SIZE, format, args);
	va_end(args);

	if (len >= PRINTFBUF_SIZE) {
		Uart0_Str("printf overflow!(");
		Uart0_Hex(len);
		Uart0_Tx(')');
		Uart0_Tx('\r');
		Uart0_Tx('\n');
	}

	pbuf = buf;
	while (*pbuf) {
		Uart0_Tx(*pbuf++);
	}

  	return len;  
}

_NOINS int printf(const char *format, ...)
{
	char buf[PRINTFBUF_SIZE];
	va_list args;
	va_start(args, format);
	int len = vsnprintf(buf, PRINTFBUF_SIZE, format, args);
	va_end(args);

#ifdef JTAG_DEBUG_PRINT
	__ecm_jprintf(buf, len);
#else
	__ecm_uprintf(buf, len);
#endif

	return len;
}
