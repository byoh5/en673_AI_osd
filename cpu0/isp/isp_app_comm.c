/* **************************************************************************
 File Name	: 	app_comm.c
 Description:	Communication interface
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"	// to use fast code
#include "msg.h"

#define TXSTR_JTAG_OUT
//-------------------------------------------------------------------------------------------------
// Variables
BYTE	gbETX = 0;
BYTE	gbRxStg = 0;
WORD	gwRxAddr = 0;
UINT	gnRxData = 0;
BYTE	gbParOn = 0x0;

#define SetEnd() {\
		gbRxStg = 0;\
		gwRxAddr = 0;\
		gnRxData = 0;\
		gbETX = 0;\
	}

//BYTE gbWinChar = 0;													// Window character
union uFLOAT gnTxGrp[8] = { {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0} };	// Graphic data

UINT gnRxPar[32] = {
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,	// Parameter data (Regulate with logic address .CAUTION)
					0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
					};

//UINT gnDownOn = 0;										// PC_CMD_DOWNLOAD

UINT gnI2cOn = 0;


//-------------------------------------------------------------------------------------------------
#if 0
void ISPM SetPar(const BYTE abAddr, const UINT anData) { if(abAddr<32) gnRxPar[abAddr] = anData; }
UINT ISPM GetPar(const BYTE abAddr) { return (abAddr<32) ? gnRxPar[abAddr] : 0 ; }
void ISPM SetGrp(const BYTE abAddr, const UINT anData) { if(abAddr<8) gnTxGrp[abAddr].m_UINT = anData; }
UINT ISPM GetGrp(const BYTE abAddr) { return (abAddr<8) ? gnTxGrp[abAddr].m_UINT : 0 ; }
#endif

#if 0
int ISPM UartIsEmpty(void)
{
	int			iEmpty;

	DI;

	if(gtUartQue.wRxLen > 0)	iEmpty = 0;
	else						iEmpty = 1;

	EI;

	return (iEmpty);
}

BYTE ISPM UartGetByte(BYTE* apbErr)
{
	BYTE		bChar;

	DI;

	if(gtUartQue.wRxLen > 0){
		gtUartQue.wRxLen--;
		bChar = *gtUartQue.pbRxTail++;
		if( gtUartQue.pbRxTail == &gtUartQue.bRxQue[UART_BUF_SIZE]){
			gtUartQue.pbRxTail  = &gtUartQue.bRxQue[0];
		}
		*apbErr = UART_NO_QUE_ERR;
		//gtUartQue.wRxLen--;
	}
	else{
		bChar = 0;
		*apbErr = UART_RX_QUE_EMPTY;
	}

	EI;

	return (bChar);
}
#endif

#if STR_QUE_DATA == 1
/*volatile*/ UINT gnQueIn0 = 0;
/*volatile*/ UINT gnQueIn1 = 0;
/*volatile*/ UINT gnQueIn2 = 0;

/*volatile*/ UINT gnQueUse0 = 0;
/*volatile*/ UINT gnQueUse1 = 0;
/*volatile*/ UINT gnQueUse2 = 0;

///*volatile*/ UINT gnQueOut0 = 0;
///*volatile*/ UINT gnQueOut1 = 0;
///*volatile*/ UINT gnQueOut2 = 0;
#endif

#if model_I2cSlave == 1	|| model_I2cSlave == 2
BYTE ISPM0 QueInFnc(TI2cQue* aptQue, const UINT anQueSize, BYTE* apData, const UINT anDataSize)
{
	if((aptQue->wRxLen + anDataSize) >= anQueSize) return 1;		// Que Full

	UINT i;
	for(i=0; i<anDataSize; i++)
	{
	#if STR_QUE_DATA == 1
		gnQueIn2 = (gnQueIn2<<8) | ((gnQueIn1>>24)&0xFF);
		gnQueIn1 = (gnQueIn1<<8) | ((gnQueIn0>>24)&0xFF);
		gnQueIn0 = (gnQueIn0<<8) | *apData;
	#endif

		//aptQue->wRxLen++;
		*aptQue->pbRxHead++ = *apData++;
		if( aptQue->pbRxHead == &aptQue->bRxQue[anQueSize]) {
			aptQue->pbRxHead  = &aptQue->bRxQue[0];
		}
		aptQue->wRxLen++;
	}

	return 0;										// Que In Ok
}
#endif

BYTE ISPM QueOutFnc(TI2cQue* aptQue, const UINT anQueSize)
{
	if(aptQue->wRxLen == 0) return 0;				// Que No Data

	//aptQue->wRxLen--;
	const BYTE bIn = *aptQue->pbRxTail++;
	if( aptQue->pbRxTail == (aptQue->bRxQue+anQueSize) ) {
		aptQue->pbRxTail  = aptQue->bRxQue;
	}
	aptQue->wRxLen--;

	#if 0//STR_QUE_DATA == 1
		gnQueOut2 = (gnQueOut2<<8) | ((gnQueOut1>>24)&0xFF);
		gnQueOut1 = (gnQueOut1<<8) | ((gnQueOut0>>24)&0xFF);
		gnQueOut0 = (gnQueOut0<<8) | bIn;
	#endif

	return bIn;										// Que Out Ok
}


