/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#include <Nirvana/CRTL/initterm.h>
#include <Nirvana/CRTL/Windows/crtdefs.h>
#include "../Global.h"

extern "C" _CRTALLOC (".CRT$XIA") const _PIFV __xi_a [] = { nullptr }; // C initializers (first)
extern "C" _CRTALLOC (".CRT$XIZ") const _PIFV __xi_z [] = { nullptr }; // C initializers (last)
extern "C" _CRTALLOC (".CRT$XCA") const _PVFV __xc_a [] = { nullptr }; // C++ initializers (first)
extern "C" _CRTALLOC (".CRT$XCZ") const _PVFV __xc_z [] = { nullptr }; // C++ initializers (last)
extern "C" _CRTALLOC (".CRT$XPA") const _PVFV __xp_a [] = { nullptr }; // C pre-terminators (first)
extern "C" _CRTALLOC (".CRT$XPZ") const _PVFV __xp_z [] = { nullptr }; // C pre-terminators (last)
extern "C" _CRTALLOC (".CRT$XTA") const _PVFV __xt_a [] = { nullptr }; // C terminators (first)
extern "C" _CRTALLOC (".CRT$XTZ") const _PVFV __xt_z [] = { nullptr }; // C terminators (last)

#pragma comment(linker, "/merge:.CRT=.rdata")

extern "C" const _PIFV __xi_a []; // First C Initializer
extern "C" const _PIFV __xi_z []; // Last C Initializer
extern "C" const _PVFV __xc_a []; // First C++ Initializer
extern "C" const _PVFV __xc_z []; // Last C++ Initializer
extern "C" const _PVFV __xp_a []; // First Pre-Terminator
extern "C" const _PVFV __xp_z []; // Last Pre-Terminator
extern "C" const _PVFV __xt_a []; // First Terminator
extern "C" const _PVFV __xt_z []; // Last Terminator

extern "C" void __do_init (void);
extern "C" void __do_fini (void);

#pragma section (".CRT$XCU", long, read) // C++ constructors
#pragma section (".CRT$XPC", long, read) // C erminator

_CRTALLOC (".CRT$XCU") static const _PVFV __attribute__ ((used)) _init = __do_init;
_CRTALLOC (".CRT$XPC") static const _PVFV __attribute__ ((used)) _fini = __do_fini;

extern "C" void __main (void)
{}

// Call C constructors
static int _initterm_e (const _PIFV* pfbegin, const _PIFV* pfend) noexcept
{
	int ret = 0;

	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend && ret == 0) {
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			ret = (**pfbegin)();
		++pfbegin;
	}

	return ret;
}

// Call C++ constructors
static void _initterm (const _PVFV* pfbegin, const _PVFV* pfend) noexcept
{
	// walk the table of function pointers from the bottom up, until
	// the end is encountered.  Do not skip the first entry.  The initial
	// value of pfbegin points to the first valid entry.  Do not try to
	// execute what pfend points to.  Only entries before pfend are valid.
	while (pfbegin < pfend)
	{
		// if current table entry is non-NULL, call thru it.
		if (*pfbegin != 0)
			(**pfbegin)();
		++pfbegin;
	}
}

extern "C" const PIMAGE_TLS_CALLBACK* __scrt_get_dyn_tls_init_callback ();

namespace CRTL {

#ifndef NDEBUG
_THREAD_LOCAL int test_tls = 0;
#endif

bool initialize () noexcept
{
	if (!CRTL::Global::initialize ())
		return false;

	// Do C initialization:
	if (_initterm_e (__xi_a, __xi_z) != 0)
		return false;

	// Do C++ initialization:
	_initterm (__xc_a, __xc_z);

  // If this module has any dynamically initialized __declspec(thread)
  // variables, then we invoke their initialization for the primary thread
  // used to start the process:
	auto tls_init_callback = __scrt_get_dyn_tls_init_callback ();
  if (*tls_init_callback)
    (*tls_init_callback) (nullptr, DLL_THREAD_ATTACH, nullptr);

  // Check that TLS is working OK.
#ifndef NDEBUG
	int cur = test_tls;
	assert (cur == 0);
	test_tls = 1;
	assert (test_tls == 1);
#endif

  return true;
}

void terminate () noexcept
{
	// Do pre-termination:
	_initterm (__xp_a, __xp_z);

	// Do termination:
	_initterm (__xt_a, __xt_z);

	CRTL::Global::terminate ();
}

}
