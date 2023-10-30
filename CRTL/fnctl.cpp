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
#include <fnctl.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>

using namespace Nirvana;

extern "C" int fcntl (int fildes, int cmd, int arg)
{
	int err = EIO;
	try {
		return g_system->fcntl ((uint16_t)fildes, (int16_t)cmd, (uint16_t)arg);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)g_system->error_number () = err;
	return -1;
}

extern "C" int open (const char* path, int oflag, int mode)
{
	int err = EIO;
	try {
		return g_system->fd_open (path, (uint16_t)oflag, (uint16_t)mode);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)g_system->error_number () = err;
	return -1;
}

extern "C" int creat (const char* path, int mode)
{
	return open (path, O_WRONLY | O_CREAT | O_TRUNC, mode);
}
