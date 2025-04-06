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
#ifndef _CTYPE_H_
#define _CTYPE_H_
#pragma once

typedef int locale_t;

#ifdef __cplusplus
extern "C" {
#endif

int isalnum(int);
int isalpha(int);

inline int isascii (int c)
{
	return ((unsigned)c & ~0x7F) == 0;
}

int isblank(int);
int iscntrl(int);
int isdigit(int);
#define isdigit_l(c, l) isdigit (c)
int isgraph (int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace (int);
int isupper(int);
int isxdigit(int);
#define isxdigit_l(c, l) isxdigit (c)
int tolower(int);
int tolower_l (int c, locale_t locale);
int toupper(int);
int toupper_l (int c, locale_t locale);

#ifdef __cplusplus
}
#endif

#endif
