#include <stdio.h>
#include <stdlib.h>																// for atoi
#include <malloc.h>																// for mallinfo

/* ------------------------ EN673 includes ------------------------------- */
#include "dev.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_inline.h"	// to use fast code
#include "factory.h"
#include "develop.h"
#include "user.h"
#include "msg.h"
#include "init.h"																// for LoadDefUserInfo

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ Project includes ------------------------------ */
#include "httpd.h"
#include "cgifuncs.h"
#include "cgissi.h"

/* ------------------------------------------------------------------------ */
#if LWIP_HTTPD_SSI

static void SSI_isp_state(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n");
	UNUSED(pcInsert);
	UNUSED(iInsertLen);
}

static void SSI_isp_iris(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tIRIS_MODE=%d;\n"
		"\tIRIS_VAR=%d;\n"
		"\tIRIS_LEVEL=%d;\n", 
		model_Iris, MP(MpIris), MP(MpIrisLvl));
}

#if model_Lens == 0
static void SSI_isp_focusadj(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tFOCUS_MODE=%d;\n"
		"\tFOCUS_AREA=%d;\n"
		"\tFOCUS_TONE=%d;\n"
		"\tFOCUS_HPOS=%d;\n"
		"\tFOCUS_VPOS=%d;\n"
		"\tFOCUS_HSIZE=%d;\n"
		"\tFOCUS_VSIZE=%d;\n",
		MP(MpFad_On), MP(MpFad_Zone), MP(MpFad_Trans), MP(MpFad_PosX), MP(MpFad_PosY), MP(MpFad_SizX), MP(MpFad_SizY));
}
#elif model_Lens == 1
static void SSI_isp_2motor(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tTMT_KEY=%d;\n"
		"\tTMT_VALUE=%d;\n" 
		"\tTMT_MODE=%d;\n"
		"\tTMT_SYNC=%d;\n"
		"\tTMT_ZTA=%d;\n"
		"\tTMT_ZSP=%d;\n"
		"\tTMT_ZEP=%d;\n"
		"\tTMT_FTA=%d;\n"
		"\tTMT_FSP=%d;\n"
		"\tTMT_FEP=%d;\n",	
		gptMsgISPTbl.KEY_DIRECTION, gptMsgISPTbl.KEY_VALUE, MP(MpMAF_mode), MP(MpMAF_Filter), MP(MpMAF_TotalPos_Z), MP(MpMAF_StartPos_Z), MP(MpMAF_EndPos_Z), MP(MpMAF_TotalPos_F),
		MP(MpMAF_StartPos_F), MP(MpMAF_EndPos_F)); // DannyLee - Need to Check
//		gptMsgISPTbl.KEY_DIRECTION, gptMsgISPTbl.KEY_VALUE, MP(MpAFmode), MP(MpScanning), MP(MpSyncTDN)); // Org
}
#endif

static void SSI_isp_exposure(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tEXPO_BRIGHTNESS=%d;\n"

		"\tEXPO_ANTI=%d;\n"
		"\tEXPO_ANTW=%d;\n"
	
		"\tEXPO_AS=%d;\n"
		
		"\tEXPO_SHUTTER=%d;\n"
		"\tEXPO_SENSUP=%d;\n"
		"\tEXPO_ACG=%d;\n"
		
		"\tEXPO_ST_MODE=%d;\n" // shutter mode
		"\tEXPO_DC_MODE=%d;\n" // iris mode
		"\tEXPO_ST_SPEED=%d;\n" // shutter speed
		"\tEXPO_IRIS=%d;\n"
		"\tEXPO_SYSFREQ=%d;\n"
		"\tEXPO_FPSHI=%d;\n",

	
		MP(MpBrightness),MP(MpAntiSatOn),MP(MpSatBrt),MP(MpAllSpeed),MP(MpShutter),MP(MpDss),MP(MpAgc),
		MP(MpShtMode),MP(MpDcMode), MP(MpShutSpd), MP(MpIris), MP(MpSysFreq), MP(MpOutFps));
}

