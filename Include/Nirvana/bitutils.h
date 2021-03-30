/// \file
/// \brief Bit utilities.
/// 
/// For algorithms see: https://github.com/hcs0/Hackers-Delight

/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#ifndef NIRVANA_BITUTILS_H_
#define NIRVANA_BITUTILS_H_

#include "NirvanaBase.h"
#include <type_traits>
#include <limits>
#include <stdint.h>

#if defined _M_AMD64
#include <intrin.h>
#if defined (__clang__)
#include <lzcntintrin.h>
#endif
#endif

namespace Nirvana {

/// \brief Number of leading zeros.
/// \tparam U ineger type.
/// \param x integer.
/// \return A number of leading zero bits in `x`.
template <typename U> unsigned int nlz (U x);

struct NlzDoubleIEEE
{
	static unsigned int nlz (uint32_t x)
	{
		union
		{
			uint32_t as_int [2];
			double as_double;
		};

		as_double = (double)x + 0.5;

		static const size_t LE = endian::native == endian::little ? 1 : 0;
		return 1054 - (as_int [LE] >> 20);
	}

	static unsigned int nlz (uint64_t x)
	{
		if (x & 0xFFFFFFFF00000000UL)
			return nlz ((uint32_t)(x >> 32));
		else
			return nlz ((uint32_t)x) + 32;
	}
};

struct NlzUnrolled
{
	static unsigned int nlz (uint64_t x)
	{
		if (x & 0xFFFFFFFF00000000UL)
			return nlz ((uint32_t)(x >> 32));
		else
			return nlz ((uint32_t)x) + 32;
	}

	static unsigned int nlz (uint32_t x);
	static unsigned int nlz (uint16_t x);
};

#if defined (_M_AMD64) || defined (__amd64)

template <> inline
unsigned int nlz <uint64_t> (uint64_t x)
{
	return (unsigned int)_lzcnt_u64 (x);
}

template <> inline
unsigned int nlz <uint32_t> (uint32_t x)
{
	return (unsigned int)_lzcnt_u32 (x);
}

#else

template <typename U> inline
unsigned int nlz (U x)
{
	return ::std::conditional_t <::std::numeric_limits <double>::is_iec559,
		NlzDoubleIEEE, NlzUnrolled>::nlz (x);
}

#endif

/// \brief Number of trailing zeros.
/// \tparam U Unsigned integer type.
/// \param x integer.
/// \return A number of trailing zero bits in `x`.
template <typename U> unsigned int ntz (U x);

struct NtzNlz
{
	template <typename U>
	static unsigned int ntz (U x)
	{
		return sizeof (U) * 8 - nlz (~x & (x - 1));
	}
};

struct NtzUnrolled
{
	template <typename U>
	static unsigned int ntz (U x);
};

template <typename U>
unsigned int NtzUnrolled::ntz (U x)
{
	static_assert (sizeof (U) == 64 || sizeof (U) == 32 || sizeof (U) == 16, "Unsipported integral type.");

	if (x == 0) return sizeof (U) * 8;
	unsigned int n = 1;
	if (sizeof (U) == 8) {
		if ((x & 0xFFFFFFFF) == 0) {
			n += 32;
			x >>= 32;
		}
	}
	if (sizeof (U) >= 4) {
		if ((x & 0x0000FFFF) == 0) {
			n += 16;
			x >>= 16;
		}
	}
	if ((x & 0x00FF) == 0) {
		n += 8;
		x >>= 8;
}
	if ((x & 0x000F) == 0) {
		n += 4;
		x >>= 4;
	}
	if ((x & 0x0003) == 0) {
		n += 2;
		x >>= 2;
	}
	return n - (unsigned int)(x & 1);
}

#if defined (_M_AMD64) || defined (__amd64)

template <> inline
unsigned int ntz <uint64_t> (uint64_t x)
{
	return (unsigned int)__popcnt64 (~x & (x - 1));
}

template <> inline
unsigned int ntz <uint32_t> (uint32_t x)
{
	return (unsigned int)__popcnt (~x & (x - 1));
}

#else

template <typename U> inline
unsigned int ntz (U x)
{
	return ::std::conditional_t <::std::numeric_limits <double>::is_iec559,
		NtzNlz, NtzUnrolled>::ntz (x);
}

#endif

/// \fn uint32_t flp2(uint32_t x)
/// \fn uint64_t flp2(uint64_t x)
/// \brief Round down to a power of 2.

uint32_t flp2 (uint32_t x);
uint64_t flp2 (uint64_t x);

/// \fn uint32_t clp2(uint32_t x)
/// \fn uint64_t clp2(uint64_t x)
/// \brief Round up to a power of 2.

uint32_t clp2 (uint32_t x);
uint64_t clp2 (uint64_t x);

}

#endif
