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
#include <Nirvana/Formatter.h>
#include <string.h>
#include <wchar.h>
#include <limits>
#include <cmath>
#include <cfenv>
#include <type_traits>
#include <Nirvana/locale_defs.h>
#include <Nirvana/FloatToBCD.h>

#pragma float_control (precise, on)

namespace Nirvana {

const Formatter::Flag Formatter::flags_ [5] = {
	{ '-', FLAG_LEFT },
	{ '+', FLAG_PLUS },
	{ ' ', FLAG_SPACE },
	{ '#', FLAG_HASH },
	{ '0', FLAG_ZEROPAD }
};

const Formatter::Special Formatter::special_values_ [SPEC_VAL_CNT] = {
	{ "nan", "NAN", 3 },
	{ "inf", "INF", 3 },
	{ "+inf", "+INF", 4 },
	{ "-inf", "-INF", 4 }
};

size_t Formatter::format (WideIn& fmt0, va_list args, WideOut& out0, const struct lconv* loc)
{
	WideInEx fmt (fmt0);
	WideOutEx out (out0);

	for (int32_t c; (c = fmt.cur ()) != EOF;) {
		if (c != '%') {
			fmt.next ();
			out.put (c);
		} else {
			c = fmt.next ();
			if (c == '%') {
				fmt.next ();
				out.put (c);
			} else {
				// flags
				unsigned flags = 0;
				for (;;) {
					const Flag* p = flags_;
					for (; p != std::end (flags_); ++p) {
						if (p->cflag == c)
							break;
					}
					if (p != std::end (flags_)) {
						flags |= p->uflag;
						c = fmt.next ();
					} else
						break;
				}

				// width
				unsigned width = 0;
				if (is_digit (c)) {
					c = fmt.get_int (width, 10);
				} else if (c == '*') {
					width = va_arg (args, int);
					c = fmt.next ();
				}

				// precision
				unsigned precision = 0;
				if (c == '.') {
					flags |= FLAG_PRECISION;
					c = fmt.next ();
					if (is_digit (c)) {
						c = fmt.get_int (precision, 10);
					} else if (c == '*') {
						precision = va_arg (args, int);
						c = fmt.next ();
					}
				}

				// length
				flags |= length_flags (fmt);
				c = fmt.cur ();

				if (std::find (int_formats_, std::end (int_formats_), c) != std::end (int_formats_)) {
					// Integer format

					unsigned base = int_base (c, flags);

					// No plus or space flag for unsigned
					if (!(flags & FLAG_SIGNED)) {
						flags &= ~(FLAG_PLUS | FLAG_SPACE);
					}

					// ignore '0' flag when precision is given
					if (flags & FLAG_PRECISION) {
						flags &= ~FLAG_ZEROPAD;
					}

					if (flags & FLAG_SIGNED) {
						// signed
						if (flags & FLAG_LONG_LONG) {
							const long long value = va_arg (args, long long);
							ntoa ((unsigned long long)(value > 0 ? value : -value), value < 0, base, precision, width,
								flags, out);
						} else if (flags & FLAG_LONG) {
							const long value = va_arg (args, long);
							ntoa ((unsigned long)(value > 0 ? value : -value), value < 0, base, precision, width,
								flags, out);
						} else {
							int value = va_arg (args, int);
							if (flags & FLAG_CHAR)
								value = (char)value;
							else if (flags & FLAG_SHORT)
								value = (short)value;
							ntoa ((unsigned)(value > 0 ? value : -value), value < 0, base, precision, width,
								flags, out);
						}
					} else {
						// unsigned
						if (flags & FLAG_LONG_LONG)
							ntoa (va_arg (args, unsigned long long), false, base, precision, width, flags, out);
						else if (flags & FLAG_LONG)
							ntoa (va_arg (args, unsigned long), false, base, precision, width, flags, out);
						else {
							unsigned int value = va_arg (args, unsigned int);
							if (flags & FLAG_CHAR)
								value = (unsigned char)value;
							else if (flags & FLAG_SHORT)
								value = (unsigned short)value;
							ntoa (value, false, base, precision, width, flags, out);
						}
					}
				} else {
					switch (c) {
						case 'F':
							flags |= FLAG_UPPERCASE;
#ifdef NIRVANA_C17
							[[fallthrough]];
#endif
						case 'f':
							if (sizeof (long double) > sizeof (double) && (flags & FLAG_LONG_DOUBLE))
								ftoa (va_arg (args, long double), precision, width, flags, loc, out);
							else
								ftoa (va_arg (args, double), precision, width, flags, loc, out);
							break;
						case 'g':
						case 'G':
						case 'E':
						case 'e':
							if ((c == 'g') || (c == 'G'))
								flags |= FLAG_ADAPT_EXP;
							if ((c == 'E') || (c == 'G'))
								flags |= FLAG_UPPERCASE;
							if (sizeof (long double) > sizeof (double) && (flags & FLAG_LONG_DOUBLE))
								etoa (va_arg (args, long double), precision, width, flags, loc, out);
							else
								etoa (va_arg (args, double), precision, width, flags, loc, out);
							break;
						case 'A':
							flags |= FLAG_UPPERCASE;
#ifdef NIRVANA_C17
							[[fallthrough]];
#endif
						case 'a':
							if (sizeof (long double) > sizeof (double) && (flags & FLAG_LONG_DOUBLE))
								atoa (va_arg (args, long double), precision, width, flags, loc, out);
							else
								atoa (va_arg (args, double), precision, width, flags, loc, out);
							break;
						case 'c': {
							// pre padding
							if (!(flags & FLAG_LEFT)) {
								for (unsigned l = 1; l < width; ++l) {
									out.put (' ');
								}
							}
							// char output
							out.put (va_arg (args, int));

							// post padding
							if (flags & FLAG_LEFT) {
								for (unsigned l = 1; l < width; ++l) {
									out.put (' ');
								}
							}
						} break;
						case 's': {
							if (flags & FLAG_LONG) {
								const wchar_t* p = va_arg (args, wchar_t*);
								unsigned l = (unsigned)wcsnlen (p, precision ?
									precision : std::numeric_limits <size_t>::max ());
								out_string (p, l, width, precision, flags, out);
							} else {
								const char* p = va_arg (args, char*);
								unsigned l = (unsigned)strnlen (p, precision ?
									precision : std::numeric_limits <size_t>::max ());
								out_string (p, l, width, precision, flags, out);
							}
						} break;
						case 'n':
							*va_arg (args, int*) = (int)out.pos ();
							break;
						case 'p':
							ntoa ((uintptr_t)va_arg (args, void*), false, 16, 2 + sizeof (void*) * 2, width,
								FLAG_ZEROPAD | FLAG_HASH, out);
							break;

						default:
							throw_BAD_PARAM (make_minor_errno (EILSEQ));
					}
				}
				fmt.next ();
			}
		}
	}
	return (int)out.pos ();
}

template <typename U> inline
void Formatter::ntoa (U value, bool negative, unsigned base, unsigned prec, unsigned width,
	unsigned flags, WideOutEx& out)
{
	ntoa_impl <typename std::conditional <(sizeof (U) >= sizeof (UWord)), U, UWord>::type> (value,
		negative, base, prec, width, flags, out);
}

template <typename U>
void Formatter::ntoa_impl (U value, bool negative, unsigned base, unsigned prec, unsigned width,
	unsigned flags, WideOutEx& out)
{
	char buf [sizeof (value) * 8 + 4];
	char* p = buf;

	// no hash for 0 values
	if (!value)
		flags &= ~FLAG_HASH;

	// write if value is != 0 or precision is not specified
	if (value || !(flags & FLAG_PRECISION))
		p = u_to_buf (value, p, buf + sizeof (buf), base, flags);

	if (!value)
		flags &= ~FLAG_HASH;
	size_t len = p - buf;
	len = ntoa_format (buf, len, sizeof (buf), negative, base, prec, width, flags);
	out_rev (buf, len, width, flags, out);
}

template <typename U>
char* Formatter::u_to_buf (U value, char* buf, const char* end, unsigned base, unsigned flags) noexcept
{
	do {
		assert (buf < end);
		if (buf >= end)
			break;
		unsigned digit = value % base;
		*(buf++) = digit < 10 ? '0' + digit : (flags & FLAG_UPPERCASE ? 'A' : 'a') + digit - 10;
		value /= base;
	} while (value);

	return buf;
}

size_t Formatter::ntoa_format (char* buf, size_t len, size_t max_len, bool negative, unsigned base,
	unsigned prec, unsigned width, unsigned flags) noexcept
{
  // Reserve place for sign
  if (width && (negative || (flags & (FLAG_PLUS | FLAG_SPACE))))
    width--;

	// pad leading zeros
  while ((len < prec) && (len < max_len)) {
    buf [len++] = '0';
  }
	if ((flags & (FLAG_LEFT | FLAG_ZEROPAD)) == FLAG_ZEROPAD) {
		while ((len < width) && (len < max_len)) {
			buf [len++] = '0';
		}
	}

	// handle hash
	if ((flags & FLAG_HASH) && (base == 2U || base == 8U || base == 16U)) {
		if (!(flags & FLAG_PRECISION) && len && ((len == prec) || (len == width))) {
			len--;
			if (len && (base == 16U)) {
				len--;
			}
		}
		if (len < max_len) {
			if (base == 16U) {
				buf [len++] = flags & FLAG_UPPERCASE ? 'X' : 'x';
			} else if (base == 2U) {
				buf [len++] = flags & FLAG_UPPERCASE ? 'B' : 'b';
			}
			if (len < max_len) {
				buf [len++] = '0';
			}
		}
	}

	char* p = sign_to_buf (buf + len, buf + max_len, negative, flags);
	return p - buf;
}

char* Formatter::sign_to_buf (char* buf, const char* end, bool negative, unsigned flags) noexcept
{
	if (buf < end) {
		if (negative)
			*(buf++) = '-';
		else if (flags & FLAG_PLUS)
			*(buf++) = '+';  // ignore the space if the '+' exists
		else if (flags & FLAG_SPACE)
			*(buf++) = ' ';
	}
	return buf;
}

template <typename F>
bool Formatter::spec_val (const F& value, unsigned int width, unsigned int flags, WideOutEx& out)
{
	SpecVal spec;
	if (value != value)
		spec = SPEC_NAN;
	else if (value < -std::numeric_limits <F>::max ())
		spec = SPEC_INF_MINUS;
	else if (value > std::numeric_limits <F>::max ())
		spec = (flags & FLAG_PLUS) ? SPEC_INF_PLUS : SPEC_INF;
	else
		return false;

	const Special& v = special_values_ [spec];
  flags &= ~FLAG_ZEROPAD; // 0 ignored when padding infs
	out_buf ((flags & FLAG_UPPERCASE) ? v.uc : v.lc, v.len, width, flags, out);
	return true;
}

template <unsigned base> inline
char* Formatter::f_to_buf (const FloatMax& value, char* p, const char* buf_end, unsigned prec, unsigned flags,
	const struct lconv* loc) noexcept
{
	int rm = std::fegetround ();
	std::fesetround (FE_TONEAREST);

	FloatMax whole;
	FloatMax frac = std::modf (value, &whole);
	if (prec == 0 && frac >= 0.5)
		++whole;

	// Fractional part
	if (prec > 0) {
		const char* end = p + prec;
		assert (end <= buf_end);
		if (end > buf_end)
			end = buf_end;
		if (frac) {
			frac *= std::pow ((FloatMax)base, (int)prec);
			frac = std::round (frac);
			p = whole_to_buf <base> (frac, p, buf_end, flags);
			assert (p <= end);
		}
		while (p < end) {
			*(p++) = '0';
		}
	}

	// add decimal
	p = dec_pt_to_buf (loc, p, buf_end, prec, flags);

	// Whole part
	if (whole)
		p = whole_to_buf <base> (whole, p, buf_end, flags);
	else {
		assert (p < buf_end);
		if (p < buf_end)
			*(p++) = '0';
	}

	std::fesetround (rm);

	return p;
}

template <typename F> inline
void Formatter::ftoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
	const struct lconv* loc, WideOutEx& out)
{
	// test for special values
	if (!spec_val (value, width, flags, out))
		ftoa_impl (value, prec, width, flags, loc, out);
}

