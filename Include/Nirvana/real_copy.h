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
#ifndef NIRVANA_CORE_REAL_COPY_H_
#define NIRVANA_CORE_REAL_COPY_H_
#pragma once

#include <stdint.h>

namespace Nirvana {

template <class InIt, class OutIt>
inline OutIt real_copy (InIt begin, InIt end, OutIt dst)
{
	while (begin != end)
		*(dst++) = *(begin++);

	return dst;
}

// Partial specialization for performance

template <>
uint8_t* real_copy (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

template <class BidIt1, class BidIt2>
inline void real_move (BidIt1 begin, BidIt1 end, BidIt2 dst)
{
	if (dst <= begin || dst >= end)
		while (begin != end)
			*(dst++) = *(begin++);
	else
		while (begin != end)
			*(--dst) = *(--end);
}

// Partial specialization for performance

template <>
void real_move (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

}

#endif
