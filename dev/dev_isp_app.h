/* **************************************************************************
 File Name	: 	dev_isp_app.h
 Description:	EN673 - application common header
 Designer	:	Kim, Sunghoon
 Modifier	:	Lee, Wanhee
 Date		:	14. 6. 18 / 15. 4. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
/*
 INDEX	:
		 0. APP
		 1. UTIL
		 2. INIT
		 3. KEY
		 4. COMM
		 5. MENU
		 6. AE
		 7. TDN
		 8. CONTRAST
		 9. MODE
		10. AWB
		11. DEFECT
		12. SHADING
		13. IMD
		14. ETC
		15. FOCUS
		16. CPU Common Processing
		17. DBG (Debugging & Testing)
*/

//*************************************************************************************************
// 0. APP
//-------------------------------------------------------------------------------------------------
extern const char *gsBuildDate;
extern WORD gDataID;
extern void InitDataID(void);

// MN_DATA_ID:MENU Reset, UP_DATA_ID:User Parameter Reset, UD_DATA_ID:User Data Reset
// gDataID:F/W 컴파일 및 다운로드 할 때마다 자동 Reset, model_ver & model_HexID:값변경 후 컴파일 및 다운로드 시 한번 Reset
#define MN_DATA_ID				gDataID/*model_ver*/
#define UP_DATA_ID				/*model_HexID*/gDataID/*model_ver*/	// release : model_HexID
#define UD_DATA_ID				gDataID/*model_ver*/

// User Parameter --------------------------------------------------

//#define USR_PAR_EA_AE		32
//#define USR_PAR_EA_AWB		32
#define USR_PAR_EA			(UP_END_REAL+1)/*444*//*512*//*256*//*128*/							// Byte total (max 4K-4)

#define UP_END 				UP_END_REAL//(USR_PAR_EA-1)					// Tail
//	#define UP(I)			gbUsrParTbl[I]

extern BYTE gbUsrParSaveChk;
extern BYTE gbUsrParReadChk;
extern BYTE gbUsrParTbl[USR_PAR_EA];


// User Data -------------------------------------------------------

#define USR_DATA_EA_AWBPRST	16
#define USR_DATA_EA_IRIS	8
#define USR_DATA_EA_SAF		2
#define USR_DATA_EA			64							// Byte total  (max 4K-4)

enum {
	UD_START 	= 0,									// Head

	UdAwbPrst	,
	UdIris		= UdAwbPrst+USR_DATA_EA_AWBPRST,
	UdSaf		= UdIris + USR_DATA_EA_IRIS,

	UD_END_REAL = UdSaf + USR_DATA_EA_SAF
};

#define UD_END 			(USR_DATA_EA-1)					// Tail
#define UD(I)			gbUsrDataTbl[I]


extern BYTE gbUsrDataSaveChk;
extern BYTE gbUsrDataReadChk;
extern BYTE gbUsrDataTbl[USR_DATA_EA];


#define UpPAR00		0
#define UpPAR01		0
#define UpPAR02		0
#define UpPAR03		0
#define UpPAR04		0
#define UpPAR05		0
#define UpPAR06		0
#define UpPAR07		0
#define UpPAR08		0
#define UpPAR09		0
#define UpPAR0A		0
#define UpPAR0B		0
#define UpPAR0C		0
#define UpPAR0D		0
#define UpPAR0E		0
#define UpPAR0F		0
#define UpPAR10		0
#define UpPAR11		0
#define UpPAR12		0
#define UpPAR13		0
#define UpPAR14		0
#define UpPAR15		0
#define UpPAR16		0
#define UpPAR17		0
#define UpPAR18		0
#define UpPAR19		0
#define UpPAR1A		0
#define UpPAR1B		0
#define UpPAR1C		0
#define UpPAR1D		0
#define UpPAR1E		0
#define UpPAR1F		0


// Common ----------------------------------------------------------
#if model_MenuDefSet == 1
	#define MENU_SET_PAR_READ_CHK	gbMnSetParReadChk = 1;
#else
	#define MENU_SET_PAR_READ_CHK
#endif


// Common ----------------------------------------------------------
#if model_Par_Save
#define	APP_READ_CHK_ALL 	gbMnParReadChk = 1;		\
							gbUsrParReadChk = 1;	\
							gbUsrDataReadChk = 1;	\
							MENU_SET_PAR_READ_CHK
#else
	#define	APP_READ_CHK_ALL 	gbMnParReadChk = 1;		\
									gbUsrDataReadChk = 1;	\
									MENU_SET_PAR_READ_CHK
#endif

extern void AppSavePar(void);
extern void AppLoadParNone(BYTE abSel);
extern void AppLoadPar(void);
extern void UsrParSet(const int aiAddr, const int aiInit);


//*************************************************************************************************
// 1. UTIL
//-------------------------------------------------------------------------------------------------
//extern UINT strlen(const char *apbStr);
extern void uchar2strb(char* apbStr, BYTE anVal, UINT anLen);
extern void uint2strh(char* apbStr, UINT anVal, UINT anLen);
extern void int2str(char* apbStr, int aiVal, UINT anLen);
extern void per2str(char* apbStr, const int aiVal, const int aiValMax, UINT anLen);

extern UINT hex2dec(const UINT hex);
extern UINT dec2hex(UINT dec);

extern void CheckByteOrdering(void);

extern void SetWaitTimeOffset(const UINT anMCLK, const UINT anCntPerSec);
extern void WaitUs(UINT anUs);

extern int 	LibUtlInterp1D (int aiXCur, int aiXPos1, int aiXPos2, int aiYPos1, int aiYPos2);
extern BYTE LibUtlKeyPass (const BYTE* abpPassKeys, BYTE abKeySize, BYTE* abpPassCnt);
extern int  LibUtlRand(void);

#define NOP4 { asm volatile("l.nop"); asm volatile("l.nop"); asm volatile("l.nop"); asm volatile("l.nop"); }


//*************************************************************************************************
// 2. INIT
//-------------------------------------------------------------------------------------------------
extern void InitFontChar(const UINT anFont[][12]);

extern void Debug_Init(void);
extern void Init(void);

extern UINT gnClkChgCnt;
extern void ISPM0 FpsClkSet(void/*const BYTE abSetOn*/);
extern void ISPM FreqAdjust(void);

#define UpINSEL 	INSELr
#define UpONSEL 	ONSELr
#define UpTRG_DIN 	TRG_DINr
#define UpTRG_HSI 	TRG_HSIr
#define UpTRG_VSI 	TRG_VSIr
#define UpPOL_HSI 	POL_HSIr
#define UpPOL_VSI 	POL_VSIr
#define UpLDI_POL 	LDI_POLr
#define UpLCK_DLY	LCK_DLYr
#define UpLDIDLY0	LDIDLY0r
#define UpLDIDLY1	LDIDLY1r
#define UpLDIDLY2	LDIDLY2r
#define UpLDIDLY3	LDIDLY3r
#define UpDCKO0_INV DCKO0_INVr
#define UpDCKO0_DLY DCKO0_DLYr
#define UpLDO0_SEL	LDO0_SELr
#define UpLDO1_SEL	LDO1_SELr
#define UpLDO2_SEL	LDO2_SELr
#define UpLDO3_SEL	LDO3_SELr


//*************************************************************************************************
// 3. KEY
//-------------------------------------------------------------------------------------------------
// Key bit field
typedef	union{			// EN773 & EN673 : Addr is the same, Bit is the opposite.
	UINT	nKey;
	struct {
		BYTE bSft;		// Addr 0, nKey[31:24]	- Shift buffer
		BYTE bOn;		// Addr 1, nKey[23:16]	- Key On/Off
		BYTE bCnt;		// Addr 2, nKey[15: 8]	- Repeat Counter
		BYTE bRpt;		// Addr 3, nKey[ 7: 0]	- Repeat Result (It is used in the OSD MENU)
	}v;	// Value
	struct {
		BYTE bSft : 8;	// Addr 0, nKey[31:24]	- bSft
		BYTE res0 : 2;	// Addr 1, nKey[23:22]	- bOn - for MENU PUSHING
		BYTE E    : 1;	// Addr 1, nKey[21]		-	"
		BYTE C    : 1;	// Addr 1, nKey[20]		-	"
		BYTE L    : 1;	// Addr 1, nKey[19]		-	"
		BYTE R    : 1;	// Addr 1, nKey[18]		-	"
		BYTE D    : 1;	// Addr 1, nKey[17]		-	"
		BYTE U    : 1;	// Addr 1, nKey[16]		-	"
		BYTE bCnt : 8;	// Addr 2, nKey[15:8]	- bCnt
		BYTE res1 : 2;	// Addr 3, nKey[7:6]	- bRpt - for MENU REPEAT
		BYTE Er   : 1;	// Addr 3, nKey[5]		-	"
		BYTE Cr   : 1;	// Addr 3, nKey[4]		-	"
		BYTE Lr   : 1;	// Addr 3, nKey[3]		-	"
		BYTE Rr   : 1;	// Addr 3, nKey[2]		-	"
		BYTE Dr   : 1;	// Addr 3, nKey[1]		-	"
		BYTE Ur   : 1;	// Addr 3, nKey[0]		-	"
	}b;	// Bit
} TKey;


//-------------------------------------------------------------------------------------------------
//
#ifndef __ISP_LIB__

#define UpADC_LEVEL_C 	DFSEL_TgtBd(0x9b , 0x4f , 0x49 )	// AD value at Push
#define UpADC_LEVEL_U 	DFSEL_TgtBd(0x12d, 0x9a , 0x95 )	// "
#define UpADC_LEVEL_D 	DFSEL_TgtBd(0x1ca, 0xe4 , 0xdc )	// "
#define UpADC_LEVEL_L 	DFSEL_TgtBd(0x265, 0x12e, 0x123)	// "
#define UpADC_LEVEL_R 	DFSEL_TgtBd(0x300, 0x179, 0x169)	// "

#define	UpAD_CHT_BIT	2/*0x3*//*0x07*/							// Chattering bit field
#define	AD_KEY_RPTF		(FPS_VDO>>2)						// First Repeat time
#define	AD_KEY_RPTT		(FPS_VDO>>3)						// Repeat time

#define	GPIO_CHT_BIT	4/*0x0f*/								// Chattering bit field
#define	GPIO_KEY_RPTF	(FPS_VDO>>2)						// First Repeat time
#define	GPIO_KEY_RPTT	(FPS_VDO>>3)						// Repeat time

#define	UART_HOLD_CNT	(2<<(FPS_VDO>30))					// Holding time
#define	UART_KEY_RPTF	(FPS_VDO>>2)						// First Repeat time
#define	UART_KEY_RPTT	(FPS_VDO>>3)						// Repeat time
#define	UART_KEY_RPTT2	(FPS_VDO>>4)						// Repeat time(Fast)	// 2016331 - WHL

#define PELCO_HOLD_CNT	(FPS_VDO>>1)						// minimum 15
#define PELCO_KEY_RPTF	(FPS_VDO)							//
#define PELCO_KEY_RPTT	(FPS_VDO)							//

#define PTZ_HOLD_CNT	1 //(FPS_VDO>>1)						// minimum 15
#define PTZ_KEY_RPTF	(FPS_VDO)							//
#define PTZ_KEY_RPTT	(FPS_VDO)							//

#define PUSH_DELAY_NOT	1					// No Holding time
#define PUSH_DELAY_MIN	(FPS_VDO)							// Min Holding time
#define PUSH_DELAY_NOR	(FPS_VDO+1)							// Max Holding time

#endif//__ISP_LIB__

#define KEY_VAL_U	1
#define KEY_VAL_D	2
#define KEY_VAL_R	4
#define KEY_VAL_L	8
#define KEY_VAL_C	16
#define KEY_VAL_E	32
#define KEY_VAL_S	64

#if 1
#define	KEY			gbMKey				// for MENU REPEAT
#define	KEY_U		((gbMKey   )&1)		//			"
#define	KEY_D		((gbMKey>>1)&1)		//			"
#define	KEY_R		((gbMKey>>2)&1)		//			"
#define	KEY_L		((gbMKey>>3)&1)		//			"
#define	KEY_C		((gbMKey>>4)&1)		//			"
#define	KEY_E		((gbMKey>>5)&1)		//			"

#define	pKEY		gbPKey				// for MENU PUSHING
#define	pKEY_U		((gbPKey   )&1)		//			"
#define	pKEY_D		((gbPKey>>1)&1)		//			"
#define	pKEY_R		((gbPKey>>2)&1)		//			"
#define	pKEY_L		((gbPKey>>3)&1)		//			"
#define	pKEY_C		((gbPKey>>4)&1)		//			"
#define	pKEY_E		((gbPKey>>5)&1)		//			"
#else
#define	KEY			(gtUKey.v.bRpt)		// for MENU REPEAT
#define	KEY_U		(gtUKey.b.Ur)		//			"
#define	KEY_D		(gtUKey.b.Dr)		//			"
#define	KEY_R		(gtUKey.b.Rr)		//			"
#define	KEY_L		(gtUKey.b.Lr)		//			"
#define	KEY_C		(gtUKey.b.Cr)		//			"
#define	KEY_E		(gtUKey.b.Er)		//			"

#define pKEY		(gtUKey.v.bOn)		// for MENU PUSHING, UART-Key used KeyHld(), ECM Key 1 click => 2 Frame (by UART_HOLD_CNT)
#define pKEY_U		(gtUKey.b.U)		//			"
#define pKEY_D		(gtUKey.b.D)		//			"
#define pKEY_R		(gtUKey.b.R)		//			"
#define pKEY_L		(gtUKey.b.L)		//			"
#define pKEY_C		(gtUKey.b.C)		//			"
#define pKEY_E		(gtUKey.b.E)		//			"
#endif

