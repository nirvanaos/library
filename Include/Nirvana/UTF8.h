/// \file
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
#ifndef NIRVANA_UTF8_H_
#define NIRVANA_UTF8_H_
#pragma once

#include "NirvanaBase.h"

namespace Nirvana {

inline
bool is_valid_utf8 (const char* p, size_t size)
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

/// Converts UTF-8 to UTF-32.
/// 
/// \param [in, out] begin UTF-8 sequence begin.
/// \param end UTF-8 sequence end.
/// \returns UTF-32 character.
/// \throws CORBA::CODESET_INCOMPATIBLE Not valid UTF-8 input.
/// \throws CORBA::BAD_PARAM begin >= end.
uint32_t utf8_to_utf32 (const char*& begin, const char* end);

/// Converts UTF-32 to UTF-8
/// 
/// \param wc Wide character
/// \param out Output buffer.
/// \param end Output buffer end. At least 4 octets must be available.
/// \returns Pointer to the buffer beyond the end of converted sequence.
/// \throws CORBA::CODESET_INCOMPATIBLE /p wc is not a valid UTF-32 character.
/// \throws std::length_error Not enough space in the output buffer.
char* utf32_to_utf8 (uint32_t wc, char* out, const char* end);

}

#endif
