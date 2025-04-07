/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*
* Send comments and/or bug reports to:
*  popov.nirvana@gmail.com
*/
#ifndef _TIME_H_
#define _TIME_H_
#pragma once

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

#ifdef _MSC_VER
typedef struct timespec _timespec64;
#endif

struct itimerspec
{
	struct timespec it_interval; // Timer period.
	struct timespec it_value;    // Timer expiration.
};

#define CLOCKS_PER_SEC 10000000

#define TIME_UTC 1

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

typedef int locale_t;

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

char* asctime (const struct tm* timeptr);
char* asctime_r (const struct tm* restrict, char* restrict);
clock_t clock (void);
int clock_getres (clockid_t clock_id, struct timespec* res);
int clock_gettime (clockid_t clock_id, struct timespec* tp);
int clock_settime (clockid_t clock_id, const struct timespec* tp);
char* ctime (const time_t* timer);
double difftime (time_t, time_t);
struct tm *gmtime (const time_t *);
struct tm *gmtime_r (const time_t *restrict, struct tm *restrict);
struct tm *localtime (const time_t *);
struct tm *localtime_r (const time_t *restrict, struct tm *restrict);
time_t mktime (struct tm *);
int nanosleep (const struct timespec*, struct timespec*);
size_t strftime (char *restrict, size_t, const char *restrict, const struct tm *restrict);
size_t strftime_l(char *restrict, size_t, const char *restrict, const struct tm *restrict,
	locale_t);
time_t time (time_t *);
int timespec_get (struct timespec* ts, int base);

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
