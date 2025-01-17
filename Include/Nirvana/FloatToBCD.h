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
#ifndef NIRVANA_FLOATTOBCD_H_
#define NIRVANA_FLOATTOBCD_H_
#pragma once

#define _USE_MATH_DEFINES

#include <Nirvana/NirvanaBase.h>
#include <limits>
#include <algorithm>
#include <Nirvana/platform.h>
#include <Nirvana/IntTypes.h>

namespace Nirvana {

class FloatToBCD
{
public:
	static const size_t MAX_PRECISION = std::numeric_limits <FloatMax>::max_digits10;
	static const size_t MAX_WHOLE_DIGITS = std::max (
		-std::numeric_limits <FloatMax>::min_exponent10,
		std::numeric_limits <FloatMax>::max_exponent10);
	static const size_t MAX_DIGITS = MAX_WHOLE_DIGITS + MAX_PRECISION;

	FloatToBCD (FloatMax whole) noexcept;

	/// \brief Pointer to the first digit calculated
	const unsigned* digits () const noexcept
	{
		return digits_;
	}

	/// \brief Calculates next digits.
	/// 
	/// \returns Pointer beyond the last digit calculated.
	///   If next () - digits () < 2 then all digits were generated.
	const unsigned* next () noexcept;

private:
	// Unsigned half-word
	using UWord2 = IntTypes <sizeof (UWord) / 2>::Unsigned;

	static const unsigned HALF_WORD_BITS = sizeof (UWord2) * 8;

	static_assert (std::numeric_limits <FloatMax>::radix == 2, "Unexpected radix");
	
	static const unsigned BITS_MAX = std::max (std::numeric_limits <FloatMax>::max_exponent,
		-std::numeric_limits <FloatMax>::min_exponent);

	static const unsigned HALF_WORDS_MAX = (BITS_MAX + HALF_WORD_BITS - 1) / HALF_WORD_BITS;

	unsigned div100 () noexcept;
	bool is_zero () const noexcept;

protected:
	UWord2* big_num_end_;
	UWord2 big_num_ [HALF_WORDS_MAX];
	unsigned digits_ [2];
};

}

#endif
