//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
// -----------------------------------------------------------------------------
#include "dev.h"	// to use printf
#include "dev_inline.h"	// to use fast code

/* ------------------------ OR32 includes -------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ------------------------ EN673 includes -------------------------------- */
#include "main.h"
#include "user.h"
#include "msg.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/err.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"

/* ------------------------ Project includes ------------------------------ */
#include "ftp_data.h"

#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)

//#define ENX_FTP_DEBUG			// printf 주석용
#define ENX_FTP_ERROR

#ifdef ENX_FTP_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

#ifdef ENX_FTP_ERROR
	int printf(const char *fmt, ...);
	#define eprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define eprintf(fmt, args...) do {} while(0);
#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0

#endif

ftpdata_state ftpdstate[10];

static err_t ftpdata_sent(void *arg, struct tcp_pcb *pcb, u16_t len);
static err_t ftpdata_poll(void *arg, struct tcp_pcb *pcb);
static err_t ftpdata_close_or_abort_conn(struct tcp_pcb *pcb, ftpdata_state *fds, UINT abort_conn);

#if 0

#endif

/********************Private Functions ***************************************/
void ftpdata_close(UINT index)
{
	err_t ert;
	tprintf("idx:%d\r\n", index);
	if(ftpdstate[index].datapcb != NULL)
	{
		UINT port = ftpdstate[index].datapcb->remote_port;
		if(ftpdstate[index].datapcb->state == CLOSE_WAIT)
		{
			ert = tcp_close(ftpdstate[index].datapcb);
			tprintf("data_pcb close(ert:%d)(remote_port:%d)(state:%s)\r\n", ert, port, tcp_debug_state_str(ftpdstate[index].datapcb->state));
		}
		else if(ftpdstate[index].datapcb->state == TIME_WAIT)
		{
			tcp_err(ftpdstate[index].datapcb, NULL);
			tcp_arg(ftpdstate[index].datapcb, NULL);
			tcp_poll(ftpdstate[index].datapcb, NULL, 0);
			tprintf("data_pcb (remote_port:%d)(state:%s)\r\n", port, tcp_debug_state_str(ftpdstate[index].datapcb->state));
		}
		else if(ftpdstate[index].datapcb->state != CLOSED)
		{
			tcp_sent(ftpdstate[index].datapcb, NULL);
			tcp_recv(ftpdstate[index].datapcb, NULL);
			ert = tcp_close(ftpdstate[index].datapcb);
			tprintf("data_pcb close(ert:%d)(remote_port:%d)(state:%s)\r\n", ert, port, tcp_debug_state_str(ftpdstate[index].datapcb->state));
			if(ftpdstate[index].datapcb->state == CLOSED)
			{
				tprintf("data_pcb closed\r\n");
				tcp_arg(ftpdstate[index].datapcb, NULL);
				tcp_err(ftpdstate[index].datapcb, NULL);
				tcp_poll(ftpdstate[index].datapcb, NULL, 3);
			}
			ftpdstate[index].datapcb = NULL;
		}
		else
			tprintf("data_pcb closed\r\n");
		UNUSED(port);
	}
	else
		tprintf("data_pcb NULL\r\n");
}

static void	ftpdata_error(void *arg, err_t err)
{
	ftpdata_state *fds = (ftpdata_state *)arg;
	if(fds != NULL)
	{
		eprintf("Error, %s, (idx:%d,%d,%d)\r\n", lwip_strerr(err), fds->index, ftpd_mb.ftp_data[fds->index].state, ftpd_mb.ftp_data[fds->index].datastate);
	}		
	else
	{
		eprintf("Error, %s\r\n", lwip_strerr(err));
	}

	if(ftpd_mb.ftp_data[fds->index].cmd == LIST || ftpd_mb.ftp_data[fds->index].cmd == RETR)
	{	// server to client상황에서도 강제종료 등의 이슈가 발생하면 recv가 호출되는듯...
		ftpd_mb.ftp_data[fds->index].state = eFtpd_DATAPORT_STOP;
	}
}

