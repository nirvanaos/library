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

typedef struct __FILE
{
	/* Buffer for I/O operations. */
	/* We reserve a few extra bytes for ungetc operations. This means */
	/* that buffer_ptr will point a few bytes *into* the allocation. */
	char* buffer_ptr_;

	/* Number of bytes the buffer can hold. */
	size_t buffer_size_;

	/* Current offset inside the buffer. */
	size_t offset_;

	/* Position inside the buffer that matches the current file pointer. */
	size_t io_offset_;

	/* Valid region of the buffer. */
	size_t valid_limit_;

	/* Begin and end of the dirty region inside the buffer. */
	size_t dirty_begin_;
	size_t dirty_end_;

	/* This points to the same place as buffer_ptr_, or a few bytes earlier */
	/* if there are bytes pushed by ungetc. If buffering is disabled, calls */
	/* to ungetc will trigger an allocation. */
	char* unget_ptr_;

	/* 0 if we are currently reading from the buffer. */
	/* 1 if we are currently writing to the buffer. */
	/* This is only really important for pipe-like streams. */
	int io_mode_;

	/* EOF and error bits. */
	int status_bits_;
}
FILE;

typedef uint64_t fpos_t;
typedef void* locale_t;

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

int remove (const char*);
int rename (const char*, const char*);
FILE* tmpfile (void);
char* tmpnam (char* s);
int fclose (FILE*);
FILE *fdopen(int, const char *);
int fflush (FILE*);
int fileno (FILE*);
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
int sscanf_l (const char* restrict, locale_t, const char* restrict, ...);
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

FILE* __get_std_stream (int i);

#ifdef __cplusplus
#undef restrict
}
#endif

#ifdef _LIBCPP_MSVCRT_LIKE

#define _sscanf_l(buf, fmt, loc, ...) sscanf_l (buf, loc, fmt, __VA_ARGS__)

#endif

#define stdin __get_std_stream (1)
#define stdout __get_std_stream (2)
#define stderr __get_std_stream (3)

#endif
