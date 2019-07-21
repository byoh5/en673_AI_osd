//******************************************************************************
//	Copyright(c) Eyenix Co.,Ltd. 2003-
//
//	File Name:		exr1k_except.c
//	Description:
//	Author:			Young-Geun Kim (ygkim@{eyenix.com;rcv.kaist.ac.kr}) 
//
//	Note:
//
//	Ver Date		Author		Changes
//	--- ----		------		-------
//	0.1	150729		ygkim		first designed
// -----------------------------------------------------------------------------
#include "dev.h"	// to use printf
#include "dev_inline.h"	// to use fast code

//******************************************************************************
// Default Timer
//------------------------------------------------------------------------------
volatile uint 	gTimerTicks;
uint 			gMsPerTick;

void reset_timer_ticks(void)		{	gTimerTicks = 0;	}
uint get_timer_ticks(void)			{  	return gTimerTicks;	}
uint get_timer_ms_per_ticks(void)	{  	return gMsPerTick;	}

volatile uint gnIsrTickOn = 0;
volatile uint gnIsrTick = 0;
ISPM_DEV void default_timer_xsr(void)
{
  	gTimerTicks++;

	gnIsrTickOn++;
	gnIsrTick++;

#ifdef DEF_CPU0
#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))&&(USE_DIGITAL_INPUT==1)
	void BT_InputCheck(void);
	BT_InputCheck();
#endif

	static UINT gbTickCnt = TIME_TICK - 1; // (configTICK_RATE_HZ - 1) ~ 0
	if (gbTickCnt==0) {
		gbTickCnt = TIME_TICK - 1;
		gptMsgShare.TIME++;
		gptMsgShare.UPTIME++;
	} else {
		gbTickCnt--;
	}
#endif

	xsr_tick();

	clear_timer();
}

//******************************************************************************
// Simple Timer
//------------------------------------------------------------------------------
void init_timer(int ticks_per_sec)
{
	stop_timer();

	uint tick_period = EXR1K_CLK/ticks_per_sec;
	gTimerTicks = 0;
    gMsPerTick	= 1000/ticks_per_sec;	// [ms/sec] / [tick/sec] = [ms/tick]
	mtspr(EXR1K_TTMR, EXR1K_TTMR_IE | EXR1K_TTMR_M_RESTART | (tick_period & EXR1K_TTMR_PERIOD));		// Enable timer in TTMR

	TRACE("Initializing Tick Timer \r\n");
	TRACE(" ticks/sec = %d \r\n", ticks_per_sec);
	TRACE(" ms/tick   = %d \r\n", gMsPerTick);
	TRACE(" period [clock count/tick] = %d \r\n", tick_period);
	sys_enable_timer();
}

void stop_timer(void)
{
	sys_disable_timer();
	mtspr(EXR1K_TTMR, 0);														// Disable timer in TTMR
}

_NOINS void clear_timer(void)
{
	disable_htb();
	mtspr(EXR1K_TTMR, mfspr(EXR1K_TTMR)&~EXR1K_TTMR_IP); 						// Clear MR[IP]: to reset timer with same interval, that is, to restart timer
	//mtspr(EXR1K_TTMR, EXR1K_TTMR_IE | EXR1K_TTMR_M_RESTART | ((EXR1K_CLK/TICKS_PER_SEC) & EXR1K_TTMR_PERIOD));
	enable_htb();
}

//******************************************************************************
// Timeout Timer
// Ref: 	Linux Kernel and EN672 source
// Note:	Simplifed and modified to have coherency with other codes
//------------------------------------------------------------------------------
#if 1
#define TIMER_1MS			1000
#define TIMER_CYCLES_MS		(BUS_CLK/TIMER_1MS)
#define TIMEOUT_SIZE		16

typedef struct {
    u32 tick;
    u32 expire_at_tick;
    int expired;
    u32 type;
    void (*cb)(void* ctx);
    void* ctx;
}timeout_t;

typedef struct {
    volatile u32 tick;
    u32 tick_cnt;
    u32 MS_PER_TICK;
    void (*tick_isr)(void);
    timeout_t timeout[TIMEOUT_SIZE];
}exr1k_timer_t;

ISPD_DEV static exr1k_timer_t TIMER = {
    .tick = 0,
};

