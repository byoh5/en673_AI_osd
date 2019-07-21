include $(CPU0DIR)/config.mk

LIBNAME = video.o

#########################################################################

OBJS  = ./enx_video.o
OBJS += ./bitratectl.o
OBJS += ./jpeg.o
OBJS += ./h264.o
OBJS += ./h264_core.o
OBJS += ./enx_mch_video.o
OBJS += ./h264_mch_core.o
OBJS += ./h264_mch.o

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