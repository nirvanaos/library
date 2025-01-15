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

namespace Nirvana {

inline
unsigned FloatToBcdBase::div100 (UWord2* big_num) noexcept
{
  const UWord b = (UWord)1 << HALF_WORD_BITS; // Number base.

  Word k = 0;
  for (UWord2* p = big_num_end_; p > big_num;) {
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

bool FloatToBcdBase::is_zero (const UWord2* big_num) const noexcept
{
	for (const UWord2* p = big_num; p != big_num_end_; ++p) {
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
