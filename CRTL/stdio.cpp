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
#include "pch/pch.h"
#include <stdio.h>
#include <unistd.h>
#include <fnctl.h>
#include <errno.h>
#include <sys/stat.h>

extern "C" int fclose (FILE * f)
{
	return close (__file2fd (f));
}

extern "C" int fflush (FILE * f)
{
	return fsync (__file2fd (f));
}

extern "C" FILE * fopen (const char* file, const char* mode)
{
	unsigned flags;

	switch (mode [0]) {
	case 'r': // open for reading
		flags = O_RDONLY;
		break;

	case 'w':  // open for writing
		flags = O_WRONLY | O_CREAT | O_TRUNC;
		break;

	case 'a':  // open for appending
		flags = O_WRONLY | O_CREAT | O_APPEND;
		break;

	default:  // illegal mode
		errno = EINVAL;
		return nullptr;
	}

	char c;
	while ((c = *++mode)) {
		switch (c) {
		case '+':
			flags = (flags & ~O_ACCMODE) | O_RDWR;
			break;

		case 'b':
			break;

		case 't':
			flags |= O_TEXT;
			break;

		case 'x':
			flags |= O_EXCL;
			break;

		default:
			break;
		}
	}

	return __fd2file (open (file, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
}

extern "C" int fputc (int c, FILE * f)
{
	unsigned char u = (unsigned char)c;
	if (write (__file2fd (f), &u, 1) == 1)
		return c;
	else
		return EOF;
}

extern "C" int fputs (const char* s, FILE * f)
{
	size_t cc = strlen (s);
	if (write (__file2fd (f), s, cc) == cc)
		return 1;
	else
		return EOF;
}

extern "C" size_t fread (void* buffer, size_t size, size_t count, FILE * f)
{
	return read (__file2fd (f), buffer, size * count) / size;
}

extern "C" size_t fwrite (const void* buffer, size_t size, size_t count, FILE * f)
{
	return write (__file2fd (f), buffer, size * count) / size;
}

extern "C" int fseek (FILE * f, long offset, int origin)
{
	return lseek (__file2fd (f), offset, origin) >= 0 ? 0 : -1;
}

extern "C" off_t ftello (FILE * f)
{
	return lseek (__file2fd (f), 0, SEEK_CUR);
}

extern "C" long ftell (FILE * f)
{
	return (long)lseek (__file2fd (f), 0, SEEK_CUR);
}

extern "C" int remove (const char* path)
{
	return unlink (path);
}

extern "C" int fgetc (FILE * f)
{
	char c;
	if (read (__file2fd (f), &c, 1) == 1)
		return c;
	else
		return EOF;
}

extern "C" int ungetc (int c, FILE * f)
{
	int err = EIO;
	try {
		Nirvana::the_system->ungetc (__file2fd (f), c);
		return c;
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_system->error_number () = err;
	return EOF;
}

extern "C" int fgetpos (FILE * f, fpos_t * pos)
{
	off_t off = lseek (__file2fd (f), 0, SEEK_CUR);
	if (off < 0)
		return -1;
	else {
		*pos = off;
		return 0;
	}
}

extern "C" int fsetpos (FILE * f, const fpos_t * pos)
{
	if (lseek (__file2fd (f), *pos, SEEK_SET) < 0)
		return -1;
	else
		return 0;
}

extern "C" int ferror (FILE * f)
{
	int err = EIO;
	try {
		return Nirvana::the_system->ferror (__file2fd (f));
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_system->error_number () = err;
	return -1;
}

extern "C" int feof (FILE * f)
{
	int err = EIO;
	try {
		return Nirvana::the_system->feof (__file2fd (f));
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_system->error_number () = err;
	return 0;
}

extern "C" void clearerr (FILE * f)
{
	int err = EIO;
	try {
		Nirvana::the_system->clearerr (__file2fd (f));
	} catch (const CORBA::NO_MEMORY&) {
		err = ENOMEM;
	} catch (const CORBA::SystemException& ex) {
		int e = Nirvana::get_minor_errno (ex.minor ());
		if (e)
			err = e;
	} catch (...) {
	}
	*(int*)Nirvana::the_system->error_number () = err;
}

extern "C" void rewind (FILE * f)
{
	fseek (f, 0, SEEK_SET);
}
