/// \file
/*
* Nirvana runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2021 Igor Popov.
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
#ifndef NIRVANA_STRTOF_H_
#define NIRVANA_STRTOF_H_
#pragma once

#include "WideInEx.h"
#include "POSIX.h"
#include <errno.h>

namespace Nirvana {

template <typename C, typename F> inline
int strtof (const C* s, C** endptr, F& ret, const struct lconv* lconv =  nullptr) noexcept
{
	ret = 0;
	size_t pos = 0;
	int err = 0;

	try {
		WideInStrT <C> in_s (s);
		WideInEx in (in_s);
		FloatMax f;
		in.get_float (f, lconv);
		ret = (F)f;
		pos = in.pos ();
	} catch (const CORBA::SystemException& ex) {
		ret = std::numeric_limits <F>::max ();
		err = get_minor_errno (ex.minor ());
	}

	if (endptr)
		*endptr = const_cast <char*> (s + pos);

	return err;
}

}

#endif
