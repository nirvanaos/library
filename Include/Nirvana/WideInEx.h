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

private:
	static int32_t decimal_point (const struct lconv* loc);
	bool is_inf ();
	bool is_nan ();
	bool skip (const std::pair <char, char>* s, size_t cnt);

	template <typename U>
	unsigned get_uint (U& ret, unsigned base, unsigned& zeros);

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

template <typename U>
unsigned WideInEx::get_uint (U& ret, unsigned base, unsigned& zcnt)
{
	U cutoff = std::numeric_limits <U>::max ();
	unsigned cutlim = cutoff % (U)base;
	cutoff /= (U)base;
	U acc = 0;
	unsigned zeros = 0, digits = 0;
	bool over = false;
	for (int32_t c = cur (); c != EOF; c = next ()) {
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

		++digits;
		if (digit == 0)
			++zeros;
		else {
			for (; zeros; zeros--) {
				if (!over && acc < cutoff)
					acc *= base;
				else {
					over = true;
					break;
				}
			}

			if (over || acc > cutoff || (acc == cutoff && digit > cutlim)) {
				over = true;
				++zeros;
			} else {
				acc *= base;
				acc += digit;
			}
		}
	}
	ret = acc;
	zcnt = zeros;
	return digits;
}

template <typename F>
int32_t WideInEx::get_float (F& ret, const struct lconv* loc)
{
	using UInt = typename std::conditional <sizeof (F) == 4, uint32_t, uint64_t>::type;

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
	unsigned digits = 0;
	if (c == '0') {
		c = next ();
		if ('x' == c || 'X' == c) {
			next ();

			UInt whole;
			unsigned zeros;
			unsigned digits = get_uint (whole, 16, zeros);
			if (!digits)
				throw_DATA_CONVERSION (make_minor_errno (EINVAL));
			num = (F)whole;
			if (zeros)
				num *= std::pow ((F)16, (F)zeros);

			c = cur ();
			if (c == dp) {
				next ();
				UInt frac;
				digits = get_uint (frac, 16, zeros);
				digits -= zeros;
				if (digits) {
					F scale = std::pow ((F)16, (F)digits);
					num = (num * scale + (F)frac) / scale;
				}
				c = cur ();
			}

			switch (c) {
				case 'P':
				case 'p':
					next ();
					int exp;
					c = get_int (exp, 10);
					num = std::ldexp (num, exp - 1);
					break;
			}

			ret = sign ? -num : num;
			return c;
		} else
			digits = 1;

	} else if (is_inf ()) {
		ret = sign ? -std::numeric_limits <F>::infinity () : std::numeric_limits <F>::infinity ();
		return cur ();
	} else if (is_nan ()) {
		ret = std::numeric_limits <F>::signaling_NaN ();
		return cur ();
	}

	UInt whole, frac;
	unsigned whole_zeros, frac_digits = 0;
	digits += get_uint (whole, 10, whole_zeros);
	if (!digits)
		throw_DATA_CONVERSION (make_minor_errno (EINVAL));
	c = cur ();
	if (c == dp) {
		next ();
		unsigned zeros;
		frac_digits = get_uint (frac, 10, zeros);
		frac_digits -= zeros;
		c = cur ();
	}
	num = (F)whole;
	unsigned scale_up = whole_zeros + frac_digits;
	if (scale_up) {
		F fscale = std::pow ((F)10, (F)scale_up);
		num *= fscale;
		if (frac_digits) {
			num += frac;
			if (whole_zeros)
				fscale = std::pow ((F)10, (F)frac_digits);
			num /= fscale;
		}
	}

	switch (c) {
		case 'E':
		case 'e':
			next ();
			int exp;
			c = get_int (exp, 10);
			if (std::numeric_limits <F>::radix == 10)
				num = std::scalbn (num, exp);
			else
				num = num * std::pow ((F)10, (F)exp);
			break;
	}
	ret = sign ? -num : num;
	return c;
}

}

#endif
