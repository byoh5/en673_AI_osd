include $(CPU0DIR)/config.mk

LIBNAME = stream.o           

#########################################################################

OBJS  = ./rtp_h264.o
OBJS += ./rtp_jpeg.o
OBJS += ./rtp_audio.o
OBJS += ./rtp_over_rtsp.o
OBJS += ./rtsp_cmd.o
OBJS += ./rtcp_recv.o
OBJS += ./Base64.o

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