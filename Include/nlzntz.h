// Nirvana project.
// Bit search functions.
// For algorithms see: http://www.hackersdelight.org/

#ifndef NIRVANA_NLZNTZ_H_
#define NIRVANA_NLZNTZ_H_

#include "Nirvana.h"
#include <type_traits>
#include <limits>
#include <stdint.h>

#if defined _M_AMD64
#include <intrin.h>
#endif

namespace Nirvana {

// Number of leading zeros.

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

		static const size_t LE = std::endian::native == std::endian::little ? 1 : 0;
		return 1054 - (as_int [LE] >> 20);
	}
};

struct NlzUnrolled
{
	static unsigned int nlz (uint32_t x)
	{
		int32_t y, m, n;

		y = -(int32_t)(x >> 16);   // If left half of x is 0,
		m = (y >> 16) & 16;  // set n = 16.  If left half
		n = 16 - m;          // is nonzero, set n = 0 and
		x = x >> m;          // shift x right 16.
												 // Now x is of the form 0000xxxx.
		y = x - 0x100;       // If positions 8-15 are 0,
		m = (y >> 16) & 8;   // add 8 to n and shift x left 8.
		n = n + m;
		x = x << m;

		y = x - 0x1000;      // If positions 12-15 are 0,
		m = (y >> 16) & 4;   // add 4 to n and shift x left 4.
		n = n + m;
		x = x << m;

		y = x - 0x4000;      // If positions 14-15 are 0,
		m = (y >> 16) & 2;   // add 2 to n and shift x left 2.
		n = n + m;
		x = x << m;

		y = x >> 14;         // Set y = 0, 1, 2, or 3.
		m = y & ~(y >> 1);   // Set m = 0, 1, 2, or 2 resp.
		return n + 2 - m;
	}
};

inline unsigned int nlz (uint32_t x)
{
	return
		::std::conditional <::std::numeric_limits <double>::is_iec559,
		NlzDoubleIEEE, NlzUnrolled>::type::nlz (x);
}

// Number of trailing zeros.

#if defined _M_AMD64

inline unsigned int ntz (UWord x)
{
	return (unsigned int)_mm_popcnt_u64 (~x & (x - 1));
}

#else

struct NtzNlz32
{
	static unsigned int ntz (uint32_t x)
	{
		return 32 - nlz (~x & (x - 1));
	}
};

struct Ntz
{
	static unsigned int ntz (UWord x)
	{
		if (x == 0) return sizeof (UWord) * 8;
		unsigned int n = 1;
		if (sizeof (UWord) > 4) { // Compile-time if.
			if ((x & 0xFFFFFFFF) == 0)
				n = n + sizeof (UWord) * 4; x = x >> sizeof (UWord) * 4;
		}
		if ((x & 0x0000FFFF) == 0)
			n = n + 16; x = x >> 16;
		if ((x & 0x000000FF) == 0)
			n = n + 8; x = x >> 8;
		if ((x & 0x0000000F) == 0)
			n = n + 4; x = x >> 4;
		if ((x & 0x00000003) == 0)
			n = n + 2; x = x >> 2;
		return n - (x & 1);
	}
};

inline unsigned int ntz (UWord x)
{
	return
		::std::conditional <sizeof (UWord) == 4 && ::std::numeric_limits <double>::is_iec559,
			NtzNlz32, Ntz>::type::ntz (x);
}

#endif

}

#endif