static void SSI_isp_backlight(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tIMG_FrameMode=%d;\n"
		"\tBACKLIG_MODE=%d;\n"
		"\tBACKLIG_HLC_LV=%d;\n"
		"\tBACKLIG_HLC_CL=%d;\n"
		"\tBACKLIG_BLC_HP=%d;\n"
		"\tBACKLIG_BLC_VP=%d;\n"
		"\tBACKLIG_BLC_HS=%d;\n"
		"\tBACKLIG_BLC_VS=%d;\n"
		"\tBACKLIG_WDR_WE=%d;\n"
		"\tBACKLIG_BLC_OSD=%d;\n",	
		MP(FrameMode), 
		(MP(FrameMode) == 1 || MP(FrameMode) == 4) ? MP(MpWdrBlc) : MP(MpBackLight), MP(MpHlMaskThrs), MP(MpHlMaskColor),
		MP(MpBlcPosX), MP(MpBlcPosY), MP(MpBlcSizX), MP(MpBlcSizY), MP(MpWdrWgt),MP(MpBlcOsdOn));
}

static void SSI_isp_daynight(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tDAYNIG_MODE=%d;\n"
		"\tDAYNIG_IRLED=%d;\n"
		"\tDAYNIG_LEDSAT=%d;\n"
		"\tDAYNIG_THRES=%d;\n"
		"\tDAYNIG_MARGIN=%d;\n"
		"\tDAYNIG_EXTERN=%d;\n"
		"\tDAYNIG_DNTHRES=%d;\n"
		"\tDAYNIG_NDTHRES=%d;\n"
		"\tDAYNIG_DELAY=%d;\n",
		MP(MpTdn), MP(MpLedOn), MP(MpLedSatCtrl), MP(MpTdnThrs), MP(MpTdnMarg), 
		MP(MpTdnSw), MP(MpTdnDNThres), MP(MpTdnNDThres), MP(MpTdnDly));
}

static void SSI_isp_color(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tCOLOR_MODE=%d;\n"
		"\tCOLOR_STYLE=%d;\n"
		"\tCOLOR_PRESET=%d;\n"
		"\tCOLOR_CGAIN_R=%d;\n"
		"\tCOLOR_CGAIN_G=%d;\n"		
		"\tCOLOR_CGAIN_B=%d;\n"		
		"\tCOLOR_CTEMP=%d;\n"
		"\tCOLOR_RGAIN=%d;\n"
		"\tCOLOR_BGAIN=%d;\n",
		MP(MpAwb), MP(MpAwbStyle), MP(MpAwbPrst), MP(MpSaturationR), MP(MpSaturation), MP(MpSaturationB), MP(MpAwbMnl), MP(MpRgain), MP(MpBgain));
}

static void SSI_isp_image(char *pcInsert, int iInsertLen)
{
#define isp_image_temp 256
	char strTemp[6][isp_image_temp];
	int i;
	snprintf(strTemp[0], isp_image_temp, "\tIMG_PR_ZDISP=[");
	snprintf(strTemp[1], isp_image_temp, "\tIMG_PR_HP=[");
	snprintf(strTemp[2], isp_image_temp, "\tIMG_PR_VP=[");
	snprintf(strTemp[3], isp_image_temp, "\tIMG_PR_HS=[");
	snprintf(strTemp[4], isp_image_temp, "\tIMG_PR_VS=[");
	for(i=0; i<PVC_EA; i++)
	{
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnPvcCfg(i)->bAction); 	strcat(strTemp[0], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnPvcCfg(i)->bPosX);		strcat(strTemp[1], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnPvcCfg(i)->bPosY);		strcat(strTemp[2], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnPvcCfg(i)->bSizX);		strcat(strTemp[3], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnPvcCfg(i)->bSizY);		strcat(strTemp[4], strTemp[5]);
	}
	strcat(strTemp[0], "];\n");
	strcat(strTemp[1], "];\n");
	strcat(strTemp[2], "];\n");
	strcat(strTemp[3], "];\n");
	strcat(strTemp[4], "];\n");

	snprintf(pcInsert, iInsertLen, "\n"
		"\tIMG_FrameMode=%d;\n"
		"\tIMG_SHARP=%d;\n"
		"\tIMG_GAMMA=%d;\n"
		"\tIMG_GAMMA_DAY=%d;\n"
		"\tIMG_GAMMA_NGT=%d;\n"
		"\tIMG_MIRROR=%d;\n"
		"\tIMG_FLIP=%d;\n"
		"\tIMG_DWDR=%d;\n"
		"\tIMG_DEFOG=%d;\n"
		"\tIMG_DE_MODE=%d;\n"
		"\tIMG_DE_LEV=%d;\n"
		"\tIMG_SHADING=%d;\n"
		"\tIMG_SHA_WE=%d;\n"
		"\tIMG_PRIVACY=%d;\n"
		"\tIMG_PR_CNT=%d;\n"
		"\tIMG_PR_MODE=%d;\n"
		"\tIMG_PR_YL=%d;\n"
		"\tIMG_PR_CBL=%d;\n"
		"\tIMG_PR_CRL=%d;\n"
		"\tIMG_PR_TRA=%d;\n"
		"%s%s%s%s%s"
		"\tIMG_BOX_COLOR_STEP='%d';\n"
		"\tIMG_BOX_XMAX='%d';\n"
		"\tIMG_BOX_YMAX='%d';\n",
		MP(FrameMode),MP(MpSharpness), MP(MpGamma), MP(MpGammaDay), MP(MpGammaNgt), MP(MpMirror), MP(MpFlip), MP(MpAce), MP(MpDefog), MP(MpDefogMode), MP(MpDefogLevel),
		MP(MpShading), MP(MpShdWeight), MP(MpPrivacy), PVC_EA, MP(MpPvcFormat), MP(MpPvcYLevel), MP(MpPvcCRLevel), MP(MpPvcCBLevel) ,MP(MpPvcTrans), 
		strTemp[0], strTemp[1], strTemp[2], strTemp[3], strTemp[4], 20, 60, 34);

