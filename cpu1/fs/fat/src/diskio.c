/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "dev.h"
#include "dev_inline.h"

#include <stdio.h>
#include <time.h>

#include "diskio.h"
#include "file.h"
#include "msg.h"

#include "timer_task.h"

#ifdef __FLS_FAT_LOAD__
/*FLS parameter */
#define ERASE_BLOCK_SIZE FAT_BLOCK_FLS 
#define NO_OF_SERCTORS ((SFLS_SIZE-FLS_OFFSET-SFLS_FIXED_SIZE)/ERASE_BLOCK_SIZE)

/**
        @ buff   : Data buffer to store read data 
        @ Sector : address (LBA)
        @ count  : Number of sectors to read (1..255)
*/
inline DRESULT fls_read_sector(BYTE *buff, DWORD sector, BYTE count)
{
	UINT addr = SFLS_BASE + FLS_OFFSET + sector * ERASE_BLOCK_SIZE;
#ifdef EN25QH256
	UINT FlsAddr = addr - SFLS_BASE;
	if(FlsAddr >= 0x1000000)	FlsEnHBL();
#endif
    FlsRead(buff, addr, count*ERASE_BLOCK_SIZE);
#ifdef EN25QH256
	if(FlsAddr >= 0x1000000)	FlsExHBL();
#endif
    return	RES_OK;
}

/**
        @ buff   : Data buffer to store data 
        @ Sector : address (LBA)
        @ count  : Number of sectors to read (1..255)
*/
inline DRESULT fls_write_sector(const BYTE *buff,	 DWORD sector, BYTE count)
{
    int i = 0;
    UINT addr = SFLS_BASE + FLS_OFFSET + sector * ERASE_BLOCK_SIZE;
	UINT offset = addr;
#ifdef EN25QH256
	UINT FlsAddr;
#endif
    for(i=1; i<=count; i++)
    {
#ifdef EN25QH256
		FlsAddr = offset - SFLS_BASE;
		if(FlsAddr >= 0x1000000)	FlsEnHBL();
#endif
        FlsErase(offset);
        offset += ERASE_BLOCK_SIZE;
#ifdef EN25QH256
		if(FlsAddr >= 0x1000000)	FlsExHBL();
#endif
    }

    offset = addr;
#ifdef EN25QH256
	FlsAddr = offset - SFLS_BASE;
	if(FlsAddr >= 0x1000000)		FlsEnHBL();
#endif
    FlsWrite(offset, buff, count * ERASE_BLOCK_SIZE);
#ifdef EN25QH256
	if(FlsAddr >= 0x1000000)		FlsExHBL();
#endif
    return RES_OK;
}
#endif

#ifdef __MEM_FAT_LOAD__
/*MEM parameter */
#define MEM_BLOCK_SIZE	(4096)
#define MEM_SERCTORS	(MEMDISK_SIZE/MEM_BLOCK_SIZE)
inline DRESULT memdisk_init(void)
{
	static int first = 1;
	if (first == 1) {
		DmaMemSet_ip((BYTE *)MEMDISK_BASE, 0, MEMDISK_SIZE);
		first = 0;
		printf("MEM_INIT\r\n");
	}
	return RES_OK;
}

inline DRESULT memdisk_read(BYTE *buff, DWORD sector, BYTE count)
{
	UINT addr = MEMDISK_BASE + (sector * MEM_BLOCK_SIZE);
	DmaMemCpy_ip(buff, (BYTE *)addr, count * MEM_BLOCK_SIZE);
	invalidate_dcache_range((uint)buff, (uint)(buff+(count * MEM_BLOCK_SIZE)));
	return RES_OK;
}

