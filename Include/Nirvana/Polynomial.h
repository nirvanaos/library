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
#ifndef NIRVANA_POLYNOMIAL_H_
#define NIRVANA_POLYNOMIAL_H_
#pragma once

#include "platform.h"
#include <cmath>
#include <cfenv>
#include <iterator>

namespace Nirvana {

class PolynomialBase
{
public:
	bool overflow () const noexcept
	{
		return overflow_;
	}

	unsigned base () const noexcept
	{
		return base_;
	}

	int exp () const noexcept
	{
		return exp_;
	}

	void add_exp (int x) noexcept
	{
		exp_ += x;
	}

	struct Part
	{
		UWord u;
		unsigned num_digits;

		template <typename F>
		F get (unsigned base, int power) const noexcept;
	};

protected:
	PolynomialBase (unsigned base, int exp, Part* parts) noexcept :
		end_ (parts),
		base_ (base),
		exp_ (exp),
		overflow_ (false)
	{}

	void add (const Part& part, const Part* end) noexcept;

	template <typename F>
	void to_float (const Part* begin, F& num) noexcept;

	Part* end_;
	unsigned base_;
	int exp_;
	bool overflow_;
};

template <typename F>
void PolynomialBase::to_float (const Part* begin, F& num) noexcept
{
	assert (!overflow_);

	const Part* p = begin;
	if (p < end_) {
		int rm = std::fegetround ();
		std::fesetround (FE_TOWARDZERO);

		int power = exp_;
		power -= p->num_digits;
		F f = p->get <F> (base_, power);
		while (end_ > ++p) {
			power -= p->num_digits;
			f += p->get <F> (base_, power);
		}

		std::fesetround (rm);

		num = f;
	} else
		num = 0;
}

template <typename F>
F PolynomialBase::Part::get (unsigned base, int power) const noexcept
{
	F ret;
	if (u) {
		if (power) {
			F weigth = std::pow ((F)base, (F)power);
			if (1 == u)
				ret = weigth;
			else
				ret = weigth * (F)u;
		} else
			ret = (F)u;
	} else
		ret = 0;
	return ret;
}

template <unsigned max_parts>
class Polynomial : public PolynomialBase
{
	using Base = PolynomialBase;

public:
	Polynomial (unsigned base, int exp = 0) noexcept :
		Base (base, exp, parts_)
	{}

	void add (const Part& part) noexcept
	{
		Base::add (part, std::end (parts_));
	}

	template <typename F>
	void to_float (F& num) noexcept
	{
		Base::to_float (parts_, num);
	}

private:
	Part parts_ [max_parts];
};

}

#endif
