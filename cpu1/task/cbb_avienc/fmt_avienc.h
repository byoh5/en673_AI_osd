/* Copyright (c) 2013-2014 Eyenix Corporation. All rights reserved.
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
* AVR product.
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

#ifndef _MAKEAVI_H_
#define _MAKEAVI_H_

#include "ff.h"
#include "dev.h"
#include "board.h"
#include "dev_types.h"

//*************************************************************************************************
// typedefs
//-------------------------------------------------------------------------------------------------

/*
  Simple AVI Format
  
RIFF ('AVI '
      LIST ('hdrl' ... )
      LIST ('movi' ... )
      ['idx1' (<AVI Index>) ]
     )

RIFF ('AVI '

      LIST ('hdrl'
            'avih'(<Main AVI Header>)
            LIST ('strl'
                  'strh'(<Stream header>)
                  'strf'(<Stream format>)
                  [ 'strd'(<Additional header data>) ]
                  [ 'strn'(<Stream name>) ]
                  ...
                 )
             ...
           )

      LIST ('movi'
            {SubChunk | LIST ('rec '
                              SubChunk1
                              SubChunk2
                              ...
                             )
               ...
            }
            ...
           )

      ['idx1' (<AVI Index>) ]
     )
*/

#define AVI_HEADSIZE 4096

typedef struct {
	UINT option;							// Video, Audio, Text
	UINT totalFrame;

	int i_width;
	int i_height;
	int f_fps;

	UINT i_movi_end;
	UINT i_riff;
	UINT i_header_end;

	UINT type;
	UINT un32vidscount;
	UINT un32audscount;
	UINT un32txt1count;
	UINT un32txt2count;
	UINT un32txt3count;
	UINT un32txt4count;

	UINT un32vidsmax;
	UINT un32audsmax;
	UINT un32txt1max;
	UINT un32txt2max;
	UINT un32txt3max;
	UINT un32txt4max;
} t_avi_t;

enum {
    H264_STREAM = 0,
    G711_STREAM = 1,
	TXT1_STREAM = 2,	// gps
	TXT2_STREAM = 3,	// gsensor
	TXT3_STREAM = 4,	// time
	TXT4_STREAM = 5,	// obd
    IDX1_ARRAY = 6
};

#define AVI_VIDEO	0x01
#define AVI_AUDIO	0x02
#define AVI_TXTS1	0x04
#define AVI_TXTS2	0x08
#define AVI_TXTS3	0x10
#define AVI_TXTS4	0x20

#define FCC(ch1, ch2, ch3, ch4)	((DWORD)((ch1 << 24) | (ch2 << 16) | (ch3 << 8) | (ch4 << 0)))

#define NUM32(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))

#define NUM16(ch2) ((((WORD)(ch2) & 0xFF) << 8) |    \
                  (((WORD)(ch2) & 0xFF00) >> 8))

typedef DWORD FOURCC;

typedef struct {
	FOURCC	fcc;						// FOURCC �ڵ带 �����Ѵ�.
	DWORD	cb;							// ����ü�� ����� �����Ѵ�. ������ 8 ����Ʈ���� ������ ���� �����Ѵ�.
} __attribute__ ((packed)) FCC_CB_t;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'LIST' �� �ƴϸ� �ȵȴ�.
	FOURCC fccListType;					// FOURCC �ڵ带 �����Ѵ�.
} __attribute__ ((packed)) RIFFLIST;

