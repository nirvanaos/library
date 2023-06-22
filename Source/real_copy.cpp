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
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>

namespace Nirvana {

template <>
void* real_copy (const void* begin, const void* end, void* dst)
{
	typedef size_t ProcessorWord;

	const uint8_t* aligned_begin = (const uint8_t*)round_up (begin, sizeof (ProcessorWord));
	const uint8_t* aligned_end = (const uint8_t*)round_down (end, sizeof (ProcessorWord));
	const uint8_t* b_src = (const uint8_t*)begin;
	uint8_t* b_dst = (uint8_t*)dst;

	if (aligned_begin < aligned_end) {

		while (b_src != aligned_begin)
			*(b_dst++) = *(b_src++);

		do {
			*(ProcessorWord*)b_dst = *(ProcessorWord*)b_src;
			b_dst += sizeof (ProcessorWord);
			b_src += sizeof (ProcessorWord);
		} while (b_src != aligned_end);

	}

	while (b_src != end)
		*(b_dst++) = *(b_src++);

	return b_dst;
}

template <>
void real_move (const void* begin, const void* end, void* dst)
{
	if (dst <= begin || dst >= end)
		real_copy (begin, end, dst);
	else {
		typedef size_t ProcessorWord;

		const uint8_t* aligned_begin = (const uint8_t*)round_up (begin, sizeof (ProcessorWord));
		const uint8_t* aligned_end = (const uint8_t*)round_down (end, sizeof (ProcessorWord));
		const uint8_t* b_src = (const uint8_t*)end;
		uint8_t* b_dst = (uint8_t*)dst + (b_src - (const uint8_t*)begin);

		if (aligned_begin < aligned_end) {

			while (end != aligned_end)
				*(--b_dst) = *(--b_src);

			do {
				b_dst -= sizeof (ProcessorWord);
				b_src -= sizeof (ProcessorWord);
				*(ProcessorWord*)b_dst = *(ProcessorWord*)b_src;
			} while (b_src != aligned_begin);

		}

		while (b_src != begin)
			*(--b_dst) = *(--b_src);
	}
}

}
