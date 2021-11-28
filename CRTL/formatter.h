#ifndef NIRVANA_CRTL_FORMATTER_H_
#define NIRVANA_CRTL_FORMATTER_H_

#include <locale>
#include <assert.h>
#include <stdarg.h>

using namespace std;

namespace Nirvana {

class Formatter
{
public:
	class COut
	{
	public:
		virtual void put (int) = 0;
	};

	class CIn
	{
	public:
		virtual int cur () const = 0;
		virtual int next () = 0;
	};

	Formatter ()
	{}

	Formatter (const std::locale& loc) :
		locale_ (loc)
	{}

	int vformat (bool wide, CIn& fmt, va_list args, COut& out) const;

private:
	void copy (const wchar_t* begin, const wchar_t* end, bool wide, COut& out) const;
	void copy (const char* begin, const char* end, bool wide, COut& out) const;

	static size_t out_rev (const char* buf, size_t len, unsigned width, unsigned flags, COut& out);

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

	std::locale locale_;
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

}

#endif
