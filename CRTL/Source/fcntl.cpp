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
#include <fcntl.h>
#include <stdarg.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>
#include "impl/fdio.h"

extern "C" int fcntl (int fildes, int cmd, ...)
{
	int arg = 0;
	switch (cmd) {
	case F_DUPFD:
	case F_SETFD:
	case F_SETFL:
		va_list arglist;
		va_start (arglist, cmd);
		arg = va_arg (arglist, int);
		va_end (arglist);
	}

	int err = EIO;
	try {
		return Nirvana::the_posix->fcntl (fildes, cmd, arg);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_posix->error_number () = err;
	return -1;
}

extern "C" int open (const char* path, int oflag, ...)
{
	mode_t mode = 0;
	if (oflag & O_CREAT) {
		va_list arglist;
		va_start (arglist, oflag);
		mode = va_arg (arglist, int);
		va_end (arglist);
	}
	int fildes;
	int err = CRTL::open (path, oflag, mode, fildes);
	if (err) {
		*(int*)Nirvana::the_posix->error_number () = err;
		return -1;
	} else
		return fildes;
}

extern "C" int creat (const char* path, int mode)
{
	return open (path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
