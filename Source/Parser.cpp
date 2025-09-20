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

struct Parser::CharSet
{
	std::unordered_set <int32_t> chars;
	bool inv;

	bool acceptable (int32_t c) const noexcept
	{
		return ((chars.find (c) != chars.end ()) ^ inv);
	}

};

void Parser::parse (WideIn& in0, WideIn& fmt0, va_list args, size_t& count, const struct lconv* loc)
{
	WideInEx fmt (fmt0);
	WideInEx in (in0);

	count = 0;
	if (in.cur () == EOF)
		return;
		
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

				if (strchr (int_formats_, c)) {
					// Integer format

					unsigned base = int_base (c, flags);
					if (flags & FLAG_SIGNED) {
						// signed
						if (flags & FLAG_LONG_LONG)
							get_int <long long> (in, base, flags, args);
						else if (flags & FLAG_LONG)
							get_int <long> (in, base, flags, args);
						else if (flags & FLAG_SHORT)
							get_int <short> (in, base, flags, args);
						else if (flags & FLAG_CHAR)
							get_int <signed char> (in, base, flags, args);
						else
							get_int <int> (in, base, flags, args);
					} else {
						// unsigned
						if (flags & FLAG_LONG_LONG)
							get_int <unsigned long long> (in, base, flags, args);
						else if (flags & FLAG_LONG)
							get_int <unsigned long> (in, base, flags, args);
						else if (flags & FLAG_SHORT)
							get_int <unsigned short> (in, base, flags, args);
						else if (flags & FLAG_CHAR)
							get_int <unsigned char> (in, base, flags, args);
						else
							get_int <unsigned int> (in, base, flags, args);
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
							FloatMax f;
							in.get_float (f, loc);
							if (!(flags & FLAG_NOASSIGN)) {
								if (sizeof (long double) > sizeof (double) && (flags & FLAG_LONG_DOUBLE))
									*va_arg (args, long double*) = f;
								else if (sizeof (double) > sizeof (float) && (flags & FLAG_LONG))
									*va_arg (args, double*) = (double)f;
								else
									*va_arg (args, float*) = (float)f;
							}
						} break;

						case 'c':
							if (flags & FLAG_LONG)
								get_char <wchar_t> (in, width, flags, args);
							else
								get_char <char> (in, width, flags, args);
							break;

						case 's':
							if (flags & FLAG_LONG)
								get_string <wchar_t> (in, width, flags, args);
							else
								get_string <char> (in, width, flags, args);
							break;

						case 'n':
							if (!(flags & FLAG_NOASSIGN))
								*va_arg (args, int*) = (int)in.pos ();
							break;

						case '[': {
							c = fmt.next ();
							CharSet set;
							set.inv = false;
							if ('^' == c) {
								set.inv = true;
								c = fmt.next ();
							}
							while (']' != c && EOF != c) {
								set.chars.insert (c);
								c = fmt.next ();
							}
							if (']' != c)
								throw_BAD_PARAM (make_minor_errno (EILSEQ));
							
							if (flags & FLAG_LONG)
								get_char <wchar_t> (in, width, flags, set, args);
							else
								get_char <char> (in, width, flags, set, args);
						} break;

						default:
							throw_BAD_PARAM (make_minor_errno (EILSEQ));
							break;
					}
				}

				if (!(flags & FLAG_NOASSIGN))
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

void Parser::get_char (WideInEx& in, unsigned width, WideOut* out, const CharSet* set)
{
	assert (width);
	int32_t c = in.cur ();
	while (c != EOF && (!set || set->acceptable (c)) && width--) {
		if (out)
			out->put (c);
		c = in.next ();
	}
}

void Parser::get_string (WideInEx& in, unsigned width, WideOut* out)
{
	if (!width)
		width = std::numeric_limits <unsigned>::max ();
	int32_t c = in.cur ();
	while (c != EOF && !iswspace (c) && width--) {
		if (out)
			out->put (c);
		c = in.next ();
	}
}

}