typedef struct {
	FCC_CB_t fcccb;						// ���� 'avih' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	dwMicroSecPerFrame;		// �����Ӱ��� ������ ����ũ�� ������ ������ �����Ѵ�. �� ���� ������ ��ü�� Ÿ�̹��� ��Ÿ����.
		DWORD	dwMaxBytesPerSec;		// ������ ���� �ִ� ������ ����Ʈ�� �����Ѵ�. �� ����, ���� ��� �� ��Ʈ����� üũ�� ���ԵǴ� �ٸ� �Ķ���Ϳ� ���� AVI ������ ǥ���ϱ� ���ؼ�, �ý����� ó���ؾ� �ϴ� ������ ����Ʈ���� ��Ÿ����.
		DWORD	dwPaddingGranularity;	// �������� �ƶ��̸�Ʈ�� ����Ʈ ������ �����Ѵ�. �� ���� ����� �����͸� �е� �Ѵ�.
		DWORD	dwFlags;				// ������ �÷����� 0 �� �̻��� ��Ʈ���� ���� ���� �Ѵ�.
		DWORD	dwTotalFrames;			// ���ϳ��� �������� �������� �Ѽ��� �����Ѵ�.
		DWORD	dwInitialFrames;		// ���͸��� �� ������ ���� �������� �����Ѵ�. ���͸��� �� ���� �̿ܿ�����, 0 �� �����Ѵ�. ���͸��� �� ������ ���� �ϴ� ���, ���ϳ����� AVI ������ ���� �����Ӻ��� ���� �ִ� �����Ӽ���, �� ����� �����Ѵ�.
		DWORD	dwStreams;				// ���ϳ��� ��Ʈ������ �����Ѵ�. ���� ���, ������� ������ ������ ���Ͽ��� 2 ���� ��Ʈ���� �ִ�.
		DWORD	dwSuggestedBufferSize;	// ������ �о�� ���� ���� ����� �����Ѵ�. �Ϲ�������, �� ������� ���ϳ��� �ִ��� üũ�� ���� �ϴµ� ����� ũ��� �Ѵ�. 0 ���� �����ϰų� �ʹ� ���� ������ �������� ���, ��� ����Ʈ����� ����߿� �޸𸮸� ������ ������ ���� ������ �ȵǾ�, �����ս��� �����Ѵ�. ���͸��� �� ������ ���, ���� ������� üũ�� �ƴϰ� ���ڵ� ��ü�� �о�µ� ����� ũ�Ⱑ �ƴϸ� �ȵȴ�.
		DWORD	dwWidth;				// AVI ������ ���� �����Ѵ� (�ȼ� ����).
		DWORD	dwHeight;				// AVI ������ ���̸� �����Ѵ� (�ȼ� ����).
		DWORD	dwReserved[4];			// ������ ���� ����. �� �迭�� ���η� �����Ѵ�.
	} __attribute__ ((packed)) mhData;
} __attribute__ ((packed)) AVIMAINHEADER;

// AVIMAINHEADER - dwFlags
#define AVIF_HASINDEX		 0x00000010 // Index at end of file?
#define AVIF_MUSTUSEINDEX	 0x00000020
#define AVIF_ISINTERLEAVED	 0x00000100
#define AVIF_TRUSTCKTYPE	 0x00000800 // Use CKType to find key frames
#define AVIF_WASCAPTUREFILE  0x00010000
#define AVIF_COPYRIGHTED	 0x00020000

