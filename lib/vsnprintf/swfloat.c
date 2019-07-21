#include "dev.h"
#include "dev_inline.h"

#include "vsnprintf.h"

//
// modf.c 
//
#define __HUGE_VAL_bytes        { 0, 0, 0, 0, 0, 0, 0xf0, 0x7f }

#define __huge_val_t    union { unsigned char __c[8]; double __d; }
#ifdef  __GNUC__
# define HUGE_VAL       (__extension__ \
                         ((__huge_val_t) { __c: __HUGE_VAL_bytes }).__d)
#else   /* Not GCC.  */
static __huge_val_t __huge_val = { __HUGE_VAL_bytes };
# define HUGE_VAL       (__huge_val.__d)
#endif  /* GCC.  */

/* modf suitable for IEEE double-precision */

#define	MASK	0x7ffL
#define FSIGN	0x80000000
#define	SHIFT	20
#define	BIAS	1022L

typedef	union
{
	double	d;
	struct
	{
		long	ms;
		long	ls;
	} i;
} Cheat;

static double modf(double d, double *ip)
{
	Cheat x;
	int e;

	if(-1 < d && d < 1) {
		*ip = 0;
		return d;
	}
	x.d = d;
	x.i.ms &= ~FSIGN;
	e = (x.i.ms >> SHIFT) & MASK;
	if(e == MASK || e == 0)
	{
		*ip = (d > 0)? HUGE_VAL : -HUGE_VAL;
		return 0;
	}
	e -= BIAS;
	if(e <= SHIFT+1) 
	{
		x.i.ms &= ~(0x1fffffL >> e);
		x.i.ls = 0;
	} else
	if(e <= SHIFT+33)
		x.i.ls &= ~(0x7fffffffL >> (e-SHIFT-2));
	if(d > 0){
		*ip = x.d;
		return d - x.d;
	}else{
		*ip = -x.d;
		return d + x.d;
	}
}

//
// cvt.c - IEEE floating point formatting routines for FreeBSD
// from GNU libc-4.6.27
//
#define CVTBUFSIZE (309 + 43)
//static char cvtbuf[CVTBUFSIZE];

static char *enx_cvt(double arg, int ndigits, int *decpt, int *sign, char *buf, int eflag)
{
  int r2;
  double fi, fj;
  char *p, *p1;

  if (ndigits < 0) ndigits = 0;
  if (ndigits >= CVTBUFSIZE - 1) ndigits = CVTBUFSIZE - 2;
  r2 = 0;
  *sign = 0;
  p = &buf[0];
  if (arg < 0)
  {
    *sign = 1;
    arg = -arg;
  }
  arg = modf(arg, &fi);
  p1 = &buf[CVTBUFSIZE];

  if (fi != 0) 
  {
    p1 = &buf[CVTBUFSIZE];
    while (fi != 0) 
    {
      fj = modf(fi / 10, &fi);
      *--p1 = (int)((fj + .03) * 10) + '0';
      r2++;
    }
    while (p1 < &buf[CVTBUFSIZE]) *p++ = *p1++;
  } 
  else if (arg > 0)
  {
    while ((fj = arg * 10) < 1) 
    {
      arg = fj;
      r2--;
    }
  }
  p1 = &buf[ndigits];
  if (eflag == 0) p1 += r2;
  *decpt = r2;
  if (p1 < &buf[0]) 
  {
    buf[0] = '\0';
    return buf;
  }
  while (p <= p1 && p < &buf[CVTBUFSIZE])
  {
    arg *= 10;
    arg = modf(arg, &fj);
    *p++ = (int) fj + '0';
  }
  if (p1 >= &buf[CVTBUFSIZE]) 
  {
    buf[CVTBUFSIZE - 1] = '\0';
    return buf;
  }
  p = p1;
  *p1 += 5;
  while (*p1 > '9') 
  {
    *p1 = '0';
    if (p1 > buf)
      ++*--p1;
    else 
    {
      *p1 = '1';
      (*decpt)++;
      if (eflag == 0) 
      {
	if (p > buf) *p = '0';
	p++;
      }
    }
  }
  *p = '\0';
  return buf;
}

//static char *enx_ecvt(double arg, int ndigits, int *decpt, int *sign)
//{
//	return enx_cvt(arg, ndigits, decpt, sign, cvtbuf, 1);
//}

static char *enx_ecvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
	return enx_cvt(arg, ndigits, decpt, sign, buf, 1);
}

//static char *enx_fcvt(double arg, int ndigits, int *decpt, int *sign)
//{
//	return enx_cvt(arg, ndigits, decpt, sign, cvtbuf, 0);
//}

static char *enx_fcvtbuf(double arg, int ndigits, int *decpt, int *sign, char *buf)
{
	return enx_cvt(arg, ndigits, decpt, sign, buf, 0);
}

