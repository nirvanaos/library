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
#include "IntTypes.h"
#include <type_traits>

namespace Nirvana {

class PolynomialBase
{
	static_assert (std::numeric_limits <FloatMax>::radix == 2, "Unexpected radix");
	static const size_t MANT_BYTES = std::numeric_limits <FloatMax>::digits / 8;
	static const size_t INT_BYTES = MANT_BYTES >= 8 ? 8 : (MANT_BYTES >= 4 ? 4 : 2);
	static const size_t WORD_BYTES = INT_BYTES >= sizeof (Nirvana::UWord) ? sizeof (Nirvana::UWord) : INT_BYTES;

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

	void exp (int exp) noexcept
	{
		exp_ = exp;
	}

	using UWord = IntTypes <WORD_BYTES>::Unsigned;

	struct Part
	{
		UWord u;
		unsigned num_digits;

		FloatMax get (unsigned base, int power) const noexcept;
	};

	FloatMax to_float () const noexcept;

protected:
	PolynomialBase (unsigned base, int exp, Part* parts) noexcept :
		end_ (parts),
		base_ (base),
		exp_ (exp),
		overflow_ (false)
	{}

	void add (const Part& part, const Part* end) noexcept;

	template <unsigned BASE, unsigned DIGITS> struct WordCount;

	template <unsigned DIGITS>
	struct WordCount <10, DIGITS>
	{
		static const unsigned COUNT = (DIGITS + std::numeric_limits <UWord>::digits10 - 1)
			/ std::numeric_limits <UWord>::digits10;
	};

	static const unsigned WORD_HEX_DIGITS = sizeof (UWord) * 2;

	template <unsigned DIGITS>
	struct WordCount <16, DIGITS>
	{
		static const unsigned COUNT = (DIGITS * 4 + WORD_HEX_DIGITS - 1) / WORD_HEX_DIGITS;
	};

private:
	Part* parts () noexcept;
	const Part* parts () const noexcept;

private:
	Part* end_;
	unsigned base_;
	int exp_;
	bool overflow_;
};

template <unsigned BASE, unsigned DIGITS>
class Polynomial : public PolynomialBase
{
	using Base = PolynomialBase;
	static const size_t WORD_COUNT = WordCount <BASE, DIGITS>::COUNT;

public:
	Polynomial (int exp = 0) noexcept :
		Base (BASE, exp, parts_)
	{}

	void add (const Part& part) noexcept
	{
		Base::add (part, parts_ + WORD_COUNT);
	}

private:
	friend class PolynomialBase;
	Part parts_ [WORD_COUNT];
};

}

#endif
