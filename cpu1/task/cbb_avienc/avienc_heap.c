#include <time.h>
#include <string.h>

#include "dev.h"
#include "dev_types.h"
#include "file.h"
#include "avienc_heap.h"
//#include "vsprintf.h"
#include "muxer_avienc.h"

#ifdef AVIENC_HEAP_USE

#if 0
#define ENTER() dprintf("%s IN\n", __func__);
#define LEAVE() dprintf("%s OUT\n", __func__);
#else
#define ENTER()   
#define LEAVE()  
#endif

#ifdef __DEVICE_SD__

#define encHeap_getParent(idx)		((int)(((idx)-1)/2))
#define encHeap_getLeftChild(idx)	((int)(2*(idx))+1)

encHeap* encHeap_create(int capacity)
{
	encHeap* h = (encHeap*)pvPortCalloc(sizeof(encHeap), 1);
	if(h)
	{
		h->capacity = capacity;
		h->usedsize = 0;
		h->node = (encHData*)pvPortCalloc(sizeof(encHData), h->capacity);
	}
	return h;
}

void encHeap_destroy(encHeap* h)
{
	if(h->node)
		vPortFree(h->node);
	if(h)
		vPortFree(h);
}

static void encHeap_swapNode(encHeap* h, int idx1, int idx2)
{
	int copySize = sizeof(encHData);
	encHData temp;
	memcpy(&temp, &h->node[idx1], copySize);
	memcpy(&h->node[idx1], &h->node[idx2], copySize);
	memcpy(&h->node[idx2], &temp, copySize);
}

static void encHeap_deQueue(encHeap* h, encHData* root) 
{
	int parentPos = 0, leftPos = 0, rightPos = 0;
	memcpy(root, &h->node[0], sizeof(encHData));
	memset(&h->node[0], 0, sizeof(encHData));
	h->usedsize--;
	encHeap_swapNode(h, 0, h->usedsize);
	leftPos = encHeap_getLeftChild(0);
	rightPos = leftPos + 1;
	while(1)
	{
		int selectChild = 0;
		if(leftPos >= h->usedsize)
			break;

		if(rightPos >= h->usedsize)
		{
			selectChild = leftPos;
		}
		else
		{
			if(strcmp(h->node[leftPos].filename+1, h->node[rightPos].filename+1) > 0)
				selectChild = rightPos;
			else
				selectChild = leftPos;
		} 

		if(strcmp(h->node[selectChild].filename+1, h->node[parentPos].filename+1) < 0)
		{
			encHeap_swapNode(h, parentPos, selectChild);
			parentPos = selectChild;
		}
		else
		{
			break;
		}
		leftPos = encHeap_getLeftChild(parentPos);
		rightPos = leftPos + 1;
	}
}

static int encHeap_enQueue(encHeap* h, encHData data)
{
	int currentPos, parentPos, deletereturn = 0;

	// 남아있는 공간이 없으면
	if(h->usedsize == h->capacity)
	{
		deletereturn = 1;	// 삭제할 파일이 존재
		if(strcmp(h->node->filename+1, data.filename+1) > 0)
		{
			// 현재 들어온 data.filename을 삭제해야 한다. 삭제 후 할 일은 없다.
			memcpy(&h->delHData, &data, sizeof(encHData));
//			printf("delete1 %s\n", data.filename);
			return deletereturn;
		}
		else
		{
			// 기존의 heap 데이터 중 가장 오래된 filename을 삭제한다. 그리고 들어온 data.filename을 추가한다.
			encHData temp;
			encHeap_deQueue(h, &temp);
			memcpy(&h->delHData, &temp, sizeof(encHData));
//			printf("delete2 %s\n", temp.filename);
		}
	}

	currentPos = h->usedsize;
	parentPos = encHeap_getParent(currentPos);

	memcpy(&h->node[currentPos], &data, sizeof(encHData));

	while(currentPos > 0 && strcmp(h->node[currentPos].filename+1, h->node[parentPos].filename+1) < 0)
	{
		encHeap_swapNode(h, currentPos, parentPos);
		currentPos = parentPos;
		parentPos = encHeap_getParent(currentPos);
	}
	h->usedsize++;

	return deletereturn;
}

int encHeap_checkQueue(encHeap* h)
{
	int currentPos, parentPos, deletereturn = 0;

	// 남아있는 공간이 없으면
	if(h->usedsize == h->capacity)
	{
		deletereturn = 1;
		// 기존의 heap 데이터 중 가장 오래된 filename을 삭제한다. 그리고 들어온 data.filename을 추가한다.
		encHData temp;
		encHeap_deQueue(h, &temp);
		memcpy(&h->delHData, &temp, sizeof(encHData));
//		printf("delete2 %s\n", temp.filename);
	}

	return deletereturn;
}

