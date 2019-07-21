/* **************************************************************************
 File Name	: 	dev_isp.h
 Description:	EN673 - Device Driver
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 17 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

/*
 INDEX	:
		 1. System
		 2. GPIO
		 3. ADC
		 4. DEV
		 5. UART
		 6. I2C
		 7. TIMER
		 8. SPI
		 9. IRQ
		10. FLASH
		11. WDT
		12. ISP
		13. SENSOR
		14. ETC
*/
#include "dev.h"

//*************************************************************************************************
// 1. System Mode user define
//-------------------------------------------------------------------------------------------------

#ifdef __ISP_LIB__
	#ifdef model_Sens
		#include "dev_isp_common.h"
	#endif
#else
	#include "dev_isp_common.h"
#endif



#ifdef model_Sens


//#include <string.h>					// for strlen


#if 1//def DEF_CPU0
//#include "isp_dev_misc.h"
//#include "dev_isp_misc.h"

#include "isp_dev_sensor.h"
//#include "isp_dev_reg_isp.h"


//*************************************************************************************************
// 4. DEV
//-------------------------------------------------------------------------------------------------
// Extern

//*************************************************************************************************
// 5. UART
//-------------------------------------------------------------------------------------------------

#define UART_BUF_SIZE		128				// User buffer size

typedef /*volatile*/ struct _UART_QUE{		// Queue structure define
	volatile WORD	wRxLen;					// Number of characters in the Rx ring buffer
	volatile BYTE*	pbRxHead;				// Pointer to where next character will be inserted
	volatile BYTE*	pbRxTail;				// Pointer from where next character will be extracted
	volatile BYTE	bRxQue[UART_BUF_SIZE];	// Ring buffer character storage (Rx)

	volatile WORD	wTxLen;					// Number of characters in the Tx ring buffer
	volatile BYTE*	pbTxHead;				// Pointer to where next character will be inserted
	volatile BYTE*	pbTxTail;				// Pointer from where next character will be extracted
	volatile BYTE	bTxQue[UART_BUF_SIZE];	// Ring buffer character storage (Tx)
} TUartQue;

extern TUartQue gtUartQue;
extern TUartQue gtUartQue1;


#define	UART_BAUD(baud)		((MCLK+(baud<<3)) / (baud<<4))
	#define	UB2400			UART_BAUD(2400)/*1934*/
	#define	UB4800			UART_BAUD(4800)/*967*/
	#define	UB9600			UART_BAUD(9600)/*483*/
	#define	UB19200			UART_BAUD(19200)/*242*/
	#define	UB38400			UART_BAUD(38400)/*121*/
	#define	UB57600			UART_BAUD(57600)/*81*/
	#define	UB115200		UART_BAUD(115200)/*40*/

#define UB_INIT			/*UB57600*/UB115200
#define MN_BR_INIT		/*MN_BR_57600*/MN_BR_115200

//*************************************************************************************************
// 6. I2C
//-------------------------------------------------------------------------------------------------
#define I2C_CLKDIV32		0
#define I2C_CLKDIV64		1
#define I2C_CLKDIV96		2		// 773.437 Kbps
#define I2C_CLKDIV128		3
#define I2C_CLKDIV160		4
#define I2C_CLKDIV192		5		// 386.718 Kbps
#define I2C_CLKDIV224		6
#define I2C_CLKDIV256		7
#define I2C_CLKDIV288		8
#define I2C_CLKDIV320		9
#define I2C_CLKDIV352		10
#define I2C_CLKDIV384		11
#define I2C_CLKDIV416		12
#define I2C_CLKDIV448		13
#define I2C_CLKDIV480		14
#define I2C_CLKDIV512		15		// 145 Kbps
#define I2C_CLKDIV2048		0x3F
#define I2C_CLKDIV8192		0xFF

#ifndef __ISP_LIB__

#define SLAVE_DEVICE		0x20	// For EN673 Master

#define EN331A_DEVICE		0x84

#define EN332_DEVICE		0xFC

#define PO2210_DEVICE		0xEE	// 0xEE(W), 0xEE|0x01(R)

#define PO3100_DEVICE		0xEE	// 0xEE(W), 0xEE|0x01(R)


#if (USE_DIGITAL_INPUT==1)||(USE_DIGITAL_INPUT==2)||(USE_DIGITAL_INPUT==3)||(USE_DIGITAL_INPUT==4)
	extern const UINT gnTbDSubInputInit[];
	extern const UINT gnTbDSubInputInitEa;

	#if (IMAGE_INPUT_MODE==1)||(IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3)
	extern const UINT gnTbDSubInputAddr[];
	#endif

#elif (USE_DIGITAL_INPUT==5)
	extern const UINT gnTbDSubInputInit[][2];
	extern const UINT gnTbDSubInputInitEa;

#endif

