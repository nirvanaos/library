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
#include <string.h>
#include <wchar.h>
#include "impl/memcpy.h"

#if defined(_MSC_VER) && !defined (__clang__)
#pragma function (memcpy)
#pragma function (memmove)
#endif

extern "C" {

void* memcpy (void* dst, const void* src, size_t count)
{
	return CRTL::memcpy ((char*)dst, (const char*)src, count);
}

wchar_t* wmemcpy (wchar_t* dst, const wchar_t* src, size_t count)
{
	return CRTL::memcpy (dst, src, count);
}

void* memmove (void* dst, const void* src, size_t count)
{
	return CRTL::memcpy ((char*)dst, (const char*)src, count);
}

wchar_t* wmemmove (wchar_t* dst, const wchar_t* src, size_t count)
{
	return CRTL::memcpy (dst, src, count);
}

}
