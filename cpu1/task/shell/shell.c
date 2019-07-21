//*************************************************************************************************
// Shell task
//*************************************************************************************************

#include "dev.h"
#include "dev_inline.h"															// for inline
#include "msg.h"

#include "shell.h"
#include "shell_cmd.h"
#include "string.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lwip/sys.h"

static char	gcHistoryBuf[100];

#ifdef __IRQ_UART0__
xSemaphoreHandle xSemShell = NULL;
#endif

//*************************************************************************************************
// Shell functions
//-------------------------------------------------------------------------------------------------
// Take the incoming string and create an argv[] array from that.
// The incoming string is assumed to be writeable.
// The argv[] array is simple a set of pointers into that string, where the whitespace delimited character sets are each NULL terminated.
INT32S _tokenize(char *string, char *argv[])
{
	INT32S	argc, done;

	/* Null out the incoming argv array. */
	for(argc=0; argc<ARGCNT; argc++)	argv[argc] = (char *)0;

	argc = 0;

	while(1){
		while( (*string==' ') || (*string=='\t') )		string++;

		if (*string==0)			break;

		argv[argc] = string;

		while( (*string!=' ') && (*string!='\t') ){
			if( (*string=='\\') && (*(string+1)=='"') ){
				strcpy(string,string+1);
			}
			else if(*string == '"'){
				strcpy(string,string+1);
				while(*string != '"'){
					if( (*string=='\\') && (*(string+1)=='"') )		strcpy(string,string+1);
					if(*string==0)		return(-1);
					string++;
				}

				strcpy(string,string+1);
				continue;
			}

			if(*string==0)		break;
			string++;
		}

		if(*string == 0){	done = 1;					}
		else			{	done = 0;	*string++ = 0;	}

		argc++;
		if(done)			break;

		if(argc >= ARGCNT){
			argc = -1;
			break;
		}
	}
	return(argc);
}

INT32S _DoCommand(char *cmdline)
{
	INT32S ret, argc;
	tMonCmd	*cmdptr;
	char *argv[ARGCNT], cmdcpy[CMDLINESIZE];

	if(cmdline[0]=='_')		cmdline++;
	if(cmdline[0]=='!')		strcpy(cmdline, gcHistoryBuf);

	strcpy(cmdcpy, cmdline);

	if(cmdline[0]!='!')		strcpy(gcHistoryBuf, cmdline);

	/* Build argc/argv structure based on incoming command line. */
	argc = _tokenize(cmdcpy, argv);

	if( (argc==0) && (argc<0) )		return (CMD_FAILURE);

	/* Step through the command table looking for a match between the first token of the command line and a command name. */
	cmdptr = gCmdList;

	if(argv[0]==NULL)				return (CMD_SUCCESS);

	while(cmdptr->name)
	{
		if(strcmp(argv[0], cmdptr->name) == 0)	break;
		cmdptr++;

		if(cmdptr->name == 0)		return (CMD_LINE_ERROR);
	}

//	if(strcmp("md", cmdptr->name) != 0)		printf("%s\n", *cmdptr->helphelp);		// for fast memory display

	ret = cmdptr->func(argc, argv);

#ifndef __IRQ_UART0__
	printf("\r\n");
#endif
	return(ret);
}

//-------------------------------------------------------------------------------------------------
//
#ifdef __IRQ_UART0__
void IsrShell(void *arg)
{
	if (xSemShell) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(xSemShell, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;	// Task switch required?
   		}
	}

	UNUSED(arg);
}