inline DRESULT memdisk_write(const BYTE *buff, DWORD sector, BYTE count)
{
	UINT addr = MEMDISK_BASE + (sector * MEM_BLOCK_SIZE);
	DmaMemCpy_ip((BYTE *)addr, (BYTE *)buff, count * MEM_BLOCK_SIZE);
	return RES_OK;
}
#endif

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
//	int result;

	switch(pdrv) {
#ifdef __FLS_FAT_LOAD__
		case FLS:
			return 0;
#endif
#ifdef __DEVICE_SD__
		case MMC:
			if(Sdio0GetActive())
				return 0;
#endif
#ifdef __MEM_FAT_LOAD__
		case MEM:
			return 0;
#endif
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
//	int result;

	switch(pdrv) {
#ifdef __FLS_FAT_LOAD__
		case FLS:
			return 0;
#endif
#ifdef __DEVICE_SD__
		case MMC:
			if(Sdio0GetActive())
				return 0;
#endif
#ifdef __MEM_FAT_LOAD__
		case MEM:
			memdisk_init();
			return 0;
#endif
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DRESULT res;
//	int result;

	if(count == 0)
		return RES_PARERR;

#ifdef GPIO_DISKIO_LED
	GpioSetHi(GPIO_DISKIO_LED);
#endif

	switch(pdrv) {
#ifdef __FLS_FAT_LOAD__
		case FLS:
			res = fls_read_sector(buff, sector, count);
			break;
#endif
#ifdef __DEVICE_SD__
		case MMC:
			if(count == 1)
			{
				if(Sdio0SectorR((BYTE *)buff, (UINT)sector) == DEF_FAIL)
				{
					res = RES_ERROR;
				}
				else
				{
					res = RES_OK;
				}
			}
			else
			{
				if(Sdio0R((BYTE *)buff, (UINT)sector, (UINT)count) == DEF_FAIL)
				{
					res = RES_ERROR;		// Ng
				}
				else
				{
					res = RES_OK;			// Ok
				}
			}
			break;
#endif
#ifdef __MEM_FAT_LOAD__
		case MEM:
			res = memdisk_read(buff, sector, count);
			break;
#endif
		default:
			res = RES_PARERR;
			break;
	}

#ifdef GPIO_DISKIO_LED
	GpioSetLo(GPIO_DISKIO_LED);
#endif

	return res;
	UNUSED(buff);
	UNUSED(sector);
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DRESULT res;
//	int result;

	if(count == 0)
		return RES_PARERR;		// Parameter error

#ifdef GPIO_DISKIO_LED
	GpioSetHi(GPIO_DISKIO_LED);
#endif

	switch(pdrv) {
#ifdef __FLS_FAT_LOAD__
		case FLS:
			res = fls_write_sector(buff, sector, count);
			break;
#endif
#ifdef __DEVICE_SD__
		case MMC:
			if(count == 1)
			{
				if(Sdio0SectorW((BYTE *)buff, (UINT)sector) == DEF_FAIL)
				{
					res = RES_ERROR;
				}
				else
				{
					res = RES_OK;
				}
			}
			else
			{
				if(Sdio0W((BYTE*)buff, (UINT)sector, (UINT)count) == DEF_FAIL)
				{
					res = RES_ERROR;		// Ng
				}
				else
				{
					res = RES_OK;			// Ok
				}
			}
			break;
#endif
#ifdef __MEM_FAT_LOAD__
		case MEM:
			res = memdisk_write(buff, sector, count);
			break;
#endif
		default:
			res = RES_PARERR;
			break;
	}

#ifdef GPIO_DISKIO_LED
	GpioSetLo(GPIO_DISKIO_LED);
#endif

	return res;
	UNUSED(buff);
	UNUSED(sector);
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
//	DRESULT res;
//	int result;

	WORD	*pW  = (WORD  *)buff;
	DWORD	*pDw = (DWORD *)buff;

	switch(pdrv) {
#ifdef __FLS_FAT_LOAD__
		case FLS:
			switch(cmd)
			{
				case CTRL_SYNC :
					return RES_OK;
				case GET_SECTOR_COUNT :
					*pDw = NO_OF_SERCTORS;
					return RES_OK;
				case GET_SECTOR_SIZE :
					*pW = (WORD)ERASE_BLOCK_SIZE;
					return RES_OK;
				case GET_BLOCK_SIZE :
					*pDw = (DWORD)1;
					return RES_OK;
				case CTRL_TRIM :
					return RES_OK;
			}
			break;
#endif
#ifdef __DEVICE_SD__
		case MMC:
			switch(cmd)
			{
				case CTRL_SYNC :
					return RES_OK;
				case GET_SECTOR_COUNT :
					*pDw = (DWORD)Sdio0GetSectorCnt();
					return RES_OK;
				case GET_SECTOR_SIZE :
					*pW = (WORD)512;
					return RES_OK;
				case GET_BLOCK_SIZE :
					*pDw = (DWORD)(Sdio0GetAUSize()+1);
					return RES_OK;
				case CTRL_TRIM :
					Sdio0E((UINT)pDw[0], (UINT)pDw[1]);
					return RES_OK;
			}
			break;
#endif
#ifdef __MEM_FAT_LOAD__
		case MEM:
			switch(cmd)
			{
				case CTRL_SYNC :
					return RES_OK;
				case GET_SECTOR_COUNT :
					*pDw = (DWORD)MEM_SERCTORS;
					return RES_OK;
				case GET_SECTOR_SIZE :
					*pW = (WORD)MEM_BLOCK_SIZE;
					return RES_OK;
				case GET_BLOCK_SIZE :
					*pDw = (DWORD)1;
					return RES_OK;
				case CTRL_TRIM :
					return RES_OK;
			}
			break;
#endif
	}
	return RES_OK;
	UNUSED(pDw);
	UNUSED(pW);
	UNUSED(cmd);
}
#endif

/*-----------------------------------------------------------------------*/
/* Time Functions                                                        */
//	[31:25] : Year(0-127 org.1980)
//	[24:21] : Month(1-12)
//	[20:16] : Day(1-31)
//	[15:11] : Hour(0-23)
//	[10: 5] : Minute(0-59)
//	[ 4: 0] : Second(0-29 *2)
DWORD get_fattime(void)
{
	struct tm tmout;
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	return    ((DWORD)(tmout.tm_year - 80)	<< 25)		// (tTime->tm_year + 1900 - 1980)
			| ((DWORD)(tmout.tm_mon + 1)	<< 21)
			| ((DWORD)(tmout.tm_mday)		<< 16)
			| ((DWORD)(tmout.tm_hour)		<< 11)
			| ((DWORD)(tmout.tm_min)		<<  5)
			| ((DWORD)(tmout.tm_sec / 2)	<<  0);
}
