/***************************************************************************
 File Name	: 	util.c
 Description:	EN673 - AES128 Encrytor
 Designer	:	YoungKyu, Jang
 Date		:	15. 6. 16 
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
****************************************************************************/
#include "dev.h"
#include "dev_inline.h"

//*************************************************************************************************
// Sorting
//-------------------------------------------------------------------------------------------------
// Quick sort
static void q_sort(int numbers[], int left, int right)
{
	int		pivot, l_hold, r_hold;

	l_hold = left;
	r_hold = right;
	pivot  = numbers[left];

	while(left < right){
		while((numbers[right] >= pivot) && (left < right))		right --;

		if(left != right)		numbers[left] = numbers[right];

		while((numbers[left] <= pivot) && (left < right))		left ++;

		if(left != right){
			numbers[right] = numbers[left];
			right --;
		}
	}

	numbers[left] = pivot;
	pivot = left;
	left  = l_hold;
	right = r_hold;

	if (left  < pivot)		q_sort(numbers, left, pivot - 1);
	if (right > pivot)		q_sort(numbers, pivot+1, right);
}

void QuickSort(int numbers[], int array_size)
{
	q_sort(numbers, 0, array_size -1);
}

//-------------------------------------------------------------------------------------------------
//
BYTE hex2ascii(BYTE hex)
{
	if(hex<=0x09)						return hex + '0';			// 0 ~ 9
	else if((hex>=0x0a)&&(hex<=0x0f))	return hex - 0x0a + 'A';	// 10 ~ 15
	return 'x';
}

void hexDump(char *desc, void *addr, int len)
{
	int i, pos = 0;
	char strbuff[80] = {0};
	unsigned char buff[17];
	unsigned char *pc = (unsigned char*)addr;

	// Output description if given.
	if (desc != NULL)
		printf ("%s:\r\n", desc);

	// Process every byte in the data.
	for (i = 0; i < len; i++) {
		// Multiple of 16 means new line (with line offset).

		if ((i % 16) == 0) {
			// Just don't print ASCII for the zeroth line.
			if (i != 0)
			{
				strbuff[pos++] = ' ';
				strbuff[pos++] = ' ';
				strncpy(&strbuff[pos], (char *)buff, 16);
				printf ("%s\r\n", strbuff);
				pos = 0;
			}

			// Output the offset.
			sprintf(&strbuff[pos], "  %04x ", i);
			pos += 7;
		}

		// Now the hex code for the specific character.
		sprintf(&strbuff[pos], " %02x", pc[i]);
		pos += 3;

		// And store a printable ASCII character for later.
		if ((pc[i] < 0x20) || (pc[i] > 0x7e))
			buff[i % 16] = '.';
		else
			buff[i % 16] = pc[i];
		buff[(i % 16) + 1] = '\0';
	}

	// Pad out last line if not exactly 16 characters.
	while ((i % 16) != 0) {
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		strbuff[pos++] = ' ';
		i++;
	}

	// And print the final ASCII bit.
	strbuff[pos++] = ' ';
	strbuff[pos++] = ' ';
	strncpy(&strbuff[pos], (char *)buff, 16);
	pos += 16;
	printf ("%s\r\n", strbuff);
}

void AesInit(void)
{
	UTIL_AESKEY0 = 0x12345678;
	UTIL_AESKEY1 = 0x12345678;
	UTIL_AESKEY2 = 0x12345678;
	UTIL_AESKEY3 = 0x12345678;
}

void AesEncryt(BYTE * Dst, BYTE * Src, UINT BkCnt)
{
	UTIL_MODE = 0;
	DmaMemCpy_isr((void*) UTIL_BASE, (void*) Src, (BkCnt<<4));
	DmaMemCpy_isr((void*) Dst, (void*) UTIL_BASE, (BkCnt<<4));
}

WORD ChkSum(BYTE * Src, UINT Len)
{
	UTIL_MODE = 1;
	DmaMemCpy_isr((BYTE*) UTIL_BASE, (BYTE*) Src, Len);
	return UTIL_CHKSUM;
}

void EndianConv(BYTE * Dst, BYTE * Src, UINT Len, BYTE Size)
{
	UTIL_MODE = 2;
	UTIL_ENDI_SIZE = Size;
	DmaMemCpy_isr((void*)UTIL_BASE, (void*)Src, Len);
	DmaMemCpy_isr((void*)Dst, (void*)UTIL_BASE, Len);
}

void G711Enc(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law)
{
	UTIL_MODE = 3;
	UTIL_ENC_LAW = Law;
	DmaMemCpy_isr((BYTE*)UTIL_BASE, Src, Len);
	DmaMemCpy_isr(Dst, (BYTE*)UTIL_BASE, Len>>1);
}

void G711Dec(BYTE * Dst, BYTE * Src, UINT Len, BYTE Law)
{
	UINT i;
	UTIL_MODE = 4;
	UTIL_DEC_LAW = Law;
	for(i=0;i<Len;i+=8)
	{
		DmaMemCpy_isr((BYTE*)UTIL_BASE, Src+i, 8);
		if (Dst == (BYTE *)AUD_BASE)
			DmaMemCpy_isr((void*)Dst, (void*)UTIL_BASE, 16);
		else
			DmaMemCpy_isr(Dst+(i*2), (BYTE*)UTIL_BASE, 16);
	}
}

void PrintHex(BYTE * Dat, UINT Len)
{
	UINT i;
	Uart0_Tx(0x0d);Uart0_Tx(0x0a);
	Uart0_Str("Length : ");
	Uart0_Hex(Len);
	Uart0_Tx(0x0d);Uart0_Tx(0x0a);
	Uart0_Str("            ");
	for(i=0;i<0x10;i++)
	{
		Uart0_Num(i&0xf);
		Uart0_Str("  ");
	}
	Uart0_Tx(0x0d);Uart0_Tx(0x0a);
	for(i=0;i<Len;i++)
	{
		if((i%0x10)==0x00)
		{
			Uart0_Hex(((UINT) Dat)&0xfffffff0);
			Uart0_Str("    ");
		}
		Uart0_Byte(*Dat);
		Dat++;
		
		if((i%0x10)==0x0f)
		{
			Uart0_Tx(0x0d);Uart0_Tx(0x0a);
		}
		else
		{
			Uart0_Str(" ");
		}
	}
	Uart0_Tx(0x0d);Uart0_Tx(0x0a);
}