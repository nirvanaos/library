/*
* Nirvana mock library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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
#include "HostAPI.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <locale.h>
#include <signal.h>

#include "constants.h"

#include "debug-trap/debug-trap.h"

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <io.h>
#include <malloc.h>
#include <direct.h>

#pragma warning(disable : 4996) // POSIX names

#define STDERR_FILENO 2

typedef unsigned long RWSize;
typedef long FileOff;
typedef struct _stat64 HostStat;

#define h_stat _stat64
#define h_fstat _fstat64

#else

#include <unistd.h>
#include <pthread.h>

typedef size_t RWSize;
typedef int64_t FileOff;
typedef struct stat HostStat;

#define h_stat ::stat
#define h_fstat ::fstat

#endif

#define FLAG_CONV(n) { Nirvana::Test::const_##n, n }

struct FlagConv
{
	unsigned n, host;
};

static int errno_from_host (int err)
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
	return Nirvana::Test::const_EINVAL;
}

static const FlagConv modes [] = {
#ifdef _WIN32
	{ Nirvana::Test::const_S_IRUSR | Nirvana::Test::const_S_IRGRP | Nirvana::Test::const_S_IROTH, _S_IREAD },
	{ Nirvana::Test::const_S_IWUSR | Nirvana::Test::const_S_IWGRP | Nirvana::Test::const_S_IWOTH, _S_IWRITE },
	{ Nirvana::Test::const_S_IXUSR | Nirvana::Test::const_S_IXGRP | Nirvana::Test::const_S_IXOTH, _S_IEXEC }
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

static unsigned mode_to_host (unsigned mode)
{
	unsigned ret = 0;
	if (mode) {
		for (const auto& c : modes) {
			if (c.n & mode)
				ret |= c.host;
		}
	}

	return ret;
}

static unsigned mode_from_host (unsigned mode)
{
	unsigned ret = 0;
	if (mode) {
		for (const auto& c : modes) {
			if (c.host & mode)
				ret |= c.n;
		}
	}

	return ret;
}

const char* loc_init = ::setlocale (0, "en_US.UTF8");

NIRVANA_MOCK_EXPORT void host_print_error (const char* s)
{
	write (STDERR_FILENO, s, (unsigned)strlen (s));
}

NIRVANA_MOCK_EXPORT void host_debug_break ()
{
	psnip_trap ();
}

NIRVANA_MOCK_EXPORT void host_atexit (void (*func)(void))
{
	::atexit (func);
}

NIRVANA_MOCK_EXPORT void* host_allocate (size_t size, size_t align)
{
#ifdef _WIN32
	return _aligned_malloc (size, align);
#else
	return aligned_alloc (align, size);
#endif
}

NIRVANA_MOCK_EXPORT void host_release (void* p)
{
#ifdef _WIN32
	_aligned_free (p);
#else
	free (p);
#endif
}

NIRVANA_MOCK_EXPORT uint64_t host_system_clock ()
{
	struct timespec ts;
	timespec_get (&ts, TIME_UTC);
	return ts.tv_sec * 10000000I64 + ts.tv_nsec / 100;
}

NIRVANA_MOCK_EXPORT int host_time_zone_offset ()
{
	time_t rawtime = time (nullptr);
	struct tm lt = *localtime (&rawtime);
	lt.tm_isdst = -1;
	time_t gmt = mktime (&lt);
	return (int)((rawtime - gmt) / 60);
}

NIRVANA_MOCK_EXPORT uint64_t host_steady_clock ()
{
#ifdef _WIN32
	unsigned __int64 t;
	QueryInterruptTimePrecise (&t);
	return t;
#else
	struct timespec* ts;
	clock_gettime (CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * TimeBase::SECOND + ts.tv_nsec / 100;
#endif
}

NIRVANA_MOCK_EXPORT size_t host_max_path ()
{
	return MAX_PATH;
}

NIRVANA_MOCK_EXPORT char* host_getcwd (char* buf, size_t size)
{
#ifdef _WIN32
	return _getcwd (buf, (int)size);
#else
	return ::getcwd (buf, size);
#endif
}

NIRVANA_MOCK_EXPORT int host_chdir (const char* dir)
{
	if (::chdir (dir) != 0)
		return errno_from_host (errno);
	else
		return 0;
}

NIRVANA_MOCK_EXPORT int host_open (const char* path, unsigned oflag, unsigned mode, int& fildesc)
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
#ifdef O_DIRECT
			,FLAG_CONV (O_DIRECT)
#endif
	};

	unsigned host_oflag = 0;
	for (const auto& c : oflags) {
		if (c.n & oflag)
			host_oflag |= c.host;
	}

	int fd = ::open (path, host_oflag, mode_to_host (mode));
	if (fd < 0)
		return errno_from_host (errno);
	
	fildesc = fd;

	return 0;
}

NIRVANA_MOCK_EXPORT int host_close (int fildesc)
{
	if (0 != ::close (fildesc))
		return errno_from_host (errno);
	else
		return 0;
}

NIRVANA_MOCK_EXPORT int host_read (int fildesc, void* p, size_t size, size_t& readed)
{
	auto cb = ::read (fildesc, p, (RWSize)size);
	if (cb < 0)
		return errno_from_host (errno);
	readed = cb;
	return 0;
}

NIRVANA_MOCK_EXPORT int host_write (int fildesc, const void* p, size_t size)
{
	auto cb = ::write (fildesc, p, (RWSize)size);
	if (cb < 0)
		return errno_from_host (errno);
	return 0;
}

NIRVANA_MOCK_EXPORT int host_seek (int fildesc, int64_t off, int whence, uint64_t& pos)
{
	static_assert (SEEK_SET == 0 && SEEK_CUR == 1 && SEEK_END == 2, "lseek constants");

	auto p = lseek (fildesc, (FileOff)off, whence);
	if (p < 0)
		return errno_from_host (errno);
	pos = p;
	return 0;
}

NIRVANA_MOCK_EXPORT int host_dup2 (int src, int dst)
{
	if (::dup2 (src, dst) < 0)
		return errno_from_host (errno);
	return 0;
}

NIRVANA_MOCK_EXPORT bool host_isatty (int fildesc)
{
	return ::isatty (fildesc) != 0;
}

NIRVANA_MOCK_EXPORT void host_sleep (uint64_t period100ns)
{
#ifdef _WIN32
	Sleep ((unsigned)(period100ns / 10000));
#else
	struct timespec t { (time_t)(period100ns / 10000000), (long)((period100ns % 10000000) * 100) };
	nanosleep (&t, nullptr);
#endif
}

NIRVANA_MOCK_EXPORT void host_yield ()
{
#ifdef _WIN32
	SwitchToThread ();
#else
	sched_yield ();
#endif
}

NIRVANA_MOCK_EXPORT int host_unlink (const char* path)
{
	if (0 != ::unlink (path))
		return errno_from_host (errno);
	return 0;
}

NIRVANA_MOCK_EXPORT int host_rmdir (const char* path)
{
	if (0 != ::rmdir (path))
		return errno_from_host (errno);
	return 0;
}

NIRVANA_MOCK_EXPORT int host_mkdir (const char* path, unsigned mode)
{
	if (0 != ::mkdir (path
#ifndef _WIN32
		, mode_to_host (mode)
#endif
	))
		return errno_from_host (errno);
	return 0;
}

static void stat_from_host (const HostStat& hst, host_Stat& st)
{
	st.ino = hst.st_ino;
	st.uid = hst.st_uid;
	st.gid = hst.st_gid;
	st.size = hst.st_size;

#ifdef st_ctime
	st.ctim = hst.st_ctim;
#else
	st.ctim.tv_sec = hst.st_ctime;
	st.ctim.tv_nsec = 0;
#endif

#ifdef st_atime
	st.atim = hst.st_atim;
#else
	st.atim.tv_sec = hst.st_atime;
	st.atim.tv_nsec = 0;
#endif

#ifdef st_mtime
	st.mtim = hst.st_mtim;
#else
	st.mtim.tv_sec = hst.st_mtime;
	st.mtim.tv_nsec = 0;
#endif

	st.dev = hst.st_dev;
	st.nlink = hst.st_nlink;

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

	st.mode = mode | mode_from_host (hst.st_mode);
}

NIRVANA_MOCK_EXPORT int host_stat (const char* path, host_Stat& st)
{
	HostStat hst;
	if (0 != h_stat (path, &hst))
		return errno_from_host (errno);

	stat_from_host (hst, st);

	return 0;
}

NIRVANA_MOCK_EXPORT int host_fstat (int fd, host_Stat& st)
{
	HostStat hst;
	if (0 != h_fstat (fd, &hst))
		return errno_from_host (errno);

	stat_from_host (hst, st);

	return 0;
}

NIRVANA_MOCK_EXPORT int host_rename (const char* oldname, const char* newname)
{
	if (0 != ::rename (oldname, newname))
		return errno_from_host (errno);
	return 0;
}

NIRVANA_MOCK_EXPORT int host_CS_alloc (void (*deleter) (void*), uint16_t& key)
{
#ifdef _WIN32
	// For the x86 platform we don't pass the deleter because of the calling convention mismatch.
	// So the deleter won't be called. This is not critical for the mock code.
	key = (uint16_t)FlsAlloc (
#ifdef _M_IX86
		nullptr
#else
		deleter
#endif
	);
#else
	pthread_key_t ptkey;
	int err = pthread_key_create (&ptkey, deleter);
	if (err)
		return errno_from_host (err);
	key = (uint16_t)key;
#endif
	return 0;
}

NIRVANA_MOCK_EXPORT void host_CS_free (unsigned key)
{
#ifdef _WIN32
	FlsFree (key);
#else
	pthread_key_delete (key);
#endif
}

NIRVANA_MOCK_EXPORT int host_CS_set (unsigned key, void* p)
{
#ifdef _WIN32
	FlsSetValue (key, p);
#else
	int err = pthread_setspecific (key, p);
	if (err)
		return errno_from_host (err);
#endif
	return 0;
}

NIRVANA_MOCK_EXPORT void* host_CS_get (unsigned key)
{
#ifdef _WIN32
	return FlsGetValue (key);
#else
	return pthread_getspecific (key);
#endif
}

NIRVANA_MOCK_EXPORT unsigned host_hardware_concurrency ()
{
#ifdef _WIN32
	SYSTEM_INFO sysinfo;
	GetSystemInfo (&sysinfo);
	return sysinfo.dwNumberOfProcessors;
#else
	return sysconf (_SC_NPROCESSORS_ONLN);
#endif
}

NIRVANA_MOCK_EXPORT const char* host_locale ()
{
	return loc_init;
}

NIRVANA_MOCK_EXPORT void host_raise (int signal)
{
	::raise (signal);
}

NIRVANA_MOCK_EXPORT int* host_error_number ()
{
	thread_local int err;
	return &err;
}

#ifdef _WIN32

BOOL WINAPI init_once (PINIT_ONCE control, PVOID parameter, PVOID* context)
{
	(*(void (*)(void))parameter) ();
	return TRUE;
}

#endif

NIRVANA_MOCK_EXPORT void host_once (host_OnceControl& control, void (*init_routine)(void))
{
#ifdef _WIN32
	static_assert (sizeof (host_OnceControl) == sizeof (INIT_ONCE), "once control");
	InitOnceExecuteOnce ((PINIT_ONCE)&control, init_once, (void*)init_routine, nullptr);
#else
	static_assert (sizeof (host_OnceControl) == sizeof (pthread_once_t), "once control");
	pthread_once ((pthread_once_t*)&control, init_routine);
#endif
}
