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
#ifndef NIRVANA_NIRVANABASE_H_
#define NIRVANA_NIRVANABASE_H_

#include <stdint.h>
#include <stdlib.h>

#ifndef countof
#define countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef __cplusplus

#if __cplusplus <= 1
#define NIRVANA_C11 // C++ 11 by default
#else

#if __cplusplus >= 201103L || _MSVC_LANG >= 201103L
#define NIRVANA_C11
#else
#error "C++11 support is required."
#endif

#if __cplusplus >= 201402L || _MSVC_LANG >= 201402L
#define NIRVANA_C14
#endif

#if __cplusplus >= 201703L || _MSVC_LANG >= 201703L
#define NIRVANA_C17
#endif

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wnull-dereference"
#pragma GCC diagnostic ignored "-Wsection"
#pragma GCC diagnostic ignored "-Wswitch"
#define GNU_OPTNONE __attribute__((optnone))
#else
#define GNU_OPTNONE
#endif

#endif

#ifdef NIRVANA_C11
#define NIRVANA_NORETURN [[noreturn]]
#define NIRVANA_NOEXCEPT noexcept
#else
#ifdef _MSC_BUILD
#define NIRVANA_NORETURN __declspec (noreturn)
#else
#define NIRVANA_NORETURN
#endif
#define NIRVANA_NOEXCEPT throw ()
#endif

#ifdef NIRVANA_C17
#define NIRVANA_NODISCARD [[nodiscard]]
#else
#define NIRVANA_NODISCARD
#endif

#ifdef NIRVANA_C14
#define NIRVANA_DEPRECATED [[deprecated]]
#elif _MSC_BUILD
#define NIRVANA_DEPRECATED __declspec (deprecated)
#endif

#ifdef _MSC_BUILD
#define NIRVANA_NOVTABLE __declspec (novtable)
#else
#define NIRVANA_NOVTABLE
#endif

namespace Nirvana {

// Endian order
enum class endian
{
	little = 0x41424344UL,
	big = 0x44434241UL,
	native = 'ABCD'
};

// Native IDL types
typedef void* Pointer;
typedef const void* ConstPointer;
typedef size_t Size; ///< Memory size
typedef ptrdiff_t PtrDiff; ///< Pointers difference
typedef uintptr_t UIntPtr; ///< Integer to fit the pointer

typedef uint64_t DeadlineTime;

// Native machine word integer types for fast performance.
// Currently defined as ptrdiff_t and size_t.
// But for some platforms may be defined differ.
typedef ptrdiff_t Word; ///< Native machine word
typedef size_t UWord; ///< Native machine unsigned word

const DeadlineTime INFINITE_DEADLINE = UINT64_MAX;

// Integral rounding

template <typename T>
inline T* round_down (T* p, uintptr_t n2)
{
	return (T*)((uintptr_t)p / n2 * n2);
}

template <typename T>
inline T* round_up (T* p, uintptr_t n2)
{
	return (T*)(((uintptr_t)p + n2 - 1) / n2 * n2);
}

inline UWord round_down (UWord i, UWord n2)
{
	return i / n2 * n2;
}

inline UWord round_up (UWord i, UWord n2)
{
	return (i + n2 - 1) / n2 * n2;
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
