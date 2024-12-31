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
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "export.h"
#include "constants.h"

#ifdef _WIN32
#pragma warning (disable : 4996)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#define FLAG_CONV(n) { const_##n, n }

namespace Nirvana {
namespace Test {

struct FlagConv
{
	unsigned n, host;
};

class POSIX :
	public CORBA::servant_traits <Nirvana::POSIX>::ServantStatic <POSIX>
{
public:
	static int* error_number ()
	{
		return &errno;
	}

	static void raise (int signal)
	{
		::raise (signal);
	}

	static void sigaction (int signal, const struct sigaction* act, struct sigaction* oldact)
	{
		throw_NO_IMPLEMENT ();
	}

	static void srand (unsigned seed)
	{
		::srand (seed);
	}

	static int rand ()
	{
		return ::rand ();
	}

	static IDL::String get_current_dir ()
	{
		char* dir = getcwd (0, 0);
		if (!dir)
			throw_NO_MEMORY ();
		IDL::String ret (dir);
		free (dir);
		return ret;
	}

	static void chdir (const IDL::String& path)
	{
		if (0 != ::chdir (path.c_str ()))
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static uint16_t open (const IDL::String& path, unsigned oflag, unsigned mode)
	{
		static const FlagConv oflags [] = {
			FLAG_CONV (O_WRONLY),
			FLAG_CONV (O_RDWR),
			FLAG_CONV (O_CREAT),
			FLAG_CONV (O_EXCL),
			FLAG_CONV (O_TRUNC),
			FLAG_CONV (O_APPEND)
#ifdef O_TEXT
			,FLAG_CONV (O_TEXT)
#endif
		};

		unsigned host_oflag = 0;
		for (const auto& c : oflags) {
			if (c.n & oflag)
				host_oflag |= c.host;
		}

		int fd = ::open (path.c_str (), host_oflag, mode_to_host (mode));
		if (fd < 0)
			throw_UNKNOWN (make_minor_errno (errno));

		return (uint16_t)fd;
	}

	static uint16_t mkostemps (CharPtr tpl, unsigned suffix_len, unsigned flags)
	{
		throw_NO_IMPLEMENT ();
	}

	static void close (unsigned fd)
	{
		if (0 != ::close (fd))
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static size_t read (unsigned fd, void* p, size_t size)
	{
		auto cb = ::read (fd, p, size);
		if (cb < 0)
			throw_UNKNOWN (make_minor_errno (errno));
		return cb;
	}

	static void write (unsigned fd, const void* p, size_t size)
	{
		auto cb = ::write (fd, p, size);
		if (cb < 0)
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static FileSize seek (unsigned fd, const FileOff& offset, uint_fast16_t whence)
	{
		auto pos = lseek (fd, offset, whence);
		if (pos < 0)
			throw_UNKNOWN (make_minor_errno (errno));
		return pos;
	}

	static int_fast16_t fcntl (unsigned fd, int_fast16_t cmd, uintptr_t arg)
	{
		throw_NO_IMPLEMENT ();
	}

	static void fsync (unsigned fd)
	{
#ifndef _WIN32
		if (0 != ::fsync (fd))
			throw_UNKNOWN (make_minor_errno (errno));
#endif
	}

	static void dup2 (unsigned src, unsigned dst)
	{
		if (::dup2 (src, dst) < 0)
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static bool isatty (unsigned fd)
	{
		return ::isatty (fd) != 0;
	}

	static void ungetc (unsigned fd, int c)
	{
		throw_NO_IMPLEMENT ();
	}

	static bool ferror (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static bool feof (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static void clearerr (unsigned fd)
	{
		throw_NO_IMPLEMENT ();
	}

	static void sleep (TimeBase::TimeT period100ns)
	{
#ifdef _WIN32
		Sleep ((unsigned)(period100ns / 10000));
#else
		struct timespec t { (time_t)(period100ns / 10000000), (long)((period100ns % 10000000) * 100) };
		nanosleep (&t, nullptr);
#endif
	}

	static void unlink (const IDL::String& path)
	{
		if (0 != ::unlink (path.c_str ()))
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static void rmdir (const IDL::String& path)
	{
		if (0 != ::rmdir (path.c_str ()))
			throw_UNKNOWN (make_minor_errno (errno));
	}

	static void mkdir (const IDL::String& path, unsigned mode)
	{
		if (0 != ::mkdir (path.c_str ()
#ifndef _WIN32
			, mode_to_host (mode)
#endif
		))
			throw_UNKNOWN (make_minor_errno (errno));
	}

	void stat (const IDL::String& path, FileStat& st)
	{
#ifdef _WIN32
#define stat _stat64
#endif
		struct stat hst;
		if (0 != stat (path.c_str (), &hst))
			throw_UNKNOWN (make_minor_errno (errno));
#ifdef _WIN32
#undef stat
#endif

		st.id (make_id (hst.st_ino));
		st.owner (make_id (hst.st_uid));
		st.group (make_id (hst.st_gid));
		st.size (hst.st_size);

#ifdef st_ctime
		st.creation_time ().time (from_timespec (hst.st_ctim));
#else
		st.creation_time ().time (hst.st_ctime / 10000000);
#endif

#ifdef st_atime
		st.last_access_time ().time (from_timespec (hst.st_atim));
#else
		st.last_access_time ().time (hst.st_atime / 10000000);
#endif

#ifdef st_mtime
		st.last_write_time ().time (from_timespec (hst.st_mtim));
#else
		st.last_write_time ().time (hst.st_mtime / 10000000);
#endif

		st.dev (hst.st_dev);
		st.nlink (hst.st_nlink);

		static const FlagConv file_types [] = {
#ifdef S_IFIFO
			FLAG_CONV (S_IFIFO),
#endif
			FLAG_CONV (S_IFCHR),
			FLAG_CONV (S_IFDIR),
#ifdef S_IFBLK
			FLAG_CONV (S_IFBLK),
#endif
			FLAG_CONV (S_IFREG)
#ifdef S_IFLNK
			,FLAG_CONV (S_IFLNK)
#endif
		};

		unsigned mode = 0;
		for (const auto& c : file_types) {
			if (c.n == (hst.st_mode & S_IFMT)) {
				mode = c.host;
				break;
			}
		}

		st.mode (mode | mode_from_host (hst.st_mode));
	}

private:
	static unsigned mode_to_host (unsigned mode);
	static unsigned mode_from_host (unsigned mode);
	static int errno_from_host (int err);

	static uint32_t make_minor_errno (int err)
	{
		return Nirvana::make_minor_errno (errno_from_host (err));
	}

	template <typename T>
	static CORBA::OctetSeq make_id (const T& x)
	{
		const CORBA::Octet* p = (const CORBA::Octet*)&x;
		return CORBA::OctetSeq (p, p + sizeof (x));
	}

	static TimeBase::TimeT from_timespec (const struct timespec& ts);

private:
	static const FlagConv modes_ [];
};

const FlagConv POSIX::modes_ [] = {
#ifdef _WIN32
	{ const_S_IRUSR | const_S_IRGRP | const_S_IROTH, _S_IREAD },
	{ const_S_IWUSR | const_S_IWGRP | const_S_IWOTH, _S_IWRITE },
	{ const_S_IXUSR | const_S_IXGRP | const_S_IXOTH, _S_IEXEC }
#else
	FLAG_CONV (S_IRUSR),
	FLAG_CONV (S_IWUSR),
	FLAG_CONV (S_IXUSR),
	FLAG_CONV (S_IRGRP),
	FLAG_CONV (S_IWGRP),
	FLAG_CONV (S_IXGRP),
	FLAG_CONV (S_IROTH),
	FLAG_CONV (S_IWOTH),
	FLAG_CONV (S_IXOTH)
#endif
};

unsigned POSIX::mode_to_host (unsigned mode)
{
	unsigned ret = 0;
	if (mode) {
		for (const auto& c : modes_) {
			if (c.n & mode)
				ret |= c.host;
		}
	}

	return ret;
}

unsigned POSIX::mode_from_host (unsigned mode)
{
	unsigned ret = 0;
	if (mode) {
		for (const auto& c : modes_) {
			if (c.host & mode)
				ret |= c.n;
		}
	}

	return ret;
}

int POSIX::errno_from_host (int err)
{
	static const FlagConv codes [] = {
		FLAG_CONV (EPERM),
		FLAG_CONV (ENOENT),
		FLAG_CONV (ESRCH),
		FLAG_CONV (EINTR),
		FLAG_CONV (EIO),
		FLAG_CONV (ENXIO),
		FLAG_CONV (E2BIG),
		FLAG_CONV (ENOEXEC),
		FLAG_CONV (EBADF),
		FLAG_CONV (ECHILD),
		FLAG_CONV (EAGAIN),
		FLAG_CONV (ENOMEM),
		FLAG_CONV (EACCES),
		FLAG_CONV (EFAULT),
		FLAG_CONV (EBUSY),
		FLAG_CONV (EEXIST),
		FLAG_CONV (EXDEV),
		FLAG_CONV (ENODEV),
		FLAG_CONV (ENOTDIR),
		FLAG_CONV (EISDIR),
		FLAG_CONV (EINVAL),
		FLAG_CONV (ENFILE),
		FLAG_CONV (EMFILE),
		FLAG_CONV (ENOTTY),
		FLAG_CONV (ETXTBSY),
		FLAG_CONV (EFBIG),
		FLAG_CONV (ENOSPC),
		FLAG_CONV (ESPIPE),
		FLAG_CONV (EROFS),
		FLAG_CONV (EMLINK),
		FLAG_CONV (EPIPE),
		FLAG_CONV (EDOM),
		FLAG_CONV (ERANGE),
		FLAG_CONV (ENOMSG),
		FLAG_CONV (EIDRM),
		FLAG_CONV (EDEADLK),
		FLAG_CONV (ENOLCK),
		FLAG_CONV (ENOSTR),
		FLAG_CONV (ENODATA),
		FLAG_CONV (ETIME),
		FLAG_CONV (ENOSR),
		FLAG_CONV (ENOLINK),
		FLAG_CONV (EPROTO),
#ifdef EMULTIHOP
		FLAG_CONV (EMULTIHOP),
#endif
		FLAG_CONV (EBADMSG),
#ifdef EFTYPE
		FLAG_CONV (EFTYPE),
#endif
		FLAG_CONV (ENOSYS),
		FLAG_CONV (ENOTEMPTY),
		FLAG_CONV (ENAMETOOLONG),
		FLAG_CONV (ELOOP),
		FLAG_CONV (EOPNOTSUPP),
#ifdef EPFNOSUPPORT
		FLAG_CONV (EPFNOSUPPORT),
#endif
		FLAG_CONV (ECONNRESET),
		FLAG_CONV (ENOBUFS),
		FLAG_CONV (EAFNOSUPPORT),
		FLAG_CONV (EPROTOTYPE),
		FLAG_CONV (ENOTSOCK),
		FLAG_CONV (ENOPROTOOPT),
		FLAG_CONV (ECONNREFUSED),
		FLAG_CONV (EADDRINUSE),
		FLAG_CONV (ECONNABORTED),
		FLAG_CONV (ENETUNREACH),
		FLAG_CONV (ENETDOWN),
		FLAG_CONV (ETIMEDOUT),
#ifdef EHOSTDOWN
		FLAG_CONV (EHOSTDOWN),
#endif
		FLAG_CONV (EHOSTUNREACH),
		FLAG_CONV (EINPROGRESS),
		FLAG_CONV (EALREADY),
		FLAG_CONV (EDESTADDRREQ),
		FLAG_CONV (EMSGSIZE),
		FLAG_CONV (EPROTONOSUPPORT),
		FLAG_CONV (EADDRNOTAVAIL),
		FLAG_CONV (ENETRESET),
		FLAG_CONV (EISCONN),
		FLAG_CONV (ENOTCONN),
#ifdef ETOOMANYREFS
		FLAG_CONV (ETOOMANYREFS),
#endif
#ifdef EDQUOT
		FLAG_CONV (EDQUOT),
#endif
#ifdef ESTALE
		FLAG_CONV (ESTALE),
#endif
		FLAG_CONV (ENOTSUP),
		FLAG_CONV (EILSEQ),
		FLAG_CONV (EOVERFLOW),
		FLAG_CONV (ECANCELED),
		FLAG_CONV (ENOTRECOVERABLE),
		FLAG_CONV (EOWNERDEAD),
		FLAG_CONV (EWOULDBLOCK)
	};

	for (const auto& c : codes) {
		if (c.host == err)
			return c.n;
	}
	return const_EINVAL;
}

TimeBase::TimeT POSIX::from_timespec (const struct timespec& ts)
{
	return ts.tv_sec * 10000000 + ts.tv_nsec / 100;
}

NIRVANA_MOCK_EXPORT CORBA::Internal::Interface* mock_posix = NIRVANA_STATIC_BRIDGE (Nirvana::POSIX, POSIX);

}

NIRVANA_SELECTANY extern
NIRVANA_STATIC_IMPORT ImportInterfaceT <POSIX> the_posix = { OLF_IMPORT_INTERFACE,
nullptr, nullptr, NIRVANA_STATIC_BRIDGE (POSIX, Test::POSIX) };

}