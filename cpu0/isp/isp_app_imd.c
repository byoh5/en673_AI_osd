/* **************************************************************************
 File Name		: app_imd.c
 Description	: Motion detector
 Designer		: Lee, Gyu-Hong
 Date			: 15.08.05
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
 
*************************************************************************** */

#include "dev_isp.h"
#include "dev_inline.h"

//*************************************************************************************************
// Constant Define
//-------------------------------------------------------------------------------------------------
#define 	MD_STEP 		32 								// Should be '32'
	
#define 	MD_OFFSET_X 	0
#define 	MD_OFFSET_Y 	0


//*************************************************************************************************
// Operation Macro Declaration & Define
//-------------------------------------------------------------------------------------------------

#define IMD_DRAW_RECT(NUM, RECT) 	gnBoxPosIMD[NUM*2] = (RECT->sy<<16) | (RECT->ey<<0);\
									gnBoxPosIMD[NUM*2+1] = (RECT->sx<<16) | (RECT->ex<<0);\


#define IMD_DRAW_RECT1(NUM, RECT) 	gnBoxPosIMD[NUM*2]	 = (RECT.sy<<16) | (RECT.ey<<0);\
									gnBoxPosIMD[NUM*2+1] = (RECT.ex<<0) | (RECT.sx<<16);

#define MINUS_IMD(A, B) 			((A <= B) ? 0x0 : A - B)


//*************************************************************************************************
// Grobal Variable Declaration
//-------------------------------------------------------------------------------------------------

UINT	gnIMD_Hw;							
UINT	gnIMD_Vw;

UINT	gnIMG_Hw;
UINT	gnIMG_Vw;

BYTE 	gbIMD_Combine_On;
BYTE	gbIMD_Moving_On;

BYTE 	gbIMD_MAX;
BYTE	gbIMD_BoxMaxBit;				

UINT	gnIMD_MovingThres; 				// Threshold for moving detection.
//UINT	gnIMD_DetectNum;
BYTE	gbIMD_Moving;

BYTE 	gbIMD_DetectNumSlowK; 			// IIR coefficient
BYTE 	gbIMD_DetectNumFastK; 			// IIR coefficient	

UINT	gnIMD_Hsp;						//Start Position
UINT	gnIMD_Vsp;						

BYTE	gbIMD_AreaBox; 					// Mask window	
BYTE	gbIMD_WinBox;					// Window Box

BYTE	gbIMD_Mask[MASK_EA]; 			// 0 : Normal operation, 1 : Mask operation
BYTE	gbIMD_Mask_sx[MASK_EA]; 		// Mask area : start X
BYTE	gbIMD_Mask_ex[MASK_EA]; 		// Mask area : end X
BYTE	gbIMD_Mask_sy[MASK_EA]; 		// Mask area : start Y
BYTE	gbIMD_Mask_ey[MASK_EA]; 		// Mask area : end Y

BYTE 	gbIMD_Win_Pos[4]; 				// Window mask area

nRECT 	DrawRectOSD[IMD_MAX_BOX_NUM]; 	// Position of Detected Motion

//Box Information for OSD
UINT 	gnBoxPosIMD[IMD_MAX_BOX_NUM*2];
UINT 	gnBoxPltIMD[IMD_MAX_BOX_NUM];
UINT 	gnBoxToneIMD[2];
UINT 	gnBoxFillIMD;
UINT 	gnBoxOnIMD;

BYTE	gbIMDFontX;
BYTE 	gbIMDFontY;

UINT 	gnDetectNum;	

BYTE 	gbIMDHold=0;

BYTE	gbIMD_OSDBoxNum;				// Public, after Area check Final the number of box.
BYTE	gbIMD_Bright;
BYTE 	gbBgDly;
BYTE	gbAccTh;
	
BYTE 	gbImdDebugOn;					//DebugMode Variable	

//*************************************************************************************************
// Function Define
//-------------------------------------------------------------------------------------------------
extern void LibImdLabeling(BYTE abOn, BYTE abMaskMode, BYTE abFilpMode, BYTE abMirrorMode);
void ISPM MovingChk(int *aiMask_sx,int *aiMask_ex,int *aiMask_sy,int *aiMask_ey);