/*
  @fcccb : Specifies a FOURCC code.  structure, not including the initial 8 bytes. 
  
  @fccType  
   Contains a FOURCC that specifies the type of the data contained in the stream.
   The following standard AVI values for video and audio are defined. 
   
   ------------------------------------- 
    FOURCC   Description
   ------------------------------------- 
   'auds'    Audio stream
   -------------------------------------
   'mids'    MIDI stream
   -------------------------------------
   'txts'    Text stream
   -------------------------------------
   'vids'    Video stream
   -------------------------------------
 
  @fccHandler
    Optionally, contains a FOURCC that identifies a specific data handler. The 
    data handler is the preferred handler for the stream. For audio and video 
    streams, this specifies the codec for decoding the stream. 

  @dwFlags
    Contains any flags for the data stream. The bits in the high-order word of 
    these flags are specific to the type of data contained in the stream. The 
    following standard flags are defined. 

    ----------------------------------------------------------------------------
    Value                   Meaning
    ----------------------------------------------------------------------------
    AVISF_DISABLED          Indicates this stream should not be enabled by default. 
    ----------------------------------------------------------------------------
    AVISF_VIDEO_PALCHANGES  Indicates this video stream contains palette changes.
                            This flag warns the playback software that it will 
                            need to animate the palette. 
    ----------------------------------------------------------------------------
 
  @wPriority
    Specifies priority of a stream type. For example, in a file with multiple 
    audio streams, the one with the highest priority might be the default stream. 

  @wLanguage
    Language tag. 

  @dwInitialFrames
    Specifies how far audio data is skewed ahead of the video frames in 
    interleaved files. Typically, this is about 0.75 seconds. If you are creating 
    interleaved files, specify the number of frames in the file prior to the 
    initial frame of the AVI sequence in this member. For more information, see 
    the remarks for the dwInitialFrames member of the AVIMAINHEADER structure.

  @dwScale
    Used with dwRate to specify the time scale that this stream will use. 
    Dividing dwRate by dwScale gives the number of samples per second. For video 
    streams, this is the frame rate. For audio streams, this rate corresponds to 
    the time needed to play nBlockAlign bytes of audio, which for PCM audio is 
    the just the sample rate. 

  @dwRate
    See dwScale. 

  @dwStart
    Specifies the starting time for this stream. The units are defined by the 
    dwRate and dwScale members in the main file header. Usually, this is zero, 
    but it can specify a delay time for a stream that does not start concurrently 
    with the file.

  @dwLength
    Specifies the length of this stream. The units are defined by the dwRate and 
    dwScale members of the stream's header.

  @dwSuggestedBufferSize
    Specifies how large a buffer should be used to read this stream. Typically, 
    this contains a value corresponding to the largest chunk present in the stream
    . Using the correct buffer size makes playback more efficient. Use zero if 
    you do not know the correct buffer size. 

  @dwQuality
    Specifies an indicator of the quality of the data in the stream. Quality is 
    represented as a number between 0 and 10,000. For compressed data, this 
    typically represents the value of the quality parameter passed to the 
    compression software. If set to ?1, drivers use the default quality value. 

  @dwSampleSize
    Specifies the size of a single sample of data. This is set to zero if the 
    samples can vary in size. If this number is nonzero, then multiple samples of 
    data can be grouped into a single chunk within the file. If it is zero, each 
    sample of data (such as a video frame) must be in a separate chunk. For video 
    streams, this number is typically zero, although it can be nonzero if all 
    video frames are the same size. For audio streams, this number should be the 
    same as the nBlockAlign member of the WAVEFORMATEX structure describing the 
    audio. 

  @rcFrame
    Specifies the destination rectangle for a text or video stream within the 
    movie rectangle specified by the dwWidth and dwHeight members of the AVI main 
    header structure. The rcFrame member is typically used in support of multiple 
    video streams. Set this rectangle to the coordinates corresponding to the 
    movie rectangle to update the whole movie rectangle. Units for this member 
    are pixels. The upper-left corner of the destination rectangle is relative to 
    the upper-left corner of the movie rectangle. 
*/
typedef struct {
	FCC_CB_t fcccb; 					// Specifies a FOURCC code. The value must be 'strh'. 
	struct {
		FOURCC	fccType;				// ��Ʈ���� ���ԵǴ� �������� Ÿ���� �����ϴ� FOURCC �� �����Ѵ�. ���� �� ������� ���ؼ� ǥ�� AVI ���� ���ǵǰ� �ִ�. auds(�����), mids(MIDI), txts(�ؽ�Ʈ), vids(����)
		FOURCC	fccHandler;				// Ư���� ������ �ڵ鷯�� ��Ÿ���� FOURCC �� �����Ѵ� (�ɼ�). ������ �ڵ鷯��, ��Ʈ���� ���ؼ� ������ �ڵ鷯�̴�. ����� ��Ʈ�� �Ǵ� ���� ��Ʈ���� ����, ��Ʈ���� ���ڵ��ϴ� �ڵ��� �����Ѵ�. 
		DWORD	dwFlags;				// ������ ��Ʈ���� ���� �÷��׸� �����Ѵ�. �̷��� �÷����� ���� ������ ��Ʈ��, ��Ʈ���� ���ԵǴ� �������� Ÿ�Կ� �����ϴ�.
		WORD	wPriority;				// ��Ʈ�� Ÿ���� �켱 ������ �����Ѵ�. ���� ���, ������ ����� ��Ʈ���� ������ ���Ͽ�����, �켱 ������ ���� ���� ��Ʈ���� ����Ʈ�� ��Ʈ���� �ȴ�. 
		WORD	wLanguage;				// 
		DWORD	dwInitialFrames;		// ���͸��� �� ���Ϸ�, ����� �����Ͱ� ���� ���������κ��� ��� ���� skew �ǰ� �ִ����� �����Ѵ�. ������, �� 0.75 ���̴�. ���͸��� �� ������ ���� �ϴ� ���, ���ϳ����� AVI ������ ���� �����Ӻ��� ���� �ִ� �����Ӽ���, �� ����� �����Ѵ�.
		DWORD	dwScale;				// dwRate �� �Բ� �����, �� ��Ʈ���� ����ϴ� Ÿ���� ������ �����Ѵ�. dwRate �� dwScale �� ������ ������, 1 �� ���� ���ü��� �䱸�ȴ�. ���� ��Ʈ���� ���, �̰��� frame rate�̴�. ����� ��Ʈ���� ���, �� ����Ʈ�� nBlockAlign ����Ʈ�� ������� ����� �ʿ��� �ð��� ����Ѵ�. �̰��� PCM ������� ���� ���� ����Ʈ�� ����������.
		DWORD	dwRate;					// dwScale �� �����Ұ�. 
		DWORD	dwStart;				// �� ��Ʈ���� ���� Ÿ���� �����Ѵ�. ������, ���� ���� ����� dwRate �� dwScale ����� ���� ���ǵȴ�.
		DWORD	dwLength;				// �� ��Ʈ���� ���̸� �����Ѵ�. ������, ��Ʈ���� ����� dwRate �� dwScale ����� ���� ���ǵȴ�. 
		DWORD	dwSuggestedBufferSize;	// �� ��Ʈ���� �о�� ���ؼ�(������) �ʿ��� ������ ũ�⸦ �����Ѵ�. ������, ��Ʈ������ �ִ��� üũ�� �����ϴ� ���̴�. �ùٸ� ���� ����� ����ϴ� ������, ����� ȿ���� ��������. �ùٸ� ���� ����� �𸣴� ����, 0 �� �����Ѵ�. 
		DWORD	dwQuality;				// ��Ʈ������ �������� ǰ���� ��Ÿ���� ���� �����Ѵ�. ǰ����, 0 ~ 10,000 �� ������ ������ ��Ÿ����. ���� �������� ���, �̰��� ����, ���� ����Ʈ����� �ǳ����� ǰ�� �Ķ������ ���� ��Ÿ����. -1 (��)�� �������� ���, ����̹��� ����Ʈ�� ǰ������ ����Ѵ�. 
		DWORD	dwSampleSize;			// �������� 1 ������ ����� �����Ѵ�. ������ ����� ��ȭ�ϴ� ����, 0 ���� �����Ѵ�. �� ���� 0 �� �ƴ� ���, ���ϳ����� ������ ������ ������ 1 ���� üũ�� �׷�ȭ �� �� �ִ�. 0 �� ���, �� ������ ���� (���� �����ӵ�)�� ���� �ٸ� üũ�� ���Ե��� ������ �� �ȴ�. ���� ��Ʈ���� ���, �� ���� ���� 0 ������, ��� ���� �������� ���� �������̸�, 0 �̿��� ������ �� �� �ִ�. ����� ��Ʈ���� ���, �� ���� ������� ����ϴ� WAVEFORMATEX ����ü�� nBlockAlign ����� ���� ������ �� �ȴ�. 
		struct {
			short int	left;
			short int	top;
			short int	right;
			short int	bottom;
		} rcFrame;						// AVI ���� ��� ����ü�� dwWidth �� dwHeight ����� ���� �����Ǵ� ������ ���簢������ �ؽ�Ʈ �Ǵ� ���� ��Ʈ���� ���� ������ ���簢���� �����Ѵ�. ����,rcFrame �����, ������ ���� ��Ʈ���� ���� �ϱ� ���ؼ� ���ȴ�. �� ���簢����, ������ ���簢���� �����ϴ� ��ǥ�� ������, ������ ���簢�� ��ü�� �����Ѵ�. �� ����� ������ �ȼ��̴�. ������ ���簢���� �»󱸼���, ������ ���簢���� �»󱸼����κ����� ��� ������ �ȴ�.
	} __attribute__ ((packed)) shData;
} __attribute__ ((packed)) AVISTREAMHEADER;