// IMG_GAMMA_CNT = 4 => UpGAMMA_STEP
// IMG_BOX_COLOR_STEP = 20 => UpBOX_COLOR_STEP
// IMG_BOX_XMAX = 60 => BOX_XMAX
// IMG_BOX_YMAX = 34 => BOX_YMAX
}

static void SSI_isp_motion(char *pcInsert, int iInsertLen)
{
#define MOT_WINDOW_CNT 4
#define isp_image_temp 256
	char strTemp[6][isp_image_temp];
	int i;
	snprintf(strTemp[0], isp_image_temp, "\tMOT_DET_ZDISP=[");
	snprintf(strTemp[1], isp_image_temp, "\tMOT_DET_HP=[");
	snprintf(strTemp[2], isp_image_temp, "\tMOT_DET_VP=[");
	snprintf(strTemp[3], isp_image_temp, "\tMOT_DET_HS=[");
	snprintf(strTemp[4], isp_image_temp, "\tMOT_DET_VS=[");
	for(i=0; i<MOT_WINDOW_CNT; i++)
	{
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnImdCfg(i)->bAction);	strcat(strTemp[0], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnImdCfg(i)->bPosX);		strcat(strTemp[1], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnImdCfg(i)->bPosY);		strcat(strTemp[2], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnImdCfg(i)->bSizX);		strcat(strTemp[3], strTemp[5]);
		snprintf(strTemp[5], isp_image_temp, "%d,", gbMnImdCfg(i)->bSizY);		strcat(strTemp[4], strTemp[5]);
	}
	strcat(strTemp[0], "];\n");
	strcat(strTemp[1], "];\n");
	strcat(strTemp[2], "];\n");
	strcat(strTemp[3], "];\n");
	strcat(strTemp[4], "];\n");

	snprintf(pcInsert, iInsertLen, "\n"
		"\tMOT_DET_WINDOW_SW=%d;\n"
		"\tMOT_DET_TONE=%d;\n"
		"\tMOT_MDRECT_FILL=%d;\n"
		"\tMOT_SENSITIVITY=%d;\n"
		"\tMOT_MOTION_OSD=%d;\n"
		"\tMOT_TEXT_ALARM=%d;\n"
		"\tMOT_SIGNAL_OUT=%d;\n"
		"\tMOT_WINDOW_CNT=%d;\n"
		"%s%s%s%s%s"
		"\tMOT_IMD_XMAX='%d';\n"
		"\tMOT_IMD_YMAX='%d';\n",
		MP(MpItl), MP(MpItlDettone), MP(MpItlRectFill), MP(MpItlSens), MP(MpItlMaskOsd), MP(MpItlAlarm), MP(MpItlSigno), MOT_WINDOW_CNT,
		strTemp[0], strTemp[1], strTemp[2], strTemp[3], strTemp[4], 60, 34);

// MOT_IMD_XMAX = 60 => gnIMD_Hw
// MOT_IMD_YMAX = 34 => gnIMD_Vw
}

static void SSI_isp_outputs(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tOUTPUT_MODE=%d;\n"
		"\tOUTPUT_LEV=%d;\n"
		"\tOUTPUT_WLEV=%d;\n"
		"\tOUTPUT_DLEV=%d;\n"
		"\tOUTPUT_RATE=%d;\n",
		MP(FrameMode), MP(MpFrameRateLv), MP(MpWdrWgt), MP(MpAdnr)-1, MP(MpSysFreq));
}

