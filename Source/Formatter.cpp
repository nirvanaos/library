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
#include <Nirvana/locale.h>
#include <Nirvana/strtoi.h>

// Based on https://github.com/mpaland/printf code.

namespace Nirvana {

const Formatter::Flag Formatter::flags_ [5] = {
	{ '-', FLAG_LEFT },
	{ '+', FLAG_PLUS },
	{ ' ', FLAG_SPACE },
	{ '#', FLAG_HASH },
	{ '0', FLAG_ZEROPAD }
};

const double Formatter::PRINTF_MAX_FLOAT = 1e9;

int Formatter::format (WideIn& fmt0, va_list args, WideOut& out0, const struct lconv* loc) noexcept
{
	try {
		WideInEx fmt (fmt0);
		WideOutEx out (out0);

		int count = 0;
		for (int32_t c; (c = fmt.cur ()) != EOF;) {
			if (c != '%') {
				fmt.next ();
				++count;
				out.put (c);
			} else {
				c = fmt.next ();
				if (c == '%') {
					fmt.next ();
					++count;
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
						strtoi (fmt, width);
						c = fmt.cur ();
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
							strtoi (fmt, precision);
							c = fmt.cur ();
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
								count += ntoa ((unsigned long long)(value > 0 ? value : -value), value < 0, base, precision, width, flags, out);
							} else if (flags & FLAG_LONG) {
								const long value = va_arg (args, long);
								count += ntoa ((unsigned long)(value > 0 ? value : -value), value < 0, base, precision, width, flags, out);
							} else {
								int value = va_arg (args, int);
								if (flags & FLAG_CHAR)
									value = (char)value;
								else if (flags & FLAG_SHORT)
									value = (short)value;
								count += ntoa ((unsigned)(value > 0 ? value : -value), value < 0, base, precision, width, flags, out);
							}
						} else {
							// unsigned
							if (flags & FLAG_LONG_LONG)
								count += ntoa (va_arg (args, unsigned long long), false, base, precision, width, flags, out);
							else if (flags & FLAG_LONG)
								count += ntoa (va_arg (args, unsigned long), false, base, precision, width, flags, out);
							else {
								unsigned int value = va_arg (args, unsigned int);
								if (flags & FLAG_CHAR)
									value = (unsigned char)value;
								else if (flags & FLAG_SHORT)
									value = (unsigned short)value;
								count += ntoa (value, false, base, precision, width, flags, out);
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
								if (flags & (FLAG_LONG | FLAG_LONG_DOUBLE))
									count += ftoa (va_arg (args, long double), precision, width, flags, loc, out);
								else
									count += ftoa (va_arg (args, double), precision, width, flags, loc, out);
								break;
							case 'g':
							case 'G':
							case 'E':
							case 'e':
								if ((c == 'g') || (c == 'G'))
									flags |= FLAG_ADAPT_EXP;
								if ((c == 'E') || (c == 'G'))
									flags |= FLAG_UPPERCASE;
								if (flags & (FLAG_LONG | FLAG_LONG_DOUBLE))
									count += etoa (va_arg (args, long double), precision, width, flags, loc, out);
								else
									count += etoa (va_arg (args, double), precision, width, flags, loc, out);
								break;
							case 'c': {
								// pre padding
								if (!(flags & FLAG_LEFT)) {
									for (unsigned l = 1; l < width; ++l) {
										out.put (' ');
										++count;
									}
								}
								// char output
								out.put (va_arg (args, int));

								// post padding
								if (flags & FLAG_LEFT) {
									for (unsigned l = 1; l < width; ++l) {
										out.put (' ');
										++count;
									}
								}
							} break;
							case 's': {
								if (flags & FLAG_LONG) {
									const wchar_t* p = va_arg (args, wchar_t*);
									unsigned l = (unsigned)wcsnlen (p, precision ? precision : std::numeric_limits <size_t>::max ());
									count += out_string (p, l, width, precision, flags, out);
								} else {
									const char* p = va_arg (args, char*);
									unsigned l = (unsigned)strnlen (p, precision ? precision : std::numeric_limits <size_t>::max ());
									count += out_string (p, l, width, precision, flags, out);
								}
							} break;
						}
					}
					fmt.next ();
				}
			}
		}
		return count;
	} catch (const CORBA::CODESET_INCOMPATIBLE&) {
		errno = EILSEQ;
	} catch (const CORBA::NO_MEMORY&) {
		errno = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		errno = err;
	} catch (const std::bad_alloc&) {
		errno = ENOMEM;
	} catch (...) {
		errno = EINVAL;
	}
	return -1;
}

unsigned Formatter::out_rev (char* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out)
{
	std::reverse (buf, buf + len);
	return out_buf (buf, len, width, flags, out);
}

