/* **************************************************************************
 File Name	: 	dev_flash.c
 Description:	EN673 - I2C device driver
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Date		:	14. 6. 18
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"


volatile UINT gnI2cPacket[2] = { 0, 0 };	// IRQ Stack을 줄이기 위해 Global로 선언


#if model_I2cSlave == 2
volatile int giI2cMasterOn = 0;

BYTE ISPMp I2c_mode(const BYTE abMasterOn)
{
	if(abMasterOn == 1 && giI2cMasterOn == 0) {	// Master Mode
#if 0
		while(I2C0_ACT || gnI2cPacket[0] || gnI2cPacket[1]);
#else
		if(I2C0_ACT) {
	#if STR_I2C_MODE == 1
			TxStrNoIRQ("I2C Master On Fail : I2C0_ACT", 0, 0);
	#endif
			/*return 1;*/while(I2C0_ACT);
		}

		if(gnI2cPacket[0] || gnI2cPacket[1]) {
	#if STR_I2C_MODE == 1
			TxStrNoIRQ("I2C Master On Fail : I2cPacket", 0, 0);
	#endif
			/*return 1;*/while(gnI2cPacket[0] || gnI2cPacket[1]);
		}
#endif

		I2C0_IRQ_EN = 0;
		giI2cMasterOn = 1;
		
		//I2cRstQue();

//		I2C0_CLK_DIV = I2C_CLKDIV_MASTER;
		I2C0_CLK_DIV = 0xFF;

		I2C0_MODE = 0;
	}

	if(abMasterOn == 0 && giI2cMasterOn == 1) {	// Slave mode
		I2C0_CLK_DIV = I2C_CLKDIV_SLAVE;

		//I2cRstQue();

		I2C0_MODE = 1;

		giI2cMasterOn = 0;
		I2C0_IRQ_EN = 1;
	}

	return 0;
}
#endif


#if model_I2cSlave == 1	|| model_I2cSlave == 2


TI2cQue gtI2cQue;

#if STR_I2C_DATA == 1
	volatile UINT gnIsrI2cTx  = 0;
	volatile UINT gnIsrI2cTxR = 0;
	volatile UINT gnIsrI2cRx  = 0;
	volatile UINT gnIsrI2cRxF = 0;
#endif

void I2cRstQue(void)
{
	gtI2cQue.wRxLen = 0;
	gtI2cQue.pbRxHead = &gtI2cQue.bRxQue[0];
	gtI2cQue.pbRxTail = &gtI2cQue.bRxQue[0];
}

void ISPM0 I2cTxDat(const WORD awData, const BYTE abDataPos)	// if abDataPos == 0, I2C0_TX_DAT is MSB
{
	switch(abDataPos)
	{
		case 1 : {
				switch(awData) {
					//case 0x280 : I2C0_TX_DAT = gbMnFcsAdjOn;								break;	//FOCUS ADJ
					case 0x281 : I2C0_TX_DAT = gbMnPvcCfg(MP(MpPvcZone))->bAction;		break;	//ZONE DISP
					case 0x282 : I2C0_TX_DAT = gbMnPvcCfg(MP(MpPvcZone))->bPosX;			break;	//H-POS
					case 0x283 : I2C0_TX_DAT = gbMnPvcCfg(MP(MpPvcZone))->bPosY;			break;	//V-POS
					case 0x284 : I2C0_TX_DAT = gbMnPvcCfg(MP(MpPvcZone))->bSizX;			break;	//H-SIZE
					case 0x285 : I2C0_TX_DAT = gbMnPvcCfg(MP(MpPvcZone))->bSizY;			break;	//V-SIZE
					case 0x286 : I2C0_TX_DAT = gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bAction;	break;	//ZONE DISP
					case 0x287 : I2C0_TX_DAT = gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bPosX;	break;	//H-POS
					case 0x288 : I2C0_TX_DAT = gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bPosY;	break;	//V-POS
					case 0x289 : I2C0_TX_DAT = gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bSizX;	break;	//H-SIZE
					case 0x28A : I2C0_TX_DAT = gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bSizY;	break;	//V-SIZE
					case 0x300 : I2C0_TX_DAT = model_Sens;								break;	//sensor : using at shutter speed
					//case 0x301 : I2C0_TX_DAT = MP(MpClockOnOff);							break;	//digital clock state(use/unuse)
					default : if(awData < 0x280) I2C0_TX_DAT = MP(awData);				break;
				}
			}
			break;
		default : I2C0_TX_DAT = 0x0; break;
	}
}

