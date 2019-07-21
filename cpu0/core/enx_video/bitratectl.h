#ifndef __H_BITRATE_CTL__
#define __H_BITRATE_CTL__

enum QUPDOWN
{
	QSTAY = 0,
	QDOWN,
	QUP,	
};
enum AFUPDOWN
{
	AFDOWN = -1,
	AFSTAY = 0,
	AFUP = 1,	
};
enum BITUPDOWN
{
	BITDOWN = -1,
	BITSTAY = 0,
	BITUP = 1,	
};
enum ENCODEMEM
{
	NONE = 0,
	OVERWRITTEN,
	WARNING1,	
	WARNING2
};


#define H264_IFRAME_RATIO (60.0f)

#define H264_OVERBIT_RATIO (1)

#define H264_QSTEP_MAX	(8)

#define MAX_AF_LEVEL 	(20)

#define AF_BASE_BYTE  (5)

#define AF_DIFF_MIN_0 	(100000) 
#define AF_DIFF_MIN_1 	(200000) 
#define AF_DIFF_MIN_2 	(300000)
#define AF_DIFF_MIN_3 	(500000)
#define AF_DIFF_MIN_4 	(800000)
#define AF_DIFF_MIN_5 	(1000000)
#define AF_DIFF_MIN_6 	(5000000)
#define AF_DIFF_MIN_7 	(10000000)
#define AF_DIFF_MIN_8 	(50000000)
#define AF_DIFF_MIN_9 	(100000000)
#define AF_DIFF_MIN_10 	(500000000)


#define  BITRATE_MIN_DIFF0 	(1)
#define  BITRATE_MIN_DIFF1 	(5)
#define  BITRATE_MIN_DIFF2 	(10)
#define  BITRATE_MIN_DIFF3 	(50)
#define  BITRATE_MIN_DIFF4 	(100)
#define  BITRATE_MIN_DIFF5 	(500)
#define  BITRATE_MIN_DIFF6 	(1000)
#define  BITRATE_MIN_DIFF7 	(5000)
#define  BITRATE_MIN_DIFF8 	(10000)
#define  BITRATE_MIN_DIFF9 	(50000)
#define  BITRATE_MIN_DIFF10 (100000)

#define  BITRATE_MIN2_DIFF0 	(1)
#define  BITRATE_MIN2_DIFF1 	(50)
#define  BITRATE_MIN2_DIFF2 	(100)
#define  BITRATE_MIN2_DIFF3 	(500)
#define  BITRATE_MIN2_DIFF4 	(1000)
#define  BITRATE_MIN2_DIFF5 	(5000)
#define  BITRATE_MIN2_DIFF6 	(10000)
#define  BITRATE_MIN2_DIFF7 	(20000)
#define  BITRATE_MIN2_DIFF8 	(40000)
#define  BITRATE_MIN2_DIFF9 	(80000)
#define  BITRATE_MIN2_DIFF10 	(100000)

#define  BITRATE_IFRM_MIN_DIFF 	(5)



typedef struct {

  int nGop;		
  int nFps;

  int nCurIfrmQuant; // 결정된 현재 I 프레임의 Quantize
  int nPreIfrmQuant; // 이전 I 프레임의 Quantize
  
  int nCurPfrmQuant; // 결정된 현재 P 프레임의 Quantize
  int nPrePfrmQuant; // 이전 P 프레임의 Quantize

  int nPreFrmBytes; //이전 프레임의 결과 BYTE
  int nPreIFrmBytes; //이전 I프레임의 결과 BYTE
  int nPrePFrmBytes; //이전 P프레임의 결과 BYTE
  
  UINT nCurAfInfoIfrm; //현재  I 프레임 AF 정보
  UINT nCurAfInfoPfrm; //현재  P 프레임 AF 정보
  UINT nPreAfInfoIfrm; //이전 I 프레임 AF 정보
  UINT nPreAfInfoPfrm; //이전 P 프레임 AF 정보
  
  int nPreAfLevelIfrm; //이전 I 프레임 AF level 정보
  int nPreAfLevelPfrm;	//이전 P 프레임 AF level 정보
  
  int nPreAeInfo;	//이전 프레임 AE 정보
  int nCurAeInfo;	//이전 프레임 AE 정보
//  int nRemainToBytes; //GOP 단위의 남은 전체 바이트
  int nTotalBytes; //GOP 단위의 전체 바이트
  int nOverBytes;
  
  int nMinAeDiff;
  	
  UINT nEndIFrmCnt;	//인코딩이 끝난 프레임 수
  UINT nEndPFrmCnt;	//인코딩이 끝난 프레임 수
  
  int nIfrmTgByte;//I frame Target Bitrate	
  int nPfrmTgByte;//P frame Target Bitrate
  int nPfrmTgByte1;//P frame Target Bitrate  	
  int nPfrmTgByte2;//P frame Target Bitrate	
  int nPfrmTgByte3;//P frame Target Bitrate
  
  float32 cPfrmQuantByByte[52];
  float32 cPfrmDiffByByte[52];
  float32 cIfrmQuantByByte[52];
  float32 cIfrmDiffByByte[52];

  int nPfrmMinBitSz;
  int nIfrmMinBitSz;
  
  int nPfrmGapSz;
  int nIfrmGapSz;
  
  UINT FrameCount;
  int nIfrmRatio;//I frame Target Bitrate	
  int nPfrmRatio;//I frame Target Bitrate	

  int nOverBitRatio;
  int nIfrmWarning;//encoding size warning
  int nIfrmWarningSize;//encoding size warning
  int nPfrmWarning;//encoding size warning
  int nPfrmWarningSize;//encoding size warning
  
} BitCtl_t;

