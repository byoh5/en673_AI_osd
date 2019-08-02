//*************************************************************************************************
// User shell commands
//*************************************************************************************************
#include <stdlib.h>		// for atoi

#include "dev.h"
#include "dev_inline.h"	// to use fast code
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sys_arch.h"

#include "shell.h"
#include "shell_cmd_isp.h"

#include "msg.h"
#include "user.h"

extern UINT gnISP_CtrState;

const char *sIspControl[]   = {"Control ISP",                                    (char*)0};
//const char *sIspOSDControl[]   = {"Control OSD",                                    (char*)0};

//const char *sISPControlM[]  = {"Control ISP(menu)",                              (char*)0};
#if (LVDS_MARGIN_TEST==1)
const char *sLvdsCmd[]      = {"Lvds timing adjust",                             (char*)0};
#endif

//*************************************************************************************************
// User define
//-------------------------------------------------------------------------------------------------
//
#if 0
const char *sIsp_MainList[] = {	// ---------------------------------------------------------------------	//	Main Manu
"=========================","= ISP Menu List         =","=========================",
"[ 1] IRIS                ","[ 2] FOCUS ADJ           ","[ 3] EXPOSUER            ",
"[ 4] BACKLIGHT           ","[ 5] DAYnNIGHT           ","[ 6] COLOR               ",
"[ 7] DNR                 ","[ 8] IMAGE               ","[ 9] MOTION              ",
"[10] SYSTEM              ","=========================","[ u] UP KEY              ",
"[ d] DOWN KEY            ","[ l] LEFT KEY            ","[ r] RIGHT KEY           ",
"[ c] CENTER KEY          ","[ e] MENU EXIT           ","========================="};

const char *sIsp_IrisSub[] = {	// ---------------------------------------------------------------------	//	IRIS
"=========================","= IRIS (Sub Menu)       =","=========================",
"[1] ELC                  ","[2] ALC                  ","========================="};

const char *sIsp_FadjSub[] = {	// ---------------------------------------------------------------------	//	FOCUS ADJ.
"=========================","= FOCUS ADJ (Sub Menu)  =","=========================",
"[1] OFF                  ","[2] ON [e]               ","========================="};

const char *sIsp_FadjSub_1[] = {	// FADJ. ON
"=========================","= FOCUS ADJ - ONe       =","=========================",
"[1] FOCUS AREA [1~3]     ","[2] DISP TONE [1~3]      ","[3] DISP H_POS [1~61]    ",
"[4] DISP V_POS [1~35]    ","[5] DISP H_SIZE [1~61]   ","[6] DISP V_SIZE [1~35]   ",
"========================="};

const char *sIsp_ExposureSub[] = {	// ------------------------------------------------------------------	//	EXPOSURE
"=========================","= EXPOSURE (Sub Menu)   =","=========================",
"[1] Brightness [1~21]    ","[2] SHUTTER [e]          ","[3] SENS-UP [1~6]        ",
"[4] AGC [1~11]           ","========================="};

const char *sIsp_ExposureSub_1[] = {	// SHUTTER
"=========================","= EXPOSURE - SHTe       =","=========================",
"[1] AUTO [1~2]           ","[2] MANUAL [1~11]        ","[3] FLICKER              ",
"========================="};

const char *sIsp_BacklightSub[] = {	// ------------------------------------------------------------------	//	BACKLIGHT
"=========================","= BACKLIGHT (Sub Menu)  =","=========================",
"[1] OFF                  ","[2] HLC [e]              ","[3] BLC [e]              ",
"[4] WDR [e]              ","========================="};

const char *sIsp_BacklightSub_1[] = {	// HLC
"=========================","= BACKLIGHT - HLCe      =","=========================",
"[1] LEVEL [1~21]         ","[2] COLOR [1~8]          ","========================="};

const char *sIsp_BacklightSub_2[] = {	// BLC
"=========================","= BACKLIGHT - BLCe      =","=========================",
"[1] H-POS [1~21]         ","[2] V-POS [1~21]         ","[3] H-SIZE [1~21]        ",
"[2] V-SIZE [1~21]        ","========================="};

const char *sIsp_BacklightSub_3[] = {	// WDR
"=========================","= BACKLIGHT - WDRe      =","=========================",
"[1] WEIGHT [1~3]         ","========================="};

