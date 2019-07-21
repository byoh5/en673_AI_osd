//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		main.c
//	Description:
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	141217		ykjang		first designed
//	0.1	150424		ygkim		redesigned
// -----------------------------------------------------------------------------
#include "dev.h"			// to use printf, ...
#include "exr1k_boot.h"		// to use code memory information
#include "dev_inline.h"		// to use fast code

#include "msg.h"
#include "user.h"

//******************************************************************************
//
//------------------------------------------------------------------------------
void Boot_PLL_Init(void)
{
	WdtSysSetTime(2);// System WDT enable(temp)

	const UINT freq = (27 * 1000 * 1000 * SYS_PLL0_N / SYS_PLL0_K / SYS_PLL0_M);
	if (freq != BUS_CLK) {
#ifndef DEF_SECTION
		SYS_PLL0_M = 1;
		SYS_PLL0_N = CLK_VALUE;
		SYS_PLL0_K = 10;
		SYS_PLL0_N = CLK_VALUE;
		SYS_PLL0_AUTO_RS = 1;
		SYS_PLL0_ACTL_EN = 1;
		SYS_PLL0_M = 1;
		SYS_PLL0_ACTL_EN = 0;
		SYS_PLL0_AUTO_RS = 0;
#endif
	}

	WdtSysSetStop();
}

void Boot_Main_Clk_Init(void)
{
//	DDR Clock
	SYS_DCK_PD = 0;
	SYS_DCK_SEL = 0;

//	ISP (ISP Register를 조금이라도 빨리 써서 CPU 점유율을 낮추고 싶으면 초기에 SYS_MBCK_SEL을 '1'로 설정하면 된다.)
	SYS_MBCK_SEL = 1;
	SYS_FONT_WLTC = 5;		// FONT Write Latancy

//	DDR Clock
	SYS_DCK_PD = 1;
}

void hexDump(char *desc, void *addr, int len)
{
	int i, pos = 0;
	char strbuff[80] = {0};
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		jprintf ("%s:\r\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
			{
				strbuff[pos++] = ' ';
				strbuff[pos++] = ' ';
				strncpy(&strbuff[pos], (char *)buff, 16);
				jprintf ("%s\r\n", strbuff);
				pos = 0;
			}

			// Output the offset.
			sprintf(&strbuff[pos], "  %04x ", i);
			pos += 7;
		}

		// Now the hex code for the specific character.
		sprintf(&strbuff[pos], " %02x", pc[i]);
		pos += 3;

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		i++;
	}

	// And print the final ASCII bit.
	strbuff[pos++] = ' ';
	strbuff[pos++] = ' ';
	strncpy(&strbuff[pos], (char *)buff, 16);
	pos += 16;
	jprintf ("%s\r\n", strbuff);
}

