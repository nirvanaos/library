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
#ifndef NIRVANA_CONVERTER_H_
#define NIRVANA_CONVERTER_H_
#pragma once

#include "Nirvana.h"
#include "nls.h"

namespace Nirvana {

/// Virtual input class.
class CIn
{
public:
	/// \brief Returns character at the current position.
	virtual int cur () const = 0;

	/// \brief Move to the next character.
	/// 
	/// \returns Next character.
	virtual int next () = 0;
};

/// Formatter input for null-terminated string.
/// 
/// \tparam C Character type.
template <typename C>
class Format : public CIn
{
public:
	Format (const C* s) :
		p_ (s)
	{}

	virtual int cur () const
	{
		return *p_;
	}

	virtual int next ()
	{
		int c = *p_;
		if (c)
			c = *++p_;
		return c;
	}

private:
	const C* p_;
};

class Converter
{
protected:
	static const unsigned FLAG_CHAR = 1 << 0;
	static const unsigned FLAG_SHORT = 1 << 1;
	static const unsigned FLAG_LONG = 1 << 2;
	static const unsigned FLAG_LONG_LONG = 1 << 3;
	static const unsigned FLAG_LONG_DOUBLE = 1 << 4;
	static const unsigned FLAG_SIGNED = 1 << 5;
	static const unsigned FLAG_UPPERCASE = 1 << 6;

	static const char int_formats_ [7];

	static unsigned strtou (CIn& in);

	static bool is_digit (int c)
	{
		return ('0' <= c) && (c <= '9');
	}

	static unsigned length_flags (CIn& fmt);
	static unsigned int_base (int c, unsigned& flags);

	static void skip_space (bool wide, CIn& in, CodePage::_ptr_type loc);
};

}

#endif
