//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
// -----------------------------------------------------------------------------

/* ------------------------ EN673 includes -------------------------------- */
#include "dev.h"
#include "dev_inline.h"

#include "user.h"
#include "msg.h"
#include "shell.h"

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/err.h"
#include "lwip/api.h"
#include "lwip/sockets.h"

/* ------------------------ Project includes ------------------------------ */
#include "term_server.h"

#if (ENX_TERMD_use==1)
term_st term[2] = {
	{ .name = "Term0S", .cpu = TERM_CPU0, .port = TERM_TCP_PORT_CPU0 }, 
	{ .name = "Term1S", .cpu = TERM_CPU1, .port = TERM_TCP_PORT_CPU1 }
};

/********************Private Functions ***************************************/
static int TermInitValue(term_st *term)
{
	switch (term->cpu) {
		case TERM_CPU0:
			term->now_pos	= gptMsgShare._JTAG_PRINT0_START;
			term->last_pos	= gptMsgShare._JTAG_PRINT0_START;
			term->base		= (UINT *)&(gptMsgShare._JTAG_PRINT0_START);
			term->point 	= (UINT *)&(gptMsgShare._JTAG_PRINT0_POINT);
			break;
		case TERM_CPU1:
			term->now_pos	= gptMsgShare._JTAG_PRINT1_START;
			term->last_pos	= gptMsgShare._JTAG_PRINT1_START;
			term->base		= (UINT *)&(gptMsgShare._JTAG_PRINT1_START);
			term->point 	= (UINT *)&(gptMsgShare._JTAG_PRINT1_POINT);
			break;
		default:
			printf("Error CPU number\r\n");
			return DEF_FAIL;
	}
	return DEF_OK;
}

int TermInitSocket(term_st *term)
{
	struct sockaddr_in sLocalAddr;
	int lSocket = lwip_socket(AF_INET, SOCK_STREAM, 0);
	if (lSocket < 0) {
		return -1;
	}

	memset((char *)&sLocalAddr, 0, sizeof(sLocalAddr));
	sLocalAddr.sin_family = AF_INET;
	sLocalAddr.sin_len = sizeof(sLocalAddr);
	sLocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	sLocalAddr.sin_port = term->port;

	if (lwip_bind(lSocket, (struct sockaddr *)&sLocalAddr, sizeof(sLocalAddr)) < 0) {
        lwip_close(lSocket);
        return -1;
	}

	if (lwip_listen(lSocket, 20) != 0 ){
        lwip_close(lSocket);
        return -1;
	}

	return lSocket;
}

void SendTask(void* arg)
{
	term_st* term =(term_st*) arg;
	while (term->state == TERM_SEND_OPEN) {
		int fir_size = 0, sec_size = *term->point - term->last_pos;
		if (sec_size > 0) {
			lwip_send(term->sock_fd, (void*)term->last_pos, sec_size, 0);
			term->last_pos += sec_size;
		} else if (sec_size < 0) {
			fir_size = (*term->base + JTAG_PRINT_SIZE) - term->last_pos;
			lwip_send(term->sock_fd, (void*)term->last_pos, fir_size, 0);
			fir_size = *term->point - *term->base;
			term->last_pos = *term->base;
			lwip_send(term->sock_fd, (void*)term->last_pos, fir_size, 0);
			term->last_pos += fir_size;
		}
		vTaskDelay(1);
	}
	lwip_send(term->sock_fd, (void *)TERM_BYE_RESPONSE, strlen(TERM_BYE_RESPONSE), 0);	// Bye message
	term->state = TERM_SEND_CLOSE_DONE;
	vTaskDelete(NULL);
}

void TermStart(void *arg)
{
	term_st *term = (term_st *)arg;

	if (TermInitValue(term) == DEF_FAIL) {
		vTaskDelete(NULL);
	}

	char buffer[1024];
	int nbytes, addrlen = sizeof(struct sockaddr_in);
	int lSocket = TermInitSocket(arg);
	if (lSocket >= 0) {
		printf("Caution! Network Terminal Open! (id:%d, port:%d)\r\n", term->cpu, term->port);
		xTaskHandle x_terms_Handle = NULL;
		while (1) {
			struct sockaddr_in client_addr;
			int clientfd = lwip_accept(lSocket, (struct sockaddr *)&client_addr, (socklen_t *)&addrlen);
			printf("Open Terminal(id:%d)\r\n", term->cpu);
			term->sock_fd = clientfd;
			if (clientfd > 0) {
				term->state = TERM_SEND_OPEN;
				lwip_send(clientfd, (void *)TERM_WELCOME_RESPONSE, strlen(TERM_WELCOME_RESPONSE), 0);			// Welcome message
				x_terms_Handle = sys_thread_new(term->name, SendTask, arg, SHELL_STACK_SIZE, SHELL_TASK_PRIO);	// Send task		

				do {
					nbytes = lwip_recv(clientfd, buffer, sizeof(buffer), 0);

					// 사용자가 Terminal을 강제 종료할 때
					if (nbytes <= 0) {
						break;
					}

					// 사용자가 Terminal을 명령으로(EXIT or exit) 종료할 때
					if ((buffer[0] == 'E' && buffer[1] == 'X' && buffer[2] == 'I' && buffer[3] == 'T') || \
						(buffer[0] == 'e' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')) {		// For escape terminal	EX) exit EXIT
						break;
					}

					if (buffer[nbytes-2] == '\r') {
						buffer[nbytes-2] = '\0'; // for change "CR" to "NULL"
					} else {
						hexDump("Term2", buffer, nbytes);
					}

					T_ShellExe(buffer);
				} while (nbytes > 0);

				term->state = TERM_SEND_CLOSE_GO;
				while (term->state != TERM_SEND_CLOSE_DONE) {
					vTaskDelay(1);
				}
				vTaskDelay(50);	// bye-message transmission time...

				printf("Close Terminal (id:%d)\r\n", term->cpu);
				lwip_close(clientfd);
			}
		} 
		lwip_close(lSocket);
	} else {
		printf("Terminal socket Init fail (id:%d)\r\n", term->cpu);
	}
	vTaskDelete(NULL);
}
#endif
