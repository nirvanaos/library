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
#include <assert.h>
#include <cmath>
#include <iterator>

namespace Nirvana {

FloatToBCD::FloatToBCD (FloatMax whole) noexcept
{
	assert (whole >= 0);

	UWord2* end = big_num_;

	FloatMax div = (FloatMax)((UWord)1 << HALF_WORD_BITS);

	while (whole > 0) {
		FloatMax part = std::fmod (whole, div);
		whole -= part;
		whole /= div;
		assert (end < std::end (big_num_));
		*(end++) = (UWord2)part;
	}
	big_num_end_ = end;
}

inline
unsigned FloatToBCD::div100 () noexcept
{
  const UWord b = (UWord)1 << HALF_WORD_BITS; // Number base.

  Word k = 0;
  for (UWord2* p = big_num_end_; p > big_num_;) {
    --p;
    UWord kbu = k * b + *p;
    UWord qhat = kbu / 100;      // divisor here.
    *p = (UWord2)qhat;
    k = kbu - qhat * 100;
  }
  unsigned r = (unsigned)k;
  assert (r < 100);
  return r;
}

bool FloatToBCD::is_zero () const noexcept
{
	for (const UWord2* p = big_num_end_; p > big_num_;) {
		if (*(--p))
			return false;
	}
	return true;
}

const unsigned* FloatToBCD::next () noexcept
{
	if (!is_zero ()) {
		unsigned rem = div100 ();
		digits_ [0] = rem % 10;
    if (rem > 9) {
      digits_ [1] = rem / 10;
      return digits_ + 2;
    }
    if (is_zero ())
      return digits_ + 1;
    digits_ [1] = 0;
		return digits_ + 2;
	}
	return digits_;
}

}
