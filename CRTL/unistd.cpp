#include <unistd.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <Nirvana/File.h>

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
		CosNaming::Name name = Nirvana::g_system->get_current_dir_name ();
		Nirvana::g_system->get_name_from_path (name, path);
		auto ns = CosNaming::NamingContext::_narrow (CORBA::g_ORB->resolve_initial_references ("NameService"));
		ns->unbind (name);
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

extern "C" int rmdir (const char* path)
{
	int err = EIO;
	try {
		CosNaming::Name name = Nirvana::g_system->get_current_dir_name ();
		Nirvana::g_system->get_name_from_path (name, path);
		auto ns = CosNaming::NamingContext::_narrow (CORBA::g_ORB->resolve_initial_references ("NameService"));
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
	} catch (...) {
	}
	*(int*)Nirvana::g_system->error_number () = err;
	return -1;
}
