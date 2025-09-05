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
#include "impl/strlen.h"
#include "impl/memcpy.h"
#include <limits>

namespace CRTL {

template <typename C> static
errno_t strcpy (C* dst, size_t dst_size, const C* src, size_t count) noexcept
{
	if (!dst)
		return EINVAL;
	if (!src) {
		dst [0] = 0;
		return EINVAL;
	}
	size_t src_size = strnlen (src, std::min (dst_size, count));
	if (dst_size <= src_size) {
		dst [0] = 0;
		return ERANGE;
	}
	if (src_size < count)
		++src_size;
	memcpy (dst, src, src_size);
	return 0;
}

}

extern "C" {

errno_t strcpy_s (char* dst, rsize_t dst_size, const char* src)
{
	return CRTL::strcpy (dst, dst_size, src, std::numeric_limits <size_t>::max ());
}

errno_t wcscpy_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return CRTL::strcpy (dst, dst_size, src, std::numeric_limits <size_t>::max ());
}

#if defined(_MSC_VER) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(strcpy)
#pragma function(wcscpy)
#endif

char* strcpy (char* dst, const char* src)
{
	CRTL::strcpy (dst, std::numeric_limits <size_t>::max (), src, std::numeric_limits <size_t>::max ());
	return dst;
}

wchar_t* wcscpy (wchar_t* dst, const wchar_t* src)
{
	CRTL::strcpy (dst, std::numeric_limits <size_t>::max (), src, std::numeric_limits <size_t>::max ());
	return dst;
}

char* strncpy (char* dst, const char* src, size_t count)
{
	CRTL::strcpy (dst, std::numeric_limits <size_t>::max (), src, count);
	return dst;
}

wchar_t* wcsncpy (wchar_t* dst, const wchar_t* src, size_t count)
{
	CRTL::strcpy (dst, std::numeric_limits <size_t>::max (), src, count);
	return dst;
}

}
