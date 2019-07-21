/* **************************************************************************
 File Name		: app_imd_test.c
 Description	: Motion detector Test function
 Designer		: Kim, Sung-hoon
 Date			: 18.02.07
 Copyright ⓒ Eyenix Co., Ltd. All Rights Reserved.
 
*************************************************************************** */
#include "dev_isp.h"
#include "dev_inline.h"
#if 0
void ISPM MotionTest(void)
{
	static BYTE bInitFlag = 1;

	if(bInitFlag) {
		// Motion ON/OFF
		IMD_ONw(1);

		// 작을수록 민감(작은 변화에도 검출, 크기와는 상관없음)하게 검출합니다.
		// 권장 값은 0xd 이며, 권장 범위는 0x3 ~ 0x17 입니다.
		IMD_THw(0xd);

		// Motion검출은 32x32 pixel 단위로 검출을 합니다.
		// 따라서 Motion검출 영역 크기는 FR_HW/32 = 1920>>5 = 60 이 됩니다.
		IMD_HWw(FR_HW>>5);

		// Motion검출은 32x32 pixel 단위로 검출을 합니다.
		// 따라서 Motion검출 영역 크기는 FR_VW/32 = 1080>>5 = 34 이 됩니다.
		IMD_VWw(FR_VW>>5);

		// 검출 지연 속도 입니다.
		// 작을 수록 검출 정보가 빠르게 갱신되며, 움직이는 객체가 잠깐 멈추는 경우에도 검출하지 않습니다.
		// 값이 클 수록 검출 정보가 느리게 갱신되며, 움직이는 객체가 잠깐 멈추거나 영상 밖으로 사라진 경우에도 검출 정보가 유지됩니다.
		// 권장 범위는 0x1 ~ 0x1e 입니다.
		IMD_BUPCw(0x1);

		// 움직임 검출 최소 면적입니다.
		// 움직임 크기가 설정된 면적보다 큰 경우만 검출합니다.
		// 설정 단위는 1당 32x32 pixel 입니다.
		// 예를 들어 4로 설정한 경우 움직임 크기가 4x32x32 pixel 보다 큰 경우만 검출합니다.
		IMD_NUMTHw(4);

		// 움직임 검출 최소 수평 크기입니다.
		// 움직임의 수평 크기가 설정된 크기보다 큰 경우만 검출합니다.
		// 설정 단위는 1당 32 pixel 입니다.
		IMD_HTHw(1);

		// 움직임 검출 최소 수직 크기입니다.
		// 움직임의 수직 크기가 설정된 크기보다 큰 경우만 검출합니다.
		// 설정 단위는 1당 32 pixel 입니다.
		IMD_VTHw(1);

		// 아래는 Motion을 사용하기 위한 기본 initial 설정입니다. 카메라 부팅 시 한번만 설정하시면 됩니다.
		IMD_HSPw(0);
		IMD_VSPw(0);
		IMD_IIRAUTOw(0);
		IMD_IIRKw(0x50);
		
		bInitFlag = 0;
	}

	//----------------------------------------------------------------------------------------
	// 검출된 Motion 영역 처리

	#define IMD_RECT_MAX	16	// 검출된 Motion 영역을 출력할 OSD BOX 개수

	typedef struct _IMD_RECT{
		unsigned char sx;
		unsigned char sy;
		unsigned char ex;
		unsigned char ey;
	}bRECT;

	bRECT DrawRect[IMD_RECT_MAX];
	int i, iDrawRectNum = 0;

	for(i=0; i<32; i++)
	{
		if(ValSft_R31(GetIsp(0x0260+i)) & BitMask_01)	// 검출된 Motion 좌표가 있는지 확인
		{
			// 검출된 Motion 좌표 가져오기
			(DrawRect + iDrawRectNum)->sx = (ValSft_R24(GetIsp(0x0260+i))&BitMask_07);
			(DrawRect + iDrawRectNum)->sy = (ValSft_R18(GetIsp(0x0260+i))&BitMask_06);
			(DrawRect + iDrawRectNum)->ex = (ValSft_R11(GetIsp(0x0260+i))&BitMask_07) + 1;
			(DrawRect + iDrawRectNum)->ey = (ValSft_R05(GetIsp(0x0260+i))&BitMask_06) + 1;

			iDrawRectNum++;
			if(iDrawRectNum == IMD_RECT_MAX) break;
		}
		else break;
	}

	//----------------------------------------------------------------------------------------
	// OSD BOX로 검출된 Motion 영역 출력
	#define BOX_COLOR_Y		0x51
	#define BOX_COLOR_Cb	0x5a
	#define BOX_COLOR_Cr	0xef

	gnBoxOnIMD = 0;
	for(i=0; i<iDrawRectNum; i++)
	{
		gnBoxPosIMD[(i<<1)]   = ((DrawRect + i)->sy<<5)<<16 | ((DrawRect + i)->ey<<5);
		gnBoxPosIMD[(i<<1)+1] = ((DrawRect + i)->sx<<5)<<16 | ((DrawRect + i)->ex<<5);
		gnBoxPltIMD[i]        = (BOX_COLOR_Y<<16)|(BOX_COLOR_Cb<<8)|BOX_COLOR_Cr;
		
		gnBoxOnIMD |= (1<<i);
	}
}
#endif
/* EOF */


