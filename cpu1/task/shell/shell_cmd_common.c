//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include <stdlib.h>		// for atoi
#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sys_arch.h"

#include "lwip/init.h"
#include "lwip/sys.h"
#include "lwip/ip_addr.h"

#include "ff.h"

#include "shell.h"
#include "shell_cmd.h"
#include "shell_cmd_common.h"

#include "msg.h"
#include "user.h"

#include <time.h>
#include <malloc.h>		// for struct mallinfo, mallinfo

#include "init.h"		// for LoadDefUserInfo

#include "timer_task.h"
#include "sntpc.h"
#if (ENX_NETWORK_Debug==1)
#include "network_debugging.h"
#endif

#ifdef __RTC_LOAD__
#include "rtc.h"
#endif

const char *sUsrCmd0[]      = {":",                                              (char*)0};
const char *sUsrCmd1[]      = {":",                                              (char*)0};
const char *sUsrCmd2[]      = {":",                                              (char*)0};
const char *sUsrCmd3[]      = {":",                                              (char*)0};
const char *sUsrCmd4[]      = {":",                                              (char*)0};
const char *sUsrCmd5[]      = {":",                                              (char*)0};
const char *sInfo[]         = {"System Info",                                    (char*)0};
const char *sUsrCkIf[]      = {"Check Interface",                                (char*)0};
const char *sUsrGPIOState[] = {"User GPIO state",                                (char*)0};
const char *sReboot[]       = {"system reboot",                                  (char*)0};
#ifdef __AUDIO__
const char *sAudioCmd[]     = {"Audio On / Off",                                 (char*)0};
#endif
const char *sTimeCmd[]      = {"System Time, TimeZone Get / Set",                (char*)0};
const char *sTaskStatus[]   = {"show freeRTOS task status",                      (char*)0};
const char *sMemStatus[]    = {"show mem status",                                (char*)0};
const char *sXsrStatus[]    = {"show xsr status",                                (char*)0};
const char *sEnvvar[]       = {"Environment Variable Get / Set",                 (char*)0};
const char *sUserAreaCmd[]	= {"User Info Control",								 (char*)0};
const char *sUserInfoCmd[]  = {"User Info View",                                 (char*)0};
#ifdef __HW_OSD__
const char *sFontOSDCmd[]   = {"Video Font OSD Setting",                         (char*)0};
#endif
#ifdef __H264__
const char *sH264Info[]     = {"H.264 information",                              (char*)0};
const char *sH264CH1[]      = {"H.264 1ch status or setting",                    (char*)0};
const char *sH264CH2[]      = {"H.264 2ch status or setting",                    (char*)0};
#endif
#ifdef __JPEG__
const char *sJpegInfo[]     = {"JPEG information / status / setting",            (char*)0};
#endif

//*************************************************************************************************
// User Variable
//-------------------------------------------------------------------------------------------------
//


