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

/*
#define PTHREAD_COND_INITIALIZER NULL
#define PTHREAD_MUTEX_INITIALIZER NULL
#define PTHREAD_MUTEX_NORMAL 0
#define PTHREAD_MUTEX_ERRORCHECK 0
#define PTHREAD_MUTEX_RECURSIVE  0
#define PTHREAD_MUTEX_DEFAULT 0

#define PTHREAD_MUTEX_STALLED 0
#define PTHREAD_MUTEX_ROBUST  0

#define PTHREAD_CREATE_DETACHED 1
#define PTHREAD_CREATE_JOINABLE 0
*/
#define PTHREAD_RWLOCK_INITIALIZER 0

#ifdef __cplusplus
extern "C" {
#define restrict
#endif

typedef int pthread_t;
typedef int pthread_attr_t;

/*
int pthread_attr_destroy (pthread_attr_t*);
int pthread_attr_getdetachstate (const pthread_attr_t*, int*);
int pthread_attr_init (pthread_attr_t*);
int pthread_attr_setdetachstate (pthread_attr_t*, int);
int pthread_create (pthread_t *restrict, const pthread_attr_t *restrict,
          void *(*)(void*), void *restrict);
int pthread_detach (pthread_t);
int pthread_join (pthread_t thread, void** value_ptr);
*/

typedef unsigned int pthread_key_t;

void *pthread_getspecific (pthread_key_t);
int pthread_key_create (pthread_key_t*, void (*)(void*));
int pthread_key_delete (pthread_key_t);
int pthread_setspecific (pthread_key_t, const void *);

/*

typedef int pthread_mutex_t;

inline int pthread_mutex_destroy (pthread_mutex_t*)
{
  return 0;
}

typedef int pthread_mutexattr_t;

inline int pthread_mutex_init (pthread_mutex_t* restrict mutex, const pthread_mutexattr_t* restrict)
{
  *mutex = 0;
  return 0;
}

inline int pthread_mutex_lock (pthread_mutex_t*)
{
  return 0;
}

inline int pthread_mutex_trylock (pthread_mutex_t*)
{
  return 0;
}

inline int pthread_mutex_unlock (pthread_mutex_t* mutex)
{
  return 0;
}

inline int pthread_mutexattr_destroy (pthread_mutexattr_t*)
{
  return 0;
}

inline int pthread_mutexattr_getrobust (const pthread_mutexattr_t* restrict, int* restrict robust)
{
  *robust = PTHREAD_MUTEX_STALLED;
  return 0;
}

inline int pthread_mutexattr_gettype (const pthread_mutexattr_t* restrict, int* restrict type)
{
  *type = PTHREAD_MUTEX_DEFAULT;
  return 0;
}

inline int pthread_mutexattr_init (pthread_mutexattr_t* attr)
{
  *attr = 0;
  return 0;
}

inline int pthread_mutexattr_setrobust (pthread_mutexattr_t*, int)
{
  return 0;
}

inline int pthread_mutexattr_settype (pthread_mutexattr_t*, int)
{
  return 0;
}

*/

typedef int pthread_rwlock_t;

#ifdef __cplusplus
#undef restrict
}
#endif

#endif