INT32S _getline(char *buf, INT32S max, INT32S timeout)
{	// CPU0 to CPU1
	while(1)
	{
		printf(PROMPT);
		if(xSemaphoreTake(xSemShell, portMAX_DELAY))
		{
			while(cQueue_isempty(&gptMsgShell) != DEF_OK)
			{
				DmaMemSet_ip((BYTE *)buf, 0, max-1);

				unsigned char len = gptMsgShell.arg[gptMsgShell.head][0] - 2;
				if(len <= (MSG_SHELL_MAXLEN - 3))	// len data CR LF 으로 data만 취하기 위해 3을 빼준다.
				{
					DmaMemCpy_ip((BYTE *)buf, (BYTE *)&gptMsgShell.arg[gptMsgShell.head][1], len);
					invalidate_dcache_range((uint)(buf), (uint)(buf+len));
					buf[len] = 0;	// CRLR 제거

					printf((char *)&gptMsgShell.arg[gptMsgShell.head][1]);
					if(_DoCommand(buf) == CMD_LINE_ERROR)
					{
						printf("Bad or not command! : [%s]\r\n", buf);
					}
				}
				DmaMemSet_ip((BYTE *)gptMsgShell.arg[gptMsgShell.head], 0, MSG_SHELL_MAXLEN);

				num_loop(gptMsgShell.head, gptMsgShell.tot_num);
			}
		}
	}

	return 0;
	
	UNUSED(timeout);
	UNUSED(max);
}
#else
INT32S _getline(char *buf, INT32S max, INT32S timeout)
{
	static INT8U	crlf;
	INT32S			total, idx;
	char			*base;
	unsigned char ck;
	
	total = idx = 0;
	base = buf;
	max -= 1;												// Make sure there is space for the null terminator

	for(idx=0; idx<max; idx++){
		while(Uart0_RxExist() == 0)			vTaskDelay(10);

//		*buf = (char)UartGetByte();
		ck = Uart0_GetByte();
		*buf = ck;
		//printf("[%X]", ck);
		if(!*buf){
			idx--;
			return 0;
		}

		if ( (*buf=='\r') || (*buf=='\n') ){
			if(crlf && (*buf != crlf))
				crlf = 0;
		    Uart0_Str("\n");
		    crlf = *buf;
		    *buf = 0;
		    break;
		}

		if (*buf=='\b'){
			if(total){
				idx -=2;
				buf--;
				total--;
				Uart0_Str("\b \b");
			}
		}
		else if(*buf==CTLC){
			Uart0_Str("^C\n");
			*base = 0;
			return 0;
		}
		else{
			Uart0_Tx(*buf);
			total++;
			buf++;
		}

		crlf = 0;

		if (idx>=max){
			printf("\nInput too long (exceeds %d bytes).\n",max+1);
			*buf = 0;
			return 0;
		}
	}

	return(strlen(base));
	UNUSED(timeout);
}
#endif

//*************************************************************************************************
// J-Shell task
//-------------------------------------------------------------------------------------------------
//
#ifdef JTAG_DEBUG_PRINT
xSemaphoreHandle xSemJ_Shell = NULL;

void Isr_JShell(void *arg)
{
	if (xSemJ_Shell) {
		portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(xSemJ_Shell, &xHigherPriorityTaskWoken);
		if(xHigherPriorityTaskWoken) {
			gbXsrTaskSwitchNeeded = 1;    // Task switch required?
		}
	}

	UNUSED(arg);
}

void J_ShellTask(void* pvParameters)
{
	if (xSemJ_Shell == NULL) {
		vSemaphoreCreateBinary(xSemJ_Shell);
	}
	xSemaphoreTake(xSemJ_Shell, portMAX_DELAY);

	char J_ShellCmdBuf[68] = {0};
	while(1) {
		printf(PROMPT);
		if (xSemaphoreTake(xSemJ_Shell, portMAX_DELAY)) {
			DmaMemCpy_ip((BYTE *)J_ShellCmdBuf, (BYTE *)gptMsgShare._JTAG_COMMAND, 64);
			invalidate_dcache_range((uint)(J_ShellCmdBuf), (uint)(J_ShellCmdBuf+64));
			DmaMemSet_ip((BYTE *)gptMsgShare._JTAG_COMMAND, 0, 64);
			if (J_ShellCmdBuf[0] != '\0') {
				printf(J_ShellCmdBuf);
				printf("\r\n");
				if (_DoCommand(J_ShellCmdBuf) == CMD_LINE_ERROR) {
					printf("Bad or not command! : [%s]\r\n", J_ShellCmdBuf);
				}
			} else {
				printf("\r\n");
			}
		}
	}

	UNUSED(pvParameters);
}
#endif

//*************************************************************************************************
// T-Shell task
//-------------------------------------------------------------------------------------------------
//
#if (ENX_TERMD_use==1)
void T_ShellExe(char *shellBuf)
{
	printf(PROMPT);
	if (shellBuf[0] != '\0')
		printf(shellBuf);
	printf("\r\n");
	if (_DoCommand(shellBuf) == CMD_LINE_ERROR) {
		printf("Bad or not command! : [%s]\r\n", shellBuf);
	}
}
#endif

//*************************************************************************************************
// Shell task
//-------------------------------------------------------------------------------------------------
//
void ShellTask(void* pvParameters)
{
#ifdef __IRQ_UART0__
	if(xSemShell == NULL) {
		vSemaphoreCreateBinary(xSemShell);	
	}
	xSemaphoreTake(xSemShell, portMAX_DELAY);
#endif

	vTaskDelay(500);

#ifdef JTAG_DEBUG_PRINT
	sys_thread_new("J-Shell", J_ShellTask, NULL, SHELL_STACK_SIZE, SHELL_TASK_PRIO);
#endif

	char U_ShellCmdBuf[100] = {0};
#ifdef __IRQ_UART0__
	_getline(U_ShellCmdBuf, CMDLINESIZE, 0); // input IDLE...
#else
	while(1)
	{
		printf(PROMPT);
		_getline(U_ShellCmdBuf, CMDLINESIZE, 0); // input IDLE...
		if(_DoCommand(U_ShellCmdBuf) == CMD_LINE_ERROR)
		{
			printf("Bad or not command! : [%s]\r\n", U_ShellCmdBuf);
		}
	}
#endif

	UNUSED(pvParameters);
}

