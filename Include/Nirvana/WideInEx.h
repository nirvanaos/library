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
#include <errno.h>

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
	typename std::enable_if <std::is_signed <I>::value>::type
		get_int (I& ret, int base);

	template <typename U> inline
		typename std::enable_if <std::is_unsigned <U>::value>::type
		get_int (U& ret, int base)
	{
		get_int (reinterpret_cast <std::make_signed <U>::type&> (ret), base);
	}

private:
	WideIn& in_;
	size_t pos_;
	int32_t cur_;
};

template <typename I>
typename std::enable_if <std::is_signed <I>::value>::type
WideInEx::get_int (I& ret, int base)
{
	typedef typename std::make_unsigned <I>::type U;

	ret = 0;

	if (base < 0 || base == 1 || base > 36)
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
		next ();
		if ((base == 0 || base == 16) && (cur () == 'x' || cur () == 'X')) {
			next ();
			base = 16;
		} else
			any = 1;

		if (base == 0)
			base = 8;
		c = cur ();
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
	for (acc = 0; (c = cur ()) != EOF; next ()) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && (unsigned)c > cutlim)) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}

	if (any < 0) {
		ret = neg ? std::numeric_limits <I>::min () : std::numeric_limits <U>::max ();
		throw CORBA::DATA_CONVERSION (make_minor_errno (ERANGE));
	} else {
		ret = neg ? -(I)acc : acc;
		if (any == 0)
			throw CORBA::DATA_CONVERSION (make_minor_errno (EINVAL));
	}
}

}

#endif
