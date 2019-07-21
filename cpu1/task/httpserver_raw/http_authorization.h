#ifndef __HTTPD_AUTHORIZATION_H__
#define __HTTPD_AUTHORIZATION_H__

#include "httpd.h"

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION

#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_BASIC
#define HTTP11_BASIC_AUTORIZATION "Authorization: Basic "
#endif
#if LWIP_HTTPD_SUPPORT_AUTHORIZATION_DIGEST
#define HTTP11_DIGEST_AUTORIZATION "Authorization: Digest "
#endif

#define HTTP11_AUTORIZATION_LEN 21
#define CRLF "\r\n"

extern int HttpAuthorization(int isPost, char *data, int data_len);
#endif

#endif	/* __HTTPD_AUTHORIZATION_H__ */
