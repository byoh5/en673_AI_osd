#include "dev.h"
#include "dev_inline.h"															// for inline

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "user.h"
#include "msg.h"

#include "lwip/api.h"

///////////////////////////////////////////////////

#ifdef __DEVICE_SD__
#include "sdcard.h"
#include "fwupdate.h"
#include "file.h"
#include "muxer_avienc.h"
#include "avi_restore.h"

#define ENX_SDdetect_DEBUG			// printf 주석용

#ifdef ENX_SDdetect_DEBUG
	int printf(const char *fmt, ...);
	#define tprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define tprintf(fmt, args...) do {} while(0);
#endif

extern UINT bSD_FAT_mounted;

static int bSD_State = sd_OFF;
static int bSD_Format = 0;

int SDcardGetFormatState(void)
{
	return bSD_Format;
}

void SDcardSetFormat(void)
{
	bSD_Format = DEF_ON;
}

static void SDcardFormatFail(void)
{
	bSD_Format = -1;
}

static void SDcardFormatDone(void)
{
	bSD_Format = DEF_OFF;
}

static void SDcardInit(void)
{
	int flag = 1;
init_start:
	vTaskDelay(10);
	FRESULT fres;
	bSD_State = sd_INIT;
	Sdio0Init();
	if (Sdio0InitProcess() == DEF_FAIL) {
		tprintf("SD card IN(init fail)\r\n");
		bSD_State = sd_ERR;
		goto init_end;
	}
	if (flag == 2) {
		Sdio0ClockDiv(eSDIO_CLK_25MHz);
	}
	fres = drive_in(MMC); // driver mount
	if (flag == 1 && fres == FR_NO_FILESYSTEM) { // FS가 없다? Read Fail?
		Sdio0ClockDiv(eSDIO_CLK_25MHz);
		fres = drive_in(MMC); // driver mount
		if (fres == FR_NO_FILESYSTEM) { // FS가 정말 없다.
			flag = 1;
#if (SDIO0_SAFE_MODE==0)
			Sdio0ClockDiv(eSDIO_CLK_50MHz);
#elif (SDIO0_SAFE_MODE==1)
			Sdio0ClockDiv(eSDIO_CLK_33MHz);
#endif
		} else {
			flag = 2;
		}
	}

	switch (fres) {
		case FR_OK:
		case FR_NO_FILESYSTEM:
			tprintf("SD card IN\r\n");
			fres = drive_init(MMC);
			if(fres != FR_OK)		goto init_err; 
			flag = 2;
			if(fwupdate_filecheck(MMC, "EN673.bin"))
			{
				int update_ing;
				FWUPDATE_INFO* fwinfo = (FWUPDATE_INFO*)pvPortCalloc(1, sizeof(FWUPDATE_INFO));
				sprintf(fwinfo->file_path, "%d:/%s", MMC, "EN673.bin");	// SD에 펌웨어 파일이 올라와 있다. 올라온 주소를 저장한다.
				fwinfo->type = eFWT_SD;				// SD에 펌웨어 파일이 올라와 있다.
				fwinfo->is_areachange = DEF_YES;	// fw write 중 전원차단 등을 예방하기 위해 area을 변경 후 write한다.
				fwinfo->is_bootwrite = DEF_NO;		// boot.bin은 write하지 않는다.
				fwinfo->is_autoreboot = DEF_YES;	// Task 내부에서 재부팅을 시도한다.
				fwinfo->is_malloc = DEF_YES;		// 이 구조체는 malloc 했으므로 free를 위해 선언되었다.
				fwinfo->is_encodingstop = DEF_YES;	// Task 내부에서 encoding stop 명령을 내린다.
				sys_thread_new("fwup", fwupdate_Task, fwinfo, JABBU_STACK_SIZE, JABBU_TASK_PRIO);
				while((update_ing = fwupdate_process()) != 100)
				{
					printf("[%d%]", update_ing);
					vTaskDelay(50);
				}
			}
#ifdef __IDX_WRITE__
			avi_restore();
#endif
			if(gtUser.bSdAviSave == SDSAVE_USE_ON)
			{
				printf("SD save Start!\r\n");
				bSD_State = sd_SAVE;
				muxer_avienc_init();
				muxer_avienc_go(eRecNormal);
			}
			else
			{
				bSD_State = sd_IDLE;
			}
			break;
		default:
			flag = 2;
			tprintf("SD card IN(mount fail:err%d)\r\n", fres);
			bSD_State = sd_ERR;
			break;
	}

init_err:
	if (flag == 1) {
		flag = 2;
		goto init_start;
	}

init_end:
	return;
}

