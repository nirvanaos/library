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
#include <Nirvana/locale.h>

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

unsigned WideInEx::get_uint (UWord& ret, unsigned base, bool drop_tz)
{
	UWord cutoff = std::numeric_limits <UWord>::max ();
	unsigned cutlim = cutoff % (UWord)base;
	cutoff /= (UWord)base;
	UWord acc = 0;
	unsigned digits = 0, zeros = 0;
	UWord tzdiv = 1;
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

		if (acc > cutoff || (acc == cutoff && digit > cutlim)) {
			zeros = 0;
			tzdiv = 1;
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
	if (drop_tz) {
		digits -= zeros;
		acc /= tzdiv;
	}
	ret = acc;
	return digits;
}

}
