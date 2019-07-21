#include "dev.h"
#include "dev_inline.h"

#include <stdlib.h>
#include <time.h>

#include "enx_os.h"
#include "lwip/memp.h"
#include "lwip/tcp.h"
#include "lwip/inet.h"
#include "lwip/pbuf.h"
#include "lwip/tcp_impl.h"
#include "Base64.h"

#include "rtsp_cmd.h"
#include "rtp_over_rtsp.h"

#include "msg.h"

#if (RTSPoverHTTP==1)

#include "http_front.h"
#include "http_tunnel.h"
#include "share_rtspoverhttp.h"

#ifndef HTTPD_FRONT_DEBUG
#define HTTPD_FRONT_DEBUG         LWIP_DBG_ON
#endif

#define ENX_TUNNEL_DGB_COMMAND		DEF_OFF
#define ENX_TUNNEL_DGB_PROCESSING	DEF_OFF
#define ENX_TUNNEL_DGB_RESPONSE		DEF_OFF
#define ENX_TUNNEL_DGB_DEBUG		DEF_OFF
#define ENX_TUNNEL_DGB_ERROR		DEF_ON
#define ENX_TUNNEL_DGB_ENTEXT		DEF_OFF

#define tprintf(debug, fmt, args...) do { if((debug)&DEF_ON) printf("%04d %s : "fmt, __LINE__, __func__, ##args); } while(0);
#if (ENX_TUNNEL_DGB_ENTEXT==DEF_ON)
#define tunnel_enter()	printf("%04d %s : ENTER\r\n", __LINE__, __func__);
#define tunnel_exit()	printf("%04d %s : EXIT\r\n", __LINE__, __func__);
#else
#define tunnel_enter()
#define tunnel_exit()
#endif

/* Return values for http_send_*() */
#define HTTP_DATA_TO_SEND_BREAK    2
#define HTTP_DATA_TO_SEND_CONTINUE 1
#define HTTP_NO_DATA_TO_SEND       0

static tunneling_t rt_tunneling[MAX_TUNNELING];

tunneling_t *tunnel_find(hs_front_t *front, char const* sessionCookie)
{
	tunnel_enter();

	int i;
	tunneling_t *p_tunnel = (tunneling_t *)front->priv;

	front->p_tunnel = NULL; 
	for(i=0; i<MAX_TUNNELING; i++)
	{
		if(p_tunnel[i].is_valid && !strcmp(p_tunnel[i].sessionCookie, sessionCookie))
		{
			tprintf(ENX_TUNNEL_DGB_DEBUG, "Find\r\n");

			tunnel_exit();
			return &p_tunnel[i];
		}
	}
	tprintf(ENX_TUNNEL_DGB_ERROR, "Not find\r\n");

	tunnel_exit();
	return NULL;
}

/**
  @return 1 - adding ok
          0 - fail
*/
int tunnel_add(hs_front_t *front, char const* sessionCookie)
{
	tunnel_enter();

	int i;
	tunneling_t *p_tunnel = (tunneling_t *)front->priv;

	for(i=0; i<MAX_TUNNELING; i++)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "p_tunnel[%d].is_valid = %d added cookie %s cookie:%s\r\n", i, p_tunnel[i].is_valid, p_tunnel[i].sessionCookie, sessionCookie);
		if(!p_tunnel[i].is_valid)
		{
			strcpy(p_tunnel[i].sessionCookie, sessionCookie);
			p_tunnel[i].in_pcb = front->in_pcb;  //could is changed by handleHTTPCmd_TunnelingPOST 
			p_tunnel[i].out_pcb = front->out_pcb;
			p_tunnel[i].priv = front;
			front->p_tunnel = &p_tunnel[i];
			p_tunnel[i].is_valid = 1;

			tprintf(ENX_TUNNEL_DGB_DEBUG, "Tunnel add ok(cookie:%s)\r\n", sessionCookie);

			tunnel_exit();
			return DEF_OK;
		}
		else if(strcmp(p_tunnel[i].sessionCookie, sessionCookie) == 0)
		{
			p_tunnel[i].in_pcb = front->in_pcb;  //could is changed by handleHTTPCmd_TunnelingPOST 
			p_tunnel[i].out_pcb = front->out_pcb;
			p_tunnel[i].priv = front;
			front->p_tunnel = &p_tunnel[i];

			tprintf(ENX_TUNNEL_DGB_DEBUG, "Tunnel already added (cookie:%s)\r\n", sessionCookie);

			tunnel_exit();
			return DEF_OK;		
		}
	}

	tprintf(ENX_TUNNEL_DGB_ERROR, "Tunnel add fail\r\n");

	tunnel_exit();
	return DEF_FAIL;
}