// AVISTREAMHEADER - dwFlags
#define AVISF_DISABLED			(0x00000001)
#define AVISF_VIDEO_PALCHANGES	(0x00010000)

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'strf' �� �ƴϸ� �ȵȴ�.
	struct {
		DWORD	biSize; 				// ����ü�� �ʿ�� �ϴ� ����Ʈ���� �����Ѵ�. ����ü�� �÷� ���̺��̳� �÷� ����ũ�� �ڿ� ��ӵǴ� ���, �� �÷� ���̺��̳� �÷� ����ũ�� ������� �� ������ ���Ե��� �ʴ´�. ���Ǹ� �����Ұ�. 
		LONG	biWidth;				// ��Ʈ ���� ���� �����Ѵ�. 
		LONG	biHeight;				// ��Ʈ ���� ���̸� �����Ѵ� (�ȼ� ����). 
		WORD	biPlanes;				// Ÿ�� ��ġ�� ���� ���� ���� �����Ѵ�. �̰��� �ݵ�� 1 ���� �����Ѵ�. 
		WORD	biBitCount; 			// 1 �ȼ� ��ó�� ��Ʈ���� �����Ѵ�. 
		FOURCC	biCompression;			// ��Ʈ ���� ����ǰ� �ִ� ���, �� ����� ������ �����ϴ� FOURCC �̴�.
		DWORD	biSizeImage;			// �̹����� ����� �����Ѵ� (����Ʈ ����). ����� RGB ��Ʈ ���� ����, 0 ���� ������ �� �ִ�. 
		LONG	biXPelsPerMeter;		// ��Ʈ ���� Ÿ�� ��ġ�� ���� �ػ󵵸� 1 ���� ���� �ȼ� ������ �����Ѵ�. 
		LONG	biYPelsPerMeter;		// ��Ʈ ���� Ÿ�� ��ġ�� ���� �ػ󵵸� 1 ���� ���� �ȼ� ������ �����Ѵ�. 
		DWORD	biClrUsed;				// �÷� ���̺��� �÷� �ε��� ���, ��Ʈ �ʳ����� ������ ����ϴ� �ε����� ���� �����Ѵ�.
		DWORD	biClrImportant; 		// ��Ʈ ���� ǥ���ϱ� ���ؼ� �߿��� �������� �÷� �ε������� �����Ѵ�. �� ���� ������ ����, ��� ���� �߿��� ��������. 
	} __attribute__ ((packed)) bmData;
} __attribute__ ((packed)) BITMAPINFOHEADER;

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'strf' �� �ƴϸ� �ȵȴ�.
	struct { 
		WORD	wFormatTag; 			// ���� ������� ���� Ÿ��. ���� �±״�, ���� ���� �˰�������� Microsoft Corporation �� ��ϵǾ� �ִ�. ���� �±��� ������ ����Ʈ��, Mmreg.h ��� ���Ͽ� �ִ�. 1 ä���̳� 2 ä���� PCM (Pulse Code Modulation) �������� ���, �� ���� WAVE_FORMAT_PCM �� �� �ʿ䰡 �ִ�. 
		WORD	nChannels;				// ���� ����� �����Ϳ� ���ԵǴ� ä�μ�. ��û�� �����ʹ� 1 ä���� �����, ���׷��� �����ʹ� 2 ä���� ����Ѵ�. 
		DWORD	nSamplesPerSec; 		// ����/�ʿ� ��Ÿ���� ���� ����Ʈ (���� Hz). wFormatTag �� WAVE_FORMAT_PCM �� ���,nSamplesPerSec �� �Ϲ����� ���� 8.0 kHz, 11.025 kHz, 22.05 kHz, 44.1 kHz �� �ȴ�. �� PCM ������ ���, �� ����� ���������� ���� �±� ��翡 ���� ������ �ʿ䰡 �ִ�. 
		DWORD	nAvgBytesPerSec;		// ���� �±׿� �ʿ��� ��� ������ ���� ����Ʈ (���� ����Ʈ/��). wFormatTag �� WAVE_FORMAT_PCM �� ���,nAvgBytesPerSec �� nSamplesPerSec �� nBlockAlign �� ���� �����ؾ� �Ѵ�. �� PCM ������ ���, �� ����� ���������� ���� �±� ��翡 ���� ������ �ʿ䰡 �ִ�. 
		WORD	nBlockAlign;			// ��� alignment (���� ����Ʈ). ��� alignment��,wFormatTag ���� Ÿ���� �������� �ּ� ���� �����̴�. wFormatTag �� WAVE_FORMAT_PCM �Ǵ� WAVE_FORMAT_EXTENSIBLE �� ���,nBlockAlign ��nChannels �� wBitsPerSample �� ���� 8 (1 ����Ʈ ���� ��Ʈ��)���� ���� ���� �����ؾ� �Ѵ�. �� PCM ������ ���, �� ����� ���������� ���� �±� ��翡 ���� ������ �ʿ䰡 �ִ�. ����Ʈ�����, �� ���� ������ nBlockAlign ����Ʈ�� �����͸� ó���� �ʿ䰡 �ִ�. ��ġ�� ���� �������� ����� �б�� �׻�, ����� ���ηκ��� ���� �ؾ� �Ѵ�. ���� ���, PCM �������� ����� ������ ���� (��, ���� �ƶ��� ����)���� ���� �ϴ� ���� �����ϴ�. 
		WORD	wBitsPerSample; 		// wFormatTag ���� Ÿ���� 1 ���� ��ó�� ��Ʈ��. wFormatTag �� WAVE_FORMAT_PCM �� ���,wBitsPerSample �� 8 �Ǵ� 16 �� �ƴϸ� �ȵȴ�. wFormatTag �� WAVE_FORMAT_EXTENSIBLE �� ���, �� ����, ������ 8 �� ����� ������ �� �ִ�. �Ϻ��� ���� ��Ŵ�� wBitsPerSample �� ���� ������ �� ���� ������, �� ������� 0 �� �����ص� �������. 
		WORD	cbSize; 				// WAVEFORMATEX ����ü�� �������� �߰��Ǵ� �߰� ���� ������ ������ (���� ����Ʈ). �� PCM ������, �� ������ ����� wFormatTag �� �߰� �Ӽ��� ���� �� �� �ִ�. wFormatTag �� �߰� ������ �ʿ���� ����, �� ����� ���η� �����ؾ� �Ѵ�. WAVE_FORMAT_PCM ���� �ۿ� ���� ���, �� ����� ���õȴ�. 
	} __attribute__ ((packed)) wmData; 
} __attribute__ ((packed)) WAVEFORMATEX;