const char *sIsp_DayNightSub[] = {	// ------------------------------------------------------------------	//	DAY&NIGHT
"=========================","= DAYnNIGHT (Sub Menu)  =","=========================",
"[1] AUTO [e]             ","[2] COLOR                ","[3] BnW [e]              ",
"[4] EXTERN [e]           ","========================="};

const char *sIsp_DayNightSub_1[] = {	// AUTO
"=========================","= DAYnNIGHT - AUTOe     =","=========================",
"[1] IR LED [1~2]         ","[2] ANTI-SAT. [1~21]     ","[3] AGC THRES [1~21]     ",
"[4] AGC MARGIN [1~21]    ","[5] DELAY [1~3]          ","========================="};

const char *sIsp_DayNightSub_2[] = {	// BW 
"=========================","= DAYnNIGHT - BWe       =","=========================",
"[1] IR LED [1~2]         ","[2] ANTI-SAT. [1~21]     ","========================="};

const char *sIsp_DayNightSub_3[] = {	// EXTERN
"=========================","= DAYnNIGHT - EXTERNe   =","=========================",
"[1] IR LED [1~2]         ","[2] ANTI-SAT. [1~21]     ","[3] EXTERN SW [1~2]      ",
"[4] D->N Thr [1~21]      ","[5] N->D Thr [1~21]      ","[6] DELAY [1~3]          ",
"========================="};

const char *sIsp_ColorSub[] = {	// ---------------------------------------------------------------------	//	COLOR
"=========================","= COLOR (Sub Menu)      =","=========================",
"[1] AWB - AUTO           ","[2] AWB - AUTOext        ","[3] AWB - PRESET         ",
"[4] AWB - MANUAL [e]     ","[5] COLOR GAIN [1~21]    ","========================="};

const char *sIsp_ColorSub_1[] = {	// MANUAL
"=========================","= COLOR - MANUALe       =","=========================",
"[1] C-TEMP [1~3]         ","[2] R-GAIN [1~21]        ","[3] B-GAIN [1~21]        ",
"========================="};

const char *sIsp_DNRSub[] = {	// ---------------------------------------------------------------------	//	DNR
"=========================","= DNR (Sub Menu)        =","=========================",
"[1] OFF                  ","[2] LOW                  ","[3] MIDDLE               ",
"[4] HIGH                 ","========================="};

const char *sIsp_ImageSub[] = {	// ---------------------------------------------------------------------	//	IMAGE
"=========================","= IMAGE (Sub Menu)      =","=========================",
"[ 1] SHARPNESS [1~11]    ","[ 2] GAMMA [1~4]         ","[ 3] MIRROR [1~2]        ",
"[ 4] FLIP [1~2]          ","[ 5] D-ZOOM [1~161)      ","[ 6] ACE [1~4]           ",
"[ 7] DEFOG [e]           ","[ 8] SHADING [e]         ","[ 9] PRIVACY [e]         ",
"[10] PIP [e]             ","[11] JPG CAPTURE [p]     ","========================="};

const char *sIsp_ImageSub_1[] = {	// DEFOG
"=========================","= IMAGE - DEFOGe        =","=========================",
"[ 1] OFF                 ","[ 2] MODE [1~2]          ","[ 3] LEVEL [1~3]         ",
"========================="};

const char *sIsp_ImageSub_2[] = {	// SHADING
"=========================","= IMAGE - SHADINGe      =","=========================",
"[ 1] OFF                 ","[ 2] WEIGHT [1~201]      ","========================="};

const char *sIsp_ImageSub_3[] = { 	// PRIVACY
"=========================","= IMAGE - PRIVACYe      =","=========================",
"[ 1] OFF                 ","[ 2] ZONE NUM [1~16]     ","[ 3] ZONE DISP [1~2]     ",
"[ 4] H-POS [1~60]        ","[ 5] V-POS [1~35]        ","[ 6] H-SIZE [1~60]       ",
"[ 7] V-SIZE [1~35]       ","[ 8] MASK MODE [1~2]     ","[ 9] Y LEVEL [1~21]      ",
"[10] CB LEVEL [1~21]     ","[11] CR LEVEL [1~21]     ","[12] TRANS [1~3]         ",
"========================="};