/**
  @return 1 = delete ok
          0 = fail
*/
static int tunnel_del(hs_front_t *front)
{
	tunnel_enter();

	int i;
	if(front)
	{
		tunneling_t *p_tunnel = (tunneling_t *)front->priv;

		if(p_tunnel)
		{
			for(i=0; i<MAX_TUNNELING; i++)
			{
				if(p_tunnel[i].is_valid)
				{
					if(!strcmp(p_tunnel[i].sessionCookie, front->p_tunnel->sessionCookie))
					{
						p_tunnel[i].is_valid = 0;

						tprintf(ENX_TUNNEL_DGB_DEBUG, "Tunnel delete ok(cookie:%s)\r\n", front->p_tunnel->sessionCookie);

						strcpy(p_tunnel[i].sessionCookie,"");

						tunnel_exit();
						return 1;
					}
				}
			}
		}
	}

	tprintf(ENX_TUNNEL_DGB_ERROR, "Tunnel delete fail\r\n");

	tunnel_exit();
	return 0;
}

int dateHeader(char *buf, int size)
{
	tunnel_enter();

	struct tm *tmprint = gmtime((const time_t *)&gptMsgShare.TIME);
	int nSize = snprintf(buf, size, "Date: %s, %s %02d %04d %02d:%02d:%02d GMT\r\n", 
		strWeek[tmprint->tm_wday], strMonth[tmprint->tm_mon], tmprint->tm_mday, tmprint->tm_year+1900,
		tmprint->tm_hour, tmprint->tm_min, tmprint->tm_sec);

	tunnel_exit();
	return nSize;
}

void reset_request_buffer(hs_front_t *front)
{
	tunnel_enter();

	if(front)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		front->req_left = sizeof(front->req_buff);
		front->req_alreay_seen = 0;
		front->base64_remainings = 0;
		front->last_crlf = &front->req_buff[-3];
	}

	tunnel_exit();
}

//arg for input pcb change 
//GET TCP ARG 
void disable_rx_pcb(struct tcp_pcb *pcb, void *arg)
{
	tunnel_enter();

	tcp_arg(pcb, NULL);
	tcp_recv(pcb, NULL);
	tprintf(ENX_TUNNEL_DGB_DEBUG, "arg(free-mem:0x%08X)\r\n", arg);
	free(arg);

	tunnel_exit();
}

void handleHTTPCmd_notSupported(hs_front_t *front)
{
	tunnel_enter();

	char buffer[200] = {0};
	dateHeader(buffer, 196);
	front->resp_left = snprintf((char*)front->resp_buff, sizeof(front->resp_buff), "HTTP/1.1 405 Method Not Allowed\r\n%s\r\n\r\n", buffer);
	front->resp_pos = 0;

	tunnel_exit();
}

int handleHTTPCmd_TunnelingPOST(hs_front_t *front, char const* sessionCookie)
{
	tunnel_enter();

	hs_front_t *prev_front = NULL;
	// Use the "sessionCookie" string to look up the separate "RTSPClientConnection"
	// object that should have been used to handle
	// an earlier HTTP "GET" request:
	int i=0;
//	tunneling_t *p_tunnel = (tunneling_t *)front->priv;
	tunneling_t *p_tunnel;

	p_tunnel = tunnel_find(front, sessionCookie);
	if(p_tunnel != NULL)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		front->p_tunnel = &p_tunnel[i];
		prev_front = (hs_front_t *)p_tunnel[i].priv;
		front->out_pcb = prev_front->out_pcb;
	}

	if (front->p_tunnel == NULL)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		// There was no previous HTTP "GET" request; treat this "POST" request as bad:
		handleHTTPCmd_notSupported(front);
		front->IsActive = FALSE; // triggers deletion of ourself

		tunnel_exit();
		return FALSE;
	}

	//  pre_front don't anymore recevie.
	disable_rx_pcb(prev_front->in_pcb, prev_front);

	tunnel_exit();
	return TRUE;
}


