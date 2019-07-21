/***************************************************************************
 File Name	: 	sfls.c
 Description:	EN673 - SFLS Device Driver
 Designer	:	YoungKyu, Jang
 Date		:	14. 9. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */
#include "dev.h"
#include "dev_inline.h"

BYTE FlsRead(BYTE *apbUser, UINT apbFlsAddr, UINT anNum)
{
	DmaMemCpy_ip(apbUser, (BYTE *)apbFlsAddr, anNum);
	invalidate_dcache_range((uint)apbUser, (uint)(apbUser+anNum));
	return DEF_OK;
}

#if 0
BYTE FlsReadHL(BYTE *apbUser, UINT apbFlsAddr, UINT anNum)
{	// SFLS_BASE = 0x06000000, 128Mb위치는 0x07000000
	// Dma으로 접근하는 플래시메모리의 Address는 0x06000000 ~ 0x06FFFFFF이므로
	if(apbFlsAddr+anNum >= SFLS_BASE + 0x01000000)	// 만약 0x07000000 까지 접근해야 한다면,
	{
		if(apbFlsAddr >= SFLS_BASE + 0x01000000)	// 시작부터 0x07000000이라면,
		{
			SflsENHBL();							// enter high bank latch mode
			DmaMemCpy_ip(apbUser-0x01000000, (BYTE *)apbFlsAddr, anNum);
			SflsEXHBL();							// exit high bank latch mode
		}
		else
		{
			UINT pos = 0x07000000 - apbFlsAddr;		// low bank에서 read해야할 size
			UINT remain = anNum - pos;				// high bank에서 read해야할 size
			DmaMemCpy_ip(apbUser, (BYTE *)apbFlsAddr, pos);
			SflsENHBL();							// enter high bank latch mode
			DmaMemCpy_ip(apbUser-0x01000000, (BYTE *)apbFlsAddr+pos, remain);
			SflsEXHBL();							// exit high bank latch mode
		}
	}
	else
	{
		DmaMemCpy_ip(apbUser, (BYTE *)apbFlsAddr, anNum);
	}
	invalidate_dcache_range((uint)apbUser, (uint)(apbUser+anNum));
	return DEF_OK;
}
#endif

BYTE FlsWrite(UINT apbFlsAddr, const BYTE *apbUser, UINT anNum)
{
	DmaMemCpy_ip((BYTE *)apbFlsAddr, (BYTE *)apbUser, anNum);
	return DEF_OK;
}

void FlsErase(UINT apbFlsAddr)
{
	SflsEraseSect(apbFlsAddr);
}

void SflsWriteEn(void)
{
	SFLS_REG_CMD = 0x06;
	SFLS_CMD = 0x00020000 | ((SFLS_WB_RDCMD_IOM&0x3)<<8);
	while(SFLS_REG_REQ);
}

#if 0
void Sfls_ENSO(void)
{
	SFLS_REG_CMD = 0xB1;
	SFLS_CMD = 0x00020000;
	while(SFLS_REG_REQ);
}

void Sfls_EXSO(void)
{
	SFLS_REG_CMD = 0xC1;
	SFLS_CMD = 0x00020000;
	while(SFLS_REG_REQ);
}
#endif

void SflsEraseSect(UINT Adr)
{
	SflsWriteEn();
	SFLS_REG_CMD = 0x20;
	SFLS_REG_ADR = (Adr & 0x00ffffff);
	SFLS_CMD = 0x00024800 | ((SFLS_WB_RDCMD_IOM&0x3)<<8) | ((SFLS_WB_RDCMD_IOM&0x3)<<6);
	while(SFLS_REG_REQ);
#if 0
	int i=0;
	UINT *p = (UINT*)( SFLS_BASE | (Adr&(~((4*1024)/*FLASH_SECT_SIZE*/-1))) );
	for(i=0; i<(0x1000>>2); i++);
	{
		*p++ = 0xFFFFFFFF;
	}
#else
	const UINT nAddr = SFLS_BASE | (Adr&(~((4*1024)/*FLASH_SECT_SIZE*/-1)));
	const UINT nStop = nAddr + 0x1000;

	invalidate_dcache_range(nAddr, nStop);
#endif
}

