//*************************************************************************************************
// User shell commands
//*************************************************************************************************

#include <stdlib.h>		// for atoi
#include "dev.h"
#include "dev_inline.h"	// to use fast code

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sys_arch.h"
#include "lwip/sys.h"	// for sys_thread_new

#include "shell.h"
#include "shell_cmd.h"
#include "shell_cmd_sdio.h"

#include "msg.h"
#include "user.h"

#include "init.h"
#include "file.h"

const char *sSdcdTest[] = {"SDIO0 test",    			(char*)0};
const char *sSdiochainTest[] = {"SDIO Chain test",  	(char*)0};

//*************************************************************************************************
// User functions
//-------------------------------------------------------------------------------------------------
//
static void sdio0_state(void)
{
	printf("SDIO0 setting ===========\r\n");
	printf("¦§SDIO0_CLK_DIV      : %d\r\n", SDIO0_CLK_DIV);
	printf("¦§SDIO0_EN           : %d\r\n", SDIO0_EN);
	printf("¦§SDIO0_CLKEN        : %d\r\n", SDIO0_CLKEN);
	printf("¦§SDIO0_MODE         : %d\r\n", SDIO0_MODE);
	printf("¦§SDIO0_DET          : %d\r\n", SDIO0_DET);
	printf("¦§SDIO0_DET_IRQ      : %d\r\n", SDIO0_DET_IRQ);
	printf("¦§SDIO0_DET_IRQ_EN   : %d\r\n", SDIO0_DET_IRQ_EN);
	printf("¦§SDIO0_DET_IRQ_CLR  : %d\r\n", SDIO0_DET_IRQ_CLR);
	printf("¦§SDIO0_CHIP_IRQ     : %d\r\n", SDIO0_CHIP_IRQ);
	printf("¦§SDIO0_CHIP_IRQ_EN  : %d\r\n", SDIO0_CHIP_IRQ_EN);
	printf("¦§SDIO0_CHIP_IRQ_CLR : %d\r\n", SDIO0_CHIP_IRQ_CLR);
	printf("¦§SDIO0_DAT_IRQ      : %d\r\n", SDIO0_DAT_IRQ);
	printf("¦§SDIO0_DAT_IRQ_EN   : %d\r\n", SDIO0_DAT_IRQ_EN);
	printf("¦§SDIO0_DAT_IRQ_CLR  : %d\r\n", SDIO0_DAT_IRQ_CLR);
	printf("¦§SDIO0_CMD_IRQ      : %d\r\n", SDIO0_CMD_IRQ);
	printf("¦§SDIO0_CMD_IRQ_EN   : %d\r\n", SDIO0_CMD_IRQ_EN);
	printf("¦§SDIO0_CMD_IRQ_CLR  : %d\r\n", SDIO0_CMD_IRQ_CLR);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO0_DAT_BL       : %d\r\n", SDIO0_DAT_BL);
	printf("¦§SDIO0_DAT_WE       : %d\r\n", SDIO0_DAT_WE);
	printf("¦§SDIO0_DAT_EN       : %d\r\n", SDIO0_DAT_EN);
	printf("¦§SDIO0_CMD_EN       : %d\r\n", SDIO0_CMD_EN);
	printf("¦§SDIO0_CMD_IDX      : %d\r\n", SDIO0_CMD_IDX);
	printf("¦§SDIO0_CMD_RESPTYPE : %d\r\n", SDIO0_CMD_RESPTYPE);
	printf("¦§SDIO0_CMD_RESPEN   : %d\r\n", SDIO0_CMD_RESPEN);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO0_BUSY         : %d\r\n", SDIO0_BUSY);
	printf("¦§SDIO0_DAT_FULL     : %d\r\n", SDIO0_DAT_FULL);
	printf("¦§SDIO0_DAT_EMPTY    : %d\r\n", SDIO0_DAT_EMPTY);
	printf("¦§SDIO0_DAT_CRCERR   : %d\r\n", SDIO0_DAT_CRCERR);
	printf("¦§SDIO0_CMD_RESP_IDX : %d\r\n", SDIO0_CMD_RESP_IDX);
	printf("¦§SDIO0_CMD_RESP_CRC : %d\r\n", SDIO0_CMD_RESP_CRC);
	printf("¦§SDIO0_CMD_RESPTOUT : %d\r\n", SDIO0_CMD_RESPTOUT);
	printf("¦§SDIO0_CMD_CRCERR   : %d\r\n", SDIO0_CMD_CRCERR);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO0_CMD_ARG      : %d\r\n", SDIO0_CMD_ARG);
	printf("¦§SDIO0_RESP0        : %d\r\n", SDIO0_RESP0);
	printf("¦§SDIO0_RESP1        : %d\r\n", SDIO0_RESP1);
	printf("¦§SDIO0_RESP2        : %d\r\n", SDIO0_RESP2);
	printf("¦§SDIO0_RESP3        : %d\r\n", SDIO0_RESP3);
	printf("¦§SDIO0_DATLEN       : %d\r\n", SDIO0_DATLEN);
	printf("¦§SDIO0_CMD_TIMEOUT  : %d\r\n", SDIO0_CMD_TIMEOUT);
	printf("¦¦SDIO0_DAT_TIMEOUT  : %d\r\n", SDIO0_DAT_TIMEOUT);
	printf("=========================\r\n");
}

