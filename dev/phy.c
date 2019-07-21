#include "dev.h"
#include "dev_inline.h"

#ifdef __ETH__
#include "user.h"
#include "phy.h"

#ifdef DEF_CPU1
#include "FreeRTOS.h"
#include "task.h"
#include <stdlib.h>			// use to rand
#endif

#include "lwip/netif.h"		// use to netif_set_up, _down
#include "lwip/netifapi.h"

#ifdef DEF_CPU1
extern struct netif gtMyNetIf;
#endif

#if (ETH_SHELL_TEST==1)
#define PHY_DEBUG 1
#else
#define PHY_DEBUG 0
#endif

PhyInfo phy_info;

UINT PhyReset(void)
{
	WORD wBuf, nLoop = 0, nCk;

#if PHY_MDIO_TEST
	UINT i, nCnt, nGetAddr = -1;
	vTaskDelay(100);
	printf("Ethernet PHY MDIO Address Test.. Idle 10sec\r\n");
	for (i = 0; i < 20; i++) {
		printf(".");
		vTaskDelay(50);
	}
	printf("Test Start!\r\n");
	for (i = 0; i < 32; i++) {
		nCnt = 0;
		phy_info.addr = i;

		MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_RESET);		// PHY reset
		do {
			vTaskDelay(1);
			MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);
		} while (wBuf & PHY_BCR_RESET && ++nCnt <= 100);
		printf("Phy address(%02d), wBuf(%d), nCnt(%d) ", phy_info.addr, wBuf, nCnt);

		if (!(wBuf & PHY_BCR_RESET)) {
			printf(" - ok\r\n");
			nGetAddr = i;
		} else {
			printf(" - fail\r\n");
		}

		vTaskDelay(1);
	}
	if (nGetAddr != -1) {
		phy_info.addr = nGetAddr;
		printf("Ethernet PHY MDIO Address(%d)\r\n", phy_info.addr);
	} else {
		printf("Ethernet PHY MDIO Error! Not Ready!\r\n");
		return DEF_FAIL;
	}
#endif

	MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_RESET);		// PHY reset
	for (nLoop = 0, nCk = 0; nLoop < 100; nLoop++) {
		MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);
		if (!(wBuf & PHY_BCR_RESET)) {
			nCk = 1;
			break;
		}
#ifdef DEF_CPU0
		WaitXms(10);
#endif
#ifdef DEF_CPU1
		vTaskDelay(1);
#endif
	}

	if (nCk == 1) {
		return DEF_OK;
	} else {
		printf("Ethernet PHY RESET Fail?\r\n");
	}

	return DEF_FAIL;
}

BOOL PhyLinkState(void)
{
	WORD wBuf, i;
	for (i = 0; i < 100; i++) {
		MdioRead(phy_info.addr, PHY_BSR_ADR, &wBuf);
		if (wBuf & PHY_BSR_LS) {
			return DEF_ON;
		}
#ifdef DEF_CPU0
		WaitXms(10);
#endif
#ifdef DEF_CPU1
		vTaskDelay(1);
#endif
	}
	PhyReset();	//	fail? sw reset
	return DEF_OFF;
}

