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

#include "Formatter.h"

namespace Nirvana {

/// \brief Generalized C-style formatting function.
/// 
/// \typeparam C Character type.
/// \param fmt Format string.
/// \param args Arguments.
/// \param out Output stream.
/// \param [out] ret Number of wide characters transmitted to the output stream.
/// \param loc Locale conversion settings.
/// \returns Zero on success or error code.
template <class C> inline
int vprintf (const C* fmt, va_list args, WideOut& out, size_t& ret,
	const struct lconv* loc = nullptr) noexcept
{
	int err = 0;
	try {
		WideInStrT <C> fmt_in (fmt);
		ret = Formatter::format (fmt_in, args, out, loc);
	} catch (const CORBA::CODESET_INCOMPATIBLE&) {
		err = EILSEQ;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = get_minor_errno (ex.minor ());
		if (e)
			err = e;
		else
			err = EINVAL;
	} catch (const std::bad_alloc&) {
		err = ENOMEM;
	} catch (...) {
		err = EINVAL;
	}
	return err;
}

/// \brief Generalized vsnprintf implementation
/// 
/// \typeparam C Character type.
/// \param buffer Output buffer pointer.
/// \param bufsz Output buffer size.
/// \param fmt Format string.
/// \param args Arguments.
/// \param ret Number of characters (not including the terminating null character) which would have
///   been written to buffer if bufsz was ignored.
/// \param loc Locale conversion settings.
/// \returns Zero on success or error code.
template <class C>
int vsnprintf (C* buffer, size_t bufsz, const C* fmt, va_list args, size_t& ret,
	const struct lconv* loc = nullptr) noexcept
{
	WideOutBufT <C> out (buffer, buffer + bufsz);
	size_t cnt;
	int err = vprintf (fmt, args, out, cnt, loc);
	ret = out.count ();
	return err;
}

}

#endif
