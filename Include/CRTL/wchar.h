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
#ifndef _WCHAR_H_
#define _WCHAR_H_
#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#define restrict
#define rsize_t size_t
#endif

struct _Mbstatet;
typedef _Mbstatet mbstate_t;

#ifndef _MSC_VER

typedef struct _Mbstatet {
  uint32_t buffer;
  uint32_t state;
} _Mbstatet;

#endif

typedef unsigned short wint_t;

#define WEOF 0xffffu

typedef unsigned short wctype_t;

struct tm;

wint_t        btowc (int);
wint_t        fgetwc (FILE *);
wchar_t      *fgetws (wchar_t *restrict, int, FILE *restrict);
wint_t        fputwc (wchar_t, FILE *);
int           fputws (const wchar_t *restrict, FILE *restrict);
int           fwide (FILE *, int);
int           fwprintf (FILE *restrict, const wchar_t *restrict, ...);
int           fwscanf (FILE *restrict, const wchar_t *restrict, ...);
wint_t        getwc (FILE *);
wint_t        getwchar (void);
size_t        mbrlen (const char *restrict, size_t, mbstate_t *restrict);
int           mbtowc (wchar_t* restrict, const char* restrict, size_t);
size_t        mbrtowc (wchar_t *restrict, const char *restrict, size_t, mbstate_t *restrict);
int           mbsinit (const mbstate_t *);
size_t        mbsnrtowcs (wchar_t *restrict, const char **restrict, size_t, size_t,
  mbstate_t *restrict);
size_t        mbsrtowcs (wchar_t *restrict, const char **restrict, size_t,
  mbstate_t *restrict);
size_t        mbsrtowcs (wchar_t *restrict, const char **restrict, size_t,
  mbstate_t *restrict);
wint_t        putwc (wchar_t, FILE *);
wint_t        putwchar (wchar_t);
int           swprintf (wchar_t *restrict, size_t, const wchar_t *restrict, ...);
int           swprintf_s (wchar_t* restrict, rsize_t, const wchar_t* restrict, ...);
int           swscanf (const wchar_t *restrict, const wchar_t *restrict, ...);
wint_t        ungetwc (wint_t, FILE *);
int           vfwprintf (FILE *restrict, const wchar_t *restrict, va_list);
int           vfwscanf (FILE *restrict, const wchar_t *restrict, va_list);
int           vswprintf (wchar_t *restrict, size_t, const wchar_t *restrict, va_list);
int           vswscanf (const wchar_t *restrict, const wchar_t *restrict, va_list);
int           vwprintf (const wchar_t *restrict, va_list);
int           vwscanf (const wchar_t *restrict, va_list);
size_t        wcrtomb (char *restrict, wchar_t, mbstate_t *restrict);
wchar_t      *wcscat (wchar_t *restrict, const wchar_t *restrict);
wchar_t      *wcschr (const wchar_t *, wchar_t);
int           wcscmp (const wchar_t *, const wchar_t *);
int           wcscoll (const wchar_t *, const wchar_t *);
wchar_t      *wcscpy (wchar_t *restrict, const wchar_t *restrict);
size_t        wcscspn (const wchar_t *, const wchar_t *);
size_t        wcsftime (wchar_t* restrict, size_t, const wchar_t* restrict,
  const struct tm* restrict);
size_t        wcslen (const wchar_t*);
wchar_t      *wcsncat (wchar_t *restrict, const wchar_t *restrict, size_t);
int           wcsncmp (const wchar_t *, const wchar_t *, size_t);
wchar_t      *wcsncpy (wchar_t *restrict, const wchar_t *restrict, size_t);
size_t        wcsnlen (const wchar_t *, size_t);

inline size_t wcsnlen_s (const wchar_t *str, size_t maxlen)
{
  return str ? wcsnlen (str, maxlen) : 0;
}

size_t        wcsnrtombs (char *restrict, const wchar_t **restrict, size_t, size_t,
                          mbstate_t *restrict);
wchar_t      *wcspbrk (const wchar_t *, const wchar_t *);
wchar_t      *wcsrchr (const wchar_t *, wchar_t);
size_t        wcsrtombs (char *restrict, const wchar_t **restrict, size_t, mbstate_t *restrict);
size_t        wcsspn (const wchar_t *, const wchar_t *);
wchar_t      *wcsstr (const wchar_t *restrict, const wchar_t *restrict);
double        wcstod (const wchar_t *restrict, wchar_t **restrict);
float         wcstof (const wchar_t *restrict, wchar_t **restrict);
wchar_t      *wcstok (wchar_t *restrict, const wchar_t *restrict, wchar_t **restrict);
long          wcstol (const wchar_t *restrict, wchar_t **restrict, int);
long double   wcstold (const wchar_t *restrict, wchar_t **restrict);
long long     wcstoll (const wchar_t *restrict, wchar_t **restrict, int);
unsigned long wcstoul (const wchar_t *restrict, wchar_t **restrict, int);
unsigned long long wcstoull (const wchar_t *restrict, wchar_t **restrict, int);
size_t        wcsxfrm (wchar_t *restrict, const wchar_t *restrict, size_t);
int           wctob (wint_t);
wchar_t      *wmemchr (const wchar_t *, wchar_t, size_t);
int           wmemcmp (const wchar_t *, const wchar_t *, size_t);
wchar_t      *wmemcpy (wchar_t *restrict, const wchar_t *restrict, size_t);
wchar_t      *wmemmove (wchar_t *, const wchar_t *, size_t);
wchar_t      *wmemset (wchar_t *, wchar_t, size_t);
int           wprintf (const wchar_t *restrict, ...);
int           wscanf (const wchar_t *restrict, ...);

#ifdef _MSC_VER
#define _scwprintf(format, ...) swprintf (nullptr, 0, format, __VA_ARGS__)
#endif

#ifdef __cplusplus
}
#undef restrict
#undef rsize_t
#endif

#endif