void TxStrNoIRQ(const char* apbStr0, UINT anVal, UINT anValLen)
{
#if defined(JTAG_DEBUG_PRINT) && defined(TXSTR_JTAG_OUT)
	if(anValLen) printf("%s%x\r\n",apbStr0,anVal);
	else		 printf("%s\r\n",apbStr0);
#else
	//while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
	if(gtUartQue.wTxLen || UART0_TX_IRQ_EN) {
		UINT nStNum;
		const UINT nSt = GetSt(&nStNum);
		SetStEnaIrq(nSt);
		while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
		SetSt(nSt,&nStNum);
	}

	char bStr[9];
	const UINT nValLen = MIN(8,anValLen);
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + nValLen + 2;

	if(nValLen) uint2strh(bStr, anVal, nValLen);

	Uart0_Tx(PC_STX);
	Uart0_Tx(PC_CMD_STR);
 	Uart0_Tx(bLen);

	if(apbStr0) Uart0_Str(apbStr0);
	if(nValLen) Uart0_Str(bStr);

	Uart0_Tx('\r');
	Uart0_Tx('\n');

	Uart0_Tx(PC_ETX);
#endif
}

void TxStrDecNoIRQ(const char* apbStr0, UINT anVal, UINT anValLen)
{
#if defined(JTAG_DEBUG_PRINT) && defined(TXSTR_JTAG_OUT)
	if(anValLen) printf("%s%d\r\n",apbStr0,anVal);
	else		 printf("%s\r\n",apbStr0);
#else
	//while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
	if(gtUartQue.wTxLen || UART0_TX_IRQ_EN) {
		UINT nStNum;
		const UINT nSt = GetSt(&nStNum);
		SetStEnaIrq(nSt);
		while(gtUartQue.wTxLen || UART0_TX_IRQ_EN) { NOP4 };
		SetSt(nSt,&nStNum);
	}

	char bStr[11];
	const UINT nValLen = MIN(10,anValLen);
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + nValLen + 2;

	if(nValLen) int2str(bStr, anVal, nValLen);

	Uart0_Tx(PC_STX);
	Uart0_Tx(PC_CMD_STR);
 	Uart0_Tx(bLen);

	if(apbStr0) Uart0_Str(apbStr0);
	if(nValLen) Uart0_Str(bStr);

	Uart0_Tx('\r');
	Uart0_Tx('\n');

	Uart0_Tx(PC_ETX);
#endif
}

void ISPM TxErr(void)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_ERR);
	UartTxByte(PC_ETX);
}

void ISPM TxAck(void)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_ACK);
	UartTxByte(PC_ETX);
}

void ISPM TxIni(void)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_INI);
	UartTxByte(PC_ETX);
}

void ISPM TxReg(UINT anAddr, UINT anData)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_REGR);

	UartTxByte(anAddr>> 8);
	UartTxByte(anAddr>> 0);

	UartTxByte(anData>>24);
	UartTxByte(anData>>16);
	UartTxByte(anData>> 8);
	UartTxByte(anData>> 0);

	UartTxByte(PC_ETX);
}

int  giTxGrpOn = 0;

void ISPM TxGrpFnc(void)
{
#ifdef ECM_REMOTE
   BYTE J_ShellCmdBuf[68];
   UINT *pJ_ShellCmdBuf = (UINT *)J_ShellCmdBuf;
   int i = 0;
   int end = 0;

   J_ShellCmdBuf[2] = PC_STX;
   J_ShellCmdBuf[3] = PC_CMD_GRP;

   for (i=0; i<GRP_NUMBER; i++) {
      pJ_ShellCmdBuf[i+1] = gnTxGrp[i].m_UINT;
   }

   end = 4 * (GRP_NUMBER + 1); // GRP(8) + pJ_ShellCmdBuf[0] 위치
   J_ShellCmdBuf[end] = PC_ETX; 

   DmaMemCpy_ip((BYTE *)gptMsgShare._ECM_REMOTE_Viewer, (BYTE *)&J_ShellCmdBuf[2], 36);
#else
	int		i;

	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_GRP);

	for(i=0; i<GRP_NUMBER; i++){
		UartTxByte(gnTxGrp[i].m_UINT>>24);
		UartTxByte(gnTxGrp[i].m_UINT>>16);
		UartTxByte(gnTxGrp[i].m_UINT>> 8);
		UartTxByte(gnTxGrp[i].m_UINT>> 0);
	}

	UartTxByte(PC_ETX);
#endif	
}

#if 0 //(model_Lens==1)	

