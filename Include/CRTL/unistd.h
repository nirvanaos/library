#ifndef _UNISTD_H_
#define _UNISTD_H_
#pragma once

#include <stdint.h>
#include "sys/types.h"

#ifdef __cplusplus
extern "C" {
#endif

int chdir (const char*);
int close (int fildes);
int dup (int fildes);
int dup2 (int fildes, int);
int fsync (int fildes);
int fdatasync (int fildes);
char* getcwd (char*, size_t);
int isatty (int fildes);
off_t lseek (int, off_t, int);
ssize_t read (int fildes, void* buf, size_t count);
int rmdir (const char* path);
ssize_t write (int fildes, const void* buf, size_t count);
int unlink (const char *);

#ifdef __cplusplus
}
#endif

#endif