static void sdio1_state(void)
{
	printf("SDIO1 setting ===========\r\n");
	printf("¦§SDIO1_CLK_DIV      : %d\r\n", SDIO1_CLK_DIV);
	printf("¦§SDIO1_EN           : %d\r\n", SDIO1_EN);
	printf("¦§SDIO1_CLKEN        : %d\r\n", SDIO1_CLKEN);
	printf("¦§SDIO1_MODE         : %d\r\n", SDIO1_MODE);
	printf("¦§SDIO1_DET          : %d\r\n", SDIO1_DET);
	printf("¦§SDIO1_DET_IRQ      : %d\r\n", SDIO1_DET_IRQ);
	printf("¦§SDIO1_DET_IRQ_EN   : %d\r\n", SDIO1_DET_IRQ_EN);
	printf("¦§SDIO1_DET_IRQ_CLR  : %d\r\n", SDIO1_DET_IRQ_CLR);
	printf("¦§SDIO1_CHIP_IRQ     : %d\r\n", SDIO1_CHIP_IRQ);
	printf("¦§SDIO1_CHIP_IRQ_EN  : %d\r\n", SDIO1_CHIP_IRQ_EN);
	printf("¦§SDIO1_CHIP_IRQ_CLR : %d\r\n", SDIO1_CHIP_IRQ_CLR);
	printf("¦§SDIO1_DAT_IRQ      : %d\r\n", SDIO1_DAT_IRQ);
	printf("¦§SDIO1_DAT_IRQ_EN   : %d\r\n", SDIO1_DAT_IRQ_EN);
	printf("¦§SDIO1_DAT_IRQ_CLR  : %d\r\n", SDIO1_DAT_IRQ_CLR);
	printf("¦§SDIO1_CMD_IRQ      : %d\r\n", SDIO1_CMD_IRQ);
	printf("¦§SDIO1_CMD_IRQ_EN   : %d\r\n", SDIO1_CMD_IRQ_EN);
	printf("¦§SDIO1_CMD_IRQ_CLR  : %d\r\n", SDIO1_CMD_IRQ_CLR);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO1_DAT_BL       : %d\r\n", SDIO1_DAT_BL);
	printf("¦§SDIO1_DAT_WE       : %d\r\n", SDIO1_DAT_WE);
	printf("¦§SDIO1_DAT_EN       : %d\r\n", SDIO1_DAT_EN);
	printf("¦§SDIO1_CMD_EN       : %d\r\n", SDIO1_CMD_EN);
	printf("¦§SDIO1_CMD_IDX      : %d\r\n", SDIO1_CMD_IDX);
	printf("¦§SDIO1_CMD_RESPTYPE : %d\r\n", SDIO1_CMD_RESPTYPE);
	printf("¦§SDIO1_CMD_RESPEN   : %d\r\n", SDIO1_CMD_RESPEN);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO1_BUSY         : %d\r\n", SDIO1_BUSY);
	printf("¦§SDIO1_DAT_FULL     : %d\r\n", SDIO1_DAT_FULL);
	printf("¦§SDIO1_DAT_EMPTY    : %d\r\n", SDIO1_DAT_EMPTY);
	printf("¦§SDIO1_DAT_CRCERR   : %d\r\n", SDIO1_DAT_CRCERR);
	printf("¦§SDIO1_CMD_RESP_IDX : %d\r\n", SDIO1_CMD_RESP_IDX);
	printf("¦§SDIO1_CMD_RESP_CRC : %d\r\n", SDIO1_CMD_RESP_CRC);
	printf("¦§SDIO1_CMD_RESPTOUT : %d\r\n", SDIO1_CMD_RESPTOUT);
	printf("¦§SDIO1_CMD_CRCERR   : %d\r\n", SDIO1_CMD_CRCERR);
	printf("¦¢=======================\r\n");
	printf("¦§SDIO1_CMD_ARG      : 0x%08X\r\n", SDIO1_CMD_ARG);
	printf("¦§SDIO1_RESP0        : 0x%08X\r\n", SDIO1_RESP0);
	printf("¦§SDIO1_RESP1        : 0x%08X\r\n", SDIO1_RESP1);
	printf("¦§SDIO1_RESP2        : 0x%08X\r\n", SDIO1_RESP2);
	printf("¦§SDIO1_RESP3        : 0x%08X\r\n", SDIO1_RESP3);
	printf("¦§SDIO1_DATLEN       : %d\r\n", SDIO1_DATLEN);
	printf("¦§SDIO1_CMD_TIMEOUT  : %d\r\n", SDIO1_CMD_TIMEOUT);
	printf("¦¦SDIO1_DAT_TIMEOUT  : %d\r\n", SDIO1_DAT_TIMEOUT);
	printf("=========================\r\n");
}