extern BitCtl_t bitctl[];

void bitctl_set_overbitsize(int ratio, int ch);

void bitctl_set_iframe_maxsize(int ratio, int ch);
void bitctl_set_pframe_maxsize(int ratio, int ch);

void bitctl_set_previous_iframe_quant(int ch, int iQuantize);

int bitctl_get_previous_iframe_quant(int ch);

void bitctl_set_previous_pframe_quant(int ch, int pQuantize);

int bitctl_get_previous_pframe_quant(int ch);

void bitctl_set_previous_iframe_byte(int ch, int iQuantize);

int bitctl_get_previous_iframe_byte(int ch);

void bitctl_set_previous_pframe_byte(int ch, int pQuantize);

int bitctl_get_previous_pframe_byte(int ch);

void bitctl_set_previous_af_iframe(int ch, UINT af);

UINT bitctl_get_previous_af_iframe(int ch);

void bitctl_set_previous_af_pframe(int ch, UINT af);

UINT bitctl_get_previous_af_pframe(int ch);

void bitctl_set_previous_ae(int ch, int ae);

int bitctl_get_previous_ae(int ch);

void bitctl_set_ae_min_diff(int ch, int min);

int bitctl_get_ae_min_diff(int ch);

void bitctl_set_iframe_quantize(int ch, int iQuantize);

int bitctl_get_iframe_quantize(int ch);

void bitctl_set_pframe_quantize(int ch, int pQuantize);

int bitctl_get_pframe_quantize(int ch);

void bitctl_set_iframe_target_bit(int ch, int byte);

int bitctl_get_iframe_target_bit(int ch);

void bitctl_set_pframe_target_bit(int ch, int byte);

int bitctl_get_pframe_target_bit(int ch);

void bitctl_set_min_bit_iframe(int ch, int bit);

int bitctl_get_min_bit_iframe(int ch);

void bitctl_set_min_bit_pframe(int ch, int bit);

int bitctl_get_min_bit_pframe(int ch);
int bitctl_get_ae_level (int ch);

UINT bitctl_get_af(void);

void bitctl_set_previous_af_level_iframe(int ch, int index);

void bitctl_set_previous_af_level_pframe(int ch, int index);

int bitctl_get_af_change_iframe(int ch);
int bitctl_get_af_change_pframe(int ch);
int bitctl_get_bit_change_pframe(int ch);

int bitctl_get_quantize_iframe_by_af(int ch);
int bitctl_get_quantize_pframe_by_af(int ch);

int bitctl_get_quantize_iframe_by_prebit(int ch);
int bitctl_get_previous_iframe_bitupdown(int ch);

int bitctl_get_up_quantize_iframe_by_af_prebit(int ch, int nAfInfoUpdown);

int bitctl_get_down_quantize_iframe_by_af_prebit(int ch, int nAfInfoUpdown);


void bitctl_init_seq(int ch);
void bitctl_init_gop(int ch);

void bitctl_init_frame_i_p(int ch, int slicetype);

void bitctl_update_frame_merge(int iframe, int ch, int Bytes);

int bitctl_get_pframe_aflevel(int ch, UINT nAfInfo);
int bitctl_get_warning(int ch, int iframe);
int bitctl_get_warning_size(int ch, int iframe);
void bitctl_check_warning(int ch);
int bitctl_get_previous_byte(int ch);

//#define BITCTL_DEBUG	//all function call
//#define BITCTL_DEBUG2	//init gop, target bit, previous bit
//#define BITCTL_DEBUG3 //I frame Q, P frame Q.
//#define BITCTL_DEBUG4 // Q by AE info, AF info.
#endif
