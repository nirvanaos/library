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
#include <strings.h>
#include <wctype.h>
#include <limits>
#include "impl/UTF8In.h"

extern "C" int strncasecmp (const char* s1, const char* s2, size_t n)
{
	CRTL::UTF8In in1 (s1, n), in2 (s2, n);
	for (;;) {
		wint_t wc1 = in1.get (), wc2 = in2.get ();
		if (wc1 && wc2) {
			wc1 = towlower (wc1);
			wc2 = towlower (wc2);
			if (wc1 < wc2)
				return -1;
			else if (wc1 > wc2)
				return 1;
		} else if (wc2)
			return -1;
		else if (wc1)
			return 1;
		else
			return 0;
	}
}

extern "C" int strcasecmp (const char* s1, const char* s2)
{
	return strncasecmp (s1, s2, std::numeric_limits <size_t>::max ());
}
