/* **************************************************************************
 File Name		: aud.c
 Description	: 
 Designer		: Jang, Young Kyu
 Date			: 2014. 6. 18
 Copyright ¨Ï Eyenix Co., Ltd. All Rights Reserved.
*************************************************************************** */

#include "dev.h"
#include "dev_inline.h"
#include "msg.h"

#ifdef __AUDIO__

#ifdef DEF_CPU0
#include "enx_audio.h"

#ifdef RTMP_SUPPORT
#include "rtmp_session.h"
#endif

#if (AUDIO_ADD_MUSIC==1)
#include "g711data.h"
UINT aud_pos = 0;
#endif
#include "G711_USER.h"
#include "G711_EVENT.h"

#define AUD_RX_SIZE		0x400	// 0x200(512byte), 0x400(1024byte)
#define AUD_G711_SIZE	0x200	// g711 data size for 66 ms

BYTE audzerobuffer[AUD_RX_SIZE] = {0};

extern void put_audio_streams(UINT addr, UINT size, UINT audio_type);

typedef struct {
	BYTE audbuffer[AUD_RX_SIZE];
	UINT aud_strm_base;
	UINT length;
	UINT over_length;
	UINT over_strm_base;
	UINT flag;
	UINT strm_base;
	UINT cnt;
} AudioPool; // sizeof => 1024 + 4*7 = 1052

ISRD AudioPool audpool = {
	.aud_strm_base = AUD_DATA_BASE,
	.length = 0,
	.over_length = 0,
	.over_strm_base = 0,
	.flag = 0,
	.strm_base = 0,
	.cnt = 0
};

void Aud_Isr(void *ctx)
{
	if(AUD_RXIRQ)
	{
		AUD_RXIRQ_CLR = 1;

		// PCM to mem(PCM)
		DmaMemCpy_isr((BYTE *)audpool.audbuffer, (BYTE *)AUD_BASE, AUD_RX_SIZE);
		// PCM to G.711
		G711Enc((BYTE *)audpool.aud_strm_base, (BYTE *)audpool.audbuffer, AUD_RX_SIZE, G711_uLaw);

		int length = audpool.length + AUD_G711_SIZE;
		if(length >= 800)
		{
			audpool.length = 800;
			audpool.over_length = length - 800;
			audpool.over_strm_base = audpool.strm_base + audpool.length;
			audpool.flag = 1;
		}
		else if(audpool.length == 0)
		{
			audpool.length += AUD_G711_SIZE;
			audpool.over_length = 0;
			audpool.strm_base = audpool.aud_strm_base;
		}
		else
		{
			audpool.length += AUD_G711_SIZE;
			audpool.over_length = 0;
		}
		audpool.cnt++;

		if(gptMsgShare.AUD_TX_MODE == e_AtxEchoRaw)
		{	// mem(PCM) to PCM, go I2S
			DmaMemCpy_isr((BYTE *)AUD_BASE, (BYTE *)audpool.audbuffer, AUD_RX_SIZE);
		}
		else if(gptMsgShare.AUD_TX_MODE == e_AtxEchoG711)
		{	// G.711 to PCM, go I2S
			G711Dec((BYTE *)AUD_BASE, (BYTE *)audpool.aud_strm_base, AUD_G711_SIZE, G711_uLaw);
		}

		if(audpool.flag == 1)
		{
			put_audio_streams(audpool.strm_base, audpool.length, 0);	// G.711
#ifdef RTMP_SUPPORT
			put_g7xx_streams_rtmp(audpool.strm_base, audpool.length);
#endif

			// SDsave(muxer_avienc)
#ifdef __DEVICE_SD__
			MsgRecPut(audpool.strm_base, audpool.length, audpool.length, eSTREAMMSG_AUDIO);
			CPU_SHARED1 |= eIP1_RECORD_INFO;
			CPU_IRQ1 = 1;
#endif

			// audpool reset
			audpool.strm_base = audpool.over_strm_base;
			audpool.length = audpool.over_length;
			audpool.flag = 0;
			audpool.over_length = 0;
			audpool.over_strm_base = 0;
		}

		// jump
#if (AUDIO_BUF_SIZE==(64*1024))
#define AUDPOOL_CNT					125
#elif (AUDIO_BUF_SIZE==(128*1024))
#define AUDPOOL_CNT					250
#elif (AUDIO_BUF_SIZE==(256*1024))
#define AUDPOOL_CNT					500
#elif (AUDIO_BUF_SIZE==(512*1024))
#define AUDPOOL_CNT					1000
#else
#error "AUDIO BUF SIZE ERROR!"
#endif
		if(audpool.cnt % AUDPOOL_CNT == 0)
		{	// init address, count
			audpool.aud_strm_base = AUD_DATA_BASE;
			audpool.cnt = 0;
		}
		else
		{	// polling
			audpool.aud_strm_base += AUD_G711_SIZE;
		}
	}

	if(AUD_TXIRQ)
	{
		AUD_TXIRQ_CLR = 1;
		gptMsgShare.AUD_TX_DONE = 1;
		int flag = 0;

		switch(gptMsgShare.AUD_TX_MODE)
		{
#if (AUDIO_ADD_MUSIC==1)
			case e_AtxMusic:
			{
				UINT send_size = (G711DATA_LEN - aud_pos) >= 0x200 ? 0x200 : (G711DATA_LEN - aud_pos);

				// G.711 to PCM, go I2S
				G711Dec((BYTE*)AUD_BASE, (BYTE*)&g711data[aud_pos], send_size, G711_uLaw);

				aud_pos += send_size;
				if(aud_pos >= G711DATA_LEN)
					aud_pos = 0;
				flag = 1;
				break;
			}
#endif
#if (AUDIO_RX)	// AUDIO_RX == (1 or 2)
			case e_AtxNetRXG711:
			{
				if(cQueue_isempty(&g711rx) == DEF_FAIL)
				{
					g711_data *auddata = &g711rx.sbuf[g711rx.head];
					DmaMemCpy_isr_async((BYTE *)AUD_BASE, (BYTE *)auddata->addr, auddata->size);
					num_loop(g711rx.head, g711rx.tot_num);
					flag = 1;
				}
				break;
			}
#endif
			case e_AtxUser:
			{
				UINT send_size = (G711_USER_LEN - aud_pos) >= 0x200 ? 0x200 : (G711_USER_LEN - aud_pos);

				// G.711 to PCM, go I2S
				G711Dec((BYTE*)AUD_BASE, (BYTE*)&G711_USER[aud_pos], send_size, G711_uLaw);

				aud_pos += send_size;
				if(aud_pos >= G711_USER_LEN)
				{
					gptMsgShare.AUD_TX_MODE = e_AtxSilent;
					aud_pos = 0;
				}
				flag = 1;
				break;
			}
			case e_AtxEvent:
			{
				UINT send_size = (G711_EVENT_LEN - aud_pos) >= 0x200 ? 0x200 : (G711_EVENT_LEN - aud_pos);
			
				// G.711 to PCM, go I2S
				G711Dec((BYTE*)AUD_BASE, (BYTE*)&G711_EVENT[aud_pos], send_size, G711_uLaw);
			
				aud_pos += send_size;
				if(aud_pos >= G711_EVENT_LEN)
				{
					gptMsgShare.AUD_TX_MODE = e_AtxSilent;
					aud_pos = 0;
				}
				flag = 1;
				break;
			}
			case e_AtxSilent:
				break;
			default:
				break;
		}

		if(flag == 0)
		{
			DmaMemCpy_isr_async((BYTE *)AUD_BASE, (BYTE *)audzerobuffer, AUD_RX_SIZE);
			aud_pos = 0;
		}
	}

	UNUSED(ctx);
}
#endif

