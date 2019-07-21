#ifndef __H_H264_MCH__
#define __H_H264_MCH__

#ifdef __H264_MCH__
#define MAX_RAW_NUM		(4)//2,4,6
#define MAX_Q_CH_NUM	(4)//2,4,6

#define PERSENT_DECREASE (2)

#define H264_STATUS_ENCEND		0x01
#define H264_STATUS_ENCVLCEND	0x02
#define H264_STATUS_FLUSHEND	0x04
#define H264_STATUS_VLCEND		0x10
#define H264_STATUS_SBUFFFULL	0x40
#define H264_STATUS_STRMERR		0x4000

#define H264_ALIGN_SIZE   	4096//1024//
#define H264_ALIGN(x) 		(((x) + H264_ALIGN_SIZE - 1) & ~(H264_ALIGN_SIZE - 1))

typedef enum {
	MCH_0  =  0,
	MCH_1  =  1,
	MCH_2  =  2,
	MCH_3  =  3,
	MCH_4  =  4,
	MCH_5  =  5,
	MCH_6  =  6,
	MCH_7  =  7,
	MCH_8  =  8,
	MCH_9  =  9,
	MCH_10 = 10,
	MCH_11 = 11,
	MCH_12 = 12,
	MCH_13 = 13,
	MCH_14 = 14,
	MCH_15 = 15
} eMCH;

// 각 채널마다 vlc를 위한 정보를 담고 있는 구조체
typedef struct {
	int q_ridx; // encoding시 write되는 queue index
	int q_eidx; // encoding시 write되는 queue index
	int q_vidx; // vlc 시 read되는 queue index
	int q_start;
	int q_end;
	int q_validrawnum; //  
	int q_validnum; //  
	int q_size;
	
	int pic[MAX_Q_CH_NUM];
	//int time[MAX_CH_NUM];
	int slicetype[MAX_Q_CH_NUM];
	int qp[MAX_Q_CH_NUM];
} sH264MCHQueue;


typedef struct {
	unsigned int yaddr;
	unsigned int caddr;//uaddr;
	//unsigned int vaddr;
} sImgBuf;

// 각 채널마다 인코딩을 위한 정보
typedef struct {

	int profile_idc;
	int level_idc;	
	int entropy;
	UINT width;
	UINT height;
	int hscale;
	int vscale;
	int i_interval;
	int fps;
	int resolution;
	int default_qp;
	int cur_qp;
	int pps_qp;
	
	int num_mb;
	int crop_b;
	int srchmode;

    int cabac_idc;
	int slice_type[MAX_Q_CH_NUM];
	int vlock_ready;
	uint32 vlcdone_tick;

	uint32 pic;
	int period;

	int brmode;
	int brsize;

	unsigned int strmbase;
	unsigned int strmendaddr;

	unsigned int ysize;	
	unsigned int csize;

	unsigned int stpsize;
	unsigned int stpinfosize;
	unsigned int strmaddr;
	unsigned int strmsize;
	
	unsigned int strsize;
	
	unsigned int stpsaddr[MAX_Q_CH_NUM];
	unsigned int stpiaddr[MAX_Q_CH_NUM];
	
    sImgBuf cur[MAX_Q_CH_NUM];
	sImgBuf rec;
	sImgBuf ref;

	u32 ystride;
	u32 cstride;
	
	int frameCnt;
	int	flagFrame;
	
	int on_profile_idc;
    int on_cabac_idc;	
	int on_entropy;
	
	int main_dec;
	
} sH264CHParam; 

typedef struct {
	
	int num_ch;
	int enable;
	u32 isp_frame;
	u32 isp_wdr;
	u32 isp_dss;
	int max_fps;
	int video_tms;
	int hold;
	int isp_outmode;
	int sen_width;
	int sen_height;
	
	int enc_state;
	int vlc_state;
	
	int raw_buffer_flag;
	int avi_save;
	
	sH264CHParam  chi[2];
	sH264MCHQueue que[2];
	
#ifdef __H264_CHECK_TIME__
	u32 start_clk_h264;
	u32 stop_clk_h264;
	u32 start_clk_vlc;
	u32 stop_clk_vlc;
	u32 tmp_clk_vlc;
	u32 tmp_clk_h264;	
#endif

} sH264Param;

