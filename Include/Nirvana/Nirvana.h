// Nirvana project.
// Main header.
#ifndef NIRVANA_NIRVANA_H_
#define NIRVANA_NIRVANA_H_

#include <stdint.h>
#include <stdlib.h>

#ifndef countof
#if defined (_countof)
#define countof _countof
#else
#define countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif
#endif

//#include <assert.h>

#undef verify
#undef assert

#ifdef NDEBUG

#define assert(exp) ((void)0)
#define verify(exp) (exp)

#else

#define assert(exp) (void)((!!(exp)) ||  (__debugbreak (), 1))
#define verify(exp) assert(exp)

#endif

#ifdef __cplusplus

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
typedef uintptr_t UWord;
typedef intptr_t Word;
typedef uint64_t DeadlineTime;

/// Integral rounding
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

/// Zero memory
template <class It>
inline void zero (It begin, It end)
{
	while (begin != end)
		*(begin++) = 0;
}

/// constant ceil(log2(n))
constexpr size_t log2_ceil (size_t n)
{
	return (n > 1) ? 1 + log2_ceil ((n + 1) / 2) : 0;
}

}

#endif
#endif
