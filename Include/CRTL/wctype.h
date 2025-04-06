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

#ifdef __cplusplus
extern "C" {
#endif

int    iswalnum (wint_t);
int    iswalnum_l (wint_t, locale_t);
int    iswalpha (wint_t);
int    iswalpha_l (wint_t, locale_t);
int    iswblank (wint_t);
int    iswblank_l (wint_t, locale_t);
int    iswcntrl (wint_t);
int    iswcntrl_l (wint_t, locale_t);
int    iswctype (wint_t, wctype_t);
int    iswctype_l (wint_t, wctype_t, locale_t);
int    iswdigit (wint_t);
int    iswdigit_l (wint_t, locale_t);
int    iswgraph (wint_t);
int    iswgraph_l (wint_t, locale_t);
int    iswlower (wint_t);
int    iswlower_l (wint_t, locale_t);
int    iswprint (wint_t);
int    iswprint_l (wint_t, locale_t);
int    iswpunct (wint_t);
int    iswpunct_l (wint_t, locale_t);
int    iswspace (wint_t);
int    iswspace_l (wint_t, locale_t);
int    iswupper (wint_t);
int    iswupper_l (wint_t, locale_t);
int    iswxdigit (wint_t);
int    iswxdigit_l (wint_t, locale_t);
wint_t towlower (wint_t);
wint_t towlower_l (wint_t, locale_t);
wint_t towupper (wint_t);
wint_t towupper_l (wint_t, locale_t);

#ifdef __cplusplus
}
#endif

#endif
