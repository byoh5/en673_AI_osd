#ifndef __IPUTIL_H__
#define __IPUTIL_H__


#define IP_UTIL_LISTEN_PORT		59163
#define IP_UTIL_SEND_PORT		59164
#define IP_UTIL_UPDATE_PORT		59165


#define u_int8_t    unsigned char
#define u_int16_t   unsigned short
#define u_int32_t   unsigned long


typedef struct __netconf_Message 
{
	u_int8_t opcode;	// opcode
	u_int8_t nettype;	// network type
	u_int16_t secs;		// retransmission seconds
	u_int32_t xid;		// transaction id, random number
	u_int32_t magic;	// magic code
	u_int32_t ciaddr;	// client's current ip address
	u_int8_t chaddr[16];	// client hardware address
	u_int32_t yiaddr;	// client's yielded ip address (your ip)
	u_int32_t miaddr;	// net mask ip address
	u_int32_t giaddr;	// gateway ip address
	u_int32_t d1iaddr;	// dns first ip address
	u_int32_t d2iaddr;	// dns second ip address
	u_int32_t d3iaddr;	// dns third ip address
	u_int32_t siaddr;	// server (CMS) ip address
	u_int32_t sport;	// server (CMS) port to notify the state of ready
	u_int32_t http_port;	// server (HTTP) port to serve http service
	u_int8_t vend[128];		// vendor specific informations
	u_int32_t stream_port; // stream server port // codemate
	u_int8_t fw_ver[20];	// firmware version // codemate
	u_int8_t vidstd;		// video standard // codemate
	u_int32_t flag;			// 0¹øºñÆ® (0: Auto Ip On , 1: Off)
	u_int8_t chaddr2[6];	// codemate
	u_int32_t running_time;
}  __attribute__((packed))NETCONF_MESSAGE ;



enum {		// opcode
	MSG_IP_SEARCH = 1,
	MSG_CAM_ACK	,
	MSG_IP_ACK	,
	MSG_IP_SET	,
	MSG_CAM_SET ,
	MSG_IPUTIL_SEARCH,
	MSG_IPUTIL_RUN,
	MSG_IP_REBOOT,
	MSG_IP_UPDATE,
	MSG_PTZ_UPDATE,	// 10
	MSG_IP_MACUPDATE,
	MSG_CAM_AUTOIP,
	MSG_IP_DHCP
};

enum {		// nettype
	E_NET_STATIC = 1,
	E_NET_DHCP	,
	E_NET_PPPoE
};


#define MAGIC			0x73747278	// magic of seboss strx configuration


/* =======================================================*/
/* ==================== External Functions =====================*/
/* =======================================================*/
extern void IpUtil_Init(void);
extern void IpUtil_Deinit(void);

#endif // __IPUTIL_H__