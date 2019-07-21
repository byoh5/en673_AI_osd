#ifndef __HTTPD_STRUCTS_H__
#define __HTTPD_STRUCTS_H__

#include "httpd.h"

/** This string is passed in the HTTP header as "Server: " */
#ifndef HTTPD_SERVER_AGENT
#define HTTPD_SERVER_AGENT "Eyenix/EN673 lwIP HTTP Server"
#endif

/** Set this to 1 if you want to include code that creates HTTP headers
 * at runtime. Default is off: HTTP headers are then created statically
 * by the makefsdata tool. Static headers mean smaller code size, but
 * the (readonly) fsdata will grow a bit as every file includes the HTTP
 * header. */
#ifndef LWIP_HTTPD_DYNAMIC_HEADERS
#define LWIP_HTTPD_DYNAMIC_HEADERS 1
#endif


#if LWIP_HTTPD_DYNAMIC_HEADERS
/** This struct is used for a list of HTTP header strings for various
 * filename extensions. */
typedef struct
{
  const char *extension;
  int headerIndex;
} tHTTPHeader;

/** A list of strings used in HTTP headers */
static const char * const g_psHTTPHeaderStrings[] =
{
 "Content-type: text/html\r\n\r\n",
 "Content-type: text/html\r\nExpires: Fri, 10 Apr 2008 14:00:00 GMT\r\nPragma: no-cache\r\n\r\n",
 "Content-type: image/gif\r\n\r\n",
 "Content-type: image/png\r\n\r\n",
 "Content-type: image/jpeg\r\n\r\n",
 "Content-type: image/bmp\r\n\r\n",
 "Content-type: image/x-icon\r\n\r\n",
 "Content-type: application/octet-stream\r\n\r\n",
 "Content-type: application/x-javascript\r\n\r\n",
 "Content-type: application/x-javascript\r\n\r\n",
 "Content-type: text/css\r\n\r\n",
 "Content-type: application/x-shockwave-flash\r\n\r\n",
 "Content-type: text/xml\r\n\r\n",
 "Content-type: text/plain\r\n\r\n",
 "HTTP/1.0 200 OK\r\n",
 "HTTP/1.0 404 File not found\r\n",
 "HTTP/1.0 400 Bad Request\r\n",
 "HTTP/1.0 501 Not Implemented\r\n",
 "HTTP/1.1 200 OK\r\n",
 "HTTP/1.1 404 File not found\r\n",
 "HTTP/1.1 400 Bad Request\r\n",
 "HTTP/1.1 501 Not Implemented\r\n",
 "Content-Length: ",
 "Connection: Close\r\n",
 "Connection: keep-alive\r\n",
 "Server: "HTTPD_SERVER_AGENT"\r\n",
 "\r\n<html><body><h2>404: The requested file cannot be found.</h2></body></html>\r\n",
 "HTTP/1.1 401 Unauthorized\r\n",
 "WWW-Authenticate: Basic realm=\""HTTPD_SERVER_AGENT"\"\r\n",
 "WWW-Authenticate: Digest realm=\""HTTPD_SERVER_AGENT"\", nonce=\"CjPk9mRqNuT25eRkajM09uTl9nM09uTl9nMz5OX25PZz==\",qop=\"auth,auth-int\",opaque=\"5ccc069c403ebaf9f0171e9517f40e41\",algorithm=MD5\r\n",
 "Content-type: text/html\r\nContent-Length: 219\r\n\r\n<html><head><meta charset=\"UTF-8\" /><title>Document Error: Unauthorized</title></head><body><h1>Access Error: 401 -- Unauthorized</h1><p>Authentication Error: Access Denied! Authorization required.</p></body></html>\r\n",
 "HTTP/1.1 500 Internal Server Error\r\n",
 "HTTP/1.1 415 Unsupported Media Type\r\n",
 "Content-type: application/octet-stream\r\nContent-disposition: attachment\r\n\r\n",
};

#ifdef __ONVIF__
#define HTTPD_ONVIF_GSOAP_SERVER_AGENT "gSOAP/2.8"
#define HTTP11_ONVIF_SOAP_URI "/onvif"
#define HTTP11_ONVIF_SOAP_CONTENTTYPE "Content-Type: application/soap+xml"
#define HTTP11_ONVIF_SOAP_SERVER "Server: "HTTPD_ONVIF_GSOAP_SERVER_AGENT"\r\n"
#endif

