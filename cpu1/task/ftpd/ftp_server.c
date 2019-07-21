//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
// -----------------------------------------------------------------------------
#include "dev.h"
#include "dev_inline.h"

/* ------------------------ OR32 includes -------------------------------- */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>		// for struct mallinfo, mallinfo

/* ------------------------ EN673 includes -------------------------------- */
#include "user.h"
#include "msg.h"

/* ------------------------ FreeRTOS includes ----------------------------- */
//#include "FreeRTOS.h"
//#include "task.h"
//#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/err.h"
#include "lwip/api.h"
#include "lwip/netif.h"
#include "lwip/tcp.h"
#include "ethernetif.h"

/* ------------------------ FAT includes ---------------------------------- */
#include "ff.h"
#include "file.h"

/* ------------------------ Project includes ------------------------------ */
#include "ftp_server.h"
#include "ftp_util.h"
#include "timer_task.h"

#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)

#if 0
#define LWIP_DEBUGF(debug, message)
	do { 
		if (((debug) & LWIP_DBG_ON))
		{
			printf(message);
			if ((debug) & LWIP_DBG_HALT)
			{
				while(1);
			}
		}
	} while(0)
#endif

#define ENX_FTP_DGB_COMMAND		DEF_ON
#define ENX_FTP_DGB_PROCESSING	DEF_ON
#define ENX_FTP_DGB_RESPONSE	DEF_ON
#define ENX_FTP_DGB_DEBUG		DEF_ON
#define ENX_FTP_DGB_ERROR		DEF_ON

#define tprintf(debug, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);

const char *strMonthArray[12] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

#define CRLF "\r\n"

static int nftpuse = DEF_OFF;
ftp_state *ftpstate = NULL;

#define NOTCMD(pcb) tprintf(ENX_FTP_DGB_RESPONSE, FTP_CMD_NOT_IMP_RESPONSE);	\
					tcp_write(pcb, FTP_CMD_NOT_IMP_RESPONSE, str_len(FTP_CMD_NOT_IMP_RESPONSE), TCP_WRITE_FLAG_COPY);

#define LOG_CMDRESMSG(msg_buff)	tprintf(ENX_FTP_DGB_RESPONSE, "%s", msg_buff);

// CWD /ABC/ �� ���� ���·� ���� ������ /�� ����
// "/" �� �� ��� �����ϸ� �ȵȴ�.
#define PATH_SLASH_DELETE(strBuff, nLen) { if((nLen) != 0 && (strBuff)[(nLen)] == '/') (strBuff)[(nLen)] = '\0';	}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0

#endif

static void
ftpd_file_close(ftp_state *local_ftpstate);

static err_t
ftpd_tcp_send(struct tcp_pcb *pcb, char *msg);

void
FTP_FILE_LIST_Task(void *arg);

void
FTP_FILE_RETR_Task(void *arg);

void
FTP_FILE_STOR_Task(void *arg);

#if 0

static int 
FTP_DATAPORT_OPEN(ip_addr_t *addr, u16_t port, ftp_state *local_ftpstate);

static err_t
FTP_DATAPORT_ACCEPT(void *arg, struct tcp_pcb *pcb, err_t err);

err_t
FTP_FILE_LIST(void *arg, struct tcp_pcb *pcb, u16_t len);

err_t
FTP_FILE_ServerToClient(void *arg, struct tcp_pcb *pcb, u16_t len);

err_t
FTP_FILE_ClientToServer(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);

static err_t
FTP_DATAPORT_CLOSE(ftp_state *local_ftpstate);

