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

TARGET = $(SRC_BOOT)

SRCDIR = main

#########################################################################
include ./config.mk

ASMS	 = $(SRCDIR)/reset.o

OBJS 	 = $(DEVDIR)/exr1k_boot.o
OBJS 	+= $(DEVDIR)/exr1k_spr.o
OBJS	+= $(DEVDIR)/exr1k_xsr.o

OBJS	+= $(LIBDIR)/stdlib.o
OBJS	+= $(LIBDIR)/string.o
OBJS	+= $(LIBDIR)/stdio.o
OBJS	+= $(LIBDIR)/vsnprintf/vsnprintf.o

OBJS	+= $(DEVDIR)/cpu.o
OBJS	+= $(DEVDIR)/ddr.o
OBJS	+= $(DEVDIR)/dma.o
OBJS	+= $(DEVDIR)/uart.o
OBJS	+= $(DEVDIR)/sfls.o
OBJS	+= $(DEVDIR)/user.o
OBJS	+= $(DEVDIR)/msg.o
OBJS	+= $(DEVDIR)/sys.o

OBJS 	+= $(SRCDIR)/main.o

#########################################################################
ifeq ($(COMPILE_DIS),y)
all: $(TARGET) clean dis
else
all: $(TARGET) clean
endif

clean:
	@echo "$(TARGET) Clean..."
	@rm -rf $(ASMS) $(OBJS) *.o
	@find . $(DEVDIR) $(LIBDIR) -name "*.nm" -delete
	@find . -name "*.o" -delete
	@find . -name "*.bak" -delete

$(TARGET): $(ASMS) $(OBJS)
	@echo "$@ Link..."
	@$(LD) -r -o $@.o $(ASMS) $(OBJS) $(GCC_LIB_OPTS)
ifeq ($(SECTION),y)
	@$(LD) -T ./mem_s.ld $@.o -o $(OUTDIR)/$@.or32
else
	@$(LD) -T ./mem.ld $@.o -o $(OUTDIR)/$@.or32
endif
	@$(OBJCOPY) -O binary $(OUTDIR)/$@.or32 $(OUTDIR)/$@.bin

#	@$(OBJDUMP) -h $(OUTDIR)/$@.or32
	@echo " "
	@echo "Generate $(OUTDIR)/$@.bin: `cat $(OUTDIR)/$@.bin | wc -c` bytes"
	@echo " @PLATFORM: $(PLATFORM)"
	@echo " @MODE    : $(MODE)"
	@echo " @FLAGS   : $(EXTRA_CFLAGS) $(STB_CFLAGS) $(INLINE_CFLAGS)"
	@echo " "

dis:
	@echo "Disassemble Create..."
	@$(OBJDUMP) -DSx $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET).dis &
	@$(OBJDUMP) -d $(OUTDIR)/$(TARGET).or32 > $(OUTDIR)/$(TARGET)_vf.dis &
