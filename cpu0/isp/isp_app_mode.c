/* **************************************************************************
 File Name	: 	app_mode.c
 Description:	EN673 - Out mode function
 Designer	:	Lee, Hoon
 Date		:	14. 3. 24
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev_isp.h"
#include "isp_app_mode.h"

	
void ISPM Cstm_OutMode(UINT nModeIdx)
{
	UINT i;

	for (i=0; i<gTblIspModeSensorEa; i++) SetIsp(gnTblIspModeSensor[i][0], gnTblIspModeSensor[i][nModeIdx]);	// Digital
}

void ISPM OutMode(void)
{
	OutModeRun();

	if(gbModeChgOn) {
		// Customize for Mode change

		//DCKO0_INVw(MN_FPS_LOW);	// It depends on POF
		//ChangeMenuSize();
	}

	// RGB Odering Set by User Parameter
#if(model_Sens == SENS_OV2718)
	if(MP(MpMirror)==MN_ON) { ICSELw(UP(UpICSEL_FLIP)); OCSELw(UP(UpOCSEL_FLIP)); }
	else                    { ICSELw(UP(UpICSEL));      OCSELw(UP(UpOCSEL));      }
#else	
	if(MP(MpFlip)==MN_ON) {	ICSELw(UP(UpICSEL_FLIP)); OCSELw(UP(UpOCSEL_FLIP)); }
	else                  { ICSELw(UP(UpICSEL));      OCSELw(UP(UpOCSEL));      }
#endif
	
	// Sync Up Condition
	#if(model_2M)
		gbSyncupCnt = ((gbModeChgOn)|(gbWdrOn!=gbWdrOnBuf)) ? 6 : gbSyncupCnt;
	
	#else
		gbSyncupCnt = (gbModeChgOn) ? 6 : gbSyncupCnt;
	
	#endif

	if(gbSyncupCnt) { gbSyncupCnt--; SYNC_UPw((gbSyncupCnt!=0)); }// 20170116 - WHL
}

#if (LVDS_MARGIN_TEST==1)
void ISPM Lvds_Adjust(void)
{
	static UINT gLvdsFrmCnt=0;
	static BYTE bLvdsClkOrder, bLvdsSelOrder, bLvdsLDIDLY;
	
	static UINT gnLAdjCnt=0;
	static UINT gbLDIDLY[8][4][8]; // LCK, SEL, LDIDLY
	static UINT nLvdsRes[8][4][8];
	static UINT startflag=0, endflag=0;
	static BYTE gbOriginClk=0, gbOriginLDI[4];

	if(startflag==0) {
		gbOriginClk=LCK_DLYr;
		gbOriginLDI[0] = LDIDLY0r;
		gbOriginLDI[1] = LDIDLY1r;
		gbOriginLDI[2] = LDIDLY2r;
		gbOriginLDI[3] = LDIDLY3r;
		startflag=1;

		AF_WIN_SELw(0x01);
		AF_SLICEw(0);
		AF_EDGE_TH1w(0x100);
		AF_EDGE_TH2w(0x50);
		AF_CLIPw(0x3C0);
		AF_CLIP_ONw(0);

		AF_YSELw(2);
		AF_PDw(0);
		AF_CLIP_ONw(0);
	}

	bLvdsClkOrder= gnLAdjCnt/32 ;			// 몇번째 clk인지 : bLvdsClkOrder(0~7)
	bLvdsSelOrder = (gnLAdjCnt-(bLvdsClkOrder*32)) / 8;
	bLvdsLDIDLY   = (gnLAdjCnt-(bLvdsClkOrder*32)) % 8;



//	DispHex(10,14,AF_SUM1_LOCKr,8); // AF SUM value
//	DispDec(5,14,bLvdsClkOrder,8);	 // LCK 선택
//	DispDec(6,14,bLvdsSelOrder,8); // LDI 0~3 SEL
//	DispDec(7,14,bLvdsLDIDLY,8); // LDI0~3 DLY
//	DispDec(8,14,gLvdsFrmCnt,8); // Frame Cnt



	if(gLvdsFrmCnt==1) {
		LCK_DLYw(bLvdsClkOrder);																						// LCK ( 0~7 )
		LDO3_SELw(bLvdsSelOrder); LDO2_SELw(bLvdsSelOrder); LDO1_SELw(bLvdsSelOrder); LDO0_SELw(bLvdsSelOrder); 		// SEL ( 0~3 )
		LDIDLY0w (bLvdsLDIDLY); LDIDLY1w (bLvdsLDIDLY); LDIDLY2w (bLvdsLDIDLY); LDIDLY3w(bLvdsLDIDLY);					// DLY ( 0~7 )
		
	}
	if(gLvdsFrmCnt==3) {
		gLvdsFrmCnt=0; 
		gbLDIDLY[bLvdsClkOrder][bLvdsSelOrder][bLvdsLDIDLY] = AF_SUM1_LOCKr; // gbLDIDLY[LCK:8][SEL:4][DLY:8];
//		printf("[%u][%u][%u] %u\r\n",bLvdsClkOrder,bLvdsSelOrder,bLvdsLDIDLY,AF_SUM1_LOCKr);

		if(bLvdsClkOrder==7 && bLvdsSelOrder==3 && bLvdsLDIDLY==7) // END
		{

			UINT i, j, k, l=0;
			QWORD tmp=0;
			gnLAdjCnt=0; gLvdsFrmCnt=0; 
			
			for(i=0;i<=bLvdsClkOrder;i++)
				for(j=0;j<=bLvdsSelOrder;j++)
					for(k=0;k<=bLvdsLDIDLY;k++)
						if(gbLDIDLY[i][j][k]!=0) {
							tmp += gbLDIDLY[i][j][k];
							l++;
//			printf("l:%u, [%u][%u][%u] %u, tmp:%lu\r\n",l,i,j,k,gbLDIDLY[i][j][k],tmp);
						}
//			printf("%u, %lu\r\n",l,tmp/l);
		
			for(i=0;i<8;i++)
				for(j=0;j<4;j++)
					for(k=0;k<8;k++) {
						if(gbLDIDLY[i][j][k]>(tmp/l)*1.2 || gbLDIDLY[i][j][k]<(tmp/l)*0.8) nLvdsRes[i][j][k]=0;
						else nLvdsRes[i][j][k]=1;							
					}
				
			endflag = 1;
		}
		else gnLAdjCnt++;
	}
	
	if(endflag == 1){
			UINT i, j =0;
		if(bLvdsClkOrder==7 && bLvdsSelOrder==3 && bLvdsLDIDLY==7){
			for(i=0;i<8;i++) {
					printf("--------------------------------\r\n");
					printf("CLK DLY | 0  1	2  3  4  5  6  7\r\n");
					printf("--------------------------------\r\n");
				for(j=0;j<4;j++){
					printf(" %u   %u  | %u  %u  %u  %u  %u  %u  %u  %u\r\n",i,j,nLvdsRes[i][j][0],nLvdsRes[i][j][1],nLvdsRes[i][j][2],nLvdsRes[i][j][3],nLvdsRes[i][j][4],nLvdsRes[i][j][5],nLvdsRes[i][j][6],nLvdsRes[i][j][7]);
				}	
					printf("--------------------------------\r\n");
			}
			
		printf("0 : invalid, 1 : valid \r\n");			
		
		
		LDO3_SELw(3); LDO2_SELw(2); LDO1_SELw(1); LDO0_SELw(0); 		// SEL ( 0~3 )	
		startflag=0;
		LCK_DLYw(gbOriginClk);
		LDIDLY0w(gbOriginLDI[0]); LDIDLY1w(gbOriginLDI[1]); LDIDLY2w(gbOriginLDI[2]); LDIDLY3w(gbOriginLDI[3]);

		extern BYTE gbLvdsOn;
		gbLvdsOn=0;
		endflag= 0;
		return;
	}

	}
	gLvdsFrmCnt++;
	return;
}
#endif
/* EOF */
