// Nirvana project.
// For algorithms see: http://www.hackersdelight.org/

#include "../Include/nlzntz.h"

namespace Nirvana {

unsigned int NlzUnrolled::nlz (uint32_t x)
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

unsigned int NtzUnrolled::ntz (UWord x)
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

uint32_t flp2 (uint32_t x)
{
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x - (x >> 1);
}

uint64_t flp2 (uint64_t x)
{
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	x = x | (x >> 32);
	return x - (x >> 1);
}

uint32_t clp2 (uint32_t x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	return x + 1;
}

uint64_t clp2 (uint64_t x)
{
	x = x - 1;
	x = x | (x >> 1);
	x = x | (x >> 2);
	x = x | (x >> 4);
	x = x | (x >> 8);
	x = x | (x >> 16);
	x = x | (x >> 32);
	return x + 1;
}

}
