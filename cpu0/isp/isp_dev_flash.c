/* **************************************************************************
 File Name	: 	dev_flash.c
 Description:	EN673 - Serial flash controller
 Designer	:	Jang, Young Kyu / Kim Sung-Hoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 08. 13
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

#define FLASH_EMPTY			0xFF
#define FLASH_VALID			0xFE
#define FLASH_INVALID		0xF0

/* Rolling addr seq. 

  			begin    erase 	write  end
  user1 : 	FE	______ FE  ________ F0
  user2 :	F0  ______ FF  ________ FE 

*/

// CAUTION ! WDT reset for long time burst proc

//*************************************************************************************************
// Base
//-------------------------------------------------------------------------------------------------
#define UART_STR(STR)	TxStrNoIRQ(STR, 0, 0)/*Uart0_Str(MSG)*/

BYTE gbSF_Status = 0;
BYTE gbSFLS_WB_RDCMD_IOM = 0;


//*************************************************************************************************
// Base
//-------------------------------------------------------------------------------------------------
BYTE ISPM SfWrite(UINT anAddr, BYTE* apbData, UINT anNum)
{	// Byte Write to Serial Flash
	// anAddr must be start point of sector
	UINT 	i;
	
	SfWrEn;
	
	for (i=0; i<anNum; i++) {
		if (((anAddr+i)&(FLASH_SECT_SIZE-1))==0) {										// Sector erase
			sfls_erase_sect(anAddr+i);
		}
		sfw8(anAddr+i, apbData[i]);
	}
	
	for (i=0; i<anNum; i++) {
		// Read & Verify
		if ( sfr8(anAddr+i) != apbData[i] ) {TxStr("SfWrErr_", i, 10); return 0;}
	}	
	return 1;
}

BYTE ISPM SfRead(UINT anAddr, BYTE* apbData, UINT anNum)
{	// Byte Read from Serial Flash

	UINT 	i;
	
	for (i=0; i<anNum; i++) apbData[i] = sfr8(anAddr+i);								// read

	SfWrDis;
	
	return 1;
}


//*************************************************************************************************
// Utillity
//-------------------------------------------------------------------------------------------------
BYTE ISPM Sf2Isp( UINT anSfaddr, UINT anIspAddr, UINT anIspBufEa, BYTE abSizeByte)
{	// Copy Flash to Isp Function 
	// anSfaddr  : Sf Start address
	// anIspAddr : Isp Start address
	// anIspBufEa: Isp data ea
	// abSizeByte: Data cast BYTE ea (2 or 4)
	
	UINT 	nBuf,i;	
	
	if (!anIspBufEa || !((abSizeByte==2)||(abSizeByte==4)) ) { TxStr("Sf2Isp arg error !",0,0); return 0;}

	for (i=0; i<anIspBufEa; i++) {
		nBuf = 		  (sfr8(anSfaddr+(i*abSizeByte)+0)<<0);
		nBuf = nBuf | (sfr8(anSfaddr+(i*abSizeByte)+1)<<8);
		if (abSizeByte==4) {
			nBuf = nBuf | (sfr8(anSfaddr+(i*abSizeByte)+2)<<16);
			nBuf = nBuf | (sfr8(anSfaddr+(i*abSizeByte)+3)<<24);
		}
		SetIsp(anIspAddr+i,nBuf);
//		TxStr("Shd Addr : ", anIspAddr+i, 10);		// verify data
//		TxStr("Shd Data : ", nBuf, 10);		// verify data
	}	
	return 1;	
}

