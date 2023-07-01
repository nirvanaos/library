/// \file
/// \brief Nirvana native types
/*
* Nirvana runtime library.
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
#ifndef NIRVANA_FNCTL_H_
#define NIRVANA_FNCTL_H_
#pragma once

#include <stdint.h>

const uint_fast16_t O_ACCMODE = 0x0003;
const uint_fast16_t O_RDONLY  = 0;
const uint_fast16_t O_WRONLY  = 0x0001;
const uint_fast16_t O_RDWR    = 0x0002;

/// Create file if it does not exist.
const uint_fast16_t O_CREAT = 0x0004;

/// Don't replace on create.
const uint_fast16_t O_EXCL = 0x0008;

const uint_fast16_t O_NOCTTY = 0x0010;

/// Truncate flag.
const uint_fast16_t O_TRUNC = 0x0020;

const uint_fast16_t O_APPEND = 0x0040;

const uint_fast16_t O_NONBLOCK = 0x0080; // Unused?

const uint_fast16_t O_DSYNC = 0x0100;

const uint_fast16_t O_DIRECT = 0x0200;

/// File mode is text (translated)
const uint_fast16_t O_TEXT = 0x0400;

/// Seek to the end of stream immediately after open
const uint_fast16_t O_ATE = 0x0800;

const uint_fast16_t FILE_SHARE_DENY_READ = 0x1000;
const uint_fast16_t FILE_SHARE_DENY_WRITE = 0x2000;

#endif
