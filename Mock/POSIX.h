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
#ifndef NIRVANA_TEST_POSIX_H_
#define NIRVANA_TEST_POSIX_H_
#pragma once

#include <CORBA/Server.h>
#include <Nirvana/POSIX_s.h>
#include <Nirvana/nls_s.h>
#include <Nirvana/locale_defs.h>
#include <mockhost/HostAPI.h>

namespace Nirvana {
namespace Test {

struct FlagConv
{
	unsigned n, host;
};

class DefaultLocale :
	public IDL::traits <Nirvana::Locale>::ServantStatic <DefaultLocale>
{
public:
	static const char* name () noexcept
	{
		return "C";
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

class POSIX :
	public CORBA::servant_traits <Nirvana::POSIX>::ServantStatic <POSIX>
{
public:
	static TimeBase::UtcT system_clock ()
	{
		TimeBase::UtcT ret = UTC ();
		ret.tdf (host_time_zone_offset ());
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
		return host_steady_clock ();
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
		static const uint64_t r = 10000000;
		return r;
	}

	static int* error_number ()
	{
		return host_error_number ();
	}

	static void raise (int signal)
	{
		host_raise (signal);
	}

	static void sigaction (int signal, const struct sigaction* act, struct sigaction* oldact)
	{
		throw_NO_IMPLEMENT ();
	}

	static IDL::String get_current_dir ()
	{
		IDL::String ret (host_max_path (), 0);
		char* dir = host_getcwd (&*ret.begin (), ret.size ());
		if (!dir)
			throw_NO_MEMORY ();
		ret.resize (strlen (dir));
		return ret;
	}

	static void chdir (const IDL::String& path)
	{
		int err = host_chdir (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static FilDesc open (const IDL::String& path, unsigned oflag, unsigned mode)
	{
		int ret;
		int err = host_open (path.c_str (), oflag, mode, ret);
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
		int err = host_close (fd);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static size_t read (FilDesc fd, void* p, size_t size)
	{
		size_t cb;
		int err = host_read (fd, p, size, cb);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		return cb;
	}

	static void write (FilDesc fd, const void* p, size_t size)
	{
		int err = host_write (fd, p, size);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static bool seek (FilDesc fd, const FileOff& offset, int whence, FileSize& pos)
	{
		int err = host_seek (fd, offset, whence, pos);
		if (err) {
			if (ESPIPE == err)
				return false;
			else
				throw_UNKNOWN (make_minor_errno (err));
		}
		return true;
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
		int err = host_dup2 (src, dst);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static bool isatty (FilDesc fd)
	{
		return host_isatty (fd);
	}

	static void fstat (FilDesc fd, FileStat& st)
	{
		host_Stat hst;
		int err = host_fstat (fd, hst);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		stat_from_host (hst, st);
	}

	static void sleep (TimeBase::TimeT period100ns)
	{
		host_sleep (period100ns);
	}

	static void yield ()
	{
		host_yield ();
	}

	static void unlink (const IDL::String& path)
	{
		int err = host_unlink (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void rmdir (const IDL::String& path)
	{
		int err = host_rmdir (path.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void mkdir (const IDL::String& path, unsigned mode)
	{
		int err = host_mkdir (path.c_str (), mode);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static void stat_from_host (const host_Stat hst, FileStat& st);

	void stat (const IDL::String& path, FileStat& st)
	{
		host_Stat hst;
		int err = host_stat (path.c_str (), hst);
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
		stat_from_host (hst, st);
	}

	static void rename (const IDL::String& oldname, const IDL::String& newname)
	{
		int err = host_rename (oldname.c_str (), newname.c_str ());
		if (err)
			throw_UNKNOWN (make_minor_errno (err));
	}

	static uint32_t hardware_concurrency ()
	{
		return host_hardware_concurrency ();
	}

	static Locale::_ptr_type cur_locale ()
	{
		assert (host_locale ());
		return DefaultLocale::_get_ptr ();
	}

	static void cur_locale (Nirvana::Locale::_ptr_type loc)
	{
	}

	static Nirvana::Locale::_ref_type create_locale (int mask, const IDL::String& name, Nirvana::Locale::_ptr_type base)
	{
		return cur_locale ();
	}

	static void once (Pointer& control, InitFunc init_func)
	{
		host_once (control, init_func);
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
	ret.time (host_system_clock ());
	return ret;
}

TimeBase::TimeT POSIX::from_timespec (const struct timespec& ts)
{
	return ts.tv_sec * 10000000 + ts.tv_nsec / 100;
}

void POSIX::stat_from_host (const host_Stat hst, FileStat& st)
{
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

}
}

#endif