void ISPM TxMafFnc(void)
{
	int		i;

	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_GRP);

	UartTxByte(PC_ETX);
}
	#define PC_CMD_TMCR 			0x42
	#define PC_CMD_TMCW 			0x32

	// System
	#define TMC_TABLEREAD			0x01
	#define TMC_TABLEWRITE			0x02 // Data 부분이 4Byte * 11 = 총 44Byte입니다.
	#define TMC_LENSINITIALIZATION	0x03 // Write Command로 보내며, Data는 4Byte로 TRUE(1)를 보냅니다.
	#define TMC_CALIBRATIONSTART	0x04 // Write Command로 보내며, Data는 4Byte로 TRUE(1)를 보냅니다.
	
	// Zoom
	#define TMC_ZOOM_TOTALSTEP		0x10
	#define TMC_ZOOM_STARTPOSITION	0x11
	#define TMC_ZOOM_ENDPOSITION	0x12
	#define TMC_ZOOM_SPEED			0x13
	#define TMC_ZOOM_MINRATIO		0x14
	#define TMC_ZOOM_MAXRATIO		0x15
	
	// Focus
	#define TMC_FOCUS_TOTALSTEP		0x20
	#define TMC_FOCUS_STARTPOSITION	0x21
	#define TMC_FOCUS_ENDPOSITION	0x22
	#define TMC_FOCUS_SPEED			0x23
	#define TMC_FOCUS_MODE			0x24
	#define TMC_FOCUS_DnNFILTER		0x25
#endif


void ISPM TxStr(const char* apbStr0, UINT anVal, UINT anValLen)
{
#if defined(JTAG_DEBUG_PRINT) && defined(TXSTR_JTAG_OUT)
	if(anValLen) printf("%s%x\r\n",apbStr0,anVal);
	else		 printf("%s\r\n",apbStr0);
#else
	char bStr[9];
	const UINT nValLen = MIN(8,anValLen);
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + nValLen + 2;

	if(nValLen) uint2strh(bStr, anVal, nValLen);

	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_STR);
 	UartTxByte(bLen);

	if(apbStr0) UartTxStr(apbStr0);
	if(nValLen) UartTxStr(bStr);

	UartTxByte('\r');
	UartTxByte('\n');

	UartTxByte(PC_ETX);
#endif
}

void ISPM TxStrDec(const char* apbStr0, UINT anVal, UINT anValLen)
{
#if defined(JTAG_DEBUG_PRINT) && defined(TXSTR_JTAG_OUT)
	if(anValLen) printf("%s%d\r\n",apbStr0,anVal);
	else		 printf("%s\r\n",apbStr0);
#else
	char bStr[11];
	const UINT nValLen = MIN(10,anValLen);
	const BYTE bLen = ((apbStr0) ? strlen(apbStr0) : 0) + nValLen + 2;

	if(nValLen) int2str(bStr, anVal, nValLen);

	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_STR);
 	UartTxByte(bLen);

	if(apbStr0) UartTxStr(apbStr0);
	if(nValLen) UartTxStr(bStr);

	UartTxByte('\r');
	UartTxByte('\n');

	UartTxByte(PC_ETX);
#endif
}

#if 0
void SetIsp(UINT anAddr, UINT anData)
{
	_wr32(ISP_BASE+(anAddr<<2), anData);
}

UINT GetIsp(UINT anAddr)
{
	return (_rd32(ISP_BASE+(anAddr<<2)));
}
#endif

void /*ISPMu*/ TxPar(const UINT anAddr, BYTE abData)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_PARR);					// Cmd

	UartTxByte(anAddr>> 8);						// Addr[15: 8]
	UartTxByte(anAddr>> 0);						// Addr[ 7: 0]

	UartTxByte(abData>> 0);						// Data[ 7: 0]

	UartTxByte(PC_ETX);							// ETX

// Debug ------------------------------------
#if 0
	#define DEBUGCNT_ADD	3
	static BYTE debugcnt=0;
	if(gbMnDebugFnc==8) {
		SETFONTID(31, debugcnt, ' ');
		debugcnt+=DEBUGCNT_ADD;
		if(debugcnt >= (ISP_FONT_EAX-DEBUGCNT_ADD-2)) { 
			debugcnt = 0;
			//DispClr(32, 0, (ISP_FONT_EAX-DEBUGCNT_ADD-2));
		}
		DispHex(32, debugcnt, anAddr/*abData*/,2);	SETFONTID(32, debugcnt+2, ' ');
		SETFONTID(31, debugcnt, 162);

		const UINT nData = (gbUsrParTbl[7]<<24) | (gbUsrParTbl[6]<<16) | (gbUsrParTbl[5]<<8) | gbUsrParTbl[4];
		DispHex(34, DispDbgX, nData , 8);
	}
#endif
}

void /*ISPMu*/ RxPar(const int aiWrite)
{
	if(aiWrite && (gbParOn == 0xFF)) {
		if(UP_START < gwRxAddr && gwRxAddr < UP_END) {
			gbUsrParTbl[gwRxAddr] = gnRxData;
			UsrParSet(gwRxAddr, 0);
		}
	}

	if(gbParOn != 0xFF) {
		TxPar(0xFFFF, 0);
	}
	else if(gwRxAddr == 0xFFFF) {
		TxPar(UP_END, gbUsrParTbl[UP_END]);
	}
	else if(gwRxAddr < USR_PAR_EA){
		TxPar(gwRxAddr, gbUsrParTbl[gwRxAddr]);
	}
	else {
		TxPar(gwRxAddr, 0);
	}
}

void /*ISPMu*/ TxParK(const BYTE abVal)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_PARK);					// Cmd
	UartTxByte(abVal);
	UartTxByte(PC_ETX);							// ETX
}

