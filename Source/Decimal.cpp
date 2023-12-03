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
#include "../../pch/pch.h"

namespace Nirvana {

void BCD_zero (uint8_t* bcd, size_t size) noexcept
{
	std::fill_n (bcd, size - 1, (uint8_t)0);
	bcd [size - 1] = 0x0C;
}

bool BCD_is_zero (const uint8_t* bcd, size_t size) noexcept
{
	for (const uint8_t* p = bcd, *end = p + size; p != end; ++p) {
		if (*p)
			return false;
	}
	return bcd [size - 1] == 0x0C;
}

}
