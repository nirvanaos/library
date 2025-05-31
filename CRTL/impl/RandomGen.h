/*
* Nirvana C runtime library.
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
#ifndef CRTL_IMPL_RANDOMGEN_H_
#define CRTL_IMPL_RANDOMGEN_H_
#pragma once

namespace CRTL {

class RandomGen
{
public:
	RandomGen () noexcept :
		state_ (2)
	{}

	int rand () noexcept;

	static int rand_r (unsigned& state) noexcept
	{
		/* Transform to [1, 0x7ffffffe] range. */
		unsigned val = (state % 0x7ffffffe) + 1;
		int ret = do_rand (val);
		state = val - 1;
		return ret;
	}

	void srand (unsigned seed) noexcept
	{
		state_ = seed;
	}

private:
	static int do_rand (unsigned& state) noexcept;

private:
	unsigned state_;
};

}

#endif
