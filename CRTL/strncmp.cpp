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
#include <CRTL/impl/strlen.h>

namespace CRTL {

template <typename C> inline
int strncmp (const C* ls, const C* rs, size_t n)
{
	if (n == 0)
		return 0;

	/* If s1 or s2 are unaligned, then compare bytes. */
	if (sizeof (Word) > sizeof (C) && !unaligned (ls) && !unaligned (rs)) {
		/* If s1 and s2 are word-aligned, compare them a word at a time. */
		const Word* lw = (const Word*)ls;
		const Word* rw = (const Word*)rs;
		while (n >= sizeof (Word) && *lw == *rw) {
			n -= sizeof (Word);

			/* If we've run out of bytes or hit a null, return zero
			since we already know *a1 == *a2.  */
			if (n == 0 || detect_null <sizeof (C)> (*lw))
				return 0;

			++lw;
			++rw;
		}

		/* A difference was detected in last few bytes of s1, so search bytewise */
		ls = (const C*)lw;
		rs = (const C*)rw;
	}

	while (n-- > 0 && *ls == *rs) {
		/* If we've run out of bytes or hit a null, return zero
		since we already know *s1 == *s2.  */
		if (n == 0 || *ls == '\0')
			return 0;
		++ls;
		++rs;
	}
	return ((unsigned)*ls - (unsigned)*rs);
}

}

extern "C"
int strncmp (const char* ls, const char* rs, size_t n)
{
	return CRTL::strncmp (ls, rs, n);
}

extern "C"
int wcsncmp (const wchar_t* ls, const wchar_t* rs, size_t n)
{
	return CRTL::strncmp (ls, rs, n);
}
