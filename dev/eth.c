//*************************************************************************************************
//
//*************************************************************************************************

#include "dev.h"
#include "phy.h"
#include "msg.h"

#if defined(__ETH__)
void MdioRead(BYTE phyadr, BYTE regadr, WORD *data)
{
	ETH_MDIO_OPCODE = 2;
	ETH_MDIO_PHYADR = phyadr;
	ETH_MDIO_REGADR = regadr;
	ETH_MDIO_STR = 1;
	while(ETH_MDIO_STR);
	*data = ETH_MDIO_RXDAT;
}

void MdioWrite(BYTE phyadr, BYTE regadr, WORD data)
{
	ETH_MDIO_OPCODE = 1;
	ETH_MDIO_PHYADR = phyadr;
	ETH_MDIO_REGADR = regadr;
	ETH_MDIO_TXDAT = data;
	ETH_MDIO_STR = 1;
	while(ETH_MDIO_STR);
}

void MdioInit(void)
{
	PAD_ETH_MDIO_PUEN = 1;

#define MDIO_CLK (960*1000)	// 960KHz
	ETH_MDIO_CLKLMT = (BUS_CLK / (MDIO_CLK * 4)) - 1;
	ETH_MDIO_CLKEN = 0;		//	MDIO_CLK Enable 	0: Just on Cmd		1:	Always On
}

void EthSetMacAdr(BYTE *apdMacAdr)
{	// Mac address set
	ETH_RX_MACADR0 = (apdMacAdr[0]<<8) | (apdMacAdr[1]<<0);
	ETH_RX_MACADR1 = (apdMacAdr[2]<< 24) | (apdMacAdr[3]<< 16) |(apdMacAdr[4]<< 8) | (apdMacAdr[5]<< 0);
}

void EthDuplexChange(UINT duplex)
{
	if (duplex == PHY_DUPLEX_HALF) {
//		ETH_TX_CRSCHK = 1;
//		ETH_TX_COLCHK = 1;
//		ETH_TX_RTYEN  = 1;
//		ETH_TX_RTYLMT = 3;
//		ETH_TX_RTY_EN = 1;
//		ETH_TX_RTY_LMT = 16;
	} else {
//		ETH_TX_CRSCHK = 1;	// 선로상에 신호를 검출
//		ETH_TX_COLCHK = 1;	//
	}
}