#if PHY_DEBUG
void PhyRegView(UINT Type, WORD Data)
{
	printf("========================================\r\n");
	switch (Type) {
		case PHY_BCR_ADR:
			printf("Type:PHY_BCR_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %s\r\n", "Reset", Data & PHY_BCR_RESET ? "Software reset" : "Normal operation");
			printf("%26s: %s\r\n", "Power-Down", Data & PHY_BCR_PWR_DOWN ? "Power-down mode" : "Normal operation");
			printf("%26s: %s\r\n", "Loopback", Data & PHY_BCR_LOOP_BACK ? "Loopback mode" : "Normal operation");
			printf("%26s: %s\r\n", "Isolate", Data & PHY_BCR_PWR_DOWN ? "Electrical isolation of PHY from I/F" : "Normal operation");
			printf("%26s: %s auto-negotiation process\r\n", "Auto-Negotiation Enable", Data & PHY_BCR_AUTONEG_EN ? "Enable" : "Disable");
			printf("%26s: %s\r\n", "Restart Auto-Negotiation", Data & PHY_BCR_RST_AUTONEG ? "Restart auto-negotiation process" : "Normal operation");
			printf("%26s: %s\r\n", "Speed Select", (Data & PHY_BCR_SPEED_M ? (Data & PHY_BCR_SPEED_L ? "Reserved" : "1000Mbps") : (Data & PHY_BCR_SPEED_L ? "100Mbps" : "10Mbps")));
			printf("%26s: %s\r\n", "Duplex Mode", Data & PHY_BCR_FULLDPLX ? "Full-duplex" : "Half-duplex");
			break;
		case PHY_BSR_ADR:
			printf("Type:PHY_BSR_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %sT4 capable\r\n", "100Base-T4", Data & PHY_BSR_100BT4 ? "" : "Not ");
			printf("%26s: %sapable of 100Mbps full-duplex\r\n", "100Base-TX Full-Duplex", Data & PHY_BSR_100BTXFD ? "C" : "Not c");
			printf("%26s: %sapable of 100Mbps half-duplex\r\n", "100Base-TX Half-Duplex", Data & PHY_BSR_100BTXHD ? "C" : "Not c");
			printf("%26s: %sapable of 10Mbps full-duplex\r\n", "10Base-T Full-Duplex", Data & PHY_BSR_10BTFD ? "C" : "Not c");
			printf("%26s: %sapable of 10Mbps half-duplex\r\n", "10Base-T Half-Duplex", Data & PHY_BSR_10BTHD ? "C" : "Not c");
#if defined(__ETH_KSZ9031RNX__)
			printf("%26s: %sxtended status info in Reg. 15h.\r\n", "Extended Status", Data & PHY_BSR_ES ? "E" : "No e");
#endif
			printf("%26s: %s\r\n", "No Preamble", Data & PHY_BSR_NP ? "Preamble suppression" : "Normal preamble");
			printf("%26s: Auto-negotiation process %scompleted\r\n", "Auto-Negotiation Complet", Data & PHY_BSR_AUTONEG_CMPLT ? "" : "not ");
			printf("%26s: %semote fault\r\n", "Remote Fault", Data & PHY_BSR_RF ? "R" : "No r");
			printf("%26s: Can%s perform auto-negotiation\r\n", "Auto-Negotiation Ability", Data & PHY_BSR_AUTONEG_ABLE ? "" : "not");
			printf("%26s: Link is %s\r\n", "Link Status", Data & PHY_BSR_LS ? "up" : "down");
			printf("%26s: Jabber %sdetect\r\n", "Jabber Detect", Data & PHY_BSR_JD ? "" : "not ");
			printf("%26s: %s\r\n", "Extended Capability", Data & PHY_BSR_EC ? "Supports extended capability registers" : "?");
			break;
		case PHY_PI1_ADR:
			printf("Type:PHY_PI1_ADR : (0x%04X)\r\n", Data);
//			if (Data & PHY_)					printf("	PHY\r\n");
			break;
		case PHY_PI2_ADR:
			printf("Type:PHY_PI2_ADR : (0x%04X)\r\n", Data);
//			if (Data & PHY_)					printf("	PHY\r\n");
			break;
		case PHY_ANAR_ADR:
			printf("Type:PHY_ANAR_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %s\r\n", "Next Page", Data & PHY_ANAR_NP ? "Next page capable" : "No next page capability");
			printf("%26s: %s\r\n", "Remote Fault", Data & PHY_ANAR_RF ? "Remote fault supported" : "No remote fault");
			printf("%26s: %s\r\n", "Pause", (Data & PHY_ANAR_ASYPAUSE ? (Data & PHY_ANAR_SYPAUSE ? "Symmetric and asymmetric pause (local device)" : "Asymmetric pause (link partner)") : (Data & PHY_ANAR_SYPAUSE ? "Symmetric pause" : "No pause")));
			printf("%26s: %s\r\n", "100Base-T4", Data & PHY_ANAR_100BT4 ? "T4 capable" : "No T4 capability");
			printf("%26s: %s\r\n", "100Base-TX Full-Duplex", Data & PHY_ANAR_100BTXFD ? "100Mbps full-duplex capable" : "No 100Mbps full-duplex capability");
			printf("%26s: %s\r\n", "100Base-TX Half-Duplex", Data & PHY_ANAR_100BTXHD ? "100Mbps half-duplex capable" : "No 100Mbps half-duplex capability");
			printf("%26s: %s\r\n", "10Base-T Full-Duplex", Data & PHY_ANAR_10BTFD ? "10Mbps full-duplex capable" : "No 10Mbps full-duplex capability");
			printf("%26s: %s\r\n", "10Base-T Half-Duplex", Data & PHY_ANAR_10BTHD ? "10Mbps half-duplex capable" : "No 10Mbps half-duplex capability");
			printf("%26s: %05b\r\n", "Selector Field", Data & PHY_ANAR_SF); 
			break;
		case PHY_ANLPAR_ADR:
			printf("Type:PHY_ANLPAR_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %s\r\n", "Next Page", Data & PHY_ANLPAR_NPAGE ? "Next page capable" : "No next page capability");
			printf("%26s: Link code word %s\r\n", "Acknowledge", Data & PHY_ANLPAR_LPACK ? "received from partner" : "not yet received");
			printf("%26s: %s\r\n", "Remote Fault", Data & PHY_ANLPAR_RFAULT ? "Remote fault detected" : "No remote fault");
			printf("%26s: %s\r\n", "Pause", (Data & PHY_ANLPAR_ASYPAUSE ? (Data & PHY_ANLPAR_SYPAUSE ? "Symmetric and asymmetric pause (local device)" : "Asymmetric pause (link partner)") : (Data & PHY_ANLPAR_SYPAUSE ? "Symmetric pause" : "No pause")));
			printf("%26s: %s\r\n", "100Base-T4", Data & PHY_ANLPAR_100BASE4 ? "T4 capable" : "No T4 capability");
			printf("%26s: %s\r\n", "100Base-TX Full-Duplex", Data & PHY_ANLPAR_100FULL ? "100Mbps full-duplex capable" : "No 100Mbps full-duplex capability");
			printf("%26s: %s\r\n", "100Base-TX Half-Duplex", Data & PHY_ANLPAR_100HALF ? "100Mbps half-duplex capable" : "No 100Mbps half-duplex capability");
			printf("%26s: %s\r\n", "10Base-T Full-Duplex", Data & PHY_ANLPAR_10FULL ? "10Mbps full-duplex capable" : "No 10Mbps full-duplex capability");
			printf("%26s: %s\r\n", "10Base-T Half-Duplex", Data & PHY_ANLPAR_10HALF ? "10Mbps half-duplex capable" : "No 10Mbps half-duplex capability");
			printf("%26s: %05b\r\n", "Selector Field", Data & PHY_ANLPAR_SLCT);
			break;
		case PHY_ANER_ADR:
			printf("Type:PHY_ANER_ADR : (0x%04X)\r\n", Data);
//			if (Data & PHY_)					printf("	PHY\r\n");
			break;
		case PHY_ANNP_ADR:
			printf("Type:PHY_ANNP_ADR : (0x%04X)\r\n", Data);
//			if (Data & PHY_)					printf("	PHY\r\n");
			break;
		case PHY_ANLPNPA_ADR:
			printf("Type:PHY_ANLPNPA_ADR : (0x%04X)\r\n", Data);
//			if (Data & PHY_)					printf("	PHY\r\n");
			break;
#if defined(__ETH_KSZ9031RNX__)
		case PHY_1000C_ADR:
			printf("Type:PHY_1000C_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %03b\r\n", "Test Mode Bits", Data & PHY_GIGACR_TMB);
			printf("%26s: %sable master-slave manual configuration value\r\n", "Master-Slave Manual Configuration Enable", Data & PHY_GIGACR_MSMCE ? "En" : "Dis");
			printf("%26s: Configure PHY as %s during master-slave negotiation\r\n", "Master-Slave Manual Configuration Value", Data & PHY_GIGACR_MSMCV ? "master" : "slave");
			printf("%26s: Indicate the preference to operate as %s\r\n", "Port Type", Data & PHY_GIGACR_PT ? "multiport device (master)" : "single-port device (slave)");
			printf("%26s: Advertise PHY is %s1000Base-T full-duplex capable\r\n", "1000Base-T Full-Duplex", Data & PHY_GIGACR_1000FULL ? "" : "not ");
			printf("%26s: Advertise PHY is %s1000Base-T half-duplex capable\r\n", "1000Base-T Half-Duplex", Data & PHY_GIGACR_1000HALF ? "" : "not ");
			break;
		case PHY_1000S_ADR:
			printf("Type:PHY_1000S_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %saster-slave configuration fault detected\r\n", "Master-Slave Configuration Fault", Data & PHY_GIGASR_MSCF ? "M" : "No m");
			printf("%26s: Local PHY configuration resolved to %s\r\n", "Master-Slave Configuration Resolution", Data & PHY_GIGASR_MSCR ? "master" : "slave");
			printf("%26s: Local receiver %sOK\r\n", "Local Receiver Status", Data & PHY_GIGASR_LRS ? "" : "not ");
			printf("%26s: Remote receiver %sOK\r\n", "Remote Receiver Status", Data & PHY_GIGASR_RRS ? "" : "not ");
			printf("%26s: Link partner is %scapable of 1000Base-T full-duplex\r\n", "Link Partner 1000Base-T Full-Duplex Capability", Data & PHY_GIGASR_1000FULLC ? "" : "not ");
			printf("%26s: Link partner is %scapable of 1000Base-T half-duplex\r\n", "Link Partner 1000Base-T Half-Duplex Capability", Data & PHY_GIGASR_1000HALFC ? "" : "not ");
			printf("%26s: %d\r\n", "Idle Error Count", Data & PHY_GIGASR_IEC);
			break;
		case PHY_ESR_ADR:
			printf("Type:PHY_ESR_ADR : (0x%04X)\r\n", Data);
			printf("%26s: PHY can%s perform 1000Base-X full-duplex\r\n", "1000Base-X Full-Duplex", Data & PHY_ESR_1000XFULL ? "" : "not");
			printf("%26s: PHY can%s perform 1000Base-X half-duplex\r\n", "1000Base-X Half-Duplex", Data & PHY_ESR_1000XHALF ? "" : "not");
			printf("%26s: PHY can%s perform 1000Base-T full-duplex\r\n", "1000Base-T Full-Duplex", Data & PHY_ESR_1000TFULL ? "" : "not");
			printf("%26s: PHY can%s perform 1000Base-T half-duplexs\r\n", "1000Base-T Half-Duplex", Data & PHY_ESR_1000THALF ? "" : "not");
			break;
#endif
#if defined(__ETH_KSZ8081MNX__)
		case PHY_PHYC1_ADR:
			printf("Type:PHY_PHYC1_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %s\r\n", "Enable Pause(Flow Control)", Data & PHY_PC1_EP ? "Flow control capable" : "No flow control capability");
			printf("%26s: Link is %s\r\n", "Link Status", Data & PHY_PC1_LS ? "up" : "down");
			printf("%26s: Polarity is %sreversed\r\n", "Polarity Status", Data & PHY_PC1_PS ? "" : "not ");
			printf("%26s: MDI%s\r\n", "MDI/MDI-X State", Data & PHY_PC1_MDI ? "-X" : "");
			printf("%26s: %s on receive differential pair\r\n", "Energy Detect", Data & PHY_PC1_ED ? "Signal present" : "NO signal detected");
			printf("%26s: PHY in %s\r\n", "PHY Isolate", Data & PHY_PC1_ISO ? "isolate mode" : "normal operation");
			printf("%26s: ", "Operation Mode Indication");
			if (Data & PHY_PC1_OMI) {
				if ((Data & PHY_PC1_OMI) == 0x1) {
					printf("10Base-T half-duplex\r\n");
				} else if ((Data & PHY_PC1_OMI) == 0x2) {
					printf("100Base-TX half-duplex\r\n");
				} else if ((Data & PHY_PC1_OMI) == 0x5) {
					printf("10Base-T full-duplex\r\n");
				} else if ((Data & PHY_PC1_OMI) == 0x6) {
					printf("100Base-TX full-duplex\r\n");
				} else {
					printf("Reserved\r\n");
				}
			} else {
				printf("Still in auto-negotiation\r\n");
			}
			break;
		case PHY_PHYC2_ADR:
			printf("Type:PHY_PHYC2_ADR : (0x%04X)\r\n", Data);
			printf("%26s: %s Auto MDI/MDI-X mode\r\n", "HP_MDIX", Data & PHY_PC2_HP_MDI ? "HP" : "Micrel");
			printf("%26s: MDI%s mode\r\n", "MDI/MDI-X Select", Data & PHY_PC2_MD_MDI ? "-X" : "");
			printf("%26s: %sable Auto MDI/MDI-X\r\n", "Pair Swap Disable", Data & PHY_PC2_PSD ? "Dis" : "En");
			printf("%26s: %s\r\n", "Force Link", Data & PHY_PC2_FL ? "Force link pass" : "Normal link operation");
			printf("%26s: %sable power saving\r\n", "Power Saving", Data & PHY_PC2_PS ? "En" : "Dis");
			printf("%26s: Interrupt pin active %s\r\n", "Interrupt Level", Data & PHY_PC2_IL ? "high" : "low");
			printf("%26s: %sable jabber counter\r\n", "Enable Jabber", Data & PHY_PC2_EJ ? "En" : "Dis");
			printf("%26s: %s\r\n", "LED Mode", (Data & PHY_PC2_LED) == 0x10 ? "LED0(Activity) LED1(Link)" : (Data & PHY_PC2_LED) == 0x00 ? "LED0(Link/Activity) LED1(Speed)" : "Reserved");
			printf("%26s: %sable transmitter\r\n", "Disable Transmitter", Data & PHY_PC2_DIS_TRANS ? "Dis" : "En");
			printf("%26s: %s\r\n", "Remote Loopback", Data & PHY_PC2_RLOOP ? "Remote (analog) loopback is enabled" : "Normal mode");
			printf("%26s: %sable SQE test\r\n", "Enable SQE Test", Data & PHY_PC2_SEQ ? "En" : "Dis");
			printf("%26s: %sable scrambler\r\n", "Disable Data Scrambling", Data & PHY_PC2_DDS ? "Dis" : "En");
			break;
#endif
	}
	printf("========================================\r\n");
}
#else
#define PhyRegView(a, b)
#endif

