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
unsigned FloatToBCDBase::div100 (UWord2* begin, UWord2* end) noexcept
{
	UWord remainder = 0;
	for (UWord2* p = end; p != begin;) {
		remainder = (remainder << (sizeof (UWord2) * 8)) + *(--p);
		*p = (UWord2)(remainder / 100);
		remainder %= 100;
	}
	return (unsigned)remainder;
}

inline
bool FloatToBCDBase::is_zero (const UWord2* begin, const UWord2* end) noexcept
{
	for (const UWord2* p = begin; p != end; ++p) {
		if (*p)
			return false;
	}
	return true;
}

const unsigned* FloatToBCDBase::next (UWord2* begin, UWord2* end) noexcept
{
	if (!is_zero (begin, end)) {
		unsigned rem = div100 (begin, end);
		digits_ [0] = rem % 10;
		digits_ [1] = rem / 10;
		return digits_ + ((rem > 9) ? 2 : 1);
	}
	return digits_;
}

}
