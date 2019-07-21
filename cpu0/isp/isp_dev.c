/* **************************************************************************
 File Name	: 	dev.c
 Description:	EN673 - common device driver & init table
 Designer	:	Lee, Hoon / Kim, Sunghoon / Lee, Wanhee
 Date		:	14. 7. 14 /       -       / 16. 3. 6
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"
#include "isp_dev_sub_tbl.h"

	
const UINT gnTblIspModeDly[][IspModeSize] = {
		//Addr   1080 25P       1080 30P       1080 50P       1080 60P       C720 25P       C720 30P       C720 50P       C720 60P
		{0x0028, ISP_REG_028_0, ISP_REG_028_0, ISP_REG_028_2, ISP_REG_028_2, ISP_REG_028_0, ISP_REG_028_0, ISP_REG_028_0, ISP_REG_028_0},	// For Sensor Flip Off
		{0x0028, ISP_REG_028_1, ISP_REG_028_1, ISP_REG_028_3, ISP_REG_028_3, ISP_REG_028_1, ISP_REG_028_1, ISP_REG_028_1, ISP_REG_028_1}	// For Sensor Filp On
		};

const UINT gTblIspModeDlyEa = numberof(gnTblIspModeDly);


#if (model_SetIsp == 1)

#define ISP_CK_ADR0		0x0005	// PCLKI1_PD[31],FPCK_PD[30],PPCK_PD[29],PSCK_PD[28],PR_CK_PD[27],FN_CK_PD[26]
#define ISP_CK_ADR1     0x0006	// YCW_CK0_PD[24],YCW_CK1_PD[23],YCW_CK2_PD[22],IMD_CK_PD[21],JPGCK_PD[20],CDS0_PCK_PD[19],CDS1_PCK_PD[18],PIP_PPCK_PD[17],IBT_PCK_PD[16],IBT0_PCK_PD[15],BT_PCK_PD[14],BT0_PCK_PD[13],YCR_CK0_PD[12],PCKO0_ON[8],PCKO1_ON[7],PCKO_OEN[5],BT1120_PCK_PD[4],BT1120_PCK0_PD[3]

#define ISP_CK_MSK0		0x03FFFFFF
#define ISP_CK_MSK1     0xF7000E47

void ISPM SetIsp(UINT anAddr, UINT anData)
{
	UINT nAcept_Mask = 0;

	switch(anAddr) {
		
		case ISP_CK_ADR0 : nAcept_Mask = ISP_CK_MSK0;
		case ISP_CK_ADR1 : nAcept_Mask = ISP_CK_MSK1;
		default          : nAcept_Mask = 0;
	}

	if(nAcept_Mask) _wr32(ISP_REG_BASE+((UINT)(anAddr)<<2), (UINT)((anData&~nAcept_Mask)|((_rd32(ISP_REG_BASE+((UINT)(anAddr)<<2))&nAcept_Mask))));
	else            _wr32(ISP_REG_BASE+((UINT)(anAddr)<<2), (UINT)(anData));
}

#endif

#if model_Pana

void SetSensFnc(WORD dummy, WORD ADDR, WORD VAL)
{
	UNUSED_ARG(dummy);

//		if(gbSensCtrMode) SetSensTwi_Pana(ADDR, VAL);
//		else			  SetSensSpi_Pana(ADDR, VAL);
	SetSensSpi_Pana(ADDR, VAL);
}

WORD GetSens(WORD ADDR)
{
//		if(gbSensCtrMode) return GetSensTwi_Pana(ADDR);
//		else			  return GetSensSpi_Pana(ADDR);
	return GetSensSpi_Pana(ADDR);
}

#elif model_Sony

void SetSensBurstFnc(BYTE ID, BYTE ADDR, BYTE* VAL, BYTE Length) // Prevent for optimization
{
	//if(PAR00) return;
	if(gbSensCtrMode) SetSensTwiBurst_Sony(ID, ADDR, VAL, Length);
	else			  SetSensSpiBurst_Sony(ID, ADDR, VAL, Length);
}

void SetSensFnc(WORD ID, WORD ADDR, WORD VAL) // Prevent for optimization
{
	//if(PAR00) return;
	if(gbSensCtrMode) SetSensTwi_Sony(ID, ADDR, VAL);
	else			  SetSensSpi_Sony(ID, ADDR, VAL);
}

BYTE GetSens(BYTE ID, BYTE ADDR)
{
	if(gbSensCtrMode) return GetSensTwi_Sony(ID, ADDR);
	else			  return GetSensSpi_Sony(ID, ADDR);
}

#elif model_Aptn

void SetSensFnc(BYTE DEV, WORD ADDR, WORD VAL)
{
	UNUSED_ARG(DEV);
	
	//if(PAR00) return;
	SetSensTwi_Aptn(SENS_AR0230_DEV, ADDR, VAL);
}

WORD GetSens(WORD ADDR)
{
	return GetSensTwi_Aptn(SENS_AR0230_DEV, ADDR);
}

#elif model_Omni

void SetSensFnc(BYTE DEV, WORD ADDR, BYTE VAL)
{
	UNUSED_ARG(DEV);
	
	SetSensTwi_Omni(SENS_OV2718_DEV, ADDR, VAL);
}

BYTE GetSens(WORD ADDR)
{
	return GetSensTwi_Omni(SENS_OV2718_DEV, ADDR);
}

//	#else

#endif

//----------------------------------------------------------------------------------------
UINT gnMCLK = 0;

UINT ISPM0 GetWaitUsTick(const UINT anUs)
{
	// When an interrupt occurs, then the time increases.
	TICK_STA(1);
	WaitUs(anUs);
	TICK_END(1);
	return nTick_1;
}

UINT CorrWaitTime(const UINT anUs)
{
	UINT nTick_1 = GetWaitUsTick(anUs);

	const UINT nNs = anUs * 1000;
	const UINT nTick_ns = (nTick_1*4000) / (gnMCLK/250000);

	if(nTick_ns < nNs && gnWaitTimeOffset) {
		gnWaitTimeOffset--;
		return 1;
	}
	else {
		return 0;
	}
}

void SetWaitTimeOffset(const UINT anMCLK, const UINT anCntPerSec)
{
	gnMCLK = anMCLK;

	mtspr(EXR1K_TTMR,	EXR1K_TTMR_M_RESTART /*| EXR1K_TTMR_IE*/);		// Restart mode, Interrupt enable
    mtspr(EXR1K_TTMR,	mfspr(EXR1K_TTMR) | (EXR1K_TTMR_PERIOD&((gnMCLK/anCntPerSec)-1)) );	// Set period
    //mtspr(EXR1K_SR  , mfspr(EXR1K_SR) | EXR1K_SR_TEE );

	#define WAIT_TIME_TEST_US	10	// CAUTION!!! - If LOW MCLK & NO ISPM & NO I-CACHE, The value must be larger.

	UINT nTick_1 = GetWaitUsTick(WAIT_TIME_TEST_US);

	const UINT nTargerClk = ((gnMCLK/10000) * WAIT_TIME_TEST_US) / 100;
	gnWaitTimeOffset = nTick_1 - nTargerClk;

	//#define OffsetCorr 14	// Used TestWaitTime()
	//if(gnWaitTimeOffset >= OffsetCorr) gnWaitTimeOffset -= OffsetCorr;
	while( CorrWaitTime(WAIT_TIME_TEST_US) );
}

