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

// Seconds from 15 October 1582 00:00:00 to 1 January 1970 00:00:00
static const int64_t UNIX_EPOCH = 12219336000;

extern "C" clock_t clock (void)
{
	return Nirvana::g_system->steady_clock ();
}

extern "C" time_t time (time_t * t)
{
	time_t time = Nirvana::g_system->UTC ().time () / 10000000 - UNIX_EPOCH;
	if (t)
		*t = time;
	return time;
}

extern "C" struct tm *localtime_r (const time_t *t, struct tm *tm)
{
	time_t time = *t + Nirvana::g_system->system_clock ().tdf ();
	return gmtime_r (&time, tm);
}
