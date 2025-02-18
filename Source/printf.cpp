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
#include "../../pch/pch.h"
#include <Nirvana/printf.h>
#include <Nirvana/Formatter.h>
#include <errno.h>

namespace Nirvana {

int printf (WideIn& fmt, va_list args, WideOut& out, const struct lconv* loc) noexcept
{
	try {
		return (int)Formatter::format (fmt, args, out, loc);
	} catch (const CORBA::CODESET_INCOMPATIBLE&) {
		errno = EILSEQ;
	} catch (const CORBA::NO_MEMORY&) {
		errno = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int err = get_minor_errno (ex.minor ());
		if (!err)
			err = EINVAL;
		errno = err;
	} catch (const std::bad_alloc&) {
		errno = ENOMEM;
	} catch (...) {
		errno = EINVAL;
	}
	return -1;
}

}