void Formatter::ftoa_impl (FloatMax value, unsigned prec, unsigned width, unsigned flags,
	const struct lconv* loc, WideOutEx& out)
{
	static const size_t MAX_PRECISION = FloatToBCD::MAX_PRECISION;

	// 'ftoa' conversion buffer size, this must be big enough to hold max digits, sign and decimal point.
	const size_t BUFFER_SIZE = 2 + FloatToBCD::MAX_DIGITS;

	char buf [BUFFER_SIZE];
	char* p = buf;
	const char* const buf_end = buf + BUFFER_SIZE;

	// set default precision, if not set explicitly
	unsigned add_zeros = 0;
	if (!(flags & FLAG_PRECISION))
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	else if (prec > MAX_PRECISION) {
		// Prevent the buffer overflow
		add_zeros = prec - MAX_PRECISION;
		prec = MAX_PRECISION;
	}

	// test for negative
	bool negative = false;
	if (value < 0) {
		negative = true;
		value = -value;
	}

	p = f_to_buf <10> (value, p, buf_end, prec, flags, loc);

	// pad leading zeros
	if (!(flags & FLAG_LEFT) && (flags & FLAG_ZEROPAD)) {
		if (width && (negative || (flags & (FLAG_PLUS | FLAG_SPACE)))) {
			width--;
		}
		const char* pad_end = buf + width;
		assert (pad_end <= buf_end);
		if (pad_end > buf_end)
			pad_end = buf_end;
		while (p < pad_end) {
			*(p++) = '0';
		}
	}

	p = sign_to_buf (p, buf_end, negative, flags);

	out_rev (buf, p - buf, width, flags, out, add_zeros);
}