//-------------------------------------------------------------------------------------------------
// Extern
extern void KeyScan(void);
extern void KeyAccLevel(int *aiKeyAdd);

extern BYTE gbUKeyVal;	// Uart for ECM
extern TKey	gtUKey;

extern BYTE gbPKeyVal;	// PELCO
extern TKey	gtPKey;

extern BYTE	gbMKey;		// Menu Key
extern BYTE	gbPKey;

extern BYTE	gbSKeyVal;	// AD Key
extern TKey	gtSKey;


extern int giKeyAdd;

extern UINT gbPTZKeyVal;	// for Onvif PTZ



//*************************************************************************************************
// 4. COMM
//-------------------------------------------------------------------------------------------------
// Tx header
// Eyenix Protocol
#define	PC_STX		0x02			// STX

	//						gbRxStg :  0   1   CMD
	#define PC_CMD_REGW		0x10	// STX CMD ADH ADL DAH DA1 DA2 DAL ETX	: ISP Reg. Write
	#define PC_CMD_REGR		0x20	// STX CMD ADH ADL ETX					: ISP Reg. Read
	#define PC_CMD_KEY		0x30	// STX CMD KEY ETX						: KEY
	//#define PC_CMD_CHA		0x40	// STX CMD CHA ETX						: gbWinChar	- Not Used

	#define PC_CMD_PARW 	0x50 		// Cmd : Write parameter in F/W. 			2 addr, 1 data.
	#define PC_CMD_PARR 	0x58 		// Cmd : Read paramter in F/W. 				2 addr, 1 data.
	#define PC_CMD_EEPW 	0x60 		// Cmd : Write paramter from F/W to EEP.
	#define PC_CMD_EEPR 	0x61 		// Cmd : Read paramter from EEP to F/W.
	//#define PC_CMD_EEPD 	0x62 		// Cmd : Write / Read for EEPROM is done.
	#define PC_CMD_PARK		0x68		// Cmd : Enable Parameter					4 data

	//#define PC_CMD_SENW 	0x70 		// Cmd : Write register in Sensor. 			2 addr, 4 data
	//#define PC_CMD_SENR 	0x78 		// Cmd : Read register from Sensor.

#if model_I2cSlave == 1	|| model_I2cSlave == 2
	#define I2C_CMD_MENUW 	/*0x50*/	// STX CMD ADH ADL WDH WDL ETX : Menu parameter write
	#define I2C_CMD_MENUR	/*0x60*/	// STX CMD ADH ADL RDH RDL ETX : Menu parameter read
	#define I2C_CMD_CLOCK	/*0x70*/	// Cmd : Internet Standard Time
#endif

	#define PD_STG			0x80	// xFF ADD CMD CMD DAT DAT CRC			: Pelco-D
	#define PP_STG			0x90	// xA0 ADD DAT DAT DAT DAT xAF CRC		: Pelco-P

	#define PC_CMD_AWBI 	0xaa	// STX CMD ETX							: InitAwbOdmCap()

	#define	PC_CMD_STR		0xb0	// TxStr()
	#define	PC_CMD_GRP		0xc0	// TxGrp()
	#define	PC_CMD_ERR		0xd0	// TxErr()

	#define	PC_CMD_INI		0xe0	// STX CMD ETX							: UartRstQue()	- ?
	#define	PC_CMD_ACK		0xf0	// STX CMD ETX							: TxAck()		- ?

	#define PC_CMD_DOWN		0xf8	// STX CMD ETX							: Uart Download request Rx

#if (model_Lens==1)	
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

#define	PC_ETX		0x03			// ETX

// Pelco D Protocol
#define PD_STX		0xFF

// Pelco P Protocol
#define PP_STX		0xA0
#define PP_ETX		0xAF

// I2C Protocol
#define	I2C_STX		PC_STX
#define I2C_ETX		PC_ETX

#define I2C_STX_SIZ		1
#define I2C_CMD_SIZ		1
#define I2C_ADR_SIZ		2
#define I2C_DAT_SIZ		2
#define I2C_ETX_SIZ		1

#define I2C_ALL_SIZ		(I2C_STX_SIZ+I2C_CMD_SIZ+I2C_ADR_SIZ+I2C_DAT_SIZ+I2C_ETX_SIZ)

//-------------------------------------------------------------------------------------------------
// Snatch logic address (Regulate with logic address .CAUTION)
#define	BASE_SENS			0x4000		// 0x300 ea 		sensor control
#define BASE_SENS_OV		0x4800
#define BASE_EN331			0x5000
#define BASE_EEPROM			0x6000
#define BASE_MICOM_PAR		0x8000

//-------------------------------------------------------------------------------------------------
// Debug Graph channel
union uFLOAT{
	float	m_float;
	UINT	m_UINT;
};

#define	GRP0			(gnTxGrp[0].m_UINT)
#define	GRP1			(gnTxGrp[1].m_UINT)
#define	GRP2			(gnTxGrp[2].m_UINT)
#define	GRP3			(gnTxGrp[3].m_UINT)
#define	GRP4			(gnTxGrp[4].m_UINT)
#define	GRP5			(gnTxGrp[5].m_UINT)
#define	GRP6			(gnTxGrp[6].m_UINT)
#define	GRP7			(gnTxGrp[7].m_UINT)

#define	GRP0F			(gnTxGrp[0].m_float)
#define	GRP1F			(gnTxGrp[1].m_float)
#define	GRP2F			(gnTxGrp[2].m_float)
#define	GRP3F			(gnTxGrp[3].m_float)
#define	GRP4F			(gnTxGrp[4].m_float)
#define	GRP5F			(gnTxGrp[5].m_float)
#define	GRP6F			(gnTxGrp[6].m_float)
#define	GRP7F			(gnTxGrp[7].m_float)

//-------------------------------------------------------------------------------------------------
// Debug Parameter channel
#define	PAR00			(gnRxPar[ 0])
#define	PAR01			(gnRxPar[ 1])
#define	PAR02			(gnRxPar[ 2])
#define	PAR03			(gnRxPar[ 3])
#define	PAR04			(gnRxPar[ 4])
#define	PAR05			(gnRxPar[ 5])
#define	PAR06			(gnRxPar[ 6])
#define	PAR07			(gnRxPar[ 7])
#define	PAR08			(gnRxPar[ 8])
#define	PAR09			(gnRxPar[ 9])
#define	PAR0A			(gnRxPar[10])
#define	PAR0B			(gnRxPar[11])
#define	PAR0C			(gnRxPar[12])
#define	PAR0D			(gnRxPar[13])
#define	PAR0E			(gnRxPar[14])
#define	PAR0F			(gnRxPar[15])
#define	PAR10			(gnRxPar[16])
#define	PAR11			(gnRxPar[17])
#define	PAR12			(gnRxPar[18])
#define	PAR13			(gnRxPar[19])
#define	PAR14			(gnRxPar[20])
#define	PAR15			(gnRxPar[21])
#define	PAR16			(gnRxPar[22])
#define	PAR17			(gnRxPar[23])
#define	PAR18			(gnRxPar[24])
#define	PAR19			(gnRxPar[25])
#define	PAR1A			(gnRxPar[26])
#define	PAR1B			(gnRxPar[27])
#define	PAR1C			(gnRxPar[28])
#define	PAR1D			(gnRxPar[29])
#define	PAR1E			(gnRxPar[30])
#define	PAR1F			(gnRxPar[31])


//-------------------------------------------------------------------------------------------------
// Extern
extern void Comm(void);

#define TxGrp()		giTxGrpOn = 1
#define TxGrpRun()	if(giTxGrpOn == 1) { TxGrpFnc(); giTxGrpOn = 0; }
extern void TxGrpFnc(void);

extern void TxStr(const char* apbStr0, UINT anVal, UINT anValLen);
extern void TxStrDec(const char* apbStr0, UINT anVal, UINT anValLen);
extern void TxStrDec2(const char* apbStr0, UINT anVal0, UINT anValLen0, UINT anVal1, UINT anValLen1);
extern void TxStrNoIRQ(const char* apbStr0, UINT anVal, UINT anValLen);
extern void TxStrDecNoIRQ(const char* apbStr0, UINT anVal, UINT anValLen);

extern UINT	gnRxPar[32];
extern union uFLOAT gnTxGrp[8];
extern int  giTxGrpOn;

#if 0
extern void SetPar(const BYTE abAddr, const UINT anData);
extern UINT GetPar(const BYTE abAddr);
extern void SetGrp(const BYTE abAddr, const UINT anData);
extern UINT GetGrp(const BYTE abAddr);
#endif

extern BYTE QueInFnc(TI2cQue* aptQue, const UINT anQueSize, BYTE* apData, const UINT anDataSize);
extern BYTE QueOutFnc(TI2cQue* aptQue, const UINT anQueSize);

#define QueOut(QUE,SIZE)	QueOutFnc((TI2cQue*)(&(QUE)), SIZE)

#define QueIn(QUE,QUE_SIZE,DATA,DATA_SIZE,FAIL_CODE)	\
			if((QUE.wRxLen + (DATA_SIZE))  < (QUE_SIZE)) {\
				QueInFnc((TI2cQue*)(&(QUE)), QUE_SIZE, (BYTE*)(DATA), DATA_SIZE);\
			} else { FAIL_CODE; }


//*************************************************************************************************
// 5. MENU
//-------------------------------------------------------------------------------------------------
extern void MsgMenuSet(void); // 180319 KDH isp_app_main
// Misc
#define MN_PAR_EA			512								// Menu parameter ea (user fix)
#define MN_LNG_EA			5								// Language max ea (1 ~ 12) (user fix)


#define	MN_TITLE			1								// title y
#define	MN_YSTART			(MN_TITLE+2)					// 


extern BYTE MN_MXSP;

#define	MN_MXSP_DEF			14/*0x1*/	// 160330 KSH		// main osd x start position
#define	MN_SXSP				(MN_MXSP+12)					// sub osd x start position
#define MN_XBAR				(MN_SXSP+3)						// sub Bar osd start position
#define MN_BAR_SIZE			7								// Bar Size, 7 or 5

#define MN_XCENT        	(MN_MXSP+11)					// Osd center position
#define MN_XTITL			(MN_MXSP+8/*11*/)

#define MN_SUBSIZE      	11/*10*/	// 160330 KSH		// Sub osd ea
#define MN_LINESIZE     	(MN_SXSP+MN_SUBSIZE-MN_MXSP)	// X ea


#define	MN_BLACK			0x0								// Attribute
#define	MN_WHITE			0x1								// "
#define MN_GRAY				0x2								// "
#define	MN_YELLOW			0x3								// "


#if model_1M
	#define MN_POS_SIZ_720P		{\
							BOAD_ONw(0x1); BOAD_MODEw(0x0);		\
							FONT_OFYw(0x67/*0x87*/); FONT_OFXw(0xd0/*0x1c0*/);  \
							FONT_VBLKw(0x88); FONT_HBLKw(0x88/*0x84*/); \
							FONT_VSIZw(0x1); FONT_HSIZw(0x1);}
	#define MN_POS_SIZ_SMALL	{\
							BOAD_ONw(0x1); BOAD_MODEw(0x0);		\
							FONT_OFYw(0x30); FONT_OFXw(0xd0/*0x50*/);   \
							FONT_VBLKw(0); FONT_HBLKw(0); \
							FONT_VSIZw(0x1); FONT_HSIZw(0x1);}
#else// model_2M
	#define MN_POS_SIZ_LARGE	{\
							BOAD_ONw(0x1); BOAD_MODEw(0x1);		\
							FONT_OFYw(0x40); FONT_OFXw(0x120);	\
							FONT_VBLKw(0x88); FONT_HBLKw(0x84);	\
							FONT_VSIZw(0x2); FONT_HSIZw(0x2);}

	#define MN_POS_SIZ_720P_CROP 	{\
							BOAD_ONw(0x1); BOAD_MODEw(0x1);		\
							FONT_OFYw(0x20/*0x8a*/); FONT_OFXw(0x70/*0x1c4*/);	\
							FONT_VBLKw(0x88); FONT_HBLKw(0x88);	\
							FONT_VSIZw(0x1); FONT_HSIZw(0x1);}

	#define MN_POS_SIZ_SMALL	{\
							BOAD_ONw(0x1); BOAD_MODEw(0x0);		\
							FONT_OFYw(0x25); FONT_OFXw(0xe0);     \
							FONT_VBLKw(0); FONT_HBLKw(0); \
							FONT_VSIZw(0x1); FONT_HSIZw(0x1);}
#endif


#define MENU_PK_SETUP		{KEY_VAL_L, KEY_VAL_L, KEY_VAL_R, KEY_VAL_R, KEY_VAL_C}		// Setup Menu pass Key sequence
#define MENU_PK_DEBUG 		{KEY_VAL_U, KEY_VAL_D, KEY_VAL_L, KEY_VAL_R, KEY_VAL_C}     // Debug Menu pass Key sequence

#define	MN_AUTO_OFF			0						// Auto menu off (0:Disable)
#define MN_OFF_TIME			30						// off after (sec)


#define FONT_SEL0							  gnFontChar0
#define FONT_SEL1	MP(MpLanguageSel) == 1  ? gnFontChar1 : FONT_SEL0
#define FONT_SEL2	MP(MpLanguageSel) == 2  ? gnFontChar2 : FONT_SEL1
#define FONT_SEL3	MP(MpLanguageSel) == 3  ? gnFontChar3 : FONT_SEL2
#define FONT_SEL4	MP(MpLanguageSel) == 4  ? gnFontChar4 : FONT_SEL3
#define FONT_SEL5	MP(MpLanguageSel) == 5  ? gnFontChar5 : FONT_SEL4
#define FONT_SEL6	MP(MpLanguageSel) == 6  ? gnFontChar6 : FONT_SEL5
#define FONT_SEL7	MP(MpLanguageSel) == 7  ? gnFontChar7 : FONT_SEL6
#define FONT_SEL8	MP(MpLanguageSel) == 8  ? gnFontChar8 : FONT_SEL7
#define FONT_SEL9	MP(MpLanguageSel) == 9  ? gnFontChar9 : FONT_SEL8
#define FONT_SELA	MP(MpLanguageSel) == 10 ? gnFontCharA : FONT_SEL9
#define FONT_SELB	MP(MpLanguageSel) == 11 ? gnFontCharB : FONT_SELA

