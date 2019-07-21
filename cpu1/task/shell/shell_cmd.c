//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include <stdlib.h>		// for atoi
#include <time.h>

#include "dev.h"
#include "dev_isp_common.h" // 180319 KDH Change from dev_isp.h
#include "dev_inline.h"	// to use fast code

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "shell.h"
#include "shell_cmd.h"
#include "shell_cmd_common.h"
#include "shell_cmd_fs.h"
#include "shell_cmd_net.h"
#include "shell_cmd_isp.h"
#include "shell_cmd_sdio.h"
#include "string.h"

#include "net_ascii.h"
#include "sys_arch.h"

#include "phy.h"
#include "msg.h"
#include "user.h"
#include "file.h"
#include "ff.h"

#include "lwip/sys.h"	// for sys_thread_new
#include "lwip/igmp.h"
#include "lwip/netif.h"
#include "networkif.h"

#include "init.h"

#ifdef __AUD_TLV320AIC3100__
#include "tlv320aic3100.h"
#endif
#ifdef __AUD_ES8374__
#include "es8374.h"
#endif

#ifdef __ONVIF__
#include "onvifSystemInfo.h"
#endif
#ifdef __WIFI__
#include "wifi.h"
#endif


INT16S	HelpDisp(INT32S argc, char *argv[]);

#if (ETH_SHELL_TEST==1)
INT16S cmd_eth_test(INT32S argc, char *argv[]);
#endif

INT16S cmd_i2s_test(INT32S argc, char *argv[]);

INT16S cmd_jpeg_test(INT32S argc, char *argv[]);

INT16S cmd_sdcd_test(INT32S argc, char *argv[]);
INT16S cmd_uart1_test(INT32S argc, char *argv[]);
INT16S cmd_state_view(INT32S argc, char *argv[]);
#ifdef RTMP_SUPPORT
INT16S cmd_rtmp_test(INT32S argc, char *argv[]);
#endif

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
const char *sHelpDisp[]	=	{"Shell command list (! : Repeat command)",	(char*)0};

const char *sEthTest[] = {"ETH test",    				(char*)0};
const char *sI2sTest[] = {"I2S test",    				(char*)0};
const char *sUart1Test[] = {"UART1 test",    			(char*)0};
const char *sStateView[] = {"State View",    			(char*)0};
#ifdef RTMP_SUPPORT
const char *sRtmpTest[] = {"RTMP test",    				(char*)0};
#endif

tMonCmd gCmdList[] =
{
	{"?",			HelpDisp,			sHelpDisp		},

//TEST
#ifdef __ETH__
#if (ETH_SHELL_TEST==1)
	{"eth",         cmd_eth_test,       sEthTest        },
#endif
#endif
	{"i2s",         cmd_i2s_test,       sI2sTest        },
	{"sd",          cmd_sdcd_test,      sSdcdTest       },
	{"sc",          cmd_sdio_chain_test,sSdiochainTest  },
	{"u1",          cmd_uart1_test,     sUart1Test      },
	{"view",        cmd_state_view,     sStateView      },
#ifdef RTMP_SUPPORT
	{"rtmp",		cmd_rtmp_test,		sRtmpTest       },
#endif
#if (LVDS_MARGIN_TEST==1)
	{"lvds",		cmd_lvds,			sLvdsCmd		},
#endif

//COMMON
	{"a",			UsrCmd0,			sUsrCmd0		},
	{"b",			UsrCmd1,			sUsrCmd1		},
	{"c",			UsrCmd2,			sUsrCmd2		},
	{"d",			UsrCmd3,			sUsrCmd3		},
	{"e",			UsrCmd4,			sUsrCmd4		},
	{"f",			UsrCmd5,			sUsrCmd5		},
	{"info",		cmd_info,			sInfo			},
	{"if",			UsrCheckInterface,	sUsrCkIf		},
	{"gs",			UsrGPIOState,		sUsrGPIOState	},
	{"reboot",      UseReboot,          sReboot         },
#ifdef __AUDIO__
	{"audio",       UsrAudioCmd,        sAudioCmd       },
#endif
	{"isp",			cmd_ispControl,		sIspControl		},
	{"isposd",		cmd_ispOSDControl,		sIspControl		},
#ifdef __HW_OSD__
	{"osd",         cmd_fontosd,        sFontOSDCmd     },
#endif
	{"time",		cmd_time,			sTimeCmd		},
	{"t",			cmd_task_stst,		sTaskStatus		},
	{"m",			cmd_mem_stst,		sMemStatus		},
	{"xssr",		cmd_xsr_stst,		sXsrStatus		},
	{"env",			cmd_envvar,			sEnvvar			},
	{"user",		cmd_userarea,		sUserAreaCmd	},
	{"userinfo",	cmd_userinfo,		sUserInfoCmd	},
#ifdef __H264__
	{"h264",        cmd_h264_info,      sH264Info       },
 	{"h1",			cmd_h264,			sH264CH1		},
#ifdef __H264_2CH__
 	{"h2",			cmd_h264,			sH264CH2		},
#endif
#endif
#ifdef __JPEG__
	{"jpeg",        cmd_jpeg_info,      sJpegInfo       },
#endif

//FileSystem
#ifdef __FAT_LOAD__
	{"ls",			UsrLSCmd,			sUsrLSCmd		},
	{"diskinit",	UsrDiskInitCmd,		sDiskInitCmd	},
	{"format",		UsrFormatCmd,		sFormatCmd		},
	{"mkdir",		UsrMKRMDIRCmd,		sMKDIRCmd		},
	{"rmdir",		UsrMKRMDIRCmd,		sRMDIRCmd		},
	{"rm",			UsrRMCmd,			sRMCmd			},
	{"pwd",			UsrPWDCmd,			sPWDCmd			},
	{"cd",			UsrCDCmd,			sCDCmd			},
	{"fcat",		UsrFCatCmd,			sFCatCmd		},
	{"fcreate",		UsrFCreateCmd,		sFCreateCmd		},
	{"fcopy",       UsrFCopyCmd,        sFCopyCmd		},
#if (ENX_FTPD_use==1)
	{"ftpd",		UsrFtpdCmd,			sFtpdCmd		},
#endif
	{"fstat",		UsrFstatCmd,		sFstatCmd		},
	{"fhash",       UsrFileHash,        sFileHash       },
#ifdef __DEVICE_SD__
	{"sdtest",      UsrSDCardSpeedTestCmd, sSDCardSpeedTestCmd},
	{"sdsave",		UsrMakeaviCmd,		sMakeaviCmd		},
#endif
	{"ftest",		UseFatTest,			sFatTestCmd		},
#endif

//Network
#if defined(__ETH__) || defined(__WIFI__)
	{"ifconfig",	cmd_ifconfig,		sIfconfigCmd	},
	{"dhcp",		cmd_dhcp,			sDhcpCmd		},
#if (ENX_SNTPC_use==1)
	{"sntp",		cmd_sntp,			sSntpCmd		},
#endif
	{"netstat", 	cmd_netstat,		sNetstatCmd 	},
#ifdef __ETH__
	{"mac", 		cmd_mac,			sMacCmd 		},
#endif
#ifdef __WIFI__
	{"wifistatus",	cmd_wifistatus, 	sWiFiStatus 	},
	{"wifi",		cmd_wifionoff,		sWiFiSW 		},
#endif
	{"ntx",			cmd_nettx_test,		sNetTx	 		},
	{"rtsp",		cmd_rtsp_state,		sRtspState		},
#endif
	{0,				0,					0				}
};