//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
INT16S cmd_time(INT32S argc, char *argv[])
{
	if (argc == 1) {
		printf("UPTIME : %us\r\n", gptMsgShare.UPTIME);
		printf("gptMsgShare.TIME : %d\r\n", gptMsgShare.TIME);

		struct tm tmout;

#if defined(__ETH__) ||defined(__WIFI__)
#if (ENX_SNTPC_use==1)
		printf("SNTP timeset : %s\r\n", sntp_info.is_timeset == 1 ? "OK" : "FAIL");
		if (sntp_info.is_timeset == 1) {
			enx_get_tmtime(sntp_info.last_gettime, &tmout, DEF_YES);
			printf("SNTP lastget(LOT) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		}
#endif
#endif

		printf("Summer Time : %s\r\n", gtUser.nTimeSummer ? "ON" : "OFF");
		TimeZone tz = getTimeZone();
		printf("TimeZone get : %d:%d(%d)\r\n", nTimeZoneSec[tz][0], nTimeZoneSec[tz][1], nTimeZoneSec[tz][2]);

		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
		printf("GMT : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
		printf("LOT : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);

#ifdef __RTC_LOAD__
		struct tm _tm;
		if (rtc_get_time(&_tm) == DEF_OK) {
			time_t tmk = mktime(&_tm);
			enx_get_tmtime(tmk, &tmout, DEF_NO);
			printf("GMT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
			enx_get_tmtime(tmk, &tmout, DEF_YES);
			printf("LOT(RTC) : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
		} else {
			printf("RTC time get fail\r\n");
		}
#endif
		printf("TICK 0(%d) 1(%d)\r\n", gptMsgDebug.CPU0_TICK, gptMsgDebug.CPU1_TICK);

		vTaskDelay(10);
	} else if (argc == 3 && (strcmp(argv[1], "zone") == 0)) {
		TimeZone tz;
		int nHour, nMin, i;
		sscanf(argv[2], "%d:%d", &nHour, &nMin);
		for (i = e_tzM1200; i <= e_tzP1400; i++) {
			if (nTimeZoneSec[i][0] == nHour && nTimeZoneSec[i][1] == nMin) {
				setTimeZone(i);
				tz = getTimeZone();
				printf("TimeZone set : %d:%d(%d)\r\n", nTimeZoneSec[tz][0], nTimeZoneSec[tz][1], nTimeZoneSec[tz][2]);
			}
		}
	} else if (argc == 7) {
		int nYear, nMonth, nDay, nHour, nMin, nSec;
		nYear = atoi(argv[1]);
		nMonth = atoi(argv[2]);
		nDay = atoi(argv[3]);
		nHour = atoi(argv[4]);
		nMin = atoi(argv[5]);
		nSec = atoi(argv[6]);
		if (set_devicetime(TimeZone_LOC, nYear, nMonth, nDay, nHour, nMin, nSec) == DEF_FAIL) {
			return 0;
		}
	} else {
		printf("CMD : time, timezone get\r\n");
		printf("	ex) time\r\n");
		printf("CMD : time set\r\n");
		printf("	format : [time YYYY MM DD hh mm ss]\r\n");
		printf("	ex) time set 2014 4 12 22 45 43\r\n");
		printf("CMD : time zone set\r\n");
		printf("	format : [time zone hh]\r\n");
		printf("	ex) time zone 9\r\n");
	}
	return 0;
}

//*************************************************************************************************
// User commands
//-------------------------------------------------------------------------------------------------
INT16S UsrCmd0(INT32S argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCmd1(INT32S argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCmd2(INT32S argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCmd3(INT32S argc, char *argv[])
{
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCmd4(INT32S argc, char *argv[])
{
	portENTER_CRITICAL();
	printf("CPU1 STOP!\r\n");
	while(1);
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCmd5(INT32S argc, char *argv[])
{
	if(argc == 2) {
		gptMsgDebug.dltFLAG = atoi(argv[1]);
	}
	printf("dltFLAG : %d\r\n", gptMsgDebug.dltFLAG);
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_info(INT32S argc, char *argv[])
{
	if (argc == 1) {
		printf("H/W info =================================\r\n");
		printf("CPU Clock             : %ukHz\r\n", 27 * 1000 * 1000 * SYS_PLL0_N / SYS_PLL0_K / SYS_PLL0_M / 1000);
		printf("ISP Clock             : %ukHz\r\n", (27 * 1000 * 1000 * SYS_PLL1_N / SYS_PLL1_K / SYS_PLL1_M / 1000) >> 1);
		printf("Model name            : [%s]\r\n", EN673_MODEL);

		printf("S/W info =================================\r\n");
		printf("Firmware compile date : %s%s\r\n", __DATE__, __TIME__);
		printf("EN673 Firmware Ver    : %X.%X.%X\r\n", (EN673_VERSION>>8)&0xf, (EN673_VERSION>>4)&0xf, EN673_VERSION&0xf);
		printf("freeRTOS Ver          : %s\r\n", "6.1.0");
		printf("lwIP Ver              : %u.%u.%u\r\n", LWIP_VERSION_MAJOR, LWIP_VERSION_MINOR, LWIP_VERSION_REVISION);
		printf("FatFS RevID           : %u\r\n", _FATFS);
		printf("Date ID               : 0x%08X\r\n", gtSystem.nDateID);
		printf("Binary address        : 0x%08X\r\n", gtSystem._pBootAddr);
	} else if (argc == 2 && (strcmp(argv[1], "log") == 0)) {
#if ENX_NETWORK_Debug
		static sys_thread_t stt_debugtask = NULL;
		if (stt_debugtask == NULL) {
			printf("Start debugging task\r\n");
			stt_debugtask = sys_thread_new("debug", Debugging_Task, NULL, _16KB_STACK_SIZE, LOW_TASK_PRIO);
		} else {
			vTaskDelete(stt_debugtask);
			printf("End debugging task\r\n");
		}
#endif
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrCheckInterface(INT32S argc, char *argv[])
{
	const char *strSwitch[] = {"Disabled", "Enabled"};
	const char *strMode[] = {"Master", "Slave"};
	UINT nBUSCLK = 27 * 1000 * 1000 * SYS_PLL0_N / SYS_PLL0_K / SYS_PLL0_M;
	printf("CPU Clock : %ukHz\r\n", nBUSCLK / 1000);
	printf("ISP Clock : %ukHz\r\n", (27 * 1000 * 1000 * SYS_PLL1_N / SYS_PLL1_K / SYS_PLL1_M / 1000) >> 1);

	printf("I2C0  SDA(%s) SCL(%s) MODE(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[1], strSwitch[1], I2C0_MODE == 0 ? strMode[0] : strMode[1], I2C0_CLK_DIV, (nBUSCLK / ((I2C0_CLK_DIV + 1) * 8)) / 1000);
	printf("I2C1  SDA(%s) SCL(%s) MODE(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[GpioGetFuncPin(GPIO_05)], strSwitch[GpioGetFuncPin(GPIO_06)], I2C1_MODE == 0 ? strMode[0] : strMode[1], I2C1_CLK_DIV, (nBUSCLK / ((I2C1_CLK_DIV + 1) * 8)) / 1000);
	printf("I2C2  SDA(%s) SCL(%s) MODE(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[GpioGetFuncPin(GPIO_18)], strSwitch[GpioGetFuncPin(GPIO_19)], I2C2_MODE == 0 ? strMode[0] : strMode[1], I2C2_CLK_DIV, (nBUSCLK / ((I2C2_CLK_DIV + 1) * 8)) / 1000);

	printf("UART0 RX(%s) TX(%s) DIV(%d) CLK(%dHz)\r\n", strSwitch[1], strSwitch[1], UART0_CLKDIV, (nBUSCLK / ((UART0_CLKDIV + 1) * 16)));
	printf("UART1 RX(%s) TX(%s) DIV(%d) CLK(%dHz)\r\n", strSwitch[GpioGetFuncPin(GPIO_14)], strSwitch[GpioGetFuncPin(GPIO_15)], UART1_CLKDIV, (nBUSCLK / ((UART1_CLKDIV + 1) * 16)));
	printf("UART2 RX(%s) TX(%s) DIV(%d) CLK(%dHz)\r\n", strSwitch[GpioGetFuncPin(GPIO_16)], strSwitch[GpioGetFuncPin(GPIO_17)], UART2_CLKDIV, (nBUSCLK / ((UART2_CLKDIV + 1) * 16)));

	printf("SPI0  EN(%s) CS_OEN(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[SPI0_EN], strSwitch[!SPI0_CS_OEN], SPI0_CLKDIV, (nBUSCLK / ((SPI0_CLKDIV + 1) * 4)) / 1000);
	printf("SPI1  EN(%s) CS_OEN(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[SPI1_EN], strSwitch[!SPI1_CS_OEN], SPI1_CLKDIV, (nBUSCLK / ((SPI1_CLKDIV + 1) * 4)) / 1000);
	printf("SPI2  EN(%s) CS_OEN(%s) DIV(%d) CLK(%dKHz)\r\n", strSwitch[SPI2_EN], strSwitch[!SPI2_CS_OEN], SPI2_CLKDIV, (nBUSCLK / ((SPI2_CLKDIV + 1) * 4)) / 1000);

	printf("SDIO0 EN(%s) CLKEN(%s) MODE(%s) DIV(%d)\r\n", strSwitch[SDIO0_EN], strSwitch[SDIO0_CLKEN], SDIO0_MODE == 0 ? strMode[1] : strMode[0], SDIO0_CLK_DIV);
	printf("SDIO1 EN(%s) CLKEN(%s) MODE(%s) DIV(%d)\r\n", strSwitch[SDIO1_EN], strSwitch[SDIO1_CLKEN], SDIO1_MODE == 0 ? strMode[1] : strMode[0], SDIO1_CLK_DIV);

	printf("ADC   EN(%s) CKEN(%s)\r\n", strSwitch[ADC_EN], strSwitch[ADC_CKEN]);
	printf("ADC0  EN(%s)\r\n", strSwitch[ADC_CH0_EN]);
	printf("ADC1  EN(%s)\r\n", strSwitch[ADC_CH1_EN]);
	printf("ADC2  EN(%s)\r\n", strSwitch[ADC_CH2_EN]);

	printf("TIM0  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH0_EN], strSwitch[TIM_CH0_PWM_EN]);
	printf("TIM1  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH1_EN], strSwitch[TIM_CH1_PWM_EN]);
	printf("TIM2  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH2_EN], strSwitch[TIM_CH2_PWM_EN]);
	printf("TIM3  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH3_EN], strSwitch[TIM_CH3_PWM_EN]);
	printf("TIM4  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH4_EN], strSwitch[TIM_CH4_PWM_EN]);
	printf("TIM5  EN(%s) PWM_EN(%s)\r\n", strSwitch[TIM_CH5_EN], strSwitch[TIM_CH5_PWM_EN]);

	printf("I2S   MODE(%s) ACK_PD(%s) TX_EN(%s) RX_EN(%s)\r\n", AUD_IO_MODE == 0 ? "Mic+PWM" : "I2S", strSwitch[AUD_ACK_PD], strSwitch[AUD_TX_EN], strSwitch[AUD_RX_EN]);

	printf("ETH   RX_EN(%s) RX_MACFILT(%s) RX_CRCFILT(%s)\r\n", strSwitch[ETH_RX_EN], strSwitch[ETH_RX_MACFILT], strSwitch[ETH_RX_CRCFILT]);

	printf("WDT   SYS_EN(%s) WDT0_EN(%s) WDT1_EN(%s)\r\n", strSwitch[SYS_WDT_EN], strSwitch[CPU_WDT0_EN], strSwitch[CPU_WDT1_EN]);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S UsrGPIOState(INT32S argc, char *argv[])
{
	char *gpio_mux_name[] = {
		"GPIO0",
		"SPI 1ch CS",
		"SPI 1ch DO",
		"SPI 1ch CK",
		"SPI 1ch DI",
		"I2C 1ch SDA",
		"I2C 1ch SCL",
		"GPIO7",
		"PWM 0ch",
		"PWM 1ch",
		"PWM 2ch",
		"PWM 3ch",
		"PWM 4ch",
		"PWM 5ch",
		"UART 2ch RX",
		"UART 2ch TX",
		"UART 1ch RX",
		"UART 1ch TX",
		"I2C 2ch SDA",
		"I2C 2ch SCL",
		"SPI 2ch DI",
		"SPI 2ch CS",
		"SPI 2ch DO",
		"SPI 2ch CK"
	};

	if (argc != 4 && argc != 1) {
		printf("Error Input!\r\n");
		printf("ex) %s <cmd:d:direction, s:state, f:function> <addr:0~23>\r\n", argv[0]);
		printf("       <cmd=>d : 0:OUT,  1:IN>\r\n");
		printf("       <cmd=>s : 0:Low,  1:High>\r\n");
		printf("       <cmd=>f : 0:GPIO, 1:Function>\r\n");
		return 0;
	}

	if (argc == 1) {
		int i;
		for (i = 0; i < 24; i++) {
			printf("GPIO%02d : Direction(%s) State(%s) Function(%s)\r\n", i, 
				GpioGetDir(i) == 1 ?  " In" :  "Out", 
				GpioGetPin(i) == 1 ? "High" : " Low", 
				GpioGetFuncPin(i) == 1 ? gpio_mux_name[i] : "GPIO");
		}
		printf("u-GPIO Reg\r\n");
		printf("u-GPIO_I        %08X\r\n", GPIO_I);
		printf("u-GPIO_O        %08X\r\n", GPIO_O);
		printf("u-GPIO_OEN      %08X\r\n", GPIO_OEN);
		printf("u-GPIO_EDGE_DIR %08X\r\n", GPIO_EDGE_DIR);
		printf("u-GPIO_IRQ_EN   %08X\r\n", GPIO_IRQ_EN);
		printf("u-GPIO_IRQ      %08X\r\n", GPIO_IRQ);
		printf("u-PAD_GPIO_PUEN %08X\r\n", PAD_GPIO_PUEN);
		return 0;
	}

	if (argc == 4) {
		BYTE addr = atoi(argv[2]);
		if (addr > 0x17) {
			printf("Error Input options [%d], GPIO 0 ~ 23\r\n", addr);
			return 0;
		}

		if (strcmp(argv[1], "d") == 0) {
			if (strcmp("0", argv[3]) == 0) {
				GpioOutDir(addr);
				printf("Set GPIO%02d : Direction(Out)\r\n", addr);
			} else if (strcmp("1", argv[3]) == 0) {
				GpioInDir(addr);
				printf("Set GPIO%02d : Direction(In)\r\n", addr);
			}
			return 0;
		} else if (strcmp(argv[1], "s") == 0) {
			if (strcmp("0", argv[3]) == 0) {
				GpioSetLo(addr);
				printf("Set GPIO%02d : State(Low)\r\n", addr);
			} else if (strcmp("1", argv[3]) == 0) {
				GpioSetHi(addr);
				printf("Set GPIO%02d : State(High)\r\n", addr);
			}
			return 0;
		} else if (strcmp(argv[1], "f") == 0) {
			if (strcmp("0", argv[3]) == 0) {
				GpioFuncPinOff(addr);
				printf("Set GPIO%02d : Function(GPIO%02d)\r\n", addr, addr);
			} else if (strcmp("1", argv[3]) == 0) {
				GpioFuncPin(addr);
				printf("Set GPIO%02d : Function(%s)\r\n", addr, gpio_mux_name[addr]);
			}
			return 0;
		} else {
			printf("Error Input options0 [%s]\r\n", argv[1]);
			return 0;
		}
		printf("Error Input options1 [%s]\r\n", argv[2]);
		return 0;
	}

	printf("Error Input options [%d]\r\n", argc);
	return 0;
}

INT16S UseReboot(INT32S argc, char *argv[])
{
	printf("System Reboot!\r\n\r\n");
	vTaskDelay(10);
	WdtSysReboot();

	return 0;

	UNUSED(argc);
	UNUSED(argv);
}

#ifdef __AUDIO__
INT16S UsrAudioCmd(INT32S argc, char *argv[])
{
	if (argc != 2) {
		printf("Error : ex) audio on   // Audio Stream on\r\n");
		printf("        ex) audio off  // Audio Stream off\r\n");
		return 0;
	}

	if (strcmp("on", argv[1]) == 0) {
		if (gtUser.bAudio != DEF_ON) {
			printf("AUDIO ON\r\n");
			gtUser.bAudio = DEF_ON;
			while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_AUDIO, DEF_ON) == DEF_FAIL) {
				vTaskDelay(1); // Time update request to CPU0
			}
		} else {
			printf("Error : Already ON\r\n");
		}
	} else if (strcmp("off", argv[1]) == 0) {
		if (gtUser.bAudio != DEF_OFF) {
			printf("AUDIO OFF\r\n");
			gtUser.bAudio = DEF_OFF;
			while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_AUDIO, DEF_OFF) == DEF_FAIL) {
				vTaskDelay(1); // Time update request to CPU0
			}
		} else {
			printf("Error : Already OFF\r\n");
		}
	} else {
		Shell_Unknown();
		printf("Error : option 'on' or 'off'\r\n");
	}

	return 0;
}
#endif

INT16S cmd_task_stst(INT32S argc, char *argv[])
{
	unsigned portBASE_TYPE num_of_task = uxTaskGetNumberOfTasks();
#if 1
	unsigned portBASE_TYPE i;
	EnxTaskInfo *etiList = (EnxTaskInfo *)pvPortCalloc(sizeof(EnxTaskInfo), num_of_task);
	EYENIX_vTaskGetRunTimeCntNStats(etiList, num_of_task);
	printf("---------------------------------Task List(%02d)----------------------------------\r\n", num_of_task);
	printf("  Task     State  Priority  Stack     Number   RunTimeCounter  Percentage\r\n");
	printf("--------------------------------------------------------------------------------\r\n");
	for (i = 0; i < num_of_task; i++) {
		printf(" %-8s    %c       %u      %5u   0x%08X     %10u  %9.1f%\r\n", etiList[i].pcTaskName, etiList[i].cStatus, etiList[i].uxPriority, 
			etiList[i].usStackRemaining, etiList[i].pxNextTCB, etiList[i].ulRunTimeCounter, etiList[i].ulStatsAsPermil/10.0);
	}
	printf("--------------------------------------------------------------------------------\r\n");
	printf("-State(S:Suspended D:Deleted B:Blocked R:Ready) TotalRunTimeCounter(%10u)-\r\n", xTaskGetTickCount());
	vPortFree(etiList);
#else
	signed char *pbuf = pvPortCalloc(52, num_of_task);
	printf("------------------Task List(%02d)-----------------\r\n", num_of_task);
	printf("Task          State   Priority  Stack   Number\r\n");
	printf("------------------------------------------------\r\n");
	vTaskList(pbuf);
	char *ptr = strtok((char *)pbuf, "\r\n");
	while(ptr != NULL) {
		printf("%s\r\n", ptr);
		ptr = strtok(NULL, "\r\n");
	}
	printf("State S:Suspended D:Deleted B:Blocked R:Ready\r\n");
	printf("------------------------------------------------\r\n");
	printf("Name      RunTimeCounter        Percentage\r\n");
	printf("------------------------------------------------\r\n");
	vTaskGetRunTimeStats(pbuf);
	ptr = strtok((char *)pbuf, "\r\n");
	while(ptr != NULL) {
		printf("%s\r\n", ptr);
		ptr = strtok(NULL, "\r\n");
	}
	printf("------------------------------------------------\r\n");
	vPortFree(pbuf);
#endif

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_mem_stst(INT32S argc, char *argv[])
{
	const char *strline = "*********************************************************\r\n";
	const char *strmemory = "%15s : 0x%08X ~ 0x%08X(%dKB)\r\n";

	if (argc == 2 && strcmp(argv[1], "map") == 0) {
		printf("***********************ADDRESS MAP***********************\r\n");
		printf(strmemory, "CPU0_ROM_BASE", CPU0_ROM_BASE, CPU0_ROM_BASE + CPU0_ROM_SIZE - 1, CPU0_ROM_SIZE>>10);
		printf(strmemory, "CPU1_ROM_BASE", CPU1_ROM_BASE, CPU1_ROM_BASE + CPU1_ROM_SIZE - 1, CPU1_ROM_SIZE>>10);
		printf(strline);
		printf(strmemory, "CPU0_HEAP_BASE", CPU0_HEAP_BASE, CPU0_HEAP_END - 1, CPU0_HEAP_SIZE>>10);
		printf(strmemory, "CPU1_HEAP_BASE", CPU1_HEAP_BASE, CPU1_HEAP_END - 1, CPU1_HEAP_SIZE>>10);
		printf(strline);
		printf(strmemory, "ISP_BUF_BASE", ISP_BUF_BASE, ISP_BUF_END - 1, ISP_BUF_SIZE>>10);
#ifdef JTAG_DEBUG_PRINT
		printf(strmemory, "JTAG_PRINT0", JTAG_PRINT0_BASE, JTAG_PRINT0_END - 1, JTAG_PRINT_SIZE>>10);
		printf(strmemory, "JTAG_PRINT1", JTAG_PRINT1_BASE, JTAG_PRINT1_END - 1, JTAG_PRINT_SIZE>>10);
#endif
		printf(strmemory, "NETRX0_BUF_BASE", NETRX0_BUF_BASE, NETRX0_BUF_END - 1, NETRX_BUF_SIZE>>10);
		printf(strmemory, "NETRX1_BUF_BASE", NETRX1_BUF_BASE, NETRX1_BUF_END - 1, NETRX_BUF_SIZE>>10);
#if (NET_TX_BUFFER==1)
		printf(strmemory, "NETTX_BUF_BASE", NETTX_BUF_BASE, NETTX_BUF_END - 1, NETTX_BUF_SIZE>>10);
#endif
#ifdef __AUDIO__
		printf(strmemory, "AUD_DATA_BASE", AUD_DATA_BASE, AUD_DATA_END - 1, AUDIO_BUF_SIZE>>10);
		printf(strmemory, "AUD_SPKBUF_BASE", AUD_SPKBUF_BASE, AUD_SPKBUF_END - 1, AUDIO_BUF_SIZE>>10);
#endif
#ifdef RTMP_SUPPORT
		printf(strmemory, "RTMP_BASE", RTMP_CODEC_BASE, RTMP_AUDIO_END - 1, (RTMP_CODEC_SIZE+RTMP_VIDEO_SIZE+RTMP_AUDIO_SIZE)>>10);
#endif
		printf(strline);
#ifdef __JPEG__
		printf(strmemory, "JPEG_IMG_BASE", JPEG_IMG_BASE, JPEG_IMG_END - 1, JPEG_IMG_SIZE>>10);
#endif
#ifdef __H264__
		printf(strmemory, "H264_BASE", H264_STRM_BASE, H264_STRM_END - 1, H264_STRM_SIZE>>10);
#endif
		printf(strline);
		printf(strmemory, "FW_UPDATE_BASE", FW_UPDATE_BASE, FW_UPDATE_END - 1, FW_UPDATE_SIZE>>10);
		printf(strline);
		printf(strmemory, "Internal-SRAM", SYSRAM_BASE, (SYSRAM_BASE+SYSRAM_SIZE) - 1, SYSRAM_SIZE>>10);
		printf(strmemory, "Share-SRAM0", SH0RAM_BASE, (SH0RAM_BASE+SH0RAM_SIZE) - 1, SH0RAM_SIZE>>10);
		printf(strmemory, "Share-SRAM1", SH1RAM_BASE, (SH1RAM_BASE+SH1RAM_SIZE) - 1, SH1RAM_SIZE>>10);
		printf(strline);
	} else {
		struct mallinfo minfo;
		minfo = mallinfo();
		printf("\r\n********************CPU%d Memory state********************\r\n", CpuIdGet());
		printf("arena : %u\r\n", (unsigned int) minfo.arena);			// This is the total size of memory allocated with sbrk by malloc, in bytes.
//		printf("sbmlks : %u\r\n", (unsigned int) minfo.smblks);			// This field is unused.
//		printf("hblks : %u\r\n", (unsigned int) minfo.hblks); 			// This is the total number of chunks allocated with mmap.
		printf("ordblks : %u\r\n", (unsigned int) minfo.ordblks);		// This is the number of chunks not in use.
//		printf("hblkhd : %u\r\n", (unsigned int) minfo.hblkhd);			// This is the total size of memory allocated with mmap, in bytes.
//		printf("usmblks : %u\r\n", (unsigned int) minfo.usmblks); 		// This field is unused.
//		printf("fsmblks : %u\r\n", (unsigned int) minfo.fsmblks); 		// This field is unused.
		printf("uordblks : %u\r\n", (unsigned int) minfo.uordblks);		// This is the total size of memory occupied by chunks handed out by malloc.
		printf("fordblks : %u\r\n", (unsigned int) minfo.fordblks);		// This is the total size of memory occupied by free (not in use) chunks.
		printf("keepcost : %u\r\n", (unsigned int) minfo.keepcost);		// This is the size of the top-most releasable chunk that normally borders the end of the heap.
		printf("*********************************************************\r\n");

		vTaskDelay(1);

		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_MEMSTAT, 0) == DEF_FAIL) {
			vTaskDelay(1); // Time update request to CPU0
		}

		vTaskDelay(50);
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_xsr_stst(INT32S argc, char *argv[])
{
	int i;
	printf("CPU%d xsr list\r\n", CpuIdGet());
	for(i=0;i<=14;i++)
	{
		printf("%02d:%s:\t0x%08X\r\n", i, get_xsr_str(i), get_xsr(i));
	}

	vTaskDelay(10);

	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_XSR, 0) == DEF_FAIL)
		vTaskDelay(1); // Time update request to CPU0

	vTaskDelay(50);

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

#ifdef __HW_OSD__
INT16S cmd_fontosd(INT32S argc, char *argv[])
{
	if (argc == 1) {
		printf("Font OSD Status\r\n");
#ifdef __HW_OSD_FRAME__
		printf("    FrameNum  : %s\r\n", gptMsgShare.VIDEO_OSD & OSD_FRAME ? "ON" : "OFF");
#endif
#ifdef __HW_OSD_TIME__
		printf("    Time      : %s\r\n", gptMsgShare.VIDEO_OSD & OSD_TIME ? "ON" : "OFF");
#endif
#ifdef __HW_OSD_H264__
		printf("    H.264 1CH : %s\r\n", gptMsgShare.VIDEO_OSD & OSD_H264_1 ? "ON" : "OFF");
		printf("    H.264 2CH : %s\r\n", gptMsgShare.VIDEO_OSD & OSD_H264_2 ? "ON" : "OFF");
#endif
#ifdef __HW_OSD_JPEG__
		printf("    JPEG      : %s\r\n", gptMsgShare.VIDEO_OSD & OSD_JPEG ? "ON" : "OFF");
#endif
	} else if (argc == 3) {
		UINT getValue = atoi(argv[2]);
#ifdef __HW_OSD_FRAME__
		if (strcmp(argv[1], "frame") == 0) {
			if (getValue == 0) {
				printf("Frame osd off\r\n");
				gptMsgShare.VIDEO_OSD &= ~OSD_FRAME;
			} else {
				printf("Frame osd on\r\n");
				gptMsgShare.VIDEO_OSD |= OSD_FRAME;
			}
		} else
#endif
#ifdef __HW_OSD_TIME__
		if (strcmp(argv[1], "time") == 0) {
			if (getValue == 0) {
				printf("Time osd off\r\n");
				gptMsgShare.VIDEO_OSD &= ~OSD_TIME;
			} else {
				printf("Time osd on\r\n");
				gptMsgShare.VIDEO_OSD |= OSD_TIME;
			}
		}
		else
#endif
#ifdef __HW_OSD_H264__
		if (strcmp(argv[1], "h1") == 0) {
			if (getValue == 0) {
				printf("H.264 main channel osd off\r\n");
				gptMsgShare.VIDEO_OSD &= ~OSD_H264_1;
			} else {
				printf("H.264 main channel osd on\r\n");
				gptMsgShare.VIDEO_OSD |= OSD_H264_1;
			}
		} else if (strcmp(argv[1], "h2") == 0) {
			if (getValue == 0) {
				printf("H.264 sub channel osd off\r\n");
				gptMsgShare.VIDEO_OSD &= ~OSD_H264_2;
			} else {
				printf("H.264 sub channel osd on\r\n");
				gptMsgShare.VIDEO_OSD |= OSD_H264_2;
			}
		} else
#endif
#ifdef __HW_OSD_JPEG__
		if (strcmp(argv[1], "jpeg") == 0) {
			if (getValue == 0) {
				printf("JPEG osd off\r\n");
				gptMsgShare.VIDEO_OSD &= ~OSD_JPEG;
			} else {
				printf("JPEG osd on\r\n");
				gptMsgShare.VIDEO_OSD |= OSD_JPEG;
			}
		} else
#endif
		{
			Shell_Unknown();
		}
	} else {
		Shell_Unknown();
	}
	return 0;
}
#endif

#ifdef __H264__
INT16S cmd_h264_info(INT32S argc, char *argv[])
{
	if (argc == 5) {
		if (strcmp(argv[1], "simmap") == 0) {
#define H264_ALIGN_SIZE   	4096
#define H264_ALIGN(x) 		(((x) + H264_ALIGN_SIZE - 1) & ~(H264_ALIGN_SIZE - 1))

			UINT h264_buf_addr = H264_STRM_BASE;
			UINT h264_streambuf_size = atoi(argv[2]);
			UINT h264_width = atoi(argv[3]);
			UINT h264_height = atoi(argv[4]);

			UINT horizontal_buffer_size, vertical_buffer_size, reconstruction_buffer_size;
			UINT isize, ysize_a, csize_a;
			UINT paddr = h264_buf_addr;
			UINT STRMSIZE = h264_streambuf_size;

			horizontal_buffer_size = ((h264_width + 15) >> 4) << 4;
			vertical_buffer_size = ((h264_height + 15) >> 4) << 4;
			reconstruction_buffer_size = horizontal_buffer_size * vertical_buffer_size;

			ysize_a = reconstruction_buffer_size;
			csize_a = reconstruction_buffer_size >> 1;
			isize = ysize_a + csize_a;

			UINT bufsize_ysize = ysize_a;
			UINT bufsize_csize = csize_a;

			UINT stp_size = ysize_a;
			UINT stp_info_size = 8 * 1024;

			UINT stp_info_addr0 = H264_ALIGN(paddr);
			UINT stp_info_addr1 = H264_ALIGN(stp_info_addr0 + stp_info_size);

			UINT stp_addr0 = H264_ALIGN(stp_info_addr1 + stp_info_size);
			UINT stp_addr1 = H264_ALIGN(stp_addr0 + stp_size);

			UINT cur_y_addr0 = H264_ALIGN(stp_addr1 + stp_size);
			UINT cur_c_addr0 = H264_ALIGN(cur_y_addr0 + bufsize_ysize);

			UINT cur_y_addr1 = H264_ALIGN(cur_c_addr0 + bufsize_csize);
			UINT cur_c_addr1 = H264_ALIGN(cur_y_addr1 + bufsize_ysize);

			UINT rec_y_addr = H264_ALIGN(cur_c_addr1 + bufsize_csize);
			UINT rec_c_addr = H264_ALIGN(rec_y_addr + bufsize_ysize);

			UINT ref_y_addr = H264_ALIGN(rec_c_addr + bufsize_csize);
			UINT ref_c_addr = H264_ALIGN(ref_y_addr + bufsize_ysize);

			UINT stream_addr = H264_ALIGN(ref_c_addr + bufsize_csize);
			UINT stream = H264_ALIGN(stream_addr);
			UINT stream_base = stream;
			UINT stream_end = H264_ALIGN(stream_base + STRMSIZE);

			paddr = stream_end;

			printf(" ---		H264 Information		---\r\n");
			printf("Channel 	   : %d\r\n", 0);
			printf("├Param Info\r\n");
			printf("│└Image Size : %d x %d\r\n", h264_width, h264_height);
			printf("├Stream Info\r\n");
			printf("│├unit size  : STP(%dByte), INFO(%dByte)\r\n", stp_size, stp_info_size);
			printf("│├STP0	   : STP(0x%08X~0x%08X), INFO(0x%08X~0x%08X)\r\n", stp_addr0, stp_addr0+stp_size, stp_info_addr0, stp_info_addr0+stp_info_size);
			printf("│├STP1	   : STP(0x%08X~0x%08X), INFO(0x%08X~0x%08X)\r\n", stp_addr1, stp_addr1+stp_size, stp_info_addr1, stp_info_addr1+stp_info_size);
			printf("│└SIZE	   : %d Bytes\r\n", (stp_addr1 + stp_size - stp_info_addr0));
			printf("├Buffer Info\r\n");
			printf("│├unit size  : Y(%dByte), C(%dByte)\r\n", bufsize_ysize, bufsize_csize);
			printf("│├CUR0	   : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", cur_y_addr0, cur_y_addr0+bufsize_ysize, cur_c_addr0, cur_c_addr0+bufsize_csize);
			printf("│├CUR1	   : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", cur_y_addr1, cur_y_addr1+bufsize_ysize, cur_c_addr1, cur_c_addr1+bufsize_csize);
			printf("│├REC 	   : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", rec_y_addr, rec_y_addr+bufsize_ysize, rec_c_addr, rec_c_addr+bufsize_csize);
			printf("│├REF 	   : Y(0x%08X~0x%08X), C(0x%08X~0x%08X)\r\n", ref_y_addr, ref_y_addr+bufsize_ysize, ref_c_addr, ref_c_addr+bufsize_csize);
			printf("│└SIZE	   : %d Bytes\r\n", (ref_c_addr + bufsize_csize - cur_y_addr0));
			printf("└Stream Buffer\r\n");
			printf("  ├BASE	   : 0x%08X ~ 0x%08X\r\n", stream_base, stream_end);
			printf("  └SIZE	   : %d Bytes\r\n", stream_end - stream_base);
			printf("Total Size	   : %d Bytes(0x%08X)\r\n", (stream_end - h264_buf_addr), (stream_end - h264_buf_addr));
		}
	} else if (argc == 3) {
		if (strcmp(argv[1], "log") == 0) {
			gptMsgDebug.H264_POLL_LOG = atoi(argv[2]);
		} else if (strcmp(argv[1], "opw") == 0) {
			UINT getValue1 = atoi(argv[2]);
			printf("Set H264_OPWAIT_T 0x%08X\r\n", getValue1);
			H264_OPWAIT_T = getValue1;
		} else {
			Shell_Unknown();
		}
	} else if (argc == 2) {
		if (strcmp(argv[1], "info") == 0) {
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_H264_INFO, 0) == DEF_FAIL)	vTaskDelay(1);
			vTaskDelay(50);
		} else if (strcmp(argv[1], "start") == 0) {
			gptMsgShare.H264_ENC_ENABLE = 1;
			vTaskDelay(10);
		} else if (strcmp(argv[1], "startkill") == 0) {
			gptMsgShare.H264_ENC_ENABLE = 1;
			SYS_CPU1_RST = 0;
		} else if (strcmp(argv[1], "stop") == 0) {
			gptMsgShare.H264_ENC_ENABLE = 0;
			vTaskDelay(10);
		} else if (strcmp(argv[1], "reset") == 0) {
			printf("MSG_CMD_SET_H264_INIT start\r\n");
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_INIT, 0) == DEF_FAIL)		vTaskDelay(1);
			vTaskDelay(10);
		} else if (strcmp(argv[1], "restart") == 0) {
			vTaskDelay(10);

			SYS_H264_RST = 0;
			printf("H264 RST = %d\r\n", SYS_H264_RST);
			vTaskDelay(10);

			printf("MSG_CMD_SET_H264_INIT start\r\n");
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_INIT, 0) == DEF_FAIL)		vTaskDelay(1);
			printf("MSG_CMD_SET_H264_INIT ok\r\n");
			vTaskDelay(10);

			gtUser.uviH264[0].eResolution = IMG_H264_RES;
//			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_FPS, gtUser.uviH264[0].sfFPS) == DEF_FAIL) vTaskDelay(1);
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_SIZE, gtUser.uviH264[0].eResolution) == DEF_FAIL)	vTaskDelay(1);

			printf("MSG_CMD_SET_H264_READY start\r\n");
			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_READY, 0) == DEF_FAIL)	vTaskDelay(1);
			printf("MSG_CMD_SET_H264_READY ok\r\n");
		} else if (strcmp(argv[1], "state") == 0) {
			printf("H264_IHSIZE     : 0x%08X\r\n", H264_IHSIZE);
			printf("H264_IVSIZE     : 0x%08X\r\n", H264_IVSIZE);
			printf("H264_YSTRIDEX   : 0x%08X\r\n", H264_YSTRIDEX);
			printf("H264_CSTRIDEX   : 0x%08X\r\n", H264_CSTRIDEX);
			printf("H264_TMBNUM     : 0x%08X\r\n", H264_TMBNUM);
			printf("H264_IDR_FLAG   : 0x%08X\r\n", H264_IDR_FLAG);
			printf("H264_PFRAME     : 0x%08X\r\n", H264_PFRAME);
			printf("H264_QSCALE     : 0x%08X\r\n", H264_QSCALE);
			printf("H264_ALN32      : 0x%08X\r\n", H264_ALN32);
			printf("H264_QUANTMODE  : 0x%08X\r\n", H264_QUANTMODE);
			printf("H264_RINGTYPE   : 0x%08X\r\n", H264_RINGTYPE);
			printf("H264_PPS_QP     : 0x%08X\r\n", H264_PPS_QP);
			printf("H264_SRCHMODE   : 0x%08X\r\n", H264_SRCHMODE);
			printf("H264_FRMBITS    : 0x%08X\r\n", H264_FRMBITS);
			printf("H264_FRMNUM     : 0x%08X\r\n", H264_FRMNUM);
			printf("H264_PROFILEIDC : 0x%08X\r\n", H264_PROFILEIDC);
			printf("H264_CABACIDC   : 0x%08X\r\n", H264_CABACIDC);
			printf("H264_CABACEN    : 0x%08X\r\n", H264_CABACEN);
			printf("H264_CROPBOT    : 0x%08X\r\n", H264_CROPBOT);
			printf("H264_LEVIDC     : 0x%08X\r\n", H264_LEVIDC);
			printf("H264_SPS_SPSID  : 0x%08X\r\n", H264_SPS_SPSID);
			printf("H264_PPS_SPSID  : 0x%08X\r\n", H264_PPS_SPSID);
			printf("H264_PPS_PPSID  : 0x%08X\r\n", H264_PPS_PPSID);
			printf("H264_SLH_PPSID  : 0x%08X\r\n", H264_SLH_PPSID);
			printf("H264_SPSINSRT   : 0x%08X\r\n", H264_SPSINSRT);
			printf("H264_PPSINSRT   : 0x%08X\r\n", H264_PPSINSRT);
			printf("H264_PREPTR     : 0x%08X\r\n", H264_PREPTR);
			printf("H264_PSTPTR     : 0x%08X\r\n", H264_PSTPTR);
			printf("H264_INTRVC     : 0x%08X\r\n", H264_INTRVC);
			printf("H264_CURYBUF    : 0x%08X\r\n", H264_CURYBUF);
			printf("H264_CURCBUF    : 0x%08X\r\n", H264_CURCBUF);
			printf("H264_CUREBUF    : 0x%08X\r\n", H264_CUREBUF);
			printf("H264_REFYBUF    : 0x%08X\r\n", H264_REFYBUF);
			printf("H264_REFCBUF    : 0x%08X\r\n", H264_REFCBUF);
			printf("H264_RECYBUF    : 0x%08X\r\n", H264_RECYBUF);
			printf("H264_RECCBUF    : 0x%08X\r\n", H264_RECCBUF);
			printf("H264_STRMBUF    : 0x%08X\r\n", H264_STRMBUF);
			printf("H264_SBUFEND    : 0x%08X\r\n", H264_SBUFEND);
			printf("H264_STPSBUFW   : 0x%08X\r\n", H264_STPSBUFW);
			printf("H264_STPIBUFW   : 0x%08X\r\n", H264_STPIBUFW);
			printf("H264_STPIBUFR   : 0x%08X\r\n", H264_STPIBUFR);
			printf("H264_SBUFSIZE   : 0x%08X\r\n", H264_SBUFSIZE);
			printf("H264_VLCBURST   : 0x%08X\r\n", H264_VLCBURST);
			printf("H264_ENCBURST   : 0x%08X\r\n", H264_ENCBURST);
			printf("H264_USEIADDR   : 0x%08X\r\n", H264_USEIADDR);
			printf("H264_KEEP4KB    : 0x%08X\r\n", H264_KEEP4KB);
			printf("H264_CLKRATIO   : 0x%08X\r\n", H264_CLKRATIO);
			printf("H264_KEEPORDER  : 0x%08X\r\n", H264_KEEPORDER);
			printf("H264_RNDROBIN   : 0x%08X\r\n", H264_RNDROBIN);
			printf("H264_EXTROBIN   : 0x%08X\r\n", H264_EXTROBIN);
			printf("H264_BIGENDIAN  : 0x%08X\r\n", H264_BIGENDIAN);
			printf("H264_STRMRESET  : 0x%08X\r\n", H264_STRMRESET);
			printf("H264_RINGINIT   : 0x%08X\r\n", H264_RINGINIT);
			printf("H264_VLCSTART   : 0x%08X\r\n", H264_VLCSTART);
			printf("H264_FLUSHSTART : 0x%08X\r\n", H264_FLUSHSTART);
			printf("H264_VLCREADY   : 0x%08X\r\n", H264_VLCREADY);
			printf("H264_ENCSTART   : 0x%08X\r\n", H264_ENCSTART);
			printf("H264_SBUFFULL   : 0x%08X\r\n", H264_SBUFFULL);
			printf("H264_VLCEND     : 0x%08X\r\n", H264_VLCEND);
			printf("H264_FLUSHEND   : 0x%08X\r\n", H264_FLUSHEND);
			printf("H264_EVEND      : 0x%08X\r\n", H264_EVEND);
			printf("H264_ENCEND     : 0x%08X\r\n", H264_ENCEND);
			printf("H264_GENBITS    : 0x%08X\r\n", H264_GENBITS);
			printf("H264_SBUFWPOS   : 0x%08X\r\n", H264_SBUFWPOS);
			printf("H264_SERRMASK   : 0x%08X\r\n", H264_SERRMASK);
			printf("H264_EERRMASK   : 0x%08X\r\n", H264_EERRMASK);
			printf("H264_INTRMASK4  : 0x%08X\r\n", H264_INTRMASK4);
			printf("H264_INTRMASK3  : 0x%08X\r\n", H264_INTRMASK3);
			printf("H264_INTRMASK2  : 0x%08X\r\n", H264_INTRMASK2);
			printf("H264_INTRMASK1  : 0x%08X\r\n", H264_INTRMASK1);
			printf("H264_INTRMASK0  : 0x%08X\r\n", H264_INTRMASK0);
			printf("H264_CURWAIT    : 0x%08X\r\n", H264_CURWAIT);
			printf("H264_REFWAIT    : 0x%08X\r\n", H264_REFWAIT);
			printf("H264_VLCWAIT    : 0x%08X\r\n", H264_VLCWAIT);
			printf("H264_SFTBITS    : 0x%08X\r\n", H264_SFTBITS);
			printf("H264_SFTDATA    : 0x%08X\r\n", H264_SFTDATA);
			printf("H264_SFTWAIT    : 0x%08X\r\n", H264_SFTWAIT);
			printf("H264_ECHNNEL    : 0x%08X\r\n", H264_ECHNNEL);
			printf("H264_MCHNNEL    : 0x%08X\r\n", H264_MCHNNEL);
			printf("H264_PREBITS    : 0x%08X\r\n", H264_PREBITS);
			printf("H264_MEMYBGN    : 0x%08X\r\n", H264_MEMYBGN);
			printf("H264_MEMYEND    : 0x%08X\r\n", H264_MEMYEND);
			printf("H264_MEMCBGN    : 0x%08X\r\n", H264_MEMCBGN);
			printf("H264_MEMCEND    : 0x%08X\r\n", H264_MEMCEND);
			printf("H264_MIHSIZE    : 0x%08X\r\n", H264_MIHSIZE);
			printf("H264_MIVSIZE    : 0x%08X\r\n", H264_MIVSIZE);
			printf("H264_MCABACIDC  : 0x%08X\r\n", H264_MCABACIDC);
			printf("H264_MCABACEN   : 0x%08X\r\n", H264_MCABACEN);
			printf("H264_MQSCALE    : 0x%08X\r\n", H264_MQSCALE);
			printf("H264_MPFRAME    : 0x%08X\r\n", H264_MPFRAME);
		}
#if 0
		else if(strcmp(argv[1], "isp") == 0)
		{
			printf("BT Setting==========\r\n");
			printf("BT_DS_HWI0	: 0x%04X(%d)\r\n", ISP_BT_DS_HWI0, ISP_BT_DS_HWI0);
			printf("BT_DS_HWI1	: 0x%04X(%d)\r\n", ISP_BT_DS_HWI1, ISP_BT_DS_HWI1);
			printf("BT0 Setting=========\r\n");
			printf("BT0_ISEL	: 0x%04X(%d)\r\n", ISP_BT0_ISEL, ISP_BT0_ISEL);
			printf("BT0_HWSEL	: 0x%04X(%d)\r\n", ISP_BT0_HWSEL, ISP_BT0_HWSEL);
			printf("BT0_VISEL	: 0x%04X(%d)\r\n", ISP_BT0_VISEL, ISP_BT0_VISEL);
			printf("BT1 Setting=========\r\n");
			printf("BT1_ISEL	: 0x%04X(%d)\r\n", ISP_BT1_ISEL, ISP_BT1_ISEL);
			printf("BT1_HWSEL	: 0x%04X(%d)\r\n", ISP_BT1_HWSEL, ISP_BT1_HWSEL);
			printf("BT1_VISEL	: 0x%04X(%d)\r\n", ISP_BT1_VISEL, ISP_BT1_VISEL);
			printf("CDS0 Setting========\r\n");
			printf("CDS0_PCK_PD : 0x%04X(%d)\r\n", ISP_CDS0_PCK_PD, ISP_CDS0_PCK_PD);
			printf("CDS0_PCK_SEL: 0x%04X(%d)\r\n", ISP_CDS0_PCK_SEL, ISP_CDS0_PCK_SEL);
			printf("CDS0_ISEL	: 0x%04X(%d)\r\n", ISP_CDS0_ISEL, ISP_CDS0_ISEL);
			printf("CDS0_VISEL	: 0x%04X(%d)\r\n", ISP_CDS0_VISEL, ISP_CDS0_VISEL);
			printf("CDS1 Setting========\r\n");
			printf("CDS1_PCK_PD : 0x%04X(%d)\r\n", ISP_CDS1_PCK_PD, ISP_CDS1_PCK_PD);
			printf("CDS1_PCK_SEL: 0x%04X(%d)\r\n", ISP_CDS1_PCK_SEL, ISP_CDS1_PCK_SEL);
			printf("CDS1_ISEL	: 0x%04X(%d)\r\n", ISP_CDS1_ISEL, ISP_CDS1_ISEL);
			printf("CDS1_VISEL	: 0x%04X(%d)\r\n", ISP_CDS1_VISEL, ISP_CDS1_VISEL);
			printf("DS0 Setting========\r\n");
			printf("DS0_ON		: 0x%04X(%d)\r\n", ISP_DS0_ON, ISP_DS0_ON);
			printf("DS0_DKX 	: 0x%04X(%d)\r\n", ISP_DS0_DKX, ISP_DS0_DKX);
			printf("DS0_DKY 	: 0x%04X(%d)\r\n", ISP_DS0_DKY, ISP_DS0_DKY);
			printf("DS0_HW		: 0x%04X(%d)\r\n", ISP_DS0_HW, ISP_DS0_HW);
			printf("DS0_VW		: 0x%04X(%d)\r\n", ISP_DS0_VW, ISP_DS0_VW);
			printf("DS1 Setting========\r\n");
			printf("DS1_ON		: 0x%04X(%d)\r\n", ISP_DS1_ON, ISP_DS1_ON);
			printf("DS1_DKX 	: 0x%04X(%d)\r\n", ISP_DS1_DKX, ISP_DS1_DKX);
			printf("DS1_DKY 	: 0x%04X(%d)\r\n", ISP_DS1_DKY, ISP_DS1_DKY);
			printf("DS1_HW		: 0x%04X(%d)\r\n", ISP_DS1_HW, ISP_DS1_HW);
			printf("DS1_VW		: 0x%04X(%d)\r\n", ISP_DS1_VW, ISP_DS1_VW);
		}
#endif
		else {
			Shell_Unknown();
		}
	} else {
		Shell_Unknown();
	}

	return 0;
}


INT16S cmd_h264(INT32S argc, char *argv[])
{
	char strTitle[12] = "H.264 -CH";
	const char *strValueErr = "Value error(%d)\r\n";
	const char *strUnknown = "Unknown";
	char *strProfileMode[] = {"baseline(cavlc)", "main(cabac)"};
	char *strBRMode[] = {"OFF", "VBR", "CBR", "CVBR"};
	char *strBitRate[] = {"20", "19", "18", "17", "16", "15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1", "0.5", "0.25", "0.125"};
	UINT selCH = 0xFFFFFFFF;
	if (strcmp(argv[0], "h1") == 0) {
		selCH = e_H264_1CH;
		strTitle[6] = '1';
	} else if (strcmp(argv[0], "h2") == 0) {
		selCH = e_H264_2CH;
		strTitle[6] = '2';
	} else {
		argc == 2;
	}
	volatile uservideoinfo *uvipH264 = &gtUser.uviH264[selCH];
	UINT printCH = selCH + 1;

	if (argc == 1) {
		printf("%s Profile(Entropy) : %s(%d)\r\n", strTitle, (((UINT)uvipH264->eProfileMode)<=e_pmMainCB) ? strProfileMode[uvipH264->eProfileMode] : strUnknown, uvipH264->eProfileMode);
		printf("%s Size             : %s(%d)\r\n", strTitle, (((UINT)uvipH264->eResolution)<e_resEndorUnknown) ? listResolution[uvipH264->eResolution].strResolution : strUnknown, uvipH264->eResolution);
		printf("%s FPS              : ", strTitle);
		UINT nFps = ssfps2fps(gptMsgShare.VIDEO_FPS, uvipH264->sfFPS);
		if (nFps == 0xffffffff) {
			printf("Video Max Frame Error?(%d)\r\n", gptMsgShare.VIDEO_FPS);
		} else {
			printf("%dfps(%d)\r\n", nFps, uvipH264->sfFPS);
		}
		printf("%s IDR              : %d\r\n", strTitle, uvipH264->nIDRFrame);
		printf("%s Quality          : %d\r\n", strTitle, uvipH264->nQuality);
		printf("%s BRmode           : %s(%d)\r\n", strTitle, (((UINT)uvipH264->eBRMode)<=e_brmCVBR) ? strBRMode[uvipH264->eBRMode] : strUnknown, uvipH264->eBRMode);
		printf("%s BRate            : %sMbps(%d)\r\n", strTitle, (((UINT)uvipH264->nBitRate)<=e_hc128k) ? strBitRate[uvipH264->nBitRate] : strUnknown, uvipH264->nBitRate);
		printf("\r\n");
	} else if (argc >= 3 && (argc & 1)) { // 3개이상이고 명령이 홀수인 경우에만 ok
		UINT var = 0;
		int i, err = 0;

		if (!(selCH == e_H264_1CH || selCH == e_H264_2CH)) {
			printf("CH error(%d)\r\n", printCH);
		} else {
			for (i = 1; i < argc; i += 2) {
				err = 0;
				var = atoi(argv[i+1]);

				if (strcmp(argv[i], "fps") == 0) {
					SensorFps ssfps = fps2ssfps(gptMsgShare.VIDEO_FPS, var);
					printf("%s FPS set ", strTitle);
					if (ssfps != 0xffffffff) {
						uvipH264->sfFPS = ssfps;
						printf("%dfps\r\n", var);
						if(selCH == e_H264_1CH)			while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_FPS, uvipH264->sfFPS) == DEF_FAIL)	vTaskDelay(1);
						else if(selCH == e_H264_2CH)	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_FPS, uvipH264->sfFPS) == DEF_FAIL)	vTaskDelay(1);
					} else {
						printf(strValueErr, var);
					}
				} else if (strcmp(argv[i], "size") == 0) {
					printf("H.264 %dCH Size set ", printCH);
					if (var < ResolutionInfoLength) {
						if (listResolution[var].rIndex != var) {
							err = 1;
						}
					} else {
						err = 1;
					}

					if (err == 0) {
						printf("%s\r\n", listResolution[var].strResolution);
						if (selCH == e_H264_1CH) {
							if ((int)listResolution[var].rIndex >= IMG_H264_RES && listResolution[var].rIndex <= e_res320x240) {
								uvipH264->eResolution = listResolution[var].rIndex;
								while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_SIZE, uvipH264->eResolution) == DEF_FAIL)	vTaskDelay(1);
							} else {
								printf("Size over(var:%d)(%d~%d)\r\n", var, IMG_H264_RES, e_res320x240);
							}
						} else if (selCH == e_H264_2CH) {
							if ((int)listResolution[var].rIndex >= IMG_H264_2CH_RES && listResolution[var].rIndex <= e_res320x240) {
								uvipH264->eResolution = listResolution[var].rIndex;
								while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_SIZE, uvipH264->eResolution) == DEF_FAIL)	vTaskDelay(1);
							} else {
								printf("Size over(var:%d)(%d~%d)\r\n", var, IMG_H264_2CH_RES, e_res320x240);
							}
						}
					} else {
						printf(strValueErr, var);
					}
				} else if(strcmp(argv[i], "q") == 0) {
					printf("%s Quality set ", strTitle);
					if (var <= H264_Quantize_MAX) {
						uvipH264->nQuality = var;
						printf("%d\r\n", uvipH264->nQuality);
						if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_QUALITY, uvipH264->nQuality) == DEF_FAIL)	vTaskDelay(1);
						else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_QUALITY, uvipH264->nQuality) == DEF_FAIL)	vTaskDelay(1);
					} else {
						printf(strValueErr, var);
					}
				} else if (strcmp(argv[i], "idr") == 0) {
					printf("%s IDR set ", strTitle);
					if (var <= H264_Idrframe_MAX) {
						uvipH264->nIDRFrame = var;
						printf("%d\r\n", uvipH264->nIDRFrame);
						if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_IDRFRAME, uvipH264->nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
						else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_IDRFRAME, uvipH264->nIDRFrame) == DEF_FAIL)	vTaskDelay(1);
					} else {
						printf(strValueErr, var);
					}
				} else if (strcmp(argv[i], "pro") == 0) {
					printf("%s Profile(Entropy) set ", strTitle);
					switch (var) {
						case e_pmBaseline:
						case e_pmMainCB:
							uvipH264->eProfileMode = var;
							printf("%s\r\n", strProfileMode[uvipH264->eProfileMode]);
							if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PROFILE, uvipH264->eProfileMode) == DEF_FAIL)	vTaskDelay(1);
							else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_PROFILE, uvipH264->eProfileMode) == DEF_FAIL)	vTaskDelay(1);
							break;
						default:
							printf(strValueErr, var);
							break;
					}
				} else if (strcmp(argv[i], "dec") == 0) {
					printf("%s Main profile decrease bitrate ", strTitle);
//					if(var >= 0 && var <= 5) {
					if (var <= 5) {
						printf("%d percent down\r\n", var*6);
						if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PROFILE_DEC, var) == DEF_FAIL)	vTaskDelay(1);
						else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_PROFILE_DEC, var) == DEF_FAIL)	vTaskDelay(1);
					} else {
						printf(strValueErr, var);
					}
				} else if (strcmp(argv[i], "brmode") == 0) {
					printf("%s BitrateMode set ", strTitle);
					switch (var) {
						case e_brmQBR:
						case e_brmVBR:
						case e_brmCBR:
						case e_brmCVBR:
							uvipH264->eBRMode = var;
							printf("%s\r\n", strBRMode[uvipH264->eBRMode]);
							if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRMODE, uvipH264->eBRMode) == DEF_FAIL) vTaskDelay(1);
							else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_BRMODE, uvipH264->eBRMode) == DEF_FAIL) vTaskDelay(1);
							break;
						default:
							printf(strValueErr, var);
							break;
					}
				} else if (strcmp(argv[i], "br") == 0) {
					printf("%s Bitrate set ", strTitle);
					switch (var) {
						case 20:	uvipH264->nBitRate = e_hc20m;	break;
						case 19:	uvipH264->nBitRate = e_hc19m;	break;
						case 18:	uvipH264->nBitRate = e_hc18m;	break;
						case 17:	uvipH264->nBitRate = e_hc17m;	break;
						case 16:	uvipH264->nBitRate = e_hc16m;	break;
						case 15:	uvipH264->nBitRate = e_hc15m;	break;
						case 14:	uvipH264->nBitRate = e_hc14m;	break;
						case 13:	uvipH264->nBitRate = e_hc13m;	break;
						case 12:	uvipH264->nBitRate = e_hc12m;	break;
						case 11:	uvipH264->nBitRate = e_hc11m;	break;
						case 10:	uvipH264->nBitRate = e_hc10m;	break;
						case 9:		uvipH264->nBitRate = e_hc9m;	break;
						case 8:		uvipH264->nBitRate = e_hc8m;	break;
						case 7:		uvipH264->nBitRate = e_hc7m;	break;
						case 6:		uvipH264->nBitRate = e_hc6m;	break;
						case 5:		uvipH264->nBitRate = e_hc5m;	break;
						case 4:		uvipH264->nBitRate = e_hc4m;	break;
						case 3:		uvipH264->nBitRate = e_hc3m;	break;
						case 2:		uvipH264->nBitRate = e_hc2m;	break;
						case 1:		uvipH264->nBitRate = e_hc1m;	break;
						case 512:	uvipH264->nBitRate = e_hc512k;	break;
						case 256:	uvipH264->nBitRate = e_hc256k;	break;
						case 128:	uvipH264->nBitRate = e_hc128k;	break;
						default:	err = 1;						break;
					}

					if (err == 0) {
						printf("%sMbps\r\n", strBitRate[uvipH264->nBitRate]);
						if (selCH == e_H264_1CH)		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_BRATE, uvipH264->nBitRate) == DEF_FAIL) vTaskDelay(1);
						else if (selCH == e_H264_2CH) 	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_BRATE, uvipH264->nBitRate) == DEF_FAIL) vTaskDelay(1);
					} else {
						printf(strValueErr, var);
					}
				} else if (strcmp(argv[1], "sw") == 0) {
					if (var == DEF_OFF) {
						printf("%s channel off\r\n", strTitle);
						if (selCH == e_H264_1CH)		gptMsgShare.H264_ENC_ENABLE &= ~0x1;
						else if (selCH == e_H264_2CH)	gptMsgShare.H264_ENC_ENABLE &= ~0x2;
					} else {
						printf("%s channel on\r\n", strTitle);
						if (selCH == e_H264_1CH)		gptMsgShare.H264_ENC_ENABLE |= 0x1;
						else if (selCH == e_H264_2CH)	gptMsgShare.H264_ENC_ENABLE |= 0x2;
					}
				} else if (strcmp(argv[1], "ifs") == 0) {
					printf("iframe max ratio %d\r\n", var);
					if (selCH == e_H264_1CH) 		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_IFRAME_SIZE, var) == DEF_FAIL) vTaskDelay(1);
					else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_IFRAME_SIZE, var) == DEF_FAIL) vTaskDelay(1);
				} else if (strcmp(argv[1], "pfs") == 0) {
					printf("pframe max ratio %d\r\n", var);
					if (selCH == e_H264_1CH) 		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_PFRAME_SIZE, var) == DEF_FAIL) vTaskDelay(1);
					else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_PFRAME_SIZE, var) == DEF_FAIL) vTaskDelay(1);
				} else if (strcmp(argv[1], "ovb") == 0) {
					printf("over bit ratio %d\r\n", var);
					if (selCH == e_H264_1CH) 		while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_1CH_OVERBIT_SIZE, var) == DEF_FAIL) vTaskDelay(1);
					else if (selCH == e_H264_2CH)	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_H264_2CH_OVERBIT_SIZE, var) == DEF_FAIL) vTaskDelay(1);
				} else {
					printf("%s opt Error(%s, %d)\r\n", strTitle, argv[i], var);
				}
			}
			vTaskDelay(10);
		}
	} else if (argc == 2) {
		if (selCH == 0xFFFFFFFF) {
			printf("H.264 CH error\r\n");
		}
		printf("%s Setting shellcmd\r\n", strTitle);
		printf("%s pro [var] : %s Profile(Entropy) setting\r\n", argv[0], strTitle);
		printf("               0:baseline(cavlc) 1:main(cabac)\r\n");
		printf("%s size [var]: %s Size setting\r\n", argv[0], strTitle);
		printf("               0:1920x1080, 1:1280x720, 2:800x600, 3:704x480\r\n");
		printf("               4:704x400, 5:640x480, 6:640x360, 7:320x240\r\n");
		printf("%s fps [var] : %s FPS setting\r\n", argv[0], strTitle);
		printf("               1 ~ %d\r\n", gptMsgShare.VIDEO_FPS);
		printf("%s idr [var] : %s IDR setting\r\n", argv[0], strTitle);
		printf("               1 ~ 150\r\n");
		printf("%s q [var]   : %s Quality setting\r\n", argv[0], strTitle);
		printf("               0 ~ 51\r\n");
		printf("%s brmode [var] : %s BitrateMode setting\r\n", argv[0], strTitle);
		printf("               0:OFF, 1:VBR, 2:CBR, 3:CBVR\r\n");
		printf("%s br [var]  : %s Bitrate setting\r\n", argv[0], strTitle);
		printf("               20:20Mbps ~ 1:1Mbps, 512:512Kbps, 256:256Kbps, 128:128Kbps\r\n");
	}

	return 0;
}
#endif

