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
#include <Nirvana/rescale.h>
#include <Nirvana/bitutils.h>

// See https://www.codeproject.com/Tips/618570/UInt-Multiplication-Squaring
// and https://www.codeproject.com/Tips/785014/UInt-Division-Modulus

namespace Nirvana {

inline
void mult64to128 (uint64_t u, uint64_t v, uint64_t& h, uint64_t& l)
{
  uint64_t u1 = (u & 0xffffffff);
  uint64_t v1 = (v & 0xffffffff);
  uint64_t t = (u1 * v1);
  uint64_t w3 = (t & 0xffffffff);
  uint64_t k = (t >> 32);

  u >>= 32;
  t = (u * v1) + k;
  k = (t & 0xffffffff);
  uint64_t w1 = (t >> 32);

  v >>= 32;
  t = (u1 * v) + k;
  k = (t >> 32);

  h = (u * v) + w1 + k;
  l = (t << 32) + w3;
}

inline
void divmod128by64 (const uint64_t u1, const uint64_t u0, uint64_t v, uint64_t& q, uint64_t& r)
{
  const uint64_t b = 1ll << 32;
  uint64_t un1, un0, vn1, vn0, q1, q0, un32, un21, un10, rhat, left, right;
  size_t s;

  s = nlz (v);
  v <<= s;
  vn1 = v >> 32;
  vn0 = v & 0xffffffff;

  if (s > 0) {
    un32 = (u1 << s) | (u0 >> (64 - s));
    un10 = u0 << s;
  } else {
    un32 = u1;
    un10 = u0;
  }

  un1 = un10 >> 32;
  un0 = un10 & 0xffffffff;

  q1 = un32 / vn1;
  rhat = un32 % vn1;

  left = q1 * vn0;
  right = (rhat << 32) + un1;
again1:
  if ((q1 >= b) || (left > right)) {
    --q1;
    rhat += vn1;
    if (rhat < b) {
      left -= vn0;
      right = (rhat << 32) | un1;
      goto again1;
    }
  }

  un21 = (un32 << 32) + (un1 - (q1 * v));

  q0 = un21 / vn1;
  rhat = un21 % vn1;

  left = q0 * vn0;
  right = (rhat << 32) | un0;
again2:
  if ((q0 >= b) || (left > right)) {
    --q0;
    rhat += vn1;
    if (rhat < b) {
      left -= vn0;
      right = (rhat << 32) | un0;
      goto again2;
    }
  }

  r = ((un21 << 32) + (un0 - (q0 * v))) >> s;
  q = (q1 << 32) | q0;
}

#ifndef NIRVANA_FAST_RESCALE64

int64_t rescale64 (int64_t a, uint64_t b, int64_t c, uint64_t d) noexcept
{
  uint64_t u = std::abs (a);
  uint64_t h, l;
  mult64to128 (u, b, h, l);
  _add128 (h, l, c);
  uint64_t q, r;
  divmod128by64 (h, l, d, q, r);
  return a < 0 ? -(int64_t)q : q;
}

#endif

}