typedef struct {
	FCC_CB_t fcccb; 					// ���� 'strf' �� �ƴϸ� �ȵȴ�.
} __attribute__ ((packed)) TXTSFORMATEX;

typedef struct {
	FOURCC	dwChunkId;					// AVI ���ϳ��� ��Ʈ���� �ĺ��ϴ� FOURCC �� �����Ѵ�. FOURCC �� 'xxyy'�� ������ �ƴϸ� �ȵȴ�. xx �� ��Ʈ�� ��ȣ, yy �� ��Ʈ���� ������ �ĺ��ϴ� 2 �ڸ����� ���� �ڵ��̴�. db������� ���� ������. dc����� ���� ������. pc�ȷ�Ʈ�� ����. wb����� ������. 
	DWORD	dwFlags;					// ������ �÷����� 0 �� �̻��� ��Ʈ���� ���� �����Ѵ�. AVIIF_KEYFRAME ����Ÿüũ�� Ű �������̴�. AVIIF_LIST ����Ÿüũ�� 'rec'����Ʈ�̴�. AVIIF_NO_TIME ����Ÿüũ�� ��Ʈ���� Ÿ�ֿ̹� ������ ���� �ʴ´�. ���� ���, �� �÷��״� �ȷ�Ʈ ���� �ÿ� �����Ѵ�. 
	DWORD	dwOffset;					// ���ϳ��� ����Ÿüũ�� ��ġ�� �����Ѵ�. ����, ����Ʈ ������ 'movi'����Ʈ�� ���ηκ��� ������(offset)�μ� ������ �ʿ䰡 �ִ�. �ٸ�, �Ϻ��� AVI ���Ͽ�����, ������ ���ηκ����� ������(offset)�μ� �����Ѵ�. 
	DWORD	dwSize;						// ����Ÿüũ�� ����� ����Ʈ ������ �����Ѵ�. 
} __attribute__ ((packed)) AVIINDEX_t;

