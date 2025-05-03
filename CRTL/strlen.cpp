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
#include <string.h>
#include <wchar.h>
#include "impl/strlen.h"

#if defined(_MSC_VER) && !(defined (__GNUG__) || defined (__clang__))
#pragma function (strlen)
#pragma function (wcslen)
#endif

extern "C"
size_t strlen (const char* s)
{
	return CRTL::strlen (s);
}

extern "C"
size_t wcslen (const wchar_t* s)
{
	return CRTL::strlen (s);
}

extern "C"
size_t strnlen (const char* s, size_t maxlen)
{
	return CRTL::strnlen (s, maxlen);
}

extern "C"
size_t wcsnlen (const wchar_t* s, size_t maxlen)
{
	return CRTL::strnlen (s, maxlen);
}