u32 get_tick(void)
{
    return TIMER.tick;
}

// CPU0 : call default_timer_xsr()
// CPU1 : call XsrTick()
ISPM_DEV void xsr_tick(void)
{
#ifdef DEF_CPU0
	gptMsgDebug.CPU0_TICK++;
#endif
#ifdef DEF_CPU1
	gptMsgDebug.CPU1_TICK++;
#endif
	TIMER.tick++;
//	if(TIMER.tick_isr)
//		TIMER.tick_isr();
}

void timer_tick_init(u32 cpu_clk, u32 tick_cnt)
{
    u32 period = cpu_clk / tick_cnt;
    TIMER.tick_cnt = tick_cnt;
    TIMER.MS_PER_TICK = TIMER_1MS / tick_cnt;
    mtspr(EXR1K_TTMR, EXR1K_TTMR_M_RESTART | EXR1K_TTMR_IE);	// Restart mode, Interrupt enable
    mtspr(EXR1K_TTMR, mfspr(EXR1K_TTMR) | (EXR1K_TTMR_PERIOD & period));	// Set period
#ifdef DEF_CPU0
    mtspr(EXR1K_SR, mfspr(EXR1K_SR) | EXR1K_SR_TEE);
#endif
}

void timer_init(void (*tick_isr)(void))
{
	u32 id;
	TIMER.tick_isr = tick_isr;
	for(id=0; id<TIMEOUT_SIZE; id++)
		TIMER.timeout[id].expired = TRUE;
}

u32 timer_get_ms(void)
{
	return TIMER.tick * TIMER.MS_PER_TICK + mfspr(EXR1K_TTCR) / TIMER_CYCLES_MS;
}

void timer_delay(u32 ms)
{
	u32 expire_at_tick = TIMER.tick + ms / TIMER.MS_PER_TICK;
	while(TIMER.tick < expire_at_tick);
}

ISPM_DEV void timer_poll(void)
{
	u32 id;
	for(id=0; id<TIMEOUT_SIZE; id++)
	{
		timeout_t* tmo = &TIMER.timeout[id];

		if (tmo->expired)						continue;
		if (tmo->expire_at_tick > TIMER.tick)	continue;

		if (tmo->cb)							tmo->cb(tmo->ctx);

		if (tmo->type == TIMEOUT_PERIODIC)		tmo->expire_at_tick = TIMER.tick + tmo->tick;
		else									tmo->expired = TRUE;
	}
}

static u32 timer_sched_timeout(u32 ms, u32 type)
{
	u32 id;
	for(id=0; id<TIMEOUT_SIZE; id++)
	{
		timeout_t* tmo = &TIMER.timeout[id];
		if (tmo->expired)
		{
			tmo->tick = ms / TIMER.MS_PER_TICK;
			tmo->expire_at_tick = TIMER.tick + tmo->tick;
			tmo->type = type;
			tmo->expired = FALSE;
			return id;
		}
	}
	return INVALID_TIMER_ID;
}

u32 timer_sched_timeout_cb(u32 ms, u32 type, void (*cb)(void *ctx), void* ctx)
{
	u32 id = timer_sched_timeout(ms, type);
	if(id != INVALID_TIMER_ID)
	{
		timeout_t* tmo = &TIMER.timeout[id];
		tmo->cb = cb;
		tmo->ctx = ctx;
	}
	return id;
}

void timer_cancel_timeout(u32 id)
{
	if(id < TIMEOUT_SIZE)
		TIMER.timeout[id].expired = TRUE;
	else
		printf("out of id\r\n");
}

int gettimeofday(struct timeval *tp, void * dummy)
{
 	u32 ms = timer_get_ms();//TIMER.tick * TIMER.MS_PER_TICK;
	tp->tv_sec = ms / TIMER_1MS;
	tp->tv_usec = (ms % TIMER_1MS) * TIMER_1MS;
	return 0;
	UNUSED(dummy);
}

u32 timeoffset(u32 offset)
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	return (t1.tv_sec + offset);
}
#else
#define	TMO_SIZE	16

typedef struct _tTimeout {
	uint 	ticks;
	uint 	expire_at_ticks;
	int 	expired;
	uchar 	type;
	void 	(*cb)(void* ctx);
	void* 	ctx;
} tTimeout;

