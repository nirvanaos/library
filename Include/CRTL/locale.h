/*
* Nirvana C runtime library.
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
#ifndef _LOCALE_H_
#define _LOCALE_H_
#pragma once

#include <stddef.h>
#include <Nirvana/locale_defs.h>

typedef void* locale_t;
#define LC_GLOBAL_LOCALE ((void*)-1)

#ifdef __cplusplus
extern "C" {
#endif

locale_t duplocale (locale_t);
void freelocale (locale_t);
struct lconv* localeconv (void);
locale_t newlocale (int, const char*, locale_t);
char* setlocale (int, const char*);
locale_t uselocale (locale_t);

#ifdef _LIBCPP_MSVCRT_LIKE
inline int _configthreadlocale(int l) { return l; }
#endif

#ifdef __cplusplus
}
#endif

#ifdef _LIBCPP_MSVCRT_LIKE

#define _locale_t locale_t

#define _create_locale(category, locale) newlocale (category, locale, (locale_t)0)
#define _free_locale freelocale

#define _ENABLE_PER_THREAD_LOCALE 1

#endif

#define _LIBCPP_PROVIDES_DEFAULT_RUNE_TABLE 1

#endif