/* Indexes into the g_psHTTPHeaderStrings array */
#define HTTP_HDR_HTML           0  /* text/html */
#define HTTP_HDR_SSI            1  /* text/html Expires... */
#define HTTP_HDR_GIF            2  /* image/gif */
#define HTTP_HDR_PNG            3  /* image/png */
#define HTTP_HDR_JPG            4  /* image/jpeg */
#define HTTP_HDR_BMP            5  /* image/bmp */
#define HTTP_HDR_ICO            6  /* image/x-icon */
#define HTTP_HDR_APP            7  /* application/octet-stream */
#define HTTP_HDR_JS             8  /* application/x-javascript */
#define HTTP_HDR_RA             9  /* application/x-javascript */
#define HTTP_HDR_CSS            10 /* text/css */
#define HTTP_HDR_SWF            11 /* application/x-shockwave-flash */
#define HTTP_HDR_XML            12 /* text/xml */
#define HTTP_HDR_DEFAULT_TYPE   13 /* text/plain */
#define HTTP_HDR_OK             14 /* 200 OK */
#define HTTP_HDR_NOT_FOUND      15 /* 404 File not found */
#define HTTP_HDR_BAD_REQUEST    16 /* 400 Bad request */
#define HTTP_HDR_NOT_IMPL       17 /* 501 Not Implemented */
#define HTTP_HDR_OK_11          18 /* 200 OK */
#define HTTP_HDR_NOT_FOUND_11   19 /* 404 File not found */
#define HTTP_HDR_BAD_REQUEST_11 20 /* 400 Bad request */
#define HTTP_HDR_NOT_IMPL_11    21 /* 501 Not Implemented */
#define HTTP_HDR_CONTENT_LENGTH 22 /* Content-Length: (HTTP 1.1)*/
#define HTTP_HDR_CONN_CLOSE     23 /* Connection: Close (HTTP 1.1) */
#define HTTP_HDR_CONN_KEEPALIVE 24 /* Connection: keep-alive (HTTP 1.1) */
#define HTTP_HDR_SERVER         25 /* Server: HTTPD_SERVER_AGENT */
#define DEFAULT_404_HTML        26 /* default 404 body */
#define DEFAULT_HDR_UNAUTHORIZED        		27 /* default 401 body */
#define HTTP_HDR_BASIC_AUTHENTICATE 	28 /* AUTHENTICATE body */
#define HTTP_HDR_DIGEST_AUTHENTICATE 	29 /* AUTHENTICATE body */
#define DEFAULT_401_HTML        		30 /* default 401 body */
#define DEFAULT_500_HTML        		31/* default 401 body */
#define DEFAULT_415_HTML        		32/* default 415 body */
#define HTTP_HDR_BIN            33 /* application/octet-stream / AVI, MP4 */


/** A list of extension-to-HTTP header strings */
const tHTTPHeader g_psHTTPHeaders[] =											// static not req
{
 { "html", HTTP_HDR_HTML},
 { "htm",  HTTP_HDR_HTML},
 { "shtml",HTTP_HDR_SSI},
 { "shtm", HTTP_HDR_SSI},
 { "ssi",  HTTP_HDR_SSI},
 { "gif",  HTTP_HDR_GIF},
 { "png",  HTTP_HDR_PNG},
 { "jpg",  HTTP_HDR_JPG},
 { "bmp",  HTTP_HDR_BMP},
 { "ico",  HTTP_HDR_ICO},
 { "class",HTTP_HDR_APP},
 { "cls",  HTTP_HDR_APP},
 { "js",   HTTP_HDR_JS},
 { "ram",  HTTP_HDR_RA},
 { "css",  HTTP_HDR_CSS},
 { "swf",  HTTP_HDR_SWF},
 { "xml",  HTTP_HDR_XML},
 { "xsl",  HTTP_HDR_XML},
 { "avi",  HTTP_HDR_BIN},
 { "mp4",  HTTP_HDR_BIN}
};

#define NUM_HTTP_HEADERS (sizeof(g_psHTTPHeaders) / sizeof(tHTTPHeader))

#endif /* LWIP_HTTPD_DYNAMIC_HEADERS */

#if LWIP_HTTPD_SSI
static const char * const g_pcSSIExtensions[] = {
  ".html", ".htm", ".shtml", ".shtm", ".ssi", ".xml"
};
#define NUM_SHTML_EXTENSIONS (sizeof(g_pcSSIExtensions) / sizeof(const char *))
#endif /* LWIP_HTTPD_SSI */

#endif /* __HTTPD_STRUCTS_H__ */
