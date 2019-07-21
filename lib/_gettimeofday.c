#if 1
//#include "board.h"
#include <errno.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include "msg.h"

/*
 * _gettimeofday -- implement in terms of time, which means we can't return the
 * microseconds.
 */
int
_gettimeofday(struct timeval *tv, void *tzvp)
{
    struct timezone *tz = tzvp;
    if (tz)
     tz->tz_minuteswest = tz->tz_dsttime = 0;
    tv->tv_usec = 0;
    tv->tv_sec = gptMsgShare.TIME;
    return 0;

}
#endif
