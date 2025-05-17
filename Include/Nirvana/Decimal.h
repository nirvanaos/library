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

class DecimalBase
{
public:
	/// Initialize BCD value to zero
	/// 
	/// \param bcd BCD array.
	/// \param size Size of the bcd.
	static void BCD_zero (uint8_t* bcd, size_t size) noexcept;

	/// Check for BCD value is zero
	/// 
	/// \param bcd BCD array.
	/// \param size Size of the bcd.
	static bool BCD_is_zero (const uint8_t* bcd, size_t size) noexcept;

protected:
	static void float_to_BCD (const FloatMax& f, unsigned digits, int scale, uint8_t* bcd);
};

/// Fixed point value
/// 
/// \tparam digits The number of total digits
/// \tparam scale The number of fractional digits
template <uint16_t digits, int16_t scale>
class Decimal : private DecimalBase
{
	typedef FixedBCD <digits, scale> ABI;
public:
	///@{
	/// Constructors

	/// Initializes value to zero.
	Decimal () noexcept
	{
		BCD_zero (abi_.bcd, sizeof (abi_.bcd));
	}

	/// The copy constructor
	Decimal (const Decimal&) = default;

	/// Construct from constant FixedBCD <digits, scale>
	///
	/// \param abi Fixed constant
	Decimal (const ABI& abi) noexcept :
		abi_ (abi)
	{}

	/// Construct from constant FixedBCD with other digits and scale
	///
	/// \param abi Fixed constant
	template <uint16_t d, int16_t s>
	explicit Decimal (const FixedBCD <d, s>& abi) :
		Decimal (Fixed (abi))
	{}

	/// Implicitly constructible from Fixed.
	/// 
	/// \param src Fixed type.
	Decimal (const Fixed& src)
	{
		dec_calc->to_BCD (src, digits, scale, abi_.bcd);
	}

	explicit Decimal (int32_t val) :
		Decimal (Fixed (val))
	{}

	explicit Decimal (uint32_t val) :
		Decimal (Fixed (val))
	{}

	explicit Decimal (int64_t val) :
		Decimal (Fixed (val))
	{}

	explicit Decimal (uint64_t val) :
		Decimal (Fixed (val))
	{}

	explicit Decimal (const long double& val)
	{
		float_to_BCD (val, digits, scale, abi_.bcd);
	}

	explicit Decimal (const double& val)
	{
		float_to_BCD (val, digits, scale, abi_.bcd);
	}

	explicit Decimal (const float& val)
	{
		float_to_BCD (val, digits, scale, abi_.bcd);
	}

	explicit Decimal (const std::string& s) :
		Decimal (Fixed (s))
	{}

	explicit Decimal (const char* s) :
		Decimal (Fixed (s))
	{}

	///@}

	/// Implicit conversion to Fixed
	///
	/// \returns Fixed.
	operator Fixed () const
	{
		return Fixed (abi_);
	}

	///@{
	/// Conversions

	explicit operator int64_t () const
	{
		return Fixed (abi_).operator int64_t ();
	}

	explicit operator long double () const
	{
		return Fixed (abi_).operator long double ();
	}

	explicit operator double () const
	{
		return Fixed (abi_).operator double ();
	}

	explicit operator float () const
	{
		return Fixed (abi_).operator float ();
	}

	/// Converts a fixed value to a string
	/// 
	/// Leading zeros are dropped, but trailing fractional zeros are preserved.
	/// (For example, a fixed<4, 2> with the value 1.1 is converted "1.10").
	/// 
	/// \returns string
	std::string to_string () const
	{
		return Fixed (abi_).to_string ();
	}

	///@}
	/// 
	///@{
	/// Operators

	Decimal& operator = (const Decimal&) = default;

	Decimal& operator = (const Fixed& src)
	{
		dec_calc->to_BCD (src, digits, scale, abi_.bcd);
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

	operator const ABI& () const noexcept
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
trailing 'd' or 'D' from the IDL fixed-point literal representation is not inserted or extracted from the stream. These
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