void
handleHTTPCmd_TunnelingGET(hs_front_t *front, char const* sessionCookie)
{
	tunnel_enter();

//	tunnel_add(front, sessionCookie);

	// Construct our response:
	char buffer[200] = {0};
	dateHeader(buffer, 196);
	front->resp_left = snprintf((char *)front->resp_buff, FRONT_TX_BUFFER, 
							"HTTP/1.1 200 OK\r\n"
							"%s"
							"Cache-Control: no-cache\r\n"
							"Pragma: no-cache\r\n"
							"Content-Type: application/x-rtsp-tunnelled\r\n"
							"\r\n",
							buffer);
	front->resp_pos = 0;

	tunnel_exit();

	UNUSED(sessionCookie);
}

void handleHTTPCmd_StreamingGET(hs_front_t *front, char const * urlSuffix, char const* fullRequestStr)
{
	tunnel_enter();
	
	handleHTTPCmd_notSupported(front);

	tunnel_exit();
	
	UNUSED(urlSuffix);
	UNUSED(fullRequestStr);
}

hs_front_t *http_front_alloc(void)
{
	tunnel_enter();

	hs_front_t *front = (hs_front_t *)malloc(sizeof(hs_front_t));
	if (front) {
		front->priv = rt_tunneling;
		tprintf(ENX_TUNNEL_DGB_DEBUG, "front(alloc-mem:0x%08X)\r\n", front);
		front->pcb_type.type = PCB_HTTP_FRONT;
		front->is_connect = 1;
	} else {
		tprintf(ENX_TUNNEL_DGB_ERROR, "front alloc fail\r\n");
	}

	tunnel_exit();
	return front;
}

int parse_http_request(hs_front_t * front,
                   char* resultCmdName, u32 resultCmdNameMaxSize,
                   char* urlSuffix,     u32 urlSuffixMaxSize,
                   char* sessionCookie, u32 sessionCookieMaxSize,
                   char* acceptStr,     u32 acceptStrMaxSize)
{
	tunnel_enter();

	// Check for the limited HTTP requests that we expect for specifying RTSP-over-HTTP tunneling.
	// This parser is currently rather dumb; it should be made smarter #####
	char const* reqStr = (char const*)front->req_buff;
	unsigned const reqStrSize = front->req_alreay_seen;

	// Read everything up to the first space as the command name:
	int parseSucceeded = FALSE;
	unsigned i;
	for (i = 0; i < resultCmdNameMaxSize-1 && i < reqStrSize; ++i)
	{
		char c = reqStr[i];
		if (c == ' ' || c == '\t')
		{
			parseSucceeded = TRUE;
			break;
		}

		resultCmdName[i] = c;
	}

	resultCmdName[i] = '\0';

	if (!parseSucceeded)
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "parseSucceeded fail\r\n");
		tunnel_exit();
		return FALSE;
	}

	// Look for the string "HTTP/", before the first \r or \n:
	parseSucceeded = FALSE;
	for (; i < reqStrSize-5 && reqStr[i] != '\r' && reqStr[i] != '\n'; ++i)
	{
		if (reqStr[i] == 'H' && reqStr[i+1] == 'T' && reqStr[i+2]== 'T' && reqStr[i+3]== 'P' && reqStr[i+4]== '/')
		{
			i += 5; // to advance past the "HTTP/"
			parseSucceeded = TRUE;
			break;
		}
	}
	if (!parseSucceeded)
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "parseSucceeded fail\r\n");
		tunnel_exit();
		return FALSE;
	}

	// Get the 'URL suffix' that occurred before this:
	unsigned k = i - 6;
	while (k > 0 && reqStr[k] == ' ') --k; // back up over white space
	unsigned j = k;
	while (j > 0 && reqStr[j] != ' ' && reqStr[j] != '/') --j;
	// The URL suffix is in position (j,k]:
	if (k - j + 1 > urlSuffixMaxSize)
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "k - j + 1 <= urlSuffixMaxSize fail\r\n");
		tunnel_exit();
		return FALSE; // there's no room>
	}
	unsigned n = 0;
	while (++j <= k) urlSuffix[n++] = reqStr[j];
	urlSuffix[n] = '\0';

	// Look for various headers that we're interested in:
	lookForHeader("x-sessioncookie", &reqStr[i], reqStrSize-i, sessionCookie, sessionCookieMaxSize);
	lookForHeader("Accept", &reqStr[i], reqStrSize-i, acceptStr, acceptStrMaxSize);

	tunnel_exit();
	return TRUE;
}

