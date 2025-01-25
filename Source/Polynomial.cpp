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
#include <Nirvana/Polynomial.h>
#include <Nirvana/WideInEx.h>

#pragma float_control (precise, on)
#pragma fp_contract (off)

namespace Nirvana {

PolynomialBase::PolynomialBase () noexcept :
	end_ (parts ()),
	digits_ (0),
	overflow_ (false)
{}

void PolynomialBase::add (const Part& part, const Part* end) noexcept
{
	if (!part.num_digits)
		return;

	const Part* begin = parts ();
	if (end_ == begin) {
		if (!part.u)
			return;
	} else if (end_ >= end) {
		overflow_ = true;
		return;
	}
	*(end_++) = part;
	digits_ += part.num_digits;
}

unsigned PolynomialBase::get_part (WideInEx& in, Part& part, unsigned base, bool drop_tz,
	bool& not_last)
{
	using UWord = PolynomialBase::UWord;

	UWord cutoff = std::numeric_limits <UWord>::max ();
	unsigned cutlim = cutoff % (UWord)base;
	cutoff /= (UWord)base;
	UWord acc = 0;
	unsigned digits = 0, zeros = 0;
	UWord tzdiv = 1;
	bool overflow = false;

	for (unsigned digit; in.get_digit (base, digit); in.next ()) {

		if (acc > cutoff || (acc == cutoff && digit > cutlim)) {
			overflow = true;
			break;
		}

		acc *= base;
		acc += digit;
		++digits;
		if (digit) {
			zeros = 0;
			tzdiv = 1;
		} else {
			++zeros;
			if (acc)
				tzdiv *= base;
		}
	}
	if (drop_tz && !overflow) {
		part.num_digits = digits - zeros;
		assert (tzdiv);
		acc /= tzdiv;
	} else
		part.num_digits = digits;
	part.u = acc;
	not_last = overflow;
	return digits;
}

unsigned PolynomialBase::get_parts (WideInEx& in, bool drop_tz, unsigned base, const Part* end)
{
	unsigned digs = 0;

	bool not_last;
	do {
		Part part;
		digs += get_part (in, part, base, drop_tz, not_last);
		add (part, end);
	} while (not_last);

	return digs;
}

}
