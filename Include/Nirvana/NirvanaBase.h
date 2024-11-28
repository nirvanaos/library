/// \file
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
#pragma once

#ifdef __cplusplus

#include <ciso646>

#if defined (_MSVC_LANG) && _MSVC_LANG > __cplusplus
#error Enable /Zc:__cplusplus MS C++ compiler option.
#endif

// Currently, we support C++ 11 and above,
// but the library is prepared to adapt to the old standard if such a need arises.
#if __cplusplus >= 201103L
#define NIRVANA_C11
#else
#error "C++11 support is required."
#endif

// For the correct new/delete implementation, if target is C++11, the library must be
// compiled in C++11 also. For the current set of compilers we always use C++14 and above.
#if __cplusplus >= 201402L
#define NIRVANA_C14
#else
#error "C++14 support is required."
#endif

#if __cplusplus >= 201703L
#define NIRVANA_C17
#endif

#if __cplusplus >= 202002L
#define NIRVANA_C20
#endif

#endif

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wnull-dereference"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wswitch-bool"
#if defined (__clang__)
#pragma GCC diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#pragma GCC diagnostic ignored "-Wsection"
#endif
#define GNU_OPTNONE __attribute__((optnone))
#else
#define GNU_OPTNONE
#endif

#ifdef _MSC_VER
#define NIRVANA_NORETURN __declspec (noreturn)
#elif defined (__GNUG__) || defined (__clang__)
#define NIRVANA_NORETURN __attribute__((__noreturn__))
#else
#define NIRVANA_NORETURN [[noreturn]]
#endif

#ifdef NIRVANA_C17
#define NIRVANA_NODISCARD [[nodiscard]]
#else
#define NIRVANA_NODISCARD
#endif

#ifdef NIRVANA_C14
#define NIRVANA_DEPRECATED(msg) [[deprecated(msg)]]
#elif _MSC_VER
#define NIRVANA_DEPRECATED(msg) __declspec (deprecated(msg))
#else
#define NIRVANA_DEPRECATED(msg)
#endif

#ifdef _MSC_VER
#define NIRVANA_NOVTABLE __declspec (novtable)
#else
#define NIRVANA_NOVTABLE
#endif

#ifdef NIRVANA_C20
#define NIRVANA_CONSTEXPR20 constexpr
#else
#define NIRVANA_CONSTEXPR20
#endif

#ifdef _MSC_VER
#define NIRVANA_NOINLINE __declspec (noinline)
#else
#define NIRVANA_NOINLINE __attribute__ ((noinline))
#endif

#if (_LIBCPP_VERSION)
#define NIRVANA_STD_BEGIN _LIBCPP_BEGIN_NAMESPACE_STD
#define NIRVANA_STD_END _LIBCPP_END_NAMESPACE_STD
#elif (__GLIBCXX__ )
#define NIRVANA_STD_BEGIN _GLIBCXX_BEGIN_NAMESPACE_VERSION
#define NIRVANA_STD_END _GLIBCXX_END_NAMESPACE_VERSION
#else
#define NIRVANA_STD_BEGIN namespace std {
#define NIRVANA_STD_END }
#endif

#ifdef __cplusplus
extern "C" {
#endif

void Nirvana_debug (const char* msg, const char* file_name, int line_number, int warning);
void Nirvana_trace (int warning, const char* file_name, int line_number, const char* format, ...);

#ifdef __cplusplus
}
#endif

#ifdef _MSC_VER
#define NIRVANA_UNREACHABLE() __assume (0)
#elif defined (__GNUG__) || defined (__clang__)
#define NIRVANA_UNREACHABLE() __builtin_unreachable ()
#endif

#ifdef NDEBUG

#define NIRVANA_UNREACHABLE_CODE() NIRVANA_UNREACHABLE ()
#define NIRVANA_TRACE(fmt, ...)
#define NIRVANA_WARNING(fmt, ...)
#define NIRVANA_ASSERT(exp) ((void)0)
#define NIRVANA_VERIFY(exp) (exp)
#define NIRVANA_ASSERT_EX(exp, warning) ((void)0)
#define NIRVANA_VERIFY_EX(exp, warning) (exp)

#else

#define NIRVANA_UNREACHABLE_CODE() { Nirvana_debug ("Executed unreachable code", __FILE__, __LINE__, 0); NIRVANA_UNREACHABLE (); }
#define NIRVANA_TRACE(fmt, ...) Nirvana_trace (0, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define NIRVANA_WARNING(fmt, ...) Nirvana_trace (1, __FILE__, __LINE__, fmt, __VA_ARGS__)
#define NIRVANA_ASSERT_EX(exp, warning) (void)((!!(exp)) || (Nirvana_debug (#exp, __FILE__, __LINE__, warning), 1))
#define NIRVANA_VERIFY_EX(exp, warning) NIRVANA_ASSERT_EX(exp, warning)
#define NIRVANA_ASSERT(exp) NIRVANA_ASSERT_EX(exp, 0)
#define NIRVANA_VERIFY(exp) NIRVANA_ASSERT(exp)

#endif

#ifdef _WIN32

typedef unsigned short _ino_t; // inode number (unused on Windows)
#define _INO_T_DEFINED

typedef long _off_t; // file offset value
#define _OFF_T_DEFINED

#endif

#endif