#endif

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_TYPE(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_TYPE_REQUEST)], WORK_BUFFER_SIZE);

	if(strcmp(strGetbuf, "I") == 0)
	{
		strlcpy(msg_buff, FTP_TYPE_I_OK_RESPONSE, SEND_BUFFER_SIZE);
	}
	else if(strcmp(strGetbuf, "A") == 0)
	{
		strlcpy(msg_buff, FTP_TYPE_A_OK_RESPONSE, SEND_BUFFER_SIZE);
	}
	else
	{
		strlcpy(msg_buff, FTP_TYPE_UNRECOGNISED_FAIL_RESPONSE, SEND_BUFFER_SIZE);
		tprintf(ENX_FTP_DGB_DEBUG, "%s\r\n", strGetbuf);
	}
	return ERR_OK;
	UNUSED(local_ftpstate);
}
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_CDUP(ftp_state *local_ftpstate, char *msg_buff)
{
	// ���� ���丮�� �̵�, 3���� ��찡 ����
	if(local_ftpstate->DirLocation == 0)
	{	// �� ��ġ�� ���� root�� ���
		strcpy(local_ftpstate->strNowdirpath, "/");		// send : [250 "/"]
	}
	else
	{
		int i, nCase = 0, nLen = strlen(local_ftpstate->strNowdirpath);
#ifdef __FLS_FAT_LOAD__	// �� ��ġ�� /FLS/ (=0:/) �� ���
		nCase |= (strncmp(local_ftpstate->strNowdirpath, FLS_Driver_ROOT, nLen) == 0 && nLen == 4);
#endif
#ifdef __DEVICE_SD__	// �� ��ġ�� /SD/ (=1:/) �� ���
		nCase |= (strncmp(local_ftpstate->strNowdirpath, SD_Driver_ROOT, nLen) == 0 && nLen == 3);
#endif
		if(nCase)
		{
			strcpy(local_ftpstate->strNowdirpath, "/");	// send : [250 "/"]
			local_ftpstate->DirLocation = 0;
		}
		else
		{
			for(i=nLen-1; i; i--)	// ��ο��� ���� ������ /�� �پ��ִ� ��θ� �����Ѵ�
			{
				if(local_ftpstate->strNowdirpath[i] == '/')
				{
					local_ftpstate->strNowdirpath[i] = '\0';
					break;
				}
			}
		}
	}
	snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_CWD_OK_RESPONSE, local_ftpstate->strNowdirpath);
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_CWD(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	int nLen;
	char strGetbuf[WORK_BUFFER_SIZE] = {0};
	char strGetcwd[WORK_BUFFER_SIZE] = {0};

	nLen = strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_CWD_REQUEST)], WORK_BUFFER_SIZE) - 1;
	PATH_SLASH_DELETE(strGetbuf, nLen);

	// �� �տ� /�� �ٿ��ش�. ���������� /, /FLS, /FLS/TT �� ��������
	if(strGetbuf[0] != '/')
	{
		nLen = snprintf(strGetcwd, WORK_BUFFER_SIZE, "/%s", strGetbuf);
	}
	else
	{
		char c_old = 0;
		int i, j, loc_len = strlen(strGetbuf);
		for(i=0,j=0;i<loc_len;j++)
		{
			if(c_old == strGetbuf[j] && c_old == '/')	// '/'�� �ߺ����� �����ϸ� �����Ѵ�.
				continue;
			strGetcwd[i] = strGetbuf[j];
			c_old = strGetbuf[j];
			i++;
		}
		nLen = i;
	}

	tprintf(ENX_FTP_DGB_PROCESSING, "%d - %d\r\n", nLen, (int)strlen(strGetcwd));

	// �Ѿ�� �޼����� / ���
	if(strlen(strGetcwd) == 1 && strcmp("/", strGetcwd) == 0)	// root �� �̵�
	{
		strcpy(local_ftpstate->strNowdirpath, "/");	// ���� ������ �ֻ����� �̵��ϰ� 
		snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_CWD_OK_RESPONSE, local_ftpstate->strNowdirpath);// send : [250 "/"]
		local_ftpstate->DirLocation = 0;			// ��ġ�� root�� �ǹǷ� 0���� ����
	}
	else
	{
		int nCase = 0;
#ifdef __FLS_FAT_LOAD__	//  ��ġ�� /FLS/ (=0:/), /FLS/xxx/ �� ���(��ü���)
		nCase |= (strncmp(strGetcwd, FLS_Driver_ROOT, strlen(FLS_Driver_ROOT)) == 0);
#endif
#ifdef __DEVICE_SD__	//  ��ġ�� /SD/ (=1:/), /SD/xxx/ �� ���(��ü���)
		nCase |= (strncmp(strGetcwd, SD_Driver_ROOT, strlen(SD_Driver_ROOT)) == 0);
#endif
		if(nCase)
		{
			strlcpy(strGetbuf, strGetcwd, WORK_BUFFER_SIZE);
		}
		else
		{
			snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s%s", local_ftpstate->strNowdirpath, strGetcwd);
			strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
		}
		FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);
		if(f_opendir(&local_ftpstate->dir, strGetcwd) == FR_OK)
		{
			f_closedir(&local_ftpstate->dir);
			strlcpy(local_ftpstate->strNowdirpath, strGetbuf, WORK_BUFFER_SIZE);
			snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_CWD_OK_RESPONSE, local_ftpstate->strNowdirpath);
			local_ftpstate->DirLocation = 1;
		}
		else
		{
			strlcpy(msg_buff, FTP_CWD_FAIL_RESPONSE, SEND_BUFFER_SIZE);
			tprintf(ENX_FTP_DGB_DEBUG, "%s - %s\r\n", strGetcwd, strGetbuf);
		}
	}
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_PWD(ftp_state *local_ftpstate, char *msg_buff)
{
	snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_PWD_OK_RESPONSE, local_ftpstate->strNowdirpath);
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_MKD(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	if(local_ftpstate->DirLocation == 0)
	{
		strlcpy(msg_buff, FTP_MKD_FAIL_RESPONSE, SEND_BUFFER_SIZE);
	}
	else
	{
		char strGetcwd[WORK_BUFFER_SIZE] = {0};
		char strGetbuf[WORK_BUFFER_SIZE] = {0};
		int nLen = snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, (const char *)&Msg[sizeof(FTP_MKD_REQUEST)]) - 1;
		PATH_SLASH_DELETE(strGetbuf, nLen);
		strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
		FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);
		if(f_mkdir(strGetcwd) == FR_OK)
		{
			snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_MKD_OK_RESPONSE, strGetbuf);
		}
		else
		{
			strlcpy(msg_buff, FTP_MKD_FAIL_RESPONSE, SEND_BUFFER_SIZE);
			tprintf(ENX_FTP_DGB_DEBUG, "%s - %s\r\n", strGetcwd, strGetbuf);
		}
	}
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_RMD(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	if(local_ftpstate->DirLocation == 0)
	{
		strlcpy(msg_buff, FTP_RMD_FAIL_RESPONSE, SEND_BUFFER_SIZE);
	}
	else
	{
		char strGetbuf[WORK_BUFFER_SIZE] = {0};
		int nLen = snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, (const char *)&Msg[sizeof(FTP_RMD_REQUEST)]) - 1;
		PATH_SLASH_DELETE(strGetbuf, nLen);
		FTP_strDriverChange(strGetbuf, FTP_Driver_ftp2fat);
		if(f_unlink(strGetbuf) == FR_OK)
		{
			strlcpy(msg_buff, FTP_RMD_OK_RESPONSE, SEND_BUFFER_SIZE);
		}
		else
		{
			strlcpy(msg_buff, FTP_RMD_FAIL_RESPONSE, SEND_BUFFER_SIZE);
			tprintf(ENX_FTP_DGB_DEBUG, "%s\r\n", strGetbuf);
		}
	}
	return ERR_OK;
}
// ���� ����///////////////////////////////////////////////////////////////////
static uint8 FTP_CMD_DELE(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	if(local_ftpstate->DirLocation == 0)
	{
		strlcpy(msg_buff, FTP_DELE_FAIL_RESPONSE, SEND_BUFFER_SIZE);
	}
	else
	{
		char strGetbuf[WORK_BUFFER_SIZE] = {0};
		int nLen = snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, (const char *)&Msg[sizeof(FTP_DELE_REQUEST)]) - 1;
		PATH_SLASH_DELETE(strGetbuf, nLen);
		FTP_strDriverChange(strGetbuf, FTP_Driver_ftp2fat);

		if(f_unlink(strGetbuf) == FR_OK)
		{
			strlcpy(msg_buff, FTP_DELE_OK_RESPONSE, SEND_BUFFER_SIZE);
		}
		else
		{
			strlcpy(msg_buff, FTP_DELE_FAIL_RESPONSE, SEND_BUFFER_SIZE);
			tprintf(ENX_FTP_DGB_DEBUG, "%s\r\n", strGetbuf);
		}
	}
	return ERR_OK;
}
// �̸� ���� step1/////////////////////////////////////////////////////////////
static uint8 FTP_CMD_RNFR(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_RNFR_REQUEST)], WORK_BUFFER_SIZE);

	if(strGetbuf[0] == '/')
	{	// �����η� �״�� ����.
		strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
	}
	else
	{	// ����η� ���� ���� ��θ� ����.
		snprintf(strGetcwd, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, strGetbuf);
	}

	FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);

	if(f_stat(strGetcwd, NULL) == FR_OK)
	{
		strlcpy(local_ftpstate->strSelectfilepath, strGetcwd, WORK_BUFFER_SIZE);
		local_ftpstate->nSelectfilepathState = 1;
		strlcpy(msg_buff, FTP_RNFR_OK_RESPONSE, SEND_BUFFER_SIZE);
	}
	else
	{
		local_ftpstate->strSelectfilepath[0] = '\0';
		local_ftpstate->nSelectfilepathState = 0;
		strlcpy(msg_buff, FTP_RNFR_FAIL_RESPONSE, SEND_BUFFER_SIZE);
		tprintf(ENX_FTP_DGB_DEBUG, "%s - %s\r\n", strGetcwd, strGetbuf);
	}
	return ERR_OK;
}
// �̸� ���� step2/////////////////////////////////////////////////////////////
static uint8 FTP_CMD_RNTO(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	if(local_ftpstate->nSelectfilepathState)
	{
		char strGetcwd[WORK_BUFFER_SIZE] = {0};
		char strGetbuf[WORK_BUFFER_SIZE] = {0};
		
		strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_RNTO_REQUEST)], WORK_BUFFER_SIZE);
		
		if(strGetbuf[0] == '/')
		{	// �����η� �״�� ����.
			strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
		}
		else
		{	// ����η� ���� ���� ��θ� ����.
			snprintf(strGetcwd, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, strGetbuf);
		}

		FTP_strDriverChange(strGetcwd, FTP_Driver_delete);

		if(f_rename(local_ftpstate->strSelectfilepath, strGetcwd) == FR_OK)
		{
			strlcpy(msg_buff, FTP_RNTO_OK_RESPONSE, SEND_BUFFER_SIZE);
		}
		else
		{
			strlcpy(msg_buff, FTP_RNTO_FAIL_RESPONSE, SEND_BUFFER_SIZE);
			tprintf(ENX_FTP_DGB_DEBUG, "%s - %s - %s\r\n", strGetcwd, strGetbuf, local_ftpstate->strSelectfilepath);
		}
	}
	else
	{
		strlcpy(msg_buff, FTP_RNTO_RNFRFAIL_RESPONSE, SEND_BUFFER_SIZE);
	}
	local_ftpstate->strSelectfilepath[0] = '\0';
	local_ftpstate->nSelectfilepathState = 0;
	return ERR_OK;
}
// ���� ũ�� ��û//////////////////////////////////////////////////////////////

static uint8 FTP_CMD_SIZE(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	FILINFO finfo;
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_SIZE_REQUEST)], WORK_BUFFER_SIZE);

	if(strGetbuf[0] == '/')
	{	// �����η� �״�� ����.
		strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
	}
	else
	{	// ����η� ���� ���� ��θ� ����.
		snprintf(strGetcwd, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, strGetbuf);
	}

	FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);

#if _USE_LFN
	TCHAR strFileName[_MAX_LFN+1] = {0};
	finfo.lfname = strFileName;
	finfo.lfsize = _MAX_LFN;
#endif
	if(f_stat(strGetcwd, &finfo) == FR_OK)
	{
		snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_SIZE_OK_RESPONSE, (unsigned int)finfo.fsize);
	}
	else
	{
		strlcpy(msg_buff, FTP_SIZE_FAIL_RESPONSE, SEND_BUFFER_SIZE);
		tprintf(ENX_FTP_DGB_DEBUG, "%s - %s\r\n", strGetcwd, strGetbuf);
	}
	return ERR_OK;
	UNUSED(local_ftpstate);
}
// �̾�ޱ�(S->C)//////////////////////////////////////////////////////////////
static uint8 FTP_CMD_REST(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_REST_REQUEST)], WORK_BUFFER_SIZE);
	tprintf(ENX_FTP_DGB_PROCESSING, "REST pos : [%s]\r\n", strGetbuf);

	local_ftpstate->resend_pos = atoi(strGetbuf);
	snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_REST_OK_RESPONSE, local_ftpstate->resend_pos);
	return ERR_OK;
}