#define PROFILE_IDC		(100)
#define LEVEL_IDC		(41)//(51)

typedef enum{
	e_eCAVLC = 0,
	e_eCABAC = 1
}H264EntropyMode;

typedef enum{
	e_pBaseline = 66,
	e_pMain = 77
}H264Profile;

#ifdef VLC_CONTROL_Q
extern u8 VLCControlQ[2];
#endif


#define H264_Quantize(ch, quantize)	do { hmc.chi[ch].cur_qp = quantize; }while(0);
#define H264_Default_Quantize(ch, quantize)	do { hmc.chi[ch].default_qp = quantize; }while(0);

extern sH264Param hmc;

#ifdef __H264_TEST__
void h264_make_raw(UINT SIZE_W, UINT SIZE_H);
#endif


UINT h264_mch_enc_check(u32 mode, u32 wdr);
//void h264_mch_raw_update(int ch, int step);


void h264_mch_set_disable(void);
void h264_mch_set_enable(void);
int h264_mch_get_enable_state(void);

void h264_mch_set_channel_number(int channel_number); 
int h264_mch_get_channel_number(void); 

void h264_mch_set_enc_state(int enc_state);
int h264_mch_get_enc_state(void);

void h264_mch_set_vlc_state(int vlc_state); 
int h264_mch_get_vlc_state(void);

int h264_mch_get_hold_state(void);
void h264_mch_set_hold_state(int state);

int h264_mch_get_isp_outmode(void);
void h264_mch_set_isp_outmode(int mode);

void h264_mch_increase_hold_state(void);
void h264_mch_decrease_hold_state(void);

u32 h264_mch_get_isp_frame(void);
void h264_mch_set_isp_frame(u32 frame);

u32 h264_mch_get_isp_wdr(void);
void h264_mch_set_isp_wdr(u32 wdr);
	
int h264_mch_get_hscale(int ch);
int h264_mch_get_vscale(int ch);

int h264_mch_get_vlock_ready_state(int ch);
void h264_mch_set_vlock_ready_state(int ch, int ready);
void h264_mch_increase_vlock_ready_state(int ch);
void h264_mch_decrease_vlock_ready_state(int ch);

int h264_mch_get_raw_valid_num(int ch);
void h264_mch_increase_raw_valid_num(int ch);
void h264_mch_decrease_raw_valid_num(int ch);

int h264_get_period(int ch);

void h264_mch_increase_pic(int ch);
u32 h264_mch_get_interval_pic(int ch, int interval);
int h264_mch_get_pic(int ch);

UINT h264_mch_get_width(int ch);
UINT h264_get_height(int ch);

int h264_mch_check_queue_enc_ready(int ch);
int h264_mch_check_queue_vlc_ready(int ch);

int h264_mch_get_timestamp(int ch);
int h264_mch_get_default_timestamp(void);
void h264_mch_set_default_timestamp(int tms);

int h264_mch_check_raw_index(int ch);
int h264_mch_get_queue_validnum(int ch);
int h264_mch_get_queue_size(int ch);
void h264_mch_set_cur_yaddr(int ch, int idx,u32 addr);
void h264_mch_set_cur_caddr(int ch, int idx, u32 addr);
u32 h264_mch_get_cur_yaddr(int ch, int idx);
u32 h264_mch_get_cur_caddr(int ch, int idx);

void h264_mch_check_enc_starttime(void);
void h264_mch_check_vlc_starttime(void);
void h264_mch_check_enc_endtime(int ch);
void h264_mch_check_vlc_endtime(int ch);

int h264_mch_check_enc_end_status_res(int status);
int h264_mch_check_vlc_end_status_res(int status);
int h264_mch_get_slice_type_res(void);
int h264_mch_get_slice_type_enc(int ch);

int h264_mch_get_enc_channel_number_res(void);
int h264_mch_get_vlc_channel_number_res(void);
int h264_mch_get_status_res(void);
u32 h264_mch_get_prebits_res(void);
u32 h264_mch_get_stream_address_res(void);
u32 h264_mch_get_genbits_res(void);

