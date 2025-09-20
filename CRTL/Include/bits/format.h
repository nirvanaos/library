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

#ifndef CRTL_BITS_FORMAT_H_
#define CRTL_BITS_FORMAT_H_
#pragma once

#if defined (__GNUG__) || defined (__clang__)
#define NIRVANA_PRINTF(f, a) __attribute__ ((format (printf, f, a)))
#define NIRVANA_SCANF(f, a) __attribute__ ((format (scanf, f, a)))
#define NIRVANA_STRFTIME(f) __attribute__ ((format (strftime, f, 0)))
#else
#define NIRVANA_PRINTF(f, a)
#define NIRVANA_SCANF(f, a)
#define NIRVANA_STRFTIME(f)
#endif

#define NIRVANA_WPRINTF(f, a)
#define NIRVANA_WSCANF(f, a)
#define NIRVANA_WSTRFTIME(f)

#endif

