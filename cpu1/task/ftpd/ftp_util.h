#ifndef _FTP_UTIL_H_
#define _FTP_UTIL_H_

/* Typedefs */
typedef unsigned char     UINT8;  		/*unsigned 8 bit definition */
typedef unsigned short    UINT16; 		/*unsigned 16 bit definition*/
typedef unsigned long     UINT32; 		/*unsigned 32 bit definition*/
typedef signed char       INT8;   		/*signed 8 bit definition */
typedef short      		  INT16;  		/*signed 16 bit definition*/
typedef long int    	  INT32;  		/*signed 32 bit definition*/

// UTIL
#define STRING_END				"\r\n"
#define NETCONN_RCV_NETBUFFER	1
#define str_len(x)				sizeof(x)-1
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

extern int strchangnull(char *buf, const char *erase);

extern int strDriverChang(char *buf, const char *driver, const char *chgdriver);

extern void FTP_strDriverChange(char* strPath, eFTP_Driver type);

extern char* strchrnul(const char *s, int c);

#endif