void h264_mch_init_queue( int ch, int q_start, int q_end);
void h264_mch_init_vlc_validnum(int ch);
int h264_mch_set_queue( int ch, int slicetype);
int h264_mch_set_update_queue(int ch);
void h264_mch_init_enc_queue(int ch);
int h264_mch_get_queue(int ch, int *pic, int *slicetype, int *qp, unsigned int *stpsaddr, unsigned int *stpiaddr);
int h264_mch_get_update_queue( int ch);
void h264_mch_init_vlc_queue(int ch);

void h264_mch_init_param(int ch, int width, int height, int i_interval, int fps, int resolution, int qp, int srchmode, int profile, int brmode, int brsize, int maxfps, int timestamp);
void h264_mch_init_buffer(void);
void h264_mch_set_param(int ch);
void h264_mch_set_enc_buffer(int ch );
void h264_mch_set_vlc_buffer(int ch);
void h264_mch_shift_buffer(int ch);
void h264_mch_encode_param(int ch, int *type);
void h264_mch_encode_command(int ch);
void h264_mch_vlc_command(void);
int h264_mch_get_framecnt(int ch);
void h264_mch_set_framecnt(int ch, int cnt);
void h264_mch_default_isp_register(void);

void init_mch_isp_resolution(int ch);

void h264_mch_set_vlcdonetick(int ch, int val);
void h264_mch_increase_vlcdonetick(int ch);
uint32 h264_mch_get_vlcdonetick(int ch);
int h264_mch_get_raw_queue(int ch);
void h264_mch_update_raw_queue(int ch);
void h264_mch_update_stream_size(int ch);
void h264_mch_update_stream_size(int ch);
int h264_mch_get_vlc_quant_res(void);
void h264_mch_update_strbuffer(int ch, int strsize);
int h264_mch_get_flagframe(int ch);
void h264_mch_set_flagframe(int ch, int cnt);
int bitctl_get_previous_byte(int ch);
void h264_mch_cbr_processing(int ch, UINT idr);
int h264_mch_get_enc_queue(int ch);

void h264_mch_set_isp_dss(u32 dss);
u32 h264_mch_get_isp_dss(void);

extern void enx_h264_mch_raw_update(int ch);
extern UINT enx_h264_mch_enc_check(u32 mode, u32 wdr, u32 dss);
extern void enx_h264_mch_check_enc_result(int ch);
extern void enx_h264_mch_update_streambuffer(int ch);
extern void enx_h264_mch_check_enc_result(int ch);

extern void enx_h264_on(void);

extern void enx_h264_set_sdsave(u32 value);
extern int enx_h264_get_sdsave(void);

extern void enx_h264_set_quantize(int qp, int ch);
extern int enx_h264_get_quantize(int ch);
extern int enx_h264_get_default_quantize(int ch);

extern int enx_h264_get_maxframe(void);
extern void enx_h264_set_maxframe(int fps);

extern int enx_h264_get_idr_interval(int ch);

extern void enx_h264_set_profile(int profilemode, int ch);
extern void enx_h264_set_frame_rate_sf(SensorFps period, u32 ch);
extern void enx_h264_set_frame_size_rs(int resolution, int ch);

extern int enx_h264_get_bit_rate(int ch);
extern int enx_h264_get_bit_rate_mode(int ch);

extern void enx_h264_set_bit_rate(H264ConstantBitRate cbr, int ch);
extern void enx_h264_set_bit_rate_mode(int mode, int ch);
extern void enx_h264_set_idrframe_period(int interval, int ch);
extern void enx_h264_info_print(void);
extern int enx_h264_mch_get_profile(int ch);
extern int enx_h264_mch_get_profile_decrease(int ch);

extern void enx_h264_mch_set_profile_dec(int var, int ch);
extern void enx_h264_enc_poll(void);
extern int enx_h264_get_stp_size(int ch);
extern int enx_h264_get_slice_type(int ch);
extern void enx_h264_image_capture_ready(void);
extern u32 enx_h264_get_prebits(void);

#endif

#endif

