//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		dev_types.h
//	Description:	EN673 SW type definition
//	Author:			EN673 team
//
//	Note:			
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	151026		ygkim		separated from "dev.h" to share the type information with IP SW
//------------------------------------------------------------------------------
#ifndef _DEV_TYPES_H_
#define	_DEV_TYPES_H_

#include <types.h>			// for ushort, uint
#include <stdarg.h>			// for va_list

typedef int						INT;		// 16 bit or 32 bit according to system
typedef unsigned int			UINT;		//	"

typedef char					CHAR;		// 8 bit
typedef char					bool;		//	"
typedef unsigned char			UCHAR;		//	"
typedef unsigned char			uchar;		//	"
typedef unsigned char			BYTE;		//	"
typedef unsigned char			BOOL;		//	"
typedef unsigned char			INT8U;		//	"
typedef unsigned char			u8;			//	"
typedef unsigned char			__u8;		//	"
typedef signed	 char			INT8S;		//	"
typedef signed   char			int8;		//	"
typedef signed char				s8;			//	"
typedef signed char				__s8;		//	"
typedef unsigned char			uint8;		//	"
typedef unsigned char			u_int8;	//	"
typedef volatile uint8			vuint8;	//	"

typedef short					SHORT;		// 16 bit
typedef unsigned short			USHORT;	//	"
typedef unsigned short			WORD;		//	"
typedef unsigned short			WCHAR;		//	"
typedef unsigned short			INT16U;	//	"
typedef unsigned short  		u16;		//	"
typedef unsigned short  		__u16;		//	"
typedef unsigned short			uint16;	//	"
typedef unsigned short	    	u_int16;	//	"
typedef signed	 short			INT16S;	//	"
typedef signed   short			int16;		//	"
typedef signed 	 short			s16;		//	"
typedef signed 	 short			__s16;		//	"
typedef volatile uint16			vuint16;	//	"

typedef long					LONG;		// 32 bit
typedef unsigned long			ULONG;		//	"
typedef unsigned long			ulong;		//	"
typedef unsigned long			DWORD;		//	"
typedef unsigned long			INT32U;	//	"
typedef signed   long			INT32S;	//	"
typedef signed   long			int32;		//	"
typedef signed   long			s32;		//	"
typedef signed   long			__s32;		//	"
typedef unsigned long			u32;		//	"
typedef unsigned long			__u32;		//	"
typedef unsigned long			uint32;	//	"
typedef unsigned long			u_int32;	//	"
typedef volatile uint32			vuint32;	//	"

typedef	long long				LLONG;		// 64 bit
typedef	long long				llong;		//	"
typedef	unsigned long long		ULLONG;	//	"
typedef unsigned long long		ullong;	//	"
typedef unsigned long long		QWORD;		//	"
typedef unsigned long long		INT64U;	//	"
typedef signed	 long long		INT64S;	//	"
typedef signed   long long		int64;		//	"
typedef unsigned long long		uint64;	//	"

typedef unsigned int 			float32;
typedef unsigned long long 		float64;

typedef union {
	float32	F;
	uint	U;
	float	f;
} tConv;

#define	TOFLOAT32(v)			(((tConv*)(&(v)))->F)		// soft float to hard float
#define	TOFLOAT(v)				(((tConv*)(&(v)))->f)		// hard float to  soft float
#define	TOUINT(v)				(((tConv*)(&(v)))->U)			

//******************************************************************************
//
//------------------------------------------------------------------------------
//
#define TRUE					1
#define FALSE					0

#ifndef NULL
#define NULL 					((void*)0)
#endif

#define	UNUSED(x)				((void)(x))

/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:        the pointer to the member.
 * @type:       the type of the container struct this is embedded in.
 * @member:     the name of the member within the struct.
 *
 */
/*typeof is gcc keywords */
#define container_of(ptr, type, member) ({                   \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#endif//_DEV_TYPES_H_
