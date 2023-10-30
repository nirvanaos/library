/// \file
/// \brief POSIX subsystem.
/*
* Nirvana runtime library.
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
#ifndef _FNCTL_H_
#define _FNCTL_H_
#pragma once

#include <stdint.h>

const uint_fast16_t O_ACCMODE = 0x0003;
const uint_fast16_t O_RDONLY  = 0;
const uint_fast16_t O_WRONLY  = 0x0001;
const uint_fast16_t O_RDWR    = 0x0002;

/// Create file if it does not exist.
const uint_fast16_t O_CREAT = 0x0004;

/// Don't replace on create.
const uint_fast16_t O_EXCL = 0x0008;

/// Truncate size to 0.
const uint_fast16_t O_TRUNC = 0x0010;

/// Append on each write.
const uint_fast16_t O_APPEND = 0x0020;

/// Non-blocked read where possible.
const uint_fast16_t O_NONBLOCK = 0x0040;

/// Open file for direct access.
const uint_fast16_t O_DIRECT = 0x0080;

/// File mode is text (translated)
const uint_fast16_t O_TEXT = 0x0100;

/// Seek to the end of stream immediately after open
const uint_fast16_t O_ATE = 0x0200;

const uint_fast16_t O_SYNC = 0x400;
const uint_fast16_t O_FSYNC = 0x400;
const uint_fast16_t O_DSYNC = 0x400;
//const uint_fast16_t O_NOCTTY = 0x4000;
//const uint_fast16_t O_TMPFILE = 0x8000;

enum {
	F_DUPFD = 1,
	F_GETFD,
	F_SETFD,
	F_GETFL,
	F_SETFL
};

const uint_fast16_t FD_CLOEXEC = 1;

#ifdef __cplusplus
extern "C" {
#endif

int creat (const char* path, int mode);
int fcntl (int fildes, int cmd, int arg);
//int open (const char* path, int oflag, int mode);

#ifdef __cplusplus
}
#endif

#endif