#define EEPROM_DEVICE		0xa2	// The A0,A1,A2 input pins are used by the 24LC64 for multiple device operation.
#define	EEPROM_PAGE_EA		256		// 24LC64 PAGE No.
#define	EEPROM_BYTE_EA		32		// 24LC64 BYTE No. each Page

//	#define I2C_CLK_SET(CH,SPEED)	I2C##CH##_CLK_DIV	= (MCLK/(32*SPEED)-1)
//	#define I2C_CLK_SET(CH,SPEED)	I2C##CH##_CLK_DIV	= (MCLK/(8*SPEED)-1)

#define I2C_CLKDIV_MASTER	/*I2C_CLKDIV96*//*I2C_CLKDIV192*//*I2C_CLKDIV512*/I2C_CLKDIV2048	// I2C_CLK = MCLK/I2C_CLKDIV512 = 74250000/512 = 145019 = 145Kbps  (가장 느린 설정=I2C_CLKDIV8192=8.192KHz)
#define I2C_CLKDIV_SLAVE	I2C_CLKDIV416	// I2C_CLK = MCLK/I2C_CLKDIV416 = 74250000/416 = 178485 = 178Kbps

#endif//__ISP_LIB__
//-------------------------------------------------------------------------------------------------
// Misc
#define	TWI_EEP_DEV_ADDR		0xA2									// 24C64 DEV Addr.
#define	TWI_EEP_PAGE_EA			256										// 24C64 PAGE No.
#define	TWI_EEP_BYTE_EA			32										// 24C64 BYTE No. each Page

// EN778
#define TWI_EEP_MENU_STT		0										// Menu par save pos (1024 byte space)
#define TWI_EEP_USER_STT		(32*TWI_EEP_BYTE_EA)					// User par save pos (1024 byte space)
#define TWI_EEP_DATA_STT		(64*TWI_EEP_BYTE_EA)					// Application data save pos (1024 byte space)

#define TWI_EEP_CSTM_STT		(96*TWI_EEP_BYTE_EA)					// Customer area

/*
	Customer area

*/

#define TWI_EEP_END				((TWI_EEP_PAGE_EA*TWI_EEP_BYTE_EA)-1)	// End of address


#define TWI_EEP_AWBPRST_STT		(64*TWI_EEP_BYTE_EA)					// AWB Preset data save pos
#define TWI_EEP_IRIS_STT		(65+TWI_EEP_BYTE_EA)					// IRIS data save pos
#define TWI_EEP_ZOOM_STT		(66+TWI_EEP_BYTE_EA)					// ZOOM data save pos


//-------------------------------------------------------------------------------------------------
// Extern
#define I2C_BUF_SIZE		128				// User buffer size

typedef /*volatile*/ struct _I2C_QUE{		// Queue structure define
	volatile WORD	wRxLen;					// Number of characters in the Rx ring buffer
	volatile BYTE*	pbRxHead;				// Pointer to where next character will be inserted
	volatile BYTE*	pbRxTail;				// Pointer from where next character will be extracted
	volatile BYTE	bRxQue[I2C_BUF_SIZE];	// Ring buffer character storage (Rx)
} TI2cQue;


extern volatile UINT gnI2cPacket[2];

#if model_I2cSlave == 2
	extern volatile int giI2cMasterOn;
	extern BYTE I2c_mode(const BYTE abMasterOn);

	#define I2C_MASTER_ON(ERR)	I2C_CODE(ERR, I2c_mode(1))
	#define I2C_MASTER_OFF()	I2c_mode(0);
#else
	#define I2c_mode(...)
	#define I2C_MASTER_ON(ERR)
	#define I2C_MASTER_OFF()
#endif


#if model_I2cSlave == 1 || model_I2cSlave == 2

	#if model_I2cSlave == 1
		#define EN331_Write(...)
		#define EN331_Read(A,D)		*(D) = 0
		#define EN332_Write(...)
		#define EN332_Read(A,D)		*(D) = 0

		#define eep_write(A,D)		{ UNUSED_ARG(A); UNUSED_ARG(D); }
		#define eep_read(A,D,S)		{ UNUSED_ARG(A); *(D) = 0; UNUSED_ARG(S); }
		#define EepPageWrite(...)
	#endif

	extern TI2cQue gtI2cQue;

	#if STR_I2C_DATA == 1
		extern volatile UINT gnIsrI2cTx;
		extern volatile UINT gnIsrI2cTxR;
		extern volatile UINT gnIsrI2cRx;
		extern volatile UINT gnIsrI2cRxF;
	#endif

	extern void I2cRstQue(void);

#endif

