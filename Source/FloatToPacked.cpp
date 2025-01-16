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
#include <Nirvana/FloatToPacked.h>

namespace Nirvana {

unsigned FloatToPackedBase::pack (int shift, uint8_t* packed, unsigned packed_size) const
{
	assert (packed_size);

	int digits = shift + digits_;
	if (digits <= 0)
		digits = 1;

	int max_digits = packed_size * 2 - 1;
	if (digits > max_digits)
		digits = max_digits;

	int digits_left = digits;
	if (digits_left % 2 == 0)
		++digits_left; // Always start from the odd nibble

	int digits_copy = digits;
	if (shift > 0)
		digits_copy -= shift;

	const uint8_t* begin = bcd ();
	const uint8_t* src = begin + digits_copy;
	const uint8_t* end = begin + digits_;
	if (src > end) {
		digits_left -= (int)(src - end);
		src = end;
	}

	std::fill_n (packed, packed_size - 1, 0);
	packed [packed_size - 1] = (negative_ && src > begin)  ? 0x0D : 0x0C;

	if (digits_left > 0 && src > begin) {
		
		if (digits_left % 2)
			packed [--digits_left / 2] |= *(--src) << 4;

		while (digits_left >= 2 && (src - begin) >= 2) {
			unsigned byte = *(--src);
			byte |= *(--src) << 4;
			packed [(digits_left -= 2) / 2] = (uint8_t)byte;
		}

		assert (digits_left % 2 == 0);
		if (digits_left > 0 && src > begin)
			packed [--digits_left / 2] = (uint8_t)(*(--src));
	}

	return digits;
}

void FloatToPackedBase::pack (unsigned digits, int scale, uint8_t* packed) const
{
	int src_whole = digits_ - scale_;
	int dst_whole = digits - scale;
	int shift = dst_whole - src_whole;
	if (src_whole > 0 && shift < 0)
		throw CORBA::DATA_CONVERSION ();
	pack (shift, packed, (digits + 2) / 2);
}

}
