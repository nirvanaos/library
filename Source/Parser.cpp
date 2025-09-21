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
#include <Nirvana/c_heap_dbg.h>

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

size_t Parser::size_alloc (size_t size_req) noexcept
{
	static const size_t OVERHEAD = sizeof (HeapBlockHdrType) + HeapBlockHdrType::TRAILER_SIZE;
	return clp2 (size_req + OVERHEAD) - OVERHEAD;
}

template <typename C>
class Parser::BufMalloc
{
	size_t INIT_SIZE = 256;

public:
	BufMalloc (C*& ptr, size_t size_hint) :
		ptr_ (ptr),
		buf_ (nullptr)
	{
		size_t cb = size_alloc (sizeof (C) * (size_hint ? (size_hint + 1) : INIT_SIZE));
		buf_ = (C*)malloc (cb);
		if (!buf_)
			throw std::bad_alloc ();
		cur_ = buf_;
		end_ = buf_ + cb / sizeof (C);
	}

	~BufMalloc ();

	void put (C c)
	{
		if (cur_ >= end_) {
			size_t cb = size_alloc ((end_ - buf_ + 1) * sizeof (C));
			C* new_buf = (C*)realloc (buf_, cb);
			if (!new_buf)
				throw std::bad_alloc ();
			cur_ = new_buf + (cur_ - buf_);
			buf_ = new_buf;
			end_ = buf_ + cb / sizeof (C);
		}
		*(cur_++) = c;
	}

private:
	C*& ptr_;
	C* buf_;
	C* end_;
	C* cur_;
};

template <typename C>
Parser::BufMalloc <C>::~BufMalloc ()
{
	if (
#ifdef NIRVANA_C17
		std::uncaught_exceptions ()
#else
		std::uncaught_exception ()
#endif
	)
		free (buf_);
	else
		ptr_ = buf_;
}

class Parser::ByteOutMalloc : public ByteOut,
	private BufMalloc <char>
{
public:
	ByteOutMalloc (char*& ptr, size_t size_hint) :
		BufMalloc <char> (ptr, size_hint)
	{}

	void put (unsigned c) override
	{
		assert (c <= 255);
		BufMalloc <char>::put ((char)c);
	}
};

template <typename C>
class Parser::WideOutMalloc : public WideOut,
	private BufMalloc <C>
{
public:
	WideOutMalloc (C*& ptr, size_t size_hint) :
		BufMalloc <C> (ptr, size_hint)
	{}

	void put (uint32_t c) override
	{
		assert (std::numeric_limits <C>::min () <= c && c <= std::numeric_limits <C>::max ());
		BufMalloc <C>::put ((C)c);
	}
};

class Parser::WideOutMallocUTF8 :
	public ByteOutMalloc,
	public WideOutUTF8
{
public:
	WideOutMallocUTF8 (char*& ptr, size_t size_hint) :
		ByteOutMalloc (ptr, size_hint),
		WideOutUTF8 (static_cast <ByteOut&> (*this))
	{}

	using WideOutUTF8::put;
};

template <typename C>
class Parser::CharOut
{
public:
	CharOut (unsigned width, unsigned flags, va_list& args);
	
	~CharOut ()
	{
		out_->~WideOut ();
	}

	operator WideOut& () noexcept
	{
		return *out_;
	}

private:
	union U {
		U () {}
		~U () {}

		WideOutNull dummy;
		WideOutStrT <C> buffer;
		WideOutMallocT <C> allocate;
	} u_;
	WideOut* out_;
};

template <typename C>
Parser::CharOut <C>::CharOut (unsigned width, unsigned flags, va_list& args)
{
	if (flags & FLAG_NOASSIGN) {
		new (&u_.dummy) WideOutNull ();
		out_ = &u_.dummy;
	} else if (flags & FLAG_MALLOC) {
		C** pp = va_arg (args, C**);
		new (&u_.allocate) WideOutMallocT <C> (*pp, width);
		out_ = &u_.allocate;
	} else {
		C* p = va_arg (args, C*);
		new (&u_.buffer) WideOutStrT <C> (p);
		out_ = &u_.buffer;
	}
}

template <typename C>
inline void Parser::get_char (WideInEx& in, unsigned width, unsigned flags, va_list& args)
{
	if (!width)
		width = 1;

	CharOut <C> out (width, flags, args);
	get_char (in, width, out, nullptr);
}

template <typename C>
inline void Parser::get_char (WideInEx& in, unsigned width, unsigned flags, const CharSet& set,
	va_list& args)
{
	CharOut <C> out (width, flags, args);
	if (!width)
		width = std::numeric_limits <unsigned>::max ();
	get_char (in, width, out, &set);
	static_cast <WideOut&> (out).put (0);
}

template <typename C>
inline void Parser::get_string (WideInEx& in, unsigned width, unsigned flags, va_list& args)
{
	CharOut <C> out (width, flags, args);
	get_string (in, width, out);
}

void Parser::get_char (WideInEx& in, unsigned width, WideOut& out, const CharSet* set)
{
	assert (width);
	int32_t c = in.cur ();
	while (c != EOF && (!set || set->acceptable (c)) && width--) {
		out.put (c);
		c = in.next ();
	}
}

void Parser::get_string (WideInEx& in, unsigned width, WideOut& out)
{
	if (!width)
		width = std::numeric_limits <unsigned>::max ();
	int32_t c = in.cur ();
	while (c != EOF && !iswspace (c) && width--) {
		out.put (c);
		c = in.next ();
	}
	out.put (0);
}

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

						case 'n':
							if (!(flags & FLAG_NOASSIGN))
								*va_arg (args, int*) = (int)in.pos ();
							break;

						case 'p':
							get_int <uintptr_t> (in, 16, flags, args);
							break;

						case 'm':
							flags |= FLAG_MALLOC;
							c = fmt.next ();
#ifdef NIRVANA_C17
							[[fallthrough]];
#endif
						default:
							switch (c) {

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

}
