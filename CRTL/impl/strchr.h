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
#ifndef CRTL_IMPL_STRCHR_H_
#define CRTL_IMPL_STRCHR_H_
#pragma once

#include <limits>
#include <Nirvana/platform.h>
#include <Nirvana/bitutils.h>

namespace CRTL {

using Nirvana::UWord;

/// \returns Nonzero if w contains a null character
template <size_t char_size>
UWord detect_null (UWord w);

template <>
inline UWord detect_null <1> (UWord w)
{
	return ((w - (UWord)0x0101010101010101ull) & ~w & (UWord)0x8080808080808080ull);
}

template <>
inline UWord detect_null <2> (UWord w)
{
	return ((w - (UWord)0x0001000100010001ull) & ~w & (UWord)0x8000800080008000ull);
}

template <>
inline UWord detect_null <4> (UWord w)
{
	return ((w - 0x0001000100010001ull) & ~w & 0x8000800080008000ull);
}

template <size_t char_size>
inline UWord detect_char (UWord w, UWord mask)
{
	return detect_null <char_size> (w ^ mask);
}

template <typename C>
inline bool match (C c, int cfind, int zero_term)
{
	int i = c;
	return ((cfind & (i ^ cfind)) | (zero_term & ~i));
}

template <typename C>
#if (defined (__GNUG__) || defined (__clang__))
__attribute__ ((optnone)) // Prevent recursion
#endif
const C* strfind (const C* p, size_t maxlen, int cfind, bool zero_term)
{
	const C* end = (const C*)std::numeric_limits <const C*>::max ();
	if ((size_t)(end - p) > maxlen)
		end = p + maxlen;

	int ztc = zero_term ? ~0 : 0;

	if (sizeof (UWord) > sizeof (C)) {
		const C* aligned = Nirvana::round_up (p, sizeof (UWord));
		const C* aligned_end = Nirvana::round_down (end, sizeof (UWord));
		if (aligned < aligned_end) {
			while (p < aligned) {
				if (match (*p, cfind, ztc))
					return p;
				++p;
			}

			UWord mask = cfind;
			for (unsigned j = sizeof (C) * 8; j < sizeof (UWord) * 8; j <<= 1)
				mask |= mask << j;
			UWord ztw = zero_term ? ~0 : 0;

			for (;;) {
				UWord w = *aligned;
				if ((mask & detect_char <sizeof (C)> (w, mask)) | (ztw & detect_null <sizeof (C)> (w)))
					break;
				aligned++;
			}

			// The block of bytes currently pointed to by aligned
			// contains either a null or the target char, or both.  We
			// catch it using the bytewise search.

			p = (const C *)aligned;
		}
	}

	while (p < end && !match (*p, cfind, ztc))
		++p;

  return p;
}

template <typename C> inline
C* strchr (const C* s, int cf)
{
	const C* pf = strfind (s, std::numeric_limits <size_t>::max (), cf, true);
	if (*pf == cf)
		return const_cast <C*> (pf);
	else
		return nullptr;
}

template <typename C> inline
C* memchr (const C* p, int cf, size_t count)
{
	const C* pf = strfind (p, count, cf, false);
	if (pf != (p + count))
		return const_cast <C*> (pf);
	else
		return nullptr;
}

}

#endif