BYTE ISPM SfAddrChk(BYTE abMode)
{	// Judgement Current address
	
	BYTE	bCurSect;

	BYTE 	bUser1chk;
	BYTE 	bUser2chk;

	if (abMode == FLASH_MENU_MODE){
		bUser1chk = sfr8(FLASH_SECT_MENU_STT*FLASH_SECT_SIZE);
		bUser2chk = sfr8(FLASH_SECT_MENU_BKUP*FLASH_SECT_SIZE);
	}
 	else { return 0; }
//	TxStrNoIRQ("user1___", bUser1chk, 2);
//	TxStrNoIRQ("user2___", bUser2chk, 2);

	if (bUser1chk==FLASH_VALID) {
		if (bUser2chk==FLASH_EMPTY)	{
			TxStrNoIRQ("user2Err", FLASH_EMPTY, 2);
		}
		else if (bUser2chk==FLASH_VALID) {				// Warning! Both valid case 
			TxStrNoIRQ("user2Err", FLASH_VALID, 2);
		}
		bCurSect = 0; 
	}
	else if (bUser2chk==FLASH_VALID) {
		if (bUser1chk==FLASH_EMPTY)	{
			TxStrNoIRQ("user1Err", FLASH_EMPTY, 2);
		}
		bCurSect = 1; 
	}
	else {
		TxStrNoIRQ("otherErr", 0x0, 2);
		bCurSect = 0;
	}		
	return 	bCurSect;
}


BYTE ISPM SfRead2(volatile BYTE* apbData, UINT anNum, BYTE abMode)
{	// Byte read from serial flash with Rolling address management
	// bMode : 0 - User, 1 - Parameter		
	UINT 	nCurAddr;
	BYTE 	bCurSect = 0;//SfAddrChk(abMode);

	if (abMode == FLASH_USER_MODE) {
		nCurAddr = (FLASH_SECT_USER_STT*FLASH_SECT_SIZE);
 	}
 	else if (abMode == FLASH_DATA_MODE) {
 		nCurAddr = (FLASH_SECT_DATA_STT*FLASH_SECT_SIZE);
 	}
	else {
//			nCurAddr = ((FLASH_SECT_MENU_STT+(bCurSect*FLASH_SECT_MENU_EA))*FLASH_SECT_SIZE);
		nCurAddr = ((FLASH_SECT_MENU_STT+(bCurSect*FLASH_SECT_MENU_EA))*FLASH_SECT_SIZE);
	}

//	UINT i;
//	for (i=0; i<anNum; i++) apbData[i] = sfr8(nCurAddr+4+i);								// read 1byte unit

	UINT	nBuf, i, j;
	const UINT nMok = anNum/4;
	const UINT nMod = anNum%4;
	
	for (i=0; i<anNum; i+=4) {														
//		if ((i == anNum-4) && (nMod != 0)) {		// NG
		if ((i == (nMok*4)) && (nMod != 0)) {										// exception
			for (j = 0; j < nMod; j++)
				apbData[i+j] = sfr8(nCurAddr+4+i+j);
		}
		else {																		// read 4byte unit // EN778
			// Default Byte Ordering is MSB(Most Significant Byte)
			nBuf = sfr32(nCurAddr+4+i);
			apbData[i+3] = (nBuf >>  0) & 0xff;
			apbData[i+2] = (nBuf >>  8) & 0xff;
			apbData[i+1] = (nBuf >> 16) & 0xff;
			apbData[i+0] = (nBuf >> 24) & 0xff;
		}
	}	
//	TxStr("ReadAddr", nCurAddr,  "\n");		// !!
	
	return 1;
}
	
void* DmaMemCpy_Invd_Dchach(void* dst, const void* src, int len)
{
	DmaMemCpy_isr((void*)dst, (void*)src, len);

	// D-Cache Invalidate (32Bytes Unit)
	const UINT nAddr = (UINT)dst;
	const UINT nStop = nAddr + ((len-1)/32+1)*32;

	invalidate_dcache_range(nAddr, nStop);

	return dst;
}

