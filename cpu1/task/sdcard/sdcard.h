#ifndef __SDCARD_H__
#define __SDCARD_H__

enum {
	sd_OFF = 0,
	sd_INIT = 1,
	sd_IDLE = 2,
	sd_SAVE = 3,
	sd_ERR = 4
};

extern int SDcardGetFormatState(void);
extern void SDcardSetFormat(void);
extern void CheckSDstate(void *ctx);
extern int getSDState(void);

#endif
