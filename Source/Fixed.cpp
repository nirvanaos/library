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
#include <Nirvana/FloatToPacked.h>
#include <Nirvana/WideInEx.h>

namespace Nirvana {

template <typename F> inline static
void float_to_number (const F& f, ::Nirvana::DecCalc::Number& n)
{
	static const size_t MAX_DIGITS = 62;

	FloatToPacked <F> conv (f, MAX_DIGITS, MAX_DIGITS);
	BCD <MAX_DIGITS> bcd;
	unsigned digits = conv.pack (0, bcd, sizeof (bcd));
	int scale = digits - (conv.digits () - conv.scale ());
	dec_calc->from_BCD (n, (uint16_t)digits, (int16_t)scale, bcd);
}

void Fixed::construct_from_float (float f)
{
	float_to_number (f, val_);
}

void Fixed::construct_from_float (const double& f)
{
	float_to_number (f, val_);
}

void Fixed::construct_from_float (const long double& f)
{
	float_to_number (f, val_);
}

Fixed::operator long double () const
{
	std::string s = dec_calc->to_string (val_);
	const char* ps = s.c_str ();
	WideInBuf chars (ps, ps + s.size ());
	long double ret;
	if (sizeof (long double) == sizeof (double))
		WideInEx (chars).get_float ((double&)ret);
	else
		WideInEx (chars).get_float (ret);
	return ret;
}

}