BOOL PhyLinkInfo(void)
{
	WORD wBCR = 0, wBSR = 0, wANLPAR = 0, w1000SR = 0, wESR = 0;
	UINT nLoop;

	for (nLoop = 0; nLoop < 50; nLoop++) {
		MdioRead(phy_info.addr, PHY_BSR_ADR, &wBSR); // Get Link Status from PHY status reg.
		PhyRegView(PHY_BSR_ADR, wBSR);

		if (wBSR & PHY_BSR_LS) { // Link is up
			nLoop = 0;
			break;
		}
#ifdef DEF_CPU0
		WaitXms(100);
#endif
#ifdef DEF_CPU1
		vTaskDelay(10);
#endif
	}

	if (nLoop) {
		if ((wBSR & PHY_BSR_LS) == 0) {
			printf("%s : Not linked.\r\n", __func__);
			phy_info.speed = PHY_SPD_0;
			phy_info.duplex = PHY_DUPLEX_UNKNOWN;
			return DEF_FAIL;
		}
	}

#if PHY_DEBUG
#if defined(__ETH_KSZ8081MNX__)
	WORD wTEST;
	MdioRead(phy_info.addr, PHY_PHYC1_ADR, &wTEST); // Read the PHY Control Register
	PhyRegView(PHY_PHYC1_ADR, wTEST);
	MdioRead(phy_info.addr, PHY_PHYC2_ADR, &wTEST); // Read the PHY Control Register
	PhyRegView(PHY_PHYC2_ADR, wTEST);
#endif
#endif

	MdioRead(phy_info.addr, PHY_BCR_ADR, &wBCR); // Read the PHY Control Register
	PhyRegView(PHY_BCR_ADR, wBCR);

	if ((wBCR & PHY_BCR_AUTONEG_EN) == PHY_BCR_AUTONEG_EN) { // If AutoNegotiation is enabled
		printf("%s : Auto-Negotiation is enabled\r\n", __func__);
		if ((wBSR & PHY_BSR_AUTONEG_CMPLT) == 0) { // If AutoNegotiation is not complete
			printf("%s : Auto-Negotiation is not complete\r\n", __func__);
			phy_info.speed = PHY_SPD_0;
			phy_info.duplex = PHY_DUPLEX_UNKNOWN;
			return DEF_FAIL; // AutoNegotitation in progress
		}

		if (wBSR & PHY_BSR_ES) {
			MdioRead(phy_info.addr, PHY_ESR_ADR, &wESR);
			if (wESR & (PHY_ESR_1000THALF | PHY_ESR_1000TFULL | PHY_ESR_1000XHALF | PHY_ESR_1000XFULL)) {
				phy_info.type = PHY_TYPE_1000;

				int nRRS = 0, nLRS = 0;
				for (nLoop = 0; nLoop < 50; nLoop++) {
					MdioRead(phy_info.addr, PHY_1000S_ADR, &w1000SR); // Get 1000Base-T Status from PHY status reg.
					PhyRegView(PHY_1000S_ADR, w1000SR);

					if(w1000SR & (PHY_GIGASR_1000HALFC | PHY_GIGASR_1000FULLC)) {
						if ((w1000SR & PHY_GIGASR_RRS) == 0) {
							nRRS++;
						} else if ((w1000SR & PHY_GIGASR_LRS) == 0) {
							nLRS++;
						} else {
							nRRS = nLRS = 0;
							break;
						}
					} else {
						break;
					}
#ifdef DEF_CPU0
					WaitXms(100);
#endif
#ifdef DEF_CPU1
					vTaskDelay(10);
#endif
				}

				int nFLG = 0;
				if (nRRS) {
					printf("%s : RRS error detected(%d)(0x%04X).\r\n", __func__, nRRS, w1000SR);
					nFLG = 1;
				}
				if (nLRS) {
					printf("%s : LRS error detected(%d)(0x%04X).\r\n", __func__, nLRS, w1000SR);
					nFLG = 1;
				}
				if (nFLG) {
					phy_info.speed = PHY_SPD_0;
					phy_info.duplex = PHY_DUPLEX_UNKNOWN;
					return DEF_FAIL;
				}
			} else {
				phy_info.type = PHY_TYPE_10_100;
			}
		} else {
			phy_info.type = PHY_TYPE_10_100;
		}

		if (w1000SR & PHY_ESR_1000TFULL) { // Select 1000 FULL
			phy_info.speed = PHY_SPD_1000;
			phy_info.duplex = PHY_DUPLEX_FULL;
		} else if (w1000SR & PHY_ESR_1000THALF) { // Select 1000 HALF
			phy_info.speed = PHY_SPD_1000;
			phy_info.duplex = PHY_DUPLEX_HALF;
		} else {
			MdioRead(phy_info.addr, PHY_ANLPAR_ADR, &wANLPAR);
			PhyRegView(PHY_ANLPAR_ADR, wANLPAR);

			if (wANLPAR & PHY_ANLPAR_100FULL) { // Select 100 FULL
				phy_info.speed = PHY_SPD_100;
				phy_info.duplex = PHY_DUPLEX_FULL;
			} else if (wANLPAR & PHY_ANLPAR_100HALF) { // Select 100 HALF
				phy_info.speed = PHY_SPD_100;
				phy_info.duplex = PHY_DUPLEX_HALF;
			} else if (wANLPAR & PHY_ANLPAR_10FULL) { // Select 10 FULL
				phy_info.speed = PHY_SPD_10;
				phy_info.duplex = PHY_DUPLEX_FULL;
			} else if (wANLPAR & PHY_ANLPAR_10HALF) { // Select 10 HALF
				phy_info.speed = PHY_SPD_10;
				phy_info.duplex = PHY_DUPLEX_HALF;
			} else {
				phy_info.autonego = PHY_AUTONEGO_ERROR;
				phy_info.speed = PHY_SPD_0;
				phy_info.duplex = PHY_DUPLEX_UNKNOWN;
				return DEF_FAIL;
			}
		}
		phy_info.autonego = PHY_AUTONEGO_OK;
	} else { // Auto-negotiation not enabled, get speed and duplex from BCR
		printf("%s : Auto-Negotiation not enabled\r\n", __func__);

		if (wBSR & PHY_BSR_ES) {
			MdioRead(phy_info.addr, PHY_ESR_ADR, &wESR);
			if (wESR & (PHY_ESR_1000THALF | PHY_ESR_1000TFULL | PHY_ESR_1000XHALF | PHY_ESR_1000XFULL)) {
				phy_info.type = PHY_TYPE_1000;

				int nRRS = 0, nLRS = 0;
				for (nLoop = 0; nLoop < 50; nLoop++) {
					MdioRead(phy_info.addr, PHY_1000S_ADR, &w1000SR); // Get 1000Base-T Status from PHY status reg.
//					PhyRegView(PHY_1000S_ADR, w1000SR);

					if(w1000SR & (PHY_GIGASR_1000HALFC | PHY_GIGASR_1000FULLC)) {
						if ((w1000SR & PHY_GIGASR_RRS) == 0) {
							nRRS++;
						} else if ((w1000SR & PHY_GIGASR_LRS) == 0) {
							nLRS++;
						} else {
							nRRS = nLRS = 0;
							break;
						}
					} else {
						break;
					}
#ifdef DEF_CPU0
					WaitXms(100);
#endif
#ifdef DEF_CPU1
					vTaskDelay(10);
#endif
				}

				int nFLG = 0;
				if (nRRS) {
					printf("%s : RRS error detected(%d)(0x%04X).\r\n", __func__, nRRS, w1000SR);
					nFLG = 1;
				}
				if (nLRS) {
					printf("%s : LRS error detected(%d)(0x%04X).\r\n", __func__, nLRS, w1000SR);
					nFLG = 1;
				}
				if (nFLG) { 
					phy_info.speed = PHY_SPD_0;
					phy_info.duplex = PHY_DUPLEX_UNKNOWN;
					return DEF_FAIL;
				}
			} else {
				phy_info.type = PHY_TYPE_10_100;
			}
		} else {
			phy_info.type = PHY_TYPE_10_100;
		}

		phy_info.autonego = PHY_AUTONEGO_NONE;
		if (wBCR & PHY_BCR_FULLDPLX) {
			phy_info.duplex = PHY_DUPLEX_FULL;
		} else {
			phy_info.duplex = PHY_DUPLEX_HALF;
		}

		if ((wBCR & PHY_BCR_SPEED_M) && (wBCR & PHY_BCR_SPEED_L) == 0) {
			phy_info.speed = PHY_SPD_1000;
		} else if ((wBCR & PHY_BCR_SPEED_M) == 0 && (wBCR & PHY_BCR_SPEED_L)) {
			phy_info.speed = PHY_SPD_100;
		} else if ((wBCR & PHY_BCR_SPEED_M) == 0 && (wBCR & PHY_BCR_SPEED_L) == 0) {
			phy_info.speed = PHY_SPD_10;
		} else {
			phy_info.speed = PHY_SPD_0;
			phy_info.duplex = PHY_DUPLEX_UNKNOWN;
			return DEF_FAIL;
		}
	}
	printf("%s : speed(%dMbps) duplex(%s)\r\n", __func__, phy_info.speed, phy_info.duplex == 1 ? "Half" : phy_info.duplex == 2 ? "Full" : "Unknown");

	return DEF_OK;
}

