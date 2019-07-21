#ifndef __SDCARD_CHECK_H__
#define __SDCARD_CHECK_H__

enum {
	sd_OFF,
	sd_INIT,
	sd_IDLE,	
	sd_READ,
	sd_SAVE,
	sd_FTP,
	sd_ERR
};


void CheckSDstate(void *pvParameters);
extern void check_camera_sdsave(void *ctx);
extern void check_sdcard_event(void *ctx);

#endif