// AVIOLDINDEX - dwFlags
#define AVIIF_LIST		 (0x00000001)
#define AVIIF_KEYFRAME	 (0x00000010)
#define AVIIF_NO_TIME	 (0x00000100)
#define AVIIF_COMPRESSOR (0x0FFF0000)

/*
LIST ('strl'
      'strh'(<Stream header>)
      'strf'(<Stream format>)
*/
typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	BITMAPINFOHEADER info;
} __attribute__ ((packed)) avi_videostream_t;

typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	WAVEFORMATEX info;
} __attribute__ ((packed)) avi_audiostream_t;

typedef struct {
	RIFFLIST list;
	AVISTREAMHEADER stream;
	TXTSFORMATEX info;
} __attribute__ ((packed)) avi_accscstream_t;

/*
RIFF ('AVI '
      LIST ('hdrl'
            'avih'(<Main AVI Header>)
*/
typedef struct {
	RIFFLIST head;			// RIFF size  AVI
	RIFFLIST list;			// LIST size hdrl
	AVIMAINHEADER data;		// avih size mhData, 'avih' chunk
} __attribute__ ((packed)) AVI_HEADER_t;


// Playback //============================//
#define RIFF_ID (0x52494646)
#define AVI_TYPE (0x41564920)
#define LIST_ID (0x4c495354)
#define HDRL_TYPE (0x6864726c)
#define AVIH_ID (0x61766968)
#define STRL_TYPE (0x7374726c)
#define STRH_ID (0x73747268)
#define VIDS_TYPE (0x76696473)
#define H264_CODEC (0x48323634)
#define AUDS_TYPE (0x61756473)
#define G711_CODEC (0x01000000)
#define STRF_ID (0x73747266)
#define MOVI_TYPE (0x6d6f7669)
#define VIDEO_00dc_ID (0x30306463)
#define AUDIO_01wb_ID (0x30317762)
#define TEXT_02tx_ID (0x30327478)
#define IDX1_ID	(0x69647831)
#define VIDEO_ENDID (IDX1_ID)
#define PLAYBACK_DATA_SIZE (160*1024)
#define PLAYBACK_MAX_FRAME (5400)
//���� �ش�
typedef struct {
	AVI_HEADER_t hdrl;
	avi_videostream_t strlVideo;
	avi_audiostream_t strlAudio;
} __attribute__ ((packed)) AviHdrl_t; 

