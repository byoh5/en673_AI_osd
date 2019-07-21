include ./config.mk

TARGET = $(SRC_CPU0)

SUBDIRS	= isp core

#########################################################################

ASMS	 = ./reset.o
ASMS	+= $(DEVDIR)/cache.o
ASMS	+= $(DEVDIR)/exr1k_floats.o

OBJS 	+= $(DEVDIR)/exr1k_boot.o
OBJS 	+= $(DEVDIR)/exr1k_spr.o
OBJS	+= $(DEVDIR)/exr1k_xsr.o
OBJS	+= $(DEVDIR)/exr1k_timer.o
OBJS	+= $(DEVDIR)/exr1k_floatc.o
ifeq ($(MODE),trace)
OBJS	+= $(DEVDIR)/exr1k_stb.o
endif

OBJS	+= $(LIBDIR)/stdlib.o
OBJS	+= $(LIBDIR)/string.o
OBJS	+= $(LIBDIR)/stdio.o
OBJS	+= $(LIBDIR)/qsort.o
OBJS	+= $(LIBDIR)/enx_math.o
OBJS	+= $(LIBDIR)/vsnprintf/swfloat.o
OBJS	+= $(LIBDIR)/vsnprintf/hwfloat.o
OBJS	+= $(LIBDIR)/vsnprintf/vsnprintf.o

OBJS 	+= $(DEVDIR)/adc.o
OBJS	+= $(DEVDIR)/audio/aud.o
OBJS	+= $(DEVDIR)/cpu.o
OBJS	+= $(DEVDIR)/dma.o
OBJS	+= $(DEVDIR)/gpio.o
OBJS	+= $(DEVDIR)/i2c.o
OBJS	+= $(DEVDIR)/isp.o
OBJS	+= $(DEVDIR)/isr.o
OBJS 	+= $(DEVDIR)/sdio.o
OBJS 	+= $(DEVDIR)/sfls.o
OBJS 	+= $(DEVDIR)/spi.o
OBJS	+= $(DEVDIR)/msg.o
OBJS	+= $(DEVDIR)/sys.o
OBJS	+= $(DEVDIR)/timer.o
OBJS	+= $(DEVDIR)/uart.o
OBJS	+= $(DEVDIR)/util.o
OBJS	+= $(DEVDIR)/user.o

#########################################################################

ifeq ($(COMPILE_DIS),y)
all: $(TARGET) clean dis
else
all: $(TARGET) clean
endif

clean:
	@echo "$(TARGET) Clean..."
	@for dir in $(SUBDIRS); do make clean -C $$dir || exit 1 ; done
	@rm -rf $(ASMS) $(OBJS) *.o
	@rm -rf $(DEVDIR)/*.nm $(LIBDIR)/*.nm $(TESTDIR)/*.nm
	@find . -name "*.o" -delete
	@find . -name "*.bak" -delete
	@find . -name "*.nm" -delete

$(TARGET): subdirs $(ASMS) $(OBJS)
	@echo "$@ Link..."
	@$(LD) -r -o $@.o $(ASMS) $(OBJS) isp/isp.o core/core.o
ifeq ($(CONFIG_WIFI_SUPPORT),y)
	@$(LD) -T ./mem_wifi.ld $@.o -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
else
	@$(LD) -T ./mem_eth.ld $@.o -o $(OUTDIR)/$@.or32 $(GCC_LIB_OPTS)
endif
	@$(OBJCOPY) -O binary $(OUTDIR)/$@.or32 $(OUTDIR)/$@.bin
	@$(OBJDUMP) -h $(OUTDIR)/$@.or32
	@echo " "
	@echo "Generate $@.bin: `cat $(OUTDIR)/$@.bin | wc -c` bytes"
	@echo " @PLATFORM: $(PLATFORM)"
	@echo " @MODE    : $(MODE)"
	@echo " @FLAGS   : $(EXTRA_CFLAGS) $(STB_CFLAGS) $(INLINE_CFLAGS)"
#	@echo " "

dis:
	@echo "Disassemble Create..."
	@$(OBJDUMP) -DSx $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET).dis &
	@$(OBJDUMP) -d $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET)_vf.dis &

subdirs:
	@for dir in $(SUBDIRS) ; do $(MAKE) -C $$dir || exit 1 ; done

#########################################################################