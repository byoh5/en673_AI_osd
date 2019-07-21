include ../config.mk

LIB	 = task.o

OBJS	 = ./_lib/Base64.o
OBJS	+= ./_lib/md5.o

OBJS	+= ./lwiperf/lwiperf.o
OBJS	+= ./ping/ping.o
OBJS	+= ./network_debugging/network_debugging.o

OBJS	+= ./shell/shell.o
OBJS	+= ./shell/shell_cmd.o
OBJS	+= ./shell/shell_cmd_isp.o
OBJS	+= ./shell/shell_cmd_sdio.o
OBJS	+= ./shell/shell_cmd_common.o
OBJS	+= ./shell/shell_cmd_net.o
OBJS	+= ./shell/shell_cmd_fs.o

OBJS	+= ./terminal/term_server.o

OBJS	+= ./timer/timer_task.o

OBJS	+= ./change/videochange.o
OBJS	+= ./change/ipchange.o

OBJS	+= ./httpserver_raw/fs.o
OBJS	+= ./httpserver_raw/fsfilelist.o
OBJS	+= ./httpserver_raw/httpd.o
OBJS	+= ./httpserver_raw/cgissi.o
OBJS	+= ./httpserver_raw/cgifuncs.o
OBJS	+= ./httpserver_raw/cgissi_en673.o
OBJS	+= ./httpserver_raw/cgissi_isp.o
ifeq ($(CONFIG_WIFI_SUPPORT),y)
OBJS	+= ./httpserver_raw/fsaplist.o
OBJS	+= ./httpserver_raw/cgissi_wifi.o
endif
OBJS	+= ./httpserver_raw/http_authorization.o
OBJS	+= ./httpserver_raw/http_tunneling.o
OBJS	+= $(LIBDIR)/rtspoverhttp/share_rtspoverhttp.o

OBJS	+= ./sntpc/sntpc.o
OBJS	+= ./upnpd/upnpd.o

OBJS	+= ./sdcard/sdcard.o

OBJS	+= ./ftpd/ftp_server.o
OBJS	+= ./ftpd/ftp_util.o

OBJS	+= ./avienc/fmt_avienc.o
OBJS	+= ./avienc/muxer_avienc.o
OBJS	+= ./avienc/stream_buffering.o
OBJS	+= ./avienc/avi_restore.o
OBJS	+= ./avienc/avienc_info.o
OBJS	+= ./smtpc/smtpc.o
OBJS	+= ./ddnsc/ddnsc.o
OBJS	+= ./iputil/iputil.o
OBJS	+= ./dhcpc/dhcpc.o
OBJS	+= ./isp/isp_task.o
OBJS	+= ./fwupdate/fwupdate.o

ifeq ($(CONFIG_WIFI_SUPPORT),y)
OBJS	+= ./dhcpd/dhcpd.o
OBJS	+= ./dhcpd/files.o
OBJS	+= ./dhcpd/leases.o
OBJS	+= ./dhcpd/options.o
OBJS	+= ./dhcpd/serverpacket.o

OBJS	+= ./wifi/wifi.o
OBJS	+= ./wifi/wifi_task.o
endif

ifeq ($(CONFIG_ONVIF_SUPPORT), y)
OBJS	+= ./onvif/interface/onvifDiscoveryInterface.o
OBJS	+= ./onvif/interface/onvifDeviceInterface.o
OBJS	+= ./onvif/interface/onvifMediaInterface.o
OBJS	+= ./onvif/interface/onvifImagingInterface.o
OBJS	+= ./onvif/interface/onvifEventHandleInterface.o
OBJS	+= ./onvif/interface/onvifPtzInterface.o
OBJS	+= ./onvif/onvifTask.o
OBJS	+= ./onvif/onvifDiscoveryTask.o
OBJS	+= ./onvif/onvifSystemInfo.o
OBJS	+= ./onvif/onvifUtil.o
OBJS	+= ./onvif/onvifProfile.o
OBJS	+= ./onvif/enx_gsoap_lwip.o
OBJS	+= ./onvif/enx_gsoap_http.o

#OBJS	+= ./onvif/interface/onvifDeviceIOInterface.o
#OBJS	+= ./onvif/interface/onvifAnalyticsDeviceInterface.o
#OBJS	+= ./onvif/interface/onvifDisplayInterface.o
#OBJS	+= ./onvif/interface/onvifReceiverInterface.o
#OBJS	+= ./onvif/interface/onvifReplayInterface.o
#OBJS	+= ./onvif/interface/onvifRecordingInterface.o
#OBJS	+= ./onvif/interface/onvifSearchInterface.o

GSOAPSERVER = ./onvif/gsoap/soapServer.o

GSOAPLIB  = ./onvif/gsoap/gsoap-2.8.8/stdsoap2.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/plugin/smdevp.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/plugin/threads.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/plugin/wsseapi.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/import/dom.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/custom/duration.o
GSOAPLIB += ./onvif/gsoap/gsoap-2.8.8/custom/sha1.o
GSOAPLIB += ./onvif/gsoap/soapC.o
GSOAPOBJ = $(GSOAPLIB)

#all: webfsdata libgsoap.a $(LIB) #clean
all: clean webfsdata $(LIB) #libgsoap.a
#all: webfsdata $(LIB)

$(LIB):$(OBJS) $(GSOAPSERVER)
	@$(LD) -r -o $@ $(OBJS) $(GSOAPSERVER) -L ../liba -lgsoap

libgsoap.a: $(GSOAPOBJ)
	@$(AR) rscv libgsoap.a $(GSOAPOBJ)
	@mv libgsoap.a ../liba/.

clean:
	@rm -rf $(OBJS) $(UPNPOBJ) $(GSOAPSERVER) $(LIB) .depend
# $(GSOAPLIB)

else

all:	webfsdata $(LIB)
#all:	$(LIB)

$(LIB):	$(OBJS)
	@$(LD) -r -o $@ $(OBJS)

clean:
	@rm -rf $(OBJS) $(UPNPOBJ) $(LIB) .depend
endif

ifeq ($(CONFIG_ACTIVEX_SUPPORT), y)
webfsdata:
	@perl ./httpserver_raw/makefsdata ./httpserver_raw/fs_activex/ ./httpserver_raw/fsdata.c
else
webfsdata:
	@perl ./httpserver_raw/makefsdata ./httpserver_raw/fs/ ./httpserver_raw/fsdata.c
endif

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
