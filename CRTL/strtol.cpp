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
#include <stdlib.h>
#include <Nirvana/strtoi.h>

namespace CRTL {

template <typename C, typename Int> inline
void strtoi (const C* s, C** endptr, int base, Int& ret) noexcept
{
	int err = Nirvana::strtoi (s, endptr, base, ret);
	if (err)
		errno = err;
}

}

extern "C"
long strtol (const char* s, char** endptr, int base)
{
	long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
long long strtoll (const char* s, char** endptr, int base)
{
	long long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
unsigned long strtoul (const char* s, char** endptr, int base)
{
	unsigned long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
unsigned long long strtoull (const char* s, char** endptr, int base)
{
	unsigned long long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
long wcstol (const wchar_t* s, wchar_t** endptr, int base)
{
	long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
long long wcstoll (const wchar_t* s, wchar_t** endptr, int base)
{
	long long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
unsigned long wcstoul (const wchar_t* s, wchar_t** endptr, int base)
{
	unsigned long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
unsigned long long wcstoull (const wchar_t* s, wchar_t** endptr, int base)
{
	unsigned long long ret;
	CRTL::strtoi (s, endptr, base, ret);
	return ret;
}

extern "C"
long atol (const char* s)
{
	return strtol (s, nullptr, 10);
}

extern "C"
long long atoll (const char* s)
{
	return strtoll (s, nullptr, 10);
}

#if INT_MAX < LONG_MAX

extern "C"
int atoi (const char* s)
{
	int ret;
	Nirvana::strtoi (s, (char**)nullptr, 10, ret);
	return ret;
}

#endif

