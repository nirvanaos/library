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
#include <string.h>
#include <wchar.h>
#include "impl/strchr.h"

extern "C" char* strchr (const char* s, int c)
{
	return CRTL::strchr (s, (char)c);
}

extern "C" wchar_t *wcschr (const wchar_t* s, wchar_t c)
{
	return CRTL::strchr (s, c);
}

extern "C" void* memchr (const void* p, int c, size_t count)
{
	return CRTL::memchr ((const char*)p, (char)c, count);
}

extern "C" wchar_t* wmemchr (const wchar_t* p, wchar_t c, size_t count)
{
	return CRTL::memchr (p, c, count);
}