void InitIMD(void)
{
	gnIMG_Hw 				= FR_HW; 						// Active image horizontal size
	gnIMG_Vw 				= FR_VW; 						// Active image vertical size

 	gnIMD_Hsp 				= IMD_HSPr; 					// IMD horizontal start posiion
	gnIMD_Vsp 				= IMD_VSPr; 					// IMD vertical start position

 	gbIMD_Combine_On 		= 0; 							
 	gbIMD_Moving_On 		= 1;							// Should be '0x1'
	
 	gbIMD_MAX 				= IMD_MAX_BOX_NUM; 				// The number of Box for IMD
 	
	gbIMD_BoxMaxBit 		= IMD_MAX_BOX_NUM - 6;			// Motion Detection Box Max Bit (Mask 4ea, Window 1ea, Margine 1ea) Total 6ea

	gbIMD_AreaBox			= 11;							// Mask window		No11,12,13,14 Active Mask Box
	gbIMD_WinBox			= 15;							// Window Box			No15. Magine Box

	gbIMD_DetectNumSlowK 	= IMD_MVFILTER_SLOW;			// For camera moving detection
	gbIMD_DetectNumFastK 	= IMD_MVFILTER_FAST;			// 		"
	gnIMD_MovingThres 		= IMD_MVFILTER_THRS; 			//

	SetIsp(0x0068, 0x803c0022);								// IMD_WON1 Enable & Area Setting
}

