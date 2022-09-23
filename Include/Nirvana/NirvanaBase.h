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

#include <stdint.h>

#ifndef countof
#define countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

#ifdef __cplusplus

#if defined (_MSVC_LANG) && _MSVC_LANG > __cplusplus
#error Enable /Zc:__cplusplus MS C++ compiler option.
#endif

#if __cplusplus >= 201103L
#define NIRVANA_C11
#else
#error "C++11 support is required."
#endif

#if __cplusplus >= 201402L
#define NIRVANA_C14
#endif

#if __cplusplus >= 201703L
#define NIRVANA_C17
#endif

#if __cplusplus >= 202002L
#define NIRVANA_C20
#endif

#if defined (__GNUG__) || defined (__clang__)
#pragma GCC diagnostic ignored "-Wnull-dereference"
#pragma GCC diagnostic ignored "-Wsection"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wswitch-bool"
#pragma GCC diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#define GNU_OPTNONE __attribute__((optnone))
#else
#define GNU_OPTNONE
#endif

#endif

#ifdef NIRVANA_C11
#define NIRVANA_NOEXCEPT noexcept
#else
#define NIRVANA_NOEXCEPT throw ()
#endif

#ifdef _MSC_BUILD
#define NIRVANA_NORETURN __declspec (noreturn)
#else
#ifdef NIRVANA_C11
#define NIRVANA_NORETURN [[noreturn]]
#else
#define NIRVANA_NORETURN
#endif
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

#ifdef NIRVANA_C20
#define NIRVANA_CONSTEXPR20 constexpr
#else
#define NIRVANA_CONSTEXPR20 inline
#endif

// In MSVC __declspec (selectany) lets linker to eliminate unreferenced static structures. TODO: Check for other compilers.
#ifdef _MSC_BUILD
#define NIRVANA_SELECTANY __declspec (selectany)
#else
#define NIRVANA_SELECTANY [[selectany]]
#endif

#endif
