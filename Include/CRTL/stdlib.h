/*
 * stdlib.h
 *
 * Definitions for common types, variables, and functions.
 */

#ifndef _STDLIB_H_
#define _STDLIB_H_

#include <Nirvana/NirvanaBase.h>
#include <stddef.h>

typedef size_t rsize_t;

#ifdef __cplusplus
extern "C" {
#endif

typedef struct 
{
	int quot; /* quotient */
	int rem; /* remainder */
} div_t;

typedef struct 
{
	long quot; /* quotient */
	long rem; /* remainder */
} ldiv_t;

typedef struct
{
	long long int quot; /* quotient */
	long long int rem; /* remainder */
} lldiv_t;

typedef int (__cdecl* __comp_fn_t)(void const*, void const*);
typedef int (__cdecl* __comp_s_fn_t)(void const*, void const*, void*);

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

#define RAND_MAX 0x7fff

NIRVANA_NORETURN void abort (void);
NIRVANA_NORETURN void exit (int status);
NIRVANA_NORETURN void _Exit (int status);
NIRVANA_NORETURN void quick_exit (int);

int abs (int);
int atexit (void (*func)(void));
double atof (const char *nptr);
int atoi (const char *nptr);
long atol (const char *nptr);
void* bsearch (const void *key, const void *base, size_t count, size_t size,
	__comp_fn_t comp);
void* bsearch_s (const void* key, const void* base, rsize_t count, rsize_t size,
	__comp_s_fn_t comp, void* context);
void *calloc(size_t count, size_t size);
div_t div (int numer, int denom);
void free (void *);
char * getenv (const char *name);
#if __POSIX_VISIBLE >= 200809
extern char *suboptarg;			/* getsubopt(3) external variable */
int	getsubopt (char **, char * const *, char **);
#endif
long labs (long);
ldiv_t ldiv (long numer, long denom);
void *malloc(size_t);
int mblen (const char *, size_t);
int mbtowc (wchar_t *__restrict, const char *__restrict, size_t);
int wctomb (char *, wchar_t);
size_t mbstowcs (wchar_t *__restrict, const char *__restrict, size_t);
size_t wcstombs (char *__restrict, const wchar_t *__restrict, size_t);
void qsort (void *base, size_t count, size_t size, __comp_fn_t comp);
void qsort_s (void* base, rsize_t count, rsize_t size, __comp_s_fn_t comp,
	void* context);
void srand (unsigned seed);
int	rand (void);
void *realloc(void *, size_t);
double strtod (const char *__restrict s, char **__restrict end);
long double strtold (const char* __restrict s, char** __restrict end);
float	strtof (const char *__restrict s, char **__restrict end);
long strtol (const char *__restrict s, char **__restrict end, int base);
unsigned long strtoul (const char *__restrict s, char **__restrict end, int base);

int	system (const char *command);

long long atoll (const char *nptr);
long long llabs (long long);
lldiv_t	lldiv (long long numer, long long denom);
long long strtoll (const char *__restrict s, char **__restrict end, int base);
unsigned long long strtoull (const char *__restrict s, char **__restrict end, int base);

void * aligned_alloc(size_t, size_t) noexcept;
int at_quick_exit(void (*)(void));

int mkstemp (char* tpl);

#ifdef __cplusplus

// Extension for compatibility with MSVC library
// MSVC code assumes following functions present at the global namespace

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

}
#endif

#endif /* _STDLIB_H_ */
