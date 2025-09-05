/*
* Nirvana C runtime library.
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
#include "fdio.h"
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>

namespace CRTL {

int read (int fildes, void* buf, size_t count, ssize_t& readed) noexcept
{
	int err = EIO;
	try {
		readed = Nirvana::the_posix->read (fildes, buf, count);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int write (int fildes, const void* buf, size_t count) noexcept
{
	int err = EIO;
	try {
		Nirvana::the_posix->write (fildes, buf, count);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int lseek (int fildes, off_t offset, int whence, fpos_t& pos) noexcept
{
	int err = EIO;
	try {
		if (Nirvana::the_posix->seek (fildes, offset, whence, pos))
			return 0;
		else
			err = ESPIPE;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int close (int fildes) noexcept
{
	int err = EIO;
	try {
		Nirvana::the_posix->close (fildes);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int open (const char* path, int oflag, mode_t mode, int& fildes) noexcept
{
	int err = EIO;
	try {
		fildes = Nirvana::the_posix->open (path, oflag, mode);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int isatty (int fildes, bool& atty) noexcept
{
	int err = EIO;
	try {
		atty = Nirvana::the_posix->isatty (fildes);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

int fcntl (int fildes, int cmd, uintptr_t param, int& ret) noexcept
{
	int err = EIO;
	try {
		ret = Nirvana::the_posix->fcntl (fildes, cmd, param);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	return err;
}

}