template <typename F> inline
void Formatter::atoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
	const struct lconv* loc, WideOutEx& out)
{
	// test for special values
	if (!spec_val (value, width, flags, out)) {
		int exp;
		F frac = std::frexp (value, &exp) * 2;
		atoa_impl (frac, exp, prec, width, flags, loc, out);
	}
}

void Formatter::atoa_impl (FloatMax frac, int exp, unsigned prec, unsigned width, unsigned flags,
	const struct lconv* loc, WideOutEx& out)
{
	static_assert (std::numeric_limits <FloatMax>::radix == 2, "Unexpected radix");
	static const size_t MAX_PRECISION = (std::numeric_limits <FloatMax>::digits - 1 + 3) / 4;

	// 'atoa' conversion buffer size, this must be big enough to hold one converted
	// float number including padded zeros (dynamically created on stack)
	// "-0x1." + MAX_PRECISION + "p-1024"
	const size_t BUFFER_SIZE = 5 + MAX_PRECISION + 6;

	char buf [BUFFER_SIZE];
	char* p = buf;
	const char* const buf_end = std::end (buf);

	// set default precision, if not set explicitly
	unsigned add_zeros = 0;
	if (!(flags & FLAG_PRECISION))
		prec = MAX_PRECISION;
	else if (prec > MAX_PRECISION) {
		// Prevent the buffer overflow
		add_zeros = prec - MAX_PRECISION;
		prec = MAX_PRECISION;
	}

	// test for negative
	bool negative = false;
	if (frac < 0) {
		negative = true;
		frac = -frac;
	}

	size_t begin = out.pos ();

	p = f_to_buf <16> (frac, p, buf_end, prec, flags, loc);

	assert (p < buf_end);
	if (p < buf_end)
		*(p++) = (flags & FLAG_UPPERCASE) ? 'X' : 'x';
	assert (p < buf_end);
	if (p < buf_end)
		*(p++) = '0';

	p = sign_to_buf (p, buf_end, negative, flags);

	// Binary exponent may contain up to 4 digits
	// expwidth is width of the exponent part: "p+00"
	unsigned int expwidth = (-100 < exp && exp < 100) ? 4U :
		((-1000 < exp && exp < 1000) ? 5U : 6U);

	unsigned int fwidth = f_width (width, expwidth, flags);

	out_rev (buf, p - buf, fwidth, flags, out, add_zeros);

	// Exponent
	out.put ((flags & FLAG_UPPERCASE) ? 'P' : 'p');
	// output the exponent value
	out_exp (exp, expwidth, out);
	// might need to right-pad spaces
	if (flags & FLAG_LEFT) {
		unsigned count = (unsigned)(out.pos () - begin);
		while (count < width) {
			out.put (' ');
		}
	}
}

