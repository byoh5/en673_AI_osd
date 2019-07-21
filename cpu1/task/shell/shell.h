#ifndef _SHELL_H_
#define _SHELL_H_

#include "dev_types.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
// Command structure
typedef struct{
	char	*name;
	INT16S (*func)(INT32S argc, char *argv[]);
	const char **helphelp;
} tMonCmd;

//-------------------------------------------------------------------------------------------------
// Constants
#define	CMD_SUCCESS			 0
#define	CMD_FAILURE			-1
#define	CMD_PARAM_ERROR     -2
#define	CMD_LINE_ERROR      -3
#define	CMD_ULVL_DENIED     -4
#define	CMD_NOT_FOUND       -5
#define	CMD_PREFILL         -6

#define	CMDLINESIZE			40
#define	ARGCNT				10

#define	CTLC				0x3

//-------------------------------------------------------------------------------------------------
//
#define PROMPT				"EXR1K > "
#define PROMPT_SIZE			30

#define Shell_Unknown()	{	INT32S __unknown; \
							printf("%s(%d) : errer case : input => ", __func__, __LINE__); \
							for(__unknown=0;__unknown<argc;__unknown++) \
								printf("%s ", argv[__unknown]); \
							printf("\r\n"); \
						}

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void T_ShellExe(char *shellBuf);
extern void ShellTask(void *pvParameters);

//-------------------------------------------------------------------------------------------------
// Variable


#endif