void /*ISPMu*/ RxParK(void)
{
	#define PW_CONTINUE		3

	if(gbParOn < PW_CONTINUE || gbParOn == 0xFF)
	{
		if(gnRxData == model_ParID) {
			gbParOn = 0xFF;
			TxParK(1);
			TxStr("Parameter Enable On ", 0, 0);
		}
		else {
			if(gbParOn == 0xFF) gbParOn = 0;
			gbParOn++;
			TxParK(0);
			TxStr("Parameter ID Error...", gbParOn, 1);
		}
	}

	if(gbParOn == PW_CONTINUE)
	{
		gbParOn++;

#if model_Par_Save
		gbUsrParTbl[UP_START]		= (BYTE)(((~UP_DATA_ID)>>8)&0xff);
		gbUsrParTbl[UP_END]			= (BYTE)(((~UP_DATA_ID))&0xff);
		gbUsrParSaveChk = 1;
		TxStr("Parameter Reset !!!", 0, 0);
#endif
	}
	else if(gbParOn > PW_CONTINUE && gbParOn != 0xFF)
	{
		TxParK(0);
	}
}

void UpReset(void)
{
	gbParOn = PW_CONTINUE;
	RxParK();
}

void /*ISPMu*/ TxEepDone(void)
{
	UartTxByte(PC_STX);
	UartTxByte(PC_CMD_EEPR);					// Cmd
	UartTxByte(PC_ETX);							// ETX
}

void /*ISPMu*/ TxParSave(void)
{
//	TimeChkStt();

//	#if model_Save 			// Save to internal flash
//		SfWrite2(gbUsrParTbl, USR_PAR_EA, FLASH_USER_MODE);
//	#else 					// Save to EEPROM
//		TwiWrEep2(TWI_EEP_USER_STT, gbUsrParTbl, USR_PAR_EA);
//	#endif

//	DispDatDec("Time____",	24, 24, TimeChkEnd());				// Calculate Processing time (usec)

#if model_Par_Save
	gbUsrParSaveChk = 1;		// EN778
#endif
	
	//DI();
	//AppSavePar();
	//EI();

	TxEepDone();
}

void /*ISPMu*/ TxParRead(void)
{
//	UINT i;
//
//	TimeChkStt();
//
//	#if model_Save
//		SfRead2(gbUsrParTbl, USR_PAR_EA, FLASH_USER_MODE);
//	#else
//		TwiRdEep(TWI_EEP_USER_STT, gbUsrParTbl, USR_PAR_EA);
//	#endif

////	DispDatDec("Time____",	24, 24, TimeChkEnd());				// Calculate Processing time (usec)
//
//	if(gbUsrParTbl[0]>2){				// if Flash is Empty
//
//		for (i = 0; i < USR_PAR_EA; i++){
//			gbUsrParTbl[i] = 0; 		// Set '0'
//		}
//
//		for (i = 0; i < gwTblParEa; i++){
//			gbUsrParTbl[gwTblPar[i][0]] = (BYTE)(gwTblPar[i][1]&0xff);
//		}
//
//		gbUsrParTbl[0] = 0;
//    }

#if model_Par_Save
	gbUsrParReadChk = 1;		// EN778
#endif

	//AppLoadPar();
	TxEepDone();
}

