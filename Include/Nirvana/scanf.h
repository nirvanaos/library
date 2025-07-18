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

#include "Parser.h"

namespace Nirvana {

// \brief Generalized C-style scan function.
template <class C>
int vscanf (WideIn& in, const C* fmt, va_list args, size_t& cnt,
	const struct lconv* loc = nullptr) noexcept
{
	int err;
	try {
		WideInStrT <C> fmt_in (fmt);
		Parser::parse (in, fmt_in, args, cnt, loc);
		err = 0;
	} catch (const CORBA::SystemException& ex) {
		err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
	} catch (...) {
		err = EINVAL;
	}
	return err;
}


/// \brief Generalized C-style scan function.
template <class C>
int vsscanf (const C* buffer, const C* fmt, va_list args, size_t& cnt,
	const struct lconv* loc = nullptr) noexcept
{
	WideInStrT <C> in (buffer);
	return vscanf (in, fmt, args, cnt, loc);
}

}

#endif
