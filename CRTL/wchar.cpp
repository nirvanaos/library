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
#include <Nirvana/mbstate_utf8.h>

extern "C" int mblen (const char* s, size_t n)
{
	if (!s)
		return 0;
	int c = *s;
	if (!c)
		return 0;
	int ocnt = Nirvana::octet_cnt (c);
	if (ocnt <= 0)
		return -1;
	return ocnt;
}

extern "C" int mbsinit (const __Mbstate* ps)
{
	return (ps && ps->__octets) ? 0 : 1;
}

extern "C" size_t mbrtowc (wchar_t* pwc, const char* s, size_t n, __Mbstate* ps)
{

}