void ISPM RxAddr(const int aiWrite)
{
	const int iRxAddr = gwRxAddr;

	#define SET_ADDR(ADDR_START, ADDR_END, WRITE_CMD, READ_CMD, READ_DATA)	\
		if((int)(ADDR_START) <= iRxAddr && iRxAddr < (int)(ADDR_END)) {\
			const WORD wAddr = iRxAddr - (ADDR_START);\
			UNUSED_ARG(wAddr);\
			if(aiWrite) { WRITE_CMD; }\
			READ_CMD\
			TxReg(iRxAddr, (READ_DATA));}


	SET_ADDR(0x0, BASE_SENS,				// ISP : 0x0000 ~ 0x3fff
		// ISP Write Code
		SetIsp(wAddr, gnRxData);
		
		GRP3 = wAddr;
		GRP4 = gnRxData;
		GRP7 = GetIsp(wAddr);
		,
		// ISP Read Code
		,
		GetIsp(wAddr))

	SET_ADDR(BASE_SENS, BASE_SENS_OV,		// Sensor : 0x4000 ~ 0x47ff
		// Sensor Write Code
		//TxStrNoIRQ("SetSens:", wAddr, 2);
		//TxStrNoIRQ("Data:", gnRxData, 8);
	#if model_Omni
		SetSens(0, wAddr+0x3000, gnRxData);
	#elif model_Sony
		SetSens((SENS_CONTROL_MODE) ? SENS_SONY_ID2_TWI : SENS_SONY_ID2, wAddr, gnRxData);	
	#else
		SetSens(0, wAddr, gnRxData);
	#endif
	#if model_Aptn
		WaitUs(20000);
	#endif
		,
		// Sensor Read Code
		//TxStrNoIRQ("GetSens:", wAddr, 8);
		//TxStr("GetSens:", wAddr, 8);
		//const UINT nBuf = GetSens(wAddr);
		,
		  
//	#if model_Pana || model_Aptn || model_Pixel
	#if model_Pana || model_Aptn
		GetSens(wAddr)
	#elif model_Omni
		GetSens(wAddr+0x3000)
	#elif model_Sony
		GetSens((SENS_CONTROL_MODE) ? SENS_SONY_ID2_TWI : SENS_SONY_ID2, wAddr)		
	#else
		GetSens(0, wAddr)
	#endif
		/*nBuf*//*0x0*//* =Dummy */)

	SET_ADDR(BASE_SENS_OV, BASE_EN331,		// Sensor OV : 0x4800 ~ 0x4fff
		// Sensor Write Code
		,
		// Sensor Read Code
		,
		0x0/* =Dummy */)

	SET_ADDR(BASE_EN331, BASE_EEPROM,		// EN331 : 0x5000 ~ 0x5fff
		// Sensor Write Code
		EN331_Write(wAddr, gnRxData);
		,
		// Sensor Read Code
		UINT nBuf = 0;
		EN331_Read(wAddr, &nBuf);
		,
		nBuf)

	SET_ADDR(BASE_EEPROM, BASE_MICOM_PAR,	// EEPROM : 0x6000 ~ 0x7fff
		// Sensor Write Code
		eep_write(wAddr, gnRxData);
		,
		// Sensor Read Code
		BYTE bBuf = 0;
		eep_read(wAddr, &bBuf, 1);
		,
		bBuf)

	SET_ADDR(BASE_MICOM_PAR, BASE_MICOM_PAR + numberof(gnRxPar),	// Micom Para : 0x8000 ~ 0x8020
#if 1
		// Sensor Write Code
		//gnRxPar[wAddr] = (/*PL_STA < wAddr &&*/ wAddr < 0x1F/*PL_END*/) ? hex2dec(gnRxData) : gnRxData ;
		gnRxPar[wAddr] = hex2dec(gnRxData);
		,
		// Sensor Read Code
		//UINT nRxParOut = (/*PL_STA < wAddr &&*/ wAddr < 0x1F/*PL_END*/) ? dec2hex(gnRxPar[wAddr]) : gnRxPar[wAddr] ;
		UINT nRxParOut = dec2hex(gnRxPar[wAddr]);
		,
		nRxParOut)
#else
		// Sensor Write Code
		gnRxPar[wAddr] = gnRxData;
		,
		// Sensor Read Code
		,
		gnRxPar[wAddr])
#endif

}

void ISPM RxMenuWrite(void)
{
	if(gwRxAddr < 0x280)
	{
		MP(gwRxAddr) = (BYTE)(gnRxData&0xff);

		if(gwRxAddr == 0x1D/*MpLanguageSel*/)	// 0x1D값은 I2C Master와 같이 수정해야 함
		{
#if 1
			SetFontChg();
#else
			gbMenuFlash[gwRxAddr] = (BYTE)(gnRxData&0xff);
			OsdClearAll();

			FONT_ONw(0x0);
			POREG_ENw(0x1);
			WaitVdi();

			SetFontChg();

			WaitVdi();
			FONT_ONw(0x1);

			gbChgMenuMode = gbMenuMode;
#endif
		}
	}
	else
	{
		switch(gwRxAddr)
		{
			//case 0x280 :	gbMnFcsAdjOn = (BYTE)(gnRxData&0xff);								break;	//FOCUS ADJ
			case 0x281 :	gbMnPvcCfg(MP(MpPvcZone))->bAction = (BYTE)(gnRxData&0xff);			break;	//ZONE DISP
			case 0x282 :	gbMnPvcCfg(MP(MpPvcZone))->bPosX = (BYTE)(gnRxData&0xff);			break;	//H-POS
			case 0x283 :	gbMnPvcCfg(MP(MpPvcZone))->bPosY = (BYTE)(gnRxData&0xff);			break;	//V-POS
			case 0x284 :	gbMnPvcCfg(MP(MpPvcZone))->bSizX = (BYTE)(gnRxData&0xff);			break;	//H-SIZE
			case 0x285 :	gbMnPvcCfg(MP(MpPvcZone))->bSizY = (BYTE)(gnRxData&0xff);			break;	//V-SIZE
			case 0x286 :	gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bAction = (BYTE)(gnRxData&0xff);	break;	//WINDOW USE
			case 0x287 :	gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bPosX = (BYTE)(gnRxData&0xff);		break;	//DET H-POS
			case 0x288 :	gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bPosY = (BYTE)(gnRxData&0xff);		break;	//DET V-POS
			case 0x289 :	gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bSizX = (BYTE)(gnRxData&0xff);		break;	//DET H-SIZE
			case 0x28A :	gbMnImdCfg(0/*MP(MpItlWindowuse)*/)->bSizY = (BYTE)(gnRxData&0xff);		break;	//DET V-SIZE
			case 0x28B :	MenuReset();														break;	//RESET
#if 1
			default : break;
#else
			case 0x28C :	gbParSaveOn = 1;	ExitMenu();									break;	//MENU_EXIT
			case 0x28D :	//SHADING DET
				if(((BYTE)(gnRxData&0xff)) == 1)
				{
					gbMnShdDet = 1;
					gbMnShading = 0;
					gbMnOutFormat = MN_FOMT_1080;
					MenuOff();
				}
				break;
			case 0x28E :	//DEFECT DET
				if(((BYTE)(gnRxData&0xff)) == 1)
				{
					gbMnDefDet = 1;
					SaveMenuResetDef();
					MenuResetDef();
					InitMenuSize();
				}
				break;
			case 0x28F :	//DEFETCT DET_DEFECT DET
				if(((BYTE)(gnRxData&0xff)) == 1)
				{
					gbDefCnt = 4;
				}
				break;
			case 0x290 :
				if(((BYTE)(gnRxData&0xff)) == 1)
				{
					for(i=1; i<FLASH_REG_EA; i++) {
						gbMenuDef[i] = gbMenuFlash[i];
					}

					#if model_Save
						SfWrite(FLASH_SECT_DEF_STT*FLASH_SECT_SIZE, gbMenuDef, FLASH_REG_EA);
			    	#else
						I2cWrEep(I2C_EEP_DEF_STT, gbMenuDef, FLASH_REG_EA);
					#endif
				}
				break;
			case 0x291 :	gbParSaveOn = 1;	ExitMenu(); 									break;	//SETUP_EXIT
#endif
		}
	}
}

