/**
 *Copyright 2013 eyenix CO., LTD.
 *ALL RIGHTS RESERVED.
 *This software is provided under license and contains proprietary
 *and confidential material which is the property of Eyenix.
 *
 *FileName     : init.c
 *Description  :
 *Reversion    :
 *Comment      : first implementation
*/

#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "exr1k.h"
#include "exr1k_sprs.h"

#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_regs_isp_macro.h" // 180323 KDH change from isp_dev_reg_isp.h

#include "msg.h"
#include "user.h"
#include "uxsr.h"

#include "main.h"
#include "enx_os.h"
#include "msgqpass.h"

#include "lwip/ip.h"
#include "lwip/dns.h"
#include "lwip/udp.h"
#include "lwip/api.h"
#include "enx_lwip_helper.h"

#include "h264.h"
#include "jpeg.h"

#include "rtp_over_rtsp.h"
#include "enx_video.h"
#include "enx_audio.h"

#ifdef __AUDIO__
#include "rtp_audio.h"
#endif

#ifdef __WIFI__
#include "inter_cpu.h"
#endif

#ifdef RTMP_SUPPORT
#include "rtmp_session.h"
#include "ngx_rtmp_stream.h"
#endif

#if (RTSPoverHTTP==1)
#include "http_tunnel.h"
#include "http_front.h"
#endif

#ifdef __ISP__
extern void isp_Init(void);
extern volatile BYTE gbVdoIrq;
extern BYTE gbIsp_Init_Flag;
#endif

#ifdef __H264__
#ifdef __H264_MCH__
#include "h264_mch.h"
#endif
#endif

//*************************************************************************************************
// Device Init Functions
//-------------------------------------------------------------------------------------------------
void InitPll1(void)
{
#if model_Sens == SENS_OV2718
	#if ISP_CLK_MODE == 1
		const UINT nPLL1_N = 60;
	#else
		const UINT nPLL1_N = 55;
	#endif
#else
	#if ISP_CLK_MODE == 1
		const UINT nPLL1_N = 120;	// 324/2=162
	#else
		const UINT nPLL1_N = 110;	// 297/2=148.5
	#endif
#endif
	SYS_PLL1_PD = 1;
	WaitXus(1000);
	SYS_PLL1_M = 1;
	SYS_PLL1_N = nPLL1_N; // 120 => 324/2 => 162MHz
	SYS_PLL1_K = 10;
	SYS_PLL1_AUTO_RS = 1;
	SYS_PLL1_ACTL_EN = 1;
	SYS_PLL0_M = 1;
	SYS_PLL1_CKEN = 1;
	SYS_PLL1_ACTL_EN = 0;
	SYS_PLL1_AUTO_RS = 0;
	printf("ISP_CLK %uKHz\r\n", (27 * 1000 * 1000 * SYS_PLL1_N / SYS_PLL1_K / SYS_PLL1_M / 1000) >> 1);
}

void InitUart(void)
{
//	UART0_CLKDIV = UB_INIT;

	UART0_STOP_BIT = 0;
	UART0_PARITY_EN = 0;

	UartRstQue();

	UART0_TX_IRQ_EN = 0;
	UART0_RX_IRQ_EN = 1;

#if (_DEBUGCODE==3)
	Uart1_Init(eUARTspeed115200);
#endif
}

void InitSpi(void)
{
//		TxStrNoIRQ(" SPI Init...", 0, 0);

#ifdef __ISP__
	// SPI 0ch init for Sensor
	SPI0_EN      = 1;
	SPI0_WS		 = 0;
	SPI0_CS_OEN  = 0;
	SPI0_BITMODE = 1; // 0 : MSB First, 1 : LSB First
//		SPI0_CLKDIV  = 0x0a;//3;
	SPI_CLK_SET(0,1000000); // CH0, 1MHz
	SPI0_IRQ_EN  = 0;
	SPI0_CLKMODE = 2;
	SPI0_CS = 1;
	SPI0_ONEBIT = 0;

#endif	// end of __ISP__
}

void InitI2c(void)
{
//		TxStrNoIRQ(" I2C Init...", 0, 0);

	// I2C Setting
	I2c2_Init(100000);

#ifdef __ISP__

	#if model_I2cSlave == 0 || model_I2cSlave == 2

		I2C_CLK_SET(0,400000); // CH0, 400KHz
	//	I2C_CLK_SET(0,100000); // CH0, 100KHz

		I2C0_MODE = 0;

	#endif

	#if model_I2cSlave == 1 || model_I2cSlave == 2

		I2C0_CLK_DIV = I2C_CLKDIV_SLAVE;

		I2cRstQue();

		I2C0_MODE = 1;
		I2C0_SLV_ADR = (SLAVE_DEVICE>>1);

		I2C0_IRQ_EN = 1;

	#endif
#endif	// end of __ISP__
}

