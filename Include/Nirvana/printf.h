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
#ifndef NIRVANA_PRINTF_H_
#define NIRVANA_PRINTF_H_
#pragma once

#include "WideIn.h"
#include "WideOut.h"
#include <stdarg.h>

namespace Nirvana {

/// \brief Generalized C-style formatting function.
/// As it intended to C formatting, it does not throw exceptions
/// but sets `errno` codes on error instead.
/// 
/// \param fmt Format string.
/// \param args Arguments.
/// \param out Output stream.
/// \param loc Locale conversion settings.
/// \returns Number of wide characters transmitted to the output stream or negative value if an output
///          error or an encoding error (for string and character conversion specifiers) occurred.
int printf (WideIn& fmt, va_list args, WideOut& out, const struct lconv* loc = nullptr) noexcept;

/// \brief Generalized snprintf implementation
/// 
/// \typeparam C Character type.
/// \param buffer Output buffer pointer.
/// \param bufsz Output buffer size.
/// \param format Format string.
/// \param args Arguments.
/// \returns number of characters (not including the terminating null character) which would have
///   been written to buffer if bufsz was ignored, or a negative value if an encoding error (for
///   string and character conversion specifiers) occurred.
template <class C>
int snprintf (C* buffer, size_t bufsz, const C* format, va_list args) noexcept
{
	WideInStrT <C> fmt (format);
	WideOutBufT <C> out (buffer, buffer + bufsz);
	printf (fmt, args, out);
	return out.count ();
}

}

#endif