/**
 * The connection shall be actively closed (using RST to close from fault states).
 * Reset the sent- and recv-callbacks.
 *
 * @param pcb the tcp pcb to reset callbacks
 * @param hs connection state to free
 */
#if 0
static err_t
http_front_close_or_abort_conn(struct tcp_pcb *pcb, hs_front_t *front, u8_t abort_conn)
{
  err_t err;
  
  tcp_arg(pcb, NULL);
  tcp_recv(pcb, NULL);
  tcp_err(pcb, NULL);
  tcp_poll(pcb, NULL, 0);
  tcp_sent(pcb, NULL);
  
  if (front )
    mem_free(front);
  
  if (abort_conn) {
    tcp_abort(pcb);
    return ERR_OK;
  }
  
  err = tcp_close(pcb);
  
  if (err != ERR_OK) {
    LWIP_DEBUGF(HTTPD_FRONT_DEBUG, ("Error %d closing %p\n", err, (void*)pcb));      
    //need tcp poll          
  }
  return err;
}
#endif

/** Call tcp_write() in a loop trying smaller and smaller length
 *
 * @param pcb tcp_pcb to send
 * @param ptr Data to send
 * @param length Length of data to send (in/out: on return, contains the
 *        amount of data sent)
 * @param apiflags directly passed to tcp_write
 * @return the return value of tcp_write
 */
static err_t http_front_write(struct tcp_pcb *pcb, const void* ptr, u16_t *length, u8_t apiflags)
{
	tunnel_enter();

	u16 len, max_len;
	err_t err;

	if(pcb == NULL)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "pcb == NULL\r\n");
	}

	LWIP_ASSERT("length != NULL", length != NULL);
	len = *length;
	if (len == 0)
	{
		tunnel_exit();
		return ERR_OK;
	}
	/* We cannot send more data than space available in the send buffer. */
	max_len = tcp_sndbuf(pcb);
	if (max_len == 0)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "max_len %d len %d\r\n", max_len, len);
		*length = 0;
		tunnel_exit();
		return ERR_OK;
	}
	if (max_len < len)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "max_len %d len %d\r\n", max_len, len);
		len = max_len;
	}

	do {
		tprintf(ENX_TUNNEL_DGB_DEBUG, "Trying go send %d bytes\r\n", len);
		err = tcp_write(pcb, ptr, len, apiflags);
		if (err == ERR_MEM)
		{
			if ((tcp_sndbuf(pcb) == 0) || (tcp_sndqueuelen(pcb) >= TCP_SND_QUEUELEN))
			{	/* no need to try smaller sizes */
				tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! no need to try smaller sizes\r\n");
				len = 1;
			}
			else
			{
				tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! len /= 2\r\n");
				len /= 2;
			}
			//printf("Send failed, trying less (%d bytes)\n", len);
		}
	} while ((err == ERR_MEM) && (len > 1));  

	if (err == ERR_OK)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "Sent %d bytes\r\n", len);
		tcp_output(pcb);
		*length = len;
	}
	else
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "ERROR! Send failed with err %d (\"%s\")\r\n", err, lwip_strerr(err));
		*length = 0;
	}

	tunnel_exit();
	return err;
}