void EthRxTxinit(UINT type, UINT speed, UINT duplex)
{
	if (type == PHY_TYPE_10_100) { // 10/100Mbps
		ETH_TX_IFGGAP	=	ETH_MAC_TX_IFGGAP;	//	100Mbps(9:960ns 10:1040ns) / 10Mbps(9:9.6us 10:10.4us)
		ETH_TX_DATTYPE	=	0;	//	TX Dat Type(0:SDR(MII))
		ETH_TX_CLKSEL	=	0;
		ETH_TX_CLKOE	=	0;

		ETH_RX_DATTYPE	=	0;	//	RX Dat Type(0:SDR(MII))

		if (speed == PHY_SPD_100) {
			ETH_RX_ERTYPE = 0;
		} else if (speed == PHY_SPD_10) {
			ETH_RX_ERTYPE = 2;
		}

	// setting
		ETH_TX_CLKEDGE	=	0;
		ETH_TX_TCKDLY	=	0;
		ETH_TX_TXENDLY	=	0;
		ETH_TX_TXD0DLY	=	0;
		ETH_TX_TXD1DLY	=	0;
		ETH_TX_TXD2DLY	=	0;
		ETH_TX_TXD3DLY	=	0;

		ETH_RX_RCKEDGE	=	0;
		ETH_RX_RCKDLY	=	7;

		if (duplex == PHY_DUPLEX_HALF) {
			ETH_TX_IFGGAP = 255;

			ETH_TX_CRSCHK = 1;	// HALF인 경우에만 설정
			ETH_TX_COLCHK = 1;	// HALF인 경우에만 설정

			ETH_TX_RTYEN  = 1;	// HALF인 경우에만 설정
			ETH_TX_RTYLMT = 5;	// HALF인 경우에만 설정
		} else {
			ETH_TX_IFGGAP = ETH_MAC_TX_IFGGAP;

			ETH_TX_CRSCHK = 0;	// HALF인 경우에만 설정
			ETH_TX_COLCHK = 0;	// HALF인 경우에만 설정

			ETH_TX_RTYEN  = 0;	// HALF인 경우에만 설정
			ETH_TX_RTYLMT = 0;	// HALF인 경우에만 설정
		}
	} else if(type == PHY_TYPE_1000) { // 1000Mbps
		ETH_TX_IFGGAP	=	ETH_MAC_TX_IFGGAP;	//	9:960ns 10:1040ns / 9:9.6us 10:10.4us

		ETH_TX_CLKSEL	=	1;
		ETH_TX_CLKOE	=	1;

		ETH_RX_ERTYPE	=	1;

	// setting
		ETH_TX_CLKEDGE	=	0;//1;
		ETH_TX_TCKDLY	=	13;//6;
		ETH_TX_TXENDLY	=	0;
		ETH_TX_TXD0DLY	=	0;
		ETH_TX_TXD1DLY	=	0;
		ETH_TX_TXD2DLY	=	0;
		ETH_TX_TXD3DLY	=	0;

		if (speed == PHY_SPD_1000) {
			ETH_TX_DATTYPE	=	1;	//	TX Dat Type(1:DDR(RGMII))
			ETH_RX_DATTYPE	=	1;	//	RX Dat Type(1:DDR(RGMII))

			ETH_RX_RCKEDGE	=	0;//1;
			ETH_RX_RCKDLY	=	8;//0;
		} else if (speed == PHY_SPD_100) {
			ETH_TX_DATTYPE	=	0;	//	TX Dat Type(0:SDR(MII))
			ETH_RX_DATTYPE	=	0;	//	RX Dat Type(0:SDR(MII))

			ETH_RX_RCKEDGE	=	0;
			ETH_RX_RCKDLY	=	1;
		} else if (speed == PHY_SPD_10) {
			ETH_TX_DATTYPE	=	0;	//	TX Dat Type(0:SDR(MII))
			ETH_RX_DATTYPE	=	0;	//	RX Dat Type(0:SDR(MII))

			ETH_RX_RCKEDGE	=	0;
			ETH_RX_RCKDLY	=	1;
		}

		if (duplex == PHY_DUPLEX_HALF) {
			ETH_TX_RTYEN  = 0;
			ETH_TX_RTYLMT = 0;
//			ETH_TX_RTYEN  = 1;
//			ETH_TX_RTYLMT = 3;
		} else {
			ETH_TX_RTYEN  = 0;
			ETH_TX_RTYLMT = 0;
		}
	} else {
		printf("Phy type error(%d)\r\n", type);
		while(1);
	}
}

void EthInit(void)
{
	PAD_ETH_TXD3_DS = 1;
	PAD_ETH_TXD2_DS = 1;
	PAD_ETH_TXD1_DS = 1;
	PAD_ETH_TXD0_DS = 1;
	PAD_ETH_TXEN_DS = 1;

	ETH_RX_EN		= 1;				//	ETH RX enable
	EthSetMacAdr((BYTE *)gptMsgShare.ETH_MAC);
	ETH_RX_MACFILT	= 1;				//	Mac Address Filter	0:	Off 	1:	Active (RX_MACADR==PACKET RXMAC)
	EthMulFiltClear();
	EthMulFiltDisable();
	ETH_RX_CRCFILT	= 1;				//	CRC Filter			0:	Off 	1:	Active (CRC Failed packet will rejected)
	ETH_TX_CRSCHK	= 0;				//
	ETH_TX_COLCHK	= 0;				//
	ETH_TX_RTYEN	= 0;				//
	ETH_TX_RTYLMT	= 0;				//
	ETH_RX_IRQEN	= 1;				//	Not RX Empty IRQ
	ETH_TX_IRQEN	= 0;				//	Not TX Full IRQ
}

