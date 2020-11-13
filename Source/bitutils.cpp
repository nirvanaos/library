// Nirvana project.
// Bit utilities.
// For algorithms see: https://github.com/hcs0/Hackers-Delight

#include <Nirvana/bitutils.h>

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
	n += m;
	x <<= m;

	y = x - 0x1000;      // If positions 12-15 are 0,
	m = (y >> 16) & 4;   // add 4 to n and shift x left 4.
	n += m;
	x <<= m;

	y = x - 0x4000;      // If positions 14-15 are 0,
	m = (y >> 16) & 2;   // add 2 to n and shift x left 2.
	n += m;
	x <<= m;

	y = x >> 14;         // Set y = 0, 1, 2, or 3.
	m = y & ~(y >> 1);   // Set m = 0, 1, 2, or 2 resp.
	return n + 2 - m;
}

unsigned int NlzUnrolled::nlz (uint16_t x)
{
	int16_t y, m, n;

	y = x - 0x100;       // If positions 8-15 are 0,
	n = (y >> 16) & 8;   // add 8 to n and shift x left 8.
	x <<= n;

	y = x - 0x1000;      // If positions 12-15 are 0,
	m = (y >> 16) & 4;   // add 4 to n and shift x left 4.
	n += m;
	x <<= m;

	y = x - 0x4000;      // If positions 14-15 are 0,
	m = (y >> 16) & 2;   // add 2 to n and shift x left 2.
	n += m;
	x <<= m;

	y = x >> 14;         // Set y = 0, 1, 2, or 3.
	m = y & ~(y >> 1);   // Set m = 0, 1, 2, or 2 resp.
	return n + 2 - m;
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
