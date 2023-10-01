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
#include "lpch.h"
#include <Nirvana/real_copy.h>
#include <Nirvana/bitutils.h>
#include <Nirvana/platform.h>

namespace Nirvana {

static unsigned get_word_size (const void* src, const void* dst, size_t size)
{
	assert (size);

	const unsigned PLATFORM_WORD_SIZE = sizeof (size_t) >= 8 ? 8 : (sizeof (size_t) >= 4 ? 4 : 2);
	const bool STRICT_ALIGN = (PLATFORM == PLATFORM_ARM || PLATFORM == PLATFORM_ARM64);

	unsigned word_size;
	if (STRICT_ALIGN) {
		word_size = PLATFORM_WORD_SIZE - 1;
		unsigned x = (unsigned)(uintptr_t)src ^ (unsigned)(uintptr_t)dst;
		while (x & word_size) {
			if (!(word_size >>= 1))
				break;
		}
		++word_size;
	} else
		word_size = PLATFORM_WORD_SIZE;

	if ((size_t)word_size > size) {
		word_size = (unsigned)size;
		// Round down to a power of two
		word_size |= word_size >> 1;
		if (PLATFORM_WORD_SIZE > 2) {
			word_size |= word_size >> 2;
			if (PLATFORM_WORD_SIZE > 4) {
				word_size |= word_size >> 4;
			}
		}
		word_size -= word_size >> 1;
	}

	return word_size;
}

template <>
void* real_copy (const void* begin, const void* end, void* dst)
{
	ptrdiff_t size = (const uint8_t*)end - (const uint8_t*)begin;
	if (size <= 0)
		return dst;

	unsigned word_size = get_word_size (begin, dst, size);

	uint8_t* b_dst = (uint8_t*)dst;
	uint8_t* b_end = b_dst + size;
	const uint8_t* b_src = (const uint8_t*)begin;

	if (1 == word_size) {

		while (b_dst < b_end)
			*(b_dst++) = *(b_src++);

	} else {

		uint8_t* aligned_begin = round_up (b_dst, word_size);
		uint8_t* aligned_end = round_down (b_end, word_size);

		if (b_dst < aligned_begin && (uintptr_t)b_dst % 2)
			*(b_dst++) = *(b_src++);

		assert ((uintptr_t)b_dst % 2 == 0);
		if (word_size > 2) {
			if (b_dst < aligned_begin && (uintptr_t)b_dst % 4) {
				assert (aligned_begin - b_dst >= 2);
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
			if (word_size > 4) {
				if (b_dst < aligned_begin && (uintptr_t)b_dst % 8) {
					assert (aligned_begin - b_dst >= 4);
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
				while (b_dst < aligned_end) {
					assert (aligned_end - b_dst >= 8);
					*(uint64_t*)b_dst = *(const uint64_t*)b_src;
					b_dst += 8;
					b_src += 8;
				}
				if (b_end - b_dst >= 4) {
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
			} else {
				while (b_dst < aligned_end) {
					assert (aligned_end - b_dst >= 4);
					*(uint32_t*)b_dst = *(const uint32_t*)b_src;
					b_dst += 4;
					b_src += 4;
				}
			}
			if (b_end - b_dst >= 2) {
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
		} else {
			while (b_dst < aligned_end) {
				assert (aligned_end - b_dst >= 2);
				*(uint16_t*)b_dst = *(const uint16_t*)b_src;
				b_dst += 2;
				b_src += 2;
			}
		}

		if (b_dst < b_end)
			*(b_dst++) = *(b_src++);

		assert (b_dst == b_end);
	}

	return b_dst;
}

template <>
void* real_copy_backward (const void* begin, const void* end, void* dst_end)
{
	ptrdiff_t size = (const uint8_t*)end - (const uint8_t*)begin;
	if (size <= 0)
		return dst_end;

	unsigned word_size = get_word_size (begin, dst_end, size);

	uint8_t* b_dst = (uint8_t*)dst_end;
	uint8_t* b_begin = b_dst - size;
	const uint8_t* b_src = (const uint8_t*)end;

	if (1 == word_size) {

		while (b_dst > b_begin)
			*(--b_dst) = *(--b_src);

	} else {

		uint8_t* aligned_begin = round_up (b_begin, word_size);
		uint8_t* aligned_end = round_down (b_dst, word_size);

		if (b_dst > aligned_end && (uintptr_t)b_dst % 2)
			*(--b_dst) = *(--b_src);

		assert ((uintptr_t)b_dst % 2 == 0);
		if (word_size > 2) {
			if (b_dst > aligned_end && (uintptr_t)b_dst % 4) {
				assert (b_dst - aligned_end >= 2);
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
			if (word_size > 4) {
				if (b_dst > aligned_end && (uintptr_t)b_dst % 8) {
					assert (b_dst - aligned_end >= 4);
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
				while (b_dst > aligned_begin) {
					assert (b_dst - aligned_begin >= 8);
					b_dst -= 8;
					b_src -= 8;
					*(uint64_t*)b_dst = *(uint64_t*)b_src;
				}
				if (b_dst - b_begin >= 4) {
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
			} else {
				while (b_dst > aligned_begin) {
					assert (b_dst - aligned_begin >= 4);
					b_dst -= 4;
					b_src -= 4;
					*(uint32_t*)b_dst = *(uint32_t*)b_src;
				}
			}
			if (b_dst - b_begin >= 2) {
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
		} else {
			while (b_dst > aligned_begin) {
				assert (b_dst - aligned_begin >= 2);
				b_dst -= 2;
				b_src -= 2;
				*(uint16_t*)b_dst = *(uint16_t*)b_src;
			}
		}

		if (b_dst > b_begin)
			*(--b_dst) = *(--b_src);
		 
		assert (b_dst == b_begin);
	}

	return b_dst;
}

}