void PhyAutoNeg(int onoff)
{
	WORD wBuf = 0;

#if (ETH_MAC_PAUSE)
	MdioRead(phy_info.addr, PHY_ANAR_ADR, &wBuf);	// Auto-Negotiation Advertisement: Read
	wBuf |= PHY_ANAR_SYPAUSE;						// Auto-Negotiation Advertisement: Enable symmetric pause
	wBuf &= ~PHY_ANAR_ASYPAUSE;						// Auto-Negotiation Advertisement: Disable asymmetric pause
	MdioWrite(phy_info.addr, PHY_ANAR_ADR, wBuf);	// Auto-Negotiation Advertisement: Write
#endif

	MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);	// Basic Control: Read1
	MdioRead(phy_info.addr, PHY_BCR_ADR, &wBuf);	// Basic Control: Read2
	if (onoff == DEF_OK) {
		wBuf |= PHY_BCR_AUTONEG_EN; 					// Basic Control: Enable auto-negotiation process
		wBuf |= PHY_BCR_RST_AUTONEG;					// Basic Control: Restart auto-negotiation process
		wBuf |= PHY_BCR_FULLDPLX;						// Basic Control: Temp
		wBuf &= ~PHY_BCR_ISOLATE;						// Basic Control: Normal operation
		MdioWrite(phy_info.addr, PHY_BCR_ADR, wBuf);	// Basic Control: Write
		phy_info.autonego = PHY_AUTONEGO_ING;
		
		printf("Phy auto negotiation start.\r\n");
	} else {
		wBuf &= ~PHY_BCR_AUTONEG_EN; 					// Basic Control: Disable auto-negotiation process
		wBuf &= ~PHY_BCR_RST_AUTONEG; 					// Basic Control: Stop auto-negotiation process
		wBuf |= PHY_BCR_SPEED_L;						// Basic Control: Set 100Mbps
		wBuf |= PHY_BCR_FULLDPLX;						// Basic Control: Set Full-duplex
		MdioWrite(phy_info.addr, PHY_BCR_ADR, wBuf);	// Basic Control: Write
		
		printf("Phy manual configuration.\r\n");
	}
}

