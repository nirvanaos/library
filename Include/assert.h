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
#ifndef ASSERT_H_
#define ASSERT_H_
#pragma once

#undef verify
#undef assert

#ifdef NDEBUG

#define assert(exp) ((void)0)
#define verify(exp) (exp)

#ifdef _MSC_BUILD
#define NIRVANA_UNREACHABLE_CODE() __assume (0)
#elif defined (__GNUG__) || defined (__clang__)
#define NIRVANA_UNREACHABLE_CODE() __builtin_unreachable()
#endif

#define _NTRACE(fmt, ...)

#else

#ifdef __cplusplus
extern "C" {
#endif

	void Nirvana_assertion_failed (const char* msg, const char* file_name, int line_number);
	void Nirvana_trace (const char* file_name, int line_number, const char* format, ...);

#ifdef __cplusplus
}
#endif

#define assert(exp) (void)((!!(exp)) || (Nirvana_assertion_failed (#exp, __FILE__, __LINE__), 1))
#define verify(exp) assert(exp)
#define NIRVANA_UNREACHABLE_CODE() Nirvana_assertion_failed ("Executed unreachable code", __FILE__, __LINE__)

#define _NTRACE(fmt, ...) Nirvana_trace (__FILE__, __LINE__, fmt, __VA_ARGS__)

#endif

#endif
