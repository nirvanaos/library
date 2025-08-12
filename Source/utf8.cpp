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
#include "../../pch/pch.h"
#include <Nirvana/utf8.h>

namespace Nirvana {

bool is_valid_utf8 (const char* p, size_t size) noexcept
{
	while (size >= 4) {
		uint32_t c4 = *(const uint32_t*)p;
		if ((c4 & 0x80808080) == 0) {
			p += 4;
			size -= 4;
		} else if ((c4 & (endian::native == endian::big ? 0x80808000 : 0x00808080)) == 0) {
			p += 3;
			size -= 3;
		} else if ((c4 & (endian::native == endian::big ? 0x80800000 : 0x00008080)) == 0) {
			p += 2;
			size -= 2;
		} else if ((c4 & (endian::native == endian::big ? 0x80000000 : 0x00000080)) == 0) {
			++p;
			--size;
		} else if ((c4 & (endian::native == endian::big ? 0xE0000000 : 0x000000E0))
			== (endian::native == endian::big ? 0xC0000000 : 0x000000C0)) {
			// 2 octets
			if ((c4 & (endian::native == endian::big ? 0x00C00000 : 0x0000C000))
				!= (endian::native == endian::big ? 0x00800000 : 0x00008000))
				return false;
			p += 2;
			size -= 2;
		} else if ((c4 & (endian::native == endian::big ? 0xF0000000 : 0x000000F0))
			== (endian::native == endian::big ? 0xE0000000 : 0x000000E0)) {
			// 3 octets
			if ((c4 & 0x00C0C000) != 0x00808000)
				return false;
			p += 3;
			size -= 3;
		} else if ((c4 & (endian::native == endian::big ? 0xF8000000 : 0x000000F8))
			== (endian::native == endian::big ? 0xF0000000 : 0x000000F0)) {
			// 4 octets
			if ((c4 & (endian::native == endian::big ? 0x00C0C0C0 : 0xC0C0C000))
				!= (endian::native == endian::big ? 0x00808080 : 0x80808000))
				return false;
			p += 4;
			size -= 4;
		} else
			return false;
	}

	while (size > 0) {
		unsigned c = *(p++);
		--size;
		if (c & 0x80) {
			size_t ocnt; // additional octets count
			if ((c & 0xE0) == 0xC0)
				ocnt = 1;
			else if ((c & 0xF0) == 0xE0)
				ocnt = 2;
			else if ((c & 0xF8) == 0xF0)
				ocnt = 3;
			else
				return false;
			if (ocnt > size)
				return false;
			while (ocnt--) {
				c = *(p++);
				--size;
				if ((c & 0xC0) != 0x80)
					return false;
			}
		}
	}

	return true;
}

int octet_cnt (int b) noexcept
{
	if (!(b & 0x80))
		return 1;
	else if ((b & 0xE0) == 0xC0)
		return 2;
	else if ((b & 0xF0) == 0xE0)
		return 3;
	else if ((b & 0xF8) == 0xF0)
		return 4;
	else
		return 0;
}

bool push_first (__Mbstate& mbs, int b) noexcept
{
	switch (octet_cnt (b)) {
		case 1:
			mbs.__wchar = b;
			mbs.__octets = 0;
			break;
		case 2:
			mbs.__wchar = (b & 0x1F) << 6;
			mbs.__octets = 1;
			break;
		case 3:
			mbs.__wchar = (b & 0x0F) << 12;
			mbs.__octets = 2;
			break;
		case 4:
			mbs.__wchar = (b & 0x07) << 18;
			mbs.__octets = 3;
			break;
		default:
			return false;
	}

	return true;
}

bool push_next (__Mbstate& mbs, int b) noexcept
{
	assert (mbs.__octets);
	if ((b & 0xC0) != 0x80)
		return false;
	mbs.__wchar |= (b & 0x3F) << ((mbs.__octets - 1) * 6);
	--mbs.__octets;
	return true;
}

bool push_wide (__Mbstate& mbs, uint32_t wc) noexcept
{
	if ((unsigned)wc <= 0x7F) {
		// 1 octet
		mbs.__wchar = (unsigned)wc;
		mbs.__octets = 1;
	} else if (wc <= 0x7FF) {
		// 2 octets
		mbs.__wchar = (((wc >> 6) & 0x1F) | 0xC0)
			| (((wc & 0x3F) | 0x80) << 8);
		mbs.__octets = 2;
	} else if (wc <= 0xFFFF) {
		// 3 octets
		mbs.__wchar = ((((unsigned)wc >> 12) & 0x0F) | 0xE0)
			| (((((unsigned)wc >> 6) & 0x3F) | 0x80) << 8)
			| ((((unsigned)wc & 0x3F) | 0x80) << 16);
		mbs.__octets = 3;
	} else if (wc <= 0x0010FFFF) {
		// 4 octets
		mbs.__wchar = ((((unsigned)wc >> 18) & 0x07) | 0xF0)
			| (((((unsigned)wc >> 12) & 0x3F) | 0x80) << 8)
			| (((((unsigned)wc >> 6) & 0x3F) | 0x80) << 16)
			| ((((unsigned)wc & 0x3F) | 0x80) << 24);
		mbs.__octets = 4;
	} else
		return false;

	return true;
}

int pop_octet (__Mbstate& mbs) noexcept
{
	assert (mbs.__octets);
	int octet = mbs.__wchar & 0xFF;
	mbs.__wchar >>= 8;
	--mbs.__octets;
	return octet;
}

}
