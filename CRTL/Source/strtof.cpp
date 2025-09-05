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
#include <stdlib.h>
#include <errno.h>
#include <Nirvana/strtof.h>
#include <Nirvana/POSIX.h>
#include "impl/locale.h"

namespace CRTL {

template <typename C, typename F> inline
void strtof (const C* s, C** endptr, F& ret,
	const struct lconv* lconv = Nirvana::the_posix->cur_locale ()->localeconv ()) noexcept
{
	int err = Nirvana::strtof (s, endptr, ret, lconv);
	if (err)
		errno = err;
}

template <typename C, typename F> inline
void strtof (const C* s, C** endptr, F& ret, locale_t loc) noexcept
{
	Nirvana::Locale::_ptr_type locale = CRTL::check_locale (loc);
	if (!locale) {
		if (endptr)
			*endptr = const_cast <C*> (s);
		ret = 0;
	} else
		strtof (s, endptr, ret, locale->localeconv ());
}

}

extern "C" {
	
float strtof (const char* str, char** endptr)
{
	float ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

float strtof_l (const char* str, char** endptr, locale_t loc)
{
	float ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

double strtod (const char* str, char** endptr)
{
	double ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

double strtod_l (const char* str, char** endptr, locale_t loc)
{
	double ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

long double strtold (const char* str, char** endptr)
{
	long double ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

long double strtold_l (const char* str, char** endptr, locale_t loc)
{
	long double ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

float wcstof (const wchar_t* str, wchar_t** endptr)
{
	float ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

float wcstof_l (const wchar_t* str, wchar_t** endptr, locale_t loc)
{
	float ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

double wcstod (const wchar_t* str, wchar_t** endptr)
{
	double ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

double wcstod_l (const wchar_t* str, wchar_t** endptr, locale_t loc)
{
	double ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

long double wcstold (const wchar_t* str, wchar_t** endptr)
{
	long double ret;
	CRTL::strtof (str, endptr, ret);
	return ret;
}

long double wcstold_l (const wchar_t* str, wchar_t** endptr, locale_t loc)
{
	long double ret;
	CRTL::strtof (str, endptr, ret, loc);
	return ret;
}

}
