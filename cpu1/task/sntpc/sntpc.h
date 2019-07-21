#ifndef _SNTPC_H_
#define _SNTPC_H_



//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
// SNTP packet format (without optional fields)
// Timestamps are coded as 64 bits:
// - 32 bits seconds since Jan 01, 1970, 00:00
// - 32 bits seconds fraction (0-padded)
// For future use, if the MSB in the seconds part is set, seconds are based on Feb 07, 2036, 06:28:16.
struct sntp_msg{
	BYTE	li_vn_mode;
	BYTE	stratum;
	BYTE	poll;
	BYTE	precision;
	UINT	root_delay;
	UINT	root_dispersion;
	UINT	reference_identifier;
	UINT	reference_timestamp[2];
	UINT	originate_timestamp[2];
	UINT	receive_timestamp[2];
	UINT	transmit_timestamp[2];
} __attribute__ ((packed));

//...............................................
// SNTP protocol defines
#define SNTP_MSG_LEN					48

#define SNTP_OFFSET_LI_VN_MODE			0
#define SNTP_LI_MASK					0xc0
#define SNTP_LI_NO_WARNING				0x00
#define SNTP_LI_LAST_MINUTE_61_SEC		0x01
#define SNTP_LI_LAST_MINUTE_59_SEC		0x02
#define SNTP_LI_ALARM_CONDITION			0x03	// Clock not synchronized

#define SNTP_VERSION_MASK				0x38
#define SNTP_VERSION					(4<<3)	// NTP Version 4

#define SNTP_MODE_MASK					0x07
#define SNTP_MODE_CLIENT				0x03
#define SNTP_MODE_SERVER				0x04
#define SNTP_MODE_BROADCAST				0x05

#define SNTP_OFFSET_STRATUM				1
#define SNTP_STRATUM_KOD				0x00

#define SNTP_OFFSET_ORIGINATE_TIME		24
#define SNTP_OFFSET_RECEIVE_TIME		32
#define SNTP_OFFSET_TRANSMIT_TIME		40

#define DIFF_SEC_1900_1970				(2208988800UL)		// number of seconds between 1900 and 1970

typedef enum {
	eSNTP_INIT    = 0,
	eSNTP_PROCESS = 1,
	eSNTP_TIMESET = 2,
	eSNTP_DELAY   = 3,
	eSNTP_FAIL    = 4,
} eSNTP_STATE;

typedef struct {
	eSNTP_STATE state;
	ip_addr_t iptDns;
	UINT is_timeset;
	UINT retry_cnt;
	time_t last_gettime;
} sntp_state;

//-------------------------------------------------------------------------------------------------
// Sanity check
// - 0    To turn off sanity checks (default; smaller code)
// - >= 1 To check address and port of the response packet to ensure the response comes from the server we sent the request to.
// - >= 2 To check returned Originate Timestamp against Transmit Timestamp sent to the server (to ensure response to older request).
// - >= 3 @todo: Discard reply if any of the LI, Stratum, or Transmit Timestamp fields is 0 or the Mode field is not 4 (unicast) or 5 (broadcast).
// - >= 4 @todo: To check that the Root Delay and Root Dispersion fields are each greater than or equal to 0 and less than infinity, where infinity is
//               currently a cozy number like one second. This check avoids using a server whose synchronization source has expired for a very long time.
#define SNTP_CHECK_RESPONSE				0

//...............................................
//
// 100 => 1s
// 6000 => 60s => 1min
// 360000 => 3600s => 1hour
// 8640000 => 24hour
#define SNTP_UPDATE_DELAY				360000				// SNTP update delay - in milliseconds. Default is 1 hour(i.e. 3600000).
#define SNTP_RECV_TIMEOUT				3000				// SNTP receive timeout - in milliseconds. Also used as retry timeout - this shouldn't be too low. Default is 3 seconds

//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
extern void sntpc_start(void);
extern void sntpc_stop(void);

//-------------------------------------------------------------------------------------------------
// Variable
extern sntp_state sntp_info;	// sntp state, info, etc

#endif // _SNTPC_H_
