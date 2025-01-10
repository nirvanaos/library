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
#ifndef NIRVANA_WIDEOUT_H_
#define NIRVANA_WIDEOUT_H_
#pragma once

#include "ByteOut.h"
#include <stdint.h>
#include <limits>
#include "nls.h"

namespace Nirvana {

class NIRVANA_NOVTABLE WideOut
{
public:
	virtual void put (uint32_t wc) = 0;
};

template <typename WC>
class WideOutBuf : public WideOut
{
public:
	WideOutBuf (WC* buf, WC* end) noexcept :
		p_ (buf),
		end_ (end),
		count_ (0)
	{}

	void put (uint32_t wc) override
	{
		if (std::numeric_limits <WC>::max () < wc)
			throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));

		if (p_ < end_) {
			*(p_++) = (WC)wc;
			*p_ = 0;
		}
		++count_;
	}

private:
	WC* p_;
	WC* end_;
	size_t count_;
};

template <class Cont>
class WideOutContainer : public WideOut
{
public:
	WideOutContainer (Cont& cont) noexcept :
		container_ (cont)
	{}

	void put (uint32_t wc) override
	{
		using WC = typename Cont::value_type;
		if (std::numeric_limits <WC>::max () < wc)
			throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));

		container_.push_back ((WC)wc);
	}

private:
	Cont& container_;
};

class WideOutUTF8 : public WideOut
{
public:
	WideOutUTF8 (ByteOut& bytes) noexcept :
		bytes_ (bytes)
	{}

	void put (uint32_t wc) override;

protected:
	ByteOut& bytes_;
};

template <class Cont>
class WideOutContainerUTF8 :
	public ByteOutContainer <Cont>,
	public WideOutUTF8
{
public:
	WideOutContainerUTF8 (Cont& cont) noexcept :
		ByteOutContainer <Cont> (cont),
		WideOutUTF8 (static_cast <ByteOut&> (*this))
	{}

	using WideOutUTF8::put;
};

template <class Cont>
using WideOutContainerT = typename std::conditional <std::is_same <char, typename Cont::value_type>::value,
	WideOutContainerUTF8 <Cont>, WideOutContainer <Cont> >::type;

class WideOutBufUTF8 :
	public ByteOutBuf,
	public WideOutUTF8
{
public:
	WideOutBufUTF8 (char* buf, char* end) noexcept :
		ByteOutBuf (buf, end),
		WideOutUTF8 (static_cast <ByteOut&> (*this))
	{}

	using WideOutUTF8::put;
};

template <typename C>
using WideOutBufT = typename std::conditional <std::is_same <char, C>::value,
	WideOutBufUTF8, WideOutBuf <C> >::type;

class WideOutCP : public WideOutUTF8
{
public:
	WideOutCP (ByteOut& bytes, CodePage::_ptr_type cp) noexcept;

	void put (uint32_t wc) override;

private:
	CodePage::_ref_type code_page_;
};

class WideOutEx
{
public:
	WideOutEx (WideOut& out);

	void put (uint32_t wc);

	size_t pos () const noexcept
	{
		return pos_;
	}

private:
	WideOut& out_;
	size_t pos_;
};

class WideIn;

void copy (WideIn& in, WideOut& out);

}

#endif
