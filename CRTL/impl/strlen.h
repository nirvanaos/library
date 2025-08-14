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
#ifndef CRTL_IMPL_STRLEN_H_
#define CRTL_IMPL_STRLEN_H_
#pragma once

#include "strchr.h"

namespace CRTL {

template <typename C> inline
size_t strlen (const C* s)
{
	return strend (s) - s;
}

template <typename C> inline
size_t strnlen (const C* s, size_t maxlen)
{
	const C* p = s;
	const C* end = (const C*)UINTPTR_MAX;
	if ((size_t)(end - p) > maxlen)
		end = p + maxlen;
	if (sizeof (UWord) > sizeof (C)) {
		const C* aligned_begin = Nirvana::round_up (p, sizeof (UWord));
		const C* aligned_end = Nirvana::round_down (end, sizeof (UWord));
		if (aligned_begin < aligned_end) {
			while (p < aligned_begin) {
				if (!*p)
					return p - s;
				++p;
			}
			/* If the string is word-aligned, we can check for the presence of
			a null in each word-sized block.  */
			const UWord* wp = (const UWord*)p;
			while ((const C*)wp < aligned_end && !detect_null <sizeof (C)> (*wp)) {
				++wp;
			}
			/* Once a null is detected, we check each byte in that block for a
			precise position of the null.  */
			p = (const C*)wp;
		}
	}
	while (p < end && *p)
		++p;
	return p - s;
}

}

#endif
