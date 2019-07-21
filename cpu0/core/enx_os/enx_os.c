#include <stdlib.h>	// rand_r, rand, srand, atoi

#include "dev.h"
#include "enx_os.h"

/* for enx_os_sprintf */
#include <stdarg.h>
#include <stddef.h>

extern int vsnprintf(char* str, size_t size, const char* format, va_list ap);

size_t enx_os_strlen(const char* s)
{
	return strlen(s);
}

char* enx_os_strncpy(char* dest, const char* src, size_t count)
{
	return strncpy(dest, src, count);
}

int enx_os_strcmp(const char* cs, const char* ct)
{
	return strcmp(cs,ct);
}

char* enx_os_strcpy(char* dest,const char* src)
{
	return strcpy(dest,src);
}

char* enx_os_memset(char* str, int c, int len)
{
	return memset(str,c,len);
}

int enx_os_rand_r(unsigned int* seed)
{
	return rand_r(seed);
}

int enx_os_rand(void)
{
	return rand();
}

void enx_os_srand(unsigned int seed)
{
	srand(seed);
}

char* enx_os_strcat(char* dest, const char* src)
{
	return strcat(dest, src);
}

int enx_os_atoi(char* str)
{
	return atoi(str);
}

int enx_os_memcmp(const void* cs, void* ct, size_t count)
{
	return memcmp(cs, ct, count);	
}

char* enx_os_strstr(const char* s1, const char* s2)
{
	return strstr(s1, s2);
}

/*
char* enx_os_strchr(const char *s, int c)
{
	return strchar(s,c);
}
*/

void* enx_os_memcpy(void* dest, const void* src, size_t count)
{
	return memcpy(dest, src, count);
}

int enx_os_snprintf(char* buf, size_t count, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(buf, count, fmt, args);
	va_end(args);

	return len;
}

int enx_os_sprintf(char* buf, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	int len = vsnprintf(buf, 2048, fmt, args);
	va_end(args);

	return len;
}

void enx_os_msleep(unsigned int ms)
{
	WaitXms(ms);
}
