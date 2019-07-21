/* **************************************************************************
 File Name	: 	app_shd.c
 Description	:	Shading Detection&Correction Function
 Designer		:	Lee, Wanhee
 Date		:	15.08.27
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#if 1 // 180319 LWH - New Structure
#include "isp_app_shd.h"

//=================================================================================================
//	Main function 
//=================================================================================================
void Shd_Init(void)	// 2015827 - WHL
{
	// Shading Register Initialization
	SHD_MODEw(1);	// 0 : PD, 1 : Off, 2 : Det mode, 3 : Cor Mode
	SHD_TESTw(0);	// 0 : Def, 1 : Block, 2 : Gain, 3 : Gain Block
	SHD_DETOSDw(1);	// It enable detection mode only!
	SHD_ECHKw(1);	// Shading Table Err Check at Detection Mode.

	// Shading Det Block Set for 2M Sensor
	SHD_DHWw(FR_HW-6); // Det H/V-Width
	SHD_DVWw(FR_VW-6);
	SHD_KHw((SHD_DET_HTW*0x40)/SHD_DHWr);	// Det Correction & Detection Ratio
	SHD_KVw((SHD_DET_VTW*0x40)/SHD_DVWr);
	
	SHD_DHOFFw(0xc);	// Detection Ofs
	SHD_DVOFFw(0x8);
	SHD_CHOFFw(0);	// Correction ofs for Optical Zoom (It's similiar with Gain)
	SHD_CVOFFw(0);

	SHD_GAINw(0x40); // 0x40 is x1

#if (!model_Shade) // Initial Shading Table Load from Flash memory
	while(CS_SFLS);
	Sf2Isp(FLASH_SECT_SHD_STT*FLASH_SECT_SIZE, ISP_BASE_SHADING, ISP_SHADING_EA, 2);
	CS_SFLS = 0;
			
#else // Initial Shading Table by fixed table
	UINT i;
 	for (i=0; i<ISP_SHADING_EA; i++) SetShading(i, gwTblShading[i]);
	
#endif
}

void Shd_CmdCheck(void)
{
	gbShd_Excute=1;
}

void ISPM Shd_PrePrecessing(void)
{
	static BYTE bShdDetState=0;
	static WORD nShdTblBuf[ISP_SHADING_EA];

	// Shading Correction Mode
 	if(MP(MpShdDet)==0) {
		bShdDetState=0;
				
		if (MP(MpShading)) SHD_MODEw(3);	// 2015827 - WHL
		else if(!MP(MpShdDet)) SHD_MODEw(1);

		gnShdWgt = LibUtlInterp1D (MINMAX(giCurAgc, ADNR_AGC_MID, ADNR_AGC_MAX), ADNR_AGC_MID, ADNR_AGC_MAX, MP(MpShdWeight), 1 );
		gnShdWgt = (gnShdWgt<<6)/100;	// 2015827 - WHL : for 100% Gain
	}
	// Shading Detection Mode
	else {
		switch(bShdDetState) {
			case 1 : {	// Det Mode Set
				SHD_MODEw(2);	// Detection Mode
				SHD_TESTw(1);	// SHD Block View
				bShdDetState++;
				break;
			}
			case (SHD_DET_FRAME-2) : {
				SHD_MODEw(1);	// Normal Mode (R/W)
				SHD_TESTw(0);	// Def
				bShdDetState++;
				break;
			}
			case (SHD_DET_FRAME-1) : {
				UINT i,j;
					
				printf("\r\nconst WORD gwTblShading[] = {");
				
				for(i=0; i<ISP_SHADING_EA; i++) {
					for(j=0; j<2; j++) nShdTblBuf[i] = (WORD)GetIsp(0x400+i);	// 201593 - WHL : Register Address Delay

					nShdTblBuf[i] = ((WORD)((nShdTblBuf[i]<<8)&0xff00)|(WORD)((nShdTblBuf[i]>>8)&0x00ff));
					
					if(i%20==0) printf("\r\n0x%04x,",nShdTblBuf[i]);
					else {
						if(i==(ISP_SHADING_EA-1)) printf("0x%04x};",nShdTblBuf[i]);
						else                      printf("0x%04x," ,nShdTblBuf[i]);
					}
				}			
				bShdDetState++;
				break;
			}
			case (SHD_DET_FRAME) : {	// Det Complete
			
				MP(MpShdDet)=0;
				
#if (!model_Shade)
			#if model_Save

				gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_SHD;
				gptMsgISPTbl.ISPTblAddr =(UINT)&nShdTblBuf;

				CPU_IRQ1 = 1;

				printf("\r\nShd Table Write Done (%d)",(UINT)(ISP_SHADING_EA<<1));
			#endif
#endif
				break;
			}
			default : { bShdDetState++; break; }	// Wait for Detection
		}
	}
}

void Shd_PostPrecessing(void)
{
	SHD_GAINw(gnShdWgt);
}

void Shd_DbgDisp(void)
{
	DebugDisp(gbMnDebugFnc==5, Hex, "SHAD_GAIN", 4, 24, gnShdWgt | (MP(MpShading)<<31))
}

//-------------------------------------------------------------------------------------------------

void ISPM Shading(void)
{
	static BYTE bInitFlag=1;
	
	if(bInitFlag) { bInitFlag=0; Shd_Init(); }

	Shd_CmdCheck();

	if(gbShd_Excute) {

		Shd_PrePrecessing();

		Shd_PostPrecessing();
	}
	Shd_DbgDisp();
}

#else // Old

#include "dev_isp.h" 
#include "isp_app_shd_tbl.h"

//=================================================================================================
//	define
//=================================================================================================
#define	SHD_DEBUG 		0		// 1 : Debuging Info display to uart
#define SHD_DET_FRAME	30		// Minimum Value is 1 (1Frame)
#define SHD_DET_HTW		3840	// Shading Detection Window H-Total Width
#define SHD_DET_VTW		2048	// Shading Detection Window V-Total Width

//=================================================================================================
//	Main function 
//=================================================================================================
void InitShd(void)	// 2015827 - WHL
{
	// Shading Register Initialization
	SHD_MODEw(1);	// 0 : PD, 1 : Off, 2 : Det mode, 3 : Cor Mode

	SHD_TESTw(0);	// 0 : Def, 1 : Block, 2 : Gain, 3 : Gain Block
	SHD_DETOSDw(1);	// It enable detection mode only!
	SHD_ECHKw(1);	// Shading Table Err Check at Detection Mode.

	// Shading Det Block Set for 2M Sensor
//	SHD_DHWw(FR_HW); // Det H/V-Width
//	SHD_DVWw(FR_VW);
	SHD_DHWw(FR_HW-6); // Det H/V-Width
	SHD_DVWw(FR_VW-6);
//		SHD_KHw(0x80);	// Det Correction & Detection Ratio
//		SHD_KVw(0x7C);
	SHD_KHw((SHD_DET_HTW*0x40)/SHD_DHWr);	// Det Correction & Detection Ratio
	SHD_KVw((SHD_DET_VTW*0x40)/SHD_DVWr);
	
//	SHD_DHOFFw(0);	// Detection Ofs
//	SHD_DVOFFw(0);
	SHD_DHOFFw(0xc);	// Detection Ofs
	SHD_DVOFFw(0x8);
	SHD_CHOFFw(0);	// Correction ofs for Optical Zoom (It's similiar with Gain)
	SHD_CVOFFw(0);

	SHD_GAINw(0x40); // 0x40 is x1

//	Initial Shading Table Load
#if (!model_Shade)
//	#if model_Save
		while(CS_SFLS);
		Sf2Isp( FLASH_SECT_SHD_STT*FLASH_SECT_SIZE, ISP_BASE_SHADING, ISP_SHADING_EA, 2);	// from FLASH
		CS_SFLS = 0;
//	#else
//		TwiEep2Isp(TWI_EEP_SHADE_STT, ISP_BASE_SHADING, ISP_SHADING_EA, 2);	// from EEPROM
//	#endif
			
#else																		// from fixed table
	UINT i;
 	for (i=0; i<ISP_SHADING_EA; i++) SetShading(i, gwTblShading[i]);
	
#endif
}

void ShadingDet(void)
{
	static BYTE bShdDetState=0;
	static WORD nShdTblBuf[ISP_SHADING_EA];
	
 	if(MP(MpShdDet)==0) { bShdDetState=0; return; }
	else if(bShdDetState==0) bShdDetState=1;

	switch(bShdDetState) {
		case 1 : {	// Det Mode Set
			SHD_MODEw(2);	// Detection Mode
			SHD_TESTw(1);	// SHD Block View
			bShdDetState++;
			break;
		}
		case (SHD_DET_FRAME-2) : {
			SHD_MODEw(1);	// Normal Mode (R/W)
			SHD_TESTw(0);	// Def
			bShdDetState++;
			break;
		}
		case (SHD_DET_FRAME-1) : {
			UINT i,j;
			
			for(i=0; i<ISP_SHADING_EA; i++) {
				for(j=0; j<2; j++) nShdTblBuf[i] = (WORD)GetIsp(0x400+i);	// 201593 - WHL : Register Address Delay

				nShdTblBuf[i] = ((WORD)((nShdTblBuf[i]<<8)&0xff00)|(WORD)((nShdTblBuf[i]>>8)&0x00ff));

				if(SHD_DEBUG) {	// 2015826 - WHL : Debuging Info for Shading Table Check
					TxStrNoIRQ("nShdTblAdr  : ", (0x400+i), 10);
					TxStrNoIRQ("nShdTblBuf : ", nShdTblBuf[i], 10);
				}
			}			
			bShdDetState++;
			break;
		}
		case (SHD_DET_FRAME) : {	// Det Complete
		
			MP(MpShdDet)=0;
			
#if (!model_Shade)
		#if model_Save

			#if 0 // Old
				SfWrite(FLASH_SECT_SHD_STT*FLASH_SECT_SIZE, (BYTE*)nShdTblBuf, ISP_SHADING_EA<<1);	// 12.12.28
			#else // New
				gptMsgISPTbl.ISPTblState=eIP1_ISP_SAVE_SHD;
				gptMsgISPTbl.ISPTblAddr =(UINT)&nShdTblBuf;

				CPU_IRQ1 = 1;
			#endif
//			#else 
//				TwiWrEep2(TWI_EEP_SHADE_STT, (BYTE*)nShdTblBuf, ISP_SHADING_EA<<1);				// 12.12.28

			TxStrNoIRQ("Shd Table Write Done : ", (UINT)(ISP_SHADING_EA<<1), 10);	// 2015826 - WHL
		#endif
#endif
			break;
		}
		default : { bShdDetState++; break; }	// Wait for Detection
	}
}

//-------------------------------------------------------------------------------------------------

void ISPM Shading (void)
{	// Shading correction
	
	ShadingDet();						// Shading table detection mode (Hidden menu)
				
	if (MP(MpShading)) SHD_MODEw(3);	// 2015827 - WHL
	else if(!MP(MpShdDet)) SHD_MODEw(1);

	UINT nWgt = MP(MpShdWeight);
	nWgt = LibUtlInterp1D (MINMAX(giCurAgc, ADNR_AGC_MID, ADNR_AGC_MAX), ADNR_AGC_MID, ADNR_AGC_MAX, nWgt, 1 );
//	nWgt = (nWgt<<7)/100;
	nWgt = (nWgt<<6)/100;	// 2015827 - WHL : for 100% Gain
	
	SHD_GAINw(nWgt);
	DebugDisp(gbMnDebugFnc==5, Hex, "SHAD_GAIN", 4, 24, nWgt | (MP(MpShading)<<31))
}
#endif

/* EOF */

