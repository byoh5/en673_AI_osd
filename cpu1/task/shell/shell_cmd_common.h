#ifndef _SHELL_USR_COMMON_H_
#define _SHELL_USR_COMMON_H_

#include "shell.h"


//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern INT16S UsrCmd0(INT32S argc, char *argv[]);
extern INT16S UsrCmd1(INT32S argc, char *argv[]);
extern INT16S UsrCmd2(INT32S argc, char *argv[]);
extern INT16S UsrCmd3(INT32S argc, char *argv[]);
extern INT16S UsrCmd4(INT32S argc, char *argv[]);
extern INT16S UsrCmd5(INT32S argc, char *argv[]);
extern INT16S cmd_info(INT32S argc, char *argv[]);
extern INT16S UsrCheckInterface(INT32S argc, char *argv[]);
extern INT16S UsrGPIOState(INT32S argc, char *argv[]);
extern INT16S UseReboot(INT32S argc, char *argv[]);
#ifdef __AUDIO__
extern INT16S UsrAudioCmd(INT32S argc, char *argv[]);
#endif
#ifdef __HW_OSD__
extern INT16S cmd_fontosd(INT32S argc, char *argv[]);
#endif

extern INT16S cmd_h264_info(INT32S argc, char *argv[]);
extern INT16S cmd_h264(INT32S argc, char *argv[]);
extern INT16S cmd_jpeg_info(INT32S argc, char *argv[]);

extern INT16S cmd_time(INT32S argc, char *argv[]);
extern INT16S cmd_task_stst(INT32S argc, char *argv[]);
extern INT16S cmd_mem_stst(INT32S argc, char *argv[]);
extern INT16S cmd_xsr_stst(INT32S argc, char *argv[]);
extern INT16S cmd_envvar(INT32S argc, char *argv[]);
extern INT16S cmd_userarea(INT32S argc, char *argv[]);
extern INT16S cmd_userinfo(INT32S argc, char *argv[]);

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sUsrCmd0[];
extern const char *sUsrCmd1[];
extern const char *sUsrCmd2[];
extern const char *sUsrCmd3[];
extern const char *sUsrCmd4[];
extern const char *sUsrCmd5[];
extern const char *sInfo[];
extern const char *sUsrCkIf[];
extern const char *sUsrGPIOState[];
extern const char *sReboot[];
#ifdef __AUDIO__
extern const char *sAudioCmd[];
#endif
extern const char *sTimeCmd[];
extern const char *sTaskStatus[];
extern const char *sMemStatus[];
extern const char *sXsrStatus[];
extern const char *sEnvvar[];
extern const char *sUserAreaCmd[];
extern const char *sUserInfoCmd[];
#ifdef __HW_OSD__
extern const char *sFontOSDCmd[];
#endif
extern const char *sH264Info[];
extern const char *sH264CH1[];
extern const char *sH264CH2[];
extern const char *sJpegInfo[];

#endif
