/* **************************************************************************
 File Name		: app_imd_test.c
 Description	: Motion detector Test function
 Designer		: Kim, Sung-hoon
 Date			: 18.02.07
 Copyright �� Eyenix Co., Ltd. All Rights Reserved.
 
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

		// �������� �ΰ�(���� ��ȭ���� ����, ũ��ʹ� �������)�ϰ� �����մϴ�.
		// ���� ���� 0xd �̸�, ���� ������ 0x3 ~ 0x17 �Դϴ�.
		IMD_THw(0xd);

		// Motion������ 32x32 pixel ������ ������ �մϴ�.
		// ���� Motion���� ���� ũ��� FR_HW/32 = 1920>>5 = 60 �� �˴ϴ�.
		IMD_HWw(FR_HW>>5);

		// Motion������ 32x32 pixel ������ ������ �մϴ�.
		// ���� Motion���� ���� ũ��� FR_VW/32 = 1080>>5 = 34 �� �˴ϴ�.
		IMD_VWw(FR_VW>>5);

		// ���� ���� �ӵ� �Դϴ�.
		// ���� ���� ���� ������ ������ ���ŵǸ�, �����̴� ��ü�� ��� ���ߴ� ��쿡�� �������� �ʽ��ϴ�.
		// ���� Ŭ ���� ���� ������ ������ ���ŵǸ�, �����̴� ��ü�� ��� ���߰ų� ���� ������ ����� ��쿡�� ���� ������ �����˴ϴ�.
		// ���� ������ 0x1 ~ 0x1e �Դϴ�.
		IMD_BUPCw(0x1);

		// ������ ���� �ּ� �����Դϴ�.
		// ������ ũ�Ⱑ ������ �������� ū ��츸 �����մϴ�.
		// ���� ������ 1�� 32x32 pixel �Դϴ�.
		// ���� ��� 4�� ������ ��� ������ ũ�Ⱑ 4x32x32 pixel ���� ū ��츸 �����մϴ�.
		IMD_NUMTHw(4);

		// ������ ���� �ּ� ���� ũ���Դϴ�.
		// �������� ���� ũ�Ⱑ ������ ũ�⺸�� ū ��츸 �����մϴ�.
		// ���� ������ 1�� 32 pixel �Դϴ�.
		IMD_HTHw(1);

		// ������ ���� �ּ� ���� ũ���Դϴ�.
		// �������� ���� ũ�Ⱑ ������ ũ�⺸�� ū ��츸 �����մϴ�.
		// ���� ������ 1�� 32 pixel �Դϴ�.
		IMD_VTHw(1);

		// �Ʒ��� Motion�� ����ϱ� ���� �⺻ initial �����Դϴ�. ī�޶� ���� �� �ѹ��� �����Ͻø� �˴ϴ�.
		IMD_HSPw(0);
		IMD_VSPw(0);
		IMD_IIRAUTOw(0);
		IMD_IIRKw(0x50);
		
		bInitFlag = 0;
	}

	//----------------------------------------------------------------------------------------
	// ����� Motion ���� ó��

	#define IMD_RECT_MAX	16	// ����� Motion ������ ����� OSD BOX ����

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
		if(ValSft_R31(GetIsp(0x0260+i)) & BitMask_01)	// ����� Motion ��ǥ�� �ִ��� Ȯ��
		{
			// ����� Motion ��ǥ ��������
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
	// OSD BOX�� ����� Motion ���� ���
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