void InitGpio(void)
{
//		TxStrNoIRQ(" GPIO Init...", 0, 0);
	FORCE_ABT_SOFFw(1);	// GPIO 20,21,22,23 강제할당

	// GPIO Setting
#ifdef GPIO_FOCUS_LED
	GpioOutDir(GPIO_FOCUS_LED);
#endif
#ifdef GPIO_ISP_VI_LED
	GpioOutDir(GPIO_ISP_VI_LED);
#endif
#ifdef GPIO_H264_ENC_LED
	GpioOutDir(GPIO_H264_ENC_LED);
#endif
#ifdef GPIO_RECORDING_LED
	GpioOutDir(GPIO_RECORDING_LED);
#endif
#ifdef GPIO_DISKIO_LED
	GpioOutDir(GPIO_DISKIO_LED);
#endif
#ifdef GPIO_LED3
	GpioOutDir(GPIO_LED3);
#endif
#ifdef GPIO_LED4
	GpioOutDir(GPIO_LED4);
#endif
#ifdef GPIO_ETH_IRQ
	GpioInDir(GPIO_ETH_IRQ);
#endif
#ifdef GPIO_JPEG_STATE
	GpioOutDir(GPIO_JPEG_STATE);
#endif
#ifdef GPIO_ETC_STATE
	GpioOutDir(GPIO_ETC_STATE);
#endif
#ifdef GPIO_SDIO1_RST
	GpioOutDir(GPIO_SDIO1_RST);
#endif
#ifdef GPIO_ETH_RST
	GpioOutDir(GPIO_ETH_RST);
#endif
#ifdef GPIO_AUD_RST
	GpioOutDir(GPIO_AUD_RST);
#endif

#if 1
	PAD_SDIO0_DET_PUEN = 0;	// 0:on 1:off / sd card detector(pin A12)
	PAD_SDIO0_CLK_PU = 1;	// 0:on 1:off / sd card clock pull-up(pin B7)
	PAD_SDIO0_CMD_PU = 1;	// 0:on 1:off / sd card command pull-up(pin C7)
	PAD_SDIO0_DAT_PU = 1;	// 0:on 1:off / sd card data pull-up(pin A7,C6,B6,A6)
	PAD_SDIO1_DET_PUEN = 1;	// 0:on 1:off / wifi detector / not used(pin A8)
	PAD_SDIO1_CLK_PU = 1;	// 0:on 1:off / wifi clock pull-up(pin B5)
	PAD_SDIO1_CMD_PU = 1;	// 0:on 1:off / wifi command pull-up(pin C5)
	PAD_SDIO1_DAT_PU = 1;	// 0:on 1:off / wifi data pull-up(pin A5,C4,B4,A4)
#endif


#ifdef __ISP__

	// Fixed GPIO Assign (Don't Change to another Port)
	GpioOutDir(GPIO_SENS_RSTN);		// Main Sensor Reset
	GpioSetHi(GPIO_SENS_RSTN);		// Sensor Reset High

	#if (model_Lens==1)	// 2 Motor focus
		GpioOutDir(GPIO_MTR_FCS_AP	);
		GpioOutDir(GPIO_MTR_FCS_AM	);
		GpioOutDir(GPIO_MTR_FCS_BP	);
		GpioOutDir(GPIO_MTR_FCS_BM	);

		GpioOutDir(GPIO_MTR_ZOOM_AP	);
		GpioOutDir(GPIO_MTR_ZOOM_AM	);
		GpioOutDir(GPIO_MTR_ZOOM_BP	);
		GpioOutDir(GPIO_MTR_ZOOM_BM	);
	#endif

	GpioOutDir(GPIO_TDN_1);
	#if (model_Tdn==2)
		GpioOutDir(GPIO_TDN_2);
	#endif

	#if (model_Led==1)
		GpioOutDir(GPIO_LED_CTL);
	#endif

	GpioFuncPin(GPIO_MPWM);

	#if (model_MotionOut==1)
		GpioOutDir(GPIO_MOTION);
	#endif

	#if (DOOR_BELL_CAMERA==1)
		GpioSetLo(GPIO_MAIN_PW_HLD);
		GpioSetLo(GPIO_MAIN_PW_CLS);
		GpioSetHi(GPIO_SENS_PEN);

		GpioOutDir(GPIO_MAIN_PW_HLD);
		GpioOutDir(GPIO_MAIN_PW_CLS);
		GpioOutDir(GPIO_SENS_PEN);

		GpioInDir(GPIO_DOOR_BELL);
		GpioInDir(GPIO_PIR_AWAKE);
	#endif

#endif	// end of __ISP__
}

