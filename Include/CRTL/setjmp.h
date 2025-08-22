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
#ifndef _SETJMP_H_
#define _SETJMP_H_
#pragma once

#ifndef _WIN32

#include <Nirvana/jmpbuf.h>

#ifdef __cplusplus
extern "C" {
#endif

/* [C11/7.13] Non-local jumps */

typedef struct __jmp_buf {
	struct __mlibc_jmpbuf_register_state __reg_state;
} jmp_buf[1];

__attribute__((__returns_twice__)) int setjmp(jmp_buf __buffer);
__attribute__((__noreturn__)) void longjmp(jmp_buf __buffer, int __value);

#ifdef __cplusplus
}
#endif

#else

// Stubs

#define _setjmp(b) __setjmp()
#define _setjmpex(b) __setjmpex()

#endif

#endif
