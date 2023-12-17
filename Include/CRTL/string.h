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
#ifndef _STRING_H_
#define _STRING_H_
#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

void    *memchr (const void *, int, size_t);
int      memcmp (const void *, const void *, size_t);
void    *memcpy (void *restrict, const void *restrict, size_t);
void    *memmove (void *, const void *, size_t);
void    *memset (void *, int, size_t);
char    *strcat (char *restrict, const char *restrict);
char    *strchr (const char *, int);
int      strcmp (const char *, const char *);
int      strcoll (const char *, const char *);
char    *strcpy (char *restrict, const char *restrict);
size_t   strcspn (const char *, const char *);
char    *strerror (int);
size_t   strlen (const char *);
char    *strncat (char *restrict, const char *restrict, size_t);
int      strncmp (const char *, const char *, size_t);
char    *strncpy (char *restrict, const char *restrict, size_t);
char    *strpbrk (const char *, const char *);
char    *strrchr (const char *, int);
size_t   strspn (const char *, const char *);
char    *strstr (const char *, const char *);
char    *strtok (char *restrict, const char *restrict);
size_t   strxfrm (char *restrict, const char *restrict, size_t);

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
