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

#ifdef _MSC_VER
#define _LCONV_STR(name) union { wchar_t* _W_##name; char* name; }
#else
#define _LCONV_STR(name) char* name
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct lconv
{
	_LCONV_STR (decimal_point);
	_LCONV_STR (thousands_sep);
	char    *grouping;

	_LCONV_STR (mon_decimal_point);
	_LCONV_STR (mon_thousands_sep);
	char    *mon_grouping;

	_LCONV_STR (positive_sign);
	_LCONV_STR (negative_sign);

	_LCONV_STR (int_curr_symbol);

	_LCONV_STR (currency_symbol);

	char     int_frac_digits;
	char     frac_digits;
	char     p_cs_precedes;
	char     p_sep_by_space;
	char     n_cs_precedes;
	char     n_sep_by_space;
	char     p_sign_posn;
	char     n_sign_posn;
	char     int_n_cs_precedes;
	char     int_n_sep_by_space;
	char     int_n_sign_posn;
	char     int_p_cs_precedes;
	char     int_p_sep_by_space;
	char     int_p_sign_posn;
};

#define LC_ALL 0
#define LC_COLLATE 1
#define LC_CTYPE 2
#define LC_MONETARY 3
#define LC_NUMERIC 4
#define LC_TIME 5
#define LC_MESSAGES 6

#define LC_COLLATE_MASK  (1 << LC_COLLATE)
#define LC_CTYPE_MASK    (1 << LC_CTYPE)
#define LC_MESSAGES_MASK (1 << LC_MESSAGES)
#define LC_MONETARY_MASK (1 << LC_MONETARY)
#define LC_NUMERIC_MASK  (1 << LC_NUMERIC)
#define LC_TIME_MASK     (1 << LC_TIME)
#define LC_ALL_MASK (LC_COLLATE_MASK|\
                     LC_CTYPE_MASK|\
                     LC_MONETARY_MASK|\
                     LC_NUMERIC_MASK|\
                     LC_TIME_MASK|\
                     LC_MESSAGES_MASK)

#define LC_MIN LC_ALL
#define LC_MAX LC_MESSAGES

typedef void* locale_t;

locale_t duplocale (locale_t);
locale_t freelocale (locale_t);
struct lconv *localeconv (void);
locale_t newlocale(int, const char *, locale_t);
char *setlocale (int, const char *);
locale_t uselocale (locale_t);

#ifdef __cplusplus
}
#endif

#undef _LCONV_STR

#endif