#define EXTERN_FONT_CHAR0						extern const UINT gnFontChar0[][12]
#define EXTERN_FONT_CHAR1	EXTERN_FONT_CHAR0;	extern const UINT gnFontChar1[][12]
#define EXTERN_FONT_CHAR2	EXTERN_FONT_CHAR1;	extern const UINT gnFontChar2[][12]
#define EXTERN_FONT_CHAR3	EXTERN_FONT_CHAR2;	extern const UINT gnFontChar3[][12]
#define EXTERN_FONT_CHAR4	EXTERN_FONT_CHAR3;	extern const UINT gnFontChar4[][12]
#define EXTERN_FONT_CHAR5	EXTERN_FONT_CHAR4;	extern const UINT gnFontChar5[][12]
#define EXTERN_FONT_CHAR6	EXTERN_FONT_CHAR5;	extern const UINT gnFontChar6[][12]
#define EXTERN_FONT_CHAR7	EXTERN_FONT_CHAR6;	extern const UINT gnFontChar7[][12]
#define EXTERN_FONT_CHAR8	EXTERN_FONT_CHAR7;	extern const UINT gnFontChar8[][12]
#define EXTERN_FONT_CHAR9	EXTERN_FONT_CHAR8;	extern const UINT gnFontChar9[][12]
#define EXTERN_FONT_CHARA	EXTERN_FONT_CHAR9;	extern const UINT gnFontCharA[][12]
#define EXTERN_FONT_CHARB	EXTERN_FONT_CHARA;	extern const UINT gnFontCharB[][12]

#if   MN_LNG_EA == 1
	#define SetFontChg() InitFontChar(FONT_SEL0);
	EXTERN_FONT_CHAR0;
#elif MN_LNG_EA == 2
	#define SetFontChg() InitFontChar(FONT_SEL1);
	EXTERN_FONT_CHAR1;
#elif MN_LNG_EA == 3
	#define SetFontChg() InitFontChar(FONT_SEL2);
	EXTERN_FONT_CHAR2;
#elif MN_LNG_EA == 4
	#define SetFontChg() InitFontChar(FONT_SEL3);
	EXTERN_FONT_CHAR3;
#elif MN_LNG_EA == 5
	#define SetFontChg() InitFontChar(FONT_SEL4);
	EXTERN_FONT_CHAR4;
#elif MN_LNG_EA == 6
	#define SetFontChg() InitFontChar(FONT_SEL5);
	EXTERN_FONT_CHAR5;
#elif MN_LNG_EA == 7
	#define SetFontChg() InitFontChar(FONT_SEL6);
	EXTERN_FONT_CHAR6;
#elif MN_LNG_EA == 8
	#define SetFontChg() InitFontChar(FONT_SEL7);
	EXTERN_FONT_CHAR7;
#elif MN_LNG_EA == 9
	#define SetFontChg() InitFontChar(FONT_SEL8);
	EXTERN_FONT_CHAR8;
#elif MN_LNG_EA == 10
	#define SetFontChg() InitFontChar(FONT_SEL9);
	EXTERN_FONT_CHAR9;
#elif MN_LNG_EA == 11
	#define SetFontChg() InitFontChar(FONT_SELA);
	EXTERN_FONT_CHARA;
#elif MN_LNG_EA == 12
	#define SetFontChg() InitFontChar(FONT_SELB);
	EXTERN_FONT_CHARB;
#endif


// -------------------------------------------------------------------------------------------------
// Menu parameter assign
#ifndef __ISP_LIB__

#define MP_END 	(MN_PAR_EA-1)	// Tail

#define gbMnPvcCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpPvcCfgIdx +((INDEX))*5))
#define gbMnPlgCfg(INDEX)		((_POLYGON*)(gbMnParTbl + MpPlgCfgIdx +((INDEX))*9))

#define gbMnImdCfg(INDEX)		((_PRIVACY*)(gbMnParTbl + MpItlWinCfgIdx +((INDEX))*5))

#define gbMnROIWdrCfg(INDEX)	((_ROIWDR*)(gbMnParTbl + MpROIWdrCfgIdx + ((INDEX))*9))

#define gbCamTitle(INDEX)		gbMnParTbl[MpCamTitleIdx+((INDEX))]

//#define MP(I)					gbMnParTbl[I]
#define MP_LIB(I)				{ extern BYTE *gpb##I; gpb##I = (BYTE*)&MP(I); }

#else//__ISP_LIB__

	#define MP(I)	(*gpb##I)
	
	#ifdef LINK_MENU
		#define MP_LIB(I)	BYTE *gpb##I
	#else
		#define MP_LIB(I)	extern BYTE *gpb##I
	#endif

#endif//__ISP_LIB__

#define MP_LIB_LIST_ADV_AE	\
	MP_LIB(MpSpotSpd);\
	MP_LIB(MpStabilizing);\
	MP_LIB(MpErrChtMax);\
	MP_LIB(MpErrChtMin);\
	MP_LIB(MpLSpotLv);\
	MP_LIB(MpSatBrt);\
	MP_LIB(MpLSpotTh);\
	MP_LIB(MpMinGammaY);\
	MP_LIB(MpSmallSpeed);


#define MENU_LIB_LIST	\
	MP_LIB(MpSysFreq);\
	MP_LIB(MpOutFps);\
	MP_LIB(MpMirror);\
	MP_LIB(MpFlip);\
	/*MP_LIB(MpOutScl);*/\
	/*MP_LIB(MpOutSclUser);*/\
	MP_LIB_LIST_ADV_AE


#ifdef __ISP_LIB__
	MENU_LIB_LIST
#endif

// -------------------------------------------------------------------------------------------------
// Parameter status define
/*
enum{	// common
   	MN_OFF,
	MN_ON
};

enum {	// common
	MN_3sLOW,
	MN_3sMID,
	MN_3sHI
};

enum {	// common
   	MN_4sOFF,
	MN_4sLOW,
	MN_4sMID,
	MN_4sHI
};

enum {	// MP(MpSysFreq)
	MN_SYSFREQ_50,
	MN_SYSFREQ_60
};

enum{	// MP(MpOutFps)
	MN_2M_1080p_25_30,
	MN_2M_1080p_50_60,
	MN_2M_720p_25_30,
	MN_2M_720p_50_60
};

enum{	// MP(MpOutFps)
	MN_2M30_1080p_25_30,
	MN_2M30_720p_25_30,
	MN_2M30_720p_50_60
};

enum{	// MP(MpOutFps)
	MN_1M_720p_25_30,
	MN_1M_720p_50_60
};
*/
#define MN_FPS_HI	((FPS_VDO==50)||(FPS_VDO==60))
#define MN_FPS_LOW	(!MN_FPS_HI)

/*
enum{	//MP(MpOutFromat)
	MN_FRMT_SD,
	MN_FRMT_HD,
	MN_FRMT_UV
};

enum{
	MN_BR_2400,
	MN_BR_4800,
	MN_BR_9600,
	MN_BR_57600,
	MN_BR_115200
};
*/
/*
//MP(MpIris)
//#define	MN_LENS_MNL		0
//#define	MN_LENS_DC		1	// DC
//#define	MN_LENS_AUTO	1	// pIRIS, AF AUTO


enum {	//MP(MpShutter)
	MN_SHUT_AUTO,
	MN_SHUT_MNL,
	MN_SHUT_FLICKER
};

enum{	//MP(MpDcMode)
	MN_DC_IN,
	MN_DC_OUT,
	MN_DC_DEBLUR
};

enum{	//MP(MpShtMode)
	MN_SHT_NORMAL,
	MN_SHT_DEBLUR
};

enum {	//MP(MpDss)	// 14.2.13
	MN_DSS_OFF	,
	MN_DSS_X2	,
	MN_DSS_X4	,
	MN_DSS_X8	,
	MN_DSS_X16	,
	MN_DSS_X32	,
	MN_DSS_X64
};

enum{	//MP(MpBackLight)
	MN_BL_OFF,
	MN_BL_HLC,
	MN_BL_BLC,
	MN_BL_WDR
};

enum {	//MP(MpTdn)
	MN_TDN_AUTO,
	MN_TDN_DAY,
	MN_TDN_NIGHT,
	MN_TDN_EXTERN
};

enum{	//MP(MpDefogMode)
	MN_DEFOG_MANUAL,
	MN_DEFOG_AUTO
};

enum {	//MP(MpAwb)
	MN_AWB_AUTO,
	MN_AWB_AUTOEXT,
	MN_AWB_PRESET,
	MN_AWB_MNL
};

enum {	//MP(MpGamma)
	MN_GAMMA_045,
	MN_GAMMA_055,
	MN_GAMMA_065,
	MN_GAMMA_075
};

enum {	//MP(MpCamTitleOn)
	MN_CT_OFF,
	MN_CT_RIGHT_UP,
	MN_CT_LEFT_DOWN
};
*/

//-------------------------------------------------------------------------------------------------
// Extern
extern void ChangeMenuSize(void);
extern void InitMenu(void);
extern void InitFontOSD(void);

extern void OsdCamTitle(void);
extern void OsdClearAll(void);
extern void Menu(void);

extern void DIn_InitMenu(void);
extern void DIn_Menu(void);

extern void MenuReset(void);
extern void MenuResetDef(void);
extern void MenuSave(UINT anSaveOn);
extern void MenuStyleSet(const BYTE abMenuStyle);

extern void menu_redraw(int iChg, int iGrayOnly);

//-------------------------------------------------------------------------------------------------
// Extern - Parameter & Global

	extern BYTE gbMnParSaveChk;
	extern BYTE gbMnParReadChk;
//	extern BYTE gbMnParTbl[MN_PAR_EA];

	extern BYTE gbMnBlcOsdOn;
	extern BYTE gbMnDebugFnc;
	extern BYTE gbMnDebugBypass;
	extern BYTE gbMnIrisCal;
	extern BYTE gbMenuState;
	extern BYTE gbMenuPushState;

#if (model_Lens==0)

#elif(model_Lens==1)

#else
	extern BYTE gbMnPresetAct;
	extern BYTE gbMnPresetSave;
	extern BYTE gbMnZoomDirSel;
	extern BYTE gbMnFocusDirSel;
	extern BYTE gbMnInitialState;
	extern BYTE gbMnPuntCorrection;
#endif


#define _S(NAME)		s##NAME

#if MN_LNG_EA == 1
  #if model_CharMax == 1
	#define PEXCH		UINT//EXCH*
  #else
	#define PEXCH		EXCH
  #endif
#else
	#define PEXCH		UINT//EXCH*
#endif

#define MENU_TITLE			((const PEXCH*)gbMenuTitle)
#define MENU_NAME			((const PEXCH*)gbMenuList)
#define MENU_VAL			((const PEXCH*)gbMenuVal)

#define MENU_TITLE_IS(V)	(MENU_TITLE == _S(V))
#define MENU_NAME_IS(V)		(MENU_NAME == _S(V))
#define MENU_VAL_IS(V)		(MENU_VAL == _S(V))

extern PEXCH* gbMenuTitle;
extern PEXCH* gbMenuList;
extern PEXCH* gbMenuVal;

#define EXTERN_STR(STR)		extern const PEXCH _S(STR)[]

EXTERN_STR(NOT_USED);
EXTERN_STR(OFF);
EXTERN_STR(MENU);
//EXTERN_STR(MOTION);
//EXTERN_STR(BACKLIGHT);
//EXTERN_STR(SMART_IR);


// FrameRate
#if (model_1M)
	#define	UpMpOutFps		MN_1M_720p_50_60				// MN_2M_1080p_25_30 = MN_1M_720p_25_30
	#define	UpFrameMode		MN_FRAMEMODE_NORMAL_50_60 		// 60FPS
#elif (model_2M)
	#define	UpMpOutFps		MN_2M_1080p_25_30				// MN_2M_1080p_25_30 = MN_1M_720p_25_30
	#define	UpFrameMode		MN_FRAMEMODE_NORMAL_25_30		// 30FPS
#else
	#define	UpMpOutFps		MN_2M30_1080p_25_30				// MN_2M_1080p_25_30 = MN_1M_720p_25_30
	#define	UpFrameMode		MN_FRAMEMODE_NORMAL_25_30		// 60FPS
#endif

//#define	UpMpFrameRateLv	MN_3sHI					// 170328 - hjlee

// System
#define	UpMpSysFreq		MN_SYSFREQ_60
//#define	UpMpOutSclUser	16/*10*/
#define	UpMpCBar		MN_OFF
#define	UpMpReset		MN_OFF
#define	UpMpLanguageSel	0
#define	UpMpOutFromat	MN_FRMT_HD
#define	UpMpCamID		/*0*/1
#define	UpMpBaudrate	MN_BR_INIT

#if model_Sens == SENS_IMX123
	#define UpMpCamTitleOn	MN_ON
#else
	#define UpMpCamTitleOn	/*MN_CT_RIGHT_UP*//*MN_CT_LEFT_DOWN*/MN_CT_OFF
#endif	