//0xff4 video ����
typedef struct {
	RIFFLIST movi;
} __attribute__ ((packed)) AviMovi_t; 

typedef struct{	
	FCC_CB_t vinfo;
	BYTE *vdata;
	UINT prereadsize;
	UINT readsize;
} __attribute__ ((packed)) AviVDatas_t;

typedef struct{
	FCC_CB_t ainfo;
	BYTE *adata;
	UINT prereadsize;
	UINT readsize;
} __attribute__ ((packed)) AviADatas_t;

typedef struct{
	FCC_CB_t tinfo;
	BYTE *tdata;
	UINT prereadsize;
	UINT readsize;
} __attribute__ ((packed)) AviTDatas_t;

typedef struct {
#if 0	
	AVIINDEX_t videoidxdata;
	AVIINDEX_t audioidxdata;
	AVIINDEX_t gpsidxdata;
#else
	AVIINDEX_t data;
#endif

} __attribute__ ((packed)) AviIdxDatas_t;

//0xff4 + 8 + video size  index ����
typedef struct {
	FCC_CB_t idxinfo;	
} __attribute__ ((packed)) AviIdx_t; 	

typedef struct {
	AviHdrl_t ListHdrl;		
	AviMovi_t ListMovi;
	AviIdx_t ListIdx;
	AviIdxDatas_t *idxdata;
	AviVDatas_t v;
	AviADatas_t a;
	AviTDatas_t t;
	UINT totalframes;
	UINT fps;
	int index;
	int fileidx;
}__attribute__ ((packed)) AVI_PARSER_t;
///=====================================//

