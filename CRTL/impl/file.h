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

#ifndef CRTL_IMPL_FILE_H_
#define CRTL_IMPL_FILE_H_
#pragma once

#include <stdio.h>

namespace CRTL {

class File {
public:
  enum class StreamType {
    unknown,
    file_like,
    pipe_like
  };

  enum class BufferMode {
    unknown,
    no_buffer,
    line_buffer,
    full_buffer
  };

  File (const File &) = delete;
  File &operator = (const File&) = delete;

private:
  int io_read (char *buffer, size_t max_size, size_t& actual_size) const;
	int io_write (const char *buffer, size_t max_size) const;
	int io_seek (off_t offset, int whence, off_t& pos) const;

private:
	/* Buffer for I/O operations. */
	/* We reserve a few extra bytes for ungetc operations. This means */
	/* that buffer_ptr_ will point a few bytes *into* the allocation. */
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

	StreamType type_;
	BufferMode bufmode_;

	// Underlying file descriptor.
	int fd_;
};

extern File *stdin;
extern File *stdout;
extern File *stderr;

} // namespace CRTL

#endif
