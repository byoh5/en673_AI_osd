//*************************************************************************************************
// File system (FatFs)
//*************************************************************************************************
#include "dev.h"
#include "dev_inline.h"

#include "file.h"
#include "ff.h"
#include "user.h"

#include <stdlib.h>	// for atoi

#if _MULTI_PARTITION ///* .Caution defined in ffconf.h*/
PARTITION VolToPart[] = 
{
	{0,0}, /* serial flash, Logical drive 0 ==> Physical drive 0, auto detection */
	{1,1}  /* SD Card     , Logical drive 1 ==> Physical drive 1, 1st partition */
}; 
#endif

FATFS gtFatFs[_VOLUMES];	// File system object

//*************************************************************************************************
// File system
//-------------------------------------------------------------------------------------------------
const char *put_rc(FRESULT rc)
{
	static const char *str[] = {
		"Succeeded\0",															// 0
		"A hard error occurred in the low level disk I/O layer\0",				// 1
		"Assertion failed\0",													// 2
		"The physical drive cannot work\0",										// 3
		"Could not find the file\0",											// 4
		"Could not find the path\0",											// 5
		"The path name format is invalid\0",									// 6
		"Access denied due to prohibited access or directory full\0",			// 7
		"Access denied due to prohibited access\0",								// 8
		"The file/directory object is invalid\0",								// 9
		"The physical drive is write protected\0",								// 10
		"The logical drive number is invalid\0",								// 11
		"The volume has no work area\0",										// 12
		"There is no valid FAT volume\0",										// 13
		"The f_mkfs() aborted due to any parameter error\0",					// 14
		"Could not get a grant to access the volume within defined period\0",	// 15
		"The operation is rejected according to the file sharing policy\0",		// 16
		"LFN working buffer could not be allocated\0",							// 17
		"Number of open files > _FS_SHARE\0",									// 18
		"Given parameter is invalid\0",											// 19
		"Error Message Error\0"													// res error
		};
	return str[min(rc,FR_INVALID_PARAMETER)];
}

FRESULT drive_init(DriveNum dnNum)
{
	FRESULT res = FR_OK;
	char strPath[16] = {0};

	switch(dnNum)
	{
		case FLS:
			sprintf(strPath, "%d:/%s", FLS, FLS_DIR_WEB_PATH);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", FLS, FLS_DIR_LOG_PATH);
			res = fat_mkdir(dnNum, strPath);
			break;
#ifdef __DEVICE_SD__
		case MMC:
			sprintf(strPath, "%d:/%s", MMC, MMC_DIR_NOR);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", MMC, MMC_DIR_EVT);
			res = fat_mkdir(dnNum, strPath);
			sprintf(strPath, "%d:/%s", MMC, MMC_DIR_SNAP);
			res = fat_mkdir(dnNum, strPath);
			break;
#endif

#ifdef __MEM_FAT_LOAD__
		case MEM:
			sprintf(strPath, "%d:/%s", MEM, "TEST");
			res = fat_mkdir(dnNum, strPath);
#endif
		default:
			break;
	}

	return res;
}

FRESULT drive_in(DriveNum dnNum)
{
	FRESULT res;
	DIR dir;
	TCHAR buf[25];
	sprintf(buf, "%d:", dnNum);
	res = f_mount(&gtFatFs[dnNum], (const TCHAR*)buf, 1);
	if(res == FR_OK)
	{
		res = f_opendir(&dir, buf);
		printf("%d:/ mount res : %s\r\n", dnNum, put_rc(res));
		if(res == FR_OK)
		{
			f_closedir(&dir);
		}
		else
		{
			printf("%d:/ dir check fail : %s\r\n", dnNum, put_rc(res));
			drive_out(dnNum);
		}
		return res;
	}
	else
	{
		printf("%d:/ mount fail : %s\r\n", dnNum, put_rc(res));
		return res;
	}
}

FRESULT drive_out(DriveNum dnNum)
{
	FRESULT res;
	TCHAR buf[25];
	sprintf(buf, "%d:", dnNum);
	res = f_mount(NULL, (const TCHAR *)buf, 1);
	printf("%d:/ unmount res : %s\r\n", dnNum, put_rc(res));
	return res;
}

//*************************************************************************************************
//
//-------------------------------------------------------------------------------------------------
FRESULT fat_mkdir(DriveNum dnNum, char *strPath)
{
	DIR dir;
	FRESULT res = f_opendir(&dir, strPath);
	switch(res)
	{
		case FR_OK:	// dir Already
			f_closedir(&dir);
			break;
		case FR_NO_FILESYSTEM:
			printf("No File System\r\n");
			if(dnNum == FLS)
			{
				char strDrive[4] = {0};
				sprintf(strDrive, "%d:", dnNum);
				if((res=f_mkfs(strDrive, 1, FAT_BLOCK_FLS))!=FR_OK)
					break;
			}
#ifdef __DEVICE_SD__
			else if(dnNum == MMC)
			{
				if((res=fat_sdmkfs(MMC, FAT_BLOCK_MMC))!=FR_OK)
				{
					printf("Error: %s\r\n", put_rc(res));
					Sdio0ClockDiv(eSDIO_CLK_25MHz);
					if((res=fat_sdmkfs(MMC, FAT_BLOCK_MMC))!=FR_OK)
						break;
				}
			}
#endif
#ifdef __MEM_FAT_LOAD__
			else if(dnNum == MEM)
			{
				char strDrive[4] = {0};
				sprintf(strDrive, "%d:", dnNum);
				printf("MEMDISK_FORMAT: (%s)\r\n", strDrive);
				if((res=f_mkfs(strDrive, 1, FAT_BLOCK_MEM))!=FR_OK) {
					printf("MEMDISK_FORMAT: fail(%s)\r\n", put_rc(res));
					break;
				}
			}
#endif
		case FR_NO_PATH:
			if((res=f_mkdir(strPath)) == FR_OK)
			{
				printf("Make %s : ", strPath);
			}
		default:
			printf("%s\r\n", put_rc(res));
			break;
	}
	return res;
}