#ifdef __JPEG__
INT16S cmd_jpeg_info(INT32S argc, char *argv[])
{
	const char *strValueErr = "Value error(%d)\r\n";
	const char *strUnknown = "Unknown";
	char *strBRMode[] = {"OFF", "VBR", "CBR"};
	char *strBitRate[] = {"50", "45", "40", "35", "30", "25", "20", "19", "18", "17", "16", "15", "14", "13", "12", "11", "10", "9", "8", "7", "6", "5", "4", "3", "2", "1"};
	volatile uservideoinfo *uvipJPEG = &(gtUser.uviJpeg);

	if (argc == 1) {
		printf("JPEG Size    : %s(%d)\r\n", (((UINT)uvipJPEG->eResolution)<e_resEndorUnknown) ? listResolution[uvipJPEG->eResolution].strResolution : strUnknown, uvipJPEG->eResolution);
		printf("JPEG FPS     : ");
		UINT nFps = ssfps2fps(gptMsgShare.VIDEO_FPS, uvipJPEG->sfFPS);
		if (nFps == 0xffffffff) {
			printf("Video Max Frame Error?(%d)\r\n", gptMsgShare.VIDEO_FPS);
		} else {
			printf("%dfps(%d)\r\n", nFps, uvipJPEG->sfFPS);
		}
		printf("JPEG Quality : %d\r\n", uvipJPEG->nQuality);
		printf("JPEG BRmode  : %s(%d)\r\n", (((UINT)uvipJPEG->eBRMode)<=e_brmCBR) ? strBRMode[uvipJPEG->eBRMode] : strUnknown, uvipJPEG->eBRMode);
		printf("JPEG BRate   : %sMbps(%d)\r\n", (((UINT)uvipJPEG->nBitRate)<=e_jc1m) ? strBitRate[uvipJPEG->nBitRate] : strUnknown, uvipJPEG->nBitRate);
		printf("\r\n");
	} else if (argc >= 3 && (argc & 1)) { // 3개이상이고 명령이 홀수인 경우에만 ok
		UINT var = 0;
		int i, err = 0;
		for (i = 1; argc > i; i += 2) {
			err = 0;
			var = atoi(argv[i+1]);

			if (strcmp(argv[i], "fps") == 0) {
				SensorFps ssfps = fps2ssfps(gptMsgShare.VIDEO_FPS, var);
				printf("JPEG FPS set ");
				if (ssfps != 0xffffffff) {
					gtUser.uviJpeg.sfFPS = ssfps;
					printf("%dfps\r\n", var);
					while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_FPS, gtUser.uviJpeg.sfFPS) == DEF_FAIL)	vTaskDelay(1);
				} else {
					printf(strValueErr, var);
				}
			} else if (strcmp(argv[i], "size") == 0) {
				printf("JPEG Size set ");
				if (var < ResolutionInfoLength) {
					gtUser.uviJpeg.eResolution = var;
					printf("%s\r\n", listResolution[gtUser.uviJpeg.eResolution].strResolution);
					while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_SIZE, gtUser.uviJpeg.eResolution) == DEF_FAIL) vTaskDelay(1);
				} else {
					printf(strValueErr, var);
				}
			} else if (strcmp(argv[i], "q") == 0) {
				printf("JPEG Quality set ");
				if (var <= JPEG_Quantize_MAX) {
					gtUser.uviJpeg.nQuality = var;
					printf("%d\r\n", var);
					while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_QUALITY, gtUser.uviJpeg.nQuality) == DEF_FAIL) vTaskDelay(1);
				} else {
					printf(strValueErr, var);
				}
			} else if (strcmp(argv[i], "brmode") == 0) {
				printf("JPEG BitrateMode set ");
				switch (var) {
					case e_brmQBR:
					case e_brmVBR:
					case e_brmCBR:
						gtUser.uviJpeg.eBRMode = var;
						printf("%s\r\n", strBRMode[gtUser.uviJpeg.eBRMode]);
						while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_BRMODE, gtUser.uviJpeg.eBRMode) == DEF_FAIL) vTaskDelay(1);
						break;
					default:
						printf(strValueErr, var);
						break;
				}
			} else if(strcmp(argv[i], "br") == 0) {
				printf("JPEG Bitrate set ");
				switch (var) {
					case 50:	gtUser.uviJpeg.nBitRate = e_jc50m;		printf("50Mbps\r\n");	break;
					case 49:
					case 48:
					case 47:
					case 46:
					case 45:	gtUser.uviJpeg.nBitRate = e_jc45m;		printf("45Mbps\r\n");	break;
					case 44:
					case 43:
					case 42:
					case 41:
					case 40:	gtUser.uviJpeg.nBitRate = e_jc40m;		printf("40Mbps\r\n");	break;
					case 39:
					case 38:
					case 37:
					case 36:
					case 35:	gtUser.uviJpeg.nBitRate = e_jc35m;		printf("35Mbps\r\n");	break;
					case 34:
					case 33:
					case 32:
					case 31:
					case 30:	gtUser.uviJpeg.nBitRate = e_jc30m;		printf("30Mbps\r\n");	break;
					case 29:
					case 28:
					case 27:
					case 26:
					case 25:	gtUser.uviJpeg.nBitRate = e_jc25m;		printf("25Mbps\r\n");	break;
					case 24:
					case 23:
					case 22:
					case 21:
					case 20:	gtUser.uviJpeg.nBitRate = e_jc20m;		printf("20Mbps\r\n");	break;
					case 19:	gtUser.uviJpeg.nBitRate = e_jc19m;		printf("19Mbps\r\n");	break;
					case 18:	gtUser.uviJpeg.nBitRate = e_jc18m;		printf("18Mbps\r\n");	break;
					case 17:	gtUser.uviJpeg.nBitRate = e_jc17m;		printf("17Mbps\r\n");	break;
					case 16:	gtUser.uviJpeg.nBitRate = e_jc16m;		printf("16Mbps\r\n");	break;
					case 15:	gtUser.uviJpeg.nBitRate = e_jc15m;		printf("15Mbps\r\n");	break;
					case 14:	gtUser.uviJpeg.nBitRate = e_jc14m;		printf("14Mbps\r\n");	break;
					case 13:	gtUser.uviJpeg.nBitRate = e_jc13m;		printf("13Mbps\r\n");	break;
					case 12:	gtUser.uviJpeg.nBitRate = e_jc12m;		printf("12Mbps\r\n");	break;
					case 11:	gtUser.uviJpeg.nBitRate = e_jc11m;		printf("11Mbps\r\n");	break;
					case 10:	gtUser.uviJpeg.nBitRate = e_jc10m;		printf("10Mbps\r\n");	break;
					case 9: 	gtUser.uviJpeg.nBitRate = e_jc9m;		printf("9Mbps\r\n");	break;
					case 8: 	gtUser.uviJpeg.nBitRate = e_jc8m;		printf("8Mbps\r\n");	break;
					case 7: 	gtUser.uviJpeg.nBitRate = e_jc7m;		printf("7Mbps\r\n");	break;
					case 6: 	gtUser.uviJpeg.nBitRate = e_jc6m;		printf("6Mbps\r\n");	break;
					case 5: 	gtUser.uviJpeg.nBitRate = e_jc5m;		printf("5Mbps\r\n");	break;
					case 4: 	gtUser.uviJpeg.nBitRate = e_jc4m;		printf("4Mbps\r\n");	break;
					case 3: 	gtUser.uviJpeg.nBitRate = e_jc3m;		printf("3Mbps\r\n");	break;
					case 2: 	gtUser.uviJpeg.nBitRate = e_jc2m;		printf("2Mbps\r\n");	break;
					case 1: 	gtUser.uviJpeg.nBitRate = e_jc1m;		printf("1Mbps\r\n");	break;
					default:	err = 1;	printf(strValueErr, var);							break;
				}
				if (err == 0) {
					while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SET_JPEG_BRATE, gtUser.uviJpeg.nBitRate) == DEF_FAIL) vTaskDelay(1);
				}
			} else {
				printf("JPEG opt Error(%s, %d)\r\n", argv[i], var);
			}
		}
	} else if (argc == 2) {
		if (strcmp(argv[1], "info") == 0) {
			while (MsgPut(&gptMsgCpu1to0, MSG_CMD_VIEW_JPEG_INFO, 0) == DEF_FAIL)	vTaskDelay(1);
			vTaskDelay(50);
		} else {
			printf("JPEG Setting shellcmd\r\n");
			printf("%s size [var]: JPEG Size setting\r\n", argv[0]);
			printf("                 0:1920x1080, 1:1280x720, 2:800x600, 3:704x480\r\n");
			printf("                 4:704x400, 5:640x480, 6:640x360, 7:320x240\r\n");
			printf("%s fps [var] : JPEG FPS setting\r\n", argv[0]);
			printf("                 1 ~ %d\r\n", gptMsgShare.VIDEO_FPS);
			printf("%s q [var]	 : JPEG Quality setting\r\n", argv[0]);
			printf("                 0 ~ 51\r\n");
			printf("%s brmode [var] : JPEG BitrateMode setting\r\n", argv[0]);
			printf("                 0:OFF, 1:VBR, 2:CBR\r\n");
			printf("%s br [var]  : JPEG Bitrate setting\r\n", argv[0]);
			printf("                 50:50Mbps ~ 25:25Mbps(5step) 20:20Mbps ~ 1:1Mbps(1step)\r\n");
		}
	} else {
		Shell_Unknown();
	}
	return 0;
}
#endif