void PhyIrq(void *ctx)
{
	static UINT PhyIrqFlag = 0;

	if (GpioGetPin(GPIO_ETH_IRQ) == 0) {
		PhyIrqFlag++;
	}

	if (PhyIrqFlag >= 2) {
		PhyIrqFlag = 0;
		WORD getData;
		MdioRead(phy_info.addr, PHY_ICS_ADR, &getData);
		printf("Phy IRQ Get Data(0x%04X)\r\n", getData);
		if (getData & PHY_ICS_LUI) { // Link-Up
			if (PhyLinkInfo() == DEF_OK) {
//				printf("NetNIC Link Up Detect Link Speed(%dMbps) %s Duplex\r\n", phy_info.speed, phy_info.duplex == PHY_DUPLEX_FULL ? "Full" : "Half");
				EthDuplexChange(phy_info.duplex);
			} else {
				printf("Unknown Speed & Duplex - System reboot\r\n");
				WdtSysReboot();
			}
			EthRxTxinit(phy_info.type, phy_info.speed, phy_info.duplex);
			netifapi_netif_set_link_up(&gtMyNetIf);
		} else if (getData & PHY_ICS_LDI) { // Link-Down
			netifapi_netif_set_link_down(&gtMyNetIf);
		} else {
			printf("Phy IRQ Get Data(0x%04X)\r\n", getData);
		}
	}

	UNUSED(ctx);
}

