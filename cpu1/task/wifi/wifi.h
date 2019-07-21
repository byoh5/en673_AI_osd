#ifndef _WIFI_H_
#define _WIFI_H_

#include "dev_types.h"
#include "wifi_cfg.h"

extern tWificfg gtWificfg;

extern u32 WifiCFG_Save(void);
extern u32 WifiCFG_Load(void);

#endif // _WIFI_H_