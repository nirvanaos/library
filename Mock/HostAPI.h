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

namespace Nirvana {
namespace Test {
namespace HostAPI {

NIRVANA_MOCK_EXPORT void print_error (const char* s);
NIRVANA_MOCK_EXPORT void debug_break ();
NIRVANA_MOCK_EXPORT void atexit (void (*func)(void));
NIRVANA_MOCK_EXPORT void* allocate (size_t size, size_t align);
NIRVANA_MOCK_EXPORT void release (void* p);
NIRVANA_MOCK_EXPORT uint64_t system_clock ();
NIRVANA_MOCK_EXPORT int time_zone_offset ();
NIRVANA_MOCK_EXPORT uint64_t steady_clock ();
NIRVANA_MOCK_EXPORT size_t max_path ();
NIRVANA_MOCK_EXPORT char* getcwd (char* buf, size_t size);
NIRVANA_MOCK_EXPORT int chdir (const char* dir);
NIRVANA_MOCK_EXPORT int open (const char* path, unsigned oflag, unsigned mode, int& fildesc);
NIRVANA_MOCK_EXPORT int close (int fildesc);
NIRVANA_MOCK_EXPORT int read (int fildesc, void* p, size_t size, size_t& readed);
NIRVANA_MOCK_EXPORT int write (int fildesc, const void* p, size_t size);
NIRVANA_MOCK_EXPORT int seek (int fildesc, int64_t off, int whence, uint64_t& pos);
NIRVANA_MOCK_EXPORT int dup2 (int src, int dst);
NIRVANA_MOCK_EXPORT bool isatty (int fildesc);
NIRVANA_MOCK_EXPORT void sleep (uint64_t period100ns);
NIRVANA_MOCK_EXPORT void yield ();
NIRVANA_MOCK_EXPORT int unlink (const char* path);
NIRVANA_MOCK_EXPORT int rmdir (const char* path);
NIRVANA_MOCK_EXPORT int mkdir (const char* path, unsigned mode);
NIRVANA_MOCK_EXPORT int* error_number ();

struct Stat
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

NIRVANA_MOCK_EXPORT int stat (const char* path, Stat& st);
NIRVANA_MOCK_EXPORT int fstat (int, Stat& st);
NIRVANA_MOCK_EXPORT int rename (const char* oldname, const char* newname);

NIRVANA_MOCK_EXPORT int CS_alloc (void (*deleter) (void*), uint16_t& key);
NIRVANA_MOCK_EXPORT void CS_free (unsigned key);
NIRVANA_MOCK_EXPORT int CS_set (unsigned key, void* p);
NIRVANA_MOCK_EXPORT void* CS_get (unsigned key);
NIRVANA_MOCK_EXPORT unsigned hardware_concurrency ();

NIRVANA_MOCK_EXPORT void raise (int signal);

typedef void* OnceControl;

NIRVANA_MOCK_EXPORT void once (OnceControl& control, void (*init_routine)(void));

NIRVANA_MOCK_EXPORT const char* locale ();

}
}
}

#endif