unsigned Formatter::out_buf (const char* buf, size_t len, unsigned width, unsigned flags, WideOutEx& out)
{
	unsigned cnt = 0;

	// pad spaces up to given width
	if (!(flags & FLAG_LEFT) && !(flags & FLAG_ZEROPAD)) {
		for (size_t i = len; i < width; i++) {
			out.put (' ');
			++cnt;
		}
	}

	// Out string as UTF8 in case of UTF8 decimal point in lconv.
	WideInBufUTF8 in (buf, buf + len);
	for (;;) {
		auto c = in.get ();
		if (c == EOF)
			break;
		out.put (c);
		++cnt;
	}

	// append pad spaces up to given width
	if (flags & FLAG_LEFT) {
		while (cnt < width) {
			out.put (' ');
			++cnt;
		}
	}

	return cnt;
}

size_t Formatter::ntoa_format (char* buf, size_t len, size_t max_len, bool negative, unsigned base,
	unsigned prec, unsigned width, unsigned flags)
{
	// pad leading zeros
	if (!(flags & FLAG_LEFT)) {
		if (width && (flags & FLAG_ZEROPAD) && (negative || (flags & (FLAG_PLUS | FLAG_SPACE)))) {
			width--;
		}
		while ((len < prec) && (len < max_len)) {
			buf [len++] = '0';
		}
		while ((flags & FLAG_ZEROPAD) && (len < width) && (len < max_len)) {
			buf [len++] = '0';
		}
	}

	// handle hash
	if ((flags & FLAG_HASH) && base != 10) {
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
				buf [len++] = 'b';
			}
			if (len < max_len) {
				buf [len++] = '0';
			}
		}
	}

	if (len < max_len) {
		if (negative) {
			buf [len++] = '-';
		} else if (flags & FLAG_PLUS) {
			buf [len++] = '+';  // ignore the space if the '+' exists
		} else if (flags & FLAG_SPACE) {
			buf [len++] = ' ';
		}
	}

	return len;
}

unsigned Formatter::ftoa (double value, unsigned prec, unsigned width, unsigned flags,
	const struct lconv* loc, WideOutEx& out)
{
	char buf [PRINTF_FTOA_BUFFER_SIZE];
	char* p = buf;
	const char* const buf_end = std::end (buf);

	double diff = 0.0;

	// powers of 10
	static const double pow10 [] = {
		1,
		10,
		100,
		1000,
		10000,
		100000,
		1000000,
		10000000,
		100000000,
		1000000000 };

	// test for special values
	if (value != value)
		return out_buf ("nan", 3, width, flags, out);
	if (value < std::numeric_limits <double>::min ())
		return out_buf ("-inf", 4, width, flags, out);
	if (value > std::numeric_limits <double>::max ()) {
		if (flags & FLAG_PLUS)
			return out_buf ("+inf", 4, width, flags, out);
		else
			return out_buf ("inf", 3, width, flags, out);
	}

	// test for very large values
	// standard printf behavior is to print EVERY whole number digit -- which could be 100s of characters overflowing your buffers == bad
	if ((value > PRINTF_MAX_FLOAT) || (value < -PRINTF_MAX_FLOAT)) {
		return etoa (value, prec, width, flags, loc, out);
	}

	// test for negative
	bool negative = false;
	if (value < 0) {
		negative = true;
		value = -value;
	}

	// set default precision, if not set explicitly
	if (!(flags & FLAG_PRECISION)) {
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}

	// limit precision to 9, cause a prec >= 10 can lead to overflow errors
	while ((p < buf_end) && (prec > 9)) {
		*(p++) = '0';
		prec--;
	}

	int whole = (int)value;
	double tmp = (value - whole) * pow10 [prec];
	unsigned long frac = (unsigned long)tmp;
	diff = tmp - frac;

	if (diff > 0.5) {
		++frac;
		// handle rollover, e.g. case 0.99 with prec 1 is 1.0
		if (frac >= pow10 [prec]) {
			frac = 0;
			++whole;
		}
	} else if (diff < 0.5) {
	} else if ((frac == 0U) || (frac & 1U)) {
		// if halfway, round up if odd OR if last digit is 0
		++frac;
	}

	if (prec == 0U) {
		diff = value - (double)whole;
		if ((!(diff < 0.5) || (diff > 0.5)) && (whole & 1)) {
			// exactly 0.5 and ODD, then round up
			// 1.5 -> 2, but 2.5 -> 2
			++whole;
		}
	} else {
		unsigned int count = prec;
		// now do fractional part, as an unsigned number
		while (p < buf_end) {
			--count;
			*(p++) = (char)(48U + (frac % 10U));
			if (!(frac /= 10U)) {
				break;
			}
		}

		// add extra 0s
		while ((p < buf_end) && (count-- > 0U)) {
			*(p++) = '0';
		}

		// add decimal
		const char* dec_pt;
		size_t dec_pt_len;
		if (loc) {
			dec_pt = loc->decimal_point;
			dec_pt_len = strlen (dec_pt);
		} else {
			dec_pt = ".";
			dec_pt_len = 1;
		}

		if ((p + dec_pt_len) <= buf_end)
			p = std::reverse_copy (dec_pt, dec_pt + dec_pt_len, p);
	}

	// do whole part, number is reversed
	while (p < buf_end) {
		*(p++) = (char)(48 + (whole % 10));
		if (!(whole /= 10)) {
			break;
		}
	}

	// pad leading zeros
	if (!(flags & FLAG_LEFT) && (flags & FLAG_ZEROPAD)) {
		if (width && (negative || (flags & (FLAG_PLUS | FLAG_SPACE)))) {
			width--;
		}
		const char* pad_end = buf + width;
		if (pad_end > buf_end)
			pad_end = buf_end;
		while (p < pad_end) {
			*(p++) = '0';
		}
	}

	if (p < buf_end) {
		if (negative) {
			*(p++) = '-';
		} else if (flags & FLAG_PLUS) {
			*(p++) = '+';  // ignore the space if the '+' exists
		} else if (flags & FLAG_SPACE) {
			*(p++) = ' ';
		}
	}

	return out_rev (buf, p - buf, width, flags, out);
}

