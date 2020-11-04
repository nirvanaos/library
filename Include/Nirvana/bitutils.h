// Nirvana project.
// Bit utilities.
// For algorithms see: https://github.com/hcs0/Hackers-Delight

#ifndef NIRVANA_BITUTILS_H_
#define NIRVANA_BITUTILS_H_

#include "NirvanaBase.h"
#include <type_traits>
#include <limits>
#include <stdint.h>

#if defined _M_AMD64
#include <intrin.h>
#endif

namespace Nirvana {

/// \fn unsigned int nlz(uint32_t x)
/// \brief Number of leading zeros.
/// \param x 32-bit integer.
/// \return A number of leading zero bits in `x`.

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
};

struct NlzUnrolled
{
	static unsigned int nlz (uint32_t x);
};

inline unsigned int nlz (uint32_t x)
{
	return
		::std::conditional <::std::numeric_limits <double>::is_iec559,
		NlzDoubleIEEE, NlzUnrolled>::type::nlz (x);
}

/// \fn unsigned int ntz(UWord x)
/// \brief Number of trailing zeros.
/// \param x Machine-word integer.
/// \return A number of trailing zero bits in `x`.

struct NtzNlz32
{
	static unsigned int ntz (uint32_t x)
	{
		return 32 - nlz (~x & (x - 1));
	}
};

struct NtzUnrolled
{
	static unsigned int ntz (UWord x);
};

#if defined _M_AMD64

inline unsigned int ntz (UWord x)
{
	return (unsigned int)__popcnt64 (~x & (x - 1));
}

#else

inline unsigned int ntz (UWord x)
{
	return
		::std::conditional <sizeof (UWord) == 4 && ::std::numeric_limits <double>::is_iec559,
			NtzNlz32, NtzUnrolled>::type::ntz (x);
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
