/// \file
/// \brief Bit utilities.
/// 
/// \see https://github.com/hcs0/Hackers-Delight
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
#ifndef NIRVANA_BITUTILS_H_
#define NIRVANA_BITUTILS_H_
#pragma once

#include "NirvanaBase.h"
#include <type_traits>
#include <limits>
#include <stdint.h>

#ifdef _MSVC_LANG
#include <intrin.h>
#endif

#ifdef __clang__
#include <lzcntintrin.h>
#endif

namespace Nirvana {

/// \brief Number of leading zeros.
/// \tparam U ineger type.
/// \param x integer.
/// \return A number of leading zero bits in `x`.
template <typename U> unsigned int nlz (U x) NIRVANA_NOEXCEPT;

struct NlzDoubleIEEE
{
	static unsigned int nlz (uint32_t x) NIRVANA_NOEXCEPT
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

	static unsigned int nlz (uint64_t x) NIRVANA_NOEXCEPT
	{
		if (x & 0xFFFFFFFF00000000UL)
			return nlz ((uint32_t)(x >> 32));
		else
			return nlz ((uint32_t)x) + 32;
	}
};

struct NlzUnrolled
{
	static unsigned int nlz (uint64_t x) NIRVANA_NOEXCEPT
	{
		if (x & 0xFFFFFFFF00000000UL)
			return nlz ((uint32_t)(x >> 32));
		else
			return nlz ((uint32_t)x) + 32;
	}

	static unsigned int nlz (uint32_t x) NIRVANA_NOEXCEPT;
	static unsigned int nlz (uint16_t x) NIRVANA_NOEXCEPT;
};

#if defined (_M_AMD64) || defined (__amd64)

template <> inline
unsigned int nlz <uint64_t> (uint64_t x) NIRVANA_NOEXCEPT
{
	return (unsigned int)_lzcnt_u64 (x);
}

template <> inline
unsigned int nlz <uint32_t> (uint32_t x) NIRVANA_NOEXCEPT
{
	return (unsigned int)_lzcnt_u32 (x);
}

#else

template <typename U> inline
unsigned int nlz (U x) NIRVANA_NOEXCEPT
{
	return ::std::conditional_t <::std::numeric_limits <double>::is_iec559,
		NlzDoubleIEEE, NlzUnrolled>::nlz (x);
}

#endif

/// \brief Number of trailing zeros.
/// \tparam U Unsigned integer type.
/// \param x integer.
/// \return A number of trailing zero bits in `x`.
template <typename U> unsigned int ntz (U x) NIRVANA_NOEXCEPT;

struct NtzNlz
{
	template <typename U>
	static unsigned int ntz (U x) NIRVANA_NOEXCEPT
	{
		return sizeof (U) * 8 - nlz (~x & (x - 1));
	}
};

struct NtzUnrolled
{
	template <typename U>
	static unsigned int ntz (U x) NIRVANA_NOEXCEPT;
};

template <typename U>
unsigned int NtzUnrolled::ntz (U x) NIRVANA_NOEXCEPT
{
	static_assert (sizeof (U) == 8 || sizeof (U) == 4 || sizeof (U) == 2, "Unsipported integral type.");

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
unsigned int ntz <uint64_t> (uint64_t x) NIRVANA_NOEXCEPT
{
	return (unsigned int)__popcnt64 (~x & (x - 1));
}

template <> inline
unsigned int ntz <uint32_t> (uint32_t x) NIRVANA_NOEXCEPT
{
	return (unsigned int)__popcnt (~x & (x - 1));
}

#else

template <typename U> inline
unsigned int ntz (U x) NIRVANA_NOEXCEPT
{
	return ::std::conditional_t <::std::numeric_limits <double>::is_iec559,
		NtzNlz, NtzUnrolled>::ntz (x);
}

#endif

/// floor(log2(n))
/// 
/// \param n A number.
/// \returns floor(log2(n)).
template <typename U> inline
unsigned ilog2_floor (U u) NIRVANA_NOEXCEPT
{
	assert (u != 0);
	return sizeof (U) * 8 - 1 - nlz (u);
}

/// ceil(log2(n))
/// 
/// \param n A number
/// \returns ceil(log2(n))
template <typename U> inline
unsigned ilog2_ceil (U u) NIRVANA_NOEXCEPT
{
	assert (u != 0);
	return sizeof (U) * 8 - nlz (u - 1);
}

/// constant ceil(log2(n))
/// 
/// This function is recursive and slow.
/// But it can be used in constant expressions.
/// 
/// \param n A number.
/// \returns ceil(log2(n))
constexpr unsigned log2_ceil (size_t n) NIRVANA_NOEXCEPT
{
	return (n > 1) ? 1 + log2_ceil ((n + 1) / 2) : 0;
}

/// \fn uint32_t flp2(uint32_t x)
/// \fn uint64_t flp2(uint64_t x)
/// \brief Round down to a power of 2.

uint32_t flp2 (uint32_t x) NIRVANA_NOEXCEPT;
uint64_t flp2 (uint64_t x) NIRVANA_NOEXCEPT;

/// \fn uint32_t clp2(uint32_t x)
/// \fn uint64_t clp2(uint64_t x)
/// \brief Round up to a power of 2.

uint32_t clp2 (uint32_t x) NIRVANA_NOEXCEPT;
uint64_t clp2 (uint64_t x) NIRVANA_NOEXCEPT;

/// Swap bytes in 16-bit value.
inline uint16_t byteswap (const uint16_t& x) NIRVANA_NOEXCEPT
{
#ifdef _MSVC_LANG
	return _byteswap_ushort (x);
#elif defined (__GNUG__) || defined (__clang__)
	return __builtin_bswap16 (x);
#else
  return ((uint16_t) ((((x) >> 8) & 0xff) | (((x) & 0xff) << 8)))
#endif
}

/// Swap bytes in 32-bit value.
inline uint32_t byteswap (const uint32_t& x) NIRVANA_NOEXCEPT
{
#ifdef _MSVC_LANG
	return _byteswap_ulong (x);
#elif defined (__GNUG__) || defined (__clang__)
	return __builtin_bswap32 (x);
#else
	return ((((x) & 0xff000000u) >> 24) | (((x) & 0x00ff0000u) >> 8)
		| (((x) & 0x0000ff00u) << 8) | (((x) & 0x000000ffu) << 24));
#endif
}

/// Swap bytes in 64-bit value.
inline uint64_t byteswap (const uint64_t& x) NIRVANA_NOEXCEPT
{
#ifdef _MSVC_LANG
	return _byteswap_uint64 (x);
#elif defined (__GNUG__) || defined (__clang__)
	return __builtin_bswap64 (x);
#else
	return ((((x) & 0xff00000000000000ull) >> 56)
		| (((x) & 0x00ff000000000000ull) >> 40)
		| (((x) & 0x0000ff0000000000ull) >> 24)
		| (((x) & 0x000000ff00000000ull) >> 8)
		| (((x) & 0x00000000ff000000ull) << 8)
		| (((x) & 0x0000000000ff0000ull) << 24)
		| (((x) & 0x000000000000ff00ull) << 40)
		| (((x) & 0x00000000000000ffull) << 56));
#endif
}

}

#endif
