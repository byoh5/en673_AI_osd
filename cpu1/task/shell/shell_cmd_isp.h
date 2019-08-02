#ifndef _SHELL_ISP_H_
#define _SHELL_ISP_H_

#include "shell.h"


//*************************************************************************************************
// Define & Variable
//-------------------------------------------------------------------------------------------------
//
#if 0
#define numberof(Arr)			(sizeof(Arr)/sizeof(Arr[0]))

#define	SHM0_REG_SET(CMD,MLV0,MLV1,MLV2,MLV3,DATA) 	(((UINT)CMD<<24)+((UINT)MLV0<<20)+((UINT)MLV1<<16)+((UINT)MLV2<<12)+((UINT)MLV3<<8)+((UINT)DATA<<0))

#define DispIspMenu(TITLE)				UINT i=0;\
	                        			for(i=0;i<gn##TITLE##Ea;i++) printf("%25s\r\n", s##TITLE[i]);

#define STRCMP1(AGSTR,T1)				(strcmp(AGSTR, T1) == 0)
#define STRCMP2(AGSTR,T1,T2)			(STRCMP1(AGSTR, T1)|STRCMP1(AGSTR, T2))
#define STRCMP3(AGSTR,T1,T2,T3)			(STRCMP1(AGSTR, T1)|STRCMP2(AGSTR, T2, T3))
#define STRCMP4(AGSTR,T1,T2,T3,T4)		(STRCMP1(AGSTR, T1)|STRCMP3(AGSTR, T2, T3, T4))
#endif


//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//



//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern INT16S cmd_ispControl(INT32S argc, char *argv[]);
extern INT16S cmd_ispOSDControl(INT32S argc, char *argv[]);
extern INT16S cmd_FontOsdControl(INT32S argc, char *argv[]);
extern INT16S cmd_GetClr(INT32S argc, char *argv[]);
extern INT16S cmd_GetMD(INT32S argc, char *argv[]);


//extern INT16S cmd_ispControl_menu(INT32S argc, char *argv[]);
#if (LVDS_MARGIN_TEST==1)
extern INT16S cmd_lvds(INT32S argc, char *argv[]);
#endif

//-------------------------------------------------------------------------------------------------
// Variable
extern const char *sIspControl[];
//extern const char *sISPControlM[];
#if (LVDS_MARGIN_TEST==1)
extern const char *sLvdsCmd[];
#endif

#endif
