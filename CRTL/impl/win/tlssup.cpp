/***
*tlssup.cpp - Thread Local Storage run-time support module
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
****/

#include <Nirvana/platform.h>
#include <Nirvana/CRTL/Windows/crtdefs.h>
#include <type_traits>

extern "C" {

struct IMAGE_TLS_DIRECTORY32 {
	unsigned long StartAddressOfRawData;
	unsigned long EndAddressOfRawData;
	unsigned long AddressOfIndex;             // PDWORD
	unsigned long AddressOfCallBacks;         // PIMAGE_TLS_CALLBACK *
	unsigned long SizeOfZeroFill;
	union {
		unsigned long Characteristics;
		struct {
			unsigned long Reserved0 : 20;
			unsigned long Alignment : 4;
			unsigned long Reserved1 : 8;
		} s;
	} u;
};

struct IMAGE_TLS_DIRECTORY64 {
	uint64_t StartAddressOfRawData;
	uint64_t EndAddressOfRawData;
	uint64_t AddressOfIndex;         // PDWORD
	uint64_t AddressOfCallBacks;     // PIMAGE_TLS_CALLBACK *;
	unsigned long SizeOfZeroFill;
	union {
		unsigned long Characteristics;
		struct {
			unsigned long Reserved0 : 20;
			unsigned long Alignment : 4;
			unsigned long Reserved1 : 8;
		} s;
	} u;
};

using IMAGE_TLS_DIRECTORY = std::conditional <sizeof (Nirvana::Word) == 8,
	IMAGE_TLS_DIRECTORY64, IMAGE_TLS_DIRECTORY32>::type;

/* Thread Local Storage index for this .EXE or .DLL */

unsigned long _tls_index = 0;

/* Special symbols to mark start and end of Thread Local Storage area. */

#pragma data_seg(".tls")

#if defined (_M_X64) || defined (__x86_64__)
_CRTALLOC(".tls")
#endif  /* defined (_M_X64) || defined (__x86_64__) */
char _tls_start = 0;

#pragma data_seg(".tls$ZZZ")

#if defined (_M_X64) || defined (__x86_64__)
_CRTALLOC(".tls$ZZZ")
#endif  /* defined (_M_X64) || defined (__x86_64__) */
char _tls_end = 0;

#pragma data_seg()

/* Start section for TLS callback array examined by the OS loader code.
 * If dynamic TLS initialization is used, then a pointer to __dyn_tls_init
 * will be placed in .CRT$XLC by inclusion of tlsdyn.obj.  This will cause
 * the .CRT$XD? array of individual TLS variable initialization callbacks
 * to be walked.
 */

_CRTALLOC(".CRT$XLA") PIMAGE_TLS_CALLBACK __xl_a = 0;

/* NULL terminator for TLS callback array.  This symbol, __xl_z, is never
 * actually referenced anywhere, but it must remain.  The OS loader code
 * walks the TLS callback array until it finds a NULL pointer, so this makes
 * sure the array is properly terminated.
 */

_CRTALLOC(".CRT$XLZ") PIMAGE_TLS_CALLBACK __xl_z = 0;

_CRTALLOC(".rdata$T")
extern const IMAGE_TLS_DIRECTORY _tls_used =
{
	(uintptr_t)&_tls_start, // start of tls data
	(uintptr_t)&_tls_end,   // end of tls data
	(uintptr_t)&_tls_index, // address of tls_index
	(uintptr_t)(&__xl_a+1), // pointer to call back array
	0,                      // size of tls zero fill
	0                       // characteristics
};

} // extern "C"
