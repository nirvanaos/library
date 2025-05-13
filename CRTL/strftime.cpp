/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#include <Nirvana/Formatter.h>
#include <algorithm>
#include <wctype.h>

namespace Nirvana {

struct DateTimeLocale
{
	const char* month [12];
	const char* abmonth [12];
	const char* day [7];
	const char* abday [7];
	const char* am;
	const char* pm;
	const char* date_format;
	const char* time_format;
	const char* time_format_ampm;
	const char* date_time_format;
};

static const DateTimeLocale default_time_locale = {
	{	"January", "Feburary", "March", "April", "May", "June", "July", "August", "September",
		"October", "November", "December" },
	{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" },
	{ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" },
	{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" },
	"AM", "PM",
	"%m/%d/%y",
	"%H:%M:%S",
	"%I:%M:%S %p"
};

}

namespace CRTL {

using namespace Nirvana;

void strftime (WideOut& out, WideIn& format, const struct tm& tm,
	const DateTimeLocale& loc = default_time_locale);

void strftime (WideOut& out, const char* format, const struct tm& tm,
	const DateTimeLocale& loc)
{
	WideInStrUTF8 in (format);
	strftime (out, in, tm, loc);
}

static void append_format (WideOut& out, const char* fmt, ...)
{
	va_list args;
	va_start (args, fmt);
	WideInStr <char> fmt_in (fmt);
	Formatter::format (fmt_in, args, out);
	va_end (args);
}

static void append (WideOut& out, const char* s)
{
	WideInStrUTF8 in (s);
	copy (in, out);
}

void strftime (WideOut& out, WideIn& format, const struct tm& tm, const DateTimeLocale& loc)
{
	//auto c = format;
	//auto p = dest;
	bool use_alternative_symbols = false;
	bool use_alternative_era_format = false;

	int32_t c = format.get ();
	while (c) {

		if(c != '%') {
			out.put (c);
			c = format.get ();
			continue;
		}

		c = format.get ();
		if (c == 'O') {
			char valid [] = {'B', 'b', 'd', 'e', 'H', 'I', 'm', 'M', 'S', 'u', 'U', 'V', 'w', 'W', 'y'};
			c = format.get ();
			if (std::find (std::begin (valid), std::end (valid), c) != std::end (valid)) {
				use_alternative_symbols = true;
				c = format.get ();
			} else {
				out.put ('%');
				out.put ('O');
				continue;
			}
		} else if (c == 'E') {
			char valid [] = {'c', 'C', 'x', 'X', 'y', 'Y'};
			c = format.get ();
			if (std::find (std::begin (valid), std::end (valid), c) != std::end (valid)) {
				use_alternative_era_format = true;
				c = format.get ();
			} else {
				out.put ('%');
				out.put ('E');
				continue;
			}
		}

		switch (c) {
		case 'Y':
			append_format (out, "%d", 1900 + tm.tm_year);
			break;
		case 'm':
			append_format (out, "%.2d", tm.tm_mon + 1);
			break;
		case 'd':
			append_format (out, "%.2d", tm.tm_mday);
			break;
		case 'z': {
			auto min = tm.tm_gmtoff / 60;
			auto diff = ((min / 60) * 100) + (min % 60);
			append_format (out, "%c%04d", diff >= 0 ? '+' : '-', abs (diff));
			break;
		}
		case 'Z':
			append (out, "UTC");
			break;
		case 'H':
			append_format (out, "%.2i", tm.tm_hour);
			break;
		case 'M':
			append_format (out, "%.2i", tm.tm_min);
			break;
		case 'S':
			append_format (out, "%.2d", tm.tm_sec);
			break;
		case 'R':
			append_format (out, "%.2i:%.2i", tm.tm_hour, tm.tm_min);
			break;
		case 'T':
			append_format (out, "%.2i:%.2i:%.2i", tm.tm_hour, tm.tm_min, tm.tm_sec);
			break;
		case 'F':
			append_format (out, "%d-%.2d-%.2d", 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday);
			break;
		case 'D':
			append_format (out, "%.2d/%.2d/%.2d", tm.tm_mon + 1, tm.tm_mday, (tm.tm_year + 1900) % 100);
			break;
		case 'a': {
			int day = tm.tm_wday;
			if (day >= 0 && day < 7)
				append (out, loc.abday [day]);
			break;
		}
		case 'b':
		case 'B':
		case 'h': {
			int mon = tm.tm_mon;
			if (mon >= 0 && mon < 12)
				append (out, (c == 'B') ? loc.month [mon] : loc.abmonth [mon]);
			break;
		}
		case 'c': {
			int day = tm.tm_wday;
			int mon = tm.tm_mon;
			if (day >= 0 && day < 7 && mon >= 0 && mon < 12) {
				append_format (out, "%s %s %2d %.2i:%.2i:%.2d %d", loc.abday [day],
					loc.abmonth [mon], tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, 1900 + tm.tm_year);
			}
			break;
		}
		case 'e':
			append_format (out, "%2d", tm.tm_mday);
			break;
		case 'l': {
			int hour = tm.tm_hour;
			if(!hour)
				hour = 12;
			if(hour > 12)
				hour -= 12;
			append_format (out, "%2d", hour);
			break;
		}
		case 'k':
			append_format (out, "%2d", tm.tm_hour);
			break;
		case 'I': {
			int hour = tm.tm_hour;
			if(!hour)
				hour = 12;
			if(hour > 12)
				hour -= 12;
			append_format (out, "%.2d", hour);
			break;
		}
		case 'p': {
			append (out, (tm.tm_hour < 12) ? loc.am : loc.pm);
			break;
		}
		case 'P': {
			const char *str = (tm.tm_hour < 12) ? loc.am : loc.pm;
			WideInStrUTF8 in (str);
			for (auto wc = in.get (); wc; wc = in.get ())
				out.put (towlower (wc));
			break;
		}
		case 'C':
			append_format (out, "%.2d", (1900 + tm.tm_year) / 100);
			break;
		case 'y':
			append_format (out, "%.2d", (1900 + tm.tm_year) % 100);
			break;
		case 'j':
			append_format (out, "%.3d", tm.tm_yday + 1);
			break;
		case 'A': {
			int day = tm.tm_wday;
			if (day >= 0 && day < 7)
				append (out, loc.day [day]);
			break;
		}
		case 'r': {
			int hour = tm.tm_hour;
			if(!hour)
				hour = 12;
			if(hour > 12)
				hour -= 12;
			append_format (out, "%.2i:%.2i:%.2i %s", hour, tm.tm_min, tm.tm_sec,
				((tm.tm_hour < 12) ? loc.am : loc.pm));
			break;
		}
		case '%':
			out.put ('%');
			break;
		case 'n':
			out.put ('\n');
			break;
		case 't':
			out.put ('\t');
			break;
		case 'x':
			strftime (out, loc.date_format, tm, loc);
			break;
		case 'X':
			strftime (out, loc.time_format, tm, loc);
			break;
		case '\0':
			out.put ('%');
			continue;
		}
		c = format.get ();
	}
}

template <class C>
size_t strftime (C* buf, size_t maxsize, const C* fmt, const struct tm* time)
{
	WideOutBufT <C> out (buf, buf + maxsize);
	WideInStrT <C> in (fmt);
	strftime (out, in, *time);
	size_t cnt = out.count ();
	if (cnt >= maxsize) {
		errno = ERANGE;
		return 0;
	} else
		return cnt;
}

}

extern "C" size_t strftime (char* buf, size_t maxsize, const char* fmt, const struct tm* time)
{
	return CRTL::strftime (buf, maxsize, fmt, time);
}

extern "C" size_t strftime_l (char* buf, size_t maxsize, const char* fmt, const struct tm* time,
	locale_t)
{
	return strftime (buf, maxsize, fmt, time);
}
