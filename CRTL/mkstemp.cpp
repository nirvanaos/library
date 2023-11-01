#include <stdlib.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <Nirvana/File.h>
#include "name_service.h"

extern "C" int mkostemps (char* tpl, int suffixlen, int flags)
{
	int err = EIO;
	try {
		IDL::String name = tpl;
		Nirvana::AccessBuf::_ref_type access = Nirvana::AccessBuf::_downcast (
			Nirvana::Dir::_narrow (Nirvana::name_service ()->resolve_str ("/var/tmp"))->
				mkostemps (name, (uint16_t)suffixlen, (uint16_t)flags)->_to_value ());
		int fd = Nirvana::g_system->fd_add (access);
		size_t tpl_end = name.size () - suffixlen;
		size_t tpl_begin = tpl_end - 6;
		const char* src = name.c_str ();
		Nirvana::real_copy (src + tpl_begin, src + tpl_end, tpl + tpl_begin);
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

extern "C" int mkstemps (char* tpl, int suffixlen)
{
	return mkostemps (tpl, suffixlen, 0600);
}

extern "C" int mkostemp (char* tpl, int flags)
{
	return mkostemps (tpl, 0, flags);
}

extern "C" int mkstemp (char* tpl)
{
	return mkostemps (tpl, 0, 0600);
}
