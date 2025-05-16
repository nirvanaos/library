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

namespace {

void civil_from_days(time_t days_since_epoch, int *year, unsigned int *month, unsigned int *day)
	noexcept
{
	time_t time = days_since_epoch + 719468;
	int era = (int)((time >= 0 ? time : time - 146096) / 146097);
	unsigned int doe = static_cast <unsigned int> (time - era * 146097);
	unsigned int yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
	int y = static_cast <int> (yoe) + era * 400;
	unsigned int doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
	unsigned int mp = (5 * doy + 2) / 153;
	unsigned int d = doy - (153 * mp + 2) / 5 + 1;
	unsigned int m = mp + (mp < 10 ? 3 : -9);

	*year = y + (m <= 2);
	*month = m;
	*day = d;
}

void weekday_from_days(time_t days_since_epoch, unsigned int *weekday) noexcept
{
	*weekday = static_cast <unsigned int> (days_since_epoch >= -4 ?
			(days_since_epoch + 4) % 7 : (days_since_epoch + 5) % 7 + 6);
}

void yearday_from_date(unsigned int year, unsigned int month, unsigned int day, unsigned int *yday)
	noexcept
{
	unsigned int n1 = 275 * month / 9;
	unsigned int n2 = (month + 9) / 12;
	unsigned int n3 = (1 + (year - 4 * year / 4 + 2) / 3);
	*yday = n1 - (n2 * n3) + day - 30;
}

}

extern "C" struct tm *gmtime_r (const time_t *unix_gmt, struct tm *res)
{
	int year;
	unsigned int month;
	unsigned int day;
	unsigned int weekday;
	unsigned int yday;

	time_t unix_local = *unix_gmt;

	int days_since_epoch = (int)(unix_local / (60 * 60 * 24));
	civil_from_days (days_since_epoch, &year, &month, &day);
	weekday_from_days (days_since_epoch, &weekday);
	yearday_from_date (year, month, day, &yday);

	res->tm_sec = unix_local % 60;
	res->tm_min = (unix_local / 60) % 60;
	res->tm_hour = (unix_local / (60*60)) % 24;
	res->tm_mday = day;
	res->tm_mon = month - 1;
	res->tm_year = year - 1900;
	res->tm_wday = weekday;
	res->tm_yday = yday - 1;
	res->tm_isdst = -1;
	res->tm_gmtoff = 0;

	return res;
}
