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
#include <stdint.h>
#include <limits.h>

#ifdef _LIBCPP_MSVCRT_LIKE
#include <malloc.h>
#endif

#if defined (_MSC_VER) && !defined (__clang__)

#define _STDLIB_NORETURN __declspec (noreturn)

#elif defined (__GNUG__) || defined (__clang__)

#define _STDLIB_NORETURN __attribute__((__noreturn__))

#elif (defined(__STDC_VERSION__))

#if (__STDC_VERSION__ > 201710L)
#define _STDLIB_NORETURN [[noreturn]]
#elif (__STDC_VERSION__ >= 201112)
#define _STDLIB_NORETURN _Noreturn
#else
#define _STDLIB_NORETURN
#endif

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

#define RAND_MAX UINT_MAX

typedef struct __Locale* locale_t;

_STDLIB_NORETURN void _Exit (int);
_STDLIB_NORETURN void abort (void);
int abs (int);
void* aligned_alloc (size_t, size_t);
int at_quick_exit (void (*)(void));
int atexit (void (*)(void));

#ifndef _MSC_VER
double atof (const char*);
#endif

long atol (const char*);
long long atoll (const char*);

#if INT_MAX < LONG_MAX
int atoi (const char*);
#else
#define atoi atol
#endif

void* bsearch (const void*, const void*, size_t, size_t, int (*)(const void *, const void *));
void* calloc (size_t, size_t);
_STDLIB_NORETURN void exit (int);
void free (void*);
char* getenv (const char*);

long labs (long j);
ldiv_t ldiv (long numer, long denom);
div_t div (int, int);

long long llabs (long long j);
lldiv_t lldiv (long long numer, long long denom);
void* malloc (size_t size);
int mblen (const char*, size_t);
size_t mbstowcs (wchar_t* restrict, const char* restrict, size_t);
int mbtowc (wchar_t* restrict, const char* restrict, size_t);
int mbtowc_l (wchar_t* restrict, const char* restrict, size_t, locale_t);
int mkstemp (char * tpl);
int mkstemps (char* tpl, int suffixlen);
int mkostemp (char* tpl, int flags);
int mkostemps (char* tpl, int suffixlen, int flags);
int posix_memalign (void **, size_t, size_t);
void qsort (void* base, size_t nmemb, size_t size,
           int (*compar)(const void *, const void *));
_STDLIB_NORETURN void quick_exit (int);
int rand (void);
int rand_r (unsigned* seed);
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

#define MB_CUR_MAX 4

#ifdef __cplusplus
#undef restrict
}
#endif

#ifdef _LIBCPP_MSVCRT_LIKE

#define _mbtowc_l mbtowc_l
#define _strtod_l strtod_l
#define _strtof_l strtof_l
#define _strtoi64_l strtoll_l
#define _strtold_l strtold_l
#define _strtoui64_l strtoull_l

#endif

#if defined (_LIBCPP_MSVCRT) && defined (__cplusplus)
extern "C++"
{
  inline long abs (long const _X) noexcept
  {
    return labs (_X);
  }

  inline long long abs (long long const _X) noexcept
  {
    return llabs (_X);
  }

  inline ldiv_t div (long const _A1, long const _A2) noexcept
  {
    return ldiv (_A1, _A2);
  }

  inline lldiv_t div (long long const _A1, long long const _A2) noexcept
  {
    return lldiv (_A1, _A2);
  }

}
#endif

// where appropriate), needed for integer-to-string conversions for several
// bases and integer types.
#define _MAX_ITOSTR_BASE16_COUNT   (8  + 1)
#define _MAX_ITOSTR_BASE10_COUNT   (1 + 10 + 1)
#define _MAX_ITOSTR_BASE8_COUNT    (11 + 1)
#define _MAX_ITOSTR_BASE2_COUNT    (32 + 1)

#define _MAX_LTOSTR_BASE16_COUNT   (8  + 1)
#define _MAX_LTOSTR_BASE10_COUNT   (1 + 10 + 1)
#define _MAX_LTOSTR_BASE8_COUNT    (11 + 1)
#define _MAX_LTOSTR_BASE2_COUNT    (32 + 1)

#define _MAX_ULTOSTR_BASE16_COUNT  (8  + 1)
#define _MAX_ULTOSTR_BASE10_COUNT  (10 + 1)
#define _MAX_ULTOSTR_BASE8_COUNT   (11 + 1)
#define _MAX_ULTOSTR_BASE2_COUNT   (32 + 1)

#define _MAX_I64TOSTR_BASE16_COUNT (16 + 1)
#define _MAX_I64TOSTR_BASE10_COUNT (1 + 19 + 1)
#define _MAX_I64TOSTR_BASE8_COUNT  (22 + 1)
#define _MAX_I64TOSTR_BASE2_COUNT  (64 + 1)

#define _MAX_U64TOSTR_BASE16_COUNT (16 + 1)
#define _MAX_U64TOSTR_BASE10_COUNT (20 + 1)
#define _MAX_U64TOSTR_BASE8_COUNT  (22 + 1)
#define _MAX_U64TOSTR_BASE2_COUNT  (64 + 1)

#undef _STDLIB_NORETURN

#endif
