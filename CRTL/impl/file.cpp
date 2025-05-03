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

#include "../pch/pch.h"
#include "file.h"
#include <Nirvana/posix_defs.h>

namespace CRTL {

int File::parse_modestring (const char* mode)
{
	// Consume the first char; this must be 'r', 'w' or 'a'.
	int flags = 0;
	bool has_plus = strchr (mode, '+');
	if (*mode == 'r') {
		if (has_plus) {
			flags = O_RDWR;
		} else {
			flags = O_RDONLY;
		}
	} else if (*mode == 'w') {
		if (has_plus) {
			flags = O_RDWR;
		} else {
			flags = O_WRONLY;
		}
		flags |= O_CREAT | O_TRUNC;
	} else if (*mode == 'a') {
		if (has_plus) {
			flags = O_APPEND | O_RDWR;
		} else {
			flags = O_APPEND | O_WRONLY;
		}
		flags |= O_CREAT;
	}
	mode += 1;

	// Consume additional flags.
	while (*mode) {
		if (*mode == '+') {
			mode++; // This is already handled above.
		} else if (*mode == 'b') {
			mode++;
		} else if (*mode == 't') {
			flags |= O_TEXT;
			mode++;
		} else if (*mode == 'x') {
			flags |= O_EXCL;
			mode++;
		}
	}

	return flags;
}

File::~File ()
{
	assert (dirty_begin_ == dirty_end_);
	deallocate_buffer ();
}

int File::reopen (const char* path, const char* mode) {
	int mode_flags = parse_modestring (mode);

	int fd;
	if (int e = open (path, mode_flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, fd))
		return e;

	flush ();
	close ();
	deallocate_buffer ();
	buffer_ptr_ = nullptr;
	unget_ptr_ = nullptr;
	buffer_size_ = DEFAULT_BUFFER_SIZE;
	reset ();
	fd_ = fd;

	if (mode_flags & O_APPEND) {
		seek (0, SEEK_END);
	}

	return 0;
}

void File::deallocate_buffer ()
{
	if (buffer_ptr_)
		Nirvana::the_memory->release (buffer_ptr_ - UNGET_BUFFER_SIZE, buffer_size_ + UNGET_BUFFER_SIZE);
}

// Note that read() and write() are asymmetric:
// While read() can trigger a write-back, write() can never trigger a read-ahead().
// This peculiarity is reflected in their code.

int File::read (char* buffer, size_t max_size, size_t& actual_size) {
	assert (max_size);

	size_t unget_length = 0;
	if (unget_ptr_ != buffer_ptr_) {
		unget_length = std::min (max_size, (size_t)(buffer_ptr_ - unget_ptr_));
		memcpy (buffer, unget_ptr_, unget_length);

		unget_ptr_ += unget_length;
		buffer += unget_length;
		max_size -= unget_length;

		if (max_size == 0) {
			actual_size = unget_length;
			return 0;
		}
	}

	if (buffer_mode_ == BufferMode::no_buffer) {
		size_t io_size;
		if (int e = io_read (buffer, max_size, io_size)) {
			__status_bits |= __MLIBC_ERROR_BIT;
			return e;
		}
		if (!io_size)
			__status_bits |= __MLIBC_EOF_BIT;
		*actual_size = io_size + unget_length;
		return 0;
	}

	// Ensure correct buffer type for pipe-like streams.
	// TODO: In order to support pipe-like streams we need to write-back the buffer.
	if (__io_mode && __valid_limit)
		mlibc::panicLogger () << "mlibc: Cannot read-write to same pipe-like stream"
		<< frg::endlog;
	__io_mode = 0;

	// Clear the buffer, then buffer new data.
	if (__offset == __valid_limit) {
		// TODO: We only have to write-back/reset if __valid_limit reaches the buffer end.
		if (int e = _write_back (); e)
			return e;
		if (int e = _reset (); e)
			return e;

		// Perform a read-ahead.
		_ensure_allocation ();
		size_t io_size;
		if (int e = io_read (__buffer_ptr, __buffer_size, &io_size); e) {
			__status_bits |= __MLIBC_ERROR_BIT;
			return e;
		}
		if (!io_size) {
			__status_bits |= __MLIBC_EOF_BIT;
			*actual_size = 0;
			return 0;
		}

		__io_offset = io_size;
		__valid_limit = io_size;
	}

	// Return data from the buffer.
	__ensure (__offset < __valid_limit);

	auto chunk = frg::min (size_t (__valid_limit - __offset), max_size);
	memcpy (buffer, __buffer_ptr + __offset, chunk);
	__offset += chunk;

	*actual_size = chunk + unget_length;
	return 0;
}

int File::write (const char* buffer, size_t max_size, size_t* actual_size) {
	assert (max_size);

	if (_init_bufmode ())
		return -1;
	if (globallyDisableBuffering || _bufmode == buffer_mode::no_buffer) {
		// As we do not buffer, nothing can be dirty.
		__ensure (__dirty_begin == __dirty_end);
		size_t io_size;
		if (int e = io_write (buffer, max_size, &io_size); e) {
			__status_bits |= __MLIBC_ERROR_BIT;
			return e;
		}
		*actual_size = io_size;
		return 0;
	}

	// Flush the buffer if necessary.
	if (__offset == __buffer_size) {
		if (int e = _write_back (); e)
			return e;
		if (int e = _reset (); e)
			return e;
	}

	// Ensure correct buffer type for pipe-like streams.
	// TODO: We could full support pipe-like files
	// by ungetc()ing all data before a write happens,
	// however, for now we just report an error.
	if (!__io_mode && __valid_limit) // TODO: Only check this for pipe-like streams.
		mlibc::panicLogger () << "mlibc: Cannot read-write to same pipe-like stream"
		<< frg::endlog;
	__io_mode = 1;

	__ensure (__offset < __buffer_size);
	auto chunk = frg::min (__buffer_size - __offset, max_size);

	// Line-buffered streams perform I/O on full lines.
	bool flush_line = false;
	if (_bufmode == buffer_mode::line_buffer) {
		auto nl = reinterpret_cast<char*> (memchr (buffer, '\n', chunk));
		if (nl) {
			chunk = nl + 1 - buffer;
			flush_line = true;
		}
	}
	__ensure (chunk);

	// Buffer data (without necessarily performing I/O).
	_ensure_allocation ();
	memcpy (__buffer_ptr + __offset, buffer, chunk);

	if (__dirty_begin != __dirty_end) {
		__dirty_begin = frg::min (__dirty_begin, __offset);
		__dirty_end = frg::max (__dirty_end, __offset + chunk);
	} else {
		__dirty_begin = __offset;
		__dirty_end = __offset + chunk;
	}
	__valid_limit = frg::max (__offset + chunk, __valid_limit);
	__offset += chunk;

	// Flush line-buffered streams.
	if (flush_line) {
		if (_write_back ())
			return -1;
	}

	*actual_size = chunk;
	return 0;
}

int File::unget (char c) {
	if (!__unget_ptr) {
		// This can happen if the file is unbuffered, but we still need
		// a space to store ungetc'd data.
		__ensure (!__buffer_ptr);
		_ensure_allocation ();
		__ensure (__unget_ptr);
	}

	if ((size_t)(__buffer_ptr - __unget_ptr) + 1 > ungetBufferSize)
		return EOF;
	else {
		*(--__unget_ptr) = c;
		return c;
	}
}

}
