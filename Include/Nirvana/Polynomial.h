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

class WideInEx;

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

	unsigned digits () const noexcept
	{
		return digits_;
	}

	using UWord = IntTypes <WORD_BYTES>::Unsigned;

	struct Part
	{
		UWord u;
		unsigned num_digits;
	};

	FloatMax to_float (int exp) const noexcept;

protected:
	PolynomialBase (unsigned base, Part* parts) noexcept :
		end_ (parts),
		base_ (base),
		digits_ (0),
		overflow_ (false)
	{}

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

	void add (const Part& part, const Part* end) noexcept;

	unsigned get_parts (WideInEx& in, bool drop_tz, unsigned base, const Part* end);

private:
	Part* parts () noexcept;
	const Part* parts () const noexcept;

	static FloatMax get (UWord u, unsigned base, int power) noexcept;

	static unsigned get_part (WideInEx& in, PolynomialBase::Part& part, unsigned base, bool drop_tz,
		bool& not_last);

private:
	Part* end_;
	unsigned base_;
	unsigned digits_;
	bool overflow_;
};

template <unsigned BASE, unsigned DIGITS>
class Polynomial : public PolynomialBase
{
	using Base = PolynomialBase;
	static const size_t WORD_COUNT = WordCount <BASE, DIGITS>::COUNT;

public:
	Polynomial () noexcept :
		Base (BASE, parts_)
	{}

	void add (const Part& part) noexcept
	{
		Base::add (part, parts_ + WORD_COUNT);
	}

	unsigned get_parts (WideInEx& in, bool drop_tz)
	{
		return Base::get_parts (in, drop_tz, BASE, parts_ + WORD_COUNT);
	}

private:
	friend class PolynomialBase;
	Part parts_ [WORD_COUNT];
};

}

#endif