static uint8 FTP_CMD_MDTM(ftp_state *local_ftpstate, char *msg_buff, const char *Msg)
{
	FILINFO finfo;
	int nLen, nCase = 0;
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	nLen = strlcpy(strGetbuf, (const char *)&Msg[sizeof(FTP_MDTM_REQUEST)], WORK_BUFFER_SIZE) - 1;
	PATH_SLASH_DELETE(strGetbuf, nLen);

	if(strGetbuf[0] == '/')
	{	// �����η� �״�� ����.
		nLen = strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
	}
	else
	{	// ����η� ���� ���� ��θ� ����.
		nLen = snprintf(strGetcwd, WORK_BUFFER_SIZE, strlen(local_ftpstate->strNowdirpath) == 1 ? "%s%s" : "%s/%s", local_ftpstate->strNowdirpath, strGetbuf);
	}

	// �Ѿ�� �޼����� / ���
	if(nLen == 1 && strcmp("/", strGetcwd) == 0)
	{
		strlcpy(msg_buff, FTP_MDTM_FAIL_RESPONSE, SEND_BUFFER_SIZE);
		tprintf(ENX_FTP_DGB_DEBUG, "%s\r\n", strGetcwd);
	}
	else
	{
#ifdef __FLS_FAT_LOAD__	//  ��ġ�� /FLS/ (=0:/) �� ���
		nCase |= (strncmp(strGetcwd, FLS_Driver_ROOT, nLen) == 0 && nLen == 4);
#endif
#ifdef __DEVICE_SD__	//  ��ġ�� /SD/ (=1:/) �� ���
		nCase |= (strncmp(strGetcwd, SD_Driver_ROOT, nLen) == 0 && nLen == 3);
#endif
		if(nCase)
		{
			snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_MDTM_OK_RESPONSE, 2016, 1, 1, 9, 0, 0);
		}
		else
		{
			FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);
#if _USE_LFN
			TCHAR strFileName[_MAX_LFN+1] = {0};
			finfo.lfname = strFileName;
			finfo.lfsize = _MAX_LFN;
#endif
			if(f_stat(strGetcwd, &finfo) == FR_OK)
			{
				snprintf(msg_buff, SEND_BUFFER_SIZE, FTP_MDTM_OK_RESPONSE, 
												(finfo.fdate >> 9) + 1980, (finfo.fdate >> 5) & 15, (finfo.fdate & 31), 
												(finfo.ftime >> 11), (finfo.ftime >> 5) & 63, (finfo.ftime & 31) * 2);
			}
			else
			{
				strlcpy(msg_buff, FTP_MDTM_FAIL_RESPONSE, SEND_BUFFER_SIZE);
				tprintf(ENX_FTP_DGB_DEBUG, "%s - %s\r\n", strGetcwd, strGetbuf);
			}
		}
	}
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////
// Ȯ���ɸ���Ʈ����//////////////////////////////////////////////////////////
static uint8 FTP_CMD_FEAT(ftp_state *local_ftpstate, char *msg_buff)
{
	strlcpy(msg_buff, FTP_FEAT_RESPONSE, SEND_BUFFER_SIZE);
	return ERR_OK;
	UNUSED(local_ftpstate);
}
///////////////////////////////////////////////////////////////////////////////

#if 0

#endif

static uint8 FTP_CMD_PASV(ftp_state *local_ftpstate, char *msg_buff)
{
	int i, flag = 0;
	for(i=0;i<FTPDATA_info_MAX;i++)
	{
		if(ftpd_mb.ftp_data[i].state == eFtpd_NONE)
		{
			flag = 1;
			break;
		}
	}

	if(i == FTPDATA_info_MAX)
	{
		for(i=0;i<FTPDATA_info_MAX;i++)
		{
			if(ftpd_mb.ftp_data[i].state == eFtpd_DATAPORT_CLOSE2)
			{
				flag = 1;
				break;
			}
		}
		if(i == FTPDATA_info_MAX)
		{		
			tprintf(ENX_FTP_DGB_ERROR, "Dataport all use\r\n");
		}
	}

	if(flag == 1)
	{
		local_ftpstate->dpindex = i;
		ftpd_mb.ftp_data[i].total_size = 0;
		ftpd_mb.ftp_data[i].port = rand() % (FTP_TCP_DATA_PORT_MAX - FTP_TCP_DATA_PORT_MIN) + FTP_TCP_DATA_PORT_MIN;	// 20000 ~ 49999
		ftpd_mb.ftp_data[i].cmd = PASV;
		ftpd_mb.ftp_data[i].state = eFtpd_DATAPORT_OPEN;
		ftpd_mb.ftp_data[i].datastate = 0;
		ftpd_mb.ftp_data[i].data[0].addr = (UINT)pvPortCalloc(sizeof(BYTE), FILE_BUFFER_64K_SIZE);
		ftpd_mb.ftp_data[i].data[0].size = 0;
		ftpd_mb.ftp_data[i].data[0].flag = 0;
		ftpd_mb.ftp_data[i].data[1].addr = (UINT)pvPortCalloc(sizeof(BYTE), FILE_BUFFER_64K_SIZE);
		ftpd_mb.ftp_data[i].data[1].size = 0;
		ftpd_mb.ftp_data[i].data[1].flag = 0;
		tprintf(ENX_FTP_DGB_PROCESSING, "FTPD(%d) data0(0x%08X) data1(0x%08X)\r\n", i, ftpd_mb.ftp_data[i].data[0].addr, ftpd_mb.ftp_data[i].data[1].addr);
		if(ftpd_mb.ftp_data[i].data[0].addr != 0 && ftpd_mb.ftp_data[i].data[1].addr != 0)
		{
			// ��������Ʈ�� ����.
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_OPEN_DATAPORT, i) == DEF_FAIL)
				vTaskDelay(1); // Time update request to Cpu0

			// ���°� ����Ǳ⸦ ��ٸ���.
			while(ftpd_mb.ftp_data[i].state == eFtpd_DATAPORT_OPEN)
				vTaskDelay(1);

			if(ftpd_mb.ftp_data[i].state == eFtpd_DATAPORT_OK)
			{	// DATAPORT open ����!
				tprintf(ENX_FTP_DGB_PROCESSING, "open ok(port:%d)\r\n", ftpd_mb.ftp_data[i].port);
				snprintf(msg_buff, SEND_BUFFER_SIZE, "%s (%d,%d,%d,%d,%d,%d).\r\n", 
					FTP_PASV_OK_RESPONSE, 
					(unsigned int)(local_ftpstate->pcb->local_ip.addr >> 24 & 0xff), 
					(unsigned int)(local_ftpstate->pcb->local_ip.addr >> 16 & 0xff), 
					(unsigned int)(local_ftpstate->pcb->local_ip.addr >> 8 & 0xff), 
					(unsigned int)(local_ftpstate->pcb->local_ip.addr & 0xff), 
					ftpd_mb.ftp_data[i].port >> 8 & 0xff, 
					ftpd_mb.ftp_data[i].port & 0xff
					);
				return ERR_OK;
			}
			else
			{	// �����ߴٸ� NONE ���·�
				ftpd_mb.ftp_data[i].state == eFtpd_NONE;
			}
		}
		else
		{
			ftpd_mb.ftp_data[i].state == eFtpd_NONE;
		}
	}

	if(ftpd_mb.ftp_data[i].data[0].addr != 0)
	{
		vPortFree((void *)ftpd_mb.ftp_data[i].data[0].addr);
		ftpd_mb.ftp_data[i].data[0].addr = 0;
	}
	if(ftpd_mb.ftp_data[i].data[1].addr != 0)
	{
		vPortFree((void *)ftpd_mb.ftp_data[i].data[1].addr);
		ftpd_mb.ftp_data[i].data[1].addr = 0;
	}

	strlcpy(msg_buff, FTP_DATA_PORT_FAILED, SEND_BUFFER_SIZE);
	return ERR_OK;
}
///////////////////////////////////////////////////////////////////////////////

//static UINT wtime_s, wtime_e;