const char *sIsp_ImageSub_4[] = {	// PIP
"=========================","= IMAGE - PIPe          =","=========================",
"[ 1] OFF                 ","[ 2] PIP CH [1~4]        ","[ 3] H-POSE [1~126]      ",
"[ 4] V-POS [1~70]        ","[ 5] SIZE [1~101]        ","========================="};

const char *sIsp_MotionSub[] = {	// -----------------------------------------------------------------	//	MOTION
"=========================","= MOTION - ONe          =","=========================",
"[1] OFF                  ","[2] DET WINDOW [e]       ","[3] DET TONE [1~5]       ",
"[4] MDRECT FILEE [1~2]   ","[5] SENSITIVITY [1~11]   ","[6] MOTION OSD [1~2]     ",
"[7] TEXT ALARM [1~2]     ","[8] SIGNAL OUT [1~1]     ","========================="};

const char *sIsp_MotionSub_1[] = {	// Motion Det Window Set
"=========================","= MOTION - DET WINDOWe  =","=========================",
"[1] WINDOW USE [1~4]     ","[2] WINDOW ZONE [1~2]    ","[3] DET H-POS [1~61]     ",
"[4] DET V-POS [1~34]     ","[5] DET H-SIZE [1~61]    ","[6] DET V-SIZE [1~34]    ",
"========================="};

const char *sIsp_SystemSub[] = {	// -----------------------------------------------------------------	//	SYSTEM
"=========================","= SYSTEM (Sub Menu)     =","=========================",
"[ 1] COMMUNICATION [e]   ","[ 2] IMAGE RANGE [e]     ","[ 3] COLOR SPACE [1~3]   ",
"[ 4] FRAME RATE [1~3]    ","[ 5] FREQ [1~2]          ","[ 6] CVBS [1~2]          ",
"[ 7] AUDIO MIC [1~2]     ","[ 8] COLOR BAR [1~2]     ","[ 9] LANGUAGE [1~5]      ",
"[10] CAM TITLE [e]       ","[11] RESET [p]           ","[12] Menu Save [p]       ",
"========================="};

const char *sIsp_SystemSub_1[] = {	// COM.
"=========================","= SYSTEM - COMe          =","=========================",
"[ 1] CAM ID [1~255]      ","[ 2] BAUDRATE [1~5]      ","[ 3] SET DONE [p]        ",
"========================="};

const char *sIsp_SystemSub_2[] = {	// IMAGE RANGE.
"=========================","= SYSTEM - IMAGE RANGEe =","=========================",
"[ 1] FULL                ","[ 2] COMP                ","[ 3] USER [1~21]         ",
"========================="};

const UINT gnIsp_MainListEa       = numberof(sIsp_MainList);
const UINT gnIsp_IrisSubEa        = numberof(sIsp_IrisSub);
const UINT gnIsp_FadjSubEa        = numberof(sIsp_FadjSub);
const UINT gnIsp_FadjSub_1Ea      = numberof(sIsp_FadjSub_1);
const UINT gnIsp_ExposureSubEa    = numberof(sIsp_ExposureSub);
const UINT gnIsp_ExposureSub_1Ea  = numberof(sIsp_ExposureSub_1);
const UINT gnIsp_BacklightSubEa   = numberof(sIsp_BacklightSub);
const UINT gnIsp_BacklightSub_1Ea = numberof(sIsp_BacklightSub_1);
const UINT gnIsp_BacklightSub_2Ea = numberof(sIsp_BacklightSub_2);
const UINT gnIsp_BacklightSub_3Ea = numberof(sIsp_BacklightSub_3);
const UINT gnIsp_DayNightSubEa    = numberof(sIsp_DayNightSub);
const UINT gnIsp_DayNightSub_1Ea  = numberof(sIsp_DayNightSub_1);
const UINT gnIsp_DayNightSub_2Ea  = numberof(sIsp_DayNightSub_2);
const UINT gnIsp_DayNightSub_3Ea  = numberof(sIsp_DayNightSub_3);
const UINT gnIsp_ColorSubEa       = numberof(sIsp_ColorSub);
const UINT gnIsp_ColorSub_1Ea     = numberof(sIsp_ColorSub_1);
const UINT gnIsp_DNRSubEa         = numberof(sIsp_DNRSub);
const UINT gnIsp_ImageSubEa       = numberof(sIsp_ImageSub);
const UINT gnIsp_ImageSub_1Ea     = numberof(sIsp_ImageSub_1);
const UINT gnIsp_ImageSub_2Ea     = numberof(sIsp_ImageSub_2);
const UINT gnIsp_ImageSub_3Ea     = numberof(sIsp_ImageSub_3);
const UINT gnIsp_ImageSub_4Ea     = numberof(sIsp_ImageSub_4);
const UINT gnIsp_MotionSubEa      = numberof(sIsp_MotionSub);
const UINT gnIsp_MotionSub_1Ea    = numberof(sIsp_MotionSub_1);
const UINT gnIsp_SystemSubEa      = numberof(sIsp_SystemSub);
const UINT gnIsp_SystemSub_1Ea    = numberof(sIsp_SystemSub_1);
const UINT gnIsp_SystemSub_2Ea    = numberof(sIsp_SystemSub_2);
#endif

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
INT16S cmd_ispControl(INT32S argc, char *argv[])
{
	if(argc == 2)
	{
		UINT input = atoi(argv[1]);
		switch(input)
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				MP(FrameMode) = input;
				break;
			default:
				Shell_Unknown();
				break;
		}
		vTaskDelay(10);
	}

	char output_set[6][6] = {"30NOR", "15WDR", "30DNR", "60NOR", "30WDR", "60DNR"};
	printf("MP(FrameMode)    = %d(%s)\r\n", MP(FrameMode), output_set[MP(FrameMode)]);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}


