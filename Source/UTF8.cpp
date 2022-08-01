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
#include <Nirvana/UTF8.h>
#include <Nirvana/throw_exception.h>
#include <stdexcept>

namespace Nirvana {

inline
static unsigned get_next_octet (const char*& p, const char* end)
{
	if (p >= end)
		throw_CODESET_INCOMPATIBLE ();
	unsigned c = *(p++);
	if ((c & 0xC0) != 0x80)
		throw_CODESET_INCOMPATIBLE ();
	return c & 0x3F;
}

uint32_t utf8_to_utf32 (const char*& begin, const char* end)
{
	const char* p = begin;
	if (p >= end)
		throw_BAD_PARAM ();
	uint32_t wc;
	uint32_t c = *(p++);
	if (!(c & 0x80)) {
		// 1 octet
		wc = c;
	} else if ((c & 0xE0) == 0xC0) {
		// 2 octets
		wc = (c & 0x1F) << 6;
		wc |= get_next_octet (p, end);
	} else if ((c & 0xF0) == 0xE0) {
		// 3 octets
		wc = (c & 0x0F) << 12;
		wc |= get_next_octet (p, end) << 6;
		wc |= get_next_octet (p, end);
	} else if ((c & 0xF8) == 0xF0) {
		// 4 octets
		wc = (c & 0x07) << 18;
		wc |= get_next_octet (p, end) << 12;
		wc |= get_next_octet (p, end) << 6;
		wc |= get_next_octet (p, end);
	} else
		throw_CODESET_INCOMPATIBLE ();

	begin = p;
	return wc;
}

inline
static void put_next_octet (unsigned c, char*& out, const char* end)
{
	if (out >= end)
		throw std::length_error ("Not enough space");
	*(out++) = (char)c;
}

char* utf32_to_utf8 (uint32_t wc, char* out, const char* end)
{
	if (wc <= 0x7F) {
		// 1 octet
		put_next_octet ((unsigned)wc, out, end);
	} else if (wc <= 0x7FF) {
		// 2 octets
		put_next_octet ((((unsigned)wc >> 6) & 0x1F) | 0xC0, out, end);
		put_next_octet (((unsigned)wc & 0x3F) | 0x80, out, end);
	} else if (wc <= 0xFFFF) {
		// 3 octets
		put_next_octet ((((unsigned)wc >> 12) & 0x0F) | 0xE0, out, end);
		put_next_octet ((((unsigned)wc >> 6) & 0x3F) | 0x80, out, end);
		put_next_octet (((unsigned)wc & 0x3F) | 0x80, out, end);
	} else if (wc <= 0x0010FFFF) {
		// 4 octets
		put_next_octet ((unsigned)((wc >> 18) & 0x07) | 0xF0, out, end);
		put_next_octet ((((unsigned)wc >> 12) & 0x3F) | 0x80, out, end);
		put_next_octet ((((unsigned)wc >> 6) & 0x3F) | 0x80, out, end);
		put_next_octet (((unsigned)wc & 0x3F) | 0x80, out, end);
	} else
		throw_CODESET_INCOMPATIBLE ();

	return out;
}

}
