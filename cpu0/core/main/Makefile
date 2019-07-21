include $(CPU0DIR)/config.mk

LIBNAME	 = cpu_main.o

#########################################################################

#ASMS  = $(DEVDIR)/cache.o
#ASMS += $(DEVDIR)/exr1k_float.o

OBJS  = main.o
OBJS += init.o
OBJS += msgqpass.o
OBJS += stream_buffering.o
ifeq ($(CONFIG_WIFI_SUPPORT),y)
OBJS += inter_cpu.o
OBJS += inter_shell.o
endif
OBJS += ftpd/ftp_data.o
OBJS += http_tunnel/http_front.o
OBJS += http_tunnel/http_tunnel.o
OBJS += $(LIBDIR)/rtspoverhttp/share_rtspoverhttp.o
OBJS += lwiperf/lwiperf.o

#/*libs*/
OBJS += $(COREDIR)/enx_os/enx_os.o
OBJS += $(COREDIR)/enx_os/sscanf.o
#OBJS += $(TOPDIR)/libs/qsort.o

#########################################################################

all: $(LIBNAME)

$(LIBNAME): $(ASMS) $(OBJS) $(BSP_OBJS)
	@$(LD) -r -o $@ $(ASMS) $(OBJS) $(BSP_OBJS)

clean:
	@rm -rf $(LIBNAME) $(ASMS) $(OBJS) $(BSP_OBJS) .depend

#########################################################################

.depend: $(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

#########################################################################