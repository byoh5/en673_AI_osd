
#ifndef __IP__UTILS__H__
#define __IP__UTILS__H__
const char* 
ip2str(struct ip_addr addr);

struct ip_addr 
str2ip(const char* str);

const char* 
mac2str(char* mac);

#endif