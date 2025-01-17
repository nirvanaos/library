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
#include "../../pch/pch.h"
#include <Nirvana/Parser.h>
#include <wctype.h>
#include <unordered_set>

namespace Nirvana {

void Parser::parse (WideIn& in0, WideIn& fmt0, va_list args, size_t& count, const struct lconv* loc)
{
	WideInEx fmt (fmt0);
	WideInEx in (in0);

	count = 0;
	for (int32_t c; (c = fmt.cur ()) != EOF;) {
		if (c != '%') {
			if (iswspace (c)) {
				in.skip_space ();
				c = fmt.skip_space ();
			} else {
				skip (in, c);
				c = fmt.next ();
			}
		} else {
			c = fmt.next ();
			if (c == '%') {
				skip (in, '%');
				c = fmt.next ();
			} else {

				// flags
				unsigned flags = 0;
				if (c == '*') {
					flags |= FLAG_NOASSIGN;
					c = fmt.next ();
				}

				// width
				unsigned width = 0;
				if (is_digit (c))
					c = fmt.get_int (width, 10);

				flags |= length_flags (fmt);
				c = fmt.cur ();

				if (std::find (int_formats_, std::end (int_formats_), c) != std::end (int_formats_)) {
					// Integer format

					unsigned base = int_base (c, flags);
					if (flags & FLAG_SIGNED) {
						// signed
						if (flags & FLAG_LONG_LONG)
							in.get_int (*va_arg (args, long long*), base);
						else if (flags & FLAG_LONG)
							in.get_int (*va_arg (args, long*), base);
						else if (flags & FLAG_SHORT)
							in.get_int (*va_arg (args, short*), base);
						else if (flags & FLAG_CHAR)
							in.get_int (*va_arg (args, signed char*), base);
						else
							in.get_int (*va_arg (args, int*), base);
					} else {
						// unsigned
						if (flags & FLAG_LONG_LONG)
							in.get_int (*va_arg (args, unsigned long long*), base);
						else if (flags & FLAG_LONG)
							in.get_int (*va_arg (args, unsigned long*), base);
						else if (flags & FLAG_SHORT)
							in.get_int (*va_arg (args, unsigned short*), base);
						else if (flags & FLAG_CHAR)
							in.get_int (*va_arg (args, unsigned char*), base);
						else
							in.get_int (*va_arg (args, unsigned int*), base);
					}
				} else {
					switch (c) {
						case 'a':
						case 'A':
						case 'e':
						case 'E':
						case 'f':
						case 'F':
						case 'g':
						case 'G':
						{
							long double f;
							in.get_float (f, loc);
							if (sizeof (long double) > sizeof (double) && (flags & FLAG_LONG_DOUBLE))
								*va_arg (args, long double*) = f;
							else if (sizeof (double) > sizeof (float) && (flags & FLAG_LONG))
								*va_arg (args, double*) = (double)f;
							else
								*va_arg (args, float*) = (float)f;
						} break;

						case 'c':
							if (flags & FLAG_LONG)
								get_char <wchar_t> (in, width, args);
							else
								get_char <char> (in, width, args);
							break;

						case 's':
							if (flags & FLAG_LONG)
								get_string <wchar_t> (in, width, args);
							else
								get_string <char> (in, width, args);
							break;

						case 'n':
							*va_arg (args, int*) = (int)in.pos ();
							break;

						case '[': {
							c = fmt.next ();
							bool inv = false;
							if ('^' == c) {
								inv = true;
								c = fmt.next ();
							}
							std::unordered_set <int32_t> chars;
							while (']' != c && EOF != c) {
								chars.insert (c);
								c = fmt.next ();
							}
							if (']' != c)
								throw_BAD_PARAM (make_minor_errno (EILSEQ));
							while (in.cur () != EOF && ((chars.find (in.cur ()) != chars.end ()) ^ inv)) {
								in.next ();
							}
						} break;

						default:
							throw_BAD_PARAM (make_minor_errno (EILSEQ));
							break;
					}
				}

				++count;
				c = fmt.next ();
			}
		}
	}
}

void Parser::skip (WideInEx& in, int c)
{
	if (in.cur () != c)
		throw CORBA::BAD_PARAM (make_minor_errno (EILSEQ));
	in.next ();
}

void Parser::get_char (WideInEx& in, unsigned width, WideOut& out)
{
	int32_t c = in.cur ();
	while (c != EOF && !width--) {
		out.put (c);
		c = in.next ();
	}
}

void Parser::get_string (WideInEx& in, unsigned width, WideOut& out)
{
	if (!width)
		width = std::numeric_limits <unsigned>::max ();
	int32_t c = in.cur ();
	while (c != EOF && !iswspace (c) && !width--) {
		out.put (c);
		c = in.next ();
	}
}

}
