include $(CPU0DIR)/config.mk

LIBNAME	= enx_port.o

#########################################################################

OBJS  = enx_lwip_helper.o
OBJS += ip_utils.o
OBJS += eth/open_eth.o
OBJS += eth/ethif.o
OBJS += enx_eth_helper.o

ifeq ($(CONFIG_WIFI_SUPPORT),y)
ifeq ($(WIFI_MODULE),sd8782)
OBJS += enx_wifi_helper.o
endif
ifeq ($(WIFI_MODULE),sd8787)
OBJS += enx_wifi_helper.o
#OBJS += enx_wifi_bt_helper.o
endif
OBJS += supplicant_init.o
OBJS += wl_tools.o
OBJS += wifi/mlanif.o

ifeq ($(WIFI_MODULE),sd8787)
#bluetooth stack
#BT = wifi/lwbt
#OBJS += $(BT)/bt_spp.o
#OBJS += $(BT)/lwbt_memp.o
#OBJS += $(BT)/hci.o
#OBJS += $(BT)/l2cap.o
#OBJS += $(BT)/sdp.o
##OBJS += $(BT)/rfcomm.o
##OBJS += $(BT)/fcs.o
#OBJS += $(BT)/hci_core.o
#OBJS += $(BT)/bt_utils.o
endif
endif

#########################################################################

all: $(LIBNAME)

$(LIBNAME): $(OBJS)
	@$(LD) -r -o $@ $(OBJS)

clean:
	@rm -rf $(LIBNAME) $(OBJS) .depend

#########################################################################

.depend: $(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@

#########################################################################