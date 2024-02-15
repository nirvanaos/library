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
#ifndef _SYS_TYPES_H_
#define _SYS_TYPES_H_
#pragma once

#include <stdint.h>
#include <stddef.h> // ptrdiff_t

typedef uint64_t  blkcnt_t;
typedef uint32_t  blksize_t;
typedef int64_t   clock_t;
typedef uint32_t  dev_t;
typedef uint32_t  gid_t;
typedef uint64_t  ino_t;
typedef uint16_t  mode_t;
typedef uint32_t  nlink_t;
typedef int64_t   off_t;
typedef ptrdiff_t ssize_t;
typedef int64_t   time_t;
typedef uint32_t  uid_t;
typedef struct pthread_cond* pthread_cond_t;
typedef struct pthread* pthread_t;
typedef struct pthread_mutex* pthread_mutex_t;

typedef struct
{
  int detached; 
} pthread_attr_t;

typedef struct
{
  unsigned type;
} pthread_mutexattr_t;

typedef unsigned int pthread_key_t;

#define _INO_T_DEFINED

#endif
