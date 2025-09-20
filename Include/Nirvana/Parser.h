/// \file
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

#ifndef NIRVANA_PARSER_H_
#define NIRVANA_PARSER_H_
#pragma once

#include "Converter.h"

namespace Nirvana {

class Parser : private Converter
{
public:
	static void parse (WideIn& in, WideIn& fmt, va_list args, size_t& count, const struct lconv* loc = nullptr);

	template <typename C>
	static size_t parse (const C* buffer, const C* format, ...);

private:
	static const unsigned FLAG_NOASSIGN = 1 << 0;

	static void skip (WideInEx& in, int c);

	template <typename Int>
	static void get_int (WideInEx& in, unsigned base, unsigned flags, va_list& args)
	{
		Int v;
		in.get_int (v, base);
		if (!(flags & FLAG_NOASSIGN))
			*va_arg (args, Int*) = v;
	}

	struct CharSet;

	template <typename C>
	static void get_char (WideInEx& in, unsigned width, unsigned flags,
		va_list& args, const CharSet* set = nullptr)
	{
		if (!width)
			width = 1;

		if (flags & FLAG_NOASSIGN)
			get_char (in, width, nullptr, set);
		else {
			WideOutStrT <C> out (va_arg (args, C*));
			get_char (in, width, &out, set);
		}
	}

	template <typename C>
	static void get_char (WideInEx& in, unsigned width, unsigned flags, const CharSet& set,
		va_list& args)
	{
		if (!width)
			width = std::numeric_limits <size_t>::max ();

		if (flags & FLAG_NOASSIGN)
			get_char (in, width, nullptr, &set);
		else {
			C* p = va_arg (args, C*);
			WideOutStrT <C> out (p);
			get_char (in, width, &out, &set);
			*out.cur_ptr () = 0;
		}
	}

	static void get_char (WideInEx& in, unsigned width, WideOut* out, const CharSet* set = nullptr);

	template <typename C>
	static void get_string (WideInEx& in, unsigned width, unsigned flags, va_list& args)
	{
		if (flags & FLAG_NOASSIGN)
			get_string (in, width, nullptr);
		else {
			C* p = va_arg (args, C*);
			WideOutStrT <C> out (p);
			get_string (in, width, &out);
			*out.cur_ptr () = 0;
		}
	}

	static void get_string (WideInEx& in, unsigned width, WideOut* out);
};

template <typename C>
size_t Parser::parse (const C* buffer, const C* format, ...)
{
	WideInStrT <C> in (buffer);
	WideInStrT <C> fmt (format);
	va_list arglist;
	va_start (arglist, format);
	size_t cnt;
	parse (in, fmt, arglist, cnt);
	va_end (arglist);
	return cnt;
}

}

#endif