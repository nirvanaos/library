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
#ifndef NIRVANA_MULDIV64_H_
#define NIRVANA_MULDIV64_H_
#pragma once

#include <stdint.h>

#ifdef _MSVC_LANG
#include <intrin.h>
#endif

namespace Nirvana {

/// Multiplies two 64-bit values and then divides the 128-bit result by a third 64-bit value.
/// The final result is rounded to the nearest integer.
/// 
/// \param number The multiplicand.
/// \param numerator The multiplier.
/// \param denominator The number by which the result of the multiplication operation is to be divided.
/// \returns The result of the multiplication and division, rounded to the nearest integer.
int64_t muldiv64 (int64_t number, uint64_t numerator, uint64_t denominator);

#if _INTEGRAL_MAX_BITS >= 128

#define NIRVANA_FAST_MULDIV64

inline int64_t muldiv64 (int64_t number, uint64_t numerator, uint64_t denominator)
{
	return (int64_t)((int128_t)number * (int128_t)numerator / denominator);
}

#elif ((defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))\
|| (defined (__clang_major__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 1))))\
&& (defined (__x86_64__) || defined (__powerpc64__) || defined (__aarch64__))

#define NIRVANA_FAST_MULDIV64

inline int64_t muldiv64 (int64_t number, uint64_t numerator, uint64_t denominator)
{
	return (int64_t)((__int128_t)number * (__int128_t)numerator / denominator);
}

#elif _MSC_VER > 1900\
&& (defined (_M_X64) || defined (_M_AMD64) || defined (_M_ARM64))

#define NIRVANA_FAST_MULDIV64

inline int64_t muldiv64 (int64_t number, uint64_t numerator, uint64_t denominator)
{
	int64_t h, l = _mul128 (number, numerator, &h);
	int64_t r;
	return _div128 (h, l, denominator, &r);
}

#else

extern int64_t muldiv64 (int64_t number, uint64_t numerator, uint64_t denominator);

#endif

}

#endif
