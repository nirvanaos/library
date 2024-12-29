/*
* Nirvana C runtime library.
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
#include "pch/pch.h"
#include <sys/stat.h>

extern "C" int stat (const char* path, struct stat* st)
{
	int err = EIO;
	try {
		Nirvana::the_posix->stat (path, st);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (const CosNaming::NamingContext::InvalidName&) {
		err = ENOENT;
	} catch (const CosNaming::NamingContext::NotFound& ex) {
		err = ex.why () == CosNaming::NamingContext::NotFoundReason::not_context ? ENOTDIR : ENOENT;
	} catch (...) {
	}
	*(int*)Nirvana::the_posix->error_number () = err;
	return -1;
}
