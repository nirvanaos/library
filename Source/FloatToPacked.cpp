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
#include <cmath>
#include <cfenv>

namespace Nirvana {

FloatToPacked::FloatToPacked (FloatMax f, unsigned max_digits, unsigned max_scale) :
	digits_ (0),
	scale_ (0),
	negative_ (false)
{
	assert (max_digits <= sizeof (bcd_));

	if (f < 0) {
		negative_ = true;
		f = -f;
	}

	uint8_t* end = bcd_;

	int rm = std::fegetround ();
	std::fesetround (FE_TONEAREST);

	FloatMax whole;
	FloatMax frac = std::modf (f, &whole);
	end = whole_to_buffer (whole, end, std::end (bcd_));
	unsigned digits = (unsigned)(end - bcd_);
	if (digits >= max_digits) {
		digits_ = max_digits;
		scale_ = max_digits - digits;
	} else {
		if (max_scale > 0) {
			unsigned frac_digits_max = max_digits - digits;
			if (max_scale > frac_digits_max)
				max_scale = frac_digits_max;
			frac *= std::pow ((FloatMax)10, (int)max_scale);
			frac = std::round (frac);
			const uint8_t* begin = end;
			end = whole_to_buffer (frac, end, std::end (bcd_));
			uint8_t* max_end = bcd_ + max_digits;
			if (end > max_end)
				end = max_end;
			scale_ = (int)(end - begin);
		} else
			scale_ = 0;
		digits_ = (unsigned)(end - bcd_);
	}

	std::fesetround (rm);

	assert (digits_ <= max_digits);
	assert (scale_ <= (int)max_scale);
	assert (digits_ == 0 || bcd_ [0]); // does not contain leading zeros
}

uint8_t* FloatToPacked::whole_to_buffer (const FloatMax& whole, uint8_t* buf, const uint8_t* end) noexcept
{
	FloatToBCD conv (whole);
	uint8_t* p = buf;
	for (;;) {
		const unsigned* d_end = conv.next ();
		for (const unsigned* d = conv.digits (); d < d_end; ++d) {
			assert (p < end);
			assert (*d < 10);
			*(p++) = (uint8_t)*d;
		}
		if (d_end - conv.digits () < 2)
			break;
	}
	std::reverse (buf, p);
	return p;
}


unsigned FloatToPacked::pack (int shift, uint8_t* packed, unsigned packed_size) const
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

	const uint8_t* src = bcd_ + digits_copy;
	const uint8_t* end = bcd_ + digits_;
	if (src > end) {
		digits_left -= (int)(src - end);
		src = end;
	}

	std::fill_n (packed, packed_size - 1, 0);
	packed [packed_size - 1] = (negative_ && src > bcd_)  ? 0x0D : 0x0C;

	if (digits_left > 0 && src > bcd_) {
		
		if (digits_left % 2)
			packed [--digits_left / 2] |= *(--src) << 4;

		while (digits_left >= 2 && (src - bcd_) >= 2) {
			unsigned byte = *(--src);
			byte |= *(--src) << 4;
			packed [(digits_left -= 2) / 2] = (uint8_t)byte;
		}

		assert (digits_left % 2 == 0);
		if (digits_left > 0 && src > bcd_)
			packed [--digits_left / 2] = (uint8_t)(*(--src));
	}

	return digits;
}

void FloatToPacked::pack (unsigned digits, int scale, uint8_t* packed) const
{
	int src_whole = digits_ - scale_;
	int dst_whole = digits - scale;
	int shift = dst_whole - src_whole;
	if (src_whole > 0 && shift < 0)
		throw CORBA::DATA_CONVERSION ();
	pack (shift, packed, (digits + 2) / 2);
}

}
