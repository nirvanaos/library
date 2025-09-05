/*
* Nirvana C runtime library.
*
* This is a part of the Nirvana project.
*
* Author: Igor Popov
*
* Copyright (c) 2025 Igor Popov.
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

#include <stddef.h>
#include <stdarg.h>
#include "sys/types.h"
#include "errno.h" // Required by the libcxx

typedef struct __FILE FILE;

typedef uint64_t fpos_t;
typedef struct __Locale* locale_t;

#define BUFSIZ 512
#define EOF (-1)

#define SEEK_CUR 1
#define SEEK_END 2
#define SEEK_SET 0

#define FILENAME_MAX 260
#define FOPEN_MAX 32767
#define TMP_MAX 32767

#define _IONBF 1
#define _IOLBF 2
#define _IOFBF 3

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

void clearerr (FILE*);
int fclose (FILE*);
FILE *fdopen(int, const char *);
int feof (FILE*);
int ferror (FILE*);
int fflush (FILE*);
int fgetc (FILE*);
int fgetpos (FILE* restrict, fpos_t* restrict);
char* fgets (char* restrict, int, FILE* restrict);
int fileno (FILE*);
FILE* fopen (const char* restrict, const char* restrict);
int fputc (int, FILE*);
int fputs (const char* restrict, FILE* restrict);
int fprintf (FILE* restrict, const char* restrict, ...);
size_t fread (void* restrict, size_t, size_t, FILE* restrict);
FILE* freopen (const char* restrict, const char* restrict, FILE* restrict);
int fscanf (FILE* restrict, const char* restrict, ...);
int fseek (FILE*, long, int);
int fseeko (FILE*, off_t, int);
int fsetpos (FILE*, const fpos_t*);
long ftell (FILE*);
off_t ftello (FILE*);
size_t fwrite (const void* restrict, size_t, size_t, FILE* restrict);


inline int getc (FILE* f)
{
	return fgetc (f);
}

int getchar (void);
char* gets (char*);  // removed in C++14
void perror (const char*);
int printf (const char* restrict, ...);
int putc (int, FILE*);
int putchar (int);
int puts (const char*);
int remove (const char*);
int rename (const char*, const char*);
void rewind (FILE*);
int scanf (const char* restrict, ...);
void setbuf (FILE* restrict, char* restrict);
int setvbuf (FILE* restrict, char* restrict, int, size_t);
int snprintf (char* restrict, size_t, const char* restrict, ...);
int snprintf_s (char* buffer, size_t bufsiz, size_t count, const char* fmt, ...);
int sscanf (const char* restrict, const char* restrict, ...);
int sscanf_l (const char* restrict, locale_t, const char* restrict, ...);
int sprintf (char* restrict, const char* restrict, ...);
int sprintf_s (char* restrict, rsize_t, const char* restrict, ...);
FILE* tmpfile (void);
char* tmpnam (char* s);
int vasprintf (char** restrict, const char* restrict, va_list);
int vfprintf (FILE* restrict, const char* restrict, va_list);
int vfscanf (FILE* restrict, const char* restrict, va_list);
int vprintf (const char* restrict, va_list);
int vscanf (const char* restrict, va_list);
int vsnprintf (char* restrict, size_t, const char* restrict, va_list);
int vsprintf (char* restrict, const char* restrict, va_list);
int vsscanf (const char* restrict, const char* restrict, va_list);
int ungetc (int, FILE*);

#ifdef _MSC_VER
#define _scprintf(format, ...) sprintf_s (nullptr, 0, format, __VA_ARGS__)
#endif

#ifdef __cplusplus
#undef restrict
}
#endif

#ifdef _LIBCPP_MSVCRT_LIKE

#define _sscanf_l(buf, fmt, loc, ...) sscanf_l (buf, loc, fmt, __VA_ARGS__)

#endif

#define stdin ((FILE*)1)
#define stdout ((FILE*)2)
#define stderr ((FILE*)3)

#endif
