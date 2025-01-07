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
#include <stdio.h>
#include <assert.h>

namespace Nirvana {

class ByteOut
{
public:
	virtual void put (int c) = 0;
};

class ByteOutBuf : public ByteOut
{
public:
	ByteOutBuf (char* buf, char* end) :
		p_ (buf),
		end_ (end)
	{}

	void put (int c) override;

private:
	char* p_;
	char* end_;
};

template <class Cont>
class ByteOutContainer : public ByteOut
{
public:
	ByteOutContainer (Cont& cont) :
		container_ (cont)
	{}

	void put (int c) override
	{
		unsigned u = c;
		assert (u <= 255);
		container_.push_back ((char)u);
	}

private:
	Cont& container_;
};

class ByteOutFile : public ByteOut
{
public:
	ByteOutFile (FILE* f) :
		f_ (f)
	{}

	void put (int c) override;

private:
	FILE* f_;
};

}

#endif