//JPEG EXIF================================//
#define SOI_MARKER 0xFFD8
#define APP1_MARKER 0xFFE1
#define EXIF_HEADER 0x457869660000
#define TIFF_HEADER 0x4949
#define TIFF_OFFSET 0x8000000
#define EXIF_SUBIFD 0x6987

#define TAG_DATETIME 	0x3201
#define TAG_LIGHTSOURCE 0x0892
#define TAG_ORIENTATION 0x1201
#define TAG_IMAGEWIDTH  0x0001
#define TAG_IMAGELENGTH 0x0101
#define TAG_METERINGMODE 0x0792
#define TAG_GPSINFO		0x2588
#define TAG_GPSLATITUDE 0x0200
#define TAG_GPSLATITUDEREF 0x0100
#define TAG_GPSLONGITUDE 0x0400
#define TAG_GPSLONGITUDEREF 0x0300

#define DATA_FORMAT_UBYTE 		0x0100 //1byte
#define DATA_FORMAT_ASCII_STR 	0x0200 //1byte
#define DATA_FORMAT_USHORT 		0x0300 //2byte
#define DATA_FORMAT_ULONG 		0x0400 //4byte
#define DATA_FORMAT_URATIONAL	0x0500 //8byte
#define DATA_FORMAT_BYTE		0x0600 //1byte
#define DATA_FORMAT_UNDEFINED	0x0700 //1byte
#define DATA_FORMAT_SHORT		0x0800 //2byte
#define DATA_FORMAT_LONG		0x0900 //4byte
#define DATA_FORMAT_RATIONAL	0x0a00 //8byte
#define DATA_FORMAT_FLOAT		0x0b00 //4byte
#define DATA_FORMAT_DOUBLE_FLOAT 0x0c00 //8byte
//======================================//

//JUNK size
#define JUNKSIZE_VAT4	(3426)	// Video(124) + Audio(102) + Text(84)*4
#define JUNKSIZE_VAT3	(3510)	// Video(124) + Audio(102) + Text(84)*3
#define JUNKSIZE_VAT2	(3594)	//3382// Video(124+28) + Audio(102+28) + Text(84+28)*2
#define JUNKSIZE_VAT1	(3678)	// Video(124) + Audio(102) + Text(84)*1

#define JUNKSIZE_VT4	(3528)	// Video(124) + Text(84)*4
#define JUNKSIZE_VT3	(3612)	// Video(124) + Text(84)*3
#define JUNKSIZE_VT2	(3696)	// Video(124) + Text(84)*2
#define JUNKSIZE_VT1	(3780)	// Video(124) + Text(84)*1

#define JUNKSIZE_VA		(3762)	// Video(124) + Audio(102)

#define JUNKSIZE_V		(3864)	// Video(124)

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern FCC_CB_t avi_write_chunkheader(UINT type, UINT data_size);
extern AVIINDEX_t avi_write_index(UINT type, UINT data_pos, UINT data_size, UINT key);
extern BYTE *avi_write_header(t_avi_t *avia);

//-------------------------------------------------------------------------------------------------
// Variable
#define AUD_DATA_BYTE 			(800)
#define AUD_SAMPLING_RATE 		(8000)
#define AUD_SAMPLING_BIT  		(8)

#define MIX_FREE_SD_4CENT		(1)
#define MAX_JPEG_FILE_SIZE   	(512*1024) //Max 4Mbyte.
#define MAX_AVI_FILE_SIZE   	(100*1024*1024)// 8Mbps
#define EVN_AVI_FILE_SIZE   	(16*1024*1024) // 8Mbps
#define DEFAULT_RATIO_NORMAL    (90)//90%NORMAL 10% EVENT
#define BYTE_PER_CLUST			(32*1024)
#ifdef  CONTIGUOUS_WRITE
//#define MAX_AVI_FILE_SIZE   250*1024*1024 // 10Mbps
//#define EVN_AVI_FILE_SIZE   15*1024*1024 // 10Mbps
#endif

#define MINIMUM_FREE_SIZE 	(750*1024)

#endif
