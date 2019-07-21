//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		stdlib.c
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150725		ygkim		merged similar versions of string functions @ ISP & IP's
// -----------------------------------------------------------------------------
#include "dev.h"
#include "dev_inline.h"		// for inline


//******************************************************************************
// 2. Basic library
//------------------------------------------------------------------------------
// jump to address
void call(uint addr)
{
  	asm("l.jalr\t\t%0" : : "r" (addr) : "r9", "r11");
  	asm("l.nop" : :);
}

//------------------------------------------------------------------------------
// Loops/exits simulation
#ifdef _SIM_MODE
void exit(int i)
{
  asm("l.add r3,r0,%0": : "r" (i));
  asm("l.nop %0": :"K" (NOP_EXIT));
  while(1);
}

void report(int value)
{
  asm("l.addi\tr3,%0,0": :"r" (value));
  asm("l.nop %0": :"K" (NOP_REPORT));
}
#else
void exit(int i)
{
	disable_htb();
	printf("EXIT %d\r\n", i);
	BREAK;
	while(1);
}

void report(int value)
{
	//disable_htb();
  	//printf("\tREPORT 0x%08x\r\n", value);
	puts_("\tR:"); puthex(value); puts_("\r\n");	// simplest call
}
#endif


//------------------------------------------------------------------------------
// trace
int printf_null(const char* format, ...)
{	
	return (int)format;
}

// assert
void assert_failed(char* file, int line)
{	
	disable_htb();
	printf("%s(L%d) : assert failed\r\n", file, line);
	exit(-1);
}

//------------------------------------------------------------------------------
// throw
void throwex(eThrowCode code, char* msg, char* file, int line)
{	
	char* throw_code_str[] = {"MSG! ","WARN! ","ERROR! "};

	printf("%s(L%d) : %s %s \r\n", file, line, throw_code_str[code&0x03], msg);
}


//------------------------------------------------------------------------------
// Pseudo-random number generator based on a Galois LFSR
/*
#define RAND_LFSR_SEED 0x14b6bc3c

_NOINS uint rand(void)
{
  	static uint lfsr = RAND_LFSR_SEED;
  	static int period = 0;
  	
  	// taps: 32 31 29 1; characteristic polynomial: x^32 + x^31 + x^29 + x + 1 
  	lfsr = (lfsr >> 1) ^ (ulong)((0 - (lfsr & 1u)) & 0xd0000001u); 
  	++period;
  	
  	return lfsr;
}
*/
/* OLD
#define RAND_MAX 0x7fff

uint rand(void)
{
     static uint rand_next = 1;

     rand_next = rand_next * 1103515245 + 12345;
     //rand_count++;
    return (uint)(rand_next>>16) & RAND_MAX;    // = (sf_uint)(next / 65536) % 32768
}
*/


//------------------------------------------------------------------------------
// Search

//------------------------------------------------------------------------------
// Arithmetic

