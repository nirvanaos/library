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
/// 
/// \tparam digits Number of digits
/// \tparam scale The scale
template <uint16_t digits, uint16_t scale>
class Decimal
{
	typedef IDL::FixedCDR <digits, scale> ABI;
public:
	///@{
	/// Constructors

	/// Initializes value to zero.
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

	Decimal (int32_t val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (uint32_t val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (int64_t val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (uint64_t val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (double val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (long double val) :
		Decimal (CORBA::Fixed (val))
	{}

	Decimal (const char* s) :
		Decimal (CORBA::Fixed (s))
	{}

	explicit Decimal (const std::string& s) :
		Decimal (CORBA::Fixed (s))
	{}

	///@}

	///@{
	/// Conversions

	operator int64_t () const
	{
		return CORBA::Fixed (*this);
	}

	operator long double () const
	{
		return CORBA::Fixed (*this);
	}

	/// Converts a fixed value to a string
	/// 
	/// Leading zeros are dropped, but trailing fractional zeros are preserved.
	/// (For example, a fixed<4, 2> with the value 1.1 is converted “1.10”).
	/// 
	/// \returns string
	std::string to_string () const
	{
		return CORBA::Fixed (abi_).to_string ();
	}

	///@}

	///@{
	/// Operators

	Decimal& operator = (const Decimal& val) = default;

	CORBA::Fixed operator += (const CORBA::Fixed& val)
	{
		CORBA::Fixed tmp (abi_);
		tmp += val;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator -= (const CORBA::Fixed& val)
	{
		CORBA::Fixed tmp (abi_);
		tmp += val;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator *= (const CORBA::Fixed& val)
	{
		CORBA::Fixed tmp (abi_);
		tmp *= val;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator /= (const CORBA::Fixed& val)
	{
		CORBA::Fixed tmp (abi_);
		tmp /= val;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator ++ ()
	{
		CORBA::Fixed tmp (abi_);
		++tmp;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator ++ (int)
	{
		CORBA::Fixed tmp (abi_);
		CORBA::Fixed ret (tmp);
		++tmp
		*this = Decimal (tmp);
		return ret;
	}

	CORBA::Fixed operator -- ()
	{
		CORBA::Fixed tmp (abi_);
		--tmp;
		*this = Decimal (tmp);
		return tmp;
	}

	CORBA::Fixed operator -- (int)
	{
		CORBA::Fixed tmp (abi_);
		CORBA::Fixed ret (tmp);
		--tmp
		*this = Decimal (tmp);
		return ret;
	}

	Decimal operator + () const
	{
		return *this;
	}

	CORBA::Fixed operator - () const
	{
		return -CORBA::Fixed (abi_);
	}

	bool operator! () const
	{
		return is_zero ();
	}

	explicit operator bool () const
	{
		return !is_zero ();
	}

	///@}

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
	bool is_zero () const NIRVANA_NOEXCEPT
	{
		for (const uint8_t* p = abi_.bcd, *end = p + digits / 2; p != end; ++p) {
			if (*p)
				return false;
		}
		return abi_.bcd [digits / 2] == 0x0C;
	}

private:
	ABI abi_;
};

template <uint16_t digits, uint16_t scale> inline
bool operator == (const Decimal <digits, scale>& val1, const Decimal <digits, scale>& val2)
{
	const IDL::FixedCDR <digits, scale>& cdr1 = val1;
	const IDL::FixedCDR <digits, scale>& cdr2 = val2;
	return std::equal (cdr1.bcd, cdr1.bcd + (digits + 2) / 2, cdr2.bcd);
}

template <uint16_t digits, uint16_t scale> inline
bool operator != (const Decimal <digits, scale>& val1, const Decimal <digits, scale>& val2)
{
	const IDL::FixedCDR <digits, scale>& cdr1 = val1;
	const IDL::FixedCDR <digits, scale>& cdr2 = val2;
	return !std::equal (cdr1.bcd, cdr1.bcd + (digits + 2) / 2, cdr2.bcd);
}

}

#endif
