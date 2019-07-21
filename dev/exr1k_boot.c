//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_boot.c
//	Description:	EXR1K boot loader functions
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150724		ygkim		first designed
// -----------------------------------------------------------------------------
#include "exr1k.h"			// to use exr1k
#include "exr1k_boot.h"		// to use code memory information
#include "dev_inline.h"	// to use fast code

//******************************************************************************
//
//------------------------------------------------------------------------------
_NOINS void _init_reset_section(void)
{
	DmaMemCpy_isr((void*)SYSRAM_BASE, (void*)SFLS_BASE, 0xF00);
}

_NOINS void _init_data_section(void)
{
	memcpy((void*)_data_s, (void*)_data_lma_s, (size_t)(_data_e - _data_s));
}

_NOINS void _init_text_section(void)
{
	memcpy((void*)_ispm_text_s, (void*)_ispm_text_lma_s, (size_t)(_ispm_text_e - _ispm_text_s));
//	memcpy((void*)_text_s, (void*)_text_lma_s, (size_t)(_text_e - _text_s));
}

_NOINS void _init_bss_section(void)
{
	BYTE *pDst = (BYTE*)(_bss_s);
	int n = _bss_e - _bss_s;

#ifdef _STB_ENABLE
	int i;
	for(i=0; i<n; i++) *pDst++ = 0;
#else
	DmaMemSet_isr(pDst, 0, (uint)n);
#endif
}


//******************************************************************************
//
//------------------------------------------------------------------------------
void set_code_hwp(tCodeMemInfo *pCodeMemInfo)
{
	tCodeSectionInfo sInfo[3];
	int nSections=0;
	int i, j;

	// get section info
	for(i=CODE_SECTION_IDX(RESET); i<=CODE_SECTION_IDX(ISPM_TEXT); i++) {
		uint vma_s = pCodeMemInfo->section[i].s;
		uint vma_e = pCodeMemInfo->section[i].e;
		uint len = vma_e - vma_s;

		if(len>0) {
			sInfo[nSections].s = vma_s; sInfo[nSections].e = vma_e;
			//TRACE("%d: %08x %08x \r\n",nSections, sInfo[nSections].s, sInfo[nSections].e);
			nSections++;
		}

	}

	// sort sections in increasing order
	//TRACE("sorted: \r\n");
	uint ad;
	for(i=0; i<nSections; i++) {
		for(j=i+1; j<nSections; j++) {
			if(sInfo[j].s<sInfo[i].s) {
				ad = sInfo[i].s; sInfo[i].s = sInfo[j].s; sInfo[j].s = ad;
				ad = sInfo[i].e; sInfo[i].e = sInfo[j].e; sInfo[j].e = ad;
			}
		}
		//TRACE("%d: %08x %08x \r\n",i, sInfo[i].s, sInfo[i].e);
	}

	// set insn hwp: watch PC outside code sections to prevent wrong PC
	mtspr(EXR1K_IWMR, 0);
	j = 0;

	// IHWP[0] = ( ld. adr < sInfo[0].s )
	set_hwp(EXR1K_SPRGID_IWB, j++, EXR1K_WCR_CT_LD_ADR, EXR1K_WCR_CC_LT, sInfo[0].s, EXR1K_WMR_CW_MATCH);
	for(i=1; i<nSections; i++) {
		// IHWP[j] & DHWP[j+1] = ( ld. adr >= sInfo[i-1].e ) & ( ld. adr < sInfo[i].s )
		set_hwp(EXR1K_SPRGID_IWB, j++, EXR1K_WCR_CT_LD_ADR, EXR1K_WCR_CC_GE, sInfo[i-1].e, EXR1K_WMR_CW_MATCH);
		set_hwp(EXR1K_SPRGID_IWB, j++, EXR1K_WCR_CT_LD_ADR, EXR1K_WCR_CC_LT, sInfo[i].s, EXR1K_WMR_CW_AND_PCHAIN);
	}
	// IHWP[end] = ( ld. adr >= sInfo[nSections-1].e )
	set_hwp(EXR1K_SPRGID_IWB, j++, EXR1K_WCR_CT_LD_ADR, EXR1K_WCR_CC_GE, sInfo[i-1].e, EXR1K_WMR_CW_MATCH);

	// set data hwp: watch store adress inside code sections to prevent code change
	mtspr(EXR1K_DWMR, 0);
	j = 0;
	for(i=0; i<nSections; i++) {
		// DHWP[j] & DHWP[j+1] = ( st. adr >= sInfo[i].s ) & ( st. adr < sInfo[i].e )
		set_hwp(EXR1K_SPRGID_DWB, j++, EXR1K_WCR_CT_ST_ADR, EXR1K_WCR_CC_GE, sInfo[i].s, EXR1K_WMR_CW_MATCH);
		set_hwp(EXR1K_SPRGID_DWB, j++, EXR1K_WCR_CT_ST_ADR, EXR1K_WCR_CC_LT, sInfo[i].e, EXR1K_WMR_CW_AND_PCHAIN);
	}
}