#define	UpMpCamTitle0	'0'
#define	UpMpCamTitle1	'0'
#define	UpMpCamTitle2	'0'
#define	UpMpCamTitle3	'0'
#define	UpMpCamTitle4	'0'
#define	UpMpCamTitle5	'0'
#define	UpMpCamTitle6	'0'
#define	UpMpCamTitle7	'0'

// AE
#define UpMpBrightness      10
#define UpMpBrightnessMin   10
#define UpMpLSpotLv         20
#define UpMpShutter     MN_SHUT_AUTO
#define UpMpShutSpd     MN_MNL_SHUT_DIV1	// 1125 line, 1/30 sec
#define UpMpShtMin      MN_MNL_SHUT_DIV512	// 2 line, 1/15000 sec
#define UpMpDeblurMin   MN_MNL_SHUT_DIV1/*MN_MNL_SHUT_DIV8*/	// 140 line, 1/250 sec
#define UpMpShtMax      MN_MNL_SHUT_DIV1	// 1125 line, 1/30 sec
#define UpMpDcMode      MN_DC_IN
#define UpMpShtMode     MN_SHT_NORMAL
#define UpMpDss         MN_DSS_OFF
#define UpMpAgc         100
#define UpMpAgcMin      0
#define UpMpAllSpeed	10
#define UpMpAgcSpeed	9/*10*/
#define UpMpIrsSpeed	10/*20*/		// 20:갑자기 밝아지는 경우(조명을 비추는 등) 흔들림, 10:야외 구름낀 주간에서 흔들림
#define UpMpOpenSpeed	9/*10*/
#define UpMpCloseSpeed	9/*10*/
#define UpMpSmallSpeed	/*0*//*1*/2		// 0:극악조건에서헌팅발생, 1:극악조건에서흔들림, 2:극악조건에서괜찮음
#define UpMpSatBrt		10/*5*//*11*//*12*//*13*/
#define UpMpMinGammaY	0				// ANTI_SAT→BACKLIGHT / 0:OFF, 1:LOW, 2:MIDDLE, 3:HIGH
#define UpMpAntiSatOn	MN_OFF
#define UpMpStabilizing	MN_4sHI

// Backlight
#define UpMpBackLight   MN_BL_OFF	// Do not use MN_BL_WDR !!! - MenuReset()에서 MP(MpBackLightNor) = UP(UpMpBackLight); 실행 시 MN_BL_WDR 이 적용되면 안됨
#define UpMpHlMaskThrs  20
#define UpMpHlMaskColor 7
#define UpMpBlcPosX     3/*8*/
#define UpMpBlcPosY     10/*7*/
#define UpMpBlcSizX     14/*3*/
#define UpMpBlcSizY		4/*3*/
#define UpMpWdrWgt		MN_3sHI/*MN_3sMID*/

// TDN, LED
#define UpMpTdn			MN_TDN_AUTO
#define UpMpTdnThrs     10
#define UpMpTdnMarg     10
#define UpMpTdnDly      MN_3sLOW
#define UpMpLedOn   	MN_OFF
#define UpMpLedSatCtrl  10
//#define UpMpSmartIR     1
#define UpMpTdnSw       0 	// High
#define UpMpTdnDNThres  13
#define UpMpTdnNDThres  7

// ACE,Defog
#define UpMpAce         MN_4sOFF
#define UpMpAceGmgn     0x20
#define UpMpAceBrt      0/*32*/
#define UpMpAceCont     0/*32*/
#if (model_Contrast==1)
	#define UpMpDefog       MN_OFF/*MN_ON*/
	#define UpMpDefogMode   MN_DEFOG_MANUAL
#else
	#define UpMpDefog       MN_OFF
	#define UpMpDefogMode   MN_DEFOG_AUTO
#endif
#define UpMpDefogLevel  MN_3sHI/*MN_3sMID*/

// AWB
#define UpMpAwb             MN_AWB_AUTO
#define UpMpAwbPrst         MN_OFF
#define UpMpAwbMnl          MN_3sMID
#define UpMpRgain           10
#define UpMpBgain           10
//  #define UpMpSaturation  10               // COLOR GAIN
#if (model_Sens == SENS_AR0237CSH)
	#define UpMpSaturation      19
	#define UpMpSaturationR     20
	#define UpMpSaturationB     21
#else
	#define UpMpSaturation      20	// 2017419 - WHL : CBB TEST
	#define UpMpSaturationR     20
	#define UpMpSaturationB     20
#endif

// ETC
#define UpMpAdnr			MN_4sMID	// Do not use MN_4sOFF !!!
#define UpMpSharpness       5
#define UpMpShpSmallEdge    128
#define UpMpShpBigEdge      10/*48*/
#define UpMpShpAgcLow       8
#define UpMpShpAgcMid       48
#define UpMpShpAgcHigh      64
#define UpMpGamma           MN_GAMMA_AUTO/*MN_GAMMA_065*//*MN_GAMMA_045*//*MN_GAMMA_055*/
#define UpMpFlip            MN_OFF/*MN_OFF*/
#define UpMpMirror          MN_OFF/*MN_OFF*/
#define UpMpShading         MN_OFF
#define UpMpShdWeight       100

// Privacy Box
#define UpMpPrivacy         MN_OFF
#define UpMpPvcZone         0
#define UpMpPvcFormat       0
#define UpMpPvcTrans        0/*2*/
#define UpMpPvcYLevel       10
#define UpMpPvcCRLevel      10
#define UpMpPvcCBLevel      10

// IMD
#define UpMpItl             MN_OFF
#define UpMpItlSens         (IMD_SENS_MAX>>1)
#define UpMpItlWindowuse    0
#define UpMpItlDettone      2
#define UpMpItlRectFill     MN_ON/*MN_OFF*/
#define UpMpItlMaskOsd      MN_ON
#define UpMpItlAlarm        MN_ON
#define UpMpItlSigno        MN_OFF/*MN_ON*/

// Hidden MENU
//#define UpMpShdDet          MN_OFF
//#define UpMpDefDet          MN_OFF

#if 1//(model_Lens==0)  // 2016610 - WHL
	#define UpMpFad_On      0
	#define UpMpFad_Zone    2
	#define UpMpFad_Trans   0
	#define UpMpFad_PosX    4
	#define UpMpFad_PosY    2
	#define UpMpFad_SizX    12
	#define UpMpFad_SizY    14
#endif

#if 1//(model_Lens==1)	 //TwoMotorAF
  #if 1
	#define UpMpMAF_mode		0
	#define UpMpMAF_Filter		0
	#define UpMpMAF_MotorInit	0
	#define UpMpMAF_TestMode	0
	#define UpMpMAF_TotalPos_Z	0
	#define UpMpMAF_StartPos_Z	0
	#define UpMpMAF_EndPos_Z	0
	#define UpMpMAF_TotalPos_F	0
	#define UpMpMAF_StartPos_F	0
	#define UpMpMAF_EndPos_F	0
	#define UpMpMAF_LensModel	model_MAF_Lens
  #else
	#define UpMpAFmode			0
	#define UpMpScanning		0
	#define UpMpOnepushAF		0
	#define UpMpSyncTDN			0
  #endif
#endif

#if	1//(IMAGE_INPUT_MODE==1)
	#define UpMpPipOn			0
	#define UpMpPipColor		1 //	Yellow
	#define UpMpPip1_PosX		62
	#define UpMpPip1_PosY		3
	#define UpMpPip1_SizX		30/*0x28*/
	#define UpMpPip1_SizY		0x16
#endif

#if	1//(DOOR_BELL_CAMERA==1)
	#define UpMpDoorCam_LP_Mode		0
#endif


//*************************************************************************************************
// 6. AE
//-------------------------------------------------------------------------------------------------
// Misc

#ifndef __ISP_LIB__

#if (model_1M)

	#define UpAE2_HSP				0x4
	#define UpAE2_HEP				0x27a/*0x270*/
	#define UpAE2_VSP				0xa/*0x0c0*/
	#define UpAE2_VEP				0x2c4

	#define UpAE2_HSP_CROP			0
	#define UpAE2_HEP_CROP			0
	#define UpAE2_VSP_CROP			0
	#define UpAE2_VEP_CROP			0

	#define UpAE_SHT_SLICE_LINE		2
	#define UpAE_OUTDOOR_THRS_VAL	2
	#define UpAE_DEBLUR_THRS_VAL	2

	#define AE_SHT_SLICE			(int)LibAeShtLine(UP(UpAE_SHT_SLICE_LINE))					// Shutter limit (line)
	#define AE_OUTDOOR_THRS			(int)LibAeShtLine(gnAeVtw>>UP(UpAE_OUTDOOR_THRS_VAL))		// shutter threshold (line)	// 1/240s
	#define AE_DEBLUR_THRS			(int)LibAeShtLine(gnAeVtw>>UP(UpAE_DEBLUR_THRS_VAL))		// shutter threshold (line)	// 1/240s

	#define UpAE_SPEED_DC			0															// Speed : 0 ~ 3 (Fast ~ Slow)

//#elif (model_2M30p)
#elif (model_2M|model_2M30p)	// 2015911 - WHL

	#define UpAE2_HSP				0x4															// Luma detection area X start
	#define UpAE2_HEP				0x3b8                                                       // Luma detection area Y end
	#define UpAE2_VSP				0xa                                                         // Luma detection area Y start
	#define UpAE2_VEP				0x434                                                       // Luma detection area Y end
	
	#define UpAE2_HSP_CROP			0x4															// Luma detection area X start	in CROP mode
	#define UpAE2_HEP_CROP			0x27c                                                       // Luma detection area Y end    in CROP mode
	#define UpAE2_VSP_CROP			0x16                                                        // Luma detection area Y start  in CROP mode
	#define UpAE2_VEP_CROP			0x2da                                                       // Luma detection area Y end    in CROP mode


	#define UpAE_SHT_SLICE_LINE		3															// Shutter limit (line)
	#define UpAE_OUTDOOR_THRS_VAL	3															// shutter threshold in OUTDOOR mode (ex. 1125>>3 =  375 => 1/240s)
	#define UpAE_DEBLUR_THRS_VAL	3                                                           // shutter threshold in DEBLUR mode (ex. 1125>>3 =  375 => 1/240s)

	#define AE_SHT_SLICE			(int)LibAeShtLine(UP(UpAE_SHT_SLICE_LINE))
	#define AE_OUTDOOR_THRS			(int)LibAeShtLine(gnAeVtw>>UP(UpAE_OUTDOOR_THRS_VAL))
	#define AE_DEBLUR_THRS			(int)LibAeShtLine(gnAeVtw>>UP(UpAE_DEBLUR_THRS_VAL))

	#define UpAE_SPEED_DC			1															// DC IRIS Speed : 0 ~ 3 (Fast ~ Slow)

#else

#endif


	// common -------------------------------------------
	#define UpAE_ORIGINAL_CTRL		0

	#define	UpAE_TGT_GAIN			36/*0x34*//*0x1a*//*0xd*/															// Normal Brightness target gain
	#define	UpAE_TGT_OFST			20/*0x10*//*0x20*/												// Normal Brightness target offset
	#define	UpAE_CLIP_OFST			0x140														// Normal Brightness Clip level (+ Target offset)

	#define	UpAE_SPEED				128															// Tracking Speed : 0 ~ 128 ~ 255 (x0.25 ~ x1 ~ x4)
	#define UpAE_SPEED_DC_VP		19															// IRIS virtual position control Speed for judge IRIS full open (Slow 9 ~ Fast 30)

	#define	UpAE_DEBLUR_GAIN		128/*100*/															// Deblur shutter increasing gradient	 (line)	// 64 ~ 256 -> 0.5 ~ 2.0

	#define	UpAE_WDR_ON_AGC			200/*300*/															// WDR AGC position for maximum weight
	#define AE_WDR_OFF_AGC			(UP(UpAE_WDR_ON_AGC)+200/*100*/)									// WDR AGC position for minimum weight
	#define AE_WDR_MAX_WGT			0x100														// WDR maximum weight (100% = 0x100)
	#define	UpAE_WDR_MIN_WGT		0/*0x80*/														// WDR minimum weight

	#define	UpAE_WDR_MOTION_SEL		0x0															// Motion mismatch effect selection	(0 or 1)
	#define	UpAE_WDR_STYLE_SEL		1					// 150802								// WDR Style ( 0 : DR first, 1 : Contrast first)

	#define	AE_WDR_LTGT_GAIN		0x3															// WDR Long Brightness target gain
	#define	UpAE_WDR_LTGT_OFST		0x64/*0x8a*/                                                // WDR Long Brightness target offset
	#define	UpAE_WDR_LTGT_OFST2		0x60/*0x30*/		        // 150802                               // WDR Long Brightness target offset (UpAE_WDR_STYLE_SEL==1)
	#define	AE_WDR_LCLIP_OFST		0x50                                                        // WDR Long Brightness Clip level (+ Target offset)

	#define	AE_WDR_STGT_GAIN		AE_WDR_LTGT_GAIN											// WDR Short Brightness target gain
	#define	UpAE_WDR_STGT_OFST		0x220/*0x262*/                                              // WDR Short Brightness target offset
	#define	UpAE_WDR_STGT_OFST2		0xa0/*0x88*//*0x280*/		        // 150802                               // WDR Short Brightness target offset (UpAE_WDR_STYLE_SEL==1)
	#define	AE_WDR_SCLIP_OFST		(-0x80)                                                     // WDR Short Brightness Clip level (+ Target offset)

	#define UpAE_WDR_SWGT_L			0x80														// WDR weight adjust for menu Low	 (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)
	#define UpAE_WDR_SWGT_M         0x80                                                        // WDR weight adjust for menu Middle (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)
	#define UpAE_WDR_SWGT_H         0x80                                                        // WDR weight adjust for menu High   (0x40 = x0.5, 0x80 = x1, 0xC0 = x1.5, 0xff = x2)
	
	#define AE_LED_PWR(VAL)			IRIS_CLPHw(VAL);											// LED Power Strength (VAL : 1 ~ 1375)

		
	#define AE_ROI_WDR_BOX_NO		16													//161011 LGH : ROI area separate box no. at ROI WDR mode
		
	//161010 LGH : Add ROI WDR mode
	typedef struct _tagROIWDR{
		BYTE	bAction;
		BYTE	bPosX_Lo;
		BYTE	bPosX_Up;
		BYTE	bPosY_Lo;
		BYTE	bPosY_Up;
		BYTE	bSizX_Lo;
		BYTE	bSizX_Up;
		BYTE	bSizY_Lo;
		BYTE	bSizY_Up;
	} _ROIWDR;
	

