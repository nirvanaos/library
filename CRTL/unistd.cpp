#include <unistd.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <Nirvana/File.h>
#include "name_service.h"

extern "C" int close (int fd)
{
	int err = EIO;
	try {
		Nirvana::g_system->fd_close ((uint16_t)fd);
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
		Nirvana::g_system->fd_flush ((uint16_t)fd);
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
			IDL::String path = Nirvana::name_service ()->to_string (Nirvana::g_system->get_current_dir_name ());
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
		return Nirvana::g_system->fd_seek ((uint16_t)fildes, offset, whence);
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
		return Nirvana::g_system->fd_read ((uint16_t)fildes, buf, count);
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
		Nirvana::name_service ()->unbind (Nirvana::g_system->get_name_from_path (path));
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
		auto ns = Nirvana::name_service ();
		CosNaming::Name name = Nirvana::g_system->get_name_from_path (path);
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
		Nirvana::name_service ()->bind_new_context (Nirvana::g_system->get_name_from_path (path));
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