void SflsEraseBlock(UINT Adr)
{
	SflsWriteEn();
	SFLS_REG_CMD = 0xd8;
	SFLS_REG_ADR = (Adr & 0x00ffffff);
	SFLS_CMD = 0x00024800 | ((SFLS_WB_RDCMD_IOM&0x3)<<8) | ((SFLS_WB_RDCMD_IOM&0x3)<<6);
	while(SFLS_REG_REQ);
}

void SflsEraseAll(void)
{
	SflsWriteEn();
	SFLS_REG_CMD = 0xC7;
	SFLS_CMD = 0x00020800 | ((SFLS_WB_RDCMD_IOM&0x3)<<8);
	while(SFLS_REG_REQ);
}

void SflsWriteReg(UCHAR dat)
{
	SflsWriteEn();
	SFLS_REG_CMD = 0x01;
	SFLS_DATIN = (dat<<24);
	SFLS_CMD = 0x00021c00 | ((SFLS_WB_RDCMD_IOM&0x3)<<8) | ((SFLS_WB_RDCMD_IOM&0x3)<<4);
	while(SFLS_REG_REQ);
}

BYTE SflsReadReg(void)
{
	SFLS_REG_CMD = 0x05;
	SFLS_CMD = 0x00021000 | ((SFLS_WB_RDCMD_IOM&0x3)<<8) | ((SFLS_WB_RDCMD_IOM&0x3)<<4);
	while(SFLS_REG_REQ);
	return (SFLS_DATOUT>>24)&0xFF;
}

#ifdef DEF_BOOT

#define UART_STR printf

#define SFLS_SPEED	(0x50000000)	// 0x0xxxxxxx ~ 0xFxxxxxxx

