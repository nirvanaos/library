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
#ifndef CRTL_IMPL_STRLEN_H_
#define CRTL_IMPL_STRLEN_H_
#pragma once

#include <Nirvana/platform.h>

namespace CRTL {

using Nirvana::UWord;
using Nirvana::unaligned;

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

template <typename C> inline
size_t strlen (const C* s)
{
	const C* p = s;
	if (sizeof (UWord) > sizeof (C) && !unaligned (p)) {
		/* If the string is word-aligned, we can check for the presence of
		 a null in each word-sized block.  */
		const UWord* wp = (const UWord*)p;
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
	if (sizeof (UWord) > sizeof (C) && !unaligned (p)) {
		/* If the string is word-aligned, we can check for the presence of
		 a null in each word-sized block.  */
		const UWord* wp = (const UWord*)p;
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
