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
#include <limits>
#include "impl/strcat.h"

extern "C" {

char* strcat (char* dst, const char* src)
{
	CRTL::strcat (dst, std::numeric_limits <size_t>::max (), src,
		std::numeric_limits <size_t>::max ());
	return dst;
}

wchar_t* wcscat (wchar_t* dst, const wchar_t* src)
{
	CRTL::strcat (dst, std::numeric_limits <size_t>::max (), src,
		std::numeric_limits <size_t>::max ());
	return dst;
}

errno_t strcat_s (char* dst, rsize_t dst_size, const char* src)
{
	return CRTL::strcat (dst, dst_size, src, std::numeric_limits <size_t>::max ());
}

errno_t wcscat_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return CRTL::strcat (dst, dst_size, src, std::numeric_limits <size_t>::max ());
}

char* strncat (char* dst, const char* src, size_t count)
{
	CRTL::strcat (dst, std::numeric_limits <size_t>::max (), src, count);
	return dst;
}

wchar_t* wcsncat (wchar_t* dst, const wchar_t* src, size_t count)
{
	CRTL::strcat (dst, std::numeric_limits <size_t>::max (), src, count);
	return dst;
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