INT16S cmd_envvar(INT32S argc, char *argv[])
{
	if (argc == 2) {
		char *out = getenv(argv[1]);
		if(out == NULL) {
			printf("GET [%s] [NULL]\r\n", argv[1]);
		} else {
			printf("GET [%s] [%s]\r\n", argv[1], out);
		}
	} else if (argc == 3) {
		setenv(argv[1], argv[2], 1);
		printf("SET [%s] [%s]\r\n", argv[1], argv[2]);
	} else {
		Shell_Unknown();
	}

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_userarea(INT32S argc, char *argv[])
{
#define SFLSUSERAREA_INFO(a, b, c) printf("%s %s %s\r\n", a, b, c)

	int err_flag = 0;
	const char *strUserInfo = "User Info";
	const char *strCommand[4] = {"Save", "Load", "Init", "Erase"};
	const char *strState[3] = {"Start", "OK", "Fail"};

	if (argc >= 3) {
		if (strcmp(argv[1], "zero") == 0) {
			SFLSUSERAREA_INFO(strUserInfo, strCommand[3], strState[0]);
			sfls_fixed_area index = atoi(argv[2]);
			switch (index) {
				case sfls_SYSTEM:
				case sfls_USER:
#if defined(__ISP__)
				case sfls_ISP_SHD:
				case sfls_ISP_MENU:
				case sfls_ISP_MENU_BAK:
				case sfls_ISP_USER:
				case sfls_ISP_DATA:
				case sfls_ISP_BLK0:
				case sfls_ISP_BLK1:
				case sfls_ISP_BLK2:
#endif
#if defined(__ONVIF__)
				case sfls_ONVIF:
#endif
#if defined(__WIFI__)
				case sfls_WIFI:
#endif
					UserErase(index);
					printf("%s %s(%d) %s\r\n", strUserInfo, strCommand[3], index, strState[1]);
					break;
				default:
					printf("%s %s(%d) %s\r\n", strUserInfo, strCommand[3], index, strState[2]);
					err_flag = 1;
					break;
			}
		} else {
			err_flag = 1;
		}
	} else if (argc >= 2) {
		if (strcmp(argv[1], "save") == 0) {
			SFLSUSERAREA_INFO(strUserInfo, strCommand[0], strState[0]);
			UserSave(sfls_USER);
			SFLSUSERAREA_INFO(strUserInfo, strCommand[0], strState[1]);
		} else if (strcmp(argv[1], "load") == 0) {
			SFLSUSERAREA_INFO(strUserInfo, strCommand[1], strState[0]);
			UserLoad(sfls_USER);
			SFLSUSERAREA_INFO(strUserInfo, strCommand[1], strState[1]);
		} else if (strcmp(argv[1], "init") == 0) {
			SFLSUSERAREA_INFO(strUserInfo, strCommand[2], strState[0]);
			LoadDefUserInfo();
			SFLSUSERAREA_INFO(strUserInfo, strCommand[2], strState[1]);
		} else {

		}
	} else {
		err_flag = 1;
	}

	if (err_flag == 1) {
#define strZero	"           %2d  : sfls_"
		printf("%s save : %s %s\r\n", argv[0], strUserInfo, strCommand[0]);
		printf("%s load : %s %s\r\n", argv[0], strUserInfo, strCommand[1]);
		printf("%s init : %s %s\r\n", argv[0], strUserInfo, strCommand[2]);
		printf("%s zero [num] : %s %s\r\n", argv[0], strUserInfo, strCommand[3]);
		printf(strZero "SYSTEM\r\n", sfls_SYSTEM);
		printf(strZero "USER\r\n", sfls_USER);
#if defined(__ISP__)
		printf(strZero "ISP_SHD\r\n", sfls_ISP_SHD);
		printf(strZero "ISP_MENU\r\n", sfls_ISP_MENU);
		printf(strZero "ISP_MENU_BAK\r\n", sfls_ISP_MENU_BAK);
		printf(strZero "ISP_USER\r\n", sfls_ISP_USER);
		printf(strZero "ISP_DATA\r\n", sfls_ISP_DATA);
		printf(strZero "ISP_BLK0\r\n", sfls_ISP_BLK0);
		printf(strZero "ISP_BLK1\r\n", sfls_ISP_BLK1);
		printf(strZero "ISP_BLK2\r\n", sfls_ISP_BLK2);
#endif
#if defined(__ONVIF__)
		printf(strZero "ONVIF\r\n", sfls_ONVIF);
#endif
#if defined(__WIFI__)
		printf(strZero "WIFI\r\n", sfls_WIFI);
#endif
	}

/*
typedef enum {
	sfls_SYSTEM = 1,		//  4KB(- 4KB)
	sfls_USER,				//  4KB(- 8KB)
#if defined(__ISP__)
	sfls_ISP_SHD,			//  4KB(-40KB)
	sfls_ISP_MENU,			//  4KB(-36KB)
	sfls_ISP_MENU_BAK,		//  4KB(-32KB)
	sfls_ISP_USER,			//  4KB(-28KB)
	sfls_ISP_DATA,			//  4KB(-24KB)
	sfls_ISP_BLK0,			//  4KB(-20KB)
	sfls_ISP_BLK1,			//  4KB(-16KB)
	sfls_ISP_BLK2,			//  4KB(-12KB)
#endif
#if defined(__ONVIF__)
	sfls_ONVIF,				// 12KB(-52KB)
#endif
#if defined(__WIFI__)
	sfls_WIFI,				//  4KB(-56KB)
#endif
}sfls_fixed_area;
*/
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

INT16S cmd_userinfo(INT32S argc, char *argv[])
{
#if 0
	printf("_pBootAddr 0x%x\r\n",gtUser._pBootAddr);
	printf("nMagicNum 0x%x\r\n",gtUser.nMagicNum);
	printf("nFirstBoot %d\r\n",gtUser.nFirstBoot);
	printf("nSYS_DQS_DLY %d\r\n",gtUser.nSYS_DQS_DLY);
	printf("nSYS_HDSEL %d\r\n",gtUser.nSYS_HDSEL);
	printf("nVersion 0x%x\r\n",gtUser.nVersion);

	printf("strUserId %s\r\n",gtUser.strUserId);
	printf("strUserPw %s\r\n",gtUser.strUserPw);

	printf("nSystemTime %d\r\n",gtUser.nSystemTime);
	printf("bUseSntp %d\r\n",gtUser.bUseSntp);
	printf("strSntpSvr %s\r\n",gtUser.strSntpSvr);
	printf("nSntpPeriodic %d\r\n",gtUser.nSntpPeriodic);
	printf("nSntpRetrySec %d\r\n",gtUser.nSntpRetrySec);
	printf("nSntpRetryMaxcnt %d\r\n",gtUser.nSntpRetryMaxcnt);
	printf("nTimeZone %d\r\n",gtUser.nTimeZone);

	ip_addr_t ip_addr;
	ip_addr.addr = gtUser.nIpAddr;
	printf("nIpAddr %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nNetMask;
	printf("nNetMask %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nGateway;
	printf("nGateway %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDnsSvr0;
	printf("nDnsSvr0 %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDnsSvr1;
	printf("nDnsSvr1 %s\r\n",ipaddr_ntoa(&ip_addr));

	printf("bUseDhcp %d\r\n",gtUser.bUseDhcp);

	printf("strDeviceId %s\r\n",gtUser.strDeviceId);
	printf("strDeviceName %s\r\n",gtUser.strDeviceName);

	printf("nRtspPort %d\r\n",gtUser.nRtspPort);

	printf("nMaxFPS %d\r\n",gtUser.nMaxFPS);

	printf("uviJpeg eResolution %d\r\n",gtUser.uviJpeg.eResolution);
	printf("uviJpeg nQuality %d\r\n",gtUser.uviJpeg.nQuality);
	printf("uviJpeg sfFPS %d\r\n",gtUser.uviJpeg.sfFPS);
	printf("uviJpeg eBRMode %d\r\n",gtUser.uviJpeg.eBRMode);
	printf("uviJpeg nUseCBR %d\r\n",gtUser.uviJpeg.nUseCBR);
	printf("uviJpeg nBitRate %d\r\n",gtUser.uviJpeg.nBitRate);
	printf("uviJpeg nIDRFrame %d\r\n",gtUser.uviJpeg.nIDRFrame);
	printf("uviJpeg nProfileMode %d\r\n",gtUser.uviJpeg.nProfileMode);

	printf("uviH264 nQuality %d\r\n",gtUser.uviH264.nQuality);
	printf("uviH264 sfFPS %d\r\n",gtUser.uviH264.sfFPS);
	printf("uviH264 eBRMode %d\r\n",gtUser.uviH264.eBRMode);
	printf("uviH264 nUseCBR %d\r\n",gtUser.uviH264.nUseCBR);
	printf("uviH264 nBitRate %d\r\n",gtUser.uviH264.nBitRate);
	printf("uviH264 nIDRFrame %d\r\n",gtUser.uviH264.nIDRFrame);
	printf("uviH264 nProfileMode %d\r\n",gtUser.uviH264.nProfileMode);

	printf("uviH264_2 nQuality %d\r\n",gtUser.uviH264_2.nQuality);
	printf("uviH264_2 sfFPS %d\r\n",gtUser.uviH264_2.sfFPS);
	printf("uviH264_2 eBRMode %d\r\n",gtUser.uviH264_2.eBRMode);
	printf("uviH264_2 nUseCBR %d\r\n",gtUser.uviH264_2.nUseCBR);
	printf("uviH264_2 nBitRate %d\r\n",gtUser.uviH264_2.nBitRate);
	printf("uviH264_2 nIDRFrame %d\r\n",gtUser.uviH264_2.nIDRFrame);
	printf("uviH264_2 nProfileMode %d\r\n",gtUser.uviH264_2.nProfileMode);

	printf("bFtpServer %d\r\n",gtUser.bFtpServer);
	printf("bSdAviSave %d\r\n",gtUser.bSdAviSave);
	printf("bOSDPrint %d\r\n",gtUser.bOSDPrint);
	printf("bAudio %d\r\n",gtUser.bAudio);
	printf("bUseDdns %d\r\n",gtUser.bUseDdns);
	printf("bUseSmtp %d\r\n",gtUser.bUseSmtp);

	printf("smtpData strSmtpSvr %s\r\n",gtUser.smtpData.strSmtpSvr);
	printf("smtpData nSmtpPort %d\r\n",gtUser.smtpData.nSmtpPort);
	printf("smtpData strBoardMailAddr %s\r\n",gtUser.smtpData.strBoardMailAddr);
	printf("smtpData strBoardMailPw %s\r\n",gtUser.smtpData.strBoardMailPw);
	printf("smtpData strUserMailAddr %s\r\n",gtUser.smtpData.strUserMailAddr);

	printf("ddnsData strDdnsHostName %s\r\n",gtUser.ddnsData.strDdnsHostName);	// Network - DDNS - DDNS Host Name
	printf("ddnsData strDdnsAcId %s\r\n",gtUser.ddnsData.strDdnsAcId);		// Network - DDNS - DDNS Account ID
	printf("ddnsData strDdnsPw %s\r\n",gtUser.ddnsData.strDdnsPw);			// Network - DDNS - DDNS Password
	printf("ddnsData strDdnsDetectedAddr %s\r\n",gtUser.ddnsData.strDdnsDetectedAddr);// Network - DDNS - DDNS Detected Address Ask to http://checkip.dyndns.com

	printf("nHttpPort %d\r\n",gtUser.nHttpPort);

	ip_addr.addr = gtUser.nDhcpIpAddr;
	printf("nDhcpIpAddr %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDhcpNetMask;
	printf("nDhcpNetMask %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDhcpGateway;
	printf("nDhcpGateway %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDhcpDnsSvr0;
	printf("nDhcpDnsSvr0 %s\r\n",ipaddr_ntoa(&ip_addr));
	ip_addr.addr = gtUser.nDhcpDnsSvr1;
	printf("nDhcpDnsSvr1 %s\r\n",ipaddr_ntoa(&ip_addr));

	printf("autoipoffon %d\r\n",gtUser.autoipoffon);
	printf("runningTime %d\r\n",gtUser.runningTime);

	//printf("tTime %x",gtUser.tTime);
	printf("tTime : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", gtUser.tTime.tm_year+1900, gtUser.tTime.tm_mon+1, gtUser.tTime.tm_mday, strWeek[gtUser.tTime.tm_wday], gtUser.tTime.tm_hour, gtUser.tTime.tm_min, gtUser.tTime.tm_sec);
#endif
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}
