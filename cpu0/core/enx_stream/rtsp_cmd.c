/* Copyright (c) 2013-16 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#include "dev.h"
#include "dev_inline.h"
#include "msg.h"

#include <time.h>

#include "Base64.h"
#include "lwip/inet.h"
#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "enx_os.h"
#include "enx_video.h"

#include "rtsp_control.h"
#include "rtsp_cmd.h"
#include "rtp_over_rtsp.h"
#include "rtcp_recv.h"
#include "rtp_hdr_gen.h"
#include "rtp_audio.h"

volatile char *gUserId;
volatile char *gUserPw;

void 
rtp_close_session(rtp_session_t *rtp_s, int rtp_type);

/**
 * macro define
*/
#define RTSP_SDP_URL_VIDEO "video"
#define RTSP_SDP_URL_AUDIO "audio"
#define RTSP_SDP_URL_BACKCHANNEL "back"
#define RTSP_SDP_URL_METADATA "meta"

//rtsp lib
void parse_request_headers( char* msg, char* hd, char* hdcontent )
{
	char *tmp, p[READBUFFER];
	char rn[3];
	char header[24];
	char cmp[] = "\r\n";
	int i;
	int len = enx_os_strlen(msg);

	tmp = p;

	strcpy(tmp, msg);

	for(i=0; i<len; i++)
	{
		enx_os_strncpy(rn, tmp, 2);
		rn[2] = '\0';

		while(enx_os_strcmp(rn, cmp) && i<len)
		{
			tmp++; // find the end of a line
			i++;
			enx_os_strncpy(rn, tmp, 2);
			rn[2] = '\0';
		}

		tmp += 2;
		i += 2;

		while( *tmp == ' ' )
		{
			tmp++; // eliminate white space in the string
			i++;
		}

		enx_os_strncpy(header, tmp, strlen(hd));
		header[ strlen(hd) ] = '\0';

		if(!enx_os_strcmp(header, hd))
			break;
	}

	if( i >= len )
	{
		*hdcontent = 0;
		return;
	}

	tmp += enx_os_strlen( hd );
	while( *tmp == ' ' || *tmp == ':' ) tmp++;

	i = 0;
	while( tmp[i] != '\r' ) i++;
	tmp[i] = '\0';
	enx_os_strcpy( hdcontent, tmp );
}

int parse_rtsp_cmd(rtsp_session_t *prtsp_ses, char* cmd)
{
	//parse cmd with headers
#define RTSP_CMD_STRING_SESSION "Session: "
#define RTSP_CMD_STRING_SESSION_LEN 9
#define RTSP_CMD_STRING_CSEQ "CSeq: "
#define RTSP_CMD_STRING_CSEQ_LEN 6

	char *datacheck;
	char p[CMSGLEN] = {0};
	int data_len = strlen(cmd);
	if((datacheck = strnstr(cmd, RTSP_CMD_STRING_SESSION, data_len)) != NULL)
	{
 		int index = strcspn(datacheck, CRLF);
		if(index != 0)
		{
			index -= RTSP_CMD_STRING_SESSION_LEN;
			datacheck += RTSP_CMD_STRING_SESSION_LEN;
			strncpy(p, datacheck, index);
			p[index] = 0;
			prtsp_ses->session = atoi(p);
//			printf("prtsp_ses->session : [%s] : %d\r\n", p, prtsp_ses->session);
		}
	}

	if((datacheck = strnstr(cmd, RTSP_CMD_STRING_CSEQ, data_len)) != NULL)
	{
		int index = strcspn(datacheck, CRLF);
		if(index != 0)
		{
			index -= RTSP_CMD_STRING_CSEQ_LEN;
			datacheck += RTSP_CMD_STRING_CSEQ_LEN;
			strncpy(p, datacheck, index);
			p[index] = 0;
			prtsp_ses->seq = atoi(p);
//			printf("prtsp_ses->seq : [%s] : %d\r\n", p, prtsp_ses->seq);
		}
	}

#ifdef __ONVIF__
#if (ONVIF_BACKCHANNEL==1)
#define RTSP_BACKCHANNEL_REQUIRED "www.onvif.org/ver20/backchannel\r\n"
	data_len = strlen(cmd);
	if ((datacheck = strnstr(cmd, RTSP_BACKCHANNEL_REQUIRED, data_len)) != NULL) {
		prtsp_ses->setup_backch = DEF_ON;
	} else {
		prtsp_ses->setup_backch = DEF_OFF;
	}
#endif
#endif

	return 1;
}

#if RTSP_SUPPORT_AUTHORIZATION
#include "lwip/mem.h"
int parse_rtsp_auth(rtsp_session_t *prtsp_ses, char* cmd)
{
#define RTSP_BASIC_AUTORIZATION "Authorization: Basic "
#define RTSP_AUTORIZATION_LEN 21

	int data_len = strlen(cmd);

	char *datacheck;
	if((datacheck = strnstr(cmd, RTSP_BASIC_AUTORIZATION, data_len)) != NULL)
	{
		int index, nonce_len;
		char *datacpy;
		char *pIdpassBase64;
		char *baseDecodeString;
		char *id, *pass;

		index = strcspn(datacheck, CRLF);
		datacpy = (char *)malloc((mem_size_t)index);
		strncpy(datacpy, datacheck, index);
		datacpy[index] = 0;

		pIdpassBase64 = datacpy + RTSP_AUTORIZATION_LEN;
		baseDecodeString = (char *)base64_decode((const char *)pIdpassBase64, (size_t)strlen(pIdpassBase64), (size_t *)&nonce_len);
		if(baseDecodeString == NULL)
		{
			free(datacpy);
			prtsp_ses->login = 0;
			return 0;
		}
		baseDecodeString[nonce_len] = 0;

		index = strcspn(baseDecodeString, ":");
		id = baseDecodeString;
		pass = &(id[index]) + 1;

		if((strncmp((const char *)gUserId, id, index) != 0) || (strcmp((const char *)gUserPw, pass) != 0))
		{
			prtsp_ses->login = 0;
		}
		else
		{
			prtsp_ses->login = 1;
		}

		free(datacpy);
		free(baseDecodeString);
	}
	else
	{
		prtsp_ses->login = 0;
	}
	return prtsp_ses->login;
}
#endif

