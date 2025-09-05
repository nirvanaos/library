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
#include <sys/stat.h>
#include <CORBA/CORBA.h>
#include <Nirvana/POSIX.h>
#include <Nirvana/Hash.h>

inline static void to_timespec (const TimeBase::TimeT& time, struct timespec& ts)
{
	ts.tv_sec = time / 10000000;
	ts.tv_nsec = (time % 10000000) * 100;
}

extern "C" int stat (const char* path, struct stat* st)
{
	int err = EIO;
	try {
		Nirvana::FileStat fst;
		Nirvana::the_posix->stat (path, fst);
		st->st_dev = fst.dev ();
		st->st_ino = Nirvana::HashFunction <ino_t>::hash_bytes (fst.id ().data (), (int)fst.id ().size ());
		st->st_uid = Nirvana::HashFunction <uid_t>::hash_bytes (fst.owner ().data (), (int)fst.owner ().size ());
		st->st_gid = Nirvana::HashFunction <gid_t>::hash_bytes (fst.group ().data (), (int)fst.group ().size ());
		st->st_mode = fst.mode ();
		st->st_nlink = fst.nlink ();
		st->st_size = fst.size ();
		to_timespec (fst.last_access_time ().time (), st->st_atim);
		to_timespec (fst.last_write_time ().time (), st->st_mtim);
		to_timespec (fst.creation_time ().time (), st->st_ctim);
		st->st_blksize = fst.blksize ();
		st->st_blocks = fst.blkcnt ();
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