static void SSI_isp_system(char *pcInsert, int iInsertLen)
{
	snprintf(pcInsert, iInsertLen, "\n"
		"\tSYS_COLBAR=%d;\n"
		"\tSYS_STYLE=%d;\n"		
		"\tSYS_LANG=%d;\n"
		"\tSYS_TITLE_SW=%d;\n"
		"\tSYS_TITLE='%c%c%c%c%c%c%c%c';\n",
		MP(MpCBar), gptMsgISPTbl.MENU_STYLE, MP(MpLanguageSel), MP(MpCamTitleOn), 
		MP(MpCamTitleIdx+0) == 0 ? ' ' : MP(MpCamTitleIdx+0), MP(MpCamTitleIdx+1) == 0 ? ' ' : MP(MpCamTitleIdx+1), 
		MP(MpCamTitleIdx+2) == 0 ? ' ' : MP(MpCamTitleIdx+2), MP(MpCamTitleIdx+3) == 0 ? ' ' : MP(MpCamTitleIdx+3), 
		MP(MpCamTitleIdx+4) == 0 ? ' ' : MP(MpCamTitleIdx+4), MP(MpCamTitleIdx+5) == 0 ? ' ' : MP(MpCamTitleIdx+5), 
		MP(MpCamTitleIdx+6) == 0 ? ' ' : MP(MpCamTitleIdx+6), MP(MpCamTitleIdx+7) == 0 ? ' ' : MP(MpCamTitleIdx+7));
}

int App_ISPSSIHandler(int iIndex, char *pcInsert, int iInsertLen)
{
	switch(iIndex)
	{
		case SSI_ISP_state:
			SSI_isp_state(pcInsert, iInsertLen);
			break;
		case SSI_ISP_iris:
			SSI_isp_iris(pcInsert, iInsertLen);
			break;
#if model_Lens == 0
		case SSI_ISP_focusadj:
			SSI_isp_focusadj(pcInsert, iInsertLen);
			break;
#elif model_Lens == 1
		case SSI_ISP_2motor:
			SSI_isp_2motor(pcInsert, iInsertLen);
			break;
#endif
		case SSI_ISP_exposure:
			SSI_isp_exposure(pcInsert, iInsertLen);
			break;
		case SSI_ISP_backlight:
			SSI_isp_backlight(pcInsert, iInsertLen);
			break;
		case SSI_ISP_daynight:
			SSI_isp_daynight(pcInsert, iInsertLen);
			break;
		case SSI_ISP_color:
			SSI_isp_color(pcInsert, iInsertLen);
			break;
		case SSI_ISP_image:
			SSI_isp_image(pcInsert, iInsertLen);
			break;
		case SSI_ISP_motion:
			SSI_isp_motion(pcInsert, iInsertLen);
			break;
		case SSI_ISP_outs:
			SSI_isp_outputs(pcInsert, iInsertLen);
			break;
		case SSI_ISP_system:
			SSI_isp_system(pcInsert, iInsertLen);
			break;
		default:
			return DEF_FAIL;
	}
	return DEF_OK;
}
#endif

#if LWIP_HTTPD_CGI
static const char strUrl_submit[] = "/submit.html";
static const char strUrl_osdkey[] = "/isp/osdkey.html";

static UINT CGI_isp_BYTE_MAX_MIN(volatile BYTE *unByte, char *pcValue, BYTE nMax, BYTE nMin)
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

const char *App_ISPCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	int i;
	UINT unISPChangeFlag = 0;
	dprintf("==============================================================\r\n");
	dprintf("call %s\r\n", __func__);

	for(i=0;i<iNumParams;i++)
	{
		if(strcmp(pcParam[i], "btOK") == 0)	continue;
		dprintf("%s : %s \r\n", pcParam[i], pcValue[i]);
		///////////////////////////////////////////////////////////////
		// IRIS ///////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		if(strcmp(pcParam[i], "i_i_md") == 0)			{
			CGI_isp_BYTE_MAX_MIN(&MP(MpIris), pcValue[i], 1, 0);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_IRIS, 0) == DEF_FAIL)
				vTaskDelay(1);
			}
		else if(strcmp(pcParam[i], "i_i_lv") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpIrisLvl), pcValue[i], 20, 0);
