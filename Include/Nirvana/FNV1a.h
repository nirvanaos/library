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
#ifndef NIRVANA_FNV1A_H_
#define NIRVANA_FNV1A_H_
#pragma once

#include "NirvanaBase.h"
#include <stdint.h>

namespace Nirvana {

// Fowler-Noll-Vo hash

namespace FNV1 {

template <typename T>
T append_bytes (T hash, const T prime, const void* begin, const size_t len) noexcept
{
	for (const uint8_t* p = (const uint8_t*)begin, *end = p + len; p != end; ++p) {
		hash ^= static_cast <T>(*p);
		hash *= prime;
	}

	return hash;
}

}

template <size_t bits> class FNV1a;

template <> class FNV1a <32>
{
public:
	static uint32_t append_bytes (uint32_t hash, const void* begin, const size_t len) noexcept
	{
		return FNV1::append_bytes (hash, prime_, begin, len);
	}

	static uint32_t hash_bytes (const void* begin, const size_t len) noexcept
	{
		return append_bytes (offset_basis_, begin, len);
	}

private:
	static const uint32_t offset_basis_ = 2166136261U;
	static const uint32_t prime_ = 16777619U;
};

template <> class FNV1a <64>
{
public:
	static uint64_t append_bytes (uint64_t hash, const void* begin, const size_t len) noexcept
	{
		return FNV1::append_bytes (hash, prime_, begin, len);
	}

	static uint64_t hash_bytes (const void* begin, const size_t len) noexcept
	{
		return append_bytes (offset_basis_, begin, len);
	}

private:
	static const uint64_t offset_basis_ = 14695981039346656037ULL;
	static const uint64_t prime_ = 1099511628211ULL;
};

}

#endif
