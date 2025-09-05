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
#ifndef _UNISTD_H_
#define _UNISTD_H_
#pragma once

#include <stdint.h>
#include <stddef.h>
#include <Nirvana/posix_defs.h>

#define STDIN_FILENO  0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

#ifndef _POSIX_PATH_MAX
#define _POSIX_PATH_MAX 255
#endif

typedef int64_t off_t;
typedef ptrdiff_t ssize_t;

#ifdef __cplusplus
extern "C" {
#endif

int access (const char*, int);
int chdir (const char*);
int close (int);
int dup (int);
int dup2 (int, int);
int fsync (int);
int fdatasync (int);
char* getcwd (char*, size_t);
int isatty (int);
off_t lseek (int, off_t, int);
ssize_t read (int, void*, size_t);
int rmdir (const char*);
unsigned sleep (unsigned);
long sysconf (int);
ssize_t write (int, const void*, size_t);
int unlink (const char *);

#ifdef __cplusplus
}
#endif

#define _SC_NPROCESSORS_ONLN 1

#endif
