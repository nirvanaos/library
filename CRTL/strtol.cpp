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
#include <Nirvana/strtoi.h>
#include <errno.h>

extern "C"
long strtol (const char* __restrict s, char** __restrict ptr, int base)
{
	long ret;
	Nirvana::strtoi (s, ptr, base, ret);
	return ret;
}

extern "C"
long long strtoll (const char* __restrict s, char** __restrict ptr, int base)
{
	long long ret;
	Nirvana::strtoi (s, ptr, base, ret);
	return ret;
}

extern "C"
unsigned long strtoul (const char* __restrict s, char** __restrict ptr, int base)
{
	unsigned long ret;
	Nirvana::strtoi (s, ptr, base, ret);
	return ret;
}

extern "C"
unsigned long long strtoull (const char* __restrict s, char** __restrict ptr, int base)
{
	unsigned long long ret;
	Nirvana::strtoi (s, ptr, base, ret);
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