tTimeout gTimeout[TMO_SIZE];

void ISPM_DEV tmo_timer_xsr(void)
{
    uint id;

  	gTimerTicks++;

    for(id=0; id<TMO_SIZE; id++)
    {
        tTimeout* tmo = &(gTimeout[id]);

        if(tmo->expired)	continue;
        if(tmo->expire_at_ticks>gTimerTicks) continue;

        if(tmo->cb)				       	tmo->cb(tmo->ctx);

        if(tmo->type == TT_PERIODIC)	tmo->expire_at_ticks = gTimerTicks + tmo->ticks;
        else				            tmo->expired = TRUE;
    }

	clear_timer();
}

void init_tmo_timer(uint ticks_per_sec)
{
    uint id;

    for(id=0; id<TMO_SIZE; id++)
		gTimeout[id].expired = TRUE;

	init_timer(ticks_per_sec);
	add_xsr(XSR_TICK, tmo_timer_xsr);

	CRITICAL_BEGIN;
	TRACE("Initializing Timout Timer \r\n");
	CRITICAL_END;
}

int add_tmo_timer_handler(uint ms, uchar type, void (*cb)(void *ctx), void* ctx)
{
    tTimeout* tmo;
    uint id;

    ASSERT(cb);
    ASSERT(type==TT_ONESHOT || type==TT_PERIODIC);

	//find an empty timeout slot
    for(id=0; id<TMO_SIZE; id++)
    {
		tmo = &(gTimeout[id]);
        if(tmo->expired) break;
    }
    ASSERT(id != TMO_SIZE);

	// set the timeout slot
    tmo->ticks = ms / gMsPerTick;						// # of gTimerTicks
    tmo->expire_at_ticks = gTimerTicks + tmo->ticks;
    tmo->expired = FALSE;
    tmo->type = type;
    tmo->cb = cb;
    tmo->ctx = ctx;

	CRITICAL_BEGIN;
	TRACE("Adding a Timeout Timer handler %d \r\n", id);
	TRACE(" ticks = %d \r\n", tmo->ticks);
	TRACE(" expire_at_ticks  = %d \r\n", tmo->ticks);
	TRACE(" type = %s \r\n", tmo->type == TT_PERIODIC ? "periodic" : "oneshot" );
	TRACE(" cb   = %08x \r\n", tmo->cb);
	TRACE(" ctx  = %08x \r\n", tmo->ctx);
	CRITICAL_END;

    return id;
}
#endif

//******************************************************************************
// BT Video Input Check Timer
//------------------------------------------------------------------------------
#ifdef DEF_CPU0
#if ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))

#include "dev_regs_isp.h"
#include "dev_en332s_ini_table_rx.h"
#include "dev_en332s_ini_table_vd.h"

UINT gnConnection_State=0;
BYTE gbConnection_Freq=30; // 0 : 50Hz, 1 : 60Hz
extern BYTE gbIsrIrqState, gbDInit_Finish;

void BT_InputCheck(void)
{
	UINT i=0,nRX_Data=0,nRX_Data2=0,nVtotalBuf=0,nHtotalBuf=0,buf=0;
	static UINT nConnection_State[2]={0xFF,0}, nConnection_IrqEdge=0, nRX_Data2_Buf=0;
	static BYTE bSwitchCheckCnt=0, bInit_RFlag=1;
	
	if(!gbDInit_Finish) return; // Wait for Device driver Initialization

	if(bInit_RFlag) {
	
#if (SDI_UCC_MODE==1) // for SDI UCC & Repeat Mode

		EN331_Write(0xde, 0x0000fff9); // repeater Only & Pin Direction(UCC Output) Setting
			EN331_Read(0xde, &buf);
			printf("\r\n UCC Modulation : 0x%08x",buf);

		//UCC Key Control
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,0,buf); // ENC_EN		
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,1,buf); // Ch A UCC Frequency/Duty Ratio		
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,3,buf); // Ch A UCC ON 	
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,7,buf); // Ch A Inversion
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,9,buf); // Ch A Internal UART
		EN332T_HANDLING_Ch(1,EN332T_UCC_TYPE,11,buf); // Ch A Uart Size/Baud rate

		EN331_Read(0x81, &buf);
		printf("\r\n UCC Control Register : 0x%08x",buf);
