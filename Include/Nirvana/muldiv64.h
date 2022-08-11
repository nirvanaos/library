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

#if _INTEGRAL_MAX_BITS >= 128

inline uint64_t muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator)
{
	return (uint64_t)((uint128_t)number * (uint128_t)numerator / denominator);
}

#elif (defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)))\
&& (defined (__x86_64__) || defined (__powerpc64__) || defined (__aarch64__))

inline uint64_t muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator)
{
	return (uint64_t)((unsigned __int128_t)number * (unsigned __int128_t)numerator / denominator);
}

#elif (defined (__clang_major__) && (__clang_major__ > 3 || (__clang_major__ == 3 && __clang_minor__ >= 1)))\
&& (defined (__x86_64__) || defined (__powerpc64__) || defined (__aarch64__))

inline uint64_t muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator)
{
	return (uint64_t)((__uint128_t)number * (__uint128_t)numerator / denominator);
}

#elif _MSC_VER > 1900\
&& (defined (__x86_64__) || defined (__powerpc64__) || defined (__aarch64__))

inline uint64_t muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator)
{
	uint64_t h, l = _umul128 (number, numerator, &h);
	uint64_t r;
	return _udiv128 (h, l, denominator, &r);
}

#else

extern uint64_t _muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator);

inline uint64_t muldiv64 (uint64_t number, uint64_t numerator, uint64_t denominator)
{
	return _muldiv64 (number, numerator, denominator);
}

#endif

}

#endif
