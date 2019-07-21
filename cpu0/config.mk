DEVDIR  = $(TOPDIR)/dev
LIBDIR  = $(TOPDIR)/lib
ISPDIR	= $(CPU0DIR)/isp
COREDIR = $(CPU0DIR)/core
MKFDIR  = $(TOPDIR)/mk_files
CORELIBDIR = $(COREDIR)/enx_lib

export ENX_NET  = $(COREDIR)/net
export LWIP     = $(COREDIR)/net/lwip

ifeq ($(CONFIG_WIFI_SUPPORT),y)
export WIFI_LIB = $(COREDIR)/net/wifi

ifeq ($(WIFI_MODULE),sd8787)
export EWL_LIB  = ewl_sd8787
export BTDIR    = $(WIFI_LIB)/lwbt
endif

ifeq ($(WIFI_MODULE),sd8782)
export EWL_LIB  = ewl_sd8782
endif

export WLDIR    = $(WIFI_LIB)/$(EWL_LIB)
export WLLIBHDIR = $(COREDIR)/enx_lib_h/$(EWL_LIB)
endif

INCDIRS	 = -I$(DEVDIR)
INCDIRS += -I$(DEVDIR)/audio
INCDIRS += -I$(DEVDIR)/rtc
INCDIRS += -I$(DEVDIR)/gss
INCDIRS += -I$(LIBDIR)
INCDIRS += -I$(LIBDIR)/rtspoverhttp
INCDIRS += -I$(LIBDIR)/vsnprintf
INCDIRS += -I$(ISPDIR)
INCDIRS += -I$(COREDIR)
INCDIRS += -I$(COREDIR)/enx_lib_h
INCDIRS += -I$(COREDIR)/main
INCDIRS += -I$(COREDIR)/main/ftpd
INCDIRS += -I$(COREDIR)/main/http_tunnel
INCDIRS += -I$(COREDIR)/main/lwiperf
INCDIRS += -I$(COREDIR)/enx_audio
INCDIRS += -I$(COREDIR)/enx_os
INCDIRS += -I$(COREDIR)/enx_video
INCDIRS += -I$(COREDIR)/enx_stream
INCDIRS += -I$(COREDIR)/enx_rtmp
INCDIRS += -I$(ENX_NET)
INCDIRS += -I$(ENX_NET)/eth
INCDIRS += -I$(LWIP)/ports
INCDIRS += -I$(LWIP)/ports/include
INCDIRS += -I$(LWIP)/netif
INCDIRS += -I$(LWIP)/include
INCDIRS += -I$(LWIP)/include/ipv4
INCDIRS += -I$(GCCDIR)/or32-elf/include
INCDIRS += -I$(GCCDIR)/or32-elf/include/sys
INCDIRS += -I$(GCCDIR)/lib/gcc/or32-elf/4.5.1-or32-1.$(CVER)/include

ifeq ($(CONFIG_WIFI_SUPPORT),y)
INCDIRS += -I$(WIFI_LIB)
INCDIRS += -I$(WLDIR)
INCDIRS += -I$(WLLIBHDIR)
INCDIRS += -I$(BTDIR)
endif

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
CFLAGS +=	-fomit-frame-pointer -fno-strength-reduce	-fno-builtin
CFLAGS +=	-mhard-mul
CFLAGS +=	-mhard-div
#CFLAGS +=	-mhard-float
#CFLAGS +=	-msoft-float
CFLAGS +=	-nostdinc -nostdlib
CFLAGS +=	-Wno-unused-value
CFLAGS +=	-Wno-format
#CFLAGS +=	-finline-functions

CFLAGS += -DDEF_CPU0
CFLAGS += -DCONFIG_DCACHE_ENABLE

ifneq ($(CONFIG_FILE),)
include $(MKFDIR)/$(CONFIG_FILE)
endif

EXTRA_CFLAGS += -D_SIZE_FIXED
EXTRA_CFLAGS += -DPERI_SHARED
EXTRA_CFLAGS +=	-DCOMMON_SHARED

CFLAGS += 	$(EXTRA_CFLAGS) $(STB_CFLAGS) $(INLINE_CFLAGS)

GCC_LIB_OPTS  = -L$(GCCDIR)/or32-elf/lib/ -lc
GCC_LIB_OPTS += -L$(GCCDIR)/or32-elf/lib/ -lor32
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
