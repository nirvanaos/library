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
#include "impl/strlen.h"

#if defined(_MSC_VER) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(strcmp)
#pragma function(wcscmp)
#endif

namespace CRTL {

using Nirvana::UWord;

template <typename C> inline
int strcmp (const C* ls, const C* rs)
{
	/* If s1 or s2 are unaligned, then compare bytes. */
	if (sizeof (UWord) > sizeof (C) && !unaligned (ls) && !unaligned (rs)) {
		/* If s1 and s2 are word-aligned, compare them a word at a time. */
		const UWord* lw = (const UWord*)ls;
		const UWord* rw = (const UWord*)rs;
		for (;;) {
			UWord l = *lw;
			UWord r = *rw;
			if (l == r) {
				/* To get here, *lw == *rw, thus if we find a null in *lw,
				then the strings must be equal, so return zero.  */
				if (detect_null <sizeof (C)> (l))
					return 0;

				++lw;
				++rw;
			} else
				break;
		}

		/* A difference was detected in last few bytes of ls, so search bytewise */
		ls = (const C*)lw;
		rs = (const C*)rw;
	}

	while (*ls != '\0' && *ls == *rs) {
		++ls;
		++rs;
	}
	return ((unsigned)*ls - (unsigned)*rs);
}

}

extern "C"
int strcmp (const char* ls, const char* rs)
{
	return CRTL::strcmp (ls, rs);
}

extern "C"
int wcscmp (const wchar_t* ls, const wchar_t* rs)
{
	return CRTL::strcmp (ls, rs);
}
