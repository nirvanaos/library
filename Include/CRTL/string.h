/*
 * string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <stddef.h>

#pragma push_macro("_CONST_RETURN")

#ifdef __cplusplus
#define _CONST_RETURN const
extern "C" {
#else
#define _CONST_RETURN
#endif

_CONST_RETURN void* memchr (const void*, int, size_t);

int memcmp (const void*, const void*, size_t);

void* memcpy (void* __restrict, const void* __restrict, size_t);
void* memmove (void* dst, const void* src, size_t cb);

#if defined(_MSC_BUILD) && !(defined (__GNUG__) || defined (__clang__))
#pragma function(memcpy)
#pragma function(memmove)
#endif

void* memset (void*, int, size_t);
char* strcat (char* __restrict, const char* __restrict);
_CONST_RETURN char* strchr (const char*, int);
int strcmp (const char*, const char*);
int strcoll (const char*, const char*);
char* strcpy (char* __restrict, const char* __restrict);
size_t strcspn (const char*, const char*);
char* strerror (int);

size_t strlen (const char*);
size_t strnlen (const char*, size_t);

inline size_t strnlen_s (const char* s, size_t strsz)
{
	return s ? strnlen (s, strsz) : 0;
}

char* strncat (char* __restrict, const char* __restrict, size_t);
int strncmp (const char*, const char*, size_t);
char* strncpy (char* __restrict, const char* __restrict, size_t);
char* strpbrk (const char*, const char*);
_CONST_RETURN char* strrchr (const char*, int);
size_t strspn (const char*, const char*);
_CONST_RETURN char* strstr (const char*, const char*);
char* strtok (char* strToken, const char* strDelimit);
size_t strxfrm (char* __restrict, const char* __restrict, size_t);

#ifdef __cplusplus
}
extern "C++" inline void* memchr (void* p, int c, size_t cb) {
	return const_cast <void*> (memchr ((const void*)p, c, cb));
}
extern "C++" inline char* strchr (char* p, int c) {
	return const_cast <char*> (strchr ((const char*)p, c));
}
extern "C++" inline char* strrchr (char* p, int c) {
	return const_cast <char*> (strrchr ((const char*)p, c));
}
extern "C++" inline char* strstr (char* p, const char* s) {
	return const_cast <char*> (strstr ((const char*)p, s));
}
#endif

#pragma pop_macro("_CONST_RETURN")

#endif
