//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		hwfloat.c
//	Description:	
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr})
//
//	Note:	
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
// 	0.1	170703		ygkim		added printing float in high precision
// -----------------------------------------------------------------------------

#include "dev.h"
#include "dev_inline.h"

#include "vsnprintf.h"

//------------------------------------------------------------------------------
// IEEE-754 single precision loat format
#define WE			8
#define WF			23
#define EXP_MAX		((1<<WE)-1)

int32 _is_neg(float32 v)      { return (v >> (WE + WF)) & 0x1;  }
int32 _mantissa(float32 v)    { return  v & ((1 << WF) - 1);    }
int32 _exponent(float32 v)    { return (v >> WF) & 0xFF;        }

//------------------------------------------------------------------------------
// High precision buffer
#define	WB			32		// Bit width/word

#define	LD			9		// Data length: ceil((127+23+126 bits)/32) = 9 Words
#define LQ			4		// Integer part Q length
#define LF			(LD-LQ)	// Fraction part F length

_NOINS void _reset_bits(uint32 *D)
{
	int i;
	for(i=0; i<LD; i++) D[i] = 0;
}

_NOINS void _insert_bits(uint32 *D, uint32 F, int S)
{
	S += WB * LF;				// add decimal point

	int lsft = S & (WB - 1);
	int hsft = S / WB;
	int hidx = (LD - 1) - hsft;	// reverse
	int lidx = hidx + 1;

	uint32 hdat = lsft==0 ? 0 : F >> (WB - lsft);
	uint32 ldat = F << lsft;

	D[hidx] = hdat;
	D[lidx] = ldat;
}

_NOINS int _Q_is_zero(uint32 *D)
{
	int i;
	for (i = 0; i < LQ; i++) {
		if (D[i]) return 0;
	}
	return 1;
}

_NOINS int _F_is_zero(uint32 *D)
{
	int i;
	for (i = LQ; i < LD; i++) {
		if (D[i]) return 0;
	}
	return 1;
}

//#define IDX(i)	(i^1)	// for little-Endian
#define IDX(i)		(i)		// for big-Endian

_NOINS int _Q_div(uint32 *D, uint32 __d)
{
	uint16* D16 = (uint16*)D;
	uint16 r = 0;

	int i;
	for (i = 0; i < LQ * 2; i++) {
		uint32 n = ((uint32)r << 16) + D16[IDX(i)];
		uint16 q = (uint16)(n / __d);	
		r = (uint16)(n % __d);
		D16[IDX(i)] = q;
	}

	return r;
}

_NOINS int _F_mul(uint32 *D, uint32 __d)
{
	uint16* D16 = (uint16*)D;
	uint16 ovf = 0;

	int i;
	for (i = LD * 2-1; i >= LQ * 2; i--) {
		uint32 n = __d * D16[IDX(i)] + ovf;
		D16[IDX(i)] = (uint16)n;
		ovf = (uint16)(n >> 16);
	}
	return ovf;
}

//------------------------------------------------------------------------------
// float convert
// : to convert a float into a string with fraction length
// args:	f,		a single precision float
//			str,	string buffer
//			flen,	fraction length
//			lzv,	leading zero is valid or not
//			lzc,	leading zero count returned
// return:	slen,	slen<0 means INF/NaN
_NOINS int enx_fcvt(float32 f, char* str, int flen, bool lzv, int* lzc)
{
	uint32 D[LD];

	int slen = 0;

	// Extract bit fields
	int expo = _exponent(f);
	int mant = _mantissa(f);

	// Check: NaNs or INF, return -length
	if (expo == EXP_MAX)
	{
		// +/-INF
		if (mant == 0)	{ 
			strcpy(str, "INF");
			return -3;	
		}

		// NaN: Return the mantissa field of the NaN.
		strcpy(str, "NaN");
		itoan(mant, str + 3, 16, 6, 0, 0);
		return -9;		
	}

	// Check: Denormals
	if (expo == 0)
		expo = -126;				// No special exponent value
	else {
		expo -= 127;				// in [254,1]-127 = [127,-126], bcz @255: NaN/INF, 0: Denormalized
		mant |= (1 << WF);			// 1.f : 24 bits
	}

	_reset_bits(D);
	_insert_bits(D, mant, expo + 9);

	// Interger part division
	do
	{
		int remainder = _Q_div(D, 10);
		str[slen++] = '0' + remainder;
		if (remainder != 0) lzv = 1;		// non zero Q sets leading zero valid
	} while (!_Q_is_zero(D));

	// Put the digits in the correct order
	str[slen] = '\0';
	strrev(str);

	// Fraction part multiplication
	int i;
	*lzc = 0;
	for (i = 0; i<50 && flen>0; i++)		// max length: log10(2^(LF * 32bits)) = 48.xxx -> roughly 50
	{ 
		int overflow = _F_mul(D, 10);
		str[slen++] = '0' + overflow;
		if (overflow != 0 || lzv)  {
			flen--; lzv = 1;				// non zero digit sets the leading zero valid
		}
		else
			(*lzc)++;
	}

	str[slen] = '\0';
	return slen;
}

