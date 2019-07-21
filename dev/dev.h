//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		dev.h
//	Description:	EN673 device function definition, especially for ISP not for IPs
//	Author:			EN673 team
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151026		ygkim		separated "dev_types.h" and "dev_regs.h"
//								to share the type and register information with IP SW and
//								not to include the following functions in IP SW
//------------------------------------------------------------------------------
#ifndef _DEV_H_
#define _DEV_H_

#define _DEV_VA_LIST

#include "board.h"			// board and memory size, base address definition

#include "dev_types.h"		// to define types
#include "dev_regs.h"		// to define device registers
#include "dev_regs_isp.h"	// to define isp registers

#include "exr1k.h"
#include "exr1k_sprs.h"
#ifdef _STB_ENABLE
#include "exr1k_stb.h"
#endif
#include "exr1k_float.h"

#include "lib.h"			// funtions may conflict with gcc or 3rd party libraries
#include "develop.h"

#define	_DEV_TEST

/*
 INDEX	:
		 1. TYPE DEF
		 2. Memory Size & Base Address Definition
		 3. IRQ Number Def
		 4. SYSTEM
		 5. UTIL
		 6. ADC
		 7. CPU
		 8. DDR
		 9. DMA
		10. GPIO
		11. ISR
		12. SERIAL FLASH
		13. TIMER
		14. ETHERNET
		15. H.264
		16. SDIO
		17. I2C
		18. AUD
		19. SPI
		20. UART
		21. UTIL
		22. WDT
		23. DEVICE TEST
		24. ISP
		25. Global common
		26. RTOS
*/

//******************************************************************************
// 1. TYPE DEF
//------------------------------------------------------------------------------
#define ISPM_DEV	__attribute__((section(".ispmtext")))	// DEV only
#define ISPD_DEV	__attribute__((section(".ispmdata")))	//	"

//#define ISPM		__attribute__((section(".ispmtext")))
//#define ISPD		__attribute__((section(".ispmdata")))

#define ISRT		__attribute__((section(".ispmtext")))
#define ISRD		__attribute__((section(".ispmdata")))

#define SH0SRD		__attribute__((section(".sh0srdata")))
#define SH1SRD		__attribute__((section(".sh1srdata")))

#define NOP()		asm volatile("l.nop")

//------------------------------------------------------------------------------
// Physical Drive Number
typedef enum
{
	FLS,
	MMC,
	MEM,
}DriveNum;

//------------------------------------------------------------------------------
// Misc define
#ifndef __BOOLEAN__
#define __BOOLEAN__
typedef unsigned char           BOOLEAN;    //  "
#endif

#ifndef	NULL
	#define	NULL				0
#endif

#define  DEF_DISABLED			0
#define  DEF_ENABLED			1

#define  DEF_FALSE				0
#define  DEF_TRUE				1

#define  DEF_NO					0
#define  DEF_YES				1

#define  DEF_OFF				0
#define  DEF_ON					1

#define  DEF_CLR				0
#define  DEF_SET				1

#define  DEF_ACTIVE				0
#define  DEF_INACTIVE			1

#define  DEF_FAIL				0
#define  DEF_OK					1

#define SDIO_FAIL				1
#define SDIO_OK					0

//------------------------------------------------------------------------------
// Octet defines
#define  DEF_OCTET_NBR_BITS		8
#define  DEF_OCTET_MASK			0xff

#define  DEF_NIBBLE_NBR_BITS	4
#define  DEF_NIBBLE_MASK		0x0f

#if (model_SetIsp == 1)
	extern void SetIsp(UINT anAddr, UINT anData);
#else
	#define	SetIsp(anAddr, anData)	(_wr32(ISP_REG_BASE+((UINT)(anAddr)<<2), (UINT)(anData)))	// Write to bridge(Isp,Aud)
#endif

#define	GetIsp(anAddr)			(_rd32(ISP_REG_BASE+((UINT)(anAddr)<<2))                )	// Read from bridge(Isp,Aud)

#define numberof(Arr)			(sizeof(Arr)/sizeof(Arr[0]))


#define	DI		CRITICAL_BEGIN
#define	EI		CRITICAL_END

//#define DI	sys_disable_int
//#define EI	sys_enable_int

//extern volatile UINT gnSR_Buffer;
#ifdef DEF_CPU1
extern volatile BYTE gbXsrTaskSwitchNeeded;
#endif

