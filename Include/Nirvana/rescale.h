/// \file
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_RESCALE_H_
#define NIRVANA_RESCALE_H_
#pragma once

#include "NirvanaBase.h"

#if defined (_MSVC_LANG) && !defined (__clang__)
#include <intrin.h>
#endif

namespace Nirvana {

/// Calculates (a * b + c) / d
inline int32_t rescale32 (int32_t a, uint32_t b, int32_t c, uint32_t d) noexcept
{
	return (int32_t)((int64_t)a * (int64_t)b + c) / d;
}

/// Calculates (a * b + c) / d
int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept;

#if _INTEGRAL_MAX_BITS >= 128

#define NIRVANA_FAST_RESCALE64

inline int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept
{
	return (int64_t)(((int128_t)a * (int128_t)b + c) / d);
}

#elif ((defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))\
|| (defined (__clang_major__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 1))))\
&& (defined (__x86_64__) || defined (__powerpc64__) || defined (__aarch64__))

#define NIRVANA_FAST_RESCALE64

inline int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept
{
	return (int64_t)(((__int128_t)a * (__int128_t)b + c) / d);
}

#else

inline void _add128 (uint64_t& h, uint64_t& l, uint64_t addition) noexcept
{
	uint64_t carry = (((l & addition) & 1) + (l >> 1) + (addition >> 1)) >> 63;
	l += addition;
	h += carry;
}

#if _MSC_VER > 1900\
&& (defined (_M_X64) || defined (_M_AMD64) || defined (_M_ARM64))

#define NIRVANA_FAST_RESCALE64

inline int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept
{
	int64_t h;
	int64_t l = _mul128 (a, b, &h);
	_add128 ((uint64_t&)h, (uint64_t&)l, c);
	int64_t rem;
	return _div128 (h, l, d, &rem);
}

#else

int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept;

#endif
#endif

}

#endif
