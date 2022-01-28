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

#include "Nirvana.h"
#include "nls.h"
#include <stdarg.h>

namespace Nirvana {

/// Universal formatter for C printf-like functions.
class Formatter
{
public:
	/// Virtual output class.
	class COut
	{
	public:
		/// Put character to output.
		/// 
		/// \param c Character.
		virtual void put (int c) = 0;

		/// Returns error flag.
		/// May be overridden for streams.
		/// If the method returns `true`, it must set `errno` code.
		virtual bool error ()
		{
			return false;
		}
	};

	/// Virtual input class for format string.
	class CIn
	{
	public:
		virtual int cur () const = 0;
		virtual int next () = 0;
	};

	/// Generalized C-style formatting function.
	/// As it intended to C formatting, it does not throw exceptions
	/// but sets `errno` codes on error instead.
	/// 
	/// \param wide `true` if \p fmt and \p out are wide character sequences.
	/// \param fmt Format string input.
	/// \param args Arguments for formatting.
	/// \param out Output stream for formatted output.
	/// \param loc Nirvana::Locale pointer or nullptr.
	/// \returns The number of characters that would have been written if n had been sufficiently large, not counting the terminating null character.
	///          If an encoding error occurs, a negative number is returned.
	static int vformat (bool wide, CIn& fmt, va_list args, COut& out,
		Locale::_ptr_type loc = Locale::_nil ()) NIRVANA_NOEXCEPT;

private:
	static unsigned copy (const wchar_t* begin, size_t cnt, bool wide, COut& out, Locale::_ptr_type loc);
	static unsigned copy (const char* begin, size_t cnt, bool wide, COut& out, Locale::_ptr_type loc);

	static unsigned out_rev (const char* buf, size_t len, unsigned width, unsigned flags, COut& out);

	static bool is_digit (int c)
	{
		return ('0' <= c) && (c <= '9');
	}

	static unsigned strtou (CIn& in);

	static size_t ntoa_format (char* buf, size_t len, bool negative, unsigned base, unsigned prec, unsigned width, unsigned flags);

	template <typename U>
	static unsigned ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width, unsigned flags, COut& out);

	static unsigned ftoa (double value, unsigned int prec, unsigned int width, unsigned int flags, COut& out);
	static unsigned etoa (double value, unsigned int prec, unsigned int width, unsigned int flags, COut& out);

	template <class C>
	static unsigned out_string (const C* p, unsigned l, const unsigned width,
		const unsigned precision, unsigned flags, bool wide, COut& out,
		Locale::_ptr_type loc)
	{
		if ((flags & FLAG_PRECISION) && l > precision)
			l = precision;

		size_t len = l;

		unsigned cnt = 0;

		// pre padding
		if (!(flags & FLAG_LEFT)) {
			while (l++ < width) {
				out.put (' ');
				++cnt;
			}
		}

		// string output
		cnt += copy (p, len, wide, out, loc);

		// post padding
		if (flags & FLAG_LEFT) {
			while (l++ < width) {
				out.put (' ');
				++cnt;
			}
		}

		return cnt;
	}

private:
	static const unsigned FLAG_ZEROPAD = 1 << 0;
	static const unsigned FLAG_LEFT = 1 << 1;
	static const unsigned FLAG_PLUS = 1 << 2;
	static const unsigned FLAG_SPACE = 1 << 3;
	static const unsigned FLAG_HASH = 1 << 4;
	static const unsigned FLAG_PRECISION = 1 << 5;
	static const unsigned FLAG_UPPERCASE = 1 << 6;
	static const unsigned FLAG_CHAR = 1 << 7;
	static const unsigned FLAG_SHORT = 1 << 8;
	static const unsigned FLAG_LONG = 1 << 9;
	static const unsigned FLAG_LONG_LONG = 1 << 10;
	static const unsigned FLAG_ADAPT_EXP = 1 << 11;

	// 'ntoa' conversion buffer size, this must be big enough to hold one converted
	// numeric number including padded zeros (dynamically created on stack)
	// default: 32 byte
	static const size_t PRINTF_NTOA_BUFFER_SIZE = 32;

	// 'ftoa' conversion buffer size, this must be big enough to hold one converted
	// float number including padded zeros (dynamically created on stack)
	// default: 32 byte
	static const size_t PRINTF_FTOA_BUFFER_SIZE = 32;

	struct Flag
	{
		unsigned cflag;
		unsigned uflag;
	};

	static const Flag flags_ [5];
	static const unsigned short int_formats_ [7];

	// define the largest float suitable to print with %f
	// default: 1e9
	static const double PRINTF_MAX_FLOAT; //  1e9

	// define the default floating point precision
	// default: 6 digits
	static const unsigned PRINTF_DEFAULT_FLOAT_PRECISION = 6;
};

template <typename U>
unsigned Formatter::ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width, unsigned flags, COut& out)
{
	char buf [PRINTF_NTOA_BUFFER_SIZE];
	size_t len = 0;

	// no hash for 0 values
	if (!value) {
		flags &= ~FLAG_HASH;
	}

	// write if precision != 0 and value is != 0
	if (!(flags & FLAG_PRECISION) || value) {
		do {
			const char digit = (char)(value % base);
			buf [len++] = digit < 10 ? '0' + digit : (flags & FLAG_UPPERCASE ? 'A' : 'a') + digit - 10;
			value /= base;
		} while (value && (len < PRINTF_NTOA_BUFFER_SIZE));
	}

	len = ntoa_format (buf, len, negative, base, prec, width, flags);
	return out_rev (buf, len, width, flags, out);
}

/// Formatter input for null-terminated string.
/// 
/// \tparam C Character type.
template <typename C>
class CIn : public Formatter::CIn
{
public:
	CIn (const C* s) :
		p_ (s)
	{}

	virtual int cur () const
	{
		return *p_;
	}

	virtual int next ()
	{
		int c = *p_;
		if (c)
			c = *++p_;
		return c;
	}

private:
	const C* p_;
};

/// Formatter output for buffer with known size.
/// 
/// \tparam C Character type.
template <typename C>
class COutBufSize : public Formatter::COut
{
public:
	COutBufSize (C* buf, size_t size) :
		p_ (buf),
		end_ (buf + size - 1)
	{}

	virtual void put (int c)
	{
		if (p_ < end_) {
			*(p_++) = (C)c;
			*p_ = 0;
		}
	}

private:
	C* p_;
	C* end_;
};

/// Formatter output for appending to std container.
/// 
/// \tparam Cont Container type.
template <typename Cont>
class COutContainer : public Formatter::COut
{
public:
	COutContainer (Cont& cont) :
		cont_ (cont)
	{}

	virtual void put (int c)
	{
		cont_.push_back ((typename Cont::value_type)c);
	}

private:
	Cont& cont_;
};

template <class Cont>
void append_format (Cont& cont, typename Cont::value_type* format, ...)
{
	typedef typename Cont::value_type CType;
	CIn <CType> in (format);
	COutContainer <Cont> out (cont);
	va_list arglist;
	va_start (arglist, format);
	Formatter ().vformat (sizeof (CType) > 1, in, arglist, out);
	va_end (arglist);
}

}

#endif
