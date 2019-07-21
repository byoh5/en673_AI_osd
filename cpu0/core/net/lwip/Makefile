include $(CPU0DIR)/config.mk

LIBNAME	= enx_lwip.o

#########################################################################

OBJS  = ./api/api_lib.o
OBJS += ./api/api_msg.o
OBJS += ./api/err.o
OBJS += ./api/netbuf.o
OBJS += ./api/netifapi.o
OBJS += ./api/sockets.o
OBJS += ./api/tcpip.o
OBJS += ./core/def.o
OBJS += ./core/dhcp.o
OBJS += ./core/dns.o
OBJS += ./core/init.o
OBJS += ./core/mem.o
OBJS += ./core/memp.o
OBJS += ./core/netif.o
OBJS += ./core/pbuf.o
OBJS += ./core/raw.o
OBJS += ./core/stats.o
OBJS += ./core/sys.o
OBJS += ./core/tcp.o
OBJS += ./core/tcp_in.o
OBJS += ./core/tcp_out.o
OBJS += ./core/timers.o
OBJS += ./core/udp.o
OBJS += ./core/ipv4/autoip.o
OBJS += ./core/ipv4/icmp.o
OBJS += ./core/ipv4/igmp.o
OBJS += ./core/ipv4/inet.o
OBJS += ./core/ipv4/inet_chksum.o
OBJS += ./core/ipv4/ip.o
OBJS += ./core/ipv4/ip_addr.o
OBJS += ./core/ipv4/ip_frag.o
OBJS += ./core/snmp/asn1_dec.o
OBJS += ./core/snmp/asn1_enc.o
OBJS += ./core/snmp/mib2.o
OBJS += ./core/snmp/mib_structs.o
OBJS += ./core/snmp/msg_in.o
OBJS += ./core/snmp/msg_out.o
OBJS += ./netif/etharp.o
OBJS += ./ports/sys_arch.o

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