#if 0
#if 0
void irq_sdio0_func(void *ctx)
{
	BYTE *buff;
	int i;
	static UINT tx_pos = 0;
	static UINT rx_pos = 0;
	static UINT rx_count = 0;
	if(SDIO0_CMD_IRQ)
	{
		SDIO0_CMD_IRQ_CLR = 1;
		switch(SDIO0_CMD_RESP_IDX)
		{
			case 10:
				SDIO0_CMD_RESPEN = 1;
				SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
				SDIO0_CMD_ARG = SDIO0_RESP0*10;
				break;
			case 11:
				SDIO0_CMD_RESPEN = 1;
				SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
				SDIO0_CMD_ARG = SDIO0_RESP0*11;
				break;
			case 12:
				SDIO0_CMD_RESPEN = 1;
				SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
				SDIO0_CMD_ARG = SDIO0_RESP0*12;
				break;
			case 20:
				SDIO0_CMD_RESPEN = 0;
				break;
			case 21:
				SDIO0_CMD_RESPEN = 0;
				break;
			case 22:
				SDIO0_CMD_RESPEN = 0;
				break;
			case 50:
				SDIO0_CMD_RESPEN = 1;
				rx_pos = 0;
				rx_count = SDIO0_RESP0;
				SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
				SDIO0_CMD_ARG = SDIO0_RESP0;
				break;
			default:
				SDIO0_CMD_RESPEN = 0;
				printf("%s(%d) : unknown CMD(%d)\r\n", __func__, __LINE__, SDIO0_CMD_RESP_IDX);
				break;
		}
		SDIO0_CMD_EN = 1;
		printf("%s(%d) : CMD In : SDIO0_CMD_RESP_IDX(%d) SDIO0_RESP0(%d)\r\n", __func__, __LINE__, SDIO0_CMD_RESP_IDX, SDIO0_RESP0);
		if(SDIO0_CMD_IDX == 50)
		{
			printf("%s(%d) : DATA RX start\r\n", __func__, __LINE__);
			SDIO0_DAT_WE = 0;
			SDIO0_DAT_EN = 1;
			buff = &sdioch_s_rx_data[rx_pos];
			DmaMemCpy_ip(buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
			invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO0_DAT_BL);
			SDIO0_DAT_EN = 0;
			printf("%s(%d) : DATA RX end\r\n", __func__, __LINE__);
		}
	}
	if(SDIO0_DAT_IRQ)
	{
		SDIO0_DAT_IRQ_CLR = 1;
		printf("%s(%d) : DAT In(rx_cnt:%d, rx_pos:%d) SDIO0_DAT_EN(%d)\r\n", __func__, __LINE__, rx_count, rx_pos, SDIO0_DAT_EN);
#if 0
		buff = &sdioch_s_rx_data[rx_pos];
		while(SDIO0_DAT_EN);
			NOP();
//		while(SDIO0_DAT_EMPTY)
//			NOP();
		printf("%s(%d) : \r\n", __func__, __LINE__);
		DmaMemCpy_ip(buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
		invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO0_DAT_BL);
		rx_pos += SDIO0_DAT_BL;
		printf("%s(%d) : next pos(%d)\r\n", __func__, __LINE__, rx_pos);
		rx_count--;
		if(rx_count != 0)
		{
			SDIO0_DAT_WE = 0;
			SDIO0_DAT_EN = 1;
		}
#endif
	}
	if(SDIO0_CHIP_IRQ)
	{
		SDIO0_CHIP_IRQ_CLR = 1;
		printf("%s(%d) : CHIP In\r\n", __func__, __LINE__);
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	UNUSED(ctx);
}
#else
void irq_sdio0_func(void *ctx)
{
	BYTE *buff;
	int i;
	static UINT tx_pos = 0;
	static UINT rx_pos = 0;
	static UINT rx_count = 0;
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	for(;;)
	{
		if(SDIO0_CMD_IRQ)
		{
			SDIO0_CMD_IRQ_CLR = 1;
			switch(SDIO0_CMD_RESP_IDX)
			{
				case 10:
					SDIO0_CMD_RESPEN = 1;
					SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
					SDIO0_CMD_ARG = SDIO0_RESP0*10;
					break;
				case 11:
					SDIO0_CMD_RESPEN = 1;
					SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
					SDIO0_CMD_ARG = SDIO0_RESP0*11;
					break;
				case 12:
					SDIO0_CMD_RESPEN = 1;
					SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
					SDIO0_CMD_ARG = SDIO0_RESP0*12;
					break;
				case 20:
					SDIO0_CMD_RESPEN = 0;
					break;
				case 21:
					SDIO0_CMD_RESPEN = 0;
					break;
				case 22:
					SDIO0_CMD_RESPEN = 0;
					break;
				case 50:
					SDIO0_CMD_RESPEN = 1;
					rx_pos = 0;
					rx_count = SDIO0_RESP0;
					SDIO0_CMD_IDX = SDIO0_CMD_RESP_IDX;
					SDIO0_CMD_ARG = SDIO0_RESP0;
					break;
				default:
					SDIO0_CMD_RESPEN = 0;
					printf("%s(%d) : unknown CMD(%d)\r\n", __func__, __LINE__, SDIO0_CMD_RESP_IDX);
					break;
			}
			SDIO0_CMD_EN = 1;
			printf("%s(%d) : CMD In : SDIO0_CMD_RESP_IDX(%d) SDIO0_RESP0(%d)\r\n", __func__, __LINE__, SDIO0_CMD_RESP_IDX, SDIO0_RESP0);
			if(SDIO0_CMD_RESP_IDX == 50)
			{
				printf("%s(%d) : data rx start\r\n", __func__, __LINE__);
				SDIO0_DAT_WE = 0;
				SDIO0_DAT_EN = 1;
				buff = &sdioch_s_rx_data[rx_pos];
				while(SDIO0_DAT_EN)
					vTaskDelay(1);
				for(;rx_count;rx_count--)
				{
					printf("%s(%d) : 1\r\n", __func__, __LINE__);
					while(SDIO0_DAT_EMPTY)
						vTaskDelay(1);
					printf("%s(%d) : 2\r\n", __func__, __LINE__);
					DmaMemCpy_ip(buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
					invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO0_DAT_BL);
					rx_pos += SDIO0_DAT_BL;
					printf("%s(%d) : next pos(%d)\r\n", __func__, __LINE__, rx_pos);
				}
				printf("%s(%d) : data rx end\r\n", __func__, __LINE__);
				SDIO0_DAT_WE = 0;
				SDIO0_DAT_EN = 0;
			}
		}
		if(SDIO0_DAT_IRQ)
		{
			SDIO0_DAT_IRQ_CLR = 1;
			printf("%s(%d) : DAT In(rx_cnt:%d, rx_pos:%d) SDIO0_DAT_EN(%d)\r\n", __func__, __LINE__, rx_count, rx_pos, SDIO0_DAT_EN);
#if 1
			SDIO0_DAT_WE = 0;
			SDIO0_DAT_EN = 1;
			buff = &sdioch_s_rx_data[rx_pos];
			while(SDIO0_DAT_EN)
				vTaskDelay(1);
			for(;rx_count;rx_count--)
			{
				printf("%s(%d) : 1\r\n", __func__, __LINE__);
				while(SDIO0_DAT_EMPTY)
					vTaskDelay(1);
				printf("%s(%d) : 2\r\n", __func__, __LINE__);
				DmaMemCpy_ip(buff, (BYTE *)SDIO0_BASE, SDIO0_DAT_BL);
				invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO0_DAT_BL);
				rx_pos += SDIO0_DAT_BL;
				printf("%s(%d) : next pos(%d)\r\n", __func__, __LINE__, rx_pos);
			}
			if(rx_count == 0)
			{
				SDIO0_DAT_WE = 0;
				SDIO0_DAT_EN = 0;
			}
#endif
		}
		if(SDIO0_CHIP_IRQ)
		{
			SDIO0_CHIP_IRQ_CLR = 1;
			printf("%s(%d) : CHIP In\r\n", __func__, __LINE__);
		}
		vTaskDelay(1);
	}
}
#endif