static UINT CGI_isp_BYTE_MAX_MIN_(volatile BYTE *unByte, char *pcValue, BYTE nMax, BYTE nMin)
{
	UINT bRes = DEF_FAIL;
	BYTE unValue = atoi(pcValue);
	if(*unByte != unValue)
	{
		if(nMax < nMin)
		{
			UINT nTemp = nMax;
			nMax = nMin;
			nMin = nTemp;
		}
		if(unValue < nMin)		unValue = nMin;
		else if(unValue > nMax)	unValue = nMax;
		*unByte = unValue;
		bRes = DEF_OK;
	}
	return bRes;
}


INT16S cmd_ispOSDControl(INT32S argc, char *argv[])
{
	printf("argc %d\r\n",argc);

	int i=0; 
	for(i=0;i<argc;i++){
		printf("%d : %s \r\n",i,argv[i]);
	}
	
	if(argc == 7){
		int nNum = atoi(argv[1]);
		CGI_isp_BYTE_MAX_MIN_(&(gbMnPvcCfg(nNum)->bAction), argv[2], 1, 0);
		CGI_isp_BYTE_MAX_MIN_(&(gbMnPvcCfg(nNum)->bPosX), argv[3], 60, 0);
		CGI_isp_BYTE_MAX_MIN_(&(gbMnPvcCfg(nNum)->bPosY), argv[4], 34, 0);
		CGI_isp_BYTE_MAX_MIN_(&(gbMnPvcCfg(nNum)->bSizX), argv[5], 60, 0);
		CGI_isp_BYTE_MAX_MIN_(&(gbMnPvcCfg(nNum)->bSizY) , argv[6], 34, 0);
	}
	
	return 0;

	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_FontOsdControl(INT32S argc, char *argv[])
{
	printf("argc %d\r\n",argc);

	int i=0; 
	for(i=0;i<argc;i++){
		printf("%d : %s \r\n",i,argv[i]);
	}
	
	txtosd_info osd_data;
	osd_data.active = atoi(argv[1]);
	strcpy(osd_data.info,argv[2]);		 
	osd_data.posX = atoi(argv[3]);
	osd_data.posY = atoi(argv[4]);
	while(MsgTxtOsdPut( &gptMsgTxtOsd, &osd_data) == DEF_FAIL)
	vTaskDelay(1);	

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_TXTOCD, 0) == DEF_FAIL)
	vTaskDelay(1);

	
	return 0;

	UNUSED(argc);
	UNUSED(argv);
}


INT16S cmd_GetMD(INT32S argc, char *argv[])
{
	printf("argc %d\r\n",argc);
	
		int i=0; 
		for(i=0;i<argc;i++){
			printf("%d : %s \r\n",i,argv[i]);
		}
	printf("MOTION %d\n",gptMsgISPTbl.ISP_MOTION_REF);


}

