/*
* Nirvana runtime library.
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

#include "../../pch/pch.h"
#include <assert.h>
#include <Nirvana/throw_exception.h>
#include <Nirvana/Module.h>

#ifdef _WIN32

extern "C" int _wcsicmp (wchar_t const* _String1, wchar_t const* _String2);

#include <Windows.h>

extern "C" BOOL WINAPI _DllMainCRTStartup (HINSTANCE inst, DWORD reason, LPVOID reserved);

namespace Nirvana {

void crt_init ()
{
	if (!_DllMainCRTStartup ((HINSTANCE)the_module->base_address (), DLL_PROCESS_ATTACH, 0))
		throw_UNKNOWN ();
}

void crt_term ()
{
	_DllMainCRTStartup (nullptr, DLL_PROCESS_DETACH, 0);
}

}

#elif !defined (__ELF__)

namespace Nirvana {

typedef void (__cdecl *_PVFV) (void);
typedef int  (__cdecl *_PIFV) (void);

extern _PIFV __xi_a [];
extern _PIFV __xi_z [];    // C initializers
extern _PVFV __xc_a [];
extern _PVFV __xc_z [];    // C++ initializers

// Call C constructors
static int _initterm_e(_PIFV * pfbegin, _PIFV * pfend)
{
	int ret = 0;

	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while ( pfbegin < pfend  && ret == 0) {
		// if current table entry is non-NULL, call thru it.
		if ( *pfbegin != 0 )
				ret = (**pfbegin)();
		++pfbegin;
	}

	return ret;
}
 
// Call C++ constructors
static void _initterm (_PVFV * pfbegin, _PVFV * pfend)
{
	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while ( pfbegin < pfend )
	{
		// if current table entry is non-NULL, call thru it.
		if ( *pfbegin != 0 )
				(**pfbegin)();
		++pfbegin;
	}
}

void crt_init ()
{
	if (_initterm_e (__xi_a, __xi_z) != 0)
		throw_UNKNOWN ();

	// Initialize C++
	_initterm (__xc_a, __xc_z);
}

void crt_term ()
{
}

}

#else

extern "C" void _init ();
extern "C" void _fini ();

namespace Nirvana {

void crt_init ()
{
	_init ();
}

void crt_term ()
{
	_fini ();
}

}

#endif