#endif
	
	// EN332T Test Pattern Enable (Default)
	#if(IMAGE_INPUT_MODE==2)
		for (i=0; i<RX_EX135_HD_Size; i++) { EN332T_HANDLING(RX_EX135_HD, i, buf) }
		for (i=0; i<RX_EX135_HD_VD8_Size; i++) { EN332T_HANDLING(RX_EX135_HD_VD8, i, buf) }
		
		for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING(RX_HDSDI, i, buf) }
		for (i=0; i<RX_HDSDI_VD8_Size; i++) { EN332T_HANDLING(RX_HDSDI_VD8, i, buf) }
		for (i=0; i<TP_VD_Size; i++) { EN332T_HANDLING(TP_VD, i, buf) } 

	#elif(IMAGE_INPUT_MODE==3)
		for (i=0; i<RX_EX135_HD_Size; i++) { EN332T_HANDLING_AOnly(RX_EX135_HD, i, buf)	}
		for (i=0; i<RX_EX135_HD_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_EX135_HD_VD16, i, buf) }
		
		for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI, i, buf)	}
		for (i=0; i<RX_HDSDI_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI_VD16, i, buf) }
		for (i=0; i<TP_VD_Size; i++) { EN332T_HANDLING_AOnly(TP_VD, i, buf) }
	
	#endif
		printf("\r\n EN332T Test Pattern Enable..(Default setting)");
	
		bInit_RFlag=0;
	}
	
	// Video Input Check from IRQ
	if(gbIsrIrqState) { gbIsrIrqState=0; nConnection_IrqEdge=(nConnection_IrqEdge<<1)|0x00000001; } // Isr IRQ State Check
	else              {                  nConnection_IrqEdge=(nConnection_IrqEdge<<1)&0xFFFFFFFE; }
	
	if(nConnection_IrqEdge>0x0FFFFFFF) { // Detected video sync more than 7frames

		if(nConnection_State[0]!=nConnection_State[1]) {
	#if(IMAGE_INPUT_MODE==2)
		printf("\r\n << BT656 Video Input Detected...  [0x%08x/0x%08x/0x%08x]", nConnection_IrqEdge, nConnection_State[0],nConnection_State[1]);
	#elif(IMAGE_INPUT_MODE==3)
		printf("\r\n << BT1120 Video Input Detected... [0x%08x/0x%08x/0x%08x]", nConnection_IrqEdge, nConnection_State[0],nConnection_State[1]);
	#endif
		}
	}	
	else if(nConnection_IrqEdge==0x00000000) { // No video sync

		if(nConnection_State[0]!=nConnection_State[1]) printf("\r\n << Video Input Disconnected... [0x%08x/0x%08x/0x%08x]", nConnection_IrqEdge, nConnection_State[0],nConnection_State[1]);
	}

	if((bSwitchCheckCnt==100)||(nConnection_State[0]!=nConnection_State[1])) { // 100 Counts or Changed connection status
//		printf(".");
		bSwitchCheckCnt=0;
	
		EN331_Read(0x302, &nRX_Data);
		EN331_Read(0x203, &nRX_Data2);

		if(((((nRX_Data>>16)&0x01)==0x00)&&(((nRX_Data>>24)&0xFF)!=0x00))||(nRX_Data2_Buf!=nRX_Data2)) { // Input Detected!! by EN3XX register

			// SDI Mode & Frequency Check
/*HD-SDI */	if	   ((((nRX_Data>>20)&0x03)==0x00)&&(((nRX_Data>>24)&0xFF)==0x80)) { nConnection_State[0]=(((nRX_Data2>> 0)&0x1FFF)==0x897) ? 1 : 2; }
/*3G SDI */	else if((((nRX_Data>>20)&0x03)==0x02)&&(((nRX_Data>>24)&0xFF)==0x40)) { nConnection_State[0]=(((nRX_Data2>> 0)&0x1FFF)==0x897) ? 3 : 4; }
/*EX270HD*/	else if((((nRX_Data>>20)&0x03)==0x01)&&(((nRX_Data>>24)&0xFF)==0x20)) { EN331_Read(0x325, &nRX_Data2); nConnection_State[0]=(((nRX_Data2>>16)&0x1FFF)==0x897) ? 5 : 6; }
/*EX135HD*/	else if((((nRX_Data>>20)&0x03)==0x03)&&(((nRX_Data>>24)&0xFF)==0x02)) { EN331_Read(0x325, &nRX_Data2); nConnection_State[0]=(((nRX_Data2>>16)&0x1FFF)==0x897) ? 7 : 8; }
/*EX2703G*/	else if((((nRX_Data>>20)&0x03)==0x01)&&(((nRX_Data>>24)&0xFF)==0x02)) { EN331_Read(0x325, &nRX_Data2); nConnection_State[0]=(((nRX_Data2>>16)&0x1FFF)==0xCE4) ? 9 :10; }
			#if 0 // Need more Test
/*EX1G54K*/	else if((((nRX_Data>>20)&0x03)==0x00)&&(((nRX_Data>>24)&0xFF)==0x90)) nConnection_State[0]=6; // 11,12
/*EX1G5TMD*/else if((((nRX_Data>>20)&0x03)==0x00)&&(((nRX_Data>>24)&0xFF)==0x04)) nConnection_State[0]=7; // 13,14
			#endif
			else																  nConnection_State[0]=15; // Unknown Format

			if(nConnection_State[0]!=nConnection_State[1]) { // Changed connection status
				
				printf("\r\n EN332T Video Input Detected [0x%08x/0x%08x]", nRX_Data, nRX_Data2);

				#if(IMAGE_INPUT_MODE==2)

				if((nConnection_State[0]==1)||(nConnection_State[0]==2)) {
					printf("\r\n HD-SDI        >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					
					for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING(RX_HDSDI, i, buf)	}
					for (i=0; i<RX_HDSDI_VD8_Size; i++) { EN332T_HANDLING(RX_HDSDI_VD8, i, buf) }
				}
				else if((nConnection_State[0]==3)||(nConnection_State[0]==4)) {
					printf("\r\n 3G-SDI        >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);			
										
					for (i=0; i<RX_3GSDI_Size; i++) { EN332T_HANDLING(RX_3GSDI, i, buf) }
					for (i=0; i<RX_3GSDI_VD8_Size; i++) { EN332T_HANDLING(RX_3GSDI_VD8, i, buf) }
				}
				else if((nConnection_State[0]==5)||(nConnection_State[0]==6)) {
					printf("\r\n EX-SDI Ver1.0 >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
																
					for (i=0; i<RX_EX270_HD_Size; i++) { EN332T_HANDLING(RX_EX270_HD, i, buf) }
					for (i=0; i<RX_EX270_HD_VD8_Size; i++) { EN332T_HANDLING(RX_EX270_HD_VD8, i, buf) }		
				}
				else if((nConnection_State[0]==7)||(nConnection_State[0]==8)) {
					printf("\r\n EX-SDI Ver2.0 >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);			
																					
					for (i=0; i<RX_EX135_HD_Size; i++) { EN332T_HANDLING(RX_EX135_HD, i, buf) }
					for (i=0; i<RX_EX135_HD_VD8_Size; i++) { EN332T_HANDLING(RX_EX135_HD_VD8, i, buf) }
				}
				else if((nConnection_State[0]==9)||(nConnection_State[0]==10)) {
					printf("\r\n EX-SDI 3G/4M  >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					printf("\r\n %d", (nRX_Data2>>16)&0x1FFF);
																					
					for (i=0; i<RX_EX270_3G_4M_Size; i++) { EN332T_HANDLING(RX_EX270_3G_4M, i, buf) }
					for (i=0; i<RX_EX270_3G_4M_VD8_Size; i++) { EN332T_HANDLING(RX_EX270_3G_4M_VD8, i, buf) }
				}
				else {
					printf("\r\n Unknown Format >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					
						for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING(RX_HDSDI, i, buf) }
						for (i=0; i<RX_HDSDI_VD8_Size; i++) { EN332T_HANDLING(RX_HDSDI_VD8, i, buf) }
						for (i=0; i<TP_VD_Size; i++) { EN332T_HANDLING(TP_VD, i, buf) } 
				}
					#elif(IMAGE_INPUT_MODE==3)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				if((nConnection_State[0]==1)||(nConnection_State[0]==2)) {
					printf("\r\n HD-SDI        >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI, i, buf)	}
					for (i=0; i<RX_HDSDI_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI_VD16, i, buf) }
				}
				else if((nConnection_State[0]==3)||(nConnection_State[0]==4)) {
					printf("\r\n 3G-SDI        >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					for (i=0; i<RX_3GSDI_Size; i++) { EN332T_HANDLING_AOnly(RX_3GSDI, i, buf)	}
					for (i=0; i<RX_3GSDI_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_3GSDI_VD16, i, buf) }
				}
				else if((nConnection_State[0]==5)||(nConnection_State[0]==6)) {
					printf("\r\n EX-SDI Ver1.0 >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					for (i=0; i<RX_EX270_HD_Size; i++) { EN332T_HANDLING_AOnly(RX_EX270_HD, i, buf)	}
					for (i=0; i<RX_EX270_HD_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_EX270_HD_VD16, i, buf) }
				}
				else if((nConnection_State[0]==7)||(nConnection_State[0]==8)) {
					printf("\r\n EX-SDI Ver2.0 >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					for (i=0; i<RX_EX135_HD_Size; i++) { EN332T_HANDLING_AOnly(RX_EX135_HD, i, buf)	}
					for (i=0; i<RX_EX135_HD_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_EX135_HD_VD16, i, buf) }
				}
				else if((nConnection_State[0]==9)||(nConnection_State[0]==10)) {
					printf("\r\n EX-SDI 3G/4M  >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					printf("\r\n %d", (nRX_Data2>>16)&0x1FFF);
					
					for (i=0; i<RX_EX270_3G_4M_Size; i++) { EN332T_HANDLING_AOnly(RX_EX270_3G_4M, i, buf)	}
					for (i=0; i<RX_EX270_3G_4M_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_EX270_3G_4M_VD16, i, buf) }
				}
				else {
					printf("\r\n Unknown Format >> V 0x%04x / H 0x%04x", nVtotalBuf, nHtotalBuf);
					
						for (i=0; i<RX_HDSDI_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI, i, buf)	}
						for (i=0; i<RX_HDSDI_VD16_Size; i++) { EN332T_HANDLING_AOnly(RX_HDSDI_VD16, i, buf) }
						for (i=0; i<TP_VD_Size; i++) { EN332T_HANDLING_AOnly(TP_VD, i, buf) }
				}
					#endif
					
				UINT nCheck_FYLine = (*((volatile UINT *)(0x9C00000)));
				UINT nCheck_FCLine = (*((volatile UINT *)(0xAC00000)));
				
//				gbConnection_Freq = (nConnection_State[0]&0x01) ? 60 : 50; // 0 : 50Hz, 1 : 60Hz
				gbConnection_Freq = (nConnection_State[0]&0x01) ? 30 : 25; // 0 : 50Hz, 1 : 60Hz
				
				if((((nCheck_FYLine>>24)&0xFF)==0x80)&&(((nCheck_FCLine>>24)&0xFF)==0x80)) { // If Continuously 2Frame Y Data is 0x80!
					
					if(gbConnection_Freq==30) { ISP_HLOCKI_POS0 = 0x114; printf("\r\n    H-Adjustment.. 0x%08x/0x%08x [60Hz]", nCheck_FYLine, nCheck_FCLine); }
					else                      { ISP_HLOCKI_POS0 = 0x2CC; printf("\r\n    H-Adjustment.. 0x%08x/0x%08x [50Hz]", nCheck_FYLine, nCheck_FCLine); }
				}
				else {
					if(gbConnection_Freq==30) { ISP_HLOCKI_POS0 = 0x113; printf("\r\n None Adjustment.. 0x%08x/0x%08x [60Hz]", nCheck_FYLine, nCheck_FCLine); }
					else                      { ISP_HLOCKI_POS0 = 0x2CB; printf("\r\n None Adjustment.. 0x%08x/0x%08x [50Hz]", nCheck_FYLine, nCheck_FCLine); }
				}
			}
			}
		else { // None Video Input...
		}
	}
	bSwitchCheckCnt++;
	gnConnection_State=nConnection_State[1]=nConnection_State[0];
}
#endif // End of ((IMAGE_INPUT_MODE==2)||(IMAGE_INPUT_MODE==3))
#endif // End of ifdef DEF_CPU0

//******************************************************************************
// Benchmark Timer
//------------------------------------------------------------------------------
#if defined(_DEBUG) && defined(_BENCHMARK)
volatile uint gBenchTimerStartTicks = 0;
volatile uint gBenchTimerStopTicks = 0;
volatile uint gBenchTimerOverheadTicks = 0;

uint bench_tic(void)
{
	gBenchTimerStartTicks = gTimerTicks;
	return gTimerTicks;
}

uint bench_toc(void)
{
	gBenchTimerStopTicks = gTimerTicks;
	return gTimerTicks;
}

uint bench_time_ms(void)
{
		return gMsPerTick * (gBenchTimerStopTicks - gBenchTimerStartTicks - gBenchTimerOverheadTicks);
}

void bench_timer_init(uint ticks_per_sec)
{
	init_timer(ticks_per_sec);

	bench_tic();
	bench_tic();
	bench_toc();
	gBenchTimerOverheadTicks = gBenchTimerStopTicks - gBenchTimerStartTicks;

	TRACE("Initializing Benchmark Timer \r\n");
	TRACE(" timer overhead  = %d \r\n", gBenchTimerOverheadTicks);
}
#endif

//******************************************************************************
// HW Benchmark Timer @ exr1k_spr.c: near HTB source
//------------------------------------------------------------------------------
#if 1// defined(_DEBUG) && defined(_HWBENCHMARK)
volatile uint gHwBenchTimerStartTicks = 0;
volatile uint gHwBenchTimerStopTicks = 0;
volatile uint gHwBenchTimerOverheadTicks = 0;	// reserved

uint hw_bench_tic(void)
{
	gHwBenchTimerStartTicks = mfspr(EXR1K_HTBTR);
	return gHwBenchTimerStartTicks;
}

uint hw_bench_toc(void)
{
	gHwBenchTimerStopTicks = mfspr(EXR1K_HTBTR);
	return gHwBenchTimerStopTicks;
}

uint hw_bench_time_clk(void)
{
	return gHwBenchTimerStopTicks - gHwBenchTimerStartTicks - gHwBenchTimerOverheadTicks;
}

void hw_bench_timer_init(void)
{
	enable_htb();		// HTBCR[EN|NE|TE] = 1; actually requires to enable TE only

	hw_bench_tic();
	hw_bench_tic();
	hw_bench_toc();
	gHwBenchTimerOverheadTicks = gHwBenchTimerStopTicks - gHwBenchTimerStartTicks;

	TRACE("Initializing HW Benchmark Timer \r\n");
	TRACE(" timer overhead  = %d \r\n", gHwBenchTimerOverheadTicks);
}
#endif

// TIME
int nTimeZoneSec[][3] = {	//
	{-12, 0, -43200},
	{-11, 0, -39600},
	{-10, 0, -36000},
	{-9, 30, -34200},
	{-9, 0, -32400},
	{-8, 0, -28800},
	{-7, 0, -25200},
	{-6, 0, -21600},
	{-5, 0, -18000},
	{-4, 0, -14400},
	{-3, 30, -12600},
	{-3, 0, -10800},
	{-2, 0, -7200},
	{-1, 0, -3600},
	{0, 0, 0},
	{1, 0, 3600},
	{2, 0, 7200},
	{3, 0, 10800},
	{3, 30, 12600},
	{4, 0, 14400},
	{4, 30, 16200},
	{5, 0, 18000},
	{5, 30, 19800},
	{5, 45, 20700},
	{6, 0, 21600},
	{6, 30, 23400},
	{7, 0, 25200},
	{8, 0, 28800},
	{8, 45, 31500},
	{9, 0, 32400},
	{9, 30, 34200},
	{10, 0, 36000},
	{10, 30, 37800},
	{11, 0, 39600},
	{12, 0, 43200},
	{12, 45, 45900},
	{13, 0, 46800},
	{14, 0, 50400}
};

const char* const strWeek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char* const strMonth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

void printtime(void)
{
	u32 total_ms = timer_get_ms();
	u32 ms = total_ms % 1000;
	u32 sec = total_ms / 1000;
	printf("[%08d.%03d] ", sec, ms);
}
