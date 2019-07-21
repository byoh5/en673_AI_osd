/* 


Programmer Share

program develop tips


Home

 
Privacy Policy

 
Disclaimer

 
Disclosure

 
Contact

 
--------------------------------------------------------------------------------
   

 


H264 over RTMP released V2.0 (Red5 the Wowza test by)

Tag: transmission of audio and video coding articles,  Author: yuquan0916 Date: 2011-04-19 
 
 
Encoders
 Flashes
 H264
 Inline
 Packet
 Protocols
 Returns
 Shorts
  

 Directly on the code 
*/




//[Cpp]  
 // Demo.cpp: define the entry point for the console application.    
 //    
 #include "stdafx.h"    
 #include <MqOaI.h>    
 extern   "C"     
{ 
#include ".. /.. / Common / common.h"    
#include ".. /.. / Common / cpu.h"     
#include "../../x264.h"    
#include ".. /.. / Encoder / set.h"    
} 
#include "librtmp / rtmp_sys.h"    
#include "librtmp / log.h"    
#include "librtmp / amf.h"    
#include "CameraDS.h"    
void ConvertYCbCr2BGR (unsigned char * pYUV, unsigned char * pBGR, int iWidth, int iHeight); 
void ConvertRGB2YUV (int w, int h, unsigned char * bmp, unsigned char * yuv); 
int InitSockets () 
{ 
# Ifdef WIN32    
	WORD version; 
	WSADATA wsaData; 
	version = MAKEWORD (1); 
	return (from WSAStartup (Version & wsaData) == 0); 
# Else    
	return TRUE; 
# Endif    
} 
inline   void CleanupSockets () 
{ 
# Ifdef WIN32    
	WSACleanup (); 
# Endif    
} 
# Define HEX2BIN (a) (((a) & 0x40)? ((A) & 0xf) +9: ((a) & 0xf))    
int HEX2BIN (char * str, char ** hex) 
{ 
	char * ptr; 
	int i, l = strlen (str); 
	if (l & 1) 
	return 0; 
	* Hex = (char *) malloc (l / 2); 
	ptr = * hex; 
	if (ptr) 
	return 0; 
	for (i = 0; i <l; i + = 2) 
	* Ptr + + = (HEX2BIN (str [i]) << 4) | HEX2BIN (str [i +1]); 
	return 1/2; 
} 
char * put_byte (char * output, uint8_t nVal) 
{ 
	output [0] = nVal; 
	return output +1; 
} 
char * put_be16 (char * output, uint16_t nVal) 
{ 
	output [1] = nVal & 0xff; 
	output [0] = nVal >> 8; 
	return output +2; 
} 
char * put_be24 (char * output, uint32_t nVal) 
{ 
	output [2] = nVal & 0xff; 
	output [1] = nVal >> 8; 
	output [0] = nVal >> 16; 
	return output +3; 
} 
char * put_be32 (char * output, uint32_t nVal) 
{ 
	output [3] = nVal & 0xff; 
	output [2] = nVal >> 8; 
	output [1] = nVal >> 16; 
	output [0] = nVal >> 24; 
	return output +4; 
} 
char * put_be64 (char * output, uint64_t nVal) 
{ 
	output = put_be32 (output, nVal >> 32); 
	output = put_be32 (output, nVal); 
	return output; 
} 
char * put_amf_string (char * c, const   char * str) 
{ 
	uint16_t len = strlen (str); 
	the c = put_be16 (c, len); 
	memcpy (c, str, len); 
	return c + len; 
} 
char * put_amf_double (char * c, double d) 
{ 
	* C + + = AMF_NUMBER; / * type: Number * /    
	{ 
		unsigned char * ci, * co; 
		CI = (unsigned char *) &d; 
		CO = (unsigned char *) c; 
		co [0] = the ci [7]; 
		co [1] = ci [6]; 
		co [2] = ci [5]; 
		co [3] = ci [4]; 
		co [4] = ci [3]; 
		co [5] = ci [2]; 
		co [6] = ci [1]; 
		CO [7] = ci [0]; 
	} 
	return c +8; 
} 
int main (int argc, char * argv []) 
{ 
	if (argc <2) 
	{ 
		RTMP_LogPrintf ("RTMP_URL IS NULL!!! / N"); 
		// Return -1;    
	} 
	if (! InitSockets ()) 
	{ 
		RTMP_LogPrintf ("InitSockets Error! / N"); 
		return -1; 
	} 
	//RTMP_LogPrintf ("InitSockets! / N "); 
	CoInitialize (NULL); 
	CCameraDS camera; 
	if (! camera.OpenCamera (0,320,240, false)) 
	{ 
	RTMP_LogPrintf ("Open Camera Error / n"); 
	return -1; 
	} 
	int nHeight = camera.GetHeight (); 
	int nWidth = camera.GetWidth (); 
	unsigned char * szRGBBuffer = new unsigned char [nHeight * nWidth * 3]; 
	RTMP_LogPrintf ("Camera Open Scuess! Picture Size [% 2dx% d] / n", nWidth, nHeight); 
	RTMP_debuglevel = RTMP_LOGINFO; 
	RTMP * r; 
	// Char uri [] = "rtmp :/ / 127.0.0.1/live/test";    
	// Char uri [] = "rtmp :/ / 192.199.15.223/live/test";    
	// Char uri [] = "rtmp :/ / 221.9.244.4/live/jltv";    
	// Char uri [] = "rtmp :/ / 192.199.15.223/oflaDemo/red5StreamDemo";    
	// Char uri [] = "rtmp :/ / 192.199.15.151/live/test";    
	char uri [] = "rtmp :/ / 127.0.0.1/live/zzj"; 
	the r = RTMP_Alloc (); 
	RTMP_Init (r); 
	RTMP_SetupURL (r, (char *) uri); 
	RTMP_EnableWrite (r); 
	RTMP_Connect (r, NULL); 
	RTMP_ConnectStream (r, 0); 
	unsigned char szNalBuffer [1024 * 32]; 
	unsigned char szBodyBuffer [1024 * 32]; 
	x264_nal_t ??* p264Nal; 
	The int i264Nal; 
	x264_param_t * p264Param; 
	x264_picture_t * p264Pic; 
	x264_t * p264Handle; 
	p264Param = new x264_param_t (); 
	p264Pic = new x264_picture_t (); 
	memset (p264Pic, 0, sizeof (x264_picture_t)); 
	x264_param_default (p264Param); / / set default param    
	p264Param-> i_threads = 2; 
	p264Param-> i_width = nWidth; / / set frame width    
	p264Param-> i_height = nHeight; / / set frame height    
	/ * Baseline level 1.1 * /    
	p264Param-> b_cabac = 0; 
	p264Param-> i_bframe = 0; 
	p264Param-> b_interlaced = 0; 
	p264Param-> rc.i_rc_method = X264_RC_ABR; / / X264_RC_CQP    
	p264Param-> i_level_idc = 21; 
	p264Param-> rc.i_bitrate = 200; 
	p264Param-> i_fps_num = 30; 
	p264Param-> i_keyint_max = p264Param-> i_fps_num * 3; 
	if ((p264Handle = x264_encoder_open (p264Param)) == NULL) 
	{ 
	fprintf (stderr, "x264_encoder_open failed / n"); 
	return -2; 
	} 
	bs_t bs = {0}; 
	x264_picture_alloc (p264Pic, X264_CSP_YV12, p264Param-> i_width, p264Param-> i_height); 
	p264Pic-> i_type = X264_TYPE_AUTO; 
	x264_picture_t pic_out; 
	RTMPPacket packet = {0}; 
	memset (& packet, 0, sizeof (RTMPPacket)); 
	packet.m_nChannel = 0x04; 
	packet.m_headerType = RTMP_PACKET_SIZE_LARGE; 
	packet.m_nTimeStamp = 0; 
	packet.m_nInfoField2 = r-> m_stream_id; 
	packet.m_hasAbsTimestamp = 0; 
	packet.m_body = (char *) szBodyBuffer; 
	char * szTmp = (char *) szBodyBuffer; 
	packet.m_packetType = RTMP_PACKET_TYPE_INFO; 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, "@ setDataFrame"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, "onMetaData"); 
	szTmp = put_byte (szTmp, AMF_OBJECT); 
	szTmp = put_amf_string (szTmp, "author"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_amf_string (szTmp, "copyright"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_amf_string (szTmp, "description"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_amf_string (szTmp, "keywords"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_amf_string (szTmp, "rating"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_amf_string (szTmp, "presetname"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, "Custom"); 
	szTmp = put_amf_string (szTmp, "width"); 
	szTmp = put_amf_double (szTmp, p264Param-> i_width); 
	szTmp = put_amf_string (szTmp, "width"); 
	szTmp = put_amf_double (szTmp, p264Param-> i_width); 
	szTmp = put_amf_string (szTmp, "height"); 
	szTmp = put_amf_double (szTmp, p264Param-> i_height); 
	szTmp = put_amf_string (szTmp, "framerate"); 
	szTmp = put_amf_double (szTmp, (double) p264Param-> i_fps_num / p264Param-> i_fps_den); 
	szTmp = put_amf_string (szTmp, "videocodecid"); 
	szTmp = put_byte (szTmp, AMF_STRING); 
	szTmp = put_amf_string (szTmp, "avc1"); 
	szTmp = put_amf_string (szTmp, "videodatarate"); 
	szTmp = put_amf_double (szTmp, p264Param-> rc.i_bitrate); 
	szTmp = put_amf_string (szTmp, "avclevel"); 
	szTmp = put_amf_double (szTmp, p264Param-> i_level_idc); 
	szTmp = put_amf_string (szTmp, "avcprofile"); 
	szTmp = put_amf_double (szTmp, 0x42); 
	szTmp = put_amf_string (szTmp, "videokeyframe_frequency"); 
	szTmp = put_amf_double (szTmp, 3); 
	szTmp = put_amf_string (szTmp, ""); 
	szTmp = put_byte (szTmp, AMF_OBJECT_END); 
	packet.m_nBodySize = szTmp-(char *) szBodyBuffer; 
	RTMP_SendPacket (r, & packet, 1); 
	packet.m_packetType = RTMP_PACKET_TYPE_VIDEO; / * VIDEO * /    
	szBodyBuffer [0] = 0x17; 
	szBodyBuffer [1] = 0x00; 
	szBodyBuffer [2] = 0x00; 
	szBodyBuffer [3] = 0x00; 
	szBodyBuffer [4] = 0x00; 
	szBodyBuffer [5] = 0x01; 
	szBodyBuffer [6] = 0x42; 
	szBodyBuffer [7] = 0xC0; 
	szBodyBuffer [8] = 0x15; 
	szBodyBuffer [9] = 0x03; 
	szBodyBuffer [10] = 0x01; 
	szTmp = (char *) szBodyBuffer +11; 
	short slen = 0; 
	bs_init (& bs, szNalBuffer, 16); / / initial words bs    
	x264_sps_write (& bs, p264Handle-> sps); / / read encoder SPS    
	slen = bs.p-bs.p_start +1; / / spslen (short)    
	slen = htons (sLen); 
	memcpy (szTmp, & slen, sizeof (short)); 
	szTmp + = sizeof (short); 
	* SzTmp = 0x67; 
	szTmp + = 1; 
	memcpy (szTmp, bs.p_start, bs.p-bs.p_start); 
	szTmp + = bs.p-bs.p_start; 
	* SzTmp = 0x01; 
	szTmp + = 1; 
	bs_init (& bs, szNalBuffer, 16); / / initial words bs    
	x264_pps_write (& bs, p264Handle-> pps); / / read encoder PPS    
	slen = bs.p-bs.p_start +1; / / spslen (short)    
	slen = htons (sLen); 
	memcpy (szTmp, & slen, sizeof (short)); 
	szTmp + = sizeof (short); 
	* SzTmp = 0x68; 
	szTmp + = 1; 
	memcpy (szTmp, bs.p_start, bs.p-bs.p_start); 
	szTmp + = bs.p-bs.p_start; 
	packet.m_nBodySize = szTmp-(char *) szBodyBuffer; 
	RTMP_SendPacket (r, & packet, 0); 
	unsigned int nTimes = 0; 
	the unsigned int oldTick = GetTickCount (); 
	unsigned int newTick = 0; 
	packet.m_nTimeStamp = 0; 
	  
	while (true) 
	{ 
	szBodyBuffer [0] = 0x17; 
	szBodyBuffer [1] = 0x01; 
	szBodyBuffer [2] = 0x00; 
	szBodyBuffer [3] = 0x00; 
	szBodyBuffer [4] = 0x42; 
	unsigned char * szTmp = szBodyBuffer +5; 
	unsigned char * pNal = szNalBuffer; 
	nTimes + +; 
	int nFramsInPack = 0; 
	while (true) 
	{ 
	nFramsInPack + +; 
	unsigned char * pCameraBuf = camera.QueryFrame (); 
	if (! pCameraBuf) 
	{ 
	return -1; 
	} 
	for (int ii = 0; ii <nHeight; ii + +) 
	{ 
	memcpy (szRGBBuffer + (nWidth * 3) * (nHeight-ii-1), pCameraBuf + (nWidth * 3) * ii, nWidth * 3); 
	/ / Memcpy (pCameraBuf + nWidth * (nHeight-ii-1), pCameraBuf + nWidth * ii, nWidth * 3);    
	/ / Memcpy (szLineBuffer, pCameraBuf + nWidth * (nHeight-ii-1), nWidth * 3);    
	} 
	ConvertRGB2YUV (nWidth, nHeight, szRGBBuffer, p264Pic-> img.plane [0]); 
	/ / Memcpy (p264Pic-> img.plane [0], szNalBuffer, nWidth * nHeight);    
	/ / Memcpy (p264Pic-> img.plane [1], szNalBuffer + nWidth * nHeight, nWidth * nHeight / 4);    
	/ / Memcpy (p264Pic-> img.plane [2], szNalBuffer + nWidth * nHeight * 5/4, nWidth * nHeight / 4);    
	/ * 
	int nCount; 
	nCount = fread (p264Pic-> img.plane [0], 1,176 * 144, yuv); 
	if (nCount <176 * 144) 
	{ 
	The fseek (YUV, SEEK_SET, 0); 
	continue; 
	} 
	nCount = fread (p264Pic-> img.plane [1], 1,176 * 144/4, yuv); 
	if (nCount <176 * 144/4) 
	{ 
	The fseek (YUV, SEEK_SET, 0); 
	continue; 
	} 
	nCount = fread (p264Pic-> img.plane [2], 1,176 * 144/4, yuv); 
	if (nCount <176 * 144/4) 
	{ 
	The fseek (YUV, SEEK_SET, 0); 
	continue; 
	} 
	* /    
	if (x264_encoder_encode (p264Handle, & p264Nal, & i264Nal, p264Pic, & pic_out) <0) 
	{ 
	fprintf (stderr, "x264_encoder_encode failed / n"); 
	} 
	for (int i = 0; i <i264Nal; i + +) 
	{ 
	int the i_size; 
	int i_data; 
	i_data = 1024 * 32; 
	if ((i_size = x264_nal_encode (pNal, & i_data, 1, & p264Nal [i]))> 0) 
	{ 
	if ((pnal [4] & 0x60) == 0) 
	{ 
	continue; 
	} 
	if (pnal [4] == 0x67) 
	{ 
	continue; 
	} 
	if (pnal [4] == 0x68) 
	{ 
	continue; 
	} 
	memmove (pNal, pNal +4, i_size-4); 
	pNal + = i_size-4; 
	} 
	else   if (the i_size <0) 
	{ 
	fprintf (stderr, "need to increase buffer size (size =% d) / n",-i_size); 
	} 
	} 
	unsigned int nSize = pNal-szNalBuffer; 
	packet.m_nBodySize = nSize +9; 
	if (i264Nal,> 1) 
	{ 
	szBodyBuffer [0] = 0x17; 
	} 
	else    
	{ 
	szBodyBuffer [0] = 0x27; 
	} 
	put_be32 ((char *) szBodyBuffer +5, nSize); 
	memcpy (szBodyBuffer +9, szNalBuffer, pNal-szNalBuffer); 
	RTMP_SendPacket (r, & packet, 0); 
	Sleep (20); 
	newTick = GetTickCount (); 
	/ / RTMP_LogStatus ("/ rInfo NAUL Type: 0x% 02x size:% 5d Tick:% 03d% 03d", szNalBuffer [0], nSize ,33-nSleep, GetTickCount ()-oldTick + nSleep);    
	packet.m_headerType = RTMP_PACKET_SIZE_MEDIUM; 
	packet.m_nTimeStamp + = newTick-oldTick; 
	oldTick = newTick; 
	break; 
	} 
	} 
	return 0; 
} 





  



 










   

Relate

 RTMP protocol and extract RTMP video streams H264 video files 
RTMP FLV flow to the standard H264, AAC conversion 
RTMP FLV flow to the standard H264, AAC conversion 
the rtmp protocol simple analytical and send the h264 flv files 
the rtmp protocol simple analytical and send the h264 flv files 
Open Source Flash Server: Red5 constructed for RTMP streaming media player platform 
red5 
About RTMP 
RED5 Installation 
RTMP Message Formats (the RTMP message format) 

 
  

¨Ï2009-2014. All Rights Reserved. Design by Programmer Share

      



    