static uint8 FTP_CMD_LIST(ftp_state *local_ftpstate, const char *Msg)
{
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	ftpd_mb.ftp_data[local_ftpstate->dpindex].cmd = LIST;
	local_ftpstate->is_root = 0;

//	tprintf(ENX_FTP_DGB_PROCESSING, "Msg len(%d) : LIST len(%d)\r\n", strlen(Msg), sizeof(FTP_LIST_REQUEST));
	if(strlen(Msg) > sizeof(FTP_LIST_REQUEST) + 2)	// +2 == CRLF
	{
		strncpy(strGetbuf, (const char *)&Msg[sizeof(FTP_LIST_REQUEST)], WORK_BUFFER_SIZE);
		FTP_strDriverChange(strGetbuf, FTP_Driver_ftp2fat);
		tprintf(ENX_FTP_DGB_PROCESSING, "LIST input [%s]\r\n", strGetbuf);

		if(strcmp(strGetbuf, "/") == 0)
			local_ftpstate->is_root = 1;
		else
			local_ftpstate->is_root = 2;

		if(strcmp(strGetbuf, FLS_RealDriver_ROOT) == 0)
			strcpy(strGetbuf, FLS_RealDriver_ROOT"/");
		if(strcmp(strGetbuf, SD_RealDriver_ROOT) == 0)
			strcpy(strGetbuf, SD_RealDriver_ROOT"/");

		tprintf(ENX_FTP_DGB_PROCESSING, "LIST query [%s] flag(%d)\r\n", strGetbuf, local_ftpstate->is_root);
	}

	ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_LIST_START;

	if((local_ftpstate->DirLocation == 0 && local_ftpstate->is_root == 0) || local_ftpstate->is_root == 1)
	{
		ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_LIST_ING1;
		if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
		{	// ��������Ʈ �̹� ����(accept)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_DATA_ALREADY_OPEN_OK);
		}
		else
		{	// ��������Ʈ ���� ��� ��(listen)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_OK_RESPONSE);
		}
		sys_thread_new("ftpLIST", FTP_FILE_LIST_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
	}
	else
	{
		FRESULT fres;
		if(local_ftpstate->is_root == 2)
		{
			strlcpy(strGetcwd, strGetbuf, WORK_BUFFER_SIZE);
		}
		else
		{
			strlcpy(strGetcwd, local_ftpstate->strNowdirpath, WORK_BUFFER_SIZE);
			FTP_strDriverChange(strGetcwd, FTP_Driver_ftp2fat);
		}
		fres = f_opendir(&local_ftpstate->dir, strGetcwd);
		if(fres == FR_OK)
		{
			ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_LIST_ING1;
			if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
			{	// ��������Ʈ �̹� ����(accept)
				ftpd_tcp_send(local_ftpstate->pcb, FTP_DATA_ALREADY_OPEN_OK);
			}
			else
			{	// ��������Ʈ ���� ��� ��(listen)
				ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_OK_RESPONSE);
			}
			sys_thread_new("ftpLIST", FTP_FILE_LIST_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
		}
		else
		{	// f_opendir error
			FILINFO finfo;
#if _USE_LFN
			TCHAR strFileName[_MAX_LFN+1] = {0};
			finfo.lfname = strFileName;
			finfo.lfsize = _MAX_LFN;
#endif
			tprintf(ENX_FTP_DGB_PROCESSING, "LIST(stat_path) : [%s]\r\n", strGetcwd);
			fres = f_stat(strGetcwd, &finfo);
			if(fres == FR_OK)
			{
				if(finfo.fattrib & AM_DIR)
				{	// LIST Directory
					tprintf(ENX_FTP_DGB_PROCESSING, "LIST(dirname) : [%s] ????\r\n", strGetcwd);
					fres = f_opendir(&local_ftpstate->dir, strGetcwd);
					if(fres == FR_OK)
					{
						ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_LIST_ING1;
						if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
						{	// ��������Ʈ �̹� ����(accept)
							ftpd_tcp_send(local_ftpstate->pcb, FTP_DATA_ALREADY_OPEN_OK);
						}
						else
						{	// ��������Ʈ ���� ��� ��(listen)
							ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_OK_RESPONSE);
						}
						sys_thread_new("ftpLIST", FTP_FILE_LIST_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
					}
					else
					{
						ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_IDLE;

						// ������ ����? �׷��� ���� ��������Ʈ�� �ݾƾ� �Ѵ�.
						while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, local_ftpstate->dpindex) == DEF_FAIL)
							vTaskDelay(1); // Time update request to Cpu0

						tprintf(ENX_FTP_DGB_PROCESSING, "list(opendir fail) end(%d)\r\n", fres);

						ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_FAIL_RESPONSE);
					}
				}
				else
				{	// LIST File => LIST Directory
					int path_len = strlen(strGetcwd);
					int i;
					tprintf(ENX_FTP_DGB_PROCESSING, "LIST(path) : [%s]\r\n", strGetcwd);
					for(i=path_len-1;i>=0;i--)
					{
						if(strGetcwd[i] == '/')
						{
							strlcpy(local_ftpstate->strSelectfilepath, &strGetcwd[i+1], WORK_BUFFER_SIZE);
							strGetcwd[i] = '\0';
							break;
						}
					}
					local_ftpstate->is_root = 3;
					tprintf(ENX_FTP_DGB_PROCESSING, "LIST(dirname) : [%s]\r\n", strGetcwd);
					tprintf(ENX_FTP_DGB_PROCESSING, "LIST(filename) : [%s]\r\n", local_ftpstate->strSelectfilepath);

					fres = f_opendir(&local_ftpstate->dir, strGetcwd);

					ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_LIST_ING1;
					if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
					{	// ��������Ʈ �̹� ����(accept)
						ftpd_tcp_send(local_ftpstate->pcb, FTP_DATA_ALREADY_OPEN_OK);
					}
					else
					{	// ��������Ʈ ���� ��� ��(listen)
						ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_OK_RESPONSE);
					}
					sys_thread_new("ftpLIST", FTP_FILE_LIST_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
				}
			}
			else
			{
				ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_IDLE;

				// ������ ����? �׷��� ���� ��������Ʈ�� �ݾƾ� �Ѵ�.
				while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, local_ftpstate->dpindex) == DEF_FAIL)
					vTaskDelay(1); // Time update request to Cpu0

				tprintf(ENX_FTP_DGB_ERROR, "list(stat fail) end(%d)\r\n", fres);

				ftpd_tcp_send(local_ftpstate->pcb, FTP_LIST_FAIL_RESPONSE);
			}
		}
	}
	return ERR_OK;
}

// ���� -> Ŭ���̾�Ʈ ���� ����////////////////////////////////////////////////
static uint8 FTP_CMD_RETR(ftp_state *local_ftpstate, const char *Msg)
{
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	ftpd_mb.ftp_data[local_ftpstate->dpindex].cmd = RETR;

	strlcpy(strGetcwd, (const char *)&Msg[sizeof(FTP_RETR_REQUEST)], WORK_BUFFER_SIZE);

	if(strGetcwd[0] == '/')
	{	// �����η� �״�� ����.
		strlcpy(strGetbuf, strGetcwd, WORK_BUFFER_SIZE);
	}
	else
	{	// ����η� ���� ���� ��θ� ����.
		snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, strGetcwd);
	}
	FTP_strDriverChange(strGetbuf, FTP_Driver_ftp2fat);

	ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_StoC_START;

	if(f_open(&local_ftpstate->fp, strGetbuf, FA_READ) == FR_OK)
	{
		if(local_ftpstate->resend_pos != 0)
		{
			tprintf(ENX_FTP_DGB_PROCESSING, "file start send pos : %d\r\n", local_ftpstate->resend_pos);
			f_lseek(&local_ftpstate->fp, local_ftpstate->resend_pos);
			local_ftpstate->resend_pos = 0;
		}

		ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_StoC_ING1;
		if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
		{	// ��������Ʈ �̹� �����(accept)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_RETR_ALREADY_OK_RESPONSE);
		}
		else
		{	// ��������Ʈ ���� ��� ��(listen)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_RETR_OK_RESPONSE);
		}
		sys_thread_new("ftpRETR", FTP_FILE_RETR_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
	}
	else
	{
		ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_IDLE;

		// ������ ����? �׷��� ���� ��������Ʈ�� �ݾƾ� �Ѵ�.
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, local_ftpstate->dpindex) == DEF_FAIL)
			vTaskDelay(1); // Time update request to Cpu0

		tprintf(ENX_FTP_DGB_ERROR, "retr(nofile) end\r\n");

		ftpd_tcp_send(local_ftpstate->pcb, FTP_WRITE_FAIL_RESPONSE);
	}
	return ERR_OK;
}
// Ŭ���̾�Ʈ -> ���� ���� ����////////////////////////////////////////////////
static uint8 FTP_CMD_STOR(ftp_state *local_ftpstate, const char *Msg)
{
	char strGetcwd[WORK_BUFFER_SIZE] = {0};
	char strGetbuf[WORK_BUFFER_SIZE] = {0};

	ftpd_mb.ftp_data[local_ftpstate->dpindex].cmd = STOR;

	strlcpy(strGetcwd, (const char *)&Msg[sizeof(FTP_RETR_REQUEST)], WORK_BUFFER_SIZE);

	if(strGetcwd[0] == '/')
	{	// �����η� ���� ����̺�� ����
		strlcpy(strGetbuf, strGetcwd, WORK_BUFFER_SIZE);
	}
	else
	{	// ����η� ���� ���� ��η� �����.
		snprintf(strGetbuf, WORK_BUFFER_SIZE, "%s/%s", local_ftpstate->strNowdirpath, strGetcwd);
	}
	FTP_strDriverChange(strGetbuf, FTP_Driver_ftp2fat);

	ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_CtoS_START;
	
	if(f_open(&local_ftpstate->fp, strGetbuf, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK)
	{
		if(local_ftpstate->resend_pos != 0)
		{
			tprintf(ENX_FTP_DGB_PROCESSING, "file start send pos : %d\r\n", local_ftpstate->resend_pos);
			f_lseek(&local_ftpstate->fp, local_ftpstate->resend_pos);
			local_ftpstate->resend_pos = 0;
		}

		sys_thread_new("ftpSTOR", FTP_FILE_STOR_Task, local_ftpstate, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
		ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_CtoS_ING1;
		if(ftpd_mb.ftp_data[local_ftpstate->dpindex].state == eFtpd_DATAPORT_CONN)
		{	// ��������Ʈ �̹� ����(accept)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_STOR_ALREADY_OK_RESPONSE);
		}
		else
		{	// ��������Ʈ ���� ��� ��(listen)
			ftpd_tcp_send(local_ftpstate->pcb, FTP_STOR_OK_RESPONSE);
		}
	}
	else
	{
		ftpd_mb.ftp_data[local_ftpstate->dpindex].datastate = DP_IDLE;

		// ������ ����? �׷��� ���� ��������Ʈ�� �ݾƾ� �Ѵ�.
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, local_ftpstate->dpindex) == DEF_FAIL)
			vTaskDelay(1); // Time update request to Cpu0

		tprintf(ENX_FTP_DGB_ERROR, "stor(noopen) end\r\n");

		ftpd_tcp_send(local_ftpstate->pcb, FTP_WRITE_FAIL_RESPONSE);
	}
	return ERR_OK;
}

