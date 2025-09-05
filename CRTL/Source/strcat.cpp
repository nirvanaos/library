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
#include <limits>
#include "impl/strlen.h"
#include "impl/memcpy.h"

#if defined(_MSC_VER)
#pragma function (strcat)
#pragma function (wcscat)
#endif

namespace CRTL {

template <typename C> static
errno_t strcat (C* dst, size_t dst_size, const C* src, size_t count)
{
	if (!dst || !src)
		return EINVAL;
	if (dst <= src && src < dst + dst_size)
		return EINVAL;
	size_t dst_len = strnlen (dst, dst_size);
	if (dst_len >= dst_size)
		return ERANGE;
	size_t dst_max = dst_size - dst_len;
	size_t src_len = strnlen (src, count);
	if (src_len + 1 >= dst_max)
		return ERANGE;
	dst += dst_len;
	if (src_len < count)
		memcpy (dst, src, src_len + 1);
	else {
		memcpy (dst, src, src_len);
		dst [src_len] = 0;
	}
	return 0;
}

template <typename C> inline
C* strcat (C* dst, const C* src)
{
	strcat (dst, std::numeric_limits <size_t>::max (), src,
		std::numeric_limits <size_t>::max ());
	return dst;
}

template <typename C> inline
errno_t strcat_s (C* dst, rsize_t dst_size, const C* src)
{
	return strcat (dst, dst_size, src, std::numeric_limits <size_t>::max ());
}

template <typename C> inline
C* strncat (C* dst, const C* src, size_t count)
{
	CRTL::strcat (dst, std::numeric_limits <size_t>::max (), src, count);
	return dst;
}

}

extern "C" {

char* strcat (char* dst, const char* src)
{
	return CRTL::strcat (dst, src);
}

wchar_t* wcscat (wchar_t* dst, const wchar_t* src)
{
	return CRTL::strcat (dst, src);
}

errno_t strcat_s (char* dst, rsize_t dst_size, const char* src)
{
	return CRTL::strcat_s (dst, dst_size, src);
}

errno_t wcscat_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return CRTL::strcat_s (dst, dst_size, src);
}

char* strncat (char* dst, const char* src, size_t count)
{
	return CRTL::strncat (dst, src, count);
}

wchar_t* wcsncat (wchar_t* dst, const wchar_t* src, size_t count)
{
	return CRTL::strncat (dst, src, count);
}

errno_t strncat_s (char* dst, rsize_t dst_size, const char* src, rsize_t count )
{
	return CRTL::strcat (dst, dst_size, src, count);
}

errno_t wcsncat_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src, rsize_t count )
{
	return CRTL::strcat (dst, dst_size, src, count);
}

}
