#ifndef CGISSI_ISP_H_
#define CGISSI_ISP_H_

#define CGIfile_isp		"/isp/appispmu.cgi"
#define CGIfile_ispky	"/isp/appispky.cgi"

#define SSI_ISP_state		700
#define SSI_ISP_iris		701
#define SSI_ISP_focusadj	702	// 2motor 사용시 비활성화
#define SSI_ISP_2motor		703	// 2motor 사용시 활성화
#define SSI_ISP_exposure	704
#define SSI_ISP_backlight	705
#define SSI_ISP_daynight	706
#define SSI_ISP_color		707
#define SSI_ISP_outs		708
#define SSI_ISP_image		709
#define SSI_ISP_motion		710
#define SSI_ISP_system		711

extern int App_ISPSSIHandler(int iIndex, char *pcInsert, int iInsertLen);

extern const char *App_ISPCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
extern const char *App_ISPKeyCGIHandler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);

#endif