#define SF_DMA_Copy
#define TxStrTestDbg	TxStr //NoIRQ
BYTE ISPM SfWrite2(volatile BYTE* apbData, UINT anNum, BYTE abMode)
{	// Byte write to serial flash with Rolling address management
	// bMode : 0 - User, 1 - Parameter
	UINT	nNewAddr = 0;
	UINT 	nCurAddr = 0;
	BYTE	bCurSect = 0;//SfAddrChk(abMode);
//		BYTE	bCurSect = SfAddrChk(abMode);
	
	SfWrEn;
	
	if (anNum>FLASH_SECT_SIZE-1)	{TxStrNoIRQ("WSizeErr", 0 , 0); return 0;}	// temporary limitation

	if (abMode == FLASH_USER_MODE) {
		nNewAddr = (FLASH_SECT_USER_STT*FLASH_SECT_SIZE);
		return 0 ;	// 201597 - WHL : for DMA Test
	}
	else if (abMode == FLASH_DATA_MODE){
 		nNewAddr = (FLASH_SECT_DATA_STT*FLASH_SECT_SIZE);
		return 0 ;	// 201597 - WHL : for DMA Test
 	}
	else {
		if (bCurSect==0) {
			nCurAddr = (FLASH_SECT_MENU_STT*FLASH_SECT_SIZE);
			nNewAddr = (FLASH_SECT_MENU_BKUP*FLASH_SECT_SIZE);	
		}
		else {
			nCurAddr = (FLASH_SECT_MENU_BKUP*FLASH_SECT_SIZE);
			nNewAddr = (FLASH_SECT_MENU_STT*FLASH_SECT_SIZE);
		}
	}

#ifdef SF_DMA_Copy	// 2015822 - WHL: Flash Write by DMA Module
	UINT i=0;

	for (i=0; i<anNum; i++) {
		if (((nNewAddr+i)&(FLASH_SECT_SIZE-1))==0) {							// sector erase
			sfls_erase_sect(nNewAddr+i);
		}	 
//		DmaMemSet((BYTE*)(SFLS_BASE|(nNewAddr+4+i)), apbData[i], 1);	// 2015826 - WHL : 1Byte Unit DMA Copy
	}
	DmaMemCpy_Invd_Dchach((void*)(SFLS_BASE|(nNewAddr+4)), (void*)apbData, anNum);
#else
	UINT	nBuf, i, j;
	const UINT nMok = anNum/4;
	const UINT nMod = anNum%4;
	
	#if 0	// Write 1Byte Unit
	for (i=0; i<anNum; i++) {													
		
		if (((nNewAddr+i)&(FLASH_SECT_SIZE-1))==0) sfls_erase_sect(nNewAddr+i);	// sector erase
			
		_wr8((SFLS_BASE|(nNewAddr+4+i)), apbData[i]);
	}
	#else	// Write 4Byte Unit
	for (i=0; i<anNum; i+=4) {													
		
		if (((nNewAddr+i)&(FLASH_SECT_SIZE-1))==0) sfls_erase_sect(nNewAddr+i);	// sector erase
			
		if ((i == (nMok*4)) && (nMod != 0)) {									// exception
			for (j = 0; j < nMod; j++){
				sfw8(nNewAddr+4+i+j, apbData[i+j]);
//				TxStrTestDbg("i+j_CNT", i+j,  "\n");
			}
		}
		else {																	// write 4 byte unit
			// Default Byte Ordering is MSB(Most Significant Byte)
			nBuf = apbData[i+3] + (apbData[i+2]<<8) + (apbData[i+1]<<16) + (apbData[i+0]<<24);
			sfw32(nNewAddr+4+i, nBuf);
//			TxStrTestDbg("i___CNT", i,  "\n");
		}
	}
	#endif
#endif	
	for (i=0; i<anNum; i++) {													// read & verify
		if (sfr8(nNewAddr+4+i) != apbData[i]) {
			TxStrTestDbg("SfWrite2 Verify Err__", nNewAddr+4+i, 8);
//			TxStrTestDbg("Origin Val : ", apbData[i], 10);
//			TxStrTestDbg("Verify Val : ", sfr8(nNewAddr+4+i), 10);
			return 0;
		}
	}
	
//	TxStr("WrAddr", nNewAddr,  "\n");		// !!
	sfw8(nNewAddr, FLASH_VALID);												// set valid flag to new Writed area

	if (abMode == FLASH_MENU_MODE) {
		sfw8(nCurAddr, FLASH_INVALID);											// set invalid flag to old area 	
	}
	SfWrDis;
	
	#ifdef SF_DMA_Copy
		TxStrTestDbg("SfWrite2(DMA) Done ! ", 0,  0);
	#else
		TxStrTestDbg("SfWrite2(PNT) Done ! ", 0,  0);
	#endif
	
	return 1;	
}


/* EOF */

