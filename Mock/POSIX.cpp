/// \file Mock implementation of the POSIX interface.
/*
* Nirvana mock library.
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
#include <CORBA/Server.h>
#include <Nirvana/POSIX_s.h>
#include <Nirvana/nls_s.h>
#include <Nirvana/locale.h>
#include <Nirvana/RandomGen.h>
#include "HostAPI.h"

namespace Nirvana {
namespace Test {

struct FlagConv
{
	unsigned n, host;
};

struct LocaleInit
{
	LocaleInit ()
	{
		NIRVANA_VERIFY (HostAPI::setlocale (0, "en_US.UTF-8"));
	}

} locale_init;

class DefaultLocale :
	public IDL::traits <Nirvana::Locale>::ServantStatic <DefaultLocale>
{
public:
	static const char* name () noexcept
	{
		return "C.UTF-8";
	}

	static Nirvana::Facet::_ref_type get_facet (int) noexcept
	{
		return Nirvana::Facet::_ptr_type (
			IDL::traits <Nirvana::Facet>::ServantStatic <DefaultLocale>::_get_ptr ());
	}

	static const struct lconv* localeconv () noexcept
	{
		return &lconv_;
	}

private:
	static const struct lconv lconv_;
};

#define LCONV_STR(t) const_cast <char*> (t)

const struct lconv DefaultLocale::lconv_ = {
	LCONV_STR ("."),
	LCONV_STR (""),
	LCONV_STR (""),
	LCONV_STR (""),
	LCONV_STR (""),
	LCONV_STR (""),
	LCONV_STR (""),
	LCONV_STR ("-"),
	LCONV_STR (""),
	LCONV_STR (""),
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX,
	CHAR_MAX
};

static RandomGen random_gen;

class POSIX :
	public CORBA::servant_traits <Nirvana::POSIX>::ServantStatic <POSIX>
{
public:
	static TimeBase::UtcT system_clock ()
	{
		TimeBase::UtcT ret = UTC ();
		ret.tdf (HostAPI::time_zone_offset ());
		return ret;
	}

	static TimeBase::UtcT UTC ();

	static void set_UTC (const TimeBase::TimeT& t)
	{
		throw_NO_IMPLEMENT ();
	}

	static const TimeBase::TimeT& system_clock_resolution ()
	{
		static const TimeBase::TimeT r = 1;
		return r;
	}

	static SteadyTime steady_clock ()
	{
		return HostAPI::steady_clock ();
	}

	static const SteadyTime& steady_clock_resolution ()
	{
		static const SteadyTime r = 1;
		return r;
	}

	static DeadlineTime deadline_clock ()
	{
		return steady_clock ();
	}

	static const uint64_t& deadline_clock_frequency ()
	{
		static const uint64_t r = 10000000UI64;
		return r;
	}

	static int* error_number ()
	{
		return HostAPI::error_number ();
	}

	static void raise (int signal)
	{
		HostAPI::raise (signal);
	}

	static void sigaction (int signal, const struct sigaction* act, struct sigaction* oldact)
	{
		throw_NO_IMPLEMENT ();
	}

	static void srand (unsigned seed)
	{
		random_gen.state (seed);
	}

	static unsigned rand ()
	{
		return random_gen ();
	}

	static IDL::String get_current_dir ()
	{
		IDL::String ret (HostAPI::max_path (), 0);
		char* dir = HostAPI::getcwd (&*ret.begin (), ret.size ());
		if (!dir)
			throw_NO_MEMORY ();
		ret.resize (strlen (dir));
		return ret;
	}

	static void chdir (const IDL::String& path)
	{
		int err = HostAPI::chdir (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static FilDesc open (const IDL::String& path, unsigned oflag, unsigned mode)
	{
		int ret;
		int err = HostAPI::open (path.c_str (), oflag, mode, ret);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));

		return ret;
	}

	static FilDesc mkostemps (CharPtr tpl, unsigned suffix_len, unsigned flags)
	{
		throw_NO_IMPLEMENT ();
	}

	static void close (FilDesc fd)
	{
		int err = HostAPI::close (fd);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static size_t read (FilDesc fd, void* p, size_t size)
	{
		size_t cb;
		int err = HostAPI::read (fd, p, size, cb);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		return cb;
	}

	static void write (FilDesc fd, const void* p, size_t size)
	{
		int err = HostAPI::write (fd, p, size);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static FileSize seek (FilDesc fd, const FileOff& offset, int whence)
	{
		FileSize pos;
		int err = HostAPI::seek (fd, offset, whence, pos);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		return pos;
	}

	static FilDesc fcntl (FilDesc fd, unsigned cmd, uintptr_t arg)
	{
		throw_NO_IMPLEMENT ();
	}

	static void fsync (FilDesc fd)
	{
	}

	static void dup2 (FilDesc src, FilDesc dst)
	{
		int err = HostAPI::dup2 (src, dst);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static bool isatty (FilDesc fd)
	{
		return HostAPI::isatty (fd);
	}

	static void sleep (TimeBase::TimeT period100ns)
	{
		HostAPI::sleep (period100ns);
	}

	static void yield ()
	{
		HostAPI::yield ();
	}

	static void unlink (const IDL::String& path)
	{
		int err = HostAPI::unlink (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void rmdir (const IDL::String& path)
	{
		int err = HostAPI::rmdir (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void mkdir (const IDL::String& path, unsigned mode)
	{
		int err = HostAPI::mkdir (path.c_str (), mode);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	void stat (const IDL::String& path, FileStat& st)
	{
		HostAPI::Stat hst;
		int err = HostAPI::stat (path.c_str (), hst);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));

		st.id (make_id (hst.ino));
		st.owner (make_id (hst.uid));
		st.group (make_id (hst.gid));
		st.size (hst.size);

		st.creation_time ().time (from_timespec (hst.ctim));
		st.last_access_time ().time (from_timespec (hst.atim));
		st.last_write_time ().time (from_timespec (hst.mtim));

		st.dev (hst.dev);
		st.nlink (hst.nlink);

		st.mode (hst.mode);
	}

	static void rename (const IDL::String& oldname, const IDL::String& newname)
	{
		int err = HostAPI::rename (oldname.c_str (), newname.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static CS_Key CS_alloc (Deleter deleter)
	{
		CS_Key key;
		int err = HostAPI::CS_alloc (deleter, key);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		return key;
	}

	static void CS_free (unsigned idx)
	{
		HostAPI::CS_free (idx);
	}

	static void CS_set (unsigned idx, void* ptr)
	{
		int err = HostAPI::CS_set (idx, ptr);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void* CS_get (unsigned idx)
	{
		return HostAPI::CS_get (idx);
	}

	static uint32_t hardware_concurrency ()
	{
		return HostAPI::hardware_concurrency ();
	}

	static Locale::_ptr_type cur_locale ()
	{
		return DefaultLocale::_get_ptr ();
	}

	static void cur_locale (Nirvana::Locale::_ptr_type loc)
	{
		throw_NO_IMPLEMENT (make_minor_errno (ENOTSUP));
	}

	static Nirvana::Locale::_ref_type create_locale (int mask, const IDL::String& name, Nirvana::Locale::_ptr_type base)
	{
		throw_NO_IMPLEMENT (make_minor_errno (ENOTSUP));
	}

private:
	template <typename T>
	static CORBA::OctetSeq make_id (const T& x)
	{
		const CORBA::Octet* p = (const CORBA::Octet*)&x;
		return CORBA::OctetSeq (p, p + sizeof (x));
	}

	static TimeBase::TimeT from_timespec (const struct timespec& ts);

};

TimeBase::UtcT POSIX::UTC ()
{
	TimeBase::UtcT ret;
	ret.time (HostAPI::system_clock ());
	return ret;
}

TimeBase::TimeT POSIX::from_timespec (const struct timespec& ts)
{
	return ts.tv_sec * 10000000 + ts.tv_nsec / 100;
}

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <POSIX> the_posix = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (POSIX, Test::POSIX) };

}
