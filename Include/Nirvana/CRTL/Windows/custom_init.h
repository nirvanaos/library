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
#pragma once

#include <Nirvana/OLF.h>
#include "crtdefs.h"

#pragma section (".CRT$XID", long, read) // Custom initializer
#pragma section (".CRT$XPU", long, read) // Custom terminator

#if defined _MSC_VER && !defined (__clang__)

#define CRTL_CUSTOM_INITIALIZER(func) int _crtl_custom_initializer () { func(); return 0; }\
extern "C" _CRTALLOC (".CRT$XID") _PIFV _crtl_custom_initializer_ptr = _crtl_custom_initializer;\
NIRVANA_LINK_SYMBOL (_crtl_custom_initializer_ptr)

#define CRTL_CUSTOM_TERMINATOR(func)\
extern "C" _CRTALLOC (".CRT$XPU") _PVFV _crtl_custom_terminator_ptr = func;\
NIRVANA_LINK_SYMBOL (_crtl_custom_terminator_ptr)

#else

#define CRTL_CUSTOM_INITIALIZER(func) int _crtl_custom_initializer () { func(); return 0; }\
_CRTALLOC (".CRT$XID") const _PIFV __attribute__ ((used)) _crtl_custom_initializer_ptr = _crtl_custom_initializer;

#define CRTL_CUSTOM_TERMINATOR(func)\
_CRTALLOC (".CRT$XPU") const _PVFV __attribute__ ((used)) _crtl_custom_terminator_ptr = func;

#endif
