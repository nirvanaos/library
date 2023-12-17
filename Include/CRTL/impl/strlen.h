/// \file
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
#ifndef NIRVANA_STRLEN_H_
#define NIRVANA_STRLEN_H_

#include <stdint.h>

namespace CRTL {

/// The machine word
typedef size_t Word;

/// \returns Nonzero if w contains a null character
template <size_t char_size>
Word detect_null (Word w);

template <>
inline Word detect_null <1> (Word w)
{
	return ((w - (Word)0x0101010101010101ull) & ~w & (Word)0x8080808080808080ull);
}

template <>
inline Word detect_null <2> (Word w)
{
	return ((w - (Word)0x0001000100010001ull) & ~w & (Word)0x8000800080008000ull);
}

template <>
inline Word detect_null <4> (Word w)
{
	return ((w - 0x0001000100010001ull) & ~w & 0x8000800080008000ull);
}

inline uintptr_t unaligned (const void* p)
{
	return (uintptr_t)p & (sizeof (Word) - 1);
}

template <typename C> inline
size_t strlen (const C* s)
{
	const C* p = s;
	if (sizeof (Word) > sizeof (C) && !unaligned (p)) {
		/* If the string is word-aligned, we can check for the presence of
		 a null in each word-sized block.  */
		const Word* wp = (const Word*)p;
		while (!detect_null <sizeof (C)> (*wp))
			++wp;
		/* Once a null is detected, we check each byte in that block for a
		 precise position of the null.  */
		p = (const C*)wp;
	}
	while (*p)
		++p;
	return p - s;
}

template <typename C> inline
size_t strnlen (const C* s, size_t maxlen)
{
	const C* p = s;
	const C* end = (const C*)UINTPTR_MAX;
	if ((size_t)(end - p) > maxlen)
		end = p + maxlen;
	if (sizeof (Word) > sizeof (C) && !unaligned (p)) {
		/* If the string is word-aligned, we can check for the presence of
		 a null in each word-sized block.  */
		const Word* wp = (const Word*)p;
		while ((const C*)wp <= end && !detect_null <sizeof (C)> (*wp)) {
			++wp;
		}
		/* Once a null is detected, we check each byte in that block for a
		 precise position of the null.  */
		p = (const C*)wp;
	}
	while (p <= end && *p)
		++p;
	return p - s;
}

}

#endif
