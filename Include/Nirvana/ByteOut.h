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
#ifndef NIRVANA_BYTEOUT_H_
#define NIRVANA_BYTEOUT_H_
#pragma once

#include <CORBA/CORBA.h>
#include <assert.h>

namespace Nirvana {

class NIRVANA_NOVTABLE ByteOut
{
public:
	virtual void put (unsigned c) = 0;
};

class ByteOutStr : public ByteOut
{
public:
	ByteOutStr (char* buf) noexcept :
		p_ (buf)
	{}

	void put (unsigned c) override
	{
		assert (c <= 255);
		*(p_++) = (char)c;
	}

	char* cur_ptr () const noexcept
	{
		return p_;
	}

private:
	char* p_;
};

class ByteOutBuf : public ByteOut
{
public:
	ByteOutBuf (char* buf, char* end) noexcept :
		p_ (buf),
		end_ (end),
		count_ (0)
	{
    assert (buf <= end);
  }

	void put (unsigned c) override;

	size_t count () const noexcept
	{
		return count_;
	}

	char* cur_ptr () const noexcept
	{
		return p_;
	}

private:
	char* p_;
	char* end_;
	size_t count_;
};

template <class Cont>
class ByteOutContainer : public ByteOut
{
public:
	ByteOutContainer (Cont& cont) noexcept :
		container_ (cont)
	{}

	void put (unsigned c) override
	{
		assert (c <= 255);
		container_.push_back ((char)c);
	}

private:
	Cont& container_;
};

}

#endif
