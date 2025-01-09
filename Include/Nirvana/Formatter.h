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

// Based on https://github.com/mpaland/printf code.

#ifndef NIRVANA_FORMATTER_H_
#define NIRVANA_FORMATTER_H_
#pragma once

#include "Converter.h"
#include "WideOut.h"
#include <stdarg.h>

struct lconv;

namespace Nirvana {

/// Universal formatter for C printf-like functions.
class Formatter : private Converter
{
public:
	/// Generalized C-style formatting function.
	/// As it intended to C formatting, it does not throw exceptions
	/// but sets `errno` codes on error instead.
	/// 
	/// \param wide `true` if \p fmt and \p out are wide character sequences.
	/// \param fmt Format string input.
	/// \param args Arguments for formatting.
	/// \param out Output stream for formatted output.
	/// \param loc `struct lconv` pointer or nullptr.
	/// \returns The number of characters that would have been written if n had been sufficiently large, not counting the terminating null character.
	///          If an encoding error occurs, a negative number is returned.
	static int format (WideIn& fmt, va_list args, WideOut& out, const struct lconv* loc = nullptr) noexcept;

	template <class Cont>
	static int append_format_v (Cont& cont, const typename Cont::value_type* format, va_list arglist);

	template <class Cont>
	static int append_format (Cont& cont, const typename Cont::value_type* format, ...);

	template <class C>
	static int snprintf (C* buf, size_t size, const C* format, ...);

private:
	static unsigned out_rev (char* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out);

	static size_t ntoa_format (char* buf, size_t len, size_t max_len, bool negative, unsigned base,
		unsigned prec, unsigned width, unsigned flags);

	template <typename U>
	static unsigned ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width,
		unsigned flags, WideOutEx& out);

	static unsigned ftoa (double value, unsigned int prec, unsigned int width, unsigned int flags,
		const struct lconv* loc, WideOutEx& out);
	static unsigned etoa (double value, unsigned int prec, unsigned int width, unsigned int flags,
		const struct lconv* loc, WideOutEx& out);

	template <class C>
	static unsigned out_string (const C* p, unsigned l, const unsigned width,
		const unsigned precision, unsigned flags, WideOutEx& out)
	{
		if ((flags & FLAG_PRECISION) && l > precision)
			l = precision;

		return out_buf (p, l, width, flags & ~FLAG_ZEROPAD, out);
	}

	template <class C>
	static unsigned out_buf (const C* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out);

	static unsigned out_buf_pre (unsigned len, unsigned width, unsigned flags, WideOutEx& out);
	static unsigned out_buf_post (unsigned cnt, unsigned width, unsigned flags, WideOutEx& out);

	template <class C>
	static unsigned get_len (const C* buf, size_t len)
	{
		return (unsigned)len;
	}

	static unsigned get_len (const char* buf, size_t len)
	{
		WideInBufUTF8 in (buf, buf + len);
		unsigned cnt = 0;
		while (in.get () != EOF)
			++cnt;
		return cnt;
	}

private:
	struct Flag
	{
		int cflag;
		unsigned uflag;
	};

	// Flags 0..7 are defined in Converter base class.
	static const unsigned FLAG_ZEROPAD = 1 << 8;
	static const unsigned FLAG_LEFT = 1 << 9;
	static const unsigned FLAG_PLUS = 1 << 10;
	static const unsigned FLAG_SPACE = 1 << 11;
	static const unsigned FLAG_HASH = 1 << 12;
	static const unsigned FLAG_PRECISION = 1 << 13;
	static const unsigned FLAG_ADAPT_EXP = 1 << 14;

	// 'ftoa' conversion buffer size, this must be big enough to hold one converted
	// float number including padded zeros (dynamically created on stack)
	// default: 32 byte
	static const size_t PRINTF_FTOA_BUFFER_SIZE = 32;

	static const Flag flags_ [5];

	// define the largest float suitable to print with %f
	// default: 1e9
	static const double PRINTF_MAX_FLOAT; //  1e9

	// define the default floating point precision
	// default: 6 digits
	static const unsigned PRINTF_DEFAULT_FLOAT_PRECISION = 6;
};

template <typename U>
unsigned Formatter::ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width, unsigned flags, WideOutEx& out)
{
	char buf [sizeof (value) * 8 + 4];
	size_t len = 0;

	// no hash for 0 values
	if (!value)
		flags &= ~FLAG_HASH;

	// write if precision != 0 and value is != 0
	if (!(flags & FLAG_PRECISION) || value) {
		do {
			const char digit = (char)(value % base);
			buf [len++] = digit < 10 ? '0' + digit : (flags & FLAG_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < sizeof (buf)));
	}

	len = ntoa_format (buf, len, sizeof (buf), negative, base, prec, width, flags);
	return out_rev (buf, len, width, flags, out);
}

template <class C>
unsigned Formatter::out_buf (const C* buf, size_t size, unsigned width, unsigned flags, WideOutEx& out)
{
	unsigned len = get_len (buf, size);

	// pad spaces up to given width
	unsigned cnt = out_buf_pre (len, width, flags, out);

	// Out narrow string as UTF8 in case of UTF8 decimal point in lconv.
	WideInBufT <C> in (buf, buf + len);
	for (;;) {
		auto c = in.get ();
		if (c == EOF)
			break;
		out.put (c);
		++cnt;
	}

	// append pad spaces up to given width
	return out_buf_post (cnt, width, flags, out);
}

template <class Cont>
int Formatter::append_format_v (Cont& cont, const typename Cont::value_type* format, va_list arglist)
{
	typedef typename Cont::value_type CType;
	WideInStrT <CType> fmt (format);
	WideOutContainerT <Cont> out (cont);
	return Formatter::format (fmt, arglist, out);
}

template <class Cont>
int Formatter::append_format (Cont& cont, const typename Cont::value_type* format, ...)
{
	va_list arglist;
	va_start (arglist, format);
	int cnt = append_format_v (cont, format, arglist);
	va_end (arglist);
	return cnt;
}

template <class C>
int Formatter::snprintf (C* buf, size_t size, const C* format, ...)
{
	WideInStrT <C> fmt (format);
	WideOutBufT <C> out (buf, size);
	va_list arglist;
	va_start (arglist, format);
	int cnt = Formatter::format (fmt, arglist, out);
	va_end (arglist);
	return cnt;
}

}

#endif
