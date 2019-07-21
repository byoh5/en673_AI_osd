/* Copyright (c) 2013 Eyenix Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* 3. The name of Eyenix may not be used to endorse or promote products derived
* from this software without specific prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Eyenix
* product.
*
* THIS SOFTWARE IS PROVIDED BY EYENIX "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
* EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL EYENIX BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
*
*/

#ifndef _UPNPD_H_
#define _UPNPD_H_

#include "lwip/def.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"

typedef enum {
	upnpudp_alive,
	upnpudp_byebye
}UpnpUdpMsgType;

UpnpUdpMsgType UpnpRecvMsg;

//*************************************************************************************************
// Misc
//-------------------------------------------------------------------------------------------------
//
#define RCVBUFSIZE			1024

#define UPNP_MCAST_ADDR		"239.255.255.250"

#define	UPNP_BRDCAST_PERIOD	100	// Periodic broadcasting time : 2√ ¥Á 1»∏


//-------------------------------------------------------------------------------------------------
// UPNP URL
#define UPNP_Root_URL				"caminfo.xml"
#define UPNP_Sub1_URL				"stream1.xml"

// UPNP Message Info
#define UPNP_SERVER					"EYENIX NETWORK CAMERA"
#define UPNP_DEVICETYPE				"enxIPCam"
#define UPNP_MANUFACTURER			"EYENIX"
#define UPNP_MANUFACTURER_URL		"http://www.eyenix.com"
#define UPNP_MODELDESCRIPTION		"EYENIX_NETCAM"
#define UPNP_MODELNAME				"ENC1000"
#define UPNP_MODELNUMBER			"ENC1000-V1.0"
#define UPNP_SERIALNUMBER			"000C29C59910"




//*************************************************************************************************
// Extern
//-------------------------------------------------------------------------------------------------
// Function
//extern void UpnpScanTask(void *pvParameters);
//extern void UpnpUdpTask(void *pvParameters);
//extern void UpnpTcpTask(void *pvParameters);

extern void UpnpMsearch(void);
extern void upnpd_init(void *pvParameters);

extern UINT GetCrc(BYTE *pData, WORD wNum);
//extern void MacFilterSet( struct netif *netif, struct ip_addr *group, u8_t action);

//-------------------------------------------------------------------------------------------------
// Variable



#endif	// _UPNPD_H_