#if model_Lens == 0
		///////////////////////////////////////////////////////////////
		// FOCUS ADJ //////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_f_md") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_On), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_f_ar") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_Zone), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_f_to") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_Trans), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_f_hp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_PosX), pcValue[i], 60, 0);
		else if(strcmp(pcParam[i], "i_f_vp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_PosY), pcValue[i], 34, 0);
		else if(strcmp(pcParam[i], "i_f_hs") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_SizX), pcValue[i], 60, 0);
		else if(strcmp(pcParam[i], "i_f_vs") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFad_SizY), pcValue[i], 34, 0);
#elif model_Lens == 1
		///////////////////////////////////////////////////////////////
		// 2-MOTOR ////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_2_kd") == 0)		CGI_isp_BYTE_MAX_MIN(&(gptMsgISPTbl.KEY_DIRECTION), pcValue[i], 4, 0);
		else if(strcmp(pcParam[i], "i_2_mv") == 0)		CGI_isp_BYTE_MAX_MIN(&(gptMsgISPTbl.KEY_VALUE), pcValue[i], 255, 0);
#if 1 // DannyLee - Need to Check
		else if(strcmp(pcParam[i], "i_2_md") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_mode), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_2_sc") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_Filter), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_2_in") == 0)		MP(MpMAF_MotorInit) = 1;
		else if(strcmp(pcParam[i], "i_2_zta") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_TotalPos_Z), pcValue[i], 255, 0);}
		else if(strcmp(pcParam[i], "i_2_zsp") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_StartPos_Z), pcValue[i], 255, 0);}
		else if(strcmp(pcParam[i], "i_2_zep") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_EndPos_Z), pcValue[i], 255, 0);}
		else if(strcmp(pcParam[i], "i_2_fta") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_TotalPos_F), pcValue[i], 255, 0);}
		else if(strcmp(pcParam[i], "i_2_fsp") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_StartPos_F), pcValue[i], 255, 0);}
		else if(strcmp(pcParam[i], "i_2_fep") == 0)		{CGI_isp_BYTE_MAX_MIN(&MP(MpMAF_EndPos_F), pcValue[i], 255, 0);}
#else // Org		
		else if(strcmp(pcParam[i], "i_2_md") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAFmode), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_2_sc") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpScanning), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_2_tn") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpSyncTDN), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_2_af") == 0)		MP(MpOnepushAF) = 1;
		else if(strcmp(pcParam[i], "i_2_in") == 0)		MP(MpOnepushInit) = 1;