void ISPM IMD(void)
{
	gbImdDebugOn = (gbMnDebugFnc==6); 						//IMD Debug Mode

	UINT nSensSt;											//Sensor (1.3M ,2M)
	BYTE bThresOffset;

	static BYTE bMpItl_Buf=0xEE;	// 2017228 - WHL

//-------------------------------------------------------------------------------------------------
// Setting values according to the sensor

	#if (model_1M)																//1.3M
		nSensSt = 0;
		gbIMDFontX = MN_MXSP+1;
		gbIMDFontY = 14;

		gnIMD_Hw = IMD_HW; 
		gnIMD_Vw = IMD_VW;
	
//		#elif(model_2M30p)
	#elif(model_2M30p|model_2M)
		if((MP(MpOutFps)==MN_2M_720p_25_30)||(MP(MpOutFps)==MN_2M_720p_50_60))	//2M - 720p 50,60 Crop Mode
		{	nSensSt  = 1;
			gbIMDFontX = MN_MXSP+1;		gbIMDFontY = 14;
			gnIMD_Hw = 0x28; 	gnIMD_Vw   = 0x17;}	
		else																	//2M - 1080p 50,60
		{	nSensSt  = 2;
			gbIMDFontX = MN_MXSP+1;		gbIMDFontY = 14;	
			gnIMD_Hw = IMD_HW; 	gnIMD_Vw   = IMD_VW;}
			
	#else
	
	#endif	
	
	if (1/*MP(MpItl)==MN_ON*/) {									//IMD on
		//-------------------------------------------------------------------------------------------------
		// 1. Variable Declaration & Initial
		int 	i;
		int 	mdThres;
		
		//Window Area
		int iWinSX = 0;
		int iWinEX = 0;
		int iWinSY = 0;
		int iWinEY = 0;
		
		//IMD_MASK 0 : Mask Inner, IMD_MASK 1 : Mask Outer									
		int iMaskSX[4]; 									// Mask OSD area : start X
		int iMaskEX[4]; 									// Mask OSD area : end X
		int iMaskSY[4]; 									// Mask OSD area : start Y
		int iMaskEY[4]; 									// Mask OSD area : end Y
			
		//Window & Mask Offset		[0] : SX or SY , [1] : EX or EY
		int iWinOffX[2] ={0,};
		int iWinOffY[2] = {0,};
		int iMaskOffX[2] = {0,};
		int iMaskOffY[2] = {0,};
		int iBoxOffX[2] = {0,};
		int iBoxOffY[2] = {0,};
															// Initial
		gnIMG_Hw = HWOr;
		gnIMG_Vw = VWOr;

		//IMD Active Area HSP, VSP Set
		switch(nSensSt)
		{
			case 0:						//1.3M
				gnIMD_Vsp = (MP(MpFlip))  ? 0x00:0x0d;
				gnIMD_Hsp = (MP(MpMirror))? 0x00:0x0d;
				break;
			case 1:						//2M - 720_50_60 Crop
				gnIMD_Vsp = (MP(MpFlip))  ? 0x00:0x00;
				gnIMD_Hsp = (MP(MpMirror))? 0x00:0x00;
				break;
			case 2:						//2M - 1080p
				gnIMD_Vsp = (MP(MpFlip))  ? 0x00:0x00;
				gnIMD_Hsp = (MP(MpMirror))? 0x00:0x00;
				break;
			default:
				gnIMD_Vsp = (MP(MpFlip))  ? 0x00:0x00;
				gnIMD_Hsp = (MP(MpMirror))? 0x00:0x00;
				break;
		}

		static UINT nIMD_HspBuf=0xEE, nIMD_VspBuf=0xEE, nIMD_HwBuf=0xEE, nIMD_VwBuf=0xEE;
		
		if((nIMD_HspBuf!=gnIMD_Hsp)|(nIMD_VspBuf!=gnIMD_Vsp)|(nIMD_HwBuf!=gnIMD_Hw)|(nIMD_VwBuf!=gnIMD_Vw))
		{	//IMD Active Area Set
		
			IMD_HSPw(gnIMD_Hsp); IMD_HWw(gnIMD_Hw); // hpos
			IMD_VSPw(gnIMD_Vsp); IMD_VWw(gnIMD_Vw); // vpos
		}
		nIMD_HspBuf = gnIMD_Hsp; nIMD_HwBuf = gnIMD_Hw;
		nIMD_VspBuf = gnIMD_Vsp; nIMD_VwBuf = gnIMD_Vw;

		//Background Delay (UP)
		gbBgDly = MIN(UP(UpIMD_BG_UP),FR);				// 1 FrameRate Max Value
		IMD_BUPCw(gbBgDly);

	if(MP(MpItl)==MN_ON) {
		//Total Window Area
		gbIMD_Win_Pos[0] = 0;							// Window Area Sx
		gbIMD_Win_Pos[1] = gnIMD_Hw;					//	  "		 Ex
		gbIMD_Win_Pos[2] = 0;							//	  "		 Sy
		gbIMD_Win_Pos[3] = gnIMD_Vw;					//	  "		 Ey
		
		//Mask Position
		#define MASK_POS_H	(gbMnImdCfg(i)->bPosX)		
		#define MASK_POS_V	(gbMnImdCfg(i)->bPosY)
		#define MASK_SIZ_H	(gbMnImdCfg(i)->bSizX)
		#define MASK_SIZ_V	(gbMnImdCfg(i)->bSizY)

		for(i=0; i<MASK_EA; i++)
		{
			gbIMD_Mask[i] 	 = gbMnImdCfg(i)->bAction;									//Mask Number [0-4] (0:Off, 1:On)
		
			gbIMD_Mask_sx[i] = MASK_POS_H;
			gbIMD_Mask_ex[i] = MASK_POS_H + MASK_SIZ_H;
			gbIMD_Mask_sy[i] = MASK_POS_V;
			gbIMD_Mask_ey[i] = MASK_POS_V + MASK_SIZ_V;

			if(nSensSt==1)										//Crop Mode
			{
				if(gbIMD_Mask_sx[i]>gnIMD_Hw)	gbIMD_Mask_sx[i] = gnIMD_Hw;
				if(gbIMD_Mask_ex[i]>gnIMD_Hw)	gbIMD_Mask_ex[i] = gnIMD_Hw;
				if(gbIMD_Mask_sy[i]>gnIMD_Vw)	gbIMD_Mask_sy[i] = gnIMD_Vw;
				if(gbIMD_Mask_ey[i]>gnIMD_Vw)	gbIMD_Mask_ey[i] = gnIMD_Vw;
			}
		}
	}
		
		//-------------------------------------------------------------------------------------------------
		// 2. Threshold
		mdThres = LibUtlInterp1D (giCurAgc, IMD_AGC_MIN, IMD_AGC_MAX, 0x0, 0x5); 		// Control threshold by AGC level
		bThresOffset = MIN(UP(UpIMD_THRES_WEG),3);											// Max offset Value 3
		IMD_THw(((IMD_SENS_MAX - MP(MpItlSens))*(1+bThresOffset) /*Up Offset Value (0-3)*/ ) + mdThres + 3); 	// 3 : Offset - (LGH_UP)

		//DebugDisp(gbImdDebugOn,,"IMD_TH",6,25,IMD_THr);
		IMD_ONw(0x1); 																	// IMD ON
		//-------------------------------------------------------------------------------------------------
		// 3. Labeling area & Labeling
//		LibImdLabeling(MP(MpItl),IMD_MASK_MODE,MP(MpFlip),MP(MpMirror)); 				//Lib Labeling
//		LibImdLabeling(MP(MpItl),IMD_MASK_MODE,0,MP(MpMirror)); 				//Lib Labeling
		LibImdLabeling(MP(MpItl),IMD_MASK_MODE,0,0); 				//Lib Labeling
		//------------------------------------------------------------------------------
		// 4. MD Window OSD Area Calcuration
	if(MP(MpItl)==MN_ON) {
			switch(nSensSt)
			{
				case 0:						//1.3M
					//Win_X_Offset
					iWinOffX[0] = (MP(MpMirror)) ? 0x0:0x0;
					iWinOffX[1] = (MP(MpMirror)) ? 0x0:0x0;

					//Win_Y_Offset
					iWinOffY[0] = (MP(MpFlip)) ? 0x0:0x0;
					iWinOffY[1] = (MP(MpFlip)) ? 0x0:0x0;
					break;
				case 1:						//2M 720_50_60 Crop
					//Win_X_Offset
					iWinOffX[0] = (MP(MpMirror)) ? 0x0:0x0;
					iWinOffX[1] = (MP(MpMirror)) ? 0x0:0x0;

					//Win_Y_Offset
					iWinOffY[0] = (MP(MpFlip)) ? 0x0:0x0;
					iWinOffY[1] = (MP(MpFlip)) ? 0x0:0x0;
					break;			
				case 2:						//2M 1080
					//Win_X_Offset
					iWinOffX[0] = (MP(MpMirror)) ? 0x0:0x0;
					iWinOffX[1] = (MP(MpMirror)) ? 0x0:0x0;

					//Win_Y_Offset
					iWinOffY[0] = (MP(MpFlip)) ? 0x0:0x0;
					iWinOffY[1] = (MP(MpFlip)) ? 0x0:0x0;
//						iWinOffY[0] = (MP(MpFlip)) ? PAR1C:PAR1D;
//						iWinOffY[1] = (MP(MpFlip)) ? PAR1E:PAR1F;
					break;			
					
				default:					
					//Win_X_Offset
					iWinOffX[0] = (MP(MpMirror)) ? 0x0:0x0;
					iWinOffX[1] = (MP(MpMirror)) ? 0x0:0x0;

					//Win_Y_Offset
					iWinOffY[0] = (MP(MpFlip)) ? 0x0:0x0;
					iWinOffY[1] = (MP(MpFlip)) ? 0x0:0x0;
					break;
			}
			
			
			iWinSX = iWinOffX[0];
			iWinSY = iWinOffY[0];		
			
			iWinEX = iWinSX +(gnIMD_Hw * MD_STEP)+ iWinOffX[1];
			iWinEY = iWinSY +(gnIMD_Vw * MD_STEP)+ iWinOffY[1];
			
			if (iWinSX < 0) 				iWinSX = 0;
			if (iWinSY < 0) 				iWinSY = 0;
			if (iWinEX > (int)gnIMG_Hw) 	iWinEX = (int)gnIMG_Hw;
			if (iWinEY > (int)gnIMG_Hw) 	iWinEY = (int)gnIMG_Hw;

		//-------------------------------------------------------------------------------
 		// 5. MD Mask OSD Area Calcuration
	
			switch(nSensSt)
			{
				case 0:						//1.3M		14-08-13
					//Mask_X_Offset
					if(MP(MpMirror))	{iMaskOffX[0] = 0x0;  iMaskOffX[1] = 0x0;
										 iBoxOffX[0]  = 0x0;  iBoxOffX[1]  = 0x0;}
		
					else				{iMaskOffX[0] = 0x0;  iMaskOffX[1] = 0x0;
										 iBoxOffX[0]  = 0x0;  iBoxOffX[1]  = 0x0;}

					//Mask_Y_Offset
					if(MP(MpFlip))		{iMaskOffY[0] = 0x0;  iMaskOffY[1] = 0x0;
										 iBoxOffY[0]  = 0x0; iBoxOffY[1]  = 0x0;}

					else				{iMaskOffY[0] = 0x0; iMaskOffY[1] = 0x0;				
										 iBoxOffY[0]  = 0x0; iBoxOffY[1]  = 0x0;}		
					break;

				case 1:						//2M 720_50_60 Crop
					//Mask_X_Offset
					if(MP(MpMirror))	{iMaskOffX[0] = 0x0;  iMaskOffX[1] = 0x0;
										 iBoxOffX[0]  = 0x0;  iBoxOffX[1]  = 0x0;}

					else				{iMaskOffX[0] = 0x0;  iMaskOffX[1] = 0x0;
										 iBoxOffX[0]  = 0x0;  iBoxOffX[1]  = 0x0;}
		
					//Mask_Y_Offset
					if(MP(MpFlip))		{iMaskOffY[0] = 0x0;  iMaskOffY[1] = 0x0;
										 iBoxOffY[0]  = 0x0;  iBoxOffY[1] = 0x0;}

					else				{iMaskOffY[0] =  0x0; iMaskOffY[1] =  0x0;
										 iBoxOffY[0]  =  0x0;  iBoxOffY[1]  = 0x0;}
					break;		

				case 2:						//2M 1080
					//Mask_X_Offset
					if(MP(MpMirror))	{iMaskOffX[0] = -0x4;  iMaskOffX[1] = -0x3;
										 iBoxOffX[0]  = -0x2;  iBoxOffX[1]  = -0x4;}

					else				{iMaskOffX[0] = 0x0;   iMaskOffX[1] = 0x0;
										 iBoxOffX[0]  = 0x0;   iBoxOffX[1]  = 0x0;}
		
					//Mask_Y_Offset
					if(MP(MpFlip))		{iMaskOffY[0] = -0x2;  iMaskOffY[1] = -0x2;
										 iBoxOffY[0]  = 0x0;   iBoxOffY[1] = -0x2;}

					else				{iMaskOffY[0] =  0x0; iMaskOffY[1] = 0x0;
										 iBoxOffY[0]  =  0x0;  iBoxOffY[1]  = 0x0;}
					break;			
					
				default:					break;
					}
					

			for (i = 0; i < MASK_EA; i++) {
					
				iMaskSX[i] = gbIMD_Mask_sx[i] * MD_STEP + iMaskOffX[0];
				iMaskSY[i] = gbIMD_Mask_sy[i] * MD_STEP + iMaskOffY[0];	

				iMaskEX[i] = gbIMD_Mask_ex[i] * MD_STEP + iMaskOffX[1]; 				// Box position offset	
				iMaskEY[i] = gbIMD_Mask_ey[i] * MD_STEP + iMaskOffY[1]; 				// 		"
					
				if (iMaskSX[i] < 0) 				iMaskSX[i] = 0;
				if (iMaskSY[i] < 0) 				iMaskSY[i] = 0;
				if (iMaskEX[i] > (int)gnIMG_Hw) 	iMaskEX[i] = (int)gnIMG_Hw;
				if (iMaskEY[i] > (int)gnIMG_Hw) 	iMaskEY[i] = (int)gnIMG_Hw;				
			}			
		//-------------------------------------------------------------------------------------------------
		// 6. MD OSD Box Calcuration
		
			for (i = 0; i < (int)gbIMD_OSDBoxNum; i++) {
				if ((gbIMD_BoxMaxBit - (unsigned char)i) >= 0) {
					
					(DrawRectOSD+i)->sx += iBoxOffX[0];
					(DrawRectOSD+i)->ex += iBoxOffX[1];	
					(DrawRectOSD+i)->sy += iBoxOffY[0];
					(DrawRectOSD+i)->ey += iBoxOffY[1];

					(DrawRectOSD+i)->sx = MAX((DrawRectOSD+i)->sx, (UINT)iWinSX);
					(DrawRectOSD+i)->ex = MIN((DrawRectOSD+i)->ex, (UINT)iWinEX);
					(DrawRectOSD+i)->sy = MAX((DrawRectOSD+i)->sy, (UINT)iWinSY);
					(DrawRectOSD+i)->ey = MIN((DrawRectOSD+i)->ey, (UINT)iWinEY);
				}
			}
	}
		//-------------------------------------------------------------------------------
		// 7. IMD Hold 
			if(gbIMDHold) { IMD_KMAXw(0x00); IMD_KMINw(0x00); IMD_KGAw(0x00); } 	
			else          { IMD_IIRAUTOw(0x0);	IMD_IIRKw(0x50);}

	if(MP(MpItl)==MN_ON) {			
		MovingChk(iMaskSX,iMaskEX,iMaskSY,iMaskEY);				//Motion Alarm

		//-------------------------------------------------------------------------------
		// 8. Mask Osd On
		if(MP(MpItlMaskOsd))		//Mask OSD On
		{
			//MD Window OSD 
			const BYTE bWintrans = MP(MpItlDettone);					//Window Tone
			const BYTE bMskBscTone = 0x3;				//Mask Basic Tone
			UINT	tmp = 0;
			nRECT 	rRect;
		
			rRect.sx = iWinSX;
			rRect.ex = iWinEX;
			rRect.sy = iWinSY;
			rRect.ey = iWinEY;

			gnBoxPltIMD[gbIMD_WinBox] 		= (235<<16 | 128<<8 | 128);				 // White (YCbCr)
			
			IMD_DRAW_RECT1(gbIMD_WinBox, rRect); 									// Draw Large Window

			//Mask Box OSD Draw & Pallet Setting
			#if (IMD_MASK_MODE==1)					//Mask Mode 
				gnBoxFillIMD = (1<<gbIMD_AreaBox)|(1<<(gbIMD_AreaBox+1))|(1<<(gbIMD_AreaBox+2))|(1<<(gbIMD_AreaBox+3));		//Fill (0~3)
	
				gnBoxToneIMD[0] = (bWintrans<<(gbIMD_AreaBox*2))|(bWintrans<<(gbIMD_AreaBox+1)*2)			//Tone (0~3)
								 |(bWintrans<<(gbIMD_AreaBox+2)*2)|(bWintrans<<(gbIMD_AreaBox+3)*2);
			
				// Mask OSD Box area		B0~3
				for (i = 0; i < MASK_EA; i++) {
					gnBoxPosIMD[(gbIMD_AreaBox+i)*2] 	= (VWIr-iMaskSY[i]) | (VWIr-iMaskEY[i])<<16;	
					gnBoxPosIMD[(gbIMD_AreaBox+i)*2+1] 	= (iMaskEX[i]) | (iMaskSX[i]<<16);
					
					gnBoxPltIMD[i] = IMD_MASK_COLOR;								//Mask OSD Box attributionn			
				} 

				if(gbIMDMenuStep)													//Select Mask [0~3]
					gnBoxPltIMD[MP(MpItlWindowuse)] = MASK_SEL_COLOR;
				
			#else
				//Detection Area Tone
				for (i = 0; i < MASK_EA; i++) {
					gnBoxPosIMD[(gbIMD_AreaBox+i)*2] 	= (iMaskSY[i]<<16)|(iMaskEY[i]);
					gnBoxPosIMD[(gbIMD_AreaBox+i)*2+1] 	= (iMaskSX[i]<<16)|(iMaskEX[i]);

					gnBoxPltIMD[gbIMD_AreaBox+i] = IMD_MASK_COLOR;								//Mask OSD Box attributionn		
				} 
		
				//transparency
				if(bWintrans==4)
				{
					gnBoxFillIMD = (0x1<<gbIMD_WinBox)|(0x0<<gbIMD_AreaBox)|(0x0<<(gbIMD_AreaBox+1))|(0x0<<(gbIMD_AreaBox+2))|(0x0<<(gbIMD_AreaBox+3));
					gnBoxToneIMD[0] = (0x3<<(gbIMD_WinBox*2))|(0x0<<(gbIMD_AreaBox+0)*2)|(0x0<<(gbIMD_AreaBox+1)*2) |(0x0<<(gbIMD_AreaBox+2)*2)|(0x0<<(gbIMD_AreaBox+3)*2);

					if(gbIMDMenuStep)
						gnBoxPltIMD[gbIMD_AreaBox+MP(MpItlWindowuse)] = MASK_SEL_COLOR;
				}

				else
				{
					gnBoxFillIMD = (0x1<<gbIMD_WinBox)|(gbIMD_Mask[0]<<gbIMD_AreaBox)|(gbIMD_Mask[1]<<(gbIMD_AreaBox+1))\
												  |(gbIMD_Mask[2]<<(gbIMD_AreaBox+2))|(gbIMD_Mask[3]<<(gbIMD_AreaBox+3));
					gnBoxToneIMD[0] = (bWintrans<<(gbIMD_WinBox*2))|(bMskBscTone<<(gbIMD_AreaBox+0)*2)|(bMskBscTone<<(gbIMD_AreaBox+1)*2)\
															  	   |(bMskBscTone<<(gbIMD_AreaBox+2)*2)|(bMskBscTone<<(gbIMD_AreaBox+3)*2);
					if(gbIMDMenuStep)
					{	
						//Bit Clear
						tmp = gnBoxToneIMD[0];
						tmp &= ~(0x3<<(gbIMD_AreaBox+MP(MpItlWindowuse))*2);
						gnBoxToneIMD[0] = tmp|(0x2<<(gbIMD_AreaBox+MP(MpItlWindowuse))*2);
						gnBoxPltIMD[gbIMD_AreaBox+MP(MpItlWindowuse)] = MASK_SEL_COLOR;
					}
				}
						
			#endif

			//IMD_Window, Mask On/Off
			gnBoxOnIMD = (0x1<<gbIMD_WinBox)|(gbIMD_Mask[0]<<gbIMD_AreaBox)|(gbIMD_Mask[1]<<(gbIMD_AreaBox+1))\
										    |(gbIMD_Mask[2]<<(gbIMD_AreaBox+2))|(gbIMD_Mask[3]<<(gbIMD_AreaBox+3));
			
			if(gbIMD_Bright && (!gbIMD_Moving)){
				//Box
				for (i = 0; i < (int)gbIMD_OSDBoxNum; i++) 
				{
					if ((gbIMD_BoxMaxBit - (unsigned char)i) >= 0) 
					{
						if(MP(MpItlRectFill)==MN_ON)
						{
							gnBoxFillIMD |= (0x1<<(gbIMD_BoxMaxBit - (unsigned char)i));
							tmp = gnBoxToneIMD[0];
							tmp &= ~(0x3<<(gbIMD_BoxMaxBit - (unsigned char)i)*2);
							gnBoxToneIMD[0] = tmp|(0x2<<(gbIMD_BoxMaxBit - (unsigned char)i)*2);
						}
						else
						{
							gnBoxFillIMD |= (0x0<<(gbIMD_BoxMaxBit - (unsigned char)i));
						}
						
						gnBoxPltIMD[(gbIMD_BoxMaxBit - (unsigned char)i)] = IMD_MDBOX_COLOR;			// attribution
						IMD_DRAW_RECT((gbIMD_BoxMaxBit - (unsigned char)i), (DrawRectOSD+i));			//Box Position Save
					}
				}

				gnBoxOnIMD = (gbIMDHold) ? 0 : gnBoxOnIMD | (gnIMD_DrawBoxBit & BitMask_11) ;			// Box on/off	 	
			}
		}																	//Mask OSD ON Code End	
		else	gnBoxOnIMD = 0;	  																	//Mask OSD OFF

	} else {
		if(bMpItl_Buf!=MP(MpItl)) DispClr(gbIMDFontY,gbIMDFontX,18);

	#if (model_MotionOut==1)
		GpioSetLo(GPIO_MOTION);																		//GPIO Low
	#endif

		gnBoxOnIMD = 0;																				//Box Off
	}

	}																		//IMD ON Code End
																								
	else {																							//IMD OFF
		IMD_ONw(0x0);

		if(bMpItl_Buf!=MP(MpItl)) DispClr(gbIMDFontY,gbIMDFontX,18);	// 2017228 - WHL

		//if(MP(MpPrivacy)==MN_OFF) DispClr(gbIMDFontY, gbIMDFontX, 18);
		
	#if (model_MotionOut==1)
		GpioSetLo(GPIO_MOTION);																		//GPIO Low
	#endif
		
		gnBoxOnIMD = 0;																				//Box Off
	}
	bMpItl_Buf=MP(MpItl);	// 2017228 - WHL

//		static BYTE bClr_Buf;
//		if(bClr_Buf!=gbMenuState)	DispClr(gbIMDFontY, gbIMDFontX, 18);
//		bClr_Buf = gbMenuState;

	
//		DebugDisp((gbMnDebugFnc==8), Dec, "FPS_VDI:", model_1M? 1:19, 24, FPS_VDI);
//		DebugDisp((gbMnDebugFnc==6), Dec, "POS_H   ", 2, 24, MASK_POS_H);
//		DebugDisp((gbMnDebugFnc==6), Dec, "POS_V   ", 3, 24, MASK_POS_V);
	DebugDisp((gbMnDebugFnc==6), Dec, "gnIMG_Hw", 4, 24, gnIMG_Hw);
	DebugDisp((gbMnDebugFnc==6), Dec, "gnIMG_Vw", 5, 24, gnIMG_Vw);
	
	DebugDisp((gbMnDebugFnc==6), Dec, "gnIMD_Hw", 7, 24, gnIMD_Hw);
	DebugDisp((gbMnDebugFnc==6), Dec, "gnIMD_Vw", 8, 24, gnIMD_Vw);
	DebugDisp((gbMnDebugFnc==6), Dec, "MD_STEP ", 9, 24, MD_STEP);
}