#endif//__ISP_LIB__

	#define model_Iris_DC		(/*(model_Iris==0)||*/(model_Iris==1)||(model_Iris==2))
	#define model_Iris_P		((model_Iris==3)||(model_Iris==4))

#if(model_Iris_DC)					// DC Iris
	#define UpMpIris	MN_LENS_MNL/*MN_LENS_DC*/
#elif(model_Iris_P)
	#define UpMpIris	MN_LENS_MNL/*MN_LENS_AUTO*/
#else
	#define UpMpIris	MN_LENS_MNL		// Fixed
#endif
#define UpMpIrisLvl     20

	enum {														// WDR Flag.
	AeSHORT=0,
	AeLONG
	};

//-------------------------------------------------------------------------------------------------
//Extern

extern void InitAe(void);
extern void Ae(void);
extern void AeDev(void);
extern int TgtMaxGet(const BYTE abWdrOn, const UINT anLSflag);
extern int TgtMinGet(const int aiTgt, const BYTE abV);

extern UINT gnAeState;						// AE control state, 0:Shutter, 1:Iris, 2:AGC, 3:DSS

extern int 	giCurAgc;						// AE AGC current value (normalized position)
extern UINT	gnAeAgcTVal;					// AE AGC Max value (normalized position)
	#define gnAgcLim	gnAeAgcTVal			// AE AGC Max value (normalized position)
extern int 	giCurDss;						// AE DSS current value
extern int 	giCurDssStg;					// AE DSS OFF counter for isp_reset()

extern int	giErr1d;						// AE Tracking finish state

extern BYTE gbWdrOn,gbWdrOnBuf,gbWdrOnBuf2;	// WDR on/off status
extern WORD	gwWdrCnt;						// WDR Entrance cnt
extern WORD gwWdrGmk;						// WDR Gamma weight

extern BYTE gbROIWdrBoxFlag;
extern UINT gnBoxROIWdr[2];
extern BYTE gbBoxOnROIWdr;
extern BYTE gbBoxFillROIWdr;
extern BYTE gbBoxToneROIWdr;

enum{
	AE_STG_SHT,	// 0
	AE_STG_IRS,	// 1
	AE_STG_AGC,	// 2
	AE_STG_DSS,	// 3
	AE_STG_DBL,	// 4
	AE_STG_TGT	// 5
};

extern BYTE gbAeStg;
extern void AntiSatStabilizingSet(void);

#define SHT_DBL_MAX		giShtDblMax
#define AGC_DBL_MAX		giAgcDblMax

#define SHT_MIN			giShtMin
#define SHT_MAX			giShtMax

#define AGC_MAX			giAgcMax
#define AGC_MIN			giAgcMin

#define AGC_SET(V)		AgcCtrl(0,0,0,0,0,0,0,1,(V))
#define SHT_SET(V)		ShtCtrl(0, 0, AE_ERR_MGN, 1100, SHT_MIN, SHT_MAX, 0, 0, 0, (V))
#define SHT_SET1(V)		ShtCtrl(1, 0, AE_ERR_MGN, 1100, SHT_MIN, SHT_MAX, 0, 0, 0, (V))
//#define IRS_SET(V)		IrsCtrl(0, 0, 0, 0, 1, (V))

#if 1
#define IRS_RUN(E,M,G)	{ IrsCent((gnMnLensChg ? 0 : E), M, AE_IRS_OPN_Max, AE_IRS_CLS_Max, AE_IRS_STAT_Max);\
						  giIrsValBuf = IrsCtrl(E, M, G, giIrsCent>>6, 0, 0);\
						  IrsPos( E, M, ParAe(PA_IRS_POS_SPEED), 0, AE_IRS_STAT_Max);\
						  /*if(gbIrsDly && giIrsPos>=AE_IRS_STAT_Max) giIrsPos = AE_IRS_STAT_Max - 1;*/ }
#else
#define IRS_RUN(E,M,G)	{ giIrsValBuf/*iIrsVal*/ = LibAeSpeedDc(E, M, AE_IRS_OPN_Max, AE_IRS_CLS_Max, UP(UpAE_SPEED_DC), 0);\
						  IrsPos(E, M, ParAe(PA_IRS_POS_SPEED)/*9*//*12*/, 0, AE_IRS_STAT_Max);\
						  /*giIrsValBuf = iIrsVal;*/ }
#endif

#define AE_CTRL_ADV			((model_ADV_AE==1)&&(model_Pana==0)&&(model_Sens!=SENS_IMX225))
#define AE_CTRL_ORIGINAL	((AE_CTRL_ADV==0) || ParAe(PA_CTRL_ORIGINAL))
#define AE_SAT_OFF			((MP(MpBackLight)==MN_BL_WDR) || (gbWdrOn==MN_ON) || (gbWdrOnBuf2==MN_ON) || ParAe(PA_SAT_OFF) || (MP(MpAntiSatOn)==MN_OFF)/*(MP(MpLSpotLv)==0)*/ /*|| MP(MpClipMax)*/)

#define AE_DEV_ON			((model_Pana==0)&&(model_Sens!=SENS_IMX225))

#define ERR_CHT_SIZ			60		// (+Lib)
#define ERR_SPOT_SUP_GAB	UP(UpAE_TGT_OFST)

#define AE_TGT_MIN_BIT		2
//#define NO_EST_SPD_DOWN_BIT	3

#if AE_CTRL_ADV
	extern BYTE gbMpShtMin;
	extern BYTE gbMpDeblurMin;
	extern BYTE gbMpShtMax;
	extern void ShutterMenuGet(void);
	extern void ShutterMenuSet(void);
#else
	#define ShutterMenuGet()
	#define ShutterMenuSet()
#endif

//extern int iErrBg1dIIR;
//extern int iCurBgIIR;
//extern int iTgt;
//extern int iCurBgSum1IIR;

//#define SetParAe(ADDR,DATA)		SetPar(ADDR,DATA)
//#define GetParAe(ADDR)			GetPar(ADDR)
//#define SetGrpAe(ADDR,DATA)		SetPar(ADDR,DATA)
//#define GetGrpAe(ADDR)			GetPar(ADDR)
#define GrpAe(ID)	gnTxGrp[ID].m_UINT
#define GrpAeF(ID)	gnTxGrp[ID].m_float

#if 1
enum{// GA = Graph of AE
	// Use List		// Scale,	Offset

	//GA_AE_POS,

	//GA_WDR_SHORT_POS,
	//GA_WDR_LONG_POS,

	//GA_AGC_GAIN,
	//GA_AGC_ERR,

	//GA_SHT_GAIN,
	//GA_SHT_ERR,

	//GA_SHT_POS,
	//GA_SHT_POS_N,
	//GA_SHT_POS_P,

	//GA_IRS_CTRL_ORI,

	//GA_IRS_VAL0,
	//GA_IRS_ERR,
	//GA_AGC_OFF,
	//GA_IRS_CTRL,

	//GA_SHT_VAL,
	
	GA_ERR_NIGHT_CNT,		// gcErrSignAccu	YGMY_01r	iCurSpotChg>>IIG	iTgtVal
	//GA_TGT_NIGHT,
	//GA_ERR_DAY_POS_N,
	//GA_ERR_DAY_OFS_CNT,
	GA_ERR_DAY_ORI,			// iErrBgChg>>IIG	nAe2ClipCnt		iTgtVal

	//GA_WDR_SHT_S_VAL,
	//GA_WDR_SHT_S_VAL_CNT,

	//GA_ERR_BG_CHG,
	//GA_CUR_SPOT_CHG,

	GA_AGC_ERR_CHG,
	GA_AGC_ERR_EST_2,

	//GA_SHT_ERR_EST,
	//GA_SHT_ERR_CHG,
	//GA_SHT_ERR_EST_2,
	//GA_SHT_ERR_EST_1,
	//GA_SHT_ERR_EST_0,

	//GA_CUR_DSS_STG,

	//GA_CLIP_CNT,
	//GA_SUM2_AVG,
	//GA_CLIP_CNT_OFF,
	//GA_CLIP_CNT_ON,
	//GA_ERR_DAY,				// iErrOfs		YGMY_01r
	//GA_ERR_NIGHT_ORI,		// iErrNightRaw
	//GA_CUR_NIGHT,
	GA_ERR_NIGHT,			// GA_ERR_DAY와 같은 위치	GA_CUR_NIGHT와 같은 위치	iErrNightRaw	gbAgcDly*10 + gbShtDly
	
	
	//GA_IRS_VAL,

	//GA_CUR,
	//GA_SHT_ERR,

	//GA_AGCw,// 5 Fix

	//GA_SHT_VAL,
	//GA_AGC_VAL,

	//GA_ERR_DAY_OFS,
	GA_ERR_CHT_PRE,			// iCurSpotEst GA_ERR 와 같은 위치	iErrSpot

	GA_AGC_VAL_PRE,			// giAgcVal + giShtVal - 125		iCurSpotChg>>IIG
	GA_ERR,					// iErrBg


	__GA_END	// !! CAUTION - GA_END must be less than 9.
};
#endif

#ifdef ENABLE_PA
#define ParAe(ID)	gnRxPar[ID]
enum{// PA = Parameter of AE
	PA_STA=0x0,

	PA_H264_SKIP_CNT,
	PA_WDR_SHORT_MIN1,
	PA_ERR_CHT_TH,
	PA_SLICE,
	PA_CLIP,
	PA_ERR_NIGHT_ON_AGC,
	PA_TGT_NIGHT_OFS,
	PA_TGT_NIGHT,
	PA_SAT_OFF,
	PA_ERR_UP_MAX,
	PA_AGC_OFF_CNT,
	PA_IRS_POS_SPEED,
	PA_IRS_OPEN_SPEED,
	PA_IRS_CLOSE_SPEED,
	PA_ERR_EST_OFF,
	PA_IRS_GAIN_MIN_BIT,
	PA_IRS_GAIN_MIN_ERR,
	PA_IRS_GAIN_MAX_ERR,
	PA_AGC_GAIN_MIN_BIT,
	PA_AGC_GAIN_MIN_ERR,
	PA_AGC_GAIN_MAX_ERR,
	PA_CTRL_ORIGINAL,
	PA_ERR_MGN,
	PA_SHT_MANUAL,
	PA_SHT_UP_SPEED,
	PA_SHT_DOWN_SPEED,
	PA_AGC_MANUAL,
	PA_AGC_UP_SPEED,
	PA_AGC_DOWN_SPEED,
	PA_IRS_MANUAL,
	PA_IRS_CNT,

	PA_END	// !! CAUTION - PA_END must be less than 33.
};
#else
	#define ParAe(V)			V
	#define PA_H264_SKIP_CNT	6
	#define PA_WDR_SHORT_MIN1	8
	#define	PA_ERR_CHT_TH		60
	#define	PA_SLICE			0
	#define	PA_CLIP				0
	#define	PA_ERR_NIGHT_ON_AGC	0
	#define	PA_TGT_NIGHT_OFS	64
	#define	PA_TGT_NIGHT		0
	#define	PA_SAT_OFF			0
	#define	PA_ERR_UP_MAX		0
	#define	PA_AGC_OFF_CNT		0
	#define	PA_IRS_POS_SPEED	UP(UpAE_SPEED_DC_VP)
	#define	PA_IRS_OPEN_SPEED	iIrsUpSpd
	#define	PA_IRS_CLOSE_SPEED	iIrsDnSpd
	#define	PA_ERR_EST_OFF		0
	#define	PA_IRS_GAIN_MIN_BIT	2
	#define	PA_IRS_GAIN_MIN_ERR	100
	#define	PA_IRS_GAIN_MAX_ERR	300
	#define	PA_AGC_GAIN_MIN_BIT	MP(MpSmallSpeed)
	#define	PA_AGC_GAIN_MIN_ERR	20
	#define	PA_AGC_GAIN_MAX_ERR	100
	#define	PA_CTRL_ORIGINAL	UP(UpAE_ORIGINAL_CTRL)
	#define	PA_ERR_MGN			8
	#define	PA_SHT_MANUAL		0
	#define	PA_SHT_UP_SPEED		iShtUpSpd
	#define	PA_SHT_DOWN_SPEED	iShtDnSpd
	#define	PA_AGC_MANUAL		0
	#define	PA_AGC_UP_SPEED		giAgcUpSpd
	#define	PA_AGC_DOWN_SPEED	giAgcDnSpd
	#define	PA_IRS_MANUAL		0
	#define	PA_IRS_CNT			0
#endif


//*************************************************************************************************
// 7. TDN
//-------------------------------------------------------------------------------------------------

#if (model_Tdn==0)			// 1 Pin 2 state control
   #define TDN_MECH_HLD		{}
   #define TDN_MECH_NIT     {GpioOutDir(GPIO_TDN_1); GpioSetHi(GPIO_TDN_1);}
   #define TDN_MECH_DAY     {GpioOutDir(GPIO_TDN_1); GpioSetLo(GPIO_TDN_1);}

