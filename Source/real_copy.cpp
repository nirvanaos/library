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
#include <algorithm>

namespace Nirvana {

static const unsigned PLATFORM_WORD_SIZE = sizeof (size_t) >= 8 ? 8 : (sizeof (size_t) >= 4 ? 4 : 2);

static unsigned get_alignment (const void* p)
{
	unsigned align;
	if ((uintptr_t)p % 2)
		align = 1;
	else if (PLATFORM_WORD_SIZE < 4 || (uintptr_t)p % 4)
		align = 2;
	else if (PLATFORM_WORD_SIZE < 8 || (uintptr_t)p % 8)
		align = 4;
	else
		align = PLATFORM_WORD_SIZE;

	return align;
}

static unsigned get_word_size (const void* begin, const void* end, void* dst)
{
	unsigned align_src = get_alignment (begin);
	unsigned align_dst = get_alignment (dst);

	unsigned word_size = PLATFORM_WORD_SIZE;
	if (align_src != align_dst)
		word_size = std::min (align_src, align_dst);

	size_t size = (const uint8_t*)end - (const uint8_t*)begin;
	assert (size);
	if ((size_t)word_size > size) {
		word_size = (unsigned)size;
		if (word_size > 1) {
			if (word_size < 4)
				word_size = 2;
			else if (PLATFORM_WORD_SIZE > 4 && word_size < 8)
				word_size = 4;
		}
	}

	return word_size;
}

template <>
void* real_copy (const void* begin, const void* end, void* dst)
{
	if (begin >= end)
		return dst;

	unsigned word_size = get_word_size (begin, end, dst);

	const uint8_t* b_src = (const uint8_t*)begin;
	const uint8_t* b_end = (const uint8_t*)end;
	uint8_t* b_dst = (uint8_t*)dst;

	if (1 == word_size) {

		while (b_src < b_end)
			*(b_dst++) = *(b_src++);

	} else {

		const uint8_t* aligned_begin = round_up (b_src, word_size);
		const uint8_t* aligned_end = round_down (b_end, word_size);

		if (b_src < aligned_begin && (uintptr_t)b_src % 2)
			*(b_dst++) = *(b_src++);

		if (word_size > 2) {
			if (b_src < aligned_begin && (uintptr_t)b_src % 4) {
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
			if (word_size > 4) {
				if (b_src < aligned_begin && (uintptr_t)b_src % 8) {
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
				while (b_src < aligned_end) {
					*(uint64_t*)b_dst = *(const uint64_t*)b_src;
					b_dst += 8;
					b_src += 8;
				}
				if (b_src < b_end && (uintptr_t)b_end % 8 >= 4) {
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
			} else {
				while (b_src < aligned_end) {
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
			}
			if (b_src < b_end && (uintptr_t)b_end % 4 >= 2) {
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
		} else {
			while (b_src < aligned_end) {
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
		}

		if (b_src < b_end)
			*(b_dst++) = *(b_src++);
	}

	return b_dst;
}

template <>
void* real_copy_backward (const void* begin, const void* end, void* dst)
{
	if (begin >= end)
		return dst;

	unsigned word_size = get_word_size (begin, end, dst);

	const uint8_t* b_src = (const uint8_t*)end;
	const uint8_t* b_begin = (const uint8_t*)begin;
	uint8_t* b_dst = (uint8_t*)dst + (b_src - b_begin);

	if (1 == word_size) {

		while (b_src > b_begin)
			*(--b_dst) = *(--b_src);

	} else {

		const uint8_t* aligned_begin = round_up (b_begin, word_size);
		const uint8_t* aligned_end = round_down (b_src, word_size);
		if (b_src > aligned_end && (uintptr_t)b_src % 2)
			*(--b_dst) = *(--b_src);

		if (word_size > 2) {
			if (b_src > aligned_end && (uintptr_t)b_src % 4) {
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
			if (word_size > 4) {
				if (b_src > aligned_end && (uintptr_t)b_src % 8) {
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
				while (b_src > aligned_begin) {
					b_dst -= 8;
					b_src -= 8;
					*(uint64_t*)b_dst = *(uint64_t*)b_src;
				}
				if (b_src > b_begin && (uintptr_t)b_begin % 8 >= 4) {
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
			} else {
				while (b_src > aligned_begin) {
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
			}
			if (b_src > b_begin && (uintptr_t)b_begin % 4 >= 2) {
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
		} else {
			while (b_src > aligned_begin) {
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
		}

		if (b_src > b_begin)
			*(--b_dst) = *(--b_src);
	}

	return b_dst;
}

}
