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

	template <typename C>
	static void get_char (WideInEx& in, unsigned width, va_list& args)
	{
		C* p = va_arg (args, C*);
		if (!width)
			width = 1;
		WideOutBufT <C> out (p, p + width);
		get_char (in, width, out);
	}

	static void get_char (WideInEx& in, unsigned width, WideOut& out);

	template <typename C>
	static void get_string (WideInEx& in, unsigned width, va_list& args)
	{
		C* p = va_arg (args, C*);
		C* end = width ? p + width : std::numeric_limits <C*>::max ();
		WideOutBufT <C> out (p, end);
		get_string (in, width, out);
	}

	static void get_string (WideInEx& in, unsigned width, WideOut& out);
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