#elif (model_Tdn==1)		// 1 Pin 3 state control
   #define TDN_MECH_HLD		{GpioInDir(GPIO_TDN_1);}
   #define TDN_MECH_NIT     {GpioOutDir(GPIO_TDN_1); GpioSetHi(GPIO_TDN_1);}
   #define TDN_MECH_DAY     {GpioOutDir(GPIO_TDN_1); GpioSetLo(GPIO_TDN_1);}

#else						// 2 Pin control
	#if (model_TgtBd==0)
	 	#if ((model_Lens==0)||(model_Lens==1))
			#if (model_Iris==1)		// Kiwi driver
			#define TDN_MECH_HLD	{GpioSetHi(GPIO_TDN_2);	GpioSetLo(GPIO_TDN_1);}		// TDN control pin assign
			#define TDN_MECH_DAY	{GpioSetHi(GPIO_TDN_2);	GpioSetHi(GPIO_TDN_1);}		//
			#define TDN_MECH_NIT	{GpioSetLo(GPIO_TDN_2);	GpioSetLo(GPIO_TDN_1);}		//
			#else
				//#define TDN_MECH_HLD 	{GpioSetLo(GPIO_TDN_2); GpioSetLo(GPIO_TDN_1);}  	// EVM3
		      	//#define TDN_MECH_DAY 	{GpioSetLo(GPIO_TDN_2); GpioSetHi(GPIO_TDN_1);}  	//
	   			//#define TDN_MECH_NIT 	{GpioSetHi(GPIO_TDN_2); GpioSetLo(GPIO_TDN_1);}  	//
				#define TDN_MECH_HLD 	{GpioSetLo(GPIO_TDN_1); GpioSetLo(GPIO_TDN_2);}  	// EV3838
		      	#define TDN_MECH_DAY 	{GpioSetLo(GPIO_TDN_1); GpioSetHi(GPIO_TDN_2);}  	//
	   			#define TDN_MECH_NIT 	{GpioSetHi(GPIO_TDN_1); GpioSetLo(GPIO_TDN_2);}  	//
			#endif

		#endif

	#else	// Customer area
			#define TDN_MECH_HLD	{}
			#define TDN_MECH_DAY	{}
			#define TDN_MECH_NIT	{}

	#endif
#endif

#if (model_Cds==1)
	#define TDN_CDS_IN		(GpioGetPin(GPIO_CDS_IN))	//(!GpioGetPin(GPIO_CDS_IN))	// assign CDS input (using GPIO)
#else
//	#define TDN_CDS_IN		(ADC_CDSr < 100)	 											// assign CDS input (using ADC)
	#define TDN_CDS_IN		(ADC_CDSr)	 												// assign CDS input (using ADC)
#endif

//	#define UpTDN_EXTN_THRS_STEP		23		//TDN extern mode threshold menu step	(1~100 ADC)
#define UpTDN_EXTN_THRS_STEP		34		//TDN extern mode threshold menu step	(1~100 ADC)
#if (model_Sens == SENS_IMX225)
#define UpTDN_AUTO_THRS_STEP		45		//TDN auto mode threshold menu step		(1~100 AGC)
#else
#define UpTDN_AUTO_THRS_STEP		28		//TDN auto mode threshold menu step		(1~100 AGC)
#endif
#define UpTDN_AUTO_MAGN_STEP		8		//TDN auto mode amrgine menu step		(1~50 AGC)
#define UpTDN_DLY_LOW				1000	//TDN low delay time	(1000->1s, 2000->2s,....,20000->20s)
#define UpTDN_DLY_MIDDLE			5000	//TDN middle delay time	(1000->1s, 2000->2s,....,20000->20s)
#define UpTDN_DLY_HIGH				20000	//TDN high delay time	(1000->1s, 2000->2s,....,20000->20s)

#if (model_Sens == SENS_IMX225)
	#define TDN_THRS		(MP(MpTdnThrs)*UP(UpTDN_AUTO_THRS_STEP))+20;						// TDN AGC threshold level
	#define TDN_MARG		(MP(MpTdnMarg)*UP(UpTDN_AUTO_MAGN_STEP))+ 4;						// "	   margin level
#else
	#define TDN_THRS		(MP(MpTdnThrs)*UP(UpTDN_AUTO_THRS_STEP))+10;						// TDN AGC threshold level
	#define TDN_MARG		(MP(MpTdnMarg)*UP(UpTDN_AUTO_MAGN_STEP))+ 4;						// "	   margin level
#endif

	#define TDN_DLY_1		(	UP(UpTDN_DLY_LOW)/FR_DUTY)		// 1s                   // 141121 LH : TDN Turn on delay time
	#define TDN_DLY_2		(UP(UpTDN_DLY_MIDDLE)/FR_DUTY)		// 5s                   // "
	#define TDN_DLY_3		(  UP(UpTDN_DLY_HIGH)/FR_DUTY)		// 20s                  // "


enum {
	TDN_HLD = 0,
	TDN_DAY,
	TDN_NIT
};

//------------------------------------------------------------------------------
// extern
extern UINT gnTdnChkOut;		// Day & BW status
extern void TDN(void);


//*************************************************************************************************
// 8. CONTRAST
//-------------------------------------------------------------------------------------------------
//Defog User Parameter

#define UpDEFOG_OSDON	0x0			// Defog Area Set OSD On/Off Function (0x0 or 0x1)

#if (model_1M) 
#define UpDEFOG_720_HW		0xa		// Defog Area Set Horizontal Width (0x0 ~ 0xf)
#define UpDEFOG_720_HSP		0xa0	// Defog Area Set Horizontal Start Position (0x0 ~ 0x790)
#define UpDEFOG_720_VW		0x9		// Defog Area Set Vertical Width (0x0 ~ 0xf)
#define UpDEFOG_720_VSP		0x60	// Defog Area Set Vertical Start Position (0x0 ~ 0x440)
#else
#define UpDEFOG_720_HW		0xa		// Defog Area Set Horizontal Width (0x0 ~ 0xf)
#define UpDEFOG_720_HSP		0xa0	// Defog Area Set Horizontal Start Position (0x0 ~ 0x790)
#define UpDEFOG_720_VW		0x9		// Defog Area Set Vertical Width (0x0 ~ 0xf)
#define UpDEFOG_720_VSP		0x60	// Defog Area Set Vertical Start Position (0x0 ~ 0x440)
#endif

#define UpDEFOG_1080_HW		0xa		// Defog Area Set Horizontal Width (0x0 ~ 0xf)
#define UpDEFOG_1080_HSP	0x1c0	// Defog Area Set Horizontal Start Position (0x0 ~ 0x790)
#define UpDEFOG_1080_VW		0xa		// Defog Area Set Vertical Width (0x0 ~ 0xf)
#define UpDEFOG_1080_VSP	0x14	// Defog Area Set Vertical Start Position (0x0 ~ 0x440)

#define UpDEFOG_TH_LOW	0x100		// Defog Maximum Strength for menu LOW (0x0 ~ 0xffff)
#define UpDEFOG_TH_MID	0x200		// Defog Maximum Strength for menu MIDDLE (LOW ~ 0xffff)
#define UpDEFOG_TH_HIGH	0x500		// Defog Maximum Strength for menu HIGH (MIDDLE ~ 0xffff)

#define UpACETBL1_LOW	0x10		// ACE Dark Area Strength for menu LOW (0x0 ~ 0xff)
#define UpACETBL1_MID	0x30		// ACE Dark Area Strength for menu MIDDLE (LOW ~ 0xff)
#define UpACETBL1_HIGH	0x50		// ACE Dark Area Strength for menu HIGH (MIDDLE ~ 0xff)

#define UpACETBL2_LOW	0x08		// ACE Bright Area Strength for menu LOW (0x0 ~ 0xff)
#define UpACETBL2_MID	0x18		// ACE Bright Area Strength for menu MIDDLE (LOW ~ 0xff)
#define UpACETBL2_HIGH	0x20		// ACE Bright Area Strength for menu HIGH (MIDDLE ~ 0xff)

//#define UpACEWDR1_TH	0x40		// ACE Dark Area Strength of WDR (0x0 ~ 0xff)
//#define UpACEWDR2_TH	0x10		// ACE Bright Area Strength of WDR (0x0 ~ 0xff)

// 150802 UpAE_WDR_STYLE_SEL==1
#define UpACEWDR1_TH	0/*0x20*/		// ACE Dark Area Strength of WDR (0x0 ~ 0xff)
#define UpACEWDR2_TH	0/*0x20*/		// ACE Bright Area Strength of WDR (0x0 ~ 0xff)

#define DEFOG_X_OFF 	30

//------------------------------------------------------------------------------
// extern
extern BYTE gbHistoM[];

extern unsigned int 	gnDefog_HS;
extern unsigned int 	gnDefog_HE;
extern unsigned int 	gnDefog_VS;
extern unsigned int 	gnDefog_VE;
extern unsigned int 	gnAe1Size;
extern unsigned int 	gnHistoM[];
//extern unsigned int 	gnDefogV;
//extern unsigned int 	gnDwdrV;

extern unsigned char 	gbContrastMode;		// DEFOG OFF/ON
extern unsigned char 	gbDefogMode;        // "	 MODE MANUAL/AUTO
extern unsigned char 	gbDefogLevel;       // "	 STRENGTH LOW/MID/HIGH

extern unsigned int 	gnHistoV[];

extern unsigned int 	nDefogTbl[];


extern void InitContrast(void);
extern void Gamma(void);
extern void AceDefog(void);
extern void LibDefog(unsigned int * anDefogThres, unsigned int * anDefogWeight, unsigned int *anDefogTbl, const BYTE abDssOn);

extern void InitHisto(void);
extern void Histo(void);

#define UpGAMMA_Y_ON		1		//Y GAMMA on/off	(0: off, 1: on)
#define UpGAMMA_C_ON		1		//C GAMMA on/off	(0: off, 1: on)


//*************************************************************************************************
// 9. MODE (Out Mode management)
//-------------------------------------------------------------------------------------------------

//	#define	UpOUTSCL_FULL_YCLIP	0xfe												// Final Output High Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_FULL_YOFST	0x01												// Final Output Offset Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_FULL_YGAIN	0x80	// (((256-OUTSCL_FULL_YOFST)*0x80)/256)		// Final Y Output Gain for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_FULL_CGAIN	0x80/*0x7f*/	// 15.3.23 ksh						// Final C Output Gain for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_FULL_CCLIP	0xFF/*0x7f*/	// "								// Final C High Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_FULL_YSLIC	0x01												// Final Output Low Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	
//	#define	UpOUTSCL_COMP_YCLIP	0xf0												// Final Output High Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_COMP_YOFST	0x10												// Final Output Offset Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_COMP_YGAIN	0x78	// (((256-OUTSCL_COMP_YOFST)*0x80)/256)		// Final Y Output Gain for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_COMP_CGAIN	0x78/*0x71*/	// 15.3.23 ksh						// Final C Output Gain for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_COMP_CCLIP	0xF0/*0x70*/	// "								// Final C High Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)
//	#define	UpOUTSCL_COMP_YSLIC	0x10												// Final Output Low Clip Value for FULL in IMAGE RANGE Menu (0x0 ~ 0xFF)

//------------------------------------------------------------------------------
// extern
extern void OutModeRun(void);
extern void Cstm_OutMode(UINT);
extern void OutMode(void);

//*************************************************************************************************
// 10. AWB
//-------------------------------------------------------------------------------------------------
#ifndef __ISP_LIB__

#if (model_1M)

  #define UpAWB_HSP				0x3			  // 150423 HSH
  #define UpAWB_HW				0x279 		  // 150423 HSH
  #define UpAWB_VSP				0x9
  #define UpAWB_VW				0x2c0

  #define UpAWB_HSP_CROP		0
  #define UpAWB_HW_CROP			0
  #define UpAWB_VSP_CROP		0
  #define UpAWB_VW_CROP			0

  #define UpAWB_TCNT_LMT		(0x1000>>1)	  // White pixel cnt limit

  #define UpAWB_DBOFFSET		DFSEL_Sens(0x4d	  ,0x31   ,0	  ,0x28 	  ,0x22   ,0,0x5f	  ,0,0,0)
  #define UpAWB_DROFFSET		DFSEL_Sens(0x9d	  ,0xea   ,0	  ,0xf4 	  ,0xc5   ,0,0x142	  ,0,0,0)
  #define AWB_DBGAIN			DFSEL_Sens(0xd	  ,0xf	  ,0	  ,0xe		  ,0xe	  ,0,0xf	  ,0,0,0)
  #define AWB_DRGAIN			DFSEL_Sens(0xb	  ,0xc	  ,0	  ,0xc		  ,0xc	  ,0,0xc	  ,0,0,0)
  
  #define UpICSEL_FLIP			DFSEL_Sens(1,2,1,1,1,1,1,1,1,1)	// ICSEL at the Flip On
  #define UpICSEL				DFSEL_Sens(1,2,1,1,1,1,1,1,1,1)	// ICSEL at the Flip Off
  #define UpOCSEL_FLIP			DFSEL_Sens(1,2,1,1,1,1,1,1,1,1)	// OCSEL at the Flip On
  #define UpOCSEL				DFSEL_Sens(1,2,1,1,1,1,1,1,1,1)	// OCSEL at the Flip Off
  