//------------------------------------------------------------------------------
// float string round
//  : rounding a float string in LSB
// args:	str,	a float string
//			len,	string length returned, could be +1
// return:	str,	the string rounded, could be +1
_NOINS char* enx_frnd(char* str, int* len)
{
	int i, up = (str[*len - 1] >= '5');
	for (i = *len - 2; i >= 0; i--) {
		if (up == 0) break;
		else {	// rounding up
			int d = str[i] - '0' + 1;
			char s = '0' + d;
			up = (d >= 10);
			if (up) s -= 10;
			str[i] = s;
		}
	}
	
	if (up) {
		*(--str) = '1';
		(*len)++;
	}
	return str;
}


//------------------------------------------------------------------------------
// float to ascii float
//	: convert a float to ascii string with "%f"
// args:	f,			a float 
//			str,		a string buffer
//			width,		initial string length including decimal point
//			fill_val,	a fill value, ' ' or '0'
//			flen,		fraction length
//			xftz,		a flag to remove fraction trailing zeros
// returns:	len,		string length
_NOINS int ftoaf(float32 f, char* str, int width, char fill_val, int flen, bool xftz, int flags)
{
	char* qstr = str + width + flen + 1;		// temp string buffer: +1 for a rounding digit
	int lzc;
	int slen = enx_fcvt(f, qstr, flen+1, 1, &lzc);	// +1 for a rounding digit
	int qlen = slen - (flen + 1);
	char* fstr = qstr + qlen;
	bool lalign = (flags & LEFT) ? 1 : 0;
	bool plus = (flags & PLUS) ? 1 : 0;

	// NaN/INF
	int sgn = _is_neg(f);
	if (slen < 0) {
		str[0] = sgn ? '-' : '+';
		strncpy(str + 1, qstr, -slen);
		return 1 - slen;				// return orignial length
	}

	// Rounding
	qstr = enx_frnd(qstr, &slen);			// adjust qstr and qlen according to the rounding result
	qlen = (int)(fstr - qstr);			

	int qleni = width - flen;			// initial integer part length includeing a sign and the decimal digit
	if (flen > 0) qleni--;				// -1 for the decimal point '.' when f exists

	int si = 0, qi = 0, fi = 0;
	int qrlen = qleni - qlen - sgn;		// Q rest length 

	// If a fill value and sgn exist, add a sign bit first: "-0000Q"
	if (fill_val != ' ' && sgn)			str[si++] = '-';
	else if (fill_val != ' ' && plus)	str[si++] = '+';

	// Fill zeros @ integer part & !lalign
	if (!lalign) {
		while (qrlen-- > 0) str[si++] = fill_val;
	}

	// If no fill value and sgn exists, add a sign bit: "    -Q"
	if (fill_val == ' ' && sgn)			str[si++] = '-';
	else if (fill_val == ' ' && plus)	str[si++] = '+';

	// Copy integer part
	while (qlen-- > 0) str[si++] = qstr[qi++];

	// Add the sign and the decimal point.
	if(flen>0) str[si++] = '.';

	// Copy fraction part
	int flen1 = flen;
	while (flen1-- > 0) str[si++] = fstr[fi++];

	// Remove the trailing 0s in fraction: xxx.000
	if (xftz == 1 && flen>0) {
		while (flen-- >= 0) {
			if (str[si - 1] > '0') break;
			str[si - 1] = '\0';	// remove '0', '.'	
			si--;
		}
	}

	// Fill spaces @ fraction part & lalign
	if (lalign) {
		while (qrlen-- > 0) str[si++] = ' ';
	}

	// Fill zeros @ fraction part
	str[si] = '\0';
	return si;
}