/**
  @return len sent.
*/
int
http_front_send(struct tcp_pcb *pcb, hs_front_t * front)
{
	tunnel_enter();

	if(front == NULL)
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "front == NULL\r\n");
		tunnel_exit();
		return 0;
	}

	if(front->resp_left == 0)
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "front->resp_left == 0\r\n");
		tunnel_exit();
		return 0;
	}

	tprintf(ENX_TUNNEL_DGB_DEBUG, "pos(%d) len(%d)\r\n", front->resp_pos, front->resp_left);
	u16 len = front->resp_left;
	http_front_write(pcb, &front->resp_buff[front->resp_pos], &len, TCP_WRITE_FLAG_COPY);
	front->resp_left -= len;
	front->resp_pos += len;

	tunnel_exit();
	return len;
}

//static int
//http_front_send(struct tcp_pcb *pcb, hs_front_t *front)
//{
//  return http_front_sendBytes(pcb,front);
//}

/**
  @return 
    length of decoded base64 msg
*/
static int 
get_decode_base64(hs_front_t *front, u8 *ptr, u32 new_bytes_read)
{
  // But first, we remove any whitespace that may be in the input data:      
  u32 toIndex   = 0;
  u32 fromIndex = 0;

  for (fromIndex=0; fromIndex < new_bytes_read; ++fromIndex) 
  {
    char c = ptr[fromIndex];
    // not 'whitespace': space,tab,CR,NL
    if (!(c == ' ' || c == '\t' || c == '\r' || c == '\n'))
    { 
       ptr[toIndex++] = c;
    }
  }      
  new_bytes_read = toIndex;
  
  u32 numBytesToDecode = front->base64_remainings + new_bytes_read;
  u32 newBase64RemainderCount = numBytesToDecode%4;
  
  numBytesToDecode -= newBase64RemainderCount;
  
  if (numBytesToDecode > 0) {
    ptr[new_bytes_read] = '\0';
    u32 decodedSize;
    unsigned char* decodedBytes = base64_decode((char const*)(ptr-front->base64_remainings), numBytesToDecode, &decodedSize);

    // Copy the new decoded bytes in place of the old ones
    // (we can do this because there are fewer decoded bytes than original):
    unsigned char* to = ptr-front->base64_remainings;
    unsigned i,j;
    for (i = 0; i < decodedSize; ++i) *to++ = decodedBytes[i];
  
    // Then copy any remaining (undecoded) bytes to the end:
    for (j = 0; j < (unsigned)front->base64_remainings; ++j) 
      *to++ = (ptr-front->base64_remainings+numBytesToDecode)[j];
  
    new_bytes_read = decodedSize - front->base64_remainings + newBase64RemainderCount;
    // adjust to allow for the size of the new decoded data (+ remainder)
    if(decodedBytes)
      free(decodedBytes);
  }
  
  front->base64_remainings = newBase64RemainderCount; 
  
  return new_bytes_read;
}