#define MAX_TIMEZONE 141
extern int nTimeZoneSec[][3];
extern const char* const strWeek[];
extern const char* const strMonth[];

//-------------------------------------------------------------------------------------------------
// Bridge addressing (Isp+Audio+Image+Voice...)
//#define	SetBrg(addr, data)		(REG32(BRG_BASE+(addr<<2)) = data)	// Write to bridge(Isp,Aud)
//#define	GetBrg(addr)			(REG32(BRG_BASE+(addr<<2))		 )	// Read from bridge(Isp,Aud)

//#define	dmacpy	DmaMemCpy					// can be overridden by users
//#define	dmaset	DmaMemSet					// can be overridden by users
//#define	dmachksum	DmaChkSum				// can be overridden by users

//#define	dmacpy	memcpy					// can be overridden by users
//#define	dmaset	memset					// can be overridden by users

//******************************************************************************
// 9. DMA
//------------------------------------------------------------------------------
//extern void* DmaMemCpy(void* dst, const void* src, size_t len);
//extern void DmaMemSet(BYTE * dst, BYTE Value, UINT len);

//#define	dmacpy	DmaMemCpy					// can be overridden by users
//#define	dmaset	DmaMemSet					// can be overridden by users

//#define	dmacpy	memcpy					// can be overridden by users
//#define	dmaset	memset					// can be overridden by users

