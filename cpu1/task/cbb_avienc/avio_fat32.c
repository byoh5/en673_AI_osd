#include "dev.h"
#include "board.h"
#include "dev_types.h"
#include "file.h"
#include "ff.h"
#include "muxer_avienc.h"
#include "avio_fat32.h"
#include "user.h"

#define ENX_AVIO_TEST_DEBUG
#ifdef ENX_AVIO_TEST_DEBUG
	int printf(const char *fmt, ...);
	#define aprintf(fmt, args...) do { printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#else
	#define aprintf(fmt, args...) do {} while(0);
#endif

/* Declarations of FatFs internal functions accessible from applications.
/  This is intended to be used for disk checking/fixing or dirty hacks :-) */
DWORD clust2sect (FATFS* fs, DWORD clst);
DWORD get_fat (FATFS* fs, DWORD clst);
FRESULT put_fat (FATFS* fs, DWORD clst, DWORD val);

static int avio_last_clust = 0;
static int aux_last_clust = 0;
static int aux_last_clust2 = 0;



/*
 get cluster offset by offset from FAT cluster #0

@param offset : 
  offset from cluster #0, bytes units is used.
  should be multiple of size of cluster
  
@return 
  cluster number
*/
int
avio_get_cluster_offset(FATFS* fs, int offset)
{
  UINT clust;
  UINT csize = fs->csize*512;

  if(0 == offset)
  	return fs->n_fatent;
	//aprintf("fs->csize %d",fs->csize);
  clust = offset / csize;

  if(clust < 2) 
    clust = 2;

  return clust;
}

/**
  alloc contiguous_clusters

  @parm fp  :    Pointer to the open file object 
  @len :    Number of bytes to allocate 

  @ return 
    start sector number of contiguous_clusters 
*/
int
avio_alloc_clusters(FIL *fp, UINT sclust, UINT len,UINT len2)
{
  DWORD csz, tcl, tcl2, ncl, ccl, cl, scl;

  if (f_lseek(fp, 0) || !len)	  /* Check if the given parameters are valid */
	return 0;
  csz = 512UL * fp->fs->csize;	  /* Cluster size in unit of byte (assuming 512 bytes/sector) */
  tcl = (len + csz - 1) / csz;	  /* Total number of clusters required */
  tcl2 = (len2 + csz - 1) / csz; 
  len = tcl * csz;				  /* Round-up file size to the cluster boundary */

  /* Check if the existing cluster chain is contiguous */
  if (len == fp->fsize) {
	ncl = 0; ccl = fp->sclust;
	do {
	  cl = get_fat(fp->fs, ccl);  /* Get the cluster status */
	  if (cl + 1 < 3) return 0;   /* Hard error? */
	  if (cl != ccl + 1 && cl < fp->fs->n_fatent) break;  /* Not contiguous? */
	  ccl = cl;
	} while (++ncl < tcl);
	if (ncl == tcl) 			/* Is the file contiguous? */
	  return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
  }
#if _FS_READONLY
  return 0;
#else
  if (f_truncate(fp)) return 0;   /* Remove the existing chain */

  /* Find a free contiguous area */
  scl = fp->fs->last_clust; 	/* Get suggested start point */

  if( !scl || (scl + tcl) >= fp->fs->n_fatent)
	ccl = cl = sclust;
  else
  {
	  ccl = cl = fp->fs->last_clust;
  }
  scl = cl; 
  ncl = 0;

  /*
	  ccl is start cluster of free cluster region
	  ncl is number of free clusters
	  scl is start cluster to be searched.
  */
  do {
	if (cl >= fp->fs->n_fatent) 
	  cl = sclust;
	if (get_fat(fp->fs, cl)) {	/* Encounterd a cluster in use */
	  do {	/* Skip the block of used clusters */
		cl++;
		if(cl == scl)
		  break;
		if (cl >= fp->fs->n_fatent)
		  cl = sclust;
	  } while (get_fat(fp->fs, cl));
	  ccl = cl; ncl = 0;
	}
	cl++; ncl++;
	if(cl == scl)
	  break;
	aprintf("ncl %d tcl %d tcl2 %d",ncl,tcl,tcl2);
  } while (ncl < (tcl+tcl2));
  
  if(ncl < tcl) return 0;
  /* Create a contiguous cluster chain */
  fp->fs->last_clust = ccl - 1;
  if (f_lseek(fp, len)) return 0;

  return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
#endif

}

