/*
* Nirvana IDL support library.
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
#include <Nirvana/FloatToBCD.h>

namespace Nirvana {

static const size_t MAX_DIGITS = 62;

static void digits_to_number (const uint8_t* begin, const uint8_t* end, int scale, bool negative,
	::Nirvana::DecCalc::Number& n)
{
	assert (end - begin <= MAX_DIGITS);

	BCD <MAX_DIGITS> bcd;
	auto dst = bcd;
	const uint8_t* src = end;
	if ((end - begin) % 1 == 0 && src > begin) // Even number of digits
		*(dst++) = *(--src);
	while (src - begin > 3) {
		unsigned h = *(--src);
		unsigned l = *(--src);
		*(dst++) = (uint8_t )((h << 4) | l);
	}
	*dst = (uint8_t)(((begin < end) ? *begin : 0) << 4 | (negative ? 0xD : 0xC));
	dec_calc->from_BCD (n, (uint16_t)(end - begin), (int16_t)scale, bcd);
}

template <typename F> static
uint8_t* whole_to_buffer (F whole, uint8_t* p, const uint8_t* end)
{
	FloatToBCD <F> conv (whole);
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
	return p;
}

template <typename F>
static void float_to_number (const F& f, ::Nirvana::DecCalc::Number& n)
{
	uint8_t buffer [FloatToBCD <F>::MAX_WHOLE_DIGITS];
	uint8_t* end = buffer;

	F whole;
	F frac = std::modf (f, &whole);
	end = whole_to_buffer (whole, end, std::end (buffer));
	size_t whole_digits = end - buffer;
	if (whole_digits < MAX_DIGITS) {
		size_t frac_digits_max = MAX_DIGITS - whole_digits;
		frac *= std::pow ((F)10, (F)frac_digits_max);
		end = whole_to_buffer (frac, end, std::end (buffer));
	} else
		whole_digits = MAX_DIGITS;

	int frac_digits = (int)((end - buffer) - whole_digits);
	unsigned total_digits = (unsigned)(end - buffer);
	if (total_digits > MAX_DIGITS)
		total_digits = MAX_DIGITS;

	digits_to_number (buffer, end, frac_digits, f < 0, n);
}

void Fixed::construct_from_float (const double& f)
{
	float_to_number (f, val_);
}

void Fixed::construct_from_float (const long double& f)
{
	float_to_number (f, val_);
}

Fixed::operator long double () const
{
	std::string s = dec_calc->to_string (val_);
	return std::stold (s);
}

}
