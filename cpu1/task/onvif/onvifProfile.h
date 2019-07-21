
#ifndef __ONVIF_PROFILE_H__
#define __ONVIF_PROFILE_H__

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <lwip/netif.h>
#include <enx_gsoap_lwip.h>

#include "dev.h"

#include "user.h"

#include "onvifSystemInfo.h"
#include "onvifUtil.h"


/* temp */
int tempdnssearchdomainsize;

extern void init_onvif_profiles(void);
extern void DataSetting(SysInfo *SysInfo);

#endif /* __ONVIF_PROFILE_H__ */


