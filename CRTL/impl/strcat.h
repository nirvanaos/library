/// \file
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
#ifndef CRTL_IMPL_STRCAT_H_
#define CRTL_IMPL_STRCAT_H_
#pragma once
#include "strlen.h"

namespace CRTL {

template <typename C> inline
errno_t strcat (C* dst, size_t dst_size, const C* src, size_t count)
{
	if (!dst || !src)
		return EINVAL;
	if (dst <= src && src < dst + dst_size)
		return EINVAL;
	size_t dst_len = strnlen (dst, dst_size);
	if (dst_len >= dst_size)
		return ERANGE;
	size_t src_max = std::min (dst_size - dst_len, count);
	size_t src_len = strnlen (src, src_max);
	if (src_len >= src_max)
		return ERANGE;

	++src_len;
	size_t cb = src_len * sizeof (C);
	Nirvana::the_memory->copy (dst + dst_len, const_cast <C*> (src), cb, 0);
	return 0;
}

}

#endif