void ftpdata_send(struct tcp_pcb *pcb, ftpdata_state *fds)
{
	err_t ert;

	if(fds == NULL || pcb == NULL)
	{
		return;
	}

	if(pcb->state != ESTABLISHED)
	{
//		tprintf("%s\r\n", tcp_debug_state_str(pcb->state));
		return;
	}

//	pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
//	pcb->flags |= TF_ACK_NOW; //diable delayed ack function

	if(ftpd_mb.ftp_data[fds->index].cmd == LIST || ftpd_mb.ftp_data[fds->index].cmd == RETR)
	{
		if(ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag == 1 || 
			ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag == 2)
		{
			if(fds->send_state == 0)
			{
				fds->send_state = 1;
				fds->send_addr = (BYTE *)ftpd_mb.ftp_data[fds->index].data[fds->send_index].addr;
				fds->send_size = ftpd_mb.ftp_data[fds->index].data[fds->send_index].size;
			}

			while(tcp_sndbuf(pcb) >= TCP_MSS && tcp_sndqueuelen(pcb) < TCP_SND_QUEUELEN)
			{
				UINT send_size = 0;
				if(fds->send_size > TCP_MSS)
				{
					send_size = TCP_MSS;
				}
				else
				{
					send_size = fds->send_size;
				}

				if(fds->send_size - send_size != 0) // 버퍼가 그대로 살아 있는 경우
					ert = tcp_write(pcb, fds->send_addr, send_size, 0);
				else	// size가 0이 될것이다. 그러면 버퍼도 비워질것이다. 그러므로 TCP_WRITE_FLAG_COPY를 쓴다...(?)
					ert = tcp_write(pcb, fds->send_addr, send_size, TCP_WRITE_FLAG_COPY);
				if(ert != ERR_OK)
					eprintf("tcp_write error : %s\r\n", lwip_strerr(ert));

				fds->send = 1;
				fds->send_addr += send_size;
				fds->send_size -= send_size;

				if(fds->send_size == 0)
				{
					fds->send_state = 0;
					if(ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag == 1)
					{
						ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag = 0; // flag down
						fds->send_index = fds->send_index == 0 ? 1 : 0; // index update
					}
					else if(ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag == 2)
					{	// 모든 데이터를 전송함.
						ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag = 0;
						fds->send_index = 0;
					}
					break;
				}
			}
		}
		else
		{	// not data? 데이터준비가 더 늦은경우다.
			tprintf("%s-Idle\r\n", tcp_debug_state_str(pcb->state));
//			timer_sched_timeout_cb(0, TIMEOUT_ONESHOT, (void (*)(void *))ftpdata_send, fds);
		}
	}
#ifdef __WDT__
	enx_wdt_reset();
#endif

}

void ftpdata_send_index(UINT index)
{
	ftpdata_send(ftpdstate[index].datapcb, &ftpdstate[index]);
}

static err_t ftpdata_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	ftpdata_state *fds = (ftpdata_state *)arg;

	if(fds == NULL)
	{
		return ERR_OK;
	}

	fds->send = 0;
	fds->retries = 0;

	if(pcb->state == ESTABLISHED)
	{
		ftpdata_send(pcb, fds);
	}
	else
	{
		tprintf("%s\r\n", tcp_debug_state_str(pcb->state));
	}

	return ERR_OK;

	UNUSED(len);
}

