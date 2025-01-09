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
#ifndef NIRVANA_STRTOI_H_
#define NIRVANA_STRTOI_H_
#pragma once

#include "WideInEx.h"

namespace Nirvana {

template <typename C, typename I> inline
void strtoi (const C* s, C** endptr, int base, I& ret) noexcept
{
	ret = 0;
	size_t pos = 0;

	try {
		WideInStrT <C> in_s (s);
		WideInEx in (in_s);
		try {
			in.get_int (ret, base);
			errno = 0;
			pos = in.pos ();
		} catch (const CORBA::DATA_CONVERSION& ex) {
			int err = get_minor_errno (ex.minor ());
			if (ERANGE == err) {
				if (std::is_signed <I>::value) {
					if (ret != std::numeric_limits <I>::min ())
						ret = std::numeric_limits <I>::max ();
				} else
					ret = std::numeric_limits <I>::max ();
				pos = in.pos ();
			}
			errno = err;
		}
	} catch (const CORBA::SystemException& ex) {
		errno = get_minor_errno (ex.minor ());
	}

	if (endptr)
		*endptr = const_cast <char*> (s + pos);
}

}

#endif
