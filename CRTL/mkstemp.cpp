#include <stdlib.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <Nirvana/File.h>
#include "name_service.h"

extern "C" int mkstemp (char* tpl)
{
	int err = EIO;
	try {
		IDL::String name = tpl;
		Nirvana::AccessBuf::_ref_type access = Nirvana::AccessBuf::_downcast (
			Nirvana::Dir::_narrow (Nirvana::name_service ()->resolve_str ("/var/tmp"))->mkostemps (name, 0, 0)->_to_value ());
		int fd = Nirvana::g_system->fd_add (access);
		const char* p = name.c_str ();
		std::copy (p, p + name.size () + 1, tpl);
		return fd;
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

