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
#ifndef NIRVANA_MURMURHASH_H_
#define NIRVANA_MURMURHASH_H_

#ifdef __cplusplus
#include <Nirvana/platform.h>
extern "C" {
#else
#include <stdint.h>
#endif

uint32_t MurmurHash2        ( const void * key, int len, uint32_t seed );
uint32_t MurmurHash2A       ( const void * key, int len, uint32_t seed );
uint32_t MurmurHashNeutral2 ( const void * key, int len, uint32_t seed );
uint32_t MurmurHashAligned2 ( const void * key, int len, uint32_t seed );
uint64_t MurmurHash64A      ( const void * key, int len, uint64_t seed );
uint64_t MurmurHash64B      ( const void * key, int len, uint64_t seed );

#ifdef __cplusplus
}

namespace Nirvana {

template <size_t bits> class MurmurHash;

template <>
class MurmurHash <32>
{
public:
	static uint32_t append_bytes (uint32_t hash, const void* begin, const size_t len) noexcept
	{
		return murmur_hash (begin, len, hash);
	}

	static uint32_t hash_bytes (const void* begin, const size_t len) noexcept
	{
		return append_bytes (seed_, begin, len);
	}

private:
	static uint32_t murmur_hash (const void* key, size_t len, uint32_t seed)
	{
		if (Nirvana::STRICT_ALIGN)
			return MurmurHashAligned2 (key, (int)len, seed);
		else
			return MurmurHash2A (key, (int)len, seed);
	}

	static const uint32_t seed_ = 1; // TBD
};

template <>
class MurmurHash <64>
{
public:
	static uint64_t append_bytes (uint64_t hash, const void* begin, const size_t len) noexcept
	{
		return murmur_hash (begin, len, hash);
	}

	static uint64_t hash_bytes (const void* begin, const size_t len) noexcept
	{
		return append_bytes (seed_, begin, len);
	}

private:
	static uint64_t murmur_hash (const void* key, size_t len, uint64_t seed)
	{
		if (sizeof (UWord) > 4)
			return MurmurHash64A (key, (int)len, seed);
		else
			return MurmurHash64B (key, (int)len, seed);
	}

	static const uint64_t seed_ = 1; // TBD
};

}

#endif

#endif 
