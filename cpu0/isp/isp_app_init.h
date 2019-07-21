/* **************************************************************************
 File Name	: 	app_init.h
 Description:	initialize functions
 Designer	:	Lee, Wanhee
 Date		:	16. 12. 03
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

volatile BYTE gbSensRstPort=0;	// Sensor Reset Port Selection (*Defalut 0)
volatile BYTE gbSensIfMode=0;	// 0 : LVDS, 1 : Parallel (Image Data I/F)
volatile BYTE gbIspClkMode=0;	// 0 : 148.5MHz, 1 : 162Mhz (ISP Clock Speed)
//	volatile BYTE __attribute__((optimize("O0"))) gbSensCntMode=0;	// 0 : SPI , 1 : TWI (Control Reg I/F)
volatile BYTE gbSensCtrMode=0;	// 0 : SPI , 1 : TWI (Control Reg I/F)

const WORD gwTblSens0_R[][2] = {		// 27MHz - 720/60P
						{0x0249, 0x0A},

						{0x0205, 0x01},	// AD12bit
						{0x0207, 0x10}, // HD720P
						{0x0209, 0x01}, // 60P Mode

						{0x020A, 0x00},
						{0x020B, 0x00},
						{0x020F, 0x00},
						{0x0212, 0x2C},
						{0x0213, 0x01},
						{0x0216, 0x09},
						{0x0218, 0xEE},
						{0x0219, 0x02},
						{0x021B, 0xC8},
						{0x021C, 0x19},
						{0x021D, 0xC2},
						{0x0244, 0xE1},	// LVDS 4CH 12bit

						{0x0246, 0x01},
						{0x0247, 0x03},
						{0x0248, 0xC0},

						{0x0249, 0x0A},

						{0x025C, 0x20},	// 37.125Mhz
						{0x025D, 0x00},
						{0x025E, 0x20},
						{0x025F, 0x00},

						{0x0270, 0x02},
						{0x0271, 0x01},
						{0x029E, 0x22},
						{0x02A5, 0xFB},
						{0x02A6, 0x02},
						{0x02B3, 0xFF},
						{0x02B4, 0x01},
						{0x02B5, 0x42},
						{0x02B8, 0x10},
						{0x02C2, 0x01},

						{0x030F, 0x0F},
						{0x0310, 0x0E},
						{0x0311, 0xE7},
						{0x0312, 0x9C},
						{0x0313, 0x83},
						{0x0314, 0x10},
						{0x0315, 0x42},
						{0x0328, 0x1E},
						{0x03ED, 0x38},

						{0x040C, 0xCF},
						{0x044C, 0x40},
						{0x044D, 0x03},
						{0x0461, 0xE0},
						{0x0462, 0x02},
						{0x046E, 0x2F},
						{0x046F, 0x30},
						{0x0470, 0x03},
						{0x0498, 0x00},
						{0x049A, 0x12},
						{0x049B, 0xF1},
						{0x049C, 0x0C}};


const UINT gnDdrRWdrLtc[2] = {0x0a0, 0x1a0};


extern void InitIrq(void);
extern void InitWdt(UINT sec);
extern void InitTickTimer(const UINT anCntPerSec);

extern void UsrParReset(void);
extern void InitPar(void);

extern void Digital_interface_set(void);


/* EOF */

