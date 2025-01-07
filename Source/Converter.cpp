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
#include <Nirvana/Converter.h>
#include <assert.h>

namespace Nirvana {

const char Converter::int_formats_ [7] = {
	'd', 'i', 'u', 'x', 'X', 'o', 'b'
};
/*
void Converter::skip_space (bool wide, CIn& in, CodePage::_ptr_type loc)
{
	if (wide) {
		for (int c = in.cur (); iswspace (c);)
			c = in.next ();
	} else if (loc) {
		for (int c = in.cur (); loc->get_category (c) == CodePage::CC_SPACE; )
			c = in.next ();
	} else {
		const char* end = s + strlen (s);
		for (;;) {
			const char* pc = s;
			if (!iswspace (utf8_to_utf32 (pc, end)))
				break;
			s = pc;
		}
	}
}
*/
unsigned Converter::strtou (CIn& in)
{
	int c = in.cur ();
	assert (is_digit (c));
	unsigned u = c - '0';
	for (;;) {
		c = in.next ();
		if (is_digit (c)) {
			// TODO: Check overflow
			u = u * 10 + (c - '0');
		} else
			break;
	}
	return u;
}

unsigned Converter::length_flags (CIn& fmt)
{
	unsigned flags = 0;
	int c = fmt.cur ();
	switch (c) {
		case 'l':
			flags |= FLAG_LONG;
			c = fmt.next ();
			if (c == 'l') {
				flags |= FLAG_LONG_LONG;
				c = fmt.next ();
			}
			break;
		case 'h':
			flags |= FLAG_SHORT;
			c = fmt.next ();
			if (c == 'h') {
				flags |= FLAG_CHAR;
				c = fmt.next ();
			}
			break;
		case 't':
			flags |= (sizeof (ptrdiff_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			c = fmt.next ();
			break;
		case 'j':
			flags |= (sizeof (intmax_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			c = fmt.next ();
			break;
		case 'z':
			flags |= (sizeof (size_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			c = fmt.next ();
			break;
		case 'L':
			flags |= FLAG_LONG_DOUBLE;
			break;
	}
	return flags;
}

unsigned Converter::int_base (int c, unsigned& flags)
{
	unsigned base;
	switch (c) {
	case 'x':
		base = 16;
		break;
	case 'X':
		base = 16;
		flags |= FLAG_UPPERCASE;
		break;
	case 'o':
		base = 8;
		break;
	case 'b':
		base = 2;
		break;
	default:
		base = 10;
		if ('u' != c)
			flags |= FLAG_SIGNED;
	}
	return base;
}

}
