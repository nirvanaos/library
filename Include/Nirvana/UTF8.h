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

#include <stdint.h>

namespace Nirvana {

/// Check that UTF-8 string is valid.
/// 
/// \param p String pointer.
/// \param size String size.
/// \returns `true` if \p p is valid UTF-8 string, `false` if not.
bool is_valid_utf8 (const char* p, size_t size) noexcept;

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
