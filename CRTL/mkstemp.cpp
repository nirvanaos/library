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
#include <stdlib.h>
#include "name_service.h"

extern "C" int mkostemps (char* tpl, int suffixlen, int flags)
{
	int err = EIO;
	try {
		CosNaming::Name dir_name;
		IDL::String file;
		size_t tpl_len;
		auto ns = CRTL::name_service ();
		{
			IDL::String tpl_path (tpl);
			tpl_len = tpl_path.size ();
			Nirvana::the_system->append_path (dir_name, tpl_path, true);
			CosNaming::Name file_name;
			file_name.push_back (std::move (dir_name.back ()));
			dir_name.pop_back ();
			file = Nirvana::the_system->to_string (file_name);
		}

		Nirvana::AccessBuf::_ref_type access = Nirvana::AccessBuf::_downcast (
			Nirvana::Dir::_narrow (ns->resolve (dir_name))->
				mkostemps (file, (uint16_t)suffixlen, (uint16_t)flags, 0)->_to_value ());

		int fd = Nirvana::the_system->fd_add (access);
		size_t src_end = file.size () - suffixlen;
		size_t src_begin = src_end - 6;
		const char* src = file.c_str ();
		Nirvana::real_copy (src + src_begin, src + src_end, tpl + tpl_len - suffixlen - 6);
		return fd;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_system->error_number () = err;
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