u32 tid_PhyCheck = INVALID_TIMER_ID;

UINT PhyInit(BYTE phy_addr)
{
	GpioSetLo(GPIO_ETH_RST);
#ifdef DEF_CPU0
	WaitXms(10);
#endif
#ifdef DEF_CPU1
	vTaskDelay(1);
#endif
	GpioSetHi(GPIO_ETH_RST);
#ifdef DEF_CPU0
	WaitXms(10);
#endif
#ifdef DEF_CPU1
	vTaskDelay(1);
#endif

	phy_info.addr = phy_addr & 0x1F;
	phy_info.type = PHY_TYPE_UNKNOWN;
	phy_info.speed = PHY_SPD_0;
	phy_info.duplex = PHY_DUPLEX_UNKNOWN;
	phy_info.autonego = PHY_AUTONEGO_NONE;

	if (PhyReset() == DEF_FAIL) {
		return DEF_FAIL;
	}

// KSZ8081, KSZ9031 only
#if (defined(__ETH_KSZ8081MNX__) || defined(__ETH_KSZ9031RNX__))
	MdioWrite(phy_info.addr, PHY_ICS_ADR, PHY_ICS_LUIE | PHY_ICS_LDIE);		// Interrupt Control(Enable link-down, link-up interrupt)
	if (tid_PhyCheck == INVALID_TIMER_ID) {
		tid_PhyCheck = timer_sched_timeout_cb(50, TIMEOUT_PERIODIC, PhyIrq, NULL);
	}
#endif

#if PHY_AUTONEG
#if defined(__ETH_KSZ9031RNX__)
/* KSZ9031RNX Revision 2.2 page 72.(May 14, 2015)

The KSZ9031RNX fast link pulse(FLP) burst-toburst transmit timing for Auto-Negotiation defaults to 8ms.
IEEE 802.3 standard specifies this timing to be 16ms +/-8ms.
Some PHY link partners need to receive the FLP with 16ms centered timing; otherwise, there can be intermittent link failures and long link-up times.

after KSZ9031RNX power-up/reset, program the following register sequence to set the FLP timing to 16ms.

the above setting for 16ms FLP transmit timing is compatible with all PHY link partners. */
	MdioWrite(phy_info.addr, 0xD, 0x0000); // Set Register
	MdioWrite(phy_info.addr, 0xE, 0x0004); // REG:4
	MdioWrite(phy_info.addr, 0xD, 0x4000); // Set Data
	MdioWrite(phy_info.addr, 0xE, 0x0006); // DATA:6

	MdioWrite(phy_info.addr, 0xD, 0x0000); // Set Register
	MdioWrite(phy_info.addr, 0xE, 0x0003); // REG:3
	MdioWrite(phy_info.addr, 0xD, 0x4000); // Set Data
	MdioWrite(phy_info.addr, 0xE, 0x1A80); // DATA:1A80
#endif
	PhyAutoNeg(DEF_ON);
#else
	PhyAutoNeg(DEF_OFF);
#endif

	return DEF_OK;
}

