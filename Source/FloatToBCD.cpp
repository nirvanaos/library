/*
* Nirvana runtime library.
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
#include "../../pch/pch.h"
#include <Nirvana/FloatToBCD.h>

// From https://github.com/hcs0/Hackers-Delight/blob/master/divmnu.c.txt

/* q[0], r[0], u[0], and v[0] contain the LEAST significant halfwords.
(The sequence is in little-endian order).

This first version is a fairly precise implementation of Knuth's
Algorithm D, for a binary computer with base b = 2**16.  The caller
supplies
   1. Space q for the quotient, m - n + 1 halfwords (at least one).
   2. Space r for the remainder (optional), n halfwords.
   3. The dividend u, m halfwords, m >= 1.
   4. The divisor v, n halfwords, n >= 2.
The most significant digit of the divisor, v[n-1], must be nonzero.  The
dividend u may have leading zeros; this just makes the algorithm take
longer and makes the quotient contain more leading zeros.  A value of
NULL may be given for the address of the remainder to signify that the
caller does not want the remainder.
   The program does not alter the input parameters u and v.
   The quotient and remainder returned may have leading zeros.  The
function itself returns a value of 0 for success and 1 for invalid
parameters (e.g., division by 0).
   For now, we must have m >= n.  Knuth's Algorithm D also requires
that the dividend be at least as long as the divisor.  (In his terms,
m >= 0 (unstated).  Therefore m+n >= n.)

int divmnu (unsigned short q [], unsigned short r [],
  const unsigned short u [], const unsigned short v [],
  int m, int n) {

  const unsigned b = 65536; // Number base (16 bits).
  unsigned short* un, * vn;  // Normalized form of u, v.
  unsigned qhat;            // Estimated quotient digit.
  unsigned rhat;            // A remainder.
  unsigned p;               // Product of two digits.
  int s, i, j, t, k;

  if (m < n || n <= 0 || v [n - 1] == 0)
    return 1;              // Return if invalid param.

  if (n == 1) {                        // Take care of
    k = 0;                            // the case of a
    for (j = m - 1; j >= 0; j--) {    // single-digit
      q [j] = (k * b + u [j]) / v [0];      // divisor here.
      k = (k * b + u [j]) - q [j] * v [0];
    }
    if (r != NULL) r [0] = k;
    return 0;
  }

  // Normalize by shifting v left just enough so that
  // its high-order bit is on, and shift u left the
  // same amount.  We may have to append a high-order
  // digit on the dividend; we do that unconditionally.

  s = nlz (v [n - 1]) - 16;        // 0 <= s <= 15.
  vn = (unsigned short*)alloca (2 * n);
  for (i = n - 1; i > 0; i--)
    vn [i] = (v [i] << s) | (v [i - 1] >> 16 - s);
  vn [0] = v [0] << s;

  un = (unsigned short*)alloca (2 * (m + 1));
  un [m] = u [m - 1] >> 16 - s;
  for (i = m - 1; i > 0; i--)
    un [i] = (u [i] << s) | (u [i - 1] >> 16 - s);
  un [0] = u [0] << s;

  for (j = m - n; j >= 0; j--) {       // Main loop.
    // Compute estimate qhat of q[j].
    qhat = (un [j + n] * b + un [j + n - 1]) / vn [n - 1];
    rhat = (un [j + n] * b + un [j + n - 1]) - qhat * vn [n - 1];
  again:
    if (qhat >= b || qhat * vn [n - 2] > b * rhat + un [j + n - 2])
    {
      qhat = qhat - 1;
      rhat = rhat + vn [n - 1];
      if (rhat < b) goto again;
    }

    // Multiply and subtract.
    k = 0;
    for (i = 0; i < n; i++) {
      p = qhat * vn [i];
      t = un [i + j] - k - (p & 0xFFFF);
      un [i + j] = t;
      k = (p >> 16) - (t >> 16);
    }
    t = un [j + n] - k;
    un [j + n] = t;

    q [j] = qhat;              // Store quotient digit.
    if (t < 0) {              // If we subtracted too
      q [j] = q [j] - 1;       // much, add back.
      k = 0;
      for (i = 0; i < n; i++) {
        t = un [i + j] + vn [i] + k;
        un [i + j] = t;
        k = t >> 16;
      }
      un [j + n] = un [j + n] + k;
    }
  } // End j.
  // If the caller wants the remainder, unnormalize
  // it and pass it back.
  if (r != NULL) {
    for (i = 0; i < n; i++)
      r [i] = (un [i] >> s) | (un [i + 1] << 16 - s);
  }
  return 0;
}
*/

