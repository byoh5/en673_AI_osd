//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		string.c
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150725		ygkim		merged similar versions of string functions @ ISP & IP's
// -----------------------------------------------------------------------------
#include <string.h>			// for strlen
#include "dev.h"

char gNumcode[] = "0123456789ABCDEF";	// defined @dev.h 


//******************************************************************************
// 3. String
//------------------------------------------------------------------------------
_NOINS char* strrev(char* str)
{
	char *s = str, *d = str + strlen(str)-1;
	while(d>s) {
		char ch=*d; *d--=*s; *s++=ch;
	}
	return str;
}

/* OLD
void strrev(char* s)
{
	int i, j;
	for(i=0, j=strlen(s)-1; i<j; i++, j--) {
	 	char c = s[i];	s[i] = s[j];	s[j] = c;
	}
}

void strrev2(char* begin, char* end)
{
	char ch;
	while(end>begin) {
		ch=*end; *end--=*begin; *begin++=ch;
	}
}
*/

//------------------------------------------------------------------------------
_NOINS char* itoa(int value, char* str, int base, int* len)
{
	char* wstr = str;
	int sign = value;

	// check base
	if(base<2||base>16) { *str = '\0'; return NULL; }

	// sign
	if(value<0)	value = -value;

	// reversed conversion
	do {
		*wstr++ = gNumcode[value%base];
	} while(value/=base);
	if(sign<0) *wstr++ = '-';
	*wstr = '\0';

	// reverse string
	strrev(str);
	*len = (int)(wstr-str);			
	return str;
}

_NOINS int itoan(int value, char* str, int base, int len, char fill_val, int sgned)		// $ADD-ygkim-170707
{
	// Check base
	if (base<2 || base>16) { *str = '\0'; return 0; }

	char* qstr = str + 32;		// temp string buffer: +32 for 32 bits in base = 2
	int sgn = 0;
	uint val = *((uint*)&value);// base=2, 8, 16: always unsigned
	if (sgned && value < 0) {
		val = (uint)(-value);
		sgn = 1;
	}

	int si = 0, qi = 0;

	// Convert to string in reverse
	do {
		qstr[qi++] = gNumcode[val%base];
	} while (val /= base);
	//strrevn(qstr,qi);			// correct order

	int rlen = len - qi - sgn;	// rest length
	
	// If a fill value and sgn exist, add a sign bit first: "-0000Q"
	if (fill_val != ' ' && sgn) str[si++] = '-';

	// Fill zeros
	while (rlen-- > 0) str[si++] = fill_val;

	// If no fill value and sgn exists, add a sign bit: "    -Q"
	if (fill_val == ' ' && sgn) str[si++] = '-';

	// Copy the string in reverse
	while (qi-- > 0) str[si++] = qstr[qi];

	str[si] = '\0';
	return si;
}

_NOINS int uitoan(uint value, char* str, int base, int width, char fill_val)
{
	char* wstr = str;
	int i;

	// check base
	if(base<2||base>16) { *str = '\0'; return 0; }

	// reversed conversion
	do {
		*wstr++=gNumcode[value%base];
	} while(value/=base);

	for(i=wstr-str; i<width; i++)	*wstr++ = fill_val;
	*wstr='\0';

	// reverse string
	strrev(str);
	return (wstr-str);
}

/* NOT YET FULLY TESTED
_NOINS char* ftoa(float32 f, char* str, int prec, int* len)
{
	const int pow[] = {1,10,100,1000,10000};
	char* wstr = str;

	if(prec>4)  prec = 4;

    int fixpart=_ftoi(f);							// max 10 digits
    int fracpart=(int)((f-fixpart)*pow[prec]);		// max 4 digits
	if(fracpart<0)	fracpart=-fracpart;

	// fixed part conversion
	int fixlen;
	itoa(fixpart, wstr, 10, &fixlen);
	wstr += fixlen;
	*wstr++ = '.';
	
	// fraction part conversion
	*len = uitoan(fracpart, wstr, 10, prec, '0');

	*len += fixlen+1;	// fix + '.' + frac			// max 16 bits
	return str;
}
*/

#ifndef DEF_BOOT
_NOINS unsigned long long enx_atoull(char *str)
{
	long long result = 0;
	while (*str >= '0' && *str <= '9')
	{
		result = (result*10) - (*str++ - '0');
	}
	return result;
}

_NOINS long long enx_atoll(char *str)
{
	long long result = 0;
	int negative = 0;
	
	while (*str == ' ' || *str == '\t')
		str++;
	if (*str == '+')
		str++;
	else if (*str == '-') {
		negative = 1;
		str++;
	}
	
	while (*str >= '0' && *str <= '9') {
		result = (result*10) - (*str++ - '0');
	}

	return negative ? result : -result;
}

_NOINS int IPtoStr(unsigned int addr, char* buf, int buflen, int type)
{
	unsigned int s_addr = addr;
	char inv[3];
	char *rp = buf;
	unsigned char *ap = (unsigned char *)&s_addr;
	unsigned char rem, n, i, t;
	int len = 0;

	for(n = 0; n < 4; n++)
	{
		i = 0;
		do {
			rem = *ap % (unsigned char)10;
			*ap /= (unsigned char)10;
			inv[i++] = '0' + rem;
		} while(*ap);
		if(type & 1)
		{
			t = 3 - i;
			while(t--)
			{
				if (len++ >= buflen)
				{
					return (rp-buf);
				}
				*rp++ = '0';
			}
		}
		while(i--)
		{
			if (len++ >= buflen)
			{
				return (rp-buf);
			}
			*rp++ = inv[i];
		}
		if (len++ >= buflen)
		{
			return (rp-buf);
		}
		*rp++ = '.';
		ap++;
	}
	*--rp = 0;
	return (rp-buf);
}

/*
 * Find the first occurrence of find in s, where the search is limited to the
 * first slen characters of s.
 */
char * strnstr(const char *s, const char *find, size_t slen)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != '\0') {
		len = strlen(find);
		do {
			do {
				if (slen-- < 1 || (sc = *s++) == '\0')
					return (NULL);
			} while (sc != c);
			if (len > slen)
				return (NULL);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return ((char *)s);
}

char* enx_strcat(char* dest, char* src)
{
	while (*dest) dest++;
	while ((*dest++ = *src++));
	return --dest;
}
#endif

//******************************************************************************
//
//------------------------------------------------------------------------------
// memchr: elememt-by-element
#ifdef _DEV_MEMCHR
void *_memchr(const void *src, int c, size_t n)
{
	const uchar* s = (const uchar*)src;

    while(n) {
        if(*s == ((uchar)c)) return (void *)s;     // silence the warning
        ++s;
        --n;
    }
    return NULL;
}
#endif
