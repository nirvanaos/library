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
#include <Nirvana/crt_startup.h>
#include "crtdefs.h"
#include "../Global.h"

extern "C" _CRTALLOC (".CRT$XIA") _PIFV __xi_a [] = { nullptr }; // C initializers (first)
extern "C" _CRTALLOC (".CRT$XIZ") _PIFV __xi_z [] = { nullptr }; // C initializers (last)
extern "C" _CRTALLOC (".CRT$XCA") _PVFV __xc_a [] = { nullptr }; // C++ initializers (first)
extern "C" _CRTALLOC (".CRT$XCZ") _PVFV __xc_z [] = { nullptr }; // C++ initializers (last)
extern "C" _CRTALLOC (".CRT$XPA") _PVFV __xp_a [] = { nullptr }; // C pre-terminators (first)
extern "C" _CRTALLOC (".CRT$XPZ") _PVFV __xp_z [] = { nullptr }; // C pre-terminators (last)
extern "C" _CRTALLOC (".CRT$XTA") _PVFV __xt_a [] = { nullptr }; // C terminators (first)
extern "C" _CRTALLOC (".CRT$XTZ") _PVFV __xt_z [] = { nullptr }; // C terminators (last)

#pragma comment(linker, "/merge:.CRT=.rdata")

extern "C" _PIFV __xi_a []; // First C Initializer
extern "C" _PIFV __xi_z []; // Last C Initializer
extern "C" _PVFV __xc_a []; // First C++ Initializer
extern "C" _PVFV __xc_z []; // Last C++ Initializer
extern "C" _PVFV __xp_a []; // First Pre-Terminator
extern "C" _PVFV __xp_z []; // Last Pre-Terminator
extern "C" _PVFV __xt_a []; // First Terminator
extern "C" _PVFV __xt_z []; // Last Terminator

// Call C constructors
static int _initterm_e (_PIFV* pfbegin, _PIFV* pfend)
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
static void _initterm (_PVFV* pfbegin, _PVFV* pfend)
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

namespace Nirvana {

#ifndef NDEBUG
_THREAD_LOCAL int test_tls = 0;
#endif

bool crt_init () noexcept
{
#ifndef NDEBUG
	int cur = test_tls;
	assert (cur == 0);
	test_tls = 1;
	assert (test_tls == 1);
#endif

	if (!CRTL::Global::initialize ())
		return false;

	// Do C initialization:
	if (_initterm_e (__xi_a, __xi_z) != 0)
		return false;

	// Do C++ initialization:
	_initterm (__xc_a, __xc_z);
	return true;
}

void crt_term () noexcept
{
	// Do pre-termination:
	_initterm (__xp_a, __xp_z);

	// Do termination:
	_initterm (__xt_a, __xt_z);

	CRTL::Global::terminate ();
}

}
