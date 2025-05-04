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
#include <Nirvana/posix_defs.h>

namespace CRTL {

class File : public FILE
{
	static const size_t DEFAULT_BUFFER_SIZE = 4096;

	// The maximum number of characters we permit the user to ungetc.
	static const size_t UNGET_BUFFER_SIZE = 8;

	static const int EOF_BIT = 1;
	static const int ERROR_BIT = 2;

public:
	static int open (const char* path, const char* mode, File* obj)
	{
		int fd;
		int e = open (path, parse_modestring (mode), fd);
		if (e)
			return e;
		try {
			obj = new File (fd);
		} catch (...) {
			return ENOMEM;
		}
		return 0;
	}

	File (int fd) noexcept;
	~File ();

  File (const File &) = delete;
  File &operator = (const File&) = delete;

	int reopen (const char* path, const char* mode)
	{
		int mode_flags = parse_modestring (mode);

		flush ();
		deallocate_buffer ();
		buffer_ptr_ = nullptr;
		unget_ptr_ = nullptr;
		buffer_size_ = DEFAULT_BUFFER_SIZE;
		reset ();
		if (path) {
			CRTL::close (fd_);
			fd_ = -1;
			int fd;
			int e = open (path, mode_flags, fd);
			if (e)
				return e;
			fd_ = fd;
		}

		if (mode_flags & O_APPEND) {
			int e = seek (0, SEEK_END);
			if (e)
				return e;
		}

		return 0;
	}

	int close () noexcept
	{
		int e = flush ();
		int e1 = CRTL::close (fd_);
		if (!e)
			e = e1;
		return e;
	}

	int read (char* buffer, size_t max_size, size_t& actual_size) noexcept;
	int write (const char* buffer, size_t size) noexcept;
	int unget (int c) noexcept;
	int tell (off_t& current_offset) noexcept;
	int seek (off_t offset, int whence) noexcept;
	int flush () noexcept;

private:
	enum class StreamType {
		unknown,
		file_like,
		pipe_like
	};

	enum class BufferMode {
		unknown = 0,
		no_buffer = _IONBF,
		line_buffer = _IOLBF,
		full_buffer = _IOFBF
	};

	static int parse_modestring (const char* mode) noexcept;

	int init_type () noexcept;
	int init_bufmode () noexcept;
	int write_back () noexcept;
	int reset () noexcept;
	int ensure_allocation () noexcept;
	void deallocate_buffer () noexcept;
	int save_pos () noexcept;
	void purge () noexcept;

	static int open (const char* path, int oflags, int& fd) noexcept
	{
		return CRTL::open (path, oflags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, fd);
	}

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

private:
	StreamType type_;
	BufferMode bufmode_;

	// Underlying file descriptor.
	int fd_;
	bool external_buffer_;
};

} // namespace CRTL

#endif