ISRT void Sfls_Winbond(BYTE Type, BYTE Capa)
{
	switch(Type)
	{
		case 0x40:	//	QUAD
		{
			switch(Capa)
			{
				case 0x16:	// W25Q32FV
				{
					UART_STR("W25Q32FV-noTEST");
					break;
				}
				case 0x17:	// W25Q64FV Test OK(18.01.05)
				{
					UART_STR("W25Q64FV, W25Q64JVIQ-QUAD");

					SFLS_REG_CMD = 0x06;					// write enable(0x06)
					SFLS_CMD = SFLS_SPEED | 0x0020000;		// WE + MCK/2
					while(SFLS_REG_REQ);

					SFLS_REG_CMD = 0x01;					// write states for enable QE bit with busy check
					SFLS_DATIN = 0x00020000;				// QE
					SFLS_CMD = SFLS_SPEED | 0x0021C01;
					while(SFLS_REG_REQ);

					SFLS_WB_RDCMD = 0xEB;					// EQPI + QUAD
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x3876AAAA;				// Enable QPI command(original:0x387AAAAA)
					SFLS_CMD = SFLS_SPEED | 0x0030000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				case 0x18:	// W25Q128FV Test OK(17.01.09)
				{
					UART_STR("W25Q128FV-QUAD");

					SFLS_REG_CMD = 0x35;					// Read status register 2
					SFLS_CMD = SFLS_SPEED | 0x0021000;
					while(SFLS_REG_REQ);

					if((SFLS_DATOUT & 0x02000000) != 0x02000000)
					{
						SFLS_REG_CMD = 0x06;
						SFLS_CMD = SFLS_SPEED | 0x0020000;	// WE + MCK/2
						while(SFLS_REG_REQ);

						SFLS_REG_CMD = 0x31;				// write states for enable QE bit with busy check
						SFLS_DATIN = 0x02000000;			// QE
						SFLS_CMD = SFLS_SPEED | 0x0021C01;
						while(SFLS_REG_REQ);
					}

					SFLS_WB_RDCMD = 0xEB;					// EQPI + QUAD
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x387AAAAA;				// Enable QPI command
					SFLS_CMD = SFLS_SPEED | 0x0030000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				default:
				{
					UART_STR("Winbond QUAD Etc...");
					break;
				}
			}
			break;
		}
		default:
		{
			UART_STR("Winbond Etc...");
			break;
		}
	}
}

ISRT void Sfls_EON(BYTE Type, BYTE Capa)
{
	switch(Type)
	{
		case 0x30:	// QUAD
		{
			switch(Capa)
			{
				case 0x16:	// EN25Q32
				{
					UART_STR("EN25Q32-noTEST");
					break;
				}
				case 0x17:	// EN25Q64
				{
					UART_STR("EN25Q64-noTEST");
					break;
				}
				case 0x18:	// EN25Q128
				{
					UART_STR("EN25Q128-QUAD");

					SFLS_WB_RDCMD = 0xEB;					// EQPI + QUAD
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x387EAAAA;
					SFLS_CMD = SFLS_SPEED | 0x0030000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				default:
				{
					UART_STR("EON QUAD Etc...");
					break;
				}
			}
			break;
		}
		case 0x70:	// new QUAD
		{
			switch(Capa)
			{
				case 0x16:	// EN25QH32A
				{
					UART_STR("EN25QH32A-noTEST");
					break;
				}
				case 0x17:	// EN25QH64A
				{
					UART_STR("EN25QH64A-noTEST");
					break;
				}
				case 0x18:	// EN25QH128A Test OK(16.09.29)
				{
					UART_STR("EN25QH128A-QUAD");

					SFLS_WB_RDCMD = 0xEB;					// EQPI + QUAD
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x387EAAAA;
					SFLS_CMD = SFLS_SPEED | 0x0030000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				default:
				{
					UART_STR("EON QUAD Etc...");
					break;
				}
			}
			break;
		}
		default:
		{
			UART_STR("EON Etc...");
			break;
		}
	}
}

ISRT void Sfls_Macronix(BYTE Type, BYTE Capa)
{
	switch(Type)
	{
		case 0x20:	// QUAD
		{
			switch(Capa)
			{
				case 0x17:	// MX25L6433F Test OK(16.10.10)
				{
					UART_STR("MX25L6433F-QUAD");

					SFLS_REG_CMD = 0x05;					// Read status register
					SFLS_CMD = SFLS_SPEED | 0x0021000;
					while(SFLS_REG_REQ);

					if((SFLS_DATOUT & 0x40000000) != 0x40000000)
					{
						SFLS_REG_CMD = 0x06;				// Write enable
						SFLS_CMD = SFLS_SPEED | 0x0020000;
						while(SFLS_REG_REQ);

						SFLS_REG_CMD = 0x01;				// Write status register for QE
						SFLS_DATIN = 0x40000000;
						SFLS_CMD = SFLS_SPEED | 0x0021C01;	// with busy check
						while(SFLS_REG_REQ);
					}

					SFLS_WB_RDCMD = 0xEB;					// 4bit Read command
					SFLS_WB_WRCMD = 0x38;					// 4bit Page program
					SFLS_WBCONT = 0x007CA0A0;				// Read(Cmd 1bit, Dat 4bit, Gap 1byte), Write (Cmd 1bit, Data 4 bit), Write Enabe (Cmd 1bit)
					SFLS_CMD = SFLS_SPEED | 0x0010000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				case 0x18:	// MX25L12835F Test OK(17.01.09)
				{
					UART_STR("MX25L12835F-QUAD");

					SFLS_REG_CMD = 0x05;					// Read status register
					SFLS_CMD = SFLS_SPEED | 0x0021000;
					while(SFLS_REG_REQ);

					if((SFLS_DATOUT & 0x40000000) != 0x40000000)
					{
						SFLS_REG_CMD = 0x06;				// Write enable
						SFLS_CMD = SFLS_SPEED | 0x0020000;
						while(SFLS_REG_REQ);

						SFLS_REG_CMD = 0x01;				// Write status register for QE
						SFLS_DATIN = 0x40000000;
						SFLS_CMD = SFLS_SPEED | 0x0021C01;	// with busy check
						while(SFLS_REG_REQ);
					}

					SFLS_WB_RDCMD = 0xEB;					// 4bit Read command
					SFLS_WB_WRCMD = 0x38;					// 4bit Page program
					SFLS_WBCONT = 0x007CA0A0;				// Read(Cmd 1bit, Dat 4bit, Gap 1byte), Write (Cmd 1bit, Data 4 bit), Write Enabe (Cmd 1bit)
					SFLS_CMD = SFLS_SPEED | 0x0010000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				default:
				{
					UART_STR("Macronix QUAD Etc...");
					break;
				}
			}
			break;
		}
		default:
		{
			UART_STR("Macronix Etc...");
			break;
		}
	}
}

ISRT void Sfls_ISSI(BYTE Type, BYTE Capa)
{
	switch(Type)
	{
		case 0x60:	// QUAD
		{
			switch(Capa)
			{
				case 0x17:	// IS25PL064 Test OK(16.11.08)
				{
					UART_STR("IS25PL064-QUAD");

					SFLS_REG_CMD = 0x05;					// Read status register
					SFLS_CMD = SFLS_SPEED | 0x0021000;
					while(SFLS_REG_REQ);

					if((SFLS_DATOUT & 0x40000000) != 0x40000000)
					{
						SFLS_REG_CMD = 0x06;				// Write enable
						SFLS_CMD = SFLS_SPEED | 0x0020000;
						while(SFLS_REG_REQ);

						SFLS_REG_CMD = 0x01;				// Write status register for QE
						SFLS_DATIN = 0x40000000;
						SFLS_CMD = SFLS_SPEED | 0x0021C00;	// with busy check
						while(SFLS_REG_REQ);
					}

					SFLS_WB_RDCMD = 0xEB;					// 4bit Read command
					SFLS_WB_WRCMD = 0x38;					// 4bit Page program
					SFLS_WBCONT = 0x007CA020;				// Read(Cmd 1bit, Adr 4bit, Dat 4bit, Gap 3clock), Write(Cmd 1bit, Adr 1bit, Data 4 bit), Write Enabe (Cmd 1bit)
					SFLS_CMD = SFLS_SPEED | 0x0010000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				case 0x18:	// IS25LP128 Test OK(16.12.29)
				{
					UART_STR("IS25LP128-QUAD");

					SFLS_REG_CMD = 0x05;					// Read status register
					SFLS_CMD = SFLS_SPEED | 0x0021000;
					while(SFLS_REG_REQ);

					if((SFLS_DATOUT & 0x40000000) != 0x40000000)
					{
						SFLS_REG_CMD = 0x06;				// Write enable
						SFLS_CMD = SFLS_SPEED | 0x0020000;
						while(SFLS_REG_REQ);

						SFLS_REG_CMD = 0x01;				// Write status register for QE
						SFLS_DATIN = 0x40000000;
						SFLS_CMD = SFLS_SPEED | 0x0021C00;	// with busy check
						while(SFLS_REG_REQ);
					}

					SFLS_WB_RDCMD = 0xEB;					// 4bit Read command
					SFLS_WB_WRCMD = 0x38;					// 4bit Page program
					SFLS_WBCONT = 0x007CA020;				// Read(Cmd 1bit, Adr 4bit, Dat 4bit, Gap 3clock), Write(Cmd 1bit, Adr 1bit, Data 4 bit), Write Enabe (Cmd 1bit)
					SFLS_CMD = SFLS_SPEED | 0x0010000;
					while(SFLS_REG_REQ);

					UART_STR("-Done");
					break;
				}
				default:
				{
					UART_STR("ISSI QUAD Etc...");
					break;
				}
			}
			break;
		}
		default:
		{
			UART_STR("ISSI Etc...");
			break;
		}
	}
}

void SflsInit(void)
{
	// Set Flash Pad Power Strength => 8mA
	PAD_SFLS_CS_DS = 1;
	PAD_SFLS_DQ0_DS = 1;
	PAD_SFLS_DQ1_DS = 1;
	PAD_SFLS_DQ2_DS = 1;
	PAD_SFLS_DQ3_DS = 1;
	PAD_SFLS_CLK_DS = 1;
	PAD_SFLS_DQ_PDEN = 0xC;

	// Get Flash Info
	SFLS_REG_CMD = 0x9F;
	SFLS_CMD = 0xF0021002;
	while(SFLS_REG_REQ);
	BYTE Manu = (SFLS_DATOUT & 0xFF000000)>>24;
	BYTE Type = (SFLS_DATOUT & 0x00FF0000)>>16;
	BYTE Capa = (SFLS_DATOUT & 0x0000FF00)>>8;
	UART_STR("SerialFlash : MANU(%x) TYPE(%x) CAPA(%x) ", Manu, Type, Capa);

	switch(Manu)
	{
		case 0x1C:	Sfls_EON(Type, Capa);		break;	// EON
		case 0x9D:	Sfls_ISSI(Type, Capa);		break;	// ISSI
		case 0xC2:	Sfls_Macronix(Type, Capa);	break;	// Macronix
		case 0xEF:	Sfls_Winbond(Type, Capa);	break;	// Winbond
		default :	//	ETC
		{
			UART_STR("ETC sfls");
			SFLS_CMD = 0x10020000;
			while(SFLS_REG_REQ);
			break;
		}
	}
	UART_STR("\r\n");
	return;
#if 0
		case 0x8C:	//	ESMT
		{
			switch(Type)
			{
				case 0x40:	//	QUAD
				{
					switch(Capa)
					{
						case 0x14:{UART_STR("F25L08QA-QUAD");break;}	//	F25L08QA
						case 0x15:{UART_STR("F25L16QA-QUAD");break;}	//	F25L16QA
					}
					SFLS_REG_CMD = 0x06;
					SFLS_CMD = 0x10020000;		//	WE + MCK/2
					while(SFLS_REG_REQ);

					SFLS_REG_CMD = 0x01;
					SFLS_DATIN = 0x40000000;	//	QE
					SFLS_CMD = 0x10021C00;
					while(SFLS_REG_REQ);

					SFLS_WB_RDCMD = 0xEB;		//	QUAD
					SFLS_WB_WRCMD = 0x32;
					SFLS_WBCONT = 0x007CA0A0;
					SFLS_CMD = 0x10010000; // 0x10030000
					while(SFLS_REG_REQ);

					break;
				}
				case 0x41:	//	QUAD
				{
					switch(Capa)
					{
						case 0x16:{UART_STR("F25L32QA-QUAD");break;}	//	F25L32QA
					}

					SFLS_REG_CMD = 0x06;
					SFLS_CMD = 0x10020000;		//	WE + MCK/2
					while(SFLS_REG_REQ);

					SFLS_REG_CMD = 0x01;
					SFLS_DATIN = 0x40000000;	//	QE
					SFLS_CMD = 0x10021C00;
					while(SFLS_REG_REQ);

					SFLS_WB_RDCMD = 0xEB;		//	QUAD
					SFLS_WB_WRCMD = 0x32;
					SFLS_WBCONT = 0x007CA0A0;
					SFLS_CMD = 0x10010000; // 0x10030000
					while(SFLS_REG_REQ);

					break;
				}
				case 0x30:	//	DUAL
				{
					switch(Capa)
					{
						case 0x10:{UART_STR("F25L05PA-DUAL");break;}	//	F25L05PA
						case 0x11:{UART_STR("F25L01PA-DUAL");break;}	//	F25L01PA
						case 0x12:{UART_STR("F25L02PA-DUAL");break;}	//	F25L02PA
						case 0x13:{UART_STR("F25L04PA-DUAL");break;}	//	F25L04PA
					}

					SFLS_WB_RDCMD = 0x3B;		//	DUAL
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x00741000;
					SFLS_CMD = 0x10010000;
					while(SFLS_REG_REQ);

					break;
				}
				case 0x20:	//	DUAL
				{
					switch(Capa)
					{
						case 0x16:{UART_STR("F25L32PA-DUAL");break;}	//	F25L32PA
						case 0x15:{UART_STR("F25L16PA-DUAL");break;}	//	F25L16PA
						case 0x14:{UART_STR("F25L08PA-DUAL");break;}	//	F25L08PA
						case 0x13:{UART_STR("F25L04PA-DUAL");break;}	//	F25L04PA
						case 0x12:{UART_STR("F25L02PA-DUAL");break;}	//	F25L02PA
						case 0x11:{UART_STR("F25L01PA-DUAL");break;}	//	F25L01PA
						case 0x10:{UART_STR("F25L05PA-DUAL");break;}	//	F25L05PA
					}
					break;
				}
				case 0x21:	//	DUAL
				{
					switch(Capa)
					{
						case 0x15:{UART_STR("F25L16PA-DUAL");break;}	//	F25L16PA
					}

					SFLS_WB_RDCMD = 0x3B;		//	DUAL
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x00741000;
					SFLS_CMD = 0x10010000;
					while(SFLS_REG_REQ);

					break;
				}
			}
			break;
		}
		case 0xBF:	//	SST
		{
			switch(Type)
			{
				case 0x25:	//	QUAD
				{
					switch(Capa)
					{
						case 0x8D:{UART_STR("SST25VF040B-SINGLE");break;}	//	SST25VF040B
						case 0x8E:{UART_STR("SST25VF080B-SINGLE");break;}	//	SST25VF080B
						case 0x41:{UART_STR("SST25VF016B-SINGLE");break;}	//	SST25VF016B
						case 0x4A:{UART_STR("SST25VF032B-SINGLE");break;}	//	SST25VF032B
						case 0x4B:{UART_STR("SST25VF064C-SINGLE");break;}	//	SST25VF064C
					}

					SFLS_REG_CMD = 0x50;
					SFLS_CMD = 0x10020000;		//	WE + MCK/2
					while(SFLS_REG_REQ);

					SFLS_WB_RDCMD = 0x0B;		//	High speed Read Cmd
					SFLS_WB_WRCMD = 0x02;
					SFLS_WBCONT = 0x00740000;
					SFLS_CMD = 0x10010000;	//
					while(SFLS_REG_REQ);

					SFLS_REG_CMD = 0x01;
					SFLS_DATIN = 0x00000000;	//	Delete Write Protection
					SFLS_CMD = 0x10021C00;
					while(SFLS_REG_REQ);

					break;
				}
				case 0x26:	//	SINGLE
				{
					switch(Capa)
					{
						case 0x01:	//	SST26VF016
						case 0x02:	//	SST26VF032
						{
							if(Capa==0x01) UART_STR("SST26VF016-SINGLE");
							else if(Capa==0x02) UART_STR("SST26VF032-SINGLE");

							SFLS_REG_CMD = 0x50;
							SFLS_CMD = 0x10020000;		//	WE + MCK/2
							while(SFLS_REG_REQ);

							SFLS_WB_RDCMD = 0x0B;		//	High speed Read Cmd
							SFLS_WB_WRCMD = 0x02;
							SFLS_WBCONT = 0x00740000;
							SFLS_CMD = 0x10010000;	//
							while(SFLS_REG_REQ);

							SFLS_REG_CMD = 0x01;
							SFLS_DATIN = 0x00000000;	//	Delete Write Protection
							SFLS_CMD = 0x10021C00;
							while(SFLS_REG_REQ);

							break;
						}
					}
					break;
					break;
				}
			}
			break;
		}
#endif
}
#endif
