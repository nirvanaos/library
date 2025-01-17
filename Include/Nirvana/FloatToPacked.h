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
#ifndef NIRVANA_FLOATTOPACKED_H_
#define NIRVANA_FLOATTOPACKED_H_
#pragma once

#include "FloatToBCD.h"

namespace Nirvana {

class FloatToPacked
{
public:
	FloatToPacked (FloatMax f, unsigned max_digits, unsigned max_scale);

	unsigned digits () const noexcept
	{
		return digits_;
	}

	int scale () const noexcept
	{
		return scale_;
	}

	bool negative () const noexcept
	{
		return negative_;
	}

	const uint8_t* bcd () const noexcept
	{
		return bcd_;
	}

	unsigned pack (int shift, uint8_t* packed, unsigned packed_size) const;

	void pack (unsigned digits, int scale, uint8_t* packed) const;

private:
	uint8_t* whole_to_buffer (const FloatMax& whole, uint8_t* buf, const uint8_t* end) noexcept;

private:
	unsigned digits_;
	int scale_;
	bool negative_;
	uint8_t bcd_ [FloatToBCD::MAX_DIGITS];
};

}

#endif
