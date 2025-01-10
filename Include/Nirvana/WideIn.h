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
#ifndef NIRVANA_WIDEIN_H_
#define NIRVANA_WIDEIN_H_
#pragma once

#include "ByteIn.h"
#include <stdint.h>
#include "nls.h"

namespace Nirvana {

/// @brief Virtual wide input class.
class NIRVANA_NOVTABLE WideIn
{
public:
	/// \returns The obtained character as an `unsigned` converted to an `int32_t`.
	/// If no characters are available, returns EOF (-1).
	virtual int32_t get () = 0;
};

/// @brief Input from null-terminated string.
template <typename WC>
class WideInStr : public WideIn
{
public:
	WideInStr (const WC* s) noexcept :
		begin_ (s),
		p_ (s)
	{}

	int32_t get () override
	{
		WC wc = *p_;
		if (wc)
			++p_;
		else
			wc = EOF;
		return wc;
	}

	const WC* cur_ptr () const noexcept
	{
		return p_;
	}

protected:
	const WC* begin_;
	const WC* p_;
};

template <typename WC>
class WideInBuf : public WideInStr <WC>
{
	using Base = WideInStr <WC>;

public:
	WideInBuf (const WC* buf, const WC* end) noexcept :
		Base (buf),
		end_ (end)
	{}

	int32_t get () override
	{
		if (Base::p_ >= end_)
			return EOF;
		else
			return *(Base::p_++);
	}

private:
	const WC* end_;
};

class WideInUTF8 : public WideIn
{
public:
	WideInUTF8 (ByteIn& bytes) noexcept :
		bytes_ (bytes)
	{}

	int32_t get () override;

private:
	unsigned get_next_octet () const;

protected:
	ByteIn& bytes_;
};

class WideInStrUTF8 :
	public ByteInStr,
	public WideInUTF8
{
public:
	WideInStrUTF8 (const char* s) noexcept :
		ByteInStr (s),
		WideInUTF8 (static_cast <ByteIn&> (*this))
	{}

	using WideInUTF8::get;
};

template <typename C>
using WideInStrT = typename std::conditional <std::is_same <char, C>::value,
	WideInStrUTF8, WideInStr <C> >::type;

class WideInBufUTF8 :
	public ByteInBuf,
	public WideInUTF8
{
public:
	WideInBufUTF8 (const char* buf, const char* end) noexcept :
		ByteInBuf (buf, end),
		WideInUTF8 (static_cast <ByteIn&> (*this))
	{}

	using WideInUTF8::get;
};

template <typename C>
using WideInBufT = typename std::conditional <std::is_same <char, C>::value,
	WideInBufUTF8, WideInBuf <C> >::type;

class WideInCP : public WideInUTF8
{
public:
	WideInCP (ByteIn& bytes, CodePage::_ptr_type cp) noexcept;

	int32_t get () override;

private:
	CodePage::_ref_type code_page_;
};

}

#endif