#elif (model_2M)

  #define UpAWB_HSP				DFSEL_Sens(0, 0, 0xb, 0x1, 0x1, 0x1, 0, 0, 0, 0)						  // AWB detection area X start
  #define UpAWB_HW				0x3a8 																  // AWB detection area Y end
  #define UpAWB_VSP				0x10																	  // AWB detection area Y start
  #define UpAWB_VW				0x420 																  // AWB detection area Y end

  #define UpAWB_HSP_CROP		DFSEL_Sens(0, 0, 0xb, 0x1, 0x1, 0x1, 0, 0, 0, 0)						  // AWB detection area X start  in CROP mode
  #define UpAWB_HW_CROP			0x268 																  // AWB detection area Y end	 in CROP mode
  #define UpAWB_VSP_CROP		0x20																	  // AWB detection area Y start  in CROP mode
  #define UpAWB_VW_CROP			0x2b0 																  // AWB detection area Y end	 in CROP mode

  #define UpAWB_TCNT_LMT		0x1000		  // White pixel cnt limit

  //                                                                         IMX327                IMX291            AR0237
  #define UpAWB_DBOFFSET		DFSEL_Sens(0, (0x4a+10) ,/*0x26*/0x34 ,0x45 ,0x5c/*0x44*//*0x2c*/ ,(0x51) ,0xb8  ,0 ,0x35/*0x3d*//*0x51*/ ,0x80 ) // Color map coordinate x offset
  #define UpAWB_DROFFSET		DFSEL_Sens(0, (0x98+10) ,/*0xc5*/0xe7 ,0xbf ,0xa1/*0x120*/        ,(0xa1) ,0x11b ,0 ,0x105/*0x139*/       ,0x10d) // Color map coordinate y offset
  #define AWB_DBGAIN			DFSEL_Sens(0, 0xd       ,/*0x0d*/0x0e ,0xe  ,0xe/*0x10*/          ,(0xe)  ,0x10  ,0 ,0x15/*0x11*//*0xe*/  ,0x13 ) // Color map coordinate X gain
  #define AWB_DRGAIN			DFSEL_Sens(0, 0xc       ,/*0x0c*/0x0d ,0xc  ,0xa/*0xd*/           ,(0xd)  ,0xe   ,0 ,0xd                  ,0xe  ) // Color map coordinate y gain

  #define UpICSEL				DFSEL_Sens(1,3,3,1,1,1,1,1,1,1)		// ICSEL at the Flip Off
  #define UpOCSEL				DFSEL_Sens(1,1,0,2,2,2,1,1,2,1)		// OCSEL at the Flip Off
  #define UpICSEL_FLIP			DFSEL_Sens(1,3,3,1,1,1,1,1,1,1)		// ICSEL at the Flip On
  #define UpOCSEL_FLIP			DFSEL_Sens(1,1,0,2,2,2,1,1,2,1)		// OCSEL at the Flip On

#elif (model_2M30p)	// 2016330 - WHL


  #define UpAWB_HSP				DFSEL_Sens(0,   0xb,	  0,  0x1, 0x1,   0x1,0xb,0,0,0x1)						  // AWB detection area X start
  #define UpAWB_HW				0x3a8 																  // AWB detection area Y end
  #define UpAWB_VSP				0x10																	  // AWB detection area Y start
  #define UpAWB_VW				0x420 																  // AWB detection area Y end

  #define UpAWB_HSP_CROP		DFSEL_Sens(0,   0xb,	  0,  0x1, 0x1,   0x1,	0,0,0,0x1)						  // AWB detection area X start  in CROP mode
  #define UpAWB_HW_CROP			0x268 																  // AWB detection area Y end	 in CROP mode
  #define UpAWB_VSP_CROP		0x20																	  // AWB detection area Y start  in CROP mode
  #define UpAWB_VW_CROP			0x2b0 																  // AWB detection area Y end	 in CROP mode

  #define UpAWB_TCNT_LMT		0x1000		  // White pixel cnt limit
                    																					// 160115 HTY
  #define UpAWB_DBOFFSET		DFSEL_Sens(0, (0x4a+10) ,0x26  ,0x45  ,0x46   ,(0x51) ,0xb8   ,0,0x51	  ,0x80 )	  // Color map coordinate x offset
  #define UpAWB_DROFFSET		DFSEL_Sens(0, (0x98+10) ,0xc5  ,0xbf  ,0x118  ,(0xa1) ,0x11b  ,0,0x139	  ,0x10d)	  // Color map coordinate y offset
  #define AWB_DBGAIN			DFSEL_Sens(0, 0xd 	    ,0x0d  ,0xe	  ,0x10	  ,(0xe)  ,0x10   ,0,0xe	  ,0x13 )	  // Color map coordinate X gain
  #define AWB_DRGAIN			DFSEL_Sens(0, 0xc 	    ,0x0c  ,0xc	  ,0xd	  ,(0xd)  ,0xe	  ,0,0xd	  ,0xe	)	  // Color map coordinate y gain

  #define UpICSEL_FLIP			DFSEL_Sens(1,2,1,1,2,1,1,1,1,1)	// ICSEL at the Flip On
  #define UpICSEL				DFSEL_Sens(1,2,0,1,2,1,1,1,3,1)	// ICSEL at the Flip Off
  #define UpOCSEL_FLIP			DFSEL_Sens(1,2,1,1,2,1,1,1,1,1)	// OCSEL at the Flip On
  #define UpOCSEL				DFSEL_Sens(1,2,0,1,2,1,1,1,3,1)	// OCSEL at the Flip Off

#else

#endif

	#define UpAWB_AGC_MIN		0x120																	// Chroma AGC position for maximum weight
	#define AWB_AGC_MAX 		0x1e0																	// Chroma AGC position for minimum weight
	#define AWB_AGC_WGT_MAX 	0x100																	// Chroma maximum weight (100% = 0x100)
	#define UpAWB_AGC_WGT_MIN	0x20/*0x40*/																	// Chroma minimum weight

	#define UpAWB_SPEED_MTRX	0xf8										// Matrix tracking speed (Low is high speed, max=0xff)
	#define AWB_SPEED_GAIN		UP(UpAWB_SPEED_MTRX)						// RGB gain tracking Speed (Low is high speed)

	#define UpAWB_YSLICE		4/*0x40*/										// White Y slice
	#define UpAWB_YCLIP 		0x200										// White Y clip

	#define AWB_GAIN_DFLT		0x100										// RGB Gain default
	#define UpAWB_GAIN_TIMES	3																		// RGB Gain limit (X times)

	#define AWB_GAIN_MIN		(AWB_GAIN_DFLT/UP(UpAWB_GAIN_TIMES))									// RGB Gain min
	#define AWB_GAIN_MAX		(AWB_GAIN_DFLT*UP(UpAWB_GAIN_TIMES))									// RGB Gain max
	#define AWB_GAIN_STEP_H 	((float)(AWB_GAIN_MAX-AWB_GAIN_DFLT)/10.f)
	#define AWB_GAIN_STEP_L 	((float)(AWB_GAIN_DFLT-AWB_GAIN_MIN)/10.f)
	
	#define UpBY_Gain_Negative		BYGAINNr
	#define UpBY_Gain_Positive		BYGAINPr
	#define UpRY_Gain_Negative		RYGAINNr
	#define UpRY_Gain_Positive		RYGAINPr
	
	#define UpBY_Hue_Negative		BYHUENr
	#define UpBY_Hue_Positive		BYHUEPr
	#define UpRY_Hue_Negative		RYHUENr
	#define UpRY_Hue_Positive		RYHUEPr

#endif//__ISP_LIB__


//-------------------------------------------------------------------------------------------------
// Extern
extern void Awb(void);
extern void InitAwb(void);

extern UINT	gnAwbErr;		// AWB tracking finish state

//*************************************************************************************************
// 11. DEFECT
//-------------------------------------------------------------------------------------------------
// Misc

//-------------------------------------------------------------------------------------------------
// Extern
extern void DefectAuto(void);


#define UpDEFECT_DIFTH			0							//Within the pattern, this register uses to detect directional defect.Difference threshold for isolated point determination.

#define UpDEFECT_AGC_MIN		ADNR_AGC_MIN				//DEFECT AGC MIN	(0~500 AGC)
#define UpDEFECT_AGC_MID		ADNR_AGC_MID				//DEFECT AGC MID	(0~500 AGC)
#define UpDEFECT_AGC_MAX		ADNR_AGC_MAX				//DEFECT AGC MHI	(0~500 AGC)
#define UpDEFECT_GA				0x80						//DEFECT GAIN
#define UpDEFECT_SPOT_GA		0x80						//DEFECT SPOT GAIN

#define UpDEFECT_TSEL			0							// Select test image in core for directional defect correction. (VLOCKI)  "0" : Normal operation  "1" : Difference values for isolated points  "2" : Image before correction  "3" : Complete correction image (100% correction image except weight).  "4" : Defect pattern detection value


//*************************************************************************************************
// 12. SHADING
//-------------------------------------------------------------------------------------------------
// Misc

//-------------------------------------------------------------------------------------------------
// Extern
extern void InitShd(void);
extern void Shading(void);


//*************************************************************************************************
// 13. IMD (Motion detector)
//-------------------------------------------------------------------------------------------------
// Misc

//User Parameter
#define UpIMD_BG_UP			0x1
#define UpIMD_THRES_WEG		0x1

#define	UpIMD_BOX_Y			0x51
#define UpIMD_BOX_Cb		0x5a
#define UpIMD_BOX_Cr		0xef

#define UpIMD_BRIGHT_MAX	0x80
#define UpIMD_BRIGHT_MIN	0xf

#define UpIMD_MOVING_TH		0x4

#define	IMD_MASK_MODE		0							// 0 : Normal operation
														// 1 : Masking Mode (Masking 4-Area Non Detection)

#define IMD_HW				(FR_HW/32)
#define IMD_VW				((model_1M)?(FR_VW/32):(FR_VW/32+1)) //HTY 200150303


#define IMD_MAX_BOX_NUM 	16							// Max detection no.

#define	IMD_AGC_MIN			0x40						// Sensitivity Min Threshold
#define	IMD_AGC_MAX			0x200						// "		   Max "

#define	IMD_MVFILTER_SLOW	250 						// For camera moving detection
#define	IMD_MVFILTER_FAST   3 	    					// "
#define	IMD_MVFILTER_THRS   0x100						// "

#define IMD_BRIGHT_TH_MAX	(UP(UpIMD_BRIGHT_MAX))		// For camera Bright change Threshold Max
#define IMD_BRIGHT_TH_MIN   (-UP(UpIMD_BRIGHT_MIN))		// " 							Min

#define IMD_MDBOX_COLOR		((UP(UpIMD_BOX_Y)<<16)|(UP(UpIMD_BOX_Cb)<<8)|(UP(UpIMD_BOX_Cr)))	// Motion OSD Box Color	 (YCbCr)

#define IMD_MASK_SEL_ON		gbIMDMenuStep = 1			// Mask Selection Color On
#define IMD_MASK_SEL_OFF	gbIMDMenuStep = 0			// Mask Selection Color Off

#define MASK_SEL_COLOR		0xd21092					// Selected Mask

//Mask Mode
#if (IMD_MASK_MODE)
	#define IMD_MASK_TONE		0x1						// Masking Mode Tone [0-3]
	#define	IMD_MASK_COLOR		0x608080				// Motion Mask Box Color (YCbCr)
#else
	#define IMD_MASK_COLOR		0x903522				//Mask Box Color
#endif

#define MASK_EA				4							// number of Mask Box(max 4ea, CAUTION. share with IMD box ea)
#define IMD_SENS_MAX		10

typedef struct _IMD_RECT_N {
	unsigned int 	sx;
	unsigned int 	ex;
	unsigned int 	sy;
	unsigned int 	ey;
}nRECT;

extern nRECT DrawRectOSD[IMD_MAX_BOX_NUM]; 				// Position of Detected Motion

//------------------------------------------------------------------------------
// extern
extern unsigned int 	gnIMD_DrawBoxBit; 				// Dected Motion bit
//extern unsigned char	gbIMD_DrawBoxNum; 				// Dected Motion bit number
extern unsigned char	gbIMD_Moving; 					// Moving Flag - Moving or not

extern unsigned int		gnIMD_Hw;
extern unsigned int 	gnIMD_Vw;

extern UINT gnBoxPosIMD[IMD_MAX_BOX_NUM*2];				// Box display
extern UINT gnBoxPltIMD[IMD_MAX_BOX_NUM];				// "
extern UINT gnBoxToneIMD[2];                            // "
extern UINT gnBoxFillIMD;                               // "
extern UINT gnBoxOnIMD;                                 // "

extern BYTE gbIMDHold;
extern BYTE gbIMDMenuStep;								//Mask Selection Step

extern void	InitIMD(void);
extern void	IMD(void);


//*************************************************************************************************
// 14. ETC (ISP applications)
//-------------------------------------------------------------------------------------------------
// ADNR
extern int giIspAgc[3];
extern BYTE gbIspAgcSta;
extern BYTE gbIspAgcEnd;
extern void IspAgcSet(void);
extern void IspAgcCurSet(void);

#define ADNR_AGC_MIN		giIspAgc[0]		// Min AGC arg.
#define ADNR_AGC_MID		giIspAgc[1]		// Mid AGC arg.
#define ADNR_AGC_MAX		giIspAgc[2]		// Max AGC arg.

#define ADNR_AGC_STA		giIspAgc[gbIspAgcSta]
#define ADNR_AGC_END		giIspAgc[gbIspAgcEnd]

#define Up3D_DNR_GAIN_LOW		0x80
#define Up3D_DNR_GAIN_MIDDLE	0x80
#define Up3D_DNR_GAIN_HIGH		0x80

extern void Adnr(void);

#define UpHLMASK_THRS_STEP	0xA/*7*/	//HLMASK threshold menu step	(0 ~ 0xA)
#define UpHLMASK_CY			100	//HLMASK CY of Color			(0 ~ 0xFF)
#define UpHLMASK_CB			100	//HLMASK CB of Color			(0 ~ 0xFF)
#define UpHLMASK_CR			100	//HLMASK CR of Color			(0 ~ 0xFF)