void ISPM0 I2cPacketDetector(const BYTE abData)
{
	BYTE bPacketOn = 0;

	#define bStx	((gnI2cPacket[0]>>8)&0xFF)
	#define bCmd	((gnI2cPacket[0]   )&0xFF)
	#define bEtx	abData

	if(bStx == I2C_STX && bEtx == I2C_ETX)
	{
		if(bCmd == I2C_CMD_MENUW || bCmd == I2C_CMD_MENUR)
		{
			bPacketOn = 1;
		}
	}

	// Default Byte Ordering is MSB(Most Significant Byte)
	gnI2cPacket[0] = ((gnI2cPacket[0]<<8)&0xFFFFFF00) | ((gnI2cPacket[1]>>24)&0xFF);
	gnI2cPacket[1] = ((gnI2cPacket[1]<<8)&0xFFFFFF00) | (abData&0xFF);
#if 0
	#define DispI2cX	(ISP_FONT_EAX-24-1)
	DispHex(33, DispI2cX   , gnI2cPacket[0], 8);
	DispHex(33, DispI2cX+8 , gnI2cPacket[1], 8);
	DispHex(34, DispI2cX   , *(((BYTE*)gnI2cPacket)+0), 2);
	DispHex(34, DispI2cX+2 , *(((BYTE*)gnI2cPacket)+1), 2);
	DispHex(34, DispI2cX+4 , *(((BYTE*)gnI2cPacket)+2), 2);
	DispHex(34, DispI2cX+6 , *(((BYTE*)gnI2cPacket)+3), 2);
	DispHex(34, DispI2cX+8 , *(((BYTE*)gnI2cPacket)+4), 2);
	DispHex(34, DispI2cX+10, *(((BYTE*)gnI2cPacket)+5), 2);
	DispHex(34, DispI2cX+12, *(((BYTE*)gnI2cPacket)+6), 2);
	DispHex(34, DispI2cX+14, *(((BYTE*)gnI2cPacket)+7), 2);
#endif

	if(bPacketOn)
	{
	#if 1
		QueIn(gtI2cQue, I2C_BUF_SIZE, ((BYTE*)gnI2cPacket)+1, I2C_ALL_SIZ,
			SID( gnIsrI2cRxF++; )
			//TxStr("UART RX FULL", bDat,4);
		)
	#endif
		gnI2cPacket[1] = 0;
		gnI2cPacket[0] = 0;
	}
}

void ISPM0 IsrI2c(void)		// CAUTION!!! - Isr stack size = 0x400, Do not use a lot of variables.
{
	/*
	Interrupt Process Time = 6us @ 81MHz
	
	Master Mode IRQ
		1. when a byte transfer action(R/W) was finished.
	
	Slave Mode IRQ
		1. when a byte was written by Master.
		2. when a Read command is arrived.
		3. when a byte was read by Master.
	*/

	I2C0_IRQ_CLR = 1;

#if model_I2cSlave == 2
	if(giI2cMasterOn || I2C0_MODE == 0) { I2C0_IRQ_EN = 0; return; }
#endif

	static WORD swDataBuf = 0;
	static BYTE sbDataPos = 0;

	switch(I2C0_SLV_RW)
	{
		case 0:		// Write
		{
			volatile BYTE bData = I2C0_RX_DAT;	//	Received Data from Master

			SID( gnIsrI2cRx++; TxStr("W", bData,2); )

			I2cPacketDetector(bData);

 			swDataBuf = ((swDataBuf<<8)&0xFF00) | (bData&0xFF);
			sbDataPos = 0;
			I2C0_SLV_GO = 1;		//	Release SCL holding, Master can put a next data
			break;
		}
		case 1:		// Read
		{
			if(I2C0_SLV_ACK_IN) {
				sbDataPos = 0;
				break;		//	Read Process End - Master wasn't acknowledged
			}
			else	// Read continue
			{
				I2cTxDat(swDataBuf, sbDataPos++);
				I2cPacketDetector(I2C0_TX_DAT);

				if(sbDataPos > 1) {
					//SID( TxStrDec2("MenuR ", swDataBuf, 4, I2C0_TX_DAT, 2); )

					//I2C0_SLV_ACK_OUT = 1;
					sbDataPos = 0;

					I2cPacketDetector(I2C_ETX);
				}

				SID( gnIsrI2cTx++; TxStr("R", I2C0_TX_DAT, 2); )

				I2C0_SLV_GO = 1;		//	Release SCL holding, Master can get a next data
				break;
			}
		}
	}
}

