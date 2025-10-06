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
#ifndef NIRVANA_CRT_CUSTOM_INIT_H_
#define NIRVANA_CRT_CUSTOM_INIT_H_
#pragma once

#ifndef __ELF__

#include "OLF.h"

#pragma section (".CRT$XID", long, read) // Custom initializer
#pragma section (".CRT$XPU", long, read) // Custom terminator

typedef void (__cdecl* _PVFV) (void);
typedef int (__cdecl* _PIFV) (void);

#if defined _MSC_VER && !defined (__clang__)

#define NIRVANA_CRT_CUSTOM_INITIALIZER(func) int _custom_initializer () { func(); return 0; }\
extern "C" __declspec (allocate(".CRT$XID")) _PIFV _custom_initializer_ptr = _custom_initializer;\
NIRVANA_LINK_SYMBOL (_custom_initializer_ptr)

#define NIRVANA_CRT_CUSTOM_TERMINATOR(func)\
extern "C" __declspec (allocate(".CRT$XPU")) _PVFV _custom_terminator_ptr = func;\
NIRVANA_LINK_SYMBOL (_custom_terminator_ptr)

#else

#define NIRVANA_CRT_CUSTOM_INITIALIZER(func) int _custom_initializer () { func(); return 0; }\
__attribute__ ((section (".CRT$XID"))) const _PIFV __attribute__ ((used)) _custom_initializer_ptr = _custom_initializer;

#define NIRVANA_CRT_CUSTOM_TERMINATOR(func)\
__attribute__ ((section (".CRT$XPU"))) const _PVFV __attribute__ ((used)) _custom_terminator_ptr = func;

#endif

#else

#define NIRVANA_CRT_CUSTOM_INITIALIZER(func) __attribute__ ((constructor (101))) void func;
#define NIRVANA_CRT_CUSTOM_TERMINATOR(func) __attribute__ ((destructor)) void func;

#endif

#endif