#if 0

#endif

///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////

/********************Private Functions ***************************************/
void FTP_FILE_LIST_Task(void *arg)
{
#define FTPD_LIST_MAX_SIZE	312

	ftp_state *local_ftpstate = (ftp_state *)arg;
	BYTE *bufpos;
	UINT msgsize, bufsize, di;
	UINT index = local_ftpstate->dpindex;

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_LIST, index) == DEF_FAIL)
		vTaskDelay(1); // Time update request to Cpu0

	if(local_ftpstate->DirLocation == 0)
	{
		const char* strRoot = "drw-rw-rw-	 1 1000 	1000  %11u %s 01  2016 %s\r\n";
//		const char* strRoot = "dr--r--r--	 1 1000 	1000  %11u %s 01 12:00 %s\r\n";

		di = 0;
		bufpos = (BYTE *)ftpd_mb.ftp_data[index].data[di].addr;
		bufsize = ftpd_mb.ftp_data[index].data[di].size = 0;

#ifdef __FLS_FAT_LOAD__
		msgsize = sprintf((char *)bufpos, strRoot, 0, strMonthArray[0], &(FLS_Driver_ROOT[1]));
		bufpos += msgsize;
		bufsize += msgsize;
#endif
#ifdef __DEVICE_SD__
		msgsize = sprintf((char *)bufpos, strRoot, 0, strMonthArray[0], &(SD_Driver_ROOT[1]));
		bufpos += msgsize;
		bufsize += msgsize;
#endif
		bufpos = '\0';

		ftpd_mb.ftp_data[index].datastate = DP_LIST_ING2;

		ftpd_mb.ftp_data[index].data[di].size = bufsize;
		ftpd_mb.ftp_data[index].data[di].flag = 2;

		while(ftpd_mb.ftp_data[index].data[di].flag == 2)
			vTaskDelay(1);

		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, index) == DEF_FAIL)
			vTaskDelay(1); // Time update request to Cpu0

		ftpd_tcp_send(local_ftpstate->pcb, FTP_LISTSEND_OK_RESPONSE);

		ftpd_mb.ftp_data[index].datastate = DP_LIST_END;
	}
	else
	{
		FRESULT res;
		FILINFO finfo;
#if _USE_LFN
		TCHAR strFileName[_MAX_LFN+1] = {0};
		finfo.lfname = strFileName;
		finfo.lfsize = _MAX_LFN;
#endif

		struct tm tmout;
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
		int current_year = tmout.tm_year + 1900;

		di = 0;
		bufpos = (BYTE *)ftpd_mb.ftp_data[index].data[di].addr;
		bufsize = ftpd_mb.ftp_data[index].data[di].size = 0;
		
		while(1)
		{
			res = f_readdir(&local_ftpstate->dir, &finfo);
			if((res != FR_OK) || !finfo.fname[0])
			{	// ��� �����͸� �� �о�鿴�ٸ�....
				if(local_ftpstate->dir.fs)
				{
					tprintf(ENX_FTP_DGB_PROCESSING, "dir close\r\n");
					f_closedir(&local_ftpstate->dir);
				}

				if(bufsize > 0)
				{
					tprintf(ENX_FTP_DGB_PROCESSING, "last data\r\n");

					ftpd_mb.ftp_data[index].data[di].size = bufsize;
					ftpd_mb.ftp_data[index].data[di].flag = 2;

					while(ftpd_mb.ftp_data[index].data[di].flag == 2)
						vTaskDelay(1);
				}
				else if(ftpd_mb.ftp_data[index].datastate == DP_LIST_ING2 && bufsize == 0)
				{
					tprintf(ENX_FTP_DGB_PROCESSING, "list end\r\n");
				}
				else if(ftpd_mb.ftp_data[index].datastate == DP_LIST_ING1)
				{	// �ش� ��ο� �ƹ��� ���ϵ��� ��� ����ִ� ����̴�.
					tprintf(ENX_FTP_DGB_PROCESSING, "list(nodata) end\r\n");
				}
				else
				{
					tprintf(ENX_FTP_DGB_ERROR, "list(unknown)\r\n");
				}
				
				while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, index) == DEF_FAIL)
					vTaskDelay(1); // Time update request to Cpu0

				ftpd_tcp_send(local_ftpstate->pcb, FTP_LISTSEND_OK_RESPONSE);

				ftpd_mb.ftp_data[index].datastate = DP_LIST_END;
				break;
			}

			if(strcmp(finfo.fname, ".") == 0 || strcmp(finfo.fname, "..") == 0)
				continue;

			ftpd_mb.ftp_data[index].datastate = DP_LIST_ING2;
 
 			// add �� buf�� ��ĥ�� �ȳ�ĥ�� �Ǵ��ؼ� index�� ������� �Ѵ�.
			// �׷��� index�� �����ϱ����� �� �ش���۰� ������ ������̶�� ����ؾ� �Ѵ�...
			if(bufsize + FTPD_LIST_MAX_SIZE > FILE_BUFFER_64K_SIZE)
			{
				tprintf(ENX_FTP_DGB_PROCESSING, "Buffer Full! index chang!\r\n");
			
				ftpd_mb.ftp_data[index].data[di].size = bufsize;
				ftpd_mb.ftp_data[index].data[di].flag = 1;

				di = ((di == 0) ? 1 : 0);	// di�� 0�̸� 1, 1�̸� 0

				// data���۸� �����ߴµ� ���� 1�̶��... ��ٸ���.
				while(ftpd_mb.ftp_data[index].data[di].flag == 1)
					vTaskDelay(1);

				bufpos = (BYTE *)ftpd_mb.ftp_data[index].data[di].addr;
				bufsize = ftpd_mb.ftp_data[index].data[di].size = 0;
			}

			int nMon = ((finfo.fdate >> 5) & 15) - 1;
			if(nMon < 0)
			{
				nMon = 0;
				finfo.fdate = 1;
			}
			if((finfo.fdate >> 9) + 1980 == current_year)
			{
				msgsize = sprintf((char *)bufpos, "%crw-rw-rw-	  1 1000	 1000  %11u %s %02d %02d:%02d %s\r\n", 
					(finfo.fattrib & AM_DIR) ? 'd' : '-', (unsigned int)finfo.fsize, 
					strMonthArray[nMon] , finfo.fdate & 31,
					(finfo.ftime >> 11), (finfo.ftime >> 5) & 63, local_ftpstate->dir.lfn_idx == 0xFFFF ? finfo.fname : finfo.lfname);
			}
			else
			{
				msgsize = sprintf((char *)bufpos, "%crw-rw-rw-	  1 1000	 1000  %11u %s %02d %5d %s\r\n", 
					(finfo.fattrib & AM_DIR) ? 'd' : '-', (unsigned int)finfo.fsize, 
					strMonthArray[nMon], finfo.fdate & 31, 
					(finfo.fdate >> 9) + 1980, local_ftpstate->dir.lfn_idx == 0xFFFF ? finfo.fname : finfo.lfname);
			}

			// ���� ��ġ�� �̵��Ѵ�.
			bufpos += msgsize;
			bufsize += msgsize;
		}
	}

	ftpd_mb.ftp_data[index].datastate = DP_IDLE;

	vPortFree((void *)ftpd_mb.ftp_data[index].data[0].addr);
	vPortFree((void *)ftpd_mb.ftp_data[index].data[1].addr);

	ftpd_mb.ftp_data[index].data[0].addr = 0;
	ftpd_mb.ftp_data[index].data[1].addr = 0;

	ftpd_mb.ftp_data[index].state = eFtpd_DATAPORT_CLOSE;

	vTaskDelay(50);	// delay 500msec

	ftpd_mb.ftp_data[index].state = eFtpd_NONE;

	vTaskDelete(NULL);
}