BYTE fat_get_freesize(DriveNum dnNum)
{
	FRESULT res;
	TCHAR buf[4] = {0};
	FATFS *fs;
	DWORD fre_clust, tot_sect;
	BYTE bFreesize;

	sprintf(buf, "%d:", dnNum);
	if((res=f_getfree((const TCHAR *)buf, &fre_clust, &fs)) != FR_OK)
	{
		printf("Error(%s:%d) : %s\r\n", __func__, __LINE__, put_rc(res));
		return -1;
	}
	tot_sect = fs->n_fatent - 2;	//  * fs->csize
	bFreesize = (BYTE)((fre_clust*100.0)/tot_sect);

#ifdef ENX_DEBUG
	unsigned int nVer;
	int nPow = 0;
	nVer = fs->ssize * fs->csize;
	for(nPow=0;nPow<10;nPow++)
	{
		if((nVer >> nPow) == 1024)
			break;
	}
	if(nPow == 10)	printf("Err : Not case\r\n");
	printf("Info : Disk Size (free size is %uKB, total size %uKB) ", (fre_clust << nPow), (tot_sect << nPow));
#endif

	printf("%d:/ Free %d%%\r\n", dnNum, bFreesize);
	return bFreesize;
}

FRESULT fat_ls(char *position)
{
	FRESULT res;
	TCHAR strFileName[_MAX_LFN+1] = {0};
	FILINFO finfo;
	DIR dir;
	UINT s1, s2;
	DWORD p1;
    p1 = s1 = s2 = 0;
	finfo.lfname = strFileName;
	finfo.lfsize = _MAX_LFN;

    res = f_opendir(&dir, position);
	if (res == FR_OK) {
	    for(;;)
		{
	    	res = f_readdir(&dir, &finfo);
	    	if((res != FR_OK) || !finfo.fname[0])
			{
				break;
			}
	    	if(finfo.fattrib & AM_DIR)
			{
	    		s2++;
	    	}
			else
	    	{
	    		s1++;
				p1 += finfo.fsize;
	    	}
	    	printf("\r\n%c%c%c%c%c %u/%02u/%02u %02u:%02u:%02u %9d  ", 
	    				(finfo.fattrib & AM_DIR) ? 'D' : '-',
	    				(finfo.fattrib & AM_RDO) ? 'R' : '-',
	    				(finfo.fattrib & AM_HID) ? 'H' : '-',
	    				(finfo.fattrib & AM_SYS) ? 'S' : '-',
	    				(finfo.fattrib & AM_ARC) ? 'A' : '-',
	    				(finfo.fdate >> 9) + 1980, 
	    				(finfo.fdate >> 5) & 15, 
	    				(finfo.fdate & 31),
	    				(finfo.ftime >> 11), 
	    				(finfo.ftime >> 5) & 63,
	    				(finfo.ftime & 31) * 2,
	    				(finfo.fsize));
			if(dir.lfn_idx == 0xFFFF)
			{
				printf("[%s]", finfo.fname);
			}
			else
			{
				printf("[%s]", finfo.lfname);
			}
		}
		f_closedir(&dir);
	}
	printf("\r\n%6d File(s), %u bytes total\r\n%6d Dir(s)\r\n", s1, p1, s2);
	if(position[0] == '0')
		fat_get_freesize(FLS);
	else if(position[0] == '1')
		fat_get_freesize(MMC);
#ifdef __MEM_FAT_LOAD__
	else if(position[0] == '2')
		fat_get_freesize(MEM);
#endif
	return res;
}

#ifdef __DEVICE_SD__
/**
	format sd card.
	we think that drive name is always '1:'
	buf if you change drvie name of SD. you should modify 'VolToPart'
	
	@param  dnNdum
			: should be '1' 
	@param au
			: sector*n
			for sd card, sector size should be 512,
			n, 1, 2, .. 128.

*/
FRESULT fat_sdmkfs(DriveNum dnNum, int au)
{
	FRESULT res;
	char drive[4] = {0};
#if _MULTI_PARTITION
	BYTE work[_MAX_SS] = {0};
	DWORD plist[] = {100, 0, 0, 0};  /* Divide drive into two partitions */ 
	res = f_fdisk(dnNum, plist, work);
	printf("f_fdisk: %s\r\n", put_rc(res));
#endif

	sprintf(drive, "%d:", dnNum);

	res = f_mount(NULL, drive, 0);				// 1. unmount
	res = f_mount(&gtFatFs[dnNum], drive, 1);	// 2. mount

	res = f_mkfs(drive, 0, au);					// 3. format
	printf("f_mkfs: %s\r\n",put_rc(res));

	res = f_mount(NULL, drive, 0);				// 4. recheck, unmount
	res = f_mount(&gtFatFs[dnNum], drive, 1);	// 5. recheck, mount => state!
	printf("mount %s\r\n",put_rc(res));

	return res;
}
#endif
