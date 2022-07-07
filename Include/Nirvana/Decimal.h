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
#ifndef NIRVANA_DECIMAL_H_
#define NIRVANA_DECIMAL_H_
#pragma once

#include <CORBA/Fixed.h>
#include <algorithm>

namespace Nirvana {

/// Fixed point value
template <uint16_t digits, uint16_t scale>
class Decimal
{
	typedef IDL::FixedCDR <digits, scale> ABI;
public:
	Decimal ()
	{
		std::fill_n (abi_.bcd, digits / 2, (uint8_t)0);
		abi_.bcd [digits / 2] = 0x0C;
	}

	Decimal (const Decimal&) = default;

	Decimal (const ABI& src) :
		abi_ (src)
	{}

	Decimal (const CORBA::Fixed& f)
	{
		g_dec_calc->to_BCD (f, digits, scale, abi_.bcd);
	}

	operator const ABI& () const
	{
		return abi_;
	}

	const ABI* operator & () const
	{
		return &abi_;
	}

	ABI* operator & ()
	{
		return &abi_;
	}

private:
	ABI abi_;
};

}

#endif
