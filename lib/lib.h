//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		lib.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:			DO NOT CHANGE _STDIO_H_. 
//					It should be the same as in <stdio.h> and 
//					this folder should be higher than <stdio.h> in library search paths
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151029		ygkim		merged similar versions of functions @ ISP & IP's
// -----------------------------------------------------------------------------
#ifndef _LIB_H_
#define _LIB_H_

#include "dev_types.h"
#include <string.h>

/*
 INDEX	:
		 1. Type definition
		 2. Basic library
		 3. String
		 4. Console I/O
		 5. Memory
		 6. Math
		 7. 
*/


//******************************************************************************
// 1. Type definition
//------------------------------------------------------------------------------
#define	LDECBITS(dec,msb,lsb)	(((dec) >> (lsb)) & (0xffffffff >> (32-((msb)-(lsb)+1))))
#define	LDECBIT(dec,bit)		(((dec) >> (bit)) & 0x01)

//------------------------------------------------------------------------------
// Macros
#define CEILOF4(value)			(((value+3)>>2)<<2)	// Ceiling a multiple of 4
#define	FLOOROF4(value)			(((value  )>>2)<<2)	// Flooring a muliple of 4

#ifndef max
#define max(a,b)				(((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)				(((a) < (b)) ? (a) : (b))
#endif

#define TO_DIGIT(c)				((c) - '0')
#define IS_DIGIT(c)     		((unsigned)TO_DIGIT(c) <= 9)
#define TO_NCHAR(n)      		((n) + '0') 		// numerical character
#define	TO_ACHAR(n)				((n) - 10 + 'a')	// alphabet character


//******************************************************************************
// 2. Basic library
//------------------------------------------------------------------------------
void call(uint addr);
//abort
void exit(int i);
void report(int value);

//------------------------------------------------------------------------------
// Random
//uint rand(void);
//srand

//------------------------------------------------------------------------------
// Search
//bsearch

//------------------------------------------------------------------------------
// Arithmetic
//abs
//labs

extern UINT floorSqrt(UINT x);

//------------------------------------------------------------------------------
// trace
//int printf_null(const char* format, ...);

#ifdef _DEBUG
#define	TRACE				printf
#else
#define	TRACE				//printf_null
#endif//_DEBUG

//------------------------------------------------------------------------------
// assert
void assert_failed(char* file, int line);

#ifdef _DEBUG
#define	ASSERT(exp)		if(!(exp)) assert_failed(__FILE__, __LINE__)
#else
#define ASSERT(exp)
#endif//_DEBUG

#define	FAIL(fmt, args...)	{ TRACE(fmt, ##args); ASSERT(0); }

//------------------------------------------------------------------------------
// throw
typedef enum { MSG=0, WARN=1, ERROR=2 } eThrowCode;

void throwex(eThrowCode code, char* msg, char* file, int line);

#ifdef	_DEBUG
#define	THROW(code,msg)	throwex(code, msg, __FILE__, __LINE__)
#else
#define	THROW(code,msg)
#endif//_DEBUG

//------------------------------------------------------------------------------
#define EN673_DEBUG() printf("%s %s %d \r\n",__FILE__,__func__,__LINE__)

#define ENX_DEBUG

#ifdef ENX_DEBUG
	#define dprintf(fmt, args...) do { printf(fmt, ##args); } while(0);
#else
	#define dprintf(fmt, args...) do {} while(0);
#endif


//******************************************************************************
// 3. String
//------------------------------------------------------------------------------
extern char gNumcode[];

char* strrev(char* s);
//void strrev2(char* begin, char* end);

char* itoa(int value, char* str, int base, int* len);
int itoan(int value, char* str, int base, int len, char fill_val, int sgned);
int uitoan(uint value, char* str, int base, int width, char fill_val);

int sscanf(const char *, const char *, ...);

unsigned long long enx_atoull(char *str);
long long enx_atoll(char *str);
int IPtoStr(unsigned int addr, char* buf, int buflen, int type);
extern char * strnstr(const char *s, const char *find, size_t slen);
extern char* enx_strcat(char* dest, char* src);

#ifdef _DEV_MEMCHR
void* _memchr(const void *src, int c, size_t n);
#define	memchr	_memchr					// can be overridden by users
#endif

#define ZEROPAD	1		// Pad with zero
#define SIGN	2		// Unsigned/signed long
#define PLUS	4		// Show plus
#define SPACE	8		// Space if plus
#define LEFT	16		// Left justified
#define SPECIAL	32		// 0x
#define LARGE	64		// Use 'ABCDEF' instead of 'abcdef'

//******************************************************************************
// 4. Console I/O
//------------------------------------------------------------------------------
#define	putc_	Uart0_Tx
#define puts_	Uart0_Str
#define	puthex	Uart0_Hex

//------------------------------------------------------------------------------
int sprintf(char* str, const char* format, ...);
int snprintf(char* str, size_t n, const char* format, ...);
int ecm_uprintf(const char *format, ...);
#ifdef JTAG_DEBUG_PRINT
int ecm_jprintf(const char *format, ...);
#endif
int uart_printf(const char *format, ...);

#ifdef JTAG_DEBUG_PRINT
#define	jprintf	ecm_jprintf
#else
#define jprintf
#endif
extern int printf(const char *format, ...);


//******************************************************************************
// 5. Memory
//------------------------------------------------------------------------------


//******************************************************************************
// 6. Math
//------------------------------------------------------------------------------


#endif//_LIB_H_