//------------------------------------------------------------------------------
// Sharpness
#define	UpSHARPNESS_GAIN		5		// Aperture weight
//#define	UpSHARPNESS_GAIN		3		// 141209

extern void Sharpness(void);

//------------------------------------------------------------------------------
// Color surpression
//HTY 20150613
#define UpLSUP_ON	1/*0*//*1*/
#define	UpLSUP_GA	/*0xff*/0x80
#define	UpLSUP_TH	0/*1*/

#define UpCSUP_ON	1
#define	UpCSUP_GA	0x80
#define CSUP_GA		0x20/*0x10*/

#define UpMpCES_ON		2
#define UpMpWdrCES_ON	20

#if (model_1M)			//         0    1    2    3    4    5    6    7    8    9
	#define UpCSUP_TH 	DFSEL_Sens(0x74,0x74,0x74,0x74,0x78,0x74,0x84,0x74,0x74,0x74)
	#define UpCSUP_WTH 	DFSEL_Sens(0x7a,0x78,0x74,0x74,0x78,0x74,0x74,0x74,0x74,0x74)

	#define UpCES_GAIN 	DFSEL_Sens(0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10)
	#define UpCES_WGAIN DFSEL_Sens(0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20)
	
	#define UpCES_LCLP 	DFSEL_Sens(0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14)
	#define UpCES_WLCLP DFSEL_Sens(0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A)

#elif (model_2M) // 2016331 - WHL  0    1    2    3    4    5    6    7    8    9
	#define UpCSUP_TH 	DFSEL_Sens(0x7a,0x78,0x74,0x74,0x74,0x70,0x74,0x74,0x74,0x74)
	#define UpCSUP_WTH 	DFSEL_Sens(0x7a,0x78,0x74,0x74,0x74,0x70,0x74,0x74,0x74,0x74)	// 190308 KSH - SENS_IMX327 0x30 -> 0x74
	
	#define UpCES_GAIN 	DFSEL_Sens(0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10)
	#define UpCES_WGAIN DFSEL_Sens(0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20)
	
	#define UpCES_LCLP 	DFSEL_Sens(0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14)
	#define UpCES_WLCLP DFSEL_Sens(0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A)
	
#elif (model_2M30p)		//         0    1    2    3    4    5    6    7    8    9
	#define UpCSUP_TH 	DFSEL_Sens(0x7a,0x78,0x74,0x74,0x74,0x74,0x74,0x74,0x74,0x74)	// 150720 K-PS1210:0x8a->0x74
	#define UpCSUP_WTH 	DFSEL_Sens(0x7a,0x78,0x74,0x74,0x74,0x74,0x74,0x74,0x74,0x74)

	#define UpCES_GAIN 	DFSEL_Sens(0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10)
	#define UpCES_WGAIN DFSEL_Sens(0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20)
	
	#define UpCES_LCLP 	DFSEL_Sens(0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x14)
	#define UpCES_WLCLP DFSEL_Sens(0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A,0x0A)

#endif

extern void CSup(void);

//------------------------------------------------------------------------------
// Highlight Mask
extern void HlMask(void);

//------------------------------------------------------------------------------
// Mirro / Flip
extern void Mirror(void);

extern void Flip(void);

//------------------------------------------------------------------------------
// Color bar
extern void ColorBar(void);

//------------------------------------------------------------------------------
// Color space
extern void ColorSpace(void);

//------------------------------------------------------------------------------
// ETC
extern void DoorBell_PowerCtr(void); // DANNY_180716
extern void PIP_Disp(void);
extern void Lvds_Adjust(void);
extern BYTE gbLvdsOn;

//------------------------------------------------------------------------------
// Baudrate config.
extern void UartBaudRate(void);

//------------------------------------------------------------------------------
// Box Privacy
#ifndef __ISP_LIB__

#define BOX_TONE_100	0
#define BOX_TONE_50		1
#define BOX_TONE_25		2
#define BOX_TONE_0		3

#define BOX_ON(ID)	BOSD_ONw(BOSD_ONr | (1<<ID))
#define BOX_OFF(ID)	BOSD_ONw(BOSD_ONr & (~(1<<ID)))

#define BOX_COLOR(ID,Y,CB,CR)	{\
	BOX##ID##_PLTw( (((Y)&0xFF)<<16) | (((CB)&0xFF)<<8) | ((CR)&0xFF) );\
}

#define BOX_ATT(ID,FILL,TONE,Y,CB,CR) {\
	BFL_ON##ID##w(FILL);\
	BTONE##ID##w(TONE);\
	BOX_COLOR(ID,Y,CB,CR)\
}

#define BOX(ID,SX,SY,EX,EY)	{\
	B##ID##_HSPw(SX);\
	B##ID##_HEDw(EX);\
	B##ID##_VSPw(SY);\
	B##ID##_VEDw(EY);\
}

//	#define PVC_EA				16//32			// number of Privacy Box (max 32ea, CAUTION. share with IMD box ea)
#define	PVC_HSP				(MP(MpMirror))?(0x0):(0x10)			// Start position of Privacy Block	// EN778
#define	PVC_VSP				0x6				// "

#define BOX_MVPXL_MIN		((FR_HW/255)+1)				//Move Pixel Min Value by 1 Step
#define BOX_MVPXL_MAX		(FR_VW)					//Move Pixel Max Value by 1 Step

#if (model_1M)
	#define UpBOX_MV_PXL	0x20		//0x6 ~ 0x2DO Box Move Pixel(1.3M)
#else
	#define UpBOX_MV_PXL	0x20		//0x8 ~ 0x438 Box Move Pixel(2M)
#endif

#define BOX_XMAX			(FR_HW/UP(UpBOX_MV_PXL))											//Menu Box Position Control X Max Value
#define BOX_YMAX			(model_1M)?(FR_VW/UP(UpBOX_MV_PXL)):(FR_VW/UP(UpBOX_MV_PXL)+1)		//Menu Box Position Control Y Max Value

#define UpBOX_COLOR_STEP	0x14		//0x1 ~ 0xFF	Color Step Divide

#define BOX_REV_POS(YPOS,XPOS)	if(MP(MpFlip))		YPOS = ((VWOr - (YPOS & BitMask_16))<<16)|(VWOr - (ValSft_R16(YPOS)));\
								else				YPOS = YPOS;\
								if(MP(MpMirror))	XPOS = ((HWOr - (XPOS & BitMask_16))<<16)|(HWOr - (ValSft_R16(XPOS)));\
								else				XPOS = XPOS;\

extern void BoxLast(void);
					

//------------------------------------------------------------------------------
// Frame Mode.
extern void FrameRateLvSet(void);
extern void FrameModeSet(void);


#endif//__ISP_LIB__
//*************************************************************************************************
// 15. FOCUS ( Focus Contorl Method )
//-------------------------------------------------------------------------------------------------
#define UpTUNING_MODE_EN    0
#define UpFOCUS_TARGET      0
#define UpZOOM_TARGET       0
#define UpCapture           0

// Focus Assist
#if (model_Lens==0)

	#define Focus		Focus_Assist()

	#define FADJ_EA		19

	extern void Focus_Assist(void);

	extern UINT gnBoxPosFADJ[];					// Box display
	extern UINT gnBoxPltFADJ[];					// "
	extern const UINT gnBox_BFL_FADJ;           // "
	extern UINT gnBoxToneFADJ[];         		// "			// 150325 WHL
	extern BYTE gbOzone_Hold;

#elif (model_Lens==1)	// 20171107 - WHL

#define Focus	Motorized_AF()

	extern void Motorized_AF(void);
	extern void MAF_PhaseControl(void);
	extern BYTE gbOzone_Hold, gbMaf_TimerEn,gbMAF_Calibration,gbMAF_DevMode,gbMAF_KeyBuf,gbAutoFocus_Flag;

#define MAF_BOX_START	27 // Box No.29~31
	
	extern UINT gnBoxPosMAF_Y[5];
	extern UINT gnBoxPosMAF_X[5];
	extern UINT gnBoxPltMAF[5];
	extern UINT gnBoxToneMAF;
	extern UINT gnBoxFillMAF;
	extern UINT gnBoxOnMAF;
	extern BYTE gbMAFBoxFlag;


#endif


//*************************************************************************************************
// 16. CPU Common Processing
//-------------------------------------------------------------------------------------------------



//*************************************************************************************************
// 17. DBG (Debugging & Testing)
//-------------------------------------------------------------------------------------------------
// Misc
//#define DispDbgYmin		1
//#define DispDbgX		24
//#define DispDbgStrLen	11

	extern void DispDbgH(char* cStr, const int aiVal, const UINT anLen);
	extern void DispDbgD(char* cStr, const int aiVal, const UINT anLen);
	extern void DispDbgB(char* cStr, const int aiVal, const UINT anLen);
	extern void DispDbgP(char* cStr, const int aiVal, const int aiValMax, const UINT anLen);

	extern void DispHex(const UINT anDispY, const UINT anDispX, const UINT anVal, const UINT anLen);
	extern void DispDec(const UINT anDispY, const UINT anDispX, const int aiVal, const UINT anLen);
	extern void DispBin(const UINT anDispY, const UINT anDispX, const UINT anVal, const UINT anLen);
	extern void DispPer(const UINT anDispY, const UINT anDispX, const int aiVal, const int aiValMax, const UINT anLen);

	extern void DispDatHex(char* cStr, const UINT anDispY, const UINT anDispX, const UINT anVal);
	extern void DispDatDec(char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal);
	extern void DispDatPer(char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal, const int aiValMax);

	extern void DispDatDec2(const BYTE abON, char* cStr, const UINT anDispY, const UINT anDispX, const int aiVal, const UINT anLen);

	#define DispDat		DispDatHex
	#define DebugDisp(ON, TYPE, STR, Y, X, VAL, ...)	{if(ON) DispDat##TYPE( STR, Y, X, VAL );}	// Debug display by Debug mode

	#define DebugDisp2(ON, TYPE, STR, Y, X, VAL, LEN)	DispDatDec2( ON, STR, Y, X, VAL, LEN);		// Debug display by Debug mode


#define TICK_STA(ID)	const UINT nTickSta_##ID = GetTT()
#define TICK_END(ID)	UINT nTick_##ID = GetTT();\
						nTick_##ID = (nTick_##ID > nTickSta_##ID) ? nTick_##ID - nTickSta_##ID :\
																	GetTTmax() - nTickSta_##ID + nTick_##ID + 1
// micro-sec = TICK / MCLK * 1000000
//           = TICK / 74250000 * 1000000
//           = TICK * 1000000 / 74250000
//           = TICK * 100 / 7425
//           = TICK * 4 / 297
//           = TICK * 4 / (MCLK / 250000)
//#define tick2us(ID)		((nTick_##ID*4   ) / (MCLK/250000))
//#define tick2ns(ID)		((nTick_##ID*4000) / (MCLK/250000))
#define tick2us(ID)		((nTick_##ID*10   ) / (CLK_VALUE*27))

#define TICK_OUT(ID,fname,fout) \
	/*if(nTick_##ID <= (0xFFFFFFFF/4000))	{ fout(fname" ns:", tick2ns(ID), 10); }\
	else*/								{ fout(fname" us:", tick2us(ID), 10); }


#define FUNC_TIME(ID,func) \
	TICK_STA(ID);\
	func;\
	TICK_END(ID);\

#define FUNC_TIME_DI(ID,func) \
	(void)(ID);\
	DI;\
	FUNC_TIME(ID,func)\
	EI;

#define FUNC_TIME_OUT_ID(AD,ID,fname,fout,func)		FUNC_TIME##AD(ID,func)	TICK_OUT(ID,fname,fout)

#define FUNC_TIME_TX(fname,func)					FUNC_TIME_OUT_ID(_DI,__LINE__,fname,TxStrDec,func)
//#define FUNC_TIME_DISP(fname,func)					FUNC_TIME_OUT_ID(_DI,__LINE__,fname,DispDbgD,func)
#define FUNC_TIME_TX_NoIRQ(fname,func)				FUNC_TIME_OUT_ID(,__LINE__,fname,TxStrDecNoIRQ,func)

#define xFUNC_TIME_TX(fname,func)					func
//#define xFUNC_TIME_DISP(fname,func)					func
#define xFUNC_TIME_TX_NoIRQ(fname,func)				func

#define FUNC_TIME_GET(ID)							tick2us(ID)		// micro-sec

// for MainVDO & MainVDI
#define FUNC_TIME_START(ID)							TICK_STA(ID)
#define FUNC_TIME_END(ID,fname)						TICK_END(ID); if(STATUS_ON) {TICK_OUT(ID,fname,DispDbgD)}

#define FUNC_TIME_OSD_ID(ON,ID,fname,Y,X,func)		{ FUNC_TIME(ID,func) DebugDisp2(ON, Dec, #fname" us", Y, X,  tick2us(ID), 6); }
#define FUNC_TIME_OSD(ON,fname,Y,X,func)			FUNC_TIME_OSD_ID(ON,__LINE__,fname,Y,X,func)
#define xFUNC_TIME_OSD(fname,Y,X,func)				func

//-------------------------------------------------------------------------------------------------
// Extern
extern void TxStrMemAddrInfo(void);

#if STR_UART_DATA == 1
extern void DispUartData(void);
#else
#define DispUartData()
#endif

extern void TxStrWaitTime(const UINT anUs);
extern void TestWaitTime(const UINT anUs);
extern void DispTime(UINT anSec);
extern void TestAppSavePar(void);
extern void DispIsrStatus(void);

extern UINT gnDispDbgY;

extern UINT gnWaitTimeOffset;
extern UINT gnWaitTimeUnit;

extern char gbStr[ISP_FONT_MAX_PATH+1+1];


/* EOF */

