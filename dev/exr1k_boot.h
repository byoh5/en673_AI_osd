//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_boot.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150724		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef	_EXR1K_BOOT_H_		
#define	_EXR1K_BOOT_H_


//******************************************************************************
// Code Memory Infomation
//------------------------------------------------------------------------------
typedef enum { 	CS_LMA=0,
				CS_RESET_LMA=1,	CS_TEXT_LMA, 	CS_ISPM_TEXT_LMA, 	CS_ISPM_DATA_LMA, 	CS_RODATA_LMA,	CS_DATA_LMA,
				CS_RESET=7, 	CS_TEXT, 		CS_ISPM_TEXT, 		CS_ISPM_DATA, 		CS_RODATA, 		CS_DATA,  		// : Code Sections
				CS_BSS,			CS_STACK
} eCodeSectionIndex;

#define	CODE_SECTION_IDX(NAME) 		CS_ ## NAME
#define	CODE_SECTION_LMA_IDX(NAME)	CS_ ## NAME ##_LMA
#define	CS_NSECTIONS				(CS_RESET-CS_RESET_LMA)
#define	VMA_TO_LMA(VMA_ID)			((VMA_ID)-CS_NSECTIONS)

typedef union {
	uint info[2];
	struct {
		uint s, e;
	};
} tCodeSectionInfo;

typedef union {
	tCodeSectionInfo section[1+2*CS_NSECTIONS];
	struct {
		tCodeSectionInfo	lma;
		tCodeSectionInfo 	reset_lma, 	text_lma, 	ispm_text_lma, 	ispm_data_lma, 	rodata_lma,	data_lma;
		tCodeSectionInfo	reset,		text,		ispm_text,		ispm_data,		rodata, 	data;
		tCodeSectionInfo	bss, 		stack;
	};
} tCodeMemInfo;

#define PRINT_CODE_STR(CODE)			TRACE("< %8s > %8s %8s %8s",CODE,"VMA-S","VMA-E","LENGTH");
#define PRINT_CODE_INFO(NAME)			TRACE("��%10s: %08x %08x %08x",#NAME, pCodeMemInfo->NAME.s, pCodeMemInfo->NAME.e,pCodeMemInfo->NAME.e-pCodeMemInfo->NAME.s);
#define PRINT_STACK_INFO(NAME)			TRACE("��%10s: %08x %08x %08x",#NAME, pCodeMemInfo->NAME.s, pCodeMemInfo->NAME.e,pCodeMemInfo->NAME.s-pCodeMemInfo->NAME.e+4);

#define PRINT_CODE_LMA_STR(CODE)		PRINT_CODE_STR(CODE); 	TRACE(" : %8s %8s","LMA-S","LMA-E");
#define PRINT_CODE_LMA_INFO(NAME)		PRINT_CODE_INFO(NAME); 	TRACE(" : %08x %08x", pCodeMemInfo->NAME ## _lma.s, pCodeMemInfo->NAME ## _lma.e);

#define PRINT_CODE_LFMA_STR(CODE)		PRINT_CODE_LMA_STR(CODE); 	TRACE(" : %8s %8s \r\n","FMA-S","FMA-E");
#define PRINT_CODE_LFMA_INFO(NAME,BASE)	PRINT_CODE_LMA_INFO(NAME); 	TRACE(" : %08x %08x \r\n",  ((uint)BASE) + pCodeMemInfo->NAME ## _lma.s, ((uint)BASE) + pCodeMemInfo->NAME ## _lma.e);

#if	!defined(_SIM_MODE) && defined(_DEBUG)
#define	PRINT_CODE_MEM_INFO(CODE,BASE)	PRINT_CODE_LFMA_STR(CODE);					\
										PRINT_CODE_INFO(lma); TRACE("\r\n");   		\
										PRINT_CODE_LFMA_INFO(reset,BASE);      		\
										PRINT_CODE_LFMA_INFO(text,BASE);       		\
										PRINT_CODE_LFMA_INFO(rodata,BASE);      	\
										PRINT_CODE_LFMA_INFO(ispm_text,BASE);  		\
										PRINT_CODE_LFMA_INFO(ispm_data,BASE);  		\
										PRINT_CODE_LFMA_INFO(data,BASE);       		\
										PRINT_CODE_INFO(bss); TRACE("\r\n");   		\
										PRINT_STACK_INFO(stack); TRACE("\r\n\r\n");	

#else
#define	PRINT_CODE_MEM_INFO(CODE,BASE)
#endif



#define STRINGIFY(X) 	#X
#define	XSTRINGIFY(X) 	STRINGIFY(X)


//******************************************************************************
//
//------------------------------------------------------------------------------
void _init_reset_section(void);
void _init_data_section(void);
void _init_text_section(void);
void _init_bss_section(void);

void set_code_hwp(tCodeMemInfo *pCodeMemInfo);


#endif//_EXR1K_BOOT_H_