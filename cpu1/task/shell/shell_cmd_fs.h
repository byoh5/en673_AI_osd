#ifndef _SHELL_USR_FS_H_
#define _SHELL_USR_FS_H_

#include "shell.h"


//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
#ifdef __FAT_LOAD__
extern INT16S UsrDiskInitCmd(INT32S argc, char *argv[]);
extern INT16S UsrLSCmd(INT32S argc, char *argv[]);
extern INT16S UsrFormatCmd(INT32S argc, char *argv[]);
extern INT16S UsrMKRMDIRCmd(INT32S argc, char *argv[]);
extern INT16S UsrRMCmd(INT32S argc, char *argv[]);
extern INT16S UsrPWDCmd(INT32S argc, char *argv[]);
extern INT16S UsrCDCmd(INT32S argc, char *argv[]);
extern INT16S UsrFCatCmd(INT32S argc, char *argv[]);
extern INT16S UsrFCreateCmd(INT32S argc, char *argv[]);
extern INT16S UsrFCopyCmd(INT32S argc, char *argv[]);
extern INT16S UsrFstatCmd(INT32S argc, char *argv[]);
extern INT16S UsrFileHash(INT32S argc, char *argv[]);
#ifdef __DEVICE_SD__
extern INT16S UsrSDCardSpeedTestCmd(INT32S argc, char *argv[]);
extern INT16S UsrMakeaviCmd(INT32S argc, char *argv[]);
#endif
extern INT16S UseFatTest(INT32S argc, char *argv[]);
#endif

//-------------------------------------------------------------------------------------------------
// Variable
#ifdef __FAT_LOAD__
extern const char *sUsrLSCmd[];
extern const char *sDiskInitCmd[];
extern const char *sFormatCmd[];
extern const char *sMKDIRCmd[];
extern const char *sRMDIRCmd[];
extern const char *sRMCmd[];
extern const char *sPWDCmd[];
extern const char *sCDCmd[];
extern const char *sFCatCmd[];
extern const char *sFCreateCmd[];
extern const char *sFCopyCmd[];
extern const char *sFstatCmd[];
extern const char *sFileHash[];
#ifdef __DEVICE_SD__
extern const char *sSDCardSpeedTestCmd[];
extern const char *sMakeaviCmd[];
#endif
extern const char *sFatTestCmd[];
#endif

#endif