//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_float.c
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	170109		ygkim		first designed
// -----------------------------------------------------------------------------

#include "exr1k_float.h"

float32 _frnd(float32 fa)
{
	const float r = 0.5f;
	float32 fr = TOFLOAT32(r);
	// float32 fr = TOFLOAT32(0.5f);	// equal 

	float32 fb = _fadd(fa, fr);			// fb = fa + 0.5f;
	int b = _ftoi(fb);					// b = int(fb);
	fb = _itof(b);						// fb = float(b);
	return fb;
}

float32 _fabs(float32 fa)
{
	uint a = TOUINT(fa) & 0x7FFFFFFF;	// remove the sign bit
	return TOFLOAT32(a);
}

float32 _fneg(float32 fa)
{
	uint a = TOUINT(fa) ^ 0x80000000;	// negate the sign bit
	return TOFLOAT32(a);
}
