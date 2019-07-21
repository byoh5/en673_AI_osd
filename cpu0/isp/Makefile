#ifeq ($(PLATFORM),windows)
#	GCCDIR = /cygdrive/c/eyenix/or32
#else
#	PLATFORM = linux
#	GCCDIR = /opt/or32
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

include ../config.mk
include lib/libver.mk

TARGET = isp

#########################################################################

OBJS 	 = isp_dev.o
OBJS	+= isp_dev_flash.o
OBJS	+= isp_dev_gpio.o
OBJS	+= isp_dev_irq.o
OBJS	+= isp_dev_isp.o
OBJS	+= isp_dev_spi.o
OBJS	+= isp_dev_timer.o
OBJS	+= isp_dev_twi.o
OBJS	+= isp_dev_uart.o
OBJS	+= isp_dev_wdt.o

OBJS	+= isp_app.o
OBJS	+= isp_app_ae.o
OBJS	+= isp_app_awb.o
OBJS	+= isp_app_comm.o
OBJS	+= isp_app_cont.o
OBJS	+= isp_app_dbg.o
OBJS	+= isp_app_defect.o
OBJS	+= isp_app_etc.o
OBJS	+= isp_app_fadj.o
OBJS	+= isp_app_maf.o

OBJS	+= isp_app_init.o
OBJS	+= isp_app_key.o
OBJS	+= isp_app_main.o
OBJS	+= isp_app_menu.o
OBJS	+= isp_app_mode.o
OBJS	+= isp_app_tdn.o
OBJS	+= isp_app_util.o
OBJS	+= isp_app_shd.o
#OBJS	+= en332s_main.o

OBJS	+= isp_app_imd.o
OBJS	+= isp_app_imd_test.o
OBJS	+= Enc_Mem_Test.o
#OBJS	+= isp_app_Test_Etc.o

OBJS	+= isp_image_path.o

#########################################################################
LIBS	+= -Llib -lEN673L_$(LIB_VER)



#########################################################################
#all: clean $(TARGET)
all: $(TARGET)

clean:
	@rm -rf $(OBJS) *.o &
	@rm -rf $(DEVDIR)/*.nm $(LIBDIR)/*.nm $(TESTDIR)/*.nm *.nm &
	@rm -rf $(OUTPUT)/*.a

$(TARGET): $(OBJS)
	@echo "ISP Link..."
	@$(LD) -r -o $(TARGET).o $(OBJS) $(LIBS)
