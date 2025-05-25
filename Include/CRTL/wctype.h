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
#ifndef _WCTYPE_H_
#define _WCTYPE_H_
#pragma once

#include <wchar.h>

#if !defined (_MSC_VER) || defined (__clang__)

typedef int wctype_t;

#endif

typedef int wctrans_t;

#ifdef __cplusplus
extern "C" {
#endif

int    iswalnum (wint_t);
int    iswalpha (wint_t);
int    iswblank (wint_t);
int    iswcntrl (wint_t);
int    iswctype (wint_t, wctype_t);
int    iswdigit (wint_t);
int    iswgraph (wint_t);
int    iswlower (wint_t);
int    iswprint (wint_t);
int    iswpunct (wint_t);
int    iswspace (wint_t);
int    iswupper (wint_t);
int    iswxdigit (wint_t);
wint_t towctrans (wint_t c, wctrans_t w);
wint_t towlower (wint_t);
wint_t towupper (wint_t);
wctrans_t wctrans (const char*);
wctype_t wctype(const char*);

#ifdef __cplusplus
}
#endif

/// Wide characters in Nirvana are always UCS-2 encoded.
/// So we don't need the locale-specific functions.
#define iswalnum_l(wc, loc) iswalnum (wc)
#define iswalpha_l(wc, loc) iswalpha (wc)
#define iswblank_l(wc, loc) iswblank (wc)
#define iswcntrl_l(wc, loc) iswcntrl (wc)
#define iswctype_l(wc, t, loc) iswctype (wc, t)
#define iswdigit_l(wc, loc) iswdigit (wc)
#define iswgraph_l(wc, loc) iswgraph (wc)
#define iswlower_l(wc, loc) iswlower (wc)
#define iswprint_l(wc, loc) iswprint (wc)
#define iswpunct_l(wc, loc) iswpunct (wc)
#define iswspace_l(wc, loc) iswspace (wc)
#define iswupper_l(wc, loc) iswupper (wc)
#define iswxdigit_l(wc, loc) iswxdigit (wc)
#define towctrans_l(wc, w, loc) towctrans (wc, w)
#define towlower_l(wc, loc) towlower (wc)
#define towupper_l(wc, loc) towupper (wc)
#define wctrans_l(c, loc) wctrans (c)
#define wctype_l(c, loc) wctype (c)

#ifdef _LIBCPP_MSVCRT_LIKE

#define _iswalpha_l iswalpha_l
#define _iswcntrl_l iswcntrl_l
#define _iswctype_l iswctype_l
#define _iswdigit_l iswdigit_l
#define _iswlower_l iswlower_l
#define _iswprint_l iswprint_l
#define _iswpunct_l iswpunct_l
#define _iswspace_l iswspace_l
#define _iswupper_l iswupper_l
#define _iswxdigit_l iswxdigit_l
#define _towlower_l towlower_l
#define _towupper_l towupper_l

#endif

#endif
