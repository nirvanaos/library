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

// TODO: A temporary solution for the string collation. We need a complete solution.

#include <string.h>
#include <wchar.h>
#include "impl/UTF8In.h"

extern "C" int strcoll (const char* s1, const char* s2)
{
	CRTL::UTF8In in1 (s1), in2 (s2);
	for (;;) {
		wint_t wc1 = in1.get (), wc2 = in2.get ();
		if (wc1 && wc2) {
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

extern "C" int strcoll_l (const char* s1, const char* s2, locale_t)
{
	return strcoll (s1, s2);
}

extern "C" size_t strxfrm (char* dest, const char* src, size_t count)
{
	CRTL::UTF8In utf (src);
	size_t rcnt = 0;
	for (wint_t wc; (wc = utf.get ());) {
		rcnt += 2;
		if (rcnt < count && dest) {
			*(dest++) = (char)(wc >> 8);
			*(dest++) = (char)wc;
		}
	}
	if (rcnt < count && dest)
		*dest = 0;
	return rcnt;
}

extern "C" size_t strxfrm_l (char* dest, const char* src, size_t count, locale_t)
{
	return strxfrm (dest, src, count);
}

extern "C" int wcscoll (const wchar_t* s1, const wchar_t* s2)
{
	return wcscmp (s1, s2);
}

extern "C" int wcscoll_l (const wchar_t* s1, const wchar_t* s2, locale_t)
{
	return wcscmp (s1, s2);
}

extern "C" size_t wcsxfrm (wchar_t* dest, const wchar_t* src, size_t count)
{
	size_t rcnt = 0;
	for (wint_t wc; (wc = *(src++));) {
		++rcnt;
		if (rcnt < count && dest)
			*(dest++) = (wchar_t)wc;
	}
	if (rcnt < count && dest)
		*dest = 0;
	return rcnt;
}

extern "C" size_t wcsxfrm_l (wchar_t* dest, const wchar_t* src, size_t count, locale_t)
{
	return wcsxfrm (dest, src, count);
}