void InitTimer(void)
{
//	TxStrNoIRQ(" Timer Init...", 0, 0);

#ifdef __ISP__

#if (model_Lens==1 || IMAGE_INPUT_MODE == 3)
	TIMER_REG_SET(1);	// CH1 (for 2Motor)
#endif

	// for IRIS
	TIM_CH5_DIV = 1;					// (MCLK/2) / (TIM_CH0_DIV+1) = counter/sec (초당 counter 증가 속도)
	TIM_CH5_CKEN = 1;
	PwmSetPrd(0x400);
	PwmSetDty(0/*0x400 - 1*/);
	PwmEna;
	TIM_CH5_PWM_EN = 1;
	TIM_CH5_IRQ_EN = 0;
#endif	// end of __ISP__
}

void HWInit(void)
{
	InitUart();

	InitSpi();

	InitI2c();

	InitGpio();

	InitTimer();

	AdcInit();
}

#if 0
#define timer_GPIO_LED 1
void timer_LED(void *ctx)
{
// timer_sched_timeout_cb(1000, TIMEOUT_PERIODIC, timer_LED, NULL);

	GpioOutDir(timer_GPIO_LED);
	if(GpioGetPin(timer_GPIO_LED))	GpioSetLo(timer_GPIO_LED);
	else							GpioSetHi(timer_GPIO_LED);
}
#endif

void timer_CPU1_watchdog(void *ctx)
{
	gptMsgShare.CPU1_WDT_COUNT++;	// 1s에 10
	UNUSED(ctx);
}

#ifdef __ISP__
void timer_isp_init(void *ctx)
{
	if(gbVdoIrq == 1)
	{
		gbIsp_Init_Flag = 0; // Init Complete

		gbVdoIrq = 0;
	}
	else
		timer_sched_timeout_cb(10, TIMEOUT_ONESHOT, timer_isp_init, NULL);
	UNUSED(ctx);
}

#ifdef __JPEG__
void enx_jpeg_on(void)
{
	enx_jpeg_init(SENSOR_SIZE_W, SENSOR_SIZE_H);

	enx_jpeg_set_maxframe(IMG_MAX_FPS);
	enx_jpeg_set_ts(90000/IMG_MAX_FPS);

	enx_jpeg_set_quantize(IMG_JPEG_QULITY); 			// param.qp
	enx_jpeg_set_frame_rate_sf(IMG_JPEG_FPS);			// param.period
	enx_jpeg_set_frame_size_rs(IMG_JPEG_RES);			// param.width, param.height
	enx_jpeg_set_bit_rate_mode(IMG_BR_J);				// brmode.mode
	enx_jpeg_set_bit_rate(IMG_BR_JR);					// brmode.bitrate

	enx_jpeg_start();
}
#endif
#endif

#ifdef __AUDIO__
void enx_g711_on(void)
{
	IntAdd(IRQ_AUD, (void*)Aud_Isr, 0);
	IrqEnable(IRQ_AUD);
#if 0
#ifdef __DEVICE_SD__
	enx_g711_set_sdsave(DEF_ON);
#else
	enx_g711_set_sdsave(DEF_OFF);
#endif

	enx_g711_start();
#endif
#if (AUDIO_RX)
	enx_rx_g711_init();
#endif
}
#endif

