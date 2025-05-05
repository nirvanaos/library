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
#include "File.h"
#include <algorithm>
#include <Nirvana/posix_defs.h>

namespace CRTL {

int File::parse_modestring (const char* mode) noexcept
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

File::File (int fd) noexcept :
	type_ (StreamType::unknown),
	bufmode_ (BufferMode::unknown),
	fd_ (fd),
	external_buffer_ (false)
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
}

File::~File ()
{
	assert (dirty_begin_ == dirty_end_);
	deallocate_buffer ();
}

int File::init_type () noexcept {
	if (type_ != StreamType::unknown)
		return 0;

	fpos_t offset;
	int e = CRTL::lseek (fd_, 0, SEEK_CUR, offset);
	switch (e) {
		case 0:
			type_ = StreamType::file_like;
			break;
		case ESPIPE:
			type_ = StreamType::pipe_like;
			break;
		default:
			return e;
	}

	return 0;
}

int File::init_bufmode () noexcept {
	if (bufmode_ != BufferMode::unknown)
		return 0;

	int e = init_type ();
	if (e)
		return e;

	if (StreamType::file_like == type_)
		bufmode_ = BufferMode::no_buffer; // Buffering perfomed in AccessBuf
	else {
		bool atty;
		e = CRTL::isatty (fd_, atty);
		if (e)
			return e;
		bufmode_ = atty ? BufferMode::line_buffer : BufferMode::full_buffer;
	}

	return 0;
}

int File::write_back () noexcept
{
	if (dirty_begin_ == dirty_end_)
		return 0;

	int e = init_type ();
	if (e)
		return e;

	// For non-pipe streams, first do a seek to reset the
	// I/O position to zero, then do a write().
	if (type_ == StreamType::file_like) {
		if (io_offset_ != dirty_begin_) {
			assert (dirty_begin_ - io_offset_ > 0);
			fpos_t new_offset;
			if ((e = io_seek (off_t (dirty_begin_) - off_t (io_offset_), SEEK_CUR, new_offset)))
				return e;
			io_offset_ = dirty_begin_;
		}
	} else {
		assert (type_ == StreamType::pipe_like);
		assert (io_offset_ == dirty_begin_);
	}

	// Now, we are in the correct position to write-back everything.
	if (io_offset_ < dirty_end_) {
		if ((e = io_write (buffer_ptr_ + io_offset_, dirty_end_ - io_offset_))) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
		io_offset_ = dirty_begin_ = dirty_end_;
	}

	return 0;
}

int File::reset () noexcept
{
	int e = init_type ();
	if (e)
		return e;

	// For pipe-like files, we must not forget already read data.
	assert (type_ != StreamType::pipe_like || offset_ == valid_limit_);

	assert (dirty_begin_ == dirty_end_);
	offset_ = 0;
	io_offset_ = 0;
	valid_limit_ = 0;

	return 0;
}

int File::ensure_allocation () noexcept
{
	if (buffer_ptr_)
		return 0;

	try {
		size_t cb = buffer_size_ + UNGET_BUFFER_SIZE;
		void* ptr = Nirvana::the_memory->allocate (nullptr, cb, 0);
		buffer_ptr_ = reinterpret_cast <char*> (ptr) + UNGET_BUFFER_SIZE;
		unget_ptr_ = buffer_ptr_;
		buffer_size_ = cb - UNGET_BUFFER_SIZE;
	} catch (...) {
		return ENOMEM;
	}
	return 0;
}

void File::deallocate_buffer () noexcept
{
	if (buffer_ptr_ && !external_buffer_) {
		try {
			Nirvana::the_memory->release (buffer_ptr_ - UNGET_BUFFER_SIZE, buffer_size_ + UNGET_BUFFER_SIZE);
		} catch (...) {}
	}
}

// Note that read() and write() are asymmetric:
// While read() can trigger a write-back, write() can never trigger a read-ahead().
// This peculiarity is reflected in their code.

int File::read (char* buffer, size_t max_size, size_t& actual_size) noexcept
{
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

	if (bufmode_ == BufferMode::no_buffer) {
		size_t io_size;
		int e = io_read (buffer, max_size, io_size);
		if (e) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
		if (!io_size)
			status_bits_ |= EOF_BIT;
		actual_size = io_size + unget_length;
		return 0;
	}

	// Ensure correct buffer type for pipe-like streams.
	// TODO: In order to support pipe-like streams we need to write-back the buffer.
	assert (!(io_mode_ && valid_limit_));
	io_mode_ = 0;

	// Clear the buffer, then buffer new data.
	if (offset_ == valid_limit_) {
		// TODO: We only have to write-back/reset if __valid_limit reaches the buffer end.
		int e = write_back ();
		if (e)
			return e;
		if (e = reset ())
			return e;

		// Perform a read-ahead.
		if ((e = ensure_allocation ()))
			return e;

		size_t io_size;
		if ((e = io_read (buffer_ptr_, buffer_size_, io_size))) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
		if (!io_size) {
			status_bits_ |= EOF_BIT;
			actual_size = 0;
			return 0;
		}

		io_offset_ = io_size;
		valid_limit_ = io_size;
	}

	// Return data from the buffer.
	assert (offset_ < valid_limit_);

	auto chunk = std::min (size_t (valid_limit_ - offset_), max_size);
	memcpy (buffer, buffer_ptr_ + offset_, chunk);
	offset_ += chunk;

	actual_size = chunk + unget_length;
	return 0;
}

