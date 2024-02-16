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

#include "types.h"
#include <time.h>

struct stat
{
	ino_t st_ino;
	dev_t st_dev;
	mode_t st_mode;
	nlink_t st_nlink;
	off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	blkcnt_t st_blocks;
	blksize_t st_blksize;
};

#ifdef __cplusplus
extern "C" {
#endif

int stat (const char* path, struct stat* buf);

#ifndef _MSC_BUILD
int mkdir (const char* path, mode_t mode);
#endif

#ifdef __cplusplus
}
#endif

#define S_IFMT  0xF000  /* type of file */
#define S_IFIFO 0x1000  /* fifo */
#define S_IFCHR 0x2000  /* character special */
#define S_IFDIR 0x4000  /* directory */
#define S_IFBLK 0x6000  /* block special */
#define S_IFREG 0x8000  /* regular */
#define S_IFLNK 0xA000  /* symbolic link  */

#define	S_IRWXU 	(S_IRUSR | S_IWUSR | S_IXUSR)
#define		S_IRUSR	0000400	/* read permission, owner */
#define		S_IWUSR	0000200	/* write permission, owner */
#define		S_IXUSR 0000100/* execute/search permission, owner */
#define	S_IRWXG		(S_IRGRP | S_IWGRP | S_IXGRP)
#define		S_IRGRP	0000040	/* read permission, group */
#define		S_IWGRP	0000020	/* write permission, grougroup */
#define		S_IXGRP 0000010/* execute/search permission, group */
#define	S_IRWXO		(S_IROTH | S_IWOTH | S_IXOTH)
#define		S_IROTH	0000004	/* read permission, other */
#define		S_IWOTH	0000002	/* write permission, other */
#define		S_IXOTH 0000001/* execute/search permission, other */

#define	S_ISCHR(m) (((m) & S_IFMT) == S_IFCHR)
#define S_ISDIR(m) (((m) & S_IFMT) == S_IFDIR)
#define	S_ISREG(m) (((m) & S_IFMT) == S_IFREG)

#endif
