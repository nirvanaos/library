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
#ifndef CRTL_IMPL_FIND_H_
#define CRTL_IMPL_FIND_H_
#pragma once

#include "strutl.h"

namespace CRTL {

class Find
{
public:
	template <typename C>
	static const C* find (const C* p, size_t maxlen, int cfind, bool zero_term) noexcept;

private:
	template <size_t char_size>
	static UWord detect_char (UWord w, UWord mask) noexcept
	{
		return detect_null <char_size> (w ^ mask);
	}

	static int stop (int c, int cfind, int zeroterm) noexcept
	{
		return is_null (c ^ cfind) | (zeroterm & is_null (c));
	}
};

template <typename C>
#if (defined (__GNUG__) || defined (__clang__))
__attribute__ ((optnone)) // Prevent recursion
#endif
const C* Find::find (const C* p, size_t maxlen, int cfind, bool zero_term) noexcept
{
	const C* end = get_end (p, maxlen);

	int ztc = zero_term ? ~0 : 0;

	if (sizeof (UWord) > sizeof (C)) {
		const UWord* aligned = (const UWord*)Nirvana::round_up (p, sizeof (UWord));
		const UWord* aligned_end = (const UWord*)Nirvana::round_down (end, sizeof (UWord));
		if (aligned < aligned_end) {
			while (p < (const C*)aligned) {
				if (stop (*p, cfind, ztc))
					return p;
				++p;
			}

			UWord mask = make_mask ((C)cfind);
			UWord ztw = zero_term ? ~(UWord)0 : 0;

			do {
				UWord w = *aligned;
				if ((mask & detect_char <sizeof (C)> (w, mask)) | (ztw & detect_null <sizeof (C)> (w)))
					break;
				aligned++;
			} while (aligned != aligned_end);

			// The block of bytes currently pointed to by aligned
			// contains either a null or the target char, or both.  We
			// catch it using the bytewise search.

			p = (const C *)aligned;
		}
	}

	while (p < end && !stop (*p, cfind, ztc))
		++p;

	return p;
}

}

#endif
