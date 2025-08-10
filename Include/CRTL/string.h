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
#ifndef _STRING_H_
#define _STRING_H_
#pragma once

#include <stddef.h>

//#ifndef __STDC_LIB_EXT1__
//#define __STDC_LIB_EXT1__ 1
//#endif

#if defined(_MSC_VER) && !(defined (__GNUG__) || defined (__clang__))
#pragma warning (push)
#pragma warning (disable: 4164)
#pragma function(memcpy)
#pragma function(memmove)
#pragma warning (pop)
#endif

typedef int errno_t;
typedef struct __Locale* locale_t;

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

void*    memchr (const void*, int, size_t);
int      memcmp (const void*, const void *, size_t);
void*    memcpy (void* restrict, const void* restrict, size_t);
void*    memmove (void*, const void*, size_t);
void*    memset (void*, int, size_t);
char*    strcat (char* restrict, const char* restrict);
char*    strchr (const char*, int);
int      strcmp (const char*, const char*);
int      strcoll (const char*, const char*);
int      strcoll_l(const char*, const char*, locale_t);
char*    strcpy (char* restrict, const char* restrict);

#if defined(__STDC_WANT_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ >= 1
errno_t  strcpy_s (char* restrict, rsize_t, const char* restrict);
#endif

size_t   strcspn (const char*, const char*);

char*    strdup (const char*);
char*    strndup (const char*, size_t);

char*    strerror (int);
char*    strerror_l (int, locale_t);
int      strerror_r (int, char*, size_t);
size_t   strlen (const char *);
char*    strncat (char *restrict, const char *restrict, size_t);
int      strncmp (const char *, const char *, size_t);
char*    strncpy (char *restrict, const char *restrict, size_t);
size_t   strnlen (const char *, size_t);

inline size_t strnlen_s (const char *str, size_t maxlen)
{
  return str ? strnlen (str, maxlen) : 0;
}

char*    strpbrk (const char *, const char *);
char*    strrchr (const char *, int);
size_t   strspn (const char *, const char *);
char*    strstr (const char *, const char *);
char*    strtok (char *restrict, const char *restrict);
size_t   strxfrm (char *restrict, const char *restrict, size_t);
size_t   strxfrm_l (char* restrict s1, const char* restrict s2, size_t n, locale_t locale);

#ifdef __cplusplus
#undef restrict
#undef CPP_CONST_RETURN
}

template <size_t size> inline
errno_t strcpy_s (char (&dest) [size], const char* src)
{
  return strcpy_s (dest, size, src);
}

#endif

#ifdef _LIBCPP_MSVCRT_LIKE

#define _strcoll_l strcoll_l
#define _strdup strdup
#define _strxfrm_l strxfrm_l

#endif

#endif
