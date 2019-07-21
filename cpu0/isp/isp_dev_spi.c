/* **************************************************************************
 File Name	: 	dev_spi.c
 Description:	EN673 - Spi control device driver
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Date		:	14. 6. 18
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"


#if 1
void SPI0_LSB_WRITE(BYTE abData)		// tested MN34041 Sensor
{
//	SPI0_TX_DAT = ((abData)<<24);	// Big Endian Structure
	SPI0_TX_DAT = abData;
	
	SPI0_RW = 1;
	SPI0_GO = 1;
	while(SPI0_GO);
}

BYTE SPI0_LSB_READ(void)				// 
{
//		SPI0_RW = 0;
	SPI0_RW = 2;
	SPI0_GO = 1;
	while(SPI0_GO);
	
//	return (SPI0_RX_DAT&0xff000000)>>24;	// Big Endian Structure
	return (BYTE)SPI0_RX_DAT;
}

void SPI_MSB_WRITE(BYTE abData)			// tested EN25Q40A
{
//	SPI1_TX_DAT = ((abData)<<24);	// Big Endian Structure
	SPI1_TX_DAT = abData;	// Big Endian Structure
	
	SPI1_RW = 1;
	SPI1_GO = 1;
	while(SPI1_GO);
}

BYTE SPI_MSB_READ(void)					// tested EN25Q40A
{
	SPI1_RW = 0;
	SPI1_GO = 1;
	while(SPI1_GO);
	
//	return (SPI1_RX_DAT&0xff000000)>>24;	// Big Endian Structure
	return (BYTE)SPI1_RX_DAT;	// Big Endian Structure
}

#else
void SPI0_LSB_WRITE(BYTE abData)		// tested MN34041 Sensor
{
	SPI0_TX_DAT = abData;

	SPI0_SPI_CONT = (SPI0_SPI_CONT&(~0x40B)) | (SPI_LSB<<10) | (SPI_EDGE_RISING<<3) | (SPI_BIT_BYTE<<2) | 3; // 3 => SPI0_RW[1] = 1 & SPI0_GO[0] = 1
	while(SPI0_GO);
}

BYTE SPI0_LSB_READ(void)				// 
{
	SPI0_SPI_CONT = (SPI0_SPI_CONT&(~0x40B)) | (SPI_LSB<<10) | (SPI_EDGE_RISING<<3) | (SPI_BIT_BYTE<<2) | 1; // 1 => SPI0_RW[1] = 0 & SPI0_GO[0] = 1
	while(SPI0_GO);

	return (SPI0_RX_DAT>>8);
}

void SPI_MSB_WRITE(BYTE abData)			// tested EN25Q40A
{
	SPI1_TX_DAT = abData<<8;

	SPI1_SPI_CONT = (SPI1_SPI_CONT&(~0x40B)) | (SPI_MSB<<10) | (SPI_EDGE_RISING<<3) | (SPI_BIT_BYTE<<2) | 3; // 3 => SPI1_CMD_RW[1] = 1 & SPI1_GO[0] = 1
	while(SPI1_GO);
}

BYTE SPI_MSB_READ(void)					// tested EN25Q40A
{
	SPI1_SPI_CONT = (SPI1_SPI_CONT&(~0x40B)) | (SPI_MSB<<10) | (SPI_EDGE_RISING<<3) | (SPI_BIT_BYTE<<2) | 1; // 1 => SPI1_CMD_RW[1] = 0 & SPI1_GO[0] = 1
	while(SPI1_GO);

	return (SPI1_RX_DAT>>8);
}
#endif

#if model_Pana

void SetSensSpi_Pana(WORD awAddr, WORD awData)
{	// data set to sensor, (adress 16b, data 16b, LSB first, active low)

#if (model_LSPI_MODE == 0)	//	Peripheral SPI

	SPI0_CS = 0;
	//WaitUs(1);	// SCS0
	SPI0_LSB_WRITE((awAddr & 0x00ff));
	SPI0_LSB_WRITE((awAddr & 0xff00)>>8);
	SPI0_LSB_WRITE((awData & 0x00ff));
	SPI0_LSB_WRITE((awData & 0xff00)>>8);
	//WaitUs(1);	// SCS1
	SPI0_CS = 1;

	//WaitUs(1);	// SCS2

#else	// 2015124 - WHL : Logic SPI Version

	static BYTE bLspiInit=1;

	if(bLspiInit) {
		SSTYPE_SELw(0);	// 2015124 - WHL : 0 -> Pana, 1 -> Sony
		ISSPILOGICw(1);
		LSPI_RSTNw(0);
		WaitXms(1000);
		LSPI_RSTNw(1);
		WaitXms(1000);

		bLspiInit=0;
	}
	SetIsp(0xfe,(UINT)((awAddr<<16)&0xFFFF0000)|(awData&0xFFFF));
	SetIsp(0xff,0x01);
	WaitXms(1);
	
#endif
}

WORD GetSensSpi_Pana(WORD awAddr)
{	// data get from sensor
	SPI0_CS = 0;

	SPI0_LSB_WRITE((awAddr & 0x00ff));
	SPI0_LSB_WRITE(((awAddr & 0xff00)>>8)|0x80);

	WORD wData = SPI0_LSB_READ();
	wData |= (SPI0_LSB_READ()<<8);
	
	SPI0_CS = 1;
	
	return wData;
}

#elif model_Sony

void SetSensSpi_Sony(WORD awID, WORD awAddr, WORD awData)
{	// data set to sensor with ID, (adress 8b, data 8b, LSB first, active low)

#if (model_LSPI_MODE == 0)	//	Peripheral SPI

	SPI0_CS = 0;

	SPI0_LSB_WRITE((BYTE)awID);

	SPI0_LSB_WRITE((BYTE)awAddr);
	SPI0_LSB_WRITE((BYTE)awData);

	SPI0_CS = 1;

#else	// 2015124 - WHL : Logic SPI Version

	static BYTE bLspiInit=1;

	if(bLspiInit) {
		SSTYPE_SELw(1);	// 2015124 - WHL : 0 -> Pana, 1 -> Sony
		ISSPILOGICw(1);
		LSPI_RSTNw(0);
		WaitXms(1000);
		LSPI_RSTNw(1);
		WaitXms(1000);

		bLspiInit=0;
	}
	SENSOR_WIDw(awID);
	SetIsp(0xfe,(UINT)((awAddr<<16)&0xFF0000)|(awData&0xFF));
	SetIsp(0xff,0x01);
	WaitXms(1);
	
#endif
}

void SetSensSpiBurst_Sony(BYTE abID, BYTE abAddr, BYTE* abData, BYTE abLength)
{
	UINT i=0;

	SPI0_CS = 0;

	SPI0_LSB_WRITE((BYTE)abID);
	SPI0_LSB_WRITE((BYTE)abAddr);
	
	for(i=0; i<abLength; i++) SPI0_LSB_WRITE(abData[i]);

	SPI0_CS = 1;
}

BYTE GetSensSpi_Sony(BYTE abID, BYTE abAddr)
{	// data get from sensor
	SPI0_CS = 0;
	
	SPI0_LSB_WRITE(abID);
	
	SPI0_LSB_WRITE(abAddr);
	
	BYTE bData = SPI0_LSB_READ();
	
	SPI0_CS = 1;
	
	return bData;
}

#endif



void SPI1_LCD_Read(BYTE abCMD, BYTE* abArg, UINT anArgEa)
{
	UINT i=0;

	SPI1_CS = 0;

	SPI_MSB_WRITE(abCMD);

	for(i=0; i<anArgEa; i++) abArg[i] = SPI_MSB_READ();

	SPI1_CS = 1;
}

void SPI1_LCD_Write(BYTE abCMD, BYTE* abArg, UINT anArgEa)
{
	UINT i=0;

	SPI1_CS = 0;

	SPI_MSB_WRITE(abCMD);

	for(i=0; i<anArgEa; i++) SPI_MSB_WRITE(abArg[i]);

	SPI1_CS = 1;
}

/* EOF */