void encHeap_deleteQueue(encHeap* h, char* filename)
{
	encHData temp;
	encHeap_deQueue(h, &temp);
	strcpy(filename, temp.filename);
}

int encHeap_addQueue(encHeap* h, char* filename)
{
	encHData temp;
	strcpy(temp.filename, filename);
	return encHeap_enQueue(h, temp);
}

#ifdef __JPEGEXIF__
void exif_parser(void *penc)
{
	t_avienc_t *enc_t = (t_avienc_t *)penc;
	char modifydate[20];
	strncpy(modifydate, enc_t->now_name+1,4);
	strncpy(modifydate+4, ":",1);  
	strncpy(modifydate+5, enc_t->now_name+5,2);
	strncpy(modifydate+7, ":",1);  
	strncpy(modifydate+8, enc_t->now_name+7,2);  
	strncpy(modifydate+10, " ",1);	
	strncpy(modifydate+11, enc_t->now_name+9,2);   
	strncpy(modifydate+13, ":",1);	 
	strncpy(modifydate+14, enc_t->now_name+11,2);	
	strncpy(modifydate+16, ":",1);	 
	strncpy(modifydate+17, enc_t->now_name+13,2);	
	strncpy(modifydate+19, 0x00,1);  

	//dprintf("modifydate %s\n",modifydate);
	//int tt=0;
	//for(tt=0;tt<20;tt++)
	//  printf("%x ",*(modifydate+tt));

	//dprintf("GPSDATA %s \n",enc_t->gpsdata);//-0000_00000N-00000_00000E-0_000-00_00

	//for(tt=0;tt<20;tt++)
	//  printf(" %d %s \n",tt, enc_t->pstrRMGMC[tt]);

	//latitude //ex) ddmm.mmmmm, dd = dd, mm = mm,	s = 0.mmmmm*60
	int latitude_dd=0;
	int latitude_mm=0;
	int latitude_ss=0; 	
	if(strlen(enc_t->pstrRMGMC[3]))
	{
		float testll = atof((char*)enc_t->pstrRMGMC[3]);//4717.11437;
		int testii = testll;
		testll = testll-testii;
		float ss = testll*60;
		//dprintf("float testl %f testi%d ss %f sz %d \n",testll,testii, ss, sizeof(double));
		latitude_dd = testii/100;
		latitude_mm = testii-(latitude_dd*100);
		latitude_ss = ss*10000; 		
		dprintf("latitude_dd %d latitude_mm %d latitude_ss %d ",latitude_dd,latitude_mm, latitude_ss);
	}


	
	//latitude ref 
	char latituderef="";
	if(strlen(enc_t->pstrRMGMC[4]))
	{
		latituderef = *(enc_t->pstrRMGMC[4]);
	}
	dprintf("latituderef %c \n",latituderef);


	
	//longitude //ex)dddmm.mmmmm, ddd = ddd, mm = mm,  s = 0.mmmmm*60 	
	int longitude_ddd=0;
	int longitude_mm=0;
	int longitude_ss=0;		
	if(strlen(enc_t->pstrRMGMC[5]))
	{
		float testlo = atof((char*)enc_t->pstrRMGMC[5]);//00833.91522;
		int testio = testlo;
		testlo = testlo - testio;
		float sso = testlo*60;
		//dprintf("float testlo %f testio%d ss %f sz %d \n", testlo, testio, sso, sizeof(double));
		longitude_ddd = testio/100;
		longitude_mm = testio-(longitude_ddd*100);
		longitude_ss = sso*10000;		
		dprintf("longitude_ddd %d longitude_mm %d longitude_ss %d ", longitude_ddd, longitude_mm, longitude_ss);
	}
	
	//longitude ref
	char longituderef = "";
	if(strlen(enc_t->pstrRMGMC[6]))
	{
		longituderef = *(enc_t->pstrRMGMC[6]);	 
	}
	dprintf("longituderef %c \n",longituderef);

	
	BYTE exifdata[EXIF_TOTAL_LENGTH]
			 = { 0xFF, 0xD8 //SOI_MARKER
			 
			   , 0xFF, 0xE1 //APP1_MARKER
			   , 0x00, 0xF6 //total length '214+2'
			   
			   , 0x45, 0x78, 0x69, 0x66, 0x00, 0x00 //EXIF_HEADER
			   
			   , 0x49, 0x49 //TIFF_HEADER
			   , 0x2A, 0x00 //identifier				   
			   , 0x08, 0x00, 0x00, 0x00 //TIFF_OFFSET					   
			   , 0x08, 0x00//number of directory entry of IFD0 is '8'.
			   
			   , 0x32, 0x01//TAG_DATETIME 1
			   , 0x02, 0x00//DATA_FORMAT_ASCII_STR
			   , 0x14, 0x00, 0x00, 0x00 //number of component 1byte x 20 = 20byte
			   , 0x6E, 0x00, 0x00, 0x00 //offset to data from TIFF_HEADER(if 4byte over)
			   
			   , 0x08, 0x92//TAG_LIGHTSOURCE 2
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x00, 0x00, 0x00, 0x00//data
			   
			   , 0x12, 0x01//TAG_ORIENTATION 3
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x00, 0x00, 0x00, 0x00//data		
								   
			   , 0x01, 0x01//TAG_IMAGELENGTH 4
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x38, 0x04, 0x00, 0x00//data //0x438 // 1080
			   
			   , 0x07, 0x92//TAG_METERINGMODE 5
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0xFF, 0xFF, 0xFF, 0xFF//data 
			   
			   , 0x00, 0x01//TAG_IMAGEWIDTH 6
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x80, 0x07, 0x00, 0x00//data //0x780 //1920
			   
			   , 0x25, 0x88//TAG_GPSINFO 7
			   , 0x04, 0x00//DATA_FORMAT_ULONG
			   , 0x01, 0x00, 0x00, 0x00//number of component 4byte x 01 = 4byte
			   , 0x88, 0x00, 0x00, 0x00//offset to data from TIFF_HEADER(if 4byte over) '0x82'
			   
			   , 0x69, 0x87//EXIF_SUBIFD   8			   
			   , 0x04, 0x00//DATA_FORMAT_ULONG
			   , 0x01, 0x00, 0x00, 0x00//Exif subIFD offset 4byte				   
			   , 0x82, 0x00, 0x00, 0x00//Exif SubIFD starts from address '0x00B0
			   , 0x00, 0x00, 0x00, 0x00//Next IFD starts from address '0x0000'

			   //datetime offset 0x6E total 20byte
			   //, 0x32, 0x30, 0x31, 0x36, 0x3A, 0x30, 0x32, 0x3A
			   //, 0x31, 0x35, 0x20, 0x31, 0x37, 0x3A, 0x30, 0x30
			   //, 0x3A, 0x33, 0x37, 0x00//datetime data 
			   ,*(modifydate+0),*(modifydate+1),*(modifydate+2),*(modifydate+3),*(modifydate+4),*(modifydate+5),*(modifydate+6),*(modifydate+7)
			   ,*(modifydate+8),*(modifydate+9),*(modifydate+10),*(modifydate+11),*(modifydate+12),*(modifydate+13),*(modifydate+14),*(modifydate+15)
			   ,*(modifydate+16),*(modifydate+17),*(modifydate+18),*(modifydate+19)
			   
			   , 0x00, 0x00 //ExifIFD entries
			   , 0x00, 0x00, 0x00, 0x00 //Next IFD

			   //GPSinfor data '0x88'
			   , 0x04, 0x00 //Gps entries // entry count '2'
			   
			   , 0x04, 0x00 //TAG_GPSLONGITUDE
			   , 0x0A, 0x00 //DATA_FORMAT_RATIONAL 
			   , 0x03, 0x00, 0x00, 0x00 //length 3x8byte = 24byte.
			   , 0xBE, 0x00, 0x00, 0x00 //offset '0xB8'

			   , 0x01, 0x00 //TAG_GPSLATITUDEREF
			   , 0x02, 0x00 //DATA_FORMAT_ASCII_STR
			   , 0x02, 0x00, 0x00, 0x00 //length 2byte					   
			   , latituderef, 0x00, 0x00, 0x00 //value

			   , 0x02, 0x00 //TAG_GPSLATITUDE
			   , 0x0A, 0x00 //DATA_FORMAT_RATIONAL
			   , 0x03, 0x00, 0x00, 0x00 //length 3x8byte = 24byte.
			   , 0xD6, 0x00, 0x00, 0x00 //offset '0xC0'
			   
			   , 0x03, 0x00 //TAG_GPSLONGITUDEREF
			   , 0x02, 0x00 //DATA_FORMAT_ASCII_STR
			   , 0x02, 0x00, 0x00, 0x00 //length 2byte		  
			   , longituderef, 0x00, 0x00, 0x00 //value
								   
			   , 0x00, 0x00, 0x00, 0x00 //nextIFD

			   //data of GPS longitude					   
			   ,(BYTE)*(((BYTE*)&longitude_ddd)+3),(BYTE)*(((BYTE*)&longitude_ddd)+2),(BYTE)*(((BYTE*)&longitude_ddd)+1),(BYTE)*(((BYTE*)&longitude_ddd)+0), 0x01, 0x00, 0x00, 0x00
			   ,(BYTE)*(((BYTE*)&longitude_mm)+3),(BYTE)*(((BYTE*)&longitude_mm)+2),(BYTE)*(((BYTE*)&longitude_mm)+1),(BYTE)*(((BYTE*)&longitude_mm)+0), 0x01, 0x00, 0x00, 0x00
			   ,(BYTE)*(((BYTE*)&longitude_ss)+3),(BYTE)*(((BYTE*)&longitude_ss)+2),(BYTE)*(((BYTE*)&longitude_ss)+1),(BYTE)*(((BYTE*)&longitude_ss)+0), 0x10, 0x27, 0x00, 0x00
			   //, 0x7f, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
			   //, 0x03, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
			   //, 0x2e, 0x14, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00
			   
			   //data of GPS latitude
			   ,(BYTE)*(((BYTE*)&latitude_dd)+3),(BYTE)*(((BYTE*)&latitude_dd)+2),(BYTE)*(((BYTE*)&latitude_dd)+1),(BYTE)*(((BYTE*)&latitude_dd)+0), 0x01, 0x00, 0x00, 0x00
			   ,(BYTE)*(((BYTE*)&latitude_mm)+3),(BYTE)*(((BYTE*)&latitude_mm)+2),(BYTE)*(((BYTE*)&latitude_mm)+1),(BYTE)*(((BYTE*)&latitude_mm)+0), 0x01, 0x00, 0x00, 0x00
			   ,(BYTE)*(((BYTE*)&latitude_ss)+3),(BYTE)*(((BYTE*)&latitude_ss)+2),(BYTE)*(((BYTE*)&latitude_ss)+1),(BYTE)*(((BYTE*)&latitude_ss)+0), 0x10, 0x27, 0x00, 0x00
			   //, 0x25, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
			   //, 0x0e, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00
			   //, 0x3c, 0x11, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00
			   };//246//
			   
	memcpy(enc_t->gpsdata, exifdata, EXIF_TOTAL_LENGTH);
}