//*************************************************************************************************
// Display help
//-------------------------------------------------------------------------------------------------
//
INT16S HelpDisp(INT32S argc, char *argv[])
{
	INT8U i = 0;

	for(i=0;gCmdList[i].name!=0;i++)
	{
		printf("[%02d]:[%10s] - [%57s]\r\n", i, gCmdList[i].name, *(gCmdList[i].helphelp));
	}
	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

UINT uart1_rx_switch;
void cmd_uart1_rx(void *ctx)
{
	char u1_rx;
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	while(1)
	{
		Uart1_Rx(&u1_rx);
		printf("UART1_RX : %c[0x%02X]\r\n", u1_rx, u1_rx);
		vTaskDelay(1);
		if(uart1_rx_switch == 0)
			break;
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	vTaskDelete(NULL);
	UNUSED(ctx);
}

INT16S cmd_uart1_test(INT32S argc, char *argv[])
{
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	if(argc >= 2)
	{
		if(strcmp(argv[1], "rx") == 0)
		{
			if(argc == 3)
			{
				if(strcmp(argv[2], "on") == 0)
				{
					uart1_rx_switch = 1;
					sys_thread_new("u1_rx", cmd_uart1_rx, NULL, _16KB_STACK_SIZE, LOW_TASK_PRIO);
				}
				else if(strcmp(argv[2], "off") == 0)
				{
					uart1_rx_switch = 0;
				}
				else
				{
					cmd_unknown();
				}
			}
			else
			{
				cmd_unknown();
			}
		}
		else if(strcmp(argv[1], "tx") == 0)
		{
			int i, nlen = strlen(argv[2]);
			for(i=0;i<nlen;i++)
			{
				Uart1_Tx(argv[2][i]);
			}
		}
		else
		{
			cmd_unknown();
		}
	}
	else
	{
		cmd_unknown();
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	return 0;
}

#ifdef __ETH__
#if (ETH_SHELL_TEST==1)
int eth_test_task_sw;
void cmd_eth_test_task(void *ctx)
{
	eth_test_task_sw = 1;
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	BYTE test_data[1514] = {0};
	int i;
	for(i=0;i<1514;i++)
	{
		test_data[i] = i;
//		if(i%2==0)
//			test_data[i] = 0xaa;
//		else
//			test_data[i] = 0x55;
	}
	while(eth_test_task_sw)
	{
		DmaEthTxCpy(test_data, 1514);
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	vTaskDelete(NULL);

	UNUSED(ctx);
}

#define eth_getset(val)	{	if(argc == 3) {	\
								printf(#val"(%d)\r\n", val); \
							} else { \
								UINT getdValue = atoi(argv[3]); \
								val = getdValue; \
								printf("input(%d) => set "#val"(%d)\r\n", getdValue, val); \
							} \
						}

void eth_rx_state(void)
{
	printf("== RX state =================\r\n");
	printf("ETH_RX_EN                : %2d\r\n", ETH_RX_EN);
	printf("ETH_RX_CRCFILT  (rx crc) : %2d\r\n", ETH_RX_CRCFILT);
	printf("ETH_RX_EMPTY             : %2d\r\n", ETH_RX_EMPTY);
	printf("ETH_RX_LEN               : %2d\r\n", ETH_RX_LEN);
	printf("ETH_RX_ERTYPE   (rx err) : %2d\r\n", ETH_RX_ERTYPE);	
	printf("== RX Mac Filter ============\r\n");
	printf("ETH_RX_MACFILT  (if mcf) : %2d\r\n", ETH_RX_MACFILT);
	printf("ETH_RX_MACADR0           : 0x%04X\r\n", ETH_RX_MACADR0);
	printf("ETH_RX_MACADR1           : 0x%08X\r\n", ETH_RX_MACADR1);
	printf("== RX Multicast Filter ======\r\n");
	printf("ETH_RX_MUL0FILT (if-muf) : %2d\r\n", ETH_RX_MUL0FILT);
	printf("ETH_RX_MUL1FILT (¦§mufi) : %2d\r\n", ETH_RX_MUL1FILT);
	printf("ETH_RX_MUL2FILT (¦§mufc) : %2d\r\n", ETH_RX_MUL2FILT);
	printf("ETH_RX_MUL3FILT (¦§mufj) : %2d\r\n", ETH_RX_MUL3FILT);
	printf("ETH_RX_MUL4FILT (¦¦mufl) : %2d\r\n", ETH_RX_MUL4FILT);
	printf("ETH_RX_MUL5FILT (      ) : %2d\r\n", ETH_RX_MUL5FILT);
	printf("ETH_RX_MUL6FILT (      ) : %2d\r\n", ETH_RX_MUL6FILT);
	printf("ETH_RX_MUL7FILT (      ) : %2d\r\n", ETH_RX_MUL7FILT);
	printf("ETH_RX_MUL8FILT (      ) : %2d\r\n", ETH_RX_MUL8FILT);
	printf("ETH_RX_MUL9FILT (      ) : %2d\r\n", ETH_RX_MUL9FILT);
	printf("ETH_RX_MUL10FILT(      ) : %2d\r\n", ETH_RX_MUL10FILT);
	printf("ETH_RX_MUL11FILT(      ) : %2d\r\n", ETH_RX_MUL11FILT);
	printf("ETH_RX_MUL0ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL0ADR);
	printf("ETH_RX_MUL1ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL1ADR);
	printf("ETH_RX_MUL2ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL2ADR);
	printf("ETH_RX_MUL3ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL3ADR);
	printf("ETH_RX_MUL4ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL4ADR);
	printf("ETH_RX_MUL5ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL5ADR);
	printf("ETH_RX_MUL6ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL6ADR);
	printf("ETH_RX_MUL7ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL7ADR);
	printf("ETH_RX_MUL8ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL8ADR);
	printf("ETH_RX_MUL9ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL9ADR);
	printf("ETH_RX_MUL10ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL10ADR);
	printf("ETH_RX_MUL11ADR  (      ) : 0x%04X\r\n", ETH_RX_MUL11ADR);
	printf("== RX Clock & Delay =========\r\n");
	printf("ETH_RX_RCKEDGE	(rx e  ) : %2d\r\n", ETH_RX_RCKEDGE);
	printf("ETH_RX_RCKDLY	(rx rck) : %2d\r\n", ETH_RX_RCKDLY);
}

void eth_tx_state(void)
{
	printf("== TX state ===============\r\n");
	printf("ETH_TX_CLKOE    (tx ce ) : %2d\r\n", ETH_TX_CLKOE);
	printf("ETH_TX_CLKSEL   (tx cs ) : %2d\r\n", ETH_TX_CLKSEL);
	printf("ETH_TX_CRSCHK   (tx cr ) : %2d\r\n", ETH_TX_CRSCHK);
	printf("ETH_TX_COLCHK   (tx co ) : %2d\r\n", ETH_TX_COLCHK);
	printf("ETH_TX_RTYEN    (tx rn ) : %2d\r\n", ETH_TX_RTYEN);
	printf("ETH_TX_RTYLMT   (tx rl ) : %2d\r\n", ETH_TX_RTYLMT);
	printf("ETH_TX_EMPTY             : %2d\r\n", ETH_TX_EMPTY);
	printf("ETH_TX_FULL              : %2d\r\n", ETH_TX_FULL);
	printf("== TX Clock & Delay =======\r\n");
	printf("ETH_IFG_GAP     (tx ifg) : %2d\r\n", ETH_TX_IFGGAP);
	printf("ETH_TX_CLKEDGE  (tx e  ) : %2d\r\n", ETH_TX_CLKEDGE);
	printf("ETH_TX_TCKDLY   (tx tck) : %2d\r\n", ETH_TX_TCKDLY);
	printf("ETH_TX_TXENDLY  (tx n)   : %2d\r\n", ETH_TX_TXENDLY);
	printf("ETH_TX_TXD0DLY  (tx 0)   : %2d\r\n", ETH_TX_TXD0DLY);
	printf("ETH_TX_TXD1DLY  (tx 1)   : %2d\r\n", ETH_TX_TXD1DLY);
	printf("ETH_TX_TXD2DLY  (tx 2)   : %2d\r\n", ETH_TX_TXD2DLY);
	printf("ETH_TX_TXD3DLY  (tx 3)   : %2d\r\n", ETH_TX_TXD3DLY);
}

INT16S cmd_eth_test(INT32S argc, char *argv[])
{
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	if(argc == 1)
	{	// state print
		printf("== Ethernet State ==========\r\n");
		printf("ETH Type               : ");
		if(ETH_RX_DATTYPE == 1 && ETH_TX_DATTYPE == 1)		printf("1000Mbps Type\r\n");
		else if(ETH_RX_DATTYPE == 0 && ETH_TX_DATTYPE == 0)	printf("10/100Mbps Type\r\n");
		else	printf("Unknown Type(ETH_RX_DATTYPE:%d, ETH_TX_DATTYPE:%d)\r\n", ETH_RX_DATTYPE, ETH_TX_DATTYPE);
		eth_rx_state();
		eth_tx_state();
		printf("== IRQ state ==============\r\n");
		printf("ETH_RX_IRQ             : %2d\r\n", ETH_RX_IRQ);
		printf("ETH_TX_IRQ             : %2d\r\n", ETH_TX_IRQ);
		printf("ETH_RX_IRQEN           : %2d\r\n", ETH_RX_IRQEN);
		printf("ETH_TX_IRQEN           : %2d\r\n", ETH_TX_IRQEN);
		printf("ETH_RX_IRQCLR          : %2d\r\n", ETH_RX_IRQCLR);
		printf("ETH_TX_IRQCLR          : %2d\r\n", ETH_TX_IRQCLR);
		printf("===========================\r\n");
	}
	else
	{
		if(strcmp(argv[1], "rx") == 0)
		{
			if(argc == 2)
			{
				eth_rx_state();
			}
			else
			{
				if(strcmp(argv[2], "on") == 0)
				{
					gptMsgDebug.ETH_RX_CHECK = 1;
				}
				else if(strcmp(argv[2], "off") == 0)
				{
					gptMsgDebug.ETH_RX_CHECK = 0;
				}
				else if(strcmp(argv[2], "type") == 0)
				{
					eth_getset(ETH_RX_DATTYPE);
				}
				else if(strcmp(argv[2], "en") == 0)
				{
					eth_getset(ETH_RX_EN);
				}
				else if(strcmp(argv[2], "rck") == 0)
				{
 					eth_getset(ETH_RX_RCKDLY);
				}
				else if(strcmp(argv[2], "err") == 0)
				{
					eth_getset(ETH_RX_ERTYPE);
				}
				else if(strcmp(argv[2], "e") == 0)
				{
					eth_getset(ETH_RX_RCKEDGE);
				}
				else if(strcmp(argv[2], "crc") == 0)
				{
					eth_getset(ETH_RX_CRCFILT);
				}
				else if(strcmp(argv[2], "auto") == 0)
				{
					int i, j;
					gptMsgDebug.ETH_RX_CHECK = 1;
					for(i=0;i<2;i++)
					{
						for(j=0;j<16;j++)
						{
							netif_set_down(&gtMyNetIf);
							ETH_RX_RCKDLY = j;
							printf("\r\nETH EDGE_RXCLK(%d) ETH_RX_RCKDLY(%d)\r\n", ETH_RX_RCKEDGE, ETH_RX_RCKDLY);
							netif_set_up(&gtMyNetIf);
							vTaskDelay(500);
						}
						ETH_RX_RCKEDGE = ~ETH_RX_RCKEDGE;
					}
					gptMsgDebug.ETH_RX_CHECK = 0;
				}
				else if(strcmp(argv[2], "reset") == 0)
				{
					printf("Ethernet RX IRQ Stop!\r\n");
					while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_ETHERNET, DEF_OFF) == DEF_FAIL)
						vTaskDelay(1);

					printf("Ethernet RX Sram Buffer Reset\r\n");
					portENTER_CRITICAL();
					DmaMemSet_isr((BYTE *)&gptMsgNetworkRXShared, 0, sizeof(gptMsgNetworkRXShared));
					DmaMemSet_isr((BYTE *)&gptMsgNetworkRX0, 0, sizeof(gptMsgNetworkRX0));
					DmaMemSet_isr((BYTE *)&gptMsgNetworkRX1, 0, sizeof(gptMsgNetworkRX1));
#if (ETH_MAC_PAUSE==1)
					gptMsgNetworkRXShared.tx_go = 0;
					gptMsgNetworkRXShared.tx_pause = 0;
					gptMsgNetworkRXShared.macp_time = 0;
					gptMsgNetworkRXShared.macp_count = 0;
#elif (ETH_MAC_PAUSE==2)
					gptMsgNetworkRXShared.tx_ifggap = 0;
#endif
					gptMsgNetworkRX0.base = NETRX0_BUF_BASE;
					gptMsgNetworkRX0.end = NETRX0_BUF_END;
					gptMsgNetworkRX0.head_pos = NETRX0_BUF_BASE;
					gptMsgNetworkRX1.base = NETRX1_BUF_BASE;
					gptMsgNetworkRX1.end = NETRX1_BUF_END;
					gptMsgNetworkRX1.head_pos = NETRX1_BUF_BASE;
					portEXIT_CRITICAL();

					printf("Ethernet RX IRQ Start!\r\n");
					while(MsgPut(&gptMsgCpu1to0, MSG_CMD_SW_ETHERNET, DEF_ON) == DEF_FAIL)
						vTaskDelay(1);
				}
				else
				{
					Shell_Unknown();
				}
			}
		}
		else if(strcmp(argv[1], "tx") == 0)
		{
			if(argc == 2)
			{
				eth_tx_state();
			}
			else
			{
				if(strcmp(argv[2], "on") == 0)
				{
					gptMsgDebug.ETH_TX_CHECK = 1;
				}
				else if(strcmp(argv[2], "off") == 0)
				{
					gptMsgDebug.ETH_TX_CHECK = 0;
				}
				else if(strcmp(argv[2], "type") == 0)
				{
					eth_getset(ETH_TX_DATTYPE);
				}
				else if(strcmp(argv[2], "tck") == 0)
				{
					eth_getset(ETH_TX_TCKDLY);
				}
				else if(strcmp(argv[2], "ifg") == 0)
				{
					eth_getset(ETH_TX_IFGGAP);
				}
				else if(strcmp(argv[2], "e") == 0)
				{
					eth_getset(ETH_TX_CLKEDGE);
				}
				else if(strcmp(argv[2], "t") == 0)
				{
					eth_getset(ETH_TX_TXENDLY);
					eth_getset(ETH_TX_TXD0DLY);
					eth_getset(ETH_TX_TXD1DLY);
					eth_getset(ETH_TX_TXD2DLY);
					eth_getset(ETH_TX_TXD3DLY);
				}
				else if(strcmp(argv[2], "dstck") == 0)
				{
					eth_getset(PAD_ETH_TCK_DS);
				}
				else if(strcmp(argv[2], "dsdata") == 0)
				{
					eth_getset(PAD_ETH_TXD3_DS);
					eth_getset(PAD_ETH_TXD2_DS);
					eth_getset(PAD_ETH_TXD1_DS);
					eth_getset(PAD_ETH_TXD0_DS);
					eth_getset(PAD_ETH_TXEN_DS);
				}
				else if(strcmp(argv[2], "n") == 0)
				{
					eth_getset(ETH_TX_TXENDLY);
				}
				else if(strcmp(argv[2], "0") == 0)
				{
					eth_getset(ETH_TX_TXD0DLY);
				}
				else if(strcmp(argv[2], "1") == 0)
				{
					eth_getset(ETH_TX_TXD1DLY);
				}
				else if(strcmp(argv[2], "2") == 0)
				{
					eth_getset(ETH_TX_TXD2DLY);
				}
				else if(strcmp(argv[2], "3") == 0)
				{
					eth_getset(ETH_TX_TXD3DLY);
				}
				else if(strcmp(argv[2], "co") == 0)
				{
					eth_getset(ETH_TX_COLCHK);
				}
				else if(strcmp(argv[2], "cr") == 0)
				{
					eth_getset(ETH_TX_CRSCHK);
				}
				else if(strcmp(argv[2], "rn") == 0)
				{
					eth_getset(ETH_TX_RTYEN);
				}
				else if(strcmp(argv[2], "rl") == 0)
				{
					eth_getset(ETH_TX_RTYLMT);
				}
				else if(strcmp(argv[2], "ce") == 0)
				{
					eth_getset(ETH_TX_CLKOE);
				}
				else if(strcmp(argv[2], "cs") == 0)
				{
					eth_getset(ETH_TX_CLKSEL);
				}
				else if(strcmp(argv[2], "start") == 0)
				{
					sys_thread_new("eth_tx", cmd_eth_test_task, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
				}
				else if(strcmp(argv[2], "stop") == 0)
				{
					eth_test_task_sw = 0;
				}
				else
				{
					Shell_Unknown();
				}
			}
		}
		else if(strcmp(argv[1], "if") == 0)
		{
			if(argc == 2)
			{
				extern struct netif *netif_list;
				struct netif *netif;
				for(netif=netif_list;netif!=NULL;netif=netif->next)
				{
					printf("%c%c%d	HWaddr %02X:%02X:%02X:%02X:%02X:%02X\r\n", netif->name[0], netif->name[1], netif->num,
						netif->hwaddr[0], netif->hwaddr[1], netif->hwaddr[2], netif->hwaddr[3], netif->hwaddr[4], netif->hwaddr[5]);

					printf("	inet addr:%IP Mask:%IP GW:%IP\r\n", netif->ip_addr.addr, netif->netmask.addr, netif->gw.addr);

					printf("	%s%s%s%s%s%s%sMTU:%d\r\n",
						netif->flags & NETIF_FLAG_UP ? "UP " : "",
						netif->flags & NETIF_FLAG_BROADCAST ? "BROADCAST " : "",
						netif->flags & NETIF_FLAG_POINTTOPOINT ? "PPP " : "",
						netif->flags & NETIF_FLAG_DHCP ? "DHCP " : "",
						netif->flags & NETIF_FLAG_LINK_UP ? "LINK-UP " : "",
						netif->flags & NETIF_FLAG_ETHARP ? "ETHARP " : "",
						netif->flags & NETIF_FLAG_IGMP ? "IGMP " : "",
						netif->mtu);
				}
			}
			else
			{
				if(argc == 3 && strcmp(argv[2], "on") == 0)
				{
					netif_set_up(&gtMyNetIf);
				}
				else if(argc == 3 && strcmp(argv[2], "off") == 0)
				{
					netif_set_down(&gtMyNetIf);
				}
				else if(argc == 3 && strcmp(argv[2], "auto") == 0)
				{
					netif_set_down(&gtMyNetIf);
					PhyInit(PHY_ADDR);
					netif_set_up(&gtMyNetIf);
				}
				else if(argc == 4 && strcmp(argv[2], "t1000") == 0)
				{
					if(strcmp(argv[3], "1000f") == 0)
					{
						EthRxTxinit(1, 1000, PHY_DUPLEX_FULL);
						printf("input(%s) => set 1000PHY 1000Mbps FULL\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "100f") == 0)
					{
						EthRxTxinit(1, 100, PHY_DUPLEX_FULL);
						printf("input(%s) => set 1000PHY 100Mbps FULL\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "100h") == 0)
					{
						EthRxTxinit(1, 100, PHY_DUPLEX_HALF);
						printf("input(%s) => set 1000PHY 100Mbps HALF\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "10f") == 0)
					{
						EthRxTxinit(1, 10, PHY_DUPLEX_FULL);
						printf("input(%s) => set 1000PHY 10Mbps FULL\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "10h") == 0)
					{
						EthRxTxinit(1, 10, PHY_DUPLEX_HALF);
						printf("input(%s) => set 1000PHY 10Mbps HALF\r\n", argv[3]);
					}
					else
					{
						Shell_Unknown();
					}
				}
				else if(argc == 4 && strcmp(argv[2], "t100") == 0)
				{
					if(strcmp(argv[3], "100f") == 0)
					{
						EthRxTxinit(0, 100, PHY_DUPLEX_FULL);
						printf("input(%s) => set 100PHY 100Mbps FULL\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "100h") == 0)
					{
						EthRxTxinit(0, 100, PHY_DUPLEX_HALF);
						printf("input(%s) => set 100PHY 100Mbps HALF\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "10f") == 0)
					{
						EthRxTxinit(0, 10, PHY_DUPLEX_FULL);
						printf("input(%s) => set 100PHY 10Mbps FULL\r\n", argv[3]);
					}
					else if(strcmp(argv[3], "10h") == 0)
					{
						EthRxTxinit(0, 10, PHY_DUPLEX_HALF);
						printf("input(%s) => set 100PHY 10Mbps HALF\r\n", argv[3]);
					}
					else
					{
						Shell_Unknown();
					}
				}
				else if((argc == 3 || argc == 4) && strcmp(argv[2], "mcf") == 0)
				{
					eth_getset(ETH_RX_MACFILT);
				}
				else if(strcmp(argv[2], "muf") == 0)
				{
					if(argc == 3) {
						printf("ETH_RX_MUL0FILT  : %2d\r\n", ETH_RX_MUL0FILT);
						printf("ETH_RX_MUL1FILT  : %2d\r\n", ETH_RX_MUL1FILT);
						printf("ETH_RX_MUL2FILT  : %2d\r\n", ETH_RX_MUL2FILT);
						printf("ETH_RX_MUL3FILT  : %2d\r\n", ETH_RX_MUL3FILT);
						printf("ETH_RX_MUL4FILT  : %2d\r\n", ETH_RX_MUL4FILT);
						printf("ETH_RX_MUL5FILT  : %2d\r\n", ETH_RX_MUL5FILT);
						printf("ETH_RX_MUL6FILT  : %2d\r\n", ETH_RX_MUL6FILT);
						printf("ETH_RX_MUL7FILT  : %2d\r\n", ETH_RX_MUL7FILT);
						printf("ETH_RX_MUL8FILT  : %2d\r\n", ETH_RX_MUL8FILT);
						printf("ETH_RX_MUL9FILT  : %2d\r\n", ETH_RX_MUL9FILT);
						printf("ETH_RX_MUL10FILT : %2d\r\n", ETH_RX_MUL10FILT);
						printf("ETH_RX_MUL11FILT : %2d\r\n", ETH_RX_MUL11FILT);
					}
					else if(argc == 4)
					{
						UINT getValue = atoi(argv[3]);
						if(getValue == 0)		EthMulFiltDisable();
						else if(getValue == 1)	EthMulFiltEnable();
						printf("input(%s) => set MulFiltSwitch %s\r\n", argv[3], getValue==0?"OFF":getValue==1?"ON":"UNKNOWN");
					}
					else
					{
						Shell_Unknown();
					}
				}
				else if(argc == 4 && strcmp(argv[2], "mufi") == 0)
				{
					EthMulFiltInsert(ipaddr_addr(argv[3]));
					printf("input(%s) => set EthMulFiltInsert\r\n", argv[3]);
				}
				else if(argc == 3 && strcmp(argv[2], "mufc") == 0)
				{
					EthMulFiltClear();
					printf("input(NULL) => set EthMulFiltClear\r\n");
				}
				else if(argc == 4 && strcmp(argv[2], "mufj") == 0)
				{
					err_t ert = IGMP_Join(argv[3]);
					if(ert == ERR_OK)	printf("IGMP Join OK(%s)\r\n", argv[3]);
					else				printf("IGMP Join Error(%d)\r\n", ert);
				}
				else if(argc == 4 && strcmp(argv[2], "mufl") == 0)
				{
					err_t ert = IGMP_Leave(argv[3]);
					if(ert == ERR_OK)	printf("IGMP Leave OK(%s)\r\n", argv[3]);
					else				printf("IGMP Leave Error(%d)\r\n", ert);
				}
				else if(argc == 3 && strcmp(argv[2], "muflist") == 0)
				{
					extern void igmp_mac_filter_list(void);
					igmp_mac_filter_list();
				}
				else if(argc == 5 && strcmp(argv[2], "mufvar") == 0)
				{
					UINT getValue = atoi(argv[3]);
					UINT setValue = atoi(argv[4]);
					printf("Set ETH_RX_MUL%dADR : %d\r\n", getValue, setValue);
					switch(getValue)
					{
						case 0:		ETH_RX_MUL0ADR = setValue;		break;
						case 1:		ETH_RX_MUL1ADR = setValue;		break;
						case 2:		ETH_RX_MUL2ADR = setValue;		break;
						case 3:		ETH_RX_MUL3ADR = setValue;		break;
						case 4:		ETH_RX_MUL4ADR = setValue;		break;
						case 5:		ETH_RX_MUL5ADR = setValue;		break;
						case 6:		ETH_RX_MUL6ADR = setValue;		break;
						case 7:		ETH_RX_MUL7ADR = setValue;		break;
						case 8:		ETH_RX_MUL8ADR = setValue;		break;
						case 9:		ETH_RX_MUL9ADR = setValue;		break;
						case 10:	ETH_RX_MUL10ADR = setValue;		break;
						case 11:	ETH_RX_MUL11ADR = setValue;		break;
					}
				}
				else if(argc == 5 && strcmp(argv[2], "mufsw") == 0)
				{
					UINT getValue = atoi(argv[3]);
					UINT setValue = atoi(argv[4]);
					printf("Set ETH_RX_MUL%dFILT : %d\r\n", getValue, setValue);
					switch(getValue)
					{
						case 0:		ETH_RX_MUL0FILT = setValue;		break;
						case 1:		ETH_RX_MUL1FILT = setValue;		break;
						case 2:		ETH_RX_MUL2FILT = setValue;		break;
						case 3:		ETH_RX_MUL3FILT = setValue;		break;
						case 4:		ETH_RX_MUL4FILT = setValue;		break;
						case 5:		ETH_RX_MUL5FILT = setValue;		break;
						case 6:		ETH_RX_MUL6FILT = setValue;		break;
						case 7:		ETH_RX_MUL7FILT = setValue;		break;
						case 8:		ETH_RX_MUL8FILT = setValue;		break;
						case 9:		ETH_RX_MUL9FILT = setValue;		break;
						case 10:	ETH_RX_MUL10FILT = setValue;	break;
						case 11:	ETH_RX_MUL11FILT = setValue;	break;
					}
				}
				else
				{
					Shell_Unknown();
				}
			}
		}
		else if(argc == 3 && strcmp(argv[1], "phy") == 0)
		{
			BYTE getValue = (BYTE)(atoi(argv[2]) & 0xFF);
			WORD wBuf;
			MdioRead(phy_info.addr, getValue, &wBuf);
			PhyRegView(getValue, wBuf);
		}
#if (PHY_LOOPBACK_TEST==1)
#if defined(__ETH_KSZ8081MNX__)
		else if(argc == 2 && strcmp(argv[1], "lbm") == 0)
		{
			UINT nSpeed = 100;
			PhyLoopbackMode(nSpeed);
		}
#elif defined(__ETH_KSZ9031RNX__)
		else if(argc == 3 && strcmp(argv[1], "lbm") == 0)
		{
			UINT nSpeed = atoi(argv[2]);
			if(nSpeed != 100 && nSpeed != 1000)
				printf("Speed Error(%d)\r\n", nSpeed);
			else
				PhyLoopbackMode(nSpeed);
		}
#endif
		else if(argc == 3 && strcmp(argv[1], "lbt") == 0)
		{
			UINT nLoop = atoi(argv[2]);
			PhyLoopbackTest(nLoop);
		}
		else if(argc == 7 && strcmp(argv[1], "lbc") == 0)
		{
			UINT CNT  = atoi(argv[2]);
			UINT TX_E = atoi(argv[3]);
			UINT TX_D = atoi(argv[4]);
			UINT RX_E = atoi(argv[5]);
			UINT RX_D = atoi(argv[6]);
			printf("Ethernet Loopback Test(TX Dly(%X) E(%X), RX Dly(%X) E(%X))(Count:%d)\r\n", TX_D, TX_E, RX_D, RX_E, CNT);
			UINT TestRes = PhyLoopbackTestCnt(TX_D, TX_E, RX_D, RX_E, CNT);
			printf("Res : %d\r\n", TestRes);
		}
#endif
		else
		{
			Shell_Unknown();
		}
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	return 0;
}
#endif
#endif

INT16S cmd_i2s_test(INT32S argc, char *argv[])
{
#ifdef __AUDIO__
	if(argc == 2 && strcmp(argv[1], "state") == 0)
	{
		Aud_RegShow();
		printf("Read AUD_CONT_T(0x%08X)\r\n", AUD_CONT_T);
		printf("Read AUD_PWM_T(0x%08X)\r\n", AUD_PWM_T);
	}
	else if(argc == 3 && strcmp(argv[1], "set0") == 0)
	{
		printf("Input %d(0x%08X)\r\n", atoi(argv[2]), atoi(argv[2]));
		AUD_CONT_T = atol(argv[2]);
		printf("Read AUD_CONT_T(0x%08X)\r\n", AUD_CONT_T);
	}
	else if(argc == 3 && strcmp(argv[1], "set1") == 0)
	{
		printf("Input %d(0x%08X)\r\n", atoi(argv[2]), atoi(argv[2]));
		AUD_PWM_T = atol(argv[2]);
		printf("Read AUD_PWM_T(0x%08X)\r\n", AUD_PWM_T);
	}
	else if(argc == 2 && strcmp(argv[1], "0") == 0)
	{
#ifdef __AUD_TLV320AIC3100__
		// Audio Reset[20]
		GpioSetLo(GPIO_AUD_RST);
		vTaskDelay(10);
		GpioSetHi(GPIO_AUD_RST);
		tlv320aic3100_init();	
#endif
#ifdef __AUD_ES8374__
		es8374_init();
#endif
	}
	else if(argc == 2 && strcmp(argv[1], "1") == 0)
	{
#ifdef __AUD_TLV320AIC3100__
		tlv320aic3100_dac_init();
#endif
	}
	else if(argc == 3 && strcmp(argv[1], "agc") == 0)
	{
#ifdef __AUD_TLV320AIC3100__
		int agc_type = atoi(argv[2]);
		tlv320aic3100_agc(agc_type);
#endif
	}
	else if(argc == 3 && strcmp(argv[1], "mode") == 0)
	{
		gptMsgShare.AUD_TX_MODE = atoi(argv[2]);
		printf("I2S Out mode(%d)\r\n", gptMsgShare.AUD_TX_MODE);
	}
	else if(argc == 2 && strcmp(argv[1], "reg") == 0)
	{
		Aud_Init(1);
		gptMsgShare.AUD_TX_GO = 1;
		vTaskDelay(10);
	}
	else if(argc == 2 && strcmp(argv[1], "2") == 0)
	{
		//BYTE *WavAdr;
		UINT Count = 0;
		//EndianConv((BYTE*)AUD_BASE, (BYTE*) *WavAdr, 0x3fc, 0);	//	16bit
		AUD_TX_EN = 1;
		Count += 0x3fc;		
	}
	else if(argc == 2 && strcmp(argv[1], "3") == 0)
	{
		AUD_TX_CLR = 1;
		AUD_TX_EN = 0;
		AUD_TX_CLR = 1;
	}
#ifdef __AUD_ES8374__
	else if(argc == 3)
	{
		BYTE read_data = 0;
		BYTE un8TempValue = strtol(argv[2], NULL, 16);
		es8374_control(1, un8TempValue, &read_data);
		printf("read addr(0x%02X) data(0x%02X)\r\n", un8TempValue, read_data);
	}
	else if(argc == 4)
	{
		BYTE write_data = strtol(argv[3], NULL, 16);
		BYTE un8TempValue = strtol(argv[2], NULL, 16);
		es8374_control(0, un8TempValue, &write_data);
		printf("write addr(0x%02X) data(0x%02X)\r\n", un8TempValue, write_data);
	}
#endif
#ifdef __AUD_TLV320AIC3100__
	else if(argc == 2 && strcmp(argv[1], "4") == 0)	//	Vol Up
	{
		tlv320aic3100_volume_updown(DEF_TRUE);
	}
	else if(argc == 2 && strcmp(argv[1], "5") == 0)	//	Vol Down
	{
		tlv320aic3100_volume_updown(DEF_FALSE);
	}
	else if(argc == 3)
	{
		BYTE read_data = 0;
		tlv320aic3100_control(1, atoi(argv[2]), &read_data);
		printf("read addr(%d) data(%d)\r\n", atoi(argv[2]), read_data);
	}
	else if(argc == 4)
	{
		BYTE write_data = atoi(argv[3]);
		tlv320aic3100_control(0, atoi(argv[2]), &write_data);
		printf("write addr(%d) data(%d)\r\n", atoi(argv[2]), write_data);
		BYTE read_data = 0;
		tlv320aic3100_control(1, atoi(argv[2]), &read_data);
		printf("verify data(%d)\r\n", read_data);
	}
#endif
	else
	{
		Shell_Unknown();
	}
#else
	printf("Not use audio\r\n");
#endif

	return 0;
	UNUSED(argc);
	UNUSED(argv);
}

static void cmd_state_view_support_hex(const char *name, const char *name2, void *val)
{
	if (name2) {
		printf("%-21s.%-8s: 0x%08X, 0x%08X\r\n", name, name2, (UINT *)val, *((UINT *)val));
	} else {
		printf("%-30s: 0x%08X, 0x%08X\r\n", name, (UINT *)val, *((UINT *)val));
	}
}

static void cmd_state_view_support_dec(const char *name, const char *name2, void *val)
{
	if (name2) {
		printf("%-21s.%-8s: 0x%08X, %d\r\n", name, name2, (UINT *)val, *((UINT *)val));
	} else {
		printf("%-30s: 0x%08X, %d\r\n", name, (UINT *)val, *((UINT *)val));
	}
}

static void cmd_state_view_support_byte(const char *name, void *val, UINT size)
{
	printf("%-30s: 0x%08X (%dbyte)\r\n", name, val, size);
}

INT16S cmd_state_view(INT32S argc, char *argv[])
{
	const char *strMNRXS = "gptMsgNetworkRXShared";
	const char *strMNRX0 = "gptMsgNetworkRX0";
	const char *strMNRX1 = "gptMsgNetworkRX1";
#ifdef __WIFI__
	const char *strMEC10 = "gptMsgEthCpu1to0";
#endif

	if (argc == 2 && strcmp(argv[1], "msgeth") == 0) {
		cmd_state_view_support_hex(strMNRXS, NULL, (void *)&gptMsgNetworkRXShared);
#if (ETH_MAC_PAUSE==1)
		cmd_state_view_support_dec(strMNRXS, "pause", (void *)&(gptMsgNetworkRXShared.tx_pause));
		cmd_state_view_support_dec(strMNRXS, "go", (void *)&(gptMsgNetworkRXShared.tx_go));
		cmd_state_view_support_dec(strMNRXS, "time", (void *)&(gptMsgNetworkRXShared.macp_time));
		cmd_state_view_support_dec(strMNRXS, "count", (void *)&(gptMsgNetworkRXShared.macp_count));
#elif (ETH_MAC_PAUSE==2)
		cmd_state_view_support_dec(strMNRXS, "ifggap", (void *)&(gptMsgNetworkRXShared.tx_ifggap));
#endif
		cmd_state_view_support_hex(strMNRXS, "buffer", (void *)&(gptMsgNetworkRXShared.buffer));

		cmd_state_view_support_hex(strMNRX0, NULL, (void *)&(gptMsgNetworkRX0));
		cmd_state_view_support_hex(strMNRX0, "base", (void *)&(gptMsgNetworkRX0.base));
		cmd_state_view_support_hex(strMNRX0, "end", (void *)&(gptMsgNetworkRX0.end));
		cmd_state_view_support_hex(strMNRX0, "head_pos", (void *)&(gptMsgNetworkRX0.head_pos));
		cmd_state_view_support_dec(strMNRX0, "tail", (void *)&(gptMsgNetworkRX0.tail));
		cmd_state_view_support_dec(strMNRX0, "head", (void *)&(gptMsgNetworkRX0.head));
		cmd_state_view_support_hex(strMNRX0, "info", (void *)&(gptMsgNetworkRX0.info));

		cmd_state_view_support_hex(strMNRX1, NULL, (void *)&(gptMsgNetworkRX1));
		cmd_state_view_support_hex(strMNRX1, "base", (void *)&(gptMsgNetworkRX1.base));
		cmd_state_view_support_hex(strMNRX1, "end", (void *)&(gptMsgNetworkRX1.end));
		cmd_state_view_support_hex(strMNRX1, "head_pos", (void *)&(gptMsgNetworkRX1.head_pos));
		cmd_state_view_support_dec(strMNRX1, "tail", (void *)&(gptMsgNetworkRX1.tail));
		cmd_state_view_support_dec(strMNRX1, "head", (void *)&(gptMsgNetworkRX1.head));
		cmd_state_view_support_hex(strMNRX1, "info", (void *)&(gptMsgNetworkRX1.info));

#ifdef __WIFI__
		cmd_state_view_support_hex(strMEC10, NULL, (void *)&(gptMsgEthCpu1to0));
		cmd_state_view_support_dec(strMEC10, "ready", (void *)&(gptMsgEthCpu1to0.ready));
		cmd_state_view_support_dec(strMEC10, "msg_type", (void *)&(gptMsgEthCpu1to0.msg_type));
		cmd_state_view_support_dec(strMEC10, "lock", (void *)&(gptMsgEthCpu1to0.lock));
		cmd_state_view_support_dec(strMEC10, "head", (void *)&(gptMsgEthCpu1to0.head));
		cmd_state_view_support_dec(strMEC10, "tail", (void *)&(gptMsgEthCpu1to0.tail));
		cmd_state_view_support_dec(strMEC10, "num", (void *)&(gptMsgEthCpu1to0.num));
#endif
	} else if(argc == 2 && strcmp(argv[1], "offset") == 0) {
		printf("gtSystem         : 0x%08X (%dbyte) 0x%08X (%dbyte)\r\n", &gtSystem, sizeof(gtSystem), SFLS_SYSTEM_BASE, SFLS_SYSTEM_SIZE);
		printf("gtUser           : 0x%08X (%dbyte) 0x%08X (%dbyte)\r\n", &gtUser, sizeof(gtUser), SFLS_USER_BASE, SFLS_USER_SIZE);
#ifdef __ISP__
		printf("gtISP            : 0x%08X (fixed) 0x%08X (%dbyte)\r\n", gtISP, SFLS_ISP_BASE, SFLS_ISP_SIZE);
#endif
#ifdef __ONVIF__
		printf("gtONVIF          : 0x%08X (%dbyte) 0x%08X (%dbyte)\r\n", &gSysInfo, sizeof(SysInfo), SFLS_ONVIF_BASE, SFLS_ONVIF_SIZE);
#endif
#ifdef __WIFI__
		printf("gtWIFI           : 0x%08X (%dbyte) 0x%08X (%dbyte)\r\n", &gtWificfg, sizeof(tWificfg), SFLS_WIFI_BASE, SFLS_WIFI_SIZE);
#endif
		printf("\r\n");
		cmd_state_view_support_byte(strMNRXS, (void *)&gptMsgNetworkRXShared, sizeof(gptMsgNetworkRXShared));
		cmd_state_view_support_byte(strMNRX0, (void *)&gptMsgNetworkRX0, sizeof(gptMsgNetworkRX0));
		cmd_state_view_support_byte(strMNRX1, (void *)&gptMsgNetworkRX1, sizeof(gptMsgNetworkRX1));
#ifdef __WIFI__
		cmd_state_view_support_byte(strMEC10, (void *)&gptMsgEthCpu1to0, sizeof(gptMsgEthCpu1to0));
#endif
		cmd_state_view_support_byte("gptMsgCpu1to0", (void *)&gptMsgCpu1to0, sizeof(gptMsgCpu1to0));
		cmd_state_view_support_byte("gptMsgCpu0to1", (void *)&gptMsgCpu0to1, sizeof(gptMsgCpu0to1));
#ifdef __DEVICE_SD__
		cmd_state_view_support_byte("gptMsgRecInfo", (void *)&gptMsgRecInfo, sizeof(gptMsgRecInfo));
#endif
#if (ENX_FTPD_use==1)
		cmd_state_view_support_byte("ftpd_mb", (void *)&ftpd_mb, sizeof(ftpd_mb));
#endif
		cmd_state_view_support_byte("gptMsgShell", (void *)&gptMsgShell, sizeof(gptMsgShell));
		cmd_state_view_support_byte("gbMnParTbl", (void *)gbMnParTbl, sizeof(gbMnParTbl));
		printf("\r\n");
		cmd_state_view_support_byte("gptMsgShare", (void *)&gptMsgShare, sizeof(gptMsgShare));
		cmd_state_view_support_byte("gptMsgISPTbl", (void *)&gptMsgISPTbl, sizeof(gptMsgISPTbl));
#if (NET_TX_BUFFER==1)
		cmd_state_view_support_byte("gptMsgEthTx", &gptMsgEthTx, sizeof(gptMsgEthTx));
#endif
#if (RTSPoverHTTP==1)
		cmd_state_view_support_byte("gptMsgTunnel", (void *)&gptMsgTunnel, sizeof(gptMsgTunnel));
		cmd_state_view_support_byte("tunnel_port", (void *)tunnel_port, sizeof(tunnel_port));
#endif
#ifdef __WIFI__
		cmd_state_view_support_byte("gptMsgWlifCmd", (void *)&gptMsgWlifCmd, sizeof(gptMsgWlifCmd));
		cmd_state_view_support_byte("gptMsgWlifEvt", (void *)&gptMsgWlifEvt, sizeof(gptMsgWlifEvt));
#endif
		cmd_state_view_support_byte("gptMsgSH01", (void *)gptMsgSH01, sizeof(gptMsgSH01));
		cmd_state_view_support_byte("gptMsgSH02", (void *)gptMsgSH02, sizeof(gptMsgSH02));
		cmd_state_view_support_byte("gptMsgSH03", (void *)gptMsgSH03, sizeof(gptMsgSH03));
		cmd_state_view_support_byte("gptMsgSH04", (void *)gptMsgSH04, sizeof(gptMsgSH04));
		cmd_state_view_support_byte("gptMsgDebug", (void *)&gptMsgDebug, sizeof(gptMsgDebug));
		printf("\r\n");
	}
	else
	{
		Shell_Unknown();
		printf("CMD1 - view msgeth : gptMsgEthCpu0to1 / gptMsgEthCpu1to0 area\r\n");
		printf("CMD1 - view offset : SFLS save area / SHSRAM address\r\n");
	}

	return 0;
}

#ifdef RTMP_SUPPORT
INT16S cmd_rtmp_test(INT32S argc, char *argv[])
{
	if(strcmp("on", argv[1]) == 0)
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_RTMP_CONNECT, DEF_ON) == DEF_FAIL)
			vTaskDelay(1);
	}
	else if(strcmp("off", argv[1]) == 0)
	{
		while(MsgPut(&gptMsgCpu1to0, MSG_CMD_RTMP_DISCONNECT, DEF_ON) == DEF_FAIL)
			vTaskDelay(1);
	}

	return 0;
	UNUSED(argc);
}
#endif
