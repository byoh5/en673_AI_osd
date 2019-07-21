/*
 * @OSF_FREE_COPYRIGHT@
 */
/*
 * HISTORY
 * 
 * Revision 1.1.1.1  1998/09/22 21:05:32  wsanchez
 * Import of Mac OS X kernel (~semeria)
 *
 * Revision 1.1.1.1  1998/03/07 02:25:56  wsanchez
 * Import of OSF Mach kernel (~mburg)
 *
 * Revision 1.1.8.2  1997/02/12  12:52:33  stephen
 * 	New file, reimplemented to be sure of copyright status
 * 	Initially only supports character matching and '%d'
 * 	[1997/02/12  12:43:09  stephen]
 *
 * $EndLog$
 */

#include <stdarg.h>
#include <stddef.h>

#define isdigit(c) ((unsigned) ((c) - '0') < 10U)

/*
 * Scan items from a string in accordance with a format.  This is much
 * simpler than the C standard function: it only recognises %d without a
 * field width, and does not treat space in the format string or the
 * input any differently from other characters.  The return value is the
 * number of characters from the input string that were successfully
 * scanned, not the number of format items matched as in standard sscanf.
 * e.mcmanus@opengroup.org, 12 Feb 97
 */
int
sscanf(const char *str, const char *format, ...)
{
	const char *start = str;
	va_list args;
	
	va_start(args, format);
	for ( ; *format != '\0'; format++) {
		if (*format == '%' && format[1] == 'd') {
			int positive;
			int value;
			int *valp;
			
			if (*str == '-') {
				positive = 0;
				str++;
			} else
				positive = 1;
			if (!isdigit(*str))
				break;
			value = 0;
			do {
				value = (value * 10) - (*str - '0');
				str++;
			} while (isdigit(*str));
			if (positive)
				value = -value;
			valp = va_arg(args, int *);
			*valp = value;
			format++;
		} 
		else if (*format == *str) {
			str++;
		} else
			break;
	}
	va_end(args);
	return str - start;
}