void init(void)
{
	mtspr(EXR1K_HTBCR, EXR1K_HTBCR_TE);

	InitPll1();

	extern void SensorIn(const BYTE abOn);
	SensorIn(0);	// Sensor 설정이 완료되기 전에 잘못된 영상이 입력되는 경우 WDR또는 DNR 메모리의 오동작 방지

/////////////////////////////////////////////////////////////////////////////////////
	InitShareSram();	// Share sram clear

	// Default time set
	gptMsgShare.TIME = DEFAULT_TIME_SET;		// Watch counter(++1 @ 1 sec)
	gptMsgShare.TIMEZONE = nTimeZoneSec[DEF_TIMEZONE][2];
/////////////////////////////////////////////////////////////////////////////////////

	//--------------------------------------------------------------------------
	// Setup Debugger: DO NOT MOVE because the STB starts @ main
#ifdef 	_DEBUG
//	set_hwp(EXR1K_SPRGID_DWB, 7, EXR1K_WCR_CT_ST_DAT, EXR1K_WCR_CC_NEQ, 0x04, EXR1K_WMR_CW_AND_PCHAIN);

	//enable_jtag_ctrl_exception();
	enable_hwp(EXR1K_SPRGID_IWB);
	enable_hwp(EXR1K_SPRGID_DWB);
	enable_hwdt(EXR1K_SPRGID_IWB, 10000);
	enable_hwdt(EXR1K_SPRGID_DWB, 10000);
	print_hwp_all_info(EXR1K_SPRGID_IWB);
	print_hwp_all_info(EXR1K_SPRGID_DWB);

	enable_htb();
	enable_stb();
#else
	mtspr(EXR1K_HTBCR, EXR1K_HTBCR_TE);
#endif//_DEBUG

	set_fp_round_mode(EXR1K_FPCSR_RM_RN);	// float32 사용시 사용
	IntInit();

#ifdef WDT_CPU0
	WdtCpuSetTime(WDT_DEFAULT_TIME);	// CPU0 WDT enable
#endif

	HWInit();

#ifdef __ISP__
	static BYTE bInit_Flag = 1;
	if(bInit_Flag)
	{
		bInit_Flag = 0;
		isp_Init();
	}
#else
	add_xsr(XSR_INT, XsrInt);
#endif

#if (IMAGE_INPUT_MODE==2)
	IntAdd(IRQ_CPU, (void*)IsrSys0, 0);
	IrqEnable(IRQ_CPU);
	CPU_IRQ0_EN = 1;	// cpu0->cpu1
#endif

#ifdef ECM_REMOTE
	IntAdd(IRQ_CPU, (void*)IsrSys0, 0);
	IrqEnable(IRQ_CPU);
	CPU_IRQ0_EN = 1;	// cpu0->cpu1
#endif

#ifdef __ISP__
#ifdef __H264__
//////////////////////////////////////////////////////
//H.264 INIT//////////////////////////////////////////
//////////////////////////////////////////////////////
	enx_h264_on();
#endif

	WDR_ADRw((ISP_BUF_BASE >> 4) << 1);
#endif

	timer_init(NULL);
	timer_tick_init(BUS_CLK, TIME_TICK); // 100HZ TIME TICK

#ifdef WDT_SYS
	timer_sched_timeout_cb(100, TIMEOUT_PERIODIC, timer_CPU1_watchdog, NULL);
#endif

	mtspr(EXR1K_SR, mfspr(EXR1K_SR)|EXR1K_SR_IEE); // Global interrupt enable
//	printf("CPU0: SR = 0x%X\r\n", mfspr(EXR1K_SR));

	srand(0);

//////////////////////////////////////////////////////
//WIFI SHELL INIT/////////////////////////////////////
//////////////////////////////////////////////////////
#ifdef __WIFI__
    inter_cpu_init();
#endif

//////////////////////////////////////////////////////
//LWIP INIT///////////////////////////////////////////
//////////////////////////////////////////////////////
	enx_lwip_init();
	enx_netif_init(NULL);

#ifdef __ISP__
//////////////////////////////////////////////////////
//ISP INIT FLAG CHECK/////////////////////////////////
//////////////////////////////////////////////////////
	timer_sched_timeout_cb(10, TIMEOUT_ONESHOT, timer_isp_init, NULL);

#ifdef __JPEG__
//////////////////////////////////////////////////////
//JPEG INIT///////////////////////////////////////////
//////////////////////////////////////////////////////
	enx_jpeg_on();
#endif

//////////////////////////////////////////////////////
//RTSP INIT///////////////////////////////////////////
//////////////////////////////////////////////////////
	rtsp_init();

#ifdef RTMP_SUPPORT
//////////////////////////////////////////////////////
//RTMP INIT///////////////////////////////////////////
//////////////////////////////////////////////////////
	rtmp_init(RTMP_DEF_PORT);
#endif

#if (RTSPoverHTTP==1)
	tunnel_port_init();
#endif

#ifdef __H264__
//	enx_h264_info_print();
	enx_h264_image_capture_ready();
#endif
#endif

#ifdef __AUDIO__
//////////////////////////////////////////////////////
//AUDIO INIT//////////////////////////////////////////
//////////////////////////////////////////////////////
	enx_g711_on();
#endif

#if (NET_LWIPERF==1)
extern void lwiperf_init(void);
	lwiperf_init();
#endif

	printf("CPU0: Ready\r\n");

	gptMsgShare.AUD_TX_DONE = 1;
#ifdef __H264_2CH__
	gptMsgShare.H264_ENC_ENABLE = 0x3;
#else
	gptMsgShare.H264_ENC_ENABLE = 0x1;
#endif
	gptMsgShare.VIDEO_OSD = OSD_TIME;// | OSD_H264_1; // | OSD_FRAME | OSD_H264_2 | OSD_JPEG;
	gptMsgShare.REC_ON = 0;

#if (model_IspRegCheck==1)
	extern BYTE gbOnCnt;
	extern BYTE gbOffCnt;
	extern BYTE gbCntStop;
	gbCntStop = 1;
	extern void TxStrDecNoIRQ(const char*, UINT, UINT);
	TxStrDecNoIRQ("1->0 CNT:", gbOffCnt, 4);
	TxStrDecNoIRQ("0->1 CNT:", gbOnCnt, 4);
#endif
}

/* EOF */

