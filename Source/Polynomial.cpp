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
#include <cmath>
#include <cfenv>

#pragma float_control (precise, on)
#pragma fp_contract (off)

namespace Nirvana {

inline
PolynomialBase::Part* PolynomialBase::parts () noexcept
{
	return static_cast <Polynomial <16, 1>&> (*this).parts_;
}

inline
const PolynomialBase::Part* PolynomialBase::parts () const noexcept
{
	return static_cast <const Polynomial <16, 1>&> (*this).parts_;
}

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
	exp_ += part.num_digits;
}

FloatMax PolynomialBase::to_float () const noexcept
{
	FloatMax ret;

	const Part* p = parts ();
	if (p < end_) {
		int rm = std::fegetround ();

		int power = exp_;
		power -= p->num_digits;
		ret = p->get (base_, power);
		while (end_ > ++p) {
			power -= p->num_digits;
			ret += p->get (base_, power);
		}

		std::fesetround (rm);
	} else
		ret = 0;

	return ret;
}

FloatMax PolynomialBase::Part::get (unsigned base, int power) const noexcept
{
	std::fesetround (FE_TONEAREST);
	FloatMax w = std::pow ((FloatMax)base, power);
	std::fesetround (FE_TOWARDZERO);
	FloatMax fu = u;
	FloatMax r = w * fu;
	return r;
}

}
