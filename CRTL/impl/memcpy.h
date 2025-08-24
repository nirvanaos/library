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
#ifndef CRTL_IMPL_STRUTL_H_
#define CRTL_IMPL_STRUTL_H_
#pragma once

#include <Nirvana/platform.h>
#include <Nirvana/real_copy.h>

namespace CRTL {

static const size_t MAX_REAL_COPY = 1024;

using namespace Nirvana;

inline static bool use_real (void* dst, const void* src, size_t count) noexcept
{
	return count <= MAX_REAL_COPY || !PAGE_SIZE_MIN || ((uintptr_t)dst % PAGE_SIZE_MIN != (uintptr_t)src % PAGE_SIZE_MIN);
}

template <typename C> 
C* memcpy (C* dst, const C* src, size_t count) noexcept
{
	if (use_real (dst, src, count))
		real_move (src, src + count, dst);
	else {
		size_t cb = count * sizeof (C);
		the_memory->copy (dst, const_cast <C*> (src), cb, Memory::SIMPLE_COPY);
	}
	return dst;
}

}

#endif
