#ifndef _WL_TOOLS_H_
#define _WL_TOOLS_H_

#include "ewl.h"

void
wl_scan_results(int num_scan, struct wpa_scan_result *results_scan);

void
wl_print_list_networks(void);

void
wl_print_apcfg(void);

void
wl_printf_sta_list(void);

const char* 
ip2str(struct ip_addr addr);

struct ip_addr 
str2ip(const char* str);

const char* 
mac2str(char* mac);

void 
wl_sprintf_ie_txt(char *sbuff, const char *proto, const u8 *ie, size_t ie_len);


#endif