void FTP_FILE_RETR_Task(void *arg)
{
	ftp_state *local_ftpstate = (ftp_state *)arg;
	UINT u32Length = 0;
	FRESULT fres;
	UINT di = 0;
	UINT index = local_ftpstate->dpindex;

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_RETR, index) == DEF_FAIL)
		vTaskDelay(1); // Time update request to Cpu0

	while(1)
	{
		if(ftpd_mb.ftp_data[index].state == eFtpd_DATAPORT_STOP)
		{
//			tprintf(ENX_FTP_DGB_PROCESSING, "Dataport Disconnect End\r\n");
			fres = f_close(&local_ftpstate->fp);

			ftpd_mb.ftp_data[index].datastate = DP_StoC_END;

			ftpd_tcp_send(local_ftpstate->pcb, FTP_DISCONNECTION_RESPONSE);

			break;
		}

		fres = f_read(&local_ftpstate->fp, (void *)ftpd_mb.ftp_data[index].data[di].addr, (TCP_MSS*44), &u32Length);
		if(f_eof(&local_ftpstate->fp) == 0 && u32Length == 0)
		{	// ������ eof�� �ƴѵ� read�� 0��... ����?
			tprintf(ENX_FTP_DGB_ERROR, "Read zero byte : msg:%s\r\n", put_rc(fres));
			vTaskDelay(1);
		}
		else if(u32Length == 0)
		{
			// ���� �������� �ִ°� �ִٸ� ��ٸ���.
			while(ftpd_mb.ftp_data[index].data[0].flag == 1)
				vTaskDelay(1);
			while(ftpd_mb.ftp_data[index].data[1].flag == 1)
				vTaskDelay(1);

			tprintf(ENX_FTP_DGB_PROCESSING, "File End\r\n");
			fres = f_close(&local_ftpstate->fp);

			ftpd_mb.ftp_data[index].datastate = DP_StoC_END;

			// ��������Ʈ ���´�.
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_FTPD_CLOSE_DATAPORT, index) == DEF_FAIL)
				vTaskDelay(1); // Time update request to Cpu0

			ftpd_tcp_send(local_ftpstate->pcb, FTP_TRANSFER_OK_RESPONSE);

			break;
		}
		else
		{
			ftpd_mb.ftp_data[index].data[di].size = u32Length;
			ftpd_mb.ftp_data[index].data[di].flag = 1;

			di = ((di == 0) ? 1 : 0);	// di�� 0�̸� 1, 1�̸� 0

			// data���۸� �����ߴµ� ���� 1�̶��... �����۾��� ���� �ʰ� ��ٸ���.
			while(ftpd_mb.ftp_data[index].data[di].flag == 1)
			{
				// �� ���߿� ������ �����ع����ٸ�..
				if(ftpd_mb.ftp_data[index].state == eFtpd_DATAPORT_STOP)
				{
					fres = f_close(&local_ftpstate->fp);
		
					ftpd_mb.ftp_data[index].datastate = DP_StoC_END;
		
					ftpd_tcp_send(local_ftpstate->pcb, FTP_DISCONNECTION_RESPONSE);
		
					break;
				}
//				vTaskDelay(1);
			}

			ftpd_mb.ftp_data[index].data[di].size = 0;
		}
	}

	ftpd_mb.ftp_data[index].datastate = DP_IDLE;

	vPortFree((void *)ftpd_mb.ftp_data[index].data[0].addr);
	vPortFree((void *)ftpd_mb.ftp_data[index].data[1].addr);

	ftpd_mb.ftp_data[index].data[0].addr = 0;
	ftpd_mb.ftp_data[index].data[1].addr = 0;

	ftpd_mb.ftp_data[index].state = eFtpd_DATAPORT_CLOSE;

	vTaskDelay(50);	// delay 500msec

	ftpd_mb.ftp_data[index].state = eFtpd_NONE;

	vTaskDelete(NULL);
}

void FTP_FILE_STOR_Task(void *arg)
{
	ftp_state *local_ftpstate = (ftp_state *)arg;
	UINT u32Length = 0;
	FRESULT fres;
	UINT di = 0;
	UINT index = local_ftpstate->dpindex;

	while(1)
	{
		if(ftpd_mb.ftp_data[index].data[di].flag == 1)
		{
			fres = f_write(&local_ftpstate->fp, (void *)ftpd_mb.ftp_data[index].data[di].addr, ftpd_mb.ftp_data[index].data[di].size, &u32Length);
			ftpd_mb.ftp_data[index].data[di].flag = 0;

			di = ((di == 0) ? 1 : 0);	// di�� 0�̸� 1, 1�̸� 0
		}
		else if(ftpd_mb.ftp_data[index].data[di].flag == 2)
		{
			fres = f_write(&local_ftpstate->fp, (void *)ftpd_mb.ftp_data[index].data[di].addr, ftpd_mb.ftp_data[index].data[di].size, &u32Length);
			ftpd_mb.ftp_data[index].data[di].flag = 0;

			tprintf(ENX_FTP_DGB_PROCESSING, "File End\r\n");
			fres = f_close(&local_ftpstate->fp);

			ftpd_mb.ftp_data[index].datastate = DP_CtoS_END;

			ftpd_tcp_send(local_ftpstate->pcb, FTP_TRANSFER_OK_RESPONSE);

			break;
		}
		else
		{
			vTaskDelay(1);
		}
	}

	ftpd_mb.ftp_data[index].datastate = DP_IDLE;

	vPortFree((void *)ftpd_mb.ftp_data[index].data[0].addr);
	vPortFree((void *)ftpd_mb.ftp_data[index].data[1].addr);

	ftpd_mb.ftp_data[index].data[0].addr = 0;
	ftpd_mb.ftp_data[index].data[1].addr = 0;

	ftpd_mb.ftp_data[index].state = eFtpd_DATAPORT_CLOSE;

	vTaskDelay(50);	// delay 500msec

	ftpd_mb.ftp_data[index].state = eFtpd_NONE;

	vTaskDelete(NULL);
}

#if 0

#endif

static void ftpd_file_close(ftp_state *local_ftpstate)
{
	if(local_ftpstate->fp.fs)
	{
		tprintf(ENX_FTP_DGB_DEBUG, "file close\r\n");
		f_close(&local_ftpstate->fp);
	}
	if(local_ftpstate->dir.fs)
	{
		tprintf(ENX_FTP_DGB_DEBUG, "dir close\r\n");
		f_closedir(&local_ftpstate->dir);
	}
}

static void ftpd_tcp_close(struct tcp_pcb *pcb)
{
	// ftpstate���� ã�Ƽ� ����Ʈ���� ����
	if(ftpstate->pcb == pcb)	// ����Ʈ�� 0�� ��ġ�� ��� ó��
	{
		ftp_state *ftpclose = ftpstate;
		ftpstate = ftpstate->next;
		ftpd_file_close(ftpclose);
		tprintf(ENX_FTP_DGB_DEBUG, "mem_free ok / case 1\r\n");
		vPortFree(ftpclose);
	}
	else	// �� ���� ���
	{
		ftp_state *list = ftpstate;
		while(list!=NULL && list->next->pcb!=pcb)	// ã�Ƽ�
		{
			list = list->next;
		}
		if(list->next != NULL)	// ����
		{
			ftp_state *ftpclose = list->next;
			list->next = ftpclose->next;
			ftpd_file_close(ftpclose);
			tprintf(ENX_FTP_DGB_DEBUG, "mem_free ok / case 2\r\n");
			vPortFree(ftpclose);
		}
	}

	tprintf(ENX_FTP_DGB_DEBUG, "FTPD_TCP_CLOSE : (%IP:%d)\r\n", pcb->remote_ip.addr, pcb->remote_port);

	tcp_arg(pcb, NULL);
	tcp_err(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_recv(pcb, NULL);
	tcp_poll(pcb, NULL, 3);
	tcp_close(pcb);
}

static void ftpd_tcp_error(void *arg, err_t err)
{
	struct tcp_pcb *pcb = ((ftp_state *)arg)->pcb;
	tprintf(ENX_FTP_DGB_DEBUG, "FTPD_TCP_ERROR : error(%s), tcp_pcb->state(%s)\r\n", lwip_strerr(err), tcp_debug_state_str(pcb->state));
	tprintf(ENX_FTP_DGB_DEBUG, "local(%IP:%d) remote(%IP:%d)\r\n", 
			pcb->local_ip.addr, pcb->local_port,
			pcb->remote_ip.addr, pcb->remote_port);
	if(pcb->state == CLOSE_WAIT)
	{
		ftpd_tcp_close(pcb);
	}
}

#if 0
static err_t ftpd_tcp_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	return ERR_OK;
	UNUSED(arg);
	UNUSED(pcb);
	UNUSED(len);
}
#endif

static err_t ftpd_tcp_send(struct tcp_pcb *pcb, char *msg)
{
	int send_len = strlen(msg);
#if (ENX_FTP_DGB_RESPONSE == DEF_ON)
	char strGetbuf[WORK_BUFFER_SIZE] = {0};
	strcpy(strGetbuf, msg);
	strchangnull(strGetbuf, CRLF);
	tprintf(ENX_FTP_DGB_RESPONSE, "[len:%d][%s]\r\n", send_len, strGetbuf);
#endif
	return tcp_write(pcb, msg, send_len, TCP_WRITE_FLAG_COPY);
}

