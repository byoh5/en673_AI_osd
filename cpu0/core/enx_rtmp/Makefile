include $(CPU0DIR)/config.mk

LIBNAME = rtmp.o

#########################################################################

OBJS  = ./rtmp_session.o
OBJS += ./ngx_rtmp_alloc.o
OBJS += ./ngx_rtmp_amf.o
OBJS += ./ngx_rtmp_array.o
OBJS += ./ngx_rtmp_connect.o
OBJS += ./ngx_rtmp_core.o
OBJS += ./ngx_rtmp_handshake.o
OBJS += ./ngx_rtmp_send.o
OBJS += ./ngx_rtmp_stream.o
OBJS += ./ffmpeg_sha256.o

#########################################################################

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	@$(LD) -r -o $@ $(OBJS)

clean:
	@rm -rf $(LIBNAME) $(OBJS) .depend

#########################################################################

.depend: $(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

#########################################################################