int main(void)
{
#ifndef DEF_SECTION
	const char *cpu_str[] = {"CPU0", "CPU1"};
	UINT *pSfls = (UINT*)SFLS_BASE;
	int i;
#endif
	int id = CpuIdGet();

	mtspr(EXR1K_HTBCR, EXR1K_HTBCR_TE); // H/W Trace buffer enable for Time Function
	if (id == 0) {
		Boot_PLL_Init();
		Boot_Main_Clk_Init();
		Uart0_Init(eUARTspeed115200);

#ifdef JTAG_DEBUG_PRINT
		gptMsgShare._JTAG_PRINT_SIZE = JTAG_PRINT_SIZE;
		gptMsgShare._JTAG_PRINT0_START = JTAG_PRINT0_BASE;
		gptMsgShare._JTAG_PRINT0_POINT = JTAG_PRINT0_BASE;
#ifdef DEF_SECTION	// section write라면 cpu0에서 cpu1의 jtag print의 base까지 정해주고
		gptMsgShare._JTAG_PRINT1_START = JTAG_PRINT1_BASE;
		gptMsgShare._JTAG_PRINT1_POINT = JTAG_PRINT1_BASE;
#endif
#else
		gptMsgShare._JTAG_PRINT_SIZE = 0;
		gptMsgShare._JTAG_PRINT0_START = 0;
		gptMsgShare._JTAG_PRINT0_POINT = 0;
		gptMsgShare._JTAG_PRINT1_START = 0;
		gptMsgShare._JTAG_PRINT1_POINT = 0;
#endif

		if (!SYS_DDR_RST) {
			DdrInit();
		}
		DmaMemSet_ip((BYTE *)DDR_BASE, 0, DDR_SIZE);
		printf("init DDR\r\n");

		printf("Bootloader Ver %X.%X.%X\r\n", (EN673_VERSION>>8)&0xf, (EN673_VERSION>>4)&0xf, EN673_VERSION&0xf);
		printf("CPU_CLK %uKHz(def:%uKHz)\r\n", (27 * 1000 * 1000 * SYS_PLL0_N / SYS_PLL0_K / SYS_PLL0_M / 1000), BUS_CLK / 1000);
		printf("ISP_CLK %uKHz\r\n", (27 * 1000 * 1000 * SYS_PLL1_N / SYS_PLL1_K / SYS_PLL1_M / 1000) >> 1);
//	#ifndef DEF_SECTION
//		printf("SF_SYS_PLL1_N : %d\r\n", *(UINT*)(SFLS_BASE + *(UINT*)SFLS_BASE + 0x100 - 4));	// Section Write 시 Flash 사용 안됨!!!
//	#else
//		//printf("DR_SYS_PLL1_N : %d\r\n",*(UINT*)(SYSRAM_BASE + 0x100 - 4));					// 미리 DDR에 복사되어야 함!!!
//	#endif
//		printf("SYS_PLL1_N : %d\r\n", SYS_PLL1_N);

#ifdef DEF_SECTION
		SYS_CPU1_CK_EN = 1;
		SYS_CPU1_RST = 1;

#define BOOT_CHECK      0x040000F0
#define BOOT_CHECK_FLAG 0x06730673

		UINT* pBootCheck = (UINT*)BOOT_CHECK;
		*pBootCheck = BOOT_CHECK_FLAG;

		while(1){
			printf("section boot finish\r\n");
			WaitXms(1000);
		}
#else
#ifdef WDT_SYS
		WdtSysSetTime(WDT_DEFAULT_TIME);// System WDT enable
#endif

		SflsInit();

		PRINT_CODE_MEM_INFO("boot", 0);

		UserLoad(sfls_SYSTEM);
#endif

		if(gtSystem.nMagicNum != MAGICNUM_SYSTEM || (UINT)gtSystem._pBootAddr == 0x00000000 || (UINT)gtSystem._pBootAddr == 0xFFFFFFFF)
		{
			printf("init gtSystem\r\n");
			printf("_pBootAddr : 0x%08X\r\n", (UINT)gtSystem._pBootAddr);
			printf("nMagicNum : 0x%08X\r\n", gtSystem.nMagicNum);
			hexDump("gtSystem Dump", (void *)&gtSystem, 64);

			gtSystem.nMagicNum = MAGICNUM_SYSTEM;
			gtSystem._pBootAddr = (UINT *)BOOT_ADDRESS;
#ifndef DEF_SECTION
			UserSave(sfls_SYSTEM);
#endif
		}
	}
#ifndef DEF_SECTION
	else
	{
#ifdef JTAG_DEBUG_PRINT
		gptMsgShare._JTAG_PRINT1_START = JTAG_PRINT1_BASE;
		gptMsgShare._JTAG_PRINT1_POINT = JTAG_PRINT1_BASE;
#endif

		UserLoad(sfls_SYSTEM);
	}

	int targetArea = id + 1;
	if(gtSystem._pBootAddr != (UINT *)BOOT_ADDRESS)
	{
		pSfls = gtSystem._pBootAddr + (SFLS_BASE / 4);
		targetArea--;
	}

	// Find its FMA
	for(i=0; i<targetArea; i++) pSfls += (*pSfls>>2);

	printf("CPU ID(%d), Binary(0x%08X)\r\n", id, pSfls);

	// Get code memory information
	tCodeMemInfo *pCodeMemInfo = (tCodeMemInfo *)pSfls;

	// Check absolute/relative LMA
//	uint reset_lma_s = pCodeMemInfo->section[CODE_SECTION_LMA_IDX(RESET)].s;
	uint fma_base = (uint)gtSystem._pBootAddr - BOOT_ADDRESS;

	PRINT_CODE_MEM_INFO(cpu_str[id], fma_base);

//	printf("reset_lma_s(0x%08X), fma_base(0x%08X), pSfls(0x%08X), \r\n", reset_lma_s, fma_base, pSfls);

	// Code and data copy
	for(i=CODE_SECTION_IDX(RESET); i<=CODE_SECTION_IDX(DATA); i++)
	{
#ifdef WDT_SYS
		WdtSysCountReset();
#endif
		uint vma_s = pCodeMemInfo->section[i].s;
		uint vma_e = pCodeMemInfo->section[i].e;
		uint lma_s = pCodeMemInfo->section[VMA_TO_LMA(i)].s;
		uint len = vma_e - vma_s;

		if((lma_s>=SFLS_BASE || lma_s <SFLS_BASE+SFLS_SIZE) &&					// a section in flash
			(vma_s <SFLS_BASE || vma_s>=SFLS_BASE+SFLS_SIZE) &&					// its VMA is not in flash
			len>0)
		{
//			printf("0x%08X <= 0x%08X (size:%d)\r\n", vma_s, (fma_base+lma_s), len);
			//printf("%x -> %x\r\n", (BYTE *)(fma_base+lma_s), (BYTE *)vma_s);
			DmaMemCpy_ip((BYTE *)vma_s, (BYTE *)(fma_base+lma_s), len);			// Flash to SRAM/DDR by DMA

#ifdef _SIM_MODE
			int j;
			uint* pSrc = (uint*) (fma_base+lma_s);
			uint* pDst = (uint*) vma_s;
			for(j=0; j<(int)len/4; j++, pSrc++, pDst++)
			{
				uint src = *pSrc;
				uint dst = *pDst;
				if(*pSrc != *pDst)
				{
					FAIL("*E: %x @%x -> %x @%x \r\n",src,pSrc, dst, pDst);
					while(1);
				}
			}
#endif
		}
	}

//	printf("CPU%d ROM From 0x%08X to 0x%08X, Addr(%0x%08X) Size(0x%08X)\n", CPU_ID, );

	TRACE("BOOT INFO:\r\n");
	TRACE("└ID: %d, CPU name: %s, START @ %08x\r\n",id, cpu_str[id], pCodeMemInfo->reset.s + 0x100);

#if 1
	// Set code section in IHWP/DHWP
	set_code_hwp(pCodeMemInfo); // Auto Gen Watch point by H/W
	//print_hwp_all_info(EXR1K_SPRGID_IWB);
	//print_hwp_all_info(EXR1K_SPRGID_DWB);
#endif

	if (id == 0) {
		SYS_CPU1_CK_EN = 1;
		SYS_CPU1_RST = 1;
	}

//	Call _reset @ cpu
	call(pCodeMemInfo->reset.s + 0x100);
#endif

	return 0;
}
