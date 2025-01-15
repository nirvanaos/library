/// \file
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_WIDEINEX_H_
#define NIRVANA_WIDEINEX_H_
#pragma once

#include "WideIn.h"
#include "throw_exception.h"
#include <limits>
#include <cmath>
#include <cfenv>

namespace Nirvana {

class WideInEx
{
public:
	WideInEx (WideIn& in);

	int32_t cur () const noexcept
	{
		return cur_;
	}

	size_t pos () const noexcept
	{
		return pos_;
	}

	int32_t next ();

	int32_t skip_space ();

	template <typename I>
	typename std::enable_if <std::is_signed <I>::value, int32_t>::type
		get_int (I& ret, unsigned base);

	template <typename U> inline
		typename std::enable_if <std::is_unsigned <U>::value, int32_t>::type
		get_int (U& ret, unsigned base)
	{
		return get_int (reinterpret_cast <std::make_signed <U>::type&> (ret), base);
	}

	template <typename F>
	int32_t get_float (F& ret, const struct lconv* loc = nullptr);

	template <unsigned base, typename F>
	int32_t get_float (F& ret, int32_t dec_pt, bool no_check);

	template <unsigned base, typename F>
	unsigned get_float_whole (F& ret, bool drop_tz);

private:
	static int32_t decimal_point (const struct lconv* loc);
	bool is_inf ();
	bool is_nan ();
	bool skip (const std::pair <char, char>* s, size_t cnt);

	unsigned get_uint (UWord& ret, unsigned base, bool drop_tz);

	struct PolyPart
	{
		UWord u;
		unsigned num_digits;

		template <typename F>
		F get (unsigned base, unsigned power) const noexcept;
	};

private:
	WideIn& in_;
	size_t pos_;
	int32_t cur_;
};

template <typename I>
typename std::enable_if <std::is_signed <I>::value, int32_t>::type
WideInEx::get_int (I& ret, unsigned base)
{
	typedef typename std::make_unsigned <I>::type U;

	ret = 0;

	if (base == 1 || base > 36)
		throw_BAD_PARAM (make_minor_errno (EINVAL));

	// Skip white space and pick up leading +/- sign if any.
	// If base is 0, allow 0x for hex and 0 for octal, else
	// assume decimal; if base is already 16, allow 0x.
	int32_t c = skip_space ();

	bool neg = false;
	if (c == '-') {
		neg = true;
		c = next ();
	} else if (c == '+')
		c = next ();

	int any = 0;

	if (c == '0') {
		c = next ();
		if ((base == 0 || base == 16) && (c == 'x' || c == 'X')) {
			c = next ();
			base = 16;
		} else
			any = 1;

		if (base == 0)
			base = 8;
	}
	if (base == 0)
		base = 10;

	/*
	 * Compute the cutoff value between legal numbers and illegal
	 * numbers.  That is the largest legal value, divided by the
	 * base.  An input number that is greater than this value, if
	 * followed by a legal input character, is too big.  One that
	 * is equal to this value may be valid or not; the limit
	 * between valid and invalid numbers is then based on the last
	 * digit.  For instance, if the range for longs is
	 * [-2147483648..2147483647] and the input base is 10,
	 * cutoff will be set to 214748364 and cutlim to either
	 * 7 (neg==0) or 8 (neg==1), meaning that if we have accumulated
	 * a value > 214748364, or equal but the next digit is > 7 (or 8),
	 * the number is too big, and we will return a range error.
	 *
	 * Set any if any `digits' consumed; make it negative to indicate
	 * overflow.
	 */
	U cutoff = neg ? -std::numeric_limits <I>::min () : std::numeric_limits <U>::max ();
	unsigned cutlim = cutoff % (U)base;
	cutoff /= (U)base;
	U acc;
	for (acc = 0; c != EOF; c = next ()) {
		unsigned digit;
		if (c >= '0' && c <= '9')
			digit = c - '0';
		else if (c >= 'A' && c <= 'Z')
			digit = c - ('A' - 10);
		else if (c >= 'a' && c <= 'z')
			digit = c - ('a' - 10);
		else
			break;
		if (digit >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && digit > cutlim))
			any = -1;
		else {
			any = 1;
			acc *= base;
			acc += digit;
		}
	}

	if (any < 0) {
		ret = neg ? std::numeric_limits <I>::min () : std::numeric_limits <U>::max ();
		throw_DATA_CONVERSION (make_minor_errno (ERANGE));
	} else {
		ret = neg ? -(I)acc : acc;
		if (any == 0)
			throw_DATA_CONVERSION (make_minor_errno (EINVAL));
	}

	return c;
}