#if model_I2cSlave == 0 || model_I2cSlave == 2

	#if model_I2cSlave == 0
		#define I2cRstQue(...)
	#endif

	extern BYTE i2c_write(BYTE dat, BYTE last, BYTE repeat);
	extern BYTE i2c_read(BYTE last, BYTE repeat);
	extern BYTE eep_write(UINT anAddr, BYTE abData);
	extern BYTE eep_read(const WORD awAddr, volatile BYTE* abData, const UINT anDataEa);
	extern BYTE EepPageWrite(const WORD awWaddr, BYTE* apbBuf, const UINT anBufEa);
	extern BYTE TwiWrEep2(const WORD awWaddr, volatile BYTE* apbBuf, const UINT anBufEa);
	#define	TwiWrEep2_continue	if(TwiWrEep2(0, 0, 0)) bTwiWrEep2Rdy = 1;
	#define TwiRdEep	eep_read

	BYTE TestRead(const BYTE abDevAddr, const BYTE abAddr, BYTE* abData, const UINT anDataEa);

	extern BYTE EN331_Write(WORD awAddr, UINT data);
	extern BYTE EN331_Read(WORD awAddr, UINT* anBuf);

	extern BYTE EN332_Write(WORD awAddr, UINT data);
	extern BYTE EN332_Read(WORD awAddr, UINT* anBuf);

	extern void SetSensTwi_Pixel(BYTE abDevaddr, BYTE abAddr, BYTE abData);
	extern BYTE GetSensTwi_Pixel(BYTE abDevaddr, BYTE abAddr);

	extern void SetSensTwi_Aptn(BYTE abDevaddr, WORD awAddr, WORD abData);
	extern WORD GetSensTwi_Aptn(BYTE abDevaddr, WORD awAddr);

	extern void SetSensTwi_Omni(BYTE abDevaddr, WORD awAddr, BYTE abData);	// 2015915 - WHL
	extern BYTE GetSensTwi_Omni(BYTE abDevaddr, WORD awAddr);

	extern BYTE i2c1_Write_Test(BYTE Address, WORD SubAddr, BYTE* bBuff, UINT nLength);	// 20151110 - WHL

#ifndef __ISP_LIB__
	// I2C Error Message
	enum {
		EEP_NO_ERROR,

		EEP_WRITE_NODEV,
		EEP_WRITE_MASTER,
		EEP_WRITE_NOACK1,
		EEP_WRITE_NOACK2,
		EEP_WRITE_NOACK3,
		EEP_WRITE_NOACK4,

		EEP_READ_NODEV,
		EEP_READ_MASTER,
		EEP_READ_NOACK1,
		EEP_READ_NOACK2,
		EEP_READ_NOACK3,

		EN331_WRITE_NODEV,
		EN331_WRITE_MASTER,
		EN331_WRITE_NOACK1,
		EN331_WRITE_NOACK2,
		EN331_WRITE_NOACK3,
		EN331_WRITE_NOACK4,
		EN331_WRITE_NOACK5,
		EN331_WRITE_NOACK6,
		EN331_WRITE_NOACK7,

		EN331_READ_NODEV,
		EN331_READ_MASTER,
		EN331_READ_NOACK1,
		EN331_READ_NOACK2,
		EN331_READ_NOACK3,
		EN331_READ_NOACK4,

		PIXEL_WRITE_NODEV,
		PIXEL_WRITE_MASTER,
		PIXEL_WRITE_NOACK1,
		PIXEL_WRITE_NOACK2,
		PIXEL_WRITE_NOACK3,
		PIXEL_WRITE_NOACK4,

		PIXEL_READ_NODEV,
		PIXEL_READ_MASTER,
		PIXEL_READ_NOACK1,
		PIXEL_READ_NOACK2,
		PIXEL_READ_NOACK3,

		SONY_WRITE_NODEV,
		SONY_WRITE_MASTER,
		SONY_WRITE_NOACK1,
		SONY_WRITE_NOACK2,
		SONY_WRITE_NOACK3,
		SONY_WRITE_NOACK4,

		SONY_READ_NODEV,
		SONY_READ_MASTER,
		SONY_READ_NOACK1,
		SONY_READ_NOACK2,
		SONY_READ_NOACK3,
		SONY_READ_NOACK4,

		TEST_READ_NODEV,
		TEST_READ_MASTER,
		TEST_READ_NOACK1,
		TEST_READ_NOACK2,
		TEST_READ_NOACK3,

		PO2210_WRITE_NOACK1,
		PO2210_WRITE_NOACK2,

		PO2210_READ_NOACK1,
		PO2210_READ_NOACK2,
	};


#endif//__ISP_LIB__

	extern BYTE gbEepromOff;
	extern BYTE gbEn331Off;
	extern BYTE gbSensorOff;

#endif


