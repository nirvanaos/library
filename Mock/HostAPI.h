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
#ifndef NIRVANA_MOCK_HOSTAPI_H_
#define NIRVANA_MOCK_HOSTAPI_H_
#pragma once

#include "export.h"
#include <stdint.h>
#include <time.h>

NIRVANA_MOCK_EXPORT void host_print_error (const char* s);
NIRVANA_MOCK_EXPORT void host_debug_break ();
NIRVANA_MOCK_EXPORT void host_atexit (void (*func)(void));
NIRVANA_MOCK_EXPORT void* host_allocate (size_t size, size_t align);
NIRVANA_MOCK_EXPORT void host_release (void* p);
NIRVANA_MOCK_EXPORT uint64_t host_system_clock ();
NIRVANA_MOCK_EXPORT int host_time_zone_offset ();
NIRVANA_MOCK_EXPORT uint64_t host_steady_clock ();
NIRVANA_MOCK_EXPORT size_t host_max_path ();
NIRVANA_MOCK_EXPORT char* host_getcwd (char* buf, size_t size);
NIRVANA_MOCK_EXPORT int host_chdir (const char* dir);
NIRVANA_MOCK_EXPORT int host_open (const char* path, unsigned oflag, unsigned mode, int& fildesc);
NIRVANA_MOCK_EXPORT int host_close (int fildesc);
NIRVANA_MOCK_EXPORT int host_read (int fildesc, void* p, size_t size, size_t& readed);
NIRVANA_MOCK_EXPORT int host_write (int fildesc, const void* p, size_t size);
NIRVANA_MOCK_EXPORT int host_seek (int fildesc, int64_t off, int whence, uint64_t& pos);
NIRVANA_MOCK_EXPORT int host_dup2 (int src, int dst);
NIRVANA_MOCK_EXPORT bool host_isatty (int fildesc);
NIRVANA_MOCK_EXPORT void host_sleep (uint64_t period100ns);
NIRVANA_MOCK_EXPORT void host_yield ();
NIRVANA_MOCK_EXPORT int host_unlink (const char* path);
NIRVANA_MOCK_EXPORT int host_rmdir (const char* path);
NIRVANA_MOCK_EXPORT int host_mkdir (const char* path, unsigned mode);
NIRVANA_MOCK_EXPORT int* host_error_number ();

struct host_Stat
{
	uint32_t dev;
	uint32_t mode;
	uint32_t nlink;
	uint32_t uid;
	uint32_t gid;
	uint32_t blksize;
	uint64_t ino;
	uint64_t size;
	uint64_t blocks;
	struct timespec atim;
	struct timespec mtim;
	struct timespec ctim;
};

NIRVANA_MOCK_EXPORT int host_stat (const char* path, host_Stat& st);
NIRVANA_MOCK_EXPORT int host_fstat (int, host_Stat& st);
NIRVANA_MOCK_EXPORT int host_rename (const char* oldname, const char* newname);

NIRVANA_MOCK_EXPORT int host_CS_alloc (void (*deleter) (void*), uint16_t& key);
NIRVANA_MOCK_EXPORT void host_CS_free (unsigned key);
NIRVANA_MOCK_EXPORT int host_CS_set (unsigned key, void* p);
NIRVANA_MOCK_EXPORT void* host_CS_get (unsigned key);
NIRVANA_MOCK_EXPORT unsigned host_hardware_concurrency ();

NIRVANA_MOCK_EXPORT void host_raise (int signal);

typedef void* host_OnceControl;

NIRVANA_MOCK_EXPORT void host_once (host_OnceControl& control, void (*init_routine)(void));

NIRVANA_MOCK_EXPORT const char* host_locale ();

#endif