/**
  @return 
    TRUE  : HTTP request for HTTP TUNNELLING
    FALSE : FAILED.
*/
int
handle_http_tunneling_session(hs_front_t *front)
{
	tunnel_enter();

  //check for a special case: HTTP commands (for setting up RTSP-over-HTTP tunneling):
  int parseSucceeded = FALSE;
  int isValidHTTPCmd = TRUE;
  
  char cmdName[RTSP_PARAM_STRING_MAX];
  char urlSuffix[RTSP_PARAM_STRING_MAX];
  char sessionCookie[RTSP_PARAM_STRING_MAX];
  char acceptStr[RTSP_PARAM_STRING_MAX];
  
  *front->last_crlf = '\0'; // temporarily, for parsing
  tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
  parseSucceeded = parse_http_request(front,
                     cmdName,       sizeof(cmdName),
                     urlSuffix,     sizeof(urlSuffix),
                     sessionCookie, sizeof(sessionCookie),
                     acceptStr,     sizeof(acceptStr));
  tprintf(ENX_TUNNEL_DGB_DEBUG, "cmdName(%s) urlSuffix(%s)\r\n", cmdName, urlSuffix);
  handleHTTPCmd_notSupported(front);
  tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
  goto next;                   
  // Check that the HTTP command is valid for RTSP-over-HTTP tunneling: There must be a 'session cookie'.                   
  if (parseSucceeded) 
  {
    if (sessionCookie[0] == '\0')
    {
      // There was no "x-sessioncookie:" header.  If there was an "Accept: application/x-rtsp-tunnelled" header,
      // then this is a bad tunneling request.  Otherwise, assume that it's an attempt to access the stream via HTTP.
      if (strcmp(acceptStr, "application/x-rtsp-tunnelled") == 0) {
        isValidHTTPCmd = FALSE;
      } else {
        handleHTTPCmd_StreamingGET(front, urlSuffix,(char const*)front->req_buff);
      }
    }
    else if (strcmp(cmdName, "GET") == 0)
    {
      handleHTTPCmd_TunnelingGET(front, sessionCookie);
    } 
    else if (strcmp(cmdName, "POST") == 0) 
    {
      // We don't respond to the "POST" command
      if( handleHTTPCmd_TunnelingPOST(front,sessionCookie))
      {
        int nCase = 0;
#ifdef __H264__
        nCase |= (strcmp(urlSuffix, "/"H264URL) == 0);
#ifdef __H264_2CH__
        nCase |= (strcmp(urlSuffix, "/"HSUBURL) == 0);
#endif
#endif
#ifdef __JPEG__
        nCase |= (strcmp(urlSuffix, "/"JPEGURL) == 0);
#endif
        if(nCase)
        {
          rtsp_con_t *prtsp = rtsp_add_user(front->out_pcb);
          front->prtsp_conn = prtsp;
          prtsp->ctx = front; 
          tcp_arg(front->out_pcb, prtsp);
          strcpy((char *)front->urlSuffix, urlSuffix);
          if(prtsp == NULL)
          {
            // needed handling error
          }
        }  
        goto exit;
      }
    }
  }
  else
    isValidHTTPCmd = FALSE;
next:
	http_front_send(front->out_pcb, front);
exit:
	tunnel_exit();
	return isValidHTTPCmd;
}


/**
  close http get-post tunnel  
*/
static void http_close_tunnelling(hs_front_t *front)
{
	tunnel_enter();

	err_t err;  
	if(!front)
	{
		tunnel_exit();
		return;
	}

	//we alse close post port for tunnel in port  
	if(front->in_pcb)
	{ 
		tunnel_port_del(front->in_pcb);     
		tcp_arg(front->in_pcb,  NULL);
		tcp_sent(front->in_pcb, NULL);
		tcp_recv(front->in_pcb, NULL);
		tcp_err(front->in_pcb,  NULL);
		err = tcp_close(front->in_pcb);
		tprintf(ENX_TUNNEL_DGB_DEBUG, "in_pcb err(%d)\r\n", err);
	}

	if(front->out_pcb)
	{  
		//we should close get port for tunnel out port
		tunnel_port_del(front->out_pcb);
		rtsp_del_user(front->out_pcb);
		tcp_arg(front->out_pcb, NULL);
		tcp_sent(front->out_pcb, NULL);
		tcp_recv(front->out_pcb, NULL);
		tcp_err(front->out_pcb,NULL);
		err = tcp_close(front->out_pcb);
		tprintf(ENX_TUNNEL_DGB_DEBUG, "out_pcb err(%d)\r\n", err);
	}

	tprintf(ENX_TUNNEL_DGB_DEBUG, "front(free?-0x%08X)\r\n", front);
	tunnel_del(front);
	front->is_connect = 0;
	//free(front);

	tunnel_exit();
}

