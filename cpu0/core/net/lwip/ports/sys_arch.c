/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

#include "dev.h"
#include "dev_inline.h"

#include "arch/cc.h"

#include "main.h"

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
//
//
#define SW_CHKSUM
#ifdef SW_CHKSUM
#define FOLD_U32T(u)          (((u) >> 16) + ((u) & 0x0000ffffUL))
#define SWAP_BYTES_IN_WORD(w) (((w) & 0xff) << 8) | (((w) & 0xff00) >> 8)
ISRT static u16_t lwip_standard_chksum(void *dataptr, int len)
{
  u8_t *pb = (u8_t *)dataptr;
  u16_t *ps, t = 0;
  u32_t sum = 0;
  int odd = ((mem_ptr_t)pb & 1);

  /* Get aligned to u16_t */
  if (odd && len > 0) {
    ((u8_t *)&t)[1] = *pb++;
    len--;
  }

  /* Add the bulk of the data */
  ps = (u16_t *)(void *)pb;
  while (len > 1) {
    sum += *ps++;
    len -= 2;
  }

  /* Consume left-over byte, if any */
  if (len > 0) {
    ((u8_t *)&t)[0] = *(u8_t *)ps;
  }

  /* Add end bytes */
  sum += t;

  /* Fold 32-bit sum to 16 bits
     calling this twice is propably faster than if statements... */
  sum = FOLD_U32T(sum);
  sum = FOLD_U32T(sum);

  /* Swap if alignment was odd */
  if (odd) {
    sum = SWAP_BYTES_IN_WORD(sum);
  }

  return (u16_t)sum;
}
#endif

u16_t my_chksum(void *dataptr, u16_t len)
{
	u16_t accdma, accdma2;
	DmaDChkSum_ip((BYTE *)dataptr, (UINT)len, &accdma);
	DmaDChkSum_ip((BYTE *)dataptr, (UINT)len, &accdma2);
#ifdef SW_CHKSUM
	if (accdma != accdma2) {
		invalidate_dcache_range((uint)dataptr, (uint)dataptr+len);
		u16_t accdma3 = lwip_standard_chksum(dataptr, len);
		printf("%s[P:0x%08X]DCK(0x%04X)DCK(0x%04X)SCK(0x%04X)len(%4d)CPU%d\r\n", sltLocalTime.data, (UINT)dataptr, accdma, accdma2, accdma3, len, CpuIdGet());
		return accdma3;
	}
#endif
	return accdma2;
}

void my_copy(void* dst, void* src, unsigned long len)
{
#if 1
#if 1//def CONFIG_DCACHE_ENABLE
    DmaMemCpy_ip((BYTE*)dst, (BYTE*)src, (UINT)len);
    invalidate_dcache_range((uint)dst, ((uint)dst)+len);  /* Maybe, we thick cache flush may bes not used. cache is write through */
#else
    DmaMemCpy_ip((BYTE*)dst, (BYTE*)src, (UINT)len);
#endif
#else
    memcpy(dst,src,len);
#endif
}