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
#include "../../pch/pch.h"
#include <Nirvana/RandomGen.h>

namespace Nirvana {

inline
uint16_t RandomGen::xorshift (uint16_t x) noexcept
{
	x ^= x << 7;
	x ^= x >> 9;
	x ^= x << 8;
	return x;
}

inline
uint32_t RandomGen::xorshift (uint32_t x) noexcept
{
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return x;
}

inline
uint64_t RandomGen::xorshift (uint64_t x) noexcept
{
	x ^= x << 13;
	x ^= x >> 7;
	x ^= x << 17;
	return x;
}

RandomGen::result_type RandomGen::rand_r (result_type& state) noexcept
{
	return state = xorshift (state);
}

}