void irq_sdio1_func(void *ctx)
{
	if(SDIO1_CMD_IRQ)
	{
		SDIO1_CMD_IRQ_CLR = 1;
		switch(SDIO1_CMD_RESP_IDX)
		{
			case 10:
				SDIO1_CMD_RESPEN = 1;
				SDIO1_CMD_IDX = SDIO1_CMD_RESP_IDX;
				SDIO1_CMD_ARG = SDIO1_RESP0*10;
				break;
			case 11:
				SDIO1_CMD_RESPEN = 1;
				SDIO1_CMD_IDX = SDIO1_CMD_RESP_IDX;
				SDIO1_CMD_ARG = SDIO1_RESP0*11;
				break;
			case 12:
				SDIO1_CMD_RESPEN = 1;
				SDIO1_CMD_IDX = SDIO1_CMD_RESP_IDX;
				SDIO1_CMD_ARG = SDIO1_RESP0*12;
				break;
			case 20:
				SDIO1_CMD_RESPEN = 0;
				break;
			case 21:
				SDIO1_CMD_RESPEN = 0;
				break;
			case 22:
				SDIO1_CMD_RESPEN = 0;
				break;
		}
		SDIO1_CMD_EN = 1;
		printf("%s(%d) : CMD In : SDIO1_CMD_RESP_IDX(%d) SDIO1_RESP0(%d)\r\n", __func__, __LINE__, SDIO1_CMD_RESP_IDX, SDIO1_RESP0);
	}
	if(SDIO1_DAT_IRQ)
	{
		SDIO1_DAT_IRQ_CLR = 1;
		printf("%s(%d) : DAT In\r\n", __func__, __LINE__);
	}
	if(SDIO1_CHIP_IRQ)
	{
		SDIO1_CHIP_IRQ_CLR = 1;
		printf("%s(%d) : CHIP In\r\n", __func__, __LINE__);
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	UNUSED(ctx);
}
#endif

#if 0
BOOL sdio0_tx_cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe)
{
	SDIO0_CMD_IDX = Cmd;
	SDIO0_CMD_ARG = Arg;
	SDIO0_CMD_RESPEN = RespEn;
	SDIO0_CMD_RESPTYPE = RespType;
	SDIO0_DAT_WE = DatWe;
	SDIO0_DAT_EN = DatEn;
	SDIO0_CMD_EN = 1;

	while(SDIO0_CMD_EN)
		vTaskDelay(1);
	vTaskDelay(10);

	if(SDIO0_CMD_CRCERR | SDIO0_CMD_RESPTOUT)
	{
		printf("SDIO0 : CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO0_CMD_CRCERR, SDIO0_CMD_RESP_CRC, SDIO0_CMD_RESPTOUT);
		return DEF_FAIL;
	}
	return DEF_OK;
}

BOOL sdio1_tx_cmd(BYTE Cmd, UINT Arg, BYTE RespEn, BYTE RespType, BYTE DatEn, BYTE DatWe)
{
	SDIO1_CMD_IDX = Cmd;
	SDIO1_CMD_ARG = Arg;
	SDIO1_CMD_RESPEN = RespEn;
	SDIO1_CMD_RESPTYPE = RespType;
	SDIO1_DAT_WE = DatWe;
	SDIO1_DAT_EN = DatEn;
	SDIO1_CMD_EN = 1;

	while(SDIO1_CMD_EN)
		vTaskDelay(1);
	vTaskDelay(10);

	if(SDIO1_CMD_CRCERR | SDIO1_CMD_RESPTOUT)
	{
		printf("SDIO1 : CMD(%d) Arg(%d) CRCERR(%d:0x%02X), RESPTOUT(%d)\r\n", Cmd, Arg, SDIO1_CMD_CRCERR, SDIO1_CMD_RESP_CRC, SDIO1_CMD_RESPTOUT);
		return DEF_FAIL;
	}
	return DEF_OK;
}

BOOL sdio0_tx_data(const BYTE *buff, UINT count)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	printf("count(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", count, SDIO0_DAT_BL, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL);

	while(SDIO0_DAT_EN);

	SDIO0_DATLEN = count;

	bRes = sdio0_tx_cmd(24, count, 1, 0, 1, 1);
	nResp = SDIO0_RESP0;

	if(bRes == DEF_OK)
	{
		DmaMemCpy_ip((BYTE *)SDIO0_BASE, (BYTE *)buff, SDIO0_DAT_BL);

		while(SDIO0_DAT_EN);
		SDIO0_DAT_WE = 0;

		if(SDIO0_DAT_CRCERR)
		{
			printf("CRC Error\r\n");
			printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO0_DAT_EN = 0;
		SDIO0_DAT_WE = 0;
		printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_CRCERR == 0 ? 'T' : 'F', SDIO0_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

	printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO0_DAT_EMPTY, SDIO0_DAT_FULL, SDIO0_DAT_CRCERR == 0 ? 'T' : 'F');

	return bRes;
}

BOOL sdio1_tx_data(const BYTE *buff, UINT count)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	printf("[24] count(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", count, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = count;

	bRes = sdio1_tx_cmd(24, count, 1, 0, 1, 1);
	nResp = SDIO1_RESP0;

	if(bRes == DEF_OK)
	{
		DmaMemCpy_ip((BYTE *)SDIO1_BASE, (BYTE *)buff, SDIO1_DAT_BL);

		while(SDIO1_DAT_EN);
		SDIO1_DAT_WE = 0;

		if(SDIO1_DAT_CRCERR)
		{
			printf("CRC Error\r\n");
			printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}
	}
	else
	{
		SDIO1_DAT_EN = 0;
		SDIO1_DAT_WE = 0;
		printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

	printf("[24] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

	return bRes;
}

BOOL sdio1_rx_data(const BYTE *buff, UINT count)
{
	BOOL bRes = DEF_FAIL;
	UINT nResp = 0;

	printf("[17] count(%d) BL(%d) DAT_EMPTY(%d) DAT_FULL(%d)\r\n", count, SDIO1_DAT_BL, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL);

	while(SDIO1_DAT_EN);

	SDIO1_DATLEN = count;

	bRes = sdio1_tx_cmd(17, count, 1, 0, 1, 0);
	nResp = SDIO1_RESP0;

	if(bRes == DEF_OK)
	{
		while(SDIO1_DAT_EN);

		if(SDIO1_DAT_CRCERR)
		{
			printf("CRC Error\r\n");
			printf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
		}

		DmaMemCpy_ip((BYTE *)buff, (BYTE *)SDIO1_BASE, SDIO1_DAT_BL);
		invalidate_dcache_range((UINT)buff, ((UINT)buff) + SDIO1_DAT_BL);
	}
	else
	{
		SDIO1_DAT_EN = 0;
		printf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c) CMD_CRC(%c) CMD_RESTTOUT(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_CRCERR == 0 ? 'T' : 'F', SDIO1_CMD_RESPTOUT == 0 ? 'T' : 'F');
	}

	printf("[17] res(%d) RESP(0x%08X) DAT_EMPTY(%d) DAT_FULL(%d) DAT_CRC(%c)\r\n", bRes, nResp, SDIO1_DAT_EMPTY, SDIO1_DAT_FULL, SDIO1_DAT_CRCERR == 0 ? 'T' : 'F');

	return bRes;
}

INT16S cmd_sdio_chain_test(INT32S argc, char *argv[])
{
#define SDIO0_MASTER 0
	int i, count;
	printf("%s(%d) : Start\r\n", __func__, __LINE__);
	if(argc >= 2)
	{
		if(strcmp(argv[1], "init") == 0)
		{
			//SDIO CH0
			SDIO0_EN = 1;
			SDIO0_CLK_DIV = 0;
			SDIO0_DAT_BL = 512;
			SDIO0_CMD_TIMEOUT = 10000;

			//SDIO CH1
			SDIO1_EN = 1;
			SDIO1_CLK_DIV = 0;
			SDIO1_DAT_BL = 512;
			SDIO1_CMD_TIMEOUT = 10000;

#if SDIO0_MASTER
			SDIO0_CLKEN = 1;
			SDIO0_MODE = 1;

			SDIO0_CMD_IRQ_EN = 0;
			SDIO0_DAT_IRQ_EN = 0;
			SDIO0_CHIP_IRQ_EN = 0;

			SDIO1_CLKEN = 0;
			SDIO1_MODE = 0;

			SDIO1_CMD_IRQ_EN = 1;
			SDIO1_DAT_IRQ_EN = 1;
			SDIO1_CHIP_IRQ_EN = 1;

//			IntAdd(IRQ_SDIO1, (void*)irq_sdio1_func, NULL);
//			Ei(IP_IRQ_SDIO1);
			printf("%s(%d) : SDIO0(M) SDIO1(S) init ok(%d,%d,%d,%d)\r\n", __func__, __LINE__, SDIO1_CMD_IRQ_EN, SDIO1_DAT_IRQ_EN, SDIO1_CHIP_IRQ_EN, SDIO1_DET_IRQ_EN);
#else
			SDIO0_CLKEN = 0;
			SDIO0_MODE = 0;

			SDIO0_CMD_IRQ_EN = 1;
			SDIO0_DAT_IRQ_EN = 1;
			SDIO0_CHIP_IRQ_EN = 1;

			SDIO1_CLKEN = 1;
			SDIO1_MODE = 1;

			SDIO1_CMD_IRQ_EN = 0;
			SDIO1_DAT_IRQ_EN = 0;
			SDIO1_CHIP_IRQ_EN = 0;

//			IntAdd(IP_IRQ_SDIO0, (void*)irq_sdio0_func, NULL);
//			Ei(IP_IRQ_SDIO0);
//			sys_thread_new("sd0irq", irq_sdio0_func, NULL, TIMER_STACK_SIZE, TIMER_TASK_PRIO);

			printf("%s(%d) : SDIO0(S) SDIO1(M) init ok(%d,%d,%d,%d)\r\n", __func__, __LINE__, SDIO0_CMD_IRQ_EN, SDIO0_DAT_IRQ_EN, SDIO0_CHIP_IRQ_EN, SDIO0_DET_IRQ_EN);
#endif
		}
		else if(strcmp(argv[1], "ctx0") == 0)
		{
			UINT tx_cmd = atoi(argv[2]);
			UINT tx_arg = atoi(argv[3]);
			printf("input tx_cmd(%d), tx_arg(%d)\r\n", tx_cmd, tx_arg);
#if SDIO0_MASTER
			sdio0_tx_cmd(tx_cmd, tx_arg, 0, 0, 0, 0);
#else
			sdio1_tx_cmd(tx_cmd, tx_arg, 0, 0, 0, 0);
#endif
			printf("tx cmd end\r\n");
		}
		else if(strcmp(argv[1], "ctx1") == 0)
		{
			UINT tx_cmd = atoi(argv[2]);
			UINT tx_arg = atoi(argv[3]);
			printf("input tx_cmd(%d), tx_arg(%d)\r\n", tx_cmd, tx_arg);
#if SDIO0_MASTER
			if(sdio0_tx_cmd(tx_cmd, tx_arg, 1, 0, 0, 0) == DEF_OK)
			{
				printf("resp0 %d(%X)\r\n", SDIO0_RESP0, SDIO0_RESP0);
			}
#else
			if(sdio1_tx_cmd(tx_cmd, tx_arg, 1, 0, 0, 0) == DEF_OK)
			{
				printf("resp0 %d(%X)\r\n", SDIO1_RESP0, SDIO1_RESP0);
			}
#endif
			printf("tx cmd end\r\n");
		}
		else if(strcmp(argv[1], "data") == 0)
		{
			if(strcmp(argv[2], "rand") == 0)
			{
				for(i=0;i<4096;i++)
				{
					gptMsgSH01[i] = rand() % 255;
					gptMsgSH02[i] = rand() % 255;
				}
				printf("%s(%d) : data rand OK\r\n", __func__, __LINE__);
			}
			else if(strcmp(argv[2], "clear") == 0)
			{
				for(i=0;i<4096;i++)
				{
					gptMsgSH01[i] = 0;
					gptMsgSH02[i] = 0;
				}
				printf("%s(%d) : data clear OK\r\n", __func__, __LINE__);
			}
		}
		else if(strcmp(argv[1], "hex") == 0)
		{
			if(argc == 3)
			{
				int print_len = atoi(argv[2]);
				for(i=0;i<print_len;i++)
				{
					if(gptMsgSH01[i] != gptMsgSH02[i])
					{
						printf("mismatch! idx(%d) mtx(0x%02X) != srx(0x%02X)\r\n", i, gptMsgSH01[i], gptMsgSH02[i]);
						break;
					}
				}
				hexDump("m_tx", (void *)gptMsgSH01, print_len);
				hexDump("s_rx", (void *)gptMsgSH02, print_len);
			}
			else
			{
				cmd_unknown();
			}
		}
		else if(strcmp(argv[1], "dtx") == 0)
		{
			count = atoi(argv[2]);
			printf("data tx(m->s) : input count(%d)\r\n", count);
#if SDIO0_MASTER
			if(sdio0_tx_data((BYTE *)gptMsgSH01, count) == DEF_OK)
#else
			if(sdio1_tx_data((BYTE *)gptMsgSH01, count) == DEF_OK)
#endif
			{
				printf("tx cmd ok end\r\n");
			}
			else
			{
				printf("tx cmd fail end\r\n");
			}
		}
		else if(strcmp(argv[1], "drx") == 0)
		{
			count = atoi(argv[2]);
			printf("data rx(s->m) : input count(%d)\r\n", count);
#if SDIO0_MASTER
			if(sdio0_rx_data((BYTE *)gptMsgSH01, count) == DEF_OK)
#else
			if(sdio1_rx_data((BYTE *)gptMsgSH01, count) == DEF_OK)
#endif
			{
				printf("rx cmd ok end\r\n");
			}
			else
			{
				printf("rx cmd fail end\r\n");
			}
		}
		else if(strcmp(argv[1], "st") == 0)
		{
			if(strcmp(argv[2], "0") == 0)
			{
				sdio0_state();
			}
			else if(strcmp(argv[2], "1") == 0)
			{
				sdio1_state();
			}
			else
			{
				cmd_unknown();
			}
		}
		else if(strcmp(argv[1], "log") == 0)
		{
//0x1 : readsb
//0x2 : read_pbuf
//0x4 : writesb
//0x8 : write_pbuf
			UINT v = atoi(argv[2]);
			if(v <= 0xFF)
			{
				gptMsgShare.WIFI_INTER_LOG = v;
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
	else
	{
		cmd_unknown();
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	return 0;
}
#else
INT16S cmd_sdio_chain_test(INT32S argc, char *argv[])
{
	if (strcmp(argv[1], "st") == 0) {
		if (strcmp(argv[2], "0") == 0) {
			sdio0_state();
		} else if(strcmp(argv[2], "1") == 0) {
			sdio1_state();
		} else {
			cmd_unknown();
		}
	} else {
		cmd_unknown();
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
	return 0;
}
#endif

#ifdef __DEVICE_SD__
BYTE nSectorTestW[4096];
BYTE nSectorTestR[4096];
sys_thread_t stt_sdcheck = NULL;

int sdcheck_switch = 0;
void cmd_sdcheck_task_s_write(void *ctx)
{
	UINT getvalue = 0;
	int i;
	sdcheck_switch = 1;
	for(i=0;i<512;i++)
	{
		nSectorTestW[i] = i;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s = 0, wtime_e = 0, tcnt = 0;
	printf("loop sdcard single sector write start(Max Sector:%d)\r\n", getSector);
	
	while(1)
	{
		if(tcnt == 0)
			wtime_s = xTaskGetTickCount();

		Sdio0SectorW(nSectorTestW, getvalue);
		tcnt++;

		if(tcnt == 1000)
			wtime_e = xTaskGetTickCount();

//		vTaskDelay(1);

		if(tcnt == 1000)
		{
			printf("write sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);
			tcnt = 0;
		}

		getvalue++;
		
		if(getvalue > getSector)
			getvalue = 0;

		if(sdcheck_switch == 2)
			break;
	}
	sdcheck_switch = 0;

	printf("task delete\r\n");
	
	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_s_read(void *ctx)
{
	UINT getvalue = 0;
	int i;
	sdcheck_switch = 1;
	for(i=0;i<512;i++)
	{
		nSectorTestR[i] = i;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s = 0, wtime_e = 0, tcnt = 0;
	printf("loop sdcard single sector read start(Max Sector:%d)\r\n", getSector);
	
	while(1)
	{
		if(tcnt == 0)
			wtime_s = xTaskGetTickCount();

		Sdio0SectorR(nSectorTestR, getvalue);
		tcnt++;

		if(tcnt == 1000)
			wtime_e = xTaskGetTickCount();

//		vTaskDelay(1);

		if(tcnt == 1000)
		{
			printf("read sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);
			tcnt = 0;
		}

		getvalue++;
		
		if(getvalue > getSector)
			getvalue = 0;

		if(sdcheck_switch == 2)
			break;
	}
	sdcheck_switch = 0;

	printf("task delete\r\n");
	
	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_m_write(void *ctx)
{
	const int test_size = (512*1024);
	
	BYTE *arrBuffer = (BYTE *)pvPortMalloc(test_size);
	if(arrBuffer == NULL)
	{
		printf("malloc fail\r\n");
		goto done;
	}

	UINT getvalue = 0;
	int i;
	sdcheck_switch = 1;
	for(i=0;i<test_size;i++)
	{
		arrBuffer[i] = i;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s, wtime_e;
	printf("loop sdcard multi sector write start(Max Sector:%d)\r\n", getSector);

	while(1)
	{
		printf("write data(0x%08X) sector num(%d) (%d)\r\n", arrBuffer, getvalue, sdcheck_switch);
//		printf("write sector num(%d) (%d)\r\n", getvalue, sdcheck_switch);
		wtime_s = xTaskGetTickCount();
		Sdio0W(arrBuffer, getvalue, 1024);
		wtime_e = xTaskGetTickCount();

//		vTaskDelay(1);

		printf("write sector num(%d) (%dms) (%d)\r\n", getvalue, (wtime_e - wtime_s)*10, sdcheck_switch);

		getvalue += 1024;

		if(getvalue > getSector)
			getvalue = 0;

		printf("write sector num(%d) (%d)\r\n", getvalue, sdcheck_switch);

		if(sdcheck_switch == 2)
		{
			asm("l.trap 0");
			break;
		}
	}
	sdcheck_switch = 0;

	vPortFree(arrBuffer);
done:
	printf("task delete\r\n");

	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_m_write_100(void *ctx)
{
	const int test_size = (512*1024);

	BYTE *arrBuffer = (BYTE *)pvPortMalloc(test_size);
	if(arrBuffer == NULL)
	{
		printf("malloc fail\r\n");
		goto done;
	}

	UINT getvalue = 0;
	int i;
	sdcheck_switch = 1;
	for(i=0;i<test_size;i++)
	{
		arrBuffer[i] = i;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s, wtime_e, wtime_ts, wtime_te;
	printf("loop sdcard multi sector write start(Max Sector:%d)\r\n", getSector);

	wtime_ts = xTaskGetTickCount();
	for(i=0;i<100;i++)
	{
		wtime_s = xTaskGetTickCount();
		Sdio0W(arrBuffer, getvalue, 1024);
		wtime_e = xTaskGetTickCount();

//		vTaskDelay(1);

		printf("write sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

		getvalue += 1024;
		
		if(getvalue > getSector)
			getvalue = 0;

		if(sdcheck_switch == 2)
		{
			break;
		}
	}
	wtime_te = xTaskGetTickCount();
	printf("total write sector num(%d) (%dms)\r\n", getvalue, ((wtime_te - wtime_ts)*10));

	sdcheck_switch = 0;

	vPortFree(arrBuffer);
done:
	printf("task delete\r\n");

	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_m_read(void *ctx)
{
	const int test_size = (512*1024);

	BYTE *arrBuffer = (BYTE *)pvPortMalloc(test_size);
	if(arrBuffer == NULL)
	{
		printf("malloc fail\r\n");
		goto done;
	}

	UINT getvalue = 0;
	int i;
	sdcheck_switch = 1;
	for(i=0;i<test_size;i++)
	{
		arrBuffer[i] = i;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s, wtime_e;
	printf("loop sdcard multi sector read start(Max Sector:%d)\r\n", getSector);

	while(1)
	{
		wtime_s = xTaskGetTickCount();
		Sdio0R(arrBuffer, getvalue, 1024);
		wtime_e = xTaskGetTickCount();

//		vTaskDelay(1);

		printf("read sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

		getvalue += 1024;
		
		if(getvalue > getSector)
			getvalue = 0;

		if(sdcheck_switch == 2)
		{
			break;
		}
	}
	sdcheck_switch = 0;

	vPortFree(arrBuffer);
done:
	printf("task delete\r\n");

	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_s_wr(void *ctx)
{
	const UINT test_sector = (8*1024);
	const int test_size = (test_sector*512);	// 4MByte

	UINT wtime_s, wtime_e, err_cnt;
	BYTE *arrBuffer;
	wtime_s = xTaskGetTickCount();
	BYTE *arrBufferWrite = (BYTE *)pvPortMalloc(test_size);
	BYTE *arrBufferRead = (BYTE *)pvPortMalloc(test_size);
	if(arrBufferWrite == NULL)
	{
		printf("write malloc fail\r\n");
		goto done;
	}
	if(arrBufferRead == NULL)
	{
		printf("read malloc fail\r\n");
		goto done;
	}
	wtime_e = xTaskGetTickCount();	
	printf("buffer malloc time(%dms)\r\n", (wtime_e - wtime_s)*10);
	printf("buffer malloc write pos(0x%08X ~ 0x%08X)\r\n", arrBufferWrite, arrBufferWrite + test_size - 1);
	printf("buffer malloc read pos(0x%08X ~ 0x%08X)\r\n", arrBufferRead, arrBufferRead + test_size - 1);

	UINT getvalue = 0;
	int i;
	wtime_s = xTaskGetTickCount();
	for(i=0;i<test_size;i++)
	{
		arrBufferWrite[i] = rand();
		arrBufferRead[i] = 0;
	}
	wtime_e = xTaskGetTickCount();	
	printf("buffer init time(%dms)\r\n", (wtime_e - wtime_s)*10);

	UINT getSector = Sdio0GetSectorCnt() - 10;

	printf("sdcard single sector write start(Max Sector:%d)\r\n", getSector);
	arrBuffer = arrBufferWrite;
	wtime_s = xTaskGetTickCount();
	for(getvalue=0;getvalue<test_sector;getvalue++)					
	{
		Sdio0SectorW(arrBuffer, getvalue);

		if(getvalue % 100 == 0)
			printf("%04d sdcard single sector write arrBuffer pos(0x%08X ~ 0x%08X)\r\n", getvalue, arrBuffer, arrBuffer + 512 - 1);

		arrBuffer += 512;
//		vTaskDelay(1);
	}
	wtime_e = xTaskGetTickCount();	
	printf("write sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

	printf("sdcard single sector read start(Max Sector:%d)\r\n", getSector);
	arrBuffer = arrBufferRead;
	wtime_s = xTaskGetTickCount();
	for(getvalue=0;getvalue<test_sector;getvalue++)
	{
		Sdio0SectorR(arrBuffer, getvalue);

		if(getvalue % 100 == 0)
			printf("%04d sdcard single sector read arrBuffer pos(0x%08X ~ 0x%08X)\r\n", getvalue, arrBuffer, arrBuffer + 512 - 1);
	
		arrBuffer += 512;
//		vTaskDelay(1);
	}
	wtime_e = xTaskGetTickCount();
	printf("read sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

	for(i=0,err_cnt=0;i<test_size;i++)
	{
		if(arrBufferRead[i] != arrBufferWrite[i])
		{
			err_cnt++;
		}
	}
	printf("sdcard single sector write/read check error(%d)\r\n", err_cnt);

	vPortFree(arrBufferWrite);
	vPortFree(arrBufferRead);
done:
	printf("task delete\r\n");

	vTaskDelete(NULL);
	UNUSED(ctx);
}

void cmd_sdcheck_task_m_wr(void *ctx)
{
	const UINT test_sector = (8*1024);
	const int test_size = (test_sector*512);	// 4MByte

	BYTE *arrBuffer;
	BYTE *arrBufferWrite = (BYTE *)pvPortMalloc(test_size);
	BYTE *arrBufferRead = (BYTE *)pvPortMalloc(test_size);
	if(arrBufferWrite == NULL)
	{
		printf("write malloc fail\r\n");
		goto done;
	}
	if(arrBufferRead == NULL)
	{
		printf("read malloc fail\r\n");
		goto done;
	}

	UINT getvalue = 0;
	int i;
	for(i=0;i<test_size;i++)
	{
		arrBufferWrite[i] = rand();
		arrBufferRead[i] = 0;
	}

	UINT getSector = Sdio0GetSectorCnt() - 10;
	UINT wtime_s, wtime_e, err_cnt;

	arrBuffer = arrBufferWrite;
	printf("sdcard multi sector write start(Max Sector:%d)\r\n", getSector);
	wtime_s = xTaskGetTickCount();
	for(getvalue=0;getvalue<test_sector;getvalue+=1024)
	{
		Sdio0W(arrBuffer, getvalue, 1024);
		arrBuffer += (512*1024);
//		vTaskDelay(1);
	}
	wtime_e = xTaskGetTickCount();	
	printf("write sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

	arrBuffer = arrBufferRead;
	printf("sdcard multi sector read start(Max Sector:%d)\r\n", getSector);
	wtime_s = xTaskGetTickCount();
	for(getvalue=0;getvalue<test_sector;getvalue+=1024)
	{
		Sdio0R(arrBuffer, getvalue, 1024);
		arrBuffer += (512*1024);
//		vTaskDelay(1);
	}
	wtime_e = xTaskGetTickCount();
	printf("read sector num(%d) (%dms)\r\n", getvalue, (wtime_e - wtime_s)*10);

	for(i=0,err_cnt=0;i<test_size;i++)
	{
		if(arrBufferRead[i] != arrBufferWrite[i])
		{
			err_cnt++;
		}
	}
	printf("sdcard multi sector write/read check error(%d)\r\n", err_cnt);

	vPortFree(arrBufferWrite);
	vPortFree(arrBufferRead);
done:
	printf("task delete\r\n");

	vTaskDelete(NULL);
	UNUSED(ctx);
}
#endif

INT16S cmd_sdcd_test(INT32S argc, char *argv[])
{
#ifdef __DEVICE_SD__
	int i, j, nFlag = 0;

	FRESULT fres;
	int getdata = 0, getvalue = 0, getvalue2 = 0;
	BYTE un8Temp;
	UINT un32Count = 0;
	if(argc >= 2)
		getdata = atoi(argv[1]);
	if(argc >= 3)
		getvalue = atoi(argv[2]);
	if(argc >= 4)
		getvalue2 = atoi(argv[3]);
	if(argc == 1)
		getdata = '?';

	printf("%s(%d) : Start(%d, %d)\r\n", __func__, __LINE__, getdata, getvalue);
	switch(getdata)
	{
		case '?':
			printf("%s 0 : set sdio0 clock\r\n", argv[0]);
			printf("%s 1 : sdio0 init\r\n", argv[0]);
			printf("%s 2 : cpu0 spr_sr\r\n", argv[0]);
			printf("%s 3 : micro sd card mount\r\n", argv[0]);
			printf("%s 4 : set sdio0 clock(cmd6)\r\n", argv[0]);
			printf("%s 5 : read sector\r\n", argv[0]);
			printf("%s 6 : write sector(0x00~0xff loop)\r\n", argv[0]);
			printf("%s 7 : write sector(input value)\r\n", argv[0]);
			printf("%s 8 : write sector 256 loop(0x00~0xff loop)\r\n", argv[0]);
			printf("%s 9 : write sector 256 loop(random)\r\n", argv[0]);
			printf("%s 10 : sector read/write memory base address\r\n", argv[0]);
			break;
		case 0:
			Sdio0ClockDiv(getvalue);
			break;
		case 1:
			Sdio0Init();
			Sdio0InitProcess();
			break;
		case 2:
			printf("CPU0 : SR : 0x%X\r\n", mfspr(EXR1K_SR));
			break;
		case 3:
			fres=drive_in(MMC);
			if(fres == FR_OK || fres == FR_NO_FILESYSTEM)
			{
				printf("SD card IN\r\n");
				drive_init(MMC);
			}
			else
			{
				printf("SD card IN(mount fail:err%d)\r\n", fres);
			}
			break;
		case 4:
			Sdio0SetClock();
			break;
		case 5:
			printf("sector num(%d) -> read -> memory(%X)\r\n", getvalue, nSectorTestR);
			Sdio0SectorR(nSectorTestR, getvalue);
			hexDump((char *)__func__, nSectorTestR, 512);
			un32Count = 0;
			for(i=0;i<512;i++)
			{
				if(nSectorTestR[i] != nSectorTestW[i])
					un32Count++;
//					printf("E:(%d) R(%d), W(%d)\r\n", i, nSectorTestR[i], nSectorTestW[i]);
			}
			if(un32Count)
				printf("Error count : %d\r\n", un32Count);
			break;
		case 6:
			printf("memory(%X) -> write -> sector num(%d)\r\n", nSectorTestW, getvalue);
			for(i=0;i<512;i++)
			{
				nSectorTestW[i] = i;
			}
			Sdio0SectorW(nSectorTestW, getvalue);
			hexDump((char *)__func__, nSectorTestW, 512);
			break;
		case 7:
			printf("memory(%X) -> write -> sector num(%d) input(%d)(0x%02X)\r\n", nSectorTestW, getvalue, getvalue2, getvalue2);
			for(i=0;i<512;i++)
			{
//				nSectorTestW[i] = getvalue2;
				nSectorTestW[i] = (rand() * rand());
			}
			Sdio0SectorW(nSectorTestW, getvalue);
			hexDump((char *)__func__, nSectorTestW, 512);
			break;
		case 8:
			for(j=0;j<256;j++,getvalue++)
			{
				nFlag = 0;
				printf("memory(%X) -> write -> sector num(%d) input(%d)(0x%02X)\r\n", nSectorTestW, getvalue, j, j);
				for(i=0;i<512;i++)
				{
					nSectorTestW[i] = j;
				}
				Sdio0SectorW(nSectorTestW, getvalue);
	//			hexDump((char*)__func__, nSectorTestW, 512);

				printf("sector num(%d) -> read -> memory(%X)\r\n", getvalue, nSectorTestR);
				Sdio0SectorR(nSectorTestR, getvalue);
	//			hexDump((char*)__func__, nSectorTestR, 512);
				for(i=0;i<512;i++)
				{
					if(nSectorTestR[i] != nSectorTestW[i])
					{
//						printf("E:(%d) R(%d), W(%d)\r\n", i, nSectorTestR[i], nSectorTestW[i]);
						nFlag = 1;
						break;
					}
				}

				if(nFlag)
				{
					printf("E:(%d)\r\n", j);
//					printf("write data\r\n");
//					hexDump(__func__, nSectorTestW, 512);
//					printf("read data\r\n");
//					hexDump(__func__, nSectorTestR, 512);
//					printf("\r\n");
				}
			}
			break;
		case 9:
			for(j=0;j<256;j++,getvalue++)
			{
				nFlag = 0;
				printf("memory(%X) -> write -> sector num(%d)\r\n", nSectorTestW, getvalue);
				for(i=0;i<512;i++)
				{
					nSectorTestW[i] = (rand() * rand())%256;
				}
				Sdio0SectorW(nSectorTestW, getvalue);
	//			hexDump(__func__, nSectorTestW, 512);
	
				printf("sector num(%d) -> read -> memory(%X)\r\n", getvalue, nSectorTestR);
				Sdio0SectorR(nSectorTestR, getvalue);
	//			hexDump(__func__, nSectorTestR, 512);
				for(i=0;i<512;i++)
				{
					if(nSectorTestR[i] != nSectorTestW[i])
					{
//						printf("E:(%d) R(%d), W(%d)\r\n", i, nSectorTestR[i], nSectorTestW[i]);
						nFlag = 1;
						break;
					}
				}

				if(nFlag)
				{
					printf("write data\r\n");
					hexDump((char *)__func__, nSectorTestW, 512);
					printf("read data\r\n");
					hexDump((char *)__func__, nSectorTestR, 512);
					printf("\r\n");
				}
			}
			break;
		case 10:
			printf("Test base address\r\n");
			printf("W : 0x%08X\r\n", nSectorTestW);
			printf("R : 0x%08X\r\n", nSectorTestR);
			break;
		case 11:
			printf("sector num(%d) -> read -> memory(%X)\r\n", getvalue, nSectorTestR);
			Sdio0SectorR(nSectorTestR, getvalue);
			hexDump((char *)__func__, nSectorTestR, 512);
			break;
		case 12:
			printf("sector erase(%d)\r\n", getvalue);
			Sdio0E(getvalue, getvalue);
			break;
		case 13:
			if(getvalue == 0)
			{
				un8Temp = rand() * rand();
				printf("get rand : %d(0x%02X)\r\n", un8Temp, un8Temp);
			}
			else
			{
				for(i=0;i<getvalue;i++)
				{
					un8Temp = (rand()+i) * (rand()-i);
					printf("get rand[%03d] : %d(0x%02X)\r\n", i, un8Temp, un8Temp);
				}
			}
			break;
		case 14:
			Sdio0SetBlockLength();
			break;
		case 15:
			Sdio0R(nSectorTestR, getvalue, 8);
			for(i=0;i<8;i++)
				hexDump((char*)__func__, nSectorTestR+(512*i), 512);
			break;
		case 16:
			for(i=0;i<4096;i++)
			{
				nSectorTestW[i] = rand();
			}
			for(i=0;i<8;i++)
				hexDump((char*)__func__, nSectorTestW+(512*i), 512);
			Sdio0W(nSectorTestW, getvalue, 8);
			break;
		case 17:
			for(i=0;i<4096;i++)
			{
				nSectorTestW[i] = i;
			}
			for(i=0;i<8;i++)
				hexDump((char*)__func__, nSectorTestW+(512*i), 512);
			Sdio0W(nSectorTestW, getvalue, 8);
			break;
		case 18:
			for(i=0;i<4096;i++)
				if(nSectorTestR[i] != nSectorTestW[i])
					printf("%04d : R(0x%02X) W(0x%02X)\r\n",i, nSectorTestR[i], nSectorTestW[i]);
			break;
		case 19:
			sys_thread_new("sdcheck", cmd_sdcheck_task_m_write_100, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			break;
		case 25:
			hexDump("READ",  nSectorTestR+(512*getvalue), 512);
			hexDump("WRITE", nSectorTestW+(512*getvalue), 512);
			break;
		case 26:
			if(sdcheck_switch == 0)
				sys_thread_new("sdcheck", cmd_sdcheck_task_s_write, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			else
				printf("sdcheck_switch value %d\r\n", sdcheck_switch);
			break;
		case 27:
			if(sdcheck_switch == 0)
				sys_thread_new("sdcheck", cmd_sdcheck_task_s_read, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			else
				printf("sdcheck_switch value %d\r\n", sdcheck_switch);
			break;
		case 28:
			if(sdcheck_switch == 0)
				sys_thread_new("sdcheck", cmd_sdcheck_task_m_write, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			else
				printf("sdcheck_switch value %d\r\n", sdcheck_switch);
			break;
		case 29:
			if(sdcheck_switch == 0)
				sys_thread_new("sdcheck", cmd_sdcheck_task_m_read, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			else
				printf("sdcheck_switch value %d\r\n", sdcheck_switch);
			break;
		case 30:
			if(sdcheck_switch == 1)
				sdcheck_switch = 2;
			else
				printf("sdcheck_switch value %d\r\n", sdcheck_switch);
			break;
		case 33:
			sys_thread_new("sdcheck", cmd_sdcheck_task_s_wr, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			break;
		case 34:
			sys_thread_new("sdcheck", cmd_sdcheck_task_m_wr, NULL, _16KB_STACK_SIZE, TIMER_TASK_PRIO);
			break;
		case 50:
			PAD_SDIO0_CLK_DS = getvalue;
			printf("PAD_SDIO0_CLK_DS : %d\r\n", PAD_SDIO0_CLK_DS);
			break;
		case 51:
			PAD_SDIO0_CMD_DS = getvalue;
			printf("PAD_SDIO0_CMD_DS : %d\r\n", PAD_SDIO0_CMD_DS);
			break;
		case 52:
			PAD_SDIO0_DAT_DS = getvalue;
			printf("PAD_SDIO0_DAT_DS : %d\r\n", PAD_SDIO0_DAT_DS);
			break;
		default:
			Shell_Unknown();
			break;
	}
	printf("%s(%d) : End\r\n", __func__, __LINE__);
#else
	printf("%s(%d) : NO SDCard Driver\r\n", __func__, __LINE__);
	UNUSED(argc);
	UNUSED(argv);
#endif

	return 0;
}

