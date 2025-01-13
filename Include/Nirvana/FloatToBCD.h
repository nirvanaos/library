/// \file
/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#ifndef NIRVANA_FLOAT2BCD_H_
#define NIRVANA_FLOAT2BCD_H_
#pragma once

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/platform.h>
#include <type_traits>
#include <cmath>

namespace Nirvana {

template <size_t> struct IntTypes;

template <>
struct IntTypes <8>
{
	using Signed = int64_t;
	using Unsigned = uint64_t;
};

template <>
struct IntTypes <4>
{
	using Signed = int32_t;
	using Unsigned = uint32_t;
};

template <>
struct IntTypes <2>
{
	using Signed = int16_t;
	using Unsigned = uint16_t;
};

template <>
struct IntTypes <1>
{
	using Signed = int8_t;
	using Unsigned = uint8_t;
};

class FloatToBCDBase
{
public:
	const unsigned* digits () const noexcept
	{
		return digits_;
	}

protected:
	// Unsigned half-word
	using UWord2 = IntTypes <sizeof (UWord) / 2>::Unsigned;

	static unsigned div100 (UWord2* begin, UWord2* end) noexcept;
	static bool is_zero (const UWord2* begin, const UWord2* end) noexcept;

	const unsigned* next (UWord2* begin, UWord2* end) noexcept;

protected:
	unsigned digits_ [2];
};

template <typename F>
class FloatToBCD : public FloatToBCDBase
{
	using Base = FloatToBCDBase;

public:
	FloatToBCD (F whole) noexcept;

	const unsigned* next () noexcept
	{
		return Base::next (big_num_, big_num_end_);
	}

private:
	static_assert (std::numeric_limits <F>::radix == 2, "Unexpected radix");

	static const size_t HEX_DIGITS = (-std::numeric_limits <F>::min_exponent + 3) / 4;
	static const size_t HALF_WORD_HEX_DIGITS = sizeof (UWord2) * 2;
	static const size_t HALF_WORDS_MAX = (HEX_DIGITS + HALF_WORD_HEX_DIGITS - 1) / HALF_WORD_HEX_DIGITS;

	UWord2 big_num_ [HALF_WORDS_MAX];
	UWord2* big_num_end_;
};

template <typename F>
FloatToBCD <F>::FloatToBCD (F whole) noexcept :
	big_num_end_ (big_num_)
{
	const F div = (F)std::numeric_limits <UWord2>::max () + 1;
	
	while (whole > 0) {
		F part = std::fmod (whole, div);
		whole -= part;
		whole /= div;
		assert (big_num_end_ < std::end (big_num_));
		*(big_num_end_++) = (UWord2)part;
	}
}

}

#endif
