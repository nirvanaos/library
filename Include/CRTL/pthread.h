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
#ifndef _PTHREAD_H_
#define _PTHREAD_H_
#pragma once

#include "sys/types.h"

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

int pthread_create (pthread_t *restrict, const pthread_attr_t *restrict,
          void *(*)(void*), void *restrict);
int pthread_join (pthread_t thread, void** value_ptr);

void *pthread_getspecific (pthread_key_t);
int pthread_key_create (pthread_key_t*, void (*)(void*));
int pthread_key_delete (pthread_key_t);
int pthread_setspecific (pthread_key_t, const void *);

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