static void handler_rtsp_cmd(hs_front_t *front, int length)
{
	tunnel_enter();

	err_t parsed = ERR_ABRT;
	rtsp_con_t *prtsp_conn = front->prtsp_conn;
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	int teardown = 0;

	front->req_buff[length] = 0;
#if (ENX_TUNNEL_DGB_RESPONSE==DEF_ON)
	hexDump(__func__, front->req_buff, length);
#endif

	if(prtsp_ses->video_type == 0xFFFFFFFF)
	{
		prtsp_ses->cur_state = STATE_INIT;

#ifdef __H264__
		if(!strcmp((char *)front->urlSuffix, "/"H264URL))
		{
			prtsp_ses->video_type = RTSP_H264;
		}
#ifdef __H264_2CH__
		else if(!strcmp((char *)front->urlSuffix, "/"HSUBURL))
		{
			prtsp_ses->video_type = RTSP_H264SUB;
		}
#endif
#ifdef __JPEG__
		else
#endif
#endif
#ifdef __JPEG__
		if(!strcmp((char *)front->urlSuffix, "/"JPEGURL))
		{
			prtsp_ses->video_type = RTSP_MJPEG;
		}
#endif
    
		// which netif?
		prtsp_ses->nif = NULL;
		struct netif *nif = netif_list;
		for (nif = netif_list; nif != NULL; nif = nif->next)
		{
			if (nif->ip_addr.addr == front->in_pcb->local_ip.addr)
			{
				prtsp_ses->nif = nif;
				break;
			}
		}

		gptMsgShare.rtsp_conn_info[prtsp_conn->valid-1].video_type = prtsp_ses->video_type;
	}    

	// Processing
#if 1
	struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, length, PBUF_POOL);
	pbuf_take(p, front->req_buff, length);
	parsed = rtsp_session_proc(prtsp_conn, &p, &teardown);

	if (prtsp_ses->rtp_type == RTP_TCP) {
		prtsp_ses->rtp_type = RTP_HTTP;
		gptMsgShare.rtsp_conn_info[prtsp_conn->valid-1].rtp_type = RTP_HTTP;
	}

#if RTSPD_SUPPORT_REQUESTLIST
	if (parsed != ERR_INPROGRESS) {
		// request fully parsed or error
		if (prtsp_conn->req != NULL) {
			pbuf_free(prtsp_conn->req);
			prtsp_conn->req = NULL;
		}
	}
#else // RTSPD_SUPPORT_REQUESTLIST
	if (p != NULL) {
		pbuf_free(p);
	}
#endif // RTSPD_SUPPORT_REQUESTLIST

	if (parsed == ERR_OK) {
		if (prtsp_ses->out_size > 0) {
			tcp_write(front->out_pcb, (const void*)prtsp_ses->out_buffer, prtsp_ses->out_size, TCP_WRITE_FLAG_COPY);
			tcp_output(front->out_pcb);
		}
		if (teardown == 2) {
			printf("call : rtsp_recv : Close(bye msg)\r\n");
			http_close_tunnelling(front);
		}
	} else if (parsed == ERR_ARG) {
		printf("call : rtsp_recv : ERR_ARG, Close\r\n");
		http_close_tunnelling(front);
	}
#else
	snd_len = rtsp_session_proc(prtsp_conn, front->req_buff, length, &teardown);
	if(snd_len > 0)
	{
		tcp_write(front->out_pcb, (const void*)prtsp_ses->out_buffer, snd_len, TCP_WRITE_FLAG_COPY);
		tcp_output(front->out_pcb);
	}
	tprintf(ENX_TUNNEL_DGB_DEBUG, "teardown(%d)\r\n", teardown);
	if(teardown == 2)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "\r\n");
		http_close_tunnelling(front);
	}
#endif
	tunnel_exit();
}