namespace Nirvana {

inline
unsigned FloatToBcdBase::div100 (UWord2* big_num) noexcept
{
  const UWord2* u = big_num;
  UWord2* q = big_num + 1;
  int m = (int)(big_num_end_ - big_num);
  const int n = 2;
  const UWord b = (UWord)1 << HALF_WORD_BITS; // Number base.

  UWord2* un;  // Normalized form of u, v.
  int i, j;
  Word t, k;

  assert (m >= n && n == 2);
  assert (big_num [0] == 0);

  // Normalize by shifting v left just enough so that
  // its high-order bit is on, and shift u left the
  // same amount. We may have to append a high-order
  // digit on the dividend; we do that unconditionally.

  //const auto s = nlz ((UWord)100) - HALF_WORD_BITS;
  const unsigned s = sizeof (UWord) * 8 - 7 - HALF_WORD_BITS;
  /*
  for (i = n - 1; i > 0; i--)
    vn [i] = (v [i] << s) | (v [i - 1] >> (HALF_WORD_BITS - s));
  vn [0] = v [0] << s;
  */
  static const UWord2 vn [2] = { 0, (UWord2)(100 << s) };

  un = (UWord2*)alloca (sizeof (UWord2) * (m + 1));
  un [m] = u [m - 1] >> (HALF_WORD_BITS - s);
  for (i = m - 1; i > 0; i--)
    un [i] = (u [i] << s) | (u [i - 1] >> (HALF_WORD_BITS - s));
  un [0] = u [0] << s;

  for (j = m - n; j >= 0; j--) {       // Main loop.
    // Estimated quotient digit.
    UWord qhat = ((UWord)un [j + n] * b + (UWord)un [j + n - 1]) / (UWord)vn [n - 1];
    // A remainder.
    UWord rhat = ((UWord)un [j + n] * b + (UWord)un [j + n - 1]) - qhat * (UWord)vn [n - 1];
  again:
    if (qhat >= b || qhat * (UWord)vn [n - 2] > b * rhat + (UWord)un [j + n - 2])
    {
      qhat = qhat - 1;
      rhat = rhat + vn [n - 1];
      if (rhat < b)
        goto again;
    }

    // Multiply and subtract.
    k = 0;
    for (i = 0; i < n; i++) {
      // Product of two digits.
      const UWord p = qhat * vn [i];
      t = un [i + j] - k - (p & ~(UWord2)0);
      un [i + j] = (UWord2)t;
      k = (p >> HALF_WORD_BITS) - (t >> HALF_WORD_BITS);
    }
    t = un [j + n] - k;
    un [j + n] = (UWord2)t;

    q [j] = (UWord2)qhat;     // Store quotient digit.
    if (t < 0) {              // If we subtracted too
      q [j] = q [j] - 1;      // much, add back.
      k = 0;
      for (i = 0; i < n; i++) {
        t = un [i + j] + vn [i] + k;
        un [i + j] = (UWord2)t;
        k = t >> HALF_WORD_BITS;
      }
      un [j + n] += (UWord2)k;
    }
  } // End j.

#ifndef NDEBUG
  unsigned r0 = (un [0] >> s) | (un [0 + 1] << (HALF_WORD_BITS - s));
  assert (r0 == 0);
#endif
  unsigned r = (un [1] >> s) | (un [1 + 1] << (HALF_WORD_BITS - s));
  assert (r < 100);
  return r;
}

bool FloatToBcdBase::is_zero (const UWord2* big_num) const noexcept
{
  assert (big_num [0] == 0);
	for (const UWord2* p = big_num + 1; p != big_num_end_; ++p) {
		if (*p)
			return false;
	}
	return true;
}

const unsigned* FloatToBcdBase::next (UWord2* big_num) noexcept
{
	if (!is_zero (big_num)) {
		unsigned rem = div100 (big_num);
		digits_ [0] = rem % 10;
    if (rem > 9) {
      digits_ [1] = rem / 10;
      return digits_ + 2;
    }
    if (is_zero (big_num))
      return digits_ + 1;
    digits_ [1] = 0;
		return digits_ + 2;
	}
	return digits_;
}

}