//------------------------------------------------------------------------------
// float to ascii float
//	: convert a float to ascii string with "%e"
// args:	f,			a float 
//			str,		a string buffer
//			width,		initial string length including decimal point
//			fill_val,	a fill value, ' ' or '0'
//			flen,		fraction length
//			expo,		exponent value returned
//			xftz,		a flag to remove fraction trailing zeros
// returns:	len,		string length
_NOINS int ftoae(float32 f, char* str, int width, int flen, int* expo, bool xftz)
{
	char* qstr = str + width + 1;		// temp string buffer: +1 for a rounding digit
	int lzc;
	int slen = enx_fcvt(f, qstr, flen+2, 0, &lzc);	// +1 for a rounding digit, +1 for one q in f.ffffr in case Q=0
	int qlen = slen - (flen+2+lzc);

	// NaN/INF
	int sgn = _is_neg(f);
	if (slen < 0) {
		str[0] = sgn ? '-' : '+';
		strncpy(str + 1, qstr, slen);
		return 1 - slen;				// return orignial length
	}

	// Rounding
	int dp;								// Decimal point position
	char* qstr1 = qstr;					// Q position
	while(1) {
		// Find 1st non-zero digit		// could be changed during rounding op.
		for (dp = 0; dp < slen; dp++)
			if (qstr1[dp]>'0') break;

		// Rounding
		int len = 1 + flen + 1;			// q.qqfffr format: +1 for q and +1 for a rounding digit
		char* qstrn = enx_frnd(qstr1 + dp, &len);
		if (qstrn == (qstr1 + dp)) break;
		if(qstrn<qstr1) qstr1 = qstrn;
	};

	qlen += (qstr-qstr1);				// adjust qlen
	qstr = qstr1 + dp;					// adjust qstr

	int exp = qlen - dp - 1;

	int si = 0, qi = 0;

	// If sgn exist, add a sign bit first: "-q"
	if (sgn) str[si++] = '-';

	// Copy an integer part
	str[si++] = qstr[qi++];

	// Add the sign and the decimal point.
	if (flen>0) str[si++] = '.';

	// Copy the rest part: integer + fraction
	int fi = flen;
	while (fi-- > 0) str[si++] = qstr[qi++];

	// Remove the trailing 0s in fraction: xxx.000
	if (xftz==1 && flen>0) {
		while (flen-- >= 0) {
			if (str[si - 1] > '0') break;
			str[si - 1] = '\0';	// remove '0', '.'	
			si--;
		}
	}

	// Add Exponent part
	*expo = exp;
	str[si++] = 'e';
	if (exp < 0) {
		str[si++] = '-';
		exp = -exp;
	}
	else
		str[si++] = '+';

	uitoan(exp, str + si, 10, 3, '0');
	si += 3;

	str[si] = '\0';
	return si;
}

//------------------------------------------------------------------------------
// float to ascii float
//	: convert a float to ascii string with "%g"
// args:	f,			a float 
//			str,		a string buffer
//			width,		initial string length including decimal point
// returns:	len,		string length
_NOINS int ftoag(float32 f, char* str, int width)
{
	// Convert to a format "%e"
	int eqlen = 1;
	int eflen = width - 1;	// fraction length
	int sgn = _is_neg(f);
	int elen = sgn + eqlen + (eflen > 0 ? 1 : 0) + eflen + 5;	// expected

	int expo;
	int elen1 = ftoae(f, str, 1 + 1 + eflen, eflen, &expo, 1);
	if (elen1 < elen) {
		char dp = sgn ? str[2] : str[1];
		int ftz = elen - elen1 - (dp=='.' ? 0 : 1);		// fraction trailing 0s: except -1 for decimal point
		eflen -= ftz;									// adjust eflen
	}

	// Convert to a format "%f"
	int fqlen = 1;
	if (expo > 0) fqlen += expo;
	int fflen = eflen;
	if (fflen >= expo) fflen -= expo;
	//int flen = sgn + fqlen + (fflen > 0 ? 1 : 0) + fflen;		// expected

	char* fstr = str + elen1 + 1;
	int flen1 = ftoaf(f, fstr, fqlen + fflen, ' ', fflen, 1, 0);

	// Check the best format
	if (fqlen <= width && flen1 < elen1) {
		strncpy(str, fstr, flen1+1);
		return flen1;
	}

	return elen1;
}

_NOINS int ftox(float32 f, char* str)
{
	char* wstr = str;
	int sgn = f&0x80000000;
	int exp = (f>>23)&0xFF;
	int frac = f&0x7FFFFF;
	int frac_msb = frac&0x800000;
	
    *wstr++ = sgn ? '-' : '+';
    int len = uitoan(exp, wstr, 16, 2, '0');	
    wstr += len;
	*wstr++ = '.';								
	
    len = uitoan(frac, wstr, 16, 6, '0');		
    wstr += len;
	*wstr = '\0';

	// special numbers
    if(exp==0 && frac) 	{ 
    		*wstr++ = '('; *wstr++ = 'D'; *wstr++ = 'N'; *wstr++ = ')'; 
    }
    if(exp==0xff) {
    	if(!frac) { 
    		*wstr++ = '('; *wstr++ = 'I'; *wstr++ = 'N'; *wstr++ = 'F'; *wstr++ = ')';
		} else if(frac_msb) {
    		*wstr++ = '('; *wstr++ = 'q'; *wstr++ = 'N'; *wstr++ = 'a'; *wstr++ = 'N'; *wstr++ = ')';
    	} else {
    		*wstr++ = '('; *wstr++ = 's'; *wstr++ = 'N'; *wstr++ = 'a'; *wstr++ = 'N'; *wstr++ = ')';
    	}
    }
		
	return (wstr-str);	// sign + exp + '.' + frac
}
