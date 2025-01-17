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
#include "../../pch/pch.h"
#include <Nirvana/WideInEx.h>
#include <cmath>
#include <cfenv>
#include <Nirvana/locale.h>
#include <Nirvana/Polynomial.h>

namespace Nirvana {

WideInEx::WideInEx (WideIn& in) :
	in_ (in),
	pos_ (0)
{
	cur_ = in_.get ();
}

int32_t WideInEx::next ()
{
	if (EOF != cur_) {
		cur_ = in_.get ();
		++pos_;
	}
	return cur_;
}

int32_t WideInEx::skip_space ()
{
	int32_t c = cur ();
	while (iswspace (c))
		c = next ();
	return c;
}

int32_t WideInEx::decimal_point (const struct lconv* loc)
{
	if (!loc)
		return '.';
	else {
		WideInStrUTF8 in (loc->decimal_point);
		return in.get ();
	}
}

bool WideInEx::skip (const std::pair <char, char>* s, size_t cnt)
{
	int32_t c = cur ();
	if (s->first == c || s->second == c) {
		while (--cnt) {
			c = next ();
			++s;
			if (s->first != c && s->second != c)
				throw_DATA_CONVERSION (make_minor_errno (EINVAL));
		}
		next ();
		return true;
	}
	return false;
}

bool WideInEx::is_inf ()
{
	static const std::pair <char, char> inf [] = {
		{ 'i', 'I'},
		{ 'n', 'N'},
		{ 'f', 'F'}
	};

	if (skip (inf, std::size (inf))) {
		static const std::pair <char, char> inity [] = {
			{ 'i', 'I'},
			{ 'n', 'N'},
			{ 'i', 'I'},
			{ 't', 'T'},
			{ 'y', 'Y'}
		};

		skip (inity, std::size (inity));

		return true;
	}
	return false;
}

bool WideInEx::is_nan ()
{
	static const std::pair <char, char> nan [] = {
		{ 'n', 'N'},
		{ 'a', 'A'},
		{ 'n', 'N'}
	};

	return skip (nan, std::size (nan));
}

int32_t WideInEx::get_float (long double& ret, const struct lconv* loc)
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

	long double num;
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
					if (num != std::numeric_limits <long double>::infinity ())
						num = std::ldexp (num, exp - 1);
					break;
			}

			goto end;
		} else
			some_digits = true;

	} else if (is_inf ()) {
		ret = sign ? -std::numeric_limits <long double>::infinity () :
			std::numeric_limits <long double>::infinity ();
		return cur ();
	} else if (is_nan ()) {
		ret = std::numeric_limits <long double>::signaling_NaN ();
		return cur ();
	}

	c = get_float <10> (num, dp, some_digits);
	switch (c) {
		case 'E':
		case 'e':
			next ();
			int exp;
			c = get_int (exp, 10);
			if (num != std::numeric_limits <long double>::infinity ()) {
				if (std::numeric_limits <long double>::radix == 10)
					num = std::scalbn (num, exp);
				else
					num = num * std::pow ((long double)10, (long double)exp);
			}
			break;
	}

end:
	ret = sign ? -num : num;
	if (num == std::numeric_limits <long double>::infinity ())
		throw_DATA_CONVERSION (make_minor_errno (ERANGE));
	return c;
}

static unsigned get_part (WideInEx& in, PolynomialBase::Part& part, unsigned base, bool drop_tz,
	bool& not_last)
{
	UWord cutoff = std::numeric_limits <UWord>::max ();
	unsigned cutlim = cutoff % (UWord)base;
	cutoff /= (UWord)base;
	UWord acc = 0;
	unsigned digits = 0, zeros = 0;
	UWord tzdiv = 1;
	bool overflow = false;
	for (int32_t c = in.cur (); c != EOF; c = in.next ()) {
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
			tzdiv *= base;
		}
	}
	if (drop_tz && !overflow) {
		part.num_digits = digits - zeros;
		acc /= tzdiv;
	} else
		part.num_digits = digits;
	part.u = acc;
	not_last = overflow;
	return digits;
}

template <class Poly>
unsigned get_parts (WideInEx& in, Poly& poly, unsigned base, bool drop_tz)
{
	unsigned digs = 0;
	
	bool not_last;
	do {
		typename Poly::Part part;
		digs += get_part (in, part, base, drop_tz, not_last);
		poly.add (part);
	} while (not_last);

	return digs;
}

template <unsigned BASE> inline
int32_t WideInEx::get_float (long double& num, int32_t dec_pt, bool no_check)
{
	num = 0;

	const unsigned MAX_DIGITS = (BASE == 10) ?
		std::max (-std::numeric_limits <long double>::min_exponent10,
			std::numeric_limits <long double>::max_exponent10)
		:
		(std::max (-std::numeric_limits <long double>::min_exponent,
			std::numeric_limits <long double>::max_exponent)
			* log2_ceil (std::numeric_limits <long double>::radix) + 3) / 4;

	using Poly = Polynomial <BASE, MAX_DIGITS>;
	Poly poly;

	unsigned all_digits = get_parts (*this, poly, BASE, false);

	bool overflow = poly.overflow ();
	
	if (cur () == dec_pt) {
		next ();
		int exp = poly.exp ();
		all_digits += get_parts (*this, poly, BASE, true);
		poly.exp (exp);
	}

	if (!all_digits && !no_check)
		throw_DATA_CONVERSION (make_minor_errno (EINVAL));

	if (overflow)
		num = std::numeric_limits <long double>::infinity ();
	else
		num = poly.to_float ();

	return cur ();
}

}