static err_t ftpd_tcp_poll(void *arg, struct tcp_pcb *pcb)
{
//	tprintf(ENX_FTP_DGB_DEBUG, "state(%s)\r\n", tcp_debug_state_str(pcb->state));
	if(pcb->state == CLOSE_WAIT)
	{
		ftpd_tcp_close(pcb);
	}
	return ERR_OK;
	UNUSED(arg);
}

static err_t ftpd_tcp_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	ftp_state *local_ftpstate = (ftp_state *)arg;

	static char msg_send[SEND_BUFFER_SIZE];
	char rcvBuf[REQUEST_BUFFER_SIZE], rcvBuf2[REQUEST_BUFFER_SIZE], *pc;
	int i;
	
	if(err == ERR_OK && p != NULL)
	{
		memset(msg_send, 0, SEND_BUFFER_SIZE);

		invalidate_dcache_range((uint)p->payload, (uint)(p->payload+p->len));

		pc = (char*)p->payload;
//		tprintf(ENX_FTP_DGB_COMMAND, "=====================================\r\n");
		for(i=0;i<p->tot_len;i++)
		{
//			UartPutByte(pc[i]);
			rcvBuf[i] = pc[i];
		}
//		tprintf(ENX_FTP_DGB_COMMAND, "=====================================\r\n");
		rcvBuf[p->tot_len] = '\0';

		tcp_recved(pcb, p->tot_len);
		i = pbuf_free(p);
//		tprintf(ENX_FTP_DGB_COMMAND, "i(%d) len(%d) tot_len(%d)\r\n", i, p->len, p->tot_len);

		// ����� �ҹ��ڷ� ������ client�� ���� ó��
		strlcpy(rcvBuf2, rcvBuf, REQUEST_BUFFER_SIZE);
		strupr(rcvBuf2);
		strchangnull(rcvBuf, CRLF);
		strchangnull(rcvBuf2, CRLF);

//		vTaskDelay(5);

		tprintf(ENX_FTP_DGB_COMMAND, "COMMAND : [%s]\r\n", rcvBuf);

		/*authentication required*/ 		  
		if(local_ftpstate->LoginState == 2)//already logged in
		{
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_SYST_REQUEST////////////////////////////////////////////////////////////////
			if(strnstr(rcvBuf2, FTP_SYST_REQUEST, str_len(FTP_SYST_REQUEST)) != NULL)
			{
				// ������ ������ ������ Ȯ�� �ʿ���
				// �ý�������(�⺻��������) ����.. ex: UNIX L8
				ftpd_tcp_send(pcb, FTP_SYST_OK_RESPONSE);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_TYPE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_TYPE_REQUEST, str_len(FTP_TYPE_REQUEST)) != NULL)
			{
				// 13.11.25 : �Է¿��� type������ ���� �� ���� ���� ���� �ʿ�
				FTP_CMD_TYPE(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_NOOP_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_NOOP_REQUEST, str_len(FTP_NOOP_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ�
				// ��� ��ü�� �ƹ��ϵ� ���� �ʴ´�
				ftpd_tcp_send(pcb, FTP_NOOP_OK_RESPONSE);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_PORT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_PORT_REQUEST, str_len(FTP_PORT_REQUEST)) != NULL)
			{
				// �ɵ�(��Ƽ��)��� �� ���������� ���� ȣ��Ʈ�� ��Ʈ
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_PASV_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_PASV_REQUEST, str_len(FTP_PASV_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ�
				// ����(��ú�)��� �� ���������� ���� ȣ��Ʈ�� ��Ʈ
				FTP_CMD_PASV(local_ftpstate, msg_send);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_LIST_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_LIST_REQUEST, str_len(FTP_LIST_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// �ش��� ȣ�� ������ PORT or PASV ȣ��
				// �ش� ���丮�� �����ϴ� �� ���Ͽ� ���� ���� ����
				FTP_CMD_LIST(local_ftpstate, rcvBuf);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_RETR_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_RETR_REQUEST, str_len(FTP_RETR_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ��������(����->Ŭ���̾�Ʈ)
				FTP_CMD_RETR(local_ftpstate, rcvBuf);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_STOR_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_STOR_REQUEST, str_len(FTP_STOR_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ��������(Ŭ���̾�Ʈ->����)
				FTP_CMD_STOR(local_ftpstate, rcvBuf);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_DELE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_DELE_REQUEST, str_len(FTP_DELE_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���ϻ���
				FTP_CMD_DELE(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_CDUP_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_CDUP_REQUEST, str_len(FTP_CDUP_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���丮 �ڷΰ���
				FTP_CMD_CDUP(local_ftpstate, msg_send);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_CWD_REQUEST/////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_CWD_REQUEST, str_len(FTP_CWD_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���丮 �̵�
				FTP_CMD_CWD(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_PWD_REQUEST/////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_PWD_REQUEST, str_len(FTP_PWD_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���� ���丮�� ���
				FTP_CMD_PWD(local_ftpstate, msg_send);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_MKD_REQUEST/////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_MKD_REQUEST, str_len(FTP_MKD_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���丮 ����
				FTP_CMD_MKD(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_RMD_REQUEST/////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_RMD_REQUEST, str_len(FTP_RMD_REQUEST)) != NULL)
			{
				// ��� ���� �Ϸ� : ���׿��� üũ �ʿ�
				// ���丮 ����
				FTP_CMD_RMD(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_ABOR_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_ABOR_REQUEST, str_len(FTP_ABOR_REQUEST)) != NULL)
			{
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_ACCT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_ACCT_REQUEST, str_len(FTP_ACCT_REQUEST)) != NULL)
			{
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_ALLO_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_ALLO_REQUEST, str_len(FTP_ALLO_REQUEST)) != NULL)
			{
				// code4
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_APPE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_APPE_REQUEST, str_len(FTP_APPE_REQUEST)) != NULL)
			{
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_EPSV_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_EPSV_REQUEST, str_len(FTP_EPSV_REQUEST)) != NULL)
			{
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_FEAT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_FEAT_REQUEST, str_len(FTP_FEAT_REQUEST)) != NULL)
			{
				// Ȯ���ɸ���Ʈ ����
				FTP_CMD_FEAT(local_ftpstate, msg_send);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_HELP_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_HELP_REQUEST, str_len(FTP_HELP_REQUEST)) != NULL)
			{
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_MDTM_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_MDTM_REQUEST, str_len(FTP_MDTM_REQUEST)) != NULL)
			{
				// ���õ� ������ ������ ���� �ð� ���� YYYYMMDDhhmmss
				FTP_CMD_MDTM(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_MODE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_MODE_REQUEST, str_len(FTP_MODE_REQUEST)) != NULL)
			{
				// ���۸�� ���� S(��Ʈ��), B(���), C(����) : �⺻S
				// code
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_REST_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_REST_REQUEST, str_len(FTP_REST_REQUEST)) != NULL)
			{
				// ���� ���� ���� ������ ����..?
				FTP_CMD_REST(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_RNFR_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_RNFR_REQUEST, str_len(FTP_RNFR_REQUEST)) != NULL)
			{
				// RNTO, RNFR ���� ��� / �����̸� ������ ����
				FTP_CMD_RNFR(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_RNTO_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_RNTO_REQUEST, str_len(FTP_RNTO_REQUEST)) != NULL)
			{
				// RNTO, RNFR ���� ��� / �����̸� ����� ���� ����� ����
				FTP_CMD_RNTO(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_SIZE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_SIZE_REQUEST, str_len(FTP_SIZE_REQUEST)) != NULL)
			{
				// 10������ ������ ũ�� ��ȯ
				FTP_CMD_SIZE(local_ftpstate, msg_send, rcvBuf);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_STAT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_STAT_REQUEST, str_len(FTP_STAT_REQUEST)) != NULL)
			{
				// �Ű��������� ���� FTP���� ���μ����� ���� �Ϲݻ�������
				// �Ű��������� ���� ...?
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_STOU_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_STOU_REQUEST, str_len(FTP_STOU_REQUEST)) != NULL)
			{
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_STRU_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_STRU_REQUEST, str_len(FTP_STRU_REQUEST)) != NULL)
			{
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_SITE_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_SITE_REQUEST, str_len(FTP_SITE_REQUEST)) != NULL)
			{
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_REIN_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_REIN_REQUEST, str_len(FTP_REIN_REQUEST)) != NULL)
			{
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_OPTS_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_OPTS_REQUEST, str_len(FTP_OPTS_REQUEST)) != NULL)
			{
				NOTCMD(pcb)
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_QUIT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_QUIT_REQUEST, str_len(FTP_QUIT_REQUEST)) != NULL)
			{
				ftpd_tcp_send(pcb, FTP_QUIT_RESPONSE);
				ftpd_tcp_close(pcb);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_PASS_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_PASS_REQUEST, str_len(FTP_PASS_REQUEST)) != NULL)
			{
				if(!strncmp(&rcvBuf[sizeof(FTP_PASS_REQUEST)], (const char *)gtUser.strUserPw, strlen((const char *)gtUser.strUserPw)))
				{
					ftpd_tcp_send(pcb, FTP_PASS_OK_RESPONSE);
				}
				else
				{
					ftpd_tcp_send(pcb, FTP_PASS_FAIL_RESPONSE);
					local_ftpstate->LoginState = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_UNKNOWN_RESPONSE////////////////////////////////////////////////////////////
			else
			{
				ftpd_tcp_send(pcb, FTP_UNKNOWN_RESPONSE);
			}
		}
		else//not logged in
		{
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_USER_REQUEST////////////////////////////////////////////////////////////////
			//FTP_PASS_REQUEST////////////////////////////////////////////////////////////////
			if(strnstr(rcvBuf2, FTP_USER_REQUEST, str_len(FTP_USER_REQUEST)) != NULL)
			{	
				if(!strncmp(&rcvBuf[sizeof(FTP_USER_REQUEST)], (const char *)gtUser.strUserId, strlen((const char *)gtUser.strUserId)))
				{
					ftpd_tcp_send(pcb, FTP_USER_RESPONSE);
					local_ftpstate->LoginState = 1;
				}
				else
				{
					ftpd_tcp_send(pcb, FTP_PASS_FAIL_RESPONSE);
					local_ftpstate->LoginState = 0;
				}
			}
			else if(strnstr(rcvBuf2, FTP_PASS_REQUEST, str_len(FTP_PASS_REQUEST)) != NULL)
			{
				if(local_ftpstate->LoginState == 1 && !strncmp(&rcvBuf[sizeof(FTP_PASS_REQUEST)], (const char *)gtUser.strUserPw, strlen((const char *)gtUser.strUserPw)))
				{
					ftpd_tcp_send(pcb, FTP_PASS_OK_RESPONSE);
					local_ftpstate->LoginState = 2;
				}
				else
				{
					ftpd_tcp_send(pcb, FTP_PASS_FAIL_RESPONSE);
					local_ftpstate->LoginState = 0;
				}
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_FEAT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_FEAT_REQUEST, str_len(FTP_FEAT_REQUEST)) != NULL)
			{
				// Ȯ���ɸ���Ʈ ����
				FTP_CMD_FEAT(local_ftpstate, msg_send);
				ftpd_tcp_send(pcb, msg_send);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_QUIT_REQUEST////////////////////////////////////////////////////////////////
			else if(strnstr(rcvBuf2, FTP_QUIT_REQUEST, str_len(FTP_QUIT_REQUEST)) != NULL)
			{
				ftpd_tcp_send(pcb, FTP_QUIT_RESPONSE);
				ftpd_tcp_close(pcb);
			}
			//////////////////////////////////////////////////////////////////////////////////
			//FTP_UNKNOWN_RESPONSE////////////////////////////////////////////////////////////
			else
			{
				ftpd_tcp_send(pcb, FTP_UNKNOWN_RESPONSE);
			}
		}
	}
	return ERR_OK;
}

