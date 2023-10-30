#ifndef _SYS_STAT_H_
#define _SYS_STAT_H_
#pragma once

#include "types.h"
#include <time.h>

struct _stat
{
	ino_t st_ino;
	dev_t st_dev;
	mode_t st_mode;
	nlink_t st_nlink;
	off_t st_size;
	time_t st_atime;
	time_t st_mtime;
	time_t st_ctime;
	blkcnt_t blkcnt;
	blksize_t blksize;
};

#endif
