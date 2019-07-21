#ifndef _FILE_H_
#define _FILE_H_

#include "ff.h"

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
#define FAT_BLOCK_FLS 4096
#define FAT_BLOCK_MMC 32768
#define FAT_BLOCK_MEM 4096

#define MAX_FILE_NUM 9999

#define MMC_DIR_ROOT_PATH	"sdcard"	// http(web)
#define MMC_DIR_LS_XML		"ls.xml"	// http(web)
#define MMC_DIR_LS_HTML		"ls.html"	// http(web)

#define MMC_DIR_NOR			"NORMAL"	// avneic
#define MMC_DIR_EVT			"EVENT"		// avienc
#define MMC_DIR_SNAP		"SNAPSHOT"	// avienc

#define FLS_DIR_WEB_PATH	"WEBFILE"	// http(web)
#define FLS_DIR_LOG_PATH	"LOG"
#define FLS_DIR_CFG_PATH	"CFG"
#define FLS_FILE_WIFI_INI	"wificfg.ini"	// Wi-Fi Config(bin)

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern const char * put_rc (FRESULT rc);

extern FRESULT drive_init(DriveNum dnNum);
extern FRESULT drive_in(DriveNum dnNum);
extern FRESULT drive_out(DriveNum dnNum);

extern FRESULT fat_mkdir(DriveNum dnNum, char *strPath);
extern BYTE fat_get_freesize(DriveNum dnNum);
extern FRESULT fat_ls(char *position);
extern FRESULT fat_sdmkfs(DriveNum dnNum, int au);

//-------------------------------------------------------------------------------------------------
// Variable

#endif // _FILE_H_