char* Formatter::dec_pt_to_buf (const struct lconv* loc, char* buf, const char* end,
	unsigned prec, unsigned flags) noexcept
{
	if (prec > 0 || (flags & FLAG_HASH)) {
		const char* dec_pt;
		size_t dec_pt_len;
		if (loc) {
			dec_pt = loc->decimal_point;
			dec_pt_len = strlen (dec_pt);
		} else {
			dec_pt = ".";
			dec_pt_len = 1;
		}

		if ((buf + dec_pt_len) <= end)
			buf = std::reverse_copy (dec_pt, dec_pt + dec_pt_len, buf);
	}
	return buf;
}

template <typename F> inline
int Formatter::get_exp_10 (const F& value) noexcept
{
	static_assert (std::numeric_limits <F>::radix == 10 || std::numeric_limits <F>::radix == 2, "Unexpected radix");
	assert (value);

	if (std::numeric_limits <F>::radix == 2) {
		F fexp = std::logb (value);
		fexp *= ((F)1 / (F)3.32192809489);
		return (int)std::round (fexp);
	} else
		return std::ilogb (value);
}

template <typename F> inline
void Formatter::etoa (F value, unsigned int prec, unsigned int width, unsigned int flags,
	const struct lconv* loc, WideOutEx& out)
{
	// test for special values
	if (!spec_val (value, width, flags, out)) {
		int exp = 0;
		if (value)
			exp = get_exp_10 (value);
		etoa_impl (value, exp, prec, width, flags, loc, out);
	}
}

