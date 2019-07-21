#ifndef __AVIO_FAT32__
#define __AVIO_FAT32__

#define AVI_START_CLUST 1280//128

//#define CLOSE_LSEEK
#define CLUST_SEEK_FAST

/*
 get cluster offset by offset from FAT cluster #0

@param offset : 
  offset from cluster #0, bytes units is used.
  should be multiple of size of cluster
  
@return 
  cluster number
*/
int
avio_get_cluster_offset(FATFS* fs, int offset);

/**
  alloc contiguous_clusters

  @parm fp  :    Pointer to the open file object 
  @len :    Number of bytes to allocate 

  @ return 
    start sector number of contiguous_clusters 
*/
int
avio_alloc_clusters(FIL *fp, UINT sclust, UINT len, UINT len2);
/*
  This case is over reserved clusters of contiguous area 
*/
#define AVIO_OVER_CLUSTER 0x1000

/**
  write avi->buffer to sd card directly by sector unit

  @return 
   number of bytes written
   if fails, retrun 0
*/
int 
avio_low_write(t_avienc_t *pavi, UINT *wb);

int 
aux_file_low_write(FIL *fp, char *to, int w_len, int sect);

/**
  alloc contiguous_clusters for aux files

  @parm fp  :    Pointer to the open file object 
  @sclust    :    start clust of disk
  @end_clust :    end clust of disk 
  @len :    Number of bytes to allocate 
  @type      :    AVI video type (NORMAL, EVENT, MANUAL, THUMBNAIL)

  @ return 
    start sector number of contiguous_clusters 
*/
int
alloc_aux_clusters(FIL *fp, DWORD sclust, DWORD end_clust, UINT len, BYTE type);

/**
  alloc contiguous_clusters for thumbnail files

  @parm fp   :    Pointer to the open file object   
  @sclust    :    start clust of disk
  @end_clust :    end clust of disk 
  @len       :    Number of bytes to allocate 
  @len2      :    Number of bytes to allocate 
  @type      :    AVI video type (NORMAL, EVENT, MANUAL, THUMBNAIL)

  @ return 
    start sector number of contiguous_clusters 
*/
int
alloc_aux_clusters_thumb(FIL *fp, DWORD sclust, DWORD end_clust, UINT len, UINT len2, BYTE type);

/**
  seek file

  @parm fp   :    Pointer to the open file object   
  @len       :    Number of bytes 

  @ return 
    success or fail
*/
int 
aux_file_lseek(FIL *fp, int w_len);

#endif