#endif
#endif
		///////////////////////////////////////////////////////////////
		// EXPOSURE ///////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_e_bn") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBrightness), pcValue[i], 20, 0);

		else if(strcmp(pcParam[i], "i_e_anti") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpAntiSatOn), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_e_antw") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpSatBrt), pcValue[i], 20, 0);

		else if(strcmp(pcParam[i], "i_e_as") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAllSpeed), pcValue[i], 20, 0);
		
		else if(strcmp(pcParam[i], "i_e_st") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpShutter), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_e_su") == 0)
		{
			BYTE nMax = (MP(FrameMode) < 3) ? 5 : 6;
			CGI_isp_BYTE_MAX_MIN(&MP(MpDss), pcValue[i], nMax, 0);
		}
		else if(strcmp(pcParam[i], "i_e_agc") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAgc), pcValue[i], 255, 0);
		else if(strcmp(pcParam[i], "i_e_sm") == 0)
		{
			if(MP(MpIris) == MN_LENS_DC)
				CGI_isp_BYTE_MAX_MIN(&MP(MpDcMode), pcValue[i], 2, 0);
			else
				CGI_isp_BYTE_MAX_MIN(&MP(MpShtMode), pcValue[i], 1, 0);
		}
		else if(strcmp(pcParam[i], "i_e_sp") == 0)
		{
			BYTE nMax = (MP(FrameMode) < 3) ? 10 : 9;
			CGI_isp_BYTE_MAX_MIN(&MP(MpShutSpd), pcValue[i], nMax, 0);
		}
		///////////////////////////////////////////////////////////////
		// BACKLIGHT //////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		
				
		else if(strcmp(pcParam[i], "i_b_md") == 0)		
			if(MP(FrameMode) == 1 || MP(FrameMode) == 4){CGI_isp_BYTE_MAX_MIN(&MP(MpWdrBlc), pcValue[i], 2, 0);}
			else										{CGI_isp_BYTE_MAX_MIN(&MP(MpBackLight), pcValue[i], 2, 0);
														CGI_isp_BYTE_MAX_MIN(&MP(MpBackLightNor), pcValue[i], 2, 0);}
		else if(strcmp(pcParam[i], "i_b_hlv") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpHlMaskThrs), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_b_hcl") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpHlMaskColor), pcValue[i], 8, 0);
		else if(strcmp(pcParam[i], "i_b_osd") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBlcOsdOn), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_b_hp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBlcPosX), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_b_vp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBlcPosY), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_b_hs") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBlcSizX), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_b_vs") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBlcSizY), pcValue[i], 20, 0);
		///////////////////////////////////////////////////////////////
		// DAY&NIGHT //////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_dn_md") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdn), pcValue[i], 3, 0);
		else if(strcmp(pcParam[i], "i_dn_ir") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpLedOn), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_dn_an") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpLedSatCtrl), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_dn_at") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdnThrs), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_dn_am") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdnMarg), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_dn_et") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdnSw), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_dn_dn") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdnDNThres), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_dn_nd") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpTdnNDThres), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_dn_dl") == 0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpTdnDly), pcValue[i], 2, 0);
		///////////////////////////////////////////////////////////////
		// COLOR //////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_c_wb") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAwb), pcValue[i], 3, 0);
		else if(strcmp(pcParam[i], "i_c_st") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAwbStyle), pcValue[i], 5, 0);
		else if(strcmp(pcParam[i], "i_c_pr") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAwbPrst), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_c_cg_r") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpSaturationR), pcValue[i], 40, 0);
		else if(strcmp(pcParam[i], "i_c_cg_g") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpSaturation), pcValue[i], 40, 0);		
		else if(strcmp(pcParam[i], "i_c_cg_b") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpSaturationB), pcValue[i], 40, 0);
		else if(strcmp(pcParam[i], "i_c_ct") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAwbMnl), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_c_rg") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpRgain), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_c_bg") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpBgain), pcValue[i], 20, 0);
		///////////////////////////////////////////////////////////////
		// IMAGE //////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_i_sp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpSharpness), pcValue[i], 10, 0);
		else if(strcmp(pcParam[i], "i_i_gm") == 0)
		{
//			BYTE nMax = UP(UpGAMMA_STEP);
			BYTE nMax = 7;
			CGI_isp_BYTE_MAX_MIN(&MP(MpGamma), pcValue[i], nMax, 0);
		}
		else if(strcmp(pcParam[i], "i_i_gm_d") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpGammaDay), pcValue[i], 6, 0);		
		else if(strcmp(pcParam[i], "i_i_gm_n") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpGammaNgt), pcValue[i], 6, 0);
		else if(strcmp(pcParam[i], "i_i_mr") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpMirror), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_fp") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFlip), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_dw") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpAce), pcValue[i], 3, 0);
		else if(strcmp(pcParam[i], "i_i_df") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpDefog), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_df_mo") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpDefogMode), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_df_lv") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpDefogLevel), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_i_sh") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpShading), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_sh_we") == 0)	CGI_isp_BYTE_MAX_MIN(&MP(MpShdWeight), pcValue[i], 200, 0);
		else if(strcmp(pcParam[i], "i_i_pr") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpPrivacy), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_pr_mo") ==0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpPvcFormat), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_i_pr_yl") ==0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpPvcYLevel), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_i_pr_cbl") ==0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpPvcCRLevel), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_i_pr_crl") ==0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpPvcCBLevel), pcValue[i], 20, 0);
		else if(strcmp(pcParam[i], "i_i_pr_tra") ==0) 	CGI_isp_BYTE_MAX_MIN(&MP(MpPvcTrans), pcValue[i], 3, 0);
		else if(strncmp(pcParam[i], "i_i_pr_zd_", 10) == 0)
		{
			char *strNum = &(pcParam[i][10]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnPvcCfg(nNum)->bAction), pcValue[i], 1, 0);
		}
		else if(strncmp(pcParam[i], "i_i_pr_hp_", 10) == 0)
		{
			char *strNum = &(pcParam[i][10]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnPvcCfg(nNum)->bPosX), pcValue[i], 60, 0);
		}
		else if(strncmp(pcParam[i], "i_i_pr_vp_", 10) == 0)
		{
			char *strNum = &(pcParam[i][10]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnPvcCfg(nNum)->bPosY), pcValue[i], 34, 0);
		}
		else if(strncmp(pcParam[i], "i_i_pr_hs_", 10) == 0)
		{
			char *strNum = &(pcParam[i][10]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnPvcCfg(nNum)->bSizX), pcValue[i], 60, 0);
		}
		else if(strncmp(pcParam[i], "i_i_pr_vs_", 10) == 0)
		{
			char *strNum = &(pcParam[i][10]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnPvcCfg(nNum)->bSizY) , pcValue[i], 34, 0);
		}
		///////////////////////////////////////////////////////////////
		// MOTION /////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_m_mt") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItl), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_m_dt") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlDettone), pcValue[i], 4, 0);
		else if(strcmp(pcParam[i], "i_m_mf") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlRectFill), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_m_st") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlSens), pcValue[i], 10, 0);
		else if(strcmp(pcParam[i], "i_m_mo") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlMaskOsd), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_m_ta") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlAlarm), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_m_so") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpItlSigno), pcValue[i], 1, 0);
		else if(strncmp(pcParam[i], "i_m_wu_", 7) == 0)
		{
			char *strNum = &(pcParam[i][7]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnImdCfg(nNum)->bAction), pcValue[i], 1, 0);
		}
		else if(strncmp(pcParam[i], "i_m_hp_", 7) == 0)
		{
			char *strNum = &(pcParam[i][7]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnImdCfg(nNum)->bPosX), pcValue[i], 60, 0);
		}
		else if(strncmp(pcParam[i], "i_m_vp_", 7) == 0)
		{
			char *strNum = &(pcParam[i][7]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnImdCfg(nNum)->bPosY), pcValue[i], 34, 0);
		}
		else if(strncmp(pcParam[i], "i_m_hs_", 7) == 0)
		{
			char *strNum = &(pcParam[i][7]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnImdCfg(nNum)->bSizX), pcValue[i], 60, 0);
		}
		else if(strncmp(pcParam[i], "i_m_vs_", 7) == 0)
		{
			char *strNum = &(pcParam[i][7]);
			int nNum = atoi(strNum);
			CGI_isp_BYTE_MAX_MIN(&(gbMnImdCfg(nNum)->bSizY) , pcValue[i], 34, 0);
		}
		///////////////////////////////////////////////////////////////
		// OUTPUT SET /////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_o_md") == 0)
		{
#if (model_2M30p)
			if(CGI_isp_BYTE_MAX_MIN(&MP(FrameMode), pcValue[i], MN_FRAMEMODE_DNR_25_30, MN_FRAMEMODE_NORMAL_25_30) == DEF_OK)
#else			// 30P_NOMAL, 15P_WDR, 30P_DNR
			if(CGI_isp_BYTE_MAX_MIN(&MP(FrameMode), pcValue[i], MN_FRAMEMODE_DNR_50_60, MN_FRAMEMODE_NORMAL_25_30) == DEF_OK)
#endif			// 30P_NOMAL, 15P_WDR, 30P_DNR, 60P_NOMAL, 30P_WDR, 60P_DNR
				unISPChangeFlag = bitchang(unISPChangeFlag, 0x1, 1);
		}
		else if(strcmp(pcParam[i], "i_o_rlv") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpFrameRateLv), pcValue[i], MN_3sHI, MN_3sLOW);
		else if(strcmp(pcParam[i], "i_o_fq") == 0)
		{
			if(CGI_isp_BYTE_MAX_MIN(&MP(MpSysFreq), pcValue[i], MN_SYSFREQ_60, MN_SYSFREQ_50) == DEF_OK)
				unISPChangeFlag = bitchang(unISPChangeFlag, 0x1, 1);
		}
		///////////////////////////////////////////////////////////////
		// SYSTEM /////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else if(strcmp(pcParam[i], "i_s_st") == 0) {
			unISPChangeFlag = bitchang(unISPChangeFlag, 0x40000000, 1);
