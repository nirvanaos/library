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
#include <limits>

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

protected:
	PolynomialBase () noexcept;
	void add (const Part& part, const Part* end) noexcept;
	unsigned get_parts (WideInEx& in, bool drop_tz, unsigned base, const Part* end);
	const Part* parts () const noexcept;

private:
	Part* parts () noexcept;

	static unsigned get_part (WideInEx& in, PolynomialBase::Part& part, unsigned base, bool drop_tz,
		bool& not_last);

protected:
	Part* end_;
	unsigned digits_;
	bool overflow_;
};

template <unsigned BASE>
class PolynomialBaseN : public PolynomialBase
{
public:
	FloatMax to_float (int exp) const;

protected:
	static constexpr unsigned word_count (unsigned digits);

private:
	static inline FloatMax mul_pow (FloatMax x, int exp);
};

extern template
FloatMax PolynomialBaseN <10>::to_float (int exp) const;

extern template
FloatMax PolynomialBaseN <16>::to_float (int exp) const;

template <> inline
constexpr unsigned PolynomialBaseN <10>::word_count (unsigned digits)
{
	const unsigned WORD_DIGITS = std::numeric_limits <UWord>::digits10;
	return (digits + WORD_DIGITS - 1) / WORD_DIGITS;
}

template <> inline
constexpr unsigned PolynomialBaseN <16>::word_count (unsigned digits)
{
	const unsigned WORD_DIGITS = sizeof (UWord) * 2;
	return (digits + WORD_DIGITS - 1) / WORD_DIGITS;
}

template <unsigned BASE, unsigned DIGITS>
class Polynomial : public PolynomialBaseN <BASE>
{
	using Base = PolynomialBaseN <BASE>;
	static const size_t WORD_COUNT = Base::word_count (DIGITS);

public:
	void add (const Base::Part& part) noexcept
	{
		Base::add (part, parts_ + WORD_COUNT);
	}

	unsigned get_parts (WideInEx& in, bool drop_tz)
	{
		return Base::get_parts (in, drop_tz, BASE, parts_ + WORD_COUNT);
	}

private:
	friend class PolynomialBase;
	Base::Part parts_ [WORD_COUNT];
};

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

}

#endif
