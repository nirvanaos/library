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
#ifndef _STDIO_H_
#define _STDIO_H_
#pragma once

#include "sys/types.h"
#include <stdarg.h>

#ifndef _FILE_DEFINED
typedef struct _FILE
{
	int dummy;
} FILE;
#define _FILE_DEFINED
#endif

typedef uint64_t fpos_t;

#define BUFSIZ 512
#define EOF (-1)

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define FILENAME_MAX 260
#define FOPEN_MAX 32767
#define TMP_MAX 32767

#define _IOFBF 0x0000
#define _IOLBF 0x0040
#define _IONBF 0x0004

#ifndef NULL
#ifdef __cplusplus
#define NULL 0
#else
#define NULL ((void *)0)
#endif
#endif

FILE* const stdin = (FILE*)1;
FILE* const stdout = (FILE*)2;
FILE* const stderr = (FILE*)3;

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

int asprintf (char** restrict, const char* restrict, ...);
int remove (const char*);
int rename (const char*, const char*);
FILE* tmpfile (void);
char* tmpnam (char* s);
int fclose (FILE*);
FILE *fdopen(int, const char *);
int fflush (FILE*);
int fileno (FILE* f);
FILE* fopen (const char* restrict, const char* restrict);
FILE* freopen (const char* restrict, const char* restrict, FILE* restrict);
void setbuf (FILE* restrict, char* restrict);
int setvbuf (FILE* restrict, char* restrict, int, size_t);
int fprintf (FILE* restrict, const char* restrict, ...);
int fscanf (FILE* restrict, const char* restrict, ...);
int printf (const char* restrict, ...);
int scanf (const char* restrict, ...);
int snprintf (char* restrict, size_t, const char* restrict, ...);
int sprintf (char* restrict, const char* restrict, ...);
int sprintf_s (char* restrict, rsize_t, const char* restrict, ...);
int sscanf (const char* restrict, const char* restrict, ...);
int vasprintf (char** restrict, const char* restrict, va_list);
int vfprintf (FILE* restrict, const char* restrict, va_list);
int vfscanf (FILE* restrict, const char* restrict, va_list);
int vprintf (const char* restrict, va_list);
int vscanf (const char* restrict, va_list);
int vsnprintf (char* restrict, size_t, const char* restrict, va_list);
int vsprintf (char* restrict, const char* restrict, va_list);
int vsscanf (const char* restrict, const char* restrict, va_list);
int fgetc (FILE*);
char* fgets (char* restrict, int, FILE* restrict);
int fputc (int, FILE*);
int fputs (const char* restrict, FILE* restrict);
int getc (FILE*);
int getchar (void);
char* gets (char*);  // removed in C++14
int putc (int, FILE*);
int putchar (int);
int puts (const char*);
int ungetc (int, FILE*);
size_t fread (void* restrict, size_t, size_t, FILE* restrict);
size_t fwrite (const void* restrict, size_t, size_t, FILE* restrict);
int fgetpos (FILE* restrict, fpos_t* restrict);
int fseek (FILE*, long, int);
int fseeko (FILE*, off_t, int);
int fsetpos (FILE*, const fpos_t*);
long ftell (FILE*);
off_t ftello (FILE*);
void rewind (FILE*);
void clearerr (FILE*);
int feof (FILE*);
int ferror (FILE*);
void perror (const char*);

#ifdef _MSC_VER
#define _scprintf(format, ...) sprintf_s (nullptr, 0, format, __VA_ARGS__)
#endif

inline int ferror_unlocked (FILE* f)
{
	return ferror (f);
}

inline void flockfile (FILE* f)
{}

inline void funlockfile (FILE* f)
{}

inline size_t fwrite_unlocked(const void *ptr, size_t size, size_t nmemb,
                                   FILE *f)
{
	return fwrite (ptr, size, nmemb, f);
}

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
