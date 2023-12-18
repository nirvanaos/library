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
#include <sys/stat.h>
#include "name_service.h"

extern "C" int stat (const char* path, struct stat* st)
{
	static const uint64_t FROM_100NS = 10000000;

	int err = EIO;
	try {
		Nirvana::FileStat fst;
		CosNaming::Name name;
		Nirvana::g_system->append_path (name, path, true);
		Nirvana::DirItem::_narrow (CRTL::name_service ()->resolve (name))->stat (fst);
		st->st_dev = fst.dev ();
		st->st_ino = fst.ino ();
		st->st_mode = fst.mode ();
		st->st_nlink = fst.nlink ();
		st->st_size = fst.size ();
		st->st_atime = fst.last_access_time ().time () / FROM_100NS;
		st->st_mtime = fst.last_write_time ().time () / FROM_100NS;
		st->st_ctime = fst.creation_time ().time () / FROM_100NS;
		st->st_blksize = fst.blksize ();
		st->st_blocks = fst.blkcnt ();
		return 0;
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
