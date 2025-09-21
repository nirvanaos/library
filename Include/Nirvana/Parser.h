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
	/// @brief Generalized C-style scan function.
	/// @param in    Input stream.
	/// @param fmt   Format stream.
	/// @param args  Arguments to store scanned data.
	/// @param count [out] Count of scanned fields.
	/// @param loc   `struct lconv` pointer or nullptr.
	/// @return `true` if all format specifiers were processed, otherwise `false`.
	static bool parse (WideIn& in, WideIn& fmt, va_list args, size_t& count, const struct lconv* loc = nullptr);

	template <typename C>
	static size_t parse (const C* buffer, const C* format, ...);

private:
	// Flags 0..7 are defined in Converter base class.
	static const unsigned FLAG_NOASSIGN = 1 << 8;
	static const unsigned FLAG_MALLOC = 1 << 9;

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

	static void get_char (WideInEx& in, unsigned width, WideOut& out, const CharSet* set = nullptr);

	template <typename C>
	static void get_char (WideInEx& in, unsigned width, unsigned flags, va_list& args);

	template <typename C>
	static void get_char (WideInEx& in, unsigned width, unsigned flags, const CharSet& set,
		va_list& args);

	template <typename C> class BufMalloc;
	static size_t size_alloc (size_t size_req) noexcept;
	class ByteOutMalloc;

	template <typename C>	class WideOutMalloc;
	class WideOutMallocUTF8;

	template <typename C>
	using WideOutMallocT = typename std::conditional <std::is_same <char, C>::value,
		WideOutMallocUTF8, WideOutMalloc <C> >::type;

	template <typename C> class CharOut;	

	template <typename C>
	static void get_string (WideInEx& in, unsigned width, unsigned flags, va_list& args);

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