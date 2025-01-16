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
#include <Nirvana/FloatToPacked.h>
#include <Nirvana/Polynomial.h>
#include <limits>

namespace Nirvana {

template <typename F> inline static
void float_to_number (const F& f, ::Nirvana::DecCalc::Number& n)
{
	static const size_t MAX_DIGITS = 62;

	FloatToPacked <F> conv (f, MAX_DIGITS, MAX_DIGITS);
	BCD <MAX_DIGITS> bcd;
	unsigned digits = conv.pack (0, bcd, sizeof (bcd));
	int scale = digits - (conv.digits () - conv.scale ());
	dec_calc->from_BCD (n, (uint16_t)digits, (int16_t)scale, bcd);
}

void Fixed::construct_from_float (float f)
{
	float_to_number (f, val_);
}

void Fixed::construct_from_float (const double& f)
{
	float_to_number (f, val_);
}

void Fixed::construct_from_float (const long double& f)
{
	float_to_number (f, val_);
}

class Fixed::Poly
{
public:
	Poly (int exp) noexcept :
		poly_ (10, exp),
		part_ { 0, 0 }
	{}

	void set_first (unsigned digit) noexcept
	{
		assert (part_.u == 0);
		assert (part_.num_digits == 0);
		assert (digit < 10);
		part_.u = digit;
		part_.num_digits = 1;
	}

	void add_digit (unsigned digit) noexcept
	{
		static const UWord MAX_WORD = std::numeric_limits <UWord>::max ();
		static const unsigned cutlim = MAX_WORD % 100;
		static const UWord cutoff = MAX_WORD / 100;

		assert (digit < 100);
		if (part_.u > cutoff || (part_.u == cutoff && digit > cutlim)) {
			poly_.add (part_);
			part_.u = 0;
			part_.num_digits = 0;
		}
		part_.u = part_.u * 100 + digit;
		part_.num_digits += 2;
	}

	void finalize (long double& val) noexcept
	{
		if (part_.num_digits)
			poly_.add (part_);
		assert (!poly_.overflow ());
		poly_.to_float (val);
	}

private:
	// We need 206 bits to represent 62-digits decimal
	static const unsigned WORD_BITS = sizeof (UWord) * 8;
	static const unsigned MAX_WORDS = (206 + WORD_BITS - 1) / WORD_BITS;

	using P = Polynomial <MAX_WORDS>;

private:
	P poly_;
	P::Part part_;
};

Fixed::operator long double () const
{
	Poly poly ((int)val_.exponent ());

	unsigned digits = (unsigned)val_.digits ();
	const uint8_t* src = val_.lsu ().data () + digits / 2;
	if (digits % 2)
		poly.set_first (*src);
	while (val_.lsu ().data () < src) {
		poly.add_digit (*(--src));
	}

	long double val;
	poly.finalize (val);
	if (val_.bits () & 0x80)
		val = -val;

	return val;
}

}
