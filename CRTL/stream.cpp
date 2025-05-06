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
#include "pch/pch.h"
#include <stdio.h>
#include <limits>
#include "impl/File.h"

extern "C" FILE* fopen (const char* file, const char* mode)
{
	CRTL::File* f = nullptr;
	int e = CRTL::File::open (file, mode, f);
	if (e)
		errno = e;
	return f;
}

extern "C" int fclose (FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->close ();
	delete f;
	if (e) {
		errno = e;
		return EOF;
	}
	return 0;
}

extern "C" size_t fread (void* buffer, size_t size, size_t count, FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	if (!size || !count)
		return 0;

	// Distinguish two cases here: If the object size is one, we perform byte-wise reads.
	// Otherwise, we try to read each object individually.
	if (size == 1) {
		size_t cb;
		int e = f->read ((char*)buffer, count, cb);
		if (e) {
			errno = e;
			return 0;
		}
		return cb;
	} else {
		for (size_t i = 0; i < count; i++) {
			size_t cb;
			int e = f->read ((char*)buffer + i * size, size, cb);
			if (e) {
				errno = e;
				return i;
			}
			if (cb < size)
				return i;
		}
		return count;
	}
}

extern "C" size_t fwrite (const void* buffer, size_t size, size_t count, FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	if (!size || !count)
		return 0;

	for (size_t i = 0; i < count; i++) {
		int e = f->write ((const char*)buffer + i * size, size);
		if (e) {
			errno = e;
			return i;
		}
	}

	return count;
}

extern "C" int fflush (FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->flush ();
	if (e) {
		errno = e;
		return EOF;
	}
	return 0;
}

extern "C" int fputc (int c, FILE* stream)
{
	char d = c;
	if (fwrite (&d, 1, 1, stream) != 1)
		return EOF;
	return 1;
}

extern "C" int fputs (const char* s, FILE* stream)
{
	size_t cc = strlen (s);
	if (cc && fwrite (s, cc, 1, stream) != 1)
		return EOF;
	return 1;
}

extern "C" int fsetpos (FILE* stream, const fpos_t* pos)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->seek (*pos, SEEK_SET);
	if (e) {
		errno = e;
		return -1;
	}
	return 0;
}

extern "C" int fseek (FILE* stream, long offset, int origin)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->seek (offset, origin);
	if (e) {
		errno = e;
		return -1;
	}
	return 0;
}

extern "C" void rewind (FILE* stream)
{
	fseek (stream, 0, SEEK_SET);
}

extern "C" off_t ftello (FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	fpos_t current_offset;
	int e = f->tell (current_offset);
	off_t ret;
	if (!e) {
		ret = current_offset;
		if (ret < 0)
			e = EOVERFLOW;
	}
	if (e) {
		errno = e;
		return -1;
	}
	return ret;
}

extern "C" int fgetpos (FILE* stream, fpos_t* pos)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->tell (*pos);
	if (e) {
		errno = e;
		return -1;
	}
	return 0;
}

extern "C" long ftell (FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	fpos_t current_offset;
	int e = f->tell (current_offset);
	if (!e) {
		if (current_offset > std::numeric_limits <long>::max ())
			e = EOVERFLOW;
	}
	if (e) {
		errno = e;
		return -1;
	}
	return (long)current_offset;
}

extern "C" int fgetc (FILE* stream)
{
	char c;
	auto bytes_read = fread (&c, 1, 1, stream);
	if (bytes_read != 1)
		return EOF;
	return c;
}

extern "C" int ungetc (int c, FILE* stream)
{
	if (c == EOF)
		return EOF;

	CRTL::File* f = static_cast <CRTL::File*> (stream);
	return f->unget (c);
}

extern "C" int ferror (FILE* stream)
{
	return stream->status_bits_ & CRTL::File::ERROR_BIT;
}

extern "C" int feof (FILE* stream)
{
	return stream->status_bits_ & CRTL::File::EOF_BIT;
}

extern "C" void clearerr (FILE* stream)
{
	stream->status_bits_ = 0;
}

extern "C" int fileno (FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	return f->fd ();
}

extern "C" FILE* freopen (const char* path, const char* mode, FILE* stream)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	int e = f->reopen (path, mode);
	if (e) {
		errno = e;
		return nullptr;
	}
	return f;
}

extern "C" int setvbuf (FILE* stream, char* buf, int type, size_t size)
{
	CRTL::File* f = static_cast <CRTL::File*> (stream);
	return f->setbuf (buf, type, size);
}

extern "C" void setbuf (FILE* stream, char* buf)
{
	setvbuf (stream, buf, buf ? _IOFBF : _IONBF, BUFSIZ);
}