void InitTickTimer(const UINT anCntPerSec)
{
#if 1
	// Restart mode, Interrupt enable, Set period
	mtspr(EXR1K_TTMR,	EXR1K_TTMR_M_RESTART | EXR1K_TTMR_IE | (EXR1K_TTMR_PERIOD&((gnMCLK/anCntPerSec)-1)) );	// Set period
	mtspr(EXR1K_SR  , mfspr(EXR1K_SR) | EXR1K_SR_TEE );
#else
	mtspr(EXR1K_TTMR,	EXR1K_TTMR_M_RESTART | EXR1K_TTMR_IE);		// Restart mode, Interrupt enable
    mtspr(EXR1K_TTMR,	mfspr(EXR1K_TTMR) | (EXR1K_TTMR_PERIOD&((MCLK/anCntPerSec)-1)) );	// Set period
    mtspr(EXR1K_SR  , mfspr(EXR1K_SR) | EXR1K_SR_TEE );
	//TxStrNoIRQ("444", 0, 0);
#endif
}

//----------------------------------------------------------------------------------------
#define SPR_SR_BUF	10
UINT gnSPR_SR[SPR_SR_BUF];
UINT gnSPR_SRi = 0;

UINT gnActCnt = 0;
UINT gnActCntMax = 0;


UINT ISPM0 GetSt(UINT *apNum)
{
	gnActCnt++;
	if(apNum) *apNum = gnActCnt;

	gnSPR_SRi++;
	if(gnSPR_SRi == SPR_SR_BUF) gnSPR_SRi = 0;

	gnSPR_SR[gnSPR_SRi] = mfspr(EXR1K_SR);

	return gnSPR_SRi;
}

void ISPM0 SetSt(const UINT anSPR_SRi, UINT *apNum)
{
	if(apNum) {
		if(*apNum > gnActCnt)	*apNum = (0xFFFFFFFF - (*apNum)) + gnActCnt + 1;	// A->B & A>B = (0x100+B-A) = (0xFF-A)+B+1
		else					*apNum = gnActCnt - (*apNum);						// A->B & A<B = B - A

		if(gnActCntMax < *apNum) gnActCntMax = *apNum;
	}

	mtspr(EXR1K_SR, gnSPR_SR[anSPR_SRi]);
}

void ISPM0 SetStDisIrq(UINT anSPR_SRi)
{
	mtspr(EXR1K_SR, gnSPR_SR[anSPR_SRi] & ~(EXR1K_SR_IEE | EXR1K_SR_TEE));
}

void ISPM0 SetStEnaIrq(UINT anSPR_SRi)
{
	mtspr(EXR1K_SR, gnSPR_SR[anSPR_SRi] | EXR1K_SR_IEE);
}

void ISPM0 ClrIrqTT(void)
{
	mtspr(EXR1K_TTMR,	mfspr(EXR1K_TTMR) & (~EXR1K_TTMR_IP)); 		// Interrupt clear
}

_NOINS UINT ISPM0 GetTT(void)
{
	return mfspr(EXR1K_HTBTR/*EXR1K_TTCR*/);
}

UINT ISPM0 GetTTmax(void)
{
	return 0xFFFFFFFF;//mfspr(EXR1K_TTMR)&0x0fffffff;
}

/* EOF */