int ISPM RxPD(BYTE bIn)
{
	BYTE crc = 0;
	crc += (gwRxAddr&0xFF);
	crc += ((gnRxData>>24)&0xFF);	// Command 1
	crc += ((gnRxData>>16)&0xFF);	// Command 2 (Zoom & Pan & Tilt)
	crc += ((gnRxData>>8)&0xFF);	// Pan Speed
	crc += ((gnRxData>>0)&0xFF);	// Tilt Speed
	if(crc != bIn) return 1;

	//if(gwRxAddr != MF(CamID)) return 1;


	//switch((gnRxData>>16)&0xFF) {
	//	case 2    : KEY = 4; break;							// RIGHT
	//	case 4    : KEY = 8; break;							// LEFT
	//	case 8    : KEY = 1; break;							// UP
	//	case 0x10 : KEY = 2; break;							// DOWN
	//	default   : if((gnRxData>>16)&0xFFFF) KEY = 0x10;	// CENTER
	//}
	TxStr("Pelco-D ", (gnRxData>>16)&0xFFFF,8);

	return 0;
}

int ISPM RxPP(BYTE bIn)
{
	//asm volatile("l.nop");

	if(gbETX != PP_ETX) return 1;

	BYTE crc = 0x0F;
	crc ^= (gwRxAddr&0xFF);
	crc ^= ((gnRxData>>24)&0xFF);	// Command 1
	crc ^= ((gnRxData>>16)&0xFF);	// Command 2 (Zoom & Pan & Tilt)
	crc ^= ((gnRxData>>8)&0xFF);	// Pan Speed
	crc ^= ((gnRxData>>0)&0xFF);	// Tilt Speed
	if(crc != bIn) return 1;

	gwRxAddr++;

	if(gwRxAddr != MP(MpCamID)) return 1;


	switch((gnRxData>>16)&0xFF) {
		case 2    : gbPKeyVal = KEY_VAL_R; break;						// RIGHT
		case 4    : gbPKeyVal = KEY_VAL_L; break;						// LEFT
		case 8    : gbPKeyVal = KEY_VAL_U; break;						// UP
		case 0x10 : gbPKeyVal = KEY_VAL_D; break;						// DOWN
		default   : if((gnRxData>>16)&0xFFFF) gbPKeyVal = KEY_VAL_C;	// CENTER
	}
	//TxStr("Pelco-P ", (gnRxData>>16)&0xFFFF,8);

	return 0;
}

//-------------------------------------------------------------------------------------------------
#define Case1(CMD,VAL)\
	case (CMD) :\
		(VAL) = bIn;\
		gbRxStg++;\
		break

#define Case2(CMD,VAL)\
	case (CMD)   :\
	case (CMD)+1 :\
		(VAL) = ((VAL)<<8) | bIn;\
		gbRxStg++;\
		break

#define Case4(CMD,VAL)\
	case (CMD)   :\
	case (CMD)+1 :\
	Case2((CMD)+2,VAL)

#define SetCase0(C,E,EC, ...)			case C : if(E) { EC } else goto err_proc; SetEnd(); break
#define SetCase1(C,E,EC,D0,D0n)			Case##D0n(C,D0);  SetCase0(C+D0n,E,EC)
#define SetCase2(C,E,EC,D0,D0n,...)		Case##D0n(C,D0);  SetCase1(C+D0n,E,EC,__VA_ARGS__)
#define SetCase3(C,E,EC,D0,D0n,...)		Case##D0n(C,D0);  SetCase2(C+D0n,E,EC,__VA_ARGS__)

#define SetCase(CMD,EndChk,EndCode,VAL_NUM,...)	SetCase##VAL_NUM(CMD,EndChk,EndCode,__VA_ARGS__)


