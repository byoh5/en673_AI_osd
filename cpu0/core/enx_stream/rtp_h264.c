/* Copyright (c) 2013-15 Eyenix Corporation. All rights reserved.
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

#include "lwip/tcp.h"
#include "lwip/udp.h"

#include "main.h"
#include "rtp_h264.h"
#include "h264.h"

#include "rtsp_cmd.h"
#include "rtsp_control.h"
#include "stream_buffering.h"

#include "rtp_hdr_gen.h"

#if 1
#define PacketSize		(TCP_MSS)	// == TCP_MSS
#define InterleavedSize	4
#define RTPHeaderSize	12
#define FUHeaderSize	2
#define HeaderSize		18
#define PayloadMaxSize	(PacketSize - HeaderSize)
#else
#define PacketSize		(TCP_MSS)
#define InterleavedSize	sizeof(rthInterleaved)
#define RTPHeaderSize	sizeof(rthRTPHeader)
#define FUHeaderSize	sizeof(rthFUHeader)
#define HeaderSize		(InterleavedSize + RTPHeaderSize + FUHeaderSize)
#define PayloadMaxSize	(PacketSize - HeaderSize)
#endif

typedef struct {
	rthInterleaved* Interleaved;// write
	rthRTPHeader* RTPHeader;	// write
	rthFUHeader* FUHeader;		// write
	uint8* payload;				// write

	uint8* imageData;			// H.264 Output data

	uint32 un32PacketLength;	// tcp_write용 all size (Interleaved size + RTPHeader size + payload size)
	uint8 un8PacketState;		// build 결과
	uint8* un8pPacket;			// tcp_write용 all data (Interleaved + RTPHeader + payload)
}__attribute__ ((packed)) rthPacket;

static inline void buildInterleavedInit(rthInterleaved* pk) {
	pk->un8Magic		= 0x24;			// 0x24 : '$'
//	pk->un8Channel		= 0x00;			// 0x00
//	pk->un16Length		= 0x0000;		// 0x0000
}

static inline void buildRTPHeaderInit(rthRTPHeader* pk) {
	pk->un2RTPVersion 	= 0x2;			// 0x2 : Ver 2.0	: 고정
	pk->un1Padding 		= 0x0;			// 0x0				: 고정
	pk->un1Extension 	= 0x0;			// 0x0				: 고정
	pk->un4CSRCcount 	= 0x0;			// 0x0				: 고정
	pk->un1Markerbit 	= 0x0;			// 0x0				: 0, 1
	pk->un7Payloadtype 	= 0x60;			// 0x60				: 96, 97, 98, 99 ... 
//	pk->un16Sequencenum = 0x0;			// seq++			: 매 패킷마다 ++
//	pk->un32Timestamp 	= 0x0;			// ts = rtptime		: ts update, 1프레임당 update
//	pk->un32SSIdentifier= 0x0;			// random			: 1회 연결마다 랜덤값 후 고정
}

static inline void buildFUHeaderInit(rthFUHeader* pk) {
	pk->un1Fbit			= 0x0;			// 0x0				: 고정
//	pk->un2NRI			= 0x0;			// 0, 2, 3			: Nal_ref_idc
	pk->un5Type			= 0x1C;			// 0x1C				: Type : FU-A
	pk->un1Startbit		= 0x0;			// 0x0 or 0x1		: 한 frame의 start
	pk->un1Endbit		= 0x0;			// 0x0 or 0x1		: 한 frame의 end
	pk->un1Forbiddenbit	= 0x0;			// 0x0				: 고정
//	pk->un5Nalunittype	= 0x0;			// 0x1, 5, 7, 8		: non-IDR, IDR, SPS, PPS
}

#define BUILD_TEST 0
#if (BUILD_TEST==1)
static INT test_FU = 0;
#endif

static void buildPacket(rthPacket* pk, rtsp_con_t* prtsp_con) {
    rtsp_session_t* prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t* prtp_ses = &prtsp_ses->prtp_s[VIDEO];
	rtp_tx_info_t *prtp_info = &prtsp_con->prtp_info;
	volatile uint32 un32FtchNum = 0;
	uint32 i, j;

	// NAL unit 관련 변수
	uint32 un32NALzeroCount = 0;
	uint32 un32NalUnitType = 0;

	// Header(include) size 관련 변수
	uint32 un32Interleavedsize = 0;	// Interleaved가 붙으면 InterleavedSize, 아니면 0, TCP only
	uint32 un32FUsize = 0;			// FU가 붙으면 FUHeaderSize, 아니면 0, 하나의 frame이 2 packet로 갈 경우에만... SPS,PPS 제외

	// Remaining data size check
	if (prtp_info->left == 0) {
		pk->un8PacketState = 1;	// NO DATA, 진입하면 안된다. 사전에 필터링 되야 함
		printf("%s(%d) : Error(%d)\r\n", __func__, __LINE__, prtp_info->nsize);
		return;
	}

	// Maximum data size that can be transmitted / set payload address
	uint32 un32SendSize = 0;
	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			un32Interleavedsize = 0;
			break;
		case RTP_TCP:
		case RTP_HTTP:
			un32Interleavedsize = InterleavedSize;
			break;
	}

	un32SendSize = (PacketSize - un32Interleavedsize - RTPHeaderSize);
	pk->payload = (uint8 *)(pk->un8pPacket + un32Interleavedsize + RTPHeaderSize);

	// NAL unit test(Max 20byte)
	if (prtp_info->data[0] == 0x00) {
		u32 loop = LWIP_MIN(prtp_info->left, 20);
		for (i = 0; i < loop; i++) {
			if (prtp_info->data[i] == 0x00 && prtp_info->data[i + 1] == 0x00 && prtp_info->data[i + 2] == 0x01) {
				un32NALzeroCount = i + 3;
				break;
			}
		}
		if ((loop == prtp_info->left) && (un32NALzeroCount != 0)) {
			printf("left(%d) NALzeroCount(%d)\r\n", prtp_info->left, un32NALzeroCount);
		}
	}

	if(un32NALzeroCount) {
		// NAL unit(0x00 0x00 0x00 0x01 or 0x00 0x00 0x01) does not transmit
		prtp_info->data += un32NALzeroCount;
		prtp_info->left -= un32NALzeroCount;

		// Get : NAL unit type
		un32NalUnitType = prtp_info->data[0] & 0x1F;
		switch(un32NalUnitType) {
			case 0x01:	// P
#if (BUILD_TEST==1)
				prtsp_con->nal_count++;
				prtsp_con->nal_dump[prtsp_con->nal_count] = prtp_info->nsize;
#endif
				if (prtp_info->left <= un32SendSize) {
					un32FtchNum = prtp_info->left;
					un32FUsize = 0;				// not include FU
				} else {
					un32FtchNum = un32SendSize;
					un32FUsize = FUHeaderSize;	// include FU
				}
				break;

			case 0x05:	// I
#if (BUILD_TEST==1)
				prtsp_con->nal_count++;
				if(prtsp_con->build_num != 30) {
					printf("Frame Count Error!![%d/%d]\r\n", prtsp_con->build_num, prtsp_con->nal_count);
					int aa;
					for(aa=0;aa<30;aa++) {
						printf("%d\t%d\r\n", aa+1, prtsp_con->nal_dump[aa]);
						prtsp_con->nal_dump[aa] = 0;
					}
					printf("\r\n=============================\r\n");
				}
				prtsp_con->build_num = 0;
				prtsp_con->nal_count = 0;
				prtsp_con->nal_dump[prtsp_con->nal_count] = prtp_info->nsize;
#endif
				if (prtp_info->left <= un32SendSize) {
					un32FtchNum = prtp_info->left;
					un32FUsize = 0;				// not include FU
				} else {
					un32FtchNum = un32SendSize;
					un32FUsize = FUHeaderSize;	// include FU
				}
				break;

			case 0x07:	// SPS
			case 0x08:	// PPS
				prtsp_ses->play = 1;	// Start!
				un32FUsize = 0;			// not include FU

				// SPS/PPS payload size check
				for (i = 0, j = 0; i < 20; i++) {
					if (prtp_info->data[i] == 0x00) {
						j++;
						if (prtp_info->data[i + 1] == 0x01 && j >= 2) {
							un32FtchNum = i + 1 - j;
							break;
						}
					} else {
						j = 0;
					}
				}
				break;

			default:
				printf("%s(%d) : NAL unit Error(Jump:%d)\r\n", __func__, __LINE__, un32NALzeroCount);
				hexDump("NAL TEST", (void *)(prtp_info->data-un32NALzeroCount), 32);
				pk->un8PacketState = 2;	// NO DATA
				return;
		}
	} else { // next packet
		un32FUsize = FUHeaderSize;		// include FU
	}

	// include FU header
	if (un32FUsize) {
		pk->FUHeader = (rthFUHeader *)pk->payload;
		pk->payload += un32FUsize;
		buildFUHeaderInit(pk->FUHeader);

		// frist packet
		if (un32NALzeroCount) {
			// 현재 Frame의 next Packet에서 사용하기 위해서 FU_NalUnitType변수에 저장
			prtp_info->un32FU_NalUnitType = un32NalUnitType;
			prtp_info->un32FU_NalRefIdc = (prtp_info->data[0] & 0x60) >> 5;

			pk->FUHeader->un1Startbit = 0x1;
#if (BUILD_TEST==1)
			test_FU++;
#endif

			// NAL type delete(0x61, 0x65)
			prtp_info->data += 1;
			prtp_info->left -= 1;
		}

		pk->FUHeader->un5Nalunittype = prtp_info->un32FU_NalUnitType;
		pk->FUHeader->un2NRI = prtp_info->un32FU_NalRefIdc;

		un32SendSize -= un32FUsize;

		// send size check
		if (prtp_info->left <= un32SendSize) {
			un32FtchNum = prtp_info->left;
			pk->FUHeader->un1Endbit = 0x1;
#if (BUILD_TEST==1)
			test_FU--;
#endif
		} else {
			un32FtchNum = un32SendSize;
		}		
	}

#if (BUILD_TEST==1)
	if (test_FU < 0 || test_FU > 1){
		printf("test_FU error(%d)\r\n", test_FU);
	}
#endif

	// total packet size
	pk->un32PacketLength = un32Interleavedsize + RTPHeaderSize + un32FUsize + un32FtchNum;

	// include Interleaved/RTP header / set interleaved frame
	switch (prtsp_ses->rtp_type) {
		case RTP_UDP:
			break;
		case RTP_TCP:
		case RTP_HTTP:
			pk->Interleaved = (rthInterleaved *)pk->un8pPacket;
			buildInterleavedInit(pk->Interleaved);
			pk->Interleaved->un8Channel = (uint8)prtsp_ses->rtp_port[VIDEO];
			pk->Interleaved->un16Length = (uint16)(RTPHeaderSize + un32FUsize + un32FtchNum);
			break;
	}

	// set rtp header
	pk->RTPHeader = (rthRTPHeader *)(pk->un8pPacket + un32Interleavedsize);
	buildRTPHeaderInit(pk->RTPHeader);
	if (prtp_info->left == un32FtchNum) {
		pk->RTPHeader->un1Markerbit = 0x1;
#if (BUILD_TEST==1)
		prtsp_con->build_num++;
	} else if (prtp_info->left < un32FtchNum) {
		printf("%s(%d) : error? (%d<%d)\r\n", __func__, __LINE__, prtp_info->left, un32FtchNum);
#endif
	}
//	pk->RTPHeader->un7Payloadtype = prtp_info->mediaformat;	// set default
	pk->RTPHeader->un16Sequencenum = ++prtp_ses->start_seq;
	pk->RTPHeader->un32Timestamp = prtp_info->ts;
	pk->RTPHeader->un32SSIdentifier = prtp_ses->ssrc;

#if (BUILD_TEST==1)
	if (prtsp_ses->rtp_type == RTP_TCP || prtsp_ses->rtp_type == RTP_HTTP) {
		if (un32FUsize) {
			if (un32FtchNum > 1442)	printf("Error!(%d>1442)\r\n", un32FtchNum);
		} else {
			if (un32FtchNum > 1444)	printf("Error!(%d>1444)\r\n", un32FtchNum);
		}
	} else if (prtsp_ses->rtp_type == RTP_UDP) {
		if (un32FUsize) {
			if (un32FtchNum > 1446)	printf("Error!(%d>1446)\r\n", un32FtchNum);
		} else {
			if (un32FtchNum > 1448)	printf("Error!(%d>1448)\r\n", un32FtchNum);
		}
	}
#endif

	// data copy
#if 1	// udp only...? test code...
	if (un32FtchNum <= 6) {
		DmaMemSet_ip((BYTE *)pk->payload, 0, 18);
	}
#endif
	DmaMemCpy_ip((BYTE *)pk->payload, (BYTE *)prtp_info->data, un32FtchNum);
//	invalidate_dcache_range((uint)pk->payload, (uint)(pk->payload+un32FtchNum));

	// next data
	prtp_info->data += un32FtchNum;
	prtp_info->left -= un32FtchNum;

#if (BUILD_TEST==1)
	if (prtp_info->left == 0) {
		if (un32FUsize) {
			if (pk->FUHeader->un1Endbit != 1) {
				printf("FU-End flag fail!\r\n");
			}
		} 

		if (pk->RTPHeader->un1Markerbit != 1) {
			printf("RTP-End flag fail!\r\n");
		}
	}
	if (un32FUsize) {
		if (pk->FUHeader->un1Endbit == 1 && pk->FUHeader->un1Startbit == 1) {
			printf("FU-Start/End flag fail!\r\n");
		}
	}
#endif

	// Packet 생성 완료
	pk->un8PacketState = 0;
}

static void getH264Image(rthPacket* pk, rtsp_con_t* prtsp_con)
{
	rtsp_session_t* prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t* prtp_ses = &prtsp_ses->prtp_s[VIDEO];
	rtp_tx_info_t* prtp_info = &prtsp_con->prtp_info;
	u32 ck_size1, ck_size2;
	switch (prtp_info->btx_st) {
		case H264_TX_READY:
			if (prtp_info->sendFlag == 0) {
//				printf("%s(%d) : not ready?\r\n", __func__, __LINE__);
				prtp_info->sendDone = 1;
				prtp_info->nsize = 0;
				prtsp_con->end = 1;
				pk->un8PacketState = 5; // NO DATA
				break;
			} else {
				UINT base = 0, nTs = 0;

				prtp_info->sendFlag = 0;
				if(sbuf_get(&(prtsp_con->sbuf_video), &base, (UINT *)&prtp_info->nsize, (UINT *)&prtp_info->iframe, &nTs) != DEF_OK) {
					pk->un8PacketState = 6; // NO DATA...?
					break;
				}

#if ENX_NETWORK_Debug
				gptMsgDebug.RTSP_frame_vid_get[prtsp_con->valid-1]++;
#endif

				if (prtp_ses->start == 0) {
					if (prtp_info->iframe == 1) {
						prtp_ses->start = 1;
					} else {
#if (BUILD_TEST==1)
						printf("%s(%d) : not start!\r\n", __func__, __LINE__);
#endif
						prtp_info->sendDone = 1;
						prtp_info->nsize = 0;
						prtsp_con->end = 1;
						pk->un8PacketState = 4;
					    break;
					}
				}

				prtp_info->left = prtp_info->nsize;
				prtp_info->data = prtp_info->base = (u8 *)base;
				invalidate_dcache_range((u32)prtp_info->base, (u32)(prtp_info->base + 32));
				if (prtsp_con->h264_print_frameNum) {
					prtsp_con->h264_print_frameNum(&prtsp_con->sequence_num, prtp_info->base);
					if (prtsp_con->sequence_num == 0xFFFFFFFF) {	// sequence_num에 오차가 생긴다면 다음 I까지 pass
						printf("%s(%d) : slow network speed? small memory allocation?\r\n", __func__, __LINE__);
						printf("%s(%d) : or changed IDR(gop)?\r\n", __func__, __LINE__);
						prtp_ses->start = 0;
						prtp_info->sendDone = 1;
						prtp_info->nsize = 0;
						prtsp_con->end = 1;
						pk->un8PacketState = 4;
						break;
					}
				}

//				printf("0x%08X[%c] - %d\r\n", base, prtp_info->iframe == 1 ? 'I' : 'P', prtp_info->left);

				// ts update start
				prtp_ses->start_rtptime += nTs;
				prtp_info->ts = prtp_ses->start_rtptime;
//				flprintf("PLAY RTP  TIME %10d\r\n", prtp_ses->start_rtptime);

				// set tx mode
				prtp_info->btx_st = H264_TX_DOING;
				prtsp_con->end = 0;
			}

			// next H264_TX_READY -> H264_TX_DOING

		case H264_TX_DOING:

			ck_size1 = prtp_info->left;				// $CMT-hjlee-171214 - remaining size TEST
			buildPacket(pk, prtsp_con);
			ck_size2 = prtp_info->left;				// $CMT-hjlee-171214 - remaining size TEST

			// 데이터가 완료되면 
			if (prtp_info->left == 0) {		// 마지막 데이터까지 패킷으로 다 만들었으면 현 상태를 종료한다
				prtp_info->btx_st = H264_TX_READY;
				prtp_info->sendDone = 1;
				prtsp_con->end = 1;
			} else if (prtp_info->left > 10485760) {	// left가 역전?
				printf("left %d -> %d\r\n", ck_size1, ck_size2);	// $CMT-hjlee-171214 - remaining size TEST
				printf("%s(%d) : the remaining size is invalid(%dbyte/%dbyte)\r\n", __func__, __LINE__, prtp_info->left, prtp_info->nsize);
				prtp_info->btx_st = H264_TX_READY;
				prtp_info->sendDone = 1;
				prtsp_con->end = 1;
				int printmax = LWIP_MIN(prtp_info->nsize, 64);
				hexDump("H.264 dump(64byte)", prtp_info->base, printmax);			// $CMT-hjlee-171214 - remaining size TEST
//				hexDump("H.264 dump", prtp_info->base, prtp_info->nsize);	// $CMT-hjlee-171214 - remaining size TEST
//				while(1);						// $CMT-hjlee-171214 - remaining size TEST
			}

			break;
	}
}

char send_RTPoverUDP_Packet(void* ctx)
{
	rtsp_con_t* prtsp_con = (rtsp_con_t*)ctx;
	rtsp_session_t *prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[VIDEO];
	struct udp_pcb* pcb = prtsp_con->upcb[VIDEO];
	ip_addr_t dst_addr;
	struct pbuf *p;
	err_t ert = ERR_INPROGRESS;
	static UINT allocate_fail_time = 0;
	static UINT send_fail_time = 0;

	if (pcb == NULL) {
		return ert;
	}

	// UDP PCB Setting
	dst_addr.addr = prtsp_ses->remote_ipaddr;

	if (!(p = pbuf_alloc(PBUF_TRANSPORT, PacketSize, PBUF_POOL))) {
		if (allocate_fail_time != gptMsgShare.TIME) {
			allocate_fail_time = gptMsgShare.TIME;
			printf("%s RTP(H.264/UDP) could not allocate pbuf_pool\r\n", sltLocalTime.data);
		}
		return ert;
	}

	rthPacket sendPK;
	sendPK.un8pPacket = p->payload;
#if (BUILD_OLD_UDP==1)
	sendPK.RTPHeader = (rthRTPHeader *)(sendPK.un8pPacket);
	buildRTPHeaderInit(sendPK.RTPHeader);
#endif
	sendPK.un8PacketState = 1;

	// Packet build
#if (BUILD_OLD_UDP==1)
	old_getH264Image(&sendPK, prtsp_con);
#else
	getH264Image(&sendPK, prtsp_con);
#endif

	if (sendPK.un8PacketState == 0) {
		if (p->tot_len != sendPK.un32PacketLength) {
			pbuf_realloc(p, sendPK.un32PacketLength);
		}

		if (prtsp_ses->nif) {
			ert = udp_sendto_if(pcb, p, &dst_addr, prtsp_ses->rtp_port[VIDEO], prtsp_ses->nif);
		} else {
			ert = udp_sendto(pcb, p, &dst_addr, prtsp_ses->rtp_port[VIDEO]);
		}

		if (ert != ERR_OK) {
			if (send_fail_time != gptMsgShare.TIME) {
				send_fail_time = gptMsgShare.TIME;
				printf("%s RTP(H.264/UDP) send error(%d)\r\n", sltLocalTime.data, ert);
			}
		} else {
#if ENX_NETWORK_Debug
			gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
			gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += p->tot_len;
#endif
			prtp_ses->packet_cnt++;
			prtp_ses->total_length += p->tot_len;//sendPK.un32PacketLength;//p->tot_len;
//			flprintf("sum(%d) len(%d) plen(%d)\r\n", prtp_ses->total_length, p->tot_len, sendPK.un32PacketLength);
		}

		// WaitXus : UDP영상이 깨짐현상(끊김현상X)이 나타나면 사용
		// 짧은 시간대에 대량의 패킷(I frame)을 전송해서 문제 발생하므로 전송간격을 준다.
		// WaitXus(50);
	} else {
		if (prtp_ses->start == 1 && sendPK.un8PacketState != 5) {
			printf("%s(%d) : type(%d) err(%d)\r\n", __func__, __LINE__, prtsp_ses->video_type, sendPK.un8PacketState);
		}
	}
	pbuf_free(p);

	return ert;
}

char send_RTPoverTCP_Packet(void* ctx)
{
	rtsp_con_t* prtsp_con = ctx;
	rtsp_session_t *prtsp_ses = &prtsp_con->prtsp_ses;
	rtp_session_t *prtp_ses = &prtsp_ses->prtp_s[VIDEO];
	struct tcp_pcb* pcb = prtsp_con->tpcb;
	err_t ert = ERR_INPROGRESS;

	// TCP PCB Setting
	pcb->flags &= ~(TF_ACK_DELAY);
	tcp_nagle_disable(pcb);

	if (tcp_sndbuf(pcb) >= PacketSize && tcp_sndqueuelen(pcb) < TCP_SND_QUEUELEN) {
		rthPacket sendPK;
		sendPK.un8pPacket = &(prtsp_con->send_buf[prtsp_con->jbuf_seq][0]);
		sendPK.un8PacketState = 1;

		// Packet build
		getH264Image(&sendPK, prtsp_con);

		// 0:이상없음, 1:아무것도 안함(초기값), -1:실패
		if (sendPK.un8PacketState == 0) {
			ert = tcp_write(pcb, sendPK.un8pPacket, sendPK.un32PacketLength, 0);
			if (ert == ERR_OK) {
#if ENX_NETWORK_Debug
// 이걸 위 아래로 분리하거나 valid-1을 미리 연산해서 매개변수로 넣던가,
				gptMsgDebug.RTP_packet_cnt_tx[prtsp_con->valid-1]++;
				gptMsgDebug.RTP_packet_len_tx[prtsp_con->valid-1] += sendPK.un32PacketLength;
#endif
				prtp_ses->packet_cnt++;
				prtp_ses->total_length += sendPK.un32PacketLength;

				ert = tcp_output(pcb);
				if (ert != ERR_OK) {
					printf("%s RTP(H.264/TCP) tcp_output error(%d)\r\n", sltLocalTime.data, ert);
				}
			} else {
				printf("%s RTP(H.264/TCP) tcp_write error(%d)\r\n", sltLocalTime.data, ert);
			}

//			WaitXus(300);

			num_loop(prtsp_con->jbuf_seq, NUM_JPEG_BUF);
		} else {
			if (prtp_ses->start == 1 && sendPK.un8PacketState != 5) {
				printf("%s(%d) : type(%d) err(%d)\r\n", __func__, __LINE__, prtsp_ses->video_type, sendPK.un8PacketState);
			}
		}
	}

	return ert;
}
