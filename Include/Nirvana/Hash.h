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
struct FNV1a
{
	typedef FNV_Traits <sizeof (size_t) * 8> Traits;

	static size_t append_bytes (size_t hash, const uint8_t* begin, const size_t len) NIRVANA_NOEXCEPT
	{
		for (const uint8_t* end = begin + len; begin != end; ++begin) {
			hash ^= static_cast <size_t>(*begin);
			hash *= Traits::prime;
		}

		return hash;
	}

	static size_t hash_bytes (const uint8_t* begin, const size_t len) NIRVANA_NOEXCEPT
	{
		return append_bytes (Traits::offset_basis, begin, len);
	}
};

typedef FNV1a Hash;

}

#endif
