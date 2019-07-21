#ifndef _TERM_SERVER_H_
#define _TERM_SERVER_H_

#include "lib.h"
#define TERM_TCP_PORT_CPU0	27015
#define TERM_TCP_PORT_CPU1	27016

#define TERM_WELCOME_RESPONSE   "$$$ EYENIX TERMINAL SERVER [Escape : exit or EXIT] $$$\r\n\r\n\r\n"
#define TERM_BYE_RESPONSE       "$$$ Bye~ $$$\r\n\r\n\r\n"

#define USE_JTAG_PRINT
#ifdef USE_JTAG_PRINT
#define TERM_PRINT_SIZE_CPU0 	JTAG_PRINT_SIZE 
#define TERM_PRINT_BASE_CPU0	JTAG_PRINT0_BASE
#define TERM_PRINT_POINT_CPU0 	JTAG_PRINT0_END
#define TERM_PRINT_SIZE_CPU1 	JTAG_PRINT_SIZE 
#define TERM_PRINT_BASE_CPU1 	JTAG_PRINT1_BASE 
#define TERM_PRINT_POINT_CPU1 	JTAG_PRINT1_END 
#else
// FOR USE MANUAL MEMORY LOCATION 
#define TERM_PRINT_SIZE_CPU0 	1024*512 
#define TERM_PRINT_BASE_CPU0 	0x09000000 - JTAG_PRINT_SIZE - 4
#define TERM_PRINT_POINT_CPU0 	(JTAG_PRINT_BASE+JTAG_PRINT_SIZE)
#define TERM_PRINT_SIZE_CPU1 	JTAG_PRINT_SIZE 
#define TERM_PRINT_BASE_CPU1 	JTAG_PRINT_BASE 
#define TERM_PRINT_POINT_CPU1 	JTAG_PRINT_POINT 
#endif

typedef enum {
	TERM_SEND_OPEN,
	TERM_SEND_CLOSE_GO,
	TERM_SEND_CLOSE_DONE,
} term_send_state;

typedef struct {
	UINT port;
	UINT cpu;
	UINT sock_fd;
	UINT last_pos;
	UINT now_pos;
	UINT *base;
	UINT *point;
	UINT state;
	CHAR name[128];
} term_st;

typedef enum {
	TERM_CPU0,	// 0
	TERM_CPU1	// 1
} term_cpu_num;

extern term_st term[2];

extern void TermStart(void *arg);

#endif
