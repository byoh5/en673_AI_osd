/* Copyright (c) 2013-2016 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#ifndef _FWUPDATE_H_
#define _FWUPDATE_H_

typedef enum
{
	eFWT_FLS = 1,
	eFWT_SD,
	eFWT_DDR
}FWUPDATE_TYPE;

typedef struct
{
	FWUPDATE_TYPE type;		// SD? FLS? DDR?
	char file_path[128];	// SD or FLS type
	BYTE* ddr_addr;			// DDR type
	UINT is_bootwrite;		// boot.bin write?
	UINT is_areachange;		// boot area change?
	UINT is_autoreboot;		// reboot?
	UINT is_malloc;			// malloc free?
	UINT is_encodingstop;	// H.264, MJPEG stop?
}FWUPDATE_INFO;

extern int fwupdate_filecheck(DriveNum dn, char *file_url);
extern int fwupdate_process(void);
extern void fwupdate_encoding_stop(void);
extern void fwupdate_encoding_start(void);
extern void fwupdate_Task(void *pvParameters);

#endif	// _FWUPDATE_H_