//*************************************************************************************************
// 8. SPI
//-------------------------------------------------------------------------------------------------
#define SPI_CLKDIV4			0
#define SPI_CLKDIV8			1
#define SPI_CLKDIV12		2
#define SPI_CLKDIV16		3
#define SPI_CLKDIV20		4
#define SPI_CLKDIV24		5
#define SPI_CLKDIV28		6
#define SPI_CLKDIV32		7
#define SPI_CLKDIV36		8
#define SPI_CLKDIV40		9
#define SPI_CLKDIV44		10
#define SPI_CLKDIV48		11
#define SPI_CLKDIV52		12
#define SPI_CLKDIV56		13
#define SPI_CLKDIV60		14
#define SPI_CLKDIV64		15

#define SPI_MSB				0
#define SPI_LSB				1
#define SPI_START_LOW		0
#define SPI_START_HIGH		1
#define SPI_EDGE_RISING		0
#define SPI_EDGE_FALLING	1
#define SPI_BIT_BYTE		0
#define SPI_BIT_WORD		1

#define SPI0_SET(EN,CLKDIV,START/*,EDGE*/)	\
	SPI0_SPI_CONT = (EN<<31) | (CLKDIV<<16) | (START<<4) /*| (EDGE<<3)*/	// 2015516 - WHL

#define SPI1_SET(EN,CLKDIV,START/*,EDGE*/)	\
	SPI1_SPI_CONT = (EN<<31) | (CLKDIV<<16) | (START<<4) /*| (EDGE<<3)*/	// 2015516 - WHL

#define SPI_END(NUM)	SPI1_SPI_CONT |= (1<<(NUM+8))	// SPI1_SPI_CS:NUM = 1(HIGH)
#define SPI_START(NUM)	SPI1_SPI_CONT &= (~1<<(NUM+8))	// SPI1_SPI_CS:NUM = 0(LOW)

//#define	SPI_CLK_SET(CH,SPEED)	SPI##CH##_CLKDIV = (MCLK/(4*SPEED)-1)

//-------------------------------------------------------------------------------------------------
// Extern
extern void SPI0_LSB_WRITE(BYTE abData);
extern void SPI_MSB_WRITE(BYTE abData);
extern BYTE SPI_MSB_READ(void);

extern void SetSensSpi_Pana(WORD awAddr, WORD awData);
extern WORD GetSensSpi_Pana(WORD awAddr);
extern void SetSensTwi_Pana(WORD awAddr, WORD awData);
extern WORD GetSensTwi_Pana(WORD awAddr);

extern void SetSensSpiBurst_Sony(BYTE abID, BYTE abAddr, BYTE* abData, BYTE abLength);
extern void SetSensSpi_Sony(WORD awID, WORD awAddr, WORD awData);
extern BYTE GetSensSpi_Sony(BYTE abID, BYTE abAddr);
extern void SetSensTwiBurst_Sony(BYTE abDevaddr, BYTE abAddr, BYTE* abData, BYTE abLength);
extern void SetSensTwi_Sony(BYTE abDevaddr, BYTE abAddr, BYTE abData);
extern BYTE GetSensTwi_Sony(BYTE abID, BYTE abAddr);


extern void SPI1_LCD_Read(BYTE abCMD, BYTE* abArg, UINT anArgEa);
extern void SPI1_LCD_Write(BYTE abCMD, BYTE* abArg, UINT anArgEa);

extern UINT ISPM0 GetSt(UINT *);
extern void ISPM0 SetStDisIrq(UINT);
extern void ISPM0 SetStEnaIrq(UINT);
extern void ISPM0 SetSt(const UINT, UINT *);
extern UINT ISPM0 GetTT(void);
extern UINT ISPM0 GetTTmax(void);

//extern UINT GetSt(UINT *);
//extern void SetStDisIrq(UINT);
//extern void SetStEnaIrq(UINT);
//extern void SetSt(const UINT, UINT *);
//extern UINT GetTT(void);
//extern UINT GetTTmax(void);




//*************************************************************************************************
// 9. IRQ
//-------------------------------------------------------------------------------------------------

// Extern
extern void WaitTt(UINT anUs);

extern volatile UINT gnIsrTick;
extern volatile UINT gnIsrTickOn;
extern UINT BT1120_FPS,BT1120_FLAG;



//*************************************************************************************************
// 10. FLASH
//-------------------------------------------------------------------------------------------------
// Block Protection Set - Flash Status Register
#define SFLS_BP_SET(BP3,BP2,BP1,BP0)	sfls_write_reg( (BP3<<5) | (BP2<<4) | (BP1<<3) | (BP0<<2) )

//-------------------------------------------------------------------------------------------------
// Extern
#if (model_Protect==1) && (model_Flash==0)
	#define SfWrEn		{ SFLS_WB_WR_EN = 1; SFLS_BP_SET(0,0,0,0); gbSF_Status = sfls_read_reg(); }
	#define SfWrDis		{ SFLS_WB_WR_EN = 0; SFLS_BP_SET(1,1,1,1); gbSF_Status = sfls_read_reg(); }
	#define SfWrInit	SfWrDis