#endif

#if model_I2cSlave == 0 || model_I2cSlave == 2

BYTE gbEepromOff = 1;
BYTE gbEn331Off = 1;

//#if model_Aptn
#if (model_Aptn||model_Omni)

//	#if model_Pixel || model_Aptn || model_Omni

BYTE gbSensorOff = 1;
#endif

BYTE ISPMp i2c_write(BYTE dat, BYTE last, BYTE repeat)
{
	I2C0_TX_DAT = dat;
	I2C0_CONT_MST = (I2C0_CONT_MST&0xFFFF0000) | (repeat<<3) | ((repeat | last)<<2) | 3;
	while(I2C0_MST_GO);
	return I2C0_MST_ACK /*|| I2C0_MST_COL*/;
}

BYTE ISPMp i2c_read(BYTE last, BYTE repeat)
{
	I2C0_CONT_MST = (I2C0_CONT_MST&0xFFFF0000) | (repeat<<3) | ((repeat | last)<<2) | 1;
	while(I2C0_MST_GO);
	return I2C0_RX_DAT;
}

BYTE eep_write(UINT anAddr, BYTE abData)	// 201 us
{
	if(gbEepromOff) return EEP_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_WRITE_MASTER)
	
	while(i2c_write(EEPROM_DEVICE,0,0));	// Avg. 47 us (I2C_CLK = 290KHz)
	I2C_CODE(EEP_WRITE_NOACK1, i2c_write((anAddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_WRITE_NOACK2, i2c_write((anAddr&0xff), 0,0))

	I2C_CODE(EEP_WRITE_NOACK3, i2c_write(abData, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE eep_read(const WORD awAddr, volatile BYTE* abData, const UINT anDataEa)	// 248 us (anDataEa = 1, I2C_CLK = 290KHz)
{
	if(gbEepromOff) return EEP_READ_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_READ_MASTER)
	
	while(i2c_write(EEPROM_DEVICE,0,0));
	I2C_CODE(EEP_READ_NOACK1, i2c_write((awAddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_READ_NOACK2, i2c_write((awAddr&0xff), 0,1))

	I2C_CODE(EEP_READ_NOACK3, i2c_write(EEPROM_DEVICE | 0x1, 0,0))

	UINT i;
	for(i=1; i<anDataEa; i++)
	{
		*abData = i2c_read(0,0);
		abData++;
	}
	*abData = i2c_read(1,0);

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE EepPageWrite(const WORD awWaddr, BYTE* apbBuf, const UINT anBufEa)
{
	// 24LC64 Page Write
	// The master transmits up to 31 additional bytes which are temporarily stored in the on-chip page buffer
	//  and will be written into memory after the master has transmitted a stop condition.
	// After receipt of each word, the 5 lower address pointer bits are internally incremented by 1.
	// If the master should transmit more than 32 bytes prior to generating the stop condition,
	//  the address counter will roll over and the previously received data will be overwritten.
	//
	// If an attempt is made to write to the array with the WP pin held high,
	//  the device will acknowledge the command but no write cycle will occur,
	//  no data will be written and the device will immediately accept a new command.
	
	if(gbEepromOff) return EEP_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(EEP_WRITE_MASTER)
	
	while(i2c_write(EEPROM_DEVICE,0,0));
	I2C_CODE(EEP_WRITE_NOACK1, i2c_write((awWaddr&0x1f00)>>8, 0,0))
	I2C_CODE(EEP_WRITE_NOACK2, i2c_write((awWaddr&0xff), 0,0))

	UINT i;
	for(i=1; i<anBufEa; i++)
	{
		I2C_CODE(EEP_WRITE_NOACK3, i2c_write(*apbBuf, 0,0))
		apbBuf++;
	}

	I2C_CODE(EEP_WRITE_NOACK4, i2c_write(*apbBuf, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE ISPM TwiWrEep2( WORD awWaddr, volatile BYTE* apbBuf, UINT anBufEa)
{	// I2C EEPROM Page(32byte) Write Function 
	// CAUTION ! :
	//			1 page Tx per 2 frame , because EEP restart delay)
	//			Fuction recall delay = after (*apbBuf ea / TWI_EEP_BYTE_EA) frame 
	// apwWaddr : Start address
	// * apbBuf : input data p
	// anBufEa	: input data ea of byte

	static WORD gwWaddr;
	static BYTE k,l;
	static BYTE* gpbBuf;
	static BYTE gbTwiCnt;
		   BYTE bTwiCnt;
	
	WORD 		wRegAddrBuf;
	//BYTE 		bRegAddr[2];
	//BYTE 		i,t;

	if (gbTwiCnt%2)	{ gbTwiCnt++; return 0; }					// 1 page Tx per 2 frame
	else 			{ bTwiCnt = gbTwiCnt>>1; }
		
	if (!bTwiCnt) {											// Burst Tx Ready
		if (!anBufEa)	return 0;					
		else if ((awWaddr+anBufEa)>=(TWI_EEP_PAGE_EA*TWI_EEP_BYTE_EA)) {
			TxStrNoIRQ("WrEEP Size Over ! ", 0, 0); 
			return 0;
		}
		else {
			gwWaddr = awWaddr;
			gpbBuf  = (BYTE*)apbBuf;					
			k = anBufEa / TWI_EEP_BYTE_EA;
			l = anBufEa % TWI_EEP_BYTE_EA;
			
			gbTwiCnt++;			
			return 0;
		}
	}
	else if (bTwiCnt>=k+2) {								// Status reset at Tx end
		TxStrNoIRQ("TwiWrEep2 Done ! ", gwWaddr, 5);

		gwWaddr = 0;
		gpbBuf  = 0;
		k = 0;
		l = 0;
		gbTwiCnt = 0;								
		return 1;											// Tx end flag
	}
	else {
		wRegAddrBuf = (bTwiCnt-1) * TWI_EEP_BYTE_EA;

		EepPageWrite(gwWaddr + wRegAddrBuf, gpbBuf + wRegAddrBuf, (bTwiCnt<k+1) ? TWI_EEP_BYTE_EA : l);
		//TxStrDec2("EepPageWrite :", gwWaddr + wRegAddrBuf, 5, (bTwiCnt<k+1) ? TWI_EEP_BYTE_EA : l, 3);
	
		gbTwiCnt++;
		
		return 0;
	}
}

BYTE EN331_Write(WORD appWaddr, UINT anBuf)	// 422 us (I2C_CLK = 290KHz)
{
//		if(gbEn331Off) return EN331_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(EN331_WRITE_MASTER)

	//----------------------------------------------------------------------------------------
	// Tx Addr
	UINT nAddr = appWaddr<<2;
		
#if (USE_DIGITAL_INPUT==5)

	while(i2c_write(EN331A_DEVICE,0,0));		// Avg. 27 us (I2C_CLK = 290KHz)
	I2C_CODE(EN331_WRITE_NOACK1, i2c_write(0x0, 0,0))
	I2C_CODE(EN331_WRITE_NOACK2, i2c_write((nAddr>>8)&0xff, 1,0))

	while(i2c_write(EN331A_DEVICE,0,0));
	I2C_CODE(EN331_WRITE_NOACK3, i2c_write(nAddr&0xff, 0,0))

#else

	while(i2c_write(EN332_DEVICE,0,0));		// Avg. 27 us (I2C_CLK = 290KHz)
	I2C_CODE(EN331_WRITE_NOACK1, i2c_write(0x0, 0,0))
	I2C_CODE(EN331_WRITE_NOACK2, i2c_write((nAddr>>8)&0xff, 1,0))

	while(i2c_write(EN332_DEVICE,0,0));
	I2C_CODE(EN331_WRITE_NOACK3, i2c_write(nAddr&0xff, 0,0))
#endif	
	//----------------------------------------------------------------------------------------
	// Tx data
	I2C_CODE(EN331_WRITE_NOACK4, i2c_write(anBuf&0xff,       0,0))
	I2C_CODE(EN331_WRITE_NOACK5, i2c_write((anBuf>>8)&0xff,  0,0))
	I2C_CODE(EN331_WRITE_NOACK6, i2c_write((anBuf>>16)&0xff, 0,0))
	I2C_CODE(EN331_WRITE_NOACK7, i2c_write((anBuf>>24)&0xff, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
//		while(1); // for Test
}

BYTE EN331_Read(WORD appWaddr, UINT* anBuf)	// 449 us (I2C_CLK = 290KHz)
{
//		if(gbEn331Off) return EN331_READ_NODEV;
	I2C_STA

	I2C_MASTER_ON(EN331_READ_MASTER)

	//----------------------------------------------------------------------------------------
	// Tx Addr
	UINT nAddr = appWaddr<<2;
		
#if (USE_DIGITAL_INPUT==5)

	while(i2c_write(EN331A_DEVICE,0,0));
	I2C_CODE(EN331_READ_NOACK1, i2c_write(0x0, 0,0))
	I2C_CODE(EN331_READ_NOACK2, i2c_write((nAddr>>8)&0xff, 1,0))

	//WaitUs(70);
	
	while(i2c_write(EN331A_DEVICE,0,0));
	I2C_CODE(EN331_READ_NOACK3, i2c_write(nAddr&0xff, 0,1))

	//----------------------------------------------------------------------------------------
	// Rx data
	while(i2c_write(EN331A_DEVICE | 0x1,0,0));

#else
	while(i2c_write(EN332_DEVICE,0,0));
	I2C_CODE(EN331_READ_NOACK1, i2c_write(0x0, 0,0))
	I2C_CODE(EN331_READ_NOACK2, i2c_write((nAddr>>8)&0xff, 1,0))

	//WaitUs(70);
	
	while(i2c_write(EN332_DEVICE,0,0));
	I2C_CODE(EN331_READ_NOACK3, i2c_write(nAddr&0xff, 0,1))

	//----------------------------------------------------------------------------------------
	// Rx data
	while(i2c_write(EN332_DEVICE | 0x1,0,0));
#endif
	//I2C_CODE(EN331_READ_NOACK4, i2c_write(EN332_DEVICE | 0x1, 0,0))

	*anBuf = i2c_read(0,0);
	*anBuf |= (i2c_read(0,0)<<8);
	*anBuf |= (i2c_read(0,0)<<16);
	*anBuf |= (i2c_read(1,0)<<24);

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
}

BYTE PO2210_Write(BYTE appWaddr, BYTE abAddr, BYTE abBuf)	// 422 us (I2C_CLK = 290KHz)
{
#if 0 // Err Ignore
	i2c_write(appWaddr,0,0);
	i2c_write(abAddr, 0,0);
	i2c_write(abBuf, 1,0);

#else
	I2C_STA

	I2C_MASTER_ON(EN331_WRITE_MASTER)
	
	//----------------------------------------------------------------------------------------
	// Tx Dev Addr
	while(i2c_write(appWaddr,0,0));		// Avg. 27 us (I2C_CLK = 290KHz)
	
	//----------------------------------------------------------------------------------------
	// Tx Reg Addr
	I2C_CODE(PO2210_WRITE_NOACK1, i2c_write(abAddr, 0,0))
	
	//----------------------------------------------------------------------------------------
	// Tx Data
	I2C_CODE(PO2210_WRITE_NOACK2, i2c_write(abBuf, 1,0))

	I2C_MASTER_OFF()
	return EEP_NO_ERROR;

	I2C_END
#endif
}

#endif

#if model_Aptn
void ISPMp SetSensTwi_Aptn(BYTE abDevaddr, WORD awAddr, WORD abData)
{	// Write to Sensor

	if(gbSensorOff) return;// PIXEL_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(PIXEL_WRITE_MASTER)
	
	//while(i2c_write(abDevaddr,0,0));	// Avg. 47 us (I2C_CLK = 290KHz)
	i2c_write(abDevaddr,0,0);
	I2C_CODE(PIXEL_WRITE_NOACK2, i2c_write(awAddr>>8, 0,0))
	I2C_CODE(PIXEL_WRITE_NOACK2, i2c_write(awAddr   , 0,0))
	I2C_CODE(PIXEL_WRITE_NOACK3, i2c_write(abData>>8, 0,0))
	I2C_CODE(PIXEL_WRITE_NOACK3, i2c_write(abData   , 1,0))

	I2C_MASTER_OFF()
	return;
	
//	I2c_Err: I2c_mode(0);
	I2C_END
}

WORD ISPMp GetSensTwi_Aptn(BYTE abDevaddr, WORD awAddr)
{	// Read from Sensor

	if(gbSensorOff) return PIXEL_READ_NODEV;

	WORD wData = 0;

	I2C_STA

	I2C_MASTER_ON(PIXEL_READ_MASTER)
	
	//while(i2c_write(abDevaddr,0,0));
	i2c_write(abDevaddr,0,0);
	I2C_CODE(PIXEL_READ_NOACK2, i2c_write(awAddr>>8, 0,0))
	I2C_CODE(PIXEL_READ_NOACK2, i2c_write(awAddr   , 0,1))

	I2C_CODE(PIXEL_READ_NOACK3, i2c_write(abDevaddr | 0x1, 0,0))

	wData = i2c_read(0,0);
	wData = (wData<<8) | i2c_read(1,0);

	I2C_MASTER_OFF()
//	return EEP_NO_ERROR;
	return wData;

	I2C_END
}
	
#elif model_Omni
void ISPMp SetSensTwi_Omni(BYTE abDevaddr, WORD awAddr, BYTE abData)
{	// Write to Sensor

	if(gbSensorOff) return;// PIXEL_WRITE_NODEV;
	I2C_STA

	I2C_MASTER_ON(PIXEL_WRITE_MASTER)
	
//	while(i2c_write(abDevaddr,0,0));	// Avg. 47 us (I2C_CLK = 290KHz)
	i2c_write(abDevaddr,0,0);	// Avg. 47 us (I2C_CLK = 290KHz)
	I2C_CODE(PIXEL_WRITE_NOACK2, i2c_write(awAddr>>8, 0,0))
	I2C_CODE(PIXEL_WRITE_NOACK2, i2c_write(awAddr   , 0,0))
	I2C_CODE(PIXEL_WRITE_NOACK3, i2c_write(abData   , 1,0))

	I2C_MASTER_OFF()
	return;
	
//	I2c_Err: I2c_mode(0);
	I2C_END
}

BYTE ISPMp GetSensTwi_Omni(BYTE abDevaddr, WORD awAddr)
{	// Read from Sensor

	if(gbSensorOff) return PIXEL_READ_NODEV;

	BYTE bData = 0;

	I2C_STA

	I2C_MASTER_ON(PIXEL_READ_MASTER)
	
//	while(i2c_write(abDevaddr,0,0));
	i2c_write(abDevaddr,0,0);
	I2C_CODE(PIXEL_READ_NOACK2, i2c_write(awAddr>>8, 0,0))
	I2C_CODE(PIXEL_READ_NOACK2, i2c_write(awAddr   , 0,1))

	I2C_CODE(PIXEL_READ_NOACK3, i2c_write(abDevaddr | 0x1, 0,0))

	bData = i2c_read(1,0);
//	wData = (wData<<8) | i2c_read(1,0);

	I2C_MASTER_OFF()
//	return EEP_NO_ERROR;
	return bData;

	I2C_END
}
	
#elif model_Sony // 0011010 r/w
	
void SetSensTwi_Sony(BYTE abDevaddr, BYTE abAddr, BYTE abData)
{	// data set to sensor with ID, (Slave Addr 8b, Sub adress 16b, data 8b, LSB first, active low)


	I2C_STA

	I2C_MASTER_ON(SONY_WRITE_MASTER)
	
	I2C_CODE(SONY_WRITE_NOACK1, i2c_write(SENS_SONY_DEV,0,0))
	I2C_CODE(SONY_WRITE_NOACK2, i2c_write(abDevaddr,0,0))
	I2C_CODE(SONY_WRITE_NOACK3, i2c_write(abAddr,0,0))

	I2C_CODE(SONY_WRITE_NOACK4, i2c_write(abData,1,0))

	I2C_MASTER_OFF()
	return;

//		I2c_Err: I2c_mode(0);

	I2C_END
}
	
void SetSensTwiBurst_Sony(BYTE abDevaddr, BYTE abAddr, BYTE* abData, BYTE abLength)
{	// data set to sensor with ID, (Slave Addr 8b, Sub adress 16b, data 8b, LSB first, active low)

	UINT i=0;

	I2C_STA

	I2C_MASTER_ON(SONY_WRITE_MASTER)
	
	I2C_CODE(SONY_WRITE_NOACK1, i2c_write(SENS_SONY_DEV,0,0))
	I2C_CODE(SONY_WRITE_NOACK2, i2c_write(abDevaddr,0,0))
	I2C_CODE(SONY_WRITE_NOACK3, i2c_write(abAddr,0,0))
	
	for(i=0; i<abLength; i++) {
		I2C_CODE(SONY_WRITE_NOACK4, i2c_write(abData[i],(i==(UINT)(abLength-1)),0))
	}

	I2C_MASTER_OFF()
	return;

//		I2c_Err: I2c_mode(0);

	I2C_END
}
	
BYTE GetSensTwi_Sony(BYTE abDevaddr, BYTE abAddr)
{	// data get from sensor

	BYTE bData = 0;

	I2C_STA

	I2C_MASTER_ON(SONY_READ_MASTER)
	
	I2C_CODE(SONY_READ_NOACK1, i2c_write(SENS_SONY_DEV,0,0))
	I2C_CODE(SONY_READ_NOACK2, i2c_write(abDevaddr,0,0))
		
	I2C_CODE(SONY_READ_NOACK3, i2c_write(abAddr,0,1))
	I2C_CODE(SONY_READ_NOACK4, i2c_write(SENS_SONY_DEV|0x01,0,0))
	bData = i2c_read(1,0);

//		I2C_MASTER_OFF()
	return bData;

	I2C_END
}
	
#endif

#if 0
BYTE ISPMp i2c1_write(BYTE dat, BYTE last, BYTE repeat)
{
	I2C1_TX_DAT = dat;
	I2C1_CONT_MST = (I2C1_CONT_MST&0xFFFF0000) | (repeat<<3) | ((repeat | last)<<2) | 3;
	while(I2C1_MST_GO);
	return I2C1_MST_ACK /*|| I2C0_MST_COL*/;
}

BYTE ISPMp i2c1_read(BYTE last, BYTE repeat)
{
	I2C1_CONT_MST = (I2C1_CONT_MST&0xFFFF0000) | (repeat<<3) | ((repeat | last)<<2) | 1;
	while(I2C1_MST_GO);
	return I2C1_RX_DAT;
}

BYTE ISPMp i2c1_Write_Test(BYTE Address, WORD SubAddr, BYTE* bBuff, UINT nLength)
{
	UINT i=0;
	
	I2c1_Write(Address,0,0);
	I2c1_Write((BYTE)((SubAddr>>8)&0xFF),0,0);
	I2c1_Write((BYTE)((SubAddr>>0)&0xFF),0,0);

	for(i=0; i<(nLength-1); i++) I2c1_Write((BYTE)bBuff[i],0,0);
	I2c1_Write((BYTE)bBuff[i],1,0);

	return 0;
}
#endif
/* EOF */

