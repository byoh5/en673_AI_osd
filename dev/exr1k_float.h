//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_float.h
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:			ref to systfloat.h
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151110		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef	_EXR1K_FLOAT_H_		
#define	_EXR1K_FLOAT_H_

#include "dev_types.h"

extern float32 _itof(int);
extern int _ftoi(float32);

extern float32 _fadd(float32, float32);
extern float32 _fsub(float32, float32);
extern float32 _fmul(float32, float32);
extern float32 _fdiv(float32, float32);
extern float32 _frem(float32, float32);
extern bool _feq(float32, float32);
extern bool _fge(float32, float32);
extern bool _fgt(float32, float32);
extern bool _fle(float32, float32);
extern bool _flt(float32, float32);
extern bool _fne(float32, float32);

extern float32 _frnd(float32);
extern float32 _fabs(float32);
extern float32 _fneg(float32);

//extern bool _feq_signaling(float32, float32);
//extern bool _fle_quiet(float32, float32);
//extern bool _flt_quiet(float32, float32);

#endif//_EXR1K_FLOAT_H_