void exif_default(void *penc)
{
	t_avienc_t *enc_t = (t_avienc_t *)penc;
	char modifydate[20];
	strncpy(modifydate, enc_t->now_name+1,4);
	strncpy(modifydate+4, ":",1);  
	strncpy(modifydate+5, enc_t->now_name+5,2);
	strncpy(modifydate+7, ":",1);  
	strncpy(modifydate+8, enc_t->now_name+7,2);  
	strncpy(modifydate+10, " ",1);	
	strncpy(modifydate+11, enc_t->now_name+9,2);   
	strncpy(modifydate+13, ":",1);	 
	strncpy(modifydate+14, enc_t->now_name+11,2);	
	strncpy(modifydate+16, ":",1);	 
	strncpy(modifydate+17, enc_t->now_name+13,2);	
	strncpy(modifydate+19, 0x00,1);  

	printf("modifydate %s\n",modifydate);

	
	BYTE exifdata[EXIF_TOTAL_LENGTH]
			 = { 0xFF, 0xD8 //SOI_MARKER
			 
			   , 0xFF, 0xE1 //APP1_MARKER
			   , 0x00, 0xF6 //total length '214+2'
			   
			   , 0x45, 0x78, 0x69, 0x66, 0x00, 0x00 //EXIF_HEADER
			   
			   , 0x49, 0x49 //TIFF_HEADER
			   , 0x2A, 0x00 //identifier				   
			   , 0x08, 0x00, 0x00, 0x00 //TIFF_OFFSET					   
			   , 0x07, 0x00//number of directory entry of IFD0 is '8'.
			   
			   , 0x32, 0x01//TAG_DATETIME 1
			   , 0x02, 0x00//DATA_FORMAT_ASCII_STR
			   , 0x14, 0x00, 0x00, 0x00 //number of component 1byte x 20 = 20byte
			   , 0x6E, 0x00, 0x00, 0x00 //offset to data from TIFF_HEADER(if 4byte over)
			   
			   , 0x08, 0x92//TAG_LIGHTSOURCE 2
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x00, 0x00, 0x00, 0x00//data
			   
			   , 0x12, 0x01//TAG_ORIENTATION 3
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x00, 0x00, 0x00, 0x00//data		
								   
			   , 0x01, 0x01//TAG_IMAGELENGTH 4
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x38, 0x04, 0x00, 0x00//data //0x438 // 1080
			   
			   , 0x07, 0x92//TAG_METERINGMODE 5
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0xFF, 0xFF, 0xFF, 0xFF//data 
			   
			   , 0x00, 0x01//TAG_IMAGEWIDTH 6
			   , 0x03, 0x00//DATA_FORMAT_USHORT
			   , 0x01, 0x00, 0x00, 0x00//number of component 2byte x 01 = 2byte
			   , 0x80, 0x07, 0x00, 0x00//data //0x780 //1920
			   
			   , 0x25, 0x88//TAG_GPSINFO 7
			   , 0x04, 0x00//DATA_FORMAT_ULONG
			   , 0x01, 0x00, 0x00, 0x00//number of component 4byte x 01 = 4byte
			   , 0x88, 0x00, 0x00, 0x00//offset to data from TIFF_HEADER(if 4byte over) '0x82'
			   
			   , 0x69, 0x87//EXIF_SUBIFD   8			   
			   , 0x04, 0x00//DATA_FORMAT_ULONG
			   , 0x01, 0x00, 0x00, 0x00//Exif subIFD offset 4byte				   
			   , 0x82, 0x00, 0x00, 0x00//Exif SubIFD starts from address '0x00B0
			   , 0x00, 0x00, 0x00, 0x00//Next IFD starts from address '0x0000'

			   //datetime offset 0x6E total 20byte
			   //, 0x32, 0x30, 0x31, 0x36, 0x3A, 0x30, 0x32, 0x3A
			   //, 0x31, 0x35, 0x20, 0x31, 0x37, 0x3A, 0x30, 0x30
			   //, 0x3A, 0x33, 0x37, 0x00//datetime data 
			   ,*(modifydate+0),*(modifydate+1),*(modifydate+2),*(modifydate+3),*(modifydate+4),*(modifydate+5),*(modifydate+6),*(modifydate+7)
			   ,*(modifydate+8),*(modifydate+9),*(modifydate+10),*(modifydate+11),*(modifydate+12),*(modifydate+13),*(modifydate+14),*(modifydate+15)
			   ,*(modifydate+16),*(modifydate+17),*(modifydate+18),*(modifydate+19)
			   
			   , 0x00, 0x00 //ExifIFD entries
			   , 0x00, 0x00, 0x00, 0x00 //Next IFD

			   //GPSinfor data '0x88'
			   , 0x04, 0x00 //Gps entries // entry count '2'
			   
			   , 0x04, 0x00 //TAG_GPSLONGITUDE
			   , 0x0A, 0x00 //DATA_FORMAT_RATIONAL 
			   , 0x03, 0x00, 0x00, 0x00 //length 3x8byte = 24byte.
			   , 0xBE, 0x00, 0x00, 0x00 //offset '0xB8'

			   , 0x01, 0x00 //TAG_GPSLATITUDEREF
			   , 0x02, 0x00 //DATA_FORMAT_ASCII_STR
			   , 0x02, 0x00, 0x00, 0x00 //length 2byte					   
			   , 0x00, 0x00, 0x00, 0x00 //value

			   , 0x02, 0x00 //TAG_GPSLATITUDE
			   , 0x0A, 0x00 //DATA_FORMAT_RATIONAL
			   , 0x03, 0x00, 0x00, 0x00 //length 3x8byte = 24byte.
			   , 0xD6, 0x00, 0x00, 0x00 //offset '0xC0'
			   
			   , 0x03, 0x00 //TAG_GPSLONGITUDEREF
			   , 0x02, 0x00 //DATA_FORMAT_ASCII_STR
			   , 0x02, 0x00, 0x00, 0x00 //length 2byte		  
			   , 0x00, 0x00, 0x00, 0x00 //value
								   
			   , 0x00, 0x00, 0x00, 0x00 //nextIFD

			   //data of GPS longitude					   
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   
			   //data of GPS latitude
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   , 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
			   };//246//
			   
	memcpy(enc_t->gpsdata, exifdata, EXIF_TOTAL_LENGTH);
}

#endif
#endif
#endif