INT16S cmd_GetClr(INT32S argc, char *argv[])
{
	printf("argc %d\r\n",argc);
	
		int i=0; 
		for(i=0;i<argc;i++){
			printf("%d : %s \r\n",i,argv[i]);
		}
	if(argc == 3){	
	UINT ox = atoi(argv[1]);
 	UINT oy = atoi(argv[2]); 
	UINT offsetY = ((oy - 1)*ox)+ ox;
	UINT offsetCb =((offsetY/2)&0xfffffff7);
	UINT offsetCr = offsetCb+1;
	BYTE* posY = (BYTE*) H264_CURYBUF;
	BYTE* posC = (BYTE*) H264_CURCBUF;
	printf("Y=%d Cb=%d Cr=%d \n",*(posY+offsetY),*(posC+offsetCb),*(posC+offsetCr));
	UINT red 	= 1.164*(*(posY+offsetY)-16) + 1.596*(*(posC+offsetCr)-128);
	UINT green 	= 1.164*(*(posY+offsetY)-16) - 0.813*(*(posC+offsetCb)-128) - 0.392*(*(posC+offsetCr)-128);
	UINT blue 	= 1.164*(*(posY+offsetY)-16) + 2.017*(*(posC+offsetCb)-128);

	printf("r=%d g=%d b=%d \n",red,green,blue);
	}
	
}