static err_t ftpd_tcp_accept(void *arg, struct tcp_pcb *pcb, err_t err)
{
	arg = NULL;
	if(err == ERR_OK)
	{
		ftp_state *newftpstate = (ftp_state *)pvPortCalloc(1, sizeof(ftp_state));
		if(newftpstate)
		{
			tprintf(ENX_FTP_DGB_DEBUG, "New Accept\r\n");

			newftpstate->pcb = pcb;
			newftpstate->next = ftpstate;
			strcpy(newftpstate->strNowdirpath, "/");
			ftpstate = newftpstate;

			ftpd_tcp_send(pcb, FTP_WELCOME_RESPONSE);
			tcp_setprio(pcb, TCP_PRIO_MIN);
			tcp_arg(pcb, newftpstate);
			tcp_recv(pcb, ftpd_tcp_recv);
			tcp_sent(pcb, NULL);	// ftpd_tcp_sent);
			tcp_err(pcb, ftpd_tcp_error);
			tcp_poll(pcb, ftpd_tcp_poll, 3);	
		}
		else
		{
			tprintf(ENX_FTP_DGB_ERROR, "New Accept / buf.. malloc fail\r\n");
			err = ERR_MEM;
		}
	}
	else
	{
		tprintf(ENX_FTP_DGB_ERROR, "New Accept / err(%s)\r\n", lwip_strerr(err));
	}
	return err;
}

static void ftpd_close(void)
{
	err_t ert;
	ftp_state *list = ftpstate;
	while(list!=NULL)
	{
		// ���� ftp_state�� ������
		ftp_state *ftpclose = list;
		ftpd_file_close(ftpclose);

//		// TCP ������ �����ϸ� ���� ����
//		if(ftpclose->datapcb)
//		{
//			tcp_arg(ftpclose->datapcb, NULL);
//			tcp_err(ftpclose->datapcb, NULL);
//			tcp_sent(ftpclose->datapcb, NULL);
//			tcp_recv(ftpclose->datapcb, NULL);
//			tcp_poll(ftpclose->datapcb, NULL, 0);
//			ert = tcp_close(ftpclose->datapcb);
//			tprintf(ENX_FTP_DGB_DEBUG, "dp  close(%s) state(%s)\r\n", lwip_strerr(ert), tcp_debug_state_str(ftpclose->datapcb->state));
//			ftpclose->datapcb = NULL;
//		}

		tcp_arg(ftpclose->pcb, NULL);
		tcp_err(ftpclose->pcb, NULL);
		tcp_sent(ftpclose->pcb, NULL);
		tcp_recv(ftpclose->pcb, NULL);
		tcp_poll(ftpclose->pcb, NULL, 0);
		ert = tcp_close(ftpclose->pcb);
		tprintf(ENX_FTP_DGB_DEBUG, "cnt close(%s) state(%s)\r\n", lwip_strerr(ert), tcp_debug_state_str(ftpclose->pcb->state));
		ftpclose->pcb = NULL;

		// list�� �������� �ѱ����
		list = list->next;

		// ������ ftp_state�� ����
//		if(ftpclose->filebuf)
//			vPortFree(ftpclose->filebuf);
		vPortFree(ftpclose);
	}

	ftpstate = NULL;
}

static struct tcp_pcb *ftpd_tcp_pcb;
static struct tcp_pcb *ftpd_tcp_pcb_li;

static err_t ftpd_init(void)
{
	ftpd_tcp_pcb = tcp_new();
	if (ftpd_tcp_pcb != NULL)
	{
		err_t ert = tcp_bind(ftpd_tcp_pcb, IPADDR_ANY, FTP_TCP_CONTROL_PORT);
		if(ert == ERR_OK)
		{
			ftpd_tcp_pcb_li = tcp_listen(ftpd_tcp_pcb);
			tcp_accept(ftpd_tcp_pcb_li, ftpd_tcp_accept);
		}
		else
		{
			tprintf(ENX_FTP_DGB_ERROR, "FTP : START err(%s)\r\n", lwip_strerr(ert));
		}
		return ert;
	}
	else
	{
		return ERR_MEM;
	}
}

void FtpdStart(void)
{
	err_t ftperr = ftpd_init();
	if(ftperr == ERR_OK)
	{
		gtUser.bFtpServer = FTPD_USE_ON;
		nftpuse = DEF_ON;
		tprintf(ENX_FTP_DGB_DEBUG, "FTP : START\r\n");
	}
	else
	{
		gtUser.bFtpServer = FTPD_USE_OFF;
		tprintf(ENX_FTP_DGB_ERROR, "FTP : START FAIL(%s)\r\n", lwip_strerr(ftperr));
	}
}

void FtpdStop(void)
{
	if(nftpuse == DEF_ON)
	{
		tprintf(ENX_FTP_DGB_DEBUG, "\r\n");
		ftpd_close();
		tprintf(ENX_FTP_DGB_DEBUG, "\r\n");

		err_t erok;

		if(ftpd_tcp_pcb_li != NULL)
		{
			tcp_accept(ftpd_tcp_pcb_li, NULL);
			tprintf(ENX_FTP_DGB_DEBUG, "\r\n");
			erok = tcp_close(ftpd_tcp_pcb_li);
			tprintf(ENX_FTP_DGB_DEBUG, "FTP : CLOSE(%s) state(%s)\r\n", lwip_strerr(erok), tcp_debug_state_str(ftpd_tcp_pcb_li->state));
			ftpd_tcp_pcb_li = NULL;
		}
		tprintf(ENX_FTP_DGB_DEBUG, "\r\n");
		erok = tcp_close(ftpd_tcp_pcb);
		tprintf(ENX_FTP_DGB_DEBUG, "FTP : CLOSE(%s) state(%s)\r\n", lwip_strerr(erok), tcp_debug_state_str(ftpd_tcp_pcb->state));
		ftpd_tcp_pcb = NULL;

		gtUser.bFtpServer = FTPD_USE_OFF;
		nftpuse = DEF_OFF;
	}		
}
#endif
#endif
