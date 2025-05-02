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
#ifndef _SYS_STAT_H_
#define _SYS_STAT_H_
#pragma once

#include "../time.h"

struct stat
{
	dev_t st_dev;
	ino_t st_ino; // Not a real inode, just a hash of DirItemId
	mode_t st_mode;
	nlink_t st_nlink;
	uid_t st_uid; // Not a real id, just a hash of SecurityId
	gid_t st_gid; // Not a real id, just a hash of SecurityId
	off_t st_size;
	blkcnt_t st_blocks;
	blksize_t st_blksize;
	struct timespec st_atim;
	struct timespec st_mtim;
	struct timespec st_ctim;

#define st_atime  st_atim.tv_sec  /* Backward compatibility */
#define st_mtime  st_mtim.tv_sec
#define st_ctime  st_ctim.tv_sec
};

#ifdef __cplusplus
extern "C" {
#endif

int stat (const char* path, struct stat* buf);

#if !defined (_MSC_VER) || defined (__clang__)
int mkdir (const char* path, mode_t mode);
#endif

#ifdef __cplusplus
}
#endif

#endif
