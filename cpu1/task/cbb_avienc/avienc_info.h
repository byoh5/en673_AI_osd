#ifndef __AVIENC_INFO__H__
#define __AVIENC_INFO__H__

#define SAVE_FILENAME_LENS		19//56

#define SAVE_NOR_DIR			MMC_DIR_NOR
#define SAVE_NOR_NAME_FORMAT	"N20%02d%02d%02d%02d%02d%02d"//-0000_00000N-00000_00000E-0_000-00_00"
#define SAVE_NOR_TIME_MAX		30
#define SAVE_NOR_VIDEO_FPS		30

#define SAVE_EVT_DIR			MMC_DIR_EVT
#define SAVE_EVT_NAME_FORMAT	"E20%02d%02d%02d%02d%02d%02d"//-0000_00000N-00000_00000E-0_000-00_00"
#define SAVE_EVT_TIME_MAX		12
#define SAVE_EVT_VIDEO_FPS		30

#define SAVE_MAN_DIR			MMC_DIR_MAN
#define SAVE_MAN_NAME_FORMAT	"M20%02d%02d%02d%02d%02d%02d"//-0000_00000N-00000_00000E-0_000-00_00"
#define SAVE_MAN_TIME_MAX		12
#define SAVE_MAN_VIDEO_FPS		30

#define SAVE_AVI_FORMAT	".avi"
#define SAVE_JPEG_THUMBNAIL_FORMAT	".jpg"

#define AUDIO_FPS				15			// 1s = 20aud data - 50ms - 400byte(cpu0\lib\audio\g711.h)
#define TXTS_GSS_FPS			15
#define TXTS_GPS_FPS			30
#define TXTS_TIME_FPS			1
#define TXTS_OBD_FPS			0

void avienc_info_sbuf_init(t_avienc_t *enc_t);
void avienc_info_sbuf_reset(t_avienc_t *enc_t);
void avienc_info_init_savetime(t_avienc_t *enc_t, UINT savetime, UINT video_fps, UINT audio_fps, UINT txt1_fps, UINT txt2_fps, UINT txt3_fps, UINT txt4_fps);
void avienc_info_init_filenamelist(t_avienc_t *enc_t, int maxfilecount);
#ifdef CONTIGUOUS_WRITE  
void avienc_info_default(t_avienc_t *enc_t, UINT offset, UINT start, UINT end, UINT len, UINT len2);
#else
void avienc_info_default(t_avienc_t *enc_t);
#endif
void avienc_info_init(t_avienc_t *enc_t, int type, char *dir_name, char *file_format);
void avienc_info_deinit(t_avienc_t *enc_t);
int avienc_info_putVideo(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);
int avienc_info_putAudio(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);
int avienc_info_putTxt1(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);
int avienc_info_putTxt2(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);
int avienc_info_putTxt3(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);
int avienc_info_putTxt4(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr);

#endif	// __AVIENC_INFO__H__