#else
	#define SfWrEn		{ SFLS_WB_WR_EN = 1; }
	#define SfWrDis		{ SFLS_WB_WR_EN = 0; }
	#define SfWrInit	{ SFLS_WB_WR_EN = 0; SFLS_BP_SET(0,0,0,0); gbSF_Status = sfls_read_reg(); }
#endif

extern BYTE gbSF_Status;
extern BYTE gbSFLS_WB_RDCMD_IOM;

// Conversion ISP function name for EN673 compatiblity
#define	sfls_init			SflsInit
#define	sfls_write_en		SflsWriteEn
#define	sfls_erase_sect		SflsEraseSect
#define	sfls_erase_all		SflsEraseAll
#define	sfls_write_reg		SflsWriteReg
#define	sfls_read_reg		SflsReadReg

//extern void sfls_page_program(int Adr, BYTE* abData, const UINT anDataEa);

extern BYTE SfWrite(UINT anAddr, BYTE* apbData, UINT anNum);	// 2015818 - WHL
extern BYTE SfRead(UINT anAddr, BYTE* apbData, UINT anNum);
extern BYTE Sf2Isp( UINT anSfaddr, UINT anIspAddr, UINT anIspBufEa, BYTE abSizeByte); 	// 2015818 - WHL
extern BYTE SfWrite2(volatile BYTE* apbData, UINT anNum, BYTE abMode);
extern BYTE SfRead2(volatile BYTE* apbData, UINT anNum, BYTE abMode);


//-------------------------------------------------------------------------------------------------
// Misc
#define FLASH_ADDR_MASK			0x00FFFFFF		// 16384KB : 0x00000000~0x00FFFFFF
#define FLASH_TOTAL_SIZE		(16384*1024)	// 16384KB	(128Mbit quad)
#define FLASH_SECT_SIZE			(4*1024)		// 4KB
#define FLASH_SECT_TOTAL_EA		4096

#define FLASH_SECT_SHD_STT		4086			// sect no. of Shading table

#define FLASH_SECT_MENU_STT		4087			// sect no. of Menu Parameter (Roll1)
#define FLASH_SECT_MENU_EA		1
#define FLASH_SECT_MENU_BKUP	4088			// sect no. of Menu Parameter (Roll2)

#define FLASH_SECT_USER_STT		4089 			// sect no. of User parameter
#define FLASH_SECT_DATA_STT		4090 			// sect no. of Application data


enum {
	FLASH_MENU_MODE,
	FLASH_USER_MODE,
	FLASH_DATA_MODE
};


//*************************************************************************************************
// 11. WDT
//-------------------------------------------------------------------------------------------------
// Misc
#define WdtEna			SYS_WDT_EN = 1  /*WDT_CNT_EN = 1*/
#define WdtDis			SYS_WDT_EN = 0  /*WDT_CNT_EN = 0*/
#define WdtRst			SYS_WDT_RST = 1 /*WDT_CNT_RST = 1*/

extern void SystemReset(void);



//*************************************************************************************************
// 12. ISP (EN673)
//-------------------------------------------------------------------------------------------------
// Misc
//#define	ISP_BASE				ISP_REG_BASE	// Normal reg start addr.

#define ISP_BASE_SHADING		0x400		// (fix) Shading table start addr			(0x400 ~ 4ff)
#define	ISP_BASE_FONT_ID		0x1000		// Font
#define	ISP_BASE_FONT_ATTR		0x1800		// "
#define	ISP_BASE_FONT_CHAR		0x2000		// "

//#define ISP_SHADING_EA			256				// (fix) number of Shading table
#define ISP_SHADING_EA			144				// (fix) number of Shading table	// 2015827 - WHL : Logic Shading Table


#define ISP_BOX_EA				32				// (fix) number of Box

#define ISP_FONT_EAX			46/*30*/			// number of Horizontal Font		ksh modi
#define ISP_FONT_ID_EA			2048		// number of Font id
#define ISP_FONT_ATTR_EA		2048		// number of Font attr
#define	ISP_FONT_CHAR_EA		341			// number of font character

//#define ISP_FONT_DEBUG_X		24			// Start Position for Debugging
#define ISP_FONT_MAX_PATH		10			// number of String for Debugging
//#define ISP_FONT_SPACE			1			// number of Space for Debugging

#define SPACE_CHAR_ID			' '

#define ISP_IRQ_VLOCKI			0			// VLOCKI, Sensor 입력 Sync 기준
#define ISP_IRQ_VLOCKW			1			// VLOCKW, Sensor 입력 Sync 기준 (WDR 기준 Sync)
#define ISP_IRQ_VLOCKO			2			// VLOCKO, ISP 출력 Sync 기준(Low Shutter 제외하고는 VLOKCI와 VLOCKO가 동일 함.)
#define ISP_IRQ_JPGEND			3
#define ISP_IRQ_USER0			4			// User Interrupt 0 (VLOCKI 기준)
#define ISP_IRQ_USER1			5			// User Interrupt 1 (VLOCKO 기준)
#define ISP_IRQ_VLOCKW2			6			// VLOCKW&O, AND Sync


