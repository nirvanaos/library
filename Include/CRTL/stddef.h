#ifndef _STDDEF_H_
#define _STDDEF_H_

#include <Nirvana/NirvanaBase.h>

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

#ifdef __cplusplus
typedef decltype(nullptr) nullptr_t;
#ifdef NIRVANA_C14
#define offsetof(st, m) __builtin_offsetof(st, m)
#else
#define offsetof(st, m) (size_t)&reinterpret_cast<const volatile char&>((((st*)0)->m))
#endif
#else
#define offsetof(st, m) ((size_t)&(((st*)0)->m))
#endif

#endif
