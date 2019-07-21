//*************************************************************************************************
// SNTP client task
//*************************************************************************************************
// This is simple "SNTP" client for socket or raw API. It is a minimal implementation of SNTPv4 as specified in RFC 4330.
// For a list of some public NTP servers, see this link : http://support.ntp.org/bin/view/Servers/NTPPoolServers
// @todo:
// - set/change servers at runtime
// - complete SNTP_CHECK_RESPONSE checks 3 and 4
// - support broadcast/multicast mode?

#include "dev.h"
#include "lib.h"

#include "factory.h"

#include "user.h"
#include "msg.h"

#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/api.h"
#include "lwip/netdb.h"
#include "lwip/inet.h"

#include "networkif.h"
#include "ethernetif.h"

#include <time.h>

#ifdef __RTC_LOAD__
#include "rtc.h"
#endif

#if defined(__ETH__) ||defined(__WIFI__)
#if (ENX_SNTPC_use==1)

#include "timer_task.h"
#include "sntpc.h"

#define ENX_SNTPC_DEBUG			// printf 주석용

#ifdef ENX_SNTPC_DEBUG
	char *strSNTPSTATE[] = {
			"eSNTP_INIT",
			"eSNTP_PROCESS",
			"eSNTP_TIMESET",
			"eSNTP_DELAY",
			"eSNTP_FAIL"
		};

	#define gprintf(state, fmt, args...) do { printf("%04d %s : %s"fmt, __LINE__, __func__, strSNTPSTATE[state], ##args); } while(0);
#else
	#define gprintf(state, fmt, args...) do {} while(0);
#endif

sntp_state sntp_info;		// sntp state, info, etc
static u32 tid_sntpc = 0xFFFFFFFF;	// timer id
static sys_thread_t stt_sntp;		// task handle

#if SNTP_CHECK_RESPONSE >= 2
static u32_t sntp_last_timestamp_sent[2];			// Saves the last timestamp sent (which is sent back by the server) to compare against in response
#endif

//*************************************************************************************************
// SNTP
//-------------------------------------------------------------------------------------------------
// SNTP processing of received timestamp
// Convert SNTP time (1900-based) to unix GMT time (1970-based)	@todo: if MSB is 1, SNTP time is 2036-based!
static void sntp_process(UINT *receive_timestamp)
{
	time_t t = (ntohl(receive_timestamp[0]) - DIFF_SEC_1900_1970);
	struct tm tmout;

	// gtUser.nSystemTime이 0인 것은 한번도 저장안함
	if (gtUser.nSystemTime == 0) {
		gtUser.nSystemTime = t;
	}

	gptMsgShare.TIME = t;	// Change system time and/or the update the RTC clock
	sntp_info.last_gettime = t;
	sntp_info.is_timeset = 1;	// Watch set end

#ifdef __RTC_LOAD__
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_NO);
	rtc_set_time(&tmout);
#endif

	// 실 구현시 불필요 코드 / 콘솔상 확인 기능
#ifdef ENX_DEBUG
	enx_get_tmtime(gptMsgShare.TIME, &tmout, DEF_YES);
	printf("SNTP : %04d-%02d-%02d %s %02d:%02d:%02d\r\n", tmout.tm_year+1900, tmout.tm_mon+1, tmout.tm_mday, strWeek[tmout.tm_wday], tmout.tm_hour, tmout.tm_min, tmout.tm_sec);
#endif
}

//-------------------------------------------------------------------------------------------------
// Initialize request struct to be sent to server.
static void sntp_initialize_request(struct sntp_msg *req)
{
	memset(req, 0, SNTP_MSG_LEN);
	req->li_vn_mode = SNTP_LI_NO_WARNING|SNTP_VERSION|SNTP_MODE_CLIENT;

#if SNTP_CHECK_RESPONSE>=2
	u32_t sntp_time_sec, sntp_time_us;

	sntp_time_sec = gptMsgShare.TIME;
	sntp_time_us  = 0;

	sntp_last_timestamp_sent[0] = htonl(sntp_time_sec + DIFF_SEC_1900_1970);	// Fill in transmit timestamp and save it in 'sntp_last_timestamp_sent'
	sntp_last_timestamp_sent[1] = htonl(sntp_time_us);							//					"
	req->transmit_timestamp[0]  = sntp_last_timestamp_sent[0];					//					"
	req->transmit_timestamp[1]  = sntp_last_timestamp_sent[1];					//					"
#endif
}