void http_front_handle_request(struct tcp_pcb *pcb, hs_front_t *front, u32 new_bytes_read)
{
	tunnel_enter();

	u32 num_byte_remaining = 0;
	u8 *tmp_ptr;
	int tunneling = FALSE;

	do {
		if (new_bytes_read >= front->req_left) 
		{
			//In first time, we don't come here.
			// Either the client socket has died, or the request was too big for us.
			// Terminate this connection:
			front->IsActive = FALSE;    
		}

		u32 end_of_msg = FALSE;
		u8 *ptr = &front->req_buff[front->req_alreay_seen];

		tprintf(ENX_TUNNEL_DGB_DEBUG, "inpcb(%d)outpcb, num_byte_remaining(%d)\r\n", front->in_pcb != front->out_pcb, num_byte_remaining);
		if(front->in_pcb != front->out_pcb && num_byte_remaining == 0)
		{
			// We're doing RTSP-over-HTTP tunneling, and input commands are assumed 
			//to have been Base64-encoded.
			// We therefore Base64-decode as much of this new data as we can 
			//(i.e., up to a multiple of 4 bytes).
			new_bytes_read = get_decode_base64(front, ptr, new_bytes_read);
			tunneling = TRUE;
		}
		tprintf(ENX_TUNNEL_DGB_DEBUG, "tunneling(%d)\r\n", tunneling);

		unsigned requestSize;

		tmp_ptr = front->last_crlf;

		// no more Base-64 bytes remain to be read/decoded          
		if(front->base64_remainings == 0)
		{  
			//look for the end of the message : <CR><LF><CR><LF>
			if (tmp_ptr < front->req_buff)  
				tmp_ptr = front->req_buff;

			while(tmp_ptr < &ptr[new_bytes_read-1])
			{
				if (*tmp_ptr == '\r' && *(tmp_ptr+1) == '\n')
				{
					if (tmp_ptr - front->last_crlf == 2) // This is it: 
					{ 
						end_of_msg = 1;
						break;
					}
					front->last_crlf = tmp_ptr;
				}
				++tmp_ptr;
			}
		}

		front->req_left -= new_bytes_read;
		front->req_alreay_seen += new_bytes_read;

		// subsequent reads will be needed to complete the request
		if(!end_of_msg)
			break;

		front->req_buff[front->req_alreay_seen] = '\0';   

		if(tunneling == TRUE)
		{
			tprintf(ENX_TUNNEL_DGB_DEBUG, "new_bytes_read(%d)\r\n", new_bytes_read);
			handler_rtsp_cmd(front, new_bytes_read);
		}
		//    else
		//      handle_http_tunneling_session(front);    

		// Check whether there are extra bytes remaining in the buffer,
		// after the end of the request (a rare case).
		// If so, move them to the front of our buffer, and keep processing it,
		// because it might be a following, pipelined request.
		//
		requestSize = (front->last_crlf+4-front->req_buff);
		num_byte_remaining = front->req_alreay_seen - requestSize;
		reset_request_buffer(front); // to prepare for any subsequent request

		if (num_byte_remaining > 0)
		{
			tprintf(ENX_TUNNEL_DGB_DEBUG, "Remaining(%d)\r\n", num_byte_remaining);
			memmove(front->req_buff, &front->req_buff[requestSize], num_byte_remaining);
			new_bytes_read = num_byte_remaining;
		}
		break;
	}while(num_byte_remaining > 0); 

	tunnel_exit();
	UNUSED(pcb);
}

////
void http_front_err(void *arg, err_t err)
{
	tunnel_enter();
	hs_front_t *front = arg;
	
	if (arg != NULL) {
		switch (err) {
			case ERR_ABRT:
			case ERR_RST:
			case ERR_CLSD:
			case ERR_CONN:
				tunnel_exit();
				
				http_close_tunnelling(front);
				break;
		}
	}
	
	tunnel_exit();
}

err_t
http_front_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	tunnel_enter();

	hs_front_t *front = arg;
	struct pbuf *q;
	int byteRead = 0;

	if(p)
	{
		tprintf(ENX_TUNNEL_DGB_DEBUG, "err(%d) size(%d)\r\n", err, p->tot_len);
	}
	else
	{
		tprintf(ENX_TUNNEL_DGB_ERROR, "err(%d) p==NULL\r\n", err);
		
		http_close_tunnelling(front);
		tunnel_exit();
		return ERR_OK;
	}

	for(q=p; q != NULL; q=q->next)
	{
		memcpy(&front->req_buff[front->req_alreay_seen+byteRead], q->payload, q->len);
		byteRead += q->len;
	}

//	hexDump("req_buff", (char *)front->req_buff, byteRead);

	/* Inform TCP that we have taken the data. */
	if(p)
	{
		tcp_recved(pcb, p->len);
		pbuf_free(p);
		pcb->flags &= ~(TF_ACK_DELAY | TF_ACK_NOW);
		pcb->flags |= TF_ACK_NOW; //diable delayed ack function
		front->req_buff[front->req_alreay_seen+byteRead+1] = 0;

		http_front_handle_request(pcb, front, byteRead);
	}

	if (front->is_connect == 0) {
		tprintf(ENX_TUNNEL_DGB_DEBUG, "front(free-mem:0x%08X)\r\n", front);
		free(front);
		front = NULL;
	}

	tunnel_exit();
	return ERR_OK;
	UNUSED(err);
}
#endif
