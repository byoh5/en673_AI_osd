#ifndef __CC_H__
#define __CC_H__

#include "dev.h"
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

typedef unsigned char	u8_t;
typedef signed char		s8_t;
typedef unsigned short	u16_t;
typedef signed short	s16_t;
typedef unsigned long	u32_t;
typedef signed long		s32_t;

typedef u32_t			mem_ptr_t;

#define U16_F "u"
#define S16_F "d"
#define X16_F "x"
#define U32_F "u"
#define S32_F "d"
#define X32_F "x"
#define SZT_F "z"

#ifndef BYTE_ORDER
		#define BYTE_ORDER	BIG_ENDIAN
#else
	#if	BYTE_ORDER!=BIG_ENDIAN
		#undef	BYTE_ORDER
		#define BYTE_ORDER	BIG_ENDIAN
	#endif
#endif

#define PACK_STRUCT_FIELD(x) x
#define PACK_STRUCT_STRUCT	__attribute__ ((packed))
#define PACK_STRUCT_BEGIN
#define PACK_STRUCT_END

#define	LWIP_PLATFORM_DIAG(x)		do {printf x;} while(0)																		// Diagnosis message
#define	LWIP_PLATFORM_ASSERT(x)		do {printf("Assertion \"%s\" failed at line %d in %s\r\n", x, __LINE__, __FILE__); } while(0)	// Assert message

//-------------------------------------------------------------------------------------------------
// H/W assist functions
u16_t my_chksum(void *dataptr, u16_t len);
#define LWIP_CHKSUM	my_chksum
#define LWIP_RAND() ((u32_t)rand())	
//-------------------------------------------------------------------------------------------------
// malloc aliasing
#define mem_free		vPortFree
#define mem_malloc		pvPortMalloc
#define mem_calloc		pvPortCalloc
#define	mem_realloc		pvPortRealloc

//-------------------------------------------------------------------------------------------------
//
//#define LWIP_DEBUG								// Debug message print
//#define LWIP_NOASSERT							// Assert message print

//-------------------------------------------------------------------------------------------------
//
#define ETH_PAD_SIZE			0


#endif /* __CC_H__ */