//-------------------------------------------------------------------------------------------------
// Bridge addressing (Isp+Audio+Image+Voice...)

extern UINT gnFontdummy;

#define SetFontChar(anAddr,anData)	{_wr32(ISP_BASE+((ISP_BASE_FONT_CHAR +(UINT)(anAddr))<<2),  (UINT)(anData));\
									gnFontdummy = *(volatile unsigned int*)(ISP_BASE+((ISP_BASE_FONT_CHAR +(UINT)(anAddr))<<2)); }

#define SetFontAttr(anAddr,anData)  {_wr32(ISP_BASE+((ISP_BASE_FONT_ATTR +(UINT)(anAddr))<<2), ((UINT)(anData)&0x3));\
									gnFontdummy = *(volatile unsigned int*)(ISP_BASE+((ISP_BASE_FONT_ATTR +(UINT)(anAddr))<<2)); }

//#define SetFont(Y,X,A)		gbFont[Y][X] = A
#define SetFontId(anAddr,anData)	{_wr32(ISP_BASE+((ISP_BASE_FONT_ID	 +(UINT)(anAddr))<<2), ((UINT)(anData)&0x1ff));\
									gnFontdummy = *(volatile unsigned int*)(ISP_BASE+((ISP_BASE_FONT_ID +(UINT)(anAddr))<<2)); }

#define SETFONTATTR(Y,X,A)			{ SetFontAttr((((Y)*ISP_FONT_EAX)+(X)), A); }
#define SETFONTID(Y,X,A)			{ SetFontId((((Y)*ISP_FONT_EAX)+(X)), A); }

#define SetShading(anAddr,anData)	{_wr32(ISP_BASE+((ISP_BASE_SHADING +(UINT)(anAddr))<<2),  (UINT)(anData));} 	//	asm("nop"); asm("nop");}


//-------------------------------------------------------------------------------------------------
// Extern
extern void InitIsp(void);
extern void isp_reset(void);

extern volatile BYTE gbVdiIrq;
extern volatile BYTE gbVdwIrq;
extern volatile BYTE gbVdoIrq;
extern volatile BYTE gbJpgIrq;

//extern void SetFontAttrs(UINT anPosY, UINT anPosX, UINT anLen, UINT anAttr);
//extern UINT DispStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anLen);
//extern void DispClr(UINT anPosY, UINT anPosX, UINT anLen);
//extern void DispClrStr(const char* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen);
//#if model_CharMax == 1
//extern void DispStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anLen);
//extern void DispClrStrEx(const EXCH* cStr, UINT anPosY, UINT anPosX, UINT anStrLen, UINT anClrLen);
//#endif
////	extern UINT DispImg(UINT idx, UINT anPosY, UINT anPosX, UINT anCnt);

#if 0
extern void WaitVdi(void);
extern void WaitVdw(void);
extern void WaitVdo(void);
extern void InitWaitVd(const int);
#endif

extern void TestPatternOn(void);

extern volatile UINT gnIsrISP0;
extern volatile UINT gnIsrISP1;
extern volatile UINT gnIsrISP2;
extern volatile UINT gnIsrISP5;
TII(extern volatile UINT gnIsrISP3;)
TII(extern volatile UINT gnIsrISP4;)
TII(extern volatile UINT gnTick_ISP;)

extern volatile UINT gnIsrJpeg;
extern volatile UINT gnIsrGpio;

extern BYTE gbIMDFontX;
extern BYTE gbIMDFontY;

//*************************************************************************************************
// 13. SENSOR
//-------------------------------------------------------------------------------------------------
extern void InitSensRun(void);
extern void Isp_SensorRst(void);
extern volatile BYTE gbSensCtrMode;



//*************************************************************************************************
// 14. ETC
//-------------------------------------------------------------------------------------------------
extern void* DmaMemCpy_Invd_Dchach(void *,const void *,int);
extern void GpioIsrEn(BYTE abSel);

#include "dev_isp_app.h"

#endif // end of "DEF_CPU0"

//#define	MN_LENS_MNL		0
//#define	MN_LENS_DC		1	// DC
//#define	MN_LENS_AUTO	1	// pIRIS, AF AUTO

#define DispDbgYmin		1
#define DispDbgX		24
#define DispDbgStrLen	11

