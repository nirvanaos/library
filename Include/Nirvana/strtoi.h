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
#include <errno.h>

namespace Nirvana {

template <typename C, typename Int> inline
void strtoi (const C* s, C** endptr, int base, Int& ret) noexcept
{
	ret = 0;

	WideInStrT <C> in_s (s);
	int32_t last = EOF;
	int err = 0;

	try {
		WideInEx in (in_s);
		in.get_int (ret, base);
		last = in.cur ();
	} catch (const CORBA::SystemException& ex) {
		err = get_minor_errno (ex.minor ());
		if (ERANGE == err) {
			if (std::is_signed <Int>::value) {
				if (ret != std::numeric_limits <Int>::min ())
					ret = std::numeric_limits <Int>::max ();
			} else
				ret = std::numeric_limits <Int>::max ();
		}
	}

	errno = err;

	if (endptr) {
		const C* end;
		if (err != 0 && err != ERANGE)
			end = s;
		else {
			end = in_s.cur_ptr ();
			if (last != EOF && end > s)
				--end;
		}
		*endptr = const_cast <C*> (end);
	}
}

}

#endif
