#ifndef _FTP_SERVER_H_
#define _FTP_SERVER_H_

#include "ff.h"

typedef enum {
	FTP_Driver_ftp2fat,
	FTP_Driver_fat2ftp,
	FTP_Driver_delete
}eFTP_Driver;

#define FLS_Driver_ROOT		"/FLS"
#define SD_Driver_ROOT		"/SD"
#define FLS_RealDriver_ROOT	"0:"
#define SD_RealDriver_ROOT	"1:"

#define FTP_TASK_PRIORITY		tskIDLE_PRIORITY + 3

#define FTP_TCP_CONTROL_PORT	21

//#define FTP_USER_COUNT			5

#define FTP_TCP_DATA_PORT_MIN	20000
#define FTP_TCP_DATA_PORT_MAX	50000

#define FTP_FILE_BUFFER			1	// FTP_FILE_ClientToServer함수에서 사용(1사용, 0비 사용)

#define REQUEST_BUFFER_SIZE		128
#define SEND_BUFFER_SIZE		256
#define WORK_BUFFER_SIZE		256
#define FILE_BUFFER_1K_SIZE		1024
#define FILE_BUFFER_4K_SIZE		4096
#define FILE_BUFFER_64K_SIZE	(64*1024)
#define FILE_BUFFER_256K_SIZE	(256*1024)

/*FTP Server Requests*/
#define FTP_ABOR_REQUEST           "ABOR"
#define FTP_ACCT_REQUEST           "ACCT"
#define FTP_ALLO_REQUEST           "ALLO"
#define FTP_APPE_REQUEST           "APPE"
#define FTP_CDUP_REQUEST           "CDUP"
#define FTP_CWD_REQUEST            "CWD"
#define FTP_DELE_REQUEST           "DELE"
#define FTP_HELP_REQUEST           "HELP"
#define FTP_LIST_REQUEST           "LIST"
#define FTP_MDTM_REQUEST           "MDTM"
#define FTP_MKD_REQUEST            "MKD"
#define FTP_MODE_REQUEST           "MODE"
#define FTP_NLST_REQUEST           "NLST"
#define FTP_NOOP_REQUEST           "NOOP"
#define FTP_PASS_REQUEST           "PASS"
#define FTP_PASV_REQUEST           "PASV"
#define FTP_PORT_REQUEST           "PORT"
#define FTP_PWD_REQUEST            "PWD"
#define FTP_QUIT_REQUEST           "QUIT"
#define FTP_REIN_REQUEST           "REIN"
#define FTP_REST_REQUEST           "REST"
#define FTP_RETR_REQUEST           "RETR"
#define FTP_RMD_REQUEST            "RMD"
#define FTP_RNFR_REQUEST           "RNFR"
#define FTP_RNTO_REQUEST           "RNTO"
#define FTP_SITE_REQUEST           "SITE"
#define FTP_SIZE_REQUEST           "SIZE"
#define FTP_STAT_REQUEST           "STAT"
#define FTP_STOR_REQUEST           "STOR"
#define FTP_STOU_REQUEST           "STOU"
#define FTP_STRU_REQUEST           "STRU"
#define FTP_SYST_REQUEST           "SYST"
#define FTP_TYPE_REQUEST           "TYPE"
#define FTP_USER_REQUEST           "USER"
#define FTP_EPSV_REQUEST           "EPSV"
#define FTP_FEAT_REQUEST           "FEAT"
#define FTP_OPTS_REQUEST           "OPTS"

/*FTP Server Response*/
#define FTP_WELCOME_RESPONSE        "220 Service Ready\r\n"
#define FTP_USER_RESPONSE           "331 USER OK. PASS needed\r\n"
#define FTP_PASS_FAIL_RESPONSE      "530 NOT LOGGED IN\r\n"
#define FTP_PASS_OK_RESPONSE        "230 USR LOGGED IN\r\n"
#define FTP_PORT_OK_RESPONSE        "200 PORT OK\r\n"
#define FTP_NLST_OK_RESPONSE        "150 NLST OK\r\n"
#define FTP_QUIT_RESPONSE           "221 BYE OK\r\n"

#define FTP_SERVICE_NOT_AVAILABLE	"421 Service not available, closing control connection.\r\n"

// 파일 삭제 메세지
#define FTP_DELE_OK_RESPONSE        "250 Delete operation successful.\r\n"
#define FTP_DELE_FAIL_RESPONSE      "550 Delete operation failed.\r\n"

// 파일 전송 메세지
#define FTP_RETR_OK_RESPONSE        	"150 RETR OK\r\n"
#define FTP_STOR_OK_RESPONSE        	"150 STOR OK\r\n"
#define FTP_RETR_ALREADY_OK_RESPONSE    "125 Data connection already open; RETR OK. \r\n"
#define FTP_STOR_ALREADY_OK_RESPONSE    "125 Data connection already open; STOR OK. \r\n"
#define FTP_WRITE_FAIL_RESPONSE			"550 File unavailable.\r\n"

// 파일 이름변경, 이동 메세지
#define FTP_RNFR_OK_RESPONSE		"350 Ready for RNTO.\r\n"
#define FTP_RNFR_FAIL_RESPONSE		"550 RNFR command failed. File unavailable.\r\n"
#define FTP_RNTO_OK_RESPONSE		"250 Rename successful.\r\n"
#define FTP_RNTO_RNFRFAIL_RESPONSE	"503 RNRF required first.\r\n"
#define FTP_RNTO_FAIL_RESPONSE		"553 RNTO command failed. File name not allowed.\r\n"

