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
#include <unistd.h>
#include <fcntl.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/POSIX.h>
#include "impl/fdio.h"

extern "C" int chdir (const char* path)
{
	int err = EINVAL;
	try {
		Nirvana::the_posix->chdir (path);
		return 0;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_posix->error_number () = err;
	return -1;
}

extern "C" int close (int fd)
{
	int err = EIO;
	try {
		Nirvana::the_posix->close (fd);
		return 0;
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

extern "C" int fsync (int fd)
{
	int err = EIO;
	try {
		Nirvana::the_posix->fsync (fd);
		return 0;
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

extern "C" int fdatasync (int fd)
{
	return fsync (fd);
}

extern "C" char* getcwd (char* buf, size_t size)
{
	int err = 0;
	if (0 == size)
		err = EINVAL;
	else {
		try {
			IDL::String path = Nirvana::the_posix->get_current_dir ();
			size_t len = path.length () + 1;
			if (size < len)
				err = ERANGE;
			else {
				const char* p = path.c_str ();
				std::copy (p, p + len, buf);
				return buf;
			}
		} catch (const CORBA::NO_MEMORY&) {
			err = ENOMEM;
		}
	}
	*(int*)Nirvana::the_posix->error_number () = err;
	return nullptr;
}

extern "C" off_t lseek (int fildes, off_t offset, int whence)
{
	off_t pos;
	int err = CRTL::lseek (fildes, offset, whence, pos);
	if (err) {
		*(int*)Nirvana::the_posix->error_number () = err;
		return -1;
	} else
		return pos;
}

extern "C" ssize_t read (int fildes, void* buf, size_t count)
{
	ssize_t readed;
	int err = CRTL::read (fildes, buf, count, readed);
	if (err) {
		*(int*)Nirvana::the_posix->error_number () = err;
		return -1;
	} else
		return readed;
}

extern "C" ssize_t write (int fildes, const void* buf, size_t count)
{
	int err = CRTL::write (fildes, buf, count);
	if (err) {
		*(int*)Nirvana::the_posix->error_number () = err;
		return -1;
	} else
		return count;
}

extern "C" int unlink (const char* path)
{
	int err = EIO;
	try {
		Nirvana::the_posix->unlink (path);
		return 0;
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

extern "C" int rmdir (const char* path)
{
	int err = EIO;
	try {
		Nirvana::the_posix->rmdir (path);
		return 0;
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

extern "C" int mkdir (const char* path, mode_t mode)
{
	int err = EIO;
	try {
		Nirvana::the_posix->mkdir (path, mode);
		return 0;
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

extern "C" int dup (int fildes)
{
	return fcntl (fildes, F_DUPFD, 0);
}

extern "C" int dup2 (int src, int dst)
{
	int err = EIO;
	try {
		Nirvana::the_posix->dup2 (src, dst);
		return 0;
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

extern "C" int isatty (int fildes)
{
	int err = EIO;
	try {
		return Nirvana::the_posix->isatty (fildes);
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

extern "C" unsigned sleep (unsigned seconds)
{
	try {
		Nirvana::the_posix->sleep ((TimeBase::TimeT)seconds * TimeBase::SECOND);
		return 0;
	} catch (...) {
		return seconds;
	}
}