//#ifndef __ISP_LIB__
//#include "msg.h"
//
//enum {
//	MP_START=0,			// Head
//
//	// System
//	FrameMode,
//	MpFrameRateLv,
//	MpSysFreq,
//	MpOutFps,
//	//MpOutScl,
//	//MpOutSclUser,
//	MpCBar,
//	MpReset,
//	MpLanguageSel,
//	MpOutFromat,
//	MpCamID,
//	MpBaudrate,
//	MpCamTitleOn,
//	MpCamTitleIdx,
//	MpCamTitle = MpCamTitleIdx+8-1,
//
//	// AE
//	MpBrightness,
//	MpIris,
//	MpIrisLvl,
//	MpShutter,
//	MpShutSpd,
//	MpDcMode,
//	MpShtMode,
//	MpDss,
//	MpAgc,
//
//	// Backlight
//	MpBackLight,
//	MpBackLightNor,
//	MpHlMaskThrs,
//	MpHlMaskColor,
//	MpBlcPosX,
//	MpBlcPosY,
//	MpBlcSizX,
//	MpBlcSizY,
//
//	//WDR_ROI
//#if model_WDR_ROI
//	MpROIWdrMode,
//	MpROIWdrZone,
//	MpROIWdrCfgIdx,
//	MpROIWdrCfg = MpROIWdrCfgIdx+(9*4)-1,
//#endif
//	MpWdrWgt,
//
//	// TDN, LED
//	MpTdn,
//	MpTdnThrs,
//	MpTdnMarg,
//	MpTdnDly,
//	MpLedOn,
//	MpLedSatCtrl,
//	//MpSmartIR,
//	MpTdnSw,
//	MpTdnDNThres,
//	MpTdnNDThres,
//
//	// ACE,Defog
//	MpAce,
//	MpDefog,
//	MpDefogMode,
//	MpDefogLevel,
//
//	// AWB
//	MpAwb,
//	MpAwbPrst,
//	MpAwbMnl,
//	MpRgain,
//	MpBgain,
//	MpSaturation,
//	MpSaturationR,	// 2017419 - WHL : CBB TEST
//	MpSaturationB,
//
//	// ETC
//	MpAdnr,
//	//MpHdaSharpness,
//	//MpSharpnessSel,
//	MpSharpness,
//	MpGamma,
//	MpFlip,
//	MpMirror,
//	MpShading,
//	MpShdWeight,
//
//	// Privacy Box
//	MpPrivacy,
//	MpPvcZone,
//	MpPvcFormat,
//	MpPvcTrans,
//	MpPvcYLevel,
//	MpPvcCRLevel,
//	MpPvcCBLevel,
//	MpPvcCfgIdx,
//	MpPvcCfg = MpPvcCfgIdx+(5*32)-1,
//
//	// IMD
//	MpItl,
//	//MpItlRes,
// 	MpItlSens,
// 	MpItlDettone,
// 	MpItlRectFill,
//	MpItlWindowuse,
//	MpItlWinCfgIdx,
//	MpItlWinCfg = MpItlWinCfgIdx+(5*4)-1,
// 	MpItlMaskOsd,
// 	MpItlAlarm,
// 	MpItlSigno,
//
//	// Hidden MENU
//	MpShdDet,
//	MpDefDet,
//	//MpDefectThrs,
//
//	// Test
//	//MpClockOnOff,
//	//MpTest,
//
//#if(model_Lens==0)
//	MpFad_On,
//	MpFad_Zone,		// 150325 WHL - Focus Adj.
//	MpFad_Trans,
//	MpFad_PosX,
//	MpFad_PosY,
//	MpFad_SizX,
//	MpFad_SizY,
//
//#elif(model_Lens==1)
//	MpMAF_mode,
//	MpMAF_Filter,
//	MpMAF_MotorInit,
//	MpMAF_TestMode,
//	MpMAF_TotalPos_Z,
//	MpMAF_StartPos_Z,
//	MpMAF_EndPos_Z,
//	MpMAF_TotalPos_F,
//	MpMAF_StartPos_F,
//	MpMAF_EndPos_F,
//
//#endif
//
//
//#if(DOOR_BELL_CAMERA==1)
//	MpDoorCam_LP_Mode,
//#endif
//
//	MP_END_REAL
//};
//
//#define MP(I)					gbMnParTbl[I]
//
//#endif // #ifndef __ISP_LIB__


