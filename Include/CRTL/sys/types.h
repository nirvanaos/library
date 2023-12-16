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

#define _INO_T_DEFINED

#endif
