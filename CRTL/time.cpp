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
	return Nirvana::the_chrono->steady_clock ();
}

extern "C" time_t time (time_t * t)
{
	time_t time = Nirvana::the_chrono->UTC ().time () / 10000000 - TimeBase::UNIX_EPOCH;
	if (t)
		*t = time;
	return time;
}

extern "C" struct tm *localtime_r (const time_t *t, struct tm *tm)
{
	time_t time = *t + Nirvana::the_chrono->system_clock ().tdf ();
	return gmtime_r (&time, tm);
}

extern "C" int nanosleep (const struct timespec* rq, struct timespec* rm)
{
	TimeBase::TimeT t = rq->tv_sec * TimeBase::SECOND + rq->tv_nsec / 100;
	struct timespec rem = { 0, 0 };
	try {
		Nirvana::the_posix->sleep (t);
	} catch (...) {
		rem = *rq;
		errno = ENOTSUP;
		return -1;
	}
	return 0;
}
