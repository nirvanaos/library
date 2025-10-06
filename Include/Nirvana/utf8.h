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

#include <stddef.h>
#include "mbstate.h"

namespace Nirvana {

/// Check that UTF-8 string is valid.
/// 
/// \param p String pointer.
/// \param size String size.
/// \returns `true` if \p p is valid UTF-8 string, `false` if not.
bool is_valid_utf8 (const char* p, size_t size) noexcept;

bool push_first (__Mbstate& mbs, int b) noexcept;
bool push_next (__Mbstate& mbs, int b) noexcept;

int octet_cnt (int b) noexcept;

bool push_wide (__Mbstate& mbs, uint32_t wc) noexcept;
int pop_octet (__Mbstate& mbs) noexcept;

inline bool state_empty (const __Mbstate& mbs) noexcept
{
	return !mbs.__octets && !mbs.__wchar;
}

inline void state_clear (__Mbstate& mbs) noexcept
{
	mbs.__wchar = 0;
	mbs.__octets = 0;
}

}

#endif
