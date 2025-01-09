/*
* Nirvana IDL support library.
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

namespace Nirvana {

Fixed::Fixed (double val)
{
	std::string s;
	Formatter::append_format (s, "%f", val);
	drop_trailing_zeros (s);
	dec_calc->from_string (val_, s.c_str ());
}

Fixed::Fixed (long double val)
{
	std::string s;
	Formatter::append_format (s, "%Lf", val);
	drop_trailing_zeros (s);
	dec_calc->from_string (val_, s.c_str ());
}

Fixed::operator long double () const
{
	std::string s = dec_calc->to_string (val_);
	return std::stold (s);
}

void Fixed::drop_trailing_zeros (std::string& s) noexcept
{
	if (s.find ('.') != std::string::npos) {
		while (!s.empty () && s.back () == '0') {
			s.pop_back ();
		}
	}
}

}