void ISPM MovingChk(int *aiMask_sx,int *aiMask_ex,int *aiMask_sy,int *aiMask_ey)
{
	int i,j;
	int	iAreaChk;
	int iMaskOn=0;
	int iMovingArea=0;

	static BYTE bMsgCnt = 1;
	static BYTE	bBgCnt = 0;

	//Camera Moving
	UINT 	nHW = DrawRectOSD[0].ex - DrawRectOSD[0].sx; 	// Get the largest box
	UINT 	nVW = DrawRectOSD[0].ey - DrawRectOSD[0].sy; 	// 		"
	
	DebugDisp((gbMnDebugFnc==6), Dec, "nHW     ", 4, 24, nHW);
	DebugDisp((gbMnDebugFnc==6), Dec, "nVW     ", 5, 24, nVW);

	UINT nMaxHw = 0,nMaxVw = 0;
	BYTE bMovingSize = 0;
	
	gbIMD_Bright = (IMD_BRIGHT_TH_MIN<giErr1d)&&(giErr1d<IMD_BRIGHT_TH_MAX);
	
	for(i=0; i<MASK_EA; i++)
	{	
		if(gbIMD_Mask[i])
		{
			iMaskOn++;									//Several of Mask On
			nMaxHw = aiMask_ex[i] - aiMask_sx[i];
			nMaxVw = aiMask_ey[i] - aiMask_sy[i];
			iMovingArea += (nHW >= (nMaxHw*(2+UP(UpIMD_MOVING_TH)-1)/(2+UP(UpIMD_MOVING_TH))))&&(nVW >= (nMaxVw*(2+UP(UpIMD_MOVING_TH)-1)/(2+UP(UpIMD_MOVING_TH))));
		}
	}

	bMovingSize = (iMovingArea >= iMaskOn)? 1 : 0;
	
	if(gbIMDHold) gbIMD_Moving = /*gbIMD_DrawBoxNum =*/ 0;
	else          gbIMD_Moving = (/*gbIMD_Moving||*/bMovingSize);	// 170330 KSH

	const BYTE bBgUpdate = (!gbIMD_Bright)||(gbIMD_Moving)||(bBgCnt);
	
	// Motion Alarm (GPIO_MD_OUT)
	if(!gbMenuState && (MP(MpItlAlarm)==MN_ON))						//Menu Off
	{
		FONT_ONw(1);
		
		if(bBgUpdate){
//			bMsgCnt = 0;
			gbIMD_OSDBoxNum = 0;
			gnIMD_DrawBoxBit = 0;

			if((!gbIMD_Bright)||(gbIMD_Moving))		bBgCnt = FPS_VDI;			//Camera Moving or Bright Change (1s)
			else									bBgCnt--;			
			
			gbBgDly = FPS_VDI;
			gbAccTh = 0x0;

			if(!gbIMD_Bright)			DispStr(" BRIGHT CHANGE !!!", gbIMDFontY, gbIMDFontX, 18);
			else if(gbIMD_Moving)		DispStr(" CAMERA MOVING !!!", gbIMDFontY, gbIMDFontX, 18);

			if(!bBgCnt)					DispClr(gbIMDFontY,gbIMDFontX,18);
		}

		else
		{
			if(bMsgCnt)			bMsgCnt--;

			if (gbIMD_OSDBoxNum > 0) 				//Motion Detection 
			{
				for(i=0; i<MASK_EA; i++)				//Detection Area 
				{
					gnDetectNum = 0;
					for(j=0; j<(int)gbIMD_OSDBoxNum; j++)		//Total Motion Detection Box
					{
						if(!IMD_MASK_MODE)				//Not Masking Mode & Detection Area On
						{
							iAreaChk = (gbIMD_Mask[i]) && (aiMask_sx[i]<=(int)DrawRectOSD[j].sx) && (aiMask_ex[i]>=(int)DrawRectOSD[j].ex)\
									   				   && (aiMask_sy[i]<=(int)DrawRectOSD[j].sy) && (aiMask_ey[i]>=(int)DrawRectOSD[j].ey);
							gnDetectNum += iAreaChk;

							if(gnDetectNum==gbIMD_OSDBoxNum)	//One Window Area All Motion !						
							{	
								switch(i)
								{
								case 0 : DispStr("WINDOW0 MOTION !!!", gbIMDFontY, gbIMDFontX, 18);	break;
								case 1 : DispStr("WINDOW1 MOTION !!!", gbIMDFontY, gbIMDFontX, 18);	break;
								case 2 : DispStr("WINDOW2 MOTION !!!", gbIMDFontY, gbIMDFontX, 18);	break;
								case 3 : DispStr("WINDOW3 MOTION !!!", gbIMDFontY, gbIMDFontX, 18);	break;
								default:															break;
								}
							}		
							else{
								DispStr("WINDOWS MOTION !!!", gbIMDFontY, gbIMDFontX, 18);	
							}
						}

						else										//Masking Mode (IMD_MASK_MODE==1)
							DispStr("WINDOW  MOTION !!!", gbIMDFontY, gbIMDFontX, 18);
					}
					if(gnDetectNum){
						bMsgCnt = FPS_VDI;							// TEXT ALARM SPEED (1s)
						break;
					}
				}
			}

			else if(!bMsgCnt)	DispClr(gbIMDFontY,gbIMDFontX,18);
		}
	}

	if(MP(MpItlSigno)==MN_ON)
	{
		if((!gbIMD_Bright) || (gbIMD_Moving) || (!gbIMD_OSDBoxNum))			//Not Correct Moving, Low Signal
		{
		#if (model_MotionOut==1)
			GpioSetLo(GPIO_MOTION);
		#endif
//			TxStr("Low",0,"\n");
			DispStr("Motion OFF!!!", gbIMDFontY, gbIMDFontX, 13);	
			gptMsgISPTbl.ISP_MOTION_REF = 0;
		}
		else
		{
		#if (model_MotionOut==1)
			GpioSetHi(GPIO_MOTION);
		#endif
//			TxStr("High",1,"\n");
			DispStr("Motion ON !!!", gbIMDFontY, gbIMDFontX, 13);	
			gptMsgISPTbl.ISP_MOTION_REF = 1;
		}
	}
//	else
//		GpioSetLo(GPIO_MOTION);

	if((!gbIMD_Bright) || (gbIMD_Moving) || (!gbIMD_OSDBoxNum))
	{
		//
	}
	else
	{
		gptMsgISPTbl.ISP_MOTION = 1;
	}
}

/* EOF */

