/******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		mem.ld
//	Description:
//	Author:			YoungKyu Jang
//					Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	140924		ykjang		first designed
//	0.1	150425		ygkim		fully revised
//	0.1	150801		ygkim		revised to be simple
// -----------------------------------------------------------------------------*/
MEMORY
{
	FLASH   : ORIGIN = 0x06000000,                              LENGTH = 0x10000
	FLAG    : ORIGIN = 0x06000000 + LENGTH(FLASH) - 0x10,       LENGTH = 0x10

	RAM     : ORIGIN = 0x04000000,                              LENGTH = 0x8000
	BT_RAM  : ORIGIN = 0x04000000 + LENGTH(RAM) - 0xB00,        LENGTH = 0xB00
	RAM1    : ORIGIN = 0x04000000,                              LENGTH = 0x8000 - LENGTH(BT_RAM) /* total 29KB */

	SH0RAM  : ORIGIN = 0xF0000000,                              LENGTH = 0x8000
	SH1RAM  : ORIGIN = 0xF0010000,                              LENGTH = 0x8000
}

/* stack and heap size */
stack_size  = 4*1024;
heap_size   = 256;

__ram_size  = ORIGIN(RAM1) + LENGTH(RAM1);

__stack_s   = __ram_size - 4;
__stack_e   = __ram_size - stack_size;

/*******************************************************************************
// Output sections
//----------------------------------------------------------------------------*/
SECTIONS
{
	.reset :
	{
		. = ALIGN(32);															
		__reset_s = .;
		*(.reset)
		*(.reset_dummy)
		. = ALIGN(32);
		__reset_e = .;
	} > RAM

	__reset_lma_s = LOADADDR(.reset);
	__reset_lma_e = ( (LOADADDR(.reset) + SIZEOF(.reset) + 31) & ~ (32-1) );

	.rodata : AT ( (LOADADDR (.reset) + SIZEOF(.reset) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__rodata_s = .;
		*(.rodata)				/* constants, strings */
		*(.rodata.*)			/* constants, strings */
		. = ALIGN(32);
		__rodata_e = .;
	} > RAM

	__rodata_lma_s = LOADADDR(.rodata);
	__rodata_lma_e = ( (LOADADDR(.rodata) + SIZEOF(.rodata) + 31) & ~ (32-1) );
	
	.text : AT ( (LOADADDR (.rodata) + SIZEOF(.rodata) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__text_s = .;
		*(.text)
		*(.text_dummy)
		. = ALIGN(32);
		__text_e = .;
	} > RAM

	__text_lma_s = LOADADDR(.text);
	__text_lma_e = ( (LOADADDR(.text) + SIZEOF(.text) + 31) & ~ (32-1) );

	.ispm_text : AT ( (LOADADDR (.text) + SIZEOF(.text) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__ispm_text_s = .;
		*(.ispmtext)
		*(.ispm_text_dummy)
		. = ALIGN(32);
		__ispm_text_e = .;
	} > RAM

	__ispm_text_lma_s = LOADADDR(.ispm_text);
	__ispm_text_lma_e = ( (LOADADDR(.ispm_text) + SIZEOF(.ispm_text) + 31) & ~ (32-1) );

	.ispm_data : AT ( (LOADADDR (.ispm_text) + SIZEOF(.ispm_text) + 31) & ~ (32-1))
	{
		. = ALIGN(32);
		__ispm_data_s = .;
		*(.ispmdata)
		. = ALIGN(32);
		__ispm_data_e = .;
	} > RAM

	__ispm_data_lma_s = LOADADDR(.ispm_data);
	__ispm_data_lma_e = ( (LOADADDR(.ispm_data) + SIZEOF(.ispm_data) + 31) & ~ (32-1) );

	.sh0sr_data (NOLOAD) : 
	{
		. = ALIGN(32);
		*(.sh0srdata)
		. = ALIGN(32);
	} > SH0RAM

	.sh1sr_data (NOLOAD) : 
	{
		. = ALIGN(32);
		*(.sh1srdata)
		. = ALIGN(32);
	} > SH1RAM

	.data : AT ( (LOADADDR(.ispm_data) + SIZEOF(.ispm_data) + 31) & ~ (32-1) )
	{
		. = ALIGN(32);
		__data_s = .;
		*(.data)
		*(.data.rel)
		*(.data.rel.local)
		*(.sdata)
		. = ALIGN(32);
		__data_e = .;
	} > RAM

	__data_lma_s = LOADADDR(.data);
	__data_lma_e = ( (LOADADDR(.data) + SIZEOF(.data) + 31) & ~ (32-1) );

	
	__lma_size = ( (LOADADDR(.data) + SIZEOF(.data) + 31) & ~ (32-1) ) - LOADADDR(.reset);
	__lma_start = 0;	/* reserved */

	.bss (NOLOAD) :
	{
		. = ALIGN(32);
		__bss_s = .;
		*(.bss) *(COMMON)
		. = ALIGN(32);
		__bss_e = .;
	} > RAM
}

