include ../config.mk

LIB  =	tcpip.o
OBJS =  ./src/api/api_lib.o		\
	./src/api/api_msg.o		\
	./src/api/err.o			\
	./src/api/netbuf.o		\
	./src/api/netdb.o		\
	./src/api/netifapi.o		\
	./src/api/sockets.o		\
	./src/api/tcpip.o		\
	./src/core/def.o		\
	./src/core/dhcp.o		\
	./src/core/dns.o		\
	./src/core/init.o		\
	./src/core/mem.o		\
	./src/core/memp.o		\
	./src/core/netif.o		\
	./src/core/pbuf.o		\
	./src/core/raw.o		\
	./src/core/stats.o		\
	./src/core/sys.o		\
	./src/core/tcp.o		\
	./src/core/tcp_in.o		\
	./src/core/tcp_out.o		\
	./src/core/timers.o		\
	./src/core/udp.o		\
	./src/core/ipv4/autoip.o	\
	./src/core/ipv4/icmp.o		\
	./src/core/ipv4/igmp.o		\
	./src/core/ipv4/inet.o		\
	./src/core/ipv4/inet_chksum.o	\
	./src/core/ipv4/ip.o		\
	./src/core/ipv4/ip_addr.o	\
	./src/core/ipv4/ip_frag.o	\
	./src/core/snmp/asn1_dec.o	\
	./src/core/snmp/asn1_enc.o	\
	./src/core/snmp/mib2.o		\
	./src/core/snmp/mib_structs.o	\
	./src/core/snmp/msg_in.o	\
	./src/core/snmp/msg_out.o	\
	./src/netif/etharp.o		\
	./sys_arch.o			\
	./ethernetif.o			\
	./wifiif.o			\
	./networkif.o

all:	$(LIB)

$(LIB):	$(OBJS)
	@$(LD) -r -o $@ $(OBJS)

clean:
	@rm -rf $(OBJS) $(LIB) .depend	

#########################################################################
.depend :	$(OBJS:.o=.c)
	@$(CC) -M $(CFLAGS) $(OBJS:.o=.c) > $@
#########################################################################