// parse SETUP, PLAY etc. call parseRTSPcmd
void parse_rtsp_msg(rtsp_session_t *prtsp_ses, char* msg)
{
	char *tmpmsg = msg;
	while(*tmpmsg == ' ') tmpmsg++;

//	hexDump(__func__ , msg, strlen(msg));

	prtsp_ses->session = 0;
	prtsp_ses->seq = 0xFFFFFFFF;

	/* dispatch based on first character */
	prtsp_ses->cmd = 0xFF;
	switch(*tmpmsg)
	{
		case 'O': // possible OPTIONS
			if(!strncmp (tmpmsg, "OPTIONS", 7))
			{
				prtsp_ses->cmd = OPTIONS;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			break;

		case 'D': // possible DESCRIBE
			if(!strncmp (tmpmsg, "DESCRIBE", 8))
			{
				prtsp_ses->cmd = DESCRIBE;
				parse_rtsp_cmd(prtsp_ses, msg);
#if RTSP_SUPPORT_AUTHORIZATION
				parse_rtsp_auth(prtsp_ses, msg);
#endif
			}
			break;

		case 'S': // possible SETUP, SET_PARAMETER
			if(!strncmp (tmpmsg, "SETUP", 5))
			{
				prtsp_ses->cmd = SETUP;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			else if(!strncmp(tmpmsg, "SET_PARAMETER", 13))
			{
				prtsp_ses->cmd = SET_PARAMETER;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			break;

		case 'P': // possible PLAY OR PAUSE
			if(!strncmp (tmpmsg, "PLAY", 4))
			{
				prtsp_ses->cmd = PLAY;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			else if(!strncmp(msg, "PAUSE", 5))
			{
				prtsp_ses->cmd = PAUSE;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			break;

		case 'T': // possible TEARDOWN
			if(!strncmp(tmpmsg, "TEARDOWN", 8))
			{
				prtsp_ses->cmd = TEARDOWN;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			break;

		case 'G':
			if(!strncmp(tmpmsg, "GET_PARAMETER", 13))
			{
				prtsp_ses->cmd = GET_PARAMETER;
				parse_rtsp_cmd(prtsp_ses, msg);
			}
			break;

		case '$':
			prtsp_ses->cmd = RTSP_INTERLEAVED;
			break;

		default:
			// RTCP가 이리로 올 수 있다.
			// TCP와 UDP가 port 다르다.
			prtsp_ses->cmd = ERROR;
			break;
	}
}

/**

*/
int build_sdp(rtsp_con_t *prtsp_conn, char *r)
{
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	uint32 video_type = prtsp_ses->video_type;
	uint32 mediaformat = prtsp_ses->mediaformat;
//	char strLocalIP[16] = {0};
	char strRemoteIP[16] = {0};
	char strRTSPURL[128] = {0};
	char strBuff[1024] = {0};

//	snprintf(strLocalIP, 16, "%IP", prtsp_conn->tpcb->local_ip.addr);
	snprintf(strRemoteIP, 16, "%IP", prtsp_conn->tpcb->remote_ip.addr);
//	printf("SDP IP : Loc(%s) -> Rem(%s)\r\n", strLocalIP, strRemoteIP);
#if 0
	snprintf(strRTSPURL, 127, "rtsp://%s:%d/", strLocalIP, gptMsgShare.RtspPort);
	switch (video_type) {
#ifdef __H264__
		case RTSP_H264: 	strcat(strRTSPURL, H264URL);	break;
#ifdef __H264_2CH__
		case RTSP_H264SUB:	strcat(strRTSPURL, HSUBURL);	break;
#endif
#endif
#ifdef __JPEG__
		case RTSP_MJPEG:	strcat(strRTSPURL, JPEGURL);	break;
#endif
		default:
			printf("%s(%d) : Video Type Error(%d)!!!!\r\n", __func__, __LINE__, video_type);
			break;
	}
#else
	snprintf(strRTSPURL, 127, "%s", prtsp_ses->rtsp_uri);
#endif

	const char* sdpLine =	"v=0\r\n"
							"o=- 0 0 IN IP4 %s\r\n"
							"s=Session streamed by \"Eyenix Live Stream\"\r\n"
							"i=%s\r\n"
							"c=IN IP4 0.0.0.0\r\n"
							"t=0 0\r\n"
							"a=tool:Eyenix Media Server\r\n"
							"a=control:%s\r\n"
							"a=range:npt=0-\r\n"
							"a=x-qt-text-nam:Session streamed by \"Eyenix Live Stream\"\r\n"
							"a=x-qt-text-inf:%s\r\n";

#ifdef __H264__
	if(video_type == RTSP_H264
#ifdef __H264_2CH__
		|| video_type == RTSP_H264SUB)
#else
		)
#endif
	{
		const char* sdpH264Line =	"m=video 0 RTP/AVP %u\r\n"	// mediaType(video, audio), port(def 0), payloadType
									"b=AS:10240\r\n"
									"a=rtpmap:%u H264/90000\r\n"// payloadType, payloadFormatName(H264, JPEG, etc..), TimestampFrequency(H264 def 90kHZ, JPEG...?),
									"a=control:%s/%s\r\n"	// track1 ~ track?? => session 당 1
									"a=recvonly\r\n"
//									"a=fmtp:96 packetization-mode=1;profile-level-id=420029;sprop-parameter-sets=Z0IAKZWgFAFsQA==,aM4eDA==\r\n" // sps(720), pps(q25)
//									"a=fmtp:96 packetization-mode=1;profile-level-id=420029;sprop-parameter-sets=Z0IAKZWgHgCJeVA=,aM4eDA==\r\n" // sps(1080), pps(q25)
									;
		sprintf(r, sdpLine, strRemoteIP, "H264Stream", strRTSPURL, "H264Stream");
		sprintf(strBuff, sdpH264Line, mediaformat, mediaformat, strRTSPURL, RTSP_SDP_URL_VIDEO);
		strcat(r, strBuff);
	}
#ifdef __JPEG__
	else
#endif
#else
	UNUSED(mediaformat);
#endif
#ifdef __JPEG__
	if (video_type == RTSP_MJPEG) {
		#define MJPEG_payloadtype 26
		const char* sdpJPEGLine =	"m=video 0 RTP/AVP %u\r\n"	// mediaTye(video, audio), port(def 0), payloadType
									"b=AS:20480\r\n"
									"a=rtpmap:%u JPEG/90000\r\n"// payloadType, payloadFormatName(H264, JPEG, etc..), TimestampFrequency(H264 def 90kHZ, JPEG...?),
									"a=control:%s/%s\r\n"	// track1 ~ track?? => session 당 1
									"a=recvonly\r\n"
									;
		sprintf(r, sdpLine, strRemoteIP, "JPEGStream", strRTSPURL, "JPEGStream");
		sprintf(strBuff, sdpJPEGLine, MJPEG_payloadtype, MJPEG_payloadtype, strRTSPURL, RTSP_SDP_URL_VIDEO);
		strcat(r, strBuff);
	}
#endif

#ifdef __AUDIO__
#if 1
	#define AUDIO_payloadtype 0
	// CONFIG_AUDIO_RTP
	// Media Description, G.711 dyamic payload type is used.
	const char* sdpAudio =	"m=audio 0 RTP/AVP %u\r\n"
							"b=AS:64\r\n"
							"a=rtpmap:%u PCMU/8000\r\n"
							"a=control:%s/%s\r\n"
							"a=recvonly\r\n"
							;
#else
#define AUDIO_payloadtype 11
	const char* sdpAudio =	"m=audio 0 RTP/AVP %u\r\n"
							"b=AS:44\r\n"
							"a=rtpmap:%u L16/44100\r\n"
							"a=control:%s/audio\r\n"
							"a=recvonly\r\n"
							;
#endif
	sprintf(strBuff, sdpAudio, AUDIO_payloadtype, AUDIO_payloadtype, strRTSPURL, RTSP_SDP_URL_AUDIO);
	strcat(r, strBuff);
#endif

#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
	#define METADATA_payloadtype 107
	// CONFIG_METADATA_RTP
	const char* sdpMeta =	"m=application 0 RTP/AVP %u\r\n"
							"b=AS:8\r\n"
							"a=rtpmap:%u vnd.onvif.metadata/90000\r\n"
							"a=control:%s/%s\r\n"
							"a=recvonly\r\n"
							;
	sprintf(strBuff, sdpMeta, METADATA_payloadtype, METADATA_payloadtype, strRTSPURL, RTSP_SDP_URL_METADATA);
	strcat(r, strBuff);
#endif
#ifdef __AUDIO__
#if (ONVIF_BACKCHANNEL==1)
	// CONFIG_AUDIO_RTP Backchannel
	// Media Description, G.711 dyamic payload type is used.
	if (prtsp_ses->setup_backch == DEF_ON) {
		const char* sdpBackCH =	"m=audio 0 RTP/AVP %u\r\n"
								"b=AS:64\r\n"
								"a=rtpmap:%u PCMU/8000\r\n"
								"a=control:%s/%s\r\n"
								"a=sendonly\r\n"
								;
		sprintf(strBuff, sdpBackCH, AUDIO_payloadtype,  AUDIO_payloadtype, strRTSPURL, RTSP_SDP_URL_BACKCHANNEL);
		strcat(r, strBuff);
	}
#endif
#endif
#endif

	return (enx_os_strlen(r));
}

void gen_rtsp_response(rtsp_con_t *prtsp_conn, int code, int mt)
{
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	rtp_session_t *prtp_ses = &(prtsp_ses->prtp_s[mt]);
	char *response = (char*)prtsp_ses->out_buffer;

	char strTime[64] = {0};
	time_t gmttime = gptMsgShare.TIME;
	struct tm *timeinfo = gmtime(&gmttime);
	enx_os_sprintf(strTime, RTSP_MSG_DATE CRLF, strWeek[timeinfo->tm_wday], strMonth[timeinfo->tm_mon], timeinfo->tm_mday, timeinfo->tm_year+1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

	switch (code) {
		case 200:
			switch (prtsp_ses->cmd) {
				case SETUP:
					switch (prtsp_ses->rtp_type) {
						case RTP_UDP:
							enx_os_sprintf(response,
								RTSP_MSG_S HDR_UDP_SETUP_REPLY "%s",
								RTSP_200,
								prtsp_ses->seq,
								prtsp_ses->session,
								prtsp_ses->rtp_port[mt],
								prtsp_ses->rtp_port[mt]+1,
								prtsp_conn->upcb[mt*2]->local_port,
								prtsp_conn->upcb[mt*2+1]->local_port,
								prtp_ses->ssrc,
								strTime);
							break;

						case RTP_TCP:
						case RTP_HTTP:
							enx_os_snprintf(response, 1460,
								RTSP_MSG_S HDR_TCP_SETUP_REPLY "%s",
								RTSP_200,
								prtsp_ses->seq,
								prtsp_ses->session,
								prtsp_ses->rtp_port[mt],
								prtsp_ses->rtp_port[mt]+1,
								prtp_ses->ssrc,
								strTime);
							break;

						default:
							dprintf("ERROR : %s(%d), CMD(SETUP)\r\n", __FUNCTION__, __LINE__);
							if (prtsp_ses->session == 0) {
								enx_os_sprintf(response, (const char*)RTSP_MSG CRLF, RTSP_501, prtsp_ses->seq);
							} else {
								enx_os_sprintf(response, (const char*)RTSP_MSG_S CRLF, RTSP_501, prtsp_ses->seq, prtsp_ses->session);
							}
							break;
					}
					break;

				case PLAY:
					enx_os_sprintf(response, (const char*)RTSP_MSG_S RTSP_MSG_RTPINFO "%s", RTSP_200,
						prtsp_ses->seq, prtsp_ses->session,
						prtsp_ses->rtsp_uri, prtp_ses->start_seq, prtp_ses->start_rtptime,
						strTime);

					if(prtsp_ses->video_type == RTSP_H264)
						gptMsgShare.Rtsp_connect[0] = 1;
					else if(prtsp_ses->video_type == RTSP_H264SUB)
						gptMsgShare.Rtsp_connect[1] = 1;
//					flprintf("START RTP TIME %10d\r\n", prtp_ses->start_rtptime);
					break;

				case PAUSE:
				case TEARDOWN:
					if(prtsp_ses->video_type == RTSP_H264)
						gptMsgShare.Rtsp_connect[0] = 0;
					else if(prtsp_ses->video_type == RTSP_H264SUB)
						gptMsgShare.Rtsp_connect[1] = 0;
					enx_os_sprintf(response, (const char*)RTSP_MSG_S "%s", RTSP_200, prtsp_ses->seq, prtsp_ses->session, strTime);
					break;

				case OPTIONS:
					if(prtsp_ses->session == 0) {
						enx_os_sprintf(response, RTSP_MSG HDR_OPTION_REPLY1 "%s", RTSP_200, prtsp_ses->seq, strTime);
					} else {
						enx_os_sprintf(response, RTSP_MSG_S HDR_OPTION_REPLY2 "%s", RTSP_200, prtsp_ses->seq, prtsp_ses->session, strTime);
					}
					break;

				case DESCRIBE:
					if (prtsp_ses->session == 0) {
					    char sdp[1024];
						int sdp_len = build_sdp(prtsp_conn, sdp);
						enx_os_sprintf(response, (const char*)RTSP_MSG"content-type: application/sdp\r\nContent-Length: %d\r\n\r\n", RTSP_200, prtsp_ses->seq, sdp_len);
						enx_os_strcat(response,sdp);
					} else {
						enx_os_sprintf(response, (const char*)RTSP_MSG_S"Server: EN673 RTSP Server\r\n\r\n", RTSP_200, prtsp_ses->seq, prtsp_ses->session);
					}
					break;

				case SET_PARAMETER:
				case GET_PARAMETER:
					if (prtsp_ses->session == 0) {
						enx_os_sprintf(response, (const char*)RTSP_MSG "%s", RTSP_200, prtsp_ses->seq, strTime);
					} else {
						enx_os_sprintf(response, (const char*)RTSP_MSG_S "%s", RTSP_200, prtsp_ses->seq, prtsp_ses->session, strTime);
					}
					break;
			}
			break;

		case 400:
			if (prtsp_ses->session == 0) {
				enx_os_sprintf(response, RTSP_MSG, RTSP_400, prtsp_ses->seq);
			} else {
				enx_os_sprintf(response, RTSP_MSG_S, RTSP_400, prtsp_ses->seq, prtsp_ses->session);
			}
			break;

#if RTSP_SUPPORT_AUTHORIZATION
		case 401:
			if (prtsp_ses->session == 0) {
				enx_os_sprintf(response, (const char*)RTSP_MSG RTSP_MSG_BAUTH CRLF, RTSP_401, prtsp_ses->seq, "Server: EN673 RTSP Server");
			} else {
				enx_os_sprintf(response, (const char*)RTSP_MSG_S RTSP_MSG_BAUTH CRLF, RTSP_401, prtsp_ses->seq, prtsp_ses->session, "Server: EN673 RTSP Server");
			}
			break;
#endif

		case 404:
			if (prtsp_ses->session == 0) {
				enx_os_sprintf(response, (const char*)RTSP_MSG CRLF, RTSP_404, prtsp_ses->seq);
			} else {
				enx_os_sprintf(response, (const char*)RTSP_MSG_S CRLF, RTSP_404, prtsp_ses->seq, prtsp_ses->session);
			}
			break;

		default:
			if (prtsp_ses->session == 0) {
				enx_os_sprintf(response, (const char*)RTSP_MSG CRLF, RTSP_501, prtsp_ses->seq);
			} else {
				enx_os_sprintf(response, (const char*)RTSP_MSG_S CRLF, RTSP_501, prtsp_ses->seq, prtsp_ses->session);
			}
			break;
	}
}

/**
    @a_mtype : media type 0: vidio, 1: audio
*/

int parse_transport_header(char *cmd, rtsp_session_t *prtsp_ses, u32 a_mtype)
{
	int rtp_port, rtcp_port;
	char *strStart;

	if ((strStart = enx_os_strstr(cmd, "client_port"))) {			/* rtp over udp */
		sscanf(strStart, "client_port=%d-%d", &rtp_port, &rtcp_port);
		prtsp_ses->rtp_port[a_mtype] = rtp_port;
		prtsp_ses->rtcp_port[a_mtype] = rtcp_port;
		prtsp_ses->rtp_type = RTP_UDP;
		return 1;
	} else if ((strStart = enx_os_strstr(cmd, "interleaved"))) {	/* rtp over rtsp */
		sscanf(strStart, "interleaved=%d-%d", &rtp_port, &rtcp_port);
		prtsp_ses->rtp_port[a_mtype] = rtp_port;
		prtsp_ses->rtcp_port[a_mtype] = rtcp_port;
		prtsp_ses->rtp_type = RTP_TCP;
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
int parse_rtsp_interleaved(rtsp_con_t *prtsp_conn, rtsp_session_t *prtsp_ses, char* data, int left)
{
#if 0
	UINT ts_start = mfspr(EXR1K_HTBTR);
	printf("1");
#endif
//	char* ori_data = data;
//	int ori_left = left;
	while (left > 0) {
		rthInterleaved* Interleaved = (rthInterleaved*)data;

		if (Interleaved->un8Magic != '$') {
			printf("Error Interleaved Magic(0x%02X)\r\n", Interleaved->un8Magic);
			//hexDump("Error Interleaved Magic", ori_data, ori_left);
			break;
		}

		if (left <= (int)sizeof(rthInterleaved)) {
			//printf("Next Interleaved! but size %d...\r\n", left);
			//hexDump("Next Interleaved! but size", ori_data, ori_left);
			prtsp_conn->req_more = sizeof(rthInterleaved) + Interleaved->un16Length - left;
			break;
		}

		if (Interleaved->un16Length > (left - sizeof(rthInterleaved))) {
			//printf("Next Packet? inter_len(%d) pay_len(%d)\r\n", Interleaved->un16Length, left - sizeof(rthInterleaved));
			prtsp_conn->req_more = sizeof(rthInterleaved) + Interleaved->un16Length - left;
			break;
		}

		// RTP recv(ex: BACKCHANNEL) - SDP:sendonly option
#ifdef __ONVIF__
#if (ONVIF_BACKCHANNEL==1)
#ifdef __AUDIO__
		if (prtsp_ses->rtp_port[BACKCHANNEL] == Interleaved->un8Channel) {
			if(prtsp_ses->play_query & RTSPpq_BACKCHANNEL) {
				printf("TCP RTP(BackCH) IN\r\n");
				rthRTPHeader* RTPHdr = (rthRTPHeader*)(data + 4);
				rtp_rx_g711_backchannel(RTPHdr, Interleaved->un16Length - sizeof(rthRTPHeader));
			}
		} else
#endif
#endif
#endif

		// RTCP recv
		// Stream 단위의 TEARDOWN을 해도 RTCP는 전송된다.
		// 현재 이 코드는 버그가 있다.(hjlee:17-10-19)
		// ONVIF_BACKCHANNEL을 활성화 할 경우 Channel이 0-1이므로
		// rtcp_port[VIDEO]와 rtcp_port[BACKCHANNEL]이 같은 1을 가지게 된다.
		// RTCP에서 별다른 작업을 하지 않으므로 무시해도 되나, 추가개발시 오작동을 염두해야 함.
		// VIDEO에선 RR이 오고, BACKCHANNEL에선 SR이 올 것으로 예상된다.
		{
			prtsp_conn->prtcp_info.valid = 0;
			if (prtsp_ses->rtcp_port[VIDEO] == Interleaved->un8Channel) {
//				if(prtsp_ses->play_query & RTSPpq_VIDEO) {
//					printf("TCP RTCP(V) IN\r\n");
					rtcppk_t* RTCPmsg = (rtcppk_t*)(data + sizeof(rthInterleaved));
//					prtsp_conn->prtcp_info.valid = 1;
//					prtsp_conn->prtcp_info.mtype = VIDEO;
#if ENX_NETWORK_Debug
					gptMsgDebug.RTCP_packet_rx[prtsp_conn->valid-1]++;
#endif
					rtcp_process(prtsp_conn, RTCPmsg, Interleaved->un16Length);
//				}
			}
#ifdef __AUDIO__
			else if (prtsp_ses->rtcp_port[AUDIO] == Interleaved->un8Channel) {
//				if(prtsp_ses->play_query & RTSPpq_AUDIO) {
//					printf("TCP RTCP(A) IN\r\n");
					rtcppk_t* RTCPmsg = (rtcppk_t*)(data + sizeof(rthInterleaved));
//					prtsp_conn->prtcp_info.valid = 1;
//					prtsp_conn->prtcp_info.mtype = AUDIO;
#if ENX_NETWORK_Debug
					gptMsgDebug.RTCP_packet_rx[prtsp_conn->valid-1]++;
#endif
					rtcp_process(prtsp_conn, RTCPmsg, Interleaved->un16Length);
//				}
			}
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
			else if (prtsp_ses->rtcp_port[METADATA] == Interleaved->un8Channel) {
//				if(prtsp_ses->play_query & RTSPpq_METADATA) {
//					printf("TCP RTCP(M) IN\r\n");
					rtcppk_t* RTCPmsg = (rtcppk_t*)(data + sizeof(rthInterleaved));
//					prtsp_conn->prtcp_info.valid = 1;
//					prtsp_conn->prtcp_info.mtype = METADATA;
#if ENX_NETWORK_Debug
					gptMsgDebug.RTCP_packet_rx[prtsp_conn->valid-1]++;
#endif
					rtcp_process(prtsp_conn, RTCPmsg, Interleaved->un16Length);
//				}
			}
#endif
#if (ONVIF_BACKCHANNEL==1)
			else if (prtsp_ses->rtcp_port[BACKCHANNEL] == Interleaved->un8Channel) {
//				if(prtsp_ses->play_query & RTSPpq_BACKCHANNEL) {
//					printf("TCP RTCP(B) IN\r\n");
					rtcppk_t* RTCPmsg = (rtcppk_t*)(data + sizeof(rthInterleaved));
//					prtsp_conn->prtcp_info.valid = 1;
//					prtsp_conn->prtcp_info.mtype = BACKCHANNEL;
#if ENX_NETWORK_Debug
					gptMsgDebug.RTCP_packet_rx[prtsp_conn->valid-1]++;
#endif
					rtcp_process(prtsp_conn, RTCPmsg, Interleaved->un16Length);
//				}
			}
#endif
#endif
			else {
				hexDump("RTSP-interleaved Error IN", data, left);
			}
		}

		if ((unsigned int)left != (sizeof(rthInterleaved) + Interleaved->un16Length)) {
			if (*((char *)(data + (sizeof(rthInterleaved) + Interleaved->un16Length))) != '$') {
				printf("left(%d)pak(%d)\r\n", left, (sizeof(rthInterleaved) + Interleaved->un16Length));
				//hexDump("RTSP-interleaved Error? Size", data, max((unsigned int)left, sizeof(rthInterleaved) + Interleaved->un16Length));
				prtsp_conn->req_more = sizeof(rthInterleaved) + Interleaved->un16Length - left;
				return left;
			}
		}

		left -= (sizeof(rthInterleaved) + Interleaved->un16Length);
		data += (sizeof(rthInterleaved) + Interleaved->un16Length);

		if (left < 0) {
			printf("left(%d)!!!\r\n", left);
		}
	}
#if 0
	UINT ts_end = mfspr(EXR1K_HTBTR);
	UINT dep, time_us;
	if(ts_start < ts_end)
		dep = ts_end - ts_start;
	else
		dep = (0xFFFFFFFF - ts_start) + ts_end;
	time_us = dep/(BUS_CLK/1000000);
	printf("[%d]", time_us);
#endif
	return left;
}

/**
      rtp connection is setup up
*/
void
rtsp_open_rtp(rtsp_con_t *prtsp_conn, int media_type, int rtp_type)
{
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	rtp_session_t *rtp_s = &(prtsp_ses->prtp_s[media_type]);

	rtp_s->ssrc          = enx_os_rand() * enx_os_rand();
	rtp_s->start_rtptime = 1 + (u32)(enx_os_rand() % (0xFFFFFFFF));
	rtp_s->start_seq     = 1 + (u32)(enx_os_rand() % (0xFFFF));
	rtp_s->packet_cnt    = 0;
	rtp_s->total_length  = 0;

	if (rtp_type == RTP_UDP) {
		if (RTP_UDP_PCB <= (media_type * 2 + 1)) {
			printf("RTP UDP PCB array overflow!(Max:%d, media_type:%d)\r\n", RTP_UDP_PCB, media_type);
			return;
		}

		int idx = media_type * 2;
		if (prtsp_conn->upcb[idx] == NULL) {
			prtsp_conn->upcb[idx] = udp_new();
			if (prtsp_conn->upcb[idx] != NULL) {
				err_t err = udp_bind(prtsp_conn->upcb[idx], IP_ADDR_ANY, 0);
				prtsp_conn->upcb[idx]->remote_ip = prtsp_conn->tpcb->remote_ip;
				prtsp_conn->upcb[idx]->remote_port = prtsp_ses->rtp_port[media_type];
				if(err == ERR_OK) {
#ifdef __AUDIO__
#if (ONVIF_BACKCHANNEL==1)
					if (media_type == BACKCHANNEL) {
						udp_recv(prtsp_conn->upcb[idx], rtp_rx_g711_over_udp_recv, prtsp_conn);
					}
#endif
#endif
//					printf("UDP(RTP) Open(%d) : %d\r\n", idx, prtsp_conn->upcb[idx]->local_port);
				} else {
					printf("udp pcb(type:%d) bind err(%d)\r\n", idx, err);
				}
			} else {
				printf("udp pcb(type:%d) null!\r\n", idx);
			}
		}
		if (prtsp_conn->upcb[idx+1] == NULL) {
			prtsp_conn->upcb[idx+1] = udp_new();
			if (prtsp_conn->upcb[idx+1] != NULL) {
				err_t err = udp_bind(prtsp_conn->upcb[idx+1], IP_ADDR_ANY, 0);
				prtsp_conn->upcb[idx+1]->remote_ip = prtsp_conn->tpcb->remote_ip;
				prtsp_conn->upcb[idx+1]->remote_port = prtsp_ses->rtcp_port[media_type];
				if(err == ERR_OK) {
					udp_recv(prtsp_conn->upcb[idx+1], rtcp_recv_fn, prtsp_conn);
//					printf("UDP(RTP) Open(%d) : %d\r\n", idx+1, prtsp_conn->upcb[idx+1]->local_port);
				} else {
					printf("udp pcb(type:%d) bind err(%d)\r\n", idx+1, err);
				}
			} else {
				printf("udp pcb(type:%d) null!\r\n", idx+1);
			}
		}
	}
}

/**
	rtp connection is closed
*/
#if 1
static void
udp_rtp_port_close(rtsp_con_t *prtsp_conn, int idx)
{
	if (RTP_UDP_PCB <= idx) {
		printf("RTP UDP PCB array overflow!(Max:%d, idx:%d)\r\n", RTP_UDP_PCB, idx);
		return;
	}

	if (prtsp_conn->upcb[idx]) {
		udp_remove(prtsp_conn->upcb[idx]);
		prtsp_conn->upcb[idx] = NULL;
	}
}

void
rtsp_close_rtp(rtsp_con_t *prtsp_conn)
{
//	flprintf("RTP Close\r\n");
	udp_rtp_port_close(prtsp_conn, VIDEO*2);			// RTP
#ifdef __AUDIO__
	udp_rtp_port_close(prtsp_conn, AUDIO*2);			// RTP
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
	udp_rtp_port_close(prtsp_conn, METADATA*2);			// RTP
#endif
#if (ONVIF_BACKCHANNEL==1)
	udp_rtp_port_close(prtsp_conn, BACKCHANNEL*2);		// RTP
#endif
#endif
}

void
rtsp_close_rtcp(rtsp_con_t *prtsp_conn)
{
//	flprintf("RTCP Close\r\n");
	udp_rtp_port_close(prtsp_conn, VIDEO*2+1);			// RTCP
#ifdef __AUDIO__
	udp_rtp_port_close(prtsp_conn, AUDIO*2+1);			// RTCP
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
	udp_rtp_port_close(prtsp_conn, METADATA*2+1);		// RTCP
#endif
#if (ONVIF_BACKCHANNEL==1)
	udp_rtp_port_close(prtsp_conn, BACKCHANNEL*2+1);	// RTCP
#endif
#endif
}
#endif

/**
    @len , received packet len
*/
err_t rtsp_session_proc(rtsp_con_t *prtsp_conn, struct pbuf **inp, int *pend)
{
	rtsp_session_t *prtsp_ses = &(prtsp_conn->prtsp_ses);
	struct pbuf *p = *inp;
	char *crlf;
	int data_len;
	char *data;
#if RTSPD_SUPPORT_REQUESTLIST
	u16_t clen;
#endif // RTSPD_SUPPORT_REQUESTLIST

	if (prtsp_conn == NULL) {
		return ERR_ABRT;
	}

#if RTSPD_SUPPORT_REQUESTLIST
	// enqueue the pbuf
	if (prtsp_conn->req == NULL) {
		prtsp_conn->req = p;
	} else {
		pbuf_cat(prtsp_conn->req, p);
	}
	/* increase pbuf ref counter as it is freed when we return but we want to
	   keep it on the req list */
	pbuf_ref(p);

	if (prtsp_conn->req->next != NULL) {
		data_len = LWIP_MIN(prtsp_conn->req->tot_len, RTSPD_MAX_REQ_LENGTH);
		pbuf_copy_partial(prtsp_conn->req, prtsp_conn->req_buf, data_len, 0);
		data = prtsp_conn->req_buf;
	} else
#endif /* RTSPD_SUPPORT_REQUESTLIST */
	{
		data = (char *)p->payload;
		data_len = p->len;
	}

	if(data[0] == '$' || data[prtsp_conn->req_offset] == '$') {
//		hexDump("interleaved input", data, data_len);
		prtsp_ses->cmd = RTSP_INTERLEAVED;
		prtsp_conn->poll_cnt = 0;
		if(data_len == 4) {
//			RTSP_INTERLEAVED
		} else {
			int rtsp_left = parse_rtsp_interleaved(prtsp_conn, prtsp_ses, data+prtsp_conn->req_offset, data_len-prtsp_conn->req_offset);
			if (rtsp_left == 0) {
				prtsp_conn->req_left = 0;
				prtsp_conn->req_offset = 0;
				return ERR_OK;
			} else if (rtsp_left < 0) {
				// 음수가 나올 경우 오류상황으로 본다.
				//printf("data_len(%d) req_left(%d) req_offset(%d) / reset\r\n", data_len, prtsp_conn->req_left, prtsp_conn->req_offset);
				prtsp_conn->req_left = 0;
				prtsp_conn->req_offset = 0;
				return ERR_BUF;
			} else {
				// data가 잔존...
				prtsp_conn->req_left = rtsp_left;
				prtsp_conn->req_offset = data_len - rtsp_left;
				//printf("data_len(%d) req_left(%d) req_offset(%d) / continue\r\n", data_len, prtsp_conn->req_left, prtsp_conn->req_offset);
			}
		}
	} else if (data_len >= RTSPD_MIN_REQ_LEN) { // received enough data for minimal request?
		crlf = strnstr(data, CRLF, data_len);
		if (crlf != NULL) {
			data[data_len] = '\0';
			parse_rtsp_msg(prtsp_ses, data);
			if (prtsp_ses->seq == 0xFFFFFFFF) {
				// next packet!
				//char debug_out[64];
				//sprintf(debug_out, "%04d:%s: Net Packet(size:%d)!", __LINE__, __func__, data_len);
				//hexDump(debug_out, data, data_len);
			} else {
				// URL search
				char *rcvUriS = strnstr(data, " ", data_len) + 1;
				int nUrlLen = data_len - ((rcvUriS + 1) - data);
				char *rcvUriE = strnstr(rcvUriS + 1, " ", nUrlLen);
				nUrlLen = rcvUriE - rcvUriS;
				char query_url[1024] = {0};
				strncpy(query_url, rcvUriS, nUrlLen);
				//printf("Query URL : %s\r\n", query_url);

				switch (prtsp_ses->cmd) {
					case OPTIONS:
						if (prtsp_ses->video_type == RTSP_H264 || prtsp_ses->video_type == RTSP_H264SUB || prtsp_ses->video_type == RTSP_MJPEG) {
							gen_rtsp_response(prtsp_conn, 200, 0);
						} else {
							gen_rtsp_response(prtsp_conn, 501, 0);
						}
						break;

					case DESCRIBE:
#if RTSP_SUPPORT_AUTHORIZATION
						if (prtsp_ses->login == 0) {
							gen_rtsp_response(prtsp_conn, 401, 0);
						} else
#endif
						if (prtsp_ses->video_type == RTSP_H264 || prtsp_ses->video_type == RTSP_H264SUB || prtsp_ses->video_type == RTSP_MJPEG) {
							gen_rtsp_response(prtsp_conn, 200, 0);
						} else {
							gen_rtsp_response(prtsp_conn, 501, 0);
						}
						break;

					case SETUP:
						if (prtsp_ses->video_type == RTSP_H264 || prtsp_ses->video_type == RTSP_H264SUB || prtsp_ses->video_type == RTSP_MJPEG) {
							struct timeval t1;
							int media_type = -1;
							if(enx_os_strstr(query_url, RTSP_SDP_URL_VIDEO)) {
								media_type = VIDEO;
								prtsp_ses->play_query |= RTSPpq_VIDEO;
	 						}
#ifdef __AUDIO__
							else if(enx_os_strstr(query_url, RTSP_SDP_URL_AUDIO)) {
								media_type = AUDIO;
								prtsp_ses->play_query |= RTSPpq_AUDIO;
							}
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
							else if(enx_os_strstr(query_url, RTSP_SDP_URL_METADATA)) {
								media_type = METADATA;
								prtsp_ses->play_query |= RTSPpq_METADATA;
							}
#endif
#if (ONVIF_BACKCHANNEL==1)
							else if(enx_os_strstr(query_url, RTSP_SDP_URL_BACKCHANNEL)) {
								media_type = BACKCHANNEL;
								prtsp_ses->play_query |= RTSPpq_BACKCHANNEL;
							}
#endif
#endif
							else {
						   		gen_rtsp_response(prtsp_conn, 501, 0);
								break;
					  	 	}

#if (RTSP_SUPPORT_RTCP_SR==1)
							prtsp_ses->rtcp_send_time[media_type] = gptMsgShare.TIME;
#endif
							gettimeofday(&t1, NULL);
							enx_os_srand(t1.tv_usec * t1.tv_sec);
							parse_transport_header(data, prtsp_ses, media_type);
							gptMsgShare.rtsp_conn_info[prtsp_conn->valid-1].rtp_type = prtsp_ses->rtp_type;
							rtsp_open_rtp(prtsp_conn, media_type, prtsp_ses->rtp_type);
							prtsp_ses->cur_state = STATE_READY;
							if (prtsp_ses->session == 0)
								prtsp_ses->session = rand();
							prtsp_ses->play = 0;

							rtp_register_streamingfn(media_type, prtsp_conn);

							gen_rtsp_response(prtsp_conn, 200, media_type);
						} else {
							gen_rtsp_response(prtsp_conn, 501, 0);
						}
						break;

					case PLAY:
						if (prtsp_ses->video_type == RTSP_H264 || prtsp_ses->video_type == RTSP_H264SUB || prtsp_ses->video_type == RTSP_MJPEG) {
							char strRTP[8], strVType[12];
							switch (prtsp_ses->rtp_type) {
								case RTP_UDP:		strcpy(strRTP, "UDP");		break;
								case RTP_TCP:		strcpy(strRTP, "TCP");		break;
								case RTP_HTTP:		strcpy(strRTP, "HTTP");		break;
								default:			strcpy(strRTP, "Unknown");	break;
							}
							switch (prtsp_ses->video_type) {
								case RTSP_H264:		strcpy(strVType, "H264_1");	break;
								case RTSP_H264SUB:	strcpy(strVType, "H264_2");	break;
								case RTSP_MJPEG:	strcpy(strVType, "MJPEG");	break;
								default:			strcpy(strVType, "Unknown");break;
							}
							printf("VIDEO(%s) RTP(%s)\r\n", strVType, strRTP);
							prtsp_ses->cur_state = STATE_PLAY;
							gen_rtsp_response(prtsp_conn, 200, 0);
						} else {
							gen_rtsp_response(prtsp_conn, 501, 0);
						}
						break;

					case PAUSE:
						if (prtsp_ses->video_type == RTSP_H264 || prtsp_ses->video_type == RTSP_H264SUB || prtsp_ses->video_type == RTSP_MJPEG) {
							if (prtsp_ses->cur_state == STATE_PLAY) {
								prtsp_ses->cur_state = STATE_PAUSE;
								gen_rtsp_response(prtsp_conn, 200, 0);
							} else {
								gen_rtsp_response(prtsp_conn, 501, 0);
							}
						} else {
							gen_rtsp_response(prtsp_conn, 501, 0);
						}
						break;

					case GET_PARAMETER:
						gen_rtsp_response(prtsp_conn, 200, 0);
						break;

					case SET_PARAMETER:
						printf("%04d:%s: dummy set parameter\r\n", __LINE__, __func__);
						gen_rtsp_response(prtsp_conn, 200, 0);
						break;

					case TEARDOWN:
						{
							int media_type = -1;
							if(enx_os_strstr(data, RTSP_SDP_URL_VIDEO)) {
								media_type = VIDEO;
								prtsp_ses->play_query &= ~RTSPpq_VIDEO;
							}
#ifdef __AUDIO__
							else if(enx_os_strstr(data, RTSP_SDP_URL_AUDIO)) {
								media_type = AUDIO;
								prtsp_ses->play_query &= ~RTSPpq_AUDIO;
							}
#endif
#ifdef __ONVIF__
#if (ONVIF_METADATA==1)
							else if(enx_os_strstr(data, RTSP_SDP_URL_METADATA)) {
								media_type = METADATA;
								prtsp_ses->play_query &= ~RTSPpq_METADATA;
							}
#endif
#if (ONVIF_BACKCHANNEL==1)
							else if(enx_os_strstr(data, RTSP_SDP_URL_BACKCHANNEL)) {
								media_type = BACKCHANNEL;
								prtsp_ses->play_query &= ~RTSPpq_BACKCHANNEL;
							}
#endif
#endif
							if (media_type == -1 || prtsp_ses->play_query == 0) {
								prtsp_ses->cur_state = STATE_END;
								gen_rtsp_response(prtsp_conn, 200, 0);
								*pend = 2;
							} else {
								// 1 session만 close
								gen_rtsp_response(prtsp_conn, 200, 0);
							}
						}

						break;

					default:
						hexDump("RTSP-default", (void *)data, data_len);
						gen_rtsp_response(prtsp_conn, 501, 0);
						break;
				}

				prtsp_ses->out_size = enx_os_strlen(prtsp_ses->out_buffer);
				return ERR_OK;
			}
		} else {
			printf("%s(%d) : invalid URI(%dbyte)\r\n", __func__, __LINE__, data_len);
//			hexDump("invalid URI1", data, LWIP_MIN(data_len, 128));
			hexDump("invalid URI1", data, data_len);
			prtsp_conn->req_left = 0;
			prtsp_conn->req_offset = 0;
			return ERR_BUF;
		}
	} else {
		printf("%s(%d) : invalid URI(%dbyte)\r\n", __func__, __LINE__, data_len);
//		hexDump("invalid URI2", data, LWIP_MIN(data_len, 128));
		hexDump("invalid URI2", data, data_len);
		prtsp_conn->req_left = 0;
		prtsp_conn->req_offset = 0;
		return ERR_BUF;
	}

#if RTSPD_SUPPORT_REQUESTLIST
	clen = pbuf_clen(prtsp_conn->req);
	if ((prtsp_conn->req->tot_len <= RTSPD_REQ_BUFSIZE) && (clen <= RTSPD_REQ_QUEUELEN)) {
		// request not fully received (too short or CRLF is missing)
		if (clen > 3) {
			flprintf("?????\r\n");
#if 0
			struct pbuf *temp_pbuf;

			temp_pbuf = prtsp_conn->req;
			prtsp_conn->req = prtsp_conn->req->next;
			temp_pbuf->next = NULL;
			temp_pbuf->ref = 1;
			//printf("Pbuf pbuf_free\r\n");
			prtsp_conn->req_offset = prtsp_conn->req_offset - temp_pbuf->len;
			pbuf_free(temp_pbuf);

			clen = pbuf_clen(prtsp_conn->req);
			//printf(">>prtsp_conn->req->tot_len %d clen %d\r\n",prtsp_conn->req->tot_len, clen);
#endif
		}
		return ERR_INPROGRESS;
	} else
#endif // LWIP_HTTPD_SUPPORT_REQUESTLIST
	{
		// could not parse request
		gen_rtsp_response(prtsp_conn, 400, 0);
		return ERR_OK;
	}
}
