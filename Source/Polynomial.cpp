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
#include <Nirvana/Polynomial.h>
#include <float.h>
#include <cmath>
#include <cfenv>
#include <Nirvana/WideInEx.h>

#pragma float_control (precise, on)

#ifdef _MSC_VER

#pragma fenv_access (on)
#pragma fp_contract (off)

#else

#pragma STDC FENV_ACCESS ON
//#pragma STDC FENV_ROUND FE_DYNAMIC
#pragma STDC FP_CONTRACT OFF

#endif

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

namespace {

template <size_t COUNT> struct Exp10Positive
{
	static const FloatMax exp [COUNT];
};

template <size_t COUNT> struct Exp10Negative
{
	static const FloatMax exp [COUNT];
};

#if (LDBL_MAX_EXP >= 32)

template <>
const FloatMax Exp10Positive <6>::exp [6] = { 1e+1F, 1e+2F, 1e+4F, 1e+8F, 1e+16F, 1e+32F };

#endif

#if (LDBL_MIN_EXP <= -32)

template <>
const FloatMax Exp10Negative <6>::exp [6] = { 1e-1F, 1e-2F, 1e-4F, 1e-8F, 1e-16F, 1e-32F };

#endif

#if (LDBL_MAX_EXP >= 256)

template <>
const FloatMax Exp10Positive <9>::exp [9] = { 1e+1, 1e+2, 1e+4, 1e+8, 1e+16, 1e+32, 1e+64, 1e+128,
	1e+256 };

#endif

#if (LDBL_MIN_EXP <= -256)

template <>
const FloatMax Exp10Negative <9>::exp [9] = { 1e-1, 1e-2, 1e-4, 1e-8, 1e-16, 1e-32, 1e-64, 1e-128,
	1e-256 };

#endif

#if (LDBL_MAX_EXP >= 4096)

template <>
const FloatMax Exp10Positive <13>::exp [13] = { 1e+1L, 1e+2L, 1e+4L, 1e+8L, 1e+16L, 1e+32L, 1e+64L,
	1e+128L, 1e+256L, 1e+512L, 1e+1024L, 1e+2048L, 1e+4096L };

#endif

#if (LDBL_MIN_EXP <= -4096)

template <>
const FloatMax Exp10Negative <13>::exp [13] = { 1e-1L, 1e-2L, 1e-4L, 1e-8L, 1e-16L, 1e-32L, 1e-64L,
	1e-128L, 1e-256L, 1e-512L, 1e-1024L, 1e-2048L, 1e-4096L };

#endif

}

template <>
inline FloatMax PolynomialBaseN <10>::mul_pow (FloatMax x, int exp)
{
	if (!x)
		return 0;

	unsigned uexp;
	const FloatMax* e;
	if (exp > 0) {
		if (exp > std::numeric_limits <FloatMax>::max_exponent10)
			throw_DATA_CONVERSION (make_minor_errno (ERANGE));
		uexp = exp;
		e = Exp10Positive <log2_floor (std::numeric_limits <FloatMax>::max_exponent10) + 1>::exp;
	} else if (exp < 0) {
		if (exp < std::numeric_limits <FloatMax>::min_exponent10)
			return 0;
		uexp = -exp;
		e = Exp10Negative <log2_floor (-std::numeric_limits <FloatMax>::min_exponent10) + 1>::exp;
	} else
		return x;

	while (uexp) {
		if (uexp & 1)
			x *= *e;
		uexp >>= 1;
		++e;
	}

	return x;
}

template <>
inline FloatMax PolynomialBaseN <16>::mul_pow (FloatMax x, int exp)
{
	if (x && exp) {
		x = std::ldexp (x, exp * 4);
		if (std::isinf (x))
			throw_DATA_CONVERSION (make_minor_errno (ERANGE));
	}
	return x;
}

template <unsigned BASE>
FloatMax PolynomialBaseN <BASE>::to_float (int exp) const
{
	FloatMax ret = 0;

	const Part* p = parts ();
	if (p < end_) {
		int rm = std::fegetround ();
		std::fesetround (FE_TOWARDZERO);

		try {
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
		} catch (...) {
			std::fesetround (rm);
			throw;
		}
		std::fesetround (rm);
	}

	if (std::isinf (ret))
		throw_DATA_CONVERSION (make_minor_errno (ERANGE));

	return ret;
}

template
FloatMax PolynomialBaseN <10>::to_float (int exp) const;

template
FloatMax PolynomialBaseN <16>::to_float (int exp) const;

}
