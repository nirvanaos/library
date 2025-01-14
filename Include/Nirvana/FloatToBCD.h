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

#define _USE_MATH_DEFINES

#include <Nirvana/NirvanaBase.h>
#include <Nirvana/platform.h>
#include <Nirvana/IntTypes.h>
#include <Nirvana/bitutils.h>
#include <assert.h>
#include <type_traits>
#include <cmath>
#include <iterator>

namespace Nirvana {

class FloatToBcdBase
{
public:
	/// \brief Pointer to the first digit calculated
	/// 
	/// </summary>
	/// <returns></returns>
	const unsigned* digits () const noexcept
	{
		return digits_;
	}

protected:
	// Unsigned half-word
	using UWord2 = IntTypes <sizeof (UWord) / 2>::Unsigned;

	static const unsigned HALF_WORD_BITS = sizeof (UWord2) * 8;

	unsigned div100 (UWord2* big_num) noexcept;
	bool is_zero (const UWord2* big_num) const noexcept;

	const unsigned* next (UWord2* big_num) noexcept;

protected:
	UWord2* big_num_end_;
	unsigned digits_ [2];
};

template <typename F>
class FloatToBCD : public FloatToBcdBase
{
	using Base = FloatToBcdBase;

public:
	FloatToBCD (F whole) noexcept;

	/// \brief Calculates next digits.
	/// 
	/// \returns Pointer beyond the last digit calculated.
	///   If next () - digits () < 2 then all digits were generated.
	const unsigned* next () noexcept
	{
		return Base::next (big_num_);
	}

private:
	static_assert (std::numeric_limits <F>::radix == 2, "Unexpected radix");
	static const unsigned BITS_MAX = std::max (std::numeric_limits <F>::max_exponent, -std::numeric_limits <F>::min_exponent);
	static const unsigned HALF_WORDS_MAX = (BITS_MAX + HALF_WORD_BITS - 1) / HALF_WORD_BITS;

	Base::UWord2 big_num_ [HALF_WORDS_MAX + 1];
};

template <typename F>
FloatToBCD <F>::FloatToBCD (F whole) noexcept
{
	assert (whole >= 0);

	big_num_ [0] = 0;
	UWord2* end = big_num_ + 1;

	F div = (F)((UWord)1 << HALF_WORD_BITS);

	while (whole > 0) {
		F part = std::fmod (whole, div);
		whole -= part;
		whole /= div;
		assert (end < std::end (big_num_));
		*(end++) = (UWord2)part;
	}
	big_num_end_ = end;
}

}

#endif
