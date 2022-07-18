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

#include "Fixed.h"
#include <algorithm>

namespace Nirvana {

/// Initialize BCD value to zero
/// 
/// \param bcd BCD array.
/// \param size Size of the bcd.
void BCD_zero (uint8_t* bcd, size_t size) NIRVANA_NOEXCEPT;

/// Check for BCD value is zero
/// 
/// \param bcd BCD array.
/// \param size Size of the bcd.
bool BCD_is_zero (const uint8_t* bcd, size_t size) NIRVANA_NOEXCEPT;

/// Fixed point value
/// 
/// \tparam digits Number of digits
/// \tparam scale The scale
template <uint16_t digits, int16_t scale>
class Decimal
{
	typedef FixedBCD <digits, scale> ABI;
public:
	///@{
	/// Constructors

	/// Initializes value to zero.
	Decimal () NIRVANA_NOEXCEPT
	{
		BCD_zero (abi_.bcd, sizeof (abi_.bcd));
	}

	Decimal (const Decimal&) = default;

	Decimal (const ABI& src) NIRVANA_NOEXCEPT :
		abi_ (src)
	{}

	Decimal (const Fixed& src)
	{
		g_dec_calc->to_BCD (src, digits, scale, abi_.bcd);
	}

	template <uint16_t d, int16_t s>
	Decimal (const FixedBCD <d, s>& abi) :
		Decimal (Fixed (abi))
	{}

	///@}

	/// Conversion
	///
	/// \returns Fixed.
	operator Fixed () const
	{
		return Fixed (abi_);
	}

	///@{
	/// Operators

	Decimal& operator = (const Decimal&) = default;

	Decimal& operator = (const Fixed& src)
	{
		g_dec_calc->to_BCD (src, digits, scale, abi_.bcd);
		return *this;
	}

	Fixed operator += (const Fixed& val)
	{
		Fixed tmp (abi_);
		tmp += val;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator -= (const Fixed& val)
	{
		Fixed tmp (abi_);
		tmp += val;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator *= (const Fixed& val)
	{
		Fixed tmp (abi_);
		tmp *= val;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator /= (const Fixed& val)
	{
		Fixed tmp (abi_);
		tmp /= val;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator ++ ()
	{
		Fixed tmp (abi_);
		++tmp;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator ++ (int)
	{
		Fixed tmp (abi_);
		Fixed ret (tmp);
		++tmp;
		*this = Decimal (tmp);
		return ret;
	}

	Fixed operator -- ()
	{
		Fixed tmp (abi_);
		--tmp;
		*this = Decimal (tmp);
		return tmp;
	}

	Fixed operator -- (int)
	{
		Fixed tmp (abi_);
		Fixed ret (tmp);
		--tmp;
		*this = Decimal (tmp);
		return ret;
	}

	Decimal operator + () const
	{
		return *this;
	}

	Fixed operator - () const
	{
		return -Fixed (abi_);
	}

	bool operator! () const
	{
		return BCD_is_zero (abi_.bcd, sizeof (abi_.bcd));
	}

	explicit operator bool () const
	{
		return !BCD_is_zero (abi_.bcd, sizeof (abi_.bcd));
	}

	///@}

	operator const ABI& () const NIRVANA_NOEXCEPT
	{
		return abi_;
	}

private:
	ABI abi_;
};

///@{
/**
The stream insertion and extraction operators << and >> convert a fixed - point value to / from a stream. These operators
insertand extract fixed-point values into the stream using the same format as for C++ floating point types. In particular, the
trailing ‘d’ or ‘D’ from the IDL fixed-point literal representation is not inserted or extracted from the stream. These
operators use all format controls appropriate to floating point defined by the stream classes except that they never use the
scientific format.
*/

template <uint16_t digits, int16_t scale> inline
std::istream& operator >> (std::istream& is, Decimal <digits, scale>& val)
{
	Fixed f;
	is >> f;
	val = f;
	return is;
}

template <uint16_t digits, int16_t scale> inline
std::ostream& operator << (std::ostream& os, const Decimal <digits, scale>& val)
{
	return os << Fixed (val);
}

///@}

///@{
/// Binary operators

template <uint16_t d, int16_t s> inline
bool operator == (const Decimal <d, s>& val1, const Decimal <d, s>& val2)
{
	const FixedBCD <d, s>& bcd1 = val1;
	const FixedBCD <d, s>& bcd2 = val2;
	return std::equal (bcd1.bcd, bcd1.bcd + sizeof (bcd1.bcd), bcd2.bcd);
}

template <uint16_t d, int16_t s> inline
bool operator != (const Decimal <d, s>& val1, const Decimal <d, s>& val2)
{
	return !operator == (val1, val2);
}

///@}

}

#endif
