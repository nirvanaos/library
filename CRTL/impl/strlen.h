/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#ifndef CRTL_IMPL_STRLEN_H_
#define CRTL_IMPL_STRLEN_H_
#pragma once

#include "strchr.h"

namespace CRTL {

template <typename C> inline
size_t strlen (const C* s)
{
	return strfind (s, std::numeric_limits <size_t>::max (), 0, true) - s;
}

template <typename C> inline
size_t strnlen (const C* s, size_t maxlen)
{
	return strfind (s, maxlen, 0, true) - s;
}

}

#endif