static err_t ftpdata_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	ftpdata_state *fds = (ftpdata_state *)arg;

	if(err != ERR_OK || fds == NULL)
	{
		if(p != NULL)
		{
			tcp_recved(pcb, p->tot_len);
			pbuf_free(p);
		}

		if(err != ERR_OK)
		{
			eprintf("Error, %s, close\r\n", lwip_strerr(err));
		}
		if(fds == NULL)
		{
			eprintf("Error, fds is NULL, close\r\n");
		}

		ftpdata_close_or_abort_conn(pcb, fds, 0);
		return ERR_OK;
	}

	fds->retries = 0;

	if(ftpd_mb.ftp_data[fds->index].cmd == LIST || ftpd_mb.ftp_data[fds->index].cmd == RETR)
	{	// server to client상황에서도 강제종료 등의 이슈가 발생하면 recv가 호출되는듯...
		if(p == NULL)
		{
			ftpd_mb.ftp_data[fds->index].data[0].flag = 0;
			ftpd_mb.ftp_data[fds->index].data[1].flag = 0;
			ftpd_mb.ftp_data[fds->index].state = eFtpd_DATAPORT_STOP;
			tprintf("close, p is NULL, %s\r\n", tcp_debug_state_str(pcb->state));
			ftpdata_close_or_abort_conn(pcb, fds, 0);
			return ERR_OK;
		}
	}

	if(p)
	{
		struct pbuf *q;
		for(q=p; q!=NULL; q=q->next)
		{
			if((fds->send_size+q->len) <= FILE_BUFFER_64K_SIZE)
			{
				fds->send_addr = (BYTE *)(ftpd_mb.ftp_data[fds->index].data[fds->send_index].addr + fds->send_size);
				DmaMemCpy_ip_async(fds->send_addr, (BYTE *)q->payload, q->len);
				fds->send_size += q->len;
			}
			else
			{	// BUFFER에 기록 후 파일에 쓴 다음 잔여 데이터를 BUFFER에 씀
				fds->send_addr = (BYTE *)(ftpd_mb.ftp_data[fds->index].data[fds->send_index].addr + fds->send_size);
				UINT bufremain = FILE_BUFFER_64K_SIZE - fds->send_size;
				DmaMemCpy_ip(fds->send_addr, (BYTE *)q->payload, bufremain);
				fds->send_size += bufremain;

				ftpd_mb.ftp_data[fds->index].data[fds->send_index].size = fds->send_size;
				ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag = 1;
				fds->send_index = fds->send_index == 0 ? 1 : 0;	// index update

				// swap했는데 swap한 버퍼도 아직 사용중이라면 0이 될때까지 기다린다...(CPU1이 0으로 해준다)
				while(ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag != 0);

				BYTE *bufremain_payload = &(((BYTE *)q->payload)[bufremain]);
				bufremain = q->len - bufremain;
				fds->send_addr = (BYTE *)ftpd_mb.ftp_data[fds->index].data[fds->send_index].addr;

				if(bufremain > FILE_BUFFER_64K_SIZE)
				{ // 남은 크기가 buffer보다 크다면, bug...
					while(1)
					{
						eprintf("bufremain(%d) overflow!!\r\n", bufremain);
						eprintf("send_size(%d) p->tot_len(%d)\r\n", fds->send_size, p->len);
					}
				}

				DmaMemCpy_ip_async(fds->send_addr, bufremain_payload, bufremain);
				fds->send_size = bufremain;
			}
		}

		while(DMA_GO);

		tcp_recved(pcb, p->tot_len);
		pbuf_free(p);
	}
	else
	{	// p == NULL
		ftpd_mb.ftp_data[fds->index].data[fds->send_index].size = fds->send_size;
		ftpd_mb.ftp_data[fds->index].data[fds->send_index].flag = 2;

		tprintf("close, p is NULL\r\n");
		ftpdata_close_or_abort_conn(pcb, fds, 0);
	}

	return ERR_OK;
}

#if 0//NOT_DATAPORT_FUNC
static err_t FTP_DATAPORT_SENT(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	ftpdata_state *fds = (ftpdata_state *)arg;
	tprintf("%d\r\n", ftpd_mb.ftp_data[fds->index].datastate);
	return ERR_OK;
}

static err_t ftpdata_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	ftpdata_state *fds = (ftpdata_state *)arg;
	tprintf("%d\r\n", ftpd_mb.ftp_data[fds->index].datastate);
	return ERR_OK;
}
#endif

