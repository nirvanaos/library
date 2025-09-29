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
#include <string.h>
#include <wchar.h>
#include "impl/strutl.h"

namespace CRTL {

template <typename C>
#if (defined (__GNUG__) || defined (__clang__))
__attribute__ ((no_builtin)) // Prevent recursion
#endif
inline static C* memset (C* dst, int c, size_t count) noexcept
{
	C* p = dst;
	C* end = p + count;
	if (sizeof (UWord) > sizeof (C)) {
		UWord* aligned = (UWord*)Nirvana::round_up (p, sizeof (UWord));
		UWord* aligned_end = (UWord*)Nirvana::round_down (end, sizeof (UWord));
		if (aligned < aligned_end) {
			while (p < (C*)aligned) {
				*(p++) = c;
			}

			UWord mask = make_mask ((C)c);
			do {
				*(aligned++) = mask;
			} while (aligned < aligned_end);

			p = (C*)aligned;
		}
	}

	while (p < end) {
		*(p++) = c;
	}

	return dst;
}

}

#if defined (_MSC_VER) && !defined (__clang__)
#pragma function (memset)
#endif

extern "C" {

void* memset (void* dst, int c, size_t count)
{
	return CRTL::memset ((char*)dst, c, count);
}

wchar_t* wmemset (wchar_t* dst, wchar_t c, size_t count)
{
	return CRTL::memset (dst, c, count);
}

}
