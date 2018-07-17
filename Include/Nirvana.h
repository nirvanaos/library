// Nirvana project.
// Main header.
#ifndef NIRVANA_NIRVANA_H_
#define NIRVANA_NIRVANA_H_

#include <BasicTypes.h>
#include <assert.h>

#undef verify

#ifdef NDEBUG

#define verify(exp) (exp)

#else

#define verify(exp) assert(exp)

#endif

// Endian order
namespace std {

enum class endian
{
	little = 0x41424344UL,
	big = 0x44434241UL,
	native = 'ABCD'
};

}

namespace Nirvana {

// Native types
typedef void* Pointer;
typedef const void* ConstPointer;
typedef size_t UWord;
typedef ptrdiff_t Word;

// Integral rounding

template <typename I>
inline I round_down (I i, UWord n2)
{
	return (I)((UWord)i / n2 * n2);
}

template <typename I>
inline I round_up (I i, UWord n2)
{
	return (I)(((UWord)i + n2 - 1) / n2 * n2);
}

// Zero memory

template <class It>
inline void zero (It begin, It end)
{
	while (begin != end)
		*(begin++) = 0;
}

}

#endif
