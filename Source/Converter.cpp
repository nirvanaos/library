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

const char Converter::int_formats_ [] = "diuxXobB";

unsigned Converter::length_flags (WideInEx& fmt)
{
	unsigned flags = 0;
	int c = fmt.cur ();
	switch (c) {
		case 'l':
			flags |= FLAG_LONG;
			c = fmt.next ();
			if (c == 'l') {
				flags |= FLAG_LONG_LONG;
				fmt.next ();
			}
			break;
		case 'h':
			flags |= FLAG_SHORT;
			c = fmt.next ();
			if (c == 'h') {
				flags |= FLAG_CHAR;
				fmt.next ();
			}
			break;
		case 't':
			flags |= (sizeof (ptrdiff_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			fmt.next ();
			break;
		case 'j':
			flags |= (sizeof (intmax_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			fmt.next ();
			break;
		case 'z':
			flags |= (sizeof (size_t) == sizeof (long) ? FLAG_LONG : FLAG_LONG_LONG);
			fmt.next ();
			break;
		case 'L':
			flags |= FLAG_LONG_DOUBLE;
			fmt.next ();
			break;
	}
	return flags;
}

unsigned Converter::int_base (int c, unsigned& flags) noexcept
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
	case 'B':
		base = 2;
		flags |= FLAG_UPPERCASE;
		break;
	case 'u':
		base = 10;
		flags |= FLAG_SIGNED;
		break;
	default:
		base = 0;
	}
	return base;
}

}