//enum{	// common
//   	MN_OFF,
//	MN_ON
//};
//
//enum {	// common
//	MN_3sLOW,
//	MN_3sMID,
//	MN_3sHI
//};
//
//enum {	// common
//   	MN_4sOFF,
//	MN_4sLOW,
//	MN_4sMID,
//	MN_4sHI
//};
//
//#if (model_2M30p)
//
//enum {	// MP(FrameMode)
//	MN_FRAMEMODE_NORMAL_25_30,
//	MN_FRAMEMODE_WDR_12_15,
//	MN_FRAMEMODE_DNR_25_30
//};
//	#define WDR_MODE	(MP(FrameMode) == MN_FRAMEMODE_WDR_12_15)
//	#define DNR_MODE	(MP(FrameMode) == MN_FRAMEMODE_DNR_25_30)
//
//#else	// (model_1M)|(model_2M)
//
//enum {	// MP(FrameMode)
//	MN_FRAMEMODE_NORMAL_25_30,
//	MN_FRAMEMODE_WDR_12_15,
//	MN_FRAMEMODE_DNR_25_30,
//	MN_FRAMEMODE_NORMAL_50_60,
//	MN_FRAMEMODE_WDR_25_30,
//	MN_FRAMEMODE_DNR_50_60
//};
//	#define WDR_MODE	(MP(FrameMode) == MN_FRAMEMODE_WDR_12_15 || MP(FrameMode) == MN_FRAMEMODE_WDR_25_30)
//	#define DNR_MODE	(MP(FrameMode) == MN_FRAMEMODE_DNR_25_30 || MP(FrameMode) == MN_FRAMEMODE_DNR_50_60)
//#endif
//
//enum {	// MP(MpSysFreq)
//	MN_SYSFREQ_50,
//	MN_SYSFREQ_60
//};
//
//enum{	// MP(MpOutFps)
//	MN_2M_1080p_25_30,
//	MN_2M_1080p_50_60,
//	MN_2M_720p_25_30,
//	MN_2M_720p_50_60
//};
//
//enum{	// MP(MpOutFps)
//	MN_2M30_1080p_25_30,
//	MN_2M30_720p_25_30,
//	MN_2M30_720p_50_60
//};
//
//enum{	// MP(MpOutFps)
//	MN_1M_720p_25_30,
//	MN_1M_720p_50_60
//};
//
//enum{	//MP(MpOutFromat)
//	MN_FRMT_SD,
//	MN_FRMT_HD,
//	MN_FRMT_UV
//};
//
//enum{
//	MN_BR_2400,
//	MN_BR_4800,
//	MN_BR_9600,
//	MN_BR_57600,
//	MN_BR_115200
//};
//
//enum {	//MP(MpShutter)
//	MN_SHUT_AUTO,
//	MN_SHUT_MNL,
//	MN_SHUT_FLICKER
//};
//
//enum{	//MP(MpDcMode)
//	MN_DC_IN,
//	MN_DC_OUT,
//	MN_DC_DEBLUR
//};
//
//enum{	//MP(MpShtMode)
//	MN_SHT_NORMAL,
//	MN_SHT_DEBLUR
//};
//
//enum {	//MP(MpDss)	// 14.2.13
//	MN_DSS_OFF	,
//	MN_DSS_X2	,
//	MN_DSS_X4	,
//	MN_DSS_X8	,
//	MN_DSS_X16	,
//	MN_DSS_X32	,
//	MN_DSS_X64
//};
//
//enum{	//MP(MpBackLight)
//	MN_BL_OFF,
//	MN_BL_HLC,
//	MN_BL_BLC,
//	MN_BL_WDR
//};
//
//enum {	//MP(MpTdn)
//	MN_TDN_AUTO,
//	MN_TDN_DAY,
//	MN_TDN_NIGHT,
//	MN_TDN_EXTERN
//};
//
//enum{	//MP(MpDefogMode)
//	MN_DEFOG_MANUAL,
//	MN_DEFOG_AUTO
//};
//
//enum {	//MP(MpAwb)
//	MN_AWB_AUTO,
//	MN_AWB_AUTOEXT,
//	MN_AWB_PRESET,
//	MN_AWB_MNL
//};
//
//enum {	//MP(MpGamma)
//	MN_GAMMA_045,
//	MN_GAMMA_055,
//	MN_GAMMA_065,
//	MN_GAMMA_075
//};
//
//enum {	//MP(MpCamTitleOn)
//	MN_CT_OFF,
//	MN_CT_RIGHT_UP,
//	MN_CT_LEFT_DOWN
//};
//
//#define PVC_EA				16//32			// number of Privacy Box (max 32ea, CAUTION. share with IMD box ea)
//
//typedef struct _tagPRIVACY{
//	BYTE	bAction;
//	BYTE	bPosX;
//	BYTE	bPosY;
//	BYTE	bSizX;
//	BYTE	bSizY;
//} _PRIVACY;
//
//extern void PrivacyBox(void);
//
////------------------------------------------------------------------------------
//// Last Box apply
//extern UINT gnBoxPosPVC[PVC_EA*2];
//extern UINT gnBoxPltPVC[PVC_EA];
//extern UINT gnBoxTonePVC[2];
//extern UINT gnBoxFillPVC;
//extern UINT gnBoxOnPVC;
//extern UINT gnBoxFmtPVC;	// 2015112 - WHL : Add Bit Box Mode
//
//#define gbMnPvcCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpPvcCfgIdx +((INDEX))*5))
//
//#define gbMnImdCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpItlWinCfgIdx +((INDEX))*5))


#endif // end of "model_Sens"

/* EOF */