int File::write (const char* buffer, size_t size) noexcept
{
	assert (size);

	int e = init_bufmode ();
	if (e)
		return e;

	if (bufmode_ == BufferMode::no_buffer) {
		// As we do not buffer, nothing can be dirty.
		assert (dirty_begin_ == dirty_end_);
		if (e = io_write (buffer, size)) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
		return 0;
	}

	// Flush the buffer if necessary.
	if (offset_ == buffer_size_) {
		if (e = write_back ())
			return e;
		if (e = reset ())
			return e;
	}

	// Ensure correct buffer type for pipe-like streams.
	// TODO: We could full support pipe-like files
	// by ungetc()ing all data before a write happens,
	// however, for now we just report an error.
	assert (!(!io_mode_ && valid_limit_)); // TODO: Only check this for pipe-like streams.
	io_mode_ = 1;

	assert (offset_ < buffer_size_);
	auto chunk = std::min (buffer_size_ - offset_, size);

	// Line-buffered streams perform I/O on full lines.
	bool flush_line = false;
	if (bufmode_ == BufferMode::line_buffer) {
		auto nl = reinterpret_cast<char*> (memchr (buffer, '\n', chunk));
		if (nl) {
			chunk = nl + 1 - buffer;
			flush_line = true;
		}
	}
	assert (chunk);

	// Buffer data (without necessarily performing I/O).
	if (e = ensure_allocation ())
		return e;

	memcpy (buffer_ptr_ + offset_, buffer, chunk);

	if (dirty_begin_ != dirty_end_) {
		dirty_begin_ = std::min (dirty_begin_, offset_);
		dirty_end_ = std::max (dirty_end_, offset_ + chunk);
	} else {
		dirty_begin_ = offset_;
		dirty_end_ = offset_ + chunk;
	}
	valid_limit_ = std::max (offset_ + chunk, valid_limit_);
	offset_ += chunk;

	// Flush line-buffered streams.
	if (flush_line) {
		if (e = write_back ())
			return e;
	}

	return 0;
}

int File::unget (int c) noexcept
{
	if (!unget_ptr_) {
		// This can happen if the file is unbuffered, but we still need
		// a space to store ungetc'd data.
		assert (!buffer_ptr_);
		if (0 != ensure_allocation ())
			return EOF;
		assert (unget_ptr_);
	}

	if ((size_t)(buffer_ptr_ - unget_ptr_) + 1 > UNGET_BUFFER_SIZE)
		return EOF;
	else {
		*(--unget_ptr_) = c;
		return c;
	}
}

int File::flush () noexcept
{
	if (dirty_end_ != dirty_begin_) {
		int e = write_back ();
		if (e)
			return e;
	}

	int e = save_pos ();
	if (e)
		return e;
	purge ();
	return 0;
}

int File::save_pos () noexcept
{
	int e = init_type ();
	if (e)
		return e;
	if (e = init_bufmode ())
		return e;

	if (type_ == StreamType::file_like && bufmode_ != BufferMode::no_buffer) {
		fpos_t new_offset;
		auto seek_offset = (off_t (offset_) - off_t (io_offset_));
		if (e = io_seek (seek_offset, SEEK_CUR, new_offset)) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
		return 0;
	}
	return 0; // nothing to do for the rest
}

void File::purge () noexcept
{
	offset_ = 0;
	io_offset_ = 0;
	valid_limit_ = 0;
	dirty_end_ = dirty_begin_;
	unget_ptr_ = buffer_ptr_;
}

int File::tell (fpos_t& current_offset) noexcept
{
	fpos_t seek_offset;
	int e = io_seek (0, SEEK_CUR, seek_offset);
	if (e)
		return e;

	current_offset = seek_offset
		+ (fpos_t (offset_) - fpos_t (io_offset_))
		+ (fpos_t (unget_ptr_) - fpos_t (buffer_ptr_));

	return 0;
}

int File::seek (off_t offset, int whence) noexcept
{
	int e = write_back ();
	if (e)
		return e;

	fpos_t new_offset;
	if (whence == SEEK_CUR) {
		auto seek_offset = offset + (off_t (offset_) - off_t (io_offset_));
		if (e = io_seek (seek_offset, whence, new_offset)) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
	} else {
		assert (whence == SEEK_SET || whence == SEEK_END);
		if (e = io_seek (offset, whence, new_offset)) {
			status_bits_ |= ERROR_BIT;
			return e;
		}
	}

	// We just forget the current buffer.
	// TODO: If the seek is "small", we can just modify our internal offset.
	purge ();

	return 0;
}

}
