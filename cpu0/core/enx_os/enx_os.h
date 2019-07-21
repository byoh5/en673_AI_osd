#ifndef __ENX_OS_H__
#define __ENX_OS_H__

#include "dev_types.h"

size_t enx_os_strlen(const char* s);
char* enx_os_strncpy(char* dest, const char* src, size_t count);
int enx_os_strcmp(const char* cs, const char* ct);
char* enx_os_strcpy(char* dest,const char* src);
char* enx_os_memset(char* str, int c, int len);
int enx_os_rand_r(unsigned int* seed);
int enx_os_rand(void);
void enx_os_srand(unsigned int seed);
char* enx_os_strcat(char* dest, const char* src);
int enx_os_atoi(char* str);
int enx_os_memcmp(const void* cs, void* ct, size_t count);
char* enx_os_strstr(const char* s1, const char* s2);
//char* enx_os_strchr(const char *s, int c);
void* enx_os_memcpy(void* dest, const void* src, size_t count);
int enx_os_snprintf(char* buf, size_t count, const char* fmt, ...);
int enx_os_sprintf(char* buf, const char* fmt, ...);
void enx_os_msleep(unsigned int ms);

#endif /* __ENX_OS_H__ */