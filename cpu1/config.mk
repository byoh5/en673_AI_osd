DEVDIR = $(TOPDIR)/dev
LIBDIR = $(TOPDIR)/lib
SRCDIR = $(TOPDIR)/cpu1
MKFDIR = $(TOPDIR)/mk_files

export LWIP = lwip-1.4.1

INCDIRS	= \
		-I$(TOPDIR) \
		-I$(DEVDIR) \
		-I$(DEVDIR)/audio \
		-I$(DEVDIR)/rtc \
		-I$(DEVDIR)/gss \
		-I$(LIBDIR) \
		-I$(LIBDIR)/rtspoverhttp \
		-I$(LIBDIR)/vsnprintf\
		-I$(SRCDIR)/rtos/src/include \
		-I$(SRCDIR)/rtos \
		-I$(SRCDIR)/$(LWIP)/src/include/ipv4 \
		-I$(SRCDIR)/$(LWIP)/src/include \
		-I$(SRCDIR)/$(LWIP)/arch \
		-I$(SRCDIR)/$(LWIP) \
		-I$(SRCDIR)/fs/fat/src \
		-I$(SRCDIR)/fs/fat \
		-I$(SRCDIR)/fs \
		-I$(SRCDIR)/task/_lib \
		-I$(SRCDIR)/task/lwiperf \
		-I$(SRCDIR)/task/ping \
		-I$(SRCDIR)/task/httpserver_raw \
		-I$(SRCDIR)/task/change \
		-I$(SRCDIR)/task/sntpc \
		-I$(SRCDIR)/task/upnpd \
		-I$(SRCDIR)/task/shell \
		-I$(SRCDIR)/task/terminal \
		-I$(SRCDIR)/task/isp \
		-I$(SRCDIR)/task/sdcard \
		-I$(SRCDIR)/task/ftpd \
		-I$(SRCDIR)/task/timer \
		-I$(SRCDIR)/task/avienc \
		-I$(SRCDIR)/task/dhcpc \
		-I$(SRCDIR)/task/dhcpd \
		-I$(SRCDIR)/task/fwupdate \
		-I$(SRCDIR)/task/smtpc \
		-I$(SRCDIR)/task/ddnsc \
		-I$(SRCDIR)/task/iputil \
		-I$(SRCDIR)/task/wifi \
		-I$(SRCDIR)/task/onvif \
		-I$(SRCDIR)/task/onvif/gsoap \
		-I$(SRCDIR)/task/onvif/gsoap/gsoap-2.8.8 \
		-I$(SRCDIR)/task/onvif/gsoap/gsoap-2.8.8/import \
		-I$(SRCDIR)/task/onvif/gsoap/gsoap-2.8.8/plugin \
		-I$(SRCDIR)/task/onvif/gsoap/gsoap-2.8.8/custom \
		-I$(SRCDIR)/task/network_debugging \
		-I$(SRCDIR) \
		-I$(GCCDIR)/or32-elf/include \
		-I$(GCCDIR)/or32-elf/include/sys \
		-I$(GCCDIR)/lib/gcc/or32-elf/4.5.1-or32-1.$(CVER)/include
#               -I$(SRCDIR)/task/gsensor \
#		-I$(SRCDIR)/task/gps \
#		-I$(SRCDIR)/task/avidec \

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
CFLAGS +=	-Os
CFLAGS +=	-fomit-frame-pointer -fno-strength-reduce -fno-builtin
CFLAGS +=	-mhard-mul
CFLAGS +=	-mhard-div
#CFLAGS +=	-mhard-float
#CFLAGS +=	-msoft-float
CFLAGS +=	-nostdinc -nostdlib
CFLAGS +=	-D_GCC_LIMITS_H_ 
CFLAGS += -Wno-unused-value
CFLAGS += -Wno-multichar
CFLAGS += -Wno-format

CFLAGS += -DDEF_CPU1
CFLAGS += -DCONFIG_DCACHE_ENABLE

ifneq ($(CONFIG_FILE),)
include $(MKFDIR)/$(CONFIG_FILE)
endif

EXTRA_CFLAGS += -D_SIZE_FIXED
EXTRA_CFLAGS += -DPERI_SHARED
EXTRA_CFLAGS += -DCOMMON_SHARED

CFLAGS += 	$(EXTRA_CFLAGS) $(STB_CFLAGS) $(INLINE_CFLAGS)

GCC_LIB_OPTS  = -L$(GCCDIR)/or32-elf/lib/ -lc
GCC_LIB_OPTS += -L$(GCCDIR)/or32-elf/lib/ -lm
GCC_LIB_OPTS += -L$(GCCDIR)/or32-elf/lib/ -lor32 -lor32uart
GCC_LIB_OPTS += -L$(GCCDIR)/lib/gcc/or32-elf/4.5.1-or32-1.$(CVER) -lgcc

ifeq ($(CONFIG_WIFI_SUPPORT), y)
	CFLAGS += -D__WIFI__
endif

ifeq ($(CONFIG_ETH_SUPPORT), y)
	CFLAGS += -D__ETH__
endif

ifeq ($(CONFIG_ONVIF_SUPPORT), y)
	ONVIF_CFLAGS = -DWITH_NOIO -DWITH_NOHTTP #-DWITH_LEAN -DWITH_NOSTDLIB	 
	CFLAGS += -D__ONVIF__
	CFLAGS += $(ONVIF_CFLAGS)
endif

ifeq ($(CONFIG_ACTIVEX_SUPPORT), y)	 
	CFLAGS += -D__ACTIVEX__
endif

#########################################################################

export	CONFIG_SHELL HOSTCC HOSTCFLAGS CROSS_COMPILE \
	AS LD CC AR NM STRIP OBJCOPY OBJDUMP \
	MAKE CFLAGS ASFLAGS GCC_OPT GCC_LIB_OPTS

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
