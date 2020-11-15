#ifndef NIRVANA_HASH_H_
#define NIRVANA_HASH_H_

#include "NirvanaBase.h"
#include <type_traits>

namespace Nirvana {

// Fowler–Noll–Vo hash

template <size_t bits> struct FNV_Traits;

template <> struct FNV_Traits <32>
{
	static const uint32_t offset_basis = 2166136261U;
	static const uint32_t prime = 16777619U;
};

template <> struct FNV_Traits <64>
{
	static const uint64_t offset_basis = 14695981039346656037ULL;
	static const uint64_t prime = 1099511628211ULL;
};

/// FNV1a
template <typename T = size_t>
struct FNV1a
{
	typedef FNV_Traits <sizeof (T) * 8> Traits;

	static T append_bytes (T hash, const void* begin, const size_t len) NIRVANA_NOEXCEPT
	{
		for (const uint8_t* p = (const uint8_t*)begin, *end = p + len; p != end; ++p) {
			hash ^= static_cast <T>(*p);
			hash *= Traits::prime;
		}

		return hash;
	}

	static T hash_bytes (const void* begin, const size_t len) NIRVANA_NOEXCEPT
	{
		return append_bytes (Traits::offset_basis, begin, len);
	}
};

typedef FNV1a <size_t> Hash;

}

#endif
