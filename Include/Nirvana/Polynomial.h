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
#include <limits>
#include <cmath>
#include <cfenv>

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
	FloatMax to_float (int exp) const noexcept;

protected:
	template <unsigned DIGITS> struct WordCount;

private:
	static inline FloatMax mul_pow (FloatMax x, int exp) noexcept;
};

template <unsigned BASE>
FloatMax PolynomialBaseN <BASE>::to_float (int exp) const noexcept
{
	FloatMax ret = 0;

	const Part* p = parts ();
	if (p < end_) {
		int rm = std::fegetround ();
		std::fesetround (FE_TOWARDZERO);

		int power = digits_ + exp;
		for (;;) {
			power -= p->num_digits;
			auto x = mul_pow (p->u, power);
			if (++p >= end_) {
				std::fesetround (FE_TONEAREST);
				ret += x;
				break;
			} else
				ret += x;
		}

		std::fesetround (rm);
	}

	return ret;
}

template <> template <unsigned DIGITS>
struct PolynomialBaseN <10>::WordCount
{
	static const unsigned WORD_DIGITS = std::numeric_limits <UWord>::digits10;
	static const unsigned COUNT = (DIGITS + WORD_DIGITS - 1) / WORD_DIGITS;
};

template <> template <unsigned DIGITS>
struct PolynomialBaseN <16>::WordCount
{
	static const unsigned WORD_DIGITS = sizeof (UWord) * 2;
	static const unsigned COUNT = (DIGITS + WORD_DIGITS - 1) / WORD_DIGITS;
};

inline FloatMax PolynomialBaseN <10>::mul_pow (FloatMax x, int exp) noexcept
{
#if (LDBL_MAX_10_EXP <= FLT_MAX_10_EXP)
	static const FloatMax pos [] = { 1e+1F, 1e+2F, 1e+4F, 1e+8F, 1e+16F, 1e+32F };
	static const FloatMax neg [] = { 1e-1F, 1e-2F, 1e-4F, 1e-8F, 1e-16F, 1e-32F };
#elif (LDBL_MAX_10_EXP <= DBL_MAX_10_EXP)
	static const FloatMax pos [] = { 1e+1, 1e+2, 1e+4, 1e+8, 1e+16, 1e+32, 1e+64, 1e+128, 1e+256 };
	static const FloatMax neg [] = { 1e-1, 1e-2, 1e-4, 1e-8, 1e-16, 1e-32, 1e-64, 1e-128, 1e-256 };
#else
	static const FloatMax pos [] = { 1e+1L, 1e+2L, 1e+4L, 1e+8L, 1e+16L, 1e+32L, 1e+64L, 1e+128L,
		1e+256L, 1e+512L, 1e+1024L, 1e+2048L, 1e+4096L };
	static const FloatMax neg [] = { 1e-1L, 1e-2L, 1e-4L, 1e-8L, 1e-16L, 1e-32L, 1e-64L, 1e-128L,
		1e-256L, 1e-512L, 1e-1024L, 1e-2048L, 1e-4096L };
#endif

	if (!x || !exp)
		return x;

	size_t uexp;
	const FloatMax* e;
	if (exp > 0) {
		uexp = exp;
		e = pos;
	} else {
		uexp = -exp;
		e = neg;
	}

	while (uexp) {
		if (uexp & 1)
			x *= *e;
		uexp >>= 1;
		++e;
	}

	return x;
}

inline FloatMax PolynomialBaseN <16>::mul_pow (FloatMax x, int exp) noexcept
{
	if (x && exp)
		return std::ldexp (x, exp * 4);
	else
		return x;
}

template <unsigned BASE, unsigned DIGITS>
class Polynomial : public PolynomialBaseN <BASE>
{
	using Base = PolynomialBaseN <BASE>;
	static const size_t WORD_COUNT = Base::WordCount <DIGITS>::COUNT;

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
