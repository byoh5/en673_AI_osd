//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
// -----------------------------------------------------------------------------
#include "dev.h"
#include "dev_inline.h"

/* ------------------------ OR32 includes -------------------------------- */
#include <stdio.h>
#include <string.h>

/* ------------------------ FreeRTOS includes ----------------------------- */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* ------------------------ lwIP includes --------------------------------- */
#include "lwip/err.h"
#include "lwip/api.h"

#ifdef __FAT_LOAD__
#if (ENX_FTPD_use==1)

#include "ftp_server.h"
#include "ftp_util.h"

int strchangnull(char *buf, const char *erase)
{
	char *cSearch = strstr(buf, erase);
	if(cSearch)
	{
		*cSearch = '\0';
		return 0;
	}
	return 1;
}

int strDriverChang(char *buf, const char *driver, const char *chgdriver)
{
	int nSize = strlen(driver);
	if(strncmp(buf, driver, nSize) == 0)	// buf를 driver길이만큼 driver과 비교하여 같으면
	{
		char bbuf[WORK_BUFFER_SIZE] = {0};
		snprintf(bbuf, WORK_BUFFER_SIZE, "%s%s", chgdriver, &buf[nSize]);
		strcpy(buf, bbuf);
		return 0;
	}
	return 1;
}

void FTP_strDriverChange(char* strPath, eFTP_Driver type)
{
	if(strPath == NULL || strPath[0] == '\0')
	{
		printf("%04d %s : strPath NULL!\r\n", __LINE__, __func__);
		return;
	}

	switch(type)
	{
		case FTP_Driver_ftp2fat:
#ifdef __FLS_FAT_LOAD__
			if(strstr(strPath, FLS_Driver_ROOT) != NULL)
			{
				strDriverChang(strPath, FLS_Driver_ROOT, FLS_RealDriver_ROOT);
			}
#ifdef __DEVICE_SD__
			else
#endif
#endif
#ifdef __DEVICE_SD__
			if(strstr(strPath, SD_Driver_ROOT) != NULL)
			{
				strDriverChang(strPath, SD_Driver_ROOT, SD_RealDriver_ROOT);
			}
#endif
			break;
		case FTP_Driver_fat2ftp:
#ifdef __FLS_FAT_LOAD__
			if(strstr(strPath, FLS_RealDriver_ROOT) != NULL)
			{
				strDriverChang(strPath, FLS_RealDriver_ROOT, FLS_Driver_ROOT);
			}
#ifdef __DEVICE_SD__
			else
#endif
#endif
#ifdef __DEVICE_SD__
			if(strstr(strPath, SD_RealDriver_ROOT) != NULL)
			{
				strDriverChang(strPath, SD_RealDriver_ROOT, SD_Driver_ROOT);
			}
#endif
			break;
		case FTP_Driver_delete:
#ifdef __FLS_FAT_LOAD__
			if(strstr(strPath, FLS_RealDriver_ROOT) != NULL)
			{
				strDriverChang(strPath, FLS_RealDriver_ROOT, "");
			}
#ifdef __DEVICE_SD__
			else
#endif
#endif
#ifdef __DEVICE_SD__
			if(strstr(strPath, SD_RealDriver_ROOT) != NULL)
			{
				strDriverChang(strPath, SD_RealDriver_ROOT, "");
			}
#endif
			else
			{
#ifdef __FLS_FAT_LOAD__
				if(strstr(strPath, FLS_Driver_ROOT) != NULL)
				{
					strDriverChang(strPath, FLS_Driver_ROOT, "");
				}
#ifdef __DEVICE_SD__
				else
#endif
#endif
#ifdef __DEVICE_SD__
				if(strstr(strPath, SD_Driver_ROOT) != NULL)
				{
					strDriverChang(strPath, SD_Driver_ROOT, "");
				}
#endif
			}
			break;
	}
}


char* strchrnul(const char *s, int c)
{
	while (*s != '\0' && *s != c)
		s++;
	return (char*)s;
}
#endif
#endif