void Formatter::etoa_impl (FloatMax value, int exp, unsigned prec, unsigned width, unsigned flags,
	const struct lconv* loc, WideOutEx& out)
{
	// determine the sign
	const bool negative = value < 0;
	if (negative)
		value = -value;

	// default precision
	if (!(flags & FLAG_PRECISION))
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;

	FloatMax expscale = 1;
	if (exp) {
		expscale = std::pow ((FloatMax)10, exp);
		if (value < expscale) {
			expscale /= 10;
			--exp;
		}
	}

	// The exponent contains at least two digits, more digits are used only if necessary.
	// expwidth is width of the exponent part: "e+00"
	unsigned int expwidth = (-100 < exp && exp < 100) ? 4U : 5U;

	// in "%g" mode, "prec" is the number of *significant figures* not decimals
	if (flags & FLAG_ADAPT_EXP) {
		// do we want to fall-back to "%f" mode?
		if ((value >= 1e-4) && (value < 1e6)) {
			if ((int)prec > exp) {
				prec = (unsigned)((int)prec - exp - 1);
			} else {
				prec = 0;
			}
			flags |= FLAG_PRECISION;   // make sure ftoa respects precision
			// no characters in exponent
			expwidth = 0U;
			exp = 0;
		} else {
			// we use one sigfig for the whole part
			if ((prec > 0) && (flags & FLAG_PRECISION))
				--prec;
		}
	}

	unsigned int fwidth = f_width (width, expwidth, flags);

	// rescale the float value
	if (exp)
		value /= expscale;

	// output the floating part
	size_t begin = out.pos ();
	ftoa_impl (negative ? -value : value, prec, fwidth, (flags & ~FLAG_ADAPT_EXP) | FLAG_PRECISION, loc, out);

	// output the exponent part
	if (expwidth) {
		// output the exponential symbol
		out.put ((flags & FLAG_UPPERCASE) ? 'E' : 'e');
		// output the exponent value
		out_exp (exp, expwidth, out);
		// might need to right-pad spaces
		if (flags & FLAG_LEFT) {
			unsigned count = (unsigned)(out.pos () - begin);
			while (count < width) {
				out.put (' ');
			}
		}
	}
}

