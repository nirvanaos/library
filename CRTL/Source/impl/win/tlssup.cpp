/***
*tlssup.cpp - Thread Local Storage run-time support module
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*
****/

#include <Nirvana/platform.h>
#include <Nirvana/OLF.h>
#include "crtdefs.h"

extern "C" {

struct IMAGE_TLS_DIRECTORY {
	const void* StartAddressOfRawData;
	const void* EndAddressOfRawData;
	uint32_t* AddressOfIndex;
	const PIMAGE_TLS_CALLBACK* AddressOfCallBacks;
	uint32_t SizeOfZeroFill;
	union {
		uint32_t Characteristics;
		struct {
			uint32_t Reserved0 : 20;
			uint32_t Alignment : 4;
			uint32_t Reserved1 : 8;
		} s;
	} u;
};

/* Thread Local Storage index for this .EXE or .DLL */

uint32_t _tls_index = 0;

/* Special symbols to mark start and end of Thread Local Storage area. */

#pragma data_seg (".tls")

//#if NIRVANA_PLATFORM (X64)
_CRTALLOC (".tls")
//#endif
char _tls_start = 0;

#pragma data_seg (".tls$ZZZ")

//#if NIRVANA_PLATFORM (X64)
_CRTALLOC (".tls$ZZZ")
//#endif
char _tls_end = 0;

#pragma data_seg ()

/* Start section for TLS callback array examined by the OS loader code.
 * If dynamic TLS initialization is used, then a pointer to __dyn_tls_init
 * will be placed in .CRT$XLC by inclusion of tlsdyn.obj.  This will cause
 * the .CRT$XD? array of individual TLS variable initialization callbacks
 * to be walked.
 */

_CRTALLOC (".CRT$XLA") const PIMAGE_TLS_CALLBACK __xl_a = 0;

/* NULL terminator for TLS callback array.  This symbol, __xl_z, is never
 * actually referenced anywhere, but it must remain.  The OS loader code
 * walks the TLS callback array until it finds a NULL pointer, so this makes
 * sure the array is properly terminated.
 */

#ifdef _MSC_VER
extern "C" _CRTALLOC (".CRT$XLZ") const PIMAGE_TLS_CALLBACK __xl_z = 0;
NIRVANA_LINK_SYMBOL (__xl_z)
#else
_CRTALLOC (".CRT$XLZ") const PIMAGE_TLS_CALLBACK __attribute__ ((used)) __xl_z = 0;
#endif

_CRTALLOC (".rdata$T")
extern const NIRVANA_CONSTINIT IMAGE_TLS_DIRECTORY _tls_used =
{
	&_tls_start,   // start of tls data
	&_tls_end,     // end of tls data
	&_tls_index,   // address of tls_index
	(&__xl_a + 1), // pointer to call back array
	0,             // size of tls zero fill
	0              // characteristics
};

} // extern "C"
