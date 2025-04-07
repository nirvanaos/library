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
#include "pch/pch.h"
#include <time.h>

extern "C" clock_t clock (void)
{
	return Nirvana::the_posix->steady_clock ();
}

extern "C" time_t time (time_t * t)
{
	time_t time = Nirvana::the_posix->UTC ().time () / 10000000 - TimeBase::UNIX_EPOCH;
	if (t)
		*t = time;
	return time;
}

extern "C" int clock_getres (clockid_t clock_id, struct timespec* res)
{
	long r;
	switch (clock_id) {
		case CLOCK_REALTIME:
			r = (long)Nirvana::the_posix->system_clock_resolution ();
			break;
		case CLOCK_MONOTONIC:
			r = (long)Nirvana::the_posix->steady_clock_resolution ();
			break;
		default:
			*(errno_t*)Nirvana::the_posix->error_number () = EINVAL;
			return -1;
	}

	if (res) {
		res->tv_sec = 0;
		res->tv_nsec = r * 100;
	}
	return 0;
}

extern "C" int clock_gettime (clockid_t clock_id, struct timespec* tp)
{
	uint64_t t;
	switch (clock_id) {
		case CLOCK_REALTIME:
			t = Nirvana::the_posix->UTC ().time () - TimeBase::UNIX_EPOCH * 10000000UI64;
			break;
		case CLOCK_MONOTONIC:
			t = Nirvana::the_posix->steady_clock ();
			break;
		default:
			*(errno_t*)Nirvana::the_posix->error_number () = EINVAL;
			return -1;
	}

	if (tp) {
		lldiv_t d = lldiv (t, 10000000I64);
		tp->tv_sec = d.quot;
		tp->tv_nsec = (long)d.rem * 100;
	}
	return 0;
}

extern "C" int clock_settime (clockid_t clock_id, const struct timespec* tp)
{
	if (CLOCK_REALTIME != clock_id || !tp) {
		*(errno_t*)Nirvana::the_posix->error_number () = EINVAL;
		return -1;
	}

	int err = EINVAL;
	if (tp->tv_sec > 0 && tp->tv_nsec >= 0 && tp->tv_nsec < 1000000000) {

		TimeBase::TimeT t = tp->tv_sec * 10000000I64 + tp->tv_nsec / 100
			+ TimeBase::UNIX_EPOCH * 10000000UI64;

		try {
			Nirvana::the_posix->set_UTC (t);
			return 0;
		} catch (const CORBA::SystemException& ex) {
			int e = Nirvana::get_minor_errno (ex.minor ());
			if (e)
				err = e;
		} catch (...) {}
	}

	*(int*)Nirvana::the_posix->error_number () = err;
	return -1;
}

extern "C" int timespec_get (struct timespec* ts, int base)
{
	if (TIME_UTC == base && 0 != clock_gettime (CLOCK_REALTIME, ts))
		return base;
	*(errno_t*)Nirvana::the_posix->error_number () = EINVAL;
	return 0;
}

extern "C" struct tm *localtime_r (const time_t *t, struct tm *tm)
{
	time_t time = *t + Nirvana::the_posix->system_clock ().tdf ();
	return gmtime_r (&time, tm);
}

extern "C" int nanosleep (const struct timespec* rq, struct timespec* rm)
{
	if (rq && rq->tv_sec >= 0 && rq->tv_nsec >= 0 && rq->tv_nsec < 1000000000) {
		TimeBase::TimeT t = rq->tv_sec * TimeBase::SECOND + rq->tv_nsec / 100;
		if (rm) {
			rm->tv_sec = 0;
			rm->tv_nsec = 0;
		}
		try {
			Nirvana::the_posix->sleep (t);
		} catch (...) {}
		return 0;
	}
	*(errno_t*)Nirvana::the_posix->error_number () = EINVAL;
	return -1;
}