#if 0
INT16S cmd_ispControl_menu(INT32S argc, char *argv[])
{		
	if(argc != 0)
	{		
		if(argc == 1)	// -----------------------------------------------------------------------------------------	//	Main Manu
		{
			DispIspMenu(Isp_MainList);
			
			printf("argc : %d argv : %s\r\n", argc, argv[0]);
		}
		else if(argc >= 2)
		{
			if(STRCMP1(argv[1], "1"))	// -------------------------------------------------------------------------	//	IRIS
			{
				DispIspMenu(Isp_IrisSub);
				
				if(argc == 3)
				{
					gnISP_CtrState = (STRCMP2(argv[2], "1", "2")) ? SHM0_REG_SET(0,atoi(argv[1]),0,0,0,atoi(argv[2])) : 0;
				}
			}
			else if(STRCMP1(argv[1], "2"))	// ---------------------------------------------------------------------	//	FOCUS ADJ.
			{
				DispIspMenu(Isp_FadjSub);
				
				if(argc >= 3)
				{					
					if(STRCMP1(argv[2], "2")) { DispIspMenu(Isp_FadjSub_1); }
					
					if     (argc == 3) gnISP_CtrState = (STRCMP2(argv[2], "1", "2")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0) : 0;
					else if(argc == 5) {
						if(STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						else if(STRCMP2(argv[3], "5", "6"))      gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
					}
				}
			}
			else if(STRCMP1(argv[1], "3"))	// ---------------------------------------------------------------------	//	AUTO EXPOSURE
			{
				DispIspMenu(Isp_ExposureSub);

				if(argc >= 3) {

					if(STRCMP1(argv[2], "2")) {	DispIspMenu(Isp_ExposureSub_1);	}
					
					if     (argc == 4) gnISP_CtrState = (STRCMP4(argv[2], "1", "2", "3", "4")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])) : 0;
					else if(argc == 5) {

						if(STRCMP1(argv[2], "2")) {
							
							if(STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP2(argv[3], "5", "6"))      gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "4"))	// ---------------------------------------------------------------------	//	BACKLIGHT
			{
				DispIspMenu(Isp_BacklightSub);
				
				if(argc >= 3) {
					
					if     (STRCMP1(argv[2], "2")) { DispIspMenu(Isp_BacklightSub_1); }	// HLC
					else if(STRCMP1(argv[2], "3")) { DispIspMenu(Isp_BacklightSub_2); }	// BLC
					else if(STRCMP1(argv[2], "4")) { DispIspMenu(Isp_BacklightSub_3); }	// WDR

					if     (argc == 3) gnISP_CtrState = (STRCMP4(argv[2], "1", "2", "3", "4")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0) : 0;
					else if(argc == 5) {

						if(STRCMP1(argv[2], "2")) {	// HLC - Sub
							
							if(STRCMP2(argv[3], "1", "2")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "3")) {	// BLC - Sub
							
							if(STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "4")) {	// WDR - Sub
							
							if(STRCMP1(argv[3], "1")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "5"))	// ---------------------------------------------------------------------	//	DAY&NIGHT
			{
				DispIspMenu(Isp_DayNightSub); 
				
				if(argc >= 3) {
					
					if     (STRCMP1(argv[2], "1")) { DispIspMenu(Isp_DayNightSub_1); }	// AUTO
//						else if(STRCMP1(argv[2], "2"))	// COLOR - No Sub Menu
					else if(STRCMP1(argv[2], "3")) { DispIspMenu(Isp_DayNightSub_2); }	// BW
					else if(STRCMP1(argv[2], "4")) { DispIspMenu(Isp_DayNightSub_3); }	// EXTERN

					if     (argc == 3) gnISP_CtrState = (STRCMP4(argv[2], "1", "2", "3", "4")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0) : 0;
					else if(argc == 5) {

						if(STRCMP1(argv[2], "1")) {	// AUTO - Sub
							
							if(STRCMP3(argv[3], "1", "2", "3")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP2(argv[3], "4", "5")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "3")) {	// BW - Sub
							
							if(STRCMP2(argv[3], "1", "2")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "4")) {	// EXTERN - Sub
							
							if(STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP2(argv[3], "5", "6"))      gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "6"))	// ---------------------------------------------------------------------	//	AUTO WHITE BALANCE
			{
				DispIspMenu(Isp_ColorSub); 
				
				if(argc >= 3) {
					
					if(STRCMP1(argv[2], "4")) { DispIspMenu(Isp_ColorSub_1); }	// MANUAL

					if     (argc == 3) gnISP_CtrState = (STRCMP4(argv[2], "1", "2", "3", "4")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0) : 0;
					else if(argc == 4) {	// COLOR GAIN - Sub

						if(STRCMP1(argv[2], "5")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3]));
					}
					else if(argc == 5) {

						if(STRCMP1(argv[2], "4")) {	// AWB MODE - Sub
							
							if(STRCMP3(argv[3], "1", "2", "3")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "7"))	// ---------------------------------------------------------------------	//	DNR
			{   
				DispIspMenu(Isp_DNRSub); 
				
				if(argc == 3)
				{
					gnISP_CtrState = (STRCMP4(argv[2], "1", "2", "3", "4")) ? SHM0_REG_SET(0,atoi(argv[1]),0,0,0,atoi(argv[2])) : 0;
				}
			}
			else if(STRCMP1(argv[1], "8"))	// ---------------------------------------------------------------------	//	IMAGE
			{
				DispIspMenu(Isp_ImageSub);
				
				if(argc >= 3) {
					
					if     (STRCMP1(argv[2], "7" )) { DispIspMenu(Isp_ImageSub_1); }	// DEFOG - Sub
					else if(STRCMP1(argv[2], "8" )) { DispIspMenu(Isp_ImageSub_2); }	// SHADING - Sub
					else if(STRCMP1(argv[2], "9" )) { DispIspMenu(Isp_ImageSub_3); }	// PRIVACY - Sub
					else if(STRCMP1(argv[2], "10")) { DispIspMenu(Isp_ImageSub_4); }	// PIP - Sub

					if(argc == 4) {
						if(STRCMP4(argv[2], "1", "2", "3", "4"))       gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // Shp,gamm,mirr,flip
						else if(STRCMP2(argv[2], "5", "6"))            gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // D-Zoom,Ace
						else if(STRCMP4(argv[2], "7", "8", "9", "10")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,0); // Dfog,Shd,Priv,PIP
					}
					else if(argc == 5) {

						if(STRCMP1(argv[2], "7")) {	// DEFOG - Sub
							
							if(STRCMP3(argv[3], "1", "2", "3")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "8")) {	// SHAD - Sub
							
							if(STRCMP2(argv[3], "1", "2")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "9")) {	// PRIVACY - Sub
							
							if     (STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP4(argv[3], "5", "6", "7", "8")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP4(argv[3], "9", "10", "11", "12")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
						else if(STRCMP1(argv[2], "10")) {	// PIP - Sub
							
							if(STRCMP3(argv[3], "1", "2", "3")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP2(argv[3], "4", "5")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "9"))	// ---------------------------------------------------------------------	//	MOTION
			{
				DispIspMenu(Isp_MotionSub);
				
				if(argc >= 3) {
					
					if(STRCMP1(argv[2], "2" )) { DispIspMenu(Isp_MotionSub_1); }	// Motion - Sub
								
					if(argc == 3) {
						if     (STRCMP4(argv[2], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0);
						else if(STRCMP4(argv[2], "5", "6", "7", "8")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0);
					}
					else if(argc == 4) {
						if(STRCMP4(argv[2], "3", "4", "5", "6")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // Tone,Mdr,Sens,OSD
						else if(STRCMP2(argv[2], "7", "8"))      gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // Text Alarm,Signal Out
					}
					else if(argc == 5) {

						if(STRCMP1(argv[2], "2")) {	// Det Set - Sub
							
							if(STRCMP4(argv[3], "1", "2", "3", "4")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
							else if(STRCMP2(argv[3], "5", "6"))      gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4]));
						}
					}
				}
			}
			else if(STRCMP1(argv[1], "10"))	// ---------------------------------------------------------------------	//	SYSTEM
			{    
				DispIspMenu(Isp_SystemSub);
				
				if(argc >= 3) {
					
					if     (STRCMP1(argv[2], "1" )) { DispIspMenu(Isp_SystemSub_1); }	// Com - Sub
					else if(STRCMP1(argv[2], "2" )) { DispIspMenu(Isp_SystemSub_2); }	// Img Range - Sub
					
					if     (argc == 3) gnISP_CtrState = (STRCMP2(argv[2], "11", "12")) ? SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,0) : 0; // Reset, Menu Save
					else if(argc == 4)
					{
						if     (STRCMP4(argv[2], "3", "4", "5", "6" )) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // C-Space,FrameRate,FREQ,CVBS
						else if(STRCMP4(argv[2], "7", "8", "9", "10")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),0,0,atoi(argv[3])); // Audio,C-Bar,Language,Cam Title
					}
					else if(argc == 5)
					{
						if(STRCMP2(argv[2], "1", "2")) gnISP_CtrState = SHM0_REG_SET(0,atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),0,atoi(argv[4])); // Com Set,Img Range(User)
					}
				}
			}

			// -----------------------------------------------------------------------------------------------------	//	KEY MANAGEMENT
 			else if(STRCMP1(argv[1], "u")) gptMsgISPTbl.ISP_UKEY = 0x01;	//	Up    
 			else if(STRCMP1(argv[1], "d")) gptMsgISPTbl.ISP_UKEY = 0x02;	//	Down  
 			else if(STRCMP1(argv[1], "l")) gptMsgISPTbl.ISP_UKEY = 0x08;	//	Left  
 			else if(STRCMP1(argv[1], "r")) gptMsgISPTbl.ISP_UKEY = 0x04;	//	Right 
 			else if(STRCMP1(argv[1], "c")) gptMsgISPTbl.ISP_UKEY = 0x10;	//	Center
 			else if(STRCMP1(argv[1], "e")) gnISP_CtrState=0x00B60000;	//	Exit
 			
 			// -----------------------------------------------------------------------------------------------------	//	Wrong Input
			else
			{
				printf("Wrong input...\r\n");
			}
			
			if(argc == 2) printf("argc : %d argv : %s %s\r\n", argc, argv[0], argv[1]);
			else          printf("argc : %d argv : %s %s %s\r\n", argc, argv[0], argv[1], argv[2]);
		}
	}
	else
	{
//			gptMsgShare.TIME
//			gptMsgShare.TIMEZONE
		printf("Wrong input...\r\n");
	}
	return 0;
	
	UNUSED(argc);
	UNUSED(argv);
}
#endif

#if (LVDS_MARGIN_TEST==1)
INT16S cmd_lvds(INT32S argc, char *argv[])
{
	if(argc>1 && strcmp(argv[1], "off")==0) 
		while( MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_LVDS, 0) == DEF_FAIL );
	else {
		while( MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_LVDS, 1) == DEF_FAIL );		
		printf("Lvds timing adjust start. Please wait a minute. \r\n");
	}
	vTaskDelay(1);	
	return 0;
	
	UNUSED(argc);
	UNUSED(argv);
}
#endif

#if 0
INT16S cmd_isp_test(INT32S argc, char *argv[])
{
	if(strcmp(argv[1], "dnr") == 0)
	{
		char *dnr_mode[] = {"OFF", "LOW", "MIDDLE", "HIGH", "ERROR"};
		char *pdnr = NULL;
		if(argc == 2)
		{
			switch(MP(MpAdnr))
			{
				case 0:
				case 1:
				case 2:
				case 3:
					pdnr = dnr_mode[MP(MpAdnr)];
					break;
				default:
					pdnr = dnr_mode[4];
					break;
			}
			printf("DNR mode Get : %s\r\n", pdnr);
		}
		else if(argc == 3)
		{
			int nInput = atoi(argv[2]);
			switch(nInput)
			{
				case 0:
				case 1:
				case 2:
				case 3:
					pdnr = dnr_mode[nInput];
					MP(MpAdnr) = nInput;
					break;
				default:
					pdnr = dnr_mode[4];
					break;
			}
			printf("DNR mode Set : %s\r\n", pdnr);
		}
		else
		{
			Shell_Unknown();
		}
	}
	else
	{
		Shell_Unknown();
	}

#if 0
	if(argc == 1)
	{
		printf("1ch setting ========================\r\n");
		printf("¦§ISP_ISEL		   : %d\r\n", ISP_ISEL);
		printf("¦§ISP_HWSEL 	   : %d\r\n", ISP_HWSEL);
		printf("¦§ISP_IVSEL 	   : %d\r\n", ISP_IVSEL);
		printf("¦§ISP_BT_DS_HWI0   : %d\r\n", ISP_BT_DS_HWI0);
		printf("¦§ISP_ISP_YADR_P0  : %d(0x%08X)\r\n", ISP_ISP_YADR_P0, ISP_ISP_YADR_P0);
		printf("¦¦ISP_ISP_CADR_P0  : %d(0x%08X)\r\n", ISP_ISP_CADR_P0, ISP_ISP_CADR_P0);

		printf("2ch setting ========================\r\n");
		printf("¦§ISP_BT0_ISEL	   : %d\r\n", ISP_BT0_ISEL);
		printf("¦§ISP_BT0_HWSEL    : %d\r\n", ISP_BT0_HWSEL);
		printf("¦§ISP_BT0_VISEL    : %d\r\n", ISP_BT0_VISEL);
		printf("¦§ISP_BT_DS_HWI1   : %d\r\n", ISP_BT_DS_HWI1);
		printf("¦§ISP_YADR0_P0	   : %d(0x%08X)\r\n", ISP_YADR0_P0, ISP_YADR0_P0);
		printf("¦¦ISP_YADR0_P0	   : %d(0x%08X)\r\n", ISP_CADR0_P0, ISP_CADR0_P0);

		printf("ds0 setting =========================\r\n");
		printf("¦§ISP_CDS0_PCK_SEL : %d\r\n", ISP_CDS0_PCK_SEL);
		printf("¦§ISP_CDS0_ISEL    : %d\r\n", ISP_CDS0_ISEL);
		printf("¦§ISP_DS0_DKX	   : %d\r\n", ISP_DS0_DKX);
		printf("¦§ISP_DS0_DKY	   : %d\r\n", ISP_DS0_DKY);
		printf("¦§ISP_DS0_HW	   : %d\r\n", ISP_DS0_HW);
		printf("¦§ISP_DS0_VW	   : %d\r\n", ISP_DS0_VW);
		printf("¦¦ISP_DS0_ON	   : %d\r\n", ISP_DS0_ON);

		printf("ds1 setting =========================\r\n");
		printf("¦§ISP_CDS1_PCK_SEL : %d\r\n", ISP_CDS1_PCK_SEL);
		printf("¦§ISP_CDS1_ISEL    : %d\r\n", ISP_CDS1_ISEL);
		printf("¦§ISP_DS1_DKX	   : %d\r\n", ISP_DS1_DKX);
		printf("¦§ISP_DS1_DKY	   : %d\r\n", ISP_DS1_DKY);
		printf("¦§ISP_DS1_HW	   : %d\r\n", ISP_DS1_HW);
		printf("¦§ISP_DS1_VW	   : %d\r\n", ISP_DS1_VW);
		printf("¦¦ISP_DS1_ON	   : %d\r\n", ISP_DS1_ON);
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1], "in") == 0)
		{
			UINT insel = atoi(argv[2]);
			UINT insel_t = ISP_INSEL;
			printf("OLD INSEL_T : 0x%X\r\n", insel_t);
			ISP_INSEL = insel;
			printf("NEW INSEL_T : 0x%X\r\n", ISP_INSEL);
		}
	}
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
#endif
