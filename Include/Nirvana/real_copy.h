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
#ifndef NIRVANA_REAL_COPY_H_
#define NIRVANA_REAL_COPY_H_
#pragma once

#include <stdint.h>
#include <Nirvana/platform.h>

namespace Nirvana {

template <typename T> inline
T* real_copy (const T* begin, const T* end, T* dst)
{
	if (sizeof (T) < sizeof (Word) || ((uintptr_t)begin & (sizeof (T) - 1)))
		return (T*)real_copy ((const void*)begin, (const void*)end, (void*)dst);
	else {
		while (begin < end)
			*(dst++) = *(begin++);
		return dst;
	}
}

template <>
void* real_copy (const void* begin, const void* end, void* dst);

template <typename T> inline
T* real_copy_backward (const T* begin, const T* end, T* dst_end)
{
	if (sizeof (T) < sizeof (Word) || ((uintptr_t)begin & (sizeof (T) - 1)))
		return (T*)real_copy_backward ((const void*)begin, (const void*)end, (void*)dst_end);
	else {
		while (begin < end)
			*(--dst_end) = *(--end);
		return dst_end;
	}
}

template <>
void* real_copy_backward (const void* begin, const void* end, void* dst_end);

template <typename T> inline
void real_move (const T* begin, const T* end, T* dst)
{
	if (dst < begin)
		real_copy (begin, end, dst);
	else if (dst > begin)
		real_copy_backward (begin, end, dst + (end - begin));
}

template <> inline
void real_move (const void* begin, const void* end, void* dst)
{
	if (dst < begin)
		real_copy (begin, end, dst);
	else if (dst > begin)
		real_copy_backward (begin, end, (void*)((uint8_t*)dst
			+ ((const uint8_t*)end - (const uint8_t*)begin)));
}

}

#endif
