#ifndef _TIME_H_
#define _TIME_H_

#include "stddef.h"
#include "sys/types.h"

struct tm
{
	int    tm_sec;   // Seconds [0,60]. 
	int    tm_min;   // Minutes [0,59]. 
	int    tm_hour;  // Hour [0,23]. 
	int    tm_mday;  // Day of month [1,31]. 
	int    tm_mon;   // Month of year [0,11]. 
	int    tm_year;  // Years since 1900. 
	int    tm_wday;  // Day of week [0,6] (Sunday =0). 
	int    tm_yday;  // Day of year [0,365]. 
	int    tm_isdst; // Daylight Savings flag. 
};

struct timespec
{
	time_t tv_sec;  // Seconds
	long   tv_nsec; // Nanoseconds
};

struct itimerspec
{
	struct timespec it_interval; // Timer period.
	struct timespec it_value;    // Timer expiration.
};

#define CLOCKS_PER_SEC 10000000

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

clock_t clock (void);
double difftime (time_t, time_t);
struct tm *gmtime (const time_t *);
struct tm *gmtime_r (const time_t *restrict, struct tm *restrict);
struct tm *localtime (const time_t *);
struct tm *localtime_r (const time_t *restrict, struct tm *restrict);
time_t mktime (struct tm *);
size_t strftime (char *restrict, size_t, const char *restrict,
                 const struct tm *restrict);
time_t time (time_t *);

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
