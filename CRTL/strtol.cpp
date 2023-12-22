/*
* Nirvana C runtime library.
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
#include "pch/pch.h"
#include <Nirvana/UTF8.h>
#include <Nirvana/nls.h>
#include <wctype.h>

using namespace Nirvana;

namespace CRTL {

const char* skip_space (const char* s, CodePage::_ptr_type loc)
{
	if (loc) {
		while (loc->get_category (*s) == CodePage::CC_SPACE) {
			++s;
		}
	} else {
		const char* end = s + strlen (s);
		for (;;) {
			const char* pc = s;
			if (!iswspace (utf8_to_utf32 (pc, end)))
				break;
			s = pc;
		}
	}
	return s;
}

const wchar_t* skip_space (const wchar_t* s)
{
	while (iswspace (*s)) {
		++s;
	}
	return s;
}

template <typename I>
I strtoi (const char* nptr, char** endptr, int base, CodePage::_ptr_type loc)
{
	typedef typename std::make_unsigned <I>::type U;

	const char* s = nptr;

	if (base < 0 || base == 1 || base > 36) {
		errno = EINVAL;
		return 0;
	}

	/*
	 * Skip white space and pick up leading +/- sign if any.
	 * If base is 0, allow 0x for hex and 0 for octal, else
	 * assume decimal; if base is already 16, allow 0x.
	 */
	try {
		s = skip_space (s, loc);
	} catch (const CORBA::SystemException& ex) {
		errno = get_minor_errno (ex.minor ());
		return 0;
	}
	unsigned c = *s++;
	bool neg = false;
	if (c == '-') {
		neg = true;
		c = *s++;
	} else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X')) {
		c = s [1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;

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
	U cutoff = neg ? -std::numeric_limits <I>::min () : std::numeric_limits <I>::max ();
	unsigned cutlim = cutoff % (U)base;
	cutoff /= (U)base;
	U acc;
	int any;
	for (acc = 0, any = 0;; c = *s++) {
		if (c >= '0' && c <= '9')
			c -= '0';
		else if (c >= 'A' && c <= 'Z')
			c -= 'A' - 10;
		else if (c >= 'a' && c <= 'z')
			c -= 'a' - 10;
		else
			break;
		if (c >= (unsigned)base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim)) {
			any = -1;
		} else {
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (endptr)
		*endptr = (char*)(any ? (char*)s - 1 : nptr);
	I ret;
	if (any < 0) {
		ret = neg ? std::numeric_limits <I>::min () : std::numeric_limits <I>::max ();
		errno = ERANGE;
	} else 
		ret = neg ? -(I)acc : acc;
	return ret;
}

}

extern "C"
long strtol (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long> (s, ptr, base, nullptr);
}

extern "C"
long long strtoll (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long long> (s, ptr, base, nullptr);
}

extern "C"
unsigned long strtoul (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long> (s, ptr, base, nullptr);
}

extern "C"
unsigned long long strtoull (const char* __restrict s, char** __restrict ptr, int base)
{
	return CRTL::strtoi <long long> (s, ptr, base, nullptr);
}

extern "C"
int atoi (const char* s)
{
	return CRTL::strtoi <int> (s, nullptr, 10, nullptr);
}

extern "C"
long atol (const char* s)
{
	return strtol (s, NULL, 10);
}
