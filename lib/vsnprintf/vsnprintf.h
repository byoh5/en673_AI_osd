#ifndef __VSNPRINTF_H__
#define __VSNPRINTF_H__

#ifndef DEF_BOOT
// software float
extern char *flt(char *str, double num, int size, int precision, char fmt, int flags);	// swfloat.c

// hardware float
extern int ftoaf(float32 f, char* str, int width, char fill_val, int flen, bool xftz, int flags);	// hwfloat.c
extern int ftoae(float32 f, char* str, int width, int flen, int* expo, bool xftz);		// hwfloat.c
extern int ftoag(float32 f, char* str, int width);										// hwfloat.c
extern int ftox(float32 f, char* str);													// hwfloat.c
#endif

// vsnprintf format list
// %c           : Character
// %s           : String of characters
// %p           : Pointer address
// %d, %ld, %i  : Signed decimal integer
// %u, %lu      : Unsigned decimal integer
// %o           : Unsigned octal
// %x, %X       : Unsigned hexadecimal integer
// %f           : Software: Decimal floating point
// %e, %E       : Software: Scientific notation (mantissa/exponent)
// %g, %G       : Software: Use the shortest representation: %e or %f
// %hf          : Hardware: Decimal floating point(FPU)
// %he          : Hardware: Scientific notation(FPU)
// %hg          : Hardware: Use the shortest representation: %he or %hf(FPU)
// %hq          : Hardware: Decimal floating point(FPU, special format)
// %IP          : IP address(unsigned int(0xC0A80001) to IP address string(192.168.0.1))(special format)
// %n           : Nothing printed. The corresponding argument must be a pointer to a signed int. The number of characters written so far is stored in the pointed location.
// %%           : A % followed by another % character will write a single % to the stream
extern int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);			// vsnprintf.c

#endif // __VSNPRINTF_H__