#if (PHY_LOOPBACK_TEST==1)
#ifdef DEF_CPU1
void PhyLoopbackMode(UINT nSpeed)
{
	printf("Set Ethernet PHY Loopback Mode\r\n");
	int nType = PHY_TYPE_UNKNOWN;

	// Disable ethernet IRQ and netif down for CPU0 and disable H.264 encoding
	gptMsgShare.H264_ENC_ENABLE = 0;
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_ETHERNET, DEF_OFF) == DEF_FAIL)
		vTaskDelay(1);

	netifapi_netif_set_down(&gtMyNetIf);
	while (netif_is_up(&gtMyNetIf)) {
		vTaskDelay(1);
	}

	// Set ethernet PHY : disable interrupt
	MdioWrite(phy_info.addr, PHY_ICS_ADR, 0);

	// Set ethernet PHY : loopback mode, Speed(100/1000Mbps), Full-duplex
	if (nSpeed == PHY_SPD_100) {
		nType = PHY_TYPE_10_100;
		MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_LOOP_BACK | PHY_BCR_SPEED_L | PHY_BCR_FULLDPLX);
	} else if (nSpeed == PHY_SPD_1000) {
		nType = PHY_TYPE_1000;
		MdioWrite(phy_info.addr, PHY_BCR_ADR, PHY_BCR_LOOP_BACK | PHY_BCR_SPEED_M | PHY_BCR_FULLDPLX);
	}

	phy_info.type = nType;
	phy_info.speed = nSpeed;
	phy_info.duplex = PHY_DUPLEX_FULL;

	printf("  Type(%d), Speed(%d), Duplex(%d)\r\n", phy_info.type, phy_info.speed, phy_info.duplex);

	EthRxTxinit(phy_info.type, phy_info.speed, phy_info.duplex);

	// Set multicast filter, mac filter disable
	EthMulFiltDisable();
	ETH_RX_MACFILT = 0;

	// Enable ethernet IRQ for CPU0 in loopback mode
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_ETH_LOOPBACK, 0) == DEF_FAIL)
		vTaskDelay(1);
}

