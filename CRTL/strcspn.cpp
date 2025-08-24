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
#include "impl/Find.h"

namespace CRTL {

template <typename C> inline static
size_t strcspn (const C* s1, const C* s2)
{
	const C* s = s1;

	for (;;) {
		C c = *s1;
		if (!c ||	c == *Find::find (s2, std::numeric_limits <size_t>::max (), c, true))
			break;
		s1++;
	}

	return s1 - s;
}

}

extern "C" {
	
size_t strcspn (const char* s1, const char* s2)
{
	return CRTL::strcspn (s1, s2);
}

size_t wcscspn (const wchar_t* s1, const wchar_t* s2)
{
	return CRTL::strcspn (s1, s2);
}

}

