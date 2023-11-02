#include <sys/stat.h>
#include <Nirvana/Nirvana.h>
#include <Nirvana/System.h>
#include <Nirvana/File.h>
#include "name_service.h"

extern "C" int stat (const char* path, struct stat* st)
{
	static const uint64_t FROM_100NS = 10000000;

	int err = EIO;
	try {
		Nirvana::FileStat fst;
		CosNaming::Name name;
		Nirvana::g_system->append_path (name, path, true);
		Nirvana::DirItem::_narrow (Nirvana::name_service ()->resolve (name))->stat (fst);
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
