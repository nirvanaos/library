/// \file
/// \brief POSIX subsystem definitions.
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
#ifndef NIRVANA_POSIX_DEFS_H_
#define NIRVANA_POSIX_DEFS_H_
#pragma once

#define O_ACCMODE  0x0003
#define O_RDONLY   0
#define O_WRONLY   0x0001
#define O_RDWR     0x0002

/// Create file if it does not exist.
#define O_CREAT    0x0004

/// Don't replace on create.
#define O_EXCL     0x0008

/// Truncate size to 0.
#define O_TRUNC    0x0010

/// Append on each write.
#define O_APPEND   0x0020

/// Non-blocked read where possible.
#define O_NONBLOCK 0x0040

/// Open file for direct access.
#define O_DIRECT   0x0080

/// File mode is text (translated)
#define O_TEXT     0x0100

#define O_SYNC     0x400
#define O_FSYNC    0x400
#define O_DSYNC    0x400
//#define O_NOCTTY 0x4000
//#define O_TMPFILE 0x8000

enum {
	F_DUPFD = 1,
	F_GETFD,
	F_SETFD,
	F_GETFL,
	F_SETFL
};

#define FD_CLOEXEC 1

// File access flags

#define F_OK 1
#define R_OK 2
#define W_OK 4
#define X_OK 8

// Seek method constants

#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2

// Mode flags

#ifndef S_IFMT

#define S_IFMT  0xF000  /* type of file */
#define S_IFIFO 0x1000  /* fifo */
#define S_IFCHR 0x2000  /* character special */
#define S_IFDIR 0x4000  /* directory */
#define S_IFBLK 0x6000  /* block special */
#define S_IFREG 0x8000  /* regular */
#define S_IFLNK 0xA000  /* symbolic link  */

#endif

#define S_IRWXU (S_IRUSR | S_IWUSR | S_IXUSR)
#define S_IRUSR 0x0100 /* read permission, owner */
#define S_IWUSR 0x0080 /* write permission, owner */
#define S_IXUSR 0x0040 /* execute/search permission, owner */
#define S_IRWXG (S_IRGRP | S_IWGRP | S_IXGRP)
#define S_IRGRP 0x0020 /* read permission, group */
#define S_IWGRP 0x0010 /* write permission, grougroup */
#define S_IXGRP 0x0008 /* execute/search permission, group */
#define S_IRWXO (S_IROTH | S_IWOTH | S_IXOTH)
#define S_IROTH 0x0004 /* read permission, other */
#define S_IWOTH 0x0002 /* write permission, other */
#define S_IXOTH 0x0001 /* execute/search permission, other */

#define S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define S_ISREG(m) (((m) & S_IFMT) == S_IFREG)

#endif
