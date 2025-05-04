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
#ifndef CRTL_IMPL_FDIO_H_
#define CRTL_IMPL_FDIO_H_
#pragma once

#include <sys/types.h>

namespace CRTL {

int read (int fildes, void* buf, size_t count, ssize_t& readed) noexcept;
int write (int fildes, const void* buf, size_t count) noexcept;
int lseek (int fildes, off_t offset, int whence, off_t& pos) noexcept;
int close (int fildes) noexcept;
int open (const char* path, int oflag, mode_t mode, int& fildes) noexcept;
int isatty (int fildes, bool& atty) noexcept;
int fcntl (int fildes, int cmd, uintptr_t param, int& ret) noexcept;

}

#endif
