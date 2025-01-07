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

class WideOut
{
public:
	virtual void put (uint32_t wc) = 0;
};

template <typename WC>
class WideOutBuf : public WideOut
{
public:
	WideOutBuf (WC* buf, WC* end) :
		p_ (buf),
		end_ (end)
	{}

	void put (uint32_t wc) override
	{
		if (std::numeric_limits <WC>::max () < wc)
			throw_CODESET_INCOMPATIBLE (make_minor_errno (EILSEQ));

		if (p_ < end_) {
			*(p_++) = (WC)wc;
			*p_ = 0;
		} else
			throw CORBA::UNKNOWN (make_minor_errno (ENOBUFS));
	}

private:
	WC* p_;
	WC* end_;
};

template <class Cont>
class WideOutContainer : public WideOut
{
public:
	WideOutContainer (Cont& cont) :
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
	WideOutUTF8 (ByteOut& bytes) :
		bytes_ (bytes)
	{}

	void put (uint32_t wc) override;

protected:
	ByteOut& bytes_;
};

class WideOutCP : public WideOutUTF8
{
public:
	WideOutCP (ByteOut& bytes, CodePage::_ptr_type cp);

	void put (uint32_t wc) override;

private:
	CodePage::_ref_type code_page_;
};

class WideIn;

void copy (WideIn& in, WideOut& out);

}

#endif
