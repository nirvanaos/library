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
#ifndef _SYS_RANDOM_H
#define _SYS_RANDOM_H
#pragma once

#include <sys/types.h>

/* getrandom flags */
#define GRND_NONBLOCK	1
#define GRND_RANDOM	2

#ifdef __cplusplus
extern "C" {
#endif

ssize_t getrandom (void*, size_t, unsigned int);
int getentropy (void*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* _SYS_RANDOM_H */
