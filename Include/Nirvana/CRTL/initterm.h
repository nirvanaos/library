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
#ifndef NIRVANA_CRTL_INITTERM_H_
#define NIRVANA_CRTL_INITTERM_H_
#pragma once

namespace CRTL {

bool initialize () noexcept;
void terminate () noexcept;

}

#ifndef __ELF__

#include "Windows/custom_init.h"

#else

#define CRTL_CUSTOM_INITIALIZER(func) __attribute__ ((constructor (101))) void func;
#define CRTL_CUSTOM_TERMINATOR(func) __attribute__ ((destructor)) void func;

#endif

#endif