// 디렉토리 생성, 삭제, 확인 메세지
//#define FTP_MKD_OK_RESPONSE         "257 \"%s/%s\" created\r\n"
#define FTP_MKD_OK_RESPONSE         "257 \"%s\" created\r\n"
#define FTP_RMD_OK_RESPONSE         "250 Remove directory operation successful.\r\n"
#define FTP_MKD_FAIL_RESPONSE       "550 Create directory operation failed.\r\n"
#define FTP_RMD_FAIL_RESPONSE       "550 Remove directory operation failed.\r\n"
#define FTP_CWD_OK_RESPONSE			"250 \"%s\"\r\n"
#define FTP_PWD_OK_RESPONSE			"257 \"%s\"\r\n"
#define FTP_CWD_FAIL_RESPONSE		"550 Failed to change directory.\r\n"

// LIST 명령
#define FTP_LISTSEND_OK_RESPONSE    "226 Directory send OK.\r\n"
#define FTP_LIST_OK_RESPONSE        "150 Here comes the directory listing.\r\n"
#define FTP_LIST_FAIL_RESPONSE		"450 File unavailable.\r\n"

// DATAPORT 관련 메세지
#define FTP_DATA_PORT_FAILED        "425 Cannot open Data Port\r\n"
#define FTP_DATA_ALREADY_OPEN_OK	"125 Data connection already open; Transfer starting.\r\n"
#define FTP_TRANSFER_OK_RESPONSE    "226 Transfer OK\r\n"
#define FTP_DISCONNECTION_RESPONSE	"426 Failure writing network stream.\r\n"

#define FTP_CMD_NOT_IMP_RESPONSE    "502 Command Unimplemented\r\n"
#define FTP_UNKNOWN_RESPONSE        "500 Unrecognized Command\r\n"
#define FTP_BAD_SEQUENCE_RESPONSE   "503 Bad Sequence of Commands\r\n"
//#define FTP_SYST_OK_RESPONSE		"215 freeRTOS\r\n"
#define FTP_SYST_OK_RESPONSE		"215 UNIX Type: L8\r\n"
#define FTP_PASV_OK_RESPONSE        "227 Entering Passive Mode"
#define FTP_NOOP_OK_RESPONSE        "200 NOOP OK\r\n"

// TYPE 명령
#define FTP_TYPE_I_OK_RESPONSE				"200 Switching to Binary mode.\r\n"
#define FTP_TYPE_A_OK_RESPONSE				"200 Switching to ASCII mode.\r\n"
#define FTP_TYPE_UNRECOGNISED_FAIL_RESPONSE	"500 Unrecognised TYPE command.\r\n"
#define FTP_TYPE_NOTIMPLEMENTED_FAIL_RESPONSE	"504 Command not implemented for that parameter.\r\n"

// SIZE 명령
#define FTP_SIZE_OK_RESPONSE		"213 %d\r\n"
#define FTP_SIZE_FAIL_RESPONSE		"550 Could not get file size.\r\n"

// MDTM 명령
#define FTP_MDTM_OK_RESPONSE		"213 %04d%02d%02d%02d%02d%02d\r\n"
#define FTP_MDTM_FAIL_RESPONSE		"550 Could not get file modification time.\r\n"

// REST 명령
#define FTP_REST_OK_RESPONSE		"350 Restart position accepted (%d).\r\n"

// FEAT 명령
#define FTP_FEAT_RESPONSE			"211-Features:\r\n MDTM\r\n PASV\r\n REST STREAM\r\n SIZE\r\n211 End\r\n"

typedef enum {
	PASV,	// 0
	LIST,	// 1
	RETR,	// 2
	STOR	// 3
}ftp_dataport_command;

typedef enum {
	DP_IDLE = 0,
	DP_LIST_START,	// 1
	DP_LIST_ING1,	// 2
	DP_LIST_ING2,	// 3
	DP_LIST_ING3,	// 4
	DP_LIST_END,	// 5
	DP_StoC_START,	// 6
	DP_StoC_ING1,	// 7
	DP_StoC_ING2,	// 8
	DP_StoC_END,	// 9
	DP_CtoS_START,	// 10
	DP_CtoS_ING1,	// 11
	DP_CtoS_ING2,	// 12
	DP_CtoS_END		// 13
}ftp_dataport_state;

typedef struct ftp_state {
	FIL fp;										// FAT
	DIR dir;									// FAT
	UINT DirLocation;							// 0:root, 1:FLS or SD
	UINT LoginState;							// User Login State
	char strNowdirpath[WORK_BUFFER_SIZE];		// PWD
	char strSelectfilepath[WORK_BUFFER_SIZE];	// RNFR, RNTO
	UINT nSelectfilepathState;					// RNFR, RNTO
	UINT is_root;								// LIST
	UINT resend_pos;							// REST(RETR, STOR)
	struct tcp_pcb *pcb;						// CONTROL port
	UINT dpindex;								// DATA port index

	struct ftp_state *next;						// ftp_state list
}ftp_state;

extern void FtpdStart(void);
extern void FtpdStop(void);

#endif
