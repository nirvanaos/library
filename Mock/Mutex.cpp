/*
* Nirvana mock library.
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
#include "Mutex.h"

namespace Nirvana {
namespace Test {

#ifdef _WIN32

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

class Mutex::Implementation
{
public:
	Implementation ()
	{
		InitializeCriticalSection (&cs_);
	}

	~Implementation ()
	{
		DeleteCriticalSection (&cs_);
	}

	void lock ()
	{
		EnterCriticalSection (&cs_);
	}

	void unlock ()
	{
		LeaveCriticalSection (&cs_);
	}

private:
	CRITICAL_SECTION cs_;
};

#else

#include <pthread.h>

class Mutex::Implementation
{
public:
	Implementation ()
	{
		pthread_mutex_init (&mutex_, nullptr);
	}

	~Implementation ()
	{
		pthread_mutex_destroy (&mutex_);
	}

	void lock ()
	{
		pthread_mutex_lock (&mutex_);
	}

	void unlock ()
	{
		pthread_mutex_unlock (&mutex_);
	}

private:
	pthread_mutex_t mutex_;
};

#endif

Mutex::Mutex () :
	impl_ (new Implementation)
{}

Mutex::~Mutex ()
{
	delete impl_;
}

void Mutex::lock ()
{
	impl_->lock ();
}

void Mutex::unlock ()
{
	impl_->unlock ();
}

}
}
