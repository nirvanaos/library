#ifndef _WCHAR_H_
#define	_WCHAR_H_

#include <stdio.h>

#pragma push_macro("_CONST_RETURN")

#ifdef __cplusplus
#define _CONST_RETURN const
extern "C" {
#else
#define _CONST_RETURN
#endif

/* As required by POSIX.1-2008, declare tm as incomplete type.
		The actual definition is in time.h. */
struct tm;

wint_t btowc (int);
int	wctob (wint_t);

wchar_t* wcscat (wchar_t* __restrict, const wchar_t* __restrict);
_CONST_RETURN wchar_t* wcschr (const wchar_t*, wchar_t);
int wcscmp (const wchar_t*, const wchar_t*);
int wcscoll (const wchar_t*, const wchar_t*);
wchar_t* wcscpy (wchar_t* __restrict, const wchar_t* __restrict);
wchar_t* wcsdup (const wchar_t*);
size_t wcscspn (const wchar_t*, const wchar_t*);
size_t wcsftime (wchar_t* __restrict, size_t,
	const wchar_t* __restrict, const struct tm* __restrict);
size_t wcslen (const wchar_t*);
wchar_t* wcsncat (wchar_t* __restrict,
	const wchar_t* __restrict, size_t);
int	wcsncmp (const wchar_t*, const wchar_t*, size_t);
wchar_t* wcsncpy (wchar_t* __restrict,
	const wchar_t* __restrict, size_t);
wchar_t* wcspbrk (const wchar_t*, const wchar_t*);
_CONST_RETURN wchar_t* wcsrchr (const wchar_t*, wchar_t);
size_t wcsspn (const wchar_t*, const wchar_t*);
_CONST_RETURN wchar_t* wcsstr (const wchar_t* __restrict,
	const wchar_t* __restrict);
wchar_t* wcstok (wchar_t* __restrict, const wchar_t* __restrict,
	wchar_t** __restrict);
float wcstof (const wchar_t* __restrict str, wchar_t** __restrict);
double wcstod (const wchar_t* __restrict, wchar_t** __restrict);
long double wcstold (const wchar_t* __restrict, wchar_t** __restrict);
size_t wcsxfrm (wchar_t* __restrict, const wchar_t* __restrict,
	size_t);
wchar_t* wmemchr (const wchar_t*, wchar_t, size_t);
int wmemcmp (const wchar_t*, const wchar_t*, size_t);
wchar_t* wmemcpy (wchar_t* __restrict, const wchar_t* __restrict,
	size_t);
wchar_t* wmemmove (wchar_t*, const wchar_t*, size_t);
wchar_t* wmemset (wchar_t*, wchar_t, size_t);
long wcstol (const wchar_t* __restrict, wchar_t** __restrict, int);
long long wcstoll (const wchar_t* __restrict, wchar_t** __restrict, int);
unsigned long wcstoul (const wchar_t* __restrict, wchar_t** __restrict, int);
unsigned long long wcstoull (const wchar_t* __restrict,
	wchar_t** __restrict, int);
int fwide (FILE*, int);
size_t mbrlen (const char* pmb, size_t max, mbstate_t* ps);
size_t mbrtowc (wchar_t* pwc, const char* s, size_t n, mbstate_t* ps);
size_t mbsrtowcs (wchar_t* dst, const char** src, size_t len, mbstate_t* ps);
int mbsinit (const mbstate_t* ps);
size_t wcrtomb (char* pmb, wchar_t wc, mbstate_t* ps);
size_t wcsrtombs (char* dst, const wchar_t** src, size_t len, mbstate_t* ps);

#ifdef __cplusplus
}
extern "C++" inline wchar_t* wcschr (wchar_t* p, wchar_t c) {
	return const_cast <wchar_t*> (wcschr ((const wchar_t*)p, c));
}
extern "C++" inline wchar_t* wcsrchr (wchar_t* p, wchar_t c) {
	return const_cast <wchar_t*> (wcsrchr ((const wchar_t*)p, c));
}
extern "C++" inline wchar_t* wcsstr (wchar_t* p, const wchar_t* s) {
	return const_cast <wchar_t*> (wcsstr ((const wchar_t*)p, s));
}
#endif

#pragma pop_macro("_CONST_RETURN")

#endif
