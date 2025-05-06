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
#ifndef NIRVANA_BYTEIN_H_
#define NIRVANA_BYTEIN_H_
#pragma once

#include <CORBA/CORBA.h>

namespace Nirvana {

/// @brief Virtual byte input class.
class NIRVANA_NOVTABLE ByteIn
{
public:
	/// Returns the obtained character as an `unsigned char` converted to an `int`.
	/// If no characters are available, returns EOF (-1).
	virtual int get () = 0;
};

/// @brief Input from null-terminated string.
class ByteInStr : public ByteIn
{
public:
	ByteInStr (const char* s) noexcept :
		p_ (s)
	{}

	int get () override;

	const char* cur_ptr () const noexcept
	{
		return p_;
	}

protected:
	const char* p_;
};

class ByteInBuf : public ByteInStr
{
	using Base = ByteInStr;

public:
	ByteInBuf (const char* buf, const char* end) noexcept :
		Base (buf),
		end_ (end)
	{}

	int get () override;

private:
	const char* end_;
};

}

#endif
