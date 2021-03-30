/*
* Nirvana IDL support library.
*
* This is a part of the Nirvana project.
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
#include <Nirvana/real_copy.h>
#include <Nirvana/NirvanaBase.h>

namespace Nirvana {

template <>
uint8_t* real_copy (const uint8_t* begin, const uint8_t* end, uint8_t* dst)
{
	const uint8_t* aligned_begin = round_up (begin, sizeof (UWord));
	const uint8_t* aligned_end = round_down (end, sizeof (UWord));

	while (begin != aligned_begin)
		*(dst++) = *(begin++);

	if (aligned_begin < aligned_end) {

		while (begin != aligned_begin)
			*(dst++) = *(begin++);

		do {
			*(UWord*)dst = *(UWord*)begin;
			dst += sizeof (UWord);
			begin += sizeof (UWord);
		} while (begin != aligned_end);

	}

	while (begin != end)
		*(dst++) = *(begin++);

	return dst;
}

template <>
void real_move (const uint8_t* begin, const uint8_t* end, uint8_t* dst)
{
	const uint8_t* aligned_begin = round_up (begin, sizeof (UWord));
	const uint8_t* aligned_end = round_down (end, sizeof (UWord));

	if (dst <= begin || dst >= end) {

		if (aligned_begin < aligned_end) {

			while (begin != aligned_begin)
				*(dst++) = *(begin++);

			do {
				*(UWord*)dst = *(UWord*)begin;
				dst += sizeof (UWord);
				begin += sizeof (UWord);
			} while (begin != aligned_end);

		}

		while (begin != end)
			*(dst++) = *(begin++);

	} else {

		if (aligned_begin < aligned_end) {

			while (end != aligned_end)
				*(--dst) = *(--end);

			do {
				dst -= sizeof (UWord);
				end -= sizeof (UWord);
				*(UWord*)dst = *(UWord*)end;
			} while (end != aligned_begin);

		}

		while (end != begin)
			*(--dst) = *(--end);

	}
}

}
