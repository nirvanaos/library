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

using namespace Nirvana;

static const size_t MAX_REAL_COPY = 1024;

extern "C"
void* memcpy (void* dst, const void* src, size_t count)
{
	if (count <= MAX_REAL_COPY)
		real_copy ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		g_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}

extern "C"
void* memmove (void* dst, const void* src, size_t count)
{
	if (count <= MAX_REAL_COPY)
		real_move ((const void*)src, (const void*)((const uint8_t*)src + count), dst);
	else
		g_memory->copy (dst, const_cast <void*> (src), count, Memory::SIMPLE_COPY);
	return dst;
}
