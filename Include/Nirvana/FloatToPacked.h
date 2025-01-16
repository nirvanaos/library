/// \file
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
#ifndef NIRVANA_FLOATTOPACKED_H_
#define NIRVANA_FLOATTOPACKED_H_
#pragma once

#include "FloatToBCD.h"
#include <cfenv>

namespace Nirvana {

class FloatToPackedBase
{
public:
	unsigned digits () const noexcept
	{
		return digits_;
	}

	int scale () const noexcept
	{
		return scale_;
	}

	bool negative () const noexcept
	{
		return negative_;
	}

	const uint8_t* bcd () const noexcept
	{
		return (const uint8_t*)(this + 1);
	}

	unsigned pack (int shift, uint8_t* packed, unsigned packed_size) const;

	void pack (unsigned digits, int scale, uint8_t* packed) const;

protected:
	FloatToPackedBase () :
		digits_ (0),
		scale_ (0),
		negative_ (false)
	{}

protected:
	unsigned digits_;
	int scale_;
	bool negative_;
};

template <typename F>
class FloatToPacked : public FloatToPackedBase
{
	using Base = FloatToPackedBase;

public:
	FloatToPacked (F f, unsigned max_digits, unsigned max_scale) noexcept;

private:
	uint8_t* whole_to_buffer (F whole, uint8_t* buf, const uint8_t* end) noexcept;

private:
	uint8_t bcd_ [FloatToBCD <F>::MAX_DIGITS];
};

template <typename F>
FloatToPacked <F>::FloatToPacked (F f, unsigned max_digits, unsigned max_scale) noexcept
{
	assert (max_digits <= sizeof (bcd_));

	if (f < 0) {
		negative_ = true;
		f = -f;
	}

	uint8_t* end = bcd_;

	int rm = std::fegetround ();
	std::fesetround (FE_TONEAREST);

	F whole;
	F frac = std::modf (f, &whole);
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
			frac *= std::pow ((F)10, (F)max_scale);
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

template <typename F>
uint8_t* FloatToPacked <F>::whole_to_buffer (F whole, uint8_t* buf, const uint8_t* end) noexcept
{
	FloatToBCD <F> conv (whole);
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

}

#endif
