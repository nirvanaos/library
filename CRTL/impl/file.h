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
#include "fdio.h"

namespace CRTL {

class File : public FILE
{
	static const size_t DEFAULT_BUFFER_SIZE = 4096;

	// The maximum number of characters we permit the user to ungetc.
	static const size_t UNGET_BUFFER_SIZE = 8;

public:

  enum class StreamType {
    file_like,
    pipe_like
  };

  enum class BufferMode {
    no_buffer,
    line_buffer,
    full_buffer
  };

	File (int fd, StreamType stream_type, bool force_no_buffer) :
		stream_type_ (stream_type),
		buffer_mode_ (BufferMode::no_buffer),
		fd_ (fd)
	{
		buffer_ptr_ = nullptr;
		unget_ptr_ = nullptr;
		buffer_size_ = DEFAULT_BUFFER_SIZE;
		offset_ = 0;
		io_offset_ = 0;
		valid_limit_ = 0;
		dirty_begin_ = 0;
		dirty_end_ = 0;
		io_mode_ = 0;
		status_bits_ = 0;

		if (!force_no_buffer) {
			bool atty = false;
			if (StreamType::pipe_like == stream_type)
				CRTL::isatty (fd, atty);
			if (atty)
				buffer_mode_ = BufferMode::line_buffer;
			else
				buffer_mode_ = BufferMode::full_buffer;
		}
	}

	~File ();

  File (const File &) = delete;
  File &operator = (const File&) = delete;

	int close () noexcept
	{
		assert (dirty_begin_ == dirty_end_);
		return CRTL::close (fd_);
	}

	int reopen (const char* path, const char* mode);
	int read (char* buffer, size_t max_size, size_t& actual_size);
	int write (const char* buffer, size_t max_size, size_t* actual_size);
	int unget (char c);
	void purge ();
	int flush ();
	int tell (off_t* current_offset);
	int seek (off_t offset, int whence);

private:
	static int parse_modestring (const char* mode) noexcept;

  int io_read (char *buffer, size_t max_size, size_t& actual_size) const noexcept
	{
		return CRTL::read (fd_, buffer, max_size, reinterpret_cast <ssize_t&> (actual_size));
	}

	int io_write (const char *buffer, size_t max_size) const noexcept
	{
		return CRTL::write (fd_, buffer, max_size);
	}

	int io_seek (off_t offset, int whence, off_t& pos) const noexcept
	{
		return CRTL::lseek (fd_, offset, whence, pos);
	}

	void deallocate_buffer ();
	int reset ();

private:
	StreamType stream_type_;
	BufferMode buffer_mode_;

	// Underlying file descriptor.
	int fd_;
};

} // namespace CRTL

#endif