static err_t ftpdata_close_or_abort_conn(struct tcp_pcb *pcb, ftpdata_state *fds, UINT abort_conn)
{
	err_t err;

	tprintf("idx:%d\r\n", (fds != NULL ? fds->index : 0xFFFFFFFF));

	tcp_arg(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_err(pcb, NULL);
	tcp_poll(pcb, NULL, 0);
	tcp_sent(pcb, NULL);

	if(abort_conn)
	{
		tcp_abort(pcb);
		return ERR_OK;
	}

	err = tcp_close(pcb);
	if(err != ERR_OK)
	{
		eprintf("error(%d) closing, try again later in poll(port:%d)\r\n", err, pcb->remote_port);
		tcp_poll(pcb, ftpdata_poll, FTPDATA_POLL_INTERVAL);
	}

	return err;
	UNUSED(fds);
}

static err_t ftpdata_poll(void *arg, struct tcp_pcb *pcb)
{
	ftpdata_state *fds = (ftpdata_state *)arg;

	if(fds == NULL)
	{
		tprintf("close, fds is NULL\r\n");
		ftpdata_close_or_abort_conn(pcb, fds, 0);
		return ERR_OK;
	}
	else
	{
		fds->retries++;
		if(fds->retries == FTPDATA_MAX_RETRIES)
		{
			tprintf("close, retries:%d\r\n", fds->retries);
			ftpdata_close_or_abort_conn(pcb, fds, 0);
			return ERR_OK;
		}

		if(ftpd_mb.ftp_data[fds->index].cmd == LIST || ftpd_mb.ftp_data[fds->index].cmd == RETR)
		{
			ftpdata_send(pcb, fds);
			if(fds->send == 1)
			{
				fds->send = 0;
				tcp_output(pcb);
			}
		}
	}
	return ERR_OK;
}

static err_t ftpdata_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	if(err == ERR_OK)
	{
		ftpdata_state *fds = (ftpdata_state *)arg;

		tprintf("New Accept(idx:%d) / no err\r\n", fds->index);

		fds->datapcb = pcb;
		ftpd_mb.ftp_data[fds->index].state = eFtpd_DATAPORT_CONN;

//		tcp_accepted(pcb);
		tcp_setprio(pcb, TCP_PRIO_MIN);

		tcp_arg(pcb, fds);
		tcp_err(pcb, ftpdata_error);
		tcp_poll(pcb, ftpdata_poll, FTPDATA_POLL_INTERVAL);
		tcp_recv(pcb, ftpdata_recv);
		tcp_sent(pcb, ftpdata_sent);

		// listen pcb close
		err_t ert = tcp_close(fds->datapcbli);
		if(ert != ERR_OK)
		{
			eprintf("data_pcb_li close fail!(%d)\r\n", ert);
		}
		else
		{
			if(fds->datapcbli->state == CLOSED)
			{
				tprintf("data_pcb_li closed\r\n");
				fds->datapcbli = NULL;
			}
		}
	}
	else
	{
		eprintf("New Accept / err?(%d)\r\n", err);
	}
	return ERR_OK;
}

void ftpdata_open(UINT index)
{
	memset(&ftpdstate[index], 0, sizeof(ftpdata_state));
	ftpdstate[index].index = index;
	ftpdstate[index].datapcb = tcp_new();
	if(ftpdstate[index].datapcb != NULL)
	{
		tprintf("tcp_new ok(idx:%d)\r\n", index);
		err_t ert = tcp_bind(ftpdstate[index].datapcb, IP_ADDR_ANY, ftpd_mb.ftp_data[index].port);
		if(ert != ERR_OK)
		{
			eprintf("tcp_bind fail : %s\r\n", lwip_strerr(ert));
		}
		else
		{
			ftpdstate[index].datapcbli = tcp_listen(ftpdstate[index].datapcb);
			if(ftpdstate[index].datapcbli == NULL)
			{
				eprintf("tcp_listen fail\r\n");
			}
			else
			{
				tcp_arg(ftpdstate[index].datapcbli, &ftpdstate[index]);
				tcp_accept(ftpdstate[index].datapcbli, ftpdata_accept);
				ftpd_mb.ftp_data[index].state = eFtpd_DATAPORT_OK;
				return;
			}
		}
	}
	else
	{
		eprintf("tcp_new fail\r\n");
	}
	ftpd_mb.ftp_data[index].state = eFtpd_FAIL;
}
#endif
#endif