void EthMulFiltEnable(void)
{	//	Multicast Filter Enable 0~11
	ETH_RX_MUL0FILT = 1;
	ETH_RX_MUL1FILT = 1;
	ETH_RX_MUL2FILT = 1;
	ETH_RX_MUL3FILT = 1;
	ETH_RX_MUL4FILT = 1;
	ETH_RX_MUL5FILT = 1;
	ETH_RX_MUL6FILT = 1;
	ETH_RX_MUL7FILT = 1;
	ETH_RX_MUL8FILT = 1;
	ETH_RX_MUL9FILT = 1;
	ETH_RX_MUL10FILT = 1;
	ETH_RX_MUL11FILT = 1;
}

void EthMulFiltDisable(void)
{	//	Multicast Filter Disable 0~11
	ETH_RX_MUL0FILT = 0;
	ETH_RX_MUL1FILT = 0;
	ETH_RX_MUL2FILT = 0;
	ETH_RX_MUL3FILT = 0;
	ETH_RX_MUL4FILT = 0;
	ETH_RX_MUL5FILT = 0;
	ETH_RX_MUL6FILT = 0;
	ETH_RX_MUL7FILT = 0;
	ETH_RX_MUL8FILT = 0;
	ETH_RX_MUL9FILT = 0;
	ETH_RX_MUL10FILT = 0;
	ETH_RX_MUL11FILT = 0;
}

void EthMulFiltClear(void)
{	// Multicast Filter Clear 0~11
	ETH_RX_MUL11ADR = 0;
	ETH_RX_MUL10ADR = 0;
	ETH_RX_MUL9ADR = 0;
	ETH_RX_MUL8ADR = 0;
	ETH_RX_MUL7ADR = 0;
	ETH_RX_MUL6ADR = 0;
	ETH_RX_MUL5ADR = 0;
	ETH_RX_MUL4ADR = 0;
	ETH_RX_MUL3ADR = 0;
	ETH_RX_MUL2ADR = 0;
	ETH_RX_MUL1ADR = 0;
	ETH_RX_MUL0ADR = 0;
}

void EthMulFiltInsert(UINT mulIp)
{	// 01:00:5E:00:00:00 ~ 01:00:5E:7F:FF:FF
	ETH_RX_MUL11ADR|= 1<<0x0;	// 0
	ETH_RX_MUL10ADR|= 1<<0x1;	// 1
	ETH_RX_MUL9ADR |= 1<<0x0;	// 0
	ETH_RX_MUL8ADR |= 1<<0x0;	// 0
	ETH_RX_MUL7ADR |= 1<<0x5;	// 5
	ETH_RX_MUL6ADR |= 1<<0xE;	// E
	ETH_RX_MUL5ADR |= (1<<((mulIp&0x700000)>>20)); // x
	ETH_RX_MUL4ADR |= (1<<((mulIp&0x0f0000)>>16)); // x
	ETH_RX_MUL3ADR |= (1<<((mulIp&0x00f000)>>12)); // x
	ETH_RX_MUL2ADR |= (1<<((mulIp&0x000f00)>>8));	// x
	ETH_RX_MUL1ADR |= (1<<((mulIp&0x0000f0)>>4));	// x
	ETH_RX_MUL0ADR |= (1<<((mulIp&0x00000f)>>0));	// x
}

void Eth8021DFiltInsert(UINT gmac)
{	// IEEE Std 802.1D and IEEE Std 802.1Q Reserved Addresses / 01:80:C2:00:00:00 ~ 0F
	ETH_RX_MUL11ADR|= 1<<0x0;	// 0
	ETH_RX_MUL10ADR|= 1<<0x1;	// 1
	ETH_RX_MUL9ADR |= 1<<0x8;	// 8
	ETH_RX_MUL8ADR |= 1<<0x0;	// 0
	ETH_RX_MUL7ADR |= 1<<0xC;	// C
	ETH_RX_MUL6ADR |= 1<<0x2;	// 2
	ETH_RX_MUL5ADR |= 1<<0x0;	// 0
	ETH_RX_MUL4ADR |= 1<<0x0;	// 0
	ETH_RX_MUL3ADR |= 1<<0x0;	// 0
	ETH_RX_MUL2ADR |= 1<<0x0;	// 0
	ETH_RX_MUL1ADR |= 1<<0x0;	// 0
	ETH_RX_MUL0ADR |= (1<<((gmac&0xf)>>0));	// 0~F
}
#endif