void ISPM Comm(void)
{
	UartBaudRate();

	DI;		// 없어도 동작함

	while(gtUartQue.wRxLen > 0 
	#if model_I2cSlave == 1 || model_I2cSlave == 2
		|| gtI2cQue.wRxLen > 0
	#endif
	) {
#if 1
		BYTE bIn = 0;

	#if model_I2cSlave == 1 || model_I2cSlave == 2
		if(gtI2cQue.wRxLen > 0) {
			gnI2cOn = 1;
			bIn = QueOut(gtI2cQue, I2C_BUF_SIZE );
		}
		else
	#endif
		{
			gnI2cOn = 0;

		  #if 0
			bIn = QueOut(gtUartQue, UART_BUF_SIZE );
		  #else
			bIn = *gtUartQue.pbRxTail++;
			if( gtUartQue.pbRxTail == (gtUartQue.bRxQue+UART_BUF_SIZE) ) {
				gtUartQue.pbRxTail  = gtUartQue.bRxQue;
			}
			gtUartQue.wRxLen--;
		  #endif

		#if STR_QUE_DATA == 1
			gnQueUse2 = (gnQueUse2<<8) | ((gnQueUse1>>24)&0xFF);
			gnQueUse1 = (gnQueUse1<<8) | ((gnQueUse0>>24)&0xFF);
			gnQueUse0 = (gnQueUse0<<8) | bIn;

		  #if 0		// CAUTION !!! Comm() 함수에서 OSD 사용 시 'EEPROM WRITE ALL' 시 멈추는 경우 발생
			#define DispUartX	(ISP_FONT_EAX-24-1)/*0*//*DispDbgX*/
			DispHex(29, DispUartX   , gnQueIn2, 8);
			DispHex(29, DispUartX+8 , gnQueIn1, 8);
			DispHex(29, DispUartX+16, gnQueIn0, 8);
			DispHex(30, DispUartX   , gnQueUse2, 8);
			DispHex(30, DispUartX+8 , gnQueUse1, 8);
			DispHex(30, DispUartX+16, gnQueUse0, 8);
			//SETFONTID(32, ISP_FONT_EAX-1, 0x1fe);
			//SETFONTID(31, ISP_FONT_EAX-1, 0x1fe);

			DispHex(28, DispUartX   , gnQueOut2, 8);
			DispHex(28, DispUartX+8 , gnQueOut1, 8);
			DispHex(28, DispUartX+16, gnQueOut0, 8);
		  #endif

		  #if 0
			if(gbDispPos>22) gbDispPos = 0;
			DispHex( 6+gbDispLine, DispUartX+gbDispPos, gnQueIn0, 2);	//  6~16
			DispHex(17+gbDispLine, DispUartX+gbDispPos,  gnQueUse0, 2);	// 17~27

			SetFontAttrs( 6+gbDispLine, DispUartX, 24, MN_WHITE);
			SetFontAttrs(17+gbDispLine, DispUartX, 24, MN_WHITE);

			gbDispPos+=2;
		  #endif
		#endif
		}
#else
		gtUartQue.wRxLen--;
		const BYTE bIn = *gtUartQue.pbRxTail++;
		if( gtUartQue.pbRxTail == &gtUartQue.bRxQue[UART_BUF_SIZE]){
			gtUartQue.pbRxTail  = &gtUartQue.bRxQue[0];
		}
		//gtUartQue.wRxLen--;
#endif

		//TxStrNoIRQ("UART Data:", bIn, 16);	// 사용 시 init과정에서 너무 많은 Rx데이터가 쌓여 있으면 문제가 될 수 있음

		switch(gbRxStg){
//...............................................
// Start -> Command
			case 0x00 :
				switch(bIn){
					case PC_STX : gbRxStg++; break;
					case PD_STX : gbRxStg = PD_STG; break;
					case PP_STX : gbRxStg = PP_STG; break;
				}
				break;

			case 0x01 :
				switch(bIn){
					case PC_CMD_REGW :
					case PC_CMD_REGR :
					case PC_CMD_KEY  :
					//case PC_CMD_CHA  :

#if model_I2cSlave == 1	|| model_I2cSlave == 2
					case I2C_CMD_MENUW :
					case I2C_CMD_MENUR :
					case I2C_CMD_CLOCK :
#endif

					case PC_CMD_AWBI :

					case PC_CMD_INI  :
					case PC_CMD_ACK  :

					case PC_CMD_PARW :
					case PC_CMD_PARR :
					case PC_CMD_EEPW :
					case PC_CMD_EEPR :
					case PC_CMD_PARK :

					case PC_CMD_DOWN :

					case PC_CMD_STR  :
						gbRxStg = bIn;
						break;

					default	:
						goto err_proc;
						break;
				}
				break;
//...............................................
// Pelco D
			SetCase(PD_STG, RxPD(bIn)==0,
					, 2, gwRxAddr,1, gnRxData,4);
//...............................................
// Pelco P
			SetCase(PP_STG, RxPP(bIn)==0,
					, 3, gwRxAddr,1, gnRxData,4, gbETX,1);
//...............................................
// Reg write process : ISP register set
			SetCase(PC_CMD_REGW, bIn==PC_ETX, if(gnI2cOn==0) { RxAddr(1); }
					, 2, gwRxAddr,2, gnRxData,4);
//...............................................
// Reg read process : ISP register get
			SetCase(PC_CMD_REGR, bIn==PC_ETX, RxAddr(0);
					, 1, gwRxAddr,2);

#if (model_Par==1)
//...............................................
// Paramter write process : Parameter set
			SetCase(PC_CMD_PARK, bIn==PC_ETX, if(gnI2cOn==0) { RxParK(); }
					, 1, gnRxData,4);

//...............................................
// Paramter write process : Parameter set
			SetCase(PC_CMD_PARW, bIn==PC_ETX, if(gnI2cOn==0) { RxPar(1); }
					, 2, gwRxAddr,2, gnRxData,1);
//...............................................
// Paramter read process : Parameter get
			SetCase(PC_CMD_PARR, bIn==PC_ETX, RxPar(0);
					, 1, gwRxAddr,2);

//...............................................
// Paramter write process : Save parameter to EEPROM(or flash)
			SetCase(PC_CMD_EEPW, bIn==PC_ETX, TxParSave();
					, 0);

//...............................................
// Paramter read process : Read parameter from EEPROM(or flash)
			SetCase(PC_CMD_EEPR, bIn==PC_ETX, TxParRead();
					, 0);
#endif
//...............................................
// Key process : Key from PC
			SetCase(PC_CMD_KEY, bIn==PC_ETX, gbUKeyVal = gnRxData; /*TxStr("UKeyVal ", gbUKeyVal,4);*/
					, 1, gnRxData,1);
					

#if model_I2cSlave == 1	|| model_I2cSlave == 2
//...............................................
// Menu write process : Menu data set
			SetCase(I2C_CMD_MENUW, bIn==PC_ETX,
					TxStrDec2("MenuW ", gwRxAddr, 4, gnRxData, 4);
					//TxStr("MenuW ", gnRxData, 8);
					RxMenuWrite();
					, 2, gwRxAddr,2, gnRxData,2);
//...............................................
// Menu read process : Menu data get
			SetCase(I2C_CMD_MENUR, bIn==PC_ETX,
					TxStrDec2("MenuR ", gwRxAddr, 4, gnRxData, 4);
					, 2, gwRxAddr,2, gnRxData,2);
#endif

//...............................................
// Download process : Download from PC
			SetCase(PC_CMD_DOWN, bIn==PC_ETX,
					if(gnI2cOn==0 && (_reset_s&FLASH_ADDR_MASK)) {
						/*gnDownOn = 1;*/ /*TxStr("DOWN ", 0,0);*/

						// UART IRQ Disable & Initialize
						UART0_TX_IRQ_EN = 0;
						UART0_RX_IRQ_EN = 0;
						gtUartQue.wTxLen = 0;
						gtUartQue.pbTxTail = gtUartQue.pbTxHead;

						// Write Flag
						SfWrEn;
						sfls_erase_sect(_reset_s - 1);
						_wr8(_reset_s - 1, 0x80);

						// System Reset
						SystemReset();
#if 0
						// Write Flag
//						sfls_erase_sect(_reset_s - 1);
						sfls_erase_sect(0x0100A000-1);	// for warning delete
						SFLS_WB_WR_EN = 1;
						_wr8(0x0100A000-1, 0x80);	// for warning delete

						// System Reset
						SYS_WDT_LMT = 500 * (MCLK/1000);	// 500ms
						SYS_SYS_WDTEN = 1;
						SYS_SYS_WDRST = 1;
						while(1);
#endif
					}
					, 0);
//...............................................
// Ini process : Uart que init
			SetCase(PC_CMD_INI, bIn==PC_ETX, UartRstQue();
					, 0);
//...............................................
// Character process : Character from PC
			/*SetCase(PC_CMD_CHA,  bIn==PC_ETX, gbWinChar = gnRxData;
					, 1, gnRxData,1);*/
//...............................................
// Ack process : Ack respond
			SetCase(PC_CMD_ACK, bIn==PC_ETX, TxAck();
					, 0);
//...............................................
// AWB ODM process : AWB Odm Initializing
			SetCase(PC_CMD_AWBI, bIn==PC_ETX, /*InitAwbOdmCap();*/ TxStr("AWB ODM ", 0,0); 
					, 0);
//...............................................
// Console process : Send CPU1
			case PC_CMD_STR:
				if(gptMsgShell.index != 0 && bIn==PC_ETX)
				{
					gptMsgShell.index = 0;
					num_loop(gptMsgShell.tail, gptMsgShell.tot_num);
					CPU_SHARED1 |= eIP1_SHELL_RX;
					CPU_IRQ1 = 1;
					SetEnd();
				}
				else
				{
					if(gptMsgShell.index == MSG_SHELL_MAXLEN)
					{
						TxStr("OverIn", bIn, 2);
						gptMsgShell.arg[gptMsgShell.tail][0]--;
					}
					else
					{
						gptMsgShell.arg[gptMsgShell.tail][gptMsgShell.index] = bIn;
						gptMsgShell.index++;
					}
				}
				break;
//...............................................
// Err process : Rx que clear, Error message send
			err_proc:
			default :
				UartRstRxQue();
				TxErr();
				gbRxStg = 0;
				break;
		}
	}

	EI;		// 없어도 동작함
}

/* EOF */