unsigned Formatter::etoa (double value, unsigned prec, unsigned width, unsigned flags,
	const struct lconv* loc, WideOutEx& out)
{
	// check for NaN and special values
	if ((value != value) || (value > std::numeric_limits <double>::max ()) || (value < std::numeric_limits <double>::min ())) {
		return ftoa (value, prec, width, flags, loc, out);
	}

	// determine the sign
	const bool negative = value < 0;
	if (negative) {
		value = -value;
	}

	// default precision
	if (!(flags & FLAG_PRECISION)) {
		prec = PRINTF_DEFAULT_FLOAT_PRECISION;
	}

	// determine the decimal exponent
	// based on the algorithm by David Gay (https://www.ampl.com/netlib/fp/dtoa.c)
	union
	{
		uint64_t U;
		double   F;
	} conv;

	conv.F = value;
	int exp2 = (int)((conv.U >> 52U) & 0x07FFU) - 1023;           // effectively log2
	conv.U = (conv.U & ((1ULL << 52U) - 1U)) | (1023ULL << 52U);  // drop the exponent so conv.F is now in [1,2)
	// now approximate log10 from the log2 integer part and an expansion of ln around 1.5
	int expval = (int)(0.1760912590558 + exp2 * 0.301029995663981 + (conv.F - 1.5) * 0.289529654602168);
	// now we want to compute 10^expval but we want to be sure it won't overflow
	exp2 = (int)(expval * 3.321928094887362 + 0.5);
	const double z = expval * 2.302585092994046 - exp2 * 0.6931471805599453;
	const double z2 = z * z;
	conv.U = (uint64_t)(exp2 + 1023) << 52U;
	// compute exp(z) using continued fractions, see https://en.wikipedia.org/wiki/Exponential_function#Continued_fractions_for_ex
	conv.F *= 1 + 2 * z / (2 - z + (z2 / (6 + (z2 / (10 + z2 / 14)))));
	// correct for rounding errors
	if (value < conv.F) {
		expval--;
		conv.F /= 10;
	}

	// the exponent format is "%+03d" and largest value is "307", so set aside 4-5 characters
	unsigned int minwidth = ((expval < 100) && (expval > -100)) ? 4U : 5U;

	// in "%g" mode, "prec" is the number of *significant figures* not decimals
	if (flags & FLAG_ADAPT_EXP) {
		// do we want to fall-back to "%f" mode?
		if ((value >= 1e-4) && (value < 1e6)) {
			if ((int)prec > expval) {
				prec = (unsigned)((int)prec - expval - 1);
			} else {
				prec = 0;
			}
			flags |= FLAG_PRECISION;   // make sure _ftoa respects precision
			// no characters in exponent
			minwidth = 0U;
			expval = 0;
		} else {
			// we use one sigfig for the whole part
			if ((prec > 0) && (flags & FLAG_PRECISION)) {
				--prec;
			}
		}
	}

	// will everything fit?
	unsigned int fwidth = width;
	if (width > minwidth) {
		// we didn't fall-back so subtract the characters required for the exponent
		fwidth -= minwidth;
	} else {
		// not enough characters, so go back to default sizing
		fwidth = 0U;
	}
	if ((flags & FLAG_LEFT) && minwidth) {
		// if we're padding on the right, DON'T pad the floating part
		fwidth = 0U;
	}

	// rescale the float value
	if (expval) {
		value /= conv.F;
	}

	// output the floating part
	unsigned count = ftoa (negative ? -value : value, prec, fwidth, flags & ~FLAG_ADAPT_EXP,
		loc, out);

	// output the exponent part
	if (minwidth) {
		// output the exponential symbol
		out.put ((flags & FLAG_UPPERCASE) ? 'E' : 'e');
		++count;
		// output the exponent value
		count += ntoa ((expval < 0) ? -expval : expval, expval < 0, 10, 0, minwidth - 1, FLAG_ZEROPAD | FLAG_PLUS, out);
		// might need to right-pad spaces
		if (flags & FLAG_LEFT) {
			while (count < width) {
				out.put (' ');
				++count;
			}
		}
	}
	return count;
}

}
