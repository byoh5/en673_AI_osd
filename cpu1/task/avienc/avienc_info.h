#ifndef __AVIENC_INFO__H__
#define __AVIENC_INFO__H__

#define SAVE_FILENAME_LENS		12	// 8+1+3

#define SAVE_NOR_DIR			MMC_DIR_NOR
#define SAVE_NOR_NAME_FORMAT	"NOR_%04d.avi"
#define SAVE_NOR_TIME_MAX		30

#define SAVE_EVT_DIR			MMC_DIR_EVT
#define SAVE_EVT_NAME_FORMAT	"EVT_%04d.avi"
#define SAVE_EVT_TIME_MAX		30

#define AUDIO_FPS				10
#define TXTS_FPS				15

void avienc_info_sbuf_init(t_avienc_t *enc_t);
void avienc_info_sbuf_reset(t_avienc_t *enc_t);
void avienc_info_init_savetime(t_avienc_t *enc_t, UINT savetime, UINT video_fps, UINT audio_fps, UINT txts_fps);
void avienc_info_init_filenamelist(t_avienc_t *enc_t, int maxfilecount);
void avienc_info_default(t_avienc_t *enc_t);
void avienc_info_init(t_avienc_t *enc_t, int type, char *dir_name, char *file_format);
void avienc_info_deinit(t_avienc_t *enc_t);
int avienc_info_put(t_avienc_t *enc_t, u32 addr, u32 size, u32 idr, u32 type);

#endif	// __AVIENC_INFO__H__
