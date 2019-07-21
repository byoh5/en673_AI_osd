DEVDIR = $(TOPDIR)/dev
LIBDIR = $(TOPDIR)/lib
MKFDIR = $(TOPDIR)/mk_files

INCDIRS	=\
		-I$(DEVDIR)\
		-I$(LIBDIR)\
		-I$(LIBDIR)/vsnprintf\
		-I$(GCCDIR)/or32-elf/include\
		-I$(GCCDIR)/or32-elf/include/sys\
		-I$(GCCDIR)/lib/gcc/or32-elf/4.5.1-or32-1.$(CVER)/include

#
# Include the make variables (CC, etc...)
#
CROSS_COMPILE = $(GCCDIR)/bin/or32-elf-

AS	= $(CROSS_COMPILE)as
LD	= $(CROSS_COMPILE)ld
CC	= $(CROSS_COMPILE)gcc
AR	= $(CROSS_COMPILE)ar
NM	= $(CROSS_COMPILE)nm
STRIP	= $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB	= $(CROSS_COMPILE)ranlib
BINSIZE = $(CROSS_COMPILE)size

CFLAGS  =	$(INCDIRS) -W -Wall -Wstrict-prototypes
CFLAGS +=	-Os -pipe
CFLAGS +=	-fomit-frame-pointer -fno-strength-reduce -fno-builtin
CFLAGS +=	-mhard-mul -mhard-div -mhard-float
CFLAGS +=	-nostdinc -nostdlib
CFLAGS += 	-Wno-unused-value

ifneq ($(CONFIG_FILE),)
include $(MKFDIR)/$(CONFIG_FILE)
endif

ifneq ($(SECTION),y)
EXTRA_CFLAGS +=	-D_SIZE_FIXED
endif
EXTRA_CFLAGS +=	-DPERI_SHARED
EXTRA_CFLAGS +=	-DCOMMON_SHARED

ifeq ($(SECTION),y)
CFLAGS += -DDEF_SECTION
endif
CFLAGS += -DDEF_BOOT
CFLAGS += $(EXTRA_CFLAGS)

GCC_LIB_OPTS  = -L$(GCCDIR)/or32-elf/lib/ -lc
GCC_LIB_OPTS += -L$(GCCDIR)/lib/gcc/or32-elf/4.5.1-or32-1.$(CVER) -lgcc

ifeq ($(CONFIG_WIFI_SUPPORT),y)
CFLAGS += -D__WIFI__

ifeq ($(WIFI_MODULE),sd8787)
CFLAGS += -DSD8787_ENABLE
#CFLAGS += -D__BT__
endif

ifeq ($(WIFI_MODULE),sd8782)
CFLAGS += -DSD8782_ENABLE
endif

endif

ifeq ($(CONFIG_ETH_SUPPORT),y)
CFLAGS += -D__ETH__
endif

ifeq ($(CONFIG_ONVIF_SUPPORT), y)
CFLAGS += -D__ONVIF__
endif

ifeq ($(CONFIG_ACTIVEX_SUPPORT), y)
CFLAGS += -D__ACTIVEX__
endif

#########################################################################
#$< 현재타겟보다 최근에 변경된 종속 항목 리스트
#$@ 현재 타켓의 이름
# 확장자 규칙
#########################################################################

%.o: %.S
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "> $*.S"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(NM) $@ > $@.nm
	@echo "> $*.c"

%.bin: %.or32
	or32-elf-objcopy -O binary $< $@

%.img: %.bin
	utils/bin2flimg 1 $< > $@

%.srec: %.bin
	utils/bin2srec $< > $@

%.hex: %.bin
	utils/bin2hex $< > $@

#########################################################################
