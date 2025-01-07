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
#include <Nirvana/Parser.h>
#include <ctype.h>
#include <wctype.h>

namespace Nirvana {

int Parser::parse (bool wide, CIn& in, CIn& fmt, va_list args, CodePage::_ptr_type loc)
{
	try {
		int count = 0;

		for (int c; (c = fmt.cur ());) {
			if (c != '%') {
				if (wide) {
					if (iswspace (c)) {
						if (iswspace (in.cur ()))
							while (iswspace (in.next ()))
								;
						while (iswspace (fmt.next ()))
							;
						continue;
					}
				} else {
					if (isspace (c)) {
						if (isspace (in.cur ()))
							while (isspace (in.next ()))
								;
						while (isspace (fmt.next ()))
							;
						continue;
					}
				}
				skip (in, c);
				fmt.next ();
			} else {
				c = fmt.next ();
				if (c == '%') {
					skip (in, '%');
					fmt.next ();
				} else {

					// flags
					unsigned flags = 0;
					if (c == '*') {
						flags |= FLAG_NOASSIGN;
						c = fmt.next ();
					}

					// width
					unsigned width = 0;
					if (is_digit (c)) {
						width = strtou (fmt);
						c = fmt.cur ();
					}

					flags |= length_flags (fmt);
					c = fmt.cur ();

					if (std::find (int_formats_, std::end (int_formats_), c) != std::end (int_formats_)) {
						// Integer format

						unsigned base = int_base (c, flags);
					} else {

					}
				}
			}
		}

		return count;

	} catch (const CORBA::SystemException& ex) {
		int err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		errno = err;
	} catch (...) {
		errno = EINVAL;
	}
	return -1;
}

void Parser::skip (CIn& in, int c)
{
	if (in.cur () != c)
		throw CORBA::BAD_PARAM (make_minor_errno (EILSEQ));
	in.next ();
}

}