//			CGI_isp_BYTE_MAX_MIN(&gptMsgISPTbl.MENU_STYLE, pcValue[i], 7, 0);
			CGI_isp_BYTE_MAX_MIN(&(gptMsgISPTbl.MENU_STYLE), pcValue[i], 7, 0);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_STYLE, 0) == DEF_FAIL)
				vTaskDelay(1);
		}
		else if(strcmp(pcParam[i], "i_s_cb") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpCBar), pcValue[i], 1, 0);
		else if(strcmp(pcParam[i], "i_s_lg") == 0)
		{
			CGI_isp_BYTE_MAX_MIN(&MP(MpLanguageSel), pcValue[i], 4, 0);
			unISPChangeFlag = bitchang(unISPChangeFlag, 0x4, 1);
		}
		else if(strcmp(pcParam[i], "i_s_ct") == 0)		CGI_isp_BYTE_MAX_MIN(&MP(MpCamTitleOn), pcValue[i], 2, 0);
		else if(strcmp(pcParam[i], "i_s_ti") == 0)
		{
			int k, nTitlelen = strlen(pcValue[i]);	// 총 8byte
			if(nTitlelen > 8)			nTitlelen = 8;
			for(k=0;k<nTitlelen;k++)	MP(MpCamTitleIdx+k) = pcValue[i][k] == '+' ? 32 : pcValue[i][k];	// len만큼 쓰고
			for(;k<8;k++)				MP(MpCamTitleIdx+k) = 32;				// 나머지 공간은 0으로 클리어
			unISPChangeFlag = bitchang(unISPChangeFlag, 0x2, 1);
		}
		else if(strcmp(pcParam[i], "i_s_rs") == 0)
		{
			unISPChangeFlag = bitchang(unISPChangeFlag, 0x80000000, 1);
		}
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
#if 0
		else if(strcmp(pcParam[i], "i_z_28") == 0)
		{
			printf("0x28 : 0x%08X\r\n", ISP_0028_T);
		}
		else if(strcmp(pcParam[i], "i_y_28") == 0)
		{
			printf("0x28 : 0x%08X => 0x%08X", ISP_0028_T, strtoul(pcValue[i], NULL, 16));
			ISP_0028_T = strtoul(pcValue[i], NULL, 16);
			printf("0x%08X\r\n", ISP_0028_T);
		}
