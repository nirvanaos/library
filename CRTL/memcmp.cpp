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
#include <Nirvana/platform.h>

namespace CRTL {

using Nirvana::UWord;
using Nirvana::unaligned;

template <typename C> inline
int memcmp (const C* ls, const C* rs, size_t count)
{
	/* If s1 or s2 are unaligned, then compare bytes. */
	if (!unaligned (ls) && !unaligned (rs)) {
		/* If s1 and s2 are word-aligned, compare them a word at a time. */
		size_t word_cnt = count * sizeof (C) / sizeof (UWord);
		if (word_cnt) {
			const UWord* lw = (const UWord*)ls;
			const UWord* rw = (const UWord*)rs;
			do {
				UWord l = *lw;
				UWord r = *rw;
				if (l == r) {
					++lw;
					++rw;
				} else
					break;
			} while (--word_cnt);
			ls = (const C*)lw;
			rs = (const C*)rw;
			count -= word_cnt * sizeof (UWord) / sizeof (C);
		}
	}

	while (count && *ls == *rs) {
		++ls;
		++rs;
		--count;
	}
	return count ? ((unsigned)*ls - (unsigned)*rs) : 0;
}

}

#if !defined (_MSC_VER) && !defined (__clang__) && !defined (__GNUG__)

extern "C"
int memcmp (const void* ls, const void* rs, size_t count)
{
	return CRTL::memcmp ((const uint8_t*)ls, (const uint8_t*)rs, count);
}

#endif

extern "C"
int wmemcmp (const wchar_t* ls, const wchar_t* rs, size_t count)
{
	return CRTL::memcmp (ls, rs, count);
}
