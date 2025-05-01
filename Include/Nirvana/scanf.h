/// \file
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_SCANF_H_
#define NIRVANA_SCANF_H_
#pragma once

#include "WideIn.h"
#include <stdarg.h>

namespace Nirvana {

/// \brief Generalized C-style formatting function.
/// As it intended to C, it does not throw exceptions
/// but sets `errno` codes on error instead.
int scanf (WideIn& in, WideIn& format, va_list args, const struct lconv* loc = nullptr);

template <class C>
int sscanf (const C* buffer, const C* format, va_list args, const struct lconv* loc = nullptr)
{
	WideInStrT <C> in (buffer);
	WideInStrT <C> fmt (format);
	return scanf (in, fmt, args, loc);
}

}

#endif
