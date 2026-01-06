#include <Nirvana/boolean_hash.h>
#include <Nirvana/Hash.h>

namespace {

inline size_t get_next (const CORBA::Octet*& ar, size_t& size)
{
	size_t s = sizeof (size_t) * 8;
	if (s > size)
		s = size;
	size -= s;
	size_t bits = 0;
	while (s--) {
		if (*(ar++))
			bits |= 1;
		bits <<= 1;
	}
	return bits;
}

}

namespace Nirvana {

size_t boolean_hash (const CORBA::Octet* ar, size_t size) noexcept
{
	size_t ret = 0;
	if (size) {
		size_t bits = get_next (ar, size);
		ret = Hash::hash_bytes (&bits, sizeof (bits));
		while (size) {
			bits = get_next (ar, size);
			ret = Hash::append_bytes (ret, &bits, sizeof (bits));
		}
	}
	return ret;
}

}