// 
// cfltcvt.c
// 
// 
static void enx_cfltcvt(double value, char *buffer, char fmt, int precision)
{
  int decpt, sign, exp, pos;
  char *digits = NULL;
  char cvtbuf[80];
  int capexp = 0;
  int magnitude;

  if (fmt == 'G' || fmt == 'E')
  {
    capexp = 1;
    fmt += 'a' - 'A';
  }

  if (fmt == 'g')
  {
    digits = enx_ecvtbuf(value, precision, &decpt, &sign, cvtbuf);
    magnitude = decpt - 1;
    if (magnitude < -4  ||  magnitude > precision - 1)
    {
      fmt = 'e';
      precision -= 1;
    }
    else
    {
      fmt = 'f';
      precision -= decpt;
    }
  }

  if (fmt == 'e')
  {
    digits = enx_ecvtbuf(value, precision + 1, &decpt, &sign, cvtbuf);

    if (sign) *buffer++ = '-';
    *buffer++ = *digits;
    if (precision > 0) *buffer++ = '.';
    memcpy(buffer, digits + 1, precision);
    buffer += precision;
    *buffer++ = capexp ? 'E' : 'e';

    if (decpt == 0)
    {
      if (value == 0.0)
	exp = 0;
      else
	exp = -1;
    }
    else
      exp = decpt - 1;

    if (exp < 0)
    {
      *buffer++ = '-';
      exp = -exp;
    }
    else
      *buffer++ = '+';

    buffer[2] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[1] = (exp % 10) + '0';
    exp = exp / 10;
    buffer[0] = (exp % 10) + '0';
    buffer += 3;
  }
  else if (fmt == 'f')
  {
    digits = enx_fcvtbuf(value, precision, &decpt, &sign, cvtbuf);
    if (sign) *buffer++ = '-';
    if (*digits)
    {
      if (decpt <= 0)
      {
	*buffer++ = '0';
	*buffer++ = '.';
	for (pos = 0; pos < -decpt; pos++) *buffer++ = '0';
	while (*digits) *buffer++ = *digits++;
      }
      else
      {
	pos = 0;
	while (*digits)
	{
	  if (pos++ == decpt) *buffer++ = '.';
	  *buffer++ = *digits++;
	}
      }
    }
    else
    {
      *buffer++ = '0';
      if (precision > 0)
      {
	*buffer++ = '.';
	for (pos = 0; pos < precision; pos++) *buffer++ = '0';
      }
    }
  }

  *buffer = '\0';
}

static void enx_forcdecpt(char *buffer)
{
  while (*buffer)
  {
    if (*buffer == '.') return;
    if (*buffer == 'e' || *buffer == 'E') break;
    buffer++;
  }

  if (*buffer)
  {
    int n = strlen(buffer);
    while (n > 0)
    {
      buffer[n + 1] = buffer[n];
      n--;
    }

    *buffer = '.';
  }
  else
  {
    *buffer++ = '.';
    *buffer = '\0';
  }
}

static void enx_cropzeros(char *buffer)
{
  char *stop;

  while (*buffer && *buffer != '.') buffer++;
  if (*buffer++)
  {
    while (*buffer && *buffer != 'e' && *buffer != 'E') buffer++;
    stop = buffer--;
    while (*buffer == '0') buffer--;
    if (*buffer == '.') buffer--;
    while ((*++buffer = *stop++));
  }
}

char *flt(char *str, double num, int size, int precision, char fmt, int flags)
{
  char tmp[80];
  char c, sign;
  int n, i;

  // Left align means no zero padding
  if (flags & LEFT) flags &= ~ZEROPAD;

  // Determine padding and sign char
  c = (flags & ZEROPAD) ? '0' : ' ';
  sign = 0;
  if (flags & SIGN)
  {
    if (num < 0.0)
    {
      sign = '-';
      num = -num;
      size--;
    }
    else if (flags & PLUS)
    {
      sign = '+';
      size--;
    }
    else if (flags & SPACE)
    {
      sign = ' ';
      size--;
    }
  }

  // Compute the precision value
  if (precision < 0)
    precision = 6; // Default precision: 6
  else if (precision == 0 && (fmt == 'g' || fmt == 'G'))
    precision = 1; // ANSI specified

  // Convert floating point number to text
  enx_cfltcvt(num, tmp, fmt, precision);

  // '#' and precision == 0 means force a decimal point
  if ((flags & SPECIAL) && precision == 0) enx_forcdecpt(tmp);

  // 'g'/'G' format means crop zero unless '#' given
  if ((fmt == 'g' || fmt == 'G') && !(flags & SPECIAL)) enx_cropzeros(tmp);

  n = strlen(tmp);

  // Output number with alignment and padding
  size -= n;
  if (!(flags & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
  if (sign) *str++ = sign;
  if (!(flags & LEFT)) while (size-- > 0) *str++ = c;
  for (i = 0; i < n; i++) *str++ = tmp[i];
  while (size-- > 0) *str++ = ' ';

  return str;
}