#endif
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////
		else
		{
//			dprintf("Error Msg : %s(%s)\r\n", pcParam[i], pcValue[i]);
		}
	}

	dprintf("==============================================================\r\n");

	if(bitcheck(unISPChangeFlag, 0x1))
	{
		gtUser.uviJpeg.sfFPS = 0;
		gtUser.uviH264[0].sfFPS = 0;
		gtUser.uviH264[1].sfFPS = 0;
#if (model_2M)
		if(MP(FrameMode) == MN_FRAMEMODE_NORMAL_50_60 || MP(FrameMode) == MN_FRAMEMODE_DNR_50_60)	// 50/60 fps(WDR이 아닌 상태)
		{
			if(gtUser.uviH264[0].eResolution == e_res1920x1080)
			{
				gtUser.uviH264[0].sfFPS = 1;
				gtUser.uviH264[1].sfFPS = 1;
			}
		}
		printf("FrameChange!(%d/%d/%d/%d/%d)\r\n", MP(FrameMode), gtUser.uviH264[0].eResolution, 
			gtUser.uviJpeg.sfFPS, gtUser.uviH264[0].sfFPS, gtUser.uviH264[1].sfFPS);
#endif
		UserSave(sfls_USER);
	}

	// ISP Title
	if(bitcheck(unISPChangeFlag, 0x2))
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_TITLE, 0) == DEF_FAIL)
			vTaskDelay(1);
	}

	// ISP Font
	if(bitcheck(unISPChangeFlag, 0x4))
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_FONT, 0) == DEF_FAIL)
			vTaskDelay(1);
	}

	// ISP value reset
	if(bitcheck(unISPChangeFlag, 0x80000000))
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_RESET, 0) == DEF_FAIL)
			vTaskDelay(1);
	}

	// ISP value save
	if(!(bitcheck(unISPChangeFlag, 0x40000000))) // unsave flag
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_ISP_SAVE, 0) == DEF_FAIL)
			vTaskDelay(1);
	}
	
	return strUrl_submit;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}

const char *App_ISPKeyCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
	printf("Input : %s\r\n", pcParam[0]);
	if(strcmp(pcParam[0], "btUp") == 0)			gptMsgISPTbl.ISP_UKEY = 0x01;
	else if(strcmp(pcParam[0], "btLe") == 0)	gptMsgISPTbl.ISP_UKEY = 0x08;
	else if(strcmp(pcParam[0], "btCe") == 0)	gptMsgISPTbl.ISP_UKEY = 0x10;
	else if(strcmp(pcParam[0], "btRi") == 0)	gptMsgISPTbl.ISP_UKEY = 0x04;
	else if(strcmp(pcParam[0], "btDo") == 0)	gptMsgISPTbl.ISP_UKEY = 0x02;

	return strUrl_osdkey;

	UNUSED(iIndex);
	UNUSED(iNumParams);
	UNUSED(pcParam);
	UNUSED(pcValue);
}
#endif
