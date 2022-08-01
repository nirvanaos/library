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

namespace Nirvana {

template <typename T> inline
T* real_copy (const T* begin, const T* end, T* dst)
{
	while (begin != end)
		*(dst++) = *(begin++);

	return dst;
}

// Specialization for performance

template <>
uint8_t* real_copy (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

template <> inline
int8_t* real_copy (const int8_t* begin, const int8_t* end, int8_t* dst)
{
	return (int8_t*)real_copy ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#if SIZE_MAX >= 0xffffffffui32

template <> inline
uint16_t* real_copy (const uint16_t* begin, const uint16_t* end, uint16_t* dst)
{
	return (uint16_t*)real_copy ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

template <> inline
int16_t* real_copy (const int16_t* begin, const int16_t* end, int16_t* dst)
{
	return (int16_t*)real_copy ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#if SIZE_MAX >= 0xffffffffffffffffui64

template <> inline
uint32_t* real_copy (const uint32_t* begin, const uint32_t* end, uint32_t* dst)
{
	return (uint32_t*)real_copy ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

template <> inline
int32_t* real_copy (const int32_t* begin, const int32_t* end, int32_t* dst)
{
	return (int32_t*)real_copy ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#endif

#endif

template <typename T> inline
void real_move (const T* begin, const T* end, T* dst)
{
	if (dst <= begin || dst >= end)
		while (begin != end)
			*(dst++) = *(begin++);
	else
		while (begin != end)
			*(--dst) = *(--end);
}

// Specialization for performance

template <>
void real_move (const uint8_t* begin, const uint8_t* end, uint8_t* dst);

template <> inline
void real_move (const int8_t* begin, const int8_t* end, int8_t* dst)
{
	real_move ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#if SIZE_MAX >= 0xffffffffui32

template <> inline
void real_move (const uint16_t* begin, const uint16_t* end, uint16_t* dst)
{
	real_move ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

template <> inline
void real_move (const int16_t* begin, const int16_t* end, int16_t* dst)
{
	real_move ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#if SIZE_MAX >= 0xffffffffffffffffui64

template <> inline
void real_move (const uint32_t* begin, const uint32_t* end, uint32_t* dst)
{
	real_move ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

template <> inline
void real_move (const int32_t* begin, const int32_t* end, int32_t* dst)
{
	real_move ((const uint8_t*)begin, (const uint8_t*)end, (uint8_t*)dst);
}

#endif

#endif


}

#endif
