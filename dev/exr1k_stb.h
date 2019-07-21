//******************************************************************************
//	Copyright(c) Young-Geun Kim 2003-
//
//	File Name:		exr1k_stb.h
//	Description:	Software Trace Buffer
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}), Eyenix Co.,Ltd.
//
//	Note:			Use the compiler options -finstrument-functions
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151007		ygkim		first designed
// -----------------------------------------------------------------------------
#ifndef _EXR1K_STB_HEADER
#define _EXR1K_STB_HEADER

#include "dev_types.h"			// to use bool
		
//#define _STB_ENABLE  					// defined @config.mk
//#define _STB_NAME_ENABLE				// defined @config.mk

//******************************************************************************
//
//------------------------------------------------------------------------------
void stb_init(ushort cr);
void stb_reset_buf(void);
void stb_enable_trace(void);
void stb_disable_trace(void);

bool stb_push_name(void* addr, const char* name);

void stb_print_ring(void);
void stb_print_name(void);
void stb_print_info(void);
void stb_print_trace(void);
void stb_print_level(void);

#endif//_EXR1K_STB_HEADER