#ifdef DEF_CPU1
void Aud_Isr(void *ctx)
{
	printf("%s(%d-%d)\r\n", __func__, AUD_RXIRQ, AUD_TXIRQ);

	if(AUD_RXIRQ)
	{
		AUD_RXIRQ_CLR = 1;
	}
	if(AUD_TXIRQ)
	{
		AUD_TXIRQ_CLR = 1;
	}
	UNUSED(ctx);
}

void Aud_RegShow(void)
{
	printf("AUD_CLK_DIV    : %d\r\n", AUD_CLK_DIV); 
	printf("AUD_CLK_SEL    : %d\r\n", AUD_CLK_SEL); 
	printf("AUD_ACK_PD     : %d\r\n", AUD_ACK_PD); 
	printf("AUD_TXIRQ_EN   : %d\r\n", AUD_TXIRQ_EN); 
	printf("AUD_TXIRQ      : %d\r\n", AUD_TXIRQ); 
	printf("AUD_RXIRQ_EN   : %d\r\n", AUD_RXIRQ_EN); 
	printf("AUD_RXIRQ      : %d\r\n", AUD_RXIRQ); 
	printf("AUD_WB_DEPTH   : %d\r\n", AUD_WB_DEPTH); 
	printf("AUD_DECI_DEPTH : %d\r\n", AUD_DECI_DEPTH); 
	printf("AUD_PWM_CH     : %d\r\n", AUD_PWM_CH); 
	printf("AUD_PWM_SIGN   : %d\r\n", AUD_PWM_SIGN); 
	printf("AUD_IO_MODE    : %d\r\n", AUD_IO_MODE); 
	printf("AUD_IO_CLKOE   : %d\r\n", AUD_IO_CLKOE); 
	printf("AUD_IO_DEPTH   : %d\r\n", AUD_IO_DEPTH); 
	printf("AUD_IRQ_TYPE   : %d\r\n", AUD_IRQ_TYPE); 
	printf("AUD_TX_EN      : %d\r\n", AUD_TX_EN); 
	printf("AUD_RX_EN      : %d\r\n", AUD_RX_EN); 
	printf("AUD_RX_CLR     : %d\r\n", AUD_RX_CLR); 
	printf("AUD_TX_CLR     : %d\r\n", AUD_TX_CLR); 
	printf("AUD_PWM_CLKDIV : %d\r\n", AUD_PWM_CLKDIV); 
	printf("AUD_WB_INCH    : %d\r\n", AUD_WB_INCH);
	printf("AUD_WB_OUTCH   : %d\r\n", AUD_WB_OUTCH);
}

void Aud_Init(BYTE CfgType)
{
	// for RESET buffer(RX, TX)
	AUD_RX_EN = 0;
	AUD_TX_EN = 0;
	AUD_RX_CLR = 1;
	AUD_TX_CLR = 1;

	// Audio TX buffer clean
	DmaMemSet_ip((BYTE *)AUD_BASE, 0, 2048);

	switch(CfgType)
	{
		case 1:	//	16bit Signed PCM 
#ifdef __AUD_ES8374__
			AUD_PWM_T  = 0x0000000A;	// RX:Right, TX:Right
#endif
#ifdef __AUD_TLV320AIC3100__
			AUD_PWM_T  = 0x00000005;	// RX:Left, TX:Left
#endif
			AUD_CONT_T = 0x006430CF;
		break;
	}
	printf("%s : AUD_CONT_T(0x%08X) AUD_PWM_T(0x%08X)\r\n", __func__, AUD_CONT_T, AUD_PWM_T);
}
#endif
#endif