/**
  alloc contiguous_clusters for aux files

  @parm fp  :    Pointer to the open file object 
  @len :    Number of bytes to allocate 

  @ return 
    start sector number of contiguous_clusters 
*/

int
alloc_aux_clusters(FIL *fp, DWORD sclust, DWORD end_clust, UINT len, BYTE type)
{
  DWORD csz, tcl, ncl, ccl, cl, scl;
  DWORD fatent = fp->fs->n_fatent;
  DWORD _sclust, _eclust, *plast_clust; 
  int sect, res, overwrite = 0;

  if (f_lseek(fp, 0) || !len)     /* Check if the given parameters are valid */
    return 0;
  csz = 512UL * fp->fs->csize;    /* Cluster size in unit of byte (assuming 512 bytes/sector) */
  tcl = (len + csz - 1) / csz;    /* Total number of clusters required */
  len = tcl * csz;                /* Round-up file size to the cluster boundary */

  if(type == AVI_NORMAL_SW)
  {
  	_sclust = sclust;
	_eclust = end_clust;
  	plast_clust = &aux_last_clust;
  }
  else if(type == AVI_EVENT_SW|| type == AVI_MANUAL_SW)	
  {
	_sclust = sclust;
	_eclust = end_clust;
	plast_clust = &aux_last_clust2;
  }
	  
  aprintf("alloc_aux_clusters sclust %d n_fatent %d last %d _sclust %d _eclust %d\n",fp->sclust,fp->fs->n_fatent, fp->fs->last_clust, _sclust, _eclust);

  if(*plast_clust > 0)
  {
		if(*plast_clust > _sclust && (*plast_clust + tcl) < _eclust)
		{
			fp->sclust = *plast_clust +1;
			fp->fs->last_clust = fp->sclust + (tcl-1);
		}
  } 		

  if(fp->fs->last_clust < _sclust)
  {
	  if(*plast_clust > _sclust && (*plast_clust + tcl) < _eclust)
		  fp->fs->last_clust = *plast_clust;
	  else
		  fp->fs->last_clust = _sclust;
  }
  else if((fp->fs->last_clust + tcl) > _eclust)
  {
	  if(*plast_clust > _sclust && (*plast_clust + tcl) < _eclust)
		  fp->fs->last_clust = *plast_clust;
	  else
		  fp->fs->last_clust = _sclust;	
  } 
  
  /* Check if the existing cluster chain is contiguous */
  if (len == fp->fsize) {
    ncl = 0; ccl = fp->sclust;
    do {
      cl = get_fat(fp->fs, ccl);  /* Get the cluster status */
      if (cl + 1 < 3) return 0;   /* Hard error? */
      if (cl != ccl + 1 && cl < fp->fs->n_fatent) break;  /* Not contiguous? */
      ccl = cl;
    } while (++ncl < tcl);
    if (ncl == tcl)             /* Is the file contiguous? */
      return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
  }

#if _FS_READONLY
  return 0;
#else

  if (f_truncate(fp)) return 0;   /* Remove the existing chain */
  
  /* Find a free contiguous area */
  fp->fs->n_fatent = _eclust;

  //if(*plast_clust <= 0)
  {
 	 DWORD cs1 = 1,cs2 = 1,cs3 = 1;
	UINT total = 0;
	UINT scl1 = fp->sclust;
	UINT ecl1 = fp->fs->last_clust;
  	while(1)
  	{	
#ifdef CLUST_SEEK_FAST  	
		while(cs1||cs2)
		{	
			//aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl, tcl2);
			if(scl1 == 0 && ecl1 - scl1 > (tcl))
				scl1 = ecl1 - ((tcl)-1);
			
			aprintf("FAST fp->sclust %d fp->fs->last_clust %d tcl %d\n",scl1,ecl1, tcl);
			cs1 = get_fat(fp->fs, scl1);			/* Get the cluster status */
			cs2 = get_fat(fp->fs, ecl1);			/* Get the cluster status */
			aprintf("FAST cs1 %d cs2 %d\n", cs1, cs2);

			if(cs1==0 && cs2==0)
			{	
#ifdef CLOSE_LSEEK
				fp->sclust = scl1;
				fp->fs->last_clust = ecl1;
				aprintf("FAST fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl);		
#else
			
				fp->fs->last_clust = scl1 - 1;
				fp->sclust = 0; 	
				if (f_lseek(fp, len)) { fp->fs->n_fatent = fatent; return 0;}
				aprintf("FAST fp->sclust %d fp->fs->last_clust %d\n",fp->sclust,fp->fs->last_clust);
				
				if(scl1 != fp->sclust || ecl1 != fp->fs->last_clust)
				{
					printf("scl1 %d fp->sclust %d  ecl1 %d last_clust %d\n", scl1, fp->sclust, ecl1, fp->fs->last_clust);
					scl1 = _sclust+1;
					cs1 = 0;
				    cs2 = 0;
					break;

				}
#endif				
				  if(*plast_clust+1 != fp->sclust)
					aprintf("\n!!!!!aux_last_clust %ld != fp->sclust %d!!!!!!!\n",*plast_clust, fp->sclust);

				  *plast_clust = fp->fs->last_clust;
				  sect = clust2sect(fp->fs, fp->sclust);
				  fp->fs->n_fatent = fatent;

				  return sect;	/* Return file start sector */
			}
					
			if( (ecl1 + (tcl)) <= fp->fs->n_fatent)
			{
			  scl1 = ecl1 + 1;	
			  //fp->sclust = fp->fs->last_clust + (fp->fs->last_clust/16);
			}
			else
			{					
				//scl1 = _sclust+1;
				//cs1 = 0;
				//cs2 = 0;
				//break;
				if(overwrite == 1)
				{
					scl1 = _sclust+1;
					cs1 = 0;
				    cs2 = 0;
					break;
				}
				scl1 = _sclust+1;
				overwrite = 1;				
			}
			ecl1 =  scl1 + ((tcl) - 1);
			
		}			
#endif
		cs3 = get_fat(fp->fs, scl1);			/* Get the cluster status */
  		//aprintf("cs1 %d \n", cs1);
		if(cs3==0)
		{
			if(total%16 == 0)
				aprintf("scl1 %d cs3 %d total %d\n", scl1, cs3, total);
			total++;
		}
		else	
		{
			if(total > 0)
				aprintf("scl1 %d total %d \n", scl1, total);
			
			total = 0;
		}

		if(cs3==0 && total == (tcl))
		{			
#ifdef CLOSE_LSEEK		
			fp->sclust = scl1 - ((tcl) -1);
			fp->fs->last_clust = fp->sclust + ((tcl) - 1);
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d\n",fp->sclust,fp->fs->last_clust, tcl);
#else				
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d\n",fp->sclust,fp->fs->last_clust, tcl);
			fp->fs->last_clust = scl1 - (tcl);
			fp->sclust = 0; 	
			if (f_lseek(fp, len)) { fp->fs->n_fatent = fatent; return 0;}
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d\n",fp->sclust,fp->fs->last_clust, tcl);

			ecl1 = scl1;
			scl1 = scl1 - (tcl-1);
			
			if(scl1 != fp->sclust || ecl1 != fp->fs->last_clust)
			{
				printf("scl1 %d fp->sclust %d  ecl1 %d last_clust %d\n", scl1, fp->sclust, ecl1, fp->fs->last_clust);
				fp->fs->n_fatent = fatent;
				return 0;
			}
#endif			
			break;
		}
		
		if( (scl1 + (tcl)) <= fp->fs->n_fatent)
		{			
			scl1++;
		}
		else
		{		
			//if(overwrite == 1){
			//	  fp->fs->n_fatent = fatent;
			//	return 0;}
		  	//scl1 = _sclust;
		  	//overwrite = 1;

			fp->fs->n_fatent = fatent;
			scl1 = _sclust;
			return 0;
		}		
  	}

  }

  if(*plast_clust+1 != fp->sclust)
 	aprintf("\n!!!!!aux_last_clust %ld != fp->sclust %d!!!!!!!\n",*plast_clust, fp->sclust);

  *plast_clust = fp->fs->last_clust;
  sect = clust2sect(fp->fs, fp->sclust);
  fp->fs->n_fatent = fatent;

  return sect;  /* Return file start sector */
#endif

}


int
alloc_aux_clusters_thumb(FIL *fp, DWORD sclust, DWORD end_clust, UINT len, UINT len2, BYTE type)
{
  DWORD csz, tcl, tcl2, ncl, ccl, cl, scl;
  DWORD fatent = fp->fs->n_fatent;
  DWORD _sclust, _eclust, *plast_clust; 
  int sect, res, overwrite = 0;

  if (f_lseek(fp, 0) || !len)     /* Check if the given parameters are valid */
    return 0;
  csz = 512UL * fp->fs->csize;    /* Cluster size in unit of byte (assuming 512 bytes/sector) */
  tcl = (len + csz - 1) / csz;    /* Total number of clusters required */
  tcl2 = (len2 + csz - 1) / csz;    /* Total number of clusters required */
  len = tcl * csz;                /* Round-up file size to the cluster boundary */
  len2 = tcl2 * csz;  

  if(type == AVI_NORMAL_SW)
  {
  	_sclust = sclust;
	_eclust = end_clust;
  	plast_clust = &aux_last_clust;
  }
  else if(type == AVI_EVENT_SW|| type == AVI_MANUAL_SW)	
  {
	_sclust = sclust;
	_eclust = end_clust;
	plast_clust = &aux_last_clust2;
  }
	  
  aprintf("alloc_aux_clusters_thumb sclust %d n_fatent %d last %d _sclust %d _eclust %d\n",fp->sclust,fp->fs->n_fatent, fp->fs->last_clust, _sclust, _eclust);

  if(*plast_clust > 0)
  {
		if(*plast_clust > _sclust && (*plast_clust + (tcl + tcl2)) < _eclust)
		{
			fp->sclust = *plast_clust +1;
			fp->fs->last_clust = fp->sclust + ((tcl + tcl2)-1);
		}
  } 	
  
  if(fp->sclust < _sclust)
  {
	  fp->sclust = _sclust+1;
	  fp->fs->last_clust = fp->sclust + ((tcl + tcl2)-1);
  }

  if(fp->fs->last_clust < _sclust)
  {
	  if(*plast_clust > _sclust && (*plast_clust + tcl + tcl2) < _eclust)
		  fp->fs->last_clust = *plast_clust;
	  else
		  fp->fs->last_clust = _sclust;
  }
  else if((fp->fs->last_clust + tcl + tcl2) > _eclust)
  {
	  if(*plast_clust > _sclust && (*plast_clust + tcl + tcl2) < _eclust)
		  fp->fs->last_clust = *plast_clust;
	  else
		  fp->fs->last_clust = _sclust;	
  } 
  
  /* Check if the existing cluster chain is contiguous */
  if (len == fp->fsize) {
    ncl = 0; ccl = fp->sclust;
    do {
      cl = get_fat(fp->fs, ccl);  /* Get the cluster status */
      if (cl + 1 < 3) return 0;   /* Hard error? */
      if (cl != ccl + 1 && cl < fp->fs->n_fatent) break;  /* Not contiguous? */
      ccl = cl;
    } while (++ncl < tcl);
    if (ncl == tcl)             /* Is the file contiguous? */
      return clust2sect(fp->fs, fp->sclust);  /* Return file start sector */
  }

#if _FS_READONLY
  return 0;
#else

  if (f_truncate(fp)) return 0;   /* Remove the existing chain */
  
  /* Find a free contiguous area */
  fp->fs->n_fatent = _eclust;
 
	DWORD cs1 = 1,cs2 = 1,cs3 = 1,cs4 = 1;

	DWORD total = 0;
	DWORD scl1 = fp->sclust;
	DWORD ecl1 = fp->fs->last_clust;
	
	int once = 1;
#if 0//test
			{
				DWORD sclt = _sclust+1;
				DWORD totalt = 0;
				while(once)
				{
					DWORD cst = get_fat(fp->fs, sclt);			/* Get the cluster status */
					if(cst==0)
					{
						if(totalt > 0)
							printf(" %ld total %ld \n", sclt, totalt);
						totalt = 0;
					}
					else	
					{
						if(totalt == 0)
							printf("scl1 %ld ", sclt);
						totalt++;
						
					}
					sclt++;
					if(sclt >=end_clust)
					{				
						once = 0;
						break;
					}
				}
			}
#endif

  	while(1)
  	{  
#ifdef CLUST_SEEK_FAST  	
	  	while(cs1||cs2||cs3)
	  	{	
	  		//aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl, tcl2);
	  		if(scl1 == 0 && ecl1 - scl1 > (tcl + tcl2))
				scl1 = ecl1 - ((tcl + tcl2)-1);
			
	  		aprintf("FAST fp->sclust %d avi scluste %d fp->fs->last_clust %d tcl %d tcl2 %d\n", scl1, scl1+tcl, ecl1,  tcl, tcl2);
			cs1 = get_fat(fp->fs, scl1);			/* Get the cluster status */
			cs2 = get_fat(fp->fs, ecl1);			/* Get the cluster status */
			cs3 = get_fat(fp->fs, scl1+tcl);		/* Get the cluster status */
	  		aprintf("FAST cs1 %d cs3 %d cs2 %d\n", cs1, cs3, cs2);

			if( (overwrite == 1) && (!(cs1==0 && cs2==0 && cs3==0)) && (cs1==0 || cs2==0 || cs3==0) )
			{			
				printf("FAST error file [fp->sclust %d avi scluste %d fp->fs->last_clust %d tcl %d tcl2 %d]\n", scl1, scl1+tcl, ecl1,  tcl, tcl2);
	  			printf("FAST error file [cs1 %d cs3 %d cs2 %d]\n", cs1, cs3, cs2);
				DWORD sclust1;
				for(sclust1 = scl1; sclust1 <= ecl1; sclust1++)
					put_fat(fp->fs, sclust1, 0x0);	
				
			}

			if(cs1==0 && cs2==0 && cs3==0)
			{	
#ifdef CLOSE_LSEEK
				fp->sclust = scl1;
				fp->fs->last_clust = fp->sclust + ((tcl) - 1);
				aprintf("FAST fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl);

#else
				fp->fs->last_clust = scl1 - 1;
				fp->sclust = 0;		
				if (f_lseek(fp, len)) { fp->fs->n_fatent = fatent; return 0;}
				aprintf("FAST fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n", fp->sclust, fp->fs->last_clust);
				if(scl1 != fp->sclust || (ecl1 - tcl2)!= fp->fs->last_clust)
				{
					printf("scl1 %d fp->sclust %d  ecl1 %d last_clust %d\n", scl1, fp->sclust, ecl1, fp->fs->last_clust);
					scl1 = _sclust+1;
					cs1 = 0;
				    cs2 = 0;
					cs3 = 0;
					break;

				}
#endif
				if(*plast_clust+1 != fp->sclust)
					aprintf("\n!!!!!aux_last_clust %ld != fp->sclust %d!!!!!!!\n",*plast_clust, fp->sclust);

				*plast_clust = fp->fs->last_clust;

				sect = clust2sect(fp->fs, fp->sclust);
				fp->fs->n_fatent = fatent;

				return sect;  /* Return file start sector */
			}
					
			if( (ecl1 + (tcl + tcl2)) <= fp->fs->n_fatent)
			{
			  scl1 = ecl1 + 1;	
			  //fp->sclust = fp->fs->last_clust + (fp->fs->last_clust/16);
			}
			else
			{					
			 	//scl1 = _sclust+1;	
				//cs1 = 0;
				//cs2 = 0;
				//break;
				if(overwrite == 1)
				{
					scl1 = _sclust+1;
					cs1 = 0;
				    cs2 = 0;
					cs3 = 0;
					break;
				}
				scl1 = _sclust+1;
				overwrite = 1;
			}
			ecl1 =  scl1 + ((tcl + tcl2) - 1);
			
	  	}	
#endif		
#if 0//1 //test
		{
			DWORD sclt = _sclust+1;
			DWORD totalt = 0;
			while(once)
			{
				DWORD cst = get_fat(fp->fs, sclt);			/* Get the cluster status */
				if(cst==0)
				{
					if(totalt > 0)
						printf(" %ld total %ld \n", sclt, totalt);
					totalt = 0;
				}
				else	
				{
					if(totalt == 0)
						printf("scl1 %ld ", sclt);
					totalt++;
					
				}
				sclt++;
				if(sclt >=end_clust)
				{				
					once = 0;
					break;
				}
			}
		}
#endif

		cs4 = get_fat(fp->fs, scl1);			/* Get the cluster status */
  		
		if(cs4==0)
		{
			if(total%16 == 0)
				aprintf("scl1 %d cs3 %d total %d\n", scl1, cs4, total);
			total++;
		}
		else	
		{
			if(total > 0)
				aprintf("scl1 %d total %d \n", scl1, total);
			
			total = 0;
		}
		
		if(cs4==0 && total == ((tcl + tcl2)))
		{			
#ifdef CLOSE_LSEEK
			fp->sclust = scl1 - ((tcl + tcl2) -1);
			fp->fs->last_clust = fp->sclust + ((tcl) - 1);
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl, tcl2);
#else			
			aprintf("scl1 %d total %d (tcl + tcl2)-1 %d\n",scl1,total,(tcl + tcl2)-1);
			
			fp->fs->last_clust = scl1 - ((tcl + tcl2));
			fp->sclust = 0;	
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d scl1 %d \n",fp->sclust,fp->fs->last_clust, tcl, tcl2, scl1);
			if (f_lseek(fp, len)) { fp->fs->n_fatent = fatent; return 0;}
			aprintf("fp->sclust %d fp->fs->last_clust %d tcl %d tcl2 %d\n",fp->sclust,fp->fs->last_clust, tcl, tcl2);

			ecl1 = scl1;
			scl1 = scl1 - ((tcl + tcl2)-1);

			if(scl1 != fp->sclust || (ecl1 - tcl2) != fp->fs->last_clust)
			{
				printf("scl1 %d fp->sclust %d  ecl1 %d last_clust %d\n", scl1, fp->sclust, ecl1, fp->fs->last_clust);
				fp->fs->n_fatent = fatent;
				return 0;
			}
#endif			
			break;
		}
		
		if( (scl1 + (tcl + tcl2)) <= fp->fs->n_fatent)
		{			
			scl1++;
		}
		else
		{		
			//if(overwrite == 1){
			//	  fp->fs->n_fatent = fatent;
			//	return 0;}
		  	//scl1 = _sclust;
		  	//overwrite = 1;
		  	
		  	scl1 = _sclust;
			fp->fs->n_fatent = fatent;
			return 0;
		}		
  	}


  if(*plast_clust+1 != fp->sclust)
 	aprintf("\n!!!!!aux_last_clust %ld != fp->sclust %d!!!!!!!\n",*plast_clust, fp->sclust);
  *plast_clust = fp->fs->last_clust;
  sect = clust2sect(fp->fs, fp->sclust);
  fp->fs->n_fatent = fatent;

  return sect;  /* Return file start sector */
#endif

}


/**
  alloc contiguous_clusters for aux files

  @parm fp  :    Pointer to the open file object 
  @len :    Number of bytes to allocate 

  @ return 
    start sector number of contiguous_clusters 
*/

#ifdef CONTIGUOUS_WRITE

/**
  write avi->buffer to sd card directly by sector unit
  @param bw			Pointer to number of bytes written 

  @return 
   FR_OK
   ...
  0x1000 : out_of_sector 
   
*/
int 
avio_low_write(t_avienc_t *pavi, UINT *wb)
{
  UINT ws = 0;
  UINT fres =0 ;
  
  FIL *fp = &pavi->fpavi;
  *wb = 0;
  /* 
    Here, We writes to SD by Sectors Uint
    except end of file, pavi->buffer.length should be mulitiples of 512.
  */
  ws = (pavi->buffer.length + 511)/512;
  if((pavi->sect + ws) <= pavi->end_sect)
  {
    fres = disk_write(fp->fs->drv, (BYTE*)pavi->buffer.base, pavi->sect, ws);   /* Write 512Ki bytes from top of the file */
    
    pavi->sect += ws; /* advance write offset */
  }
  else
    fres = AVIO_OVER_CLUSTER;
  *wb = pavi->buffer.length;
  
//  if(fres != FR_OK)
//    return 0;
    
  return fres;
}


/**
  write to to sd card directly by sector unit

  @return 
   FR_OK
   ...
  0x1000 : out_of_sector 
   
*/
int 
aux_file_low_write(FIL *fp, char *to, int w_len, int sect)
{
  UINT ws = 0;
  UINT fres =0 ;
  /* 
    Here, We writes to SD by Sectors Uint
    except end of file, pavi->buffer.length should be mulitiples of 512.
  */
  ws = (w_len + 511)/512;

  fres = disk_write(fp->fs->drv, to, sect, ws);   /* Write 512Ki bytes from top of the file */
        
  return fres;
}

#ifdef CLOSE_LSEEK	
int 
aux_file_lseek(FIL *fp, int w_len)
{
	UINT sclust = fp->sclust, fres;
	UINT eclust = fp->fs->last_clust;
	
	aprintf("[ f_lseek before sclust %d last %d\n", fp->sclust, fp->fs->last_clust);
	
	fp->fs->last_clust = fp->sclust - 1;
	fp->sclust = 0;		
	if ( fres = f_lseek(fp, w_len)) 
	{
		aprintf("f_lseek Err Msg : %s\n", put_rc(fres));
		return -1;
	}
	
	aprintf("[ f_lseek after sclust %d last %d\n", fp->sclust, fp->fs->last_clust);
	
	if(sclust != fp->sclust || eclust != fp->fs->last_clust)
	{
		aprintf("[ f_lseek after  missmatch sclust %d s %d last %d l %d\n", sclust, fp->sclust, eclust,  fp->fs->last_clust);
		
		fp->fs->last_clust = fp->sclust - 1;
		fp->sclust = 0; 	
		
		if ( fres = f_lseek(fp, w_len)) 
		{
			aprintf("re f_lseek Err Msg : %s\n", put_rc(fres));
			return -1;
		}
		
		if(sclust != fp->sclust || eclust != fp->fs->last_clust)
		{
			aprintf("[ re f_lseek after  missmatch sclust %d s %d last %d l %d\n", sclust, fp->sclust, eclust,  fp->fs->last_clust);
			return -1;
		}
	}
}		
#endif		

#endif