UINT PhyLoopbackTestCnt(UINT TX_DLY, UINT TX_EDGE, UINT RX_DLY, UINT RX_EDGE, UINT COUNT)
{
	UINT i, j;
	UINT idleCnt = 0;

	ETH_TX_TCKDLY = TX_DLY;
	ETH_TX_CLKEDGE = TX_EDGE;

	ETH_RX_RCKDLY = RX_DLY;
	ETH_RX_RCKEDGE = RX_EDGE;

#define PHY_LOOPBACK_PACKET_SIZE 1500

#if 1
	for (j = 0; j < PHY_LOOPBACK_PACKET_SIZE; j++) {
		gptMsgSH02[j] = rand();
		gptMsgSH03[j] = rand();
		gptMsgSH04[j] = rand();
	}
#endif

	for (i = 0; i < COUNT; i++) {
		gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_idle;

		if (i % 3 == 0)			DmaMemCpy_ip((BYTE *)gptMsgSH01, (BYTE *)gptMsgSH02, PHY_LOOPBACK_PACKET_SIZE);
		else if (i % 3 == 1)	DmaMemCpy_ip((BYTE *)gptMsgSH01, (BYTE *)gptMsgSH03, PHY_LOOPBACK_PACKET_SIZE);
		else if (i % 3 == 2)	DmaMemCpy_ip((BYTE *)gptMsgSH01, (BYTE *)gptMsgSH04, PHY_LOOPBACK_PACKET_SIZE);

		DmaEthTxCpy((BYTE *)gptMsgSH01, PHY_LOOPBACK_PACKET_SIZE);
		while (gptMsgDebug.ETH_LOOPBACK_CHECK == ePlb_idle) {
			idleCnt++;
			WaitXus(1);
			if (idleCnt >= 100000) {
				break;
			}
		}
		idleCnt = 0;
		if (gptMsgDebug.ETH_LOOPBACK_CHECK == ePlb_idle) {
			gptMsgDebug.ETH_LOOPBACK_CHECK = ePlb_timeout;
			break;
		}
		if (gptMsgDebug.ETH_LOOPBACK_CHECK > ePlb_ok) {
			break;
		}
	}
	return gptMsgDebug.ETH_LOOPBACK_CHECK;
}

void PhyLoopbackTest(UINT nLoop)
{
	while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_OFF) == DEF_FAIL) {
		vTaskDelay(1);
	}

	UINT k, z, t, r, TestRes;
	printf("Ethernet Loopback Test(step loop %d)\r\n", nLoop);
	printf("O:pass, 2:compare fail, 3:zero data, 4:data length underflow or overflow, 5:timeout\r\n");
	printf("          |            RX-Edge(0)           |            RX-Edge(1)           |\r\n");
	printf("RX-DLY	  | 0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3 4 5 6 7 8 9 A B C D E F |\r\n");
	for (t = 0; t < 16; t++) { // TX dly
		printf("\r\nTX-DLY(%X)\r\n", t);
		for(r = 0; r < 2; r++) { // TX edge
			printf("TX-Edge(%X)| ", r);
			for(z = 0; z < 2; z++) { // RX edge
				for(k = 0; k < 16; k++) { // RX dly
					TestRes = PhyLoopbackTestCnt(t, r, k, z, nLoop);
					if (TestRes == ePlb_ok) {	// 0:none, idle
						printf("O ");			// 1:test oK
					} else {					// 2:data error, compare fail
						printf("%d ", TestRes);	// 3:zero data
					}							// 4:data length underflow or overflow
												// 5:timeout
				}
				printf("| ");
			}
			printf("\r\n");
		}
	}
	vTaskDelay(10);

	while (MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_VLOCK_IRQ, DEF_ON) == DEF_FAIL) {
		vTaskDelay(1);
	}
}
#endif
#endif
#endif
