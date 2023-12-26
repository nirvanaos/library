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
#ifndef _STDLIB_H_
#define _STDLIB_H_
#pragma once

#include <stddef.h>

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ > 201710L)
#define _STDLIB_NORETURN [[_STDLIB_NORETURN]]
#else
#define _STDLIB_NORETURN _Noreturn
#endif

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL 0LL
#else
#define NULL ((void *)0)
#endif
#endif

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#ifndef _DIV_T_DEFINED
#define _DIV_T_DEFINED

typedef struct _div_t {
  int quot;
  int rem;
} div_t;

typedef struct _ldiv_t {
  long quot;
  long rem;
} ldiv_t;

typedef struct {
  long long quot, rem;
} lldiv_t;

#endif

#define RAND_MAX 0x7fff

typedef void* locale_t;

_STDLIB_NORETURN void _Exit (int);
_STDLIB_NORETURN void abort (void);
int abs (int);
int at_quick_exit (void (*)(void));
int atexit (void (*)(void));
double atof (const char*);
int atoi (const char*);
long atol (const char*);
long long atoll (const char*);
void* bsearch (const void*, const void*, size_t, size_t, int (*)(const void *, const void *));
void* calloc (size_t, size_t);
div_t div (int, int);
_STDLIB_NORETURN void exit (int);
void free (void*);
char* getenv (const char*);
long labs (long j);
ldiv_t ldiv (long numer, long denom);
long long llabs (long long j);
lldiv_t lldiv (long long numer, long long denom);
void* malloc (size_t size);
int mblen (const char*, size_t);
size_t mbstowcs (wchar_t* restrict, const char* restrict, size_t);
int mbtowc (wchar_t* restrict, const char* restrict, size_t);
int mkstemp (char * tpl);
int mkstemps (char* tpl, int suffixlen);
int mkostemp (char* tpl, int flags);
int mkostemps (char* tpl, int suffixlen, int flags);
int posix_memalign (void **, size_t, size_t);
void qsort (void* base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
_STDLIB_NORETURN void quick_exit (int);
int rand (void);
void* realloc (void*, size_t);
void srand (unsigned);
double strtod (const char* restrict, char** restrict);
double strtod_l (const char* restrict, char** restrict, locale_t);
float strtof (const char* restrict, char** restrict);
float strtof_l (const char* restrict, char** restrict, locale_t);
long strtol  (const char* restrict, char** restrict, int);
#define strtol_l(n, ep, radix, loc) strtol (n, ep, radix)
long double strtold (const char* restrict, char** restrict);
long double strtold_l (const char* restrict, char** restrict, locale_t);
long long strtoll (const char* restrict, char** restrict, int);
#define strtoll_l(n, ep, radix, loc) strtoll (n, ep, radix)
unsigned long strtoul (const char* restrict, char** restrict, int);
#define strtoul_l(n, ep, radix, loc) strtoul (n, ep, radix)
unsigned long long strtoull (const char* restrict, char** restrict, int);
#define strtoull_l(n, ep, radix, loc) strtoull (n, ep, radix)
int system (const char*);
size_t wcstombs (char* restrict, const wchar_t* restrict, size_t);
int wctomb (char*, wchar_t);

int __mb_cur_max_func (void);

#define MB_CUR_MAX (__mb_cur_max_func ())

#ifdef __cplusplus
#undef restrict
}
#endif

#undef _STDLIB_NORETURN

#endif
