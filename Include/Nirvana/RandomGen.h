/// \file Random number generator
/// Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs"
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
#ifndef NIRVANA_RANDOMGEN_H_
#define NIRVANA_RANDOMGEN_H_
#pragma once

#include <stdint.h>
#include <limits>

namespace Nirvana {

/// Pseudorandom number generator.
/// 
/// We use fast and non-cryptographically secure Xorshift algorithm.
/// http://www.jstatsoft.org/v08/i14/paper
class RandomGen
{
public:
	typedef unsigned result_type;

	template <typename I> struct InitSeed;

	template <>
	struct InitSeed <uint32_t>
	{
		static const uint32_t value = 2463534242;
	};

	template <>
	struct InitSeed <uint64_t>
	{
		static const uint64_t value = 88172645463325252LL;
	};

	RandomGen (result_type seed = InitSeed <result_type>::value) noexcept :
		state_ (seed)
	{}

	static result_type min () noexcept
	{
		return 0;
	}

	static result_type max () noexcept
	{
		return std::numeric_limits <result_type>::max ();
	}

	void state (result_type seed) noexcept
	{
		state_ = seed;
	}

	result_type operator () () noexcept;

protected:
	static uint16_t xorshift (uint16_t x) noexcept;
	static uint32_t xorshift (uint32_t x) noexcept;
	static uint64_t xorshift (uint64_t x) noexcept;

protected:
	result_type state_;
};

}

#endif