template <unsigned base, typename F>
unsigned WideInEx::get_float_whole (F& num, bool drop_tz)
{
	static_assert (base == 10 || base == 16, "Unexpected base");
	static_assert (std::numeric_limits <F>::radix == 2, "Unexpected radix");

	const size_t MAX_WORDS = (base == 10) ?
		((std::max (-std::numeric_limits <F>::min_exponent10, std::numeric_limits <F>::max_exponent10)
			+ std::numeric_limits <UWord>::digits10 - 1) / std::numeric_limits <UWord>::digits10)
	:
		((std::max (-std::numeric_limits <F>::min_exponent, std::numeric_limits <F>::max_exponent)
			+ 3) / 4 / (sizeof (UWord) * 2));

	// Get polynomial

	PolyPart poly [MAX_WORDS];
	PolyPart* poly_end = poly;
	unsigned total_digits = 0;
	bool overflow = false;

	UWord u;
	while (unsigned digits = get_uint (u, base, drop_tz)) {
		if (poly_end < std::end (poly)) {
			poly_end->u = u;
			poly_end->num_digits = digits;
			total_digits += digits;
			++poly_end;
		} else
			overflow = true;
	}

	// Check for overflow
	if (overflow && !drop_tz) {
		num = std::numeric_limits <F>::infinity ();
		return std::numeric_limits <unsigned>::max ();
	}

	// Convert polinomial to float
	if (total_digits) {
		unsigned power = total_digits;
		const PolyPart* p = poly;
		power -= p->num_digits;
		F f = p->get <F> (base, power);
		while (poly_end > ++p) {
			power -= p->num_digits;
			f += p->get <F> (base, power);
		}
		num = f;
	} else
		num = 0;

	return total_digits;
}

template <typename F>
F WideInEx::PolyPart::get (unsigned base, unsigned power) const noexcept
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

template <unsigned base, typename F>
int32_t WideInEx::get_float (F& num, int32_t dec_pt, bool no_check)
{
	unsigned any_digits;
	
	int rm = std::fegetround ();
	std::fesetround (FE_TOWARDZERO);

	try {

		any_digits = get_float_whole <base> (num, false);

		int32_t c = cur ();
		if (c == dec_pt) {
			next ();
			F frac;
			unsigned frac_digits = get_float_whole <base> (frac, true);
			if (frac_digits && any_digits != std::numeric_limits <unsigned>::max ()) {
				std::fesetround (FE_TONEAREST);
				frac *= std::pow ((F)base, -(F)frac_digits);
				assert (frac < 1);
				num += frac;
				any_digits += frac_digits;
			}
			c = cur ();
		}

		if (!any_digits && !no_check)
			throw_DATA_CONVERSION (make_minor_errno (EINVAL));

		std::fesetround (rm);

		return c;

	} catch (...) {
		std::fesetround (rm);
		throw;
	}
}

template <typename F>
int32_t WideInEx::get_float (F& ret, const struct lconv* loc)
{
	bool sign = false;
	auto c = skip_space ();
	switch (c) {
		case '-':
			sign = true;
#ifdef NIRVANA_C17
			[[fallthrough]];
#endif
		case '+':
			c = next ();
	}

	const int32_t dp = decimal_point (loc);

	F num;
	bool some_digits = false;
	if (c == '0') {
		c = next ();
		if ('x' == c || 'X' == c) {
			next ();
			c = get_float <16> (num, dp, false);
			switch (c) {
				case 'P':
				case 'p':
					next ();
					int exp;
					c = get_int (exp, 10);
					if (num != std::numeric_limits <F>::infinity ())
						num = std::ldexp (num, exp - 1);
					break;
			}

			goto end;
		} else
			some_digits = true;

	} else if (is_inf ()) {
		ret = sign ? -std::numeric_limits <F>::infinity () : std::numeric_limits <F>::infinity ();
		return cur ();
	} else if (is_nan ()) {
		ret = std::numeric_limits <F>::signaling_NaN ();
		return cur ();
	}

	c = get_float <10> (num, dp, some_digits);
	switch (c) {
		case 'E':
		case 'e':
			next ();
			int exp;
			c = get_int (exp, 10);
			if (num != std::numeric_limits <F>::infinity ()) {
				if (std::numeric_limits <F>::radix == 10)
					num = std::scalbn (num, exp);
				else
					num = num * std::pow ((F)10, (F)exp);
			}
			break;
	}

end:
	ret = sign ? -num : num;
	if (num == std::numeric_limits <F>::infinity ())
		throw_DATA_CONVERSION (make_minor_errno (ERANGE));
	return c;
}

}

#endif
