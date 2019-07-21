include $(CPU0DIR)/config.mk

TARGET = core

#########################################################################

# order is important here:
SUBDIRS	= net main enx_rtmp enx_video enx_stream enx_audio #enx_rtmp

OBJ  = main/cpu_main.o
OBJ += enx_audio/audio.o
OBJ += enx_video/video.o
OBJ += enx_stream/stream.o
OBJ += enx_rtmp/rtmp.o
OBJ += $(LWIP)/enx_lwip.o 
OBJ += $(ENX_NET)/enx_port.o

#########################################################################

ifeq ($(CONFIG_WIFI_SUPPORT),y)
all: tcp $(TARGET) #dis clean ewl
else
all: tcp $(TARGET) #dis clean
endif

ifeq ($(CONFIG_WIFI_SUPPORT),y)
LINK_LIB += -L$(COREDIR)/enx_lib/ -l$(EWL_LIB)
endif

$(TARGET): subdirs
	@echo "Core Link..."
	@$(LD) -r -o $(TARGET).o $(OBJ) $(LINK_LIB)

tcp:
	@$(MAKE) -C $(LWIP)

ewl:
	@$(MAKE) -C $(WLDIR)

subdirs:
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir || exit 1 ; done

clean:
	@for dir in $(SUBDIRS) $(LWIP); do make clean -C $$dir || exit 1 ; done
	@rm -rf $(OBJS) $(ASMS) $(LIB) *.o *.dis *.or32 *.bin
	@find . $(DEVDIR) $(LIBDIR) -name "*.nm" -delete
	@find . -name "*.o" -delete
	@find . -name "*.bak" -delete

distclean: clean
	find . -type f \
		\( -name .depend -o -name '*.srec' -o -name '*.bin' \
		-o -name '*.pdf' \) \
		-print | xargs rm -f
	rm -f $(OBJS) *.bak tags TAGS
	rm -fr *.*~

#########################################################################