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
#include "pch/pch.h"
#include <string.h>
#include <wchar.h>
#include <Nirvana/platform.h>

#if defined(_MSC_VER) && !(defined (__GNUG__) || defined (__clang__))
#pragma warning (push)
#pragma warning (disable: 4164)
#pragma function (memcpy)
#pragma function (memmove)
#pragma warning (pop)
#endif

using namespace Nirvana;

static const size_t MAX_REAL_COPY = 1024;

inline static bool use_real (void* dst, const void* src, size_t count)
{
	return count <= MAX_REAL_COPY || !PAGE_SIZE_MIN || ((uintptr_t)dst % PAGE_SIZE_MIN != (uintptr_t)src % PAGE_SIZE_MIN);
}

extern "C"
void* memcpy (void* dst, const void* src, size_t count)
{
	if (use_real (dst, src, count))
		real_copy ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		the_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}

extern "C"
void* memmove (void* dst, const void* src, size_t count)
{
	if (use_real (dst, src, count))
		real_move ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		the_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}

extern "C"
wchar_t* wmemcpy (wchar_t* dst, const wchar_t* src, size_t count)
{
	if (use_real (dst, src, count))
		real_copy (src, src + count, dst);
	else {
		count *= sizeof (wchar_t);
		the_memory->copy (dst, const_cast <wchar_t*> (src), count, Memory::SIMPLE_COPY);
	}
	return dst;
}

extern "C"
wchar_t* wmemmove (wchar_t* dst, const wchar_t* src, size_t count)
{
	if (use_real (dst, src, count))
		real_move (src, src + count, dst);
	else {
		count *= sizeof (wchar_t);
		the_memory->copy (dst, const_cast <wchar_t*> (src), count, Memory::SIMPLE_COPY);
	}
	return dst;
}
