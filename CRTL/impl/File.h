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
#include <Nirvana/SimpleList.h>
#include <Nirvana/ObjectMemory.h>

namespace CRTL {

class FileDyn;

class File
{
	static const size_t DEFAULT_BUFFER_SIZE = 4096;

	// The maximum number of characters we permit the user to ungetc.
	static const size_t UNGET_BUFFER_SIZE = 8;

	static const int EOF_BIT = 1;
	static const int ERROR_BIT = 2;

public:
	static File* cast (FILE* stream) noexcept;
	static FileDyn* cast_no_std (FILE* stream) noexcept;
	
	static int is_std_stream (FILE* stream) noexcept
	{
		uintptr_t i = (uintptr_t)stream;
		return (1 <= i && i <= 3) ? (int)i : 0;
	}

	static FILE* cast (File* f) noexcept
	{
		return reinterpret_cast <FILE*> (f);
	}

	static int parse_modestring (const char* mode) noexcept;

	static int open (const char* path, int oflags, int& fd) noexcept
	{
		return CRTL::open (path, oflags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH, fd);
	}

	File (int fd, bool external_descriptor) noexcept;
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
			int fd;
			int e = open (path, mode_flags, fd);
			if (e)
				return e;
			if (!external_descriptor_)
				CRTL::close (fd_);
			fd_ = fd;
			external_descriptor_ = false;
			type_ = StreamType::unknown;
			bufmode_ = BufferMode::unknown;
			status_bits_ = 0;
		} else {
			int ret;
			int e = CRTL::fcntl (fd_, F_SETFL, mode_flags & (O_APPEND | O_TEXT | O_ACCMODE), ret);
			if (e)
				return e;
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
		if (!external_descriptor_) {
			int e1 = CRTL::close (fd_);
			if (!e)
				e = e1;
		}
		return e;
	}

	int read (char* buffer, size_t max_size, size_t& actual_size) noexcept;
	int write (const char* buffer, size_t size) noexcept;
	int unget (int c) noexcept;
	int tell (fpos_t& current_offset) noexcept;
	int seek (off_t offset, int whence) noexcept;
	int flush () noexcept;

	int fd () const noexcept
	{
		return fd_;
	}

	int setbuf (char* buf, int type, size_t size) noexcept
	{
		if (dirty_begin_ != dirty_end_)
			return -1;
		if (type <= 0 || type > _IOFBF)
			return -1;
		if (buf) {
			if (size < BUFSIZ)
				return -1;
			deallocate_buffer ();
			buffer_ptr_ = buf + UNGET_BUFFER_SIZE;
			unget_ptr_ = buffer_ptr_;
			buffer_size_ = size - UNGET_BUFFER_SIZE;
			external_buffer_ = true;
		}
		bufmode_ = (BufferMode)type;
		return 0;
	}

	bool error () const noexcept
	{
		return status_bits_ & ERROR_BIT;
	}

	bool eof () const noexcept
	{
		return status_bits_ & EOF_BIT;
	}

	void clearerr () noexcept
	{
		status_bits_ = 0;
	}

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

	int init_type () noexcept;
	int init_bufmode () noexcept;
	int write_back () noexcept;
	int reset () noexcept;
	int ensure_allocation () noexcept;
	void deallocate_buffer () noexcept;
	int save_pos () noexcept;
	void purge () noexcept;

  int io_read (char *buffer, size_t max_size, size_t& actual_size) const noexcept
	{
		return CRTL::read (fd_, buffer, max_size, reinterpret_cast <ssize_t&> (actual_size));
	}

	int io_write (const char *buffer, size_t max_size) const noexcept
	{
		return CRTL::write (fd_, buffer, max_size);
	}

	int io_seek (off_t offset, int whence, fpos_t& pos) const noexcept
	{
		return CRTL::lseek (fd_, offset, whence, pos);
	}

private:
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
	
	StreamType type_;
	BufferMode bufmode_;

	// Underlying file descriptor.
	int fd_;

	bool external_buffer_;
	bool external_descriptor_;
};

class FileDyn :
	public File,
	public Nirvana::SimpleList <FileDyn>::Element,
	public Nirvana::ObjectMemory
{
public:
	FileDyn (int fd, Nirvana::SimpleList <FileDyn>& list) noexcept :
		File (fd, false)
	{
		list.push_back (*this);
	}

	~FileDyn ()
	{}
};

} // namespace CRTL

#endif
