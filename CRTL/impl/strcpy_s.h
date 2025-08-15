/// \file
/*
* Nirvana C runtime library.
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
#ifndef CRTL_IMPL_STRCPY_S_H_
#define CRTL_IMPL_STRCPY_S_H_

#include <Nirvana/Nirvana.h>
#include <Nirvana/errors.h>
#include "strlen.h"
#include <stdlib.h>

namespace CRTL {

template <typename C> inline
errno_t strcpy_s (C* dst, rsize_t dst_size, const C* src) noexcept
{
	if (!dst)
		return EINVAL;
	if (!src) {
		dst [0] = 0;
		return EINVAL;
	}
	size_t src_size = strnlen (src, dst_size);
	if (dst_size <= src_size) {
		dst [0] = 0;
		return ERANGE;
	}
	++src_size;
	size_t cb = src_size * sizeof (C);
	Nirvana::the_memory->copy (dst, const_cast <C*> (src), cb, 0);
	return 0;
}

template <typename C> inline
errno_t strncpy_s (C* dst, rsize_t dst_size, const C* src, size_t count) noexcept
{
	if (!dst)
		return EINVAL;
	if (!src) {
		dst [0] = 0;
		return EINVAL;
	}
	size_t src_size = strnlen (src, std::min (dst_size, count));
	if (dst_size <= src_size) {
		dst [0] = 0;
		return ERANGE;
	}
	++src_size;
	size_t cb = src_size * sizeof (C);
	Nirvana::the_memory->copy (dst, const_cast <C*> (src), cb, 0);
	return 0;
}

}

#endif
