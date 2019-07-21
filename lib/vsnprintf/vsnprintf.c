#include "dev.h"
#include "dev_inline.h"

#include "vsnprintf.h"

#define	BF_LEN	270

#define is_digit(c) ((c) >= '0' && (c) <= '9')

static int skip_atoi(const char **s)
{
	int i = 0;
	while (is_digit(**s)) i = i*10 + *((*s)++) - '0';
	return i;
}

static char *number(char *str, long num, int base, int size, int precision, int type)
{
	const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	const char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char c, sign, tmp[66];
	char *dig = (char *)digits;
	int i;

	if (type & LARGE) dig = (char *)upper_digits;
	if (type & LEFT) type &= ~ZEROPAD;
	if (base < 2 || base > 36) return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN)
	{
		if (num < 0)
		{
			sign = '-';
			num = -num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}

	if (type & SPECIAL)
	{
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}

	i = 0;

	if (num == 0)
		tmp[i++] = '0';
	else
	{
		while (num != 0)
		{
			tmp[i++] = dig[((unsigned long) num) % (unsigned) base];
			num = ((unsigned long) num) / (unsigned) base;
		}
	}

	if (i > precision) precision = i;
	size -= precision;
	if (!(type & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
	if (sign) *str++ = sign;

	if (type & SPECIAL)
	{
		if (base == 8)
			*str++ = '0';
		else if (base == 16)
		{
			*str++ = '0';
			*str++ = 'x';
		}
	}

	if (!(type & LEFT)) while (size-- > 0) *str++ = c;
	while (i < precision--) *str++ = '0';
	while (i-- > 0) *str++ = tmp[i];
	while (size-- > 0) *str++ = ' ';

	return str;
}

#ifndef DEF_BOOT
static char *numberll(char *str, long long num, int base, int size, int precision, int type)
{
	const char *digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	const char *upper_digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char c, sign, tmp[66];
	char *dig = (char *)digits;
	int i;

	if (type & LARGE) dig = (char *)upper_digits;
	if (type & LEFT) type &= ~ZEROPAD;
	if (base < 2 || base > 36) return 0;

	c = (type & ZEROPAD) ? '0' : ' ';
	sign = 0;
	if (type & SIGN)
	{
		if (num < 0LL)
		{
			sign = '-';
			num = -num;
			size--;
		}
		else if (type & PLUS)
		{
			sign = '+';
			size--;
		}
		else if (type & SPACE)
		{
			sign = ' ';
			size--;
		}
	}

	if (type & SPECIAL)
	{
		if (base == 16)
			size -= 2;
		else if (base == 8)
			size--;
	}

	i = 0;

	if (num == 0LL)
		tmp[i++] = '0';
	else
	{
		while (num != 0LL)
		{
			tmp[i++] = dig[((unsigned long long) num) % (unsigned) base];
			num = ((unsigned long long) num) / (unsigned) base;
		}
	}

	if (i > precision) precision = i;
	size -= precision;
	if (!(type & (ZEROPAD | LEFT))) while (size-- > 0) *str++ = ' ';
	if (sign) *str++ = sign;

	if (type & SPECIAL)
	{
		if (base == 8)
			*str++ = '0';
		else if (base == 16)
		{
			*str++ = '0';
			*str++ = 'x';
		}
	}

	if (!(type & LEFT)) while (size-- > 0) *str++ = c;
	while (i < precision--) *str++ = '0';
	while (i-- > 0) *str++ = tmp[i];
	while (size-- > 0) *str++ = ' ';

	return str;
}
#endif

int vsnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
	int len;
	unsigned long num;
	int i, base;
	char *str;
	const char *s;
	int flags;
	int dotflag;
	int field_width;
	int precision;
	int qualifier;
#ifndef DEF_BOOT
	char buff[BF_LEN+1] = {0};
#endif

	size--;
	for (str = buf; *fmt && size; ++fmt) {
		if (*fmt != '%') {
			*str++ = *fmt;
			size--;
			continue;
		}

		flags = 0;
		dotflag = 0;
repeat:
		++fmt;
		switch (*fmt) {
			case '-': flags |= LEFT; goto repeat;
			case '+': flags |= PLUS; goto repeat;
			case ' ': flags |= SPACE; goto repeat;
			case '#': flags |= SPECIAL; goto repeat;
			case '0': flags |= ZEROPAD; goto repeat;
		}

		field_width = -1;
		if (is_digit(*fmt))
			field_width = skip_atoi(&fmt);
		else if (*fmt == '*') {
			++fmt;
			field_width = va_arg(args, int);
			if (field_width < 0) {
				field_width = -field_width;
				flags |= LEFT;
			}
		}

		precision = -1;
		if (*fmt == '.') {
			dotflag++;
			++fmt;
			if (is_digit(*fmt))
				precision = skip_atoi(&fmt);
			else if (*fmt == '*') {
				++fmt;
				precision = va_arg(args, int);
			}
			/* NB: the default precision value is conversion dependent */
		}

		qualifier = -1;
		if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L' || *fmt == 'I') {
			qualifier = *fmt;
			++fmt;
		}

		switch (*fmt) {
		case 'c':
			if (!(flags & LEFT))
			{
				while (--field_width > 0 && size)
				{
					*str++ = ' ';
					size--;
				}
			}
			if (size)
			{
				*str++ = (unsigned char)va_arg(args, int);
				size--;
			}
			while (--field_width > 0 && size)
			{
				*str++ = ' ';
				size--;
			}
			continue;
		case 's':
			if (dotflag && precision < 0)
				precision = 0;
			s = va_arg(args, char *);
			if(!s)
				s = "(null)";

			len = strnlen(s, precision);

			if(!(flags & LEFT))
			{
				while(len < field_width-- && size)
				{
					*str++ = ' ';
					size--;
				}
			}

			for(i = 0; i < len && size; ++i)
			{
				*str++ = *s++;
				size--;
			}

			while(len < field_width-- && size)
			{
				*str++ = ' ';
				size--;
			}
			continue;

		case 'p':
			if (dotflag && precision < 0)
				precision = 0;
			if (field_width == -1)
			{
				field_width = 2 * sizeof(void*);
				flags |= ZEROPAD;
			}
			str = number(str, (unsigned long)va_arg(args, void*), 16, field_width, precision, flags);
			continue;

		case 'n':
			if (qualifier == 'l')
			{
				long *ip = va_arg(args, long*);
				*ip = (str - buf);
			}
			else
			{
				int *ip = va_arg(args, int*);
				*ip = (str - buf);
			}
			continue;

		case 'b':
			base = 2;
			break;

		case 'o':
			base = 8;
			break;

		case 'd':
		case 'i':
			flags |= SIGN;
		case 'u':
			base = 10;
			break;

		case 'X':
			flags |= LARGE;
		case 'x':
			base = 16;
			break;

#ifndef DEF_BOOT
		case 'l':
			++fmt;
			switch(*fmt)	// %llb, %llo, %lld, %lli, %llu, %llX, %llx
			{
				case 'b':
					base = 2;
					break;
				
				case 'o':
					base = 8;
					break;

				case 'd':
				case 'i':
					flags |= SIGN;
				case 'u':
					base = 10;
					break;

				case 'X':
					flags |= LARGE;
				case 'x':
					base = 16;
					break;

				default:
					if (size)
					{
						*str++ = '%';
						size--;
					}
					if(qualifier == 'l' && size)
					{
						*str++ = 'l';
						size--;
					}
					if (size)
					{
						*str++ = 'l';
						size--;
					}
					if (*fmt && size)
					{
						*str++ = *fmt;
						size--;
					}
					else
					{
						--fmt;
					}

					continue;
			}

			{
				long long numll;
				if(flags & SIGN)
				{
					numll = va_arg(args, long long);
				}
				else
				{
					numll = va_arg(args, unsigned long long);
				}

				if(dotflag && precision < 0)
				{
					precision = 0;
				}
				str = numberll(str, numll, base, field_width, precision, flags);
			}
			continue;

		case 'f':
		case 'E':
		case 'G':
		case 'e':
		case 'g':
			if(qualifier == 'h')	// Hardware float(FPU)(%hf, %hE, %hG, %he, %hg)
			{
				if(precision == -1)
				{
					precision = 6;
				}
				else
				{
					precision = min(precision, 50);		
				}
				field_width = min(field_width, BF_LEN);	
				num = va_arg(args, float32);
				if(*fmt == 'e' || *fmt == 'E')								// %he
				{
					int expo;
					len = ftoae(num, buff, precision+2, precision, &expo, 0);
				}
				else if(*fmt == 'g' || *fmt == 'G')							// %hg
					len = ftoag(num, buff, precision);
				else
					len = ftoaf(num, buff, field_width, ' ', precision, 0, flags);	// %hf
				for(i = 0; i < len && size; ++i)
				{
					*str++ = buff[i];
					size--;
				}
				continue;
			}
			else					// Sofrware float(%f, %E, %G, %e, %g)
			{
				str = flt(str, va_arg(args, double), field_width, precision, *fmt, flags | SIGN);
			}
			continue;

		case 'q': // special format for floats
			if(qualifier == 'h')		// Hardware float(FPU)(%hf)
			{
				num = va_arg(args, float32);
				len = ftox(num, buff);
				if(!(flags & LEFT))
				{
					while(len < field_width-- && size)
					{
						*str++ = ' ';
						size--;
					}
				}
				for(i = 0; i < len && size; ++i)
				{
					*str++ = buff[i];
					size--;
				}
				while(len < field_width-- && size)
				{
					*str++ = ' ';
					size--;
				}
			}
			else
			{
				if (size)
				{
					*str++ = '%';
					size--;
				}
				if (*fmt && size)
				{
					*str++ = *fmt;
					size--;
				}
				else
				{
					--fmt;
				}
			}
			continue;

		case 'P':	// special format for IPaddress(%IP)
			if(qualifier == 'I')
			{
				num = va_arg(args, unsigned int);
				precision = min(size, 15);
				len = IPtoStr(num, buff, precision, flags);
				if(!(flags & LEFT))
				{
					while(len < field_width-- && size)
					{
						*str++ = ' ';
						size--;
					}
				}
				for(i = 0; i < len && size; ++i)
				{
					*str++ = buff[i];
					size--;
				}
				while(len < field_width-- && size)
				{
					*str++ = ' ';
					size--;
				}
			}
			else
			{
				if (size)
				{
					*str++ = '%';
					size--;
				}
				if (*fmt && size)
				{
					*str++ = *fmt;
					size--;
				}
				else
				{
					--fmt;
				}
			}
			continue;
#endif

		default:
			if (*fmt != '%' && size)
			{
				*str++ = '%';
				size--;
			}
			if(qualifier == 'l' && size)
			{
				*str++ = 'l';
				size--;
			}
			if (*fmt && size)
			{
				*str++ = *fmt;
				size--;
			}
			else
			{
				--fmt;
			}

			continue;
		}

		if(qualifier == 'l')
		{
			num = va_arg(args, unsigned long);
		}
		else if(qualifier == 'h')
		{
			if(flags & SIGN)
			{
				num = (short)va_arg(args, int);
			}
			else
			{
				num = (unsigned short)va_arg(args, unsigned);
			}
		}
		else if(flags & SIGN)
		{
			num = va_arg(args, int);
		}
		else
		{
			num = va_arg(args, unsigned int);
		}

		if(dotflag && precision < 0)
		{
			precision = 0;
		}
		str = number(str, num, base, field_width, precision, flags);
	}

	*str = '\0';
	return str - buf;
}