//-------------------------------------------------------------------------------------------------
// Send an SNTP request via sockets (Minimal implementation that does not fully conform to the SNTPv4 RFC, especially regarding server load and error procesing)
static void SntpRequest(ip_addr_t atDnsIp)
{
	int					sock, tolen, size, timeout, retry_cnt=0;
	struct sockaddr_in	local, to;
	struct sntp_msg		sntpmsg;
	u32_t				sntp_server_address;
	struct in_addr		temp;

	sntp_server_address = atDnsIp.addr;
	temp.s_addr = sntp_server_address;
	sock = lwip_socket(AF_INET, SOCK_DGRAM, 0);													// Create new socket

	if(sock >= 0){
		memset(&local, 0, sizeof(local));														// Prepare local address
		local.sin_family      = AF_INET;														//		"
		local.sin_port        = htons(INADDR_ANY);												//		"
		local.sin_addr.s_addr = htonl(INADDR_ANY);												//		"

		memset(&to, 0, sizeof(to));																// Prepare SNTP server address
		to.sin_family      = AF_INET;															//		"
		to.sin_port        = htons(SNTP_PORT);													//		"
		to.sin_addr.s_addr = sntp_server_address;												//		"
		lwip_connect(sock,(struct sockaddr*)&to,sizeof(struct sockaddr_in));

		if(lwip_bind(sock, (struct sockaddr*)&local, sizeof(local)) == 0){						// Bind to local address
			timeout = SNTP_RECV_TIMEOUT;														// Set recv timeout
			lwip_setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

			sntp_initialize_request(&sntpmsg);													// Prepare SNTP request
_retry:
			if(lwip_sendto(sock, &sntpmsg, SNTP_MSG_LEN, 0, (struct sockaddr*)&to, sizeof(to)) >= 0){					// Send SNTP request to server
				tolen = sizeof(to);
				size  = lwip_recvfrom(sock, &sntpmsg, SNTP_MSG_LEN, 0, (struct sockaddr*)&to, (socklen_t*)&tolen);	// Receive SNTP server response

				if(size == SNTP_MSG_LEN){																				// If the response size is good
					if(((sntpmsg.li_vn_mode & SNTP_MODE_MASK) == SNTP_MODE_SERVER) || ((sntpmsg.li_vn_mode & SNTP_MODE_MASK) == SNTP_MODE_BROADCAST))
					{
						sntp_process(sntpmsg.receive_timestamp);			// If this is a SNTP response...
					}
				}
				else{
					retry_cnt++;
					if(retry_cnt == 3)		goto	_end;
					else					goto	_retry;
				}
			}
		}
_end:
		closesocket(sock);
	}
}

//-------------------------------------------------------------------------------------------------
// Task
void SntpcTask(void *pvParameters)
{
	sntp_info.state = eSNTP_INIT;
	sntp_info.is_timeset = 0;
	sntp_info.retry_cnt = 0;
	sntp_info.iptDns.addr = 0;
	gprintf(sntp_info.state, "\r\n");

	while (1) {
		network_check_netif();

		sntp_info.state = eSNTP_PROCESS;
		gprintf(sntp_info.state, ":Get Host by name.\r\n");
		err_t ert = netconn_gethostbyname((const char *)gtUser.strSntpSvr, &sntp_info.iptDns);	// Get DNS server address

		gprintf(sntp_info.state, ":%s(%IP) ert(%s)\r\n", gtUser.strSntpSvr, sntp_info.iptDns.addr, lwip_strerr(ert));
		if (ert == ERR_OK) {
			SntpRequest(sntp_info.iptDns);
			if (sntp_info.is_timeset == 1) { // Time Set ==> 1
				sntp_info.state = eSNTP_TIMESET;
				gprintf(sntp_info.state, "\r\n");
				stt_sntp = NULL;
				vTaskDelete(NULL);
			}
		}

		if (sntp_info.retry_cnt++ > gtUser.nSntpRetryMaxcnt) {
			sntp_info.state = eSNTP_FAIL;
			gprintf(sntp_info.state, "\r\n");
			stt_sntp = NULL;
			vTaskDelete(NULL);
		}

		sntp_info.state = eSNTP_DELAY;
		gprintf(sntp_info.state, "\r\n");
		vTaskDelay(gtUser.nSntpRetrySec * configTICK_RATE_HZ); // 5s뒤 재시도
	}

	UNUSED(pvParameters);
}

static void sntpc_init(void *ctx)
{
	if (stt_sntp == NULL) {
		gprintf(sntp_info.state, " : Task Start\r\n");
		stt_sntp = sys_thread_new("sntpc", SntpcTask, NULL, _32KB_STACK_SIZE, SNTPC_TASK_PRIO);
	} else {
		// 이미 task가 살아있는 상태. 시간값을 받고 죽거나, 최대 SNTP_RETRY_CNT회 시도 후 죽거나
		gprintf(sntp_info.state, " : sntpc start fail\r\n");
	}
	UNUSED(ctx);
}

void sntpc_start(void)
{
	sntpc_stop();		// 기존 설정 stop
	sntpc_init(NULL);	// 즉시 SNTP Task 생성
	tid_sntpc = timer_sched_timeout_cb(gtUser.nSntpPeriodic * 1000, TIMEOUT_PERIODIC, sntpc_init, NULL);	// n초 단위로  반복
}

void sntpc_stop(void)
{
	if (tid_sntpc != 0xFFFFFFFF) { // sntp timer가 활성화 되어있다면...
		if (stt_sntp != NULL && sntp_info.state != eSNTP_PROCESS) { // sntp task는 살아있으나, 언제든지 죽여도 된다.
			vTaskDelete(stt_sntp);
			stt_sntp = NULL;
		}

		// 그 외의 case에서는
		// 1. sntp task가 죽어있는 상태, timer만 죽인다.
		// 2. sntp task가 네트워크 통신중이다. 성공하든 실패(최대 SNTP_RETRY_CNT회)하든 task는 알아서 죽는다.

		timer_cancel_timeout(tid_sntpc);
		tid_sntpc = 0xFFFFFFFF;
	}
}
#endif
#endif