// Function
#ifndef _INLINE
extern void DmaEi(void);
extern void DmaDi(void);
extern void DmaMemCpy_isr_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemCpy_isr(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemSet_isr(BYTE *apbDst, BYTE abVal, UINT anNum);
extern void DmaDChkSum_isr(BYTE *apbDst, UINT anNum, WORD *apnChkSum);
#endif

extern void DmaEthTxCpy_async(BYTE *apbSrc, UINT anNum);
extern void DmaEthTxCpy(BYTE *apbSrc, UINT anNum);
extern void DmaMemCpy_ip_async(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemCpy_sync(void* apbDst,  void* apbSrc, UINT anNum);

extern void DmaMemCpy_ip(BYTE *apbDst, BYTE *apbSrc, UINT anNum);
extern void DmaMemSet_ip(BYTE *apbDst, BYTE abVal, UINT anNum);
extern void DmaDChkSum_ip(BYTE *apbDst, UINT anNum, WORD *apnChkSum);
extern void DmaUChkSum_ip(BYTE *apbSrc, UINT anNum, WORD *apnChkSum);

//******************************************************************************
// 10. GPIO
//------------------------------------------------------------------------------

//******************************************************************************
// 14. ETHERNET - (15.10.23):hjlee
//------------------------------------------------------------------------------
// Port
#define RTSP_PORT				554						// RTSP server port (Default is 554)
#define HTTP_PORT				80						// HTTP server port (Default is 80)
#define HTTP_ONVIF_PORT 		80//8080				//ONVIF server port
#define UPNP_MCAST_PORT			1900					// UPNP multicast port (Default is 1900)
#define UPNP_LOCAL_PORT			55555					// UPNP local port (Default is random)
#define SNTP_PORT				123						// SNTP server port (Default is 123)

//extern void EthInit(void);
//extern void EthTxPkt(BYTE* SrcAdr, UINT PktLen);
//extern UINT EthRxPkt(BYTE* DstAdr);
//extern void EthIsr(void);
//extern void MdioRead(BYTE phyadr, BYTE regadr, WORD *data);
//extern void MdioWrite(BYTE phyadr, BYTE regadr, WORD data);

//extern void EthEiTx(void);
//extern void EthDiTx(void);
//extern void EthEiRx(void);
//extern void EthDiRx(void);
//extern void EthTxEna(void);
//extern void EthTxDis(void);
//extern void EthRxEna(void);
//extern void EthRxDis(void);
//extern void EthPauseEna(void);
//extern void EthPauseDis(void);
//extern void EthProModeEna(void);
//extern void EthProModeDis(void);
//extern void EthHalfDupEna(void);
//extern void EthHalfDupDis(void);
//extern void EthLoopBackEna(void);
//extern void EthLoopBackDis(void);
//extern void EthMdioClkDiv(BYTE abDiv);
//extern void EthSetHash(ULLONG adHash);
extern void EthSetMacAdr(BYTE *apdMacAdr);
extern void EthDuplexChange(UINT duplex);

extern void EthRxTxinit(UINT type, UINT speed, UINT duplex);

//extern void EthInit(void);
//extern void EthTxPkt_new(BYTE *apSrc, UINT anLen);

extern void EthMulFiltEnable(void);
extern void EthMulFiltDisable(void);
extern void EthMulFiltClear(void);
extern void EthMulFiltInsert(UINT mulIp);
extern void Eth8021DFiltInsert(UINT gmac);

extern void IsrWifiEvt(void *dummy);
extern void IsrWifiRx(void *dummy);
extern void IsrEthRx(void *dummy);
//extern void IsrEthTx(void *dummy);
//extern void EthDuplexChange(UINT duplex);

//******************************************************************************
// 16. SDIO
//------------------------------------------------------------------------------
typedef enum {
	eSDIO_CLK_100KHz = 998,
	eSDIO_CLK_200KHz = 499,
	eSDIO_CLK_300KHz = 332,
	eSDIO_CLK_400KHz = 249,
	eSDIO_CLK_10MHz = 9,
	eSDIO_CLK_25MHz = 3,
	eSDIO_CLK_33MHz = 2,
	eSDIO_CLK_50MHz = 1,
	eSDIO_CLK_100MHz = 0,
} SDIO_CLK;

// CMD5
#define OCR_VOL_WIND		0x00ff8000

// R4
#define R4_C				0x80000000
#define R4_FUNC_NUM			0x70000000
#define R4_MEM_PRESENT		0x08000000
#define R4_OCR				0x00ffff00

// CMD52
#define CMD52_READ  		0x00000000
#define CMD52_WRITE			0x80000000
#define R5_RWMASK			0x000000ff

// CMD53
#define CMD53_READ  		0x00000000
#define CMD53_WRITE  		0x80000000
#define CMD53_BMODE			0x08000000

#define SDIO_3V3			1
#define SDIO_1V8			0

//extern void Sdio0EiDet(void);
//extern void Sdio0DiDet(void);
//extern void Sdio0EiCmd(void);
//extern void Sdio0DiCmd(void);
//extern void Sdio0EiDat(void);
//extern void Sdio0DiDat(void);
//extern void Sdio0EiCip(void);
//extern void Sdio0DiCip(void);

extern UINT Sdio0GetActive(void);
extern UINT Sdio0GetSectorCnt(void);
extern UINT Sdio0GetAUSize(void);

extern void Sdio0Init(void);
extern void Sdio0Deinit(void);
extern BYTE Sdio0InitProcess(void);
extern BOOL Sdio0SetClock(void);				// shell_cmd test(CMD 6)
extern BOOL Sdio0SetBlockLength(void);			// shell_cmd test(CMD16)
extern void Sdio0ClockDiv(SDIO_CLK nSetKHz);	// shell_cmd test

extern BYTE Sdio0E(UINT start_sctor, UINT end_sctor);
extern BYTE Sdio0SectorW(const BYTE *buff, UINT sector);
extern BYTE Sdio0W(BYTE *buff, UINT sector, UINT count);
extern BYTE Sdio0SectorR(const BYTE *buff, UINT sector);
extern BYTE Sdio0R(BYTE *buff, UINT sector, UINT count);

extern UINT Sdio0GetInstall(void);

extern void IsrSdio0(void *dummy);

extern void Sdio1EiDet(void);
extern void Sdio1DiDet(void);
extern void Sdio1EiCmd(void);
extern void Sdio1DiCmd(void);
extern void Sdio1EiDat(void);
extern void Sdio1DiDat(void);
extern void Sdio1EiCip(void);
extern void Sdio1DiCip(void);

extern void Sdio1PadVoltage(int voltage);
extern void Sdio1Init(void);
extern BYTE Sdio1InitProcess(void);
//extern BOOL Sdio1SetClock(void);
//extern BOOL Sdio1SetBlockLength(void);
//extern void Sdio1ClockDiv(SDIO_CLK nSetKHz);
extern void Sdio1Cfg(SDIO_CLK nSetKHz, WORD DataBl, BYTE Mode);

extern BYTE Sdio1E(UINT start_sctor, UINT end_sctor);
extern BYTE Sdio1SectorW(const BYTE *buff, UINT sector);
extern BYTE Sdio1W(BYTE *buff, UINT sector, UINT count);
extern BYTE Sdio1SectorR(const BYTE *buff, UINT sector);
extern BYTE Sdio1R(BYTE *buff, UINT sector, UINT count);

extern UINT Sdio1GetInstall(void);

extern void IsrSdio1(void *dummy);

extern BYTE Sdio1RegW(BYTE data, UINT func, UINT addr);
extern BYTE Sdio1RegR(BYTE *data, UINT func, UINT addr);


//******************************************************************************
// 20. UART
//------------------------------------------------------------------------------

//******************************************************************************
// 21. UTIL
//------------------------------------------------------------------------------
//extern void Secu_Init();
//extern void AesEncryt(BYTE * Dst, BYTE * Src, UINT BkCnt);
//extern WORD ChkSum(BYTE * Src, UINT Len);
extern void EndianConv(BYTE * Dst, BYTE * Src, UINT Len, BYTE Size);
//extern void G711Enc(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law);
//extern void G711Dec(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law);
#define bitchang(bitCheckValue, bitLocation, setBit) (setBit ? (bitCheckValue | bitLocation) : (bitCheckValue & (~(bitLocation))))
#define bitcheck(bitCheckValue, bitLocation) (bitCheckValue & bitLocation)
#define num_loop(val, MAX)	{ ++(val); if((val)>=(MAX)){val = 0;} }
#define cQueue_isfull(cQueue)	(((((cQueue)->tail+1) % (cQueue)->tot_num) == (cQueue)->head) ? DEF_OK : DEF_FAIL)
#define cQueue_isempty(cQueue)	(((cQueue)->head == (cQueue)->tail) ? DEF_OK : DEF_FAIL)

void QuickSort(int numbers[], int array_size);
BYTE hex2ascii(BYTE hex);
void hexDump(char *desc, void *addr, int len);

//******************************************************************************
// 24. ISP
//------------------------------------------------------------------------------


//******************************************************************************
// 26. RTOS
//------------------------------------------------------------------------------
// Task size
#define _64KB_STACK_SIZE		(64*1024)
#define _32KB_STACK_SIZE		(32*1024)
#define _16KB_STACK_SIZE		(16*1024)

#define ETHRX_STACK_SIZE		(32*1024)				// Aucutal size in bytes = X * sizeof(portSTACK_TYPE)
#define TCPIP_STACK_SIZE		(32*1024)				//			"
#define IDLE_STACK_SIZE			(32*1024)				//			"
#define XSR_STACK_SIZE			(32*1024)				//			"

#define WIFI_STACK_SIZE         (64*1024)
#define SHELL_STACK_SIZE		(64*1024)				//			"
#define JABBU_STACK_SIZE		(32*1024)				//			"
#define MAKEAVI_STACK_SIZE		(64*1024)				//			"
#define DHCPC_STACK_SIZE		( 8*1024)
#define ONVIF_STACK_SIZE		(64*1024)
#define UPNPUDPD_STACK_SIZE		( 8*1024)				//			"

//------------------------------------------------------------------------------
// Task priority
#define ETHRX_TASK_PRIO			(tskIDLE_PRIORITY + 7)	// Must be highest   priority
#define TCPIP_TASK_PRIO			(tskIDLE_PRIORITY + 6)	// Must be highest-1 priority
#define MAKEAVI_TASK_PRIO		(tskIDLE_PRIORITY + 5)
#define FTPD_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define HTTPD_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define SNTPC_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define TIMER_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define ISP_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define WIFI_TASK_PRIO			(tskIDLE_PRIORITY + 7)
#define WIFI_AP_TASK_PRIO		(tskIDLE_PRIORITY + 7)
#define WIFI_STA_TASK_PRIO		(tskIDLE_PRIORITY + 7)
#define JABBU_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define SHELL_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define DHCPC_TASK_PRIO			(tskIDLE_PRIORITY + 6)
#define LOW_TASK_PRIO			(tskIDLE_PRIORITY + 4)
#define DISCOVERY_TASK_PRIO		(tskIDLE_PRIORITY + 5)
#define ONVIF_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define REBOOT_TASK_PRIO		(tskIDLE_PRIORITY + 3)
#define CHANGE_TASK_PRIO		(tskIDLE_PRIORITY + 5)
#define IPUTIL_TASK_PRIO		(tskIDLE_PRIORITY + 5)

#endif//_DEV_H_
