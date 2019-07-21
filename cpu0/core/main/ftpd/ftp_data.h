#ifndef _FTP_DATA_H_
#define _FTP_DATA_H_

#define FTPDATA_POLL_INTERVAL	4
#define FTPDATA_MAX_RETRIES		4
#define FILE_BUFFER_64K_SIZE	(64*1024)
#define FILE_BUFFER_256K_SIZE	(256*1024)

typedef enum {
	PASV,
	LIST,
	RETR,
	STOR
}ftp_dataport_command;

typedef enum {
	DP_IDLE = 0,
	DP_LIST_START,
	DP_LIST_ING1,
	DP_LIST_ING2,
	DP_LIST_ING3,
	DP_LIST_END,
	DP_StoC_START,
	DP_StoC_ING1,
	DP_StoC_ING2,
	DP_StoC_END,
	DP_CtoS_START,
	DP_CtoS_ING1,
	DP_CtoS_ING2,
	DP_CtoS_END
}ftp_dataport_state;

typedef struct {
	struct tcp_pcb *datapcb;
	struct tcp_pcb *datapcbli;
	UINT index;
	UINT retries;
	UINT send;
	UINT flag;

	UINT send_index;
	UINT send_state;
	BYTE *send_addr;
	UINT send_size;
}ftpdata_state;

extern ftpdata_state ftpdstate[10];

extern void ftpdata_close(UINT index);
extern void ftpdata_open(UINT index);
extern void ftpdata_send_index(UINT index);

//extern void FTP_DATAPORT_LIST(UINT index);
//extern void FTP_DATAPORT_RETR(UINT index);
//extern void FTP_DATAPORT_STOR(UINT index);


#endif
