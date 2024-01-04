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
#include <fnctl.h>
#include "name_service.h"
#include <Nirvana/Legacy/Legacy.h>

extern "C" int close (int fd)
{
	int err = EIO;
	try {
		Nirvana::g_system->close ((uint16_t)fd);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" int fsync (int fd)
{
	int err = EIO;
	try {
		Nirvana::g_system->flush ((uint16_t)fd);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
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
			IDL::String path = Nirvana::g_system->to_string (Nirvana::g_system->get_current_dir_name ());
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
	*(int*)Nirvana::g_system->error_number () = err;
	return nullptr;
}

extern "C" off_t lseek (int fildes, off_t offset, int whence)
{
	int err = EIO;
	try {
		return Nirvana::g_system->seek ((uint16_t)fildes, offset, whence);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" ssize_t read (int fildes, void* buf, size_t count)
{
	int err = EIO;
	try {
		return Nirvana::g_system->read ((uint16_t)fildes, buf, count);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" ssize_t write (int fildes, const void* buf, size_t count)
{
	int err = EIO;
	try {
		Nirvana::g_system->write ((uint16_t)fildes, buf, count);
		return count;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" int unlink (const char* path)
{
	int err = EIO;
	try {
		CosNaming::Name name;
		Nirvana::g_system->append_path (name, path, true);
		CRTL::name_service ()->unbind (name);
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
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" int rmdir (const char* path)
{
	int err = EIO;
	try {
		CosNaming::Name name;
		Nirvana::g_system->append_path (name, path, true);
		auto ns = CRTL::name_service ();
		Nirvana::Dir::_ref_type dir = Nirvana::Dir::_narrow (ns->resolve (name));
		if (dir) {
			dir->destroy ();
			dir = nullptr;
			ns->unbind (name);
			return 0;
		} else
			err = ENOTDIR;
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
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" int mkdir (const char* path, mode_t mode)
{
	int err = EIO;
	try {
		CosNaming::Name name;
		Nirvana::g_system->append_path (name, path, true);
		CRTL::name_service ()->bind_new_context (name);
		// TODO:: Fix access mask
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
	} catch (const CosNaming::NamingContext::AlreadyBound&) {
		err = EEXIST;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
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
		Nirvana::g_system->dup2 ((uint16_t)src, (uint16_t)dst);
		return 0;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" int isatty (int fildes)
{
	int err = EIO;
	try {
		return Nirvana::g_system->isatty ((uint16_t)fildes);
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}

extern "C" unsigned sleep (unsigned seconds)
{
	try {
		Nirvana::g_system->sleep ((TimeBase::TimeT)seconds * TimeBase::SECOND);
		return 0;
	} catch (...) {
		return seconds;
	}
}
