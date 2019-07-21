/* **************************************************************************
 File Name  :   isp_app_para_tbl.h
 Description:   EN673 - User Parameter table
 Designer   :   Kim, Sunghoon / Lee, Wanhee
 Date       :   15. 6. 8 / 16. 3. 30
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */


#define USR_PAR_LIST/* User Parameter List */\
\
/*--¡¡SYSTEM¡¡--*/\
UP_SET(1, UpINSEL)/* Pre ISP Test Pattern Selection (0 ~ 7) */\
UP_SET(1, UpONSEL)/* Post ISP Test Pattern Selection (0 ~ 7) */\
UP_SET(1, UpTRG_DIN)/* Image Input Latch Position (0:Pixel Clock Negative, 1:Pixel Clock Positive) */\
UP_SET(1, UpTRG_HSI)/* Horizontal Sync Input Latch Position (0:Pixel Clock Negative, 1:Pixel Clock Positive) */\
UP_SET(1, UpTRG_VSI)/* Vertical Sync Input Latch Position (0:Pixel Clock Negative, 1:Pixel Clock Positive) */\
UP_SET(1, UpPOL_HSI)/* Horizontal Sync Input Polarity (0:When HSI Falling Position - H SYNC occur,  1:When HSI Rising Position - H SYNC occur) */\
UP_SET(1, UpPOL_VSI)/* Vertical Sync Input Polarity (0:When VSI Falling Position - V SYNC occur,  1:When VSI Rising Position - V SYNC occur) */\
UP_SET(1, UpLDI_POL)/* LVDS Data Catch Position. Sets Latch Position of path from LVDS clock domain to pixel clock domain. LVDS Clock -> Pixel Clock Path (0:Negative Edge, 1:Positive Edge) */\
UP_SET(1, UpLCK_DLY)/* Sub-LVDS Input Clock Delay Adjust(pLDIP2,pLDIM2 Input),  1 Step : Max case, about 0.3 ns  (0 ~ 7) */\
UP_SET(1, UpLDIDLY0)/* LVDS Channel 0 Delay Adjust,  Adjusts the input delay of LVDS Channel pLDIP0, pLDIM0,  1 Step : about 0.4 ns  (0 ~ 7) */\
UP_SET(1, UpLDIDLY1)/* LVDS Channel 1 Delay Adjust,  Adjusts the input delay of LVDS Channel pLDIP1, pLDIM1,  1 Step : about 0.4 ns  (0 ~ 7) */\
UP_SET(1, UpLDIDLY2)/* LVDS Channel 2 Delay Adjust,  Adjusts the input delay of LVDS Channel pLDIP2, pLDIM2,  1 Step : about 0.4 ns  (0 ~ 7) */\
UP_SET(1, UpLDIDLY3)/* LVDS Channel 3 Delay Adjust,  Adjusts the input delay of LVDS Channel pLDIP3, pLDIM3,  1 Step : about 0.4 ns  (0 ~ 7) */\
UP_SET(1, UpDCKO0_INV)/* BT1120 Output Clock Inversion (0:Normal Output, 1:Inversion Output) */\
UP_SET(1, UpDCKO0_DLY)/* DCKO Output Clock Delay Cell Adjust,  1 Step : Max case, about 0.3 ns  (0 ~ 7) */\
UP_SET(1, UpLDO0_SEL)/* LVDS data order selection, Adjusts the Alignment of LVDS input data. LVDS channel 0 order (LDIP0, LDIM0) */\
UP_SET(1, UpLDO1_SEL)/* LVDS data order selection, Adjusts the Alignment of LVDS input data. LVDS channel 1 order (LDIP1, LDIM1) */\
UP_SET(1, UpLDO2_SEL)/* LVDS data order selection, Adjusts the Alignment of LVDS input data. LVDS channel 3 order (LDIP3, LDIM3) */\
UP_SET(1, UpLDO3_SEL)/* LVDS data order selection, Adjusts the Alignment of LVDS input data. LVDS channel 4 order (LDIP4, LDIM4) */\
/*--¡¡KEY¡¡--*/\
UP_SET(2, UpADC_LEVEL_C)/* ADC Value of Center Key (0 ~ 0x1FF) */\
UP_SET(2, UpADC_LEVEL_U)/* ADC Value of Up Key (0 ~ 0x1FF) */\
UP_SET(2, UpADC_LEVEL_D)/* ADC Value of Down Key (0 ~ 0x1FF) */\
UP_SET(2, UpADC_LEVEL_L)/* ADC Value of Left Key (0 ~ 0x1FF) */\
UP_SET(2, UpADC_LEVEL_R)/* ADC Value of Right Key (0 ~ 0x1FF) */\
UP_SET(1, UpAD_CHT_BIT)/* number of bit for Chattering ADC Value of Key (1 ~ 8) */\
/*--¡¡TDN¡¡--*/\
UP_SET(1, UpTDN_EXTN_THRS_STEP)/* ADC Value unit per one step of THRES in DAY&NIGHT-EXTERN Menu (1 ~ 0x64) */\
UP_SET(1, UpTDN_AUTO_THRS_STEP)/* AGC Value unit per one step of THRES in DAY&NIGHT-AUTO Menu (1 ~ 0x64) */\
UP_SET(1, UpTDN_AUTO_MAGN_STEP)/* AGC Value unit per one step of MARGIN in DAY&NIGHT-AUTO Menu (1 ~ 0x64) */\
UP_SET(2, UpTDN_DLY_LOW)/* TDN low delay time (1000->1s, 2000->2s, ..., 20000->20s) */\
UP_SET(2, UpTDN_DLY_MIDDLE)/* TDN middle delay time (1000->1s, 2000->2s, ..., 20000->20s) */\
UP_SET(2, UpTDN_DLY_HIGH)/* TDN high delay time (1000->1s, 2000->2s, ..., 20000->20s) */\
/*--¡¡AE¡¡--*/\
UP_SET(1, UpAE_ORIGINAL_CTRL)/* AE Origianl control (0 or 1) */\
UP_SET(2, UpAE2_HSP)/* Luma detection area X start (default +- value) */\
UP_SET(2, UpAE2_HEP)/* Luma detection area Y end (default +- value) */\
UP_SET(2, UpAE2_VSP)/* Luma detection area Y start (default +- value) */\
UP_SET(2, UpAE2_VEP)/* Luma detection area Y end (default +- value) */\
UP_SET(2, UpAE2_HSP_CROP)/* Luma detection area X start in CROP mode (default +- value) */\
UP_SET(2, UpAE2_HEP_CROP)/* Luma detection area Y end in CROP mode (default +- value) */\
UP_SET(2, UpAE2_VSP_CROP)/* Luma detection area Y start in CROP mode (default +- value) */\
UP_SET(2, UpAE2_VEP_CROP)/* Luma detection area Y end in CROP mode (default +- value) */\
UP_SET(2, UpAE_SHT_SLICE_LINE)/* Shutter limit to minimum (line value : spec min ~ 1123) */\
UP_SET(1, UpAE_OUTDOOR_THRS_VAL)/* Shutter threshold in OUTDOOR mode (ex. 1125>>3 =  375 => 1/240s) */\
UP_SET(1, UpAE_DEBLUR_THRS_VAL)/* Shutter threshold in DEBLUR mode (ex. 1125>>3 =  375 => 1/240s) */\
UP_SET(1, UpAE_DEBLUR_GAIN)/* Deblur shutter increasing gradient (0x40 ~ 0xFF -> 0.5 ~ 2.0) */\
UP_SET(1, UpAE_SPEED)/* AE Origianl control Only, Tracking Speed (0 ~ 0x80 ~ 0xFF ->  x0.25 ~ x1 ~ x4) */\
UP_SET(1, UpAE_SPEED_DC)/* AE Origianl control Only, DC IRIS Speed (Fast 0 ~ Slow 3) */\
UP_SET(1, UpAE_SPEED_DC_VP)/* IRIS Virtual Position control Speed for judge IRIS full open (Slow 9 ~ Fast 30) */\
UP_SET(2, UpAE_TGT_GAIN)/* Normal Brightness target gain ((UpAE_TGT_GAIN * BRIGHTNESS in Menu) + UpAE_TGT_OFST < 0x3FF) */\
UP_SET(2, UpAE_TGT_OFST)/* Normal Brightness target offset ((UpAE_TGT_GAIN * BRIGHTNESS in Menu) + UpAE_TGT_OFST < 0x3FF) */\
UP_SET(2, UpAE_CLIP_OFST)/* Normal Brightness Clip level (AE Target +  UpAE_CLIP_OFST < 0x3FF) */\
UP_SET(2, UpAE_WDR_ON_AGC)/* WDR AGC position for maximum weight (default +- value) */\
UP_SET(1, UpAE_WDR_MIN_WGT)/* WDR minimum weight (under 0x100(100%)) */\
UP_SET(1, UpAE_WDR_MOTION_SEL)/* Motion mismatch effect selection (0 or 1) */\
UP_SET(1, UpAE_WDR_STYLE_SEL)/* WDR style selection (0 or 1) */\
UP_SET(2, UpAE_WDR_LTGT_OFST)/* WDR Long Brightness target offset (default +- value) */\
UP_SET(2, UpAE_WDR_STGT_OFST)/* WDR Short Brightness target offset (default +- value) */\
UP_SET(2, UpAE_WDR_LTGT_OFST2)/* WDR Short Brightness target offset for UpAE_WDR_STYLE_SEL==1 (default +- value) */\
UP_SET(2, UpAE_WDR_STGT_OFST2)/* WDR Short Brightness target offset for UpAE_WDR_STYLE_SEL==1 (default +- value) */\
UP_SET(1, UpAE_WDR_SWGT_L)/* WDR weight adjust for menu Low (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpAE_WDR_SWGT_M)/* WDR weight adjust for menu Middle (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpAE_WDR_SWGT_H)/* WDR weight adjust for menu High (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
/*--¡¡AWB¡¡--*/\
UP_SET(2, UpAWB_HSP)/* AWB detection area X start (default +- value) */\
UP_SET(2, UpAWB_HW)/* AWB detection area Y end (default +- value) */\
UP_SET(2, UpAWB_VSP)/* AWB detection area Y start (default +- value) */\
UP_SET(2, UpAWB_VW)/* AWB detection area Y end (default +- value) */\
UP_SET(2, UpAWB_HSP_CROP)/* AWB detection area X start in CROP mode (default +- value) */\
UP_SET(2, UpAWB_HW_CROP)/* AWB detection area Y end in CROP mode (default +- value) */\
UP_SET(2, UpAWB_VSP_CROP)/* AWB detection area Y start in CROP mode (default +- value) */\
UP_SET(2, UpAWB_VW_CROP)/* AWB detection area Y end in CROP mode (default +- value) */\
UP_SET(2, UpAWB_TCNT_LMT)/* White pixel cnt limit to minimum (under 0x10000) */\
UP_SET(2, UpAWB_DBOFFSET)/* Color map coordinate x offset (default +- value) */\
UP_SET(2, UpAWB_DROFFSET)/* Color map coordinate y offset (default +- value) */\
UP_SET(2, UpAWB_AGC_MIN)/* Chroma AGC position for maximum weight (default +- value) */\
UP_SET(1, UpAWB_AGC_WGT_MIN)/* Chroma minimum weight (under 0x100(100%)) */\
UP_SET(1, UpAWB_SPEED_MTRX)/* Matrix tracking speed (Low is high speed, max=0xff) */\
UP_SET(2, UpAWB_YSLICE)/* White Y slice (under 0x3ff) */\
UP_SET(2, UpAWB_YCLIP)/* White Y clip (under 0x3ff, more than UpAWB_YSLICE) */\
UP_SET(1, UpAWB_GAIN_TIMES)/* RGB Gain limit (X times, under 4) */\
UP_SET(1, UpICSEL)/* ISP Pre Process side RGB array set (0~3) */\
UP_SET(1, UpOCSEL)/* ISP Post Process side RGB array set (0~3) */\
UP_SET(1, UpICSEL_FLIP)/* ISP Pre-Process side RGB array set @ Flip (0~3) */\
UP_SET(1, UpOCSEL_FLIP)/* ISP Post-Process side RGB array set @ Flip (0~3) */\
/*--¡¡Color¡¡suppression¡¡--*/\
UP_SET(1, UpLSUP_ON)/* Low Light color suppression on/off (0: off, 1: on) */\
UP_SET(1, UpLSUP_GA)/* Low Light color suppression gain (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpLSUP_TH)/* Low Light color suppression threshold (0 ~ 0xFF) */\
UP_SET(1, UpCSUP_ON)/* High Light color suppression on/off (Y domain control) (0: off, 1: on) */\
UP_SET(1, UpCSUP_GA)/* High Light color suppression gain (Y domain control) (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpCSUP_TH)/* High Light color suppression threshold (Y domain control) (0 ~ 0xFF) */\
UP_SET(1, UpCSUP_WTH)/* High Light color suppression threshold when the WDR Mode (Y domain control) (0 ~ 0xFF) */\
UP_SET(1, UpMpCES_ON)/* Edge color suppression on/off in normal(0: off, 1: on) */\
UP_SET(1, UpMpWdrCES_ON)/* Edge color suppression on/off in WDR(0: off, 1: on) */\
UP_SET(1, UpCES_GAIN)/* Edge color suppression normal gain (0 ~ 0x3F) */\
UP_SET(1, UpCES_WGAIN)/* Edge color suppression WDR gain (0 ~ 0x3F) */\
UP_SET(1, UpCES_LCLP)/* Edge color suppression normal LClip (0 ~ 0xFF) */\
UP_SET(1, UpCES_WLCLP)/* Edge color suppression WDR LClip (0 ~ 0xFF) */\
/*--¡¡Hue¡¡&¡¡Gain¡¡--*/\
UP_SET(1, UpBY_Gain_Negative)/* B-Y Negative Gain Control (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, UpBY_Gain_Positive)/* B-Y Positive Gain Control (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, UpRY_Gain_Negative)/* R-Y Negative Gain Control (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, UpRY_Gain_Positive)/* R-Y Positive Gain Control (0 ~ 0xFF, Default:0x60) */\
UP_SET(1, UpBY_Hue_Negative)/* B-Y Negative Hue Control (0x1:-90¡Æ, 0x80:0¡Æ, 0xFF:90¡Æ) */\
UP_SET(1, UpBY_Hue_Positive)/* B-Y Positive Hue Control (0x1:-90¡Æ, 0x80:0¡Æ, 0xFF:90¡Æ) */\
UP_SET(1, UpRY_Hue_Negative)/* R-Y Negative Hue Control (0x1:-90¡Æ, 0x80:0¡Æ, 0xFF:90¡Æ) */\
UP_SET(1, UpRY_Hue_Positive)/* R-Y Positive Hue Control (0x1:-90¡Æ, 0x80:0¡Æ, 0xFF:90¡Æ) */\
/*--¡¡Gamma¡¡--*/\
UP_SET(1, UpGAMMA_Y_ON)/* Y GAMMA on/off (0: off, 1: on) */\
UP_SET(1, UpGAMMA_C_ON)/* C GAMMA on/off (0:off, 1:on) */\
/*--¡¡DNR¡¡--*/\
UP_SET(1, Up3D_DNR_GAIN_LOW)/* 3D-DNR gain adjust for menu LOW (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, Up3D_DNR_GAIN_MIDDLE)/* 3D-DNR gain adjust for menu MIDDLE (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, Up3D_DNR_GAIN_HIGH)/* 3D-DNR gain adjust for menu HIGH (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
/*--¡¡Sharpness¡¡--*/\
UP_SET(1, UpSHARPNESS_GAIN)/* Sharpness Gain (0=OFF, 1=x5, 2=x2.5, 3=x1.67, 4=1.25, 5=x1, 6=x0.83, 8=x0.625, 0xA=x0.5, 0x14=x0.25, 0x32=x0.1) */\
/*--¡¡Defog¡¡--*/\
UP_SET(1, UpDEFOG_OSDON)/* Defog Area Set OSD On/Off Function (0x0 or 0x1) */\
UP_SET(1, UpDEFOG_720_HW)/* Defog Area Set Horizontal Width in 720 mode (0x0 ~ 0xf) */\
UP_SET(2, UpDEFOG_720_HSP)/* Defog Area Set Horizontal Start Position in 720 mode (0x0 ~ 0x790) */\
UP_SET(1, UpDEFOG_720_VW)/* Defog Area Set Vertical Width in 720 mode (0x0 ~ 0xf) */\
UP_SET(2, UpDEFOG_720_VSP)/* Defog Area Set Vertical Start Position in 720 mode (0x0 ~ 0x440) */\
UP_SET(1, UpDEFOG_1080_HW)/* Defog Area Set Horizontal Width in 1080 mode (0x0 ~ 0xf) */\
UP_SET(2, UpDEFOG_1080_HSP)/* Defog Area Set Horizontal Start Position in 1080 mode (0x0 ~ 0x790) */\
UP_SET(1, UpDEFOG_1080_VW)/* Defog Area Set Vertical Width in 1080 mode (0x0 ~ 0xf) */\
UP_SET(2, UpDEFOG_1080_VSP)/* Defog Area Set Vertical Start Position in 1080 mode (0x0 ~ 0x440) */\
UP_SET(2, UpDEFOG_TH_LOW)/* Defog Maximum Strength for menu LOW (0x0 ~ 0xffff) */\
UP_SET(2, UpDEFOG_TH_MID)/* Defog Maximum Strength for menu MIDDLE (LOW ~ 0xffff) */\
UP_SET(2, UpDEFOG_TH_HIGH)/* Defog Maximum Strength for menu HIGH (MIDDLE ~ 0xffff) */\
/*--¡¡ACE¡¡--*/\
UP_SET(1, UpACETBL1_LOW)/* ACE Dark Area Strength for menu LOW (0x0 ~ 0xff) */\
UP_SET(1, UpACETBL1_MID)/* ACE Dark Area Strength for menu MIDDLE (LOW ~ 0xff) */\
UP_SET(1, UpACETBL1_HIGH)/* ACE Dark Area Strength for menu HIGH (MIDDLE ~ 0xff) */\
UP_SET(1, UpACETBL2_LOW)/* ACE Bright Area Strength for menu LOW (0x0 ~ 0xff) */\
UP_SET(1, UpACETBL2_MID)/* ACE Bright Area Strength for menu MIDDLE (LOW ~ 0xff) */\
UP_SET(1, UpACETBL2_HIGH)/* ACE Bright Area Strength for menu HIGH (MIDDLE ~ 0xff) */\
UP_SET(1, UpACEWDR1_TH)/* ACE Dark Area Strength of WDR (0x0 ~ 0xff) */\
UP_SET(1, UpACEWDR2_TH)/*  ACE Bright Area Strength of WDR (0x0 ~ 0xff) */\
/*--¡¡High¡¡Light¡¡Mask¡¡--*/\
UP_SET(1, UpHLMASK_THRS_STEP)/* Menu Step of High Light masking level (0 ~ 0xA) */\
UP_SET(1, UpHLMASK_CY)/* HLMASK CY of Color for Customize Color Menu (0 ~ 0xFF) */\
UP_SET(1, UpHLMASK_CB)/* HLMASK CB of Color for Customize Color Menu (0 ~ 0xFF) */\
UP_SET(1, UpHLMASK_CR)/* HLMASK CR of Color for Customize Color Menu (0 ~ 0xFF) */\
/*--¡¡Defect¡¡correction¡¡--*/\
UP_SET(1, UpDEFECT_DIFTH)/* Within the pattern, this register uses to detect directional defect. Difference threshold for isolated point determination. (0x0 ~ 0xff) */\
UP_SET(2, UpDEFECT_AGC_MIN)/* AGC value of Rather dark for control defect. (0 ~ 0x1F4 AGC value) */\
UP_SET(2, UpDEFECT_AGC_MID)/* AGC value of Dark for control defect. (MIN ~ 0x1F4 AGC value) */\
UP_SET(2, UpDEFECT_AGC_MAX)/* AGC value of Very dark for control defect. (MIDDLE ~ 0x1F4 AGC value) */\
UP_SET(1, UpDEFECT_GA)/* Within the pattern, this parameter uses to detect directional defect. Difference gain for isolated point determination. (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpDEFECT_SPOT_GA)/* Spot pattern area gain. (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2) */\
UP_SET(1, UpDEFECT_TSEL)/* Select test image in core for directional defect correction. (VLOCKI)  "0" : Normal operation  "1" : Difference values for isolated points  "2" : Image before correction  "3" : Complete correction image (100% correction image except weight).  "4" : Defect pattern detection value */\
/*--¡¡Motion¡¡&¡¡Intelligence¡¡--*/\
UP_SET(1, UpIMD_BG_UP)/* Number of Frame for Background Image Update delay (0 ~ FPS of 1sec by Sensor, 1.3M Sensor : 0 ~ 60, 2M Sensor : 0 ~ 30) */\
UP_SET(1, UpIMD_THRES_WEG)/* Weight for SENSITIVITY Menu (0 ~ 3) */\
UP_SET(1, UpIMD_BOX_Y)/* Change Y of OSD BOX for display of Motion (0x0 ~ 0xFF) */\
UP_SET(1, UpIMD_BOX_Cb)/* Change Cb of OSD BOX for display of Motion (0x0 ~ 0xFF) */\
UP_SET(1, UpIMD_BOX_Cr)/* Change Cr of OSD BOX for display of Motion (0x0 ~ 0xFF) */\
UP_SET(1, UpIMD_BRIGHT_MAX)/* AE error in bright for detection of 'BRIGHT CHANGE' (0x0 ~ 0xFF) */\
UP_SET(1, UpIMD_BRIGHT_MIN)/* AE error in dark for detection of 'BRIGHT CHANGE' (0x0 ~ 0xFF) */\
UP_SET(1, UpIMD_MOVING_TH)/* Size Ratio of 'DET WINDOW' for detection of 'CAMERA MOVING' (0:x0.5, 1:x0.667, 2:x0.75, 3:x0.8, 4:x0.833, 5:x0.857, 6:x0.875, 7:x0.889, 8:x0.9) */\
/*--¡¡Privacy¡¡Box¡¡--*/\
UP_SET(1, UpBOX_MV_PXL)/* Pixel unit per one step of POS & SIZE in PRIVACY Menu (0x0 ~ 0xFF) */\
UP_SET(1, UpBOX_COLOR_STEP)/* Color Step for Y & CB & CR LEVEL in PRIVACY Menu (0x0 ~ 0xFF) */\
/*--¡¡2Motor¡¡--*/\
UP_SET(1, UpTUNING_MODE_EN)/* Tuning Mode Enable */\
UP_SET(2, UpFOCUS_TARGET)/* Focus count (0x0 ~ 0x1FF) */\
UP_SET(2, UpZOOM_TARGET)/* Zoom count (0x0 ~ 0x1FF) */\
UP_SET(1, UpCapture)/* Capture current position */\
/*--¡¡MENU:¡¡OUTPUT¡¡SET¡¡--*/\
UP_SET(1, UpFrameMode)/* MODE : 0 = 30P_NOMAL, 1 = 15P_WDR, 2 = 30P_DNR, 3 = 60P_NOMAL, 4 = 30P_WDR, 5 = 60P_DNR */\
UP_SET(1, UpMpWdrWgt)/* WDR : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(1, UpMpAdnr)/* DNR : 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, UpMpSysFreq)/* FREQ : 0 = 25Hz, 1 = 30Hz */\
/*--¡¡MENU:¡¡SYSTEM¡¡--*/\
UP_SET(1, UpMpDoorCam_LP_Mode)/* POWER SAVE : 0 = OFF, 1 = 10S, 2 = 15S, 3 = 20S, 4 = 25S, 5 = 30S */\
UP_SET(1, UpMpCamID)/* COM. ¡æ CAM ID : 0 ~ 255 */\
UP_SET(1, UpMpBaudrate)/* COM. ¡æ BAUDRATE : Only 115200, Not yet supported */\
UP_SET(1, UpMpOutFromat)/* Color space : 0 = SD, 1 = HD, 2 = UV */\
UP_SET(1, UpMpCBar)/* COLOR BAR : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpLanguageSel)/* LANGUAGE : 0 = ENG, 1 = CHN, 1 = CHN(S), 1 = JPN, 1 = KOR */\
UP_SET(1, UpMpCamTitleOn)/* CAM TITLE : 0 = OFF, 1 = RIGHT UP, 2 = LEFT DOWN */\
UP_SET(1, UpMpCamTitle0)/* The first letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle1)/* The second letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle2)/* The third letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle3)/* The fourth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle4)/* The fifth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle5)/* The sixth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle6)/* The seventh letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpCamTitle7)/* The eighth letter of the CAM TITLE, Use ASCII code. */\
UP_SET(1, UpMpReset)/* RESET */\
/*--¡¡MENU:¡¡IRIS¡¡--*/\
UP_SET(1, UpMpIris)/* IRIS (DC iris) : 0 = ELC, 1 = ALC,  (piris & AF iris) : 0 = MANUAL, 1 = AUTO */\
UP_SET(1, UpMpIrisLvl)/* IRIS(piris & AF iris):MANUAL ¡æ LEVEL : 0 ~ 20 */\
/*--¡¡MENU:¡¡EXPOSURE¡¡--*/\
UP_SET(1, UpMpBrightness)/* BRIGHTNESS ¡æ DAY : 0 ~ 20 */\
UP_SET(1, UpMpBrightnessMin)/* BRIGHTNESS ¡æ NIGHT : 0 ~ 20 */\
UP_SET(1, UpMpAntiSatOn)/* ANTI_SAT : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpSatBrt)/* ANTI_SAT:ON ¡æ WEIGHT : 0 ~ 20 */\
UP_SET(1, UpMpLSpotLv)/* ANTI_SAT:ON ¡æ LEVEL : 0 ~ 20 */\
UP_SET(1, UpMpStabilizing)/* ANTI_SAT:ON ¡æ STABILIZING : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, UpMpMinGammaY)/* ANTI_SAT:ON ¡æ BACKLIGHT : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH */\
UP_SET(1, UpMpAllSpeed)/* AE SPEED ¡æ AE SPEED : 0 ~ 20 */\
UP_SET(1, UpMpSmallSpeed)/* AE SPEED ¡æ STABILIZING : 0 = HIGH, 1 = MIDDLE, 2 = LOW */\
UP_SET(1, UpMpOpenSpeed)/* AE SPEED ¡æ SHT. OPEN : 0 ~ 20 */\
UP_SET(1, UpMpCloseSpeed)/* AE SPEED ¡æ SHT. CLOSE : 0 ~ 20 */\
UP_SET(1, UpMpAgcSpeed)/* AE SPEED ¡æ AGC SPEED : 0 ~ 20 */\
UP_SET(1, UpMpIrsSpeed)/* AE SPEED ¡æ IRIS SPEED : 0 ~ 20 */\
UP_SET(1, UpMpShutter)/* SHUTTER : 0 = AUTO, 1 = MANUAL, 2 = FLICKER */\
UP_SET(1, UpMpShutSpd)/* SHUTTER:MANUAL ¡æ SPEED (Depends on 'FREQ' & 'OUTPUT SET¡æMODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, UpMpShtMin)/* SHUTTER:AUTO ¡æ SHUTTER MIN (Depends on 'FREQ' & 'OUTPUT SET¡æMODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, UpMpDeblurMin)/* SHUTTER:AUTO ¡æ DEBLUR MIN (Depends on 'FREQ' & 'OUTPUT SET¡æMODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, UpMpShtMax)/* SHUTTER:AUTO ¡æ SHUTTER MAX (Depends on 'FREQ' & 'OUTPUT SET¡æMODE') : 0 = 1/30, 1 = 1/60, 2 = 1/120, 3 = 1/250, 4 = 1/500, 5 = 1/1000, 6 = 1/2000, 7 = 1/4000, 8 = 1/8000, 9 = 1/15000, 10 = 1/30000 */\
UP_SET(1, UpMpDcMode)/* IRIS:ALC & SHUTTER:AUTO ¡æ MODE : 0 = INDOOR, 1 = OUTDOOR, 2 = DEBLUR */\
UP_SET(1, UpMpShtMode)/* IRIS:ELC/MANUAL/AUTO & SHUTTER:AUTO ¡æ MODE : 0 = NORMAL, 1 = DEBLUR */\
UP_SET(1, UpMpDss)/* SEMS-UP : 0 = OFF, 1 = X2, 2 = X4, 3 = X8, 4 = X16, 5 = X32, 6 = X64(Must be set OUTPUT SET¡æMODE:60P_NOMAL£ü30P_WDR£ü60P_DNR) */\
UP_SET(1, UpMpAgc)/* AGC MAX : 0 ~ 10 */\
UP_SET(1, UpMpAgcMin)/* AGC MIN : 0 ~ 10 */\
/*--¡¡MENU:¡¡BACKLIGHT¡¡--*/\
UP_SET(1, UpMpBackLight)/* BACKLIGHT : 0 = OFF, 1 = HLC, 2 = BLC */\
UP_SET(1, UpMpHlMaskThrs)/* BACKLIGHT:HLC ¡æ LEVEL : 0 ~ 20 */\
UP_SET(1, UpMpHlMaskColor)/* BACKLIGHT:HLC ¡æ COLOR : 0 = WHT, 1 = YEL, 2 = CYN, 3 = GRN, 4 = MAG, 5 = RED, 6 = BLU, 7 = BLK, 8 = CUSTOMIZE */\
UP_SET(1, UpMpBlcPosX)/* BACKLIGHT:BLC ¡æ H-POS : 0 ~ 20 */\
UP_SET(1, UpMpBlcPosY)/* BACKLIGHT:BLC ¡æ V-POS : 0 ~ 20 */\
UP_SET(1, UpMpBlcSizX)/* BACKLIGHT:BLC ¡æ H-SIZE : 0 ~ 20 */\
UP_SET(1, UpMpBlcSizY)/* BACKLIGHT:BLC ¡æ V-SIZE : 0 ~ 20 */\
/*--¡¡MENU:¡¡DAY&NIGHT¡¡--*/\
UP_SET(1, UpMpTdn)/* DAY&NIGHT : 0 = AUTO, 1 = COLOR, 2 = B&W, 3 = EXTERN */\
UP_SET(1, UpMpTdnThrs)/* DAY&NIGHT:AUTO ¡æ AGC THRES : 0 ~ 20 */\
UP_SET(1, UpMpTdnMarg)/* DAY&NIGHT:AUTO ¡æ AGC MARGIN : 0 ~ 20 */\
UP_SET(1, UpMpTdnDly)/*  DAY&NIGHT:AUTO ¡æ DELAY : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(1, UpMpLedOn)/* DAY&NIGHT ¡æ IR LED : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpLedSatCtrl)/* DAY&NIGHT ¡æ ANTI-SAT. : 0 ~ 20 (Must be set IR LED:ON) */\
UP_SET(1, UpMpTdnSw)/* DAY&NIGHT:EXTERN ¡æ EXTERN SW : 0 = HIGH, 1 = LOW */\
UP_SET(1, UpMpTdnDNThres)/* DAY&NIGHT:EXTERN ¡æ D>N THRES : 0 ~ 20 */\
UP_SET(1, UpMpTdnNDThres)/* DAY&NIGHT:EXTERN ¡æ N>D THRES : 0 ~ 20 */\
/*--¡¡MENU:¡¡COLOR¡¡--*/\
UP_SET(1, UpMpAwb)/* AWB : 0 = AUTO, 1 = AUTOext, 2 = PRESET, 3 = MANUAL */\
UP_SET(1, UpMpAwbPrst)/* AWB:PRESET ¡æ 0 = Do not press, 1 = Press and hold */\
UP_SET(1, UpMpAwbMnl)/* AWB:MANUAL ¡æ C-TEMP : 0 = 3000K, 1 = 5000K, 2 = 8000K */\
UP_SET(1, UpMpRgain)/* AWB:MANUAL ¡æ R-GAIN : 0 ~ 20 */\
UP_SET(1, UpMpBgain)/* AWB:MANUAL ¡æ B-GAIN : 0 ~ 20 */\
UP_SET(1, UpMpSaturation)/* G-GAIN : 0 ~ 40 */\
UP_SET(1, UpMpSaturationR)/* R-GAIN : 0 ~ 40 */\
UP_SET(1, UpMpSaturationB)/* B-GAIN : 0 ~ 40 */\
/*--¡¡MENU:¡¡IMAGE¡¡--*/\
UP_SET(1, UpMpAce)/* ACE : 0 = OFF, 1 = LOW, 2 = MIDDLE, 3 = HIGH (Must be set DEFOG:OFF) */\
UP_SET(1, UpMpAceGmgn)/* ACE ¡æ WEIGHT : 0 ~ 255 */\
UP_SET(1, UpMpAceBrt)/* ACE & DEFOG ¡æ DIGITAL BRIGHTNESS : 0 ~ 32 */\
UP_SET(1, UpMpAceCont)/* ACE & DEFOG ¡æ DIGITAL CONTRAST : 0 ~ 32 */\
UP_SET(1, UpMpDefog)/* DEFOG : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpDefogMode)/* DEFOG:ON ¡æ MODE : 0 = MANUAL, 1 = AUTO */\
UP_SET(1, UpMpDefogLevel)/* DEFOG:ON ¡æ LEVEL : 0 = LOW, 1 = MIDDLE, 2 = HIGH */\
UP_SET(1, UpMpSharpness)/* SHARPNESS : 0 ~ 10 */\
UP_SET(1, UpMpShpSmallEdge)/* SHARPNESS ¡æ SMALL EDGE : 0 ~ 255 */\
UP_SET(1, UpMpShpBigEdge)/* SHARPNESS ¡æ BIG EDGE : 0 ~ 255 */\
UP_SET(1, UpMpShpAgcLow)/* SHARPNESS ¡æ AGC LOW : 0% ~ 99% */\
UP_SET(1, UpMpShpAgcMid)/* SHARPNESS ¡æ AGC MID : 0% ~ 99% */\
UP_SET(1, UpMpShpAgcHigh)/* SHARPNESS ¡æ AGC HIGH : 0% ~ 99% */\
UP_SET(1, UpMpGamma)/* GAMMA : 0 = 0.45, 1 = 0.55, 2 = 0.65, 3 = 0.75 */\
UP_SET(1, UpMpFlip)/* FLIP : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpMirror)/* MIRROR : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpShading)/* SHADING : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpShdWeight)/* SHADING:ON ¡æ WEIGHT : 0 ~ 200 % */\
UP_SET(1, UpMpPrivacy)/* PRIVACY : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpPvcZone)/* PRIVACY:ON ¡æ ZONE NUM : 0 ~ 15 */\
UP_SET(1, UpMpPvcFormat)/* PRIVACY:ON ¡æ MASK MODE : 0 = FILL, 1 = FORMAT */\
UP_SET(1, UpMpPvcTrans)/* PRIVACY:ON ¡æ TRANS : 0 ~ 3 */\
UP_SET(1, UpMpPvcYLevel)/* PRIVACY:ON ¡æ Y LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_SET(1, UpMpPvcCRLevel)/* PRIVACY:ON ¡æ CB LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_SET(1, UpMpPvcCBLevel)/* PRIVACY:ON ¡æ CR LEVEL : 0 ~ 20 (Must be set MASK MODE:FILL) */\
UP_SET(1, UpMpPipOn)/* PIP : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpPipColor)/* PIP:ON ¡æ COLOR : 0 = WHT, 1 = YEL, 2 = CYN, 3 = GRN, 4 = MAG, 5 = RED, 6 = BLU, 7 = BLK, 8 = CUSTOMIZE */\
UP_SET(1, UpMpPip1_PosX)/* PIP:ON ¡æ H-POS : 0 ~ 255 */\
UP_SET(1, UpMpPip1_PosY)/* PIP:ON ¡æ V-POS : 0 ~ 255 */\
UP_SET(1, UpMpPip1_SizX)/* PIP:ON ¡æ SIZE : 0 ~ 100 */\
/*--¡¡MENU:¡¡MOTION¡¡--*/\
UP_SET(1, UpMpItl)/* MOTION : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpItlSens)/* MOTION:ON ¡æ SENSITIVITY : 0 ~ 10 */\
UP_SET(1, UpMpItlWindowuse)/* MOTION:ON ¡æ DET WINDOW ¡æ WINDOW ZONE : 0 ~ 3 */\
UP_SET(1, UpMpItlDettone)/* MOTION:ON ¡æ DET TONE : 0 ~ 4 */\
UP_SET(1, UpMpItlRectFill)/* MOTION:ON ¡æ MDRECT FILL : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpItlMaskOsd)/* MOTION:ON ¡æ MOTION OSD : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpItlAlarm)/* MOTION:ON ¡æ TEXT ALARM : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpItlSigno)/* MOTION:ON ¡æ SIGNAL OUT : 0 = OFF, 1 = ON */\
/*--¡¡MENU:¡¡FOCUS¡¡ADJ¡¡--*/\
UP_SET(1, UpMpFad_On)/* FOCUS ADJ : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpFad_Zone)/* FOCUS ADJ:ON ¡æ FOCUS AREA : 0 = NARROW, 1 = MIDDLE, 2 = WIDE */\
UP_SET(1, UpMpFad_Trans)/* FOCUS ADJ:ON ¡æ DISP TONE : 0 ~ 2 */\
UP_SET(1, UpMpFad_PosX)/* FOCUS ADJ:ON ¡æ DISP H_POS : 0 ~ 60 */\
UP_SET(1, UpMpFad_PosY)/* FOCUS ADJ:ON ¡æ DISP V_POS : 0 ~ 34 */\
UP_SET(1, UpMpFad_SizX)/* FOCUS ADJ:ON ¡æ DISP H_SIZE : 0 ~ 60 */\
UP_SET(1, UpMpFad_SizY)/* FOCUS ADJ:ON ¡æ DISP V_SIZE : 0 ~ 34 */\
/*--¡¡MENU:¡¡MOTORIZED¡¡--*/\
UP_SET(1, UpMpMAF_mode)/* AF MODE : 0 = ONESHOT, 1 = MANUAL */\
UP_SET(1, UpMpMAF_Filter)/* D&N FILTER : 0 = SYNC OFF, 1 = SYNC ON */\
UP_SET(1, UpMpMAF_MotorInit)/* INITIAL : 0 = Do not press, 1 = Press and hold */\
UP_SET(1, UpMpMAF_TestMode)/* SETUP ¡æ MOTOR SETUP ¡æ DEV MODE : 0 = OFF, 1 = ON */\
UP_SET(1, UpMpMAF_TotalPos_Z)/* SETUP ¡æ MOTOR SETUP ¡æ ZOOM AREA ¡æ TOTAL AREA : 0 ~ 255 */\
UP_SET(1, UpMpMAF_StartPos_Z)/* SETUP ¡æ MOTOR SETUP ¡æ ZOOM AREA ¡æ START POS : 0 ~ 255 */\
UP_SET(1, UpMpMAF_EndPos_Z)/* SETUP ¡æ MOTOR SETUP ¡æ ZOOM AREA ¡æ END POS : 0 ~ 255 */\
UP_SET(1, UpMpMAF_TotalPos_F)/* SETUP ¡æ MOTOR SETUP ¡æ FOCUS AREA ¡æ TOTAL AREA : 0 ~ 255 */\
UP_SET(1, UpMpMAF_StartPos_F)/* SETUP ¡æ MOTOR SETUP ¡æ FOCUS AREA ¡æ START POS : 0 ~ 255 */\
UP_SET(1, UpMpMAF_EndPos_F)/* SETUP ¡æ MOTOR SETUP ¡æ FOCUS AREA ¡æ END POS : 0 ~ 255 */\
UP_SET(1, UpMpMAF_LensModel)/* SETUP ¡æ MOTOR SETUP ¡æ LENS MODEL SELECTION : 0 ~ 9 */\
/*--¡¡USER¡¡Custom¡¡--*/\
UP_SET(1, UpPAR00)\
UP_SET(1, UpPAR01)\
UP_SET(1, UpPAR02)\
UP_SET(1, UpPAR03)\
UP_SET(1, UpPAR04)\
UP_SET(1, UpPAR05)\
UP_SET(1, UpPAR06)\
UP_SET(1, UpPAR07)\
UP_SET(1, UpPAR08)\
UP_SET(1, UpPAR09)\
UP_SET(1, UpPAR0A)\
UP_SET(1, UpPAR0B)\
UP_SET(1, UpPAR0C)\
UP_SET(1, UpPAR0D)\
UP_SET(1, UpPAR0E)\
UP_SET(1, UpPAR0F)\
UP_SET(1, UpPAR10)\
UP_SET(1, UpPAR11)\
UP_SET(1, UpPAR12)\
UP_SET(1, UpPAR13)\
UP_SET(1, UpPAR14)\
UP_SET(1, UpPAR15)\
UP_SET(1, UpPAR16)\
UP_SET(1, UpPAR17)\
UP_SET(1, UpPAR18)\
UP_SET(1, UpPAR19)\
UP_SET(1, UpPAR1A)\
UP_SET(1, UpPAR1B)\
UP_SET(1, UpPAR1C)\
UP_SET(1, UpPAR1D)\
UP_SET(1, UpPAR1E)\
UP_SET(1, UpPAR1F)\
/*--¡¡END¡¡--*/\