void Formatter::out_exp (int exp, unsigned expwidth, WideOutEx& out)
{
	ntoa ((exp < 0) ? -exp : exp, exp < 0, 10, 0, expwidth - 1, FLAG_ZEROPAD | FLAG_PLUS, out);
}

unsigned Formatter::f_width (unsigned width, unsigned expwidth, unsigned flags) noexcept
{
	// will everything fit?
	unsigned int fwidth = width;
	if (width > expwidth) {
		// we didn't fall-back so subtract the characters required for the exponent
		fwidth -= expwidth;
	} else {
		// not enough characters, so go back to default sizing
		fwidth = 0U;
	}
	if ((flags & FLAG_LEFT) && expwidth) {
		// if we're padding on the right, DON'T pad the floating part
		fwidth = 0U;
	}
	return fwidth;
}

char* Formatter::whole_to_buf_16 (FloatMax whole, char* buf, const char* end, unsigned flags) noexcept
{
	static const FloatMax DIV = (FloatMax)std::numeric_limits <UWord>::max () + 1;
	static const size_t PART_LEN = sizeof (UWord) * 2;

	if (whole > 0) {
		for (;;) {
			FloatMax part = std::fmod (whole, DIV);
			whole -= part;
			whole /= DIV;
			UWord u = (UWord)part;
			const char* end = buf + PART_LEN;
			buf = u_to_buf (u, buf, end, 16, flags);
			if (whole) {
				while (buf < end)
					*(buf++) = '0';
			} else
				break;
		}
	}

	return buf;
}

char* Formatter::whole_to_buf_10 (const FloatMax& whole, char* buf, const char* end, unsigned flags) noexcept
{
	FloatToBCD conv (whole);

	for (;;) {
		const unsigned* d_end = conv.next ();
		for (const unsigned* d = conv.digits (); d < d_end; ++d) {
			assert (buf < end);
			*(buf++) = (char)(*d + '0');
		}
		if (d_end - conv.digits () < 2)
			break;
	}

	return buf;
}

void Formatter::out_rev (char* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out,
	unsigned add_zeros)
{
	std::reverse (buf, buf + len);
	out_buf (buf, len, width, flags, out, add_zeros);
}

template <class C>
void Formatter::out_buf (const C* buf, size_t size, unsigned width, unsigned flags, WideOutEx& out,
	unsigned add_zeros)
{
	unsigned len = get_len (buf, size);

	// Pad up to given width
	auto begin = out.pos ();
	out_buf_pre (len, width, flags, out);

	// Out narrow string as UTF8 in case of UTF8 decimal point in lconv.
	WideInBufT <C> in (buf, buf + len);
	for (;;) {
		auto c = in.get ();
		if (c == EOF)
			break;
		out.put (c);
	}

	while (add_zeros--) {
		out.put ('0');
	}

	// Pad spaces up to given width
	out_buf_post ((unsigned)(out.pos () - begin), width, flags, out);
}

void Formatter::out_buf_pre (unsigned len, unsigned width, unsigned flags, WideOutEx& out)
{
	// pad spaces up to given width
	if (!(flags & FLAG_LEFT)) {
    int c = (flags & FLAG_ZEROPAD) ? '0' : ' ';
		while (len < width) {
			out.put (c);
			++len;
		}
	}
}

void Formatter::out_buf_post (unsigned len, unsigned width, unsigned flags, WideOutEx& out)
{
	// append pad spaces up to given width
	if (flags & FLAG_LEFT) {
		while (len < width) {
			out.put (' ');
			++len;
		}
	}
}

}
