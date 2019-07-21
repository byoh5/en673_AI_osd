#include "dev.h"

#include "lwip/init.h"
#include "lwip/netif.h"

extern int sscanf(const char *str, const char *format, ...);

const char* 
ip2str(struct ip_addr addr)
{
    static char buf[16];
    snprintf(buf, sizeof(buf), "%lu.%lu.%lu.%lu",
             (addr.addr >> 24) & 0xff,
             (addr.addr >> 16) & 0xff,
             (addr.addr >> 8) & 0xff,
             (addr.addr) & 0xff);
    return buf;
}

struct ip_addr 
str2ip(const char* str)
{
    int a,b,c,d;
    u32_t ip = 0;

    sscanf(str,"%d.%d.%d.%d",&a,&b,&c,&d);
    if (a < 0 || a > 255 || b < 0 || b > 255 ||
        c < 0 || c > 255 || d < 0 || d > 255)
        goto out;
    
    ip = (a << 24) | (b << 16) | (c << 8) | d;
out:
    return *(struct ip_addr*) &ip;
}

const char* 
mac2str(char* mac)
{
    static char buf[18];
    snprintf(buf, sizeof(buf), "%02x-%02x-%02x-%02x-%02x-%02x",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    return buf;
}