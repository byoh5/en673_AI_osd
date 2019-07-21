#ifeq ($(PLATFORM),windows)
#	GCCDIR = /cygdrive/c/eyenix/or32
#	export CVER = 0rc1
#else
#	PLATFORM = linux
#	GCCDIR = /opt/or32
#	export CVER = 0rc4
#endif
#
#ifeq ($(MODE),debug)
#	export EXTRA_CFLAGS := -D_DEBUG -D_BENCHMARK #-D_MALLOC
#else ifeq ($(MODE),trace)
#	export EXTRA_CFLAGS := -D_DEBUG -D_BENCHMARK #-D_MALLOC
#	#export EXTRA_CFLAGS := -D_DEBUG -D_BENCHMARK -D_INLINE#-D_MALLOC
#	export STB_CFLAGS := -finstrument-functions -D_STB_ENABLE -D_STB_NAME_ENABLE
#else
#	MODE = release
#endif

TARGET = $(SRC_CPU1)

# order is important here:
SUBDIRS	= rtos fs task $(LWIP)
LIB     = main_app.o

#########################################################################
include ./config.mk

ASMS     = reset.o
ASMS	+= $(DEVDIR)/cache.o
ASMS	+= $(DEVDIR)/exr1k_floats.o

OBJS    += file.o init.o main.o
OBJS    += $(LIBDIR)/qsort.o

# OLD DEV DEVDIR

OBJS    += $(DEVDIR)/sdcd.o
OBJS    += $(DEVDIR)/eth.o
OBJS    += $(DEVDIR)/phy.o
OBJS    += $(DEVDIR)/msg.o
OBJS    += $(DEVDIR)/uxsr.o
OBJS	+= $(DEVDIR)/user.o

ifeq ($(CONFIG_WIFI_SUPPORT),y)
OBJS    += $(DEVDIR)/wlif_tools.o
endif

# NEW DEV

OBJS 	+= $(DEVDIR)/exr1k_boot.o
OBJS	+= $(DEVDIR)/exr1k_spr.o
OBJS	+= $(DEVDIR)/exr1k_xsr.o
OBJS	+= $(DEVDIR)/exr1k_timer.o
OBJS	+= $(DEVDIR)/exr1k_floatc.o
ifeq ($(MODE),trace)
OBJS	+= $(DEVDIR)/exr1k_stb.o
endif

OBJS	+= $(LIBDIR)/stdlib.o
OBJS	+= $(LIBDIR)/string.o
OBJS	+= $(LIBDIR)/stdio.o
OBJS    += $(LIBDIR)/_gettimeofday.o
OBJS	+= $(LIBDIR)/vsnprintf/swfloat.o
OBJS	+= $(LIBDIR)/vsnprintf/hwfloat.o
OBJS	+= $(LIBDIR)/vsnprintf/vsnprintf.o

OBJS	+= $(DEVDIR)/adc.o
OBJS	+= $(DEVDIR)/cpu.o
OBJS	+= $(DEVDIR)/dma.o
OBJS	+= $(DEVDIR)/gpio.o

OBJS	+= $(DEVDIR)/i2c.o
#OBJS	+= $(DEVDIR)/i2s.o
OBJS	+= $(DEVDIR)/isp.o
OBJS	+= $(DEVDIR)/isr.o
#OBJS	+= $(DEVDIR)/sdio.o
#OBJS	+= $(DEVDIR)/secu.o
OBJS	+= $(DEVDIR)/sfls.o
OBJS	+= $(DEVDIR)/spi.o
OBJS	+= $(DEVDIR)/sys.o
#OBJS	+= $(DEVDIR)/test.o
OBJS	+= $(DEVDIR)/timer.o
OBJS	+= $(DEVDIR)/uart.o
OBJS	+= $(DEVDIR)/util.o

OBJS	+= $(DEVDIR)/audio/aud.o
OBJS	+= $(DEVDIR)/audio/tlv320aic3100.o
OBJS	+= $(DEVDIR)/audio/es8374.o

OBJS	+= $(DEVDIR)/rtc/rtc.o
OBJS	+= $(DEVDIR)/rtc/rtc-lib.o
OBJS	+= $(DEVDIR)/rtc/s35390a.o
OBJS	+= $(DEVDIR)/rtc/pcf8563.o
OBJS	+= $(DEVDIR)/gss/bma250.o

LIBS  = rtos/rtos.o fs/fs.o task/task.o $(LWIP)/tcpip.o $(LIB)

#########################################################################
ifeq ($(COMPILE_DIS),y)
all: $(TARGET) clean dis
#all: $(TARGET) dis
else
#all: $(TARGET) clean
all: $(TARGET)
endif

$(LIB):	$(ASMS) $(OBJS)
	@$(LD) -r -o $@ $(OBJS) $(ASMS)

subdirs:
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir || exit 1 ; done

$(TARGET): subdirs $(LIBS)
	@echo "$@ Link..."
ifeq ($(CONFIG_WIFI_SUPPORT),y)
ifeq ($(CONFIG_ONVIF_SUPPORT),y)
ifeq ($(CONFIG_ACTIVEX_SUPPORT),y)
	@$(LD) -T ./mem_wifi_onvif_activex.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
else
	@$(LD) -T ./mem_wifi_onvif.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
endif
else
ifeq ($(CONFIG_ACTIVEX_SUPPORT),y)
	@$(LD) -T ./mem_wifi_activex.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
else
	@$(LD) -T ./mem_wifi.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
endif
endif
else
ifeq ($(CONFIG_ONVIF_SUPPORT),y)
ifeq ($(CONFIG_ACTIVEX_SUPPORT),y)
	@$(LD) -T ./mem_eth_onvif_activex.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
else
	@$(LD) -T ./mem_eth_onvif.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
endif
else
ifeq ($(CONFIG_ACTIVEX_SUPPORT),y)
	@$(LD) -T ./mem_eth_activex.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
else
	@$(LD) -T ./mem_eth.ld $(LIBS) -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
endif
endif
endif
	@$(OBJCOPY) -O binary $(OUTDIR)/$@.or32 $(OUTDIR)/$@.bin
#	@$(OBJDUMP) -h $(OUTDIR)/$@.or32
	@echo " "
	@echo "Generate $@.bin: `cat $(OUTDIR)/$@.bin | wc -c` bytes"
	@echo " @PLATFORM: $(PLATFORM)"
	@echo " @MODE    : $(MODE)"
	@echo " @FLAGS   : $(EXTRA_CFLAGS) $(STB_CFLAGS) $(INLINE_CFLAGS)"
	@echo " "

dis:
	@echo "Disassemble Create..."
	@$(OBJDUMP) -DSx $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET).dis &
	@$(OBJDUMP) -d $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET)_vf.dis &

depend dep:
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir .depend ; done
#########################################################################

clean:
	@echo "$(TARGET) Clean..."
	@for dir in $(SUBDIRS); do make clean -C $$dir || exit 1 ; done
	@rm -rf $(OBJS) $(ASMS) $(LIB) *.dis *.or32 *.bin
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
