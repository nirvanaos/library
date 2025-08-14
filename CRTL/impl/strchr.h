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

template <typename C> inline
#if (defined (__GNUG__) || defined (__clang__))
__attribute__ ((optnone))
#endif
const C* strend (const C* s)
{
	const C* p = s;
	if (sizeof (UWord) > sizeof (C)) {
		const C* aligned = Nirvana::round_up (p, sizeof (UWord));
		while (p < aligned) {
			if (!*p)
				return p;
			++p;
		}
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
	return p;
}

template <size_t char_size>
inline UWord detect_char (UWord w, UWord mask)
{
	return detect_null <char_size> (w ^ mask);
}

template <typename C> inline
#if (defined (__GNUG__) || defined (__clang__))
__attribute__ ((optnone))
#endif
const C* strfind (const C* s, const C cf)
{
  // Special case for finding 0.
  if (!cf)
		return strend (s);

	const C* p = s;
	
  // All other cases.  Align the pointer, then search a long at a time.
	if (sizeof (UWord) > sizeof (C)) {
		const C* aligned = Nirvana::round_up (p, sizeof (UWord));
		while (p < aligned) {
			C c = *p;
			if (c == cf || !c)
				return p;
			++p;
		}

		UWord mask = *p;
		for (unsigned j = sizeof (C) * 8; j < sizeof (UWord) * 8; j <<= 1)
			mask |= mask << j;

		for (;;) {
			UWord w = *aligned;
		 	if (detect_null <sizeof (C)> (w) || detect_char <sizeof (C)> (w, mask))
				break;
			aligned++;
		}

		// The block of bytes currently pointed to by aligned
    // contains either a null or the target char, or both.  We
    // catch it using the bytewise search.

   	p = (const C *)aligned;
	}

	for (;;) {
		C c = *p;
		if (c == cf || !c)
			break;
	}

  return p;
}

template <typename C> inline
C* strchr (const C* s, const C cf)
{
	const C* pf = strfind (s, cf);
	if (*pf == cf)
		return const_cast <C*> (pf);
	else
		return nullptr;
}

}

#endif