void CheckSDstate(void *ctx)
{
	FRESULT res;
	UINT oldSdSviSave = gtUser.bSdAviSave;

	vTaskDelay(300);	// It will run in 3 seconds.

	while (1) {
		if (!SDIO0_DET) { // SD card IN
			switch (bSD_State) {
				case sd_OFF:	// Initialization... This is done by "SDcardInit"
					SDcardInit();
					break;
				case sd_INIT:	// Do nothing until initialization is finished.
					break;
				case sd_IDLE:	// sd_IDLE state by "SDcardInit" or "sdsave off"
					if (SDcardGetFormatState() == DEF_ON) {
						if ((res=fat_sdmkfs(MMC, FAT_BLOCK_MMC))!=FR_OK) {
							printf("Error1\r\n");
							Sdio0ClockDiv(eSDIO_CLK_25MHz);
							if((res=fat_sdmkfs(MMC, FAT_BLOCK_MMC))!=FR_OK) {
								printf("Error2\r\n");
							}
						}

						if (res == FR_OK) {
							if((res=drive_init(MMC))!=FR_OK) {
								printf("Error3\r\n");
							}
						}

						if (res == FR_OK) {
							printf("SD Format & Init OK!\r\n");
							gtUser.bSdAviSave = oldSdSviSave;
							SDcardFormatDone();
						} else {
							printf("SD Format & Init Fail: %s\r\n", put_rc(res));
							SDcardFormatFail();
						}
					}

					if (gtUser.bSdAviSave == SDSAVE_USE_ON) { // user command(sdsave on)
						bSD_State = sd_SAVE;
						printf("SD save Start!\r\n");
						drive_init(MMC);
						muxer_avienc_init();
						muxer_avienc_go(eRecNormal);
					}
					break;
				case sd_SAVE:	// sd_SAVE state by "SDcardInitTask" or "sdsave on"
					if (SDcardGetFormatState() == DEF_ON) {
						oldSdSviSave = gtUser.bSdAviSave;
						if (gtUser.bSdAviSave == SDSAVE_USE_ON) {
							gtUser.bSdAviSave = SDSAVE_USE_OFF;
						}
					}

					if (gtUser.bSdAviSave == SDSAVE_USE_OFF) { // user command(sdsave off)
						bSD_State = sd_IDLE;
#ifdef GPIO_RECORDING_LED
						GpioSetLo(GPIO_RECORDING_LED);
#endif
						tprintf("SD save Stop\r\n");
						gAvienc[eRecNormal].state = AVIENC_EXIT;
						gAvienc[eRecEvent].state = AVIENC_EXIT;
					}
					break;
				case sd_ERR:
					tprintf("SD state Error!\r\n");
					break;
			}
		}
		else if(SDIO0_DET && (bSD_State != sd_OFF))	// SD card OUT
		{
			bSD_State = sd_OFF;
			bSD_FAT_mounted = 0;
			muxer_avienc_all_stop();
			drive_out(MMC); // driver unmount
			Sdio0Deinit();	// 400MHz set
			tprintf("SD card OUT\r\n");
#ifdef GPIO_RECORDING_LED
			GpioSetLo(GPIO_RECORDING_LED);
#endif
		}
		else if(bSD_State == sd_ERR)
		{
			static int cnt = 0;
			cnt++;
			if(cnt == 10*300)
			{
				tprintf("SDcard Error / reload\r\n");
				bSD_State = sd_OFF;
				cnt = 0;
			}
		}
		vTaskDelay(20);
	}
	UNUSED(ctx);
}

int getSDState(void)
{
	return bSD_State;
}
#endif
