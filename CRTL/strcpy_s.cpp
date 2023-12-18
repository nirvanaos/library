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
#include <CRTL/impl/strcpy_s.h>

extern "C"
errno_t strcpy_s (char* dst, rsize_t dst_size, const char* src)
{
	return CRTL::strcpy_s (dst, dst_size, src);
}

extern "C"
errno_t wcscpy_s (wchar_t* dst, rsize_t dst_size, const wchar_t* src)
{
	return CRTL::strcpy_s (dst, dst_size, src);
}
