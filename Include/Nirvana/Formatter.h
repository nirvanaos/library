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
#ifndef NIRVANA_FORMATTER_H_
#define NIRVANA_FORMATTER_H_
#pragma once

#include "Converter.h"

struct lconv;

namespace Nirvana {

/// Universal formatter for C printf-like functions.
class Formatter : private Converter
{
public:
	/// Generalized C-style formatting function.
	/// 
	/// \param wide `true` if \p fmt and \p out are wide character sequences.
	/// \param fmt Format string input.
	/// \param args Arguments for formatting.
	/// \param out Output stream for formatted output.
	/// \param loc `struct lconv` pointer or nullptr.
	/// \returns The number of characters that would have been written if n had been sufficiently large, not counting the terminating null character.
	///          If an encoding error occurs, a negative number is returned.
	static size_t format (WideIn& fmt, va_list args, WideOut& out, const struct lconv* loc = nullptr) noexcept;

	template <class Cont>
	static size_t append_format_v (Cont& cont, const typename Cont::value_type* format, va_list arglist);

	template <class Cont>
	static size_t append_format (Cont& cont, const typename Cont::value_type* format, ...);

private:
	static void out_rev (char* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out, unsigned zeros = 0);

	static size_t ntoa_format (char* buf, size_t len, size_t max_len, bool negative, unsigned base,
		unsigned prec, unsigned width, unsigned flags) noexcept;

	template <typename U>
	static void ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width,
		unsigned flags, WideOutEx& out);

	template <typename U>
	static char* u_to_buf (U value, char* buf, const char* end, unsigned base, unsigned flags) noexcept;

	template <typename F>
	static char* whole_to_buf_16 (F whole, char* buf, const char* end, unsigned flags) noexcept;

	template <typename F>
	static char* whole_to_buf_10 (F whole, char* buf, const char* end, unsigned flags) noexcept;

	template <unsigned base, typename F>
	static char* whole_to_buf (F whole, char* buf, const char* end, unsigned flags) noexcept
	{
		if (base == 16)
			return whole_to_buf_16 (whole, buf, end, flags);
		else
			return whole_to_buf_10 (whole, buf, end, flags);
	}

	template <unsigned base, typename F>
	static char* f_to_buf (F value, char* buf, const char* end, unsigned prec, unsigned flags,
		const struct lconv* loc) noexcept;

	template <typename F>
	static bool spec_val (F value, unsigned int width, unsigned int flags, WideOutEx& out);

	template <typename F>
	static void ftoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
		const struct lconv* loc, WideOutEx& out);

	template <typename F>
	static void etoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
		const struct lconv* loc, WideOutEx& out);

	template <typename F>
	static void atoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
		const struct lconv* loc, WideOutEx& out);

	template <typename F>
	static int get_exp_10 (F value) noexcept;

	template <class C>
	static void out_string (const C* p, unsigned l, const unsigned width,
		const unsigned precision, unsigned flags, WideOutEx& out)
	{
		if ((flags & FLAG_PRECISION) && l > precision)
			l = precision;

		out_buf (p, l, width, flags & ~FLAG_ZEROPAD, out);
	}

	template <class C>
	static void out_buf (const C* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out, unsigned zeros = 0);

	static void out_buf_pre (unsigned len, unsigned width, unsigned flags, WideOutEx& out);
	static void out_buf_post (unsigned len, unsigned width, unsigned flags, WideOutEx& out);

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

	static char* dec_pt_to_buf (const struct lconv* loc, char* buf, const char* end, unsigned prec,
		unsigned flags) noexcept;

	static char* sign_to_buf (char* buf, const char* end, bool negative, unsigned flags) noexcept;

	static void out_exp (int exp, unsigned expwidth, WideOutEx& out);

	static unsigned f_width (unsigned width, unsigned expwidth, unsigned flags) noexcept;

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

	static const Flag flags_ [5];

	// define the default floating point precision
	// default: 6 digits
	static const unsigned PRINTF_DEFAULT_FLOAT_PRECISION = 6;

	// Special values

	struct Special
	{
		const char* lc;
		const char* uc;
		size_t len;
	};

	enum SpecVal
	{
		SPEC_NAN = 0,
		SPEC_INF,
		SPEC_INF_PLUS,
		SPEC_INF_MINUS,

		SPEC_VAL_CNT
	};

	static const Special special_values_ [SPEC_VAL_CNT];
};

template <class Cont>
size_t Formatter::append_format_v (Cont& cont, const typename Cont::value_type* format, va_list arglist)
{
	typedef typename Cont::value_type CType;
	WideInStrT <CType> fmt (format);
	WideOutContainerT <Cont> out (cont);
	return Formatter::format (fmt, arglist, out);
}

template <class Cont>
size_t Formatter::append_format (Cont& cont, const typename Cont::value_type* format, ...)
{
	va_list arglist;
	va_start (arglist, format);
	size_t cnt = append_format_v (cont, format, arglist);
	va_end (arglist);
	return cnt;
}

}

#endif
